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

Downloading |ns3|
*****************

The |ns3| system as a whole is a fairly complex system and has a
number of dependencies on other components.  Along with the systems you will
most likely deal with every day (the GNU toolchain, Mercurial, a text
editor) you will need to ensure that a number of additional libraries are
present on your system before proceeding.  |ns3| provides a wiki
page that includes pages with many useful hints and tips.
One such page is the "Installation" page,
http://www.nsnam.org/wiki/Installation.

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
http://www.nsnam.org/release/, or you can work with repositories
using Mercurial.  We recommend using Mercurial unless there's a good reason
not to.  See the end of this section for instructions on how to get a tarball
release.

The simplest way to get started using Mercurial repositories is to use the
``ns-3-allinone`` environment.  This is a set of scripts that manages the 
downloading and building of various subsystems of |ns3| for you.  We 
recommend that you begin your |ns3| work in this environment.

Downloading |ns3| Using a Tarball
+++++++++++++++++++++++++++++++++

A tarball is a particular format of software archive where multiple
files are bundled together and the archive possibly compressed.
|ns3| software releases are provided via a downloadable tarball.
The process for downloading |ns3| via tarball is simple; you just
have to pick a release, download it and decompress it.

Let's assume that you, as a user, wish to build |ns3| in a local
directory called ``workspace``. 
If you adopt the ``workspace`` directory approach, you can 
get a copy of a release by typing the following into your Linux shell 
(substitute the appropriate version numbers, of course)::

  $ cd
  $ mkdir workspace
  $ cd workspace
  $ wget http://www.nsnam.org/release/ns-allinone-3.20.tar.bz2
  $ tar xjf ns-allinone-3.20.tar.bz2

If you change into the directory ``ns-allinone-3.20`` you should see a
number of files::

  $ ls
  bake      constants.py   ns-3.20               README
  build.py  netanim-3.103  pybindgen-0.16.0.825  util.py

You are now ready to build the |ns3| distribution.

Downloading |ns3| Using Bake
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
following into your Linux shell (assuming you have installed Mercurial)::

  $ cd
  $ mkdir workspace
  $ cd workspace
  $ hg clone http://code.nsnam.org/bake

As the hg (Mercurial) command executes, you should see something like the 
following displayed,

::

  ...
  destination directory: bake
  requesting all changes
  adding changesets
  adding manifests
  adding file changes
  added 252 changesets with 661 changes to 62 files
  updating to branch default
  45 files updated, 0 files merged, 0 files removed, 0 files unresolved

After the clone command completes, you should have a directory called 
``bake``, the contents of which should look something like the following::

  $ ls
  bake                  bakeconf.xml  doc       generate-binary.py  TODO
  bake.py               examples      test

Notice that you really just downloaded some Python scripts and a Python
module called ``bake``.  The next step
will be to use those scripts to download and build the |ns3|
distribution of your choice.

There are a few configuration targets available:

1.  ``ns-3.20``:  the module corresponding to the release; it will download
    components similar to the release tarball.
2.  ``ns-3-dev``:  a similar module but using the development code tree
3.  ``ns-allinone-3.20``:  the module that includes other optional features
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
this tutorial example with ``ns-3.20``.

We are now going to use the bake tool to pull down the various pieces of 
|ns3| you will be using.  First, we'll say a word about running bake.

bake works by downloading source packages into a source directory,
and installing libraries into a build directory.  bake can be run
by referencing the binary, but if one chooses to run bake from
outside of the directory it was downloaded into, it is advisable
to put bake into your path, such as follows (Linux bash shell example).
First, change into the 'bake' directory, and then set the following
environment variables

::

  $ export BAKE_HOME=`pwd`
  $ export PATH=$PATH:$BAKE_HOME:$BAKE_HOME/build/bin
  $ export PYTHONPATH=$PYTHONPATH:$BAKE_HOME:$BAKE_HOME/build/lib

This will put the bake.py program into the shell's path, and will allow
other programs to find executables and libraries created by bake.  Although
several bake use cases do not require setting PATH and PYTHONPATH as above,
full builds of ns-3-allinone (with the optional packages) typically do.

Step into the workspace directory and type the following into your shell::

  $ ./bake.py configure -e ns-3.20

Next, we'l ask bake to check whether we have enough tools to download
various components.  Type::

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

Next, try to download the software::

  $ ./bake.py download

