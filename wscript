## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# python lib modules
import sys
import shutil
import types
import optparse
import os.path
import re
import shlex
import subprocess
import textwrap

from utils import read_config_file


# WAF modules
from waflib import Utils, Scripting, Configure, Build, Options, TaskGen, Context, Task, Logs, Errors
from waflib.Errors import WafError


# local modules
import wutils


# By default, all modules will be enabled, examples will be disabled,
# and tests will be disabled.
modules_enabled  = ['all_modules']
examples_enabled = False
tests_enabled    = False

# Bug 1868:  be conservative about -Wstrict-overflow for optimized builds
# on older compilers; it can generate spurious warnings.  
cc_version_warn_strict_overflow = ('4', '8', '2')

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

Configure.autoconfig = 0

# the following two variables are used by the target "waf dist"
VERSION = file("VERSION", "rt").read().strip()
APPNAME = 'ns'

wutils.VERSION = VERSION
wutils.APPNAME = APPNAME

# we don't use VNUM anymore (see bug #1327 for details)
wutils.VNUM = None

# these variables are mandatory ('/' are converted automatically)
top = '.'
out = 'build'

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

# Print the sorted list of module names in columns.
def print_module_names(names):
    # Sort the list of module names.
    names.sort()

    # Print the list of module names in 3 columns.
    i = 1
    for name in names:
        print name.ljust(25),
        if i == 3:
                print
                i = 0
        i = i+1

    if i != 1:
        print

def options(opt):
    # options provided by the modules
    opt.load('compiler_c')
    opt.load('compiler_cxx')
    opt.load('cflags')
    opt.load('gnu_dirs')

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
    opt.recurse('src')
    opt.recurse('bindings/python')
    opt.recurse('src/internet')


def _check_compilation_flag(conf, flag, mode='cxx', linkflags=None):
    """
    Checks if the C++ compiler accepts a certain compilation flag or flags
    flag: can be a string or a list of strings
    """
    l = []
    if flag:
        l.append(flag)
    if isinstance(linkflags, list):
        l.extend(linkflags)
    else:
        if linkflags:
            l.append(linkflags)
    if len(l) > 1:
        flag_str = 'flags ' + ' '.join(l)
    else:
        flag_str = 'flag ' + ' '.join(l)
    if flag_str > 28:
        flag_str = flag_str[:28] + "..."

    conf.start_msg('Checking for compilation %s support' % (flag_str,))
    env = conf.env.derive()

    retval = False
    if mode == 'cc':
        mode = 'c'

    if mode == 'cxx':
        env.append_value('CXXFLAGS', flag)
    else:
        env.append_value('CFLAGS', flag)

    if linkflags is not None:
        env.append_value("LINKFLAGS", linkflags)

    try:
        retval = conf.check(compiler=mode, fragment='int main() { return 0; }', features='c', env=env)
    except Errors.ConfigurationError:
        ok = False
    else:
        ok = (retval == True)
    conf.end_msg(ok)
    return ok


def report_optional_feature(conf, name, caption, was_enabled, reason_not_enabled):
    conf.env.append_value('NS3_OPTIONAL_FEATURES', [(name, caption, was_enabled, reason_not_enabled)])


def check_optional_feature(conf, name):
    for (name1, caption, was_enabled, reason_not_enabled) in conf.env.NS3_OPTIONAL_FEATURES:
        if name1 == name:
            return was_enabled
    raise KeyError("Feature %r not declared yet" % (name,))


# starting with waf 1.6, conf.check() becomes fatal by default if the
# test fails, this alternative method makes the test non-fatal, as it
# was in waf <= 1.5
def _check_nonfatal(conf, *args, **kwargs):
    try:
        return conf.check(*args, **kwargs)
    except conf.errors.ConfigurationError:
        return None


