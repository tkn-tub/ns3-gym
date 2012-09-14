.. include:: replace.txt

Testing framework
-----------------

ns-3 consists of a simulation core engine, a set of models, example programs, 
and tests.  Over time, new contributors contribute models, tests, and 
examples.  A Python test program ``test.py`` serves as the test
execution manager; ``test.py`` can run test code and examples to
look for regressions, can output the results into a number of forms, and
can manage code coverage analysis tools.  On top of this, we layer 
*Buildbots* that are automated build robots that perform
robustness testing by running the test framework on different systems
and with different configuration options.

BuildBots
*********

At the highest level of ns-3 testing are the buildbots (build robots).  
If you are unfamiliar with
this system look at `<http://djmitche.github.com/buildbot/docs/0.7.11/>`_.  
This is an open-source automated system that allows |ns3| to be rebuilt
and tested each time something has changed.  By running the buildbots on a number
of different systems we can ensure that |ns3| builds and executes
properly on all of its supported systems.

Users (and developers) typically will not interact with the buildbot system other 
than to read its messages regarding test results.  If a failure is detected in 
one of the automated build and test jobs, the buildbot will send an email to the
*ns-developers* mailing list.  This email will look something like::

  The Buildbot has detected a new failure of osx-ppc-g++-4.2 on NsNam.
  Full details are available at:
   http://ns-regression.ee.washington.edu:8010/builders/osx-ppc-g%2B%2B-4.2/builds/0
  
  Buildbot URL: http://ns-regression.ee.washington.edu:8010/
  
  Buildslave for this Build: darwin-ppc
  
  Build Reason: The web-page 'force build' button was pressed by 'ww': ww
  
  Build Source Stamp: HEAD
  Blamelist: 
  
  BUILD FAILED: failed shell_5 shell_6 shell_7 shell_8 shell_9 shell_10 shell_11 shell_12 
  
  sincerely,
  -The Buildbot

In the full details URL shown in the email, one can search for the keyword 
``failed`` and select the ``stdio`` link for the corresponding step to see
the reason for the failure.

The buildbot will do its job quietly if there are no errors, and the system will
undergo build and test cycles every day to verify that all is well.

Test.py
*******
The buildbots use a Python program, ``test.py``, that is responsible for 
running all of the tests and collecting the resulting reports into a human-
readable form.  This program is also available for use by users and developers
as well.

``test.py`` is very flexible in allowing the user to specify the number
and kind of tests to run; and also the amount and kind of output to generate.

Before running ``test.py``, make sure that ns3's examples and tests
have been built by doing the following

::

  ./waf configure --enable-examples --enable-tests
  ./waf

By default, ``test.py`` will run all available tests and report status
back in a very concise form.  Running the command

::

  ./test.py

will result in a number of ``PASS``, ``FAIL``, ``CRASH`` or ``SKIP``
indications followed by the kind of test that was run and its display name.
 
::

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  'build' finished successfully (0.939s)
  FAIL: TestSuite ns3-wifi-propagation-loss-models
  PASS: TestSuite object-name-service
  PASS: TestSuite pcap-file-object
  PASS: TestSuite ns3-tcp-cwnd
  ...
  PASS: TestSuite ns3-tcp-interoperability
  PASS: Example csma-broadcast
  PASS: Example csma-multicast

This mode is intended to be used by users who are interested in determining if
their distribution is working correctly, and by developers who are interested
in determining if changes they have made have caused any regressions.

There are a number of options available to control the behavior of ``test.py``.
if you run ``test.py --help`` you should see a command summary like:

