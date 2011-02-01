from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## flow-monitor-helper.h: ns3::FlowMonitorHelper [class]
    module.add_class('FlowMonitorHelper')
    ## histogram.h: ns3::Histogram [class]
    module.add_class('Histogram')
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter<ns3::FlowClassifier> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::FlowClassifier', 'ns3::empty', 'ns3::DefaultDeleter<ns3::FlowClassifier>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter<ns3::FlowProbe> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::FlowProbe', 'ns3::empty', 'ns3::DefaultDeleter<ns3::FlowProbe>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## flow-classifier.h: ns3::FlowClassifier [class]
    module.add_class('FlowClassifier', parent=root_module['ns3::SimpleRefCount< ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter<ns3::FlowClassifier> >'])
    ## flow-monitor.h: ns3::FlowMonitor [class]
    module.add_class('FlowMonitor', parent=root_module['ns3::Object'])
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats [struct]
    module.add_class('FlowStats', outer_class=root_module['ns3::FlowMonitor'])
    ## flow-probe.h: ns3::FlowProbe [class]
    module.add_class('FlowProbe', parent=root_module['ns3::SimpleRefCount< ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter<ns3::FlowProbe> >'])
    ## flow-probe.h: ns3::FlowProbe::FlowStats [struct]
    module.add_class('FlowStats', outer_class=root_module['ns3::FlowProbe'])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier [class]
    module.add_class('Ipv4FlowClassifier', parent=root_module['ns3::FlowClassifier'])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple [struct]
    module.add_class('FiveTuple', outer_class=root_module['ns3::Ipv4FlowClassifier'])
    ## ipv4-flow-probe.h: ns3::Ipv4FlowProbe [class]
    module.add_class('Ipv4FlowProbe', parent=root_module['ns3::FlowProbe'])
    ## ipv4-flow-probe.h: ns3::Ipv4FlowProbe::DropReason [enumeration]
    module.add_enum('DropReason', ['DROP_NO_ROUTE', 'DROP_TTL_EXPIRE', 'DROP_BAD_CHECKSUM', 'DROP_QUEUE', 'DROP_INTERFACE_DOWN', 'DROP_ROUTE_ERROR', 'DROP_INVALID_REASON'], outer_class=root_module['ns3::Ipv4FlowProbe'])
    module.add_container('std::map< unsigned int, ns3::FlowMonitor::FlowStats >', ('unsigned int', 'ns3::FlowMonitor::FlowStats'), container_type='map')
    module.add_container('std::map< unsigned int, ns3::FlowProbe::FlowStats >', ('unsigned int', 'ns3::FlowProbe::FlowStats'), container_type='map')
    typehandlers.add_type_alias('uint32_t', 'ns3::FlowPacketId')
    typehandlers.add_type_alias('uint32_t*', 'ns3::FlowPacketId*')
    typehandlers.add_type_alias('uint32_t&', 'ns3::FlowPacketId&')
    typehandlers.add_type_alias('uint32_t', 'ns3::FlowId')
    typehandlers.add_type_alias('uint32_t*', 'ns3::FlowId*')
    typehandlers.add_type_alias('uint32_t&', 'ns3::FlowId&')
    
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
    register_Ns3FlowMonitorHelper_methods(root_module, root_module['ns3::FlowMonitorHelper'])
    register_Ns3Histogram_methods(root_module, root_module['ns3::Histogram'])
    register_Ns3FlowClassifier_methods(root_module, root_module['ns3::FlowClassifier'])
    register_Ns3FlowMonitor_methods(root_module, root_module['ns3::FlowMonitor'])
    register_Ns3FlowMonitorFlowStats_methods(root_module, root_module['ns3::FlowMonitor::FlowStats'])
    register_Ns3FlowProbe_methods(root_module, root_module['ns3::FlowProbe'])
    register_Ns3FlowProbeFlowStats_methods(root_module, root_module['ns3::FlowProbe::FlowStats'])
    register_Ns3Ipv4FlowClassifier_methods(root_module, root_module['ns3::Ipv4FlowClassifier'])
    register_Ns3Ipv4FlowClassifierFiveTuple_methods(root_module, root_module['ns3::Ipv4FlowClassifier::FiveTuple'])
    register_Ns3Ipv4FlowProbe_methods(root_module, root_module['ns3::Ipv4FlowProbe'])
    return

