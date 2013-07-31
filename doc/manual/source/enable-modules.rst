.. include:: replace.txt
.. highlight:: bash

Enabling Subsets of |ns3| Modules
---------------------------------

As with most software projects, |ns3| is ever growing larger in terms of number of modules, lines of code, and memory footprint.  Users, however, may only use a few of those modules at a time.  For this reason, users may want to explicitly enable only the subset of the possible |ns3| modules that they actually need for their research.  

This chapter discusses how to enable only the |ns3| modules that you are intersted in using.

How to enable a subset of |ns3|'s modules
*****************************************

If shared libraries are being built, then enabling a module will cause at least one library to be built:

.. sourcecode:: text

  libns3-modulename.so

If the module has a test library and test libraries are being built, then

.. sourcecode:: text

  libns3-modulename-test.so

will be built, too.  Other modules that the module depends on and their test libraries will also be built.  

By default, all modules are built in |ns3|.  There are two ways to enable a subset of these modules:

#. Using waf's --enable-modules option
#. Using the |ns3| configuration file

Enable modules using waf's --enable-modules option
++++++++++++++++++++++++++++++++++++++++++++++++++

To enable only the core module with example and tests, for example,
try these commands: ::

  $ ./waf clean
  $ ./waf configure --enable-examples --enable-tests --enable-modules=core
  $ ./waf build
  $ cd build/debug/
  $ ls

and the following libraries should be present:

.. sourcecode:: text

  bindings  libns3-core.so       ns3      scratch  utils
  examples  libns3-core-test.so  samples  src

Note the ``./waf clean`` step is done here only to make it more obvious which module libraries were built.  You don't have to do ``./waf clean`` in order to enable subsets of modules.

Running test.py will cause only those tests that depend on module core to be run:
 
.. sourcecode:: text

  24 of 24 tests passed (24 passed, 0 skipped, 0 failed, 0 crashed, 0 valgrind errors)

Repeat the above steps for the "network" module instead of the "core" module, and the following will be built, since network depends on core:

.. sourcecode:: text

  bindings  libns3-core.so       libns3-network.so       ns3      scratch  utils
  examples  libns3-core-test.so  libns3-network-test.so  samples  src

Running test.py will cause those tests that depend on only the core and network modules to be run:

.. sourcecode:: text

  31 of 31 tests passed (31 passed, 0 skipped, 0 failed, 0 crashed, 0 valgrind errors)

Enable modules using the |ns3| configuration file
+++++++++++++++++++++++++++++++++++++++++++++++++

A configuration file, .ns3rc, has been added to |ns3| that allows users to specify which modules are to be included in the build.

When enabling a subset of |ns3| modules, the precedence rules are as follows:

#. the --enable-modules configure string overrides any .ns3rc file
#. the .ns3rc file in the top level |ns3| directory is next consulted, if present
#. the system searches for ~/.ns3rc if the above two are unspecified

If none of the above limits the modules to be built, all modules that waf knows about will be built.

The maintained version of the .ns3rc file in the |ns3| source code repository resides in the ``utils`` directory.  The reason for this is if it were in the top-level directory of the repository, it would be prone to accidental checkins from maintainers that enable the modules they want to use.  Therefore, users need to manually copy the .ns3rc from the ``utils`` directory to their preferred place (top level directory or their home directory) to enable persistent modular build configuration.

Assuming that you are in the top level |ns3| directory, you can get a copy of the .ns3rc file that is in the ``utils`` directory as follows: ::

    $ cp utils/.ns3rc .

The .ns3rc file should now be in your top level |ns3| directory, and it contains the following:

.. sourcecode:: python

  #! /usr/bin/env python
  
  # A list of the modules that will be enabled when ns-3 is run.
  # Modules that depend on the listed modules will be enabled also.
  #
  # All modules can be enabled by choosing 'all_modules'.
  modules_enabled = ['all_modules']
  
  # Set this equal to true if you want examples to be run.
  examples_enabled = False
  
  # Set this equal to true if you want tests to be run.
  tests_enabled = False

Use your favorite editor to modify the .ns3rc file to only enable the core module with examples and tests like this:

.. sourcecode:: python

  #! /usr/bin/env python
  
  # A list of the modules that will be enabled when ns-3 is run.
  # Modules that depend on the listed modules will be enabled also.
  #
  # All modules can be enabled by choosing 'all_modules'.
  modules_enabled = ['core']
  
  # Set this equal to true if you want examples to be run.
  examples_enabled = True
  
  # Set this equal to true if you want tests to be run.
  tests_enabled = True

Only the core module will be enabled now if you try these commands: ::

  $ ./waf clean
  $ ./waf configure
  $ ./waf build
  $ cd build/debug/
  $ ls

and the following libraries should be present:

.. sourcecode:: text

  bindings  libns3-core.so       ns3      scratch  utils
  examples  libns3-core-test.so  samples  src

Note the ``./waf clean`` step is done here only to make it more obvious which module libraries were built.  You don't have to do ``./waf clean`` in order to enable subsets of modules.

Running test.py will cause only those tests that depend on module core to be run:
 
.. sourcecode:: text

  24 of 24 tests passed (24 passed, 0 skipped, 0 failed, 0 crashed, 0 valgrind errors)

Repeat the above steps for the "network" module instead of the "core" module, and the following will be built, since network depends on core:

.. sourcecode:: text

  bindings  libns3-core.so       libns3-network.so       ns3      scratch  utils
  examples  libns3-core-test.so  libns3-network-test.so  samples  src

Running test.py will cause those tests that depend on only the core and network modules to be run:

.. sourcecode:: text

  31 of 31 tests passed (31 passed, 0 skipped, 0 failed, 0 crashed, 0 valgrind errors)
