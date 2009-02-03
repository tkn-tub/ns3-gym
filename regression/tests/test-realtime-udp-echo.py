#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

def may_run(env, options):
    if not env["ENABLE_REAL_TIME"]:
        return "Real-time support not available"
