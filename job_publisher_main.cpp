#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <getopt.h>

#include "JobLogReader.h"
#include "JobPublisherJobLogConsumer.h"

#include "Globals.h"

void Dump();

void
usage(char *argv[])
{
	printf("usage: %s "
		   "--file <job_queue.log> "
		   "[--host <broker host>] "
		   "[--port <broker port>]\n",
		   argv[0]);
	exit(1);
}

void
parse_args(int argc, char *argv[], string &host, int &port, string &file)
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
				printf("invalid port: %s\n", optarg);
				usage(argv);
			}
			break;
		case 'f':
			file = optarg;
			break;
		case ':':
			printf("%s requires an argument\n", argv[optind - 1]);
			usage(argv);
			break;
		case '?':
			printf("unknown argument: %s\n", argv[optind - 1]);
			usage(argv);
			break;
		default:
			usage(argv);
			break;
		}
	}

	if (file.empty()) {
		printf("--file required\n");
		usage(argv);
	}

	if (optind < argc) {
		printf("unknown arguments: ");
		while (optind < argc) {
			printf("%s ", argv[optind++]);
		}
		printf("\n");
		usage(argv);
	}
}

int main(int argc, char *argv[])
{
	string file;
	string host = "localhost";
	int port = 5672;

	parse_args(argc, argv, host, port, file);

	printf("config -- host = %s; port: %d; file: %s\n",
		   host.c_str(), port, file.c_str());

	JobPublisherJobLogConsumer *consumer = new JobPublisherJobLogConsumer();
	JobLogReader *reader = new JobLogReader(consumer);

	reader->SetJobLogFileName(file);

//	reader->Poll();

	Dump();

	return 0;
}

void Stop()
{
	Dump();

	consumer->Reset();

//	mirror->stop();

	delete reader; reader = NULL;

	exit(0);
}

//-------------------------------------------------------------

int main_shutdown_fast()
{
	printf("main_shutdown_fast() called\n");

	Stop();

	exit(0);
	return 0;
}

//-------------------------------------------------------------

int main_shutdown_graceful()
{
	printf("main_shutdown_graceful() called\n");

	Stop();

	exit(0);
	return 0;
}


void
Dump()
{
	printf("***BEGIN DUMP***\n");
	printf("Total number of jobs: %d\n", g_jobs.size());
//	for (JobCollectionType::const_iterator i = g_jobs.begin();
//		 g_jobs.end() != i;
//		 i++) {
//		printf("%s %s\n", (*i).first.c_str(), (*i).second.GetKey().c_str());
//	}
	printf("***END DUMP***\n");
}
