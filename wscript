## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
import sys
import shutil
import types
import optparse
import os.path

import pproc as subprocess

import Params
import Object
import ccroot
import Task

import wutils
import regression

Params.g_autoconfig = 1

# the following two variables are used by the target "waf dist"
VERSION = file("VERSION").read().strip()
APPNAME = 'ns'

wutils.VERSION = VERSION
wutils.APPNAME = APPNAME

# these variables are mandatory ('/' are converted automatically)
srcdir = '.'
blddir = 'build'


def dist_hook():
    import tarfile
    shutil.rmtree("doc/html", True)
    shutil.rmtree("doc/latex", True)
    shutil.rmtree("nsc", True)

    if not os.path.exists("bindings/python/pybindgen"):
        Params.fatal("Missing pybindgen checkout; run './waf configure --pybindgen-checkout' first.")

    ## build the name of the traces subdirectory.  Will be something like
    ## ns-3-dev-ref-traces
    traces_name = APPNAME + '-' + VERSION + regression.REGRESSION_SUFFIX
    ## Create a tar.bz2 file with the traces
    traces_dir = os.path.join(regression.REGRESSION_DIR, traces_name)
    if not os.path.isdir(traces_dir):
        Params.warning("Not creating traces archive: the %s directory does not exist" % traces_dir)
    else:
        traceball = traces_name + wutils.TRACEBALL_SUFFIX
        tar = tarfile.open(os.path.join("..", traceball), 'w:bz2')
        tar.add(traces_dir)
        tar.close()
        ## Now remove it; we do not ship the traces with the main tarball...
        shutil.rmtree(traces_dir, True)

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
    opt.add_option('--pyrun',
                   help=('Run a python program using locally built ns3 python module;'
                         ' argument is the path to the python program, optionally followed'
                         ' by command-line options that are passed to the program.'),
                   type="string", default='', dest='pyrun')
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
                   help=("Generate new regression test traces."),
                   default=False, dest='regression_generate', action="store_true")
    opt.add_option('--regression-tests',
                   help=('For regression testing, only run/generate the indicated regression tests, '
                         'specified as a comma separated list of test names'),
                   dest='regression_tests', type="string")
    opt.add_option('--enable-sudo',
                   help=('Use sudo to setup suid bits on ns3 executables.'),
                   dest='enable_sudo', action='store_true',
                   default=False)
    opt.add_option('--with-regression-traces',
                   help=('Path to the regression reference traces directory'),
                   default=None,
                   dest='regression_traces', type="string")

    # options provided in a script in a subdirectory named "src"
    opt.sub_options('src')
    opt.sub_options('bindings/python')
    opt.sub_options('src/internet-stack')


def check_compilation_flag(conf, flag):
    """
    Checks if the C++ compiler accepts a certain compilation flag or flags
    flag: can be a string or a list of strings
    """

    # Check for -Wno-error=deprecated-declarations
    save_CXXFLAGS = list(conf.env['CXXFLAGS'])
    conf.env.append_value('CXXFLAGS', flag)
    e = conf.create_test_configurator()
    e.mandatory = 0
    e.code = '#include <stdio.h>\nint main() { return 0; }\n'
    e.want_message = 0
    ok = e.run()
    conf.check_message_custom(flag, 'compilation flag support',
                              (ok and 'yes' or 'no'))

    if not ok: # if it doesn't accept, remove it again
        conf.env['CXXFLAGS'] = save_CXXFLAGS
    
def report_optional_feature(conf, name, caption, was_enabled, reason_not_enabled):
    conf.env.append_value('NS3_OPTIONAL_FEATURES', (name, caption, was_enabled, reason_not_enabled))

