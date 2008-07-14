from pybindgen import Module, FileCodeSink, write_preamble, param, retval

def register_types(module):
    root_module = module.get_root()
    
    ## csma-channel.h: ns3::WireState [enumeration]
    module.add_enum('WireState', ['IDLE', 'TRANSMITTING', 'PROPAGATING'])
    ## csma-channel.h: ns3::CsmaDeviceRec [class]
    module.add_class('CsmaDeviceRec')
    ## backoff.h: ns3::Backoff [class]
    module.add_class('Backoff')
    ## csma-channel.h: ns3::CsmaChannel [class]
    module.add_class('CsmaChannel', parent=root_module['ns3::Channel'])
    ## csma-net-device.h: ns3::CsmaNetDevice [class]
    module.add_class('CsmaNetDevice', parent=root_module['ns3::NetDevice'])
    ## csma-net-device.h: ns3::CsmaNetDevice::CsmaEncapsulationMode [enumeration]
    module.add_enum('CsmaEncapsulationMode', ['ETHERNET_V1', 'IP_ARP', 'RAW', 'LLC'], outer_class=root_module['ns3::CsmaNetDevice'])
    
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
    register_Ns3CsmaDeviceRec_methods(root_module, root_module['ns3::CsmaDeviceRec'])
    register_Ns3Backoff_methods(root_module, root_module['ns3::Backoff'])
    register_Ns3CsmaChannel_methods(root_module, root_module['ns3::CsmaChannel'])
    register_Ns3CsmaNetDevice_methods(root_module, root_module['ns3::CsmaNetDevice'])
    return

def register_Ns3CsmaDeviceRec_methods(root_module, cls):
    ## csma-channel.h: ns3::CsmaDeviceRec::devicePtr [variable]
    cls.add_instance_attribute('devicePtr', 'ns3::Ptr< ns3::CsmaNetDevice >', is_const=False)
    ## csma-channel.h: ns3::CsmaDeviceRec::active [variable]
    cls.add_instance_attribute('active', 'bool', is_const=False)
    ## csma-channel.h: ns3::CsmaDeviceRec::CsmaDeviceRec() [constructor]
    cls.add_constructor([], visibility='public')
    ## csma-channel.h: ns3::CsmaDeviceRec::CsmaDeviceRec(ns3::Ptr<ns3::CsmaNetDevice> device) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')], visibility='public')
    ## csma-channel.h: bool ns3::CsmaDeviceRec::IsActive() [member function]
    cls.add_method('IsActive', 'bool', [])
    return

def register_Ns3Backoff_methods(root_module, cls):
    ## backoff.h: ns3::Backoff::m_minSlots [variable]
    cls.add_instance_attribute('m_minSlots', 'uint32_t', is_const=False)
    ## backoff.h: ns3::Backoff::m_maxSlots [variable]
    cls.add_instance_attribute('m_maxSlots', 'uint32_t', is_const=False)
    ## backoff.h: ns3::Backoff::m_ceiling [variable]
    cls.add_instance_attribute('m_ceiling', 'uint32_t', is_const=False)
    ## backoff.h: ns3::Backoff::m_maxRetries [variable]
    cls.add_instance_attribute('m_maxRetries', 'uint32_t', is_const=False)
    ## backoff.h: ns3::Backoff::m_slotTime [variable]
    cls.add_instance_attribute('m_slotTime', 'ns3::Time', is_const=False)
    ## backoff.h: ns3::Backoff::Backoff() [constructor]
    cls.add_constructor([], visibility='public')
    ## backoff.h: ns3::Backoff::Backoff(ns3::Time slotTime, uint32_t minSlots, uint32_t maxSlots, uint32_t ceiling, uint32_t maxRetries) [constructor]
    cls.add_constructor([param('ns3::Time', 'slotTime'), param('uint32_t', 'minSlots'), param('uint32_t', 'maxSlots'), param('uint32_t', 'ceiling'), param('uint32_t', 'maxRetries')], visibility='public')
    ## backoff.h: ns3::Time ns3::Backoff::GetBackoffTime() [member function]
    cls.add_method('GetBackoffTime', 'ns3::Time', [])
    ## backoff.h: void ns3::Backoff::ResetBackoffTime() [member function]
    cls.add_method('ResetBackoffTime', 'void', [])
    ## backoff.h: bool ns3::Backoff::MaxRetriesReached() [member function]
    cls.add_method('MaxRetriesReached', 'bool', [])
    ## backoff.h: void ns3::Backoff::IncrNumRetries() [member function]
    cls.add_method('IncrNumRetries', 'void', [])
    return