should yield something like::

   >> Searching for system dependency pygoocanvas - OK
   >> Searching for system dependency python-dev - OK
   >> Searching for system dependency pygraphviz - OK
   >> Downloading pybindgen-0.16.0.825 - OK
   >> Searching for system dependency g++ - OK
   >> Searching for system dependency qt4 - OK
   >> Downloading netanim-3.103 - OK
   >> Downloading ns-3.20 - OK    

The above suggests that three sources have been downloaded.  Check the
``source`` directory now and type ``ls``; one should see::

  $ ls
  netanim-3.103  ns-3.20  pybindgen-0.16.0.825

You are now ready to build the |ns3| distribution.

Building |ns3|
**************

Building with ``build.py``
++++++++++++++++++++++++++
When working from a released tarball, the first time you build the 
|ns3| project you can build using a convenience program found in the
``allinone`` directory.  This program is called ``build.py``.  This 
program will get the project configured for you
in the most commonly useful way.  However, please note that more advanced
configuration and work with |ns3| will typically involve using the
native |ns3| build system, Waf, to be introduced later in this tutorial.  

If you downloaded
using a tarball you should have a directory called something like 
``ns-allinone-3.20`` under your ``~/workspace`` directory.  
Type the following::

  $ ./build.py --enable-examples --enable-tests

Because we are working with examples and tests in this tutorial, and
because they are not built by default in |ns3|, the arguments for
build.py tells it to build them for us.  The program also defaults to
building all available modules.  Later, you can build
|ns3| without examples and tests, or eliminate the modules that
are not necessary for your work, if you wish.

You will see lots of typical compiler output messages displayed as the build
script builds the various pieces you downloaded.  Eventually you should see the
following magic words::

   Waf: Leaving directory `/path/to/workspace/ns-allinone-3.20/ns-3.20/build'
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

   Leaving directory `./ns-3.20'

Regarding the portion about modules not built::

  Modules not built (see ns-3 tutorial for explanation):
  brite                     click                     openflow                 
  visualizer               

This just means that some |ns3| modules that have dependencies on
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

and you should see something like::

  >> Building pybindgen-0.16.0.825 - OK
  >> Building netanim-3.103 - OK
  >> Building ns-3.20 - OK

*Hint:  you can also perform both steps, download and build by calling 'bake.py deploy'.*

If there happens to be a failure, please have a look at what the following
command tells you; it may give a hint as to a missing dependency::

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
following commands::

  $ ./waf clean
  $ ./waf --build-profile=optimized --enable-examples --enable-tests configure

This runs Waf out of the local directory (which is provided as a convenience
for you).  The first command to clean out the previous build is not 
typically strictly necessary but is good practice (but see `Build Profiles`_,
below); it will remove the
previously built libraries and object files found in directory ``build/``. 
When the project is reconfigured and the build system checks for various 
dependencies, you should see
output that looks similar to the following::

  Setting top to                           : .
  Setting out to                           : build 
  Checking for 'gcc' (c compiler)          : /usr/bin/gcc 
  Checking for cc version                  : 4.2.1 
  Checking for 'g++' (c++ compiler)        : /usr/bin/g++ 
  Checking boost includes                  : 1_46_1 
  Checking boost libs                      : ok 
  Checking for boost linkage               : ok 
  Checking for click location              : not found 
  Checking for program pkg-config          : /sw/bin/pkg-config 
  Checking for 'gtk+-2.0' >= 2.12          : yes 
  Checking for 'libxml-2.0' >= 2.7         : yes 
  Checking for type uint128_t              : not found 
  Checking for type __uint128_t            : yes 
  Checking high precision implementation   : 128-bit integer (default) 
  Checking for header stdint.h             : yes 
  Checking for header inttypes.h           : yes 
  Checking for header sys/inttypes.h       : not found 
  Checking for header sys/types.h          : yes 
  Checking for header sys/stat.h           : yes 
  Checking for header dirent.h             : yes 
  Checking for header stdlib.h             : yes 
  Checking for header signal.h             : yes 
  Checking for header pthread.h            : yes 
  Checking for header stdint.h             : yes 
  Checking for header inttypes.h           : yes 
  Checking for header sys/inttypes.h       : not found 
  Checking for library rt                  : not found 
  Checking for header netpacket/packet.h   : not found 
  Checking for header sys/ioctl.h          : yes 
  Checking for header net/if.h             : not found 
  Checking for header net/ethernet.h       : yes 
  Checking for header linux/if_tun.h       : not found 
  Checking for header netpacket/packet.h   : not found 
  Checking for NSC location                : not found 
  Checking for 'mpic++'                    : yes 
  Checking for 'sqlite3'                   : yes 
  Checking for header linux/if_tun.h       : not found 
  Checking for program sudo                : /usr/bin/sudo 
  Checking for program valgrind            : /sw/bin/valgrind 
  Checking for 'gsl'                       : yes 
  Checking for compilation flag -Wno-error=deprecated-d... support : ok 
  Checking for compilation flag -Wno-error=deprecated-d... support : ok 
  Checking for compilation flag -fstrict-aliasing... support       : ok 
  Checking for compilation flag -fstrict-aliasing... support       : ok 
  Checking for compilation flag -Wstrict-aliasing... support       : ok 
  Checking for compilation flag -Wstrict-aliasing... support       : ok 
  Checking for program doxygen                                     : /usr/local/bin/doxygen 
  ---- Summary of optional NS-3 features:
  Build profile                 : debug
  Build directory               : build
  Python Bindings               : enabled
  BRITE Integration             : not enabled (BRITE not enabled (see option --with-brite))
  NS-3 Click Integration        : not enabled (nsclick not enabled (see option --with-nsclick))
  GtkConfigStore                : enabled
  XmlIo                         : enabled
  Threading Primitives          : enabled
  Real Time Simulator           : enabled (librt is not available)
  Emulated Net Device           : enabled (<netpacket/packet.h> include not detected)
  File descriptor NetDevice     : enabled
  Tap FdNetDevice               : not enabled (needs linux/if_tun.h)
  Emulation FdNetDevice         : not enabled (needs netpacket/packet.h)
  PlanetLab FdNetDevice         : not enabled (PlanetLab operating system not detected (see option --force-planetlab))
  Network Simulation Cradle     : not enabled (NSC not found (see option --with-nsc))
  MPI Support                   : enabled
  NS-3 OpenFlow Integration     : not enabled (Required boost libraries not found, missing: system, signals, filesystem)
  SQlite stats data output      : enabled
  Tap Bridge                    : not enabled (<linux/if_tun.h> include not detected)
  PyViz visualizer              : enabled
  Use sudo to set suid bit      : not enabled (option --enable-sudo not selected)
  Build tests                   : enabled
  Build examples                : enabled
  GNU Scientific Library (GSL)  : enabled
  'configure' finished successfully (1.944s)

