## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
import sys
import shlex
import shutil
import types
import optparse
import os.path

import Params
import Object
import pproc as subprocess

Params.g_autoconfig = 1


# the following two variables are used by the target "waf dist"
VERSION = file("VERSION").read().strip()
APPNAME = 'ns'

# these variables are mandatory ('/' are converted automatically)
srcdir = '.'
blddir = 'build'

#
# The directory in which the tarball of the reference traces lives.  This is
# used if Mercurial is not on the system.
#
REGRESSION_TRACES_URL = "http://www.nsnam.org/releases/"

#
# The name of the tarball to find the reference traces in if there is no
# mercurial on the system.  It is expected to be created using tar -cjf and
# will be extracted using tar -xjf
#
REGRESSION_TRACES_TAR_NAME  = "ns-3.0.12-ref-traces.tar.bz2"

#
# The path to the Mercurial repository used to find the reference traces if
# we find "hg" on the system.  We expect that the repository will be named
# identically to refDirName below
#
REGRESSION_TRACES_REPO = "http://code.nsnam.org/"

#
# The local directory name (relative to the 'regression' dir) into
# which the reference traces will go in either case (net or hg).
#
REGRESSION_TRACES_DIR_NAME = "ns-3-dev-ref-traces"


def dist_hook():
    shutil.rmtree("doc/html", True)
    shutil.rmtree("doc/latex", True)

def set_options(opt):

    def debug_option_callback(option, opt, value, parser):
        if value == 'debug':
            setattr(parser.values, option.dest, 'ultradebug')
        elif value == 'optimized':
            setattr(parser.values, option.dest, 'optimized')
        else:
            raise optparse.OptionValueError("allowed --debug-level values"
                                            " are debug, optimized.")

    opt.add_option('-d', '--debug-level',
                   action='callback',
                   type="string", dest='debug_level', default='ultradebug',
                   help=('Specify the debug level, does nothing if CFLAGS is set'
                         ' in the environment. [Allowed Values: debug, optimized].'
                         ' WARNING: this option only has effect '
                         'with the configure command.'),
                   callback=debug_option_callback)
    
    # options provided by the modules
    opt.tool_options('compiler_cxx')

    opt.add_option('--cwd',
                   help=('Set the working directory for a program.'),
                   action="store", type="string", default=None,
                   dest='cwd_launch')

    opt.add_option('--enable-gcov',
                   help=('Enable code coverage analysis.'
                         ' WARNING: this option only has effect '
                         'with the configure command.'),
                   action="store_true", default=False,
                   dest='enable_gcov')

    opt.add_option('--no-task-lines',
                   help=("Don't print task lines, i.e. messages saying which tasks are being executed by WAF."
                         "  Coupled with a single -v will cause WAF to output only the executed commands,"
                         " just like 'make' does by default."),
                   action="store_true", default=False,
                   dest='no_task_lines')

    opt.add_option('--lcov-report',
                   help=('Generate a code coverage report '
                         '(use this option at build time, not in configure)'),
                   action="store_true", default=False,
                   dest='lcov_report')

    opt.add_option('--doxygen',
                   help=('Run doxygen to generate html documentation from source comments'),
                   action="store_true", default=False,
                   dest='doxygen')

    opt.add_option('--run',
                   help=('Run a locally built program; argument can be a program name,'
                         ' or a command starting with the program name.'),
                   type="string", default='', dest='run')
    opt.add_option('--command-template',
                   help=('Template of the command used to run the program given by --run;'
                         ' It should be a shell command string containing %s inside,'
                         ' which will be replaced by the actual program.'),
                   type="string", default=None, dest='command_template')
    opt.add_option('--valgrind',
                   help=('Change the default command template to run programs and unit tests with valgrind'),
                   action="store_true", default=False,
                   dest='valgrind')
    opt.add_option('--shell',
                   help=('Run a shell with an environment suitably modified to run locally built programs'),
                   action="store_true", default=False,
                   dest='shell')

    opt.add_option('--regression',
                   help=("Enable regression testing; only used for the 'check' target"),
                   default=False, dest='regression', action="store_true")
    opt.add_option('--regression-generate',
                   help=("Generate new regression test traces; only used for the 'check' target"),
                   default=False, dest='regression_generate', action="store_true")
    opt.add_option('--regression-tests',
                   help=('For regression testing, only run/generate the indicated regression tests, '
                         'specified as a comma separated list of test names'),
                   dest='regression_tests', type="string")

    # options provided in a script in a subdirectory named "src"
    opt.sub_options('src')