def configure(conf):
    
    # attach some extra methods
    conf.check_compilation_flag = types.MethodType(check_compilation_flag, conf)
    conf.report_optional_feature = types.MethodType(report_optional_feature, conf)
    conf.env['NS3_OPTIONAL_FEATURES'] = []

    conf.env['NS3_BUILDDIR'] = conf.m_blddir
    conf.check_tool('compiler_cxx')

    # create the second environment, set the variant and set its name
    variant_env = conf.env.copy()
    debug_level = Params.g_options.debug_level.lower()
    if debug_level == 'ultradebug':
        variant_name = 'debug'
    else:
        variant_name = debug_level

    variant_env['INCLUDEDIR'] = os.path.join(variant_env['PREFIX'], 'include')

    if Params.g_options.regression_traces is not None:
        variant_env['REGRESSION_TRACES'] = os.path.join("..", Params.g_options.regression_traces)
    else:
        variant_env['REGRESSION_TRACES'] = None

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

        variant_env.append_value('CXXFLAGS', '-Werror')

        check_compilation_flag(conf, '-Wno-error=deprecated-declarations')

        
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
    conf.sub_config('bindings/python')

    if Params.g_options.enable_modules:
        conf.env['NS3_ENABLED_MODULES'] = ['ns3-'+mod for mod in
                                           Params.g_options.enable_modules.split(',')]

    # we cannot run regression tests without diff
    conf.find_program('diff', var='DIFF')

    # for suid bits
    conf.find_program('sudo', var='SUDO')

    # we cannot pull regression traces without mercurial
    conf.find_program('hg', var='MERCURIAL')

    # Write a summary of optional features status
    print "---- Summary of optional NS-3 features:"
    for (name, caption, was_enabled, reason_not_enabled) in conf.env['NS3_OPTIONAL_FEATURES']:
        if was_enabled:
            status = 'enabled'
        else:
            status = 'not enabled (%s)' % reason_not_enabled
        print "%-30s: %s" % (caption, status)


class SuidBuildTask(Task.TaskBase):
    """task that makes a binary Suid
    """
    def __init__(self, bld, program):
        self.m_display = 'build-suid'
        self.prio = 1000 # build after the rest of ns-3
        self.__program = program
        self.__env = bld.env ()
        super(SuidBuildTask, self).__init__()

    def run(self):
        try:
            program_obj = wutils.find_program(self.__program.target, self.__env)
        except ValueError, ex:
            Params.fatal(str(ex))

        try:
            program_node = program_obj.path.find_build(ccroot.get_target_name(program_obj))
        except AttributeError:
            Params.fatal("%s does not appear to be a program" % (self.__program.name,))

        filename = program_node.abspath(self.__env)
        os.system ('sudo chown root ' + filename)
        os.system ('sudo chmod u+s ' + filename)

def create_suid_program(bld, name):
    program = bld.create_obj('cpp', 'program')
    program.is_ns3_program = True
    program.module_deps = list()
    program.name = name
    program.target = name
    if bld.env ()['SUDO'] and Params.g_options.enable_sudo:
        SuidBuildTask (bld, program)
    return program

def create_ns3_program(bld, name, dependencies=('simulator',)):
    program = bld.create_obj('cpp', 'program')
    program.is_ns3_program = True
    program.name = name
    program.target = program.name
    program.uselib_local = 'ns3'
    program.ns3_module_dependencies = ['ns3-'+dep for dep in dependencies]
    return program

def add_scratch_programs(bld):
    all_modules = [mod[len("ns3-"):] for mod in bld.env()['NS3_MODULES']]
    for filename in os.listdir("scratch"):
        if filename.startswith('.') or filename == 'CVS':
	    continue
        if os.path.isdir(os.path.join("scratch", filename)):
            obj = bld.create_ns3_program(filename, all_modules)
            obj.path = obj.path.find_dir('scratch')
            obj.find_sources_in_dirs(filename)
            obj.target = os.path.join(filename, filename)
            obj.name = obj.target
        elif filename.endswith(".cc"):
            name = filename[:-len(".cc")]
            obj = bld.create_ns3_program(name, all_modules)
            obj.source = "scratch/%s" % filename
            obj.target = "scratch/%s" % name
            obj.name = obj.target


##
## This replacement spawn function increases the maximum command line length to 32k
##
def _exec_command_interact_win32(s):
    if Params.g_verbose:
        print s
    startupinfo = subprocess.STARTUPINFO()
    startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
    proc = subprocess.Popen(s, shell=False, startupinfo=startupinfo)
    stat = proc.wait()
    if stat & 0xff:
        return stat | 0x80
    return stat >> 8


