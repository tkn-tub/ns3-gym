from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## adhoc-aloha-noack-ideal-phy-helper.h: ns3::AdhocAlohaNoackIdealPhyHelper [class]
    module.add_class('AdhocAlohaNoackIdealPhyHelper')
    ## spectrum-model.h: ns3::BandInfo [struct]
    module.add_class('BandInfo')
    ## microwave-oven-spectrum-value-helper.h: ns3::MicrowaveOvenSpectrumValueHelper [class]
    module.add_class('MicrowaveOvenSpectrumValueHelper')
    ## multi-model-spectrum-channel.h: ns3::RxSpectrumModelInfo [class]
    module.add_class('RxSpectrumModelInfo')
    ## spectrum-analyzer-helper.h: ns3::SpectrumAnalyzerHelper [class]
    module.add_class('SpectrumAnalyzerHelper')
    ## spectrum-helper.h: ns3::SpectrumChannelHelper [class]
    module.add_class('SpectrumChannelHelper')
    ## spectrum-helper.h: ns3::SpectrumPhyHelper [class]
    module.add_class('SpectrumPhyHelper')
    ## spectrum-type.h: ns3::SpectrumType [class]
    module.add_class('SpectrumType')
    ## spectrum-type.h: ns3::SpectrumTypeFactory [class]
    module.add_class('SpectrumTypeFactory')
    ## multi-model-spectrum-channel.h: ns3::TxSpectrumModelInfo [class]
    module.add_class('TxSpectrumModelInfo')
    ## waveform-generator-helper.h: ns3::WaveformGeneratorHelper [class]
    module.add_class('WaveformGeneratorHelper')
    ## wifi-spectrum-value-helper.h: ns3::WifiSpectrumValue5MhzFactory [class]
    module.add_class('WifiSpectrumValue5MhzFactory', allow_subclassing=True)
    ## wifi-spectrum-value-helper.h: ns3::WifiSpectrumValueHelper [class]
    module.add_class('WifiSpectrumValueHelper', allow_subclassing=True)
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::SpectrumConverter', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumConverter>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::SpectrumModel', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumModel>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::SpectrumValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## spectrum-converter.h: ns3::SpectrumConverter [class]
    module.add_class('SpectrumConverter', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> >'])
    ## spectrum-error-model.h: ns3::SpectrumErrorModel [class]
    module.add_class('SpectrumErrorModel', parent=root_module['ns3::Object'])
    ## spectrum-interference.h: ns3::SpectrumInterference [class]
    module.add_class('SpectrumInterference', parent=root_module['ns3::Object'])
    ## spectrum-model.h: ns3::SpectrumModel [class]
    module.add_class('SpectrumModel', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >'])
    ## spectrum-phy.h: ns3::SpectrumPhy [class]
    module.add_class('SpectrumPhy', parent=root_module['ns3::Object'])
    ## spectrum-propagation-loss-model.h: ns3::SpectrumPropagationLossModel [class]
    module.add_class('SpectrumPropagationLossModel', parent=root_module['ns3::Object'])
    ## spectrum-value.h: ns3::SpectrumValue [class]
    module.add_class('SpectrumValue', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >'])
    ## waveform-generator.h: ns3::WaveformGenerator [class]
    module.add_class('WaveformGenerator', parent=root_module['ns3::SpectrumPhy'])
    ## aloha-noack-mac-header.h: ns3::AlohaNoackMacHeader [class]
    module.add_class('AlohaNoackMacHeader', parent=root_module['ns3::Header'])
    ## friis-spectrum-propagation-loss.h: ns3::FriisSpectrumPropagationLossModel [class]
    module.add_class('FriisSpectrumPropagationLossModel', parent=root_module['ns3::SpectrumPropagationLossModel'])
    ## half-duplex-ideal-phy.h: ns3::HalfDuplexIdealPhy [class]
    module.add_class('HalfDuplexIdealPhy', parent=root_module['ns3::SpectrumPhy'])
    ## half-duplex-ideal-phy.h: ns3::HalfDuplexIdealPhy::State [enumeration]
    module.add_enum('State', ['IDLE', 'TX', 'RX'], outer_class=root_module['ns3::HalfDuplexIdealPhy'])
    ## non-communicating-net-device.h: ns3::NonCommunicatingNetDevice [class]
    module.add_class('NonCommunicatingNetDevice', parent=root_module['ns3::NetDevice'])
    ## spectrum-error-model.h: ns3::ShannonSpectrumErrorModel [class]
    module.add_class('ShannonSpectrumErrorModel', parent=root_module['ns3::SpectrumErrorModel'])
    ## spectrum-analyzer.h: ns3::SpectrumAnalyzer [class]
    module.add_class('SpectrumAnalyzer', parent=root_module['ns3::SpectrumPhy'])
    ## spectrum-channel.h: ns3::SpectrumChannel [class]
    module.add_class('SpectrumChannel', parent=root_module['ns3::Channel'])
    ## aloha-noack-net-device.h: ns3::AlohaNoackNetDevice [class]
    module.add_class('AlohaNoackNetDevice', parent=root_module['ns3::NetDevice'])
    ## aloha-noack-net-device.h: ns3::AlohaNoackNetDevice::State [enumeration]
    module.add_enum('State', ['IDLE', 'TX', 'RX'], outer_class=root_module['ns3::AlohaNoackNetDevice'])
    ## multi-model-spectrum-channel.h: ns3::MultiModelSpectrumChannel [class]
    module.add_class('MultiModelSpectrumChannel', parent=root_module['ns3::SpectrumChannel'])
    ## single-model-spectrum-channel.h: ns3::SingleModelSpectrumChannel [class]
    module.add_class('SingleModelSpectrumChannel', parent=root_module['ns3::SpectrumChannel'])
    module.add_container('ns3::SpectrumConverterMap_t', ('unsigned int', 'ns3::SpectrumConverter'), container_type='map')
    module.add_container('ns3::Bands', 'ns3::BandInfo', container_type='vector')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >', 'ns3::Values')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >*', 'ns3::Values*')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >&', 'ns3::Values&')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::SpectrumConverter, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::SpectrumConverter > > >', 'ns3::SpectrumConverterMap_t')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::SpectrumConverter, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::SpectrumConverter > > >*', 'ns3::SpectrumConverterMap_t*')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::SpectrumConverter, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::SpectrumConverter > > >&', 'ns3::SpectrumConverterMap_t&')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::TxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::TxSpectrumModelInfo > > >', 'ns3::TxSpectrumModelInfoMap_t')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::TxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::TxSpectrumModelInfo > > >*', 'ns3::TxSpectrumModelInfoMap_t*')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::TxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::TxSpectrumModelInfo > > >&', 'ns3::TxSpectrumModelInfoMap_t&')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >', 'ns3::Bands')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >*', 'ns3::Bands*')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >&', 'ns3::Bands&')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::RxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::RxSpectrumModelInfo > > >', 'ns3::RxSpectrumModelInfoMap_t')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::RxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::RxSpectrumModelInfo > > >*', 'ns3::RxSpectrumModelInfoMap_t*')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::RxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::RxSpectrumModelInfo > > >&', 'ns3::RxSpectrumModelInfoMap_t&')
    typehandlers.add_type_alias('uint32_t', 'ns3::SpectrumModelUid_t')
    typehandlers.add_type_alias('uint32_t*', 'ns3::SpectrumModelUid_t*')
    typehandlers.add_type_alias('uint32_t&', 'ns3::SpectrumModelUid_t&')
    
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
    register_Ns3AdhocAlohaNoackIdealPhyHelper_methods(root_module, root_module['ns3::AdhocAlohaNoackIdealPhyHelper'])
    register_Ns3BandInfo_methods(root_module, root_module['ns3::BandInfo'])
    register_Ns3MicrowaveOvenSpectrumValueHelper_methods(root_module, root_module['ns3::MicrowaveOvenSpectrumValueHelper'])
    register_Ns3RxSpectrumModelInfo_methods(root_module, root_module['ns3::RxSpectrumModelInfo'])
    register_Ns3SpectrumAnalyzerHelper_methods(root_module, root_module['ns3::SpectrumAnalyzerHelper'])
    register_Ns3SpectrumChannelHelper_methods(root_module, root_module['ns3::SpectrumChannelHelper'])
    register_Ns3SpectrumPhyHelper_methods(root_module, root_module['ns3::SpectrumPhyHelper'])
    register_Ns3SpectrumType_methods(root_module, root_module['ns3::SpectrumType'])
    register_Ns3SpectrumTypeFactory_methods(root_module, root_module['ns3::SpectrumTypeFactory'])
    register_Ns3TxSpectrumModelInfo_methods(root_module, root_module['ns3::TxSpectrumModelInfo'])
    register_Ns3WaveformGeneratorHelper_methods(root_module, root_module['ns3::WaveformGeneratorHelper'])
    register_Ns3WifiSpectrumValue5MhzFactory_methods(root_module, root_module['ns3::WifiSpectrumValue5MhzFactory'])
    register_Ns3WifiSpectrumValueHelper_methods(root_module, root_module['ns3::WifiSpectrumValueHelper'])
    register_Ns3SpectrumConverter_methods(root_module, root_module['ns3::SpectrumConverter'])
    register_Ns3SpectrumErrorModel_methods(root_module, root_module['ns3::SpectrumErrorModel'])
    register_Ns3SpectrumInterference_methods(root_module, root_module['ns3::SpectrumInterference'])
    register_Ns3SpectrumModel_methods(root_module, root_module['ns3::SpectrumModel'])
    register_Ns3SpectrumPhy_methods(root_module, root_module['ns3::SpectrumPhy'])
    register_Ns3SpectrumPropagationLossModel_methods(root_module, root_module['ns3::SpectrumPropagationLossModel'])
    register_Ns3SpectrumValue_methods(root_module, root_module['ns3::SpectrumValue'])
    register_Ns3WaveformGenerator_methods(root_module, root_module['ns3::WaveformGenerator'])
    register_Ns3AlohaNoackMacHeader_methods(root_module, root_module['ns3::AlohaNoackMacHeader'])
    register_Ns3FriisSpectrumPropagationLossModel_methods(root_module, root_module['ns3::FriisSpectrumPropagationLossModel'])
    register_Ns3HalfDuplexIdealPhy_methods(root_module, root_module['ns3::HalfDuplexIdealPhy'])
    register_Ns3NonCommunicatingNetDevice_methods(root_module, root_module['ns3::NonCommunicatingNetDevice'])
    register_Ns3ShannonSpectrumErrorModel_methods(root_module, root_module['ns3::ShannonSpectrumErrorModel'])
    register_Ns3SpectrumAnalyzer_methods(root_module, root_module['ns3::SpectrumAnalyzer'])
    register_Ns3SpectrumChannel_methods(root_module, root_module['ns3::SpectrumChannel'])
    register_Ns3AlohaNoackNetDevice_methods(root_module, root_module['ns3::AlohaNoackNetDevice'])
    register_Ns3MultiModelSpectrumChannel_methods(root_module, root_module['ns3::MultiModelSpectrumChannel'])
    register_Ns3SingleModelSpectrumChannel_methods(root_module, root_module['ns3::SingleModelSpectrumChannel'])
    return

