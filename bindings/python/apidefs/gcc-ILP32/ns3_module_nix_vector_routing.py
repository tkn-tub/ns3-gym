from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## ipv4-nix-vector-helper.h: ns3::Ipv4NixVectorHelper [class]
    module.add_class('Ipv4NixVectorHelper', parent=root_module['ns3::Ipv4RoutingHelper'])
    ## ipv4-nix-vector-routing.h: ns3::Ipv4NixVectorRouting [class]
    module.add_class('Ipv4NixVectorRouting', parent=root_module['ns3::Ipv4RoutingProtocol'])
    typehandlers.add_type_alias('std::map< ns3::Ipv4Address, ns3::Ptr< ns3::NixVector >, std::less< ns3::Ipv4Address >, std::allocator< std::pair< ns3::Ipv4Address const, ns3::Ptr< ns3::NixVector > > > >', 'ns3::NixMap_t')
    typehandlers.add_type_alias('std::map< ns3::Ipv4Address, ns3::Ptr< ns3::NixVector >, std::less< ns3::Ipv4Address >, std::allocator< std::pair< ns3::Ipv4Address const, ns3::Ptr< ns3::NixVector > > > >*', 'ns3::NixMap_t*')
    typehandlers.add_type_alias('std::map< ns3::Ipv4Address, ns3::Ptr< ns3::NixVector >, std::less< ns3::Ipv4Address >, std::allocator< std::pair< ns3::Ipv4Address const, ns3::Ptr< ns3::NixVector > > > >&', 'ns3::NixMap_t&')
    typehandlers.add_type_alias('std::map< ns3::Ipv4Address, ns3::Ptr< ns3::Ipv4Route >, std::less< ns3::Ipv4Address >, std::allocator< std::pair< ns3::Ipv4Address const, ns3::Ptr< ns3::Ipv4Route > > > >', 'ns3::Ipv4RouteMap_t')
    typehandlers.add_type_alias('std::map< ns3::Ipv4Address, ns3::Ptr< ns3::Ipv4Route >, std::less< ns3::Ipv4Address >, std::allocator< std::pair< ns3::Ipv4Address const, ns3::Ptr< ns3::Ipv4Route > > > >*', 'ns3::Ipv4RouteMap_t*')
    typehandlers.add_type_alias('std::map< ns3::Ipv4Address, ns3::Ptr< ns3::Ipv4Route >, std::less< ns3::Ipv4Address >, std::allocator< std::pair< ns3::Ipv4Address const, ns3::Ptr< ns3::Ipv4Route > > > >&', 'ns3::Ipv4RouteMap_t&')
    
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
    register_Ns3Ipv4NixVectorHelper_methods(root_module, root_module['ns3::Ipv4NixVectorHelper'])
    register_Ns3Ipv4NixVectorRouting_methods(root_module, root_module['ns3::Ipv4NixVectorRouting'])
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

def register_Ns3Ipv4NixVectorRouting_methods(root_module, cls):
    ## ipv4-nix-vector-routing.h: ns3::Ipv4NixVectorRouting::Ipv4NixVectorRouting(ns3::Ipv4NixVectorRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4NixVectorRouting const &', 'arg0')])
    ## ipv4-nix-vector-routing.h: ns3::Ipv4NixVectorRouting::Ipv4NixVectorRouting() [constructor]
    cls.add_constructor([])
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::FlushGlobalNixRoutingCache() [member function]
    cls.add_method('FlushGlobalNixRoutingCache', 
                   'void', 
                   [])
    ## ipv4-nix-vector-routing.h: static ns3::TypeId ns3::Ipv4NixVectorRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   visibility='private', is_virtual=True)
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   visibility='private', is_virtual=True)
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   visibility='private', is_virtual=True)
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   visibility='private', is_virtual=True)
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::PrintRoutingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintRoutingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## ipv4-nix-vector-routing.h: bool ns3::Ipv4NixVectorRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void, ns3::Ptr<ns3::Ipv4Route>, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::Ipv4Header const&, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   visibility='private', is_virtual=True)
    ## ipv4-nix-vector-routing.h: ns3::Ptr<ns3::Ipv4Route> ns3::Ipv4NixVectorRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::NetDevice> oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice >', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   visibility='private', is_virtual=True)
    ## ipv4-nix-vector-routing.h: void ns3::Ipv4NixVectorRouting::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
                   visibility='private', is_virtual=True)
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

