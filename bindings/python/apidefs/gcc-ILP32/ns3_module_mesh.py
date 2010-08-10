from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## mesh-wifi-beacon.h: ns3::MeshWifiBeacon [class]
    module.add_class('MeshWifiBeacon')
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::MeshWifiInterfaceMacPlugin', 'ns3::empty', 'ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## mesh-information-element-vector.h: ns3::MeshInformationElementVector [class]
    module.add_class('MeshInformationElementVector', parent=root_module['ns3::WifiInformationElementVector'])
    ## mesh-l2-routing-protocol.h: ns3::MeshL2RoutingProtocol [class]
    module.add_class('MeshL2RoutingProtocol', parent=root_module['ns3::Object'])
    ## mesh-wifi-interface-mac.h: ns3::MeshWifiInterfaceMac [class]
    module.add_class('MeshWifiInterfaceMac', parent=root_module['ns3::WifiMac'])
    ## mesh-wifi-interface-mac-plugin.h: ns3::MeshWifiInterfaceMacPlugin [class]
    module.add_class('MeshWifiInterfaceMacPlugin', parent=root_module['ns3::SimpleRefCount< ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >'])
    ## mesh-point-device.h: ns3::MeshPointDevice [class]
    module.add_class('MeshPointDevice', parent=root_module['ns3::NetDevice'])
    
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
    register_Ns3MeshWifiBeacon_methods(root_module, root_module['ns3::MeshWifiBeacon'])
    register_Ns3MeshInformationElementVector_methods(root_module, root_module['ns3::MeshInformationElementVector'])
    register_Ns3MeshL2RoutingProtocol_methods(root_module, root_module['ns3::MeshL2RoutingProtocol'])
    register_Ns3MeshWifiInterfaceMac_methods(root_module, root_module['ns3::MeshWifiInterfaceMac'])
    register_Ns3MeshWifiInterfaceMacPlugin_methods(root_module, root_module['ns3::MeshWifiInterfaceMacPlugin'])
    register_Ns3MeshPointDevice_methods(root_module, root_module['ns3::MeshPointDevice'])
    return

