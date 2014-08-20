#!/bin/bash

# Expects one argument: a text file with four arguments per line:
# run date key "description"

if [ $# -ne 1 ]
then
  echo $0 expects exactly one argument: a text file with four arguments per line:
  echo run date key "description"
  exit 10
fi

if [ ! -f ${1} ]
then
  echo File $1 does not seem to exist!
  exit 20
fi

addDescrip()
{
  echo $2 $3 $4 > $CMSSW_BASE/src/DQM/L1TMonitor/test/run$1/description.txt
  rm -r ~/WWW/run$1
}

while read line
do
  addDescrip $line
done < $1