::

  Usage: test.py [options]
  
  Options:
    -h, --help            show this help message and exit
    -b BUILDPATH, --buildpath=BUILDPATH
                          specify the path where ns-3 was built (defaults to the
                          build directory for the current variant)
    -c KIND, --constrain=KIND
                          constrain the test-runner by kind of test
    -e EXAMPLE, --example=EXAMPLE
                          specify a single example to run (no relative path is
                          needed)
    -g, --grind           run the test suites and examples using valgrind
    -k, --kinds           print the kinds of tests available
    -l, --list            print the list of known tests
    -m, --multiple        report multiple failures from test suites and test
                          cases
    -n, --nowaf           do not run waf before starting testing
    -p PYEXAMPLE, --pyexample=PYEXAMPLE
                          specify a single python example to run (with relative
                          path)
    -r, --retain          retain all temporary files (which are normally
                          deleted)
    -s TEST-SUITE, --suite=TEST-SUITE
                          specify a single test suite to run
    -t TEXT-FILE, --text=TEXT-FILE
                          write detailed test results into TEXT-FILE.txt
    -v, --verbose         print progress and informational messages
    -w HTML-FILE, --web=HTML-FILE, --html=HTML-FILE
                          write detailed test results into HTML-FILE.html
    -x XML-FILE, --xml=XML-FILE
                          write detailed test results into XML-FILE.xml

If one specifies an optional output style, one can generate detailed descriptions
of the tests and status.  Available styles are ``text`` and ``HTML``.
The buildbots will select the HTML option to generate HTML test reports for the
nightly builds using

::

  ./test.py --html=nightly.html

In this case, an HTML file named ''nightly.html'' would be created with a pretty
summary of the testing done.  A ''human readable'' format is available for users
interested in the details.

::

  ./test.py --text=results.txt

In the example above, the test suite checking the |ns3| wireless
device propagation loss models failed.  By default no further information is
provided.

To further explore the failure, ``test.py`` allows a single test suite
to be specified.  Running the command

::

  ./test.py --suite=ns3-wifi-propagation-loss-models

or equivalently

::

  ./test.py -s ns3-wifi-propagation-loss-models

results in that single test suite being run.

::

  FAIL: TestSuite ns3-wifi-propagation-loss-models

To find detailed information regarding the failure, one must specify the kind
of output desired.  For example, most people will probably be interested in
a text file::

  ./test.py --suite=ns3-wifi-propagation-loss-models --text=results.txt

This will result in that single test suite being run with the test status written to 
the file ''results.txt''.

You should find something similar to the following in that file::


  FAIL: Test Suite ''ns3-wifi-propagation-loss-models'' (real 0.02 user 0.01 system 0.00)
  PASS: Test Case "Check ... Friis ... model ..." (real 0.01 user 0.00 system 0.00)
  FAIL: Test Case "Check ... Log Distance ... model" (real 0.01 user 0.01 system 0.00)
    Details:
      Message:   Got unexpected SNR value
      Condition: [long description of what actually failed]
      Actual:    176.395
      Limit:     176.407 +- 0.0005
      File:      ../src/test/ns3wifi/propagation-loss-models-test-suite.cc
      Line:      360

Notice that the Test Suite is composed of two Test Cases.  The first test case
checked the Friis propagation loss model and passed.  The second test case 
failed checking the Log Distance propagation model.  In this case, an SNR of
176.395 was found, and the test expected a value of 176.407 correct to three
decimal places.  The file which implemented the failing test is listed as well
as the line of code which triggered the failure.

If you desire, you could just as easily have written an HTML file using the 
``--html`` option as described above.

Typically a user will run all tests at least once after downloading 
|ns3| to ensure that his or her environment has been built correctly
and is generating correct results according to the test suites.  Developers
will typically run the test suites before and after making a change to ensure
that they have not introduced a regression with their changes.  In this case,
developers may not want to run all tests, but only a subset.  For example,
the developer might only want to run the unit tests periodically while making
changes to a repository.  In this case, ``test.py`` can be told to constrain
the types of tests being run to a particular class of tests.  The following
command will result in only the unit tests being run::

  ./test.py --constrain=unit

Similarly, the following command will result in only the example smoke tests
being run::

  ./test.py --constrain=unit

