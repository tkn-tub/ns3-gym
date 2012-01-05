.. include:: replace.txt

Adding a New Module to |ns3|
----------------------------

When you have created a group of related classes, examples, and tests,
they can be combined together into an |ns3| module so that they can be
used with existing |ns3| modules and by other researchers.

This chapter walks you through the steps necessary to add a new module
to |ns3|.

Step 1 - Familiarize yourself with the module layout
****************************************************

All modules can be found in the ``src`` directory.  Each module can be
found in a directory that has the same name as the module.  For
example, the spectrum module can be found here: ::

  src/spectrum

A prototypical module has the following directory structure and
required files: ::

  src/
        module-name/
                bindings/
                doc/
                examples/
                        wscript
                helper/
                model/
                test/
                        examples-to-run.py
                wscript

Not all directories will be present in each module.

Step 2 - Create your new module based on the template module
************************************************************

A python program is provided in the source directory that will create a skeleton for a new module ::

  src/create-module.py

For the purposes of this discussion we will assume that your new module is called "new-module".  From the ``src`` directory, do the following to create the new module: ::

  ./create-module.py new-module

Next, cd into ``new-module``; you will find this directory layout: ::

  examples  helper  model  test  wscript

We next walk through how to customize this module.  All |ns3| modules
depend on the 'core' module and usually on other modules.  This 
dependency is specified in the wscript file.  
Let's assume that 'new-module' depends on the internet,
mobility, and aodv modules.  Then the call to the function that will
create this module should look like this before editing: ::

  def build(bld):
      module = bld.create_ns3_module('new-module', ['core'])

and after editing: ::

  def build(bld):
      module = bld.create_ns3_module('new-module', ['internet', 'mobility', 'aodv'])

Your module will most likely have model source files.  Initial skeletons (which will compile successfully) are created in ``model/new-module.cc`` and ``model/new-module.h``.  

If your module will have helper source files, then they will go into the helper/ directory; again, initial skeletons are created in that directory.

Finally, it is good practice to write tests.  A skeleton test suite and test case is created in the test/ directory.  The below constructor specifies that it will be a unit test named 'new-module':  ::

  New-moduleTestSuite::New-moduleTestSuite ()
    : TestSuite ("new-module", UNIT)
  {
    AddTestCase (new New-moduleTestCase1);
  }

Step 3 - Adding to your module's source files
*********************************************

If your new module has model and/or helper source files, then they
must be specified in your  ::

  src/new-module/wscript

file by modifying it with your text editor.

As an example, the source files for the spectrum module are specified
in ::

  src/spectrum/wscript

with the following list of source files: ::

    module.source = [
        'model/spectrum-model.cc',
        'model/spectrum-value.cc',
               .
	       .
	       .
        'model/microwave-oven-spectrum-value-helper.cc',
        'helper/spectrum-helper.cc',
        'helper/adhoc-aloha-noack-ideal-phy-helper.cc',
        'helper/waveform-generator-helper.cc',
        'helper/spectrum-analyzer-helper.cc',
        ]

Step 4 - Specify your module's header files
*******************************************

If your new module has model and/or helper header files, then they
must be specified in your  ::

  src/new-module/wscript

file by modifying it with your text editor.

As an example, the header files for the spectrum module are specified
in  ::

  src/spectrum/wscript

with the following function call, module name, and list of header
files.  Note that the argument for the function new_task_gen() tells
waf to install this module's headers with the other |ns3| headers: ::

    headers = bld.new_task_gen(features=['ns3header'])

    headers.module = 'spectrum'

    headers.source = [
        'model/spectrum-model.h',
        'model/spectrum-value.h',
               .
	       .
	       .
        'model/microwave-oven-spectrum-value-helper.h',
        'helper/spectrum-helper.h',
        'helper/adhoc-aloha-noack-ideal-phy-helper.h',
        'helper/waveform-generator-helper.h',
        'helper/spectrum-analyzer-helper.h',
        ]

