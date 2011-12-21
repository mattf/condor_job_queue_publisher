#/***************************************************************
# *
# * Copyright (C) 2010 Red Hat, Inc.
# * 
# * Licensed under the Apache License, Version 2.0 (the "License"); you
# * may not use this file except in compliance with the License.  You may
# * obtain a copy of the License at
# * 
# *    http://www.apache.org/licenses/LICENSE-2.0
# * 
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
# ***************************************************************/

STD_CXXFLAGS= -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Werror
CXXFLAGS=$(STD_CXXFLAGS) -D_NO_CONDOR_ -Ilib
LDFLAGS=-pthread

JOB_QUEUE_PUBLISHER=job_queue_publisher
JOB_QUEUE_PUBLISHER_OBJS=job_queue_publisher_main.o JobQueuePublisherClassAdLogConsumer.o Job.o Globals.o Config.o CondorKeepAlive.o lib/libclassadlogreader.a
JOB_QUEUE_PUBLISHER_LIBS=-lqpidcommon -lqpidclient -lqpidtypes -lqpidmessaging

TEST_JOB=test_Job
TEST_JOB_OBJS=test_Job.o Job.o
TEST_JOB_LIBS=

MEMORY_PERFORMANCE=memory_performance
MEMORY_PERFORMANCE_OBJS=memory_performance.o

SINK=sink
SINK_OBJS=sink.o
SINK_LIBS=-lqpidcommon -lqpidclient -lqpidtypes -lqpidmessaging

all: $(JOB_QUEUE_PUBLISHER) $(TEST_JOB) $(MEMORY_PERFORMANCE) $(SINK)

clean:
	(cd lib; make clean)
	rm -f $(JOB_QUEUE_PUBLISHER) $(JOB_QUEUE_PUBLISHER_OBJS)
	rm -f $(TEST_JOB) $(TEST_JOB_OBJS)
	rm -f $(MEMORY_PERFORMANCE) $(MEMORY_PERFORMANCE_OBJS)
	rm -f $(SINK) $(SINK_OBJS)

test: $(TEST_JOB) $(JOB_QUEUE_PUBLISHER) $(SINK)
	@echo "NOTE: You better have a broker running with a queue named jp-test-queue"
	@echo " Try: service qpidd start && qpid-config add queue jp-test-queue"
	./$(TEST_JOB)
	sh ./test_job_queue_publisher.sh --address=jp-test-queue

$(JOB_QUEUE_PUBLISHER): $(JOB_QUEUE_PUBLISHER_OBJS)
	g++ $(LDFLAGS) -o $@ $^ $(JOB_QUEUE_PUBLISHER_LIBS)

$(TEST_JOB): $(TEST_JOB_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^ $(TEST_JOB_LIBS)

$(MEMORY_PERFORMANCE): $(MEMORY_PERFORMANCE_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^

$(SINK): $(SINK_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^ $(SINK_LIBS)

lib/libclassadlogreader.a: lib/Makefile
	(cd lib; make)
