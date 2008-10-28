#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

import os
import sys
import tracediff

def run(verbose, generate, refDirName):
    """Execute a test."""
    if tracediff.env['ENABLE_PYTHON_BINDINGS']:
        return tracediff.run_test(verbose, generate, refDirName,
                                  "csma-bridge", pyscript=os.path.join('examples', 'csma-bridge.py'))
    else:
        print >> sys.stderr, "Skipping csma-bridge: Python bindings not available."
        raise NotImplementedError
