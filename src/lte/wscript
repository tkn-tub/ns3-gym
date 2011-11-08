## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):

    module = bld.create_ns3_module('lte', ['core', 'network', 'spectrum', 'buildings', 'stats'])
    module.source = [
        'model/lte-common.cc',
        'model/lte-spectrum-phy.cc',
        'model/lte-phy.cc',
        'model/lte-enb-phy.cc',
        'model/lte-ue-phy.cc',
        'model/lte-spectrum-value-helper.cc',
        'model/lte-amc.cc',
        'model/lte-enb-rrc.cc',
        'model/lte-ue-rrc.cc',
        'model/lte-rlc.cc',
        'model/lte-rlc-tag.cc',
        'model/eps-bearer.cc',        
        'model/lte-net-device.cc',
        'model/lte-enb-net-device.cc',
        'model/lte-ue-net-device.cc',
        'model/ideal-control-messages.cc',
        'helper/lena-helper.cc',
        'helper/lte-stats-calculator.cc',
        'helper/rlc-stats-calculator.cc',
        'helper/mac-stats-calculator.cc',
        'model/ff-mac-csched-sap.cc',
        'model/ff-mac-sched-sap.cc',
        'model/lte-mac-sap.cc',
        'model/ff-mac-scheduler.cc',
        'model/lte-enb-cmac-sap.cc',
        'model/lte-ue-cmac-sap.cc',
        'model/rr-ff-mac-scheduler.cc',
        'model/lte-enb-mac.cc',
        'model/lte-ue-mac.cc',
        'model/lte-mac-tag.cc',
        'model/lte-phy-tag.cc',
        'model/lte-enb-phy-sap.cc',
        'model/lte-ue-phy-sap.cc',
        'model/lte-interference.cc',
        'model/lte-sinr-chunk-processor.cc',
        'model/pf-ff-mac-scheduler.cc',
        'model/epc-gtpu-v1.cc',
        'model/trace-fading-loss-model.cc'
        ]

    module_test = bld.create_ns3_module_test_library('lte')
    module_test.source = [
        'test/lte-test-downlink-sinr.cc',
        'test/lte-test-uplink-sinr.cc',
        'test/lte-test-link-adaptation.cc',
        'test/lte-test-interference.cc',
        'test/lte-test-sinr-chunk-processor.cc',
        'test/lte-test-ue-phy.cc',
        'test/lte-test-rr-ff-mac-scheduler.cc',
        'test/lte-test-pf-ff-mac-scheduler.cc',
        'test/lte-test-earfcn.cc',
        'test/lte-test-spectrum-value-helper.cc',
        'test/epc-test-gtpu-v1.cc',
        'test/lte-test-pathloss-model.cc'
        ]
    
    headers = bld.new_task_gen(features=['ns3header'])
    headers.module = 'lte'
    headers.source = [
        'model/lte-common.h',
        'model/lte-spectrum-phy.h',
        'model/lte-phy.h',
        'model/lte-enb-phy.h',
        'model/lte-ue-phy.h',
        'model/lte-spectrum-value-helper.h',
        'model/lte-amc.h',
        'model/lte-enb-rrc.h',
        'model/lte-ue-rrc.h',
        'model/lte-rlc.h',
        'model/lte-rlc-tag.h',
        'model/eps-bearer.h',        
        'model/lte-net-device.h',
        'model/lte-enb-net-device.h',
        'model/lte-ue-net-device.h',
        'model/ideal-control-messages.h',
        'helper/lena-helper.h',
        'helper/lte-stats-calculator.h',
        'helper/mac-stats-calculator.h',
        'helper/rlc-stats-calculator.h',
        'model/ff-mac-common.h',
        'model/ff-mac-csched-sap.h',
        'model/ff-mac-sched-sap.h',
        'model/lte-enb-cmac-sap.h',
        'model/lte-ue-cmac-sap.h',
        'model/lte-mac-sap.h',
        'model/ff-mac-scheduler.h',
        'model/rr-ff-mac-scheduler.h',
        'model/lte-enb-mac.h',
        'model/lte-ue-mac.h',
        'model/lte-mac-tag.h',
        'model/lte-phy-tag.h',
        'model/lte-enb-phy-sap.h',
        'model/lte-ue-phy-sap.h',
        'model/lte-interference.h',
        'model/lte-sinr-chunk-processor.h',
        'model/pf-ff-mac-scheduler.h',
        'model/epc-gtpu-v1.h',
        'model/trace-fading-loss-model.h',
        'test/lte-test-downlink-sinr.h',
        'test/lte-test-uplink-sinr.h',
        'test/lte-test-link-adaptation.h',
        'test/lte-test-interference.h',
        'test/lte-test-sinr-chunk-processor.h',
        'test/lte-test-ue-phy.h',
        'test/lte-test-rr-ff-mac-scheduler.h',
        'test/lte-test-pf-ff-mac-scheduler.h',
        'test/lte-test-pf-ff-mac-scheduler.h',
        'test/epc-test-gtpu-v1.h',
        'test/lte-test-pathloss-model.h'
        ]

    if (bld.env['ENABLE_EXAMPLES']):
      bld.add_subdirs('examples')

    bld.ns3_python_bindings()
