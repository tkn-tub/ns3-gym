from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## ipv4-l3-click-protocol.h: ns3::Ipv4L3ClickProtocol [class]
    module.add_class('Ipv4L3ClickProtocol', parent=root_module['ns3::Ipv4'])
    ## ipv4-click-routing.h: ns3::Ipv4ClickRouting [class]
    module.add_class('Ipv4ClickRouting', parent=root_module['ns3::Ipv4RoutingProtocol'])
    
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
    register_Ns3Ipv4L3ClickProtocol_methods(root_module, root_module['ns3::Ipv4L3ClickProtocol'])
    register_Ns3Ipv4ClickRouting_methods(root_module, root_module['ns3::Ipv4ClickRouting'])
    return

def register_Ns3Ipv4L3ClickProtocol_methods(root_module, cls):
    ## ipv4-l3-click-protocol.h: ns3::Ipv4L3ClickProtocol::Ipv4L3ClickProtocol() [constructor]
    cls.add_constructor([])
    ## ipv4-l3-click-protocol.h: ns3::Ipv4L3ClickProtocol::Ipv4L3ClickProtocol(ns3::Ipv4L3ClickProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4L3ClickProtocol const &', 'arg0')])
    return

def register_Ns3Ipv4ClickRouting_methods(root_module, cls):
    ## ipv4-click-routing.h: ns3::Ipv4ClickRouting::Ipv4ClickRouting() [constructor]
    cls.add_constructor([])
    ## ipv4-click-routing.h: ns3::Ipv4ClickRouting::Ipv4ClickRouting(ns3::Ipv4ClickRouting const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4ClickRouting const &', 'arg0')])
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

