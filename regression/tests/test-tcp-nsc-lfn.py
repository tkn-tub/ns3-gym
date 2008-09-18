#! /usr/bin/env python

"""Trace-comparison-type regression test for the Network Simulation Cradle."""

import os
import shutil
import sys
import tracediff
import platform


def run(verbose, generate, refDirName):
    """Run a Network Simulation Cradle test involving two TCP streams."""

    if not tracediff.env['ENABLE_NSC']:
        print >>sys.stderr, "Skipping tcp-nsc-lfn: NSC not available."
        raise NotImplementedError

    testName = "tcp-nsc-lfn"
    arguments = ["--ns3::OnOffApplication::DataRate=40000", "--runtime=20"]
    platform_bits = platform.architecture()[0]
    
    if platform_bits == "64bit":
        traceDirName = testName + "_64bit.ref"
    elif platform_bits == "32bit":
        traceDirName = testName + "_32bit.ref"
    else:
        # Something unexpected. How should we signal an error here? Rasing a
        # string might not be the best idea?
        raise "Unknown architecture, not 64 or 32 bit?"

    return tracediff.run_test(verbose, generate, refDirName,
        testName, arguments=arguments, refTestName=traceDirName)
