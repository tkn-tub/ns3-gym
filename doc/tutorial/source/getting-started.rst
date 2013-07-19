.. include:: replace.txt
.. highlight:: bash

Getting Started
---------------

This section is aimed at getting a user to a working state starting
with a machine that may never have had |ns3| installed.  It covers
supported platforms, prerequisites, ways to obtain |ns3|, ways to
build |ns3|, and ways to verify your build and run simple programs.

Overview
********

|ns3| is built as a system of software libraries that work together.
User programs can be written that links with (or imports from) these
libraries.  User programs are written in either the C++ or Python 
programming languages.

|ns3| is distributed as source code, meaning that the target system
needs to have a software development environment to build the libraries
first, then build the user program.  |ns3| could in principle be 
distributed as pre-built libraries for selected systems, and in the
future it may be distributed that way, but at present, many users
actually do their work by editing |ns3| itself, so having the source
code around to rebuild the libraries is useful.  If someone would like 
to undertake the job of making pre-built libraries and packages for 
operating systems, please contact the ns-developers mailing list.

In the following, we'll look at two ways of downloading and building
|ns3|.  The first is to download and build an official release
from the main web site.  The second is to fetch and build development
copies of |ns3|.  We'll walk through both examples since the tools
involved are slightly different.

Downloading ns-3
****************

The |ns3| system as a whole is a fairly complex system and has a
number of dependencies on other components.  Along with the systems you will
most likely deal with every day (the GNU toolchain, Mercurial, a text
editor) you will need to ensure that a number of additional libraries are
present on your system before proceeding.  |ns3| provides a wiki
page that includes pages with many useful hints and tips.
One such page is the "Installation" page,
http://www.nsnam.org/wiki/index.php/Installation.

The "Prerequisites" section of this wiki page explains which packages are 
required to support common |ns3| options, and also provides the 
commands used to install them for common Linux variants.  Cygwin users will
have to use the Cygwin installer (if you are a Cygwin user, you used it to
install Cygwin). 

You may want to take this opportunity to explore the |ns3| wiki 
a bit since there really is a wealth of information there. 

From this point forward, we are going to assume that the reader is working in
Linux or a Linux emulation environment (Linux, Cygwin, etc.) and has the GNU
toolchain installed and verified along with the prerequisites mentioned 
above.  We are also going to assume that you have Mercurial and Waf installed
and running on the target system.

The |ns3| code is available in Mercurial repositories on the server
http://code.nsnam.org.  You can also download a tarball release at
http://www.nsnam.org/releases/, or you can work with repositories
using Mercurial.  We recommend using Mercurial unless there's a good reason
not to.  See the end of this section for instructions on how to get a tarball
release.

The simplest way to get started using Mercurial repositories is to use the
``ns-3-allinone`` environment.  This is a set of scripts that manages the 
downloading and building of various subsystems of |ns3| for you.  We 
recommend that you begin your |ns3| work in this environment.

Downloading ns-3 Using a Tarball
++++++++++++++++++++++++++++++++

A tarball is a particular format of software archive where multiple
files are bundled together and the archive possibly compressed.
|ns3| software releases are provided via a downloadable tarball.
The process for downloading |ns3| via tarball is simple; you just
have to pick a release, download it and decompress it.

Let's assume that you, as a user, wish to build |ns3| in a local
directory called ``workspace``. 
If you adopt the ``workspace`` directory approach, you can 
get a copy of a release by typing the following into your Linux shell 
(substitute the appropriate version numbers, of course):

::

  $ cd
  $ mkdir workspace
  $ cd workspace
  $ wget http://www.nsnam.org/releases/ns-allinone-3.17.tar.bz2
  $ tar xjf ns-allinone-3.17.tar.bz2

If you change into the directory ``ns-allinone-3.17`` you should see a
number of files:

::

  bake      constants.py   ns-3.17               README
  build.py  netanim-3.103  pybindgen-0.16.0.825  util.py

You are now ready to build the |ns3| distribution.

Downloading ns-3 Using Bake
++++++++++++++++++++++++++++

Bake is a tool for distributed integration and building, 
developed for the |ns3| project.  First of all, Bake is 
developed in Python, and should be fetched from the project's 
master code repositories using a tool called Mercurial, so to 
run Bake one must have Python and mercurial on one's machine.

