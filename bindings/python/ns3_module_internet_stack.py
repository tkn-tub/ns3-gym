from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## tcp-header.h: ns3::TcpHeader [class]
    module.add_class('TcpHeader', parent=root_module['ns3::Header'])
    ## tcp-header.h: ns3::TcpHeader::Flags_t [enumeration]
    module.add_enum('Flags_t', ['NONE', 'FIN', 'SYN', 'RST', 'PSH', 'ACK', 'URG'], outer_class=root_module['ns3::TcpHeader'])
    ## udp-header.h: ns3::UdpHeader [class]
    module.add_class('UdpHeader', parent=root_module['ns3::Header'])
    ## ipv4-interface.h: ns3::Ipv4Interface [class]
    module.add_class('Ipv4Interface', parent=root_module['ns3::Object'])
    ## ipv4-l3-protocol.h: ns3::Ipv4L3Protocol [class]
    module.add_class('Ipv4L3Protocol', parent=root_module['ns3::Object'])
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting [class]
    module.add_class('Ipv4StaticRouting', parent=root_module['ns3::Ipv4RoutingProtocol'])
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_TimeStepPrecision(module):
    root_module = module.get_root()
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3TcpHeader_methods(root_module, root_module['ns3::TcpHeader'])
    register_Ns3UdpHeader_methods(root_module, root_module['ns3::UdpHeader'])
    register_Ns3Ipv4Interface_methods(root_module, root_module['ns3::Ipv4Interface'])
    register_Ns3Ipv4L3Protocol_methods(root_module, root_module['ns3::Ipv4L3Protocol'])
    register_Ns3Ipv4StaticRouting_methods(root_module, root_module['ns3::Ipv4StaticRouting'])
    return

