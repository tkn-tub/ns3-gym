from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## tcp-socket.h: ns3::TcpStates_t [enumeration]
    module.add_enum('TcpStates_t', ['CLOSED', 'LISTEN', 'SYN_SENT', 'SYN_RCVD', 'ESTABLISHED', 'CLOSE_WAIT', 'LAST_ACK', 'FIN_WAIT_1', 'FIN_WAIT_2', 'CLOSING', 'TIME_WAIT', 'LAST_STATE'])
    ## ipv4-address-generator.h: ns3::Ipv4AddressGenerator [class]
    module.add_class('Ipv4AddressGenerator')
    ## ipv4-interface-address.h: ns3::Ipv4InterfaceAddress [class]
    module.add_class('Ipv4InterfaceAddress')
    ## ipv4-interface-address.h: ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e [enumeration]
    module.add_enum('InterfaceAddressScope_e', ['HOST', 'LINK', 'GLOBAL'], outer_class=root_module['ns3::Ipv4InterfaceAddress'])
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress [class]
    module.add_class('Ipv6InterfaceAddress')
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::State_e [enumeration]
    module.add_enum('State_e', ['TENTATIVE', 'DEPRECATED', 'PREFERRED', 'PERMANENT', 'HOMEADDRESS', 'TENTATIVE_OPTIMISTIC', 'INVALID'], outer_class=root_module['ns3::Ipv6InterfaceAddress'])
    ## ipv6-interface-address.h: ns3::Ipv6InterfaceAddress::Scope_e [enumeration]
    module.add_enum('Scope_e', ['HOST', 'LINKLOCAL', 'GLOBAL'], outer_class=root_module['ns3::Ipv6InterfaceAddress'])
    ## ipv6-extension-header.h: ns3::OptionField [class]
    module.add_class('OptionField')
    ## icmpv4.h: ns3::Icmpv4DestinationUnreachable [class]
    module.add_class('Icmpv4DestinationUnreachable', parent=root_module['ns3::Header'])
    ## icmpv4.h: ns3::Icmpv4DestinationUnreachable [enumeration]
    module.add_enum('', ['NET_UNREACHABLE', 'HOST_UNREACHABLE', 'PROTOCOL_UNREACHABLE', 'PORT_UNREACHABLE', 'FRAG_NEEDED', 'SOURCE_ROUTE_FAILED'], outer_class=root_module['ns3::Icmpv4DestinationUnreachable'])
    ## icmpv4.h: ns3::Icmpv4Echo [class]
    module.add_class('Icmpv4Echo', parent=root_module['ns3::Header'])
    ## icmpv4.h: ns3::Icmpv4Header [class]
    module.add_class('Icmpv4Header', parent=root_module['ns3::Header'])
    ## icmpv4.h: ns3::Icmpv4Header [enumeration]
    module.add_enum('', ['ECHO_REPLY', 'DEST_UNREACH', 'ECHO', 'TIME_EXCEEDED'], outer_class=root_module['ns3::Icmpv4Header'])
    ## icmpv4.h: ns3::Icmpv4TimeExceeded [class]
    module.add_class('Icmpv4TimeExceeded', parent=root_module['ns3::Header'])
    ## icmpv4.h: ns3::Icmpv4TimeExceeded [enumeration]
    module.add_enum('', ['TIME_TO_LIVE', 'FRAGMENT_REASSEMBLY'], outer_class=root_module['ns3::Icmpv4TimeExceeded'])
    ## icmpv6-header.h: ns3::Icmpv6Header [class]
    module.add_class('Icmpv6Header', parent=root_module['ns3::Header'])
    ## icmpv6-header.h: ns3::Icmpv6Header::Type_e [enumeration]
    module.add_enum('Type_e', ['ICMPV6_ERROR_DESTINATION_UNREACHABLE', 'ICMPV6_ERROR_PACKET_TOO_BIG', 'ICMPV6_ERROR_TIME_EXCEEDED', 'ICMPV6_ERROR_PARAMETER_ERROR', 'ICMPV6_ECHO_REQUEST', 'ICMPV6_ECHO_REPLY', 'ICMPV6_SUBSCRIBE_REQUEST', 'ICMPV6_SUBSCRIBE_REPORT', 'ICMPV6_SUBSCRIVE_END', 'ICMPV6_ND_ROUTER_SOLICITATION', 'ICMPV6_ND_ROUTER_ADVERTISEMENT', 'ICMPV6_ND_NEIGHBOR_SOLICITATION', 'ICMPV6_ND_NEIGHBOR_ADVERTISEMENT', 'ICMPV6_ND_REDIRECTION', 'ICMPV6_ROUTER_RENUMBER', 'ICMPV6_INFORMATION_REQUEST', 'ICMPV6_INFORMATION_RESPONSE', 'ICMPV6_INVERSE_ND_SOLICITATION', 'ICMPV6_INVERSE_ND_ADVERSTISEMENT', 'ICMPV6_MLDV2_SUBSCRIBE_REPORT', 'ICMPV6_MOBILITY_HA_DISCOVER_REQUEST', 'ICMPV6_MOBILITY_HA_DISCOVER_RESPONSE', 'ICMPV6_MOBILITY_MOBILE_PREFIX_SOLICITATION', 'ICMPV6_SECURE_ND_CERTIFICATE_PATH_SOLICITATION', 'ICMPV6_SECURE_ND_CERTIFICATE_PATH_ADVERTISEMENT', 'ICMPV6_EXPERIMENTAL_MOBILITY'], outer_class=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6Header::OptionType_e [enumeration]
    module.add_enum('OptionType_e', ['ICMPV6_OPT_LINK_LAYER_SOURCE', 'ICMPV6_OPT_LINK_LAYER_TARGET', 'ICMPV6_OPT_PREFIX', 'ICMPV6_OPT_REDIRECTED', 'ICMPV6_OPT_MTU'], outer_class=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6Header::ErrorDestinationUnreachable_e [enumeration]
    module.add_enum('ErrorDestinationUnreachable_e', ['ICMPV6_NO_ROUTE', 'ICMPV6_ADM_PROHIBITED', 'ICMPV6_NOT_NEIGHBOUR', 'ICMPV6_ADDR_UNREACHABLE', 'ICMPV6_PORT_UNREACHABLE'], outer_class=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6Header::ErrorTimeExceeded_e [enumeration]
    module.add_enum('ErrorTimeExceeded_e', ['ICMPV6_HOPLIMIT', 'ICMPV6_FRAGTIME'], outer_class=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6Header::ErrorParameterError_e [enumeration]
    module.add_enum('ErrorParameterError_e', ['ICMPV6_MALFORMED_HEADER', 'ICMPV6_UNKNOWN_NEXT_HEADER', 'ICMPV6_UNKNOWN_OPTION'], outer_class=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6NA [class]
    module.add_class('Icmpv6NA', parent=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6NS [class]
    module.add_class('Icmpv6NS', parent=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6OptionHeader [class]
    module.add_class('Icmpv6OptionHeader', parent=root_module['ns3::Header'])
    ## icmpv6-header.h: ns3::Icmpv6OptionLinkLayerAddress [class]
    module.add_class('Icmpv6OptionLinkLayerAddress', parent=root_module['ns3::Icmpv6OptionHeader'])
    ## icmpv6-header.h: ns3::Icmpv6OptionMtu [class]
    module.add_class('Icmpv6OptionMtu', parent=root_module['ns3::Icmpv6OptionHeader'])
    ## icmpv6-header.h: ns3::Icmpv6OptionPrefixInformation [class]
    module.add_class('Icmpv6OptionPrefixInformation', parent=root_module['ns3::Icmpv6OptionHeader'])
    ## icmpv6-header.h: ns3::Icmpv6OptionRedirected [class]
    module.add_class('Icmpv6OptionRedirected', parent=root_module['ns3::Icmpv6OptionHeader'])
    ## icmpv6-header.h: ns3::Icmpv6ParameterError [class]
    module.add_class('Icmpv6ParameterError', parent=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6RA [class]
    module.add_class('Icmpv6RA', parent=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6RS [class]
    module.add_class('Icmpv6RS', parent=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6Redirection [class]
    module.add_class('Icmpv6Redirection', parent=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6TimeExceeded [class]
    module.add_class('Icmpv6TimeExceeded', parent=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6TooBig [class]
    module.add_class('Icmpv6TooBig', parent=root_module['ns3::Icmpv6Header'])
    ## ipv4-header.h: ns3::Ipv4Header [class]
    module.add_class('Ipv4Header', parent=root_module['ns3::Header'])
    ## ipv4-packet-info-tag.h: ns3::Ipv4PacketInfoTag [class]
    module.add_class('Ipv4PacketInfoTag', parent=root_module['ns3::Tag'])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionHeader [class]
    module.add_class('Ipv6ExtensionHeader', parent=root_module['ns3::Header'])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionHopByHopHeader [class]
    module.add_class('Ipv6ExtensionHopByHopHeader', parent=[root_module['ns3::Ipv6ExtensionHeader'], root_module['ns3::OptionField']])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionRoutingHeader [class]
    module.add_class('Ipv6ExtensionRoutingHeader', parent=root_module['ns3::Ipv6ExtensionHeader'])
    ## ipv6-header.h: ns3::Ipv6Header [class]
    module.add_class('Ipv6Header', parent=root_module['ns3::Header'])
    ## ipv6-header.h: ns3::Ipv6Header::NextHeader_e [enumeration]
    module.add_enum('NextHeader_e', ['IPV6_EXT_HOP_BY_HOP', 'IPV6_IPV4', 'IPV6_TCP', 'IPV6_UDP', 'IPV6_IPV6', 'IPV6_EXT_ROUTING', 'IPV6_EXT_FRAGMENTATION', 'IPV6_EXT_CONFIDENTIALITY', 'IPV6_EXT_AUTHENTIFICATION', 'IPV6_ICMPV6', 'IPV6_EXT_END', 'IPV6_EXT_DESTINATION', 'IPV6_SCTP', 'IPV6_EXT_MOBILITY', 'IPV6_UDP_LITE'], outer_class=root_module['ns3::Ipv6Header'])
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader [class]
    module.add_class('Ipv6OptionHeader', parent=root_module['ns3::Header'])
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Alignment [struct]
    module.add_class('Alignment', outer_class=root_module['ns3::Ipv6OptionHeader'])
    ## ipv6-option-header.h: ns3::Ipv6OptionJumbogramHeader [class]
    module.add_class('Ipv6OptionJumbogramHeader', parent=root_module['ns3::Ipv6OptionHeader'])
    ## ipv6-option-header.h: ns3::Ipv6OptionPad1Header [class]
    module.add_class('Ipv6OptionPad1Header', parent=root_module['ns3::Ipv6OptionHeader'])
    ## ipv6-option-header.h: ns3::Ipv6OptionPadnHeader [class]
    module.add_class('Ipv6OptionPadnHeader', parent=root_module['ns3::Ipv6OptionHeader'])
    ## ipv6-option-header.h: ns3::Ipv6OptionRouterAlertHeader [class]
    module.add_class('Ipv6OptionRouterAlertHeader', parent=root_module['ns3::Ipv6OptionHeader'])
    ## ipv6-packet-info-tag.h: ns3::Ipv6PacketInfoTag [class]
    module.add_class('Ipv6PacketInfoTag', parent=root_module['ns3::Tag'])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Ipv4MulticastRoute', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv4MulticastRoute>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Ipv4Route', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv4Route>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Ipv6MulticastRoute', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv6MulticastRoute>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Ipv6Route', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv6Route>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## tcp-header.h: ns3::TcpHeader [class]
    module.add_class('TcpHeader', parent=root_module['ns3::Header'])
    ## tcp-header.h: ns3::TcpHeader::Flags_t [enumeration]
    module.add_enum('Flags_t', ['NONE', 'FIN', 'SYN', 'RST', 'PSH', 'ACK', 'URG'], outer_class=root_module['ns3::TcpHeader'])
    ## tcp-socket.h: ns3::TcpSocket [class]
    module.add_class('TcpSocket', parent=root_module['ns3::Socket'])
    ## tcp-socket-factory.h: ns3::TcpSocketFactory [class]
    module.add_class('TcpSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## udp-header.h: ns3::UdpHeader [class]
    module.add_class('UdpHeader', parent=root_module['ns3::Header'])
    ## udp-socket.h: ns3::UdpSocket [class]
    module.add_class('UdpSocket', parent=root_module['ns3::Socket'])
    ## udp-socket-factory.h: ns3::UdpSocketFactory [class]
    module.add_class('UdpSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## arp-cache.h: ns3::ArpCache [class]
    module.add_class('ArpCache', parent=root_module['ns3::Object'])
    ## arp-cache.h: ns3::ArpCache::Entry [class]
    module.add_class('Entry', outer_class=root_module['ns3::ArpCache'])
    ## arp-header.h: ns3::ArpHeader [class]
    module.add_class('ArpHeader', parent=root_module['ns3::Header'])
    ## arp-header.h: ns3::ArpHeader::ArpType_e [enumeration]
    module.add_enum('ArpType_e', ['ARP_TYPE_REQUEST', 'ARP_TYPE_REPLY'], outer_class=root_module['ns3::ArpHeader'])
    ## arp-l3-protocol.h: ns3::ArpL3Protocol [class]
    module.add_class('ArpL3Protocol', parent=root_module['ns3::Object'])
    ## icmpv6-header.h: ns3::Icmpv6DestinationUnreachable [class]
    module.add_class('Icmpv6DestinationUnreachable', parent=root_module['ns3::Icmpv6Header'])
    ## icmpv6-header.h: ns3::Icmpv6Echo [class]
    module.add_class('Icmpv6Echo', parent=root_module['ns3::Icmpv6Header'])
    ## ipv4.h: ns3::Ipv4 [class]
    module.add_class('Ipv4', parent=root_module['ns3::Object'])
    ## ipv4-interface.h: ns3::Ipv4Interface [class]
    module.add_class('Ipv4Interface', parent=root_module['ns3::Object'])
    ## ipv4-l3-protocol.h: ns3::Ipv4L3Protocol [class]
    module.add_class('Ipv4L3Protocol', parent=root_module['ns3::Ipv4'])
    ## ipv4-l3-protocol.h: ns3::Ipv4L3Protocol::DropReason [enumeration]
    module.add_enum('DropReason', ['DROP_TTL_EXPIRED', 'DROP_NO_ROUTE', 'DROP_BAD_CHECKSUM', 'DROP_INTERFACE_DOWN', 'DROP_ROUTE_ERROR'], outer_class=root_module['ns3::Ipv4L3Protocol'])
    ## ipv4-l4-protocol.h: ns3::Ipv4L4Protocol [class]
    module.add_class('Ipv4L4Protocol', parent=root_module['ns3::Object'])
    ## ipv4-l4-protocol.h: ns3::Ipv4L4Protocol::RxStatus [enumeration]
    module.add_enum('RxStatus', ['RX_OK', 'RX_CSUM_FAILED', 'RX_ENDPOINT_CLOSED', 'RX_ENDPOINT_UNREACH'], outer_class=root_module['ns3::Ipv4L4Protocol'])
    ## ipv4-route.h: ns3::Ipv4MulticastRoute [class]
    module.add_class('Ipv4MulticastRoute', parent=root_module['ns3::SimpleRefCount< ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >'])
    ## ipv4-raw-socket-factory.h: ns3::Ipv4RawSocketFactory [class]
    module.add_class('Ipv4RawSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## ipv4-raw-socket-impl.h: ns3::Ipv4RawSocketImpl [class]
    module.add_class('Ipv4RawSocketImpl', parent=root_module['ns3::Socket'])
    ## ipv4-route.h: ns3::Ipv4Route [class]
    module.add_class('Ipv4Route', parent=root_module['ns3::SimpleRefCount< ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >'])
    ## ipv4-routing-protocol.h: ns3::Ipv4RoutingProtocol [class]
    module.add_class('Ipv4RoutingProtocol', parent=root_module['ns3::Object'])
    ## ipv6.h: ns3::Ipv6 [class]
    module.add_class('Ipv6', parent=root_module['ns3::Object'])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionAHHeader [class]
    module.add_class('Ipv6ExtensionAHHeader', parent=root_module['ns3::Ipv6ExtensionHeader'])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionDestinationHeader [class]
    module.add_class('Ipv6ExtensionDestinationHeader', parent=[root_module['ns3::Ipv6ExtensionHeader'], root_module['ns3::OptionField']])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionESPHeader [class]
    module.add_class('Ipv6ExtensionESPHeader', parent=root_module['ns3::Ipv6ExtensionHeader'])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionFragmentHeader [class]
    module.add_class('Ipv6ExtensionFragmentHeader', parent=root_module['ns3::Ipv6ExtensionHeader'])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionLooseRoutingHeader [class]
    module.add_class('Ipv6ExtensionLooseRoutingHeader', parent=root_module['ns3::Ipv6ExtensionRoutingHeader'])
    ## ipv6-interface.h: ns3::Ipv6Interface [class]
    module.add_class('Ipv6Interface', parent=root_module['ns3::Object'])
    ## ipv6-l3-protocol.h: ns3::Ipv6L3Protocol [class]
    module.add_class('Ipv6L3Protocol', parent=root_module['ns3::Ipv6'])
    ## ipv6-l3-protocol.h: ns3::Ipv6L3Protocol::DropReason [enumeration]
    module.add_enum('DropReason', ['DROP_TTL_EXPIRED', 'DROP_NO_ROUTE', 'DROP_INTERFACE_DOWN', 'DROP_ROUTE_ERROR', 'DROP_UNKNOWN_PROTOCOL'], outer_class=root_module['ns3::Ipv6L3Protocol'])
    ## ipv6-l4-protocol.h: ns3::Ipv6L4Protocol [class]
    module.add_class('Ipv6L4Protocol', parent=root_module['ns3::Object'])
    ## ipv6-l4-protocol.h: ns3::Ipv6L4Protocol::RxStatus_e [enumeration]
    module.add_enum('RxStatus_e', ['RX_OK', 'RX_CSUM_FAILED', 'RX_ENDPOINT_UNREACH'], outer_class=root_module['ns3::Ipv6L4Protocol'])
    ## ipv6-route.h: ns3::Ipv6MulticastRoute [class]
    module.add_class('Ipv6MulticastRoute', parent=root_module['ns3::SimpleRefCount< ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> >'])
    ## ipv6-raw-socket-factory.h: ns3::Ipv6RawSocketFactory [class]
    module.add_class('Ipv6RawSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## ipv6-route.h: ns3::Ipv6Route [class]
    module.add_class('Ipv6Route', parent=root_module['ns3::SimpleRefCount< ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> >'])
    ## ipv6-routing-protocol.h: ns3::Ipv6RoutingProtocol [class]
    module.add_class('Ipv6RoutingProtocol', parent=root_module['ns3::Object'])
    ## ndisc-cache.h: ns3::NdiscCache [class]
    module.add_class('NdiscCache', parent=root_module['ns3::Object'])
    ## ndisc-cache.h: ns3::NdiscCache::Entry [class]
    module.add_class('Entry', outer_class=root_module['ns3::NdiscCache'])
    ## tcp-l4-protocol.h: ns3::TcpL4Protocol [class]
    module.add_class('TcpL4Protocol', parent=root_module['ns3::Ipv4L4Protocol'])
    ## udp-l4-protocol.h: ns3::UdpL4Protocol [class]
    module.add_class('UdpL4Protocol', parent=root_module['ns3::Ipv4L4Protocol'])
    ## icmpv4-l4-protocol.h: ns3::Icmpv4L4Protocol [class]
    module.add_class('Icmpv4L4Protocol', parent=root_module['ns3::Ipv4L4Protocol'])
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol [class]
    module.add_class('Icmpv6L4Protocol', parent=root_module['ns3::Ipv6L4Protocol'])
    ## loopback-net-device.h: ns3::LoopbackNetDevice [class]
    module.add_class('LoopbackNetDevice', parent=root_module['ns3::NetDevice'])
    
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
    register_Ns3Ipv4AddressGenerator_methods(root_module, root_module['ns3::Ipv4AddressGenerator'])
    register_Ns3Ipv4InterfaceAddress_methods(root_module, root_module['ns3::Ipv4InterfaceAddress'])
    register_Ns3Ipv6InterfaceAddress_methods(root_module, root_module['ns3::Ipv6InterfaceAddress'])
    register_Ns3OptionField_methods(root_module, root_module['ns3::OptionField'])
    register_Ns3Icmpv4DestinationUnreachable_methods(root_module, root_module['ns3::Icmpv4DestinationUnreachable'])
    register_Ns3Icmpv4Echo_methods(root_module, root_module['ns3::Icmpv4Echo'])
    register_Ns3Icmpv4Header_methods(root_module, root_module['ns3::Icmpv4Header'])
    register_Ns3Icmpv4TimeExceeded_methods(root_module, root_module['ns3::Icmpv4TimeExceeded'])
    register_Ns3Icmpv6Header_methods(root_module, root_module['ns3::Icmpv6Header'])
    register_Ns3Icmpv6NA_methods(root_module, root_module['ns3::Icmpv6NA'])
    register_Ns3Icmpv6NS_methods(root_module, root_module['ns3::Icmpv6NS'])
    register_Ns3Icmpv6OptionHeader_methods(root_module, root_module['ns3::Icmpv6OptionHeader'])
    register_Ns3Icmpv6OptionLinkLayerAddress_methods(root_module, root_module['ns3::Icmpv6OptionLinkLayerAddress'])
    register_Ns3Icmpv6OptionMtu_methods(root_module, root_module['ns3::Icmpv6OptionMtu'])
    register_Ns3Icmpv6OptionPrefixInformation_methods(root_module, root_module['ns3::Icmpv6OptionPrefixInformation'])
    register_Ns3Icmpv6OptionRedirected_methods(root_module, root_module['ns3::Icmpv6OptionRedirected'])
    register_Ns3Icmpv6ParameterError_methods(root_module, root_module['ns3::Icmpv6ParameterError'])
    register_Ns3Icmpv6RA_methods(root_module, root_module['ns3::Icmpv6RA'])
    register_Ns3Icmpv6RS_methods(root_module, root_module['ns3::Icmpv6RS'])
    register_Ns3Icmpv6Redirection_methods(root_module, root_module['ns3::Icmpv6Redirection'])
    register_Ns3Icmpv6TimeExceeded_methods(root_module, root_module['ns3::Icmpv6TimeExceeded'])
    register_Ns3Icmpv6TooBig_methods(root_module, root_module['ns3::Icmpv6TooBig'])
    register_Ns3Ipv4Header_methods(root_module, root_module['ns3::Ipv4Header'])
    register_Ns3Ipv4PacketInfoTag_methods(root_module, root_module['ns3::Ipv4PacketInfoTag'])
    register_Ns3Ipv6ExtensionHeader_methods(root_module, root_module['ns3::Ipv6ExtensionHeader'])
    register_Ns3Ipv6ExtensionHopByHopHeader_methods(root_module, root_module['ns3::Ipv6ExtensionHopByHopHeader'])
    register_Ns3Ipv6ExtensionRoutingHeader_methods(root_module, root_module['ns3::Ipv6ExtensionRoutingHeader'])
    register_Ns3Ipv6Header_methods(root_module, root_module['ns3::Ipv6Header'])
    register_Ns3Ipv6OptionHeader_methods(root_module, root_module['ns3::Ipv6OptionHeader'])
    register_Ns3Ipv6OptionHeaderAlignment_methods(root_module, root_module['ns3::Ipv6OptionHeader::Alignment'])
    register_Ns3Ipv6OptionJumbogramHeader_methods(root_module, root_module['ns3::Ipv6OptionJumbogramHeader'])
    register_Ns3Ipv6OptionPad1Header_methods(root_module, root_module['ns3::Ipv6OptionPad1Header'])
    register_Ns3Ipv6OptionPadnHeader_methods(root_module, root_module['ns3::Ipv6OptionPadnHeader'])
    register_Ns3Ipv6OptionRouterAlertHeader_methods(root_module, root_module['ns3::Ipv6OptionRouterAlertHeader'])
    register_Ns3Ipv6PacketInfoTag_methods(root_module, root_module['ns3::Ipv6PacketInfoTag'])
    register_Ns3TcpHeader_methods(root_module, root_module['ns3::TcpHeader'])
    register_Ns3TcpSocket_methods(root_module, root_module['ns3::TcpSocket'])
    register_Ns3TcpSocketFactory_methods(root_module, root_module['ns3::TcpSocketFactory'])
    register_Ns3UdpHeader_methods(root_module, root_module['ns3::UdpHeader'])
    register_Ns3UdpSocket_methods(root_module, root_module['ns3::UdpSocket'])
    register_Ns3UdpSocketFactory_methods(root_module, root_module['ns3::UdpSocketFactory'])
    register_Ns3ArpCache_methods(root_module, root_module['ns3::ArpCache'])
    register_Ns3ArpCacheEntry_methods(root_module, root_module['ns3::ArpCache::Entry'])
    register_Ns3ArpHeader_methods(root_module, root_module['ns3::ArpHeader'])
    register_Ns3ArpL3Protocol_methods(root_module, root_module['ns3::ArpL3Protocol'])
    register_Ns3Icmpv6DestinationUnreachable_methods(root_module, root_module['ns3::Icmpv6DestinationUnreachable'])
    register_Ns3Icmpv6Echo_methods(root_module, root_module['ns3::Icmpv6Echo'])
    register_Ns3Ipv4_methods(root_module, root_module['ns3::Ipv4'])
    register_Ns3Ipv4Interface_methods(root_module, root_module['ns3::Ipv4Interface'])
    register_Ns3Ipv4L3Protocol_methods(root_module, root_module['ns3::Ipv4L3Protocol'])
    register_Ns3Ipv4L4Protocol_methods(root_module, root_module['ns3::Ipv4L4Protocol'])
    register_Ns3Ipv4MulticastRoute_methods(root_module, root_module['ns3::Ipv4MulticastRoute'])
    register_Ns3Ipv4RawSocketFactory_methods(root_module, root_module['ns3::Ipv4RawSocketFactory'])
    register_Ns3Ipv4RawSocketImpl_methods(root_module, root_module['ns3::Ipv4RawSocketImpl'])
    register_Ns3Ipv4Route_methods(root_module, root_module['ns3::Ipv4Route'])
    register_Ns3Ipv4RoutingProtocol_methods(root_module, root_module['ns3::Ipv4RoutingProtocol'])
    register_Ns3Ipv6_methods(root_module, root_module['ns3::Ipv6'])
    register_Ns3Ipv6ExtensionAHHeader_methods(root_module, root_module['ns3::Ipv6ExtensionAHHeader'])
    register_Ns3Ipv6ExtensionDestinationHeader_methods(root_module, root_module['ns3::Ipv6ExtensionDestinationHeader'])
    register_Ns3Ipv6ExtensionESPHeader_methods(root_module, root_module['ns3::Ipv6ExtensionESPHeader'])
    register_Ns3Ipv6ExtensionFragmentHeader_methods(root_module, root_module['ns3::Ipv6ExtensionFragmentHeader'])
    register_Ns3Ipv6ExtensionLooseRoutingHeader_methods(root_module, root_module['ns3::Ipv6ExtensionLooseRoutingHeader'])
    register_Ns3Ipv6Interface_methods(root_module, root_module['ns3::Ipv6Interface'])
    register_Ns3Ipv6L3Protocol_methods(root_module, root_module['ns3::Ipv6L3Protocol'])
    register_Ns3Ipv6L4Protocol_methods(root_module, root_module['ns3::Ipv6L4Protocol'])
    register_Ns3Ipv6MulticastRoute_methods(root_module, root_module['ns3::Ipv6MulticastRoute'])
    register_Ns3Ipv6RawSocketFactory_methods(root_module, root_module['ns3::Ipv6RawSocketFactory'])
    register_Ns3Ipv6Route_methods(root_module, root_module['ns3::Ipv6Route'])
    register_Ns3Ipv6RoutingProtocol_methods(root_module, root_module['ns3::Ipv6RoutingProtocol'])
    register_Ns3NdiscCache_methods(root_module, root_module['ns3::NdiscCache'])
    register_Ns3NdiscCacheEntry_methods(root_module, root_module['ns3::NdiscCache::Entry'])
    register_Ns3TcpL4Protocol_methods(root_module, root_module['ns3::TcpL4Protocol'])
    register_Ns3UdpL4Protocol_methods(root_module, root_module['ns3::UdpL4Protocol'])
    register_Ns3Icmpv4L4Protocol_methods(root_module, root_module['ns3::Icmpv4L4Protocol'])
    register_Ns3Icmpv6L4Protocol_methods(root_module, root_module['ns3::Icmpv6L4Protocol'])
    register_Ns3LoopbackNetDevice_methods(root_module, root_module['ns3::LoopbackNetDevice'])
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

def register_Ns3OptionField_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::OptionField::OptionField(ns3::OptionField const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OptionField const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::OptionField::OptionField(uint32_t optionsOffset) [constructor]
    cls.add_constructor([param('uint32_t', 'optionsOffset')])
    ## ipv6-extension-header.h: void ns3::OptionField::AddOption(ns3::Ipv6OptionHeader const & option) [member function]
    cls.add_method('AddOption', 
                   'void', 
                   [param('ns3::Ipv6OptionHeader const &', 'option')])
    ## ipv6-extension-header.h: uint32_t ns3::OptionField::Deserialize(ns3::Buffer::Iterator start, uint32_t length) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'length')])
    ## ipv6-extension-header.h: ns3::Buffer ns3::OptionField::GetOptionBuffer() [member function]
    cls.add_method('GetOptionBuffer', 
                   'ns3::Buffer', 
                   [])
    ## ipv6-extension-header.h: uint32_t ns3::OptionField::GetOptionsOffset() [member function]
    cls.add_method('GetOptionsOffset', 
                   'uint32_t', 
                   [])
    ## ipv6-extension-header.h: uint32_t ns3::OptionField::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: void ns3::OptionField::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    return

def register_Ns3Icmpv4DestinationUnreachable_methods(root_module, cls):
    ## icmpv4.h: ns3::Icmpv4DestinationUnreachable::Icmpv4DestinationUnreachable(ns3::Icmpv4DestinationUnreachable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv4DestinationUnreachable const &', 'arg0')])
    ## icmpv4.h: ns3::Icmpv4DestinationUnreachable::Icmpv4DestinationUnreachable() [constructor]
    cls.add_constructor([])
    ## icmpv4.h: void ns3::Icmpv4DestinationUnreachable::GetData(uint8_t * payload) const [member function]
    cls.add_method('GetData', 
                   'void', 
                   [param('uint8_t *', 'payload')], 
                   is_const=True)
    ## icmpv4.h: ns3::Ipv4Header ns3::Icmpv4DestinationUnreachable::GetHeader() const [member function]
    cls.add_method('GetHeader', 
                   'ns3::Ipv4Header', 
                   [], 
                   is_const=True)
    ## icmpv4.h: uint16_t ns3::Icmpv4DestinationUnreachable::GetNextHopMtu() const [member function]
    cls.add_method('GetNextHopMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## icmpv4.h: static ns3::TypeId ns3::Icmpv4DestinationUnreachable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv4.h: void ns3::Icmpv4DestinationUnreachable::SetData(ns3::Ptr<ns3::Packet const> data) [member function]
    cls.add_method('SetData', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'data')])
    ## icmpv4.h: void ns3::Icmpv4DestinationUnreachable::SetHeader(ns3::Ipv4Header header) [member function]
    cls.add_method('SetHeader', 
                   'void', 
                   [param('ns3::Ipv4Header', 'header')])
    ## icmpv4.h: void ns3::Icmpv4DestinationUnreachable::SetNextHopMtu(uint16_t mtu) [member function]
    cls.add_method('SetNextHopMtu', 
                   'void', 
                   [param('uint16_t', 'mtu')])
    ## icmpv4.h: uint32_t ns3::Icmpv4DestinationUnreachable::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   visibility='private', is_virtual=True)
    ## icmpv4.h: ns3::TypeId ns3::Icmpv4DestinationUnreachable::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## icmpv4.h: uint32_t ns3::Icmpv4DestinationUnreachable::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4DestinationUnreachable::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4DestinationUnreachable::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3Icmpv4Echo_methods(root_module, cls):
    ## icmpv4.h: ns3::Icmpv4Echo::Icmpv4Echo(ns3::Icmpv4Echo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv4Echo const &', 'arg0')])
    ## icmpv4.h: ns3::Icmpv4Echo::Icmpv4Echo() [constructor]
    cls.add_constructor([])
    ## icmpv4.h: uint32_t ns3::Icmpv4Echo::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv4.h: uint32_t ns3::Icmpv4Echo::GetData(uint8_t * payload) const [member function]
    cls.add_method('GetData', 
                   'uint32_t', 
                   [param('uint8_t *', 'payload')], 
                   is_const=True)
    ## icmpv4.h: uint32_t ns3::Icmpv4Echo::GetDataSize() const [member function]
    cls.add_method('GetDataSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv4.h: uint16_t ns3::Icmpv4Echo::GetIdentifier() const [member function]
    cls.add_method('GetIdentifier', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## icmpv4.h: ns3::TypeId ns3::Icmpv4Echo::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: uint16_t ns3::Icmpv4Echo::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## icmpv4.h: uint32_t ns3::Icmpv4Echo::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: static ns3::TypeId ns3::Icmpv4Echo::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv4.h: void ns3::Icmpv4Echo::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4Echo::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4Echo::SetData(ns3::Ptr<ns3::Packet const> data) [member function]
    cls.add_method('SetData', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'data')])
    ## icmpv4.h: void ns3::Icmpv4Echo::SetIdentifier(uint16_t id) [member function]
    cls.add_method('SetIdentifier', 
                   'void', 
                   [param('uint16_t', 'id')])
    ## icmpv4.h: void ns3::Icmpv4Echo::SetSequenceNumber(uint16_t seq) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'seq')])
    return

def register_Ns3Icmpv4Header_methods(root_module, cls):
    ## icmpv4.h: ns3::Icmpv4Header::Icmpv4Header(ns3::Icmpv4Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv4Header const &', 'arg0')])
    ## icmpv4.h: ns3::Icmpv4Header::Icmpv4Header() [constructor]
    cls.add_constructor([])
    ## icmpv4.h: uint32_t ns3::Icmpv4Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4Header::EnableChecksum() [member function]
    cls.add_method('EnableChecksum', 
                   'void', 
                   [])
    ## icmpv4.h: uint8_t ns3::Icmpv4Header::GetCode() const [member function]
    cls.add_method('GetCode', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv4.h: ns3::TypeId ns3::Icmpv4Header::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: uint32_t ns3::Icmpv4Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: uint8_t ns3::Icmpv4Header::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv4.h: static ns3::TypeId ns3::Icmpv4Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv4.h: void ns3::Icmpv4Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4Header::SetCode(uint8_t code) [member function]
    cls.add_method('SetCode', 
                   'void', 
                   [param('uint8_t', 'code')])
    ## icmpv4.h: void ns3::Icmpv4Header::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    return

def register_Ns3Icmpv4TimeExceeded_methods(root_module, cls):
    ## icmpv4.h: ns3::Icmpv4TimeExceeded::Icmpv4TimeExceeded(ns3::Icmpv4TimeExceeded const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv4TimeExceeded const &', 'arg0')])
    ## icmpv4.h: ns3::Icmpv4TimeExceeded::Icmpv4TimeExceeded() [constructor]
    cls.add_constructor([])
    ## icmpv4.h: uint32_t ns3::Icmpv4TimeExceeded::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4TimeExceeded::GetData(uint8_t * payload) const [member function]
    cls.add_method('GetData', 
                   'void', 
                   [param('uint8_t *', 'payload')], 
                   is_const=True)
    ## icmpv4.h: ns3::Ipv4Header ns3::Icmpv4TimeExceeded::GetHeader() const [member function]
    cls.add_method('GetHeader', 
                   'ns3::Ipv4Header', 
                   [], 
                   is_const=True)
    ## icmpv4.h: ns3::TypeId ns3::Icmpv4TimeExceeded::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: uint32_t ns3::Icmpv4TimeExceeded::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: static ns3::TypeId ns3::Icmpv4TimeExceeded::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv4.h: void ns3::Icmpv4TimeExceeded::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4TimeExceeded::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv4.h: void ns3::Icmpv4TimeExceeded::SetData(ns3::Ptr<ns3::Packet const> data) [member function]
    cls.add_method('SetData', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'data')])
    ## icmpv4.h: void ns3::Icmpv4TimeExceeded::SetHeader(ns3::Ipv4Header header) [member function]
    cls.add_method('SetHeader', 
                   'void', 
                   [param('ns3::Ipv4Header', 'header')])
    return

def register_Ns3Icmpv6Header_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6Header::Icmpv6Header(ns3::Icmpv6Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6Header const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6Header::Icmpv6Header() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: void ns3::Icmpv6Header::CalculatePseudoHeaderChecksum(ns3::Ipv6Address src, ns3::Ipv6Address dst, uint16_t length, uint8_t protocol) [member function]
    cls.add_method('CalculatePseudoHeaderChecksum', 
                   'void', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('uint16_t', 'length'), param('uint8_t', 'protocol')])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: uint16_t ns3::Icmpv6Header::GetChecksum() const [member function]
    cls.add_method('GetChecksum', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint8_t ns3::Icmpv6Header::GetCode() const [member function]
    cls.add_method('GetCode', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6Header::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint8_t ns3::Icmpv6Header::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6Header::SetChecksum(uint16_t checksum) [member function]
    cls.add_method('SetChecksum', 
                   'void', 
                   [param('uint16_t', 'checksum')])
    ## icmpv6-header.h: void ns3::Icmpv6Header::SetCode(uint8_t code) [member function]
    cls.add_method('SetCode', 
                   'void', 
                   [param('uint8_t', 'code')])
    ## icmpv6-header.h: void ns3::Icmpv6Header::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    return

def register_Ns3Icmpv6NA_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6NA::Icmpv6NA(ns3::Icmpv6NA const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6NA const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6NA::Icmpv6NA() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6NA::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: bool ns3::Icmpv6NA::GetFlagO() const [member function]
    cls.add_method('GetFlagO', 
                   'bool', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: bool ns3::Icmpv6NA::GetFlagR() const [member function]
    cls.add_method('GetFlagR', 
                   'bool', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: bool ns3::Icmpv6NA::GetFlagS() const [member function]
    cls.add_method('GetFlagS', 
                   'bool', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6NA::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: ns3::Ipv6Address ns3::Icmpv6NA::GetIpv6Target() const [member function]
    cls.add_method('GetIpv6Target', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6NA::GetReserved() const [member function]
    cls.add_method('GetReserved', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6NA::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6NA::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6NA::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6NA::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6NA::SetFlagO(bool o) [member function]
    cls.add_method('SetFlagO', 
                   'void', 
                   [param('bool', 'o')])
    ## icmpv6-header.h: void ns3::Icmpv6NA::SetFlagR(bool r) [member function]
    cls.add_method('SetFlagR', 
                   'void', 
                   [param('bool', 'r')])
    ## icmpv6-header.h: void ns3::Icmpv6NA::SetFlagS(bool s) [member function]
    cls.add_method('SetFlagS', 
                   'void', 
                   [param('bool', 's')])
    ## icmpv6-header.h: void ns3::Icmpv6NA::SetIpv6Target(ns3::Ipv6Address target) [member function]
    cls.add_method('SetIpv6Target', 
                   'void', 
                   [param('ns3::Ipv6Address', 'target')])
    ## icmpv6-header.h: void ns3::Icmpv6NA::SetReserved(uint32_t reserved) [member function]
    cls.add_method('SetReserved', 
                   'void', 
                   [param('uint32_t', 'reserved')])
    return

def register_Ns3Icmpv6NS_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6NS::Icmpv6NS(ns3::Icmpv6NS const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6NS const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6NS::Icmpv6NS(ns3::Ipv6Address target) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'target')])
    ## icmpv6-header.h: ns3::Icmpv6NS::Icmpv6NS() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6NS::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6NS::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: ns3::Ipv6Address ns3::Icmpv6NS::GetIpv6Target() const [member function]
    cls.add_method('GetIpv6Target', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6NS::GetReserved() const [member function]
    cls.add_method('GetReserved', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6NS::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6NS::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6NS::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6NS::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6NS::SetIpv6Target(ns3::Ipv6Address target) [member function]
    cls.add_method('SetIpv6Target', 
                   'void', 
                   [param('ns3::Ipv6Address', 'target')])
    ## icmpv6-header.h: void ns3::Icmpv6NS::SetReserved(uint32_t reserved) [member function]
    cls.add_method('SetReserved', 
                   'void', 
                   [param('uint32_t', 'reserved')])
    return

def register_Ns3Icmpv6OptionHeader_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6OptionHeader::Icmpv6OptionHeader(ns3::Icmpv6OptionHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6OptionHeader const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6OptionHeader::Icmpv6OptionHeader() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6OptionHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint8_t ns3::Icmpv6OptionHeader::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint8_t ns3::Icmpv6OptionHeader::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6OptionHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionHeader::SetLength(uint8_t len) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint8_t', 'len')])
    ## icmpv6-header.h: void ns3::Icmpv6OptionHeader::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    return

def register_Ns3Icmpv6OptionLinkLayerAddress_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6OptionLinkLayerAddress::Icmpv6OptionLinkLayerAddress(ns3::Icmpv6OptionLinkLayerAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6OptionLinkLayerAddress const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6OptionLinkLayerAddress::Icmpv6OptionLinkLayerAddress(bool source) [constructor]
    cls.add_constructor([param('bool', 'source')])
    ## icmpv6-header.h: ns3::Icmpv6OptionLinkLayerAddress::Icmpv6OptionLinkLayerAddress(bool source, ns3::Address addr) [constructor]
    cls.add_constructor([param('bool', 'source'), param('ns3::Address', 'addr')])
    ## icmpv6-header.h: ns3::Icmpv6OptionLinkLayerAddress::Icmpv6OptionLinkLayerAddress() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionLinkLayerAddress::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::Address ns3::Icmpv6OptionLinkLayerAddress::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6OptionLinkLayerAddress::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionLinkLayerAddress::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6OptionLinkLayerAddress::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionLinkLayerAddress::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionLinkLayerAddress::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionLinkLayerAddress::SetAddress(ns3::Address addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'addr')])
    return

def register_Ns3Icmpv6OptionMtu_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6OptionMtu::Icmpv6OptionMtu(ns3::Icmpv6OptionMtu const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6OptionMtu const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6OptionMtu::Icmpv6OptionMtu() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: ns3::Icmpv6OptionMtu::Icmpv6OptionMtu(uint32_t mtu) [constructor]
    cls.add_constructor([param('uint32_t', 'mtu')])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionMtu::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6OptionMtu::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionMtu::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint16_t ns3::Icmpv6OptionMtu::GetReserved() const [member function]
    cls.add_method('GetReserved', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionMtu::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6OptionMtu::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionMtu::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionMtu::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionMtu::SetMtu(uint32_t mtu) [member function]
    cls.add_method('SetMtu', 
                   'void', 
                   [param('uint32_t', 'mtu')])
    ## icmpv6-header.h: void ns3::Icmpv6OptionMtu::SetReserved(uint16_t reserved) [member function]
    cls.add_method('SetReserved', 
                   'void', 
                   [param('uint16_t', 'reserved')])
    return

def register_Ns3Icmpv6OptionPrefixInformation_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6OptionPrefixInformation::Icmpv6OptionPrefixInformation(ns3::Icmpv6OptionPrefixInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6OptionPrefixInformation const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6OptionPrefixInformation::Icmpv6OptionPrefixInformation() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: ns3::Icmpv6OptionPrefixInformation::Icmpv6OptionPrefixInformation(ns3::Ipv6Address network, uint8_t prefixlen) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'network'), param('uint8_t', 'prefixlen')])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionPrefixInformation::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: uint8_t ns3::Icmpv6OptionPrefixInformation::GetFlags() const [member function]
    cls.add_method('GetFlags', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6OptionPrefixInformation::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionPrefixInformation::GetPreferredTime() const [member function]
    cls.add_method('GetPreferredTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::Ipv6Address ns3::Icmpv6OptionPrefixInformation::GetPrefix() const [member function]
    cls.add_method('GetPrefix', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint8_t ns3::Icmpv6OptionPrefixInformation::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionPrefixInformation::GetReserved() const [member function]
    cls.add_method('GetReserved', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionPrefixInformation::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6OptionPrefixInformation::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionPrefixInformation::GetValidTime() const [member function]
    cls.add_method('GetValidTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionPrefixInformation::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionPrefixInformation::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionPrefixInformation::SetFlags(uint8_t flags) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## icmpv6-header.h: void ns3::Icmpv6OptionPrefixInformation::SetPreferredTime(uint32_t preferredTime) [member function]
    cls.add_method('SetPreferredTime', 
                   'void', 
                   [param('uint32_t', 'preferredTime')])
    ## icmpv6-header.h: void ns3::Icmpv6OptionPrefixInformation::SetPrefix(ns3::Ipv6Address prefix) [member function]
    cls.add_method('SetPrefix', 
                   'void', 
                   [param('ns3::Ipv6Address', 'prefix')])
    ## icmpv6-header.h: void ns3::Icmpv6OptionPrefixInformation::SetPrefixLength(uint8_t prefixLength) [member function]
    cls.add_method('SetPrefixLength', 
                   'void', 
                   [param('uint8_t', 'prefixLength')])
    ## icmpv6-header.h: void ns3::Icmpv6OptionPrefixInformation::SetReserved(uint32_t reserved) [member function]
    cls.add_method('SetReserved', 
                   'void', 
                   [param('uint32_t', 'reserved')])
    ## icmpv6-header.h: void ns3::Icmpv6OptionPrefixInformation::SetValidTime(uint32_t validTime) [member function]
    cls.add_method('SetValidTime', 
                   'void', 
                   [param('uint32_t', 'validTime')])
    return

def register_Ns3Icmpv6OptionRedirected_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6OptionRedirected::Icmpv6OptionRedirected(ns3::Icmpv6OptionRedirected const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6OptionRedirected const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6OptionRedirected::Icmpv6OptionRedirected() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionRedirected::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6OptionRedirected::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6OptionRedirected::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6OptionRedirected::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6OptionRedirected::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionRedirected::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionRedirected::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6OptionRedirected::SetPacket(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('SetPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    return

def register_Ns3Icmpv6ParameterError_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6ParameterError::Icmpv6ParameterError(ns3::Icmpv6ParameterError const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6ParameterError const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6ParameterError::Icmpv6ParameterError() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6ParameterError::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6ParameterError::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6ParameterError::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6ParameterError::GetPtr() const [member function]
    cls.add_method('GetPtr', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6ParameterError::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6ParameterError::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6ParameterError::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6ParameterError::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6ParameterError::SetPacket(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('SetPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## icmpv6-header.h: void ns3::Icmpv6ParameterError::SetPtr(uint32_t ptr) [member function]
    cls.add_method('SetPtr', 
                   'void', 
                   [param('uint32_t', 'ptr')])
    return

def register_Ns3Icmpv6RA_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6RA::Icmpv6RA(ns3::Icmpv6RA const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6RA const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6RA::Icmpv6RA() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6RA::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: uint8_t ns3::Icmpv6RA::GetCurHopLimit() const [member function]
    cls.add_method('GetCurHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: bool ns3::Icmpv6RA::GetFlagH() const [member function]
    cls.add_method('GetFlagH', 
                   'bool', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: bool ns3::Icmpv6RA::GetFlagM() const [member function]
    cls.add_method('GetFlagM', 
                   'bool', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: bool ns3::Icmpv6RA::GetFlagO() const [member function]
    cls.add_method('GetFlagO', 
                   'bool', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint8_t ns3::Icmpv6RA::GetFlags() const [member function]
    cls.add_method('GetFlags', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6RA::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint16_t ns3::Icmpv6RA::GetLifeTime() const [member function]
    cls.add_method('GetLifeTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6RA::GetReachableTime() const [member function]
    cls.add_method('GetReachableTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6RA::GetRetransmissionTime() const [member function]
    cls.add_method('GetRetransmissionTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6RA::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6RA::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6RA::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6RA::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6RA::SetCurHopLimit(uint8_t m) [member function]
    cls.add_method('SetCurHopLimit', 
                   'void', 
                   [param('uint8_t', 'm')])
    ## icmpv6-header.h: void ns3::Icmpv6RA::SetFlagH(bool h) [member function]
    cls.add_method('SetFlagH', 
                   'void', 
                   [param('bool', 'h')])
    ## icmpv6-header.h: void ns3::Icmpv6RA::SetFlagM(bool m) [member function]
    cls.add_method('SetFlagM', 
                   'void', 
                   [param('bool', 'm')])
    ## icmpv6-header.h: void ns3::Icmpv6RA::SetFlagO(bool o) [member function]
    cls.add_method('SetFlagO', 
                   'void', 
                   [param('bool', 'o')])
    ## icmpv6-header.h: void ns3::Icmpv6RA::SetFlags(uint8_t f) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint8_t', 'f')])
    ## icmpv6-header.h: void ns3::Icmpv6RA::SetLifeTime(uint16_t l) [member function]
    cls.add_method('SetLifeTime', 
                   'void', 
                   [param('uint16_t', 'l')])
    ## icmpv6-header.h: void ns3::Icmpv6RA::SetReachableTime(uint32_t r) [member function]
    cls.add_method('SetReachableTime', 
                   'void', 
                   [param('uint32_t', 'r')])
    ## icmpv6-header.h: void ns3::Icmpv6RA::SetRetransmissionTime(uint32_t r) [member function]
    cls.add_method('SetRetransmissionTime', 
                   'void', 
                   [param('uint32_t', 'r')])
    return

def register_Ns3Icmpv6RS_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6RS::Icmpv6RS(ns3::Icmpv6RS const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6RS const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6RS::Icmpv6RS() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6RS::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6RS::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6RS::GetReserved() const [member function]
    cls.add_method('GetReserved', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6RS::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6RS::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6RS::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6RS::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6RS::SetReserved(uint32_t reserved) [member function]
    cls.add_method('SetReserved', 
                   'void', 
                   [param('uint32_t', 'reserved')])
    return

def register_Ns3Icmpv6Redirection_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6Redirection::Icmpv6Redirection(ns3::Icmpv6Redirection const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6Redirection const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6Redirection::Icmpv6Redirection() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6Redirection::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::Ipv6Address ns3::Icmpv6Redirection::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6Redirection::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6Redirection::GetReserved() const [member function]
    cls.add_method('GetReserved', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6Redirection::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: ns3::Ipv6Address ns3::Icmpv6Redirection::GetTarget() const [member function]
    cls.add_method('GetTarget', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6Redirection::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6Redirection::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6Redirection::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6Redirection::SetDestination(ns3::Ipv6Address destination) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Ipv6Address', 'destination')])
    ## icmpv6-header.h: void ns3::Icmpv6Redirection::SetReserved(uint32_t reserved) [member function]
    cls.add_method('SetReserved', 
                   'void', 
                   [param('uint32_t', 'reserved')])
    ## icmpv6-header.h: void ns3::Icmpv6Redirection::SetTarget(ns3::Ipv6Address target) [member function]
    cls.add_method('SetTarget', 
                   'void', 
                   [param('ns3::Ipv6Address', 'target')])
    return

def register_Ns3Icmpv6TimeExceeded_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6TimeExceeded::Icmpv6TimeExceeded(ns3::Icmpv6TimeExceeded const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6TimeExceeded const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6TimeExceeded::Icmpv6TimeExceeded() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6TimeExceeded::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6TimeExceeded::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6TimeExceeded::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6TimeExceeded::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6TimeExceeded::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6TimeExceeded::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6TimeExceeded::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6TimeExceeded::SetPacket(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('SetPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    return

def register_Ns3Icmpv6TooBig_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6TooBig::Icmpv6TooBig(ns3::Icmpv6TooBig const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6TooBig const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6TooBig::Icmpv6TooBig() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6TooBig::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6TooBig::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6TooBig::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6TooBig::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6TooBig::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6TooBig::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6TooBig::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6TooBig::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6TooBig::SetMtu(uint32_t mtu) [member function]
    cls.add_method('SetMtu', 
                   'void', 
                   [param('uint32_t', 'mtu')])
    ## icmpv6-header.h: void ns3::Icmpv6TooBig::SetPacket(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('SetPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
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

def register_Ns3Ipv6ExtensionHeader_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionHeader::Ipv6ExtensionHeader(ns3::Ipv6ExtensionHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ExtensionHeader const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionHeader::Ipv6ExtensionHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-extension-header.h: ns3::TypeId ns3::Ipv6ExtensionHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: uint16_t ns3::Ipv6ExtensionHeader::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: uint8_t ns3::Ipv6ExtensionHeader::GetNextHeader() const [member function]
    cls.add_method('GetNextHeader', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: static ns3::TypeId ns3::Ipv6ExtensionHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionHeader::SetLength(uint16_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint16_t', 'length')])
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionHeader::SetNextHeader(uint8_t nextHeader) [member function]
    cls.add_method('SetNextHeader', 
                   'void', 
                   [param('uint8_t', 'nextHeader')])
    return

def register_Ns3Ipv6ExtensionHopByHopHeader_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionHopByHopHeader::Ipv6ExtensionHopByHopHeader(ns3::Ipv6ExtensionHopByHopHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ExtensionHopByHopHeader const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionHopByHopHeader::Ipv6ExtensionHopByHopHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionHopByHopHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-extension-header.h: ns3::TypeId ns3::Ipv6ExtensionHopByHopHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionHopByHopHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: static ns3::TypeId ns3::Ipv6ExtensionHopByHopHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionHopByHopHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionHopByHopHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv6ExtensionRoutingHeader_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionRoutingHeader::Ipv6ExtensionRoutingHeader(ns3::Ipv6ExtensionRoutingHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ExtensionRoutingHeader const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionRoutingHeader::Ipv6ExtensionRoutingHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionRoutingHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-extension-header.h: ns3::TypeId ns3::Ipv6ExtensionRoutingHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: uint8_t ns3::Ipv6ExtensionRoutingHeader::GetSegmentsLeft() const [member function]
    cls.add_method('GetSegmentsLeft', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionRoutingHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: static ns3::TypeId ns3::Ipv6ExtensionRoutingHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-extension-header.h: uint8_t ns3::Ipv6ExtensionRoutingHeader::GetTypeRouting() const [member function]
    cls.add_method('GetTypeRouting', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionRoutingHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionRoutingHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionRoutingHeader::SetSegmentsLeft(uint8_t segmentsLeft) [member function]
    cls.add_method('SetSegmentsLeft', 
                   'void', 
                   [param('uint8_t', 'segmentsLeft')])
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionRoutingHeader::SetTypeRouting(uint8_t typeRouting) [member function]
    cls.add_method('SetTypeRouting', 
                   'void', 
                   [param('uint8_t', 'typeRouting')])
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

def register_Ns3Ipv6OptionHeader_methods(root_module, cls):
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Ipv6OptionHeader(ns3::Ipv6OptionHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6OptionHeader const &', 'arg0')])
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Ipv6OptionHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Alignment ns3::Ipv6OptionHeader::GetAlignment() const [member function]
    cls.add_method('GetAlignment', 
                   'ns3::Ipv6OptionHeader::Alignment', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: ns3::TypeId ns3::Ipv6OptionHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: uint8_t ns3::Ipv6OptionHeader::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: uint8_t ns3::Ipv6OptionHeader::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-option-header.h: static ns3::TypeId ns3::Ipv6OptionHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionHeader::SetLength(uint8_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint8_t', 'length')])
    ## ipv6-option-header.h: void ns3::Ipv6OptionHeader::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    return

def register_Ns3Ipv6OptionHeaderAlignment_methods(root_module, cls):
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Alignment::Alignment() [constructor]
    cls.add_constructor([])
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Alignment::Alignment(ns3::Ipv6OptionHeader::Alignment const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6OptionHeader::Alignment const &', 'arg0')])
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Alignment::factor [variable]
    cls.add_instance_attribute('factor', 'uint8_t', is_const=False)
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Alignment::offset [variable]
    cls.add_instance_attribute('offset', 'uint8_t', is_const=False)
    return

def register_Ns3Ipv6OptionJumbogramHeader_methods(root_module, cls):
    ## ipv6-option-header.h: ns3::Ipv6OptionJumbogramHeader::Ipv6OptionJumbogramHeader(ns3::Ipv6OptionJumbogramHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6OptionJumbogramHeader const &', 'arg0')])
    ## ipv6-option-header.h: ns3::Ipv6OptionJumbogramHeader::Ipv6OptionJumbogramHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionJumbogramHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Alignment ns3::Ipv6OptionJumbogramHeader::GetAlignment() const [member function]
    cls.add_method('GetAlignment', 
                   'ns3::Ipv6OptionHeader::Alignment', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionJumbogramHeader::GetDataLength() const [member function]
    cls.add_method('GetDataLength', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-option-header.h: ns3::TypeId ns3::Ipv6OptionJumbogramHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionJumbogramHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: static ns3::TypeId ns3::Ipv6OptionJumbogramHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionJumbogramHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionJumbogramHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionJumbogramHeader::SetDataLength(uint32_t dataLength) [member function]
    cls.add_method('SetDataLength', 
                   'void', 
                   [param('uint32_t', 'dataLength')])
    return

def register_Ns3Ipv6OptionPad1Header_methods(root_module, cls):
    ## ipv6-option-header.h: ns3::Ipv6OptionPad1Header::Ipv6OptionPad1Header(ns3::Ipv6OptionPad1Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6OptionPad1Header const &', 'arg0')])
    ## ipv6-option-header.h: ns3::Ipv6OptionPad1Header::Ipv6OptionPad1Header() [constructor]
    cls.add_constructor([])
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionPad1Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-option-header.h: ns3::TypeId ns3::Ipv6OptionPad1Header::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionPad1Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: static ns3::TypeId ns3::Ipv6OptionPad1Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionPad1Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionPad1Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv6OptionPadnHeader_methods(root_module, cls):
    ## ipv6-option-header.h: ns3::Ipv6OptionPadnHeader::Ipv6OptionPadnHeader(ns3::Ipv6OptionPadnHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6OptionPadnHeader const &', 'arg0')])
    ## ipv6-option-header.h: ns3::Ipv6OptionPadnHeader::Ipv6OptionPadnHeader(uint32_t pad=2) [constructor]
    cls.add_constructor([param('uint32_t', 'pad', default_value='2')])
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionPadnHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-option-header.h: ns3::TypeId ns3::Ipv6OptionPadnHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionPadnHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: static ns3::TypeId ns3::Ipv6OptionPadnHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionPadnHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionPadnHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv6OptionRouterAlertHeader_methods(root_module, cls):
    ## ipv6-option-header.h: ns3::Ipv6OptionRouterAlertHeader::Ipv6OptionRouterAlertHeader(ns3::Ipv6OptionRouterAlertHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6OptionRouterAlertHeader const &', 'arg0')])
    ## ipv6-option-header.h: ns3::Ipv6OptionRouterAlertHeader::Ipv6OptionRouterAlertHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionRouterAlertHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-option-header.h: ns3::Ipv6OptionHeader::Alignment ns3::Ipv6OptionRouterAlertHeader::GetAlignment() const [member function]
    cls.add_method('GetAlignment', 
                   'ns3::Ipv6OptionHeader::Alignment', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: ns3::TypeId ns3::Ipv6OptionRouterAlertHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: uint32_t ns3::Ipv6OptionRouterAlertHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: static ns3::TypeId ns3::Ipv6OptionRouterAlertHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-option-header.h: uint16_t ns3::Ipv6OptionRouterAlertHeader::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionRouterAlertHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionRouterAlertHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-option-header.h: void ns3::Ipv6OptionRouterAlertHeader::SetValue(uint16_t value) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('uint16_t', 'value')])
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

def register_Ns3TcpHeader_methods(root_module, cls):
    ## tcp-header.h: ns3::TcpHeader::TcpHeader(ns3::TcpHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TcpHeader const &', 'arg0')])
    ## tcp-header.h: ns3::TcpHeader::TcpHeader() [constructor]
    cls.add_constructor([])
    ## tcp-header.h: uint32_t ns3::TcpHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## tcp-header.h: void ns3::TcpHeader::EnableChecksums() [member function]
    cls.add_method('EnableChecksums', 
                   'void', 
                   [])
    ## tcp-header.h: ns3::SequenceNumber32 ns3::TcpHeader::GetAckNumber() const [member function]
    cls.add_method('GetAckNumber', 
                   'ns3::SequenceNumber32', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint16_t ns3::TcpHeader::GetDestinationPort() const [member function]
    cls.add_method('GetDestinationPort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint8_t ns3::TcpHeader::GetFlags() const [member function]
    cls.add_method('GetFlags', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: ns3::TypeId ns3::TcpHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tcp-header.h: uint8_t ns3::TcpHeader::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: ns3::SequenceNumber32 ns3::TcpHeader::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'ns3::SequenceNumber32', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint32_t ns3::TcpHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tcp-header.h: uint16_t ns3::TcpHeader::GetSourcePort() const [member function]
    cls.add_method('GetSourcePort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: static ns3::TypeId ns3::TcpHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tcp-header.h: uint16_t ns3::TcpHeader::GetUrgentPointer() const [member function]
    cls.add_method('GetUrgentPointer', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint16_t ns3::TcpHeader::GetWindowSize() const [member function]
    cls.add_method('GetWindowSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: void ns3::TcpHeader::InitializeChecksum(ns3::Ipv4Address source, ns3::Ipv4Address destination, uint8_t protocol) [member function]
    cls.add_method('InitializeChecksum', 
                   'void', 
                   [param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'destination'), param('uint8_t', 'protocol')])
    ## tcp-header.h: bool ns3::TcpHeader::IsChecksumOk() const [member function]
    cls.add_method('IsChecksumOk', 
                   'bool', 
                   [], 
                   is_const=True)
    ## tcp-header.h: void ns3::TcpHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## tcp-header.h: void ns3::TcpHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## tcp-header.h: void ns3::TcpHeader::SetAckNumber(ns3::SequenceNumber32 ackNumber) [member function]
    cls.add_method('SetAckNumber', 
                   'void', 
                   [param('ns3::SequenceNumber32', 'ackNumber')])
    ## tcp-header.h: void ns3::TcpHeader::SetDestinationPort(uint16_t port) [member function]
    cls.add_method('SetDestinationPort', 
                   'void', 
                   [param('uint16_t', 'port')])
    ## tcp-header.h: void ns3::TcpHeader::SetFlags(uint8_t flags) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## tcp-header.h: void ns3::TcpHeader::SetLength(uint8_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint8_t', 'length')])
    ## tcp-header.h: void ns3::TcpHeader::SetSequenceNumber(ns3::SequenceNumber32 sequenceNumber) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('ns3::SequenceNumber32', 'sequenceNumber')])
    ## tcp-header.h: void ns3::TcpHeader::SetSourcePort(uint16_t port) [member function]
    cls.add_method('SetSourcePort', 
                   'void', 
                   [param('uint16_t', 'port')])
    ## tcp-header.h: void ns3::TcpHeader::SetUrgentPointer(uint16_t urgentPointer) [member function]
    cls.add_method('SetUrgentPointer', 
                   'void', 
                   [param('uint16_t', 'urgentPointer')])
    ## tcp-header.h: void ns3::TcpHeader::SetWindowSize(uint16_t windowSize) [member function]
    cls.add_method('SetWindowSize', 
                   'void', 
                   [param('uint16_t', 'windowSize')])
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
    ## tcp-socket.h: ns3::TcpSocket::TcpStateName [variable]
    cls.add_static_attribute('TcpStateName', 'char const * [ 11 ] const', is_const=True)
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
    ## tcp-socket.h: ns3::Time ns3::TcpSocket::GetPersistTimeout() const [member function]
    cls.add_method('GetPersistTimeout', 
                   'ns3::Time', 
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
    ## tcp-socket.h: void ns3::TcpSocket::SetPersistTimeout(ns3::Time timeout) [member function]
    cls.add_method('SetPersistTimeout', 
                   'void', 
                   [param('ns3::Time', 'timeout')], 
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

def register_Ns3UdpHeader_methods(root_module, cls):
    ## udp-header.h: ns3::UdpHeader::UdpHeader(ns3::UdpHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpHeader const &', 'arg0')])
    ## udp-header.h: ns3::UdpHeader::UdpHeader() [constructor]
    cls.add_constructor([])
    ## udp-header.h: uint32_t ns3::UdpHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## udp-header.h: void ns3::UdpHeader::EnableChecksums() [member function]
    cls.add_method('EnableChecksums', 
                   'void', 
                   [])
    ## udp-header.h: uint16_t ns3::UdpHeader::GetDestinationPort() const [member function]
    cls.add_method('GetDestinationPort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## udp-header.h: ns3::TypeId ns3::UdpHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## udp-header.h: uint32_t ns3::UdpHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## udp-header.h: uint16_t ns3::UdpHeader::GetSourcePort() const [member function]
    cls.add_method('GetSourcePort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## udp-header.h: static ns3::TypeId ns3::UdpHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-header.h: void ns3::UdpHeader::InitializeChecksum(ns3::Ipv4Address source, ns3::Ipv4Address destination, uint8_t protocol) [member function]
    cls.add_method('InitializeChecksum', 
                   'void', 
                   [param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'destination'), param('uint8_t', 'protocol')])
    ## udp-header.h: bool ns3::UdpHeader::IsChecksumOk() const [member function]
    cls.add_method('IsChecksumOk', 
                   'bool', 
                   [], 
                   is_const=True)
    ## udp-header.h: void ns3::UdpHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## udp-header.h: void ns3::UdpHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## udp-header.h: void ns3::UdpHeader::SetDestinationPort(uint16_t port) [member function]
    cls.add_method('SetDestinationPort', 
                   'void', 
                   [param('uint16_t', 'port')])
    ## udp-header.h: void ns3::UdpHeader::SetSourcePort(uint16_t port) [member function]
    cls.add_method('SetSourcePort', 
                   'void', 
                   [param('uint16_t', 'port')])
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

def register_Ns3ArpCache_methods(root_module, cls):
    ## arp-cache.h: ns3::ArpCache::ArpCache() [constructor]
    cls.add_constructor([])
    ## arp-cache.h: ns3::ArpCache::Entry * ns3::ArpCache::Add(ns3::Ipv4Address to) [member function]
    cls.add_method('Add', 
                   'ns3::ArpCache::Entry *', 
                   [param('ns3::Ipv4Address', 'to')])
    ## arp-cache.h: void ns3::ArpCache::Flush() [member function]
    cls.add_method('Flush', 
                   'void', 
                   [])
    ## arp-cache.h: ns3::Time ns3::ArpCache::GetAliveTimeout() const [member function]
    cls.add_method('GetAliveTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## arp-cache.h: ns3::Time ns3::ArpCache::GetDeadTimeout() const [member function]
    cls.add_method('GetDeadTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## arp-cache.h: ns3::Ptr<ns3::NetDevice> ns3::ArpCache::GetDevice() const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## arp-cache.h: ns3::Ptr<ns3::Ipv4Interface> ns3::ArpCache::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ptr< ns3::Ipv4Interface >', 
                   [], 
                   is_const=True)
    ## arp-cache.h: static ns3::TypeId ns3::ArpCache::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## arp-cache.h: ns3::Time ns3::ArpCache::GetWaitReplyTimeout() const [member function]
    cls.add_method('GetWaitReplyTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## arp-cache.h: ns3::ArpCache::Entry * ns3::ArpCache::Lookup(ns3::Ipv4Address destination) [member function]
    cls.add_method('Lookup', 
                   'ns3::ArpCache::Entry *', 
                   [param('ns3::Ipv4Address', 'destination')])
    ## arp-cache.h: void ns3::ArpCache::SetAliveTimeout(ns3::Time aliveTimeout) [member function]
    cls.add_method('SetAliveTimeout', 
                   'void', 
                   [param('ns3::Time', 'aliveTimeout')])
    ## arp-cache.h: void ns3::ArpCache::SetArpRequestCallback(ns3::Callback<void, ns3::Ptr<ns3::ArpCache const>, ns3::Ipv4Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> arpRequestCallback) [member function]
    cls.add_method('SetArpRequestCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::ArpCache const >, ns3::Ipv4Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'arpRequestCallback')])
    ## arp-cache.h: void ns3::ArpCache::SetDeadTimeout(ns3::Time deadTimeout) [member function]
    cls.add_method('SetDeadTimeout', 
                   'void', 
                   [param('ns3::Time', 'deadTimeout')])
    ## arp-cache.h: void ns3::ArpCache::SetDevice(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::Ipv4Interface> interface) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Ipv4Interface >', 'interface')])
    ## arp-cache.h: void ns3::ArpCache::SetWaitReplyTimeout(ns3::Time waitReplyTimeout) [member function]
    cls.add_method('SetWaitReplyTimeout', 
                   'void', 
                   [param('ns3::Time', 'waitReplyTimeout')])
    ## arp-cache.h: void ns3::ArpCache::StartWaitReplyTimer() [member function]
    cls.add_method('StartWaitReplyTimer', 
                   'void', 
                   [])
    ## arp-cache.h: void ns3::ArpCache::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ArpCacheEntry_methods(root_module, cls):
    ## arp-cache.h: ns3::ArpCache::Entry::Entry(ns3::ArpCache::Entry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ArpCache::Entry const &', 'arg0')])
    ## arp-cache.h: ns3::ArpCache::Entry::Entry(ns3::ArpCache * arp) [constructor]
    cls.add_constructor([param('ns3::ArpCache *', 'arp')])
    ## arp-cache.h: void ns3::ArpCache::Entry::ClearRetries() [member function]
    cls.add_method('ClearRetries', 
                   'void', 
                   [])
    ## arp-cache.h: ns3::Ptr<ns3::Packet> ns3::ArpCache::Entry::DequeuePending() [member function]
    cls.add_method('DequeuePending', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## arp-cache.h: ns3::Ipv4Address ns3::ArpCache::Entry::GetIpv4Address() const [member function]
    cls.add_method('GetIpv4Address', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## arp-cache.h: ns3::Address ns3::ArpCache::Entry::GetMacAddress() const [member function]
    cls.add_method('GetMacAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## arp-cache.h: uint32_t ns3::ArpCache::Entry::GetRetries() const [member function]
    cls.add_method('GetRetries', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## arp-cache.h: void ns3::ArpCache::Entry::IncrementRetries() [member function]
    cls.add_method('IncrementRetries', 
                   'void', 
                   [])
    ## arp-cache.h: bool ns3::ArpCache::Entry::IsAlive() [member function]
    cls.add_method('IsAlive', 
                   'bool', 
                   [])
    ## arp-cache.h: bool ns3::ArpCache::Entry::IsDead() [member function]
    cls.add_method('IsDead', 
                   'bool', 
                   [])
    ## arp-cache.h: bool ns3::ArpCache::Entry::IsExpired() const [member function]
    cls.add_method('IsExpired', 
                   'bool', 
                   [], 
                   is_const=True)
    ## arp-cache.h: bool ns3::ArpCache::Entry::IsWaitReply() [member function]
    cls.add_method('IsWaitReply', 
                   'bool', 
                   [])
    ## arp-cache.h: void ns3::ArpCache::Entry::MarkAlive(ns3::Address macAddress) [member function]
    cls.add_method('MarkAlive', 
                   'void', 
                   [param('ns3::Address', 'macAddress')])
    ## arp-cache.h: void ns3::ArpCache::Entry::MarkDead() [member function]
    cls.add_method('MarkDead', 
                   'void', 
                   [])
    ## arp-cache.h: void ns3::ArpCache::Entry::MarkWaitReply(ns3::Ptr<ns3::Packet> waiting) [member function]
    cls.add_method('MarkWaitReply', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'waiting')])
    ## arp-cache.h: void ns3::ArpCache::Entry::SetIpv4Address(ns3::Ipv4Address destination) [member function]
    cls.add_method('SetIpv4Address', 
                   'void', 
                   [param('ns3::Ipv4Address', 'destination')])
    ## arp-cache.h: bool ns3::ArpCache::Entry::UpdateWaitReply(ns3::Ptr<ns3::Packet> waiting) [member function]
    cls.add_method('UpdateWaitReply', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'waiting')])
    return

def register_Ns3ArpHeader_methods(root_module, cls):
    ## arp-header.h: ns3::ArpHeader::ArpHeader() [constructor]
    cls.add_constructor([])
    ## arp-header.h: ns3::ArpHeader::ArpHeader(ns3::ArpHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ArpHeader const &', 'arg0')])
    ## arp-header.h: uint32_t ns3::ArpHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## arp-header.h: ns3::Address ns3::ArpHeader::GetDestinationHardwareAddress() [member function]
    cls.add_method('GetDestinationHardwareAddress', 
                   'ns3::Address', 
                   [])
    ## arp-header.h: ns3::Ipv4Address ns3::ArpHeader::GetDestinationIpv4Address() [member function]
    cls.add_method('GetDestinationIpv4Address', 
                   'ns3::Ipv4Address', 
                   [])
    ## arp-header.h: ns3::TypeId ns3::ArpHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## arp-header.h: uint32_t ns3::ArpHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## arp-header.h: ns3::Address ns3::ArpHeader::GetSourceHardwareAddress() [member function]
    cls.add_method('GetSourceHardwareAddress', 
                   'ns3::Address', 
                   [])
    ## arp-header.h: ns3::Ipv4Address ns3::ArpHeader::GetSourceIpv4Address() [member function]
    cls.add_method('GetSourceIpv4Address', 
                   'ns3::Ipv4Address', 
                   [])
    ## arp-header.h: static ns3::TypeId ns3::ArpHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## arp-header.h: bool ns3::ArpHeader::IsReply() const [member function]
    cls.add_method('IsReply', 
                   'bool', 
                   [], 
                   is_const=True)
    ## arp-header.h: bool ns3::ArpHeader::IsRequest() const [member function]
    cls.add_method('IsRequest', 
                   'bool', 
                   [], 
                   is_const=True)
    ## arp-header.h: void ns3::ArpHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## arp-header.h: void ns3::ArpHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## arp-header.h: void ns3::ArpHeader::SetReply(ns3::Address sourceHardwareAddress, ns3::Ipv4Address sourceProtocolAddress, ns3::Address destinationHardwareAddress, ns3::Ipv4Address destinationProtocolAddress) [member function]
    cls.add_method('SetReply', 
                   'void', 
                   [param('ns3::Address', 'sourceHardwareAddress'), param('ns3::Ipv4Address', 'sourceProtocolAddress'), param('ns3::Address', 'destinationHardwareAddress'), param('ns3::Ipv4Address', 'destinationProtocolAddress')])
    ## arp-header.h: void ns3::ArpHeader::SetRequest(ns3::Address sourceHardwareAddress, ns3::Ipv4Address sourceProtocolAddress, ns3::Address destinationHardwareAddress, ns3::Ipv4Address destinationProtocolAddress) [member function]
    cls.add_method('SetRequest', 
                   'void', 
                   [param('ns3::Address', 'sourceHardwareAddress'), param('ns3::Ipv4Address', 'sourceProtocolAddress'), param('ns3::Address', 'destinationHardwareAddress'), param('ns3::Ipv4Address', 'destinationProtocolAddress')])
    ## arp-header.h: ns3::ArpHeader::m_ipv4Dest [variable]
    cls.add_instance_attribute('m_ipv4Dest', 'ns3::Ipv4Address', is_const=False)
    ## arp-header.h: ns3::ArpHeader::m_ipv4Source [variable]
    cls.add_instance_attribute('m_ipv4Source', 'ns3::Ipv4Address', is_const=False)
    ## arp-header.h: ns3::ArpHeader::m_macDest [variable]
    cls.add_instance_attribute('m_macDest', 'ns3::Address', is_const=False)
    ## arp-header.h: ns3::ArpHeader::m_macSource [variable]
    cls.add_instance_attribute('m_macSource', 'ns3::Address', is_const=False)
    ## arp-header.h: ns3::ArpHeader::m_type [variable]
    cls.add_instance_attribute('m_type', 'uint16_t', is_const=False)
    return

def register_Ns3ArpL3Protocol_methods(root_module, cls):
    ## arp-l3-protocol.h: ns3::ArpL3Protocol::PROT_NUMBER [variable]
    cls.add_static_attribute('PROT_NUMBER', 'uint16_t const', is_const=True)
    ## arp-l3-protocol.h: static ns3::TypeId ns3::ArpL3Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## arp-l3-protocol.h: ns3::ArpL3Protocol::ArpL3Protocol() [constructor]
    cls.add_constructor([])
    ## arp-l3-protocol.h: void ns3::ArpL3Protocol::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## arp-l3-protocol.h: ns3::Ptr<ns3::ArpCache> ns3::ArpL3Protocol::CreateCache(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::Ipv4Interface> interface) [member function]
    cls.add_method('CreateCache', 
                   'ns3::Ptr< ns3::ArpCache >', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Ipv4Interface >', 'interface')])
    ## arp-l3-protocol.h: void ns3::ArpL3Protocol::Receive(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::Packet const> p, uint16_t protocol, ns3::Address const & from, ns3::Address const & to, ns3::NetDevice::PacketType packetType) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Packet const >', 'p'), param('uint16_t', 'protocol'), param('ns3::Address const &', 'from'), param('ns3::Address const &', 'to'), param('ns3::NetDevice::PacketType', 'packetType')])
    ## arp-l3-protocol.h: bool ns3::ArpL3Protocol::Lookup(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Address destination, ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::ArpCache> cache, ns3::Address * hardwareDestination) [member function]
    cls.add_method('Lookup', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Address', 'destination'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::ArpCache >', 'cache'), param('ns3::Address *', 'hardwareDestination')])
    ## arp-l3-protocol.h: void ns3::ArpL3Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## arp-l3-protocol.h: void ns3::ArpL3Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Icmpv6DestinationUnreachable_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6DestinationUnreachable::Icmpv6DestinationUnreachable(ns3::Icmpv6DestinationUnreachable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6DestinationUnreachable const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6DestinationUnreachable::Icmpv6DestinationUnreachable() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6DestinationUnreachable::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6DestinationUnreachable::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6DestinationUnreachable::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6DestinationUnreachable::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6DestinationUnreachable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6DestinationUnreachable::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6DestinationUnreachable::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6DestinationUnreachable::SetPacket(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('SetPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    return

def register_Ns3Icmpv6Echo_methods(root_module, cls):
    ## icmpv6-header.h: ns3::Icmpv6Echo::Icmpv6Echo(ns3::Icmpv6Echo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6Echo const &', 'arg0')])
    ## icmpv6-header.h: ns3::Icmpv6Echo::Icmpv6Echo() [constructor]
    cls.add_constructor([])
    ## icmpv6-header.h: ns3::Icmpv6Echo::Icmpv6Echo(bool request) [constructor]
    cls.add_constructor([param('bool', 'request')])
    ## icmpv6-header.h: uint32_t ns3::Icmpv6Echo::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## icmpv6-header.h: uint16_t ns3::Icmpv6Echo::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: ns3::TypeId ns3::Icmpv6Echo::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: uint16_t ns3::Icmpv6Echo::GetSeq() const [member function]
    cls.add_method('GetSeq', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## icmpv6-header.h: uint32_t ns3::Icmpv6Echo::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: static ns3::TypeId ns3::Icmpv6Echo::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-header.h: void ns3::Icmpv6Echo::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6Echo::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## icmpv6-header.h: void ns3::Icmpv6Echo::SetId(uint16_t id) [member function]
    cls.add_method('SetId', 
                   'void', 
                   [param('uint16_t', 'id')])
    ## icmpv6-header.h: void ns3::Icmpv6Echo::SetSeq(uint16_t seq) [member function]
    cls.add_method('SetSeq', 
                   'void', 
                   [param('uint16_t', 'seq')])
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
    ## ipv4.h: void ns3::Ipv4::Insert(ns3::Ptr<ns3::Ipv4L4Protocol> protocol) [member function]
    cls.add_method('Insert', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4L4Protocol >', 'protocol')], 
                   is_pure_virtual=True, is_virtual=True)
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
    ## ipv4.h: void ns3::Ipv4::Send(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Address source, ns3::Ipv4Address destination, uint8_t protocol, ns3::Ptr<ns3::Ipv4Route> route) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'destination'), param('uint8_t', 'protocol'), param('ns3::Ptr< ns3::Ipv4Route >', 'route')], 
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

def register_Ns3Ipv4Interface_methods(root_module, cls):
    ## ipv4-interface.h: ns3::Ipv4Interface::Ipv4Interface(ns3::Ipv4Interface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Interface const &', 'arg0')])
    ## ipv4-interface.h: ns3::Ipv4Interface::Ipv4Interface() [constructor]
    cls.add_constructor([])
    ## ipv4-interface.h: bool ns3::Ipv4Interface::AddAddress(ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('AddAddress', 
                   'bool', 
                   [param('ns3::Ipv4InterfaceAddress', 'address')])
    ## ipv4-interface.h: ns3::Ipv4InterfaceAddress ns3::Ipv4Interface::GetAddress(uint32_t index) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4InterfaceAddress', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## ipv4-interface.h: ns3::Ptr<ns3::ArpCache> ns3::Ipv4Interface::GetArpCache() const [member function]
    cls.add_method('GetArpCache', 
                   'ns3::Ptr< ns3::ArpCache >', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv4Interface::GetDevice() const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: uint16_t ns3::Ipv4Interface::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: uint32_t ns3::Ipv4Interface::GetNAddresses() const [member function]
    cls.add_method('GetNAddresses', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: static ns3::TypeId ns3::Ipv4Interface::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-interface.h: bool ns3::Ipv4Interface::IsDown() const [member function]
    cls.add_method('IsDown', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: bool ns3::Ipv4Interface::IsForwarding() const [member function]
    cls.add_method('IsForwarding', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: bool ns3::Ipv4Interface::IsUp() const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: ns3::Ipv4InterfaceAddress ns3::Ipv4Interface::RemoveAddress(uint32_t index) [member function]
    cls.add_method('RemoveAddress', 
                   'ns3::Ipv4InterfaceAddress', 
                   [param('uint32_t', 'index')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::Send(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Address dest) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Address', 'dest')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetArpCache(ns3::Ptr<ns3::ArpCache> arg0) [member function]
    cls.add_method('SetArpCache', 
                   'void', 
                   [param('ns3::Ptr< ns3::ArpCache >', 'arg0')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetDown() [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetForwarding(bool val) [member function]
    cls.add_method('SetForwarding', 
                   'void', 
                   [param('bool', 'val')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetMetric(uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint16_t', 'metric')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetUp() [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [])
    ## ipv4-interface.h: void ns3::Ipv4Interface::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Ipv4L3Protocol_methods(root_module, cls):
    ## ipv4-l3-protocol.h: ns3::Ipv4L3Protocol::PROT_NUMBER [variable]
    cls.add_static_attribute('PROT_NUMBER', 'uint16_t const', is_const=True)
    ## ipv4-l3-protocol.h: static ns3::TypeId ns3::Ipv4L3Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-l3-protocol.h: ns3::Ipv4L3Protocol::Ipv4L3Protocol() [constructor]
    cls.add_constructor([])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetRoutingProtocol(ns3::Ptr<ns3::Ipv4RoutingProtocol> routingProtocol) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4RoutingProtocol >', 'routingProtocol')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4L3Protocol::GetRoutingProtocol() const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: ns3::Ptr<ns3::Socket> ns3::Ipv4L3Protocol::CreateRawSocket() [member function]
    cls.add_method('CreateRawSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::DeleteRawSocket(ns3::Ptr<ns3::Socket> socket) [member function]
    cls.add_method('DeleteRawSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Insert(ns3::Ptr<ns3::Ipv4L4Protocol> protocol) [member function]
    cls.add_method('Insert', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4L4Protocol >', 'protocol')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: ns3::Ptr<ns3::Ipv4L4Protocol> ns3::Ipv4L3Protocol::GetProtocol(int protocolNumber) const [member function]
    cls.add_method('GetProtocol', 
                   'ns3::Ptr< ns3::Ipv4L4Protocol >', 
                   [param('int', 'protocolNumber')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Remove(ns3::Ptr<ns3::Ipv4L4Protocol> protocol) [member function]
    cls.add_method('Remove', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4L4Protocol >', 'protocol')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetDefaultTtl(uint8_t ttl) [member function]
    cls.add_method('SetDefaultTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Receive(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::Packet const> p, uint16_t protocol, ns3::Address const & from, ns3::Address const & to, ns3::NetDevice::PacketType packetType) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Packet const >', 'p'), param('uint16_t', 'protocol'), param('ns3::Address const &', 'from'), param('ns3::Address const &', 'to'), param('ns3::NetDevice::PacketType', 'packetType')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Send(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Address source, ns3::Ipv4Address destination, uint8_t protocol, ns3::Ptr<ns3::Ipv4Route> route) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'destination'), param('uint8_t', 'protocol'), param('ns3::Ptr< ns3::Ipv4Route >', 'route')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SendWithHeader(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Header ipHeader, ns3::Ptr<ns3::Ipv4Route> route) [member function]
    cls.add_method('SendWithHeader', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Header', 'ipHeader'), param('ns3::Ptr< ns3::Ipv4Route >', 'route')])
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::AddInterface(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddInterface', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: ns3::Ptr<ns3::Ipv4Interface> ns3::Ipv4L3Protocol::GetInterface(uint32_t i) const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ptr< ns3::Ipv4Interface >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: int32_t ns3::Ipv4L3Protocol::GetInterfaceForAddress(ns3::Ipv4Address addr) const [member function]
    cls.add_method('GetInterfaceForAddress', 
                   'int32_t', 
                   [param('ns3::Ipv4Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: int32_t ns3::Ipv4L3Protocol::GetInterfaceForPrefix(ns3::Ipv4Address addr, ns3::Ipv4Mask mask) const [member function]
    cls.add_method('GetInterfaceForPrefix', 
                   'int32_t', 
                   [param('ns3::Ipv4Address', 'addr'), param('ns3::Ipv4Mask', 'mask')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: int32_t ns3::Ipv4L3Protocol::GetInterfaceForDevice(ns3::Ptr<const ns3::NetDevice> device) const [member function]
    cls.add_method('GetInterfaceForDevice', 
                   'int32_t', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::IsDestinationAddress(ns3::Ipv4Address address, uint32_t iif) const [member function]
    cls.add_method('IsDestinationAddress', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address'), param('uint32_t', 'iif')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::AddAddress(uint32_t i, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('AddAddress', 
                   'bool', 
                   [param('uint32_t', 'i'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: ns3::Ipv4InterfaceAddress ns3::Ipv4L3Protocol::GetAddress(uint32_t interfaceIndex, uint32_t addressIndex) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4InterfaceAddress', 
                   [param('uint32_t', 'interfaceIndex'), param('uint32_t', 'addressIndex')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::GetNAddresses(uint32_t interface) const [member function]
    cls.add_method('GetNAddresses', 
                   'uint32_t', 
                   [param('uint32_t', 'interface')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::RemoveAddress(uint32_t interfaceIndex, uint32_t addressIndex) [member function]
    cls.add_method('RemoveAddress', 
                   'bool', 
                   [param('uint32_t', 'interfaceIndex'), param('uint32_t', 'addressIndex')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: ns3::Ipv4Address ns3::Ipv4L3Protocol::SelectSourceAddress(ns3::Ptr<const ns3::NetDevice> device, ns3::Ipv4Address dst, ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e scope) [member function]
    cls.add_method('SelectSourceAddress', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device'), param('ns3::Ipv4Address', 'dst'), param('ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e', 'scope')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetMetric(uint32_t i, uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'i'), param('uint16_t', 'metric')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: uint16_t ns3::Ipv4L3Protocol::GetMetric(uint32_t i) const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: uint16_t ns3::Ipv4L3Protocol::GetMtu(uint32_t i) const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::IsUp(uint32_t i) const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetUp(uint32_t i) [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [param('uint32_t', 'i')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetDown(uint32_t i) [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [param('uint32_t', 'i')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::IsForwarding(uint32_t i) const [member function]
    cls.add_method('IsForwarding', 
                   'bool', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetForwarding(uint32_t i, bool val) [member function]
    cls.add_method('SetForwarding', 
                   'void', 
                   [param('uint32_t', 'i'), param('bool', 'val')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv4L3Protocol::GetNetDevice(uint32_t i) [member function]
    cls.add_method('GetNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetIpForward(bool forward) [member function]
    cls.add_method('SetIpForward', 
                   'void', 
                   [param('bool', 'forward')], 
                   visibility='private', is_virtual=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::GetIpForward() const [member function]
    cls.add_method('GetIpForward', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetWeakEsModel(bool model) [member function]
    cls.add_method('SetWeakEsModel', 
                   'void', 
                   [param('bool', 'model')], 
                   visibility='private', is_virtual=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::GetWeakEsModel() const [member function]
    cls.add_method('GetWeakEsModel', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3Ipv4L4Protocol_methods(root_module, cls):
    ## ipv4-l4-protocol.h: ns3::Ipv4L4Protocol::Ipv4L4Protocol() [constructor]
    cls.add_constructor([])
    ## ipv4-l4-protocol.h: ns3::Ipv4L4Protocol::Ipv4L4Protocol(ns3::Ipv4L4Protocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4L4Protocol const &', 'arg0')])
    ## ipv4-l4-protocol.h: ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr<ns3::Ipv4Route>, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::Ipv4L4Protocol::GetDownTarget() const [member function]
    cls.add_method('GetDownTarget', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4-l4-protocol.h: int ns3::Ipv4L4Protocol::GetProtocolNumber() const [member function]
    cls.add_method('GetProtocolNumber', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4-l4-protocol.h: static ns3::TypeId ns3::Ipv4L4Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-l4-protocol.h: ns3::Ipv4L4Protocol::RxStatus ns3::Ipv4L4Protocol::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::Ipv4Interface> incomingInterface) [member function]
    cls.add_method('Receive', 
                   'ns3::Ipv4L4Protocol::RxStatus', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::Ipv4Interface >', 'incomingInterface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4-l4-protocol.h: void ns3::Ipv4L4Protocol::ReceiveIcmp(ns3::Ipv4Address icmpSource, uint8_t icmpTtl, uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo, ns3::Ipv4Address payloadSource, ns3::Ipv4Address payloadDestination, uint8_t const * payload) [member function]
    cls.add_method('ReceiveIcmp', 
                   'void', 
                   [param('ns3::Ipv4Address', 'icmpSource'), param('uint8_t', 'icmpTtl'), param('uint8_t', 'icmpType'), param('uint8_t', 'icmpCode'), param('uint32_t', 'icmpInfo'), param('ns3::Ipv4Address', 'payloadSource'), param('ns3::Ipv4Address', 'payloadDestination'), param('uint8_t const *', 'payload')], 
                   is_virtual=True)
    ## ipv4-l4-protocol.h: void ns3::Ipv4L4Protocol::SetDownTarget(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr<ns3::Ipv4Route>, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetDownTarget', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
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

def register_Ns3Ipv4RawSocketImpl_methods(root_module, cls):
    ## ipv4-raw-socket-impl.h: ns3::Ipv4RawSocketImpl::Ipv4RawSocketImpl(ns3::Ipv4RawSocketImpl const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4RawSocketImpl const &', 'arg0')])
    ## ipv4-raw-socket-impl.h: ns3::Ipv4RawSocketImpl::Ipv4RawSocketImpl() [constructor]
    cls.add_constructor([])
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::Bind(ns3::Address const & address) [member function]
    cls.add_method('Bind', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::Bind() [member function]
    cls.add_method('Bind', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::Close() [member function]
    cls.add_method('Close', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::Connect(ns3::Address const & address) [member function]
    cls.add_method('Connect', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: bool ns3::Ipv4RawSocketImpl::ForwardUp(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header ipHeader, ns3::Ptr<ns3::Ipv4Interface> incomingInterface) [member function]
    cls.add_method('ForwardUp', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header', 'ipHeader'), param('ns3::Ptr< ns3::Ipv4Interface >', 'incomingInterface')])
    ## ipv4-raw-socket-impl.h: bool ns3::Ipv4RawSocketImpl::GetAllowBroadcast() const [member function]
    cls.add_method('GetAllowBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-raw-socket-impl.h: ns3::Socket::SocketErrno ns3::Ipv4RawSocketImpl::GetErrno() const [member function]
    cls.add_method('GetErrno', 
                   'ns3::Socket::SocketErrno', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-raw-socket-impl.h: ns3::Ptr<ns3::Node> ns3::Ipv4RawSocketImpl::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-raw-socket-impl.h: uint32_t ns3::Ipv4RawSocketImpl::GetRxAvailable() const [member function]
    cls.add_method('GetRxAvailable', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::GetSockName(ns3::Address & address) const [member function]
    cls.add_method('GetSockName', 
                   'int', 
                   [param('ns3::Address &', 'address')], 
                   is_const=True, is_virtual=True)
    ## ipv4-raw-socket-impl.h: ns3::Socket::SocketType ns3::Ipv4RawSocketImpl::GetSocketType() const [member function]
    cls.add_method('GetSocketType', 
                   'ns3::Socket::SocketType', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-raw-socket-impl.h: uint32_t ns3::Ipv4RawSocketImpl::GetTxAvailable() const [member function]
    cls.add_method('GetTxAvailable', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-raw-socket-impl.h: static ns3::TypeId ns3::Ipv4RawSocketImpl::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::Listen() [member function]
    cls.add_method('Listen', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: ns3::Ptr<ns3::Packet> ns3::Ipv4RawSocketImpl::Recv(uint32_t maxSize, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags')], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: ns3::Ptr<ns3::Packet> ns3::Ipv4RawSocketImpl::RecvFrom(uint32_t maxSize, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::Send(ns3::Ptr<ns3::Packet> p, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags')], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::SendTo(ns3::Ptr<ns3::Packet> p, uint32_t flags, ns3::Address const & toAddress) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags'), param('ns3::Address const &', 'toAddress')], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: bool ns3::Ipv4RawSocketImpl::SetAllowBroadcast(bool allowBroadcast) [member function]
    cls.add_method('SetAllowBroadcast', 
                   'bool', 
                   [param('bool', 'allowBroadcast')], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: void ns3::Ipv4RawSocketImpl::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ipv4-raw-socket-impl.h: void ns3::Ipv4RawSocketImpl::SetProtocol(uint16_t protocol) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint16_t', 'protocol')])
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::ShutdownRecv() [member function]
    cls.add_method('ShutdownRecv', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: int ns3::Ipv4RawSocketImpl::ShutdownSend() [member function]
    cls.add_method('ShutdownSend', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## ipv4-raw-socket-impl.h: void ns3::Ipv4RawSocketImpl::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
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
    ## ipv4-routing-protocol.h: void ns3::Ipv4RoutingProtocol::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
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

def register_Ns3Ipv6ExtensionAHHeader_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionAHHeader::Ipv6ExtensionAHHeader(ns3::Ipv6ExtensionAHHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ExtensionAHHeader const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionAHHeader::Ipv6ExtensionAHHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionAHHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-extension-header.h: ns3::TypeId ns3::Ipv6ExtensionAHHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionAHHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: static ns3::TypeId ns3::Ipv6ExtensionAHHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionAHHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionAHHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv6ExtensionDestinationHeader_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionDestinationHeader::Ipv6ExtensionDestinationHeader(ns3::Ipv6ExtensionDestinationHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ExtensionDestinationHeader const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionDestinationHeader::Ipv6ExtensionDestinationHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionDestinationHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-extension-header.h: ns3::TypeId ns3::Ipv6ExtensionDestinationHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionDestinationHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: static ns3::TypeId ns3::Ipv6ExtensionDestinationHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionDestinationHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionDestinationHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv6ExtensionESPHeader_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionESPHeader::Ipv6ExtensionESPHeader(ns3::Ipv6ExtensionESPHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ExtensionESPHeader const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionESPHeader::Ipv6ExtensionESPHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionESPHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-extension-header.h: ns3::TypeId ns3::Ipv6ExtensionESPHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionESPHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: static ns3::TypeId ns3::Ipv6ExtensionESPHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionESPHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionESPHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv6ExtensionFragmentHeader_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionFragmentHeader::Ipv6ExtensionFragmentHeader(ns3::Ipv6ExtensionFragmentHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ExtensionFragmentHeader const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionFragmentHeader::Ipv6ExtensionFragmentHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionFragmentHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionFragmentHeader::GetIdentification() const [member function]
    cls.add_method('GetIdentification', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: ns3::TypeId ns3::Ipv6ExtensionFragmentHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: bool ns3::Ipv6ExtensionFragmentHeader::GetMoreFragment() const [member function]
    cls.add_method('GetMoreFragment', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: uint16_t ns3::Ipv6ExtensionFragmentHeader::GetOffset() const [member function]
    cls.add_method('GetOffset', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionFragmentHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: static ns3::TypeId ns3::Ipv6ExtensionFragmentHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionFragmentHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionFragmentHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionFragmentHeader::SetIdentification(uint32_t identification) [member function]
    cls.add_method('SetIdentification', 
                   'void', 
                   [param('uint32_t', 'identification')])
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionFragmentHeader::SetMoreFragment(bool moreFragment) [member function]
    cls.add_method('SetMoreFragment', 
                   'void', 
                   [param('bool', 'moreFragment')])
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionFragmentHeader::SetOffset(uint16_t offset) [member function]
    cls.add_method('SetOffset', 
                   'void', 
                   [param('uint16_t', 'offset')])
    return

def register_Ns3Ipv6ExtensionLooseRoutingHeader_methods(root_module, cls):
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionLooseRoutingHeader::Ipv6ExtensionLooseRoutingHeader(ns3::Ipv6ExtensionLooseRoutingHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ExtensionLooseRoutingHeader const &', 'arg0')])
    ## ipv6-extension-header.h: ns3::Ipv6ExtensionLooseRoutingHeader::Ipv6ExtensionLooseRoutingHeader() [constructor]
    cls.add_constructor([])
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionLooseRoutingHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-extension-header.h: ns3::TypeId ns3::Ipv6ExtensionLooseRoutingHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: ns3::Ipv6Address ns3::Ipv6ExtensionLooseRoutingHeader::GetRouterAddress(uint8_t index) const [member function]
    cls.add_method('GetRouterAddress', 
                   'ns3::Ipv6Address', 
                   [param('uint8_t', 'index')], 
                   is_const=True)
    ## ipv6-extension-header.h: std::vector<ns3::Ipv6Address, std::allocator<ns3::Ipv6Address> > ns3::Ipv6ExtensionLooseRoutingHeader::GetRoutersAddress() const [member function]
    cls.add_method('GetRoutersAddress', 
                   'std::vector< ns3::Ipv6Address >', 
                   [], 
                   is_const=True)
    ## ipv6-extension-header.h: uint32_t ns3::Ipv6ExtensionLooseRoutingHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: static ns3::TypeId ns3::Ipv6ExtensionLooseRoutingHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionLooseRoutingHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionLooseRoutingHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionLooseRoutingHeader::SetNumberAddress(uint8_t n) [member function]
    cls.add_method('SetNumberAddress', 
                   'void', 
                   [param('uint8_t', 'n')])
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionLooseRoutingHeader::SetRouterAddress(uint8_t index, ns3::Ipv6Address addr) [member function]
    cls.add_method('SetRouterAddress', 
                   'void', 
                   [param('uint8_t', 'index'), param('ns3::Ipv6Address', 'addr')])
    ## ipv6-extension-header.h: void ns3::Ipv6ExtensionLooseRoutingHeader::SetRoutersAddress(std::vector<ns3::Ipv6Address, std::allocator<ns3::Ipv6Address> > routersAddress) [member function]
    cls.add_method('SetRoutersAddress', 
                   'void', 
                   [param('std::vector< ns3::Ipv6Address >', 'routersAddress')])
    return

def register_Ns3Ipv6Interface_methods(root_module, cls):
    ## ipv6-interface.h: ns3::Ipv6Interface::Ipv6Interface(ns3::Ipv6Interface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Interface const &', 'arg0')])
    ## ipv6-interface.h: ns3::Ipv6Interface::Ipv6Interface() [constructor]
    cls.add_constructor([])
    ## ipv6-interface.h: bool ns3::Ipv6Interface::AddAddress(ns3::Ipv6InterfaceAddress iface) [member function]
    cls.add_method('AddAddress', 
                   'bool', 
                   [param('ns3::Ipv6InterfaceAddress', 'iface')])
    ## ipv6-interface.h: ns3::Ipv6InterfaceAddress ns3::Ipv6Interface::GetAddress(uint32_t index) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv6InterfaceAddress', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## ipv6-interface.h: ns3::Ipv6InterfaceAddress ns3::Ipv6Interface::GetAddressMatchingDestination(ns3::Ipv6Address dst) [member function]
    cls.add_method('GetAddressMatchingDestination', 
                   'ns3::Ipv6InterfaceAddress', 
                   [param('ns3::Ipv6Address', 'dst')])
    ## ipv6-interface.h: uint16_t ns3::Ipv6Interface::GetBaseReachableTime() const [member function]
    cls.add_method('GetBaseReachableTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: uint8_t ns3::Ipv6Interface::GetCurHopLimit() const [member function]
    cls.add_method('GetCurHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv6Interface::GetDevice() const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-interface.h: ns3::Ipv6InterfaceAddress ns3::Ipv6Interface::GetLinkLocalAddress() const [member function]
    cls.add_method('GetLinkLocalAddress', 
                   'ns3::Ipv6InterfaceAddress', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: uint16_t ns3::Ipv6Interface::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: uint32_t ns3::Ipv6Interface::GetNAddresses() const [member function]
    cls.add_method('GetNAddresses', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: uint16_t ns3::Ipv6Interface::GetReachableTime() const [member function]
    cls.add_method('GetReachableTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: uint16_t ns3::Ipv6Interface::GetRetransTimer() const [member function]
    cls.add_method('GetRetransTimer', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: static ns3::TypeId ns3::Ipv6Interface::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-interface.h: bool ns3::Ipv6Interface::IsDown() const [member function]
    cls.add_method('IsDown', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: bool ns3::Ipv6Interface::IsForwarding() const [member function]
    cls.add_method('IsForwarding', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: bool ns3::Ipv6Interface::IsUp() const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h: ns3::Ipv6InterfaceAddress ns3::Ipv6Interface::RemoveAddress(uint32_t index) [member function]
    cls.add_method('RemoveAddress', 
                   'ns3::Ipv6InterfaceAddress', 
                   [param('uint32_t', 'index')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::Send(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Address dest) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Address', 'dest')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetBaseReachableTime(uint16_t baseReachableTime) [member function]
    cls.add_method('SetBaseReachableTime', 
                   'void', 
                   [param('uint16_t', 'baseReachableTime')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetCurHopLimit(uint8_t curHopLimit) [member function]
    cls.add_method('SetCurHopLimit', 
                   'void', 
                   [param('uint8_t', 'curHopLimit')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetDown() [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetForwarding(bool forward) [member function]
    cls.add_method('SetForwarding', 
                   'void', 
                   [param('bool', 'forward')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetMetric(uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint16_t', 'metric')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetNsDadUid(ns3::Ipv6Address address, uint32_t uid) [member function]
    cls.add_method('SetNsDadUid', 
                   'void', 
                   [param('ns3::Ipv6Address', 'address'), param('uint32_t', 'uid')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetReachableTime(uint16_t reachableTime) [member function]
    cls.add_method('SetReachableTime', 
                   'void', 
                   [param('uint16_t', 'reachableTime')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetRetransTimer(uint16_t retransTimer) [member function]
    cls.add_method('SetRetransTimer', 
                   'void', 
                   [param('uint16_t', 'retransTimer')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetState(ns3::Ipv6Address address, ns3::Ipv6InterfaceAddress::State_e state) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::Ipv6Address', 'address'), param('ns3::Ipv6InterfaceAddress::State_e', 'state')])
    ## ipv6-interface.h: void ns3::Ipv6Interface::SetUp() [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [])
    ## ipv6-interface.h: void ns3::Ipv6Interface::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Ipv6L3Protocol_methods(root_module, cls):
    ## ipv6-l3-protocol.h: ns3::Ipv6L3Protocol::PROT_NUMBER [variable]
    cls.add_static_attribute('PROT_NUMBER', 'uint16_t const', is_const=True)
    ## ipv6-l3-protocol.h: static ns3::TypeId ns3::Ipv6L3Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-l3-protocol.h: ns3::Ipv6L3Protocol::Ipv6L3Protocol() [constructor]
    cls.add_constructor([])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::Insert(ns3::Ptr<ns3::Ipv6L4Protocol> protocol) [member function]
    cls.add_method('Insert', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6L4Protocol >', 'protocol')])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::Remove(ns3::Ptr<ns3::Ipv6L4Protocol> protocol) [member function]
    cls.add_method('Remove', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6L4Protocol >', 'protocol')])
    ## ipv6-l3-protocol.h: ns3::Ptr<ns3::Ipv6L4Protocol> ns3::Ipv6L3Protocol::GetProtocol(int protocolNumber) const [member function]
    cls.add_method('GetProtocol', 
                   'ns3::Ptr< ns3::Ipv6L4Protocol >', 
                   [param('int', 'protocolNumber')], 
                   is_const=True)
    ## ipv6-l3-protocol.h: ns3::Ptr<ns3::Socket> ns3::Ipv6L3Protocol::CreateRawSocket() [member function]
    cls.add_method('CreateRawSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::DeleteRawSocket(ns3::Ptr<ns3::Socket> socket) [member function]
    cls.add_method('DeleteRawSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket')])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::SetDefaultTtl(uint8_t ttl) [member function]
    cls.add_method('SetDefaultTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::Receive(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::Packet const> p, uint16_t protocol, ns3::Address const & from, ns3::Address const & to, ns3::NetDevice::PacketType packetType) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Packet const >', 'p'), param('uint16_t', 'protocol'), param('ns3::Address const &', 'from'), param('ns3::Address const &', 'to'), param('ns3::NetDevice::PacketType', 'packetType')])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::Send(ns3::Ptr<ns3::Packet> packet, ns3::Ipv6Address source, ns3::Ipv6Address destination, uint8_t protocol, ns3::Ptr<ns3::Ipv6Route> route) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv6Address', 'source'), param('ns3::Ipv6Address', 'destination'), param('uint8_t', 'protocol'), param('ns3::Ptr< ns3::Ipv6Route >', 'route')])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::SetRoutingProtocol(ns3::Ptr<ns3::Ipv6RoutingProtocol> routingProtocol) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6RoutingProtocol >', 'routingProtocol')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: ns3::Ptr<ns3::Ipv6RoutingProtocol> ns3::Ipv6L3Protocol::GetRoutingProtocol() const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv6RoutingProtocol >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: uint32_t ns3::Ipv6L3Protocol::AddInterface(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddInterface', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: ns3::Ptr<ns3::Ipv6Interface> ns3::Ipv6L3Protocol::GetInterface(uint32_t i) const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ptr< ns3::Ipv6Interface >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv6-l3-protocol.h: uint32_t ns3::Ipv6L3Protocol::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: int32_t ns3::Ipv6L3Protocol::GetInterfaceForAddress(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetInterfaceForAddress', 
                   'int32_t', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: int32_t ns3::Ipv6L3Protocol::GetInterfaceForPrefix(ns3::Ipv6Address addr, ns3::Ipv6Prefix mask) const [member function]
    cls.add_method('GetInterfaceForPrefix', 
                   'int32_t', 
                   [param('ns3::Ipv6Address', 'addr'), param('ns3::Ipv6Prefix', 'mask')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: int32_t ns3::Ipv6L3Protocol::GetInterfaceForDevice(ns3::Ptr<const ns3::NetDevice> device) const [member function]
    cls.add_method('GetInterfaceForDevice', 
                   'int32_t', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: bool ns3::Ipv6L3Protocol::AddAddress(uint32_t i, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('AddAddress', 
                   'bool', 
                   [param('uint32_t', 'i'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: ns3::Ipv6InterfaceAddress ns3::Ipv6L3Protocol::GetAddress(uint32_t interfaceIndex, uint32_t addressIndex) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv6InterfaceAddress', 
                   [param('uint32_t', 'interfaceIndex'), param('uint32_t', 'addressIndex')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: uint32_t ns3::Ipv6L3Protocol::GetNAddresses(uint32_t interface) const [member function]
    cls.add_method('GetNAddresses', 
                   'uint32_t', 
                   [param('uint32_t', 'interface')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: bool ns3::Ipv6L3Protocol::RemoveAddress(uint32_t interfaceIndex, uint32_t addressIndex) [member function]
    cls.add_method('RemoveAddress', 
                   'bool', 
                   [param('uint32_t', 'interfaceIndex'), param('uint32_t', 'addressIndex')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::SetMetric(uint32_t i, uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'i'), param('uint16_t', 'metric')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: uint16_t ns3::Ipv6L3Protocol::GetMetric(uint32_t i) const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: uint16_t ns3::Ipv6L3Protocol::GetMtu(uint32_t i) const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: bool ns3::Ipv6L3Protocol::IsUp(uint32_t i) const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::SetUp(uint32_t i) [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [param('uint32_t', 'i')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::SetDown(uint32_t i) [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [param('uint32_t', 'i')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: bool ns3::Ipv6L3Protocol::IsForwarding(uint32_t i) const [member function]
    cls.add_method('IsForwarding', 
                   'bool', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::SetForwarding(uint32_t i, bool val) [member function]
    cls.add_method('SetForwarding', 
                   'void', 
                   [param('uint32_t', 'i'), param('bool', 'val')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv6L3Protocol::GetNetDevice(uint32_t i) [member function]
    cls.add_method('GetNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: ns3::Ptr<ns3::Icmpv6L4Protocol> ns3::Ipv6L3Protocol::GetIcmpv6() const [member function]
    cls.add_method('GetIcmpv6', 
                   'ns3::Ptr< ns3::Icmpv6L4Protocol >', 
                   [], 
                   is_const=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::AddAutoconfiguredAddress(uint32_t interface, ns3::Ipv6Address network, ns3::Ipv6Prefix mask, uint8_t flags, uint32_t validTime, uint32_t preferredTime, ns3::Ipv6Address defaultRouter=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('AddAutoconfiguredAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'mask'), param('uint8_t', 'flags'), param('uint32_t', 'validTime'), param('uint32_t', 'preferredTime'), param('ns3::Ipv6Address', 'defaultRouter', default_value='ns3::Ipv6Address::GetZero( )')])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::RemoveAutoconfiguredAddress(uint32_t interface, ns3::Ipv6Address network, ns3::Ipv6Prefix mask, ns3::Ipv6Address defaultRouter) [member function]
    cls.add_method('RemoveAutoconfiguredAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'network'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'defaultRouter')])
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::RegisterExtensions() [member function]
    cls.add_method('RegisterExtensions', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::RegisterOptions() [member function]
    cls.add_method('RegisterOptions', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ipv6-l3-protocol.h: void ns3::Ipv6L3Protocol::SetIpForward(bool forward) [member function]
    cls.add_method('SetIpForward', 
                   'void', 
                   [param('bool', 'forward')], 
                   visibility='private', is_virtual=True)
    ## ipv6-l3-protocol.h: bool ns3::Ipv6L3Protocol::GetIpForward() const [member function]
    cls.add_method('GetIpForward', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3Ipv6L4Protocol_methods(root_module, cls):
    ## ipv6-l4-protocol.h: ns3::Ipv6L4Protocol::Ipv6L4Protocol() [constructor]
    cls.add_constructor([])
    ## ipv6-l4-protocol.h: ns3::Ipv6L4Protocol::Ipv6L4Protocol(ns3::Ipv6L4Protocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6L4Protocol const &', 'arg0')])
    ## ipv6-l4-protocol.h: int ns3::Ipv6L4Protocol::GetProtocolNumber() const [member function]
    cls.add_method('GetProtocolNumber', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv6-l4-protocol.h: static ns3::TypeId ns3::Ipv6L4Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-l4-protocol.h: ns3::Ipv6L4Protocol::RxStatus_e ns3::Ipv6L4Protocol::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Address const & src, ns3::Ipv6Address const & dst, ns3::Ptr<ns3::Ipv6Interface> incomingInterface) [member function]
    cls.add_method('Receive', 
                   'ns3::Ipv6L4Protocol::RxStatus_e', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Address const &', 'src'), param('ns3::Ipv6Address const &', 'dst'), param('ns3::Ptr< ns3::Ipv6Interface >', 'incomingInterface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv6-l4-protocol.h: void ns3::Ipv6L4Protocol::ReceiveIcmp(ns3::Ipv6Address icmpSource, uint8_t icmpTtl, uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo, ns3::Ipv6Address payloadSource, ns3::Ipv6Address payloadDestination, uint8_t const * payload) [member function]
    cls.add_method('ReceiveIcmp', 
                   'void', 
                   [param('ns3::Ipv6Address', 'icmpSource'), param('uint8_t', 'icmpTtl'), param('uint8_t', 'icmpType'), param('uint8_t', 'icmpCode'), param('uint32_t', 'icmpInfo'), param('ns3::Ipv6Address', 'payloadSource'), param('ns3::Ipv6Address', 'payloadDestination'), param('uint8_t const *', 'payload')], 
                   is_virtual=True)
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

def register_Ns3NdiscCache_methods(root_module, cls):
    ## ndisc-cache.h: ns3::NdiscCache::NdiscCache() [constructor]
    cls.add_constructor([])
    ## ndisc-cache.h: ns3::NdiscCache::Entry * ns3::NdiscCache::Add(ns3::Ipv6Address to) [member function]
    cls.add_method('Add', 
                   'ns3::NdiscCache::Entry *', 
                   [param('ns3::Ipv6Address', 'to')])
    ## ndisc-cache.h: void ns3::NdiscCache::Flush() [member function]
    cls.add_method('Flush', 
                   'void', 
                   [])
    ## ndisc-cache.h: ns3::Ptr<ns3::NetDevice> ns3::NdiscCache::GetDevice() const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: ns3::Ptr<ns3::Ipv6Interface> ns3::NdiscCache::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ptr< ns3::Ipv6Interface >', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: static ns3::TypeId ns3::NdiscCache::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndisc-cache.h: uint32_t ns3::NdiscCache::GetUnresQlen() [member function]
    cls.add_method('GetUnresQlen', 
                   'uint32_t', 
                   [])
    ## ndisc-cache.h: ns3::NdiscCache::Entry * ns3::NdiscCache::Lookup(ns3::Ipv6Address dst) [member function]
    cls.add_method('Lookup', 
                   'ns3::NdiscCache::Entry *', 
                   [param('ns3::Ipv6Address', 'dst')])
    ## ndisc-cache.h: void ns3::NdiscCache::Remove(ns3::NdiscCache::Entry * entry) [member function]
    cls.add_method('Remove', 
                   'void', 
                   [param('ns3::NdiscCache::Entry *', 'entry')])
    ## ndisc-cache.h: void ns3::NdiscCache::SetDevice(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::Ipv6Interface> interface) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Ipv6Interface >', 'interface')])
    ## ndisc-cache.h: void ns3::NdiscCache::SetUnresQlen(uint32_t unresQlen) [member function]
    cls.add_method('SetUnresQlen', 
                   'void', 
                   [param('uint32_t', 'unresQlen')])
    ## ndisc-cache.h: ns3::NdiscCache::DEFAULT_UNRES_QLEN [variable]
    cls.add_static_attribute('DEFAULT_UNRES_QLEN', 'uint32_t const', is_const=True)
    ## ndisc-cache.h: void ns3::NdiscCache::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3NdiscCacheEntry_methods(root_module, cls):
    ## ndisc-cache.h: ns3::NdiscCache::Entry::Entry(ns3::NdiscCache::Entry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NdiscCache::Entry const &', 'arg0')])
    ## ndisc-cache.h: ns3::NdiscCache::Entry::Entry(ns3::NdiscCache * nd) [constructor]
    cls.add_constructor([param('ns3::NdiscCache *', 'nd')])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::AddWaitingPacket(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('AddWaitingPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::ClearWaitingPacket() [member function]
    cls.add_method('ClearWaitingPacket', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::FunctionDelayTimeout() [member function]
    cls.add_method('FunctionDelayTimeout', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::FunctionProbeTimeout() [member function]
    cls.add_method('FunctionProbeTimeout', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::FunctionReachableTimeout() [member function]
    cls.add_method('FunctionReachableTimeout', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::FunctionRetransmitTimeout() [member function]
    cls.add_method('FunctionRetransmitTimeout', 
                   'void', 
                   [])
    ## ndisc-cache.h: ns3::Time ns3::NdiscCache::Entry::GetLastReachabilityConfirmation() const [member function]
    cls.add_method('GetLastReachabilityConfirmation', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: ns3::Address ns3::NdiscCache::Entry::GetMacAddress() const [member function]
    cls.add_method('GetMacAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: uint8_t ns3::NdiscCache::Entry::GetNSRetransmit() const [member function]
    cls.add_method('GetNSRetransmit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::IncNSRetransmit() [member function]
    cls.add_method('IncNSRetransmit', 
                   'void', 
                   [])
    ## ndisc-cache.h: bool ns3::NdiscCache::Entry::IsDelay() const [member function]
    cls.add_method('IsDelay', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: bool ns3::NdiscCache::Entry::IsIncomplete() const [member function]
    cls.add_method('IsIncomplete', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: bool ns3::NdiscCache::Entry::IsProbe() const [member function]
    cls.add_method('IsProbe', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: bool ns3::NdiscCache::Entry::IsReachable() const [member function]
    cls.add_method('IsReachable', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: bool ns3::NdiscCache::Entry::IsRouter() const [member function]
    cls.add_method('IsRouter', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: bool ns3::NdiscCache::Entry::IsStale() const [member function]
    cls.add_method('IsStale', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::MarkDelay() [member function]
    cls.add_method('MarkDelay', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::MarkIncomplete(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('MarkIncomplete', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::MarkProbe() [member function]
    cls.add_method('MarkProbe', 
                   'void', 
                   [])
    ## ndisc-cache.h: std::list<ns3::Ptr<ns3::Packet>, std::allocator<ns3::Ptr<ns3::Packet> > > ns3::NdiscCache::Entry::MarkReachable(ns3::Address mac) [member function]
    cls.add_method('MarkReachable', 
                   'std::list< ns3::Ptr< ns3::Packet > >', 
                   [param('ns3::Address', 'mac')])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::MarkReachable() [member function]
    cls.add_method('MarkReachable', 
                   'void', 
                   [])
    ## ndisc-cache.h: std::list<ns3::Ptr<ns3::Packet>, std::allocator<ns3::Ptr<ns3::Packet> > > ns3::NdiscCache::Entry::MarkStale(ns3::Address mac) [member function]
    cls.add_method('MarkStale', 
                   'std::list< ns3::Ptr< ns3::Packet > >', 
                   [param('ns3::Address', 'mac')])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::MarkStale() [member function]
    cls.add_method('MarkStale', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::ResetNSRetransmit() [member function]
    cls.add_method('ResetNSRetransmit', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::SetIpv6Address(ns3::Ipv6Address ipv6Address) [member function]
    cls.add_method('SetIpv6Address', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ipv6Address')])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::SetMacAddress(ns3::Address mac) [member function]
    cls.add_method('SetMacAddress', 
                   'void', 
                   [param('ns3::Address', 'mac')])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::SetRouter(bool router) [member function]
    cls.add_method('SetRouter', 
                   'void', 
                   [param('bool', 'router')])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::StartDelayTimer() [member function]
    cls.add_method('StartDelayTimer', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::StartProbeTimer() [member function]
    cls.add_method('StartProbeTimer', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::StartReachableTimer() [member function]
    cls.add_method('StartReachableTimer', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::StartRetransmitTimer() [member function]
    cls.add_method('StartRetransmitTimer', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::StopDelayTimer() [member function]
    cls.add_method('StopDelayTimer', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::StopProbeTimer() [member function]
    cls.add_method('StopProbeTimer', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::StopReachableTimer() [member function]
    cls.add_method('StopReachableTimer', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::StopRetransmitTimer() [member function]
    cls.add_method('StopRetransmitTimer', 
                   'void', 
                   [])
    ## ndisc-cache.h: void ns3::NdiscCache::Entry::UpdateLastReachabilityconfirmation() [member function]
    cls.add_method('UpdateLastReachabilityconfirmation', 
                   'void', 
                   [])
    return

def register_Ns3TcpL4Protocol_methods(root_module, cls):
    ## tcp-l4-protocol.h: ns3::TcpL4Protocol::PROT_NUMBER [variable]
    cls.add_static_attribute('PROT_NUMBER', 'uint8_t const', is_const=True)
    ## tcp-l4-protocol.h: static ns3::TypeId ns3::TcpL4Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tcp-l4-protocol.h: ns3::TcpL4Protocol::TcpL4Protocol() [constructor]
    cls.add_constructor([])
    ## tcp-l4-protocol.h: void ns3::TcpL4Protocol::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## tcp-l4-protocol.h: int ns3::TcpL4Protocol::GetProtocolNumber() const [member function]
    cls.add_method('GetProtocolNumber', 
                   'int', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tcp-l4-protocol.h: ns3::Ptr<ns3::Socket> ns3::TcpL4Protocol::CreateSocket() [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [])
    ## tcp-l4-protocol.h: ns3::Ptr<ns3::Socket> ns3::TcpL4Protocol::CreateSocket(ns3::TypeId socketTypeId) [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [param('ns3::TypeId', 'socketTypeId')])
    ## tcp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::TcpL4Protocol::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [])
    ## tcp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::TcpL4Protocol::Allocate(ns3::Ipv4Address address) [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [param('ns3::Ipv4Address', 'address')])
    ## tcp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::TcpL4Protocol::Allocate(uint16_t port) [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [param('uint16_t', 'port')])
    ## tcp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::TcpL4Protocol::Allocate(ns3::Ipv4Address address, uint16_t port) [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [param('ns3::Ipv4Address', 'address'), param('uint16_t', 'port')])
    ## tcp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::TcpL4Protocol::Allocate(ns3::Ipv4Address localAddress, uint16_t localPort, ns3::Ipv4Address peerAddress, uint16_t peerPort) [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [param('ns3::Ipv4Address', 'localAddress'), param('uint16_t', 'localPort'), param('ns3::Ipv4Address', 'peerAddress'), param('uint16_t', 'peerPort')])
    ## tcp-l4-protocol.h: void ns3::TcpL4Protocol::DeAllocate(ns3::Ipv4EndPoint * endPoint) [member function]
    cls.add_method('DeAllocate', 
                   'void', 
                   [param('ns3::Ipv4EndPoint *', 'endPoint')])
    ## tcp-l4-protocol.h: void ns3::TcpL4Protocol::Send(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Address saddr, ns3::Ipv4Address daddr, uint16_t sport, uint16_t dport, ns3::Ptr<ns3::NetDevice> oif=0) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Address', 'saddr'), param('ns3::Ipv4Address', 'daddr'), param('uint16_t', 'sport'), param('uint16_t', 'dport'), param('ns3::Ptr< ns3::NetDevice >', 'oif', default_value='0')])
    ## tcp-l4-protocol.h: ns3::Ipv4L4Protocol::RxStatus ns3::TcpL4Protocol::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::Ipv4Interface> incomingInterface) [member function]
    cls.add_method('Receive', 
                   'ns3::Ipv4L4Protocol::RxStatus', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::Ipv4Interface >', 'incomingInterface')], 
                   is_virtual=True)
    ## tcp-l4-protocol.h: void ns3::TcpL4Protocol::SetDownTarget(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr<ns3::Ipv4Route>, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetDownTarget', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## tcp-l4-protocol.h: ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr<ns3::Ipv4Route>, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::TcpL4Protocol::GetDownTarget() const [member function]
    cls.add_method('GetDownTarget', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tcp-l4-protocol.h: void ns3::TcpL4Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## tcp-l4-protocol.h: void ns3::TcpL4Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3UdpL4Protocol_methods(root_module, cls):
    ## udp-l4-protocol.h: ns3::UdpL4Protocol::PROT_NUMBER [variable]
    cls.add_static_attribute('PROT_NUMBER', 'uint8_t const', is_const=True)
    ## udp-l4-protocol.h: static ns3::TypeId ns3::UdpL4Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-l4-protocol.h: ns3::UdpL4Protocol::UdpL4Protocol() [constructor]
    cls.add_constructor([])
    ## udp-l4-protocol.h: void ns3::UdpL4Protocol::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## udp-l4-protocol.h: int ns3::UdpL4Protocol::GetProtocolNumber() const [member function]
    cls.add_method('GetProtocolNumber', 
                   'int', 
                   [], 
                   is_const=True, is_virtual=True)
    ## udp-l4-protocol.h: ns3::Ptr<ns3::Socket> ns3::UdpL4Protocol::CreateSocket() [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [])
    ## udp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::UdpL4Protocol::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [])
    ## udp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::UdpL4Protocol::Allocate(ns3::Ipv4Address address) [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [param('ns3::Ipv4Address', 'address')])
    ## udp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::UdpL4Protocol::Allocate(uint16_t port) [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [param('uint16_t', 'port')])
    ## udp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::UdpL4Protocol::Allocate(ns3::Ipv4Address address, uint16_t port) [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [param('ns3::Ipv4Address', 'address'), param('uint16_t', 'port')])
    ## udp-l4-protocol.h: ns3::Ipv4EndPoint * ns3::UdpL4Protocol::Allocate(ns3::Ipv4Address localAddress, uint16_t localPort, ns3::Ipv4Address peerAddress, uint16_t peerPort) [member function]
    cls.add_method('Allocate', 
                   'ns3::Ipv4EndPoint *', 
                   [param('ns3::Ipv4Address', 'localAddress'), param('uint16_t', 'localPort'), param('ns3::Ipv4Address', 'peerAddress'), param('uint16_t', 'peerPort')])
    ## udp-l4-protocol.h: void ns3::UdpL4Protocol::DeAllocate(ns3::Ipv4EndPoint * endPoint) [member function]
    cls.add_method('DeAllocate', 
                   'void', 
                   [param('ns3::Ipv4EndPoint *', 'endPoint')])
    ## udp-l4-protocol.h: void ns3::UdpL4Protocol::Send(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Address saddr, ns3::Ipv4Address daddr, uint16_t sport, uint16_t dport) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Address', 'saddr'), param('ns3::Ipv4Address', 'daddr'), param('uint16_t', 'sport'), param('uint16_t', 'dport')])
    ## udp-l4-protocol.h: void ns3::UdpL4Protocol::Send(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Address saddr, ns3::Ipv4Address daddr, uint16_t sport, uint16_t dport, ns3::Ptr<ns3::Ipv4Route> route) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Address', 'saddr'), param('ns3::Ipv4Address', 'daddr'), param('uint16_t', 'sport'), param('uint16_t', 'dport'), param('ns3::Ptr< ns3::Ipv4Route >', 'route')])
    ## udp-l4-protocol.h: ns3::Ipv4L4Protocol::RxStatus ns3::UdpL4Protocol::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::Ipv4Interface> interface) [member function]
    cls.add_method('Receive', 
                   'ns3::Ipv4L4Protocol::RxStatus', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::Ipv4Interface >', 'interface')], 
                   is_virtual=True)
    ## udp-l4-protocol.h: void ns3::UdpL4Protocol::ReceiveIcmp(ns3::Ipv4Address icmpSource, uint8_t icmpTtl, uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo, ns3::Ipv4Address payloadSource, ns3::Ipv4Address payloadDestination, uint8_t const * payload) [member function]
    cls.add_method('ReceiveIcmp', 
                   'void', 
                   [param('ns3::Ipv4Address', 'icmpSource'), param('uint8_t', 'icmpTtl'), param('uint8_t', 'icmpType'), param('uint8_t', 'icmpCode'), param('uint32_t', 'icmpInfo'), param('ns3::Ipv4Address', 'payloadSource'), param('ns3::Ipv4Address', 'payloadDestination'), param('uint8_t const *', 'payload')], 
                   is_virtual=True)
    ## udp-l4-protocol.h: void ns3::UdpL4Protocol::SetDownTarget(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr<ns3::Ipv4Route>, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetDownTarget', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## udp-l4-protocol.h: ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr<ns3::Ipv4Route>, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::UdpL4Protocol::GetDownTarget() const [member function]
    cls.add_method('GetDownTarget', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## udp-l4-protocol.h: void ns3::UdpL4Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-l4-protocol.h: void ns3::UdpL4Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Icmpv4L4Protocol_methods(root_module, cls):
    ## icmpv4-l4-protocol.h: ns3::Icmpv4L4Protocol::Icmpv4L4Protocol(ns3::Icmpv4L4Protocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv4L4Protocol const &', 'arg0')])
    ## icmpv4-l4-protocol.h: ns3::Icmpv4L4Protocol::Icmpv4L4Protocol() [constructor]
    cls.add_constructor([])
    ## icmpv4-l4-protocol.h: ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr<ns3::Ipv4Route>, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::Icmpv4L4Protocol::GetDownTarget() const [member function]
    cls.add_method('GetDownTarget', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv4-l4-protocol.h: int ns3::Icmpv4L4Protocol::GetProtocolNumber() const [member function]
    cls.add_method('GetProtocolNumber', 
                   'int', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv4-l4-protocol.h: static uint16_t ns3::Icmpv4L4Protocol::GetStaticProtocolNumber() [member function]
    cls.add_method('GetStaticProtocolNumber', 
                   'uint16_t', 
                   [], 
                   is_static=True)
    ## icmpv4-l4-protocol.h: static ns3::TypeId ns3::Icmpv4L4Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv4-l4-protocol.h: ns3::Ipv4L4Protocol::RxStatus ns3::Icmpv4L4Protocol::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::Ipv4Interface> incomingInterface) [member function]
    cls.add_method('Receive', 
                   'ns3::Ipv4L4Protocol::RxStatus', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::Ipv4Interface >', 'incomingInterface')], 
                   is_virtual=True)
    ## icmpv4-l4-protocol.h: void ns3::Icmpv4L4Protocol::SendDestUnreachFragNeeded(ns3::Ipv4Header header, ns3::Ptr<ns3::Packet const> orgData, uint16_t nextHopMtu) [member function]
    cls.add_method('SendDestUnreachFragNeeded', 
                   'void', 
                   [param('ns3::Ipv4Header', 'header'), param('ns3::Ptr< ns3::Packet const >', 'orgData'), param('uint16_t', 'nextHopMtu')])
    ## icmpv4-l4-protocol.h: void ns3::Icmpv4L4Protocol::SendDestUnreachPort(ns3::Ipv4Header header, ns3::Ptr<ns3::Packet const> orgData) [member function]
    cls.add_method('SendDestUnreachPort', 
                   'void', 
                   [param('ns3::Ipv4Header', 'header'), param('ns3::Ptr< ns3::Packet const >', 'orgData')])
    ## icmpv4-l4-protocol.h: void ns3::Icmpv4L4Protocol::SendTimeExceededTtl(ns3::Ipv4Header header, ns3::Ptr<ns3::Packet const> orgData) [member function]
    cls.add_method('SendTimeExceededTtl', 
                   'void', 
                   [param('ns3::Ipv4Header', 'header'), param('ns3::Ptr< ns3::Packet const >', 'orgData')])
    ## icmpv4-l4-protocol.h: void ns3::Icmpv4L4Protocol::SetDownTarget(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr<ns3::Ipv4Route>, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetDownTarget', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## icmpv4-l4-protocol.h: void ns3::Icmpv4L4Protocol::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## icmpv4-l4-protocol.h: ns3::Icmpv4L4Protocol::PROT_NUMBER [variable]
    cls.add_static_attribute('PROT_NUMBER', 'uint8_t const', is_const=True)
    ## icmpv4-l4-protocol.h: void ns3::Icmpv4L4Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## icmpv4-l4-protocol.h: void ns3::Icmpv4L4Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Icmpv6L4Protocol_methods(root_module, cls):
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::Icmpv6L4Protocol(ns3::Icmpv6L4Protocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Icmpv6L4Protocol const &', 'arg0')])
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::Icmpv6L4Protocol() [constructor]
    cls.add_constructor([])
    ## icmpv6-l4-protocol.h: ns3::Ptr<ns3::NdiscCache> ns3::Icmpv6L4Protocol::CreateCache(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::Ipv6Interface> interface) [member function]
    cls.add_method('CreateCache', 
                   'ns3::Ptr< ns3::NdiscCache >', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Ipv6Interface >', 'interface')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::DoDAD(ns3::Ipv6Address target, ns3::Ptr<ns3::Ipv6Interface> interface) [member function]
    cls.add_method('DoDAD', 
                   'void', 
                   [param('ns3::Ipv6Address', 'target'), param('ns3::Ptr< ns3::Ipv6Interface >', 'interface')])
    ## icmpv6-l4-protocol.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6L4Protocol::ForgeEchoRequest(ns3::Ipv6Address src, ns3::Ipv6Address dst, uint16_t id, uint16_t seq, ns3::Ptr<ns3::Packet> data) [member function]
    cls.add_method('ForgeEchoRequest', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('uint16_t', 'id'), param('uint16_t', 'seq'), param('ns3::Ptr< ns3::Packet >', 'data')])
    ## icmpv6-l4-protocol.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6L4Protocol::ForgeNA(ns3::Ipv6Address src, ns3::Ipv6Address dst, ns3::Address * hardwareAddress, uint8_t flags) [member function]
    cls.add_method('ForgeNA', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('ns3::Address *', 'hardwareAddress'), param('uint8_t', 'flags')])
    ## icmpv6-l4-protocol.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6L4Protocol::ForgeNS(ns3::Ipv6Address src, ns3::Ipv6Address dst, ns3::Ipv6Address target, ns3::Address hardwareAddress) [member function]
    cls.add_method('ForgeNS', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Address', 'target'), param('ns3::Address', 'hardwareAddress')])
    ## icmpv6-l4-protocol.h: ns3::Ptr<ns3::Packet> ns3::Icmpv6L4Protocol::ForgeRS(ns3::Ipv6Address src, ns3::Ipv6Address dst, ns3::Address hardwareAddress) [member function]
    cls.add_method('ForgeRS', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('ns3::Address', 'hardwareAddress')])
    ## icmpv6-l4-protocol.h: static void ns3::Icmpv6L4Protocol::FunctionDadTimeout(ns3::Ptr<ns3::Icmpv6L4Protocol> icmpv6, ns3::Ipv6Interface * interface, ns3::Ipv6Address addr) [member function]
    cls.add_method('FunctionDadTimeout', 
                   'void', 
                   [param('ns3::Ptr< ns3::Icmpv6L4Protocol >', 'icmpv6'), param('ns3::Ipv6Interface *', 'interface'), param('ns3::Ipv6Address', 'addr')], 
                   is_static=True)
    ## icmpv6-l4-protocol.h: int ns3::Icmpv6L4Protocol::GetProtocolNumber() const [member function]
    cls.add_method('GetProtocolNumber', 
                   'int', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-l4-protocol.h: static uint16_t ns3::Icmpv6L4Protocol::GetStaticProtocolNumber() [member function]
    cls.add_method('GetStaticProtocolNumber', 
                   'uint16_t', 
                   [], 
                   is_static=True)
    ## icmpv6-l4-protocol.h: static ns3::TypeId ns3::Icmpv6L4Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## icmpv6-l4-protocol.h: int ns3::Icmpv6L4Protocol::GetVersion() const [member function]
    cls.add_method('GetVersion', 
                   'int', 
                   [], 
                   is_const=True, is_virtual=True)
    ## icmpv6-l4-protocol.h: bool ns3::Icmpv6L4Protocol::IsAlwaysDad() const [member function]
    cls.add_method('IsAlwaysDad', 
                   'bool', 
                   [], 
                   is_const=True)
    ## icmpv6-l4-protocol.h: bool ns3::Icmpv6L4Protocol::Lookup(ns3::Ipv6Address dst, ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::NdiscCache> cache, ns3::Address * hardwareDestination) [member function]
    cls.add_method('Lookup', 
                   'bool', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::NdiscCache >', 'cache'), param('ns3::Address *', 'hardwareDestination')])
    ## icmpv6-l4-protocol.h: bool ns3::Icmpv6L4Protocol::Lookup(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Address dst, ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::NdiscCache> cache, ns3::Address * hardwareDestination) [member function]
    cls.add_method('Lookup', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Address', 'dst'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::NdiscCache >', 'cache'), param('ns3::Address *', 'hardwareDestination')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## icmpv6-l4-protocol.h: ns3::Ipv6L4Protocol::RxStatus_e ns3::Icmpv6L4Protocol::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Address const & src, ns3::Ipv6Address const & dst, ns3::Ptr<ns3::Ipv6Interface> interface) [member function]
    cls.add_method('Receive', 
                   'ns3::Ipv6L4Protocol::RxStatus_e', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Address const &', 'src'), param('ns3::Ipv6Address const &', 'dst'), param('ns3::Ptr< ns3::Ipv6Interface >', 'interface')], 
                   is_virtual=True)
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendEchoReply(ns3::Ipv6Address src, ns3::Ipv6Address dst, uint16_t id, uint16_t seq, ns3::Ptr<ns3::Packet> data) [member function]
    cls.add_method('SendEchoReply', 
                   'void', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('uint16_t', 'id'), param('uint16_t', 'seq'), param('ns3::Ptr< ns3::Packet >', 'data')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendErrorDestinationUnreachable(ns3::Ptr<ns3::Packet> malformedPacket, ns3::Ipv6Address dst, uint8_t code) [member function]
    cls.add_method('SendErrorDestinationUnreachable', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'malformedPacket'), param('ns3::Ipv6Address', 'dst'), param('uint8_t', 'code')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendErrorParameterError(ns3::Ptr<ns3::Packet> malformedPacket, ns3::Ipv6Address dst, uint8_t code, uint32_t ptr) [member function]
    cls.add_method('SendErrorParameterError', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'malformedPacket'), param('ns3::Ipv6Address', 'dst'), param('uint8_t', 'code'), param('uint32_t', 'ptr')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendErrorTimeExceeded(ns3::Ptr<ns3::Packet> malformedPacket, ns3::Ipv6Address dst, uint8_t code) [member function]
    cls.add_method('SendErrorTimeExceeded', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'malformedPacket'), param('ns3::Ipv6Address', 'dst'), param('uint8_t', 'code')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendErrorTooBig(ns3::Ptr<ns3::Packet> malformedPacket, ns3::Ipv6Address dst, uint32_t mtu) [member function]
    cls.add_method('SendErrorTooBig', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'malformedPacket'), param('ns3::Ipv6Address', 'dst'), param('uint32_t', 'mtu')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendMessage(ns3::Ptr<ns3::Packet> packet, ns3::Ipv6Address src, ns3::Ipv6Address dst, uint8_t ttl) [member function]
    cls.add_method('SendMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('uint8_t', 'ttl')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendMessage(ns3::Ptr<ns3::Packet> packet, ns3::Ipv6Address dst, ns3::Icmpv6Header & icmpv6Hdr, uint8_t ttl) [member function]
    cls.add_method('SendMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv6Address', 'dst'), param('ns3::Icmpv6Header &', 'icmpv6Hdr'), param('uint8_t', 'ttl')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendNA(ns3::Ipv6Address src, ns3::Ipv6Address dst, ns3::Address * hardwareAddress, uint8_t flags) [member function]
    cls.add_method('SendNA', 
                   'void', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('ns3::Address *', 'hardwareAddress'), param('uint8_t', 'flags')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendNS(ns3::Ipv6Address src, ns3::Ipv6Address dst, ns3::Ipv6Address target, ns3::Address hardwareAddress) [member function]
    cls.add_method('SendNS', 
                   'void', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Address', 'target'), param('ns3::Address', 'hardwareAddress')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendRS(ns3::Ipv6Address src, ns3::Ipv6Address dst, ns3::Address hardwareAddress) [member function]
    cls.add_method('SendRS', 
                   'void', 
                   [param('ns3::Ipv6Address', 'src'), param('ns3::Ipv6Address', 'dst'), param('ns3::Address', 'hardwareAddress')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SendRedirection(ns3::Ptr<ns3::Packet> redirectedPacket, ns3::Ipv6Address dst, ns3::Ipv6Address redirTarget, ns3::Ipv6Address redirDestination, ns3::Address redirHardwareTarget) [member function]
    cls.add_method('SendRedirection', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'redirectedPacket'), param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Address', 'redirTarget'), param('ns3::Ipv6Address', 'redirDestination'), param('ns3::Address', 'redirHardwareTarget')])
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::DELAY_FIRST_PROBE_TIME [variable]
    cls.add_static_attribute('DELAY_FIRST_PROBE_TIME', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_ANYCAST_DELAY_TIME [variable]
    cls.add_static_attribute('MAX_ANYCAST_DELAY_TIME', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_FINAL_RTR_ADVERTISEMENTS [variable]
    cls.add_static_attribute('MAX_FINAL_RTR_ADVERTISEMENTS', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_INITIAL_RTR_ADVERTISEMENTS [variable]
    cls.add_static_attribute('MAX_INITIAL_RTR_ADVERTISEMENTS', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_INITIAL_RTR_ADVERT_INTERVAL [variable]
    cls.add_static_attribute('MAX_INITIAL_RTR_ADVERT_INTERVAL', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_MULTICAST_SOLICIT [variable]
    cls.add_static_attribute('MAX_MULTICAST_SOLICIT', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_NEIGHBOR_ADVERTISEMENT [variable]
    cls.add_static_attribute('MAX_NEIGHBOR_ADVERTISEMENT', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_RANDOM_FACTOR [variable]
    cls.add_static_attribute('MAX_RANDOM_FACTOR', 'double const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_RA_DELAY_TIME [variable]
    cls.add_static_attribute('MAX_RA_DELAY_TIME', 'uint32_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_RTR_SOLICITATIONS [variable]
    cls.add_static_attribute('MAX_RTR_SOLICITATIONS', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_RTR_SOLICITATION_DELAY [variable]
    cls.add_static_attribute('MAX_RTR_SOLICITATION_DELAY', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MAX_UNICAST_SOLICIT [variable]
    cls.add_static_attribute('MAX_UNICAST_SOLICIT', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MIN_DELAY_BETWEEN_RAS [variable]
    cls.add_static_attribute('MIN_DELAY_BETWEEN_RAS', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::MIN_RANDOM_FACTOR [variable]
    cls.add_static_attribute('MIN_RANDOM_FACTOR', 'double const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::PROT_NUMBER [variable]
    cls.add_static_attribute('PROT_NUMBER', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::REACHABLE_TIME [variable]
    cls.add_static_attribute('REACHABLE_TIME', 'uint32_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::RETRANS_TIMER [variable]
    cls.add_static_attribute('RETRANS_TIMER', 'uint32_t const', is_const=True)
    ## icmpv6-l4-protocol.h: ns3::Icmpv6L4Protocol::RTR_SOLICITATION_INTERVAL [variable]
    cls.add_static_attribute('RTR_SOLICITATION_INTERVAL', 'uint8_t const', is_const=True)
    ## icmpv6-l4-protocol.h: void ns3::Icmpv6L4Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3LoopbackNetDevice_methods(root_module, cls):
    ## loopback-net-device.h: ns3::LoopbackNetDevice::LoopbackNetDevice(ns3::LoopbackNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LoopbackNetDevice const &', 'arg0')])
    ## loopback-net-device.h: ns3::LoopbackNetDevice::LoopbackNetDevice() [constructor]
    cls.add_constructor([])
    ## loopback-net-device.h: void ns3::LoopbackNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## loopback-net-device.h: ns3::Address ns3::LoopbackNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: ns3::Address ns3::LoopbackNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: ns3::Ptr<ns3::Channel> ns3::LoopbackNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: uint32_t ns3::LoopbackNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: uint16_t ns3::LoopbackNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: ns3::Address ns3::LoopbackNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: ns3::Address ns3::LoopbackNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: ns3::Ptr<ns3::Node> ns3::LoopbackNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: static ns3::TypeId ns3::LoopbackNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## loopback-net-device.h: void ns3::LoopbackNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## loopback-net-device.h: void ns3::LoopbackNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## loopback-net-device.h: void ns3::LoopbackNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## loopback-net-device.h: void ns3::LoopbackNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## loopback-net-device.h: void ns3::LoopbackNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## loopback-net-device.h: bool ns3::LoopbackNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## loopback-net-device.h: void ns3::LoopbackNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
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

