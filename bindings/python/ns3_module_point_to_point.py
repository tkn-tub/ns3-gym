from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## point-to-point-channel.h: ns3::PointToPointChannel [class]
    module.add_class('PointToPointChannel', parent=root_module['ns3::Channel'])
    ## point-to-point-net-device.h: ns3::PointToPointNetDevice [class]
    module.add_class('PointToPointNetDevice', parent=root_module['ns3::NetDevice'])
    
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
    register_Ns3PointToPointChannel_methods(root_module, root_module['ns3::PointToPointChannel'])
    register_Ns3PointToPointNetDevice_methods(root_module, root_module['ns3::PointToPointNetDevice'])
    return

def register_Ns3PointToPointChannel_methods(root_module, cls):
    ## point-to-point-channel.h: ns3::PointToPointChannel::PointToPointChannel(ns3::PointToPointChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointChannel const &', 'arg0')])
    ## point-to-point-channel.h: static ns3::TypeId ns3::PointToPointChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## point-to-point-channel.h: ns3::PointToPointChannel::PointToPointChannel() [constructor]
    cls.add_constructor([])
    ## point-to-point-channel.h: void ns3::PointToPointChannel::Attach(ns3::Ptr<ns3::PointToPointNetDevice> device) [member function]
    cls.add_method('Attach', 
                   'void', 
                   [param('ns3::Ptr< ns3::PointToPointNetDevice >', 'device')])
    ## point-to-point-channel.h: bool ns3::PointToPointChannel::TransmitStart(ns3::Ptr<ns3::Packet> p, ns3::Ptr<ns3::PointToPointNetDevice> src, ns3::Time txTime) [member function]
    cls.add_method('TransmitStart', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ptr< ns3::PointToPointNetDevice >', 'src'), param('ns3::Time', 'txTime')])
    ## point-to-point-channel.h: uint32_t ns3::PointToPointChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-channel.h: ns3::Ptr<ns3::PointToPointNetDevice> ns3::PointToPointChannel::GetPointToPointDevice(uint32_t i) const [member function]
    cls.add_method('GetPointToPointDevice', 
                   'ns3::Ptr< ns3::PointToPointNetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## point-to-point-channel.h: ns3::Ptr<ns3::NetDevice> ns3::PointToPointChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3PointToPointNetDevice_methods(root_module, cls):
    ## point-to-point-net-device.h: ns3::PointToPointNetDevice::PointToPointNetDevice(ns3::PointToPointNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointNetDevice const &', 'arg0')])
    ## point-to-point-net-device.h: static ns3::TypeId ns3::PointToPointNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## point-to-point-net-device.h: ns3::PointToPointNetDevice::PointToPointNetDevice() [constructor]
    cls.add_constructor([])
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetDataRate(ns3::DataRate bps) [member function]
    cls.add_method('SetDataRate', 
                   'void', 
                   [param('ns3::DataRate', 'bps')])
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetInterframeGap(ns3::Time t) [member function]
    cls.add_method('SetInterframeGap', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::Attach(ns3::Ptr<ns3::PointToPointChannel> ch) [member function]
    cls.add_method('Attach', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PointToPointChannel >', 'ch')])
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetQueue(ns3::Ptr<ns3::Queue> queue) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Queue >', 'queue')])
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetReceiveErrorModel(ns3::Ptr<ns3::ErrorModel> em) [member function]
    cls.add_method('SetReceiveErrorModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorModel >', 'em')])
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::Receive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetAddress(ns3::Mac48Address addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'addr')])
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetFrameSize(uint16_t frameSize) [member function]
    cls.add_method('SetFrameSize', 
                   'void', 
                   [param('uint16_t', 'frameSize')])
    ## point-to-point-net-device.h: uint16_t ns3::PointToPointNetDevice::GetFrameSize() const [member function]
    cls.add_method('GetFrameSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetName(std::string const name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('std::string const', 'name')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: std::string ns3::PointToPointNetDevice::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: uint32_t ns3::PointToPointNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: ns3::Ptr<ns3::Channel> ns3::PointToPointNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: ns3::Address ns3::PointToPointNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: uint16_t ns3::PointToPointNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: ns3::Address ns3::PointToPointNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: ns3::Address ns3::PointToPointNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: ns3::Ptr<ns3::Node> ns3::PointToPointNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: ns3::Address ns3::PointToPointNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType >', 'cb')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::DoDispose() [member function]
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

