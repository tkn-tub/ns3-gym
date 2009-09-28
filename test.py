#! /usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
#
# Copyright (c) 2009 University of Washington
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

import os
import sys
import optparse
import subprocess
import threading
import Queue
import signal
import random
import xml.dom.minidom
import shutil

#
# XXX This should really be part of a waf command to list the configuration
# items relative to optional ns-3 pieces.
#
# A list of interesting configuration items in the waf configuration 
# cache which we may be interested in when deciding on which examples
# to run and how to run them.  These are set by waf during the 
# configuration phase and the corresponding assignments are usually
# found in the associated subdirectory wscript files.
#
interesting_config_items = [
    "NS3_BUILDDIR",
    "NS3_MODULE_PATH",
    "ENABLE_EMU",
    "ENABLE_GSL",
    "ENABLE_GTK_CONFIG_STORE",
    "ENABLE_LIBXML2",
    "ENABLE_NSC",
    "ENABLE_PYTHON_BINDINGS",
    "ENABLE_PYTHON_SCANNING",
    "ENABLE_REAL_TIME",
    "ENABLE_STATIC_NS3",
    "ENABLE_SUDO",
    "ENABLE_TAP",
    "ENABLE_THREADING",
]

#
# A list of examples to run as smoke tests just to ensure that they remain 
# buildable and runnable over time.  Also a condition under which to run
# the example (from the waf configuration).
#
# XXX Should this not be read from a configuration file somewhere and not
# hardcoded.
#
example_tests = [
    ("csma-bridge", "True"),
    ("csma-bridge-one-hop", "True"),
    ("csma-broadcast", "True"),
    ("csma-multicast", "True"),
    ("csma-one-subnet", "True"),
    ("csma-packet-socket", "True"),
    ("csma-ping", "True"),
    ("csma-raw-ip-socket", "True"),
    ("csma-star", "True"),
    ("dynamic-global-routing", "True"),
    ("first", "True"),
    ("global-injection-slash32", "True"),
    ("global-routing-slash32", "True"),
    ("hello-simulator", "True"),
    ("icmpv6-redirect", "True"),
    ("mesh", "True"),
    ("mixed-global-routing", "True"),
    ("mixed-wireless", "True"),
    ("multirate", "False"), # takes forever to run
    ("nix-simple", "True"),
    ("nms-p2p-nix", "False"), # takes forever to run
    ("object-names", "True"),
    ("ping6", "True"),
    ("radvd", "True"),
    ("radvd-two-prefix", "True"),    
    ("realtime-udp-echo", "ENABLE_REAL_TIME == True"),
    ("second", "True"),
    ("simple-alternate-routing", "True"),
    ("simple-error-model", "True"),
    ("simple-global-routing", "True"),
    ("simple-point-to-point-olsr", "True"),
    ("simple-routing-ping6", "True"),
    ("simple-wifi-frame-aggregation", "True"),
    ("star", "True"),
    ("static-routing-slash32", "True"),
    ("tcp-large-transfer", "True"),
    ("tcp-nsc-zoo", "ENABLE_NSC == True"),
    ("tcp-star-server", "True"),
    ("test-ipv6", "True"),
    ("third", "True"),
    ("udp-echo", "True"),
    ("virtual-net-device", "True"),
    ("wifi-adhoc", "False"), # takes forever to run
    ("wifi-ap", "True"),
    ("wifi-wired-bridging", "True"),
]

#
# Most of the examples produce gangs of trace files, so we want to find
# somewhere to put them that won't pollute the current directory.  One
# obvious place is somewhere in /tmp.
#
TMP_TRACES_DIR = "/tmp/unchecked-traces"

#
# The test suites are going to want to output status.  They are running
# concurrently.  This means that unless we are careful, the output of
# the test suites will be interleaved.  Rather than introducing a lock
# file that could unintentionally start serializing execution, we ask
# the tests to write their output to a temporary directory and then 
# put together the final output file when we "join" the test tasks back
# to the main thread.
#
TMP_OUTPUT_DIR = "/tmp/testpy"

