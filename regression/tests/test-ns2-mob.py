#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

import os
import shutil
import tracediff

def run(verbose, generate):
    """Execute a test."""
    arguments = ["../../../samples/ns2-mob.tr", "out.tr"]
    return tracediff.run_test(verbose, generate, "main-ns2-mob", arguments=arguments)
