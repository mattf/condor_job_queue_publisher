FILES=JobLogReader.o classadlogentry.o prober.o classadlogparser.o Globals.o job_publisher_main.o JobPublisherJobLogConsumer.o Job.o

all: $(FILES)

clean:
	rm -f $(FILES)
