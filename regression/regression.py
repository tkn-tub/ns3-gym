#! /usr/bin/env python
# regression.py adapted from python language regression scripts.

"""Regression test.

This will find all modules whose name is "test_*" in the tests
directory, and run them.

Command line options:

-v: verbose   -- run tests in verbose mode with output to stdout
-g: generate  -- write the output file for a test instead of comparing it

If non-option arguments are present, they are names for tests to run.
If no test names are given, all tests are run.

"""

import sys
import os
import getopt

verbose = 0
generate = 0

def main(tests = None, testdir = None):
    """Execute regression tests.

    Arguments:
    tests -- a list of strings containing test names (optional)
    testdir -- the directory in which to look for tests (optional)
    """

    global verbose
    global generate
    
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'vg')
    except getopt.error, msg:
        print msg
        print __doc__
        return 2

    for o, a in opts:
        if o == '-v': verbose = 1
        if o == '-g': generate = 1

    bad = []

    if not testdir:
        testdir = os.path.join(os.curdir, "tests")

    if not os.path.exists(testdir):
        print "Tests directory does not exist"
        return 3
    
    if verbose:
        print "tests directory: ", testdir

    sys.path.append(testdir)
        
    for i in range(len(args)):
        if args[i][-3:] == '.py':
            args[i] = args[i][:-3]

    if not tests:
        tests = args

    if not tests:
        tests = findtests(testdir)

    for test in tests:
        if verbose:
            print "main(): running test", test
        result = runtest(test)
        if result == 0:
            if generate:
                print "GENERATE ", test
            else:
                print "PASS ", test
        else:
            bad.append(test)
            print "FAIL ", test

    return len(bad) > 0

def findtests(testdir):
    """Return a list of test modules in the test directory

    Arguments:
    testdir -- the directory to look in for tests
    """
    if verbose:
        print "findtests(", testdir, ")"
    if verbose:
        print "findtests(): look in ", testdir

    names = os.listdir(testdir)
    if verbose:
        print "findtests(): found ", names
    tests = []
    for name in names:
        if name[:5] == "test_" and name[-3:] == ".py":
            testname = name[:-3]
            tests.append(testname)
    tests.sort()
    if verbose:
        print "findtests(): found tests ", tests
    return tests

def runtest(test):
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
    
    if verbose:
        print "runtest(): run ", test
    mod = __import__(test, globals(), locals(), [])
    return mod.run(verbose, generate)

if __name__ == '__main__':
    sys.exit(main())
