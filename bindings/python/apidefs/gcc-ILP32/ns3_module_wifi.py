from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## wifi-mac-header.h: ns3::WifiMacType [enumeration]
    module.add_enum('WifiMacType', ['WIFI_MAC_CTL_RTS', 'WIFI_MAC_CTL_CTS', 'WIFI_MAC_CTL_ACK', 'WIFI_MAC_CTL_BACKREQ', 'WIFI_MAC_CTL_BACKRESP', 'WIFI_MAC_MGT_BEACON', 'WIFI_MAC_MGT_ASSOCIATION_REQUEST', 'WIFI_MAC_MGT_ASSOCIATION_RESPONSE', 'WIFI_MAC_MGT_DISASSOCIATION', 'WIFI_MAC_MGT_REASSOCIATION_REQUEST', 'WIFI_MAC_MGT_REASSOCIATION_RESPONSE', 'WIFI_MAC_MGT_PROBE_REQUEST', 'WIFI_MAC_MGT_PROBE_RESPONSE', 'WIFI_MAC_MGT_AUTHENTICATION', 'WIFI_MAC_MGT_DEAUTHENTICATION', 'WIFI_MAC_MGT_ACTION', 'WIFI_MAC_MGT_ACTION_NO_ACK', 'WIFI_MAC_MGT_MULTIHOP_ACTION', 'WIFI_MAC_DATA', 'WIFI_MAC_DATA_CFACK', 'WIFI_MAC_DATA_CFPOLL', 'WIFI_MAC_DATA_CFACK_CFPOLL', 'WIFI_MAC_DATA_NULL', 'WIFI_MAC_DATA_NULL_CFACK', 'WIFI_MAC_DATA_NULL_CFPOLL', 'WIFI_MAC_DATA_NULL_CFACK_CFPOLL', 'WIFI_MAC_QOSDATA', 'WIFI_MAC_QOSDATA_CFACK', 'WIFI_MAC_QOSDATA_CFPOLL', 'WIFI_MAC_QOSDATA_CFACK_CFPOLL', 'WIFI_MAC_QOSDATA_NULL', 'WIFI_MAC_QOSDATA_NULL_CFPOLL', 'WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL'])
    ## wifi-preamble.h: ns3::WifiPreamble [enumeration]
    module.add_enum('WifiPreamble', ['WIFI_PREAMBLE_LONG', 'WIFI_PREAMBLE_SHORT'])
    ## wifi-phy-standard.h: ns3::WifiPhyStandard [enumeration]
    module.add_enum('WifiPhyStandard', ['WIFI_PHY_STANDARD_80211a', 'WIFI_PHY_STANDARD_80211b', 'WIFI_PHY_STANDARD_80211_10Mhz', 'WIFI_PHY_STANDARD_80211_5Mhz', 'WIFI_PHY_STANDARD_holland', 'WIFI_PHY_STANDARD_80211p_CCH', 'WIFI_PHY_STANDARD_80211p_SCH', 'WIFI_PHY_UNKNOWN'])
    ## qos-tag.h: ns3::UserPriority [enumeration]
    module.add_enum('UserPriority', ['UP_BK', 'UP_BE', 'UP_EE', 'UP_CL', 'UP_VI', 'UP_VO', 'UP_NC'])
    ## qos-utils.h: ns3::AccessClass [enumeration]
    module.add_enum('AccessClass', ['AC_VO', 'AC_VI', 'AC_BE', 'AC_BK', 'AC_BE_NQOS', 'AC_UNDEF'])
    ## edca-txop-n.h: ns3::TypeOfStation [enumeration]
    module.add_enum('TypeOfStation', ['STA', 'AP', 'ADHOC_STA'])
    ## capability-information.h: ns3::CapabilityInformation [class]
    module.add_class('CapabilityInformation')
    ## dcf-manager.h: ns3::DcfManager [class]
    module.add_class('DcfManager')
    ## dcf-manager.h: ns3::DcfState [class]
    module.add_class('DcfState', allow_subclassing=True)
    ## interference-helper.h: ns3::InterferenceHelper [class]
    module.add_class('InterferenceHelper', allow_subclassing=False)
    ## interference-helper.h: ns3::InterferenceHelper::SnrPer [struct]
    module.add_class('SnrPer', outer_class=root_module['ns3::InterferenceHelper'])
    ## mac-low.h: ns3::MacLowDcfListener [class]
    module.add_class('MacLowDcfListener', allow_subclassing=True)
    ## mac-low.h: ns3::MacLowTransmissionListener [class]
    module.add_class('MacLowTransmissionListener', allow_subclassing=True)
    ## mac-low.h: ns3::MacLowTransmissionParameters [class]
    module.add_class('MacLowTransmissionParameters')
    ## mac-rx-middle.h: ns3::MacRxMiddle [class]
    module.add_class('MacRxMiddle')
    ## minstrel-wifi-manager.h: ns3::RateInfo [struct]
    module.add_class('RateInfo')
    ## ssid.h: ns3::Ssid [class]
    module.add_class('Ssid')
    ## status-code.h: ns3::StatusCode [class]
    module.add_class('StatusCode')
    ## supported-rates.h: ns3::SupportedRates [class]
    module.add_class('SupportedRates')
    ## rraa-wifi-manager.h: ns3::ThresholdsItem [struct]
    module.add_class('ThresholdsItem')
    ## wifi-mode.h: ns3::WifiMode [class]
    module.add_class('WifiMode')
    ## wifi-mode.h: ns3::WifiMode::ModulationType [enumeration]
    module.add_enum('ModulationType', ['BPSK', 'QPSK', 'DBPSK', 'DQPSK', 'QAM', 'UNKNOWN'], outer_class=root_module['ns3::WifiMode'])
    ## wifi-mode.h: ns3::WifiModeFactory [class]
    module.add_class('WifiModeFactory')