def configure(conf):
    conf.load('relocation', tooldir=['waf-tools'])

    # attach some extra methods
    conf.check_nonfatal = types.MethodType(_check_nonfatal, conf)
    conf.check_compilation_flag = types.MethodType(_check_compilation_flag, conf)
    conf.report_optional_feature = types.MethodType(report_optional_feature, conf)
    conf.check_optional_feature = types.MethodType(check_optional_feature, conf)
    conf.env['NS3_OPTIONAL_FEATURES'] = []

    conf.load('compiler_c')
    cc_string = '.'.join(conf.env['CC_VERSION'])
    conf.msg('Checking for cc version',cc_string,'GREEN')
    conf.load('compiler_cxx')
    conf.load('cflags', tooldir=['waf-tools'])
    conf.load('command', tooldir=['waf-tools'])
    conf.load('gnu_dirs')
    conf.load('clang_compilation_database', tooldir=['waf-tools'])

    env = conf.env

    if Options.options.enable_gcov:
        env['GCOV_ENABLED'] = True
        env.append_value('CCFLAGS', '-fprofile-arcs')
        env.append_value('CCFLAGS', '-ftest-coverage')
        env.append_value('CXXFLAGS', '-fprofile-arcs')
        env.append_value('CXXFLAGS', '-ftest-coverage')
        env.append_value('LINKFLAGS', '-lgcov')
        env.append_value('LINKFLAGS', '-coverage')

    if Options.options.build_profile == 'debug':
        env.append_value('DEFINES', 'NS3_ASSERT_ENABLE')
        env.append_value('DEFINES', 'NS3_LOG_ENABLE')

    env['PLATFORM'] = sys.platform
    env['BUILD_PROFILE'] = Options.options.build_profile
    if Options.options.build_profile == "release":
        env['BUILD_SUFFIX'] = ''
    else:
        env['BUILD_SUFFIX'] = '-'+Options.options.build_profile
    
    env['APPNAME'] = wutils.APPNAME
    env['VERSION'] = wutils.VERSION

    if conf.env['CXX_NAME'] in ['gcc', 'icc']:
        if Options.options.build_profile == 'release': 
            env.append_value('CXXFLAGS', '-fomit-frame-pointer') 
        if Options.options.build_profile == 'optimized': 
            if conf.check_compilation_flag('-march=native'):
                env.append_value('CXXFLAGS', '-march=native') 
            env.append_value('CXXFLAGS', '-fstrict-overflow')
            if conf.env['CC_VERSION'] == cc_version_warn_strict_overflow:
                env.append_value('CXXFLAGS', '-Wstrict-overflow=5')

        if sys.platform == 'win32':
            env.append_value("LINKFLAGS", "-Wl,--enable-runtime-pseudo-reloc")
        elif sys.platform == 'cygwin':
            env.append_value("LINKFLAGS", "-Wl,--enable-auto-import")

        cxx = env['CXX']
        cxx_check_libstdcxx = cxx + ['-print-file-name=libstdc++.so']
        p = subprocess.Popen(cxx_check_libstdcxx, stdout=subprocess.PIPE)
        libstdcxx_location = os.path.dirname(p.stdout.read().strip())
        p.wait()
        if libstdcxx_location:
            conf.env.append_value('NS3_MODULE_PATH', libstdcxx_location)

        if Options.platform in ['linux']:
            if conf.check_compilation_flag('-Wl,--soname=foo'):
                env['WL_SONAME_SUPPORTED'] = True

    env['ENABLE_STATIC_NS3'] = False
    if Options.options.enable_static:
        if Options.platform == 'darwin':
            if conf.check_compilation_flag(flag=[], linkflags=['-Wl,-all_load']):
                conf.report_optional_feature("static", "Static build", True, '')
                env['ENABLE_STATIC_NS3'] = True
            else:
                conf.report_optional_feature("static", "Static build", False,
                                             "Link flag -Wl,-all_load does not work")
        else:
            if conf.check_compilation_flag(flag=[], linkflags=['-Wl,--whole-archive,-Bstatic', '-Wl,-Bdynamic,--no-whole-archive']):
                conf.report_optional_feature("static", "Static build", True, '')
                env['ENABLE_STATIC_NS3'] = True
            else:
                conf.report_optional_feature("static", "Static build", False,
                                             "Link flag -Wl,--whole-archive,-Bstatic does not work")

    # Set this so that the lists won't be printed at the end of this
    # configure command.
    conf.env['PRINT_BUILT_MODULES_AT_END'] = False

    conf.env['MODULES_NOT_BUILT'] = []

    conf.recurse('bindings/python')

    conf.recurse('src')

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

    # Add the template module to the list of enabled modules that
    # should not be built if this is a static build on Darwin.  They
    # don't work there for the template module, and this is probably
    # because the template module has no source files.
    if conf.env['ENABLE_STATIC_NS3'] and sys.platform == 'darwin':
        conf.env['MODULES_NOT_BUILT'].append('template')

    # Remove these modules from the list of enabled modules.
    for not_built in conf.env['MODULES_NOT_BUILT']:
        not_built_name = 'ns3-' + not_built
        if not_built_name in conf.env['NS3_ENABLED_MODULES']:
            conf.env['NS3_ENABLED_MODULES'].remove(not_built_name)
            if not conf.env['NS3_ENABLED_MODULES']:
                raise WafError('Exiting because the ' + not_built + ' module can not be built and it was the only one enabled.')

    conf.recurse('src/mpi')

    # for suid bits
    try:
        conf.find_program('sudo', var='SUDO')
    except WafError:
        pass

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

    conf.report_optional_feature("ENABLE_EXAMPLES", "Build examples", env['ENABLE_EXAMPLES'], 
                                 why_not_examples)

    env['VALGRIND_FOUND'] = False
    try:
        conf.find_program('valgrind', var='VALGRIND')
        env['VALGRIND_FOUND'] = True
    except WafError:
        pass

    # These flags are used for the implicitly dependent modules.
    if env['ENABLE_STATIC_NS3']:
        if sys.platform == 'darwin':
            env.STLIB_MARKER = '-Wl,-all_load'
        else:
            env.STLIB_MARKER = '-Wl,--whole-archive,-Bstatic'
            env.SHLIB_MARKER = '-Wl,-Bdynamic,--no-whole-archive'


    have_gsl = conf.check_cfg(package='gsl', args=['--cflags', '--libs'],
                              uselib_store='GSL', mandatory=False)
    conf.env['ENABLE_GSL'] = have_gsl
    conf.report_optional_feature("GSL", "GNU Scientific Library (GSL)",
                                 conf.env['ENABLE_GSL'],
                                 "GSL not found")

    # for compiling C code, copy over the CXX* flags
    conf.env.append_value('CCFLAGS', conf.env['CXXFLAGS'])

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

    try:
        conf.find_program('doxygen', var='DOXYGEN')
    except WafError:
        pass

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
    print "%-30s: %s%s%s" % ("Build profile", Logs.colors('GREEN'),
                             Options.options.build_profile, Logs.colors('NORMAL'))
    bld = wutils.bld
    print "%-30s: %s%s%s" % ("Build directory", Logs.colors('GREEN'),
                             Options.options.out, Logs.colors('NORMAL'))
    
    
    for (name, caption, was_enabled, reason_not_enabled) in conf.env['NS3_OPTIONAL_FEATURES']:
        if was_enabled:
            status = 'enabled'
            color = 'GREEN'
        else:
            status = 'not enabled (%s)' % reason_not_enabled
            color = 'RED'
        print "%-30s: %s%s%s" % (caption, Logs.colors(color), status, Logs.colors('NORMAL'))


