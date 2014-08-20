#!/bin/bash

while read line
do
  ./makeRctDqmFiles.sh $line
done < $1