def register_Ns3AdhocAlohaNoackIdealPhyHelper_methods(root_module, cls):
    ## adhoc-aloha-noack-ideal-phy-helper.h: ns3::AdhocAlohaNoackIdealPhyHelper::AdhocAlohaNoackIdealPhyHelper(ns3::AdhocAlohaNoackIdealPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AdhocAlohaNoackIdealPhyHelper const &', 'arg0')])
    ## adhoc-aloha-noack-ideal-phy-helper.h: ns3::AdhocAlohaNoackIdealPhyHelper::AdhocAlohaNoackIdealPhyHelper() [constructor]
    cls.add_constructor([])
    ## adhoc-aloha-noack-ideal-phy-helper.h: ns3::NetDeviceContainer ns3::AdhocAlohaNoackIdealPhyHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## adhoc-aloha-noack-ideal-phy-helper.h: ns3::NetDeviceContainer ns3::AdhocAlohaNoackIdealPhyHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## adhoc-aloha-noack-ideal-phy-helper.h: ns3::NetDeviceContainer ns3::AdhocAlohaNoackIdealPhyHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## adhoc-aloha-noack-ideal-phy-helper.h: void ns3::AdhocAlohaNoackIdealPhyHelper::SetChannel(ns3::Ptr<ns3::SpectrumChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'channel')])
    ## adhoc-aloha-noack-ideal-phy-helper.h: void ns3::AdhocAlohaNoackIdealPhyHelper::SetChannel(std::string channelName) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('std::string', 'channelName')])
    ## adhoc-aloha-noack-ideal-phy-helper.h: void ns3::AdhocAlohaNoackIdealPhyHelper::SetDeviceAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## adhoc-aloha-noack-ideal-phy-helper.h: void ns3::AdhocAlohaNoackIdealPhyHelper::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue >', 'noisePsd')])
    ## adhoc-aloha-noack-ideal-phy-helper.h: void ns3::AdhocAlohaNoackIdealPhyHelper::SetPhyAttribute(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetPhyAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')])
    ## adhoc-aloha-noack-ideal-phy-helper.h: void ns3::AdhocAlohaNoackIdealPhyHelper::SetTxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue> txPsd) [member function]
    cls.add_method('SetTxPowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd')])
    return

def register_Ns3BandInfo_methods(root_module, cls):
    ## spectrum-model.h: ns3::BandInfo::BandInfo() [constructor]
    cls.add_constructor([])
    ## spectrum-model.h: ns3::BandInfo::BandInfo(ns3::BandInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BandInfo const &', 'arg0')])
    ## spectrum-model.h: ns3::BandInfo::fc [variable]
    cls.add_instance_attribute('fc', 'double', is_const=False)
    ## spectrum-model.h: ns3::BandInfo::fh [variable]
    cls.add_instance_attribute('fh', 'double', is_const=False)
    ## spectrum-model.h: ns3::BandInfo::fl [variable]
    cls.add_instance_attribute('fl', 'double', is_const=False)
    return

def register_Ns3MicrowaveOvenSpectrumValueHelper_methods(root_module, cls):
    ## microwave-oven-spectrum-value-helper.h: ns3::MicrowaveOvenSpectrumValueHelper::MicrowaveOvenSpectrumValueHelper() [constructor]
    cls.add_constructor([])
    ## microwave-oven-spectrum-value-helper.h: ns3::MicrowaveOvenSpectrumValueHelper::MicrowaveOvenSpectrumValueHelper(ns3::MicrowaveOvenSpectrumValueHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MicrowaveOvenSpectrumValueHelper const &', 'arg0')])
    ## microwave-oven-spectrum-value-helper.h: static ns3::Ptr<ns3::SpectrumValue> ns3::MicrowaveOvenSpectrumValueHelper::CreatePowerSpectralDensityMwo1() [member function]
    cls.add_method('CreatePowerSpectralDensityMwo1', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_static=True)
    ## microwave-oven-spectrum-value-helper.h: static ns3::Ptr<ns3::SpectrumValue> ns3::MicrowaveOvenSpectrumValueHelper::CreatePowerSpectralDensityMwo2() [member function]
    cls.add_method('CreatePowerSpectralDensityMwo2', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_static=True)
    return