def get_node_text(node):
    for child in node.childNodes:
        if child.nodeType == child.TEXT_NODE:
            return child.nodeValue
    return "None"

#
# A simple example of writing a text file with a test result summary.
#
def translate_to_text(results_file, text_file):
    f = open(text_file, 'w')
    dom = xml.dom.minidom.parse(results_file)
    for suite in dom.getElementsByTagName("TestSuite"):
        result = get_node_text(suite.getElementsByTagName("SuiteResult")[0])
        name = get_node_text(suite.getElementsByTagName("SuiteName")[0])
        time = get_node_text(suite.getElementsByTagName("SuiteTime")[0])
        output = "%s: Test Suite \"%s\" (%s)\n" % (result, name, time)
        f.write(output)
        if result != "CRASH":
            for case in suite.getElementsByTagName("TestCase"):
                result = get_node_text(case.getElementsByTagName("CaseResult")[0])
                name = get_node_text(case.getElementsByTagName("CaseName")[0])
                time = get_node_text(case.getElementsByTagName("CaseTime")[0])
                output =   "  %s: Test Case \"%s\" (%s)\n" % (result, name, time)
                f.write(output)

                if result == "FAIL":
                    f.write("    Details:\n")
                    f.write("      Message:   %s\n" % get_node_text(case.getElementsByTagName("CaseMessage")[0]))
                    f.write("      Condition: %s\n" % get_node_text(case.getElementsByTagName("CaseCondition")[0]))
                    f.write("      Actual:    %s\n" % get_node_text(case.getElementsByTagName("CaseActual")[0]))
                    f.write("      Limit:     %s\n" % get_node_text(case.getElementsByTagName("CaseLimit")[0]))
                    f.write("      File:      %s\n" % get_node_text(case.getElementsByTagName("CaseFile")[0]))
                    f.write("      Line:      %s\n" % get_node_text(case.getElementsByTagName("CaseLine")[0]))

    for example in dom.getElementsByTagName("Example"):
        result = get_node_text(example.getElementsByTagName("Result")[0])
        name = get_node_text(example.getElementsByTagName("Name")[0])
        output = "%s: Example \"%s\"\n" % (result, name)
        f.write(output)

    f.close()
    
