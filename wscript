## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# python lib modules
import sys
import shutil
import types
import optparse
import os.path

# WAF modules
import pproc as subprocess
import Options
import Logs
import TaskGen

import ccroot
ccroot.USE_TOP_LEVEL = True

import Task
import Utils
import Build
import Configure

import cflags # override the build profiles from waf
cflags.profiles = {
	# profile name: [optimization_level, warnings_level, debug_level]
	'debug':     [0, 2, 3],
	'optimized': [3, 2, 1],
	}
cflags.default_profile = 'debug'

# local modules
import wutils
import regression

Configure.autoconfig = 1

# the following two variables are used by the target "waf dist"
VERSION = file("VERSION", "rt").read().strip()
APPNAME = 'ns'

wutils.VERSION = VERSION
wutils.APPNAME = APPNAME

#
# The last part of the path name to use to find the regression traces.  The
# path will be APPNAME + '-' + VERSION + REGRESSION_SUFFIX, e.g.,
# ns-3-dev-ref-traces
#
REGRESSION_SUFFIX = "-ref-traces"


# these variables are mandatory ('/' are converted automatically)
srcdir = '.'
blddir = 'build'


def dist_hook():
    import tarfile
    shutil.rmtree("doc/html", True)
    shutil.rmtree("doc/latex", True)
    shutil.rmtree("nsc", True)

    ## build the name of the traces subdirectory.  Will be something like
    ## ns-3-dev-ref-traces
    traces_name = APPNAME + '-' + VERSION + regression.REGRESSION_SUFFIX
    ## Create a tar.bz2 file with the traces
    traces_dir = os.path.join(regression.REGRESSION_DIR, traces_name)
    if not os.path.isdir(traces_dir):
        Logs.warn("Not creating traces archive: the %s directory does not exist" % traces_dir)
    else:
        traceball = traces_name + wutils.TRACEBALL_SUFFIX
        tar = tarfile.open(os.path.join("..", traceball), 'w:bz2')
        tar.add(traces_dir)
        tar.close()
        ## Now remove it; we do not ship the traces with the main tarball...
        shutil.rmtree(traces_dir, True)

def set_options(opt):
    # options provided by the modules
    opt.tool_options('compiler_cxx')
    opt.tool_options('cflags')

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
    opt.add_option('--enable-sudo',
                   help=('Use sudo to setup suid bits on ns3 executables.'),
                   dest='enable_sudo', action='store_true',
                   default=False)
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

    env = conf.env.copy()
    env.append_value('CXXFLAGS', flag)
    try:
        retval = conf.run_c_code(code='#include <stdio.h>\nint main() { return 0; }\n',
                                 env=env, compile_filename='test.cc',
                                 compile_mode='cxx',type='program', execute=False)
    except Configure.ConfigurationError:
        ok = False
    else:
        ok = (retval == 0)
    conf.check_message_custom(flag, 'support', (ok and 'yes' or 'no'))
    return ok

    
def report_optional_feature(conf, name, caption, was_enabled, reason_not_enabled):
    conf.env.append_value('NS3_OPTIONAL_FEATURES', (name, caption, was_enabled, reason_not_enabled))