To see a quick list of the legal kinds of constraints, you can ask for them
to be listed.  The following command

::

  ./test.py --kinds

will result in the following list being displayed::

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  'build' finished successfully (0.939s)Waf: Entering directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  bvt:         Build Verification Tests (to see if build completed successfully)
  core:        Run all TestSuite-based tests (exclude examples)
  example:     Examples (to see if example programs run successfully)
  performance: Performance Tests (check to see if the system is as fast as expected)
  system:      System Tests (spans modules to check integration of modules)
  unit:        Unit Tests (within modules to check basic functionality)

Any of these kinds of test can be provided as a constraint using the ``--constraint`` 
option.

To see a quick list of all of the test suites available, you can ask for them
to be listed.  The following command,

::

  ./test.py --list

will result in a list of the test suite being displayed, similar to::

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  'build' finished successfully (0.939s)
  histogram
  ns3-wifi-interference
  ns3-tcp-cwnd
  ns3-tcp-interoperability
  sample
  devices-mesh-flame
  devices-mesh-dot11s
  devices-mesh
  ...
  object-name-service
  callback
  attributes
  config
  global-value
  command-line
  basic-random-number
  object

Any of these listed suites can be selected to be run by itself using the 
``--suite`` option as shown above.

Similarly to test suites, one can run a single C++ example program
using the ``--example`` option.  Note that the relative path for the
example does not need to be included and that the executables built
for C++ examples do not have extensions.  Entering

::

  ./test.py --example=udp-echo

results in that single example being run.

::

  PASS: Example examples/udp/udp-echo

You can specify the directory where ns-3 was built using the
``--buildpath`` option as follows.

::

  ./test.py --buildpath=/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build/debug --example=wifi-simple-adhoc

One can run a single Python example program using the ``--pyexample``
option.  Note that the relative path for the example must be included
and that Python examples do need their extensions.  Entering

::

  ./test.py --pyexample=examples/tutorial/first.py

results in that single example being run.

::

  PASS: Example examples/tutorial/first.py

Because Python examples are not built, you do not need to specify the
directory where ns-3 was built to run them.

Normally when example programs are executed, they write a large amount of trace 
file data.  This is normally saved to the base directory of the distribution 
(e.g., /home/user/ns-3-dev).  When ``test.py`` runs an example, it really
is completely unconcerned with the trace files.  It just wants to to determine
if the example can be built and run without error.  Since this is the case, the
trace files are written into a ``/tmp/unchecked-traces`` directory.  If you 
run the above example, you should be able to find the associated 
``udp-echo.tr`` and ``udp-echo-n-1.pcap`` files there.

The list of available examples is defined by the contents of the ''examples''
directory in the distribution.  If you select an example for execution using
the ``--example`` option, ``test.py`` will not make any attempt to decide
if the example has been configured or not, it will just try to run it and
report the result of the attempt.

When ``test.py`` runs, by default it will first ensure that the system has
been completely built.  This can be defeated by selecting the ``--nowaf``
option.

::

  ./test.py --list --nowaf

will result in a list of the currently built test suites being displayed, similar to::

  ns3-wifi-propagation-loss-models
  ns3-tcp-cwnd
  ns3-tcp-interoperability
  pcap-file-object
  object-name-service
  random-number-generators

Note the absence of the ``Waf`` build messages.

``test.py`` also supports running the test suites and examples under valgrind.
Valgrind is a flexible program for debugging and profiling Linux executables.  By
default, valgrind runs a tool called memcheck, which performs a range of memory-
checking functions, including detecting accesses to uninitialised memory, misuse 
of allocated memory (double frees, access after free, etc.) and detecting memory 
leaks.  This can be selected by using the ``--grind`` option.

::

  ./test.py --grind

