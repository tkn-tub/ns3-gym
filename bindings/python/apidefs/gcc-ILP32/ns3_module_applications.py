from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## bulk-send-helper.h: ns3::BulkSendHelper [class]
    module.add_class('BulkSendHelper')
    ## on-off-helper.h: ns3::OnOffHelper [class]
    module.add_class('OnOffHelper')
    ## packet-loss-counter.h: ns3::PacketLossCounter [class]
    module.add_class('PacketLossCounter')
    ## packet-sink-helper.h: ns3::PacketSinkHelper [class]
    module.add_class('PacketSinkHelper')
    ## ping6-helper.h: ns3::Ping6Helper [class]
    module.add_class('Ping6Helper')
    ## udp-client-server-helper.h: ns3::UdpClientHelper [class]
    module.add_class('UdpClientHelper')
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper [class]
    module.add_class('UdpEchoClientHelper')
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper [class]
    module.add_class('UdpEchoServerHelper')
    ## udp-client-server-helper.h: ns3::UdpServerHelper [class]
    module.add_class('UdpServerHelper')
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper [class]
    module.add_class('UdpTraceClientHelper')
    ## v4ping-helper.h: ns3::V4PingHelper [class]
    module.add_class('V4PingHelper')
    ## seq-ts-header.h: ns3::SeqTsHeader [class]
    module.add_class('SeqTsHeader', parent=root_module['ns3::Header'])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::RadvdInterface', 'ns3::empty', 'ns3::DefaultDeleter<ns3::RadvdInterface>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::RadvdPrefix', 'ns3::empty', 'ns3::DefaultDeleter<ns3::RadvdPrefix>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## bulk-send-application.h: ns3::BulkSendApplication [class]
    module.add_class('BulkSendApplication', parent=root_module['ns3::Application'])
    ## onoff-application.h: ns3::OnOffApplication [class]
    module.add_class('OnOffApplication', parent=root_module['ns3::Application'])
    ## packet-sink.h: ns3::PacketSink [class]
    module.add_class('PacketSink', parent=root_module['ns3::Application'])
    ## ping6.h: ns3::Ping6 [class]
    module.add_class('Ping6', parent=root_module['ns3::Application'])
    ## radvd.h: ns3::Radvd [class]
    module.add_class('Radvd', parent=root_module['ns3::Application'])
    ## radvd-interface.h: ns3::RadvdInterface [class]
    module.add_class('RadvdInterface', parent=root_module['ns3::SimpleRefCount< ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> >'])
    ## radvd-prefix.h: ns3::RadvdPrefix [class]
    module.add_class('RadvdPrefix', parent=root_module['ns3::SimpleRefCount< ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> >'])
    ## udp-client.h: ns3::UdpClient [class]
    module.add_class('UdpClient', parent=root_module['ns3::Application'])
    ## udp-echo-client.h: ns3::UdpEchoClient [class]
    module.add_class('UdpEchoClient', parent=root_module['ns3::Application'])
    ## udp-echo-server.h: ns3::UdpEchoServer [class]
    module.add_class('UdpEchoServer', parent=root_module['ns3::Application'])
    ## udp-server.h: ns3::UdpServer [class]
    module.add_class('UdpServer', parent=root_module['ns3::Application'])
    ## udp-trace-client.h: ns3::UdpTraceClient [class]
    module.add_class('UdpTraceClient', parent=root_module['ns3::Application'])
    ## v4ping.h: ns3::V4Ping [class]
    module.add_class('V4Ping', parent=root_module['ns3::Application'])
    
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
    register_Ns3BulkSendHelper_methods(root_module, root_module['ns3::BulkSendHelper'])
    register_Ns3OnOffHelper_methods(root_module, root_module['ns3::OnOffHelper'])
    register_Ns3PacketLossCounter_methods(root_module, root_module['ns3::PacketLossCounter'])
    register_Ns3PacketSinkHelper_methods(root_module, root_module['ns3::PacketSinkHelper'])
    register_Ns3Ping6Helper_methods(root_module, root_module['ns3::Ping6Helper'])
    register_Ns3UdpClientHelper_methods(root_module, root_module['ns3::UdpClientHelper'])
    register_Ns3UdpEchoClientHelper_methods(root_module, root_module['ns3::UdpEchoClientHelper'])
    register_Ns3UdpEchoServerHelper_methods(root_module, root_module['ns3::UdpEchoServerHelper'])
    register_Ns3UdpServerHelper_methods(root_module, root_module['ns3::UdpServerHelper'])
    register_Ns3UdpTraceClientHelper_methods(root_module, root_module['ns3::UdpTraceClientHelper'])
    register_Ns3V4PingHelper_methods(root_module, root_module['ns3::V4PingHelper'])
    register_Ns3SeqTsHeader_methods(root_module, root_module['ns3::SeqTsHeader'])
    register_Ns3BulkSendApplication_methods(root_module, root_module['ns3::BulkSendApplication'])
    register_Ns3OnOffApplication_methods(root_module, root_module['ns3::OnOffApplication'])
    register_Ns3PacketSink_methods(root_module, root_module['ns3::PacketSink'])
    register_Ns3Ping6_methods(root_module, root_module['ns3::Ping6'])
    register_Ns3Radvd_methods(root_module, root_module['ns3::Radvd'])
    register_Ns3RadvdInterface_methods(root_module, root_module['ns3::RadvdInterface'])
    register_Ns3RadvdPrefix_methods(root_module, root_module['ns3::RadvdPrefix'])
    register_Ns3UdpClient_methods(root_module, root_module['ns3::UdpClient'])
    register_Ns3UdpEchoClient_methods(root_module, root_module['ns3::UdpEchoClient'])
    register_Ns3UdpEchoServer_methods(root_module, root_module['ns3::UdpEchoServer'])
    register_Ns3UdpServer_methods(root_module, root_module['ns3::UdpServer'])
    register_Ns3UdpTraceClient_methods(root_module, root_module['ns3::UdpTraceClient'])
    register_Ns3V4Ping_methods(root_module, root_module['ns3::V4Ping'])
    return

