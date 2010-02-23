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

CXXFLAGS=-g
LDFLAGS=-pthread

JOB_PUBLISHER=job_publisher
JOB_PUBLISHER_OBJS=job_publisher_main.o JobPublisherClassAdLogConsumer.o Job.o Globals.o Config.o ClassAdLogReader.o ClassAdLogEntry.o ClassAdLogProber.o ClassAdLogParser.o
JOB_PUBLISHER_LIBS=-lqpidcommon -lqpidclient

TEST_JOB=test_Job
TEST_JOB_OBJS=test_Job.o Job.o
TEST_JOB_LIBS=

MEMORY_PERFORMANCE=memory_performance
MEMORY_PERFORMANCE_OBJS=memory_performance.o

SINK=sink
SINK_OBJS=sink.o
SINK_LIBS=-lqpidcommon -lqpidclient

all: $(JOB_PUBLISHER) $(TEST_JOB) $(MEMORY_PERFORMANCE) $(SINK)

clean:
	rm -f $(JOB_PUBLISHER) $(JOB_PUBLISHER_OBJS)
	rm -f $(TEST_JOB) $(TEST_JOB_OBJS)
	rm -f $(MEMORY_PERFORMANCE) $(MEMORY_PERFORMANCE_OBJS)
	rm -f $(SINK) $(SINK_OBJS)

$(JOB_PUBLISHER): $(JOB_PUBLISHER_OBJS)
	g++ $(LDFLAGS) -o $@ $^ $(JOB_PUBLISHER_LIBS)

$(TEST_JOB): $(TEST_JOB_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^ $(TEST_JOB_LIBS)

$(MEMORY_PERFORMANCE): $(MEMORY_PERFORMANCE_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^

$(SINK): $(SINK_OBJS)
	g++ $(LDFLAGS) -g -o $@ $^ $(SINK_LIBS)