def register_Ns3RxSpectrumModelInfo_methods(root_module, cls):
    ## multi-model-spectrum-channel.h: ns3::RxSpectrumModelInfo::RxSpectrumModelInfo(ns3::RxSpectrumModelInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RxSpectrumModelInfo const &', 'arg0')])
    ## multi-model-spectrum-channel.h: ns3::RxSpectrumModelInfo::RxSpectrumModelInfo(ns3::Ptr<ns3::SpectrumModel const> rxSpectrumModel) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::SpectrumModel const >', 'rxSpectrumModel')])
    ## multi-model-spectrum-channel.h: ns3::RxSpectrumModelInfo::m_rxPhyList [variable]
    cls.add_instance_attribute('m_rxPhyList', 'std::list< ns3::Ptr< ns3::SpectrumPhy > >', is_const=False)
    ## multi-model-spectrum-channel.h: ns3::RxSpectrumModelInfo::m_rxSpectrumModel [variable]
    cls.add_instance_attribute('m_rxSpectrumModel', 'ns3::Ptr< ns3::SpectrumModel const >', is_const=False)
    return

def register_Ns3SpectrumAnalyzerHelper_methods(root_module, cls):
    ## spectrum-analyzer-helper.h: ns3::SpectrumAnalyzerHelper::SpectrumAnalyzerHelper(ns3::SpectrumAnalyzerHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumAnalyzerHelper const &', 'arg0')])
    ## spectrum-analyzer-helper.h: ns3::SpectrumAnalyzerHelper::SpectrumAnalyzerHelper() [constructor]
    cls.add_constructor([])
    ## spectrum-analyzer-helper.h: void ns3::SpectrumAnalyzerHelper::EnableAsciiAll(std::string prefix) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::string', 'prefix')])
    ## spectrum-analyzer-helper.h: ns3::NetDeviceContainer ns3::SpectrumAnalyzerHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## spectrum-analyzer-helper.h: ns3::NetDeviceContainer ns3::SpectrumAnalyzerHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## spectrum-analyzer-helper.h: ns3::NetDeviceContainer ns3::SpectrumAnalyzerHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## spectrum-analyzer-helper.h: void ns3::SpectrumAnalyzerHelper::SetChannel(ns3::Ptr<ns3::SpectrumChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'channel')])
    ## spectrum-analyzer-helper.h: void ns3::SpectrumAnalyzerHelper::SetChannel(std::string channelName) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('std::string', 'channelName')])
    ## spectrum-analyzer-helper.h: void ns3::SpectrumAnalyzerHelper::SetDeviceAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## spectrum-analyzer-helper.h: void ns3::SpectrumAnalyzerHelper::SetPhyAttribute(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetPhyAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')])
    ## spectrum-analyzer-helper.h: void ns3::SpectrumAnalyzerHelper::SetRxSpectrumModel(ns3::Ptr<ns3::SpectrumModel> m) [member function]
    cls.add_method('SetRxSpectrumModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumModel >', 'm')])
    return

def register_Ns3SpectrumChannelHelper_methods(root_module, cls):
    ## spectrum-helper.h: ns3::SpectrumChannelHelper::SpectrumChannelHelper() [constructor]
    cls.add_constructor([])
    ## spectrum-helper.h: ns3::SpectrumChannelHelper::SpectrumChannelHelper(ns3::SpectrumChannelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumChannelHelper const &', 'arg0')])
    ## spectrum-helper.h: void ns3::SpectrumChannelHelper::AddSpectrumPropagationLoss(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('AddSpectrumPropagationLoss', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## spectrum-helper.h: ns3::Ptr<ns3::SpectrumChannel> ns3::SpectrumChannelHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::SpectrumChannel >', 
                   [], 
                   is_const=True)
    ## spectrum-helper.h: static ns3::SpectrumChannelHelper ns3::SpectrumChannelHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::SpectrumChannelHelper', 
                   [], 
                   is_static=True)
    ## spectrum-helper.h: void ns3::SpectrumChannelHelper::SetChannel(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## spectrum-helper.h: void ns3::SpectrumChannelHelper::SetPropagationDelay(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPropagationDelay', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    return

def register_Ns3SpectrumPhyHelper_methods(root_module, cls):
    ## spectrum-helper.h: ns3::SpectrumPhyHelper::SpectrumPhyHelper() [constructor]
    cls.add_constructor([])
    ## spectrum-helper.h: ns3::SpectrumPhyHelper::SpectrumPhyHelper(ns3::SpectrumPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumPhyHelper const &', 'arg0')])
    ## spectrum-helper.h: ns3::Ptr<ns3::SpectrumPhy> ns3::SpectrumPhyHelper::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::NetDevice> device) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::SpectrumPhy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   is_const=True)
    ## spectrum-helper.h: void ns3::SpectrumPhyHelper::SetChannel(ns3::Ptr<ns3::SpectrumChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'channel')])
    ## spectrum-helper.h: void ns3::SpectrumPhyHelper::SetChannel(std::string channelName) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('std::string', 'channelName')])
    ## spectrum-helper.h: void ns3::SpectrumPhyHelper::SetPhy(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## spectrum-helper.h: void ns3::SpectrumPhyHelper::SetPhyAttribute(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetPhyAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')])
    return

def register_Ns3SpectrumType_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## spectrum-type.h: ns3::SpectrumType::SpectrumType(ns3::SpectrumType const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumType const &', 'arg0')])
    ## spectrum-type.h: std::string ns3::SpectrumType::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## spectrum-type.h: uint32_t ns3::SpectrumType::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3SpectrumTypeFactory_methods(root_module, cls):
    ## spectrum-type.h: ns3::SpectrumTypeFactory::SpectrumTypeFactory() [constructor]
    cls.add_constructor([])
    ## spectrum-type.h: ns3::SpectrumTypeFactory::SpectrumTypeFactory(ns3::SpectrumTypeFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumTypeFactory const &', 'arg0')])
    ## spectrum-type.h: static ns3::SpectrumType ns3::SpectrumTypeFactory::Create(std::string name) [member function]
    cls.add_method('Create', 
                   'ns3::SpectrumType', 
                   [param('std::string', 'name')], 
                   is_static=True)
    ## spectrum-type.h: static std::string ns3::SpectrumTypeFactory::GetNameByUid(uint32_t uid) [member function]
    cls.add_method('GetNameByUid', 
                   'std::string', 
                   [param('uint32_t', 'uid')], 
                   is_static=True)
    return

def register_Ns3TxSpectrumModelInfo_methods(root_module, cls):
    ## multi-model-spectrum-channel.h: ns3::TxSpectrumModelInfo::TxSpectrumModelInfo(ns3::TxSpectrumModelInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TxSpectrumModelInfo const &', 'arg0')])
    ## multi-model-spectrum-channel.h: ns3::TxSpectrumModelInfo::TxSpectrumModelInfo(ns3::Ptr<ns3::SpectrumModel const> txSpectrumModel) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::SpectrumModel const >', 'txSpectrumModel')])
    ## multi-model-spectrum-channel.h: ns3::TxSpectrumModelInfo::m_spectrumConverterMap [variable]
    cls.add_instance_attribute('m_spectrumConverterMap', 'ns3::SpectrumConverterMap_t', is_const=False)
    ## multi-model-spectrum-channel.h: ns3::TxSpectrumModelInfo::m_txSpectrumModel [variable]
    cls.add_instance_attribute('m_txSpectrumModel', 'ns3::Ptr< ns3::SpectrumModel const >', is_const=False)
    return

