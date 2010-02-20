#!/bin/sh

FILE=job_queue.log.gen

rm -f $FILE
sh ./job_queue.log.sh $FILE

CREATE_COUNT=$(grep ^101 $FILE | wc -l)
DESTROY_COUNT=$(grep ^102 $FILE | wc -l)

TOTAL=$(./job_publisher --dump --file $FILE 2>&1 | grep "Total" | sed 's/.*Total.*: \(.*\)/\1/')

if [ $((CREATE_COUNT - DESTROY_COUNT)) -eq $TOTAL ]; then
   echo "SUCCESS: $CREATE_COUNT - $DESTROY_COUNT = $TOTAL"
   exit 0
else
   echo "FAIL: $CREATE_COUNT - $DESTROY_COUNT != $TOTAL"
   exit 1
fi

