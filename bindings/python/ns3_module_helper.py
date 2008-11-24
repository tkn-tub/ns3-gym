from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## application-container.h: ns3::ApplicationContainer [class]
    module.add_class('ApplicationContainer')
    ## bridge-helper.h: ns3::BridgeHelper [class]
    module.add_class('BridgeHelper', allow_subclassing=False)
    ## csma-helper.h: ns3::CsmaHelper [class]
    module.add_class('CsmaHelper', allow_subclassing=False)
    ## emu-helper.h: ns3::EmuHelper [class]
    module.add_class('EmuHelper', allow_subclassing=False)
    ## internet-stack-helper.h: ns3::InternetStackHelper [class]
    module.add_class('InternetStackHelper', allow_subclassing=False)
    ## ipv4-address-helper.h: ns3::Ipv4AddressHelper [class]
    module.add_class('Ipv4AddressHelper', allow_subclassing=False)
    ## ipv4-interface-container.h: ns3::Ipv4InterfaceContainer [class]
    module.add_class('Ipv4InterfaceContainer')
    ## mobility-helper.h: ns3::MobilityHelper [class]
    module.add_class('MobilityHelper', allow_subclassing=False)
    ## net-device-container.h: ns3::NetDeviceContainer [class]
    module.add_class('NetDeviceContainer')
    ## node-container.h: ns3::NodeContainer [class]
    module.add_class('NodeContainer')
    ## ns2-mobility-helper.h: ns3::Ns2MobilityHelper [class]
    module.add_class('Ns2MobilityHelper', allow_subclassing=False)
    ## olsr-helper.h: ns3::OlsrHelper [class]
    module.add_class('OlsrHelper', allow_subclassing=False)
    ## on-off-helper.h: ns3::OnOffHelper [class]
    module.add_class('OnOffHelper', allow_subclassing=False)
    ## packet-sink-helper.h: ns3::PacketSinkHelper [class]
    module.add_class('PacketSinkHelper', allow_subclassing=False)
    ## packet-socket-helper.h: ns3::PacketSocketHelper [class]
    module.add_class('PacketSocketHelper', allow_subclassing=False)
    ## point-to-point-helper.h: ns3::PointToPointHelper [class]
    module.add_class('PointToPointHelper', allow_subclassing=False)
    ## static-multicast-route-helper.h: ns3::StaticMulticastRouteHelper [class]
    module.add_class('StaticMulticastRouteHelper', allow_subclassing=False)
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper [class]
    module.add_class('UdpEchoClientHelper', allow_subclassing=False)
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper [class]
    module.add_class('UdpEchoServerHelper', allow_subclassing=False)
    ## v4ping-helper.h: ns3::V4PingHelper [class]
    module.add_class('V4PingHelper', allow_subclassing=False)
    ## wifi-helper.h: ns3::WifiHelper [class]
    module.add_class('WifiHelper', allow_subclassing=False)
    ## wifi-helper.h: ns3::WifiPhyHelper [class]
    module.add_class('WifiPhyHelper', allow_subclassing=False)
    ## yans-wifi-phy-helper.h: ns3::YansWifiChannelHelper [class]
    module.add_class('YansWifiChannelHelper', allow_subclassing=False)
    ## yans-wifi-phy-helper.h: ns3::YansWifiPhyHelper [class]
    module.add_class('YansWifiPhyHelper', allow_subclassing=False, parent=root_module['ns3::WifiPhyHelper'])
    
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
    register_Ns3ApplicationContainer_methods(root_module, root_module['ns3::ApplicationContainer'])
    register_Ns3BridgeHelper_methods(root_module, root_module['ns3::BridgeHelper'])
    register_Ns3CsmaHelper_methods(root_module, root_module['ns3::CsmaHelper'])
    register_Ns3EmuHelper_methods(root_module, root_module['ns3::EmuHelper'])
    register_Ns3InternetStackHelper_methods(root_module, root_module['ns3::InternetStackHelper'])
    register_Ns3Ipv4AddressHelper_methods(root_module, root_module['ns3::Ipv4AddressHelper'])
    register_Ns3Ipv4InterfaceContainer_methods(root_module, root_module['ns3::Ipv4InterfaceContainer'])
    register_Ns3MobilityHelper_methods(root_module, root_module['ns3::MobilityHelper'])
    register_Ns3NetDeviceContainer_methods(root_module, root_module['ns3::NetDeviceContainer'])
    register_Ns3NodeContainer_methods(root_module, root_module['ns3::NodeContainer'])
    register_Ns3Ns2MobilityHelper_methods(root_module, root_module['ns3::Ns2MobilityHelper'])
    register_Ns3OlsrHelper_methods(root_module, root_module['ns3::OlsrHelper'])
    register_Ns3OnOffHelper_methods(root_module, root_module['ns3::OnOffHelper'])
    register_Ns3PacketSinkHelper_methods(root_module, root_module['ns3::PacketSinkHelper'])
    register_Ns3PacketSocketHelper_methods(root_module, root_module['ns3::PacketSocketHelper'])
    register_Ns3PointToPointHelper_methods(root_module, root_module['ns3::PointToPointHelper'])
    register_Ns3StaticMulticastRouteHelper_methods(root_module, root_module['ns3::StaticMulticastRouteHelper'])
    register_Ns3UdpEchoClientHelper_methods(root_module, root_module['ns3::UdpEchoClientHelper'])
    register_Ns3UdpEchoServerHelper_methods(root_module, root_module['ns3::UdpEchoServerHelper'])
    register_Ns3V4PingHelper_methods(root_module, root_module['ns3::V4PingHelper'])
    register_Ns3WifiHelper_methods(root_module, root_module['ns3::WifiHelper'])
    register_Ns3WifiPhyHelper_methods(root_module, root_module['ns3::WifiPhyHelper'])
    register_Ns3YansWifiChannelHelper_methods(root_module, root_module['ns3::YansWifiChannelHelper'])
    register_Ns3YansWifiPhyHelper_methods(root_module, root_module['ns3::YansWifiPhyHelper'])
    return