One practice is to create a directory called ``workspace`` in one's home 
directory under which one can keep local Mercurial repositories.  
Any directory name will do, but we'll assume that ``workspace`` is used
herein (note:  ``repos`` may also be used in some documentation as
an example directory name).  You can get a copy of ``bake`` by typing the 
following into your Linux shell (assuming you have installed Mercurial):

::

  $ cd
  $ mkdir workspace
  $ cd workspace
  $ hg clone http://code.nsnam.org/bake

As the hg (Mercurial) command executes, you should see something like the 
following displayed,

::

  destination directory: bake
  requesting all changes
  adding changesets
  adding manifests
  adding file changes
  added 252 changesets with 661 changes to 62 files
  updating to branch default
  45 files updated, 0 files merged, 0 files removed, 0 files unresolved

After the clone command completes, you should have a directory called 
``bake``, the contents of which should look something like the following:

::

  bake                  bakeconf.xml  doc       generate-binary.py  TODO
  bake.py               examples      test

Notice that you really just downloaded some Python scripts and a Python
module called ``bake``.  The next step
will be to use those scripts to download and build the |ns3|
distribution of your choice.

There are a few configuration targets available:

1.  ``ns-3.17``:  the module corresponding to the release; it will download
    components similar to the release tarball.
2.  ``ns-3-dev``:  a similar module but using the development code tree
3.  ``ns-allinone-3.17``:  the module that includes other optional features
    such as click routing, openflow for |ns3|, and the Network Simulation
    Cradle
4.  ``ns-3-allinone``:  similar to the released version of the allinone
    module, but for development code.

The current development snapshot (unreleased) of |ns3| may be found 
at http://code.nsnam.org/ns-3-dev/.  The 
developers attempt to keep these repository in consistent, working states but
they are in a development area with unreleased code present, so you may want 
to consider staying with an official release if you do not need newly-
introduced features.

You can find the latest version  of the
code either by inspection of the repository list or by going to the 
`"ns-3 Releases"
<http://www.nsnam.org/releases>`_
web page and clicking on the latest release link.  We'll proceed in
this tutorial example with ``ns-3.17``.

We are now going to use the bake tool to pull down the various pieces of 
|ns3| you will be using.  First, we'll say a word about running bake.

bake works by downloading source packages into a source directory,
and installing libraries into a build directory.  bake can be run
by referencing the binary, but if one chooses to run bake from
outside of the directory it was downloaded into, it is advisable
to put bake into your path, such as follows (Linux bash shell example):

::

  $ export BAKE_HOME=`pwd`/bake
  $ export PATH=$PATH:$BAKE_HOME
  $ export PYTHONPATH=$PYTHONPATH:$BAKE_HOME

However, setting environment variables is not strictly necessary to
complete this tutorial, so we'll call bake directly by specifying the path 
to it in our shell commands.

Step into the workspace directory and type the following into your shell:

::

  $ ./bake.py configure -e ns-3.17

Next, we'l ask bake to check whether we have enough tools to download
various components.  Type:

::

  $ ./bake.py check

You should see something like the following,

::

   > Python - OK
   > GNU C++ compiler - OK
   > Mercurial - OK
   > CVS - OK
   > GIT - OK
   > Bazaar - OK
   > Tar tool - OK
   > Unzip tool - OK
   > Unrar tool - is missing
   > 7z  data compression utility - OK
   > XZ data compression utility - OK
   > Make - OK
   > cMake - OK
   > patch tool - OK
   > autoreconf tool - OK

   > Path searched for tools: /usr/lib64/qt-3.3/bin /usr/lib64/ccache
   /usr/local/bin /bin /usr/bin /usr/local/sbin /usr/sbin /sbin
   /home/tomh/bin bin

In particular, download tools such as Mercurial, CVS, GIT, and Bazaar
are our principal concerns at this point, since they allow us to fetch
the code.  Please install missing tools at this stage if you are able to.

Next, try to download the software:

::

  ./bake.py download

should yield something like:

::

   >> Searching for system dependency pygoocanvas - OK
   >> Searching for system dependency python-dev - OK
   >> Searching for system dependency pygraphviz - OK
   >> Downloading pybindgen-0.16.0.825 - OK
   >> Searching for system dependency g++ - OK
   >> Searching for system dependency qt4 - OK
   >> Downloading netanim-3.103 - OK
   >> Downloading ns-3.17 - OK    

