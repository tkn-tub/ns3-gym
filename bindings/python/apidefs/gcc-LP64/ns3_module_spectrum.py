from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## microwave-oven-spectrum-value-helper.h: ns3::MicrowaveOvenSpectrumValueHelper [class]
    module.add_class('MicrowaveOvenSpectrumValueHelper')
    ## multi-model-spectrum-channel.h: ns3::RxSpectrumModelInfo [class]
    module.add_class('RxSpectrumModelInfo')
    ## multi-model-spectrum-channel.h: ns3::TxSpectrumModelInfo [class]
    module.add_class('TxSpectrumModelInfo')
    ## wifi-spectrum-value-helper.h: ns3::WifiSpectrumValue5MhzFactory [class]
    module.add_class('WifiSpectrumValue5MhzFactory', allow_subclassing=True)
    ## wifi-spectrum-value-helper.h: ns3::WifiSpectrumValueHelper [class]
    module.add_class('WifiSpectrumValueHelper', allow_subclassing=True)
    ## spectrum-error-model.h: ns3::SpectrumErrorModel [class]
    module.add_class('SpectrumErrorModel', parent=root_module['ns3::Object'])
    ## spectrum-interference.h: ns3::SpectrumInterference [class]
    module.add_class('SpectrumInterference', parent=root_module['ns3::Object'])
    ## waveform-generator.h: ns3::WaveformGenerator [class]
    module.add_class('WaveformGenerator', parent=root_module['ns3::SpectrumPhy'])
    ## aloha-noack-mac-header.h: ns3::AlohaNoackMacHeader [class]
    module.add_class('AlohaNoackMacHeader', parent=root_module['ns3::Header'])
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
    ## aloha-noack-net-device.h: ns3::AlohaNoackNetDevice [class]
    module.add_class('AlohaNoackNetDevice', parent=root_module['ns3::NetDevice'])
    ## aloha-noack-net-device.h: ns3::AlohaNoackNetDevice::State [enumeration]
    module.add_enum('State', ['IDLE', 'TX', 'RX'], outer_class=root_module['ns3::AlohaNoackNetDevice'])
    ## multi-model-spectrum-channel.h: ns3::MultiModelSpectrumChannel [class]
    module.add_class('MultiModelSpectrumChannel', parent=root_module['ns3::SpectrumChannel'])
    ## single-model-spectrum-channel.h: ns3::SingleModelSpectrumChannel [class]
    module.add_class('SingleModelSpectrumChannel', parent=root_module['ns3::SpectrumChannel'])
    typehandlers.add_type_alias('std::map< unsigned int, ns3::SpectrumConverter, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::SpectrumConverter > > >', 'ns3::SpectrumConverterMap_t')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::SpectrumConverter, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::SpectrumConverter > > >*', 'ns3::SpectrumConverterMap_t*')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::SpectrumConverter, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::SpectrumConverter > > >&', 'ns3::SpectrumConverterMap_t&')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::TxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::TxSpectrumModelInfo > > >', 'ns3::TxSpectrumModelInfoMap_t')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::TxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::TxSpectrumModelInfo > > >*', 'ns3::TxSpectrumModelInfoMap_t*')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::TxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::TxSpectrumModelInfo > > >&', 'ns3::TxSpectrumModelInfoMap_t&')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::RxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::RxSpectrumModelInfo > > >', 'ns3::RxSpectrumModelInfoMap_t')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::RxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::RxSpectrumModelInfo > > >*', 'ns3::RxSpectrumModelInfoMap_t*')
    typehandlers.add_type_alias('std::map< unsigned int, ns3::RxSpectrumModelInfo, std::less< unsigned int >, std::allocator< std::pair< unsigned int const, ns3::RxSpectrumModelInfo > > >&', 'ns3::RxSpectrumModelInfoMap_t&')
    
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
    register_Ns3MicrowaveOvenSpectrumValueHelper_methods(root_module, root_module['ns3::MicrowaveOvenSpectrumValueHelper'])
    register_Ns3RxSpectrumModelInfo_methods(root_module, root_module['ns3::RxSpectrumModelInfo'])
    register_Ns3TxSpectrumModelInfo_methods(root_module, root_module['ns3::TxSpectrumModelInfo'])
    register_Ns3WifiSpectrumValue5MhzFactory_methods(root_module, root_module['ns3::WifiSpectrumValue5MhzFactory'])
    register_Ns3WifiSpectrumValueHelper_methods(root_module, root_module['ns3::WifiSpectrumValueHelper'])
    register_Ns3SpectrumErrorModel_methods(root_module, root_module['ns3::SpectrumErrorModel'])
    register_Ns3SpectrumInterference_methods(root_module, root_module['ns3::SpectrumInterference'])
    register_Ns3WaveformGenerator_methods(root_module, root_module['ns3::WaveformGenerator'])
    register_Ns3AlohaNoackMacHeader_methods(root_module, root_module['ns3::AlohaNoackMacHeader'])
    register_Ns3HalfDuplexIdealPhy_methods(root_module, root_module['ns3::HalfDuplexIdealPhy'])
    register_Ns3NonCommunicatingNetDevice_methods(root_module, root_module['ns3::NonCommunicatingNetDevice'])
    register_Ns3ShannonSpectrumErrorModel_methods(root_module, root_module['ns3::ShannonSpectrumErrorModel'])
    register_Ns3SpectrumAnalyzer_methods(root_module, root_module['ns3::SpectrumAnalyzer'])
    register_Ns3AlohaNoackNetDevice_methods(root_module, root_module['ns3::AlohaNoackNetDevice'])
    register_Ns3MultiModelSpectrumChannel_methods(root_module, root_module['ns3::MultiModelSpectrumChannel'])
    register_Ns3SingleModelSpectrumChannel_methods(root_module, root_module['ns3::SingleModelSpectrumChannel'])
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
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_virtual=True)
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetPhyMacRxEndErrorCallback(ns3::PhyMacRxEndErrorCallback c) [member function]
    cls.add_method('SetPhyMacRxEndErrorCallback', 
                   'void', 
                   [param('ns3::PhyMacRxEndErrorCallback', 'c')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetPhyMacRxEndOkCallback(ns3::PhyMacRxEndOkCallback c) [member function]
    cls.add_method('SetPhyMacRxEndOkCallback', 
                   'void', 
                   [param('ns3::PhyMacRxEndOkCallback', 'c')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetPhyMacRxStartCallback(ns3::PhyMacRxStartCallback c) [member function]
    cls.add_method('SetPhyMacRxStartCallback', 
                   'void', 
                   [param('ns3::PhyMacRxStartCallback', 'c')])
    ## half-duplex-ideal-phy.h: void ns3::HalfDuplexIdealPhy::SetPhyMacTxEndCallback(ns3::PhyMacTxEndCallback c) [member function]
    cls.add_method('SetPhyMacTxEndCallback', 
                   'void', 
                   [param('ns3::PhyMacTxEndCallback', 'c')])
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
    ## aloha-noack-net-device.h: void ns3::AlohaNoackNetDevice::SetPhyMacTxStartCallback(ns3::PhyMacTxStartCallback c) [member function]
    cls.add_method('SetPhyMacTxStartCallback', 
                   'void', 
                   [param('ns3::PhyMacTxStartCallback', 'c')])
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

