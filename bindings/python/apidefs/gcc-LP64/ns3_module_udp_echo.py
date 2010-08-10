from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## udp-echo-client.h: ns3::UdpEchoClient [class]
    module.add_class('UdpEchoClient', parent=root_module['ns3::Application'])
    ## udp-echo-server.h: ns3::UdpEchoServer [class]
    module.add_class('UdpEchoServer', parent=root_module['ns3::Application'])
    
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
    register_Ns3UdpEchoClient_methods(root_module, root_module['ns3::UdpEchoClient'])
    register_Ns3UdpEchoServer_methods(root_module, root_module['ns3::UdpEchoServer'])
    return

def register_Ns3UdpEchoClient_methods(root_module, cls):
    ## udp-echo-client.h: ns3::UdpEchoClient::UdpEchoClient(ns3::UdpEchoClient const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoClient const &', 'arg0')])
    ## udp-echo-client.h: ns3::UdpEchoClient::UdpEchoClient() [constructor]
    cls.add_constructor([])
    ## udp-echo-client.h: uint32_t ns3::UdpEchoClient::GetDataSize() const [member function]
    cls.add_method('GetDataSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## udp-echo-client.h: static ns3::TypeId ns3::UdpEchoClient::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetDataSize(uint32_t dataSize) [member function]
    cls.add_method('SetDataSize', 
                   'void', 
                   [param('uint32_t', 'dataSize')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetFill(std::string fill) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('std::string', 'fill')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetFill(uint8_t fill, uint32_t dataSize) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('uint8_t', 'fill'), param('uint32_t', 'dataSize')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetFill(uint8_t * fill, uint32_t fillSize, uint32_t dataSize) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('uint8_t *', 'fill'), param('uint32_t', 'fillSize'), param('uint32_t', 'dataSize')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetRemote(ns3::Ipv4Address ip, uint16_t port) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-echo-client.h: void ns3::UdpEchoClient::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-echo-client.h: void ns3::UdpEchoClient::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UdpEchoServer_methods(root_module, cls):
    ## udp-echo-server.h: ns3::UdpEchoServer::UdpEchoServer(ns3::UdpEchoServer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoServer const &', 'arg0')])
    ## udp-echo-server.h: ns3::UdpEchoServer::UdpEchoServer() [constructor]
    cls.add_constructor([])
    ## udp-echo-server.h: static ns3::TypeId ns3::UdpEchoServer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-echo-server.h: void ns3::UdpEchoServer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-echo-server.h: void ns3::UdpEchoServer::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-echo-server.h: void ns3::UdpEchoServer::StopApplication() [member function]
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

