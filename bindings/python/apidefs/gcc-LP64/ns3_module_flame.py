from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## flame-installer.h: ns3::FlameStack [class]
    module.add_class('FlameStack', parent=root_module['ns3::MeshStack'])
    
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
    
    ## flame-header.h: ns3::flame::FlameHeader [class]
    module.add_class('FlameHeader', parent=root_module['ns3::Header'])
    ## flame-protocol.h: ns3::flame::FlameProtocol [class]
    module.add_class('FlameProtocol', parent=root_module['ns3::MeshL2RoutingProtocol'])
    ## flame-protocol-mac.h: ns3::flame::FlameProtocolMac [class]
    module.add_class('FlameProtocolMac', parent=root_module['ns3::MeshWifiInterfaceMacPlugin'])
    ## flame-rtable.h: ns3::flame::FlameRtable [class]
    module.add_class('FlameRtable', parent=root_module['ns3::Object'])
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult [struct]
    module.add_class('LookupResult', outer_class=root_module['ns3::flame::FlameRtable'])
    ## flame-protocol.h: ns3::flame::FlameTag [class]
    module.add_class('FlameTag', parent=root_module['ns3::Tag'])

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3FlameStack_methods(root_module, root_module['ns3::FlameStack'])
    register_Ns3FlameFlameHeader_methods(root_module, root_module['ns3::flame::FlameHeader'])
    register_Ns3FlameFlameProtocol_methods(root_module, root_module['ns3::flame::FlameProtocol'])
    register_Ns3FlameFlameProtocolMac_methods(root_module, root_module['ns3::flame::FlameProtocolMac'])
    register_Ns3FlameFlameRtable_methods(root_module, root_module['ns3::flame::FlameRtable'])
    register_Ns3FlameFlameRtableLookupResult_methods(root_module, root_module['ns3::flame::FlameRtable::LookupResult'])
    register_Ns3FlameFlameTag_methods(root_module, root_module['ns3::flame::FlameTag'])
    return

def register_Ns3FlameStack_methods(root_module, cls):
    ## flame-installer.h: ns3::FlameStack::FlameStack(ns3::FlameStack const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlameStack const &', 'arg0')])
    ## flame-installer.h: ns3::FlameStack::FlameStack() [constructor]
    cls.add_constructor([])
    ## flame-installer.h: void ns3::FlameStack::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-installer.h: static ns3::TypeId ns3::FlameStack::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-installer.h: bool ns3::FlameStack::InstallStack(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('InstallStack', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')], 
                   is_virtual=True)
    ## flame-installer.h: void ns3::FlameStack::Report(ns3::Ptr<ns3::MeshPointDevice> const mp, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp'), param('std::ostream &', 'arg1')], 
                   is_virtual=True)
    ## flame-installer.h: void ns3::FlameStack::ResetStats(ns3::Ptr<ns3::MeshPointDevice> const mp) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp')], 
                   is_virtual=True)
    return