def register_Ns3ApplicationContainer_methods(root_module, cls):
    ## application-container.h: ns3::ApplicationContainer::ApplicationContainer(ns3::ApplicationContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ApplicationContainer const &', 'arg0')])
    ## application-container.h: ns3::ApplicationContainer::ApplicationContainer() [constructor]
    cls.add_constructor([])
    ## application-container.h: ns3::ApplicationContainer::ApplicationContainer(ns3::Ptr<ns3::Application> application) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Application >', 'application')])
    ## application-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Application>*,std::vector<ns3::Ptr<ns3::Application>, std::allocator<ns3::Ptr<ns3::Application> > > > ns3::ApplicationContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Application > const, std::vector< ns3::Ptr< ns3::Application > > >', 
                   [], 
                   is_const=True)
    ## application-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Application>*,std::vector<ns3::Ptr<ns3::Application>, std::allocator<ns3::Ptr<ns3::Application> > > > ns3::ApplicationContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Application > const, std::vector< ns3::Ptr< ns3::Application > > >', 
                   [], 
                   is_const=True)
    ## application-container.h: uint32_t ns3::ApplicationContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## application-container.h: ns3::Ptr<ns3::Application> ns3::ApplicationContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Application >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## application-container.h: void ns3::ApplicationContainer::Add(ns3::ApplicationContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::ApplicationContainer', 'other')])
    ## application-container.h: void ns3::ApplicationContainer::Add(ns3::Ptr<ns3::Application> application) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'application')])
    ## application-container.h: void ns3::ApplicationContainer::Start(ns3::Time start) [member function]
    cls.add_method('Start', 
                   'void', 
                   [param('ns3::Time', 'start')])
    ## application-container.h: void ns3::ApplicationContainer::Stop(ns3::Time stop) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time', 'stop')])
    return

def register_Ns3BridgeHelper_methods(root_module, cls):
    ## bridge-helper.h: ns3::BridgeHelper::BridgeHelper(ns3::BridgeHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BridgeHelper const &', 'arg0')])
    ## bridge-helper.h: ns3::BridgeHelper::BridgeHelper() [constructor]
    cls.add_constructor([])
    ## bridge-helper.h: void ns3::BridgeHelper::SetDeviceAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## bridge-helper.h: ns3::NetDeviceContainer ns3::BridgeHelper::Install(ns3::Ptr<ns3::Node> node, ns3::NetDeviceContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::NetDeviceContainer', 'c')])
    return

