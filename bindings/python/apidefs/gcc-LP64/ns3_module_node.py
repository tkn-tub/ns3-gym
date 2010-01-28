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
