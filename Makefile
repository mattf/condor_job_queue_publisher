CXXFLAGS=-g
LDFLAGS=-pthread

JOB_PUBLISHER=job_publisher
JOB_PUBLISHER_OBJS=job_publisher_main.o JobPublisherJobLogConsumer.o Job.o Globals.o JobLogReader.o ClassAdLogEntry.o prober.o ClassAdLogParser.o
JOB_PUBLISHER_LIBS=-lqpidcommon -lqpidclient

TEST_DETECTTYPE=test_DetectType
TEST_DETECTTYPE_OBJS=test_DetectType.o Utils.o

TEST_JOB=test_Job
TEST_JOB_OBJS=test_Job.o Job.o
TEST_JOB_LIBS=

MEMORY_PERFORMANCE=memory_performance
MEMORY_PERFORMANCE_OBJS=memory_performance.o

SINK=sink
SINK_OBJS=sink.o
SINK_LIBS=-lqpidcommon -lqpidclient

all: $(JOB_PUBLISHER) $(TEST_DETECTTYPE) $(TEST_JOB) $(MEMORY_PERFORMANCE) $(SINK)

clean:
	rm -f $(JOB_PUBLISHER) $(JOB_PUBLISHER_OBJS)
	rm -f $(TEST_DETECTTYPE) $(TEST_DETECTTYPE_OBJS)
	rm -f $(TEST_JOB) $(TEST_JOB_OBJS)
	rm -f $(MEMORY_PERFORMANCE) $(MEMORY_PERFORMANCE_OBJS)
	rm -f $(SINK) $(SINK_OBJS)

$(JOB_PUBLISHER): $(JOB_PUBLISHER_OBJS)
	g++ $(LDFLAGS) -o $@ $^ $(JOB_PUBLISHER_LIBS)

$(TEST_DETECTTYPE): $(TEST_DETECTTYPE_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^

$(TEST_JOB): $(TEST_JOB_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^ $(TEST_JOB_LIBS)

$(MEMORY_PERFORMANCE): $(MEMORY_PERFORMANCE_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^

$(SINK): $(SINK_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^ $(SINK_LIBS)