class SuidBuild_task(Task.Task):
    """task that makes a binary Suid
    """
    after = 'link'
    def __init__(self, *args, **kwargs):
        super(SuidBuild_task, self).__init__(*args, **kwargs)
        self.m_display = 'build-suid'
        try:
            program_obj = wutils.find_program(self.generator.name, self.generator.env)
        except ValueError, ex:
            raise WafError(str(ex))
        program_node = program_obj.path.find_or_declare(program_obj.target)
        self.filename = program_node.get_bld().abspath()


    def run(self):
        print >> sys.stderr, 'setting suid bit on executable ' + self.filename
        if subprocess.Popen(['sudo', 'chown', 'root', self.filename]).wait():
            return 1
        if subprocess.Popen(['sudo', 'chmod', 'u+s', self.filename]).wait():
            return 1
        return 0

    def runnable_status(self):
        "RUN_ME SKIP_ME or ASK_LATER"
        try:
            st = os.stat(self.filename)
        except OSError:
            return Task.ASK_LATER
        if st.st_uid == 0:
            return Task.SKIP_ME
        else:
            return Task.RUN_ME

def create_suid_program(bld, name):
    grp = bld.current_group
    bld.add_group() # this to make sure no two sudo tasks run at the same time
    program = bld(features='cxx cxxprogram')
    program.is_ns3_program = True
    program.module_deps = list()
    program.name = name
    program.target = "%s%s-%s%s" % (wutils.APPNAME, wutils.VERSION, name, bld.env.BUILD_SUFFIX)

    if bld.env['ENABLE_SUDO']:
        program.create_task("SuidBuild")

    bld.set_group(grp)

    return program

