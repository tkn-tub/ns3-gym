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

def dist_hook(srcdir, blddir):
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

    opt.add_option('--enable-gcov',
                   help=('Enable code coverage analysis.'
                         ' WARNING: this option only has effect '
                         'with the configure command.'),
                   action="store_true", default=False,
                   dest='enable_gcov')

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

    opt.add_option('--shell',
                   help=('Run a shell with an environment suitably modified to run locally built programs'),
                   action="store_true", default=False,
                   dest='shell')

    # options provided in a script in a subdirectory named "src"
    opt.sub_options('src')


def configure(conf):
    if not conf.check_tool('compiler_cxx'):
        Params.fatal("No suitable compiler found")

    # create the second environment, set the variant and set its name
    variant_env = conf.env.copy()
    debug_level = Params.g_options.debug_level.lower()
    if debug_level == 'ultradebug':
        variant_name = 'debug'
    else:
        variant_name = debug_level

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
        variant_env.append_value('CXXDEFINES', 'NS3_DEBUG_ENABLE')
        variant_env.append_value('CXXDEFINES', 'NS3_ASSERT_ENABLE')

    ## In optimized builds we still want debugging symbols, e.g. for
    ## profiling, and at least partially usable stack traces.
    if ('optimized' in Params.g_options.debug_level.lower() 
        and 'CXXFLAGS' not in os.environ):
        for flag in variant_env['CXXFLAGS_DEBUG']:
            ## this probably doesn't work for MSVC
            if flag.startswith('-g'):
                variant_env.append_value('CXXFLAGS', flag)

    if sys.platform == 'win32':
        if os.path.basename(conf.env['CXX']).startswith("g++"):
            variant_env.append_value("LINKFLAGS", "-Wl,--enable-runtime-pseudo-reloc")

    conf.sub_config('src')


def create_ns3_program(bld, name, dependencies=('simulator',)):
    program = bld.create_obj('cpp', 'program')
    program.name = name
    program.target = program.name
    program.uselib_local = 'ns3'
    return program


def build(bld):
    print "Entering directory `%s/build'" % Params.g_build.m_curdirnode.abspath()
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

    # process subfolders from here
    bld.add_subdirs('src')
    bld.add_subdirs('samples utils examples')

    ## Create a single ns3 library containing all modules
    lib = bld.create_obj('cpp', 'shlib')
    lib.name = 'ns3'
    lib.target = 'ns3'
    lib.add_objects = list(bld.env_of_name('default')['NS3_MODULES'])

def shutdown():
    #import UnitTest
    #ut = UnitTest.unit_test()
    #ut.change_to_testfile_dir = True
    #ut.want_to_see_test_output = True
    #ut.want_to_see_test_error = True
    #ut.run()
    #ut.print_results()

    if Params.g_commands['check']:
        run_program('run-tests')

    if Params.g_options.lcov_report:
        lcov_report()

    if Params.g_options.run:
        run_program(Params.g_options.run, Params.g_options.command_template)
        raise SystemExit(0)

    if Params.g_options.command_template:
        Params.fatal("Option --command-template requires the option --run to be given")

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

def _run_argv(argv, os_env=None):
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

    if command_template is None:
        argv = shlex.split(program_string)
        program_name = argv[0]

        try:
            program_obj = _find_program(program_name, env)
        except ValueError, ex:
            Params.fatal(str(ex))

        try:
            program_node, = program_obj.m_linktask.m_outputs
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
            program_node, = program_obj.m_linktask.m_outputs
        except AttributeError:
            Params.fatal("%s does not appear to be a program" % (program_name,))

        execvec = shlex.split(command_template % (program_node.abspath(env),))


    former_cwd = os.getcwd()
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

