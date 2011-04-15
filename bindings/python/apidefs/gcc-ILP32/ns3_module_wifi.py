from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## wifi-mac-header.h: ns3::WifiMacType [enumeration]
    module.add_enum('WifiMacType', ['WIFI_MAC_CTL_RTS', 'WIFI_MAC_CTL_CTS', 'WIFI_MAC_CTL_ACK', 'WIFI_MAC_CTL_BACKREQ', 'WIFI_MAC_CTL_BACKRESP', 'WIFI_MAC_MGT_BEACON', 'WIFI_MAC_MGT_ASSOCIATION_REQUEST', 'WIFI_MAC_MGT_ASSOCIATION_RESPONSE', 'WIFI_MAC_MGT_DISASSOCIATION', 'WIFI_MAC_MGT_REASSOCIATION_REQUEST', 'WIFI_MAC_MGT_REASSOCIATION_RESPONSE', 'WIFI_MAC_MGT_PROBE_REQUEST', 'WIFI_MAC_MGT_PROBE_RESPONSE', 'WIFI_MAC_MGT_AUTHENTICATION', 'WIFI_MAC_MGT_DEAUTHENTICATION', 'WIFI_MAC_MGT_ACTION', 'WIFI_MAC_MGT_ACTION_NO_ACK', 'WIFI_MAC_MGT_MULTIHOP_ACTION', 'WIFI_MAC_DATA', 'WIFI_MAC_DATA_CFACK', 'WIFI_MAC_DATA_CFPOLL', 'WIFI_MAC_DATA_CFACK_CFPOLL', 'WIFI_MAC_DATA_NULL', 'WIFI_MAC_DATA_NULL_CFACK', 'WIFI_MAC_DATA_NULL_CFPOLL', 'WIFI_MAC_DATA_NULL_CFACK_CFPOLL', 'WIFI_MAC_QOSDATA', 'WIFI_MAC_QOSDATA_CFACK', 'WIFI_MAC_QOSDATA_CFPOLL', 'WIFI_MAC_QOSDATA_CFACK_CFPOLL', 'WIFI_MAC_QOSDATA_NULL', 'WIFI_MAC_QOSDATA_NULL_CFPOLL', 'WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL'])
    ## wifi-preamble.h: ns3::WifiPreamble [enumeration]
    module.add_enum('WifiPreamble', ['WIFI_PREAMBLE_LONG', 'WIFI_PREAMBLE_SHORT'])
    ## wifi-mode.h: ns3::WifiModulationClass [enumeration]
    module.add_enum('WifiModulationClass', ['WIFI_MOD_CLASS_UNKNOWN', 'WIFI_MOD_CLASS_IR', 'WIFI_MOD_CLASS_FHSS', 'WIFI_MOD_CLASS_DSSS', 'WIFI_MOD_CLASS_ERP_PBCC', 'WIFI_MOD_CLASS_DSSS_OFDM', 'WIFI_MOD_CLASS_ERP_OFDM', 'WIFI_MOD_CLASS_OFDM', 'WIFI_MOD_CLASS_HT'])
    ## wifi-phy-standard.h: ns3::WifiPhyStandard [enumeration]
    module.add_enum('WifiPhyStandard', ['WIFI_PHY_STANDARD_80211a', 'WIFI_PHY_STANDARD_80211b', 'WIFI_PHY_STANDARD_80211g', 'WIFI_PHY_STANDARD_80211_10Mhz', 'WIFI_PHY_STANDARD_80211_5Mhz', 'WIFI_PHY_STANDARD_holland', 'WIFI_PHY_STANDARD_80211p_CCH', 'WIFI_PHY_STANDARD_80211p_SCH'])
    ## qos-utils.h: ns3::AcIndex [enumeration]
    module.add_enum('AcIndex', ['AC_BE', 'AC_BK', 'AC_VI', 'AC_VO', 'AC_BE_NQOS', 'AC_UNDEF'])
    ## ctrl-headers.h: ns3::BlockAckType [enumeration]
    module.add_enum('BlockAckType', ['BASIC_BLOCK_ACK', 'COMPRESSED_BLOCK_ACK', 'MULTI_TID_BLOCK_ACK'])
    ## qos-tag.h: ns3::UserPriority [enumeration]
    module.add_enum('UserPriority', ['UP_BK', 'UP_BE', 'UP_EE', 'UP_CL', 'UP_VI', 'UP_VO', 'UP_NC'])
    ## wifi-mode.h: ns3::WifiCodeRate [enumeration]
    module.add_enum('WifiCodeRate', ['WIFI_CODE_RATE_UNDEFINED', 'WIFI_CODE_RATE_3_4', 'WIFI_CODE_RATE_2_3', 'WIFI_CODE_RATE_1_2'])
    ## edca-txop-n.h: ns3::TypeOfStation [enumeration]
    module.add_enum('TypeOfStation', ['STA', 'AP', 'ADHOC_STA', 'MESH'])
    ## athstats-helper.h: ns3::AthstatsHelper [class]
    module.add_class('AthstatsHelper')
    ## block-ack-manager.h: ns3::Bar [struct]
    module.add_class('Bar')
    ## block-ack-agreement.h: ns3::BlockAckAgreement [class]
    module.add_class('BlockAckAgreement')
    ## block-ack-cache.h: ns3::BlockAckCache [class]
    module.add_class('BlockAckCache')
    ## block-ack-manager.h: ns3::BlockAckManager [class]
    module.add_class('BlockAckManager')
    ## capability-information.h: ns3::CapabilityInformation [class]
    module.add_class('CapabilityInformation')
    ## dcf-manager.h: ns3::DcfManager [class]
    module.add_class('DcfManager')
    ## dcf-manager.h: ns3::DcfState [class]
    module.add_class('DcfState', allow_subclassing=True)
    ## dsss-error-rate-model.h: ns3::DsssErrorRateModel [class]
    module.add_class('DsssErrorRateModel')
    ## interference-helper.h: ns3::InterferenceHelper [class]
    module.add_class('InterferenceHelper')
    ## interference-helper.h: ns3::InterferenceHelper::SnrPer [struct]
    module.add_class('SnrPer', outer_class=root_module['ns3::InterferenceHelper'])
    ## mac-low.h: ns3::MacLowBlockAckEventListener [class]
    module.add_class('MacLowBlockAckEventListener', allow_subclassing=True)
    ## mac-low.h: ns3::MacLowDcfListener [class]
    module.add_class('MacLowDcfListener', allow_subclassing=True)
    ## mac-low.h: ns3::MacLowTransmissionListener [class]
    module.add_class('MacLowTransmissionListener', allow_subclassing=True)
    ## mac-low.h: ns3::MacLowTransmissionParameters [class]
    module.add_class('MacLowTransmissionParameters')
    ## mac-rx-middle.h: ns3::MacRxMiddle [class]
    module.add_class('MacRxMiddle')
    ## originator-block-ack-agreement.h: ns3::OriginatorBlockAckAgreement [class]
    module.add_class('OriginatorBlockAckAgreement', parent=root_module['ns3::BlockAckAgreement'])
    ## originator-block-ack-agreement.h: ns3::OriginatorBlockAckAgreement::State [enumeration]
    module.add_enum('State', ['PENDING', 'ESTABLISHED', 'INACTIVE', 'UNSUCCESSFUL'], outer_class=root_module['ns3::OriginatorBlockAckAgreement'])
    ## minstrel-wifi-manager.h: ns3::RateInfo [struct]
    module.add_class('RateInfo')
    ## status-code.h: ns3::StatusCode [class]
    module.add_class('StatusCode')
    ## wifi-helper.h: ns3::WifiHelper [class]
    module.add_class('WifiHelper')
    ## wifi-helper.h: ns3::WifiMacHelper [class]
    module.add_class('WifiMacHelper', allow_subclassing=True)
    ## wifi-mode.h: ns3::WifiMode [class]
    module.add_class('WifiMode')
    ## wifi-mode.h: ns3::WifiModeFactory [class]
    module.add_class('WifiModeFactory')
    ## wifi-helper.h: ns3::WifiPhyHelper [class]
    module.add_class('WifiPhyHelper', allow_subclassing=True)
    ## wifi-phy.h: ns3::WifiPhyListener [class]
    module.add_class('WifiPhyListener', allow_subclassing=True)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation [struct]
    module.add_class('WifiRemoteStation')
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationInfo [class]
    module.add_class('WifiRemoteStationInfo')
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationState [struct]
    module.add_class('WifiRemoteStationState')
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationState [enumeration]
    module.add_enum('', ['BRAND_NEW', 'DISASSOC', 'WAIT_ASSOC_TX_OK', 'GOT_ASSOC_TX_OK'], outer_class=root_module['ns3::WifiRemoteStationState'])
    ## yans-wifi-helper.h: ns3::YansWifiChannelHelper [class]
    module.add_class('YansWifiChannelHelper')
    ## yans-wifi-helper.h: ns3::YansWifiPhyHelper [class]
    module.add_class('YansWifiPhyHelper', parent=[root_module['ns3::WifiPhyHelper'], root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## yans-wifi-helper.h: ns3::YansWifiPhyHelper::SupportedPcapDataLinkTypes [enumeration]
    module.add_enum('SupportedPcapDataLinkTypes', ['DLT_IEEE802_11', 'DLT_PRISM_HEADER', 'DLT_IEEE802_11_RADIO'], outer_class=root_module['ns3::YansWifiPhyHelper'])
    ## mgt-headers.h: ns3::MgtAddBaRequestHeader [class]
    module.add_class('MgtAddBaRequestHeader', parent=root_module['ns3::Header'])
    ## mgt-headers.h: ns3::MgtAddBaResponseHeader [class]
    module.add_class('MgtAddBaResponseHeader', parent=root_module['ns3::Header'])
    ## mgt-headers.h: ns3::MgtAssocRequestHeader [class]
    module.add_class('MgtAssocRequestHeader', parent=root_module['ns3::Header'])
    ## mgt-headers.h: ns3::MgtAssocResponseHeader [class]
    module.add_class('MgtAssocResponseHeader', parent=root_module['ns3::Header'])
    ## mgt-headers.h: ns3::MgtDelBaHeader [class]
    module.add_class('MgtDelBaHeader', parent=root_module['ns3::Header'])
    ## mgt-headers.h: ns3::MgtProbeRequestHeader [class]
    module.add_class('MgtProbeRequestHeader', parent=root_module['ns3::Header'])
    ## mgt-headers.h: ns3::MgtProbeResponseHeader [class]
    module.add_class('MgtProbeResponseHeader', parent=root_module['ns3::Header'])
    ## nqos-wifi-mac-helper.h: ns3::NqosWifiMacHelper [class]
    module.add_class('NqosWifiMacHelper', parent=root_module['ns3::WifiMacHelper'])
    ## qos-tag.h: ns3::QosTag [class]
    module.add_class('QosTag', parent=root_module['ns3::Tag'])
    ## qos-wifi-mac-helper.h: ns3::QosWifiMacHelper [class]
    module.add_class('QosWifiMacHelper', parent=root_module['ns3::WifiMacHelper'])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::InterferenceHelper::Event, ns3::empty, ns3::DefaultDeleter<ns3::InterferenceHelper::Event> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::InterferenceHelper::Event', 'ns3::empty', 'ns3::DefaultDeleter<ns3::InterferenceHelper::Event>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::WifiInformationElement', 'ns3::empty', 'ns3::DefaultDeleter<ns3::WifiInformationElement>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## mgt-headers.h: ns3::WifiActionHeader [class]
    module.add_class('WifiActionHeader', parent=root_module['ns3::Header'])
    ## mgt-headers.h: ns3::WifiActionHeader::CategoryValue [enumeration]
    module.add_enum('CategoryValue', ['BLOCK_ACK', 'MESH_PEERING_MGT', 'MESH_LINK_METRIC', 'MESH_PATH_SELECTION', 'MESH_INTERWORKING', 'MESH_RESOURCE_COORDINATION', 'MESH_PROXY_FORWARDING'], outer_class=root_module['ns3::WifiActionHeader'])
    ## mgt-headers.h: ns3::WifiActionHeader::PeerLinkMgtActionValue [enumeration]
    module.add_enum('PeerLinkMgtActionValue', ['PEER_LINK_OPEN', 'PEER_LINK_CONFIRM', 'PEER_LINK_CLOSE'], outer_class=root_module['ns3::WifiActionHeader'])
    ## mgt-headers.h: ns3::WifiActionHeader::LinkMetricActionValue [enumeration]
    module.add_enum('LinkMetricActionValue', ['LINK_METRIC_REQUEST', 'LINK_METRIC_REPORT'], outer_class=root_module['ns3::WifiActionHeader'])
    ## mgt-headers.h: ns3::WifiActionHeader::PathSelectionActionValue [enumeration]
    module.add_enum('PathSelectionActionValue', ['PATH_SELECTION'], outer_class=root_module['ns3::WifiActionHeader'])
    ## mgt-headers.h: ns3::WifiActionHeader::InterworkActionValue [enumeration]
    module.add_enum('InterworkActionValue', ['PORTAL_ANNOUNCEMENT'], outer_class=root_module['ns3::WifiActionHeader'])
    ## mgt-headers.h: ns3::WifiActionHeader::ResourceCoordinationActionValue [enumeration]
    module.add_enum('ResourceCoordinationActionValue', ['CONGESTION_CONTROL_NOTIFICATION', 'MDA_SETUP_REQUEST', 'MDA_SETUP_REPLY', 'MDAOP_ADVERTISMENT_REQUEST', 'MDAOP_ADVERTISMENTS', 'MDAOP_SET_TEARDOWN', 'BEACON_TIMING_REQUEST', 'BEACON_TIMING_RESPONSE', 'TBTT_ADJUSTMENT_REQUEST', 'MESH_CHANNEL_SWITCH_ANNOUNCEMENT'], outer_class=root_module['ns3::WifiActionHeader'])
    ## mgt-headers.h: ns3::WifiActionHeader::BlockAckActionValue [enumeration]
    module.add_enum('BlockAckActionValue', ['BLOCK_ACK_ADDBA_REQUEST', 'BLOCK_ACK_ADDBA_RESPONSE', 'BLOCK_ACK_DELBA'], outer_class=root_module['ns3::WifiActionHeader'])
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue [union]
    module.add_class('ActionValue', outer_class=root_module['ns3::WifiActionHeader'])
    ## wifi-information-element.h: ns3::WifiInformationElement [class]
    module.add_class('WifiInformationElement', parent=root_module['ns3::SimpleRefCount< ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >'])
    ## wifi-information-element-vector.h: ns3::WifiInformationElementVector [class]
    module.add_class('WifiInformationElementVector', parent=root_module['ns3::Header'])
    ## wifi-mac.h: ns3::WifiMac [class]
    module.add_class('WifiMac', parent=root_module['ns3::Object'])
    ## wifi-mac-header.h: ns3::WifiMacHeader [class]
    module.add_class('WifiMacHeader', parent=root_module['ns3::Header'])
    ## wifi-mac-header.h: ns3::WifiMacHeader::QosAckPolicy [enumeration]
    module.add_enum('QosAckPolicy', ['NORMAL_ACK', 'NO_ACK', 'NO_EXPLICIT_ACK', 'BLOCK_ACK'], outer_class=root_module['ns3::WifiMacHeader'])
    ## wifi-mac-header.h: ns3::WifiMacHeader::AddressType [enumeration]
    module.add_enum('AddressType', ['ADDR1', 'ADDR2', 'ADDR3', 'ADDR4'], outer_class=root_module['ns3::WifiMacHeader'])
    ## wifi-phy.h: ns3::WifiPhy [class]
    module.add_class('WifiPhy', parent=root_module['ns3::Object'])
    ## wifi-phy.h: ns3::WifiPhy::State [enumeration]
    module.add_enum('State', ['IDLE', 'CCA_BUSY', 'TX', 'RX', 'SWITCHING'], outer_class=root_module['ns3::WifiPhy'])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationManager [class]
    module.add_class('WifiRemoteStationManager', parent=root_module['ns3::Object'])
    ## yans-wifi-phy.h: ns3::YansWifiPhy [class]
    module.add_class('YansWifiPhy', parent=root_module['ns3::WifiPhy'])
    ## aarf-wifi-manager.h: ns3::AarfWifiManager [class]
    module.add_class('AarfWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## aarfcd-wifi-manager.h: ns3::AarfcdWifiManager [class]
    module.add_class('AarfcdWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## amrr-wifi-manager.h: ns3::AmrrWifiManager [class]
    module.add_class('AmrrWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## amsdu-subframe-header.h: ns3::AmsduSubframeHeader [class]
    module.add_class('AmsduSubframeHeader', parent=root_module['ns3::Header'])
    ## arf-wifi-manager.h: ns3::ArfWifiManager [class]
    module.add_class('ArfWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## athstats-helper.h: ns3::AthstatsWifiTraceSink [class]
    module.add_class('AthstatsWifiTraceSink', parent=root_module['ns3::Object'])
    ## cara-wifi-manager.h: ns3::CaraWifiManager [class]
    module.add_class('CaraWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiManager [class]
    module.add_class('ConstantRateWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## ctrl-headers.h: ns3::CtrlBAckRequestHeader [class]
    module.add_class('CtrlBAckRequestHeader', parent=root_module['ns3::Header'])
    ## ctrl-headers.h: ns3::CtrlBAckResponseHeader [class]
    module.add_class('CtrlBAckResponseHeader', parent=root_module['ns3::Header'])
    ## dcf.h: ns3::Dcf [class]
    module.add_class('Dcf', parent=root_module['ns3::Object'])
    ## edca-txop-n.h: ns3::EdcaTxopN [class]
    module.add_class('EdcaTxopN', parent=root_module['ns3::Dcf'])
    ## error-rate-model.h: ns3::ErrorRateModel [class]
    module.add_class('ErrorRateModel', parent=root_module['ns3::Object'])
    ## supported-rates.h: ns3::ExtendedSupportedRatesIE [class]
    module.add_class('ExtendedSupportedRatesIE', parent=root_module['ns3::WifiInformationElement'])
    ## ideal-wifi-manager.h: ns3::IdealWifiManager [class]
    module.add_class('IdealWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## mac-low.h: ns3::MacLow [class]
    module.add_class('MacLow', parent=root_module['ns3::Object'])
    ## mgt-headers.h: ns3::MgtBeaconHeader [class]
    module.add_class('MgtBeaconHeader', parent=root_module['ns3::MgtProbeResponseHeader'])
    ## minstrel-wifi-manager.h: ns3::MinstrelWifiManager [class]
    module.add_class('MinstrelWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## msdu-aggregator.h: ns3::MsduAggregator [class]
    module.add_class('MsduAggregator', parent=root_module['ns3::Object'])
    ## nist-error-rate-model.h: ns3::NistErrorRateModel [class]
    module.add_class('NistErrorRateModel', parent=root_module['ns3::ErrorRateModel'])
    ## onoe-wifi-manager.h: ns3::OnoeWifiManager [class]
    module.add_class('OnoeWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## regular-wifi-mac.h: ns3::RegularWifiMac [class]
    module.add_class('RegularWifiMac', parent=root_module['ns3::WifiMac'])
    ## rraa-wifi-manager.h: ns3::RraaWifiManager [class]
    module.add_class('RraaWifiManager', parent=root_module['ns3::WifiRemoteStationManager'])
    ## ssid.h: ns3::Ssid [class]
    module.add_class('Ssid', parent=root_module['ns3::WifiInformationElement'])
    ## ssid.h: ns3::SsidChecker [class]
    module.add_class('SsidChecker', parent=root_module['ns3::AttributeChecker'])
    ## ssid.h: ns3::SsidValue [class]
    module.add_class('SsidValue', parent=root_module['ns3::AttributeValue'])
    ## sta-wifi-mac.h: ns3::StaWifiMac [class]
    module.add_class('StaWifiMac', parent=root_module['ns3::RegularWifiMac'])
    ## supported-rates.h: ns3::SupportedRates [class]
    module.add_class('SupportedRates', parent=root_module['ns3::WifiInformationElement'])
    ## wifi-channel.h: ns3::WifiChannel [class]
    module.add_class('WifiChannel', parent=root_module['ns3::Channel'])
    ## wifi-mode.h: ns3::WifiModeChecker [class]
    module.add_class('WifiModeChecker', parent=root_module['ns3::AttributeChecker'])
    ## wifi-mode.h: ns3::WifiModeValue [class]
    module.add_class('WifiModeValue', parent=root_module['ns3::AttributeValue'])
    ## wifi-net-device.h: ns3::WifiNetDevice [class]
    module.add_class('WifiNetDevice', parent=root_module['ns3::NetDevice'])
    ## yans-error-rate-model.h: ns3::YansErrorRateModel [class]
    module.add_class('YansErrorRateModel', parent=root_module['ns3::ErrorRateModel'])
    ## yans-wifi-channel.h: ns3::YansWifiChannel [class]
    module.add_class('YansWifiChannel', parent=root_module['ns3::WifiChannel'])
    ## adhoc-wifi-mac.h: ns3::AdhocWifiMac [class]
    module.add_class('AdhocWifiMac', parent=root_module['ns3::RegularWifiMac'])
    ## ap-wifi-mac.h: ns3::ApWifiMac [class]
    module.add_class('ApWifiMac', parent=root_module['ns3::RegularWifiMac'])
    ## dca-txop.h: ns3::DcaTxop [class]
    module.add_class('DcaTxop', parent=root_module['ns3::Dcf'])
    module.add_container('ns3::WifiModeList', 'ns3::WifiMode', container_type='vector')
    typehandlers.add_type_alias('std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > >', 'ns3::WifiModeList')
    typehandlers.add_type_alias('std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > >*', 'ns3::WifiModeList*')
    typehandlers.add_type_alias('std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > >&', 'ns3::WifiModeList&')
    typehandlers.add_type_alias('uint8_t', 'ns3::WifiInformationElementId')
    typehandlers.add_type_alias('uint8_t*', 'ns3::WifiInformationElementId*')
    typehandlers.add_type_alias('uint8_t&', 'ns3::WifiInformationElementId&')
    typehandlers.add_type_alias('__gnu_cxx::__normal_iterator< ns3::WifiMode const *, std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > > >', 'ns3::WifiModeListIterator')
    typehandlers.add_type_alias('__gnu_cxx::__normal_iterator< ns3::WifiMode const *, std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > > >*', 'ns3::WifiModeListIterator*')
    typehandlers.add_type_alias('__gnu_cxx::__normal_iterator< ns3::WifiMode const *, std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > > >&', 'ns3::WifiModeListIterator&')
    typehandlers.add_type_alias('std::vector< ns3::RateInfo, std::allocator< ns3::RateInfo > >', 'ns3::MinstrelRate')
    typehandlers.add_type_alias('std::vector< ns3::RateInfo, std::allocator< ns3::RateInfo > >*', 'ns3::MinstrelRate*')
    typehandlers.add_type_alias('std::vector< ns3::RateInfo, std::allocator< ns3::RateInfo > >&', 'ns3::MinstrelRate&')
    typehandlers.add_type_alias('std::vector< std::vector< unsigned int, std::allocator< unsigned int > >, std::allocator< std::vector< unsigned int, std::allocator< unsigned int > > > >', 'ns3::SampleRate')
    typehandlers.add_type_alias('std::vector< std::vector< unsigned int, std::allocator< unsigned int > >, std::allocator< std::vector< unsigned int, std::allocator< unsigned int > > > >*', 'ns3::SampleRate*')
    typehandlers.add_type_alias('std::vector< std::vector< unsigned int, std::allocator< unsigned int > >, std::allocator< std::vector< unsigned int, std::allocator< unsigned int > > > >&', 'ns3::SampleRate&')
    
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
    register_Ns3AthstatsHelper_methods(root_module, root_module['ns3::AthstatsHelper'])
    register_Ns3Bar_methods(root_module, root_module['ns3::Bar'])
    register_Ns3BlockAckAgreement_methods(root_module, root_module['ns3::BlockAckAgreement'])
    register_Ns3BlockAckCache_methods(root_module, root_module['ns3::BlockAckCache'])
    register_Ns3BlockAckManager_methods(root_module, root_module['ns3::BlockAckManager'])
    register_Ns3CapabilityInformation_methods(root_module, root_module['ns3::CapabilityInformation'])
    register_Ns3DcfManager_methods(root_module, root_module['ns3::DcfManager'])
    register_Ns3DcfState_methods(root_module, root_module['ns3::DcfState'])
    register_Ns3DsssErrorRateModel_methods(root_module, root_module['ns3::DsssErrorRateModel'])
    register_Ns3InterferenceHelper_methods(root_module, root_module['ns3::InterferenceHelper'])
    register_Ns3InterferenceHelperSnrPer_methods(root_module, root_module['ns3::InterferenceHelper::SnrPer'])
    register_Ns3MacLowBlockAckEventListener_methods(root_module, root_module['ns3::MacLowBlockAckEventListener'])
    register_Ns3MacLowDcfListener_methods(root_module, root_module['ns3::MacLowDcfListener'])
    register_Ns3MacLowTransmissionListener_methods(root_module, root_module['ns3::MacLowTransmissionListener'])
    register_Ns3MacLowTransmissionParameters_methods(root_module, root_module['ns3::MacLowTransmissionParameters'])
    register_Ns3MacRxMiddle_methods(root_module, root_module['ns3::MacRxMiddle'])
    register_Ns3OriginatorBlockAckAgreement_methods(root_module, root_module['ns3::OriginatorBlockAckAgreement'])
    register_Ns3RateInfo_methods(root_module, root_module['ns3::RateInfo'])
    register_Ns3StatusCode_methods(root_module, root_module['ns3::StatusCode'])
    register_Ns3WifiHelper_methods(root_module, root_module['ns3::WifiHelper'])
    register_Ns3WifiMacHelper_methods(root_module, root_module['ns3::WifiMacHelper'])
    register_Ns3WifiMode_methods(root_module, root_module['ns3::WifiMode'])
    register_Ns3WifiModeFactory_methods(root_module, root_module['ns3::WifiModeFactory'])
    register_Ns3WifiPhyHelper_methods(root_module, root_module['ns3::WifiPhyHelper'])
    register_Ns3WifiPhyListener_methods(root_module, root_module['ns3::WifiPhyListener'])
    register_Ns3WifiRemoteStation_methods(root_module, root_module['ns3::WifiRemoteStation'])
    register_Ns3WifiRemoteStationInfo_methods(root_module, root_module['ns3::WifiRemoteStationInfo'])
    register_Ns3WifiRemoteStationState_methods(root_module, root_module['ns3::WifiRemoteStationState'])
    register_Ns3YansWifiChannelHelper_methods(root_module, root_module['ns3::YansWifiChannelHelper'])
    register_Ns3YansWifiPhyHelper_methods(root_module, root_module['ns3::YansWifiPhyHelper'])
    register_Ns3MgtAddBaRequestHeader_methods(root_module, root_module['ns3::MgtAddBaRequestHeader'])
    register_Ns3MgtAddBaResponseHeader_methods(root_module, root_module['ns3::MgtAddBaResponseHeader'])
    register_Ns3MgtAssocRequestHeader_methods(root_module, root_module['ns3::MgtAssocRequestHeader'])
    register_Ns3MgtAssocResponseHeader_methods(root_module, root_module['ns3::MgtAssocResponseHeader'])
    register_Ns3MgtDelBaHeader_methods(root_module, root_module['ns3::MgtDelBaHeader'])
    register_Ns3MgtProbeRequestHeader_methods(root_module, root_module['ns3::MgtProbeRequestHeader'])
    register_Ns3MgtProbeResponseHeader_methods(root_module, root_module['ns3::MgtProbeResponseHeader'])
    register_Ns3NqosWifiMacHelper_methods(root_module, root_module['ns3::NqosWifiMacHelper'])
    register_Ns3QosTag_methods(root_module, root_module['ns3::QosTag'])
    register_Ns3QosWifiMacHelper_methods(root_module, root_module['ns3::QosWifiMacHelper'])
    register_Ns3WifiActionHeader_methods(root_module, root_module['ns3::WifiActionHeader'])
    register_Ns3WifiActionHeaderActionValue_methods(root_module, root_module['ns3::WifiActionHeader::ActionValue'])
    register_Ns3WifiInformationElement_methods(root_module, root_module['ns3::WifiInformationElement'])
    register_Ns3WifiInformationElementVector_methods(root_module, root_module['ns3::WifiInformationElementVector'])
    register_Ns3WifiMac_methods(root_module, root_module['ns3::WifiMac'])
    register_Ns3WifiMacHeader_methods(root_module, root_module['ns3::WifiMacHeader'])
    register_Ns3WifiPhy_methods(root_module, root_module['ns3::WifiPhy'])
    register_Ns3WifiRemoteStationManager_methods(root_module, root_module['ns3::WifiRemoteStationManager'])
    register_Ns3YansWifiPhy_methods(root_module, root_module['ns3::YansWifiPhy'])
    register_Ns3AarfWifiManager_methods(root_module, root_module['ns3::AarfWifiManager'])
    register_Ns3AarfcdWifiManager_methods(root_module, root_module['ns3::AarfcdWifiManager'])
    register_Ns3AmrrWifiManager_methods(root_module, root_module['ns3::AmrrWifiManager'])
    register_Ns3AmsduSubframeHeader_methods(root_module, root_module['ns3::AmsduSubframeHeader'])
    register_Ns3ArfWifiManager_methods(root_module, root_module['ns3::ArfWifiManager'])
    register_Ns3AthstatsWifiTraceSink_methods(root_module, root_module['ns3::AthstatsWifiTraceSink'])
    register_Ns3CaraWifiManager_methods(root_module, root_module['ns3::CaraWifiManager'])
    register_Ns3ConstantRateWifiManager_methods(root_module, root_module['ns3::ConstantRateWifiManager'])
    register_Ns3CtrlBAckRequestHeader_methods(root_module, root_module['ns3::CtrlBAckRequestHeader'])
    register_Ns3CtrlBAckResponseHeader_methods(root_module, root_module['ns3::CtrlBAckResponseHeader'])
    register_Ns3Dcf_methods(root_module, root_module['ns3::Dcf'])
    register_Ns3EdcaTxopN_methods(root_module, root_module['ns3::EdcaTxopN'])
    register_Ns3ErrorRateModel_methods(root_module, root_module['ns3::ErrorRateModel'])
    register_Ns3ExtendedSupportedRatesIE_methods(root_module, root_module['ns3::ExtendedSupportedRatesIE'])
    register_Ns3IdealWifiManager_methods(root_module, root_module['ns3::IdealWifiManager'])
    register_Ns3MacLow_methods(root_module, root_module['ns3::MacLow'])
    register_Ns3MgtBeaconHeader_methods(root_module, root_module['ns3::MgtBeaconHeader'])
    register_Ns3MinstrelWifiManager_methods(root_module, root_module['ns3::MinstrelWifiManager'])
    register_Ns3MsduAggregator_methods(root_module, root_module['ns3::MsduAggregator'])
    register_Ns3NistErrorRateModel_methods(root_module, root_module['ns3::NistErrorRateModel'])
    register_Ns3OnoeWifiManager_methods(root_module, root_module['ns3::OnoeWifiManager'])
    register_Ns3RegularWifiMac_methods(root_module, root_module['ns3::RegularWifiMac'])
    register_Ns3RraaWifiManager_methods(root_module, root_module['ns3::RraaWifiManager'])
    register_Ns3Ssid_methods(root_module, root_module['ns3::Ssid'])
    register_Ns3SsidChecker_methods(root_module, root_module['ns3::SsidChecker'])
    register_Ns3SsidValue_methods(root_module, root_module['ns3::SsidValue'])
    register_Ns3StaWifiMac_methods(root_module, root_module['ns3::StaWifiMac'])
    register_Ns3SupportedRates_methods(root_module, root_module['ns3::SupportedRates'])
    register_Ns3WifiChannel_methods(root_module, root_module['ns3::WifiChannel'])
    register_Ns3WifiModeChecker_methods(root_module, root_module['ns3::WifiModeChecker'])
    register_Ns3WifiModeValue_methods(root_module, root_module['ns3::WifiModeValue'])
    register_Ns3WifiNetDevice_methods(root_module, root_module['ns3::WifiNetDevice'])
    register_Ns3YansErrorRateModel_methods(root_module, root_module['ns3::YansErrorRateModel'])
    register_Ns3YansWifiChannel_methods(root_module, root_module['ns3::YansWifiChannel'])
    register_Ns3AdhocWifiMac_methods(root_module, root_module['ns3::AdhocWifiMac'])
    register_Ns3ApWifiMac_methods(root_module, root_module['ns3::ApWifiMac'])
    register_Ns3DcaTxop_methods(root_module, root_module['ns3::DcaTxop'])
    return

def register_Ns3AthstatsHelper_methods(root_module, cls):
    ## athstats-helper.h: ns3::AthstatsHelper::AthstatsHelper(ns3::AthstatsHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AthstatsHelper const &', 'arg0')])
    ## athstats-helper.h: ns3::AthstatsHelper::AthstatsHelper() [constructor]
    cls.add_constructor([])
    ## athstats-helper.h: void ns3::AthstatsHelper::EnableAthstats(std::string filename, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAthstats', 
                   'void', 
                   [param('std::string', 'filename'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')])
    ## athstats-helper.h: void ns3::AthstatsHelper::EnableAthstats(std::string filename, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('EnableAthstats', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## athstats-helper.h: void ns3::AthstatsHelper::EnableAthstats(std::string filename, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAthstats', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NetDeviceContainer', 'd')])
    ## athstats-helper.h: void ns3::AthstatsHelper::EnableAthstats(std::string filename, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAthstats', 
                   'void', 
                   [param('std::string', 'filename'), param('ns3::NodeContainer', 'n')])
    return

def register_Ns3Bar_methods(root_module, cls):
    ## block-ack-manager.h: ns3::Bar::Bar(ns3::Bar const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Bar const &', 'arg0')])
    ## block-ack-manager.h: ns3::Bar::Bar() [constructor]
    cls.add_constructor([])
    ## block-ack-manager.h: ns3::Bar::Bar(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address recipient, uint8_t tid, bool immediate) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('bool', 'immediate')])
    ## block-ack-manager.h: ns3::Bar::bar [variable]
    cls.add_instance_attribute('bar', 'ns3::Ptr< ns3::Packet const >', is_const=False)
    ## block-ack-manager.h: ns3::Bar::immediate [variable]
    cls.add_instance_attribute('immediate', 'bool', is_const=False)
    ## block-ack-manager.h: ns3::Bar::recipient [variable]
    cls.add_instance_attribute('recipient', 'ns3::Mac48Address', is_const=False)
    ## block-ack-manager.h: ns3::Bar::tid [variable]
    cls.add_instance_attribute('tid', 'uint8_t', is_const=False)
    return

def register_Ns3BlockAckAgreement_methods(root_module, cls):
    ## block-ack-agreement.h: ns3::BlockAckAgreement::BlockAckAgreement(ns3::BlockAckAgreement const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BlockAckAgreement const &', 'arg0')])
    ## block-ack-agreement.h: ns3::BlockAckAgreement::BlockAckAgreement() [constructor]
    cls.add_constructor([])
    ## block-ack-agreement.h: ns3::BlockAckAgreement::BlockAckAgreement(ns3::Mac48Address peer, uint8_t tid) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'peer'), param('uint8_t', 'tid')])
    ## block-ack-agreement.h: uint16_t ns3::BlockAckAgreement::GetBufferSize() const [member function]
    cls.add_method('GetBufferSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h: ns3::Mac48Address ns3::BlockAckAgreement::GetPeer() const [member function]
    cls.add_method('GetPeer', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h: uint16_t ns3::BlockAckAgreement::GetStartingSequence() const [member function]
    cls.add_method('GetStartingSequence', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h: uint16_t ns3::BlockAckAgreement::GetStartingSequenceControl() const [member function]
    cls.add_method('GetStartingSequenceControl', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h: uint8_t ns3::BlockAckAgreement::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h: uint16_t ns3::BlockAckAgreement::GetTimeout() const [member function]
    cls.add_method('GetTimeout', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h: bool ns3::BlockAckAgreement::IsAmsduSupported() const [member function]
    cls.add_method('IsAmsduSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h: bool ns3::BlockAckAgreement::IsImmediateBlockAck() const [member function]
    cls.add_method('IsImmediateBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h: void ns3::BlockAckAgreement::SetAmsduSupport(bool supported) [member function]
    cls.add_method('SetAmsduSupport', 
                   'void', 
                   [param('bool', 'supported')])
    ## block-ack-agreement.h: void ns3::BlockAckAgreement::SetBufferSize(uint16_t bufferSize) [member function]
    cls.add_method('SetBufferSize', 
                   'void', 
                   [param('uint16_t', 'bufferSize')])
    ## block-ack-agreement.h: void ns3::BlockAckAgreement::SetDelayedBlockAck() [member function]
    cls.add_method('SetDelayedBlockAck', 
                   'void', 
                   [])
    ## block-ack-agreement.h: void ns3::BlockAckAgreement::SetImmediateBlockAck() [member function]
    cls.add_method('SetImmediateBlockAck', 
                   'void', 
                   [])
    ## block-ack-agreement.h: void ns3::BlockAckAgreement::SetStartingSequence(uint16_t seq) [member function]
    cls.add_method('SetStartingSequence', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## block-ack-agreement.h: void ns3::BlockAckAgreement::SetTimeout(uint16_t timeout) [member function]
    cls.add_method('SetTimeout', 
                   'void', 
                   [param('uint16_t', 'timeout')])
    return

def register_Ns3BlockAckCache_methods(root_module, cls):
    ## block-ack-cache.h: ns3::BlockAckCache::BlockAckCache() [constructor]
    cls.add_constructor([])
    ## block-ack-cache.h: ns3::BlockAckCache::BlockAckCache(ns3::BlockAckCache const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BlockAckCache const &', 'arg0')])
    ## block-ack-cache.h: void ns3::BlockAckCache::FillBlockAckBitmap(ns3::CtrlBAckResponseHeader * blockAckHeader) [member function]
    cls.add_method('FillBlockAckBitmap', 
                   'void', 
                   [param('ns3::CtrlBAckResponseHeader *', 'blockAckHeader')])
    ## block-ack-cache.h: void ns3::BlockAckCache::Init(uint16_t winStart, uint16_t winSize) [member function]
    cls.add_method('Init', 
                   'void', 
                   [param('uint16_t', 'winStart'), param('uint16_t', 'winSize')])
    ## block-ack-cache.h: void ns3::BlockAckCache::UpdateWithBlockAckReq(uint16_t startingSeq) [member function]
    cls.add_method('UpdateWithBlockAckReq', 
                   'void', 
                   [param('uint16_t', 'startingSeq')])
    ## block-ack-cache.h: void ns3::BlockAckCache::UpdateWithMpdu(ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('UpdateWithMpdu', 
                   'void', 
                   [param('ns3::WifiMacHeader const *', 'hdr')])
    return

def register_Ns3BlockAckManager_methods(root_module, cls):
    ## block-ack-manager.h: ns3::BlockAckManager::BlockAckManager() [constructor]
    cls.add_constructor([])
    ## block-ack-manager.h: void ns3::BlockAckManager::CreateAgreement(ns3::MgtAddBaRequestHeader const * reqHdr, ns3::Mac48Address recipient) [member function]
    cls.add_method('CreateAgreement', 
                   'void', 
                   [param('ns3::MgtAddBaRequestHeader const *', 'reqHdr'), param('ns3::Mac48Address', 'recipient')])
    ## block-ack-manager.h: void ns3::BlockAckManager::DestroyAgreement(ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('DestroyAgreement', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## block-ack-manager.h: bool ns3::BlockAckManager::ExistsAgreement(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('ExistsAgreement', 
                   'bool', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## block-ack-manager.h: bool ns3::BlockAckManager::ExistsAgreementInState(ns3::Mac48Address recipient, uint8_t tid, ns3::OriginatorBlockAckAgreement::State state) const [member function]
    cls.add_method('ExistsAgreementInState', 
                   'bool', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('ns3::OriginatorBlockAckAgreement::State', 'state')], 
                   is_const=True)
    ## block-ack-manager.h: uint32_t ns3::BlockAckManager::GetNBufferedPackets(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('GetNBufferedPackets', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## block-ack-manager.h: uint32_t ns3::BlockAckManager::GetNRetryNeededPackets(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('GetNRetryNeededPackets', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## block-ack-manager.h: ns3::Ptr<ns3::Packet const> ns3::BlockAckManager::GetNextPacket(ns3::WifiMacHeader & hdr) [member function]
    cls.add_method('GetNextPacket', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [param('ns3::WifiMacHeader &', 'hdr')])
    ## block-ack-manager.h: uint32_t ns3::BlockAckManager::GetNextPacketSize() const [member function]
    cls.add_method('GetNextPacketSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## block-ack-manager.h: uint16_t ns3::BlockAckManager::GetSeqNumOfNextRetryPacket(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('GetSeqNumOfNextRetryPacket', 
                   'uint16_t', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## block-ack-manager.h: bool ns3::BlockAckManager::HasBar(ns3::Bar & bar) [member function]
    cls.add_method('HasBar', 
                   'bool', 
                   [param('ns3::Bar &', 'bar')])
    ## block-ack-manager.h: bool ns3::BlockAckManager::HasOtherFragments(uint16_t sequenceNumber) const [member function]
    cls.add_method('HasOtherFragments', 
                   'bool', 
                   [param('uint16_t', 'sequenceNumber')], 
                   is_const=True)
    ## block-ack-manager.h: bool ns3::BlockAckManager::HasPackets() const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [], 
                   is_const=True)
    ## block-ack-manager.h: void ns3::BlockAckManager::NotifyAgreementEstablished(ns3::Mac48Address recipient, uint8_t tid, uint16_t startingSeq) [member function]
    cls.add_method('NotifyAgreementEstablished', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('uint16_t', 'startingSeq')])
    ## block-ack-manager.h: void ns3::BlockAckManager::NotifyAgreementUnsuccessful(ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('NotifyAgreementUnsuccessful', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## block-ack-manager.h: void ns3::BlockAckManager::NotifyGotBlockAck(ns3::CtrlBAckResponseHeader const * blockAck, ns3::Mac48Address recipient) [member function]
    cls.add_method('NotifyGotBlockAck', 
                   'void', 
                   [param('ns3::CtrlBAckResponseHeader const *', 'blockAck'), param('ns3::Mac48Address', 'recipient')])
    ## block-ack-manager.h: void ns3::BlockAckManager::NotifyMpduTransmission(ns3::Mac48Address recipient, uint8_t tid, uint16_t nextSeqNumber) [member function]
    cls.add_method('NotifyMpduTransmission', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('uint16_t', 'nextSeqNumber')])
    ## block-ack-manager.h: void ns3::BlockAckManager::SetBlockAckInactivityCallback(ns3::Callback<void, ns3::Mac48Address, unsigned char, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetBlockAckInactivityCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Mac48Address, unsigned char, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## block-ack-manager.h: void ns3::BlockAckManager::SetBlockAckThreshold(uint8_t nPackets) [member function]
    cls.add_method('SetBlockAckThreshold', 
                   'void', 
                   [param('uint8_t', 'nPackets')])
    ## block-ack-manager.h: void ns3::BlockAckManager::SetBlockAckType(ns3::BlockAckType bAckType) [member function]
    cls.add_method('SetBlockAckType', 
                   'void', 
                   [param('ns3::BlockAckType', 'bAckType')])
    ## block-ack-manager.h: void ns3::BlockAckManager::SetBlockDestinationCallback(ns3::Callback<void, ns3::Mac48Address, unsigned char, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetBlockDestinationCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Mac48Address, unsigned char, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## block-ack-manager.h: void ns3::BlockAckManager::SetMaxPacketDelay(ns3::Time maxDelay) [member function]
    cls.add_method('SetMaxPacketDelay', 
                   'void', 
                   [param('ns3::Time', 'maxDelay')])
    ## block-ack-manager.h: void ns3::BlockAckManager::SetQueue(ns3::Ptr<ns3::WifiMacQueue> queue) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiMacQueue >', 'queue')])
    ## block-ack-manager.h: void ns3::BlockAckManager::SetTxMiddle(ns3::MacTxMiddle * txMiddle) [member function]
    cls.add_method('SetTxMiddle', 
                   'void', 
                   [param('ns3::MacTxMiddle *', 'txMiddle')])
    ## block-ack-manager.h: void ns3::BlockAckManager::SetUnblockDestinationCallback(ns3::Callback<void, ns3::Mac48Address, unsigned char, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetUnblockDestinationCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Mac48Address, unsigned char, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## block-ack-manager.h: void ns3::BlockAckManager::StorePacket(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const & hdr, ns3::Time tStamp) [member function]
    cls.add_method('StorePacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const &', 'hdr'), param('ns3::Time', 'tStamp')])
    ## block-ack-manager.h: bool ns3::BlockAckManager::SwitchToBlockAckIfNeeded(ns3::Mac48Address recipient, uint8_t tid, uint16_t startingSeq) [member function]
    cls.add_method('SwitchToBlockAckIfNeeded', 
                   'bool', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('uint16_t', 'startingSeq')])
    ## block-ack-manager.h: void ns3::BlockAckManager::TearDownBlockAck(ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('TearDownBlockAck', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## block-ack-manager.h: void ns3::BlockAckManager::UpdateAgreement(ns3::MgtAddBaResponseHeader const * respHdr, ns3::Mac48Address recipient) [member function]
    cls.add_method('UpdateAgreement', 
                   'void', 
                   [param('ns3::MgtAddBaResponseHeader const *', 'respHdr'), param('ns3::Mac48Address', 'recipient')])
    return

def register_Ns3CapabilityInformation_methods(root_module, cls):
    ## capability-information.h: ns3::CapabilityInformation::CapabilityInformation(ns3::CapabilityInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CapabilityInformation const &', 'arg0')])
    ## capability-information.h: ns3::CapabilityInformation::CapabilityInformation() [constructor]
    cls.add_constructor([])
    ## capability-information.h: ns3::Buffer::Iterator ns3::CapabilityInformation::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## capability-information.h: uint32_t ns3::CapabilityInformation::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## capability-information.h: bool ns3::CapabilityInformation::IsEss() const [member function]
    cls.add_method('IsEss', 
                   'bool', 
                   [], 
                   is_const=True)
    ## capability-information.h: bool ns3::CapabilityInformation::IsIbss() const [member function]
    cls.add_method('IsIbss', 
                   'bool', 
                   [], 
                   is_const=True)
    ## capability-information.h: ns3::Buffer::Iterator ns3::CapabilityInformation::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## capability-information.h: void ns3::CapabilityInformation::SetEss() [member function]
    cls.add_method('SetEss', 
                   'void', 
                   [])
    ## capability-information.h: void ns3::CapabilityInformation::SetIbss() [member function]
    cls.add_method('SetIbss', 
                   'void', 
                   [])
    return

def register_Ns3DcfManager_methods(root_module, cls):
    ## dcf-manager.h: ns3::DcfManager::DcfManager(ns3::DcfManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DcfManager const &', 'arg0')])
    ## dcf-manager.h: ns3::DcfManager::DcfManager() [constructor]
    cls.add_constructor([])
    ## dcf-manager.h: void ns3::DcfManager::Add(ns3::DcfState * dcf) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::DcfState *', 'dcf')])
    ## dcf-manager.h: ns3::Time ns3::DcfManager::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## dcf-manager.h: void ns3::DcfManager::NotifyAckTimeoutResetNow() [member function]
    cls.add_method('NotifyAckTimeoutResetNow', 
                   'void', 
                   [])
    ## dcf-manager.h: void ns3::DcfManager::NotifyAckTimeoutStartNow(ns3::Time duration) [member function]
    cls.add_method('NotifyAckTimeoutStartNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## dcf-manager.h: void ns3::DcfManager::NotifyCtsTimeoutResetNow() [member function]
    cls.add_method('NotifyCtsTimeoutResetNow', 
                   'void', 
                   [])
    ## dcf-manager.h: void ns3::DcfManager::NotifyCtsTimeoutStartNow(ns3::Time duration) [member function]
    cls.add_method('NotifyCtsTimeoutStartNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## dcf-manager.h: void ns3::DcfManager::NotifyMaybeCcaBusyStartNow(ns3::Time duration) [member function]
    cls.add_method('NotifyMaybeCcaBusyStartNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## dcf-manager.h: void ns3::DcfManager::NotifyNavResetNow(ns3::Time duration) [member function]
    cls.add_method('NotifyNavResetNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## dcf-manager.h: void ns3::DcfManager::NotifyNavStartNow(ns3::Time duration) [member function]
    cls.add_method('NotifyNavStartNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## dcf-manager.h: void ns3::DcfManager::NotifyRxEndErrorNow() [member function]
    cls.add_method('NotifyRxEndErrorNow', 
                   'void', 
                   [])
    ## dcf-manager.h: void ns3::DcfManager::NotifyRxEndOkNow() [member function]
    cls.add_method('NotifyRxEndOkNow', 
                   'void', 
                   [])
    ## dcf-manager.h: void ns3::DcfManager::NotifyRxStartNow(ns3::Time duration) [member function]
    cls.add_method('NotifyRxStartNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## dcf-manager.h: void ns3::DcfManager::NotifySwitchingStartNow(ns3::Time duration) [member function]
    cls.add_method('NotifySwitchingStartNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## dcf-manager.h: void ns3::DcfManager::NotifyTxStartNow(ns3::Time duration) [member function]
    cls.add_method('NotifyTxStartNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## dcf-manager.h: void ns3::DcfManager::RequestAccess(ns3::DcfState * state) [member function]
    cls.add_method('RequestAccess', 
                   'void', 
                   [param('ns3::DcfState *', 'state')])
    ## dcf-manager.h: void ns3::DcfManager::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')])
    ## dcf-manager.h: void ns3::DcfManager::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')])
    ## dcf-manager.h: void ns3::DcfManager::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')])
    ## dcf-manager.h: void ns3::DcfManager::SetupLowListener(ns3::Ptr<ns3::MacLow> low) [member function]
    cls.add_method('SetupLowListener', 
                   'void', 
                   [param('ns3::Ptr< ns3::MacLow >', 'low')])
    ## dcf-manager.h: void ns3::DcfManager::SetupPhyListener(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetupPhyListener', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')])
    return

def register_Ns3DcfState_methods(root_module, cls):
    ## dcf-manager.h: ns3::DcfState::DcfState(ns3::DcfState const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DcfState const &', 'arg0')])
    ## dcf-manager.h: ns3::DcfState::DcfState() [constructor]
    cls.add_constructor([])
    ## dcf-manager.h: uint32_t ns3::DcfState::GetAifsn() const [member function]
    cls.add_method('GetAifsn', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dcf-manager.h: uint32_t ns3::DcfState::GetCw() const [member function]
    cls.add_method('GetCw', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dcf-manager.h: uint32_t ns3::DcfState::GetCwMax() const [member function]
    cls.add_method('GetCwMax', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dcf-manager.h: uint32_t ns3::DcfState::GetCwMin() const [member function]
    cls.add_method('GetCwMin', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dcf-manager.h: bool ns3::DcfState::IsAccessRequested() const [member function]
    cls.add_method('IsAccessRequested', 
                   'bool', 
                   [], 
                   is_const=True)
    ## dcf-manager.h: void ns3::DcfState::ResetCw() [member function]
    cls.add_method('ResetCw', 
                   'void', 
                   [])
    ## dcf-manager.h: void ns3::DcfState::SetAifsn(uint32_t aifsn) [member function]
    cls.add_method('SetAifsn', 
                   'void', 
                   [param('uint32_t', 'aifsn')])
    ## dcf-manager.h: void ns3::DcfState::SetCwMax(uint32_t maxCw) [member function]
    cls.add_method('SetCwMax', 
                   'void', 
                   [param('uint32_t', 'maxCw')])
    ## dcf-manager.h: void ns3::DcfState::SetCwMin(uint32_t minCw) [member function]
    cls.add_method('SetCwMin', 
                   'void', 
                   [param('uint32_t', 'minCw')])
    ## dcf-manager.h: void ns3::DcfState::StartBackoffNow(uint32_t nSlots) [member function]
    cls.add_method('StartBackoffNow', 
                   'void', 
                   [param('uint32_t', 'nSlots')])
    ## dcf-manager.h: void ns3::DcfState::UpdateFailedCw() [member function]
    cls.add_method('UpdateFailedCw', 
                   'void', 
                   [])
    ## dcf-manager.h: void ns3::DcfState::DoNotifyAccessGranted() [member function]
    cls.add_method('DoNotifyAccessGranted', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## dcf-manager.h: void ns3::DcfState::DoNotifyChannelSwitching() [member function]
    cls.add_method('DoNotifyChannelSwitching', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## dcf-manager.h: void ns3::DcfState::DoNotifyCollision() [member function]
    cls.add_method('DoNotifyCollision', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## dcf-manager.h: void ns3::DcfState::DoNotifyInternalCollision() [member function]
    cls.add_method('DoNotifyInternalCollision', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3DsssErrorRateModel_methods(root_module, cls):
    ## dsss-error-rate-model.h: ns3::DsssErrorRateModel::DsssErrorRateModel() [constructor]
    cls.add_constructor([])
    ## dsss-error-rate-model.h: ns3::DsssErrorRateModel::DsssErrorRateModel(ns3::DsssErrorRateModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DsssErrorRateModel const &', 'arg0')])
    ## dsss-error-rate-model.h: static double ns3::DsssErrorRateModel::DqpskFunction(double x) [member function]
    cls.add_method('DqpskFunction', 
                   'double', 
                   [param('double', 'x')], 
                   is_static=True)
    ## dsss-error-rate-model.h: static double ns3::DsssErrorRateModel::GetDsssDbpskSuccessRate(double sinr, uint32_t nbits) [member function]
    cls.add_method('GetDsssDbpskSuccessRate', 
                   'double', 
                   [param('double', 'sinr'), param('uint32_t', 'nbits')], 
                   is_static=True)
    ## dsss-error-rate-model.h: static double ns3::DsssErrorRateModel::GetDsssDqpskCck11SuccessRate(double sinr, uint32_t nbits) [member function]
    cls.add_method('GetDsssDqpskCck11SuccessRate', 
                   'double', 
                   [param('double', 'sinr'), param('uint32_t', 'nbits')], 
                   is_static=True)
    ## dsss-error-rate-model.h: static double ns3::DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate(double sinr, uint32_t nbits) [member function]
    cls.add_method('GetDsssDqpskCck5_5SuccessRate', 
                   'double', 
                   [param('double', 'sinr'), param('uint32_t', 'nbits')], 
                   is_static=True)
    ## dsss-error-rate-model.h: static double ns3::DsssErrorRateModel::GetDsssDqpskSuccessRate(double sinr, uint32_t nbits) [member function]
    cls.add_method('GetDsssDqpskSuccessRate', 
                   'double', 
                   [param('double', 'sinr'), param('uint32_t', 'nbits')], 
                   is_static=True)
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
    ## interference-helper.h: static ns3::Time ns3::InterferenceHelper::CalculateTxDuration(uint32_t size, ns3::WifiMode payloadMode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('CalculateTxDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## interference-helper.h: void ns3::InterferenceHelper::EraseEvents() [member function]
    cls.add_method('EraseEvents', 
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
    ## interference-helper.h: double ns3::InterferenceHelper::GetNoiseFigure() const [member function]
    cls.add_method('GetNoiseFigure', 
                   'double', 
                   [], 
                   is_const=True)
    ## interference-helper.h: static uint32_t ns3::InterferenceHelper::GetPayloadDurationMicroSeconds(uint32_t size, ns3::WifiMode payloadMode) [member function]
    cls.add_method('GetPayloadDurationMicroSeconds', 
                   'uint32_t', 
                   [param('uint32_t', 'size'), param('ns3::WifiMode', 'payloadMode')], 
                   is_static=True)
    ## interference-helper.h: static uint32_t ns3::InterferenceHelper::GetPlcpHeaderDurationMicroSeconds(ns3::WifiMode payloadMode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpHeaderDurationMicroSeconds', 
                   'uint32_t', 
                   [param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## interference-helper.h: static ns3::WifiMode ns3::InterferenceHelper::GetPlcpHeaderMode(ns3::WifiMode payloadMode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpHeaderMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## interference-helper.h: static uint32_t ns3::InterferenceHelper::GetPlcpPreambleDurationMicroSeconds(ns3::WifiMode mode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpPreambleDurationMicroSeconds', 
                   'uint32_t', 
                   [param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## interference-helper.h: void ns3::InterferenceHelper::NotifyRxEnd() [member function]
    cls.add_method('NotifyRxEnd', 
                   'void', 
                   [])
    ## interference-helper.h: void ns3::InterferenceHelper::NotifyRxStart() [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [])
    ## interference-helper.h: void ns3::InterferenceHelper::SetErrorRateModel(ns3::Ptr<ns3::ErrorRateModel> rate) [member function]
    cls.add_method('SetErrorRateModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorRateModel >', 'rate')])
    ## interference-helper.h: void ns3::InterferenceHelper::SetNoiseFigure(double value) [member function]
    cls.add_method('SetNoiseFigure', 
                   'void', 
                   [param('double', 'value')])
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

def register_Ns3MacLowBlockAckEventListener_methods(root_module, cls):
    ## mac-low.h: ns3::MacLowBlockAckEventListener::MacLowBlockAckEventListener(ns3::MacLowBlockAckEventListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacLowBlockAckEventListener const &', 'arg0')])
    ## mac-low.h: ns3::MacLowBlockAckEventListener::MacLowBlockAckEventListener() [constructor]
    cls.add_constructor([])
    ## mac-low.h: void ns3::MacLowBlockAckEventListener::BlockAckInactivityTimeout(ns3::Mac48Address originator, uint8_t tid) [member function]
    cls.add_method('BlockAckInactivityTimeout', 
                   'void', 
                   [param('ns3::Mac48Address', 'originator'), param('uint8_t', 'tid')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3MacLowDcfListener_methods(root_module, cls):
    ## mac-low.h: ns3::MacLowDcfListener::MacLowDcfListener(ns3::MacLowDcfListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacLowDcfListener const &', 'arg0')])
    ## mac-low.h: ns3::MacLowDcfListener::MacLowDcfListener() [constructor]
    cls.add_constructor([])
    ## mac-low.h: void ns3::MacLowDcfListener::AckTimeoutReset() [member function]
    cls.add_method('AckTimeoutReset', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowDcfListener::AckTimeoutStart(ns3::Time duration) [member function]
    cls.add_method('AckTimeoutStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowDcfListener::CtsTimeoutReset() [member function]
    cls.add_method('CtsTimeoutReset', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowDcfListener::CtsTimeoutStart(ns3::Time duration) [member function]
    cls.add_method('CtsTimeoutStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowDcfListener::NavReset(ns3::Time duration) [member function]
    cls.add_method('NavReset', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowDcfListener::NavStart(ns3::Time duration) [member function]
    cls.add_method('NavStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3MacLowTransmissionListener_methods(root_module, cls):
    ## mac-low.h: ns3::MacLowTransmissionListener::MacLowTransmissionListener(ns3::MacLowTransmissionListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacLowTransmissionListener const &', 'arg0')])
    ## mac-low.h: ns3::MacLowTransmissionListener::MacLowTransmissionListener() [constructor]
    cls.add_constructor([])
    ## mac-low.h: void ns3::MacLowTransmissionListener::Cancel() [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowTransmissionListener::GotAck(double snr, ns3::WifiMode txMode) [member function]
    cls.add_method('GotAck', 
                   'void', 
                   [param('double', 'snr'), param('ns3::WifiMode', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowTransmissionListener::GotBlockAck(ns3::CtrlBAckResponseHeader const * blockAck, ns3::Mac48Address source) [member function]
    cls.add_method('GotBlockAck', 
                   'void', 
                   [param('ns3::CtrlBAckResponseHeader const *', 'blockAck'), param('ns3::Mac48Address', 'source')], 
                   is_virtual=True)
    ## mac-low.h: void ns3::MacLowTransmissionListener::GotCts(double snr, ns3::WifiMode txMode) [member function]
    cls.add_method('GotCts', 
                   'void', 
                   [param('double', 'snr'), param('ns3::WifiMode', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowTransmissionListener::MissedAck() [member function]
    cls.add_method('MissedAck', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowTransmissionListener::MissedBlockAck() [member function]
    cls.add_method('MissedBlockAck', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## mac-low.h: void ns3::MacLowTransmissionListener::MissedCts() [member function]
    cls.add_method('MissedCts', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## mac-low.h: void ns3::MacLowTransmissionListener::StartNext() [member function]
    cls.add_method('StartNext', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3MacLowTransmissionParameters_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## mac-low.h: ns3::MacLowTransmissionParameters::MacLowTransmissionParameters(ns3::MacLowTransmissionParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacLowTransmissionParameters const &', 'arg0')])
    ## mac-low.h: ns3::MacLowTransmissionParameters::MacLowTransmissionParameters() [constructor]
    cls.add_constructor([])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::DisableAck() [member function]
    cls.add_method('DisableAck', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::DisableNextData() [member function]
    cls.add_method('DisableNextData', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::DisableOverrideDurationId() [member function]
    cls.add_method('DisableOverrideDurationId', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::DisableRts() [member function]
    cls.add_method('DisableRts', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableAck() [member function]
    cls.add_method('EnableAck', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableBasicBlockAck() [member function]
    cls.add_method('EnableBasicBlockAck', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableCompressedBlockAck() [member function]
    cls.add_method('EnableCompressedBlockAck', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableFastAck() [member function]
    cls.add_method('EnableFastAck', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableMultiTidBlockAck() [member function]
    cls.add_method('EnableMultiTidBlockAck', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableNextData(uint32_t size) [member function]
    cls.add_method('EnableNextData', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableOverrideDurationId(ns3::Time durationId) [member function]
    cls.add_method('EnableOverrideDurationId', 
                   'void', 
                   [param('ns3::Time', 'durationId')])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableRts() [member function]
    cls.add_method('EnableRts', 
                   'void', 
                   [])
    ## mac-low.h: void ns3::MacLowTransmissionParameters::EnableSuperFastAck() [member function]
    cls.add_method('EnableSuperFastAck', 
                   'void', 
                   [])
    ## mac-low.h: ns3::Time ns3::MacLowTransmissionParameters::GetDurationId() const [member function]
    cls.add_method('GetDurationId', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mac-low.h: uint32_t ns3::MacLowTransmissionParameters::GetNextPacketSize() const [member function]
    cls.add_method('GetNextPacketSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::HasDurationId() const [member function]
    cls.add_method('HasDurationId', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::HasNextPacket() const [member function]
    cls.add_method('HasNextPacket', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::MustSendRts() const [member function]
    cls.add_method('MustSendRts', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::MustWaitAck() const [member function]
    cls.add_method('MustWaitAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::MustWaitBasicBlockAck() const [member function]
    cls.add_method('MustWaitBasicBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::MustWaitCompressedBlockAck() const [member function]
    cls.add_method('MustWaitCompressedBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::MustWaitFastAck() const [member function]
    cls.add_method('MustWaitFastAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::MustWaitMultiTidBlockAck() const [member function]
    cls.add_method('MustWaitMultiTidBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::MustWaitNormalAck() const [member function]
    cls.add_method('MustWaitNormalAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac-low.h: bool ns3::MacLowTransmissionParameters::MustWaitSuperFastAck() const [member function]
    cls.add_method('MustWaitSuperFastAck', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3MacRxMiddle_methods(root_module, cls):
    ## mac-rx-middle.h: ns3::MacRxMiddle::MacRxMiddle(ns3::MacRxMiddle const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacRxMiddle const &', 'arg0')])
    ## mac-rx-middle.h: ns3::MacRxMiddle::MacRxMiddle() [constructor]
    cls.add_constructor([])
    ## mac-rx-middle.h: void ns3::MacRxMiddle::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr')])
    ## mac-rx-middle.h: void ns3::MacRxMiddle::SetForwardCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::WifiMacHeader const*, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetForwardCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::WifiMacHeader const *, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    return

def register_Ns3OriginatorBlockAckAgreement_methods(root_module, cls):
    ## originator-block-ack-agreement.h: ns3::OriginatorBlockAckAgreement::OriginatorBlockAckAgreement(ns3::OriginatorBlockAckAgreement const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OriginatorBlockAckAgreement const &', 'arg0')])
    ## originator-block-ack-agreement.h: ns3::OriginatorBlockAckAgreement::OriginatorBlockAckAgreement() [constructor]
    cls.add_constructor([])
    ## originator-block-ack-agreement.h: ns3::OriginatorBlockAckAgreement::OriginatorBlockAckAgreement(ns3::Mac48Address recipient, uint8_t tid) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## originator-block-ack-agreement.h: void ns3::OriginatorBlockAckAgreement::CompleteExchange() [member function]
    cls.add_method('CompleteExchange', 
                   'void', 
                   [])
    ## originator-block-ack-agreement.h: bool ns3::OriginatorBlockAckAgreement::IsBlockAckRequestNeeded() const [member function]
    cls.add_method('IsBlockAckRequestNeeded', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h: bool ns3::OriginatorBlockAckAgreement::IsEstablished() const [member function]
    cls.add_method('IsEstablished', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h: bool ns3::OriginatorBlockAckAgreement::IsInactive() const [member function]
    cls.add_method('IsInactive', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h: bool ns3::OriginatorBlockAckAgreement::IsPending() const [member function]
    cls.add_method('IsPending', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h: bool ns3::OriginatorBlockAckAgreement::IsUnsuccessful() const [member function]
    cls.add_method('IsUnsuccessful', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h: void ns3::OriginatorBlockAckAgreement::NotifyMpduTransmission(uint16_t nextSeqNumber) [member function]
    cls.add_method('NotifyMpduTransmission', 
                   'void', 
                   [param('uint16_t', 'nextSeqNumber')])
    ## originator-block-ack-agreement.h: void ns3::OriginatorBlockAckAgreement::SetState(ns3::OriginatorBlockAckAgreement::State state) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::OriginatorBlockAckAgreement::State', 'state')])
    return

def register_Ns3RateInfo_methods(root_module, cls):
    ## minstrel-wifi-manager.h: ns3::RateInfo::RateInfo() [constructor]
    cls.add_constructor([])
    ## minstrel-wifi-manager.h: ns3::RateInfo::RateInfo(ns3::RateInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RateInfo const &', 'arg0')])
    ## minstrel-wifi-manager.h: ns3::RateInfo::adjustedRetryCount [variable]
    cls.add_instance_attribute('adjustedRetryCount', 'uint32_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::attemptHist [variable]
    cls.add_instance_attribute('attemptHist', 'uint64_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::ewmaProb [variable]
    cls.add_instance_attribute('ewmaProb', 'uint32_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::numRateAttempt [variable]
    cls.add_instance_attribute('numRateAttempt', 'uint32_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::numRateSuccess [variable]
    cls.add_instance_attribute('numRateSuccess', 'uint32_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::perfectTxTime [variable]
    cls.add_instance_attribute('perfectTxTime', 'ns3::Time', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::prevNumRateAttempt [variable]
    cls.add_instance_attribute('prevNumRateAttempt', 'uint32_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::prevNumRateSuccess [variable]
    cls.add_instance_attribute('prevNumRateSuccess', 'uint32_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::prob [variable]
    cls.add_instance_attribute('prob', 'uint32_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::retryCount [variable]
    cls.add_instance_attribute('retryCount', 'uint32_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::successHist [variable]
    cls.add_instance_attribute('successHist', 'uint64_t', is_const=False)
    ## minstrel-wifi-manager.h: ns3::RateInfo::throughput [variable]
    cls.add_instance_attribute('throughput', 'uint32_t', is_const=False)
    return

def register_Ns3StatusCode_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## status-code.h: ns3::StatusCode::StatusCode(ns3::StatusCode const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StatusCode const &', 'arg0')])
    ## status-code.h: ns3::StatusCode::StatusCode() [constructor]
    cls.add_constructor([])
    ## status-code.h: ns3::Buffer::Iterator ns3::StatusCode::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## status-code.h: uint32_t ns3::StatusCode::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## status-code.h: bool ns3::StatusCode::IsSuccess() const [member function]
    cls.add_method('IsSuccess', 
                   'bool', 
                   [], 
                   is_const=True)
    ## status-code.h: ns3::Buffer::Iterator ns3::StatusCode::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## status-code.h: void ns3::StatusCode::SetFailure() [member function]
    cls.add_method('SetFailure', 
                   'void', 
                   [])
    ## status-code.h: void ns3::StatusCode::SetSuccess() [member function]
    cls.add_method('SetSuccess', 
                   'void', 
                   [])
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
    ## wifi-helper.h: static void ns3::WifiHelper::EnableLogComponents() [member function]
    cls.add_method('EnableLogComponents', 
                   'void', 
                   [], 
                   is_static=True)
    ## wifi-helper.h: ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## wifi-helper.h: ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## wifi-helper.h: ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('std::string', 'nodeName')], 
                   is_const=True)
    ## wifi-helper.h: void ns3::WifiHelper::SetRemoteStationManager(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetRemoteStationManager', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## wifi-helper.h: void ns3::WifiHelper::SetStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('SetStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')])
    return

def register_Ns3WifiMacHelper_methods(root_module, cls):
    ## wifi-helper.h: ns3::WifiMacHelper::WifiMacHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h: ns3::WifiMacHelper::WifiMacHelper(ns3::WifiMacHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMacHelper const &', 'arg0')])
    ## wifi-helper.h: ns3::Ptr<ns3::WifiMac> ns3::WifiMacHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
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
    ## wifi-mode.h: ns3::WifiCodeRate ns3::WifiMode::GetCodeRate() const [member function]
    cls.add_method('GetCodeRate', 
                   'ns3::WifiCodeRate', 
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
    ## wifi-mode.h: ns3::WifiModulationClass ns3::WifiMode::GetModulationClass() const [member function]
    cls.add_method('GetModulationClass', 
                   'ns3::WifiModulationClass', 
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
    return

def register_Ns3WifiModeFactory_methods(root_module, cls):
    ## wifi-mode.h: ns3::WifiModeFactory::WifiModeFactory(ns3::WifiModeFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeFactory const &', 'arg0')])
    ## wifi-mode.h: static ns3::WifiMode ns3::WifiModeFactory::CreateWifiMode(std::string uniqueName, ns3::WifiModulationClass modClass, bool isMandatory, uint32_t bandwidth, uint32_t dataRate, ns3::WifiCodeRate codingRate, uint8_t constellationSize) [member function]
    cls.add_method('CreateWifiMode', 
                   'ns3::WifiMode', 
                   [param('std::string', 'uniqueName'), param('ns3::WifiModulationClass', 'modClass'), param('bool', 'isMandatory'), param('uint32_t', 'bandwidth'), param('uint32_t', 'dataRate'), param('ns3::WifiCodeRate', 'codingRate'), param('uint8_t', 'constellationSize')], 
                   is_static=True)
    return

def register_Ns3WifiPhyHelper_methods(root_module, cls):
    ## wifi-helper.h: ns3::WifiPhyHelper::WifiPhyHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h: ns3::WifiPhyHelper::WifiPhyHelper(ns3::WifiPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhyHelper const &', 'arg0')])
    ## wifi-helper.h: ns3::Ptr<ns3::WifiPhy> ns3::WifiPhyHelper::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WifiNetDevice> device) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::WifiNetDevice >', 'device')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3WifiPhyListener_methods(root_module, cls):
    ## wifi-phy.h: ns3::WifiPhyListener::WifiPhyListener() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h: ns3::WifiPhyListener::WifiPhyListener(ns3::WifiPhyListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhyListener const &', 'arg0')])
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyMaybeCcaBusyStart(ns3::Time duration) [member function]
    cls.add_method('NotifyMaybeCcaBusyStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyRxEndError() [member function]
    cls.add_method('NotifyRxEndError', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyRxEndOk() [member function]
    cls.add_method('NotifyRxEndOk', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyRxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifySwitchingStart(ns3::Time duration) [member function]
    cls.add_method('NotifySwitchingStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhyListener::NotifyTxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyTxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3WifiRemoteStation_methods(root_module, cls):
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation::WifiRemoteStation() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation::WifiRemoteStation(ns3::WifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStation const &', 'arg0')])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation::m_slrc [variable]
    cls.add_instance_attribute('m_slrc', 'uint32_t', is_const=False)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation::m_ssrc [variable]
    cls.add_instance_attribute('m_ssrc', 'uint32_t', is_const=False)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation::m_state [variable]
    cls.add_instance_attribute('m_state', 'ns3::WifiRemoteStationState *', is_const=False)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation::m_tid [variable]
    cls.add_instance_attribute('m_tid', 'uint8_t', is_const=False)
    return

def register_Ns3WifiRemoteStationInfo_methods(root_module, cls):
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationInfo::WifiRemoteStationInfo(ns3::WifiRemoteStationInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStationInfo const &', 'arg0')])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationInfo::WifiRemoteStationInfo() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h: double ns3::WifiRemoteStationInfo::GetFrameErrorRate() const [member function]
    cls.add_method('GetFrameErrorRate', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationInfo::NotifyTxFailed() [member function]
    cls.add_method('NotifyTxFailed', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationInfo::NotifyTxSuccess(uint32_t retryCounter) [member function]
    cls.add_method('NotifyTxSuccess', 
                   'void', 
                   [param('uint32_t', 'retryCounter')])
    return

def register_Ns3WifiRemoteStationState_methods(root_module, cls):
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationState::WifiRemoteStationState() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationState::WifiRemoteStationState(ns3::WifiRemoteStationState const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStationState const &', 'arg0')])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationState::m_address [variable]
    cls.add_instance_attribute('m_address', 'ns3::Mac48Address', is_const=False)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationState::m_info [variable]
    cls.add_instance_attribute('m_info', 'ns3::WifiRemoteStationInfo', is_const=False)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationState::m_operationalRateSet [variable]
    cls.add_instance_attribute('m_operationalRateSet', 'ns3::WifiModeList', is_const=False)
    return

def register_Ns3YansWifiChannelHelper_methods(root_module, cls):
    ## yans-wifi-helper.h: ns3::YansWifiChannelHelper::YansWifiChannelHelper(ns3::YansWifiChannelHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansWifiChannelHelper const &', 'arg0')])
    ## yans-wifi-helper.h: ns3::YansWifiChannelHelper::YansWifiChannelHelper() [constructor]
    cls.add_constructor([])
    ## yans-wifi-helper.h: void ns3::YansWifiChannelHelper::AddPropagationLoss(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('AddPropagationLoss', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## yans-wifi-helper.h: ns3::Ptr<ns3::YansWifiChannel> ns3::YansWifiChannelHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::YansWifiChannel >', 
                   [], 
                   is_const=True)
    ## yans-wifi-helper.h: static ns3::YansWifiChannelHelper ns3::YansWifiChannelHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::YansWifiChannelHelper', 
                   [], 
                   is_static=True)
    ## yans-wifi-helper.h: void ns3::YansWifiChannelHelper::SetPropagationDelay(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetPropagationDelay', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    return

def register_Ns3YansWifiPhyHelper_methods(root_module, cls):
    ## yans-wifi-helper.h: ns3::YansWifiPhyHelper::YansWifiPhyHelper(ns3::YansWifiPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansWifiPhyHelper const &', 'arg0')])
    ## yans-wifi-helper.h: ns3::YansWifiPhyHelper::YansWifiPhyHelper() [constructor]
    cls.add_constructor([])
    ## yans-wifi-helper.h: static ns3::YansWifiPhyHelper ns3::YansWifiPhyHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::YansWifiPhyHelper', 
                   [], 
                   is_static=True)
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::Set(std::string name, ns3::AttributeValue const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'v')])
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::SetChannel(ns3::Ptr<ns3::YansWifiChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::YansWifiChannel >', 'channel')])
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::SetChannel(std::string channelName) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('std::string', 'channelName')])
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::SetErrorRateModel(std::string name, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetErrorRateModel', 
                   'void', 
                   [param('std::string', 'name'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::SetPcapDataLinkType(ns3::YansWifiPhyHelper::SupportedPcapDataLinkTypes dlt) [member function]
    cls.add_method('SetPcapDataLinkType', 
                   'void', 
                   [param('ns3::YansWifiPhyHelper::SupportedPcapDataLinkTypes', 'dlt')])
    ## yans-wifi-helper.h: ns3::Ptr<ns3::WifiPhy> ns3::YansWifiPhyHelper::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WifiNetDevice> device) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::WifiNetDevice >', 'device')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## yans-wifi-helper.h: void ns3::YansWifiPhyHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3MgtAddBaRequestHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::MgtAddBaRequestHeader::MgtAddBaRequestHeader(ns3::MgtAddBaRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtAddBaRequestHeader const &', 'arg0')])
    ## mgt-headers.h: ns3::MgtAddBaRequestHeader::MgtAddBaRequestHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: uint32_t ns3::MgtAddBaRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h: uint16_t ns3::MgtAddBaRequestHeader::GetBufferSize() const [member function]
    cls.add_method('GetBufferSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: ns3::TypeId ns3::MgtAddBaRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint32_t ns3::MgtAddBaRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint16_t ns3::MgtAddBaRequestHeader::GetStartingSequence() const [member function]
    cls.add_method('GetStartingSequence', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: uint8_t ns3::MgtAddBaRequestHeader::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: uint16_t ns3::MgtAddBaRequestHeader::GetTimeout() const [member function]
    cls.add_method('GetTimeout', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: static ns3::TypeId ns3::MgtAddBaRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h: bool ns3::MgtAddBaRequestHeader::IsAmsduSupported() const [member function]
    cls.add_method('IsAmsduSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: bool ns3::MgtAddBaRequestHeader::IsImmediateBlockAck() const [member function]
    cls.add_method('IsImmediateBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::SetAmsduSupport(bool supported) [member function]
    cls.add_method('SetAmsduSupport', 
                   'void', 
                   [param('bool', 'supported')])
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::SetBufferSize(uint16_t size) [member function]
    cls.add_method('SetBufferSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::SetDelayedBlockAck() [member function]
    cls.add_method('SetDelayedBlockAck', 
                   'void', 
                   [])
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::SetImmediateBlockAck() [member function]
    cls.add_method('SetImmediateBlockAck', 
                   'void', 
                   [])
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::SetStartingSequence(uint16_t seq) [member function]
    cls.add_method('SetStartingSequence', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::SetTid(uint8_t tid) [member function]
    cls.add_method('SetTid', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## mgt-headers.h: void ns3::MgtAddBaRequestHeader::SetTimeout(uint16_t timeout) [member function]
    cls.add_method('SetTimeout', 
                   'void', 
                   [param('uint16_t', 'timeout')])
    return

def register_Ns3MgtAddBaResponseHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::MgtAddBaResponseHeader::MgtAddBaResponseHeader(ns3::MgtAddBaResponseHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtAddBaResponseHeader const &', 'arg0')])
    ## mgt-headers.h: ns3::MgtAddBaResponseHeader::MgtAddBaResponseHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: uint32_t ns3::MgtAddBaResponseHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h: uint16_t ns3::MgtAddBaResponseHeader::GetBufferSize() const [member function]
    cls.add_method('GetBufferSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: ns3::TypeId ns3::MgtAddBaResponseHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint32_t ns3::MgtAddBaResponseHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: ns3::StatusCode ns3::MgtAddBaResponseHeader::GetStatusCode() const [member function]
    cls.add_method('GetStatusCode', 
                   'ns3::StatusCode', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: uint8_t ns3::MgtAddBaResponseHeader::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: uint16_t ns3::MgtAddBaResponseHeader::GetTimeout() const [member function]
    cls.add_method('GetTimeout', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: static ns3::TypeId ns3::MgtAddBaResponseHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h: bool ns3::MgtAddBaResponseHeader::IsAmsduSupported() const [member function]
    cls.add_method('IsAmsduSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: bool ns3::MgtAddBaResponseHeader::IsImmediateBlockAck() const [member function]
    cls.add_method('IsImmediateBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::SetAmsduSupport(bool supported) [member function]
    cls.add_method('SetAmsduSupport', 
                   'void', 
                   [param('bool', 'supported')])
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::SetBufferSize(uint16_t size) [member function]
    cls.add_method('SetBufferSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::SetDelayedBlockAck() [member function]
    cls.add_method('SetDelayedBlockAck', 
                   'void', 
                   [])
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::SetImmediateBlockAck() [member function]
    cls.add_method('SetImmediateBlockAck', 
                   'void', 
                   [])
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::SetStatusCode(ns3::StatusCode code) [member function]
    cls.add_method('SetStatusCode', 
                   'void', 
                   [param('ns3::StatusCode', 'code')])
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::SetTid(uint8_t tid) [member function]
    cls.add_method('SetTid', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## mgt-headers.h: void ns3::MgtAddBaResponseHeader::SetTimeout(uint16_t timeout) [member function]
    cls.add_method('SetTimeout', 
                   'void', 
                   [param('uint16_t', 'timeout')])
    return

def register_Ns3MgtAssocRequestHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::MgtAssocRequestHeader::MgtAssocRequestHeader(ns3::MgtAssocRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtAssocRequestHeader const &', 'arg0')])
    ## mgt-headers.h: ns3::MgtAssocRequestHeader::MgtAssocRequestHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: uint32_t ns3::MgtAssocRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h: ns3::TypeId ns3::MgtAssocRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint16_t ns3::MgtAssocRequestHeader::GetListenInterval() const [member function]
    cls.add_method('GetListenInterval', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: uint32_t ns3::MgtAssocRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: ns3::Ssid ns3::MgtAssocRequestHeader::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: ns3::SupportedRates ns3::MgtAssocRequestHeader::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: static ns3::TypeId ns3::MgtAssocRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h: void ns3::MgtAssocRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtAssocRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtAssocRequestHeader::SetListenInterval(uint16_t interval) [member function]
    cls.add_method('SetListenInterval', 
                   'void', 
                   [param('uint16_t', 'interval')])
    ## mgt-headers.h: void ns3::MgtAssocRequestHeader::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')])
    ## mgt-headers.h: void ns3::MgtAssocRequestHeader::SetSupportedRates(ns3::SupportedRates rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates', 'rates')])
    return

def register_Ns3MgtAssocResponseHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::MgtAssocResponseHeader::MgtAssocResponseHeader(ns3::MgtAssocResponseHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtAssocResponseHeader const &', 'arg0')])
    ## mgt-headers.h: ns3::MgtAssocResponseHeader::MgtAssocResponseHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: uint32_t ns3::MgtAssocResponseHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h: ns3::TypeId ns3::MgtAssocResponseHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint32_t ns3::MgtAssocResponseHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: ns3::StatusCode ns3::MgtAssocResponseHeader::GetStatusCode() [member function]
    cls.add_method('GetStatusCode', 
                   'ns3::StatusCode', 
                   [])
    ## mgt-headers.h: ns3::SupportedRates ns3::MgtAssocResponseHeader::GetSupportedRates() [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [])
    ## mgt-headers.h: static ns3::TypeId ns3::MgtAssocResponseHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h: void ns3::MgtAssocResponseHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtAssocResponseHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtAssocResponseHeader::SetStatusCode(ns3::StatusCode code) [member function]
    cls.add_method('SetStatusCode', 
                   'void', 
                   [param('ns3::StatusCode', 'code')])
    ## mgt-headers.h: void ns3::MgtAssocResponseHeader::SetSupportedRates(ns3::SupportedRates rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates', 'rates')])
    return

def register_Ns3MgtDelBaHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::MgtDelBaHeader::MgtDelBaHeader(ns3::MgtDelBaHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtDelBaHeader const &', 'arg0')])
    ## mgt-headers.h: ns3::MgtDelBaHeader::MgtDelBaHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: uint32_t ns3::MgtDelBaHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h: ns3::TypeId ns3::MgtDelBaHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint32_t ns3::MgtDelBaHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint8_t ns3::MgtDelBaHeader::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: static ns3::TypeId ns3::MgtDelBaHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h: bool ns3::MgtDelBaHeader::IsByOriginator() const [member function]
    cls.add_method('IsByOriginator', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: void ns3::MgtDelBaHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtDelBaHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtDelBaHeader::SetByOriginator() [member function]
    cls.add_method('SetByOriginator', 
                   'void', 
                   [])
    ## mgt-headers.h: void ns3::MgtDelBaHeader::SetByRecipient() [member function]
    cls.add_method('SetByRecipient', 
                   'void', 
                   [])
    ## mgt-headers.h: void ns3::MgtDelBaHeader::SetTid(uint8_t arg0) [member function]
    cls.add_method('SetTid', 
                   'void', 
                   [param('uint8_t', 'arg0')])
    return

def register_Ns3MgtProbeRequestHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::MgtProbeRequestHeader::MgtProbeRequestHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: ns3::MgtProbeRequestHeader::MgtProbeRequestHeader(ns3::MgtProbeRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtProbeRequestHeader const &', 'arg0')])
    ## mgt-headers.h: uint32_t ns3::MgtProbeRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h: ns3::TypeId ns3::MgtProbeRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint32_t ns3::MgtProbeRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: ns3::Ssid ns3::MgtProbeRequestHeader::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: ns3::SupportedRates ns3::MgtProbeRequestHeader::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: static ns3::TypeId ns3::MgtProbeRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h: void ns3::MgtProbeRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtProbeRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtProbeRequestHeader::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')])
    ## mgt-headers.h: void ns3::MgtProbeRequestHeader::SetSupportedRates(ns3::SupportedRates rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates', 'rates')])
    return

def register_Ns3MgtProbeResponseHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::MgtProbeResponseHeader::MgtProbeResponseHeader(ns3::MgtProbeResponseHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtProbeResponseHeader const &', 'arg0')])
    ## mgt-headers.h: ns3::MgtProbeResponseHeader::MgtProbeResponseHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: uint32_t ns3::MgtProbeResponseHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h: uint64_t ns3::MgtProbeResponseHeader::GetBeaconIntervalUs() const [member function]
    cls.add_method('GetBeaconIntervalUs', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: ns3::TypeId ns3::MgtProbeResponseHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint32_t ns3::MgtProbeResponseHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: ns3::Ssid ns3::MgtProbeResponseHeader::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: ns3::SupportedRates ns3::MgtProbeResponseHeader::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mgt-headers.h: uint64_t ns3::MgtProbeResponseHeader::GetTimestamp() [member function]
    cls.add_method('GetTimestamp', 
                   'uint64_t', 
                   [])
    ## mgt-headers.h: static ns3::TypeId ns3::MgtProbeResponseHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h: void ns3::MgtProbeResponseHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtProbeResponseHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::MgtProbeResponseHeader::SetBeaconIntervalUs(uint64_t us) [member function]
    cls.add_method('SetBeaconIntervalUs', 
                   'void', 
                   [param('uint64_t', 'us')])
    ## mgt-headers.h: void ns3::MgtProbeResponseHeader::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')])
    ## mgt-headers.h: void ns3::MgtProbeResponseHeader::SetSupportedRates(ns3::SupportedRates rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates', 'rates')])
    return

def register_Ns3NqosWifiMacHelper_methods(root_module, cls):
    ## nqos-wifi-mac-helper.h: ns3::NqosWifiMacHelper::NqosWifiMacHelper(ns3::NqosWifiMacHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NqosWifiMacHelper const &', 'arg0')])
    ## nqos-wifi-mac-helper.h: ns3::NqosWifiMacHelper::NqosWifiMacHelper() [constructor]
    cls.add_constructor([])
    ## nqos-wifi-mac-helper.h: static ns3::NqosWifiMacHelper ns3::NqosWifiMacHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::NqosWifiMacHelper', 
                   [], 
                   is_static=True)
    ## nqos-wifi-mac-helper.h: void ns3::NqosWifiMacHelper::SetType(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## nqos-wifi-mac-helper.h: ns3::Ptr<ns3::WifiMac> ns3::NqosWifiMacHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3QosTag_methods(root_module, cls):
    ## qos-tag.h: ns3::QosTag::QosTag(ns3::QosTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::QosTag const &', 'arg0')])
    ## qos-tag.h: ns3::QosTag::QosTag() [constructor]
    cls.add_constructor([])
    ## qos-tag.h: ns3::QosTag::QosTag(uint8_t tid) [constructor]
    cls.add_constructor([param('uint8_t', 'tid')])
    ## qos-tag.h: void ns3::QosTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## qos-tag.h: ns3::TypeId ns3::QosTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## qos-tag.h: uint32_t ns3::QosTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## qos-tag.h: uint8_t ns3::QosTag::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## qos-tag.h: static ns3::TypeId ns3::QosTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## qos-tag.h: void ns3::QosTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## qos-tag.h: void ns3::QosTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## qos-tag.h: void ns3::QosTag::SetTid(uint8_t tid) [member function]
    cls.add_method('SetTid', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## qos-tag.h: void ns3::QosTag::SetUserPriority(ns3::UserPriority up) [member function]
    cls.add_method('SetUserPriority', 
                   'void', 
                   [param('ns3::UserPriority', 'up')])
    return

def register_Ns3QosWifiMacHelper_methods(root_module, cls):
    ## qos-wifi-mac-helper.h: ns3::QosWifiMacHelper::QosWifiMacHelper(ns3::QosWifiMacHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::QosWifiMacHelper const &', 'arg0')])
    ## qos-wifi-mac-helper.h: ns3::QosWifiMacHelper::QosWifiMacHelper() [constructor]
    cls.add_constructor([])
    ## qos-wifi-mac-helper.h: static ns3::QosWifiMacHelper ns3::QosWifiMacHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::QosWifiMacHelper', 
                   [], 
                   is_static=True)
    ## qos-wifi-mac-helper.h: void ns3::QosWifiMacHelper::SetBlockAckInactivityTimeoutForAc(ns3::AcIndex ac, uint16_t timeout) [member function]
    cls.add_method('SetBlockAckInactivityTimeoutForAc', 
                   'void', 
                   [param('ns3::AcIndex', 'ac'), param('uint16_t', 'timeout')])
    ## qos-wifi-mac-helper.h: void ns3::QosWifiMacHelper::SetBlockAckThresholdForAc(ns3::AcIndex ac, uint8_t threshold) [member function]
    cls.add_method('SetBlockAckThresholdForAc', 
                   'void', 
                   [param('ns3::AcIndex', 'ac'), param('uint8_t', 'threshold')])
    ## qos-wifi-mac-helper.h: void ns3::QosWifiMacHelper::SetMsduAggregatorForAc(ns3::AcIndex ac, std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMsduAggregatorForAc', 
                   'void', 
                   [param('ns3::AcIndex', 'ac'), param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()')])
    ## qos-wifi-mac-helper.h: void ns3::QosWifiMacHelper::SetType(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## qos-wifi-mac-helper.h: ns3::Ptr<ns3::WifiMac> ns3::QosWifiMacHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3WifiActionHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::WifiActionHeader::WifiActionHeader(ns3::WifiActionHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiActionHeader const &', 'arg0')])
    ## mgt-headers.h: ns3::WifiActionHeader::WifiActionHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: uint32_t ns3::WifiActionHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue ns3::WifiActionHeader::GetAction() [member function]
    cls.add_method('GetAction', 
                   'ns3::WifiActionHeader::ActionValue', 
                   [])
    ## mgt-headers.h: ns3::WifiActionHeader::CategoryValue ns3::WifiActionHeader::GetCategory() [member function]
    cls.add_method('GetCategory', 
                   'ns3::WifiActionHeader::CategoryValue', 
                   [])
    ## mgt-headers.h: ns3::TypeId ns3::WifiActionHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: uint32_t ns3::WifiActionHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: static ns3::TypeId ns3::WifiActionHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h: void ns3::WifiActionHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::WifiActionHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h: void ns3::WifiActionHeader::SetAction(ns3::WifiActionHeader::CategoryValue type, ns3::WifiActionHeader::ActionValue action) [member function]
    cls.add_method('SetAction', 
                   'void', 
                   [param('ns3::WifiActionHeader::CategoryValue', 'type'), param('ns3::WifiActionHeader::ActionValue', 'action')])
    return

def register_Ns3WifiActionHeaderActionValue_methods(root_module, cls):
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue::ActionValue() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue::ActionValue(ns3::WifiActionHeader::ActionValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiActionHeader::ActionValue const &', 'arg0')])
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue::blockAck [variable]
    cls.add_instance_attribute('blockAck', 'ns3::WifiActionHeader::BlockAckActionValue', is_const=False)
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue::interwork [variable]
    cls.add_instance_attribute('interwork', 'ns3::WifiActionHeader::InterworkActionValue', is_const=False)
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue::linkMetrtic [variable]
    cls.add_instance_attribute('linkMetrtic', 'ns3::WifiActionHeader::LinkMetricActionValue', is_const=False)
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue::pathSelection [variable]
    cls.add_instance_attribute('pathSelection', 'ns3::WifiActionHeader::PathSelectionActionValue', is_const=False)
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue::peerLink [variable]
    cls.add_instance_attribute('peerLink', 'ns3::WifiActionHeader::PeerLinkMgtActionValue', is_const=False)
    ## mgt-headers.h: ns3::WifiActionHeader::ActionValue::resourceCoordination [variable]
    cls.add_instance_attribute('resourceCoordination', 'ns3::WifiActionHeader::ResourceCoordinationActionValue', is_const=False)
    return

def register_Ns3WifiInformationElement_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## wifi-information-element.h: ns3::WifiInformationElement::WifiInformationElement() [constructor]
    cls.add_constructor([])
    ## wifi-information-element.h: ns3::WifiInformationElement::WifiInformationElement(ns3::WifiInformationElement const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiInformationElement const &', 'arg0')])
    ## wifi-information-element.h: ns3::Buffer::Iterator ns3::WifiInformationElement::Deserialize(ns3::Buffer::Iterator i) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')])
    ## wifi-information-element.h: ns3::Buffer::Iterator ns3::WifiInformationElement::DeserializeIfPresent(ns3::Buffer::Iterator i) [member function]
    cls.add_method('DeserializeIfPresent', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')])
    ## wifi-information-element.h: uint8_t ns3::WifiInformationElement::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-information-element.h: ns3::WifiInformationElementId ns3::WifiInformationElement::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-information-element.h: uint8_t ns3::WifiInformationElement::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-information-element.h: uint16_t ns3::WifiInformationElement::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-information-element.h: void ns3::WifiInformationElement::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element.h: ns3::Buffer::Iterator ns3::WifiInformationElement::Serialize(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True)
    ## wifi-information-element.h: void ns3::WifiInformationElement::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3WifiInformationElementVector_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## wifi-information-element-vector.h: ns3::WifiInformationElementVector::WifiInformationElementVector(ns3::WifiInformationElementVector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiInformationElementVector const &', 'arg0')])
    ## wifi-information-element-vector.h: ns3::WifiInformationElementVector::WifiInformationElementVector() [constructor]
    cls.add_constructor([])
    ## wifi-information-element-vector.h: bool ns3::WifiInformationElementVector::AddInformationElement(ns3::Ptr<ns3::WifiInformationElement> element) [member function]
    cls.add_method('AddInformationElement', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WifiInformationElement >', 'element')])
    ## wifi-information-element-vector.h: __gnu_cxx::__normal_iterator<ns3::Ptr<ns3::WifiInformationElement>*,std::vector<ns3::Ptr<ns3::WifiInformationElement>, std::allocator<ns3::Ptr<ns3::WifiInformationElement> > > > ns3::WifiInformationElementVector::Begin() [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::WifiInformationElement >, std::vector< ns3::Ptr< ns3::WifiInformationElement > > >', 
                   [])
    ## wifi-information-element-vector.h: uint32_t ns3::WifiInformationElementVector::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wifi-information-element-vector.h: uint32_t ns3::WifiInformationElementVector::DeserializeSingleIe(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DeserializeSingleIe', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wifi-information-element-vector.h: __gnu_cxx::__normal_iterator<ns3::Ptr<ns3::WifiInformationElement>*,std::vector<ns3::Ptr<ns3::WifiInformationElement>, std::allocator<ns3::Ptr<ns3::WifiInformationElement> > > > ns3::WifiInformationElementVector::End() [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::WifiInformationElement >, std::vector< ns3::Ptr< ns3::WifiInformationElement > > >', 
                   [])
    ## wifi-information-element-vector.h: ns3::Ptr<ns3::WifiInformationElement> ns3::WifiInformationElementVector::FindFirst(ns3::WifiInformationElementId id) const [member function]
    cls.add_method('FindFirst', 
                   'ns3::Ptr< ns3::WifiInformationElement >', 
                   [param('ns3::WifiInformationElementId', 'id')], 
                   is_const=True)
    ## wifi-information-element-vector.h: ns3::TypeId ns3::WifiInformationElementVector::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element-vector.h: uint32_t ns3::WifiInformationElementVector::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element-vector.h: static ns3::TypeId ns3::WifiInformationElementVector::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-information-element-vector.h: void ns3::WifiInformationElementVector::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element-vector.h: void ns3::WifiInformationElementVector::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element-vector.h: void ns3::WifiInformationElementVector::SetMaxSize(uint16_t size) [member function]
    cls.add_method('SetMaxSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## wifi-information-element-vector.h: uint32_t ns3::WifiInformationElementVector::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, visibility='protected')
    return

def register_Ns3WifiMac_methods(root_module, cls):
    ## wifi-mac.h: ns3::WifiMac::WifiMac() [constructor]
    cls.add_constructor([])
    ## wifi-mac.h: ns3::WifiMac::WifiMac(ns3::WifiMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMac const &', 'arg0')])
    ## wifi-mac.h: void ns3::WifiMac::ConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('ConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')])
    ## wifi-mac.h: void ns3::WifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Mac48Address ns3::WifiMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetBasicBlockAckTimeout() const [member function]
    cls.add_method('GetBasicBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Mac48Address ns3::WifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetCompressedBlockAckTimeout() const [member function]
    cls.add_method('GetCompressedBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetMaxPropagationDelay() const [member function]
    cls.add_method('GetMaxPropagationDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wifi-mac.h: ns3::Time ns3::WifiMac::GetMsduLifetime() const [member function]
    cls.add_method('GetMsduLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
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
    ## wifi-mac.h: ns3::Ssid ns3::WifiMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: static ns3::TypeId ns3::WifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-mac.h: void ns3::WifiMac::NotifyPromiscRx(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyPromiscRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h: void ns3::WifiMac::NotifyRx(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h: void ns3::WifiMac::NotifyRxDrop(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h: void ns3::WifiMac::NotifyTx(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h: void ns3::WifiMac::NotifyTxDrop(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h: void ns3::WifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetBasicBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetBasicBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')], 
                   is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetCompressedBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetCompressedBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')], 
                   is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetMaxPropagationDelay(ns3::Time delay) [member function]
    cls.add_method('SetMaxPropagationDelay', 
                   'void', 
                   [param('ns3::Time', 'delay')])
    ## wifi-mac.h: void ns3::WifiMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_pure_virtual=True, is_virtual=True)
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
    ## wifi-mac.h: bool ns3::WifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h: void ns3::WifiMac::ConfigureCCHDcf(ns3::Ptr<ns3::Dcf> dcf, uint32_t cwmin, uint32_t cwmax, ns3::AcIndex ac) [member function]
    cls.add_method('ConfigureCCHDcf', 
                   'void', 
                   [param('ns3::Ptr< ns3::Dcf >', 'dcf'), param('uint32_t', 'cwmin'), param('uint32_t', 'cwmax'), param('ns3::AcIndex', 'ac')], 
                   visibility='protected')
    ## wifi-mac.h: void ns3::WifiMac::ConfigureDcf(ns3::Ptr<ns3::Dcf> dcf, uint32_t cwmin, uint32_t cwmax, ns3::AcIndex ac) [member function]
    cls.add_method('ConfigureDcf', 
                   'void', 
                   [param('ns3::Ptr< ns3::Dcf >', 'dcf'), param('uint32_t', 'cwmin'), param('uint32_t', 'cwmax'), param('ns3::AcIndex', 'ac')], 
                   visibility='protected')
    ## wifi-mac.h: void ns3::WifiMac::FinishConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('FinishConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3WifiMacHeader_methods(root_module, cls):
    ## wifi-mac-header.h: ns3::WifiMacHeader::WifiMacHeader(ns3::WifiMacHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMacHeader const &', 'arg0')])
    ## wifi-mac-header.h: ns3::WifiMacHeader::WifiMacHeader() [constructor]
    cls.add_constructor([])
    ## wifi-mac-header.h: uint32_t ns3::WifiMacHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wifi-mac-header.h: ns3::Mac48Address ns3::WifiMacHeader::GetAddr1() const [member function]
    cls.add_method('GetAddr1', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: ns3::Mac48Address ns3::WifiMacHeader::GetAddr2() const [member function]
    cls.add_method('GetAddr2', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: ns3::Mac48Address ns3::WifiMacHeader::GetAddr3() const [member function]
    cls.add_method('GetAddr3', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: ns3::Mac48Address ns3::WifiMacHeader::GetAddr4() const [member function]
    cls.add_method('GetAddr4', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: ns3::Time ns3::WifiMacHeader::GetDuration() const [member function]
    cls.add_method('GetDuration', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: uint16_t ns3::WifiMacHeader::GetFragmentNumber() const [member function]
    cls.add_method('GetFragmentNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: ns3::TypeId ns3::WifiMacHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-header.h: ns3::WifiMacHeader::QosAckPolicy ns3::WifiMacHeader::GetQosAckPolicy() const [member function]
    cls.add_method('GetQosAckPolicy', 
                   'ns3::WifiMacHeader::QosAckPolicy', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: uint8_t ns3::WifiMacHeader::GetQosTid() const [member function]
    cls.add_method('GetQosTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: uint8_t ns3::WifiMacHeader::GetQosTxopLimit() const [member function]
    cls.add_method('GetQosTxopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: uint16_t ns3::WifiMacHeader::GetRawDuration() const [member function]
    cls.add_method('GetRawDuration', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: uint16_t ns3::WifiMacHeader::GetSequenceControl() const [member function]
    cls.add_method('GetSequenceControl', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: uint16_t ns3::WifiMacHeader::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: uint32_t ns3::WifiMacHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-header.h: uint32_t ns3::WifiMacHeader::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: ns3::WifiMacType ns3::WifiMacHeader::GetType() const [member function]
    cls.add_method('GetType', 
                   'ns3::WifiMacType', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: static ns3::TypeId ns3::WifiMacHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-mac-header.h: char const * ns3::WifiMacHeader::GetTypeString() const [member function]
    cls.add_method('GetTypeString', 
                   'char const *', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsAck() const [member function]
    cls.add_method('IsAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsAction() const [member function]
    cls.add_method('IsAction', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsAssocReq() const [member function]
    cls.add_method('IsAssocReq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsAssocResp() const [member function]
    cls.add_method('IsAssocResp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsAuthentication() const [member function]
    cls.add_method('IsAuthentication', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsBeacon() const [member function]
    cls.add_method('IsBeacon', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsBlockAck() const [member function]
    cls.add_method('IsBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsBlockAckReq() const [member function]
    cls.add_method('IsBlockAckReq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsCfpoll() const [member function]
    cls.add_method('IsCfpoll', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsCtl() const [member function]
    cls.add_method('IsCtl', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsCts() const [member function]
    cls.add_method('IsCts', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsData() const [member function]
    cls.add_method('IsData', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsDeauthentication() const [member function]
    cls.add_method('IsDeauthentication', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsDisassociation() const [member function]
    cls.add_method('IsDisassociation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsFromDs() const [member function]
    cls.add_method('IsFromDs', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsMgt() const [member function]
    cls.add_method('IsMgt', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsMoreFragments() const [member function]
    cls.add_method('IsMoreFragments', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsMultihopAction() const [member function]
    cls.add_method('IsMultihopAction', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsProbeReq() const [member function]
    cls.add_method('IsProbeReq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsProbeResp() const [member function]
    cls.add_method('IsProbeResp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsQosAck() const [member function]
    cls.add_method('IsQosAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsQosAmsdu() const [member function]
    cls.add_method('IsQosAmsdu', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsQosBlockAck() const [member function]
    cls.add_method('IsQosBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsQosData() const [member function]
    cls.add_method('IsQosData', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsQosEosp() const [member function]
    cls.add_method('IsQosEosp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsQosNoAck() const [member function]
    cls.add_method('IsQosNoAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsReassocReq() const [member function]
    cls.add_method('IsReassocReq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsReassocResp() const [member function]
    cls.add_method('IsReassocResp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsRetry() const [member function]
    cls.add_method('IsRetry', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsRts() const [member function]
    cls.add_method('IsRts', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: bool ns3::WifiMacHeader::IsToDs() const [member function]
    cls.add_method('IsToDs', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h: void ns3::WifiMacHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-header.h: void ns3::WifiMacHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetAction() [member function]
    cls.add_method('SetAction', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetAddr1(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr1', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetAddr2(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr2', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetAddr3(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr3', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetAddr4(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr4', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetAssocReq() [member function]
    cls.add_method('SetAssocReq', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetAssocResp() [member function]
    cls.add_method('SetAssocResp', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetBeacon() [member function]
    cls.add_method('SetBeacon', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetBlockAck() [member function]
    cls.add_method('SetBlockAck', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetBlockAckReq() [member function]
    cls.add_method('SetBlockAckReq', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetDsFrom() [member function]
    cls.add_method('SetDsFrom', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetDsNotFrom() [member function]
    cls.add_method('SetDsNotFrom', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetDsNotTo() [member function]
    cls.add_method('SetDsNotTo', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetDsTo() [member function]
    cls.add_method('SetDsTo', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetDuration(ns3::Time duration) [member function]
    cls.add_method('SetDuration', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetFragmentNumber(uint8_t frag) [member function]
    cls.add_method('SetFragmentNumber', 
                   'void', 
                   [param('uint8_t', 'frag')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetId(uint16_t id) [member function]
    cls.add_method('SetId', 
                   'void', 
                   [param('uint16_t', 'id')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetMoreFragments() [member function]
    cls.add_method('SetMoreFragments', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetMultihopAction() [member function]
    cls.add_method('SetMultihopAction', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetNoMoreFragments() [member function]
    cls.add_method('SetNoMoreFragments', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetNoRetry() [member function]
    cls.add_method('SetNoRetry', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetProbeReq() [member function]
    cls.add_method('SetProbeReq', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetProbeResp() [member function]
    cls.add_method('SetProbeResp', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosAckPolicy(ns3::WifiMacHeader::QosAckPolicy arg0) [member function]
    cls.add_method('SetQosAckPolicy', 
                   'void', 
                   [param('ns3::WifiMacHeader::QosAckPolicy', 'arg0')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosAmsdu() [member function]
    cls.add_method('SetQosAmsdu', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosBlockAck() [member function]
    cls.add_method('SetQosBlockAck', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosEosp() [member function]
    cls.add_method('SetQosEosp', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosNoAck() [member function]
    cls.add_method('SetQosNoAck', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosNoAmsdu() [member function]
    cls.add_method('SetQosNoAmsdu', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosNoEosp() [member function]
    cls.add_method('SetQosNoEosp', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosNormalAck() [member function]
    cls.add_method('SetQosNormalAck', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosTid(uint8_t tid) [member function]
    cls.add_method('SetQosTid', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetQosTxopLimit(uint8_t txop) [member function]
    cls.add_method('SetQosTxopLimit', 
                   'void', 
                   [param('uint8_t', 'txop')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetRawDuration(uint16_t duration) [member function]
    cls.add_method('SetRawDuration', 
                   'void', 
                   [param('uint16_t', 'duration')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetRetry() [member function]
    cls.add_method('SetRetry', 
                   'void', 
                   [])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetSequenceNumber(uint16_t seq) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetType(ns3::WifiMacType type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('ns3::WifiMacType', 'type')])
    ## wifi-mac-header.h: void ns3::WifiMacHeader::SetTypeData() [member function]
    cls.add_method('SetTypeData', 
                   'void', 
                   [])
    return

def register_Ns3WifiPhy_methods(root_module, cls):
    ## wifi-phy.h: ns3::WifiPhy::WifiPhy(ns3::WifiPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhy const &', 'arg0')])
    ## wifi-phy.h: ns3::WifiPhy::WifiPhy() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h: double ns3::WifiPhy::CalculateSnr(ns3::WifiMode txMode, double ber) const [member function]
    cls.add_method('CalculateSnr', 
                   'double', 
                   [param('ns3::WifiMode', 'txMode'), param('double', 'ber')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::CalculateTxDuration(uint32_t size, ns3::WifiMode payloadMode, ns3::WifiPreamble preamble) const [member function]
    cls.add_method('CalculateTxDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::ConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('ConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: ns3::Ptr<ns3::WifiChannel> ns3::WifiPhy::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::WifiChannel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: uint16_t ns3::WifiPhy::GetChannelNumber() const [member function]
    cls.add_method('GetChannelNumber', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetDelayUntilIdle() [member function]
    cls.add_method('GetDelayUntilIdle', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetDsssRate11Mbps() [member function]
    cls.add_method('GetDsssRate11Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetDsssRate1Mbps() [member function]
    cls.add_method('GetDsssRate1Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetDsssRate2Mbps() [member function]
    cls.add_method('GetDsssRate2Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetDsssRate5_5Mbps() [member function]
    cls.add_method('GetDsssRate5_5Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate12Mbps() [member function]
    cls.add_method('GetErpOfdmRate12Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate18Mbps() [member function]
    cls.add_method('GetErpOfdmRate18Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate24Mbps() [member function]
    cls.add_method('GetErpOfdmRate24Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate36Mbps() [member function]
    cls.add_method('GetErpOfdmRate36Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate48Mbps() [member function]
    cls.add_method('GetErpOfdmRate48Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate54Mbps() [member function]
    cls.add_method('GetErpOfdmRate54Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate6Mbps() [member function]
    cls.add_method('GetErpOfdmRate6Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate9Mbps() [member function]
    cls.add_method('GetErpOfdmRate9Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetLastRxStartTime() const [member function]
    cls.add_method('GetLastRxStartTime', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: ns3::WifiMode ns3::WifiPhy::GetMode(uint32_t mode) const [member function]
    cls.add_method('GetMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'mode')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: uint32_t ns3::WifiPhy::GetNModes() const [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: uint32_t ns3::WifiPhy::GetNTxPower() const [member function]
    cls.add_method('GetNTxPower', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate12Mbps() [member function]
    cls.add_method('GetOfdmRate12Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate12MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate12MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate12MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate12MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate13_5MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate13_5MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate18Mbps() [member function]
    cls.add_method('GetOfdmRate18Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate18MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate18MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate1_5MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate1_5MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate24Mbps() [member function]
    cls.add_method('GetOfdmRate24Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate24MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate24MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate27MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate27MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate2_25MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate2_25MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate36Mbps() [member function]
    cls.add_method('GetOfdmRate36Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate3MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate3MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate3MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate3MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate48Mbps() [member function]
    cls.add_method('GetOfdmRate48Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate4_5MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate4_5MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate4_5MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate4_5MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate54Mbps() [member function]
    cls.add_method('GetOfdmRate54Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate6Mbps() [member function]
    cls.add_method('GetOfdmRate6Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate6MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate6MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate6MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate6MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate9Mbps() [member function]
    cls.add_method('GetOfdmRate9Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate9MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate9MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: static ns3::WifiMode ns3::WifiPhy::GetOfdmRate9MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate9MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: ns3::Time ns3::WifiPhy::GetStateDuration() [member function]
    cls.add_method('GetStateDuration', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: double ns3::WifiPhy::GetTxPowerEnd() const [member function]
    cls.add_method('GetTxPowerEnd', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: double ns3::WifiPhy::GetTxPowerStart() const [member function]
    cls.add_method('GetTxPowerStart', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h: static ns3::TypeId ns3::WifiPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
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
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateSwitching() [member function]
    cls.add_method('IsStateSwitching', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: bool ns3::WifiPhy::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::NotifyPromiscSniffRx(ns3::Ptr<ns3::Packet const> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, bool isShortPreamble, double signalDbm, double noiseDbm) [member function]
    cls.add_method('NotifyPromiscSniffRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'channelFreqMhz'), param('uint16_t', 'channelNumber'), param('uint32_t', 'rate'), param('bool', 'isShortPreamble'), param('double', 'signalDbm'), param('double', 'noiseDbm')])
    ## wifi-phy.h: void ns3::WifiPhy::NotifyPromiscSniffTx(ns3::Ptr<ns3::Packet const> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, bool isShortPreamble) [member function]
    cls.add_method('NotifyPromiscSniffTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'channelFreqMhz'), param('uint16_t', 'channelNumber'), param('uint32_t', 'rate'), param('bool', 'isShortPreamble')])
    ## wifi-phy.h: void ns3::WifiPhy::NotifyRxBegin(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRxBegin', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h: void ns3::WifiPhy::NotifyRxDrop(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h: void ns3::WifiPhy::NotifyRxEnd(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRxEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h: void ns3::WifiPhy::NotifyTxBegin(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTxBegin', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h: void ns3::WifiPhy::NotifyTxDrop(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h: void ns3::WifiPhy::NotifyTxEnd(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTxEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h: void ns3::WifiPhy::RegisterListener(ns3::WifiPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::WifiPhyListener *', 'listener')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::SendPacket(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMode mode, ns3::WifiPreamble preamble, uint8_t txPowerLevel) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble'), param('uint8_t', 'txPowerLevel')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::SetChannelNumber(uint16_t id) [member function]
    cls.add_method('SetChannelNumber', 
                   'void', 
                   [param('uint16_t', 'id')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::SetReceiveErrorCallback(ns3::Callback<void,ns3::Ptr<const ns3::Packet>,double,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h: void ns3::WifiPhy::SetReceiveOkCallback(ns3::Callback<void,ns3::Ptr<ns3::Packet>,double,ns3::WifiMode,ns3::WifiPreamble,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::WifiMode, ns3::WifiPreamble, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3WifiRemoteStationManager_methods(root_module, cls):
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationManager::WifiRemoteStationManager(ns3::WifiRemoteStationManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStationManager const &', 'arg0')])
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationManager::WifiRemoteStationManager() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::AddBasicMode(ns3::WifiMode mode) [member function]
    cls.add_method('AddBasicMode', 
                   'void', 
                   [param('ns3::WifiMode', 'mode')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::AddSupportedMode(ns3::Mac48Address address, ns3::WifiMode mode) [member function]
    cls.add_method('AddSupportedMode', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'mode')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetAckMode(ns3::Mac48Address address, ns3::WifiMode dataMode) [member function]
    cls.add_method('GetAckMode', 
                   'ns3::WifiMode', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'dataMode')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetBasicMode(uint32_t i) const [member function]
    cls.add_method('GetBasicMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetCtsMode(ns3::Mac48Address address, ns3::WifiMode rtsMode) [member function]
    cls.add_method('GetCtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'rtsMode')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetDataMode(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, uint32_t fullPacketSize) [member function]
    cls.add_method('GetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fullPacketSize')])
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetDefaultMode() const [member function]
    cls.add_method('GetDefaultMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetFragmentOffset(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('GetFragmentOffset', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')])
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetFragmentSize(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('GetFragmentSize', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')])
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetFragmentationThreshold() const [member function]
    cls.add_method('GetFragmentationThreshold', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStationInfo ns3::WifiRemoteStationManager::GetInfo(ns3::Mac48Address address) [member function]
    cls.add_method('GetInfo', 
                   'ns3::WifiRemoteStationInfo', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetMaxSlrc() const [member function]
    cls.add_method('GetMaxSlrc', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetMaxSsrc() const [member function]
    cls.add_method('GetMaxSsrc', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetNBasicModes() const [member function]
    cls.add_method('GetNBasicModes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetNonUnicastMode() const [member function]
    cls.add_method('GetNonUnicastMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetRtsCtsThreshold() const [member function]
    cls.add_method('GetRtsCtsThreshold', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetRtsMode(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('GetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h: static ns3::TypeId ns3::WifiRemoteStationManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::IsAssociated(ns3::Mac48Address address) const [member function]
    cls.add_method('IsAssociated', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::IsBrandNew(ns3::Mac48Address address) const [member function]
    cls.add_method('IsBrandNew', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::IsLastFragment(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('IsLastFragment', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')])
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::IsWaitAssocTxOk(ns3::Mac48Address address) const [member function]
    cls.add_method('IsWaitAssocTxOk', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::NeedDataRetransmission(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NeedDataRetransmission', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::NeedFragmentation(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NeedFragmentation', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::NeedRts(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NeedRts', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::NeedRtsRetransmission(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NeedRtsRetransmission', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::PrepareForQueue(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, uint32_t fullPacketSize) [member function]
    cls.add_method('PrepareForQueue', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fullPacketSize')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::RecordDisassociated(ns3::Mac48Address address) [member function]
    cls.add_method('RecordDisassociated', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::RecordGotAssocTxFailed(ns3::Mac48Address address) [member function]
    cls.add_method('RecordGotAssocTxFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::RecordGotAssocTxOk(ns3::Mac48Address address) [member function]
    cls.add_method('RecordGotAssocTxOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::RecordWaitAssocTxOk(ns3::Mac48Address address) [member function]
    cls.add_method('RecordWaitAssocTxOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::ReportDataFailed(ns3::Mac48Address address, ns3::WifiMacHeader const * header) [member function]
    cls.add_method('ReportDataFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::ReportDataOk(ns3::Mac48Address address, ns3::WifiMacHeader const * header, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('ReportDataOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::ReportFinalDataFailed(ns3::Mac48Address address, ns3::WifiMacHeader const * header) [member function]
    cls.add_method('ReportFinalDataFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::ReportFinalRtsFailed(ns3::Mac48Address address, ns3::WifiMacHeader const * header) [member function]
    cls.add_method('ReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::ReportRtsFailed(ns3::Mac48Address address, ns3::WifiMacHeader const * header) [member function]
    cls.add_method('ReportRtsFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::ReportRtsOk(ns3::Mac48Address address, ns3::WifiMacHeader const * header, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('ReportRtsOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::ReportRxOk(ns3::Mac48Address address, ns3::WifiMacHeader const * header, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('ReportRxOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::Reset(ns3::Mac48Address address) [member function]
    cls.add_method('Reset', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetFragmentationThreshold(uint32_t threshold) [member function]
    cls.add_method('SetFragmentationThreshold', 
                   'void', 
                   [param('uint32_t', 'threshold')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetMaxSlrc(uint32_t maxSlrc) [member function]
    cls.add_method('SetMaxSlrc', 
                   'void', 
                   [param('uint32_t', 'maxSlrc')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetMaxSsrc(uint32_t maxSsrc) [member function]
    cls.add_method('SetMaxSsrc', 
                   'void', 
                   [param('uint32_t', 'maxSsrc')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetRtsCtsThreshold(uint32_t threshold) [member function]
    cls.add_method('SetRtsCtsThreshold', 
                   'void', 
                   [param('uint32_t', 'threshold')])
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::SetupPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetupPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h: uint32_t ns3::WifiRemoteStationManager::GetNSupported(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetNSupported', 
                   'uint32_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::GetSupported(ns3::WifiRemoteStation const * station, uint32_t i) const [member function]
    cls.add_method('GetSupported', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation const *', 'station'), param('uint32_t', 'i')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h: ns3::WifiRemoteStation * ns3::WifiRemoteStationManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: ns3::WifiMode ns3::WifiRemoteStationManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::DoNeedDataRetransmission(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedDataRetransmission', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::DoNeedFragmentation(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedFragmentation', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::DoNeedRts(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedRts', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::DoNeedRtsRetransmission(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedRtsRetransmission', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: void ns3::WifiRemoteStationManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h: bool ns3::WifiRemoteStationManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
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
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetChannelNumber(uint16_t id) [member function]
    cls.add_method('SetChannelNumber', 
                   'void', 
                   [param('uint16_t', 'id')], 
                   is_virtual=True)
    ## yans-wifi-phy.h: uint16_t ns3::YansWifiPhy::GetChannelNumber() const [member function]
    cls.add_method('GetChannelNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetChannelFrequencyMhz() const [member function]
    cls.add_method('GetChannelFrequencyMhz', 
                   'double', 
                   [], 
                   is_const=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::StartReceivePacket(ns3::Ptr<ns3::Packet> packet, double rxPowerDbm, ns3::WifiMode mode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('StartReceivePacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxPowerDbm'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble')])
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetRxNoiseFigure(double noiseFigureDb) [member function]
    cls.add_method('SetRxNoiseFigure', 
                   'void', 
                   [param('double', 'noiseFigureDb')])
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
    ## yans-wifi-phy.h: double ns3::YansWifiPhy::GetRxNoiseFigure() const [member function]
    cls.add_method('GetRxNoiseFigure', 
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
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetReceiveOkCallback(ns3::Callback<void,ns3::Ptr<ns3::Packet>,double,ns3::WifiMode,ns3::WifiPreamble,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::WifiMode, ns3::WifiPreamble, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SetReceiveErrorCallback(ns3::Callback<void,ns3::Ptr<const ns3::Packet>,double,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, double, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::SendPacket(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMode mode, ns3::WifiPreamble preamble, uint8_t txPowerLevel) [member function]
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
    ## yans-wifi-phy.h: bool ns3::YansWifiPhy::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: bool ns3::YansWifiPhy::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_virtual=True)
    ## yans-wifi-phy.h: bool ns3::YansWifiPhy::IsStateSwitching() [member function]
    cls.add_method('IsStateSwitching', 
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
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::ConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('ConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_virtual=True)
    ## yans-wifi-phy.h: void ns3::YansWifiPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AarfWifiManager_methods(root_module, cls):
    ## aarf-wifi-manager.h: ns3::AarfWifiManager::AarfWifiManager(ns3::AarfWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AarfWifiManager const &', 'arg0')])
    ## aarf-wifi-manager.h: ns3::AarfWifiManager::AarfWifiManager() [constructor]
    cls.add_constructor([])
    ## aarf-wifi-manager.h: static ns3::TypeId ns3::AarfWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aarf-wifi-manager.h: ns3::WifiRemoteStation * ns3::AarfWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: ns3::WifiMode ns3::AarfWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: ns3::WifiMode ns3::AarfWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: void ns3::AarfWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: void ns3::AarfWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: void ns3::AarfWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: void ns3::AarfWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: void ns3::AarfWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: void ns3::AarfWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: void ns3::AarfWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## aarf-wifi-manager.h: bool ns3::AarfWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3AarfcdWifiManager_methods(root_module, cls):
    ## aarfcd-wifi-manager.h: ns3::AarfcdWifiManager::AarfcdWifiManager(ns3::AarfcdWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AarfcdWifiManager const &', 'arg0')])
    ## aarfcd-wifi-manager.h: ns3::AarfcdWifiManager::AarfcdWifiManager() [constructor]
    cls.add_constructor([])
    ## aarfcd-wifi-manager.h: static ns3::TypeId ns3::AarfcdWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## aarfcd-wifi-manager.h: ns3::WifiRemoteStation * ns3::AarfcdWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: ns3::WifiMode ns3::AarfcdWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: ns3::WifiMode ns3::AarfcdWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: bool ns3::AarfcdWifiManager::DoNeedRts(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedRts', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: void ns3::AarfcdWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: void ns3::AarfcdWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: void ns3::AarfcdWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: void ns3::AarfcdWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: void ns3::AarfcdWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: void ns3::AarfcdWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: void ns3::AarfcdWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## aarfcd-wifi-manager.h: bool ns3::AarfcdWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3AmrrWifiManager_methods(root_module, cls):
    ## amrr-wifi-manager.h: ns3::AmrrWifiManager::AmrrWifiManager(ns3::AmrrWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AmrrWifiManager const &', 'arg0')])
    ## amrr-wifi-manager.h: ns3::AmrrWifiManager::AmrrWifiManager() [constructor]
    cls.add_constructor([])
    ## amrr-wifi-manager.h: static ns3::TypeId ns3::AmrrWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## amrr-wifi-manager.h: ns3::WifiRemoteStation * ns3::AmrrWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: ns3::WifiMode ns3::AmrrWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: ns3::WifiMode ns3::AmrrWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: void ns3::AmrrWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## amrr-wifi-manager.h: bool ns3::AmrrWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3AmsduSubframeHeader_methods(root_module, cls):
    ## amsdu-subframe-header.h: ns3::AmsduSubframeHeader::AmsduSubframeHeader(ns3::AmsduSubframeHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AmsduSubframeHeader const &', 'arg0')])
    ## amsdu-subframe-header.h: ns3::AmsduSubframeHeader::AmsduSubframeHeader() [constructor]
    cls.add_constructor([])
    ## amsdu-subframe-header.h: uint32_t ns3::AmsduSubframeHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## amsdu-subframe-header.h: ns3::Mac48Address ns3::AmsduSubframeHeader::GetDestinationAddr() const [member function]
    cls.add_method('GetDestinationAddr', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## amsdu-subframe-header.h: ns3::TypeId ns3::AmsduSubframeHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## amsdu-subframe-header.h: uint16_t ns3::AmsduSubframeHeader::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## amsdu-subframe-header.h: uint32_t ns3::AmsduSubframeHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## amsdu-subframe-header.h: ns3::Mac48Address ns3::AmsduSubframeHeader::GetSourceAddr() const [member function]
    cls.add_method('GetSourceAddr', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## amsdu-subframe-header.h: static ns3::TypeId ns3::AmsduSubframeHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## amsdu-subframe-header.h: void ns3::AmsduSubframeHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## amsdu-subframe-header.h: void ns3::AmsduSubframeHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## amsdu-subframe-header.h: void ns3::AmsduSubframeHeader::SetDestinationAddr(ns3::Mac48Address to) [member function]
    cls.add_method('SetDestinationAddr', 
                   'void', 
                   [param('ns3::Mac48Address', 'to')])
    ## amsdu-subframe-header.h: void ns3::AmsduSubframeHeader::SetLength(uint16_t arg0) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## amsdu-subframe-header.h: void ns3::AmsduSubframeHeader::SetSourceAddr(ns3::Mac48Address to) [member function]
    cls.add_method('SetSourceAddr', 
                   'void', 
                   [param('ns3::Mac48Address', 'to')])
    return

def register_Ns3ArfWifiManager_methods(root_module, cls):
    ## arf-wifi-manager.h: ns3::ArfWifiManager::ArfWifiManager(ns3::ArfWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ArfWifiManager const &', 'arg0')])
    ## arf-wifi-manager.h: ns3::ArfWifiManager::ArfWifiManager() [constructor]
    cls.add_constructor([])
    ## arf-wifi-manager.h: static ns3::TypeId ns3::ArfWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## arf-wifi-manager.h: ns3::WifiRemoteStation * ns3::ArfWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: ns3::WifiMode ns3::ArfWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: ns3::WifiMode ns3::ArfWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: void ns3::ArfWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## arf-wifi-manager.h: bool ns3::ArfWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3AthstatsWifiTraceSink_methods(root_module, cls):
    ## athstats-helper.h: ns3::AthstatsWifiTraceSink::AthstatsWifiTraceSink(ns3::AthstatsWifiTraceSink const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AthstatsWifiTraceSink const &', 'arg0')])
    ## athstats-helper.h: ns3::AthstatsWifiTraceSink::AthstatsWifiTraceSink() [constructor]
    cls.add_constructor([])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::DevRxTrace(std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DevRxTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::DevTxTrace(std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DevTxTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## athstats-helper.h: static ns3::TypeId ns3::AthstatsWifiTraceSink::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::Open(std::string const & name) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string const &', 'name')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::PhyRxErrorTrace(std::string context, ns3::Ptr<ns3::Packet const> packet, double snr) [member function]
    cls.add_method('PhyRxErrorTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('double', 'snr')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::PhyRxOkTrace(std::string context, ns3::Ptr<ns3::Packet const> packet, double snr, ns3::WifiMode mode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('PhyRxOkTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('double', 'snr'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::PhyStateTrace(std::string context, ns3::Time start, ns3::Time duration, ns3::WifiPhy::State state) [member function]
    cls.add_method('PhyStateTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Time', 'start'), param('ns3::Time', 'duration'), param('ns3::WifiPhy::State', 'state')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::PhyTxTrace(std::string context, ns3::Ptr<ns3::Packet const> packet, ns3::WifiMode mode, ns3::WifiPreamble preamble, uint8_t txPower) [member function]
    cls.add_method('PhyTxTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMode', 'mode'), param('ns3::WifiPreamble', 'preamble'), param('uint8_t', 'txPower')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::TxDataFailedTrace(std::string context, ns3::Mac48Address address) [member function]
    cls.add_method('TxDataFailedTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Mac48Address', 'address')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::TxFinalDataFailedTrace(std::string context, ns3::Mac48Address address) [member function]
    cls.add_method('TxFinalDataFailedTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Mac48Address', 'address')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::TxFinalRtsFailedTrace(std::string context, ns3::Mac48Address address) [member function]
    cls.add_method('TxFinalRtsFailedTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Mac48Address', 'address')])
    ## athstats-helper.h: void ns3::AthstatsWifiTraceSink::TxRtsFailedTrace(std::string context, ns3::Mac48Address address) [member function]
    cls.add_method('TxRtsFailedTrace', 
                   'void', 
                   [param('std::string', 'context'), param('ns3::Mac48Address', 'address')])
    return

def register_Ns3CaraWifiManager_methods(root_module, cls):
    ## cara-wifi-manager.h: ns3::CaraWifiManager::CaraWifiManager(ns3::CaraWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CaraWifiManager const &', 'arg0')])
    ## cara-wifi-manager.h: ns3::CaraWifiManager::CaraWifiManager() [constructor]
    cls.add_constructor([])
    ## cara-wifi-manager.h: static ns3::TypeId ns3::CaraWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## cara-wifi-manager.h: ns3::WifiRemoteStation * ns3::CaraWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: ns3::WifiMode ns3::CaraWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: ns3::WifiMode ns3::CaraWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: bool ns3::CaraWifiManager::DoNeedRts(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedRts', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: void ns3::CaraWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: void ns3::CaraWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: void ns3::CaraWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: void ns3::CaraWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: void ns3::CaraWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: void ns3::CaraWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: void ns3::CaraWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## cara-wifi-manager.h: bool ns3::CaraWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ConstantRateWifiManager_methods(root_module, cls):
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiManager::ConstantRateWifiManager(ns3::ConstantRateWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantRateWifiManager const &', 'arg0')])
    ## constant-rate-wifi-manager.h: ns3::ConstantRateWifiManager::ConstantRateWifiManager() [constructor]
    cls.add_constructor([])
    ## constant-rate-wifi-manager.h: static ns3::TypeId ns3::ConstantRateWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## constant-rate-wifi-manager.h: ns3::WifiRemoteStation * ns3::ConstantRateWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: ns3::WifiMode ns3::ConstantRateWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: ns3::WifiMode ns3::ConstantRateWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: void ns3::ConstantRateWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## constant-rate-wifi-manager.h: bool ns3::ConstantRateWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3CtrlBAckRequestHeader_methods(root_module, cls):
    ## ctrl-headers.h: ns3::CtrlBAckRequestHeader::CtrlBAckRequestHeader(ns3::CtrlBAckRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CtrlBAckRequestHeader const &', 'arg0')])
    ## ctrl-headers.h: ns3::CtrlBAckRequestHeader::CtrlBAckRequestHeader() [constructor]
    cls.add_constructor([])
    ## ctrl-headers.h: uint32_t ns3::CtrlBAckRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ctrl-headers.h: ns3::TypeId ns3::CtrlBAckRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h: uint32_t ns3::CtrlBAckRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h: uint16_t ns3::CtrlBAckRequestHeader::GetStartingSequence() const [member function]
    cls.add_method('GetStartingSequence', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: uint16_t ns3::CtrlBAckRequestHeader::GetStartingSequenceControl() const [member function]
    cls.add_method('GetStartingSequenceControl', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: uint8_t ns3::CtrlBAckRequestHeader::GetTidInfo() const [member function]
    cls.add_method('GetTidInfo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: static ns3::TypeId ns3::CtrlBAckRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckRequestHeader::IsBasic() const [member function]
    cls.add_method('IsBasic', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckRequestHeader::IsCompressed() const [member function]
    cls.add_method('IsCompressed', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckRequestHeader::IsMultiTid() const [member function]
    cls.add_method('IsMultiTid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckRequestHeader::MustSendHtImmediateAck() const [member function]
    cls.add_method('MustSendHtImmediateAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: void ns3::CtrlBAckRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h: void ns3::CtrlBAckRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h: void ns3::CtrlBAckRequestHeader::SetHtImmediateAck(bool immediateAck) [member function]
    cls.add_method('SetHtImmediateAck', 
                   'void', 
                   [param('bool', 'immediateAck')])
    ## ctrl-headers.h: void ns3::CtrlBAckRequestHeader::SetStartingSequence(uint16_t seq) [member function]
    cls.add_method('SetStartingSequence', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## ctrl-headers.h: void ns3::CtrlBAckRequestHeader::SetTidInfo(uint8_t tid) [member function]
    cls.add_method('SetTidInfo', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## ctrl-headers.h: void ns3::CtrlBAckRequestHeader::SetType(ns3::BlockAckType type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('ns3::BlockAckType', 'type')])
    return

def register_Ns3CtrlBAckResponseHeader_methods(root_module, cls):
    ## ctrl-headers.h: ns3::CtrlBAckResponseHeader::CtrlBAckResponseHeader(ns3::CtrlBAckResponseHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CtrlBAckResponseHeader const &', 'arg0')])
    ## ctrl-headers.h: ns3::CtrlBAckResponseHeader::CtrlBAckResponseHeader() [constructor]
    cls.add_constructor([])
    ## ctrl-headers.h: uint32_t ns3::CtrlBAckResponseHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ctrl-headers.h: uint16_t const * ns3::CtrlBAckResponseHeader::GetBitmap() const [member function]
    cls.add_method('GetBitmap', 
                   'uint16_t const *', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: uint64_t ns3::CtrlBAckResponseHeader::GetCompressedBitmap() const [member function]
    cls.add_method('GetCompressedBitmap', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: ns3::TypeId ns3::CtrlBAckResponseHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h: uint32_t ns3::CtrlBAckResponseHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h: uint16_t ns3::CtrlBAckResponseHeader::GetStartingSequence() const [member function]
    cls.add_method('GetStartingSequence', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: uint16_t ns3::CtrlBAckResponseHeader::GetStartingSequenceControl() const [member function]
    cls.add_method('GetStartingSequenceControl', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: uint8_t ns3::CtrlBAckResponseHeader::GetTidInfo() const [member function]
    cls.add_method('GetTidInfo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: static ns3::TypeId ns3::CtrlBAckResponseHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckResponseHeader::IsBasic() const [member function]
    cls.add_method('IsBasic', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckResponseHeader::IsCompressed() const [member function]
    cls.add_method('IsCompressed', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckResponseHeader::IsFragmentReceived(uint16_t seq, uint8_t frag) const [member function]
    cls.add_method('IsFragmentReceived', 
                   'bool', 
                   [param('uint16_t', 'seq'), param('uint8_t', 'frag')], 
                   is_const=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckResponseHeader::IsMultiTid() const [member function]
    cls.add_method('IsMultiTid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckResponseHeader::IsPacketReceived(uint16_t seq) const [member function]
    cls.add_method('IsPacketReceived', 
                   'bool', 
                   [param('uint16_t', 'seq')], 
                   is_const=True)
    ## ctrl-headers.h: bool ns3::CtrlBAckResponseHeader::MustSendHtImmediateAck() const [member function]
    cls.add_method('MustSendHtImmediateAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::ResetBitmap() [member function]
    cls.add_method('ResetBitmap', 
                   'void', 
                   [])
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::SetHtImmediateAck(bool immeadiateAck) [member function]
    cls.add_method('SetHtImmediateAck', 
                   'void', 
                   [param('bool', 'immeadiateAck')])
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::SetReceivedFragment(uint16_t seq, uint8_t frag) [member function]
    cls.add_method('SetReceivedFragment', 
                   'void', 
                   [param('uint16_t', 'seq'), param('uint8_t', 'frag')])
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::SetReceivedPacket(uint16_t seq) [member function]
    cls.add_method('SetReceivedPacket', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::SetStartingSequence(uint16_t seq) [member function]
    cls.add_method('SetStartingSequence', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::SetStartingSequenceControl(uint16_t seqControl) [member function]
    cls.add_method('SetStartingSequenceControl', 
                   'void', 
                   [param('uint16_t', 'seqControl')])
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::SetTidInfo(uint8_t tid) [member function]
    cls.add_method('SetTidInfo', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## ctrl-headers.h: void ns3::CtrlBAckResponseHeader::SetType(ns3::BlockAckType type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('ns3::BlockAckType', 'type')])
    return

def register_Ns3Dcf_methods(root_module, cls):
    ## dcf.h: ns3::Dcf::Dcf() [constructor]
    cls.add_constructor([])
    ## dcf.h: ns3::Dcf::Dcf(ns3::Dcf const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Dcf const &', 'arg0')])
    ## dcf.h: uint32_t ns3::Dcf::GetAifsn() const [member function]
    cls.add_method('GetAifsn', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## dcf.h: uint32_t ns3::Dcf::GetMaxCw() const [member function]
    cls.add_method('GetMaxCw', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## dcf.h: uint32_t ns3::Dcf::GetMinCw() const [member function]
    cls.add_method('GetMinCw', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## dcf.h: static ns3::TypeId ns3::Dcf::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dcf.h: void ns3::Dcf::SetAifsn(uint32_t aifsn) [member function]
    cls.add_method('SetAifsn', 
                   'void', 
                   [param('uint32_t', 'aifsn')], 
                   is_pure_virtual=True, is_virtual=True)
    ## dcf.h: void ns3::Dcf::SetMaxCw(uint32_t maxCw) [member function]
    cls.add_method('SetMaxCw', 
                   'void', 
                   [param('uint32_t', 'maxCw')], 
                   is_pure_virtual=True, is_virtual=True)
    ## dcf.h: void ns3::Dcf::SetMinCw(uint32_t minCw) [member function]
    cls.add_method('SetMinCw', 
                   'void', 
                   [param('uint32_t', 'minCw')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3EdcaTxopN_methods(root_module, cls):
    ## edca-txop-n.h: static ns3::TypeId ns3::EdcaTxopN::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## edca-txop-n.h: ns3::EdcaTxopN::EdcaTxopN() [constructor]
    cls.add_constructor([])
    ## edca-txop-n.h: void ns3::EdcaTxopN::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetLow(ns3::Ptr<ns3::MacLow> low) [member function]
    cls.add_method('SetLow', 
                   'void', 
                   [param('ns3::Ptr< ns3::MacLow >', 'low')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetTxMiddle(ns3::MacTxMiddle * txMiddle) [member function]
    cls.add_method('SetTxMiddle', 
                   'void', 
                   [param('ns3::MacTxMiddle *', 'txMiddle')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetManager(ns3::DcfManager * manager) [member function]
    cls.add_method('SetManager', 
                   'void', 
                   [param('ns3::DcfManager *', 'manager')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetTxOkCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetTxFailedCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxFailedCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> remoteManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'remoteManager')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetTypeOfStation(ns3::TypeOfStation type) [member function]
    cls.add_method('SetTypeOfStation', 
                   'void', 
                   [param('ns3::TypeOfStation', 'type')])
    ## edca-txop-n.h: ns3::TypeOfStation ns3::EdcaTxopN::GetTypeOfStation() const [member function]
    cls.add_method('GetTypeOfStation', 
                   'ns3::TypeOfStation', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h: ns3::Ptr<ns3::WifiMacQueue> ns3::EdcaTxopN::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::WifiMacQueue >', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetMinCw(uint32_t minCw) [member function]
    cls.add_method('SetMinCw', 
                   'void', 
                   [param('uint32_t', 'minCw')], 
                   is_virtual=True)
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetMaxCw(uint32_t maxCw) [member function]
    cls.add_method('SetMaxCw', 
                   'void', 
                   [param('uint32_t', 'maxCw')], 
                   is_virtual=True)
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetAifsn(uint32_t aifsn) [member function]
    cls.add_method('SetAifsn', 
                   'void', 
                   [param('uint32_t', 'aifsn')], 
                   is_virtual=True)
    ## edca-txop-n.h: uint32_t ns3::EdcaTxopN::GetMinCw() const [member function]
    cls.add_method('GetMinCw', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## edca-txop-n.h: uint32_t ns3::EdcaTxopN::GetMaxCw() const [member function]
    cls.add_method('GetMaxCw', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## edca-txop-n.h: uint32_t ns3::EdcaTxopN::GetAifsn() const [member function]
    cls.add_method('GetAifsn', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## edca-txop-n.h: ns3::Ptr<ns3::MacLow> ns3::EdcaTxopN::Low() [member function]
    cls.add_method('Low', 
                   'ns3::Ptr< ns3::MacLow >', 
                   [])
    ## edca-txop-n.h: ns3::Ptr<ns3::MsduAggregator> ns3::EdcaTxopN::GetMsduAggregator() const [member function]
    cls.add_method('GetMsduAggregator', 
                   'ns3::Ptr< ns3::MsduAggregator >', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h: bool ns3::EdcaTxopN::NeedsAccess() const [member function]
    cls.add_method('NeedsAccess', 
                   'bool', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h: void ns3::EdcaTxopN::NotifyAccessGranted() [member function]
    cls.add_method('NotifyAccessGranted', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::NotifyInternalCollision() [member function]
    cls.add_method('NotifyInternalCollision', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::NotifyCollision() [member function]
    cls.add_method('NotifyCollision', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::NotifyChannelSwitching() [member function]
    cls.add_method('NotifyChannelSwitching', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::GotCts(double snr, ns3::WifiMode txMode) [member function]
    cls.add_method('GotCts', 
                   'void', 
                   [param('double', 'snr'), param('ns3::WifiMode', 'txMode')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::MissedCts() [member function]
    cls.add_method('MissedCts', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::GotAck(double snr, ns3::WifiMode txMode) [member function]
    cls.add_method('GotAck', 
                   'void', 
                   [param('double', 'snr'), param('ns3::WifiMode', 'txMode')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::GotBlockAck(ns3::CtrlBAckResponseHeader const * blockAck, ns3::Mac48Address recipient) [member function]
    cls.add_method('GotBlockAck', 
                   'void', 
                   [param('ns3::CtrlBAckResponseHeader const *', 'blockAck'), param('ns3::Mac48Address', 'recipient')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::MissedBlockAck() [member function]
    cls.add_method('MissedBlockAck', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::GotAddBaResponse(ns3::MgtAddBaResponseHeader const * respHdr, ns3::Mac48Address recipient) [member function]
    cls.add_method('GotAddBaResponse', 
                   'void', 
                   [param('ns3::MgtAddBaResponseHeader const *', 'respHdr'), param('ns3::Mac48Address', 'recipient')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::GotDelBaFrame(ns3::MgtDelBaHeader const * delBaHdr, ns3::Mac48Address recipient) [member function]
    cls.add_method('GotDelBaFrame', 
                   'void', 
                   [param('ns3::MgtDelBaHeader const *', 'delBaHdr'), param('ns3::Mac48Address', 'recipient')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::MissedAck() [member function]
    cls.add_method('MissedAck', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::StartNext() [member function]
    cls.add_method('StartNext', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::Cancel() [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::RestartAccessIfNeeded() [member function]
    cls.add_method('RestartAccessIfNeeded', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::StartAccessIfNeeded() [member function]
    cls.add_method('StartAccessIfNeeded', 
                   'void', 
                   [])
    ## edca-txop-n.h: bool ns3::EdcaTxopN::NeedRts() [member function]
    cls.add_method('NeedRts', 
                   'bool', 
                   [])
    ## edca-txop-n.h: bool ns3::EdcaTxopN::NeedRtsRetransmission() [member function]
    cls.add_method('NeedRtsRetransmission', 
                   'bool', 
                   [])
    ## edca-txop-n.h: bool ns3::EdcaTxopN::NeedDataRetransmission() [member function]
    cls.add_method('NeedDataRetransmission', 
                   'bool', 
                   [])
    ## edca-txop-n.h: bool ns3::EdcaTxopN::NeedFragmentation() const [member function]
    cls.add_method('NeedFragmentation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h: uint32_t ns3::EdcaTxopN::GetNextFragmentSize() [member function]
    cls.add_method('GetNextFragmentSize', 
                   'uint32_t', 
                   [])
    ## edca-txop-n.h: uint32_t ns3::EdcaTxopN::GetFragmentSize() [member function]
    cls.add_method('GetFragmentSize', 
                   'uint32_t', 
                   [])
    ## edca-txop-n.h: uint32_t ns3::EdcaTxopN::GetFragmentOffset() [member function]
    cls.add_method('GetFragmentOffset', 
                   'uint32_t', 
                   [])
    ## edca-txop-n.h: bool ns3::EdcaTxopN::IsLastFragment() const [member function]
    cls.add_method('IsLastFragment', 
                   'bool', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h: void ns3::EdcaTxopN::NextFragment() [member function]
    cls.add_method('NextFragment', 
                   'void', 
                   [])
    ## edca-txop-n.h: ns3::Ptr<ns3::Packet> ns3::EdcaTxopN::GetFragmentPacket(ns3::WifiMacHeader * hdr) [member function]
    cls.add_method('GetFragmentPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::WifiMacHeader *', 'hdr')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetAccessCategory(ns3::AcIndex ac) [member function]
    cls.add_method('SetAccessCategory', 
                   'void', 
                   [param('ns3::AcIndex', 'ac')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::Queue(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('Queue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetMsduAggregator(ns3::Ptr<ns3::MsduAggregator> aggr) [member function]
    cls.add_method('SetMsduAggregator', 
                   'void', 
                   [param('ns3::Ptr< ns3::MsduAggregator >', 'aggr')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::PushFront(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('PushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::CompleteConfig() [member function]
    cls.add_method('CompleteConfig', 
                   'void', 
                   [])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetBlockAckThreshold(uint8_t threshold) [member function]
    cls.add_method('SetBlockAckThreshold', 
                   'void', 
                   [param('uint8_t', 'threshold')])
    ## edca-txop-n.h: uint8_t ns3::EdcaTxopN::GetBlockAckThreshold() const [member function]
    cls.add_method('GetBlockAckThreshold', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h: void ns3::EdcaTxopN::SetBlockAckInactivityTimeout(uint16_t timeout) [member function]
    cls.add_method('SetBlockAckInactivityTimeout', 
                   'void', 
                   [param('uint16_t', 'timeout')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::SendDelbaFrame(ns3::Mac48Address addr, uint8_t tid, bool byOriginator) [member function]
    cls.add_method('SendDelbaFrame', 
                   'void', 
                   [param('ns3::Mac48Address', 'addr'), param('uint8_t', 'tid'), param('bool', 'byOriginator')])
    ## edca-txop-n.h: void ns3::EdcaTxopN::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ErrorRateModel_methods(root_module, cls):
    ## error-rate-model.h: ns3::ErrorRateModel::ErrorRateModel() [constructor]
    cls.add_constructor([])
    ## error-rate-model.h: ns3::ErrorRateModel::ErrorRateModel(ns3::ErrorRateModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErrorRateModel const &', 'arg0')])
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
    ## error-rate-model.h: static ns3::TypeId ns3::ErrorRateModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3ExtendedSupportedRatesIE_methods(root_module, cls):
    ## supported-rates.h: ns3::ExtendedSupportedRatesIE::ExtendedSupportedRatesIE(ns3::ExtendedSupportedRatesIE const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ExtendedSupportedRatesIE const &', 'arg0')])
    ## supported-rates.h: ns3::ExtendedSupportedRatesIE::ExtendedSupportedRatesIE() [constructor]
    cls.add_constructor([])
    ## supported-rates.h: ns3::ExtendedSupportedRatesIE::ExtendedSupportedRatesIE(ns3::SupportedRates * rates) [constructor]
    cls.add_constructor([param('ns3::SupportedRates *', 'rates')])
    ## supported-rates.h: uint8_t ns3::ExtendedSupportedRatesIE::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## supported-rates.h: ns3::WifiInformationElementId ns3::ExtendedSupportedRatesIE::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h: uint8_t ns3::ExtendedSupportedRatesIE::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h: uint16_t ns3::ExtendedSupportedRatesIE::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## supported-rates.h: ns3::Buffer::Iterator ns3::ExtendedSupportedRatesIE::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## supported-rates.h: void ns3::ExtendedSupportedRatesIE::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3IdealWifiManager_methods(root_module, cls):
    ## ideal-wifi-manager.h: ns3::IdealWifiManager::IdealWifiManager(ns3::IdealWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IdealWifiManager const &', 'arg0')])
    ## ideal-wifi-manager.h: ns3::IdealWifiManager::IdealWifiManager() [constructor]
    cls.add_constructor([])
    ## ideal-wifi-manager.h: static ns3::TypeId ns3::IdealWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::SetupPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetupPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## ideal-wifi-manager.h: ns3::WifiRemoteStation * ns3::IdealWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: ns3::WifiMode ns3::IdealWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: ns3::WifiMode ns3::IdealWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: void ns3::IdealWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## ideal-wifi-manager.h: bool ns3::IdealWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3MacLow_methods(root_module, cls):
    ## mac-low.h: ns3::MacLow::MacLow(ns3::MacLow const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacLow const &', 'arg0')])
    ## mac-low.h: ns3::MacLow::MacLow() [constructor]
    cls.add_constructor([])
    ## mac-low.h: ns3::Time ns3::MacLow::CalculateTransmissionTime(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const * hdr, ns3::MacLowTransmissionParameters const & parameters) const [member function]
    cls.add_method('CalculateTransmissionTime', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr'), param('ns3::MacLowTransmissionParameters const &', 'parameters')], 
                   is_const=True)
    ## mac-low.h: void ns3::MacLow::CreateBlockAckAgreement(ns3::MgtAddBaResponseHeader const * respHdr, ns3::Mac48Address originator, uint16_t startingSeq) [member function]
    cls.add_method('CreateBlockAckAgreement', 
                   'void', 
                   [param('ns3::MgtAddBaResponseHeader const *', 'respHdr'), param('ns3::Mac48Address', 'originator'), param('uint16_t', 'startingSeq')])
    ## mac-low.h: void ns3::MacLow::DestroyBlockAckAgreement(ns3::Mac48Address originator, uint8_t tid) [member function]
    cls.add_method('DestroyBlockAckAgreement', 
                   'void', 
                   [param('ns3::Mac48Address', 'originator'), param('uint8_t', 'tid')])
    ## mac-low.h: ns3::Time ns3::MacLow::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mac-low.h: ns3::Mac48Address ns3::MacLow::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mac-low.h: ns3::Time ns3::MacLow::GetBasicBlockAckTimeout() const [member function]
    cls.add_method('GetBasicBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mac-low.h: ns3::Mac48Address ns3::MacLow::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mac-low.h: ns3::Time ns3::MacLow::GetCompressedBlockAckTimeout() const [member function]
    cls.add_method('GetCompressedBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mac-low.h: ns3::Time ns3::MacLow::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mac-low.h: ns3::Time ns3::MacLow::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mac-low.h: ns3::Time ns3::MacLow::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mac-low.h: ns3::Time ns3::MacLow::GetSlotTime() const [member function]
    cls.add_method('GetSlotTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mac-low.h: void ns3::MacLow::NotifySwitchingStartNow(ns3::Time duration) [member function]
    cls.add_method('NotifySwitchingStartNow', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## mac-low.h: void ns3::MacLow::ReceiveError(ns3::Ptr<ns3::Packet const> packet, double rxSnr) [member function]
    cls.add_method('ReceiveError', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('double', 'rxSnr')])
    ## mac-low.h: void ns3::MacLow::ReceiveOk(ns3::Ptr<ns3::Packet> packet, double rxSnr, ns3::WifiMode txMode, ns3::WifiPreamble preamble) [member function]
    cls.add_method('ReceiveOk', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode'), param('ns3::WifiPreamble', 'preamble')])
    ## mac-low.h: void ns3::MacLow::RegisterBlockAckListenerForAc(ns3::AcIndex ac, ns3::MacLowBlockAckEventListener * listener) [member function]
    cls.add_method('RegisterBlockAckListenerForAc', 
                   'void', 
                   [param('ns3::AcIndex', 'ac'), param('ns3::MacLowBlockAckEventListener *', 'listener')])
    ## mac-low.h: void ns3::MacLow::RegisterDcfListener(ns3::MacLowDcfListener * listener) [member function]
    cls.add_method('RegisterDcfListener', 
                   'void', 
                   [param('ns3::MacLowDcfListener *', 'listener')])
    ## mac-low.h: void ns3::MacLow::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')])
    ## mac-low.h: void ns3::MacLow::SetAddress(ns3::Mac48Address ad) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'ad')])
    ## mac-low.h: void ns3::MacLow::SetBasicBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetBasicBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')])
    ## mac-low.h: void ns3::MacLow::SetBssid(ns3::Mac48Address ad) [member function]
    cls.add_method('SetBssid', 
                   'void', 
                   [param('ns3::Mac48Address', 'ad')])
    ## mac-low.h: void ns3::MacLow::SetCompressedBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetCompressedBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')])
    ## mac-low.h: void ns3::MacLow::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')])
    ## mac-low.h: void ns3::MacLow::SetPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')])
    ## mac-low.h: void ns3::MacLow::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')])
    ## mac-low.h: void ns3::MacLow::SetRxCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::WifiMacHeader const*, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetRxCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::WifiMacHeader const *, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## mac-low.h: void ns3::MacLow::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')])
    ## mac-low.h: void ns3::MacLow::SetSlotTime(ns3::Time slotTime) [member function]
    cls.add_method('SetSlotTime', 
                   'void', 
                   [param('ns3::Time', 'slotTime')])
    ## mac-low.h: void ns3::MacLow::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> manager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'manager')])
    ## mac-low.h: void ns3::MacLow::StartTransmission(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const * hdr, ns3::MacLowTransmissionParameters parameters, ns3::MacLowTransmissionListener * listener) [member function]
    cls.add_method('StartTransmission', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr'), param('ns3::MacLowTransmissionParameters', 'parameters'), param('ns3::MacLowTransmissionListener *', 'listener')])
    ## mac-low.h: void ns3::MacLow::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3MgtBeaconHeader_methods(root_module, cls):
    ## mgt-headers.h: ns3::MgtBeaconHeader::MgtBeaconHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h: ns3::MgtBeaconHeader::MgtBeaconHeader(ns3::MgtBeaconHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtBeaconHeader const &', 'arg0')])
    return

def register_Ns3MinstrelWifiManager_methods(root_module, cls):
    ## minstrel-wifi-manager.h: ns3::MinstrelWifiManager::MinstrelWifiManager(ns3::MinstrelWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MinstrelWifiManager const &', 'arg0')])
    ## minstrel-wifi-manager.h: ns3::MinstrelWifiManager::MinstrelWifiManager() [constructor]
    cls.add_constructor([])
    ## minstrel-wifi-manager.h: static ns3::TypeId ns3::MinstrelWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## minstrel-wifi-manager.h: void ns3::MinstrelWifiManager::SetupPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetupPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## minstrel-wifi-manager.h: ns3::WifiRemoteStation * ns3::MinstrelWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: ns3::WifiMode ns3::MinstrelWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: ns3::WifiMode ns3::MinstrelWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: void ns3::MinstrelWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: void ns3::MinstrelWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: void ns3::MinstrelWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: void ns3::MinstrelWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: void ns3::MinstrelWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: void ns3::MinstrelWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: void ns3::MinstrelWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## minstrel-wifi-manager.h: bool ns3::MinstrelWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3MsduAggregator_methods(root_module, cls):
    ## msdu-aggregator.h: ns3::MsduAggregator::MsduAggregator() [constructor]
    cls.add_constructor([])
    ## msdu-aggregator.h: ns3::MsduAggregator::MsduAggregator(ns3::MsduAggregator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MsduAggregator const &', 'arg0')])
    ## msdu-aggregator.h: bool ns3::MsduAggregator::Aggregate(ns3::Ptr<ns3::Packet const> packet, ns3::Ptr<ns3::Packet> aggregatedPacket, ns3::Mac48Address src, ns3::Mac48Address dest) [member function]
    cls.add_method('Aggregate', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Ptr< ns3::Packet >', 'aggregatedPacket'), param('ns3::Mac48Address', 'src'), param('ns3::Mac48Address', 'dest')], 
                   is_pure_virtual=True, is_virtual=True)
    ## msdu-aggregator.h: static std::list<std::pair<ns3::Ptr<ns3::Packet>, ns3::AmsduSubframeHeader>, std::allocator<std::pair<ns3::Ptr<ns3::Packet>, ns3::AmsduSubframeHeader> > > ns3::MsduAggregator::Deaggregate(ns3::Ptr<ns3::Packet> aggregatedPacket) [member function]
    cls.add_method('Deaggregate', 
                   'std::list< std::pair< ns3::Ptr< ns3::Packet >, ns3::AmsduSubframeHeader > >', 
                   [param('ns3::Ptr< ns3::Packet >', 'aggregatedPacket')], 
                   is_static=True)
    ## msdu-aggregator.h: static ns3::TypeId ns3::MsduAggregator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3NistErrorRateModel_methods(root_module, cls):
    ## nist-error-rate-model.h: ns3::NistErrorRateModel::NistErrorRateModel(ns3::NistErrorRateModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NistErrorRateModel const &', 'arg0')])
    ## nist-error-rate-model.h: ns3::NistErrorRateModel::NistErrorRateModel() [constructor]
    cls.add_constructor([])
    ## nist-error-rate-model.h: double ns3::NistErrorRateModel::GetChunkSuccessRate(ns3::WifiMode mode, double snr, uint32_t nbits) const [member function]
    cls.add_method('GetChunkSuccessRate', 
                   'double', 
                   [param('ns3::WifiMode', 'mode'), param('double', 'snr'), param('uint32_t', 'nbits')], 
                   is_const=True, is_virtual=True)
    ## nist-error-rate-model.h: static ns3::TypeId ns3::NistErrorRateModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3OnoeWifiManager_methods(root_module, cls):
    ## onoe-wifi-manager.h: ns3::OnoeWifiManager::OnoeWifiManager(ns3::OnoeWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OnoeWifiManager const &', 'arg0')])
    ## onoe-wifi-manager.h: ns3::OnoeWifiManager::OnoeWifiManager() [constructor]
    cls.add_constructor([])
    ## onoe-wifi-manager.h: static ns3::TypeId ns3::OnoeWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## onoe-wifi-manager.h: ns3::WifiRemoteStation * ns3::OnoeWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: ns3::WifiMode ns3::OnoeWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: ns3::WifiMode ns3::OnoeWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: void ns3::OnoeWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## onoe-wifi-manager.h: bool ns3::OnoeWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RegularWifiMac_methods(root_module, cls):
    ## regular-wifi-mac.h: static ns3::TypeId ns3::RegularWifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## regular-wifi-mac.h: ns3::RegularWifiMac::RegularWifiMac() [constructor]
    cls.add_constructor([])
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: ns3::Time ns3::RegularWifiMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: ns3::Time ns3::RegularWifiMac::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: ns3::Time ns3::RegularWifiMac::GetSlot() const [member function]
    cls.add_method('GetSlot', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: ns3::Time ns3::RegularWifiMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: ns3::Time ns3::RegularWifiMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: ns3::Time ns3::RegularWifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: ns3::Mac48Address ns3::RegularWifiMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: ns3::Ssid ns3::RegularWifiMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetBssid(ns3::Mac48Address bssid) [member function]
    cls.add_method('SetBssid', 
                   'void', 
                   [param('ns3::Mac48Address', 'bssid')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: ns3::Mac48Address ns3::RegularWifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: bool ns3::RegularWifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: ns3::Ptr<ns3::WifiPhy> ns3::RegularWifiMac::GetWifiPhy() const [member function]
    cls.add_method('GetWifiPhy', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: ns3::Ptr<ns3::WifiRemoteStationManager> ns3::RegularWifiMac::GetWifiRemoteStationManager() const [member function]
    cls.add_method('GetWifiRemoteStationManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetBasicBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetBasicBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: ns3::Time ns3::RegularWifiMac::GetBasicBlockAckTimeout() const [member function]
    cls.add_method('GetBasicBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetCompressedBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetCompressedBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')], 
                   is_virtual=True)
    ## regular-wifi-mac.h: ns3::Time ns3::RegularWifiMac::GetCompressedBlockAckTimeout() const [member function]
    cls.add_method('GetCompressedBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::FinishConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('FinishConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetTypeOfStation(ns3::TypeOfStation type) [member function]
    cls.add_method('SetTypeOfStation', 
                   'void', 
                   [param('ns3::TypeOfStation', 'type')], 
                   visibility='protected')
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::TxOk(ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('TxOk', 
                   'void', 
                   [param('ns3::WifiMacHeader const &', 'hdr')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::TxFailed(ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('TxFailed', 
                   'void', 
                   [param('ns3::WifiMacHeader const &', 'hdr')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::ForwardUp(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address from, ns3::Mac48Address to) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address', 'from'), param('ns3::Mac48Address', 'to')], 
                   visibility='protected')
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::DeaggregateAmsduAndForward(ns3::Ptr<ns3::Packet> aggregatedPacket, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('DeaggregateAmsduAndForward', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'aggregatedPacket'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SendAddBaResponse(ns3::MgtAddBaRequestHeader const * reqHdr, ns3::Mac48Address originator) [member function]
    cls.add_method('SendAddBaResponse', 
                   'void', 
                   [param('ns3::MgtAddBaRequestHeader const *', 'reqHdr'), param('ns3::Mac48Address', 'originator')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h: void ns3::RegularWifiMac::SetQosSupported(bool enable) [member function]
    cls.add_method('SetQosSupported', 
                   'void', 
                   [param('bool', 'enable')], 
                   visibility='protected')
    ## regular-wifi-mac.h: bool ns3::RegularWifiMac::GetQosSupported() const [member function]
    cls.add_method('GetQosSupported', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    return

def register_Ns3RraaWifiManager_methods(root_module, cls):
    ## rraa-wifi-manager.h: ns3::RraaWifiManager::RraaWifiManager(ns3::RraaWifiManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RraaWifiManager const &', 'arg0')])
    ## rraa-wifi-manager.h: ns3::RraaWifiManager::RraaWifiManager() [constructor]
    cls.add_constructor([])
    ## rraa-wifi-manager.h: static ns3::TypeId ns3::RraaWifiManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rraa-wifi-manager.h: ns3::WifiRemoteStation * ns3::RraaWifiManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: ns3::WifiMode ns3::RraaWifiManager::DoGetDataMode(ns3::WifiRemoteStation * station, uint32_t size) [member function]
    cls.add_method('DoGetDataMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('uint32_t', 'size')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: ns3::WifiMode ns3::RraaWifiManager::DoGetRtsMode(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: bool ns3::RraaWifiManager::DoNeedRts(ns3::WifiRemoteStation * st, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedRts', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'st'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: void ns3::RraaWifiManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   visibility='private', is_virtual=True)
    ## rraa-wifi-manager.h: bool ns3::RraaWifiManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
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
    ## ssid.h: uint8_t ns3::Ssid::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ssid.h: ns3::WifiInformationElementId ns3::Ssid::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ssid.h: uint8_t ns3::Ssid::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
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
    ## ssid.h: void ns3::Ssid::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3SsidChecker_methods(root_module, cls):
    ## ssid.h: ns3::SsidChecker::SsidChecker() [constructor]
    cls.add_constructor([])
    ## ssid.h: ns3::SsidChecker::SsidChecker(ns3::SsidChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SsidChecker const &', 'arg0')])
    return

def register_Ns3SsidValue_methods(root_module, cls):
    ## ssid.h: ns3::SsidValue::SsidValue() [constructor]
    cls.add_constructor([])
    ## ssid.h: ns3::SsidValue::SsidValue(ns3::SsidValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SsidValue const &', 'arg0')])
    ## ssid.h: ns3::SsidValue::SsidValue(ns3::Ssid const & value) [constructor]
    cls.add_constructor([param('ns3::Ssid const &', 'value')])
    ## ssid.h: ns3::Ptr<ns3::AttributeValue> ns3::SsidValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ssid.h: bool ns3::SsidValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ssid.h: ns3::Ssid ns3::SsidValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## ssid.h: std::string ns3::SsidValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ssid.h: void ns3::SsidValue::Set(ns3::Ssid const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ssid const &', 'value')])
    return

def register_Ns3StaWifiMac_methods(root_module, cls):
    ## sta-wifi-mac.h: static ns3::TypeId ns3::StaWifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## sta-wifi-mac.h: ns3::StaWifiMac::StaWifiMac() [constructor]
    cls.add_constructor([])
    ## sta-wifi-mac.h: void ns3::StaWifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## sta-wifi-mac.h: void ns3::StaWifiMac::SetMaxMissedBeacons(uint32_t missed) [member function]
    cls.add_method('SetMaxMissedBeacons', 
                   'void', 
                   [param('uint32_t', 'missed')])
    ## sta-wifi-mac.h: void ns3::StaWifiMac::SetProbeRequestTimeout(ns3::Time timeout) [member function]
    cls.add_method('SetProbeRequestTimeout', 
                   'void', 
                   [param('ns3::Time', 'timeout')])
    ## sta-wifi-mac.h: void ns3::StaWifiMac::SetAssocRequestTimeout(ns3::Time timeout) [member function]
    cls.add_method('SetAssocRequestTimeout', 
                   'void', 
                   [param('ns3::Time', 'timeout')])
    ## sta-wifi-mac.h: void ns3::StaWifiMac::StartActiveAssociation() [member function]
    cls.add_method('StartActiveAssociation', 
                   'void', 
                   [])
    ## sta-wifi-mac.h: void ns3::StaWifiMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='private', is_virtual=True)
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
    ## supported-rates.h: uint8_t ns3::SupportedRates::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## supported-rates.h: ns3::WifiInformationElementId ns3::SupportedRates::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h: uint8_t ns3::SupportedRates::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
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
    ## supported-rates.h: void ns3::SupportedRates::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h: void ns3::SupportedRates::SetBasicRate(uint32_t bs) [member function]
    cls.add_method('SetBasicRate', 
                   'void', 
                   [param('uint32_t', 'bs')])
    ## supported-rates.h: ns3::SupportedRates::extended [variable]
    cls.add_instance_attribute('extended', 'ns3::ExtendedSupportedRatesIE', is_const=False)
    return

def register_Ns3WifiChannel_methods(root_module, cls):
    ## wifi-channel.h: ns3::WifiChannel::WifiChannel() [constructor]
    cls.add_constructor([])
    ## wifi-channel.h: ns3::WifiChannel::WifiChannel(ns3::WifiChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiChannel const &', 'arg0')])
    ## wifi-channel.h: static ns3::TypeId ns3::WifiChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3WifiModeChecker_methods(root_module, cls):
    ## wifi-mode.h: ns3::WifiModeChecker::WifiModeChecker() [constructor]
    cls.add_constructor([])
    ## wifi-mode.h: ns3::WifiModeChecker::WifiModeChecker(ns3::WifiModeChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeChecker const &', 'arg0')])
    return

def register_Ns3WifiModeValue_methods(root_module, cls):
    ## wifi-mode.h: ns3::WifiModeValue::WifiModeValue() [constructor]
    cls.add_constructor([])
    ## wifi-mode.h: ns3::WifiModeValue::WifiModeValue(ns3::WifiModeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeValue const &', 'arg0')])
    ## wifi-mode.h: ns3::WifiModeValue::WifiModeValue(ns3::WifiMode const & value) [constructor]
    cls.add_constructor([param('ns3::WifiMode const &', 'value')])
    ## wifi-mode.h: ns3::Ptr<ns3::AttributeValue> ns3::WifiModeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mode.h: bool ns3::WifiModeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## wifi-mode.h: ns3::WifiMode ns3::WifiModeValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-mode.h: std::string ns3::WifiModeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## wifi-mode.h: void ns3::WifiModeValue::Set(ns3::WifiMode const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::WifiMode const &', 'value')])
    return

def register_Ns3WifiNetDevice_methods(root_module, cls):
    ## wifi-net-device.h: ns3::WifiNetDevice::WifiNetDevice(ns3::WifiNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiNetDevice const &', 'arg0')])
    ## wifi-net-device.h: ns3::WifiNetDevice::WifiNetDevice() [constructor]
    cls.add_constructor([])
    ## wifi-net-device.h: void ns3::WifiNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Ptr<ns3::Channel> ns3::WifiNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: uint32_t ns3::WifiNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Ptr<ns3::WifiMac> ns3::WifiNetDevice::GetMac() const [member function]
    cls.add_method('GetMac', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_const=True)
    ## wifi-net-device.h: uint16_t ns3::WifiNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Address ns3::WifiNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: ns3::Ptr<ns3::Node> ns3::WifiNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
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
    ## wifi-net-device.h: static ns3::TypeId ns3::WifiNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## wifi-net-device.h: bool ns3::WifiNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetMac(ns3::Ptr<ns3::WifiMac> mac) [member function]
    cls.add_method('SetMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiMac >', 'mac')])
    ## wifi-net-device.h: bool ns3::WifiNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')])
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## wifi-net-device.h: void ns3::WifiNetDevice::SetRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> manager) [member function]
    cls.add_method('SetRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'manager')])
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
    ## wifi-net-device.h: void ns3::WifiNetDevice::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3YansErrorRateModel_methods(root_module, cls):
    ## yans-error-rate-model.h: ns3::YansErrorRateModel::YansErrorRateModel(ns3::YansErrorRateModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::YansErrorRateModel const &', 'arg0')])
    ## yans-error-rate-model.h: ns3::YansErrorRateModel::YansErrorRateModel() [constructor]
    cls.add_constructor([])
    ## yans-error-rate-model.h: double ns3::YansErrorRateModel::GetChunkSuccessRate(ns3::WifiMode mode, double snr, uint32_t nbits) const [member function]
    cls.add_method('GetChunkSuccessRate', 
                   'double', 
                   [param('ns3::WifiMode', 'mode'), param('double', 'snr'), param('uint32_t', 'nbits')], 
                   is_const=True, is_virtual=True)
    ## yans-error-rate-model.h: static ns3::TypeId ns3::YansErrorRateModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3YansWifiChannel_methods(root_module, cls):
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
    ## yans-wifi-channel.h: void ns3::YansWifiChannel::Send(ns3::Ptr<ns3::YansWifiPhy> sender, ns3::Ptr<ns3::Packet const> packet, double txPowerDbm, ns3::WifiMode wifiMode, ns3::WifiPreamble preamble) const [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::YansWifiPhy >', 'sender'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('double', 'txPowerDbm'), param('ns3::WifiMode', 'wifiMode'), param('ns3::WifiPreamble', 'preamble')], 
                   is_const=True)
    return

def register_Ns3AdhocWifiMac_methods(root_module, cls):
    ## adhoc-wifi-mac.h: static ns3::TypeId ns3::AdhocWifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## adhoc-wifi-mac.h: ns3::AdhocWifiMac::AdhocWifiMac() [constructor]
    cls.add_constructor([])
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## adhoc-wifi-mac.h: void ns3::AdhocWifiMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ApWifiMac_methods(root_module, cls):
    ## ap-wifi-mac.h: static ns3::TypeId ns3::ApWifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ap-wifi-mac.h: ns3::ApWifiMac::ApWifiMac() [constructor]
    cls.add_constructor([])
    ## ap-wifi-mac.h: void ns3::ApWifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## ap-wifi-mac.h: bool ns3::ApWifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::SetBeaconInterval(ns3::Time interval) [member function]
    cls.add_method('SetBeaconInterval', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## ap-wifi-mac.h: ns3::Time ns3::ApWifiMac::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::StartBeaconing() [member function]
    cls.add_method('StartBeaconing', 
                   'void', 
                   [])
    ## ap-wifi-mac.h: void ns3::ApWifiMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='private', is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::TxOk(ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('TxOk', 
                   'void', 
                   [param('ns3::WifiMacHeader const &', 'hdr')], 
                   visibility='private', is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::TxFailed(ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('TxFailed', 
                   'void', 
                   [param('ns3::WifiMacHeader const &', 'hdr')], 
                   visibility='private', is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::DeaggregateAmsduAndForward(ns3::Ptr<ns3::Packet> aggregatedPacket, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('DeaggregateAmsduAndForward', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'aggregatedPacket'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='private', is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## ap-wifi-mac.h: void ns3::ApWifiMac::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3DcaTxop_methods(root_module, cls):
    ## dca-txop.h: static ns3::TypeId ns3::DcaTxop::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dca-txop.h: ns3::DcaTxop::DcaTxop() [constructor]
    cls.add_constructor([])
    ## dca-txop.h: void ns3::DcaTxop::SetLow(ns3::Ptr<ns3::MacLow> low) [member function]
    cls.add_method('SetLow', 
                   'void', 
                   [param('ns3::Ptr< ns3::MacLow >', 'low')])
    ## dca-txop.h: void ns3::DcaTxop::SetManager(ns3::DcfManager * manager) [member function]
    cls.add_method('SetManager', 
                   'void', 
                   [param('ns3::DcfManager *', 'manager')])
    ## dca-txop.h: void ns3::DcaTxop::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> remoteManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'remoteManager')])
    ## dca-txop.h: void ns3::DcaTxop::SetTxOkCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## dca-txop.h: void ns3::DcaTxop::SetTxFailedCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxFailedCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## dca-txop.h: ns3::Ptr<ns3::WifiMacQueue> ns3::DcaTxop::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::WifiMacQueue >', 
                   [], 
                   is_const=True)
    ## dca-txop.h: void ns3::DcaTxop::SetMinCw(uint32_t minCw) [member function]
    cls.add_method('SetMinCw', 
                   'void', 
                   [param('uint32_t', 'minCw')], 
                   is_virtual=True)
    ## dca-txop.h: void ns3::DcaTxop::SetMaxCw(uint32_t maxCw) [member function]
    cls.add_method('SetMaxCw', 
                   'void', 
                   [param('uint32_t', 'maxCw')], 
                   is_virtual=True)
    ## dca-txop.h: void ns3::DcaTxop::SetAifsn(uint32_t aifsn) [member function]
    cls.add_method('SetAifsn', 
                   'void', 
                   [param('uint32_t', 'aifsn')], 
                   is_virtual=True)
    ## dca-txop.h: uint32_t ns3::DcaTxop::GetMinCw() const [member function]
    cls.add_method('GetMinCw', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dca-txop.h: uint32_t ns3::DcaTxop::GetMaxCw() const [member function]
    cls.add_method('GetMaxCw', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dca-txop.h: uint32_t ns3::DcaTxop::GetAifsn() const [member function]
    cls.add_method('GetAifsn', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dca-txop.h: void ns3::DcaTxop::Queue(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('Queue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## dca-txop.h: void ns3::DcaTxop::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## dca-txop.h: void ns3::DcaTxop::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
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
    ## qos-utils.h: extern uint8_t ns3::QosUtilsGetTidForPacket(ns3::Ptr<ns3::Packet const> packet) [free function]
    module.add_function('QosUtilsGetTidForPacket', 
                        'uint8_t', 
                        [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## qos-utils.h: extern bool ns3::QosUtilsIsOldPacket(uint16_t startingSeq, uint16_t seqNumber) [free function]
    module.add_function('QosUtilsIsOldPacket', 
                        'bool', 
                        [param('uint16_t', 'startingSeq'), param('uint16_t', 'seqNumber')])
    ## qos-utils.h: extern uint32_t ns3::QosUtilsMapSeqControlToUniqueInteger(uint16_t seqControl, uint16_t endSequence) [free function]
    module.add_function('QosUtilsMapSeqControlToUniqueInteger', 
                        'uint32_t', 
                        [param('uint16_t', 'seqControl'), param('uint16_t', 'endSequence')])
    ## qos-utils.h: extern ns3::AcIndex ns3::QosUtilsMapTidToAc(uint8_t tid) [free function]
    module.add_function('QosUtilsMapTidToAc', 
                        'ns3::AcIndex', 
                        [param('uint8_t', 'tid')])
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