def create_ns3_program(bld, name, dependencies=('core',)):
    program = bld(features='cxx cxxprogram')

    program.is_ns3_program = True
    program.name = name
    program.target = "%s%s-%s%s" % (wutils.APPNAME, wutils.VERSION, name, bld.env.BUILD_SUFFIX)
    # Each of the modules this program depends on has its own library.
    program.ns3_module_dependencies = ['ns3-'+dep for dep in dependencies]
    program.includes = "#"
    program.use = program.ns3_module_dependencies
    if program.env['ENABLE_STATIC_NS3']:
        if sys.platform == 'darwin':
            program.env.STLIB_MARKER = '-Wl,-all_load'
        else:
            program.env.STLIB_MARKER = '-Wl,-Bstatic,--whole-archive'
            program.env.SHLIB_MARKER = '-Wl,-Bdynamic,--no-whole-archive'
    else:
        if program.env.DEST_BINFMT == 'elf':
            # All ELF platforms are impacted but only the gcc compiler has a flag to fix it.
            if 'gcc' in (program.env.CXX_NAME, program.env.CC_NAME): 
                program.env.append_value ('SHLIB_MARKER', '-Wl,--no-as-needed')

    return program

def register_ns3_script(bld, name, dependencies=('core',)):
    ns3_module_dependencies = ['ns3-'+dep for dep in dependencies]
    bld.env.append_value('NS3_SCRIPT_DEPENDENCIES', [(name, ns3_module_dependencies)])

def add_examples_programs(bld):
    env = bld.env
    if env['ENABLE_EXAMPLES']:
        try:
            for dir in os.listdir('examples'):
                if dir.startswith('.') or dir == 'CVS':
                    continue
                if os.path.isdir(os.path.join('examples', dir)):
                    bld.recurse(os.path.join('examples', dir))
        except OSError:
            return

def add_scratch_programs(bld):
    all_modules = [mod[len("ns3-"):] for mod in bld.env['NS3_ENABLED_MODULES']]
    try:
        for filename in os.listdir("scratch"):
            if filename.startswith('.') or filename == 'CVS':
                continue
            if os.path.isdir(os.path.join("scratch", filename)):
                obj = bld.create_ns3_program(filename, all_modules)
                obj.path = obj.path.find_dir('scratch').find_dir(filename)
                obj.source = obj.path.ant_glob('*.cc')
                obj.target = filename
                obj.name = obj.target
                obj.install_path = None
            elif filename.endswith(".cc"):
                name = filename[:-len(".cc")]
                obj = bld.create_ns3_program(name, all_modules)
                obj.path = obj.path.find_dir('scratch')
                obj.source = filename
                obj.target = name
                obj.name = obj.target
                obj.install_path = None
    except OSError:
        return

def _get_all_task_gen(self):
    for group in self.groups:
        for taskgen in group:
            yield taskgen


# ok, so WAF does not provide an API to prevent an
# arbitrary taskgen from running; we have to muck around with
# WAF internal state, something that might stop working if
# WAF is upgraded...
def _exclude_taskgen(self, taskgen):
    for group in self.groups:
        for tg1 in group:
            if tg1 is taskgen:
                group.remove(tg1)
                break
        else:
            continue
        break


def _find_ns3_module(self, name):
    for obj in _get_all_task_gen(self):
        # disable the modules themselves
        if hasattr(obj, "is_ns3_module") and obj.name == name:
            return obj
    raise KeyError(name)


