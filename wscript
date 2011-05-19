## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# python lib modules
import sys
import shutil
import types
import optparse
import os.path
import re
import shlex
import textwrap

# WAF modules
import pproc as subprocess
import Options

import Logs
import TaskGen
import Constants

import ccroot
ccroot.USE_TOP_LEVEL = True

import Task
Task.algotype = Constants.JOBCONTROL # so that Task.maxjobs=1 takes effect

import Utils
import Build
import Configure
import Scripting

from utils import read_config_file

# By default, all modules will be enabled, examples will be disabled,
# and tests will be disabled.
modules_enabled  = ['all_modules']
examples_enabled = False
tests_enabled    = False

# Get the information out of the NS-3 configuration file.
config_file_exists = False
(config_file_exists, modules_enabled, examples_enabled, tests_enabled) = read_config_file()

sys.path.insert(0, os.path.abspath('waf-tools'))
try:
    import cflags # override the build profiles from waf
finally:
    sys.path.pop(0)

cflags.profiles = {
	# profile name: [optimization_level, warnings_level, debug_level]
	'debug':     [0, 2, 3],
	'optimized': [3, 2, 1],
	'release':   [3, 2, 0],
	}
cflags.default_profile = 'debug'

# local modules
import wutils

Configure.autoconfig = 1

# the following two variables are used by the target "waf dist"
VERSION = file("VERSION", "rt").read().strip()
APPNAME = 'ns'

wutils.VERSION = VERSION
wutils.APPNAME = APPNAME

# these variables are mandatory ('/' are converted automatically)
srcdir = '.'
blddir = 'build'

def load_env():
    bld_cls = getattr(Utils.g_module, 'build_context', Utils.Context)
    bld_ctx = bld_cls()
    bld_ctx.load_dirs(os.path.abspath(os.path.join (srcdir,'..')),
                      os.path.abspath(os.path.join (srcdir,'..', blddir)))
    bld_ctx.load_envs()
    env = bld_ctx.get_env()
    return env

def get_files(base_dir):
    retval = []
    reference=os.path.dirname(base_dir)
    for root, dirs, files in os.walk(base_dir):
        if root.find('.hg') != -1:
            continue
        for file in files:
            if file.find('.hg') != -1:
                continue
            fullname = os.path.join(root,file)
            # we can't use os.path.relpath because it's new in python 2.6
            relname = fullname.replace(reference + '/','')
            retval.append([fullname,relname])
    return retval


def dist_hook():
    import tarfile
    shutil.rmtree("doc/html", True)
    shutil.rmtree("doc/latex", True)
    shutil.rmtree("nsc", True)

def set_options(opt):
    # options provided by the modules
    opt.tool_options('compiler_cc')
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

    opt.add_option('--run',
                   help=('Run a locally built program; argument can be a program name,'
                         ' or a command starting with the program name.'),
                   type="string", default='', dest='run')
    opt.add_option('--visualize',
                   help=('Modify --run arguments to enable the visualizer'),
                   action="store_true", default=False, dest='visualize')
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
                   help=('DEPRECATED (run ./waf shell)'),
                   action="store_true", default=False,
                   dest='shell')
    opt.add_option('--enable-sudo',
                   help=('Use sudo to setup suid bits on ns3 executables.'),
                   dest='enable_sudo', action='store_true',
                   default=False)
    opt.add_option('--enable-tests',
                   help=('Build the ns-3 tests.'),
                   dest='enable_tests', action='store_true',
                   default=False)
    opt.add_option('--disable-tests',
                   help=('Do not build the ns-3 tests.'),
                   dest='disable_tests', action='store_true',
                   default=False)
    opt.add_option('--enable-examples',
                   help=('Build the ns-3 examples.'),
                   dest='enable_examples', action='store_true',
                   default=False)
    opt.add_option('--disable-examples',
                   help=('Do not build the ns-3 examples.'),
                   dest='disable_examples', action='store_true',
                   default=False)
    opt.add_option('--check',
                   help=('DEPRECATED (run ./test.py)'),
                   default=False, dest='check', action="store_true")
    opt.add_option('--enable-static',
                   help=('Compile NS-3 statically: works only on linux, without python'),
                   dest='enable_static', action='store_true',
                   default=False)
    opt.add_option('--enable-mpi',
                   help=('Compile NS-3 with MPI and distributed simulation support'),
                   dest='enable_mpi', action='store_true',
                   default=False)
    opt.add_option('--doxygen-no-build',
                   help=('Run doxygen to generate html documentation from source comments, '
                         'but do not wait for ns-3 to finish the full build.'),
                   action="store_true", default=False,
                   dest='doxygen_no_build')

    # options provided in subdirectories
    opt.sub_options('src')
    opt.sub_options('bindings/python')
    opt.sub_options('src/internet')


