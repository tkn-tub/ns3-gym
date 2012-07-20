#! /usr/bin/env python
# -*- coding: utf-8 -*-
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# A list of C++ examples to run in order to ensure that they remain
# buildable and runnable over time.  Each tuple in the list contains
#
#     (example_name, do_run, do_valgrind_run).
#
# See test.py for more information.
cpp_examples = [
    ("lena-cqi-threshold", "True", "True"),
    ("lena-dual-stripe --simTime=0.01", "True", "True"),
    ("lena-dual-stripe --epc=1 --simTime=0.01", "True", "True"),
    ("lena-dual-stripe --epc=1 --useUdp=0 --simTime=0.01", "True", "True"),
    ("lena-dual-stripe --epc=1 --fadingTrace=../../src/lte/model/fading-traces/fading_trace_EPA_3kmph.fad --simTime=0.01", "True", "True"),
    ("lena-dual-stripe --nBlocks=1  --nMacroEnbSites=0 --macroUeDensity=0 --homeEnbDeploymentRatio=1 --homeEnbActivationRatio=1 --homeUesHomeEnbRatio=2 --macroEnbTxPowerDbm=0 --simTime=0.01", "True", "True"),
    ("lena-fading", "True", "True"),
    ("lena-gtpu-tunnel", "True", "True"),
    ("lena-intercell-interference", "True", "True"),
    ("lena-pathloss-traces", "True", "True"),
    ("lena-profiling", "True", "True"),
    ("lena-rlc-traces", "True", "True"),
    ("lena-rem", "True", "True"),
    ("lena-rem-sector-antenna", "True", "True"),
    ("lena-simple", "True", "True"),
    ("lena-simple-epc", "True", "True"),
]

# A list of Python examples to run in order to ensure that they remain
# runnable over time.  Each tuple in the list contains
#
#     (example_name, do_run).
#
# See test.py for more information.
python_examples = []