def build(bld):
    env = bld.env

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
        from waflib import Runner
        def null_printout(s):
            pass
        Runner.printout = null_printout

    Options.cwd_launch = bld.path.abspath()
    bld.create_ns3_program = types.MethodType(create_ns3_program, bld)
    bld.register_ns3_script = types.MethodType(register_ns3_script, bld)
    bld.create_suid_program = types.MethodType(create_suid_program, bld)
    bld.__class__.all_task_gen = property(_get_all_task_gen)
    bld.exclude_taskgen = types.MethodType(_exclude_taskgen, bld)
    bld.find_ns3_module = types.MethodType(_find_ns3_module, bld)

    # Clean documentation build directories; other cleaning happens later
    if bld.cmd == 'clean':
        _cleandocs()

    # process subfolders from here
    bld.recurse('src')

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
                module_obj = bld.get_tgen_by_name(module)
                if module_obj is None:
                    raise ValueError("module %s not found" % module)
                # Each enabled module has its own library.
                for dep in module_obj.use:
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

    add_examples_programs(bld)
    add_scratch_programs(bld)

    if env['NS3_ENABLED_MODULES']:
        modules = env['NS3_ENABLED_MODULES']

        # Exclude the programs other misc task gens that depend on disabled modules
        for obj in list(bld.all_task_gen):

            # check for ns3moduleheader_taskgen
            if 'ns3moduleheader' in getattr(obj, "features", []):
                if ("ns3-%s" % obj.module) not in modules:
                    obj.mode = 'remove' # tell it to remove headers instead of installing

            # check for programs
            if hasattr(obj, 'ns3_module_dependencies'):
                # this is an NS-3 program (bld.create_ns3_program)
                program_built = True
                for dep in obj.ns3_module_dependencies:
                    if dep not in modules: # prog. depends on a module that isn't enabled?
                        bld.exclude_taskgen(obj)
                        program_built = False
                        break

                # Add this program to the list if all of its
                # dependencies will be built.
                if program_built:
                    object_name = "%s%s-%s%s" % (wutils.APPNAME, wutils.VERSION, 
                                                  obj.name, bld.env.BUILD_SUFFIX)

                    # Get the relative path to the program from the
                    # launch directory.
                    launch_dir = os.path.abspath(Context.launch_dir)
                    object_relative_path = os.path.join(
                        wutils.relpath(obj.path.get_bld().abspath(), launch_dir),
                        object_name)

                    bld.env.append_value('NS3_RUNNABLE_PROGRAMS', object_relative_path)

            # disable the modules themselves
            if hasattr(obj, "is_ns3_module") and obj.name not in modules:
                bld.exclude_taskgen(obj) # kill the module

            # disable the module test libraries
            if hasattr(obj, "is_ns3_module_test_library"):
                if not env['ENABLE_TESTS'] or (obj.module_name not in modules):
                    bld.exclude_taskgen(obj) # kill the module test library

            # disable the ns3header_taskgen
            if 'ns3header' in getattr(obj, "features", []):
                if ("ns3-%s" % obj.module) not in modules:
                    obj.mode = 'remove' # tell it to remove headers instead of installing 

            # disable the ns3privateheader_taskgen
            if 'ns3privateheader' in getattr(obj, "features", []):
                if ("ns3-%s" % obj.module) not in modules:
                    obj.mode = 'remove' # tell it to remove headers instead of installing 

            # disable pcfile taskgens for disabled modules
            if 'ns3pcfile' in getattr(obj, "features", []):
                if obj.module not in bld.env.NS3_ENABLED_MODULES:
                    bld.exclude_taskgen(obj)


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

    bld.recurse('bindings/python')

    # Process this subfolder here after the lists of enabled modules
    # and module test libraries have been set.
    bld.recurse('utils')

    # Set this so that the lists will be printed at the end of this
    # build command.
    bld.env['PRINT_BUILT_MODULES_AT_END'] = True

    # Do not print the modules built if build command was "clean"
    if bld.cmd == 'clean':
        bld.env['PRINT_BUILT_MODULES_AT_END'] = False

    if Options.options.run:
        # Check that the requested program name is valid
        program_name, dummy_program_argv = wutils.get_run_program(Options.options.run, wutils.get_command_template(env))

        # When --run'ing a program, tell WAF to only build that program,
        # nothing more; this greatly speeds up compilation when all you
        # want to do is run a test program.
        Options.options.targets += ',' + os.path.basename(program_name)
        if getattr(Options.options, "visualize", False):
            program_obj = wutils.find_program(program_name, bld.env)
            program_obj.use.append('ns3-visualizer')
        for gen in bld.all_task_gen:
            if type(gen).__name__ in ['ns3header_taskgen', 'ns3privateheader_taskgen', 'ns3moduleheader_taskgen']:
                gen.post()
        bld.env['PRINT_BUILT_MODULES_AT_END'] = False 

    if Options.options.doxygen_no_build:
        _doxygen(bld)
        raise SystemExit(0)

