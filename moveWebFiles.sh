#!/bin/bash

# Script to move old run files from the web to the scratch space
# Author: M. Weinberg

# Expects two arguments: a minimum and maximum run number to move to the scratch space

if [ $# -ne 2 ]
then
  echo $0 expects exactly two arguments: a minimum and maximum run number to move to the scratch space

  exit 10
fi

for f in ~/WWW/run*
do
  if [ ${f:36:6} -ge $1 -a ${f:36:6} -le $2 ]
  then
    rm ${f}/thumb*
    cp -r ${f} oldRunTestFiles/
    rm -r $f
    runNo=${f:36:6}
    ./sedml ~/WWW/index.html "s,[^\n]*${runNo}[^\n]*\n[^\n]*\n[^\n]*\n,," temp.txt
    mv temp.txt ~/WWW/index.html
  fi
done