def configure(conf):
    conf.check_tool('compiler_cxx')

    # create the second environment, set the variant and set its name
    variant_env = conf.env.copy()
    debug_level = Params.g_options.debug_level.lower()
    if debug_level == 'ultradebug':
        variant_name = 'debug'
    else:
        variant_name = debug_level

    variant_env['INCLUDEDIR'] = os.path.join(variant_env['PREFIX'], 'include')

    if Params.g_options.enable_gcov:
        variant_name += '-gcov'
        variant_env.append_value('CCFLAGS', '-fprofile-arcs')
        variant_env.append_value('CCFLAGS', '-ftest-coverage')
        variant_env.append_value('CXXFLAGS', '-fprofile-arcs')
        variant_env.append_value('CXXFLAGS', '-ftest-coverage')
        variant_env.append_value('LINKFLAGS', '-fprofile-arcs')
    
    conf.env['NS3_ACTIVE_VARIANT'] = variant_name
    variant_env['NS3_ACTIVE_VARIANT'] = variant_name
    variant_env.set_variant(variant_name)
    conf.set_env_name(variant_name, variant_env)
    conf.setenv(variant_name)

    variant_env.append_value('CXXDEFINES', 'RUN_SELF_TESTS')
    
    if (os.path.basename(conf.env['CXX']).startswith("g++")
        and 'CXXFLAGS' not in os.environ):
        variant_env.append_value('CXXFLAGS', ['-Werror'])

    if 'debug' in Params.g_options.debug_level.lower():
        variant_env.append_value('CXXDEFINES', 'NS3_ASSERT_ENABLE')
        variant_env.append_value('CXXDEFINES', 'NS3_LOG_ENABLE')

    ## In optimized builds we still want debugging symbols, e.g. for
    ## profiling, and at least partially usable stack traces.
    if ('optimized' in Params.g_options.debug_level.lower() 
        and 'CXXFLAGS' not in os.environ):
        for flag in variant_env['CXXFLAGS_DEBUG']:
            ## this probably doesn't work for MSVC
            if flag.startswith('-g'):
                variant_env.append_value('CXXFLAGS', flag)

    ## in optimized builds, replace -O2 with -O3
    if 'optimized' in Params.g_options.debug_level.lower():
        lst = variant_env['CXXFLAGS']
        for i, flag in enumerate(lst):
            if flag == '-O2':
                lst[i] = '-O3'

    if sys.platform == 'win32':
        if os.path.basename(conf.env['CXX']).startswith("g++"):
            variant_env.append_value("LINKFLAGS", "-Wl,--enable-runtime-pseudo-reloc")

    conf.sub_config('src')
    conf.sub_config('utils')

    if Params.g_options.enable_modules:
        conf.env['NS3_ENABLED_MODULES'] = ['ns3-'+mod for mod in
                                           Params.g_options.enable_modules.split(',')]


def create_ns3_program(bld, name, dependencies=('simulator',)):
    program = bld.create_obj('cpp', 'program')
    program.name = name
    program.target = program.name
    program.uselib_local = 'ns3'
    program.ns3_module_dependencies = ['ns3-'+dep for dep in dependencies]
    return program


