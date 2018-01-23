#!/bin/bash

# Process doxygen log to generate sorted list of top offenders.
#

me=$(basename $0)
DIR="$(dirname $0)"
# Trick to get the absolute path, since doxygen prefixes errors that way
ROOT=$(cd "$DIR/.."; pwd -P)

# Known log files
STANDARDLOGFILE=doxygen.log
WARNINGSLOGFILE=doxygen.warnings.log
# Default choice:  generate it
LOG="$DIR/$WARNINGSLOGFILE"

# Verbose log
VERBLOG="$DIR/doxygen.verbose.log"


# Options ------------------------------
#

function usage
{
    cat <<-EOF
	
	Usage: $me [-beithv] [-s <log-file> | -l | -w] [-m <module>] [-f <regex>] [-F <regex>]
	
	Run doxygen to generate all errors; report error counts
	by module and file.
	
	-i  Skip build and print-introspected-doxygen.
	
	-s  Skip doxygen run; use existing <log-file>.
	-w  Skip doxygen run; use existing warnings log doc/$WARNINGSLOGFILE
	-l  Skip doxygen run; use the normal doxygen log doc/$STANDARDLOGFILE

	-b  Omit the blacklist filter of files whose warnings we ignore
	-e  Filter out warnings from */examples/*
	-t  Filter out warnings from */test/*
	-m  Only include files matching src/<module>
	-f  Only include files matching the <regex>
	-F  Exclude files matching the <regex>

	-v  Show detailed output from each step. Multiple allowed.
	-h  Print this usage message
	    
	The default behavior is to modify doxygen.conf temporarily to
	report all undocumented elements, and to reduce the run time.
	The output of this special run is kept in doc/$WARNINGSLOGFILE.
	To further reduce the run time, the -i option also skips
	print-introspected-doxygen, so waf doesn\'t have to compile
	any modified files at all.

	The -f, -l, and -s options skip the doxygen run altogether.
	The first two use a specified or the standard log file;
	the -s option uses the warnings log from a prior run.
	Only the first of -f <log-file>, -s, or -l will have effect.
		
	The -e and -t options exclude examples and test directories
	from the counts.  The -m option only includes a specific module.
	The -F option only includes files (or warnings) matching the <regex>.
	The -m and -F options append the relevant warnings after the
	numerical report.  These can be used in any combination.
	
EOF
    exit 1
}

# Messaging ----------------------------
#

# Arg -v Verbosity level
verbosity=0

function verbose
{
    if [ "$1" == "-n" ]; then
	echo -n "$2"
    elif [ $verbosity -eq 1 ]; then
	echo "$1 $2"
    else
	echo "$2"
    fi
}

# Use file handle 6 for verbose output
rm -f $VERBLOG
exec 6>$VERBLOG

function status_report
{
    local status="$1"
    local long_msg="$2"
    if [ $status -eq 0 ]; then
	verbose "$long_msg "  "done."
	rm -f $VERBLOG
    else
	verbose "$long_msg "  "FAILED.  Details:"
	cat $VERBLOG
	rm -f $VERBLOG
	exit 1
    fi
}
   

# Argument processing ------------------
#

# -f argument
use_filearg=0
logfile_arg=
# -l
use_standard=0
# skip doxygen run; using existing log file
skip_doxy=0 
# skip print-introspected-doxygen, avoiding a build 
skip_intro=0 

# Filtering flags
filter_blacklist=1
filter_examples=0
filter_test=0
filter_module=""
filter_in=""
filter_out=""

echo
echo "$me:"

while getopts :bef:F:hilm:s:tvw option ; do

    case $option in
	(b)  filter_blacklist=0       ;;
	
	(e)  filter_examples=1        ;;

	(f)  filter_in="$OPTARG"      ;;

	(F)  filter_out="$OPTARG"     ;;

	(h)  usage                    ;;
	
	(i)  skip_intro=1             ;;
	
	(l)  use_standard=1           ;;

	(m)  filter_module="$OPTARG"  ;;

	(s)  use_filearg=1
	     logfile_arg="$OPTARG"
	     ;;

	(t)  filter_test=1            ;;

	(v)  verbosity=1
	     exec 6>&1
	     ;;

	(w)  use_filearg=1
	     logfile_arg="$DIR/$WARNINGSLOGFILE"
	     ;;
	     
	(:)  echo "$me: Missing argument to -$OPTARG" ; usage ;;
	
	(\?) echo "$me: Invalid option: -$OPTARG"     ; usage ;;
	
    esac
done

function checklogfile
{
    if [ -e "$1" ] ; then
	skip_doxy=1
	LOG="$1"
    else
	echo "$me: log file $1 does not exist."
	exit 1
    fi
}
    
# Which log file
if [[ $use_filearg -eq 1 && "${logfile_arg:-}" != "" ]] ; then
    checklogfile "$logfile_arg"
elif [ $use_standard -eq 1 ]; then
    checklogfile "$DIR/$STANDARDLOGFILE"
fi

#  Run doxygen -------------------------
#

if [ $skip_doxy -eq 1 ]; then
    echo
    echo "Skipping doxygen run, using existing log file $LOG"
else

    if [ $skip_intro -eq 1 ]; then
	verbose "" "Skipping ./waf build and print-introspected-doxygen."
    else
        # Run introspection, which may require a build
	verbose -n "Building..."
	(cd "$ROOT" && ./waf build >&6 2>&6 )
	status_report $? "./waf build"
	verbose -n "Running print-introspected-doxygen..."
	(cd "$ROOT" && ./waf --run print-introspected-doxygen >doc/introspected-doxygen.h >&6 2>&6 )
	status_report $? "./waf --run print-introspected-doxygen"
	
    fi

    # Modify doxygen.conf to generate all the warnings
    # (We also suppress dot graphs, so shorten the run time.)

    conf=$DIR/doxygen.conf

    sed -i.bak -E '/^(EXTRACT_ALL|HAVE_DOT|CLASS_DIAGRAMS|WARNINGS|SOURCE_BROWSER) /s/(YES|yes)/no/;/^HTML_OUTPUT /s/html/html-warn/' $conf

    verbose -n "Rebuilding doxygen (v$(doxygen --version)) docs with full errors..."
    (cd "$ROOT" && ./waf --doxygen-no-build >&6 2>&6 )
    status=$?

    rm -f $conf
    mv -f $conf.bak $conf

    status_report $status "Doxygen run"

    cp -f "$DIR/$STANDARDLOGFILE" "$DIR/$WARNINGSLOGFILE"

fi

# Log filters --------------------------
#

# Append a regular expression to a parameter
#  with '\|' alternation operator if the parameter wasn't empty to begin with.
function REappend
{
    local param="$1"
    local token="$2"

    eval "${param}=\"${!param:-}${!param:+\\|}$token\""
}

# Filter in regular expression for -m and -f
filter_inRE=""
if [ "$filter_module" != "" ] ; then
    REappend filter_inRE src/$filter_module
fi
if [ "$filter_in" != "" ] ; then
    REappend filter_inRE "$filter_in"
fi

# Blacklist filter of files whose warnings we ignore
filter_blacklistRE=""

#   External files: adding our own doxygen makes diffs with upstream very hard
#     cairo-wideint
REappend filter_blacklistRE "cairo-wideint"

#   Functions with varying numbers of arguments
#   This is temporary until we move to C++-14
REappend filter_blacklistRE "Schedule(Time"
REappend filter_blacklistRE "ScheduleWithContext(uint32_t"
REappend filter_blacklistRE "Schedule\\(Now\\|Destroy\\)(\\(MEM\\|void\\)"

#   ATTRIBUTE_HELPER_CPP( and _HEADER(
REappend filter_blacklistRE "ATTRIBUTE_HELPER_\\(CPP\\|HEADER\\)("

filter_outRE=""
if [ $filter_blacklist -eq 1 ]; then
    echo "Filtering out blacklist: \"$filter_blacklistRE\""
    REappend filter_outRE "$filter_blacklistRE"
fi
# Filter out regular expression for -e, -t and -F
if [ $filter_examples -eq 1 ]; then
    REappend filter_outRE "/examples/"
fi
if [ $filter_test -eq 1 ]; then
    REappend filter_outRE "/test/"
fi
if [ "$filter_out" != "" ] ; then
    REappend filter_outRE "$filter_out"
fi

#  Show the resulting filters
echo
echo "Net result of all filters:"
if [ "${filter_inRE:-}" != "" ] ; then
    echo "Filtering in \"$filter_inRE\""
fi
if [ "${filter_outRE:-}" != "" ] ; then
    echo "Filtering out \"$filter_outRE\""
fi

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

    flog=$(                         \
	echo "$flog"              | \
	sort -t ':' -k1,1 -k2,2n  | \
	uniq                        \
	)

    echo "$flog"
}

# Analyze the log ----------------------
#
verbose -n "Filtering the doxygen log..."

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
    filterin=$(              \
	filter_log         | \
	sed "s|$ROOT/||g"    \
	)
fi

status_report 0 "Filter"
echo


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
