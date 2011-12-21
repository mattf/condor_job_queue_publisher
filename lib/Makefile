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

CXXFLAGS=-g -D_NO_CONDOR_ -O2

LIB=libclassadlogreader.a
LIB_OBJS=ClassAdLogReader.o ClassAdLogEntry.o ClassAdLogProber.o ClassAdLogParser.o log.o

all: $(LIB)

clean:
	rm -f $(LIB) $(LIB_OBJS)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^