def build(bld):
    if Params.g_options.no_task_lines:
        import Runner
        def null_printout(s):
            pass
        Runner.printout = null_printout

    Params.g_cwd_launch = Params.g_build.m_curdirnode.abspath()
    bld.create_ns3_program = types.MethodType(create_ns3_program, bld)
    variant_name = bld.env_of_name('default')['NS3_ACTIVE_VARIANT']
    variant_env = bld.env_of_name(variant_name)
    bld.m_allenvs['default'] = variant_env # switch to the active variant

    if Params.g_options.shell:
        run_shell()
        raise SystemExit(0)

    if Params.g_options.doxygen:
        doxygen()
        raise SystemExit(0)

    check_shell()

    if Params.g_options.doxygen:
        doxygen()
        raise SystemExit(0)

    print "Entering directory `%s/build'" % Params.g_build.m_curdirnode.abspath()
    # process subfolders from here
    bld.add_subdirs('src')
    bld.add_subdirs('samples utils examples tutorial')

    ## if --enabled-modules option was given, we disable building the
    ## modules that were not enabled, and programs that depend on
    ## disabled modules.
    env = bld.env()

    if Params.g_options.enable_modules:
        Params.warning("the option --enable-modules is being applied to this build only;"
                       " to make it permanent it needs to be given to waf configure.")
        env['NS3_ENABLED_MODULES'] = ['ns3-'+mod for mod in
                                      Params.g_options.enable_modules.split(',')]

    if env['NS3_ENABLED_MODULES']:
        modules = env['NS3_ENABLED_MODULES']
        changed = True
        while changed:
            changed = False
            for module in modules:
                module_obj = Object.name_to_obj(module)
                if module_obj is None:
                    raise ValueError("module %s not found" % module)
                for dep in module_obj.add_objects:
                    if not dep.startswith('ns3-'):
                        continue
                    if dep not in modules:
                        modules.append(dep)
                        changed = True

        ## remove objects that depend on modules not listed
        for obj in list(Object.g_allobjs):
            if hasattr(obj, 'ns3_module_dependencies'):
                for dep in obj.ns3_module_dependencies:
                    if dep not in modules:
                        Object.g_allobjs.remove(obj)
                        break
            if obj.name in env['NS3_MODULES'] and obj.name not in modules:
                Object.g_allobjs.remove(obj)

    ## Create a single ns3 library containing all enabled modules
    lib = bld.create_obj('cpp', 'shlib')
    lib.name = 'ns3'
    lib.target = 'ns3'
    if env['NS3_ENABLED_MODULES']:
        lib.add_objects = list(modules)
    else:
        lib.add_objects = list(env['NS3_MODULES'])


def get_command_template():
    if Params.g_options.valgrind:
        if Params.g_options.command_template:
            Params.fatal("Options --command-template and --valgrind are conflicting")
        return "valgrind --leak-check=full %s"
    else:
        return (Params.g_options.command_template or '%s')


def shutdown():
    #import UnitTest
    #ut = UnitTest.unit_test()
    #ut.change_to_testfile_dir = True
    #ut.want_to_see_test_output = True
    #ut.want_to_see_test_error = True
    #ut.run()
    #ut.print_results()

    if Params.g_commands['check']:
        _run_waf_check()

    if Params.g_options.regression or Params.g_options.regression_generate:
        _dir = os.getcwd()
        os.chdir("regression")
        try:
            run_regression()
        finally:
            os.chdir(_dir)

    if Params.g_options.lcov_report:
        lcov_report()

    if Params.g_options.run:
        run_program(Params.g_options.run, get_command_template())
        raise SystemExit(0)

    if Params.g_options.command_template:
        Params.fatal("Option --command-template requires the option --run to be given")

def _run_waf_check():
    ## generate the trace sources list docs
    env = Params.g_build.env_of_name('default')
    proc_env = _get_proc_env()
    try:
        program_obj = _find_program('print-introspected-doxygen', env)
    except ValueError: # could happen if print-introspected-doxygen is
                       # not built because of waf configure
                       # --enable-modules=xxx
        pass
    else:
        prog = program_obj.path.find_build(program_obj.get_target_name()).abspath(env)
        out = open('doc/introspected-doxygen.h', 'w')
        if subprocess.Popen([prog], stdout=out, env=proc_env).wait():
            raise SystemExit(1)
        out.close()

    run_program('run-tests', get_command_template())


def _find_program(program_name, env):
    launch_dir = os.path.abspath(Params.g_cwd_launch)
    found_programs = []
    for obj in Object.g_allobjs:
        if obj.m_type != 'program' or not obj.target:
            continue

        ## filter out programs not in the subtree starting at the launch dir
        if not (obj.path.abspath().startswith(launch_dir)
                or obj.path.abspath(env).startswith(launch_dir)):
            continue
        
        found_programs.append(obj.target)
        if obj.target == program_name:
            return obj
    raise ValueError("program '%s' not found; available programs are: %r"
                     % (program_name, found_programs))

