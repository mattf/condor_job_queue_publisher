#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <getopt.h>

#include <syslog.h>

#include "JobLogReader.h"
#include "JobPublisherJobLogConsumer.h"

#include "Globals.h"

void Dump();

void
usage(char *argv[])
{
	syslog(LOG_ERR,
		   "usage: %s "
		   "--file <job_queue.log> "
		   "[--host <broker host>] "
		   "[--port <broker port>]\n",
		   argv[0]);
	exit(1);
}

void
parse_args(int argc, char *argv[],
		   const char *&host, int &port, const char *&file)
{
	static struct option options[] = {
		{"host", 1, NULL, 'h'},
		{"port", 1, NULL, 'p'},
		{"file", 1, NULL, 'f'},
		{NULL, NULL, NULL, NULL}
	};

	int c;
	while (1) {
		c = getopt_long(argc, argv, ":h:p:f:", options, NULL);
		if (-1 == c) break;

		switch (c) {
		case 'h':
			host = optarg;
			break;
		case 'p':
			port = (int) strtol(optarg, NULL, 10);
			if (0 == port) {
				syslog(LOG_ERR, "invalid port: %s\n", optarg);
				usage(argv);
			}
			break;
		case 'f':
			file = optarg;
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

	if (!file) {
		syslog(LOG_ERR, "--file required\n");
		usage(argv);
	}

	if (optind < argc) {
		syslog(LOG_ERR, "unknown arguments: ");
		while (optind < argc) {
			syslog(LOG_ERR, "%s ", argv[optind++]);
		}
		usage(argv);
	}
}

int main(int argc, char *argv[])
{
	const char *file = NULL;
	const char *host = "localhost";
	int port = 5672;

	openlog("job_publisher", LOG_PID|LOG_PERROR, LOG_DAEMON);

	parse_args(argc, argv, host, port, file);

	syslog(LOG_INFO, "config -- host = %s; port: %d; file: %s\n",
		   host, port, file);

//	closelog();
//	openlog("job_publisher", LOG_PID, LOG_DAEMON);

	JobPublisherJobLogConsumer *consumer = new JobPublisherJobLogConsumer();
	JobLogReader *reader = new JobLogReader(consumer);

	reader->SetJobLogFileName(file);

//	reader->Poll();

	Dump();

	delete reader;

	return 0;
}

void
Dump()
{
	syslog(LOG_DEBUG, "***BEGIN DUMP***");
	syslog(LOG_DEBUG, "Total number of jobs: %d", g_jobs.size());
//	for (JobCollectionType::const_iterator i = g_jobs.begin();
//		 g_jobs.end() != i;
//		 i++) {
//		printf("%s %s\n", (*i).first.c_str(), (*i).second.GetKey().c_str());
//	}
	syslog(LOG_DEBUG, "***END DUMP***");
}
