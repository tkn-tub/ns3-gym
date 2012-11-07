#!/bin/bash

# Process doxygen.warnings.log to generate sorted list of top offenders

DIR=`dirname $0`
ROOT=`hg root`

# First, we have to modify doxygen.conf to generate all the warnings
# (We also suppress dot graphs, so shorten the run time.)

conf=$DIR/doxygen.conf

sed -i.bak -E '/^EXTRACT_ALL |^HAVE_DOT |^WARNINGS /s/YES/no/' $conf
rm -f $conf.bak

echo -n "Rebuilding doxygen docs with full errors..."
(cd $ROOT && ./waf --doxygen >/dev/null 2>&1)
status=$?

hg revert $conf

if [ "$status" = "0" ]; then
    echo "Done."
else
    echo "FAILED."
    exit 1
fi


# Save the log file
log=$DIR/doxygen.warnings.log

mv $DIR/doxygen.log $log

# Now we're ready to summarize the log


echo
echo "Summary Report of Doxygen warnings:"
echo
echo "Count of warning by module:"
echo "Count Module"
grep "^$ROOT" $log  | \
    cut -d ':' -f 1 | \
    sed "s|$ROOT||g" | \
    cut -d '/' -f 1-3 | \
    sort | \
    uniq -c | \
    sort -nr

echo 

undocparam=` \
grep -v "^$ROOT" $log | \
    grep -v "not generated, too many nodes" | \
    grep "^  parameter '" $log | \
    wc -l | \
    sed 's/^[ \t]*//;s/[ \t]*$//' `

echo "+ $undocparam additional undocumented parameters."
echo

echo "(All counts are lower bounds.)"
echo
