## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

import os
from waflib import Logs, Options
from waflib.Errors import WafError

def options(opt):
    opt.add_option('--with-openflow',
                   help=('Path to OFSID source for NS-3 OpenFlow Integration support'),
                   default='', dest='with_openflow')

REQUIRED_BOOST_LIBS = ['system', 'signals', 'filesystem']

def required_boost_libs(conf):
    conf.env['REQUIRED_BOOST_LIBS'] += REQUIRED_BOOST_LIBS

def configure(conf):
    if not conf.env['LIB_BOOST']:
        conf.report_optional_feature("openflow", "NS-3 OpenFlow Integration", False,
                                     "Required boost libraries not found")

        # Add this module to the list of modules that won't be built
        # if they are enabled.
        conf.env['MODULES_NOT_BUILT'].append('openflow')
        return 

    present_boost_libs = []
    for boost_lib_name in conf.env['LIB_BOOST']:
        if boost_lib_name.startswith("boost_"):
            boost_lib_name = boost_lib_name[6:]
        if boost_lib_name.endswith("-mt"):
            boost_lib_name = boost_lib_name[:-3]
        present_boost_libs.append(boost_lib_name)

    missing_boost_libs = [lib for lib in REQUIRED_BOOST_LIBS if lib not in present_boost_libs]
    if missing_boost_libs != []:
        conf.report_optional_feature("openflow", "NS-3 OpenFlow Integration", False,
                                     "Required boost libraries not found, missing: %s" % ', '.join(missing_boost_libs))

        # Add this module to the list of modules that won't be built
        # if they are enabled.
        conf.env['MODULES_NOT_BUILT'].append('openflow')
        return 

    if Options.options.with_openflow:
        if os.path.isdir(Options.options.with_openflow):
            conf.msg("Checking for OpenFlow location", ("%s (given)" % Options.options.with_openflow))
            conf.env['WITH_OPENFLOW'] = os.path.abspath(Options.options.with_openflow)
    else:
        # bake.py uses ../../build, while ns-3-dev uses ../openflow.
        lib_to_check = 'libopenflow.a'
        openflow_bake_build_dir = os.path.join('..', '..', 'build')
        openflow_bake_lib_dir = os.path.join(openflow_bake_build_dir, 'lib')
        openflow_dir = os.path.join('..','openflow')
        if os.path.exists(os.path.join(openflow_bake_lib_dir, lib_to_check)):
            conf.msg("Checking for OpenFlow location",("%s (guessed)" % openflow_bake_build_dir))
            conf.env['WITH_OPENFLOW'] = os.path.abspath(openflow_bake_build_dir)
        elif os.path.isdir(openflow_dir):
            conf.msg("Checking for OpenFlow location", ("%s (guessed)" % openflow_dir))
            conf.env['WITH_OPENFLOW'] = os.path.abspath(openflow_dir)
        del openflow_bake_build_dir
        del openflow_bake_lib_dir
        del openflow_dir
    if not conf.env['WITH_OPENFLOW']:
        conf.msg("Checking for OpenFlow location", False)
        conf.report_optional_feature("openflow", "NS-3 OpenFlow Integration", False,
                                     "OpenFlow not enabled (see option --with-openflow)")
        # Add this module to the list of modules that won't be built
        # if they are enabled.
        conf.env['MODULES_NOT_BUILT'].append('openflow')
        return 

    test_code = '''
#include "openflow/openflow.h"
#include "openflow/nicira-ext.h"
#include "openflow/ericsson-ext.h"

extern "C"
{
#define private _private
#define delete _delete
#define list List

#include "openflow/private/csum.h"
#include "openflow/private/poll-loop.h"
#include "openflow/private/rconn.h"
#include "openflow/private/stp.h"
#include "openflow/private/vconn.h"
#include "openflow/private/xtoxll.h"

#include "openflow/private/chain.h"
#include "openflow/private/table.h"
#include "openflow/private/datapath.h" // The functions below are defined in datapath.c
uint32_t save_buffer (ofpbuf *);
ofpbuf * retrieve_buffer (uint32_t id);
void discard_buffer (uint32_t id);
#include "openflow/private/dp_act.h" // The functions below are defined in dp_act.c
void set_vlan_vid (ofpbuf *buffer, sw_flow_key *key, const ofp_action_header *ah);
void set_vlan_pcp (ofpbuf *buffer, sw_flow_key *key, const ofp_action_header *ah);
void strip_vlan (ofpbuf *buffer, sw_flow_key *key, const ofp_action_header *ah);
void set_dl_addr (ofpbuf *buffer, sw_flow_key *key, const ofp_action_header *ah);
void set_nw_addr (ofpbuf *buffer, sw_flow_key *key, const ofp_action_header *ah);
void set_tp_port (ofpbuf *buffer, sw_flow_key *key, const ofp_action_header *ah);
void set_mpls_label (ofpbuf *buffer, sw_flow_key *key, const ofp_action_header *ah);
void set_mpls_exp (ofpbuf *buffer, sw_flow_key *key, const ofp_action_header *ah);
#include "openflow/private/pt_act.h" // The function below is defined in pt_act.c
void update_checksums (ofpbuf *buffer, const sw_flow_key *key, uint32_t old_word, uint32_t new_word);

#undef list
#undef private
#undef delete
}

int main()
{
  return 0;
}
'''

    conf.env['DL'] = conf.check(mandatory=True, lib='dl', define_name='DL', uselib_store='DL')
    conf.env['XML2'] = conf.check(mandatory=True, lib='xml2', define_name='XML2', uselib_store='XML2')
    
    conf.env.append_value('NS3_MODULE_PATH',os.path.abspath(os.path.join(conf.env['WITH_OPENFLOW'],'build','default')))
    conf.env.append_value('NS3_MODULE_PATH',os.path.abspath(os.path.join(conf.env['WITH_OPENFLOW'],'lib')))
    
    conf.env['INCLUDES_OPENFLOW'] = [
        os.path.abspath(os.path.join(conf.env['WITH_OPENFLOW'],'include'))]
    conf.env['LIBPATH_OPENFLOW'] = [
        os.path.abspath(os.path.join(conf.env['WITH_OPENFLOW'],'build','default')),
        os.path.abspath(os.path.join(conf.env['WITH_OPENFLOW'],'lib'))]
    
    conf.env['DEFINES_OPENFLOW'] = ['NS3_OPENFLOW']

    conf.env['OPENFLOW'] = conf.check_nonfatal(fragment=test_code, lib='openflow', 
                                               libpath=conf.env['LIBPATH_OPENFLOW'],
                                               use='OPENFLOW DL XML2')
    conf.env.append_value('LIB_OPENFLOW', 'dl')
    conf.report_optional_feature("openflow", "NS-3 OpenFlow Integration",
                                 conf.env['OPENFLOW'], "openflow library not found")
    if conf.env['OPENFLOW']:
        conf.env['ENABLE_OPENFLOW'] = True
    else:
        # Add this module to the list of modules that won't be built
        # if they are enabled.
        conf.env['MODULES_NOT_BUILT'].append('openflow')