#
# A simple example of writing an HTML file with a test result summary.
#
def translate_to_html(results_file, html_file):
    f = open(html_file, 'w')
    f.write("<html>\n")
    f.write("<body>\n")
    f.write("<center><h1>ns-3 Test Results</h1></center>\n")

    dom = xml.dom.minidom.parse(results_file)

    f.write("<h2>Test Suites</h2>\n")
    for suite in dom.getElementsByTagName("TestSuite"):
        name = get_node_text(suite.getElementsByTagName("SuiteName")[0])
        result = get_node_text(suite.getElementsByTagName("SuiteResult")[0])
        time = get_node_text(suite.getElementsByTagName("SuiteTime")[0])

        if result == "PASS":
            f.write("<h3 style=\"color:green\">%s: %s (%s)</h3>\n" % (result, name, time))
        else:
            f.write("<h3 style=\"color:red\">%s: %s (%s)</h3>\n" % (result, name, time))


        f.write("<table border=\"1\">\n")
        f.write("<th> Result </th>\n")

        if result == "CRASH":
            f.write("<tr>\n")
            f.write("<td style=\"color:red\">%s</td>\n" % result)
            f.write("</tr>\n")
            f.write("</table>\n")
            continue

        f.write("<th>Test Case Name</th>\n")
        f.write("<th> Time </th>\n")

        if result == "FAIL":
            f.write("<th>Details</th>\n")

        for case in suite.getElementsByTagName("TestCase"):
            f.write("<tr>\n")
            name = get_node_text(case.getElementsByTagName("CaseName")[0])
            result = get_node_text(case.getElementsByTagName("CaseResult")[0])
            time = get_node_text(case.getElementsByTagName("CaseTime")[0])
            if result == "FAIL":
                f.write("<td style=\"color:red\">%s</td>\n" % result)
                f.write("<td>%s</td>\n" % name)
                f.write("<td>%s</td>\n" % time)
                f.write("<td>")
                f.write("<b>Message: </b>%s, " % get_node_text(case.getElementsByTagName("CaseMessage")[0]))
                f.write("<b>Condition: </b>%s, " % get_node_text(case.getElementsByTagName("CaseCondition")[0]))
                f.write("<b>Actual: </b>%s, " % get_node_text(case.getElementsByTagName("CaseActual")[0]))
                f.write("<b>Limit: </b>%s, " % get_node_text(case.getElementsByTagName("CaseLimit")[0]))
                f.write("<b>File: </b>%s, " % get_node_text(case.getElementsByTagName("CaseFile")[0]))
                f.write("<b>Line: </b>%s" % get_node_text(case.getElementsByTagName("CaseLine")[0]))
                f.write("</td>\n")
            else:
                f.write("<td style=\"color:green\">%s</td>\n" % result)
                f.write("<td>%s</td>\n" % name)
                f.write("<td>%s</td>\n" % time)
                f.write("<td></td>\n")
            
            f.write("</tr>\n")
        f.write("</table>\n")

    f.write("<h2>Examples</h2>\n")
    f.write("<table border=\"1\">\n")
    f.write("<th> Result </th>\n")
    f.write("<th>Example Name</th>\n")
    for example in dom.getElementsByTagName("Example"):
        f.write("<tr>\n")
        result = get_node_text(example.getElementsByTagName("Result")[0])
        if result in ["FAIL", "CRASH"]:
            f.write("<td style=\"color:red\">%s</td>\n" % result)
        else:
            f.write("<td style=\"color:green\">%s</td>\n" % result)
        name =   get_node_text(example.getElementsByTagName("Name")[0])
        f.write("<td>%s</td>\n" % name)
        f.write("</tr>\n")

    f.write("</table>\n")

    f.write("</body>\n")
    f.write("</html>\n")
    f.close()
    
#
# Python Control-C handling is broken in the presence of multiple threads.  
# Signals get delivered to the runnable/running thread by default and if 
# it is blocked, the signal is simply ignored.  So we hook sigint and set 
# a global variable telling the system to shut down gracefully.
#
thread_exit = False

def sigint_hook(signal, frame):
    global thread_exit
    thread_exit = True
    return 0

#
# Waf can be configured to compile in debug or optimized modes.  In each
# case, the resulting built goes into a different directory.  If we want
# test tests to run from the correct code-base, we have to figure out which
# mode waf is running in.  This is called its active variant.
#
# XXX This function pokes around in the waf internal state file.  To be a
# little less hacky, we should add a commmand to waf to return this info
# and use that result.
#
def read_waf_active_variant():
    for line in open("build/c4che/default.cache.py").readlines():
        if line.startswith("NS3_ACTIVE_VARIANT"):
            exec(line, globals())
            break

    if options.verbose:
        print "NS3_ACTIVE_VARIANT == %s" % NS3_ACTIVE_VARIANT

#
# In general, the build process itself naturally takes care of figuring out
# which tests are built into the test runner.  For example, if waf configure
# determines that ENABLE_EMU is false due to some missing dependency,
# the tests for the emu net device simply will not be built and will 
# therefore not be included in the built test runner.
#
# Examples, however, are a different story.  In that case, we are just given
# a list of examples that could be run.  Instead of just failing, for example,
# nsc-tcp-zoo if NSC is not present, we look into the waf saved configuration
# for relevant configuration items.  
#
# XXX This function pokes around in the waf internal state file.  To be a
# little less hacky, we should add a commmand to waf to return this info
# and use that result.
#
def read_waf_config():
    for line in open("build/c4che/%s.cache.py" % NS3_ACTIVE_VARIANT).readlines():
        for item in interesting_config_items:
            if line.startswith(item):
                exec(line, globals())

    if options.verbose:
        for item in interesting_config_items:
            print "%s ==" % item, eval(item)