def _check_compilation_flag(conf, flag, mode='cxx'):
    """
    Checks if the C++ compiler accepts a certain compilation flag or flags
    flag: can be a string or a list of strings
    """

    env = conf.env.copy()
    if mode == 'cxx':
        fname = 'test.cc'
        env.append_value('CXXFLAGS', flag)
    else:
        fname = 'test.c'
        env.append_value('CCFLAGS', flag)
    try:
        retval = conf.run_c_code(code='#include <stdio.h>\nint main() { return 0; }\n',
                                 env=env, compile_filename=fname,
                                 compile_mode=mode, type='cprogram', execute=False)
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
    conf.check_compilation_flag = types.MethodType(_check_compilation_flag, conf)
    conf.report_optional_feature = types.MethodType(report_optional_feature, conf)
    conf.env['NS3_OPTIONAL_FEATURES'] = []

    conf.env['NS3_BUILDDIR'] = conf.blddir
    conf.check_tool('compiler_cc')
    conf.check_tool('compiler_cxx')
    conf.check_tool('cflags', ['waf-tools'])
    try:
        conf.check_tool('pkgconfig', ['waf-tools'])
    except Configure.ConfigurationError:
        pass
    conf.check_tool('command', ['waf-tools'])

    # create the second environment, set the variant and set its name
    variant_env = conf.env.copy()
    variant_name = Options.options.build_profile

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

    if Options.options.build_profile == 'debug':
        env.append_value('CXXDEFINES', 'NS3_ASSERT_ENABLE')
        env.append_value('CXXDEFINES', 'NS3_LOG_ENABLE')

    env['PLATFORM'] = sys.platform

    if conf.env['CXX_NAME'] in ['gcc', 'icc']:
        if Options.options.build_profile == 'release': 
            env.append_value('CXXFLAGS', '-fomit-frame-pointer') 
            if conf.check_compilation_flag('-march=native'):
                env.append_value('CXXFLAGS', '-march=native') 

        if sys.platform == 'win32':
            env.append_value("LINKFLAGS", "-Wl,--enable-runtime-pseudo-reloc")
        elif sys.platform == 'cygwin':
            env.append_value("LINKFLAGS", "-Wl,--enable-auto-import")

        cxx, = env['CXX']

        p = subprocess.Popen([cxx, '-print-file-name=libstdc++.so'], stdout=subprocess.PIPE)
        libstdcxx_location = os.path.dirname(p.stdout.read().strip())
        p.wait()
        if libstdcxx_location:
            conf.env.append_value('NS3_MODULE_PATH', libstdcxx_location)

        if Options.platform in ['linux']:
            if conf.check_compilation_flag('-Wl,--soname=foo'):
                env['WL_SONAME_SUPPORTED'] = True

    conf.sub_config('src')

    # Set the list of enabled modules.
    if Options.options.enable_modules:
        # Use the modules explicitly enabled. 
        conf.env['NS3_ENABLED_MODULES'] = ['ns3-'+mod for mod in
                                           Options.options.enable_modules.split(',')]
    else:
        # Use the enabled modules list from the ns3 configuration file.
        if modules_enabled[0] == 'all_modules':
            # Enable all modules if requested.
            conf.env['NS3_ENABLED_MODULES'] = conf.env['NS3_MODULES']
        else:
            # Enable the modules from the list.
            conf.env['NS3_ENABLED_MODULES'] = ['ns3-'+mod for mod in
                                               modules_enabled]
    conf.sub_config('bindings/python')

    conf.sub_config('src/mpi')

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

    # Decide if tests will be built or not.
    if Options.options.enable_tests:
        # Tests were explicitly enabled. 
        env['ENABLE_TESTS'] = True
        why_not_tests = "option --enable-tests selected"
    elif Options.options.disable_tests:
        # Tests were explicitly disabled. 
        env['ENABLE_TESTS'] = False
        why_not_tests = "option --disable-tests selected"
    else:
        # Enable tests based on the ns3 configuration file.
        env['ENABLE_TESTS'] = tests_enabled
        if config_file_exists:
            why_not_tests = "based on configuration file"
        elif tests_enabled:
            why_not_tests = "defaults to enabled"
        else:
            why_not_tests = "defaults to disabled"

    conf.report_optional_feature("ENABLE_TESTS", "Build tests", env['ENABLE_TESTS'], why_not_tests)

    # Decide if examples will be built or not.
    if Options.options.enable_examples:
        # Examples were explicitly enabled. 
        env['ENABLE_EXAMPLES'] = True
        why_not_examples = "option --enable-examples selected"
    elif Options.options.disable_examples:
        # Examples were explicitly disabled. 
        env['ENABLE_EXAMPLES'] = False
        why_not_examples = "option --disable-examples selected"
    else:
        # Enable examples based on the ns3 configuration file.
        env['ENABLE_EXAMPLES'] = examples_enabled
        if config_file_exists:
            why_not_examples = "based on configuration file"
        elif examples_enabled:
            why_not_examples = "defaults to enabled"
        else:
            why_not_examples = "defaults to disabled"

    env['EXAMPLE_DIRECTORIES'] = []
    for dir in os.listdir('examples'):
        if dir.startswith('.') or dir == 'CVS':
            continue
        if os.path.isdir(os.path.join('examples', dir)):
            env['EXAMPLE_DIRECTORIES'].append(dir)

    conf.report_optional_feature("ENABLE_EXAMPLES", "Build examples", env['ENABLE_EXAMPLES'], 
                                 why_not_examples)

    conf.find_program('valgrind', var='VALGRIND')

    env['ENABLE_STATIC_NS3'] = False
    if Options.options.enable_static:
        if env['PLATFORM'].startswith('linux') and \
                env['CXX_NAME'] in ['gcc', 'icc']:
            if re.match('i[3-6]86', os.uname()[4]):
                conf.report_optional_feature("static", "Static build", True, '')
                env['ENABLE_STATIC_NS3'] = True
            elif os.uname()[4] == 'x86_64':
                if env['ENABLE_PYTHON_BINDINGS'] and \
                        not conf.check_compilation_flag('-mcmodel=large'):
                    conf.report_optional_feature("static", "Static build", False,
                                                 "Can't enable static builds because " + \
                                                     "no -mcmodel=large compiler " \
                                                     "option. Try --disable-python or upgrade your " \
                                                     "compiler to at least gcc 4.3.x.")
                else:
                    conf.report_optional_feature("static", "Static build", True, '')
                    env['ENABLE_STATIC_NS3'] = True                    
        elif env['CXX_NAME'] == 'gcc' and \
                (env['PLATFORM'].startswith('darwin') or \
                     env['PLATFORM'].startswith('cygwin')):
                conf.report_optional_feature("static", "Static build", True, '')
                env['ENABLE_STATIC_NS3'] = True
        else:
            conf.report_optional_feature("static", "Static build", False,
                                         "Unsupported platform")
    else:
        conf.report_optional_feature("static", "Static build", False,
                                     "option --enable-static not selected")

    # These flags are used for the implicitly dependent modules.
    if env['ENABLE_STATIC_NS3']:
        if sys.platform == 'darwin':
            env.STATICLIB_MARKER = '-Wl,-all_load'
        else:
            env.STATICLIB_MARKER = '-Wl,--whole-archive,-Bstatic'
            env.SHLIB_MARKER = '-Wl,-Bdynamic,--no-whole-archive'

    have_gsl = conf.pkg_check_modules('GSL', 'gsl', mandatory=False)
    conf.env['ENABLE_GSL'] = have_gsl

    conf.report_optional_feature("GSL", "GNU Scientific Library (GSL)",
                                 conf.env['ENABLE_GSL'],
                                 "GSL not found")
    if have_gsl:
        conf.env.append_value('CXXDEFINES', "ENABLE_GSL")
        conf.env.append_value('CCDEFINES', "ENABLE_GSL")

    # for compiling C code, copy over the CXX* flags
    conf.env.append_value('CCFLAGS', conf.env['CXXFLAGS'])
    conf.env.append_value('CCDEFINES', conf.env['CXXDEFINES'])

    def add_gcc_flag(flag):
        if env['COMPILER_CXX'] == 'g++' and 'CXXFLAGS' not in os.environ:
            if conf.check_compilation_flag(flag, mode='cxx'):
                env.append_value('CXXFLAGS', flag)
        if env['COMPILER_CC'] == 'gcc' and 'CCFLAGS' not in os.environ:
            if conf.check_compilation_flag(flag, mode='cc'):
                env.append_value('CCFLAGS', flag)

    add_gcc_flag('-Wno-error=deprecated-declarations')
    add_gcc_flag('-fstrict-aliasing')
    add_gcc_flag('-Wstrict-aliasing')

    conf.find_program('doxygen', var='DOXYGEN')

    # append user defined flags after all our ones
    for (confvar, envvar) in [['CCFLAGS', 'CCFLAGS_EXTRA'],
                              ['CXXFLAGS', 'CXXFLAGS_EXTRA'],
                              ['LINKFLAGS', 'LINKFLAGS_EXTRA'],
                              ['LINKFLAGS', 'LDFLAGS_EXTRA']]:
        if envvar in os.environ:
            value = shlex.split(os.environ[envvar])
            conf.env.append_value(confvar, value)

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
    after = 'cxx_link cc_link'
    maxjobs = 1
    def __init__(self, bld, program):
        self.bld = bld
        self.m_display = 'build-suid'
        self.__program = program
        self.__env = bld.env.copy ()
        super(SuidBuildTask, self).__init__(generator=self)
        try:
            program_obj = wutils.find_program(self.__program.target, self.__env)
        except ValueError, ex:
            raise Utils.WafError(str(ex))
        program_node = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj))
        self.filename = program_node.abspath(self.__env)


    def run(self):
        print >> sys.stderr, 'setting suid bit on executable ' + self.filename
        if subprocess.Popen(['sudo', 'chown', 'root', self.filename]).wait():
            return 1
        if subprocess.Popen(['sudo', 'chmod', 'u+s', self.filename]).wait():
            return 1
        return 0

    def runnable_status(self):
        "RUN_ME SKIP_ME or ASK_LATER"
        st = os.stat(self.filename)
        if st.st_uid == 0:
            return Constants.SKIP_ME
        else:
            return Constants.RUN_ME


