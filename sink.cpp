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

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

using namespace std;
using namespace qpid::messaging;
using namespace qpid::types;

void
usage(char *argv[])
{
	cerr <<	"usage: " << argv[0]
		 << "--address <queue or topic> "
		 << "[--broker <broker url>]"
		 << endl;
	exit(1);
}

int
main(int argc, char *argv[])
{
	string broker = "amqp:tcp:127.0.0.1:5672";
	string address;

	static struct option options[] = {
		{"broker", 1, NULL, 'b'},
		{"address", 1, NULL, 'a'},
		{NULL, NULL, NULL, NULL}
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

	int count = 0;
	while (1) {
		Message message = receiver.fetch();
		Variant::Map content;
		decode(message, content);

		string id = message.getSubject();

		if ("DONE" == id) {
			session.acknowledge();
			break;
		}

		for (qpid::types::Variant::Map::const_iterator i = content.begin();
			 content.end() != i;
			 i++) {
			cout << id << " " << (*i).first << " " << (*i).second << endl;
		}
		if (!(++count % 128)) session.acknowledge();
	}
	session.acknowledge();

	receiver.close();
	connection.close();

	return 0;
}