def register_Ns3CsmaHelper_methods(root_module, cls):
    ## csma-helper.h: ns3::CsmaHelper::CsmaHelper(ns3::CsmaHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsmaHelper const &', 'arg0')])
    ## csma-helper.h: ns3::CsmaHelper::CsmaHelper() [constructor]
    cls.add_constructor([])
    ## csma-helper.h: void ns3::CsmaHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## csma-helper.h: void ns3::CsmaHelper::SetDeviceAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## csma-helper.h: void ns3::CsmaHelper::SetChannelAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetChannelAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## csma-helper.h: void ns3::CsmaHelper::SetDeviceParameter(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceParameter', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')], 
                   deprecated=True)
    ## csma-helper.h: void ns3::CsmaHelper::SetChannelParameter(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetChannelParameter', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')], 
                   deprecated=True)
    ## csma-helper.h: static void ns3::CsmaHelper::EnablePcap(std::string filename, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## csma-helper.h: static void ns3::CsmaHelper::EnablePcap(std::string filename, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## csma-helper.h: static void ns3::CsmaHelper::EnablePcap(std::string filename, ns3::NodeContainer n) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## csma-helper.h: static void ns3::CsmaHelper::EnablePcapAll(std::string filename) [member function]
    cls.add_method('EnablePcapAll', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_static=True)
    ## csma-helper.h: static void ns3::CsmaHelper::EnableAscii(std::ostream & os, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## csma-helper.h: static void ns3::CsmaHelper::EnableAscii(std::ostream & os, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## csma-helper.h: static void ns3::CsmaHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## csma-helper.h: static void ns3::CsmaHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## csma-helper.h: ns3::NetDeviceContainer ns3::CsmaHelper::Install(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::CsmaChannel> channel) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::CsmaChannel >', 'channel')], 
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
    ## csma-helper.h: void ns3::CsmaHelper::InstallStar(ns3::Ptr<ns3::Node> hub, ns3::NodeContainer spokes, ns3::NetDeviceContainer & hubDevices, ns3::NetDeviceContainer & spokeDevices) [member function]
    cls.add_method('InstallStar', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'hub'), param('ns3::NodeContainer', 'spokes'), param('ns3::NetDeviceContainer &', 'hubDevices'), param('ns3::NetDeviceContainer &', 'spokeDevices')])
    return

def register_Ns3EmuHelper_methods(root_module, cls):
    ## emu-helper.h: ns3::EmuHelper::EmuHelper(ns3::EmuHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EmuHelper const &', 'arg0')])
    ## emu-helper.h: ns3::EmuHelper::EmuHelper() [constructor]
    cls.add_constructor([])
    ## emu-helper.h: void ns3::EmuHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## emu-helper.h: void ns3::EmuHelper::SetAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## emu-helper.h: static void ns3::EmuHelper::EnablePcap(std::string filename, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## emu-helper.h: static void ns3::EmuHelper::EnablePcap(std::string filename, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## emu-helper.h: static void ns3::EmuHelper::EnablePcap(std::string filename, ns3::NodeContainer n) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## emu-helper.h: static void ns3::EmuHelper::EnablePcapAll(std::string filename) [member function]
    cls.add_method('EnablePcapAll', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_static=True)
    ## emu-helper.h: static void ns3::EmuHelper::EnableAscii(std::ostream & os, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## emu-helper.h: static void ns3::EmuHelper::EnableAscii(std::ostream & os, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## emu-helper.h: static void ns3::EmuHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## emu-helper.h: static void ns3::EmuHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
    ## emu-helper.h: ns3::NetDeviceContainer ns3::EmuHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## emu-helper.h: ns3::NetDeviceContainer ns3::EmuHelper::Install(ns3::NodeContainer const & c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer const &', 'c')], 
                   is_const=True)
    return

