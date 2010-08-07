from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## ping6.h: ns3::Ping6 [class]
    module.add_class('Ping6', parent=root_module['ns3::Application'])
    
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
    register_Ns3Ping6_methods(root_module, root_module['ns3::Ping6'])
    return

def register_Ns3Ping6_methods(root_module, cls):
    ## ping6.h: ns3::Ping6::Ping6(ns3::Ping6 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ping6 const &', 'arg0')])
    ## ping6.h: ns3::Ping6::Ping6() [constructor]
    cls.add_constructor([])
    ## ping6.h: static ns3::TypeId ns3::Ping6::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ping6.h: void ns3::Ping6::SetIfIndex(uint32_t ifIndex) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t', 'ifIndex')])
    ## ping6.h: void ns3::Ping6::SetLocal(ns3::Ipv6Address ipv6) [member function]
    cls.add_method('SetLocal', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ipv6')])
    ## ping6.h: void ns3::Ping6::SetRemote(ns3::Ipv6Address ipv6) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ipv6')])
    ## ping6.h: void ns3::Ping6::SetRouters(std::vector<ns3::Ipv6Address, std::allocator<ns3::Ipv6Address> > routers) [member function]
    cls.add_method('SetRouters', 
                   'void', 
                   [param('std::vector< ns3::Ipv6Address >', 'routers')])
    ## ping6.h: void ns3::Ping6::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ping6.h: void ns3::Ping6::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## ping6.h: void ns3::Ping6::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
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

