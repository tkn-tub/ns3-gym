from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## wifi-information-element-vector.h: ns3::WifiElementId [enumeration]
    module.add_enum('WifiElementId', ['IE11S_MESH_CONFIGURATION', 'IE11S_MESH_ID', 'IE11S_LINK_METRIC_REPORT', 'IE11S_CONGESTION_NOTIFICATION', 'IE11S_PEERING_MANAGEMENT', 'IE11S_SUPP_MBSS_REG_CLASSES_CHANNELS', 'IE11S_MESH_CHANNEL_SWITCH_ANNOUNCEMENT', 'IE11S_MESH_TIM', 'IE11S_AWAKE_WINDOW', 'IE11S_BEACON_TIMING', 'IE11S_MCCAOP_SETUP_REQUEST', 'IE11S_MCCAOP_SETUP_REPLY', 'IE11S_MCCAOP_ADVERTISEMENT', 'IE11S_MCCAOP_RESERVATION_TEARDOWN', 'IE11S_PORTAL_ANNOUNCEMENT', 'IE11S_RANN', 'IE11S_PREQ', 'IE11S_PREP', 'IE11S_PERR', 'IE11S_PROXY_UPDATE', 'IE11S_PROXY_UPDATE_CONFIRMATION', 'IE11S_ABBREVIATED_HANDSHAKE', 'IE11S_MESH_PEERING_PROTOCOL_VERSION'])
    ## mesh-wifi-beacon.h: ns3::MeshWifiBeacon [class]
    module.add_class('MeshWifiBeacon')
