from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## csma-channel.h: ns3::WireState [enumeration]
    module.add_enum('WireState', ['IDLE', 'TRANSMITTING', 'PROPAGATING'])
    ## backoff.h: ns3::Backoff [class]
    module.add_class('Backoff')
    ## csma-channel.h: ns3::CsmaDeviceRec [class]
    module.add_class('CsmaDeviceRec')
    ## csma-star-helper.h: ns3::CsmaStarHelper [class]
    module.add_class('CsmaStarHelper')
    ## csma-helper.h: ns3::CsmaHelper [class]
    module.add_class('CsmaHelper', parent=[root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## csma-channel.h: ns3::CsmaChannel [class]
    module.add_class('CsmaChannel', parent=root_module['ns3::Channel'])
    ## csma-net-device.h: ns3::CsmaNetDevice [class]
    module.add_class('CsmaNetDevice', parent=root_module['ns3::NetDevice'])
    ## csma-net-device.h: ns3::CsmaNetDevice::EncapsulationMode [enumeration]
    module.add_enum('EncapsulationMode', ['ILLEGAL', 'DIX', 'LLC'], outer_class=root_module['ns3::CsmaNetDevice'])
    
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
    register_Ns3Backoff_methods(root_module, root_module['ns3::Backoff'])
    register_Ns3CsmaDeviceRec_methods(root_module, root_module['ns3::CsmaDeviceRec'])
    register_Ns3CsmaStarHelper_methods(root_module, root_module['ns3::CsmaStarHelper'])
    register_Ns3CsmaHelper_methods(root_module, root_module['ns3::CsmaHelper'])
    register_Ns3CsmaChannel_methods(root_module, root_module['ns3::CsmaChannel'])
    register_Ns3CsmaNetDevice_methods(root_module, root_module['ns3::CsmaNetDevice'])
    return

def register_Ns3Backoff_methods(root_module, cls):
    ## backoff.h: ns3::Backoff::Backoff(ns3::Backoff const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Backoff const &', 'arg0')])
    ## backoff.h: ns3::Backoff::Backoff() [constructor]
    cls.add_constructor([])
    ## backoff.h: ns3::Backoff::Backoff(ns3::Time slotTime, uint32_t minSlots, uint32_t maxSlots, uint32_t ceiling, uint32_t maxRetries) [constructor]
    cls.add_constructor([param('ns3::Time', 'slotTime'), param('uint32_t', 'minSlots'), param('uint32_t', 'maxSlots'), param('uint32_t', 'ceiling'), param('uint32_t', 'maxRetries')])
    ## backoff.h: ns3::Time ns3::Backoff::GetBackoffTime() [member function]
    cls.add_method('GetBackoffTime', 
                   'ns3::Time', 
                   [])
    ## backoff.h: void ns3::Backoff::IncrNumRetries() [member function]
    cls.add_method('IncrNumRetries', 
                   'void', 
                   [])
    ## backoff.h: bool ns3::Backoff::MaxRetriesReached() [member function]
    cls.add_method('MaxRetriesReached', 
                   'bool', 
                   [])
    ## backoff.h: void ns3::Backoff::ResetBackoffTime() [member function]
    cls.add_method('ResetBackoffTime', 
                   'void', 
                   [])
    ## backoff.h: ns3::Backoff::m_ceiling [variable]
    cls.add_instance_attribute('m_ceiling', 'uint32_t', is_const=False)
    ## backoff.h: ns3::Backoff::m_maxRetries [variable]
    cls.add_instance_attribute('m_maxRetries', 'uint32_t', is_const=False)
    ## backoff.h: ns3::Backoff::m_maxSlots [variable]
    cls.add_instance_attribute('m_maxSlots', 'uint32_t', is_const=False)
    ## backoff.h: ns3::Backoff::m_minSlots [variable]
    cls.add_instance_attribute('m_minSlots', 'uint32_t', is_const=False)
    ## backoff.h: ns3::Backoff::m_slotTime [variable]
    cls.add_instance_attribute('m_slotTime', 'ns3::Time', is_const=False)
    return

def register_Ns3CsmaDeviceRec_methods(root_module, cls):
    ## csma-channel.h: ns3::CsmaDeviceRec::CsmaDeviceRec(ns3::CsmaDeviceRec const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsmaDeviceRec const &', 'arg0')])
    ## csma-channel.h: ns3::CsmaDeviceRec::CsmaDeviceRec() [constructor]
    cls.add_constructor([])
    ## csma-channel.h: ns3::CsmaDeviceRec::CsmaDeviceRec(ns3::Ptr<ns3::CsmaNetDevice> device) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: bool ns3::CsmaDeviceRec::IsActive() [member function]
    cls.add_method('IsActive', 
                   'bool', 
                   [])
    ## csma-channel.h: ns3::CsmaDeviceRec::active [variable]
    cls.add_instance_attribute('active', 'bool', is_const=False)
    ## csma-channel.h: ns3::CsmaDeviceRec::devicePtr [variable]
    cls.add_instance_attribute('devicePtr', 'ns3::Ptr< ns3::CsmaNetDevice >', is_const=False)
    return

def register_Ns3CsmaStarHelper_methods(root_module, cls):
    ## csma-star-helper.h: ns3::CsmaStarHelper::CsmaStarHelper(ns3::CsmaStarHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsmaStarHelper const &', 'arg0')])
    ## csma-star-helper.h: ns3::CsmaStarHelper::CsmaStarHelper(uint32_t numSpokes, ns3::CsmaHelper csmaHelper) [constructor]
    cls.add_constructor([param('uint32_t', 'numSpokes'), param('ns3::CsmaHelper', 'csmaHelper')])
    ## csma-star-helper.h: void ns3::CsmaStarHelper::AssignIpv4Addresses(ns3::Ipv4AddressHelper address) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4AddressHelper', 'address')])
    ## csma-star-helper.h: ns3::Ptr<ns3::Node> ns3::CsmaStarHelper::GetHub() const [member function]
    cls.add_method('GetHub', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## csma-star-helper.h: ns3::NetDeviceContainer ns3::CsmaStarHelper::GetHubDevices() const [member function]
    cls.add_method('GetHubDevices', 
                   'ns3::NetDeviceContainer', 
                   [], 
                   is_const=True)
    ## csma-star-helper.h: ns3::Ipv4Address ns3::CsmaStarHelper::GetHubIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetHubIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## csma-star-helper.h: ns3::NetDeviceContainer ns3::CsmaStarHelper::GetSpokeDevices() const [member function]
    cls.add_method('GetSpokeDevices', 
                   'ns3::NetDeviceContainer', 
                   [], 
                   is_const=True)
    ## csma-star-helper.h: ns3::Ipv4Address ns3::CsmaStarHelper::GetSpokeIpv4Address(uint32_t i) const [member function]
    cls.add_method('GetSpokeIpv4Address', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## csma-star-helper.h: ns3::Ptr<ns3::Node> ns3::CsmaStarHelper::GetSpokeNode(uint32_t i) const [member function]
    cls.add_method('GetSpokeNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## csma-star-helper.h: void ns3::CsmaStarHelper::InstallStack(ns3::InternetStackHelper stack) [member function]
    cls.add_method('InstallStack', 
                   'void', 
                   [param('ns3::InternetStackHelper', 'stack')])
    ## csma-star-helper.h: uint32_t ns3::CsmaStarHelper::SpokeCount() const [member function]
    cls.add_method('SpokeCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3CsmaHelper_methods(root_module, cls):
    ## csma-helper.h: ns3::CsmaHelper::CsmaHelper(ns3::CsmaHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsmaHelper const &', 'arg0')])
    ## csma-helper.h: ns3::CsmaHelper::CsmaHelper() [constructor]
    cls.add_constructor([])
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(std::string name) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::CsmaChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::CsmaChannel >', 'channel')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::Ptr<ns3::Node> node, std::string channelName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string', 'channelName')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(std::string nodeName, ns3::Ptr<ns3::CsmaChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName'), param('ns3::Ptr< ns3::CsmaChannel >', 'channel')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(std::string nodeName, std::string channelName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName'), param('std::string', 'channelName')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::NodeContainer const & c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer const &', 'c')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::NodeContainer const & c, ns3::Ptr<ns3::CsmaChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer const &', 'c'), param('ns3::Ptr< ns3::CsmaChannel >', 'channel')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::NodeContainer const & c, std::string channelName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer const &', 'c'), param('std::string', 'channelName')], 
                   is_const=True)
    ## csma-helper.h: void ns3::CsmaHelper::SetChannelAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetChannelAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## csma-helper.h: void ns3::CsmaHelper::SetDeviceAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## csma-helper.h: void ns3::CsmaHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## csma-helper.h: void ns3::CsmaHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## csma-helper.h: void ns3::CsmaHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3CsmaChannel_methods(root_module, cls):
    ## csma-channel.h: ns3::CsmaChannel::CsmaChannel(ns3::CsmaChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsmaChannel const &', 'arg0')])
    ## csma-channel.h: ns3::CsmaChannel::CsmaChannel() [constructor]
    cls.add_constructor([])
    ## csma-channel.h: int32_t ns3::CsmaChannel::Attach(ns3::Ptr<ns3::CsmaNetDevice> device) [member function]
    cls.add_method('Attach', 
                   'int32_t', 
                   [param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: bool ns3::CsmaChannel::Detach(ns3::Ptr<ns3::CsmaNetDevice> device) [member function]
    cls.add_method('Detach', 
                   'bool', 
                   [param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: bool ns3::CsmaChannel::Detach(uint32_t deviceId) [member function]
    cls.add_method('Detach', 
                   'bool', 
                   [param('uint32_t', 'deviceId')])
    ## csma-channel.h: ns3::Ptr<ns3::CsmaNetDevice> ns3::CsmaChannel::GetCsmaDevice(uint32_t i) const [member function]
    cls.add_method('GetCsmaDevice', 
                   'ns3::Ptr< ns3::CsmaNetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## csma-channel.h: ns3::DataRate ns3::CsmaChannel::GetDataRate() [member function]
    cls.add_method('GetDataRate', 
                   'ns3::DataRate', 
                   [])
    ## csma-channel.h: ns3::Time ns3::CsmaChannel::GetDelay() [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [])
    ## csma-channel.h: ns3::Ptr<ns3::NetDevice> ns3::CsmaChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## csma-channel.h: int32_t ns3::CsmaChannel::GetDeviceNum(ns3::Ptr<ns3::CsmaNetDevice> device) [member function]
    cls.add_method('GetDeviceNum', 
                   'int32_t', 
                   [param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: uint32_t ns3::CsmaChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-channel.h: uint32_t ns3::CsmaChannel::GetNumActDevices() [member function]
    cls.add_method('GetNumActDevices', 
                   'uint32_t', 
                   [])
    ## csma-channel.h: ns3::WireState ns3::CsmaChannel::GetState() [member function]
    cls.add_method('GetState', 
                   'ns3::WireState', 
                   [])
    ## csma-channel.h: static ns3::TypeId ns3::CsmaChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## csma-channel.h: bool ns3::CsmaChannel::IsActive(uint32_t deviceId) [member function]
    cls.add_method('IsActive', 
                   'bool', 
                   [param('uint32_t', 'deviceId')])
    ## csma-channel.h: bool ns3::CsmaChannel::IsBusy() [member function]
    cls.add_method('IsBusy', 
                   'bool', 
                   [])
    ## csma-channel.h: void ns3::CsmaChannel::PropagationCompleteEvent() [member function]
    cls.add_method('PropagationCompleteEvent', 
                   'void', 
                   [])
    ## csma-channel.h: bool ns3::CsmaChannel::Reattach(uint32_t deviceId) [member function]
    cls.add_method('Reattach', 
                   'bool', 
                   [param('uint32_t', 'deviceId')])
    ## csma-channel.h: bool ns3::CsmaChannel::Reattach(ns3::Ptr<ns3::CsmaNetDevice> device) [member function]
    cls.add_method('Reattach', 
                   'bool', 
                   [param('ns3::Ptr< ns3::CsmaNetDevice >', 'device')])
    ## csma-channel.h: bool ns3::CsmaChannel::TransmitEnd() [member function]
    cls.add_method('TransmitEnd', 
                   'bool', 
                   [])
    ## csma-channel.h: bool ns3::CsmaChannel::TransmitStart(ns3::Ptr<ns3::Packet> p, uint32_t srcId) [member function]
    cls.add_method('TransmitStart', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'srcId')])
    return

def register_Ns3CsmaNetDevice_methods(root_module, cls):
    ## csma-net-device.h: static ns3::TypeId ns3::CsmaNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## csma-net-device.h: ns3::CsmaNetDevice::CsmaNetDevice() [constructor]
    cls.add_constructor([])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetInterframeGap(ns3::Time t) [member function]
    cls.add_method('SetInterframeGap', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetBackoffParams(ns3::Time slotTime, uint32_t minSlots, uint32_t maxSlots, uint32_t maxRetries, uint32_t ceiling) [member function]
    cls.add_method('SetBackoffParams', 
                   'void', 
                   [param('ns3::Time', 'slotTime'), param('uint32_t', 'minSlots'), param('uint32_t', 'maxSlots'), param('uint32_t', 'maxRetries'), param('uint32_t', 'ceiling')])
    ## csma-net-device.h: bool ns3::CsmaNetDevice::Attach(ns3::Ptr<ns3::CsmaChannel> ch) [member function]
    cls.add_method('Attach', 
                   'bool', 
                   [param('ns3::Ptr< ns3::CsmaChannel >', 'ch')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetQueue(ns3::Ptr<ns3::Queue> queue) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Queue >', 'queue')])
    ## csma-net-device.h: ns3::Ptr<ns3::Queue> ns3::CsmaNetDevice::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::Queue >', 
                   [], 
                   is_const=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetReceiveErrorModel(ns3::Ptr<ns3::ErrorModel> em) [member function]
    cls.add_method('SetReceiveErrorModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorModel >', 'em')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ptr<ns3::CsmaNetDevice> sender) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ptr< ns3::CsmaNetDevice >', 'sender')])
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsSendEnabled() [member function]
    cls.add_method('IsSendEnabled', 
                   'bool', 
                   [])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetSendEnable(bool enable) [member function]
    cls.add_method('SetSendEnable', 
                   'void', 
                   [param('bool', 'enable')])
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsReceiveEnabled() [member function]
    cls.add_method('IsReceiveEnabled', 
                   'bool', 
                   [])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetReceiveEnable(bool enable) [member function]
    cls.add_method('SetReceiveEnable', 
                   'void', 
                   [param('bool', 'enable')])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetEncapsulationMode(ns3::CsmaNetDevice::EncapsulationMode mode) [member function]
    cls.add_method('SetEncapsulationMode', 
                   'void', 
                   [param('ns3::CsmaNetDevice::EncapsulationMode', 'mode')])
    ## csma-net-device.h: ns3::CsmaNetDevice::EncapsulationMode ns3::CsmaNetDevice::GetEncapsulationMode() [member function]
    cls.add_method('GetEncapsulationMode', 
                   'ns3::CsmaNetDevice::EncapsulationMode', 
                   [])
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## csma-net-device.h: uint32_t ns3::CsmaNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: ns3::Ptr<ns3::Channel> ns3::CsmaNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## csma-net-device.h: uint16_t ns3::CsmaNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## csma-net-device.h: ns3::Address ns3::CsmaNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: ns3::Address ns3::CsmaNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: ns3::Address ns3::CsmaNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## csma-net-device.h: ns3::Ptr<ns3::Node> ns3::CsmaNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## csma-net-device.h: ns3::Address ns3::CsmaNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## csma-net-device.h: bool ns3::CsmaNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## csma-net-device.h: void ns3::CsmaNetDevice::AddHeader(ns3::Ptr<ns3::Packet> p, ns3::Mac48Address source, ns3::Mac48Address dest, uint16_t protocolNumber) [member function]
    cls.add_method('AddHeader', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Mac48Address', 'source'), param('ns3::Mac48Address', 'dest'), param('uint16_t', 'protocolNumber')], 
                   visibility='protected')
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

