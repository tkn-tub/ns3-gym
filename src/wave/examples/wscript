# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    obj = bld.create_ns3_program('wave-simple-80211p',
        ['core', 'applications', 'mobility', 'network', 'wifi','wave'])
    obj.source = 'wave-simple-80211p.cc'
    
    obj = bld.create_ns3_program('wave-simple-device',
        ['core', 'applications', 'mobility', 'network', 'wifi','wave'])
    obj.source = 'wave-simple-device.cc'

    obj = bld.create_ns3_program('vanet-routing-compare',
        ['core', 'aodv', 'applications', 'dsr', 'dsdv', 'flow-monitor', 'mobility', 'network', 'olsr', 'propagation', 'wifi', 'wave'])
    obj.source = 'vanet-routing-compare.cc'
