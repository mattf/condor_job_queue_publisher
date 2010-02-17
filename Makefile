FILES=JobLogReader.o classadlogentry.o prober.o classadlogparser.o Globals.o job_publisher_main.o JobPublisherJobLogConsumer.o Job.o Utils.o detect_type.o

all: $(FILES)

clean:
	rm -f $(FILES)
	rm -f detect_type
	rm -f test_Job

detect_type: detect_type.o Utils.o
	g++ -g -o $@ $^

test_Job: test_Job.o Utils.o Job.o
	g++ -g -o $@ $^