def register_Ns3MeshWifiBeacon_methods(root_module, cls):
    ## mesh-wifi-beacon.h: ns3::MeshWifiBeacon::MeshWifiBeacon(ns3::MeshWifiBeacon const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshWifiBeacon const &', 'arg0')])
    ## mesh-wifi-beacon.h: ns3::MeshWifiBeacon::MeshWifiBeacon(ns3::Ssid ssid, ns3::SupportedRates rates, uint64_t us) [constructor]
    cls.add_constructor([param('ns3::Ssid', 'ssid'), param('ns3::SupportedRates', 'rates'), param('uint64_t', 'us')])
    ## mesh-wifi-beacon.h: void ns3::MeshWifiBeacon::AddInformationElement(ns3::Ptr<ns3::WifiInformationElement> ie) [member function]
    cls.add_method('AddInformationElement', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiInformationElement >', 'ie')])
    ## mesh-wifi-beacon.h: ns3::MgtBeaconHeader ns3::MeshWifiBeacon::BeaconHeader() const [member function]
    cls.add_method('BeaconHeader', 
                   'ns3::MgtBeaconHeader', 
                   [], 
                   is_const=True)
    ## mesh-wifi-beacon.h: ns3::WifiMacHeader ns3::MeshWifiBeacon::CreateHeader(ns3::Mac48Address address, ns3::Mac48Address mpAddress) [member function]
    cls.add_method('CreateHeader', 
                   'ns3::WifiMacHeader', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::Mac48Address', 'mpAddress')])
    ## mesh-wifi-beacon.h: ns3::Ptr<ns3::Packet> ns3::MeshWifiBeacon::CreatePacket() [member function]
    cls.add_method('CreatePacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## mesh-wifi-beacon.h: ns3::Time ns3::MeshWifiBeacon::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    return

def register_Ns3MeshInformationElementVector_methods(root_module, cls):
    ## mesh-information-element-vector.h: ns3::MeshInformationElementVector::MeshInformationElementVector() [constructor]
    cls.add_constructor([])
    ## mesh-information-element-vector.h: ns3::MeshInformationElementVector::MeshInformationElementVector(ns3::MeshInformationElementVector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshInformationElementVector const &', 'arg0')])
    ## mesh-information-element-vector.h: uint32_t ns3::MeshInformationElementVector::DeserializeSingleIe(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DeserializeSingleIe', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    return

def register_Ns3MeshL2RoutingProtocol_methods(root_module, cls):
    ## mesh-l2-routing-protocol.h: ns3::MeshL2RoutingProtocol::MeshL2RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## mesh-l2-routing-protocol.h: ns3::MeshL2RoutingProtocol::MeshL2RoutingProtocol(ns3::MeshL2RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshL2RoutingProtocol const &', 'arg0')])
    ## mesh-l2-routing-protocol.h: ns3::Ptr<ns3::MeshPointDevice> ns3::MeshL2RoutingProtocol::GetMeshPoint() const [member function]
    cls.add_method('GetMeshPoint', 
                   'ns3::Ptr< ns3::MeshPointDevice >', 
                   [], 
                   is_const=True)
    ## mesh-l2-routing-protocol.h: static ns3::TypeId ns3::MeshL2RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-l2-routing-protocol.h: bool ns3::MeshL2RoutingProtocol::RemoveRoutingStuff(uint32_t fromIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet> packet, uint16_t & protocolType) [member function]
    cls.add_method('RemoveRoutingStuff', 
                   'bool', 
                   [param('uint32_t', 'fromIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t &', 'protocolType')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-l2-routing-protocol.h: bool ns3::MeshL2RoutingProtocol::RequestRoute(uint32_t sourceIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet const> packet, uint16_t protocolType, ns3::Callback<void, bool, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'sourceIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocolType'), param('ns3::Callback< void, bool, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-l2-routing-protocol.h: void ns3::MeshL2RoutingProtocol::SetMeshPoint(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('SetMeshPoint', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')])
    return

def register_Ns3MeshWifiInterfaceMac_methods(root_module, cls):
    ## mesh-wifi-interface-mac.h: ns3::MeshWifiInterfaceMac::MeshWifiInterfaceMac(ns3::MeshWifiInterfaceMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshWifiInterfaceMac const &', 'arg0')])
    ## mesh-wifi-interface-mac.h: ns3::MeshWifiInterfaceMac::MeshWifiInterfaceMac() [constructor]
    cls.add_constructor([])
    ## mesh-wifi-interface-mac.h: bool ns3::MeshWifiInterfaceMac::CheckSupportedRates(ns3::SupportedRates rates) const [member function]
    cls.add_method('CheckSupportedRates', 
                   'bool', 
                   [param('ns3::SupportedRates', 'rates')], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::FinishConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('FinishConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Mac48Address ns3::MeshWifiInterfaceMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: ns3::Mac48Address ns3::MeshWifiInterfaceMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: uint16_t ns3::MeshWifiInterfaceMac::GetFrequencyChannel() const [member function]
    cls.add_method('GetFrequencyChannel', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: uint32_t ns3::MeshWifiInterfaceMac::GetLinkMetric(ns3::Mac48Address peerAddress) [member function]
    cls.add_method('GetLinkMetric', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'peerAddress')])
    ## mesh-wifi-interface-mac.h: ns3::Mac48Address ns3::MeshWifiInterfaceMac::GetMeshPointAddress() const [member function]
    cls.add_method('GetMeshPointAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: ns3::WifiPhyStandard ns3::MeshWifiInterfaceMac::GetPhyStandard() const [member function]
    cls.add_method('GetPhyStandard', 
                   'ns3::WifiPhyStandard', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetSlot() const [member function]
    cls.add_method('GetSlot', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Ssid ns3::MeshWifiInterfaceMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::MeshWifiInterfaceMac::GetStationManager() [member function]
    cls.add_method('GetStationManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [])
    ## mesh-wifi-interface-mac.h: ns3::SupportedRates ns3::MeshWifiInterfaceMac::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: ns3::Time ns3::MeshWifiInterfaceMac::GetTbtt() const [member function]
    cls.add_method('GetTbtt', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: static ns3::TypeId ns3::MeshWifiInterfaceMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-wifi-interface-mac.h: ns3::Ptr<ns3::WifiPhy> ns3::MeshWifiInterfaceMac::GetWifiPhy() const [member function]
    cls.add_method('GetWifiPhy', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::InstallPlugin(ns3::Ptr<ns3::MeshWifiInterfaceMacPlugin> plugin) [member function]
    cls.add_method('InstallPlugin', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMacPlugin >', 'plugin')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SendManagementFrame(ns3::Ptr<ns3::Packet> frame, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('SendManagementFrame', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'frame'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetBeaconGeneration(bool enable) [member function]
    cls.add_method('SetBeaconGeneration', 
                   'void', 
                   [param('bool', 'enable')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetBeaconInterval(ns3::Time interval) [member function]
    cls.add_method('SetBeaconInterval', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetLinkMetricCallback(ns3::Callback<unsigned int, ns3::Mac48Address, ns3::Ptr<ns3::MeshWifiInterfaceMac>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetLinkMetricCallback', 
                   'void', 
                   [param('ns3::Callback< unsigned int, ns3::Mac48Address, ns3::Ptr< ns3::MeshWifiInterfaceMac >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetMeshPointAddress(ns3::Mac48Address arg0) [member function]
    cls.add_method('SetMeshPointAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'arg0')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetQueue(ns3::AcIndex ac) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('ns3::AcIndex', 'ac')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetRandomStartDelay(ns3::Time interval) [member function]
    cls.add_method('SetRandomStartDelay', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::ShiftTbtt(ns3::Time shift) [member function]
    cls.add_method('ShiftTbtt', 
                   'void', 
                   [param('ns3::Time', 'shift')])
    ## mesh-wifi-interface-mac.h: bool ns3::MeshWifiInterfaceMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::SwitchFrequencyChannel(uint16_t new_id) [member function]
    cls.add_method('SwitchFrequencyChannel', 
                   'void', 
                   [param('uint16_t', 'new_id')])
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## mesh-wifi-interface-mac.h: void ns3::MeshWifiInterfaceMac::ForwardUp(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address src, ns3::Mac48Address dst) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address', 'src'), param('ns3::Mac48Address', 'dst')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3MeshWifiInterfaceMacPlugin_methods(root_module, cls):
    ## mesh-wifi-interface-mac-plugin.h: ns3::MeshWifiInterfaceMacPlugin::MeshWifiInterfaceMacPlugin() [constructor]
    cls.add_constructor([])
    ## mesh-wifi-interface-mac-plugin.h: ns3::MeshWifiInterfaceMacPlugin::MeshWifiInterfaceMacPlugin(ns3::MeshWifiInterfaceMacPlugin const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshWifiInterfaceMacPlugin const &', 'arg0')])
    ## mesh-wifi-interface-mac-plugin.h: bool ns3::MeshWifiInterfaceMacPlugin::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const & header) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const &', 'header')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h: void ns3::MeshWifiInterfaceMacPlugin::SetParent(ns3::Ptr<ns3::MeshWifiInterfaceMac> parent) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMac >', 'parent')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h: void ns3::MeshWifiInterfaceMacPlugin::UpdateBeacon(ns3::MeshWifiBeacon & beacon) const [member function]
    cls.add_method('UpdateBeacon', 
                   'void', 
                   [param('ns3::MeshWifiBeacon &', 'beacon')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h: bool ns3::MeshWifiInterfaceMacPlugin::UpdateOutcomingFrame(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader & header, ns3::Mac48Address from, ns3::Mac48Address to) [member function]
    cls.add_method('UpdateOutcomingFrame', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader &', 'header'), param('ns3::Mac48Address', 'from'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3MeshPointDevice_methods(root_module, cls):
    ## mesh-point-device.h: ns3::MeshPointDevice::MeshPointDevice(ns3::MeshPointDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshPointDevice const &', 'arg0')])
    ## mesh-point-device.h: ns3::MeshPointDevice::MeshPointDevice() [constructor]
    cls.add_constructor([])
    ## mesh-point-device.h: void ns3::MeshPointDevice::AddInterface(ns3::Ptr<ns3::NetDevice> port) [member function]
    cls.add_method('AddInterface', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'port')])
    ## mesh-point-device.h: void ns3::MeshPointDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## mesh-point-device.h: ns3::Address ns3::MeshPointDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Address ns3::MeshPointDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Ptr<ns3::Channel> ns3::MeshPointDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: uint32_t ns3::MeshPointDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Ptr<ns3::NetDevice> ns3::MeshPointDevice::GetInterface(uint32_t id) const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'id')], 
                   is_const=True)
    ## mesh-point-device.h: std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > ns3::MeshPointDevice::GetInterfaces() const [member function]
    cls.add_method('GetInterfaces', 
                   'std::vector< ns3::Ptr< ns3::NetDevice > >', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h: uint16_t ns3::MeshPointDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Address ns3::MeshPointDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Address ns3::MeshPointDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: uint32_t ns3::MeshPointDevice::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h: ns3::Ptr<ns3::Node> ns3::MeshPointDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: ns3::Ptr<ns3::MeshL2RoutingProtocol> ns3::MeshPointDevice::GetRoutingProtocol() const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::MeshL2RoutingProtocol >', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h: static ns3::TypeId ns3::MeshPointDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::Report(std::ostream & os) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## mesh-point-device.h: bool ns3::MeshPointDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetAddress(ns3::Address a) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'a')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## mesh-point-device.h: bool ns3::MeshPointDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## mesh-point-device.h: void ns3::MeshPointDevice::SetRoutingProtocol(ns3::Ptr<ns3::MeshL2RoutingProtocol> protocol) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshL2RoutingProtocol >', 'protocol')])
    ## mesh-point-device.h: bool ns3::MeshPointDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
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