The above suggests that three sources have been downloaded.  Check the
``source`` directory now and type ``ls``; one should see:

::

  netanim-3.103  ns-3.17  pybindgen-0.16.0.825

You are now ready to build the |ns3| distribution.

Building ns-3
*************

Building with build.py
++++++++++++++++++++++
When working from a released tarball, the first time you build the 
|ns3| project you can build using a convenience program found in the
``allinone`` directory.  This program is called ``build.py``.  This 
program will get the project configured for you
in the most commonly useful way.  However, please note that more advanced
configuration and work with |ns3| will typically involve using the
native |ns3| build system, Waf, to be introduced later in this tutorial.  

If you downloaded
using a tarball you should have a directory called something like 
``ns-allinone-3.17`` under your ``~/workspace`` directory.  
Type the following:

::

  $ ./build.py --enable-examples --enable-tests

Because we are working with examples and tests in this tutorial, and
because they are not built by default in |ns3|, the arguments for
build.py tells it to build them for us.  The program also defaults to
building all available modules.  Later, you can build
|ns3| without examples and tests, or eliminate the modules that
are not necessary for your work, if you wish.

You will see lots of typical compiler output messages displayed as the build
script builds the various pieces you downloaded.  Eventually you should see the
following magic words:

::

   Waf: Leaving directory `/path/to/workspace/ns-allinone-3.17/ns-3.17/build'
   'build' finished successfully (6m25.032s)
  
   Modules built:
   antenna                   aodv                      applications             
   bridge                    buildings                 config-store             
   core                      csma                      csma-layout              
   dsdv                      dsr                       emu                      
   energy                    fd-net-device             flow-monitor             
   internet                  lte                       mesh                     
   mobility                  mpi                       netanim (no Python)      
   network                   nix-vector-routing        olsr                     
   point-to-point            point-to-point-layout     propagation              
   spectrum                  stats                     tap-bridge               
   test (no Python)          tools                     topology-read            
   uan                       virtual-net-device        wifi                     
   wimax                    
  
   Modules not built (see ns-3 tutorial for explanation):
   brite                     click                     openflow                 
   visualizer               

   Leaving directory `./ns-3.17'

Regarding the portion about modules not built:

::

  Modules not built (see ns-3 tutorial for explanation):
  brite                     click                     openflow                 
  visualizer               

This just means that some ns-3 modules that have dependencies on
outside libraries may not have been built, or that the configuration
specifically asked not to build them.  It does not mean that the 
simulator did not build successfully or that it will provide wrong 
results for the modules listed as being built.

Building with bake
++++++++++++++++++

If you used bake above to fetch source code from project repositories, you
may continue to use it to build |ns3|.  Type 

::

  $ ./bake.py build

and you should see something like:

:: 

  >> Building pybindgen-0.16.0.825 - OK
  >> Building netanim-3.103 - OK
  >> Building ns-3.17 - OK

*Hint:  you can also perform both steps, download and build by calling 'bake.py deploy'.*

If there happens to be a failure, please have a look at what the following
command tells you; it may give a hint as to a missing dependency:

::

  $ ./bake.py show

This will list out the various dependencies of the packages you are
trying to build.

Building with Waf
+++++++++++++++++

Up to this point, we have used either the `build.py` script, or the
`bake` tool, to get started with building |ns3|.  These tools are useful
for building |ns3| and supporting libraries, and they call into
the |ns3| directory to call the Waf build tool to do the actual building.  
Most users quickly transition to using Waf directly to configure and 
build |ns3|.  So, to proceed, please change your working directory to 
the |ns3| directory that you have initially built.

It's not 
strictly required at this point, but it will be valuable to take a slight
detour and look at how to make changes to the configuration of the project.
Probably the most useful configuration change you can make will be to 
build the optimized version of the code.  By default you have configured
your project to build the debug version.  Let's tell the project to 
make an optimized build.  To explain to Waf that it should do optimized
builds that include the examples and tests, you will need to execute the 
following commands,

::

  $ ./waf clean
  $ ./waf -d optimized --enable-examples --enable-tests configure

This runs Waf out of the local directory (which is provided as a convenience
for you).  The first command to clean out the previous build is not 
typically strictly necessary but is good practice; it will remove the
previously built libraries and object files found in directory ``build/``. 
When the project is reconfigured and the build system checks for various 
dependencies, you should see
output that looks similar to the following,

