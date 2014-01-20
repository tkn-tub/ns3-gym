#!/bin/bash

# Process doxygen log to generate sorted list of top offenders.
#

me=$(basename $0)
DIR="$(dirname $0)"
ROOT="$(hg root)"

# Known log files
STANDARDLOGFILE=doxygen.log
WARNINGSLOGFILE=doxygen.warnings.log
# Default choice:  generate it
LOG="$DIR/$WARNINGSLOGFILE"


# Options ------------------------------
#

function usage
{
    cat <<-EOF
	
	Usage: $me [-eth] [-f <log-file> | -l | -s] [-m <module> | -F <regex>]
	
	Run doxygen to generate all errors; report error counts
	by module and file.
	
	The default behavior is to modify doxygen.conf temporarily to
	report all undocumented elements, and to reduce the run time.
	The output of this special run is kept in doc/$WARNINGSLOGFILE.

	The -e and -t options exclude examples and test directories
	from the counts.  The -m option only includes a specific module.
	The -F option only includes files (or warnings) matching the <regex>.
	The -m and -F options append the relevant warnings after the
	numerical report.  These can be used in any combination.
	
	-e  Filter out warnings from */examples/*
	-t  Filter out warnings from */test/*
	-m  Only include files matching src/<module>
	-F  Only include files matching the <regex> 

	The -f, -l, and -s options skip the doxygen run altogether.
	The first two use a specified or the standard log file;
	the -s option uses the warnings log from a prior run.
	Only the first of -f <log-file>, -s, or -l will have effect.
		
	-f  Skip doxygen run; use existing <log-file>.
	-s  Skip doxygen run; use existing warnings log doc/$WARNINGSLOGFILE
	-l  Skip doxygen run; use the normal doxygen log doc/$STANDARDLOGFILE
		
	-h  Print this usage message
	    
EOF
    exit 1
}

# Argument processing ------------------
#

# -f argument
usefilearg=0
logfilearg=
# -l
usestandard=0
# skip doxygen run; using existing log file
SKIPDOXY=0

# Filtering flags
filter_examples=0
filter_test=0
filter_module=""
filter_regex=""

while getopts :etm:F:lF:sh option ; do

    case $option in
	
	(e)  filter_examples=1        ;;
	
	(t)  filter_test=1            ;;

	(m)  filter_module="$OPTARG"  ;;

	(F)  filter_regex="$OPTARG"   ;;

	(l)  usestandard=1            ;;

	(f)  usefilearg=1
	     logfilearg="$OPTARG"
	     ;;

	(s)  usefilearg=1
	     logfilearg="$DIR/$WARNINGSLOGFILE"
	     ;;

	(h)  usage ;;
	(:)  echo "$me: Missing argument to -$OPTARG" ; usage ;;
	(\?) echo "$me: Invalid option: -$OPTARG"     ; usage ;;
    esac
done

function checklogfile
{
    if [ -e "$1" ] ; then
	SKIPDOXY=1
	LOG="$1"
    else
	echo "$me: log file $1 does not exist."
	exit 1
    fi
}
    
# Which log file
if [[ $usefilearg -eq 1 && "${logfilearg:-}" != "" ]] ; then
    checklogfile "$logfilearg"
elif [ $usestandard -eq 1 ]; then
    checklogfile "$DIR/$STANDARDLOGFILE"
fi

#  Run doxygen -------------------------
#

if [ $SKIPDOXY -eq 1 ]; then
    echo
    echo "Skipping doxygen run, using existing log file $LOG"
else

    # Run introspection, which may require a build
    (cd "$ROOT" && ./waf --run print-introspected-doxygen >doc/introspected-doxygen.h)

    # Modify doxygen.conf to generate all the warnings
    # (We also suppress dot graphs, so shorten the run time.)

    conf=$DIR/doxygen.conf

    sed -i.bak -E '/^EXTRACT_ALL |^HAVE_DOT |^WARNINGS /s/YES/no/' $conf
    rm -f $conf.bak

    echo
    echo -n "Rebuilding doxygen docs with full errors..."
    (cd "$ROOT" && ./waf --doxygen >/dev/null 2>&1)
    status=$?

    hg revert $conf

    if [ $status -eq 0 ]; then
	echo "Done."
    else
	echo "FAILED."
	exit 1
    fi

    cp -f "$DIR/$STANDARDLOGFILE" "$DIR/$WARNINGSLOGFILE"

fi

# Log filters --------------------------
#

# Filter regular expression for -m and -F
filter_inRE=""
if [ "$filter_module" != "" ] ; then
    filter_inRE="src/$filter_module"
fi
if [ "$filter_regex" != "" ] ; then
    filter_inRE="${filter_inRE:-}${filter_inRE:+\\|}$filter_regex"
fi

# Filter regular expression for -e and -t
filter_outRE=""
if [ $filter_examples -eq 1 ]; then
    filter_outRE="/examples/"
fi
if [ $filter_test -eq 1 ]; then
    filter_outRE="${filter_outRE:-}${filter_outRE:+\\|}/test/"
fi

if [ "${filter_inRE:-}" != "" ] ; then
    echo "Filtering in \"$filter_inRE\""
fi
if [ "${filter_outRE:-}" != "" ] ; then
    echo "Filtering out \"$filter_outRE\""
fi
echo

# Filter log file
function filter_log
{
    local flog;
    flog=$( cat "$LOG" | grep "^$ROOT" )

    if [ "${filter_inRE:-}" != "" ] ; then
	flog=$( echo "$flog" | grep "$filter_inRE" )
    fi

    if [ "${filter_outRE:-}" != "" ] ; then
	flog=$( echo "$flog" | grep -v "$filter_outRE" )
    fi

    echo "$flog"
}
    

# Analyze the log ----------------------
#

# List of module directories (e.g, "src/core/model")
undocmods=$(                \
    filter_log            | \
    cut -d ':' -f 1       | \
    sed "s|$ROOT/||g"     | \
    cut -d '/' -f 1-3     | \
    sort                  | \
    uniq -c               | \
    sort -nr                \
    )

# Number of directories
modcount=$(                         \
    echo "$undocmods"             | \
    wc -l                         | \
    sed 's/^[ \t]*//;s/[ \t]*$//'   \
    )