def register_Ns3BulkSendHelper_methods(root_module, cls):
    ## bulk-send-helper.h: ns3::BulkSendHelper::BulkSendHelper(ns3::BulkSendHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BulkSendHelper const &', 'arg0')])
    ## bulk-send-helper.h: ns3::BulkSendHelper::BulkSendHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
    ## bulk-send-helper.h: ns3::ApplicationContainer ns3::BulkSendHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## bulk-send-helper.h: ns3::ApplicationContainer ns3::BulkSendHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## bulk-send-helper.h: ns3::ApplicationContainer ns3::BulkSendHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## bulk-send-helper.h: void ns3::BulkSendHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3OnOffHelper_methods(root_module, cls):
    ## on-off-helper.h: ns3::OnOffHelper::OnOffHelper(ns3::OnOffHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OnOffHelper const &', 'arg0')])
    ## on-off-helper.h: ns3::OnOffHelper::OnOffHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
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
    ## on-off-helper.h: ns3::ApplicationContainer ns3::OnOffHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## on-off-helper.h: void ns3::OnOffHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3PacketLossCounter_methods(root_module, cls):
    ## packet-loss-counter.h: ns3::PacketLossCounter::PacketLossCounter(ns3::PacketLossCounter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketLossCounter const &', 'arg0')])
    ## packet-loss-counter.h: ns3::PacketLossCounter::PacketLossCounter(uint8_t bitmapSize) [constructor]
    cls.add_constructor([param('uint8_t', 'bitmapSize')])
    ## packet-loss-counter.h: uint16_t ns3::PacketLossCounter::GetBitMapSize() const [member function]
    cls.add_method('GetBitMapSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## packet-loss-counter.h: uint32_t ns3::PacketLossCounter::GetLost() const [member function]
    cls.add_method('GetLost', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-loss-counter.h: void ns3::PacketLossCounter::NotifyReceived(uint32_t seq) [member function]
    cls.add_method('NotifyReceived', 
                   'void', 
                   [param('uint32_t', 'seq')])
    ## packet-loss-counter.h: void ns3::PacketLossCounter::SetBitMapSize(uint16_t size) [member function]
    cls.add_method('SetBitMapSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    return

def register_Ns3PacketSinkHelper_methods(root_module, cls):
    ## packet-sink-helper.h: ns3::PacketSinkHelper::PacketSinkHelper(ns3::PacketSinkHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSinkHelper const &', 'arg0')])
    ## packet-sink-helper.h: ns3::PacketSinkHelper::PacketSinkHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
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
    ## packet-sink-helper.h: ns3::ApplicationContainer ns3::PacketSinkHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## packet-sink-helper.h: void ns3::PacketSinkHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3Ping6Helper_methods(root_module, cls):
    ## ping6-helper.h: ns3::Ping6Helper::Ping6Helper(ns3::Ping6Helper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ping6Helper const &', 'arg0')])
    ## ping6-helper.h: ns3::Ping6Helper::Ping6Helper() [constructor]
    cls.add_constructor([])
    ## ping6-helper.h: ns3::ApplicationContainer ns3::Ping6Helper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetIfIndex(uint32_t ifIndex) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t', 'ifIndex')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetLocal(ns3::Ipv6Address ip) [member function]
    cls.add_method('SetLocal', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ip')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetRemote(ns3::Ipv6Address ip) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ip')])
    ## ping6-helper.h: void ns3::Ping6Helper::SetRoutersAddress(std::vector<ns3::Ipv6Address, std::allocator<ns3::Ipv6Address> > routers) [member function]
    cls.add_method('SetRoutersAddress', 
                   'void', 
                   [param('std::vector< ns3::Ipv6Address >', 'routers')])
    return

def register_Ns3UdpClientHelper_methods(root_module, cls):
    ## udp-client-server-helper.h: ns3::UdpClientHelper::UdpClientHelper(ns3::UdpClientHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpClientHelper const &', 'arg0')])
    ## udp-client-server-helper.h: ns3::UdpClientHelper::UdpClientHelper() [constructor]
    cls.add_constructor([])
    ## udp-client-server-helper.h: ns3::UdpClientHelper::UdpClientHelper(ns3::Ipv4Address ip, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-client-server-helper.h: ns3::ApplicationContainer ns3::UdpClientHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## udp-client-server-helper.h: void ns3::UdpClientHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3UdpEchoClientHelper_methods(root_module, cls):
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper::UdpEchoClientHelper(ns3::UdpEchoClientHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoClientHelper const &', 'arg0')])
    ## udp-echo-helper.h: ns3::UdpEchoClientHelper::UdpEchoClientHelper(ns3::Ipv4Address ip, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoClientHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoClientHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoClientHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetFill(ns3::Ptr<ns3::Application> app, std::string fill) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'app'), param('std::string', 'fill')])
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetFill(ns3::Ptr<ns3::Application> app, uint8_t fill, uint32_t dataLength) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'app'), param('uint8_t', 'fill'), param('uint32_t', 'dataLength')])
    ## udp-echo-helper.h: void ns3::UdpEchoClientHelper::SetFill(ns3::Ptr<ns3::Application> app, uint8_t * fill, uint32_t fillLength, uint32_t dataLength) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'app'), param('uint8_t *', 'fill'), param('uint32_t', 'fillLength'), param('uint32_t', 'dataLength')])
    return