def register_Ns3WaveformGeneratorHelper_methods(root_module, cls):
    ## waveform-generator-helper.h: ns3::WaveformGeneratorHelper::WaveformGeneratorHelper(ns3::WaveformGeneratorHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WaveformGeneratorHelper const &', 'arg0')])
    ## waveform-generator-helper.h: ns3::WaveformGeneratorHelper::WaveformGeneratorHelper() [constructor]
    cls.add_constructor([])
    ## waveform-generator-helper.h: ns3::NetDeviceContainer ns3::WaveformGeneratorHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## waveform-generator-helper.h: ns3::NetDeviceContainer ns3::WaveformGeneratorHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## waveform-generator-helper.h: ns3::NetDeviceContainer ns3::WaveformGeneratorHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## waveform-generator-helper.h: void ns3::WaveformGeneratorHelper::SetChannel(ns3::Ptr<ns3::SpectrumChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'channel')])
    ## waveform-generator-helper.h: void ns3::WaveformGeneratorHelper::SetChannel(std::string channelName) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('std::string', 'channelName')])
    ## waveform-generator-helper.h: void ns3::WaveformGeneratorHelper::SetDeviceAttribute(std::string n1, ns3::AttributeValue const & v1) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n1'), param('ns3::AttributeValue const &', 'v1')])
    ## waveform-generator-helper.h: void ns3::WaveformGeneratorHelper::SetPhyAttribute(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetPhyAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')])
    ## waveform-generator-helper.h: void ns3::WaveformGeneratorHelper::SetTxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue> txPsd) [member function]
    cls.add_method('SetTxPowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd')])
    return

def register_Ns3WifiSpectrumValue5MhzFactory_methods(root_module, cls):
    ## wifi-spectrum-value-helper.h: ns3::WifiSpectrumValue5MhzFactory::WifiSpectrumValue5MhzFactory() [constructor]
    cls.add_constructor([])
    ## wifi-spectrum-value-helper.h: ns3::WifiSpectrumValue5MhzFactory::WifiSpectrumValue5MhzFactory(ns3::WifiSpectrumValue5MhzFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiSpectrumValue5MhzFactory const &', 'arg0')])
    ## wifi-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::WifiSpectrumValue5MhzFactory::CreateConstant(double psd) [member function]
    cls.add_method('CreateConstant', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'psd')], 
                   is_virtual=True)
    ## wifi-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::WifiSpectrumValue5MhzFactory::CreateRfFilter(uint32_t channel) [member function]
    cls.add_method('CreateRfFilter', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('uint32_t', 'channel')], 
                   is_virtual=True)
    ## wifi-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::WifiSpectrumValue5MhzFactory::CreateTxPowerSpectralDensity(double txPower, uint32_t channel) [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'txPower'), param('uint32_t', 'channel')], 
                   is_virtual=True)
    return

def register_Ns3WifiSpectrumValueHelper_methods(root_module, cls):
    ## wifi-spectrum-value-helper.h: ns3::WifiSpectrumValueHelper::WifiSpectrumValueHelper() [constructor]
    cls.add_constructor([])
    ## wifi-spectrum-value-helper.h: ns3::WifiSpectrumValueHelper::WifiSpectrumValueHelper(ns3::WifiSpectrumValueHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiSpectrumValueHelper const &', 'arg0')])
    ## wifi-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::WifiSpectrumValueHelper::CreateConstant(double psd) [member function]
    cls.add_method('CreateConstant', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'psd')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::WifiSpectrumValueHelper::CreateRfFilter(uint32_t channel) [member function]
    cls.add_method('CreateRfFilter', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('uint32_t', 'channel')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-spectrum-value-helper.h: ns3::Ptr<ns3::SpectrumValue> ns3::WifiSpectrumValueHelper::CreateTxPowerSpectralDensity(double txPower, uint32_t channel) [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'txPower'), param('uint32_t', 'channel')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3SpectrumConverter_methods(root_module, cls):
    ## spectrum-converter.h: ns3::SpectrumConverter::SpectrumConverter(ns3::SpectrumConverter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumConverter const &', 'arg0')])
    ## spectrum-converter.h: ns3::SpectrumConverter::SpectrumConverter(ns3::Ptr<ns3::SpectrumModel const> fromSpectrumModel, ns3::Ptr<ns3::SpectrumModel const> toSpectrumModel) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::SpectrumModel const >', 'fromSpectrumModel'), param('ns3::Ptr< ns3::SpectrumModel const >', 'toSpectrumModel')])
    ## spectrum-converter.h: ns3::SpectrumConverter::SpectrumConverter() [constructor]
    cls.add_constructor([])
    ## spectrum-converter.h: ns3::Ptr<ns3::SpectrumValue> ns3::SpectrumConverter::Convert(ns3::Ptr<ns3::SpectrumValue const> vvf) const [member function]
    cls.add_method('Convert', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'vvf')], 
                   is_const=True)
    return

