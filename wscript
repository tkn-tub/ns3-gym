# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

import os
import sys
import subprocess

from waflib import Options
from waflib.Errors import WafError

#def options(opt):
#    pass

def configure(conf):
    conf.env['ENABLE_ZMQ'] = conf.check(mandatory=False, lib='zmq', define_name='HAVE_ZMQ', uselib='ZMQ')
    conf.env['ENABLE_PROTOBUF'] = conf.check(mandatory=False, lib='protobuf', define_name='HAVE_PROTOBUF', uselib='PROTOBUF')

    # check if protoc is installed
    conf.env['ENABLE_PROTOC'] = False

    protoc_min_version = (3,0,0)
    protoc = conf.find_program('protoc', var='PROTOC')

    try:
        cmd = [protoc[0], '--version']
        output = subprocess.check_output(cmd).decode("utf-8")
        output = output.split(" ")[1].rstrip()
        protoc_version = tuple(output.split('.'))
        protoc_version = tuple(map(int, protoc_version))
        conf.msg('Checking for protoc version', output)
        conf.env['ENABLE_PROTOC'] = True
    except Exception:
        conf.fatal('Could not determine the protoc version %r'%protoc)

    if not conf.env['ENABLE_ZMQ'] or not conf.env['ENABLE_PROTOBUF'] or not conf.env['ENABLE_PROTOC']:
        conf.env['MODULES_NOT_BUILT'].append('opengym')
        return

    # if protoc was found, check if version >= 3.0.0
    if protoc_version < protoc_min_version:
        conf.fatal('protoc version %s older than minimum supported version %s' %
                ('.'.join(map(str, protoc_version)), '.'.join(map(str, protoc_min_version)) ))

    conf.env.append_value("LINKFLAGS", ["-lzmq", "-lprotobuf"])
    conf.env.append_value("LIB", ["zmq", "protobuf"])

    # build protobuff messages
    try:
        pbSrcDir = str(conf.path) + "/model/"
        protoc = protoc[0]
        rc = subprocess.call(protoc+" -I="+pbSrcDir+" --cpp_out="+pbSrcDir+" "+pbSrcDir+"messages.proto", shell=True)

        if rc == 0:
            conf.msg("Build ns3gym Protobuf C++ messages", "Done")
        else:
            conf.fatal('Build ns3gym Protobuf C++ messages failed')

        rc = subprocess.call(protoc+" -I="+pbSrcDir+" --python_out="+pbSrcDir+"ns3gym/ns3gym/ "+pbSrcDir+"messages.proto", shell=True)

        if rc == 0:
            conf.msg("Build ns3gym Protobuf Python messages", "Done")
        else:
            conf.fatal('Build ns3gym Protobuf Python messages failed')

    except Exception as e:
        conf.env['MODULES_NOT_BUILT'].append('opengym')
        conf.fatal('Build of ns3gym Protobuf messages failed')

def build(bld):
    # Don't do anything for this module if click should not be built.
    if 'opengym' in bld.env['MODULES_NOT_BUILT']:
        return

    module = bld.create_ns3_module('opengym', ['core'])
    module.source = [
        'model/opengym_interface.cc',
        'model/messages.pb.cc',
        'model/container.cc',
        'model/spaces.cc',
        'model/opengym_env.cc',
        'helper/opengym-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('opengym')
    module_test.source = [
        'test/opengym-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'opengym'
    headers.source = [
        'model/opengym_interface.h',
        'model/messages.pb.h',
        'model/container.h',
        'model/spaces.h',
        'model/opengym_env.h',
        'helper/opengym-helper.h',
        ]

    if bld.env['ENABLE_ZMQ']:
        module.use.extend(['lzmq'])
        module.use.extend(['lprotobuf'])

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

