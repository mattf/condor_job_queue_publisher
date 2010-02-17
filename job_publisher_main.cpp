#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "JobLogReader.h"
#include "JobPublisherJobLogConsumer.h"

#include "Globals.h"

JobLogReader *reader;
JobPublisherJobLogConsumer *consumer;

void Dump();

int main_config(bool is_full);


int main(int /* argc */, char * /* argv */ [])
{
	printf("main() called\n");

	consumer = new JobPublisherJobLogConsumer();

	reader = new JobLogReader(consumer);

	main_config(true);

	const char *host = "localhost";
	int port = 5672;

//	port = param_integer("QMF_BROKER_PORT", 5672);
//	if (NULL == (host = param("QMF_BROKER_HOST"))) {
//		host = strdup("localhost");
//	}

	return 0;
}

//-------------------------------------------------------------

int 
main_config( bool /* is_full */ )
{
	printf("main_config() called\n");

	reader->SetJobLogFileName("XXX");

	return 0;
}

//-------------------------------------------------------------

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
	printf("***END DUMP***\n");
}
