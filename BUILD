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
