#! /usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# A list of C++ examples to run in order to ensure that they remain
# buildable and runnable over time.  Each tuple in the list contains
#
#     (example_name, do_run, do_valgrind_run).
#
# See test.py for more information.
cpp_examples = [
    ("star", "True", "True"),
    ("tcp-large-transfer", "True", "True"),
    ("tcp-nsc-lfn", "NSC_ENABLED == True", "False"),
    ("tcp-nsc-zoo", "NSC_ENABLED == True", "False"),
    ("tcp-star-server", "True", "True"),
    ("tcp-variants-comparison", "True", "True"),
]

# A list of Python examples to run in order to ensure that they remain
# runnable over time.  Each tuple in the list contains
#
#     (example_name, do_run).
#
# See test.py for more information.
python_examples = []
