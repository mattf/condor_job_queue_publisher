AUTOMAKE_OPTIONS = foreign
## top directory
bin_PROGRAMS = condor_job_queue_publisher test_Job sink memory_performance

condor_job_queue_publisher_SOURCES = \
	job_queue_publisher_main.cpp \
	JobQueuePublisherClassAdLogConsumer.cpp \
	Job.cpp \
	Globals.cpp \
	Config.cpp \
	CondorKeepAlive.cpp \
	lib/ClassAdLogReader.cpp \
	lib/ClassAdLogEntry.cpp \
	lib/ClassAdLogProber.cpp \
	lib/ClassAdLogParser.cpp \
	lib/log.cpp \
	JobQueuePublisherClassAdLogConsumer.h \
	Job.h \
	Globals.h \
	Config.h \
	CondorKeepAlive.h \
	lib/ClassAdLogReader.h \
	lib/ClassAdLogEntry.h \
	lib/ClassAdLogProber.h \
	lib/ClassAdLogParser.h \
	lib/log.h

condor_job_queue_publisher_LDFLAGS=-lqpidmessaging -lqpidtypes -lpthread

memory_performance_SOURCES = \
	memory_performance.cpp

memory_performance_LDFLAGS = -lpthread

test_Job_SOURCES = \
	test_Job.cpp \
	Job.cpp \
	Job.h

test_Job_LDFLAGS = -lpthread

sink_SOURCES = \
	sink.cpp \
	Job.cpp \
	Job.h

sink_LDFLAGS = -lqpidmessaging -lqpidtypes -lpthread

AM_CXXFLAGS = -Ilib -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -Werror -D_NO_CONDOR_ 

EXTRA_DIST = autogen.sh condor-job-queue-publisher.spec

dist_pkgdata_SCRIPTS = test_job_queue_publisher.sh job_queue.log.sh

test: sink condor_job_queue_publisher test_Job
	@echo "NOTE: You better have a broker running with a queue named jp-test-queue"
	@echo " Try: service qpidd start && qpid-config add queue jp-test-queue"
	./test_Job
	sh ./test_job_queue_publisher.sh --address=jp-test-queue

