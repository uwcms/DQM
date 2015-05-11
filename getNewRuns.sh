#!/bin/bash

if [ -e newRuns.txt ]
then
  mv newRuns.txt oldRuns.txt
fi

if [ -e diff.txt ]
then
  rm diff.txt
fi

for i in `seq 0 10`;
do

ss="${i} days ago"

DATE=`date --date="$ss" +%Y%m%d`

echo $DATE

NPROC=$(($NPROC+1))

#dbs search --query="find run, run.createdate where run.numlss >= 20 and run.createdate >= $DATE and file like */MinimumBias/RAW/v1*" > newRuns.txt
#dbsql "find run, run.createdate where run.numlss > = 20 and run.createdate = 2012-05-14 and file like *Run2012B/MultiJet/RAW/v1*" >> newRuns.txt &
#dbsql "find run, run.createdate where run.createdate = $DATE and file like *Commissioning2014/Cosmics/RAW/v3*" >> newRuns.txt &

das_client.py --query="run date = ${DATE} | grep run.run_number, run.start_time,
 run.nlumis>10" >> newRuns.txt

if [ "$NPROC" -ge 5 ]; then
        wait
        NPROC=0
fi

done

grep " 201" newRuns.txt | sort -r > newerRuns.txt

/bin/mv newerRuns.txt newRuns.txt

if [ -e oldRuns.txt ]
then
  diff oldRuns.txt newRuns.txt | sed -n '/^>/ p' | sed 's/^.\{2\}//g' > diff.txt
  /bin/mv diff.txt newRuns.txt
fi