def _cleandir(name):
    try:
        shutil.rmtree(name)
    except:
        pass

def _cleandocs():
    _cleandir('doc/html')
    _cleandir('doc/manual/build')
    _cleandir('doc/manual/source-temp')
    _cleandir('doc/tutorial/build')
    _cleandir('doc/tutorial-pt-br/build')
    _cleandir('doc/models/build')
    _cleandir('doc/models/source-temp')

# 'distclean' typically only cleans out build/ directory
# Here we clean out any build or documentation artifacts not in build/
def distclean(ctx):
    _cleandocs()
    # Now call waf's normal distclean
    Scripting.distclean(ctx)

def shutdown(ctx):
    bld = wutils.bld
    if wutils.bld is None:
        return
    env = bld.env

    # Only print the lists if a build was done.
    if (env['PRINT_BUILT_MODULES_AT_END']):

        # Print the list of built modules.
        print
        print 'Modules built:'
        names_without_prefix = []
        for name in env['NS3_ENABLED_MODULES']:
            name1 = name[len('ns3-'):]
            if name not in env.MODULAR_BINDINGS_MODULES:
                name1 += " (no Python)"
            names_without_prefix.append(name1)
        print_module_names(names_without_prefix)
        print

        # Print the list of enabled modules that were not built.
        if env['MODULES_NOT_BUILT']:
            print 'Modules not built (see ns-3 tutorial for explanation):'
            print_module_names(env['MODULES_NOT_BUILT'])
            print

        # Set this so that the lists won't be printed until the next
        # build is done.
        bld.env['PRINT_BUILT_MODULES_AT_END'] = False

    # Write the build status file.
    build_status_file = os.path.join(bld.out_dir, 'build-status.py')
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
        lcov_report(bld)

    if Options.options.run:
        wutils.run_program(Options.options.run, env, wutils.get_command_template(env),
                           visualize=Options.options.visualize)
        raise SystemExit(0)

    if Options.options.pyrun:
        wutils.run_python_program(Options.options.pyrun, env,
                                  visualize=Options.options.visualize)
        raise SystemExit(0)

    if Options.options.shell:
        raise WafError("Please run `./waf shell' now, instead of `./waf --shell'")

    if Options.options.check:
        raise WafError("Please run `./test.py' now, instead of `./waf --check'")

    check_shell(bld)



class CheckContext(Context.Context):
    """run the equivalent of the old ns-3 unit tests using test.py"""
    cmd = 'check'

    def execute(self):

        # first we execute the build
	bld = Context.create_context("build")
	bld.options = Options.options # provided for convenience
	bld.cmd = "build"
	bld.execute()

        wutils.bld = bld
        wutils.run_python_program("test.py -n -c core", bld.env)


class print_introspected_doxygen_task(Task.TaskBase):
    after = 'cxx link'
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
            prog = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj)).get_bld().abspath(env)

            # Create a header file with the introspected information.
            doxygen_out = open(os.path.join('doc', 'introspected-doxygen.h'), 'w')
            if subprocess.Popen([prog], stdout=doxygen_out, env=proc_env).wait():
                raise SystemExit(1)
            doxygen_out.close()
        
            # Create a text file with the introspected information.
            text_out = open(os.path.join('doc', 'ns3-object.txt'), 'w')
            if subprocess.Popen([prog, '--output-text'], stdout=text_out, env=proc_env).wait():
                raise SystemExit(1)
            text_out.close()

class run_python_unit_tests_task(Task.TaskBase):
    after = 'cxx link'
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
        raise WafError(msg)


