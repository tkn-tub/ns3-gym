## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):

    module = bld.create_ns3_module('lte', ['internet', 'spectrum', 'wimax'])
    module.source = [
        'model/lte-spectrum-phy.cc',
        'model/lte-spectrum-signal-parameters.cc',
        'model/enb-lte-spectrum-phy.cc',
        'model/ue-lte-spectrum-phy.cc',
        'model/lte-phy.cc',
        'model/enb-phy.cc',
        'model/ue-phy.cc',
        'model/lte-spectrum-value-helper.cc',
        'model/lte-propagation-loss-model.cc',
        'model/discrete-time-loss-model.cc',
        'model/penetration-loss-model.cc',
        'model/shadowing-loss-model.cc',
        'model/path-loss-model.cc',
        'model/jakes-fading-loss-model.cc',
        'model/channel-realization.cc',
        'model/amc-module.cc',
        'model/lte-mac-queue.cc',
        'model/rrc-entity.cc',
        'model/rlc-entity.cc',
        'model/mac-entity.cc',
        'model/lte-mac-header.cc',
        'model/enb-mac-entity.cc',
        'model/ue-mac-entity.cc',
        'model/radio-bearer-instance.cc',
        'model/bearer-qos-parameters.cc',
        'model/lte-net-device.cc',
        'model/ue-record.cc',
        'model/ue-manager.cc',
        'model/enb-net-device.cc',
        'model/ue-net-device.cc',
        'model/packet-scheduler.cc',
        'model/simple-packet-scheduler.cc',
        'model/ideal-control-messages.cc',
        'helper/lte-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('lte')
    module_test.source = [
        'test/lte-phy-test.cc',
        'test/lte-device-test.cc',
        'test/lte-bearer-test.cc',
        'test/lte-propagation-loss-model-test.cc',
        ]
    
    headers = bld.new_task_gen(features=['ns3header'])
    headers.module = 'lte'
    headers.source = [
        'model/lte-spectrum-phy.h',
        'model/lte-spectrum-signal-parameters.h',
        'model/enb-lte-spectrum-phy.h',
        'model/ue-lte-spectrum-phy.h',
        'model/lte-phy.h',
        'model/enb-phy.h',
        'model/ue-phy.h',
        'model/lte-spectrum-value-helper.h',
        'model/lte-propagation-loss-model.h',
        'model/discrete-time-loss-model.h',
        'model/penetration-loss-model.h',
        'model/shadowing-loss-model.h',
        'model/path-loss-model.h',
        'model/jakes-fading-loss-model.h',
        'model/channel-realization.h',
        'model/amc-module.h',
        'model/lte-mac-queue.h',
        'model/rrc-entity.h',
        'model/rlc-entity.h',
        'model/mac-entity.h',
        'model/lte-mac-header.h',
        'model/enb-mac-entity.h',
        'model/ue-mac-entity.h',
        'model/radio-bearer-instance.h',
        'model/bearer-qos-parameters.h',
        'model/lte-net-device.h',
        'model/ue-record.h',
        'model/ue-manager.h',
        'model/enb-net-device.h',
        'model/ue-net-device.h',
        'model/packet-scheduler.h',
        'model/simple-packet-scheduler.h',
        'model/ideal-control-messages.h',
        'helper/lte-helper.h',
        ]

    if (bld.env['ENABLE_EXAMPLES']):
      bld.add_subdirs('examples')

    bld.ns3_python_bindings()
