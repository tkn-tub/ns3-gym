from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## packetbb.h: ns3::PbbAddressLength [enumeration]
    module.add_enum('PbbAddressLength', ['IPV4', 'IPV6'])
    ## ethernet-header.h: ns3::ethernet_header_t [enumeration]
    module.add_enum('ethernet_header_t', ['LENGTH', 'VLAN', 'QINQ'])
    ## address.h: ns3::Address [class]
    module.add_class('Address')
    ## address.h: ns3::Address::MaxSize_e [enumeration]
    module.add_enum('MaxSize_e', ['MAX_SIZE'], outer_class=root_module['ns3::Address'])
    ## channel-list.h: ns3::ChannelList [class]
    module.add_class('ChannelList')
    ## inet6-socket-address.h: ns3::Inet6SocketAddress [class]
    module.add_class('Inet6SocketAddress')
    ## inet6-socket-address.h: ns3::Inet6SocketAddress [class]
    root_module['ns3::Inet6SocketAddress'].implicitly_converts_to(root_module['ns3::Address'])
    ## inet-socket-address.h: ns3::InetSocketAddress [class]
    module.add_class('InetSocketAddress')
    ## inet-socket-address.h: ns3::InetSocketAddress [class]
    root_module['ns3::InetSocketAddress'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv4-address.h: ns3::Ipv4Address [class]
    module.add_class('Ipv4Address')
    ## ipv4-address.h: ns3::Ipv4Address [class]
    root_module['ns3::Ipv4Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv4-address-generator.h: ns3::Ipv4AddressGenerator [class]
    module.add_class('Ipv4AddressGenerator')
    ## ipv4-interface-address.h: ns3::Ipv4InterfaceAddress [class]
    module.add_class('Ipv4InterfaceAddress')
    ## ipv4-interface-address.h: ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e [enumeration]
    module.add_enum('InterfaceAddressScope_e', ['HOST', 'LINK', 'GLOBAL'], outer_class=root_module['ns3::Ipv4InterfaceAddress'])
    ## ipv4-address.h: ns3::Ipv4Mask [class]
    module.add_class('Ipv4Mask')
    ## ipv6-address.h: ns3::Ipv6Address [class]
    module.add_class('Ipv6Address')
    ## ipv6-address.h: ns3::Ipv6Address [class]
    root_module['ns3::Ipv6Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress [class]
    module.add_class('Ipv6InterfaceAddress')
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::State_e [enumeration]
    module.add_enum('State_e', ['TENTATIVE', 'DEPRECATED', 'PREFERRED', 'PERMANENT', 'HOMEADDRESS', 'TENTATIVE_OPTIMISTIC', 'INVALID'], outer_class=root_module['ns3::Ipv6InterfaceAddress'])
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::Scope_e [enumeration]
    module.add_enum('Scope_e', ['HOST', 'LINKLOCAL', 'GLOBAL'], outer_class=root_module['ns3::Ipv6InterfaceAddress'])
    ## ipv6-address.h: ns3::Ipv6Prefix [class]
    module.add_class('Ipv6Prefix')
    ## mac48-address.h: ns3::Mac48Address [class]
    module.add_class('Mac48Address')
    ## mac48-address.h: ns3::Mac48Address [class]
    root_module['ns3::Mac48Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## mac64-address.h: ns3::Mac64Address [class]
    module.add_class('Mac64Address')
    ## mac64-address.h: ns3::Mac64Address [class]
    root_module['ns3::Mac64Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## node-list.h: ns3::NodeList [class]
    module.add_class('NodeList')
    ## packet-socket-address.h: ns3::PacketSocketAddress [class]
    module.add_class('PacketSocketAddress')
    ## packet-socket-address.h: ns3::PacketSocketAddress [class]
    root_module['ns3::PacketSocketAddress'].implicitly_converts_to(root_module['ns3::Address'])
    ## packetbb.h: ns3::PbbAddressTlvBlock [class]
    module.add_class('PbbAddressTlvBlock')
    ## packetbb.h: ns3::PbbTlvBlock [class]
    module.add_class('PbbTlvBlock')
    ## ipv4-header.h: ns3::Ipv4Header [class]
    module.add_class('Ipv4Header', parent=root_module['ns3::Header'])
    ## ipv4-packet-info-tag.h: ns3::Ipv4PacketInfoTag [class]
    module.add_class('Ipv4PacketInfoTag', parent=root_module['ns3::Tag'])
    ## ipv6-header.h: ns3::Ipv6Header [class]
    module.add_class('Ipv6Header', parent=root_module['ns3::Header'])
    ## ipv6-header.h: ns3::Ipv6Header::NextHeader_e [enumeration]
    module.add_enum('NextHeader_e', ['IPV6_EXT_HOP_BY_HOP', 'IPV6_IPV4', 'IPV6_TCP', 'IPV6_UDP', 'IPV6_IPV6', 'IPV6_EXT_ROUTING', 'IPV6_EXT_FRAGMENTATION', 'IPV6_EXT_CONFIDENTIALITY', 'IPV6_EXT_AUTHENTIFICATION', 'IPV6_ICMPV6', 'IPV6_EXT_END', 'IPV6_EXT_DESTINATION', 'IPV6_SCTP', 'IPV6_EXT_MOBILITY', 'IPV6_UDP_LITE'], outer_class=root_module['ns3::Ipv6Header'])
    ## ipv6-packet-info-tag.h: ns3::Ipv6PacketInfoTag [class]
    module.add_class('Ipv6PacketInfoTag', parent=root_module['ns3::Tag'])
    ## llc-snap-header.h: ns3::LlcSnapHeader [class]
    module.add_class('LlcSnapHeader', parent=root_module['ns3::Header'])
    ## queue.h: ns3::Queue [class]
    module.add_class('Queue', parent=root_module['ns3::Object'])
    ## radiotap-header.h: ns3::RadiotapHeader [class]
    module.add_class('RadiotapHeader', parent=root_module['ns3::Header'])
    ## radiotap-header.h: ns3::RadiotapHeader [enumeration]
    module.add_enum('', ['FRAME_FLAG_NONE', 'FRAME_FLAG_CFP', 'FRAME_FLAG_SHORT_PREAMBLE', 'FRAME_FLAG_WEP', 'FRAME_FLAG_FRAGMENTED', 'FRAME_FLAG_FCS_INCLUDED', 'FRAME_FLAG_DATA_PADDING', 'FRAME_FLAG_BAD_FCS', 'FRAME_FLAG_SHORT_GUARD'], outer_class=root_module['ns3::RadiotapHeader'])
    ## radiotap-header.h: ns3::RadiotapHeader [enumeration]
    module.add_enum('', ['CHANNEL_FLAG_NONE', 'CHANNEL_FLAG_TURBO', 'CHANNEL_FLAG_CCK', 'CHANNEL_FLAG_OFDM', 'CHANNEL_FLAG_SPECTRUM_2GHZ', 'CHANNEL_FLAG_SPECTRUM_5GHZ', 'CHANNEL_FLAG_PASSIVE', 'CHANNEL_FLAG_DYNAMIC', 'CHANNEL_FLAG_GFSK'], outer_class=root_module['ns3::RadiotapHeader'])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Ipv4MulticastRoute', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv4MulticastRoute>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Ipv4Route', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv4Route>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Ipv6MulticastRoute', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv6MulticastRoute>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Ipv6Route', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv6Route>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::PbbAddressBlock', 'ns3::empty', 'ns3::DefaultDeleter<ns3::PbbAddressBlock>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::PbbMessage', 'ns3::empty', 'ns3::DefaultDeleter<ns3::PbbMessage>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::PbbPacket', 'ns3::Header', 'ns3::DefaultDeleter<ns3::PbbPacket>'], parent=root_module['ns3::Header'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::PbbTlv', 'ns3::empty', 'ns3::DefaultDeleter<ns3::PbbTlv>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## socket.h: ns3::Socket [class]
    module.add_class('Socket', parent=root_module['ns3::Object'])
    ## socket.h: ns3::Socket::SocketErrno [enumeration]
    module.add_enum('SocketErrno', ['ERROR_NOTERROR', 'ERROR_ISCONN', 'ERROR_NOTCONN', 'ERROR_MSGSIZE', 'ERROR_AGAIN', 'ERROR_SHUTDOWN', 'ERROR_OPNOTSUPP', 'ERROR_AFNOSUPPORT', 'ERROR_INVAL', 'ERROR_BADF', 'ERROR_NOROUTETOHOST', 'ERROR_NODEV', 'ERROR_ADDRNOTAVAIL', 'SOCKET_ERRNO_LAST'], outer_class=root_module['ns3::Socket'])
    ## socket.h: ns3::SocketAddressTag [class]
    module.add_class('SocketAddressTag', parent=root_module['ns3::Tag'])
    ## socket-factory.h: ns3::SocketFactory [class]
    module.add_class('SocketFactory', parent=root_module['ns3::Object'])
    ## socket.h: ns3::SocketIpTtlTag [class]
    module.add_class('SocketIpTtlTag', parent=root_module['ns3::Tag'])
    ## socket.h: ns3::SocketSetDontFragmentTag [class]
    module.add_class('SocketSetDontFragmentTag', parent=root_module['ns3::Tag'])
    ## spectrum-phy.h: ns3::SpectrumPhy [class]
    module.add_class('SpectrumPhy', parent=root_module['ns3::Object'])
    ## tcp-socket.h: ns3::TcpSocket [class]
    module.add_class('TcpSocket', parent=root_module['ns3::Socket'])
    ## tcp-socket-factory.h: ns3::TcpSocketFactory [class]
    module.add_class('TcpSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## udp-socket.h: ns3::UdpSocket [class]
    module.add_class('UdpSocket', parent=root_module['ns3::Socket'])
    ## udp-socket-factory.h: ns3::UdpSocketFactory [class]
    module.add_class('UdpSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## application.h: ns3::Application [class]
    module.add_class('Application', parent=root_module['ns3::Object'])
    ## channel.h: ns3::Channel [class]
    module.add_class('Channel', parent=root_module['ns3::Object'])
    ## drop-tail-queue.h: ns3::DropTailQueue [class]
    module.add_class('DropTailQueue', parent=root_module['ns3::Queue'])
    ## drop-tail-queue.h: ns3::DropTailQueue::Mode [enumeration]
    module.add_enum('Mode', ['ILLEGAL', 'PACKETS', 'BYTES'], outer_class=root_module['ns3::DropTailQueue'])
    ## ethernet-header.h: ns3::EthernetHeader [class]
    module.add_class('EthernetHeader', parent=root_module['ns3::Header'])
    ## ethernet-trailer.h: ns3::EthernetTrailer [class]
    module.add_class('EthernetTrailer', parent=root_module['ns3::Trailer'])
    ## ipv4.h: ns3::Ipv4 [class]
    module.add_class('Ipv4', parent=root_module['ns3::Object'])
    ## ipv4-address.h: ns3::Ipv4AddressChecker [class]
    module.add_class('Ipv4AddressChecker', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h: ns3::Ipv4AddressValue [class]
    module.add_class('Ipv4AddressValue', parent=root_module['ns3::AttributeValue'])
    ## ipv4-address.h: ns3::Ipv4MaskChecker [class]
    module.add_class('Ipv4MaskChecker', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h: ns3::Ipv4MaskValue [class]
    module.add_class('Ipv4MaskValue', parent=root_module['ns3::AttributeValue'])
    ## ipv4-route.h: ns3::Ipv4MulticastRoute [class]
    module.add_class('Ipv4MulticastRoute', parent=root_module['ns3::SimpleRefCount< ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >'])
    ## ipv4-raw-socket-factory.h: ns3::Ipv4RawSocketFactory [class]
    module.add_class('Ipv4RawSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## ipv4-route.h: ns3::Ipv4Route [class]
    module.add_class('Ipv4Route', parent=root_module['ns3::SimpleRefCount< ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >'])
    ## ipv4-routing-protocol.h: ns3::Ipv4RoutingProtocol [class]
    module.add_class('Ipv4RoutingProtocol', parent=root_module['ns3::Object'])
    ## ipv6.h: ns3::Ipv6 [class]
    module.add_class('Ipv6', parent=root_module['ns3::Object'])
    ## ipv6-address.h: ns3::Ipv6AddressChecker [class]
    module.add_class('Ipv6AddressChecker', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h: ns3::Ipv6AddressValue [class]
    module.add_class('Ipv6AddressValue', parent=root_module['ns3::AttributeValue'])
    ## ipv6-route.h: ns3::Ipv6MulticastRoute [class]
    module.add_class('Ipv6MulticastRoute', parent=root_module['ns3::SimpleRefCount< ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> >'])
    ## ipv6-address.h: ns3::Ipv6PrefixChecker [class]
    module.add_class('Ipv6PrefixChecker', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h: ns3::Ipv6PrefixValue [class]
    module.add_class('Ipv6PrefixValue', parent=root_module['ns3::AttributeValue'])
    ## ipv6-raw-socket-factory.h: ns3::Ipv6RawSocketFactory [class]
    module.add_class('Ipv6RawSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## ipv6-route.h: ns3::Ipv6Route [class]
    module.add_class('Ipv6Route', parent=root_module['ns3::SimpleRefCount< ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> >'])
    ## ipv6-routing-protocol.h: ns3::Ipv6RoutingProtocol [class]
    module.add_class('Ipv6RoutingProtocol', parent=root_module['ns3::Object'])
    ## mac48-address.h: ns3::Mac48AddressChecker [class]
    module.add_class('Mac48AddressChecker', parent=root_module['ns3::AttributeChecker'])
    ## mac48-address.h: ns3::Mac48AddressValue [class]
    module.add_class('Mac48AddressValue', parent=root_module['ns3::AttributeValue'])
    ## net-device.h: ns3::NetDevice [class]
    module.add_class('NetDevice', parent=root_module['ns3::Object'])
    ## net-device.h: ns3::NetDevice::PacketType [enumeration]
    module.add_enum('PacketType', ['PACKET_HOST', 'NS3_PACKET_HOST', 'PACKET_BROADCAST', 'NS3_PACKET_BROADCAST', 'PACKET_MULTICAST', 'NS3_PACKET_MULTICAST', 'PACKET_OTHERHOST', 'NS3_PACKET_OTHERHOST'], outer_class=root_module['ns3::NetDevice'])
    ## node.h: ns3::Node [class]
    module.add_class('Node', parent=root_module['ns3::Object'])
    ## packet-socket-factory.h: ns3::PacketSocketFactory [class]
    module.add_class('PacketSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## packetbb.h: ns3::PbbAddressBlock [class]
    module.add_class('PbbAddressBlock', parent=root_module['ns3::SimpleRefCount< ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> >'])
    ## packetbb.h: ns3::PbbAddressBlockIpv4 [class]
    module.add_class('PbbAddressBlockIpv4', parent=root_module['ns3::PbbAddressBlock'])
    ## packetbb.h: ns3::PbbAddressBlockIpv6 [class]
    module.add_class('PbbAddressBlockIpv6', parent=root_module['ns3::PbbAddressBlock'])
    ## packetbb.h: ns3::PbbMessage [class]
    module.add_class('PbbMessage', parent=root_module['ns3::SimpleRefCount< ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> >'])
    ## packetbb.h: ns3::PbbMessageIpv4 [class]
    module.add_class('PbbMessageIpv4', parent=root_module['ns3::PbbMessage'])
    ## packetbb.h: ns3::PbbMessageIpv6 [class]
    module.add_class('PbbMessageIpv6', parent=root_module['ns3::PbbMessage'])
    ## packetbb.h: ns3::PbbPacket [class]
    module.add_class('PbbPacket', parent=root_module['ns3::SimpleRefCount< ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> >'])
    ## packetbb.h: ns3::PbbTlv [class]
    module.add_class('PbbTlv', parent=root_module['ns3::SimpleRefCount< ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> >'])
    ## simple-channel.h: ns3::SimpleChannel [class]
    module.add_class('SimpleChannel', parent=root_module['ns3::Channel'])
    ## simple-net-device.h: ns3::SimpleNetDevice [class]
    module.add_class('SimpleNetDevice', parent=root_module['ns3::NetDevice'])
    ## spectrum-channel.h: ns3::SpectrumChannel [class]
    module.add_class('SpectrumChannel', parent=root_module['ns3::Channel'])
    ## address.h: ns3::AddressChecker [class]
    module.add_class('AddressChecker', parent=root_module['ns3::AttributeChecker'])
    ## address.h: ns3::AddressValue [class]
    module.add_class('AddressValue', parent=root_module['ns3::AttributeValue'])
    ## packetbb.h: ns3::PbbAddressTlv [class]
    module.add_class('PbbAddressTlv', parent=root_module['ns3::PbbTlv'])
    module.add_container('ns3::olsr::MprSet', 'ns3::Ipv4Address', container_type='set')
    module.add_container('std::vector< ns3::Ipv4Address >', 'ns3::Ipv4Address', container_type='vector')
    module.add_container('std::vector< ns3::Ipv6Address >', 'ns3::Ipv6Address', container_type='vector')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::PhyMacTxEndCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::PhyMacTxEndCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::PhyMacTxEndCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::PhyMacRxEndErrorCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::PhyMacRxEndErrorCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::PhyMacRxEndErrorCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::PhyMacRxEndOkCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::PhyMacRxEndOkCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::PhyMacRxEndOkCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::PhyMacRxStartCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::PhyMacRxStartCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::PhyMacRxStartCallback&')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::PhyMacTxStartCallback')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::PhyMacTxStartCallback*')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::PhyMacTxStartCallback&')
    
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
    
    module.add_container('std::vector< ns3::Mac48Address >', 'ns3::Mac48Address', container_type='vector')

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Address_methods(root_module, root_module['ns3::Address'])
    register_Ns3ChannelList_methods(root_module, root_module['ns3::ChannelList'])
    register_Ns3Inet6SocketAddress_methods(root_module, root_module['ns3::Inet6SocketAddress'])
    register_Ns3InetSocketAddress_methods(root_module, root_module['ns3::InetSocketAddress'])
    register_Ns3Ipv4Address_methods(root_module, root_module['ns3::Ipv4Address'])
    register_Ns3Ipv4AddressGenerator_methods(root_module, root_module['ns3::Ipv4AddressGenerator'])
    register_Ns3Ipv4InterfaceAddress_methods(root_module, root_module['ns3::Ipv4InterfaceAddress'])
    register_Ns3Ipv4Mask_methods(root_module, root_module['ns3::Ipv4Mask'])
    register_Ns3Ipv6Address_methods(root_module, root_module['ns3::Ipv6Address'])
    register_Ns3Ipv6InterfaceAddress_methods(root_module, root_module['ns3::Ipv6InterfaceAddress'])
    register_Ns3Ipv6Prefix_methods(root_module, root_module['ns3::Ipv6Prefix'])
    register_Ns3Mac48Address_methods(root_module, root_module['ns3::Mac48Address'])
    register_Ns3Mac64Address_methods(root_module, root_module['ns3::Mac64Address'])
    register_Ns3NodeList_methods(root_module, root_module['ns3::NodeList'])
    register_Ns3PacketSocketAddress_methods(root_module, root_module['ns3::PacketSocketAddress'])
    register_Ns3PbbAddressTlvBlock_methods(root_module, root_module['ns3::PbbAddressTlvBlock'])
    register_Ns3PbbTlvBlock_methods(root_module, root_module['ns3::PbbTlvBlock'])
    register_Ns3Ipv4Header_methods(root_module, root_module['ns3::Ipv4Header'])
    register_Ns3Ipv4PacketInfoTag_methods(root_module, root_module['ns3::Ipv4PacketInfoTag'])
    register_Ns3Ipv6Header_methods(root_module, root_module['ns3::Ipv6Header'])
    register_Ns3Ipv6PacketInfoTag_methods(root_module, root_module['ns3::Ipv6PacketInfoTag'])
    register_Ns3LlcSnapHeader_methods(root_module, root_module['ns3::LlcSnapHeader'])
    register_Ns3Queue_methods(root_module, root_module['ns3::Queue'])
    register_Ns3RadiotapHeader_methods(root_module, root_module['ns3::RadiotapHeader'])
    register_Ns3Socket_methods(root_module, root_module['ns3::Socket'])
    register_Ns3SocketAddressTag_methods(root_module, root_module['ns3::SocketAddressTag'])
    register_Ns3SocketFactory_methods(root_module, root_module['ns3::SocketFactory'])
    register_Ns3SocketIpTtlTag_methods(root_module, root_module['ns3::SocketIpTtlTag'])
    register_Ns3SocketSetDontFragmentTag_methods(root_module, root_module['ns3::SocketSetDontFragmentTag'])
    register_Ns3SpectrumPhy_methods(root_module, root_module['ns3::SpectrumPhy'])
    register_Ns3TcpSocket_methods(root_module, root_module['ns3::TcpSocket'])
    register_Ns3TcpSocketFactory_methods(root_module, root_module['ns3::TcpSocketFactory'])
    register_Ns3UdpSocket_methods(root_module, root_module['ns3::UdpSocket'])
    register_Ns3UdpSocketFactory_methods(root_module, root_module['ns3::UdpSocketFactory'])
    register_Ns3Application_methods(root_module, root_module['ns3::Application'])
    register_Ns3Channel_methods(root_module, root_module['ns3::Channel'])
    register_Ns3DropTailQueue_methods(root_module, root_module['ns3::DropTailQueue'])
    register_Ns3EthernetHeader_methods(root_module, root_module['ns3::EthernetHeader'])
    register_Ns3EthernetTrailer_methods(root_module, root_module['ns3::EthernetTrailer'])
    register_Ns3Ipv4_methods(root_module, root_module['ns3::Ipv4'])
    register_Ns3Ipv4AddressChecker_methods(root_module, root_module['ns3::Ipv4AddressChecker'])
    register_Ns3Ipv4AddressValue_methods(root_module, root_module['ns3::Ipv4AddressValue'])
    register_Ns3Ipv4MaskChecker_methods(root_module, root_module['ns3::Ipv4MaskChecker'])
    register_Ns3Ipv4MaskValue_methods(root_module, root_module['ns3::Ipv4MaskValue'])
    register_Ns3Ipv4MulticastRoute_methods(root_module, root_module['ns3::Ipv4MulticastRoute'])
    register_Ns3Ipv4RawSocketFactory_methods(root_module, root_module['ns3::Ipv4RawSocketFactory'])
    register_Ns3Ipv4Route_methods(root_module, root_module['ns3::Ipv4Route'])
    register_Ns3Ipv4RoutingProtocol_methods(root_module, root_module['ns3::Ipv4RoutingProtocol'])
    register_Ns3Ipv6_methods(root_module, root_module['ns3::Ipv6'])
    register_Ns3Ipv6AddressChecker_methods(root_module, root_module['ns3::Ipv6AddressChecker'])
    register_Ns3Ipv6AddressValue_methods(root_module, root_module['ns3::Ipv6AddressValue'])
    register_Ns3Ipv6MulticastRoute_methods(root_module, root_module['ns3::Ipv6MulticastRoute'])
    register_Ns3Ipv6PrefixChecker_methods(root_module, root_module['ns3::Ipv6PrefixChecker'])
    register_Ns3Ipv6PrefixValue_methods(root_module, root_module['ns3::Ipv6PrefixValue'])
    register_Ns3Ipv6RawSocketFactory_methods(root_module, root_module['ns3::Ipv6RawSocketFactory'])
    register_Ns3Ipv6Route_methods(root_module, root_module['ns3::Ipv6Route'])
    register_Ns3Ipv6RoutingProtocol_methods(root_module, root_module['ns3::Ipv6RoutingProtocol'])
    register_Ns3Mac48AddressChecker_methods(root_module, root_module['ns3::Mac48AddressChecker'])
    register_Ns3Mac48AddressValue_methods(root_module, root_module['ns3::Mac48AddressValue'])
    register_Ns3NetDevice_methods(root_module, root_module['ns3::NetDevice'])
    register_Ns3Node_methods(root_module, root_module['ns3::Node'])
    register_Ns3PacketSocketFactory_methods(root_module, root_module['ns3::PacketSocketFactory'])
    register_Ns3PbbAddressBlock_methods(root_module, root_module['ns3::PbbAddressBlock'])
    register_Ns3PbbAddressBlockIpv4_methods(root_module, root_module['ns3::PbbAddressBlockIpv4'])
    register_Ns3PbbAddressBlockIpv6_methods(root_module, root_module['ns3::PbbAddressBlockIpv6'])
    register_Ns3PbbMessage_methods(root_module, root_module['ns3::PbbMessage'])
    register_Ns3PbbMessageIpv4_methods(root_module, root_module['ns3::PbbMessageIpv4'])
    register_Ns3PbbMessageIpv6_methods(root_module, root_module['ns3::PbbMessageIpv6'])
    register_Ns3PbbPacket_methods(root_module, root_module['ns3::PbbPacket'])
    register_Ns3PbbTlv_methods(root_module, root_module['ns3::PbbTlv'])
    register_Ns3SimpleChannel_methods(root_module, root_module['ns3::SimpleChannel'])
    register_Ns3SimpleNetDevice_methods(root_module, root_module['ns3::SimpleNetDevice'])
    register_Ns3SpectrumChannel_methods(root_module, root_module['ns3::SpectrumChannel'])
    register_Ns3AddressChecker_methods(root_module, root_module['ns3::AddressChecker'])
    register_Ns3AddressValue_methods(root_module, root_module['ns3::AddressValue'])
    register_Ns3PbbAddressTlv_methods(root_module, root_module['ns3::PbbAddressTlv'])
    return

def register_Ns3Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## address.h: ns3::Address::Address() [constructor]
    cls.add_constructor([])
    ## address.h: ns3::Address::Address(uint8_t type, uint8_t const * buffer, uint8_t len) [constructor]
    cls.add_constructor([param('uint8_t', 'type'), param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h: ns3::Address::Address(ns3::Address const & address) [copy constructor]
    cls.add_constructor([param('ns3::Address const &', 'address')])
    ## address.h: bool ns3::Address::CheckCompatible(uint8_t type, uint8_t len) const [member function]
    cls.add_method('CheckCompatible', 
                   'bool', 
                   [param('uint8_t', 'type'), param('uint8_t', 'len')], 
                   is_const=True)
    ## address.h: uint32_t ns3::Address::CopyAllFrom(uint8_t const * buffer, uint8_t len) [member function]
    cls.add_method('CopyAllFrom', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h: uint32_t ns3::Address::CopyAllTo(uint8_t * buffer, uint8_t len) const [member function]
    cls.add_method('CopyAllTo', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint8_t', 'len')], 
                   is_const=True)
    ## address.h: uint32_t ns3::Address::CopyFrom(uint8_t const * buffer, uint8_t len) [member function]
    cls.add_method('CopyFrom', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h: uint32_t ns3::Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## address.h: void ns3::Address::Deserialize(ns3::TagBuffer buffer) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buffer')])
    ## address.h: uint8_t ns3::Address::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## address.h: uint32_t ns3::Address::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## address.h: bool ns3::Address::IsInvalid() const [member function]
    cls.add_method('IsInvalid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## address.h: bool ns3::Address::IsMatchingType(uint8_t type) const [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('uint8_t', 'type')], 
                   is_const=True)
    ## address.h: static uint8_t ns3::Address::Register() [member function]
    cls.add_method('Register', 
                   'uint8_t', 
                   [], 
                   is_static=True)
    ## address.h: void ns3::Address::Serialize(ns3::TagBuffer buffer) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buffer')], 
                   is_const=True)
    return

def register_Ns3ChannelList_methods(root_module, cls):
    ## channel-list.h: ns3::ChannelList::ChannelList() [constructor]
    cls.add_constructor([])
    ## channel-list.h: ns3::ChannelList::ChannelList(ns3::ChannelList const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ChannelList const &', 'arg0')])
    ## channel-list.h: static uint32_t ns3::ChannelList::Add(ns3::Ptr<ns3::Channel> channel) [member function]
    cls.add_method('Add', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Channel >', 'channel')], 
                   is_static=True)
    ## channel-list.h: static __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Channel>*,std::vector<ns3::Ptr<ns3::Channel>, std::allocator<ns3::Ptr<ns3::Channel> > > > ns3::ChannelList::Begin() [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Channel > const, std::vector< ns3::Ptr< ns3::Channel > > >', 
                   [], 
                   is_static=True)
    ## channel-list.h: static __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Channel>*,std::vector<ns3::Ptr<ns3::Channel>, std::allocator<ns3::Ptr<ns3::Channel> > > > ns3::ChannelList::End() [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Channel > const, std::vector< ns3::Ptr< ns3::Channel > > >', 
                   [], 
                   is_static=True)
    ## channel-list.h: static ns3::Ptr<ns3::Channel> ns3::ChannelList::GetChannel(uint32_t n) [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [param('uint32_t', 'n')], 
                   is_static=True)
    ## channel-list.h: static uint32_t ns3::ChannelList::GetNChannels() [member function]
    cls.add_method('GetNChannels', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    return

def register_Ns3Inet6SocketAddress_methods(root_module, cls):
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(ns3::Inet6SocketAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Inet6SocketAddress const &', 'arg0')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(ns3::Ipv6Address ipv6, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'ipv6'), param('uint16_t', 'port')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(ns3::Ipv6Address ipv6) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'ipv6')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(char const * ipv6, uint16_t port) [constructor]
    cls.add_constructor([param('char const *', 'ipv6'), param('uint16_t', 'port')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(char const * ipv6) [constructor]
    cls.add_constructor([param('char const *', 'ipv6')])
    ## inet6-socket-address.h: static ns3::Inet6SocketAddress ns3::Inet6SocketAddress::ConvertFrom(ns3::Address const & addr) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Inet6SocketAddress', 
                   [param('ns3::Address const &', 'addr')], 
                   is_static=True)
    ## inet6-socket-address.h: ns3::Ipv6Address ns3::Inet6SocketAddress::GetIpv6() const [member function]
    cls.add_method('GetIpv6', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## inet6-socket-address.h: uint16_t ns3::Inet6SocketAddress::GetPort() const [member function]
    cls.add_method('GetPort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## inet6-socket-address.h: static bool ns3::Inet6SocketAddress::IsMatchingType(ns3::Address const & addr) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'addr')], 
                   is_static=True)
    ## inet6-socket-address.h: void ns3::Inet6SocketAddress::SetIpv6(ns3::Ipv6Address ipv6) [member function]
    cls.add_method('SetIpv6', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ipv6')])
    ## inet6-socket-address.h: void ns3::Inet6SocketAddress::SetPort(uint16_t port) [member function]
    cls.add_method('SetPort', 
                   'void', 
                   [param('uint16_t', 'port')])
    return

def register_Ns3InetSocketAddress_methods(root_module, cls):
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(ns3::InetSocketAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::InetSocketAddress const &', 'arg0')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(ns3::Ipv4Address ipv4, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ipv4'), param('uint16_t', 'port')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(ns3::Ipv4Address ipv4) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ipv4')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(char const * ipv4, uint16_t port) [constructor]
    cls.add_constructor([param('char const *', 'ipv4'), param('uint16_t', 'port')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(char const * ipv4) [constructor]
    cls.add_constructor([param('char const *', 'ipv4')])
    ## inet-socket-address.h: static ns3::InetSocketAddress ns3::InetSocketAddress::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::InetSocketAddress', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## inet-socket-address.h: ns3::Ipv4Address ns3::InetSocketAddress::GetIpv4() const [member function]
    cls.add_method('GetIpv4', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## inet-socket-address.h: uint16_t ns3::InetSocketAddress::GetPort() const [member function]
    cls.add_method('GetPort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## inet-socket-address.h: static bool ns3::InetSocketAddress::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## inet-socket-address.h: void ns3::InetSocketAddress::SetIpv4(ns3::Ipv4Address address) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ipv4Address', 'address')])
    ## inet-socket-address.h: void ns3::InetSocketAddress::SetPort(uint16_t port) [member function]
    cls.add_method('SetPort', 
                   'void', 
                   [param('uint16_t', 'port')])
    return

def register_Ns3Ipv4Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ipv4-address.h: ns3::Ipv4Address::Ipv4Address(ns3::Ipv4Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Address const &', 'arg0')])
    ## ipv4-address.h: ns3::Ipv4Address::Ipv4Address() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4Address::Ipv4Address(uint32_t address) [constructor]
    cls.add_constructor([param('uint32_t', 'address')])
    ## ipv4-address.h: ns3::Ipv4Address::Ipv4Address(char const * address) [constructor]
    cls.add_constructor([param('char const *', 'address')])
    ## ipv4-address.h: ns3::Ipv4Address ns3::Ipv4Address::CombineMask(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('CombineMask', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::Deserialize(uint8_t const * buf) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Ipv4Address', 
                   [param('uint8_t const *', 'buf')], 
                   is_static=True)
    ## ipv4-address.h: uint32_t ns3::Ipv4Address::Get() const [member function]
    cls.add_method('Get', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::GetAny() [member function]
    cls.add_method('GetAny', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: ns3::Ipv4Address ns3::Ipv4Address::GetSubnetDirectedBroadcast(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('GetSubnetDirectedBroadcast', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsEqual(ns3::Ipv4Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv4Address const &', 'other')], 
                   is_const=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsLocalMulticast() const [member function]
    cls.add_method('IsLocalMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: static bool ns3::Ipv4Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsSubnetDirectedBroadcast(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('IsSubnetDirectedBroadcast', 
                   'bool', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Address::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Address::Serialize(uint8_t * buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Address::Set(uint32_t address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint32_t', 'address')])
    ## ipv4-address.h: void ns3::Ipv4Address::Set(char const * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('char const *', 'address')])
    return

def register_Ns3Ipv4AddressGenerator_methods(root_module, cls):
    ## ipv4-address-generator.h: ns3::Ipv4AddressGenerator::Ipv4AddressGenerator() [constructor]
    cls.add_constructor([])
    ## ipv4-address-generator.h: ns3::Ipv4AddressGenerator::Ipv4AddressGenerator(ns3::Ipv4AddressGenerator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressGenerator const &', 'arg0')])
    ## ipv4-address-generator.h: static bool ns3::Ipv4AddressGenerator::AddAllocated(ns3::Ipv4Address const addr) [member function]
    cls.add_method('AddAllocated', 
                   'bool', 
                   [param('ns3::Ipv4Address const', 'addr')], 
                   is_static=True)
    ## ipv4-address-generator.h: static ns3::Ipv4Address ns3::Ipv4AddressGenerator::GetAddress(ns3::Ipv4Mask const mask) [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const', 'mask')], 
                   is_static=True)
    ## ipv4-address-generator.h: static ns3::Ipv4Address ns3::Ipv4AddressGenerator::GetNetwork(ns3::Ipv4Mask const mask) [member function]
    cls.add_method('GetNetwork', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const', 'mask')], 
                   is_static=True)
    ## ipv4-address-generator.h: static void ns3::Ipv4AddressGenerator::Init(ns3::Ipv4Address const net, ns3::Ipv4Mask const mask, ns3::Ipv4Address const addr="0.0.0.1") [member function]
    cls.add_method('Init', 
                   'void', 
                   [param('ns3::Ipv4Address const', 'net'), param('ns3::Ipv4Mask const', 'mask'), param('ns3::Ipv4Address const', 'addr', default_value='"0.0.0.1"')], 
                   is_static=True)
    ## ipv4-address-generator.h: static void ns3::Ipv4AddressGenerator::InitAddress(ns3::Ipv4Address const addr, ns3::Ipv4Mask const mask) [member function]
    cls.add_method('InitAddress', 
                   'void', 
                   [param('ns3::Ipv4Address const', 'addr'), param('ns3::Ipv4Mask const', 'mask')], 
                   is_static=True)
    ## ipv4-address-generator.h: static ns3::Ipv4Address ns3::Ipv4AddressGenerator::NextAddress(ns3::Ipv4Mask const mask) [member function]
    cls.add_method('NextAddress', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const', 'mask')], 
                   is_static=True)
    ## ipv4-address-generator.h: static ns3::Ipv4Address ns3::Ipv4AddressGenerator::NextNetwork(ns3::Ipv4Mask const mask) [member function]
    cls.add_method('NextNetwork', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const', 'mask')], 
                   is_static=True)
    ## ipv4-address-generator.h: static void ns3::Ipv4AddressGenerator::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [], 
                   is_static=True)
    ## ipv4-address-generator.h: static void ns3::Ipv4AddressGenerator::TestMode() [member function]
    cls.add_method('TestMode', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3Ipv4InterfaceAddress_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv4-interface-address.h: ns3::Ipv4InterfaceAddress::Ipv4InterfaceAddress() [constructor]
    cls.add_constructor([])
    ## ipv4-interface-address.h: ns3::Ipv4InterfaceAddress::Ipv4InterfaceAddress(ns3::Ipv4Address local, ns3::Ipv4Mask mask) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'local'), param('ns3::Ipv4Mask', 'mask')])
    ## ipv4-interface-address.h: ns3::Ipv4InterfaceAddress::Ipv4InterfaceAddress(ns3::Ipv4InterfaceAddress const & o) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4InterfaceAddress const &', 'o')])
    ## ipv4-interface-address.h: ns3::Ipv4Address ns3::Ipv4InterfaceAddress::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h: ns3::Ipv4Address ns3::Ipv4InterfaceAddress::GetLocal() const [member function]
    cls.add_method('GetLocal', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h: ns3::Ipv4Mask ns3::Ipv4InterfaceAddress::GetMask() const [member function]
    cls.add_method('GetMask', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h: ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e ns3::Ipv4InterfaceAddress::GetScope() const [member function]
    cls.add_method('GetScope', 
                   'ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h: bool ns3::Ipv4InterfaceAddress::IsSecondary() const [member function]
    cls.add_method('IsSecondary', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h: void ns3::Ipv4InterfaceAddress::SetBroadcast(ns3::Ipv4Address broadcast) [member function]
    cls.add_method('SetBroadcast', 
                   'void', 
                   [param('ns3::Ipv4Address', 'broadcast')])
    ## ipv4-interface-address.h: void ns3::Ipv4InterfaceAddress::SetLocal(ns3::Ipv4Address local) [member function]
    cls.add_method('SetLocal', 
                   'void', 
                   [param('ns3::Ipv4Address', 'local')])
    ## ipv4-interface-address.h: void ns3::Ipv4InterfaceAddress::SetMask(ns3::Ipv4Mask mask) [member function]
    cls.add_method('SetMask', 
                   'void', 
                   [param('ns3::Ipv4Mask', 'mask')])
    ## ipv4-interface-address.h: void ns3::Ipv4InterfaceAddress::SetPrimary() [member function]
    cls.add_method('SetPrimary', 
                   'void', 
                   [])
    ## ipv4-interface-address.h: void ns3::Ipv4InterfaceAddress::SetScope(ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e scope) [member function]
    cls.add_method('SetScope', 
                   'void', 
                   [param('ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e', 'scope')])
    ## ipv4-interface-address.h: void ns3::Ipv4InterfaceAddress::SetSecondary() [member function]
    cls.add_method('SetSecondary', 
                   'void', 
                   [])
    return

def register_Ns3Ipv4Mask_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv4-address.h: ns3::Ipv4Mask::Ipv4Mask(ns3::Ipv4Mask const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Mask const &', 'arg0')])
    ## ipv4-address.h: ns3::Ipv4Mask::Ipv4Mask() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4Mask::Ipv4Mask(uint32_t mask) [constructor]
    cls.add_constructor([param('uint32_t', 'mask')])
    ## ipv4-address.h: ns3::Ipv4Mask::Ipv4Mask(char const * mask) [constructor]
    cls.add_constructor([param('char const *', 'mask')])
    ## ipv4-address.h: uint32_t ns3::Ipv4Mask::Get() const [member function]
    cls.add_method('Get', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: uint32_t ns3::Ipv4Mask::GetInverse() const [member function]
    cls.add_method('GetInverse', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Mask ns3::Ipv4Mask::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: static ns3::Ipv4Mask ns3::Ipv4Mask::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: uint16_t ns3::Ipv4Mask::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Mask ns3::Ipv4Mask::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: bool ns3::Ipv4Mask::IsEqual(ns3::Ipv4Mask other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv4Mask', 'other')], 
                   is_const=True)
    ## ipv4-address.h: bool ns3::Ipv4Mask::IsMatch(ns3::Ipv4Address a, ns3::Ipv4Address b) const [member function]
    cls.add_method('IsMatch', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'a'), param('ns3::Ipv4Address', 'b')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Mask::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Mask::Set(uint32_t mask) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint32_t', 'mask')])
    return

def register_Ns3Ipv6Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address(char const * address) [constructor]
    cls.add_constructor([param('char const *', 'address')])
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address(uint8_t * address) [constructor]
    cls.add_constructor([param('uint8_t *', 'address')])
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address(ns3::Ipv6Address const & addr) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Address const &', 'addr')])
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address(ns3::Ipv6Address const * addr) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address const *', 'addr')])
    ## ipv6-address.h: ns3::Ipv6Address ns3::Ipv6Address::CombinePrefix(ns3::Ipv6Prefix const & prefix) [member function]
    cls.add_method('CombinePrefix', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv6Prefix const &', 'prefix')])
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::Deserialize(uint8_t const * buf) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Ipv6Address', 
                   [param('uint8_t const *', 'buf')], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetAllHostsMulticast() [member function]
    cls.add_method('GetAllHostsMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetAllNodesMulticast() [member function]
    cls.add_method('GetAllNodesMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetAllRoutersMulticast() [member function]
    cls.add_method('GetAllRoutersMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetAny() [member function]
    cls.add_method('GetAny', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: void ns3::Ipv6Address::GetBytes(uint8_t * buf) const [member function]
    cls.add_method('GetBytes', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsAllHostsMulticast() const [member function]
    cls.add_method('IsAllHostsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsAllNodesMulticast() const [member function]
    cls.add_method('IsAllNodesMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsAllRoutersMulticast() const [member function]
    cls.add_method('IsAllRoutersMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsAny() const [member function]
    cls.add_method('IsAny', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsEqual(ns3::Ipv6Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Address const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsLinkLocal() const [member function]
    cls.add_method('IsLinkLocal', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsLocalhost() const [member function]
    cls.add_method('IsLocalhost', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: static bool ns3::Ipv6Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsSolicitedMulticast() const [member function]
    cls.add_method('IsSolicitedMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac48Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac48Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'mac')], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::MakeSolicitedAddress(ns3::Ipv6Address addr) [member function]
    cls.add_method('MakeSolicitedAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_static=True)
    ## ipv6-address.h: void ns3::Ipv6Address::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv6-address.h: void ns3::Ipv6Address::Serialize(uint8_t * buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h: void ns3::Ipv6Address::Set(char const * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('char const *', 'address')])
    ## ipv6-address.h: void ns3::Ipv6Address::Set(uint8_t * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint8_t *', 'address')])
    return

def register_Ns3Ipv6InterfaceAddress_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::Ipv6InterfaceAddress() [constructor]
    cls.add_constructor([])
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::Ipv6InterfaceAddress(ns3::Ipv6Address address) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'address')])
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::Ipv6InterfaceAddress(ns3::Ipv6Address address, ns3::Ipv6Prefix prefix) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'address'), param('ns3::Ipv6Prefix', 'prefix')])
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::Ipv6InterfaceAddress(ns3::Ipv6InterfaceAddress const & o) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6InterfaceAddress const &', 'o')])
    ## ipv6-interface-address.h: ns3::Ipv6Address ns3::Ipv6InterfaceAddress::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h: uint32_t ns3::Ipv6InterfaceAddress::GetNsDadUid() const [member function]
    cls.add_method('GetNsDadUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h: ns3::Ipv6Prefix ns3::Ipv6InterfaceAddress::GetPrefix() const [member function]
    cls.add_method('GetPrefix', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::Scope_e ns3::Ipv6InterfaceAddress::GetScope() const [member function]
    cls.add_method('GetScope', 
                   'ns3::Ipv6InterfaceAddress::Scope_e', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::State_e ns3::Ipv6InterfaceAddress::GetState() const [member function]
    cls.add_method('GetState', 
                   'ns3::Ipv6InterfaceAddress::State_e', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h: void ns3::Ipv6InterfaceAddress::SetAddress(ns3::Ipv6Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Ipv6Address', 'address')])
    ## ipv6-interface-address.h: void ns3::Ipv6InterfaceAddress::SetNsDadUid(uint32_t uid) [member function]
    cls.add_method('SetNsDadUid', 
                   'void', 
                   [param('uint32_t', 'uid')])
    ## ipv6-interface-address.h: void ns3::Ipv6InterfaceAddress::SetScope(ns3::Ipv6InterfaceAddress::Scope_e scope) [member function]
    cls.add_method('SetScope', 
                   'void', 
                   [param('ns3::Ipv6InterfaceAddress::Scope_e', 'scope')])
    ## ipv6-interface-address.h: void ns3::Ipv6InterfaceAddress::SetState(ns3::Ipv6InterfaceAddress::State_e state) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::Ipv6InterfaceAddress::State_e', 'state')])
    return

def register_Ns3Ipv6Prefix_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(uint8_t * prefix) [constructor]
    cls.add_constructor([param('uint8_t *', 'prefix')])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(char const * prefix) [constructor]
    cls.add_constructor([param('char const *', 'prefix')])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(uint8_t prefix) [constructor]
    cls.add_constructor([param('uint8_t', 'prefix')])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(ns3::Ipv6Prefix const & prefix) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const &', 'prefix')])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(ns3::Ipv6Prefix const * prefix) [constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const *', 'prefix')])
    ## ipv6-address.h: void ns3::Ipv6Prefix::GetBytes(uint8_t * buf) const [member function]
    cls.add_method('GetBytes', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h: static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: uint8_t ns3::Ipv6Prefix::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: bool ns3::Ipv6Prefix::IsEqual(ns3::Ipv6Prefix const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Prefix const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Prefix::IsMatch(ns3::Ipv6Address a, ns3::Ipv6Address b) const [member function]
    cls.add_method('IsMatch', 
                   'bool', 
                   [param('ns3::Ipv6Address', 'a'), param('ns3::Ipv6Address', 'b')], 
                   is_const=True)
    ## ipv6-address.h: void ns3::Ipv6Prefix::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3Mac48Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## mac48-address.h: ns3::Mac48Address::Mac48Address(ns3::Mac48Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48Address const &', 'arg0')])
    ## mac48-address.h: ns3::Mac48Address::Mac48Address() [constructor]
    cls.add_constructor([])
    ## mac48-address.h: ns3::Mac48Address::Mac48Address(char const * str) [constructor]
    cls.add_constructor([param('char const *', 'str')])
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Mac48Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## mac48-address.h: void ns3::Mac48Address::CopyFrom(uint8_t const * buffer) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('uint8_t const *', 'buffer')])
    ## mac48-address.h: void ns3::Mac48Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'void', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetMulticast(ns3::Ipv4Address address) [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Mac48Address', 
                   [param('ns3::Ipv4Address', 'address')], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetMulticast(ns3::Ipv6Address address) [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Mac48Address', 
                   [param('ns3::Ipv6Address', 'address')], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetMulticast6Prefix() [member function]
    cls.add_method('GetMulticast6Prefix', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetMulticastPrefix() [member function]
    cls.add_method('GetMulticastPrefix', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h: bool ns3::Mac48Address::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac48-address.h: bool ns3::Mac48Address::IsGroup() const [member function]
    cls.add_method('IsGroup', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac48-address.h: static bool ns3::Mac48Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    return

def register_Ns3Mac64Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## mac64-address.h: ns3::Mac64Address::Mac64Address(ns3::Mac64Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac64Address const &', 'arg0')])
    ## mac64-address.h: ns3::Mac64Address::Mac64Address() [constructor]
    cls.add_constructor([])
    ## mac64-address.h: ns3::Mac64Address::Mac64Address(char const * str) [constructor]
    cls.add_constructor([param('char const *', 'str')])
    ## mac64-address.h: static ns3::Mac64Address ns3::Mac64Address::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::Mac64Address', 
                   [], 
                   is_static=True)
    ## mac64-address.h: static ns3::Mac64Address ns3::Mac64Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Mac64Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## mac64-address.h: void ns3::Mac64Address::CopyFrom(uint8_t const * buffer) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('uint8_t const *', 'buffer')])
    ## mac64-address.h: void ns3::Mac64Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'void', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## mac64-address.h: static bool ns3::Mac64Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    return

def register_Ns3NodeList_methods(root_module, cls):
    ## node-list.h: ns3::NodeList::NodeList() [constructor]
    cls.add_constructor([])
    ## node-list.h: ns3::NodeList::NodeList(ns3::NodeList const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NodeList const &', 'arg0')])
    ## node-list.h: static uint32_t ns3::NodeList::Add(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Add', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_static=True)
    ## node-list.h: static __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeList::Begin() [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_static=True)
    ## node-list.h: static __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeList::End() [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_static=True)
    ## node-list.h: static uint32_t ns3::NodeList::GetNNodes() [member function]
    cls.add_method('GetNNodes', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## node-list.h: static ns3::Ptr<ns3::Node> ns3::NodeList::GetNode(uint32_t n) [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'n')], 
                   is_static=True)
    return

def register_Ns3PacketSocketAddress_methods(root_module, cls):
    ## packet-socket-address.h: ns3::PacketSocketAddress::PacketSocketAddress(ns3::PacketSocketAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSocketAddress const &', 'arg0')])
    ## packet-socket-address.h: ns3::PacketSocketAddress::PacketSocketAddress() [constructor]
    cls.add_constructor([])
    ## packet-socket-address.h: static ns3::PacketSocketAddress ns3::PacketSocketAddress::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::PacketSocketAddress', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## packet-socket-address.h: ns3::Address ns3::PacketSocketAddress::GetPhysicalAddress() const [member function]
    cls.add_method('GetPhysicalAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## packet-socket-address.h: uint16_t ns3::PacketSocketAddress::GetProtocol() const [member function]
    cls.add_method('GetProtocol', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## packet-socket-address.h: uint32_t ns3::PacketSocketAddress::GetSingleDevice() const [member function]
    cls.add_method('GetSingleDevice', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-socket-address.h: static bool ns3::PacketSocketAddress::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## packet-socket-address.h: bool ns3::PacketSocketAddress::IsSingleDevice() const [member function]
    cls.add_method('IsSingleDevice', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet-socket-address.h: void ns3::PacketSocketAddress::SetAllDevices() [member function]
    cls.add_method('SetAllDevices', 
                   'void', 
                   [])
    ## packet-socket-address.h: void ns3::PacketSocketAddress::SetPhysicalAddress(ns3::Address const address) [member function]
    cls.add_method('SetPhysicalAddress', 
                   'void', 
                   [param('ns3::Address const', 'address')])
    ## packet-socket-address.h: void ns3::PacketSocketAddress::SetProtocol(uint16_t protocol) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint16_t', 'protocol')])
    ## packet-socket-address.h: void ns3::PacketSocketAddress::SetSingleDevice(uint32_t device) [member function]
    cls.add_method('SetSingleDevice', 
                   'void', 
                   [param('uint32_t', 'device')])
    return

def register_Ns3PbbAddressTlvBlock_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbAddressTlvBlock::PbbAddressTlvBlock(ns3::PbbAddressTlvBlock const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressTlvBlock const &', 'arg0')])
    ## packetbb.h: ns3::PbbAddressTlvBlock::PbbAddressTlvBlock() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> ns3::PbbAddressTlvBlock::Back() const [member function]
    cls.add_method('Back', 
                   'ns3::Ptr< ns3::PbbAddressTlv >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Begin() [member function]
    cls.add_method('Begin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Begin() const [member function]
    cls.add_method('Begin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: bool ns3::PbbAddressTlvBlock::Empty() const [member function]
    cls.add_method('Empty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::End() [member function]
    cls.add_method('End', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::End() const [member function]
    cls.add_method('End', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > position) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > last) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> ns3::PbbAddressTlvBlock::Front() const [member function]
    cls.add_method('Front', 
                   'ns3::Ptr< ns3::PbbAddressTlv >', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint32_t ns3::PbbAddressTlvBlock::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Insert(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > position, ns3::Ptr<ns3::PbbAddressTlv> const tlv) [member function]
    cls.add_method('Insert', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'position'), param('ns3::Ptr< ns3::PbbAddressTlv > const', 'tlv')])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::PopBack() [member function]
    cls.add_method('PopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::PopFront() [member function]
    cls.add_method('PopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressTlvBlock::PushBack(ns3::Ptr<ns3::PbbAddressTlv> tlv) [member function]
    cls.add_method('PushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::PushFront(ns3::Ptr<ns3::PbbAddressTlv> tlv) [member function]
    cls.add_method('PushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: int ns3::PbbAddressTlvBlock::Size() const [member function]
    cls.add_method('Size', 
                   'int', 
                   [], 
                   is_const=True)
    return

def register_Ns3PbbTlvBlock_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbTlvBlock::PbbTlvBlock(ns3::PbbTlvBlock const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbTlvBlock const &', 'arg0')])
    ## packetbb.h: ns3::PbbTlvBlock::PbbTlvBlock() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbTlvBlock::Back() const [member function]
    cls.add_method('Back', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Begin() [member function]
    cls.add_method('Begin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Begin() const [member function]
    cls.add_method('Begin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlvBlock::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbTlvBlock::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: bool ns3::PbbTlvBlock::Empty() const [member function]
    cls.add_method('Empty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::End() [member function]
    cls.add_method('End', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::End() const [member function]
    cls.add_method('End', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > position) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > last) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbTlvBlock::Front() const [member function]
    cls.add_method('Front', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint32_t ns3::PbbTlvBlock::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Insert(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > position, ns3::Ptr<ns3::PbbTlv> const tlv) [member function]
    cls.add_method('Insert', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'position'), param('ns3::Ptr< ns3::PbbTlv > const', 'tlv')])
    ## packetbb.h: void ns3::PbbTlvBlock::PopBack() [member function]
    cls.add_method('PopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbTlvBlock::PopFront() [member function]
    cls.add_method('PopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbTlvBlock::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlvBlock::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlvBlock::PushBack(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('PushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbTlvBlock::PushFront(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('PushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbTlvBlock::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: int ns3::PbbTlvBlock::Size() const [member function]
    cls.add_method('Size', 
                   'int', 
                   [], 
                   is_const=True)
    return

def register_Ns3Ipv4Header_methods(root_module, cls):
    ## ipv4-header.h: ns3::Ipv4Header::Ipv4Header(ns3::Ipv4Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Header const &', 'arg0')])
    ## ipv4-header.h: ns3::Ipv4Header::Ipv4Header() [constructor]
    cls.add_constructor([])
    ## ipv4-header.h: uint32_t ns3::Ipv4Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv4-header.h: void ns3::Ipv4Header::EnableChecksum() [member function]
    cls.add_method('EnableChecksum', 
                   'void', 
                   [])
    ## ipv4-header.h: ns3::Ipv4Address ns3::Ipv4Header::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: uint16_t ns3::Ipv4Header::GetFragmentOffset() const [member function]
    cls.add_method('GetFragmentOffset', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: uint16_t ns3::Ipv4Header::GetIdentification() const [member function]
    cls.add_method('GetIdentification', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: ns3::TypeId ns3::Ipv4Header::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-header.h: uint16_t ns3::Ipv4Header::GetPayloadSize() const [member function]
    cls.add_method('GetPayloadSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: uint8_t ns3::Ipv4Header::GetProtocol() const [member function]
    cls.add_method('GetProtocol', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: uint32_t ns3::Ipv4Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-header.h: ns3::Ipv4Address ns3::Ipv4Header::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: uint8_t ns3::Ipv4Header::GetTos() const [member function]
    cls.add_method('GetTos', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: uint8_t ns3::Ipv4Header::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: static ns3::TypeId ns3::Ipv4Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-header.h: bool ns3::Ipv4Header::IsChecksumOk() const [member function]
    cls.add_method('IsChecksumOk', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: bool ns3::Ipv4Header::IsDontFragment() const [member function]
    cls.add_method('IsDontFragment', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: bool ns3::Ipv4Header::IsLastFragment() const [member function]
    cls.add_method('IsLastFragment', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-header.h: void ns3::Ipv4Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv4-header.h: void ns3::Ipv4Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv4-header.h: void ns3::Ipv4Header::SetDestination(ns3::Ipv4Address destination) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Ipv4Address', 'destination')])
    ## ipv4-header.h: void ns3::Ipv4Header::SetDontFragment() [member function]
    cls.add_method('SetDontFragment', 
                   'void', 
                   [])
    ## ipv4-header.h: void ns3::Ipv4Header::SetFragmentOffset(uint16_t offset) [member function]
    cls.add_method('SetFragmentOffset', 
                   'void', 
                   [param('uint16_t', 'offset')])
    ## ipv4-header.h: void ns3::Ipv4Header::SetIdentification(uint16_t identification) [member function]
    cls.add_method('SetIdentification', 
                   'void', 
                   [param('uint16_t', 'identification')])
    ## ipv4-header.h: void ns3::Ipv4Header::SetLastFragment() [member function]
    cls.add_method('SetLastFragment', 
                   'void', 
                   [])
    ## ipv4-header.h: void ns3::Ipv4Header::SetMayFragment() [member function]
    cls.add_method('SetMayFragment', 
                   'void', 
                   [])
    ## ipv4-header.h: void ns3::Ipv4Header::SetMoreFragments() [member function]
    cls.add_method('SetMoreFragments', 
                   'void', 
                   [])
    ## ipv4-header.h: void ns3::Ipv4Header::SetPayloadSize(uint16_t size) [member function]
    cls.add_method('SetPayloadSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## ipv4-header.h: void ns3::Ipv4Header::SetProtocol(uint8_t num) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint8_t', 'num')])
    ## ipv4-header.h: void ns3::Ipv4Header::SetSource(ns3::Ipv4Address source) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Ipv4Address', 'source')])
    ## ipv4-header.h: void ns3::Ipv4Header::SetTos(uint8_t tos) [member function]
    cls.add_method('SetTos', 
                   'void', 
                   [param('uint8_t', 'tos')])
    ## ipv4-header.h: void ns3::Ipv4Header::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3Ipv4PacketInfoTag_methods(root_module, cls):
    ## ipv4-packet-info-tag.h: ns3::Ipv4PacketInfoTag::Ipv4PacketInfoTag(ns3::Ipv4PacketInfoTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4PacketInfoTag const &', 'arg0')])
    ## ipv4-packet-info-tag.h: ns3::Ipv4PacketInfoTag::Ipv4PacketInfoTag() [constructor]
    cls.add_constructor([])
    ## ipv4-packet-info-tag.h: void ns3::Ipv4PacketInfoTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## ipv4-packet-info-tag.h: ns3::Ipv4Address ns3::Ipv4PacketInfoTag::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-packet-info-tag.h: ns3::TypeId ns3::Ipv4PacketInfoTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-packet-info-tag.h: ns3::Ipv4Address ns3::Ipv4PacketInfoTag::GetLocalAddress() const [member function]
    cls.add_method('GetLocalAddress', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-packet-info-tag.h: uint32_t ns3::Ipv4PacketInfoTag::GetRecvIf() const [member function]
    cls.add_method('GetRecvIf', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-packet-info-tag.h: uint32_t ns3::Ipv4PacketInfoTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-packet-info-tag.h: uint8_t ns3::Ipv4PacketInfoTag::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv4-packet-info-tag.h: static ns3::TypeId ns3::Ipv4PacketInfoTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-packet-info-tag.h: void ns3::Ipv4PacketInfoTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv4-packet-info-tag.h: void ns3::Ipv4PacketInfoTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv4-packet-info-tag.h: void ns3::Ipv4PacketInfoTag::SetAddress(ns3::Ipv4Address addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Ipv4Address', 'addr')])
    ## ipv4-packet-info-tag.h: void ns3::Ipv4PacketInfoTag::SetLocalAddress(ns3::Ipv4Address addr) [member function]
    cls.add_method('SetLocalAddress', 
                   'void', 
                   [param('ns3::Ipv4Address', 'addr')])
    ## ipv4-packet-info-tag.h: void ns3::Ipv4PacketInfoTag::SetRecvIf(uint32_t ifindex) [member function]
    cls.add_method('SetRecvIf', 
                   'void', 
                   [param('uint32_t', 'ifindex')])
    ## ipv4-packet-info-tag.h: void ns3::Ipv4PacketInfoTag::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3Ipv6Header_methods(root_module, cls):
    ## ipv6-header.h: ns3::Ipv6Header::Ipv6Header(ns3::Ipv6Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Header const &', 'arg0')])
    ## ipv6-header.h: ns3::Ipv6Header::Ipv6Header() [constructor]
    cls.add_constructor([])
    ## ipv6-header.h: uint32_t ns3::Ipv6Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-header.h: ns3::Ipv6Address ns3::Ipv6Header::GetDestinationAddress() const [member function]
    cls.add_method('GetDestinationAddress', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-header.h: uint32_t ns3::Ipv6Header::GetFlowLabel() const [member function]
    cls.add_method('GetFlowLabel', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h: uint8_t ns3::Ipv6Header::GetHopLimit() const [member function]
    cls.add_method('GetHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h: ns3::TypeId ns3::Ipv6Header::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-header.h: uint8_t ns3::Ipv6Header::GetNextHeader() const [member function]
    cls.add_method('GetNextHeader', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h: uint16_t ns3::Ipv6Header::GetPayloadLength() const [member function]
    cls.add_method('GetPayloadLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h: uint32_t ns3::Ipv6Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-header.h: ns3::Ipv6Address ns3::Ipv6Header::GetSourceAddress() const [member function]
    cls.add_method('GetSourceAddress', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-header.h: uint8_t ns3::Ipv6Header::GetTrafficClass() const [member function]
    cls.add_method('GetTrafficClass', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h: static ns3::TypeId ns3::Ipv6Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-header.h: void ns3::Ipv6Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-header.h: void ns3::Ipv6Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-header.h: void ns3::Ipv6Header::SetDestinationAddress(ns3::Ipv6Address dst) [member function]
    cls.add_method('SetDestinationAddress', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst')])
    ## ipv6-header.h: void ns3::Ipv6Header::SetFlowLabel(uint32_t flow) [member function]
    cls.add_method('SetFlowLabel', 
                   'void', 
                   [param('uint32_t', 'flow')])
    ## ipv6-header.h: void ns3::Ipv6Header::SetHopLimit(uint8_t limit) [member function]
    cls.add_method('SetHopLimit', 
                   'void', 
                   [param('uint8_t', 'limit')])
    ## ipv6-header.h: void ns3::Ipv6Header::SetNextHeader(uint8_t next) [member function]
    cls.add_method('SetNextHeader', 
                   'void', 
                   [param('uint8_t', 'next')])
    ## ipv6-header.h: void ns3::Ipv6Header::SetPayloadLength(uint16_t len) [member function]
    cls.add_method('SetPayloadLength', 
                   'void', 
                   [param('uint16_t', 'len')])
    ## ipv6-header.h: void ns3::Ipv6Header::SetSourceAddress(ns3::Ipv6Address src) [member function]
    cls.add_method('SetSourceAddress', 
                   'void', 
                   [param('ns3::Ipv6Address', 'src')])
    ## ipv6-header.h: void ns3::Ipv6Header::SetTrafficClass(uint8_t traffic) [member function]
    cls.add_method('SetTrafficClass', 
                   'void', 
                   [param('uint8_t', 'traffic')])
    return

def register_Ns3Ipv6PacketInfoTag_methods(root_module, cls):
    ## ipv6-packet-info-tag.h: ns3::Ipv6PacketInfoTag::Ipv6PacketInfoTag(ns3::Ipv6PacketInfoTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PacketInfoTag const &', 'arg0')])
    ## ipv6-packet-info-tag.h: ns3::Ipv6PacketInfoTag::Ipv6PacketInfoTag() [constructor]
    cls.add_constructor([])
    ## ipv6-packet-info-tag.h: void ns3::Ipv6PacketInfoTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## ipv6-packet-info-tag.h: ns3::Ipv6Address ns3::Ipv6PacketInfoTag::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-packet-info-tag.h: uint8_t ns3::Ipv6PacketInfoTag::GetHoplimit() const [member function]
    cls.add_method('GetHoplimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-packet-info-tag.h: ns3::TypeId ns3::Ipv6PacketInfoTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-packet-info-tag.h: uint32_t ns3::Ipv6PacketInfoTag::GetRecvIf() const [member function]
    cls.add_method('GetRecvIf', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-packet-info-tag.h: uint32_t ns3::Ipv6PacketInfoTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-packet-info-tag.h: uint8_t ns3::Ipv6PacketInfoTag::GetTrafficClass() const [member function]
    cls.add_method('GetTrafficClass', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-packet-info-tag.h: static ns3::TypeId ns3::Ipv6PacketInfoTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-packet-info-tag.h: void ns3::Ipv6PacketInfoTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-packet-info-tag.h: void ns3::Ipv6PacketInfoTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv6-packet-info-tag.h: void ns3::Ipv6PacketInfoTag::SetAddress(ns3::Ipv6Address addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Ipv6Address', 'addr')])
    ## ipv6-packet-info-tag.h: void ns3::Ipv6PacketInfoTag::SetHoplimit(uint8_t ttl) [member function]
    cls.add_method('SetHoplimit', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    ## ipv6-packet-info-tag.h: void ns3::Ipv6PacketInfoTag::SetRecvIf(uint32_t ifindex) [member function]
    cls.add_method('SetRecvIf', 
                   'void', 
                   [param('uint32_t', 'ifindex')])
    ## ipv6-packet-info-tag.h: void ns3::Ipv6PacketInfoTag::SetTrafficClass(uint8_t tclass) [member function]
    cls.add_method('SetTrafficClass', 
                   'void', 
                   [param('uint8_t', 'tclass')])
    return

def register_Ns3LlcSnapHeader_methods(root_module, cls):
    ## llc-snap-header.h: ns3::LlcSnapHeader::LlcSnapHeader(ns3::LlcSnapHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LlcSnapHeader const &', 'arg0')])
    ## llc-snap-header.h: ns3::LlcSnapHeader::LlcSnapHeader() [constructor]
    cls.add_constructor([])
    ## llc-snap-header.h: uint32_t ns3::LlcSnapHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## llc-snap-header.h: ns3::TypeId ns3::LlcSnapHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## llc-snap-header.h: uint32_t ns3::LlcSnapHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## llc-snap-header.h: uint16_t ns3::LlcSnapHeader::GetType() [member function]
    cls.add_method('GetType', 
                   'uint16_t', 
                   [])
    ## llc-snap-header.h: static ns3::TypeId ns3::LlcSnapHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## llc-snap-header.h: void ns3::LlcSnapHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## llc-snap-header.h: void ns3::LlcSnapHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## llc-snap-header.h: void ns3::LlcSnapHeader::SetType(uint16_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint16_t', 'type')])
    return

def register_Ns3Queue_methods(root_module, cls):
    ## queue.h: ns3::Queue::Queue(ns3::Queue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Queue const &', 'arg0')])
    ## queue.h: ns3::Queue::Queue() [constructor]
    cls.add_constructor([])
    ## queue.h: ns3::Ptr<ns3::Packet> ns3::Queue::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## queue.h: void ns3::Queue::DequeueAll() [member function]
    cls.add_method('DequeueAll', 
                   'void', 
                   [])
    ## queue.h: bool ns3::Queue::Enqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## queue.h: uint32_t ns3::Queue::GetNBytes() const [member function]
    cls.add_method('GetNBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetNPackets() const [member function]
    cls.add_method('GetNPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetTotalDroppedBytes() const [member function]
    cls.add_method('GetTotalDroppedBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetTotalDroppedPackets() const [member function]
    cls.add_method('GetTotalDroppedPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetTotalReceivedBytes() const [member function]
    cls.add_method('GetTotalReceivedBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetTotalReceivedPackets() const [member function]
    cls.add_method('GetTotalReceivedPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: static ns3::TypeId ns3::Queue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## queue.h: bool ns3::Queue::IsEmpty() const [member function]
    cls.add_method('IsEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## queue.h: ns3::Ptr<ns3::Packet const> ns3::Queue::Peek() const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## queue.h: void ns3::Queue::ResetStatistics() [member function]
    cls.add_method('ResetStatistics', 
                   'void', 
                   [])
    ## queue.h: void ns3::Queue::Drop(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('Drop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')], 
                   visibility='protected')
    ## queue.h: ns3::Ptr<ns3::Packet> ns3::Queue::DoDequeue() [member function]
    cls.add_method('DoDequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## queue.h: bool ns3::Queue::DoEnqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoEnqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## queue.h: ns3::Ptr<ns3::Packet const> ns3::Queue::DoPeek() const [member function]
    cls.add_method('DoPeek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RadiotapHeader_methods(root_module, cls):
    ## radiotap-header.h: ns3::RadiotapHeader::RadiotapHeader(ns3::RadiotapHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadiotapHeader const &', 'arg0')])
    ## radiotap-header.h: ns3::RadiotapHeader::RadiotapHeader() [constructor]
    cls.add_constructor([])
    ## radiotap-header.h: uint32_t ns3::RadiotapHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## radiotap-header.h: uint8_t ns3::RadiotapHeader::GetAntennaNoisePower() const [member function]
    cls.add_method('GetAntennaNoisePower', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint8_t ns3::RadiotapHeader::GetAntennaSignalPower() const [member function]
    cls.add_method('GetAntennaSignalPower', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint16_t ns3::RadiotapHeader::GetChannelFlags() const [member function]
    cls.add_method('GetChannelFlags', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint16_t ns3::RadiotapHeader::GetChannelFrequency() const [member function]
    cls.add_method('GetChannelFrequency', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint8_t ns3::RadiotapHeader::GetFrameFlags() const [member function]
    cls.add_method('GetFrameFlags', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: ns3::TypeId ns3::RadiotapHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## radiotap-header.h: uint8_t ns3::RadiotapHeader::GetRate() const [member function]
    cls.add_method('GetRate', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint32_t ns3::RadiotapHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## radiotap-header.h: uint64_t ns3::RadiotapHeader::GetTsft() const [member function]
    cls.add_method('GetTsft', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: static ns3::TypeId ns3::RadiotapHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radiotap-header.h: void ns3::RadiotapHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## radiotap-header.h: void ns3::RadiotapHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## radiotap-header.h: void ns3::RadiotapHeader::SetAntennaNoisePower(int8_t noise) [member function]
    cls.add_method('SetAntennaNoisePower', 
                   'void', 
                   [param('int8_t', 'noise')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetAntennaNoisePower(double noise) [member function]
    cls.add_method('SetAntennaNoisePower', 
                   'void', 
                   [param('double', 'noise')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetAntennaSignalPower(int8_t signal) [member function]
    cls.add_method('SetAntennaSignalPower', 
                   'void', 
                   [param('int8_t', 'signal')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetAntennaSignalPower(double signal) [member function]
    cls.add_method('SetAntennaSignalPower', 
                   'void', 
                   [param('double', 'signal')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetChannelFrequencyAndFlags(uint16_t frequency, uint16_t flags) [member function]
    cls.add_method('SetChannelFrequencyAndFlags', 
                   'void', 
                   [param('uint16_t', 'frequency'), param('uint16_t', 'flags')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetFrameFlags(uint8_t flags) [member function]
    cls.add_method('SetFrameFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetRate(uint8_t rate) [member function]
    cls.add_method('SetRate', 
                   'void', 
                   [param('uint8_t', 'rate')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetTsft(uint64_t tsft) [member function]
    cls.add_method('SetTsft', 
                   'void', 
                   [param('uint64_t', 'tsft')])
    return

def register_Ns3Socket_methods(root_module, cls):
    ## socket.h: ns3::Socket::Socket(ns3::Socket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Socket const &', 'arg0')])
    ## socket.h: ns3::Socket::Socket() [constructor]
    cls.add_constructor([])
    ## socket.h: int ns3::Socket::Bind(ns3::Address const & address) [member function]
    cls.add_method('Bind', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::Bind() [member function]
    cls.add_method('Bind', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: void ns3::Socket::BindToNetDevice(ns3::Ptr<ns3::NetDevice> netdevice) [member function]
    cls.add_method('BindToNetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'netdevice')], 
                   is_virtual=True)
    ## socket.h: int ns3::Socket::Close() [member function]
    cls.add_method('Close', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::Connect(ns3::Address const & address) [member function]
    cls.add_method('Connect', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: static ns3::Ptr<ns3::Socket> ns3::Socket::CreateSocket(ns3::Ptr<ns3::Node> node, ns3::TypeId tid) [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::TypeId', 'tid')], 
                   is_static=True)
    ## socket.h: bool ns3::Socket::GetAllowBroadcast() const [member function]
    cls.add_method('GetAllowBroadcast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::NetDevice> ns3::Socket::GetBoundNetDevice() [member function]
    cls.add_method('GetBoundNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## socket.h: ns3::Socket::SocketErrno ns3::Socket::GetErrno() const [member function]
    cls.add_method('GetErrno', 
                   'ns3::Socket::SocketErrno', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::Node> ns3::Socket::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::Socket::GetRxAvailable() const [member function]
    cls.add_method('GetRxAvailable', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: int ns3::Socket::GetSockName(ns3::Address & address) const [member function]
    cls.add_method('GetSockName', 
                   'int', 
                   [param('ns3::Address &', 'address')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::Socket::GetTxAvailable() const [member function]
    cls.add_method('GetTxAvailable', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: int ns3::Socket::Listen() [member function]
    cls.add_method('Listen', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::Packet> ns3::Socket::Recv(uint32_t maxSize, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::Packet> ns3::Socket::Recv() [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## socket.h: int ns3::Socket::Recv(uint8_t * buf, uint32_t size, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'int', 
                   [param('uint8_t *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags')])
    ## socket.h: ns3::Ptr<ns3::Packet> ns3::Socket::RecvFrom(uint32_t maxSize, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::Packet> ns3::Socket::RecvFrom(ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Address &', 'fromAddress')])
    ## socket.h: int ns3::Socket::RecvFrom(uint8_t * buf, uint32_t size, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'int', 
                   [param('uint8_t *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')])
    ## socket.h: int ns3::Socket::Send(ns3::Ptr<ns3::Packet> p, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## socket.h: int ns3::Socket::Send(uint8_t const * buf, uint32_t size, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('uint8_t const *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags')])
    ## socket.h: int ns3::Socket::SendTo(ns3::Ptr<ns3::Packet> p, uint32_t flags, ns3::Address const & toAddress) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags'), param('ns3::Address const &', 'toAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::SendTo(uint8_t const * buf, uint32_t size, uint32_t flags, ns3::Address const & address) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('uint8_t const *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags'), param('ns3::Address const &', 'address')])
    ## socket.h: void ns3::Socket::SetAcceptCallback(ns3::Callback<bool, ns3::Ptr<ns3::Socket>, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionRequest, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> newConnectionCreated) [member function]
    cls.add_method('SetAcceptCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::Socket >, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionRequest'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'newConnectionCreated')])
    ## socket.h: bool ns3::Socket::SetAllowBroadcast(bool allowBroadcast) [member function]
    cls.add_method('SetAllowBroadcast', 
                   'bool', 
                   [param('bool', 'allowBroadcast')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: void ns3::Socket::SetCloseCallbacks(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> normalClose, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> errorClose) [member function]
    cls.add_method('SetCloseCallbacks', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'normalClose'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'errorClose')])
    ## socket.h: void ns3::Socket::SetConnectCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionSucceeded, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionFailed) [member function]
    cls.add_method('SetConnectCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionSucceeded'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionFailed')])
    ## socket.h: void ns3::Socket::SetDataSentCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> dataSent) [member function]
    cls.add_method('SetDataSentCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'dataSent')])
    ## socket.h: void ns3::Socket::SetRecvCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> arg0) [member function]
    cls.add_method('SetRecvCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'arg0')])
    ## socket.h: void ns3::Socket::SetRecvPktInfo(bool flag) [member function]
    cls.add_method('SetRecvPktInfo', 
                   'void', 
                   [param('bool', 'flag')])
    ## socket.h: void ns3::Socket::SetSendCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> sendCb) [member function]
    cls.add_method('SetSendCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'sendCb')])
    ## socket.h: int ns3::Socket::ShutdownRecv() [member function]
    cls.add_method('ShutdownRecv', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::ShutdownSend() [member function]
    cls.add_method('ShutdownSend', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: void ns3::Socket::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## socket.h: void ns3::Socket::NotifyConnectionFailed() [member function]
    cls.add_method('NotifyConnectionFailed', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: bool ns3::Socket::NotifyConnectionRequest(ns3::Address const & from) [member function]
    cls.add_method('NotifyConnectionRequest', 
                   'bool', 
                   [param('ns3::Address const &', 'from')], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyConnectionSucceeded() [member function]
    cls.add_method('NotifyConnectionSucceeded', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyDataRecv() [member function]
    cls.add_method('NotifyDataRecv', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyDataSent(uint32_t size) [member function]
    cls.add_method('NotifyDataSent', 
                   'void', 
                   [param('uint32_t', 'size')], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyErrorClose() [member function]
    cls.add_method('NotifyErrorClose', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyNewConnectionCreated(ns3::Ptr<ns3::Socket> socket, ns3::Address const & from) [member function]
    cls.add_method('NotifyNewConnectionCreated', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket'), param('ns3::Address const &', 'from')], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyNormalClose() [member function]
    cls.add_method('NotifyNormalClose', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifySend(uint32_t spaceAvailable) [member function]
    cls.add_method('NotifySend', 
                   'void', 
                   [param('uint32_t', 'spaceAvailable')], 
                   visibility='protected')
    return

def register_Ns3SocketAddressTag_methods(root_module, cls):
    ## socket.h: ns3::SocketAddressTag::SocketAddressTag(ns3::SocketAddressTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketAddressTag const &', 'arg0')])
    ## socket.h: ns3::SocketAddressTag::SocketAddressTag() [constructor]
    cls.add_constructor([])
    ## socket.h: void ns3::SocketAddressTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h: ns3::Address ns3::SocketAddressTag::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## socket.h: ns3::TypeId ns3::SocketAddressTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::SocketAddressTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: static ns3::TypeId ns3::SocketAddressTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h: void ns3::SocketAddressTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketAddressTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketAddressTag::SetAddress(ns3::Address addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'addr')])
    return

def register_Ns3SocketFactory_methods(root_module, cls):
    ## socket-factory.h: ns3::SocketFactory::SocketFactory(ns3::SocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketFactory const &', 'arg0')])
    ## socket-factory.h: ns3::SocketFactory::SocketFactory() [constructor]
    cls.add_constructor([])
    ## socket-factory.h: ns3::Ptr<ns3::Socket> ns3::SocketFactory::CreateSocket() [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket-factory.h: static ns3::TypeId ns3::SocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3SocketIpTtlTag_methods(root_module, cls):
    ## socket.h: ns3::SocketIpTtlTag::SocketIpTtlTag(ns3::SocketIpTtlTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketIpTtlTag const &', 'arg0')])
    ## socket.h: ns3::SocketIpTtlTag::SocketIpTtlTag() [constructor]
    cls.add_constructor([])
    ## socket.h: void ns3::SocketIpTtlTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h: ns3::TypeId ns3::SocketIpTtlTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::SocketIpTtlTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: uint8_t ns3::SocketIpTtlTag::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## socket.h: static ns3::TypeId ns3::SocketIpTtlTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h: void ns3::SocketIpTtlTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketIpTtlTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketIpTtlTag::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3SocketSetDontFragmentTag_methods(root_module, cls):
    ## socket.h: ns3::SocketSetDontFragmentTag::SocketSetDontFragmentTag(ns3::SocketSetDontFragmentTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketSetDontFragmentTag const &', 'arg0')])
    ## socket.h: ns3::SocketSetDontFragmentTag::SocketSetDontFragmentTag() [constructor]
    cls.add_constructor([])
    ## socket.h: void ns3::SocketSetDontFragmentTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h: void ns3::SocketSetDontFragmentTag::Disable() [member function]
    cls.add_method('Disable', 
                   'void', 
                   [])
    ## socket.h: void ns3::SocketSetDontFragmentTag::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [])
    ## socket.h: ns3::TypeId ns3::SocketSetDontFragmentTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::SocketSetDontFragmentTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: static ns3::TypeId ns3::SocketSetDontFragmentTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h: bool ns3::SocketSetDontFragmentTag::IsEnabled() const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## socket.h: void ns3::SocketSetDontFragmentTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketSetDontFragmentTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3SpectrumPhy_methods(root_module, cls):
    ## spectrum-phy.h: ns3::SpectrumPhy::SpectrumPhy() [constructor]
    cls.add_constructor([])
    ## spectrum-phy.h: ns3::SpectrumPhy::SpectrumPhy(ns3::SpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumPhy const &', 'arg0')])
    ## spectrum-phy.h: ns3::Ptr<ns3::Object> ns3::SpectrumPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: ns3::Ptr<ns3::Object> ns3::SpectrumPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: ns3::Ptr<ns3::SpectrumModel const> ns3::SpectrumPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## spectrum-phy.h: static ns3::TypeId ns3::SpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-phy.h: void ns3::SpectrumPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: void ns3::SpectrumPhy::SetDevice(ns3::Ptr<ns3::Object> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'd')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: void ns3::SpectrumPhy::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: void ns3::SpectrumPhy::StartRx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::SpectrumType st, ns3::Time duration) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3TcpSocket_methods(root_module, cls):
    ## tcp-socket.h: ns3::TcpSocket::TcpSocket(ns3::TcpSocket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TcpSocket const &', 'arg0')])
    ## tcp-socket.h: ns3::TcpSocket::TcpSocket() [constructor]
    cls.add_constructor([])
    ## tcp-socket.h: static ns3::TypeId ns3::TcpSocket::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tcp-socket.h: uint32_t ns3::TcpSocket::GetConnCount() const [member function]
    cls.add_method('GetConnCount', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: ns3::Time ns3::TcpSocket::GetConnTimeout() const [member function]
    cls.add_method('GetConnTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: uint32_t ns3::TcpSocket::GetDelAckMaxCount() const [member function]
    cls.add_method('GetDelAckMaxCount', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: ns3::Time ns3::TcpSocket::GetDelAckTimeout() const [member function]
    cls.add_method('GetDelAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: uint32_t ns3::TcpSocket::GetInitialCwnd() const [member function]
    cls.add_method('GetInitialCwnd', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: uint32_t ns3::TcpSocket::GetRcvBufSize() const [member function]
    cls.add_method('GetRcvBufSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: uint32_t ns3::TcpSocket::GetSSThresh() const [member function]
    cls.add_method('GetSSThresh', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: uint32_t ns3::TcpSocket::GetSegSize() const [member function]
    cls.add_method('GetSegSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: uint32_t ns3::TcpSocket::GetSndBufSize() const [member function]
    cls.add_method('GetSndBufSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetConnCount(uint32_t count) [member function]
    cls.add_method('SetConnCount', 
                   'void', 
                   [param('uint32_t', 'count')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetConnTimeout(ns3::Time timeout) [member function]
    cls.add_method('SetConnTimeout', 
                   'void', 
                   [param('ns3::Time', 'timeout')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetDelAckMaxCount(uint32_t count) [member function]
    cls.add_method('SetDelAckMaxCount', 
                   'void', 
                   [param('uint32_t', 'count')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetDelAckTimeout(ns3::Time timeout) [member function]
    cls.add_method('SetDelAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'timeout')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetInitialCwnd(uint32_t count) [member function]
    cls.add_method('SetInitialCwnd', 
                   'void', 
                   [param('uint32_t', 'count')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetRcvBufSize(uint32_t size) [member function]
    cls.add_method('SetRcvBufSize', 
                   'void', 
                   [param('uint32_t', 'size')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetSSThresh(uint32_t threshold) [member function]
    cls.add_method('SetSSThresh', 
                   'void', 
                   [param('uint32_t', 'threshold')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetSegSize(uint32_t size) [member function]
    cls.add_method('SetSegSize', 
                   'void', 
                   [param('uint32_t', 'size')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## tcp-socket.h: void ns3::TcpSocket::SetSndBufSize(uint32_t size) [member function]
    cls.add_method('SetSndBufSize', 
                   'void', 
                   [param('uint32_t', 'size')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3TcpSocketFactory_methods(root_module, cls):
    ## tcp-socket-factory.h: ns3::TcpSocketFactory::TcpSocketFactory() [constructor]
    cls.add_constructor([])
    ## tcp-socket-factory.h: ns3::TcpSocketFactory::TcpSocketFactory(ns3::TcpSocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TcpSocketFactory const &', 'arg0')])
    ## tcp-socket-factory.h: static ns3::TypeId ns3::TcpSocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UdpSocket_methods(root_module, cls):
    ## udp-socket.h: ns3::UdpSocket::UdpSocket(ns3::UdpSocket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpSocket const &', 'arg0')])
    ## udp-socket.h: ns3::UdpSocket::UdpSocket() [constructor]
    cls.add_constructor([])
    ## udp-socket.h: static ns3::TypeId ns3::UdpSocket::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-socket.h: int ns3::UdpSocket::MulticastJoinGroup(uint32_t interface, ns3::Address const & groupAddress) [member function]
    cls.add_method('MulticastJoinGroup', 
                   'int', 
                   [param('uint32_t', 'interface'), param('ns3::Address const &', 'groupAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## udp-socket.h: int ns3::UdpSocket::MulticastLeaveGroup(uint32_t interface, ns3::Address const & groupAddress) [member function]
    cls.add_method('MulticastLeaveGroup', 
                   'int', 
                   [param('uint32_t', 'interface'), param('ns3::Address const &', 'groupAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## udp-socket.h: int32_t ns3::UdpSocket::GetIpMulticastIf() const [member function]
    cls.add_method('GetIpMulticastIf', 
                   'int32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## udp-socket.h: bool ns3::UdpSocket::GetIpMulticastLoop() const [member function]
    cls.add_method('GetIpMulticastLoop', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## udp-socket.h: uint8_t ns3::UdpSocket::GetIpMulticastTtl() const [member function]
    cls.add_method('GetIpMulticastTtl', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## udp-socket.h: uint8_t ns3::UdpSocket::GetIpTtl() const [member function]
    cls.add_method('GetIpTtl', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## udp-socket.h: bool ns3::UdpSocket::GetMtuDiscover() const [member function]
    cls.add_method('GetMtuDiscover', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## udp-socket.h: uint32_t ns3::UdpSocket::GetRcvBufSize() const [member function]
    cls.add_method('GetRcvBufSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## udp-socket.h: void ns3::UdpSocket::SetIpMulticastIf(int32_t ipIf) [member function]
    cls.add_method('SetIpMulticastIf', 
                   'void', 
                   [param('int32_t', 'ipIf')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## udp-socket.h: void ns3::UdpSocket::SetIpMulticastLoop(bool loop) [member function]
    cls.add_method('SetIpMulticastLoop', 
                   'void', 
                   [param('bool', 'loop')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## udp-socket.h: void ns3::UdpSocket::SetIpMulticastTtl(uint8_t ipTtl) [member function]
    cls.add_method('SetIpMulticastTtl', 
                   'void', 
                   [param('uint8_t', 'ipTtl')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## udp-socket.h: void ns3::UdpSocket::SetIpTtl(uint8_t ipTtl) [member function]
    cls.add_method('SetIpTtl', 
                   'void', 
                   [param('uint8_t', 'ipTtl')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## udp-socket.h: void ns3::UdpSocket::SetMtuDiscover(bool discover) [member function]
    cls.add_method('SetMtuDiscover', 
                   'void', 
                   [param('bool', 'discover')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## udp-socket.h: void ns3::UdpSocket::SetRcvBufSize(uint32_t size) [member function]
    cls.add_method('SetRcvBufSize', 
                   'void', 
                   [param('uint32_t', 'size')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3UdpSocketFactory_methods(root_module, cls):
    ## udp-socket-factory.h: ns3::UdpSocketFactory::UdpSocketFactory() [constructor]
    cls.add_constructor([])
    ## udp-socket-factory.h: ns3::UdpSocketFactory::UdpSocketFactory(ns3::UdpSocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpSocketFactory const &', 'arg0')])
    ## udp-socket-factory.h: static ns3::TypeId ns3::UdpSocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3Application_methods(root_module, cls):
    ## application.h: ns3::Application::Application(ns3::Application const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Application const &', 'arg0')])
    ## application.h: ns3::Application::Application() [constructor]
    cls.add_constructor([])
    ## application.h: ns3::Ptr<ns3::Node> ns3::Application::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## application.h: static ns3::TypeId ns3::Application::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## application.h: void ns3::Application::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## application.h: void ns3::Application::SetStartTime(ns3::Time start) [member function]
    cls.add_method('SetStartTime', 
                   'void', 
                   [param('ns3::Time', 'start')])
    ## application.h: void ns3::Application::SetStopTime(ns3::Time stop) [member function]
    cls.add_method('SetStopTime', 
                   'void', 
                   [param('ns3::Time', 'stop')])
    ## application.h: void ns3::Application::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## application.h: void ns3::Application::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## application.h: void ns3::Application::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## application.h: void ns3::Application::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Channel_methods(root_module, cls):
    ## channel.h: ns3::Channel::Channel(ns3::Channel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Channel const &', 'arg0')])
    ## channel.h: ns3::Channel::Channel() [constructor]
    cls.add_constructor([])
    ## channel.h: ns3::Ptr<ns3::NetDevice> ns3::Channel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## channel.h: uint32_t ns3::Channel::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## channel.h: uint32_t ns3::Channel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## channel.h: static ns3::TypeId ns3::Channel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3DropTailQueue_methods(root_module, cls):
    ## drop-tail-queue.h: ns3::DropTailQueue::DropTailQueue(ns3::DropTailQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DropTailQueue const &', 'arg0')])
    ## drop-tail-queue.h: ns3::DropTailQueue::DropTailQueue() [constructor]
    cls.add_constructor([])
    ## drop-tail-queue.h: ns3::DropTailQueue::Mode ns3::DropTailQueue::GetMode() [member function]
    cls.add_method('GetMode', 
                   'ns3::DropTailQueue::Mode', 
                   [])
    ## drop-tail-queue.h: static ns3::TypeId ns3::DropTailQueue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## drop-tail-queue.h: void ns3::DropTailQueue::SetMode(ns3::DropTailQueue::Mode mode) [member function]
    cls.add_method('SetMode', 
                   'void', 
                   [param('ns3::DropTailQueue::Mode', 'mode')])
    ## drop-tail-queue.h: ns3::Ptr<ns3::Packet> ns3::DropTailQueue::DoDequeue() [member function]
    cls.add_method('DoDequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   visibility='private', is_virtual=True)
    ## drop-tail-queue.h: bool ns3::DropTailQueue::DoEnqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoEnqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## drop-tail-queue.h: ns3::Ptr<ns3::Packet const> ns3::DropTailQueue::DoPeek() const [member function]
    cls.add_method('DoPeek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3EthernetHeader_methods(root_module, cls):
    ## ethernet-header.h: ns3::EthernetHeader::EthernetHeader(ns3::EthernetHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EthernetHeader const &', 'arg0')])
    ## ethernet-header.h: ns3::EthernetHeader::EthernetHeader(bool hasPreamble) [constructor]
    cls.add_constructor([param('bool', 'hasPreamble')])
    ## ethernet-header.h: ns3::EthernetHeader::EthernetHeader() [constructor]
    cls.add_constructor([])
    ## ethernet-header.h: uint32_t ns3::EthernetHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ethernet-header.h: ns3::Mac48Address ns3::EthernetHeader::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: uint32_t ns3::EthernetHeader::GetHeaderSize() const [member function]
    cls.add_method('GetHeaderSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: ns3::TypeId ns3::EthernetHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ethernet-header.h: uint16_t ns3::EthernetHeader::GetLengthType() const [member function]
    cls.add_method('GetLengthType', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: ns3::ethernet_header_t ns3::EthernetHeader::GetPacketType() const [member function]
    cls.add_method('GetPacketType', 
                   'ns3::ethernet_header_t', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: uint64_t ns3::EthernetHeader::GetPreambleSfd() const [member function]
    cls.add_method('GetPreambleSfd', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: uint32_t ns3::EthernetHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ethernet-header.h: ns3::Mac48Address ns3::EthernetHeader::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: static ns3::TypeId ns3::EthernetHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ethernet-header.h: void ns3::EthernetHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ethernet-header.h: void ns3::EthernetHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ethernet-header.h: void ns3::EthernetHeader::SetDestination(ns3::Mac48Address destination) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination')])
    ## ethernet-header.h: void ns3::EthernetHeader::SetLengthType(uint16_t size) [member function]
    cls.add_method('SetLengthType', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## ethernet-header.h: void ns3::EthernetHeader::SetPreambleSfd(uint64_t preambleSfd) [member function]
    cls.add_method('SetPreambleSfd', 
                   'void', 
                   [param('uint64_t', 'preambleSfd')])
    ## ethernet-header.h: void ns3::EthernetHeader::SetSource(ns3::Mac48Address source) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Mac48Address', 'source')])
    return

def register_Ns3EthernetTrailer_methods(root_module, cls):
    ## ethernet-trailer.h: ns3::EthernetTrailer::EthernetTrailer(ns3::EthernetTrailer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EthernetTrailer const &', 'arg0')])
    ## ethernet-trailer.h: ns3::EthernetTrailer::EthernetTrailer() [constructor]
    cls.add_constructor([])
    ## ethernet-trailer.h: void ns3::EthernetTrailer::CalcFcs(ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('CalcFcs', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## ethernet-trailer.h: bool ns3::EthernetTrailer::CheckFcs(ns3::Ptr<ns3::Packet const> p) const [member function]
    cls.add_method('CheckFcs', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_const=True)
    ## ethernet-trailer.h: uint32_t ns3::EthernetTrailer::Deserialize(ns3::Buffer::Iterator end) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'end')], 
                   is_virtual=True)
    ## ethernet-trailer.h: void ns3::EthernetTrailer::EnableFcs(bool enable) [member function]
    cls.add_method('EnableFcs', 
                   'void', 
                   [param('bool', 'enable')])
    ## ethernet-trailer.h: uint32_t ns3::EthernetTrailer::GetFcs() [member function]
    cls.add_method('GetFcs', 
                   'uint32_t', 
                   [])
    ## ethernet-trailer.h: ns3::TypeId ns3::EthernetTrailer::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ethernet-trailer.h: uint32_t ns3::EthernetTrailer::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ethernet-trailer.h: uint32_t ns3::EthernetTrailer::GetTrailerSize() const [member function]
    cls.add_method('GetTrailerSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ethernet-trailer.h: static ns3::TypeId ns3::EthernetTrailer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ethernet-trailer.h: void ns3::EthernetTrailer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ethernet-trailer.h: void ns3::EthernetTrailer::Serialize(ns3::Buffer::Iterator end) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'end')], 
                   is_const=True, is_virtual=True)
    ## ethernet-trailer.h: void ns3::EthernetTrailer::SetFcs(uint32_t fcs) [member function]
    cls.add_method('SetFcs', 
                   'void', 
                   [param('uint32_t', 'fcs')])
    return

def register_Ns3Ipv4_methods(root_module, cls):
    ## ipv4.h: ns3::Ipv4::Ipv4(ns3::Ipv4 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4 const &', 'arg0')])
    ## ipv4.h: ns3::Ipv4::Ipv4() [constructor]
    cls.add_constructor([])
    ## ipv4.h: bool ns3::Ipv4::AddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('AddAddress', 
                   'bool', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: uint32_t ns3::Ipv4::AddInterface(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddInterface', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: ns3::Ipv4InterfaceAddress ns3::Ipv4::GetAddress(uint32_t interface, uint32_t addressIndex) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4InterfaceAddress', 
                   [param('uint32_t', 'interface'), param('uint32_t', 'addressIndex')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: int32_t ns3::Ipv4::GetInterfaceForAddress(ns3::Ipv4Address address) const [member function]
    cls.add_method('GetInterfaceForAddress', 
                   'int32_t', 
                   [param('ns3::Ipv4Address', 'address')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: int32_t ns3::Ipv4::GetInterfaceForDevice(ns3::Ptr<const ns3::NetDevice> device) const [member function]
    cls.add_method('GetInterfaceForDevice', 
                   'int32_t', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: int32_t ns3::Ipv4::GetInterfaceForPrefix(ns3::Ipv4Address address, ns3::Ipv4Mask mask) const [member function]
    cls.add_method('GetInterfaceForPrefix', 
                   'int32_t', 
                   [param('ns3::Ipv4Address', 'address'), param('ns3::Ipv4Mask', 'mask')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: uint16_t ns3::Ipv4::GetMetric(uint32_t interface) const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: uint16_t ns3::Ipv4::GetMtu(uint32_t interface) const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: uint32_t ns3::Ipv4::GetNAddresses(uint32_t interface) const [member function]
    cls.add_method('GetNAddresses', 
                   'uint32_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: uint32_t ns3::Ipv4::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv4::GetNetDevice(uint32_t interface) [member function]
    cls.add_method('GetNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4::GetRoutingProtocol() const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: static ns3::TypeId ns3::Ipv4::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4.h: bool ns3::Ipv4::IsDestinationAddress(ns3::Ipv4Address address, uint32_t iif) const [member function]
    cls.add_method('IsDestinationAddress', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address'), param('uint32_t', 'iif')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: bool ns3::Ipv4::IsForwarding(uint32_t interface) const [member function]
    cls.add_method('IsForwarding', 
                   'bool', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: bool ns3::Ipv4::IsUp(uint32_t interface) const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h: bool ns3::Ipv4::RemoveAddress(uint32_t interface, uint32_t addressIndex) [member function]
    cls.add_method('RemoveAddress', 
                   'bool', 
                   [param('uint32_t', 'interface'), param('uint32_t', 'addressIndex')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: ns3::Ipv4Address ns3::Ipv4::SelectSourceAddress(ns3::Ptr<const ns3::NetDevice> device, ns3::Ipv4Address dst, ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e scope) [member function]
    cls.add_method('SelectSourceAddress', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device'), param('ns3::Ipv4Address', 'dst'), param('ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e', 'scope')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: void ns3::Ipv4::SetDown(uint32_t interface) [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: void ns3::Ipv4::SetForwarding(uint32_t interface, bool val) [member function]
    cls.add_method('SetForwarding', 
                   'void', 
                   [param('uint32_t', 'interface'), param('bool', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: void ns3::Ipv4::SetMetric(uint32_t interface, uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'interface'), param('uint16_t', 'metric')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: void ns3::Ipv4::SetRoutingProtocol(ns3::Ptr<ns3::Ipv4RoutingProtocol> routingProtocol) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4RoutingProtocol >', 'routingProtocol')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: void ns3::Ipv4::SetUp(uint32_t interface) [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h: ns3::Ipv4::IF_ANY [variable]
    cls.add_static_attribute('IF_ANY', 'uint32_t const', is_const=True)
    ## ipv4.h: bool ns3::Ipv4::GetIpForward() const [member function]
    cls.add_method('GetIpForward', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## ipv4.h: bool ns3::Ipv4::GetWeakEsModel() const [member function]
    cls.add_method('GetWeakEsModel', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## ipv4.h: void ns3::Ipv4::SetIpForward(bool forward) [member function]
    cls.add_method('SetIpForward', 
                   'void', 
                   [param('bool', 'forward')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## ipv4.h: void ns3::Ipv4::SetWeakEsModel(bool model) [member function]
    cls.add_method('SetWeakEsModel', 
                   'void', 
                   [param('bool', 'model')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3Ipv4AddressChecker_methods(root_module, cls):
    ## ipv4-address.h: ns3::Ipv4AddressChecker::Ipv4AddressChecker() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4AddressChecker::Ipv4AddressChecker(ns3::Ipv4AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressChecker const &', 'arg0')])
    return

def register_Ns3Ipv4AddressValue_methods(root_module, cls):
    ## ipv4-address.h: ns3::Ipv4AddressValue::Ipv4AddressValue() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4AddressValue::Ipv4AddressValue(ns3::Ipv4AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressValue const &', 'arg0')])
    ## ipv4-address.h: ns3::Ipv4AddressValue::Ipv4AddressValue(ns3::Ipv4Address const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address const &', 'value')])
    ## ipv4-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Ipv4AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h: bool ns3::Ipv4AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv4-address.h: ns3::Ipv4Address ns3::Ipv4AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: std::string ns3::Ipv4AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h: void ns3::Ipv4AddressValue::Set(ns3::Ipv4Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'value')])
    return

def register_Ns3Ipv4MaskChecker_methods(root_module, cls):
    ## ipv4-address.h: ns3::Ipv4MaskChecker::Ipv4MaskChecker() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4MaskChecker::Ipv4MaskChecker(ns3::Ipv4MaskChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MaskChecker const &', 'arg0')])
    return

def register_Ns3Ipv4MaskValue_methods(root_module, cls):
    ## ipv4-address.h: ns3::Ipv4MaskValue::Ipv4MaskValue() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4MaskValue::Ipv4MaskValue(ns3::Ipv4MaskValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MaskValue const &', 'arg0')])
    ## ipv4-address.h: ns3::Ipv4MaskValue::Ipv4MaskValue(ns3::Ipv4Mask const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv4Mask const &', 'value')])
    ## ipv4-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Ipv4MaskValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h: bool ns3::Ipv4MaskValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv4-address.h: ns3::Ipv4Mask ns3::Ipv4MaskValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: std::string ns3::Ipv4MaskValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h: void ns3::Ipv4MaskValue::Set(ns3::Ipv4Mask const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv4Mask const &', 'value')])
    return

def register_Ns3Ipv4MulticastRoute_methods(root_module, cls):
    ## ipv4-route.h: ns3::Ipv4MulticastRoute::Ipv4MulticastRoute(ns3::Ipv4MulticastRoute const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MulticastRoute const &', 'arg0')])
    ## ipv4-route.h: ns3::Ipv4MulticastRoute::Ipv4MulticastRoute() [constructor]
    cls.add_constructor([])
    ## ipv4-route.h: ns3::Ipv4Address ns3::Ipv4MulticastRoute::GetGroup() const [member function]
    cls.add_method('GetGroup', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h: ns3::Ipv4Address ns3::Ipv4MulticastRoute::GetOrigin() const [member function]
    cls.add_method('GetOrigin', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h: uint32_t ns3::Ipv4MulticastRoute::GetOutputTtl(uint32_t oif) const [member function]
    cls.add_method('GetOutputTtl', 
                   'uint32_t', 
                   [param('uint32_t', 'oif')], 
                   is_const=True)
    ## ipv4-route.h: uint32_t ns3::Ipv4MulticastRoute::GetParent() const [member function]
    cls.add_method('GetParent', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-route.h: void ns3::Ipv4MulticastRoute::SetGroup(ns3::Ipv4Address const group) [member function]
    cls.add_method('SetGroup', 
                   'void', 
                   [param('ns3::Ipv4Address const', 'group')])
    ## ipv4-route.h: void ns3::Ipv4MulticastRoute::SetOrigin(ns3::Ipv4Address const origin) [member function]
    cls.add_method('SetOrigin', 
                   'void', 
                   [param('ns3::Ipv4Address const', 'origin')])
    ## ipv4-route.h: void ns3::Ipv4MulticastRoute::SetOutputTtl(uint32_t oif, uint32_t ttl) [member function]
    cls.add_method('SetOutputTtl', 
                   'void', 
                   [param('uint32_t', 'oif'), param('uint32_t', 'ttl')])
    ## ipv4-route.h: void ns3::Ipv4MulticastRoute::SetParent(uint32_t iif) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('uint32_t', 'iif')])
    ## ipv4-route.h: ns3::Ipv4MulticastRoute::MAX_INTERFACES [variable]
    cls.add_static_attribute('MAX_INTERFACES', 'uint32_t const', is_const=True)
    ## ipv4-route.h: ns3::Ipv4MulticastRoute::MAX_TTL [variable]
    cls.add_static_attribute('MAX_TTL', 'uint32_t const', is_const=True)
    return

def register_Ns3Ipv4RawSocketFactory_methods(root_module, cls):
    ## ipv4-raw-socket-factory.h: ns3::Ipv4RawSocketFactory::Ipv4RawSocketFactory() [constructor]
    cls.add_constructor([])
    ## ipv4-raw-socket-factory.h: ns3::Ipv4RawSocketFactory::Ipv4RawSocketFactory(ns3::Ipv4RawSocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4RawSocketFactory const &', 'arg0')])
    ## ipv4-raw-socket-factory.h: static ns3::TypeId ns3::Ipv4RawSocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3Ipv4Route_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv4-route.h: ns3::Ipv4Route::Ipv4Route(ns3::Ipv4Route const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Route const &', 'arg0')])
    ## ipv4-route.h: ns3::Ipv4Route::Ipv4Route() [constructor]
    cls.add_constructor([])
    ## ipv4-route.h: ns3::Ipv4Address ns3::Ipv4Route::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h: ns3::Ipv4Address ns3::Ipv4Route::GetGateway() const [member function]
    cls.add_method('GetGateway', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv4Route::GetOutputDevice() const [member function]
    cls.add_method('GetOutputDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## ipv4-route.h: ns3::Ipv4Address ns3::Ipv4Route::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h: void ns3::Ipv4Route::SetDestination(ns3::Ipv4Address dest) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest')])
    ## ipv4-route.h: void ns3::Ipv4Route::SetGateway(ns3::Ipv4Address gw) [member function]
    cls.add_method('SetGateway', 
                   'void', 
                   [param('ns3::Ipv4Address', 'gw')])
    ## ipv4-route.h: void ns3::Ipv4Route::SetOutputDevice(ns3::Ptr<ns3::NetDevice> outputDevice) [member function]
    cls.add_method('SetOutputDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'outputDevice')])
    ## ipv4-route.h: void ns3::Ipv4Route::SetSource(ns3::Ipv4Address src) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Ipv4Address', 'src')])
    return

def register_Ns3Ipv4RoutingProtocol_methods(root_module, cls):
    ## ipv4-routing-protocol.h: ns3::Ipv4RoutingProtocol::Ipv4RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## ipv4-routing-protocol.h: ns3::Ipv4RoutingProtocol::Ipv4RoutingProtocol(ns3::Ipv4RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4RoutingProtocol const &', 'arg0')])
    ## ipv4-routing-protocol.h: static ns3::TypeId ns3::Ipv4RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-routing-protocol.h: void ns3::Ipv4RoutingProtocol::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4-routing-protocol.h: void ns3::Ipv4RoutingProtocol::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4-routing-protocol.h: void ns3::Ipv4RoutingProtocol::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4-routing-protocol.h: void ns3::Ipv4RoutingProtocol::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4-routing-protocol.h: bool ns3::Ipv4RoutingProtocol::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4-routing-protocol.h: ns3::Ptr<ns3::Ipv4Route> ns3::Ipv4RoutingProtocol::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4-routing-protocol.h: void ns3::Ipv4RoutingProtocol::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3Ipv6_methods(root_module, cls):
    ## ipv6.h: ns3::Ipv6::Ipv6(ns3::Ipv6 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6 const &', 'arg0')])
    ## ipv6.h: ns3::Ipv6::Ipv6() [constructor]
    cls.add_constructor([])
    ## ipv6.h: bool ns3::Ipv6::AddAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('AddAddress', 
                   'bool', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: uint32_t ns3::Ipv6::AddInterface(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddInterface', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: ns3::Ipv6InterfaceAddress ns3::Ipv6::GetAddress(uint32_t interface, uint32_t addressIndex) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv6InterfaceAddress', 
                   [param('uint32_t', 'interface'), param('uint32_t', 'addressIndex')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: int32_t ns3::Ipv6::GetInterfaceForAddress(ns3::Ipv6Address address) const [member function]
    cls.add_method('GetInterfaceForAddress', 
                   'int32_t', 
                   [param('ns3::Ipv6Address', 'address')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: int32_t ns3::Ipv6::GetInterfaceForDevice(ns3::Ptr<const ns3::NetDevice> device) const [member function]
    cls.add_method('GetInterfaceForDevice', 
                   'int32_t', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: int32_t ns3::Ipv6::GetInterfaceForPrefix(ns3::Ipv6Address address, ns3::Ipv6Prefix mask) const [member function]
    cls.add_method('GetInterfaceForPrefix', 
                   'int32_t', 
                   [param('ns3::Ipv6Address', 'address'), param('ns3::Ipv6Prefix', 'mask')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: uint16_t ns3::Ipv6::GetMetric(uint32_t interface) const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: uint16_t ns3::Ipv6::GetMtu(uint32_t interface) const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: uint32_t ns3::Ipv6::GetNAddresses(uint32_t interface) const [member function]
    cls.add_method('GetNAddresses', 
                   'uint32_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: uint32_t ns3::Ipv6::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv6::GetNetDevice(uint32_t interface) [member function]
    cls.add_method('GetNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: ns3::Ptr<ns3::Ipv6RoutingProtocol> ns3::Ipv6::GetRoutingProtocol() const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv6RoutingProtocol >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: static ns3::TypeId ns3::Ipv6::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6.h: bool ns3::Ipv6::IsForwarding(uint32_t interface) const [member function]
    cls.add_method('IsForwarding', 
                   'bool', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: bool ns3::Ipv6::IsUp(uint32_t interface) const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6.h: void ns3::Ipv6::RegisterExtensions() [member function]
    cls.add_method('RegisterExtensions', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: void ns3::Ipv6::RegisterOptions() [member function]
    cls.add_method('RegisterOptions', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: bool ns3::Ipv6::RemoveAddress(uint32_t interface, uint32_t addressIndex) [member function]
    cls.add_method('RemoveAddress', 
                   'bool', 
                   [param('uint32_t', 'interface'), param('uint32_t', 'addressIndex')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: void ns3::Ipv6::SetDown(uint32_t interface) [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: void ns3::Ipv6::SetForwarding(uint32_t interface, bool val) [member function]
    cls.add_method('SetForwarding', 
                   'void', 
                   [param('uint32_t', 'interface'), param('bool', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: void ns3::Ipv6::SetMetric(uint32_t interface, uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'interface'), param('uint16_t', 'metric')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: void ns3::Ipv6::SetRoutingProtocol(ns3::Ptr<ns3::Ipv6RoutingProtocol> routingProtocol) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6RoutingProtocol >', 'routingProtocol')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: void ns3::Ipv6::SetUp(uint32_t interface) [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6.h: ns3::Ipv6::IF_ANY [variable]
    cls.add_static_attribute('IF_ANY', 'uint32_t const', is_const=True)
    ## ipv6.h: bool ns3::Ipv6::GetIpForward() const [member function]
    cls.add_method('GetIpForward', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## ipv6.h: void ns3::Ipv6::SetIpForward(bool forward) [member function]
    cls.add_method('SetIpForward', 
                   'void', 
                   [param('bool', 'forward')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3Ipv6AddressChecker_methods(root_module, cls):
    ## ipv6-address.h: ns3::Ipv6AddressChecker::Ipv6AddressChecker() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6AddressChecker::Ipv6AddressChecker(ns3::Ipv6AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6AddressChecker const &', 'arg0')])
    return

def register_Ns3Ipv6AddressValue_methods(root_module, cls):
    ## ipv6-address.h: ns3::Ipv6AddressValue::Ipv6AddressValue() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6AddressValue::Ipv6AddressValue(ns3::Ipv6AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6AddressValue const &', 'arg0')])
    ## ipv6-address.h: ns3::Ipv6AddressValue::Ipv6AddressValue(ns3::Ipv6Address const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address const &', 'value')])
    ## ipv6-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Ipv6AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h: bool ns3::Ipv6AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv6-address.h: ns3::Ipv6Address ns3::Ipv6AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: std::string ns3::Ipv6AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h: void ns3::Ipv6AddressValue::Set(ns3::Ipv6Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv6Address const &', 'value')])
    return

def register_Ns3Ipv6MulticastRoute_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv6-route.h: ns3::Ipv6MulticastRoute::Ipv6MulticastRoute(ns3::Ipv6MulticastRoute const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6MulticastRoute const &', 'arg0')])
    ## ipv6-route.h: ns3::Ipv6MulticastRoute::Ipv6MulticastRoute() [constructor]
    cls.add_constructor([])
    ## ipv6-route.h: ns3::Ipv6Address ns3::Ipv6MulticastRoute::GetGroup() const [member function]
    cls.add_method('GetGroup', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-route.h: ns3::Ipv6Address ns3::Ipv6MulticastRoute::GetOrigin() const [member function]
    cls.add_method('GetOrigin', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-route.h: uint32_t ns3::Ipv6MulticastRoute::GetOutputTtl(uint32_t oif) const [member function]
    cls.add_method('GetOutputTtl', 
                   'uint32_t', 
                   [param('uint32_t', 'oif')], 
                   is_const=True)
    ## ipv6-route.h: uint32_t ns3::Ipv6MulticastRoute::GetParent() const [member function]
    cls.add_method('GetParent', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-route.h: void ns3::Ipv6MulticastRoute::SetGroup(ns3::Ipv6Address const group) [member function]
    cls.add_method('SetGroup', 
                   'void', 
                   [param('ns3::Ipv6Address const', 'group')])
    ## ipv6-route.h: void ns3::Ipv6MulticastRoute::SetOrigin(ns3::Ipv6Address const origin) [member function]
    cls.add_method('SetOrigin', 
                   'void', 
                   [param('ns3::Ipv6Address const', 'origin')])
    ## ipv6-route.h: void ns3::Ipv6MulticastRoute::SetOutputTtl(uint32_t oif, uint32_t ttl) [member function]
    cls.add_method('SetOutputTtl', 
                   'void', 
                   [param('uint32_t', 'oif'), param('uint32_t', 'ttl')])
    ## ipv6-route.h: void ns3::Ipv6MulticastRoute::SetParent(uint32_t iif) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('uint32_t', 'iif')])
    ## ipv6-route.h: ns3::Ipv6MulticastRoute::MAX_INTERFACES [variable]
    cls.add_static_attribute('MAX_INTERFACES', 'uint32_t const', is_const=True)
    ## ipv6-route.h: ns3::Ipv6MulticastRoute::MAX_TTL [variable]
    cls.add_static_attribute('MAX_TTL', 'uint32_t const', is_const=True)
    return

def register_Ns3Ipv6PrefixChecker_methods(root_module, cls):
    ## ipv6-address.h: ns3::Ipv6PrefixChecker::Ipv6PrefixChecker() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6PrefixChecker::Ipv6PrefixChecker(ns3::Ipv6PrefixChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PrefixChecker const &', 'arg0')])
    return

def register_Ns3Ipv6PrefixValue_methods(root_module, cls):
    ## ipv6-address.h: ns3::Ipv6PrefixValue::Ipv6PrefixValue() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6PrefixValue::Ipv6PrefixValue(ns3::Ipv6PrefixValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PrefixValue const &', 'arg0')])
    ## ipv6-address.h: ns3::Ipv6PrefixValue::Ipv6PrefixValue(ns3::Ipv6Prefix const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const &', 'value')])
    ## ipv6-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Ipv6PrefixValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h: bool ns3::Ipv6PrefixValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv6-address.h: ns3::Ipv6Prefix ns3::Ipv6PrefixValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: std::string ns3::Ipv6PrefixValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h: void ns3::Ipv6PrefixValue::Set(ns3::Ipv6Prefix const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv6Prefix const &', 'value')])
    return

def register_Ns3Ipv6RawSocketFactory_methods(root_module, cls):
    ## ipv6-raw-socket-factory.h: ns3::Ipv6RawSocketFactory::Ipv6RawSocketFactory() [constructor]
    cls.add_constructor([])
    ## ipv6-raw-socket-factory.h: ns3::Ipv6RawSocketFactory::Ipv6RawSocketFactory(ns3::Ipv6RawSocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6RawSocketFactory const &', 'arg0')])
    ## ipv6-raw-socket-factory.h: static ns3::TypeId ns3::Ipv6RawSocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3Ipv6Route_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv6-route.h: ns3::Ipv6Route::Ipv6Route(ns3::Ipv6Route const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Route const &', 'arg0')])
    ## ipv6-route.h: ns3::Ipv6Route::Ipv6Route() [constructor]
    cls.add_constructor([])
    ## ipv6-route.h: ns3::Ipv6Address ns3::Ipv6Route::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-route.h: ns3::Ipv6Address ns3::Ipv6Route::GetGateway() const [member function]
    cls.add_method('GetGateway', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-route.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv6Route::GetOutputDevice() const [member function]
    cls.add_method('GetOutputDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## ipv6-route.h: ns3::Ipv6Address ns3::Ipv6Route::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-route.h: void ns3::Ipv6Route::SetDestination(ns3::Ipv6Address dest) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dest')])
    ## ipv6-route.h: void ns3::Ipv6Route::SetGateway(ns3::Ipv6Address gw) [member function]
    cls.add_method('SetGateway', 
                   'void', 
                   [param('ns3::Ipv6Address', 'gw')])
    ## ipv6-route.h: void ns3::Ipv6Route::SetOutputDevice(ns3::Ptr<ns3::NetDevice> outputDevice) [member function]
    cls.add_method('SetOutputDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'outputDevice')])
    ## ipv6-route.h: void ns3::Ipv6Route::SetSource(ns3::Ipv6Address src) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Ipv6Address', 'src')])
    return

def register_Ns3Ipv6RoutingProtocol_methods(root_module, cls):
    ## ipv6-routing-protocol.h: ns3::Ipv6RoutingProtocol::Ipv6RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## ipv6-routing-protocol.h: ns3::Ipv6RoutingProtocol::Ipv6RoutingProtocol(ns3::Ipv6RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6RoutingProtocol const &', 'arg0')])
    ## ipv6-routing-protocol.h: static ns3::TypeId ns3::Ipv6RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-routing-protocol.h: void ns3::Ipv6RoutingProtocol::NotifyAddAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-routing-protocol.h: void ns3::Ipv6RoutingProtocol::NotifyAddRoute(ns3::Ipv6Address dst, ns3::Ipv6Prefix mask, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('NotifyAddRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address::GetZero( )')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-routing-protocol.h: void ns3::Ipv6RoutingProtocol::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-routing-protocol.h: void ns3::Ipv6RoutingProtocol::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-routing-protocol.h: void ns3::Ipv6RoutingProtocol::NotifyRemoveAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-routing-protocol.h: void ns3::Ipv6RoutingProtocol::NotifyRemoveRoute(ns3::Ipv6Address dst, ns3::Ipv6Prefix mask, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('NotifyRemoveRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address::GetZero( )')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-routing-protocol.h: bool ns3::Ipv6RoutingProtocol::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv6Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void,ns3::Ptr<ns3::Ipv6Route>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv6MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::Socket::SocketErrno,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv6Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv6Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv6MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-routing-protocol.h: ns3::Ptr<ns3::Ipv6Route> ns3::Ipv6RoutingProtocol::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv6Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-routing-protocol.h: void ns3::Ipv6RoutingProtocol::SetIpv6(ns3::Ptr<ns3::Ipv6> ipv6) [member function]
    cls.add_method('SetIpv6', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6 >', 'ipv6')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3Mac48AddressChecker_methods(root_module, cls):
    ## mac48-address.h: ns3::Mac48AddressChecker::Mac48AddressChecker() [constructor]
    cls.add_constructor([])
    ## mac48-address.h: ns3::Mac48AddressChecker::Mac48AddressChecker(ns3::Mac48AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48AddressChecker const &', 'arg0')])
    return

def register_Ns3Mac48AddressValue_methods(root_module, cls):
    ## mac48-address.h: ns3::Mac48AddressValue::Mac48AddressValue() [constructor]
    cls.add_constructor([])
    ## mac48-address.h: ns3::Mac48AddressValue::Mac48AddressValue(ns3::Mac48AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48AddressValue const &', 'arg0')])
    ## mac48-address.h: ns3::Mac48AddressValue::Mac48AddressValue(ns3::Mac48Address const & value) [constructor]
    cls.add_constructor([param('ns3::Mac48Address const &', 'value')])
    ## mac48-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Mac48AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac48-address.h: bool ns3::Mac48AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## mac48-address.h: ns3::Mac48Address ns3::Mac48AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mac48-address.h: std::string ns3::Mac48AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## mac48-address.h: void ns3::Mac48AddressValue::Set(ns3::Mac48Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Mac48Address const &', 'value')])
    return

def register_Ns3NetDevice_methods(root_module, cls):
    ## net-device.h: ns3::NetDevice::NetDevice() [constructor]
    cls.add_constructor([])
    ## net-device.h: ns3::NetDevice::NetDevice(ns3::NetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDevice const &', 'arg0')])
    ## net-device.h: void ns3::NetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: ns3::Address ns3::NetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Address ns3::NetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Ptr<ns3::Channel> ns3::NetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: uint32_t ns3::NetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: uint16_t ns3::NetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Address ns3::NetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Address ns3::NetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Ptr<ns3::Node> ns3::NetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: static ns3::TypeId ns3::NetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## net-device.h: bool ns3::NetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3Node_methods(root_module, cls):
    ## node.h: ns3::Node::Node(ns3::Node const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Node const &', 'arg0')])
    ## node.h: ns3::Node::Node() [constructor]
    cls.add_constructor([])
    ## node.h: ns3::Node::Node(uint32_t systemId) [constructor]
    cls.add_constructor([param('uint32_t', 'systemId')])
    ## node.h: uint32_t ns3::Node::AddApplication(ns3::Ptr<ns3::Application> application) [member function]
    cls.add_method('AddApplication', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Application >', 'application')])
    ## node.h: uint32_t ns3::Node::AddDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddDevice', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## node.h: static bool ns3::Node::ChecksumEnabled() [member function]
    cls.add_method('ChecksumEnabled', 
                   'bool', 
                   [], 
                   is_static=True)
    ## node.h: ns3::Ptr<ns3::Application> ns3::Node::GetApplication(uint32_t index) const [member function]
    cls.add_method('GetApplication', 
                   'ns3::Ptr< ns3::Application >', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## node.h: ns3::Ptr<ns3::NetDevice> ns3::Node::GetDevice(uint32_t index) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## node.h: uint32_t ns3::Node::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h: uint32_t ns3::Node::GetNApplications() const [member function]
    cls.add_method('GetNApplications', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h: uint32_t ns3::Node::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h: uint32_t ns3::Node::GetSystemId() const [member function]
    cls.add_method('GetSystemId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h: static ns3::TypeId ns3::Node::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## node.h: void ns3::Node::RegisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler, uint16_t protocolType, ns3::Ptr<ns3::NetDevice> device, bool promiscuous=false) [member function]
    cls.add_method('RegisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler'), param('uint16_t', 'protocolType'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'promiscuous', default_value='false')])
    ## node.h: void ns3::Node::UnregisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler) [member function]
    cls.add_method('UnregisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler')])
    ## node.h: void ns3::Node::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## node.h: void ns3::Node::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## node.h: void ns3::Node::NotifyDeviceAdded(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('NotifyDeviceAdded', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PacketSocketFactory_methods(root_module, cls):
    ## packet-socket-factory.h: ns3::PacketSocketFactory::PacketSocketFactory(ns3::PacketSocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSocketFactory const &', 'arg0')])
    ## packet-socket-factory.h: ns3::PacketSocketFactory::PacketSocketFactory() [constructor]
    cls.add_constructor([])
    ## packet-socket-factory.h: ns3::Ptr<ns3::Socket> ns3::PacketSocketFactory::CreateSocket() [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_virtual=True)
    ## packet-socket-factory.h: static ns3::TypeId ns3::PacketSocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3PbbAddressBlock_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbAddressBlock::PbbAddressBlock(ns3::PbbAddressBlock const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressBlock const &', 'arg0')])
    ## packetbb.h: ns3::PbbAddressBlock::PbbAddressBlock() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Address ns3::PbbAddressBlock::AddressBack() const [member function]
    cls.add_method('AddressBack', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressBegin() [member function]
    cls.add_method('AddressBegin', 
                   'std::_List_iterator< ns3::Address >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Address> ns3::PbbAddressBlock::AddressBegin() const [member function]
    cls.add_method('AddressBegin', 
                   'std::_List_const_iterator< ns3::Address >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::AddressClear() [member function]
    cls.add_method('AddressClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbAddressBlock::AddressEmpty() const [member function]
    cls.add_method('AddressEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressEnd() [member function]
    cls.add_method('AddressEnd', 
                   'std::_List_iterator< ns3::Address >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Address> ns3::PbbAddressBlock::AddressEnd() const [member function]
    cls.add_method('AddressEnd', 
                   'std::_List_const_iterator< ns3::Address >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressErase(std::_List_iterator<ns3::Address> position) [member function]
    cls.add_method('AddressErase', 
                   'std::_List_iterator< ns3::Address >', 
                   [param('std::_List_iterator< ns3::Address >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressErase(std::_List_iterator<ns3::Address> first, std::_List_iterator<ns3::Address> last) [member function]
    cls.add_method('AddressErase', 
                   'std::_List_iterator< ns3::Address >', 
                   [param('std::_List_iterator< ns3::Address >', 'first'), param('std::_List_iterator< ns3::Address >', 'last')])
    ## packetbb.h: ns3::Address ns3::PbbAddressBlock::AddressFront() const [member function]
    cls.add_method('AddressFront', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressInsert(std::_List_iterator<ns3::Address> position, ns3::Address const value) [member function]
    cls.add_method('AddressInsert', 
                   'std::_List_iterator< ns3::Address >', 
                   [param('std::_List_iterator< ns3::Address >', 'position'), param('ns3::Address const', 'value')])
    ## packetbb.h: void ns3::PbbAddressBlock::AddressPopBack() [member function]
    cls.add_method('AddressPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::AddressPopFront() [member function]
    cls.add_method('AddressPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::AddressPushBack(ns3::Address address) [member function]
    cls.add_method('AddressPushBack', 
                   'void', 
                   [param('ns3::Address', 'address')])
    ## packetbb.h: void ns3::PbbAddressBlock::AddressPushFront(ns3::Address address) [member function]
    cls.add_method('AddressPushFront', 
                   'void', 
                   [param('ns3::Address', 'address')])
    ## packetbb.h: int ns3::PbbAddressBlock::AddressSize() const [member function]
    cls.add_method('AddressSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: uint32_t ns3::PbbAddressBlock::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbAddressBlock::PrefixBack() const [member function]
    cls.add_method('PrefixBack', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixBegin() [member function]
    cls.add_method('PrefixBegin', 
                   'std::_List_iterator< unsigned char >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<unsigned char> ns3::PbbAddressBlock::PrefixBegin() const [member function]
    cls.add_method('PrefixBegin', 
                   'std::_List_const_iterator< unsigned char >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixClear() [member function]
    cls.add_method('PrefixClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbAddressBlock::PrefixEmpty() const [member function]
    cls.add_method('PrefixEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixEnd() [member function]
    cls.add_method('PrefixEnd', 
                   'std::_List_iterator< unsigned char >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<unsigned char> ns3::PbbAddressBlock::PrefixEnd() const [member function]
    cls.add_method('PrefixEnd', 
                   'std::_List_const_iterator< unsigned char >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixErase(std::_List_iterator<unsigned char> position) [member function]
    cls.add_method('PrefixErase', 
                   'std::_List_iterator< unsigned char >', 
                   [param('std::_List_iterator< unsigned char >', 'position')])
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixErase(std::_List_iterator<unsigned char> first, std::_List_iterator<unsigned char> last) [member function]
    cls.add_method('PrefixErase', 
                   'std::_List_iterator< unsigned char >', 
                   [param('std::_List_iterator< unsigned char >', 'first'), param('std::_List_iterator< unsigned char >', 'last')])
    ## packetbb.h: uint8_t ns3::PbbAddressBlock::PrefixFront() const [member function]
    cls.add_method('PrefixFront', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixInsert(std::_List_iterator<unsigned char> position, uint8_t const value) [member function]
    cls.add_method('PrefixInsert', 
                   'std::_List_iterator< unsigned char >', 
                   [param('std::_List_iterator< unsigned char >', 'position'), param('uint8_t const', 'value')])
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixPopBack() [member function]
    cls.add_method('PrefixPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixPopFront() [member function]
    cls.add_method('PrefixPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixPushBack(uint8_t prefix) [member function]
    cls.add_method('PrefixPushBack', 
                   'void', 
                   [param('uint8_t', 'prefix')])
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixPushFront(uint8_t prefix) [member function]
    cls.add_method('PrefixPushFront', 
                   'void', 
                   [param('uint8_t', 'prefix')])
    ## packetbb.h: int ns3::PbbAddressBlock::PrefixSize() const [member function]
    cls.add_method('PrefixSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> ns3::PbbAddressBlock::TlvBack() [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbAddressTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> const ns3::PbbAddressBlock::TlvBack() const [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbAddressTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvBegin() [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvBegin() const [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::TlvClear() [member function]
    cls.add_method('TlvClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbAddressBlock::TlvEmpty() const [member function]
    cls.add_method('TlvEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvEnd() [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvEnd() const [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvErase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > position) [member function]
    cls.add_method('TlvErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvErase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > last) [member function]
    cls.add_method('TlvErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> ns3::PbbAddressBlock::TlvFront() [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbAddressTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> const ns3::PbbAddressBlock::TlvFront() const [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbAddressTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvInsert(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > position, ns3::Ptr<ns3::PbbTlv> const value) [member function]
    cls.add_method('TlvInsert', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'position'), param('ns3::Ptr< ns3::PbbTlv > const', 'value')])
    ## packetbb.h: void ns3::PbbAddressBlock::TlvPopBack() [member function]
    cls.add_method('TlvPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::TlvPopFront() [member function]
    cls.add_method('TlvPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::TlvPushBack(ns3::Ptr<ns3::PbbAddressTlv> address) [member function]
    cls.add_method('TlvPushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressTlv >', 'address')])
    ## packetbb.h: void ns3::PbbAddressBlock::TlvPushFront(ns3::Ptr<ns3::PbbAddressTlv> address) [member function]
    cls.add_method('TlvPushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressTlv >', 'address')])
    ## packetbb.h: int ns3::PbbAddressBlock::TlvSize() const [member function]
    cls.add_method('TlvSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Address ns3::PbbAddressBlock::DeserializeAddress(uint8_t * buffer) const [member function]
    cls.add_method('DeserializeAddress', 
                   'ns3::Address', 
                   [param('uint8_t *', 'buffer')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: uint8_t ns3::PbbAddressBlock::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlock::PrintAddress(std::ostream & os, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('PrintAddress', 
                   'void', 
                   [param('std::ostream &', 'os'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlock::SerializeAddress(uint8_t * buffer, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('SerializeAddress', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbAddressBlockIpv4_methods(root_module, cls):
    ## packetbb.h: ns3::PbbAddressBlockIpv4::PbbAddressBlockIpv4(ns3::PbbAddressBlockIpv4 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressBlockIpv4 const &', 'arg0')])
    ## packetbb.h: ns3::PbbAddressBlockIpv4::PbbAddressBlockIpv4() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Address ns3::PbbAddressBlockIpv4::DeserializeAddress(uint8_t * buffer) const [member function]
    cls.add_method('DeserializeAddress', 
                   'ns3::Address', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: uint8_t ns3::PbbAddressBlockIpv4::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlockIpv4::PrintAddress(std::ostream & os, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('PrintAddress', 
                   'void', 
                   [param('std::ostream &', 'os'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlockIpv4::SerializeAddress(uint8_t * buffer, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('SerializeAddress', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbAddressBlockIpv6_methods(root_module, cls):
    ## packetbb.h: ns3::PbbAddressBlockIpv6::PbbAddressBlockIpv6(ns3::PbbAddressBlockIpv6 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressBlockIpv6 const &', 'arg0')])
    ## packetbb.h: ns3::PbbAddressBlockIpv6::PbbAddressBlockIpv6() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Address ns3::PbbAddressBlockIpv6::DeserializeAddress(uint8_t * buffer) const [member function]
    cls.add_method('DeserializeAddress', 
                   'ns3::Address', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: uint8_t ns3::PbbAddressBlockIpv6::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlockIpv6::PrintAddress(std::ostream & os, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('PrintAddress', 
                   'void', 
                   [param('std::ostream &', 'os'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlockIpv6::SerializeAddress(uint8_t * buffer, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('SerializeAddress', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbMessage_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbMessage::PbbMessage(ns3::PbbMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbMessage const &', 'arg0')])
    ## packetbb.h: ns3::PbbMessage::PbbMessage() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessage::AddressBlockBack() [member function]
    cls.add_method('AddressBlockBack', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> const ns3::PbbMessage::AddressBlockBack() const [member function]
    cls.add_method('AddressBlockBack', 
                   'ns3::Ptr< ns3::PbbAddressBlock > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockBegin() [member function]
    cls.add_method('AddressBlockBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockBegin() const [member function]
    cls.add_method('AddressBlockBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::AddressBlockClear() [member function]
    cls.add_method('AddressBlockClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbMessage::AddressBlockEmpty() const [member function]
    cls.add_method('AddressBlockEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockEnd() [member function]
    cls.add_method('AddressBlockEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockEnd() const [member function]
    cls.add_method('AddressBlockEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockErase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > position) [member function]
    cls.add_method('AddressBlockErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockErase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > first, std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > last) [member function]
    cls.add_method('AddressBlockErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessage::AddressBlockFront() [member function]
    cls.add_method('AddressBlockFront', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> const ns3::PbbMessage::AddressBlockFront() const [member function]
    cls.add_method('AddressBlockFront', 
                   'ns3::Ptr< ns3::PbbAddressBlock > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::AddressBlockPopBack() [member function]
    cls.add_method('AddressBlockPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbMessage::AddressBlockPopFront() [member function]
    cls.add_method('AddressBlockPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbMessage::AddressBlockPushBack(ns3::Ptr<ns3::PbbAddressBlock> block) [member function]
    cls.add_method('AddressBlockPushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressBlock >', 'block')])
    ## packetbb.h: void ns3::PbbMessage::AddressBlockPushFront(ns3::Ptr<ns3::PbbAddressBlock> block) [member function]
    cls.add_method('AddressBlockPushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressBlock >', 'block')])
    ## packetbb.h: int ns3::PbbMessage::AddressBlockSize() const [member function]
    cls.add_method('AddressBlockSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: static ns3::Ptr<ns3::PbbMessage> ns3::PbbMessage::DeserializeMessage(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('DeserializeMessage', 
                   'ns3::Ptr< ns3::PbbMessage >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_static=True)
    ## packetbb.h: uint8_t ns3::PbbMessage::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbMessage::GetHopLimit() const [member function]
    cls.add_method('GetHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Address ns3::PbbMessage::GetOriginatorAddress() const [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint16_t ns3::PbbMessage::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint32_t ns3::PbbMessage::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbMessage::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbMessage::HasHopCount() const [member function]
    cls.add_method('HasHopCount', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbMessage::HasHopLimit() const [member function]
    cls.add_method('HasHopLimit', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbMessage::HasOriginatorAddress() const [member function]
    cls.add_method('HasOriginatorAddress', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbMessage::HasSequenceNumber() const [member function]
    cls.add_method('HasSequenceNumber', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::SetHopCount(uint8_t hopcount) [member function]
    cls.add_method('SetHopCount', 
                   'void', 
                   [param('uint8_t', 'hopcount')])
    ## packetbb.h: void ns3::PbbMessage::SetHopLimit(uint8_t hoplimit) [member function]
    cls.add_method('SetHopLimit', 
                   'void', 
                   [param('uint8_t', 'hoplimit')])
    ## packetbb.h: void ns3::PbbMessage::SetOriginatorAddress(ns3::Address address) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Address', 'address')])
    ## packetbb.h: void ns3::PbbMessage::SetSequenceNumber(uint16_t seqnum) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'seqnum')])
    ## packetbb.h: void ns3::PbbMessage::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbMessage::TlvBack() [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> const ns3::PbbMessage::TlvBack() const [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvBegin() [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvBegin() const [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::TlvClear() [member function]
    cls.add_method('TlvClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbMessage::TlvEmpty() const [member function]
    cls.add_method('TlvEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvEnd() [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvEnd() const [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvErase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > position) [member function]
    cls.add_method('TlvErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvErase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > last) [member function]
    cls.add_method('TlvErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbMessage::TlvFront() [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> const ns3::PbbMessage::TlvFront() const [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::TlvPopBack() [member function]
    cls.add_method('TlvPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbMessage::TlvPopFront() [member function]
    cls.add_method('TlvPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbMessage::TlvPushBack(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('TlvPushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbMessage::TlvPushFront(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('TlvPushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: int ns3::PbbMessage::TlvSize() const [member function]
    cls.add_method('TlvSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessage::AddressBlockDeserialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('AddressBlockDeserialize', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::Address ns3::PbbMessage::DeserializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('DeserializeOriginatorAddress', 
                   'ns3::Address', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::PbbAddressLength ns3::PbbMessage::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'ns3::PbbAddressLength', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessage::PrintOriginatorAddress(std::ostream & os) const [member function]
    cls.add_method('PrintOriginatorAddress', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessage::SerializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('SerializeOriginatorAddress', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbMessageIpv4_methods(root_module, cls):
    ## packetbb.h: ns3::PbbMessageIpv4::PbbMessageIpv4(ns3::PbbMessageIpv4 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbMessageIpv4 const &', 'arg0')])
    ## packetbb.h: ns3::PbbMessageIpv4::PbbMessageIpv4() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessageIpv4::AddressBlockDeserialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('AddressBlockDeserialize', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::Address ns3::PbbMessageIpv4::DeserializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('DeserializeOriginatorAddress', 
                   'ns3::Address', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::PbbAddressLength ns3::PbbMessageIpv4::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'ns3::PbbAddressLength', 
                   [], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessageIpv4::PrintOriginatorAddress(std::ostream & os) const [member function]
    cls.add_method('PrintOriginatorAddress', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessageIpv4::SerializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('SerializeOriginatorAddress', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbMessageIpv6_methods(root_module, cls):
    ## packetbb.h: ns3::PbbMessageIpv6::PbbMessageIpv6(ns3::PbbMessageIpv6 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbMessageIpv6 const &', 'arg0')])
    ## packetbb.h: ns3::PbbMessageIpv6::PbbMessageIpv6() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessageIpv6::AddressBlockDeserialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('AddressBlockDeserialize', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::Address ns3::PbbMessageIpv6::DeserializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('DeserializeOriginatorAddress', 
                   'ns3::Address', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::PbbAddressLength ns3::PbbMessageIpv6::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'ns3::PbbAddressLength', 
                   [], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessageIpv6::PrintOriginatorAddress(std::ostream & os) const [member function]
    cls.add_method('PrintOriginatorAddress', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessageIpv6::SerializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('SerializeOriginatorAddress', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbPacket_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbPacket::PbbPacket(ns3::PbbPacket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbPacket const &', 'arg0')])
    ## packetbb.h: ns3::PbbPacket::PbbPacket() [constructor]
    cls.add_constructor([])
    ## packetbb.h: uint32_t ns3::PbbPacket::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > position) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > last) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'last')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > position) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > first, std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > last) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 'last')])
    ## packetbb.h: ns3::TypeId ns3::PbbPacket::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packetbb.h: uint16_t ns3::PbbPacket::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint32_t ns3::PbbPacket::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packetbb.h: static ns3::TypeId ns3::PbbPacket::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## packetbb.h: uint8_t ns3::PbbPacket::GetVersion() const [member function]
    cls.add_method('GetVersion', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbPacket::HasSequenceNumber() const [member function]
    cls.add_method('HasSequenceNumber', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbMessage> ns3::PbbPacket::MessageBack() [member function]
    cls.add_method('MessageBack', 
                   'ns3::Ptr< ns3::PbbMessage >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbMessage> const ns3::PbbPacket::MessageBack() const [member function]
    cls.add_method('MessageBack', 
                   'ns3::Ptr< ns3::PbbMessage > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::MessageBegin() [member function]
    cls.add_method('MessageBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::MessageBegin() const [member function]
    cls.add_method('MessageBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::MessageClear() [member function]
    cls.add_method('MessageClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbPacket::MessageEmpty() const [member function]
    cls.add_method('MessageEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::MessageEnd() [member function]
    cls.add_method('MessageEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::MessageEnd() const [member function]
    cls.add_method('MessageEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbMessage> ns3::PbbPacket::MessageFront() [member function]
    cls.add_method('MessageFront', 
                   'ns3::Ptr< ns3::PbbMessage >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbMessage> const ns3::PbbPacket::MessageFront() const [member function]
    cls.add_method('MessageFront', 
                   'ns3::Ptr< ns3::PbbMessage > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::MessagePopBack() [member function]
    cls.add_method('MessagePopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbPacket::MessagePopFront() [member function]
    cls.add_method('MessagePopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbPacket::MessagePushBack(ns3::Ptr<ns3::PbbMessage> message) [member function]
    cls.add_method('MessagePushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbMessage >', 'message')])
    ## packetbb.h: void ns3::PbbPacket::MessagePushFront(ns3::Ptr<ns3::PbbMessage> message) [member function]
    cls.add_method('MessagePushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbMessage >', 'message')])
    ## packetbb.h: int ns3::PbbPacket::MessageSize() const [member function]
    cls.add_method('MessageSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## packetbb.h: void ns3::PbbPacket::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## packetbb.h: void ns3::PbbPacket::SetSequenceNumber(uint16_t number) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'number')])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbPacket::TlvBack() [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> const ns3::PbbPacket::TlvBack() const [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::TlvBegin() [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::TlvBegin() const [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::TlvClear() [member function]
    cls.add_method('TlvClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbPacket::TlvEmpty() const [member function]
    cls.add_method('TlvEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::TlvEnd() [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::TlvEnd() const [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbPacket::TlvFront() [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> const ns3::PbbPacket::TlvFront() const [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::TlvPopBack() [member function]
    cls.add_method('TlvPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbPacket::TlvPopFront() [member function]
    cls.add_method('TlvPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbPacket::TlvPushBack(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('TlvPushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbPacket::TlvPushFront(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('TlvPushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: int ns3::PbbPacket::TlvSize() const [member function]
    cls.add_method('TlvSize', 
                   'int', 
                   [], 
                   is_const=True)
    return

def register_Ns3PbbTlv_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbTlv::PbbTlv(ns3::PbbTlv const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbTlv const &', 'arg0')])
    ## packetbb.h: ns3::PbbTlv::PbbTlv() [constructor]
    cls.add_constructor([])
    ## packetbb.h: void ns3::PbbTlv::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: uint32_t ns3::PbbTlv::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbTlv::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbTlv::GetTypeExt() const [member function]
    cls.add_method('GetTypeExt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Buffer ns3::PbbTlv::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'ns3::Buffer', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbTlv::HasTypeExt() const [member function]
    cls.add_method('HasTypeExt', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbTlv::HasValue() const [member function]
    cls.add_method('HasValue', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlv::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlv::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlv::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlv::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## packetbb.h: void ns3::PbbTlv::SetTypeExt(uint8_t type) [member function]
    cls.add_method('SetTypeExt', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## packetbb.h: void ns3::PbbTlv::SetValue(ns3::Buffer start) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('ns3::Buffer', 'start')])
    ## packetbb.h: void ns3::PbbTlv::SetValue(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## packetbb.h: uint8_t ns3::PbbTlv::GetIndexStart() const [member function]
    cls.add_method('GetIndexStart', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: uint8_t ns3::PbbTlv::GetIndexStop() const [member function]
    cls.add_method('GetIndexStop', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: bool ns3::PbbTlv::HasIndexStart() const [member function]
    cls.add_method('HasIndexStart', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: bool ns3::PbbTlv::HasIndexStop() const [member function]
    cls.add_method('HasIndexStop', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: bool ns3::PbbTlv::IsMultivalue() const [member function]
    cls.add_method('IsMultivalue', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: void ns3::PbbTlv::SetIndexStart(uint8_t index) [member function]
    cls.add_method('SetIndexStart', 
                   'void', 
                   [param('uint8_t', 'index')], 
                   visibility='protected')
    ## packetbb.h: void ns3::PbbTlv::SetIndexStop(uint8_t index) [member function]
    cls.add_method('SetIndexStop', 
                   'void', 
                   [param('uint8_t', 'index')], 
                   visibility='protected')
    ## packetbb.h: void ns3::PbbTlv::SetMultivalue(bool isMultivalue) [member function]
    cls.add_method('SetMultivalue', 
                   'void', 
                   [param('bool', 'isMultivalue')], 
                   visibility='protected')
    return

def register_Ns3SimpleChannel_methods(root_module, cls):
    ## simple-channel.h: ns3::SimpleChannel::SimpleChannel(ns3::SimpleChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimpleChannel const &', 'arg0')])
    ## simple-channel.h: ns3::SimpleChannel::SimpleChannel() [constructor]
    cls.add_constructor([])
    ## simple-channel.h: void ns3::SimpleChannel::Add(ns3::Ptr<ns3::SimpleNetDevice> device) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::SimpleNetDevice >', 'device')])
    ## simple-channel.h: ns3::Ptr<ns3::NetDevice> ns3::SimpleChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## simple-channel.h: uint32_t ns3::SimpleChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-channel.h: static ns3::TypeId ns3::SimpleChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## simple-channel.h: void ns3::SimpleChannel::Send(ns3::Ptr<ns3::Packet> p, uint16_t protocol, ns3::Mac48Address to, ns3::Mac48Address from, ns3::Ptr<ns3::SimpleNetDevice> sender) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint16_t', 'protocol'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from'), param('ns3::Ptr< ns3::SimpleNetDevice >', 'sender')])
    return

def register_Ns3SimpleNetDevice_methods(root_module, cls):
    ## simple-net-device.h: ns3::SimpleNetDevice::SimpleNetDevice(ns3::SimpleNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimpleNetDevice const &', 'arg0')])
    ## simple-net-device.h: ns3::SimpleNetDevice::SimpleNetDevice() [constructor]
    cls.add_constructor([])
    ## simple-net-device.h: void ns3::SimpleNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## simple-net-device.h: ns3::Address ns3::SimpleNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Address ns3::SimpleNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Ptr<ns3::Channel> ns3::SimpleNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: uint32_t ns3::SimpleNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: uint16_t ns3::SimpleNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Address ns3::SimpleNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Address ns3::SimpleNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Ptr<ns3::Node> ns3::SimpleNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: static ns3::TypeId ns3::SimpleNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::Receive(ns3::Ptr<ns3::Packet> packet, uint16_t protocol, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t', 'protocol'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')])
    ## simple-net-device.h: bool ns3::SimpleNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetChannel(ns3::Ptr<ns3::SimpleChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SimpleChannel >', 'channel')])
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetReceiveErrorModel(ns3::Ptr<ns3::ErrorModel> em) [member function]
    cls.add_method('SetReceiveErrorModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorModel >', 'em')])
    ## simple-net-device.h: bool ns3::SimpleNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3SpectrumChannel_methods(root_module, cls):
    ## spectrum-channel.h: ns3::SpectrumChannel::SpectrumChannel() [constructor]
    cls.add_constructor([])
    ## spectrum-channel.h: ns3::SpectrumChannel::SpectrumChannel(ns3::SpectrumChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumChannel const &', 'arg0')])
    ## spectrum-channel.h: void ns3::SpectrumChannel::AddRx(ns3::Ptr<ns3::SpectrumPhy> phy) [member function]
    cls.add_method('AddRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h: void ns3::SpectrumChannel::AddSpectrumPropagationLossModel(ns3::Ptr<ns3::SpectrumPropagationLossModel> loss) [member function]
    cls.add_method('AddSpectrumPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPropagationLossModel >', 'loss')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h: static ns3::TypeId ns3::SpectrumChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-channel.h: void ns3::SpectrumChannel::SetPropagationDelayModel(ns3::Ptr<ns3::PropagationDelayModel> delay) [member function]
    cls.add_method('SetPropagationDelayModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationDelayModel >', 'delay')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h: void ns3::SpectrumChannel::StartTx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> txPsd, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> sender) [member function]
    cls.add_method('StartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'sender')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3AddressChecker_methods(root_module, cls):
    ## address.h: ns3::AddressChecker::AddressChecker() [constructor]
    cls.add_constructor([])
    ## address.h: ns3::AddressChecker::AddressChecker(ns3::AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AddressChecker const &', 'arg0')])
    return

def register_Ns3AddressValue_methods(root_module, cls):
    ## address.h: ns3::AddressValue::AddressValue() [constructor]
    cls.add_constructor([])
    ## address.h: ns3::AddressValue::AddressValue(ns3::AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AddressValue const &', 'arg0')])
    ## address.h: ns3::AddressValue::AddressValue(ns3::Address const & value) [constructor]
    cls.add_constructor([param('ns3::Address const &', 'value')])
    ## address.h: ns3::Ptr<ns3::AttributeValue> ns3::AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## address.h: bool ns3::AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## address.h: ns3::Address ns3::AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## address.h: std::string ns3::AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## address.h: void ns3::AddressValue::Set(ns3::Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Address const &', 'value')])
    return

def register_Ns3PbbAddressTlv_methods(root_module, cls):
    ## packetbb.h: ns3::PbbAddressTlv::PbbAddressTlv() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::PbbAddressTlv::PbbAddressTlv(ns3::PbbAddressTlv const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressTlv const &', 'arg0')])
    ## packetbb.h: uint8_t ns3::PbbAddressTlv::GetIndexStart() const [member function]
    cls.add_method('GetIndexStart', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbAddressTlv::GetIndexStop() const [member function]
    cls.add_method('GetIndexStop', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbAddressTlv::HasIndexStart() const [member function]
    cls.add_method('HasIndexStart', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbAddressTlv::HasIndexStop() const [member function]
    cls.add_method('HasIndexStop', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbAddressTlv::IsMultivalue() const [member function]
    cls.add_method('IsMultivalue', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressTlv::SetIndexStart(uint8_t index) [member function]
    cls.add_method('SetIndexStart', 
                   'void', 
                   [param('uint8_t', 'index')])
    ## packetbb.h: void ns3::PbbAddressTlv::SetIndexStop(uint8_t index) [member function]
    cls.add_method('SetIndexStop', 
                   'void', 
                   [param('uint8_t', 'index')])
    ## packetbb.h: void ns3::PbbAddressTlv::SetMultivalue(bool isMultivalue) [member function]
    cls.add_method('SetMultivalue', 
                   'void', 
                   [param('bool', 'isMultivalue')])
    return

def register_functions(root_module):
    module = root_module
    ## address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeAddressChecker() [free function]
    module.add_function('MakeAddressChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ipv4-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeIpv4AddressChecker() [free function]
    module.add_function('MakeIpv4AddressChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ipv4-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeIpv4MaskChecker() [free function]
    module.add_function('MakeIpv4MaskChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ipv6-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeIpv6AddressChecker() [free function]
    module.add_function('MakeIpv6AddressChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ipv6-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeIpv6PrefixChecker() [free function]
    module.add_function('MakeIpv6PrefixChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## mac48-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeMac48AddressChecker() [free function]
    module.add_function('MakeMac48AddressChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## address-utils.h: extern void ns3::ReadFrom(ns3::Buffer::Iterator & i, ns3::Address & ad, uint32_t len) [free function]
    module.add_function('ReadFrom', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Address &', 'ad'), param('uint32_t', 'len')])
    ## address-utils.h: extern void ns3::ReadFrom(ns3::Buffer::Iterator & i, ns3::Ipv4Address & ad) [free function]
    module.add_function('ReadFrom', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Ipv4Address &', 'ad')])
    ## address-utils.h: extern void ns3::ReadFrom(ns3::Buffer::Iterator & i, ns3::Ipv6Address & ad) [free function]
    module.add_function('ReadFrom', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Ipv6Address &', 'ad')])
    ## address-utils.h: extern void ns3::ReadFrom(ns3::Buffer::Iterator & i, ns3::Mac48Address & ad) [free function]
    module.add_function('ReadFrom', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Mac48Address &', 'ad')])
    ## address-utils.h: extern void ns3::WriteTo(ns3::Buffer::Iterator & i, ns3::Address const & ad) [free function]
    module.add_function('WriteTo', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Address const &', 'ad')])
    ## address-utils.h: extern void ns3::WriteTo(ns3::Buffer::Iterator & i, ns3::Ipv4Address ad) [free function]
    module.add_function('WriteTo', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Ipv4Address', 'ad')])
    ## address-utils.h: extern void ns3::WriteTo(ns3::Buffer::Iterator & i, ns3::Ipv6Address ad) [free function]
    module.add_function('WriteTo', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Ipv6Address', 'ad')])
    ## address-utils.h: extern void ns3::WriteTo(ns3::Buffer::Iterator & i, ns3::Mac48Address ad) [free function]
    module.add_function('WriteTo', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Mac48Address', 'ad')])
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_aodv(module.get_submodule('aodv'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    ## address-utils.h: extern bool ns3::addressUtils::IsMulticast(ns3::Address const & ad) [free function]
    module.add_function('IsMulticast', 
                        'bool', 
                        [param('ns3::Address const &', 'ad')])
    return

def register_functions_ns3_aodv(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