def register_Ns3FlameFlameHeader_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## flame-header.h: ns3::flame::FlameHeader::FlameHeader(ns3::flame::FlameHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameHeader const &', 'arg0')])
    ## flame-header.h: ns3::flame::FlameHeader::FlameHeader() [constructor]
    cls.add_constructor([])
    ## flame-header.h: void ns3::flame::FlameHeader::AddCost(uint8_t cost) [member function]
    cls.add_method('AddCost', 
                   'void', 
                   [param('uint8_t', 'cost')])
    ## flame-header.h: uint32_t ns3::flame::FlameHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## flame-header.h: uint8_t ns3::flame::FlameHeader::GetCost() const [member function]
    cls.add_method('GetCost', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## flame-header.h: ns3::TypeId ns3::flame::FlameHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-header.h: ns3::Mac48Address ns3::flame::FlameHeader::GetOrigDst() const [member function]
    cls.add_method('GetOrigDst', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## flame-header.h: ns3::Mac48Address ns3::flame::FlameHeader::GetOrigSrc() const [member function]
    cls.add_method('GetOrigSrc', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## flame-header.h: uint16_t ns3::flame::FlameHeader::GetProtocol() const [member function]
    cls.add_method('GetProtocol', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-header.h: uint16_t ns3::flame::FlameHeader::GetSeqno() const [member function]
    cls.add_method('GetSeqno', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-header.h: uint32_t ns3::flame::FlameHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-header.h: static ns3::TypeId ns3::flame::FlameHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-header.h: void ns3::flame::FlameHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## flame-header.h: void ns3::flame::FlameHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## flame-header.h: void ns3::flame::FlameHeader::SetOrigDst(ns3::Mac48Address dst) [member function]
    cls.add_method('SetOrigDst', 
                   'void', 
                   [param('ns3::Mac48Address', 'dst')])
    ## flame-header.h: void ns3::flame::FlameHeader::SetOrigSrc(ns3::Mac48Address OrigSrc) [member function]
    cls.add_method('SetOrigSrc', 
                   'void', 
                   [param('ns3::Mac48Address', 'OrigSrc')])
    ## flame-header.h: void ns3::flame::FlameHeader::SetProtocol(uint16_t protocol) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint16_t', 'protocol')])
    ## flame-header.h: void ns3::flame::FlameHeader::SetSeqno(uint16_t seqno) [member function]
    cls.add_method('SetSeqno', 
                   'void', 
                   [param('uint16_t', 'seqno')])
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

def register_Ns3FlameFlameProtocolMac_methods(root_module, cls):
    ## flame-protocol-mac.h: ns3::flame::FlameProtocolMac::FlameProtocolMac(ns3::flame::FlameProtocolMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameProtocolMac const &', 'arg0')])
    ## flame-protocol-mac.h: ns3::flame::FlameProtocolMac::FlameProtocolMac(uint32_t arg0, ns3::Ptr<ns3::flame::FlameProtocol> arg1) [constructor]
    cls.add_constructor([param('uint32_t', 'arg0'), param('ns3::Ptr< ns3::flame::FlameProtocol >', 'arg1')])
    ## flame-protocol-mac.h: uint16_t ns3::flame::FlameProtocolMac::GetChannelId() const [member function]
    cls.add_method('GetChannelId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-protocol-mac.h: bool ns3::flame::FlameProtocolMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const & header) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const &', 'header')], 
                   is_virtual=True)
    ## flame-protocol-mac.h: void ns3::flame::FlameProtocolMac::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## flame-protocol-mac.h: void ns3::flame::FlameProtocolMac::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## flame-protocol-mac.h: void ns3::flame::FlameProtocolMac::SetParent(ns3::Ptr<ns3::MeshWifiInterfaceMac> parent) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMac >', 'parent')], 
                   is_virtual=True)
    ## flame-protocol-mac.h: void ns3::flame::FlameProtocolMac::UpdateBeacon(ns3::MeshWifiBeacon & beacon) const [member function]
    cls.add_method('UpdateBeacon', 
                   'void', 
                   [param('ns3::MeshWifiBeacon &', 'beacon')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol-mac.h: bool ns3::flame::FlameProtocolMac::UpdateOutcomingFrame(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader & header, ns3::Mac48Address from, ns3::Mac48Address to) [member function]
    cls.add_method('UpdateOutcomingFrame', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader &', 'header'), param('ns3::Mac48Address', 'from'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    return

def register_Ns3FlameFlameRtable_methods(root_module, cls):
    ## flame-rtable.h: ns3::flame::FlameRtable::FlameRtable() [constructor]
    cls.add_constructor([])
    ## flame-rtable.h: void ns3::flame::FlameRtable::AddPath(ns3::Mac48Address const destination, ns3::Mac48Address const retransmitter, uint32_t const interface, uint8_t const cost, uint16_t const seqnum) [member function]
    cls.add_method('AddPath', 
                   'void', 
                   [param('ns3::Mac48Address const', 'destination'), param('ns3::Mac48Address const', 'retransmitter'), param('uint32_t const', 'interface'), param('uint8_t const', 'cost'), param('uint16_t const', 'seqnum')])
    ## flame-rtable.h: void ns3::flame::FlameRtable::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-rtable.h: static ns3::TypeId ns3::flame::FlameRtable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult ns3::flame::FlameRtable::Lookup(ns3::Mac48Address destination) [member function]
    cls.add_method('Lookup', 
                   'ns3::flame::FlameRtable::LookupResult', 
                   [param('ns3::Mac48Address', 'destination')])
    ## flame-rtable.h: ns3::flame::FlameRtable::INTERFACE_ANY [variable]
    cls.add_static_attribute('INTERFACE_ANY', 'uint32_t const', is_const=True)
    ## flame-rtable.h: ns3::flame::FlameRtable::MAX_COST [variable]
    cls.add_static_attribute('MAX_COST', 'uint32_t const', is_const=True)
    return

def register_Ns3FlameFlameRtableLookupResult_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::LookupResult(ns3::flame::FlameRtable::LookupResult const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameRtable::LookupResult const &', 'arg0')])
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::LookupResult(ns3::Mac48Address r=ns3::Mac48Address::GetBroadcast(), uint32_t i=ns3::flame::FlameRtable::INTERFACE_ANY, uint8_t c=ns3::flame::FlameRtable::MAX_COST, uint16_t s=0) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'r', default_value='ns3::Mac48Address::GetBroadcast()'), param('uint32_t', 'i', default_value='ns3::flame::FlameRtable::INTERFACE_ANY'), param('uint8_t', 'c', default_value='ns3::flame::FlameRtable::MAX_COST'), param('uint16_t', 's', default_value='0')])
    ## flame-rtable.h: bool ns3::flame::FlameRtable::LookupResult::IsValid() const [member function]
    cls.add_method('IsValid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::cost [variable]
    cls.add_instance_attribute('cost', 'uint8_t', is_const=False)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::ifIndex [variable]
    cls.add_instance_attribute('ifIndex', 'uint32_t', is_const=False)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::retransmitter [variable]
    cls.add_instance_attribute('retransmitter', 'ns3::Mac48Address', is_const=False)
    ## flame-rtable.h: ns3::flame::FlameRtable::LookupResult::seqnum [variable]
    cls.add_instance_attribute('seqnum', 'uint16_t', is_const=False)
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

