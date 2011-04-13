from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## pyviz.h: ns3::PyViz [class]
    module.add_class('PyViz')
    ## pyviz.h: ns3::PyViz::PacketCaptureMode [enumeration]
    module.add_enum('PacketCaptureMode', ['PACKET_CAPTURE_DISABLED', 'PACKET_CAPTURE_FILTER_HEADERS_OR', 'PACKET_CAPTURE_FILTER_HEADERS_AND'], outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::LastPacketsSample [struct]
    module.add_class('LastPacketsSample', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics [struct]
    module.add_class('NetDeviceStatistics', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::NodeStatistics [struct]
    module.add_class('NodeStatistics', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions [struct]
    module.add_class('PacketCaptureOptions', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::PacketDropSample [struct]
    module.add_class('PacketDropSample', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::PacketSample [struct]
    module.add_class('PacketSample', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::RxPacketSample [struct]
    module.add_class('RxPacketSample', parent=root_module['ns3::PyViz::PacketSample'], outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::TransmissionSample [struct]
    module.add_class('TransmissionSample', outer_class=root_module['ns3::PyViz'])
    ## pyviz.h: ns3::PyViz::TxPacketSample [struct]
    module.add_class('TxPacketSample', parent=root_module['ns3::PyViz::PacketSample'], outer_class=root_module['ns3::PyViz'])
    module.add_container('std::vector< ns3::PyViz::RxPacketSample >', 'ns3::PyViz::RxPacketSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::TxPacketSample >', 'ns3::PyViz::TxPacketSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::PacketSample >', 'ns3::PyViz::PacketSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::TransmissionSample >', 'ns3::PyViz::TransmissionSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::PacketDropSample >', 'ns3::PyViz::PacketDropSample', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::NetDeviceStatistics >', 'ns3::PyViz::NetDeviceStatistics', container_type='vector')
    module.add_container('std::vector< ns3::PyViz::NodeStatistics >', 'ns3::PyViz::NodeStatistics', container_type='vector')
    
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
    register_Ns3PyViz_methods(root_module, root_module['ns3::PyViz'])
    register_Ns3PyVizLastPacketsSample_methods(root_module, root_module['ns3::PyViz::LastPacketsSample'])
    register_Ns3PyVizNetDeviceStatistics_methods(root_module, root_module['ns3::PyViz::NetDeviceStatistics'])
    register_Ns3PyVizNodeStatistics_methods(root_module, root_module['ns3::PyViz::NodeStatistics'])
    register_Ns3PyVizPacketCaptureOptions_methods(root_module, root_module['ns3::PyViz::PacketCaptureOptions'])
    register_Ns3PyVizPacketDropSample_methods(root_module, root_module['ns3::PyViz::PacketDropSample'])
    register_Ns3PyVizPacketSample_methods(root_module, root_module['ns3::PyViz::PacketSample'])
    register_Ns3PyVizRxPacketSample_methods(root_module, root_module['ns3::PyViz::RxPacketSample'])
    register_Ns3PyVizTransmissionSample_methods(root_module, root_module['ns3::PyViz::TransmissionSample'])
    register_Ns3PyVizTxPacketSample_methods(root_module, root_module['ns3::PyViz::TxPacketSample'])
    return

def register_Ns3PyViz_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::PyViz(ns3::PyViz const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::PyViz() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::LastPacketsSample ns3::PyViz::GetLastPackets(uint32_t nodeId) const [member function]
    cls.add_method('GetLastPackets', 
                   'ns3::PyViz::LastPacketsSample', 
                   [param('uint32_t', 'nodeId')], 
                   is_const=True)
    ## pyviz.h: std::vector<ns3::PyViz::NodeStatistics,std::allocator<ns3::PyViz::NodeStatistics> > ns3::PyViz::GetNodesStatistics() const [member function]
    cls.add_method('GetNodesStatistics', 
                   'std::vector< ns3::PyViz::NodeStatistics >', 
                   [], 
                   is_const=True)
    ## pyviz.h: std::vector<ns3::PyViz::PacketDropSample,std::allocator<ns3::PyViz::PacketDropSample> > ns3::PyViz::GetPacketDropSamples() const [member function]
    cls.add_method('GetPacketDropSamples', 
                   'std::vector< ns3::PyViz::PacketDropSample >', 
                   [], 
                   is_const=True)
    ## pyviz.h: std::vector<std::string, std::allocator<std::string> > ns3::PyViz::GetPauseMessages() const [member function]
    cls.add_method('GetPauseMessages', 
                   'std::vector< std::string >', 
                   [], 
                   is_const=True)
    ## pyviz.h: std::vector<ns3::PyViz::TransmissionSample,std::allocator<ns3::PyViz::TransmissionSample> > ns3::PyViz::GetTransmissionSamples() const [member function]
    cls.add_method('GetTransmissionSamples', 
                   'std::vector< ns3::PyViz::TransmissionSample >', 
                   [], 
                   is_const=True)
    ## pyviz.h: static void ns3::PyViz::LineClipping(double boundsX1, double boundsY1, double boundsX2, double boundsY2, double & lineX1, double & lineY1, double & lineX2, double & lineY2) [member function]
    cls.add_method('LineClipping', 
                   'void', 
                   [param('double', 'boundsX1'), param('double', 'boundsY1'), param('double', 'boundsX2'), param('double', 'boundsY2'), param('double &', 'lineX1', direction=3), param('double &', 'lineY1', direction=3), param('double &', 'lineX2', direction=3), param('double &', 'lineY2', direction=3)], 
                   is_static=True)
    ## pyviz.h: static void ns3::PyViz::Pause(std::string const & message) [member function]
    cls.add_method('Pause', 
                   'void', 
                   [param('std::string const &', 'message')], 
                   is_static=True)
    ## pyviz.h: void ns3::PyViz::RegisterCsmaLikeDevice(std::string const & deviceTypeName) [member function]
    cls.add_method('RegisterCsmaLikeDevice', 
                   'void', 
                   [param('std::string const &', 'deviceTypeName')])
    ## pyviz.h: void ns3::PyViz::RegisterDropTracePath(std::string const & tracePath) [member function]
    cls.add_method('RegisterDropTracePath', 
                   'void', 
                   [param('std::string const &', 'tracePath')])
    ## pyviz.h: void ns3::PyViz::RegisterPointToPointLikeDevice(std::string const & deviceTypeName) [member function]
    cls.add_method('RegisterPointToPointLikeDevice', 
                   'void', 
                   [param('std::string const &', 'deviceTypeName')])
    ## pyviz.h: void ns3::PyViz::RegisterWifiLikeDevice(std::string const & deviceTypeName) [member function]
    cls.add_method('RegisterWifiLikeDevice', 
                   'void', 
                   [param('std::string const &', 'deviceTypeName')])
    ## pyviz.h: void ns3::PyViz::SetNodesOfInterest(std::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> > nodes) [member function]
    cls.add_method('SetNodesOfInterest', 
                   'void', 
                   [param('std::set< unsigned int >', 'nodes')])
    ## pyviz.h: void ns3::PyViz::SetPacketCaptureOptions(uint32_t nodeId, ns3::PyViz::PacketCaptureOptions options) [member function]
    cls.add_method('SetPacketCaptureOptions', 
                   'void', 
                   [param('uint32_t', 'nodeId'), param('ns3::PyViz::PacketCaptureOptions', 'options')])
    ## pyviz.h: void ns3::PyViz::SimulatorRunUntil(ns3::Time time) [member function]
    cls.add_method('SimulatorRunUntil', 
                   'void', 
                   [param('ns3::Time', 'time')])
    return

def register_Ns3PyVizLastPacketsSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::LastPacketsSample::LastPacketsSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::LastPacketsSample::LastPacketsSample(ns3::PyViz::LastPacketsSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::LastPacketsSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::LastPacketsSample::lastDroppedPackets [variable]
    cls.add_instance_attribute('lastDroppedPackets', 'std::vector< ns3::PyViz::PacketSample >', is_const=False)
    ## pyviz.h: ns3::PyViz::LastPacketsSample::lastReceivedPackets [variable]
    cls.add_instance_attribute('lastReceivedPackets', 'std::vector< ns3::PyViz::RxPacketSample >', is_const=False)
    ## pyviz.h: ns3::PyViz::LastPacketsSample::lastTransmittedPackets [variable]
    cls.add_instance_attribute('lastTransmittedPackets', 'std::vector< ns3::PyViz::TxPacketSample >', is_const=False)
    return

def register_Ns3PyVizNetDeviceStatistics_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::NetDeviceStatistics(ns3::PyViz::NetDeviceStatistics const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::NetDeviceStatistics const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::NetDeviceStatistics() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::receivedBytes [variable]
    cls.add_instance_attribute('receivedBytes', 'uint64_t', is_const=False)
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::receivedPackets [variable]
    cls.add_instance_attribute('receivedPackets', 'uint32_t', is_const=False)
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::transmittedBytes [variable]
    cls.add_instance_attribute('transmittedBytes', 'uint64_t', is_const=False)
    ## pyviz.h: ns3::PyViz::NetDeviceStatistics::transmittedPackets [variable]
    cls.add_instance_attribute('transmittedPackets', 'uint32_t', is_const=False)
    return

def register_Ns3PyVizNodeStatistics_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::NodeStatistics::NodeStatistics() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::NodeStatistics::NodeStatistics(ns3::PyViz::NodeStatistics const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::NodeStatistics const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::NodeStatistics::nodeId [variable]
    cls.add_instance_attribute('nodeId', 'uint32_t', is_const=False)
    ## pyviz.h: ns3::PyViz::NodeStatistics::statistics [variable]
    cls.add_instance_attribute('statistics', 'std::vector< ns3::PyViz::NetDeviceStatistics >', is_const=False)
    return

def register_Ns3PyVizPacketCaptureOptions_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::PacketCaptureOptions() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::PacketCaptureOptions(ns3::PyViz::PacketCaptureOptions const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::PacketCaptureOptions const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::headers [variable]
    cls.add_instance_attribute('headers', 'std::set< ns3::TypeId >', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::mode [variable]
    cls.add_instance_attribute('mode', 'ns3::PyViz::PacketCaptureMode', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketCaptureOptions::numLastPackets [variable]
    cls.add_instance_attribute('numLastPackets', 'uint32_t', is_const=False)
    return

def register_Ns3PyVizPacketDropSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::PacketDropSample::PacketDropSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::PacketDropSample::PacketDropSample(ns3::PyViz::PacketDropSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::PacketDropSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::PacketDropSample::bytes [variable]
    cls.add_instance_attribute('bytes', 'uint32_t', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketDropSample::transmitter [variable]
    cls.add_instance_attribute('transmitter', 'ns3::Ptr< ns3::Node >', is_const=False)
    return

def register_Ns3PyVizPacketSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::PacketSample::PacketSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::PacketSample::PacketSample(ns3::PyViz::PacketSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::PacketSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::PacketSample::device [variable]
    cls.add_instance_attribute('device', 'ns3::Ptr< ns3::NetDevice >', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketSample::packet [variable]
    cls.add_instance_attribute('packet', 'ns3::Ptr< ns3::Packet >', is_const=False)
    ## pyviz.h: ns3::PyViz::PacketSample::time [variable]
    cls.add_instance_attribute('time', 'ns3::Time', is_const=False)
    return

def register_Ns3PyVizRxPacketSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::RxPacketSample::RxPacketSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::RxPacketSample::RxPacketSample(ns3::PyViz::RxPacketSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::RxPacketSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::RxPacketSample::from [variable]
    cls.add_instance_attribute('from', 'ns3::Mac48Address', is_const=False)
    return

def register_Ns3PyVizTransmissionSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::TransmissionSample::TransmissionSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::TransmissionSample::TransmissionSample(ns3::PyViz::TransmissionSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::TransmissionSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::TransmissionSample::bytes [variable]
    cls.add_instance_attribute('bytes', 'uint32_t', is_const=False)
    ## pyviz.h: ns3::PyViz::TransmissionSample::channel [variable]
    cls.add_instance_attribute('channel', 'ns3::Ptr< ns3::Channel >', is_const=False)
    ## pyviz.h: ns3::PyViz::TransmissionSample::receiver [variable]
    cls.add_instance_attribute('receiver', 'ns3::Ptr< ns3::Node >', is_const=False)
    ## pyviz.h: ns3::PyViz::TransmissionSample::transmitter [variable]
    cls.add_instance_attribute('transmitter', 'ns3::Ptr< ns3::Node >', is_const=False)
    return

def register_Ns3PyVizTxPacketSample_methods(root_module, cls):
    ## pyviz.h: ns3::PyViz::TxPacketSample::TxPacketSample() [constructor]
    cls.add_constructor([])
    ## pyviz.h: ns3::PyViz::TxPacketSample::TxPacketSample(ns3::PyViz::TxPacketSample const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PyViz::TxPacketSample const &', 'arg0')])
    ## pyviz.h: ns3::PyViz::TxPacketSample::to [variable]
    cls.add_instance_attribute('to', 'ns3::Mac48Address', is_const=False)
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

