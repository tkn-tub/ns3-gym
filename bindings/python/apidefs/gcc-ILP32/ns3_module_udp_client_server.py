from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## packet-loss-counter.h: ns3::PacketLossCounter [class]
    module.add_class('PacketLossCounter')
    ## udp-client.h: ns3::UdpClient [class]
    module.add_class('UdpClient', parent=root_module['ns3::Application'])
    ## udp-server.h: ns3::UdpServer [class]
    module.add_class('UdpServer', parent=root_module['ns3::Application'])
    ## udp-trace-client.h: ns3::UdpTraceClient [class]
    module.add_class('UdpTraceClient', parent=root_module['ns3::Application'])
    
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
    register_Ns3PacketLossCounter_methods(root_module, root_module['ns3::PacketLossCounter'])
    register_Ns3UdpClient_methods(root_module, root_module['ns3::UdpClient'])
    register_Ns3UdpServer_methods(root_module, root_module['ns3::UdpServer'])
    register_Ns3UdpTraceClient_methods(root_module, root_module['ns3::UdpTraceClient'])
    return

def register_Ns3PacketLossCounter_methods(root_module, cls):
    ## packet-loss-counter.h: ns3::PacketLossCounter::PacketLossCounter(ns3::PacketLossCounter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketLossCounter const &', 'arg0')])
    ## packet-loss-counter.h: ns3::PacketLossCounter::PacketLossCounter(uint8_t bitmapSize) [constructor]
    cls.add_constructor([param('uint8_t', 'bitmapSize')])
    ## packet-loss-counter.h: uint16_t ns3::PacketLossCounter::GetBitMapSize() const [member function]
    cls.add_method('GetBitMapSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## packet-loss-counter.h: uint32_t ns3::PacketLossCounter::GetLost() const [member function]
    cls.add_method('GetLost', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-loss-counter.h: void ns3::PacketLossCounter::NotifyReceived(uint32_t seq) [member function]
    cls.add_method('NotifyReceived', 
                   'void', 
                   [param('uint32_t', 'seq')])
    ## packet-loss-counter.h: void ns3::PacketLossCounter::SetBitMapSize(uint16_t size) [member function]
    cls.add_method('SetBitMapSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    return

def register_Ns3UdpClient_methods(root_module, cls):
    ## udp-client.h: ns3::UdpClient::UdpClient(ns3::UdpClient const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpClient const &', 'arg0')])
    ## udp-client.h: ns3::UdpClient::UdpClient() [constructor]
    cls.add_constructor([])
    ## udp-client.h: static ns3::TypeId ns3::UdpClient::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-client.h: void ns3::UdpClient::SetRemote(ns3::Ipv4Address ip, uint16_t port) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-client.h: void ns3::UdpClient::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-client.h: void ns3::UdpClient::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-client.h: void ns3::UdpClient::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UdpServer_methods(root_module, cls):
    ## udp-server.h: ns3::UdpServer::UdpServer(ns3::UdpServer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpServer const &', 'arg0')])
    ## udp-server.h: ns3::UdpServer::UdpServer() [constructor]
    cls.add_constructor([])
    ## udp-server.h: uint32_t ns3::UdpServer::GetLost() const [member function]
    cls.add_method('GetLost', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## udp-server.h: uint16_t ns3::UdpServer::GetPacketWindowSize() const [member function]
    cls.add_method('GetPacketWindowSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## udp-server.h: uint32_t ns3::UdpServer::GetReceived() const [member function]
    cls.add_method('GetReceived', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## udp-server.h: static ns3::TypeId ns3::UdpServer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-server.h: void ns3::UdpServer::SetPacketWindowSize(uint16_t size) [member function]
    cls.add_method('SetPacketWindowSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## udp-server.h: void ns3::UdpServer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-server.h: void ns3::UdpServer::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-server.h: void ns3::UdpServer::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UdpTraceClient_methods(root_module, cls):
    ## udp-trace-client.h: ns3::UdpTraceClient::UdpTraceClient(ns3::UdpTraceClient const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpTraceClient const &', 'arg0')])
    ## udp-trace-client.h: ns3::UdpTraceClient::UdpTraceClient() [constructor]
    cls.add_constructor([])
    ## udp-trace-client.h: ns3::UdpTraceClient::UdpTraceClient(ns3::Ipv4Address ip, uint16_t port, char * traceFile) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port'), param('char *', 'traceFile')])
    ## udp-trace-client.h: uint16_t ns3::UdpTraceClient::GetMaxPacketSize() [member function]
    cls.add_method('GetMaxPacketSize', 
                   'uint16_t', 
                   [])
    ## udp-trace-client.h: static ns3::TypeId ns3::UdpTraceClient::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-trace-client.h: void ns3::UdpTraceClient::SetMaxPacketSize(uint16_t maxPacketSize) [member function]
    cls.add_method('SetMaxPacketSize', 
                   'void', 
                   [param('uint16_t', 'maxPacketSize')])
    ## udp-trace-client.h: void ns3::UdpTraceClient::SetRemote(ns3::Ipv4Address ip, uint16_t port) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-trace-client.h: void ns3::UdpTraceClient::SetTraceFile(std::string filename) [member function]
    cls.add_method('SetTraceFile', 
                   'void', 
                   [param('std::string', 'filename')])
    ## udp-trace-client.h: void ns3::UdpTraceClient::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-trace-client.h: void ns3::UdpTraceClient::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-trace-client.h: void ns3::UdpTraceClient::StopApplication() [member function]
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

