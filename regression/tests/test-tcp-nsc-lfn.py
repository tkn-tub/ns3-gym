#! /usr/bin/env python

"""Trace-comparison-type regression test for the Network Simulation Cradle."""

import platform
import sys

def may_run(env, options):
    if not env['NSC_ENABLED']:
        return "NSC not available"
    else:
        if options.valgrind:
            return "NSC does not get along with valgrind"
        if sys.platform != 'linux2':
            return "NSC is not well supported on non-Linux platforms"
        return 0


platform_bits = platform.architecture()[0]
if platform_bits == "64bit":
    trace_dir_name = "tcp-nsc-lfn_64bit.ref"
elif platform_bits == "32bit":
    trace_dir_name = "tcp-nsc-lfn_32bit.ref"
else:
    raise AssertionError("Unknown architecture, not 64 or 32 bit?")
del platform_bits

arguments = ["--ns3::OnOffApplication::DataRate=40000", "--runtime=20"]


