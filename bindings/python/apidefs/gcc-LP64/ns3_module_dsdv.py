from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## dsdv-helper.h: ns3::DsdvHelper [class]
    module.add_class('DsdvHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    
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
    
    ## dsdv-rtable.h: ns3::dsdv::RouteFlags [enumeration]
    module.add_enum('RouteFlags', ['VALID', 'INVALID'])
    ## dsdv-packet.h: ns3::dsdv::DsdvHeader [class]
    module.add_class('DsdvHeader', parent=root_module['ns3::Header'])
    ## dsdv-packet-queue.h: ns3::dsdv::PacketQueue [class]
    module.add_class('PacketQueue')
    ## dsdv-packet-queue.h: ns3::dsdv::QueueEntry [class]
    module.add_class('QueueEntry')
    ## dsdv-routing-protocol.h: ns3::dsdv::RoutingProtocol [class]
    module.add_class('RoutingProtocol', parent=root_module['ns3::Ipv4RoutingProtocol'])
    ## dsdv-rtable.h: ns3::dsdv::RoutingTable [class]
    module.add_class('RoutingTable')
    ## dsdv-rtable.h: ns3::dsdv::RoutingTableEntry [class]
    module.add_class('RoutingTableEntry')
    module.add_container('std::map< ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry >', ('ns3::Ipv4Address', 'ns3::dsdv::RoutingTableEntry'), container_type='map')

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3DsdvHelper_methods(root_module, root_module['ns3::DsdvHelper'])
    register_Ns3DsdvDsdvHeader_methods(root_module, root_module['ns3::dsdv::DsdvHeader'])
    register_Ns3DsdvPacketQueue_methods(root_module, root_module['ns3::dsdv::PacketQueue'])
    register_Ns3DsdvQueueEntry_methods(root_module, root_module['ns3::dsdv::QueueEntry'])
    register_Ns3DsdvRoutingProtocol_methods(root_module, root_module['ns3::dsdv::RoutingProtocol'])
    register_Ns3DsdvRoutingTable_methods(root_module, root_module['ns3::dsdv::RoutingTable'])
    register_Ns3DsdvRoutingTableEntry_methods(root_module, root_module['ns3::dsdv::RoutingTableEntry'])
    return

def register_Ns3DsdvHelper_methods(root_module, cls):
    ## dsdv-helper.h: ns3::DsdvHelper::DsdvHelper(ns3::DsdvHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DsdvHelper const &', 'arg0')])
    ## dsdv-helper.h: ns3::DsdvHelper::DsdvHelper() [constructor]
    cls.add_constructor([])
    ## dsdv-helper.h: ns3::DsdvHelper * ns3::DsdvHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::DsdvHelper *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dsdv-helper.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::DsdvHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## dsdv-helper.h: void ns3::DsdvHelper::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3DsdvDsdvHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## dsdv-packet.h: ns3::dsdv::DsdvHeader::DsdvHeader(ns3::dsdv::DsdvHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::DsdvHeader const &', 'arg0')])
    ## dsdv-packet.h: ns3::dsdv::DsdvHeader::DsdvHeader(ns3::Ipv4Address dst=ns3::Ipv4Address(), uint32_t hopcount=0, uint32_t dstSeqNo=0) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'dst', default_value='ns3::Ipv4Address()'), param('uint32_t', 'hopcount', default_value='0'), param('uint32_t', 'dstSeqNo', default_value='0')])
    ## dsdv-packet.h: uint32_t ns3::dsdv::DsdvHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## dsdv-packet.h: ns3::Ipv4Address ns3::dsdv::DsdvHeader::GetDst() const [member function]
    cls.add_method('GetDst', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## dsdv-packet.h: uint32_t ns3::dsdv::DsdvHeader::GetDstSeqno() const [member function]
    cls.add_method('GetDstSeqno', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-packet.h: uint32_t ns3::dsdv::DsdvHeader::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-packet.h: ns3::TypeId ns3::dsdv::DsdvHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dsdv-packet.h: uint32_t ns3::dsdv::DsdvHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dsdv-packet.h: static ns3::TypeId ns3::dsdv::DsdvHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::SetDst(ns3::Ipv4Address destination) [member function]
    cls.add_method('SetDst', 
                   'void', 
                   [param('ns3::Ipv4Address', 'destination')])
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::SetDstSeqno(uint32_t sequenceNumber) [member function]
    cls.add_method('SetDstSeqno', 
                   'void', 
                   [param('uint32_t', 'sequenceNumber')])
    ## dsdv-packet.h: void ns3::dsdv::DsdvHeader::SetHopCount(uint32_t hopCount) [member function]
    cls.add_method('SetHopCount', 
                   'void', 
                   [param('uint32_t', 'hopCount')])
    return

def register_Ns3DsdvPacketQueue_methods(root_module, cls):
    ## dsdv-packet-queue.h: ns3::dsdv::PacketQueue::PacketQueue(ns3::dsdv::PacketQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::PacketQueue const &', 'arg0')])
    ## dsdv-packet-queue.h: ns3::dsdv::PacketQueue::PacketQueue() [constructor]
    cls.add_constructor([])
    ## dsdv-packet-queue.h: bool ns3::dsdv::PacketQueue::Dequeue(ns3::Ipv4Address dst, ns3::dsdv::QueueEntry & entry) [member function]
    cls.add_method('Dequeue', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('ns3::dsdv::QueueEntry &', 'entry')])
    ## dsdv-packet-queue.h: void ns3::dsdv::PacketQueue::DropPacketWithDst(ns3::Ipv4Address dst) [member function]
    cls.add_method('DropPacketWithDst', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## dsdv-packet-queue.h: bool ns3::dsdv::PacketQueue::Enqueue(ns3::dsdv::QueueEntry & entry) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::dsdv::QueueEntry &', 'entry')])
    ## dsdv-packet-queue.h: bool ns3::dsdv::PacketQueue::Find(ns3::Ipv4Address dst) [member function]
    cls.add_method('Find', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## dsdv-packet-queue.h: uint32_t ns3::dsdv::PacketQueue::GetCountForPacketsWithDst(ns3::Ipv4Address dst) [member function]
    cls.add_method('GetCountForPacketsWithDst', 
                   'uint32_t', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## dsdv-packet-queue.h: uint32_t ns3::dsdv::PacketQueue::GetMaxPacketsPerDst() const [member function]
    cls.add_method('GetMaxPacketsPerDst', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: uint32_t ns3::dsdv::PacketQueue::GetMaxQueueLen() const [member function]
    cls.add_method('GetMaxQueueLen', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Time ns3::dsdv::PacketQueue::GetQueueTimeout() const [member function]
    cls.add_method('GetQueueTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: uint32_t ns3::dsdv::PacketQueue::GetSize() [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [])
    ## dsdv-packet-queue.h: void ns3::dsdv::PacketQueue::SetMaxPacketsPerDst(uint32_t len) [member function]
    cls.add_method('SetMaxPacketsPerDst', 
                   'void', 
                   [param('uint32_t', 'len')])
    ## dsdv-packet-queue.h: void ns3::dsdv::PacketQueue::SetMaxQueueLen(uint32_t len) [member function]
    cls.add_method('SetMaxQueueLen', 
                   'void', 
                   [param('uint32_t', 'len')])
    ## dsdv-packet-queue.h: void ns3::dsdv::PacketQueue::SetQueueTimeout(ns3::Time t) [member function]
    cls.add_method('SetQueueTimeout', 
                   'void', 
                   [param('ns3::Time', 't')])
    return

def register_Ns3DsdvQueueEntry_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## dsdv-packet-queue.h: ns3::dsdv::QueueEntry::QueueEntry(ns3::dsdv::QueueEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::QueueEntry const &', 'arg0')])
    ## dsdv-packet-queue.h: ns3::dsdv::QueueEntry::QueueEntry(ns3::Ptr<ns3::Packet const> pa=0, ns3::Ipv4Header const & h=ns3::Ipv4Header(), ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb=ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>(), ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb=ns3::Callback<void, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>()) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet const >', 'pa', default_value='0'), param('ns3::Ipv4Header const &', 'h', default_value='ns3::Ipv4Header()'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb', default_value='ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>()'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb', default_value='ns3::Callback<void, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>()')])
    ## dsdv-packet-queue.h: ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::dsdv::QueueEntry::GetErrorCallback() const [member function]
    cls.add_method('GetErrorCallback', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Time ns3::dsdv::QueueEntry::GetExpireTime() const [member function]
    cls.add_method('GetExpireTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Ipv4Header ns3::dsdv::QueueEntry::GetIpv4Header() const [member function]
    cls.add_method('GetIpv4Header', 
                   'ns3::Ipv4Header', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Ptr<ns3::Packet const> ns3::dsdv::QueueEntry::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::dsdv::QueueEntry::GetUnicastForwardCallback() const [member function]
    cls.add_method('GetUnicastForwardCallback', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('SetErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')])
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetExpireTime(ns3::Time exp) [member function]
    cls.add_method('SetExpireTime', 
                   'void', 
                   [param('ns3::Time', 'exp')])
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetIpv4Header(ns3::Ipv4Header h) [member function]
    cls.add_method('SetIpv4Header', 
                   'void', 
                   [param('ns3::Ipv4Header', 'h')])
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetPacket(ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('SetPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## dsdv-packet-queue.h: void ns3::dsdv::QueueEntry::SetUnicastForwardCallback(ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb) [member function]
    cls.add_method('SetUnicastForwardCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb')])
    return

def register_Ns3DsdvRoutingProtocol_methods(root_module, cls):
    ## dsdv-routing-protocol.h: ns3::dsdv::RoutingProtocol::RoutingProtocol(ns3::dsdv::RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::RoutingProtocol const &', 'arg0')])
    ## dsdv-routing-protocol.h: ns3::dsdv::RoutingProtocol::RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: bool ns3::dsdv::RoutingProtocol::GetEnableBufferFlag() const [member function]
    cls.add_method('GetEnableBufferFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dsdv-routing-protocol.h: bool ns3::dsdv::RoutingProtocol::GetEnableRAFlag() const [member function]
    cls.add_method('GetEnableRAFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dsdv-routing-protocol.h: static ns3::TypeId ns3::dsdv::RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dsdv-routing-protocol.h: bool ns3::dsdv::RoutingProtocol::GetWSTFlag() const [member function]
    cls.add_method('GetWSTFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True, is_virtual=True)
    ## dsdv-routing-protocol.h: bool ns3::dsdv::RoutingProtocol::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: ns3::Ptr<ns3::Ipv4Route> ns3::dsdv::RoutingProtocol::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::SetEnableBufferFlag(bool f) [member function]
    cls.add_method('SetEnableBufferFlag', 
                   'void', 
                   [param('bool', 'f')])
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::SetEnableRAFlag(bool f) [member function]
    cls.add_method('SetEnableRAFlag', 
                   'void', 
                   [param('bool', 'f')])
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_virtual=True)
    ## dsdv-routing-protocol.h: void ns3::dsdv::RoutingProtocol::SetWSTFlag(bool f) [member function]
    cls.add_method('SetWSTFlag', 
                   'void', 
                   [param('bool', 'f')])
    ## dsdv-routing-protocol.h: ns3::dsdv::RoutingProtocol::DSDV_PORT [variable]
    cls.add_static_attribute('DSDV_PORT', 'uint32_t const', is_const=True)
    return

def register_Ns3DsdvRoutingTable_methods(root_module, cls):
    ## dsdv-rtable.h: ns3::dsdv::RoutingTable::RoutingTable(ns3::dsdv::RoutingTable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::RoutingTable const &', 'arg0')])
    ## dsdv-rtable.h: ns3::dsdv::RoutingTable::RoutingTable() [constructor]
    cls.add_constructor([])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::AddIpv4Event(ns3::Ipv4Address arg0, ns3::EventId arg1) [member function]
    cls.add_method('AddIpv4Event', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'arg0'), param('ns3::EventId', 'arg1')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::AddRoute(ns3::dsdv::RoutingTableEntry & r) [member function]
    cls.add_method('AddRoute', 
                   'bool', 
                   [param('ns3::dsdv::RoutingTableEntry &', 'r')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::AnyRunningEvent(ns3::Ipv4Address address) [member function]
    cls.add_method('AnyRunningEvent', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::DeleteAllRoutesFromInterface(ns3::Ipv4InterfaceAddress iface) [member function]
    cls.add_method('DeleteAllRoutesFromInterface', 
                   'void', 
                   [param('ns3::Ipv4InterfaceAddress', 'iface')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::DeleteIpv4Event(ns3::Ipv4Address address) [member function]
    cls.add_method('DeleteIpv4Event', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::DeleteRoute(ns3::Ipv4Address dst) [member function]
    cls.add_method('DeleteRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::ForceDeleteIpv4Event(ns3::Ipv4Address address) [member function]
    cls.add_method('ForceDeleteIpv4Event', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address')])
    ## dsdv-rtable.h: ns3::EventId ns3::dsdv::RoutingTable::GetEventId(ns3::Ipv4Address address) [member function]
    cls.add_method('GetEventId', 
                   'ns3::EventId', 
                   [param('ns3::Ipv4Address', 'address')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::GetListOfAllRoutes(std::map<ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry, std::less<ns3::Ipv4Address>, std::allocator<std::pair<ns3::Ipv4Address const, ns3::dsdv::RoutingTableEntry> > > & allRoutes) [member function]
    cls.add_method('GetListOfAllRoutes', 
                   'void', 
                   [param('std::map< ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry > &', 'allRoutes')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::GetListOfDestinationWithNextHop(ns3::Ipv4Address nxtHp, std::map<ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry, std::less<ns3::Ipv4Address>, std::allocator<std::pair<ns3::Ipv4Address const, ns3::dsdv::RoutingTableEntry> > > & dstList) [member function]
    cls.add_method('GetListOfDestinationWithNextHop', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nxtHp'), param('std::map< ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry > &', 'dstList')])
    ## dsdv-rtable.h: ns3::Time ns3::dsdv::RoutingTable::Getholddowntime() const [member function]
    cls.add_method('Getholddowntime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::LookupRoute(ns3::Ipv4Address dst, ns3::dsdv::RoutingTableEntry & rt) [member function]
    cls.add_method('LookupRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('ns3::dsdv::RoutingTableEntry &', 'rt')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::LookupRoute(ns3::Ipv4Address id, ns3::dsdv::RoutingTableEntry & rt, bool forRouteInput) [member function]
    cls.add_method('LookupRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'id'), param('ns3::dsdv::RoutingTableEntry &', 'rt'), param('bool', 'forRouteInput')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::Print(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::Purge(std::map<ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry, std::less<ns3::Ipv4Address>, std::allocator<std::pair<ns3::Ipv4Address const, ns3::dsdv::RoutingTableEntry> > > & removedAddresses) [member function]
    cls.add_method('Purge', 
                   'void', 
                   [param('std::map< ns3::Ipv4Address, ns3::dsdv::RoutingTableEntry > &', 'removedAddresses')])
    ## dsdv-rtable.h: uint32_t ns3::dsdv::RoutingTable::RoutingTableSize() [member function]
    cls.add_method('RoutingTableSize', 
                   'uint32_t', 
                   [])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTable::Setholddowntime(ns3::Time t) [member function]
    cls.add_method('Setholddowntime', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTable::Update(ns3::dsdv::RoutingTableEntry & rt) [member function]
    cls.add_method('Update', 
                   'bool', 
                   [param('ns3::dsdv::RoutingTableEntry &', 'rt')])
    return

def register_Ns3DsdvRoutingTableEntry_methods(root_module, cls):
    ## dsdv-rtable.h: ns3::dsdv::RoutingTableEntry::RoutingTableEntry(ns3::dsdv::RoutingTableEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dsdv::RoutingTableEntry const &', 'arg0')])
    ## dsdv-rtable.h: ns3::dsdv::RoutingTableEntry::RoutingTableEntry(ns3::Ptr<ns3::NetDevice> dev=0, ns3::Ipv4Address dst=ns3::Ipv4Address(), u_int32_t m_seqNo=0, ns3::Ipv4InterfaceAddress iface=ns3::Ipv4InterfaceAddress(), u_int32_t hops=0, ns3::Ipv4Address nextHop=ns3::Ipv4Address(), ns3::Time lifetime=ns3::Simulator::Now( ), ns3::Time SettlingTime=ns3::Simulator::Now( ), bool changedEntries=false) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'dev', default_value='0'), param('ns3::Ipv4Address', 'dst', default_value='ns3::Ipv4Address()'), param('u_int32_t', 'm_seqNo', default_value='0'), param('ns3::Ipv4InterfaceAddress', 'iface', default_value='ns3::Ipv4InterfaceAddress()'), param('u_int32_t', 'hops', default_value='0'), param('ns3::Ipv4Address', 'nextHop', default_value='ns3::Ipv4Address()'), param('ns3::Time', 'lifetime', default_value='ns3::Simulator::Now( )'), param('ns3::Time', 'SettlingTime', default_value='ns3::Simulator::Now( )'), param('bool', 'changedEntries', default_value='false')])
    ## dsdv-rtable.h: ns3::Ipv4Address ns3::dsdv::RoutingTableEntry::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: bool ns3::dsdv::RoutingTableEntry::GetEntriesChanged() const [member function]
    cls.add_method('GetEntriesChanged', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::dsdv::RouteFlags ns3::dsdv::RoutingTableEntry::GetFlag() const [member function]
    cls.add_method('GetFlag', 
                   'ns3::dsdv::RouteFlags', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: uint32_t ns3::dsdv::RoutingTableEntry::GetHop() const [member function]
    cls.add_method('GetHop', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Ipv4InterfaceAddress ns3::dsdv::RoutingTableEntry::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ipv4InterfaceAddress', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Time ns3::dsdv::RoutingTableEntry::GetLifeTime() const [member function]
    cls.add_method('GetLifeTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Ipv4Address ns3::dsdv::RoutingTableEntry::GetNextHop() const [member function]
    cls.add_method('GetNextHop', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Ptr<ns3::NetDevice> ns3::dsdv::RoutingTableEntry::GetOutputDevice() const [member function]
    cls.add_method('GetOutputDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Ptr<ns3::Ipv4Route> ns3::dsdv::RoutingTableEntry::GetRoute() const [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: uint32_t ns3::dsdv::RoutingTableEntry::GetSeqNo() const [member function]
    cls.add_method('GetSeqNo', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: ns3::Time ns3::dsdv::RoutingTableEntry::GetSettlingTime() const [member function]
    cls.add_method('GetSettlingTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::Print(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetEntriesChanged(bool entriesChanged) [member function]
    cls.add_method('SetEntriesChanged', 
                   'void', 
                   [param('bool', 'entriesChanged')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetFlag(ns3::dsdv::RouteFlags flag) [member function]
    cls.add_method('SetFlag', 
                   'void', 
                   [param('ns3::dsdv::RouteFlags', 'flag')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetHop(uint32_t hopCount) [member function]
    cls.add_method('SetHop', 
                   'void', 
                   [param('uint32_t', 'hopCount')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetInterface(ns3::Ipv4InterfaceAddress iface) [member function]
    cls.add_method('SetInterface', 
                   'void', 
                   [param('ns3::Ipv4InterfaceAddress', 'iface')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetLifeTime(ns3::Time lifeTime) [member function]
    cls.add_method('SetLifeTime', 
                   'void', 
                   [param('ns3::Time', 'lifeTime')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetNextHop(ns3::Ipv4Address nextHop) [member function]
    cls.add_method('SetNextHop', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nextHop')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetOutputDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('SetOutputDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetRoute(ns3::Ptr<ns3::Ipv4Route> route) [member function]
    cls.add_method('SetRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4Route >', 'route')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetSeqNo(uint32_t sequenceNumber) [member function]
    cls.add_method('SetSeqNo', 
                   'void', 
                   [param('uint32_t', 'sequenceNumber')])
    ## dsdv-rtable.h: void ns3::dsdv::RoutingTableEntry::SetSettlingTime(ns3::Time settlingTime) [member function]
    cls.add_method('SetSettlingTime', 
                   'void', 
                   [param('ns3::Time', 'settlingTime')])
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

