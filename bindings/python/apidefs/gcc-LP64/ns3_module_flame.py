from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    
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
    
    ## flame-protocol.h: ns3::flame::FlameProtocol [class]
    module.add_class('FlameProtocol', parent=root_module['ns3::MeshL2RoutingProtocol'])
    ## flame-protocol.h: ns3::flame::FlameTag [class]
    module.add_class('FlameTag', parent=root_module['ns3::Tag'])

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3FlameFlameProtocol_methods(root_module, root_module['ns3::flame::FlameProtocol'])
    register_Ns3FlameFlameTag_methods(root_module, root_module['ns3::flame::FlameTag'])
    return

def register_Ns3FlameFlameProtocol_methods(root_module, cls):
    ## flame-protocol.h: ns3::flame::FlameProtocol::FlameProtocol() [constructor]
    cls.add_constructor([])
    ## flame-protocol.h: void ns3::flame::FlameProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-protocol.h: ns3::Mac48Address ns3::flame::FlameProtocol::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [])
    ## flame-protocol.h: static ns3::TypeId ns3::flame::FlameProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-protocol.h: bool ns3::flame::FlameProtocol::Install(ns3::Ptr<ns3::MeshPointDevice> arg0) [member function]
    cls.add_method('Install', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'arg0')])
    ## flame-protocol.h: bool ns3::flame::FlameProtocol::RemoveRoutingStuff(uint32_t fromIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet> packet, uint16_t & protocolType) [member function]
    cls.add_method('RemoveRoutingStuff', 
                   'bool', 
                   [param('uint32_t', 'fromIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t &', 'protocolType')], 
                   is_virtual=True)
    ## flame-protocol.h: void ns3::flame::FlameProtocol::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## flame-protocol.h: bool ns3::flame::FlameProtocol::RequestRoute(uint32_t sourceIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet const> packet, uint16_t protocolType, ns3::Callback<void, bool, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'sourceIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocolType'), param('ns3::Callback< void, bool, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_virtual=True)
    ## flame-protocol.h: void ns3::flame::FlameProtocol::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    return

def register_Ns3FlameFlameTag_methods(root_module, cls):
    ## flame-protocol.h: ns3::flame::FlameTag::FlameTag(ns3::flame::FlameTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameTag const &', 'arg0')])
    ## flame-protocol.h: ns3::flame::FlameTag::FlameTag(ns3::Mac48Address a=ns3::Mac48Address()) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'a', default_value='ns3::Mac48Address()')])
    ## flame-protocol.h: void ns3::flame::FlameTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## flame-protocol.h: ns3::TypeId ns3::flame::FlameTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h: uint32_t ns3::flame::FlameTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h: static ns3::TypeId ns3::flame::FlameTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-protocol.h: void ns3::flame::FlameTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h: void ns3::flame::FlameTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h: ns3::flame::FlameTag::receiver [variable]
    cls.add_instance_attribute('receiver', 'ns3::Mac48Address', is_const=False)
    ## flame-protocol.h: ns3::flame::FlameTag::transmitter [variable]
    cls.add_instance_attribute('transmitter', 'ns3::Mac48Address', is_const=False)
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

