#!/bin/sh

ADDRESS=$1

FAIL=0

FILE=job_queue.log.gen

rm -f $FILE
sh ./job_queue.log.sh $FILE

CREATE_COUNT=$(grep ^101 $FILE | wc -l)
DESTROY_COUNT=$(grep ^102 $FILE | wc -l)

./job_publisher --dump --file $FILE $ADDRESS > $FILE.tmp 2>&1

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

# Extract the attributes from the job_publisher --dump
grep -- -\> $FILE.tmp | sed 's/.*-> \(.*\)/\1/' | sort > $FILE.tmp3

diff -u $FILE.tmp2 $FILE.tmp3
if [ $? -eq 0 ]; then
   echo "SUCCESS: job_publisher represents $FILE"
   rm -f $FILE.tmp2 $FILE.tmp3
else
   echo "FAIL: ^^ difference between job_publisher memory and $FILE"
   FAIL=1
fi

if [ ! -z "$ADDRESS" ]; then
   ./sink $ADDRESS | grep -v ^config | sort > sink.out

   grep "=>" $FILE.tmp | sed 's/.*=> \(.*\)/\1/' | grep -v ^0 | sort > job_publisher.sink.out

   for j in $(grep ^102 job_queue.log.gen | colrm 1 4); do
      grep -v ^"$j " sink.out > sink.out.tmp
      mv -f sink.out.tmp sink.out
   done

   diff -u sink.out job_publisher.sink.out
   if [ $? -eq 0 ]; then
      echo "SUCCESS: sink agrees with job_publisher"
      rm -f $FILE.tmp sink.out job_publisher.sink.out
   else
      echo "FAIL: ^^ difference between sink and job_publisher"
      FAIL=1
   fi
fi

exit $FAIL
