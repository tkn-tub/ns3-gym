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
    "ENABLE_NSC",
    "ENABLE_REAL_TIME",
    "ENABLE_EXAMPLES",
]

ENABLE_NSC = False
ENABLE_REAL_TIME = False
ENABLE_EXAMPLES = True

#
# If the user has constrained us to run certain kinds of tests, we can tell waf
# to only build
core_kinds = ["bvt", "core", "system", "unit"]

#
# A list of examples to run as smoke tests just to ensure that they remain 
# buildable and runnable over time.  Also a condition under which to run
# the example (from the waf configuration).
#
# XXX Should this not be read from a configuration file somewhere and not
# hardcoded.
#
example_tests = [
    ("csma/csma-bridge", "True"),
    ("csma/csma-bridge-one-hop", "True"),
    ("csma/csma-broadcast", "True"),
    ("csma/csma-multicast", "True"),
    ("csma/csma-one-subnet", "True"),
    ("csma/csma-packet-socket", "True"),
    ("csma/csma-ping", "True"),
    ("csma/csma-raw-ip-socket", "True"),
    ("csma/csma-star", "True"),

    ("emulation/emu-ping", "False"),
    ("emulation/emu-udp-echo", "False"),

    ("error-model/simple-error-model", "True"),

    ("ipv6/icmpv6-redirect", "True"),
    ("ipv6/ping6", "True"),
    ("ipv6/radvd", "True"),
    ("ipv6/radvd-two-prefix", "True"),    
    ("ipv6/test-ipv6", "True"),

    ("mesh/mesh", "True"),

    ("naming/object-names", "True"),

    ("realtime/realtime-udp-echo", "ENABLE_REAL_TIME == True"),

    ("routing/dynamic-global-routing", "True"),
    ("routing/global-injection-slash32", "True"),
    ("routing/global-routing-slash32", "True"),
    ("routing/mixed-global-routing", "True"),
    ("routing/nix-simple", "True"),
    ("routing/nms-p2p-nix", "False"), # Takes too long to run
    ("routing/simple-alternate-routing", "True"),
    ("routing/simple-global-routing", "True"),
    ("routing/simple-point-to-point-olsr", "True"),
    ("routing/simple-routing-ping6", "True"),
    ("routing/static-routing-slash32", "True"),

    ("stats/wifi-example-sim", "True"),

    ("tap/tap-wifi-dumbbell", "False"), # Requires manual configuration

    ("tcp/star", "True"),
    ("tcp/tcp-large-transfer", "True"),
    ("tcp/tcp-nsc-lfn", "ENABLE_NSC == True"),
    ("tcp/tcp-nsc-zoo", "ENABLE_NSC == True"),
    ("tcp/tcp-star-server", "True"),

    ("tunneling/virtual-net-device", "True"),

    ("tutorial/first", "True"),
    ("tutorial/hello-simulator", "True"),
    ("tutorial/second", "True"),
    ("tutorial/third", "True"),
    ("tutorial/fourth", "True"),

    ("udp/udp-echo", "True"),

    ("wireless/mixed-wireless", "True"),
    ("wireless/multirate", "False"), # Takes too long to run
    ("wireless/simple-wifi-frame-aggregation", "True"),
    ("wireless/wifi-adhoc", "False"), # Takes too long to run
    ("wireless/wifi-ap --verbose=0", "True"), # Don't let it spew to stdout
    ("wireless/wifi-clear-channel-cmu", "False"), # Requires specific hardware
    ("wireless/wifi-simple-adhoc", "True"),
    ("wireless/wifi-simple-adhoc-grid", "True"),
    ("wireless/wifi-simple-infra", "True"),
    ("wireless/wifi-simple-interference", "True"),
    ("wireless/wifi-wired-bridging", "True"),
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
# A simple example of writing a text file with a test result summary.  It is 
# expected that this output will be fine for developers looking for problems.
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
                    for details in case.getElementsByTagName("FailureDetails"):
                        f.write("    Details:\n")
                        f.write("      Message:   %s\n" % get_node_text(details.getElementsByTagName("Message")[0]))
                        f.write("      Condition: %s\n" % get_node_text(details.getElementsByTagName("Condition")[0]))
                        f.write("      Actual:    %s\n" % get_node_text(details.getElementsByTagName("Actual")[0]))
                        f.write("      Limit:     %s\n" % get_node_text(details.getElementsByTagName("Limit")[0]))
                        f.write("      File:      %s\n" % get_node_text(details.getElementsByTagName("File")[0]))
                        f.write("      Line:      %s\n" % get_node_text(details.getElementsByTagName("Line")[0]))

    for example in dom.getElementsByTagName("Example"):
        result = get_node_text(example.getElementsByTagName("Result")[0])
        name = get_node_text(example.getElementsByTagName("Name")[0])
        output = "%s: Example \"%s\"\n" % (result, name)
        f.write(output)

    f.close()
    
#
# A simple example of writing an HTML file with a test result summary.  It is 
# expected that this will eventually be made prettier as time progresses and
# we have time to tweak it.  This may end up being moved to a separate module
# since it will probably grow over time.
#
def translate_to_html(results_file, html_file):
    f = open(html_file, 'w')
    f.write("<html>\n")
    f.write("<body>\n")
    f.write("<center><h1>ns-3 Test Results</h1></center>\n")

    #
    # Read and parse the whole results file.
    #
    dom = xml.dom.minidom.parse(results_file)

    #
    # Iterate through the test suites
    #
    f.write("<h2>Test Suites</h2>\n")
    for suite in dom.getElementsByTagName("TestSuite"):
     
        #
        # For each test suite, get its name, result and execution time info
        #
        name = get_node_text(suite.getElementsByTagName("SuiteName")[0])
        result = get_node_text(suite.getElementsByTagName("SuiteResult")[0])
        time = get_node_text(suite.getElementsByTagName("SuiteTime")[0])

        # 
        # Print a level three header in green with the result, name and time.
        # If the test suite passed, the header is printed in green, otherwise
        # it is printed in red.
        #
        if result == "PASS":
            f.write("<h3 style=\"color:green\">%s: %s (%s)</h3>\n" % (result, name, time))
        else:
            f.write("<h3 style=\"color:red\">%s: %s (%s)</h3>\n" % (result, name, time))

        #
        # The test case information goes in a table.
        #
        f.write("<table border=\"1\">\n")

        #
        # The first column of the table has the heading Result
        #
        f.write("<th> Result </th>\n")

        #
        # If the suite crashed, there is no further information, so just
        # delare a new table row with the result (CRASH) in it.  Looks like:
        #
        #   +--------+
        #   | Result |
        #   +--------+
        #   | CRASH  |
        #   +--------+
        #
        # Then go on to the next test suite.  Valgrind errors look the same.
        #
        if result in ["CRASH", "VALGR"]:
            f.write("<tr>\n")
            f.write("<td style=\"color:red\">%s</td>\n" % result)
            f.write("</tr>\n")
            f.write("</table>\n")
            continue

        #
        # If the suite didn't crash, we expect more information, so fill out
        # the table heading row.  Like,
        #
        #   +--------+----------------+------+
        #   | Result | Test Case Name | Time |
        #   +--------+----------------+------+
        #
        f.write("<th>Test Case Name</th>\n")
        f.write("<th> Time </th>\n")

        #
        # If the test case failed, we need to print out some failure details
        # so extend the heading row again.  Like,
        #
        #   +--------+----------------+------+-----------------+
        #   | Result | Test Case Name | Time | Failure Details |
        #   +--------+----------------+------+-----------------+
        #
        if result == "FAIL":
            f.write("<th>Failure Details</th>\n")

        #
        # Now iterate through all of the test cases.
        #
        for case in suite.getElementsByTagName("TestCase"):

            #
            # Get the name, result and timing information from xml to use in
            # printing table below.
            #
            name = get_node_text(case.getElementsByTagName("CaseName")[0])
            result = get_node_text(case.getElementsByTagName("CaseResult")[0])
            time = get_node_text(case.getElementsByTagName("CaseTime")[0])

            #
            # If the test case failed, we iterate through possibly multiple
            # failure details
            #
            if result == "FAIL":
                #
                # There can be multiple failures for each test case.  The first
                # row always gets the result, name and timing information along
                # with the failure details.  Remaining failures don't duplicate
                # this information but just get blanks for readability.  Like,
                #
                #   +--------+----------------+------+-----------------+
                #   | Result | Test Case Name | Time | Failure Details |
                #   +--------+----------------+------+-----------------+
                #   |  FAIL  | The name       | time | It's busted     |   
                #   +--------+----------------+------+-----------------+
                #   |        |                |      | Really broken   |   
                #   +--------+----------------+------+-----------------+
                #   |        |                |      | Busted bad      |   
                #   +--------+----------------+------+-----------------+
                #

                first_row = True
                for details in case.getElementsByTagName("FailureDetails"):

                    #
                    # Start a new row in the table for each possible Failure Detail
                    #
                    f.write("<tr>\n")

                    if first_row:
                        first_row = False
                        f.write("<td style=\"color:red\">%s</td>\n" % result)
                        f.write("<td>%s</td>\n" % name)
                        f.write("<td>%s</td>\n" % time)
                    else:
                        f.write("<td></td>\n")
                        f.write("<td></td>\n")
                        f.write("<td></td>\n")

                    f.write("<td>")
                    f.write("<b>Message: </b>%s, " % get_node_text(details.getElementsByTagName("Message")[0]))
                    f.write("<b>Condition: </b>%s, " % get_node_text(details.getElementsByTagName("Condition")[0]))
                    f.write("<b>Actual: </b>%s, " % get_node_text(details.getElementsByTagName("Actual")[0]))
                    f.write("<b>Limit: </b>%s, " % get_node_text(details.getElementsByTagName("Limit")[0]))
                    f.write("<b>File: </b>%s, " % get_node_text(details.getElementsByTagName("File")[0]))
                    f.write("<b>Line: </b>%s" % get_node_text(details.getElementsByTagName("Line")[0]))
                    f.write("</td>\n")
                    
                    #
                    # End the table row
                    #
                    f.write("</td>\n")
            else:
                #
                # If this particular test case passed, then we just print the PASS
                # result in green, followed by the test case name and its execution
                # time information.  These go off in <td> ... </td> table data.
                # The details table entry is left blank.
                #
                #   +--------+----------------+------+---------+
                #   | Result | Test Case Name | Time | Details |
                #   +--------+----------------+------+---------+
                #   |  PASS  | The name       | time |         |   
                #   +--------+----------------+------+---------+
                #
                f.write("<tr>\n")
                f.write("<td style=\"color:green\">%s</td>\n" % result)
                f.write("<td>%s</td>\n" % name)
                f.write("<td>%s</td>\n" % time)
                f.write("<td></td>\n")
                f.write("</tr>\n")
        #
        # All of the rows are written, so we need to end the table.
        #
        f.write("</table>\n")

    #
    # That's it for all of the test suites.  Now we have to do something about 
    # our examples.
    #
    f.write("<h2>Examples</h2>\n")

    #
    # Example status is rendered in a table just like the suites.
    #
    f.write("<table border=\"1\">\n")

    #
    # The table headings look like,
    #
    #   +--------+--------------+
    #   | Result | Example Name |
    #   +--------+--------------+
    #
    f.write("<th> Result </th>\n")
    f.write("<th>Example Name</th>\n")

    #
    # Now iterate through all of the examples
    #
    for example in dom.getElementsByTagName("Example"):
        
        #
        # Start a new row for each example
        #
        f.write("<tr>\n")
        
        #
        # Get the result and name of the example in question
        #
        result = get_node_text(example.getElementsByTagName("Result")[0])
        name =   get_node_text(example.getElementsByTagName("Name")[0])

        #
        # If the example either failed or crashed, print its result status
        # in red; otherwise green.  This goes in a <td> ... </td> table data
        #
        if result == "PASS":
            f.write("<td style=\"color:green\">%s</td>\n" % result)
        else:
            f.write("<td style=\"color:red\">%s</td>\n" % result)

        #
        # Write the example name as a new tagle data.
        #
        f.write("<td>%s</td>\n" % name)

        #
        # That's it for the current example, so terminate the row.
        #
        f.write("</tr>\n")

    #
    # That's it for the table of examples, so terminate the table.
    #
    f.write("</table>\n")

    #
    # And that's it for the report, so finish up.
    #
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

    LIBRARY_PATH = "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:'"        

    if sys.platform == "darwin":
        LIBRARY_PATH = "DYLD_LIBRARY_PATH='"
    elif sys.platform == "win32":
        LIBRARY_PATH = "PATH=$PATH:'"
    elif sys.platform == "cygwin":
        LIBRARY_PATH = "PATH=$PATH:'"

    for path in NS3_MODULE_PATH:
        LIBRARY_PATH = LIBRARY_PATH + path + ":"

    LIBRARY_PATH = LIBRARY_PATH + "'"

    if options.verbose:
        print "LIBRARY_PATH == %s" % LIBRARY_PATH

def run_job_synchronously(shell_command, directory, valgrind):
    if valgrind:
        cmd = "%s valgrind --error-exitcode=2 %s/%s/%s" % (LIBRARY_PATH, NS3_BUILDDIR, NS3_ACTIVE_VARIANT, shell_command)
    else:
        cmd = "%s %s/%s/%s" % (LIBRARY_PATH, NS3_BUILDDIR, NS3_ACTIVE_VARIANT, shell_command)

    if options.verbose:
        print "Synchronously execute %s" % cmd

    proc = subprocess.Popen(cmd, shell=True, cwd=directory, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout_results, stderr_results = proc.communicate()
    return (proc.returncode, stdout_results, stderr_results)

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
                    (job.returncode, standard_out, standard_err) = run_job_synchronously(job.shell_command, job.cwd,
                                                                                         options.valgrind)
                else:
                    #
                    # If we're a test suite, we need to provide a little more info
                    # to the test runner, specifically the base directory and temp
                    # file name
                    #
                    (job.returncode, standard_out, standard_err) = run_job_synchronously(job.shell_command + 
                        " --basedir=%s --out=%s" % (job.basedir, job.tmp_file_name), job.cwd, options.valgrind)

                if options.verbose:
                    print "returncode = %d" % job.returncode
                    print "---------- beign standard out ----------"
                    print standard_out
                    print "---------- begin standard err ----------"
                    print standard_err
                    print "---------- end standard err ----------"

                self.output_queue.put(job)

#
# This is the main function that does the work of interacting with the test-runner
# itself.
#
def run_tests():
    #
    # Run waf to make sure that everything is built, configured and ready to go
    # unless we are explicitly told not to.  We want to be careful about causing
    # our users pain while waiting for extraneous stuff to compile and link, so
    # we allow users that know what they''re doing to not invoke waf at all.
    #
    if not options.nowaf:

        #
        # If the user is running the "kinds" or "list" options, there is an 
        # implied dependency on the test-runner since we call that program
        # if those options are selected.  We will exit after processing those
        # options, so if we see them, we can safely only build the test-runner.
        #
        # If the user has constrained us to running only a particular type of
        # file, we can only ask waf to build what we know will be necessary.
        # For example, if the user only wants to run BVT tests, we only have
        # to build the test-runner and can ignore all of the examples.
        #
        if options.kinds or options.list or (len(options.constrain) and options.constrain in core_kinds):
            proc = subprocess.Popen("./waf --target=test-runner", shell=True)
        else:
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
        (rc, standard_out, standard_err) = run_job_synchronously("utils/test-runner --kinds", os.getcwd(), False)
        print standard_out

    if options.list:
        (rc, standard_out, standard_err) = run_job_synchronously("utils/test-runner --list", os.getcwd(), False)
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
    #  ./test,py:                                           run all of the suites and examples
    #  ./test.py --constrain=core:                          run all of the suites of all kinds
    #  ./test.py --constrain=unit:                          run all unit suites
    #  ./test,py --suite=some-test-suite:                   run a single suite
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
            (rc, suites, standard_err) = run_job_synchronously("utils/test-runner --list --constrain=%s" % 
                options.constrain, os.getcwd(), False)
        else:
            (rc, suites, standard_err) = run_job_synchronously("utils/test-runner --list", os.getcwd(), False)
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
    total_tests = 0
    for test in suite_list:
        if len(test):
            job = Job()
            job.set_is_example(False)
            job.set_display_name(test)
            job.set_tmp_file_name(TMP_OUTPUT_DIR + "%d" % random.randint(0, sys.maxint))
            job.set_cwd(os.getcwd())
            job.set_basedir(os.getcwd())
            if (options.multiple):
                multiple = " --multiple"
            else:
                multiple = ""

            job.set_shell_command("utils/test-runner --suite='%s'%s" % (test, multiple))

            if options.verbose:
                print "Queue %s" % test

            input_queue.put(job)
            jobs = jobs + 1
            total_tests = total_tests + 1
    
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
    #  ./test.py --constrain=example                        run all of the examples
    #  ./test,py --suite=some-test-suite:                   run no examples
    #  ./test,py --example=some-example:                    run the single example
    #  ./test,py --suite=some-suite --example=some-example: run the single example
    #
    # XXX could use constrain to separate out examples used for performance 
    # testing
    #
    if len(options.suite) == 0 and len(options.example) == 0:
        if len(options.constrain) == 0 or options.constrain == "example":
            if ENABLE_EXAMPLES:
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
                        total_tests = total_tests + 1

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
        total_tests = total_tests + 1

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
    passed_tests = 0
    failed_tests = 0
    crashed_tests = 0
    valgrind_errors = 0
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
            passed_tests = passed_tests + 1
        elif job.returncode == 1:
            failed_tests = failed_tests + 1
            status = "FAIL"
        elif job.returncode == 2:
            valgrind_errors = valgrind_errors + 1
            status = "VALGR"
        else:
            crashed_tests = crashed_tests + 1
            status = "CRASH"

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
            elif job.returncode == 2:
                f.write('  <Result>VALGR</Result>\n')
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
            # Another issue is how to deal with a valgrind error.  If we run
            # a test suite under valgrind and it passes, we will get a return
            # code of 0 and there will be a valid xml results file since the code
            # ran to completion.  If we get a return code of 1 under valgrind,
            # the test case failed, but valgrind did not find any problems so the
            # test case return code was passed through.  We will have a valid xml
            # results file here as well since the test suite ran.  If we see a 
            # return code of 2, this means that valgrind found an error (we asked
            # it to return 2 if it found a problem in run_job_synchronously) but
            # the suite ran to completion so there is a valid xml results file.
            # If the suite crashes under valgrind we will see some other error 
            # return code (like 139).  If valgrind finds an illegal instruction or
            # some other strange problem, it will die with its own strange return
            # code (like 132).  However, if the test crashes by itself, not under
            # valgrind we will also see some other return code.
            #
            # If the return code is 0, 1, or 2, we have a valid xml file.  If we 
            # get another return code, we have no xml and we can't really say what
            # happened -- maybe the TestSuite crashed, maybe valgrind crashed due
            # to an illegal instruction.  If we get something beside 0-2, we assume
            # a crash and fake up an xml entry.  After this is all done, we still
            # need to indicate a valgrind error somehow, so we fake up an xml entry
            # with a VALGR result.  Thus, in the case of a working TestSuite that
            # fails valgrind, we'll see the PASS entry for the working TestSuite
            # followed by a VALGR failing test suite of the same name.
            #
            if job.returncode == 0 or job.returncode == 1 or job.returncode == 2:
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

            if job.returncode == 2:
                f = open(xml_results_file, 'a')
                f.write("<TestSuite>\n")
                f.write("  <SuiteName>%s</SuiteName>\n" % job.display_name)
                f.write('  <SuiteResult>VALGR</SuiteResult>\n')
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
    # Print a quick summary of events
    #
    print "%d of %d tests passed (%d passed, %d failed, %d crashed, %d valgrind errors)" % (passed_tests, total_tests, 
        passed_tests, failed_tests, crashed_tests, valgrind_errors)
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

    if passed_tests == total_tests:
        return 0 # success
    else:
        return 1 # catchall for general errors

def main(argv):
    random.seed()

    parser = optparse.OptionParser()
    parser.add_option("-c", "--constrain", action="store", type="string", dest="constrain", default="",
                      metavar="KIND",
                      help="constrain the test-runner by kind of test")

    parser.add_option("-e", "--example", action="store", type="string", dest="example", default="",
                      metavar="EXAMPLE",
                      help="specify a single example to run")

    parser.add_option("-g", "--grind", action="store_true", dest="valgrind", default=False,
                      help="run the test suites and examples using valgrind")

    parser.add_option("-k", "--kinds", action="store_true", dest="kinds", default=False,
                      help="print the kinds of tests available")

    parser.add_option("-l", "--list", action="store_true", dest="list", default=False,
                      help="print the list of known tests")

    parser.add_option("-m", "--multiple", action="store_true", dest="multiple", default=False,
                      help="report multiple failures from test suites and test cases")

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
    
    return run_tests()

if __name__ == '__main__':
    sys.exit(main(sys.argv))