def register_Ns3InternetStackHelper_methods(root_module, cls):
    ## internet-stack-helper.h: ns3::InternetStackHelper::InternetStackHelper(ns3::InternetStackHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::InternetStackHelper const &', 'arg0')])
    ## internet-stack-helper.h: ns3::InternetStackHelper::InternetStackHelper() [constructor]
    cls.add_constructor([])
    ## internet-stack-helper.h: void ns3::InternetStackHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## internet-stack-helper.h: void ns3::InternetStackHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## internet-stack-helper.h: void ns3::InternetStackHelper::SetNscStack(std::string soname) [member function]
    cls.add_method('SetNscStack', 
                   'void', 
                   [param('std::string', 'soname')])
    ## internet-stack-helper.h: static void ns3::InternetStackHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## internet-stack-helper.h: static void ns3::InternetStackHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
    ## internet-stack-helper.h: static void ns3::InternetStackHelper::EnablePcapAll(std::string filename) [member function]
    cls.add_method('EnablePcapAll', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_static=True)
    return

def register_Ns3Ipv4AddressHelper_methods(root_module, cls):
    ## ipv4-address-helper.h: ns3::Ipv4AddressHelper::Ipv4AddressHelper(ns3::Ipv4AddressHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressHelper const &', 'arg0')])
    ## ipv4-address-helper.h: ns3::Ipv4AddressHelper::Ipv4AddressHelper() [constructor]
    cls.add_constructor([])
    ## ipv4-address-helper.h: void ns3::Ipv4AddressHelper::SetBase(ns3::Ipv4Address network, ns3::Ipv4Mask mask, ns3::Ipv4Address base="0.0.0.1") [member function]
    cls.add_method('SetBase', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'mask'), param('ns3::Ipv4Address', 'base', default_value='"0.0.0.1"')])
    ## ipv4-address-helper.h: ns3::Ipv4Address ns3::Ipv4AddressHelper::NewNetwork() [member function]
    cls.add_method('NewNetwork', 
                   'ns3::Ipv4Address', 
                   [])
    ## ipv4-address-helper.h: ns3::Ipv4Address ns3::Ipv4AddressHelper::NewAddress() [member function]
    cls.add_method('NewAddress', 
                   'ns3::Ipv4Address', 
                   [])
    ## ipv4-address-helper.h: ns3::Ipv4InterfaceContainer ns3::Ipv4AddressHelper::Assign(ns3::NetDeviceContainer const & c) [member function]
    cls.add_method('Assign', 
                   'ns3::Ipv4InterfaceContainer', 
                   [param('ns3::NetDeviceContainer const &', 'c')])
    return

def register_Ns3Ipv4InterfaceContainer_methods(root_module, cls):
    ## ipv4-interface-container.h: ns3::Ipv4InterfaceContainer::Ipv4InterfaceContainer(ns3::Ipv4InterfaceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4InterfaceContainer const &', 'arg0')])
    ## ipv4-interface-container.h: ns3::Ipv4InterfaceContainer::Ipv4InterfaceContainer() [constructor]
    cls.add_constructor([])
    ## ipv4-interface-container.h: void ns3::Ipv4InterfaceContainer::Add(ns3::Ipv4InterfaceContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ipv4InterfaceContainer', 'other')])
    ## ipv4-interface-container.h: uint32_t ns3::Ipv4InterfaceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-interface-container.h: ns3::Ipv4Address ns3::Ipv4InterfaceContainer::GetAddress(uint32_t i) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-interface-container.h: void ns3::Ipv4InterfaceContainer::SetMetric(uint32_t i, uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'i'), param('uint16_t', 'metric')])
    ## ipv4-interface-container.h: void ns3::Ipv4InterfaceContainer::Add(ns3::Ptr<ns3::Ipv4> ipv4, uint32_t interface) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4'), param('uint32_t', 'interface')])
    return