def build(bld):
    if Params.g_options.no_task_lines:
        import Runner
        def null_printout(s):
            pass
        Runner.printout = null_printout

    if sys.platform == 'win32':
        import Runner
        Runner.exec_command = _exec_command_interact_win32

    Params.g_cwd_launch = Params.g_build.m_curdirnode.abspath()
    bld.create_ns3_program = types.MethodType(create_ns3_program, bld)
    bld.create_suid_program = types.MethodType(create_suid_program, bld)
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

    print "Entering directory `%s'" % os.path.join(Params.g_build.m_curdirnode.abspath(), 'build')
    # process subfolders from here
    bld.add_subdirs('src')
    bld.add_subdirs('samples utils examples')

    add_scratch_programs(bld)

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
        env['NS3_ENABLED_MODULES'] = list(modules)
        lib.uselib_local = list(modules)
    else:
        lib.add_objects = list(env['NS3_MODULES'])
        lib.uselib_local = list(env['NS3_MODULES'])

    bld.add_subdirs('bindings/python')

    if Params.g_options.run:
        # Check that the requested program name is valid
        program_name, dummy_program_argv = wutils.get_run_program(Params.g_options.run, get_command_template())

        # When --run'ing a program, tell WAF to only build that program,
        # nothing more; this greatly speeds up compilation when all you
        # want to do is run a test program.
        if not Params.g_options.compile_targets:
            Params.g_options.compile_targets = program_name



def get_command_template(*arguments):
    if Params.g_options.valgrind:
        if Params.g_options.command_template:
            Params.fatal("Options --command-template and --valgrind are conflicting")
        cmd = "valgrind --leak-check=full %s"
    else:
        cmd = Params.g_options.command_template or '%s'
    for arg in arguments:
        cmd = cmd + " " + arg
    return cmd


def shutdown():
    #import UnitTest
    #ut = UnitTest.unit_test()
    #ut.change_to_testfile_dir = True
    #ut.want_to_see_test_output = True
    #ut.want_to_see_test_error = True
    #ut.run()
    #ut.print_results()
    env = Params.g_build.env_of_name('default')

    if Params.g_commands['check']:
        _run_waf_check()

    if Params.g_options.regression or Params.g_options.regression_generate:
        if not env['DIFF']:
            Params.fatal("Cannot run regression tests: the 'diff' program is not installed.")

        _dir = os.getcwd()
        os.chdir("regression")
        regression_traces = env['REGRESSION_TRACES']
        if not regression_traces:
            regression_traces = None
        try:
            retval = regression.run_regression(regression_traces)
        finally:
            os.chdir(_dir)
        if retval:
            sys.exit(retval)

    if Params.g_options.lcov_report:
        lcov_report()

    if Params.g_options.run:
        wutils.run_program(Params.g_options.run, get_command_template())
        raise SystemExit(0)

    if Params.g_options.pyrun:
        wutils.run_python_program(Params.g_options.pyrun)
        raise SystemExit(0)

def _run_waf_check():
    ## generate the trace sources list docs
    env = Params.g_build.env_of_name('default')
    proc_env = wutils.get_proc_env()
    try:
        program_obj = wutils.find_program('print-introspected-doxygen', env)
    except ValueError: # could happen if print-introspected-doxygen is
                       # not built because of waf configure
                       # --enable-modules=xxx
        pass
    else:
        prog = program_obj.path.find_build(ccroot.get_target_name(program_obj)).abspath(env)
        out = open(os.path.join('doc', 'introspected-doxygen.h'), 'w')
        if subprocess.Popen([prog], stdout=out, env=proc_env).wait():
            raise SystemExit(1)
        out.close()

    print "-- Running NS-3 C++ core unit tests..."
    wutils.run_program('run-tests', get_command_template())

    if env['ENABLE_PYTHON_BINDINGS']:
        print "-- Running NS-3 Python bindings unit tests..."
        wutils.run_argv([env['PYTHON'], os.path.join("utils", "python-unit-tests.py")], proc_env)
    else:
        print "-- Skipping NS-3 Python bindings unit tests: Python bindings not enabled."




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
    wutils.run_argv([shell], {'NS3_MODULE_PATH': os.pathsep.join(env['NS3_MODULE_PATH'])})

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
                elif name.startswith('.') or name.startswith(',,') or name.startswith('++') or name.startswith('CVS'):
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


