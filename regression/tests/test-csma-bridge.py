#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

import os.path

def may_run(env):
    """Returns 0 when it can run, return non-zero or string (reason) when it cannot run"""
    if env['ENABLE_PYTHON_BINDINGS']:
        return 0
    else:
        return "Python bindings not available."

pyscript = os.path.join('examples', 'csma-bridge.py')
