	.. include:: replace.txt


Getting Started
---------------

Downloading ns-3
****************

The |ns3| system as a whole is a fairly complex system and has a
number of dependencies on other components.  Along with the systems you will
most likely deal with every day (the GNU toolchain, Mercurial, you programmer
editor) you will need to ensure that a number of additional libraries are
present on your system before proceeding.  |ns3| provides a wiki
for your reading pleasure that includes pages with many useful hints and tips.
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
and running on the target system as described in the "Getting Started" section 
of the  |ns3| web site: 
http://www.nsnam.org/getting_started.html.

The |ns3| code is available in Mercurial repositories on the server
http://code.nsnam.org.  You can also download a tarball release at
http://www.nsnam.org/releases/, or you can work with repositories
using Mercurial.  We recommend using Mercurial unless there's a good reason
not to.  See the end of this section for instructions on how to get a tarball
release.

The simplest way to get started using Mercurial repositories is to use the
``ns-3-allinone`` environment.  This is a set of scripts that manages the 
downloading and building of various subsystems of |ns3| for you.  We 
recommend that you begin your |ns3| adventures in this environment
as it can really simplify your life at this point.

Downloading ns-3 Using Mercurial
++++++++++++++++++++++++++++++++
One practice is to create a directory called ``repos`` in one's home 
directory under which one can keep local Mercurial repositories.  
*Hint:  we will assume you do this later in the tutorial.*  If you adopt
that approach, you can get a copy of ``ns-3-allinone`` by typing the 
following into your Linux shell (assuming you have installed Mercurial):

::

  cd
  mkdir repos
  cd repos
  hg clone http://code.nsnam.org/ns-3-allinone

As the hg (Mercurial) command executes, you should see something like the 
following displayed,

::

  destination directory: ns-3-allinone
  requesting all changes
  adding changesets
  adding manifests
  adding file changes
  added 31 changesets with 45 changes to 7 files
  7 files updated, 0 files merged, 0 files removed, 0 files unresolved

After the clone command completes, you should have a directory called 
``ns-3-allinone`` under your ``~/repos`` directory, the contents of which should 
look something like the following:

::

  build.py*  constants.py  dist.py*  download.py*  README  util.py

Notice that you really just downloaded some Python scripts.  The next step
will be to use those scripts to download and build the |ns3|
distribution of your choice.

If you go to the following link: http://code.nsnam.org/,
you will see a number of repositories.  Many are the private repositories of
the |ns3| development team.  The repositories of interest to you will
be prefixed with "ns-3".  Official releases of |ns3| will be 
numbered as ``ns-3.<release>.<hotfix>``.  For example, a second hotfix to a
still hypothetical release forty two of |ns3| would be numbered as
``ns-3.42.2``.

The current development snapshot (unreleased) of |ns3| may be found 
at http://code.nsnam.org/ns-3-dev/.  The 
developers attempt to keep these repository in consistent, working states but
they are in a development area with unreleased code present, so you may want 
to consider staying with an official release if you do not need newly-
introduced features.

Since the release numbers are going to be changing, I will stick with 
the more constant ns-3-dev here in the tutorial, but you can replace the 
string "ns-3-dev" with your choice of release (e.g., ns-3.10) in the 
text below.  You can find the latest version  of the
code either by inspection of the repository list or by going to the 
`"Getting Started"
<http://www.nsnam.org/getting_started.html>`_
web page and looking for the latest release identifier.

Go ahead and change into the ``ns-3-allinone`` directory you created when
you cloned that repository.  We are now going to use the ``download.py`` 
script to pull down the various pieces of |ns3| you will be using.

Go ahead and type the following into your shell (remember you can substitute
the name of your chosen release number instead of ``ns-3-dev`` -- like
``"ns-3.10"`` if you want to work with a 
stable release).

::

  ./download.py -n ns-3-dev

Note that the default for the ``-n`` option is ``ns-3-dev`` and so the
above is actually redundant.  We provide this example to illustrate how to
specify alternate repositories.  In order to download ``ns-3-dev`` you 
can actually use the defaults and simply type,

::

  ./download.py

As the hg (Mercurial) command executes, you should see something like the 
following,

