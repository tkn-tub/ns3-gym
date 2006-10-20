If you want to build ns3, you need to install scons (see
http://www.scons.org). scons takes care of building
the whole source tree using your system compiler. scons
0.91.1 and 0.91.96 have been tested and are known to 
work on linux FC5, Mac os X and MinGW.

To start a build, you can just type 'scons' which
will generate a debug shared build by default, located
in the directory 'build-dir/dbg-shared/bin' and
'build-dir/dbg-shared/lib'.

All builds are built with debugging symbols. Debugging
builds enable asserts while optimized builds disable them.
On platforms which support it, rpath is used which means that
the executable binaries generated link explicitely against
the right libraries. This saves you the pain of having to
setup environment variables to point to the right libraries.

1) Options
----------

- verbose: if you have installed scons 0.91.96 or higher, 
  the default build output is terse. To get a more verbose 
  output, you need to set the 'verbose' variable to 'y'.
Example: scons verbose=y
- cflags: flags for the C compiler.
Example: scons cflags="-O3 -ffast-math"
- cxxflags: flags for the C++ compiler.
Example: scons cxxflags="-O3 -ffast-math"
- ldflags: flags for the linker:
Example: scons ldflags="-L/foo -L/bar"

2) Targets
----------

- doc: build the doxygen documentation.
Example: scons doc

- dbg-shared: a debug build using shared libraries.
  The files are built in 'build-dir/dbg-shared/'.
Example: scons dbg-shared

- dbg-static: a debug build using static libraries
  The files are built in 'build-dir/dbg-static/'.
Example: scons dbg-static

- opt-shared: an optimized build using shared libraries.
  The files are built in 'build-dir/opt-shared/'.
Example: scons opt-shared

- opt-static: an optimized build using static libraries.
  The files are built in 'build-dir/opt-static/'.
Example: scons opt-static

- dbg: an alias for dbg-shared
Example: scons dbg

- opt: an alias for opt-shared
Example: scons opt

- all: alias for dbg-shared, dbg-static, opt-shared 
  and opt-static
Example: scons all

- gcov: code coverage analysis. Build a debugging version of
  the code for code coverage analysis in 'build-dir/gcov'. Once
  the code has been built, you can run various applications to
  exercise the code paths. To generate an html report from
  the gcov data, use the lcov-report target

- lcov-report: generate html report of gcov data. The output
  is stored in 'build-dir/lcov-report/'.

- dist: generate a release tarball and zipfile from the 
  source tree. The tarball and zipfile name are generated
  according to the version number stored in the SConstruct
  file.
Example in SConstruct:
ns3 = Ns3 ()
ns3.name = 'foo'
ns3.version = '0.0.10'
Example command: scons dist
Example output files:
foo-0.0.10.tar.gz
foo-0.0.10.zip

- distcheck: generate a release tarball and zipfile and 
  attempt to run the 'all' target for the release tarball.
Example: scons distcheck

3) How the build system works
-----------------------------

The current build system defines what are called "ns3 modules": each module
is a set of source files, normal header files and installable header
files. Each module also depends on a set of other modules. We build
modules automatically in the correct order. That is, we always start
from the module which does not depend on any other module (core) and
proceed with the other modules and make sure that when a module is
built, all the modules it depends upon have already been built.

To build a module, we:
1) generate the .o files
2) link the .o files together 
3) install the installable headers in the common directory
top_build_dir/include/ns3.

This means that if you want to use a header from your own module, you
should just include it: #include "foo.h" but if you want to include a
header from another module, you need to include it with #include
"ns3/bar.h". This allows you to make sure that our "public" ns3 headers
do not conflict with existing system-level headers.   For instance,
if you were to define a header called queue.h, you would include
ns3/queue.h rather than queue.h, when including from a separate module,
since many systems provide a queue.h system include file.  

4) How to add files to a module ?
---------------------------------

In the main SConstruct file, you can add source code
to the add_sources method. For example, to add a foo.cc
file to the core module, we coud do this:
core.add_sources ('foo.cc')
Of course, if this file implements public API, its 
header should be installable:
core.add_inst_headers ('foo.h')

5) How to create a new module ?
-------------------------------

# create a new module. First arg is the name of
# the new module. Second arg is the directory in
# which all source files for this module reside.
my_module = Ns3Module ('my', 'src/my_dir')
# add it to build system
ns3.add (my_module)
# specify module dependencies. Here, depends
# on the 'ipv4' and 'core' modules
my_module.add_deps (['core', 'ipv4']) 
# add source code to build located in 
# src/my_dir
my_module.add_sources ([
	'my_a.cc',
	'my_b.cc',
	'my_c.cc'
])
my_module.add_sources ([
	'my_d.cc'
])
# add headers which are not public
my_module.add_headers ([
	'my_a.h',
	'my_c.h'
])
# add headers which are public
my_module.add_inst_headers ([
	'my_b.h'
])
my_module.add_inst_headers ([
	'my_d.h'
])
# if you need to link against an external library,
# you must add 'external' dependencies. Here, the 
# pthread library
my_module.add_external_dep ('pthread')
# by default, a module is conceptually a library. If you
# want to generate an executable from a module you need to:
my_module.set_executable ()