def register_Ns3MobilityHelper_methods(root_module, cls):
    ## mobility-helper.h: ns3::MobilityHelper::MobilityHelper(ns3::MobilityHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MobilityHelper const &', 'arg0')])
    ## mobility-helper.h: ns3::MobilityHelper::MobilityHelper() [constructor]
    cls.add_constructor([])
    ## mobility-helper.h: void ns3::MobilityHelper::SetPositionAllocator(ns3::Ptr<ns3::PositionAllocator> allocator) [member function]
    cls.add_method('SetPositionAllocator', 
                   'void', 
                   [param('ns3::Ptr< ns3::PositionAllocator >', 'allocator')])
    ## mobility-helper.h: void ns3::MobilityHelper::SetPositionAllocator(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue(), std::string n8="", ns3::AttributeValue const & v8=ns3::EmptyAttributeValue(), std::string n9="", ns3::AttributeValue const & v9=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPositionAllocator', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n8', default_value='""'), param('ns3::AttributeValue const &', 'v8', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n9', default_value='""'), param('ns3::AttributeValue const &', 'v9', default_value='ns3::EmptyAttributeValue()')])
    ## mobility-helper.h: void ns3::MobilityHelper::SetMobilityModel(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue(), std::string n8="", ns3::AttributeValue const & v8=ns3::EmptyAttributeValue(), std::string n9="", ns3::AttributeValue const & v9=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMobilityModel', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n8', default_value='""'), param('ns3::AttributeValue const &', 'v8', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n9', default_value='""'), param('ns3::AttributeValue const &', 'v9', default_value='ns3::EmptyAttributeValue()')])
    ## mobility-helper.h: void ns3::MobilityHelper::PushReferenceMobilityModel(ns3::Ptr<ns3::Object> reference) [member function]
    cls.add_method('PushReferenceMobilityModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'reference')])
    ## mobility-helper.h: void ns3::MobilityHelper::PopReferenceMobilityModel() [member function]
    cls.add_method('PopReferenceMobilityModel', 
                   'void', 
                   [])
    ## mobility-helper.h: std::string ns3::MobilityHelper::GetMobilityModelType() const [member function]
    cls.add_method('GetMobilityModelType', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## mobility-helper.h: void ns3::MobilityHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## mobility-helper.h: void ns3::MobilityHelper::Install(ns3::NodeContainer container) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer', 'container')], 
                   is_const=True)
    ## mobility-helper.h: void ns3::MobilityHelper::InstallAll() [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [])
    ## mobility-helper.h: static void ns3::MobilityHelper::EnableAscii(std::ostream & os, uint32_t nodeid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('uint32_t', 'nodeid')], 
                   is_static=True)
    ## mobility-helper.h: static void ns3::MobilityHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## mobility-helper.h: static void ns3::MobilityHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
    return

def register_Ns3NetDeviceContainer_methods(root_module, cls):
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer(ns3::NetDeviceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDeviceContainer const &', 'arg0')])
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer() [constructor]
    cls.add_constructor([])
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer(ns3::Ptr<ns3::NetDevice> dev) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'dev')])
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer(ns3::NetDeviceContainer const & a, ns3::NetDeviceContainer const & b) [constructor]
    cls.add_constructor([param('ns3::NetDeviceContainer const &', 'a'), param('ns3::NetDeviceContainer const &', 'b')])
    ## net-device-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::NetDevice>*,std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > > ns3::NetDeviceContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::NetDevice > const, std::vector< ns3::Ptr< ns3::NetDevice > > >', 
                   [], 
                   is_const=True)
    ## net-device-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::NetDevice>*,std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > > ns3::NetDeviceContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::NetDevice > const, std::vector< ns3::Ptr< ns3::NetDevice > > >', 
                   [], 
                   is_const=True)
    ## net-device-container.h: uint32_t ns3::NetDeviceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## net-device-container.h: ns3::Ptr<ns3::NetDevice> ns3::NetDeviceContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## net-device-container.h: void ns3::NetDeviceContainer::Add(ns3::NetDeviceContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::NetDeviceContainer', 'other')])
    ## net-device-container.h: void ns3::NetDeviceContainer::Add(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    return

def register_Ns3NodeContainer_methods(root_module, cls):
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'arg0')])
    ## node-container.h: ns3::NodeContainer::NodeContainer() [constructor]
    cls.add_constructor([])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node')])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b')])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c')])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c, ns3::NodeContainer const & d) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c'), param('ns3::NodeContainer const &', 'd')])
    ## node-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h: uint32_t ns3::NodeContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node-container.h: ns3::Ptr<ns3::Node> ns3::NodeContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## node-container.h: void ns3::NodeContainer::Create(uint32_t n) [member function]
    cls.add_method('Create', 
                   'void', 
                   [param('uint32_t', 'n')])
    ## node-container.h: void ns3::NodeContainer::Add(ns3::NodeContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::NodeContainer', 'other')])
    ## node-container.h: void ns3::NodeContainer::Add(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## node-container.h: static ns3::NodeContainer ns3::NodeContainer::GetGlobal() [member function]
    cls.add_method('GetGlobal', 
                   'ns3::NodeContainer', 
                   [], 
                   is_static=True)
    return

