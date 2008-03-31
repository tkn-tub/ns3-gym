#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

import os
import shutil

def run(verbose, generate):
    """Execute a test."""

    testName = "tcp-large-transfer"
    repoName = "ns-3-ref-traces/"
    refDirName = testName + ".ref"

    if not os.path.exists(repoName):
        print"No reference trace repository"
        return 1

    if generate:
        if not os.path.exists(repoName + refDirName):
            print "creating new " + repoName + refDirName
            os.mkdir(repoName + refDirName)

        os.system("./waf --cwd regression/" + repoName + refDirName +
            " --run " + testName + " >& /dev/null")

        print "Remember to commit " + repoName + refDirName
        return 0
    else:
        if not os.path.exists(repoName + refDirName):
            print "Cannot locate reference traces"
            return 1

        shutil.rmtree("traces");
        os.mkdir("traces")
        
        os.system("./waf --cwd regression/traces --run " +
          testName + " >& /dev/null")
        
        return os.system("diff -q traces " + repoName + refDirName +
         " >& /dev/null")