def create_suid_program(bld, name):
    program = bld.new_task_gen('cxx', 'program')
    program.is_ns3_program = True
    program.module_deps = list()
    program.name = name
    program.target = name

    if bld.env['ENABLE_SUDO']:
        SuidBuildTask(bld, program)

    return program

def create_ns3_program(bld, name, dependencies=('core',)):
    program = bld.new_task_gen('cxx', 'program')
    program.is_ns3_program = True
    program.name = name
    program.target = program.name
    # Each of the modules this program depends on has its own library.
    program.uselib_local = ['ns3-' + dep for dep in dependencies]
    program.ns3_module_dependencies = ['ns3-'+dep for dep in dependencies]
    if program.env['ENABLE_STATIC_NS3']:
        if sys.platform == 'darwin':
            program.env.append_value('LINKFLAGS', '-Wl,-all_load')
            for dep in dependencies:
                program.env.append_value('LINKFLAGS', '-lns3-' + dep)
        else:
            program.env.append_value('LINKFLAGS', '-Wl,--whole-archive,-Bstatic')
            for dep in dependencies:
                program.env.append_value('LINKFLAGS', '-lns3-' + dep)
            program.env.append_value('LINKFLAGS', '-Wl,-Bdynamic,--no-whole-archive')
    return program

def register_ns3_script(bld, name, dependencies=('core',)):
    ns3_module_dependencies = ['ns3-'+dep for dep in dependencies]
    bld.env.append_value('NS3_SCRIPT_DEPENDENCIES', (name, ns3_module_dependencies))