def register_Ns3TcpHeader_methods(root_module, cls):
    ## tcp-header.h: ns3::TcpHeader::TcpHeader(ns3::TcpHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TcpHeader const &', 'arg0')])
    ## tcp-header.h: ns3::TcpHeader::TcpHeader() [constructor]
    cls.add_constructor([])
    ## tcp-header.h: void ns3::TcpHeader::EnableChecksums() [member function]
    cls.add_method('EnableChecksums', 
                   'void', 
                   [])
    ## tcp-header.h: void ns3::TcpHeader::SetSourcePort(uint16_t port) [member function]
    cls.add_method('SetSourcePort', 
                   'void', 
                   [param('uint16_t', 'port')])
    ## tcp-header.h: void ns3::TcpHeader::SetDestinationPort(uint16_t port) [member function]
    cls.add_method('SetDestinationPort', 
                   'void', 
                   [param('uint16_t', 'port')])
    ## tcp-header.h: void ns3::TcpHeader::SetSequenceNumber(SequenceNumber sequenceNumber) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('SequenceNumber', 'sequenceNumber')])
    ## tcp-header.h: void ns3::TcpHeader::SetAckNumber(SequenceNumber ackNumber) [member function]
    cls.add_method('SetAckNumber', 
                   'void', 
                   [param('SequenceNumber', 'ackNumber')])
    ## tcp-header.h: void ns3::TcpHeader::SetLength(uint8_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint8_t', 'length')])
    ## tcp-header.h: void ns3::TcpHeader::SetFlags(uint8_t flags) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## tcp-header.h: void ns3::TcpHeader::SetWindowSize(uint16_t windowSize) [member function]
    cls.add_method('SetWindowSize', 
                   'void', 
                   [param('uint16_t', 'windowSize')])
    ## tcp-header.h: void ns3::TcpHeader::SetUrgentPointer(uint16_t urgentPointer) [member function]
    cls.add_method('SetUrgentPointer', 
                   'void', 
                   [param('uint16_t', 'urgentPointer')])
    ## tcp-header.h: uint16_t ns3::TcpHeader::GetSourcePort() const [member function]
    cls.add_method('GetSourcePort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint16_t ns3::TcpHeader::GetDestinationPort() const [member function]
    cls.add_method('GetDestinationPort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: SequenceNumber ns3::TcpHeader::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'SequenceNumber', 
                   [], 
                   is_const=True)
    ## tcp-header.h: SequenceNumber ns3::TcpHeader::GetAckNumber() const [member function]
    cls.add_method('GetAckNumber', 
                   'SequenceNumber', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint8_t ns3::TcpHeader::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint8_t ns3::TcpHeader::GetFlags() const [member function]
    cls.add_method('GetFlags', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint16_t ns3::TcpHeader::GetWindowSize() const [member function]
    cls.add_method('GetWindowSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: uint16_t ns3::TcpHeader::GetUrgentPointer() const [member function]
    cls.add_method('GetUrgentPointer', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## tcp-header.h: void ns3::TcpHeader::InitializeChecksum(ns3::Ipv4Address source, ns3::Ipv4Address destination, uint8_t protocol) [member function]
    cls.add_method('InitializeChecksum', 
                   'void', 
                   [param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'destination'), param('uint8_t', 'protocol')])
    ## tcp-header.h: static ns3::TypeId ns3::TcpHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tcp-header.h: ns3::TypeId ns3::TcpHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tcp-header.h: void ns3::TcpHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## tcp-header.h: uint32_t ns3::TcpHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tcp-header.h: void ns3::TcpHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## tcp-header.h: uint32_t ns3::TcpHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## tcp-header.h: bool ns3::TcpHeader::IsChecksumOk() const [member function]
    cls.add_method('IsChecksumOk', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3UdpHeader_methods(root_module, cls):
    ## udp-header.h: ns3::UdpHeader::UdpHeader(ns3::UdpHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpHeader const &', 'arg0')])
    ## udp-header.h: ns3::UdpHeader::UdpHeader() [constructor]
    cls.add_constructor([])
    ## udp-header.h: void ns3::UdpHeader::EnableChecksums() [member function]
    cls.add_method('EnableChecksums', 
                   'void', 
                   [])
    ## udp-header.h: void ns3::UdpHeader::SetDestinationPort(uint16_t port) [member function]
    cls.add_method('SetDestinationPort', 
                   'void', 
                   [param('uint16_t', 'port')])
    ## udp-header.h: void ns3::UdpHeader::SetSourcePort(uint16_t port) [member function]
    cls.add_method('SetSourcePort', 
                   'void', 
                   [param('uint16_t', 'port')])
    ## udp-header.h: uint16_t ns3::UdpHeader::GetSourcePort() const [member function]
    cls.add_method('GetSourcePort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## udp-header.h: uint16_t ns3::UdpHeader::GetDestinationPort() const [member function]
    cls.add_method('GetDestinationPort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## udp-header.h: void ns3::UdpHeader::InitializeChecksum(ns3::Ipv4Address source, ns3::Ipv4Address destination, uint8_t protocol) [member function]
    cls.add_method('InitializeChecksum', 
                   'void', 
                   [param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'destination'), param('uint8_t', 'protocol')])
    ## udp-header.h: static ns3::TypeId ns3::UdpHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-header.h: ns3::TypeId ns3::UdpHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## udp-header.h: void ns3::UdpHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## udp-header.h: uint32_t ns3::UdpHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## udp-header.h: void ns3::UdpHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## udp-header.h: uint32_t ns3::UdpHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## udp-header.h: bool ns3::UdpHeader::IsChecksumOk() const [member function]
    cls.add_method('IsChecksumOk', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3Ipv4Interface_methods(root_module, cls):
    ## ipv4-interface.h: ns3::Ipv4Interface::Ipv4Interface(ns3::Ipv4Interface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Interface const &', 'arg0')])
    ## ipv4-interface.h: static ns3::TypeId ns3::Ipv4Interface::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-interface.h: ns3::Ipv4Interface::Ipv4Interface() [constructor]
    cls.add_constructor([])
    ## ipv4-interface.h: ns3::Ptr<ns3::NetDevice> ns3::Ipv4Interface::GetDevice() const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetAddress(ns3::Ipv4Address a) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Ipv4Address', 'a')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetNetworkMask(ns3::Ipv4Mask mask) [member function]
    cls.add_method('SetNetworkMask', 
                   'void', 
                   [param('ns3::Ipv4Mask', 'mask')])
    ## ipv4-interface.h: ns3::Ipv4Address ns3::Ipv4Interface::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: ns3::Ipv4Mask ns3::Ipv4Interface::GetNetworkMask() const [member function]
    cls.add_method('GetNetworkMask', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetMetric(uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint16_t', 'metric')])
    ## ipv4-interface.h: uint16_t ns3::Ipv4Interface::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: ns3::Ipv4Address ns3::Ipv4Interface::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: uint16_t ns3::Ipv4Interface::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: bool ns3::Ipv4Interface::IsUp() const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: bool ns3::Ipv4Interface::IsDown() const [member function]
    cls.add_method('IsDown', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetUp() [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [])
    ## ipv4-interface.h: void ns3::Ipv4Interface::SetDown() [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [])
    ## ipv4-interface.h: void ns3::Ipv4Interface::Send(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Address dest) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Address', 'dest')])
    ## ipv4-interface.h: void ns3::Ipv4Interface::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ipv4-interface.h: void ns3::Ipv4Interface::SendTo(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Address dest) [member function]
    cls.add_method('SendTo', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Address', 'dest')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
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
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Insert(ns3::Ptr<ns3::Ipv4L4Protocol> protocol) [member function]
    cls.add_method('Insert', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4L4Protocol >', 'protocol')])
    ## ipv4-l3-protocol.h: ns3::Ptr<ns3::Ipv4L4Protocol> ns3::Ipv4L3Protocol::GetProtocol(int protocolNumber) [member function]
    cls.add_method('GetProtocol', 
                   'ns3::Ptr< ns3::Ipv4L4Protocol >', 
                   [param('int', 'protocolNumber')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Remove(ns3::Ptr<ns3::Ipv4L4Protocol> protocol) [member function]
    cls.add_method('Remove', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4L4Protocol >', 'protocol')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetDefaultTtl(uint8_t ttl) [member function]
    cls.add_method('SetDefaultTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    ## ipv4-l3-protocol.h: ns3::Ptr<ns3::Ipv4Interface> ns3::Ipv4L3Protocol::FindInterfaceForDevice(ns3::Ptr<const ns3::NetDevice> device) [member function]
    cls.add_method('FindInterfaceForDevice', 
                   'ns3::Ptr< ns3::Ipv4Interface >', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Receive(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<const ns3::Packet> p, uint16_t protocol, ns3::Address const & from, ns3::Address const & to, ns3::NetDevice::PacketType packetType) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Packet const >', 'p'), param('uint16_t', 'protocol'), param('ns3::Address const &', 'from'), param('ns3::Address const &', 'to'), param('ns3::NetDevice::PacketType', 'packetType')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Send(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Address source, ns3::Ipv4Address destination, uint8_t protocol) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'destination'), param('uint8_t', 'protocol')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::AddHostRouteTo(ns3::Ipv4Address dest, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::AddHostRouteTo(ns3::Ipv4Address dest, uint32_t interface) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest'), param('uint32_t', 'interface')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::AddNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::AddNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, uint32_t interface) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('uint32_t', 'interface')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetDefaultRoute(ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('SetDefaultRoute', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::Lookup(ns3::Ipv4Header const & ipHeader, ns3::Ptr<ns3::Packet> packet, ns3::Callback<void,bool,const ns3::Ipv4Route&,ns3::Ptr<ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty> routeReply) [member function]
    cls.add_method('Lookup', 
                   'void', 
                   [param('ns3::Ipv4Header const &', 'ipHeader'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Callback< void, bool, ns3::Ipv4Route const &, ns3::Ptr< ns3::Packet >, ns3::Ipv4Header const &, ns3::empty, ns3::empty >', 'routeReply')])
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::GetNRoutes() [member function]
    cls.add_method('GetNRoutes', 
                   'uint32_t', 
                   [])
    ## ipv4-l3-protocol.h: ns3::Ipv4Route * ns3::Ipv4L3Protocol::GetRoute(uint32_t i) [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ipv4Route *', 
                   [param('uint32_t', 'i')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::RemoveRoute(uint32_t i) [member function]
    cls.add_method('RemoveRoute', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::AddMulticastRoute(ns3::Ipv4Address origin, ns3::Ipv4Address group, uint32_t inputInterface, std::vector<unsigned int, std::allocator<unsigned int> > outputInterfaces) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group'), param('uint32_t', 'inputInterface'), param('std::vector< unsigned int >', 'outputInterfaces')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetDefaultMulticastRoute(uint32_t onputInterface) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('uint32_t', 'onputInterface')])
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::GetNMulticastRoutes() const [member function]
    cls.add_method('GetNMulticastRoutes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-l3-protocol.h: ns3::Ipv4MulticastRoute * ns3::Ipv4L3Protocol::GetMulticastRoute(uint32_t i) const [member function]
    cls.add_method('GetMulticastRoute', 
                   'ns3::Ipv4MulticastRoute *', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::RemoveMulticastRoute(ns3::Ipv4Address origin, ns3::Ipv4Address group, uint32_t inputInterface) [member function]
    cls.add_method('RemoveMulticastRoute', 
                   'void', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group'), param('uint32_t', 'inputInterface')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::RemoveMulticastRoute(uint32_t i) [member function]
    cls.add_method('RemoveMulticastRoute', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::AddInterface(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddInterface', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## ipv4-l3-protocol.h: ns3::Ptr<ns3::Ipv4Interface> ns3::Ipv4L3Protocol::GetInterface(uint32_t i) const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ptr< ns3::Ipv4Interface >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::FindInterfaceForAddr(ns3::Ipv4Address addr) const [member function]
    cls.add_method('FindInterfaceForAddr', 
                   'uint32_t', 
                   [param('ns3::Ipv4Address', 'addr')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: uint32_t ns3::Ipv4L3Protocol::FindInterfaceForAddr(ns3::Ipv4Address addr, ns3::Ipv4Mask mask) const [member function]
    cls.add_method('FindInterfaceForAddr', 
                   'uint32_t', 
                   [param('ns3::Ipv4Address', 'addr'), param('ns3::Ipv4Mask', 'mask')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: int32_t ns3::Ipv4L3Protocol::FindInterfaceIndexForDevice(ns3::Ptr<ns3::NetDevice> device) const [member function]
    cls.add_method('FindInterfaceIndexForDevice', 
                   'int32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::JoinMulticastGroup(ns3::Ipv4Address origin, ns3::Ipv4Address group) [member function]
    cls.add_method('JoinMulticastGroup', 
                   'void', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::LeaveMulticastGroup(ns3::Ipv4Address origin, ns3::Ipv4Address group) [member function]
    cls.add_method('LeaveMulticastGroup', 
                   'void', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetAddress(uint32_t i, ns3::Ipv4Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('uint32_t', 'i'), param('ns3::Ipv4Address', 'address')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetNetworkMask(uint32_t i, ns3::Ipv4Mask mask) [member function]
    cls.add_method('SetNetworkMask', 
                   'void', 
                   [param('uint32_t', 'i'), param('ns3::Ipv4Mask', 'mask')])
    ## ipv4-l3-protocol.h: ns3::Ipv4Mask ns3::Ipv4L3Protocol::GetNetworkMask(uint32_t t) const [member function]
    cls.add_method('GetNetworkMask', 
                   'ns3::Ipv4Mask', 
                   [param('uint32_t', 't')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: ns3::Ipv4Address ns3::Ipv4L3Protocol::GetAddress(uint32_t i) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetMetric(uint32_t i, uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'i'), param('uint16_t', 'metric')])
    ## ipv4-l3-protocol.h: uint16_t ns3::Ipv4L3Protocol::GetMetric(uint32_t i) const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::GetIfIndexForDestination(ns3::Ipv4Address destination, uint32_t & ifIndex) const [member function]
    cls.add_method('GetIfIndexForDestination', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'destination'), param('uint32_t &', 'ifIndex')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: uint16_t ns3::Ipv4L3Protocol::GetMtu(uint32_t i) const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: bool ns3::Ipv4L3Protocol::IsUp(uint32_t i) const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetUp(uint32_t i) [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::SetDown(uint32_t i) [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::AddRoutingProtocol(ns3::Ptr<ns3::Ipv4RoutingProtocol> routingProtocol, int priority) [member function]
    cls.add_method('AddRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4RoutingProtocol >', 'routingProtocol'), param('int', 'priority')])
    ## ipv4-l3-protocol.h: void ns3::Ipv4L3Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Ipv4StaticRouting_methods(root_module, cls):
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting::Ipv4StaticRouting(ns3::Ipv4StaticRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4StaticRouting const &', 'arg0')])
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting::Ipv4StaticRouting() [constructor]
    cls.add_constructor([])
    ## ipv4-static-routing.h: bool ns3::Ipv4StaticRouting::RequestRoute(uint32_t ifIndex, ns3::Ipv4Header const & ipHeader, ns3::Ptr<ns3::Packet> packet, ns3::Callback<void,bool,const ns3::Ipv4Route&,ns3::Ptr<ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'ifIndex'), param('ns3::Ipv4Header const &', 'ipHeader'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Callback< void, bool, ns3::Ipv4Route const &, ns3::Ptr< ns3::Packet >, ns3::Ipv4Header const &, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: bool ns3::Ipv4StaticRouting::RequestIfIndex(ns3::Ipv4Address destination, uint32_t & ifIndex) [member function]
    cls.add_method('RequestIfIndex', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'destination'), param('uint32_t &', 'ifIndex')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddHostRouteTo(ns3::Ipv4Address dest, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddHostRouteTo(ns3::Ipv4Address dest, uint32_t interface) [member function]
    cls.add_method('AddHostRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest'), param('uint32_t', 'interface')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, uint32_t interface) [member function]
    cls.add_method('AddNetworkRouteTo', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('uint32_t', 'interface')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::SetDefaultRoute(ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('SetDefaultRoute', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')])
    ## ipv4-static-routing.h: uint32_t ns3::Ipv4StaticRouting::GetNRoutes() [member function]
    cls.add_method('GetNRoutes', 
                   'uint32_t', 
                   [])
    ## ipv4-static-routing.h: ns3::Ipv4Route * ns3::Ipv4StaticRouting::GetDefaultRoute() [member function]
    cls.add_method('GetDefaultRoute', 
                   'ns3::Ipv4Route *', 
                   [])
    ## ipv4-static-routing.h: ns3::Ipv4Route * ns3::Ipv4StaticRouting::GetRoute(uint32_t i) [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ipv4Route *', 
                   [param('uint32_t', 'i')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::RemoveRoute(uint32_t i) [member function]
    cls.add_method('RemoveRoute', 
                   'void', 
                   [param('uint32_t', 'i')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::AddMulticastRoute(ns3::Ipv4Address origin, ns3::Ipv4Address group, uint32_t inputInterface, std::vector<unsigned int, std::allocator<unsigned int> > outputInterfaces) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group'), param('uint32_t', 'inputInterface'), param('std::vector< unsigned int >', 'outputInterfaces')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::SetDefaultMulticastRoute(uint32_t outputInterface) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('uint32_t', 'outputInterface')])
    ## ipv4-static-routing.h: uint32_t ns3::Ipv4StaticRouting::GetNMulticastRoutes() const [member function]
    cls.add_method('GetNMulticastRoutes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-static-routing.h: ns3::Ipv4MulticastRoute * ns3::Ipv4StaticRouting::GetMulticastRoute(uint32_t i) const [member function]
    cls.add_method('GetMulticastRoute', 
                   'ns3::Ipv4MulticastRoute *', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-static-routing.h: ns3::Ipv4MulticastRoute * ns3::Ipv4StaticRouting::GetDefaultMulticastRoute() const [member function]
    cls.add_method('GetDefaultMulticastRoute', 
                   'ns3::Ipv4MulticastRoute *', 
                   [], 
                   is_const=True)
    ## ipv4-static-routing.h: bool ns3::Ipv4StaticRouting::RemoveMulticastRoute(ns3::Ipv4Address origin, ns3::Ipv4Address group, uint32_t inputInterface) [member function]
    cls.add_method('RemoveMulticastRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group'), param('uint32_t', 'inputInterface')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::RemoveMulticastRoute(uint32_t index) [member function]
    cls.add_method('RemoveMulticastRoute', 
                   'void', 
                   [param('uint32_t', 'index')])
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    ## internet-stack.h: extern void ns3::AddInternetStack(ns3::Ptr<ns3::Node> node) [free function]
    module.add_function('AddInternetStack', 
                        'void', 
                        [param('ns3::Ptr< ns3::Node >', 'node')])
    ## internet-stack.h: extern void ns3::AddNscInternetStack(ns3::Ptr<ns3::Node> node, std::string const & soname) [free function]
    module.add_function('AddNscInternetStack', 
                        'void', 
                        [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string const &', 'soname')])
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