As it runs, ``test.py`` and the programs that it runs indirectly, generate large
numbers of temporary files.  Usually, the content of these files is not interesting,
however in some cases it can be useful (for debugging purposes) to view these files.
``test.py`` provides a ``--retain`` option which will cause these temporary
files to be kept after the run is completed.  The files are saved in a directory 
named ``testpy-output`` under a subdirectory named according to the current Coordinated
Universal Time (also known as Greenwich Mean Time).

::

  ./test.py --retain

Finally, ``test.py`` provides a ``--verbose`` option which will print
large amounts of information about its progress.  It is not expected that this
will be terribly useful unless there is an error.  In this case, you can get
access to the standard output and standard error reported by running test suites
and examples.  Select verbose in the following way::

  ./test.py --verbose

All of these options can be mixed and matched.  For example, to run all of the 
ns-3 core test suites under valgrind, in verbose mode, while generating an HTML
output file, one would do::

  ./test.py --verbose --grind --constrain=core --html=results.html 

TestTaxonomy
************

As mentioned above, tests are grouped into a number of broadly defined 
classifications to allow users to selectively run tests to address the different
kinds of testing that need to be done.

* Build Verification Tests
* Unit Tests 
* System Tests
* Examples
* Performance Tests

BuildVerificationTests
++++++++++++++++++++++

These are relatively simple tests that are built along with the distribution
and are used to make sure that the build is pretty much working.  Our
current unit tests live in the source files of the code they test and are
built into the ns-3 modules; and so fit the description of BVTs.  BVTs live
in the same source code that is built into the ns-3 code.  Our current tests
are examples of this kind of test.

Unit Tests
++++++++++

Unit tests are more involved tests that go into detail to make sure that a
piece of code works as advertised in isolation.  There is really no reason
for this kind of test to be built into an ns-3 module.  It turns out, for
example, that the unit tests for the object name service are about the same
size as the object name service code itself.  Unit tests are tests that
check a single bit of functionality that are not built into the ns-3 code,
but live in the same directory as the code it tests.  It is possible that
these tests check integration of multiple implementation files in a module
as well.  The file src/core/test/names-test-suite.cc is an example of this kind
of test.  The file src/network/test/pcap-file-test-suite.cc is another example
that uses a known good pcap file as a test vector file.  This file is stored
locally in the src/network directory.

System Tests
++++++++++++

System tests are those that involve more than one module in the system.  We
have lots of this kind of test running in our current regression framework,
but they are typically overloaded examples.  We provide a new place 
for this kind of test in the directory ``src/test``.  The file
src/test/ns3tcp/ns3-interop-test-suite.cc is an example of this kind of
test.  It uses NSC TCP to test the ns-3 TCP implementation.  Often there
will be test vectors required for this kind of test, and they are stored in
the directory where the test lives.  For example,
ns3tcp-interop-response-vectors.pcap is a file consisting of a number of TCP
headers that are used as the expected responses of the ns-3 TCP under test
to a stimulus generated by the NSC TCP which is used as a ''known good''
implementation.

Examples
++++++++

