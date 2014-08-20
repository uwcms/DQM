#!/bin/bash

rm -r LSFJOB_*/

if [ ! -d oldRunTestFiles ]
then
  mkdir oldRunTestFiles
fi

if [ ! -d ../python/oldRunPythonFiles ]
then
  mkdir ../python/oldRunPythonFiles
fi

cp -r run*/ oldRunTestFiles/
rm -r run*/
mv run*.root oldRunTestFiles/
mv testRCToffline_*.py oldRunTestFiles/

mv ../python/L1TRCToffline_*_cff.py ../python/oldRunPythonFiles/
mv ../python/L1TRCToffline_*_cff.pyc ../python/oldRunPythonFiles/
mv ../python/Rct_LUTconfiguration_*_cff.py ../python/oldRunPythonFiles/
mv ../python/Rct_LUTconfiguration_*_cff.pyc ../python/oldRunPythonFiles/
