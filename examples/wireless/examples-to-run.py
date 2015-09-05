#! /usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# A list of C++ examples to run in order to ensure that they remain
# buildable and runnable over time.  Each tuple in the list contains
#
#     (example_name, do_run, do_valgrind_run).
#
# See test.py for more information.
cpp_examples = [
    ("mixed-wireless", "True", "True"),
    ("multirate --totalTime=0.3s --rateManager=ns3::AarfcdWifiManager", "True", "True"), 
    ("multirate --totalTime=0.3s --rateManager=ns3::AmrrWifiManager", "True", "True"), 
    ("multirate --totalTime=0.3s --rateManager=ns3::CaraWifiManager", "True", "True"), 
    ("multirate --totalTime=0.3s --rateManager=ns3::IdealWifiManager", "True", "True"), 
    ("multirate --totalTime=0.3s --rateManager=ns3::MinstrelWifiManager", "True", "True"), 
    ("multirate --totalTime=0.3s --rateManager=ns3::OnoeWifiManager", "True", "True"), 
    ("multirate --totalTime=0.3s --rateManager=ns3::RraaWifiManager", "True", "True"), 
    ("simple-wifi-frame-aggregation", "True", "True"),
    ("wifi-adhoc", "False", "True"), # Takes too long to run
    ("wifi-ap --verbose=0", "True", "True"), # Don't let it spew to stdout
    ("wifi-clear-channel-cmu", "False", "True"), # Requires specific hardware
    ("wifi-simple-adhoc", "True", "True"),
    ("wifi-simple-adhoc-grid", "True", "True"),
    ("wifi-simple-infra", "True", "True"),
    ("wifi-simple-interference", "True", "True"),
    ("wifi-wired-bridging", "True", "True"),
    ("power-adaptation-distance --manager=ns3::ParfWifiManager --outputFileName=parf --steps=5 --stepsSize=10", "True", "True"),
    ("power-adaptation-distance --manager=ns3::AparfWifiManager --outputFileName=aparf --steps=5 --stepsSize=10", "True", "True"),
    ("ofdm-ht-validation", "True", "True"),
    ("ofdm-validation", "True", "True"),
    ("ofdm-vht-validation", "True", "True"),
]

# A list of Python examples to run in order to ensure that they remain
# runnable over time.  Each tuple in the list contains
#
#     (example_name, do_run).
#
# See test.py for more information.
python_examples = [
    ("wifi-ap.py", "True"),
    ("mixed-wireless.py", "True"),
]
