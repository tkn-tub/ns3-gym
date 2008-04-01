#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

import sys
import os
import shutil

def run_test(verbose, generate, refDirName, testName):
    """Execute a test."""

    repoName = "ns-3-ref-traces/"
    refDirName = testName + ".ref"

    if not os.path.exists(repoName):
        print"No reference trace repository"
        return 1

    if generate:
        if not os.path.exists(repoName + refDirName):
            print "creating new " + repoName + refDirName
            os.mkdir(repoName + refDirName)

        try:
            os.system("./waf --cwd regression/" + repoName + refDirName +
                " --run " + testName + " > /dev/null 2>&1")
        except:
            sys.exit(1)

        print "Remember to commit " + repoName + refDirName
        return 0
    else:
        if not os.path.exists(repoName + refDirName):
            print "Cannot locate reference traces"
            return 1

        shutil.rmtree("traces");
        os.mkdir("traces")

        os.system("./waf --cwd regression/traces --run " +
          testName + " > /dev/null 2>&1")

        if verbose:
            diffCmd = "diff traces " + repoName + refDirName + " | head"
        else:
            diffCmd = "diff traces " + repoName + refDirName + \
                " > /dev/null 2>&1"

        return os.system(diffCmd)
                