def add_examples_programs(bld):
    env = bld.env_of_name('default')
    if env['ENABLE_EXAMPLES']:
        for dir in os.listdir('examples'):
            if dir.startswith('.') or dir == 'CVS':
                continue
            if os.path.isdir(os.path.join('examples', dir)):
                bld.add_subdirs(os.path.join('examples', dir))


def add_scratch_programs(bld):
    all_modules = [mod[len("ns3-"):] for mod in bld.env['NS3_ENABLED_MODULES']]
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
    # If --enabled-modules option was given, then print a warning
    # message and exit this function.
    if Options.options.enable_modules:
        Logs.warn("No modules were built.  Use waf configure --enable-modules to enable modules.")
        return

    bld.env['NS3_MODULES_WITH_TEST_LIBRARIES'] = []
    bld.env['NS3_ENABLED_MODULE_TEST_LIBRARIES'] = []
    bld.env['NS3_SCRIPT_DEPENDENCIES'] = []
    bld.env['NS3_RUNNABLE_PROGRAMS'] = []
    bld.env['NS3_RUNNABLE_SCRIPTS'] = []

    wutils.bld = bld
    if Options.options.no_task_lines:
        import Runner
        def null_printout(s):
            pass
        Runner.printout = null_printout

    Options.cwd_launch = bld.path.abspath()
    bld.create_ns3_program = types.MethodType(create_ns3_program, bld)
    bld.register_ns3_script = types.MethodType(register_ns3_script, bld)
    bld.create_suid_program = types.MethodType(create_suid_program, bld)

    # switch default variant to the one matching our debug level
    variant_name = bld.env_of_name('default')['NS3_ACTIVE_VARIANT']
    variant_env = bld.env_of_name(variant_name)
    bld.all_envs['default'] = variant_env

    # process subfolders from here
    bld.add_subdirs('src')

    env = bld.env

    # If modules have been enabled, then set lists of enabled modules
    # and enabled module test libraries.
    if env['NS3_ENABLED_MODULES']:
        modules = env['NS3_ENABLED_MODULES']

        # Find out about additional modules that need to be enabled
        # due to dependency constraints.
        changed = True
        while changed:
            changed = False
            for module in modules:
                module_obj = bld.name_to_obj(module, env)
                if module_obj is None:
                    raise ValueError("module %s not found" % module)
                # Each enabled module has its own library.
                for dep in module_obj.uselib_local:
                    if not dep.startswith('ns3-'):
                        continue
                    if dep not in modules:
                        modules.append(dep)
                        changed = True

        env['NS3_ENABLED_MODULES'] = modules

        # If tests are being built, then set the list of the enabled
        # module test libraries.
        if env['ENABLE_TESTS']:
            for (mod, testlib) in bld.env['NS3_MODULES_WITH_TEST_LIBRARIES']:
                if mod in bld.env['NS3_ENABLED_MODULES']:
                    bld.env.append_value('NS3_ENABLED_MODULE_TEST_LIBRARIES', testlib)

    # Process this subfolder here after the lists of enabled modules
    # and module test libraries have been set.
    bld.add_subdirs('utils')

    add_examples_programs(bld)
    add_scratch_programs(bld)

    if env['NS3_ENABLED_MODULES']:
        modules = env['NS3_ENABLED_MODULES']

        def exclude_taskgen(bld, taskgen):
            # ok, so WAF does not provide an API to prevent an
            # arbitrary taskgen from running; we have to muck around with
            # WAF internal state, something that might stop working if
            # WAF is upgraded...
            bld.all_task_gen.remove(taskgen)
            for group in bld.task_manager.groups:
                try:
                    group.tasks_gen.remove(taskgen)
                except ValueError:
                    pass
                else:
                    break

        # Exclude the programs other misc task gens that depend on disabled modules
        for obj in list(bld.all_task_gen):

            # check for ns3moduleheader_taskgen
            if type(obj).__name__ == 'ns3moduleheader_taskgen':
                if ("ns3-%s" % obj.module) not in modules:
                    obj.mode = 'remove' # tell it to remove headers instead of installing

            # check for programs
            if hasattr(obj, 'ns3_module_dependencies'):
                # this is an NS-3 program (bld.create_ns3_program)
                program_built = True
                for dep in obj.ns3_module_dependencies:
                    if dep not in modules: # prog. depends on a module that isn't enabled?
                        exclude_taskgen(bld, obj)
                        program_built = False
                        break

                # Add this program to the list if all of its
                # dependencies will be built.
                if program_built:
                    bld.env.append_value('NS3_RUNNABLE_PROGRAMS', obj.name)

            # disable the modules themselves
            if hasattr(obj, "is_ns3_module") and obj.name not in modules:
                exclude_taskgen(bld, obj) # kill the module

            # disable the module test libraries
            if hasattr(obj, "is_ns3_module_test_library"):
                if not env['ENABLE_TESTS'] or (obj.module_name not in modules):
                    exclude_taskgen(bld, obj) # kill the module test library

            # disable the ns3header_taskgen
            if type(obj).__name__ == 'ns3header_taskgen':
                if ("ns3-%s" % obj.module) not in modules:
                    obj.mode = 'remove' # tell it to remove headers instead of installing 

    if env['NS3_ENABLED_MODULES']:
        env['NS3_ENABLED_MODULES'] = list(modules)

    # Determine which scripts will be runnable.
    for (script, dependencies) in bld.env['NS3_SCRIPT_DEPENDENCIES']:
        script_runnable = True
        for dep in dependencies:
            if dep not in modules:
                script_runnable = False
                break

        # Add this script to the list if all of its dependencies will
        # be built.
        if script_runnable:
            bld.env.append_value('NS3_RUNNABLE_SCRIPTS', script)

    bld.add_subdirs('bindings/python')

    if Options.options.run:
        # Check that the requested program name is valid
        program_name, dummy_program_argv = wutils.get_run_program(Options.options.run, wutils.get_command_template(env))

        # When --run'ing a program, tell WAF to only build that program,
        # nothing more; this greatly speeds up compilation when all you
        # want to do is run a test program.
        Options.options.compile_targets += ',' + os.path.basename(program_name)
        for gen in bld.all_task_gen:
            if type(gen).__name__ in ['ns3header_taskgen', 'ns3moduleheader_taskgen']:
                gen.post()

    if Options.options.doxygen_no_build:
        _doxygen(bld)
        raise SystemExit(0)

