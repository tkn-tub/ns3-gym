from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## wifi-preamble.h: ns3::WifiPreamble [enumeration]
    module.add_enum('WifiPreamble', ['WIFI_PREAMBLE_LONG', 'WIFI_PREAMBLE_SHORT'])
    ## wifi-phy-standard.h: ns3::WifiPhyStandard [enumeration]
    module.add_enum('WifiPhyStandard', ['WIFI_PHY_STANDARD_80211a', 'WIFI_PHY_STANDARD_holland'])
    ## interference-helper.h: ns3::InterferenceHelper [class]
    module.add_class('InterferenceHelper', allow_subclassing=False)
    ## interference-helper.h: ns3::InterferenceHelper::SnrPer [struct]
    module.add_class('SnrPer', outer_class=root_module['ns3::InterferenceHelper'])
    ## ssid.h: ns3::Ssid [class]
    module.add_class('Ssid')
    ## supported-rates.h: ns3::SupportedRates [class]
    module.add_class('SupportedRates')
    ## rraa-wifi-manager.h: ns3::ThresholdsItem [struct]
    module.add_class('ThresholdsItem')
    ## wifi-mode.h: ns3::WifiMode [class]
    module.add_class('WifiMode')
    ## wifi-mode.h: ns3::WifiMode::ModulationType [enumeration]
    module.add_enum('ModulationType', ['BPSK', 'QAM'], outer_class=root_module['ns3::WifiMode'])
    ## wifi-mode.h: ns3::WifiModeFactory [class]
    module.add_class('WifiModeFactory')
    ## wifi-phy.h: ns3::WifiPhyListener [class]
    module.add_class('WifiPhyListener', allow_subclassing=True)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation [class]
    module.add_class('WifiRemoteStation', allow_subclassing=True)
    ## amrr-wifi-manager.h: ns3::AmrrWifiRemoteStation [class]
    module.add_class('AmrrWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## arf-wifi-manager.h: ns3::ArfWifiRemoteStation [class]
    module.add_class('ArfWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiRemoteStation [class]
    module.add_class('ConstantRateWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## ideal-wifi-manager.h: ns3::IdealWifiRemoteStation [class]
    module.add_class('IdealWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## onoe-wifi-manager.h: ns3::OnoeWifiRemoteStation [class]
    module.add_class('OnoeWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## propagation-delay-model.h: ns3::PropagationDelayModel [class]
    module.add_class('PropagationDelayModel', parent=root_module['ns3::Object'])
    ## propagation-loss-model.h: ns3::PropagationLossModel [class]
    module.add_class('PropagationLossModel', parent=root_module['ns3::Object'])
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel [class]
    module.add_class('RandomPropagationDelayModel', parent=root_module['ns3::PropagationDelayModel'])
    ## propagation-loss-model.h: ns3::RandomPropagationLossModel [class]
    module.add_class('RandomPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## rraa-wifi-manager.h: ns3::RraaWifiRemoteStation [class]
    module.add_class('RraaWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## ssid.h: ns3::SsidChecker [class]
    module.add_class('SsidChecker', parent=root_module['ns3::AttributeChecker'])
    ## ssid.h: ns3::SsidValue [class]
    module.add_class('SsidValue', parent=root_module['ns3::AttributeValue'])
    ## wifi-mac.h: ns3::WifiMac [class]
    module.add_class('WifiMac', parent=root_module['ns3::Object'])
    ## wifi-mode.h: ns3::WifiModeChecker [class]
    module.add_class('WifiModeChecker', parent=root_module['ns3::AttributeChecker'])
    ## wifi-mode.h: ns3::WifiModeValue [class]
    module.add_class('WifiModeValue', parent=root_module['ns3::AttributeValue'])
    ## wifi-phy.h: ns3::WifiPhy [class]
    module.add_class('WifiPhy', parent=root_module['ns3::Object'])
    ## wifi-phy.h: ns3::WifiPhy::State [enumeration]
    module.add_enum('State', ['SYNC', 'TX', 'CCA_BUSY', 'IDLE'], outer_class=root_module['ns3::WifiPhy'])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationManager [class]
    module.add_class('WifiRemoteStationManager', parent=root_module['ns3::Object'])
    ## yans-wifi-phy.h: ns3::YansWifiPhy [class]
    module.add_class('YansWifiPhy', parent=root_module['ns3::WifiPhy'])
    ## aarf-wifi-manager.h: ns3::AarfWifiRemoteStation [class]
    module.add_class('AarfWifiRemoteStation', parent=root_module['ns3::ArfWifiRemoteStation'])
    ## adhoc-wifi-mac.h: ns3::AdhocWifiMac [class]
    module.add_class('AdhocWifiMac', parent=root_module['ns3::WifiMac'])
    ## amrr-wifi-manager.h: ns3::AmrrWifiManager [class]
    module.add_class('AmrrWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## arf-wifi-manager.h: ns3::ArfWifiManager [class]
    module.add_class('ArfWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiManager [class]
    module.add_class('ConstantRateWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel [class]
    module.add_class('ConstantSpeedPropagationDelayModel', parent=root_module['ns3::PropagationDelayModel'])
    ## error-rate-model.h: ns3::ErrorRateModel [class]
    module.add_class('ErrorRateModel', parent=root_module['ns3::Object'])
    ## propagation-loss-model.h: ns3::FriisPropagationLossModel [class]
    module.add_class('FriisPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## ideal-wifi-manager.h: ns3::IdealWifiManager [class]
    module.add_class('IdealWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## jakes-propagation-loss-model.h: ns3::JakesPropagationLossModel [class]
    module.add_class('JakesPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::LogDistancePropagationLossModel [class]
    module.add_class('LogDistancePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## nqap-wifi-mac.h: ns3::NqapWifiMac [class]
    module.add_class('NqapWifiMac', parent=root_module['ns3::WifiMac'])
    ## nqsta-wifi-mac.h: ns3::NqstaWifiMac [class]
    module.add_class('NqstaWifiMac', parent=root_module['ns3::WifiMac'])
    ## onoe-wifi-manager.h: ns3::OnoeWifiManager [class]
    module.add_class('OnoeWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## rraa-wifi-manager.h: ns3::RraaWifiManager [class]
    module.add_class('RraaWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## wifi-channel.h: ns3::WifiChannel [class]
    module.add_class('WifiChannel', parent=root_module['ns3::Channel'])
    ## wifi-net-device.h: ns3::WifiNetDevice [class]
    module.add_class('WifiNetDevice', parent=root_module['ns3::NetDevice'])
    ## yans-error-rate-model.h: ns3::YansErrorRateModel [class]
    module.add_class('YansErrorRateModel', parent=root_module['ns3::ErrorRateModel'])
    ## yans-wifi-channel.h: ns3::YansWifiChannel [class]
    module.add_class('YansWifiChannel', parent=root_module['ns3::WifiChannel'])
    ## aarf-wifi-manager.h: ns3::AarfWifiManager [class]
    module.add_class('AarfWifiManager', parent=root_module['ns3::ArfWifiManager'])
    
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
    register_Ns3InterferenceHelper_methods(root_module, root_module['ns3::InterferenceHelper'])
    register_Ns3InterferenceHelperSnrPer_methods(root_module, root_module['ns3::InterferenceHelper::SnrPer'])
    register_Ns3Ssid_methods(root_module, root_module['ns3::Ssid'])
    register_Ns3SupportedRates_methods(root_module, root_module['ns3::SupportedRates'])
    register_Ns3ThresholdsItem_methods(root_module, root_module['ns3::ThresholdsItem'])
    register_Ns3WifiMode_methods(root_module, root_module['ns3::WifiMode'])
    register_Ns3WifiModeFactory_methods(root_module, root_module['ns3::WifiModeFactory'])
    register_Ns3WifiPhyListener_methods(root_module, root_module['ns3::WifiPhyListener'])
    register_Ns3WifiRemoteStation_methods(root_module, root_module['ns3::WifiRemoteStation'])
    register_Ns3AmrrWifiRemoteStation_methods(root_module, root_module['ns3::AmrrWifiRemoteStation'])
    register_Ns3ArfWifiRemoteStation_methods(root_module, root_module['ns3::ArfWifiRemoteStation'])
    register_Ns3ConstantRateWifiRemoteStation_methods(root_module, root_module['ns3::ConstantRateWifiRemoteStation'])
    register_Ns3IdealWifiRemoteStation_methods(root_module, root_module['ns3::IdealWifiRemoteStation'])
    register_Ns3OnoeWifiRemoteStation_methods(root_module, root_module['ns3::OnoeWifiRemoteStation'])
    register_Ns3PropagationDelayModel_methods(root_module, root_module['ns3::PropagationDelayModel'])
    register_Ns3PropagationLossModel_methods(root_module, root_module['ns3::PropagationLossModel'])
    register_Ns3RandomPropagationDelayModel_methods(root_module, root_module['ns3::RandomPropagationDelayModel'])
    register_Ns3RandomPropagationLossModel_methods(root_module, root_module['ns3::RandomPropagationLossModel'])
    register_Ns3RraaWifiRemoteStation_methods(root_module, root_module['ns3::RraaWifiRemoteStation'])
    register_Ns3SsidChecker_methods(root_module, root_module['ns3::SsidChecker'])
    register_Ns3SsidValue_methods(root_module, root_module['ns3::SsidValue'])
    register_Ns3WifiMac_methods(root_module, root_module['ns3::WifiMac'])
    register_Ns3WifiModeChecker_methods(root_module, root_module['ns3::WifiModeChecker'])
    register_Ns3WifiModeValue_methods(root_module, root_module['ns3::WifiModeValue'])
    register_Ns3WifiPhy_methods(root_module, root_module['ns3::WifiPhy'])
    register_Ns3WifiRemoteStationManager_methods(root_module, root_module['ns3::WifiRemoteStationManager'])
    register_Ns3YansWifiPhy_methods(root_module, root_module['ns3::YansWifiPhy'])
    register_Ns3AarfWifiRemoteStation_methods(root_module, root_module['ns3::AarfWifiRemoteStation'])
    register_Ns3AdhocWifiMac_methods(root_module, root_module['ns3::AdhocWifiMac'])
    register_Ns3AmrrWifiManager_methods(root_module, root_module['ns3::AmrrWifiManager'])
    register_Ns3ArfWifiManager_methods(root_module, root_module['ns3::ArfWifiManager'])
    register_Ns3ConstantRateWifiManager_methods(root_module, root_module['ns3::ConstantRateWifiManager'])
    register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, root_module['ns3::ConstantSpeedPropagationDelayModel'])
    register_Ns3ErrorRateModel_methods(root_module, root_module['ns3::ErrorRateModel'])
    register_Ns3FriisPropagationLossModel_methods(root_module, root_module['ns3::FriisPropagationLossModel'])
    register_Ns3IdealWifiManager_methods(root_module, root_module['ns3::IdealWifiManager'])
    register_Ns3JakesPropagationLossModel_methods(root_module, root_module['ns3::JakesPropagationLossModel'])
    register_Ns3LogDistancePropagationLossModel_methods(root_module, root_module['ns3::LogDistancePropagationLossModel'])
    register_Ns3NqapWifiMac_methods(root_module, root_module['ns3::NqapWifiMac'])
    register_Ns3NqstaWifiMac_methods(root_module, root_module['ns3::NqstaWifiMac'])
    register_Ns3OnoeWifiManager_methods(root_module, root_module['ns3::OnoeWifiManager'])
    register_Ns3RraaWifiManager_methods(root_module, root_module['ns3::RraaWifiManager'])
    register_Ns3WifiChannel_methods(root_module, root_module['ns3::WifiChannel'])
    register_Ns3WifiNetDevice_methods(root_module, root_module['ns3::WifiNetDevice'])
    register_Ns3YansErrorRateModel_methods(root_module, root_module['ns3::YansErrorRateModel'])
    register_Ns3YansWifiChannel_methods(root_module, root_module['ns3::YansWifiChannel'])
    register_Ns3AarfWifiManager_methods(root_module, root_module['ns3::AarfWifiManager'])
    return

def register_Ns3InterferenceHelper_methods(root_module, cls):
    ## interference-helper.h: ns3::InterferenceHelper::InterferenceHelper() [constructor]
    cls.add_constructor([])
    ## interference-helper.h: ns3::Ptr<ns3::InterferenceHelper::Event> ns3::InterferenceHelper::Add(uint32_t size, ns3::WifiMode payloadMode, ns3::WifiPreamble preamble, ns3::Time duration, double rxPower) [member function]
    cls.add_method('Add', 
                   'ns3::Ptr< ns3::InterferenceHelper::Event >', 
                   [param('uint32_t', 'size'), param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble'), param('ns3::Time', 'duration'), param('double', 'rxPower')])
    ## interference-helper.h: ns3::InterferenceHelper::SnrPer ns3::InterferenceHelper::CalculateSnrPer(ns3::Ptr<ns3::InterferenceHelper::Event> event) [member function]
    cls.add_method('CalculateSnrPer', 
                   'ns3::InterferenceHelper::SnrPer', 
                   [param('ns3::Ptr< ns3::InterferenceHelper::Event >', 'event')])
    ## interference-helper.h: ns3::Time ns3::InterferenceHelper::CalculateTxDuration(uint32_t size, ns3::WifiMode payloadMode, ns3::WifiPreamble preamble) const [member function]
    cls.add_method('CalculateTxDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_const=True)
    ## interference-helper.h: void ns3::InterferenceHelper::Configure80211aParameters() [member function]
    cls.add_method('Configure80211aParameters', 
                   'void', 
                   [])
    ## interference-helper.h: ns3::Time ns3::InterferenceHelper::GetEnergyDuration(double energyW) [member function]
    cls.add_method('GetEnergyDuration', 
                   'ns3::Time', 
                   [param('double', 'energyW')])
    ## interference-helper.h: ns3::Ptr<ns3::ErrorRateModel> ns3::InterferenceHelper::GetErrorRateModel() const [member function]
    cls.add_method('GetErrorRateModel', 
                   'ns3::Ptr< ns3::ErrorRateModel >', 
                   [], 
                   is_const=True)
    ## interference-helper.h: double ns3::InterferenceHelper::GetNoiseFloorW() const [member function]
    cls.add_method('GetNoiseFloorW', 
                   'double', 
                   [], 
                   is_const=True)
    ## interference-helper.h: void ns3::InterferenceHelper::SetErrorRateModel(ns3::Ptr<ns3::ErrorRateModel> rate) [member function]
    cls.add_method('SetErrorRateModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorRateModel >', 'rate')])
    ## interference-helper.h: void ns3::InterferenceHelper::SetNoiseFloorW(double noiseFloor) [member function]
    cls.add_method('SetNoiseFloorW', 
                   'void', 
                   [param('double', 'noiseFloor')])
    return

def register_Ns3InterferenceHelperSnrPer_methods(root_module, cls):
    ## interference-helper.h: ns3::InterferenceHelper::SnrPer::SnrPer() [constructor]
    cls.add_constructor([])
    ## interference-helper.h: ns3::InterferenceHelper::SnrPer::SnrPer(ns3::InterferenceHelper::SnrPer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::InterferenceHelper::SnrPer const &', 'arg0')])
    ## interference-helper.h: ns3::InterferenceHelper::SnrPer::per [variable]
    cls.add_instance_attribute('per', 'double', is_const=False)
    ## interference-helper.h: ns3::InterferenceHelper::SnrPer::snr [variable]
    cls.add_instance_attribute('snr', 'double', is_const=False)
    return

def register_Ns3Ssid_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ssid.h: ns3::Ssid::Ssid(ns3::Ssid const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ssid const &', 'arg0')])
    ## ssid.h: ns3::Ssid::Ssid() [constructor]
    cls.add_constructor([])
    ## ssid.h: ns3::Ssid::Ssid(std::string s) [constructor]
    cls.add_constructor([param('std::string', 's')])
    ## ssid.h: ns3::Ssid::Ssid(char const * ssid, uint8_t length) [constructor]
    cls.add_constructor([param('char const *', 'ssid'), param('uint8_t', 'length')])
    ## ssid.h: ns3::Buffer::Iterator ns3::Ssid::Deserialize(ns3::Buffer::Iterator i) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')])
    ## ssid.h: uint32_t ns3::Ssid::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ssid.h: uint32_t ns3::Ssid::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ssid.h: bool ns3::Ssid::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ssid.h: bool ns3::Ssid::IsEqual(ns3::Ssid const & o) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ssid const &', 'o')], 
                   is_const=True)
    ## ssid.h: char * ns3::Ssid::PeekString() const [member function]
    cls.add_method('PeekString', 
                   'char *', 
                   [], 
                   is_const=True)
    ## ssid.h: ns3::Buffer::Iterator ns3::Ssid::Serialize(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True)
    return

def register_Ns3SupportedRates_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## supported-rates.h: ns3::SupportedRates::SupportedRates(ns3::SupportedRates const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SupportedRates const &', 'arg0')])
    ## supported-rates.h: ns3::SupportedRates::SupportedRates() [constructor]
    cls.add_constructor([])
    ## supported-rates.h: void ns3::SupportedRates::AddSupportedRate(uint32_t bs) [member function]
    cls.add_method('AddSupportedRate', 
                   'void', 
                   [param('uint32_t', 'bs')])
    ## supported-rates.h: ns3::Buffer::Iterator ns3::SupportedRates::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## supported-rates.h: uint8_t ns3::SupportedRates::GetNRates() const [member function]
    cls.add_method('GetNRates', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## supported-rates.h: uint32_t ns3::SupportedRates::GetRate(uint8_t i) const [member function]
    cls.add_method('GetRate', 
                   'uint32_t', 
                   [param('uint8_t', 'i')], 
                   is_const=True)
    ## supported-rates.h: uint32_t ns3::SupportedRates::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## supported-rates.h: bool ns3::SupportedRates::IsBasicRate(uint32_t bs) const [member function]
    cls.add_method('IsBasicRate', 
                   'bool', 
                   [param('uint32_t', 'bs')], 
                   is_const=True)
    ## supported-rates.h: bool ns3::SupportedRates::IsSupportedRate(uint32_t bs) const [member function]
    cls.add_method('IsSupportedRate', 
                   'bool', 
                   [param('uint32_t', 'bs')], 
                   is_const=True)
    ## supported-rates.h: ns3::Buffer::Iterator ns3::SupportedRates::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## supported-rates.h: void ns3::SupportedRates::SetBasicRate(uint32_t bs) [member function]
    cls.add_method('SetBasicRate', 
                   'void', 
                   [param('uint32_t', 'bs')])
    return

def register_Ns3ThresholdsItem_methods(root_module, cls):
    ## rraa-wifi-manager.h: ns3::ThresholdsItem::datarate [variable]
    cls.add_instance_attribute('datarate', 'uint32_t', is_const=False)
    ## rraa-wifi-manager.h: ns3::ThresholdsItem::pori [variable]
    cls.add_instance_attribute('pori', 'double', is_const=False)
    ## rraa-wifi-manager.h: ns3::ThresholdsItem::pmtl [variable]
    cls.add_instance_attribute('pmtl', 'double', is_const=False)
    ## rraa-wifi-manager.h: ns3::ThresholdsItem::ewnd [variable]
    cls.add_instance_attribute('ewnd', 'uint32_t', is_const=False)
    ## rraa-wifi-manager.h: ns3::ThresholdsItem::ThresholdsItem(ns3::ThresholdsItem const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ThresholdsItem const &', 'arg0')])
    ## rraa-wifi-manager.h: ns3::ThresholdsItem::ThresholdsItem() [constructor]
    cls.add_constructor([])
    return

def register_Ns3WifiMode_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## wifi-mode.h: ns3::WifiMode::WifiMode(ns3::WifiMode const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMode const &', 'arg0')])
    ## wifi-mode.h: ns3::WifiMode::WifiMode() [constructor]
    cls.add_constructor([])
    ## wifi-mode.h: ns3::WifiMode::WifiMode(std::string name) [constructor]
    cls.add_constructor([param('std::string', 'name')])
    ## wifi-mode.h: uint32_t ns3::WifiMode::GetBandwidth() const [member function]
    cls.add_method('GetBandwidth', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: uint8_t ns3::WifiMode::GetConstellationSize() const [member function]
    cls.add_method('GetConstellationSize', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: uint32_t ns3::WifiMode::GetDataRate() const [member function]
    cls.add_method('GetDataRate', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: ns3::WifiMode::ModulationType ns3::WifiMode::GetModulationType() const [member function]
    cls.add_method('GetModulationType', 
                   'ns3::WifiMode::ModulationType', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: uint32_t ns3::WifiMode::GetPhyRate() const [member function]
    cls.add_method('GetPhyRate', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: uint32_t ns3::WifiMode::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: std::string ns3::WifiMode::GetUniqueName() const [member function]
    cls.add_method('GetUniqueName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: bool ns3::WifiMode::IsMandatory() const [member function]
    cls.add_method('IsMandatory', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: bool ns3::WifiMode::IsModulationBpsk() const [member function]
    cls.add_method('IsModulationBpsk', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: bool ns3::WifiMode::IsModulationQam() const [member function]
    cls.add_method('IsModulationQam', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3WifiModeFactory_methods(root_module, cls):
    ## wifi-mode.h: ns3::WifiModeFactory::WifiModeFactory(ns3::WifiModeFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeFactory const &', 'arg0')])
    ## wifi-mode.h: static ns3::WifiMode ns3::WifiModeFactory::CreateBpsk(std::string uniqueName, bool isMandatory, uint32_t bandwidth, uint32_t dataRate, uint32_t phyRate) [member function]
    cls.add_method('CreateBpsk', 
                   'ns3::WifiMode', 
                   [param('std::string', 'uniqueName'), param('bool', 'isMandatory'), param('uint32_t', 'bandwidth'), param('uint32_t', 'dataRate'), param('uint32_t', 'phyRate')], 
                   is_static=True)
    ## wifi-mode.h: static ns3::WifiMode ns3::WifiModeFactory::CreateQam(std::string uniqueName, bool isMandatory, uint32_t bandwidth, uint32_t dataRate, uint32_t phyRate, uint8_t constellationSize) [member function]
    cls.add_method('CreateQam', 
                   'ns3::WifiMode', 
                   [param('std::string', 'uniqueName'), param('bool', 'isMandatory'), param('uint32_t', 'bandwidth'), param('uint32_t', 'dataRate'), param('uint32_t', 'phyRate'), param('uint8_t', 'constellationSize')], 
                   is_static=True)
    return

def register_Ns3WifiPhyListener_methods(root_module, cls):
    ## wifi-phy.h: ns3::WifiPhyListener::WifiPhyListener(ns3::WifiPhyListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhyListener const &', 'arg0')])
    ## wifi-phy.h: ns3::WifiPhyListener::WifiPhyListener() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyRxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyRxEndOk() [member function]
    cls.add_method('NotifyRxEndOk', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyRxEndError() [member function]
    cls.add_method('NotifyRxEndError', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyTxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyTxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyMaybeCcaBusyStart(ns3::Time duration) [member function]
    cls.add_method('NotifyMaybeCcaBusyStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3WifiRemoteStation_methods(root_module, cls):
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation::WifiRemoteStation(ns3::WifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStation const &', 'arg0')])
    ## wifi-remote-station-manager.h: static ns3::TypeId ns3::WifiRemoteStation::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation::WifiRemoteStation() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::AddSupportedMode(ns3::WifiMode mode) [member function]
    cls.add_method('AddSupportedMode', 
                   'void', 
                   [param('ns3::WifiMode', 'mode')])
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::IsBrandNew() const [member function]
    cls.add_method('IsBrandNew', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::IsAssociated() const [member function]
    cls.add_method('IsAssociated', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::IsWaitAssocTxOk() const [member function]
    cls.add_method('IsWaitAssocTxOk', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::RecordWaitAssocTxOk() [member function]
    cls.add_method('RecordWaitAssocTxOk', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::RecordGotAssocTxOk() [member function]
    cls.add_method('RecordGotAssocTxOk', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::RecordGotAssocTxFailed() [member function]
    cls.add_method('RecordGotAssocTxFailed', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::RecordDisassociated() [member function]
    cls.add_method('RecordDisassociated', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::PrepareForQueue(ns3::Ptr<const ns3::Packet> packet, uint32_t fullPacketSize) [member function]
    cls.add_method('PrepareForQueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fullPacketSize')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::GetDataMode(ns3::Ptr<const ns3::Packet> packet, uint32_t fullPacketSize) [member function]
    cls.add_method('GetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fullPacketSize')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::GetRtsMode(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('GetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::ReportRtsFailed() [member function]
    cls.add_method('ReportRtsFailed', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::ReportDataFailed() [member function]
    cls.add_method('ReportDataFailed', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::ReportRtsOk(double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('ReportRtsOk', 
                   'void', 
                   [param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::ReportDataOk(double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('ReportDataOk', 
                   'void', 
                   [param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::ReportFinalRtsFailed() [member function]
    cls.add_method('ReportFinalRtsFailed', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::ReportFinalDataFailed() [member function]
    cls.add_method('ReportFinalDataFailed', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::ReportRxOk(double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('ReportRxOk', 
                   'void', 
                   [param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')])
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::NeedRts(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedRts', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::NeedRtsRetransmission(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedRtsRetransmission', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::NeedDataRetransmission(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedDataRetransmission', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::NeedFragmentation(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedFragmentation', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStation::GetFragmentSize(ns3::Ptr<const ns3::Packet> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('GetFragmentSize', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStation::GetFragmentOffset(ns3::Ptr<const ns3::Packet> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('GetFragmentOffset', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::IsLastFragment(ns3::Ptr<const ns3::Packet> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('IsLastFragment', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::GetCtsMode(ns3::WifiMode rtsMode) [member function]
    cls.add_method('GetCtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiMode', 'rtsMode')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::GetAckMode(ns3::WifiMode dataMode) [member function]
    cls.add_method('GetAckMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiMode', 'dataMode')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::DoReportRtsFailed() [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::DoReportDataFailed() [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::DoReportRtsOk(double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::DoReportDataOk(double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::DoReportFinalRtsFailed() [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::DoReportFinalDataFailed() [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStation::DoReportRxOk(double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStation::GetNSupportedModes() const [member function]
    cls.add_method('GetNSupportedModes', 
                   'uint32_t', 
                   [], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::GetSupportedMode(uint32_t i) const [member function]
    cls.add_method('GetSupportedMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'i')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::WifiRemoteStation::GetManager() const [member function]
    cls.add_method('GetManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::DoGetDataMode(uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'size')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::DoGetRtsMode() [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3AmrrWifiRemoteStation_methods(root_module, cls):
    ## amrr-wifi-manager.h: ns3::AmrrWifiRemoteStation::AmrrWifiRemoteStation(ns3::AmrrWifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AmrrWifiRemoteStation const &', 'arg0')])
    ## amrr-wifi-manager.h: ns3::AmrrWifiRemoteStation::AmrrWifiRemoteStation(ns3::Ptr<ns3::AmrrWifiManager> stations) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::AmrrWifiManager >', 'stations')])
    ## amrr-wifi-manager.h: void ns3::AmrrWifiRemoteStation::DoReportRxOk(double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='protected', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiRemoteStation::DoReportRtsFailed() [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiRemoteStation::DoReportDataFailed() [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiRemoteStation::DoReportRtsOk(double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='protected', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiRemoteStation::DoReportDataOk(double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='protected', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiRemoteStation::DoReportFinalRtsFailed() [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiRemoteStation::DoReportFinalDataFailed() [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## amrr-wifi-manager.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::AmrrWifiRemoteStation::GetManager() const [member function]
    cls.add_method('GetManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: ns3::WifiMode ns3::AmrrWifiRemoteStation::DoGetDataMode(uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: ns3::WifiMode ns3::AmrrWifiRemoteStation::DoGetRtsMode() [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ArfWifiRemoteStation_methods(root_module, cls):
    ## arf-wifi-manager.h: ns3::ArfWifiRemoteStation::ArfWifiRemoteStation(ns3::ArfWifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ArfWifiRemoteStation const &', 'arg0')])
    ## arf-wifi-manager.h: ns3::ArfWifiRemoteStation::ArfWifiRemoteStation(ns3::Ptr<ns3::ArfWifiManager> stations, int minTimerTimeout, int minSuccessThreshold) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::ArfWifiManager >', 'stations'), param('int', 'minTimerTimeout'), param('int', 'minSuccessThreshold')])
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::DoReportRxOk(double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='protected', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::DoReportRtsFailed() [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::DoReportDataFailed() [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::DoReportRtsOk(double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='protected', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::DoReportDataOk(double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='protected', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::DoReportFinalRtsFailed() [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::DoReportFinalDataFailed() [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## arf-wifi-manager.h: uint32_t ns3::ArfWifiRemoteStation::GetMinTimerTimeout() [member function]
    cls.add_method('GetMinTimerTimeout', 
                   'uint32_t', 
                   [], 
                   visibility='protected')
    ## arf-wifi-manager.h: uint32_t ns3::ArfWifiRemoteStation::GetMinSuccessThreshold() [member function]
    cls.add_method('GetMinSuccessThreshold', 
                   'uint32_t', 
                   [], 
                   visibility='protected')
    ## arf-wifi-manager.h: uint32_t ns3::ArfWifiRemoteStation::GetTimerTimeout() [member function]
    cls.add_method('GetTimerTimeout', 
                   'uint32_t', 
                   [], 
                   visibility='protected')
    ## arf-wifi-manager.h: uint32_t ns3::ArfWifiRemoteStation::GetSuccessThreshold() [member function]
    cls.add_method('GetSuccessThreshold', 
                   'uint32_t', 
                   [], 
                   visibility='protected')
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::SetTimerTimeout(uint32_t timerTimeout) [member function]
    cls.add_method('SetTimerTimeout', 
                   'void', 
                   [param('uint32_t', 'timerTimeout')], 
                   visibility='protected')
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::SetSuccessThreshold(uint32_t successThreshold) [member function]
    cls.add_method('SetSuccessThreshold', 
                   'void', 
                   [param('uint32_t', 'successThreshold')], 
                   visibility='protected')
    ## arf-wifi-manager.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::ArfWifiRemoteStation::GetManager() const [member function]
    cls.add_method('GetManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: ns3::WifiMode ns3::ArfWifiRemoteStation::DoGetDataMode(uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: ns3::WifiMode ns3::ArfWifiRemoteStation::DoGetRtsMode() [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::ReportRecoveryFailure() [member function]
    cls.add_method('ReportRecoveryFailure', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiRemoteStation::ReportFailure() [member function]
    cls.add_method('ReportFailure', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ConstantRateWifiRemoteStation_methods(root_module, cls):
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiRemoteStation::ConstantRateWifiRemoteStation(ns3::ConstantRateWifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantRateWifiRemoteStation const &', 'arg0')])
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiRemoteStation::ConstantRateWifiRemoteStation(ns3::Ptr<ns3::ConstantRateWifiManager> stations) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::ConstantRateWifiManager >', 'stations')])
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiRemoteStation::DoReportRxOk(double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='protected', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiRemoteStation::DoReportRtsFailed() [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiRemoteStation::DoReportDataFailed() [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiRemoteStation::DoReportRtsOk(double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='protected', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiRemoteStation::DoReportDataOk(double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='protected', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiRemoteStation::DoReportFinalRtsFailed() [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiRemoteStation::DoReportFinalDataFailed() [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## constant-rate-wifi-manager.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::ConstantRateWifiRemoteStation::GetManager() const [member function]
    cls.add_method('GetManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: ns3::WifiMode ns3::ConstantRateWifiRemoteStation::DoGetDataMode(uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: ns3::WifiMode ns3::ConstantRateWifiRemoteStation::DoGetRtsMode() [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3IdealWifiRemoteStation_methods(root_module, cls):
    ## ideal-wifi-manager.h: ns3::IdealWifiRemoteStation::IdealWifiRemoteStation(ns3::IdealWifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IdealWifiRemoteStation const &', 'arg0')])
    ## ideal-wifi-manager.h: ns3::IdealWifiRemoteStation::IdealWifiRemoteStation(ns3::Ptr<ns3::IdealWifiManager> stations) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::IdealWifiManager >', 'stations')])
    ## ideal-wifi-manager.h: void ns3::IdealWifiRemoteStation::DoReportRxOk(double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='protected', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiRemoteStation::DoReportRtsFailed() [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiRemoteStation::DoReportDataFailed() [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiRemoteStation::DoReportRtsOk(double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='protected', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiRemoteStation::DoReportDataOk(double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='protected', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiRemoteStation::DoReportFinalRtsFailed() [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiRemoteStation::DoReportFinalDataFailed() [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ideal-wifi-manager.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::IdealWifiRemoteStation::GetManager() const [member function]
    cls.add_method('GetManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: ns3::WifiMode ns3::IdealWifiRemoteStation::DoGetDataMode(uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: ns3::WifiMode ns3::IdealWifiRemoteStation::DoGetRtsMode() [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3OnoeWifiRemoteStation_methods(root_module, cls):
    ## onoe-wifi-manager.h: ns3::OnoeWifiRemoteStation::OnoeWifiRemoteStation(ns3::OnoeWifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OnoeWifiRemoteStation const &', 'arg0')])
    ## onoe-wifi-manager.h: ns3::OnoeWifiRemoteStation::OnoeWifiRemoteStation(ns3::Ptr<ns3::OnoeWifiManager> stations) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::OnoeWifiManager >', 'stations')])
    ## onoe-wifi-manager.h: void ns3::OnoeWifiRemoteStation::DoReportRxOk(double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='protected', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiRemoteStation::DoReportRtsFailed() [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiRemoteStation::DoReportDataFailed() [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiRemoteStation::DoReportRtsOk(double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='protected', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiRemoteStation::DoReportDataOk(double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='protected', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiRemoteStation::DoReportFinalRtsFailed() [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiRemoteStation::DoReportFinalDataFailed() [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## onoe-wifi-manager.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::OnoeWifiRemoteStation::GetManager() const [member function]
    cls.add_method('GetManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: ns3::WifiMode ns3::OnoeWifiRemoteStation::DoGetDataMode(uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: ns3::WifiMode ns3::OnoeWifiRemoteStation::DoGetRtsMode() [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::PropagationDelayModel::PropagationDelayModel(ns3::PropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: ns3::PropagationDelayModel::PropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: static ns3::TypeId ns3::PropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-delay-model.h: ns3::Time ns3::PropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3PropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::PropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::PropagationLossModel::PropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::PropagationLossModel::SetNext(ns3::Ptr<ns3::PropagationLossModel> next) [member function]
    cls.add_method('SetNext', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationLossModel >', 'next')])
    ## propagation-loss-model.h: double ns3::PropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::PropagationLossModel::DoGetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoGetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RandomPropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel::RandomPropagationDelayModel(ns3::RandomPropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomPropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: static ns3::TypeId ns3::RandomPropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel::RandomPropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: ns3::Time ns3::RandomPropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3RandomPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::RandomPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::RandomPropagationLossModel::RandomPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::RandomPropagationLossModel::DoGetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoGetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RraaWifiRemoteStation_methods(root_module, cls):
    ## rraa-wifi-manager.h: ns3::RraaWifiRemoteStation::RraaWifiRemoteStation(ns3::RraaWifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RraaWifiRemoteStation const &', 'arg0')])
    ## rraa-wifi-manager.h: ns3::RraaWifiRemoteStation::RraaWifiRemoteStation(ns3::Ptr<ns3::RraaWifiManager> stations) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::RraaWifiManager >', 'stations')])
    ## rraa-wifi-manager.h: bool ns3::RraaWifiRemoteStation::NeedRts(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedRts', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiRemoteStation::DoReportRxOk(double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='protected', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiRemoteStation::DoReportRtsFailed() [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiRemoteStation::DoReportDataFailed() [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiRemoteStation::DoReportRtsOk(double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='protected', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiRemoteStation::DoReportDataOk(double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='protected', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiRemoteStation::DoReportFinalRtsFailed() [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiRemoteStation::DoReportFinalDataFailed() [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## rraa-wifi-manager.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::RraaWifiRemoteStation::GetManager() const [member function]
    cls.add_method('GetManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: ns3::WifiMode ns3::RraaWifiRemoteStation::DoGetDataMode(uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: ns3::WifiMode ns3::RraaWifiRemoteStation::DoGetRtsMode() [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3SsidChecker_methods(root_module, cls):
    ## ssid.h: ns3::SsidChecker::SsidChecker(ns3::SsidChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SsidChecker const &', 'arg0')])
    ## ssid.h: ns3::SsidChecker::SsidChecker() [constructor]
    cls.add_constructor([])
    return

def register_Ns3SsidValue_methods(root_module, cls):
    ## ssid.h: ns3::SsidValue::SsidValue(ns3::SsidValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SsidValue const &', 'arg0')])
    ## ssid.h: ns3::SsidValue::SsidValue() [constructor]
    cls.add_constructor([])
    ## ssid.h: ns3::SsidValue::SsidValue(ns3::Ssid const & value) [constructor]
    cls.add_constructor([param('ns3::Ssid const &', 'value')])
    ## ssid.h: void ns3::SsidValue::Set(ns3::Ssid const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ssid const &', 'value')])
    ## ssid.h: ns3::Ssid ns3::SsidValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## ssid.h: ns3::Ptr<ns3::AttributeValue> ns3::SsidValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ssid.h: std::string ns3::SsidValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ssid.h: bool ns3::SsidValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    return

def register_Ns3WifiMac_methods(root_module, cls):
    ## wifi-mac.h: ns3::WifiMac::WifiMac(ns3::WifiMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMac const &', 'arg0')])
    ## wifi-mac.h: ns3::WifiMac::WifiMac() [constructor]
    cls.add_constructor([])
    ## wifi-mac.h: static ns3::TypeId ns3::WifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-mac.h: void ns3::WifiMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetMaxPropagationDelay(ns3::Time delay) [member function]
    cls.add_method('SetMaxPropagationDelay', 
                   'void', 
                   [param('ns3::Time', 'delay')])
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetSlot() const [member function]
    cls.add_method('GetSlot', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetMsduLifetime() const [member function]
    cls.add_method('GetMsduLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetMaxPropagationDelay() const [member function]
    cls.add_method('GetMaxPropagationDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wifi-mac.h: uint32_t ns3::WifiMac::GetMaxMsduSize() const [member function]
    cls.add_method('GetMaxMsduSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-mac.h: ns3::Mac48Address ns3::WifiMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Ssid ns3::WifiMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: ns3::Mac48Address ns3::WifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: bool ns3::WifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3WifiModeChecker_methods(root_module, cls):
    ## wifi-mode.h: ns3::WifiModeChecker::WifiModeChecker(ns3::WifiModeChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeChecker const &', 'arg0')])
    ## wifi-mode.h: ns3::WifiModeChecker::WifiModeChecker() [constructor]
    cls.add_constructor([])
    return

def register_Ns3WifiModeValue_methods(root_module, cls):
    ## wifi-mode.h: ns3::WifiModeValue::WifiModeValue(ns3::WifiModeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeValue const &', 'arg0')])
    ## wifi-mode.h: ns3::WifiModeValue::WifiModeValue() [constructor]
    cls.add_constructor([])
    ## wifi-mode.h: ns3::WifiModeValue::WifiModeValue(ns3::WifiMode const & value) [constructor]
    cls.add_constructor([param('ns3::WifiMode const &', 'value')])
    ## wifi-mode.h: void ns3::WifiModeValue::Set(ns3::WifiMode const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::WifiMode const &', 'value')])
    ## wifi-mode.h: ns3::WifiMode ns3::WifiModeValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: ns3::Ptr<ns3::AttributeValue> ns3::WifiModeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mode.h: std::string ns3::WifiModeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## wifi-mode.h: bool ns3::WifiModeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    return

def register_Ns3WifiPhy_methods(root_module, cls):
    ## wifi-phy.h: ns3::WifiPhy::g_6mba [variable]
    cls.add_static_attribute('g_6mba', 'ns3::WifiMode', is_const=False)
    ## wifi-phy.h: ns3::WifiPhy::g_9mba [variable]
    cls.add_static_attribute('g_9mba', 'ns3::WifiMode', is_const=False)
    ## wifi-phy.h: ns3::WifiPhy::g_12mba [variable]
    cls.add_static_attribute('g_12mba', 'ns3::WifiMode', is_const=False)
    ## wifi-phy.h: ns3::WifiPhy::g_18mba [variable]
    cls.add_static_attribute('g_18mba', 'ns3::WifiMode', is_const=False)
    ## wifi-phy.h: ns3::WifiPhy::g_24mba [variable]
    cls.add_static_attribute('g_24mba', 'ns3::WifiMode', is_const=False)
    ## wifi-phy.h: ns3::WifiPhy::g_36mba [variable]
    cls.add_static_attribute('g_36mba', 'ns3::WifiMode', is_const=False)
    ## wifi-phy.h: ns3::WifiPhy::g_48mba [variable]
    cls.add_static_attribute('g_48mba', 'ns3::WifiMode', is_const=False)
    ## wifi-phy.h: ns3::WifiPhy::g_54mba [variable]
    cls.add_static_attribute('g_54mba', 'ns3::WifiMode', is_const=False)
    ## wifi-phy.h: ns3::WifiPhy::WifiPhy(ns3::WifiPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhy const &', 'arg0')])
    ## wifi-phy.h: static ns3::TypeId ns3::WifiPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: ns3::WifiPhy::WifiPhy() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h: double ns3::WifiPhy::GetTxPowerStart() const [member function]
    cls.add_method('GetTxPowerStart', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: double ns3::WifiPhy::GetTxPowerEnd() const [member function]
    cls.add_method('GetTxPowerEnd', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: uint32_t ns3::WifiPhy::GetNTxPower() const [member function]
    cls.add_method('GetNTxPower', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::SetReceiveOkCallback(ns3::Callback<void,ns3::Ptr<ns3::Packet>,double,ns3::WifiMode,ns3::WifiPreamble,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::WifiMode, ns3::WifiPreamble, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::SetReceiveErrorCallback(ns3::Callback<void,ns3::Ptr<const ns3::Packet>,double,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::SendPacket(ns3::Ptr<const ns3::Packet> packet, ns3::WifiMode mode, ns3::WifiPreamble preamble, uint8_t txPowerLevel) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble'), param('uint8_t', 'txPowerLevel')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::RegisterListener(ns3::WifiPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::WifiPhyListener *', 'listener')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateSync() [member function]
    cls.add_method('IsStateSync', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetStateDuration() [member function]
    cls.add_method('GetStateDuration', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetDelayUntilIdle() [member function]
    cls.add_method('GetDelayUntilIdle', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetLastRxStartTime() const [member function]
    cls.add_method('GetLastRxStartTime', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::CalculateTxDuration(uint32_t size, ns3::WifiMode payloadMode, ns3::WifiPreamble preamble) const [member function]
    cls.add_method('CalculateTxDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: uint32_t ns3::WifiPhy::GetNModes() const [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: ns3::WifiMode ns3::WifiPhy::GetMode(uint32_t mode) const [member function]
    cls.add_method('GetMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'mode')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: double ns3::WifiPhy::CalculateSnr(ns3::WifiMode txMode, double ber) const [member function]
    cls.add_method('CalculateSnr', 
                   'double', 
                   [param('ns3::WifiMode', 'txMode'), param('double', 'ber')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: ns3::Ptr<ns3::WifiChannel> ns3::WifiPhy::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::WifiChannel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3WifiRemoteStationManager_methods(root_module, cls):
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationManager::WifiRemoteStationManager(ns3::WifiRemoteStationManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStationManager const &', 'arg0')])
    ## wifi-remote-station-manager.h: static ns3::TypeId ns3::WifiRemoteStationManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationManager::WifiRemoteStationManager() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetupPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetupPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetMaxSsrc() const [member function]
    cls.add_method('GetMaxSsrc', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetMaxSlrc() const [member function]
    cls.add_method('GetMaxSlrc', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetRtsCtsThreshold() const [member function]
    cls.add_method('GetRtsCtsThreshold', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetFragmentationThreshold() const [member function]
    cls.add_method('GetFragmentationThreshold', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetMaxSsrc(uint32_t maxSsrc) [member function]
    cls.add_method('SetMaxSsrc', 
                   'void', 
                   [param('uint32_t', 'maxSsrc')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetMaxSlrc(uint32_t maxSlrc) [member function]
    cls.add_method('SetMaxSlrc', 
                   'void', 
                   [param('uint32_t', 'maxSlrc')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetRtsCtsThreshold(uint32_t threshold) [member function]
    cls.add_method('SetRtsCtsThreshold', 
                   'void', 
                   [param('uint32_t', 'threshold')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetFragmentationThreshold(uint32_t threshold) [member function]
    cls.add_method('SetFragmentationThreshold', 
                   'void', 
                   [param('uint32_t', 'threshold')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::AddBasicMode(ns3::WifiMode mode) [member function]
    cls.add_method('AddBasicMode', 
                   'void', 
                   [param('ns3::WifiMode', 'mode')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetDefaultMode() const [member function]
    cls.add_method('GetDefaultMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetNBasicModes() const [member function]
    cls.add_method('GetNBasicModes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetBasicMode(uint32_t i) const [member function]
    cls.add_method('GetBasicMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## wifi-remote-station-manager.h: __gnu_cxx::__normal_iterator<const ns3::WifiMode*,std::vector<ns3::WifiMode, std::allocator<ns3::WifiMode> > > ns3::WifiRemoteStationManager::BeginBasicModes() const [member function]
    cls.add_method('BeginBasicModes', 
                   '__gnu_cxx::__normal_iterator< ns3::WifiMode const *, std::vector< ns3::WifiMode > >', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: __gnu_cxx::__normal_iterator<const ns3::WifiMode*,std::vector<ns3::WifiMode, std::allocator<ns3::WifiMode> > > ns3::WifiRemoteStationManager::EndBasicModes() const [member function]
    cls.add_method('EndBasicModes', 
                   '__gnu_cxx::__normal_iterator< ns3::WifiMode const *, std::vector< ns3::WifiMode > >', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation * ns3::WifiRemoteStationManager::Lookup(ns3::Mac48Address address) [member function]
    cls.add_method('Lookup', 
                   'ns3::WifiRemoteStation *', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation * ns3::WifiRemoteStationManager::LookupNonUnicast() [member function]
    cls.add_method('LookupNonUnicast', 
                   'ns3::WifiRemoteStation *', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation * ns3::WifiRemoteStationManager::CreateStation() [member function]
    cls.add_method('CreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3YansWifiPhy_methods(root_module, cls):
    ## yans-wifi-phy.h: static ns3::TypeId ns3::YansWifiPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## yans-wifi-phy.h: ns3::YansWifiPhy::YansWifiPhy() [constructor]
    cls.add_constructor([])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetChannel(ns3::Ptr<ns3::YansWifiChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::YansWifiChannel >', 'channel')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::StartReceivePacket(ns3::Ptr<ns3::Packet> packet, double rxPowerDbm, ns3::WifiMode mode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('StartReceivePacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDbm'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('SetStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetRxNoise(double ratio) [member function]
    cls.add_method('SetRxNoise', 
                   'void', 
                   [param('double', 'ratio')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetTxPowerStart(double start) [member function]
    cls.add_method('SetTxPowerStart', 
                   'void', 
                   [param('double', 'start')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetTxPowerEnd(double end) [member function]
    cls.add_method('SetTxPowerEnd', 
                   'void', 
                   [param('double', 'end')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetNTxPower(uint32_t n) [member function]
    cls.add_method('SetNTxPower', 
                   'void', 
                   [param('uint32_t', 'n')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetTxGain(double gain) [member function]
    cls.add_method('SetTxGain', 
                   'void', 
                   [param('double', 'gain')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetRxGain(double gain) [member function]
    cls.add_method('SetRxGain', 
                   'void', 
                   [param('double', 'gain')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetEdThreshold(double threshold) [member function]
    cls.add_method('SetEdThreshold', 
                   'void', 
                   [param('double', 'threshold')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetCcaMode1Threshold(double threshold) [member function]
    cls.add_method('SetCcaMode1Threshold', 
                   'void', 
                   [param('double', 'threshold')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetErrorRateModel(ns3::Ptr<ns3::ErrorRateModel> rate) [member function]
    cls.add_method('SetErrorRateModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorRateModel >', 'rate')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetDevice(ns3::Ptr<ns3::Object> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'device')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetMobility(ns3::Ptr<ns3::Object> mobility) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'mobility')])
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetRxNoise() const [member function]
    cls.add_method('GetRxNoise', 
                   'double', 
                   [], 
                   is_const=True)
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetTxGain() const [member function]
    cls.add_method('GetTxGain', 
                   'double', 
                   [], 
                   is_const=True)
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetRxGain() const [member function]
    cls.add_method('GetRxGain', 
                   'double', 
                   [], 
                   is_const=True)
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetEdThreshold() const [member function]
    cls.add_method('GetEdThreshold', 
                   'double', 
                   [], 
                   is_const=True)
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetCcaMode1Threshold() const [member function]
    cls.add_method('GetCcaMode1Threshold', 
                   'double', 
                   [], 
                   is_const=True)
    ## yans-wifi-phy.h: ns3::Ptr<ns3::ErrorRateModel> ns3::YansWifiPhy::GetErrorRateModel() const [member function]
    cls.add_method('GetErrorRateModel', 
                   'ns3::Ptr< ns3::ErrorRateModel >', 
                   [], 
                   is_const=True)
    ## yans-wifi-phy.h: ns3::Ptr<ns3::Object> ns3::YansWifiPhy::GetDevice() const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## yans-wifi-phy.h: ns3::Ptr<ns3::Object> ns3::YansWifiPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [])
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetTxPowerStart() const [member function]
    cls.add_method('GetTxPowerStart', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetTxPowerEnd() const [member function]
    cls.add_method('GetTxPowerEnd', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: uint32_t ns3::YansWifiPhy::GetNTxPower() const [member function]
    cls.add_method('GetNTxPower', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetReceiveOkCallback(ns3::Callback<void,ns3::Ptr<ns3::Packet>,double,ns3::WifiMode,ns3::WifiPreamble,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::WifiMode, ns3::WifiPreamble, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetReceiveErrorCallback(ns3::Callback<void,ns3::Ptr<const ns3::Packet>,double,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SendPacket(ns3::Ptr<const ns3::Packet> packet, ns3::WifiMode mode, ns3::WifiPreamble preamble, uint8_t txPowerLevel) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble'), param('uint8_t', 'txPowerLevel')], 
                   is_virtual=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::RegisterListener(ns3::WifiPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::WifiPhyListener *', 'listener')], 
                   is_virtual=True)
    ## yans-wifi-phy.h: bool ns3::YansWifiPhy::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: bool ns3::YansWifiPhy::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: bool ns3::YansWifiPhy::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: bool ns3::YansWifiPhy::IsStateSync() [member function]
    cls.add_method('IsStateSync', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: bool ns3::YansWifiPhy::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: ns3::Time ns3::YansWifiPhy::GetStateDuration() [member function]
    cls.add_method('GetStateDuration', 
                   'ns3::Time', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: ns3::Time ns3::YansWifiPhy::GetDelayUntilIdle() [member function]
    cls.add_method('GetDelayUntilIdle', 
                   'ns3::Time', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: ns3::Time ns3::YansWifiPhy::GetLastRxStartTime() const [member function]
    cls.add_method('GetLastRxStartTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: ns3::Time ns3::YansWifiPhy::CalculateTxDuration(uint32_t size, ns3::WifiMode payloadMode, ns3::WifiPreamble preamble) const [member function]
    cls.add_method('CalculateTxDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: uint32_t ns3::YansWifiPhy::GetNModes() const [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: ns3::WifiMode ns3::YansWifiPhy::GetMode(uint32_t mode) const [member function]
    cls.add_method('GetMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'mode')], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::CalculateSnr(ns3::WifiMode txMode, double ber) const [member function]
    cls.add_method('CalculateSnr', 
                   'double', 
                   [param('ns3::WifiMode', 'txMode'), param('double', 'ber')], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: ns3::Ptr<ns3::WifiChannel> ns3::YansWifiPhy::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::WifiChannel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AarfWifiRemoteStation_methods(root_module, cls):
    ## aarf-wifi-manager.h: ns3::AarfWifiRemoteStation::AarfWifiRemoteStation(ns3::AarfWifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AarfWifiRemoteStation const &', 'arg0')])
    ## aarf-wifi-manager.h: ns3::AarfWifiRemoteStation::AarfWifiRemoteStation(ns3::Ptr<ns3::AarfWifiManager> stations, uint32_t minTimerThreshold, uint32_t minSuccessThreshold, double successK, uint32_t maxSuccessThreshold, double timerK) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::AarfWifiManager >', 'stations'), param('uint32_t', 'minTimerThreshold'), param('uint32_t', 'minSuccessThreshold'), param('double', 'successK'), param('uint32_t', 'maxSuccessThreshold'), param('double', 'timerK')])
    ## aarf-wifi-manager.h: void ns3::AarfWifiRemoteStation::ReportRecoveryFailure() [member function]
    cls.add_method('ReportRecoveryFailure', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: void ns3::AarfWifiRemoteStation::ReportFailure() [member function]
    cls.add_method('ReportFailure', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AdhocWifiMac_methods(root_module, cls):
    ## adhoc-wifi-mac.h: static ns3::TypeId ns3::AdhocWifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## adhoc-wifi-mac.h: ns3::AdhocWifiMac::AdhocWifiMac() [constructor]
    cls.add_constructor([])
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Time ns3::AdhocWifiMac::GetSlot() const [member function]
    cls.add_method('GetSlot', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Time ns3::AdhocWifiMac::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Time ns3::AdhocWifiMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Time ns3::AdhocWifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Time ns3::AdhocWifiMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Time ns3::AdhocWifiMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: bool ns3::AdhocWifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Mac48Address ns3::AdhocWifiMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Ssid ns3::AdhocWifiMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: ns3::Mac48Address ns3::AdhocWifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AmrrWifiManager_methods(root_module, cls):
    ## amrr-wifi-manager.h: ns3::AmrrWifiManager::AmrrWifiManager(ns3::AmrrWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AmrrWifiManager const &', 'arg0')])
    ## amrr-wifi-manager.h: static ns3::TypeId ns3::AmrrWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## amrr-wifi-manager.h: ns3::AmrrWifiManager::AmrrWifiManager() [constructor]
    cls.add_constructor([])
    ## amrr-wifi-manager.h: ns3::WifiRemoteStation * ns3::AmrrWifiManager::CreateStation() [member function]
    cls.add_method('CreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ArfWifiManager_methods(root_module, cls):
    ## arf-wifi-manager.h: ns3::ArfWifiManager::ArfWifiManager(ns3::ArfWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ArfWifiManager const &', 'arg0')])
    ## arf-wifi-manager.h: static ns3::TypeId ns3::ArfWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## arf-wifi-manager.h: ns3::ArfWifiManager::ArfWifiManager() [constructor]
    cls.add_constructor([])
    ## arf-wifi-manager.h: ns3::WifiRemoteStation * ns3::ArfWifiManager::CreateStation() [member function]
    cls.add_method('CreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ConstantRateWifiManager_methods(root_module, cls):
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiManager::ConstantRateWifiManager(ns3::ConstantRateWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantRateWifiManager const &', 'arg0')])
    ## constant-rate-wifi-manager.h: static ns3::TypeId ns3::ConstantRateWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiManager::ConstantRateWifiManager() [constructor]
    cls.add_constructor([])
    ## constant-rate-wifi-manager.h: ns3::WifiMode ns3::ConstantRateWifiManager::GetDataMode() const [member function]
    cls.add_method('GetDataMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## constant-rate-wifi-manager.h: ns3::WifiMode ns3::ConstantRateWifiManager::GetCtlMode() const [member function]
    cls.add_method('GetCtlMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## constant-rate-wifi-manager.h: ns3::WifiRemoteStation * ns3::ConstantRateWifiManager::CreateStation() [member function]
    cls.add_method('CreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel(ns3::ConstantSpeedPropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantSpeedPropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: static ns3::TypeId ns3::ConstantSpeedPropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: ns3::Time ns3::ConstantSpeedPropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## propagation-delay-model.h: void ns3::ConstantSpeedPropagationDelayModel::SetSpeed(double speed) [member function]
    cls.add_method('SetSpeed', 
                   'void', 
                   [param('double', 'speed')])
    ## propagation-delay-model.h: double ns3::ConstantSpeedPropagationDelayModel::GetSpeed() const [member function]
    cls.add_method('GetSpeed', 
                   'double', 
                   [], 
                   is_const=True)
    return

def register_Ns3ErrorRateModel_methods(root_module, cls):
    ## error-rate-model.h: ns3::ErrorRateModel::ErrorRateModel(ns3::ErrorRateModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErrorRateModel const &', 'arg0')])
    ## error-rate-model.h: ns3::ErrorRateModel::ErrorRateModel() [constructor]
    cls.add_constructor([])
    ## error-rate-model.h: static ns3::TypeId ns3::ErrorRateModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## error-rate-model.h: double ns3::ErrorRateModel::CalculateSnr(ns3::WifiMode txMode, double ber) const [member function]
    cls.add_method('CalculateSnr', 
                   'double', 
                   [param('ns3::WifiMode', 'txMode'), param('double', 'ber')], 
                   is_const=True)
    ## error-rate-model.h: double ns3::ErrorRateModel::GetChunkSuccessRate(ns3::WifiMode mode, double snr, uint32_t nbits) const [member function]
    cls.add_method('GetChunkSuccessRate', 
                   'double', 
                   [param('ns3::WifiMode', 'mode'), param('double', 'snr'), param('uint32_t', 'nbits')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3FriisPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::FriisPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::FriisPropagationLossModel::FriisPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::FriisPropagationLossModel::SetLambda(double frequency, double speed) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'frequency'), param('double', 'speed')])
    ## propagation-loss-model.h: void ns3::FriisPropagationLossModel::SetLambda(double lambda) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'lambda')])
    ## propagation-loss-model.h: void ns3::FriisPropagationLossModel::SetSystemLoss(double systemLoss) [member function]
    cls.add_method('SetSystemLoss', 
                   'void', 
                   [param('double', 'systemLoss')])
    ## propagation-loss-model.h: void ns3::FriisPropagationLossModel::SetMinDistance(double minDistance) [member function]
    cls.add_method('SetMinDistance', 
                   'void', 
                   [param('double', 'minDistance')])
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::GetMinDistance() const [member function]
    cls.add_method('GetMinDistance', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::GetLambda() const [member function]
    cls.add_method('GetLambda', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::GetSystemLoss() const [member function]
    cls.add_method('GetSystemLoss', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::DoGetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoGetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3IdealWifiManager_methods(root_module, cls):
    ## ideal-wifi-manager.h: ns3::IdealWifiManager::IdealWifiManager(ns3::IdealWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IdealWifiManager const &', 'arg0')])
    ## ideal-wifi-manager.h: static ns3::TypeId ns3::IdealWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ideal-wifi-manager.h: ns3::IdealWifiManager::IdealWifiManager() [constructor]
    cls.add_constructor([])
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::SetupPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetupPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## ideal-wifi-manager.h: double ns3::IdealWifiManager::GetSnrThreshold(ns3::WifiMode mode) const [member function]
    cls.add_method('GetSnrThreshold', 
                   'double', 
                   [param('ns3::WifiMode', 'mode')], 
                   is_const=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::AddModeSnrThreshold(ns3::WifiMode mode, double ber) [member function]
    cls.add_method('AddModeSnrThreshold', 
                   'void', 
                   [param('ns3::WifiMode', 'mode'), param('double', 'ber')])
    ## ideal-wifi-manager.h: ns3::WifiRemoteStation * ns3::IdealWifiManager::CreateStation() [member function]
    cls.add_method('CreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3JakesPropagationLossModel_methods(root_module, cls):
    ## jakes-propagation-loss-model.h: static ns3::TypeId ns3::JakesPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## jakes-propagation-loss-model.h: ns3::JakesPropagationLossModel::JakesPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## jakes-propagation-loss-model.h: void ns3::JakesPropagationLossModel::SetNRays(uint8_t nRays) [member function]
    cls.add_method('SetNRays', 
                   'void', 
                   [param('uint8_t', 'nRays')])
    ## jakes-propagation-loss-model.h: void ns3::JakesPropagationLossModel::SetNOscillators(uint8_t nOscillators) [member function]
    cls.add_method('SetNOscillators', 
                   'void', 
                   [param('uint8_t', 'nOscillators')])
    ## jakes-propagation-loss-model.h: double ns3::JakesPropagationLossModel::DoGetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoGetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3LogDistancePropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::LogDistancePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::LogDistancePropagationLossModel::LogDistancePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::LogDistancePropagationLossModel::SetPathLossExponent(double n) [member function]
    cls.add_method('SetPathLossExponent', 
                   'void', 
                   [param('double', 'n')])
    ## propagation-loss-model.h: double ns3::LogDistancePropagationLossModel::GetPathLossExponent() const [member function]
    cls.add_method('GetPathLossExponent', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: void ns3::LogDistancePropagationLossModel::SetReference(double referenceDistance, double referenceLoss) [member function]
    cls.add_method('SetReference', 
                   'void', 
                   [param('double', 'referenceDistance'), param('double', 'referenceLoss')])
    ## propagation-loss-model.h: double ns3::LogDistancePropagationLossModel::DoGetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoGetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3NqapWifiMac_methods(root_module, cls):
    ## nqap-wifi-mac.h: static ns3::TypeId ns3::NqapWifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## nqap-wifi-mac.h: ns3::NqapWifiMac::NqapWifiMac() [constructor]
    cls.add_constructor([])
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Time ns3::NqapWifiMac::GetSlot() const [member function]
    cls.add_method('GetSlot', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Time ns3::NqapWifiMac::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Time ns3::NqapWifiMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Time ns3::NqapWifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Time ns3::NqapWifiMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Time ns3::NqapWifiMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: bool ns3::NqapWifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Mac48Address ns3::NqapWifiMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Ssid ns3::NqapWifiMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: ns3::Mac48Address ns3::NqapWifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetBeaconInterval(ns3::Time interval) [member function]
    cls.add_method('SetBeaconInterval', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## nqap-wifi-mac.h: ns3::Time ns3::NqapWifiMac::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::StartBeaconing() [member function]
    cls.add_method('StartBeaconing', 
                   'void', 
                   [])
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3NqstaWifiMac_methods(root_module, cls):
    ## nqsta-wifi-mac.h: static ns3::TypeId ns3::NqstaWifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## nqsta-wifi-mac.h: ns3::NqstaWifiMac::NqstaWifiMac() [constructor]
    cls.add_constructor([])
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Time ns3::NqstaWifiMac::GetSlot() const [member function]
    cls.add_method('GetSlot', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Time ns3::NqstaWifiMac::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Time ns3::NqstaWifiMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Time ns3::NqstaWifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Time ns3::NqstaWifiMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Time ns3::NqstaWifiMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: bool ns3::NqstaWifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Mac48Address ns3::NqstaWifiMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Ssid ns3::NqstaWifiMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: ns3::Mac48Address ns3::NqstaWifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetMaxMissedBeacons(uint32_t missed) [member function]
    cls.add_method('SetMaxMissedBeacons', 
                   'void', 
                   [param('uint32_t', 'missed')])
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetProbeRequestTimeout(ns3::Time timeout) [member function]
    cls.add_method('SetProbeRequestTimeout', 
                   'void', 
                   [param('ns3::Time', 'timeout')])
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetAssocRequestTimeout(ns3::Time timeout) [member function]
    cls.add_method('SetAssocRequestTimeout', 
                   'void', 
                   [param('ns3::Time', 'timeout')])
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::StartActiveAssociation() [member function]
    cls.add_method('StartActiveAssociation', 
                   'void', 
                   [])
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3OnoeWifiManager_methods(root_module, cls):
    ## onoe-wifi-manager.h: ns3::OnoeWifiManager::OnoeWifiManager(ns3::OnoeWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OnoeWifiManager const &', 'arg0')])
    ## onoe-wifi-manager.h: static ns3::TypeId ns3::OnoeWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## onoe-wifi-manager.h: ns3::OnoeWifiManager::OnoeWifiManager() [constructor]
    cls.add_constructor([])
    ## onoe-wifi-manager.h: ns3::WifiRemoteStation * ns3::OnoeWifiManager::CreateStation() [member function]
    cls.add_method('CreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3RraaWifiManager_methods(root_module, cls):
    ## rraa-wifi-manager.h: ns3::RraaWifiManager::RraaWifiManager(ns3::RraaWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RraaWifiManager const &', 'arg0')])
    ## rraa-wifi-manager.h: static ns3::TypeId ns3::RraaWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rraa-wifi-manager.h: ns3::RraaWifiManager::RraaWifiManager() [constructor]
    cls.add_constructor([])
    ## rraa-wifi-manager.h: bool ns3::RraaWifiManager::OnlyBasic() [member function]
    cls.add_method('OnlyBasic', 
                   'bool', 
                   [])
    ## rraa-wifi-manager.h: ns3::Time ns3::RraaWifiManager::GetTimeout() const [member function]
    cls.add_method('GetTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## rraa-wifi-manager.h: ns3::ThresholdsItem ns3::RraaWifiManager::GetThresholds(ns3::WifiMode mode) const [member function]
    cls.add_method('GetThresholds', 
                   'ns3::ThresholdsItem', 
                   [param('ns3::WifiMode', 'mode')], 
                   is_const=True)
    ## rraa-wifi-manager.h: ns3::WifiRemoteStation * ns3::RraaWifiManager::CreateStation() [member function]
    cls.add_method('CreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3WifiChannel_methods(root_module, cls):
    ## wifi-channel.h: ns3::WifiChannel::WifiChannel(ns3::WifiChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiChannel const &', 'arg0')])
    ## wifi-channel.h: ns3::WifiChannel::WifiChannel() [constructor]
    cls.add_constructor([])
    ## wifi-channel.h: static ns3::TypeId ns3::WifiChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3WifiNetDevice_methods(root_module, cls):
    ## wifi-net-device.h: ns3::WifiNetDevice::WifiNetDevice(ns3::WifiNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiNetDevice const &', 'arg0')])
    ## wifi-net-device.h: static ns3::TypeId ns3::WifiNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-net-device.h: ns3::WifiNetDevice::WifiNetDevice() [constructor]
    cls.add_constructor([])
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetMac(ns3::Ptr<ns3::WifiMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiMac >', 'mac')])
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')])
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> manager) [member function]
    cls.add_method('SetRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'manager')])
    ## wifi-net-device.h: ns3::Ptr<ns3::WifiMac> ns3::WifiNetDevice::GetMac() const [member function]
    cls.add_method('GetMac', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_const=True)
    ## wifi-net-device.h: ns3::Ptr<ns3::WifiPhy> ns3::WifiNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [], 
                   is_const=True)
    ## wifi-net-device.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::WifiNetDevice::GetRemoteStationManager() const [member function]
    cls.add_method('GetRemoteStationManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetName(std::string const name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('std::string const', 'name')], 
                   is_virtual=True)
    ## wifi-net-device.h: std::string ns3::WifiNetDevice::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## wifi-net-device.h: uint32_t ns3::WifiNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Ptr<ns3::Channel> ns3::WifiNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## wifi-net-device.h: uint16_t ns3::WifiNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## wifi-net-device.h: ns3::Ptr<ns3::Node> ns3::WifiNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType >', 'cb')], 
                   is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3YansErrorRateModel_methods(root_module, cls):
    ## yans-error-rate-model.h: ns3::YansErrorRateModel::YansErrorRateModel(ns3::YansErrorRateModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansErrorRateModel const &', 'arg0')])
    ## yans-error-rate-model.h: static ns3::TypeId ns3::YansErrorRateModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## yans-error-rate-model.h: ns3::YansErrorRateModel::YansErrorRateModel() [constructor]
    cls.add_constructor([])
    ## yans-error-rate-model.h: double ns3::YansErrorRateModel::GetChunkSuccessRate(ns3::WifiMode mode, double snr, uint32_t nbits) const [member function]
    cls.add_method('GetChunkSuccessRate', 
                   'double', 
                   [param('ns3::WifiMode', 'mode'), param('double', 'snr'), param('uint32_t', 'nbits')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3YansWifiChannel_methods(root_module, cls):
    ## yans-wifi-channel.h: ns3::YansWifiChannel::YansWifiChannel(ns3::YansWifiChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansWifiChannel const &', 'arg0')])
    ## yans-wifi-channel.h: static ns3::TypeId ns3::YansWifiChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## yans-wifi-channel.h: ns3::YansWifiChannel::YansWifiChannel() [constructor]
    cls.add_constructor([])
    ## yans-wifi-channel.h: uint32_t ns3::YansWifiChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-channel.h: ns3::Ptr<ns3::NetDevice> ns3::YansWifiChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-channel.h: void ns3::YansWifiChannel::Add(ns3::Ptr<ns3::YansWifiPhy> phy) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::YansWifiPhy >', 'phy')])
    ## yans-wifi-channel.h: void ns3::YansWifiChannel::SetPropagationLossModel(ns3::Ptr<ns3::PropagationLossModel> loss) [member function]
    cls.add_method('SetPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationLossModel >', 'loss')])
    ## yans-wifi-channel.h: void ns3::YansWifiChannel::SetPropagationDelayModel(ns3::Ptr<ns3::PropagationDelayModel> delay) [member function]
    cls.add_method('SetPropagationDelayModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationDelayModel >', 'delay')])
    ## yans-wifi-channel.h: void ns3::YansWifiChannel::Send(ns3::Ptr<ns3::YansWifiPhy> sender, ns3::Ptr<const ns3::Packet> packet, double txPowerDbm, ns3::WifiMode wifiMode, ns3::WifiPreamble preamble) const [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::YansWifiPhy >', 'sender'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('double', 'txPowerDbm'), param('ns3::WifiMode', 'wifiMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_const=True)
    return

def register_Ns3AarfWifiManager_methods(root_module, cls):
    ## aarf-wifi-manager.h: ns3::AarfWifiManager::AarfWifiManager(ns3::AarfWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AarfWifiManager const &', 'arg0')])
    ## aarf-wifi-manager.h: static ns3::TypeId ns3::AarfWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aarf-wifi-manager.h: ns3::AarfWifiManager::AarfWifiManager() [constructor]
    cls.add_constructor([])
    ## aarf-wifi-manager.h: ns3::WifiRemoteStation * ns3::AarfWifiManager::CreateStation() [member function]
    cls.add_method('CreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    ## ssid.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeSsidChecker() [free function]
    module.add_function('MakeSsidChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## wifi-mode.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeWifiModeChecker() [free function]
    module.add_function('MakeWifiModeChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
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

