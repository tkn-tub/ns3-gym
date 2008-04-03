#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

import sys
import os
import shutil

def run_test(verbose, generate, refDirName, testName):
    """Execute a test."""

    refTestDirName = refDirName + "/" + testName + ".ref"

    if not os.path.exists(refDirName):
        print"No reference trace repository"
        return 1

    if generate:
        if not os.path.exists(refTestDirName):
            print "creating new " + refTestDirName
            os.mkdir(refTestDirName)

        os.system("./waf --cwd regression/" + refTestDirName +
            " --run " + testName + " > /dev/null 2>&1")

        print "Remember to commit " + refTestDirName
        return 0
    else:
        if not os.path.exists(refTestDirName):
            print "Cannot locate reference traces"
            return 1

        shutil.rmtree("traces");
        os.mkdir("traces")

        os.system("./waf --cwd regression/traces --run " +
          testName + " > /dev/null 2>&1")

        if verbose:
            diffCmd = "diff traces " + refTestDirName + " | head"
        else:
            diffCmd = "diff traces " + refTestDirName + \
                " > /dev/null 2>&1"

        rc = os.system(diffCmd)
        if rc:
            print "----------"
            print "Traces differ in test: test-" + testName
            print "Reference traces in directory: " + refTestDirName
            print "Traces in directory: traces"
            print "Rerun regression test as: " + \
                "\"python regression.py test-" + testName + "\""
            print "Then do \"diff -u traces " + refTestDirName + \
                "\" for details"
            print "----------"
        return rc
                
