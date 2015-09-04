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

# NOTE: Run this script with the Python3 interpreter if the python3 compatibility 
#       of the ns-3 unit test runner needs to be tested.

#   The following options of waf are being tested for poratability by this script.
#   To see the options supported by this script, run with the -h option on the command line
#
#  build    : executes the build  (pre: configure, post: clean)
#  check    : run the equivalent of the old ns-3 unit tests using test.py
#  clean    : cleans the project
#  configure: configures the project (pore: None, post: distclean)
#  dist     : makes a tarball for redistributing the sources (pre:none )
#  distcheck: checks if the project compiles (tarball from 'dist') (pre: dist, post: rm -rf ns-3*.tar.bz2)
#  docs     : build all the documentation: doxygen, manual, tutorial, models (pre: configure; post: distclean)
#  doxygen  : do a full build, generate the introspected doxygen and then the doxygen
#  install  : installs the targets on the system (pre: configure, post: uninstall )
#  list     : lists the targets to execute (pre: configure)
#  shell    : run a shell with an environment suitably modified to run locally built programs (pre:configure)
#  sphinx   : build the Sphinx documentation: manual, tutorial, models 
#  step     : executes tasks in a step-by-step fashion, for debugging (pre: configure)
#  uninstall: removes the targets installed (pre: install, post uninstall)
#  *update   : updates the plugins from the *waflib/extras* directory




from __future__ import print_function
from TestBase import TestBaseClass
import sys

def replace(pre, post, main_cmd_list):
    if pre:
        pre = pre + ' && '
    else:
        pre = ''
    if post:
        post = ' && ' + post
    else:
        post = ''
    return [ pre + main_cmd + post for main_cmd in main_cmd_list ]

def main(argv):
    """
        Prepares test cases and executes
    """
    runner = TestBaseClass(argv[1:], "Test suite for the ns-3 Waf build system", 'waf')
    in_cmds = runner.override_cmds()
    if in_cmds:
        cmds = in_cmds.split(',')
    else:
        cmds = ['basic', 'build', 'configure', 'step', 'clean', 'dist', 'list']

    config_test_cases = [
        "--enable-gcov",
        "--enable-sudo",
        "--enable-sudo",
        "--enable-tests",
        "--disable-tests",
        "--enable-examples",
        "--disable-examples",
        "--doxygen-no-build",
        "--enable-static",
        "--enable-mpi",
        "--enable-rpath",
        "--enable-modules=build/utils/test-runner.cc.1.o",
        "--boost-static",
        "--boost-mt",
        "--boost-linkage_autodetect",
        "--boost-python=33",
        "--disable-gtk",
        "--int64x64=cairo",
        "--disable-pthread",
        "--force-planetlab",
        "--nopyc",
        "--nopyo",
        "--disable-python",
        "--apiscan=all",
        "--with-python=/usr/bin/python2.7",
        "--no32bit-scan",
        "-o test_out && rm -rf test_out",
        "--out=test_out && rm -rf test_out",
        "-t test_top && rm -rf test_top",
        "--top=test_top && rm -rf test_top",
        "--download",    
        "--check-c-compiler=gc",
        "--check-cxx-compiler=g++",       
    ]

    basic_test_cases = [
        "--version",
        "-h",
        "--help",
    ]

    build_test_cases = [
        "-j10",
        "--jobs=10",
        "-d optimized",
        "-d debug",
        "-d release",
        "--build-profile optimized",
        "--build-profile debug",
        "--build-profile release",
        "-p",
        "--progress",
    ]

    step_test_cases = [
        "--files=\"*/main.c,*/test/main.o\"",
    ]

    install_test_cases = [
        "-f",
        "--force",
        "--prefix=./test-prefix && rm -rf ./test-prefix",
        "--exec-prefix=.",
        "--bindir=./test-prefix/bin --sbindir=./test-prefix/sbin --libexecdir=./test-prefix/libexec --sysconfdir=./test-prefix/etc --sharedstatedir=./test-prefix/com --localstatedir=./test-prefix/var --libdir=./test-prefix/lib --includedir=./test-prefix/include --oldincludedir=./test-prefix/usr/include --datarootdir=./test-prefix/share --datadir=./test-prefix/share_root --infodir=./test-prefix/info --localedir=./test-prefix/locale --mandir=./test-prefix/man --docdir=./test-prefix/doc/package --htmldir=./test-prefix/doc --dvidir=./test-prefix/doc --pdfdir=./test-prefix/doc --psdir=./test-prefix/doc && rm -rf ./test-prefix",
    ]

    common_test_cases = [
        "",
        "-k",
        "--keep",
        "-v",
        "--verbose",
        "--nocache",
        "--zones=task_gen",
        "--zones=deps",
        "--zones=tasks",
        "--no-task-lines",
    ]

    test_case_mappings = {
        'basic' : basic_test_cases,
        'configure' : config_test_cases,
        'build' : build_test_cases,
        'step' : step_test_cases,
        'install' : install_test_cases,
    }

    waf_string = sys.executable + ' waf'
    cmd_execute_list = []
    for cmd in cmds:
        if cmd == 'basic':
            cmd_list = []
        else:
            cmd_list = ['%s %s %s' % (waf_string, cmd, option) for option in common_test_cases ]
        if cmd in test_case_mappings:
            cmd_list += ['%s %s %s' % (waf_string, cmd, option)  for option in test_case_mappings[cmd] ]
        if cmd == 'basic':
            cmd_list.append('%s configure && %s build && %s --run scratch/myfirst' % tuple([waf_string]*3))
            cmd_list.append('%s configure && %s build && %s --pyrun scratch/myfirst.py' % tuple([waf_string]*3))
        if cmd == 'build':
            cmd_list = replace(waf_string+' configure', waf_string+' clean', cmd_list)
            cmd_list.append('%s configure --enable-gcov && %s build --lcov-report && %s clean' % tuple([waf_string]*3))
        elif cmd == 'configure':
            cmd_list = replace(None, waf_string+' distclean', cmd_list)
        elif cmd == 'distcheck':
            cmd_list = replace(waf_string+' dist', 'rm -rf ns-3*.tar.bz2', cmd_list)
        elif cmd == 'docs':
            cmd_list = replace(waf_string+' configure', waf_string+' distclean', cmd_list)
        elif cmd == 'install':
            cmd_list = replace(waf_string+' configure', waf_string+' uninstall', cmd_list)
        elif cmd == 'list':
            cmd_list = replace(waf_string+' configure', waf_string +' distclean', cmd_list)
        elif cmd == 'shell':
            cmd_list = replace(waf_string+' configure', waf_string+' distclean', cmd_list)
        elif cmd == 'step':
            cmd_list = replace(waf_string+' configure', waf_string+' distclean', cmd_list)
        elif cmd == 'uninstall':
            cmd_list = replace(waf_string+' install', None, cmd_list)
        cmd_execute_list += cmd_list

    return runner.runtests(cmd_execute_list)

if __name__ == '__main__':
    sys.exit(main(sys.argv))