::

  Checking for program g++                 : ok /usr/bin/g++
  Checking for program cpp                 : ok /usr/bin/cpp
  Checking for program ar                  : ok /usr/bin/ar
  Checking for program ranlib              : ok /usr/bin/ranlib
  Checking for g++                         : ok
  Checking for program pkg-config          : ok /usr/bin/pkg-config
  Checking for -Wno-error=deprecated-declarations support : yes
  Checking for -Wl,--soname=foo support                   : yes
  Checking for header stdlib.h                            : ok
  Checking for header signal.h                            : ok
  Checking for header pthread.h                           : ok
  Checking for high precision time implementation         : 128-bit integer
  Checking for header stdint.h                            : ok
  Checking for header inttypes.h                          : ok
  Checking for header sys/inttypes.h                      : not found
  Checking for library rt                                 : ok
  Checking for header netpacket/packet.h                  : ok
  Checking for pkg-config flags for GSL                   : ok
  Checking for header linux/if_tun.h                      : ok
  Checking for pkg-config flags for GTK_CONFIG_STORE      : ok
  Checking for pkg-config flags for LIBXML2               : ok
  Checking for library sqlite3                            : ok
  Checking for NSC location                               : ok ../nsc (guessed)
  Checking for library dl                                 : ok
  Checking for NSC supported architecture x86_64          : ok
  Checking for program python                             : ok /usr/bin/python
  Checking for Python version >= 2.3                      : ok 2.5.2
  Checking for library python2.5                          : ok
  Checking for program python2.5-config                   : ok /usr/bin/python2.5-config
  Checking for header Python.h                            : ok
  Checking for -fvisibility=hidden support                : yes
  Checking for pybindgen location                         : ok ../pybindgen (guessed)
  Checking for Python module pybindgen                    : ok
  Checking for pybindgen version                          : ok 0.10.0.640
  Checking for Python module pygccxml                     : ok
  Checking for pygccxml version                           : ok 0.9.5
  Checking for program gccxml                             : ok /usr/local/bin/gccxml
  Checking for gccxml version                             : ok 0.9.0
  Checking for program sudo                               : ok /usr/bin/sudo
  Checking for program hg                                 : ok /usr/bin/hg
  Checking for program valgrind                           : ok /usr/bin/valgrind
  ---- Summary of optional NS-3 features:
  Threading Primitives          : enabled
  Real Time Simulator           : enabled
  Emulated Net Device           : enabled
  GNU Scientific Library (GSL)  : enabled
  Tap Bridge                    : enabled
  GtkConfigStore                : enabled
  XmlIo                         : enabled
  SQlite stats data output      : enabled
  Network Simulation Cradle     : enabled
  Python Bindings               : enabled
  Python API Scanning Support   : enabled
  Use sudo to set suid bit      : not enabled (option --enable-sudo not selected)
  Build tests                   : enabled
  Build examples                : enabled
  Static build                  : not enabled (option --enable-static not selected)
  'configure' finished successfully (2.870s)

Note the last part of the above output.  Some ns-3 options are not enabled by
default or require support from the underlying system to work properly.
For instance, to enable XmlTo, the library libxml-2.0 must be found on the
system.  If this library were not found, the corresponding |ns3| feature 
would not be enabled and a message would be displayed.  Note further that there is 
a feature to use the program ``sudo`` to set the suid bit of certain programs.
This is not enabled by default and so this feature is reported as "not enabled."

Now go ahead and switch back to the debug build that includes the examples and tests.

::

  $ ./waf clean
  $ ./waf -d debug --enable-examples --enable-tests configure

The build system is now configured and you can build the debug versions of 
the |ns3| programs by simply typing

::

  $ ./waf

Okay, sorry, I made you build the |ns3| part of the system twice,
but now you know how to change the configuration and build optimized code.

Here are a few more introductory tips about Waf.
Some waf commands are meaningful during the build phase and some commands are valid
in the configuration phase.  For example, if you wanted to use the emulation 
features of |ns3|, you might want to enable setting the suid bit using
sudo as described above.  This turns out to be a configuration-time command, and so 
you could reconfigure using the following command that also includes the examples and tests

::

  $ ./waf configure -d debug --enable-sudo --enable-examples --enable-tests

If you do this, waf will have run sudo to change the socket creator programs of the
emulation code to run as root.  There are many other configure- and build-time options
available in waf.  To explore these options, type:

::

  $ ./waf --help

