#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

import os
import shutil
import tracediff

def run(verbose, generate, refDirName):
    """Execute a test."""
    #print tracediff.env
    return tracediff.run_test(verbose, generate, refDirName, "udp-echo")