def _get_proc_env(os_env=None):
    env = Params.g_build.env_of_name('default')
    if sys.platform == 'linux2':
        pathvar = 'LD_LIBRARY_PATH'
    elif sys.platform == 'darwin':
        pathvar = 'DYLD_LIBRARY_PATH'
    elif sys.platform == 'win32':
        pathvar = 'PATH'
    elif sys.platform == 'cygwin':
        pathvar = 'PATH'
    else:
        Params.warning(("Don't know how to configure "
                        "dynamic library path for the platform '%s'") % (sys.platform,))
        pathvar = None

    proc_env = dict(os.environ)
    if os_env is not None:
        proc_env.update(os_env)

    if pathvar is not None:
        if pathvar in proc_env:
            proc_env[pathvar] = os.pathsep.join(list(env['NS3_MODULE_PATH']) + [proc_env[pathvar]])
        else:
            proc_env[pathvar] = os.pathsep.join(list(env['NS3_MODULE_PATH']))
    return proc_env

def _run_argv(argv, os_env=None):
    proc_env = _get_proc_env(os_env)
    retval = subprocess.Popen(argv, env=proc_env).wait()
    if retval:
        Params.fatal("Command %s exited with code %i" % (argv, retval))


def run_program(program_string, command_template=None):
    """
    if command_template is not None, then program_string == program
    name and argv is given by command_template with %s replaced by the
    full path to the program.  Else, program_string is interpreted as
    a shell command with first name being the program name.
    """
    env = Params.g_build.env_of_name('default')

    if command_template in (None, '%s'):
        argv = shlex.split(program_string)
        program_name = argv[0]

        try:
            program_obj = _find_program(program_name, env)
        except ValueError, ex:
            Params.fatal(str(ex))

        try:
            program_node = program_obj.path.find_build(program_obj.get_target_name())
        except AttributeError:
            Params.fatal("%s does not appear to be a program" % (program_name,))

        execvec = [program_node.abspath(env)] + argv[1:]

    else:

        program_name = program_string
        try:
            program_obj = _find_program(program_name, env)
        except ValueError, ex:
            Params.fatal(str(ex))
        try:
            program_node = program_obj.path.find_build(program_obj.get_target_name())
        except AttributeError:
            Params.fatal("%s does not appear to be a program" % (program_name,))

        execvec = shlex.split(command_template % (program_node.abspath(env),))

    former_cwd = os.getcwd()
    if (Params.g_options.cwd_launch):
        os.chdir(Params.g_options.cwd_launch)
    else:
        os.chdir(Params.g_cwd_launch)
    try:
        retval = _run_argv(execvec)
    finally:
        os.chdir(former_cwd)

    return retval

def check_shell():
    if 'NS3_MODULE_PATH' not in os.environ:
        return
    env = Params.g_build.env_of_name('default')
    correct_modpath = os.pathsep.join(env['NS3_MODULE_PATH'])
    found_modpath = os.environ['NS3_MODULE_PATH']
    if found_modpath != correct_modpath:
        msg = ("Detected shell (waf --shell) with incorrect configuration\n"
               "=========================================================\n"
               "Possible reasons for this problem:\n"
               "  1. You switched to another ns-3 tree from inside this shell\n"
               "  2. You switched ns-3 debug level (waf configure --debug)\n"
               "  3. You modified the list of built ns-3 modules\n"
               "You should correct this situation before running any program.  Possible solutions:\n"
               "  1. Exit this shell, and start a new one\n"
               "  2. Run a new nested shell")
        Params.fatal(msg)


def run_shell():
    if sys.platform == 'win32':
        shell = os.environ.get("COMSPEC", "cmd.exe")
    else:
        shell = os.environ.get("SHELL", "/bin/sh")

    env = Params.g_build.env_of_name('default')
    _run_argv([shell], {'NS3_MODULE_PATH': os.pathsep.join(env['NS3_MODULE_PATH'])})

def doxygen():
    if not os.path.exists('doc/introspected-doxygen.h'):
        Params.warning("doc/introspected-doxygen.h does not exist; run waf check to generate it.")

    ## run doxygen
    doxygen_config = os.path.join('doc', 'doxygen.conf')
    if subprocess.Popen(['doxygen', doxygen_config]).wait():
        raise SystemExit(1)

