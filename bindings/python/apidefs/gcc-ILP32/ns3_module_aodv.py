from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## aodv-helper.h: ns3::AodvHelper [class]
    module.add_class('AodvHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    
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
    
    ## aodv-packet.h: ns3::aodv::MessageType [enumeration]
    module.add_enum('MessageType', ['AODVTYPE_RREQ', 'AODVTYPE_RREP', 'AODVTYPE_RERR', 'AODVTYPE_RREP_ACK'])
    ## aodv-rtable.h: ns3::aodv::RouteFlags [enumeration]
    module.add_enum('RouteFlags', ['VALID', 'INVALID', 'IN_SEARCH'])
    ## aodv-dpd.h: ns3::aodv::DuplicatePacketDetection [class]
    module.add_class('DuplicatePacketDetection')
    ## aodv-id-cache.h: ns3::aodv::IdCache [class]
    module.add_class('IdCache')
    ## aodv-neighbor.h: ns3::aodv::Neighbors [class]
    module.add_class('Neighbors')
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbor [struct]
    module.add_class('Neighbor', outer_class=root_module['ns3::aodv::Neighbors'])
    ## aodv-rqueue.h: ns3::aodv::QueueEntry [class]
    module.add_class('QueueEntry')
    ## aodv-rqueue.h: ns3::aodv::RequestQueue [class]
    module.add_class('RequestQueue')
    ## aodv-packet.h: ns3::aodv::RerrHeader [class]
    module.add_class('RerrHeader', parent=root_module['ns3::Header'])
    ## aodv-routing-protocol.h: ns3::aodv::RoutingProtocol [class]
    module.add_class('RoutingProtocol', parent=root_module['ns3::Ipv4RoutingProtocol'])
    ## aodv-rtable.h: ns3::aodv::RoutingTable [class]
    module.add_class('RoutingTable')
    ## aodv-rtable.h: ns3::aodv::RoutingTableEntry [class]
    module.add_class('RoutingTableEntry')
    ## aodv-packet.h: ns3::aodv::RrepAckHeader [class]
    module.add_class('RrepAckHeader', parent=root_module['ns3::Header'])
    ## aodv-packet.h: ns3::aodv::RrepHeader [class]
    module.add_class('RrepHeader', parent=root_module['ns3::Header'])
    ## aodv-packet.h: ns3::aodv::RreqHeader [class]
    module.add_class('RreqHeader', parent=root_module['ns3::Header'])
    ## aodv-packet.h: ns3::aodv::TypeHeader [class]
    module.add_class('TypeHeader', parent=root_module['ns3::Header'])

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
    register_Ns3AodvHelper_methods(root_module, root_module['ns3::AodvHelper'])
    register_Ns3AodvDuplicatePacketDetection_methods(root_module, root_module['ns3::aodv::DuplicatePacketDetection'])
    register_Ns3AodvIdCache_methods(root_module, root_module['ns3::aodv::IdCache'])
    register_Ns3AodvNeighbors_methods(root_module, root_module['ns3::aodv::Neighbors'])
    register_Ns3AodvNeighborsNeighbor_methods(root_module, root_module['ns3::aodv::Neighbors::Neighbor'])
    register_Ns3AodvQueueEntry_methods(root_module, root_module['ns3::aodv::QueueEntry'])
    register_Ns3AodvRequestQueue_methods(root_module, root_module['ns3::aodv::RequestQueue'])
    register_Ns3AodvRerrHeader_methods(root_module, root_module['ns3::aodv::RerrHeader'])
    register_Ns3AodvRoutingProtocol_methods(root_module, root_module['ns3::aodv::RoutingProtocol'])
    register_Ns3AodvRoutingTable_methods(root_module, root_module['ns3::aodv::RoutingTable'])
    register_Ns3AodvRoutingTableEntry_methods(root_module, root_module['ns3::aodv::RoutingTableEntry'])
    register_Ns3AodvRrepAckHeader_methods(root_module, root_module['ns3::aodv::RrepAckHeader'])
    register_Ns3AodvRrepHeader_methods(root_module, root_module['ns3::aodv::RrepHeader'])
    register_Ns3AodvRreqHeader_methods(root_module, root_module['ns3::aodv::RreqHeader'])
    register_Ns3AodvTypeHeader_methods(root_module, root_module['ns3::aodv::TypeHeader'])
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

def register_Ns3AodvDuplicatePacketDetection_methods(root_module, cls):
    ## aodv-dpd.h: ns3::aodv::DuplicatePacketDetection::DuplicatePacketDetection(ns3::aodv::DuplicatePacketDetection const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::DuplicatePacketDetection const &', 'arg0')])
    ## aodv-dpd.h: ns3::aodv::DuplicatePacketDetection::DuplicatePacketDetection(ns3::Time lifetime) [constructor]
    cls.add_constructor([param('ns3::Time', 'lifetime')])
    ## aodv-dpd.h: ns3::Time ns3::aodv::DuplicatePacketDetection::GetLifetime() const [member function]
    cls.add_method('GetLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-dpd.h: bool ns3::aodv::DuplicatePacketDetection::IsDuplicate(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header) [member function]
    cls.add_method('IsDuplicate', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header')])
    ## aodv-dpd.h: void ns3::aodv::DuplicatePacketDetection::SetLifetime(ns3::Time lifetime) [member function]
    cls.add_method('SetLifetime', 
                   'void', 
                   [param('ns3::Time', 'lifetime')])
    return

def register_Ns3AodvIdCache_methods(root_module, cls):
    ## aodv-id-cache.h: ns3::aodv::IdCache::IdCache(ns3::aodv::IdCache const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::IdCache const &', 'arg0')])
    ## aodv-id-cache.h: ns3::aodv::IdCache::IdCache(ns3::Time lifetime) [constructor]
    cls.add_constructor([param('ns3::Time', 'lifetime')])
    ## aodv-id-cache.h: ns3::Time ns3::aodv::IdCache::GetLifeTime() const [member function]
    cls.add_method('GetLifeTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-id-cache.h: uint32_t ns3::aodv::IdCache::GetSize() [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [])
    ## aodv-id-cache.h: bool ns3::aodv::IdCache::IsDuplicate(ns3::Ipv4Address addr, uint32_t id) [member function]
    cls.add_method('IsDuplicate', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'addr'), param('uint32_t', 'id')])
    ## aodv-id-cache.h: void ns3::aodv::IdCache::Purge() [member function]
    cls.add_method('Purge', 
                   'void', 
                   [])
    ## aodv-id-cache.h: void ns3::aodv::IdCache::SetLifetime(ns3::Time lifetime) [member function]
    cls.add_method('SetLifetime', 
                   'void', 
                   [param('ns3::Time', 'lifetime')])
    return

def register_Ns3AodvNeighbors_methods(root_module, cls):
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbors(ns3::aodv::Neighbors const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::Neighbors const &', 'arg0')])
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbors(ns3::Time delay) [constructor]
    cls.add_constructor([param('ns3::Time', 'delay')])
    ## aodv-neighbor.h: void ns3::aodv::Neighbors::AddArpCache(ns3::Ptr<ns3::ArpCache> arg0) [member function]
    cls.add_method('AddArpCache', 
                   'void', 
                   [param('ns3::Ptr< ns3::ArpCache >', 'arg0')])
    ## aodv-neighbor.h: void ns3::aodv::Neighbors::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## aodv-neighbor.h: void ns3::aodv::Neighbors::DelArpCache(ns3::Ptr<ns3::ArpCache> arg0) [member function]
    cls.add_method('DelArpCache', 
                   'void', 
                   [param('ns3::Ptr< ns3::ArpCache >', 'arg0')])
    ## aodv-neighbor.h: ns3::Callback<void, ns3::Ipv4Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::aodv::Neighbors::GetCallback() const [member function]
    cls.add_method('GetCallback', 
                   'ns3::Callback< void, ns3::Ipv4Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## aodv-neighbor.h: ns3::Time ns3::aodv::Neighbors::GetExpireTime(ns3::Ipv4Address addr) [member function]
    cls.add_method('GetExpireTime', 
                   'ns3::Time', 
                   [param('ns3::Ipv4Address', 'addr')])
    ## aodv-neighbor.h: ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::aodv::Neighbors::GetTxErrorCallback() const [member function]
    cls.add_method('GetTxErrorCallback', 
                   'ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## aodv-neighbor.h: bool ns3::aodv::Neighbors::IsNeighbor(ns3::Ipv4Address addr) [member function]
    cls.add_method('IsNeighbor', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'addr')])
    ## aodv-neighbor.h: void ns3::aodv::Neighbors::Purge() [member function]
    cls.add_method('Purge', 
                   'void', 
                   [])
    ## aodv-neighbor.h: void ns3::aodv::Neighbors::ScheduleTimer() [member function]
    cls.add_method('ScheduleTimer', 
                   'void', 
                   [])
    ## aodv-neighbor.h: void ns3::aodv::Neighbors::SetCallback(ns3::Callback<void, ns3::Ipv4Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ipv4Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## aodv-neighbor.h: void ns3::aodv::Neighbors::Update(ns3::Ipv4Address addr, ns3::Time expire) [member function]
    cls.add_method('Update', 
                   'void', 
                   [param('ns3::Ipv4Address', 'addr'), param('ns3::Time', 'expire')])
    return

def register_Ns3AodvNeighborsNeighbor_methods(root_module, cls):
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbor::Neighbor(ns3::aodv::Neighbors::Neighbor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::Neighbors::Neighbor const &', 'arg0')])
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbor::Neighbor(ns3::Ipv4Address ip, ns3::Mac48Address mac, ns3::Time t) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('ns3::Mac48Address', 'mac'), param('ns3::Time', 't')])
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbor::close [variable]
    cls.add_instance_attribute('close', 'bool', is_const=False)
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbor::m_expireTime [variable]
    cls.add_instance_attribute('m_expireTime', 'ns3::Time', is_const=False)
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbor::m_hardwareAddress [variable]
    cls.add_instance_attribute('m_hardwareAddress', 'ns3::Mac48Address', is_const=False)
    ## aodv-neighbor.h: ns3::aodv::Neighbors::Neighbor::m_neighborAddress [variable]
    cls.add_instance_attribute('m_neighborAddress', 'ns3::Ipv4Address', is_const=False)
    return

def register_Ns3AodvQueueEntry_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## aodv-rqueue.h: ns3::aodv::QueueEntry::QueueEntry(ns3::aodv::QueueEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::QueueEntry const &', 'arg0')])
    ## aodv-rqueue.h: ns3::aodv::QueueEntry::QueueEntry(ns3::Ptr<ns3::Packet const> pa=0, ns3::Ipv4Header const & h=ns3::Ipv4Header(), ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb=ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>(), ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb=ns3::Callback<void, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>(), ns3::Time exp=ns3::Simulator::Now( )) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet const >', 'pa', default_value='0'), param('ns3::Ipv4Header const &', 'h', default_value='ns3::Ipv4Header()'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb', default_value='ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>()'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb', default_value='ns3::Callback<void, ns3::Ptr<const ns3::Packet>, const ns3::Ipv4Header&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty>()'), param('ns3::Time', 'exp', default_value='ns3::Simulator::Now( )')])
    ## aodv-rqueue.h: ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::aodv::QueueEntry::GetErrorCallback() const [member function]
    cls.add_method('GetErrorCallback', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## aodv-rqueue.h: ns3::Time ns3::aodv::QueueEntry::GetExpireTime() const [member function]
    cls.add_method('GetExpireTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-rqueue.h: ns3::Ipv4Header ns3::aodv::QueueEntry::GetIpv4Header() const [member function]
    cls.add_method('GetIpv4Header', 
                   'ns3::Ipv4Header', 
                   [], 
                   is_const=True)
    ## aodv-rqueue.h: ns3::Ptr<ns3::Packet const> ns3::aodv::QueueEntry::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## aodv-rqueue.h: ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::aodv::QueueEntry::GetUnicastForwardCallback() const [member function]
    cls.add_method('GetUnicastForwardCallback', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## aodv-rqueue.h: void ns3::aodv::QueueEntry::SetErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('SetErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')])
    ## aodv-rqueue.h: void ns3::aodv::QueueEntry::SetExpireTime(ns3::Time exp) [member function]
    cls.add_method('SetExpireTime', 
                   'void', 
                   [param('ns3::Time', 'exp')])
    ## aodv-rqueue.h: void ns3::aodv::QueueEntry::SetIpv4Header(ns3::Ipv4Header h) [member function]
    cls.add_method('SetIpv4Header', 
                   'void', 
                   [param('ns3::Ipv4Header', 'h')])
    ## aodv-rqueue.h: void ns3::aodv::QueueEntry::SetPacket(ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('SetPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## aodv-rqueue.h: void ns3::aodv::QueueEntry::SetUnicastForwardCallback(ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb) [member function]
    cls.add_method('SetUnicastForwardCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb')])
    return

def register_Ns3AodvRequestQueue_methods(root_module, cls):
    ## aodv-rqueue.h: ns3::aodv::RequestQueue::RequestQueue(ns3::aodv::RequestQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::RequestQueue const &', 'arg0')])
    ## aodv-rqueue.h: ns3::aodv::RequestQueue::RequestQueue(uint32_t maxLen, ns3::Time routeToQueueTimeout) [constructor]
    cls.add_constructor([param('uint32_t', 'maxLen'), param('ns3::Time', 'routeToQueueTimeout')])
    ## aodv-rqueue.h: bool ns3::aodv::RequestQueue::Dequeue(ns3::Ipv4Address dst, ns3::aodv::QueueEntry & entry) [member function]
    cls.add_method('Dequeue', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('ns3::aodv::QueueEntry &', 'entry')])
    ## aodv-rqueue.h: void ns3::aodv::RequestQueue::DropPacketWithDst(ns3::Ipv4Address dst) [member function]
    cls.add_method('DropPacketWithDst', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## aodv-rqueue.h: bool ns3::aodv::RequestQueue::Enqueue(ns3::aodv::QueueEntry & entry) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::aodv::QueueEntry &', 'entry')])
    ## aodv-rqueue.h: bool ns3::aodv::RequestQueue::Find(ns3::Ipv4Address dst) [member function]
    cls.add_method('Find', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## aodv-rqueue.h: uint32_t ns3::aodv::RequestQueue::GetMaxQueueLen() const [member function]
    cls.add_method('GetMaxQueueLen', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## aodv-rqueue.h: ns3::Time ns3::aodv::RequestQueue::GetQueueTimeout() const [member function]
    cls.add_method('GetQueueTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-rqueue.h: uint32_t ns3::aodv::RequestQueue::GetSize() [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [])
    ## aodv-rqueue.h: void ns3::aodv::RequestQueue::SetMaxQueueLen(uint32_t len) [member function]
    cls.add_method('SetMaxQueueLen', 
                   'void', 
                   [param('uint32_t', 'len')])
    ## aodv-rqueue.h: void ns3::aodv::RequestQueue::SetQueueTimeout(ns3::Time t) [member function]
    cls.add_method('SetQueueTimeout', 
                   'void', 
                   [param('ns3::Time', 't')])
    return

def register_Ns3AodvRerrHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## aodv-packet.h: ns3::aodv::RerrHeader::RerrHeader(ns3::aodv::RerrHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::RerrHeader const &', 'arg0')])
    ## aodv-packet.h: ns3::aodv::RerrHeader::RerrHeader() [constructor]
    cls.add_constructor([])
    ## aodv-packet.h: bool ns3::aodv::RerrHeader::AddUnDestination(ns3::Ipv4Address dst, uint32_t seqNo) [member function]
    cls.add_method('AddUnDestination', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('uint32_t', 'seqNo')])
    ## aodv-packet.h: void ns3::aodv::RerrHeader::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## aodv-packet.h: uint32_t ns3::aodv::RerrHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## aodv-packet.h: uint8_t ns3::aodv::RerrHeader::GetDestCount() const [member function]
    cls.add_method('GetDestCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: ns3::TypeId ns3::aodv::RerrHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: bool ns3::aodv::RerrHeader::GetNoDelete() const [member function]
    cls.add_method('GetNoDelete', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint32_t ns3::aodv::RerrHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: static ns3::TypeId ns3::aodv::RerrHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aodv-packet.h: void ns3::aodv::RerrHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: bool ns3::aodv::RerrHeader::RemoveUnDestination(std::pair<ns3::Ipv4Address,unsigned int> & un) [member function]
    cls.add_method('RemoveUnDestination', 
                   'bool', 
                   [param('std::pair< ns3::Ipv4Address, unsigned int > &', 'un')])
    ## aodv-packet.h: void ns3::aodv::RerrHeader::Serialize(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: void ns3::aodv::RerrHeader::SetNoDelete(bool f) [member function]
    cls.add_method('SetNoDelete', 
                   'void', 
                   [param('bool', 'f')])
    return

def register_Ns3AodvRoutingProtocol_methods(root_module, cls):
    ## aodv-routing-protocol.h: ns3::aodv::RoutingProtocol::RoutingProtocol(ns3::aodv::RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::RoutingProtocol const &', 'arg0')])
    ## aodv-routing-protocol.h: ns3::aodv::RoutingProtocol::RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## aodv-routing-protocol.h: bool ns3::aodv::RoutingProtocol::GetBroadcastEnable() const [member function]
    cls.add_method('GetBroadcastEnable', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-routing-protocol.h: bool ns3::aodv::RoutingProtocol::GetDesinationOnlyFlag() const [member function]
    cls.add_method('GetDesinationOnlyFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-routing-protocol.h: bool ns3::aodv::RoutingProtocol::GetGratuitousReplyFlag() const [member function]
    cls.add_method('GetGratuitousReplyFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-routing-protocol.h: bool ns3::aodv::RoutingProtocol::GetHelloEnable() const [member function]
    cls.add_method('GetHelloEnable', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-routing-protocol.h: uint32_t ns3::aodv::RoutingProtocol::GetMaxQueueLen() const [member function]
    cls.add_method('GetMaxQueueLen', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## aodv-routing-protocol.h: ns3::Time ns3::aodv::RoutingProtocol::GetMaxQueueTime() const [member function]
    cls.add_method('GetMaxQueueTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-routing-protocol.h: static ns3::TypeId ns3::aodv::RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True, is_virtual=True)
    ## aodv-routing-protocol.h: bool ns3::aodv::RoutingProtocol::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## aodv-routing-protocol.h: ns3::Ptr<ns3::Ipv4Route> ns3::aodv::RoutingProtocol::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::SetBroadcastEnable(bool f) [member function]
    cls.add_method('SetBroadcastEnable', 
                   'void', 
                   [param('bool', 'f')])
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::SetDesinationOnlyFlag(bool f) [member function]
    cls.add_method('SetDesinationOnlyFlag', 
                   'void', 
                   [param('bool', 'f')])
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::SetGratuitousReplyFlag(bool f) [member function]
    cls.add_method('SetGratuitousReplyFlag', 
                   'void', 
                   [param('bool', 'f')])
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::SetHelloEnable(bool f) [member function]
    cls.add_method('SetHelloEnable', 
                   'void', 
                   [param('bool', 'f')])
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_virtual=True)
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::SetMaxQueueLen(uint32_t len) [member function]
    cls.add_method('SetMaxQueueLen', 
                   'void', 
                   [param('uint32_t', 'len')])
    ## aodv-routing-protocol.h: void ns3::aodv::RoutingProtocol::SetMaxQueueTime(ns3::Time t) [member function]
    cls.add_method('SetMaxQueueTime', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## aodv-routing-protocol.h: ns3::aodv::RoutingProtocol::AODV_PORT [variable]
    cls.add_static_attribute('AODV_PORT', 'uint32_t const', is_const=True)
    return

def register_Ns3AodvRoutingTable_methods(root_module, cls):
    ## aodv-rtable.h: ns3::aodv::RoutingTable::RoutingTable(ns3::aodv::RoutingTable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::RoutingTable const &', 'arg0')])
    ## aodv-rtable.h: ns3::aodv::RoutingTable::RoutingTable(ns3::Time t) [constructor]
    cls.add_constructor([param('ns3::Time', 't')])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTable::AddRoute(ns3::aodv::RoutingTableEntry & r) [member function]
    cls.add_method('AddRoute', 
                   'bool', 
                   [param('ns3::aodv::RoutingTableEntry &', 'r')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTable::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## aodv-rtable.h: void ns3::aodv::RoutingTable::DeleteAllRoutesFromInterface(ns3::Ipv4InterfaceAddress iface) [member function]
    cls.add_method('DeleteAllRoutesFromInterface', 
                   'void', 
                   [param('ns3::Ipv4InterfaceAddress', 'iface')])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTable::DeleteRoute(ns3::Ipv4Address dst) [member function]
    cls.add_method('DeleteRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst')])
    ## aodv-rtable.h: ns3::Time ns3::aodv::RoutingTable::GetBadLinkLifetime() const [member function]
    cls.add_method('GetBadLinkLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: void ns3::aodv::RoutingTable::GetListOfDestinationWithNextHop(ns3::Ipv4Address nextHop, std::map<ns3::Ipv4Address, unsigned int, std::less<ns3::Ipv4Address>, std::allocator<std::pair<ns3::Ipv4Address const, unsigned int> > > & unreachable) [member function]
    cls.add_method('GetListOfDestinationWithNextHop', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nextHop'), param('std::map< ns3::Ipv4Address, unsigned int > &', 'unreachable')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTable::InvalidateRoutesWithDst(std::map<ns3::Ipv4Address, unsigned int, std::less<ns3::Ipv4Address>, std::allocator<std::pair<ns3::Ipv4Address const, unsigned int> > > const & unreachable) [member function]
    cls.add_method('InvalidateRoutesWithDst', 
                   'void', 
                   [param('std::map< ns3::Ipv4Address, unsigned int > const &', 'unreachable')])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTable::LookupRoute(ns3::Ipv4Address dst, ns3::aodv::RoutingTableEntry & rt) [member function]
    cls.add_method('LookupRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('ns3::aodv::RoutingTableEntry &', 'rt')])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTable::LookupValidRoute(ns3::Ipv4Address dst, ns3::aodv::RoutingTableEntry & rt) [member function]
    cls.add_method('LookupValidRoute', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('ns3::aodv::RoutingTableEntry &', 'rt')])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTable::MarkLinkAsUnidirectional(ns3::Ipv4Address neighbor, ns3::Time blacklistTimeout) [member function]
    cls.add_method('MarkLinkAsUnidirectional', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'neighbor'), param('ns3::Time', 'blacklistTimeout')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTable::Print(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## aodv-rtable.h: void ns3::aodv::RoutingTable::Purge() [member function]
    cls.add_method('Purge', 
                   'void', 
                   [])
    ## aodv-rtable.h: void ns3::aodv::RoutingTable::SetBadLinkLifetime(ns3::Time t) [member function]
    cls.add_method('SetBadLinkLifetime', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTable::SetEntryState(ns3::Ipv4Address dst, ns3::aodv::RouteFlags state) [member function]
    cls.add_method('SetEntryState', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'dst'), param('ns3::aodv::RouteFlags', 'state')])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTable::Update(ns3::aodv::RoutingTableEntry & rt) [member function]
    cls.add_method('Update', 
                   'bool', 
                   [param('ns3::aodv::RoutingTableEntry &', 'rt')])
    return

def register_Ns3AodvRoutingTableEntry_methods(root_module, cls):
    ## aodv-rtable.h: ns3::aodv::RoutingTableEntry::RoutingTableEntry(ns3::aodv::RoutingTableEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::RoutingTableEntry const &', 'arg0')])
    ## aodv-rtable.h: ns3::aodv::RoutingTableEntry::RoutingTableEntry(ns3::Ptr<ns3::NetDevice> dev=0, ns3::Ipv4Address dst=ns3::Ipv4Address(), bool vSeqNo=false, uint32_t m_seqNo=0, ns3::Ipv4InterfaceAddress iface=ns3::Ipv4InterfaceAddress(), uint16_t hops=0, ns3::Ipv4Address nextHop=ns3::Ipv4Address(), ns3::Time lifetime=ns3::Simulator::Now( )) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'dev', default_value='0'), param('ns3::Ipv4Address', 'dst', default_value='ns3::Ipv4Address()'), param('bool', 'vSeqNo', default_value='false'), param('uint32_t', 'm_seqNo', default_value='0'), param('ns3::Ipv4InterfaceAddress', 'iface', default_value='ns3::Ipv4InterfaceAddress()'), param('uint16_t', 'hops', default_value='0'), param('ns3::Ipv4Address', 'nextHop', default_value='ns3::Ipv4Address()'), param('ns3::Time', 'lifetime', default_value='ns3::Simulator::Now( )')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::DeleteAllPrecursors() [member function]
    cls.add_method('DeleteAllPrecursors', 
                   'void', 
                   [])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTableEntry::DeletePrecursor(ns3::Ipv4Address id) [member function]
    cls.add_method('DeletePrecursor', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'id')])
    ## aodv-rtable.h: ns3::Time ns3::aodv::RoutingTableEntry::GetBlacklistTimeout() const [member function]
    cls.add_method('GetBlacklistTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: ns3::Ipv4Address ns3::aodv::RoutingTableEntry::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: ns3::aodv::RouteFlags ns3::aodv::RoutingTableEntry::GetFlag() const [member function]
    cls.add_method('GetFlag', 
                   'ns3::aodv::RouteFlags', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: uint16_t ns3::aodv::RoutingTableEntry::GetHop() const [member function]
    cls.add_method('GetHop', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: ns3::Ipv4InterfaceAddress ns3::aodv::RoutingTableEntry::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ipv4InterfaceAddress', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: ns3::Time ns3::aodv::RoutingTableEntry::GetLifeTime() const [member function]
    cls.add_method('GetLifeTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: ns3::Ipv4Address ns3::aodv::RoutingTableEntry::GetNextHop() const [member function]
    cls.add_method('GetNextHop', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: ns3::Ptr<ns3::NetDevice> ns3::aodv::RoutingTableEntry::GetOutputDevice() const [member function]
    cls.add_method('GetOutputDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::GetPrecursors(std::vector<ns3::Ipv4Address, std::allocator<ns3::Ipv4Address> > & prec) const [member function]
    cls.add_method('GetPrecursors', 
                   'void', 
                   [param('std::vector< ns3::Ipv4Address > &', 'prec')], 
                   is_const=True)
    ## aodv-rtable.h: ns3::Ptr<ns3::Ipv4Route> ns3::aodv::RoutingTableEntry::GetRoute() const [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: uint8_t ns3::aodv::RoutingTableEntry::GetRreqCnt() const [member function]
    cls.add_method('GetRreqCnt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: uint32_t ns3::aodv::RoutingTableEntry::GetSeqNo() const [member function]
    cls.add_method('GetSeqNo', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: bool ns3::aodv::RoutingTableEntry::GetValidSeqNo() const [member function]
    cls.add_method('GetValidSeqNo', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::IncrementRreqCnt() [member function]
    cls.add_method('IncrementRreqCnt', 
                   'void', 
                   [])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTableEntry::InsertPrecursor(ns3::Ipv4Address id) [member function]
    cls.add_method('InsertPrecursor', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'id')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::Invalidate(ns3::Time badLinkLifetime) [member function]
    cls.add_method('Invalidate', 
                   'void', 
                   [param('ns3::Time', 'badLinkLifetime')])
    ## aodv-rtable.h: bool ns3::aodv::RoutingTableEntry::IsPrecursorListEmpty() const [member function]
    cls.add_method('IsPrecursorListEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: bool ns3::aodv::RoutingTableEntry::IsUnidirectional() const [member function]
    cls.add_method('IsUnidirectional', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-rtable.h: bool ns3::aodv::RoutingTableEntry::LookupPrecursor(ns3::Ipv4Address id) [member function]
    cls.add_method('LookupPrecursor', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'id')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::Print(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetBalcklistTimeout(ns3::Time t) [member function]
    cls.add_method('SetBalcklistTimeout', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetFlag(ns3::aodv::RouteFlags flag) [member function]
    cls.add_method('SetFlag', 
                   'void', 
                   [param('ns3::aodv::RouteFlags', 'flag')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetHop(uint16_t hop) [member function]
    cls.add_method('SetHop', 
                   'void', 
                   [param('uint16_t', 'hop')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetInterface(ns3::Ipv4InterfaceAddress iface) [member function]
    cls.add_method('SetInterface', 
                   'void', 
                   [param('ns3::Ipv4InterfaceAddress', 'iface')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetLifeTime(ns3::Time lt) [member function]
    cls.add_method('SetLifeTime', 
                   'void', 
                   [param('ns3::Time', 'lt')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetNextHop(ns3::Ipv4Address nextHop) [member function]
    cls.add_method('SetNextHop', 
                   'void', 
                   [param('ns3::Ipv4Address', 'nextHop')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetOutputDevice(ns3::Ptr<ns3::NetDevice> dev) [member function]
    cls.add_method('SetOutputDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'dev')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetRoute(ns3::Ptr<ns3::Ipv4Route> r) [member function]
    cls.add_method('SetRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4Route >', 'r')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetRreqCnt(uint8_t n) [member function]
    cls.add_method('SetRreqCnt', 
                   'void', 
                   [param('uint8_t', 'n')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetSeqNo(uint32_t sn) [member function]
    cls.add_method('SetSeqNo', 
                   'void', 
                   [param('uint32_t', 'sn')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetUnidirectional(bool u) [member function]
    cls.add_method('SetUnidirectional', 
                   'void', 
                   [param('bool', 'u')])
    ## aodv-rtable.h: void ns3::aodv::RoutingTableEntry::SetValidSeqNo(bool s) [member function]
    cls.add_method('SetValidSeqNo', 
                   'void', 
                   [param('bool', 's')])
    ## aodv-rtable.h: ns3::aodv::RoutingTableEntry::m_ackTimer [variable]
    cls.add_instance_attribute('m_ackTimer', 'ns3::Timer', is_const=False)
    return

def register_Ns3AodvRrepAckHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## aodv-packet.h: ns3::aodv::RrepAckHeader::RrepAckHeader(ns3::aodv::RrepAckHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::RrepAckHeader const &', 'arg0')])
    ## aodv-packet.h: ns3::aodv::RrepAckHeader::RrepAckHeader() [constructor]
    cls.add_constructor([])
    ## aodv-packet.h: uint32_t ns3::aodv::RrepAckHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## aodv-packet.h: ns3::TypeId ns3::aodv::RrepAckHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: uint32_t ns3::aodv::RrepAckHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: static ns3::TypeId ns3::aodv::RrepAckHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aodv-packet.h: void ns3::aodv::RrepAckHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: void ns3::aodv::RrepAckHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3AodvRrepHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## aodv-packet.h: ns3::aodv::RrepHeader::RrepHeader(ns3::aodv::RrepHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::RrepHeader const &', 'arg0')])
    ## aodv-packet.h: ns3::aodv::RrepHeader::RrepHeader(uint8_t prefixSize=0, uint8_t hopCount=0, ns3::Ipv4Address dst=ns3::Ipv4Address(), uint32_t dstSeqNo=0, ns3::Ipv4Address origin=ns3::Ipv4Address(), ns3::Time lifetime=ns3::MilliSeconds( )) [constructor]
    cls.add_constructor([param('uint8_t', 'prefixSize', default_value='0'), param('uint8_t', 'hopCount', default_value='0'), param('ns3::Ipv4Address', 'dst', default_value='ns3::Ipv4Address()'), param('uint32_t', 'dstSeqNo', default_value='0'), param('ns3::Ipv4Address', 'origin', default_value='ns3::Ipv4Address()'), param('ns3::Time', 'lifetime', default_value='ns3::MilliSeconds(0)')])
    ## aodv-packet.h: uint32_t ns3::aodv::RrepHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## aodv-packet.h: bool ns3::aodv::RrepHeader::GetAckRequired() const [member function]
    cls.add_method('GetAckRequired', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: ns3::Ipv4Address ns3::aodv::RrepHeader::GetDst() const [member function]
    cls.add_method('GetDst', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint32_t ns3::aodv::RrepHeader::GetDstSeqno() const [member function]
    cls.add_method('GetDstSeqno', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint8_t ns3::aodv::RrepHeader::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: ns3::TypeId ns3::aodv::RrepHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: ns3::Time ns3::aodv::RrepHeader::GetLifeTime() const [member function]
    cls.add_method('GetLifeTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: ns3::Ipv4Address ns3::aodv::RrepHeader::GetOrigin() const [member function]
    cls.add_method('GetOrigin', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint8_t ns3::aodv::RrepHeader::GetPrefixSize() const [member function]
    cls.add_method('GetPrefixSize', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint32_t ns3::aodv::RrepHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: static ns3::TypeId ns3::aodv::RrepHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aodv-packet.h: void ns3::aodv::RrepHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: void ns3::aodv::RrepHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: void ns3::aodv::RrepHeader::SetAckRequired(bool f) [member function]
    cls.add_method('SetAckRequired', 
                   'void', 
                   [param('bool', 'f')])
    ## aodv-packet.h: void ns3::aodv::RrepHeader::SetDst(ns3::Ipv4Address a) [member function]
    cls.add_method('SetDst', 
                   'void', 
                   [param('ns3::Ipv4Address', 'a')])
    ## aodv-packet.h: void ns3::aodv::RrepHeader::SetDstSeqno(uint32_t s) [member function]
    cls.add_method('SetDstSeqno', 
                   'void', 
                   [param('uint32_t', 's')])
    ## aodv-packet.h: void ns3::aodv::RrepHeader::SetHello(ns3::Ipv4Address src, uint32_t srcSeqNo, ns3::Time lifetime) [member function]
    cls.add_method('SetHello', 
                   'void', 
                   [param('ns3::Ipv4Address', 'src'), param('uint32_t', 'srcSeqNo'), param('ns3::Time', 'lifetime')])
    ## aodv-packet.h: void ns3::aodv::RrepHeader::SetHopCount(uint8_t count) [member function]
    cls.add_method('SetHopCount', 
                   'void', 
                   [param('uint8_t', 'count')])
    ## aodv-packet.h: void ns3::aodv::RrepHeader::SetLifeTime(ns3::Time t) [member function]
    cls.add_method('SetLifeTime', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## aodv-packet.h: void ns3::aodv::RrepHeader::SetOrigin(ns3::Ipv4Address a) [member function]
    cls.add_method('SetOrigin', 
                   'void', 
                   [param('ns3::Ipv4Address', 'a')])
    ## aodv-packet.h: void ns3::aodv::RrepHeader::SetPrefixSize(uint8_t sz) [member function]
    cls.add_method('SetPrefixSize', 
                   'void', 
                   [param('uint8_t', 'sz')])
    return

def register_Ns3AodvRreqHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## aodv-packet.h: ns3::aodv::RreqHeader::RreqHeader(ns3::aodv::RreqHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::RreqHeader const &', 'arg0')])
    ## aodv-packet.h: ns3::aodv::RreqHeader::RreqHeader(uint8_t flags=0, uint8_t reserved=0, uint8_t hopCount=0, uint32_t requestID=0, ns3::Ipv4Address dst=ns3::Ipv4Address(), uint32_t dstSeqNo=0, ns3::Ipv4Address origin=ns3::Ipv4Address(), uint32_t originSeqNo=0) [constructor]
    cls.add_constructor([param('uint8_t', 'flags', default_value='0'), param('uint8_t', 'reserved', default_value='0'), param('uint8_t', 'hopCount', default_value='0'), param('uint32_t', 'requestID', default_value='0'), param('ns3::Ipv4Address', 'dst', default_value='ns3::Ipv4Address()'), param('uint32_t', 'dstSeqNo', default_value='0'), param('ns3::Ipv4Address', 'origin', default_value='ns3::Ipv4Address()'), param('uint32_t', 'originSeqNo', default_value='0')])
    ## aodv-packet.h: uint32_t ns3::aodv::RreqHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## aodv-packet.h: bool ns3::aodv::RreqHeader::GetDestinationOnly() const [member function]
    cls.add_method('GetDestinationOnly', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: ns3::Ipv4Address ns3::aodv::RreqHeader::GetDst() const [member function]
    cls.add_method('GetDst', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint32_t ns3::aodv::RreqHeader::GetDstSeqno() const [member function]
    cls.add_method('GetDstSeqno', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: bool ns3::aodv::RreqHeader::GetGratiousRrep() const [member function]
    cls.add_method('GetGratiousRrep', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint8_t ns3::aodv::RreqHeader::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint32_t ns3::aodv::RreqHeader::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: ns3::TypeId ns3::aodv::RreqHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: ns3::Ipv4Address ns3::aodv::RreqHeader::GetOrigin() const [member function]
    cls.add_method('GetOrigin', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint32_t ns3::aodv::RreqHeader::GetOriginSeqno() const [member function]
    cls.add_method('GetOriginSeqno', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: uint32_t ns3::aodv::RreqHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: static ns3::TypeId ns3::aodv::RreqHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aodv-packet.h: bool ns3::aodv::RreqHeader::GetUnknownSeqno() const [member function]
    cls.add_method('GetUnknownSeqno', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: void ns3::aodv::RreqHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: void ns3::aodv::RreqHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetDestinationOnly(bool f) [member function]
    cls.add_method('SetDestinationOnly', 
                   'void', 
                   [param('bool', 'f')])
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetDst(ns3::Ipv4Address a) [member function]
    cls.add_method('SetDst', 
                   'void', 
                   [param('ns3::Ipv4Address', 'a')])
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetDstSeqno(uint32_t s) [member function]
    cls.add_method('SetDstSeqno', 
                   'void', 
                   [param('uint32_t', 's')])
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetGratiousRrep(bool f) [member function]
    cls.add_method('SetGratiousRrep', 
                   'void', 
                   [param('bool', 'f')])
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetHopCount(uint8_t count) [member function]
    cls.add_method('SetHopCount', 
                   'void', 
                   [param('uint8_t', 'count')])
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetId(uint32_t id) [member function]
    cls.add_method('SetId', 
                   'void', 
                   [param('uint32_t', 'id')])
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetOrigin(ns3::Ipv4Address a) [member function]
    cls.add_method('SetOrigin', 
                   'void', 
                   [param('ns3::Ipv4Address', 'a')])
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetOriginSeqno(uint32_t s) [member function]
    cls.add_method('SetOriginSeqno', 
                   'void', 
                   [param('uint32_t', 's')])
    ## aodv-packet.h: void ns3::aodv::RreqHeader::SetUnknownSeqno(bool f) [member function]
    cls.add_method('SetUnknownSeqno', 
                   'void', 
                   [param('bool', 'f')])
    return

def register_Ns3AodvTypeHeader_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## aodv-packet.h: ns3::aodv::TypeHeader::TypeHeader(ns3::aodv::TypeHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::aodv::TypeHeader const &', 'arg0')])
    ## aodv-packet.h: ns3::aodv::TypeHeader::TypeHeader(ns3::aodv::MessageType t) [constructor]
    cls.add_constructor([param('ns3::aodv::MessageType', 't')])
    ## aodv-packet.h: uint32_t ns3::aodv::TypeHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## aodv-packet.h: ns3::aodv::MessageType ns3::aodv::TypeHeader::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::aodv::MessageType', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: ns3::TypeId ns3::aodv::TypeHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: uint32_t ns3::aodv::TypeHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: static ns3::TypeId ns3::aodv::TypeHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aodv-packet.h: bool ns3::aodv::TypeHeader::IsValid() const [member function]
    cls.add_method('IsValid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## aodv-packet.h: void ns3::aodv::TypeHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## aodv-packet.h: void ns3::aodv::TypeHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
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

