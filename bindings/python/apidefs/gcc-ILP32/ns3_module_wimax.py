from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## ul-job.h: ns3::ReqType [enumeration]
    module.add_enum('ReqType', ['DATA', 'UNICAST_POLLING'])
    ## cid.h: ns3::Cid [class]
    module.add_class('Cid')
    ## cid.h: ns3::Cid::Type [enumeration]
    module.add_enum('Type', ['BROADCAST', 'INITIAL_RANGING', 'BASIC', 'PRIMARY', 'TRANSPORT', 'MULTICAST', 'PADDING'], outer_class=root_module['ns3::Cid'])
    ## cid-factory.h: ns3::CidFactory [class]
    module.add_class('CidFactory')
    ## cs-parameters.h: ns3::CsParameters [class]
    module.add_class('CsParameters')
    ## cs-parameters.h: ns3::CsParameters::Action [enumeration]
    module.add_enum('Action', ['ADD', 'REPLACE', 'DELETE'], outer_class=root_module['ns3::CsParameters'])
    ## dl-mac-messages.h: ns3::DcdChannelEncodings [class]
    module.add_class('DcdChannelEncodings', allow_subclassing=True)
    ## ofdm-downlink-frame-prefix.h: ns3::DlFramePrefixIe [class]
    module.add_class('DlFramePrefixIe')
    ## ipcs-classifier-record.h: ns3::IpcsClassifierRecord [class]
    module.add_class('IpcsClassifierRecord')
    ## dl-mac-messages.h: ns3::OfdmDcdChannelEncodings [class]
    module.add_class('OfdmDcdChannelEncodings', parent=root_module['ns3::DcdChannelEncodings'])
    ## dl-mac-messages.h: ns3::OfdmDlBurstProfile [class]
    module.add_class('OfdmDlBurstProfile')
    ## dl-mac-messages.h: ns3::OfdmDlBurstProfile::Diuc [enumeration]
    module.add_enum('Diuc', ['DIUC_STC_ZONE', 'DIUC_BURST_PROFILE_1', 'DIUC_BURST_PROFILE_2', 'DIUC_BURST_PROFILE_3', 'DIUC_BURST_PROFILE_4', 'DIUC_BURST_PROFILE_5', 'DIUC_BURST_PROFILE_6', 'DIUC_BURST_PROFILE_7', 'DIUC_BURST_PROFILE_8', 'DIUC_BURST_PROFILE_9', 'DIUC_BURST_PROFILE_10', 'DIUC_BURST_PROFILE_11', 'DIUC_GAP', 'DIUC_END_OF_MAP'], outer_class=root_module['ns3::OfdmDlBurstProfile'])
    ## dl-mac-messages.h: ns3::OfdmDlMapIe [class]
    module.add_class('OfdmDlMapIe')
    ## ul-mac-messages.h: ns3::OfdmUlBurstProfile [class]
    module.add_class('OfdmUlBurstProfile')
    ## ul-mac-messages.h: ns3::OfdmUlBurstProfile::Uiuc [enumeration]
    module.add_enum('Uiuc', ['UIUC_INITIAL_RANGING', 'UIUC_REQ_REGION_FULL', 'UIUC_REQ_REGION_FOCUSED', 'UIUC_FOCUSED_CONTENTION_IE', 'UIUC_BURST_PROFILE_5', 'UIUC_BURST_PROFILE_6', 'UIUC_BURST_PROFILE_7', 'UIUC_BURST_PROFILE_8', 'UIUC_BURST_PROFILE_9', 'UIUC_BURST_PROFILE_10', 'UIUC_BURST_PROFILE_11', 'UIUC_BURST_PROFILE_12', 'UIUC_SUBCH_NETWORK_ENTRY', 'UIUC_END_OF_MAP'], outer_class=root_module['ns3::OfdmUlBurstProfile'])
    ## ul-mac-messages.h: ns3::OfdmUlMapIe [class]
    module.add_class('OfdmUlMapIe')
    ## snr-to-block-error-rate-manager.h: ns3::SNRToBlockErrorRateManager [class]
    module.add_class('SNRToBlockErrorRateManager')
    ## snr-to-block-error-rate-record.h: ns3::SNRToBlockErrorRateRecord [class]
    module.add_class('SNRToBlockErrorRateRecord')
    ## ss-record.h: ns3::SSRecord [class]
    module.add_class('SSRecord')
    ## send-params.h: ns3::SendParams [class]
    module.add_class('SendParams')
    ## service-flow.h: ns3::ServiceFlow [class]
    module.add_class('ServiceFlow')
    ## service-flow.h: ns3::ServiceFlow::Direction [enumeration]
    module.add_enum('Direction', ['SF_DIRECTION_DOWN', 'SF_DIRECTION_UP'], outer_class=root_module['ns3::ServiceFlow'])
    ## service-flow.h: ns3::ServiceFlow::Type [enumeration]
    module.add_enum('Type', ['SF_TYPE_PROVISIONED', 'SF_TYPE_ADMITTED', 'SF_TYPE_ACTIVE'], outer_class=root_module['ns3::ServiceFlow'])
    ## service-flow.h: ns3::ServiceFlow::SchedulingType [enumeration]
    module.add_enum('SchedulingType', ['SF_TYPE_NONE', 'SF_TYPE_UNDEF', 'SF_TYPE_BE', 'SF_TYPE_NRTPS', 'SF_TYPE_RTPS', 'SF_TYPE_UGS', 'SF_TYPE_ALL'], outer_class=root_module['ns3::ServiceFlow'])
    ## service-flow.h: ns3::ServiceFlow::CsSpecification [enumeration]
    module.add_enum('CsSpecification', ['ATM', 'IPV4', 'IPV6', 'ETHERNET', 'VLAN', 'IPV4_OVER_ETHERNET', 'IPV6_OVER_ETHERNET', 'IPV4_OVER_VLAN', 'IPV6_OVER_VLAN'], outer_class=root_module['ns3::ServiceFlow'])
    ## service-flow.h: ns3::ServiceFlow::ModulationType [enumeration]
    module.add_enum('ModulationType', ['MODULATION_TYPE_BPSK_12', 'MODULATION_TYPE_QPSK_12', 'MODULATION_TYPE_QPSK_34', 'MODULATION_TYPE_QAM16_12', 'MODULATION_TYPE_QAM16_34', 'MODULATION_TYPE_QAM64_23', 'MODULATION_TYPE_QAM64_34'], outer_class=root_module['ns3::ServiceFlow'])
    ## service-flow-record.h: ns3::ServiceFlowRecord [class]
    module.add_class('ServiceFlowRecord')
    ## wimax-tlv.h: ns3::TlvValue [class]
    module.add_class('TlvValue', allow_subclassing=True)
    ## wimax-tlv.h: ns3::TosTlvValue [class]
    module.add_class('TosTlvValue', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h: ns3::U16TlvValue [class]
    module.add_class('U16TlvValue', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h: ns3::U32TlvValue [class]
    module.add_class('U32TlvValue', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h: ns3::U8TlvValue [class]
    module.add_class('U8TlvValue', parent=root_module['ns3::TlvValue'])
    ## ul-mac-messages.h: ns3::UcdChannelEncodings [class]
    module.add_class('UcdChannelEncodings', allow_subclassing=True)
    ## wimax-tlv.h: ns3::VectorTlvValue [class]
    module.add_class('VectorTlvValue', parent=root_module['ns3::TlvValue'])
    ## wimax-helper.h: ns3::WimaxHelper [class]
    module.add_class('WimaxHelper', parent=[root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## wimax-helper.h: ns3::WimaxHelper::NetDeviceType [enumeration]
    module.add_enum('NetDeviceType', ['DEVICE_TYPE_SUBSCRIBER_STATION', 'DEVICE_TYPE_BASE_STATION'], outer_class=root_module['ns3::WimaxHelper'])
    ## wimax-helper.h: ns3::WimaxHelper::PhyType [enumeration]
    module.add_enum('PhyType', ['SIMPLE_PHY_TYPE_OFDM'], outer_class=root_module['ns3::WimaxHelper'])
    ## wimax-helper.h: ns3::WimaxHelper::SchedulerType [enumeration]
    module.add_enum('SchedulerType', ['SCHED_TYPE_SIMPLE', 'SCHED_TYPE_RTPS', 'SCHED_TYPE_MBQOS'], outer_class=root_module['ns3::WimaxHelper'])
    ## simple-ofdm-send-param.h: ns3::simpleOfdmSendParam [class]
    module.add_class('simpleOfdmSendParam')
    ## wimax-tlv.h: ns3::ClassificationRuleVectorTlvValue [class]
    module.add_class('ClassificationRuleVectorTlvValue', parent=root_module['ns3::VectorTlvValue'])
    ## wimax-tlv.h: ns3::ClassificationRuleVectorTlvValue::ClassificationRuleTlvType [enumeration]
    module.add_enum('ClassificationRuleTlvType', ['Priority', 'ToS', 'Protocol', 'IP_src', 'IP_dst', 'Port_src', 'Port_dst', 'Index'], outer_class=root_module['ns3::ClassificationRuleVectorTlvValue'])
    ## wimax-tlv.h: ns3::CsParamVectorTlvValue [class]
    module.add_class('CsParamVectorTlvValue', parent=root_module['ns3::VectorTlvValue'])
    ## wimax-tlv.h: ns3::CsParamVectorTlvValue::Type [enumeration]
    module.add_enum('Type', ['Classifier_DSC_Action', 'Packet_Classification_Rule'], outer_class=root_module['ns3::CsParamVectorTlvValue'])
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue [class]
    module.add_class('Ipv4AddressTlvValue', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue::ipv4Addr [struct]
    module.add_class('ipv4Addr', outer_class=root_module['ns3::Ipv4AddressTlvValue'])
    ## wimax-mac-header.h: ns3::MacHeaderType [class]
    module.add_class('MacHeaderType', parent=root_module['ns3::Header'])
    ## wimax-mac-header.h: ns3::MacHeaderType::HeaderType [enumeration]
    module.add_enum('HeaderType', ['HEADER_TYPE_GENERIC', 'HEADER_TYPE_BANDWIDTH'], outer_class=root_module['ns3::MacHeaderType'])
    ## mac-messages.h: ns3::ManagementMessageType [class]
    module.add_class('ManagementMessageType', parent=root_module['ns3::Header'])
    ## mac-messages.h: ns3::ManagementMessageType::MessageType [enumeration]
    module.add_enum('MessageType', ['MESSAGE_TYPE_UCD', 'MESSAGE_TYPE_DCD', 'MESSAGE_TYPE_DL_MAP', 'MESSAGE_TYPE_UL_MAP', 'MESSAGE_TYPE_RNG_REQ', 'MESSAGE_TYPE_RNG_RSP', 'MESSAGE_TYPE_REG_REQ', 'MESSAGE_TYPE_REG_RSP', 'MESSAGE_TYPE_DSA_REQ', 'MESSAGE_TYPE_DSA_RSP', 'MESSAGE_TYPE_DSA_ACK'], outer_class=root_module['ns3::ManagementMessageType'])
    ## ofdm-downlink-frame-prefix.h: ns3::OfdmDownlinkFramePrefix [class]
    module.add_class('OfdmDownlinkFramePrefix', parent=root_module['ns3::Header'])
    ## send-params.h: ns3::OfdmSendParams [class]
    module.add_class('OfdmSendParams', parent=root_module['ns3::SendParams'])
    ## ul-mac-messages.h: ns3::OfdmUcdChannelEncodings [class]
    module.add_class('OfdmUcdChannelEncodings', parent=root_module['ns3::UcdChannelEncodings'])
    ## wimax-tlv.h: ns3::PortRangeTlvValue [class]
    module.add_class('PortRangeTlvValue', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h: ns3::PortRangeTlvValue::PortRange [struct]
    module.add_class('PortRange', outer_class=root_module['ns3::PortRangeTlvValue'])
    ## ul-job.h: ns3::PriorityUlJob [class]
    module.add_class('PriorityUlJob', parent=root_module['ns3::Object'])
    ## wimax-tlv.h: ns3::ProtocolTlvValue [class]
    module.add_class('ProtocolTlvValue', parent=root_module['ns3::TlvValue'])
    ## mac-messages.h: ns3::RngReq [class]
    module.add_class('RngReq', parent=root_module['ns3::Header'])
    ## mac-messages.h: ns3::RngRsp [class]
    module.add_class('RngRsp', parent=root_module['ns3::Header'])
    ## ss-manager.h: ns3::SSManager [class]
    module.add_class('SSManager', parent=root_module['ns3::Object'])
    ## service-flow-manager.h: ns3::ServiceFlowManager [class]
    module.add_class('ServiceFlowManager', parent=root_module['ns3::Object'])
    ## service-flow-manager.h: ns3::ServiceFlowManager::ConfirmationCode [enumeration]
    module.add_enum('ConfirmationCode', ['CONFIRMATION_CODE_SUCCESS', 'CONFIRMATION_CODE_REJECT'], outer_class=root_module['ns3::ServiceFlowManager'])
    ## wimax-tlv.h: ns3::SfVectorTlvValue [class]
    module.add_class('SfVectorTlvValue', parent=root_module['ns3::VectorTlvValue'])
    ## wimax-tlv.h: ns3::SfVectorTlvValue::Type [enumeration]
    module.add_enum('Type', ['SFID', 'CID', 'Service_Class_Name', 'reserved1', 'QoS_Parameter_Set_Type', 'Traffic_Priority', 'Maximum_Sustained_Traffic_Rate', 'Maximum_Traffic_Burst', 'Minimum_Reserved_Traffic_Rate', 'Minimum_Tolerable_Traffic_Rate', 'Service_Flow_Scheduling_Type', 'Request_Transmission_Policy', 'Tolerated_Jitter', 'Maximum_Latency', 'Fixed_length_versus_Variable_length_SDU_Indicator', 'SDU_Size', 'Target_SAID', 'ARQ_Enable', 'ARQ_WINDOW_SIZE', 'ARQ_RETRY_TIMEOUT_Transmitter_Delay', 'ARQ_RETRY_TIMEOUT_Receiver_Delay', 'ARQ_BLOCK_LIFETIME', 'ARQ_SYNC_LOSS', 'ARQ_DELIVER_IN_ORDER', 'ARQ_PURGE_TIMEOUT', 'ARQ_BLOCK_SIZE', 'reserved2', 'CS_Specification', 'IPV4_CS_Parameters'], outer_class=root_module['ns3::SfVectorTlvValue'])
    ## ss-service-flow-manager.h: ns3::SsServiceFlowManager [class]
    module.add_class('SsServiceFlowManager', parent=root_module['ns3::ServiceFlowManager'])
    ## ss-service-flow-manager.h: ns3::SsServiceFlowManager::ConfirmationCode [enumeration]
    module.add_enum('ConfirmationCode', ['CONFIRMATION_CODE_SUCCESS', 'CONFIRMATION_CODE_REJECT'], outer_class=root_module['ns3::SsServiceFlowManager'])
    ## wimax-tlv.h: ns3::Tlv [class]
    module.add_class('Tlv', parent=root_module['ns3::Header'])
    ## wimax-tlv.h: ns3::Tlv::CommonTypes [enumeration]
    module.add_enum('CommonTypes', ['HMAC_TUPLE', 'MAC_VERSION_ENCODING', 'CURRENT_TRANSMIT_POWER', 'DOWNLINK_SERVICE_FLOW', 'UPLINK_SERVICE_FLOW', 'VENDOR_ID_EMCODING', 'VENDOR_SPECIFIC_INFORMATION'], outer_class=root_module['ns3::Tlv'])
    ## ul-mac-messages.h: ns3::Ucd [class]
    module.add_class('Ucd', parent=root_module['ns3::Header'])
    ## ul-job.h: ns3::UlJob [class]
    module.add_class('UlJob', parent=root_module['ns3::Object'])
    ## ul-job.h: ns3::UlJob::JobPriority [enumeration]
    module.add_enum('JobPriority', ['LOW', 'INTERMEDIATE', 'HIGH'], outer_class=root_module['ns3::UlJob'])
    ## ul-mac-messages.h: ns3::UlMap [class]
    module.add_class('UlMap', parent=root_module['ns3::Header'])
    ## bs-uplink-scheduler.h: ns3::UplinkScheduler [class]
    module.add_class('UplinkScheduler', parent=root_module['ns3::Object'])
    ## bs-uplink-scheduler-mbqos.h: ns3::UplinkSchedulerMBQoS [class]
    module.add_class('UplinkSchedulerMBQoS', parent=root_module['ns3::UplinkScheduler'])
    ## bs-uplink-scheduler-rtps.h: ns3::UplinkSchedulerRtps [class]
    module.add_class('UplinkSchedulerRtps', parent=root_module['ns3::UplinkScheduler'])
    ## bs-uplink-scheduler-simple.h: ns3::UplinkSchedulerSimple [class]
    module.add_class('UplinkSchedulerSimple', parent=root_module['ns3::UplinkScheduler'])
    ## wimax-connection.h: ns3::WimaxConnection [class]
    module.add_class('WimaxConnection', parent=root_module['ns3::Object'])
    ## wimax-mac-queue.h: ns3::WimaxMacQueue [class]
    module.add_class('WimaxMacQueue', parent=root_module['ns3::Object'])
    ## wimax-mac-to-mac-header.h: ns3::WimaxMacToMacHeader [class]
    module.add_class('WimaxMacToMacHeader', parent=root_module['ns3::Header'])
    ## wimax-phy.h: ns3::WimaxPhy [class]
    module.add_class('WimaxPhy', parent=root_module['ns3::Object'])
    ## wimax-phy.h: ns3::WimaxPhy::ModulationType [enumeration]
    module.add_enum('ModulationType', ['MODULATION_TYPE_BPSK_12', 'MODULATION_TYPE_QPSK_12', 'MODULATION_TYPE_QPSK_34', 'MODULATION_TYPE_QAM16_12', 'MODULATION_TYPE_QAM16_34', 'MODULATION_TYPE_QAM64_23', 'MODULATION_TYPE_QAM64_34'], outer_class=root_module['ns3::WimaxPhy'])
    ## wimax-phy.h: ns3::WimaxPhy::PhyState [enumeration]
    module.add_enum('PhyState', ['PHY_STATE_IDLE', 'PHY_STATE_SCANNING', 'PHY_STATE_TX', 'PHY_STATE_RX'], outer_class=root_module['ns3::WimaxPhy'])
    ## wimax-phy.h: ns3::WimaxPhy::PhyType [enumeration]
    module.add_enum('PhyType', ['SimpleWimaxPhy', 'simpleOfdmWimaxPhy'], outer_class=root_module['ns3::WimaxPhy'])
    ## bs-scheduler.h: ns3::BSScheduler [class]
    module.add_class('BSScheduler', parent=root_module['ns3::Object'])
    ## bs-scheduler-rtps.h: ns3::BSSchedulerRtps [class]
    module.add_class('BSSchedulerRtps', parent=root_module['ns3::BSScheduler'])
    ## bs-scheduler-simple.h: ns3::BSSchedulerSimple [class]
    module.add_class('BSSchedulerSimple', parent=root_module['ns3::BSScheduler'])
    ## wimax-mac-header.h: ns3::BandwidthRequestHeader [class]
    module.add_class('BandwidthRequestHeader', parent=root_module['ns3::Header'])
    ## wimax-mac-header.h: ns3::BandwidthRequestHeader::HeaderType [enumeration]
    module.add_enum('HeaderType', ['HEADER_TYPE_INCREMENTAL', 'HEADER_TYPE_AGGREGATE'], outer_class=root_module['ns3::BandwidthRequestHeader'])
    ## bs-service-flow-manager.h: ns3::BsServiceFlowManager [class]
    module.add_class('BsServiceFlowManager', parent=root_module['ns3::ServiceFlowManager'])
    ## bs-service-flow-manager.h: ns3::BsServiceFlowManager::ConfirmationCode [enumeration]
    module.add_enum('ConfirmationCode', ['CONFIRMATION_CODE_SUCCESS', 'CONFIRMATION_CODE_REJECT'], outer_class=root_module['ns3::BsServiceFlowManager'])
    ## connection-manager.h: ns3::ConnectionManager [class]
    module.add_class('ConnectionManager', parent=root_module['ns3::Object'])
    ## dl-mac-messages.h: ns3::Dcd [class]
    module.add_class('Dcd', parent=root_module['ns3::Header'])
    ## dl-mac-messages.h: ns3::DlMap [class]
    module.add_class('DlMap', parent=root_module['ns3::Header'])
    ## mac-messages.h: ns3::DsaAck [class]
    module.add_class('DsaAck', parent=root_module['ns3::Header'])
    ## mac-messages.h: ns3::DsaReq [class]
    module.add_class('DsaReq', parent=root_module['ns3::Header'])
    ## mac-messages.h: ns3::DsaRsp [class]
    module.add_class('DsaRsp', parent=root_module['ns3::Header'])
    ## wimax-mac-header.h: ns3::FragmentationSubheader [class]
    module.add_class('FragmentationSubheader', parent=root_module['ns3::Header'])
    ## wimax-mac-header.h: ns3::GenericMacHeader [class]
    module.add_class('GenericMacHeader', parent=root_module['ns3::Header'])
    ## wimax-mac-header.h: ns3::GrantManagementSubheader [class]
    module.add_class('GrantManagementSubheader', parent=root_module['ns3::Header'])
    ## ipcs-classifier.h: ns3::IpcsClassifier [class]
    module.add_class('IpcsClassifier', parent=root_module['ns3::Object'])
    ## simple-ofdm-wimax-phy.h: ns3::SimpleOfdmWimaxPhy [class]
    module.add_class('SimpleOfdmWimaxPhy', parent=root_module['ns3::WimaxPhy'])
    ## simple-ofdm-wimax-phy.h: ns3::SimpleOfdmWimaxPhy::FrameDurationCode [enumeration]
    module.add_enum('FrameDurationCode', ['FRAME_DURATION_2_POINT_5_MS', 'FRAME_DURATION_4_MS', 'FRAME_DURATION_5_MS', 'FRAME_DURATION_8_MS', 'FRAME_DURATION_10_MS', 'FRAME_DURATION_12_POINT_5_MS', 'FRAME_DURATION_20_MS'], outer_class=root_module['ns3::SimpleOfdmWimaxPhy'])
    ## wimax-channel.h: ns3::WimaxChannel [class]
    module.add_class('WimaxChannel', parent=root_module['ns3::Channel'])
    ## wimax-net-device.h: ns3::WimaxNetDevice [class]
    module.add_class('WimaxNetDevice', parent=root_module['ns3::NetDevice'])
    ## wimax-net-device.h: ns3::WimaxNetDevice::Direction [enumeration]
    module.add_enum('Direction', ['DIRECTION_DOWNLINK', 'DIRECTION_UPLINK'], outer_class=root_module['ns3::WimaxNetDevice'])
    ## wimax-net-device.h: ns3::WimaxNetDevice::RangingStatus [enumeration]
    module.add_enum('RangingStatus', ['RANGING_STATUS_EXPIRED', 'RANGING_STATUS_CONTINUE', 'RANGING_STATUS_ABORT', 'RANGING_STATUS_SUCCESS'], outer_class=root_module['ns3::WimaxNetDevice'])
    ## bs-net-device.h: ns3::BaseStationNetDevice [class]
    module.add_class('BaseStationNetDevice', parent=root_module['ns3::WimaxNetDevice'])
    ## bs-net-device.h: ns3::BaseStationNetDevice::State [enumeration]
    module.add_enum('State', ['BS_STATE_DL_SUB_FRAME', 'BS_STATE_UL_SUB_FRAME', 'BS_STATE_TTG', 'BS_STATE_RTG'], outer_class=root_module['ns3::BaseStationNetDevice'])
    ## bs-net-device.h: ns3::BaseStationNetDevice::MacPreamble [enumeration]
    module.add_enum('MacPreamble', ['SHORT_PREAMBLE', 'LONG_PREAMBLE'], outer_class=root_module['ns3::BaseStationNetDevice'])
    ## simple-ofdm-wimax-channel.h: ns3::SimpleOfdmWimaxChannel [class]
    module.add_class('SimpleOfdmWimaxChannel', parent=root_module['ns3::WimaxChannel'])
    ## simple-ofdm-wimax-channel.h: ns3::SimpleOfdmWimaxChannel::PropModel [enumeration]
    module.add_enum('PropModel', ['RANDOM_PROPAGATION', 'FRIIS_PROPAGATION', 'LOG_DISTANCE_PROPAGATION', 'COST231_PROPAGATION'], outer_class=root_module['ns3::SimpleOfdmWimaxChannel'])
    ## ss-net-device.h: ns3::SubscriberStationNetDevice [class]
    module.add_class('SubscriberStationNetDevice', parent=root_module['ns3::WimaxNetDevice'])
    ## ss-net-device.h: ns3::SubscriberStationNetDevice::State [enumeration]
    module.add_enum('State', ['SS_STATE_IDLE', 'SS_STATE_SCANNING', 'SS_STATE_SYNCHRONIZING', 'SS_STATE_ACQUIRING_PARAMETERS', 'SS_STATE_WAITING_REG_RANG_INTRVL', 'SS_STATE_WAITING_INV_RANG_INTRVL', 'SS_STATE_WAITING_RNG_RSP', 'SS_STATE_ADJUSTING_PARAMETERS', 'SS_STATE_REGISTERED', 'SS_STATE_TRANSMITTING', 'SS_STATE_STOPPED'], outer_class=root_module['ns3::SubscriberStationNetDevice'])
    ## ss-net-device.h: ns3::SubscriberStationNetDevice::EventType [enumeration]
    module.add_enum('EventType', ['EVENT_NONE', 'EVENT_WAIT_FOR_RNG_RSP', 'EVENT_DL_MAP_SYNC_TIMEOUT', 'EVENT_LOST_DL_MAP', 'EVENT_LOST_UL_MAP', 'EVENT_DCD_WAIT_TIMEOUT', 'EVENT_UCD_WAIT_TIMEOUT', 'EVENT_RANG_OPP_WAIT_TIMEOUT'], outer_class=root_module['ns3::SubscriberStationNetDevice'])
    module.add_container('std::vector< ns3::DlFramePrefixIe >', 'ns3::DlFramePrefixIe', container_type='vector')
    module.add_container('std::vector< ns3::OfdmUlBurstProfile >', 'ns3::OfdmUlBurstProfile', container_type='vector')
    module.add_container('std::list< ns3::OfdmUlMapIe >', 'ns3::OfdmUlMapIe', container_type='list')
    module.add_container('std::deque< ns3::WimaxMacQueue::QueueElement >', 'ns3::WimaxMacQueue::QueueElement', container_type='dequeue')
    module.add_container('std::vector< ns3::OfdmDlBurstProfile >', 'ns3::OfdmDlBurstProfile', container_type='vector')
    module.add_container('std::list< ns3::OfdmDlMapIe >', 'ns3::OfdmDlMapIe', container_type='list')
    typehandlers.add_type_alias('std::vector< bool, std::allocator< bool > >', 'ns3::bvec')
    typehandlers.add_type_alias('std::vector< bool, std::allocator< bool > >*', 'ns3::bvec*')
    typehandlers.add_type_alias('std::vector< bool, std::allocator< bool > >&', 'ns3::bvec&')
    
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
    register_Ns3Cid_methods(root_module, root_module['ns3::Cid'])
    register_Ns3CidFactory_methods(root_module, root_module['ns3::CidFactory'])
    register_Ns3CsParameters_methods(root_module, root_module['ns3::CsParameters'])
    register_Ns3DcdChannelEncodings_methods(root_module, root_module['ns3::DcdChannelEncodings'])
    register_Ns3DlFramePrefixIe_methods(root_module, root_module['ns3::DlFramePrefixIe'])
    register_Ns3IpcsClassifierRecord_methods(root_module, root_module['ns3::IpcsClassifierRecord'])
    register_Ns3OfdmDcdChannelEncodings_methods(root_module, root_module['ns3::OfdmDcdChannelEncodings'])
    register_Ns3OfdmDlBurstProfile_methods(root_module, root_module['ns3::OfdmDlBurstProfile'])
    register_Ns3OfdmDlMapIe_methods(root_module, root_module['ns3::OfdmDlMapIe'])
    register_Ns3OfdmUlBurstProfile_methods(root_module, root_module['ns3::OfdmUlBurstProfile'])
    register_Ns3OfdmUlMapIe_methods(root_module, root_module['ns3::OfdmUlMapIe'])
    register_Ns3SNRToBlockErrorRateManager_methods(root_module, root_module['ns3::SNRToBlockErrorRateManager'])
    register_Ns3SNRToBlockErrorRateRecord_methods(root_module, root_module['ns3::SNRToBlockErrorRateRecord'])
    register_Ns3SSRecord_methods(root_module, root_module['ns3::SSRecord'])
    register_Ns3SendParams_methods(root_module, root_module['ns3::SendParams'])
    register_Ns3ServiceFlow_methods(root_module, root_module['ns3::ServiceFlow'])
    register_Ns3ServiceFlowRecord_methods(root_module, root_module['ns3::ServiceFlowRecord'])
    register_Ns3TlvValue_methods(root_module, root_module['ns3::TlvValue'])
    register_Ns3TosTlvValue_methods(root_module, root_module['ns3::TosTlvValue'])
    register_Ns3U16TlvValue_methods(root_module, root_module['ns3::U16TlvValue'])
    register_Ns3U32TlvValue_methods(root_module, root_module['ns3::U32TlvValue'])
    register_Ns3U8TlvValue_methods(root_module, root_module['ns3::U8TlvValue'])
    register_Ns3UcdChannelEncodings_methods(root_module, root_module['ns3::UcdChannelEncodings'])
    register_Ns3VectorTlvValue_methods(root_module, root_module['ns3::VectorTlvValue'])
    register_Ns3WimaxHelper_methods(root_module, root_module['ns3::WimaxHelper'])
    register_Ns3SimpleOfdmSendParam_methods(root_module, root_module['ns3::simpleOfdmSendParam'])
    register_Ns3ClassificationRuleVectorTlvValue_methods(root_module, root_module['ns3::ClassificationRuleVectorTlvValue'])
    register_Ns3CsParamVectorTlvValue_methods(root_module, root_module['ns3::CsParamVectorTlvValue'])
    register_Ns3Ipv4AddressTlvValue_methods(root_module, root_module['ns3::Ipv4AddressTlvValue'])
    register_Ns3Ipv4AddressTlvValueIpv4Addr_methods(root_module, root_module['ns3::Ipv4AddressTlvValue::ipv4Addr'])
    register_Ns3MacHeaderType_methods(root_module, root_module['ns3::MacHeaderType'])
    register_Ns3ManagementMessageType_methods(root_module, root_module['ns3::ManagementMessageType'])
    register_Ns3OfdmDownlinkFramePrefix_methods(root_module, root_module['ns3::OfdmDownlinkFramePrefix'])
    register_Ns3OfdmSendParams_methods(root_module, root_module['ns3::OfdmSendParams'])
    register_Ns3OfdmUcdChannelEncodings_methods(root_module, root_module['ns3::OfdmUcdChannelEncodings'])
    register_Ns3PortRangeTlvValue_methods(root_module, root_module['ns3::PortRangeTlvValue'])
    register_Ns3PortRangeTlvValuePortRange_methods(root_module, root_module['ns3::PortRangeTlvValue::PortRange'])
    register_Ns3PriorityUlJob_methods(root_module, root_module['ns3::PriorityUlJob'])
    register_Ns3ProtocolTlvValue_methods(root_module, root_module['ns3::ProtocolTlvValue'])
    register_Ns3RngReq_methods(root_module, root_module['ns3::RngReq'])
    register_Ns3RngRsp_methods(root_module, root_module['ns3::RngRsp'])
    register_Ns3SSManager_methods(root_module, root_module['ns3::SSManager'])
    register_Ns3ServiceFlowManager_methods(root_module, root_module['ns3::ServiceFlowManager'])
    register_Ns3SfVectorTlvValue_methods(root_module, root_module['ns3::SfVectorTlvValue'])
    register_Ns3SsServiceFlowManager_methods(root_module, root_module['ns3::SsServiceFlowManager'])
    register_Ns3Tlv_methods(root_module, root_module['ns3::Tlv'])
    register_Ns3Ucd_methods(root_module, root_module['ns3::Ucd'])
    register_Ns3UlJob_methods(root_module, root_module['ns3::UlJob'])
    register_Ns3UlMap_methods(root_module, root_module['ns3::UlMap'])
    register_Ns3UplinkScheduler_methods(root_module, root_module['ns3::UplinkScheduler'])
    register_Ns3UplinkSchedulerMBQoS_methods(root_module, root_module['ns3::UplinkSchedulerMBQoS'])
    register_Ns3UplinkSchedulerRtps_methods(root_module, root_module['ns3::UplinkSchedulerRtps'])
    register_Ns3UplinkSchedulerSimple_methods(root_module, root_module['ns3::UplinkSchedulerSimple'])
    register_Ns3WimaxConnection_methods(root_module, root_module['ns3::WimaxConnection'])
    register_Ns3WimaxMacQueue_methods(root_module, root_module['ns3::WimaxMacQueue'])
    register_Ns3WimaxMacToMacHeader_methods(root_module, root_module['ns3::WimaxMacToMacHeader'])
    register_Ns3WimaxPhy_methods(root_module, root_module['ns3::WimaxPhy'])
    register_Ns3BSScheduler_methods(root_module, root_module['ns3::BSScheduler'])
    register_Ns3BSSchedulerRtps_methods(root_module, root_module['ns3::BSSchedulerRtps'])
    register_Ns3BSSchedulerSimple_methods(root_module, root_module['ns3::BSSchedulerSimple'])
    register_Ns3BandwidthRequestHeader_methods(root_module, root_module['ns3::BandwidthRequestHeader'])
    register_Ns3BsServiceFlowManager_methods(root_module, root_module['ns3::BsServiceFlowManager'])
    register_Ns3ConnectionManager_methods(root_module, root_module['ns3::ConnectionManager'])
    register_Ns3Dcd_methods(root_module, root_module['ns3::Dcd'])
    register_Ns3DlMap_methods(root_module, root_module['ns3::DlMap'])
    register_Ns3DsaAck_methods(root_module, root_module['ns3::DsaAck'])
    register_Ns3DsaReq_methods(root_module, root_module['ns3::DsaReq'])
    register_Ns3DsaRsp_methods(root_module, root_module['ns3::DsaRsp'])
    register_Ns3FragmentationSubheader_methods(root_module, root_module['ns3::FragmentationSubheader'])
    register_Ns3GenericMacHeader_methods(root_module, root_module['ns3::GenericMacHeader'])
    register_Ns3GrantManagementSubheader_methods(root_module, root_module['ns3::GrantManagementSubheader'])
    register_Ns3IpcsClassifier_methods(root_module, root_module['ns3::IpcsClassifier'])
    register_Ns3SimpleOfdmWimaxPhy_methods(root_module, root_module['ns3::SimpleOfdmWimaxPhy'])
    register_Ns3WimaxChannel_methods(root_module, root_module['ns3::WimaxChannel'])
    register_Ns3WimaxNetDevice_methods(root_module, root_module['ns3::WimaxNetDevice'])
    register_Ns3BaseStationNetDevice_methods(root_module, root_module['ns3::BaseStationNetDevice'])
    register_Ns3SimpleOfdmWimaxChannel_methods(root_module, root_module['ns3::SimpleOfdmWimaxChannel'])
    register_Ns3SubscriberStationNetDevice_methods(root_module, root_module['ns3::SubscriberStationNetDevice'])
    return

def register_Ns3Cid_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## cid.h: ns3::Cid::Cid(ns3::Cid const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Cid const &', 'arg0')])
    ## cid.h: ns3::Cid::Cid() [constructor]
    cls.add_constructor([])
    ## cid.h: ns3::Cid::Cid(uint16_t cid) [constructor]
    cls.add_constructor([param('uint16_t', 'cid')])
    ## cid.h: static ns3::Cid ns3::Cid::Broadcast() [member function]
    cls.add_method('Broadcast', 
                   'ns3::Cid', 
                   [], 
                   is_static=True)
    ## cid.h: uint16_t ns3::Cid::GetIdentifier() const [member function]
    cls.add_method('GetIdentifier', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## cid.h: static ns3::Cid ns3::Cid::InitialRanging() [member function]
    cls.add_method('InitialRanging', 
                   'ns3::Cid', 
                   [], 
                   is_static=True)
    ## cid.h: bool ns3::Cid::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## cid.h: bool ns3::Cid::IsInitialRanging() const [member function]
    cls.add_method('IsInitialRanging', 
                   'bool', 
                   [], 
                   is_const=True)
    ## cid.h: bool ns3::Cid::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## cid.h: bool ns3::Cid::IsPadding() const [member function]
    cls.add_method('IsPadding', 
                   'bool', 
                   [], 
                   is_const=True)
    ## cid.h: static ns3::Cid ns3::Cid::Padding() [member function]
    cls.add_method('Padding', 
                   'ns3::Cid', 
                   [], 
                   is_static=True)
    return

def register_Ns3CidFactory_methods(root_module, cls):
    ## cid-factory.h: ns3::CidFactory::CidFactory(ns3::CidFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CidFactory const &', 'arg0')])
    ## cid-factory.h: ns3::CidFactory::CidFactory() [constructor]
    cls.add_constructor([])
    ## cid-factory.h: ns3::Cid ns3::CidFactory::Allocate(ns3::Cid::Type type) [member function]
    cls.add_method('Allocate', 
                   'ns3::Cid', 
                   [param('ns3::Cid::Type', 'type')])
    ## cid-factory.h: ns3::Cid ns3::CidFactory::AllocateBasic() [member function]
    cls.add_method('AllocateBasic', 
                   'ns3::Cid', 
                   [])
    ## cid-factory.h: ns3::Cid ns3::CidFactory::AllocateMulticast() [member function]
    cls.add_method('AllocateMulticast', 
                   'ns3::Cid', 
                   [])
    ## cid-factory.h: ns3::Cid ns3::CidFactory::AllocatePrimary() [member function]
    cls.add_method('AllocatePrimary', 
                   'ns3::Cid', 
                   [])
    ## cid-factory.h: ns3::Cid ns3::CidFactory::AllocateTransportOrSecondary() [member function]
    cls.add_method('AllocateTransportOrSecondary', 
                   'ns3::Cid', 
                   [])
    ## cid-factory.h: void ns3::CidFactory::FreeCid(ns3::Cid cid) [member function]
    cls.add_method('FreeCid', 
                   'void', 
                   [param('ns3::Cid', 'cid')])
    ## cid-factory.h: bool ns3::CidFactory::IsBasic(ns3::Cid cid) const [member function]
    cls.add_method('IsBasic', 
                   'bool', 
                   [param('ns3::Cid', 'cid')], 
                   is_const=True)
    ## cid-factory.h: bool ns3::CidFactory::IsPrimary(ns3::Cid cid) const [member function]
    cls.add_method('IsPrimary', 
                   'bool', 
                   [param('ns3::Cid', 'cid')], 
                   is_const=True)
    ## cid-factory.h: bool ns3::CidFactory::IsTransport(ns3::Cid cid) const [member function]
    cls.add_method('IsTransport', 
                   'bool', 
                   [param('ns3::Cid', 'cid')], 
                   is_const=True)
    return

def register_Ns3CsParameters_methods(root_module, cls):
    ## cs-parameters.h: ns3::CsParameters::CsParameters(ns3::CsParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsParameters const &', 'arg0')])
    ## cs-parameters.h: ns3::CsParameters::CsParameters() [constructor]
    cls.add_constructor([])
    ## cs-parameters.h: ns3::CsParameters::CsParameters(ns3::Tlv tlv) [constructor]
    cls.add_constructor([param('ns3::Tlv', 'tlv')])
    ## cs-parameters.h: ns3::CsParameters::CsParameters(ns3::CsParameters::Action classifierDscAction, ns3::IpcsClassifierRecord classifier) [constructor]
    cls.add_constructor([param('ns3::CsParameters::Action', 'classifierDscAction'), param('ns3::IpcsClassifierRecord', 'classifier')])
    ## cs-parameters.h: ns3::CsParameters::Action ns3::CsParameters::GetClassifierDscAction() const [member function]
    cls.add_method('GetClassifierDscAction', 
                   'ns3::CsParameters::Action', 
                   [], 
                   is_const=True)
    ## cs-parameters.h: ns3::IpcsClassifierRecord ns3::CsParameters::GetPacketClassifierRule() const [member function]
    cls.add_method('GetPacketClassifierRule', 
                   'ns3::IpcsClassifierRecord', 
                   [], 
                   is_const=True)
    ## cs-parameters.h: void ns3::CsParameters::SetClassifierDscAction(ns3::CsParameters::Action action) [member function]
    cls.add_method('SetClassifierDscAction', 
                   'void', 
                   [param('ns3::CsParameters::Action', 'action')])
    ## cs-parameters.h: void ns3::CsParameters::SetPacketClassifierRule(ns3::IpcsClassifierRecord packetClassifierRule) [member function]
    cls.add_method('SetPacketClassifierRule', 
                   'void', 
                   [param('ns3::IpcsClassifierRecord', 'packetClassifierRule')])
    ## cs-parameters.h: ns3::Tlv ns3::CsParameters::ToTlv() const [member function]
    cls.add_method('ToTlv', 
                   'ns3::Tlv', 
                   [], 
                   is_const=True)
    return

def register_Ns3DcdChannelEncodings_methods(root_module, cls):
    ## dl-mac-messages.h: ns3::DcdChannelEncodings::DcdChannelEncodings(ns3::DcdChannelEncodings const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DcdChannelEncodings const &', 'arg0')])
    ## dl-mac-messages.h: ns3::DcdChannelEncodings::DcdChannelEncodings() [constructor]
    cls.add_constructor([])
    ## dl-mac-messages.h: uint16_t ns3::DcdChannelEncodings::GetBsEirp() const [member function]
    cls.add_method('GetBsEirp', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint16_t ns3::DcdChannelEncodings::GetEirxPIrMax() const [member function]
    cls.add_method('GetEirxPIrMax', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint32_t ns3::DcdChannelEncodings::GetFrequency() const [member function]
    cls.add_method('GetFrequency', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint16_t ns3::DcdChannelEncodings::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::DcdChannelEncodings::Read(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Read', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## dl-mac-messages.h: void ns3::DcdChannelEncodings::SetBsEirp(uint16_t bs_eirp) [member function]
    cls.add_method('SetBsEirp', 
                   'void', 
                   [param('uint16_t', 'bs_eirp')])
    ## dl-mac-messages.h: void ns3::DcdChannelEncodings::SetEirxPIrMax(uint16_t rss_ir_max) [member function]
    cls.add_method('SetEirxPIrMax', 
                   'void', 
                   [param('uint16_t', 'rss_ir_max')])
    ## dl-mac-messages.h: void ns3::DcdChannelEncodings::SetFrequency(uint32_t frequency) [member function]
    cls.add_method('SetFrequency', 
                   'void', 
                   [param('uint32_t', 'frequency')])
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::DcdChannelEncodings::Write(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Write', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::DcdChannelEncodings::DoRead(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DoRead', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::DcdChannelEncodings::DoWrite(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('DoWrite', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3DlFramePrefixIe_methods(root_module, cls):
    ## ofdm-downlink-frame-prefix.h: ns3::DlFramePrefixIe::DlFramePrefixIe(ns3::DlFramePrefixIe const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DlFramePrefixIe const &', 'arg0')])
    ## ofdm-downlink-frame-prefix.h: ns3::DlFramePrefixIe::DlFramePrefixIe() [constructor]
    cls.add_constructor([])
    ## ofdm-downlink-frame-prefix.h: uint8_t ns3::DlFramePrefixIe::GetDiuc() const [member function]
    cls.add_method('GetDiuc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint16_t ns3::DlFramePrefixIe::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint8_t ns3::DlFramePrefixIe::GetPreamblePresent() const [member function]
    cls.add_method('GetPreamblePresent', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint8_t ns3::DlFramePrefixIe::GetRateId() const [member function]
    cls.add_method('GetRateId', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint16_t ns3::DlFramePrefixIe::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint16_t ns3::DlFramePrefixIe::GetStartTime() const [member function]
    cls.add_method('GetStartTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: ns3::Buffer::Iterator ns3::DlFramePrefixIe::Read(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Read', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## ofdm-downlink-frame-prefix.h: void ns3::DlFramePrefixIe::SetDiuc(uint8_t diuc) [member function]
    cls.add_method('SetDiuc', 
                   'void', 
                   [param('uint8_t', 'diuc')])
    ## ofdm-downlink-frame-prefix.h: void ns3::DlFramePrefixIe::SetLength(uint16_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint16_t', 'length')])
    ## ofdm-downlink-frame-prefix.h: void ns3::DlFramePrefixIe::SetPreamblePresent(uint8_t preamblePresent) [member function]
    cls.add_method('SetPreamblePresent', 
                   'void', 
                   [param('uint8_t', 'preamblePresent')])
    ## ofdm-downlink-frame-prefix.h: void ns3::DlFramePrefixIe::SetRateId(uint8_t rateId) [member function]
    cls.add_method('SetRateId', 
                   'void', 
                   [param('uint8_t', 'rateId')])
    ## ofdm-downlink-frame-prefix.h: void ns3::DlFramePrefixIe::SetStartTime(uint16_t startTime) [member function]
    cls.add_method('SetStartTime', 
                   'void', 
                   [param('uint16_t', 'startTime')])
    ## ofdm-downlink-frame-prefix.h: ns3::Buffer::Iterator ns3::DlFramePrefixIe::Write(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Write', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    return

def register_Ns3IpcsClassifierRecord_methods(root_module, cls):
    ## ipcs-classifier-record.h: ns3::IpcsClassifierRecord::IpcsClassifierRecord(ns3::IpcsClassifierRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IpcsClassifierRecord const &', 'arg0')])
    ## ipcs-classifier-record.h: ns3::IpcsClassifierRecord::IpcsClassifierRecord() [constructor]
    cls.add_constructor([])
    ## ipcs-classifier-record.h: ns3::IpcsClassifierRecord::IpcsClassifierRecord(ns3::Ipv4Address srcAddress, ns3::Ipv4Mask srcMask, ns3::Ipv4Address dstAddress, ns3::Ipv4Mask dstMask, uint16_t srcPortLow, uint16_t srcPortHigh, uint16_t dstPortLow, uint16_t dstPortHigh, uint8_t protocol, uint8_t priority) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'srcAddress'), param('ns3::Ipv4Mask', 'srcMask'), param('ns3::Ipv4Address', 'dstAddress'), param('ns3::Ipv4Mask', 'dstMask'), param('uint16_t', 'srcPortLow'), param('uint16_t', 'srcPortHigh'), param('uint16_t', 'dstPortLow'), param('uint16_t', 'dstPortHigh'), param('uint8_t', 'protocol'), param('uint8_t', 'priority')])
    ## ipcs-classifier-record.h: ns3::IpcsClassifierRecord::IpcsClassifierRecord(ns3::Tlv tlv) [constructor]
    cls.add_constructor([param('ns3::Tlv', 'tlv')])
    ## ipcs-classifier-record.h: void ns3::IpcsClassifierRecord::AddDstAddr(ns3::Ipv4Address dstAddress, ns3::Ipv4Mask dstMask) [member function]
    cls.add_method('AddDstAddr', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dstAddress'), param('ns3::Ipv4Mask', 'dstMask')])
    ## ipcs-classifier-record.h: void ns3::IpcsClassifierRecord::AddDstPortRange(uint16_t dstPortLow, uint16_t dstPortHigh) [member function]
    cls.add_method('AddDstPortRange', 
                   'void', 
                   [param('uint16_t', 'dstPortLow'), param('uint16_t', 'dstPortHigh')])
    ## ipcs-classifier-record.h: void ns3::IpcsClassifierRecord::AddProtocol(uint8_t proto) [member function]
    cls.add_method('AddProtocol', 
                   'void', 
                   [param('uint8_t', 'proto')])
    ## ipcs-classifier-record.h: void ns3::IpcsClassifierRecord::AddSrcAddr(ns3::Ipv4Address srcAddress, ns3::Ipv4Mask srcMask) [member function]
    cls.add_method('AddSrcAddr', 
                   'void', 
                   [param('ns3::Ipv4Address', 'srcAddress'), param('ns3::Ipv4Mask', 'srcMask')])
    ## ipcs-classifier-record.h: void ns3::IpcsClassifierRecord::AddSrcPortRange(uint16_t srcPortLow, uint16_t srcPortHigh) [member function]
    cls.add_method('AddSrcPortRange', 
                   'void', 
                   [param('uint16_t', 'srcPortLow'), param('uint16_t', 'srcPortHigh')])
    ## ipcs-classifier-record.h: bool ns3::IpcsClassifierRecord::CheckMatch(ns3::Ipv4Address srcAddress, ns3::Ipv4Address dstAddress, uint16_t srcPort, uint16_t dstPort, uint8_t proto) const [member function]
    cls.add_method('CheckMatch', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'srcAddress'), param('ns3::Ipv4Address', 'dstAddress'), param('uint16_t', 'srcPort'), param('uint16_t', 'dstPort'), param('uint8_t', 'proto')], 
                   is_const=True)
    ## ipcs-classifier-record.h: uint16_t ns3::IpcsClassifierRecord::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipcs-classifier-record.h: uint16_t ns3::IpcsClassifierRecord::GetIndex() const [member function]
    cls.add_method('GetIndex', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipcs-classifier-record.h: uint8_t ns3::IpcsClassifierRecord::GetPriority() const [member function]
    cls.add_method('GetPriority', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipcs-classifier-record.h: void ns3::IpcsClassifierRecord::SetCid(uint16_t cid) [member function]
    cls.add_method('SetCid', 
                   'void', 
                   [param('uint16_t', 'cid')])
    ## ipcs-classifier-record.h: void ns3::IpcsClassifierRecord::SetIndex(uint16_t index) [member function]
    cls.add_method('SetIndex', 
                   'void', 
                   [param('uint16_t', 'index')])
    ## ipcs-classifier-record.h: void ns3::IpcsClassifierRecord::SetPriority(uint8_t prio) [member function]
    cls.add_method('SetPriority', 
                   'void', 
                   [param('uint8_t', 'prio')])
    ## ipcs-classifier-record.h: ns3::Tlv ns3::IpcsClassifierRecord::ToTlv() const [member function]
    cls.add_method('ToTlv', 
                   'ns3::Tlv', 
                   [], 
                   is_const=True)
    return

def register_Ns3OfdmDcdChannelEncodings_methods(root_module, cls):
    ## dl-mac-messages.h: ns3::OfdmDcdChannelEncodings::OfdmDcdChannelEncodings(ns3::OfdmDcdChannelEncodings const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OfdmDcdChannelEncodings const &', 'arg0')])
    ## dl-mac-messages.h: ns3::OfdmDcdChannelEncodings::OfdmDcdChannelEncodings() [constructor]
    cls.add_constructor([])
    ## dl-mac-messages.h: ns3::Mac48Address ns3::OfdmDcdChannelEncodings::GetBaseStationId() const [member function]
    cls.add_method('GetBaseStationId', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDcdChannelEncodings::GetChannelNr() const [member function]
    cls.add_method('GetChannelNr', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDcdChannelEncodings::GetFrameDurationCode() const [member function]
    cls.add_method('GetFrameDurationCode', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint32_t ns3::OfdmDcdChannelEncodings::GetFrameNumber() const [member function]
    cls.add_method('GetFrameNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDcdChannelEncodings::GetRtg() const [member function]
    cls.add_method('GetRtg', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint16_t ns3::OfdmDcdChannelEncodings::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDcdChannelEncodings::GetTtg() const [member function]
    cls.add_method('GetTtg', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: void ns3::OfdmDcdChannelEncodings::SetBaseStationId(ns3::Mac48Address baseStationId) [member function]
    cls.add_method('SetBaseStationId', 
                   'void', 
                   [param('ns3::Mac48Address', 'baseStationId')])
    ## dl-mac-messages.h: void ns3::OfdmDcdChannelEncodings::SetChannelNr(uint8_t channelNr) [member function]
    cls.add_method('SetChannelNr', 
                   'void', 
                   [param('uint8_t', 'channelNr')])
    ## dl-mac-messages.h: void ns3::OfdmDcdChannelEncodings::SetFrameDurationCode(uint8_t frameDurationCode) [member function]
    cls.add_method('SetFrameDurationCode', 
                   'void', 
                   [param('uint8_t', 'frameDurationCode')])
    ## dl-mac-messages.h: void ns3::OfdmDcdChannelEncodings::SetFrameNumber(uint32_t frameNumber) [member function]
    cls.add_method('SetFrameNumber', 
                   'void', 
                   [param('uint32_t', 'frameNumber')])
    ## dl-mac-messages.h: void ns3::OfdmDcdChannelEncodings::SetRtg(uint8_t rtg) [member function]
    cls.add_method('SetRtg', 
                   'void', 
                   [param('uint8_t', 'rtg')])
    ## dl-mac-messages.h: void ns3::OfdmDcdChannelEncodings::SetTtg(uint8_t ttg) [member function]
    cls.add_method('SetTtg', 
                   'void', 
                   [param('uint8_t', 'ttg')])
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmDcdChannelEncodings::DoRead(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DoRead', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   visibility='private', is_virtual=True)
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmDcdChannelEncodings::DoWrite(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('DoWrite', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3OfdmDlBurstProfile_methods(root_module, cls):
    ## dl-mac-messages.h: ns3::OfdmDlBurstProfile::OfdmDlBurstProfile(ns3::OfdmDlBurstProfile const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OfdmDlBurstProfile const &', 'arg0')])
    ## dl-mac-messages.h: ns3::OfdmDlBurstProfile::OfdmDlBurstProfile() [constructor]
    cls.add_constructor([])
    ## dl-mac-messages.h: uint8_t ns3::OfdmDlBurstProfile::GetDiuc() const [member function]
    cls.add_method('GetDiuc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDlBurstProfile::GetFecCodeType() const [member function]
    cls.add_method('GetFecCodeType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDlBurstProfile::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint16_t ns3::OfdmDlBurstProfile::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDlBurstProfile::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmDlBurstProfile::Read(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Read', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## dl-mac-messages.h: void ns3::OfdmDlBurstProfile::SetDiuc(uint8_t diuc) [member function]
    cls.add_method('SetDiuc', 
                   'void', 
                   [param('uint8_t', 'diuc')])
    ## dl-mac-messages.h: void ns3::OfdmDlBurstProfile::SetFecCodeType(uint8_t fecCodeType) [member function]
    cls.add_method('SetFecCodeType', 
                   'void', 
                   [param('uint8_t', 'fecCodeType')])
    ## dl-mac-messages.h: void ns3::OfdmDlBurstProfile::SetLength(uint8_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint8_t', 'length')])
    ## dl-mac-messages.h: void ns3::OfdmDlBurstProfile::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmDlBurstProfile::Write(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Write', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    return

def register_Ns3OfdmDlMapIe_methods(root_module, cls):
    ## dl-mac-messages.h: ns3::OfdmDlMapIe::OfdmDlMapIe(ns3::OfdmDlMapIe const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OfdmDlMapIe const &', 'arg0')])
    ## dl-mac-messages.h: ns3::OfdmDlMapIe::OfdmDlMapIe() [constructor]
    cls.add_constructor([])
    ## dl-mac-messages.h: ns3::Cid ns3::OfdmDlMapIe::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDlMapIe::GetDiuc() const [member function]
    cls.add_method('GetDiuc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::OfdmDlMapIe::GetPreamblePresent() const [member function]
    cls.add_method('GetPreamblePresent', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint16_t ns3::OfdmDlMapIe::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint16_t ns3::OfdmDlMapIe::GetStartTime() const [member function]
    cls.add_method('GetStartTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmDlMapIe::Read(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Read', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## dl-mac-messages.h: void ns3::OfdmDlMapIe::SetCid(ns3::Cid cid) [member function]
    cls.add_method('SetCid', 
                   'void', 
                   [param('ns3::Cid', 'cid')])
    ## dl-mac-messages.h: void ns3::OfdmDlMapIe::SetDiuc(uint8_t diuc) [member function]
    cls.add_method('SetDiuc', 
                   'void', 
                   [param('uint8_t', 'diuc')])
    ## dl-mac-messages.h: void ns3::OfdmDlMapIe::SetPreamblePresent(uint8_t preamblePresent) [member function]
    cls.add_method('SetPreamblePresent', 
                   'void', 
                   [param('uint8_t', 'preamblePresent')])
    ## dl-mac-messages.h: void ns3::OfdmDlMapIe::SetStartTime(uint16_t startTime) [member function]
    cls.add_method('SetStartTime', 
                   'void', 
                   [param('uint16_t', 'startTime')])
    ## dl-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmDlMapIe::Write(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Write', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    return

def register_Ns3OfdmUlBurstProfile_methods(root_module, cls):
    ## ul-mac-messages.h: ns3::OfdmUlBurstProfile::OfdmUlBurstProfile(ns3::OfdmUlBurstProfile const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OfdmUlBurstProfile const &', 'arg0')])
    ## ul-mac-messages.h: ns3::OfdmUlBurstProfile::OfdmUlBurstProfile() [constructor]
    cls.add_constructor([])
    ## ul-mac-messages.h: uint8_t ns3::OfdmUlBurstProfile::GetFecCodeType() const [member function]
    cls.add_method('GetFecCodeType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::OfdmUlBurstProfile::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint16_t ns3::OfdmUlBurstProfile::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::OfdmUlBurstProfile::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::OfdmUlBurstProfile::GetUiuc() const [member function]
    cls.add_method('GetUiuc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmUlBurstProfile::Read(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Read', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## ul-mac-messages.h: void ns3::OfdmUlBurstProfile::SetFecCodeType(uint8_t fecCodeType) [member function]
    cls.add_method('SetFecCodeType', 
                   'void', 
                   [param('uint8_t', 'fecCodeType')])
    ## ul-mac-messages.h: void ns3::OfdmUlBurstProfile::SetLength(uint8_t length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint8_t', 'length')])
    ## ul-mac-messages.h: void ns3::OfdmUlBurstProfile::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## ul-mac-messages.h: void ns3::OfdmUlBurstProfile::SetUiuc(uint8_t uiuc) [member function]
    cls.add_method('SetUiuc', 
                   'void', 
                   [param('uint8_t', 'uiuc')])
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmUlBurstProfile::Write(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Write', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    return

def register_Ns3OfdmUlMapIe_methods(root_module, cls):
    ## ul-mac-messages.h: ns3::OfdmUlMapIe::OfdmUlMapIe(ns3::OfdmUlMapIe const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OfdmUlMapIe const &', 'arg0')])
    ## ul-mac-messages.h: ns3::OfdmUlMapIe::OfdmUlMapIe() [constructor]
    cls.add_constructor([])
    ## ul-mac-messages.h: ns3::Cid ns3::OfdmUlMapIe::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint16_t ns3::OfdmUlMapIe::GetDuration() const [member function]
    cls.add_method('GetDuration', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::OfdmUlMapIe::GetMidambleRepetitionInterval() const [member function]
    cls.add_method('GetMidambleRepetitionInterval', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint16_t ns3::OfdmUlMapIe::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint16_t ns3::OfdmUlMapIe::GetStartTime() const [member function]
    cls.add_method('GetStartTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::OfdmUlMapIe::GetSubchannelIndex() const [member function]
    cls.add_method('GetSubchannelIndex', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::OfdmUlMapIe::GetUiuc() const [member function]
    cls.add_method('GetUiuc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmUlMapIe::Read(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Read', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## ul-mac-messages.h: void ns3::OfdmUlMapIe::SetCid(ns3::Cid cid) [member function]
    cls.add_method('SetCid', 
                   'void', 
                   [param('ns3::Cid', 'cid')])
    ## ul-mac-messages.h: void ns3::OfdmUlMapIe::SetDuration(uint16_t duration) [member function]
    cls.add_method('SetDuration', 
                   'void', 
                   [param('uint16_t', 'duration')])
    ## ul-mac-messages.h: void ns3::OfdmUlMapIe::SetMidambleRepetitionInterval(uint8_t midambleRepetitionInterval) [member function]
    cls.add_method('SetMidambleRepetitionInterval', 
                   'void', 
                   [param('uint8_t', 'midambleRepetitionInterval')])
    ## ul-mac-messages.h: void ns3::OfdmUlMapIe::SetStartTime(uint16_t startTime) [member function]
    cls.add_method('SetStartTime', 
                   'void', 
                   [param('uint16_t', 'startTime')])
    ## ul-mac-messages.h: void ns3::OfdmUlMapIe::SetSubchannelIndex(uint8_t subchannelIndex) [member function]
    cls.add_method('SetSubchannelIndex', 
                   'void', 
                   [param('uint8_t', 'subchannelIndex')])
    ## ul-mac-messages.h: void ns3::OfdmUlMapIe::SetUiuc(uint8_t uiuc) [member function]
    cls.add_method('SetUiuc', 
                   'void', 
                   [param('uint8_t', 'uiuc')])
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmUlMapIe::Write(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Write', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    return

def register_Ns3SNRToBlockErrorRateManager_methods(root_module, cls):
    ## snr-to-block-error-rate-manager.h: ns3::SNRToBlockErrorRateManager::SNRToBlockErrorRateManager(ns3::SNRToBlockErrorRateManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SNRToBlockErrorRateManager const &', 'arg0')])
    ## snr-to-block-error-rate-manager.h: ns3::SNRToBlockErrorRateManager::SNRToBlockErrorRateManager() [constructor]
    cls.add_constructor([])
    ## snr-to-block-error-rate-manager.h: void ns3::SNRToBlockErrorRateManager::ActivateLoss(bool loss) [member function]
    cls.add_method('ActivateLoss', 
                   'void', 
                   [param('bool', 'loss')])
    ## snr-to-block-error-rate-manager.h: double ns3::SNRToBlockErrorRateManager::GetBlockErrorRate(double SNR, uint8_t modulation) [member function]
    cls.add_method('GetBlockErrorRate', 
                   'double', 
                   [param('double', 'SNR'), param('uint8_t', 'modulation')])
    ## snr-to-block-error-rate-manager.h: ns3::SNRToBlockErrorRateRecord * ns3::SNRToBlockErrorRateManager::GetSNRToBlockErrorRateRecord(double SNR, uint8_t modulation) [member function]
    cls.add_method('GetSNRToBlockErrorRateRecord', 
                   'ns3::SNRToBlockErrorRateRecord *', 
                   [param('double', 'SNR'), param('uint8_t', 'modulation')])
    ## snr-to-block-error-rate-manager.h: std::string ns3::SNRToBlockErrorRateManager::GetTraceFilePath() [member function]
    cls.add_method('GetTraceFilePath', 
                   'std::string', 
                   [])
    ## snr-to-block-error-rate-manager.h: void ns3::SNRToBlockErrorRateManager::LoadDefaultTraces() [member function]
    cls.add_method('LoadDefaultTraces', 
                   'void', 
                   [])
    ## snr-to-block-error-rate-manager.h: void ns3::SNRToBlockErrorRateManager::LoadTraces() [member function]
    cls.add_method('LoadTraces', 
                   'void', 
                   [])
    ## snr-to-block-error-rate-manager.h: void ns3::SNRToBlockErrorRateManager::ReLoadTraces() [member function]
    cls.add_method('ReLoadTraces', 
                   'void', 
                   [])
    ## snr-to-block-error-rate-manager.h: void ns3::SNRToBlockErrorRateManager::SetTraceFilePath(char * traceFilePath) [member function]
    cls.add_method('SetTraceFilePath', 
                   'void', 
                   [param('char *', 'traceFilePath')])
    return

def register_Ns3SNRToBlockErrorRateRecord_methods(root_module, cls):
    ## snr-to-block-error-rate-record.h: ns3::SNRToBlockErrorRateRecord::SNRToBlockErrorRateRecord(ns3::SNRToBlockErrorRateRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SNRToBlockErrorRateRecord const &', 'arg0')])
    ## snr-to-block-error-rate-record.h: ns3::SNRToBlockErrorRateRecord::SNRToBlockErrorRateRecord(double snrValue, double bitErrorRate, double BlockErrorRate, double sigma2, double I1, double I2) [constructor]
    cls.add_constructor([param('double', 'snrValue'), param('double', 'bitErrorRate'), param('double', 'BlockErrorRate'), param('double', 'sigma2'), param('double', 'I1'), param('double', 'I2')])
    ## snr-to-block-error-rate-record.h: ns3::SNRToBlockErrorRateRecord * ns3::SNRToBlockErrorRateRecord::Copy() [member function]
    cls.add_method('Copy', 
                   'ns3::SNRToBlockErrorRateRecord *', 
                   [])
    ## snr-to-block-error-rate-record.h: double ns3::SNRToBlockErrorRateRecord::GetBitErrorRate() [member function]
    cls.add_method('GetBitErrorRate', 
                   'double', 
                   [])
    ## snr-to-block-error-rate-record.h: double ns3::SNRToBlockErrorRateRecord::GetBlockErrorRate() [member function]
    cls.add_method('GetBlockErrorRate', 
                   'double', 
                   [])
    ## snr-to-block-error-rate-record.h: double ns3::SNRToBlockErrorRateRecord::GetI1() [member function]
    cls.add_method('GetI1', 
                   'double', 
                   [])
    ## snr-to-block-error-rate-record.h: double ns3::SNRToBlockErrorRateRecord::GetI2() [member function]
    cls.add_method('GetI2', 
                   'double', 
                   [])
    ## snr-to-block-error-rate-record.h: double ns3::SNRToBlockErrorRateRecord::GetSNRValue() [member function]
    cls.add_method('GetSNRValue', 
                   'double', 
                   [])
    ## snr-to-block-error-rate-record.h: double ns3::SNRToBlockErrorRateRecord::GetSigma2() [member function]
    cls.add_method('GetSigma2', 
                   'double', 
                   [])
    ## snr-to-block-error-rate-record.h: void ns3::SNRToBlockErrorRateRecord::SetBitErrorRate(double arg0) [member function]
    cls.add_method('SetBitErrorRate', 
                   'void', 
                   [param('double', 'arg0')])
    ## snr-to-block-error-rate-record.h: void ns3::SNRToBlockErrorRateRecord::SetBlockErrorRate(double arg0) [member function]
    cls.add_method('SetBlockErrorRate', 
                   'void', 
                   [param('double', 'arg0')])
    ## snr-to-block-error-rate-record.h: void ns3::SNRToBlockErrorRateRecord::SetI1(double arg0) [member function]
    cls.add_method('SetI1', 
                   'void', 
                   [param('double', 'arg0')])
    ## snr-to-block-error-rate-record.h: void ns3::SNRToBlockErrorRateRecord::SetI2(double arg0) [member function]
    cls.add_method('SetI2', 
                   'void', 
                   [param('double', 'arg0')])
    ## snr-to-block-error-rate-record.h: void ns3::SNRToBlockErrorRateRecord::SetSNRValue(double arg0) [member function]
    cls.add_method('SetSNRValue', 
                   'void', 
                   [param('double', 'arg0')])
    return

def register_Ns3SSRecord_methods(root_module, cls):
    ## ss-record.h: ns3::SSRecord::SSRecord(ns3::SSRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SSRecord const &', 'arg0')])
    ## ss-record.h: ns3::SSRecord::SSRecord() [constructor]
    cls.add_constructor([])
    ## ss-record.h: ns3::SSRecord::SSRecord(ns3::Mac48Address macAddress) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'macAddress')])
    ## ss-record.h: ns3::SSRecord::SSRecord(ns3::Mac48Address macAddress, ns3::Ipv4Address IPaddress) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'macAddress'), param('ns3::Ipv4Address', 'IPaddress')])
    ## ss-record.h: void ns3::SSRecord::AddServiceFlow(ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('AddServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow *', 'serviceFlow')])
    ## ss-record.h: void ns3::SSRecord::DisablePollForRanging() [member function]
    cls.add_method('DisablePollForRanging', 
                   'void', 
                   [])
    ## ss-record.h: void ns3::SSRecord::EnablePollForRanging() [member function]
    cls.add_method('EnablePollForRanging', 
                   'void', 
                   [])
    ## ss-record.h: bool ns3::SSRecord::GetAreServiceFlowsAllocated() const [member function]
    cls.add_method('GetAreServiceFlowsAllocated', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-record.h: ns3::Cid ns3::SSRecord::GetBasicCid() const [member function]
    cls.add_method('GetBasicCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## ss-record.h: ns3::DsaRsp ns3::SSRecord::GetDsaRsp() const [member function]
    cls.add_method('GetDsaRsp', 
                   'ns3::DsaRsp', 
                   [], 
                   is_const=True)
    ## ss-record.h: uint8_t ns3::SSRecord::GetDsaRspRetries() const [member function]
    cls.add_method('GetDsaRspRetries', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ss-record.h: bool ns3::SSRecord::GetHasServiceFlowBe() const [member function]
    cls.add_method('GetHasServiceFlowBe', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-record.h: bool ns3::SSRecord::GetHasServiceFlowNrtps() const [member function]
    cls.add_method('GetHasServiceFlowNrtps', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-record.h: bool ns3::SSRecord::GetHasServiceFlowRtps() const [member function]
    cls.add_method('GetHasServiceFlowRtps', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-record.h: bool ns3::SSRecord::GetHasServiceFlowUgs() const [member function]
    cls.add_method('GetHasServiceFlowUgs', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-record.h: ns3::Ipv4Address ns3::SSRecord::GetIPAddress() [member function]
    cls.add_method('GetIPAddress', 
                   'ns3::Ipv4Address', 
                   [])
    ## ss-record.h: uint8_t ns3::SSRecord::GetInvitedRangRetries() const [member function]
    cls.add_method('GetInvitedRangRetries', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ss-record.h: bool ns3::SSRecord::GetIsBroadcastSS() [member function]
    cls.add_method('GetIsBroadcastSS', 
                   'bool', 
                   [])
    ## ss-record.h: ns3::Mac48Address ns3::SSRecord::GetMacAddress() const [member function]
    cls.add_method('GetMacAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ss-record.h: ns3::WimaxPhy::ModulationType ns3::SSRecord::GetModulationType() const [member function]
    cls.add_method('GetModulationType', 
                   'ns3::WimaxPhy::ModulationType', 
                   [], 
                   is_const=True)
    ## ss-record.h: bool ns3::SSRecord::GetPollForRanging() const [member function]
    cls.add_method('GetPollForRanging', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-record.h: bool ns3::SSRecord::GetPollMeBit() const [member function]
    cls.add_method('GetPollMeBit', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-record.h: ns3::Cid ns3::SSRecord::GetPrimaryCid() const [member function]
    cls.add_method('GetPrimaryCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## ss-record.h: uint8_t ns3::SSRecord::GetRangingCorrectionRetries() const [member function]
    cls.add_method('GetRangingCorrectionRetries', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ss-record.h: ns3::WimaxNetDevice::RangingStatus ns3::SSRecord::GetRangingStatus() const [member function]
    cls.add_method('GetRangingStatus', 
                   'ns3::WimaxNetDevice::RangingStatus', 
                   [], 
                   is_const=True)
    ## ss-record.h: std::vector<ns3::ServiceFlow*,std::allocator<ns3::ServiceFlow*> > ns3::SSRecord::GetServiceFlows(ns3::ServiceFlow::SchedulingType schedulingType) const [member function]
    cls.add_method('GetServiceFlows', 
                   'std::vector< ns3::ServiceFlow * >', 
                   [param('ns3::ServiceFlow::SchedulingType', 'schedulingType')], 
                   is_const=True)
    ## ss-record.h: uint16_t ns3::SSRecord::GetSfTransactionId() const [member function]
    cls.add_method('GetSfTransactionId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ss-record.h: void ns3::SSRecord::IncrementDsaRspRetries() [member function]
    cls.add_method('IncrementDsaRspRetries', 
                   'void', 
                   [])
    ## ss-record.h: void ns3::SSRecord::IncrementInvitedRangingRetries() [member function]
    cls.add_method('IncrementInvitedRangingRetries', 
                   'void', 
                   [])
    ## ss-record.h: void ns3::SSRecord::IncrementRangingCorrectionRetries() [member function]
    cls.add_method('IncrementRangingCorrectionRetries', 
                   'void', 
                   [])
    ## ss-record.h: void ns3::SSRecord::ResetInvitedRangingRetries() [member function]
    cls.add_method('ResetInvitedRangingRetries', 
                   'void', 
                   [])
    ## ss-record.h: void ns3::SSRecord::ResetRangingCorrectionRetries() [member function]
    cls.add_method('ResetRangingCorrectionRetries', 
                   'void', 
                   [])
    ## ss-record.h: void ns3::SSRecord::SetAreServiceFlowsAllocated(bool val) [member function]
    cls.add_method('SetAreServiceFlowsAllocated', 
                   'void', 
                   [param('bool', 'val')])
    ## ss-record.h: void ns3::SSRecord::SetBasicCid(ns3::Cid basicCid) [member function]
    cls.add_method('SetBasicCid', 
                   'void', 
                   [param('ns3::Cid', 'basicCid')])
    ## ss-record.h: void ns3::SSRecord::SetDsaRsp(ns3::DsaRsp dsaRsp) [member function]
    cls.add_method('SetDsaRsp', 
                   'void', 
                   [param('ns3::DsaRsp', 'dsaRsp')])
    ## ss-record.h: void ns3::SSRecord::SetDsaRspRetries(uint8_t dsaRspRetries) [member function]
    cls.add_method('SetDsaRspRetries', 
                   'void', 
                   [param('uint8_t', 'dsaRspRetries')])
    ## ss-record.h: void ns3::SSRecord::SetIPAddress(ns3::Ipv4Address IPaddress) [member function]
    cls.add_method('SetIPAddress', 
                   'void', 
                   [param('ns3::Ipv4Address', 'IPaddress')])
    ## ss-record.h: void ns3::SSRecord::SetIsBroadcastSS(bool arg0) [member function]
    cls.add_method('SetIsBroadcastSS', 
                   'void', 
                   [param('bool', 'arg0')])
    ## ss-record.h: void ns3::SSRecord::SetMacAddress(ns3::Mac48Address macAddress) [member function]
    cls.add_method('SetMacAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'macAddress')])
    ## ss-record.h: void ns3::SSRecord::SetModulationType(ns3::WimaxPhy::ModulationType modulationType) [member function]
    cls.add_method('SetModulationType', 
                   'void', 
                   [param('ns3::WimaxPhy::ModulationType', 'modulationType')])
    ## ss-record.h: void ns3::SSRecord::SetPollMeBit(bool pollMeBit) [member function]
    cls.add_method('SetPollMeBit', 
                   'void', 
                   [param('bool', 'pollMeBit')])
    ## ss-record.h: void ns3::SSRecord::SetPrimaryCid(ns3::Cid primaryCid) [member function]
    cls.add_method('SetPrimaryCid', 
                   'void', 
                   [param('ns3::Cid', 'primaryCid')])
    ## ss-record.h: void ns3::SSRecord::SetRangingStatus(ns3::WimaxNetDevice::RangingStatus rangingStatus) [member function]
    cls.add_method('SetRangingStatus', 
                   'void', 
                   [param('ns3::WimaxNetDevice::RangingStatus', 'rangingStatus')])
    ## ss-record.h: void ns3::SSRecord::SetSfTransactionId(uint16_t sfTransactionId) [member function]
    cls.add_method('SetSfTransactionId', 
                   'void', 
                   [param('uint16_t', 'sfTransactionId')])
    return

def register_Ns3SendParams_methods(root_module, cls):
    ## send-params.h: ns3::SendParams::SendParams(ns3::SendParams const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SendParams const &', 'arg0')])
    ## send-params.h: ns3::SendParams::SendParams() [constructor]
    cls.add_constructor([])
    return

def register_Ns3ServiceFlow_methods(root_module, cls):
    ## service-flow.h: ns3::ServiceFlow::ServiceFlow(ns3::Tlv tlv) [constructor]
    cls.add_constructor([param('ns3::Tlv', 'tlv')])
    ## service-flow.h: ns3::ServiceFlow::ServiceFlow(ns3::ServiceFlow::Direction direction) [constructor]
    cls.add_constructor([param('ns3::ServiceFlow::Direction', 'direction')])
    ## service-flow.h: ns3::ServiceFlow::ServiceFlow() [constructor]
    cls.add_constructor([])
    ## service-flow.h: ns3::ServiceFlow::ServiceFlow(ns3::ServiceFlow const & sf) [copy constructor]
    cls.add_constructor([param('ns3::ServiceFlow const &', 'sf')])
    ## service-flow.h: ns3::ServiceFlow::ServiceFlow(uint32_t sfid, ns3::ServiceFlow::Direction direction, ns3::Ptr<ns3::WimaxConnection> connection) [constructor]
    cls.add_constructor([param('uint32_t', 'sfid'), param('ns3::ServiceFlow::Direction', 'direction'), param('ns3::Ptr< ns3::WimaxConnection >', 'connection')])
    ## service-flow.h: bool ns3::ServiceFlow::CheckClassifierMatch(ns3::Ipv4Address srcAddress, ns3::Ipv4Address dstAddress, uint16_t srcPort, uint16_t dstPort, uint8_t proto) const [member function]
    cls.add_method('CheckClassifierMatch', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'srcAddress'), param('ns3::Ipv4Address', 'dstAddress'), param('uint16_t', 'srcPort'), param('uint16_t', 'dstPort'), param('uint8_t', 'proto')], 
                   is_const=True)
    ## service-flow.h: void ns3::ServiceFlow::CleanUpQueue() [member function]
    cls.add_method('CleanUpQueue', 
                   'void', 
                   [])
    ## service-flow.h: void ns3::ServiceFlow::CopyParametersFrom(ns3::ServiceFlow sf) [member function]
    cls.add_method('CopyParametersFrom', 
                   'void', 
                   [param('ns3::ServiceFlow', 'sf')])
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetArqBlockLifeTime() const [member function]
    cls.add_method('GetArqBlockLifeTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetArqBlockSize() const [member function]
    cls.add_method('GetArqBlockSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint8_t ns3::ServiceFlow::GetArqDeliverInOrder() const [member function]
    cls.add_method('GetArqDeliverInOrder', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint8_t ns3::ServiceFlow::GetArqEnable() const [member function]
    cls.add_method('GetArqEnable', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetArqPurgeTimeout() const [member function]
    cls.add_method('GetArqPurgeTimeout', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetArqRetryTimeoutRx() const [member function]
    cls.add_method('GetArqRetryTimeoutRx', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetArqRetryTimeoutTx() const [member function]
    cls.add_method('GetArqRetryTimeoutTx', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetArqSyncLoss() const [member function]
    cls.add_method('GetArqSyncLoss', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetArqWindowSize() const [member function]
    cls.add_method('GetArqWindowSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::Ptr<ns3::WimaxConnection> ns3::ServiceFlow::GetConnection() const [member function]
    cls.add_method('GetConnection', 
                   'ns3::Ptr< ns3::WimaxConnection >', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::CsParameters ns3::ServiceFlow::GetConvergenceSublayerParam() const [member function]
    cls.add_method('GetConvergenceSublayerParam', 
                   'ns3::CsParameters', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::ServiceFlow::CsSpecification ns3::ServiceFlow::GetCsSpecification() const [member function]
    cls.add_method('GetCsSpecification', 
                   'ns3::ServiceFlow::CsSpecification', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::ServiceFlow::Direction ns3::ServiceFlow::GetDirection() const [member function]
    cls.add_method('GetDirection', 
                   'ns3::ServiceFlow::Direction', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint8_t ns3::ServiceFlow::GetFixedversusVariableSduIndicator() const [member function]
    cls.add_method('GetFixedversusVariableSduIndicator', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: bool ns3::ServiceFlow::GetIsEnabled() const [member function]
    cls.add_method('GetIsEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## service-flow.h: bool ns3::ServiceFlow::GetIsMulticast() const [member function]
    cls.add_method('GetIsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint32_t ns3::ServiceFlow::GetMaxSustainedTrafficRate() const [member function]
    cls.add_method('GetMaxSustainedTrafficRate', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint32_t ns3::ServiceFlow::GetMaxTrafficBurst() const [member function]
    cls.add_method('GetMaxTrafficBurst', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint32_t ns3::ServiceFlow::GetMaximumLatency() const [member function]
    cls.add_method('GetMaximumLatency', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint32_t ns3::ServiceFlow::GetMinReservedTrafficRate() const [member function]
    cls.add_method('GetMinReservedTrafficRate', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint32_t ns3::ServiceFlow::GetMinTolerableTrafficRate() const [member function]
    cls.add_method('GetMinTolerableTrafficRate', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::WimaxPhy::ModulationType ns3::ServiceFlow::GetModulation() const [member function]
    cls.add_method('GetModulation', 
                   'ns3::WimaxPhy::ModulationType', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint8_t ns3::ServiceFlow::GetQosParamSetType() const [member function]
    cls.add_method('GetQosParamSetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::Ptr<ns3::WimaxMacQueue> ns3::ServiceFlow::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::WimaxMacQueue >', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::ServiceFlowRecord * ns3::ServiceFlow::GetRecord() const [member function]
    cls.add_method('GetRecord', 
                   'ns3::ServiceFlowRecord *', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint32_t ns3::ServiceFlow::GetRequestTransmissionPolicy() const [member function]
    cls.add_method('GetRequestTransmissionPolicy', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::ServiceFlow::SchedulingType ns3::ServiceFlow::GetSchedulingType() const [member function]
    cls.add_method('GetSchedulingType', 
                   'ns3::ServiceFlow::SchedulingType', 
                   [], 
                   is_const=True)
    ## service-flow.h: char * ns3::ServiceFlow::GetSchedulingTypeStr() const [member function]
    cls.add_method('GetSchedulingTypeStr', 
                   'char *', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint8_t ns3::ServiceFlow::GetSduSize() const [member function]
    cls.add_method('GetSduSize', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: std::string ns3::ServiceFlow::GetServiceClassName() const [member function]
    cls.add_method('GetServiceClassName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::ServiceFlow::SchedulingType ns3::ServiceFlow::GetServiceSchedulingType() const [member function]
    cls.add_method('GetServiceSchedulingType', 
                   'ns3::ServiceFlow::SchedulingType', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint32_t ns3::ServiceFlow::GetSfid() const [member function]
    cls.add_method('GetSfid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetTargetSAID() const [member function]
    cls.add_method('GetTargetSAID', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint32_t ns3::ServiceFlow::GetToleratedJitter() const [member function]
    cls.add_method('GetToleratedJitter', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint8_t ns3::ServiceFlow::GetTrafficPriority() const [member function]
    cls.add_method('GetTrafficPriority', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: ns3::ServiceFlow::Type ns3::ServiceFlow::GetType() const [member function]
    cls.add_method('GetType', 
                   'ns3::ServiceFlow::Type', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetUnsolicitedGrantInterval() const [member function]
    cls.add_method('GetUnsolicitedGrantInterval', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: uint16_t ns3::ServiceFlow::GetUnsolicitedPollingInterval() const [member function]
    cls.add_method('GetUnsolicitedPollingInterval', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## service-flow.h: bool ns3::ServiceFlow::HasPackets() const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [], 
                   is_const=True)
    ## service-flow.h: bool ns3::ServiceFlow::HasPackets(ns3::MacHeaderType::HeaderType packetType) const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')], 
                   is_const=True)
    ## service-flow.h: void ns3::ServiceFlow::InitValues() [member function]
    cls.add_method('InitValues', 
                   'void', 
                   [])
    ## service-flow.h: void ns3::ServiceFlow::PrintQoSParameters() const [member function]
    cls.add_method('PrintQoSParameters', 
                   'void', 
                   [], 
                   is_const=True)
    ## service-flow.h: void ns3::ServiceFlow::SetArqBlockLifeTime(uint16_t arg0) [member function]
    cls.add_method('SetArqBlockLifeTime', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetArqBlockSize(uint16_t arg0) [member function]
    cls.add_method('SetArqBlockSize', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetArqDeliverInOrder(uint8_t arg0) [member function]
    cls.add_method('SetArqDeliverInOrder', 
                   'void', 
                   [param('uint8_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetArqEnable(uint8_t arg0) [member function]
    cls.add_method('SetArqEnable', 
                   'void', 
                   [param('uint8_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetArqPurgeTimeout(uint16_t arg0) [member function]
    cls.add_method('SetArqPurgeTimeout', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetArqRetryTimeoutRx(uint16_t arg0) [member function]
    cls.add_method('SetArqRetryTimeoutRx', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetArqRetryTimeoutTx(uint16_t arg0) [member function]
    cls.add_method('SetArqRetryTimeoutTx', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetArqSyncLoss(uint16_t arg0) [member function]
    cls.add_method('SetArqSyncLoss', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetArqWindowSize(uint16_t arg0) [member function]
    cls.add_method('SetArqWindowSize', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetConnection(ns3::Ptr<ns3::WimaxConnection> connection) [member function]
    cls.add_method('SetConnection', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxConnection >', 'connection')])
    ## service-flow.h: void ns3::ServiceFlow::SetConvergenceSublayerParam(ns3::CsParameters arg0) [member function]
    cls.add_method('SetConvergenceSublayerParam', 
                   'void', 
                   [param('ns3::CsParameters', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetCsSpecification(ns3::ServiceFlow::CsSpecification arg0) [member function]
    cls.add_method('SetCsSpecification', 
                   'void', 
                   [param('ns3::ServiceFlow::CsSpecification', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetDirection(ns3::ServiceFlow::Direction direction) [member function]
    cls.add_method('SetDirection', 
                   'void', 
                   [param('ns3::ServiceFlow::Direction', 'direction')])
    ## service-flow.h: void ns3::ServiceFlow::SetFixedversusVariableSduIndicator(uint8_t arg0) [member function]
    cls.add_method('SetFixedversusVariableSduIndicator', 
                   'void', 
                   [param('uint8_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetIsEnabled(bool isEnabled) [member function]
    cls.add_method('SetIsEnabled', 
                   'void', 
                   [param('bool', 'isEnabled')])
    ## service-flow.h: void ns3::ServiceFlow::SetIsMulticast(bool isMulticast) [member function]
    cls.add_method('SetIsMulticast', 
                   'void', 
                   [param('bool', 'isMulticast')])
    ## service-flow.h: void ns3::ServiceFlow::SetMaxSustainedTrafficRate(uint32_t arg0) [member function]
    cls.add_method('SetMaxSustainedTrafficRate', 
                   'void', 
                   [param('uint32_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetMaxTrafficBurst(uint32_t arg0) [member function]
    cls.add_method('SetMaxTrafficBurst', 
                   'void', 
                   [param('uint32_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetMaximumLatency(uint32_t arg0) [member function]
    cls.add_method('SetMaximumLatency', 
                   'void', 
                   [param('uint32_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetMinReservedTrafficRate(uint32_t arg0) [member function]
    cls.add_method('SetMinReservedTrafficRate', 
                   'void', 
                   [param('uint32_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetMinTolerableTrafficRate(uint32_t arg0) [member function]
    cls.add_method('SetMinTolerableTrafficRate', 
                   'void', 
                   [param('uint32_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetModulation(ns3::WimaxPhy::ModulationType modulationType) [member function]
    cls.add_method('SetModulation', 
                   'void', 
                   [param('ns3::WimaxPhy::ModulationType', 'modulationType')])
    ## service-flow.h: void ns3::ServiceFlow::SetQosParamSetType(uint8_t arg0) [member function]
    cls.add_method('SetQosParamSetType', 
                   'void', 
                   [param('uint8_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetRecord(ns3::ServiceFlowRecord * record) [member function]
    cls.add_method('SetRecord', 
                   'void', 
                   [param('ns3::ServiceFlowRecord *', 'record')])
    ## service-flow.h: void ns3::ServiceFlow::SetRequestTransmissionPolicy(uint32_t arg0) [member function]
    cls.add_method('SetRequestTransmissionPolicy', 
                   'void', 
                   [param('uint32_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetSduSize(uint8_t arg0) [member function]
    cls.add_method('SetSduSize', 
                   'void', 
                   [param('uint8_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetServiceClassName(std::string arg0) [member function]
    cls.add_method('SetServiceClassName', 
                   'void', 
                   [param('std::string', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetServiceSchedulingType(ns3::ServiceFlow::SchedulingType arg0) [member function]
    cls.add_method('SetServiceSchedulingType', 
                   'void', 
                   [param('ns3::ServiceFlow::SchedulingType', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetSfid(uint32_t arg0) [member function]
    cls.add_method('SetSfid', 
                   'void', 
                   [param('uint32_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetTargetSAID(uint16_t arg0) [member function]
    cls.add_method('SetTargetSAID', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetToleratedJitter(uint32_t arg0) [member function]
    cls.add_method('SetToleratedJitter', 
                   'void', 
                   [param('uint32_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetTrafficPriority(uint8_t arg0) [member function]
    cls.add_method('SetTrafficPriority', 
                   'void', 
                   [param('uint8_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetType(ns3::ServiceFlow::Type type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('ns3::ServiceFlow::Type', 'type')])
    ## service-flow.h: void ns3::ServiceFlow::SetUnsolicitedGrantInterval(uint16_t arg0) [member function]
    cls.add_method('SetUnsolicitedGrantInterval', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: void ns3::ServiceFlow::SetUnsolicitedPollingInterval(uint16_t arg0) [member function]
    cls.add_method('SetUnsolicitedPollingInterval', 
                   'void', 
                   [param('uint16_t', 'arg0')])
    ## service-flow.h: ns3::Tlv ns3::ServiceFlow::ToTlv() const [member function]
    cls.add_method('ToTlv', 
                   'ns3::Tlv', 
                   [], 
                   is_const=True)
    return

def register_Ns3ServiceFlowRecord_methods(root_module, cls):
    ## service-flow-record.h: ns3::ServiceFlowRecord::ServiceFlowRecord(ns3::ServiceFlowRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ServiceFlowRecord const &', 'arg0')])
    ## service-flow-record.h: ns3::ServiceFlowRecord::ServiceFlowRecord() [constructor]
    cls.add_constructor([])
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetBacklogged() const [member function]
    cls.add_method('GetBacklogged', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetBackloggedTemp() const [member function]
    cls.add_method('GetBackloggedTemp', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetBwSinceLastExpiry() [member function]
    cls.add_method('GetBwSinceLastExpiry', 
                   'uint32_t', 
                   [])
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetBytesRcvd() const [member function]
    cls.add_method('GetBytesRcvd', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetBytesSent() const [member function]
    cls.add_method('GetBytesSent', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: ns3::Time ns3::ServiceFlowRecord::GetDlTimeStamp() const [member function]
    cls.add_method('GetDlTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetGrantSize() const [member function]
    cls.add_method('GetGrantSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: ns3::Time ns3::ServiceFlowRecord::GetGrantTimeStamp() const [member function]
    cls.add_method('GetGrantTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetGrantedBandwidth() [member function]
    cls.add_method('GetGrantedBandwidth', 
                   'uint32_t', 
                   [])
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetGrantedBandwidthTemp() [member function]
    cls.add_method('GetGrantedBandwidthTemp', 
                   'uint32_t', 
                   [])
    ## service-flow-record.h: ns3::Time ns3::ServiceFlowRecord::GetLastGrantTime() const [member function]
    cls.add_method('GetLastGrantTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetPktsRcvd() const [member function]
    cls.add_method('GetPktsRcvd', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetPktsSent() const [member function]
    cls.add_method('GetPktsSent', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow-record.h: uint32_t ns3::ServiceFlowRecord::GetRequestedBandwidth() [member function]
    cls.add_method('GetRequestedBandwidth', 
                   'uint32_t', 
                   [])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::IncreaseBacklogged(uint32_t backlogged) [member function]
    cls.add_method('IncreaseBacklogged', 
                   'void', 
                   [param('uint32_t', 'backlogged')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::IncreaseBackloggedTemp(uint32_t backloggedTemp) [member function]
    cls.add_method('IncreaseBackloggedTemp', 
                   'void', 
                   [param('uint32_t', 'backloggedTemp')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetBacklogged(uint32_t backlogged) [member function]
    cls.add_method('SetBacklogged', 
                   'void', 
                   [param('uint32_t', 'backlogged')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetBackloggedTemp(uint32_t backloggedTemp) [member function]
    cls.add_method('SetBackloggedTemp', 
                   'void', 
                   [param('uint32_t', 'backloggedTemp')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetBwSinceLastExpiry(uint32_t bwSinceLastExpiry) [member function]
    cls.add_method('SetBwSinceLastExpiry', 
                   'void', 
                   [param('uint32_t', 'bwSinceLastExpiry')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetBytesRcvd(uint32_t bytesRcvd) [member function]
    cls.add_method('SetBytesRcvd', 
                   'void', 
                   [param('uint32_t', 'bytesRcvd')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetBytesSent(uint32_t bytesSent) [member function]
    cls.add_method('SetBytesSent', 
                   'void', 
                   [param('uint32_t', 'bytesSent')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetDlTimeStamp(ns3::Time dlTimeStamp) [member function]
    cls.add_method('SetDlTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'dlTimeStamp')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetGrantSize(uint32_t grantSize) [member function]
    cls.add_method('SetGrantSize', 
                   'void', 
                   [param('uint32_t', 'grantSize')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetGrantTimeStamp(ns3::Time grantTimeStamp) [member function]
    cls.add_method('SetGrantTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'grantTimeStamp')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetGrantedBandwidth(uint32_t grantedBandwidth) [member function]
    cls.add_method('SetGrantedBandwidth', 
                   'void', 
                   [param('uint32_t', 'grantedBandwidth')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetGrantedBandwidthTemp(uint32_t grantedBandwidthTemp) [member function]
    cls.add_method('SetGrantedBandwidthTemp', 
                   'void', 
                   [param('uint32_t', 'grantedBandwidthTemp')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetLastGrantTime(ns3::Time grantTime) [member function]
    cls.add_method('SetLastGrantTime', 
                   'void', 
                   [param('ns3::Time', 'grantTime')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetPktsRcvd(uint32_t pktsRcvd) [member function]
    cls.add_method('SetPktsRcvd', 
                   'void', 
                   [param('uint32_t', 'pktsRcvd')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetPktsSent(uint32_t pktsSent) [member function]
    cls.add_method('SetPktsSent', 
                   'void', 
                   [param('uint32_t', 'pktsSent')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::SetRequestedBandwidth(uint32_t requestedBandwidth) [member function]
    cls.add_method('SetRequestedBandwidth', 
                   'void', 
                   [param('uint32_t', 'requestedBandwidth')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::UpdateBwSinceLastExpiry(uint32_t bwSinceLastExpiry) [member function]
    cls.add_method('UpdateBwSinceLastExpiry', 
                   'void', 
                   [param('uint32_t', 'bwSinceLastExpiry')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::UpdateBytesRcvd(uint32_t bytesRcvd) [member function]
    cls.add_method('UpdateBytesRcvd', 
                   'void', 
                   [param('uint32_t', 'bytesRcvd')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::UpdateBytesSent(uint32_t bytesSent) [member function]
    cls.add_method('UpdateBytesSent', 
                   'void', 
                   [param('uint32_t', 'bytesSent')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::UpdateGrantedBandwidth(uint32_t grantedBandwidth) [member function]
    cls.add_method('UpdateGrantedBandwidth', 
                   'void', 
                   [param('uint32_t', 'grantedBandwidth')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::UpdateGrantedBandwidthTemp(uint32_t grantedBandwidthTemp) [member function]
    cls.add_method('UpdateGrantedBandwidthTemp', 
                   'void', 
                   [param('uint32_t', 'grantedBandwidthTemp')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::UpdatePktsRcvd(uint32_t pktsRcvd) [member function]
    cls.add_method('UpdatePktsRcvd', 
                   'void', 
                   [param('uint32_t', 'pktsRcvd')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::UpdatePktsSent(uint32_t pktsSent) [member function]
    cls.add_method('UpdatePktsSent', 
                   'void', 
                   [param('uint32_t', 'pktsSent')])
    ## service-flow-record.h: void ns3::ServiceFlowRecord::UpdateRequestedBandwidth(uint32_t requestedBandwidth) [member function]
    cls.add_method('UpdateRequestedBandwidth', 
                   'void', 
                   [param('uint32_t', 'requestedBandwidth')])
    return

def register_Ns3TlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::TlvValue::TlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::TlvValue::TlvValue(ns3::TlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::TlvValue * ns3::TlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::TlvValue *', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::TlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLen) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLen')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::TlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wimax-tlv.h: void ns3::TlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3TosTlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::TosTlvValue::TosTlvValue(ns3::TosTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TosTlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::TosTlvValue::TosTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::TosTlvValue::TosTlvValue(uint8_t arg0, uint8_t arg1, uint8_t arg2) [constructor]
    cls.add_constructor([param('uint8_t', 'arg0'), param('uint8_t', 'arg1'), param('uint8_t', 'arg2')])
    ## wimax-tlv.h: ns3::TosTlvValue * ns3::TosTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::TosTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::TosTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    ## wimax-tlv.h: uint8_t ns3::TosTlvValue::GetHigh() const [member function]
    cls.add_method('GetHigh', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint8_t ns3::TosTlvValue::GetLow() const [member function]
    cls.add_method('GetLow', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint8_t ns3::TosTlvValue::GetMask() const [member function]
    cls.add_method('GetMask', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint32_t ns3::TosTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: void ns3::TosTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3U16TlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::U16TlvValue::U16TlvValue(ns3::U16TlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::U16TlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::U16TlvValue::U16TlvValue(uint16_t value) [constructor]
    cls.add_constructor([param('uint16_t', 'value')])
    ## wimax-tlv.h: ns3::U16TlvValue::U16TlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::U16TlvValue * ns3::U16TlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::U16TlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::U16TlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLen) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLen')], 
                   is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::U16TlvValue::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## wimax-tlv.h: uint32_t ns3::U16TlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint16_t ns3::U16TlvValue::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: void ns3::U16TlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3U32TlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::U32TlvValue::U32TlvValue(ns3::U32TlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::U32TlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::U32TlvValue::U32TlvValue(uint32_t value) [constructor]
    cls.add_constructor([param('uint32_t', 'value')])
    ## wimax-tlv.h: ns3::U32TlvValue::U32TlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::U32TlvValue * ns3::U32TlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::U32TlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::U32TlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLen) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLen')], 
                   is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::U32TlvValue::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## wimax-tlv.h: uint32_t ns3::U32TlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::U32TlvValue::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: void ns3::U32TlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3U8TlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::U8TlvValue::U8TlvValue(ns3::U8TlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::U8TlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::U8TlvValue::U8TlvValue(uint8_t value) [constructor]
    cls.add_constructor([param('uint8_t', 'value')])
    ## wimax-tlv.h: ns3::U8TlvValue::U8TlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::U8TlvValue * ns3::U8TlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::U8TlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::U8TlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLen) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLen')], 
                   is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::U8TlvValue::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## wimax-tlv.h: uint32_t ns3::U8TlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint8_t ns3::U8TlvValue::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: void ns3::U8TlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3UcdChannelEncodings_methods(root_module, cls):
    ## ul-mac-messages.h: ns3::UcdChannelEncodings::UcdChannelEncodings(ns3::UcdChannelEncodings const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UcdChannelEncodings const &', 'arg0')])
    ## ul-mac-messages.h: ns3::UcdChannelEncodings::UcdChannelEncodings() [constructor]
    cls.add_constructor([])
    ## ul-mac-messages.h: uint16_t ns3::UcdChannelEncodings::GetBwReqOppSize() const [member function]
    cls.add_method('GetBwReqOppSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint32_t ns3::UcdChannelEncodings::GetFrequency() const [member function]
    cls.add_method('GetFrequency', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint16_t ns3::UcdChannelEncodings::GetRangReqOppSize() const [member function]
    cls.add_method('GetRangReqOppSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint16_t ns3::UcdChannelEncodings::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::UcdChannelEncodings::Read(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Read', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## ul-mac-messages.h: void ns3::UcdChannelEncodings::SetBwReqOppSize(uint16_t bwReqOppSize) [member function]
    cls.add_method('SetBwReqOppSize', 
                   'void', 
                   [param('uint16_t', 'bwReqOppSize')])
    ## ul-mac-messages.h: void ns3::UcdChannelEncodings::SetFrequency(uint32_t frequency) [member function]
    cls.add_method('SetFrequency', 
                   'void', 
                   [param('uint32_t', 'frequency')])
    ## ul-mac-messages.h: void ns3::UcdChannelEncodings::SetRangReqOppSize(uint16_t rangReqOppSize) [member function]
    cls.add_method('SetRangReqOppSize', 
                   'void', 
                   [param('uint16_t', 'rangReqOppSize')])
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::UcdChannelEncodings::Write(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Write', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::UcdChannelEncodings::DoRead(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DoRead', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::UcdChannelEncodings::DoWrite(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('DoWrite', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3VectorTlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::VectorTlvValue::VectorTlvValue(ns3::VectorTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VectorTlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::VectorTlvValue::VectorTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: void ns3::VectorTlvValue::Add(ns3::Tlv const & val) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Tlv const &', 'val')])
    ## wimax-tlv.h: __gnu_cxx::__normal_iterator<ns3::Tlv* const*,std::vector<ns3::Tlv*, std::allocator<ns3::Tlv*> > > ns3::VectorTlvValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Tlv * const *, std::vector< ns3::Tlv * > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: ns3::VectorTlvValue * ns3::VectorTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::VectorTlvValue *', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::VectorTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wimax-tlv.h: __gnu_cxx::__normal_iterator<ns3::Tlv* const*,std::vector<ns3::Tlv*, std::allocator<ns3::Tlv*> > > ns3::VectorTlvValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Tlv * const *, std::vector< ns3::Tlv * > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint32_t ns3::VectorTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: void ns3::VectorTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3WimaxHelper_methods(root_module, cls):
    ## wimax-helper.h: ns3::WimaxHelper::WimaxHelper(ns3::WimaxHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WimaxHelper const &', 'arg0')])
    ## wimax-helper.h: ns3::WimaxHelper::WimaxHelper() [constructor]
    cls.add_constructor([])
    ## wimax-helper.h: ns3::Ptr<ns3::BSScheduler> ns3::WimaxHelper::CreateBSScheduler(ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('CreateBSScheduler', 
                   'ns3::Ptr< ns3::BSScheduler >', 
                   [param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxHelper::CreatePhy(ns3::WimaxHelper::PhyType phyType) [member function]
    cls.add_method('CreatePhy', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [param('ns3::WimaxHelper::PhyType', 'phyType')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxHelper::CreatePhy(ns3::WimaxHelper::PhyType phyType, char * SNRTraceFilePath, bool activateLoss) [member function]
    cls.add_method('CreatePhy', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [param('ns3::WimaxHelper::PhyType', 'phyType'), param('char *', 'SNRTraceFilePath'), param('bool', 'activateLoss')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxHelper::CreatePhyWithoutChannel(ns3::WimaxHelper::PhyType phyType) [member function]
    cls.add_method('CreatePhyWithoutChannel', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [param('ns3::WimaxHelper::PhyType', 'phyType')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxHelper::CreatePhyWithoutChannel(ns3::WimaxHelper::PhyType phyType, char * SNRTraceFilePath, bool activateLoss) [member function]
    cls.add_method('CreatePhyWithoutChannel', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [param('ns3::WimaxHelper::PhyType', 'phyType'), param('char *', 'SNRTraceFilePath'), param('bool', 'activateLoss')])
    ## wimax-helper.h: ns3::ServiceFlow ns3::WimaxHelper::CreateServiceFlow(ns3::ServiceFlow::Direction direction, ns3::ServiceFlow::SchedulingType schedulinType, ns3::IpcsClassifierRecord classifier) [member function]
    cls.add_method('CreateServiceFlow', 
                   'ns3::ServiceFlow', 
                   [param('ns3::ServiceFlow::Direction', 'direction'), param('ns3::ServiceFlow::SchedulingType', 'schedulinType'), param('ns3::IpcsClassifierRecord', 'classifier')])
    ## wimax-helper.h: ns3::Ptr<ns3::UplinkScheduler> ns3::WimaxHelper::CreateUplinkScheduler(ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('CreateUplinkScheduler', 
                   'ns3::Ptr< ns3::UplinkScheduler >', 
                   [param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: static void ns3::WimaxHelper::EnableAsciiForConnection(ns3::Ptr<ns3::OutputStreamWrapper> oss, uint32_t nodeid, uint32_t deviceid, char * netdevice, char * connection) [member function]
    cls.add_method('EnableAsciiForConnection', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'oss'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('char *', 'netdevice'), param('char *', 'connection')], 
                   is_static=True)
    ## wimax-helper.h: static void ns3::WimaxHelper::EnableLogComponents() [member function]
    cls.add_method('EnableLogComponents', 
                   'void', 
                   [], 
                   is_static=True)
    ## wimax-helper.h: ns3::NetDeviceContainer ns3::WimaxHelper::Install(ns3::NodeContainer c, ns3::WimaxHelper::NetDeviceType type, ns3::WimaxHelper::PhyType phyType, ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::WimaxHelper::NetDeviceType', 'type'), param('ns3::WimaxHelper::PhyType', 'phyType'), param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: ns3::NetDeviceContainer ns3::WimaxHelper::Install(ns3::NodeContainer c, ns3::WimaxHelper::NetDeviceType deviceType, ns3::WimaxHelper::PhyType phyType, ns3::Ptr<ns3::WimaxChannel> channel, ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::WimaxHelper::NetDeviceType', 'deviceType'), param('ns3::WimaxHelper::PhyType', 'phyType'), param('ns3::Ptr< ns3::WimaxChannel >', 'channel'), param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: ns3::NetDeviceContainer ns3::WimaxHelper::Install(ns3::NodeContainer c, ns3::WimaxHelper::NetDeviceType deviceType, ns3::WimaxHelper::PhyType phyType, ns3::WimaxHelper::SchedulerType schedulerType, double frameDuration) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::WimaxHelper::NetDeviceType', 'deviceType'), param('ns3::WimaxHelper::PhyType', 'phyType'), param('ns3::WimaxHelper::SchedulerType', 'schedulerType'), param('double', 'frameDuration')])
    ## wimax-helper.h: ns3::Ptr<ns3::WimaxNetDevice> ns3::WimaxHelper::Install(ns3::Ptr<ns3::Node> node, ns3::WimaxHelper::NetDeviceType deviceType, ns3::WimaxHelper::PhyType phyType, ns3::Ptr<ns3::WimaxChannel> channel, ns3::WimaxHelper::SchedulerType schedulerType) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::WimaxNetDevice >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::WimaxHelper::NetDeviceType', 'deviceType'), param('ns3::WimaxHelper::PhyType', 'phyType'), param('ns3::Ptr< ns3::WimaxChannel >', 'channel'), param('ns3::WimaxHelper::SchedulerType', 'schedulerType')])
    ## wimax-helper.h: void ns3::WimaxHelper::SetPropagationLossModel(ns3::SimpleOfdmWimaxChannel::PropModel propagationModel) [member function]
    cls.add_method('SetPropagationLossModel', 
                   'void', 
                   [param('ns3::SimpleOfdmWimaxChannel::PropModel', 'propagationModel')])
    ## wimax-helper.h: void ns3::WimaxHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## wimax-helper.h: void ns3::WimaxHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename, bool promiscuous) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename'), param('bool', 'promiscuous')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3SimpleOfdmSendParam_methods(root_module, cls):
    ## simple-ofdm-send-param.h: ns3::simpleOfdmSendParam::simpleOfdmSendParam(ns3::simpleOfdmSendParam const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::simpleOfdmSendParam const &', 'arg0')])
    ## simple-ofdm-send-param.h: ns3::simpleOfdmSendParam::simpleOfdmSendParam() [constructor]
    cls.add_constructor([])
    ## simple-ofdm-send-param.h: ns3::simpleOfdmSendParam::simpleOfdmSendParam(ns3::bvec const & fecBlock, uint32_t burstSize, bool isFirstBlock, uint64_t Frequency, ns3::WimaxPhy::ModulationType modulationType, uint8_t direction, double rxPowerDbm) [constructor]
    cls.add_constructor([param('ns3::bvec const &', 'fecBlock'), param('uint32_t', 'burstSize'), param('bool', 'isFirstBlock'), param('uint64_t', 'Frequency'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('uint8_t', 'direction'), param('double', 'rxPowerDbm')])
    ## simple-ofdm-send-param.h: ns3::simpleOfdmSendParam::simpleOfdmSendParam(uint32_t burstSize, bool isFirstBlock, uint64_t Frequency, ns3::WimaxPhy::ModulationType modulationType, uint8_t direction, double rxPowerDbm, ns3::Ptr<ns3::PacketBurst> burst) [constructor]
    cls.add_constructor([param('uint32_t', 'burstSize'), param('bool', 'isFirstBlock'), param('uint64_t', 'Frequency'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('uint8_t', 'direction'), param('double', 'rxPowerDbm'), param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-send-param.h: ns3::Ptr<ns3::PacketBurst> ns3::simpleOfdmSendParam::GetBurst() [member function]
    cls.add_method('GetBurst', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [])
    ## simple-ofdm-send-param.h: uint32_t ns3::simpleOfdmSendParam::GetBurstSize() [member function]
    cls.add_method('GetBurstSize', 
                   'uint32_t', 
                   [])
    ## simple-ofdm-send-param.h: uint8_t ns3::simpleOfdmSendParam::GetDirection() [member function]
    cls.add_method('GetDirection', 
                   'uint8_t', 
                   [])
    ## simple-ofdm-send-param.h: ns3::bvec ns3::simpleOfdmSendParam::GetFecBlock() [member function]
    cls.add_method('GetFecBlock', 
                   'ns3::bvec', 
                   [])
    ## simple-ofdm-send-param.h: uint64_t ns3::simpleOfdmSendParam::GetFrequency() [member function]
    cls.add_method('GetFrequency', 
                   'uint64_t', 
                   [])
    ## simple-ofdm-send-param.h: bool ns3::simpleOfdmSendParam::GetIsFirstBlock() [member function]
    cls.add_method('GetIsFirstBlock', 
                   'bool', 
                   [])
    ## simple-ofdm-send-param.h: ns3::WimaxPhy::ModulationType ns3::simpleOfdmSendParam::GetModulationType() [member function]
    cls.add_method('GetModulationType', 
                   'ns3::WimaxPhy::ModulationType', 
                   [])
    ## simple-ofdm-send-param.h: double ns3::simpleOfdmSendParam::GetRxPowerDbm() [member function]
    cls.add_method('GetRxPowerDbm', 
                   'double', 
                   [])
    ## simple-ofdm-send-param.h: void ns3::simpleOfdmSendParam::SetBurstSize(uint32_t burstSize) [member function]
    cls.add_method('SetBurstSize', 
                   'void', 
                   [param('uint32_t', 'burstSize')])
    ## simple-ofdm-send-param.h: void ns3::simpleOfdmSendParam::SetDirection(uint8_t direction) [member function]
    cls.add_method('SetDirection', 
                   'void', 
                   [param('uint8_t', 'direction')])
    ## simple-ofdm-send-param.h: void ns3::simpleOfdmSendParam::SetFecBlock(ns3::bvec const & fecBlock) [member function]
    cls.add_method('SetFecBlock', 
                   'void', 
                   [param('ns3::bvec const &', 'fecBlock')])
    ## simple-ofdm-send-param.h: void ns3::simpleOfdmSendParam::SetFrequency(uint64_t Frequency) [member function]
    cls.add_method('SetFrequency', 
                   'void', 
                   [param('uint64_t', 'Frequency')])
    ## simple-ofdm-send-param.h: void ns3::simpleOfdmSendParam::SetIsFirstBlock(bool isFirstBlock) [member function]
    cls.add_method('SetIsFirstBlock', 
                   'void', 
                   [param('bool', 'isFirstBlock')])
    ## simple-ofdm-send-param.h: void ns3::simpleOfdmSendParam::SetModulationType(ns3::WimaxPhy::ModulationType modulationType) [member function]
    cls.add_method('SetModulationType', 
                   'void', 
                   [param('ns3::WimaxPhy::ModulationType', 'modulationType')])
    ## simple-ofdm-send-param.h: void ns3::simpleOfdmSendParam::SetRxPowerDbm(double rxPowerDbm) [member function]
    cls.add_method('SetRxPowerDbm', 
                   'void', 
                   [param('double', 'rxPowerDbm')])
    return

def register_Ns3ClassificationRuleVectorTlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::ClassificationRuleVectorTlvValue::ClassificationRuleVectorTlvValue(ns3::ClassificationRuleVectorTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ClassificationRuleVectorTlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::ClassificationRuleVectorTlvValue::ClassificationRuleVectorTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::ClassificationRuleVectorTlvValue * ns3::ClassificationRuleVectorTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::ClassificationRuleVectorTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::ClassificationRuleVectorTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    return

def register_Ns3CsParamVectorTlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::CsParamVectorTlvValue::CsParamVectorTlvValue(ns3::CsParamVectorTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsParamVectorTlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::CsParamVectorTlvValue::CsParamVectorTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::CsParamVectorTlvValue * ns3::CsParamVectorTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::CsParamVectorTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::CsParamVectorTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    return

def register_Ns3Ipv4AddressTlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue::Ipv4AddressTlvValue(ns3::Ipv4AddressTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressTlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue::Ipv4AddressTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: void ns3::Ipv4AddressTlvValue::Add(ns3::Ipv4Address address, ns3::Ipv4Mask Mask) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ipv4Address', 'address'), param('ns3::Ipv4Mask', 'Mask')])
    ## wimax-tlv.h: __gnu_cxx::__normal_iterator<const ns3::Ipv4AddressTlvValue::ipv4Addr*,std::vector<ns3::Ipv4AddressTlvValue::ipv4Addr, std::allocator<ns3::Ipv4AddressTlvValue::ipv4Addr> > > ns3::Ipv4AddressTlvValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ipv4AddressTlvValue::ipv4Addr const *, std::vector< ns3::Ipv4AddressTlvValue::ipv4Addr > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue * ns3::Ipv4AddressTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ipv4AddressTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::Ipv4AddressTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    ## wimax-tlv.h: __gnu_cxx::__normal_iterator<const ns3::Ipv4AddressTlvValue::ipv4Addr*,std::vector<ns3::Ipv4AddressTlvValue::ipv4Addr, std::allocator<ns3::Ipv4AddressTlvValue::ipv4Addr> > > ns3::Ipv4AddressTlvValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ipv4AddressTlvValue::ipv4Addr const *, std::vector< ns3::Ipv4AddressTlvValue::ipv4Addr > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint32_t ns3::Ipv4AddressTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: void ns3::Ipv4AddressTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv4AddressTlvValueIpv4Addr_methods(root_module, cls):
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue::ipv4Addr::ipv4Addr() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue::ipv4Addr::ipv4Addr(ns3::Ipv4AddressTlvValue::ipv4Addr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressTlvValue::ipv4Addr const &', 'arg0')])
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue::ipv4Addr::Address [variable]
    cls.add_instance_attribute('Address', 'ns3::Ipv4Address', is_const=False)
    ## wimax-tlv.h: ns3::Ipv4AddressTlvValue::ipv4Addr::Mask [variable]
    cls.add_instance_attribute('Mask', 'ns3::Ipv4Mask', is_const=False)
    return

def register_Ns3MacHeaderType_methods(root_module, cls):
    ## wimax-mac-header.h: ns3::MacHeaderType::MacHeaderType(ns3::MacHeaderType const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacHeaderType const &', 'arg0')])
    ## wimax-mac-header.h: ns3::MacHeaderType::MacHeaderType() [constructor]
    cls.add_constructor([])
    ## wimax-mac-header.h: ns3::MacHeaderType::MacHeaderType(uint8_t type) [constructor]
    cls.add_constructor([param('uint8_t', 'type')])
    ## wimax-mac-header.h: uint32_t ns3::MacHeaderType::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wimax-mac-header.h: ns3::TypeId ns3::MacHeaderType::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: std::string ns3::MacHeaderType::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint32_t ns3::MacHeaderType::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: uint8_t ns3::MacHeaderType::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: static ns3::TypeId ns3::MacHeaderType::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-mac-header.h: void ns3::MacHeaderType::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::MacHeaderType::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::MacHeaderType::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    return

def register_Ns3ManagementMessageType_methods(root_module, cls):
    ## mac-messages.h: ns3::ManagementMessageType::ManagementMessageType(ns3::ManagementMessageType const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ManagementMessageType const &', 'arg0')])
    ## mac-messages.h: ns3::ManagementMessageType::ManagementMessageType() [constructor]
    cls.add_constructor([])
    ## mac-messages.h: ns3::ManagementMessageType::ManagementMessageType(uint8_t type) [constructor]
    cls.add_constructor([param('uint8_t', 'type')])
    ## mac-messages.h: uint32_t ns3::ManagementMessageType::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mac-messages.h: ns3::TypeId ns3::ManagementMessageType::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: std::string ns3::ManagementMessageType::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::ManagementMessageType::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: uint8_t ns3::ManagementMessageType::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: static ns3::TypeId ns3::ManagementMessageType::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-messages.h: void ns3::ManagementMessageType::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::ManagementMessageType::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::ManagementMessageType::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    return

def register_Ns3OfdmDownlinkFramePrefix_methods(root_module, cls):
    ## ofdm-downlink-frame-prefix.h: ns3::OfdmDownlinkFramePrefix::OfdmDownlinkFramePrefix(ns3::OfdmDownlinkFramePrefix const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OfdmDownlinkFramePrefix const &', 'arg0')])
    ## ofdm-downlink-frame-prefix.h: ns3::OfdmDownlinkFramePrefix::OfdmDownlinkFramePrefix() [constructor]
    cls.add_constructor([])
    ## ofdm-downlink-frame-prefix.h: void ns3::OfdmDownlinkFramePrefix::AddDlFramePrefixElement(ns3::DlFramePrefixIe dlFramePrefixElement) [member function]
    cls.add_method('AddDlFramePrefixElement', 
                   'void', 
                   [param('ns3::DlFramePrefixIe', 'dlFramePrefixElement')])
    ## ofdm-downlink-frame-prefix.h: uint32_t ns3::OfdmDownlinkFramePrefix::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ofdm-downlink-frame-prefix.h: ns3::Mac48Address ns3::OfdmDownlinkFramePrefix::GetBaseStationId() const [member function]
    cls.add_method('GetBaseStationId', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint8_t ns3::OfdmDownlinkFramePrefix::GetConfigurationChangeCount() const [member function]
    cls.add_method('GetConfigurationChangeCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: std::vector<ns3::DlFramePrefixIe, std::allocator<ns3::DlFramePrefixIe> > ns3::OfdmDownlinkFramePrefix::GetDlFramePrefixElements() const [member function]
    cls.add_method('GetDlFramePrefixElements', 
                   'std::vector< ns3::DlFramePrefixIe >', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint32_t ns3::OfdmDownlinkFramePrefix::GetFrameNumber() const [member function]
    cls.add_method('GetFrameNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint8_t ns3::OfdmDownlinkFramePrefix::GetHcs() const [member function]
    cls.add_method('GetHcs', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: std::string ns3::OfdmDownlinkFramePrefix::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## ofdm-downlink-frame-prefix.h: uint32_t ns3::OfdmDownlinkFramePrefix::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ofdm-downlink-frame-prefix.h: void ns3::OfdmDownlinkFramePrefix::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ofdm-downlink-frame-prefix.h: void ns3::OfdmDownlinkFramePrefix::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ofdm-downlink-frame-prefix.h: void ns3::OfdmDownlinkFramePrefix::SetBaseStationId(ns3::Mac48Address baseStationId) [member function]
    cls.add_method('SetBaseStationId', 
                   'void', 
                   [param('ns3::Mac48Address', 'baseStationId')])
    ## ofdm-downlink-frame-prefix.h: void ns3::OfdmDownlinkFramePrefix::SetConfigurationChangeCount(uint8_t configurationChangeCount) [member function]
    cls.add_method('SetConfigurationChangeCount', 
                   'void', 
                   [param('uint8_t', 'configurationChangeCount')])
    ## ofdm-downlink-frame-prefix.h: void ns3::OfdmDownlinkFramePrefix::SetFrameNumber(uint32_t frameNumber) [member function]
    cls.add_method('SetFrameNumber', 
                   'void', 
                   [param('uint32_t', 'frameNumber')])
    ## ofdm-downlink-frame-prefix.h: void ns3::OfdmDownlinkFramePrefix::SetHcs(uint8_t hcs) [member function]
    cls.add_method('SetHcs', 
                   'void', 
                   [param('uint8_t', 'hcs')])
    return

def register_Ns3OfdmSendParams_methods(root_module, cls):
    ## send-params.h: ns3::OfdmSendParams::OfdmSendParams(ns3::OfdmSendParams const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OfdmSendParams const &', 'arg0')])
    ## send-params.h: ns3::OfdmSendParams::OfdmSendParams(ns3::Ptr<ns3::PacketBurst> burst, uint8_t modulationType, uint8_t direction) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::PacketBurst >', 'burst'), param('uint8_t', 'modulationType'), param('uint8_t', 'direction')])
    ## send-params.h: ns3::Ptr<ns3::PacketBurst> ns3::OfdmSendParams::GetBurst() const [member function]
    cls.add_method('GetBurst', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [], 
                   is_const=True)
    ## send-params.h: uint8_t ns3::OfdmSendParams::GetDirection() const [member function]
    cls.add_method('GetDirection', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## send-params.h: uint8_t ns3::OfdmSendParams::GetModulationType() const [member function]
    cls.add_method('GetModulationType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3OfdmUcdChannelEncodings_methods(root_module, cls):
    ## ul-mac-messages.h: ns3::OfdmUcdChannelEncodings::OfdmUcdChannelEncodings(ns3::OfdmUcdChannelEncodings const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OfdmUcdChannelEncodings const &', 'arg0')])
    ## ul-mac-messages.h: ns3::OfdmUcdChannelEncodings::OfdmUcdChannelEncodings() [constructor]
    cls.add_constructor([])
    ## ul-mac-messages.h: uint8_t ns3::OfdmUcdChannelEncodings::GetSbchnlFocContCodes() const [member function]
    cls.add_method('GetSbchnlFocContCodes', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::OfdmUcdChannelEncodings::GetSbchnlReqRegionFullParams() const [member function]
    cls.add_method('GetSbchnlReqRegionFullParams', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint16_t ns3::OfdmUcdChannelEncodings::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: void ns3::OfdmUcdChannelEncodings::SetSbchnlFocContCodes(uint8_t sbchnlFocContCodes) [member function]
    cls.add_method('SetSbchnlFocContCodes', 
                   'void', 
                   [param('uint8_t', 'sbchnlFocContCodes')])
    ## ul-mac-messages.h: void ns3::OfdmUcdChannelEncodings::SetSbchnlReqRegionFullParams(uint8_t sbchnlReqRegionFullParams) [member function]
    cls.add_method('SetSbchnlReqRegionFullParams', 
                   'void', 
                   [param('uint8_t', 'sbchnlReqRegionFullParams')])
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmUcdChannelEncodings::DoRead(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DoRead', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   visibility='private', is_virtual=True)
    ## ul-mac-messages.h: ns3::Buffer::Iterator ns3::OfdmUcdChannelEncodings::DoWrite(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('DoWrite', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3PortRangeTlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::PortRangeTlvValue::PortRangeTlvValue(ns3::PortRangeTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PortRangeTlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::PortRangeTlvValue::PortRangeTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: void ns3::PortRangeTlvValue::Add(uint16_t portLow, uint16_t portHigh) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('uint16_t', 'portLow'), param('uint16_t', 'portHigh')])
    ## wimax-tlv.h: __gnu_cxx::__normal_iterator<const ns3::PortRangeTlvValue::PortRange*,std::vector<ns3::PortRangeTlvValue::PortRange, std::allocator<ns3::PortRangeTlvValue::PortRange> > > ns3::PortRangeTlvValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::PortRangeTlvValue::PortRange const *, std::vector< ns3::PortRangeTlvValue::PortRange > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: ns3::PortRangeTlvValue * ns3::PortRangeTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::PortRangeTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::PortRangeTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    ## wimax-tlv.h: __gnu_cxx::__normal_iterator<const ns3::PortRangeTlvValue::PortRange*,std::vector<ns3::PortRangeTlvValue::PortRange, std::allocator<ns3::PortRangeTlvValue::PortRange> > > ns3::PortRangeTlvValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::PortRangeTlvValue::PortRange const *, std::vector< ns3::PortRangeTlvValue::PortRange > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint32_t ns3::PortRangeTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: void ns3::PortRangeTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3PortRangeTlvValuePortRange_methods(root_module, cls):
    ## wimax-tlv.h: ns3::PortRangeTlvValue::PortRange::PortRange() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::PortRangeTlvValue::PortRange::PortRange(ns3::PortRangeTlvValue::PortRange const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PortRangeTlvValue::PortRange const &', 'arg0')])
    ## wimax-tlv.h: ns3::PortRangeTlvValue::PortRange::PortHigh [variable]
    cls.add_instance_attribute('PortHigh', 'uint16_t', is_const=False)
    ## wimax-tlv.h: ns3::PortRangeTlvValue::PortRange::PortLow [variable]
    cls.add_instance_attribute('PortLow', 'uint16_t', is_const=False)
    return

def register_Ns3PriorityUlJob_methods(root_module, cls):
    ## ul-job.h: ns3::PriorityUlJob::PriorityUlJob(ns3::PriorityUlJob const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PriorityUlJob const &', 'arg0')])
    ## ul-job.h: ns3::PriorityUlJob::PriorityUlJob() [constructor]
    cls.add_constructor([])
    ## ul-job.h: int ns3::PriorityUlJob::GetPriority() [member function]
    cls.add_method('GetPriority', 
                   'int', 
                   [])
    ## ul-job.h: ns3::Ptr<ns3::UlJob> ns3::PriorityUlJob::GetUlJob() [member function]
    cls.add_method('GetUlJob', 
                   'ns3::Ptr< ns3::UlJob >', 
                   [])
    ## ul-job.h: void ns3::PriorityUlJob::SetPriority(int priority) [member function]
    cls.add_method('SetPriority', 
                   'void', 
                   [param('int', 'priority')])
    ## ul-job.h: void ns3::PriorityUlJob::SetUlJob(ns3::Ptr<ns3::UlJob> job) [member function]
    cls.add_method('SetUlJob', 
                   'void', 
                   [param('ns3::Ptr< ns3::UlJob >', 'job')])
    return

def register_Ns3ProtocolTlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::ProtocolTlvValue::ProtocolTlvValue(ns3::ProtocolTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ProtocolTlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::ProtocolTlvValue::ProtocolTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: void ns3::ProtocolTlvValue::Add(uint8_t protiocol) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('uint8_t', 'protiocol')])
    ## wimax-tlv.h: __gnu_cxx::__normal_iterator<const unsigned char*,std::vector<unsigned char, std::allocator<unsigned char> > > ns3::ProtocolTlvValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< unsigned char const *, std::vector< unsigned char > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: ns3::ProtocolTlvValue * ns3::ProtocolTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::ProtocolTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::ProtocolTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    ## wimax-tlv.h: __gnu_cxx::__normal_iterator<const unsigned char*,std::vector<unsigned char, std::allocator<unsigned char> > > ns3::ProtocolTlvValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< unsigned char const *, std::vector< unsigned char > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint32_t ns3::ProtocolTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: void ns3::ProtocolTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3RngReq_methods(root_module, cls):
    ## mac-messages.h: ns3::RngReq::RngReq(ns3::RngReq const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RngReq const &', 'arg0')])
    ## mac-messages.h: ns3::RngReq::RngReq() [constructor]
    cls.add_constructor([])
    ## mac-messages.h: uint32_t ns3::RngReq::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mac-messages.h: ns3::TypeId ns3::RngReq::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: ns3::Mac48Address ns3::RngReq::GetMacAddress() const [member function]
    cls.add_method('GetMacAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mac-messages.h: std::string ns3::RngReq::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint8_t ns3::RngReq::GetRangingAnomalies() const [member function]
    cls.add_method('GetRangingAnomalies', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint8_t ns3::RngReq::GetReqDlBurstProfile() const [member function]
    cls.add_method('GetReqDlBurstProfile', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::RngReq::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: static ns3::TypeId ns3::RngReq::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-messages.h: void ns3::RngReq::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::RngReq::PrintDebug() const [member function]
    cls.add_method('PrintDebug', 
                   'void', 
                   [], 
                   is_const=True)
    ## mac-messages.h: void ns3::RngReq::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::RngReq::SetMacAddress(ns3::Mac48Address macAddress) [member function]
    cls.add_method('SetMacAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'macAddress')])
    ## mac-messages.h: void ns3::RngReq::SetRangingAnomalies(uint8_t rangingAnomalies) [member function]
    cls.add_method('SetRangingAnomalies', 
                   'void', 
                   [param('uint8_t', 'rangingAnomalies')])
    ## mac-messages.h: void ns3::RngReq::SetReqDlBurstProfile(uint8_t reqDlBurstProfile) [member function]
    cls.add_method('SetReqDlBurstProfile', 
                   'void', 
                   [param('uint8_t', 'reqDlBurstProfile')])
    return

def register_Ns3RngRsp_methods(root_module, cls):
    ## mac-messages.h: ns3::RngRsp::RngRsp(ns3::RngRsp const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RngRsp const &', 'arg0')])
    ## mac-messages.h: ns3::RngRsp::RngRsp() [constructor]
    cls.add_constructor([])
    ## mac-messages.h: uint32_t ns3::RngRsp::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mac-messages.h: uint8_t ns3::RngRsp::GetAasBdcastPermission() const [member function]
    cls.add_method('GetAasBdcastPermission', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: ns3::Cid ns3::RngRsp::GetBasicCid() const [member function]
    cls.add_method('GetBasicCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::RngRsp::GetDlFreqOverride() const [member function]
    cls.add_method('GetDlFreqOverride', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint16_t ns3::RngRsp::GetDlOperBurstProfile() const [member function]
    cls.add_method('GetDlOperBurstProfile', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::RngRsp::GetFrameNumber() const [member function]
    cls.add_method('GetFrameNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint8_t ns3::RngRsp::GetInitRangOppNumber() const [member function]
    cls.add_method('GetInitRangOppNumber', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: ns3::TypeId ns3::RngRsp::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: ns3::Mac48Address ns3::RngRsp::GetMacAddress() const [member function]
    cls.add_method('GetMacAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mac-messages.h: std::string ns3::RngRsp::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::RngRsp::GetOffsetFreqAdjust() const [member function]
    cls.add_method('GetOffsetFreqAdjust', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint8_t ns3::RngRsp::GetPowerLevelAdjust() const [member function]
    cls.add_method('GetPowerLevelAdjust', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: ns3::Cid ns3::RngRsp::GetPrimaryCid() const [member function]
    cls.add_method('GetPrimaryCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint8_t ns3::RngRsp::GetRangStatus() const [member function]
    cls.add_method('GetRangStatus', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint8_t ns3::RngRsp::GetRangSubchnl() const [member function]
    cls.add_method('GetRangSubchnl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::RngRsp::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: uint32_t ns3::RngRsp::GetTimingAdjust() const [member function]
    cls.add_method('GetTimingAdjust', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: static ns3::TypeId ns3::RngRsp::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-messages.h: uint8_t ns3::RngRsp::GetUlChnlIdOverride() const [member function]
    cls.add_method('GetUlChnlIdOverride', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: void ns3::RngRsp::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::RngRsp::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::RngRsp::SetAasBdcastPermission(uint8_t aasBdcastPermission) [member function]
    cls.add_method('SetAasBdcastPermission', 
                   'void', 
                   [param('uint8_t', 'aasBdcastPermission')])
    ## mac-messages.h: void ns3::RngRsp::SetBasicCid(ns3::Cid basicCid) [member function]
    cls.add_method('SetBasicCid', 
                   'void', 
                   [param('ns3::Cid', 'basicCid')])
    ## mac-messages.h: void ns3::RngRsp::SetDlFreqOverride(uint32_t dlFreqOverride) [member function]
    cls.add_method('SetDlFreqOverride', 
                   'void', 
                   [param('uint32_t', 'dlFreqOverride')])
    ## mac-messages.h: void ns3::RngRsp::SetDlOperBurstProfile(uint16_t dlOperBurstProfile) [member function]
    cls.add_method('SetDlOperBurstProfile', 
                   'void', 
                   [param('uint16_t', 'dlOperBurstProfile')])
    ## mac-messages.h: void ns3::RngRsp::SetFrameNumber(uint32_t frameNumber) [member function]
    cls.add_method('SetFrameNumber', 
                   'void', 
                   [param('uint32_t', 'frameNumber')])
    ## mac-messages.h: void ns3::RngRsp::SetInitRangOppNumber(uint8_t initRangOppNumber) [member function]
    cls.add_method('SetInitRangOppNumber', 
                   'void', 
                   [param('uint8_t', 'initRangOppNumber')])
    ## mac-messages.h: void ns3::RngRsp::SetMacAddress(ns3::Mac48Address macAddress) [member function]
    cls.add_method('SetMacAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'macAddress')])
    ## mac-messages.h: void ns3::RngRsp::SetOffsetFreqAdjust(uint32_t offsetFreqAdjust) [member function]
    cls.add_method('SetOffsetFreqAdjust', 
                   'void', 
                   [param('uint32_t', 'offsetFreqAdjust')])
    ## mac-messages.h: void ns3::RngRsp::SetPowerLevelAdjust(uint8_t powerLevelAdjust) [member function]
    cls.add_method('SetPowerLevelAdjust', 
                   'void', 
                   [param('uint8_t', 'powerLevelAdjust')])
    ## mac-messages.h: void ns3::RngRsp::SetPrimaryCid(ns3::Cid primaryCid) [member function]
    cls.add_method('SetPrimaryCid', 
                   'void', 
                   [param('ns3::Cid', 'primaryCid')])
    ## mac-messages.h: void ns3::RngRsp::SetRangStatus(uint8_t rangStatus) [member function]
    cls.add_method('SetRangStatus', 
                   'void', 
                   [param('uint8_t', 'rangStatus')])
    ## mac-messages.h: void ns3::RngRsp::SetRangSubchnl(uint8_t rangSubchnl) [member function]
    cls.add_method('SetRangSubchnl', 
                   'void', 
                   [param('uint8_t', 'rangSubchnl')])
    ## mac-messages.h: void ns3::RngRsp::SetTimingAdjust(uint32_t timingAdjust) [member function]
    cls.add_method('SetTimingAdjust', 
                   'void', 
                   [param('uint32_t', 'timingAdjust')])
    ## mac-messages.h: void ns3::RngRsp::SetUlChnlIdOverride(uint8_t ulChnlIdOverride) [member function]
    cls.add_method('SetUlChnlIdOverride', 
                   'void', 
                   [param('uint8_t', 'ulChnlIdOverride')])
    return

def register_Ns3SSManager_methods(root_module, cls):
    ## ss-manager.h: ns3::SSManager::SSManager(ns3::SSManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SSManager const &', 'arg0')])
    ## ss-manager.h: ns3::SSManager::SSManager() [constructor]
    cls.add_constructor([])
    ## ss-manager.h: ns3::SSRecord * ns3::SSManager::CreateSSRecord(ns3::Mac48Address const & macAddress) [member function]
    cls.add_method('CreateSSRecord', 
                   'ns3::SSRecord *', 
                   [param('ns3::Mac48Address const &', 'macAddress')])
    ## ss-manager.h: void ns3::SSManager::DeleteSSRecord(ns3::Cid cid) [member function]
    cls.add_method('DeleteSSRecord', 
                   'void', 
                   [param('ns3::Cid', 'cid')])
    ## ss-manager.h: ns3::Mac48Address ns3::SSManager::GetMacAddress(ns3::Cid cid) const [member function]
    cls.add_method('GetMacAddress', 
                   'ns3::Mac48Address', 
                   [param('ns3::Cid', 'cid')], 
                   is_const=True)
    ## ss-manager.h: uint32_t ns3::SSManager::GetNRegisteredSSs() const [member function]
    cls.add_method('GetNRegisteredSSs', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ss-manager.h: uint32_t ns3::SSManager::GetNSSs() const [member function]
    cls.add_method('GetNSSs', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ss-manager.h: ns3::SSRecord * ns3::SSManager::GetSSRecord(ns3::Mac48Address const & macAddress) const [member function]
    cls.add_method('GetSSRecord', 
                   'ns3::SSRecord *', 
                   [param('ns3::Mac48Address const &', 'macAddress')], 
                   is_const=True)
    ## ss-manager.h: ns3::SSRecord * ns3::SSManager::GetSSRecord(ns3::Cid cid) const [member function]
    cls.add_method('GetSSRecord', 
                   'ns3::SSRecord *', 
                   [param('ns3::Cid', 'cid')], 
                   is_const=True)
    ## ss-manager.h: std::vector<ns3::SSRecord*,std::allocator<ns3::SSRecord*> > * ns3::SSManager::GetSSRecords() const [member function]
    cls.add_method('GetSSRecords', 
                   'std::vector< ns3::SSRecord * > *', 
                   [], 
                   is_const=True)
    ## ss-manager.h: bool ns3::SSManager::IsInRecord(ns3::Mac48Address const & macAddress) const [member function]
    cls.add_method('IsInRecord', 
                   'bool', 
                   [param('ns3::Mac48Address const &', 'macAddress')], 
                   is_const=True)
    ## ss-manager.h: bool ns3::SSManager::IsRegistered(ns3::Mac48Address const & macAddress) const [member function]
    cls.add_method('IsRegistered', 
                   'bool', 
                   [param('ns3::Mac48Address const &', 'macAddress')], 
                   is_const=True)
    return

def register_Ns3ServiceFlowManager_methods(root_module, cls):
    ## service-flow-manager.h: ns3::ServiceFlowManager::ServiceFlowManager(ns3::ServiceFlowManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ServiceFlowManager const &', 'arg0')])
    ## service-flow-manager.h: ns3::ServiceFlowManager::ServiceFlowManager() [constructor]
    cls.add_constructor([])
    ## service-flow-manager.h: void ns3::ServiceFlowManager::AddServiceFlow(ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('AddServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow *', 'serviceFlow')])
    ## service-flow-manager.h: bool ns3::ServiceFlowManager::AreServiceFlowsAllocated() [member function]
    cls.add_method('AreServiceFlowsAllocated', 
                   'bool', 
                   [])
    ## service-flow-manager.h: bool ns3::ServiceFlowManager::AreServiceFlowsAllocated(std::vector<ns3::ServiceFlow*,std::allocator<ns3::ServiceFlow*> > * serviceFlows) [member function]
    cls.add_method('AreServiceFlowsAllocated', 
                   'bool', 
                   [param('std::vector< ns3::ServiceFlow * > *', 'serviceFlows')])
    ## service-flow-manager.h: bool ns3::ServiceFlowManager::AreServiceFlowsAllocated(std::vector<ns3::ServiceFlow*,std::allocator<ns3::ServiceFlow*> > serviceFlows) [member function]
    cls.add_method('AreServiceFlowsAllocated', 
                   'bool', 
                   [param('std::vector< ns3::ServiceFlow * >', 'serviceFlows')])
    ## service-flow-manager.h: ns3::ServiceFlow * ns3::ServiceFlowManager::DoClassify(ns3::Ipv4Address SrcAddress, ns3::Ipv4Address DstAddress, uint16_t SrcPort, uint16_t DstPort, uint8_t Proto, ns3::ServiceFlow::Direction dir) const [member function]
    cls.add_method('DoClassify', 
                   'ns3::ServiceFlow *', 
                   [param('ns3::Ipv4Address', 'SrcAddress'), param('ns3::Ipv4Address', 'DstAddress'), param('uint16_t', 'SrcPort'), param('uint16_t', 'DstPort'), param('uint8_t', 'Proto'), param('ns3::ServiceFlow::Direction', 'dir')], 
                   is_const=True)
    ## service-flow-manager.h: void ns3::ServiceFlowManager::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## service-flow-manager.h: ns3::ServiceFlow * ns3::ServiceFlowManager::GetNextServiceFlowToAllocate() [member function]
    cls.add_method('GetNextServiceFlowToAllocate', 
                   'ns3::ServiceFlow *', 
                   [])
    ## service-flow-manager.h: uint32_t ns3::ServiceFlowManager::GetNrServiceFlows() const [member function]
    cls.add_method('GetNrServiceFlows', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## service-flow-manager.h: ns3::ServiceFlow * ns3::ServiceFlowManager::GetServiceFlow(uint32_t sfid) const [member function]
    cls.add_method('GetServiceFlow', 
                   'ns3::ServiceFlow *', 
                   [param('uint32_t', 'sfid')], 
                   is_const=True)
    ## service-flow-manager.h: ns3::ServiceFlow * ns3::ServiceFlowManager::GetServiceFlow(ns3::Cid cid) const [member function]
    cls.add_method('GetServiceFlow', 
                   'ns3::ServiceFlow *', 
                   [param('ns3::Cid', 'cid')], 
                   is_const=True)
    ## service-flow-manager.h: std::vector<ns3::ServiceFlow*,std::allocator<ns3::ServiceFlow*> > ns3::ServiceFlowManager::GetServiceFlows(ns3::ServiceFlow::SchedulingType schedulingType) const [member function]
    cls.add_method('GetServiceFlows', 
                   'std::vector< ns3::ServiceFlow * >', 
                   [param('ns3::ServiceFlow::SchedulingType', 'schedulingType')], 
                   is_const=True)
    return

def register_Ns3SfVectorTlvValue_methods(root_module, cls):
    ## wimax-tlv.h: ns3::SfVectorTlvValue::SfVectorTlvValue(ns3::SfVectorTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SfVectorTlvValue const &', 'arg0')])
    ## wimax-tlv.h: ns3::SfVectorTlvValue::SfVectorTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::SfVectorTlvValue * ns3::SfVectorTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::SfVectorTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint32_t ns3::SfVectorTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    return

def register_Ns3SsServiceFlowManager_methods(root_module, cls):
    ## ss-service-flow-manager.h: ns3::SsServiceFlowManager::SsServiceFlowManager(ns3::SsServiceFlowManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SsServiceFlowManager const &', 'arg0')])
    ## ss-service-flow-manager.h: ns3::SsServiceFlowManager::SsServiceFlowManager(ns3::Ptr<ns3::SubscriberStationNetDevice> device) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::SubscriberStationNetDevice >', 'device')])
    ## ss-service-flow-manager.h: void ns3::SsServiceFlowManager::AddServiceFlow(ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('AddServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow *', 'serviceFlow')])
    ## ss-service-flow-manager.h: void ns3::SsServiceFlowManager::AddServiceFlow(ns3::ServiceFlow serviceFlow) [member function]
    cls.add_method('AddServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow', 'serviceFlow')])
    ## ss-service-flow-manager.h: ns3::Ptr<ns3::Packet> ns3::SsServiceFlowManager::CreateDsaAck() [member function]
    cls.add_method('CreateDsaAck', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## ss-service-flow-manager.h: ns3::DsaReq ns3::SsServiceFlowManager::CreateDsaReq(ns3::ServiceFlow const * serviceFlow) [member function]
    cls.add_method('CreateDsaReq', 
                   'ns3::DsaReq', 
                   [param('ns3::ServiceFlow const *', 'serviceFlow')])
    ## ss-service-flow-manager.h: void ns3::SsServiceFlowManager::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ss-service-flow-manager.h: ns3::EventId ns3::SsServiceFlowManager::GetDsaAckTimeoutEvent() const [member function]
    cls.add_method('GetDsaAckTimeoutEvent', 
                   'ns3::EventId', 
                   [], 
                   is_const=True)
    ## ss-service-flow-manager.h: ns3::EventId ns3::SsServiceFlowManager::GetDsaRspTimeoutEvent() const [member function]
    cls.add_method('GetDsaRspTimeoutEvent', 
                   'ns3::EventId', 
                   [], 
                   is_const=True)
    ## ss-service-flow-manager.h: uint8_t ns3::SsServiceFlowManager::GetMaxDsaReqRetries() const [member function]
    cls.add_method('GetMaxDsaReqRetries', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ss-service-flow-manager.h: void ns3::SsServiceFlowManager::InitiateServiceFlows() [member function]
    cls.add_method('InitiateServiceFlows', 
                   'void', 
                   [])
    ## ss-service-flow-manager.h: void ns3::SsServiceFlowManager::ProcessDsaRsp(ns3::DsaRsp const & dsaRsp) [member function]
    cls.add_method('ProcessDsaRsp', 
                   'void', 
                   [param('ns3::DsaRsp const &', 'dsaRsp')])
    ## ss-service-flow-manager.h: void ns3::SsServiceFlowManager::ScheduleDsaReq(ns3::ServiceFlow const * serviceFlow) [member function]
    cls.add_method('ScheduleDsaReq', 
                   'void', 
                   [param('ns3::ServiceFlow const *', 'serviceFlow')])
    ## ss-service-flow-manager.h: void ns3::SsServiceFlowManager::SetMaxDsaReqRetries(uint8_t maxDsaReqRetries) [member function]
    cls.add_method('SetMaxDsaReqRetries', 
                   'void', 
                   [param('uint8_t', 'maxDsaReqRetries')])
    return

def register_Ns3Tlv_methods(root_module, cls):
    ## wimax-tlv.h: ns3::Tlv::Tlv(uint8_t type, uint64_t length, ns3::TlvValue const & value) [constructor]
    cls.add_constructor([param('uint8_t', 'type'), param('uint64_t', 'length'), param('ns3::TlvValue const &', 'value')])
    ## wimax-tlv.h: ns3::Tlv::Tlv() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h: ns3::Tlv::Tlv(ns3::Tlv const & tlv) [copy constructor]
    cls.add_constructor([param('ns3::Tlv const &', 'tlv')])
    ## wimax-tlv.h: ns3::Tlv * ns3::Tlv::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Tlv *', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: ns3::TlvValue * ns3::Tlv::CopyValue() const [member function]
    cls.add_method('CopyValue', 
                   'ns3::TlvValue *', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint32_t ns3::Tlv::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wimax-tlv.h: ns3::TypeId ns3::Tlv::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint64_t ns3::Tlv::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint32_t ns3::Tlv::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: uint8_t ns3::Tlv::GetSizeOfLen() const [member function]
    cls.add_method('GetSizeOfLen', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: uint8_t ns3::Tlv::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h: ns3::TlvValue * ns3::Tlv::PeekValue() [member function]
    cls.add_method('PeekValue', 
                   'ns3::TlvValue *', 
                   [])
    ## wimax-tlv.h: void ns3::Tlv::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h: void ns3::Tlv::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ucd_methods(root_module, cls):
    ## ul-mac-messages.h: ns3::Ucd::Ucd(ns3::Ucd const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ucd const &', 'arg0')])
    ## ul-mac-messages.h: ns3::Ucd::Ucd() [constructor]
    cls.add_constructor([])
    ## ul-mac-messages.h: void ns3::Ucd::AddUlBurstProfile(ns3::OfdmUlBurstProfile ulBurstProfile) [member function]
    cls.add_method('AddUlBurstProfile', 
                   'void', 
                   [param('ns3::OfdmUlBurstProfile', 'ulBurstProfile')])
    ## ul-mac-messages.h: uint32_t ns3::Ucd::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ul-mac-messages.h: ns3::OfdmUcdChannelEncodings ns3::Ucd::GetChannelEncodings() const [member function]
    cls.add_method('GetChannelEncodings', 
                   'ns3::OfdmUcdChannelEncodings', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::Ucd::GetConfigurationChangeCount() const [member function]
    cls.add_method('GetConfigurationChangeCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: ns3::TypeId ns3::Ucd::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ul-mac-messages.h: std::string ns3::Ucd::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::Ucd::GetNrUlBurstProfiles() const [member function]
    cls.add_method('GetNrUlBurstProfiles', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::Ucd::GetRangingBackoffEnd() const [member function]
    cls.add_method('GetRangingBackoffEnd', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::Ucd::GetRangingBackoffStart() const [member function]
    cls.add_method('GetRangingBackoffStart', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::Ucd::GetRequestBackoffEnd() const [member function]
    cls.add_method('GetRequestBackoffEnd', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint8_t ns3::Ucd::GetRequestBackoffStart() const [member function]
    cls.add_method('GetRequestBackoffStart', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint32_t ns3::Ucd::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ul-mac-messages.h: static ns3::TypeId ns3::Ucd::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ul-mac-messages.h: std::vector<ns3::OfdmUlBurstProfile, std::allocator<ns3::OfdmUlBurstProfile> > ns3::Ucd::GetUlBurstProfiles() const [member function]
    cls.add_method('GetUlBurstProfiles', 
                   'std::vector< ns3::OfdmUlBurstProfile >', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: void ns3::Ucd::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ul-mac-messages.h: void ns3::Ucd::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ul-mac-messages.h: void ns3::Ucd::SetChannelEncodings(ns3::OfdmUcdChannelEncodings channelEncodings) [member function]
    cls.add_method('SetChannelEncodings', 
                   'void', 
                   [param('ns3::OfdmUcdChannelEncodings', 'channelEncodings')])
    ## ul-mac-messages.h: void ns3::Ucd::SetConfigurationChangeCount(uint8_t ucdCount) [member function]
    cls.add_method('SetConfigurationChangeCount', 
                   'void', 
                   [param('uint8_t', 'ucdCount')])
    ## ul-mac-messages.h: void ns3::Ucd::SetNrUlBurstProfiles(uint8_t nrUlBurstProfiles) [member function]
    cls.add_method('SetNrUlBurstProfiles', 
                   'void', 
                   [param('uint8_t', 'nrUlBurstProfiles')])
    ## ul-mac-messages.h: void ns3::Ucd::SetRangingBackoffEnd(uint8_t rangingBackoffEnd) [member function]
    cls.add_method('SetRangingBackoffEnd', 
                   'void', 
                   [param('uint8_t', 'rangingBackoffEnd')])
    ## ul-mac-messages.h: void ns3::Ucd::SetRangingBackoffStart(uint8_t rangingBackoffStart) [member function]
    cls.add_method('SetRangingBackoffStart', 
                   'void', 
                   [param('uint8_t', 'rangingBackoffStart')])
    ## ul-mac-messages.h: void ns3::Ucd::SetRequestBackoffEnd(uint8_t requestBackoffEnd) [member function]
    cls.add_method('SetRequestBackoffEnd', 
                   'void', 
                   [param('uint8_t', 'requestBackoffEnd')])
    ## ul-mac-messages.h: void ns3::Ucd::SetRequestBackoffStart(uint8_t requestBackoffStart) [member function]
    cls.add_method('SetRequestBackoffStart', 
                   'void', 
                   [param('uint8_t', 'requestBackoffStart')])
    return

def register_Ns3UlJob_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## ul-job.h: ns3::UlJob::UlJob(ns3::UlJob const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UlJob const &', 'arg0')])
    ## ul-job.h: ns3::UlJob::UlJob() [constructor]
    cls.add_constructor([])
    ## ul-job.h: ns3::Time ns3::UlJob::GetDeadline() [member function]
    cls.add_method('GetDeadline', 
                   'ns3::Time', 
                   [])
    ## ul-job.h: ns3::Time ns3::UlJob::GetPeriod() [member function]
    cls.add_method('GetPeriod', 
                   'ns3::Time', 
                   [])
    ## ul-job.h: ns3::Time ns3::UlJob::GetReleaseTime() [member function]
    cls.add_method('GetReleaseTime', 
                   'ns3::Time', 
                   [])
    ## ul-job.h: ns3::ServiceFlow::SchedulingType ns3::UlJob::GetSchedulingType() [member function]
    cls.add_method('GetSchedulingType', 
                   'ns3::ServiceFlow::SchedulingType', 
                   [])
    ## ul-job.h: ns3::ServiceFlow * ns3::UlJob::GetServiceFlow() [member function]
    cls.add_method('GetServiceFlow', 
                   'ns3::ServiceFlow *', 
                   [])
    ## ul-job.h: uint32_t ns3::UlJob::GetSize() [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [])
    ## ul-job.h: ns3::SSRecord * ns3::UlJob::GetSsRecord() [member function]
    cls.add_method('GetSsRecord', 
                   'ns3::SSRecord *', 
                   [])
    ## ul-job.h: ns3::ReqType ns3::UlJob::GetType() [member function]
    cls.add_method('GetType', 
                   'ns3::ReqType', 
                   [])
    ## ul-job.h: void ns3::UlJob::SetDeadline(ns3::Time deadline) [member function]
    cls.add_method('SetDeadline', 
                   'void', 
                   [param('ns3::Time', 'deadline')])
    ## ul-job.h: void ns3::UlJob::SetPeriod(ns3::Time period) [member function]
    cls.add_method('SetPeriod', 
                   'void', 
                   [param('ns3::Time', 'period')])
    ## ul-job.h: void ns3::UlJob::SetReleaseTime(ns3::Time releaseTime) [member function]
    cls.add_method('SetReleaseTime', 
                   'void', 
                   [param('ns3::Time', 'releaseTime')])
    ## ul-job.h: void ns3::UlJob::SetSchedulingType(ns3::ServiceFlow::SchedulingType schedulingType) [member function]
    cls.add_method('SetSchedulingType', 
                   'void', 
                   [param('ns3::ServiceFlow::SchedulingType', 'schedulingType')])
    ## ul-job.h: void ns3::UlJob::SetServiceFlow(ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('SetServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow *', 'serviceFlow')])
    ## ul-job.h: void ns3::UlJob::SetSize(uint32_t size) [member function]
    cls.add_method('SetSize', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## ul-job.h: void ns3::UlJob::SetSsRecord(ns3::SSRecord * ssRecord) [member function]
    cls.add_method('SetSsRecord', 
                   'void', 
                   [param('ns3::SSRecord *', 'ssRecord')])
    ## ul-job.h: void ns3::UlJob::SetType(ns3::ReqType type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('ns3::ReqType', 'type')])
    return

def register_Ns3UlMap_methods(root_module, cls):
    ## ul-mac-messages.h: ns3::UlMap::UlMap(ns3::UlMap const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UlMap const &', 'arg0')])
    ## ul-mac-messages.h: ns3::UlMap::UlMap() [constructor]
    cls.add_constructor([])
    ## ul-mac-messages.h: void ns3::UlMap::AddUlMapElement(ns3::OfdmUlMapIe ulMapElement) [member function]
    cls.add_method('AddUlMapElement', 
                   'void', 
                   [param('ns3::OfdmUlMapIe', 'ulMapElement')])
    ## ul-mac-messages.h: uint32_t ns3::UlMap::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ul-mac-messages.h: uint32_t ns3::UlMap::GetAllocationStartTime() const [member function]
    cls.add_method('GetAllocationStartTime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: ns3::TypeId ns3::UlMap::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ul-mac-messages.h: std::string ns3::UlMap::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: uint32_t ns3::UlMap::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ul-mac-messages.h: static ns3::TypeId ns3::UlMap::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ul-mac-messages.h: uint8_t ns3::UlMap::GetUcdCount() const [member function]
    cls.add_method('GetUcdCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: std::list<ns3::OfdmUlMapIe, std::allocator<ns3::OfdmUlMapIe> > ns3::UlMap::GetUlMapElements() const [member function]
    cls.add_method('GetUlMapElements', 
                   'std::list< ns3::OfdmUlMapIe >', 
                   [], 
                   is_const=True)
    ## ul-mac-messages.h: void ns3::UlMap::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ul-mac-messages.h: void ns3::UlMap::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ul-mac-messages.h: void ns3::UlMap::SetAllocationStartTime(uint32_t allocationStartTime) [member function]
    cls.add_method('SetAllocationStartTime', 
                   'void', 
                   [param('uint32_t', 'allocationStartTime')])
    ## ul-mac-messages.h: void ns3::UlMap::SetUcdCount(uint8_t ucdCount) [member function]
    cls.add_method('SetUcdCount', 
                   'void', 
                   [param('uint8_t', 'ucdCount')])
    return

def register_Ns3UplinkScheduler_methods(root_module, cls):
    ## bs-uplink-scheduler.h: ns3::UplinkScheduler::UplinkScheduler(ns3::UplinkScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UplinkScheduler const &', 'arg0')])
    ## bs-uplink-scheduler.h: ns3::UplinkScheduler::UplinkScheduler() [constructor]
    cls.add_constructor([])
    ## bs-uplink-scheduler.h: ns3::UplinkScheduler::UplinkScheduler(ns3::Ptr<ns3::BaseStationNetDevice> bs) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::BaseStationNetDevice >', 'bs')])
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::AddUplinkAllocation(ns3::OfdmUlMapIe & ulMapIe, uint32_t const & allocationSize, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('AddUplinkAllocation', 
                   'void', 
                   [param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('uint32_t const &', 'allocationSize'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::AllocateInitialRangingInterval(uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('AllocateInitialRangingInterval', 
                   'void', 
                   [param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: uint32_t ns3::UplinkScheduler::CalculateAllocationStartTime() [member function]
    cls.add_method('CalculateAllocationStartTime', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: ns3::Ptr<ns3::BaseStationNetDevice> ns3::UplinkScheduler::GetBs() [member function]
    cls.add_method('GetBs', 
                   'ns3::Ptr< ns3::BaseStationNetDevice >', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::GetChannelDescriptorsToUpdate(bool & arg0, bool & arg1, bool & arg2, bool & arg3) [member function]
    cls.add_method('GetChannelDescriptorsToUpdate', 
                   'void', 
                   [param('bool &', 'arg0'), param('bool &', 'arg1'), param('bool &', 'arg2'), param('bool &', 'arg3')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: ns3::Time ns3::UplinkScheduler::GetDcdTimeStamp() const [member function]
    cls.add_method('GetDcdTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler.h: bool ns3::UplinkScheduler::GetIsInvIrIntrvlAllocated() const [member function]
    cls.add_method('GetIsInvIrIntrvlAllocated', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler.h: bool ns3::UplinkScheduler::GetIsIrIntrvlAllocated() const [member function]
    cls.add_method('GetIsIrIntrvlAllocated', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler.h: uint8_t ns3::UplinkScheduler::GetNrIrOppsAllocated() const [member function]
    cls.add_method('GetNrIrOppsAllocated', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler.h: ns3::Time ns3::UplinkScheduler::GetTimeStampIrInterval() [member function]
    cls.add_method('GetTimeStampIrInterval', 
                   'ns3::Time', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: static ns3::TypeId ns3::UplinkScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bs-uplink-scheduler.h: ns3::Time ns3::UplinkScheduler::GetUcdTimeStamp() const [member function]
    cls.add_method('GetUcdTimeStamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler.h: std::list<ns3::OfdmUlMapIe, std::allocator<ns3::OfdmUlMapIe> > ns3::UplinkScheduler::GetUplinkAllocations() const [member function]
    cls.add_method('GetUplinkAllocations', 
                   'std::list< ns3::OfdmUlMapIe >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::InitOnce() [member function]
    cls.add_method('InitOnce', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::OnSetRequestedBandwidth(ns3::ServiceFlowRecord * sfr) [member function]
    cls.add_method('OnSetRequestedBandwidth', 
                   'void', 
                   [param('ns3::ServiceFlowRecord *', 'sfr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::ProcessBandwidthRequest(ns3::BandwidthRequestHeader const & bwRequestHdr) [member function]
    cls.add_method('ProcessBandwidthRequest', 
                   'void', 
                   [param('ns3::BandwidthRequestHeader const &', 'bwRequestHdr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::Schedule() [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::ServiceBandwidthRequests(ns3::SSRecord const * ssRecord, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceBandwidthRequests', 
                   'void', 
                   [param('ns3::SSRecord const *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: bool ns3::UplinkScheduler::ServiceBandwidthRequests(ns3::ServiceFlow * serviceFlow, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceBandwidthRequests', 
                   'bool', 
                   [param('ns3::ServiceFlow *', 'serviceFlow'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::ServiceUnsolicitedGrants(ns3::SSRecord const * ssRecord, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceUnsolicitedGrants', 
                   'void', 
                   [param('ns3::SSRecord const *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::SetBs(ns3::Ptr<ns3::BaseStationNetDevice> bs) [member function]
    cls.add_method('SetBs', 
                   'void', 
                   [param('ns3::Ptr< ns3::BaseStationNetDevice >', 'bs')], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::SetDcdTimeStamp(ns3::Time dcdTimeStamp) [member function]
    cls.add_method('SetDcdTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'dcdTimeStamp')], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::SetIsInvIrIntrvlAllocated(bool isInvIrIntrvlAllocated) [member function]
    cls.add_method('SetIsInvIrIntrvlAllocated', 
                   'void', 
                   [param('bool', 'isInvIrIntrvlAllocated')], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::SetIsIrIntrvlAllocated(bool isIrIntrvlAllocated) [member function]
    cls.add_method('SetIsIrIntrvlAllocated', 
                   'void', 
                   [param('bool', 'isIrIntrvlAllocated')], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::SetNrIrOppsAllocated(uint8_t nrIrOppsAllocated) [member function]
    cls.add_method('SetNrIrOppsAllocated', 
                   'void', 
                   [param('uint8_t', 'nrIrOppsAllocated')], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::SetTimeStampIrInterval(ns3::Time timeStampIrInterval) [member function]
    cls.add_method('SetTimeStampIrInterval', 
                   'void', 
                   [param('ns3::Time', 'timeStampIrInterval')], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::SetUcdTimeStamp(ns3::Time ucdTimeStamp) [member function]
    cls.add_method('SetUcdTimeStamp', 
                   'void', 
                   [param('ns3::Time', 'ucdTimeStamp')], 
                   is_virtual=True)
    ## bs-uplink-scheduler.h: void ns3::UplinkScheduler::SetupServiceFlow(ns3::SSRecord * ssRecord, ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('SetupServiceFlow', 
                   'void', 
                   [param('ns3::SSRecord *', 'ssRecord'), param('ns3::ServiceFlow *', 'serviceFlow')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3UplinkSchedulerMBQoS_methods(root_module, cls):
    ## bs-uplink-scheduler-mbqos.h: ns3::UplinkSchedulerMBQoS::UplinkSchedulerMBQoS(ns3::UplinkSchedulerMBQoS const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UplinkSchedulerMBQoS const &', 'arg0')])
    ## bs-uplink-scheduler-mbqos.h: ns3::UplinkSchedulerMBQoS::UplinkSchedulerMBQoS() [constructor]
    cls.add_constructor([])
    ## bs-uplink-scheduler-mbqos.h: ns3::UplinkSchedulerMBQoS::UplinkSchedulerMBQoS(ns3::Time time) [constructor]
    cls.add_constructor([param('ns3::Time', 'time')])
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::AddUplinkAllocation(ns3::OfdmUlMapIe & ulMapIe, uint32_t const & allocationSize, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('AddUplinkAllocation', 
                   'void', 
                   [param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('uint32_t const &', 'allocationSize'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::AllocateInitialRangingInterval(uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('AllocateInitialRangingInterval', 
                   'void', 
                   [param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: uint32_t ns3::UplinkSchedulerMBQoS::CalculateAllocationStartTime() [member function]
    cls.add_method('CalculateAllocationStartTime', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::CheckDeadline(uint32_t & availableSymbols) [member function]
    cls.add_method('CheckDeadline', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::CheckMinimumBandwidth(uint32_t & availableSymbols) [member function]
    cls.add_method('CheckMinimumBandwidth', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-uplink-scheduler-mbqos.h: uint32_t ns3::UplinkSchedulerMBQoS::CountSymbolsJobs(ns3::Ptr<ns3::UlJob> job) [member function]
    cls.add_method('CountSymbolsJobs', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::UlJob >', 'job')])
    ## bs-uplink-scheduler-mbqos.h: uint32_t ns3::UplinkSchedulerMBQoS::CountSymbolsQueue(std::list<ns3::Ptr<ns3::UlJob>, std::allocator<ns3::Ptr<ns3::UlJob> > > jobs) [member function]
    cls.add_method('CountSymbolsQueue', 
                   'uint32_t', 
                   [param('std::list< ns3::Ptr< ns3::UlJob > >', 'jobs')])
    ## bs-uplink-scheduler-mbqos.h: ns3::Ptr<ns3::UlJob> ns3::UplinkSchedulerMBQoS::CreateUlJob(ns3::SSRecord * ssRecord, ns3::ServiceFlow::SchedulingType schedType, ns3::ReqType reqType) [member function]
    cls.add_method('CreateUlJob', 
                   'ns3::Ptr< ns3::UlJob >', 
                   [param('ns3::SSRecord *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedType'), param('ns3::ReqType', 'reqType')])
    ## bs-uplink-scheduler-mbqos.h: ns3::Ptr<ns3::UlJob> ns3::UplinkSchedulerMBQoS::DequeueJob(ns3::UlJob::JobPriority priority) [member function]
    cls.add_method('DequeueJob', 
                   'ns3::Ptr< ns3::UlJob >', 
                   [param('ns3::UlJob::JobPriority', 'priority')])
    ## bs-uplink-scheduler-mbqos.h: ns3::Time ns3::UplinkSchedulerMBQoS::DetermineDeadline(ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('DetermineDeadline', 
                   'ns3::Time', 
                   [param('ns3::ServiceFlow *', 'serviceFlow')])
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::EnqueueJob(ns3::UlJob::JobPriority priority, ns3::Ptr<ns3::UlJob> job) [member function]
    cls.add_method('EnqueueJob', 
                   'void', 
                   [param('ns3::UlJob::JobPriority', 'priority'), param('ns3::Ptr< ns3::UlJob >', 'job')])
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::GetChannelDescriptorsToUpdate(bool & arg0, bool & arg1, bool & arg2, bool & arg3) [member function]
    cls.add_method('GetChannelDescriptorsToUpdate', 
                   'void', 
                   [param('bool &', 'arg0'), param('bool &', 'arg1'), param('bool &', 'arg2'), param('bool &', 'arg3')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: uint32_t ns3::UplinkSchedulerMBQoS::GetPendingSize(ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('GetPendingSize', 
                   'uint32_t', 
                   [param('ns3::ServiceFlow *', 'serviceFlow')])
    ## bs-uplink-scheduler-mbqos.h: static ns3::TypeId ns3::UplinkSchedulerMBQoS::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bs-uplink-scheduler-mbqos.h: std::list<ns3::OfdmUlMapIe, std::allocator<ns3::OfdmUlMapIe> > ns3::UplinkSchedulerMBQoS::GetUplinkAllocations() const [member function]
    cls.add_method('GetUplinkAllocations', 
                   'std::list< ns3::OfdmUlMapIe >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::InitOnce() [member function]
    cls.add_method('InitOnce', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::OnSetRequestedBandwidth(ns3::ServiceFlowRecord * sfr) [member function]
    cls.add_method('OnSetRequestedBandwidth', 
                   'void', 
                   [param('ns3::ServiceFlowRecord *', 'sfr')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::ProcessBandwidthRequest(ns3::BandwidthRequestHeader const & bwRequestHdr) [member function]
    cls.add_method('ProcessBandwidthRequest', 
                   'void', 
                   [param('ns3::BandwidthRequestHeader const &', 'bwRequestHdr')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::Schedule() [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::ServiceBandwidthRequests(ns3::SSRecord const * ssRecord, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceBandwidthRequests', 
                   'void', 
                   [param('ns3::SSRecord const *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: bool ns3::UplinkSchedulerMBQoS::ServiceBandwidthRequests(ns3::ServiceFlow * serviceFlow, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceBandwidthRequests', 
                   'bool', 
                   [param('ns3::ServiceFlow *', 'serviceFlow'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: bool ns3::UplinkSchedulerMBQoS::ServiceBandwidthRequestsBytes(ns3::ServiceFlow * serviceFlow, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols, uint32_t allocationSizeBytes) [member function]
    cls.add_method('ServiceBandwidthRequestsBytes', 
                   'bool', 
                   [param('ns3::ServiceFlow *', 'serviceFlow'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols'), param('uint32_t', 'allocationSizeBytes')])
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::ServiceUnsolicitedGrants(ns3::SSRecord const * ssRecord, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceUnsolicitedGrants', 
                   'void', 
                   [param('ns3::SSRecord const *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::SetupServiceFlow(ns3::SSRecord * ssRecord, ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('SetupServiceFlow', 
                   'void', 
                   [param('ns3::SSRecord *', 'ssRecord'), param('ns3::ServiceFlow *', 'serviceFlow')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-mbqos.h: void ns3::UplinkSchedulerMBQoS::UplinkSchedWindowTimer() [member function]
    cls.add_method('UplinkSchedWindowTimer', 
                   'void', 
                   [])
    return

def register_Ns3UplinkSchedulerRtps_methods(root_module, cls):
    ## bs-uplink-scheduler-rtps.h: ns3::UplinkSchedulerRtps::UplinkSchedulerRtps(ns3::UplinkSchedulerRtps const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UplinkSchedulerRtps const &', 'arg0')])
    ## bs-uplink-scheduler-rtps.h: ns3::UplinkSchedulerRtps::UplinkSchedulerRtps() [constructor]
    cls.add_constructor([])
    ## bs-uplink-scheduler-rtps.h: ns3::UplinkSchedulerRtps::UplinkSchedulerRtps(ns3::Ptr<ns3::BaseStationNetDevice> bs) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::BaseStationNetDevice >', 'bs')])
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::AddUplinkAllocation(ns3::OfdmUlMapIe & ulMapIe, uint32_t const & allocationSize, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('AddUplinkAllocation', 
                   'void', 
                   [param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('uint32_t const &', 'allocationSize'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::AllocateInitialRangingInterval(uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('AllocateInitialRangingInterval', 
                   'void', 
                   [param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: uint32_t ns3::UplinkSchedulerRtps::CalculateAllocationStartTime() [member function]
    cls.add_method('CalculateAllocationStartTime', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::GetChannelDescriptorsToUpdate(bool & arg0, bool & arg1, bool & arg2, bool & arg3) [member function]
    cls.add_method('GetChannelDescriptorsToUpdate', 
                   'void', 
                   [param('bool &', 'arg0'), param('bool &', 'arg1'), param('bool &', 'arg2'), param('bool &', 'arg3')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: static ns3::TypeId ns3::UplinkSchedulerRtps::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bs-uplink-scheduler-rtps.h: std::list<ns3::OfdmUlMapIe, std::allocator<ns3::OfdmUlMapIe> > ns3::UplinkSchedulerRtps::GetUplinkAllocations() const [member function]
    cls.add_method('GetUplinkAllocations', 
                   'std::list< ns3::OfdmUlMapIe >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::InitOnce() [member function]
    cls.add_method('InitOnce', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::OnSetRequestedBandwidth(ns3::ServiceFlowRecord * sfr) [member function]
    cls.add_method('OnSetRequestedBandwidth', 
                   'void', 
                   [param('ns3::ServiceFlowRecord *', 'sfr')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::ProcessBandwidthRequest(ns3::BandwidthRequestHeader const & bwRequestHdr) [member function]
    cls.add_method('ProcessBandwidthRequest', 
                   'void', 
                   [param('ns3::BandwidthRequestHeader const &', 'bwRequestHdr')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::Schedule() [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::ServiceBandwidthRequests(ns3::SSRecord const * ssRecord, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceBandwidthRequests', 
                   'void', 
                   [param('ns3::SSRecord const *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: bool ns3::UplinkSchedulerRtps::ServiceBandwidthRequests(ns3::ServiceFlow * serviceFlow, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceBandwidthRequests', 
                   'bool', 
                   [param('ns3::ServiceFlow *', 'serviceFlow'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::ServiceUnsolicitedGrants(ns3::SSRecord const * ssRecord, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceUnsolicitedGrants', 
                   'void', 
                   [param('ns3::SSRecord const *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::SetupServiceFlow(ns3::SSRecord * ssRecord, ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('SetupServiceFlow', 
                   'void', 
                   [param('ns3::SSRecord *', 'ssRecord'), param('ns3::ServiceFlow *', 'serviceFlow')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-rtps.h: void ns3::UplinkSchedulerRtps::ULSchedulerRTPSConnection(uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ULSchedulerRTPSConnection', 
                   'void', 
                   [param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')])
    return

def register_Ns3UplinkSchedulerSimple_methods(root_module, cls):
    ## bs-uplink-scheduler-simple.h: ns3::UplinkSchedulerSimple::UplinkSchedulerSimple(ns3::UplinkSchedulerSimple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UplinkSchedulerSimple const &', 'arg0')])
    ## bs-uplink-scheduler-simple.h: ns3::UplinkSchedulerSimple::UplinkSchedulerSimple() [constructor]
    cls.add_constructor([])
    ## bs-uplink-scheduler-simple.h: ns3::UplinkSchedulerSimple::UplinkSchedulerSimple(ns3::Ptr<ns3::BaseStationNetDevice> bs) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::BaseStationNetDevice >', 'bs')])
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::AddUplinkAllocation(ns3::OfdmUlMapIe & ulMapIe, uint32_t const & allocationSize, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('AddUplinkAllocation', 
                   'void', 
                   [param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('uint32_t const &', 'allocationSize'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::AllocateInitialRangingInterval(uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('AllocateInitialRangingInterval', 
                   'void', 
                   [param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: uint32_t ns3::UplinkSchedulerSimple::CalculateAllocationStartTime() [member function]
    cls.add_method('CalculateAllocationStartTime', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::GetChannelDescriptorsToUpdate(bool & arg0, bool & arg1, bool & arg2, bool & arg3) [member function]
    cls.add_method('GetChannelDescriptorsToUpdate', 
                   'void', 
                   [param('bool &', 'arg0'), param('bool &', 'arg1'), param('bool &', 'arg2'), param('bool &', 'arg3')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: static ns3::TypeId ns3::UplinkSchedulerSimple::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bs-uplink-scheduler-simple.h: std::list<ns3::OfdmUlMapIe, std::allocator<ns3::OfdmUlMapIe> > ns3::UplinkSchedulerSimple::GetUplinkAllocations() const [member function]
    cls.add_method('GetUplinkAllocations', 
                   'std::list< ns3::OfdmUlMapIe >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::InitOnce() [member function]
    cls.add_method('InitOnce', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::OnSetRequestedBandwidth(ns3::ServiceFlowRecord * sfr) [member function]
    cls.add_method('OnSetRequestedBandwidth', 
                   'void', 
                   [param('ns3::ServiceFlowRecord *', 'sfr')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::ProcessBandwidthRequest(ns3::BandwidthRequestHeader const & bwRequestHdr) [member function]
    cls.add_method('ProcessBandwidthRequest', 
                   'void', 
                   [param('ns3::BandwidthRequestHeader const &', 'bwRequestHdr')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::Schedule() [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::ServiceBandwidthRequests(ns3::SSRecord const * ssRecord, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceBandwidthRequests', 
                   'void', 
                   [param('ns3::SSRecord const *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: bool ns3::UplinkSchedulerSimple::ServiceBandwidthRequests(ns3::ServiceFlow * serviceFlow, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceBandwidthRequests', 
                   'bool', 
                   [param('ns3::ServiceFlow *', 'serviceFlow'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::ServiceUnsolicitedGrants(ns3::SSRecord const * ssRecord, ns3::ServiceFlow::SchedulingType schedulingType, ns3::OfdmUlMapIe & ulMapIe, ns3::WimaxPhy::ModulationType const modulationType, uint32_t & symbolsToAllocation, uint32_t & availableSymbols) [member function]
    cls.add_method('ServiceUnsolicitedGrants', 
                   'void', 
                   [param('ns3::SSRecord const *', 'ssRecord'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType'), param('ns3::OfdmUlMapIe &', 'ulMapIe'), param('ns3::WimaxPhy::ModulationType const', 'modulationType'), param('uint32_t &', 'symbolsToAllocation'), param('uint32_t &', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-uplink-scheduler-simple.h: void ns3::UplinkSchedulerSimple::SetupServiceFlow(ns3::SSRecord * ssRecord, ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('SetupServiceFlow', 
                   'void', 
                   [param('ns3::SSRecord *', 'ssRecord'), param('ns3::ServiceFlow *', 'serviceFlow')], 
                   is_virtual=True)
    return

def register_Ns3WimaxConnection_methods(root_module, cls):
    ## wimax-connection.h: ns3::WimaxConnection::WimaxConnection(ns3::WimaxConnection const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WimaxConnection const &', 'arg0')])
    ## wimax-connection.h: ns3::WimaxConnection::WimaxConnection(ns3::Cid cid, ns3::Cid::Type type) [constructor]
    cls.add_constructor([param('ns3::Cid', 'cid'), param('ns3::Cid::Type', 'type')])
    ## wimax-connection.h: void ns3::WimaxConnection::ClearFragmentsQueue() [member function]
    cls.add_method('ClearFragmentsQueue', 
                   'void', 
                   [])
    ## wimax-connection.h: ns3::Ptr<ns3::Packet> ns3::WimaxConnection::Dequeue(ns3::MacHeaderType::HeaderType packetType=::ns3::MacHeaderType::HEADER_TYPE_GENERIC) [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType', default_value='::ns3::MacHeaderType::HEADER_TYPE_GENERIC')])
    ## wimax-connection.h: ns3::Ptr<ns3::Packet> ns3::WimaxConnection::Dequeue(ns3::MacHeaderType::HeaderType packetType, uint32_t availableByte) [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType'), param('uint32_t', 'availableByte')])
    ## wimax-connection.h: bool ns3::WimaxConnection::Enqueue(ns3::Ptr<ns3::Packet> packet, ns3::MacHeaderType const & hdrType, ns3::GenericMacHeader const & hdr) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::MacHeaderType const &', 'hdrType'), param('ns3::GenericMacHeader const &', 'hdr')])
    ## wimax-connection.h: void ns3::WimaxConnection::FragmentEnqueue(ns3::Ptr<ns3::Packet const> fragment) [member function]
    cls.add_method('FragmentEnqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'fragment')])
    ## wimax-connection.h: ns3::Cid ns3::WimaxConnection::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## wimax-connection.h: std::list<ns3::Ptr<ns3::Packet const>, std::allocator<ns3::Ptr<ns3::Packet const> > > const ns3::WimaxConnection::GetFragmentsQueue() const [member function]
    cls.add_method('GetFragmentsQueue', 
                   'std::list< ns3::Ptr< ns3::Packet const > > const', 
                   [], 
                   is_const=True)
    ## wimax-connection.h: ns3::Ptr<ns3::WimaxMacQueue> ns3::WimaxConnection::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::WimaxMacQueue >', 
                   [], 
                   is_const=True)
    ## wimax-connection.h: uint8_t ns3::WimaxConnection::GetSchedulingType() const [member function]
    cls.add_method('GetSchedulingType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-connection.h: ns3::ServiceFlow * ns3::WimaxConnection::GetServiceFlow() const [member function]
    cls.add_method('GetServiceFlow', 
                   'ns3::ServiceFlow *', 
                   [], 
                   is_const=True)
    ## wimax-connection.h: ns3::Cid::Type ns3::WimaxConnection::GetType() const [member function]
    cls.add_method('GetType', 
                   'ns3::Cid::Type', 
                   [], 
                   is_const=True)
    ## wimax-connection.h: static ns3::TypeId ns3::WimaxConnection::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-connection.h: std::string ns3::WimaxConnection::GetTypeStr() const [member function]
    cls.add_method('GetTypeStr', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## wimax-connection.h: bool ns3::WimaxConnection::HasPackets() const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wimax-connection.h: bool ns3::WimaxConnection::HasPackets(ns3::MacHeaderType::HeaderType packetType) const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')], 
                   is_const=True)
    ## wimax-connection.h: void ns3::WimaxConnection::SetServiceFlow(ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('SetServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow *', 'serviceFlow')])
    ## wimax-connection.h: void ns3::WimaxConnection::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3WimaxMacQueue_methods(root_module, cls):
    ## wimax-mac-queue.h: ns3::WimaxMacQueue::WimaxMacQueue(ns3::WimaxMacQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WimaxMacQueue const &', 'arg0')])
    ## wimax-mac-queue.h: ns3::WimaxMacQueue::WimaxMacQueue() [constructor]
    cls.add_constructor([])
    ## wimax-mac-queue.h: ns3::WimaxMacQueue::WimaxMacQueue(uint32_t maxSize) [constructor]
    cls.add_constructor([param('uint32_t', 'maxSize')])
    ## wimax-mac-queue.h: bool ns3::WimaxMacQueue::CheckForFragmentation(ns3::MacHeaderType::HeaderType packetType) [member function]
    cls.add_method('CheckForFragmentation', 
                   'bool', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')])
    ## wimax-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::WimaxMacQueue::Dequeue(ns3::MacHeaderType::HeaderType packetType) [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')])
    ## wimax-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::WimaxMacQueue::Dequeue(ns3::MacHeaderType::HeaderType packetType, uint32_t availableByte) [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType'), param('uint32_t', 'availableByte')])
    ## wimax-mac-queue.h: bool ns3::WimaxMacQueue::Enqueue(ns3::Ptr<ns3::Packet> packet, ns3::MacHeaderType const & hdrType, ns3::GenericMacHeader const & hdr) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::MacHeaderType const &', 'hdrType'), param('ns3::GenericMacHeader const &', 'hdr')])
    ## wimax-mac-queue.h: uint32_t ns3::WimaxMacQueue::GetFirstPacketHdrSize(ns3::MacHeaderType::HeaderType packetType) [member function]
    cls.add_method('GetFirstPacketHdrSize', 
                   'uint32_t', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')])
    ## wimax-mac-queue.h: uint32_t ns3::WimaxMacQueue::GetFirstPacketPayloadSize(ns3::MacHeaderType::HeaderType packetType) [member function]
    cls.add_method('GetFirstPacketPayloadSize', 
                   'uint32_t', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')])
    ## wimax-mac-queue.h: uint32_t ns3::WimaxMacQueue::GetFirstPacketRequiredByte(ns3::MacHeaderType::HeaderType packetType) [member function]
    cls.add_method('GetFirstPacketRequiredByte', 
                   'uint32_t', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')])
    ## wimax-mac-queue.h: uint32_t ns3::WimaxMacQueue::GetMaxSize() const [member function]
    cls.add_method('GetMaxSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-queue.h: uint32_t ns3::WimaxMacQueue::GetNBytes() const [member function]
    cls.add_method('GetNBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-queue.h: std::deque<ns3::WimaxMacQueue::QueueElement, std::allocator<ns3::WimaxMacQueue::QueueElement> > const & ns3::WimaxMacQueue::GetPacketQueue() const [member function]
    cls.add_method('GetPacketQueue', 
                   'std::deque< ns3::WimaxMacQueue::QueueElement > const &', 
                   [], 
                   is_const=True)
    ## wimax-mac-queue.h: uint32_t ns3::WimaxMacQueue::GetQueueLengthWithMACOverhead() [member function]
    cls.add_method('GetQueueLengthWithMACOverhead', 
                   'uint32_t', 
                   [])
    ## wimax-mac-queue.h: uint32_t ns3::WimaxMacQueue::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-queue.h: static ns3::TypeId ns3::WimaxMacQueue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-mac-queue.h: bool ns3::WimaxMacQueue::IsEmpty() const [member function]
    cls.add_method('IsEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wimax-mac-queue.h: bool ns3::WimaxMacQueue::IsEmpty(ns3::MacHeaderType::HeaderType packetType) const [member function]
    cls.add_method('IsEmpty', 
                   'bool', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')], 
                   is_const=True)
    ## wimax-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::WimaxMacQueue::Peek(ns3::GenericMacHeader & hdr) const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::GenericMacHeader &', 'hdr')], 
                   is_const=True)
    ## wimax-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::WimaxMacQueue::Peek(ns3::GenericMacHeader & hdr, ns3::Time & timeStamp) const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::GenericMacHeader &', 'hdr'), param('ns3::Time &', 'timeStamp')], 
                   is_const=True)
    ## wimax-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::WimaxMacQueue::Peek(ns3::MacHeaderType::HeaderType packetType) const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')], 
                   is_const=True)
    ## wimax-mac-queue.h: ns3::Ptr<ns3::Packet> ns3::WimaxMacQueue::Peek(ns3::MacHeaderType::HeaderType packetType, ns3::Time & timeStamp) const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType'), param('ns3::Time &', 'timeStamp')], 
                   is_const=True)
    ## wimax-mac-queue.h: void ns3::WimaxMacQueue::SetFragmentNumber(ns3::MacHeaderType::HeaderType packetType) [member function]
    cls.add_method('SetFragmentNumber', 
                   'void', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')])
    ## wimax-mac-queue.h: void ns3::WimaxMacQueue::SetFragmentOffset(ns3::MacHeaderType::HeaderType packetType, uint32_t offset) [member function]
    cls.add_method('SetFragmentOffset', 
                   'void', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType'), param('uint32_t', 'offset')])
    ## wimax-mac-queue.h: void ns3::WimaxMacQueue::SetFragmentation(ns3::MacHeaderType::HeaderType packetType) [member function]
    cls.add_method('SetFragmentation', 
                   'void', 
                   [param('ns3::MacHeaderType::HeaderType', 'packetType')])
    ## wimax-mac-queue.h: void ns3::WimaxMacQueue::SetMaxSize(uint32_t maxSize) [member function]
    cls.add_method('SetMaxSize', 
                   'void', 
                   [param('uint32_t', 'maxSize')])
    return

def register_Ns3WimaxMacToMacHeader_methods(root_module, cls):
    ## wimax-mac-to-mac-header.h: ns3::WimaxMacToMacHeader::WimaxMacToMacHeader(ns3::WimaxMacToMacHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WimaxMacToMacHeader const &', 'arg0')])
    ## wimax-mac-to-mac-header.h: ns3::WimaxMacToMacHeader::WimaxMacToMacHeader() [constructor]
    cls.add_constructor([])
    ## wimax-mac-to-mac-header.h: ns3::WimaxMacToMacHeader::WimaxMacToMacHeader(uint32_t len) [constructor]
    cls.add_constructor([param('uint32_t', 'len')])
    ## wimax-mac-to-mac-header.h: uint32_t ns3::WimaxMacToMacHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wimax-mac-to-mac-header.h: ns3::TypeId ns3::WimaxMacToMacHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-to-mac-header.h: uint32_t ns3::WimaxMacToMacHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-to-mac-header.h: uint8_t ns3::WimaxMacToMacHeader::GetSizeOfLen() const [member function]
    cls.add_method('GetSizeOfLen', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-to-mac-header.h: static ns3::TypeId ns3::WimaxMacToMacHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-mac-to-mac-header.h: void ns3::WimaxMacToMacHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-to-mac-header.h: void ns3::WimaxMacToMacHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3WimaxPhy_methods(root_module, cls):
    ## wimax-phy.h: ns3::WimaxPhy::WimaxPhy(ns3::WimaxPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WimaxPhy const &', 'arg0')])
    ## wimax-phy.h: ns3::WimaxPhy::WimaxPhy() [constructor]
    cls.add_constructor([])
    ## wimax-phy.h: void ns3::WimaxPhy::Attach(ns3::Ptr<ns3::WimaxChannel> channel) [member function]
    cls.add_method('Attach', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxChannel >', 'channel')])
    ## wimax-phy.h: void ns3::WimaxPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## wimax-phy.h: ns3::Ptr<ns3::WimaxChannel> ns3::WimaxPhy::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::WimaxChannel >', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint32_t ns3::WimaxPhy::GetChannelBandwidth() const [member function]
    cls.add_method('GetChannelBandwidth', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::EventId ns3::WimaxPhy::GetChnlSrchTimeoutEvent() const [member function]
    cls.add_method('GetChnlSrchTimeoutEvent', 
                   'ns3::EventId', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint32_t ns3::WimaxPhy::GetDataRate(ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('GetDataRate', 
                   'uint32_t', 
                   [param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_const=True)
    ## wimax-phy.h: ns3::Ptr<ns3::NetDevice> ns3::WimaxPhy::GetDevice() const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::Time ns3::WimaxPhy::GetFrameDuration() const [member function]
    cls.add_method('GetFrameDuration', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::Time ns3::WimaxPhy::GetFrameDuration(uint8_t frameDurationCode) const [member function]
    cls.add_method('GetFrameDuration', 
                   'ns3::Time', 
                   [param('uint8_t', 'frameDurationCode')], 
                   is_const=True)
    ## wimax-phy.h: uint8_t ns3::WimaxPhy::GetFrameDurationCode() const [member function]
    cls.add_method('GetFrameDurationCode', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::Time ns3::WimaxPhy::GetFrameDurationSec() const [member function]
    cls.add_method('GetFrameDurationSec', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint32_t ns3::WimaxPhy::GetFrequency() const [member function]
    cls.add_method('GetFrequency', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: double ns3::WimaxPhy::GetGValue() const [member function]
    cls.add_method('GetGValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::Ptr<ns3::Object> ns3::WimaxPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## wimax-phy.h: uint16_t ns3::WimaxPhy::GetNfft() const [member function]
    cls.add_method('GetNfft', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint64_t ns3::WimaxPhy::GetNrBytes(uint32_t symbols, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('GetNrBytes', 
                   'uint64_t', 
                   [param('uint32_t', 'symbols'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_const=True)
    ## wimax-phy.h: uint8_t ns3::WimaxPhy::GetNrCarriers() const [member function]
    cls.add_method('GetNrCarriers', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint64_t ns3::WimaxPhy::GetNrSymbols(uint32_t size, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('GetNrSymbols', 
                   'uint64_t', 
                   [param('uint32_t', 'size'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_const=True)
    ## wimax-phy.h: ns3::WimaxPhy::PhyType ns3::WimaxPhy::GetPhyType() const [member function]
    cls.add_method('GetPhyType', 
                   'ns3::WimaxPhy::PhyType', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wimax-phy.h: ns3::Time ns3::WimaxPhy::GetPsDuration() const [member function]
    cls.add_method('GetPsDuration', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint16_t ns3::WimaxPhy::GetPsPerFrame() const [member function]
    cls.add_method('GetPsPerFrame', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint16_t ns3::WimaxPhy::GetPsPerSymbol() const [member function]
    cls.add_method('GetPsPerSymbol', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::Callback<void, ns3::Ptr<ns3::PacketBurst const>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::WimaxPhy::GetReceiveCallback() const [member function]
    cls.add_method('GetReceiveCallback', 
                   'ns3::Callback< void, ns3::Ptr< ns3::PacketBurst const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint16_t ns3::WimaxPhy::GetRtg() const [member function]
    cls.add_method('GetRtg', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint64_t ns3::WimaxPhy::GetRxFrequency() const [member function]
    cls.add_method('GetRxFrequency', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: double ns3::WimaxPhy::GetSamplingFactor() const [member function]
    cls.add_method('GetSamplingFactor', 
                   'double', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: double ns3::WimaxPhy::GetSamplingFrequency() const [member function]
    cls.add_method('GetSamplingFrequency', 
                   'double', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint64_t ns3::WimaxPhy::GetScanningFrequency() const [member function]
    cls.add_method('GetScanningFrequency', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::WimaxPhy::PhyState ns3::WimaxPhy::GetState() const [member function]
    cls.add_method('GetState', 
                   'ns3::WimaxPhy::PhyState', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::Time ns3::WimaxPhy::GetSymbolDuration() const [member function]
    cls.add_method('GetSymbolDuration', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint32_t ns3::WimaxPhy::GetSymbolsPerFrame() const [member function]
    cls.add_method('GetSymbolsPerFrame', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: ns3::Time ns3::WimaxPhy::GetTransmissionTime(uint32_t size, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('GetTransmissionTime', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_const=True)
    ## wimax-phy.h: uint16_t ns3::WimaxPhy::GetTtg() const [member function]
    cls.add_method('GetTtg', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: uint64_t ns3::WimaxPhy::GetTxFrequency() const [member function]
    cls.add_method('GetTxFrequency', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: static ns3::TypeId ns3::WimaxPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-phy.h: bool ns3::WimaxPhy::IsDuplex() const [member function]
    cls.add_method('IsDuplex', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: void ns3::WimaxPhy::Send(ns3::SendParams * params) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::SendParams *', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wimax-phy.h: void ns3::WimaxPhy::SetChannelBandwidth(uint32_t channelBandwidth) [member function]
    cls.add_method('SetChannelBandwidth', 
                   'void', 
                   [param('uint32_t', 'channelBandwidth')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetDataRates() [member function]
    cls.add_method('SetDataRates', 
                   'void', 
                   [])
    ## wimax-phy.h: void ns3::WimaxPhy::SetDevice(ns3::Ptr<ns3::WimaxNetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxNetDevice >', 'device')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetDuplex(uint64_t rxFrequency, uint64_t txFrequency) [member function]
    cls.add_method('SetDuplex', 
                   'void', 
                   [param('uint64_t', 'rxFrequency'), param('uint64_t', 'txFrequency')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetFrameDuration(ns3::Time frameDuration) [member function]
    cls.add_method('SetFrameDuration', 
                   'void', 
                   [param('ns3::Time', 'frameDuration')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetFrequency(uint32_t frequency) [member function]
    cls.add_method('SetFrequency', 
                   'void', 
                   [param('uint32_t', 'frequency')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetMobility(ns3::Ptr<ns3::Object> mobility) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'mobility')], 
                   is_virtual=True)
    ## wimax-phy.h: void ns3::WimaxPhy::SetNrCarriers(uint8_t nrCarriers) [member function]
    cls.add_method('SetNrCarriers', 
                   'void', 
                   [param('uint8_t', 'nrCarriers')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetPhyParameters() [member function]
    cls.add_method('SetPhyParameters', 
                   'void', 
                   [])
    ## wimax-phy.h: void ns3::WimaxPhy::SetPsDuration(ns3::Time psDuration) [member function]
    cls.add_method('SetPsDuration', 
                   'void', 
                   [param('ns3::Time', 'psDuration')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetPsPerFrame(uint16_t psPerFrame) [member function]
    cls.add_method('SetPsPerFrame', 
                   'void', 
                   [param('uint16_t', 'psPerFrame')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetPsPerSymbol(uint16_t psPerSymbol) [member function]
    cls.add_method('SetPsPerSymbol', 
                   'void', 
                   [param('uint16_t', 'psPerSymbol')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetReceiveCallback(ns3::Callback<void, ns3::Ptr<ns3::PacketBurst const>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::PacketBurst const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetScanningCallback() const [member function]
    cls.add_method('SetScanningCallback', 
                   'void', 
                   [], 
                   is_const=True)
    ## wimax-phy.h: void ns3::WimaxPhy::SetSimplex(uint64_t frequency) [member function]
    cls.add_method('SetSimplex', 
                   'void', 
                   [param('uint64_t', 'frequency')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetState(ns3::WimaxPhy::PhyState state) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::WimaxPhy::PhyState', 'state')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetSymbolDuration(ns3::Time symbolDuration) [member function]
    cls.add_method('SetSymbolDuration', 
                   'void', 
                   [param('ns3::Time', 'symbolDuration')])
    ## wimax-phy.h: void ns3::WimaxPhy::SetSymbolsPerFrame(uint32_t symbolsPerFrame) [member function]
    cls.add_method('SetSymbolsPerFrame', 
                   'void', 
                   [param('uint32_t', 'symbolsPerFrame')])
    ## wimax-phy.h: void ns3::WimaxPhy::StartScanning(uint64_t frequency, ns3::Time timeout, ns3::Callback<void, bool, unsigned long long, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('StartScanning', 
                   'void', 
                   [param('uint64_t', 'frequency'), param('ns3::Time', 'timeout'), param('ns3::Callback< void, bool, unsigned long long, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## wimax-phy.h: void ns3::WimaxPhy::DoAttach(ns3::Ptr<ns3::WimaxChannel> channel) [member function]
    cls.add_method('DoAttach', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxChannel >', 'channel')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: uint32_t ns3::WimaxPhy::DoGetDataRate(ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('DoGetDataRate', 
                   'uint32_t', 
                   [param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: ns3::Time ns3::WimaxPhy::DoGetFrameDuration(uint8_t frameDurationCode) const [member function]
    cls.add_method('DoGetFrameDuration', 
                   'ns3::Time', 
                   [param('uint8_t', 'frameDurationCode')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: uint8_t ns3::WimaxPhy::DoGetFrameDurationCode() const [member function]
    cls.add_method('DoGetFrameDurationCode', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: double ns3::WimaxPhy::DoGetGValue() const [member function]
    cls.add_method('DoGetGValue', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: uint16_t ns3::WimaxPhy::DoGetNfft() const [member function]
    cls.add_method('DoGetNfft', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: uint64_t ns3::WimaxPhy::DoGetNrBytes(uint32_t symbols, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('DoGetNrBytes', 
                   'uint64_t', 
                   [param('uint32_t', 'symbols'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: uint64_t ns3::WimaxPhy::DoGetNrSymbols(uint32_t size, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('DoGetNrSymbols', 
                   'uint64_t', 
                   [param('uint32_t', 'size'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: uint16_t ns3::WimaxPhy::DoGetRtg() const [member function]
    cls.add_method('DoGetRtg', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: double ns3::WimaxPhy::DoGetSamplingFactor() const [member function]
    cls.add_method('DoGetSamplingFactor', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: double ns3::WimaxPhy::DoGetSamplingFrequency() const [member function]
    cls.add_method('DoGetSamplingFrequency', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: ns3::Time ns3::WimaxPhy::DoGetTransmissionTime(uint32_t size, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('DoGetTransmissionTime', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: uint16_t ns3::WimaxPhy::DoGetTtg() const [member function]
    cls.add_method('DoGetTtg', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: void ns3::WimaxPhy::DoSetDataRates() [member function]
    cls.add_method('DoSetDataRates', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wimax-phy.h: void ns3::WimaxPhy::DoSetPhyParameters() [member function]
    cls.add_method('DoSetPhyParameters', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3BSScheduler_methods(root_module, cls):
    ## bs-scheduler.h: ns3::BSScheduler::BSScheduler(ns3::BSScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BSScheduler const &', 'arg0')])
    ## bs-scheduler.h: ns3::BSScheduler::BSScheduler() [constructor]
    cls.add_constructor([])
    ## bs-scheduler.h: ns3::BSScheduler::BSScheduler(ns3::Ptr<ns3::BaseStationNetDevice> bs) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::BaseStationNetDevice >', 'bs')])
    ## bs-scheduler.h: void ns3::BSScheduler::AddDownlinkBurst(ns3::Ptr<const ns3::WimaxConnection> connection, uint8_t diuc, ns3::WimaxPhy::ModulationType modulationType, ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('AddDownlinkBurst', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxConnection const >', 'connection'), param('uint8_t', 'diuc'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('ns3::Ptr< ns3::PacketBurst >', 'burst')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-scheduler.h: bool ns3::BSScheduler::CheckForFragmentation(ns3::Ptr<ns3::WimaxConnection> connection, int availableSymbols, ns3::WimaxPhy::ModulationType modulationType) [member function]
    cls.add_method('CheckForFragmentation', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection >', 'connection'), param('int', 'availableSymbols'), param('ns3::WimaxPhy::ModulationType', 'modulationType')])
    ## bs-scheduler.h: ns3::Ptr<ns3::PacketBurst> ns3::BSScheduler::CreateUgsBurst(ns3::ServiceFlow * serviceFlow, ns3::WimaxPhy::ModulationType modulationType, uint32_t availableSymbols) [member function]
    cls.add_method('CreateUgsBurst', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [param('ns3::ServiceFlow *', 'serviceFlow'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('uint32_t', 'availableSymbols')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-scheduler.h: ns3::Ptr<ns3::BaseStationNetDevice> ns3::BSScheduler::GetBs() [member function]
    cls.add_method('GetBs', 
                   'ns3::Ptr< ns3::BaseStationNetDevice >', 
                   [], 
                   is_virtual=True)
    ## bs-scheduler.h: std::list<std::pair<ns3::OfdmDlMapIe*, ns3::Ptr<ns3::PacketBurst> >,std::allocator<std::pair<ns3::OfdmDlMapIe*, ns3::Ptr<ns3::PacketBurst> > > > * ns3::BSScheduler::GetDownlinkBursts() const [member function]
    cls.add_method('GetDownlinkBursts', 
                   'std::list< std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > > > *', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## bs-scheduler.h: static ns3::TypeId ns3::BSScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bs-scheduler.h: void ns3::BSScheduler::Schedule() [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-scheduler.h: bool ns3::BSScheduler::SelectConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')], 
                   is_pure_virtual=True, is_virtual=True)
    ## bs-scheduler.h: void ns3::BSScheduler::SetBs(ns3::Ptr<ns3::BaseStationNetDevice> bs) [member function]
    cls.add_method('SetBs', 
                   'void', 
                   [param('ns3::Ptr< ns3::BaseStationNetDevice >', 'bs')], 
                   is_virtual=True)
    return

def register_Ns3BSSchedulerRtps_methods(root_module, cls):
    ## bs-scheduler-rtps.h: ns3::BSSchedulerRtps::BSSchedulerRtps(ns3::BSSchedulerRtps const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BSSchedulerRtps const &', 'arg0')])
    ## bs-scheduler-rtps.h: ns3::BSSchedulerRtps::BSSchedulerRtps() [constructor]
    cls.add_constructor([])
    ## bs-scheduler-rtps.h: ns3::BSSchedulerRtps::BSSchedulerRtps(ns3::Ptr<ns3::BaseStationNetDevice> bs) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::BaseStationNetDevice >', 'bs')])
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::AddDownlinkBurst(ns3::Ptr<const ns3::WimaxConnection> connection, uint8_t diuc, ns3::WimaxPhy::ModulationType modulationType, ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('AddDownlinkBurst', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxConnection const >', 'connection'), param('uint8_t', 'diuc'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('ns3::Ptr< ns3::PacketBurst >', 'burst')], 
                   is_virtual=True)
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::BSSchedulerBEConnection(uint32_t & availableSymbols) [member function]
    cls.add_method('BSSchedulerBEConnection', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::BSSchedulerBasicConnection(uint32_t & availableSymbols) [member function]
    cls.add_method('BSSchedulerBasicConnection', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::BSSchedulerBroadcastConnection(uint32_t & availableSymbols) [member function]
    cls.add_method('BSSchedulerBroadcastConnection', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::BSSchedulerInitialRangingConnection(uint32_t & availableSymbols) [member function]
    cls.add_method('BSSchedulerInitialRangingConnection', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::BSSchedulerNRTPSConnection(uint32_t & availableSymbols) [member function]
    cls.add_method('BSSchedulerNRTPSConnection', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::BSSchedulerPrimaryConnection(uint32_t & availableSymbols) [member function]
    cls.add_method('BSSchedulerPrimaryConnection', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::BSSchedulerRTPSConnection(uint32_t & availableSymbols) [member function]
    cls.add_method('BSSchedulerRTPSConnection', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::BSSchedulerUGSConnection(uint32_t & availableSymbols) [member function]
    cls.add_method('BSSchedulerUGSConnection', 
                   'void', 
                   [param('uint32_t &', 'availableSymbols')])
    ## bs-scheduler-rtps.h: ns3::Ptr<ns3::PacketBurst> ns3::BSSchedulerRtps::CreateUgsBurst(ns3::ServiceFlow * serviceFlow, ns3::WimaxPhy::ModulationType modulationType, uint32_t availableSymbols) [member function]
    cls.add_method('CreateUgsBurst', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [param('ns3::ServiceFlow *', 'serviceFlow'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('uint32_t', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-scheduler-rtps.h: std::list<std::pair<ns3::OfdmDlMapIe*, ns3::Ptr<ns3::PacketBurst> >,std::allocator<std::pair<ns3::OfdmDlMapIe*, ns3::Ptr<ns3::PacketBurst> > > > * ns3::BSSchedulerRtps::GetDownlinkBursts() const [member function]
    cls.add_method('GetDownlinkBursts', 
                   'std::list< std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > > > *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-scheduler-rtps.h: static ns3::TypeId ns3::BSSchedulerRtps::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bs-scheduler-rtps.h: void ns3::BSSchedulerRtps::Schedule() [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-scheduler-rtps.h: bool ns3::BSSchedulerRtps::SelectBEConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectBEConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')])
    ## bs-scheduler-rtps.h: bool ns3::BSSchedulerRtps::SelectConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')], 
                   is_virtual=True)
    ## bs-scheduler-rtps.h: bool ns3::BSSchedulerRtps::SelectIRandBCConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectIRandBCConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')])
    ## bs-scheduler-rtps.h: bool ns3::BSSchedulerRtps::SelectMenagementConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectMenagementConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')])
    ## bs-scheduler-rtps.h: bool ns3::BSSchedulerRtps::SelectNRTPSConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectNRTPSConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')])
    ## bs-scheduler-rtps.h: bool ns3::BSSchedulerRtps::SelectRTPSConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectRTPSConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')])
    ## bs-scheduler-rtps.h: bool ns3::BSSchedulerRtps::SelectUGSConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectUGSConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')])
    return

def register_Ns3BSSchedulerSimple_methods(root_module, cls):
    ## bs-scheduler-simple.h: ns3::BSSchedulerSimple::BSSchedulerSimple(ns3::BSSchedulerSimple const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BSSchedulerSimple const &', 'arg0')])
    ## bs-scheduler-simple.h: ns3::BSSchedulerSimple::BSSchedulerSimple() [constructor]
    cls.add_constructor([])
    ## bs-scheduler-simple.h: ns3::BSSchedulerSimple::BSSchedulerSimple(ns3::Ptr<ns3::BaseStationNetDevice> bs) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::BaseStationNetDevice >', 'bs')])
    ## bs-scheduler-simple.h: void ns3::BSSchedulerSimple::AddDownlinkBurst(ns3::Ptr<const ns3::WimaxConnection> connection, uint8_t diuc, ns3::WimaxPhy::ModulationType modulationType, ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('AddDownlinkBurst', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxConnection const >', 'connection'), param('uint8_t', 'diuc'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('ns3::Ptr< ns3::PacketBurst >', 'burst')], 
                   is_virtual=True)
    ## bs-scheduler-simple.h: ns3::Ptr<ns3::PacketBurst> ns3::BSSchedulerSimple::CreateUgsBurst(ns3::ServiceFlow * serviceFlow, ns3::WimaxPhy::ModulationType modulationType, uint32_t availableSymbols) [member function]
    cls.add_method('CreateUgsBurst', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [param('ns3::ServiceFlow *', 'serviceFlow'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('uint32_t', 'availableSymbols')], 
                   is_virtual=True)
    ## bs-scheduler-simple.h: std::list<std::pair<ns3::OfdmDlMapIe*, ns3::Ptr<ns3::PacketBurst> >,std::allocator<std::pair<ns3::OfdmDlMapIe*, ns3::Ptr<ns3::PacketBurst> > > > * ns3::BSSchedulerSimple::GetDownlinkBursts() const [member function]
    cls.add_method('GetDownlinkBursts', 
                   'std::list< std::pair< ns3::OfdmDlMapIe *, ns3::Ptr< ns3::PacketBurst > > > *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bs-scheduler-simple.h: static ns3::TypeId ns3::BSSchedulerSimple::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bs-scheduler-simple.h: void ns3::BSSchedulerSimple::Schedule() [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-scheduler-simple.h: bool ns3::BSSchedulerSimple::SelectConnection(ns3::Ptr<ns3::WimaxConnection> & connection) [member function]
    cls.add_method('SelectConnection', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WimaxConnection > &', 'connection')], 
                   is_virtual=True)
    return

def register_Ns3BandwidthRequestHeader_methods(root_module, cls):
    ## wimax-mac-header.h: ns3::BandwidthRequestHeader::BandwidthRequestHeader(ns3::BandwidthRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BandwidthRequestHeader const &', 'arg0')])
    ## wimax-mac-header.h: ns3::BandwidthRequestHeader::BandwidthRequestHeader() [constructor]
    cls.add_constructor([])
    ## wimax-mac-header.h: uint32_t ns3::BandwidthRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wimax-mac-header.h: uint32_t ns3::BandwidthRequestHeader::GetBr() const [member function]
    cls.add_method('GetBr', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: ns3::Cid ns3::BandwidthRequestHeader::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::BandwidthRequestHeader::GetEc() const [member function]
    cls.add_method('GetEc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::BandwidthRequestHeader::GetHcs() const [member function]
    cls.add_method('GetHcs', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::BandwidthRequestHeader::GetHt() const [member function]
    cls.add_method('GetHt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: ns3::TypeId ns3::BandwidthRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: std::string ns3::BandwidthRequestHeader::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint32_t ns3::BandwidthRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: uint8_t ns3::BandwidthRequestHeader::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: static ns3::TypeId ns3::BandwidthRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-mac-header.h: void ns3::BandwidthRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::BandwidthRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::BandwidthRequestHeader::SetBr(uint32_t br) [member function]
    cls.add_method('SetBr', 
                   'void', 
                   [param('uint32_t', 'br')])
    ## wimax-mac-header.h: void ns3::BandwidthRequestHeader::SetCid(ns3::Cid cid) [member function]
    cls.add_method('SetCid', 
                   'void', 
                   [param('ns3::Cid', 'cid')])
    ## wimax-mac-header.h: void ns3::BandwidthRequestHeader::SetEc(uint8_t ec) [member function]
    cls.add_method('SetEc', 
                   'void', 
                   [param('uint8_t', 'ec')])
    ## wimax-mac-header.h: void ns3::BandwidthRequestHeader::SetHcs(uint8_t hcs) [member function]
    cls.add_method('SetHcs', 
                   'void', 
                   [param('uint8_t', 'hcs')])
    ## wimax-mac-header.h: void ns3::BandwidthRequestHeader::SetHt(uint8_t HT) [member function]
    cls.add_method('SetHt', 
                   'void', 
                   [param('uint8_t', 'HT')])
    ## wimax-mac-header.h: void ns3::BandwidthRequestHeader::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## wimax-mac-header.h: bool ns3::BandwidthRequestHeader::check_hcs() const [member function]
    cls.add_method('check_hcs', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3BsServiceFlowManager_methods(root_module, cls):
    ## bs-service-flow-manager.h: ns3::BsServiceFlowManager::BsServiceFlowManager(ns3::BsServiceFlowManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BsServiceFlowManager const &', 'arg0')])
    ## bs-service-flow-manager.h: ns3::BsServiceFlowManager::BsServiceFlowManager(ns3::Ptr<ns3::BaseStationNetDevice> device) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::BaseStationNetDevice >', 'device')])
    ## bs-service-flow-manager.h: void ns3::BsServiceFlowManager::AddMulticastServiceFlow(ns3::ServiceFlow sf, ns3::WimaxPhy::ModulationType modulation) [member function]
    cls.add_method('AddMulticastServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow', 'sf'), param('ns3::WimaxPhy::ModulationType', 'modulation')])
    ## bs-service-flow-manager.h: void ns3::BsServiceFlowManager::AddServiceFlow(ns3::ServiceFlow * serviceFlow) [member function]
    cls.add_method('AddServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow *', 'serviceFlow')])
    ## bs-service-flow-manager.h: void ns3::BsServiceFlowManager::AllocateServiceFlows(ns3::DsaReq const & dsaReq, ns3::Cid cid) [member function]
    cls.add_method('AllocateServiceFlows', 
                   'void', 
                   [param('ns3::DsaReq const &', 'dsaReq'), param('ns3::Cid', 'cid')])
    ## bs-service-flow-manager.h: void ns3::BsServiceFlowManager::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-service-flow-manager.h: ns3::EventId ns3::BsServiceFlowManager::GetDsaAckTimeoutEvent() const [member function]
    cls.add_method('GetDsaAckTimeoutEvent', 
                   'ns3::EventId', 
                   [], 
                   is_const=True)
    ## bs-service-flow-manager.h: ns3::ServiceFlow * ns3::BsServiceFlowManager::GetServiceFlow(uint32_t sfid) const [member function]
    cls.add_method('GetServiceFlow', 
                   'ns3::ServiceFlow *', 
                   [param('uint32_t', 'sfid')], 
                   is_const=True)
    ## bs-service-flow-manager.h: ns3::ServiceFlow * ns3::BsServiceFlowManager::GetServiceFlow(ns3::Cid cid) const [member function]
    cls.add_method('GetServiceFlow', 
                   'ns3::ServiceFlow *', 
                   [param('ns3::Cid', 'cid')], 
                   is_const=True)
    ## bs-service-flow-manager.h: std::vector<ns3::ServiceFlow*,std::allocator<ns3::ServiceFlow*> > ns3::BsServiceFlowManager::GetServiceFlows(ns3::ServiceFlow::SchedulingType schedulingType) const [member function]
    cls.add_method('GetServiceFlows', 
                   'std::vector< ns3::ServiceFlow * >', 
                   [param('ns3::ServiceFlow::SchedulingType', 'schedulingType')], 
                   is_const=True)
    ## bs-service-flow-manager.h: void ns3::BsServiceFlowManager::ProcessDsaAck(ns3::DsaAck const & dsaAck, ns3::Cid cid) [member function]
    cls.add_method('ProcessDsaAck', 
                   'void', 
                   [param('ns3::DsaAck const &', 'dsaAck'), param('ns3::Cid', 'cid')])
    ## bs-service-flow-manager.h: ns3::ServiceFlow * ns3::BsServiceFlowManager::ProcessDsaReq(ns3::DsaReq const & dsaReq, ns3::Cid cid) [member function]
    cls.add_method('ProcessDsaReq', 
                   'ns3::ServiceFlow *', 
                   [param('ns3::DsaReq const &', 'dsaReq'), param('ns3::Cid', 'cid')])
    ## bs-service-flow-manager.h: void ns3::BsServiceFlowManager::SetMaxDsaRspRetries(uint8_t maxDsaRspRetries) [member function]
    cls.add_method('SetMaxDsaRspRetries', 
                   'void', 
                   [param('uint8_t', 'maxDsaRspRetries')])
    return

def register_Ns3ConnectionManager_methods(root_module, cls):
    ## connection-manager.h: ns3::ConnectionManager::ConnectionManager(ns3::ConnectionManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConnectionManager const &', 'arg0')])
    ## connection-manager.h: ns3::ConnectionManager::ConnectionManager() [constructor]
    cls.add_constructor([])
    ## connection-manager.h: void ns3::ConnectionManager::AddConnection(ns3::Ptr<ns3::WimaxConnection> connection, ns3::Cid::Type type) [member function]
    cls.add_method('AddConnection', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxConnection >', 'connection'), param('ns3::Cid::Type', 'type')])
    ## connection-manager.h: void ns3::ConnectionManager::AllocateManagementConnections(ns3::SSRecord * ssRecord, ns3::RngRsp * rngrsp) [member function]
    cls.add_method('AllocateManagementConnections', 
                   'void', 
                   [param('ns3::SSRecord *', 'ssRecord'), param('ns3::RngRsp *', 'rngrsp')])
    ## connection-manager.h: ns3::Ptr<ns3::WimaxConnection> ns3::ConnectionManager::CreateConnection(ns3::Cid::Type type) [member function]
    cls.add_method('CreateConnection', 
                   'ns3::Ptr< ns3::WimaxConnection >', 
                   [param('ns3::Cid::Type', 'type')])
    ## connection-manager.h: void ns3::ConnectionManager::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## connection-manager.h: ns3::Ptr<ns3::WimaxConnection> ns3::ConnectionManager::GetConnection(ns3::Cid cid) [member function]
    cls.add_method('GetConnection', 
                   'ns3::Ptr< ns3::WimaxConnection >', 
                   [param('ns3::Cid', 'cid')])
    ## connection-manager.h: std::vector<ns3::Ptr<ns3::WimaxConnection>, std::allocator<ns3::Ptr<ns3::WimaxConnection> > > ns3::ConnectionManager::GetConnections(ns3::Cid::Type type) const [member function]
    cls.add_method('GetConnections', 
                   'std::vector< ns3::Ptr< ns3::WimaxConnection > >', 
                   [param('ns3::Cid::Type', 'type')], 
                   is_const=True)
    ## connection-manager.h: uint32_t ns3::ConnectionManager::GetNPackets(ns3::Cid::Type type, ns3::ServiceFlow::SchedulingType schedulingType) const [member function]
    cls.add_method('GetNPackets', 
                   'uint32_t', 
                   [param('ns3::Cid::Type', 'type'), param('ns3::ServiceFlow::SchedulingType', 'schedulingType')], 
                   is_const=True)
    ## connection-manager.h: bool ns3::ConnectionManager::HasPackets() const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [], 
                   is_const=True)
    ## connection-manager.h: void ns3::ConnectionManager::SetCidFactory(ns3::CidFactory * cidFactory) [member function]
    cls.add_method('SetCidFactory', 
                   'void', 
                   [param('ns3::CidFactory *', 'cidFactory')])
    return

def register_Ns3Dcd_methods(root_module, cls):
    ## dl-mac-messages.h: ns3::Dcd::Dcd(ns3::Dcd const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Dcd const &', 'arg0')])
    ## dl-mac-messages.h: ns3::Dcd::Dcd() [constructor]
    cls.add_constructor([])
    ## dl-mac-messages.h: void ns3::Dcd::AddDlBurstProfile(ns3::OfdmDlBurstProfile dlBurstProfile) [member function]
    cls.add_method('AddDlBurstProfile', 
                   'void', 
                   [param('ns3::OfdmDlBurstProfile', 'dlBurstProfile')])
    ## dl-mac-messages.h: uint32_t ns3::Dcd::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## dl-mac-messages.h: ns3::OfdmDcdChannelEncodings ns3::Dcd::GetChannelEncodings() const [member function]
    cls.add_method('GetChannelEncodings', 
                   'ns3::OfdmDcdChannelEncodings', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::Dcd::GetConfigurationChangeCount() const [member function]
    cls.add_method('GetConfigurationChangeCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: std::vector<ns3::OfdmDlBurstProfile, std::allocator<ns3::OfdmDlBurstProfile> > ns3::Dcd::GetDlBurstProfiles() const [member function]
    cls.add_method('GetDlBurstProfiles', 
                   'std::vector< ns3::OfdmDlBurstProfile >', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: ns3::TypeId ns3::Dcd::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dl-mac-messages.h: std::string ns3::Dcd::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::Dcd::GetNrDlBurstProfiles() const [member function]
    cls.add_method('GetNrDlBurstProfiles', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint32_t ns3::Dcd::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dl-mac-messages.h: static ns3::TypeId ns3::Dcd::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dl-mac-messages.h: void ns3::Dcd::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## dl-mac-messages.h: void ns3::Dcd::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## dl-mac-messages.h: void ns3::Dcd::SetChannelEncodings(ns3::OfdmDcdChannelEncodings channelEncodings) [member function]
    cls.add_method('SetChannelEncodings', 
                   'void', 
                   [param('ns3::OfdmDcdChannelEncodings', 'channelEncodings')])
    ## dl-mac-messages.h: void ns3::Dcd::SetConfigurationChangeCount(uint8_t configurationChangeCount) [member function]
    cls.add_method('SetConfigurationChangeCount', 
                   'void', 
                   [param('uint8_t', 'configurationChangeCount')])
    ## dl-mac-messages.h: void ns3::Dcd::SetNrDlBurstProfiles(uint8_t nrDlBurstProfiles) [member function]
    cls.add_method('SetNrDlBurstProfiles', 
                   'void', 
                   [param('uint8_t', 'nrDlBurstProfiles')])
    return

def register_Ns3DlMap_methods(root_module, cls):
    ## dl-mac-messages.h: ns3::DlMap::DlMap(ns3::DlMap const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DlMap const &', 'arg0')])
    ## dl-mac-messages.h: ns3::DlMap::DlMap() [constructor]
    cls.add_constructor([])
    ## dl-mac-messages.h: void ns3::DlMap::AddDlMapElement(ns3::OfdmDlMapIe dlMapElement) [member function]
    cls.add_method('AddDlMapElement', 
                   'void', 
                   [param('ns3::OfdmDlMapIe', 'dlMapElement')])
    ## dl-mac-messages.h: uint32_t ns3::DlMap::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## dl-mac-messages.h: ns3::Mac48Address ns3::DlMap::GetBaseStationId() const [member function]
    cls.add_method('GetBaseStationId', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint8_t ns3::DlMap::GetDcdCount() const [member function]
    cls.add_method('GetDcdCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: std::list<ns3::OfdmDlMapIe, std::allocator<ns3::OfdmDlMapIe> > ns3::DlMap::GetDlMapElements() const [member function]
    cls.add_method('GetDlMapElements', 
                   'std::list< ns3::OfdmDlMapIe >', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: ns3::TypeId ns3::DlMap::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dl-mac-messages.h: std::string ns3::DlMap::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## dl-mac-messages.h: uint32_t ns3::DlMap::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dl-mac-messages.h: static ns3::TypeId ns3::DlMap::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dl-mac-messages.h: void ns3::DlMap::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## dl-mac-messages.h: void ns3::DlMap::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## dl-mac-messages.h: void ns3::DlMap::SetBaseStationId(ns3::Mac48Address baseStationID) [member function]
    cls.add_method('SetBaseStationId', 
                   'void', 
                   [param('ns3::Mac48Address', 'baseStationID')])
    ## dl-mac-messages.h: void ns3::DlMap::SetDcdCount(uint8_t dcdCount) [member function]
    cls.add_method('SetDcdCount', 
                   'void', 
                   [param('uint8_t', 'dcdCount')])
    return

def register_Ns3DsaAck_methods(root_module, cls):
    ## mac-messages.h: ns3::DsaAck::DsaAck(ns3::DsaAck const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DsaAck const &', 'arg0')])
    ## mac-messages.h: ns3::DsaAck::DsaAck() [constructor]
    cls.add_constructor([])
    ## mac-messages.h: uint32_t ns3::DsaAck::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mac-messages.h: uint16_t ns3::DsaAck::GetConfirmationCode() const [member function]
    cls.add_method('GetConfirmationCode', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: ns3::TypeId ns3::DsaAck::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: std::string ns3::DsaAck::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::DsaAck::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: uint16_t ns3::DsaAck::GetTransactionId() const [member function]
    cls.add_method('GetTransactionId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: static ns3::TypeId ns3::DsaAck::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-messages.h: void ns3::DsaAck::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::DsaAck::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::DsaAck::SetConfirmationCode(uint16_t confirmationCode) [member function]
    cls.add_method('SetConfirmationCode', 
                   'void', 
                   [param('uint16_t', 'confirmationCode')])
    ## mac-messages.h: void ns3::DsaAck::SetTransactionId(uint16_t transactionId) [member function]
    cls.add_method('SetTransactionId', 
                   'void', 
                   [param('uint16_t', 'transactionId')])
    return

def register_Ns3DsaReq_methods(root_module, cls):
    ## mac-messages.h: ns3::DsaReq::DsaReq(ns3::DsaReq const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DsaReq const &', 'arg0')])
    ## mac-messages.h: ns3::DsaReq::DsaReq() [constructor]
    cls.add_constructor([])
    ## mac-messages.h: ns3::DsaReq::DsaReq(ns3::ServiceFlow sf) [constructor]
    cls.add_constructor([param('ns3::ServiceFlow', 'sf')])
    ## mac-messages.h: uint32_t ns3::DsaReq::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mac-messages.h: ns3::Cid ns3::DsaReq::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## mac-messages.h: ns3::TypeId ns3::DsaReq::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: std::string ns3::DsaReq::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::DsaReq::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: ns3::ServiceFlow ns3::DsaReq::GetServiceFlow() const [member function]
    cls.add_method('GetServiceFlow', 
                   'ns3::ServiceFlow', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::DsaReq::GetSfid() const [member function]
    cls.add_method('GetSfid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint16_t ns3::DsaReq::GetTransactionId() const [member function]
    cls.add_method('GetTransactionId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: static ns3::TypeId ns3::DsaReq::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-messages.h: void ns3::DsaReq::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::DsaReq::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::DsaReq::SetCid(ns3::Cid cid) [member function]
    cls.add_method('SetCid', 
                   'void', 
                   [param('ns3::Cid', 'cid')])
    ## mac-messages.h: void ns3::DsaReq::SetServiceFlow(ns3::ServiceFlow sf) [member function]
    cls.add_method('SetServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow', 'sf')])
    ## mac-messages.h: void ns3::DsaReq::SetSfid(uint32_t sfid) [member function]
    cls.add_method('SetSfid', 
                   'void', 
                   [param('uint32_t', 'sfid')])
    ## mac-messages.h: void ns3::DsaReq::SetTransactionId(uint16_t transactionId) [member function]
    cls.add_method('SetTransactionId', 
                   'void', 
                   [param('uint16_t', 'transactionId')])
    return

def register_Ns3DsaRsp_methods(root_module, cls):
    ## mac-messages.h: ns3::DsaRsp::DsaRsp(ns3::DsaRsp const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DsaRsp const &', 'arg0')])
    ## mac-messages.h: ns3::DsaRsp::DsaRsp() [constructor]
    cls.add_constructor([])
    ## mac-messages.h: uint32_t ns3::DsaRsp::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mac-messages.h: ns3::Cid ns3::DsaRsp::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint16_t ns3::DsaRsp::GetConfirmationCode() const [member function]
    cls.add_method('GetConfirmationCode', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: ns3::TypeId ns3::DsaRsp::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: std::string ns3::DsaRsp::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::DsaRsp::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: ns3::ServiceFlow ns3::DsaRsp::GetServiceFlow() const [member function]
    cls.add_method('GetServiceFlow', 
                   'ns3::ServiceFlow', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint32_t ns3::DsaRsp::GetSfid() const [member function]
    cls.add_method('GetSfid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: uint16_t ns3::DsaRsp::GetTransactionId() const [member function]
    cls.add_method('GetTransactionId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mac-messages.h: static ns3::TypeId ns3::DsaRsp::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-messages.h: void ns3::DsaRsp::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::DsaRsp::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mac-messages.h: void ns3::DsaRsp::SetCid(ns3::Cid cid) [member function]
    cls.add_method('SetCid', 
                   'void', 
                   [param('ns3::Cid', 'cid')])
    ## mac-messages.h: void ns3::DsaRsp::SetConfirmationCode(uint16_t confirmationCode) [member function]
    cls.add_method('SetConfirmationCode', 
                   'void', 
                   [param('uint16_t', 'confirmationCode')])
    ## mac-messages.h: void ns3::DsaRsp::SetServiceFlow(ns3::ServiceFlow sf) [member function]
    cls.add_method('SetServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow', 'sf')])
    ## mac-messages.h: void ns3::DsaRsp::SetSfid(uint32_t sfid) [member function]
    cls.add_method('SetSfid', 
                   'void', 
                   [param('uint32_t', 'sfid')])
    ## mac-messages.h: void ns3::DsaRsp::SetTransactionId(uint16_t transactionId) [member function]
    cls.add_method('SetTransactionId', 
                   'void', 
                   [param('uint16_t', 'transactionId')])
    return

def register_Ns3FragmentationSubheader_methods(root_module, cls):
    ## wimax-mac-header.h: ns3::FragmentationSubheader::FragmentationSubheader(ns3::FragmentationSubheader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FragmentationSubheader const &', 'arg0')])
    ## wimax-mac-header.h: ns3::FragmentationSubheader::FragmentationSubheader() [constructor]
    cls.add_constructor([])
    ## wimax-mac-header.h: uint32_t ns3::FragmentationSubheader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wimax-mac-header.h: uint8_t ns3::FragmentationSubheader::GetFc() const [member function]
    cls.add_method('GetFc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::FragmentationSubheader::GetFsn() const [member function]
    cls.add_method('GetFsn', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: ns3::TypeId ns3::FragmentationSubheader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: std::string ns3::FragmentationSubheader::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint32_t ns3::FragmentationSubheader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: static ns3::TypeId ns3::FragmentationSubheader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-mac-header.h: void ns3::FragmentationSubheader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::FragmentationSubheader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::FragmentationSubheader::SetFc(uint8_t fc) [member function]
    cls.add_method('SetFc', 
                   'void', 
                   [param('uint8_t', 'fc')])
    ## wimax-mac-header.h: void ns3::FragmentationSubheader::SetFsn(uint8_t fsn) [member function]
    cls.add_method('SetFsn', 
                   'void', 
                   [param('uint8_t', 'fsn')])
    return

def register_Ns3GenericMacHeader_methods(root_module, cls):
    ## wimax-mac-header.h: ns3::GenericMacHeader::GenericMacHeader(ns3::GenericMacHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GenericMacHeader const &', 'arg0')])
    ## wimax-mac-header.h: ns3::GenericMacHeader::GenericMacHeader() [constructor]
    cls.add_constructor([])
    ## wimax-mac-header.h: uint32_t ns3::GenericMacHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wimax-mac-header.h: uint8_t ns3::GenericMacHeader::GetCi() const [member function]
    cls.add_method('GetCi', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: ns3::Cid ns3::GenericMacHeader::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::GenericMacHeader::GetEc() const [member function]
    cls.add_method('GetEc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::GenericMacHeader::GetEks() const [member function]
    cls.add_method('GetEks', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::GenericMacHeader::GetHcs() const [member function]
    cls.add_method('GetHcs', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::GenericMacHeader::GetHt() const [member function]
    cls.add_method('GetHt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: ns3::TypeId ns3::GenericMacHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: uint16_t ns3::GenericMacHeader::GetLen() const [member function]
    cls.add_method('GetLen', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: std::string ns3::GenericMacHeader::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint32_t ns3::GenericMacHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: uint8_t ns3::GenericMacHeader::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: static ns3::TypeId ns3::GenericMacHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-mac-header.h: void ns3::GenericMacHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::GenericMacHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::GenericMacHeader::SetCi(uint8_t ci) [member function]
    cls.add_method('SetCi', 
                   'void', 
                   [param('uint8_t', 'ci')])
    ## wimax-mac-header.h: void ns3::GenericMacHeader::SetCid(ns3::Cid cid) [member function]
    cls.add_method('SetCid', 
                   'void', 
                   [param('ns3::Cid', 'cid')])
    ## wimax-mac-header.h: void ns3::GenericMacHeader::SetEc(uint8_t ec) [member function]
    cls.add_method('SetEc', 
                   'void', 
                   [param('uint8_t', 'ec')])
    ## wimax-mac-header.h: void ns3::GenericMacHeader::SetEks(uint8_t eks) [member function]
    cls.add_method('SetEks', 
                   'void', 
                   [param('uint8_t', 'eks')])
    ## wimax-mac-header.h: void ns3::GenericMacHeader::SetHcs(uint8_t hcs) [member function]
    cls.add_method('SetHcs', 
                   'void', 
                   [param('uint8_t', 'hcs')])
    ## wimax-mac-header.h: void ns3::GenericMacHeader::SetHt(uint8_t HT) [member function]
    cls.add_method('SetHt', 
                   'void', 
                   [param('uint8_t', 'HT')])
    ## wimax-mac-header.h: void ns3::GenericMacHeader::SetLen(uint16_t len) [member function]
    cls.add_method('SetLen', 
                   'void', 
                   [param('uint16_t', 'len')])
    ## wimax-mac-header.h: void ns3::GenericMacHeader::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## wimax-mac-header.h: bool ns3::GenericMacHeader::check_hcs() const [member function]
    cls.add_method('check_hcs', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3GrantManagementSubheader_methods(root_module, cls):
    ## wimax-mac-header.h: ns3::GrantManagementSubheader::GrantManagementSubheader(ns3::GrantManagementSubheader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GrantManagementSubheader const &', 'arg0')])
    ## wimax-mac-header.h: ns3::GrantManagementSubheader::GrantManagementSubheader() [constructor]
    cls.add_constructor([])
    ## wimax-mac-header.h: uint32_t ns3::GrantManagementSubheader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wimax-mac-header.h: ns3::TypeId ns3::GrantManagementSubheader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: std::string ns3::GrantManagementSubheader::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint16_t ns3::GrantManagementSubheader::GetPbr() const [member function]
    cls.add_method('GetPbr', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint8_t ns3::GrantManagementSubheader::GetPm() const [member function]
    cls.add_method('GetPm', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: uint32_t ns3::GrantManagementSubheader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: uint8_t ns3::GrantManagementSubheader::GetSi() const [member function]
    cls.add_method('GetSi', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-mac-header.h: static ns3::TypeId ns3::GrantManagementSubheader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-mac-header.h: void ns3::GrantManagementSubheader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::GrantManagementSubheader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wimax-mac-header.h: void ns3::GrantManagementSubheader::SetPbr(uint16_t pbr) [member function]
    cls.add_method('SetPbr', 
                   'void', 
                   [param('uint16_t', 'pbr')])
    ## wimax-mac-header.h: void ns3::GrantManagementSubheader::SetPm(uint8_t pm) [member function]
    cls.add_method('SetPm', 
                   'void', 
                   [param('uint8_t', 'pm')])
    ## wimax-mac-header.h: void ns3::GrantManagementSubheader::SetSi(uint8_t si) [member function]
    cls.add_method('SetSi', 
                   'void', 
                   [param('uint8_t', 'si')])
    return

def register_Ns3IpcsClassifier_methods(root_module, cls):
    ## ipcs-classifier.h: ns3::IpcsClassifier::IpcsClassifier(ns3::IpcsClassifier const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IpcsClassifier const &', 'arg0')])
    ## ipcs-classifier.h: ns3::IpcsClassifier::IpcsClassifier() [constructor]
    cls.add_constructor([])
    ## ipcs-classifier.h: ns3::ServiceFlow * ns3::IpcsClassifier::Classify(ns3::Ptr<ns3::Packet const> packet, ns3::Ptr<ns3::ServiceFlowManager> sfm, ns3::ServiceFlow::Direction dir) [member function]
    cls.add_method('Classify', 
                   'ns3::ServiceFlow *', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Ptr< ns3::ServiceFlowManager >', 'sfm'), param('ns3::ServiceFlow::Direction', 'dir')])
    return

def register_Ns3SimpleOfdmWimaxPhy_methods(root_module, cls):
    ## simple-ofdm-wimax-phy.h: ns3::SimpleOfdmWimaxPhy::SimpleOfdmWimaxPhy(ns3::SimpleOfdmWimaxPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimpleOfdmWimaxPhy const &', 'arg0')])
    ## simple-ofdm-wimax-phy.h: ns3::SimpleOfdmWimaxPhy::SimpleOfdmWimaxPhy() [constructor]
    cls.add_constructor([])
    ## simple-ofdm-wimax-phy.h: ns3::SimpleOfdmWimaxPhy::SimpleOfdmWimaxPhy(char * tracesPath) [constructor]
    cls.add_constructor([param('char *', 'tracesPath')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::ActivateLoss(bool loss) [member function]
    cls.add_method('ActivateLoss', 
                   'void', 
                   [param('bool', 'loss')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::DoAttach(ns3::Ptr<ns3::WimaxChannel> channel) [member function]
    cls.add_method('DoAttach', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxChannel >', 'channel')], 
                   is_virtual=True)
    ## simple-ofdm-wimax-phy.h: uint32_t ns3::SimpleOfdmWimaxPhy::GetBandwidth() const [member function]
    cls.add_method('GetBandwidth', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## simple-ofdm-wimax-phy.h: double ns3::SimpleOfdmWimaxPhy::GetNoiseFigure() const [member function]
    cls.add_method('GetNoiseFigure', 
                   'double', 
                   [], 
                   is_const=True)
    ## simple-ofdm-wimax-phy.h: ns3::WimaxPhy::PhyType ns3::SimpleOfdmWimaxPhy::GetPhyType() const [member function]
    cls.add_method('GetPhyType', 
                   'ns3::WimaxPhy::PhyType', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-ofdm-wimax-phy.h: double ns3::SimpleOfdmWimaxPhy::GetTxPower() const [member function]
    cls.add_method('GetTxPower', 
                   'double', 
                   [], 
                   is_const=True)
    ## simple-ofdm-wimax-phy.h: static ns3::TypeId ns3::SimpleOfdmWimaxPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::NotifyRxBegin(ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('NotifyRxBegin', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::NotifyRxDrop(ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('NotifyRxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::NotifyRxEnd(ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('NotifyRxEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::NotifyTxBegin(ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('NotifyTxBegin', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::NotifyTxDrop(ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('NotifyTxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::NotifyTxEnd(ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('NotifyTxEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::Send(ns3::Ptr<ns3::PacketBurst> burst, ns3::WimaxPhy::ModulationType modulationType, uint8_t direction) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'burst'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('uint8_t', 'direction')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::Send(ns3::SendParams * params) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::SendParams *', 'params')], 
                   is_virtual=True)
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::SetBandwidth(uint32_t BW) [member function]
    cls.add_method('SetBandwidth', 
                   'void', 
                   [param('uint32_t', 'BW')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::SetNoiseFigure(double nf) [member function]
    cls.add_method('SetNoiseFigure', 
                   'void', 
                   [param('double', 'nf')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::SetReceiveCallback(ns3::Callback<void,ns3::Ptr<ns3::PacketBurst>,ns3::Ptr<ns3::WimaxConnection>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::PacketBurst >, ns3::Ptr< ns3::WimaxConnection >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::SetSNRToBlockErrorRateTracesPath(char * tracesPath) [member function]
    cls.add_method('SetSNRToBlockErrorRateTracesPath', 
                   'void', 
                   [param('char *', 'tracesPath')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::SetTxPower(double txPower) [member function]
    cls.add_method('SetTxPower', 
                   'void', 
                   [param('double', 'txPower')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::StartReceive(uint32_t burstSize, bool isFirstBlock, uint64_t frequency, ns3::WimaxPhy::ModulationType modulationType, uint8_t direction, double rxPower, ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('StartReceive', 
                   'void', 
                   [param('uint32_t', 'burstSize'), param('bool', 'isFirstBlock'), param('uint64_t', 'frequency'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('uint8_t', 'direction'), param('double', 'rxPower'), param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: uint32_t ns3::SimpleOfdmWimaxPhy::DoGetDataRate(ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('DoGetDataRate', 
                   'uint32_t', 
                   [param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: ns3::Time ns3::SimpleOfdmWimaxPhy::DoGetFrameDuration(uint8_t frameDurationCode) const [member function]
    cls.add_method('DoGetFrameDuration', 
                   'ns3::Time', 
                   [param('uint8_t', 'frameDurationCode')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: uint8_t ns3::SimpleOfdmWimaxPhy::DoGetFrameDurationCode() const [member function]
    cls.add_method('DoGetFrameDurationCode', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: double ns3::SimpleOfdmWimaxPhy::DoGetGValue() const [member function]
    cls.add_method('DoGetGValue', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: uint16_t ns3::SimpleOfdmWimaxPhy::DoGetNfft() const [member function]
    cls.add_method('DoGetNfft', 
                   'uint16_t', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: uint64_t ns3::SimpleOfdmWimaxPhy::DoGetNrBytes(uint32_t symbols, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('DoGetNrBytes', 
                   'uint64_t', 
                   [param('uint32_t', 'symbols'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: uint64_t ns3::SimpleOfdmWimaxPhy::DoGetNrSymbols(uint32_t size, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('DoGetNrSymbols', 
                   'uint64_t', 
                   [param('uint32_t', 'size'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: uint16_t ns3::SimpleOfdmWimaxPhy::DoGetRtg() const [member function]
    cls.add_method('DoGetRtg', 
                   'uint16_t', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: double ns3::SimpleOfdmWimaxPhy::DoGetSamplingFactor() const [member function]
    cls.add_method('DoGetSamplingFactor', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: double ns3::SimpleOfdmWimaxPhy::DoGetSamplingFrequency() const [member function]
    cls.add_method('DoGetSamplingFrequency', 
                   'double', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: ns3::Time ns3::SimpleOfdmWimaxPhy::DoGetTransmissionTime(uint32_t size, ns3::WimaxPhy::ModulationType modulationType) const [member function]
    cls.add_method('DoGetTransmissionTime', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WimaxPhy::ModulationType', 'modulationType')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: uint16_t ns3::SimpleOfdmWimaxPhy::DoGetTtg() const [member function]
    cls.add_method('DoGetTtg', 
                   'uint16_t', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::DoSetDataRates() [member function]
    cls.add_method('DoSetDataRates', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-phy.h: void ns3::SimpleOfdmWimaxPhy::DoSetPhyParameters() [member function]
    cls.add_method('DoSetPhyParameters', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3WimaxChannel_methods(root_module, cls):
    ## wimax-channel.h: ns3::WimaxChannel::WimaxChannel(ns3::WimaxChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WimaxChannel const &', 'arg0')])
    ## wimax-channel.h: ns3::WimaxChannel::WimaxChannel() [constructor]
    cls.add_constructor([])
    ## wimax-channel.h: void ns3::WimaxChannel::Attach(ns3::Ptr<ns3::WimaxPhy> phy) [member function]
    cls.add_method('Attach', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxPhy >', 'phy')])
    ## wimax-channel.h: ns3::Ptr<ns3::NetDevice> ns3::WimaxChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## wimax-channel.h: uint32_t ns3::WimaxChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-channel.h: void ns3::WimaxChannel::DoAttach(ns3::Ptr<ns3::WimaxPhy> phy) [member function]
    cls.add_method('DoAttach', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxPhy >', 'phy')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wimax-channel.h: ns3::Ptr<ns3::NetDevice> ns3::WimaxChannel::DoGetDevice(uint32_t i) const [member function]
    cls.add_method('DoGetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wimax-channel.h: uint32_t ns3::WimaxChannel::DoGetNDevices() const [member function]
    cls.add_method('DoGetNDevices', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3WimaxNetDevice_methods(root_module, cls):
    ## wimax-net-device.h: ns3::WimaxNetDevice::m_direction [variable]
    cls.add_static_attribute('m_direction', 'uint8_t', is_const=False)
    ## wimax-net-device.h: ns3::WimaxNetDevice::m_frameStartTime [variable]
    cls.add_static_attribute('m_frameStartTime', 'ns3::Time', is_const=False)
    ## wimax-net-device.h: ns3::WimaxNetDevice::m_traceRx [variable]
    cls.add_instance_attribute('m_traceRx', 'ns3::TracedCallback< ns3::Ptr< ns3::Packet const >, ns3::Mac48Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', is_const=False)
    ## wimax-net-device.h: ns3::WimaxNetDevice::m_traceTx [variable]
    cls.add_instance_attribute('m_traceTx', 'ns3::TracedCallback< ns3::Ptr< ns3::Packet const >, ns3::Mac48Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', is_const=False)
    ## wimax-net-device.h: static ns3::TypeId ns3::WimaxNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wimax-net-device.h: ns3::WimaxNetDevice::WimaxNetDevice() [constructor]
    cls.add_constructor([])
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetTtg(uint16_t ttg) [member function]
    cls.add_method('SetTtg', 
                   'void', 
                   [param('uint16_t', 'ttg')])
    ## wimax-net-device.h: uint16_t ns3::WimaxNetDevice::GetTtg() const [member function]
    cls.add_method('GetTtg', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetRtg(uint16_t rtg) [member function]
    cls.add_method('SetRtg', 
                   'void', 
                   [param('uint16_t', 'rtg')])
    ## wimax-net-device.h: uint16_t ns3::WimaxNetDevice::GetRtg() const [member function]
    cls.add_method('GetRtg', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::Attach(ns3::Ptr<ns3::WimaxChannel> channel) [member function]
    cls.add_method('Attach', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxChannel >', 'channel')])
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetPhy(ns3::Ptr<ns3::WimaxPhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxPhy >', 'phy')])
    ## wimax-net-device.h: ns3::Ptr<ns3::WimaxPhy> ns3::WimaxNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::WimaxPhy >', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetChannel(ns3::Ptr<ns3::WimaxChannel> wimaxChannel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxChannel >', 'wimaxChannel')])
    ## wimax-net-device.h: uint64_t ns3::WimaxNetDevice::GetChannel(uint8_t index) const [member function]
    cls.add_method('GetChannel', 
                   'uint64_t', 
                   [param('uint8_t', 'index')], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetNrFrames(uint32_t nrFrames) [member function]
    cls.add_method('SetNrFrames', 
                   'void', 
                   [param('uint32_t', 'nrFrames')])
    ## wimax-net-device.h: uint32_t ns3::WimaxNetDevice::GetNrFrames() const [member function]
    cls.add_method('GetNrFrames', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetMacAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetMacAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wimax-net-device.h: ns3::Mac48Address ns3::WimaxNetDevice::GetMacAddress() const [member function]
    cls.add_method('GetMacAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetState(uint8_t state) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('uint8_t', 'state')])
    ## wimax-net-device.h: uint8_t ns3::WimaxNetDevice::GetState() const [member function]
    cls.add_method('GetState', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: ns3::Ptr<ns3::WimaxConnection> ns3::WimaxNetDevice::GetInitialRangingConnection() const [member function]
    cls.add_method('GetInitialRangingConnection', 
                   'ns3::Ptr< ns3::WimaxConnection >', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: ns3::Ptr<ns3::WimaxConnection> ns3::WimaxNetDevice::GetBroadcastConnection() const [member function]
    cls.add_method('GetBroadcastConnection', 
                   'ns3::Ptr< ns3::WimaxConnection >', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetCurrentDcd(ns3::Dcd dcd) [member function]
    cls.add_method('SetCurrentDcd', 
                   'void', 
                   [param('ns3::Dcd', 'dcd')])
    ## wimax-net-device.h: ns3::Dcd ns3::WimaxNetDevice::GetCurrentDcd() const [member function]
    cls.add_method('GetCurrentDcd', 
                   'ns3::Dcd', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetCurrentUcd(ns3::Ucd ucd) [member function]
    cls.add_method('SetCurrentUcd', 
                   'void', 
                   [param('ns3::Ucd', 'ucd')])
    ## wimax-net-device.h: ns3::Ucd ns3::WimaxNetDevice::GetCurrentUcd() const [member function]
    cls.add_method('GetCurrentUcd', 
                   'ns3::Ucd', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: ns3::Ptr<ns3::ConnectionManager> ns3::WimaxNetDevice::GetConnectionManager() const [member function]
    cls.add_method('GetConnectionManager', 
                   'ns3::Ptr< ns3::ConnectionManager >', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetConnectionManager(ns3::Ptr<ns3::ConnectionManager> connectionManager) [member function]
    cls.add_method('SetConnectionManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::ConnectionManager >', 'connectionManager')], 
                   is_virtual=True)
    ## wimax-net-device.h: ns3::Ptr<ns3::BurstProfileManager> ns3::WimaxNetDevice::GetBurstProfileManager() const [member function]
    cls.add_method('GetBurstProfileManager', 
                   'ns3::Ptr< ns3::BurstProfileManager >', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetBurstProfileManager(ns3::Ptr<ns3::BurstProfileManager> burstProfileManager) [member function]
    cls.add_method('SetBurstProfileManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::BurstProfileManager >', 'burstProfileManager')])
    ## wimax-net-device.h: ns3::Ptr<ns3::BandwidthManager> ns3::WimaxNetDevice::GetBandwidthManager() const [member function]
    cls.add_method('GetBandwidthManager', 
                   'ns3::Ptr< ns3::BandwidthManager >', 
                   [], 
                   is_const=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetBandwidthManager(ns3::Ptr<ns3::BandwidthManager> bandwidthManager) [member function]
    cls.add_method('SetBandwidthManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::BandwidthManager >', 'bandwidthManager')])
    ## wimax-net-device.h: void ns3::WimaxNetDevice::CreateDefaultConnections() [member function]
    cls.add_method('CreateDefaultConnections', 
                   'void', 
                   [])
    ## wimax-net-device.h: void ns3::WimaxNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetReceiveCallback() [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [])
    ## wimax-net-device.h: void ns3::WimaxNetDevice::ForwardUp(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest')])
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::Enqueue(ns3::Ptr<ns3::Packet> packet, ns3::MacHeaderType const & hdrType, ns3::Ptr<ns3::WimaxConnection> connection) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::MacHeaderType const &', 'hdrType'), param('ns3::Ptr< ns3::WimaxConnection >', 'connection')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::ForwardDown(ns3::Ptr<ns3::PacketBurst> burst, ns3::WimaxPhy::ModulationType modulationType) [member function]
    cls.add_method('ForwardDown', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'burst'), param('ns3::WimaxPhy::ModulationType', 'modulationType')])
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetName(std::string const name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('std::string const', 'name')], 
                   is_virtual=True)
    ## wimax-net-device.h: std::string ns3::WimaxNetDevice::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## wimax-net-device.h: uint32_t ns3::WimaxNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: ns3::Ptr<ns3::Channel> ns3::WimaxNetDevice::GetPhyChannel() const [member function]
    cls.add_method('GetPhyChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: ns3::Ptr<ns3::Channel> ns3::WimaxNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## wimax-net-device.h: ns3::Address ns3::WimaxNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## wimax-net-device.h: uint16_t ns3::WimaxNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: ns3::Address ns3::WimaxNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: ns3::Address ns3::WimaxNetDevice::GetMulticast() const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: ns3::Address ns3::WimaxNetDevice::MakeMulticastAddress(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('MakeMulticastAddress', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## wimax-net-device.h: ns3::Ptr<ns3::Node> ns3::WimaxNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## wimax-net-device.h: ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> ns3::WimaxNetDevice::GetPromiscReceiveCallback() [member function]
    cls.add_method('GetPromiscReceiveCallback', 
                   'ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 
                   [])
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## wimax-net-device.h: ns3::Address ns3::WimaxNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: ns3::Address ns3::WimaxNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::IsPromisc() [member function]
    cls.add_method('IsPromisc', 
                   'bool', 
                   [])
    ## wimax-net-device.h: void ns3::WimaxNetDevice::NotifyPromiscTrace(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('NotifyPromiscTrace', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## wimax-net-device.h: bool ns3::WimaxNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wimax-net-device.h: void ns3::WimaxNetDevice::DoReceive(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wimax-net-device.h: ns3::Ptr<ns3::WimaxChannel> ns3::WimaxNetDevice::DoGetChannel() const [member function]
    cls.add_method('DoGetChannel', 
                   'ns3::Ptr< ns3::WimaxChannel >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3BaseStationNetDevice_methods(root_module, cls):
    ## bs-net-device.h: static ns3::TypeId ns3::BaseStationNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bs-net-device.h: ns3::BaseStationNetDevice::BaseStationNetDevice() [constructor]
    cls.add_constructor([])
    ## bs-net-device.h: ns3::BaseStationNetDevice::BaseStationNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WimaxPhy> phy) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::WimaxPhy >', 'phy')])
    ## bs-net-device.h: ns3::BaseStationNetDevice::BaseStationNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::WimaxPhy> phy, ns3::Ptr<ns3::UplinkScheduler> uplinkScheduler, ns3::Ptr<ns3::BSScheduler> bsScheduler) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::WimaxPhy >', 'phy'), param('ns3::Ptr< ns3::UplinkScheduler >', 'uplinkScheduler'), param('ns3::Ptr< ns3::BSScheduler >', 'bsScheduler')])
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetInitialRangingInterval(ns3::Time initialRangInterval) [member function]
    cls.add_method('SetInitialRangingInterval', 
                   'void', 
                   [param('ns3::Time', 'initialRangInterval')])
    ## bs-net-device.h: void ns3::BaseStationNetDevice::InitBaseStationNetDevice() [member function]
    cls.add_method('InitBaseStationNetDevice', 
                   'void', 
                   [])
    ## bs-net-device.h: ns3::Time ns3::BaseStationNetDevice::GetInitialRangingInterval() const [member function]
    cls.add_method('GetInitialRangingInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetDcdInterval(ns3::Time dcdInterval) [member function]
    cls.add_method('SetDcdInterval', 
                   'void', 
                   [param('ns3::Time', 'dcdInterval')])
    ## bs-net-device.h: ns3::Time ns3::BaseStationNetDevice::GetDcdInterval() const [member function]
    cls.add_method('GetDcdInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetUcdInterval(ns3::Time ucdInterval) [member function]
    cls.add_method('SetUcdInterval', 
                   'void', 
                   [param('ns3::Time', 'ucdInterval')])
    ## bs-net-device.h: ns3::Time ns3::BaseStationNetDevice::GetUcdInterval() const [member function]
    cls.add_method('GetUcdInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetIntervalT8(ns3::Time interval) [member function]
    cls.add_method('SetIntervalT8', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## bs-net-device.h: ns3::Time ns3::BaseStationNetDevice::GetIntervalT8() const [member function]
    cls.add_method('GetIntervalT8', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetMaxRangingCorrectionRetries(uint8_t maxRangCorrectionRetries) [member function]
    cls.add_method('SetMaxRangingCorrectionRetries', 
                   'void', 
                   [param('uint8_t', 'maxRangCorrectionRetries')])
    ## bs-net-device.h: uint8_t ns3::BaseStationNetDevice::GetMaxRangingCorrectionRetries() const [member function]
    cls.add_method('GetMaxRangingCorrectionRetries', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetMaxInvitedRangRetries(uint8_t maxInvitedRangRetries) [member function]
    cls.add_method('SetMaxInvitedRangRetries', 
                   'void', 
                   [param('uint8_t', 'maxInvitedRangRetries')])
    ## bs-net-device.h: uint8_t ns3::BaseStationNetDevice::GetMaxInvitedRangRetries() const [member function]
    cls.add_method('GetMaxInvitedRangRetries', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetRangReqOppSize(uint8_t rangReqOppSize) [member function]
    cls.add_method('SetRangReqOppSize', 
                   'void', 
                   [param('uint8_t', 'rangReqOppSize')])
    ## bs-net-device.h: uint8_t ns3::BaseStationNetDevice::GetRangReqOppSize() const [member function]
    cls.add_method('GetRangReqOppSize', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetBwReqOppSize(uint8_t bwReqOppSize) [member function]
    cls.add_method('SetBwReqOppSize', 
                   'void', 
                   [param('uint8_t', 'bwReqOppSize')])
    ## bs-net-device.h: uint8_t ns3::BaseStationNetDevice::GetBwReqOppSize() const [member function]
    cls.add_method('GetBwReqOppSize', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetNrDlSymbols(uint32_t dlSymbols) [member function]
    cls.add_method('SetNrDlSymbols', 
                   'void', 
                   [param('uint32_t', 'dlSymbols')])
    ## bs-net-device.h: uint32_t ns3::BaseStationNetDevice::GetNrDlSymbols() const [member function]
    cls.add_method('GetNrDlSymbols', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetNrUlSymbols(uint32_t ulSymbols) [member function]
    cls.add_method('SetNrUlSymbols', 
                   'void', 
                   [param('uint32_t', 'ulSymbols')])
    ## bs-net-device.h: uint32_t ns3::BaseStationNetDevice::GetNrUlSymbols() const [member function]
    cls.add_method('GetNrUlSymbols', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: uint32_t ns3::BaseStationNetDevice::GetNrDcdSent() const [member function]
    cls.add_method('GetNrDcdSent', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: uint32_t ns3::BaseStationNetDevice::GetNrUcdSent() const [member function]
    cls.add_method('GetNrUcdSent', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: ns3::Time ns3::BaseStationNetDevice::GetDlSubframeStartTime() const [member function]
    cls.add_method('GetDlSubframeStartTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: ns3::Time ns3::BaseStationNetDevice::GetUlSubframeStartTime() const [member function]
    cls.add_method('GetUlSubframeStartTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: uint8_t ns3::BaseStationNetDevice::GetRangingOppNumber() const [member function]
    cls.add_method('GetRangingOppNumber', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: ns3::Ptr<ns3::SSManager> ns3::BaseStationNetDevice::GetSSManager() const [member function]
    cls.add_method('GetSSManager', 
                   'ns3::Ptr< ns3::SSManager >', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetSSManager(ns3::Ptr<ns3::SSManager> ssManager) [member function]
    cls.add_method('SetSSManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::SSManager >', 'ssManager')])
    ## bs-net-device.h: ns3::Ptr<ns3::UplinkScheduler> ns3::BaseStationNetDevice::GetUplinkScheduler() const [member function]
    cls.add_method('GetUplinkScheduler', 
                   'ns3::Ptr< ns3::UplinkScheduler >', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetUplinkScheduler(ns3::Ptr<ns3::UplinkScheduler> ulScheduler) [member function]
    cls.add_method('SetUplinkScheduler', 
                   'void', 
                   [param('ns3::Ptr< ns3::UplinkScheduler >', 'ulScheduler')])
    ## bs-net-device.h: ns3::Ptr<ns3::BSLinkManager> ns3::BaseStationNetDevice::GetLinkManager() const [member function]
    cls.add_method('GetLinkManager', 
                   'ns3::Ptr< ns3::BSLinkManager >', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetBSScheduler(ns3::Ptr<ns3::BSScheduler> bsSchedule) [member function]
    cls.add_method('SetBSScheduler', 
                   'void', 
                   [param('ns3::Ptr< ns3::BSScheduler >', 'bsSchedule')])
    ## bs-net-device.h: ns3::Ptr<ns3::BSScheduler> ns3::BaseStationNetDevice::GetBSScheduler() const [member function]
    cls.add_method('GetBSScheduler', 
                   'ns3::Ptr< ns3::BSScheduler >', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetLinkManager(ns3::Ptr<ns3::BSLinkManager> linkManager) [member function]
    cls.add_method('SetLinkManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::BSLinkManager >', 'linkManager')])
    ## bs-net-device.h: ns3::Ptr<ns3::IpcsClassifier> ns3::BaseStationNetDevice::GetBsClassifier() const [member function]
    cls.add_method('GetBsClassifier', 
                   'ns3::Ptr< ns3::IpcsClassifier >', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetBsClassifier(ns3::Ptr<ns3::IpcsClassifier> classifier) [member function]
    cls.add_method('SetBsClassifier', 
                   'void', 
                   [param('ns3::Ptr< ns3::IpcsClassifier >', 'classifier')])
    ## bs-net-device.h: ns3::Time ns3::BaseStationNetDevice::GetPsDuration() const [member function]
    cls.add_method('GetPsDuration', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: ns3::Time ns3::BaseStationNetDevice::GetSymbolDuration() const [member function]
    cls.add_method('GetSymbolDuration', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## bs-net-device.h: bool ns3::BaseStationNetDevice::Enqueue(ns3::Ptr<ns3::Packet> packet, ns3::MacHeaderType const & hdrType, ns3::Ptr<ns3::WimaxConnection> connection) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::MacHeaderType const &', 'hdrType'), param('ns3::Ptr< ns3::WimaxConnection >', 'connection')], 
                   is_virtual=True)
    ## bs-net-device.h: ns3::Ptr<ns3::WimaxConnection> ns3::BaseStationNetDevice::GetConnection(ns3::Cid cid) [member function]
    cls.add_method('GetConnection', 
                   'ns3::Ptr< ns3::WimaxConnection >', 
                   [param('ns3::Cid', 'cid')])
    ## bs-net-device.h: void ns3::BaseStationNetDevice::MarkUplinkAllocations() [member function]
    cls.add_method('MarkUplinkAllocations', 
                   'void', 
                   [])
    ## bs-net-device.h: void ns3::BaseStationNetDevice::MarkRangingOppStart(ns3::Time rangingOppStartTime) [member function]
    cls.add_method('MarkRangingOppStart', 
                   'void', 
                   [param('ns3::Time', 'rangingOppStartTime')])
    ## bs-net-device.h: ns3::Ptr<ns3::BsServiceFlowManager> ns3::BaseStationNetDevice::GetServiceFlowManager() const [member function]
    cls.add_method('GetServiceFlowManager', 
                   'ns3::Ptr< ns3::BsServiceFlowManager >', 
                   [], 
                   is_const=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::SetServiceFlowManager(ns3::Ptr<ns3::BsServiceFlowManager> arg0) [member function]
    cls.add_method('SetServiceFlowManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::BsServiceFlowManager >', 'arg0')])
    ## bs-net-device.h: void ns3::BaseStationNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## bs-net-device.h: bool ns3::BaseStationNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   visibility='private', is_virtual=True)
    ## bs-net-device.h: void ns3::BaseStationNetDevice::DoReceive(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3SimpleOfdmWimaxChannel_methods(root_module, cls):
    ## simple-ofdm-wimax-channel.h: ns3::SimpleOfdmWimaxChannel::SimpleOfdmWimaxChannel(ns3::SimpleOfdmWimaxChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimpleOfdmWimaxChannel const &', 'arg0')])
    ## simple-ofdm-wimax-channel.h: ns3::SimpleOfdmWimaxChannel::SimpleOfdmWimaxChannel() [constructor]
    cls.add_constructor([])
    ## simple-ofdm-wimax-channel.h: ns3::SimpleOfdmWimaxChannel::SimpleOfdmWimaxChannel(ns3::SimpleOfdmWimaxChannel::PropModel propModel) [constructor]
    cls.add_constructor([param('ns3::SimpleOfdmWimaxChannel::PropModel', 'propModel')])
    ## simple-ofdm-wimax-channel.h: void ns3::SimpleOfdmWimaxChannel::Send(ns3::Time BlockTime, uint32_t burstSize, ns3::Ptr<ns3::WimaxPhy> phy, bool isFirstBlock, bool isLastBlock, uint64_t frequency, ns3::WimaxPhy::ModulationType modulationType, uint8_t direction, double txPowerDbm, ns3::Ptr<ns3::PacketBurst> burst) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Time', 'BlockTime'), param('uint32_t', 'burstSize'), param('ns3::Ptr< ns3::WimaxPhy >', 'phy'), param('bool', 'isFirstBlock'), param('bool', 'isLastBlock'), param('uint64_t', 'frequency'), param('ns3::WimaxPhy::ModulationType', 'modulationType'), param('uint8_t', 'direction'), param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::PacketBurst >', 'burst')])
    ## simple-ofdm-wimax-channel.h: void ns3::SimpleOfdmWimaxChannel::SetPropagationModel(ns3::SimpleOfdmWimaxChannel::PropModel propModel) [member function]
    cls.add_method('SetPropagationModel', 
                   'void', 
                   [param('ns3::SimpleOfdmWimaxChannel::PropModel', 'propModel')])
    ## simple-ofdm-wimax-channel.h: void ns3::SimpleOfdmWimaxChannel::DoAttach(ns3::Ptr<ns3::WimaxPhy> phy) [member function]
    cls.add_method('DoAttach', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxPhy >', 'phy')], 
                   visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-channel.h: ns3::Ptr<ns3::NetDevice> ns3::SimpleOfdmWimaxChannel::DoGetDevice(uint32_t i) const [member function]
    cls.add_method('DoGetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, visibility='private', is_virtual=True)
    ## simple-ofdm-wimax-channel.h: uint32_t ns3::SimpleOfdmWimaxChannel::DoGetNDevices() const [member function]
    cls.add_method('DoGetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3SubscriberStationNetDevice_methods(root_module, cls):
    ## ss-net-device.h: ns3::SubscriberStationNetDevice::m_linkManager [variable]
    cls.add_instance_attribute('m_linkManager', 'ns3::Ptr< ns3::SSLinkManager >', is_const=False)
    ## ss-net-device.h: static ns3::TypeId ns3::SubscriberStationNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ss-net-device.h: ns3::SubscriberStationNetDevice::SubscriberStationNetDevice() [constructor]
    cls.add_constructor([])
    ## ss-net-device.h: ns3::SubscriberStationNetDevice::SubscriberStationNetDevice(ns3::Ptr<ns3::Node> arg0, ns3::Ptr<ns3::WimaxPhy> arg1) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'arg0'), param('ns3::Ptr< ns3::WimaxPhy >', 'arg1')])
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::InitSubscriberStationNetDevice() [member function]
    cls.add_method('InitSubscriberStationNetDevice', 
                   'void', 
                   [])
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetLostDlMapInterval(ns3::Time lostDlMapInterval) [member function]
    cls.add_method('SetLostDlMapInterval', 
                   'void', 
                   [param('ns3::Time', 'lostDlMapInterval')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetLostDlMapInterval() const [member function]
    cls.add_method('GetLostDlMapInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetLostUlMapInterval(ns3::Time lostUlMapInterval) [member function]
    cls.add_method('SetLostUlMapInterval', 
                   'void', 
                   [param('ns3::Time', 'lostUlMapInterval')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetLostUlMapInterval() const [member function]
    cls.add_method('GetLostUlMapInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetMaxDcdInterval(ns3::Time maxDcdInterval) [member function]
    cls.add_method('SetMaxDcdInterval', 
                   'void', 
                   [param('ns3::Time', 'maxDcdInterval')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetMaxDcdInterval() const [member function]
    cls.add_method('GetMaxDcdInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetMaxUcdInterval(ns3::Time maxUcdInterval) [member function]
    cls.add_method('SetMaxUcdInterval', 
                   'void', 
                   [param('ns3::Time', 'maxUcdInterval')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetMaxUcdInterval() const [member function]
    cls.add_method('GetMaxUcdInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetIntervalT1(ns3::Time interval1) [member function]
    cls.add_method('SetIntervalT1', 
                   'void', 
                   [param('ns3::Time', 'interval1')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetIntervalT1() const [member function]
    cls.add_method('GetIntervalT1', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetIntervalT2(ns3::Time interval2) [member function]
    cls.add_method('SetIntervalT2', 
                   'void', 
                   [param('ns3::Time', 'interval2')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetIntervalT2() const [member function]
    cls.add_method('GetIntervalT2', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetIntervalT3(ns3::Time interval3) [member function]
    cls.add_method('SetIntervalT3', 
                   'void', 
                   [param('ns3::Time', 'interval3')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetIntervalT3() const [member function]
    cls.add_method('GetIntervalT3', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetIntervalT7(ns3::Time interval7) [member function]
    cls.add_method('SetIntervalT7', 
                   'void', 
                   [param('ns3::Time', 'interval7')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetIntervalT7() const [member function]
    cls.add_method('GetIntervalT7', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetIntervalT12(ns3::Time interval12) [member function]
    cls.add_method('SetIntervalT12', 
                   'void', 
                   [param('ns3::Time', 'interval12')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetIntervalT12() const [member function]
    cls.add_method('GetIntervalT12', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetIntervalT20(ns3::Time interval20) [member function]
    cls.add_method('SetIntervalT20', 
                   'void', 
                   [param('ns3::Time', 'interval20')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetIntervalT20() const [member function]
    cls.add_method('GetIntervalT20', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetIntervalT21(ns3::Time interval21) [member function]
    cls.add_method('SetIntervalT21', 
                   'void', 
                   [param('ns3::Time', 'interval21')])
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetIntervalT21() const [member function]
    cls.add_method('GetIntervalT21', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetMaxContentionRangingRetries(uint8_t maxContentionRangingRetries) [member function]
    cls.add_method('SetMaxContentionRangingRetries', 
                   'void', 
                   [param('uint8_t', 'maxContentionRangingRetries')])
    ## ss-net-device.h: uint8_t ns3::SubscriberStationNetDevice::GetMaxContentionRangingRetries() const [member function]
    cls.add_method('GetMaxContentionRangingRetries', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetBasicConnection(ns3::Ptr<ns3::WimaxConnection> basicConnection) [member function]
    cls.add_method('SetBasicConnection', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxConnection >', 'basicConnection')])
    ## ss-net-device.h: ns3::Ptr<ns3::WimaxConnection> ns3::SubscriberStationNetDevice::GetBasicConnection() const [member function]
    cls.add_method('GetBasicConnection', 
                   'ns3::Ptr< ns3::WimaxConnection >', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetPrimaryConnection(ns3::Ptr<ns3::WimaxConnection> primaryConnection) [member function]
    cls.add_method('SetPrimaryConnection', 
                   'void', 
                   [param('ns3::Ptr< ns3::WimaxConnection >', 'primaryConnection')])
    ## ss-net-device.h: ns3::Ptr<ns3::WimaxConnection> ns3::SubscriberStationNetDevice::GetPrimaryConnection() const [member function]
    cls.add_method('GetPrimaryConnection', 
                   'ns3::Ptr< ns3::WimaxConnection >', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: ns3::Cid ns3::SubscriberStationNetDevice::GetBasicCid() const [member function]
    cls.add_method('GetBasicCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: ns3::Cid ns3::SubscriberStationNetDevice::GetPrimaryCid() const [member function]
    cls.add_method('GetPrimaryCid', 
                   'ns3::Cid', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetModulationType(ns3::WimaxPhy::ModulationType modulationType) [member function]
    cls.add_method('SetModulationType', 
                   'void', 
                   [param('ns3::WimaxPhy::ModulationType', 'modulationType')])
    ## ss-net-device.h: ns3::WimaxPhy::ModulationType ns3::SubscriberStationNetDevice::GetModulationType() const [member function]
    cls.add_method('GetModulationType', 
                   'ns3::WimaxPhy::ModulationType', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetAreManagementConnectionsAllocated(bool areManagementConnectionsAllocated) [member function]
    cls.add_method('SetAreManagementConnectionsAllocated', 
                   'void', 
                   [param('bool', 'areManagementConnectionsAllocated')])
    ## ss-net-device.h: bool ns3::SubscriberStationNetDevice::GetAreManagementConnectionsAllocated() const [member function]
    cls.add_method('GetAreManagementConnectionsAllocated', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetAreServiceFlowsAllocated(bool areServiceFlowsAllocated) [member function]
    cls.add_method('SetAreServiceFlowsAllocated', 
                   'void', 
                   [param('bool', 'areServiceFlowsAllocated')])
    ## ss-net-device.h: bool ns3::SubscriberStationNetDevice::GetAreServiceFlowsAllocated() const [member function]
    cls.add_method('GetAreServiceFlowsAllocated', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: ns3::Ptr<ns3::SSScheduler> ns3::SubscriberStationNetDevice::GetScheduler() const [member function]
    cls.add_method('GetScheduler', 
                   'ns3::Ptr< ns3::SSScheduler >', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetScheduler(ns3::Ptr<ns3::SSScheduler> ssScheduler) [member function]
    cls.add_method('SetScheduler', 
                   'void', 
                   [param('ns3::Ptr< ns3::SSScheduler >', 'ssScheduler')])
    ## ss-net-device.h: bool ns3::SubscriberStationNetDevice::HasServiceFlows() const [member function]
    cls.add_method('HasServiceFlows', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: bool ns3::SubscriberStationNetDevice::Enqueue(ns3::Ptr<ns3::Packet> packet, ns3::MacHeaderType const & hdrType, ns3::Ptr<ns3::WimaxConnection> connection) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::MacHeaderType const &', 'hdrType'), param('ns3::Ptr< ns3::WimaxConnection >', 'connection')], 
                   is_virtual=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SendBurst(uint8_t uiuc, uint16_t nrSymbols, ns3::Ptr<ns3::WimaxConnection> connection, ns3::MacHeaderType::HeaderType packetType=::ns3::MacHeaderType::HEADER_TYPE_GENERIC) [member function]
    cls.add_method('SendBurst', 
                   'void', 
                   [param('uint8_t', 'uiuc'), param('uint16_t', 'nrSymbols'), param('ns3::Ptr< ns3::WimaxConnection >', 'connection'), param('ns3::MacHeaderType::HeaderType', 'packetType', default_value='::ns3::MacHeaderType::HEADER_TYPE_GENERIC')])
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::AddServiceFlow(ns3::ServiceFlow * sf) [member function]
    cls.add_method('AddServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow *', 'sf')])
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::AddServiceFlow(ns3::ServiceFlow sf) [member function]
    cls.add_method('AddServiceFlow', 
                   'void', 
                   [param('ns3::ServiceFlow', 'sf')])
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetTimer(ns3::EventId eventId, ns3::EventId & event) [member function]
    cls.add_method('SetTimer', 
                   'void', 
                   [param('ns3::EventId', 'eventId'), param('ns3::EventId &', 'event')])
    ## ss-net-device.h: bool ns3::SubscriberStationNetDevice::IsRegistered() const [member function]
    cls.add_method('IsRegistered', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: ns3::Time ns3::SubscriberStationNetDevice::GetTimeToAllocation(ns3::Time defferTime) [member function]
    cls.add_method('GetTimeToAllocation', 
                   'ns3::Time', 
                   [param('ns3::Time', 'defferTime')])
    ## ss-net-device.h: ns3::Ptr<ns3::IpcsClassifier> ns3::SubscriberStationNetDevice::GetIpcsClassifier() const [member function]
    cls.add_method('GetIpcsClassifier', 
                   'ns3::Ptr< ns3::IpcsClassifier >', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetIpcsPacketClassifier(ns3::Ptr<ns3::IpcsClassifier> arg0) [member function]
    cls.add_method('SetIpcsPacketClassifier', 
                   'void', 
                   [param('ns3::Ptr< ns3::IpcsClassifier >', 'arg0')])
    ## ss-net-device.h: ns3::Ptr<ns3::SSLinkManager> ns3::SubscriberStationNetDevice::GetLinkManager() const [member function]
    cls.add_method('GetLinkManager', 
                   'ns3::Ptr< ns3::SSLinkManager >', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetLinkManager(ns3::Ptr<ns3::SSLinkManager> arg0) [member function]
    cls.add_method('SetLinkManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::SSLinkManager >', 'arg0')])
    ## ss-net-device.h: ns3::Ptr<ns3::SsServiceFlowManager> ns3::SubscriberStationNetDevice::GetServiceFlowManager() const [member function]
    cls.add_method('GetServiceFlowManager', 
                   'ns3::Ptr< ns3::SsServiceFlowManager >', 
                   [], 
                   is_const=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::SetServiceFlowManager(ns3::Ptr<ns3::SsServiceFlowManager> arg0) [member function]
    cls.add_method('SetServiceFlowManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::SsServiceFlowManager >', 'arg0')])
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## ss-net-device.h: bool ns3::SubscriberStationNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   visibility='private', is_virtual=True)
    ## ss-net-device.h: void ns3::SubscriberStationNetDevice::DoReceive(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')], 
                   visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    ## crc8.h: extern uint8_t ns3::CRC8Calculate(uint8_t const * data, int length) [free function]
    module.add_function('CRC8Calculate', 
                        'uint8_t', 
                        [param('uint8_t const *', 'data'), param('int', 'length')])
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