def shutdown(ctx):
    bld = wutils.bld
    if wutils.bld is None:
        return
    env = bld.env

    # Don't print the list if this a clean or distribution clean.
    if ('clean' not in Options.arg_line) and ('distclean' not in Options.arg_line):

        # Get the sorted list of built modules without the "ns3-" in their name.
        modules_without_prefix =[mod[len('ns3-'):] for mod in env['NS3_ENABLED_MODULES']]
        modules_without_prefix.sort()

        # Print the list of built modules in 3 columns.
        print
        print 'Modules built:'
        i = 1
        for mod in modules_without_prefix:
            print mod.ljust(25),
            if i == 3:
                    print
                    i = 0
            i = i+1
        print
        print

    # Write the build status file.
    build_status_file = os.path.join (env['NS3_BUILDDIR'], env['NS3_ACTIVE_VARIANT'], 'build-status.py')
    out = open(build_status_file, 'w')
    out.write('#! /usr/bin/env python\n')
    out.write('\n')
    out.write('# Programs that are runnable.\n')
    out.write('ns3_runnable_programs = ' + str(env['NS3_RUNNABLE_PROGRAMS']) + '\n')
    out.write('\n')
    out.write('# Scripts that are runnable.\n')
    out.write('ns3_runnable_scripts = ' + str(env['NS3_RUNNABLE_SCRIPTS']) + '\n')
    out.write('\n')
    out.close()

    if Options.options.lcov_report:
        lcov_report()

    if Options.options.run:
        wutils.run_program(Options.options.run, env, wutils.get_command_template(env),
                           visualize=Options.options.visualize)
        raise SystemExit(0)

    if Options.options.pyrun:
        wutils.run_python_program(Options.options.pyrun, env,
                                  visualize=Options.options.visualize)
        raise SystemExit(0)

    if Options.options.shell:
        raise Utils.WafError("Please run `./waf shell' now, instead of `./waf --shell'")

    if Options.options.check:
        raise Utils.WafError("Please run `./test.py' now, instead of `./waf --check'")

    check_shell(bld)