def build(bld):
    # Don't do anything for this module if openflow's not enabled.
    if 'openflow' in bld.env['MODULES_NOT_BUILT']:
        return

    # Build the Switch module
    obj = bld.create_ns3_module('openflow', ['internet'])
    obj.source = [
        ]

    obj_test = bld.create_ns3_module_test_library('openflow')
    obj_test.source = [
        ]

    if bld.env['OPENFLOW'] and bld.env['DL'] and bld.env['XML2']:
        obj.use.extend('OPENFLOW DL XML2'.split())
        obj_test.use.extend('OPENFLOW DL XML2'.split())

    headers = bld(features='ns3header')
    headers.module = 'openflow'
    headers.source = [
        ]

    if bld.env['ENABLE_OPENFLOW']:
        obj.source.append('model/openflow-interface.cc')
        obj.source.append('model/openflow-switch-net-device.cc')
        obj.source.append('helper/openflow-switch-helper.cc')

        obj.env.append_value('DEFINES', 'NS3_OPENFLOW')
        obj_test.source.append('test/openflow-switch-test-suite.cc')
        headers.source.append('model/openflow-interface.h')
        headers.source.append('model/openflow-switch-net-device.h')
        headers.source.append('helper/openflow-switch-helper.h')

    if bld.env['ENABLE_EXAMPLES'] and bld.env['ENABLE_OPENFLOW']:
        bld.recurse('examples')