def configure(conf):
    # attach some extra methods
    conf.check_compilation_flag = types.MethodType(check_compilation_flag, conf)
    conf.report_optional_feature = types.MethodType(report_optional_feature, conf)
    conf.env['NS3_OPTIONAL_FEATURES'] = []

    conf.env['NS3_BUILDDIR'] = conf.blddir
    conf.check_tool('compiler_cxx')
    conf.check_tool('cflags')
    try:
        conf.check_tool('pkgconfig')
    except Configure.ConfigurationError:
        pass
    conf.check_tool('command')

    # create the second environment, set the variant and set its name
    variant_env = conf.env.copy()
    variant_name = Options.options.build_profile

    # Check for the location of regression reference traces
    if Options.options.regression_traces is not None:
        if os.path.isdir(Options.options.regression_traces):
            conf.check_message("regression traces location", '', True, ("%s (given)" % Options.options.regression_traces))
            variant_env['REGRESSION_TRACES'] = os.path.abspath(Options.options.regression_traces)
    else:
        traces = os.path.join('..', "%s-%s%s" % (APPNAME, VERSION, REGRESSION_SUFFIX))
        if os.path.isdir(traces):
            conf.check_message("regression reference traces", '', True, ("%s (guessed)" % traces))
            variant_env['REGRESSION_TRACES'] = os.path.abspath(traces)
        del traces
    if not variant_env['REGRESSION_TRACES']:
        conf.check_message("regression reference traces", '', False)

    if Options.options.enable_gcov:
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
    env = variant_env

    env.append_value('CXXDEFINES', 'RUN_SELF_TESTS')
    
    if env['COMPILER_CXX'] == 'g++' and 'CXXFLAGS' not in os.environ:
        if check_compilation_flag(conf, '-Wno-error=deprecated-declarations'):
            env.append_value('CXXFLAGS', '-Wno-error=deprecated-declarations')
        
    if Options.options.build_profile == 'debug':
        env.append_value('CXXDEFINES', 'NS3_ASSERT_ENABLE')
        env.append_value('CXXDEFINES', 'NS3_LOG_ENABLE')

    if sys.platform == 'win32':
        if env['COMPILER_CXX'] == 'g++':
            env.append_value("LINKFLAGS", "-Wl,--enable-runtime-pseudo-reloc")

    conf.sub_config('src')
    conf.sub_config('utils')
    conf.sub_config('bindings/python')

    if Options.options.enable_modules:
        conf.env['NS3_ENABLED_MODULES'] = ['ns3-'+mod for mod in
                                           Options.options.enable_modules.split(',')]

    # we cannot run regression tests without diff
    conf.find_program('diff', var='DIFF')

    # for suid bits
    conf.find_program('sudo', var='SUDO')

    why_not_sudo = "because we like it"
    if Options.options.enable_sudo and conf.env['SUDO']:
        env['ENABLE_SUDO'] = True
    else:
        env['ENABLE_SUDO'] = False
        if Options.options.enable_sudo:
            why_not_sudo = "program sudo not found"
        else:
            why_not_sudo = "option --enable-sudo not selected"

    conf.report_optional_feature("ENABLE_SUDO", "Use sudo to set suid bit", env['ENABLE_SUDO'], why_not_sudo)

    # we cannot pull regression traces without mercurial
    conf.find_program('hg', var='MERCURIAL')

    conf.find_program('valgrind', var='VALGRIND')

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
    after = 'link'
    def __init__(self, bld, program):
        self.m_display = 'build-suid'
        self.__program = program
        self.__env = bld.env.copy ()
        super(SuidBuildTask, self).__init__()

    def run(self):
        try:
            program_obj = wutils.find_program(self.__program.target, self.__env)
        except ValueError, ex:
            raise Utils.WafError(str(ex))

        program_node = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj))
        #try:
        #    program_node = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj))
        #except AttributeError:
        #    raise Utils.WafError("%s does not appear to be a program" % (self.__program.name,))

        filename = program_node.abspath(self.__env)
        print 'setting suid bit on executable ' + filename

        if subprocess.Popen('sudo chown root ' + filename, shell=True).wait():
            raise SystemExit(1)
        if subprocess.Popen('sudo chmod u+s ' + filename, shell=True).wait():
            raise SystemExit(1)

def create_suid_program(bld, name):
    program = bld.new_task_gen('cxx', 'program')
    program.is_ns3_program = True
    program.module_deps = list()
    program.name = name
    program.target = name

    if bld.env['ENABLE_SUDO']:
        SuidBuildTask(bld, program)

    return program

def create_ns3_program(bld, name, dependencies=('simulator',)):
    program = bld.new_task_gen('cxx', 'program')
    program.is_ns3_program = True
    program.name = name
    program.target = program.name
    program.uselib_local = 'ns3'
    program.ns3_module_dependencies = ['ns3-'+dep for dep in dependencies]
    return program

def add_scratch_programs(bld):
    all_modules = [mod[len("ns3-"):] for mod in bld.env['NS3_MODULES']]
    for filename in os.listdir("scratch"):
        if filename.startswith('.') or filename == 'CVS':
	    continue
        if os.path.isdir(os.path.join("scratch", filename)):
            obj = bld.create_ns3_program(filename, all_modules)
            obj.path = obj.path.find_dir('scratch').find_dir(filename)
            obj.find_sources_in_dirs('.')
            obj.target = filename
            obj.name = obj.target
        elif filename.endswith(".cc"):
            name = filename[:-len(".cc")]
            obj = bld.create_ns3_program(name, all_modules)
            obj.path = obj.path.find_dir('scratch')
            obj.source = filename
            obj.target = name
            obj.name = obj.target