class Ns3ShellContext(Context.Context):
    """run a shell with an environment suitably modified to run locally built programs"""
    cmd = 'shell'

    def execute(self):

        # first we execute the build
	bld = Context.create_context("build")
	bld.options = Options.options # provided for convenience
	bld.cmd = "build"
	bld.execute()

        # Set this so that the lists won't be printed when the user
        # exits the shell.
        bld.env['PRINT_BUILT_MODULES_AT_END'] = False

        if sys.platform == 'win32':
            shell = os.environ.get("COMSPEC", "cmd.exe")
        else:
            shell = os.environ.get("SHELL", "/bin/sh")

        env = bld.env
        os_env = {
            'NS3_MODULE_PATH': os.pathsep.join(env['NS3_MODULE_PATH']),
            'NS3_EXECUTABLE_PATH': os.pathsep.join(env['NS3_EXECUTABLE_PATH']),
            }
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

    prog = program_obj.path.find_or_declare(program_obj.target).get_bld().abspath()

    if not os.path.exists(prog):
        Logs.error("print-introspected-doxygen has not been built yet."
                   " You need to build ns-3 at least once before "
                   "generating doxygen docs...")
        raise SystemExit(1)

    # Create a header file with the introspected information.
    doxygen_out = open(os.path.join('doc', 'introspected-doxygen.h'), 'w')
    if subprocess.Popen([prog], stdout=doxygen_out, env=proc_env).wait():
        raise SystemExit(1)
    doxygen_out.close()

    # Create a text file with the introspected information.
    text_out = open(os.path.join('doc', 'ns3-object.txt'), 'w')
    if subprocess.Popen([prog, '--output-text'], stdout=text_out, env=proc_env).wait():
        raise SystemExit(1)
    text_out.close()

    _getVersion()
    doxygen_config = os.path.join('doc', 'doxygen.conf')
    if subprocess.Popen(env['DOXYGEN'] + [doxygen_config]).wait():
        Logs.error("Doxygen build returned an error.")
        raise SystemExit(1)


def _getVersion():
    """update the ns3_version.js file, when building documentation"""

    prog = "doc/ns3_html_theme/get_version.sh"
    if subprocess.Popen([prog]).wait() :
        Logs.error(prog + " returned an error")
        raise SystemExit(1)

class Ns3DoxygenContext(Context.Context):
    """do a full build, generate the introspected doxygen and then the doxygen"""
    cmd = 'doxygen'
    def execute(self):
        # first we execute the build
	bld = Context.create_context("build")
	bld.options = Options.options # provided for convenience
	bld.cmd = "build"
	bld.execute()
        _doxygen(bld)

class Ns3SphinxContext(Context.Context):
    """build the Sphinx documentation: manual, tutorial, models"""
    
    cmd = 'sphinx'

    def sphinx_build(self, path):
        print
        print "[waf] Building sphinx docs for " + path
        if subprocess.Popen(["make", "SPHINXOPTS=-N", "-k",
                             "html", "singlehtml", "latexpdf" ],
                            cwd=path).wait() :
            Logs.error("Sphinx build of " + path + " returned an error.")
            raise SystemExit(1)

    def execute(self):
        _getVersion()
        for sphinxdir in ["manual", "models", "tutorial", "tutorial-pt-br"] :
            self.sphinx_build(os.path.join("doc", sphinxdir))
     

class Ns3DocContext(Context.Context):
    """build all the documentation: doxygen, manual, tutorial, models"""
    
    cmd = 'docs'

    def execute(self):
        steps = ['doxygen', 'sphinx']
        Options.commands = steps + Options.commands
        
    
def lcov_report(bld):
    env = bld.env

    if not env['GCOV_ENABLED']:
        raise WafError("project not configured for code coverage;"
                       " reconfigure with --enable-gcov")

    os.chdir(out)
    try:
        lcov_report_dir = 'lcov-report'
        create_dir_command = "rm -rf " + lcov_report_dir
        create_dir_command += " && mkdir " + lcov_report_dir + ";"

        if subprocess.Popen(create_dir_command, shell=True).wait():
            raise SystemExit(1)

        info_file = os.path.join(lcov_report_dir, 'report.info')
        lcov_command = "../utils/lcov/lcov -c -d . -o " + info_file
        lcov_command += " -b " + os.getcwd()
        if subprocess.Popen(lcov_command, shell=True).wait():
            raise SystemExit(1)

        genhtml_command = "../utils/lcov/genhtml -o " + lcov_report_dir
        genhtml_command += " " + info_file
        if subprocess.Popen(genhtml_command, shell=True).wait():
            raise SystemExit(1)
    finally:
        os.chdir("..")

