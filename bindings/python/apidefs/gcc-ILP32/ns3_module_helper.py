from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## animation-interface.h: ns3::AnimationInterface [class]
    module.add_class('AnimationInterface')
    ## application-container.h: ns3::ApplicationContainer [class]
    module.add_class('ApplicationContainer')
    ## trace-helper.h: ns3::AsciiTraceHelper [class]
    module.add_class('AsciiTraceHelper', allow_subclassing=False)
    ## athstats-helper.h: ns3::AthstatsHelper [class]
    module.add_class('AthstatsHelper', allow_subclassing=False)
    ## bridge-helper.h: ns3::BridgeHelper [class]
    module.add_class('BridgeHelper', allow_subclassing=False)
    ## csma-star-helper.h: ns3::CsmaStarHelper [class]
    module.add_class('CsmaStarHelper', allow_subclassing=False)
    ## flow-monitor-helper.h: ns3::FlowMonitorHelper [class]
    module.add_class('FlowMonitorHelper', allow_subclassing=False)
    ## ipv4-address-helper.h: ns3::Ipv4AddressHelper [class]
    module.add_class('Ipv4AddressHelper', allow_subclassing=False)
    ## ipv4-interface-container.h: ns3::Ipv4InterfaceContainer [class]
    module.add_class('Ipv4InterfaceContainer')
    ## ipv4-routing-helper.h: ns3::Ipv4RoutingHelper [class]
    module.add_class('Ipv4RoutingHelper', allow_subclassing=False)
    ## ipv4-static-routing-helper.h: ns3::Ipv4StaticRoutingHelper [class]
    module.add_class('Ipv4StaticRoutingHelper', allow_subclassing=False, parent=root_module['ns3::Ipv4RoutingHelper'])
    ## ipv6-address-helper.h: ns3::Ipv6AddressHelper [class]
    module.add_class('Ipv6AddressHelper', allow_subclassing=False)
    ## ipv6-interface-container.h: ns3::Ipv6InterfaceContainer [class]
    module.add_class('Ipv6InterfaceContainer')
    ## ipv6-routing-helper.h: ns3::Ipv6RoutingHelper [class]
    module.add_class('Ipv6RoutingHelper', allow_subclassing=False)
    ## ipv6-static-routing-helper.h: ns3::Ipv6StaticRoutingHelper [class]
    module.add_class('Ipv6StaticRoutingHelper', allow_subclassing=False, parent=root_module['ns3::Ipv6RoutingHelper'])
    ## mesh-helper.h: ns3::MeshHelper [class]
    module.add_class('MeshHelper', allow_subclassing=False)
    ## mesh-helper.h: ns3::MeshHelper::ChannelPolicy [enumeration]
    module.add_enum('ChannelPolicy', ['SPREAD_CHANNELS', 'ZERO_CHANNEL'], outer_class=root_module['ns3::MeshHelper'])
    ## mobility-helper.h: ns3::MobilityHelper [class]
    module.add_class('MobilityHelper', allow_subclassing=False)
    ## net-device-container.h: ns3::NetDeviceContainer [class]
    module.add_class('NetDeviceContainer')
    ## node-container.h: ns3::NodeContainer [class]
    module.add_class('NodeContainer')
    ## ns2-mobility-helper.h: ns3::Ns2MobilityHelper [class]
    module.add_class('Ns2MobilityHelper', allow_subclassing=False)
    ## olsr-helper.h: ns3::OlsrHelper [class]
    module.add_class('OlsrHelper', allow_subclassing=False, parent=root_module['ns3::Ipv4RoutingHelper'])
    ## on-off-helper.h: ns3::OnOffHelper [class]
    module.add_class('OnOffHelper', allow_subclassing=False)
    ## packet-sink-helper.h: ns3::PacketSinkHelper [class]
    module.add_class('PacketSinkHelper', allow_subclassing=False)
    ## packet-socket-helper.h: ns3::PacketSocketHelper [class]
    module.add_class('PacketSocketHelper', allow_subclassing=False)
    ## trace-helper.h: ns3::PcapHelper [class]
    module.add_class('PcapHelper', allow_subclassing=False)
    ## trace-helper.h: ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_NULL'], outer_class=root_module['ns3::PcapHelper'])
    ## trace-helper.h: ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_EN10MB'], outer_class=root_module['ns3::PcapHelper'])
    ## trace-helper.h: ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_PPP'], outer_class=root_module['ns3::PcapHelper'])
    ## trace-helper.h: ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_RAW'], outer_class=root_module['ns3::PcapHelper'])
    ## trace-helper.h: ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_IEEE802_11'], outer_class=root_module['ns3::PcapHelper'])
    ## trace-helper.h: ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_PRISM_HEADER'], outer_class=root_module['ns3::PcapHelper'])
    ## trace-helper.h: ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_IEEE802_11_RADIO'], outer_class=root_module['ns3::PcapHelper'])
    ## trace-helper.h: ns3::PcapHelperForDevice [class]
    module.add_class('PcapHelperForDevice', allow_subclassing=True)
    ## trace-helper.h: ns3::PcapHelperForIpv4 [class]
    module.add_class('PcapHelperForIpv4', allow_subclassing=True)
    ## ping6-helper.h: ns3::Ping6Helper [class]
    module.add_class('Ping6Helper', allow_subclassing=False)
    ## point-to-point-dumbbell-helper.h: ns3::PointToPointDumbbellHelper [class]
    module.add_class('PointToPointDumbbellHelper', allow_subclassing=False)
    ## point-to-point-grid-helper.h: ns3::PointToPointGridHelper [class]
    module.add_class('PointToPointGridHelper', allow_subclassing=False)
    ## point-to-point-star-helper.h: ns3::PointToPointStarHelper [class]
    module.add_class('PointToPointStarHelper', allow_subclassing=False)
    ## tap-bridge-helper.h: ns3::TapBridgeHelper [class]
    module.add_class('TapBridgeHelper', allow_subclassing=False)
    ## trace-helper.h: ns3::TraceHelperForDevice [class]
    module.add_class('TraceHelperForDevice', parent=root_module['ns3::PcapHelperForDevice'])
    ## udp-client-server-helper.h: ns3::UdpClientHelper [class]
    module.add_class('UdpClientHelper', allow_subclassing=False)
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper [class]
    module.add_class('UdpEchoClientHelper', allow_subclassing=False)
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper [class]
    module.add_class('UdpEchoServerHelper', allow_subclassing=False)
    ## udp-client-server-helper.h: ns3::UdpServerHelper [class]
    module.add_class('UdpServerHelper', allow_subclassing=False)
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper [class]
    module.add_class('UdpTraceClientHelper', allow_subclassing=False)
    ## v4ping-helper.h: ns3::V4PingHelper [class]
    module.add_class('V4PingHelper', allow_subclassing=False)
    ## wifi-helper.h: ns3::WifiHelper [class]
    module.add_class('WifiHelper', allow_subclassing=False)
    ## wifi-helper.h: ns3::WifiMacHelper [class]
    module.add_class('WifiMacHelper', allow_subclassing=False)
    ## wifi-helper.h: ns3::WifiPhyHelper [class]
    module.add_class('WifiPhyHelper', allow_subclassing=False, parent=root_module['ns3::TraceHelperForDevice'])
