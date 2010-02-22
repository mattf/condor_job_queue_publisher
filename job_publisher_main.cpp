#include <iostream>

#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <getopt.h>

#include <syslog.h>

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/MapContent.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>

#include "JobLogReader.h"
#include "JobPublisherJobLogConsumer.h"

#include "Globals.h"

#include "Config.h"

using namespace qpid::messaging;

void Dump();

void PublishJob(const string &key);

Sender sender;

void
usage(char *argv[])
{
	syslog(LOG_ERR,
		   "usage: %s "
		   "--file <job_queue.log> "
		   "[--broker <broker url>] "
		   "[--address <queue or topic>]\n",
		   argv[0]);
	exit(1);
}

void
parse_args(int argc, char *argv[], Config &config)
{
	static struct option options[] = {
		{"file", 1, NULL, 'f'},
		{"broker", 1, NULL, 'b'},
		{"address", 1, NULL, 'a'},
		{"dump", 0, NULL, 'd'},
		{NULL, NULL, NULL, NULL}
	};

	int c;
	while (1) {
		c = getopt_long(argc, argv, ":f:b:p:a:d", options, NULL);
		if (-1 == c) break;

		switch (c) {
		case 'f':
			config.file = optarg;
			break;
		case 'b':
			config.broker = optarg;
			break;
		case 'a':
			config.address = optarg;
			break;
		case 'd':
			config.dump = true;
			break;
		case ':':
			syslog(LOG_ERR, "%s requires an argument\n", argv[optind - 1]);
			usage(argv);
			break;
		case '?':
			syslog(LOG_ERR, "unknown argument: %s\n", argv[optind - 1]);
			usage(argv);
			break;
		default:
			usage(argv);
			break;
		}
	}

	if (config.file.empty()) {
		syslog(LOG_ERR, "--file required\n");
		usage(argv);
	}

//	if (optind < argc) {
//		syslog(LOG_ERR, "unknown arguments: ");
//		while (optind < argc) {
//			syslog(LOG_ERR, "%s ", argv[optind++]);
//		}
//		usage(argv);
//	}
}

int main(int argc, char *argv[])
{
	Connection connection;
	Session session;

	Config config;
	config.broker = "amqp:tcp:127.0.0.1:5672";

	openlog("job_publisher", LOG_PID|LOG_PERROR, LOG_DAEMON);

	parse_args(argc, argv, config);

	syslog(LOG_INFO, "config -- file = %s; broker = %s; address: %s\n",
		   config.file.c_str(),
		   config.broker.c_str(),
		   config.address.c_str());

	if (!config.address.empty()) {
		connection.open(config.broker);
		session = connection.newSession();
		sender = session.createSender(config.address);
	}

//	closelog();
//	openlog("job_publisher", LOG_PID, LOG_DAEMON);

	JobPublisherJobLogConsumer *consumer = new JobPublisherJobLogConsumer();
	JobLogReader *reader = new JobLogReader(consumer);

	reader->SetJobLogFileName(config.file.c_str());

	while (1) {
		reader->Poll();

		cout << "Dirty: ";
		for (JobSetType::const_iterator i = g_dirty_jobs.begin();
			 g_dirty_jobs.end() != i;
			 i++) {
			std::cout << (*i) << " ";
			PublishJob((*i));
			g_dirty_jobs.erase(i);
		}
		cout << endl;

		cout << "Delete: ";
		for (JobSetType::const_iterator i = g_delete_jobs.begin();
			 g_delete_jobs.end() != i;
			 i++) {
			std::cout << (*i) << " ";
			PublishJob((*i));
			g_jobs.erase((*i));
			g_delete_jobs.erase(i);
		}
		cout << endl;

		if (config.dump) {
			Dump();
			consumer->Reset();
			break;
		}

		sleep(5);
	}

	delete reader;

	if (!config.address.empty()) {
		session.sync();
		connection.close();
	}

	return 0;
}

void
Dump()
{
	syslog(LOG_DEBUG, "***BEGIN DUMP***");
	syslog(LOG_DEBUG, "Total number of jobs: %d", g_jobs.size());
/*
	string jobs = "Jobs: ";
	for (JobCollectionType::const_iterator i = g_jobs.begin();
		 g_jobs.end() != i;
		 i++) {
		jobs += (*i).first;
		jobs += " ";
	}
	syslog(LOG_DEBUG, "%s", jobs.c_str());
*/
	string line;
	for (JobCollectionType::const_iterator i = g_jobs.begin();
		 g_jobs.end() != i;
		 i++) {
		for (Job::AttributeMapType::const_iterator j = (*i).second.GetAttributes().begin();
			 (*i).second.GetAttributes().end() != j;
			 j++) {
			line = (*i).first;
			line += " ";
			line += (*j).first;
			line += " ";
			line += (*j).second;
			syslog(LOG_DEBUG, "-> %s", line.c_str());
		}
	}
	syslog(LOG_DEBUG, "***END DUMP***");
}

void
PublishJob(const string &key)
{
	cout << "Publish: " << key << endl;

	Message message;
	MapContent content(message);

	JobCollectionType::const_iterator job = g_jobs.find(key);
	assert(g_jobs.end() != job);

	for (Job::AttributeMapType::const_iterator attr =
			 (*job).second.GetAttributes().begin();
		 (*job).second.GetAttributes().end() != attr;
		 attr++) {
		content[(*attr).first] = (*attr).second;
	}

	content.encode();
	sender.send(message);
}