check_context = Build.BuildContext

def check(bld):
    """run the equivalent of the old ns-3 unit tests using test.py"""
    env = wutils.bld.env
    wutils.run_python_program("test.py -n -c core", env)

class print_introspected_doxygen_task(Task.TaskBase):
    after = 'cc cxx cc_link cxx_link'
    color = 'BLUE'

    def __init__(self, bld):
        self.bld = bld
        super(print_introspected_doxygen_task, self).__init__(generator=self)
        
    def __str__(self):
        return 'print-introspected-doxygen\n'

    def runnable_status(self):
        return Task.RUN_ME

    def run(self):
        ## generate the trace sources list docs
        env = wutils.bld.env
        proc_env = wutils.get_proc_env()
        try:
            program_obj = wutils.find_program('print-introspected-doxygen', env)
        except ValueError: # could happen if print-introspected-doxygen is
                           # not built because of waf configure
                           # --enable-modules=xxx
            pass
        else:
            prog = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj)).abspath(env)
            out = open(os.path.join('..', 'doc', 'introspected-doxygen.h'), 'w')
            if subprocess.Popen([prog], stdout=out, env=proc_env).wait():
                raise SystemExit(1)
            out.close()

class run_python_unit_tests_task(Task.TaskBase):
    after = 'cc cxx cc_link cxx_link'
    color = 'BLUE'

    def __init__(self, bld):
        self.bld = bld
        super(run_python_unit_tests_task, self).__init__(generator=self)
        
    def __str__(self):
        return 'run-python-unit-tests\n'

    def runnable_status(self):
        return Task.RUN_ME

    def run(self):
        proc_env = wutils.get_proc_env()
        wutils.run_argv([self.bld.env['PYTHON'], os.path.join("..", "utils", "python-unit-tests.py")],
                        self.bld.env, proc_env, force_no_valgrind=True)