def lcov_report():
    env = Params.g_build.env_of_name('default')
    variant_name = env['NS3_ACTIVE_VARIANT']

    if 'gcov' not in variant_name:
        Params.fatal("project not configured for code coverage;"
                     " reconfigure with --enable-gcov")

    os.chdir(blddir)
    try:
        lcov_report_dir = os.path.join(variant_name, 'lcov-report')
        create_dir_command = "rm -rf " + lcov_report_dir
        create_dir_command += " && mkdir " + lcov_report_dir + ";"

        if subprocess.Popen(create_dir_command, shell=True).wait():
            raise SystemExit(1)

        info_file = os.path.join(lcov_report_dir, variant_name + '.info')
        lcov_command = "../utils/lcov/lcov -c -d . -o " + info_file
        lcov_command += " --source-dirs=" + os.getcwd()
        lcov_command += ":" + os.path.join(
            os.getcwd(), variant_name, 'include')
        if subprocess.Popen(lcov_command, shell=True).wait():
            raise SystemExit(1)

        genhtml_command = "../utils/lcov/genhtml -o " + lcov_report_dir
        genhtml_command += " " + info_file
        if subprocess.Popen(genhtml_command, shell=True).wait():
            raise SystemExit(1)
    finally:
        os.chdir("..")




##
## The default WAF DistDir implementation is rather slow, because it
## first copies everything and only later removes unwanted files and
## directories; this means that it needless copies the full build dir
## and the .hg repository tree.  Here we provide a replacement DistDir
## implementation that is more efficient.
##
import Scripting
from Scripting import g_dist_exts, g_excludes, BLDDIR
import Utils
import os

def copytree(src, dst, symlinks=False, excludes=(), build_dir=None):
    """Recursively copy a directory tree using copy2().

    The destination directory must not already exist.
    If exception(s) occur, an Error is raised with a list of reasons.

    If the optional symlinks flag is true, symbolic links in the
    source tree result in symbolic links in the destination tree; if
    it is false, the contents of the files pointed to by symbolic
    links are copied.

    XXX Consider this example code rather than the ultimate tool.

    Note: this is a modified version of shutil.copytree from python
    2.5.2 library; modified for WAF purposes to exclude dot dirs and
    another list of files.
    """
    names = os.listdir(src)
    os.makedirs(dst)
    errors = []
    for name in names:
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)
        try:
            if symlinks and os.path.islink(srcname):
                linkto = os.readlink(srcname)
                os.symlink(linkto, dstname)
            elif os.path.isdir(srcname):
                if name in excludes:
                    continue
                elif name.startswith('.') or name.startswith(',,') or name.startswith('++'):
                    continue
                elif name == build_dir:
                    continue
                else:
                    ## build_dir is not passed into the recursive
                    ## copytree, but that is intentional; it is a
                    ## directory name valid only at the top level.
                    copytree(srcname, dstname, symlinks, excludes)
            else:
                ends = name.endswith
                to_remove = False
                if name.startswith('.') or name.startswith('++'):
                    to_remove = True
                else:
                    for x in g_dist_exts:
                        if ends(x):
                            to_remove = True
                            break
                if not to_remove:
                    shutil.copy2(srcname, dstname)
            # XXX What about devices, sockets etc.?
        except (IOError, os.error), why:
            errors.append((srcname, dstname, str(why)))
        # catch the Error from the recursive copytree so that we can
        # continue with other files
        except shutil.Error, err:
            errors.extend(err.args[0])
    try:
        shutil.copystat(src, dst)
    except WindowsError:
        # can't copy file access times on Windows
        pass
    except OSError, why:
        errors.extend((src, dst, str(why)))
    if errors:
        raise shutil.Error, errors


def DistDir(appname, version):
    "make a distribution directory with all the sources in it"
    import shutil

    # Our temporary folder where to put our files
    TMPFOLDER=appname+'-'+version

    # Remove an old package directory
    if os.path.exists(TMPFOLDER): shutil.rmtree(TMPFOLDER)

    global g_dist_exts, g_excludes

    # Remove the Build dir
    build_dir = getattr(Utils.g_module, BLDDIR, None)

    # Copy everything into the new folder
    copytree('.', TMPFOLDER, excludes=g_excludes, build_dir=build_dir)

    # TODO undocumented hook
    dist_hook = getattr(Utils.g_module, 'dist_hook', None)
    if dist_hook:
        os.chdir(TMPFOLDER)
        try:
            dist_hook()
        finally:
            # go back to the root directory
            os.chdir('..')
    return TMPFOLDER

Scripting.DistDir = DistDir