#
# It seems pointless to fork a process to run waf to fork a process to run
# the test runner, so we just run the test runner directly.  The main thing 
# that waf would do for us would be to sort out the shared library path but
# we can deal with that easily and do here.
#
# There can be many different ns-3 repositories on a system, and each has 
# its own shared libraries, so ns-3 doesn't hardcode a shared library search
# path -- it is cooked up dynamically, so we do that too.
#
def make_library_path():
    global LIBRARY_PATH

    LIBRARY_PATH = "LD_LIBRARY_PATH='"        

    if sys.platform == "darwin":
        LIBRARY_PATH = "DYLD_LIBRARY_PATH='"
    elif sys.platform == "win32":
        LIBRARY_PATH = "PATH='"
    elif sys.platform == "cygwin":
        LIBRARY_PATH = "PATH='"

    for path in NS3_MODULE_PATH:
        LIBRARY_PATH = LIBRARY_PATH + path + ":"

    LIBRARY_PATH = LIBRARY_PATH + "'"

def run_job_synchronously(shell_command, directory):
    cmd = "%s %s/%s/%s" % (LIBRARY_PATH, NS3_BUILDDIR, NS3_ACTIVE_VARIANT, shell_command)
    if options.verbose:
        print "Synchronously execute %s" % cmd
    proc = subprocess.Popen(cmd, shell=True, cwd=directory, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout_results = proc.communicate()[0]
    return (proc.returncode, stdout_results)

#
# This class defines a unit of testing work.  It will typically refer to
# a test suite to run using the test-runner, or an example to run directly.
#
class Job():
    def __init__(self):
        self.is_break = False
        self.is_example = False
        self.shell_command = ""
        self.display_name = ""
        self.cwd = ""
        self.tmp_file_name = ""
        self.returncode = False

    #
    # A job is either a standard job or a special job indicating that a worker
    # thread should exist.  This special job is indicated by setting is_break 
    # to true.
    #
    def set_is_break(self, is_break):
        self.is_break = is_break

    #
    # Examples are treated differently than standard test suites.  This is
    # mostly because they are completely unaware that they are being run as 
    # tests.  So we have to do some special case processing to make them look
    # like tests.
    #
    def set_is_example(self, is_example):
        self.is_example = is_example

    #
    # This is the shell command that will be executed in the job.  For example,
    #
    #  "utils/test-runner --suite=some-test-suite"
    #
    def set_shell_command(self, shell_command):
        self.shell_command = shell_command

    #
    # This is the dispaly name of the job, typically the test suite or example 
    # name.  For example,
    #
    #  "some-test-suite" or "udp-echo"
    #
    def set_display_name(self, display_name):
        self.display_name = display_name

    #
    # This is the base directory of the repository out of which the tests are
    # being run.  It will be used deep down in the testing framework to determine
    # where the source directory of the test was, and therefore where to find 
    # provided test vectors.  For example,
    #
    #  "/home/user/repos/ns-3-dev"
    #
    def set_basedir(self, basedir):
        self.basedir = basedir

    #
    # This is the current working directory that will be given to an executing
    # test as it is being run.  It will be used for examples to tell them where
    # to write all of the pcap files that we will be carefully ignoring.  For
    # example,
    #
    #  "/tmp/unchecked-traces"
    #
    def set_cwd(self, cwd):
        self.cwd = cwd

    #
    # This is the temporary results file name that will be given to an executing 
    # test as it is being run.  We will be running all of our tests in parallel
    # so there must be multiple temporary output files.  These will be collected
    # into a single XML file at the end and then be deleted.  The file names are
    # just giant random numbers, for example
    #
    #  "/tmp/testpy/5437925246732857"
    #
    def set_tmp_file_name(self, tmp_file_name):
        self.tmp_file_name = tmp_file_name

    #
    # The return code received when the job process is executed.
    #
    def set_returncode(self, returncode):
        self.returncode = returncode

#
# The worker thread class that handles the actual running of a given test.
# Once spawned, it receives requests for work through its input_queue and
# ships the results back through the output_queue.
#
class worker_thread(threading.Thread):
    def __init__(self, input_queue, output_queue):
        threading.Thread.__init__(self)
        self.input_queue = input_queue
        self.output_queue = output_queue

    def run(self):
        while True:
            job = self.input_queue.get()
            #
            # Worker threads continue running until explicitly told to stop with
            # a special job.
            #
            if job.is_break:
                return
            #
            # If the global interrupt handler sets the thread_exit variable,
            # we stop doing real work and just report back a "break" in the
            # normal command processing has happened.
            #
            if thread_exit == True:
                job.set_is_break(True)
                self.output_queue.put(job)
                continue
            #
            # Otherwise go about the business of running tests as normal.
            #
            else:
                if options.verbose:
                    print "Launch %s" % job.shell_command

                if job.is_example:
                    #
                    # If we have an example, the shell command is all we need to
                    # know.  It will be something like "examples/udp-echo"
                    #
                    (job.returncode, standard_out) = run_job_synchronously(job.shell_command, job.cwd)
                else:
                    #
                    # If we're a test suite, we need to provide a little more info
                    # to the test runner, specifically the base directory and temp
                    # file name
                    #
                    (job.returncode, standard_out) = run_job_synchronously(job.shell_command + " --basedir=%s --out=%s" %
                        (job.basedir, job.tmp_file_name), job.cwd)

                if options.verbose:
                    print standard_out

                self.output_queue.put(job)

#
# This is the main function that does the work of interacting with the test-runner
# itself.
#
def run_tests():
    #
    # Run waf to make sure that everything is built, configured and ready to go
    # unless we are explicitly told not to.
    #
    if options.nowaf == False:
        proc = subprocess.Popen("./waf", shell=True)
        proc.communicate()

    #
    # Pull some interesting configuration information out of waf, primarily
    # so we can know where executables can be found, but also to tell us what
    # pieces of the system have been built.  This will tell us what examples 
    # are runnable.
    #
    read_waf_active_variant()
    read_waf_config()
    make_library_path()

    #
    # There are a couple of options that imply we can to exit before starting
    # up a bunch of threads and running tests.  Let's detect these cases and 
    # handle them without doing all of the hard work.
    #
    if options.kinds:
        (rc, standard_out) = run_job_synchronously("utils/test-runner --kinds", os.getcwd())
        print standard_out

    if options.list:
        (rc, standard_out) = run_job_synchronously("utils/test-runner --list", os.getcwd())
        print standard_out

    if options.kinds or options.list:
        return

    #
    # We communicate results in two ways.  First, a simple message relating 
    # PASS, FAIL, or SKIP is always written to the standard output.  It is 
    # expected that this will be one of the main use cases.  A developer can
    # just run test.py with no options and see that all of the tests still 
    # pass.
    #
    # The second main use case is when detailed status is requested (with the
    # --text or --html options).  Typicall this will be text if a developer
    # finds a problem, or HTML for nightly builds.  In these cases, an
    # XML file is written containing the status messages from the test suites.
    # This file is then read and translated into text or HTML.  It is expected
    # that nobody will really be interested in the XML, so we write it to 
    # somewhere in /tmp with a random name to avoid collisions.  Just in case 
    # some strange once-in-a-lifetime error occurs, we always write the info
    # so it can be found, we just may not use it.
    #
    # When we run examples as smoke tests, they are going to want to create
    # lots and lots of trace files.  We aren't really interested in the contents
    # of the trace files, so we also just stash them off in /tmp somewhere.
    #
    if not os.path.exists(TMP_OUTPUT_DIR):
        os.makedirs(TMP_OUTPUT_DIR)

    if not os.path.exists(TMP_TRACES_DIR):
        os.makedirs(TMP_TRACES_DIR)

    #
    # Create the main output file and start filling it with XML.  We need to 
    # do this since the tests will just append individual results to this file.
    #
    xml_results_file = TMP_OUTPUT_DIR + "%d.xml" % random.randint(0, sys.maxint)
    f = open(xml_results_file, 'w')
    f.write('<?xml version="1.0"?>\n')
    f.write('<TestResults>\n')
    f.close()

    #
    # We need to figure out what test suites to execute.  We are either given one 
    # suite or example explicitly via the --suite or --example option, or we
    # need to call into the test runner and ask it to list all of the available
    # test suites.  Further, we need to provide the constraint information if it
    # has been given to us.
    # 
    # This translates into allowing the following options with respect to the 
    # suites
    #
    #  ./test,py:                                           run all of the suites
    #  ./test.py --constrain=unit:                          run all unit suites
    #  ./test,py --suite=some-test-suite:                   run the single suite
    #  ./test,py --example=udp-echo:                        run no test suites
    #  ./test,py --suite=some-suite --example=some-example: run the single suite
    #
    # We can also use the --constrain option to provide an ordering of test 
    # execution quite easily.
    #
    if len(options.suite):
        suites = options.suite + "\n"
    elif len(options.example) == 0:
        if len(options.constrain):
            (rc, suites) = run_job_synchronously("utils/test-runner --list --constrain=%s" % options.constrain, os.getcwd())
        else:
            (rc, suites) = run_job_synchronously("utils/test-runner --list", os.getcwd())
    else:
        suites = ""

    #
    # suite_list will either a single test suite name that the user has 
    # indicated she wants to run or a list of test suites provided by
    # the test-runner possibly according to user provided constraints.
    # We go through the trouble of setting up the parallel execution 
    # even in the case of a single suite to avoid having two process the
    # results in two different places.
    #
    suite_list = suites.split('\n')

    #
    # We now have a possibly large number of test suites to run, so we want to
    # run them in parallel.  We're going to spin up a number of worker threads
    # that will run our test jobs for us.
    #
    input_queue = Queue.Queue(0)
    output_queue = Queue.Queue(0)

    jobs = 0
    threads=[]

    #
    # In Python 2.6 you can just use multiprocessing module, but we don't want
    # to introduce that dependency yet; so we jump through a few hoops.
    #
    processors = 1

    if 'SC_NPROCESSORS_ONLN'in os.sysconf_names:
        processors = os.sysconf('SC_NPROCESSORS_ONLN')
    else:
        proc = subprocess.Popen("sysctl -n hw.ncpu", shell = True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout_results, stderr_results = proc.communicate()
        if len(stderr_results) == 0:
            processors = int(stdout_results)

    #
    # Now, spin up one thread per processor which will eventually mean one test
    # per processor running concurrently.
    #
    for i in range(processors):
        thread = worker_thread(input_queue, output_queue)
        threads.append(thread)
        thread.start()

    #
    # We now have worker threads spun up, and a list of work to do.  So, run 
    # through the list of test suites and dispatch a job to run each one.
    # 
    # Dispatching will run with unlimited speed and the worker threads will 
    # execute as fast as possible from the queue.
    #
    for test in suite_list:
        if len(test):
            job = Job()
            job.set_is_example(False)
            job.set_display_name(test)
            job.set_tmp_file_name(TMP_OUTPUT_DIR + "%d" % random.randint(0, sys.maxint))
            job.set_cwd(os.getcwd())
            job.set_basedir(os.getcwd())
            job.set_shell_command("utils/test-runner --suite='%s'" % test)

            if options.verbose:
                print "Queue %s" % test

            input_queue.put(job)
            jobs = jobs + 1
    
    #
    # We've taken care of the discovered or specified test suites.  Now we
    # have to deal with examples run as smoke tests.  We have a list of all of
    # the example programs it makes sense to try and run.  Each example will
    # have a condition associated with it that must evaluate to true for us
    # to try and execute it.  This is used to determine if the example has
    # a dependency that is not satisfied.  For example, if an example depends
    # on NSC being configured by waf, that example should have a condition
    # that evaluates to true if NSC is enabled.  For example,
    #
    #      ("tcp-nsc-zoo", "ENABLE_NSC == True"),
    #
    # In this case, the example "tcp-nsc-zoo" will only be run if we find the
    # waf configuration variable "ENABLE_NSC" to be True.
    #
    # We don't care at all how the trace files come out, so we just write them 
    # to a single temporary directory.
    #
    # XXX As it stands, all of the trace files have unique names, and so file
    # collisions can only happen if two instances of an example are running in
    # two versions of the test.py process concurrently.  We may want to create
    # uniquely named temporary traces directories to avoid this problem.
    #
    # We need to figure out what examples to execute.  We are either given one 
    # suite or example explicitly via the --suite or --example option, or we
    # need to walk the list of examples looking for available example 
    # conditions.
    #
    # This translates into allowing the following options with respect to the 
    # suites
    #
    #  ./test,py:                                           run all of the examples
    #  ./test.py --constrain=unit                           run no examples
    #  ./test.py --constrain=example                       run all of the examples
    #  ./test,py --suite=some-test-suite:                   run no examples
    #  ./test,py --example=some-example:                    run the single example
    #  ./test,py --suite=some-suite --example=some-example: run the single example
    #
    # XXX could use constrain to separate out examples used for performance 
    # testing
    #
    if len(options.suite) == 0 and len(options.example) == 0:
        if len(options.constrain) == 0 or options.constrain == "example":
            for test, condition in example_tests:
                if eval(condition) == True:
                    job = Job()
                    job.set_is_example(True)
                    job.set_display_name(test)
                    job.set_tmp_file_name("")
                    job.set_cwd(TMP_TRACES_DIR)
                    job.set_basedir(os.getcwd())
                    job.set_shell_command("examples/%s" % test)

                    if options.verbose:
                        print "Queue %s" % test

                    input_queue.put(job)
                    jobs = jobs + 1
    elif len(options.example):
        #
        # If you tell me to run an example, I will try and run the example
        # irrespective of any condition.
        #
        job = Job()
        job.set_is_example(True)
        job.set_display_name(options.example)
        job.set_tmp_file_name("")
        job.set_cwd(TMP_TRACES_DIR)
        job.set_basedir(os.getcwd())
        job.set_shell_command("examples/%s" % options.example)
        
        if options.verbose:
            print "Queue %s" % test

        input_queue.put(job)
        jobs = jobs + 1

    #
    # Tell the worker threads to pack up and go home for the day.  Each one
    # will exit when they see their is_break task.
    #
    for i in range(processors):
        job = Job()
        job.set_is_break(True)
        input_queue.put(job)

    #
    # Now all of the tests have been dispatched, so all we have to do here
    # in the main thread is to wait for them to complete.  Keyboard interrupt
    # handling is broken as mentioned above.  We use a signal handler to catch
    # sigint and set a global variable.  When the worker threads sense this
    # they stop doing real work and will just start throwing jobs back at us
    # with is_break set to True.  In this case, there are no real results so we 
    # ignore them.  If there are real results, we always print PASS or FAIL to
    # standard out as a quick indication of what happened.
    #
    for i in range(jobs):
        job = output_queue.get()
        if job.is_break:
            continue

        if job.is_example:
            kind = "Example"
        else:
            kind = "TestSuite"

        if job.returncode == 0:
            status = "PASS"
        else:
            status = "FAIL"

        print "%s: %s %s" % (status, kind, job.display_name)

        if job.is_example == True:
            #
            # Examples are the odd man out here.  They are written without any
            # knowledge that they are going to be run as a test, so we need to 
            # cook up some kind of output for them.  We're writing an xml file,
            # so we do some simple XML that says we ran the example.
            #
            # XXX We could add some timing information to the examples, i.e. run
            # them through time and print the results here.
            #
            f = open(xml_results_file, 'a')
            f.write('<Example>\n')
            example_name = "  <Name>%s</Name>\n" % job.display_name
            f.write(example_name)
            if job.returncode == 0:
                f.write('  <Result>PASS</Result>\n')
            elif job.returncode == 1:
                f.write('  <Result>FAIL</Result>\n')
            else:
                f.write('  <Result>CRASH</Result>\n')

            f.write('</Example>\n')
            f.close()
        else:
            #
            # If we're not running an example, we're running a test suite.
            # These puppies are running concurrently and generating output
            # that was written to a temporary file to avoid collisions.
            #
            # Now that we are executing sequentially in the main thread, we can
            # concatenate the contents of the associated temp file to the main 
            # results file and remove that temp file.
            #
            # One thing to consider is that a test suite can crash just as
            # well as any other program, so we need to deal with that 
            # possibility as well.  If it ran correctly it will return 0
            # if it passed, or 1 if it failed.  In this case, we can count
            # on the results file it saved being complete.  If it crashed, it 
            # will return some other code, and the file should be considered 
            # corrupt and useless.  If the suite didn't create any XML, then
            # we're going to have to do it ourselves.
            #
            if job.returncode == 0 or job.returncode == 1:
                f_to = open(xml_results_file, 'a')
                f_from = open(job.tmp_file_name, 'r')
                f_to.write(f_from.read())
                f_to.close()
                f_from.close()
            else:
                f = open(xml_results_file, 'a')
                f.write("<TestSuite>\n")
                f.write("  <SuiteName>%s</SuiteName>\n" % job.display_name)
                f.write('  <SuiteResult>CRASH</SuiteResult>\n')
                f.write('  <SuiteTime>Execution times not available</SuiteTime>\n')
                f.write("</TestSuite>\n")
                f.close()

            os.remove(job.tmp_file_name)

    #
    # We have all of the tests run and the results written out.  One final 
    # bit of housekeeping is to wait for all of the threads to close down
    # so we can exit gracefully.
    #
    for thread in threads:
        thread.join()
    
    #
    # Back at the beginning of time, we started the body of an XML document
    # since the test suites and examples were going to just write their 
    # individual pieces.  So, we need to finish off and close out the XML 
    # document
    #
    f = open(xml_results_file, 'a')
    f.write('</TestResults>\n')
    f.close()

    #
    # The last things to do are to translate the XML results file to "human
    # readable form" if the user asked for it (or make an XML file somewhere)
    #
    if len(options.html):
        translate_to_html(xml_results_file, options.html)

    if len(options.text):
        translate_to_text(xml_results_file, options.text)

    if len(options.xml):
        shutil.copyfile(xml_results_file, options.xml)

def main(argv):
    random.seed()

    parser = optparse.OptionParser()
    parser.add_option("-c", "--constrain", action="store", type="string", dest="constrain", default="",
                      metavar="KIND",
                      help="constrain the test-runner by kind of test")

    parser.add_option("-e", "--example", action="store", type="string", dest="example", default="",
                      metavar="EXAMPLE",
                      help="specify a single example to run")

    parser.add_option("-k", "--kinds", action="store_true", dest="kinds", default=False,
                      help="print the kinds of tests available")

    parser.add_option("-l", "--list", action="store_true", dest="list", default=False,
                      help="print the list of known tests")

    parser.add_option("-n", "--nowaf", action="store_true", dest="nowaf", default=False,
                      help="do not run waf before starting testing")

    parser.add_option("-s", "--suite", action="store", type="string", dest="suite", default="",
                      metavar="TEST-SUITE",
                      help="specify a single test suite to run")

    parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False,
                      help="print progress and informational messages")

    parser.add_option("-w", "--web", "--html", action="store", type="string", dest="html", default="",
                      metavar="HTML-FILE",
                      help="write detailed test results into HTML-FILE.html")

    parser.add_option("-t", "--text", action="store", type="string", dest="text", default="",
                      metavar="TEXT-FILE",
                      help="write detailed test results into TEXT-FILE.txt")

    parser.add_option("-x", "--xml", action="store", type="string", dest="xml", default="",
                      metavar="XML-FILE",
                      help="write detailed test results into XML-FILE.xml")

    global options
    options = parser.parse_args()[0]
    signal.signal(signal.SIGINT, sigint_hook)
    run_tests()
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