def check_shell(bld):
    if ('NS3_MODULE_PATH' not in os.environ) or ('NS3_EXECUTABLE_PATH' not in os.environ):
        return
    env = bld.env
    correct_modpath = os.pathsep.join(env['NS3_MODULE_PATH'])
    found_modpath = os.environ['NS3_MODULE_PATH']
    correct_execpath = os.pathsep.join(env['NS3_EXECUTABLE_PATH'])
    found_execpath = os.environ['NS3_EXECUTABLE_PATH']
    if (found_modpath != correct_modpath) or (correct_execpath != found_execpath):
        msg = ("Detected shell (./waf shell) with incorrect configuration\n"
               "=========================================================\n"
               "Possible reasons for this problem:\n"
               "  1. You switched to another ns-3 tree from inside this shell\n"
               "  2. You switched ns-3 debug level (waf configure --debug)\n"
               "  3. You modified the list of built ns-3 modules\n"
               "You should correct this situation before running any program.  Possible solutions:\n"
               "  1. Exit this shell, and start a new one\n"
               "  2. Run a new nested shell")
        raise Utils.WafError(msg)


shell_context = Build.BuildContext
def shell(ctx):
    """run a shell with an environment suitably modified to run locally built programs"""

    #make sure we build first"
    Scripting.build(ctx)

    if sys.platform == 'win32':
        shell = os.environ.get("COMSPEC", "cmd.exe")
    else:
        shell = os.environ.get("SHELL", "/bin/sh")

    env = wutils.bld.env
    os_env = {'NS3_MODULE_PATH': os.pathsep.join(env['NS3_MODULE_PATH']), 'NS3_EXECUTABLE_PATH': os.pathsep.join(env['NS3_EXECUTABLE_PATH'])}
    wutils.run_argv([shell], env, os_env)

def _doxygen(bld):
    env = wutils.bld.env
    proc_env = wutils.get_proc_env()

    if not env['DOXYGEN']:
        Logs.error("waf configure did not detect doxygen in the system -> cannot build api docs.")
        raise SystemExit(1)
        return

    try:
        program_obj = wutils.find_program('print-introspected-doxygen', env)
    except ValueError: 
        Logs.warn("print-introspected-doxygen does not exist")
        raise SystemExit(1)
        return

    prog = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj)).abspath(env)

    if not os.path.exists(prog):
        Logs.error("print-introspected-doxygen has not been built yet."
                   " You need to build ns-3 at least once before "
                   "generating doxygen docs...")
        raise SystemExit(1)

    out = open(os.path.join('doc', 'introspected-doxygen.h'), 'w')

    if subprocess.Popen([prog], stdout=out, env=proc_env).wait():
        raise SystemExit(1)
    out.close()

    doxygen_config = os.path.join('doc', 'doxygen.conf')
    if subprocess.Popen([env['DOXYGEN'], doxygen_config]).wait():
        raise SystemExit(1)

def doxygen(bld):
    """do a full build, generate the introspected doxygen and then the doxygen"""
    Scripting.build(bld)
    _doxygen(bld)

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

def _copytree(src, dst, symlinks=False, excludes=(), build_dir=None):
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
    #"make a distribution directory with all the sources in it"
    import shutil

    # Our temporary folder where to put our files
    TMPFOLDER=appname+'-'+version

    # Remove an old package directory
    if os.path.exists(TMPFOLDER): shutil.rmtree(TMPFOLDER)

    global g_dist_exts, g_excludes

    # Remove the Build dir
    build_dir = getattr(Utils.g_module, BLDDIR, None)

    # Copy everything into the new folder
    _copytree('.', TMPFOLDER, excludes=excludes, build_dir=build_dir)

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