def register_Ns3FlowMonitorHelper_methods(root_module, cls):
    ## flow-monitor-helper.h: ns3::FlowMonitorHelper::FlowMonitorHelper(ns3::FlowMonitorHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowMonitorHelper const &', 'arg0')])
    ## flow-monitor-helper.h: ns3::FlowMonitorHelper::FlowMonitorHelper() [constructor]
    cls.add_constructor([])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowClassifier> ns3::FlowMonitorHelper::GetClassifier() [member function]
    cls.add_method('GetClassifier', 
                   'ns3::Ptr< ns3::FlowClassifier >', 
                   [])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowMonitor> ns3::FlowMonitorHelper::GetMonitor() [member function]
    cls.add_method('GetMonitor', 
                   'ns3::Ptr< ns3::FlowMonitor >', 
                   [])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowMonitor> ns3::FlowMonitorHelper::Install(ns3::NodeContainer nodes) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::FlowMonitor >', 
                   [param('ns3::NodeContainer', 'nodes')])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowMonitor> ns3::FlowMonitorHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::FlowMonitor >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## flow-monitor-helper.h: ns3::Ptr<ns3::FlowMonitor> ns3::FlowMonitorHelper::InstallAll() [member function]
    cls.add_method('InstallAll', 
                   'ns3::Ptr< ns3::FlowMonitor >', 
                   [])
    ## flow-monitor-helper.h: void ns3::FlowMonitorHelper::SetMonitorAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetMonitorAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    return