def register_Ns3UdpEchoServerHelper_methods(root_module, cls):
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper::UdpEchoServerHelper(ns3::UdpEchoServerHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoServerHelper const &', 'arg0')])
    ## udp-echo-helper.h: ns3::UdpEchoServerHelper::UdpEchoServerHelper(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoServerHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoServerHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## udp-echo-helper.h: ns3::ApplicationContainer ns3::UdpEchoServerHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## udp-echo-helper.h: void ns3::UdpEchoServerHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3UdpServerHelper_methods(root_module, cls):
    ## udp-client-server-helper.h: ns3::UdpServerHelper::UdpServerHelper(ns3::UdpServerHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpServerHelper const &', 'arg0')])
    ## udp-client-server-helper.h: ns3::UdpServerHelper::UdpServerHelper() [constructor]
    cls.add_constructor([])
    ## udp-client-server-helper.h: ns3::UdpServerHelper::UdpServerHelper(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## udp-client-server-helper.h: ns3::Ptr<ns3::UdpServer> ns3::UdpServerHelper::GetServer() [member function]
    cls.add_method('GetServer', 
                   'ns3::Ptr< ns3::UdpServer >', 
                   [])
    ## udp-client-server-helper.h: ns3::ApplicationContainer ns3::UdpServerHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## udp-client-server-helper.h: void ns3::UdpServerHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3UdpTraceClientHelper_methods(root_module, cls):
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper::UdpTraceClientHelper(ns3::UdpTraceClientHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpTraceClientHelper const &', 'arg0')])
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper::UdpTraceClientHelper() [constructor]
    cls.add_constructor([])
    ## udp-client-server-helper.h: ns3::UdpTraceClientHelper::UdpTraceClientHelper(ns3::Ipv4Address ip, uint16_t port, std::string filename) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port'), param('std::string', 'filename')])
    ## udp-client-server-helper.h: ns3::ApplicationContainer ns3::UdpTraceClientHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## udp-client-server-helper.h: void ns3::UdpTraceClientHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3V4PingHelper_methods(root_module, cls):
    ## v4ping-helper.h: ns3::V4PingHelper::V4PingHelper(ns3::V4PingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::V4PingHelper const &', 'arg0')])
    ## v4ping-helper.h: ns3::V4PingHelper::V4PingHelper(ns3::Ipv4Address remote) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'remote')])
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
    ## v4ping-helper.h: ns3::ApplicationContainer ns3::V4PingHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## v4ping-helper.h: void ns3::V4PingHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3SeqTsHeader_methods(root_module, cls):
    ## seq-ts-header.h: ns3::SeqTsHeader::SeqTsHeader(ns3::SeqTsHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SeqTsHeader const &', 'arg0')])
    ## seq-ts-header.h: ns3::SeqTsHeader::SeqTsHeader() [constructor]
    cls.add_constructor([])
    ## seq-ts-header.h: uint32_t ns3::SeqTsHeader::GetSeq() const [member function]
    cls.add_method('GetSeq', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## seq-ts-header.h: ns3::Time ns3::SeqTsHeader::GetTs() const [member function]
    cls.add_method('GetTs', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## seq-ts-header.h: static ns3::TypeId ns3::SeqTsHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## seq-ts-header.h: void ns3::SeqTsHeader::SetSeq(uint32_t seq) [member function]
    cls.add_method('SetSeq', 
                   'void', 
                   [param('uint32_t', 'seq')])
    ## seq-ts-header.h: uint32_t ns3::SeqTsHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   visibility='private', is_virtual=True)
    ## seq-ts-header.h: ns3::TypeId ns3::SeqTsHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## seq-ts-header.h: uint32_t ns3::SeqTsHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## seq-ts-header.h: void ns3::SeqTsHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## seq-ts-header.h: void ns3::SeqTsHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3BulkSendApplication_methods(root_module, cls):
    ## bulk-send-application.h: ns3::BulkSendApplication::BulkSendApplication(ns3::BulkSendApplication const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BulkSendApplication const &', 'arg0')])
    ## bulk-send-application.h: ns3::BulkSendApplication::BulkSendApplication() [constructor]
    cls.add_constructor([])
    ## bulk-send-application.h: ns3::Ptr<ns3::Socket> ns3::BulkSendApplication::GetSocket() const [member function]
    cls.add_method('GetSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_const=True)
    ## bulk-send-application.h: static ns3::TypeId ns3::BulkSendApplication::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bulk-send-application.h: void ns3::BulkSendApplication::SetMaxBytes(uint32_t maxBytes) [member function]
    cls.add_method('SetMaxBytes', 
                   'void', 
                   [param('uint32_t', 'maxBytes')])
    ## bulk-send-application.h: void ns3::BulkSendApplication::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## bulk-send-application.h: void ns3::BulkSendApplication::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## bulk-send-application.h: void ns3::BulkSendApplication::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3OnOffApplication_methods(root_module, cls):
    ## onoff-application.h: ns3::OnOffApplication::OnOffApplication(ns3::OnOffApplication const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OnOffApplication const &', 'arg0')])
    ## onoff-application.h: ns3::OnOffApplication::OnOffApplication() [constructor]
    cls.add_constructor([])
    ## onoff-application.h: ns3::Ptr<ns3::Socket> ns3::OnOffApplication::GetSocket() const [member function]
    cls.add_method('GetSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_const=True)
    ## onoff-application.h: static ns3::TypeId ns3::OnOffApplication::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## onoff-application.h: void ns3::OnOffApplication::SetMaxBytes(uint32_t maxBytes) [member function]
    cls.add_method('SetMaxBytes', 
                   'void', 
                   [param('uint32_t', 'maxBytes')])
    ## onoff-application.h: void ns3::OnOffApplication::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## onoff-application.h: void ns3::OnOffApplication::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## onoff-application.h: void ns3::OnOffApplication::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PacketSink_methods(root_module, cls):
    ## packet-sink.h: ns3::PacketSink::PacketSink(ns3::PacketSink const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSink const &', 'arg0')])
    ## packet-sink.h: ns3::PacketSink::PacketSink() [constructor]
    cls.add_constructor([])
    ## packet-sink.h: std::list<ns3::Ptr<ns3::Socket>, std::allocator<ns3::Ptr<ns3::Socket> > > ns3::PacketSink::GetAcceptedSockets() const [member function]
    cls.add_method('GetAcceptedSockets', 
                   'std::list< ns3::Ptr< ns3::Socket > >', 
                   [], 
                   is_const=True)
    ## packet-sink.h: ns3::Ptr<ns3::Socket> ns3::PacketSink::GetListeningSocket() const [member function]
    cls.add_method('GetListeningSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_const=True)
    ## packet-sink.h: uint32_t ns3::PacketSink::GetTotalRx() const [member function]
    cls.add_method('GetTotalRx', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-sink.h: static ns3::TypeId ns3::PacketSink::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## packet-sink.h: void ns3::PacketSink::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## packet-sink.h: void ns3::PacketSink::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## packet-sink.h: void ns3::PacketSink::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Ping6_methods(root_module, cls):
    ## ping6.h: ns3::Ping6::Ping6(ns3::Ping6 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ping6 const &', 'arg0')])
    ## ping6.h: ns3::Ping6::Ping6() [constructor]
    cls.add_constructor([])
    ## ping6.h: static ns3::TypeId ns3::Ping6::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ping6.h: void ns3::Ping6::SetIfIndex(uint32_t ifIndex) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t', 'ifIndex')])
    ## ping6.h: void ns3::Ping6::SetLocal(ns3::Ipv6Address ipv6) [member function]
    cls.add_method('SetLocal', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ipv6')])
    ## ping6.h: void ns3::Ping6::SetRemote(ns3::Ipv6Address ipv6) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ipv6')])
    ## ping6.h: void ns3::Ping6::SetRouters(std::vector<ns3::Ipv6Address, std::allocator<ns3::Ipv6Address> > routers) [member function]
    cls.add_method('SetRouters', 
                   'void', 
                   [param('std::vector< ns3::Ipv6Address >', 'routers')])
    ## ping6.h: void ns3::Ping6::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ping6.h: void ns3::Ping6::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## ping6.h: void ns3::Ping6::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Radvd_methods(root_module, cls):
    ## radvd.h: ns3::Radvd::Radvd(ns3::Radvd const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Radvd const &', 'arg0')])
    ## radvd.h: ns3::Radvd::Radvd() [constructor]
    cls.add_constructor([])
    ## radvd.h: void ns3::Radvd::AddConfiguration(ns3::Ptr<ns3::RadvdInterface> routerInterface) [member function]
    cls.add_method('AddConfiguration', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadvdInterface >', 'routerInterface')])
    ## radvd.h: static ns3::TypeId ns3::Radvd::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radvd.h: ns3::Radvd::MAX_RA_DELAY_TIME [variable]
    cls.add_static_attribute('MAX_RA_DELAY_TIME', 'uint32_t const', is_const=True)
    ## radvd.h: void ns3::Radvd::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## radvd.h: void ns3::Radvd::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## radvd.h: void ns3::Radvd::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3RadvdInterface_methods(root_module, cls):
    ## radvd-interface.h: ns3::RadvdInterface::RadvdInterface(ns3::RadvdInterface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadvdInterface const &', 'arg0')])
    ## radvd-interface.h: ns3::RadvdInterface::RadvdInterface(uint32_t interface) [constructor]
    cls.add_constructor([param('uint32_t', 'interface')])
    ## radvd-interface.h: ns3::RadvdInterface::RadvdInterface(uint32_t interface, uint32_t maxRtrAdvInterval, uint32_t minRtrAdvInterval) [constructor]
    cls.add_constructor([param('uint32_t', 'interface'), param('uint32_t', 'maxRtrAdvInterval'), param('uint32_t', 'minRtrAdvInterval')])
    ## radvd-interface.h: void ns3::RadvdInterface::AddPrefix(ns3::Ptr<ns3::RadvdPrefix> routerPrefix) [member function]
    cls.add_method('AddPrefix', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadvdPrefix >', 'routerPrefix')])
    ## radvd-interface.h: uint8_t ns3::RadvdInterface::GetCurHopLimit() const [member function]
    cls.add_method('GetCurHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetDefaultLifeTime() const [member function]
    cls.add_method('GetDefaultLifeTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint8_t ns3::RadvdInterface::GetDefaultPreference() const [member function]
    cls.add_method('GetDefaultPreference', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetHomeAgentLifeTime() const [member function]
    cls.add_method('GetHomeAgentLifeTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetHomeAgentPreference() const [member function]
    cls.add_method('GetHomeAgentPreference', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetInterface() const [member function]
    cls.add_method('GetInterface', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetLinkMtu() const [member function]
    cls.add_method('GetLinkMtu', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetMaxRtrAdvInterval() const [member function]
    cls.add_method('GetMaxRtrAdvInterval', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetMinDelayBetweenRAs() const [member function]
    cls.add_method('GetMinDelayBetweenRAs', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetMinRtrAdvInterval() const [member function]
    cls.add_method('GetMinRtrAdvInterval', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: std::list<ns3::Ptr<ns3::RadvdPrefix>, std::allocator<ns3::Ptr<ns3::RadvdPrefix> > > ns3::RadvdInterface::GetPrefixes() const [member function]
    cls.add_method('GetPrefixes', 
                   'std::list< ns3::Ptr< ns3::RadvdPrefix > >', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetReachableTime() const [member function]
    cls.add_method('GetReachableTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: uint32_t ns3::RadvdInterface::GetRetransTimer() const [member function]
    cls.add_method('GetRetransTimer', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsHomeAgentFlag() const [member function]
    cls.add_method('IsHomeAgentFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsHomeAgentInfo() const [member function]
    cls.add_method('IsHomeAgentInfo', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsIntervalOpt() const [member function]
    cls.add_method('IsIntervalOpt', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsManagedFlag() const [member function]
    cls.add_method('IsManagedFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsMobRtrSupportFlag() const [member function]
    cls.add_method('IsMobRtrSupportFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsOtherConfigFlag() const [member function]
    cls.add_method('IsOtherConfigFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsSendAdvert() const [member function]
    cls.add_method('IsSendAdvert', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: bool ns3::RadvdInterface::IsSourceLLAddress() const [member function]
    cls.add_method('IsSourceLLAddress', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-interface.h: void ns3::RadvdInterface::SetCurHopLimit(uint8_t curHopLimit) [member function]
    cls.add_method('SetCurHopLimit', 
                   'void', 
                   [param('uint8_t', 'curHopLimit')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetDefaultLifeTime(uint32_t defaultLifeTime) [member function]
    cls.add_method('SetDefaultLifeTime', 
                   'void', 
                   [param('uint32_t', 'defaultLifeTime')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetDefaultPreference(uint8_t defaultPreference) [member function]
    cls.add_method('SetDefaultPreference', 
                   'void', 
                   [param('uint8_t', 'defaultPreference')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetHomeAgentFlag(bool homeAgentFlag) [member function]
    cls.add_method('SetHomeAgentFlag', 
                   'void', 
                   [param('bool', 'homeAgentFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetHomeAgentInfo(bool homeAgentFlag) [member function]
    cls.add_method('SetHomeAgentInfo', 
                   'void', 
                   [param('bool', 'homeAgentFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetHomeAgentLifeTime(uint32_t homeAgentLifeTime) [member function]
    cls.add_method('SetHomeAgentLifeTime', 
                   'void', 
                   [param('uint32_t', 'homeAgentLifeTime')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetHomeAgentPreference(uint32_t homeAgentPreference) [member function]
    cls.add_method('SetHomeAgentPreference', 
                   'void', 
                   [param('uint32_t', 'homeAgentPreference')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetIntervalOpt(bool intervalOpt) [member function]
    cls.add_method('SetIntervalOpt', 
                   'void', 
                   [param('bool', 'intervalOpt')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetLinkMtu(uint32_t linkMtu) [member function]
    cls.add_method('SetLinkMtu', 
                   'void', 
                   [param('uint32_t', 'linkMtu')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetManagedFlag(bool managedFlag) [member function]
    cls.add_method('SetManagedFlag', 
                   'void', 
                   [param('bool', 'managedFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetMaxRtrAdvInterval(uint32_t maxRtrAdvInterval) [member function]
    cls.add_method('SetMaxRtrAdvInterval', 
                   'void', 
                   [param('uint32_t', 'maxRtrAdvInterval')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetMinDelayBetweenRAs(uint32_t minDelayBetweenRAs) [member function]
    cls.add_method('SetMinDelayBetweenRAs', 
                   'void', 
                   [param('uint32_t', 'minDelayBetweenRAs')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetMinRtrAdvInterval(uint32_t minRtrAdvInterval) [member function]
    cls.add_method('SetMinRtrAdvInterval', 
                   'void', 
                   [param('uint32_t', 'minRtrAdvInterval')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetMobRtrSupportFlag(bool mobRtrSupportFlag) [member function]
    cls.add_method('SetMobRtrSupportFlag', 
                   'void', 
                   [param('bool', 'mobRtrSupportFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetOtherConfigFlag(bool otherConfigFlag) [member function]
    cls.add_method('SetOtherConfigFlag', 
                   'void', 
                   [param('bool', 'otherConfigFlag')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetReachableTime(uint32_t reachableTime) [member function]
    cls.add_method('SetReachableTime', 
                   'void', 
                   [param('uint32_t', 'reachableTime')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetRetransTimer(uint32_t retransTimer) [member function]
    cls.add_method('SetRetransTimer', 
                   'void', 
                   [param('uint32_t', 'retransTimer')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetSendAdvert(bool sendAdvert) [member function]
    cls.add_method('SetSendAdvert', 
                   'void', 
                   [param('bool', 'sendAdvert')])
    ## radvd-interface.h: void ns3::RadvdInterface::SetSourceLLAddress(bool sourceLLAddress) [member function]
    cls.add_method('SetSourceLLAddress', 
                   'void', 
                   [param('bool', 'sourceLLAddress')])
    return

def register_Ns3RadvdPrefix_methods(root_module, cls):
    ## radvd-prefix.h: ns3::RadvdPrefix::RadvdPrefix(ns3::RadvdPrefix const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadvdPrefix const &', 'arg0')])
    ## radvd-prefix.h: ns3::RadvdPrefix::RadvdPrefix(ns3::Ipv6Address network, uint8_t prefixLength, uint32_t preferredLifeTime=604800, uint32_t validLifeTime=2592000, bool onLinkFlag=true, bool autonomousFlag=true, bool routerAddrFlag=false) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'network'), param('uint8_t', 'prefixLength'), param('uint32_t', 'preferredLifeTime', default_value='604800'), param('uint32_t', 'validLifeTime', default_value='2592000'), param('bool', 'onLinkFlag', default_value='true'), param('bool', 'autonomousFlag', default_value='true'), param('bool', 'routerAddrFlag', default_value='false')])
    ## radvd-prefix.h: ns3::Ipv6Address ns3::RadvdPrefix::GetNetwork() const [member function]
    cls.add_method('GetNetwork', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: uint32_t ns3::RadvdPrefix::GetPreferredLifeTime() const [member function]
    cls.add_method('GetPreferredLifeTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: uint8_t ns3::RadvdPrefix::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: uint32_t ns3::RadvdPrefix::GetValidLifeTime() const [member function]
    cls.add_method('GetValidLifeTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: bool ns3::RadvdPrefix::IsAutonomousFlag() const [member function]
    cls.add_method('IsAutonomousFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: bool ns3::RadvdPrefix::IsOnLinkFlag() const [member function]
    cls.add_method('IsOnLinkFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: bool ns3::RadvdPrefix::IsRouterAddrFlag() const [member function]
    cls.add_method('IsRouterAddrFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetAutonomousFlag(bool autonomousFlag) [member function]
    cls.add_method('SetAutonomousFlag', 
                   'void', 
                   [param('bool', 'autonomousFlag')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetNetwork(ns3::Ipv6Address network) [member function]
    cls.add_method('SetNetwork', 
                   'void', 
                   [param('ns3::Ipv6Address', 'network')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetOnLinkFlag(bool onLinkFlag) [member function]
    cls.add_method('SetOnLinkFlag', 
                   'void', 
                   [param('bool', 'onLinkFlag')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetPreferredLifeTime(uint32_t preferredLifeTime) [member function]
    cls.add_method('SetPreferredLifeTime', 
                   'void', 
                   [param('uint32_t', 'preferredLifeTime')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetPrefixLength(uint8_t prefixLength) [member function]
    cls.add_method('SetPrefixLength', 
                   'void', 
                   [param('uint8_t', 'prefixLength')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetRouterAddrFlag(bool routerAddrFlag) [member function]
    cls.add_method('SetRouterAddrFlag', 
                   'void', 
                   [param('bool', 'routerAddrFlag')])
    ## radvd-prefix.h: void ns3::RadvdPrefix::SetValidLifeTime(uint32_t validLifeTime) [member function]
    cls.add_method('SetValidLifeTime', 
                   'void', 
                   [param('uint32_t', 'validLifeTime')])
    return

def register_Ns3UdpClient_methods(root_module, cls):
    ## udp-client.h: ns3::UdpClient::UdpClient(ns3::UdpClient const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpClient const &', 'arg0')])
    ## udp-client.h: ns3::UdpClient::UdpClient() [constructor]
    cls.add_constructor([])
    ## udp-client.h: static ns3::TypeId ns3::UdpClient::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-client.h: void ns3::UdpClient::SetRemote(ns3::Ipv4Address ip, uint16_t port) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-client.h: void ns3::UdpClient::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-client.h: void ns3::UdpClient::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-client.h: void ns3::UdpClient::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UdpEchoClient_methods(root_module, cls):
    ## udp-echo-client.h: ns3::UdpEchoClient::UdpEchoClient(ns3::UdpEchoClient const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoClient const &', 'arg0')])
    ## udp-echo-client.h: ns3::UdpEchoClient::UdpEchoClient() [constructor]
    cls.add_constructor([])
    ## udp-echo-client.h: uint32_t ns3::UdpEchoClient::GetDataSize() const [member function]
    cls.add_method('GetDataSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## udp-echo-client.h: static ns3::TypeId ns3::UdpEchoClient::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetDataSize(uint32_t dataSize) [member function]
    cls.add_method('SetDataSize', 
                   'void', 
                   [param('uint32_t', 'dataSize')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetFill(std::string fill) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('std::string', 'fill')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetFill(uint8_t fill, uint32_t dataSize) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('uint8_t', 'fill'), param('uint32_t', 'dataSize')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetFill(uint8_t * fill, uint32_t fillSize, uint32_t dataSize) [member function]
    cls.add_method('SetFill', 
                   'void', 
                   [param('uint8_t *', 'fill'), param('uint32_t', 'fillSize'), param('uint32_t', 'dataSize')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::SetRemote(ns3::Ipv4Address ip, uint16_t port) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-echo-client.h: void ns3::UdpEchoClient::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-echo-client.h: void ns3::UdpEchoClient::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-echo-client.h: void ns3::UdpEchoClient::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UdpEchoServer_methods(root_module, cls):
    ## udp-echo-server.h: ns3::UdpEchoServer::UdpEchoServer(ns3::UdpEchoServer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpEchoServer const &', 'arg0')])
    ## udp-echo-server.h: ns3::UdpEchoServer::UdpEchoServer() [constructor]
    cls.add_constructor([])
    ## udp-echo-server.h: static ns3::TypeId ns3::UdpEchoServer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-echo-server.h: void ns3::UdpEchoServer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-echo-server.h: void ns3::UdpEchoServer::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-echo-server.h: void ns3::UdpEchoServer::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UdpServer_methods(root_module, cls):
    ## udp-server.h: ns3::UdpServer::UdpServer(ns3::UdpServer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpServer const &', 'arg0')])
    ## udp-server.h: ns3::UdpServer::UdpServer() [constructor]
    cls.add_constructor([])
    ## udp-server.h: uint32_t ns3::UdpServer::GetLost() const [member function]
    cls.add_method('GetLost', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## udp-server.h: uint16_t ns3::UdpServer::GetPacketWindowSize() const [member function]
    cls.add_method('GetPacketWindowSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## udp-server.h: uint32_t ns3::UdpServer::GetReceived() const [member function]
    cls.add_method('GetReceived', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## udp-server.h: static ns3::TypeId ns3::UdpServer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-server.h: void ns3::UdpServer::SetPacketWindowSize(uint16_t size) [member function]
    cls.add_method('SetPacketWindowSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## udp-server.h: void ns3::UdpServer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-server.h: void ns3::UdpServer::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-server.h: void ns3::UdpServer::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UdpTraceClient_methods(root_module, cls):
    ## udp-trace-client.h: ns3::UdpTraceClient::UdpTraceClient(ns3::UdpTraceClient const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpTraceClient const &', 'arg0')])
    ## udp-trace-client.h: ns3::UdpTraceClient::UdpTraceClient() [constructor]
    cls.add_constructor([])
    ## udp-trace-client.h: ns3::UdpTraceClient::UdpTraceClient(ns3::Ipv4Address ip, uint16_t port, char * traceFile) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port'), param('char *', 'traceFile')])
    ## udp-trace-client.h: uint16_t ns3::UdpTraceClient::GetMaxPacketSize() [member function]
    cls.add_method('GetMaxPacketSize', 
                   'uint16_t', 
                   [])
    ## udp-trace-client.h: static ns3::TypeId ns3::UdpTraceClient::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## udp-trace-client.h: void ns3::UdpTraceClient::SetMaxPacketSize(uint16_t maxPacketSize) [member function]
    cls.add_method('SetMaxPacketSize', 
                   'void', 
                   [param('uint16_t', 'maxPacketSize')])
    ## udp-trace-client.h: void ns3::UdpTraceClient::SetRemote(ns3::Ipv4Address ip, uint16_t port) [member function]
    cls.add_method('SetRemote', 
                   'void', 
                   [param('ns3::Ipv4Address', 'ip'), param('uint16_t', 'port')])
    ## udp-trace-client.h: void ns3::UdpTraceClient::SetTraceFile(std::string filename) [member function]
    cls.add_method('SetTraceFile', 
                   'void', 
                   [param('std::string', 'filename')])
    ## udp-trace-client.h: void ns3::UdpTraceClient::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## udp-trace-client.h: void ns3::UdpTraceClient::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## udp-trace-client.h: void ns3::UdpTraceClient::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3V4Ping_methods(root_module, cls):
    ## v4ping.h: ns3::V4Ping::V4Ping(ns3::V4Ping const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::V4Ping const &', 'arg0')])
    ## v4ping.h: ns3::V4Ping::V4Ping() [constructor]
    cls.add_constructor([])
    ## v4ping.h: static ns3::TypeId ns3::V4Ping::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## v4ping.h: void ns3::V4Ping::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## v4ping.h: void ns3::V4Ping::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## v4ping.h: void ns3::V4Ping::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
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

