#!/bin/sh

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

ADDRESS=$1

FAIL=0

FILE=job_queue.log.gen

rm -f $FILE
sh ./job_queue.log.sh $FILE

CREATE_COUNT=$(grep ^101 $FILE | wc -l)
DESTROY_COUNT=$(grep ^102 $FILE | wc -l)

./job_queue_publisher --dump --file $FILE $ADDRESS > $FILE.tmp 2>&1

TOTAL=$(grep "Total number of jobs" $FILE.tmp | sed 's/.*Total.*: \(.*\)/\1/')

if [ $((CREATE_COUNT - DESTROY_COUNT)) -eq $TOTAL ]; then
   echo "SUCCESS: $CREATE_COUNT - $DESTROY_COUNT = $TOTAL"
else
   echo "FAIL: $CREATE_COUNT - $DESTROY_COUNT != $TOTAL"
   FAIL=1
fi

# Extract all attributes for all jobs, then remove the deleted jobs
grep ^103 job_queue.log.gen | colrm 1 4 | sort > $FILE.tmp2
for j in $(grep ^102 job_queue.log.gen | colrm 1 4); do
   grep -v ^"$j " $FILE.tmp2 > $FILE.tmp2.tmp
   mv -f $FILE.tmp2.tmp $FILE.tmp2
done

# Extract the attributes from the job_queue_publisher --dump
grep -- -\> $FILE.tmp | sed 's/.*-> \(.*\)/\1/' | sort > $FILE.tmp3

diff -u $FILE.tmp2 $FILE.tmp3
if [ $? -eq 0 ]; then
   echo "SUCCESS: job_queue_publisher represents $FILE"
   rm -f $FILE.tmp2 $FILE.tmp3
else
   echo "FAIL: ^^ difference between job_queue_publisher memory and $FILE"
   FAIL=1
fi

if [ ! -z "$ADDRESS" ]; then
   ./sink $ADDRESS | grep -v ^config | sort > sink.out

   grep "=>" $FILE.tmp | sed 's/.*=> \(.*\)/\1/' | grep -v ^0 | sort > job_queue_publisher.sink.out

   for j in $(grep ^102 job_queue.log.gen | colrm 1 4); do
      grep -v ^"$j " sink.out > sink.out.tmp
      mv -f sink.out.tmp sink.out
   done

   diff -u sink.out job_queue_publisher.sink.out
   if [ $? -eq 0 ]; then
      echo "SUCCESS: sink agrees with job_queue_publisher"
      rm -f $FILE.tmp sink.out job_queue_publisher.sink.out
   else
      echo "FAIL: ^^ difference between sink and job_queue_publisher"
      FAIL=1
   fi
fi

cat >> job_queue.log.gen-tiny << EOF
107 28 CreationTimestamp 1266722428
101 1.0 Job Machine
103 1.0 Attr3 "Child"
101 01.-1 Job Machine
103 01.-1 Attr0 "Parent"
103 01.-1 Attr1 "Parent"
103 1.0 Attr1 "Child"
EOF

./job_queue_publisher --file job_queue.log.gen-tiny --dump > job_queue_publisher.out-tiny 2>&1
grep "=> 1.0 Attr1 \"Child\"" job_queue_publisher.out-tiny > /dev/null 2>&1
if [ $? -eq 0 ]; then
   echo "SUCCESS: job_queue_publisher properly overrides parent attributes"
   rm -f job_queue_publisher.out-tiny job_queue.log.gen-tiny
else
   echo "FAIL: job_queue_publisher does not override parent attributes in children"
   FAIL=1
fi

exit $FAIL
