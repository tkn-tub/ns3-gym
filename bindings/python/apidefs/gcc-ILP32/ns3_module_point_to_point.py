from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## point-to-point-helper.h: ns3::PointToPointHelper [class]
    module.add_class('PointToPointHelper', parent=[root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## ppp-header.h: ns3::PppHeader [class]
    module.add_class('PppHeader', parent=root_module['ns3::Header'])
    ## point-to-point-channel.h: ns3::PointToPointChannel [class]
    module.add_class('PointToPointChannel', parent=root_module['ns3::Channel'])
    ## point-to-point-net-device.h: ns3::PointToPointNetDevice [class]
    module.add_class('PointToPointNetDevice', parent=[root_module['ns3::NetDevice'], root_module['ns3::MpiNetDevice']])
    ## point-to-point-remote-channel.h: ns3::PointToPointRemoteChannel [class]
    module.add_class('PointToPointRemoteChannel', parent=root_module['ns3::PointToPointChannel'])
    
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
    register_Ns3PointToPointHelper_methods(root_module, root_module['ns3::PointToPointHelper'])
    register_Ns3PppHeader_methods(root_module, root_module['ns3::PppHeader'])
    register_Ns3PointToPointChannel_methods(root_module, root_module['ns3::PointToPointChannel'])
    register_Ns3PointToPointNetDevice_methods(root_module, root_module['ns3::PointToPointNetDevice'])
    register_Ns3PointToPointRemoteChannel_methods(root_module, root_module['ns3::PointToPointRemoteChannel'])
    return

def register_Ns3PointToPointHelper_methods(root_module, cls):
    ## point-to-point-helper.h: ns3::PointToPointHelper::PointToPointHelper(ns3::PointToPointHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointHelper const &', 'arg0')])
    ## point-to-point-helper.h: ns3::PointToPointHelper::PointToPointHelper() [constructor]
    cls.add_constructor([])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::Ptr<ns3::Node> a, ns3::Ptr<ns3::Node> b) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'a'), param('ns3::Ptr< ns3::Node >', 'b')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::Ptr<ns3::Node> a, std::string bName) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'a'), param('std::string', 'bName')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(std::string aName, ns3::Ptr<ns3::Node> b) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'aName'), param('ns3::Ptr< ns3::Node >', 'b')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(std::string aNode, std::string bNode) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'aNode'), param('std::string', 'bNode')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetChannelAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetChannelAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetDeviceAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## point-to-point-helper.h: void ns3::PointToPointHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PppHeader_methods(root_module, cls):
    ## ppp-header.h: ns3::PppHeader::PppHeader(ns3::PppHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PppHeader const &', 'arg0')])
    ## ppp-header.h: ns3::PppHeader::PppHeader() [constructor]
    cls.add_constructor([])
    ## ppp-header.h: uint32_t ns3::PppHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ppp-header.h: ns3::TypeId ns3::PppHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ppp-header.h: uint16_t ns3::PppHeader::GetProtocol() [member function]
    cls.add_method('GetProtocol', 
                   'uint16_t', 
                   [])
    ## ppp-header.h: uint32_t ns3::PppHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ppp-header.h: static ns3::TypeId ns3::PppHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ppp-header.h: void ns3::PppHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ppp-header.h: void ns3::PppHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ppp-header.h: void ns3::PppHeader::SetProtocol(uint16_t protocol) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint16_t', 'protocol')])
    return

def register_Ns3PointToPointChannel_methods(root_module, cls):
    ## point-to-point-channel.h: ns3::PointToPointChannel::PointToPointChannel(ns3::PointToPointChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointChannel const &', 'arg0')])
    ## point-to-point-channel.h: ns3::PointToPointChannel::PointToPointChannel() [constructor]
    cls.add_constructor([])
    ## point-to-point-channel.h: void ns3::PointToPointChannel::Attach(ns3::Ptr<ns3::PointToPointNetDevice> device) [member function]
    cls.add_method('Attach', 
                   'void', 
                   [param('ns3::Ptr< ns3::PointToPointNetDevice >', 'device')])
    ## point-to-point-channel.h: ns3::Ptr<ns3::NetDevice> ns3::PointToPointChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
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
    ## point-to-point-channel.h: static ns3::TypeId ns3::PointToPointChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## point-to-point-channel.h: bool ns3::PointToPointChannel::TransmitStart(ns3::Ptr<ns3::Packet> p, ns3::Ptr<ns3::PointToPointNetDevice> src, ns3::Time txTime) [member function]
    cls.add_method('TransmitStart', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ptr< ns3::PointToPointNetDevice >', 'src'), param('ns3::Time', 'txTime')], 
                   is_virtual=True)
    ## point-to-point-channel.h: ns3::Time ns3::PointToPointChannel::GetDelay() const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True, visibility='protected')
    ## point-to-point-channel.h: ns3::Ptr<ns3::PointToPointNetDevice> ns3::PointToPointChannel::GetDestination(uint32_t i) const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ptr< ns3::PointToPointNetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, visibility='protected')
    ## point-to-point-channel.h: ns3::Ptr<ns3::PointToPointNetDevice> ns3::PointToPointChannel::GetSource(uint32_t i) const [member function]
    cls.add_method('GetSource', 
                   'ns3::Ptr< ns3::PointToPointNetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, visibility='protected')
    ## point-to-point-channel.h: bool ns3::PointToPointChannel::IsInitialized() const [member function]
    cls.add_method('IsInitialized', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    return

def register_Ns3PointToPointNetDevice_methods(root_module, cls):
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
    ## point-to-point-net-device.h: ns3::Ptr<ns3::Queue> ns3::PointToPointNetDevice::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::Queue >', 
                   [], 
                   is_const=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetReceiveErrorModel(ns3::Ptr<ns3::ErrorModel> em) [member function]
    cls.add_method('SetReceiveErrorModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorModel >', 'em')])
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::Receive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
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
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
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
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
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
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
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
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: ns3::Address ns3::PointToPointNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## point-to-point-net-device.h: bool ns3::PointToPointNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::DoMpiReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoMpiReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='protected', is_virtual=True)
    ## point-to-point-net-device.h: void ns3::PointToPointNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PointToPointRemoteChannel_methods(root_module, cls):
    ## point-to-point-remote-channel.h: ns3::PointToPointRemoteChannel::PointToPointRemoteChannel(ns3::PointToPointRemoteChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointRemoteChannel const &', 'arg0')])
    ## point-to-point-remote-channel.h: ns3::PointToPointRemoteChannel::PointToPointRemoteChannel() [constructor]
    cls.add_constructor([])
    ## point-to-point-remote-channel.h: static ns3::TypeId ns3::PointToPointRemoteChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## point-to-point-remote-channel.h: bool ns3::PointToPointRemoteChannel::TransmitStart(ns3::Ptr<ns3::Packet> p, ns3::Ptr<ns3::PointToPointNetDevice> src, ns3::Time txTime) [member function]
    cls.add_method('TransmitStart', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ptr< ns3::PointToPointNetDevice >', 'src'), param('ns3::Time', 'txTime')], 
                   is_virtual=True)
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

