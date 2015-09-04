#! /usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
#
# Copyright (c) 2014 Siddharth Santurkar
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation;
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

from __future__ import print_function
import sys
import subprocess
import argparse
import os

def print_case_in_file(case_string, out):
    for i in range(100):
        print("-", end = '', file = out)
    print(file=out)
    print("running test case " + case_string, end='\n\n', file = out)
    out.flush()

def print_failed_cases(failed_cases):
    print("\nFailed Cases:")
    for case in failed_cases:
        print(case)

def print_cmds(cmds):
    print('Commands to be executed:')
    for cmd in cmds:
        print(cmd.replace(sys.executable, ''))

def set_workdir():
    dir_files = [ f for f in os.listdir('.') if os.path.exists(f) ]
    if not 'VERSION' in dir_files and not 'waf' in dir_files:
        if os.path.split(os.path.abspath('.'))[1] == 'tests' and os.path.split(os.path.abspath(os.pardir))[1] == 'utils':
                os.chdir('../../')
        else:
            print('Error: Invalid working directory')
            sys.exit(1)

class TestBaseClass:
    """
        Generic class for testing tools based on provided commands and test cases.
    """

    def __init__(self, argv, desc, mode):
        """
            Provide input argument list, description and mode of the suite being executed.
        """
        self.my_env = os.environ
        set_workdir()
        self.my_env['LD_LIBRARY_PATH'] = os.getcwd() + "/build"
        self.mode = mode
        self.outfile = 'test-port-'+self.mode+'.out'
        self.options = self.parseargs(argv , desc)

    def parseargs(self, argv, desc):
        """
            Parses the commandline arguments
        """
        parser = argparse.ArgumentParser(description = desc)
        parser.add_argument('-f', '--file', action='store', dest='out_file', default = self.outfile,
                            metavar="FILE",
                            help='File to be used for storing the command specific output (Default: '+self.outfile+')')
        parser.add_argument('-c', action='store_true', dest='cmds', default=False,
                            help='List out all the commands being tested')
        parser.add_argument('-m', action='store_true', dest='mute', default=False,
                            help='Sends only stderr output to FILE')
        parser.add_argument('-x', '--customcmd', action='store', dest='custcmd', default = None,
                            help='Enter a comma-separated list of commands to override the existing ones. NOT APPLICABLE FOR TEST-PY SUITE.')
        return parser.parse_args(argv)

    def override_cmds(self):
        """
            Can be used by importing suite to handle custom commands
        """
        return self.options.custcmd

    def runtests(self, cmds):
        """
            Execute the tests.
        """
        if self.options.cmds:
            print_cmds(cmds)
            return

        final_return = 0
        total_tests = len(cmds)
        passed = 0
        progress = 0.0
        failed_cases = []
        with open(self.options.out_file, 'w') as out:
            outstream = out
            with open(os.devnull, 'w') as sink:
                if self.options.mute:
                    outstream = sink
                for cmd in cmds:
                    case_string = cmd.replace(sys.executable, '')
                    print("running test case: " + case_string)
                    print_case_in_file(case_string, out)
                    progress += 1
                    ret = subprocess.call(cmd, shell=True, env=self.my_env, stdout=outstream, stderr=out)
                    if not ret:
                        passed += 1
                    else:
                        final_return = 1
                        failed_cases.append(case_string)
                    print("[ %s out of %s ] test cases passed; Progress = %.2f%% \n" % (passed, total_tests, progress*100/total_tests))
        if final_return != 0:
            print_failed_cases(failed_cases)
        else:
            print("\nAll cases passed")
        print ("Detailed output available in " + self.options.out_file, end='\n\n')
        return final_return
