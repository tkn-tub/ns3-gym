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
    grep "^  parameter '" | \
    wc -l | \
    sed 's/^[ \t]*//;s/[ \t]*$//' `

echo "+ $undocparam undocumented parameters."
echo


function sort_tags {
    local conf=${1:-doxygen.conf}
    local tags=${2:-$1.tags}
    grep -v "^#" $conf | \
	sed '/^$/d' | \
	sed ':a;/\\$/N;s/\\\n/|n /;ta' | \
	sort | \
	sed 's/|n /\\\n/g' > $tags
}

orig=doxygen-r7657.conf
conf=doxygen.conf
tags=doxygen.conf.tags
delt=$tags.changed

sort_tags $orig
sort_tags $conf $tags

# Show only changed lines from second file
diff --changed-group-format='%>' \
     --unchanged-group-format='' \
     $orig.tags $tags > $delt

deltn=`cat $delt | \
    wc -l | \
    sed 's/^[ \t]*//;s/[ \t]*$//' `

echo "R7657 sorted tags in:    $orig.tags"
echo "Sorted doxygen tags in:  $tags"
echo "Changed doxygen tags in: $delt, ($deltn lines)"
echo