# For a function with multiple undocumented parameters,
# Doxygen prints the additional parameters on separate lines,
# so they don't show up in the totals above.
# Rather than work too hard to get the exact number for each file,
# we just list the total here. 
addlparam=$(                                  \
    grep "^  parameter '" "$LOG"            | \
    wc -l                                   | \
    sed 's/^[ \t]*//;s/[ \t]*$//'             \
    )

# Total number of warnings
warncount=$(                                  \
    echo "$undocmods"                       | \
    awk '{total += $1}; END {print total}'    \
    )
warncount=$((warncount + addlparam))

# List of files appearing in the log
undocfiles=$(               \
    filter_log            | \
    cut -d ':' -f 1       | \
    sed "s|$ROOT||g"      | \
    cut -d '/' -f 2-      | \
    sort                  | \
    uniq -c               | \
    sort -k 2               \
    )

# Sorted by number, decreasing
undocsort=$(echo "$undocfiles" | sort -k1nr,2 )

# Total number of files
filecount=$(                        \
    echo "$undocfiles"            | \
    wc -l                         | \
    sed 's/^[ \t]*//;s/[ \t]*$//'   \
    )

# Filtered in warnings
filterin=
if [ "${filter_inRE:-}" != "" ] ; then
    filterin=$(                 \
	filter_log            | \
	sed "s|$ROOT/||g"       \
	)
fi



# Summarize the log --------------------
#

echo
echo "Report of Doxygen warnings"
echo "----------------------------------------"
echo
echo "(All counts are lower bounds.)"
echo
echo "Warnings by module/directory:"
echo
echo "Count Directory"
echo "----- ----------------------------------"
echo "$undocmods"
echo " $addlparam additional undocumented parameters."
echo "----------------------------------------"
printf "%6d total warnings\n" $warncount
printf "%6d directories with warnings\n" $modcount
echo
echo
echo "Warnings by file (alphabetical)"
echo
echo "Count File" 
echo "----- ----------------------------------"
echo "$undocfiles"
echo "----------------------------------------"
printf "%6d files with warnings\n" $filecount
echo
echo
echo "Warnings by file (numerical)"
echo
echo "Count File" 
echo "----- ----------------------------------"
echo "$undocsort"
echo "----------------------------------------"
printf "%6d files with warnings\n" $filecount
echo
echo
echo "Doxygen Warnings Summary"
echo "----------------------------------------"
printf "%6d directories\n" $modcount
printf "%6d files\n" $filecount
printf "%6d warnings\n" $warncount

if [ "$filterin" != "" ] ; then
    echo
    echo
    echo "Filtered Warnings"
    echo "========================================"
    echo "$filterin"
fi
