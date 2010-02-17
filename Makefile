CXXFLAGS=-g

LIBRARY=libclassadlogreader.so.1.0.0
LIBRARY_OBJS=JobLogReader-pic.o classadlogentry-pic.o prober-pic.o classadlogparser-pic.o

JOB_PUBLISHER=job_publisher
JOB_PUBLISHER_OBJS=job_publisher_main.o JobPublisherJobLogConsumer.o Utils.o Job.o Globals.o
JOB_PUBLISHER_LIBS=$(LIBRARY)

TEST_DETECTTYPE=test_DetectType
TEST_DETECTTYPE_OBJS=test_DetectType.o Utils.o

TEST_JOB=test_Job
TEST_JOB_OBJS=test_Job.o Utils.o Job.o

%-pic.o: %.cpp
	gcc -fpic -c $< -o $@

all: $(LIBRARY) $(JOB_PUBLISHER) $(TEST_DETECTTYPE) $(TEST_JOB)

clean:
	rm -f $(LIBRARY) $(LIBRARY_OBJS)
	rm -f $(JOB_PUBLISHER) $(JOB_PUBLISHER_OBJS)
	rm -f $(TEST_DETECTTYPE) $(TEST_DETECTTYPE_OBJS)
	rm -f $(TEST_JOB) $(TEST_JOB_OBJS)

$(LIBRARY): $(LIBRARY_OBJS)
	gcc -g -shared -Wl,-soname,$@ -o $@ $^

$(JOB_PUBLISHER): $(JOB_PUBLISHER_OBJS) $(LIBRARY)
	g++ $(LDFLAGS) -o $@ $^ $(JOB_PUBLISHER_LIBS)

$(TEST_DETECTTYPE): $(TEST_DETECTTYPE_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^

$(TEST_JOB): $(TEST_JOB_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^
