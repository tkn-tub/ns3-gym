from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## wifi-preamble.h: ns3::WifiPreamble [enumeration]
    module.add_enum('WifiPreamble', ['WIFI_PREAMBLE_LONG', 'WIFI_PREAMBLE_SHORT'])
    ## wifi-phy-standard.h: ns3::WifiPhyStandard [enumeration]
    module.add_enum('WifiPhyStandard', ['WIFI_PHY_STANDARD_80211a', 'WIFI_PHY_STANDARD_holland'])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation [class]
    module.add_class('WifiRemoteStation', allow_subclassing=True)
    ## wifi-phy.h: ns3::WifiPhyListener [class]
    module.add_class('WifiPhyListener', allow_subclassing=True)
    ## rraa-wifi-manager.h: ns3::ThresholdsItem [struct]
    module.add_class('ThresholdsItem')
    ## supported-rates.h: ns3::SupportedRates [class]
    module.add_class('SupportedRates')
    ## ideal-wifi-manager.h: ns3::IdealWifiRemoteStation [class]
    module.add_class('IdealWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## wifi-mode.h: ns3::WifiModeChecker [class]
    module.add_class('WifiModeChecker', parent=root_module['ns3::AttributeChecker'])
    ## wifi-mode.h: ns3::WifiModeFactory [class]
    module.add_class('WifiModeFactory')
    ## onoe-wifi-manager.h: ns3::OnoeWifiRemoteStation [class]
    module.add_class('OnoeWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## rraa-wifi-manager.h: ns3::RraaWifiRemoteStation [class]
    module.add_class('RraaWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## ssid.h: ns3::Ssid [class]
    module.add_class('Ssid')
    ## wifi-mode.h: ns3::WifiMode [class]
    module.add_class('WifiMode')
    ## wifi-mode.h: ns3::WifiMode::ModulationType [enumeration]
    module.add_enum('ModulationType', ['BPSK', 'QAM'], outer_class=root_module['ns3::WifiMode'])
    ## ssid.h: ns3::SsidValue [class]
    module.add_class('SsidValue', parent=root_module['ns3::AttributeValue'])
    ## ssid.h: ns3::SsidChecker [class]
    module.add_class('SsidChecker', parent=root_module['ns3::AttributeChecker'])
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiRemoteStation [class]
    module.add_class('ConstantRateWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## propagation-loss-model.h: ns3::PropagationLossModel [class]
    module.add_class('PropagationLossModel', parent=root_module['ns3::Object'])
    ## arf-wifi-manager.h: ns3::ArfWifiRemoteStation [class]
    module.add_class('ArfWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## wifi-mac.h: ns3::WifiMac [class]
    module.add_class('WifiMac', parent=root_module['ns3::Object'])
    ## nqap-wifi-mac.h: ns3::NqapWifiMac [class]
    module.add_class('NqapWifiMac', parent=root_module['ns3::WifiMac'])
    ## composite-propagation-loss-model.h: ns3::CompositePropagationLossModel [class]
    module.add_class('CompositePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## amrr-wifi-manager.h: ns3::AmrrWifiRemoteStation [class]
    module.add_class('AmrrWifiRemoteStation', parent=root_module['ns3::WifiRemoteStation'])
    ## propagation-loss-model.h: ns3::FriisPropagationLossModel [class]
    module.add_class('FriisPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## nqsta-wifi-mac.h: ns3::NqstaWifiMac [class]
    module.add_class('NqstaWifiMac', parent=root_module['ns3::WifiMac'])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationManager [class]
    module.add_class('WifiRemoteStationManager', parent=root_module['ns3::Object'])
    ## wifi-mode.h: ns3::WifiModeValue [class]
    module.add_class('WifiModeValue', parent=root_module['ns3::AttributeValue'])
    ## onoe-wifi-manager.h: ns3::OnoeWifiManager [class]
    module.add_class('OnoeWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## wifi-phy.h: ns3::WifiPhy [class]
    module.add_class('WifiPhy', parent=root_module['ns3::Object'])
    ## wifi-phy.h: ns3::WifiPhy::State [enumeration]
    module.add_enum('State', ['SYNC', 'TX', 'CCA_BUSY', 'IDLE'], outer_class=root_module['ns3::WifiPhy'])
    ## wifi-channel.h: ns3::WifiChannel [class]
    module.add_class('WifiChannel', parent=root_module['ns3::Channel'])
    ## propagation-loss-model.h: ns3::LogDistancePropagationLossModel [class]
    module.add_class('LogDistancePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiManager [class]
    module.add_class('ConstantRateWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## aarf-wifi-manager.h: ns3::AarfWifiRemoteStation [class]
    module.add_class('AarfWifiRemoteStation', parent=root_module['ns3::ArfWifiRemoteStation'])
    ## propagation-delay-model.h: ns3::PropagationDelayModel [class]
    module.add_class('PropagationDelayModel', parent=root_module['ns3::Object'])
    ## adhoc-wifi-mac.h: ns3::AdhocWifiMac [class]
    module.add_class('AdhocWifiMac', parent=root_module['ns3::WifiMac'])
    ## jakes-propagation-loss-model.h: ns3::JakesPropagationLossModel [class]
    module.add_class('JakesPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## wifi-net-device.h: ns3::WifiNetDevice [class]
    module.add_class('WifiNetDevice', parent=root_module['ns3::NetDevice'])
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel [class]
    module.add_class('ConstantSpeedPropagationDelayModel', parent=root_module['ns3::PropagationDelayModel'])
    ## propagation-loss-model.h: ns3::RandomPropagationLossModel [class]
    module.add_class('RandomPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel [class]
    module.add_class('RandomPropagationDelayModel', parent=root_module['ns3::PropagationDelayModel'])
    ## amrr-wifi-manager.h: ns3::AmrrWifiManager [class]
    module.add_class('AmrrWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## rraa-wifi-manager.h: ns3::RraaWifiManager [class]
    module.add_class('RraaWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## ideal-wifi-manager.h: ns3::IdealWifiManager [class]
    module.add_class('IdealWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## arf-wifi-manager.h: ns3::ArfWifiManager [class]
    module.add_class('ArfWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## aarf-wifi-manager.h: ns3::AarfWifiManager [class]
    module.add_class('AarfWifiManager', parent=root_module['ns3::ArfWifiManager'])
    
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
    

def register_methods(root_module):
    register_Ns3WifiRemoteStation_methods(root_module, root_module['ns3::WifiRemoteStation'])
    register_Ns3WifiPhyListener_methods(root_module, root_module['ns3::WifiPhyListener'])
    register_Ns3ThresholdsItem_methods(root_module, root_module['ns3::ThresholdsItem'])
    register_Ns3SupportedRates_methods(root_module, root_module['ns3::SupportedRates'])
    register_Ns3IdealWifiRemoteStation_methods(root_module, root_module['ns3::IdealWifiRemoteStation'])
    register_Ns3WifiModeChecker_methods(root_module, root_module['ns3::WifiModeChecker'])
    register_Ns3WifiModeFactory_methods(root_module, root_module['ns3::WifiModeFactory'])
    register_Ns3OnoeWifiRemoteStation_methods(root_module, root_module['ns3::OnoeWifiRemoteStation'])
    register_Ns3RraaWifiRemoteStation_methods(root_module, root_module['ns3::RraaWifiRemoteStation'])
    register_Ns3Ssid_methods(root_module, root_module['ns3::Ssid'])
    register_Ns3WifiMode_methods(root_module, root_module['ns3::WifiMode'])
    register_Ns3SsidValue_methods(root_module, root_module['ns3::SsidValue'])
    register_Ns3SsidChecker_methods(root_module, root_module['ns3::SsidChecker'])
    register_Ns3ConstantRateWifiRemoteStation_methods(root_module, root_module['ns3::ConstantRateWifiRemoteStation'])
    register_Ns3PropagationLossModel_methods(root_module, root_module['ns3::PropagationLossModel'])
    register_Ns3ArfWifiRemoteStation_methods(root_module, root_module['ns3::ArfWifiRemoteStation'])
    register_Ns3WifiMac_methods(root_module, root_module['ns3::WifiMac'])
    register_Ns3NqapWifiMac_methods(root_module, root_module['ns3::NqapWifiMac'])
    register_Ns3CompositePropagationLossModel_methods(root_module, root_module['ns3::CompositePropagationLossModel'])
    register_Ns3AmrrWifiRemoteStation_methods(root_module, root_module['ns3::AmrrWifiRemoteStation'])
    register_Ns3FriisPropagationLossModel_methods(root_module, root_module['ns3::FriisPropagationLossModel'])
    register_Ns3NqstaWifiMac_methods(root_module, root_module['ns3::NqstaWifiMac'])
    register_Ns3WifiRemoteStationManager_methods(root_module, root_module['ns3::WifiRemoteStationManager'])
    register_Ns3WifiModeValue_methods(root_module, root_module['ns3::WifiModeValue'])
    register_Ns3OnoeWifiManager_methods(root_module, root_module['ns3::OnoeWifiManager'])
    register_Ns3WifiPhy_methods(root_module, root_module['ns3::WifiPhy'])
    register_Ns3WifiChannel_methods(root_module, root_module['ns3::WifiChannel'])
    register_Ns3LogDistancePropagationLossModel_methods(root_module, root_module['ns3::LogDistancePropagationLossModel'])
    register_Ns3ConstantRateWifiManager_methods(root_module, root_module['ns3::ConstantRateWifiManager'])
    register_Ns3AarfWifiRemoteStation_methods(root_module, root_module['ns3::AarfWifiRemoteStation'])
    register_Ns3PropagationDelayModel_methods(root_module, root_module['ns3::PropagationDelayModel'])
    register_Ns3AdhocWifiMac_methods(root_module, root_module['ns3::AdhocWifiMac'])
    register_Ns3JakesPropagationLossModel_methods(root_module, root_module['ns3::JakesPropagationLossModel'])
    register_Ns3WifiNetDevice_methods(root_module, root_module['ns3::WifiNetDevice'])
    register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, root_module['ns3::ConstantSpeedPropagationDelayModel'])
    register_Ns3RandomPropagationLossModel_methods(root_module, root_module['ns3::RandomPropagationLossModel'])
    register_Ns3RandomPropagationDelayModel_methods(root_module, root_module['ns3::RandomPropagationDelayModel'])
    register_Ns3AmrrWifiManager_methods(root_module, root_module['ns3::AmrrWifiManager'])
    register_Ns3RraaWifiManager_methods(root_module, root_module['ns3::RraaWifiManager'])
    register_Ns3IdealWifiManager_methods(root_module, root_module['ns3::IdealWifiManager'])
    register_Ns3ArfWifiManager_methods(root_module, root_module['ns3::ArfWifiManager'])
    register_Ns3AarfWifiManager_methods(root_module, root_module['ns3::AarfWifiManager'])
    return

def register_Ns3WifiRemoteStation_methods(root_module, cls):
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
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('uint32_t', 'fullPacketSize')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::GetDataMode(ns3::Ptr<const ns3::Packet> packet, uint32_t fullPacketSize) [member function]
    cls.add_method('GetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('uint32_t', 'fullPacketSize')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStation::GetRtsMode(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('GetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')])
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
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::NeedRtsRetransmission(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedRtsRetransmission', 
                   'bool', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::NeedDataRetransmission(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedDataRetransmission', 
                   'bool', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::NeedFragmentation(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedFragmentation', 
                   'bool', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStation::GetNFragments(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('GetNFragments', 
                   'uint32_t', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStation::GetFragmentSize(ns3::Ptr<const ns3::Packet> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('GetFragmentSize', 
                   'uint32_t', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('uint32_t', 'fragmentNumber')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStation::IsLastFragment(ns3::Ptr<const ns3::Packet> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('IsLastFragment', 
                   'bool', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('uint32_t', 'fragmentNumber')], 
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

def register_Ns3WifiPhyListener_methods(root_module, cls):
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
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyCcaBusyStart(ns3::Time duration) [member function]
    cls.add_method('NotifyCcaBusyStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    cls.add_constructor([])
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
    cls.add_constructor([])
    return

def register_Ns3SupportedRates_methods(root_module, cls):
    ## supported-rates.h: ns3::SupportedRates::SupportedRates(ns3::SupportedRates const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SupportedRates&', 'arg0', is_const=True)])
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
    cls.add_output_stream_operator()
    return

def register_Ns3IdealWifiRemoteStation_methods(root_module, cls):
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

def register_Ns3WifiModeChecker_methods(root_module, cls):
    cls.add_constructor([])
    return

def register_Ns3WifiModeFactory_methods(root_module, cls):
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

def register_Ns3OnoeWifiRemoteStation_methods(root_module, cls):
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

def register_Ns3RraaWifiRemoteStation_methods(root_module, cls):
    ## rraa-wifi-manager.h: ns3::RraaWifiRemoteStation::RraaWifiRemoteStation(ns3::Ptr<ns3::RraaWifiManager> stations) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::RraaWifiManager >', 'stations')])
    ## rraa-wifi-manager.h: bool ns3::RraaWifiRemoteStation::NeedRts(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('NeedRts', 
                   'bool', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')], 
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

def register_Ns3Ssid_methods(root_module, cls):
    ## ssid.h: ns3::Ssid::Ssid(ns3::Ssid const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ssid&', 'arg0', is_const=True)])
    ## ssid.h: ns3::Ssid::Ssid() [constructor]
    cls.add_constructor([])
    ## ssid.h: ns3::Ssid::Ssid(char const * ssid) [constructor]
    cls.add_constructor([param('char *', 'ssid', transfer_ownership=False, is_const=True)])
    ## ssid.h: ns3::Ssid::Ssid(char const * ssid, uint8_t length) [constructor]
    cls.add_constructor([param('char *', 'ssid', transfer_ownership=False, is_const=True), param('uint8_t', 'length')])
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
                   [param('ns3::Ssid&', 'o', is_const=True)], 
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
    cls.add_output_stream_operator()
    return

def register_Ns3WifiMode_methods(root_module, cls):
    ## wifi-mode.h: ns3::WifiMode::WifiMode(ns3::WifiMode const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMode&', 'arg0', is_const=True)])
    ## wifi-mode.h: ns3::WifiMode::WifiMode() [constructor]
    cls.add_constructor([])
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
    cls.add_output_stream_operator()
    return

def register_Ns3SsidValue_methods(root_module, cls):
    ## ssid.h: ns3::SsidValue::SsidValue() [constructor]
    cls.add_constructor([])
    ## ssid.h: ns3::SsidValue::SsidValue(ns3::Ssid const & value) [constructor]
    cls.add_constructor([param('ns3::Ssid&', 'value', is_const=True)])
    ## ssid.h: void ns3::SsidValue::Set(ns3::Ssid const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ssid&', 'value', is_const=True)])
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

def register_Ns3SsidChecker_methods(root_module, cls):
    cls.add_constructor([])
    return

def register_Ns3ConstantRateWifiRemoteStation_methods(root_module, cls):
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

def register_Ns3PropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::PropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: double ns3::PropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    cls.add_constructor([])
    return

def register_Ns3ArfWifiRemoteStation_methods(root_module, cls):
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

def register_Ns3WifiMac_methods(root_module, cls):
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
                   [param('ns3::Time', 'pifs')])
    ## wifi-mac.h: void ns3::WifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')])
    ## wifi-mac.h: void ns3::WifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')])
    ## wifi-mac.h: void ns3::WifiMac::SetMaxPropagationDelay(ns3::Time delay) [member function]
    cls.add_method('SetMaxPropagationDelay', 
                   'void', 
                   [param('ns3::Time', 'delay')])
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
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
                   is_const=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
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
    ## wifi-mac.h: ns3::Mac48Address ns3::WifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
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
    ## wifi-mac.h: void ns3::WifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    cls.add_constructor([])
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
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## nqap-wifi-mac.h: void ns3::NqapWifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
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
    ## nqap-wifi-mac.h: ns3::Mac48Address ns3::NqapWifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
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

def register_Ns3CompositePropagationLossModel_methods(root_module, cls):
    ## composite-propagation-loss-model.h: static ns3::TypeId ns3::CompositePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## composite-propagation-loss-model.h: ns3::CompositePropagationLossModel::CompositePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## composite-propagation-loss-model.h: double ns3::CompositePropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## composite-propagation-loss-model.h: void ns3::CompositePropagationLossModel::AddPropagationLossModel(ns3::Ptr<ns3::PropagationLossModel> pl) [member function]
    cls.add_method('AddPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationLossModel >', 'pl')])
    ## composite-propagation-loss-model.h: void ns3::CompositePropagationLossModel::AddDefaults() [member function]
    cls.add_method('AddDefaults', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3AmrrWifiRemoteStation_methods(root_module, cls):
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
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
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
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## nqsta-wifi-mac.h: void ns3::NqstaWifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
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
    ## nqsta-wifi-mac.h: ns3::Mac48Address ns3::NqstaWifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
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

def register_Ns3WifiRemoteStationManager_methods(root_module, cls):
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
                   '__gnu_cxx::__normal_iterator< const ns3::WifiMode*, std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > > >', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: __gnu_cxx::__normal_iterator<const ns3::WifiMode*,std::vector<ns3::WifiMode, std::allocator<ns3::WifiMode> > > ns3::WifiRemoteStationManager::EndBasicModes() const [member function]
    cls.add_method('EndBasicModes', 
                   '__gnu_cxx::__normal_iterator< const ns3::WifiMode*, std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > > >', 
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

def register_Ns3WifiModeValue_methods(root_module, cls):
    ## wifi-mode.h: ns3::WifiModeValue::WifiModeValue() [constructor]
    cls.add_constructor([])
    ## wifi-mode.h: ns3::WifiModeValue::WifiModeValue(ns3::WifiMode const & value) [constructor]
    cls.add_constructor([param('ns3::WifiMode&', 'value', is_const=True)])
    ## wifi-mode.h: void ns3::WifiModeValue::Set(ns3::WifiMode const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::WifiMode&', 'value', is_const=True)])
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

def register_Ns3OnoeWifiManager_methods(root_module, cls):
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

def register_Ns3WifiPhy_methods(root_module, cls):
    ## wifi-phy.h: static ns3::TypeId ns3::WifiPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: ns3::WifiPhy::WifiPhy() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h: void ns3::WifiPhy::SetStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('SetStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')])
    ## wifi-phy.h: void ns3::WifiPhy::SetRxNoise(double ratio) [member function]
    cls.add_method('SetRxNoise', 
                   'void', 
                   [param('double', 'ratio')])
    ## wifi-phy.h: void ns3::WifiPhy::SetTxPowerStart(double start) [member function]
    cls.add_method('SetTxPowerStart', 
                   'void', 
                   [param('double', 'start')])
    ## wifi-phy.h: void ns3::WifiPhy::SetTxPowerEnd(double end) [member function]
    cls.add_method('SetTxPowerEnd', 
                   'void', 
                   [param('double', 'end')])
    ## wifi-phy.h: void ns3::WifiPhy::SetNTxPower(uint32_t n) [member function]
    cls.add_method('SetNTxPower', 
                   'void', 
                   [param('uint32_t', 'n')])
    ## wifi-phy.h: void ns3::WifiPhy::SetTxGain(double gain) [member function]
    cls.add_method('SetTxGain', 
                   'void', 
                   [param('double', 'gain')])
    ## wifi-phy.h: void ns3::WifiPhy::SetRxGain(double gain) [member function]
    cls.add_method('SetRxGain', 
                   'void', 
                   [param('double', 'gain')])
    ## wifi-phy.h: void ns3::WifiPhy::SetEdThreshold(double threshold) [member function]
    cls.add_method('SetEdThreshold', 
                   'void', 
                   [param('double', 'threshold')])
    ## wifi-phy.h: double ns3::WifiPhy::GetRxNoise() const [member function]
    cls.add_method('GetRxNoise', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: double ns3::WifiPhy::GetTxPowerStart() const [member function]
    cls.add_method('GetTxPowerStart', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: double ns3::WifiPhy::GetTxPowerEnd() const [member function]
    cls.add_method('GetTxPowerEnd', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: uint32_t ns3::WifiPhy::GetNTxPower() const [member function]
    cls.add_method('GetNTxPower', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: double ns3::WifiPhy::GetTxGain() const [member function]
    cls.add_method('GetTxGain', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: double ns3::WifiPhy::GetRxGain() const [member function]
    cls.add_method('GetRxGain', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: double ns3::WifiPhy::GetEdThreshold() const [member function]
    cls.add_method('GetEdThreshold', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: void ns3::WifiPhy::SetChannel(ns3::Ptr<ns3::WifiChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiChannel >', 'channel')])
    ## wifi-phy.h: void ns3::WifiPhy::SetReceiveOkCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::WifiMode, ns3::WifiPreamble, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::WifiMode, ns3::WifiPreamble, ns3::empty, ns3::empty >', 'callback')])
    ## wifi-phy.h: void ns3::WifiPhy::SetReceiveErrorCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## wifi-phy.h: void ns3::WifiPhy::SendPacket(ns3::Ptr<const ns3::Packet> packet, ns3::WifiMode mode, ns3::WifiPreamble preamble, uint8_t txPowerLevel) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble'), param('uint8_t', 'txPowerLevel')])
    ## wifi-phy.h: void ns3::WifiPhy::RegisterListener(ns3::WifiPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::WifiPhyListener *', 'listener')])
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [])
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [])
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [])
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateSync() [member function]
    cls.add_method('IsStateSync', 
                   'bool', 
                   [])
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [])
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetStateDuration() [member function]
    cls.add_method('GetStateDuration', 
                   'ns3::Time', 
                   [])
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetDelayUntilIdle() [member function]
    cls.add_method('GetDelayUntilIdle', 
                   'ns3::Time', 
                   [])
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetLastRxStartTime() const [member function]
    cls.add_method('GetLastRxStartTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::CalculateTxDuration(uint32_t size, ns3::WifiMode payloadMode, ns3::WifiPreamble preamble) const [member function]
    cls.add_method('CalculateTxDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_const=True)
    ## wifi-phy.h: uint32_t ns3::WifiPhy::GetNModes() const [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: ns3::WifiMode ns3::WifiPhy::GetMode(uint32_t mode) const [member function]
    cls.add_method('GetMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'mode')], 
                   is_const=True)
    ## wifi-phy.h: double ns3::WifiPhy::CalculateSnr(ns3::WifiMode txMode, double ber) const [member function]
    cls.add_method('CalculateSnr', 
                   'double', 
                   [param('ns3::WifiMode', 'txMode'), param('double', 'ber')], 
                   is_const=True)
    ## wifi-phy.h: void ns3::WifiPhy::StartReceivePacket(ns3::Ptr<ns3::Packet> packet, double rxPowerDbm, ns3::WifiMode mode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('StartReceivePacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDbm'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble')])
    ## wifi-phy.h: ns3::Ptr<ns3::WifiChannel> ns3::WifiPhy::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::WifiChannel >', 
                   [], 
                   is_const=True)
    ## wifi-phy.h: void ns3::WifiPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3WifiChannel_methods(root_module, cls):
    ## wifi-channel.h: static ns3::TypeId ns3::WifiChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-channel.h: ns3::WifiChannel::WifiChannel() [constructor]
    cls.add_constructor([])
    ## wifi-channel.h: uint32_t ns3::WifiChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-channel.h: ns3::Ptr<ns3::NetDevice> ns3::WifiChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## wifi-channel.h: void ns3::WifiChannel::SetPropagationLossModel(ns3::Ptr<ns3::PropagationLossModel> loss) [member function]
    cls.add_method('SetPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationLossModel >', 'loss')])
    ## wifi-channel.h: void ns3::WifiChannel::SetPropagationDelayModel(ns3::Ptr<ns3::PropagationDelayModel> delay) [member function]
    cls.add_method('SetPropagationDelayModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationDelayModel >', 'delay')])
    ## wifi-channel.h: void ns3::WifiChannel::Add(ns3::Ptr<ns3::NetDevice> device, ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device'), param('ns3::Ptr< ns3::WifiPhy >', 'phy')])
    ## wifi-channel.h: void ns3::WifiChannel::Send(ns3::Ptr<ns3::WifiPhy> sender, ns3::Ptr<const ns3::Packet> packet, double txPowerDbm, ns3::WifiMode wifiMode, ns3::WifiPreamble preamble) const [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'sender'), param('ns3::Ptr< const ns3::Packet >', 'packet'), param('double', 'txPowerDbm'), param('ns3::WifiMode', 'wifiMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_const=True)
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
    ## propagation-loss-model.h: void ns3::LogDistancePropagationLossModel::SetReferenceModel(ns3::Ptr<ns3::PropagationLossModel> model) [member function]
    cls.add_method('SetReferenceModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationLossModel >', 'model')])
    ## propagation-loss-model.h: void ns3::LogDistancePropagationLossModel::SetReferenceDistance(double referenceDistance) [member function]
    cls.add_method('SetReferenceDistance', 
                   'void', 
                   [param('double', 'referenceDistance')])
    ## propagation-loss-model.h: double ns3::LogDistancePropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3ConstantRateWifiManager_methods(root_module, cls):
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

def register_Ns3AarfWifiRemoteStation_methods(root_module, cls):
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

def register_Ns3PropagationDelayModel_methods(root_module, cls):
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
    cls.add_constructor([])
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
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::Enqueue(ns3::Ptr<const ns3::Packet> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
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
    ## adhoc-wifi-mac.h: ns3::Mac48Address ns3::AdhocWifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
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
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
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
    ## jakes-propagation-loss-model.h: double ns3::JakesPropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## jakes-propagation-loss-model.h: void ns3::JakesPropagationLossModel::SetNRays(uint8_t nRays) [member function]
    cls.add_method('SetNRays', 
                   'void', 
                   [param('uint8_t', 'nRays')])
    ## jakes-propagation-loss-model.h: void ns3::JakesPropagationLossModel::SetNOscillators(uint8_t nOscillators) [member function]
    cls.add_method('SetNOscillators', 
                   'void', 
                   [param('uint8_t', 'nOscillators')])
    return

def register_Ns3WifiNetDevice_methods(root_module, cls):
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
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetChannel(ns3::Ptr<ns3::WifiChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiChannel >', 'channel')])
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
                   [param('std::string', 'name', is_const=True)], 
                   is_virtual=True)
    ## wifi-net-device.h: std::string ns3::WifiNetDevice::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t', 'index', is_const=True)], 
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
                   [param('uint16_t', 'mtu', is_const=True)], 
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
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetMulticast() const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::MakeMulticastAddress(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('MakeMulticastAddress', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address&', 'dest', is_const=True), param('uint16_t', 'protocolNumber')], 
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
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet >, unsigned short, ns3::Address const&, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address&', 'source', is_const=True), param('ns3::Address&', 'dest', is_const=True), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet >, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType >', 'cb')], 
                   is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::SupportsPromiscuous() const [member function]
    cls.add_method('SupportsPromiscuous', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, cls):
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

def register_Ns3RandomPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::RandomPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::RandomPropagationLossModel::RandomPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::RandomPropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3RandomPropagationDelayModel_methods(root_module, cls):
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

def register_Ns3AmrrWifiManager_methods(root_module, cls):
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

def register_Ns3RraaWifiManager_methods(root_module, cls):
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

def register_Ns3IdealWifiManager_methods(root_module, cls):
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

def register_Ns3ArfWifiManager_methods(root_module, cls):
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

def register_Ns3AarfWifiManager_methods(root_module, cls):
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
    ## wifi-mode.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeWifiModeChecker() [free function]
    module.add_function('MakeWifiModeChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ssid.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeSsidChecker() [free function]
    module.add_function('MakeSsidChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
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
    return

