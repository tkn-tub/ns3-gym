from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## animation-interface.h: ns3::AnimationInterface [class]
    module.add_class('AnimationInterface')
    ## athstats-helper.h: ns3::AthstatsHelper [class]
    module.add_class('AthstatsHelper')
    ## bulk-send-helper.h: ns3::BulkSendHelper [class]
    module.add_class('BulkSendHelper')
    ## csma-star-helper.h: ns3::CsmaStarHelper [class]
    module.add_class('CsmaStarHelper')
    ## mesh-helper.h: ns3::MeshHelper [class]
    module.add_class('MeshHelper')
    ## mesh-helper.h: ns3::MeshHelper::ChannelPolicy [enumeration]
    module.add_enum('ChannelPolicy', ['SPREAD_CHANNELS', 'ZERO_CHANNEL'], outer_class=root_module['ns3::MeshHelper'])
    ## mobility-helper.h: ns3::MobilityHelper [class]
    module.add_class('MobilityHelper')
    ## ns2-mobility-helper.h: ns3::Ns2MobilityHelper [class]
    module.add_class('Ns2MobilityHelper')
    ## on-off-helper.h: ns3::OnOffHelper [class]
    module.add_class('OnOffHelper')
    ## packet-sink-helper.h: ns3::PacketSinkHelper [class]
    module.add_class('PacketSinkHelper')
    ## ping6-helper.h: ns3::Ping6Helper [class]
    module.add_class('Ping6Helper')
    ## point-to-point-dumbbell-helper.h: ns3::PointToPointDumbbellHelper [class]
    module.add_class('PointToPointDumbbellHelper')
    ## point-to-point-grid-helper.h: ns3::PointToPointGridHelper [class]
    module.add_class('PointToPointGridHelper')
    ## point-to-point-helper.h: ns3::PointToPointHelper [class]
    module.add_class('PointToPointHelper', parent=[root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## point-to-point-star-helper.h: ns3::PointToPointStarHelper [class]
    module.add_class('PointToPointStarHelper')
    ## tap-bridge-helper.h: ns3::TapBridgeHelper [class]
    module.add_class('TapBridgeHelper')
    ## topology-reader-helper.h: ns3::TopologyReaderHelper [class]
    module.add_class('TopologyReaderHelper')
    ## udp-client-server-helper.h: ns3::UdpClientHelper [class]
    module.add_class('UdpClientHelper')
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper [class]
    module.add_class('UdpEchoClientHelper')
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper [class]
    module.add_class('UdpEchoServerHelper')
    ## udp-client-server-helper.h: ns3::UdpServerHelper [class]
    module.add_class('UdpServerHelper')
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper [class]
    module.add_class('UdpTraceClientHelper')
    ## v4ping-helper.h: ns3::V4PingHelper [class]
    module.add_class('V4PingHelper')
    ## wifi-helper.h: ns3::WifiHelper [class]
    module.add_class('WifiHelper')
    ## wifi-helper.h: ns3::WifiMacHelper [class]
    module.add_class('WifiMacHelper', allow_subclassing=True)
    ## wifi-helper.h: ns3::WifiPhyHelper [class]
    module.add_class('WifiPhyHelper', allow_subclassing=True)
    ## wimax-helper.h: ns3::WimaxHelper [class]
    module.add_class('WimaxHelper', parent=[root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## wimax-helper.h: ns3::WimaxHelper::NetDeviceType [enumeration]
    module.add_enum('NetDeviceType', ['DEVICE_TYPE_SUBSCRIBER_STATION', 'DEVICE_TYPE_BASE_STATION'], outer_class=root_module['ns3::WimaxHelper'])
    ## wimax-helper.h: ns3::WimaxHelper::PhyType [enumeration]
    module.add_enum('PhyType', ['SIMPLE_PHY_TYPE_OFDM'], outer_class=root_module['ns3::WimaxHelper'])
    ## wimax-helper.h: ns3::WimaxHelper::SchedulerType [enumeration]
    module.add_enum('SchedulerType', ['SCHED_TYPE_SIMPLE', 'SCHED_TYPE_RTPS', 'SCHED_TYPE_MBQOS'], outer_class=root_module['ns3::WimaxHelper'])
    ## yans-wifi-helper.h: ns3::YansWifiChannelHelper [class]
    module.add_class('YansWifiChannelHelper')
    ## yans-wifi-helper.h: ns3::YansWifiPhyHelper [class]
    module.add_class('YansWifiPhyHelper', parent=[root_module['ns3::WifiPhyHelper'], root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## yans-wifi-helper.h: ns3::YansWifiPhyHelper::SupportedPcapDataLinkTypes [enumeration]
    module.add_enum('SupportedPcapDataLinkTypes', ['DLT_IEEE802_11', 'DLT_PRISM_HEADER', 'DLT_IEEE802_11_RADIO'], outer_class=root_module['ns3::YansWifiPhyHelper'])
    ## aodv-helper.h: ns3::AodvHelper [class]
    module.add_class('AodvHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    ## csma-helper.h: ns3::CsmaHelper [class]
    module.add_class('CsmaHelper', parent=[root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## emu-helper.h: ns3::EmuHelper [class]
    module.add_class('EmuHelper', parent=[root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## ipv4-nix-vector-helper.h: ns3::Ipv4NixVectorHelper [class]
    module.add_class('Ipv4NixVectorHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    ## nqos-wifi-mac-helper.h: ns3::NqosWifiMacHelper [class]
    module.add_class('NqosWifiMacHelper', parent=root_module['ns3::WifiMacHelper'])
    ## qos-wifi-mac-helper.h: ns3::QosWifiMacHelper [class]
    module.add_class('QosWifiMacHelper', parent=root_module['ns3::WifiMacHelper'])
    ## athstats-helper.h: ns3::AthstatsWifiTraceSink [class]
    module.add_class('AthstatsWifiTraceSink', parent=root_module['ns3::Object'])
    ## canvas-location.h: ns3::CanvasLocation [class]
    module.add_class('CanvasLocation', parent=root_module['ns3::Object'])
    ## mesh-stack-installer.h: ns3::MeshStack [class]
    module.add_class('MeshStack', parent=root_module['ns3::Object'])
    ## dot11s-installer.h: ns3::Dot11sStack [class]
    module.add_class('Dot11sStack', parent=root_module['ns3::MeshStack'])
    ## flame-installer.h: ns3::FlameStack [class]
    module.add_class('FlameStack', parent=root_module['ns3::MeshStack'])
    
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
    

def register_types_ns3_dsdv(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3AnimationInterface_methods(root_module, root_module['ns3::AnimationInterface'])
    register_Ns3AthstatsHelper_methods(root_module, root_module['ns3::AthstatsHelper'])
    register_Ns3BulkSendHelper_methods(root_module, root_module['ns3::BulkSendHelper'])
    register_Ns3CsmaStarHelper_methods(root_module, root_module['ns3::CsmaStarHelper'])
    register_Ns3MeshHelper_methods(root_module, root_module['ns3::MeshHelper'])
    register_Ns3MobilityHelper_methods(root_module, root_module['ns3::MobilityHelper'])
    register_Ns3Ns2MobilityHelper_methods(root_module, root_module['ns3::Ns2MobilityHelper'])
    register_Ns3OnOffHelper_methods(root_module, root_module['ns3::OnOffHelper'])
    register_Ns3PacketSinkHelper_methods(root_module, root_module['ns3::PacketSinkHelper'])
    register_Ns3Ping6Helper_methods(root_module, root_module['ns3::Ping6Helper'])
    register_Ns3PointToPointDumbbellHelper_methods(root_module, root_module['ns3::PointToPointDumbbellHelper'])
    register_Ns3PointToPointGridHelper_methods(root_module, root_module['ns3::PointToPointGridHelper'])
    register_Ns3PointToPointHelper_methods(root_module, root_module['ns3::PointToPointHelper'])
    register_Ns3PointToPointStarHelper_methods(root_module, root_module['ns3::PointToPointStarHelper'])
    register_Ns3TapBridgeHelper_methods(root_module, root_module['ns3::TapBridgeHelper'])
    register_Ns3TopologyReaderHelper_methods(root_module, root_module['ns3::TopologyReaderHelper'])
    register_Ns3UdpClientHelper_methods(root_module, root_module['ns3::UdpClientHelper'])
    register_Ns3UdpEchoClientHelper_methods(root_module, root_module['ns3::UdpEchoClientHelper'])
    register_Ns3UdpEchoServerHelper_methods(root_module, root_module['ns3::UdpEchoServerHelper'])
    register_Ns3UdpServerHelper_methods(root_module, root_module['ns3::UdpServerHelper'])
    register_Ns3UdpTraceClientHelper_methods(root_module, root_module['ns3::UdpTraceClientHelper'])
    register_Ns3V4PingHelper_methods(root_module, root_module['ns3::V4PingHelper'])
    register_Ns3WifiHelper_methods(root_module, root_module['ns3::WifiHelper'])
    register_Ns3WifiMacHelper_methods(root_module, root_module['ns3::WifiMacHelper'])
    register_Ns3WifiPhyHelper_methods(root_module, root_module['ns3::WifiPhyHelper'])
    register_Ns3WimaxHelper_methods(root_module, root_module['ns3::WimaxHelper'])
    register_Ns3YansWifiChannelHelper_methods(root_module, root_module['ns3::YansWifiChannelHelper'])
    register_Ns3YansWifiPhyHelper_methods(root_module, root_module['ns3::YansWifiPhyHelper'])
    register_Ns3AodvHelper_methods(root_module, root_module['ns3::AodvHelper'])
    register_Ns3CsmaHelper_methods(root_module, root_module['ns3::CsmaHelper'])
    register_Ns3EmuHelper_methods(root_module, root_module['ns3::EmuHelper'])
    register_Ns3Ipv4NixVectorHelper_methods(root_module, root_module['ns3::Ipv4NixVectorHelper'])
    register_Ns3NqosWifiMacHelper_methods(root_module, root_module['ns3::NqosWifiMacHelper'])
    register_Ns3QosWifiMacHelper_methods(root_module, root_module['ns3::QosWifiMacHelper'])
    register_Ns3AthstatsWifiTraceSink_methods(root_module, root_module['ns3::AthstatsWifiTraceSink'])
    register_Ns3CanvasLocation_methods(root_module, root_module['ns3::CanvasLocation'])
    register_Ns3MeshStack_methods(root_module, root_module['ns3::MeshStack'])
    register_Ns3Dot11sStack_methods(root_module, root_module['ns3::Dot11sStack'])
    register_Ns3FlameStack_methods(root_module, root_module['ns3::FlameStack'])
    return

def register_Ns3AnimationInterface_methods(root_module, cls):
    ## animation-interface.h: ns3::AnimationInterface::AnimationInterface(ns3::AnimationInterface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AnimationInterface const &', 'arg0')])
    ## animation-interface.h: ns3::AnimationInterface::AnimationInterface() [constructor]
    cls.add_constructor([])
    ## animation-interface.h: bool ns3::AnimationInterface::SetOutputFile(std::string const & fn) [member function]
    cls.add_method('SetOutputFile', 
                   'bool', 
                   [param('std::string const &', 'fn')])
    ## animation-interface.h: bool ns3::AnimationInterface::SetServerPort(uint16_t port) [member function]
    cls.add_method('SetServerPort', 
                   'bool', 
                   [param('uint16_t', 'port')])
    ## animation-interface.h: void ns3::AnimationInterface::StartAnimation() [member function]
    cls.add_method('StartAnimation', 
                   'void', 
                   [])
    ## animation-interface.h: void ns3::AnimationInterface::StopAnimation() [member function]
    cls.add_method('StopAnimation', 
                   'void', 
                   [])
    return

def register_Ns3AthstatsHelper_methods(root_module, cls):
    ## athstats-helper.h: ns3::AthstatsHelper::AthstatsHelper(ns3::AthstatsHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AthstatsHelper const &', 'arg0')])
    ## athstats-helper.h: ns3::AthstatsHelper::AthstatsHelper() [constructor]
    cls.add_constructor([])
    ## athstats-helper.h: void ns3::AthstatsHelper::EnableAthstats(std::string filename, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAthstats', 
                   'void', 
                   [param('std::string', 'filename'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')])
    ## athstats-helper.h: void ns3::AthstatsHelper::EnableAthstats(std::string filename, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('EnableAthstats', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## athstats-helper.h: void ns3::AthstatsHelper::EnableAthstats(std::string filename, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAthstats', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NetDeviceContainer', 'd')])
    ## athstats-helper.h: void ns3::AthstatsHelper::EnableAthstats(std::string filename, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAthstats', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NodeContainer', 'n')])
    return

def register_Ns3BulkSendHelper_methods(root_module, cls):
    ## bulk-send-helper.h: ns3::BulkSendHelper::BulkSendHelper(ns3::BulkSendHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BulkSendHelper const &', 'arg0')])
    ## bulk-send-helper.h: ns3::BulkSendHelper::BulkSendHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
    ## bulk-send-helper.h: ns3::ApplicationContainer ns3::BulkSendHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## bulk-send-helper.h: ns3::ApplicationContainer ns3::BulkSendHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## bulk-send-helper.h: ns3::ApplicationContainer ns3::BulkSendHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## bulk-send-helper.h: void ns3::BulkSendHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3CsmaStarHelper_methods(root_module, cls):
    ## csma-star-helper.h: ns3::CsmaStarHelper::CsmaStarHelper(ns3::CsmaStarHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsmaStarHelper const &', 'arg0')])
    ## csma-star-helper.h: ns3::CsmaStarHelper::CsmaStarHelper(uint32_t numSpokes, ns3::CsmaHelper csmaHelper) [constructor]
    cls.add_constructor([param('uint32_t', 'numSpokes'), param('ns3::CsmaHelper', 'csmaHelper')])
    ## csma-star-helper.h: void ns3::CsmaStarHelper::AssignIpv4Addresses(ns3::Ipv4AddressHelper address) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4AddressHelper', 'address')])
    ## csma-star-helper.h: ns3::Ptr<ns3::Node> ns3::CsmaStarHelper::GetHub() const [member function]
    cls.add_method('GetHub', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## csma-star-helper.h: ns3::NetDeviceContainer ns3::CsmaStarHelper::GetHubDevices() const [member function]
    cls.add_method('GetHubDevices', 
                   'ns3::NetDeviceContainer', 
                   [], 
                   is_const=True)
    ## csma-star-helper.h: ns3::Ipv4Address ns3::CsmaStarHelper::GetHubIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetHubIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## csma-star-helper.h: ns3::NetDeviceContainer ns3::CsmaStarHelper::GetSpokeDevices() const [member function]
    cls.add_method('GetSpokeDevices', 
                   'ns3::NetDeviceContainer', 
                   [], 
                   is_const=True)
    ## csma-star-helper.h: ns3::Ipv4Address ns3::CsmaStarHelper::GetSpokeIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetSpokeIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## csma-star-helper.h: ns3::Ptr<ns3::Node> ns3::CsmaStarHelper::GetSpokeNode(uint32_t i) const [member function]
    cls.add_method('GetSpokeNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## csma-star-helper.h: void ns3::CsmaStarHelper::InstallStack(ns3::InternetStackHelper stack) [member function]
    cls.add_method('InstallStack', 
                   'void', 
                   [param('ns3::InternetStackHelper', 'stack')])
    ## csma-star-helper.h: uint32_t ns3::CsmaStarHelper::SpokeCount() const [member function]
    cls.add_method('SpokeCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
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

def register_Ns3MobilityHelper_methods(root_module, cls):
    ## mobility-helper.h: ns3::MobilityHelper::MobilityHelper(ns3::MobilityHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MobilityHelper const &', 'arg0')])
    ## mobility-helper.h: ns3::MobilityHelper::MobilityHelper() [constructor]
    cls.add_constructor([])
    ## mobility-helper.h: static void ns3::MobilityHelper::EnableAscii(std::ostream & os, uint32_t nodeid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('uint32_t', 'nodeid')], 
                   is_static=True)
    ## mobility-helper.h: static void ns3::MobilityHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## mobility-helper.h: static void ns3::MobilityHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
    ## mobility-helper.h: std::string ns3::MobilityHelper::GetMobilityModelType() const [member function]
    cls.add_method('GetMobilityModelType', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## mobility-helper.h: void ns3::MobilityHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## mobility-helper.h: void ns3::MobilityHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## mobility-helper.h: void ns3::MobilityHelper::Install(ns3::NodeContainer container) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer', 'container')], 
                   is_const=True)
    ## mobility-helper.h: void ns3::MobilityHelper::InstallAll() [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [])
    ## mobility-helper.h: void ns3::MobilityHelper::PopReferenceMobilityModel() [member function]
    cls.add_method('PopReferenceMobilityModel', 
                   'void', 
                   [])
    ## mobility-helper.h: void ns3::MobilityHelper::PushReferenceMobilityModel(ns3::Ptr<ns3::Object> reference) [member function]
    cls.add_method('PushReferenceMobilityModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'reference')])
    ## mobility-helper.h: void ns3::MobilityHelper::PushReferenceMobilityModel(std::string referenceName) [member function]
    cls.add_method('PushReferenceMobilityModel', 
                   'void', 
                   [param('std::string', 'referenceName')])
    ## mobility-helper.h: void ns3::MobilityHelper::SetMobilityModel(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue(), std::string n8="", ns3::AttributeValue const & v8=ns3::EmptyAttributeValue(), std::string n9="", ns3::AttributeValue const & v9=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMobilityModel', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n8', default_value='""'), param('ns3::AttributeValue const &', 'v8', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n9', default_value='""'), param('ns3::AttributeValue const &', 'v9', default_value='ns3::EmptyAttributeValue()')])
    ## mobility-helper.h: void ns3::MobilityHelper::SetPositionAllocator(ns3::Ptr<ns3::PositionAllocator> allocator) [member function]
    cls.add_method('SetPositionAllocator', 
                   'void', 
                   [param('ns3::Ptr< ns3::PositionAllocator >', 'allocator')])
    ## mobility-helper.h: void ns3::MobilityHelper::SetPositionAllocator(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue(), std::string n8="", ns3::AttributeValue const & v8=ns3::EmptyAttributeValue(), std::string n9="", ns3::AttributeValue const & v9=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPositionAllocator', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n8', default_value='""'), param('ns3::AttributeValue const &', 'v8', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n9', default_value='""'), param('ns3::AttributeValue const &', 'v9', default_value='ns3::EmptyAttributeValue()')])
    return

def register_Ns3Ns2MobilityHelper_methods(root_module, cls):
    ## ns2-mobility-helper.h: ns3::Ns2MobilityHelper::Ns2MobilityHelper(ns3::Ns2MobilityHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ns2MobilityHelper const &', 'arg0')])
    ## ns2-mobility-helper.h: ns3::Ns2MobilityHelper::Ns2MobilityHelper(std::string filename) [constructor]
    cls.add_constructor([param('std::string', 'filename')])
    ## ns2-mobility-helper.h: void ns3::Ns2MobilityHelper::Install() const [member function]
    cls.add_method('Install', 
                   'void', 
                   [], 
                   is_const=True)
    return

def register_Ns3OnOffHelper_methods(root_module, cls):
    ## on-off-helper.h: ns3::OnOffHelper::OnOffHelper(ns3::OnOffHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OnOffHelper const &', 'arg0')])
    ## on-off-helper.h: ns3::OnOffHelper::OnOffHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
    ## on-off-helper.h: ns3::ApplicationContainer ns3::OnOffHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## on-off-helper.h: ns3::ApplicationContainer ns3::OnOffHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## on-off-helper.h: ns3::ApplicationContainer ns3::OnOffHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## on-off-helper.h: void ns3::OnOffHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3PacketSinkHelper_methods(root_module, cls):
    ## packet-sink-helper.h: ns3::PacketSinkHelper::PacketSinkHelper(ns3::PacketSinkHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSinkHelper const &', 'arg0')])
    ## packet-sink-helper.h: ns3::PacketSinkHelper::PacketSinkHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
    ## packet-sink-helper.h: ns3::ApplicationContainer ns3::PacketSinkHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## packet-sink-helper.h: ns3::ApplicationContainer ns3::PacketSinkHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## packet-sink-helper.h: ns3::ApplicationContainer ns3::PacketSinkHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## packet-sink-helper.h: void ns3::PacketSinkHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3Ping6Helper_methods(root_module, cls):
    ## ping6-helper.h: ns3::Ping6Helper::Ping6Helper(ns3::Ping6Helper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ping6Helper const &', 'arg0')])
    ## ping6-helper.h: ns3::Ping6Helper::Ping6Helper() [constructor]
    cls.add_constructor([])
    ## ping6-helper.h: ns3::ApplicationContainer ns3::Ping6Helper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetIfIndex(uint32_t ifIndex) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t', 'ifIndex')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetLocal(ns3::Ipv6Address ip) [member function]
    cls.add_method('SetLocal', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ip')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetRemote(ns3::Ipv6Address ip) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ip')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetRoutersAddress(std::vector<ns3::Ipv6Address, std::allocator<ns3::Ipv6Address> > routers) [member function]
    cls.add_method('SetRoutersAddress', 
                   'void', 
                   [param('std::vector< ns3::Ipv6Address >', 'routers')])
    return

def register_Ns3PointToPointDumbbellHelper_methods(root_module, cls):
    ## point-to-point-dumbbell-helper.h: ns3::PointToPointDumbbellHelper::PointToPointDumbbellHelper(ns3::PointToPointDumbbellHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointDumbbellHelper const &', 'arg0')])
    ## point-to-point-dumbbell-helper.h: ns3::PointToPointDumbbellHelper::PointToPointDumbbellHelper(uint32_t nLeftLeaf, ns3::PointToPointHelper leftHelper, uint32_t nRightLeaf, ns3::PointToPointHelper rightHelper, ns3::PointToPointHelper bottleneckHelper) [constructor]
    cls.add_constructor([param('uint32_t', 'nLeftLeaf'), param('ns3::PointToPointHelper', 'leftHelper'), param('uint32_t', 'nRightLeaf'), param('ns3::PointToPointHelper', 'rightHelper'), param('ns3::PointToPointHelper', 'bottleneckHelper')])
    ## point-to-point-dumbbell-helper.h: void ns3::PointToPointDumbbellHelper::AssignIpv4Addresses(ns3::Ipv4AddressHelper leftIp, ns3::Ipv4AddressHelper rightIp, ns3::Ipv4AddressHelper routerIp) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4AddressHelper', 'leftIp'), param('ns3::Ipv4AddressHelper', 'rightIp'), param('ns3::Ipv4AddressHelper', 'routerIp')])
    ## point-to-point-dumbbell-helper.h: void ns3::PointToPointDumbbellHelper::BoundingBox(double ulx, double uly, double lrx, double lry) [member function]
    cls.add_method('BoundingBox', 
                   'void', 
                   [param('double', 'ulx'), param('double', 'uly'), param('double', 'lrx'), param('double', 'lry')])
    ## point-to-point-dumbbell-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointDumbbellHelper::GetLeft() const [member function]
    cls.add_method('GetLeft', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointDumbbellHelper::GetLeft(uint32_t i) const [member function]
    cls.add_method('GetLeft', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ipv4Address ns3::PointToPointDumbbellHelper::GetLeftIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetLeftIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointDumbbellHelper::GetRight() const [member function]
    cls.add_method('GetRight', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointDumbbellHelper::GetRight(uint32_t i) const [member function]
    cls.add_method('GetRight', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: ns3::Ipv4Address ns3::PointToPointDumbbellHelper::GetRightIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetRightIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: void ns3::PointToPointDumbbellHelper::InstallStack(ns3::InternetStackHelper stack) [member function]
    cls.add_method('InstallStack', 
                   'void', 
                   [param('ns3::InternetStackHelper', 'stack')])
    ## point-to-point-dumbbell-helper.h: uint32_t ns3::PointToPointDumbbellHelper::LeftCount() const [member function]
    cls.add_method('LeftCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## point-to-point-dumbbell-helper.h: uint32_t ns3::PointToPointDumbbellHelper::RightCount() const [member function]
    cls.add_method('RightCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3PointToPointGridHelper_methods(root_module, cls):
    ## point-to-point-grid-helper.h: ns3::PointToPointGridHelper::PointToPointGridHelper(ns3::PointToPointGridHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointGridHelper const &', 'arg0')])
    ## point-to-point-grid-helper.h: ns3::PointToPointGridHelper::PointToPointGridHelper(uint32_t nRows, uint32_t nCols, ns3::PointToPointHelper pointToPoint) [constructor]
    cls.add_constructor([param('uint32_t', 'nRows'), param('uint32_t', 'nCols'), param('ns3::PointToPointHelper', 'pointToPoint')])
    ## point-to-point-grid-helper.h: void ns3::PointToPointGridHelper::AssignIpv4Addresses(ns3::Ipv4AddressHelper rowIp, ns3::Ipv4AddressHelper colIp) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4AddressHelper', 'rowIp'), param('ns3::Ipv4AddressHelper', 'colIp')])
    ## point-to-point-grid-helper.h: void ns3::PointToPointGridHelper::BoundingBox(double ulx, double uly, double lrx, double lry) [member function]
    cls.add_method('BoundingBox', 
                   'void', 
                   [param('double', 'ulx'), param('double', 'uly'), param('double', 'lrx'), param('double', 'lry')])
    ## point-to-point-grid-helper.h: ns3::Ipv4Address ns3::PointToPointGridHelper::GetIpv4Address(uint32_t row, uint32_t col) [member function]
    cls.add_method('GetIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'row'), param('uint32_t', 'col')])
    ## point-to-point-grid-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointGridHelper::GetNode(uint32_t row, uint32_t col) [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'row'), param('uint32_t', 'col')])
    ## point-to-point-grid-helper.h: void ns3::PointToPointGridHelper::InstallStack(ns3::InternetStackHelper stack) [member function]
    cls.add_method('InstallStack', 
                   'void', 
                   [param('ns3::InternetStackHelper', 'stack')])
    return

def register_Ns3PointToPointHelper_methods(root_module, cls):
    ## point-to-point-helper.h: ns3::PointToPointHelper::PointToPointHelper(ns3::PointToPointHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointHelper const &', 'arg0')])
    ## point-to-point-helper.h: ns3::PointToPointHelper::PointToPointHelper() [constructor]
    cls.add_constructor([])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::Ptr<ns3::Node> a, ns3::Ptr<ns3::Node> b) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'a'), param('ns3::Ptr< ns3::Node >', 'b')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::Ptr<ns3::Node> a, std::string bName) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'a'), param('std::string', 'bName')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(std::string aName, ns3::Ptr<ns3::Node> b) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'aName'), param('ns3::Ptr< ns3::Node >', 'b')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(std::string aNode, std::string bNode) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'aNode'), param('std::string', 'bNode')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetChannelAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetChannelAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetDeviceAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## point-to-point-helper.h: void ns3::PointToPointHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PointToPointStarHelper_methods(root_module, cls):
    ## point-to-point-star-helper.h: ns3::PointToPointStarHelper::PointToPointStarHelper(ns3::PointToPointStarHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointStarHelper const &', 'arg0')])
    ## point-to-point-star-helper.h: ns3::PointToPointStarHelper::PointToPointStarHelper(uint32_t numSpokes, ns3::PointToPointHelper p2pHelper) [constructor]
    cls.add_constructor([param('uint32_t', 'numSpokes'), param('ns3::PointToPointHelper', 'p2pHelper')])
    ## point-to-point-star-helper.h: void ns3::PointToPointStarHelper::AssignIpv4Addresses(ns3::Ipv4AddressHelper address) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4AddressHelper', 'address')])
    ## point-to-point-star-helper.h: void ns3::PointToPointStarHelper::BoundingBox(double ulx, double uly, double lrx, double lry) [member function]
    cls.add_method('BoundingBox', 
                   'void', 
                   [param('double', 'ulx'), param('double', 'uly'), param('double', 'lrx'), param('double', 'lry')])
    ## point-to-point-star-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointStarHelper::GetHub() const [member function]
    cls.add_method('GetHub', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## point-to-point-star-helper.h: ns3::Ipv4Address ns3::PointToPointStarHelper::GetHubIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetHubIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-star-helper.h: ns3::Ipv4Address ns3::PointToPointStarHelper::GetSpokeIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetSpokeIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-star-helper.h: ns3::Ptr<ns3::Node> ns3::PointToPointStarHelper::GetSpokeNode(uint32_t i) const [member function]
    cls.add_method('GetSpokeNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-star-helper.h: void ns3::PointToPointStarHelper::InstallStack(ns3::InternetStackHelper stack) [member function]
    cls.add_method('InstallStack', 
                   'void', 
                   [param('ns3::InternetStackHelper', 'stack')])
    ## point-to-point-star-helper.h: uint32_t ns3::PointToPointStarHelper::SpokeCount() const [member function]
    cls.add_method('SpokeCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3TapBridgeHelper_methods(root_module, cls):
    ## tap-bridge-helper.h: ns3::TapBridgeHelper::TapBridgeHelper(ns3::TapBridgeHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TapBridgeHelper const &', 'arg0')])
    ## tap-bridge-helper.h: ns3::TapBridgeHelper::TapBridgeHelper() [constructor]
    cls.add_constructor([])
    ## tap-bridge-helper.h: ns3::TapBridgeHelper::TapBridgeHelper(ns3::Ipv4Address gateway) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'gateway')])
    ## tap-bridge-helper.h: ns3::Ptr<ns3::NetDevice> ns3::TapBridgeHelper::Install(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## tap-bridge-helper.h: ns3::Ptr<ns3::NetDevice> ns3::TapBridgeHelper::Install(std::string nodeName, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('std::string', 'nodeName'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## tap-bridge-helper.h: ns3::Ptr<ns3::NetDevice> ns3::TapBridgeHelper::Install(ns3::Ptr<ns3::Node> node, std::string ndName) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string', 'ndName')])
    ## tap-bridge-helper.h: ns3::Ptr<ns3::NetDevice> ns3::TapBridgeHelper::Install(std::string nodeName, std::string ndName) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('std::string', 'nodeName'), param('std::string', 'ndName')])
    ## tap-bridge-helper.h: ns3::Ptr<ns3::NetDevice> ns3::TapBridgeHelper::Install(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::NetDevice> nd, ns3::AttributeValue const & bridgeType) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('ns3::AttributeValue const &', 'bridgeType')])
    ## tap-bridge-helper.h: void ns3::TapBridgeHelper::SetAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    return

def register_Ns3TopologyReaderHelper_methods(root_module, cls):
    ## topology-reader-helper.h: ns3::TopologyReaderHelper::TopologyReaderHelper(ns3::TopologyReaderHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TopologyReaderHelper const &', 'arg0')])
    ## topology-reader-helper.h: ns3::TopologyReaderHelper::TopologyReaderHelper() [constructor]
    cls.add_constructor([])
    ## topology-reader-helper.h: ns3::Ptr<ns3::TopologyReader> ns3::TopologyReaderHelper::GetTopologyReader() [member function]
    cls.add_method('GetTopologyReader', 
                   'ns3::Ptr< ns3::TopologyReader >', 
                   [])
    ## topology-reader-helper.h: void ns3::TopologyReaderHelper::SetFileName(std::string const fileName) [member function]
    cls.add_method('SetFileName', 
                   'void', 
                   [param('std::string const', 'fileName')])
    ## topology-reader-helper.h: void ns3::TopologyReaderHelper::SetFileType(std::string const fileType) [member function]
    cls.add_method('SetFileType', 
                   'void', 
                   [param('std::string const', 'fileType')])
    return

def register_Ns3UdpClientHelper_methods(root_module, cls):
    ## udp-client-server-helper.h: ns3::UdpClientHelper::UdpClientHelper(ns3::UdpClientHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpClientHelper const &', 'arg0')])
    ## udp-client-server-helper.h: ns3::UdpClientHelper::UdpClientHelper() [constructor]
    cls.add_constructor([])
    ## udp-client-server-helper.h: ns3::UdpClientHelper::UdpClientHelper(ns3::Ipv4Address ip, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-client-server-helper.h: ns3::ApplicationContainer ns3::UdpClientHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## udp-client-server-helper.h: void ns3::UdpClientHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3UdpEchoClientHelper_methods(root_module, cls):
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper::UdpEchoClientHelper(ns3::UdpEchoClientHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoClientHelper const &', 'arg0')])
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper::UdpEchoClientHelper(ns3::Ipv4Address ip, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoClientHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoClientHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoClientHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetFill(ns3::Ptr<ns3::Application> app, std::string fill) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'app'), param('std::string', 'fill')])
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetFill(ns3::Ptr<ns3::Application> app, uint8_t fill, uint32_t dataLength) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'app'), param('uint8_t', 'fill'), param('uint32_t', 'dataLength')])
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetFill(ns3::Ptr<ns3::Application> app, uint8_t * fill, uint32_t fillLength, uint32_t dataLength) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'app'), param('uint8_t *', 'fill'), param('uint32_t', 'fillLength'), param('uint32_t', 'dataLength')])
    return

def register_Ns3UdpEchoServerHelper_methods(root_module, cls):
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper::UdpEchoServerHelper(ns3::UdpEchoServerHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoServerHelper const &', 'arg0')])
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper::UdpEchoServerHelper(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoServerHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoServerHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoServerHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## udp-echo-helper.h: void ns3::UdpEchoServerHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3UdpServerHelper_methods(root_module, cls):
    ## udp-client-server-helper.h: ns3::UdpServerHelper::UdpServerHelper(ns3::UdpServerHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpServerHelper const &', 'arg0')])
    ## udp-client-server-helper.h: ns3::UdpServerHelper::UdpServerHelper() [constructor]
    cls.add_constructor([])
    ## udp-client-server-helper.h: ns3::UdpServerHelper::UdpServerHelper(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## udp-client-server-helper.h: ns3::Ptr<ns3::UdpServer> ns3::UdpServerHelper::GetServer() [member function]
    cls.add_method('GetServer', 
                   'ns3::Ptr< ns3::UdpServer >', 
                   [])
    ## udp-client-server-helper.h: ns3::ApplicationContainer ns3::UdpServerHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## udp-client-server-helper.h: void ns3::UdpServerHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3UdpTraceClientHelper_methods(root_module, cls):
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper::UdpTraceClientHelper(ns3::UdpTraceClientHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpTraceClientHelper const &', 'arg0')])
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper::UdpTraceClientHelper() [constructor]
    cls.add_constructor([])
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper::UdpTraceClientHelper(ns3::Ipv4Address ip, uint16_t port, std::string filename) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port'), param('std::string', 'filename')])
    ## udp-client-server-helper.h: ns3::ApplicationContainer ns3::UdpTraceClientHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## udp-client-server-helper.h: void ns3::UdpTraceClientHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3V4PingHelper_methods(root_module, cls):
    ## v4ping-helper.h: ns3::V4PingHelper::V4PingHelper(ns3::V4PingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::V4PingHelper const &', 'arg0')])
    ## v4ping-helper.h: ns3::V4PingHelper::V4PingHelper(ns3::Ipv4Address remote) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'remote')])
    ## v4ping-helper.h: ns3::ApplicationContainer ns3::V4PingHelper::Install(ns3::NodeContainer nodes) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'nodes')], 
                   is_const=True)
    ## v4ping-helper.h: ns3::ApplicationContainer ns3::V4PingHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## v4ping-helper.h: ns3::ApplicationContainer ns3::V4PingHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## v4ping-helper.h: void ns3::V4PingHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3WifiHelper_methods(root_module, cls):
    ## wifi-helper.h: ns3::WifiHelper::WifiHelper(ns3::WifiHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiHelper const &', 'arg0')])
    ## wifi-helper.h: ns3::WifiHelper::WifiHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h: static ns3::WifiHelper ns3::WifiHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::WifiHelper', 
                   [], 
                   is_static=True)
    ## wifi-helper.h: static void ns3::WifiHelper::EnableLogComponents() [member function]
    cls.add_method('EnableLogComponents', 
                   'void', 
                   [], 
                   is_static=True)
    ## wifi-helper.h: ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## wifi-helper.h: ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## wifi-helper.h: ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('std::string', 'nodeName')], 
                   is_const=True)
    ## wifi-helper.h: void ns3::WifiHelper::SetRemoteStationManager(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetRemoteStationManager', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## wifi-helper.h: void ns3::WifiHelper::SetStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('SetStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')])
    return

def register_Ns3WifiMacHelper_methods(root_module, cls):
    ## wifi-helper.h: ns3::WifiMacHelper::WifiMacHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h: ns3::WifiMacHelper::WifiMacHelper(ns3::WifiMacHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMacHelper const &', 'arg0')])
    ## wifi-helper.h: ns3::Ptr<ns3::WifiMac> ns3::WifiMacHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3WifiPhyHelper_methods(root_module, cls):
    ## wifi-helper.h: ns3::WifiPhyHelper::WifiPhyHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h: ns3::WifiPhyHelper::WifiPhyHelper(ns3::WifiPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhyHelper const &', 'arg0')])
    ## wifi-helper.h: ns3::Ptr<ns3::WifiPhy> ns3::WifiPhyHelper::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WifiNetDevice> device) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::WifiNetDevice >', 'device')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3WimaxHelper_methods(root_module, cls):
    ## wimax-helper.h: ns3::WimaxHelper::WimaxHelper(ns3::WimaxHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WimaxHelper const &', 'arg0')])
    ## wimax-helper.h: ns3::WimaxHelper::WimaxHelper() [constructor]
    cls.add_constructor([])
    ## wimax-helper.h: ns3::Ptr<ns3::BSScheduler> ns3::WimaxHelper::CreateBSScheduler(ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('CreateBSScheduler', 
                   'ns3::Ptr< ns3::BSScheduler >', 
                   [param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxHelper::CreatePhy(ns3::WimaxHelper::PhyType phyType) [member function]
    cls.add_method('CreatePhy', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [param('ns3::WimaxHelper::PhyType', 'phyType')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxHelper::CreatePhy(ns3::WimaxHelper::PhyType phyType, char * SNRTraceFilePath, bool activateLoss) [member function]
    cls.add_method('CreatePhy', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [param('ns3::WimaxHelper::PhyType', 'phyType'), param('char *', 'SNRTraceFilePath'), param('bool', 'activateLoss')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxHelper::CreatePhyWithoutChannel(ns3::WimaxHelper::PhyType phyType) [member function]
    cls.add_method('CreatePhyWithoutChannel', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [param('ns3::WimaxHelper::PhyType', 'phyType')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxHelper::CreatePhyWithoutChannel(ns3::WimaxHelper::PhyType phyType, char * SNRTraceFilePath, bool activateLoss) [member function]
    cls.add_method('CreatePhyWithoutChannel', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [param('ns3::WimaxHelper::PhyType', 'phyType'), param('char *', 'SNRTraceFilePath'), param('bool', 'activateLoss')])
    ## wimax-helper.h: ns3::ServiceFlow ns3::WimaxHelper::CreateServiceFlow(ns3::ServiceFlow::Direction direction, ns3::ServiceFlow::SchedulingType schedulinType, ns3::IpcsClassifierRecord classifier) [member function]
    cls.add_method('CreateServiceFlow', 
                   'ns3::ServiceFlow', 
                   [param('ns3::ServiceFlow::Direction', 'direction'), param('ns3::ServiceFlow::SchedulingType', 'schedulinType'), param('ns3::IpcsClassifierRecord', 'classifier')])
    ## wimax-helper.h: ns3::Ptr<ns3::UplinkScheduler> ns3::WimaxHelper::CreateUplinkScheduler(ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('CreateUplinkScheduler', 
                   'ns3::Ptr< ns3::UplinkScheduler >', 
                   [param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: static void ns3::WimaxHelper::EnableAsciiForConnection(ns3::Ptr<ns3::OutputStreamWrapper> oss, uint32_t nodeid, uint32_t deviceid, char * netdevice, char * connection) [member function]
    cls.add_method('EnableAsciiForConnection', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'oss'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('char *', 'netdevice'), param('char *', 'connection')], 
                   is_static=True)
    ## wimax-helper.h: static void ns3::WimaxHelper::EnableLogComponents() [member function]
    cls.add_method('EnableLogComponents', 
                   'void', 
                   [], 
                   is_static=True)
    ## wimax-helper.h: ns3::NetDeviceContainer ns3::WimaxHelper::Install(ns3::NodeContainer c, ns3::WimaxHelper::NetDeviceType type, ns3::WimaxHelper::PhyType phyType, ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::WimaxHelper::NetDeviceType', 'type'), param('ns3::WimaxHelper::PhyType', 'phyType'), param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: ns3::NetDeviceContainer ns3::WimaxHelper::Install(ns3::NodeContainer c, ns3::WimaxHelper::NetDeviceType deviceType, ns3::WimaxHelper::PhyType phyType, ns3::Ptr<ns3::WimaxChannel> channel, ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::WimaxHelper::NetDeviceType', 'deviceType'), param('ns3::WimaxHelper::PhyType', 'phyType'), param('ns3::Ptr< ns3::WimaxChannel >', 'channel'), param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: ns3::NetDeviceContainer ns3::WimaxHelper::Install(ns3::NodeContainer c, ns3::WimaxHelper::NetDeviceType deviceType, ns3::WimaxHelper::PhyType phyType, ns3::WimaxHelper::SchedulerType schedulerType, double frameDuration) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::WimaxHelper::NetDeviceType', 'deviceType'), param('ns3::WimaxHelper::PhyType', 'phyType'), param('ns3::WimaxHelper::SchedulerType', 'schedulerType'), param('double', 'frameDuration')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxNetDevice> ns3::WimaxHelper::Install(ns3::Ptr<ns3::Node> node, ns3::WimaxHelper::NetDeviceType deviceType, ns3::WimaxHelper::PhyType phyType, ns3::Ptr<ns3::WimaxChannel> channel, ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::WimaxNetDevice >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::WimaxHelper::NetDeviceType', 'deviceType'), param('ns3::WimaxHelper::PhyType', 'phyType'), param('ns3::Ptr< ns3::WimaxChannel >', 'channel'), param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: void ns3::WimaxHelper::SetPropagationLossModel(ns3::SimpleOfdmWimaxChannel::PropModel propagationModel) [member function]
    cls.add_method('SetPropagationLossModel', 
                   'void', 
                   [param('ns3::SimpleOfdmWimaxChannel::PropModel', 'propagationModel')])
    ## wimax-helper.h: void ns3::WimaxHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## wimax-helper.h: void ns3::WimaxHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename, bool promiscuous) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename'), param('bool', 'promiscuous')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3YansWifiChannelHelper_methods(root_module, cls):
    ## yans-wifi-helper.h: ns3::YansWifiChannelHelper::YansWifiChannelHelper(ns3::YansWifiChannelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansWifiChannelHelper const &', 'arg0')])
    ## yans-wifi-helper.h: ns3::YansWifiChannelHelper::YansWifiChannelHelper() [constructor]
    cls.add_constructor([])
    ## yans-wifi-helper.h: void ns3::YansWifiChannelHelper::AddPropagationLoss(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('AddPropagationLoss', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## yans-wifi-helper.h: ns3::Ptr<ns3::YansWifiChannel> ns3::YansWifiChannelHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::YansWifiChannel >', 
                   [], 
                   is_const=True)
    ## yans-wifi-helper.h: static ns3::YansWifiChannelHelper ns3::YansWifiChannelHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::YansWifiChannelHelper', 
                   [], 
                   is_static=True)
    ## yans-wifi-helper.h: void ns3::YansWifiChannelHelper::SetPropagationDelay(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPropagationDelay', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    return

def register_Ns3YansWifiPhyHelper_methods(root_module, cls):
    ## yans-wifi-helper.h: ns3::YansWifiPhyHelper::YansWifiPhyHelper(ns3::YansWifiPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansWifiPhyHelper const &', 'arg0')])
    ## yans-wifi-helper.h: ns3::YansWifiPhyHelper::YansWifiPhyHelper() [constructor]
    cls.add_constructor([])
    ## yans-wifi-helper.h: static ns3::YansWifiPhyHelper ns3::YansWifiPhyHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::YansWifiPhyHelper', 
                   [], 
                   is_static=True)
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')])
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::SetChannel(ns3::Ptr<ns3::YansWifiChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::YansWifiChannel >', 'channel')])
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::SetChannel(std::string channelName) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('std::string', 'channelName')])
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::SetErrorRateModel(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetErrorRateModel', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::SetPcapDataLinkType(ns3::YansWifiPhyHelper::SupportedPcapDataLinkTypes dlt) [member function]
    cls.add_method('SetPcapDataLinkType', 
                   'void', 
                   [param('ns3::YansWifiPhyHelper::SupportedPcapDataLinkTypes', 'dlt')])
    ## yans-wifi-helper.h: ns3::Ptr<ns3::WifiPhy> ns3::YansWifiPhyHelper::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WifiNetDevice> device) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::WifiNetDevice >', 'device')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AodvHelper_methods(root_module, cls):
    ## aodv-helper.h: ns3::AodvHelper::AodvHelper(ns3::AodvHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AodvHelper const &', 'arg0')])
    ## aodv-helper.h: ns3::AodvHelper::AodvHelper() [constructor]
    cls.add_constructor([])
    ## aodv-helper.h: ns3::AodvHelper * ns3::AodvHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::AodvHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-helper.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::AodvHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## aodv-helper.h: void ns3::AodvHelper::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3CsmaHelper_methods(root_module, cls):
    ## csma-helper.h: ns3::CsmaHelper::CsmaHelper(ns3::CsmaHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsmaHelper const &', 'arg0')])
    ## csma-helper.h: ns3::CsmaHelper::CsmaHelper() [constructor]
    cls.add_constructor([])
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(std::string name) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::CsmaChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::CsmaChannel >', 'channel')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::Ptr<ns3::Node> node, std::string channelName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string', 'channelName')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(std::string nodeName, ns3::Ptr<ns3::CsmaChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName'), param('ns3::Ptr< ns3::CsmaChannel >', 'channel')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(std::string nodeName, std::string channelName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName'), param('std::string', 'channelName')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::NodeContainer const & c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer const &', 'c')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::NodeContainer const & c, ns3::Ptr<ns3::CsmaChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer const &', 'c'), param('ns3::Ptr< ns3::CsmaChannel >', 'channel')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::NodeContainer const & c, std::string channelName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer const &', 'c'), param('std::string', 'channelName')], 
                   is_const=True)
    ## csma-helper.h: void ns3::CsmaHelper::SetChannelAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetChannelAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## csma-helper.h: void ns3::CsmaHelper::SetDeviceAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## csma-helper.h: void ns3::CsmaHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## csma-helper.h: void ns3::CsmaHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## csma-helper.h: void ns3::CsmaHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3EmuHelper_methods(root_module, cls):
    ## emu-helper.h: ns3::EmuHelper::EmuHelper(ns3::EmuHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EmuHelper const &', 'arg0')])
    ## emu-helper.h: ns3::EmuHelper::EmuHelper() [constructor]
    cls.add_constructor([])
    ## emu-helper.h: ns3::NetDeviceContainer ns3::EmuHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## emu-helper.h: ns3::NetDeviceContainer ns3::EmuHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## emu-helper.h: ns3::NetDeviceContainer ns3::EmuHelper::Install(ns3::NodeContainer const & c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer const &', 'c')], 
                   is_const=True)
    ## emu-helper.h: void ns3::EmuHelper::SetAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## emu-helper.h: void ns3::EmuHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## emu-helper.h: void ns3::EmuHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## emu-helper.h: void ns3::EmuHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Ipv4NixVectorHelper_methods(root_module, cls):
    ## ipv4-nix-vector-helper.h: ns3::Ipv4NixVectorHelper::Ipv4NixVectorHelper() [constructor]
    cls.add_constructor([])
    ## ipv4-nix-vector-helper.h: ns3::Ipv4NixVectorHelper::Ipv4NixVectorHelper(ns3::Ipv4NixVectorHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4NixVectorHelper const &', 'arg0')])
    ## ipv4-nix-vector-helper.h: ns3::Ipv4NixVectorHelper * ns3::Ipv4NixVectorHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ipv4NixVectorHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-nix-vector-helper.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4NixVectorHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3NqosWifiMacHelper_methods(root_module, cls):
    ## nqos-wifi-mac-helper.h: ns3::NqosWifiMacHelper::NqosWifiMacHelper(ns3::NqosWifiMacHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NqosWifiMacHelper const &', 'arg0')])
    ## nqos-wifi-mac-helper.h: ns3::NqosWifiMacHelper::NqosWifiMacHelper() [constructor]
    cls.add_constructor([])
    ## nqos-wifi-mac-helper.h: static ns3::NqosWifiMacHelper ns3::NqosWifiMacHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::NqosWifiMacHelper', 
                   [], 
                   is_static=True)
    ## nqos-wifi-mac-helper.h: void ns3::NqosWifiMacHelper::SetType(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## nqos-wifi-mac-helper.h: ns3::Ptr<ns3::WifiMac> ns3::NqosWifiMacHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3QosWifiMacHelper_methods(root_module, cls):
    ## qos-wifi-mac-helper.h: ns3::QosWifiMacHelper::QosWifiMacHelper(ns3::QosWifiMacHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::QosWifiMacHelper const &', 'arg0')])
    ## qos-wifi-mac-helper.h: ns3::QosWifiMacHelper::QosWifiMacHelper() [constructor]
    cls.add_constructor([])
    ## qos-wifi-mac-helper.h: static ns3::QosWifiMacHelper ns3::QosWifiMacHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::QosWifiMacHelper', 
                   [], 
                   is_static=True)
    ## qos-wifi-mac-helper.h: void ns3::QosWifiMacHelper::SetBlockAckInactivityTimeoutForAc(ns3::AcIndex ac, uint16_t timeout) [member function]
    cls.add_method('SetBlockAckInactivityTimeoutForAc', 
                   'void', 
                   [param('ns3::AcIndex', 'ac'), param('uint16_t', 'timeout')])
    ## qos-wifi-mac-helper.h: void ns3::QosWifiMacHelper::SetBlockAckThresholdForAc(ns3::AcIndex ac, uint8_t threshold) [member function]
    cls.add_method('SetBlockAckThresholdForAc', 
                   'void', 
                   [param('ns3::AcIndex', 'ac'), param('uint8_t', 'threshold')])
    ## qos-wifi-mac-helper.h: void ns3::QosWifiMacHelper::SetMsduAggregatorForAc(ns3::AcIndex ac, std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMsduAggregatorForAc', 
                   'void', 
                   [param('ns3::AcIndex', 'ac'), param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()')])
    ## qos-wifi-mac-helper.h: void ns3::QosWifiMacHelper::SetType(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## qos-wifi-mac-helper.h: ns3::Ptr<ns3::WifiMac> ns3::QosWifiMacHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3AthstatsWifiTraceSink_methods(root_module, cls):
    ## athstats-helper.h: ns3::AthstatsWifiTraceSink::AthstatsWifiTraceSink(ns3::AthstatsWifiTraceSink const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AthstatsWifiTraceSink const &', 'arg0')])
    ## athstats-helper.h: ns3::AthstatsWifiTraceSink::AthstatsWifiTraceSink() [constructor]
    cls.add_constructor([])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::DevRxTrace(std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DevRxTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::DevTxTrace(std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DevTxTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## athstats-helper.h: static ns3::TypeId ns3::AthstatsWifiTraceSink::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::Open(std::string const & name) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string const &', 'name')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::PhyRxErrorTrace(std::string context, ns3::Ptr<ns3::Packet const> packet, double snr) [member function]
    cls.add_method('PhyRxErrorTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('double', 'snr')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::PhyRxOkTrace(std::string context, ns3::Ptr<ns3::Packet const> packet, double snr, ns3::WifiMode mode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('PhyRxOkTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('double', 'snr'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::PhyStateTrace(std::string context, ns3::Time start, ns3::Time duration, ns3::WifiPhy::State state) [member function]
    cls.add_method('PhyStateTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Time', 'start'), param('ns3::Time', 'duration'), param('ns3::WifiPhy::State', 'state')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::PhyTxTrace(std::string context, ns3::Ptr<ns3::Packet const> packet, ns3::WifiMode mode, ns3::WifiPreamble preamble, uint8_t txPower) [member function]
    cls.add_method('PhyTxTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble'), param('uint8_t', 'txPower')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::TxDataFailedTrace(std::string context, ns3::Mac48Address address) [member function]
    cls.add_method('TxDataFailedTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Mac48Address', 'address')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::TxFinalDataFailedTrace(std::string context, ns3::Mac48Address address) [member function]
    cls.add_method('TxFinalDataFailedTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Mac48Address', 'address')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::TxFinalRtsFailedTrace(std::string context, ns3::Mac48Address address) [member function]
    cls.add_method('TxFinalRtsFailedTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Mac48Address', 'address')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::TxRtsFailedTrace(std::string context, ns3::Mac48Address address) [member function]
    cls.add_method('TxRtsFailedTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Mac48Address', 'address')])
    return

def register_Ns3CanvasLocation_methods(root_module, cls):
    ## canvas-location.h: ns3::CanvasLocation::CanvasLocation(ns3::CanvasLocation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CanvasLocation const &', 'arg0')])
    ## canvas-location.h: ns3::CanvasLocation::CanvasLocation() [constructor]
    cls.add_constructor([])
    ## canvas-location.h: ns3::Vector ns3::CanvasLocation::GetLocation() const [member function]
    cls.add_method('GetLocation', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## canvas-location.h: static ns3::TypeId ns3::CanvasLocation::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## canvas-location.h: void ns3::CanvasLocation::SetLocation(ns3::Vector const & location) [member function]
    cls.add_method('SetLocation', 
                   'void', 
                   [param('ns3::Vector const &', 'location')])
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
    return

def register_functions_ns3_dsdv(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

