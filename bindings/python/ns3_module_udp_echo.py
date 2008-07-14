from pybindgen import Module, FileCodeSink, write_preamble, param, retval

def register_types(module):
    root_module = module.get_root()
    
    ## udp-echo-client.h: ns3::UdpEchoClient [class]
    module.add_class('UdpEchoClient', parent=root_module['ns3::Application'])
    ## udp-echo-server.h: ns3::UdpEchoServer [class]
    module.add_class('UdpEchoServer', parent=root_module['ns3::Application'])
    
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
    register_Ns3UdpEchoClient_methods(root_module, root_module['ns3::UdpEchoClient'])
    register_Ns3UdpEchoServer_methods(root_module, root_module['ns3::UdpEchoServer'])
    return

def register_Ns3UdpEchoClient_methods(root_module, cls):
    ## udp-echo-client.h: static ns3::TypeId ns3::UdpEchoClient::GetTypeId() [member function]
    cls.add_method('GetTypeId', 'ns3::TypeId', [], is_static=True)
    ## udp-echo-client.h: ns3::UdpEchoClient::UdpEchoClient() [constructor]
    cls.add_constructor([], visibility='public')
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetRemote(ns3::Ipv4Address ip, uint16_t port) [member function]
    cls.add_method('SetRemote', 'void', [param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::DoDispose() [member function]
    cls.add_method('DoDispose', 'void', [], visibility='protected', is_virtual=True)
    ## udp-echo-client.h: void ns3::UdpEchoClient::StartApplication() [member function]
    cls.add_method('StartApplication', 'void', [], visibility='private', is_virtual=True)
    ## udp-echo-client.h: void ns3::UdpEchoClient::StopApplication() [member function]
    cls.add_method('StopApplication', 'void', [], visibility='private', is_virtual=True)
    return

def register_Ns3UdpEchoServer_methods(root_module, cls):
    ## udp-echo-server.h: static ns3::TypeId ns3::UdpEchoServer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 'ns3::TypeId', [], is_static=True)
    ## udp-echo-server.h: ns3::UdpEchoServer::UdpEchoServer() [constructor]
    cls.add_constructor([], visibility='public')
    ## udp-echo-server.h: void ns3::UdpEchoServer::DoDispose() [member function]
    cls.add_method('DoDispose', 'void', [], visibility='protected', is_virtual=True)
    ## udp-echo-server.h: void ns3::UdpEchoServer::StartApplication() [member function]
    cls.add_method('StartApplication', 'void', [], visibility='private', is_virtual=True)
    ## udp-echo-server.h: void ns3::UdpEchoServer::StopApplication() [member function]
    cls.add_method('StopApplication', 'void', [], visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
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