def register_Ns3Histogram_methods(root_module, cls):
    ## histogram.h: ns3::Histogram::Histogram(ns3::Histogram const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Histogram const &', 'arg0')])
    ## histogram.h: ns3::Histogram::Histogram(double binWidth) [constructor]
    cls.add_constructor([param('double', 'binWidth')])
    ## histogram.h: ns3::Histogram::Histogram() [constructor]
    cls.add_constructor([])
    ## histogram.h: void ns3::Histogram::AddValue(double value) [member function]
    cls.add_method('AddValue', 
                   'void', 
                   [param('double', 'value')])
    ## histogram.h: uint32_t ns3::Histogram::GetBinCount(uint32_t index) [member function]
    cls.add_method('GetBinCount', 
                   'uint32_t', 
                   [param('uint32_t', 'index')])
    ## histogram.h: double ns3::Histogram::GetBinEnd(uint32_t index) [member function]
    cls.add_method('GetBinEnd', 
                   'double', 
                   [param('uint32_t', 'index')])
    ## histogram.h: double ns3::Histogram::GetBinStart(uint32_t index) [member function]
    cls.add_method('GetBinStart', 
                   'double', 
                   [param('uint32_t', 'index')])
    ## histogram.h: double ns3::Histogram::GetBinWidth(uint32_t index) const [member function]
    cls.add_method('GetBinWidth', 
                   'double', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## histogram.h: uint32_t ns3::Histogram::GetNBins() const [member function]
    cls.add_method('GetNBins', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## histogram.h: void ns3::Histogram::SerializeToXmlStream(std::ostream & os, int indent, std::string elementName) const [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent'), param('std::string', 'elementName')], 
                   is_const=True)
    ## histogram.h: void ns3::Histogram::SetDefaultBinWidth(double binWidth) [member function]
    cls.add_method('SetDefaultBinWidth', 
                   'void', 
                   [param('double', 'binWidth')])
    return

def register_Ns3FlowClassifier_methods(root_module, cls):
    ## flow-classifier.h: ns3::FlowClassifier::FlowClassifier() [constructor]
    cls.add_constructor([])
    ## flow-classifier.h: void ns3::FlowClassifier::SerializeToXmlStream(std::ostream & os, int indent) const [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## flow-classifier.h: ns3::FlowId ns3::FlowClassifier::GetNewFlowId() [member function]
    cls.add_method('GetNewFlowId', 
                   'ns3::FlowId', 
                   [], 
                   visibility='protected')
    return

def register_Ns3FlowMonitor_methods(root_module, cls):
    ## flow-monitor.h: ns3::FlowMonitor::FlowMonitor(ns3::FlowMonitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowMonitor const &', 'arg0')])
    ## flow-monitor.h: ns3::FlowMonitor::FlowMonitor() [constructor]
    cls.add_constructor([])
    ## flow-monitor.h: void ns3::FlowMonitor::AddProbe(ns3::Ptr<ns3::FlowProbe> probe) [member function]
    cls.add_method('AddProbe', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe')])
    ## flow-monitor.h: void ns3::FlowMonitor::CheckForLostPackets() [member function]
    cls.add_method('CheckForLostPackets', 
                   'void', 
                   [])
    ## flow-monitor.h: void ns3::FlowMonitor::CheckForLostPackets(ns3::Time maxDelay) [member function]
    cls.add_method('CheckForLostPackets', 
                   'void', 
                   [param('ns3::Time', 'maxDelay')])
    ## flow-monitor.h: std::vector<ns3::Ptr<ns3::FlowProbe>, std::allocator<ns3::Ptr<ns3::FlowProbe> > > ns3::FlowMonitor::GetAllProbes() const [member function]
    cls.add_method('GetAllProbes', 
                   'std::vector< ns3::Ptr< ns3::FlowProbe > >', 
                   [], 
                   is_const=True)
    ## flow-monitor.h: std::map<unsigned int, ns3::FlowMonitor::FlowStats, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, ns3::FlowMonitor::FlowStats> > > ns3::FlowMonitor::GetFlowStats() const [member function]
    cls.add_method('GetFlowStats', 
                   'std::map< unsigned int, ns3::FlowMonitor::FlowStats >', 
                   [], 
                   is_const=True)
    ## flow-monitor.h: ns3::TypeId ns3::FlowMonitor::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flow-monitor.h: static ns3::TypeId ns3::FlowMonitor::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flow-monitor.h: void ns3::FlowMonitor::ReportDrop(ns3::Ptr<ns3::FlowProbe> probe, ns3::FlowId flowId, ns3::FlowPacketId packetId, uint32_t packetSize, uint32_t reasonCode) [member function]
    cls.add_method('ReportDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe'), param('ns3::FlowId', 'flowId'), param('ns3::FlowPacketId', 'packetId'), param('uint32_t', 'packetSize'), param('uint32_t', 'reasonCode')])
    ## flow-monitor.h: void ns3::FlowMonitor::ReportFirstTx(ns3::Ptr<ns3::FlowProbe> probe, ns3::FlowId flowId, ns3::FlowPacketId packetId, uint32_t packetSize) [member function]
    cls.add_method('ReportFirstTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe'), param('ns3::FlowId', 'flowId'), param('ns3::FlowPacketId', 'packetId'), param('uint32_t', 'packetSize')])
    ## flow-monitor.h: void ns3::FlowMonitor::ReportForwarding(ns3::Ptr<ns3::FlowProbe> probe, ns3::FlowId flowId, ns3::FlowPacketId packetId, uint32_t packetSize) [member function]
    cls.add_method('ReportForwarding', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe'), param('ns3::FlowId', 'flowId'), param('ns3::FlowPacketId', 'packetId'), param('uint32_t', 'packetSize')])
    ## flow-monitor.h: void ns3::FlowMonitor::ReportLastRx(ns3::Ptr<ns3::FlowProbe> probe, ns3::FlowId flowId, ns3::FlowPacketId packetId, uint32_t packetSize) [member function]
    cls.add_method('ReportLastRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowProbe >', 'probe'), param('ns3::FlowId', 'flowId'), param('ns3::FlowPacketId', 'packetId'), param('uint32_t', 'packetSize')])
    ## flow-monitor.h: void ns3::FlowMonitor::SerializeToXmlFile(std::string fileName, bool enableHistograms, bool enableProbes) [member function]
    cls.add_method('SerializeToXmlFile', 
                   'void', 
                   [param('std::string', 'fileName'), param('bool', 'enableHistograms'), param('bool', 'enableProbes')])
    ## flow-monitor.h: void ns3::FlowMonitor::SerializeToXmlStream(std::ostream & os, int indent, bool enableHistograms, bool enableProbes) [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent'), param('bool', 'enableHistograms'), param('bool', 'enableProbes')])
    ## flow-monitor.h: std::string ns3::FlowMonitor::SerializeToXmlString(int indent, bool enableHistograms, bool enableProbes) [member function]
    cls.add_method('SerializeToXmlString', 
                   'std::string', 
                   [param('int', 'indent'), param('bool', 'enableHistograms'), param('bool', 'enableProbes')])
    ## flow-monitor.h: void ns3::FlowMonitor::SetFlowClassifier(ns3::Ptr<ns3::FlowClassifier> classifier) [member function]
    cls.add_method('SetFlowClassifier', 
                   'void', 
                   [param('ns3::Ptr< ns3::FlowClassifier >', 'classifier')])
    ## flow-monitor.h: void ns3::FlowMonitor::Start(ns3::Time const & time) [member function]
    cls.add_method('Start', 
                   'void', 
                   [param('ns3::Time const &', 'time')])
    ## flow-monitor.h: void ns3::FlowMonitor::StartRightNow() [member function]
    cls.add_method('StartRightNow', 
                   'void', 
                   [])
    ## flow-monitor.h: void ns3::FlowMonitor::Stop(ns3::Time const & time) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time const &', 'time')])
    ## flow-monitor.h: void ns3::FlowMonitor::StopRightNow() [member function]
    cls.add_method('StopRightNow', 
                   'void', 
                   [])
    ## flow-monitor.h: void ns3::FlowMonitor::NotifyConstructionCompleted() [member function]
    cls.add_method('NotifyConstructionCompleted', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3FlowMonitorFlowStats_methods(root_module, cls):
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::FlowStats() [constructor]
    cls.add_constructor([])
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::FlowStats(ns3::FlowMonitor::FlowStats const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowMonitor::FlowStats const &', 'arg0')])
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::bytesDropped [variable]
    cls.add_instance_attribute('bytesDropped', 'std::vector< unsigned long >', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::delayHistogram [variable]
    cls.add_instance_attribute('delayHistogram', 'ns3::Histogram', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::delaySum [variable]
    cls.add_instance_attribute('delaySum', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::flowInterruptionsHistogram [variable]
    cls.add_instance_attribute('flowInterruptionsHistogram', 'ns3::Histogram', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::jitterHistogram [variable]
    cls.add_instance_attribute('jitterHistogram', 'ns3::Histogram', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::jitterSum [variable]
    cls.add_instance_attribute('jitterSum', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::lastDelay [variable]
    cls.add_instance_attribute('lastDelay', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::lostPackets [variable]
    cls.add_instance_attribute('lostPackets', 'uint32_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::packetSizeHistogram [variable]
    cls.add_instance_attribute('packetSizeHistogram', 'ns3::Histogram', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::packetsDropped [variable]
    cls.add_instance_attribute('packetsDropped', 'std::vector< unsigned int >', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::rxBytes [variable]
    cls.add_instance_attribute('rxBytes', 'uint64_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::rxPackets [variable]
    cls.add_instance_attribute('rxPackets', 'uint32_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timeFirstRxPacket [variable]
    cls.add_instance_attribute('timeFirstRxPacket', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timeFirstTxPacket [variable]
    cls.add_instance_attribute('timeFirstTxPacket', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timeLastRxPacket [variable]
    cls.add_instance_attribute('timeLastRxPacket', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timeLastTxPacket [variable]
    cls.add_instance_attribute('timeLastTxPacket', 'ns3::Time', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::timesForwarded [variable]
    cls.add_instance_attribute('timesForwarded', 'uint32_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::txBytes [variable]
    cls.add_instance_attribute('txBytes', 'uint64_t', is_const=False)
    ## flow-monitor.h: ns3::FlowMonitor::FlowStats::txPackets [variable]
    cls.add_instance_attribute('txPackets', 'uint32_t', is_const=False)
    return

def register_Ns3FlowProbe_methods(root_module, cls):
    ## flow-probe.h: void ns3::FlowProbe::AddPacketDropStats(ns3::FlowId flowId, uint32_t packetSize, uint32_t reasonCode) [member function]
    cls.add_method('AddPacketDropStats', 
                   'void', 
                   [param('ns3::FlowId', 'flowId'), param('uint32_t', 'packetSize'), param('uint32_t', 'reasonCode')])
    ## flow-probe.h: void ns3::FlowProbe::AddPacketStats(ns3::FlowId flowId, uint32_t packetSize, ns3::Time delayFromFirstProbe) [member function]
    cls.add_method('AddPacketStats', 
                   'void', 
                   [param('ns3::FlowId', 'flowId'), param('uint32_t', 'packetSize'), param('ns3::Time', 'delayFromFirstProbe')])
    ## flow-probe.h: std::map<unsigned int, ns3::FlowProbe::FlowStats, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, ns3::FlowProbe::FlowStats> > > ns3::FlowProbe::GetStats() const [member function]
    cls.add_method('GetStats', 
                   'std::map< unsigned int, ns3::FlowProbe::FlowStats >', 
                   [], 
                   is_const=True)
    ## flow-probe.h: void ns3::FlowProbe::SerializeToXmlStream(std::ostream & os, int indent, uint32_t index) const [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent'), param('uint32_t', 'index')], 
                   is_const=True)
    ## flow-probe.h: ns3::FlowProbe::FlowProbe(ns3::Ptr<ns3::FlowMonitor> flowMonitor) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::FlowMonitor >', 'flowMonitor')], 
                        visibility='protected')
    return

def register_Ns3FlowProbeFlowStats_methods(root_module, cls):
    ## flow-probe.h: ns3::FlowProbe::FlowStats::FlowStats(ns3::FlowProbe::FlowStats const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowProbe::FlowStats const &', 'arg0')])
    ## flow-probe.h: ns3::FlowProbe::FlowStats::FlowStats() [constructor]
    cls.add_constructor([])
    ## flow-probe.h: ns3::FlowProbe::FlowStats::bytes [variable]
    cls.add_instance_attribute('bytes', 'uint64_t', is_const=False)
    ## flow-probe.h: ns3::FlowProbe::FlowStats::bytesDropped [variable]
    cls.add_instance_attribute('bytesDropped', 'std::vector< unsigned long >', is_const=False)
    ## flow-probe.h: ns3::FlowProbe::FlowStats::delayFromFirstProbeSum [variable]
    cls.add_instance_attribute('delayFromFirstProbeSum', 'ns3::Time', is_const=False)
    ## flow-probe.h: ns3::FlowProbe::FlowStats::packets [variable]
    cls.add_instance_attribute('packets', 'uint32_t', is_const=False)
    ## flow-probe.h: ns3::FlowProbe::FlowStats::packetsDropped [variable]
    cls.add_instance_attribute('packetsDropped', 'std::vector< unsigned int >', is_const=False)
    return

def register_Ns3Ipv4FlowClassifier_methods(root_module, cls):
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::Ipv4FlowClassifier() [constructor]
    cls.add_constructor([])
    ## ipv4-flow-classifier.h: bool ns3::Ipv4FlowClassifier::Classify(ns3::Ipv4Header const & ipHeader, ns3::Ptr<ns3::Packet const> ipPayload, uint32_t * out_flowId, uint32_t * out_packetId) [member function]
    cls.add_method('Classify', 
                   'bool', 
                   [param('ns3::Ipv4Header const &', 'ipHeader'), param('ns3::Ptr< ns3::Packet const >', 'ipPayload'), param('uint32_t *', 'out_flowId'), param('uint32_t *', 'out_packetId')])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple ns3::Ipv4FlowClassifier::FindFlow(ns3::FlowId flowId) const [member function]
    cls.add_method('FindFlow', 
                   'ns3::Ipv4FlowClassifier::FiveTuple', 
                   [param('ns3::FlowId', 'flowId')], 
                   is_const=True)
    ## ipv4-flow-classifier.h: void ns3::Ipv4FlowClassifier::SerializeToXmlStream(std::ostream & os, int indent) const [member function]
    cls.add_method('SerializeToXmlStream', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'indent')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv4FlowClassifierFiveTuple_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::FiveTuple() [constructor]
    cls.add_constructor([])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::FiveTuple(ns3::Ipv4FlowClassifier::FiveTuple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4FlowClassifier::FiveTuple const &', 'arg0')])
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::destinationAddress [variable]
    cls.add_instance_attribute('destinationAddress', 'ns3::Ipv4Address', is_const=False)
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::destinationPort [variable]
    cls.add_instance_attribute('destinationPort', 'uint16_t', is_const=False)
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::protocol [variable]
    cls.add_instance_attribute('protocol', 'uint8_t', is_const=False)
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::sourceAddress [variable]
    cls.add_instance_attribute('sourceAddress', 'ns3::Ipv4Address', is_const=False)
    ## ipv4-flow-classifier.h: ns3::Ipv4FlowClassifier::FiveTuple::sourcePort [variable]
    cls.add_instance_attribute('sourcePort', 'uint16_t', is_const=False)
    return

def register_Ns3Ipv4FlowProbe_methods(root_module, cls):
    ## ipv4-flow-probe.h: ns3::Ipv4FlowProbe::Ipv4FlowProbe(ns3::Ptr<ns3::FlowMonitor> monitor, ns3::Ptr<ns3::Ipv4FlowClassifier> classifier, ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::FlowMonitor >', 'monitor'), param('ns3::Ptr< ns3::Ipv4FlowClassifier >', 'classifier'), param('ns3::Ptr< ns3::Node >', 'node')])
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