The examples are tested by the framework to make sure they built and will
run.  Nothing is checked, and currently the pcap files are just written off
into /tmp to be discarded.  If the examples run (don't crash) they pass this
smoke test.

Performance Tests
+++++++++++++++++

Performance tests are those which exercise a particular part of the system
and determine if the tests have executed to completion in a reasonable time.

Running Tests
*************

Tests are typically run using the high level ``test.py`` program. To get a list of the available command-line options, run ``test.py --help``

The test program ``test.py`` will run both tests and those examples that
have been added to the list to check.  The difference between tests
and examples is as follows.  Tests generally check that specific simulation
output or events conforms to expected behavior.  In contrast, the output
of examples is not checked, and the test program merely checks the exit
status of the example program to make sure that it runs without error.

Briefly, to run all tests, first one must configure tests during configuration
stage, and also (optionally) examples if examples are to be checked:

::

   ./waf --configure --enable-examples --enable-tests

Then, build ns-3, and after it is built, just run ``test.py``.  ``test.py -h``
will show a number of configuration options that modify the behavior
of test.py.

The program ``test.py`` invokes, for C++ tests and examples, a lower-level
C++ program called ``test-runner`` to actually run the tests.  As discussed
below, this ``test-runner`` can be a helpful way to debug tests.

Debugging Tests
***************

The debugging of the test programs is best performed running the low-level test-runner program. The test-runner is the bridge from generic Python code to |ns3| code. It is written in C++ and uses the automatic test discovery process in the 
|ns3| code to find and allow execution of all of the various tests.

The main reason why ``test.py`` is not suitable for debugging is that it is not allowed for logging to be turned on using the ``NS_LOG`` environmental variable when test.py runs.  This limitation does not apply to the test-runner executable. Hence, if you want to see logging output from your tests, you have to run them using the test-runner directly.

In order to execute the test-runner, you run it like any other ns-3 executable
-- using ``waf``.  To get a list of available options, you can type::

  ./waf --run "test-runner --help"

You should see something like the following::

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone-test/ns-3-dev/build'
  'build' finished successfully (0.353s)
  --assert:               Tell tests to segfault (like assert) if an error is detected
  --basedir=dir:          Set the base directory (where to find src) to ''dir''
  --tempdir=dir:          Set the temporary directory (where to find data files) to ''dir''
  --constrain=test-type:  Constrain checks to test suites of type ''test-type''
  --help:                 Print this message
  --kinds:                List all of the available kinds of tests
  --list:                 List all of the test suites (optionally constrained by test-type)
  --out=file-name:        Set the test status output file to ''file-name''
  --suite=suite-name:     Run the test suite named ''suite-name''
  --verbose:              Turn on messages in the run test suites

There are a number of things available to you which will be familiar to you if
you have looked at ``test.py``.  This should be expected since the test-
runner is just an interface between ``test.py`` and |ns3|.  You 
may notice that example-related commands are missing here.  That is because 
the examples are really not |ns3| tests.  ``test.py`` runs them
as if they were to present a unified testing environment, but they are really
completely different and not to be found here.

The first new option that appears here, but not in test.py is the ``--assert``
option.  This option is useful when debugging a test case when running under a 
debugger like ``gdb``.  When selected, this option tells the underlying
test case to cause a segmentation violation if an error is detected.  This has
the nice side-effect of causing program execution to stop (break into the 
debugger) when an error is detected.  If you are using gdb, you could use this
option something like,

::

  ./waf shell
  cd build/debug/utils
  gdb test-runner
  run --suite=global-value --assert

If an error is then found in the global-value test suite, a segfault would be 
generated and the (source level) debugger would stop at the ``NS_TEST_ASSERT_MSG``
that detected the error.

Another new option that appears here is the ``--basedir`` option.  It turns out
that some tests may need to reference the source directory of the |ns3|  
distribution to find local data, so a base directory is always required to run
a test.

If you run a test from test.py, the Python program will provide the basedir 
option for you.  To run one of the tests directly from the test-runner 
using ``waf``, you will need to specify the test suite to run along with 
the base directory.  So you could use the shell and do::

  ./waf --run "test-runner --basedir=`pwd` --suite=pcap-file-object"

Note the ''backward'' quotation marks on the ``pwd`` command.  

If you are running the test suite out of a debugger, it can be quite painful 
to remember and constantly type the absolute path of the distribution base 
directory.
Because of this, if you omit the basedir, the test-runner will try to figure one
out for you.  It begins in the current working directory and walks up the 
directory tree looking for a directory file with files named ``VERSION`` and
``LICENSE.``  If it finds one, it assumes that must be the basedir and provides
it for you.

Test output
+++++++++++

Many test suites need to write temporary files (such as pcap files)
in the process of running the tests.  The tests then need a temporary directory
to write to.  The Python test utility (test.py) will provide a temporary file
automatically, but if run stand-alone this temporary directory must be provided.
Just as in the basedir case, it can be annoying to continually have to provide
a ``--tempdir``, so the test runner will figure one out for you if you don't
provide one.  It first looks for environment variables named ``TMP`` and 
``TEMP`` and uses those.  If neither ``TMP`` nor ``TEMP`` are defined
it picks ``/tmp``.  The code then tacks on an identifier indicating what 
created the directory (ns-3) then the time (hh.mm.ss) followed by a large random
number.  The test runner creates a directory of that name to be used as the
temporary directory.  Temporary files then go into a directory that will be
named something like

::

  /tmp/ns-3.10.25.37.61537845

The time is provided as a hint so that you can relatively easily reconstruct
what directory was used if you need to go back and look at the files that were
placed in that directory.

Another class of output is test output like pcap traces that are generated
to compare to reference output.  The test program will typically delete
these after the test suites all run.  To disable the deletion of test
output, run ``test.py`` with the "retain" option:

::

   ./test.py -r

and test output can be found in the ``testpy-output/`` directory.

Reporting of test failures
++++++++++++++++++++++++++

When you run a test suite using the test-runner it will run the test quietly
by default.  The only indication that you will get that the test passed is 
the *absence* of a message from ``waf`` saying that the program 
returned something other than a zero exit code.  To get some output from the
test, you need to specify an output file to which the tests will write their 
XML status using the ``--out`` option.  You need to be careful interpreting
the results because the test suites will *append* results onto this file.
Try,

::

  ./waf --run "test-runner --basedir=`pwd` --suite=pcap-file-object --out=myfile.xml"

If you look at the file ``myfile.xml`` you should see something like,

::

  <TestSuite>
    <SuiteName>pcap-file-object</SuiteName>
    <TestCase>
      <CaseName>Check to see that PcapFile::Open with mode ''w'' works</CaseName>
      <CaseResult>PASS</CaseResult>
      <CaseTime>real 0.00 user 0.00 system 0.00</CaseTime>
    </TestCase>
    <TestCase>
      <CaseName>Check to see that PcapFile::Open with mode ''r'' works</CaseName>
      <CaseResult>PASS</CaseResult>
      <CaseTime>real 0.00 user 0.00 system 0.00</CaseTime>
    </TestCase>
    <TestCase>
      <CaseName>Check to see that PcapFile::Open with mode ''a'' works</CaseName>
      <CaseResult>PASS</CaseResult>
      <CaseTime>real 0.00 user 0.00 system 0.00</CaseTime>
    </TestCase>
    <TestCase>
      <CaseName>Check to see that PcapFileHeader is managed correctly</CaseName>
      <CaseResult>PASS</CaseResult>
      <CaseTime>real 0.00 user 0.00 system 0.00</CaseTime>
    </TestCase>
    <TestCase>
      <CaseName>Check to see that PcapRecordHeader is managed correctly</CaseName>
      <CaseResult>PASS</CaseResult>
      <CaseTime>real 0.00 user 0.00 system 0.00</CaseTime>
    </TestCase>
    <TestCase>
      <CaseName>Check to see that PcapFile can read out a known good pcap file</CaseName>
      <CaseResult>PASS</CaseResult>
      <CaseTime>real 0.00 user 0.00 system 0.00</CaseTime>
    </TestCase>
    <SuiteResult>PASS</SuiteResult>
    <SuiteTime>real 0.00 user 0.00 system 0.00</SuiteTime>
  </TestSuite>

If you are familiar with XML this should be fairly self-explanatory.  It is 
also not a complete XML file since test suites are designed to have their
output appended to a master XML status file as described in the ``test.py``
section.

Debugging test suite failures
+++++++++++++++++++++++++++++

To debug test crashes, such as::

  CRASH: TestSuite ns3-wifi-interference

You can access the underlying test-runner program via gdb as follows, and
then pass the "--basedir=`pwd`" argument to run (you can also pass other
arguments as needed, but basedir is the minimum needed)::

  ./waf --command-template="gdb %s" --run "test-runner"
  Waf: Entering directory `/home/tomh/hg/sep09/ns-3-allinone/ns-3-dev-678/build'
  Waf: Leaving directory `/home/tomh/hg/sep09/ns-3-allinone/ns-3-dev-678/build'
  'build' finished successfully (0.380s)
  GNU gdb 6.8-debian
  Copyright (C) 2008 Free Software Foundation, Inc.
  L cense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
  This is free software: you are free to change and redistribute it.
  There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
  and "show warranty" for details.
  This GDB was configured as "x86_64-linux-gnu"...
  (gdb) r --basedir=`pwd`
  Starting program: <..>/build/debug/utils/test-runner --basedir=`pwd`
  [Thread debugging using libthread_db enabled]
  assert failed. file=../src/core/model/type-id.cc, line=138, cond="uid <= m_information.size () && uid != 0"
  ...

Here is another example of how to use valgrind to debug a memory problem
such as::

  VALGR: TestSuite devices-mesh-dot11s-regression

  ./waf --command-template="valgrind %s --basedir=`pwd` --suite=devices-mesh-dot11s-regression" --run test-runner

Class TestRunner
****************

The executables that run dedicated test programs use a TestRunner class.  This
class provides for automatic test registration and listing, as well as a way to
execute the individual tests.  Individual test suites use C++ global 
constructors
to add themselves to a collection of test suites managed by the test runner.
The test runner is used to list all of the available tests and to select a test
to be run.  This is a quite simple class that provides three static methods to
provide or Adding and Getting test suites to a collection of tests.  See the 
doxygen for class ``ns3::TestRunner`` for details.

Test Suite
**********

All |ns3| tests are classified into Test Suites and Test Cases.  A 
test suite is a collection of test cases that completely exercise a given kind
of functionality.  As described above, test suites can be classified as,

* Build Verification Tests
* Unit Tests 
* System Tests
* Examples
* Performance Tests

This classification is exported from the TestSuite class.  This class is quite
simple, existing only as a place to export this type and to accumulate test
cases.  From a user perspective, in order to create a new TestSuite in the 
system one only has to define a new class that inherits from class ``TestSuite``
and perform these two duties.

The following code will define a new class that can be run by ``test.py``
as a ''unit'' test with the display name, ``my-test-suite-name``.

::

  class MySuite : public TestSuite
  {
  public:
    MyTestSuite ();
  };
  
  MyTestSuite::MyTestSuite ()
    : TestSuite ("my-test-suite-name", UNIT)
  {
    AddTestCase (new MyTestCase);
  }
  
  MyTestSuite myTestSuite;

The base class takes care of all of the registration and reporting required to
be a good citizen in the test framework.

Test Case
*********

Individual tests are created using a TestCase class.  Common models for the use
of a test case include "one test case per feature", and "one test case per method."
Mixtures of these models may be used.

In order to create a new test case in the system, all one has to do is to inherit
from the  ``TestCase`` base class, override the constructor to give the test 
case a name and override the ``DoRun`` method to run the test.

::

  class MyTestCase : public TestCase
  {
    MyTestCase ();
    virtual void DoRun (void);
  };
  
  MyTestCase::MyTestCase ()
    : TestCase ("Check some bit of functionality")
  {
  }
  
  void
  MyTestCase::DoRun (void)
  {
    NS_TEST_ASSERT_MSG_EQ (true, true, "Some failure message");
  }

Utilities
*********

There are a number of utilities of various kinds that are also part of the 
testing framework.  Examples include a generalized pcap file useful for
storing test vectors; a generic container useful for transient storage of
test vectors during test execution; and tools for generating presentations
based on validation and verification testing results.

These utilities are not documented here, but for example, please see
how the TCP tests found in ``src/test/ns3tcp/`` use pcap files and reference
output.