### Regression testing
class Regression(object):
    def __init__(self, testdir):
        self.testdir = testdir

    def run_test(self, verbose, generate, refDirName, testName):
        refTestDirName = os.path.join(refDirName, (testName + ".ref"))

        if not os.path.exists(refDirName):
            print"No reference trace repository"
            return 1

        if generate:
            if not os.path.exists(refTestDirName):
                print "creating new " + refTestDirName
                os.mkdir(refTestDirName)

            #os.system("./waf --cwd regression/" + refTestDirName +
            #    " --run " + testName + " > /dev/null 2>&1")
            Params.g_options.cwd_launch = refTestDirName
            run_program(testName)

            print "Remember to commit " + refTestDirName
            return 0
        else:
            if not os.path.exists(refTestDirName):
                print "Cannot locate reference traces"
                return 1

            shutil.rmtree("traces");
            os.mkdir("traces")

            #os.system("./waf --cwd regression/traces --run " +
            #  testName + " > /dev/null 2>&1")
            Params.g_options.cwd_launch = "traces"
            run_program(testName, command_template=get_command_template())

            if verbose:
                diffCmd = "diff traces " + refTestDirName + " | head"
            else:
                diffCmd = "diff traces " + refTestDirName + \
                    " > /dev/null 2>&1"

            rc = os.system(diffCmd)
            if rc:
                print "----------"
                print "Traces differ in test: test-" + testName
                print "Reference traces in directory: " + refTestDirName
                print "Traces in directory: traces"
                print "Rerun regression test as: " + \
                    "\"python regression.py test-" + testName + "\""
                print "Then do \"diff -u traces " + refTestDirName + \
                    "\" for details"
                print "----------"
            return rc

def _find_tests(testdir):
    """Return a list of test modules in the test directory

    Arguments:
    testdir -- the directory to look in for tests
    """
    names = os.listdir(testdir)
    tests = []
    for name in names:
        if name[:5] == "test-" and name[-3:] == ".py":
            testname = name[:-3]
            tests.append(testname)
    tests.sort()
    return tests

def run_regression():
    """Execute regression tests."""

    testdir = "tests"
    if not os.path.exists(testdir):
        print "Tests directory does not exist"
        sys.exit(3)
    
    sys.path.append(testdir)
    sys.modules['tracediff'] = Regression(testdir)

    if Params.g_options.regression_tests:
        tests = Params.g_options.regression_tests.split(',')
    else:
        tests = _find_tests(testdir)

    print "========== Running Regression Tests =========="

    if os.system("hg version > /dev/null 2>&1") == 0:
        print "Synchronizing reference traces using Mercurial."
        if not os.path.exists(REGRESSION_TRACES_DIR_NAME):
            os.system("hg clone " + REGRESSION_TRACES_REPO + REGRESSION_TRACES_DIR_NAME + " > /dev/null 2>&1")
        else:
            _dir = os.getcwd()
            os.chdir(REGRESSION_TRACES_DIR_NAME)
            try:
                os.system("hg pull " + REGRESSION_TRACES_REPO + REGRESSION_TRACES_DIR_NAME + " > /dev/null 2>&1")
            finally:
                os.chdir("..")
    else:
        print "Synchronizing reference traces from web."
        urllib.urlretrieve(REGRESSION_TRACES_URL + REGRESSION_TRACES_TAR_NAME, REGRESSION_TRACES_TAR_NAME)
        os.system("tar -xjf %s" % (REGRESSION_TRACES_TAR_NAME,))

    print "Done."

    if not os.path.exists(REGRESSION_TRACES_DIR_NAME):
        print "Reference traces directory does not exist"
        return 3
    
    bad = []

    for test in tests:
        result = _run_regression_test(test)
        if result == 0:
            if Params.g_options.regression_generate:
                print "GENERATE " + test
            else:
                print "PASS " + test
        else:
            bad.append(test)
            print "FAIL " + test

    return len(bad) > 0


def _run_regression_test(test):
    """Run a single test.

    Arguments:
    test -- the name of the test
    """
    if os.path.exists("traces"):
        files = os.listdir("traces")
        for file in files:
            if file == '.' or file == '..':
                continue
            path = "traces" + os.sep + file
            os.remove(path)
    else:
        os.mkdir("traces")
    
    mod = __import__(test, globals(), locals(), [])
    return mod.run(verbose=(Params.g_options.verbose > 0),
                   generate=Params.g_options.regression_generate,
                   refDirName=REGRESSION_TRACES_DIR_NAME)