::

      #
      # Get NS-3
      #
  
  Cloning ns-3 branch
   =>  hg clone http://code.nsnam.org/ns-3-dev ns-3-dev
  requesting all changes
  adding changesets
  adding manifests
  adding file changes
  added 4634 changesets with 16500 changes to 1762 files
  870 files updated, 0 files merged, 0 files removed, 0 files unresolved

This is output by the download script as it fetches the actual ``ns-3``
code from the repository.

The download script is smart enough to know that on some platforms various
pieces of ns-3 are not supported.  On your platform you may not see some
of these pieces come down.  However, on most platforms, the process should
continue with something like,

::

      #
      # Get PyBindGen
      #

  Required pybindgen version:  0.10.0.640
  Trying to fetch pybindgen; this will fail if no network connection is available.  Hit Ctrl-C to skip.
   =>  bzr checkout -rrevno:640 https://launchpad.net/pybindgen pybindgen
  Fetch was successful.

This was the download script getting the Python bindings generator for you.
Note that you will need bazaar (bzr), a version control system, to download 
PyBindGen. Next you should see (modulo platform variations) something along 
the lines of,

::

      #
      # Get NSC
      #

  Required NSC version:  nsc-0.5.0
  Retrieving nsc from https://secure.wand.net.nz/mercurial/nsc
   =>  hg clone https://secure.wand.net.nz/mercurial/nsc nsc
  requesting all changes
  adding changesets
  adding manifests
  adding file changes
  added 273 changesets with 17565 changes to 15175 files
  10622 files updated, 0 files merged, 0 files removed, 0 files unresolved

This part of the process is the script downloading the Network Simulation
Cradle for you. Note that NSC is not supported on OSX or Cygwin and works 
best with gcc-3.4 or gcc-4.2 or greater series.

After the download.py script completes, you should have several new directories
under ``~/repos/ns-3-allinone``:

::

  build.py*     constants.pyc  download.py*  nsc/        README      util.pyc
  constants.py  dist.py*       ns-3-dev/     pybindgen/  util.py

Go ahead and change into ``ns-3-dev`` under your ``~/repos/ns-3-allinone`` 
directory.  You should see something like the following there:

::

  AUTHORS       doc       ns3            scratch   testpy.supp  VERSION   waf-tools
  bindings      examples  README         src       utils        waf*      wscript
  CHANGES.html  LICENSE   RELEASE_NOTES  test.py*  utils.py     waf.bat*  wutils.py

You are now ready to build the |ns3| distribution.

Downloading ns-3 Using a Tarball
++++++++++++++++++++++++++++++++
The process for downloading |ns3| via tarball is simpler than the
Mercurial process since all of the pieces are pre-packaged for you.  You just
have to pick a release, download it and decompress it.

As mentioned above, one practice is to create a directory called ``repos``
in one's home directory under which one can keep local Mercurial repositories.
One could also keep a ``tarballs`` directory.  *Hint:  the tutorial
will assume you downloaded into a ``repos`` directory, so remember the
placekeeper.``*  If you adopt the ``tarballs`` directory approach, you can 
get a copy of a release by typing the following into your Linux shell 
(substitute the appropriate version numbers, of course):

::

  cd
  mkdir tarballs
  cd tarballs
  wget http://www.nsnam.org/releases/ns-allinone-3.10.tar.bz2
  tar xjf ns-allinone-3.10.tar.bz2

If you change into the directory ``ns-allinone-3.10`` you should see a
number of files:

::

  build.py      ns-3.10/      pybindgen-0.15.0/    util.py
  constants.py  nsc-0.5.2/    README  

You are now ready to build the |ns3| distribution.

Building ns-3
*************

Building with build.py
++++++++++++++++++++++
The first time you build the |ns3| project you should build using the
``allinone`` environment.  This will get the project configured for you
in the most commonly useful way.

Change into the directory you created in the download section above.  If you
downloaded using Mercurial you should have a directory called 
``ns-3-allinone`` under your ``~/repos`` directory.  If you downloaded
using a tarball you should have a directory called something like 
``ns-allinone-3.10`` under your ``~/tarballs`` directory.  Take a deep
breath and type the following:

::

  ./build.py --enable-examples --enable-tests

Because we are working with examples and tests in this tutorial, and
because they are not built by default in |ns3|, the arguments for
build.py tells it to build them for us.  In the future you can build
|ns3| without examples and tests if you wish.

You will see lots of typical compiler output messages displayed as the build
script builds the various pieces you downloaded.  Eventually you should see the
following magic words:

::

  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (2m30.586s)
  
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

Once the project has built you can say goodbye to your old friends, the 
``ns-3-allinone`` scripts.  You got what you needed from them and will now 
interact directly with Waf and we do it in the ``ns-3-dev`` directory,
not in the ``ns-3-allinone`` directory.  Go ahead and change into the 
``ns-3-dev`` directory (or the directory for the appropriate release you
downloaded.

::

  cd ns-3-dev

Building with Waf
+++++++++++++++++
We use Waf to configure and build the |ns3| project.  It's not 
strictly required at this point, but it will be valuable to take a slight
detour and look at how to make changes to the configuration of the project.
Probably the most useful configuration change you can make will be to 
build the optimized version of the code.  By default you have configured
your project to build the debug version.  Let's tell the project to 
make an optimized build.  To explain to Waf that it should do optimized
builds that include the examples and tests, you will need to execute the 
following command,

::

  ./waf -d optimized --enable-examples --enable-tests configure

This runs Waf out of the local directory (which is provided as a convenience
for you).  As the build system checks for various dependencies you should see
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

  ./waf -d debug --enable-examples --enable-tests configure

The build system is now configured and you can build the debug versions of 
the |ns3| programs by simply typing,

::

  ./waf

Some waf commands are meaningful during the build phase and some commands are valid
in the configuration phase.  For example, if you wanted to use the emulation 
features of |ns3|, you might want to enable setting the suid bit using
sudo as described above.  This turns out to be a configuration-time command, and so 
you could reconfigure using the following command that also includes the examples and tests

::

  ./waf -d debug --enable-sudo --enable-examples --enable-tests configure

If you do this, waf will have run sudo to change the socket creator programs of the
emulation code to run as root.  There are many other configure- and build-time options
available in waf.  To explore these options, type:

::

  ./waf --help

We'll use some of the testing-related commands in the next section.

Okay, sorry, I made you build the |ns3| part of the system twice,
but now you know how to change the configuration and build optimized code.

Testing ns-3
************

You can run the unit tests of the |ns3| distribution by running the 
"./test.py -c core" script,

::

  ./test.py -c core

These tests are run in parallel by waf. You should eventually
see a report saying that,

::

  47 of 47 tests passed (47 passed, 0 failed, 0 crashed, 0 valgrind errors)

This is the important message.

You will also see output from the test runner and the output will actually look something like,

::

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
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
  95 of 95 tests passed (95 passed, 0 failed, 0 crashed, 0 valgrind errors)

This command is typically run by ``users`` to quickly verify that an 
|ns3| distribution has built correctly.  

Running a Script
****************
We typically run scripts under the control of Waf.  This allows the build 
system to ensure that the shared library paths are set correctly and that
the libraries are available at run time.  To run a program, simply use the
``--run`` option in Waf.  Let's run the |ns3| equivalent of the
ubiquitous hello world program by typing the following:

::

  ./waf --run hello-simulator

Waf first checks to make sure that the program is built correctly and 
executes a build if required.  Waf then executes the program, which 
produces the following output.

::

  Hello Simulator

*Congratulations.  You are now an ns-3 user.*

*What do I do if I don't see the output?*

If you don't see ``waf`` messages indicating that the build was 
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

  ./waf -d debug --enable-examples --enable-tests configure

to tell ``waf`` to build the debug versions of the |ns3| 
programs that includes the examples and tests.  You must still build 
the actual debug version of the code by typing,

::

  ./waf

Now, if you run the ``hello-simulator`` program, you should see the 
expected output.

If you want to run programs under another tool such as gdb or valgrind,
see this `wiki entry
<http://www.nsnam.org/wiki/index.php/User_FAQ#How_to_run_NS-3_programs_under_another_tool>`_.

