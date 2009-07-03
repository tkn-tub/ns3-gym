from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## ipv4-routing-table-entry.h: ns3::Ipv4MulticastRoutingTableEntry [class]
    module.add_class('Ipv4MulticastRoutingTableEntry')
    ## ipv4-routing-table-entry.h: ns3::Ipv4RoutingTableEntry [class]
    module.add_class('Ipv4RoutingTableEntry')
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting [class]
    module.add_class('Ipv4StaticRouting', parent=root_module['ns3::Ipv4RoutingProtocol'])
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace addressUtils
    
    nested_module = module.add_cpp_namespace('addressUtils')
    register_types_ns3_addressUtils(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_TimeStepPrecision(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Ipv4MulticastRoutingTableEntry_methods(root_module, root_module['ns3::Ipv4MulticastRoutingTableEntry'])
    register_Ns3Ipv4RoutingTableEntry_methods(root_module, root_module['ns3::Ipv4RoutingTableEntry'])
    register_Ns3Ipv4StaticRouting_methods(root_module, root_module['ns3::Ipv4StaticRouting'])
    return

def register_Ns3Ipv4MulticastRoutingTableEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv4-routing-table-entry.h: ns3::Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry() [constructor]
    cls.add_constructor([])
    ## ipv4-routing-table-entry.h: ns3::Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry(ns3::Ipv4MulticastRoutingTableEntry const & route) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MulticastRoutingTableEntry const &', 'route')])
    ## ipv4-routing-table-entry.h: ns3::Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry(ns3::Ipv4MulticastRoutingTableEntry const * route) [constructor]
    cls.add_constructor([param('ns3::Ipv4MulticastRoutingTableEntry const *', 'route')])
    ## ipv4-routing-table-entry.h: static ns3::Ipv4MulticastRoutingTableEntry ns3::Ipv4MulticastRoutingTableEntry::CreateMulticastRoute(ns3::Ipv4Address origin, ns3::Ipv4Address group, uint32_t inputInterface, std::vector<unsigned int, std::allocator<unsigned int> > outputInterfaces) [member function]
    cls.add_method('CreateMulticastRoute', 
                   'ns3::Ipv4MulticastRoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'origin'), param('ns3::Ipv4Address', 'group'), param('uint32_t', 'inputInterface'), param('std::vector< unsigned int >', 'outputInterfaces')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4MulticastRoutingTableEntry::GetGroup() const [member function]
    cls.add_method('GetGroup', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: uint32_t ns3::Ipv4MulticastRoutingTableEntry::GetInputInterface() const [member function]
    cls.add_method('GetInputInterface', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: uint32_t ns3::Ipv4MulticastRoutingTableEntry::GetNOutputInterfaces() const [member function]
    cls.add_method('GetNOutputInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4MulticastRoutingTableEntry::GetOrigin() const [member function]
    cls.add_method('GetOrigin', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: uint32_t ns3::Ipv4MulticastRoutingTableEntry::GetOutputInterface(uint32_t n) const [member function]
    cls.add_method('GetOutputInterface', 
                   'uint32_t', 
                   [param('uint32_t', 'n')], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: std::vector<unsigned int, std::allocator<unsigned int> > ns3::Ipv4MulticastRoutingTableEntry::GetOutputInterfaces() const [member function]
    cls.add_method('GetOutputInterfaces', 
                   'std::vector< unsigned int >', 
                   [], 
                   is_const=True)
    return

def register_Ns3Ipv4RoutingTableEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv4-routing-table-entry.h: ns3::Ipv4RoutingTableEntry::Ipv4RoutingTableEntry() [constructor]
    cls.add_constructor([])
    ## ipv4-routing-table-entry.h: ns3::Ipv4RoutingTableEntry::Ipv4RoutingTableEntry(ns3::Ipv4RoutingTableEntry const & route) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4RoutingTableEntry const &', 'route')])
    ## ipv4-routing-table-entry.h: ns3::Ipv4RoutingTableEntry::Ipv4RoutingTableEntry(ns3::Ipv4RoutingTableEntry const * route) [constructor]
    cls.add_constructor([param('ns3::Ipv4RoutingTableEntry const *', 'route')])
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateDefaultRoute(ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('CreateDefaultRoute', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateHostRouteTo(ns3::Ipv4Address dest, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('CreateHostRouteTo', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'dest'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateHostRouteTo(ns3::Ipv4Address dest, uint32_t interface) [member function]
    cls.add_method('CreateHostRouteTo', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'dest'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, ns3::Ipv4Address nextHop, uint32_t interface) [member function]
    cls.add_method('CreateNetworkRouteTo', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('ns3::Ipv4Address', 'nextHop'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: static ns3::Ipv4RoutingTableEntry ns3::Ipv4RoutingTableEntry::CreateNetworkRouteTo(ns3::Ipv4Address network, ns3::Ipv4Mask networkMask, uint32_t interface) [member function]
    cls.add_method('CreateNetworkRouteTo', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'networkMask'), param('uint32_t', 'interface')], 
                   is_static=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4RoutingTableEntry::GetDest() const [member function]
    cls.add_method('GetDest', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4RoutingTableEntry::GetDestNetwork() const [member function]
    cls.add_method('GetDestNetwork', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Mask ns3::Ipv4RoutingTableEntry::GetDestNetworkMask() const [member function]
    cls.add_method('GetDestNetworkMask', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: ns3::Ipv4Address ns3::Ipv4RoutingTableEntry::GetGateway() const [member function]
    cls.add_method('GetGateway', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: uint32_t ns3::Ipv4RoutingTableEntry::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: bool ns3::Ipv4RoutingTableEntry::IsDefault() const [member function]
    cls.add_method('IsDefault', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: bool ns3::Ipv4RoutingTableEntry::IsGateway() const [member function]
    cls.add_method('IsGateway', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: bool ns3::Ipv4RoutingTableEntry::IsHost() const [member function]
    cls.add_method('IsHost', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-routing-table-entry.h: bool ns3::Ipv4RoutingTableEntry::IsNetwork() const [member function]
    cls.add_method('IsNetwork', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3Ipv4StaticRouting_methods(root_module, cls):
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting::Ipv4StaticRouting(ns3::Ipv4StaticRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4StaticRouting const &', 'arg0')])
    ## ipv4-static-routing.h: static ns3::TypeId ns3::Ipv4StaticRouting::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-static-routing.h: ns3::Ipv4StaticRouting::Ipv4StaticRouting() [constructor]
    cls.add_constructor([])
    ## ipv4-static-routing.h: ns3::Ptr<ns3::Ipv4Route> ns3::Ipv4StaticRouting::RouteOutput(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, uint32_t oif, ns3::Socket::SocketErrno & sockerr) [member function]
    cls.add_method('RouteOutput', 
                   'ns3::Ptr< ns3::Ipv4Route >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('uint32_t', 'oif'), param('ns3::Socket::SocketErrno &', 'sockerr')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: bool ns3::Ipv4StaticRouting::RouteInput(ns3::Ptr<ns3::Packet const> p, ns3::Ipv4Header const & header, ns3::Ptr<const ns3::NetDevice> idev, ns3::Callback<void,ns3::Ptr<ns3::Ipv4Route>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Ipv4MulticastRoute>,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> mcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,unsigned int,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> lcb, ns3::Callback<void,ns3::Ptr<const ns3::Packet>,const ns3::Ipv4Header&,ns3::Socket::SocketErrno,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::NetDevice const >', 'idev'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4Route >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Ipv4MulticastRoute >, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'mcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'lcb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::Ipv4Header const &, ns3::Socket::SocketErrno, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::NotifyAddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyAddAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::NotifyRemoveAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('NotifyRemoveAddress', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_virtual=True)
    ## ipv4-static-routing.h: void ns3::Ipv4StaticRouting::SetIpv4(ns3::Ptr<ns3::Ipv4> ipv4) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4')], 
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
    ## ipv4-static-routing.h: ns3::Ipv4RoutingTableEntry ns3::Ipv4StaticRouting::GetDefaultRoute() [member function]
    cls.add_method('GetDefaultRoute', 
                   'ns3::Ipv4RoutingTableEntry', 
                   [])
    ## ipv4-static-routing.h: ns3::Ipv4RoutingTableEntry ns3::Ipv4StaticRouting::GetRoute(uint32_t i) [member function]
    cls.add_method('GetRoute', 
                   'ns3::Ipv4RoutingTableEntry', 
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
    ## ipv4-static-routing.h: ns3::Ipv4MulticastRoutingTableEntry ns3::Ipv4StaticRouting::GetMulticastRoute(uint32_t i) const [member function]
    cls.add_method('GetMulticastRoute', 
                   'ns3::Ipv4MulticastRoutingTableEntry', 
                   [param('uint32_t', 'i')], 
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
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

