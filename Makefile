FILES=JobLogReader.o classadlogentry.o prober.o classadlogparser.o Globals.o job_publisher_main.o JobPublisherJobLogConsumer.o Job.o Utils.o test_DetectType.o test_Job.o

all: $(FILES)

clean:
	rm -f $(FILES)
	rm -f test_DetectType
	rm -f test_Job

test_DetectType: test_DetectType.o Utils.o
	g++ -g -o $@ $^

test_Job: test_Job.o Utils.o Job.o
	g++ -g -o $@ $^
