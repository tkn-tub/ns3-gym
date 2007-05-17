## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
import os
import sys

import Params
import Object
import Action
import Common
import shutil
import pproc as subprocess

Params.g_autoconfig = 1

# the following two variables are used by the target "waf dist"
VERSION = '3.0.1'
APPNAME = 'ns-3-waf'

# these variables are mandatory ('/' are converted automatically)
srcdir = '.'
blddir = 'build'

class Ns3Header(Object.genobj):
    """A set of NS-3 header files"""
    def __init__(self, env=None):
        Object.genobj.__init__(self, 'other')
        self.inst_var = 'INCLUDEDIR'
        self.inst_dir = 'ns3'
        self.env = env
        if not self.env:
            self.env = Params.g_build.m_allenvs['default']

    def apply(self):
        ns3_dir_node = Params.g_build.m_srcnode.find_dir("ns3")
        inputs = []
        outputs = []
        for filename in self.to_list(self.source):
            src_node = self.path.find_source(filename)
            if src_node is None:
                Params.fatal("source ns3 header file %s not found" % (filename,))
            dst_node = ns3_dir_node.find_build(os.path.basename(filename))
            assert dst_node is not None
            inputs.append(src_node)
            outputs.append(dst_node)
        task = self.create_task('ns3_headers', self.env, 1)
        task.set_inputs(inputs)
        task.set_outputs(outputs)

    def install(self):
        for i in self.m_tasks:
            current = Params.g_build.m_curdirnode
            lst = map(lambda a: a.relpath_gen(current), i.m_outputs)
            Common.install_files(self.inst_var, self.inst_dir, lst)

def _ns3_headers_inst(task):
    assert len(task.m_inputs) == len(task.m_outputs)
    inputs = [node.srcpath(task.m_env) for node in task.m_inputs]
    outputs = [node.bldpath(task.m_env) for node in task.m_outputs]
    for src, dst in zip(inputs, outputs):
        try:
            os.chmod(dst, 0600)
        except OSError:
            pass
        shutil.copy2(src, dst)
        ## make the headers in builddir read-only, to prevent
        ## accidental modification
        os.chmod(dst, 0400)
    return 0

def init():
    Object.register('ns3header', Ns3Header)
    Action.Action('ns3_headers', func=_ns3_headers_inst, color='BLUE')

def set_options(opt):
    # options provided by the modules
    opt.tool_options('compiler_cxx')

    opt.add_option('--enable-gcov',
                   help=('Enable code coverage analysis'),
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

    # options provided in a script in a subdirectory named "src"
    opt.sub_options('src')


def configure(conf):
    if not conf.check_tool('compiler_cxx'):
        Params.fatal("No suitable compiler found")


    # create the second environment, set the variant and set its name
    variant_env = conf.env.copy()
    variant_name = Params.g_options.debug_level.lower()

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
    if 'debug' in Params.g_options.debug_level.lower():
        variant_env.append_value('CXXDEFINES', 'NS3_DEBUG_ENABLE')
        variant_env.append_value('CXXDEFINES', 'NS3_ASSERT_ENABLE')

    if sys.platform == 'win32':
        variant_env.append_value("LINKFLAGS", "-Wl,--enable-runtime-pseudo-reloc")

    conf.sub_config('src')


def build(bld):
    variant_name = bld.env_of_name('default')['NS3_ACTIVE_VARIANT']
    variant_env = bld.env_of_name(variant_name)
    bld.m_allenvs['default'] = variant_env # switch to the active variant
    # process subfolders from here
    bld.add_subdirs('src')
    bld.add_subdirs('samples utils examples')


def shutdown():
    import UnitTest
    ut = UnitTest.unit_test()
    ut.change_to_testfile_dir = True
    ut.want_to_see_test_output = True
    ut.want_to_see_test_error = True
    ut.run()
    #ut.print_results()

    if Params.g_options.lcov_report:
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

    if Params.g_options.doxygen:
        doxygen_config = os.path.join('doc', 'doxygen.conf')
        if subprocess.Popen(['doxygen', doxygen_config]).wait():
            raise SystemExit(1)