def register_Ns3CsmaChannel_methods(root_module, cls):
    ## csma-channel.h: static ns3::TypeId ns3::CsmaChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 'ns3::TypeId', [], is_static=True)
    ## csma-channel.h: ns3::CsmaChannel::CsmaChannel() [constructor]
    cls.add_constructor([], visibility='public')
    ## csma-channel.h: int32_t ns3::CsmaChannel::Attach(ns3::Ptr<ns3::CsmaNetDevice> device) [member function]
    cls.add_method('Attach', 'int32_t', [param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: bool ns3::CsmaChannel::Detach(ns3::Ptr<ns3::CsmaNetDevice> device) [member function]
    cls.add_method('Detach', 'bool', [param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: bool ns3::CsmaChannel::Detach(uint32_t deviceId) [member function]
    cls.add_method('Detach', 'bool', [param('uint32_t', 'deviceId')])
    ## csma-channel.h: bool ns3::CsmaChannel::Reattach(uint32_t deviceId) [member function]
    cls.add_method('Reattach', 'bool', [param('uint32_t', 'deviceId')])
    ## csma-channel.h: bool ns3::CsmaChannel::Reattach(ns3::Ptr<ns3::CsmaNetDevice> device) [member function]
    cls.add_method('Reattach', 'bool', [param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: bool ns3::CsmaChannel::TransmitStart(ns3::Ptr<ns3::Packet> p, uint32_t srcId) [member function]
    cls.add_method('TransmitStart', 'bool', [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'srcId')])
    ## csma-channel.h: bool ns3::CsmaChannel::TransmitEnd() [member function]
    cls.add_method('TransmitEnd', 'bool', [])
    ## csma-channel.h: void ns3::CsmaChannel::PropagationCompleteEvent() [member function]
    cls.add_method('PropagationCompleteEvent', 'void', [])
    ## csma-channel.h: int32_t ns3::CsmaChannel::GetDeviceNum(ns3::Ptr<ns3::CsmaNetDevice> device) [member function]
    cls.add_method('GetDeviceNum', 'int32_t', [param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: ns3::WireState ns3::CsmaChannel::GetState() [member function]
    cls.add_method('GetState', 'ns3::WireState', [])
    ## csma-channel.h: bool ns3::CsmaChannel::IsBusy() [member function]
    cls.add_method('IsBusy', 'bool', [])
    ## csma-channel.h: bool ns3::CsmaChannel::IsActive(uint32_t deviceId) [member function]
    cls.add_method('IsActive', 'bool', [param('uint32_t', 'deviceId')])
    ## csma-channel.h: uint32_t ns3::CsmaChannel::GetNumActDevices() [member function]
    cls.add_method('GetNumActDevices', 'uint32_t', [])
    ## csma-channel.h: uint32_t ns3::CsmaChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 'uint32_t', [], is_const=True, is_virtual=True)
    ## csma-channel.h: ns3::Ptr<ns3::NetDevice> ns3::CsmaChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 'ns3::Ptr< ns3::NetDevice >', [param('uint32_t', 'i')], is_const=True, is_virtual=True)
    ## csma-channel.h: ns3::Ptr<ns3::CsmaNetDevice> ns3::CsmaChannel::GetCsmaDevice(uint32_t i) const [member function]
    cls.add_method('GetCsmaDevice', 'ns3::Ptr< ns3::CsmaNetDevice >', [param('uint32_t', 'i')], is_const=True)
    ## csma-channel.h: ns3::DataRate ns3::CsmaChannel::GetDataRate() [member function]
    cls.add_method('GetDataRate', 'ns3::DataRate', [], is_virtual=True)
    ## csma-channel.h: ns3::Time ns3::CsmaChannel::GetDelay() [member function]
    cls.add_method('GetDelay', 'ns3::Time', [], is_virtual=True)
    return

def register_Ns3CsmaNetDevice_methods(root_module, cls):
    ## csma-net-device.h: static ns3::TypeId ns3::CsmaNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 'ns3::TypeId', [], is_static=True)
    ## csma-net-device.h: ns3::CsmaNetDevice::CsmaNetDevice() [constructor]
    cls.add_constructor([], visibility='public')
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetInterframeGap(ns3::Time t) [member function]
    cls.add_method('SetInterframeGap', 'void', [param('ns3::Time', 't')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetBackoffParams(ns3::Time slotTime, uint32_t minSlots, uint32_t maxSlots, uint32_t maxRetries, uint32_t ceiling) [member function]
    cls.add_method('SetBackoffParams', 'void', [param('ns3::Time', 'slotTime'), param('uint32_t', 'minSlots'), param('uint32_t', 'maxSlots'), param('uint32_t', 'maxRetries'), param('uint32_t', 'ceiling')])
    ## csma-net-device.h: bool ns3::CsmaNetDevice::Attach(ns3::Ptr<ns3::CsmaChannel> ch) [member function]
    cls.add_method('Attach', 'bool', [param('ns3::Ptr< ns3::CsmaChannel >', 'ch')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetQueue(ns3::Ptr<ns3::Queue> queue) [member function]
    cls.add_method('SetQueue', 'void', [param('ns3::Ptr< ns3::Queue >', 'queue')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetReceiveErrorModel(ns3::Ptr<ns3::ErrorModel> em) [member function]
    cls.add_method('SetReceiveErrorModel', 'void', [param('ns3::Ptr< ns3::ErrorModel >', 'em')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ptr<ns3::CsmaNetDevice> sender) [member function]
    cls.add_method('Receive', 'void', [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ptr< ns3::CsmaNetDevice >', 'sender')])
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsSendEnabled() [member function]
    cls.add_method('IsSendEnabled', 'bool', [])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetSendEnable(bool enable) [member function]
    cls.add_method('SetSendEnable', 'void', [param('bool', 'enable')])
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsReceiveEnabled() [member function]
    cls.add_method('IsReceiveEnabled', 'bool', [])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetReceiveEnable(bool enable) [member function]
    cls.add_method('SetReceiveEnable', 'void', [param('bool', 'enable')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetAddress(ns3::Mac48Address addr) [member function]
    cls.add_method('SetAddress', 'void', [param('ns3::Mac48Address', 'addr')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetName(std::string const name) [member function]
    cls.add_method('SetName', 'void', [param('std::string', 'name', is_const=True)], is_virtual=True)
    ## csma-net-device.h: std::string ns3::CsmaNetDevice::GetName() const [member function]
    cls.add_method('GetName', 'std::string', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 'void', [param('uint32_t', 'index', is_const=True)], is_virtual=True)
    ## csma-net-device.h: uint32_t ns3::CsmaNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 'uint32_t', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: ns3::Ptr<ns3::Channel> ns3::CsmaNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 'ns3::Ptr< ns3::Channel >', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: ns3::Address ns3::CsmaNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 'ns3::Address', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 'bool', [param('uint16_t', 'mtu', is_const=True)], is_virtual=True)
    ## csma-net-device.h: uint16_t ns3::CsmaNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 'uint16_t', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 'bool', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetLinkChangeCallback', 'void', [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 'bool', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: ns3::Address ns3::CsmaNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 'ns3::Address', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 'bool', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: ns3::Address ns3::CsmaNetDevice::GetMulticast() const [member function]
    cls.add_method('GetMulticast', 'ns3::Address', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: ns3::Address ns3::CsmaNetDevice::MakeMulticastAddress(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('MakeMulticastAddress', 'ns3::Address', [param('ns3::Ipv4Address', 'multicastGroup')], is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 'bool', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 'bool', [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address&', 'dest', is_const=True), param('uint16_t', 'protocolNumber')], is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 'bool', [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address&', 'source', is_const=True), param('ns3::Address&', 'dest', is_const=True), param('uint16_t', 'protocolNumber')], is_virtual=True)
    ## csma-net-device.h: ns3::Ptr<ns3::Node> ns3::CsmaNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 'ns3::Ptr< ns3::Node >', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 'void', [param('ns3::Ptr< ns3::Node >', 'node')], is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 'bool', [], is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType> cb) [member function]
    cls.add_method('SetReceiveCallback', 'void', [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet >, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType >', 'cb')], is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 'void', [], visibility='protected', is_virtual=True)
    ## csma-net-device.h: ns3::Ptr<ns3::Queue> ns3::CsmaNetDevice::GetQueue() const [member function]
    cls.add_method('GetQueue', 'ns3::Ptr< ns3::Queue >', [], is_const=True, visibility='protected')
    ## csma-net-device.h: void ns3::CsmaNetDevice::AddHeader(ns3::Ptr<ns3::Packet> p, ns3::Mac48Address source, ns3::Mac48Address dest, uint16_t protocolNumber) [member function]
    cls.add_method('AddHeader', 'void', [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Mac48Address', 'source'), param('ns3::Mac48Address', 'dest'), param('uint16_t', 'protocolNumber')], visibility='protected')
    ## csma-net-device.h: bool ns3::CsmaNetDevice::ProcessHeader(ns3::Ptr<ns3::Packet> p, uint16_t & param) [member function]
    cls.add_method('ProcessHeader', 'bool', [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint16_t&', 'param')], visibility='protected')
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

