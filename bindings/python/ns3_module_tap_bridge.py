from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## tap-bridge.h: ns3::TapBridge [class]
    module.add_class('TapBridge', parent=root_module['ns3::NetDevice'])
    ## tap-bridge.h: ns3::TapBridge::Mode [enumeration]
    module.add_enum('Mode', ['ILLEGAL', 'CONFIGURE_LOCAL', 'USE_LOCAL', 'USE_BRIDGE'], outer_class=root_module['ns3::TapBridge'])
    
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
    register_Ns3TapBridge_methods(root_module, root_module['ns3::TapBridge'])
    return

def register_Ns3TapBridge_methods(root_module, cls):
    ## tap-bridge.h: ns3::TapBridge::TapBridge(ns3::TapBridge const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TapBridge const &', 'arg0')])
    ## tap-bridge.h: static ns3::TypeId ns3::TapBridge::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tap-bridge.h: ns3::TapBridge::TapBridge() [constructor]
    cls.add_constructor([])
    ## tap-bridge.h: ns3::Ptr<ns3::NetDevice> ns3::TapBridge::GetBridgedNetDevice() [member function]
    cls.add_method('GetBridgedNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## tap-bridge.h: void ns3::TapBridge::SetBridgedNetDevice(ns3::Ptr<ns3::NetDevice> bridgedDevice) [member function]
    cls.add_method('SetBridgedNetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'bridgedDevice')])
    ## tap-bridge.h: void ns3::TapBridge::Start(ns3::Time tStart) [member function]
    cls.add_method('Start', 
                   'void', 
                   [param('ns3::Time', 'tStart')])
    ## tap-bridge.h: void ns3::TapBridge::Stop(ns3::Time tStop) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time', 'tStop')])
    ## tap-bridge.h: void ns3::TapBridge::SetMode(ns3::TapBridge::Mode mode) [member function]
    cls.add_method('SetMode', 
                   'void', 
                   [param('ns3::TapBridge::Mode', 'mode')])
    ## tap-bridge.h: ns3::TapBridge::Mode ns3::TapBridge::GetMode() [member function]
    cls.add_method('GetMode', 
                   'ns3::TapBridge::Mode', 
                   [])
    ## tap-bridge.h: void ns3::TapBridge::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## tap-bridge.h: uint32_t ns3::TapBridge::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: ns3::Ptr<ns3::Channel> ns3::TapBridge::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: ns3::Address ns3::TapBridge::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## tap-bridge.h: uint16_t ns3::TapBridge::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: void ns3::TapBridge::SetLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: ns3::Address ns3::TapBridge::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: ns3::Address ns3::TapBridge::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## tap-bridge.h: ns3::Ptr<ns3::Node> ns3::TapBridge::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: void ns3::TapBridge::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: void ns3::TapBridge::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## tap-bridge.h: void ns3::TapBridge::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## tap-bridge.h: bool ns3::TapBridge::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: ns3::Address ns3::TapBridge::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## tap-bridge.h: void ns3::TapBridge::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## tap-bridge.h: void ns3::TapBridge::ReceiveFromBridgedDevice(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<const ns3::Packet> packet, uint16_t protocol, ns3::Address const & src, ns3::Address const & dst, ns3::NetDevice::PacketType packetType) [member function]
    cls.add_method('ReceiveFromBridgedDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocol'), param('ns3::Address const &', 'src'), param('ns3::Address const &', 'dst'), param('ns3::NetDevice::PacketType', 'packetType')], 
                   visibility='protected')
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

