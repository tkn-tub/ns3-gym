#! /usr/bin/env python

"""Generic trace-comparison-type regression test."""

import os.path

program = "main-ns2-mob"

def get_arguments(env, top_dir):
    ns2_tracefile = os.path.abspath(os.path.join(top_dir, "samples", "ns2-mob.tr"))
    return [ns2_tracefile, "out.tr"]

trace_dir_name = "main-ns2-mob.ref"
