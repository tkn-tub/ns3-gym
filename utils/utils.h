/**
 * \ingroup utils 
 * \defgroup CheckStyle check-style.py
 *
 * The check-style.py script will test and reformat code according to the
 * ns-3 coding style posted at https://www.nsnam.org/developers/contributing-code/coding-style/
 * It requires that you install 'uncrustify' 
 *
 * It has multiple levels of conformance:
 *   - level=0: the default: merely checks indentation
 *   - level=1: checks also for missing spaces before parentheses
 *   - level=2: checks also for missing newlines and braces around single-line statements
 *   - level=3: checks also for missing trailing whitespaces
 *
 * Examples:
 * 
 * check a single file (level 0 by default):
\verbatim
./check-style.py -f src/core/object.h
\endverbatim
 *  
 * fix the style of a single file:
\verbatim
./check-style.py --level=2 --in-place -f src/core/object.h
\endverbatim
 * 
 * look at the changes needed for a single file:
\verbatim
./check-style.py --diff --level=1 -f src/core/object.h | less
\endverbatim
 *
 * look at the status of all files modified in your mercurial repository:
\verbatim
./check-style.py --check-hg
\endverbatim
 * 
 * look at the changes needed for all modified files in your mercurial
 * repository:
\verbatim
./check-style.py --check-hg --diff |less
\endverbatim
 * 
 * Enable this script to run as a 'commit' hook in your repository and
 * disallow commits which contain files with invalid style:
 * 
\verbatim
cat hgrc (can be appended to .hg/hgrc or ~/.hg/hgrc or /etc/hg/hgrc
[hooks]
# uncomment below line to enable: works only with mercurial >= 1.3
#pretxncommit.indent = path-to-binary/check-indent.py --check-hg-hook
# uncomment below line to enable: works with all (?) versions 
# of mercurial but requires that PYTHONPATH is defined to point to 
# the directory which contains check-indent.py
#pretxncommit.indent = python:check-indent.run_as_hg_hook
\endverbatim
 *
 * Usage:
\verbatim
Usage: check-style.py [options]

Options:
  -h, --help            show this help message and exit
  --debug               Output some debugging information
  -l LEVEL, --level=LEVEL
                        Level of style conformance: higher levels include all
                        lower levels. level=0: re-indent only. level=1: add
                        extra spaces. level=2: insert extra newlines and extra
                        braces around single-line statements. level=3: remove
                        all trailing spaces
  --check-hg-hook       Get the list of files to check from mercurial's list
                        of modified and added files and assume that the script
                        runs as a pretxncommit mercurial hook
  --check-hg            Get the list of files to check from mercurial's list
                        of modified and added files
  -f FILE, --check-file=FILE
                        Check a single file
  --diff                Generate a diff on stdout of the indented files
  -i, --in-place        Indent the input files in-place
\endverbatim
 */
