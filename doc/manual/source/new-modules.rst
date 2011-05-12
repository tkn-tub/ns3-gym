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

Step 2 - Copy the template module
*********************************

The template module ::

  src/template

is a skeleton module that shows how modules should be created.

For the purposes of this discussion we will assume that your new module is called "new-module".  From the top level |ns3| directory, do the following to copy the template module to a new directory with the same name as your new module: ::

  cp -r src/template src/new-module

Now you will need to open the following file in your favorite text editor: ::

  src/new-module/wscript

and replace all of the occurrences of "template" in this wscript file with the name of your new module, i.e. "new-module" for our assumed module name.

If your module will have model source files, then create the following directory where they will go: :: 

  mkdir src/new-module/model

Copy all of your module's model source files to the above directory.

If your module will have helper source files, then create the following directory where they will go: :: 

  mkdir src/new-module/helper

Copy all of your module's helper source files to the above directory.

Step 3 - Specify your module's source files
*******************************************

If your new module has model and/or helper source files, then they
must be specified in your  ::

  src/new-module/wscript

file by modifying it with your text editor.

As an example, the source files for the spectrum module are specified
in ::

  src/spectrum/wscript

with the following function call and list
of source files.  Note that the second argument for the function
create_ns3_module() is the list of modules that the module being created
depends on: ::

    module = bld.create_ns3_module('spectrum', ['internet', 'propagation', 'applications'])

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
files: ::

    headers = bld.new_task_gen('ns3header')

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
function call and source file name.  Note that the second argument for
the function create_ns3_program() is the list of modules that the
program being created depends on: ::

    obj = bld.create_ns3_program('sample-simulator', ['core'])

    obj.source = 'sample-simulator.cc'

The core module's Python examples are specified using the following
function call.  Note that the second argument for the function
register_ns3_script() is the list of modules that the Python example
depends on: ::

    bld.register_ns3_script('sample-simulator.py', ['core'])

Step 7 - Specify which of your module's examples should be run
**************************************************************

If your new module has examples, then you must specify which of them
should be run in your ::

  src/new-module/tests/examples-to-run.py

file by modifying it with your text editor.  These examples are run by
test.py.

As an example, the examples that are run by test.py for the core module are specified in  ::

  src/core/tests/examples-to-run

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

Step 8 - Build and test your new module
***************************************

You can now build and test your module as normal: ::

  ./waf configure --enable-examples --enable-tests
  ./waf build
  ./test.py


