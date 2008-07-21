from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    
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
    
    ## olsr-header.h: ns3::olsr::MessageHeader [class]
    module.add_class('MessageHeader', parent=root_module['ns3::Header'])
    ## olsr-header.h: ns3::olsr::MessageHeader::MessageType [enumeration]
    module.add_enum('MessageType', ['HELLO_MESSAGE', 'TC_MESSAGE', 'MID_MESSAGE', 'HNA_MESSAGE'], outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid [struct]
    module.add_class('Mid', outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello [struct]
    module.add_class('Hello', outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::LinkMessage [struct]
    module.add_class('LinkMessage', outer_class=root_module['ns3::olsr::MessageHeader::Hello'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc [struct]
    module.add_class('Tc', outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna [struct]
    module.add_class('Hna', outer_class=root_module['ns3::olsr::MessageHeader'])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Association [struct]
    module.add_class('Association', outer_class=root_module['ns3::olsr::MessageHeader::Hna'])
    ## olsr-agent.h: ns3::olsr::Agent [class]
    module.add_class('Agent', parent=root_module['ns3::Object'])
    ## olsr-header.h: ns3::olsr::PacketHeader [class]
    module.add_class('PacketHeader', parent=root_module['ns3::Header'])

def register_methods(root_module):
    register_Ns3OlsrMessageHeader_methods(root_module, root_module['ns3::olsr::MessageHeader'])
    register_Ns3OlsrMessageHeaderMid_methods(root_module, root_module['ns3::olsr::MessageHeader::Mid'])
    register_Ns3OlsrMessageHeaderHello_methods(root_module, root_module['ns3::olsr::MessageHeader::Hello'])
    register_Ns3OlsrMessageHeaderHelloLinkMessage_methods(root_module, root_module['ns3::olsr::MessageHeader::Hello::LinkMessage'])
    register_Ns3OlsrMessageHeaderTc_methods(root_module, root_module['ns3::olsr::MessageHeader::Tc'])
    register_Ns3OlsrMessageHeaderHna_methods(root_module, root_module['ns3::olsr::MessageHeader::Hna'])
    register_Ns3OlsrMessageHeaderHnaAssociation_methods(root_module, root_module['ns3::olsr::MessageHeader::Hna::Association'])
    register_Ns3OlsrAgent_methods(root_module, root_module['ns3::olsr::Agent'])
    register_Ns3OlsrPacketHeader_methods(root_module, root_module['ns3::olsr::PacketHeader'])
    return

def register_Ns3OlsrMessageHeader_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::MessageHeader(ns3::olsr::MessageHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader&', 'arg0', is_const=True)])
    ## olsr-header.h: ns3::olsr::MessageHeader::MessageHeader() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello & ns3::olsr::MessageHeader::GetHello() [member function]
    cls.add_method('GetHello', 
                   'ns3::olsr::MessageHeader::Hello&', 
                   [])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello const & ns3::olsr::MessageHeader::GetHello() const [member function]
    cls.add_method('GetHello', 
                   retval('ns3::olsr::MessageHeader::Hello&', is_const=True), 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna & ns3::olsr::MessageHeader::GetHna() [member function]
    cls.add_method('GetHna', 
                   'ns3::olsr::MessageHeader::Hna&', 
                   [])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna const & ns3::olsr::MessageHeader::GetHna() const [member function]
    cls.add_method('GetHna', 
                   retval('ns3::olsr::MessageHeader::Hna&', is_const=True), 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint8_t ns3::olsr::MessageHeader::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::TypeId ns3::olsr::MessageHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: uint16_t ns3::olsr::MessageHeader::GetMessageSequenceNumber() const [member function]
    cls.add_method('GetMessageSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::MessageType ns3::olsr::MessageHeader::GetMessageType() const [member function]
    cls.add_method('GetMessageType', 
                   'ns3::olsr::MessageHeader::MessageType', 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid & ns3::olsr::MessageHeader::GetMid() [member function]
    cls.add_method('GetMid', 
                   'ns3::olsr::MessageHeader::Mid&', 
                   [])
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid const & ns3::olsr::MessageHeader::GetMid() const [member function]
    cls.add_method('GetMid', 
                   retval('ns3::olsr::MessageHeader::Mid&', is_const=True), 
                   [], 
                   is_const=True)
    ## olsr-header.h: ns3::Ipv4Address ns3::olsr::MessageHeader::GetOriginatorAddress() const [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc & ns3::olsr::MessageHeader::GetTc() [member function]
    cls.add_method('GetTc', 
                   'ns3::olsr::MessageHeader::Tc&', 
                   [])
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc const & ns3::olsr::MessageHeader::GetTc() const [member function]
    cls.add_method('GetTc', 
                   retval('ns3::olsr::MessageHeader::Tc&', is_const=True), 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint8_t ns3::olsr::MessageHeader::GetTimeToLive() const [member function]
    cls.add_method('GetTimeToLive', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: static ns3::TypeId ns3::olsr::MessageHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## olsr-header.h: ns3::Time ns3::olsr::MessageHeader::GetVTime() const [member function]
    cls.add_method('GetVTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetHopCount(uint8_t hopCount) [member function]
    cls.add_method('SetHopCount', 
                   'void', 
                   [param('uint8_t', 'hopCount')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetMessageSequenceNumber(uint16_t messageSequenceNumber) [member function]
    cls.add_method('SetMessageSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'messageSequenceNumber')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetMessageType(ns3::olsr::MessageHeader::MessageType messageType) [member function]
    cls.add_method('SetMessageType', 
                   'void', 
                   [param('ns3::olsr::MessageHeader::MessageType', 'messageType')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetOriginatorAddress(ns3::Ipv4Address originatorAddress) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Ipv4Address', 'originatorAddress')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetTimeToLive(uint8_t timeToLive) [member function]
    cls.add_method('SetTimeToLive', 
                   'void', 
                   [param('uint8_t', 'timeToLive')])
    ## olsr-header.h: void ns3::olsr::MessageHeader::SetVTime(ns3::Time time) [member function]
    cls.add_method('SetVTime', 
                   'void', 
                   [param('ns3::Time', 'time')])
    cls.add_output_stream_operator()
    return

def register_Ns3OlsrMessageHeaderMid_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid::Mid(ns3::olsr::MessageHeader::Mid const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Mid&', 'arg0', is_const=True)])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Mid::Deserialize(ns3::Buffer::Iterator start, uint32_t messageSize) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'messageSize')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Mid::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Mid::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Mid::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Mid::interfaceAddresses [variable]
    cls.add_instance_attribute('interfaceAddresses', 'std::vector< ns3::Ipv4Address, std::allocator< ns3::Ipv4Address > >', is_const=False)
    return

def register_Ns3OlsrMessageHeaderHello_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::Hello(ns3::olsr::MessageHeader::Hello const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Hello&', 'arg0', is_const=True)])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Hello::Deserialize(ns3::Buffer::Iterator start, uint32_t messageSize) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'messageSize')])
    ## olsr-header.h: ns3::Time ns3::olsr::MessageHeader::Hello::GetHTime() const [member function]
    cls.add_method('GetHTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Hello::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hello::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hello::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hello::SetHTime(ns3::Time time) [member function]
    cls.add_method('SetHTime', 
                   'void', 
                   [param('ns3::Time', 'time')])
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::hTime [variable]
    cls.add_instance_attribute('hTime', 'uint8_t', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::linkMessages [variable]
    cls.add_instance_attribute('linkMessages', 'std::vector< ns3::olsr::MessageHeader::Hello::LinkMessage, std::allocator< ns3::olsr::MessageHeader::Hello::LinkMessage > >', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::willingness [variable]
    cls.add_instance_attribute('willingness', 'uint8_t', is_const=False)
    return

def register_Ns3OlsrMessageHeaderHelloLinkMessage_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::LinkMessage::linkCode [variable]
    cls.add_instance_attribute('linkCode', 'uint8_t', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hello::LinkMessage::neighborInterfaceAddresses [variable]
    cls.add_instance_attribute('neighborInterfaceAddresses', 'std::vector< ns3::Ipv4Address, std::allocator< ns3::Ipv4Address > >', is_const=False)
    cls.add_constructor([])
    return

def register_Ns3OlsrMessageHeaderTc_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc::Tc(ns3::olsr::MessageHeader::Tc const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Tc&', 'arg0', is_const=True)])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Tc::Deserialize(ns3::Buffer::Iterator start, uint32_t messageSize) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'messageSize')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Tc::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Tc::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Tc::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc::ansn [variable]
    cls.add_instance_attribute('ansn', 'uint16_t', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Tc::neighborAddresses [variable]
    cls.add_instance_attribute('neighborAddresses', 'std::vector< ns3::Ipv4Address, std::allocator< ns3::Ipv4Address > >', is_const=False)
    return

def register_Ns3OlsrMessageHeaderHna_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Hna(ns3::olsr::MessageHeader::Hna const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::MessageHeader::Hna&', 'arg0', is_const=True)])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Hna::Deserialize(ns3::Buffer::Iterator start, uint32_t messageSize) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'messageSize')])
    ## olsr-header.h: uint32_t ns3::olsr::MessageHeader::Hna::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hna::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_const=True)
    ## olsr-header.h: void ns3::olsr::MessageHeader::Hna::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::associations [variable]
    cls.add_instance_attribute('associations', 'std::vector< ns3::olsr::MessageHeader::Hna::Association, std::allocator< ns3::olsr::MessageHeader::Hna::Association > >', is_const=False)
    return

def register_Ns3OlsrMessageHeaderHnaAssociation_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Association::address [variable]
    cls.add_instance_attribute('address', 'ns3::Ipv4Address', is_const=False)
    ## olsr-header.h: ns3::olsr::MessageHeader::Hna::Association::mask [variable]
    cls.add_instance_attribute('mask', 'ns3::Ipv4Mask', is_const=False)
    cls.add_constructor([])
    return

def register_Ns3OlsrAgent_methods(root_module, cls):
    ## olsr-agent.h: static ns3::TypeId ns3::olsr::Agent::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## olsr-agent.h: void ns3::olsr::Agent::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_virtual=True)
    ## olsr-agent.h: void ns3::olsr::Agent::SetMainInterface(uint32_t interface) [member function]
    cls.add_method('SetMainInterface', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## olsr-agent.h: void ns3::olsr::Agent::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    cls.add_constructor([])
    return

def register_Ns3OlsrPacketHeader_methods(root_module, cls):
    ## olsr-header.h: ns3::olsr::PacketHeader::PacketHeader(ns3::olsr::PacketHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::olsr::PacketHeader&', 'arg0', is_const=True)])
    ## olsr-header.h: ns3::olsr::PacketHeader::PacketHeader() [constructor]
    cls.add_constructor([])
    ## olsr-header.h: uint32_t ns3::olsr::PacketHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## olsr-header.h: ns3::TypeId ns3::olsr::PacketHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: uint16_t ns3::olsr::PacketHeader::GetPacketLength() const [member function]
    cls.add_method('GetPacketLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint16_t ns3::olsr::PacketHeader::GetPacketSequenceNumber() const [member function]
    cls.add_method('GetPacketSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## olsr-header.h: uint32_t ns3::olsr::PacketHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: static ns3::TypeId ns3::olsr::PacketHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## olsr-header.h: void ns3::olsr::PacketHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: void ns3::olsr::PacketHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## olsr-header.h: void ns3::olsr::PacketHeader::SetPacketLength(uint16_t length) [member function]
    cls.add_method('SetPacketLength', 
                   'void', 
                   [param('uint16_t', 'length')])
    ## olsr-header.h: void ns3::olsr::PacketHeader::SetPacketSequenceNumber(uint16_t seqnum) [member function]
    cls.add_method('SetPacketSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'seqnum')])
    cls.add_output_stream_operator()
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
    ## olsr-header.h: extern double ns3::olsr::EmfToSeconds(uint8_t emf) [free function]
    module.add_function('EmfToSeconds', 
                        'double', 
                        [param('uint8_t', 'emf')])
    ## olsr-header.h: extern uint8_t ns3::olsr::SecondsToEmf(double seconds) [free function]
    module.add_function('SecondsToEmf', 
                        'uint8_t', 
                        [param('double', 'seconds')])
    return