Note the last part of the above output.  Some |ns3| options are not enabled by
default or require support from the underlying system to work properly.
For instance, to enable XmlTo, the library libxml-2.0 must be found on the
system.  If this library were not found, the corresponding |ns3| feature 
would not be enabled and a message would be displayed.  Note further that there is 
a feature to use the program ``sudo`` to set the suid bit of certain programs.
This is not enabled by default and so this feature is reported as "not enabled."

Now go ahead and switch back to the debug build that includes the examples and tests.

::

  $ ./waf clean
  $ ./waf --build-profile=debug --enable-examples --enable-tests configure

The build system is now configured and you can build the debug versions of 
the |ns3| programs by simply typing

::

  $ ./waf

Okay, sorry, I made you build the |ns3| part of the system twice,
but now you know how to change the configuration and build optimized code.

The build.py script discussed above supports also the ``--enable-examples``
and ``enable-tests`` arguments, but in general, does not directly support
other waf options; for example, this will not work:

::

  $ ./build.py --disable-python

will result in

::

  build.py: error: no such option: --disable-python

However, the special operator ``--`` can be used to pass additional
options through to waf, so instead of the above, the following will work:

::

  $ ./build.py -- --disable-python   

as it generates the underlying command ``./waf configure --disable-python``.

Here are a few more introductory tips about Waf.

Configure vs. Build
===================

Some Waf commands are only meaningful during the configure phase and some commands are valid
in the build phase.  For example, if you wanted to use the emulation 
features of |ns3|, you might want to enable setting the suid bit using
sudo as described above.  This turns out to be a configuration-time command, and so 
you could reconfigure using the following command that also includes the examples and tests.

::

  $ ./waf configure --enable-sudo --enable-examples --enable-tests

If you do this, Waf will have run sudo to change the socket creator programs of the
emulation code to run as root.

There are many other configure- and build-time options
available in Waf.  To explore these options, type::

  $ ./waf --help

We'll use some of the testing-related commands in the next section.

Build Profiles
==============

We already saw how you can configure Waf for ``debug`` or ``optimized`` builds::

  $ ./waf --build-profile=debug

There is also an intermediate build profile, ``release``.  ``-d`` is a
synonym for ``--build-profile``.

By default Waf puts the build artifacts in the ``build`` directory.  
You can specify a different output directory with the ``--out``
option, e.g.

::

  $ ./waf configure --out=foo

