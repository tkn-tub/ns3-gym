#!/bin/bash

# Process doxygen.warnings.log to generate sorted list of top offenders

logfile=doxygen.warnings.log
rootdir=`dirname $PWD`

echo
echo "Summary Report of Doxygen warnings:"
echo
echo "Count of warning by module:"
echo "Count Module"
grep "^$rootdir" $logfile  | \
    cut -d ':' -f 1 | \
    sed "s|$rootdir||g" | \
    cut -d '/' -f 1-3 | \
    sort | \
    uniq -c | \
    sort -nr

echo 

undocparam=` \
grep -v "^$rootdir" $logfile | \
    grep -v "not generated, too many nodes" | \
    grep "^  parameter '" $logfile | \
    wc -l | \
    sed 's/^[ \t]*//;s/[ \t]*$//' `

echo "+ $undocparam additional undocumented parameters."
echo

echo "(All counts are lower bounds.)"
echo
