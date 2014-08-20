#!/bin/bash

while read line
do
  root -b -q -l "newRct.C++(\"${line%% *}\")"
done < $1
