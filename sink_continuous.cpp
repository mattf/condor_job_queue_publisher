/***************************************************************
 *
 * Copyright (C) 2010 Red Hat, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Session.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>

using namespace std;
using namespace qpid::messaging;
using namespace qpid::types;

void
usage(char *argv[])
{
	cerr <<	"usage: " << argv[0]
		 << " --address <queue or topic> "
		 << "[--broker <broker url>]"
		 << endl;
	exit(1);
}

int
slurp_events(Session &session, Receiver& receiver, bool all=false)
{

	int count = 0;
	Message message;
	while (1) {
		try {
			message = receiver.fetch(Duration::SECOND * 1);
		} catch (const NoMessageAvailable& msg) {
			return count;
		}
		Variant::Map content;
		decode(message, content);

		string id = message.getSubject();
		cout << id << endl;

		if (!all && "DONE" == id) {
			break;
		}

		for (qpid::types::Variant::Map::const_iterator i = content.begin();
				content.end() != i; i++) {
			cout << id << " " << (*i).first << " " << (*i).second << endl;
		}
		session.acknowledge();
		count++;
	}
	session.acknowledge();

	return count;
}

pid_t
launch_publisher(const string &address, const string &broker, int interval)
{
	stringstream ss;
	ss << "./condor_job_queue_publisher "
		"--file job_queue.log.short.gen --broker " <<
		broker << " --address " << address <<
		" --interval " << interval <<
		" --dump 2";
	pid_t pid = fork();
	if (pid == 0) {
		execl("/bin/sh", "sh", "-c", ss.str().c_str(), (char *)0);
		exit(1);
	} else if (pid < 0) {
		exit(3);
	}
	return pid;
}

void
write_events1()
{
	unlink("job_queue.log.short.gen");
	ofstream outfile("job_queue.log.short.gen");
	outfile << "107 28 CreationTimestamp 1325554373" << endl
		<< "101 01.-1 Job Machine" << endl
		<< "103 01.-1 Args \"1m\"" << endl
		<< "103 01.-1 BufferBlockSize 32768" << endl
		<< "103 01.-1 BufferSize 524288" << endl
		<< "103 01.-1 ClusterId 1" << endl
		<< "103 01.-1 Cmd \"/bin/sleep\"" << endl
		<< "101 1.1 Job Machine" << endl
		<< "103 1.1 AutoCluster 3" << endl
		<< "103 1.1 JobRunCount 1" << endl
		<< "103 1.1 ProcId 1" << endl
		<< "103 1.1 LastRejMatchTime 1325554373" << endl
		<< "101 1.2 Job Machine" << endl
		<< "103 1.2 AutoCluster 3" << endl
		<< "103 1.2 JobRunCount 1" << endl
		<< "103 1.2 ProcId 2" << endl
		<< "103 1.2 LastRejMatchTime 1325554373" << endl;
	outfile.flush();
	outfile.close();
}

void
write_events2()
{
	ofstream outfile("job_queue.log.short.gen", fstream::app);
	outfile << "101 1.3 Job Machine" << endl
		<< "103 1.3 AutoCluster 3" << endl
		<< "103 1.3 JobRunCount 1" << endl
		<< "103 1.3 ProcId 3" << endl
		<< "103 1.3 LastRejMatchTime 1325550106" << endl;
	outfile.flush();
	outfile.close();
}

int
main(int argc, char *argv[])
{
	string broker = "amqp:tcp:127.0.0.1:5672";
	string address;

	static struct option options[] = {
		{"broker", 1, NULL, 'b'},
		{"address", 1, NULL, 'a'},
		{NULL, 0, NULL, 0}
	};

	int c;
	while (1) {
		c = getopt_long(argc, argv, ":b:a:", options, NULL);
		if (-1 == c) break;

		switch (c) {
		case 'b':
			broker = optarg;
			break;
		case 'a':
			address = optarg;
			break;
		case ':':
			cerr << argv[optind - 1] << " requires an argument" << endl;
			usage(argv);
			break;
		case '?':
			cerr << "unknown argument: " << argv[optind - 1] << endl;
			usage(argv);
			break;
		default:
			usage(argv);
			break;
		}
	}

	if (address.empty()) {
		cerr << "--address required" << endl;
		usage(argv);
	}

	cout << "config: address = " << address << "; broker = " << broker << endl;

	Connection connection(broker);
	connection.open();
	Session session = connection.createSession();
	Receiver receiver = session.createReceiver(address);
	receiver.setCapacity(1024);

	// Write out a few events.
	write_events1();

	// Clear out the queue.
	int count = slurp_events(session, receiver, true);

	// Launch job publisher.
	pid_t pid = launch_publisher(address, broker, 15);

	// Pull in all the events.
	cout << "Reading the first set of events" << endl;
	count = slurp_events(session, receiver);
	if (count != 2) {
		cout << "Incorrect number of events returned (" << count << ")." << endl;
		kill(pid, SIGKILL);
		return 4;
	}

	cout << "Got first set of events, writing more." << endl;
	// Write out a few new events.
	time_t t = time(NULL);
	write_events2();

	// See if there's some more events.
	count = slurp_events(session, receiver);
	if (count != 1) {
		cout << "Incorrect number of events returned " << count << endl;
		kill(pid, SIGKILL);
		return 5;
	}
	t = t - time(NULL);

	kill(pid, SIGKILL);
	wait();

	if (t > 5) {
		return 1;
	} else {
		return 0;
	}
}