Combining this with build profiles lets you switch between the different
compile options in a clean way::

  $ ./waf configure --build-profile=debug --out=build/debug
  $ ./waf build
  ...
  $ ./waf configure --build-profile=optimized --out=build/optimized
  $ ./waf build
  ...

This allows you to work with multiple builds rather than always
overwriting the last build.  When you switch, Waf will only compile
what it has to, instead of recompiling everything.

When you do switch build profiles like this, you have to be careful
to give the same configuration parameters each time.  It may be convenient
to define some environment variables to help you avoid mistakes::

  $ export NS3CONFIG="--enable-examples --enable-tests"
  $ export NS3DEBUG="--build-profile=debug --out=build/debug"
  $ export NS3OPT=="--build-profile=optimized --out=build/optimized"

  $ ./waf configure $NS3CONFIG $NS3DEBUG
  $ ./waf build
  ...
  $ ./waf configure $NS3CONFIG $NS3OPT
  $ ./waf build

Compilers
=========

In the examples above, Waf uses the GCC C++ compiler, ``g++``, for
building |ns3|. However, it's possible to change the C++ compiler used by Waf
by defining the ``CXX`` environment variable.
For example, to use the Clang C++ compiler, ``clang++``,

::

  $ CXX="clang++" ./waf configure
  $ ./waf build

One can also set up Waf to do distributed compilation with ``distcc`` in
a similar way::

  $ CXX="distcc g++" ./waf configure
  $ ./waf build

More info on ``distcc`` and distributed compilation can be found on it's
`project page
<http://code.google.com/p/distcc/>`_
under Documentation section.

Install
=======

Waf may be used to install libraries in various places on the system.
The default location where libraries and executables are built is
in the ``build`` directory, and because Waf knows the location of these
libraries and executables, it is not necessary to install the libraries
elsewhere.

If users choose to install things outside of the build directory, users
may issue the ``./waf install`` command.  By default, the prefix for
installation is ``/usr/local``, so ``./waf install`` will install programs
into ``/usr/local/bin``, libraries into ``/usr/local/lib``, and headers
into ``/usr/local/include``.  Superuser privileges are typically needed
to install to the default prefix, so the typical command would be
``sudo ./waf install``.  When running programs with Waf, Waf will
first prefer to use shared libraries in the build directory, then 
will look for libraries in the library path configured in the local
environment.  So when installing libraries to the system, it is good
practice to check that the intended libraries are being used.

Users may choose to install to a different prefix by passing the ``--prefix``
option at configure time, such as:

::

  ./waf configure --prefix=/opt/local

If later after the build the user issues the ``./waf install`` command, the 
prefix ``/opt/local`` will be used.

The ``./waf clean`` command should be used prior to reconfiguring 
the project if Waf will be used to install things at a different prefix.

In summary, it is not necessary to call ``./waf install`` to use |ns3|.
Most users will not need this command since Waf will pick up the
current libraries from the ``build`` directory, but some users may find 
it useful if their use case involves working with programs outside
of the |ns3| directory.

One Waf
=======

There is only one Waf script, at the top level of the |ns3| source tree.
As you work, you may find yourself spending a lot of time in ``scratch/``,
or deep in ``src/...``, and needing to invoke Waf.  You could just
remember where you are, and invoke Waf like this::

  $ ../../../waf ...

but that get's tedious, and error prone, and there are better solutions.

If you have the full |ns3| repository this little gem is a start::

  $ cd $(hg root) && ./waf ...

Even better is to define this as a shell function::

  $ function waff { cd $(hg root) && ./waf $* ; }

  $ waff build

If you only have the tarball, an environment variable can help::

  $ export NS3DIR="$PWD"
  $ function waff { cd $NS3DIR && ./waf $* ; }

  $ cd scratch
  $ waff build

It might be tempting in a module directory to add a trivial ``waf``
script along the lines of ``exec ../../waf``.  Please don't.  It's
confusing to new-comers, and when done poorly it leads to subtle build
errors.  The solutions above are the way to go.


Testing |ns3|
*************

You can run the unit tests of the |ns3| distribution by running the 
``./test.py -c core`` script::

  $ ./test.py -c core

These tests are run in parallel by Waf. You should eventually
see a report saying that

::

  92 of 92 tests passed (92 passed, 0 failed, 0 crashed, 0 valgrind errors)

This is the important message.

You will also see the summary output from Waf and the test runner
executing each test, which will actually look something like::

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

  ...

  PASS: TestSuite object
  PASS: TestSuite random-number-generators
  92 of 92 tests passed (92 passed, 0 failed, 0 crashed, 0 valgrind errors)