def register_Ns3Ns2MobilityHelper_methods(root_module, cls):
    ## ns2-mobility-helper.h: ns3::Ns2MobilityHelper::Ns2MobilityHelper(ns3::Ns2MobilityHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ns2MobilityHelper const &', 'arg0')])
    ## ns2-mobility-helper.h: ns3::Ns2MobilityHelper::Ns2MobilityHelper(std::string filename) [constructor]
    cls.add_constructor([param('std::string', 'filename')])
    ## ns2-mobility-helper.h: void ns3::Ns2MobilityHelper::Install() const [member function]
    cls.add_method('Install', 
                   'void', 
                   [], 
                   is_const=True)
    return

def register_Ns3OlsrHelper_methods(root_module, cls):
    ## olsr-helper.h: ns3::OlsrHelper::OlsrHelper(ns3::OlsrHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OlsrHelper const &', 'arg0')])
    ## olsr-helper.h: ns3::OlsrHelper::OlsrHelper() [constructor]
    cls.add_constructor([])
    ## olsr-helper.h: void ns3::OlsrHelper::SetAgent(std::string tid, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetAgent', 
                   'void', 
                   [param('std::string', 'tid'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## olsr-helper.h: void ns3::OlsrHelper::Install(ns3::NodeContainer container) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer', 'container')])
    ## olsr-helper.h: void ns3::OlsrHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## olsr-helper.h: void ns3::OlsrHelper::InstallAll() [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [])
    return

def register_Ns3OnOffHelper_methods(root_module, cls):
    ## on-off-helper.h: ns3::OnOffHelper::OnOffHelper(ns3::OnOffHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OnOffHelper const &', 'arg0')])
    ## on-off-helper.h: ns3::OnOffHelper::OnOffHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
    ## on-off-helper.h: void ns3::OnOffHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## on-off-helper.h: ns3::ApplicationContainer ns3::OnOffHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## on-off-helper.h: ns3::ApplicationContainer ns3::OnOffHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    return

def register_Ns3PacketSinkHelper_methods(root_module, cls):
    ## packet-sink-helper.h: ns3::PacketSinkHelper::PacketSinkHelper(ns3::PacketSinkHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSinkHelper const &', 'arg0')])
    ## packet-sink-helper.h: ns3::PacketSinkHelper::PacketSinkHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
    ## packet-sink-helper.h: void ns3::PacketSinkHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## packet-sink-helper.h: ns3::ApplicationContainer ns3::PacketSinkHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## packet-sink-helper.h: ns3::ApplicationContainer ns3::PacketSinkHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    return

def register_Ns3PacketSocketHelper_methods(root_module, cls):
    ## packet-socket-helper.h: ns3::PacketSocketHelper::PacketSocketHelper(ns3::PacketSocketHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSocketHelper const &', 'arg0')])
    ## packet-socket-helper.h: ns3::PacketSocketHelper::PacketSocketHelper() [constructor]
    cls.add_constructor([])
    ## packet-socket-helper.h: void ns3::PacketSocketHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## packet-socket-helper.h: void ns3::PacketSocketHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    return

def register_Ns3PointToPointHelper_methods(root_module, cls):
    ## point-to-point-helper.h: ns3::PointToPointHelper::PointToPointHelper(ns3::PointToPointHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointHelper const &', 'arg0')])
    ## point-to-point-helper.h: ns3::PointToPointHelper::PointToPointHelper() [constructor]
    cls.add_constructor([])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetDeviceAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetChannelAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetChannelAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetDeviceParameter(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetDeviceParameter', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')], 
                   deprecated=True)
    ## point-to-point-helper.h: void ns3::PointToPointHelper::SetChannelParameter(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetChannelParameter', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')], 
                   deprecated=True)
    ## point-to-point-helper.h: static void ns3::PointToPointHelper::EnablePcap(std::string filename, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## point-to-point-helper.h: static void ns3::PointToPointHelper::EnablePcap(std::string filename, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## point-to-point-helper.h: static void ns3::PointToPointHelper::EnablePcap(std::string filename, ns3::NodeContainer n) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## point-to-point-helper.h: static void ns3::PointToPointHelper::EnablePcapAll(std::string filename) [member function]
    cls.add_method('EnablePcapAll', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_static=True)
    ## point-to-point-helper.h: static void ns3::PointToPointHelper::EnableAscii(std::ostream & os, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## point-to-point-helper.h: static void ns3::PointToPointHelper::EnableAscii(std::ostream & os, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## point-to-point-helper.h: static void ns3::PointToPointHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## point-to-point-helper.h: static void ns3::PointToPointHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## point-to-point-helper.h: ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::Ptr<ns3::Node> a, ns3::Ptr<ns3::Node> b) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'a'), param('ns3::Ptr< ns3::Node >', 'b')])
    ## point-to-point-helper.h: void ns3::PointToPointHelper::InstallStar(ns3::Ptr<ns3::Node> hub, ns3::NodeContainer spokes, ns3::NetDeviceContainer & hubDevices, ns3::NetDeviceContainer & spokeDevices) [member function]
    cls.add_method('InstallStar', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'hub'), param('ns3::NodeContainer', 'spokes'), param('ns3::NetDeviceContainer &', 'hubDevices'), param('ns3::NetDeviceContainer &', 'spokeDevices')])
    return

