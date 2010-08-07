from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## ipv4-list-routing.h: ns3::Ipv4ListRouting [class]
    module.add_class('Ipv4ListRouting', parent=root_module['ns3::Ipv4RoutingProtocol'])
    ## ipv6-list-routing.h: ns3::Ipv6ListRouting [class]
    module.add_class('Ipv6ListRouting', parent=root_module['ns3::Ipv6RoutingProtocol'])
    
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
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Ipv4ListRouting_methods(root_module, root_module['ns3::Ipv4ListRouting'])
    register_Ns3Ipv6ListRouting_methods(root_module, root_module['ns3::Ipv6ListRouting'])
    return

def register_Ns3Ipv4ListRouting_methods(root_module, cls):
    ## ipv4-list-routing.h: ns3::Ipv4ListRouting::Ipv4ListRouting(ns3::Ipv4ListRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4ListRouting const &', 'arg0')])
    ## ipv4-list-routing.h: ns3::Ipv4ListRouting::Ipv4ListRouting() [constructor]
    cls.add_constructor([])
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::AddRoutingProtocol(ns3::Ptr<ns3::Ipv4RoutingProtocol> routingProtocol, int16_t priority) [member function]
    cls.add_method('AddRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4RoutingProtocol >', 'routingProtocol'), param('int16_t', 'priority')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: uint32_t ns3::Ipv4ListRouting::GetNRoutingProtocols() const [member function]
    cls.add_method('GetNRoutingProtocols', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-list-routing.h: ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4ListRouting::GetRoutingProtocol(uint32_t index, int16_t & priority) const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [param('uint32_t', 'index'), param('int16_t &', 'priority', direction=2)], 
                   is_const=True, is_virtual=True)
    ## ipv4-list-routing.h: static ns3::TypeId ns3::Ipv4ListRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: bool ns3::Ipv4ListRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: ns3::Ptr<ns3::Ipv4Route> ns3::Ipv4ListRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ipv4-list-routing.h: void ns3::Ipv4ListRouting::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Ipv6ListRouting_methods(root_module, cls):
    ## ipv6-list-routing.h: ns3::Ipv6ListRouting::Ipv6ListRouting(ns3::Ipv6ListRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6ListRouting const &', 'arg0')])
    ## ipv6-list-routing.h: ns3::Ipv6ListRouting::Ipv6ListRouting() [constructor]
    cls.add_constructor([])
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::AddRoutingProtocol(ns3::Ptr<ns3::Ipv6RoutingProtocol> routingProtocol, int16_t priority) [member function]
    cls.add_method('AddRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6RoutingProtocol >', 'routingProtocol'), param('int16_t', 'priority')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: uint32_t ns3::Ipv6ListRouting::GetNRoutingProtocols() const [member function]
    cls.add_method('GetNRoutingProtocols', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-list-routing.h: ns3::Ptr<ns3::Ipv6RoutingProtocol> ns3::Ipv6ListRouting::GetRoutingProtocol(uint32_t index, int16_t & priority) const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv6RoutingProtocol >', 
                   [param('uint32_t', 'index'), param('int16_t &', 'priority')], 
                   is_const=True, is_virtual=True)
    ## ipv6-list-routing.h: static ns3::TypeId ns3::Ipv6ListRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyAddRoute(ns3::Ipv6Address dst, ns3::Ipv6Prefix mask, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('NotifyAddRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address::GetZero( )')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv6InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv6InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::NotifyRemoveRoute(ns3::Ipv6Address dst, ns3::Ipv6Prefix mask, ns3::Ipv6Address nextHop, uint32_t interface, ns3::Ipv6Address prefixToUse=ns3::Ipv6Address::GetZero( )) [member function]
    cls.add_method('NotifyRemoveRoute', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst'), param('ns3::Ipv6Prefix', 'mask'), param('ns3::Ipv6Address', 'nextHop'), param('uint32_t', 'interface'), param('ns3::Ipv6Address', 'prefixToUse', default_value='ns3::Ipv6Address::GetZero( )')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: bool ns3::Ipv6ListRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv6Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void,ns3::Ptr<ns3::Ipv6Route>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv6MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv6Header&,ns3::Socket::SocketErrno,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv6Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv6Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv6MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv6Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: ns3::Ptr<ns3::Ipv6Route> ns3::Ipv6ListRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv6Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::SetIpv6(ns3::Ptr<ns3::Ipv6> ipv6) [member function]
    cls.add_method('SetIpv6', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv6 >', 'ipv6')], 
                   is_virtual=True)
    ## ipv6-list-routing.h: void ns3::Ipv6ListRouting::DoDispose() [member function]
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

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