This command is typically run by users to quickly verify that an 
|ns3| distribution has built correctly.  (Note the order of the ``PASS: ...``
lines can vary, which is okay.  What's important is that the summary line at
the end report that all tests passed; none failed or crashed.)

Running a Script
****************

We typically run scripts under the control of Waf.  This allows the build 
system to ensure that the shared library paths are set correctly and that
the libraries are available at run time.  To run a program, simply use the
``--run`` option in Waf.  Let's run the |ns3| equivalent of the
ubiquitous hello world program by typing the following::

  $ ./waf --run hello-simulator

Waf first checks to make sure that the program is built correctly and 
executes a build if required.  Waf then executes the program, which 
produces the following output.

::

  Hello Simulator

Congratulations!  You are now an ns-3 user!

**What do I do if I don't see the output?**

If you see Waf messages indicating that the build was 
completed successfully, but do not see the "Hello Simulator" output, 
chances are that you have switched your build mode to ``optimized`` in 
the `Building with Waf`_ section, but have missed the change back to 
``debug`` mode.  All of the console output used in this tutorial uses a 
special |ns3| logging component that is useful for printing 
user messages to the console.  Output from this component is 
automatically disabled when you compile optimized code -- it is 
"optimized out."  If you don't see the "Hello Simulator" output,
type the following::

  $ ./waf configure --build-profile=debug --enable-examples --enable-tests

to tell Waf to build the debug versions of the |ns3| 
programs that includes the examples and tests.  You must still build 
the actual debug version of the code by typing

::

  $ ./waf

Now, if you run the ``hello-simulator`` program, you should see the 
expected output.

Program Arguments
+++++++++++++++++

To feed command line arguments to an |ns3| program use this pattern::

  $ ./waf --run <ns3-program> --command-template="%s <args>"

Substitute your program name for ``<ns3-program>``, and the arguments
for ``<args>``.  The ``--command-template`` argument to Waf is
basically a recipe for constructing the actual command line Waf should use
to execute the program.  Waf checks that the build is complete,
sets the shared library paths, then invokes the executable
using the provided command line template, 
inserting the program name for the ``%s`` placeholder.
(I admit this is a bit awkward, but that's the way it is.  Patches welcome!)

Another particularly useful example is to run a test suite by itself.
Let's assume that a ``mytest`` test suite exists (it doesn't).
Above, we used the ``./test.py`` script to run a whole slew of
tests in parallel, by repeatedly invoking the real testing program,
``test-runner``.  To invoke ``test-runner`` directly for a single test::

  $ ./waf --run test-runner --command-template="%s --suite=mytest --verbose"

This passes the arguments to the ``test-runner`` program.
Since ``mytest`` does not exist, an error message will be generated.
To print the available ``test-runner`` options::

  $ ./waf --run test-runner --command-template="%s --help"

Debugging
+++++++++

To run |ns3| programs under the control of another utility, such as
a debugger (*e.g.* ``gdb``) or memory checker (*e.g.* ``valgrind``),
you use a similar ``--command-template="..."`` form.

For example, to run your |ns3| program ``hello-simulator`` with the arguments
``<args>`` under the ``gdb`` debugger::

  $ ./waf --run=hello-simulator --command-template="gdb %s --args <args>"

Notice that the |ns3| program name goes with the ``--run`` argument,
and the control utility (here ``gdb``) is the first token
in the ``--commmand-template`` argument.  The ``--args`` tells ``gdb``
that the remainder of the command line belongs to the "inferior" program.
(Some ``gdb``'s don't understand the ``--args`` feature.  In this case,
omit the program arguments from the ``--command-template``,
and use the ``gdb`` command ``set args``.)

We can combine this recipe and the previous one to run a test under the
debugger::

  $ ./waf --run test-runner --command-template="gdb %s --args --suite=mytest --verbose"

Working Directory
+++++++++++++++++

Waf needs to run from it's location at the top of the |ns3| tree.
This becomes the working directory where output files will be written.
But what if you want to keep those ouf to the |ns3| source tree?  Use
the ``--cwd`` argument::

  $ ./waf --cwd=...

It may be more convenient to start with your working directory where
you want the output files, in which case a little indirection can help::

  $ function waff {
      CWD="$PWD"
      cd $NS3DIR >/dev/null
      ./waf --cwd="$CWD" $*
      cd - >/dev/null
    }

This embellishment of the previous version saves the current working directory,
``cd``'s to the Waf directory, then instructs Waf to change the working
directory *back* to the saved current working directory before running the
program.
