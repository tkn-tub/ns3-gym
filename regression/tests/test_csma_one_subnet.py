#! /usr/bin/env python

"""Regression test csma-one-subnet."""

import os

def run(verbose, generate):
    """Execute a test."""
    os.system("./waf --cwd regression/traces --run csma-one-subnet")
    return 0
