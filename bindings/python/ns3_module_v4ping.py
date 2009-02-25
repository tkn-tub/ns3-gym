from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## v4ping.h: ns3::V4Ping [class]
    module.add_class('V4Ping', parent=root_module['ns3::Application'])
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
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
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3V4Ping_methods(root_module, root_module['ns3::V4Ping'])
    return

def register_Ns3V4Ping_methods(root_module, cls):
    ## v4ping.h: ns3::V4Ping::V4Ping(ns3::V4Ping const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::V4Ping const &', 'arg0')])
    ## v4ping.h: static ns3::TypeId ns3::V4Ping::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## v4ping.h: ns3::V4Ping::V4Ping() [constructor]
    cls.add_constructor([])
    ## v4ping.h: void ns3::V4Ping::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## v4ping.h: void ns3::V4Ping::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## v4ping.h: void ns3::V4Ping::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

