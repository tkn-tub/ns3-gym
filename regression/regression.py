#! /usr/bin/env python
# regression.py adapted from python language regression scripts.

"""Regression test.

This will find all modules whose name is "test-*" in the tests
directory, and run them.

Command line options:

-v: verbose   -- run tests in verbose mode (print diff output)
-g: generate  -- write the output file for a test instead of comparing it

If non-option arguments are present, they are names for tests to run.
If no test names are given, all tests are run.

"""

import getopt
import sys
import os
import urllib
import subprocess

verbose = 0
generate = 0

#
# The directory in which the tarball of the reference traces lives.  This is
# used if Mercurial is not on the system.
#
refUrl = "http://www.nsnam.org/releases/"

#
# The name of the tarball to find the reference traces in if there is no
# mercurial on the system.  It is expected to be created using tar -cjf and
# will be extracted using tar -xjf
#
refTarName = "ns-3-ref-traces.tar.bz2"

#
# The path to the Mercurial repository used to find the reference traces if
# we find "hg" on the system.  We expect that the repository will be named
# identically to refDirName below
#
refRepo = "http://code.nsnam.org/craigdo/"

#
# The local directory name into which the reference traces will go in either
# case (net or hg).
#
refDirName = "ns-3-ref-traces"

def main(tests = None, testdir = None):
    """Execute regression tests.

    Arguments:
    tests -- a list of strings containing test names (optional)
    testdir -- the directory in which to look for tests (optional)
    """

    global verbose
    global generate
    global refUrl
    global refTarName
    global refRepo
    global refRepoName
    global refDirName
    
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'vg')
    except getopt.error, msg:
        print msg
        print __doc__
        return 2

    for o, a in opts:
        if o == '-v': verbose = 1
        if o == '-g': generate = 1

    print "========== Running Unit Tests =========="
    os.system("./waf check")

    print "========== Running Regression Tests =========="
    if os.system("hg version > /dev/null 2>&1") == 0:
        print "Synchronizing reference traces using Mercurial."
        if not os.path.exists(refDirName):
            os.system("hg clone " + refRepo + refDirName +
              " > /dev/null 2>&1")
        else:
            os.chdir(refDirName)
            os.system("hg pull " + refRepo + refDirName +
                " > /dev/null 2>&1")
            os.chdir("..")
    else:
        print "Synchronizing reference traces from web."
        urllib.urlretrieve(refUrl + refTarName, refTarName)
        os.system("tar -xjf " + refTarName)

    print "Done."

    bad = []

    if not os.path.exists(refDirName):
        print "Reference traces directory does not exist"
        return 3
    
    if not testdir:
        testdir = os.path.join(os.curdir, "tests")

    if not os.path.exists(testdir):
        print "Tests directory does not exist"
        return 3
    
    sys.path.append(testdir)
        
    for i in range(len(args)):
        if args[i][-3:] == '.py':
            args[i] = args[i][:-3]

    if not tests:
        tests = args

    if not tests:
        tests = findtests(testdir)

    for test in tests:
        result = run_test(test)
        if result == 0:
            if generate:
                print "GENERATE" + test
            else:
                print "PASS " + test
        else:
            bad.append(test)
            print "FAIL " + test

    return len(bad) > 0

def findtests(testdir):
    """Return a list of test modules in the test directory

    Arguments:
    testdir -- the directory to look in for tests
    """
    names = os.listdir(testdir)
    tests = []
    for name in names:
        if name[:5] == "test-" and name[-3:] == ".py":
            testname = name[:-3]
            tests.append(testname)
    tests.sort()
    return tests

def run_test(test):
    """Run a single test.

    Arguments:
    test -- the name of the test
    """
    if os.path.exists("traces"):
        files = os.listdir("traces")
        for file in files:
            if file == '.' or file == '..':
                continue
            path = "traces" + os.sep + file
            os.remove(path)
    else:
        os.mkdir("traces")
    
    mod = __import__(test, globals(), locals(), [])
    return mod.run(verbose, generate, refDirName)

if __name__ == '__main__':
    sys.exit(main())
