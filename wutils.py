import os
import os.path
import sys
import pproc as subprocess
import shlex

# WAF modules
import ccroot
import Options
import Utils
import Logs
import TaskGen
import Build


# these are set from the main wscript file
APPNAME=None
VERSION=None

#
# The last part of the path name to use to find the regression traces tarball.
# path will be APPNAME + '-' + VERSION + REGRESSION_SUFFIX + TRACEBALL_SUFFIX,
# e.g., ns-3-dev-ref-traces.tar.bz2
#
TRACEBALL_SUFFIX = ".tar.bz2"



def get_command_template(env, arguments=()):
    if Options.options.valgrind:
        if Options.options.command_template:
            raise Utils.WafError("Options --command-template and --valgrind are conflicting")
        if not env['VALGRIND']:
            raise Utils.WafError("valgrind is not installed")
        cmd = env['VALGRIND'] + " --leak-check=full --error-exitcode=1 %s"
    else:
        cmd = Options.options.command_template or '%s'
    for arg in arguments:
        cmd = cmd + " " + arg
    return cmd


if hasattr(os.path, "relpath"):
    relpath = os.path.relpath # since Python 2.6
else:
    def relpath(path, start=os.path.curdir):
        """Return a relative version of a path"""

        if not path:
            raise ValueError("no path specified")

        start_list = os.path.abspath(start).split(os.path.sep)
        path_list = os.path.abspath(path).split(os.path.sep)

        # Work out how much of the filepath is shared by start and path.
        i = len(os.path.commonprefix([start_list, path_list]))

        rel_list = [os.path.pardir] * (len(start_list)-i) + path_list[i:]
        if not rel_list:
            return os.path.curdir
        return os.path.join(*rel_list)


def find_program(program_name, env):
    launch_dir = os.path.abspath(Options.cwd_launch)
    top_dir = os.path.abspath(Options.launch_dir)
    found_programs = []
    for obj in Build.bld.all_task_gen:
        if not getattr(obj, 'is_ns3_program', False):
            continue

        ## filter out programs not in the subtree starting at the launch dir
        if not (obj.path.abspath().startswith(launch_dir)
                or obj.path.abspath(env).startswith(launch_dir)):
            continue
        
        name1 = obj.target
        name2 = os.path.join(relpath(obj.path.abspath(), top_dir), obj.target)
        names = [name1, name2]
        found_programs.extend(names)
        if program_name in names:
            return obj
    raise ValueError("program '%s' not found; available programs are: %r"
                     % (program_name, found_programs))

def get_proc_env(os_env=None):
    env = Build.bld.env
    if sys.platform == 'linux2':
        pathvar = 'LD_LIBRARY_PATH'
    elif sys.platform == 'darwin':
        pathvar = 'DYLD_LIBRARY_PATH'
    elif sys.platform == 'win32':
        pathvar = 'PATH'
    elif sys.platform == 'cygwin':
        pathvar = 'PATH'
    elif sys.platform.startswith('freebsd'):
        pathvar = 'LD_LIBRARY_PATH'
    else:
        Logs.warn(("Don't know how to configure "
                        "dynamic library path for the platform %r;"
                        " assuming it's LD_LIBRARY_PATH.") % (sys.platform,))
        pathvar = 'LD_LIBRARY_PATH'        

    proc_env = dict(os.environ)
    if os_env is not None:
        proc_env.update(os_env)

    if pathvar is not None:
        if pathvar in proc_env:
            proc_env[pathvar] = os.pathsep.join(list(env['NS3_MODULE_PATH']) + [proc_env[pathvar]])
        else:
            proc_env[pathvar] = os.pathsep.join(list(env['NS3_MODULE_PATH']))

    pymoddir = Build.bld.path.find_dir('bindings/python').abspath(env)
    if 'PYTHONPATH' in proc_env:
        proc_env['PYTHONPATH'] = os.pathsep.join([pymoddir] + [proc_env['PYTHONPATH']])
    else:
        proc_env['PYTHONPATH'] = pymoddir

    return proc_env

def run_argv(argv, os_env=None, cwd=None):
    proc_env = get_proc_env(os_env)
    #env = Build.bld.env
    retval = subprocess.Popen(argv, env=proc_env, cwd=cwd).wait()
    if retval:
        raise Utils.WafError("Command %s exited with code %i" % (argv, retval))
    return retval

def get_run_program(program_string, command_template=None):
    """
    Return the program name and argv of the process that would be executed by
    run_program(program_string, command_template).
    """
    #print "get_run_program_argv(program_string=%r, command_template=%r)" % (program_string, command_template)
    env = Build.bld.env

    if command_template in (None, '%s'):
        argv = shlex.split(program_string)
        program_name = argv[0]

        try:
            program_obj = find_program(program_name, env)
        except ValueError, ex:
            raise Utils.WafError(str(ex))

        program_node = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj))
        #try:
        #    program_node = program_obj.path.find_build(ccroot.get_target_name(program_obj))
        #except AttributeError:
        #    raise Utils.WafError("%s does not appear to be a program" % (program_name,))

        execvec = [program_node.abspath(env)] + argv[1:]

    else:

        program_name = program_string
        try:
            program_obj = find_program(program_name, env)
        except ValueError, ex:
            raise Utils.WafError(str(ex))

        program_node = program_obj.path.find_or_declare(ccroot.get_target_name(program_obj))
        #try:
        #    program_node = program_obj.path.find_build(ccroot.get_target_name(program_obj))
        #except AttributeError:
        #    raise Utils.WafError("%s does not appear to be a program" % (program_name,))

        execvec = shlex.split(command_template % (program_node.abspath(env),))
    return program_name, execvec

def run_program(program_string, command_template=None, cwd=None):
    """
    if command_template is not None, then program_string == program
    name and argv is given by command_template with %s replaced by the
    full path to the program.  Else, program_string is interpreted as
    a shell command with first name being the program name.
    """
    dummy_program_name, execvec = get_run_program(program_string, command_template)
    if cwd is None:
        if (Options.options.cwd_launch):
            cwd = Options.options.cwd_launch
        else:
            cwd = Options.cwd_launch
    return run_argv(execvec, cwd=cwd)



def run_python_program(program_string):
    env = Build.bld.env
    execvec = shlex.split(program_string)
    if (Options.options.cwd_launch):
        cwd = Options.options.cwd_launch
    else:
        cwd = Options.cwd_launch
    return run_argv([env['PYTHON']] + execvec, cwd=cwd)


