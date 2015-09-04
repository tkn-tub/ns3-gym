#! /usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
#
# Copyright (c) 2014 Siddharth Santurkar
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation;
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

# NOTE: Run this script with the Python3 interpreter if the python3 compatibility 
#       of the ns-3 unit test runner needs to be tested.

#   The following options of test.py are being tested for poratability by this script.
#   To see the options supported by this script, run with the -h option on the command line
#      
#   -h, --help            show this help message and exit
#   -b BUILDPATH, --buildpath=BUILDPATH
#                         specify the path where ns-3 was built (defaults to the
#                         build directory for the current variant)
#   -c KIND, --constrain=KIND
#                         constrain the test-runner by kind of test
#   -d, --duration        print the duration of each test suite and example
#   -e EXAMPLE, --example=EXAMPLE
#                         specify a single example to run (no relative path is
#                         needed)
#   -u, --update-data     If examples use reference data files, get them to re-
#                         generate them
#   -f FULLNESS, --fullness=FULLNESS
#                         choose the duration of tests to run: QUICK, EXTENSIVE,
#                         or TAKES_FOREVER, where EXTENSIVE includes QUICK and
#                         TAKES_FOREVER includes QUICK and EXTENSIVE (only QUICK
#                         tests are run by default)
#   -g, --grind           run the test suites and examples using valgrind
#   -k, --kinds           print the kinds of tests available
#   -l, --list            print the list of known tests
#   -m, --multiple        report multiple failures from test suites and test
#                         cases
#   -n, --nowaf           do not run waf before starting testing
#   -p PYEXAMPLE, --pyexample=PYEXAMPLE
#                         specify a single python example to run (with relative
#                         path)
#   -r, --retain          retain all temporary files (which are normally
#                         deleted)
#   -s TEST-SUITE, --suite=TEST-SUITE
#                         specify a single test suite to run
#   -t TEXT-FILE, --text=TEXT-FILE
#                         write detailed test results into TEXT-FILE.txt
#   -v, --verbose         print progress and informational messages
#   -w HTML-FILE, --web=HTML-FILE, --html=HTML-FILE
#                         write detailed test results into HTML-FILE.html
#   -x XML-FILE, --xml=XML-FILE
#                         write detailed test results into XML-FILE.xml



from __future__ import print_function
from TestBase import TestBaseClass
import sys

def main(argv):
    """
        Prepares test cases and executes
    """
    test_cases = [
      '',
      '-h',
      '--help',
      '-b build/',
      '--buildpath=build/',
      '-c performance',
      '--constrain=performance',
      '-d',
      '--duration',
      '-e socket-options-ipv6',
      '--example=socket-options-ipv6',
      '-u',
      '--update-data',
      '-f EXTENSIVE --fullness=EXTENSIVE'
      '-g',
      '--grind',
      '-l',
      '--list',
      '-m',
      '--multiple',
      '-n',
      '--nowaf',
      '-p first',
      '--pyexample=first',
      '-r',
      '--retain',
      '-s ns3-tcp-interoperability',
      '--suite=ns3-tcp-interoperability',
      '-t t_opt.txt',
      '--text=t_opt.txt && rm -rf t_opt.txt',
      '-v',
      '--verbose',
      '-w t_opt.html && rm -rf t_opt.html',
      '--web=t_opt.html && rm -rf t_opt.html',
      '--html=t_opt.html && rm -rf t_opt.html',
      '-x t_opt.xml && rm -rf t_opt.xml',
      '--xml=t_opt.xml && rm -rf t_opt.xml',
    ]

    configure_string = sys.executable + ' waf configure --enable-tests --enable-examples'
    clean_string = sys.executable + ' waf clean'
    cmd_execute_list = [ '%s && %s test.py %s && %s' % (configure_string, sys.executable, option, clean_string) for option in test_cases]
    runner = TestBaseClass(argv[1:], "Test suite for the ns-3 unit test runner" , 'test-py')
    return runner.runtests(cmd_execute_list)

if __name__ == '__main__':
    sys.exit(main(sys.argv))