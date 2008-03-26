#! /usr/bin/env python

"""Regression test simple-point-to-point."""

import os

def rmdir(dir):
    if os.path.exists(dir):
        files = os.listdir(dir)
        for file in files:
            if file == '.' or file == '..':
                continue
            path = dir + os.sep + file
            os.remove(path)
        os.rmdir(dir)

def run(verbose, generate):
    """Execute a test."""

    os.system("./waf --cwd regression/traces --run simple-point-to-point >& /dev/null")

    if generate:
        rmdir("knowns")
        os.rename("traces", "knowns")
        os.system("tar -cjf tests/test-simple-point-to-point.bz2 knowns/")
        rmdir("knowns")
        return 0
    else:
        rmdir("knowns")
        os.system("tar -xjf tests/test-simple-point-to-point.bz2 knowns/")
        return os.system("diff -q knowns traces >& /dev/null")