def register_Ns3SpectrumErrorModel_methods(root_module, cls):
    ## spectrum-error-model.h: ns3::SpectrumErrorModel::SpectrumErrorModel() [constructor]
    cls.add_constructor([])
    ## spectrum-error-model.h: ns3::SpectrumErrorModel::SpectrumErrorModel(ns3::SpectrumErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumErrorModel const &', 'arg0')])
    ## spectrum-error-model.h: void ns3::SpectrumErrorModel::EvaluateChunk(ns3::SpectrumValue const & sinr, ns3::Time duration) [member function]
    cls.add_method('EvaluateChunk', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr'), param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-error-model.h: bool ns3::SpectrumErrorModel::IsRxCorrect() [member function]
    cls.add_method('IsRxCorrect', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-error-model.h: void ns3::SpectrumErrorModel::StartRx(ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3SpectrumInterference_methods(root_module, cls):
    ## spectrum-interference.h: ns3::SpectrumInterference::SpectrumInterference(ns3::SpectrumInterference const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumInterference const &', 'arg0')])
    ## spectrum-interference.h: ns3::SpectrumInterference::SpectrumInterference() [constructor]
    cls.add_constructor([])
    ## spectrum-interference.h: void ns3::SpectrumInterference::AddSignal(ns3::Ptr<ns3::SpectrumValue const> spd, ns3::Time const duration) [member function]
    cls.add_method('AddSignal', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'spd'), param('ns3::Time const', 'duration')])
    ## spectrum-interference.h: bool ns3::SpectrumInterference::EndRx() [member function]
    cls.add_method('EndRx', 
                   'bool', 
                   [])
    ## spectrum-interference.h: void ns3::SpectrumInterference::SetErrorModel(ns3::Ptr<ns3::SpectrumErrorModel> e) [member function]
    cls.add_method('SetErrorModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumErrorModel >', 'e')])
    ## spectrum-interference.h: void ns3::SpectrumInterference::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
    ## spectrum-interference.h: void ns3::SpectrumInterference::StartRx(ns3::Ptr<ns3::Packet const> p, ns3::Ptr<ns3::SpectrumValue const> rxPsd) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd')])
    ## spectrum-interference.h: void ns3::SpectrumInterference::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3SpectrumModel_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## spectrum-model.h: ns3::SpectrumModel::SpectrumModel(ns3::SpectrumModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumModel const &', 'arg0')])
    ## spectrum-model.h: ns3::SpectrumModel::SpectrumModel(std::vector<double, std::allocator<double> > centerFreqs) [constructor]
    cls.add_constructor([param('std::vector< double >', 'centerFreqs')])
    ## spectrum-model.h: ns3::SpectrumModel::SpectrumModel(ns3::Bands bands) [constructor]
    cls.add_constructor([param('ns3::Bands', 'bands')])
    ## spectrum-model.h: __gnu_cxx::__normal_iterator<const ns3::BandInfo*,std::vector<ns3::BandInfo, std::allocator<ns3::BandInfo> > > ns3::SpectrumModel::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::BandInfo const *, std::vector< ns3::BandInfo > >', 
                   [], 
                   is_const=True)
    ## spectrum-model.h: __gnu_cxx::__normal_iterator<const ns3::BandInfo*,std::vector<ns3::BandInfo, std::allocator<ns3::BandInfo> > > ns3::SpectrumModel::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::BandInfo const *, std::vector< ns3::BandInfo > >', 
                   [], 
                   is_const=True)
    ## spectrum-model.h: size_t ns3::SpectrumModel::GetNumBands() const [member function]
    cls.add_method('GetNumBands', 
                   'size_t', 
                   [], 
                   is_const=True)
    ## spectrum-model.h: ns3::SpectrumModelUid_t ns3::SpectrumModel::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'ns3::SpectrumModelUid_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3SpectrumPhy_methods(root_module, cls):
    ## spectrum-phy.h: ns3::SpectrumPhy::SpectrumPhy() [constructor]
    cls.add_constructor([])
    ## spectrum-phy.h: ns3::SpectrumPhy::SpectrumPhy(ns3::SpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumPhy const &', 'arg0')])
    ## spectrum-phy.h: ns3::Ptr<ns3::Object> ns3::SpectrumPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: ns3::Ptr<ns3::Object> ns3::SpectrumPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: ns3::Ptr<ns3::SpectrumModel const> ns3::SpectrumPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## spectrum-phy.h: static ns3::TypeId ns3::SpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-phy.h: void ns3::SpectrumPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: void ns3::SpectrumPhy::SetDevice(ns3::Ptr<ns3::Object> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'd')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: void ns3::SpectrumPhy::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h: void ns3::SpectrumPhy::StartRx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::SpectrumType st, ns3::Time duration) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3SpectrumPropagationLossModel_methods(root_module, cls):
    ## spectrum-propagation-loss-model.h: ns3::SpectrumPropagationLossModel::SpectrumPropagationLossModel(ns3::SpectrumPropagationLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumPropagationLossModel const &', 'arg0')])
    ## spectrum-propagation-loss-model.h: ns3::SpectrumPropagationLossModel::SpectrumPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## spectrum-propagation-loss-model.h: ns3::Ptr<ns3::SpectrumValue> ns3::SpectrumPropagationLossModel::CalcRxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> txPsd, ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('CalcRxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'txPsd'), param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_const=True)
    ## spectrum-propagation-loss-model.h: static ns3::TypeId ns3::SpectrumPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-propagation-loss-model.h: void ns3::SpectrumPropagationLossModel::SetNext(ns3::Ptr<ns3::SpectrumPropagationLossModel> next) [member function]
    cls.add_method('SetNext', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPropagationLossModel >', 'next')])
    ## spectrum-propagation-loss-model.h: void ns3::SpectrumPropagationLossModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## spectrum-propagation-loss-model.h: ns3::Ptr<ns3::SpectrumValue> ns3::SpectrumPropagationLossModel::DoCalcRxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> txPsd, ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'txPsd'), param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3SpectrumValue_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_numeric_operator('*', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('double', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('ns3::SpectrumValue const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('double', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('ns3::SpectrumValue const &', 'right'))
    cls.add_unary_numeric_operator('-')
    cls.add_binary_numeric_operator('-', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('double', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('ns3::SpectrumValue const &', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('double', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('*=', param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('*=', param('double', 'right'))
    cls.add_inplace_numeric_operator('+=', param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('+=', param('double', 'right'))
    cls.add_inplace_numeric_operator('-=', param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('-=', param('double', 'right'))
    cls.add_inplace_numeric_operator('/=', param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('/=', param('double', 'right'))
    ## spectrum-value.h: ns3::SpectrumValue::SpectrumValue(ns3::SpectrumValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumValue const &', 'arg0')])
    ## spectrum-value.h: ns3::SpectrumValue::SpectrumValue(ns3::Ptr<ns3::SpectrumModel const> sm) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::SpectrumModel const >', 'sm')])
    ## spectrum-value.h: ns3::SpectrumValue::SpectrumValue() [constructor]
    cls.add_constructor([])
    ## spectrum-value.h: __gnu_cxx::__normal_iterator<const ns3::BandInfo*,std::vector<ns3::BandInfo, std::allocator<ns3::BandInfo> > > ns3::SpectrumValue::ConstBandsBegin() const [member function]
    cls.add_method('ConstBandsBegin', 
                   '__gnu_cxx::__normal_iterator< ns3::BandInfo const *, std::vector< ns3::BandInfo > >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h: __gnu_cxx::__normal_iterator<const ns3::BandInfo*,std::vector<ns3::BandInfo, std::allocator<ns3::BandInfo> > > ns3::SpectrumValue::ConstBandsEnd() const [member function]
    cls.add_method('ConstBandsEnd', 
                   '__gnu_cxx::__normal_iterator< ns3::BandInfo const *, std::vector< ns3::BandInfo > >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h: __gnu_cxx::__normal_iterator<const double*,std::vector<double, std::allocator<double> > > ns3::SpectrumValue::ConstValuesBegin() const [member function]
    cls.add_method('ConstValuesBegin', 
                   '__gnu_cxx::__normal_iterator< double const *, std::vector< double > >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h: __gnu_cxx::__normal_iterator<const double*,std::vector<double, std::allocator<double> > > ns3::SpectrumValue::ConstValuesEnd() const [member function]
    cls.add_method('ConstValuesEnd', 
                   '__gnu_cxx::__normal_iterator< double const *, std::vector< double > >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h: ns3::Ptr<ns3::SpectrumValue> ns3::SpectrumValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h: ns3::Ptr<ns3::SpectrumModel const> ns3::SpectrumValue::GetSpectrumModel() const [member function]
    cls.add_method('GetSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h: ns3::SpectrumModelUid_t ns3::SpectrumValue::GetSpectrumModelUid() const [member function]
    cls.add_method('GetSpectrumModelUid', 
                   'ns3::SpectrumModelUid_t', 
                   [], 
                   is_const=True)
    ## spectrum-value.h: __gnu_cxx::__normal_iterator<double*,std::vector<double, std::allocator<double> > > ns3::SpectrumValue::ValuesBegin() [member function]
    cls.add_method('ValuesBegin', 
                   '__gnu_cxx::__normal_iterator< double *, std::vector< double > >', 
                   [])
    ## spectrum-value.h: __gnu_cxx::__normal_iterator<double*,std::vector<double, std::allocator<double> > > ns3::SpectrumValue::ValuesEnd() [member function]
    cls.add_method('ValuesEnd', 
                   '__gnu_cxx::__normal_iterator< double *, std::vector< double > >', 
                   [])
    return

def register_Ns3WaveformGenerator_methods(root_module, cls):
    ## waveform-generator.h: ns3::WaveformGenerator::WaveformGenerator(ns3::WaveformGenerator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WaveformGenerator const &', 'arg0')])
    ## waveform-generator.h: ns3::WaveformGenerator::WaveformGenerator() [constructor]
    cls.add_constructor([])
    ## waveform-generator.h: ns3::Ptr<ns3::Object> ns3::WaveformGenerator::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## waveform-generator.h: double ns3::WaveformGenerator::GetDutyCycle() const [member function]
    cls.add_method('GetDutyCycle', 
                   'double', 
                   [], 
                   is_const=True)
    ## waveform-generator.h: ns3::Ptr<ns3::Object> ns3::WaveformGenerator::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## waveform-generator.h: ns3::Time ns3::WaveformGenerator::GetPeriod() const [member function]
    cls.add_method('GetPeriod', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## waveform-generator.h: ns3::Ptr<ns3::SpectrumModel const> ns3::WaveformGenerator::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## waveform-generator.h: ns3::SpectrumType ns3::WaveformGenerator::GetSpectrumType() [member function]
    cls.add_method('GetSpectrumType', 
                   'ns3::SpectrumType', 
                   [])
    ## waveform-generator.h: static ns3::TypeId ns3::WaveformGenerator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## waveform-generator.h: void ns3::WaveformGenerator::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_virtual=True)
    ## waveform-generator.h: void ns3::WaveformGenerator::SetDevice(ns3::Ptr<ns3::Object> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'd')], 
                   is_virtual=True)
    ## waveform-generator.h: void ns3::WaveformGenerator::SetDutyCycle(double value) [member function]
    cls.add_method('SetDutyCycle', 
                   'void', 
                   [param('double', 'value')])
    ## waveform-generator.h: void ns3::WaveformGenerator::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_virtual=True)
    ## waveform-generator.h: void ns3::WaveformGenerator::SetPeriod(ns3::Time period) [member function]
    cls.add_method('SetPeriod', 
                   'void', 
                   [param('ns3::Time', 'period')])
    ## waveform-generator.h: void ns3::WaveformGenerator::SetTxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue> txs) [member function]
    cls.add_method('SetTxPowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue >', 'txs')])
    ## waveform-generator.h: void ns3::WaveformGenerator::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## waveform-generator.h: void ns3::WaveformGenerator::StartRx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::SpectrumType st, ns3::Time duration) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## waveform-generator.h: void ns3::WaveformGenerator::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## waveform-generator.h: void ns3::WaveformGenerator::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## waveform-generator.h: void ns3::WaveformGenerator::GenerateWaveform() [member function]
    cls.add_method('GenerateWaveform', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AlohaNoackMacHeader_methods(root_module, cls):
    ## aloha-noack-mac-header.h: ns3::AlohaNoackMacHeader::AlohaNoackMacHeader() [constructor]
    cls.add_constructor([])
    ## aloha-noack-mac-header.h: ns3::AlohaNoackMacHeader::AlohaNoackMacHeader(ns3::AlohaNoackMacHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AlohaNoackMacHeader const &', 'arg0')])
    ## aloha-noack-mac-header.h: uint32_t ns3::AlohaNoackMacHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## aloha-noack-mac-header.h: ns3::Mac48Address ns3::AlohaNoackMacHeader::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## aloha-noack-mac-header.h: ns3::TypeId ns3::AlohaNoackMacHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-mac-header.h: uint32_t ns3::AlohaNoackMacHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-mac-header.h: ns3::Mac48Address ns3::AlohaNoackMacHeader::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## aloha-noack-mac-header.h: static ns3::TypeId ns3::AlohaNoackMacHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aloha-noack-mac-header.h: void ns3::AlohaNoackMacHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-mac-header.h: void ns3::AlohaNoackMacHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-mac-header.h: void ns3::AlohaNoackMacHeader::SetDestination(ns3::Mac48Address destination) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination')])
    ## aloha-noack-mac-header.h: void ns3::AlohaNoackMacHeader::SetSource(ns3::Mac48Address source) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Mac48Address', 'source')])
    return

def register_Ns3FriisSpectrumPropagationLossModel_methods(root_module, cls):
    ## friis-spectrum-propagation-loss.h: ns3::FriisSpectrumPropagationLossModel::FriisSpectrumPropagationLossModel(ns3::FriisSpectrumPropagationLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FriisSpectrumPropagationLossModel const &', 'arg0')])
    ## friis-spectrum-propagation-loss.h: ns3::FriisSpectrumPropagationLossModel::FriisSpectrumPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## friis-spectrum-propagation-loss.h: double ns3::FriisSpectrumPropagationLossModel::CalculateLoss(double f, double d) const [member function]
    cls.add_method('CalculateLoss', 
                   'double', 
                   [param('double', 'f'), param('double', 'd')], 
                   is_const=True)
    ## friis-spectrum-propagation-loss.h: ns3::Ptr<ns3::SpectrumValue> ns3::FriisSpectrumPropagationLossModel::DoCalcRxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> txPsd, ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'txPsd'), param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_const=True, is_virtual=True)
    ## friis-spectrum-propagation-loss.h: static ns3::TypeId ns3::FriisSpectrumPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3HalfDuplexIdealPhy_methods(root_module, cls):
    ## half-duplex-ideal-phy.h: ns3::HalfDuplexIdealPhy::HalfDuplexIdealPhy(ns3::HalfDuplexIdealPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::HalfDuplexIdealPhy const &', 'arg0')])
    ## half-duplex-ideal-phy.h: ns3::HalfDuplexIdealPhy::HalfDuplexIdealPhy() [constructor]
    cls.add_constructor([])
    ## half-duplex-ideal-phy.h: ns3::Ptr<ns3::Object> ns3::HalfDuplexIdealPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## half-duplex-ideal-phy.h: ns3::Ptr<ns3::Object> ns3::HalfDuplexIdealPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## half-duplex-ideal-phy.h: ns3::DataRate ns3::HalfDuplexIdealPhy::GetRate() const [member function]
    cls.add_method('GetRate', 
                   'ns3::DataRate', 
                   [], 
                   is_const=True)
    ## half-duplex-ideal-phy.h: ns3::Ptr<ns3::SpectrumModel const> ns3::HalfDuplexIdealPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## half-duplex-ideal-phy.h: ns3::SpectrumType ns3::HalfDuplexIdealPhy::GetSpectrumType() [member function]
    cls.add_method('GetSpectrumType', 
                   'ns3::SpectrumType', 
                   [])
    ## half-duplex-ideal-phy.h: static ns3::TypeId ns3::HalfDuplexIdealPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_virtual=True)
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetDevice(ns3::Ptr<ns3::Object> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'd')], 
                   is_virtual=True)
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetGenericPhyRxEndErrorCallback(ns3::GenericPhyRxEndErrorCallback c) [member function]
    cls.add_method('SetGenericPhyRxEndErrorCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxEndErrorCallback', 'c')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetGenericPhyRxEndOkCallback(ns3::GenericPhyRxEndOkCallback c) [member function]
    cls.add_method('SetGenericPhyRxEndOkCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxEndOkCallback', 'c')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetGenericPhyRxStartCallback(ns3::GenericPhyRxStartCallback c) [member function]
    cls.add_method('SetGenericPhyRxStartCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxStartCallback', 'c')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetGenericPhyTxEndCallback(ns3::GenericPhyTxEndCallback c) [member function]
    cls.add_method('SetGenericPhyTxEndCallback', 
                   'void', 
                   [param('ns3::GenericPhyTxEndCallback', 'c')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_virtual=True)
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetRate(ns3::DataRate rate) [member function]
    cls.add_method('SetRate', 
                   'void', 
                   [param('ns3::DataRate', 'rate')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetTxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue> txPsd) [member function]
    cls.add_method('SetTxPowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::StartRx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::SpectrumType st, ns3::Time duration) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## half-duplex-ideal-phy.h: bool ns3::HalfDuplexIdealPhy::StartTx(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('StartTx', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3NonCommunicatingNetDevice_methods(root_module, cls):
    ## non-communicating-net-device.h: ns3::NonCommunicatingNetDevice::NonCommunicatingNetDevice(ns3::NonCommunicatingNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NonCommunicatingNetDevice const &', 'arg0')])
    ## non-communicating-net-device.h: ns3::NonCommunicatingNetDevice::NonCommunicatingNetDevice() [constructor]
    cls.add_constructor([])
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: ns3::Address ns3::NonCommunicatingNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: ns3::Address ns3::NonCommunicatingNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: ns3::Ptr<ns3::Channel> ns3::NonCommunicatingNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: uint32_t ns3::NonCommunicatingNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: uint16_t ns3::NonCommunicatingNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: ns3::Address ns3::NonCommunicatingNetDevice::GetMulticast(ns3::Ipv4Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: ns3::Address ns3::NonCommunicatingNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: ns3::Ptr<ns3::Node> ns3::NonCommunicatingNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: ns3::Ptr<ns3::Object> ns3::NonCommunicatingNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## non-communicating-net-device.h: static ns3::TypeId ns3::NonCommunicatingNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::SetChannel(ns3::Ptr<ns3::Channel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::Channel >', 'c')])
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::SetPhy(ns3::Ptr<ns3::Object> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'phy')])
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## non-communicating-net-device.h: bool ns3::NonCommunicatingNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## non-communicating-net-device.h: void ns3::NonCommunicatingNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ShannonSpectrumErrorModel_methods(root_module, cls):
    ## spectrum-error-model.h: ns3::ShannonSpectrumErrorModel::ShannonSpectrumErrorModel() [constructor]
    cls.add_constructor([])
    ## spectrum-error-model.h: ns3::ShannonSpectrumErrorModel::ShannonSpectrumErrorModel(ns3::ShannonSpectrumErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ShannonSpectrumErrorModel const &', 'arg0')])
    ## spectrum-error-model.h: void ns3::ShannonSpectrumErrorModel::EvaluateChunk(ns3::SpectrumValue const & sinr, ns3::Time duration) [member function]
    cls.add_method('EvaluateChunk', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr'), param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## spectrum-error-model.h: bool ns3::ShannonSpectrumErrorModel::IsRxCorrect() [member function]
    cls.add_method('IsRxCorrect', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## spectrum-error-model.h: void ns3::ShannonSpectrumErrorModel::StartRx(ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_virtual=True)
    ## spectrum-error-model.h: void ns3::ShannonSpectrumErrorModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3SpectrumAnalyzer_methods(root_module, cls):
    ## spectrum-analyzer.h: ns3::SpectrumAnalyzer::SpectrumAnalyzer(ns3::SpectrumAnalyzer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumAnalyzer const &', 'arg0')])
    ## spectrum-analyzer.h: ns3::SpectrumAnalyzer::SpectrumAnalyzer() [constructor]
    cls.add_constructor([])
    ## spectrum-analyzer.h: ns3::Ptr<ns3::Object> ns3::SpectrumAnalyzer::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## spectrum-analyzer.h: ns3::Ptr<ns3::Object> ns3::SpectrumAnalyzer::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## spectrum-analyzer.h: ns3::Ptr<ns3::SpectrumModel const> ns3::SpectrumAnalyzer::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## spectrum-analyzer.h: static ns3::TypeId ns3::SpectrumAnalyzer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_virtual=True)
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::SetDevice(ns3::Ptr<ns3::Object> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'd')], 
                   is_virtual=True)
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_virtual=True)
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::SetRxSpectrumModel(ns3::Ptr<ns3::SpectrumModel> m) [member function]
    cls.add_method('SetRxSpectrumModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumModel >', 'm')])
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::StartRx(ns3::Ptr<ns3::PacketBurst> pb, ns3::Ptr<ns3::SpectrumValue const> rxPowerSpectralDensity, ns3::SpectrumType st, ns3::Time duration) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPowerSpectralDensity'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## spectrum-analyzer.h: void ns3::SpectrumAnalyzer::GenerateReport() [member function]
    cls.add_method('GenerateReport', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3SpectrumChannel_methods(root_module, cls):
    ## spectrum-channel.h: ns3::SpectrumChannel::SpectrumChannel() [constructor]
    cls.add_constructor([])
    ## spectrum-channel.h: ns3::SpectrumChannel::SpectrumChannel(ns3::SpectrumChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumChannel const &', 'arg0')])
    ## spectrum-channel.h: void ns3::SpectrumChannel::AddRx(ns3::Ptr<ns3::SpectrumPhy> phy) [member function]
    cls.add_method('AddRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h: void ns3::SpectrumChannel::AddSpectrumPropagationLossModel(ns3::Ptr<ns3::SpectrumPropagationLossModel> loss) [member function]
    cls.add_method('AddSpectrumPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPropagationLossModel >', 'loss')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h: static ns3::TypeId ns3::SpectrumChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-channel.h: void ns3::SpectrumChannel::SetPropagationDelayModel(ns3::Ptr<ns3::PropagationDelayModel> delay) [member function]
    cls.add_method('SetPropagationDelayModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationDelayModel >', 'delay')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h: void ns3::SpectrumChannel::StartTx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> txPsd, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> sender) [member function]
    cls.add_method('StartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'sender')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3AlohaNoackNetDevice_methods(root_module, cls):
    ## aloha-noack-net-device.h: ns3::AlohaNoackNetDevice::AlohaNoackNetDevice(ns3::AlohaNoackNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AlohaNoackNetDevice const &', 'arg0')])
    ## aloha-noack-net-device.h: ns3::AlohaNoackNetDevice::AlohaNoackNetDevice() [constructor]
    cls.add_constructor([])
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: ns3::Address ns3::AlohaNoackNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: ns3::Address ns3::AlohaNoackNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: ns3::Ptr<ns3::Channel> ns3::AlohaNoackNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: uint32_t ns3::AlohaNoackNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: uint16_t ns3::AlohaNoackNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: ns3::Address ns3::AlohaNoackNetDevice::GetMulticast(ns3::Ipv4Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: ns3::Address ns3::AlohaNoackNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: ns3::Ptr<ns3::Node> ns3::AlohaNoackNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: ns3::Ptr<ns3::Object> ns3::AlohaNoackNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## aloha-noack-net-device.h: static ns3::TypeId ns3::AlohaNoackNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::NotifyReceptionEndError() [member function]
    cls.add_method('NotifyReceptionEndError', 
                   'void', 
                   [])
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::NotifyReceptionEndOk(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('NotifyReceptionEndOk', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::NotifyReceptionStart() [member function]
    cls.add_method('NotifyReceptionStart', 
                   'void', 
                   [])
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::NotifyTransmissionEnd(ns3::Ptr<ns3::Packet const> arg0) [member function]
    cls.add_method('NotifyTransmissionEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'arg0')])
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetChannel(ns3::Ptr<ns3::Channel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::Channel >', 'c')])
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetGenericPhyTxStartCallback(ns3::GenericPhyTxStartCallback c) [member function]
    cls.add_method('SetGenericPhyTxStartCallback', 
                   'void', 
                   [param('ns3::GenericPhyTxStartCallback', 'c')])
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetPhy(ns3::Ptr<ns3::Object> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'phy')])
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetQueue(ns3::Ptr<ns3::Queue> queue) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Queue >', 'queue')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## aloha-noack-net-device.h: bool ns3::AlohaNoackNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3MultiModelSpectrumChannel_methods(root_module, cls):
    ## multi-model-spectrum-channel.h: ns3::MultiModelSpectrumChannel::MultiModelSpectrumChannel(ns3::MultiModelSpectrumChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MultiModelSpectrumChannel const &', 'arg0')])
    ## multi-model-spectrum-channel.h: ns3::MultiModelSpectrumChannel::MultiModelSpectrumChannel() [constructor]
    cls.add_constructor([])
    ## multi-model-spectrum-channel.h: void ns3::MultiModelSpectrumChannel::AddRx(ns3::Ptr<ns3::SpectrumPhy> phy) [member function]
    cls.add_method('AddRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPhy >', 'phy')], 
                   is_virtual=True)
    ## multi-model-spectrum-channel.h: void ns3::MultiModelSpectrumChannel::AddSpectrumPropagationLossModel(ns3::Ptr<ns3::SpectrumPropagationLossModel> loss) [member function]
    cls.add_method('AddSpectrumPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPropagationLossModel >', 'loss')], 
                   is_virtual=True)
    ## multi-model-spectrum-channel.h: ns3::Ptr<ns3::NetDevice> ns3::MultiModelSpectrumChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## multi-model-spectrum-channel.h: uint32_t ns3::MultiModelSpectrumChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## multi-model-spectrum-channel.h: ns3::Ptr<ns3::SpectrumPropagationLossModel> ns3::MultiModelSpectrumChannel::GetSpectrumPropagationLossModel() [member function]
    cls.add_method('GetSpectrumPropagationLossModel', 
                   'ns3::Ptr< ns3::SpectrumPropagationLossModel >', 
                   [], 
                   is_virtual=True)
    ## multi-model-spectrum-channel.h: static ns3::TypeId ns3::MultiModelSpectrumChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## multi-model-spectrum-channel.h: void ns3::MultiModelSpectrumChannel::SetPropagationDelayModel(ns3::Ptr<ns3::PropagationDelayModel> delay) [member function]
    cls.add_method('SetPropagationDelayModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationDelayModel >', 'delay')], 
                   is_virtual=True)
    ## multi-model-spectrum-channel.h: void ns3::MultiModelSpectrumChannel::StartTx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> txPsd, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> sender) [member function]
    cls.add_method('StartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'sender')], 
                   is_virtual=True)
    ## multi-model-spectrum-channel.h: void ns3::MultiModelSpectrumChannel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## multi-model-spectrum-channel.h: void ns3::MultiModelSpectrumChannel::StartRx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> rxPowerSpectrum, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> receiver) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'rxPowerSpectrum'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'receiver')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3SingleModelSpectrumChannel_methods(root_module, cls):
    ## single-model-spectrum-channel.h: ns3::SingleModelSpectrumChannel::SingleModelSpectrumChannel(ns3::SingleModelSpectrumChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SingleModelSpectrumChannel const &', 'arg0')])
    ## single-model-spectrum-channel.h: ns3::SingleModelSpectrumChannel::SingleModelSpectrumChannel() [constructor]
    cls.add_constructor([])
    ## single-model-spectrum-channel.h: void ns3::SingleModelSpectrumChannel::AddRx(ns3::Ptr<ns3::SpectrumPhy> phy) [member function]
    cls.add_method('AddRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPhy >', 'phy')], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h: void ns3::SingleModelSpectrumChannel::AddSpectrumPropagationLossModel(ns3::Ptr<ns3::SpectrumPropagationLossModel> loss) [member function]
    cls.add_method('AddSpectrumPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPropagationLossModel >', 'loss')], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h: ns3::Ptr<ns3::NetDevice> ns3::SingleModelSpectrumChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## single-model-spectrum-channel.h: uint32_t ns3::SingleModelSpectrumChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## single-model-spectrum-channel.h: ns3::Ptr<ns3::SpectrumPropagationLossModel> ns3::SingleModelSpectrumChannel::GetSpectrumPropagationLossModel() [member function]
    cls.add_method('GetSpectrumPropagationLossModel', 
                   'ns3::Ptr< ns3::SpectrumPropagationLossModel >', 
                   [], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h: static ns3::TypeId ns3::SingleModelSpectrumChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## single-model-spectrum-channel.h: void ns3::SingleModelSpectrumChannel::SetPropagationDelayModel(ns3::Ptr<ns3::PropagationDelayModel> delay) [member function]
    cls.add_method('SetPropagationDelayModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationDelayModel >', 'delay')], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h: void ns3::SingleModelSpectrumChannel::StartTx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> txPsd, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> sender) [member function]
    cls.add_method('StartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'sender')], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h: void ns3::SingleModelSpectrumChannel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## single-model-spectrum-channel.h: void ns3::SingleModelSpectrumChannel::StartRx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> rxPowerSpectrum, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> receiver) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'rxPowerSpectrum'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'receiver')], 
                   visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    ## spectrum-value.h: extern ns3::SpectrumValue ns3::Log(ns3::SpectrumValue const & arg) [free function]
    module.add_function('Log', 
                        'ns3::SpectrumValue', 
                        [param('ns3::SpectrumValue const &', 'arg')])
    ## spectrum-value.h: extern ns3::SpectrumValue ns3::Log10(ns3::SpectrumValue const & arg) [free function]
    module.add_function('Log10', 
                        'ns3::SpectrumValue', 
                        [param('ns3::SpectrumValue const &', 'arg')])
    ## spectrum-value.h: extern ns3::SpectrumValue ns3::Log2(ns3::SpectrumValue const & arg) [free function]
    module.add_function('Log2', 
                        'ns3::SpectrumValue', 
                        [param('ns3::SpectrumValue const &', 'arg')])
    ## spectrum-value.h: extern double ns3::Norm(ns3::SpectrumValue const & x) [free function]
    module.add_function('Norm', 
                        'double', 
                        [param('ns3::SpectrumValue const &', 'x')])
    ## spectrum-value.h: extern ns3::SpectrumValue ns3::Pow(ns3::SpectrumValue const & base, double exp) [free function]
    module.add_function('Pow', 
                        'ns3::SpectrumValue', 
                        [param('ns3::SpectrumValue const &', 'base'), param('double', 'exp')])
    ## spectrum-value.h: extern ns3::SpectrumValue ns3::Pow(double base, ns3::SpectrumValue const & exp) [free function]
    module.add_function('Pow', 
                        'ns3::SpectrumValue', 
                        [param('double', 'base'), param('ns3::SpectrumValue const &', 'exp')])
    ## spectrum-value.h: extern double ns3::Prod(ns3::SpectrumValue const & x) [free function]
    module.add_function('Prod', 
                        'double', 
                        [param('ns3::SpectrumValue const &', 'x')])
    ## spectrum-value.h: extern double ns3::Sum(ns3::SpectrumValue const & x) [free function]
    module.add_function('Sum', 
                        'double', 
                        [param('ns3::SpectrumValue const &', 'x')])
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