We'll use some of the testing-related commands in the next section.

Finally, as an aside, it is possible to specify that waf builds the 
project in 
a directory different than the default ``build/`` directory by passing
the ``-o`` option to configure; e.g.

::

  $ ./waf configure -d debug -o build/debug --enable-examples --enable-tests

This allows users to work with multiple builds rather than always
overwriting the last build.

In the examples above, waf uses GCC C++ compiler, command ``g++``, for
building ns-3. However, it's possible to change C++ compiler used by waf.
Say one wants to use Clang C++ compiler, command ``clang++``; it's done by

::

  $ CXX="clang++" ./waf configure
  $ ./waf build

One can also set up waf to do distributed compilation with ``distcc`` in
a similar way:

::

  $ CXX="distcc g++" ./waf configure
  $ ./waf build

More info on distcc and distributed compilation can be found on it's
`project page
<http://code.google.com/p/distcc/>`_
under Documentation section.

Testing ns-3
************

You can run the unit tests of the |ns3| distribution by running the 
"./test.py -c core" script,

::

  $ ./test.py -c core

These tests are run in parallel by waf. You should eventually
see a report saying that,

::

  92 of 92 tests passed (92 passed, 0 failed, 0 crashed, 0 valgrind errors)

This is the important message.

You will also see output from the test runner and the output will actually look something like,

::

  Waf: Entering directory `/path/to/workspace/ns-3-allinone/ns-3-dev/build'
  Waf: Leaving directory `/path/to/workspace/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (1.799s)
  
  Modules built: 
  aodv                      applications              bridge
  click                     config-store              core
  csma                      csma-layout               dsdv
  emu                       energy                    flow-monitor
  internet                  lte                       mesh
  mobility                  mpi                       netanim
  network                   nix-vector-routing        ns3tcp
  ns3wifi                   olsr                      openflow
  point-to-point            point-to-point-layout     propagation
  spectrum                  stats                     tap-bridge
  template                  test                      tools
  topology-read             uan                       virtual-net-device
  visualizer                wifi                      wimax

  PASS: TestSuite ns3-wifi-interference
  PASS: TestSuite histogram
  PASS: TestSuite sample
  PASS: TestSuite ipv4-address-helper
  PASS: TestSuite devices-wifi
  PASS: TestSuite propagation-loss-model

  ...

  PASS: TestSuite attributes
  PASS: TestSuite config
  PASS: TestSuite global-value
  PASS: TestSuite command-line
  PASS: TestSuite basic-random-number
  PASS: TestSuite object
  PASS: TestSuite random-number-generators
  92 of 92 tests passed (92 passed, 0 failed, 0 crashed, 0 valgrind errors)

This command is typically run by users to quickly verify that an 
|ns3| distribution has built correctly.  

Running a Script
****************
We typically run scripts under the control of Waf.  This allows the build 
system to ensure that the shared library paths are set correctly and that
the libraries are available at run time.  To run a program, simply use the
``--run`` option in Waf.  Let's run the |ns3| equivalent of the
ubiquitous hello world program by typing the following:

::

  $ ./waf --run hello-simulator

Waf first checks to make sure that the program is built correctly and 
executes a build if required.  Waf then executes the program, which 
produces the following output.

::

  Hello Simulator

*Congratulations.  You are now an ns-3 user.*

*What do I do if I don't see the output?*

If you see ``waf`` messages indicating that the build was 
completed successfully, but do not see the "Hello Simulator" output, 
chances are that you have switched your build mode to "optimized" in 
the "Building with Waf" section, but have missed the change back to 
"debug" mode.  All of the console output used in this tutorial uses a 
special |ns3| logging component that is useful for printing 
user messages to the console.  Output from this component is 
automatically disabled when you compile optimized code -- it is 
"optimized out."  If you don't see the "Hello Simulator" output,
type the following,

::

  $ ./waf configure -d debug --enable-examples --enable-tests

to tell ``waf`` to build the debug versions of the |ns3| 
programs that includes the examples and tests.  You must still build 
the actual debug version of the code by typing,

::

  $ ./waf

Now, if you run the ``hello-simulator`` program, you should see the 
expected output.

If you want to run programs under another tool such as gdb or valgrind,
see this `wiki entry
<http://www.nsnam.org/wiki/index.php/User_FAQ#How_to_run_NS-3_programs_under_another_tool>`_.

