#! /usr/bin/env python

"""Compare that Wifi-Wired Bridging generates correct traces."""

import os
import shutil
import tracediff

def run(verbose, generate):
    """Execute a test."""

    return tracediff.run_test(verbose, generate, "wifi-wired-bridging", ["--SendIp=0"])