def register_Ns3StaticMulticastRouteHelper_methods(root_module, cls):
    ## static-multicast-route-helper.h: ns3::StaticMulticastRouteHelper::StaticMulticastRouteHelper(ns3::StaticMulticastRouteHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StaticMulticastRouteHelper const &', 'arg0')])
    ## static-multicast-route-helper.h: ns3::StaticMulticastRouteHelper::StaticMulticastRouteHelper() [constructor]
    cls.add_constructor([])
    ## static-multicast-route-helper.h: void ns3::StaticMulticastRouteHelper::AddMulticastRoute(ns3::Ptr<ns3::Node> arg0, ns3::Ipv4Address source, ns3::Ipv4Address group, ns3::Ptr<ns3::NetDevice> input, ns3::NetDeviceContainer output) [member function]
    cls.add_method('AddMulticastRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'arg0'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'group'), param('ns3::Ptr< ns3::NetDevice >', 'input'), param('ns3::NetDeviceContainer', 'output')])
    ## static-multicast-route-helper.h: void ns3::StaticMulticastRouteHelper::SetDefaultMulticastRoute(ns3::Ptr<ns3::Node> n, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('SetDefaultMulticastRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'n'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## static-multicast-route-helper.h: void ns3::StaticMulticastRouteHelper::JoinMulticastGroup(ns3::Ptr<ns3::Node> n, ns3::Ipv4Address source, ns3::Ipv4Address group) [member function]
    cls.add_method('JoinMulticastGroup', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'n'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'group')])
    return