Step 5 - Specify your module's tests
************************************

If your new module has tests, then they must be specified in your ::

  src/new-module/wscript

file by modifying it with your text editor.

As an example, the tests for the spectrum module are specified in ::

  src/spectrum/wscript

with the following function call and list of test suites: ::

    module_test = bld.create_ns3_module_test_library('spectrum')

    module_test.source = [
        'test/spectrum-interference-test.cc',
        'test/spectrum-value-test.cc',
        ]


Step 6 - Specify your module's examples
***************************************

If your new module has examples, then they must be specified in your ::

  src/new-module/examples/wscript

file by modifying it with your text editor.

As an example, the examples for the core module are specified in ::

  src/core/examples/wscript

The core module's C++ examples are specified using the following
function calls and source file names.  Note that the second argument
for the function ``create_ns3_program()`` is the list of modules that the
program being created depends on: ::

    obj = bld.create_ns3_program('main-callback', ['core'])
    obj.source = 'main-callback.cc'

    obj = bld.create_ns3_program('sample-simulator', ['core'])
    obj.source = 'sample-simulator.cc'

The core module's Python examples are specified using the following
function call.  Note that the second argument for the function
register_ns3_script() is the list of modules that the Python example
depends on: ::

    bld.register_ns3_script('sample-simulator.py', ['core'])

Step 7 - Specify which of your module's examples should be run as tests
***********************************************************************

The test framework can also be instrumented to run example programs to
try to catch regressions in the examples.  However, not all examples
are suitable for regression tests.  A file called ``examples-to-run.py``
that exists in each module's test directory can control the invocation
of the examples when the test framework runs.

As an example, the examples that are run by ``test.py`` for the core module are specified in  ::

  src/core/test/examples-to-run.py

using the following two lists of C++ and Python examples: ::

  # A list of C++ examples to run in order to ensure that they remain
  # buildable and runnable over time.  Each tuple in the list contains
  #
  #     (example_name, do_run, do_valgrind_run).
  #
  # See test.py for more information.
  cpp_examples = [
      ("main-attribute-value", "True", "True"),
      ("main-callback", "True", "True"),
      ("sample-simulator", "True", "True"),
      ("main-ptr", "True", "True"),
      ("main-random-variable", "True", "True"),
      ("sample-random-variable", "True", "True"),
  ]
  
  # A list of Python examples to run in order to ensure that they remain
  # runnable over time.  Each tuple in the list contains
  #
  #     (example_name, do_run).
  #
  # See test.py for more information.
  python_examples = [
      ("sample-simulator.py", "True"),
  ]

Each tuple in the C++ list of examples to run contains ::

    (example_name, do_run, do_valgrind_run)

where example_name is the executable to be run, do_run is a
condition under which to run the example, and do_valgrind_run is
a condition under which to run the example under valgrind.  This
is needed because NSC causes illegal instruction crashes with
some tests when they are run under valgrind.

Note that the two conditions are Python statements that
can depend on waf configuration variables.  For example, ::

    ("tcp-nsc-lfn", "NSC_ENABLED == True", "NSC_ENABLED == False"),

Each tuple in the Python list of examples to run contains ::

    (example_name, do_run)

where example_name is the Python script to be run and
do_run is a condition under which to run the example.

Note that the condition is a Python statement that can
depend on waf configuration variables.  For example, ::

    ("realtime-udp-echo.py", "ENABLE_REAL_TIME == False"),

If your new module has examples, then you must specify which of them
should be run in your ::

  src/new-module/test/examples-to-run.py

file by modifying it with your text editor.  These examples are run by
test.py.

Step 8 - Build and test your new module
***************************************

You can now build and test your module as normal: ::

  ./waf configure --enable-examples --enable-tests
  ./waf build
  ./test.py

and look for your new module's test suite (and example programs, if enabled) in the test output.
