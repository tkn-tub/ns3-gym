from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## mesh-helper.h: ns3::MeshHelper [class]
    module.add_class('MeshHelper')
    ## mesh-helper.h: ns3::MeshHelper::ChannelPolicy [enumeration]
    module.add_enum('ChannelPolicy', ['SPREAD_CHANNELS', 'ZERO_CHANNEL'], outer_class=root_module['ns3::MeshHelper'])
    ## mesh-wifi-beacon.h: ns3::MeshWifiBeacon [class]
    module.add_class('MeshWifiBeacon')
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::MeshWifiInterfaceMacPlugin', 'ns3::empty', 'ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::dot11s::DestinationAddressUnit', 'ns3::empty', 'ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::dot11s::IeBeaconTimingUnit', 'ns3::empty', 'ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## mesh-information-element-vector.h: ns3::MeshInformationElementVector [class]
    module.add_class('MeshInformationElementVector', parent=root_module['ns3::WifiInformationElementVector'])
    ## mesh-l2-routing-protocol.h: ns3::MeshL2RoutingProtocol [class]
    module.add_class('MeshL2RoutingProtocol', parent=root_module['ns3::Object'])
    ## mesh-stack-installer.h: ns3::MeshStack [class]
    module.add_class('MeshStack', parent=root_module['ns3::Object'])
    ## mesh-wifi-interface-mac-plugin.h: ns3::MeshWifiInterfaceMacPlugin [class]
    module.add_class('MeshWifiInterfaceMacPlugin', parent=root_module['ns3::SimpleRefCount< ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >'])
    ## dot11s-installer.h: ns3::Dot11sStack [class]
    module.add_class('Dot11sStack', parent=root_module['ns3::MeshStack'])
    ## flame-installer.h: ns3::FlameStack [class]
    module.add_class('FlameStack', parent=root_module['ns3::MeshStack'])
    ## mesh-point-device.h: ns3::MeshPointDevice [class]
    module.add_class('MeshPointDevice', parent=root_module['ns3::NetDevice'])
    ## mesh-wifi-interface-mac.h: ns3::MeshWifiInterfaceMac [class]
    module.add_class('MeshWifiInterfaceMac', parent=root_module['ns3::RegularWifiMac'])
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace addressUtils
    
    nested_module = module.add_cpp_namespace('addressUtils')
    register_types_ns3_addressUtils(nested_module)
    
    
    ## Register a nested module for the namespace aodv
    
    nested_module = module.add_cpp_namespace('aodv')
    register_types_ns3_aodv(nested_module)
    
    
    ## Register a nested module for the namespace dot11s
    
    nested_module = module.add_cpp_namespace('dot11s')
    register_types_ns3_dot11s(nested_module)
    
    
    ## Register a nested module for the namespace dsdv
    
    nested_module = module.add_cpp_namespace('dsdv')
    register_types_ns3_dsdv(nested_module)
    
    
    ## Register a nested module for the namespace flame
    
    nested_module = module.add_cpp_namespace('flame')
    register_types_ns3_flame(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_aodv(module):
    root_module = module.get_root()
    

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    
    ## ie-dot11s-configuration.h: ns3::dot11s::dot11sCongestionControlMode [enumeration]
    module.add_enum('dot11sCongestionControlMode', ['CONGESTION_SIGNALING', 'CONGESTION_NULL'])
    ## ie-dot11s-configuration.h: ns3::dot11s::dot11sAuthenticationProtocol [enumeration]
    module.add_enum('dot11sAuthenticationProtocol', ['AUTH_NULL', 'AUTH_SAE'])
    ## ie-dot11s-configuration.h: ns3::dot11s::dot11sPathSelectionProtocol [enumeration]
    module.add_enum('dot11sPathSelectionProtocol', ['PROTOCOL_HWMP'])
    ## ie-dot11s-configuration.h: ns3::dot11s::dot11sSynchronizationProtocolIdentifier [enumeration]
    module.add_enum('dot11sSynchronizationProtocolIdentifier', ['SYNC_NEIGHBOUR_OFFSET', 'SYNC_NULL'])
    ## ie-dot11s-peer-management.h: ns3::dot11s::PmpReasonCode [enumeration]
    module.add_enum('PmpReasonCode', ['REASON11S_PEERING_CANCELLED', 'REASON11S_MESH_MAX_PEERS', 'REASON11S_MESH_CAPABILITY_POLICY_VIOLATION', 'REASON11S_MESH_CLOSE_RCVD', 'REASON11S_MESH_MAX_RETRIES', 'REASON11S_MESH_CONFIRM_TIMEOUT', 'REASON11S_MESH_INVALID_GTK', 'REASON11S_MESH_INCONSISTENT_PARAMETERS', 'REASON11S_MESH_INVALID_SECURITY_CAPABILITY', 'REASON11S_RESERVED'])
    ## ie-dot11s-configuration.h: ns3::dot11s::dot11sPathSelectionMetric [enumeration]
    module.add_enum('dot11sPathSelectionMetric', ['METRIC_AIRTIME'])
    ## ie-dot11s-preq.h: ns3::dot11s::DestinationAddressUnit [class]
    module.add_class('DestinationAddressUnit', parent=root_module['ns3::SimpleRefCount< ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> >'])
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability [class]
    module.add_class('Dot11sMeshCapability')
    ## hwmp-protocol.h: ns3::dot11s::HwmpProtocol [class]
    module.add_class('HwmpProtocol', parent=root_module['ns3::MeshL2RoutingProtocol'])
    ## hwmp-protocol.h: ns3::dot11s::HwmpProtocol::FailedDestination [struct]
    module.add_class('FailedDestination', outer_class=root_module['ns3::dot11s::HwmpProtocol'])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable [class]
    module.add_class('HwmpRtable', parent=root_module['ns3::Object'])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult [struct]
    module.add_class('LookupResult', outer_class=root_module['ns3::dot11s::HwmpRtable'])
    ## ie-dot11s-beacon-timing.h: ns3::dot11s::IeBeaconTiming [class]
    module.add_class('IeBeaconTiming', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-beacon-timing.h: ns3::dot11s::IeBeaconTimingUnit [class]
    module.add_class('IeBeaconTimingUnit', parent=root_module['ns3::SimpleRefCount< ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >'])
    ## ie-dot11s-configuration.h: ns3::dot11s::IeConfiguration [class]
    module.add_class('IeConfiguration', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-metric-report.h: ns3::dot11s::IeLinkMetricReport [class]
    module.add_class('IeLinkMetricReport', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshId [class]
    module.add_class('IeMeshId', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshIdChecker [class]
    module.add_class('IeMeshIdChecker', parent=root_module['ns3::AttributeChecker'])
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshIdValue [class]
    module.add_class('IeMeshIdValue', parent=root_module['ns3::AttributeValue'])
    ## ie-dot11s-peer-management.h: ns3::dot11s::IePeerManagement [class]
    module.add_class('IePeerManagement', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-peer-management.h: ns3::dot11s::IePeerManagement::Subtype [enumeration]
    module.add_enum('Subtype', ['PEER_OPEN', 'PEER_CONFIRM', 'PEER_CLOSE'], outer_class=root_module['ns3::dot11s::IePeerManagement'])
    ## ie-dot11s-peering-protocol.h: ns3::dot11s::IePeeringProtocol [class]
    module.add_class('IePeeringProtocol', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-perr.h: ns3::dot11s::IePerr [class]
    module.add_class('IePerr', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-prep.h: ns3::dot11s::IePrep [class]
    module.add_class('IePrep', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-preq.h: ns3::dot11s::IePreq [class]
    module.add_class('IePreq', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-rann.h: ns3::dot11s::IeRann [class]
    module.add_class('IeRann', parent=root_module['ns3::WifiInformationElement'])
    ## dot11s-mac-header.h: ns3::dot11s::MeshHeader [class]
    module.add_class('MeshHeader', parent=root_module['ns3::Header'])
    ## peer-link.h: ns3::dot11s::PeerLink [class]
    module.add_class('PeerLink', parent=root_module['ns3::Object'])
    ## peer-link.h: ns3::dot11s::PeerLink::PeerState [enumeration]
    module.add_enum('PeerState', ['IDLE', 'OPN_SNT', 'CNF_RCVD', 'OPN_RCVD', 'ESTAB', 'HOLDING'], outer_class=root_module['ns3::dot11s::PeerLink'])
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart [class]
    module.add_class('PeerLinkFrameStart', parent=root_module['ns3::Header'])
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields [struct]
    module.add_class('PlinkFrameStartFields', outer_class=root_module['ns3::dot11s::PeerLinkFrameStart'])
    ## peer-management-protocol.h: ns3::dot11s::PeerManagementProtocol [class]
    module.add_class('PeerManagementProtocol', parent=root_module['ns3::Object'])
    module.add_container('std::vector< ns3::dot11s::HwmpProtocol::FailedDestination >', 'ns3::dot11s::HwmpProtocol::FailedDestination', container_type='vector')

def register_types_ns3_dsdv(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    
    ## flame-header.h: ns3::flame::FlameHeader [class]
    module.add_class('FlameHeader', parent=root_module['ns3::Header'])
    ## flame-protocol.h: ns3::flame::FlameProtocol [class]
    module.add_class('FlameProtocol', parent=root_module['ns3::MeshL2RoutingProtocol'])
    ## flame-protocol-mac.h: ns3::flame::FlameProtocolMac [class]
    module.add_class('FlameProtocolMac', parent=root_module['ns3::MeshWifiInterfaceMacPlugin'])
    ## flame-rtable.h: ns3::flame::FlameRtable [class]
    module.add_class('FlameRtable', parent=root_module['ns3::Object'])
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult [struct]
    module.add_class('LookupResult', outer_class=root_module['ns3::flame::FlameRtable'])
    ## flame-protocol.h: ns3::flame::FlameTag [class]
    module.add_class('FlameTag', parent=root_module['ns3::Tag'])

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3MeshHelper_methods(root_module, root_module['ns3::MeshHelper'])
    register_Ns3MeshWifiBeacon_methods(root_module, root_module['ns3::MeshWifiBeacon'])
    register_Ns3MeshInformationElementVector_methods(root_module, root_module['ns3::MeshInformationElementVector'])
    register_Ns3MeshL2RoutingProtocol_methods(root_module, root_module['ns3::MeshL2RoutingProtocol'])
    register_Ns3MeshStack_methods(root_module, root_module['ns3::MeshStack'])
    register_Ns3MeshWifiInterfaceMacPlugin_methods(root_module, root_module['ns3::MeshWifiInterfaceMacPlugin'])
    register_Ns3Dot11sStack_methods(root_module, root_module['ns3::Dot11sStack'])
    register_Ns3FlameStack_methods(root_module, root_module['ns3::FlameStack'])
    register_Ns3MeshPointDevice_methods(root_module, root_module['ns3::MeshPointDevice'])
    register_Ns3MeshWifiInterfaceMac_methods(root_module, root_module['ns3::MeshWifiInterfaceMac'])
    register_Ns3Dot11sDestinationAddressUnit_methods(root_module, root_module['ns3::dot11s::DestinationAddressUnit'])
    register_Ns3Dot11sDot11sMeshCapability_methods(root_module, root_module['ns3::dot11s::Dot11sMeshCapability'])
    register_Ns3Dot11sHwmpProtocol_methods(root_module, root_module['ns3::dot11s::HwmpProtocol'])
    register_Ns3Dot11sHwmpProtocolFailedDestination_methods(root_module, root_module['ns3::dot11s::HwmpProtocol::FailedDestination'])
    register_Ns3Dot11sHwmpRtable_methods(root_module, root_module['ns3::dot11s::HwmpRtable'])
    register_Ns3Dot11sHwmpRtableLookupResult_methods(root_module, root_module['ns3::dot11s::HwmpRtable::LookupResult'])
    register_Ns3Dot11sIeBeaconTiming_methods(root_module, root_module['ns3::dot11s::IeBeaconTiming'])
    register_Ns3Dot11sIeBeaconTimingUnit_methods(root_module, root_module['ns3::dot11s::IeBeaconTimingUnit'])
    register_Ns3Dot11sIeConfiguration_methods(root_module, root_module['ns3::dot11s::IeConfiguration'])
    register_Ns3Dot11sIeLinkMetricReport_methods(root_module, root_module['ns3::dot11s::IeLinkMetricReport'])
    register_Ns3Dot11sIeMeshId_methods(root_module, root_module['ns3::dot11s::IeMeshId'])
    register_Ns3Dot11sIeMeshIdChecker_methods(root_module, root_module['ns3::dot11s::IeMeshIdChecker'])
    register_Ns3Dot11sIeMeshIdValue_methods(root_module, root_module['ns3::dot11s::IeMeshIdValue'])
    register_Ns3Dot11sIePeerManagement_methods(root_module, root_module['ns3::dot11s::IePeerManagement'])
    register_Ns3Dot11sIePeeringProtocol_methods(root_module, root_module['ns3::dot11s::IePeeringProtocol'])
    register_Ns3Dot11sIePerr_methods(root_module, root_module['ns3::dot11s::IePerr'])
    register_Ns3Dot11sIePrep_methods(root_module, root_module['ns3::dot11s::IePrep'])
    register_Ns3Dot11sIePreq_methods(root_module, root_module['ns3::dot11s::IePreq'])
    register_Ns3Dot11sIeRann_methods(root_module, root_module['ns3::dot11s::IeRann'])
    register_Ns3Dot11sMeshHeader_methods(root_module, root_module['ns3::dot11s::MeshHeader'])
    register_Ns3Dot11sPeerLink_methods(root_module, root_module['ns3::dot11s::PeerLink'])
    register_Ns3Dot11sPeerLinkFrameStart_methods(root_module, root_module['ns3::dot11s::PeerLinkFrameStart'])
    register_Ns3Dot11sPeerLinkFrameStartPlinkFrameStartFields_methods(root_module, root_module['ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields'])
    register_Ns3Dot11sPeerManagementProtocol_methods(root_module, root_module['ns3::dot11s::PeerManagementProtocol'])
    register_Ns3FlameFlameHeader_methods(root_module, root_module['ns3::flame::FlameHeader'])
    register_Ns3FlameFlameProtocol_methods(root_module, root_module['ns3::flame::FlameProtocol'])
    register_Ns3FlameFlameProtocolMac_methods(root_module, root_module['ns3::flame::FlameProtocolMac'])
    register_Ns3FlameFlameRtable_methods(root_module, root_module['ns3::flame::FlameRtable'])
    register_Ns3FlameFlameRtableLookupResult_methods(root_module, root_module['ns3::flame::FlameRtable::LookupResult'])
    register_Ns3FlameFlameTag_methods(root_module, root_module['ns3::flame::FlameTag'])
    return

def register_Ns3MeshHelper_methods(root_module, cls):
    ## mesh-helper.h: ns3::MeshHelper::MeshHelper(ns3::MeshHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshHelper const &', 'arg0')])
    ## mesh-helper.h: ns3::MeshHelper::MeshHelper() [constructor]
    cls.add_constructor([])
    ## mesh-helper.h: static ns3::MeshHelper ns3::MeshHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::MeshHelper', 
                   [], 
                   is_static=True)
    ## mesh-helper.h: ns3::NetDeviceContainer ns3::MeshHelper::Install(ns3::WifiPhyHelper const & phyHelper, ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phyHelper'), param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## mesh-helper.h: void ns3::MeshHelper::Report(ns3::Ptr<ns3::NetDevice> const & arg0, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice > const &', 'arg0'), param('std::ostream &', 'arg1')])
    ## mesh-helper.h: void ns3::MeshHelper::ResetStats(ns3::Ptr<ns3::NetDevice> const & arg0) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice > const &', 'arg0')])
    ## mesh-helper.h: void ns3::MeshHelper::SetMacType(std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMacType', 
                   'void', 
                   [param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## mesh-helper.h: void ns3::MeshHelper::SetNumberOfInterfaces(uint32_t nInterfaces) [member function]
    cls.add_method('SetNumberOfInterfaces', 
                   'void', 
                   [param('uint32_t', 'nInterfaces')])
    ## mesh-helper.h: void ns3::MeshHelper::SetRemoteStationManager(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetRemoteStationManager', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## mesh-helper.h: void ns3::MeshHelper::SetSpreadInterfaceChannels(ns3::MeshHelper::ChannelPolicy arg0) [member function]
    cls.add_method('SetSpreadInterfaceChannels', 
                   'void', 
                   [param('ns3::MeshHelper::ChannelPolicy', 'arg0')])
    ## mesh-helper.h: void ns3::MeshHelper::SetStackInstaller(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetStackInstaller', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## mesh-helper.h: void ns3::MeshHelper::SetStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('SetStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')])
    return

def register_Ns3MeshWifiBeacon_methods(root_module, cls):
    ## mesh-wifi-beacon.h: ns3::MeshWifiBeacon::MeshWifiBeacon(ns3::MeshWifiBeacon const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshWifiBeacon const &', 'arg0')])
    ## mesh-wifi-beacon.h: ns3::MeshWifiBeacon::MeshWifiBeacon(ns3::Ssid ssid, ns3::SupportedRates rates, uint64_t us) [constructor]
    cls.add_constructor([param('ns3::Ssid', 'ssid'), param('ns3::SupportedRates', 'rates'), param('uint64_t', 'us')])
    ## mesh-wifi-beacon.h: void ns3::MeshWifiBeacon::AddInformationElement(ns3::Ptr<ns3::WifiInformationElement> ie) [member function]
    cls.add_method('AddInformationElement', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiInformationElement >', 'ie')])
    ## mesh-wifi-beacon.h: ns3::MgtBeaconHeader ns3::MeshWifiBeacon::BeaconHeader() const [member function]
    cls.add_method('BeaconHeader', 
                   'ns3::MgtBeaconHeader', 
                   [], 
                   is_const=True)
    ## mesh-wifi-beacon.h: ns3::WifiMacHeader ns3::MeshWifiBeacon::CreateHeader(ns3::Mac48Address address, ns3::Mac48Address mpAddress) [member function]
    cls.add_method('CreateHeader', 
                   'ns3::WifiMacHeader', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::Mac48Address', 'mpAddress')])
    ## mesh-wifi-beacon.h: ns3::Ptr<ns3::Packet> ns3::MeshWifiBeacon::CreatePacket() [member function]
    cls.add_method('CreatePacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## mesh-wifi-beacon.h: ns3::Time ns3::MeshWifiBeacon::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    return

def register_Ns3MeshInformationElementVector_methods(root_module, cls):
    ## mesh-information-element-vector.h: ns3::MeshInformationElementVector::MeshInformationElementVector() [constructor]
    cls.add_constructor([])
    ## mesh-information-element-vector.h: ns3::MeshInformationElementVector::MeshInformationElementVector(ns3::MeshInformationElementVector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshInformationElementVector const &', 'arg0')])
    ## mesh-information-element-vector.h: uint32_t ns3::MeshInformationElementVector::DeserializeSingleIe(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DeserializeSingleIe', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    return

def register_Ns3MeshL2RoutingProtocol_methods(root_module, cls):
    ## mesh-l2-routing-protocol.h: ns3::MeshL2RoutingProtocol::MeshL2RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## mesh-l2-routing-protocol.h: ns3::MeshL2RoutingProtocol::MeshL2RoutingProtocol(ns3::MeshL2RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshL2RoutingProtocol const &', 'arg0')])
    ## mesh-l2-routing-protocol.h: ns3::Ptr<ns3::MeshPointDevice> ns3::MeshL2RoutingProtocol::GetMeshPoint() const [member function]
    cls.add_method('GetMeshPoint', 
                   'ns3::Ptr< ns3::MeshPointDevice >', 
                   [], 
                   is_const=True)
    ## mesh-l2-routing-protocol.h: static ns3::TypeId ns3::MeshL2RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-l2-routing-protocol.h: bool ns3::MeshL2RoutingProtocol::RemoveRoutingStuff(uint32_t fromIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet> packet, uint16_t & protocolType) [member function]
    cls.add_method('RemoveRoutingStuff', 
                   'bool', 
                   [param('uint32_t', 'fromIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t &', 'protocolType')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-l2-routing-protocol.h: bool ns3::MeshL2RoutingProtocol::RequestRoute(uint32_t sourceIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet const> packet, uint16_t protocolType, ns3::Callback<void, bool, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'sourceIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocolType'), param('ns3::Callback< void, bool, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-l2-routing-protocol.h: void ns3::MeshL2RoutingProtocol::SetMeshPoint(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('SetMeshPoint', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')])
    return

def register_Ns3MeshStack_methods(root_module, cls):
    ## mesh-stack-installer.h: ns3::MeshStack::MeshStack() [constructor]
    cls.add_constructor([])
    ## mesh-stack-installer.h: ns3::MeshStack::MeshStack(ns3::MeshStack const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshStack const &', 'arg0')])
    ## mesh-stack-installer.h: bool ns3::MeshStack::InstallStack(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('InstallStack', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-stack-installer.h: void ns3::MeshStack::Report(ns3::Ptr<ns3::MeshPointDevice> const mp, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp'), param('std::ostream &', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-stack-installer.h: void ns3::MeshStack::ResetStats(ns3::Ptr<ns3::MeshPointDevice> const mp) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3MeshWifiInterfaceMacPlugin_methods(root_module, cls):
    ## mesh-wifi-interface-mac-plugin.h: ns3::MeshWifiInterfaceMacPlugin::MeshWifiInterfaceMacPlugin() [constructor]
    cls.add_constructor([])
    ## mesh-wifi-interface-mac-plugin.h: ns3::MeshWifiInterfaceMacPlugin::MeshWifiInterfaceMacPlugin(ns3::MeshWifiInterfaceMacPlugin const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshWifiInterfaceMacPlugin const &', 'arg0')])
    ## mesh-wifi-interface-mac-plugin.h: bool ns3::MeshWifiInterfaceMacPlugin::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const & header) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const &', 'header')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h: void ns3::MeshWifiInterfaceMacPlugin::SetParent(ns3::Ptr<ns3::MeshWifiInterfaceMac> parent) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMac >', 'parent')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h: void ns3::MeshWifiInterfaceMacPlugin::UpdateBeacon(ns3::MeshWifiBeacon & beacon) const [member function]
    cls.add_method('UpdateBeacon', 
                   'void', 
                   [param('ns3::MeshWifiBeacon &', 'beacon')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h: bool ns3::MeshWifiInterfaceMacPlugin::UpdateOutcomingFrame(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader & header, ns3::Mac48Address from, ns3::Mac48Address to) [member function]
    cls.add_method('UpdateOutcomingFrame', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader &', 'header'), param('ns3::Mac48Address', 'from'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3Dot11sStack_methods(root_module, cls):
    ## dot11s-installer.h: ns3::Dot11sStack::Dot11sStack(ns3::Dot11sStack const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Dot11sStack const &', 'arg0')])
    ## dot11s-installer.h: ns3::Dot11sStack::Dot11sStack() [constructor]
    cls.add_constructor([])
    ## dot11s-installer.h: void ns3::Dot11sStack::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## dot11s-installer.h: static ns3::TypeId ns3::Dot11sStack::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dot11s-installer.h: bool ns3::Dot11sStack::InstallStack(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('InstallStack', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')], 
                   is_virtual=True)
    ## dot11s-installer.h: void ns3::Dot11sStack::Report(ns3::Ptr<ns3::MeshPointDevice> const mp, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp'), param('std::ostream &', 'arg1')], 
                   is_virtual=True)
    ## dot11s-installer.h: void ns3::Dot11sStack::ResetStats(ns3::Ptr<ns3::MeshPointDevice> const mp) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp')], 
                   is_virtual=True)
    return

def register_Ns3FlameStack_methods(root_module, cls):
    ## flame-installer.h: ns3::FlameStack::FlameStack(ns3::FlameStack const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlameStack const &', 'arg0')])
    ## flame-installer.h: ns3::FlameStack::FlameStack() [constructor]
    cls.add_constructor([])
    ## flame-installer.h: void ns3::FlameStack::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-installer.h: static ns3::TypeId ns3::FlameStack::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-installer.h: bool ns3::FlameStack::InstallStack(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('InstallStack', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')], 
                   is_virtual=True)
    ## flame-installer.h: void ns3::FlameStack::Report(ns3::Ptr<ns3::MeshPointDevice> const mp, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp'), param('std::ostream &', 'arg1')], 
                   is_virtual=True)
    ## flame-installer.h: void ns3::FlameStack::ResetStats(ns3::Ptr<ns3::MeshPointDevice> const mp) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp')], 
                   is_virtual=True)
    return

def register_Ns3MeshPointDevice_methods(root_module, cls):
    ## mesh-point-device.h: ns3::MeshPointDevice::MeshPointDevice(ns3::MeshPointDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshPointDevice const &', 'arg0')])
    ## mesh-point-device.h: ns3::MeshPointDevice::MeshPointDevice() [constructor]
    cls.add_constructor([])
    ## mesh-point-device.h: void ns3::MeshPointDevice::AddInterface(ns3::Ptr<ns3::NetDevice> port) [member function]
    cls.add_method('AddInterface', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'port')])
    ## mesh-point-device.h: void ns3::MeshPointDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## mesh-point-device.h: ns3::Address ns3::MeshPointDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Address ns3::MeshPointDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Ptr<ns3::Channel> ns3::MeshPointDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: uint32_t ns3::MeshPointDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Ptr<ns3::NetDevice> ns3::MeshPointDevice::GetInterface(uint32_t id) const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'id')], 
                   is_const=True)
    ## mesh-point-device.h: std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > ns3::MeshPointDevice::GetInterfaces() const [member function]
    cls.add_method('GetInterfaces', 
                   'std::vector< ns3::Ptr< ns3::NetDevice > >', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h: uint16_t ns3::MeshPointDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Address ns3::MeshPointDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Address ns3::MeshPointDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: uint32_t ns3::MeshPointDevice::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h: ns3::Ptr<ns3::Node> ns3::MeshPointDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Ptr<ns3::MeshL2RoutingProtocol> ns3::MeshPointDevice::GetRoutingProtocol() const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::MeshL2RoutingProtocol >', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h: static ns3::TypeId ns3::MeshPointDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::Report(std::ostream & os) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## mesh-point-device.h: bool ns3::MeshPointDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetAddress(ns3::Address a) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'a')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetRoutingProtocol(ns3::Ptr<ns3::MeshL2RoutingProtocol> protocol) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshL2RoutingProtocol >', 'protocol')])
    ## mesh-point-device.h: bool ns3::MeshPointDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3MeshWifiInterfaceMac_methods(root_module, cls):
    ## mesh-wifi-interface-mac.h: ns3::MeshWifiInterfaceMac::MeshWifiInterfaceMac() [constructor]
    cls.add_constructor([])
    ## mesh-wifi-interface-mac.h: bool ns3::MeshWifiInterfaceMac::CheckSupportedRates(ns3::SupportedRates rates) const [member function]
    cls.add_method('CheckSupportedRates', 
                   'bool', 
                   [param('ns3::SupportedRates', 'rates')], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::FinishConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('FinishConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: uint16_t ns3::MeshWifiInterfaceMac::GetFrequencyChannel() const [member function]
    cls.add_method('GetFrequencyChannel', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: uint32_t ns3::MeshWifiInterfaceMac::GetLinkMetric(ns3::Mac48Address peerAddress) [member function]
    cls.add_method('GetLinkMetric', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'peerAddress')])
    ## mesh-wifi-interface-mac.h: ns3::Mac48Address ns3::MeshWifiInterfaceMac::GetMeshPointAddress() const [member function]
    cls.add_method('GetMeshPointAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: ns3::WifiPhyStandard ns3::MeshWifiInterfaceMac::GetPhyStandard() const [member function]
    cls.add_method('GetPhyStandard', 
                   'ns3::WifiPhyStandard', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: ns3::SupportedRates ns3::MeshWifiInterfaceMac::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetTbtt() const [member function]
    cls.add_method('GetTbtt', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: static ns3::TypeId ns3::MeshWifiInterfaceMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::InstallPlugin(ns3::Ptr<ns3::MeshWifiInterfaceMacPlugin> plugin) [member function]
    cls.add_method('InstallPlugin', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMacPlugin >', 'plugin')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SendManagementFrame(ns3::Ptr<ns3::Packet> frame, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('SendManagementFrame', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'frame'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetBeaconGeneration(bool enable) [member function]
    cls.add_method('SetBeaconGeneration', 
                   'void', 
                   [param('bool', 'enable')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetBeaconInterval(ns3::Time interval) [member function]
    cls.add_method('SetBeaconInterval', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetLinkMetricCallback(ns3::Callback<unsigned int, ns3::Mac48Address, ns3::Ptr<ns3::MeshWifiInterfaceMac>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetLinkMetricCallback', 
                   'void', 
                   [param('ns3::Callback< unsigned int, ns3::Mac48Address, ns3::Ptr< ns3::MeshWifiInterfaceMac >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetMeshPointAddress(ns3::Mac48Address arg0) [member function]
    cls.add_method('SetMeshPointAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'arg0')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetRandomStartDelay(ns3::Time interval) [member function]
    cls.add_method('SetRandomStartDelay', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::ShiftTbtt(ns3::Time shift) [member function]
    cls.add_method('ShiftTbtt', 
                   'void', 
                   [param('ns3::Time', 'shift')])
    ## mesh-wifi-interface-mac.h: bool ns3::MeshWifiInterfaceMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SwitchFrequencyChannel(uint16_t new_id) [member function]
    cls.add_method('SwitchFrequencyChannel', 
                   'void', 
                   [param('uint16_t', 'new_id')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Dot11sDestinationAddressUnit_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-preq.h: ns3::dot11s::DestinationAddressUnit::DestinationAddressUnit(ns3::dot11s::DestinationAddressUnit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::DestinationAddressUnit const &', 'arg0')])
    ## ie-dot11s-preq.h: ns3::dot11s::DestinationAddressUnit::DestinationAddressUnit() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-preq.h: uint32_t ns3::dot11s::DestinationAddressUnit::GetDestSeqNumber() const [member function]
    cls.add_method('GetDestSeqNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: ns3::Mac48Address ns3::dot11s::DestinationAddressUnit::GetDestinationAddress() const [member function]
    cls.add_method('GetDestinationAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: bool ns3::dot11s::DestinationAddressUnit::IsDo() [member function]
    cls.add_method('IsDo', 
                   'bool', 
                   [])
    ## ie-dot11s-preq.h: bool ns3::dot11s::DestinationAddressUnit::IsRf() [member function]
    cls.add_method('IsRf', 
                   'bool', 
                   [])
    ## ie-dot11s-preq.h: bool ns3::dot11s::DestinationAddressUnit::IsUsn() [member function]
    cls.add_method('IsUsn', 
                   'bool', 
                   [])
    ## ie-dot11s-preq.h: void ns3::dot11s::DestinationAddressUnit::SetDestSeqNumber(uint32_t dest_seq_number) [member function]
    cls.add_method('SetDestSeqNumber', 
                   'void', 
                   [param('uint32_t', 'dest_seq_number')])
    ## ie-dot11s-preq.h: void ns3::dot11s::DestinationAddressUnit::SetDestinationAddress(ns3::Mac48Address dest_address) [member function]
    cls.add_method('SetDestinationAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'dest_address')])
    ## ie-dot11s-preq.h: void ns3::dot11s::DestinationAddressUnit::SetFlags(bool doFlag, bool rfFlag, bool usnFlag) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('bool', 'doFlag'), param('bool', 'rfFlag'), param('bool', 'usnFlag')])
    return

def register_Ns3Dot11sDot11sMeshCapability_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::Dot11sMeshCapability(ns3::dot11s::Dot11sMeshCapability const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::Dot11sMeshCapability const &', 'arg0')])
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::Dot11sMeshCapability() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-configuration.h: ns3::Buffer::Iterator ns3::dot11s::Dot11sMeshCapability::Deserialize(ns3::Buffer::Iterator i) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')])
    ## ie-dot11s-configuration.h: uint8_t ns3::dot11s::Dot11sMeshCapability::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-configuration.h: uint16_t ns3::dot11s::Dot11sMeshCapability::GetUint16() const [member function]
    cls.add_method('GetUint16', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-configuration.h: bool ns3::dot11s::Dot11sMeshCapability::Is(uint16_t cap, uint8_t n) const [member function]
    cls.add_method('Is', 
                   'bool', 
                   [param('uint16_t', 'cap'), param('uint8_t', 'n')], 
                   is_const=True)
    ## ie-dot11s-configuration.h: ns3::Buffer::Iterator ns3::dot11s::Dot11sMeshCapability::Serialize(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True)
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::MCCAEnabled [variable]
    cls.add_instance_attribute('MCCAEnabled', 'bool', is_const=False)
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::MCCASupported [variable]
    cls.add_instance_attribute('MCCASupported', 'bool', is_const=False)
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::TBTTAdjustment [variable]
    cls.add_instance_attribute('TBTTAdjustment', 'bool', is_const=False)
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::acceptPeerLinks [variable]
    cls.add_instance_attribute('acceptPeerLinks', 'bool', is_const=False)
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::beaconTimingReport [variable]
    cls.add_instance_attribute('beaconTimingReport', 'bool', is_const=False)
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::forwarding [variable]
    cls.add_instance_attribute('forwarding', 'bool', is_const=False)
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability::powerSaveLevel [variable]
    cls.add_instance_attribute('powerSaveLevel', 'bool', is_const=False)
    return

def register_Ns3Dot11sHwmpProtocol_methods(root_module, cls):
    ## hwmp-protocol.h: ns3::dot11s::HwmpProtocol::HwmpProtocol() [constructor]
    cls.add_constructor([])
    ## hwmp-protocol.h: void ns3::dot11s::HwmpProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## hwmp-protocol.h: static ns3::TypeId ns3::dot11s::HwmpProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## hwmp-protocol.h: bool ns3::dot11s::HwmpProtocol::Install(ns3::Ptr<ns3::MeshPointDevice> arg0) [member function]
    cls.add_method('Install', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'arg0')])
    ## hwmp-protocol.h: void ns3::dot11s::HwmpProtocol::PeerLinkStatus(ns3::Mac48Address meshPontAddress, ns3::Mac48Address peerAddress, uint32_t interface, bool status) [member function]
    cls.add_method('PeerLinkStatus', 
                   'void', 
                   [param('ns3::Mac48Address', 'meshPontAddress'), param('ns3::Mac48Address', 'peerAddress'), param('uint32_t', 'interface'), param('bool', 'status')])
    ## hwmp-protocol.h: bool ns3::dot11s::HwmpProtocol::RemoveRoutingStuff(uint32_t fromIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet> packet, uint16_t & protocolType) [member function]
    cls.add_method('RemoveRoutingStuff', 
                   'bool', 
                   [param('uint32_t', 'fromIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t &', 'protocolType')], 
                   is_virtual=True)
    ## hwmp-protocol.h: void ns3::dot11s::HwmpProtocol::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## hwmp-protocol.h: bool ns3::dot11s::HwmpProtocol::RequestRoute(uint32_t sourceIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet const> packet, uint16_t protocolType, ns3::Callback<void, bool, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'sourceIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocolType'), param('ns3::Callback< void, bool, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_virtual=True)
    ## hwmp-protocol.h: void ns3::dot11s::HwmpProtocol::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## hwmp-protocol.h: void ns3::dot11s::HwmpProtocol::SetNeighboursCallback(ns3::Callback<std::vector<ns3::Mac48Address, std::allocator<ns3::Mac48Address> >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetNeighboursCallback', 
                   'void', 
                   [param('ns3::Callback< std::vector< ns3::Mac48Address >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## hwmp-protocol.h: void ns3::dot11s::HwmpProtocol::SetRoot() [member function]
    cls.add_method('SetRoot', 
                   'void', 
                   [])
    ## hwmp-protocol.h: void ns3::dot11s::HwmpProtocol::UnsetRoot() [member function]
    cls.add_method('UnsetRoot', 
                   'void', 
                   [])
    return

def register_Ns3Dot11sHwmpProtocolFailedDestination_methods(root_module, cls):
    ## hwmp-protocol.h: ns3::dot11s::HwmpProtocol::FailedDestination::FailedDestination() [constructor]
    cls.add_constructor([])
    ## hwmp-protocol.h: ns3::dot11s::HwmpProtocol::FailedDestination::FailedDestination(ns3::dot11s::HwmpProtocol::FailedDestination const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::HwmpProtocol::FailedDestination const &', 'arg0')])
    ## hwmp-protocol.h: ns3::dot11s::HwmpProtocol::FailedDestination::destination [variable]
    cls.add_instance_attribute('destination', 'ns3::Mac48Address', is_const=False)
    ## hwmp-protocol.h: ns3::dot11s::HwmpProtocol::FailedDestination::seqnum [variable]
    cls.add_instance_attribute('seqnum', 'uint32_t', is_const=False)
    return

def register_Ns3Dot11sHwmpRtable_methods(root_module, cls):
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::HwmpRtable(ns3::dot11s::HwmpRtable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::HwmpRtable const &', 'arg0')])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::HwmpRtable() [constructor]
    cls.add_constructor([])
    ## hwmp-rtable.h: void ns3::dot11s::HwmpRtable::AddPrecursor(ns3::Mac48Address destination, uint32_t precursorInterface, ns3::Mac48Address precursorAddress, ns3::Time lifetime) [member function]
    cls.add_method('AddPrecursor', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination'), param('uint32_t', 'precursorInterface'), param('ns3::Mac48Address', 'precursorAddress'), param('ns3::Time', 'lifetime')])
    ## hwmp-rtable.h: void ns3::dot11s::HwmpRtable::AddProactivePath(uint32_t metric, ns3::Mac48Address root, ns3::Mac48Address retransmitter, uint32_t interface, ns3::Time lifetime, uint32_t seqnum) [member function]
    cls.add_method('AddProactivePath', 
                   'void', 
                   [param('uint32_t', 'metric'), param('ns3::Mac48Address', 'root'), param('ns3::Mac48Address', 'retransmitter'), param('uint32_t', 'interface'), param('ns3::Time', 'lifetime'), param('uint32_t', 'seqnum')])
    ## hwmp-rtable.h: void ns3::dot11s::HwmpRtable::AddReactivePath(ns3::Mac48Address destination, ns3::Mac48Address retransmitter, uint32_t interface, uint32_t metric, ns3::Time lifetime, uint32_t seqnum) [member function]
    cls.add_method('AddReactivePath', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination'), param('ns3::Mac48Address', 'retransmitter'), param('uint32_t', 'interface'), param('uint32_t', 'metric'), param('ns3::Time', 'lifetime'), param('uint32_t', 'seqnum')])
    ## hwmp-rtable.h: void ns3::dot11s::HwmpRtable::DeleteProactivePath() [member function]
    cls.add_method('DeleteProactivePath', 
                   'void', 
                   [])
    ## hwmp-rtable.h: void ns3::dot11s::HwmpRtable::DeleteProactivePath(ns3::Mac48Address root) [member function]
    cls.add_method('DeleteProactivePath', 
                   'void', 
                   [param('ns3::Mac48Address', 'root')])
    ## hwmp-rtable.h: void ns3::dot11s::HwmpRtable::DeleteReactivePath(ns3::Mac48Address destination) [member function]
    cls.add_method('DeleteReactivePath', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination')])
    ## hwmp-rtable.h: void ns3::dot11s::HwmpRtable::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## hwmp-rtable.h: std::vector<std::pair<unsigned int, ns3::Mac48Address>, std::allocator<std::pair<unsigned int, ns3::Mac48Address> > > ns3::dot11s::HwmpRtable::GetPrecursors(ns3::Mac48Address destination) [member function]
    cls.add_method('GetPrecursors', 
                   'std::vector< std::pair< unsigned int, ns3::Mac48Address > >', 
                   [param('ns3::Mac48Address', 'destination')])
    ## hwmp-rtable.h: static ns3::TypeId ns3::dot11s::HwmpRtable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## hwmp-rtable.h: std::vector<ns3::dot11s::HwmpProtocol::FailedDestination, std::allocator<ns3::dot11s::HwmpProtocol::FailedDestination> > ns3::dot11s::HwmpRtable::GetUnreachableDestinations(ns3::Mac48Address peerAddress) [member function]
    cls.add_method('GetUnreachableDestinations', 
                   'std::vector< ns3::dot11s::HwmpProtocol::FailedDestination >', 
                   [param('ns3::Mac48Address', 'peerAddress')])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult ns3::dot11s::HwmpRtable::LookupProactive() [member function]
    cls.add_method('LookupProactive', 
                   'ns3::dot11s::HwmpRtable::LookupResult', 
                   [])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult ns3::dot11s::HwmpRtable::LookupProactiveExpired() [member function]
    cls.add_method('LookupProactiveExpired', 
                   'ns3::dot11s::HwmpRtable::LookupResult', 
                   [])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult ns3::dot11s::HwmpRtable::LookupReactive(ns3::Mac48Address destination) [member function]
    cls.add_method('LookupReactive', 
                   'ns3::dot11s::HwmpRtable::LookupResult', 
                   [param('ns3::Mac48Address', 'destination')])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult ns3::dot11s::HwmpRtable::LookupReactiveExpired(ns3::Mac48Address destination) [member function]
    cls.add_method('LookupReactiveExpired', 
                   'ns3::dot11s::HwmpRtable::LookupResult', 
                   [param('ns3::Mac48Address', 'destination')])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::INTERFACE_ANY [variable]
    cls.add_static_attribute('INTERFACE_ANY', 'uint32_t const', is_const=True)
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::MAX_METRIC [variable]
    cls.add_static_attribute('MAX_METRIC', 'uint32_t const', is_const=True)
    return

def register_Ns3Dot11sHwmpRtableLookupResult_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult::LookupResult(ns3::dot11s::HwmpRtable::LookupResult const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::HwmpRtable::LookupResult const &', 'arg0')])
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult::LookupResult(ns3::Mac48Address r=ns3::Mac48Address::GetBroadcast(), uint32_t i=ns3::dot11s::HwmpRtable::INTERFACE_ANY, uint32_t m=ns3::dot11s::HwmpRtable::MAX_METRIC, uint32_t s=0, ns3::Time l=ns3::Seconds( )) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'r', default_value='ns3::Mac48Address::GetBroadcast()'), param('uint32_t', 'i', default_value='ns3::dot11s::HwmpRtable::INTERFACE_ANY'), param('uint32_t', 'm', default_value='ns3::dot11s::HwmpRtable::MAX_METRIC'), param('uint32_t', 's', default_value='0'), param('ns3::Time', 'l', default_value='ns3::Seconds(0)')])
    ## hwmp-rtable.h: bool ns3::dot11s::HwmpRtable::LookupResult::IsValid() const [member function]
    cls.add_method('IsValid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult::ifIndex [variable]
    cls.add_instance_attribute('ifIndex', 'uint32_t', is_const=False)
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult::lifetime [variable]
    cls.add_instance_attribute('lifetime', 'ns3::Time', is_const=False)
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult::metric [variable]
    cls.add_instance_attribute('metric', 'uint32_t', is_const=False)
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult::retransmitter [variable]
    cls.add_instance_attribute('retransmitter', 'ns3::Mac48Address', is_const=False)
    ## hwmp-rtable.h: ns3::dot11s::HwmpRtable::LookupResult::seqnum [variable]
    cls.add_instance_attribute('seqnum', 'uint32_t', is_const=False)
    return

def register_Ns3Dot11sIeBeaconTiming_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ie-dot11s-beacon-timing.h: ns3::dot11s::IeBeaconTiming::IeBeaconTiming(ns3::dot11s::IeBeaconTiming const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeBeaconTiming const &', 'arg0')])
    ## ie-dot11s-beacon-timing.h: ns3::dot11s::IeBeaconTiming::IeBeaconTiming() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-beacon-timing.h: void ns3::dot11s::IeBeaconTiming::AddNeighboursTimingElementUnit(uint16_t aid, ns3::Time last_beacon, ns3::Time beacon_interval) [member function]
    cls.add_method('AddNeighboursTimingElementUnit', 
                   'void', 
                   [param('uint16_t', 'aid'), param('ns3::Time', 'last_beacon'), param('ns3::Time', 'beacon_interval')])
    ## ie-dot11s-beacon-timing.h: void ns3::dot11s::IeBeaconTiming::ClearTimingElement() [member function]
    cls.add_method('ClearTimingElement', 
                   'void', 
                   [])
    ## ie-dot11s-beacon-timing.h: void ns3::dot11s::IeBeaconTiming::DelNeighboursTimingElementUnit(uint16_t aid, ns3::Time last_beacon, ns3::Time beacon_interval) [member function]
    cls.add_method('DelNeighboursTimingElementUnit', 
                   'void', 
                   [param('uint16_t', 'aid'), param('ns3::Time', 'last_beacon'), param('ns3::Time', 'beacon_interval')])
    ## ie-dot11s-beacon-timing.h: uint8_t ns3::dot11s::IeBeaconTiming::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-beacon-timing.h: ns3::WifiInformationElementId ns3::dot11s::IeBeaconTiming::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-beacon-timing.h: uint8_t ns3::dot11s::IeBeaconTiming::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-beacon-timing.h: std::vector<ns3::Ptr<ns3::dot11s::IeBeaconTimingUnit>, std::allocator<ns3::Ptr<ns3::dot11s::IeBeaconTimingUnit> > > ns3::dot11s::IeBeaconTiming::GetNeighboursTimingElementsList() [member function]
    cls.add_method('GetNeighboursTimingElementsList', 
                   'std::vector< ns3::Ptr< ns3::dot11s::IeBeaconTimingUnit > >', 
                   [])
    ## ie-dot11s-beacon-timing.h: void ns3::dot11s::IeBeaconTiming::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-beacon-timing.h: void ns3::dot11s::IeBeaconTiming::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Dot11sIeBeaconTimingUnit_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-beacon-timing.h: ns3::dot11s::IeBeaconTimingUnit::IeBeaconTimingUnit(ns3::dot11s::IeBeaconTimingUnit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeBeaconTimingUnit const &', 'arg0')])
    ## ie-dot11s-beacon-timing.h: ns3::dot11s::IeBeaconTimingUnit::IeBeaconTimingUnit() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-beacon-timing.h: uint8_t ns3::dot11s::IeBeaconTimingUnit::GetAid() const [member function]
    cls.add_method('GetAid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-beacon-timing.h: uint16_t ns3::dot11s::IeBeaconTimingUnit::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-beacon-timing.h: uint16_t ns3::dot11s::IeBeaconTimingUnit::GetLastBeacon() const [member function]
    cls.add_method('GetLastBeacon', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-beacon-timing.h: void ns3::dot11s::IeBeaconTimingUnit::SetAid(uint8_t aid) [member function]
    cls.add_method('SetAid', 
                   'void', 
                   [param('uint8_t', 'aid')])
    ## ie-dot11s-beacon-timing.h: void ns3::dot11s::IeBeaconTimingUnit::SetBeaconInterval(uint16_t beaconInterval) [member function]
    cls.add_method('SetBeaconInterval', 
                   'void', 
                   [param('uint16_t', 'beaconInterval')])
    ## ie-dot11s-beacon-timing.h: void ns3::dot11s::IeBeaconTimingUnit::SetLastBeacon(uint16_t lastBeacon) [member function]
    cls.add_method('SetLastBeacon', 
                   'void', 
                   [param('uint16_t', 'lastBeacon')])
    return

def register_Ns3Dot11sIeConfiguration_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-configuration.h: ns3::dot11s::IeConfiguration::IeConfiguration(ns3::dot11s::IeConfiguration const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeConfiguration const &', 'arg0')])
    ## ie-dot11s-configuration.h: ns3::dot11s::IeConfiguration::IeConfiguration() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-configuration.h: uint8_t ns3::dot11s::IeConfiguration::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-configuration.h: ns3::WifiInformationElementId ns3::dot11s::IeConfiguration::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-configuration.h: uint8_t ns3::dot11s::IeConfiguration::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-configuration.h: uint8_t ns3::dot11s::IeConfiguration::GetNeighborCount() [member function]
    cls.add_method('GetNeighborCount', 
                   'uint8_t', 
                   [])
    ## ie-dot11s-configuration.h: bool ns3::dot11s::IeConfiguration::IsAirtime() [member function]
    cls.add_method('IsAirtime', 
                   'bool', 
                   [])
    ## ie-dot11s-configuration.h: bool ns3::dot11s::IeConfiguration::IsHWMP() [member function]
    cls.add_method('IsHWMP', 
                   'bool', 
                   [])
    ## ie-dot11s-configuration.h: ns3::dot11s::Dot11sMeshCapability const & ns3::dot11s::IeConfiguration::MeshCapability() [member function]
    cls.add_method('MeshCapability', 
                   'ns3::dot11s::Dot11sMeshCapability const &', 
                   [])
    ## ie-dot11s-configuration.h: void ns3::dot11s::IeConfiguration::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-configuration.h: void ns3::dot11s::IeConfiguration::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-configuration.h: void ns3::dot11s::IeConfiguration::SetMetric(ns3::dot11s::dot11sPathSelectionMetric metricId) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('ns3::dot11s::dot11sPathSelectionMetric', 'metricId')])
    ## ie-dot11s-configuration.h: void ns3::dot11s::IeConfiguration::SetNeighborCount(uint8_t neighbors) [member function]
    cls.add_method('SetNeighborCount', 
                   'void', 
                   [param('uint8_t', 'neighbors')])
    ## ie-dot11s-configuration.h: void ns3::dot11s::IeConfiguration::SetRouting(ns3::dot11s::dot11sPathSelectionProtocol routingId) [member function]
    cls.add_method('SetRouting', 
                   'void', 
                   [param('ns3::dot11s::dot11sPathSelectionProtocol', 'routingId')])
    return

def register_Ns3Dot11sIeLinkMetricReport_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    ## ie-dot11s-metric-report.h: ns3::dot11s::IeLinkMetricReport::IeLinkMetricReport(ns3::dot11s::IeLinkMetricReport const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeLinkMetricReport const &', 'arg0')])
    ## ie-dot11s-metric-report.h: ns3::dot11s::IeLinkMetricReport::IeLinkMetricReport() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-metric-report.h: ns3::dot11s::IeLinkMetricReport::IeLinkMetricReport(uint32_t metric) [constructor]
    cls.add_constructor([param('uint32_t', 'metric')])
    ## ie-dot11s-metric-report.h: uint8_t ns3::dot11s::IeLinkMetricReport::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-metric-report.h: ns3::WifiInformationElementId ns3::dot11s::IeLinkMetricReport::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-metric-report.h: uint8_t ns3::dot11s::IeLinkMetricReport::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-metric-report.h: uint32_t ns3::dot11s::IeLinkMetricReport::GetMetric() [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [])
    ## ie-dot11s-metric-report.h: void ns3::dot11s::IeLinkMetricReport::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-metric-report.h: void ns3::dot11s::IeLinkMetricReport::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-metric-report.h: void ns3::dot11s::IeLinkMetricReport::SetMetric(uint32_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    return

def register_Ns3Dot11sIeMeshId_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshId::IeMeshId(ns3::dot11s::IeMeshId const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeMeshId const &', 'arg0')])
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshId::IeMeshId() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshId::IeMeshId(std::string s) [constructor]
    cls.add_constructor([param('std::string', 's')])
    ## ie-dot11s-id.h: uint8_t ns3::dot11s::IeMeshId::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-id.h: ns3::WifiInformationElementId ns3::dot11s::IeMeshId::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h: uint8_t ns3::dot11s::IeMeshId::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h: bool ns3::dot11s::IeMeshId::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-id.h: bool ns3::dot11s::IeMeshId::IsEqual(ns3::dot11s::IeMeshId const & o) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::dot11s::IeMeshId const &', 'o')], 
                   is_const=True)
    ## ie-dot11s-id.h: char * ns3::dot11s::IeMeshId::PeekString() const [member function]
    cls.add_method('PeekString', 
                   'char *', 
                   [], 
                   is_const=True)
    ## ie-dot11s-id.h: void ns3::dot11s::IeMeshId::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h: void ns3::dot11s::IeMeshId::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Dot11sIeMeshIdChecker_methods(root_module, cls):
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshIdChecker::IeMeshIdChecker() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshIdChecker::IeMeshIdChecker(ns3::dot11s::IeMeshIdChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeMeshIdChecker const &', 'arg0')])
    return

def register_Ns3Dot11sIeMeshIdValue_methods(root_module, cls):
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshIdValue::IeMeshIdValue() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshIdValue::IeMeshIdValue(ns3::dot11s::IeMeshIdValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeMeshIdValue const &', 'arg0')])
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshIdValue::IeMeshIdValue(ns3::dot11s::IeMeshId const & value) [constructor]
    cls.add_constructor([param('ns3::dot11s::IeMeshId const &', 'value')])
    ## ie-dot11s-id.h: ns3::Ptr<ns3::AttributeValue> ns3::dot11s::IeMeshIdValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h: bool ns3::dot11s::IeMeshIdValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ie-dot11s-id.h: ns3::dot11s::IeMeshId ns3::dot11s::IeMeshIdValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::dot11s::IeMeshId', 
                   [], 
                   is_const=True)
    ## ie-dot11s-id.h: std::string ns3::dot11s::IeMeshIdValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h: void ns3::dot11s::IeMeshIdValue::Set(ns3::dot11s::IeMeshId const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::dot11s::IeMeshId const &', 'value')])
    return

def register_Ns3Dot11sIePeerManagement_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-peer-management.h: ns3::dot11s::IePeerManagement::IePeerManagement(ns3::dot11s::IePeerManagement const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePeerManagement const &', 'arg0')])
    ## ie-dot11s-peer-management.h: ns3::dot11s::IePeerManagement::IePeerManagement() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-peer-management.h: uint8_t ns3::dot11s::IePeerManagement::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-peer-management.h: ns3::WifiInformationElementId ns3::dot11s::IePeerManagement::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peer-management.h: uint8_t ns3::dot11s::IePeerManagement::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peer-management.h: uint16_t ns3::dot11s::IePeerManagement::GetLocalLinkId() const [member function]
    cls.add_method('GetLocalLinkId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h: uint16_t ns3::dot11s::IePeerManagement::GetPeerLinkId() const [member function]
    cls.add_method('GetPeerLinkId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h: ns3::dot11s::PmpReasonCode ns3::dot11s::IePeerManagement::GetReasonCode() const [member function]
    cls.add_method('GetReasonCode', 
                   'ns3::dot11s::PmpReasonCode', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h: uint8_t ns3::dot11s::IePeerManagement::GetSubtype() const [member function]
    cls.add_method('GetSubtype', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h: void ns3::dot11s::IePeerManagement::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peer-management.h: void ns3::dot11s::IePeerManagement::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peer-management.h: void ns3::dot11s::IePeerManagement::SetPeerClose(uint16_t localLinkID, uint16_t peerLinkId, ns3::dot11s::PmpReasonCode reasonCode) [member function]
    cls.add_method('SetPeerClose', 
                   'void', 
                   [param('uint16_t', 'localLinkID'), param('uint16_t', 'peerLinkId'), param('ns3::dot11s::PmpReasonCode', 'reasonCode')])
    ## ie-dot11s-peer-management.h: void ns3::dot11s::IePeerManagement::SetPeerConfirm(uint16_t localLinkID, uint16_t peerLinkId) [member function]
    cls.add_method('SetPeerConfirm', 
                   'void', 
                   [param('uint16_t', 'localLinkID'), param('uint16_t', 'peerLinkId')])
    ## ie-dot11s-peer-management.h: void ns3::dot11s::IePeerManagement::SetPeerOpen(uint16_t localLinkId) [member function]
    cls.add_method('SetPeerOpen', 
                   'void', 
                   [param('uint16_t', 'localLinkId')])
    ## ie-dot11s-peer-management.h: bool ns3::dot11s::IePeerManagement::SubtypeIsClose() const [member function]
    cls.add_method('SubtypeIsClose', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h: bool ns3::dot11s::IePeerManagement::SubtypeIsConfirm() const [member function]
    cls.add_method('SubtypeIsConfirm', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h: bool ns3::dot11s::IePeerManagement::SubtypeIsOpen() const [member function]
    cls.add_method('SubtypeIsOpen', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3Dot11sIePeeringProtocol_methods(root_module, cls):
    ## ie-dot11s-peering-protocol.h: ns3::dot11s::IePeeringProtocol::IePeeringProtocol(ns3::dot11s::IePeeringProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePeeringProtocol const &', 'arg0')])
    ## ie-dot11s-peering-protocol.h: ns3::dot11s::IePeeringProtocol::IePeeringProtocol() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-peering-protocol.h: uint8_t ns3::dot11s::IePeeringProtocol::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-peering-protocol.h: ns3::WifiInformationElementId ns3::dot11s::IePeeringProtocol::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peering-protocol.h: uint8_t ns3::dot11s::IePeeringProtocol::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peering-protocol.h: void ns3::dot11s::IePeeringProtocol::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peering-protocol.h: void ns3::dot11s::IePeeringProtocol::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Dot11sIePerr_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-perr.h: ns3::dot11s::IePerr::IePerr(ns3::dot11s::IePerr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePerr const &', 'arg0')])
    ## ie-dot11s-perr.h: ns3::dot11s::IePerr::IePerr() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-perr.h: void ns3::dot11s::IePerr::AddAddressUnit(ns3::dot11s::HwmpProtocol::FailedDestination unit) [member function]
    cls.add_method('AddAddressUnit', 
                   'void', 
                   [param('ns3::dot11s::HwmpProtocol::FailedDestination', 'unit')])
    ## ie-dot11s-perr.h: void ns3::dot11s::IePerr::DeleteAddressUnit(ns3::Mac48Address address) [member function]
    cls.add_method('DeleteAddressUnit', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## ie-dot11s-perr.h: uint8_t ns3::dot11s::IePerr::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-perr.h: ns3::WifiInformationElementId ns3::dot11s::IePerr::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-perr.h: std::vector<ns3::dot11s::HwmpProtocol::FailedDestination, std::allocator<ns3::dot11s::HwmpProtocol::FailedDestination> > ns3::dot11s::IePerr::GetAddressUnitVector() const [member function]
    cls.add_method('GetAddressUnitVector', 
                   'std::vector< ns3::dot11s::HwmpProtocol::FailedDestination >', 
                   [], 
                   is_const=True)
    ## ie-dot11s-perr.h: uint8_t ns3::dot11s::IePerr::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-perr.h: uint8_t ns3::dot11s::IePerr::GetNumOfDest() const [member function]
    cls.add_method('GetNumOfDest', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-perr.h: bool ns3::dot11s::IePerr::IsFull() const [member function]
    cls.add_method('IsFull', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-perr.h: void ns3::dot11s::IePerr::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-perr.h: void ns3::dot11s::IePerr::ResetPerr() [member function]
    cls.add_method('ResetPerr', 
                   'void', 
                   [])
    ## ie-dot11s-perr.h: void ns3::dot11s::IePerr::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Dot11sIePrep_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-prep.h: ns3::dot11s::IePrep::IePrep(ns3::dot11s::IePrep const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePrep const &', 'arg0')])
    ## ie-dot11s-prep.h: ns3::dot11s::IePrep::IePrep() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::DecrementTtl() [member function]
    cls.add_method('DecrementTtl', 
                   'void', 
                   [])
    ## ie-dot11s-prep.h: uint8_t ns3::dot11s::IePrep::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-prep.h: ns3::WifiInformationElementId ns3::dot11s::IePrep::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-prep.h: ns3::Mac48Address ns3::dot11s::IePrep::GetDestinationAddress() const [member function]
    cls.add_method('GetDestinationAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: uint32_t ns3::dot11s::IePrep::GetDestinationSeqNumber() const [member function]
    cls.add_method('GetDestinationSeqNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: uint8_t ns3::dot11s::IePrep::GetFlags() const [member function]
    cls.add_method('GetFlags', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: uint8_t ns3::dot11s::IePrep::GetHopcount() const [member function]
    cls.add_method('GetHopcount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: uint8_t ns3::dot11s::IePrep::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-prep.h: uint32_t ns3::dot11s::IePrep::GetLifetime() const [member function]
    cls.add_method('GetLifetime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: uint32_t ns3::dot11s::IePrep::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: ns3::Mac48Address ns3::dot11s::IePrep::GetOriginatorAddress() const [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: uint32_t ns3::dot11s::IePrep::GetOriginatorSeqNumber() const [member function]
    cls.add_method('GetOriginatorSeqNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: uint32_t ns3::dot11s::IePrep::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::IncrementMetric(uint32_t metric) [member function]
    cls.add_method('IncrementMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetDestinationAddress(ns3::Mac48Address dest_address) [member function]
    cls.add_method('SetDestinationAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'dest_address')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetDestinationSeqNumber(uint32_t dest_seq_number) [member function]
    cls.add_method('SetDestinationSeqNumber', 
                   'void', 
                   [param('uint32_t', 'dest_seq_number')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetFlags(uint8_t flags) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetHopcount(uint8_t hopcount) [member function]
    cls.add_method('SetHopcount', 
                   'void', 
                   [param('uint8_t', 'hopcount')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetLifetime(uint32_t lifetime) [member function]
    cls.add_method('SetLifetime', 
                   'void', 
                   [param('uint32_t', 'lifetime')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetMetric(uint32_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetOriginatorAddress(ns3::Mac48Address originator_address) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'originator_address')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetOriginatorSeqNumber(uint32_t originator_seq_number) [member function]
    cls.add_method('SetOriginatorSeqNumber', 
                   'void', 
                   [param('uint32_t', 'originator_seq_number')])
    ## ie-dot11s-prep.h: void ns3::dot11s::IePrep::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3Dot11sIePreq_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-preq.h: ns3::dot11s::IePreq::IePreq(ns3::dot11s::IePreq const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePreq const &', 'arg0')])
    ## ie-dot11s-preq.h: ns3::dot11s::IePreq::IePreq() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::AddDestinationAddressElement(bool doFlag, bool rfFlag, ns3::Mac48Address dest_address, uint32_t dest_seq_number) [member function]
    cls.add_method('AddDestinationAddressElement', 
                   'void', 
                   [param('bool', 'doFlag'), param('bool', 'rfFlag'), param('ns3::Mac48Address', 'dest_address'), param('uint32_t', 'dest_seq_number')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::ClearDestinationAddressElements() [member function]
    cls.add_method('ClearDestinationAddressElements', 
                   'void', 
                   [])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::DecrementTtl() [member function]
    cls.add_method('DecrementTtl', 
                   'void', 
                   [])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::DelDestinationAddressElement(ns3::Mac48Address dest_address) [member function]
    cls.add_method('DelDestinationAddressElement', 
                   'void', 
                   [param('ns3::Mac48Address', 'dest_address')])
    ## ie-dot11s-preq.h: uint8_t ns3::dot11s::IePreq::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-preq.h: ns3::WifiInformationElementId ns3::dot11s::IePreq::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-preq.h: uint8_t ns3::dot11s::IePreq::GetDestCount() const [member function]
    cls.add_method('GetDestCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: std::vector<ns3::Ptr<ns3::dot11s::DestinationAddressUnit>, std::allocator<ns3::Ptr<ns3::dot11s::DestinationAddressUnit> > > ns3::dot11s::IePreq::GetDestinationList() [member function]
    cls.add_method('GetDestinationList', 
                   'std::vector< ns3::Ptr< ns3::dot11s::DestinationAddressUnit > >', 
                   [])
    ## ie-dot11s-preq.h: uint8_t ns3::dot11s::IePreq::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: uint8_t ns3::dot11s::IePreq::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-preq.h: uint32_t ns3::dot11s::IePreq::GetLifetime() const [member function]
    cls.add_method('GetLifetime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: uint32_t ns3::dot11s::IePreq::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: ns3::Mac48Address ns3::dot11s::IePreq::GetOriginatorAddress() const [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: uint32_t ns3::dot11s::IePreq::GetOriginatorSeqNumber() const [member function]
    cls.add_method('GetOriginatorSeqNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: uint32_t ns3::dot11s::IePreq::GetPreqID() const [member function]
    cls.add_method('GetPreqID', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: uint8_t ns3::dot11s::IePreq::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::IncrementMetric(uint32_t metric) [member function]
    cls.add_method('IncrementMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-preq.h: bool ns3::dot11s::IePreq::IsFull() const [member function]
    cls.add_method('IsFull', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: bool ns3::dot11s::IePreq::IsNeedNotPrep() const [member function]
    cls.add_method('IsNeedNotPrep', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: bool ns3::dot11s::IePreq::IsUnicastPreq() const [member function]
    cls.add_method('IsUnicastPreq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h: bool ns3::dot11s::IePreq::MayAddAddress(ns3::Mac48Address originator) [member function]
    cls.add_method('MayAddAddress', 
                   'bool', 
                   [param('ns3::Mac48Address', 'originator')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetDestCount(uint8_t dest_count) [member function]
    cls.add_method('SetDestCount', 
                   'void', 
                   [param('uint8_t', 'dest_count')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetHopcount(uint8_t hopcount) [member function]
    cls.add_method('SetHopcount', 
                   'void', 
                   [param('uint8_t', 'hopcount')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetLifetime(uint32_t lifetime) [member function]
    cls.add_method('SetLifetime', 
                   'void', 
                   [param('uint32_t', 'lifetime')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetMetric(uint32_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetNeedNotPrep() [member function]
    cls.add_method('SetNeedNotPrep', 
                   'void', 
                   [])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetOriginatorAddress(ns3::Mac48Address originator_address) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'originator_address')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetOriginatorSeqNumber(uint32_t originator_seq_number) [member function]
    cls.add_method('SetOriginatorSeqNumber', 
                   'void', 
                   [param('uint32_t', 'originator_seq_number')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetPreqID(uint32_t id) [member function]
    cls.add_method('SetPreqID', 
                   'void', 
                   [param('uint32_t', 'id')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetTTL(uint8_t ttl) [member function]
    cls.add_method('SetTTL', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    ## ie-dot11s-preq.h: void ns3::dot11s::IePreq::SetUnicastPreq() [member function]
    cls.add_method('SetUnicastPreq', 
                   'void', 
                   [])
    return

def register_Ns3Dot11sIeRann_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-rann.h: ns3::dot11s::IeRann::IeRann(ns3::dot11s::IeRann const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeRann const &', 'arg0')])
    ## ie-dot11s-rann.h: ns3::dot11s::IeRann::IeRann() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::DecrementTtl() [member function]
    cls.add_method('DecrementTtl', 
                   'void', 
                   [])
    ## ie-dot11s-rann.h: uint8_t ns3::dot11s::IeRann::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-rann.h: ns3::WifiInformationElementId ns3::dot11s::IeRann::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-rann.h: uint32_t ns3::dot11s::IeRann::GetDestSeqNumber() [member function]
    cls.add_method('GetDestSeqNumber', 
                   'uint32_t', 
                   [])
    ## ie-dot11s-rann.h: uint8_t ns3::dot11s::IeRann::GetFlags() [member function]
    cls.add_method('GetFlags', 
                   'uint8_t', 
                   [])
    ## ie-dot11s-rann.h: uint8_t ns3::dot11s::IeRann::GetHopcount() [member function]
    cls.add_method('GetHopcount', 
                   'uint8_t', 
                   [])
    ## ie-dot11s-rann.h: uint8_t ns3::dot11s::IeRann::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-rann.h: uint32_t ns3::dot11s::IeRann::GetMetric() [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [])
    ## ie-dot11s-rann.h: ns3::Mac48Address ns3::dot11s::IeRann::GetOriginatorAddress() [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Mac48Address', 
                   [])
    ## ie-dot11s-rann.h: uint8_t ns3::dot11s::IeRann::GetTtl() [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [])
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::IncrementMetric(uint32_t metric) [member function]
    cls.add_method('IncrementMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::SetDestSeqNumber(uint32_t dest_seq_number) [member function]
    cls.add_method('SetDestSeqNumber', 
                   'void', 
                   [param('uint32_t', 'dest_seq_number')])
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::SetFlags(uint8_t flags) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::SetHopcount(uint8_t hopcount) [member function]
    cls.add_method('SetHopcount', 
                   'void', 
                   [param('uint8_t', 'hopcount')])
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::SetMetric(uint32_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::SetOriginatorAddress(ns3::Mac48Address originator_address) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'originator_address')])
    ## ie-dot11s-rann.h: void ns3::dot11s::IeRann::SetTTL(uint8_t ttl) [member function]
    cls.add_method('SetTTL', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3Dot11sMeshHeader_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## dot11s-mac-header.h: ns3::dot11s::MeshHeader::MeshHeader(ns3::dot11s::MeshHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::MeshHeader const &', 'arg0')])
    ## dot11s-mac-header.h: ns3::dot11s::MeshHeader::MeshHeader() [constructor]
    cls.add_constructor([])
    ## dot11s-mac-header.h: uint32_t ns3::dot11s::MeshHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## dot11s-mac-header.h: ns3::Mac48Address ns3::dot11s::MeshHeader::GetAddr4() const [member function]
    cls.add_method('GetAddr4', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h: ns3::Mac48Address ns3::dot11s::MeshHeader::GetAddr5() const [member function]
    cls.add_method('GetAddr5', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h: ns3::Mac48Address ns3::dot11s::MeshHeader::GetAddr6() const [member function]
    cls.add_method('GetAddr6', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h: uint8_t ns3::dot11s::MeshHeader::GetAddressExt() const [member function]
    cls.add_method('GetAddressExt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h: ns3::TypeId ns3::dot11s::MeshHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dot11s-mac-header.h: uint32_t ns3::dot11s::MeshHeader::GetMeshSeqno() const [member function]
    cls.add_method('GetMeshSeqno', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h: uint8_t ns3::dot11s::MeshHeader::GetMeshTtl() const [member function]
    cls.add_method('GetMeshTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h: uint32_t ns3::dot11s::MeshHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dot11s-mac-header.h: static ns3::TypeId ns3::dot11s::MeshHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dot11s-mac-header.h: void ns3::dot11s::MeshHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## dot11s-mac-header.h: void ns3::dot11s::MeshHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## dot11s-mac-header.h: void ns3::dot11s::MeshHeader::SetAddr4(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr4', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## dot11s-mac-header.h: void ns3::dot11s::MeshHeader::SetAddr5(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr5', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## dot11s-mac-header.h: void ns3::dot11s::MeshHeader::SetAddr6(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr6', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## dot11s-mac-header.h: void ns3::dot11s::MeshHeader::SetAddressExt(uint8_t num_of_addresses) [member function]
    cls.add_method('SetAddressExt', 
                   'void', 
                   [param('uint8_t', 'num_of_addresses')])
    ## dot11s-mac-header.h: void ns3::dot11s::MeshHeader::SetMeshSeqno(uint32_t seqno) [member function]
    cls.add_method('SetMeshSeqno', 
                   'void', 
                   [param('uint32_t', 'seqno')])
    ## dot11s-mac-header.h: void ns3::dot11s::MeshHeader::SetMeshTtl(uint8_t TTL) [member function]
    cls.add_method('SetMeshTtl', 
                   'void', 
                   [param('uint8_t', 'TTL')])
    return

def register_Ns3Dot11sPeerLink_methods(root_module, cls):
    ## peer-link.h: static ns3::TypeId ns3::dot11s::PeerLink::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## peer-link.h: ns3::dot11s::PeerLink::PeerLink() [constructor]
    cls.add_constructor([])
    ## peer-link.h: void ns3::dot11s::PeerLink::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## peer-link.h: void ns3::dot11s::PeerLink::SetBeaconInformation(ns3::Time lastBeacon, ns3::Time BeaconInterval) [member function]
    cls.add_method('SetBeaconInformation', 
                   'void', 
                   [param('ns3::Time', 'lastBeacon'), param('ns3::Time', 'BeaconInterval')])
    ## peer-link.h: void ns3::dot11s::PeerLink::SetLinkStatusCallback(ns3::Callback<void,unsigned int,ns3::Mac48Address,bool,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> cb) [member function]
    cls.add_method('SetLinkStatusCallback', 
                   'void', 
                   [param('ns3::Callback< void, unsigned int, ns3::Mac48Address, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## peer-link.h: void ns3::dot11s::PeerLink::SetPeerAddress(ns3::Mac48Address macaddr) [member function]
    cls.add_method('SetPeerAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'macaddr')])
    ## peer-link.h: void ns3::dot11s::PeerLink::SetPeerMeshPointAddress(ns3::Mac48Address macaddr) [member function]
    cls.add_method('SetPeerMeshPointAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'macaddr')])
    ## peer-link.h: void ns3::dot11s::PeerLink::SetInterface(uint32_t interface) [member function]
    cls.add_method('SetInterface', 
                   'void', 
                   [param('uint32_t', 'interface')])
    ## peer-link.h: void ns3::dot11s::PeerLink::SetLocalLinkId(uint16_t id) [member function]
    cls.add_method('SetLocalLinkId', 
                   'void', 
                   [param('uint16_t', 'id')])
    ## peer-link.h: void ns3::dot11s::PeerLink::SetLocalAid(uint16_t aid) [member function]
    cls.add_method('SetLocalAid', 
                   'void', 
                   [param('uint16_t', 'aid')])
    ## peer-link.h: uint16_t ns3::dot11s::PeerLink::GetPeerAid() const [member function]
    cls.add_method('GetPeerAid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## peer-link.h: void ns3::dot11s::PeerLink::SetBeaconTimingElement(ns3::dot11s::IeBeaconTiming beaconTiming) [member function]
    cls.add_method('SetBeaconTimingElement', 
                   'void', 
                   [param('ns3::dot11s::IeBeaconTiming', 'beaconTiming')])
    ## peer-link.h: ns3::Mac48Address ns3::dot11s::PeerLink::GetPeerAddress() const [member function]
    cls.add_method('GetPeerAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## peer-link.h: uint16_t ns3::dot11s::PeerLink::GetLocalAid() const [member function]
    cls.add_method('GetLocalAid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## peer-link.h: ns3::Time ns3::dot11s::PeerLink::GetLastBeacon() const [member function]
    cls.add_method('GetLastBeacon', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## peer-link.h: ns3::Time ns3::dot11s::PeerLink::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## peer-link.h: ns3::dot11s::IeBeaconTiming ns3::dot11s::PeerLink::GetBeaconTimingElement() const [member function]
    cls.add_method('GetBeaconTimingElement', 
                   'ns3::dot11s::IeBeaconTiming', 
                   [], 
                   is_const=True)
    ## peer-link.h: void ns3::dot11s::PeerLink::MLMECancelPeerLink(ns3::dot11s::PmpReasonCode reason) [member function]
    cls.add_method('MLMECancelPeerLink', 
                   'void', 
                   [param('ns3::dot11s::PmpReasonCode', 'reason')])
    ## peer-link.h: void ns3::dot11s::PeerLink::MLMEActivePeerLinkOpen() [member function]
    cls.add_method('MLMEActivePeerLinkOpen', 
                   'void', 
                   [])
    ## peer-link.h: void ns3::dot11s::PeerLink::MLMEPeeringRequestReject() [member function]
    cls.add_method('MLMEPeeringRequestReject', 
                   'void', 
                   [])
    ## peer-link.h: void ns3::dot11s::PeerLink::MLMESetSignalStatusCallback(ns3::Callback<void, unsigned int, ns3::Mac48Address, ns3::Mac48Address, ns3::dot11s::PeerLink::PeerState, ns3::dot11s::PeerLink::PeerState, ns3::empty, ns3::empty, ns3::empty, ns3::empty> arg0) [member function]
    cls.add_method('MLMESetSignalStatusCallback', 
                   'void', 
                   [param('ns3::Callback< void, unsigned int, ns3::Mac48Address, ns3::Mac48Address, ns3::dot11s::PeerLink::PeerState, ns3::dot11s::PeerLink::PeerState, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'arg0')])
    ## peer-link.h: void ns3::dot11s::PeerLink::TransmissionSuccess() [member function]
    cls.add_method('TransmissionSuccess', 
                   'void', 
                   [])
    ## peer-link.h: void ns3::dot11s::PeerLink::TransmissionFailure() [member function]
    cls.add_method('TransmissionFailure', 
                   'void', 
                   [])
    ## peer-link.h: void ns3::dot11s::PeerLink::Report(std::ostream & os) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3Dot11sPeerLinkFrameStart_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PeerLinkFrameStart() [constructor]
    cls.add_constructor([])
    ## peer-link-frame.h: uint32_t ns3::dot11s::PeerLinkFrameStart::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields ns3::dot11s::PeerLinkFrameStart::GetFields() const [member function]
    cls.add_method('GetFields', 
                   'ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields', 
                   [], 
                   is_const=True)
    ## peer-link-frame.h: ns3::TypeId ns3::dot11s::PeerLinkFrameStart::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h: uint32_t ns3::dot11s::PeerLinkFrameStart::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h: static ns3::TypeId ns3::dot11s::PeerLinkFrameStart::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## peer-link-frame.h: void ns3::dot11s::PeerLinkFrameStart::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h: void ns3::dot11s::PeerLinkFrameStart::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h: void ns3::dot11s::PeerLinkFrameStart::SetPlinkFrameStart(ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields arg0) [member function]
    cls.add_method('SetPlinkFrameStart', 
                   'void', 
                   [param('ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields', 'arg0')])
    ## peer-link-frame.h: void ns3::dot11s::PeerLinkFrameStart::SetPlinkFrameSubtype(uint8_t subtype) [member function]
    cls.add_method('SetPlinkFrameSubtype', 
                   'void', 
                   [param('uint8_t', 'subtype')])
    return

def register_Ns3Dot11sPeerLinkFrameStartPlinkFrameStartFields_methods(root_module, cls):
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::PlinkFrameStartFields() [constructor]
    cls.add_constructor([])
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::PlinkFrameStartFields(ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields const &', 'arg0')])
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::aid [variable]
    cls.add_instance_attribute('aid', 'uint16_t', is_const=False)
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::capability [variable]
    cls.add_instance_attribute('capability', 'uint16_t', is_const=False)
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::config [variable]
    cls.add_instance_attribute('config', 'ns3::dot11s::IeConfiguration', is_const=False)
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::meshId [variable]
    cls.add_instance_attribute('meshId', 'ns3::dot11s::IeMeshId', is_const=False)
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::protocol [variable]
    cls.add_instance_attribute('protocol', 'ns3::dot11s::IePeeringProtocol', is_const=False)
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::rates [variable]
    cls.add_instance_attribute('rates', 'ns3::SupportedRates', is_const=False)
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::reasonCode [variable]
    cls.add_instance_attribute('reasonCode', 'uint16_t', is_const=False)
    ## peer-link-frame.h: ns3::dot11s::PeerLinkFrameStart::PlinkFrameStartFields::subtype [variable]
    cls.add_instance_attribute('subtype', 'uint8_t', is_const=False)
    return

def register_Ns3Dot11sPeerManagementProtocol_methods(root_module, cls):
    ## peer-management-protocol.h: ns3::dot11s::PeerManagementProtocol::PeerManagementProtocol() [constructor]
    cls.add_constructor([])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::ConfigurationMismatch(uint32_t interface, ns3::Mac48Address peerAddress) [member function]
    cls.add_method('ConfigurationMismatch', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## peer-management-protocol.h: ns3::Ptr<ns3::dot11s::PeerLink> ns3::dot11s::PeerManagementProtocol::FindPeerLink(uint32_t interface, ns3::Mac48Address peerAddress) [member function]
    cls.add_method('FindPeerLink', 
                   'ns3::Ptr< ns3::dot11s::PeerLink >', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress')])
    ## peer-management-protocol.h: ns3::Mac48Address ns3::dot11s::PeerManagementProtocol::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [])
    ## peer-management-protocol.h: bool ns3::dot11s::PeerManagementProtocol::GetBeaconCollisionAvoidance() const [member function]
    cls.add_method('GetBeaconCollisionAvoidance', 
                   'bool', 
                   [], 
                   is_const=True)
    ## peer-management-protocol.h: ns3::Ptr<ns3::dot11s::IeBeaconTiming> ns3::dot11s::PeerManagementProtocol::GetBeaconTimingElement(uint32_t interface) [member function]
    cls.add_method('GetBeaconTimingElement', 
                   'ns3::Ptr< ns3::dot11s::IeBeaconTiming >', 
                   [param('uint32_t', 'interface')])
    ## peer-management-protocol.h: ns3::Ptr<ns3::dot11s::IeMeshId> ns3::dot11s::PeerManagementProtocol::GetMeshId() const [member function]
    cls.add_method('GetMeshId', 
                   'ns3::Ptr< ns3::dot11s::IeMeshId >', 
                   [], 
                   is_const=True)
    ## peer-management-protocol.h: uint8_t ns3::dot11s::PeerManagementProtocol::GetNumberOfLinks() [member function]
    cls.add_method('GetNumberOfLinks', 
                   'uint8_t', 
                   [])
    ## peer-management-protocol.h: std::vector<ns3::Ptr<ns3::dot11s::PeerLink>,std::allocator<ns3::Ptr<ns3::dot11s::PeerLink> > > ns3::dot11s::PeerManagementProtocol::GetPeerLinks() const [member function]
    cls.add_method('GetPeerLinks', 
                   'std::vector< ns3::Ptr< ns3::dot11s::PeerLink > >', 
                   [], 
                   is_const=True)
    ## peer-management-protocol.h: std::vector<ns3::Mac48Address,std::allocator<ns3::Mac48Address> > ns3::dot11s::PeerManagementProtocol::GetPeers(uint32_t interface) const [member function]
    cls.add_method('GetPeers', 
                   'std::vector< ns3::Mac48Address >', 
                   [param('uint32_t', 'interface')], 
                   is_const=True)
    ## peer-management-protocol.h: static ns3::TypeId ns3::dot11s::PeerManagementProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## peer-management-protocol.h: bool ns3::dot11s::PeerManagementProtocol::Install(ns3::Ptr<ns3::MeshPointDevice> arg0) [member function]
    cls.add_method('Install', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'arg0')])
    ## peer-management-protocol.h: bool ns3::dot11s::PeerManagementProtocol::IsActiveLink(uint32_t interface, ns3::Mac48Address peerAddress) [member function]
    cls.add_method('IsActiveLink', 
                   'bool', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::NotifyBeaconSent(uint32_t interface, ns3::Time beaconInterval) [member function]
    cls.add_method('NotifyBeaconSent', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Time', 'beaconInterval')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::ReceiveBeacon(uint32_t interface, ns3::Mac48Address peerAddress, ns3::Time beaconInterval, ns3::Ptr<ns3::dot11s::IeBeaconTiming> beaconTiming) [member function]
    cls.add_method('ReceiveBeacon', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress'), param('ns3::Time', 'beaconInterval'), param('ns3::Ptr< ns3::dot11s::IeBeaconTiming >', 'beaconTiming')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::ReceivePeerLinkFrame(uint32_t interface, ns3::Mac48Address peerAddress, ns3::Mac48Address peerMeshPointAddress, uint16_t aid, ns3::dot11s::IePeerManagement peerManagementElement, ns3::dot11s::IeConfiguration meshConfig) [member function]
    cls.add_method('ReceivePeerLinkFrame', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress'), param('ns3::Mac48Address', 'peerMeshPointAddress'), param('uint16_t', 'aid'), param('ns3::dot11s::IePeerManagement', 'peerManagementElement'), param('ns3::dot11s::IeConfiguration', 'meshConfig')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::SetBeaconCollisionAvoidance(bool enable) [member function]
    cls.add_method('SetBeaconCollisionAvoidance', 
                   'void', 
                   [param('bool', 'enable')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::SetMeshId(std::string s) [member function]
    cls.add_method('SetMeshId', 
                   'void', 
                   [param('std::string', 's')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::SetPeerLinkStatusCallback(ns3::Callback<void, ns3::Mac48Address, ns3::Mac48Address, unsigned int, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPeerLinkStatusCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Mac48Address, ns3::Mac48Address, unsigned int, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::TransmissionFailure(uint32_t interface, ns3::Mac48Address const peerAddress) [member function]
    cls.add_method('TransmissionFailure', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address const', 'peerAddress')])
    ## peer-management-protocol.h: void ns3::dot11s::PeerManagementProtocol::TransmissionSuccess(uint32_t interface, ns3::Mac48Address const peerAddress) [member function]
    cls.add_method('TransmissionSuccess', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address const', 'peerAddress')])
    return

def register_Ns3FlameFlameHeader_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## flame-header.h: ns3::flame::FlameHeader::FlameHeader(ns3::flame::FlameHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameHeader const &', 'arg0')])
    ## flame-header.h: ns3::flame::FlameHeader::FlameHeader() [constructor]
    cls.add_constructor([])
    ## flame-header.h: void ns3::flame::FlameHeader::AddCost(uint8_t cost) [member function]
    cls.add_method('AddCost', 
                   'void', 
                   [param('uint8_t', 'cost')])
    ## flame-header.h: uint32_t ns3::flame::FlameHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## flame-header.h: uint8_t ns3::flame::FlameHeader::GetCost() const [member function]
    cls.add_method('GetCost', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## flame-header.h: ns3::TypeId ns3::flame::FlameHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-header.h: ns3::Mac48Address ns3::flame::FlameHeader::GetOrigDst() const [member function]
    cls.add_method('GetOrigDst', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## flame-header.h: ns3::Mac48Address ns3::flame::FlameHeader::GetOrigSrc() const [member function]
    cls.add_method('GetOrigSrc', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## flame-header.h: uint16_t ns3::flame::FlameHeader::GetProtocol() const [member function]
    cls.add_method('GetProtocol', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-header.h: uint16_t ns3::flame::FlameHeader::GetSeqno() const [member function]
    cls.add_method('GetSeqno', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-header.h: uint32_t ns3::flame::FlameHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-header.h: static ns3::TypeId ns3::flame::FlameHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-header.h: void ns3::flame::FlameHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## flame-header.h: void ns3::flame::FlameHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## flame-header.h: void ns3::flame::FlameHeader::SetOrigDst(ns3::Mac48Address dst) [member function]
    cls.add_method('SetOrigDst', 
                   'void', 
                   [param('ns3::Mac48Address', 'dst')])
    ## flame-header.h: void ns3::flame::FlameHeader::SetOrigSrc(ns3::Mac48Address OrigSrc) [member function]
    cls.add_method('SetOrigSrc', 
                   'void', 
                   [param('ns3::Mac48Address', 'OrigSrc')])
    ## flame-header.h: void ns3::flame::FlameHeader::SetProtocol(uint16_t protocol) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint16_t', 'protocol')])
    ## flame-header.h: void ns3::flame::FlameHeader::SetSeqno(uint16_t seqno) [member function]
    cls.add_method('SetSeqno', 
                   'void', 
                   [param('uint16_t', 'seqno')])
    return

def register_Ns3FlameFlameProtocol_methods(root_module, cls):
    ## flame-protocol.h: ns3::flame::FlameProtocol::FlameProtocol() [constructor]
    cls.add_constructor([])
    ## flame-protocol.h: void ns3::flame::FlameProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-protocol.h: ns3::Mac48Address ns3::flame::FlameProtocol::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [])
    ## flame-protocol.h: static ns3::TypeId ns3::flame::FlameProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-protocol.h: bool ns3::flame::FlameProtocol::Install(ns3::Ptr<ns3::MeshPointDevice> arg0) [member function]
    cls.add_method('Install', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'arg0')])
    ## flame-protocol.h: bool ns3::flame::FlameProtocol::RemoveRoutingStuff(uint32_t fromIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet> packet, uint16_t & protocolType) [member function]
    cls.add_method('RemoveRoutingStuff', 
                   'bool', 
                   [param('uint32_t', 'fromIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t &', 'protocolType')], 
                   is_virtual=True)
    ## flame-protocol.h: void ns3::flame::FlameProtocol::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## flame-protocol.h: bool ns3::flame::FlameProtocol::RequestRoute(uint32_t sourceIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet const> packet, uint16_t protocolType, ns3::Callback<void, bool, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'sourceIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocolType'), param('ns3::Callback< void, bool, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_virtual=True)
    ## flame-protocol.h: void ns3::flame::FlameProtocol::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    return

def register_Ns3FlameFlameProtocolMac_methods(root_module, cls):
    ## flame-protocol-mac.h: ns3::flame::FlameProtocolMac::FlameProtocolMac(ns3::flame::FlameProtocolMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameProtocolMac const &', 'arg0')])
    ## flame-protocol-mac.h: ns3::flame::FlameProtocolMac::FlameProtocolMac(uint32_t arg0, ns3::Ptr<ns3::flame::FlameProtocol> arg1) [constructor]
    cls.add_constructor([param('uint32_t', 'arg0'), param('ns3::Ptr< ns3::flame::FlameProtocol >', 'arg1')])
    ## flame-protocol-mac.h: uint16_t ns3::flame::FlameProtocolMac::GetChannelId() const [member function]
    cls.add_method('GetChannelId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-protocol-mac.h: bool ns3::flame::FlameProtocolMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const & header) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const &', 'header')], 
                   is_virtual=True)
    ## flame-protocol-mac.h: void ns3::flame::FlameProtocolMac::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## flame-protocol-mac.h: void ns3::flame::FlameProtocolMac::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## flame-protocol-mac.h: void ns3::flame::FlameProtocolMac::SetParent(ns3::Ptr<ns3::MeshWifiInterfaceMac> parent) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMac >', 'parent')], 
                   is_virtual=True)
    ## flame-protocol-mac.h: void ns3::flame::FlameProtocolMac::UpdateBeacon(ns3::MeshWifiBeacon & beacon) const [member function]
    cls.add_method('UpdateBeacon', 
                   'void', 
                   [param('ns3::MeshWifiBeacon &', 'beacon')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol-mac.h: bool ns3::flame::FlameProtocolMac::UpdateOutcomingFrame(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader & header, ns3::Mac48Address from, ns3::Mac48Address to) [member function]
    cls.add_method('UpdateOutcomingFrame', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader &', 'header'), param('ns3::Mac48Address', 'from'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    return

def register_Ns3FlameFlameRtable_methods(root_module, cls):
    ## flame-rtable.h: ns3::flame::FlameRtable::FlameRtable() [constructor]
    cls.add_constructor([])
    ## flame-rtable.h: void ns3::flame::FlameRtable::AddPath(ns3::Mac48Address const destination, ns3::Mac48Address const retransmitter, uint32_t const interface, uint8_t const cost, uint16_t const seqnum) [member function]
    cls.add_method('AddPath', 
                   'void', 
                   [param('ns3::Mac48Address const', 'destination'), param('ns3::Mac48Address const', 'retransmitter'), param('uint32_t const', 'interface'), param('uint8_t const', 'cost'), param('uint16_t const', 'seqnum')])
    ## flame-rtable.h: void ns3::flame::FlameRtable::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-rtable.h: static ns3::TypeId ns3::flame::FlameRtable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult ns3::flame::FlameRtable::Lookup(ns3::Mac48Address destination) [member function]
    cls.add_method('Lookup', 
                   'ns3::flame::FlameRtable::LookupResult', 
                   [param('ns3::Mac48Address', 'destination')])
    ## flame-rtable.h: ns3::flame::FlameRtable::INTERFACE_ANY [variable]
    cls.add_static_attribute('INTERFACE_ANY', 'uint32_t const', is_const=True)
    ## flame-rtable.h: ns3::flame::FlameRtable::MAX_COST [variable]
    cls.add_static_attribute('MAX_COST', 'uint32_t const', is_const=True)
    return

def register_Ns3FlameFlameRtableLookupResult_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::LookupResult(ns3::flame::FlameRtable::LookupResult const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameRtable::LookupResult const &', 'arg0')])
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::LookupResult(ns3::Mac48Address r=ns3::Mac48Address::GetBroadcast(), uint32_t i=ns3::flame::FlameRtable::INTERFACE_ANY, uint8_t c=ns3::flame::FlameRtable::MAX_COST, uint16_t s=0) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'r', default_value='ns3::Mac48Address::GetBroadcast()'), param('uint32_t', 'i', default_value='ns3::flame::FlameRtable::INTERFACE_ANY'), param('uint8_t', 'c', default_value='ns3::flame::FlameRtable::MAX_COST'), param('uint16_t', 's', default_value='0')])
    ## flame-rtable.h: bool ns3::flame::FlameRtable::LookupResult::IsValid() const [member function]
    cls.add_method('IsValid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::cost [variable]
    cls.add_instance_attribute('cost', 'uint8_t', is_const=False)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::ifIndex [variable]
    cls.add_instance_attribute('ifIndex', 'uint32_t', is_const=False)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::retransmitter [variable]
    cls.add_instance_attribute('retransmitter', 'ns3::Mac48Address', is_const=False)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::seqnum [variable]
    cls.add_instance_attribute('seqnum', 'uint16_t', is_const=False)
    return

def register_Ns3FlameFlameTag_methods(root_module, cls):
    ## flame-protocol.h: ns3::flame::FlameTag::FlameTag(ns3::flame::FlameTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameTag const &', 'arg0')])
    ## flame-protocol.h: ns3::flame::FlameTag::FlameTag(ns3::Mac48Address a=ns3::Mac48Address()) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'a', default_value='ns3::Mac48Address()')])
    ## flame-protocol.h: void ns3::flame::FlameTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## flame-protocol.h: ns3::TypeId ns3::flame::FlameTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h: uint32_t ns3::flame::FlameTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h: static ns3::TypeId ns3::flame::FlameTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-protocol.h: void ns3::flame::FlameTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h: void ns3::flame::FlameTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h: ns3::flame::FlameTag::receiver [variable]
    cls.add_instance_attribute('receiver', 'ns3::Mac48Address', is_const=False)
    ## flame-protocol.h: ns3::flame::FlameTag::transmitter [variable]
    cls.add_instance_attribute('transmitter', 'ns3::Mac48Address', is_const=False)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_aodv(module.get_submodule('aodv'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_dsdv(module.get_submodule('dsdv'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_aodv(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    ## ie-dot11s-id.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::dot11s::MakeIeMeshIdChecker() [free function]
    module.add_function('MakeIeMeshIdChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    return

def register_functions_ns3_dsdv(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

