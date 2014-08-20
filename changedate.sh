#!/bin/bash
# Script to use when copying runs from the RCT Trigger Supervisor 
# Run History Control Panel, in case of no response from DBS 
# or other run search tool.   Useful in case of hand-transfered runs
# Note:  Final file may need some hand editing, may chop off last line
# No warranties or guarantees on this script...
# Instructions:
# Copy the runs from the run history panel into a file called temp.txt
# Check to make sure there isn't an extra blank line at the end of it
# Run like so:  ./changedate.sh > newfile.txt
# Edit the file to Double check that you got all the runs needed, 
# delete any already done.  Good Luck! Pam
NUMLINES=`wc -l < temp.txt`
#echo $NUMLINES
for i in `seq 1 $NUMLINES`; do
   #echo $i
   RUN=`awk -v line=$i 'NR==line {print $1}' temp.txt`
   DATE=`awk -v line=$i 'NR==line {print $3,$4,$5,$6}' temp.txt`
   NEWDATE=`date --date "$DATE" +"%a, %d %b %Y %T GMT" `
   echo $RUN $NEWDATE
done