def build(bld):
    if Options.options.no_task_lines:
        import Runner
        def null_printout(s):
            pass
        Runner.printout = null_printout

    Options.cwd_launch = bld.path.abspath()
    bld.create_ns3_program = types.MethodType(create_ns3_program, bld)
    bld.create_suid_program = types.MethodType(create_suid_program, bld)

    # switch default variant to the one matching our debug level
    variant_name = bld.env_of_name('default')['NS3_ACTIVE_VARIANT']
    variant_env = bld.env_of_name(variant_name)
    bld.all_envs['default'] = variant_env

    if Options.options.shell:
        run_shell()
        raise SystemExit(0)

    if Options.options.doxygen:
        doxygen()
        raise SystemExit(0)

    check_shell()

    if Options.options.doxygen:
        doxygen()
        raise SystemExit(0)

    print "Entering directory `%s'" % os.path.join(bld.path.abspath(), 'build')
    # process subfolders from here
    bld.add_subdirs('src')
    bld.add_subdirs('samples utils examples')

    add_scratch_programs(bld)

    ## if --enabled-modules option was given, we disable building the
    ## modules that were not enabled, and programs that depend on
    ## disabled modules.
    env = bld.env

    if Options.options.enable_modules:
        Logs.warn("the option --enable-modules is being applied to this build only;"
                       " to make it permanent it needs to be given to waf configure.")
        env['NS3_ENABLED_MODULES'] = ['ns3-'+mod for mod in
                                      Options.options.enable_modules.split(',')]

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
        for obj in list(Build.bld.all_task_gen):
            if hasattr(obj, 'ns3_module_dependencies'):
                for dep in obj.ns3_module_dependencies:
                    if dep not in modules:
                        Build.bld.all_task_gen.remove(obj)
                        break
            if obj.name in env['NS3_MODULES'] and obj.name not in modules:
                Build.bld.all_task_gen.remove(obj)

    ## Create a single ns3 library containing all enabled modules
    lib = bld.new_task_gen('cxx', 'shlib')
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

    if Options.options.run:
        # Check that the requested program name is valid
        program_name, dummy_program_argv = wutils.get_run_program(Options.options.run, wutils.get_command_template(env))

        # When --run'ing a program, tell WAF to only build that program,
        # nothing more; this greatly speeds up compilation when all you
        # want to do is run a test program.
        if not Options.options.compile_targets:
            Options.options.compile_targets = os.path.basename(program_name)

    if Options.options.regression or Options.options.regression_generate:
        if not env['DIFF']:
            raise Utils.WafError("Cannot run regression tests: the 'diff' program is not installed.")

        regression_traces = env['REGRESSION_TRACES']
        if not regression_traces:
            raise Utils.WafError("Cannot run regression tests: reference traces directory not given"
                                 " (--with-regression-traces configure option)")
        regression.run_regression(bld, regression_traces)


def shutdown():
    env = Build.bld.env

    if Options.commands['check']:
        _run_waf_check()

    if Options.options.lcov_report:
        lcov_report()

    if Options.options.run:
        wutils.run_program(Options.options.run, wutils.get_command_template(env))
        raise SystemExit(0)

    if Options.options.pyrun:
        wutils.run_python_program(Options.options.pyrun)
        raise SystemExit(0)

def _run_waf_check():
    ## generate the trace sources list docs
    env = Build.bld.env
    proc_env = wutils.get_proc_env()
    try:
        program_obj = wutils.find_program('print-introspected-doxygen', env)
    except ValueError: # could happen if print-introspected-doxygen is
                       # not built because of waf configure
                       # --enable-modules=xxx
        pass
    else:
        prog = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj)).abspath(env)
        out = open(os.path.join('doc', 'introspected-doxygen.h'), 'w')
        if subprocess.Popen([prog], stdout=out, env=proc_env).wait():
            raise SystemExit(1)
        out.close()

    print "-- Running NS-3 C++ core unit tests..."
    wutils.run_program('run-tests', wutils.get_command_template(env))

    if env['ENABLE_PYTHON_BINDINGS']:
        print "-- Running NS-3 Python bindings unit tests..."
        wutils.run_argv([env['PYTHON'], os.path.join("utils", "python-unit-tests.py")], proc_env)
    else:
        print "-- Skipping NS-3 Python bindings unit tests: Python bindings not enabled."




def check_shell():
    if 'NS3_MODULE_PATH' not in os.environ:
        return
    env = Build.bld.env
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
        raise Utils.WafError(msg)


def run_shell():
    if sys.platform == 'win32':
        shell = os.environ.get("COMSPEC", "cmd.exe")
    else:
        shell = os.environ.get("SHELL", "/bin/sh")

    env = Build.bld.env
    wutils.run_argv([shell], {'NS3_MODULE_PATH': os.pathsep.join(env['NS3_MODULE_PATH'])})

def doxygen():
    if not os.path.exists('doc/introspected-doxygen.h'):
        Logs.warn("doc/introspected-doxygen.h does not exist; run waf check to generate it.")

    ## run doxygen
    doxygen_config = os.path.join('doc', 'doxygen.conf')
    if subprocess.Popen(['doxygen', doxygen_config]).wait():
        raise SystemExit(1)

def lcov_report():
    env = Build.bld.env
    variant_name = env['NS3_ACTIVE_VARIANT']

    if 'gcov' not in variant_name:
        raise Utils.WafError("project not configured for code coverage;"
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
from Scripting import dist_exts, excludes, BLDDIR
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
                    for x in dist_exts:
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
    copytree('.', TMPFOLDER, excludes=excludes, build_dir=build_dir)

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