def register_Ns3UdpEchoClientHelper_methods(root_module, cls):
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper::UdpEchoClientHelper(ns3::UdpEchoClientHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoClientHelper const &', 'arg0')])
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper::UdpEchoClientHelper(ns3::Ipv4Address ip, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoClientHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoClientHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    return

def register_Ns3UdpEchoServerHelper_methods(root_module, cls):
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper::UdpEchoServerHelper(ns3::UdpEchoServerHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoServerHelper const &', 'arg0')])
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper::UdpEchoServerHelper(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## udp-echo-helper.h: void ns3::UdpEchoServerHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoServerHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoServerHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    return

def register_Ns3V4PingHelper_methods(root_module, cls):
    ## v4ping-helper.h: ns3::V4PingHelper::V4PingHelper(ns3::V4PingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::V4PingHelper const &', 'arg0')])
    ## v4ping-helper.h: ns3::V4PingHelper::V4PingHelper(ns3::Ipv4Address remote) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'remote')])
    ## v4ping-helper.h: void ns3::V4PingHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## v4ping-helper.h: ns3::ApplicationContainer ns3::V4PingHelper::Install(ns3::NodeContainer nodes) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'nodes')], 
                   is_const=True)
    ## v4ping-helper.h: ns3::ApplicationContainer ns3::V4PingHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    return

def register_Ns3WifiHelper_methods(root_module, cls):
    ## wifi-helper.h: ns3::WifiHelper::WifiHelper(ns3::WifiHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiHelper const &', 'arg0')])
    ## wifi-helper.h: ns3::WifiHelper::WifiHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h: static ns3::WifiHelper ns3::WifiHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::WifiHelper', 
                   [], 
                   is_static=True)
    ## wifi-helper.h: void ns3::WifiHelper::SetRemoteStationManager(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetRemoteStationManager', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## wifi-helper.h: void ns3::WifiHelper::SetMac(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## wifi-helper.h: ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    return

def register_Ns3WifiPhyHelper_methods(root_module, cls):
    ## wifi-helper.h: ns3::WifiPhyHelper::WifiPhyHelper(ns3::WifiPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhyHelper const &', 'arg0')])
    ## wifi-helper.h: ns3::WifiPhyHelper::WifiPhyHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h: ns3::Ptr<ns3::WifiPhy> ns3::WifiPhyHelper::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WifiNetDevice> device) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::WifiNetDevice >', 'device')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3YansWifiChannelHelper_methods(root_module, cls):
    ## yans-wifi-phy-helper.h: ns3::YansWifiChannelHelper::YansWifiChannelHelper(ns3::YansWifiChannelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansWifiChannelHelper const &', 'arg0')])
    ## yans-wifi-phy-helper.h: ns3::YansWifiChannelHelper::YansWifiChannelHelper() [constructor]
    cls.add_constructor([])
    ## yans-wifi-phy-helper.h: static ns3::YansWifiChannelHelper ns3::YansWifiChannelHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::YansWifiChannelHelper', 
                   [], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: void ns3::YansWifiChannelHelper::AddPropagationLoss(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('AddPropagationLoss', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## yans-wifi-phy-helper.h: void ns3::YansWifiChannelHelper::SetPropagationDelay(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPropagationDelay', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## yans-wifi-phy-helper.h: ns3::Ptr<ns3::YansWifiChannel> ns3::YansWifiChannelHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::YansWifiChannel >', 
                   [], 
                   is_const=True)
    return

def register_Ns3YansWifiPhyHelper_methods(root_module, cls):
    ## yans-wifi-phy-helper.h: ns3::YansWifiPhyHelper::YansWifiPhyHelper(ns3::YansWifiPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansWifiPhyHelper const &', 'arg0')])
    ## yans-wifi-phy-helper.h: ns3::YansWifiPhyHelper::YansWifiPhyHelper() [constructor]
    cls.add_constructor([])
    ## yans-wifi-phy-helper.h: static ns3::YansWifiPhyHelper ns3::YansWifiPhyHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::YansWifiPhyHelper', 
                   [], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: void ns3::YansWifiPhyHelper::SetChannel(ns3::Ptr<ns3::YansWifiChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::YansWifiChannel >', 'channel')])
    ## yans-wifi-phy-helper.h: void ns3::YansWifiPhyHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')])
    ## yans-wifi-phy-helper.h: void ns3::YansWifiPhyHelper::SetErrorRateModel(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetErrorRateModel', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## yans-wifi-phy-helper.h: ns3::Ptr<ns3::WifiPhy> ns3::YansWifiPhyHelper::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WifiNetDevice> device) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::WifiNetDevice >', 'device')], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy-helper.h: static void ns3::YansWifiPhyHelper::EnablePcap(std::string filename, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: static void ns3::YansWifiPhyHelper::EnablePcap(std::string filename, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: static void ns3::YansWifiPhyHelper::EnablePcap(std::string filename, ns3::NodeContainer n) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: static void ns3::YansWifiPhyHelper::EnablePcapAll(std::string filename) [member function]
    cls.add_method('EnablePcapAll', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: static void ns3::YansWifiPhyHelper::EnableAscii(std::ostream & os, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: static void ns3::YansWifiPhyHelper::EnableAscii(std::ostream & os, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NetDeviceContainer', 'd')], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: static void ns3::YansWifiPhyHelper::EnableAscii(std::ostream & os, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::ostream &', 'os'), param('ns3::NodeContainer', 'n')], 
                   is_static=True)
    ## yans-wifi-phy-helper.h: static void ns3::YansWifiPhyHelper::EnableAsciiAll(std::ostream & os) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_static=True)
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

