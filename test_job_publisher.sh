#!/bin/sh

FAIL=0

FILE=job_queue.log.gen

rm -f $FILE
sh ./job_queue.log.sh $FILE

CREATE_COUNT=$(grep ^101 $FILE | wc -l)
DESTROY_COUNT=$(grep ^102 $FILE | wc -l)

./job_publisher --dump --file $FILE > $FILE.tmp 2>&1

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
   rm -f $FILE.tmp $FILE.tmp2 $FILE.tmp3
else
   echo "FAIL: ^^ difference between job_publisher memory and $FILE"
   FAIL=1
fi

exit $FAIL
