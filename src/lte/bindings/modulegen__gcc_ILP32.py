from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers


import pybindgen.settings
import warnings

class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, wrapper, exception, traceback_):
        warnings.warn("exception %r in wrapper %s" % (exception, wrapper))
        return True
pybindgen.settings.error_handler = ErrorHandler()


import sys

def module_init():
    root_module = Module('ns.lte', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    root_module = module.get_root()
    
    ## address.h (module 'network'): ns3::Address [class]
    module.add_class('Address', import_from_module='ns.network')
    ## address.h (module 'network'): ns3::Address::MaxSize_e [enumeration]
    module.add_enum('MaxSize_e', ['MAX_SIZE'], outer_class=root_module['ns3::Address'], import_from_module='ns.network')
    ## eps-bearer.h (module 'lte'): ns3::AllocationRetentionPriority [struct]
    module.add_class('AllocationRetentionPriority')
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList [class]
    module.add_class('AttributeConstructionList', import_from_module='ns.core')
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item [struct]
    module.add_class('Item', import_from_module='ns.core', outer_class=root_module['ns3::AttributeConstructionList'])
    ## spectrum-model.h (module 'spectrum'): ns3::BandInfo [struct]
    module.add_class('BandInfo', import_from_module='ns.spectrum')
    ## buffer.h (module 'network'): ns3::Buffer [class]
    module.add_class('Buffer', import_from_module='ns.network')
    ## buffer.h (module 'network'): ns3::Buffer::Iterator [class]
    module.add_class('Iterator', import_from_module='ns.network', outer_class=root_module['ns3::Buffer'])
    ## lte-common.h (module 'lte'): ns3::BufferSizeLevelBsr [class]
    module.add_class('BufferSizeLevelBsr')
    ## packet.h (module 'network'): ns3::ByteTagIterator [class]
    module.add_class('ByteTagIterator', import_from_module='ns.network')
    ## packet.h (module 'network'): ns3::ByteTagIterator::Item [class]
    module.add_class('Item', import_from_module='ns.network', outer_class=root_module['ns3::ByteTagIterator'])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList [class]
    module.add_class('ByteTagList', import_from_module='ns.network')
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator [class]
    module.add_class('Iterator', import_from_module='ns.network', outer_class=root_module['ns3::ByteTagList'])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item [struct]
    module.add_class('Item', import_from_module='ns.network', outer_class=root_module['ns3::ByteTagList::Iterator'])
    ## callback.h (module 'core'): ns3::CallbackBase [class]
    module.add_class('CallbackBase', import_from_module='ns.core')
    ## data-output-interface.h (module 'stats'): ns3::DataOutputCallback [class]
    module.add_class('DataOutputCallback', allow_subclassing=True, import_from_module='ns.stats')
    ## data-rate.h (module 'network'): ns3::DataRate [class]
    module.add_class('DataRate', import_from_module='ns.network')
    ## eps-bearer.h (module 'lte'): ns3::EpsBearer [struct]
    module.add_class('EpsBearer')
    ## eps-bearer.h (module 'lte'): ns3::EpsBearer::Qci [enumeration]
    module.add_enum('Qci', ['GBR_CONV_VOICE', 'GBR_CONV_VIDEO', 'GBR_GAMING', 'GBR_NON_CONV_VIDEO', 'NGBR_IMS', 'NGBR_VIDEO_TCP_OPERATOR', 'NGBR_VOICE_VIDEO_GAMING', 'NGBR_VIDEO_TCP_PREMIUM', 'NGBR_VIDEO_TCP_DEFAULT'], outer_class=root_module['ns3::EpsBearer'])
    ## event-id.h (module 'core'): ns3::EventId [class]
    module.add_class('EventId', import_from_module='ns.core')
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider [class]
    module.add_class('FfMacCschedSapProvider', allow_subclassing=True)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters [struct]
    module.add_class('CschedCellConfigReqParameters', outer_class=root_module['ns3::FfMacCschedSapProvider'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::HoppingMode_e [enumeration]
    module.add_enum('HoppingMode_e', ['inter', 'interintra'], outer_class=root_module['ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::PhichResource_e [enumeration]
    module.add_enum('PhichResource_e', ['PHICH_R_ONE_SIXTH', 'PHICH_R_HALF', 'PHICH_R_ONE', 'PHICH_R_TWO'], outer_class=root_module['ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::DuplexMode_e [enumeration]
    module.add_enum('DuplexMode_e', ['DM_TDD', 'DM_FDD'], outer_class=root_module['ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::Enable64Qam_e [enumeration]
    module.add_enum('Enable64Qam_e', ['MOD_16QAM', 'MOD_64QAM'], outer_class=root_module['ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters [struct]
    module.add_class('CschedLcConfigReqParameters', outer_class=root_module['ns3::FfMacCschedSapProvider'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters [struct]
    module.add_class('CschedLcReleaseReqParameters', outer_class=root_module['ns3::FfMacCschedSapProvider'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters [struct]
    module.add_class('CschedUeConfigReqParameters', outer_class=root_module['ns3::FfMacCschedSapProvider'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::MeasGapConfigPattern_e [enumeration]
    module.add_enum('MeasGapConfigPattern_e', ['MGP_GP1', 'MGP_GP2', 'OFF'], outer_class=root_module['ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::OpenClosedLoop_e [enumeration]
    module.add_enum('OpenClosedLoop_e', ['noneloop', 'openloop', 'closedloop'], outer_class=root_module['ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::RepMode_e [enumeration]
    module.add_enum('RepMode_e', ['rm12', 'rm20', 'rm22', 'rm30', 'rm31', 'nonemode'], outer_class=root_module['ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::FeedbackMode_e [enumeration]
    module.add_enum('FeedbackMode_e', ['bundling', 'multiplexing'], outer_class=root_module['ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters [struct]
    module.add_class('CschedUeReleaseReqParameters', outer_class=root_module['ns3::FfMacCschedSapProvider'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser [class]
    module.add_class('FfMacCschedSapUser', allow_subclassing=True)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters [struct]
    module.add_class('CschedCellConfigCnfParameters', outer_class=root_module['ns3::FfMacCschedSapUser'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters [struct]
    module.add_class('CschedCellConfigUpdateIndParameters', outer_class=root_module['ns3::FfMacCschedSapUser'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters [struct]
    module.add_class('CschedLcConfigCnfParameters', outer_class=root_module['ns3::FfMacCschedSapUser'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters [struct]
    module.add_class('CschedLcReleaseCnfParameters', outer_class=root_module['ns3::FfMacCschedSapUser'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters [struct]
    module.add_class('CschedUeConfigCnfParameters', outer_class=root_module['ns3::FfMacCschedSapUser'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters [struct]
    module.add_class('CschedUeConfigUpdateIndParameters', outer_class=root_module['ns3::FfMacCschedSapUser'])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters [struct]
    module.add_class('CschedUeReleaseCnfParameters', outer_class=root_module['ns3::FfMacCschedSapUser'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider [class]
    module.add_class('FfMacSchedSapProvider', allow_subclassing=True)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters [struct]
    module.add_class('SchedDlCqiInfoReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters [struct]
    module.add_class('SchedDlMacBufferReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters [struct]
    module.add_class('SchedDlPagingBufferReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters [struct]
    module.add_class('SchedDlRachInfoReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters [struct]
    module.add_class('SchedDlRlcBufferReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters [struct]
    module.add_class('SchedDlTriggerReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters [struct]
    module.add_class('SchedUlCqiInfoReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters [struct]
    module.add_class('SchedUlMacCtrlInfoReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters [struct]
    module.add_class('SchedUlNoiseInterferenceReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters [struct]
    module.add_class('SchedUlSrInfoReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters [struct]
    module.add_class('SchedUlTriggerReqParameters', outer_class=root_module['ns3::FfMacSchedSapProvider'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser [class]
    module.add_class('FfMacSchedSapUser', allow_subclassing=True)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedDlConfigIndParameters [struct]
    module.add_class('SchedDlConfigIndParameters', outer_class=root_module['ns3::FfMacSchedSapUser'])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedUlConfigIndParameters [struct]
    module.add_class('SchedUlConfigIndParameters', outer_class=root_module['ns3::FfMacSchedSapUser'])
    ## eps-bearer.h (module 'lte'): ns3::GbrQosInformation [struct]
    module.add_class('GbrQosInformation')
    ## lte-common.h (module 'lte'): ns3::ImsiLcidPair_t [struct]
    module.add_class('ImsiLcidPair_t')
    ## int-to-type.h (module 'core'): ns3::IntToType<0> [struct]
    module.add_class('IntToType', import_from_module='ns.core', template_parameters=['0'])
    ## int-to-type.h (module 'core'): ns3::IntToType<0>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 0 >'], import_from_module='ns.core')
    ## int-to-type.h (module 'core'): ns3::IntToType<1> [struct]
    module.add_class('IntToType', import_from_module='ns.core', template_parameters=['1'])
    ## int-to-type.h (module 'core'): ns3::IntToType<1>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 1 >'], import_from_module='ns.core')
    ## int-to-type.h (module 'core'): ns3::IntToType<2> [struct]
    module.add_class('IntToType', import_from_module='ns.core', template_parameters=['2'])
    ## int-to-type.h (module 'core'): ns3::IntToType<2>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 2 >'], import_from_module='ns.core')
    ## int-to-type.h (module 'core'): ns3::IntToType<3> [struct]
    module.add_class('IntToType', import_from_module='ns.core', template_parameters=['3'])
    ## int-to-type.h (module 'core'): ns3::IntToType<3>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 3 >'], import_from_module='ns.core')
    ## int-to-type.h (module 'core'): ns3::IntToType<4> [struct]
    module.add_class('IntToType', import_from_module='ns.core', template_parameters=['4'])
    ## int-to-type.h (module 'core'): ns3::IntToType<4>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 4 >'], import_from_module='ns.core')
    ## int-to-type.h (module 'core'): ns3::IntToType<5> [struct]
    module.add_class('IntToType', import_from_module='ns.core', template_parameters=['5'])
    ## int-to-type.h (module 'core'): ns3::IntToType<5>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 5 >'], import_from_module='ns.core')
    ## int-to-type.h (module 'core'): ns3::IntToType<6> [struct]
    module.add_class('IntToType', import_from_module='ns.core', template_parameters=['6'])
    ## int-to-type.h (module 'core'): ns3::IntToType<6>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 6 >'], import_from_module='ns.core')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address [class]
    module.add_class('Ipv4Address', import_from_module='ns.network')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address [class]
    root_module['ns3::Ipv4Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv4-address-helper.h (module 'internet'): ns3::Ipv4AddressHelper [class]
    module.add_class('Ipv4AddressHelper', import_from_module='ns.internet')
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4InterfaceAddress [class]
    module.add_class('Ipv4InterfaceAddress', import_from_module='ns.internet')
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e [enumeration]
    module.add_enum('InterfaceAddressScope_e', ['HOST', 'LINK', 'GLOBAL'], outer_class=root_module['ns3::Ipv4InterfaceAddress'], import_from_module='ns.internet')
    ## ipv4-interface-container.h (module 'internet'): ns3::Ipv4InterfaceContainer [class]
    module.add_class('Ipv4InterfaceContainer', import_from_module='ns.internet')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask [class]
    module.add_class('Ipv4Mask', import_from_module='ns.network')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address [class]
    module.add_class('Ipv6Address', import_from_module='ns.network')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address [class]
    root_module['ns3::Ipv6Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress [class]
    module.add_class('Ipv6InterfaceAddress', import_from_module='ns.internet')
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress::State_e [enumeration]
    module.add_enum('State_e', ['TENTATIVE', 'DEPRECATED', 'PREFERRED', 'PERMANENT', 'HOMEADDRESS', 'TENTATIVE_OPTIMISTIC', 'INVALID'], outer_class=root_module['ns3::Ipv6InterfaceAddress'], import_from_module='ns.internet')
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress::Scope_e [enumeration]
    module.add_enum('Scope_e', ['HOST', 'LINKLOCAL', 'GLOBAL'], outer_class=root_module['ns3::Ipv6InterfaceAddress'], import_from_module='ns.internet')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix [class]
    module.add_class('Ipv6Prefix', import_from_module='ns.network')
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider [class]
    module.add_class('LteEnbCmacSapProvider', allow_subclassing=True)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo [struct]
    module.add_class('LcInfo', outer_class=root_module['ns3::LteEnbCmacSapProvider'])
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapUser [class]
    module.add_class('LteEnbCmacSapUser', allow_subclassing=True)
    ## lte-enb-phy-sap.h (module 'lte'): ns3::LteEnbPhySapProvider [class]
    module.add_class('LteEnbPhySapProvider', allow_subclassing=True)
    ## lte-enb-phy-sap.h (module 'lte'): ns3::LteEnbPhySapUser [class]
    module.add_class('LteEnbPhySapUser', allow_subclassing=True)
    ## lte-common.h (module 'lte'): ns3::LteFfConverter [class]
    module.add_class('LteFfConverter')
    ## lte-common.h (module 'lte'): ns3::LteFlowId_t [struct]
    module.add_class('LteFlowId_t')
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider [class]
    module.add_class('LteMacSapProvider', allow_subclassing=True)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters [struct]
    module.add_class('ReportBufferStatusParameters', outer_class=root_module['ns3::LteMacSapProvider'])
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::TransmitPduParameters [struct]
    module.add_class('TransmitPduParameters', outer_class=root_module['ns3::LteMacSapProvider'])
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapUser [class]
    module.add_class('LteMacSapUser', allow_subclassing=True)
    ## lte-mi-error-model.h (module 'lte'): ns3::LteMiErrorModel [class]
    module.add_class('LteMiErrorModel')
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider [class]
    module.add_class('LtePdcpSapProvider', allow_subclassing=True)
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider::TransmitRrcPduParameters [struct]
    module.add_class('TransmitRrcPduParameters', outer_class=root_module['ns3::LtePdcpSapProvider'])
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser [class]
    module.add_class('LtePdcpSapUser', allow_subclassing=True)
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser::ReceiveRrcPduParameters [struct]
    module.add_class('ReceiveRrcPduParameters', outer_class=root_module['ns3::LtePdcpSapUser'])
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider [class]
    module.add_class('LteRlcSapProvider', allow_subclassing=True)
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider::TransmitPdcpPduParameters [struct]
    module.add_class('TransmitPdcpPduParameters', outer_class=root_module['ns3::LteRlcSapProvider'])
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapUser [class]
    module.add_class('LteRlcSapUser', allow_subclassing=True)
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::LteSpectrumValueHelper [class]
    module.add_class('LteSpectrumValueHelper')
    ## lte-ue-cmac-sap.h (module 'lte'): ns3::LteUeCmacSapProvider [class]
    module.add_class('LteUeCmacSapProvider', allow_subclassing=True)
    ## lte-ue-cmac-sap.h (module 'lte'): ns3::LteUeCmacSapUser [class]
    module.add_class('LteUeCmacSapUser', allow_subclassing=True)
    ## lte-common.h (module 'lte'): ns3::LteUeConfig_t [struct]
    module.add_class('LteUeConfig_t')
    ## lte-ue-phy-sap.h (module 'lte'): ns3::LteUePhySapProvider [class]
    module.add_class('LteUePhySapProvider', allow_subclassing=True)
    ## lte-ue-phy-sap.h (module 'lte'): ns3::LteUePhySapUser [class]
    module.add_class('LteUePhySapUser', allow_subclassing=True)
    ## mac48-address.h (module 'network'): ns3::Mac48Address [class]
    module.add_class('Mac48Address', import_from_module='ns.network')
    ## mac48-address.h (module 'network'): ns3::Mac48Address [class]
    root_module['ns3::Mac48Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## names.h (module 'core'): ns3::Names [class]
    module.add_class('Names', import_from_module='ns.core')
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer [class]
    module.add_class('NetDeviceContainer', import_from_module='ns.network')
    ## node-container.h (module 'network'): ns3::NodeContainer [class]
    module.add_class('NodeContainer', import_from_module='ns.network')
    ## object-base.h (module 'core'): ns3::ObjectBase [class]
    module.add_class('ObjectBase', allow_subclassing=True, import_from_module='ns.core')
    ## object.h (module 'core'): ns3::ObjectDeleter [struct]
    module.add_class('ObjectDeleter', import_from_module='ns.core')
    ## object-factory.h (module 'core'): ns3::ObjectFactory [class]
    module.add_class('ObjectFactory', import_from_module='ns.core')
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata [class]
    module.add_class('PacketMetadata', import_from_module='ns.network')
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item [struct]
    module.add_class('Item', import_from_module='ns.network', outer_class=root_module['ns3::PacketMetadata'])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item [enumeration]
    module.add_enum('', ['PAYLOAD', 'HEADER', 'TRAILER'], outer_class=root_module['ns3::PacketMetadata::Item'], import_from_module='ns.network')
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::ItemIterator [class]
    module.add_class('ItemIterator', import_from_module='ns.network', outer_class=root_module['ns3::PacketMetadata'])
    ## packet.h (module 'network'): ns3::PacketTagIterator [class]
    module.add_class('PacketTagIterator', import_from_module='ns.network')
    ## packet.h (module 'network'): ns3::PacketTagIterator::Item [class]
    module.add_class('Item', import_from_module='ns.network', outer_class=root_module['ns3::PacketTagIterator'])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList [class]
    module.add_class('PacketTagList', import_from_module='ns.network')
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData [struct]
    module.add_class('TagData', import_from_module='ns.network', outer_class=root_module['ns3::PacketTagList'])
    ## lte-rlc-sequence-number.h (module 'lte'): ns3::SequenceNumber10 [class]
    module.add_class('SequenceNumber10')
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Object', 'ns3::ObjectBase', 'ns3::ObjectDeleter'], parent=root_module['ns3::ObjectBase'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simulator.h (module 'core'): ns3::Simulator [class]
    module.add_class('Simulator', destructor_visibility='private', import_from_module='ns.core')
    ## data-calculator.h (module 'stats'): ns3::StatisticalSummary [class]
    module.add_class('StatisticalSummary', allow_subclassing=True, import_from_module='ns.stats')
    ## tag.h (module 'network'): ns3::Tag [class]
    module.add_class('Tag', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## tag-buffer.h (module 'network'): ns3::TagBuffer [class]
    module.add_class('TagBuffer', import_from_module='ns.network')
    ## lte-spectrum-phy.h (module 'lte'): ns3::TbId_t [struct]
    module.add_class('TbId_t')
    ## timer.h (module 'core'): ns3::Timer [class]
    module.add_class('Timer', import_from_module='ns.core')
    ## timer.h (module 'core'): ns3::Timer::DestroyPolicy [enumeration]
    module.add_enum('DestroyPolicy', ['CANCEL_ON_DESTROY', 'REMOVE_ON_DESTROY', 'CHECK_ON_DESTROY'], outer_class=root_module['ns3::Timer'], import_from_module='ns.core')
    ## timer.h (module 'core'): ns3::Timer::State [enumeration]
    module.add_enum('State', ['RUNNING', 'EXPIRED', 'SUSPENDED'], outer_class=root_module['ns3::Timer'], import_from_module='ns.core')
    ## timer-impl.h (module 'core'): ns3::TimerImpl [class]
    module.add_class('TimerImpl', allow_subclassing=True, import_from_module='ns.core')
    ## lte-common.h (module 'lte'): ns3::TransmissionModesLayers [class]
    module.add_class('TransmissionModesLayers')
    ## type-id.h (module 'core'): ns3::TypeId [class]
    module.add_class('TypeId', import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId::AttributeFlag [enumeration]
    module.add_enum('AttributeFlag', ['ATTR_GET', 'ATTR_SET', 'ATTR_CONSTRUCT', 'ATTR_SGC'], outer_class=root_module['ns3::TypeId'], import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation [struct]
    module.add_class('AttributeInformation', import_from_module='ns.core', outer_class=root_module['ns3::TypeId'])
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation [struct]
    module.add_class('TraceSourceInformation', import_from_module='ns.core', outer_class=root_module['ns3::TypeId'])
    ## vector.h (module 'core'): ns3::Vector2D [class]
    module.add_class('Vector2D', import_from_module='ns.core')
    ## vector.h (module 'core'): ns3::Vector3D [class]
    module.add_class('Vector3D', import_from_module='ns.core')
    ## empty.h (module 'core'): ns3::empty [class]
    module.add_class('empty', import_from_module='ns.core')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t [class]
    module.add_class('int64x64_t', import_from_module='ns.core')
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::pfsFlowPerf_t [struct]
    module.add_class('pfsFlowPerf_t')
    ## lte-spectrum-phy.h (module 'lte'): ns3::tbInfo_t [struct]
    module.add_class('tbInfo_t')
    ## chunk.h (module 'network'): ns3::Chunk [class]
    module.add_class('Chunk', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## header.h (module 'network'): ns3::Header [class]
    module.add_class('Header', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Header [class]
    module.add_class('Ipv4Header', import_from_module='ns.internet', parent=root_module['ns3::Header'])
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Header::DscpType [enumeration]
    module.add_enum('DscpType', ['DscpDefault', 'CS1', 'AF11', 'AF12', 'AF13', 'CS2', 'AF21', 'AF22', 'AF23', 'CS3', 'AF31', 'AF32', 'AF33', 'CS4', 'AF41', 'AF42', 'AF43', 'CS5', 'EF', 'CS6', 'CS7'], outer_class=root_module['ns3::Ipv4Header'], import_from_module='ns.internet')
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Header::EcnType [enumeration]
    module.add_enum('EcnType', ['NotECT', 'ECT1', 'ECT0', 'CE'], outer_class=root_module['ns3::Ipv4Header'], import_from_module='ns.internet')
    ## ipv6-header.h (module 'internet'): ns3::Ipv6Header [class]
    module.add_class('Ipv6Header', import_from_module='ns.internet', parent=root_module['ns3::Header'])
    ## ipv6-header.h (module 'internet'): ns3::Ipv6Header::NextHeader_e [enumeration]
    module.add_enum('NextHeader_e', ['IPV6_EXT_HOP_BY_HOP', 'IPV6_IPV4', 'IPV6_TCP', 'IPV6_UDP', 'IPV6_IPV6', 'IPV6_EXT_ROUTING', 'IPV6_EXT_FRAGMENTATION', 'IPV6_EXT_CONFIDENTIALITY', 'IPV6_EXT_AUTHENTIFICATION', 'IPV6_ICMPV6', 'IPV6_EXT_END', 'IPV6_EXT_DESTINATION', 'IPV6_SCTP', 'IPV6_EXT_MOBILITY', 'IPV6_UDP_LITE'], outer_class=root_module['ns3::Ipv6Header'], import_from_module='ns.internet')
    ## lte-pdcp-header.h (module 'lte'): ns3::LtePdcpHeader [class]
    module.add_class('LtePdcpHeader', parent=root_module['ns3::Header'])
    ## lte-pdcp-header.h (module 'lte'): ns3::LtePdcpHeader::DcBit_t [enumeration]
    module.add_enum('DcBit_t', ['CONTROL_PDU', 'DATA_PDU'], outer_class=root_module['ns3::LtePdcpHeader'])
    ## lte-phy-tag.h (module 'lte'): ns3::LtePhyTag [class]
    module.add_class('LtePhyTag', parent=root_module['ns3::Tag'])
    ## lte-radio-bearer-tag.h (module 'lte'): ns3::LteRadioBearerTag [class]
    module.add_class('LteRadioBearerTag', parent=root_module['ns3::Tag'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader [class]
    module.add_class('LteRlcAmHeader', parent=root_module['ns3::Header'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::DataControlPdu_t [enumeration]
    module.add_enum('DataControlPdu_t', ['CONTROL_PDU', 'DATA_PDU'], outer_class=root_module['ns3::LteRlcAmHeader'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::ControPduType_t [enumeration]
    module.add_enum('ControPduType_t', ['STATUS_PDU'], outer_class=root_module['ns3::LteRlcAmHeader'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::FramingInfoFirstByte_t [enumeration]
    module.add_enum('FramingInfoFirstByte_t', ['FIRST_BYTE', 'NO_FIRST_BYTE'], outer_class=root_module['ns3::LteRlcAmHeader'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::FramingInfoLastByte_t [enumeration]
    module.add_enum('FramingInfoLastByte_t', ['LAST_BYTE', 'NO_LAST_BYTE'], outer_class=root_module['ns3::LteRlcAmHeader'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::ExtensionBit_t [enumeration]
    module.add_enum('ExtensionBit_t', ['DATA_FIELD_FOLLOWS', 'E_LI_FIELDS_FOLLOWS'], outer_class=root_module['ns3::LteRlcAmHeader'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::ResegmentationFlag_t [enumeration]
    module.add_enum('ResegmentationFlag_t', ['PDU', 'SEGMENT'], outer_class=root_module['ns3::LteRlcAmHeader'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::PollingBit_t [enumeration]
    module.add_enum('PollingBit_t', ['STATUS_REPORT_NOT_REQUESTED', 'STATUS_REPORT_IS_REQUESTED'], outer_class=root_module['ns3::LteRlcAmHeader'])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::LastSegmentFlag_t [enumeration]
    module.add_enum('LastSegmentFlag_t', ['NO_LAST_PDU_SEGMENT', 'LAST_PDU_SEGMENT'], outer_class=root_module['ns3::LteRlcAmHeader'])
    ## lte-rlc-header.h (module 'lte'): ns3::LteRlcHeader [class]
    module.add_class('LteRlcHeader', parent=root_module['ns3::Header'])
    ## lte-rlc-header.h (module 'lte'): ns3::LteRlcHeader::ExtensionBit_t [enumeration]
    module.add_enum('ExtensionBit_t', ['DATA_FIELD_FOLLOWS', 'E_LI_FIELDS_FOLLOWS'], outer_class=root_module['ns3::LteRlcHeader'])
    ## lte-rlc-header.h (module 'lte'): ns3::LteRlcHeader::FramingInfoFirstByte_t [enumeration]
    module.add_enum('FramingInfoFirstByte_t', ['FIRST_BYTE', 'NO_FIRST_BYTE'], outer_class=root_module['ns3::LteRlcHeader'])
    ## lte-rlc-header.h (module 'lte'): ns3::LteRlcHeader::FramingInfoLastByte_t [enumeration]
    module.add_enum('FramingInfoLastByte_t', ['LAST_BYTE', 'NO_LAST_BYTE'], outer_class=root_module['ns3::LteRlcHeader'])
    ## lte-rlc-sdu-status-tag.h (module 'lte'): ns3::LteRlcSduStatusTag [class]
    module.add_class('LteRlcSduStatusTag', parent=root_module['ns3::Tag'])
    ## lte-rlc-sdu-status-tag.h (module 'lte'): ns3::LteRlcSduStatusTag::SduStatus_t [enumeration]
    module.add_enum('SduStatus_t', ['FULL_SDU', 'FIRST_SEGMENT', 'MIDDLE_SEGMENT', 'LAST_SEGMENT', 'ANY_SEGMENT'], outer_class=root_module['ns3::LteRlcSduStatusTag'])
    ## object.h (module 'core'): ns3::Object [class]
    module.add_class('Object', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    ## object.h (module 'core'): ns3::Object::AggregateIterator [class]
    module.add_class('AggregateIterator', import_from_module='ns.core', outer_class=root_module['ns3::Object'])
    ## packet-burst.h (module 'network'): ns3::PacketBurst [class]
    module.add_class('PacketBurst', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## lte-pdcp-tag.h (module 'lte'): ns3::PdcpTag [class]
    module.add_class('PdcpTag', parent=root_module['ns3::Tag'])
    ## radio-environment-map-helper.h (module 'lte'): ns3::RadioEnvironmentMapHelper [class]
    module.add_class('RadioEnvironmentMapHelper', parent=root_module['ns3::Object'])
    ## random-variable-stream.h (module 'core'): ns3::RandomVariableStream [class]
    module.add_class('RandomVariableStream', import_from_module='ns.core', parent=root_module['ns3::Object'])
    ## lte-rlc-tag.h (module 'lte'): ns3::RlcTag [class]
    module.add_class('RlcTag', parent=root_module['ns3::Tag'])
    ## random-variable-stream.h (module 'core'): ns3::SequentialRandomVariable [class]
    module.add_class('SequentialRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeChecker', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeChecker>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::CallbackImplBase', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CallbackImplBase>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EpcTft, ns3::empty, ns3::DefaultDeleter<ns3::EpcTft> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::EpcTft', 'ns3::empty', 'ns3::DefaultDeleter<ns3::EpcTft>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EpcTftClassifier, ns3::empty, ns3::DefaultDeleter<ns3::EpcTftClassifier> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::EpcTftClassifier', 'ns3::empty', 'ns3::DefaultDeleter<ns3::EpcTftClassifier>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::EventImpl', 'ns3::empty', 'ns3::DefaultDeleter<ns3::EventImpl>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::IdealControlMessage', 'ns3::empty', 'ns3::DefaultDeleter<ns3::IdealControlMessage>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Ipv4MulticastRoute', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv4MulticastRoute>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Ipv4Route', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Ipv4Route>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::LteSinrChunkProcessor, ns3::empty, ns3::DefaultDeleter<ns3::LteSinrChunkProcessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::LteSinrChunkProcessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::LteSinrChunkProcessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::NixVector', 'ns3::empty', 'ns3::DefaultDeleter<ns3::NixVector>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Packet', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Packet>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::SpectrumModel', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumModel>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumSignalParameters, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumSignalParameters> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::SpectrumSignalParameters', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumSignalParameters>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::SpectrumValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::TraceSourceAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::TraceSourceAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## socket.h (module 'network'): ns3::Socket [class]
    module.add_class('Socket', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## socket.h (module 'network'): ns3::Socket::SocketErrno [enumeration]
    module.add_enum('SocketErrno', ['ERROR_NOTERROR', 'ERROR_ISCONN', 'ERROR_NOTCONN', 'ERROR_MSGSIZE', 'ERROR_AGAIN', 'ERROR_SHUTDOWN', 'ERROR_OPNOTSUPP', 'ERROR_AFNOSUPPORT', 'ERROR_INVAL', 'ERROR_BADF', 'ERROR_NOROUTETOHOST', 'ERROR_NODEV', 'ERROR_ADDRNOTAVAIL', 'ERROR_ADDRINUSE', 'SOCKET_ERRNO_LAST'], outer_class=root_module['ns3::Socket'], import_from_module='ns.network')
    ## socket.h (module 'network'): ns3::Socket::SocketType [enumeration]
    module.add_enum('SocketType', ['NS3_SOCK_STREAM', 'NS3_SOCK_SEQPACKET', 'NS3_SOCK_DGRAM', 'NS3_SOCK_RAW'], outer_class=root_module['ns3::Socket'], import_from_module='ns.network')
    ## socket.h (module 'network'): ns3::SocketAddressTag [class]
    module.add_class('SocketAddressTag', import_from_module='ns.network', parent=root_module['ns3::Tag'])
    ## socket.h (module 'network'): ns3::SocketIpTtlTag [class]
    module.add_class('SocketIpTtlTag', import_from_module='ns.network', parent=root_module['ns3::Tag'])
    ## socket.h (module 'network'): ns3::SocketSetDontFragmentTag [class]
    module.add_class('SocketSetDontFragmentTag', import_from_module='ns.network', parent=root_module['ns3::Tag'])
    ## spectrum-interference.h (module 'spectrum'): ns3::SpectrumInterference [class]
    module.add_class('SpectrumInterference', import_from_module='ns.spectrum', parent=root_module['ns3::Object'])
    ## spectrum-model.h (module 'spectrum'): ns3::SpectrumModel [class]
    module.add_class('SpectrumModel', import_from_module='ns.spectrum', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >'])
    ## spectrum-phy.h (module 'spectrum'): ns3::SpectrumPhy [class]
    module.add_class('SpectrumPhy', import_from_module='ns.spectrum', parent=root_module['ns3::Object'])
    ## spectrum-propagation-loss-model.h (module 'spectrum'): ns3::SpectrumPropagationLossModel [class]
    module.add_class('SpectrumPropagationLossModel', import_from_module='ns.spectrum', parent=root_module['ns3::Object'])
    ## spectrum-signal-parameters.h (module 'spectrum'): ns3::SpectrumSignalParameters [struct]
    module.add_class('SpectrumSignalParameters', import_from_module='ns.spectrum', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumSignalParameters, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumSignalParameters> >'])
    ## spectrum-value.h (module 'spectrum'): ns3::SpectrumValue [class]
    module.add_class('SpectrumValue', import_from_module='ns.spectrum', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >'])
    ## nstime.h (module 'core'): ns3::Time [class]
    module.add_class('Time', import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time::Unit [enumeration]
    module.add_enum('Unit', ['S', 'MS', 'US', 'NS', 'PS', 'FS', 'LAST'], outer_class=root_module['ns3::Time'], import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time [class]
    root_module['ns3::Time'].implicitly_converts_to(root_module['ns3::int64x64_t'])
    ## trace-fading-loss-model.h (module 'lte'): ns3::TraceFadingLossModel [class]
    module.add_class('TraceFadingLossModel', parent=root_module['ns3::SpectrumPropagationLossModel'])
    ## trace-source-accessor.h (module 'core'): ns3::TraceSourceAccessor [class]
    module.add_class('TraceSourceAccessor', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    ## trailer.h (module 'network'): ns3::Trailer [class]
    module.add_class('Trailer', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## random-variable-stream.h (module 'core'): ns3::TriangularRandomVariable [class]
    module.add_class('TriangularRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## lte-enb-rrc.h (module 'lte'): ns3::UeInfo [class]
    module.add_class('UeInfo', parent=root_module['ns3::Object'])
    ## random-variable-stream.h (module 'core'): ns3::UniformRandomVariable [class]
    module.add_class('UniformRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## random-variable-stream.h (module 'core'): ns3::WeibullRandomVariable [class]
    module.add_class('WeibullRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## random-variable-stream.h (module 'core'): ns3::ZetaRandomVariable [class]
    module.add_class('ZetaRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## random-variable-stream.h (module 'core'): ns3::ZipfRandomVariable [class]
    module.add_class('ZipfRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## application.h (module 'network'): ns3::Application [class]
    module.add_class('Application', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## attribute.h (module 'core'): ns3::AttributeAccessor [class]
    module.add_class('AttributeAccessor', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    ## attribute.h (module 'core'): ns3::AttributeChecker [class]
    module.add_class('AttributeChecker', allow_subclassing=False, automatic_type_narrowing=True, import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    ## attribute.h (module 'core'): ns3::AttributeValue [class]
    module.add_class('AttributeValue', allow_subclassing=False, automatic_type_narrowing=True, import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    ## boolean.h (module 'core'): ns3::BooleanChecker [class]
    module.add_class('BooleanChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## boolean.h (module 'core'): ns3::BooleanValue [class]
    module.add_class('BooleanValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## callback.h (module 'core'): ns3::CallbackChecker [class]
    module.add_class('CallbackChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## callback.h (module 'core'): ns3::CallbackImplBase [class]
    module.add_class('CallbackImplBase', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    ## callback.h (module 'core'): ns3::CallbackValue [class]
    module.add_class('CallbackValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## channel.h (module 'network'): ns3::Channel [class]
    module.add_class('Channel', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## random-variable-stream.h (module 'core'): ns3::ConstantRandomVariable [class]
    module.add_class('ConstantRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## data-calculator.h (module 'stats'): ns3::DataCalculator [class]
    module.add_class('DataCalculator', import_from_module='ns.stats', parent=root_module['ns3::Object'])
    ## data-output-interface.h (module 'stats'): ns3::DataOutputInterface [class]
    module.add_class('DataOutputInterface', import_from_module='ns.stats', parent=root_module['ns3::Object'])
    ## data-rate.h (module 'network'): ns3::DataRateChecker [class]
    module.add_class('DataRateChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## data-rate.h (module 'network'): ns3::DataRateValue [class]
    module.add_class('DataRateValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## random-variable-stream.h (module 'core'): ns3::DeterministicRandomVariable [class]
    module.add_class('DeterministicRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## double.h (module 'core'): ns3::DoubleValue [class]
    module.add_class('DoubleValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## random-variable-stream.h (module 'core'): ns3::EmpiricalRandomVariable [class]
    module.add_class('EmpiricalRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue [class]
    module.add_class('EmptyAttributeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## enum.h (module 'core'): ns3::EnumChecker [class]
    module.add_class('EnumChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## enum.h (module 'core'): ns3::EnumValue [class]
    module.add_class('EnumValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## epc-enb-application.h (module 'lte'): ns3::EpcEnbApplication [class]
    module.add_class('EpcEnbApplication', parent=root_module['ns3::Application'])
    ## epc-helper.h (module 'lte'): ns3::EpcHelper [class]
    module.add_class('EpcHelper', parent=root_module['ns3::Object'])
    ## epc-sgw-pgw-application.h (module 'lte'): ns3::EpcSgwPgwApplication [class]
    module.add_class('EpcSgwPgwApplication', parent=root_module['ns3::Application'])
    ## epc-tft.h (module 'lte'): ns3::EpcTft [class]
    module.add_class('EpcTft', parent=root_module['ns3::SimpleRefCount< ns3::EpcTft, ns3::empty, ns3::DefaultDeleter<ns3::EpcTft> >'])
    ## epc-tft.h (module 'lte'): ns3::EpcTft::Direction [enumeration]
    module.add_enum('Direction', ['DOWNLINK', 'UPLINK', 'BIDIRECTIONAL'], outer_class=root_module['ns3::EpcTft'])
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter [struct]
    module.add_class('PacketFilter', outer_class=root_module['ns3::EpcTft'])
    ## epc-tft-classifier.h (module 'lte'): ns3::EpcTftClassifier [class]
    module.add_class('EpcTftClassifier', parent=root_module['ns3::SimpleRefCount< ns3::EpcTftClassifier, ns3::empty, ns3::DefaultDeleter<ns3::EpcTftClassifier> >'])
    ## random-variable-stream.h (module 'core'): ns3::ErlangRandomVariable [class]
    module.add_class('ErlangRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## event-impl.h (module 'core'): ns3::EventImpl [class]
    module.add_class('EventImpl', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    ## random-variable-stream.h (module 'core'): ns3::ExponentialRandomVariable [class]
    module.add_class('ExponentialRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## ff-mac-scheduler.h (module 'lte'): ns3::FfMacScheduler [class]
    module.add_class('FfMacScheduler', parent=root_module['ns3::Object'])
    ## random-variable-stream.h (module 'core'): ns3::GammaRandomVariable [class]
    module.add_class('GammaRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## epc-gtpu-header.h (module 'lte'): ns3::GtpuHeader [class]
    module.add_class('GtpuHeader', parent=root_module['ns3::Header'])
    ## ideal-control-messages.h (module 'lte'): ns3::IdealControlMessage [class]
    module.add_class('IdealControlMessage', parent=root_module['ns3::SimpleRefCount< ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >'])
    ## ideal-control-messages.h (module 'lte'): ns3::IdealControlMessage::MessageType [enumeration]
    module.add_enum('MessageType', ['CQI_FEEDBACKS', 'ALLOCATION_MAP', 'DL_DCI', 'UL_DCI', 'DL_CQI', 'UL_CQI', 'BSR'], outer_class=root_module['ns3::IdealControlMessage'])
    ## integer.h (module 'core'): ns3::IntegerValue [class]
    module.add_class('IntegerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## ip-l4-protocol.h (module 'internet'): ns3::IpL4Protocol [class]
    module.add_class('IpL4Protocol', import_from_module='ns.internet', parent=root_module['ns3::Object'])
    ## ip-l4-protocol.h (module 'internet'): ns3::IpL4Protocol::RxStatus [enumeration]
    module.add_enum('RxStatus', ['RX_OK', 'RX_CSUM_FAILED', 'RX_ENDPOINT_CLOSED', 'RX_ENDPOINT_UNREACH'], outer_class=root_module['ns3::IpL4Protocol'], import_from_module='ns.internet')
    ## ipv4.h (module 'internet'): ns3::Ipv4 [class]
    module.add_class('Ipv4', import_from_module='ns.internet', parent=root_module['ns3::Object'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressChecker [class]
    module.add_class('Ipv4AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue [class]
    module.add_class('Ipv4AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskChecker [class]
    module.add_class('Ipv4MaskChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue [class]
    module.add_class('Ipv4MaskValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv4-route.h (module 'internet'): ns3::Ipv4MulticastRoute [class]
    module.add_class('Ipv4MulticastRoute', import_from_module='ns.internet', parent=root_module['ns3::SimpleRefCount< ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >'])
    ## ipv4-route.h (module 'internet'): ns3::Ipv4Route [class]
    module.add_class('Ipv4Route', import_from_module='ns.internet', parent=root_module['ns3::SimpleRefCount< ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressChecker [class]
    module.add_class('Ipv6AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue [class]
    module.add_class('Ipv6AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv6-interface.h (module 'internet'): ns3::Ipv6Interface [class]
    module.add_class('Ipv6Interface', import_from_module='ns.internet', parent=root_module['ns3::Object'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixChecker [class]
    module.add_class('Ipv6PrefixChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue [class]
    module.add_class('Ipv6PrefixValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## random-variable-stream.h (module 'core'): ns3::LogNormalRandomVariable [class]
    module.add_class('LogNormalRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## lte-amc.h (module 'lte'): ns3::LteAmc [class]
    module.add_class('LteAmc', parent=root_module['ns3::Object'])
    ## lte-amc.h (module 'lte'): ns3::LteAmc::AmcModel [enumeration]
    module.add_enum('AmcModel', ['PiroEW2010', 'MiErrorModel'], outer_class=root_module['ns3::LteAmc'])
    ## lte-enb-mac.h (module 'lte'): ns3::LteEnbMac [class]
    module.add_class('LteEnbMac', parent=root_module['ns3::Object'])
    ## lte-enb-rrc.h (module 'lte'): ns3::LteEnbRrc [class]
    module.add_class('LteEnbRrc', parent=root_module['ns3::Object'])
    ## lte-helper.h (module 'lte'): ns3::LteHelper [class]
    module.add_class('LteHelper', parent=root_module['ns3::Object'])
    ## lte-helper.h (module 'lte'): ns3::LteHelper::LteEpsBearerToRlcMapping_t [enumeration]
    module.add_enum('LteEpsBearerToRlcMapping_t', ['RLC_SM_ALWAYS', 'RLC_UM_ALWAYS', 'RLC_AM_ALWAYS', 'PER_BASED'], outer_class=root_module['ns3::LteHelper'])
    ## lte-hex-grid-enb-topology-helper.h (module 'lte'): ns3::LteHexGridEnbTopologyHelper [class]
    module.add_class('LteHexGridEnbTopologyHelper', parent=root_module['ns3::Object'])
    ## lte-interference.h (module 'lte'): ns3::LteInterference [class]
    module.add_class('LteInterference', parent=root_module['ns3::Object'])
    ## lte-pdcp.h (module 'lte'): ns3::LtePdcp [class]
    module.add_class('LtePdcp', parent=root_module['ns3::Object'])
    ## lte-phy.h (module 'lte'): ns3::LtePhy [class]
    module.add_class('LtePhy', parent=root_module['ns3::Object'])
    ## lte-radio-bearer-info.h (module 'lte'): ns3::LteRadioBearerInfo [class]
    module.add_class('LteRadioBearerInfo', parent=root_module['ns3::Object'])
    ## lte-rlc.h (module 'lte'): ns3::LteRlc [class]
    module.add_class('LteRlc', parent=root_module['ns3::Object'])
    ## lte-rlc-am.h (module 'lte'): ns3::LteRlcAm [class]
    module.add_class('LteRlcAm', parent=root_module['ns3::LteRlc'])
    ## lte-rlc.h (module 'lte'): ns3::LteRlcSm [class]
    module.add_class('LteRlcSm', parent=root_module['ns3::LteRlc'])
    ## lte-rlc-um.h (module 'lte'): ns3::LteRlcUm [class]
    module.add_class('LteRlcUm', parent=root_module['ns3::LteRlc'])
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LteSinrChunkProcessor [class]
    module.add_class('LteSinrChunkProcessor', parent=root_module['ns3::SimpleRefCount< ns3::LteSinrChunkProcessor, ns3::empty, ns3::DefaultDeleter<ns3::LteSinrChunkProcessor> >'])
    ## lte-spectrum-phy.h (module 'lte'): ns3::LteSpectrumPhy [class]
    module.add_class('LteSpectrumPhy', parent=root_module['ns3::SpectrumPhy'])
    ## lte-spectrum-phy.h (module 'lte'): ns3::LteSpectrumPhy::State [enumeration]
    module.add_enum('State', ['IDLE', 'TX', 'RX'], outer_class=root_module['ns3::LteSpectrumPhy'])
    ## lte-spectrum-signal-parameters.h (module 'lte'): ns3::LteSpectrumSignalParameters [struct]
    module.add_class('LteSpectrumSignalParameters', parent=root_module['ns3::SpectrumSignalParameters'])
    ## lte-stats-calculator.h (module 'lte'): ns3::LteStatsCalculator [class]
    module.add_class('LteStatsCalculator', parent=root_module['ns3::Object'])
    ## lte-ue-mac.h (module 'lte'): ns3::LteUeMac [class]
    module.add_class('LteUeMac', parent=root_module['ns3::Object'])
    ## lte-ue-phy.h (module 'lte'): ns3::LteUePhy [class]
    module.add_class('LteUePhy', parent=root_module['ns3::LtePhy'])
    ## lte-ue-rrc.h (module 'lte'): ns3::LteUeRrc [class]
    module.add_class('LteUeRrc', parent=root_module['ns3::Object'])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressChecker [class]
    module.add_class('Mac48AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue [class]
    module.add_class('Mac48AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## mac-stats-calculator.h (module 'lte'): ns3::MacStatsCalculator [class]
    module.add_class('MacStatsCalculator', parent=root_module['ns3::LteStatsCalculator'])
    ## mobility-model.h (module 'mobility'): ns3::MobilityModel [class]
    module.add_class('MobilityModel', import_from_module='ns.mobility', parent=root_module['ns3::Object'])
    ## net-device.h (module 'network'): ns3::NetDevice [class]
    module.add_class('NetDevice', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## net-device.h (module 'network'): ns3::NetDevice::PacketType [enumeration]
    module.add_enum('PacketType', ['PACKET_HOST', 'NS3_PACKET_HOST', 'PACKET_BROADCAST', 'NS3_PACKET_BROADCAST', 'PACKET_MULTICAST', 'NS3_PACKET_MULTICAST', 'PACKET_OTHERHOST', 'NS3_PACKET_OTHERHOST'], outer_class=root_module['ns3::NetDevice'], import_from_module='ns.network')
    ## nix-vector.h (module 'network'): ns3::NixVector [class]
    module.add_class('NixVector', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    ## node.h (module 'network'): ns3::Node [class]
    module.add_class('Node', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## random-variable-stream.h (module 'core'): ns3::NormalRandomVariable [class]
    module.add_class('NormalRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryChecker [class]
    module.add_class('ObjectFactoryChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue [class]
    module.add_class('ObjectFactoryValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## packet.h (module 'network'): ns3::Packet [class]
    module.add_class('Packet', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    ## random-variable-stream.h (module 'core'): ns3::ParetoRandomVariable [class]
    module.add_class('ParetoRandomVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariableStream'])
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::PfFfMacScheduler [class]
    module.add_class('PfFfMacScheduler', parent=root_module['ns3::FfMacScheduler'])
    ## pointer.h (module 'core'): ns3::PointerChecker [class]
    module.add_class('PointerChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## pointer.h (module 'core'): ns3::PointerValue [class]
    module.add_class('PointerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## radio-bearer-stats-calculator.h (module 'lte'): ns3::RadioBearerStatsCalculator [class]
    module.add_class('RadioBearerStatsCalculator', parent=root_module['ns3::LteStatsCalculator'])
    ## rem-spectrum-phy.h (module 'lte'): ns3::RemSpectrumPhy [class]
    module.add_class('RemSpectrumPhy', parent=root_module['ns3::SpectrumPhy'])
    ## rr-ff-mac-scheduler.h (module 'lte'): ns3::RrFfMacScheduler [class]
    module.add_class('RrFfMacScheduler', parent=root_module['ns3::FfMacScheduler'])
    ## spectrum-channel.h (module 'spectrum'): ns3::SpectrumChannel [class]
    module.add_class('SpectrumChannel', import_from_module='ns.spectrum', parent=root_module['ns3::Channel'])
    ## string.h (module 'core'): ns3::StringChecker [class]
    module.add_class('StringChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## string.h (module 'core'): ns3::StringValue [class]
    module.add_class('StringValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## nstime.h (module 'core'): ns3::TimeChecker [class]
    module.add_class('TimeChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## nstime.h (module 'core'): ns3::TimeValue [class]
    module.add_class('TimeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## type-id.h (module 'core'): ns3::TypeIdChecker [class]
    module.add_class('TypeIdChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## type-id.h (module 'core'): ns3::TypeIdValue [class]
    module.add_class('TypeIdValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## uinteger.h (module 'core'): ns3::UintegerValue [class]
    module.add_class('UintegerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## ideal-control-messages.h (module 'lte'): ns3::UlDciIdealControlMessage [class]
    module.add_class('UlDciIdealControlMessage', parent=root_module['ns3::IdealControlMessage'])
    ## vector.h (module 'core'): ns3::Vector2DChecker [class]
    module.add_class('Vector2DChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## vector.h (module 'core'): ns3::Vector2DValue [class]
    module.add_class('Vector2DValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## vector.h (module 'core'): ns3::Vector3DChecker [class]
    module.add_class('Vector3DChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## vector.h (module 'core'): ns3::Vector3DValue [class]
    module.add_class('Vector3DValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::VirtualNetDevice [class]
    module.add_class('VirtualNetDevice', import_from_module='ns.virtual_net_device', parent=root_module['ns3::NetDevice'])
    ## address.h (module 'network'): ns3::AddressChecker [class]
    module.add_class('AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## address.h (module 'network'): ns3::AddressValue [class]
    module.add_class('AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ideal-control-messages.h (module 'lte'): ns3::BsrIdealControlMessage [class]
    module.add_class('BsrIdealControlMessage', parent=root_module['ns3::IdealControlMessage'])
    ## ideal-control-messages.h (module 'lte'): ns3::DlCqiIdealControlMessage [class]
    module.add_class('DlCqiIdealControlMessage', parent=root_module['ns3::IdealControlMessage'])
    ## ideal-control-messages.h (module 'lte'): ns3::DlDciIdealControlMessage [class]
    module.add_class('DlDciIdealControlMessage', parent=root_module['ns3::IdealControlMessage'])
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LteCqiSinrChunkProcessor [class]
    module.add_class('LteCqiSinrChunkProcessor', parent=root_module['ns3::LteSinrChunkProcessor'])
    ## lte-enb-phy.h (module 'lte'): ns3::LteEnbPhy [class]
    module.add_class('LteEnbPhy', parent=root_module['ns3::LtePhy'])
    ## lte-net-device.h (module 'lte'): ns3::LteNetDevice [class]
    module.add_class('LteNetDevice', parent=root_module['ns3::NetDevice'])
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LtePemSinrChunkProcessor [class]
    module.add_class('LtePemSinrChunkProcessor', parent=root_module['ns3::LteSinrChunkProcessor'])
    ## lte-ue-net-device.h (module 'lte'): ns3::LteUeNetDevice [class]
    module.add_class('LteUeNetDevice', parent=root_module['ns3::LteNetDevice'])
    ## lte-enb-net-device.h (module 'lte'): ns3::LteEnbNetDevice [class]
    module.add_class('LteEnbNetDevice', parent=root_module['ns3::LteNetDevice'])
    module.add_container('std::vector< unsigned char >', 'unsigned char', container_type='vector')
    module.add_container('std::vector< VendorSpecificListElement_s >', 'VendorSpecificListElement_s', container_type='vector')
    module.add_container('std::vector< LogicalChannelConfigListElement_s >', 'LogicalChannelConfigListElement_s', container_type='vector')
    module.add_container('std::vector< PagingInfoListElement_s >', 'PagingInfoListElement_s', container_type='vector')
    module.add_container('std::vector< DlInfoListElement_s >', 'DlInfoListElement_s', container_type='vector')
    module.add_container('std::vector< RachListElement_s >', 'RachListElement_s', container_type='vector')
    module.add_container('std::vector< CqiListElement_s >', 'CqiListElement_s', container_type='vector')
    module.add_container('std::vector< UlInfoListElement_s >', 'UlInfoListElement_s', container_type='vector')
    module.add_container('std::vector< SrListElement_s >', 'SrListElement_s', container_type='vector')
    module.add_container('std::vector< MacCeListElement_s >', 'MacCeListElement_s', container_type='vector')
    module.add_container('std::vector< BuildDataListElement_s >', 'BuildDataListElement_s', container_type='vector')
    module.add_container('std::vector< BuildRarListElement_s >', 'BuildRarListElement_s', container_type='vector')
    module.add_container('std::vector< BuildBroadcastListElement_s >', 'BuildBroadcastListElement_s', container_type='vector')
    module.add_container('std::vector< UlDciListElement_s >', 'UlDciListElement_s', container_type='vector')
    module.add_container('std::vector< PhichListElement_s >', 'PhichListElement_s', container_type='vector')
    module.add_container('std::vector< int >', 'int', container_type='vector')
    module.add_container('std::list< ns3::Ptr< ns3::Packet > >', 'ns3::Ptr< ns3::Packet >', container_type='list')
    module.add_container('std::vector< double >', 'double', container_type='vector')
    module.add_container('ns3::Bands', 'ns3::BandInfo', container_type='vector')
    module.add_container('std::map< unsigned int, unsigned int >', ('unsigned int', 'unsigned int'), container_type='map')
    module.add_container('std::map< unsigned short, ns3::Ptr< ns3::UeInfo > >', ('short unsigned int', 'ns3::Ptr< ns3::UeInfo >'), container_type='map')
    module.add_container('std::list< ns3::Ptr< ns3::IdealControlMessage > >', 'ns3::Ptr< ns3::IdealControlMessage >', container_type='list')
    module.add_container('std::list< ns3::UlDciIdealControlMessage >', 'ns3::UlDciIdealControlMessage', container_type='list')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, unsigned long long, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, unsigned long long > > >', 'ns3::Uint64Map')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, unsigned long long, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, unsigned long long > > >*', 'ns3::Uint64Map*')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, unsigned long long, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, unsigned long long > > >&', 'ns3::Uint64Map&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyTxEndCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyTxEndCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyTxEndCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxStartCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxStartCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxStartCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxEndErrorCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxEndErrorCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxEndErrorCallback&')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >', 'ns3::Values')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >*', 'ns3::Values*')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >&', 'ns3::Values&')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyTxStartCallback')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyTxStartCallback*')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyTxStartCallback&')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >', 'ns3::Bands')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >*', 'ns3::Bands*')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >&', 'ns3::Bands&')
    typehandlers.add_type_alias('ns3::Vector3D', 'ns3::Vector')
    typehandlers.add_type_alias('ns3::Vector3D*', 'ns3::Vector*')
    typehandlers.add_type_alias('ns3::Vector3D&', 'ns3::Vector&')
    module.add_typedef(root_module['ns3::Vector3D'], 'Vector')
    typehandlers.add_type_alias('ns3::Vector3DValue', 'ns3::VectorValue')
    typehandlers.add_type_alias('ns3::Vector3DValue*', 'ns3::VectorValue*')
    typehandlers.add_type_alias('ns3::Vector3DValue&', 'ns3::VectorValue&')
    module.add_typedef(root_module['ns3::Vector3DValue'], 'VectorValue')
    typehandlers.add_type_alias('uint32_t', 'ns3::SpectrumModelUid_t')
    typehandlers.add_type_alias('uint32_t*', 'ns3::SpectrumModelUid_t*')
    typehandlers.add_type_alias('uint32_t&', 'ns3::SpectrumModelUid_t&')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, unsigned int, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, unsigned int > > >', 'ns3::Uint32Map')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, unsigned int, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, unsigned int > > >*', 'ns3::Uint32Map*')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, unsigned int, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, unsigned int > > >&', 'ns3::Uint32Map&')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, double, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, double > > >', 'ns3::DoubleMap')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, double, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, double > > >*', 'ns3::DoubleMap*')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, double, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, double > > >&', 'ns3::DoubleMap&')
    typehandlers.add_type_alias('std::map< ns3::TbId_t, ns3::tbInfo_t, std::less< ns3::TbId_t >, std::allocator< std::pair< ns3::TbId_t const, ns3::tbInfo_t > > >', 'ns3::expectedTbs_t')
    typehandlers.add_type_alias('std::map< ns3::TbId_t, ns3::tbInfo_t, std::less< ns3::TbId_t >, std::allocator< std::pair< ns3::TbId_t const, ns3::tbInfo_t > > >*', 'ns3::expectedTbs_t*')
    typehandlers.add_type_alias('std::map< ns3::TbId_t, ns3::tbInfo_t, std::less< ns3::TbId_t >, std::allocator< std::pair< ns3::TbId_t const, ns3::tbInfo_t > > >&', 'ns3::expectedTbs_t&')
    typehandlers.add_type_alias('ns3::Vector3DChecker', 'ns3::VectorChecker')
    typehandlers.add_type_alias('ns3::Vector3DChecker*', 'ns3::VectorChecker*')
    typehandlers.add_type_alias('ns3::Vector3DChecker&', 'ns3::VectorChecker&')
    module.add_typedef(root_module['ns3::Vector3DChecker'], 'VectorChecker')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::LteFlowId_t, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::LteFlowId_t > > >', 'ns3::FlowIdMap')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::LteFlowId_t, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::LteFlowId_t > > >*', 'ns3::FlowIdMap*')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::LteFlowId_t, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::LteFlowId_t > > >&', 'ns3::FlowIdMap&')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned int > >, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned int > > > > >', 'ns3::Uint32StatsMap')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned int > >, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned int > > > > >*', 'ns3::Uint32StatsMap*')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned int > >, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned int > > > > >&', 'ns3::Uint32StatsMap&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxEndOkCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxEndOkCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxEndOkCallback&')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned long long > >, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned long long > > > > >', 'ns3::Uint64StatsMap')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned long long > >, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned long long > > > > >*', 'ns3::Uint64StatsMap*')
    typehandlers.add_type_alias('std::map< ns3::ImsiLcidPair_t, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned long long > >, std::less< ns3::ImsiLcidPair_t >, std::allocator< std::pair< ns3::ImsiLcidPair_t const, ns3::Ptr< ns3::MinMaxAvgTotalCalculator< unsigned long long > > > > >&', 'ns3::Uint64StatsMap&')
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    
    ## config.h (module 'core'): ns3::Config::MatchContainer [class]
    module.add_class('MatchContainer', import_from_module='ns.core')
    module.add_container('std::vector< ns3::Ptr< ns3::Object > >', 'ns3::Ptr< ns3::Object >', container_type='vector')
    module.add_container('std::vector< std::string >', 'std::string', container_type='vector')

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Address_methods(root_module, root_module['ns3::Address'])
    register_Ns3AllocationRetentionPriority_methods(root_module, root_module['ns3::AllocationRetentionPriority'])
    register_Ns3AttributeConstructionList_methods(root_module, root_module['ns3::AttributeConstructionList'])
    register_Ns3AttributeConstructionListItem_methods(root_module, root_module['ns3::AttributeConstructionList::Item'])
    register_Ns3BandInfo_methods(root_module, root_module['ns3::BandInfo'])
    register_Ns3Buffer_methods(root_module, root_module['ns3::Buffer'])
    register_Ns3BufferIterator_methods(root_module, root_module['ns3::Buffer::Iterator'])
    register_Ns3BufferSizeLevelBsr_methods(root_module, root_module['ns3::BufferSizeLevelBsr'])
    register_Ns3ByteTagIterator_methods(root_module, root_module['ns3::ByteTagIterator'])
    register_Ns3ByteTagIteratorItem_methods(root_module, root_module['ns3::ByteTagIterator::Item'])
    register_Ns3ByteTagList_methods(root_module, root_module['ns3::ByteTagList'])
    register_Ns3ByteTagListIterator_methods(root_module, root_module['ns3::ByteTagList::Iterator'])
    register_Ns3ByteTagListIteratorItem_methods(root_module, root_module['ns3::ByteTagList::Iterator::Item'])
    register_Ns3CallbackBase_methods(root_module, root_module['ns3::CallbackBase'])
    register_Ns3DataOutputCallback_methods(root_module, root_module['ns3::DataOutputCallback'])
    register_Ns3DataRate_methods(root_module, root_module['ns3::DataRate'])
    register_Ns3EpsBearer_methods(root_module, root_module['ns3::EpsBearer'])
    register_Ns3EventId_methods(root_module, root_module['ns3::EventId'])
    register_Ns3FfMacCschedSapProvider_methods(root_module, root_module['ns3::FfMacCschedSapProvider'])
    register_Ns3FfMacCschedSapProviderCschedCellConfigReqParameters_methods(root_module, root_module['ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters'])
    register_Ns3FfMacCschedSapProviderCschedLcConfigReqParameters_methods(root_module, root_module['ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters'])
    register_Ns3FfMacCschedSapProviderCschedLcReleaseReqParameters_methods(root_module, root_module['ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters'])
    register_Ns3FfMacCschedSapProviderCschedUeConfigReqParameters_methods(root_module, root_module['ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters'])
    register_Ns3FfMacCschedSapProviderCschedUeReleaseReqParameters_methods(root_module, root_module['ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters'])
    register_Ns3FfMacCschedSapUser_methods(root_module, root_module['ns3::FfMacCschedSapUser'])
    register_Ns3FfMacCschedSapUserCschedCellConfigCnfParameters_methods(root_module, root_module['ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters'])
    register_Ns3FfMacCschedSapUserCschedCellConfigUpdateIndParameters_methods(root_module, root_module['ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters'])
    register_Ns3FfMacCschedSapUserCschedLcConfigCnfParameters_methods(root_module, root_module['ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters'])
    register_Ns3FfMacCschedSapUserCschedLcReleaseCnfParameters_methods(root_module, root_module['ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters'])
    register_Ns3FfMacCschedSapUserCschedUeConfigCnfParameters_methods(root_module, root_module['ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters'])
    register_Ns3FfMacCschedSapUserCschedUeConfigUpdateIndParameters_methods(root_module, root_module['ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters'])
    register_Ns3FfMacCschedSapUserCschedUeReleaseCnfParameters_methods(root_module, root_module['ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters'])
    register_Ns3FfMacSchedSapProvider_methods(root_module, root_module['ns3::FfMacSchedSapProvider'])
    register_Ns3FfMacSchedSapProviderSchedDlCqiInfoReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedDlMacBufferReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedDlPagingBufferReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedDlRachInfoReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedDlRlcBufferReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedDlTriggerReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedUlCqiInfoReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedUlMacCtrlInfoReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedUlNoiseInterferenceReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedUlSrInfoReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters'])
    register_Ns3FfMacSchedSapProviderSchedUlTriggerReqParameters_methods(root_module, root_module['ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters'])
    register_Ns3FfMacSchedSapUser_methods(root_module, root_module['ns3::FfMacSchedSapUser'])
    register_Ns3FfMacSchedSapUserSchedDlConfigIndParameters_methods(root_module, root_module['ns3::FfMacSchedSapUser::SchedDlConfigIndParameters'])
    register_Ns3FfMacSchedSapUserSchedUlConfigIndParameters_methods(root_module, root_module['ns3::FfMacSchedSapUser::SchedUlConfigIndParameters'])
    register_Ns3GbrQosInformation_methods(root_module, root_module['ns3::GbrQosInformation'])
    register_Ns3ImsiLcidPair_t_methods(root_module, root_module['ns3::ImsiLcidPair_t'])
    register_Ns3IntToType__0_methods(root_module, root_module['ns3::IntToType< 0 >'])
    register_Ns3IntToType__1_methods(root_module, root_module['ns3::IntToType< 1 >'])
    register_Ns3IntToType__2_methods(root_module, root_module['ns3::IntToType< 2 >'])
    register_Ns3IntToType__3_methods(root_module, root_module['ns3::IntToType< 3 >'])
    register_Ns3IntToType__4_methods(root_module, root_module['ns3::IntToType< 4 >'])
    register_Ns3IntToType__5_methods(root_module, root_module['ns3::IntToType< 5 >'])
    register_Ns3IntToType__6_methods(root_module, root_module['ns3::IntToType< 6 >'])
    register_Ns3Ipv4Address_methods(root_module, root_module['ns3::Ipv4Address'])
    register_Ns3Ipv4AddressHelper_methods(root_module, root_module['ns3::Ipv4AddressHelper'])
    register_Ns3Ipv4InterfaceAddress_methods(root_module, root_module['ns3::Ipv4InterfaceAddress'])
    register_Ns3Ipv4InterfaceContainer_methods(root_module, root_module['ns3::Ipv4InterfaceContainer'])
    register_Ns3Ipv4Mask_methods(root_module, root_module['ns3::Ipv4Mask'])
    register_Ns3Ipv6Address_methods(root_module, root_module['ns3::Ipv6Address'])
    register_Ns3Ipv6InterfaceAddress_methods(root_module, root_module['ns3::Ipv6InterfaceAddress'])
    register_Ns3Ipv6Prefix_methods(root_module, root_module['ns3::Ipv6Prefix'])
    register_Ns3LteEnbCmacSapProvider_methods(root_module, root_module['ns3::LteEnbCmacSapProvider'])
    register_Ns3LteEnbCmacSapProviderLcInfo_methods(root_module, root_module['ns3::LteEnbCmacSapProvider::LcInfo'])
    register_Ns3LteEnbCmacSapUser_methods(root_module, root_module['ns3::LteEnbCmacSapUser'])
    register_Ns3LteEnbPhySapProvider_methods(root_module, root_module['ns3::LteEnbPhySapProvider'])
    register_Ns3LteEnbPhySapUser_methods(root_module, root_module['ns3::LteEnbPhySapUser'])
    register_Ns3LteFfConverter_methods(root_module, root_module['ns3::LteFfConverter'])
    register_Ns3LteFlowId_t_methods(root_module, root_module['ns3::LteFlowId_t'])
    register_Ns3LteMacSapProvider_methods(root_module, root_module['ns3::LteMacSapProvider'])
    register_Ns3LteMacSapProviderReportBufferStatusParameters_methods(root_module, root_module['ns3::LteMacSapProvider::ReportBufferStatusParameters'])
    register_Ns3LteMacSapProviderTransmitPduParameters_methods(root_module, root_module['ns3::LteMacSapProvider::TransmitPduParameters'])
    register_Ns3LteMacSapUser_methods(root_module, root_module['ns3::LteMacSapUser'])
    register_Ns3LteMiErrorModel_methods(root_module, root_module['ns3::LteMiErrorModel'])
    register_Ns3LtePdcpSapProvider_methods(root_module, root_module['ns3::LtePdcpSapProvider'])
    register_Ns3LtePdcpSapProviderTransmitRrcPduParameters_methods(root_module, root_module['ns3::LtePdcpSapProvider::TransmitRrcPduParameters'])
    register_Ns3LtePdcpSapUser_methods(root_module, root_module['ns3::LtePdcpSapUser'])
    register_Ns3LtePdcpSapUserReceiveRrcPduParameters_methods(root_module, root_module['ns3::LtePdcpSapUser::ReceiveRrcPduParameters'])
    register_Ns3LteRlcSapProvider_methods(root_module, root_module['ns3::LteRlcSapProvider'])
    register_Ns3LteRlcSapProviderTransmitPdcpPduParameters_methods(root_module, root_module['ns3::LteRlcSapProvider::TransmitPdcpPduParameters'])
    register_Ns3LteRlcSapUser_methods(root_module, root_module['ns3::LteRlcSapUser'])
    register_Ns3LteSpectrumValueHelper_methods(root_module, root_module['ns3::LteSpectrumValueHelper'])
    register_Ns3LteUeCmacSapProvider_methods(root_module, root_module['ns3::LteUeCmacSapProvider'])
    register_Ns3LteUeCmacSapUser_methods(root_module, root_module['ns3::LteUeCmacSapUser'])
    register_Ns3LteUeConfig_t_methods(root_module, root_module['ns3::LteUeConfig_t'])
    register_Ns3LteUePhySapProvider_methods(root_module, root_module['ns3::LteUePhySapProvider'])
    register_Ns3LteUePhySapUser_methods(root_module, root_module['ns3::LteUePhySapUser'])
    register_Ns3Mac48Address_methods(root_module, root_module['ns3::Mac48Address'])
    register_Ns3Names_methods(root_module, root_module['ns3::Names'])
    register_Ns3NetDeviceContainer_methods(root_module, root_module['ns3::NetDeviceContainer'])
    register_Ns3NodeContainer_methods(root_module, root_module['ns3::NodeContainer'])
    register_Ns3ObjectBase_methods(root_module, root_module['ns3::ObjectBase'])
    register_Ns3ObjectDeleter_methods(root_module, root_module['ns3::ObjectDeleter'])
    register_Ns3ObjectFactory_methods(root_module, root_module['ns3::ObjectFactory'])
    register_Ns3PacketMetadata_methods(root_module, root_module['ns3::PacketMetadata'])
    register_Ns3PacketMetadataItem_methods(root_module, root_module['ns3::PacketMetadata::Item'])
    register_Ns3PacketMetadataItemIterator_methods(root_module, root_module['ns3::PacketMetadata::ItemIterator'])
    register_Ns3PacketTagIterator_methods(root_module, root_module['ns3::PacketTagIterator'])
    register_Ns3PacketTagIteratorItem_methods(root_module, root_module['ns3::PacketTagIterator::Item'])
    register_Ns3PacketTagList_methods(root_module, root_module['ns3::PacketTagList'])
    register_Ns3PacketTagListTagData_methods(root_module, root_module['ns3::PacketTagList::TagData'])
    register_Ns3SequenceNumber10_methods(root_module, root_module['ns3::SequenceNumber10'])
    register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    register_Ns3Simulator_methods(root_module, root_module['ns3::Simulator'])
    register_Ns3StatisticalSummary_methods(root_module, root_module['ns3::StatisticalSummary'])
    register_Ns3Tag_methods(root_module, root_module['ns3::Tag'])
    register_Ns3TagBuffer_methods(root_module, root_module['ns3::TagBuffer'])
    register_Ns3TbId_t_methods(root_module, root_module['ns3::TbId_t'])
    register_Ns3Timer_methods(root_module, root_module['ns3::Timer'])
    register_Ns3TimerImpl_methods(root_module, root_module['ns3::TimerImpl'])
    register_Ns3TransmissionModesLayers_methods(root_module, root_module['ns3::TransmissionModesLayers'])
    register_Ns3TypeId_methods(root_module, root_module['ns3::TypeId'])
    register_Ns3TypeIdAttributeInformation_methods(root_module, root_module['ns3::TypeId::AttributeInformation'])
    register_Ns3TypeIdTraceSourceInformation_methods(root_module, root_module['ns3::TypeId::TraceSourceInformation'])
    register_Ns3Vector2D_methods(root_module, root_module['ns3::Vector2D'])
    register_Ns3Vector3D_methods(root_module, root_module['ns3::Vector3D'])
    register_Ns3Empty_methods(root_module, root_module['ns3::empty'])
    register_Ns3Int64x64_t_methods(root_module, root_module['ns3::int64x64_t'])
    register_Ns3PfsFlowPerf_t_methods(root_module, root_module['ns3::pfsFlowPerf_t'])
    register_Ns3TbInfo_t_methods(root_module, root_module['ns3::tbInfo_t'])
    register_Ns3Chunk_methods(root_module, root_module['ns3::Chunk'])
    register_Ns3Header_methods(root_module, root_module['ns3::Header'])
    register_Ns3Ipv4Header_methods(root_module, root_module['ns3::Ipv4Header'])
    register_Ns3Ipv6Header_methods(root_module, root_module['ns3::Ipv6Header'])
    register_Ns3LtePdcpHeader_methods(root_module, root_module['ns3::LtePdcpHeader'])
    register_Ns3LtePhyTag_methods(root_module, root_module['ns3::LtePhyTag'])
    register_Ns3LteRadioBearerTag_methods(root_module, root_module['ns3::LteRadioBearerTag'])
    register_Ns3LteRlcAmHeader_methods(root_module, root_module['ns3::LteRlcAmHeader'])
    register_Ns3LteRlcHeader_methods(root_module, root_module['ns3::LteRlcHeader'])
    register_Ns3LteRlcSduStatusTag_methods(root_module, root_module['ns3::LteRlcSduStatusTag'])
    register_Ns3Object_methods(root_module, root_module['ns3::Object'])
    register_Ns3ObjectAggregateIterator_methods(root_module, root_module['ns3::Object::AggregateIterator'])
    register_Ns3PacketBurst_methods(root_module, root_module['ns3::PacketBurst'])
    register_Ns3PdcpTag_methods(root_module, root_module['ns3::PdcpTag'])
    register_Ns3RadioEnvironmentMapHelper_methods(root_module, root_module['ns3::RadioEnvironmentMapHelper'])
    register_Ns3RandomVariableStream_methods(root_module, root_module['ns3::RandomVariableStream'])
    register_Ns3RlcTag_methods(root_module, root_module['ns3::RlcTag'])
    register_Ns3SequentialRandomVariable_methods(root_module, root_module['ns3::SequentialRandomVariable'])
    register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    register_Ns3SimpleRefCount__Ns3EpcTft_Ns3Empty_Ns3DefaultDeleter__lt__ns3EpcTft__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::EpcTft, ns3::empty, ns3::DefaultDeleter<ns3::EpcTft> >'])
    register_Ns3SimpleRefCount__Ns3EpcTftClassifier_Ns3Empty_Ns3DefaultDeleter__lt__ns3EpcTftClassifier__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::EpcTftClassifier, ns3::empty, ns3::DefaultDeleter<ns3::EpcTftClassifier> >'])
    register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    register_Ns3SimpleRefCount__Ns3IdealControlMessage_Ns3Empty_Ns3DefaultDeleter__lt__ns3IdealControlMessage__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >'])
    register_Ns3SimpleRefCount__Ns3Ipv4MulticastRoute_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv4MulticastRoute__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >'])
    register_Ns3SimpleRefCount__Ns3Ipv4Route_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv4Route__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >'])
    register_Ns3SimpleRefCount__Ns3LteSinrChunkProcessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3LteSinrChunkProcessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::LteSinrChunkProcessor, ns3::empty, ns3::DefaultDeleter<ns3::LteSinrChunkProcessor> >'])
    register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    register_Ns3SimpleRefCount__Ns3SpectrumModel_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumModel__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >'])
    register_Ns3SimpleRefCount__Ns3SpectrumSignalParameters_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumSignalParameters__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SpectrumSignalParameters, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumSignalParameters> >'])
    register_Ns3SimpleRefCount__Ns3SpectrumValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >'])
    register_Ns3SimpleRefCount__Ns3TraceSourceAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3TraceSourceAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    register_Ns3Socket_methods(root_module, root_module['ns3::Socket'])
    register_Ns3SocketAddressTag_methods(root_module, root_module['ns3::SocketAddressTag'])
    register_Ns3SocketIpTtlTag_methods(root_module, root_module['ns3::SocketIpTtlTag'])
    register_Ns3SocketSetDontFragmentTag_methods(root_module, root_module['ns3::SocketSetDontFragmentTag'])
    register_Ns3SpectrumInterference_methods(root_module, root_module['ns3::SpectrumInterference'])
    register_Ns3SpectrumModel_methods(root_module, root_module['ns3::SpectrumModel'])
    register_Ns3SpectrumPhy_methods(root_module, root_module['ns3::SpectrumPhy'])
    register_Ns3SpectrumPropagationLossModel_methods(root_module, root_module['ns3::SpectrumPropagationLossModel'])
    register_Ns3SpectrumSignalParameters_methods(root_module, root_module['ns3::SpectrumSignalParameters'])
    register_Ns3SpectrumValue_methods(root_module, root_module['ns3::SpectrumValue'])
    register_Ns3Time_methods(root_module, root_module['ns3::Time'])
    register_Ns3TraceFadingLossModel_methods(root_module, root_module['ns3::TraceFadingLossModel'])
    register_Ns3TraceSourceAccessor_methods(root_module, root_module['ns3::TraceSourceAccessor'])
    register_Ns3Trailer_methods(root_module, root_module['ns3::Trailer'])
    register_Ns3TriangularRandomVariable_methods(root_module, root_module['ns3::TriangularRandomVariable'])
    register_Ns3UeInfo_methods(root_module, root_module['ns3::UeInfo'])
    register_Ns3UniformRandomVariable_methods(root_module, root_module['ns3::UniformRandomVariable'])
    register_Ns3WeibullRandomVariable_methods(root_module, root_module['ns3::WeibullRandomVariable'])
    register_Ns3ZetaRandomVariable_methods(root_module, root_module['ns3::ZetaRandomVariable'])
    register_Ns3ZipfRandomVariable_methods(root_module, root_module['ns3::ZipfRandomVariable'])
    register_Ns3Application_methods(root_module, root_module['ns3::Application'])
    register_Ns3AttributeAccessor_methods(root_module, root_module['ns3::AttributeAccessor'])
    register_Ns3AttributeChecker_methods(root_module, root_module['ns3::AttributeChecker'])
    register_Ns3AttributeValue_methods(root_module, root_module['ns3::AttributeValue'])
    register_Ns3BooleanChecker_methods(root_module, root_module['ns3::BooleanChecker'])
    register_Ns3BooleanValue_methods(root_module, root_module['ns3::BooleanValue'])
    register_Ns3CallbackChecker_methods(root_module, root_module['ns3::CallbackChecker'])
    register_Ns3CallbackImplBase_methods(root_module, root_module['ns3::CallbackImplBase'])
    register_Ns3CallbackValue_methods(root_module, root_module['ns3::CallbackValue'])
    register_Ns3Channel_methods(root_module, root_module['ns3::Channel'])
    register_Ns3ConstantRandomVariable_methods(root_module, root_module['ns3::ConstantRandomVariable'])
    register_Ns3DataCalculator_methods(root_module, root_module['ns3::DataCalculator'])
    register_Ns3DataOutputInterface_methods(root_module, root_module['ns3::DataOutputInterface'])
    register_Ns3DataRateChecker_methods(root_module, root_module['ns3::DataRateChecker'])
    register_Ns3DataRateValue_methods(root_module, root_module['ns3::DataRateValue'])
    register_Ns3DeterministicRandomVariable_methods(root_module, root_module['ns3::DeterministicRandomVariable'])
    register_Ns3DoubleValue_methods(root_module, root_module['ns3::DoubleValue'])
    register_Ns3EmpiricalRandomVariable_methods(root_module, root_module['ns3::EmpiricalRandomVariable'])
    register_Ns3EmptyAttributeValue_methods(root_module, root_module['ns3::EmptyAttributeValue'])
    register_Ns3EnumChecker_methods(root_module, root_module['ns3::EnumChecker'])
    register_Ns3EnumValue_methods(root_module, root_module['ns3::EnumValue'])
    register_Ns3EpcEnbApplication_methods(root_module, root_module['ns3::EpcEnbApplication'])
    register_Ns3EpcHelper_methods(root_module, root_module['ns3::EpcHelper'])
    register_Ns3EpcSgwPgwApplication_methods(root_module, root_module['ns3::EpcSgwPgwApplication'])
    register_Ns3EpcTft_methods(root_module, root_module['ns3::EpcTft'])
    register_Ns3EpcTftPacketFilter_methods(root_module, root_module['ns3::EpcTft::PacketFilter'])
    register_Ns3EpcTftClassifier_methods(root_module, root_module['ns3::EpcTftClassifier'])
    register_Ns3ErlangRandomVariable_methods(root_module, root_module['ns3::ErlangRandomVariable'])
    register_Ns3EventImpl_methods(root_module, root_module['ns3::EventImpl'])
    register_Ns3ExponentialRandomVariable_methods(root_module, root_module['ns3::ExponentialRandomVariable'])
    register_Ns3FfMacScheduler_methods(root_module, root_module['ns3::FfMacScheduler'])
    register_Ns3GammaRandomVariable_methods(root_module, root_module['ns3::GammaRandomVariable'])
    register_Ns3GtpuHeader_methods(root_module, root_module['ns3::GtpuHeader'])
    register_Ns3IdealControlMessage_methods(root_module, root_module['ns3::IdealControlMessage'])
    register_Ns3IntegerValue_methods(root_module, root_module['ns3::IntegerValue'])
    register_Ns3IpL4Protocol_methods(root_module, root_module['ns3::IpL4Protocol'])
    register_Ns3Ipv4_methods(root_module, root_module['ns3::Ipv4'])
    register_Ns3Ipv4AddressChecker_methods(root_module, root_module['ns3::Ipv4AddressChecker'])
    register_Ns3Ipv4AddressValue_methods(root_module, root_module['ns3::Ipv4AddressValue'])
    register_Ns3Ipv4MaskChecker_methods(root_module, root_module['ns3::Ipv4MaskChecker'])
    register_Ns3Ipv4MaskValue_methods(root_module, root_module['ns3::Ipv4MaskValue'])
    register_Ns3Ipv4MulticastRoute_methods(root_module, root_module['ns3::Ipv4MulticastRoute'])
    register_Ns3Ipv4Route_methods(root_module, root_module['ns3::Ipv4Route'])
    register_Ns3Ipv6AddressChecker_methods(root_module, root_module['ns3::Ipv6AddressChecker'])
    register_Ns3Ipv6AddressValue_methods(root_module, root_module['ns3::Ipv6AddressValue'])
    register_Ns3Ipv6Interface_methods(root_module, root_module['ns3::Ipv6Interface'])
    register_Ns3Ipv6PrefixChecker_methods(root_module, root_module['ns3::Ipv6PrefixChecker'])
    register_Ns3Ipv6PrefixValue_methods(root_module, root_module['ns3::Ipv6PrefixValue'])
    register_Ns3LogNormalRandomVariable_methods(root_module, root_module['ns3::LogNormalRandomVariable'])
    register_Ns3LteAmc_methods(root_module, root_module['ns3::LteAmc'])
    register_Ns3LteEnbMac_methods(root_module, root_module['ns3::LteEnbMac'])
    register_Ns3LteEnbRrc_methods(root_module, root_module['ns3::LteEnbRrc'])
    register_Ns3LteHelper_methods(root_module, root_module['ns3::LteHelper'])
    register_Ns3LteHexGridEnbTopologyHelper_methods(root_module, root_module['ns3::LteHexGridEnbTopologyHelper'])
    register_Ns3LteInterference_methods(root_module, root_module['ns3::LteInterference'])
    register_Ns3LtePdcp_methods(root_module, root_module['ns3::LtePdcp'])
    register_Ns3LtePhy_methods(root_module, root_module['ns3::LtePhy'])
    register_Ns3LteRadioBearerInfo_methods(root_module, root_module['ns3::LteRadioBearerInfo'])
    register_Ns3LteRlc_methods(root_module, root_module['ns3::LteRlc'])
    register_Ns3LteRlcAm_methods(root_module, root_module['ns3::LteRlcAm'])
    register_Ns3LteRlcSm_methods(root_module, root_module['ns3::LteRlcSm'])
    register_Ns3LteRlcUm_methods(root_module, root_module['ns3::LteRlcUm'])
    register_Ns3LteSinrChunkProcessor_methods(root_module, root_module['ns3::LteSinrChunkProcessor'])
    register_Ns3LteSpectrumPhy_methods(root_module, root_module['ns3::LteSpectrumPhy'])
    register_Ns3LteSpectrumSignalParameters_methods(root_module, root_module['ns3::LteSpectrumSignalParameters'])
    register_Ns3LteStatsCalculator_methods(root_module, root_module['ns3::LteStatsCalculator'])
    register_Ns3LteUeMac_methods(root_module, root_module['ns3::LteUeMac'])
    register_Ns3LteUePhy_methods(root_module, root_module['ns3::LteUePhy'])
    register_Ns3LteUeRrc_methods(root_module, root_module['ns3::LteUeRrc'])
    register_Ns3Mac48AddressChecker_methods(root_module, root_module['ns3::Mac48AddressChecker'])
    register_Ns3Mac48AddressValue_methods(root_module, root_module['ns3::Mac48AddressValue'])
    register_Ns3MacStatsCalculator_methods(root_module, root_module['ns3::MacStatsCalculator'])
    register_Ns3MobilityModel_methods(root_module, root_module['ns3::MobilityModel'])
    register_Ns3NetDevice_methods(root_module, root_module['ns3::NetDevice'])
    register_Ns3NixVector_methods(root_module, root_module['ns3::NixVector'])
    register_Ns3Node_methods(root_module, root_module['ns3::Node'])
    register_Ns3NormalRandomVariable_methods(root_module, root_module['ns3::NormalRandomVariable'])
    register_Ns3ObjectFactoryChecker_methods(root_module, root_module['ns3::ObjectFactoryChecker'])
    register_Ns3ObjectFactoryValue_methods(root_module, root_module['ns3::ObjectFactoryValue'])
    register_Ns3Packet_methods(root_module, root_module['ns3::Packet'])
    register_Ns3ParetoRandomVariable_methods(root_module, root_module['ns3::ParetoRandomVariable'])
    register_Ns3PfFfMacScheduler_methods(root_module, root_module['ns3::PfFfMacScheduler'])
    register_Ns3PointerChecker_methods(root_module, root_module['ns3::PointerChecker'])
    register_Ns3PointerValue_methods(root_module, root_module['ns3::PointerValue'])
    register_Ns3RadioBearerStatsCalculator_methods(root_module, root_module['ns3::RadioBearerStatsCalculator'])
    register_Ns3RemSpectrumPhy_methods(root_module, root_module['ns3::RemSpectrumPhy'])
    register_Ns3RrFfMacScheduler_methods(root_module, root_module['ns3::RrFfMacScheduler'])
    register_Ns3SpectrumChannel_methods(root_module, root_module['ns3::SpectrumChannel'])
    register_Ns3StringChecker_methods(root_module, root_module['ns3::StringChecker'])
    register_Ns3StringValue_methods(root_module, root_module['ns3::StringValue'])
    register_Ns3TimeChecker_methods(root_module, root_module['ns3::TimeChecker'])
    register_Ns3TimeValue_methods(root_module, root_module['ns3::TimeValue'])
    register_Ns3TypeIdChecker_methods(root_module, root_module['ns3::TypeIdChecker'])
    register_Ns3TypeIdValue_methods(root_module, root_module['ns3::TypeIdValue'])
    register_Ns3UintegerValue_methods(root_module, root_module['ns3::UintegerValue'])
    register_Ns3UlDciIdealControlMessage_methods(root_module, root_module['ns3::UlDciIdealControlMessage'])
    register_Ns3Vector2DChecker_methods(root_module, root_module['ns3::Vector2DChecker'])
    register_Ns3Vector2DValue_methods(root_module, root_module['ns3::Vector2DValue'])
    register_Ns3Vector3DChecker_methods(root_module, root_module['ns3::Vector3DChecker'])
    register_Ns3Vector3DValue_methods(root_module, root_module['ns3::Vector3DValue'])
    register_Ns3VirtualNetDevice_methods(root_module, root_module['ns3::VirtualNetDevice'])
    register_Ns3AddressChecker_methods(root_module, root_module['ns3::AddressChecker'])
    register_Ns3AddressValue_methods(root_module, root_module['ns3::AddressValue'])
    register_Ns3BsrIdealControlMessage_methods(root_module, root_module['ns3::BsrIdealControlMessage'])
    register_Ns3DlCqiIdealControlMessage_methods(root_module, root_module['ns3::DlCqiIdealControlMessage'])
    register_Ns3DlDciIdealControlMessage_methods(root_module, root_module['ns3::DlDciIdealControlMessage'])
    register_Ns3LteCqiSinrChunkProcessor_methods(root_module, root_module['ns3::LteCqiSinrChunkProcessor'])
    register_Ns3LteEnbPhy_methods(root_module, root_module['ns3::LteEnbPhy'])
    register_Ns3LteNetDevice_methods(root_module, root_module['ns3::LteNetDevice'])
    register_Ns3LtePemSinrChunkProcessor_methods(root_module, root_module['ns3::LtePemSinrChunkProcessor'])
    register_Ns3LteUeNetDevice_methods(root_module, root_module['ns3::LteUeNetDevice'])
    register_Ns3LteEnbNetDevice_methods(root_module, root_module['ns3::LteEnbNetDevice'])
    register_Ns3ConfigMatchContainer_methods(root_module, root_module['ns3::Config::MatchContainer'])
    return

def register_Ns3Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## address.h (module 'network'): ns3::Address::Address() [constructor]
    cls.add_constructor([])
    ## address.h (module 'network'): ns3::Address::Address(uint8_t type, uint8_t const * buffer, uint8_t len) [constructor]
    cls.add_constructor([param('uint8_t', 'type'), param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h (module 'network'): ns3::Address::Address(ns3::Address const & address) [copy constructor]
    cls.add_constructor([param('ns3::Address const &', 'address')])
    ## address.h (module 'network'): bool ns3::Address::CheckCompatible(uint8_t type, uint8_t len) const [member function]
    cls.add_method('CheckCompatible', 
                   'bool', 
                   [param('uint8_t', 'type'), param('uint8_t', 'len')], 
                   is_const=True)
    ## address.h (module 'network'): uint32_t ns3::Address::CopyAllFrom(uint8_t const * buffer, uint8_t len) [member function]
    cls.add_method('CopyAllFrom', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h (module 'network'): uint32_t ns3::Address::CopyAllTo(uint8_t * buffer, uint8_t len) const [member function]
    cls.add_method('CopyAllTo', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint8_t', 'len')], 
                   is_const=True)
    ## address.h (module 'network'): uint32_t ns3::Address::CopyFrom(uint8_t const * buffer, uint8_t len) [member function]
    cls.add_method('CopyFrom', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h (module 'network'): uint32_t ns3::Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## address.h (module 'network'): void ns3::Address::Deserialize(ns3::TagBuffer buffer) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buffer')])
    ## address.h (module 'network'): uint8_t ns3::Address::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## address.h (module 'network'): uint32_t ns3::Address::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## address.h (module 'network'): bool ns3::Address::IsInvalid() const [member function]
    cls.add_method('IsInvalid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## address.h (module 'network'): bool ns3::Address::IsMatchingType(uint8_t type) const [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('uint8_t', 'type')], 
                   is_const=True)
    ## address.h (module 'network'): static uint8_t ns3::Address::Register() [member function]
    cls.add_method('Register', 
                   'uint8_t', 
                   [], 
                   is_static=True)
    ## address.h (module 'network'): void ns3::Address::Serialize(ns3::TagBuffer buffer) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buffer')], 
                   is_const=True)
    return

def register_Ns3AllocationRetentionPriority_methods(root_module, cls):
    ## eps-bearer.h (module 'lte'): ns3::AllocationRetentionPriority::AllocationRetentionPriority() [constructor]
    cls.add_constructor([])
    ## eps-bearer.h (module 'lte'): ns3::AllocationRetentionPriority::AllocationRetentionPriority(ns3::AllocationRetentionPriority const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AllocationRetentionPriority const &', 'arg0')])
    ## eps-bearer.h (module 'lte'): ns3::AllocationRetentionPriority::preemprionCapability [variable]
    cls.add_instance_attribute('preemprionCapability', 'bool', is_const=False)
    ## eps-bearer.h (module 'lte'): ns3::AllocationRetentionPriority::preemprionVulnerability [variable]
    cls.add_instance_attribute('preemprionVulnerability', 'bool', is_const=False)
    ## eps-bearer.h (module 'lte'): ns3::AllocationRetentionPriority::priorityLevel [variable]
    cls.add_instance_attribute('priorityLevel', 'uint8_t', is_const=False)
    return

def register_Ns3AttributeConstructionList_methods(root_module, cls):
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::AttributeConstructionList(ns3::AttributeConstructionList const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeConstructionList const &', 'arg0')])
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::AttributeConstructionList() [constructor]
    cls.add_constructor([])
    ## attribute-construction-list.h (module 'core'): void ns3::AttributeConstructionList::Add(std::string name, ns3::Ptr<ns3::AttributeChecker const> checker, ns3::Ptr<ns3::AttributeValue> value) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker'), param('ns3::Ptr< ns3::AttributeValue >', 'value')])
    ## attribute-construction-list.h (module 'core'): std::_List_const_iterator<ns3::AttributeConstructionList::Item> ns3::AttributeConstructionList::Begin() const [member function]
    cls.add_method('Begin', 
                   'std::_List_const_iterator< ns3::AttributeConstructionList::Item >', 
                   [], 
                   is_const=True)
    ## attribute-construction-list.h (module 'core'): std::_List_const_iterator<ns3::AttributeConstructionList::Item> ns3::AttributeConstructionList::End() const [member function]
    cls.add_method('End', 
                   'std::_List_const_iterator< ns3::AttributeConstructionList::Item >', 
                   [], 
                   is_const=True)
    ## attribute-construction-list.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::AttributeConstructionList::Find(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('Find', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True)
    return

def register_Ns3AttributeConstructionListItem_methods(root_module, cls):
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::Item() [constructor]
    cls.add_constructor([])
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::Item(ns3::AttributeConstructionList::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeConstructionList::Item const &', 'arg0')])
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::checker [variable]
    cls.add_instance_attribute('checker', 'ns3::Ptr< ns3::AttributeChecker const >', is_const=False)
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::name [variable]
    cls.add_instance_attribute('name', 'std::string', is_const=False)
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::value [variable]
    cls.add_instance_attribute('value', 'ns3::Ptr< ns3::AttributeValue >', is_const=False)
    return

def register_Ns3BandInfo_methods(root_module, cls):
    ## spectrum-model.h (module 'spectrum'): ns3::BandInfo::BandInfo() [constructor]
    cls.add_constructor([])
    ## spectrum-model.h (module 'spectrum'): ns3::BandInfo::BandInfo(ns3::BandInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BandInfo const &', 'arg0')])
    ## spectrum-model.h (module 'spectrum'): ns3::BandInfo::fc [variable]
    cls.add_instance_attribute('fc', 'double', is_const=False)
    ## spectrum-model.h (module 'spectrum'): ns3::BandInfo::fh [variable]
    cls.add_instance_attribute('fh', 'double', is_const=False)
    ## spectrum-model.h (module 'spectrum'): ns3::BandInfo::fl [variable]
    cls.add_instance_attribute('fl', 'double', is_const=False)
    return

def register_Ns3Buffer_methods(root_module, cls):
    ## buffer.h (module 'network'): ns3::Buffer::Buffer() [constructor]
    cls.add_constructor([])
    ## buffer.h (module 'network'): ns3::Buffer::Buffer(uint32_t dataSize) [constructor]
    cls.add_constructor([param('uint32_t', 'dataSize')])
    ## buffer.h (module 'network'): ns3::Buffer::Buffer(uint32_t dataSize, bool initialize) [constructor]
    cls.add_constructor([param('uint32_t', 'dataSize'), param('bool', 'initialize')])
    ## buffer.h (module 'network'): ns3::Buffer::Buffer(ns3::Buffer const & o) [copy constructor]
    cls.add_constructor([param('ns3::Buffer const &', 'o')])
    ## buffer.h (module 'network'): bool ns3::Buffer::AddAtEnd(uint32_t end) [member function]
    cls.add_method('AddAtEnd', 
                   'bool', 
                   [param('uint32_t', 'end')])
    ## buffer.h (module 'network'): void ns3::Buffer::AddAtEnd(ns3::Buffer const & o) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Buffer const &', 'o')])
    ## buffer.h (module 'network'): bool ns3::Buffer::AddAtStart(uint32_t start) [member function]
    cls.add_method('AddAtStart', 
                   'bool', 
                   [param('uint32_t', 'start')])
    ## buffer.h (module 'network'): ns3::Buffer::Iterator ns3::Buffer::Begin() const [member function]
    cls.add_method('Begin', 
                   'ns3::Buffer::Iterator', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): void ns3::Buffer::CopyData(std::ostream * os, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'void', 
                   [param('std::ostream *', 'os'), param('uint32_t', 'size')], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::CopyData(uint8_t * buffer, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')], 
                   is_const=True)
    ## buffer.h (module 'network'): ns3::Buffer ns3::Buffer::CreateFragment(uint32_t start, uint32_t length) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::Buffer', 
                   [param('uint32_t', 'start'), param('uint32_t', 'length')], 
                   is_const=True)
    ## buffer.h (module 'network'): ns3::Buffer ns3::Buffer::CreateFullCopy() const [member function]
    cls.add_method('CreateFullCopy', 
                   'ns3::Buffer', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Deserialize(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h (module 'network'): ns3::Buffer::Iterator ns3::Buffer::End() const [member function]
    cls.add_method('End', 
                   'ns3::Buffer::Iterator', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): int32_t ns3::Buffer::GetCurrentEndOffset() const [member function]
    cls.add_method('GetCurrentEndOffset', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): int32_t ns3::Buffer::GetCurrentStartOffset() const [member function]
    cls.add_method('GetCurrentStartOffset', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): uint8_t const * ns3::Buffer::PeekData() const [member function]
    cls.add_method('PeekData', 
                   'uint8_t const *', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): void ns3::Buffer::RemoveAtEnd(uint32_t end) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## buffer.h (module 'network'): void ns3::Buffer::RemoveAtStart(uint32_t start) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'start')])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3BufferIterator_methods(root_module, cls):
    ## buffer.h (module 'network'): ns3::Buffer::Iterator::Iterator(ns3::Buffer::Iterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator const &', 'arg0')])
    ## buffer.h (module 'network'): ns3::Buffer::Iterator::Iterator() [constructor]
    cls.add_constructor([])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::CalculateIpChecksum(uint16_t size) [member function]
    cls.add_method('CalculateIpChecksum', 
                   'uint16_t', 
                   [param('uint16_t', 'size')])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::CalculateIpChecksum(uint16_t size, uint32_t initialChecksum) [member function]
    cls.add_method('CalculateIpChecksum', 
                   'uint16_t', 
                   [param('uint16_t', 'size'), param('uint32_t', 'initialChecksum')])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::GetDistanceFrom(ns3::Buffer::Iterator const & o) const [member function]
    cls.add_method('GetDistanceFrom', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator const &', 'o')], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): bool ns3::Buffer::Iterator::IsEnd() const [member function]
    cls.add_method('IsEnd', 
                   'bool', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): bool ns3::Buffer::Iterator::IsStart() const [member function]
    cls.add_method('IsStart', 
                   'bool', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Next() [member function]
    cls.add_method('Next', 
                   'void', 
                   [])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Next(uint32_t delta) [member function]
    cls.add_method('Next', 
                   'void', 
                   [param('uint32_t', 'delta')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Prev() [member function]
    cls.add_method('Prev', 
                   'void', 
                   [])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Prev(uint32_t delta) [member function]
    cls.add_method('Prev', 
                   'void', 
                   [param('uint32_t', 'delta')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Read(uint8_t * buffer, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::ReadLsbtohU16() [member function]
    cls.add_method('ReadLsbtohU16', 
                   'uint16_t', 
                   [])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::ReadLsbtohU32() [member function]
    cls.add_method('ReadLsbtohU32', 
                   'uint32_t', 
                   [])
    ## buffer.h (module 'network'): uint64_t ns3::Buffer::Iterator::ReadLsbtohU64() [member function]
    cls.add_method('ReadLsbtohU64', 
                   'uint64_t', 
                   [])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::ReadNtohU16() [member function]
    cls.add_method('ReadNtohU16', 
                   'uint16_t', 
                   [])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::ReadNtohU32() [member function]
    cls.add_method('ReadNtohU32', 
                   'uint32_t', 
                   [])
    ## buffer.h (module 'network'): uint64_t ns3::Buffer::Iterator::ReadNtohU64() [member function]
    cls.add_method('ReadNtohU64', 
                   'uint64_t', 
                   [])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::ReadU16() [member function]
    cls.add_method('ReadU16', 
                   'uint16_t', 
                   [])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::ReadU32() [member function]
    cls.add_method('ReadU32', 
                   'uint32_t', 
                   [])
    ## buffer.h (module 'network'): uint64_t ns3::Buffer::Iterator::ReadU64() [member function]
    cls.add_method('ReadU64', 
                   'uint64_t', 
                   [])
    ## buffer.h (module 'network'): uint8_t ns3::Buffer::Iterator::ReadU8() [member function]
    cls.add_method('ReadU8', 
                   'uint8_t', 
                   [])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Write(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Write(ns3::Buffer::Iterator start, ns3::Buffer::Iterator end) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start'), param('ns3::Buffer::Iterator', 'end')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtolsbU16(uint16_t data) [member function]
    cls.add_method('WriteHtolsbU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtolsbU32(uint32_t data) [member function]
    cls.add_method('WriteHtolsbU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtolsbU64(uint64_t data) [member function]
    cls.add_method('WriteHtolsbU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtonU16(uint16_t data) [member function]
    cls.add_method('WriteHtonU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtonU32(uint32_t data) [member function]
    cls.add_method('WriteHtonU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtonU64(uint64_t data) [member function]
    cls.add_method('WriteHtonU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU16(uint16_t data) [member function]
    cls.add_method('WriteU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU32(uint32_t data) [member function]
    cls.add_method('WriteU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU64(uint64_t data) [member function]
    cls.add_method('WriteU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU8(uint8_t data) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU8(uint8_t data, uint32_t len) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'data'), param('uint32_t', 'len')])
    return

def register_Ns3BufferSizeLevelBsr_methods(root_module, cls):
    ## lte-common.h (module 'lte'): ns3::BufferSizeLevelBsr::BufferSizeLevelBsr() [constructor]
    cls.add_constructor([])
    ## lte-common.h (module 'lte'): ns3::BufferSizeLevelBsr::BufferSizeLevelBsr(ns3::BufferSizeLevelBsr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BufferSizeLevelBsr const &', 'arg0')])
    ## lte-common.h (module 'lte'): static uint32_t ns3::BufferSizeLevelBsr::BsrId2BufferSize(uint8_t val) [member function]
    cls.add_method('BsrId2BufferSize', 
                   'uint32_t', 
                   [param('uint8_t', 'val')], 
                   is_static=True)
    ## lte-common.h (module 'lte'): static uint8_t ns3::BufferSizeLevelBsr::BufferSize2BsrId(uint32_t val) [member function]
    cls.add_method('BufferSize2BsrId', 
                   'uint8_t', 
                   [param('uint32_t', 'val')], 
                   is_static=True)
    ## lte-common.h (module 'lte'): ns3::BufferSizeLevelBsr::m_bufferSizeLevelBsr [variable]
    cls.add_static_attribute('m_bufferSizeLevelBsr', 'int [ 64 ]', is_const=False)
    return

def register_Ns3ByteTagIterator_methods(root_module, cls):
    ## packet.h (module 'network'): ns3::ByteTagIterator::ByteTagIterator(ns3::ByteTagIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagIterator const &', 'arg0')])
    ## packet.h (module 'network'): bool ns3::ByteTagIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::ByteTagIterator::Item ns3::ByteTagIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::ByteTagIterator::Item', 
                   [])
    return

def register_Ns3ByteTagIteratorItem_methods(root_module, cls):
    ## packet.h (module 'network'): ns3::ByteTagIterator::Item::Item(ns3::ByteTagIterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagIterator::Item const &', 'arg0')])
    ## packet.h (module 'network'): uint32_t ns3::ByteTagIterator::Item::GetEnd() const [member function]
    cls.add_method('GetEnd', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::ByteTagIterator::Item::GetStart() const [member function]
    cls.add_method('GetStart', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::ByteTagIterator::Item::GetTag(ns3::Tag & tag) const [member function]
    cls.add_method('GetTag', 
                   'void', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::TypeId ns3::ByteTagIterator::Item::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    return

def register_Ns3ByteTagList_methods(root_module, cls):
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::ByteTagList() [constructor]
    cls.add_constructor([])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::ByteTagList(ns3::ByteTagList const & o) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList const &', 'o')])
    ## byte-tag-list.h (module 'network'): ns3::TagBuffer ns3::ByteTagList::Add(ns3::TypeId tid, uint32_t bufferSize, int32_t start, int32_t end) [member function]
    cls.add_method('Add', 
                   'ns3::TagBuffer', 
                   [param('ns3::TypeId', 'tid'), param('uint32_t', 'bufferSize'), param('int32_t', 'start'), param('int32_t', 'end')])
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::Add(ns3::ByteTagList const & o) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::ByteTagList const &', 'o')])
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::AddAtEnd(int32_t adjustment, int32_t appendOffset) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('int32_t', 'adjustment'), param('int32_t', 'appendOffset')])
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::AddAtStart(int32_t adjustment, int32_t prependOffset) [member function]
    cls.add_method('AddAtStart', 
                   'void', 
                   [param('int32_t', 'adjustment'), param('int32_t', 'prependOffset')])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator ns3::ByteTagList::Begin(int32_t offsetStart, int32_t offsetEnd) const [member function]
    cls.add_method('Begin', 
                   'ns3::ByteTagList::Iterator', 
                   [param('int32_t', 'offsetStart'), param('int32_t', 'offsetEnd')], 
                   is_const=True)
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::RemoveAll() [member function]
    cls.add_method('RemoveAll', 
                   'void', 
                   [])
    return

def register_Ns3ByteTagListIterator_methods(root_module, cls):
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Iterator(ns3::ByteTagList::Iterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList::Iterator const &', 'arg0')])
    ## byte-tag-list.h (module 'network'): uint32_t ns3::ByteTagList::Iterator::GetOffsetStart() const [member function]
    cls.add_method('GetOffsetStart', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## byte-tag-list.h (module 'network'): bool ns3::ByteTagList::Iterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item ns3::ByteTagList::Iterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::ByteTagList::Iterator::Item', 
                   [])
    return

def register_Ns3ByteTagListIteratorItem_methods(root_module, cls):
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::Item(ns3::ByteTagList::Iterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList::Iterator::Item const &', 'arg0')])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::Item(ns3::TagBuffer buf) [constructor]
    cls.add_constructor([param('ns3::TagBuffer', 'buf')])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::buf [variable]
    cls.add_instance_attribute('buf', 'ns3::TagBuffer', is_const=False)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::end [variable]
    cls.add_instance_attribute('end', 'int32_t', is_const=False)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::size [variable]
    cls.add_instance_attribute('size', 'uint32_t', is_const=False)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::start [variable]
    cls.add_instance_attribute('start', 'int32_t', is_const=False)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3CallbackBase_methods(root_module, cls):
    ## callback.h (module 'core'): ns3::CallbackBase::CallbackBase(ns3::CallbackBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackBase const &', 'arg0')])
    ## callback.h (module 'core'): ns3::CallbackBase::CallbackBase() [constructor]
    cls.add_constructor([])
    ## callback.h (module 'core'): ns3::Ptr<ns3::CallbackImplBase> ns3::CallbackBase::GetImpl() const [member function]
    cls.add_method('GetImpl', 
                   'ns3::Ptr< ns3::CallbackImplBase >', 
                   [], 
                   is_const=True)
    ## callback.h (module 'core'): ns3::CallbackBase::CallbackBase(ns3::Ptr<ns3::CallbackImplBase> impl) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CallbackImplBase >', 'impl')], 
                        visibility='protected')
    ## callback.h (module 'core'): static std::string ns3::CallbackBase::Demangle(std::string const & mangled) [member function]
    cls.add_method('Demangle', 
                   'std::string', 
                   [param('std::string const &', 'mangled')], 
                   is_static=True, visibility='protected')
    return

def register_Ns3DataOutputCallback_methods(root_module, cls):
    ## data-output-interface.h (module 'stats'): ns3::DataOutputCallback::DataOutputCallback() [constructor]
    cls.add_constructor([])
    ## data-output-interface.h (module 'stats'): ns3::DataOutputCallback::DataOutputCallback(ns3::DataOutputCallback const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataOutputCallback const &', 'arg0')])
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, int val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('int', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, uint32_t val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('uint32_t', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, double val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('double', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, std::string val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('std::string', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputCallback::OutputSingleton(std::string key, std::string variable, ns3::Time val) [member function]
    cls.add_method('OutputSingleton', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('ns3::Time', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputCallback::OutputStatistic(std::string key, std::string variable, ns3::StatisticalSummary const * statSum) [member function]
    cls.add_method('OutputStatistic', 
                   'void', 
                   [param('std::string', 'key'), param('std::string', 'variable'), param('ns3::StatisticalSummary const *', 'statSum')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3DataRate_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## data-rate.h (module 'network'): ns3::DataRate::DataRate(ns3::DataRate const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRate const &', 'arg0')])
    ## data-rate.h (module 'network'): ns3::DataRate::DataRate() [constructor]
    cls.add_constructor([])
    ## data-rate.h (module 'network'): ns3::DataRate::DataRate(uint64_t bps) [constructor]
    cls.add_constructor([param('uint64_t', 'bps')])
    ## data-rate.h (module 'network'): ns3::DataRate::DataRate(std::string rate) [constructor]
    cls.add_constructor([param('std::string', 'rate')])
    ## data-rate.h (module 'network'): double ns3::DataRate::CalculateTxTime(uint32_t bytes) const [member function]
    cls.add_method('CalculateTxTime', 
                   'double', 
                   [param('uint32_t', 'bytes')], 
                   is_const=True)
    ## data-rate.h (module 'network'): uint64_t ns3::DataRate::GetBitRate() const [member function]
    cls.add_method('GetBitRate', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3EpsBearer_methods(root_module, cls):
    ## eps-bearer.h (module 'lte'): ns3::EpsBearer::EpsBearer(ns3::EpsBearer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EpsBearer const &', 'arg0')])
    ## eps-bearer.h (module 'lte'): ns3::EpsBearer::EpsBearer(ns3::EpsBearer::Qci x) [constructor]
    cls.add_constructor([param('ns3::EpsBearer::Qci', 'x')])
    ## eps-bearer.h (module 'lte'): uint16_t ns3::EpsBearer::GetPacketDelayBudgetMs() const [member function]
    cls.add_method('GetPacketDelayBudgetMs', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## eps-bearer.h (module 'lte'): double ns3::EpsBearer::GetPacketErrorLossRate() const [member function]
    cls.add_method('GetPacketErrorLossRate', 
                   'double', 
                   [], 
                   is_const=True)
    ## eps-bearer.h (module 'lte'): uint8_t ns3::EpsBearer::GetPriority() const [member function]
    cls.add_method('GetPriority', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## eps-bearer.h (module 'lte'): bool ns3::EpsBearer::IsGbr() const [member function]
    cls.add_method('IsGbr', 
                   'bool', 
                   [], 
                   is_const=True)
    ## eps-bearer.h (module 'lte'): ns3::EpsBearer::arp [variable]
    cls.add_instance_attribute('arp', 'ns3::AllocationRetentionPriority', is_const=False)
    ## eps-bearer.h (module 'lte'): ns3::EpsBearer::gbrQosInfo [variable]
    cls.add_instance_attribute('gbrQosInfo', 'ns3::GbrQosInformation', is_const=False)
    ## eps-bearer.h (module 'lte'): ns3::EpsBearer::qci [variable]
    cls.add_instance_attribute('qci', 'ns3::EpsBearer::Qci', is_const=False)
    return

def register_Ns3EventId_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('==')
    ## event-id.h (module 'core'): ns3::EventId::EventId(ns3::EventId const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EventId const &', 'arg0')])
    ## event-id.h (module 'core'): ns3::EventId::EventId() [constructor]
    cls.add_constructor([])
    ## event-id.h (module 'core'): ns3::EventId::EventId(ns3::Ptr<ns3::EventImpl> const & impl, uint64_t ts, uint32_t context, uint32_t uid) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::EventImpl > const &', 'impl'), param('uint64_t', 'ts'), param('uint32_t', 'context'), param('uint32_t', 'uid')])
    ## event-id.h (module 'core'): void ns3::EventId::Cancel() [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [])
    ## event-id.h (module 'core'): uint32_t ns3::EventId::GetContext() const [member function]
    cls.add_method('GetContext', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): uint64_t ns3::EventId::GetTs() const [member function]
    cls.add_method('GetTs', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): uint32_t ns3::EventId::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): bool ns3::EventId::IsExpired() const [member function]
    cls.add_method('IsExpired', 
                   'bool', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): bool ns3::EventId::IsRunning() const [member function]
    cls.add_method('IsRunning', 
                   'bool', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): ns3::EventImpl * ns3::EventId::PeekEventImpl() const [member function]
    cls.add_method('PeekEventImpl', 
                   'ns3::EventImpl *', 
                   [], 
                   is_const=True)
    return

def register_Ns3FfMacCschedSapProvider_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::FfMacCschedSapProvider() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::FfMacCschedSapProvider(ns3::FfMacCschedSapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapProvider const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapProvider::CschedCellConfigReq(ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters const & params) [member function]
    cls.add_method('CschedCellConfigReq', 
                   'void', 
                   [param('ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapProvider::CschedLcConfigReq(ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters const & params) [member function]
    cls.add_method('CschedLcConfigReq', 
                   'void', 
                   [param('ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapProvider::CschedLcReleaseReq(ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters const & params) [member function]
    cls.add_method('CschedLcReleaseReq', 
                   'void', 
                   [param('ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapProvider::CschedUeConfigReq(ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters const & params) [member function]
    cls.add_method('CschedUeConfigReq', 
                   'void', 
                   [param('ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapProvider::CschedUeReleaseReq(ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters const & params) [member function]
    cls.add_method('CschedUeReleaseReq', 
                   'void', 
                   [param('ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3FfMacCschedSapProviderCschedCellConfigReqParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::CschedCellConfigReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::CschedCellConfigReqParameters(ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_antennaPortsCount [variable]
    cls.add_instance_attribute('m_antennaPortsCount', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_deltaPucchShift [variable]
    cls.add_instance_attribute('m_deltaPucchShift', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_dlBandwidth [variable]
    cls.add_instance_attribute('m_dlBandwidth', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_dlCyclicPrefixLength [variable]
    cls.add_instance_attribute('m_dlCyclicPrefixLength', 'NormalExtended_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_duplexMode [variable]
    cls.add_instance_attribute('m_duplexMode', 'ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::DuplexMode_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_enable64Qam [variable]
    cls.add_instance_attribute('m_enable64Qam', 'ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::Enable64Qam_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_hoppingMode [variable]
    cls.add_instance_attribute('m_hoppingMode', 'ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::HoppingMode_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_initialNrOfPdcchOfdmSymbols [variable]
    cls.add_instance_attribute('m_initialNrOfPdcchOfdmSymbols', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_macContentionResolutionTimer [variable]
    cls.add_instance_attribute('m_macContentionResolutionTimer', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_maxHarqMsg3Tx [variable]
    cls.add_instance_attribute('m_maxHarqMsg3Tx', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_mbsfnSubframeConfigRfOffset [variable]
    cls.add_instance_attribute('m_mbsfnSubframeConfigRfOffset', 'std::vector< unsigned char >', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_mbsfnSubframeConfigRfPeriod [variable]
    cls.add_instance_attribute('m_mbsfnSubframeConfigRfPeriod', 'std::vector< unsigned char >', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_mbsfnSubframeConfigSfAllocation [variable]
    cls.add_instance_attribute('m_mbsfnSubframeConfigSfAllocation', 'std::vector< unsigned char >', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_n1PucchAn [variable]
    cls.add_instance_attribute('m_n1PucchAn', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_nSb [variable]
    cls.add_instance_attribute('m_nSb', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_ncsAn [variable]
    cls.add_instance_attribute('m_ncsAn', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_nrbCqi [variable]
    cls.add_instance_attribute('m_nrbCqi', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_phichDuration [variable]
    cls.add_instance_attribute('m_phichDuration', 'NormalExtended_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_phichResource [variable]
    cls.add_instance_attribute('m_phichResource', 'ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::PhichResource_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_prachConfigurationIndex [variable]
    cls.add_instance_attribute('m_prachConfigurationIndex', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_prachFreqOffset [variable]
    cls.add_instance_attribute('m_prachFreqOffset', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_puschHoppingOffset [variable]
    cls.add_instance_attribute('m_puschHoppingOffset', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_raResponseWindowSize [variable]
    cls.add_instance_attribute('m_raResponseWindowSize', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_siConfiguration [variable]
    cls.add_instance_attribute('m_siConfiguration', 'SiConfiguration_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_specialSubframePatterns [variable]
    cls.add_instance_attribute('m_specialSubframePatterns', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_srsBandwidthConfiguration [variable]
    cls.add_instance_attribute('m_srsBandwidthConfiguration', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_srsMaxUpPts [variable]
    cls.add_instance_attribute('m_srsMaxUpPts', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_srsSubframeConfiguration [variable]
    cls.add_instance_attribute('m_srsSubframeConfiguration', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_srsSubframeOffset [variable]
    cls.add_instance_attribute('m_srsSubframeOffset', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_subframeAssignment [variable]
    cls.add_instance_attribute('m_subframeAssignment', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_ulBandwidth [variable]
    cls.add_instance_attribute('m_ulBandwidth', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_ulCyclicPrefixLength [variable]
    cls.add_instance_attribute('m_ulCyclicPrefixLength', 'NormalExtended_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedCellConfigReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapProviderCschedLcConfigReqParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters::CschedLcConfigReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters::CschedLcConfigReqParameters(ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters::m_logicalChannelConfigList [variable]
    cls.add_instance_attribute('m_logicalChannelConfigList', 'std::vector< LogicalChannelConfigListElement_s >', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters::m_reconfigureFlag [variable]
    cls.add_instance_attribute('m_reconfigureFlag', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcConfigReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapProviderCschedLcReleaseReqParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters::CschedLcReleaseReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters::CschedLcReleaseReqParameters(ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters::m_logicalChannelIdentity [variable]
    cls.add_instance_attribute('m_logicalChannelIdentity', 'std::vector< unsigned char >', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedLcReleaseReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapProviderCschedUeConfigReqParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::CschedUeConfigReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::CschedUeConfigReqParameters(ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_ackNackRepetitionFactor [variable]
    cls.add_instance_attribute('m_ackNackRepetitionFactor', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_ackNackSrsSimultaneousTransmission [variable]
    cls.add_instance_attribute('m_ackNackSrsSimultaneousTransmission', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_aperiodicCqiRepMode [variable]
    cls.add_instance_attribute('m_aperiodicCqiRepMode', 'ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::RepMode_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_betaOffsetAckIndex [variable]
    cls.add_instance_attribute('m_betaOffsetAckIndex', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_betaOffsetCqiIndex [variable]
    cls.add_instance_attribute('m_betaOffsetCqiIndex', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_betaOffsetRiIndex [variable]
    cls.add_instance_attribute('m_betaOffsetRiIndex', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_cqiConfig [variable]
    cls.add_instance_attribute('m_cqiConfig', 'CqiConfig_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_cqiConfigPresent [variable]
    cls.add_instance_attribute('m_cqiConfigPresent', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_drxConfig [variable]
    cls.add_instance_attribute('m_drxConfig', 'DrxConfig_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_drxConfigPresent [variable]
    cls.add_instance_attribute('m_drxConfigPresent', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_maxHarqTx [variable]
    cls.add_instance_attribute('m_maxHarqTx', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_measGapConfigPattern [variable]
    cls.add_instance_attribute('m_measGapConfigPattern', 'ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::MeasGapConfigPattern_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_measGapConfigSubframeOffset [variable]
    cls.add_instance_attribute('m_measGapConfigSubframeOffset', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_reconfigureFlag [variable]
    cls.add_instance_attribute('m_reconfigureFlag', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_simultaneousAckNackAndCqi [variable]
    cls.add_instance_attribute('m_simultaneousAckNackAndCqi', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_spsConfig [variable]
    cls.add_instance_attribute('m_spsConfig', 'SpsConfig_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_spsConfigPresent [variable]
    cls.add_instance_attribute('m_spsConfigPresent', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_srConfig [variable]
    cls.add_instance_attribute('m_srConfig', 'SrConfig_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_srConfigPresent [variable]
    cls.add_instance_attribute('m_srConfigPresent', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_tddAckNackFeedbackMode [variable]
    cls.add_instance_attribute('m_tddAckNackFeedbackMode', 'ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::FeedbackMode_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_timeAlignmentTimer [variable]
    cls.add_instance_attribute('m_timeAlignmentTimer', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_transmissionMode [variable]
    cls.add_instance_attribute('m_transmissionMode', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_ttiBundling [variable]
    cls.add_instance_attribute('m_ttiBundling', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_ueAggregatedMaximumBitrateDl [variable]
    cls.add_instance_attribute('m_ueAggregatedMaximumBitrateDl', 'uint64_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_ueAggregatedMaximumBitrateUl [variable]
    cls.add_instance_attribute('m_ueAggregatedMaximumBitrateUl', 'uint64_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_ueCapabilities [variable]
    cls.add_instance_attribute('m_ueCapabilities', 'UeCapabilities_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_ueTransmitAntennaSelection [variable]
    cls.add_instance_attribute('m_ueTransmitAntennaSelection', 'ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::OpenClosedLoop_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapProviderCschedUeReleaseReqParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters::CschedUeReleaseReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters::CschedUeReleaseReqParameters(ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapProvider::CschedUeReleaseReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapUser_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::FfMacCschedSapUser() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::FfMacCschedSapUser(ns3::FfMacCschedSapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapUser const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapUser::CschedCellConfigCnf(ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters const & params) [member function]
    cls.add_method('CschedCellConfigCnf', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapUser::CschedCellConfigUpdateInd(ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters const & params) [member function]
    cls.add_method('CschedCellConfigUpdateInd', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapUser::CschedLcConfigCnf(ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters const & params) [member function]
    cls.add_method('CschedLcConfigCnf', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapUser::CschedLcReleaseCnf(ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters const & params) [member function]
    cls.add_method('CschedLcReleaseCnf', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapUser::CschedUeConfigCnf(ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters const & params) [member function]
    cls.add_method('CschedUeConfigCnf', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapUser::CschedUeConfigUpdateInd(ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters const & params) [member function]
    cls.add_method('CschedUeConfigUpdateInd', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-csched-sap.h (module 'lte'): void ns3::FfMacCschedSapUser::CschedUeReleaseCnf(ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters const & params) [member function]
    cls.add_method('CschedUeReleaseCnf', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3FfMacCschedSapUserCschedCellConfigCnfParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters::CschedCellConfigCnfParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters::CschedCellConfigCnfParameters(ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters::m_result [variable]
    cls.add_instance_attribute('m_result', 'Result_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigCnfParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapUserCschedCellConfigUpdateIndParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters::CschedCellConfigUpdateIndParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters::CschedCellConfigUpdateIndParameters(ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters::m_prbUtilizationDl [variable]
    cls.add_instance_attribute('m_prbUtilizationDl', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters::m_prbUtilizationUl [variable]
    cls.add_instance_attribute('m_prbUtilizationUl', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedCellConfigUpdateIndParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapUserCschedLcConfigCnfParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters::CschedLcConfigCnfParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters::CschedLcConfigCnfParameters(ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters::m_logicalChannelIdentity [variable]
    cls.add_instance_attribute('m_logicalChannelIdentity', 'std::vector< unsigned char >', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters::m_result [variable]
    cls.add_instance_attribute('m_result', 'Result_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcConfigCnfParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapUserCschedLcReleaseCnfParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters::CschedLcReleaseCnfParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters::CschedLcReleaseCnfParameters(ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters::m_logicalChannelIdentity [variable]
    cls.add_instance_attribute('m_logicalChannelIdentity', 'std::vector< unsigned char >', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters::m_result [variable]
    cls.add_instance_attribute('m_result', 'Result_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedLcReleaseCnfParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapUserCschedUeConfigCnfParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters::CschedUeConfigCnfParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters::CschedUeConfigCnfParameters(ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters::m_result [variable]
    cls.add_instance_attribute('m_result', 'Result_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigCnfParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapUserCschedUeConfigUpdateIndParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::CschedUeConfigUpdateIndParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::CschedUeConfigUpdateIndParameters(ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_cqiConfig [variable]
    cls.add_instance_attribute('m_cqiConfig', 'CqiConfig_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_cqiConfigPresent [variable]
    cls.add_instance_attribute('m_cqiConfigPresent', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_spsConfig [variable]
    cls.add_instance_attribute('m_spsConfig', 'SpsConfig_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_spsConfigPresent [variable]
    cls.add_instance_attribute('m_spsConfigPresent', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_srConfig [variable]
    cls.add_instance_attribute('m_srConfig', 'SrConfig_s', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_srConfigPresent [variable]
    cls.add_instance_attribute('m_srConfigPresent', 'bool', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_transmissionMode [variable]
    cls.add_instance_attribute('m_transmissionMode', 'uint8_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeConfigUpdateIndParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacCschedSapUserCschedUeReleaseCnfParameters_methods(root_module, cls):
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters::CschedUeReleaseCnfParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters::CschedUeReleaseCnfParameters(ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters const &', 'arg0')])
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters::m_result [variable]
    cls.add_instance_attribute('m_result', 'Result_e', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-csched-sap.h (module 'lte'): ns3::FfMacCschedSapUser::CschedUeReleaseCnfParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProvider_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::FfMacSchedSapProvider() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::FfMacSchedSapProvider(ns3::FfMacSchedSapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedDlCqiInfoReq(ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters const & params) [member function]
    cls.add_method('SchedDlCqiInfoReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedDlMacBufferReq(ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters const & params) [member function]
    cls.add_method('SchedDlMacBufferReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedDlPagingBufferReq(ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters const & params) [member function]
    cls.add_method('SchedDlPagingBufferReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedDlRachInfoReq(ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters const & params) [member function]
    cls.add_method('SchedDlRachInfoReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedDlRlcBufferReq(ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters const & params) [member function]
    cls.add_method('SchedDlRlcBufferReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedDlTriggerReq(ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters const & params) [member function]
    cls.add_method('SchedDlTriggerReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedUlCqiInfoReq(ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters const & params) [member function]
    cls.add_method('SchedUlCqiInfoReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReq(ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters const & params) [member function]
    cls.add_method('SchedUlMacCtrlInfoReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReq(ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters const & params) [member function]
    cls.add_method('SchedUlNoiseInterferenceReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedUlSrInfoReq(ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters const & params) [member function]
    cls.add_method('SchedUlSrInfoReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapProvider::SchedUlTriggerReq(ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters const & params) [member function]
    cls.add_method('SchedUlTriggerReq', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3FfMacSchedSapProviderSchedDlCqiInfoReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters::SchedDlCqiInfoReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters::SchedDlCqiInfoReqParameters(ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters::m_cqiList [variable]
    cls.add_instance_attribute('m_cqiList', 'std::vector< CqiListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters::m_sfnSf [variable]
    cls.add_instance_attribute('m_sfnSf', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlCqiInfoReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedDlMacBufferReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters::SchedDlMacBufferReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters::SchedDlMacBufferReqParameters(ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters::m_ceBitmap [variable]
    cls.add_instance_attribute('m_ceBitmap', 'CeBitmap_e', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlMacBufferReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedDlPagingBufferReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters::SchedDlPagingBufferReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters::SchedDlPagingBufferReqParameters(ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters::m_pagingInfoList [variable]
    cls.add_instance_attribute('m_pagingInfoList', 'std::vector< PagingInfoListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlPagingBufferReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedDlRachInfoReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters::SchedDlRachInfoReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters::SchedDlRachInfoReqParameters(ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters::m_rachList [variable]
    cls.add_instance_attribute('m_rachList', 'std::vector< RachListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters::m_sfnSf [variable]
    cls.add_instance_attribute('m_sfnSf', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRachInfoReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedDlRlcBufferReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::SchedDlRlcBufferReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::SchedDlRlcBufferReqParameters(ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::m_logicalChannelIdentity [variable]
    cls.add_instance_attribute('m_logicalChannelIdentity', 'uint8_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::m_rlcRetransmissionHolDelay [variable]
    cls.add_instance_attribute('m_rlcRetransmissionHolDelay', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::m_rlcRetransmissionQueueSize [variable]
    cls.add_instance_attribute('m_rlcRetransmissionQueueSize', 'uint32_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::m_rlcStatusPduSize [variable]
    cls.add_instance_attribute('m_rlcStatusPduSize', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::m_rlcTransmissionQueueHolDelay [variable]
    cls.add_instance_attribute('m_rlcTransmissionQueueHolDelay', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::m_rlcTransmissionQueueSize [variable]
    cls.add_instance_attribute('m_rlcTransmissionQueueSize', 'uint32_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlRlcBufferReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedDlTriggerReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters::SchedDlTriggerReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters::SchedDlTriggerReqParameters(ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters::m_dlInfoList [variable]
    cls.add_instance_attribute('m_dlInfoList', 'std::vector< DlInfoListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters::m_sfnSf [variable]
    cls.add_instance_attribute('m_sfnSf', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedDlTriggerReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedUlCqiInfoReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters::SchedUlCqiInfoReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters::SchedUlCqiInfoReqParameters(ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters::m_sfnSf [variable]
    cls.add_instance_attribute('m_sfnSf', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters::m_ulCqi [variable]
    cls.add_instance_attribute('m_ulCqi', 'UlCqi_s', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlCqiInfoReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedUlMacCtrlInfoReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters::SchedUlMacCtrlInfoReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters::SchedUlMacCtrlInfoReqParameters(ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters::m_macCeList [variable]
    cls.add_instance_attribute('m_macCeList', 'std::vector< MacCeListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters::m_sfnSf [variable]
    cls.add_instance_attribute('m_sfnSf', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlMacCtrlInfoReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedUlNoiseInterferenceReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters::SchedUlNoiseInterferenceReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters::SchedUlNoiseInterferenceReqParameters(ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters::m_rip [variable]
    cls.add_instance_attribute('m_rip', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters::m_sfnSf [variable]
    cls.add_instance_attribute('m_sfnSf', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters::m_tnp [variable]
    cls.add_instance_attribute('m_tnp', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlNoiseInterferenceReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedUlSrInfoReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters::SchedUlSrInfoReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters::SchedUlSrInfoReqParameters(ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters::m_sfnSf [variable]
    cls.add_instance_attribute('m_sfnSf', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters::m_srList [variable]
    cls.add_instance_attribute('m_srList', 'std::vector< SrListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlSrInfoReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapProviderSchedUlTriggerReqParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters::SchedUlTriggerReqParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters::SchedUlTriggerReqParameters(ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters::m_sfnSf [variable]
    cls.add_instance_attribute('m_sfnSf', 'uint16_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters::m_ulInfoList [variable]
    cls.add_instance_attribute('m_ulInfoList', 'std::vector< UlInfoListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapProvider::SchedUlTriggerReqParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapUser_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::FfMacSchedSapUser() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::FfMacSchedSapUser(ns3::FfMacSchedSapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapUser const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapUser::SchedDlConfigInd(ns3::FfMacSchedSapUser::SchedDlConfigIndParameters const & params) [member function]
    cls.add_method('SchedDlConfigInd', 
                   'void', 
                   [param('ns3::FfMacSchedSapUser::SchedDlConfigIndParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-sched-sap.h (module 'lte'): void ns3::FfMacSchedSapUser::SchedUlConfigInd(ns3::FfMacSchedSapUser::SchedUlConfigIndParameters const & params) [member function]
    cls.add_method('SchedUlConfigInd', 
                   'void', 
                   [param('ns3::FfMacSchedSapUser::SchedUlConfigIndParameters const &', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3FfMacSchedSapUserSchedDlConfigIndParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedDlConfigIndParameters::SchedDlConfigIndParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedDlConfigIndParameters::SchedDlConfigIndParameters(ns3::FfMacSchedSapUser::SchedDlConfigIndParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapUser::SchedDlConfigIndParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedDlConfigIndParameters::m_buildBroadcastList [variable]
    cls.add_instance_attribute('m_buildBroadcastList', 'std::vector< BuildBroadcastListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedDlConfigIndParameters::m_buildDataList [variable]
    cls.add_instance_attribute('m_buildDataList', 'std::vector< BuildDataListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedDlConfigIndParameters::m_buildRarList [variable]
    cls.add_instance_attribute('m_buildRarList', 'std::vector< BuildRarListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedDlConfigIndParameters::m_nrOfPdcchOfdmSymbols [variable]
    cls.add_instance_attribute('m_nrOfPdcchOfdmSymbols', 'uint8_t', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedDlConfigIndParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3FfMacSchedSapUserSchedUlConfigIndParameters_methods(root_module, cls):
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedUlConfigIndParameters::SchedUlConfigIndParameters() [constructor]
    cls.add_constructor([])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedUlConfigIndParameters::SchedUlConfigIndParameters(ns3::FfMacSchedSapUser::SchedUlConfigIndParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacSchedSapUser::SchedUlConfigIndParameters const &', 'arg0')])
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedUlConfigIndParameters::m_dciList [variable]
    cls.add_instance_attribute('m_dciList', 'std::vector< UlDciListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedUlConfigIndParameters::m_phichList [variable]
    cls.add_instance_attribute('m_phichList', 'std::vector< PhichListElement_s >', is_const=False)
    ## ff-mac-sched-sap.h (module 'lte'): ns3::FfMacSchedSapUser::SchedUlConfigIndParameters::m_vendorSpecificList [variable]
    cls.add_instance_attribute('m_vendorSpecificList', 'std::vector< VendorSpecificListElement_s >', is_const=False)
    return

def register_Ns3GbrQosInformation_methods(root_module, cls):
    ## eps-bearer.h (module 'lte'): ns3::GbrQosInformation::GbrQosInformation() [constructor]
    cls.add_constructor([])
    ## eps-bearer.h (module 'lte'): ns3::GbrQosInformation::GbrQosInformation(ns3::GbrQosInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GbrQosInformation const &', 'arg0')])
    ## eps-bearer.h (module 'lte'): ns3::GbrQosInformation::gbrDl [variable]
    cls.add_instance_attribute('gbrDl', 'uint64_t', is_const=False)
    ## eps-bearer.h (module 'lte'): ns3::GbrQosInformation::gbrUl [variable]
    cls.add_instance_attribute('gbrUl', 'uint64_t', is_const=False)
    ## eps-bearer.h (module 'lte'): ns3::GbrQosInformation::mbrDl [variable]
    cls.add_instance_attribute('mbrDl', 'uint64_t', is_const=False)
    ## eps-bearer.h (module 'lte'): ns3::GbrQosInformation::mbrUl [variable]
    cls.add_instance_attribute('mbrUl', 'uint64_t', is_const=False)
    return

def register_Ns3ImsiLcidPair_t_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## lte-common.h (module 'lte'): ns3::ImsiLcidPair_t::ImsiLcidPair_t(ns3::ImsiLcidPair_t const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ImsiLcidPair_t const &', 'arg0')])
    ## lte-common.h (module 'lte'): ns3::ImsiLcidPair_t::ImsiLcidPair_t() [constructor]
    cls.add_constructor([])
    ## lte-common.h (module 'lte'): ns3::ImsiLcidPair_t::ImsiLcidPair_t(uint64_t const a, uint8_t const b) [constructor]
    cls.add_constructor([param('uint64_t const', 'a'), param('uint8_t const', 'b')])
    ## lte-common.h (module 'lte'): ns3::ImsiLcidPair_t::m_imsi [variable]
    cls.add_instance_attribute('m_imsi', 'uint64_t', is_const=False)
    ## lte-common.h (module 'lte'): ns3::ImsiLcidPair_t::m_lcId [variable]
    cls.add_instance_attribute('m_lcId', 'uint8_t', is_const=False)
    return

def register_Ns3IntToType__0_methods(root_module, cls):
    ## int-to-type.h (module 'core'): ns3::IntToType<0>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h (module 'core'): ns3::IntToType<0>::IntToType(ns3::IntToType<0> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 0 > const &', 'arg0')])
    return

def register_Ns3IntToType__1_methods(root_module, cls):
    ## int-to-type.h (module 'core'): ns3::IntToType<1>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h (module 'core'): ns3::IntToType<1>::IntToType(ns3::IntToType<1> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 1 > const &', 'arg0')])
    return

def register_Ns3IntToType__2_methods(root_module, cls):
    ## int-to-type.h (module 'core'): ns3::IntToType<2>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h (module 'core'): ns3::IntToType<2>::IntToType(ns3::IntToType<2> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 2 > const &', 'arg0')])
    return

def register_Ns3IntToType__3_methods(root_module, cls):
    ## int-to-type.h (module 'core'): ns3::IntToType<3>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h (module 'core'): ns3::IntToType<3>::IntToType(ns3::IntToType<3> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 3 > const &', 'arg0')])
    return

def register_Ns3IntToType__4_methods(root_module, cls):
    ## int-to-type.h (module 'core'): ns3::IntToType<4>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h (module 'core'): ns3::IntToType<4>::IntToType(ns3::IntToType<4> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 4 > const &', 'arg0')])
    return

def register_Ns3IntToType__5_methods(root_module, cls):
    ## int-to-type.h (module 'core'): ns3::IntToType<5>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h (module 'core'): ns3::IntToType<5>::IntToType(ns3::IntToType<5> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 5 > const &', 'arg0')])
    return

def register_Ns3IntToType__6_methods(root_module, cls):
    ## int-to-type.h (module 'core'): ns3::IntToType<6>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h (module 'core'): ns3::IntToType<6>::IntToType(ns3::IntToType<6> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 6 > const &', 'arg0')])
    return

def register_Ns3Ipv4Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address::Ipv4Address(ns3::Ipv4Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Address const &', 'arg0')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address::Ipv4Address() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address::Ipv4Address(uint32_t address) [constructor]
    cls.add_constructor([param('uint32_t', 'address')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address::Ipv4Address(char const * address) [constructor]
    cls.add_constructor([param('char const *', 'address')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address ns3::Ipv4Address::CombineMask(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('CombineMask', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::Deserialize(uint8_t const * buf) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Ipv4Address', 
                   [param('uint8_t const *', 'buf')], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): uint32_t ns3::Ipv4Address::Get() const [member function]
    cls.add_method('Get', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::GetAny() [member function]
    cls.add_method('GetAny', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address ns3::Ipv4Address::GetSubnetDirectedBroadcast(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('GetSubnetDirectedBroadcast', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsEqual(ns3::Ipv4Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv4Address const &', 'other')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsLocalMulticast() const [member function]
    cls.add_method('IsLocalMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static bool ns3::Ipv4Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsSubnetDirectedBroadcast(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('IsSubnetDirectedBroadcast', 
                   'bool', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Address::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Address::Serialize(uint8_t * buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Address::Set(uint32_t address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint32_t', 'address')])
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Address::Set(char const * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('char const *', 'address')])
    return

def register_Ns3Ipv4AddressHelper_methods(root_module, cls):
    ## ipv4-address-helper.h (module 'internet'): ns3::Ipv4AddressHelper::Ipv4AddressHelper(ns3::Ipv4AddressHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressHelper const &', 'arg0')])
    ## ipv4-address-helper.h (module 'internet'): ns3::Ipv4AddressHelper::Ipv4AddressHelper() [constructor]
    cls.add_constructor([])
    ## ipv4-address-helper.h (module 'internet'): ns3::Ipv4AddressHelper::Ipv4AddressHelper(ns3::Ipv4Address network, ns3::Ipv4Mask mask, ns3::Ipv4Address base="0.0.0.1") [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'mask'), param('ns3::Ipv4Address', 'base', default_value='"0.0.0.1"')])
    ## ipv4-address-helper.h (module 'internet'): ns3::Ipv4InterfaceContainer ns3::Ipv4AddressHelper::Assign(ns3::NetDeviceContainer const & c) [member function]
    cls.add_method('Assign', 
                   'ns3::Ipv4InterfaceContainer', 
                   [param('ns3::NetDeviceContainer const &', 'c')])
    ## ipv4-address-helper.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4AddressHelper::NewAddress() [member function]
    cls.add_method('NewAddress', 
                   'ns3::Ipv4Address', 
                   [])
    ## ipv4-address-helper.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4AddressHelper::NewNetwork() [member function]
    cls.add_method('NewNetwork', 
                   'ns3::Ipv4Address', 
                   [])
    ## ipv4-address-helper.h (module 'internet'): void ns3::Ipv4AddressHelper::SetBase(ns3::Ipv4Address network, ns3::Ipv4Mask mask, ns3::Ipv4Address base="0.0.0.1") [member function]
    cls.add_method('SetBase', 
                   'void', 
                   [param('ns3::Ipv4Address', 'network'), param('ns3::Ipv4Mask', 'mask'), param('ns3::Ipv4Address', 'base', default_value='"0.0.0.1"')])
    return

def register_Ns3Ipv4InterfaceAddress_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4InterfaceAddress::Ipv4InterfaceAddress() [constructor]
    cls.add_constructor([])
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4InterfaceAddress::Ipv4InterfaceAddress(ns3::Ipv4Address local, ns3::Ipv4Mask mask) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'local'), param('ns3::Ipv4Mask', 'mask')])
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4InterfaceAddress::Ipv4InterfaceAddress(ns3::Ipv4InterfaceAddress const & o) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4InterfaceAddress const &', 'o')])
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4InterfaceAddress::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4InterfaceAddress::GetLocal() const [member function]
    cls.add_method('GetLocal', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4Mask ns3::Ipv4InterfaceAddress::GetMask() const [member function]
    cls.add_method('GetMask', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h (module 'internet'): ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e ns3::Ipv4InterfaceAddress::GetScope() const [member function]
    cls.add_method('GetScope', 
                   'ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h (module 'internet'): bool ns3::Ipv4InterfaceAddress::IsSecondary() const [member function]
    cls.add_method('IsSecondary', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-interface-address.h (module 'internet'): void ns3::Ipv4InterfaceAddress::SetBroadcast(ns3::Ipv4Address broadcast) [member function]
    cls.add_method('SetBroadcast', 
                   'void', 
                   [param('ns3::Ipv4Address', 'broadcast')])
    ## ipv4-interface-address.h (module 'internet'): void ns3::Ipv4InterfaceAddress::SetLocal(ns3::Ipv4Address local) [member function]
    cls.add_method('SetLocal', 
                   'void', 
                   [param('ns3::Ipv4Address', 'local')])
    ## ipv4-interface-address.h (module 'internet'): void ns3::Ipv4InterfaceAddress::SetMask(ns3::Ipv4Mask mask) [member function]
    cls.add_method('SetMask', 
                   'void', 
                   [param('ns3::Ipv4Mask', 'mask')])
    ## ipv4-interface-address.h (module 'internet'): void ns3::Ipv4InterfaceAddress::SetPrimary() [member function]
    cls.add_method('SetPrimary', 
                   'void', 
                   [])
    ## ipv4-interface-address.h (module 'internet'): void ns3::Ipv4InterfaceAddress::SetScope(ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e scope) [member function]
    cls.add_method('SetScope', 
                   'void', 
                   [param('ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e', 'scope')])
    ## ipv4-interface-address.h (module 'internet'): void ns3::Ipv4InterfaceAddress::SetSecondary() [member function]
    cls.add_method('SetSecondary', 
                   'void', 
                   [])
    return

def register_Ns3Ipv4InterfaceContainer_methods(root_module, cls):
    ## ipv4-interface-container.h (module 'internet'): ns3::Ipv4InterfaceContainer::Ipv4InterfaceContainer(ns3::Ipv4InterfaceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4InterfaceContainer const &', 'arg0')])
    ## ipv4-interface-container.h (module 'internet'): ns3::Ipv4InterfaceContainer::Ipv4InterfaceContainer() [constructor]
    cls.add_constructor([])
    ## ipv4-interface-container.h (module 'internet'): void ns3::Ipv4InterfaceContainer::Add(ns3::Ipv4InterfaceContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ipv4InterfaceContainer', 'other')])
    ## ipv4-interface-container.h (module 'internet'): void ns3::Ipv4InterfaceContainer::Add(ns3::Ptr<ns3::Ipv4> ipv4, uint32_t interface) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4 >', 'ipv4'), param('uint32_t', 'interface')])
    ## ipv4-interface-container.h (module 'internet'): void ns3::Ipv4InterfaceContainer::Add(std::pair<ns3::Ptr<ns3::Ipv4>,unsigned int> ipInterfacePair) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::pair< ns3::Ptr< ns3::Ipv4 >, unsigned int >', 'ipInterfacePair')])
    ## ipv4-interface-container.h (module 'internet'): void ns3::Ipv4InterfaceContainer::Add(std::string ipv4Name, uint32_t interface) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'ipv4Name'), param('uint32_t', 'interface')])
    ## ipv4-interface-container.h (module 'internet'): __gnu_cxx::__normal_iterator<const std::pair<ns3::Ptr<ns3::Ipv4>, unsigned int>*,std::vector<std::pair<ns3::Ptr<ns3::Ipv4>, unsigned int>, std::allocator<std::pair<ns3::Ptr<ns3::Ipv4>, unsigned int> > > > ns3::Ipv4InterfaceContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< std::pair< ns3::Ptr< ns3::Ipv4 >, unsigned int > const, std::vector< std::pair< ns3::Ptr< ns3::Ipv4 >, unsigned int > > >', 
                   [], 
                   is_const=True)
    ## ipv4-interface-container.h (module 'internet'): __gnu_cxx::__normal_iterator<const std::pair<ns3::Ptr<ns3::Ipv4>, unsigned int>*,std::vector<std::pair<ns3::Ptr<ns3::Ipv4>, unsigned int>, std::allocator<std::pair<ns3::Ptr<ns3::Ipv4>, unsigned int> > > > ns3::Ipv4InterfaceContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< std::pair< ns3::Ptr< ns3::Ipv4 >, unsigned int > const, std::vector< std::pair< ns3::Ptr< ns3::Ipv4 >, unsigned int > > >', 
                   [], 
                   is_const=True)
    ## ipv4-interface-container.h (module 'internet'): std::pair<ns3::Ptr<ns3::Ipv4>,unsigned int> ns3::Ipv4InterfaceContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'std::pair< ns3::Ptr< ns3::Ipv4 >, unsigned int >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## ipv4-interface-container.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4InterfaceContainer::GetAddress(uint32_t i, uint32_t j=0) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4Address', 
                   [param('uint32_t', 'i'), param('uint32_t', 'j', default_value='0')], 
                   is_const=True)
    ## ipv4-interface-container.h (module 'internet'): uint32_t ns3::Ipv4InterfaceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-interface-container.h (module 'internet'): void ns3::Ipv4InterfaceContainer::SetMetric(uint32_t i, uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'i'), param('uint16_t', 'metric')])
    return

def register_Ns3Ipv4Mask_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask::Ipv4Mask(ns3::Ipv4Mask const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Mask const &', 'arg0')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask::Ipv4Mask() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask::Ipv4Mask(uint32_t mask) [constructor]
    cls.add_constructor([param('uint32_t', 'mask')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask::Ipv4Mask(char const * mask) [constructor]
    cls.add_constructor([param('char const *', 'mask')])
    ## ipv4-address.h (module 'network'): uint32_t ns3::Ipv4Mask::Get() const [member function]
    cls.add_method('Get', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): uint32_t ns3::Ipv4Mask::GetInverse() const [member function]
    cls.add_method('GetInverse', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Mask ns3::Ipv4Mask::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Mask ns3::Ipv4Mask::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): uint16_t ns3::Ipv4Mask::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Mask ns3::Ipv4Mask::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Mask::IsEqual(ns3::Ipv4Mask other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv4Mask', 'other')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Mask::IsMatch(ns3::Ipv4Address a, ns3::Ipv4Address b) const [member function]
    cls.add_method('IsMatch', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'a'), param('ns3::Ipv4Address', 'b')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Mask::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Mask::Set(uint32_t mask) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint32_t', 'mask')])
    return

def register_Ns3Ipv6Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address(char const * address) [constructor]
    cls.add_constructor([param('char const *', 'address')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address(uint8_t * address) [constructor]
    cls.add_constructor([param('uint8_t *', 'address')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address(ns3::Ipv6Address const & addr) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Address const &', 'addr')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address(ns3::Ipv6Address const * addr) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address const *', 'addr')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address ns3::Ipv6Address::CombinePrefix(ns3::Ipv6Prefix const & prefix) [member function]
    cls.add_method('CombinePrefix', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv6Prefix const &', 'prefix')])
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::Deserialize(uint8_t const * buf) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Ipv6Address', 
                   [param('uint8_t const *', 'buf')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetAllHostsMulticast() [member function]
    cls.add_method('GetAllHostsMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetAllNodesMulticast() [member function]
    cls.add_method('GetAllNodesMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetAllRoutersMulticast() [member function]
    cls.add_method('GetAllRoutersMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetAny() [member function]
    cls.add_method('GetAny', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::GetBytes(uint8_t * buf) const [member function]
    cls.add_method('GetBytes', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): ns3::Ipv4Address ns3::Ipv6Address::GetIpv4MappedAddress() const [member function]
    cls.add_method('GetIpv4MappedAddress', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsAllHostsMulticast() const [member function]
    cls.add_method('IsAllHostsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsAllNodesMulticast() const [member function]
    cls.add_method('IsAllNodesMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsAllRoutersMulticast() const [member function]
    cls.add_method('IsAllRoutersMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsAny() const [member function]
    cls.add_method('IsAny', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsEqual(ns3::Ipv6Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Address const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsIpv4MappedAddress() [member function]
    cls.add_method('IsIpv4MappedAddress', 
                   'bool', 
                   [])
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsLinkLocal() const [member function]
    cls.add_method('IsLinkLocal', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsLinkLocalMulticast() const [member function]
    cls.add_method('IsLinkLocalMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsLocalhost() const [member function]
    cls.add_method('IsLocalhost', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static bool ns3::Ipv6Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsSolicitedMulticast() const [member function]
    cls.add_method('IsSolicitedMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac48Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac48Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'mac')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeIpv4MappedAddress(ns3::Ipv4Address addr) [member function]
    cls.add_method('MakeIpv4MappedAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv4Address', 'addr')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeSolicitedAddress(ns3::Ipv6Address addr) [member function]
    cls.add_method('MakeSolicitedAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::Serialize(uint8_t * buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::Set(char const * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('char const *', 'address')])
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::Set(uint8_t * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint8_t *', 'address')])
    return

def register_Ns3Ipv6InterfaceAddress_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress::Ipv6InterfaceAddress() [constructor]
    cls.add_constructor([])
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress::Ipv6InterfaceAddress(ns3::Ipv6Address address) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'address')])
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress::Ipv6InterfaceAddress(ns3::Ipv6Address address, ns3::Ipv6Prefix prefix) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'address'), param('ns3::Ipv6Prefix', 'prefix')])
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress::Ipv6InterfaceAddress(ns3::Ipv6InterfaceAddress const & o) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6InterfaceAddress const &', 'o')])
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6Address ns3::Ipv6InterfaceAddress::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h (module 'internet'): uint32_t ns3::Ipv6InterfaceAddress::GetNsDadUid() const [member function]
    cls.add_method('GetNsDadUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6Prefix ns3::Ipv6InterfaceAddress::GetPrefix() const [member function]
    cls.add_method('GetPrefix', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress::Scope_e ns3::Ipv6InterfaceAddress::GetScope() const [member function]
    cls.add_method('GetScope', 
                   'ns3::Ipv6InterfaceAddress::Scope_e', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h (module 'internet'): ns3::Ipv6InterfaceAddress::State_e ns3::Ipv6InterfaceAddress::GetState() const [member function]
    cls.add_method('GetState', 
                   'ns3::Ipv6InterfaceAddress::State_e', 
                   [], 
                   is_const=True)
    ## ipv6-interface-address.h (module 'internet'): void ns3::Ipv6InterfaceAddress::SetAddress(ns3::Ipv6Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Ipv6Address', 'address')])
    ## ipv6-interface-address.h (module 'internet'): void ns3::Ipv6InterfaceAddress::SetNsDadUid(uint32_t uid) [member function]
    cls.add_method('SetNsDadUid', 
                   'void', 
                   [param('uint32_t', 'uid')])
    ## ipv6-interface-address.h (module 'internet'): void ns3::Ipv6InterfaceAddress::SetScope(ns3::Ipv6InterfaceAddress::Scope_e scope) [member function]
    cls.add_method('SetScope', 
                   'void', 
                   [param('ns3::Ipv6InterfaceAddress::Scope_e', 'scope')])
    ## ipv6-interface-address.h (module 'internet'): void ns3::Ipv6InterfaceAddress::SetState(ns3::Ipv6InterfaceAddress::State_e state) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::Ipv6InterfaceAddress::State_e', 'state')])
    return

def register_Ns3Ipv6Prefix_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(uint8_t * prefix) [constructor]
    cls.add_constructor([param('uint8_t *', 'prefix')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(char const * prefix) [constructor]
    cls.add_constructor([param('char const *', 'prefix')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(uint8_t prefix) [constructor]
    cls.add_constructor([param('uint8_t', 'prefix')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(ns3::Ipv6Prefix const & prefix) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const &', 'prefix')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(ns3::Ipv6Prefix const * prefix) [constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const *', 'prefix')])
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Prefix::GetBytes(uint8_t * buf) const [member function]
    cls.add_method('GetBytes', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): uint8_t ns3::Ipv6Prefix::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Prefix::IsEqual(ns3::Ipv6Prefix const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Prefix const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Prefix::IsMatch(ns3::Ipv6Address a, ns3::Ipv6Address b) const [member function]
    cls.add_method('IsMatch', 
                   'bool', 
                   [param('ns3::Ipv6Address', 'a'), param('ns3::Ipv6Address', 'b')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Prefix::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3LteEnbCmacSapProvider_methods(root_module, cls):
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LteEnbCmacSapProvider() [constructor]
    cls.add_constructor([])
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LteEnbCmacSapProvider(ns3::LteEnbCmacSapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteEnbCmacSapProvider const &', 'arg0')])
    ## lte-enb-cmac-sap.h (module 'lte'): void ns3::LteEnbCmacSapProvider::AddLc(ns3::LteEnbCmacSapProvider::LcInfo lcinfo, ns3::LteMacSapUser * msu) [member function]
    cls.add_method('AddLc', 
                   'void', 
                   [param('ns3::LteEnbCmacSapProvider::LcInfo', 'lcinfo'), param('ns3::LteMacSapUser *', 'msu')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-cmac-sap.h (module 'lte'): void ns3::LteEnbCmacSapProvider::AddUe(uint16_t rnti) [member function]
    cls.add_method('AddUe', 
                   'void', 
                   [param('uint16_t', 'rnti')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-cmac-sap.h (module 'lte'): void ns3::LteEnbCmacSapProvider::ConfigureMac(uint8_t ulBandwidth, uint8_t dlBandwidth) [member function]
    cls.add_method('ConfigureMac', 
                   'void', 
                   [param('uint8_t', 'ulBandwidth'), param('uint8_t', 'dlBandwidth')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-cmac-sap.h (module 'lte'): void ns3::LteEnbCmacSapProvider::ReconfigureLc(ns3::LteEnbCmacSapProvider::LcInfo lcinfo) [member function]
    cls.add_method('ReconfigureLc', 
                   'void', 
                   [param('ns3::LteEnbCmacSapProvider::LcInfo', 'lcinfo')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-cmac-sap.h (module 'lte'): void ns3::LteEnbCmacSapProvider::ReleaseLc(uint16_t rnti, uint8_t lcid) [member function]
    cls.add_method('ReleaseLc', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint8_t', 'lcid')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-cmac-sap.h (module 'lte'): void ns3::LteEnbCmacSapProvider::RrcUpdateConfigurationReq(ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters params) [member function]
    cls.add_method('RrcUpdateConfigurationReq', 
                   'void', 
                   [param('ns3::FfMacCschedSapProvider::CschedUeConfigReqParameters', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteEnbCmacSapProviderLcInfo_methods(root_module, cls):
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::LcInfo() [constructor]
    cls.add_constructor([])
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::LcInfo(ns3::LteEnbCmacSapProvider::LcInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteEnbCmacSapProvider::LcInfo const &', 'arg0')])
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::gbrDl [variable]
    cls.add_instance_attribute('gbrDl', 'uint64_t', is_const=False)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::gbrUl [variable]
    cls.add_instance_attribute('gbrUl', 'uint64_t', is_const=False)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::isGbr [variable]
    cls.add_instance_attribute('isGbr', 'bool', is_const=False)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::lcGroup [variable]
    cls.add_instance_attribute('lcGroup', 'uint8_t', is_const=False)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::lcId [variable]
    cls.add_instance_attribute('lcId', 'uint8_t', is_const=False)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::mbrDl [variable]
    cls.add_instance_attribute('mbrDl', 'uint64_t', is_const=False)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::mbrUl [variable]
    cls.add_instance_attribute('mbrUl', 'uint64_t', is_const=False)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::qci [variable]
    cls.add_instance_attribute('qci', 'uint8_t', is_const=False)
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapProvider::LcInfo::rnti [variable]
    cls.add_instance_attribute('rnti', 'uint16_t', is_const=False)
    return

def register_Ns3LteEnbCmacSapUser_methods(root_module, cls):
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapUser::LteEnbCmacSapUser() [constructor]
    cls.add_constructor([])
    ## lte-enb-cmac-sap.h (module 'lte'): ns3::LteEnbCmacSapUser::LteEnbCmacSapUser(ns3::LteEnbCmacSapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteEnbCmacSapUser const &', 'arg0')])
    ## lte-enb-cmac-sap.h (module 'lte'): void ns3::LteEnbCmacSapUser::NotifyLcConfigResult(uint16_t rnti, uint8_t lcid, bool success) [member function]
    cls.add_method('NotifyLcConfigResult', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint8_t', 'lcid'), param('bool', 'success')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-cmac-sap.h (module 'lte'): void ns3::LteEnbCmacSapUser::RrcConfigurationUpdateInd(ns3::LteUeConfig_t params) [member function]
    cls.add_method('RrcConfigurationUpdateInd', 
                   'void', 
                   [param('ns3::LteUeConfig_t', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteEnbPhySapProvider_methods(root_module, cls):
    ## lte-enb-phy-sap.h (module 'lte'): ns3::LteEnbPhySapProvider::LteEnbPhySapProvider() [constructor]
    cls.add_constructor([])
    ## lte-enb-phy-sap.h (module 'lte'): ns3::LteEnbPhySapProvider::LteEnbPhySapProvider(ns3::LteEnbPhySapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteEnbPhySapProvider const &', 'arg0')])
    ## lte-enb-phy-sap.h (module 'lte'): uint8_t ns3::LteEnbPhySapProvider::GetMacChTtiDelay() [member function]
    cls.add_method('GetMacChTtiDelay', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapProvider::SendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('SendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapProvider::SendMacPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('SendMacPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapProvider::SetBandwidth(uint8_t ulBandwidth, uint8_t dlBandwidth) [member function]
    cls.add_method('SetBandwidth', 
                   'void', 
                   [param('uint8_t', 'ulBandwidth'), param('uint8_t', 'dlBandwidth')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapProvider::SetCellId(uint16_t cellId) [member function]
    cls.add_method('SetCellId', 
                   'void', 
                   [param('uint16_t', 'cellId')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapProvider::SetTransmissionMode(uint16_t rnti, uint8_t txMode) [member function]
    cls.add_method('SetTransmissionMode', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint8_t', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteEnbPhySapUser_methods(root_module, cls):
    ## lte-enb-phy-sap.h (module 'lte'): ns3::LteEnbPhySapUser::LteEnbPhySapUser() [constructor]
    cls.add_constructor([])
    ## lte-enb-phy-sap.h (module 'lte'): ns3::LteEnbPhySapUser::LteEnbPhySapUser(ns3::LteEnbPhySapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteEnbPhySapUser const &', 'arg0')])
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapUser::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapUser::ReceivePhyPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('ReceivePhyPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapUser::SubframeIndication(uint32_t frameNo, uint32_t subframeNo) [member function]
    cls.add_method('SubframeIndication', 
                   'void', 
                   [param('uint32_t', 'frameNo'), param('uint32_t', 'subframeNo')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-enb-phy-sap.h (module 'lte'): void ns3::LteEnbPhySapUser::UlCqiReport(UlCqi_s ulcqi) [member function]
    cls.add_method('UlCqiReport', 
                   'void', 
                   [param('UlCqi_s', 'ulcqi')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteFfConverter_methods(root_module, cls):
    ## lte-common.h (module 'lte'): ns3::LteFfConverter::LteFfConverter() [constructor]
    cls.add_constructor([])
    ## lte-common.h (module 'lte'): ns3::LteFfConverter::LteFfConverter(ns3::LteFfConverter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteFfConverter const &', 'arg0')])
    ## lte-common.h (module 'lte'): static uint16_t ns3::LteFfConverter::double2fpS11dot3(double val) [member function]
    cls.add_method('double2fpS11dot3', 
                   'uint16_t', 
                   [param('double', 'val')], 
                   is_static=True)
    ## lte-common.h (module 'lte'): static double ns3::LteFfConverter::fpS11dot3toDouble(uint16_t val) [member function]
    cls.add_method('fpS11dot3toDouble', 
                   'double', 
                   [param('uint16_t', 'val')], 
                   is_static=True)
    ## lte-common.h (module 'lte'): static double ns3::LteFfConverter::getMinFpS11dot3Value() [member function]
    cls.add_method('getMinFpS11dot3Value', 
                   'double', 
                   [], 
                   is_static=True)
    return

def register_Ns3LteFlowId_t_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## lte-common.h (module 'lte'): ns3::LteFlowId_t::LteFlowId_t(ns3::LteFlowId_t const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteFlowId_t const &', 'arg0')])
    ## lte-common.h (module 'lte'): ns3::LteFlowId_t::LteFlowId_t() [constructor]
    cls.add_constructor([])
    ## lte-common.h (module 'lte'): ns3::LteFlowId_t::LteFlowId_t(uint16_t const a, uint8_t const b) [constructor]
    cls.add_constructor([param('uint16_t const', 'a'), param('uint8_t const', 'b')])
    ## lte-common.h (module 'lte'): ns3::LteFlowId_t::m_lcId [variable]
    cls.add_instance_attribute('m_lcId', 'uint8_t', is_const=False)
    ## lte-common.h (module 'lte'): ns3::LteFlowId_t::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    return

def register_Ns3LteMacSapProvider_methods(root_module, cls):
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::LteMacSapProvider() [constructor]
    cls.add_constructor([])
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::LteMacSapProvider(ns3::LteMacSapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMacSapProvider const &', 'arg0')])
    ## lte-mac-sap.h (module 'lte'): void ns3::LteMacSapProvider::ReportBufferStatus(ns3::LteMacSapProvider::ReportBufferStatusParameters params) [member function]
    cls.add_method('ReportBufferStatus', 
                   'void', 
                   [param('ns3::LteMacSapProvider::ReportBufferStatusParameters', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-mac-sap.h (module 'lte'): void ns3::LteMacSapProvider::TransmitPdu(ns3::LteMacSapProvider::TransmitPduParameters params) [member function]
    cls.add_method('TransmitPdu', 
                   'void', 
                   [param('ns3::LteMacSapProvider::TransmitPduParameters', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteMacSapProviderReportBufferStatusParameters_methods(root_module, cls):
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::ReportBufferStatusParameters() [constructor]
    cls.add_constructor([])
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::ReportBufferStatusParameters(ns3::LteMacSapProvider::ReportBufferStatusParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMacSapProvider::ReportBufferStatusParameters const &', 'arg0')])
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::lcid [variable]
    cls.add_instance_attribute('lcid', 'uint8_t', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::retxQueueHolDelay [variable]
    cls.add_instance_attribute('retxQueueHolDelay', 'uint16_t', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::retxQueueSize [variable]
    cls.add_instance_attribute('retxQueueSize', 'uint32_t', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::rnti [variable]
    cls.add_instance_attribute('rnti', 'uint16_t', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::statusPduSize [variable]
    cls.add_instance_attribute('statusPduSize', 'uint16_t', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::txQueueHolDelay [variable]
    cls.add_instance_attribute('txQueueHolDelay', 'uint16_t', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::ReportBufferStatusParameters::txQueueSize [variable]
    cls.add_instance_attribute('txQueueSize', 'uint32_t', is_const=False)
    return

def register_Ns3LteMacSapProviderTransmitPduParameters_methods(root_module, cls):
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::TransmitPduParameters::TransmitPduParameters() [constructor]
    cls.add_constructor([])
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::TransmitPduParameters::TransmitPduParameters(ns3::LteMacSapProvider::TransmitPduParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMacSapProvider::TransmitPduParameters const &', 'arg0')])
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::TransmitPduParameters::layer [variable]
    cls.add_instance_attribute('layer', 'uint8_t', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::TransmitPduParameters::lcid [variable]
    cls.add_instance_attribute('lcid', 'uint8_t', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::TransmitPduParameters::pdu [variable]
    cls.add_instance_attribute('pdu', 'ns3::Ptr< ns3::Packet >', is_const=False)
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapProvider::TransmitPduParameters::rnti [variable]
    cls.add_instance_attribute('rnti', 'uint16_t', is_const=False)
    return

def register_Ns3LteMacSapUser_methods(root_module, cls):
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapUser::LteMacSapUser() [constructor]
    cls.add_constructor([])
    ## lte-mac-sap.h (module 'lte'): ns3::LteMacSapUser::LteMacSapUser(ns3::LteMacSapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMacSapUser const &', 'arg0')])
    ## lte-mac-sap.h (module 'lte'): void ns3::LteMacSapUser::NotifyHarqDeliveryFailure() [member function]
    cls.add_method('NotifyHarqDeliveryFailure', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-mac-sap.h (module 'lte'): void ns3::LteMacSapUser::NotifyTxOpportunity(uint32_t bytes, uint8_t layer) [member function]
    cls.add_method('NotifyTxOpportunity', 
                   'void', 
                   [param('uint32_t', 'bytes'), param('uint8_t', 'layer')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-mac-sap.h (module 'lte'): void ns3::LteMacSapUser::ReceivePdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('ReceivePdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteMiErrorModel_methods(root_module, cls):
    ## lte-mi-error-model.h (module 'lte'): ns3::LteMiErrorModel::LteMiErrorModel() [constructor]
    cls.add_constructor([])
    ## lte-mi-error-model.h (module 'lte'): ns3::LteMiErrorModel::LteMiErrorModel(ns3::LteMiErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMiErrorModel const &', 'arg0')])
    ## lte-mi-error-model.h (module 'lte'): static double ns3::LteMiErrorModel::GetTbError(ns3::SpectrumValue const & sinr, std::vector<int, std::allocator<int> > const & map, uint16_t size, uint8_t mcs) [member function]
    cls.add_method('GetTbError', 
                   'double', 
                   [param('ns3::SpectrumValue const &', 'sinr'), param('std::vector< int > const &', 'map'), param('uint16_t', 'size'), param('uint8_t', 'mcs')], 
                   is_static=True)
    ## lte-mi-error-model.h (module 'lte'): static double ns3::LteMiErrorModel::MappingMiBler(double mib, uint8_t mcs, uint16_t cbSize) [member function]
    cls.add_method('MappingMiBler', 
                   'double', 
                   [param('double', 'mib'), param('uint8_t', 'mcs'), param('uint16_t', 'cbSize')], 
                   is_static=True)
    ## lte-mi-error-model.h (module 'lte'): static double ns3::LteMiErrorModel::Mib(ns3::SpectrumValue const & sinr, std::vector<int, std::allocator<int> > const & map, uint8_t mcs) [member function]
    cls.add_method('Mib', 
                   'double', 
                   [param('ns3::SpectrumValue const &', 'sinr'), param('std::vector< int > const &', 'map'), param('uint8_t', 'mcs')], 
                   is_static=True)
    return

def register_Ns3LtePdcpSapProvider_methods(root_module, cls):
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider::LtePdcpSapProvider() [constructor]
    cls.add_constructor([])
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider::LtePdcpSapProvider(ns3::LtePdcpSapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePdcpSapProvider const &', 'arg0')])
    ## lte-pdcp-sap.h (module 'lte'): void ns3::LtePdcpSapProvider::TransmitRrcPdu(ns3::LtePdcpSapProvider::TransmitRrcPduParameters params) [member function]
    cls.add_method('TransmitRrcPdu', 
                   'void', 
                   [param('ns3::LtePdcpSapProvider::TransmitRrcPduParameters', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LtePdcpSapProviderTransmitRrcPduParameters_methods(root_module, cls):
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider::TransmitRrcPduParameters::TransmitRrcPduParameters() [constructor]
    cls.add_constructor([])
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider::TransmitRrcPduParameters::TransmitRrcPduParameters(ns3::LtePdcpSapProvider::TransmitRrcPduParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePdcpSapProvider::TransmitRrcPduParameters const &', 'arg0')])
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider::TransmitRrcPduParameters::lcid [variable]
    cls.add_instance_attribute('lcid', 'uint8_t', is_const=False)
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider::TransmitRrcPduParameters::rnti [variable]
    cls.add_instance_attribute('rnti', 'uint16_t', is_const=False)
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapProvider::TransmitRrcPduParameters::rrcPdu [variable]
    cls.add_instance_attribute('rrcPdu', 'ns3::Ptr< ns3::Packet >', is_const=False)
    return

def register_Ns3LtePdcpSapUser_methods(root_module, cls):
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser::LtePdcpSapUser() [constructor]
    cls.add_constructor([])
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser::LtePdcpSapUser(ns3::LtePdcpSapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePdcpSapUser const &', 'arg0')])
    ## lte-pdcp-sap.h (module 'lte'): void ns3::LtePdcpSapUser::ReceiveRrcPdu(ns3::LtePdcpSapUser::ReceiveRrcPduParameters params) [member function]
    cls.add_method('ReceiveRrcPdu', 
                   'void', 
                   [param('ns3::LtePdcpSapUser::ReceiveRrcPduParameters', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LtePdcpSapUserReceiveRrcPduParameters_methods(root_module, cls):
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser::ReceiveRrcPduParameters::ReceiveRrcPduParameters() [constructor]
    cls.add_constructor([])
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser::ReceiveRrcPduParameters::ReceiveRrcPduParameters(ns3::LtePdcpSapUser::ReceiveRrcPduParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePdcpSapUser::ReceiveRrcPduParameters const &', 'arg0')])
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser::ReceiveRrcPduParameters::lcid [variable]
    cls.add_instance_attribute('lcid', 'uint8_t', is_const=False)
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser::ReceiveRrcPduParameters::rnti [variable]
    cls.add_instance_attribute('rnti', 'uint16_t', is_const=False)
    ## lte-pdcp-sap.h (module 'lte'): ns3::LtePdcpSapUser::ReceiveRrcPduParameters::rrcPdu [variable]
    cls.add_instance_attribute('rrcPdu', 'ns3::Ptr< ns3::Packet >', is_const=False)
    return

def register_Ns3LteRlcSapProvider_methods(root_module, cls):
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider::LteRlcSapProvider() [constructor]
    cls.add_constructor([])
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider::LteRlcSapProvider(ns3::LteRlcSapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcSapProvider const &', 'arg0')])
    ## lte-rlc-sap.h (module 'lte'): void ns3::LteRlcSapProvider::TransmitPdcpPdu(ns3::LteRlcSapProvider::TransmitPdcpPduParameters params) [member function]
    cls.add_method('TransmitPdcpPdu', 
                   'void', 
                   [param('ns3::LteRlcSapProvider::TransmitPdcpPduParameters', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteRlcSapProviderTransmitPdcpPduParameters_methods(root_module, cls):
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider::TransmitPdcpPduParameters::TransmitPdcpPduParameters() [constructor]
    cls.add_constructor([])
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider::TransmitPdcpPduParameters::TransmitPdcpPduParameters(ns3::LteRlcSapProvider::TransmitPdcpPduParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcSapProvider::TransmitPdcpPduParameters const &', 'arg0')])
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider::TransmitPdcpPduParameters::lcid [variable]
    cls.add_instance_attribute('lcid', 'uint8_t', is_const=False)
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider::TransmitPdcpPduParameters::pdcpPdu [variable]
    cls.add_instance_attribute('pdcpPdu', 'ns3::Ptr< ns3::Packet >', is_const=False)
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapProvider::TransmitPdcpPduParameters::rnti [variable]
    cls.add_instance_attribute('rnti', 'uint16_t', is_const=False)
    return

def register_Ns3LteRlcSapUser_methods(root_module, cls):
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapUser::LteRlcSapUser() [constructor]
    cls.add_constructor([])
    ## lte-rlc-sap.h (module 'lte'): ns3::LteRlcSapUser::LteRlcSapUser(ns3::LteRlcSapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcSapUser const &', 'arg0')])
    ## lte-rlc-sap.h (module 'lte'): void ns3::LteRlcSapUser::ReceivePdcpPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('ReceivePdcpPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteSpectrumValueHelper_methods(root_module, cls):
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::LteSpectrumValueHelper::LteSpectrumValueHelper() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::LteSpectrumValueHelper::LteSpectrumValueHelper(ns3::LteSpectrumValueHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteSpectrumValueHelper const &', 'arg0')])
    ## lte-spectrum-value-helper.h (module 'lte'): static ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateNoisePowerSpectralDensity(uint16_t earfcn, uint8_t bandwdith, double noiseFigure) [member function]
    cls.add_method('CreateNoisePowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('uint16_t', 'earfcn'), param('uint8_t', 'bandwdith'), param('double', 'noiseFigure')], 
                   is_static=True)
    ## lte-spectrum-value-helper.h (module 'lte'): static ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateNoisePowerSpectralDensity(double noiseFigure, ns3::Ptr<ns3::SpectrumModel> spectrumModel) [member function]
    cls.add_method('CreateNoisePowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'noiseFigure'), param('ns3::Ptr< ns3::SpectrumModel >', 'spectrumModel')], 
                   is_static=True)
    ## lte-spectrum-value-helper.h (module 'lte'): static ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateTxPowerSpectralDensity(uint16_t earfcn, uint8_t bandwdith, double powerTx, std::vector<int, std::allocator<int> > activeRbs) [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('uint16_t', 'earfcn'), param('uint8_t', 'bandwdith'), param('double', 'powerTx'), param('std::vector< int >', 'activeRbs')], 
                   is_static=True)
    ## lte-spectrum-value-helper.h (module 'lte'): static double ns3::LteSpectrumValueHelper::GetCarrierFrequency(uint16_t earfcn) [member function]
    cls.add_method('GetCarrierFrequency', 
                   'double', 
                   [param('uint16_t', 'earfcn')], 
                   is_static=True)
    ## lte-spectrum-value-helper.h (module 'lte'): static double ns3::LteSpectrumValueHelper::GetChannelBandwidth(uint8_t txBandwidthConf) [member function]
    cls.add_method('GetChannelBandwidth', 
                   'double', 
                   [param('uint8_t', 'txBandwidthConf')], 
                   is_static=True)
    ## lte-spectrum-value-helper.h (module 'lte'): static double ns3::LteSpectrumValueHelper::GetDownlinkCarrierFrequency(uint16_t earfcn) [member function]
    cls.add_method('GetDownlinkCarrierFrequency', 
                   'double', 
                   [param('uint16_t', 'earfcn')], 
                   is_static=True)
    ## lte-spectrum-value-helper.h (module 'lte'): static ns3::Ptr<ns3::SpectrumModel> ns3::LteSpectrumValueHelper::GetSpectrumModel(uint16_t earfcn, uint8_t bandwdith) [member function]
    cls.add_method('GetSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel >', 
                   [param('uint16_t', 'earfcn'), param('uint8_t', 'bandwdith')], 
                   is_static=True)
    ## lte-spectrum-value-helper.h (module 'lte'): static double ns3::LteSpectrumValueHelper::GetUplinkCarrierFrequency(uint16_t earfcn) [member function]
    cls.add_method('GetUplinkCarrierFrequency', 
                   'double', 
                   [param('uint16_t', 'earfcn')], 
                   is_static=True)
    return

def register_Ns3LteUeCmacSapProvider_methods(root_module, cls):
    ## lte-ue-cmac-sap.h (module 'lte'): ns3::LteUeCmacSapProvider::LteUeCmacSapProvider() [constructor]
    cls.add_constructor([])
    ## lte-ue-cmac-sap.h (module 'lte'): ns3::LteUeCmacSapProvider::LteUeCmacSapProvider(ns3::LteUeCmacSapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteUeCmacSapProvider const &', 'arg0')])
    ## lte-ue-cmac-sap.h (module 'lte'): void ns3::LteUeCmacSapProvider::AddLc(uint8_t lcId, ns3::LteMacSapUser * msu) [member function]
    cls.add_method('AddLc', 
                   'void', 
                   [param('uint8_t', 'lcId'), param('ns3::LteMacSapUser *', 'msu')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-ue-cmac-sap.h (module 'lte'): void ns3::LteUeCmacSapProvider::ConfigureUe(uint16_t rnti) [member function]
    cls.add_method('ConfigureUe', 
                   'void', 
                   [param('uint16_t', 'rnti')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-ue-cmac-sap.h (module 'lte'): void ns3::LteUeCmacSapProvider::RemoveLc(uint8_t lcId) [member function]
    cls.add_method('RemoveLc', 
                   'void', 
                   [param('uint8_t', 'lcId')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-ue-cmac-sap.h (module 'lte'): void ns3::LteUeCmacSapProvider::RrcUpdateConfigurationReq(ns3::LteUeConfig_t params) [member function]
    cls.add_method('RrcUpdateConfigurationReq', 
                   'void', 
                   [param('ns3::LteUeConfig_t', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteUeCmacSapUser_methods(root_module, cls):
    ## lte-ue-cmac-sap.h (module 'lte'): ns3::LteUeCmacSapUser::LteUeCmacSapUser() [constructor]
    cls.add_constructor([])
    ## lte-ue-cmac-sap.h (module 'lte'): ns3::LteUeCmacSapUser::LteUeCmacSapUser(ns3::LteUeCmacSapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteUeCmacSapUser const &', 'arg0')])
    ## lte-ue-cmac-sap.h (module 'lte'): void ns3::LteUeCmacSapUser::LcConfigCompleted() [member function]
    cls.add_method('LcConfigCompleted', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteUeConfig_t_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## lte-common.h (module 'lte'): ns3::LteUeConfig_t::LteUeConfig_t(ns3::LteUeConfig_t const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteUeConfig_t const &', 'arg0')])
    ## lte-common.h (module 'lte'): ns3::LteUeConfig_t::LteUeConfig_t() [constructor]
    cls.add_constructor([])
    ## lte-common.h (module 'lte'): ns3::LteUeConfig_t::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    ## lte-common.h (module 'lte'): ns3::LteUeConfig_t::m_transmissionMode [variable]
    cls.add_instance_attribute('m_transmissionMode', 'uint8_t', is_const=False)
    return

def register_Ns3LteUePhySapProvider_methods(root_module, cls):
    ## lte-ue-phy-sap.h (module 'lte'): ns3::LteUePhySapProvider::LteUePhySapProvider() [constructor]
    cls.add_constructor([])
    ## lte-ue-phy-sap.h (module 'lte'): ns3::LteUePhySapProvider::LteUePhySapProvider(ns3::LteUePhySapProvider const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteUePhySapProvider const &', 'arg0')])
    ## lte-ue-phy-sap.h (module 'lte'): void ns3::LteUePhySapProvider::SendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('SendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-ue-phy-sap.h (module 'lte'): void ns3::LteUePhySapProvider::SendMacPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('SendMacPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-ue-phy-sap.h (module 'lte'): void ns3::LteUePhySapProvider::SetBandwidth(uint8_t ulBandwidth, uint8_t dlBandwidth) [member function]
    cls.add_method('SetBandwidth', 
                   'void', 
                   [param('uint8_t', 'ulBandwidth'), param('uint8_t', 'dlBandwidth')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-ue-phy-sap.h (module 'lte'): void ns3::LteUePhySapProvider::SetTransmissionMode(uint8_t txMode) [member function]
    cls.add_method('SetTransmissionMode', 
                   'void', 
                   [param('uint8_t', 'txMode')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteUePhySapUser_methods(root_module, cls):
    ## lte-ue-phy-sap.h (module 'lte'): ns3::LteUePhySapUser::LteUePhySapUser() [constructor]
    cls.add_constructor([])
    ## lte-ue-phy-sap.h (module 'lte'): ns3::LteUePhySapUser::LteUePhySapUser(ns3::LteUePhySapUser const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteUePhySapUser const &', 'arg0')])
    ## lte-ue-phy-sap.h (module 'lte'): void ns3::LteUePhySapUser::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-ue-phy-sap.h (module 'lte'): void ns3::LteUePhySapUser::ReceivePhyPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('ReceivePhyPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-ue-phy-sap.h (module 'lte'): void ns3::LteUePhySapUser::SubframeIndication(uint32_t frameNo, uint32_t subframeNo) [member function]
    cls.add_method('SubframeIndication', 
                   'void', 
                   [param('uint32_t', 'frameNo'), param('uint32_t', 'subframeNo')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3Mac48Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## mac48-address.h (module 'network'): ns3::Mac48Address::Mac48Address(ns3::Mac48Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48Address const &', 'arg0')])
    ## mac48-address.h (module 'network'): ns3::Mac48Address::Mac48Address() [constructor]
    cls.add_constructor([])
    ## mac48-address.h (module 'network'): ns3::Mac48Address::Mac48Address(char const * str) [constructor]
    cls.add_constructor([param('char const *', 'str')])
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Mac48Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## mac48-address.h (module 'network'): void ns3::Mac48Address::CopyFrom(uint8_t const * buffer) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('uint8_t const *', 'buffer')])
    ## mac48-address.h (module 'network'): void ns3::Mac48Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'void', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetMulticast(ns3::Ipv4Address address) [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Mac48Address', 
                   [param('ns3::Ipv4Address', 'address')], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetMulticast(ns3::Ipv6Address address) [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Mac48Address', 
                   [param('ns3::Ipv6Address', 'address')], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetMulticast6Prefix() [member function]
    cls.add_method('GetMulticast6Prefix', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetMulticastPrefix() [member function]
    cls.add_method('GetMulticastPrefix', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h (module 'network'): bool ns3::Mac48Address::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac48-address.h (module 'network'): bool ns3::Mac48Address::IsGroup() const [member function]
    cls.add_method('IsGroup', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac48-address.h (module 'network'): static bool ns3::Mac48Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    return

def register_Ns3Names_methods(root_module, cls):
    ## names.h (module 'core'): ns3::Names::Names() [constructor]
    cls.add_constructor([])
    ## names.h (module 'core'): ns3::Names::Names(ns3::Names const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Names const &', 'arg0')])
    ## names.h (module 'core'): static void ns3::Names::Add(std::string name, ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h (module 'core'): static void ns3::Names::Add(std::string path, std::string name, ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'path'), param('std::string', 'name'), param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h (module 'core'): static void ns3::Names::Add(ns3::Ptr<ns3::Object> context, std::string name, ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'context'), param('std::string', 'name'), param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h (module 'core'): static void ns3::Names::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_static=True)
    ## names.h (module 'core'): static std::string ns3::Names::FindName(ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('FindName', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h (module 'core'): static std::string ns3::Names::FindPath(ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('FindPath', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h (module 'core'): static void ns3::Names::Rename(std::string oldpath, std::string newname) [member function]
    cls.add_method('Rename', 
                   'void', 
                   [param('std::string', 'oldpath'), param('std::string', 'newname')], 
                   is_static=True)
    ## names.h (module 'core'): static void ns3::Names::Rename(std::string path, std::string oldname, std::string newname) [member function]
    cls.add_method('Rename', 
                   'void', 
                   [param('std::string', 'path'), param('std::string', 'oldname'), param('std::string', 'newname')], 
                   is_static=True)
    ## names.h (module 'core'): static void ns3::Names::Rename(ns3::Ptr<ns3::Object> context, std::string oldname, std::string newname) [member function]
    cls.add_method('Rename', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'context'), param('std::string', 'oldname'), param('std::string', 'newname')], 
                   is_static=True)
    return

def register_Ns3NetDeviceContainer_methods(root_module, cls):
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer(ns3::NetDeviceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDeviceContainer const &', 'arg0')])
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer() [constructor]
    cls.add_constructor([])
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer(ns3::Ptr<ns3::NetDevice> dev) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'dev')])
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer(std::string devName) [constructor]
    cls.add_constructor([param('std::string', 'devName')])
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer(ns3::NetDeviceContainer const & a, ns3::NetDeviceContainer const & b) [constructor]
    cls.add_constructor([param('ns3::NetDeviceContainer const &', 'a'), param('ns3::NetDeviceContainer const &', 'b')])
    ## net-device-container.h (module 'network'): void ns3::NetDeviceContainer::Add(ns3::NetDeviceContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::NetDeviceContainer', 'other')])
    ## net-device-container.h (module 'network'): void ns3::NetDeviceContainer::Add(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## net-device-container.h (module 'network'): void ns3::NetDeviceContainer::Add(std::string deviceName) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'deviceName')])
    ## net-device-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::NetDevice>*,std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > > ns3::NetDeviceContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::NetDevice > const, std::vector< ns3::Ptr< ns3::NetDevice > > >', 
                   [], 
                   is_const=True)
    ## net-device-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::NetDevice>*,std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > > ns3::NetDeviceContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::NetDevice > const, std::vector< ns3::Ptr< ns3::NetDevice > > >', 
                   [], 
                   is_const=True)
    ## net-device-container.h (module 'network'): ns3::Ptr<ns3::NetDevice> ns3::NetDeviceContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## net-device-container.h (module 'network'): uint32_t ns3::NetDeviceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3NodeContainer_methods(root_module, cls):
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'arg0')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer() [constructor]
    cls.add_constructor([])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(std::string nodeName) [constructor]
    cls.add_constructor([param('std::string', 'nodeName')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c, ns3::NodeContainer const & d) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c'), param('ns3::NodeContainer const &', 'd')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c, ns3::NodeContainer const & d, ns3::NodeContainer const & e) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c'), param('ns3::NodeContainer const &', 'd'), param('ns3::NodeContainer const &', 'e')])
    ## node-container.h (module 'network'): void ns3::NodeContainer::Add(ns3::NodeContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::NodeContainer', 'other')])
    ## node-container.h (module 'network'): void ns3::NodeContainer::Add(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## node-container.h (module 'network'): void ns3::NodeContainer::Add(std::string nodeName) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'nodeName')])
    ## node-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h (module 'network'): void ns3::NodeContainer::Create(uint32_t n) [member function]
    cls.add_method('Create', 
                   'void', 
                   [param('uint32_t', 'n')])
    ## node-container.h (module 'network'): void ns3::NodeContainer::Create(uint32_t n, uint32_t systemId) [member function]
    cls.add_method('Create', 
                   'void', 
                   [param('uint32_t', 'n'), param('uint32_t', 'systemId')])
    ## node-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h (module 'network'): ns3::Ptr<ns3::Node> ns3::NodeContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## node-container.h (module 'network'): static ns3::NodeContainer ns3::NodeContainer::GetGlobal() [member function]
    cls.add_method('GetGlobal', 
                   'ns3::NodeContainer', 
                   [], 
                   is_static=True)
    ## node-container.h (module 'network'): uint32_t ns3::NodeContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3ObjectBase_methods(root_module, cls):
    ## object-base.h (module 'core'): ns3::ObjectBase::ObjectBase() [constructor]
    cls.add_constructor([])
    ## object-base.h (module 'core'): ns3::ObjectBase::ObjectBase(ns3::ObjectBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectBase const &', 'arg0')])
    ## object-base.h (module 'core'): void ns3::ObjectBase::GetAttribute(std::string name, ns3::AttributeValue & value) const [member function]
    cls.add_method('GetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'value')], 
                   is_const=True)
    ## object-base.h (module 'core'): bool ns3::ObjectBase::GetAttributeFailSafe(std::string name, ns3::AttributeValue & attribute) const [member function]
    cls.add_method('GetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'attribute')], 
                   is_const=True)
    ## object-base.h (module 'core'): ns3::TypeId ns3::ObjectBase::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## object-base.h (module 'core'): static ns3::TypeId ns3::ObjectBase::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## object-base.h (module 'core'): void ns3::ObjectBase::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::SetAttributeFailSafe(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::TraceConnect(std::string name, std::string context, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceConnect', 
                   'bool', 
                   [param('std::string', 'name'), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::TraceConnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceConnectWithoutContext', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::TraceDisconnect(std::string name, std::string context, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceDisconnect', 
                   'bool', 
                   [param('std::string', 'name'), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::TraceDisconnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceDisconnectWithoutContext', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h (module 'core'): void ns3::ObjectBase::ConstructSelf(ns3::AttributeConstructionList const & attributes) [member function]
    cls.add_method('ConstructSelf', 
                   'void', 
                   [param('ns3::AttributeConstructionList const &', 'attributes')], 
                   visibility='protected')
    ## object-base.h (module 'core'): void ns3::ObjectBase::NotifyConstructionCompleted() [member function]
    cls.add_method('NotifyConstructionCompleted', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3ObjectDeleter_methods(root_module, cls):
    ## object.h (module 'core'): ns3::ObjectDeleter::ObjectDeleter() [constructor]
    cls.add_constructor([])
    ## object.h (module 'core'): ns3::ObjectDeleter::ObjectDeleter(ns3::ObjectDeleter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectDeleter const &', 'arg0')])
    ## object.h (module 'core'): static void ns3::ObjectDeleter::Delete(ns3::Object * object) [member function]
    cls.add_method('Delete', 
                   'void', 
                   [param('ns3::Object *', 'object')], 
                   is_static=True)
    return

def register_Ns3ObjectFactory_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## object-factory.h (module 'core'): ns3::ObjectFactory::ObjectFactory(ns3::ObjectFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactory const &', 'arg0')])
    ## object-factory.h (module 'core'): ns3::ObjectFactory::ObjectFactory() [constructor]
    cls.add_constructor([])
    ## object-factory.h (module 'core'): ns3::ObjectFactory::ObjectFactory(std::string typeId) [constructor]
    cls.add_constructor([param('std::string', 'typeId')])
    ## object-factory.h (module 'core'): ns3::Ptr<ns3::Object> ns3::ObjectFactory::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## object-factory.h (module 'core'): ns3::TypeId ns3::ObjectFactory::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::SetTypeId(ns3::TypeId tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('ns3::TypeId', 'tid')])
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::SetTypeId(char const * tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('char const *', 'tid')])
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::SetTypeId(std::string tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('std::string', 'tid')])
    return

def register_Ns3PacketMetadata_methods(root_module, cls):
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::PacketMetadata(uint64_t uid, uint32_t size) [constructor]
    cls.add_constructor([param('uint64_t', 'uid'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::PacketMetadata(ns3::PacketMetadata const & o) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata const &', 'o')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::AddAtEnd(ns3::PacketMetadata const & o) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::PacketMetadata const &', 'o')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::AddHeader(ns3::Header const & header, uint32_t size) [member function]
    cls.add_method('AddHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::AddPaddingAtEnd(uint32_t end) [member function]
    cls.add_method('AddPaddingAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::AddTrailer(ns3::Trailer const & trailer, uint32_t size) [member function]
    cls.add_method('AddTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::ItemIterator ns3::PacketMetadata::BeginItem(ns3::Buffer buffer) const [member function]
    cls.add_method('BeginItem', 
                   'ns3::PacketMetadata::ItemIterator', 
                   [param('ns3::Buffer', 'buffer')], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata ns3::PacketMetadata::CreateFragment(uint32_t start, uint32_t end) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::PacketMetadata', 
                   [param('uint32_t', 'start'), param('uint32_t', 'end')], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): uint32_t ns3::PacketMetadata::Deserialize(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): static void ns3::PacketMetadata::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet-metadata.h (module 'network'): static void ns3::PacketMetadata::EnableChecking() [member function]
    cls.add_method('EnableChecking', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet-metadata.h (module 'network'): uint32_t ns3::PacketMetadata::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): uint64_t ns3::PacketMetadata::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::RemoveAtEnd(uint32_t end) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::RemoveAtStart(uint32_t start) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'start')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::RemoveHeader(ns3::Header const & header, uint32_t size) [member function]
    cls.add_method('RemoveHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::RemoveTrailer(ns3::Trailer const & trailer, uint32_t size) [member function]
    cls.add_method('RemoveTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): uint32_t ns3::PacketMetadata::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3PacketMetadataItem_methods(root_module, cls):
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::Item() [constructor]
    cls.add_constructor([])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::Item(ns3::PacketMetadata::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata::Item const &', 'arg0')])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::current [variable]
    cls.add_instance_attribute('current', 'ns3::Buffer::Iterator', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::currentSize [variable]
    cls.add_instance_attribute('currentSize', 'uint32_t', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::currentTrimedFromEnd [variable]
    cls.add_instance_attribute('currentTrimedFromEnd', 'uint32_t', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::currentTrimedFromStart [variable]
    cls.add_instance_attribute('currentTrimedFromStart', 'uint32_t', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::isFragment [variable]
    cls.add_instance_attribute('isFragment', 'bool', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3PacketMetadataItemIterator_methods(root_module, cls):
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::ItemIterator::ItemIterator(ns3::PacketMetadata::ItemIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata::ItemIterator const &', 'arg0')])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::ItemIterator::ItemIterator(ns3::PacketMetadata const * metadata, ns3::Buffer buffer) [constructor]
    cls.add_constructor([param('ns3::PacketMetadata const *', 'metadata'), param('ns3::Buffer', 'buffer')])
    ## packet-metadata.h (module 'network'): bool ns3::PacketMetadata::ItemIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item ns3::PacketMetadata::ItemIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::PacketMetadata::Item', 
                   [])
    return

def register_Ns3PacketTagIterator_methods(root_module, cls):
    ## packet.h (module 'network'): ns3::PacketTagIterator::PacketTagIterator(ns3::PacketTagIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagIterator const &', 'arg0')])
    ## packet.h (module 'network'): bool ns3::PacketTagIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::PacketTagIterator::Item ns3::PacketTagIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::PacketTagIterator::Item', 
                   [])
    return

def register_Ns3PacketTagIteratorItem_methods(root_module, cls):
    ## packet.h (module 'network'): ns3::PacketTagIterator::Item::Item(ns3::PacketTagIterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagIterator::Item const &', 'arg0')])
    ## packet.h (module 'network'): void ns3::PacketTagIterator::Item::GetTag(ns3::Tag & tag) const [member function]
    cls.add_method('GetTag', 
                   'void', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::TypeId ns3::PacketTagIterator::Item::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    return

def register_Ns3PacketTagList_methods(root_module, cls):
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::PacketTagList() [constructor]
    cls.add_constructor([])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::PacketTagList(ns3::PacketTagList const & o) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagList const &', 'o')])
    ## packet-tag-list.h (module 'network'): void ns3::PacketTagList::Add(ns3::Tag const & tag) const [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData const * ns3::PacketTagList::Head() const [member function]
    cls.add_method('Head', 
                   'ns3::PacketTagList::TagData const *', 
                   [], 
                   is_const=True)
    ## packet-tag-list.h (module 'network'): bool ns3::PacketTagList::Peek(ns3::Tag & tag) const [member function]
    cls.add_method('Peek', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet-tag-list.h (module 'network'): bool ns3::PacketTagList::Remove(ns3::Tag & tag) [member function]
    cls.add_method('Remove', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    ## packet-tag-list.h (module 'network'): void ns3::PacketTagList::RemoveAll() [member function]
    cls.add_method('RemoveAll', 
                   'void', 
                   [])
    return

def register_Ns3PacketTagListTagData_methods(root_module, cls):
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::TagData() [constructor]
    cls.add_constructor([])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::TagData(ns3::PacketTagList::TagData const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagList::TagData const &', 'arg0')])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::count [variable]
    cls.add_instance_attribute('count', 'uint32_t', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::data [variable]
    cls.add_instance_attribute('data', 'uint8_t [ 20 ]', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::next [variable]
    cls.add_instance_attribute('next', 'ns3::PacketTagList::TagData *', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3SequenceNumber10_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_numeric_operator('+', root_module['ns3::SequenceNumber10'], root_module['ns3::SequenceNumber10'], param('uint16_t', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::SequenceNumber10'], root_module['ns3::SequenceNumber10'], param('uint16_t', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## lte-rlc-sequence-number.h (module 'lte'): ns3::SequenceNumber10::SequenceNumber10() [constructor]
    cls.add_constructor([])
    ## lte-rlc-sequence-number.h (module 'lte'): ns3::SequenceNumber10::SequenceNumber10(uint16_t value) [constructor]
    cls.add_constructor([param('uint16_t', 'value')])
    ## lte-rlc-sequence-number.h (module 'lte'): ns3::SequenceNumber10::SequenceNumber10(ns3::SequenceNumber10 const & value) [copy constructor]
    cls.add_constructor([param('ns3::SequenceNumber10 const &', 'value')])
    ## lte-rlc-sequence-number.h (module 'lte'): uint16_t ns3::SequenceNumber10::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-sequence-number.h (module 'lte'): void ns3::SequenceNumber10::SetModulusBase(ns3::SequenceNumber10 modulusBase) [member function]
    cls.add_method('SetModulusBase', 
                   'void', 
                   [param('ns3::SequenceNumber10', 'modulusBase')])
    ## lte-rlc-sequence-number.h (module 'lte'): void ns3::SequenceNumber10::SetModulusBase(uint16_t modulusBase) [member function]
    cls.add_method('SetModulusBase', 
                   'void', 
                   [param('uint16_t', 'modulusBase')])
    return

def register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::SimpleRefCount(ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3Simulator_methods(root_module, cls):
    ## simulator.h (module 'core'): ns3::Simulator::Simulator(ns3::Simulator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Simulator const &', 'arg0')])
    ## simulator.h (module 'core'): static void ns3::Simulator::Cancel(ns3::EventId const & id) [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [param('ns3::EventId const &', 'id')], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static uint32_t ns3::Simulator::GetContext() [member function]
    cls.add_method('GetContext', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static ns3::Time ns3::Simulator::GetDelayLeft(ns3::EventId const & id) [member function]
    cls.add_method('GetDelayLeft', 
                   'ns3::Time', 
                   [param('ns3::EventId const &', 'id')], 
                   is_static=True)
    ## simulator.h (module 'core'): static ns3::Ptr<ns3::SimulatorImpl> ns3::Simulator::GetImplementation() [member function]
    cls.add_method('GetImplementation', 
                   'ns3::Ptr< ns3::SimulatorImpl >', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static ns3::Time ns3::Simulator::GetMaximumSimulationTime() [member function]
    cls.add_method('GetMaximumSimulationTime', 
                   'ns3::Time', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static uint32_t ns3::Simulator::GetSystemId() [member function]
    cls.add_method('GetSystemId', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static bool ns3::Simulator::IsExpired(ns3::EventId const & id) [member function]
    cls.add_method('IsExpired', 
                   'bool', 
                   [param('ns3::EventId const &', 'id')], 
                   is_static=True)
    ## simulator.h (module 'core'): static bool ns3::Simulator::IsFinished() [member function]
    cls.add_method('IsFinished', 
                   'bool', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static ns3::Time ns3::Simulator::Now() [member function]
    cls.add_method('Now', 
                   'ns3::Time', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::Remove(ns3::EventId const & id) [member function]
    cls.add_method('Remove', 
                   'void', 
                   [param('ns3::EventId const &', 'id')], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::SetImplementation(ns3::Ptr<ns3::SimulatorImpl> impl) [member function]
    cls.add_method('SetImplementation', 
                   'void', 
                   [param('ns3::Ptr< ns3::SimulatorImpl >', 'impl')], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::SetScheduler(ns3::ObjectFactory schedulerFactory) [member function]
    cls.add_method('SetScheduler', 
                   'void', 
                   [param('ns3::ObjectFactory', 'schedulerFactory')], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::Stop(ns3::Time const & time) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time const &', 'time')], 
                   is_static=True)
    return

def register_Ns3StatisticalSummary_methods(root_module, cls):
    ## data-calculator.h (module 'stats'): ns3::StatisticalSummary::StatisticalSummary() [constructor]
    cls.add_constructor([])
    ## data-calculator.h (module 'stats'): ns3::StatisticalSummary::StatisticalSummary(ns3::StatisticalSummary const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StatisticalSummary const &', 'arg0')])
    ## data-calculator.h (module 'stats'): long int ns3::StatisticalSummary::getCount() const [member function]
    cls.add_method('getCount', 
                   'long int', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h (module 'stats'): double ns3::StatisticalSummary::getMax() const [member function]
    cls.add_method('getMax', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h (module 'stats'): double ns3::StatisticalSummary::getMean() const [member function]
    cls.add_method('getMean', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h (module 'stats'): double ns3::StatisticalSummary::getMin() const [member function]
    cls.add_method('getMin', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h (module 'stats'): double ns3::StatisticalSummary::getSqrSum() const [member function]
    cls.add_method('getSqrSum', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h (module 'stats'): double ns3::StatisticalSummary::getStddev() const [member function]
    cls.add_method('getStddev', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h (module 'stats'): double ns3::StatisticalSummary::getSum() const [member function]
    cls.add_method('getSum', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h (module 'stats'): double ns3::StatisticalSummary::getVariance() const [member function]
    cls.add_method('getVariance', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3Tag_methods(root_module, cls):
    ## tag.h (module 'network'): ns3::Tag::Tag() [constructor]
    cls.add_constructor([])
    ## tag.h (module 'network'): ns3::Tag::Tag(ns3::Tag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Tag const &', 'arg0')])
    ## tag.h (module 'network'): void ns3::Tag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_pure_virtual=True, is_virtual=True)
    ## tag.h (module 'network'): uint32_t ns3::Tag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## tag.h (module 'network'): static ns3::TypeId ns3::Tag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tag.h (module 'network'): void ns3::Tag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## tag.h (module 'network'): void ns3::Tag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3TagBuffer_methods(root_module, cls):
    ## tag-buffer.h (module 'network'): ns3::TagBuffer::TagBuffer(ns3::TagBuffer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TagBuffer const &', 'arg0')])
    ## tag-buffer.h (module 'network'): ns3::TagBuffer::TagBuffer(uint8_t * start, uint8_t * end) [constructor]
    cls.add_constructor([param('uint8_t *', 'start'), param('uint8_t *', 'end')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::CopyFrom(ns3::TagBuffer o) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('ns3::TagBuffer', 'o')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::Read(uint8_t * buffer, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')])
    ## tag-buffer.h (module 'network'): double ns3::TagBuffer::ReadDouble() [member function]
    cls.add_method('ReadDouble', 
                   'double', 
                   [])
    ## tag-buffer.h (module 'network'): uint16_t ns3::TagBuffer::ReadU16() [member function]
    cls.add_method('ReadU16', 
                   'uint16_t', 
                   [])
    ## tag-buffer.h (module 'network'): uint32_t ns3::TagBuffer::ReadU32() [member function]
    cls.add_method('ReadU32', 
                   'uint32_t', 
                   [])
    ## tag-buffer.h (module 'network'): uint64_t ns3::TagBuffer::ReadU64() [member function]
    cls.add_method('ReadU64', 
                   'uint64_t', 
                   [])
    ## tag-buffer.h (module 'network'): uint8_t ns3::TagBuffer::ReadU8() [member function]
    cls.add_method('ReadU8', 
                   'uint8_t', 
                   [])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::TrimAtEnd(uint32_t trim) [member function]
    cls.add_method('TrimAtEnd', 
                   'void', 
                   [param('uint32_t', 'trim')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::Write(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteDouble(double v) [member function]
    cls.add_method('WriteDouble', 
                   'void', 
                   [param('double', 'v')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteU16(uint16_t data) [member function]
    cls.add_method('WriteU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteU32(uint32_t data) [member function]
    cls.add_method('WriteU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteU64(uint64_t v) [member function]
    cls.add_method('WriteU64', 
                   'void', 
                   [param('uint64_t', 'v')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteU8(uint8_t v) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'v')])
    return

def register_Ns3TbId_t_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## lte-spectrum-phy.h (module 'lte'): ns3::TbId_t::TbId_t(ns3::TbId_t const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TbId_t const &', 'arg0')])
    ## lte-spectrum-phy.h (module 'lte'): ns3::TbId_t::TbId_t() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-phy.h (module 'lte'): ns3::TbId_t::TbId_t(uint16_t const a, uint8_t const b) [constructor]
    cls.add_constructor([param('uint16_t const', 'a'), param('uint8_t const', 'b')])
    ## lte-spectrum-phy.h (module 'lte'): ns3::TbId_t::m_layer [variable]
    cls.add_instance_attribute('m_layer', 'uint8_t', is_const=False)
    ## lte-spectrum-phy.h (module 'lte'): ns3::TbId_t::m_rnti [variable]
    cls.add_instance_attribute('m_rnti', 'uint16_t', is_const=False)
    return

def register_Ns3Timer_methods(root_module, cls):
    ## timer.h (module 'core'): ns3::Timer::Timer(ns3::Timer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Timer const &', 'arg0')])
    ## timer.h (module 'core'): ns3::Timer::Timer() [constructor]
    cls.add_constructor([])
    ## timer.h (module 'core'): ns3::Timer::Timer(ns3::Timer::DestroyPolicy destroyPolicy) [constructor]
    cls.add_constructor([param('ns3::Timer::DestroyPolicy', 'destroyPolicy')])
    ## timer.h (module 'core'): void ns3::Timer::Cancel() [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [])
    ## timer.h (module 'core'): ns3::Time ns3::Timer::GetDelay() const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## timer.h (module 'core'): ns3::Time ns3::Timer::GetDelayLeft() const [member function]
    cls.add_method('GetDelayLeft', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## timer.h (module 'core'): ns3::Timer::State ns3::Timer::GetState() const [member function]
    cls.add_method('GetState', 
                   'ns3::Timer::State', 
                   [], 
                   is_const=True)
    ## timer.h (module 'core'): bool ns3::Timer::IsExpired() const [member function]
    cls.add_method('IsExpired', 
                   'bool', 
                   [], 
                   is_const=True)
    ## timer.h (module 'core'): bool ns3::Timer::IsRunning() const [member function]
    cls.add_method('IsRunning', 
                   'bool', 
                   [], 
                   is_const=True)
    ## timer.h (module 'core'): bool ns3::Timer::IsSuspended() const [member function]
    cls.add_method('IsSuspended', 
                   'bool', 
                   [], 
                   is_const=True)
    ## timer.h (module 'core'): void ns3::Timer::Remove() [member function]
    cls.add_method('Remove', 
                   'void', 
                   [])
    ## timer.h (module 'core'): void ns3::Timer::Resume() [member function]
    cls.add_method('Resume', 
                   'void', 
                   [])
    ## timer.h (module 'core'): void ns3::Timer::Schedule() [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [])
    ## timer.h (module 'core'): void ns3::Timer::Schedule(ns3::Time delay) [member function]
    cls.add_method('Schedule', 
                   'void', 
                   [param('ns3::Time', 'delay')])
    ## timer.h (module 'core'): void ns3::Timer::SetDelay(ns3::Time const & delay) [member function]
    cls.add_method('SetDelay', 
                   'void', 
                   [param('ns3::Time const &', 'delay')])
    ## timer.h (module 'core'): void ns3::Timer::Suspend() [member function]
    cls.add_method('Suspend', 
                   'void', 
                   [])
    return

def register_Ns3TimerImpl_methods(root_module, cls):
    ## timer-impl.h (module 'core'): ns3::TimerImpl::TimerImpl() [constructor]
    cls.add_constructor([])
    ## timer-impl.h (module 'core'): ns3::TimerImpl::TimerImpl(ns3::TimerImpl const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TimerImpl const &', 'arg0')])
    ## timer-impl.h (module 'core'): void ns3::TimerImpl::Invoke() [member function]
    cls.add_method('Invoke', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## timer-impl.h (module 'core'): ns3::EventId ns3::TimerImpl::Schedule(ns3::Time const & delay) [member function]
    cls.add_method('Schedule', 
                   'ns3::EventId', 
                   [param('ns3::Time const &', 'delay')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3TransmissionModesLayers_methods(root_module, cls):
    ## lte-common.h (module 'lte'): ns3::TransmissionModesLayers::TransmissionModesLayers() [constructor]
    cls.add_constructor([])
    ## lte-common.h (module 'lte'): ns3::TransmissionModesLayers::TransmissionModesLayers(ns3::TransmissionModesLayers const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TransmissionModesLayers const &', 'arg0')])
    ## lte-common.h (module 'lte'): static uint8_t ns3::TransmissionModesLayers::TxMode2LayerNum(uint8_t txMode) [member function]
    cls.add_method('TxMode2LayerNum', 
                   'uint8_t', 
                   [param('uint8_t', 'txMode')], 
                   is_static=True)
    return

def register_Ns3TypeId_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## type-id.h (module 'core'): ns3::TypeId::TypeId(char const * name) [constructor]
    cls.add_constructor([param('char const *', 'name')])
    ## type-id.h (module 'core'): ns3::TypeId::TypeId() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeId::TypeId(ns3::TypeId const & o) [copy constructor]
    cls.add_constructor([param('ns3::TypeId const &', 'o')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::AddAttribute(std::string name, std::string help, ns3::AttributeValue const & initialValue, ns3::Ptr<ns3::AttributeAccessor const> accessor, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('AddAttribute', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::AttributeValue const &', 'initialValue'), param('ns3::Ptr< ns3::AttributeAccessor const >', 'accessor'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::AddAttribute(std::string name, std::string help, uint32_t flags, ns3::AttributeValue const & initialValue, ns3::Ptr<ns3::AttributeAccessor const> accessor, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('AddAttribute', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('uint32_t', 'flags'), param('ns3::AttributeValue const &', 'initialValue'), param('ns3::Ptr< ns3::AttributeAccessor const >', 'accessor'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::AddTraceSource(std::string name, std::string help, ns3::Ptr<ns3::TraceSourceAccessor const> accessor) [member function]
    cls.add_method('AddTraceSource', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::Ptr< ns3::TraceSourceAccessor const >', 'accessor')])
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation ns3::TypeId::GetAttribute(uint32_t i) const [member function]
    cls.add_method('GetAttribute', 
                   'ns3::TypeId::AttributeInformation', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetAttributeFullName(uint32_t i) const [member function]
    cls.add_method('GetAttributeFullName', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetAttributeN() const [member function]
    cls.add_method('GetAttributeN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): ns3::Callback<ns3::ObjectBase*,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ns3::TypeId::GetConstructor() const [member function]
    cls.add_method('GetConstructor', 
                   'ns3::Callback< ns3::ObjectBase *, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetGroupName() const [member function]
    cls.add_method('GetGroupName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::GetParent() const [member function]
    cls.add_method('GetParent', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): static ns3::TypeId ns3::TypeId::GetRegistered(uint32_t i) [member function]
    cls.add_method('GetRegistered', 
                   'ns3::TypeId', 
                   [param('uint32_t', 'i')], 
                   is_static=True)
    ## type-id.h (module 'core'): static uint32_t ns3::TypeId::GetRegisteredN() [member function]
    cls.add_method('GetRegisteredN', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation ns3::TypeId::GetTraceSource(uint32_t i) const [member function]
    cls.add_method('GetTraceSource', 
                   'ns3::TypeId::TraceSourceInformation', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetTraceSourceN() const [member function]
    cls.add_method('GetTraceSourceN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): uint16_t ns3::TypeId::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::HasConstructor() const [member function]
    cls.add_method('HasConstructor', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::HasParent() const [member function]
    cls.add_method('HasParent', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::HideFromDocumentation() [member function]
    cls.add_method('HideFromDocumentation', 
                   'ns3::TypeId', 
                   [])
    ## type-id.h (module 'core'): bool ns3::TypeId::IsChildOf(ns3::TypeId other) const [member function]
    cls.add_method('IsChildOf', 
                   'bool', 
                   [param('ns3::TypeId', 'other')], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::LookupAttributeByName(std::string name, ns3::TypeId::AttributeInformation * info) const [member function]
    cls.add_method('LookupAttributeByName', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::TypeId::AttributeInformation *', 'info', transfer_ownership=False)], 
                   is_const=True)
    ## type-id.h (module 'core'): static ns3::TypeId ns3::TypeId::LookupByName(std::string name) [member function]
    cls.add_method('LookupByName', 
                   'ns3::TypeId', 
                   [param('std::string', 'name')], 
                   is_static=True)
    ## type-id.h (module 'core'): ns3::Ptr<ns3::TraceSourceAccessor const> ns3::TypeId::LookupTraceSourceByName(std::string name) const [member function]
    cls.add_method('LookupTraceSourceByName', 
                   'ns3::Ptr< ns3::TraceSourceAccessor const >', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::MustHideFromDocumentation() const [member function]
    cls.add_method('MustHideFromDocumentation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::SetAttributeInitialValue(uint32_t i, ns3::Ptr<ns3::AttributeValue const> initialValue) [member function]
    cls.add_method('SetAttributeInitialValue', 
                   'bool', 
                   [param('uint32_t', 'i'), param('ns3::Ptr< ns3::AttributeValue const >', 'initialValue')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::SetGroupName(std::string groupName) [member function]
    cls.add_method('SetGroupName', 
                   'ns3::TypeId', 
                   [param('std::string', 'groupName')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::SetParent(ns3::TypeId tid) [member function]
    cls.add_method('SetParent', 
                   'ns3::TypeId', 
                   [param('ns3::TypeId', 'tid')])
    ## type-id.h (module 'core'): void ns3::TypeId::SetUid(uint16_t tid) [member function]
    cls.add_method('SetUid', 
                   'void', 
                   [param('uint16_t', 'tid')])
    return

def register_Ns3TypeIdAttributeInformation_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::AttributeInformation() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::AttributeInformation(ns3::TypeId::AttributeInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeId::AttributeInformation const &', 'arg0')])
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::accessor [variable]
    cls.add_instance_attribute('accessor', 'ns3::Ptr< ns3::AttributeAccessor const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::checker [variable]
    cls.add_instance_attribute('checker', 'ns3::Ptr< ns3::AttributeChecker const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::flags [variable]
    cls.add_instance_attribute('flags', 'uint32_t', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::help [variable]
    cls.add_instance_attribute('help', 'std::string', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::initialValue [variable]
    cls.add_instance_attribute('initialValue', 'ns3::Ptr< ns3::AttributeValue const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::name [variable]
    cls.add_instance_attribute('name', 'std::string', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::originalInitialValue [variable]
    cls.add_instance_attribute('originalInitialValue', 'ns3::Ptr< ns3::AttributeValue const >', is_const=False)
    return

def register_Ns3TypeIdTraceSourceInformation_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::TraceSourceInformation() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::TraceSourceInformation(ns3::TypeId::TraceSourceInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeId::TraceSourceInformation const &', 'arg0')])
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::accessor [variable]
    cls.add_instance_attribute('accessor', 'ns3::Ptr< ns3::TraceSourceAccessor const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::help [variable]
    cls.add_instance_attribute('help', 'std::string', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::name [variable]
    cls.add_instance_attribute('name', 'std::string', is_const=False)
    return

def register_Ns3Vector2D_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## vector.h (module 'core'): ns3::Vector2D::Vector2D(ns3::Vector2D const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector2D const &', 'arg0')])
    ## vector.h (module 'core'): ns3::Vector2D::Vector2D(double _x, double _y) [constructor]
    cls.add_constructor([param('double', '_x'), param('double', '_y')])
    ## vector.h (module 'core'): ns3::Vector2D::Vector2D() [constructor]
    cls.add_constructor([])
    ## vector.h (module 'core'): ns3::Vector2D::x [variable]
    cls.add_instance_attribute('x', 'double', is_const=False)
    ## vector.h (module 'core'): ns3::Vector2D::y [variable]
    cls.add_instance_attribute('y', 'double', is_const=False)
    return

def register_Ns3Vector3D_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## vector.h (module 'core'): ns3::Vector3D::Vector3D(ns3::Vector3D const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector3D const &', 'arg0')])
    ## vector.h (module 'core'): ns3::Vector3D::Vector3D(double _x, double _y, double _z) [constructor]
    cls.add_constructor([param('double', '_x'), param('double', '_y'), param('double', '_z')])
    ## vector.h (module 'core'): ns3::Vector3D::Vector3D() [constructor]
    cls.add_constructor([])
    ## vector.h (module 'core'): ns3::Vector3D::x [variable]
    cls.add_instance_attribute('x', 'double', is_const=False)
    ## vector.h (module 'core'): ns3::Vector3D::y [variable]
    cls.add_instance_attribute('y', 'double', is_const=False)
    ## vector.h (module 'core'): ns3::Vector3D::z [variable]
    cls.add_instance_attribute('z', 'double', is_const=False)
    return

def register_Ns3Empty_methods(root_module, cls):
    ## empty.h (module 'core'): ns3::empty::empty() [constructor]
    cls.add_constructor([])
    ## empty.h (module 'core'): ns3::empty::empty(ns3::empty const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::empty const &', 'arg0')])
    return

def register_Ns3Int64x64_t_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_inplace_numeric_operator('+=', param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short unsigned int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned char const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('signed char const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('double const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short unsigned int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned char const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('signed char const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('double const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short unsigned int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned char const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('signed char const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('double const', 'right'))
    cls.add_unary_numeric_operator('-')
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short unsigned int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned char const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('signed char const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('double const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    cls.add_inplace_numeric_operator('*=', param('ns3::int64x64_t const &', 'right'))
    cls.add_inplace_numeric_operator('-=', param('ns3::int64x64_t const &', 'right'))
    cls.add_inplace_numeric_operator('/=', param('ns3::int64x64_t const &', 'right'))
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>=')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t() [constructor]
    cls.add_constructor([])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(double v) [constructor]
    cls.add_constructor([param('double', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(int v) [constructor]
    cls.add_constructor([param('int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long int v) [constructor]
    cls.add_constructor([param('long int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long long int v) [constructor]
    cls.add_constructor([param('long long int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(unsigned int v) [constructor]
    cls.add_constructor([param('unsigned int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long unsigned int v) [constructor]
    cls.add_constructor([param('long unsigned int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long long unsigned int v) [constructor]
    cls.add_constructor([param('long long unsigned int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(int64_t hi, uint64_t lo) [constructor]
    cls.add_constructor([param('int64_t', 'hi'), param('uint64_t', 'lo')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(ns3::int64x64_t const & o) [copy constructor]
    cls.add_constructor([param('ns3::int64x64_t const &', 'o')])
    ## int64x64-double.h (module 'core'): double ns3::int64x64_t::GetDouble() const [member function]
    cls.add_method('GetDouble', 
                   'double', 
                   [], 
                   is_const=True)
    ## int64x64-double.h (module 'core'): int64_t ns3::int64x64_t::GetHigh() const [member function]
    cls.add_method('GetHigh', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## int64x64-double.h (module 'core'): uint64_t ns3::int64x64_t::GetLow() const [member function]
    cls.add_method('GetLow', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## int64x64-double.h (module 'core'): static ns3::int64x64_t ns3::int64x64_t::Invert(uint64_t v) [member function]
    cls.add_method('Invert', 
                   'ns3::int64x64_t', 
                   [param('uint64_t', 'v')], 
                   is_static=True)
    ## int64x64-double.h (module 'core'): void ns3::int64x64_t::MulByInvert(ns3::int64x64_t const & o) [member function]
    cls.add_method('MulByInvert', 
                   'void', 
                   [param('ns3::int64x64_t const &', 'o')])
    return

def register_Ns3PfsFlowPerf_t_methods(root_module, cls):
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::pfsFlowPerf_t::pfsFlowPerf_t() [constructor]
    cls.add_constructor([])
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::pfsFlowPerf_t::pfsFlowPerf_t(ns3::pfsFlowPerf_t const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::pfsFlowPerf_t const &', 'arg0')])
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::pfsFlowPerf_t::flowStart [variable]
    cls.add_instance_attribute('flowStart', 'ns3::Time', is_const=False)
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::pfsFlowPerf_t::lastAveragedThroughput [variable]
    cls.add_instance_attribute('lastAveragedThroughput', 'double', is_const=False)
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::pfsFlowPerf_t::lastTtiBytesTrasmitted [variable]
    cls.add_instance_attribute('lastTtiBytesTrasmitted', 'unsigned int', is_const=False)
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::pfsFlowPerf_t::totalBytesTransmitted [variable]
    cls.add_instance_attribute('totalBytesTransmitted', 'long unsigned int', is_const=False)
    return

def register_Ns3TbInfo_t_methods(root_module, cls):
    ## lte-spectrum-phy.h (module 'lte'): ns3::tbInfo_t::tbInfo_t() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-phy.h (module 'lte'): ns3::tbInfo_t::tbInfo_t(ns3::tbInfo_t const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::tbInfo_t const &', 'arg0')])
    ## lte-spectrum-phy.h (module 'lte'): ns3::tbInfo_t::corrupt [variable]
    cls.add_instance_attribute('corrupt', 'bool', is_const=False)
    ## lte-spectrum-phy.h (module 'lte'): ns3::tbInfo_t::mcs [variable]
    cls.add_instance_attribute('mcs', 'uint8_t', is_const=False)
    ## lte-spectrum-phy.h (module 'lte'): ns3::tbInfo_t::rbBitmap [variable]
    cls.add_instance_attribute('rbBitmap', 'std::vector< int >', is_const=False)
    ## lte-spectrum-phy.h (module 'lte'): ns3::tbInfo_t::size [variable]
    cls.add_instance_attribute('size', 'uint16_t', is_const=False)
    return

def register_Ns3Chunk_methods(root_module, cls):
    ## chunk.h (module 'network'): ns3::Chunk::Chunk() [constructor]
    cls.add_constructor([])
    ## chunk.h (module 'network'): ns3::Chunk::Chunk(ns3::Chunk const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Chunk const &', 'arg0')])
    ## chunk.h (module 'network'): uint32_t ns3::Chunk::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_virtual=True)
    ## chunk.h (module 'network'): static ns3::TypeId ns3::Chunk::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## chunk.h (module 'network'): void ns3::Chunk::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3Header_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## header.h (module 'network'): ns3::Header::Header() [constructor]
    cls.add_constructor([])
    ## header.h (module 'network'): ns3::Header::Header(ns3::Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Header const &', 'arg0')])
    ## header.h (module 'network'): uint32_t ns3::Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_virtual=True)
    ## header.h (module 'network'): uint32_t ns3::Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## header.h (module 'network'): static ns3::TypeId ns3::Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## header.h (module 'network'): void ns3::Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## header.h (module 'network'): void ns3::Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3Ipv4Header_methods(root_module, cls):
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Header::Ipv4Header(ns3::Ipv4Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Header const &', 'arg0')])
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Header::Ipv4Header() [constructor]
    cls.add_constructor([])
    ## ipv4-header.h (module 'internet'): uint32_t ns3::Ipv4Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv4-header.h (module 'internet'): std::string ns3::Ipv4Header::DscpTypeToString(ns3::Ipv4Header::DscpType dscp) const [member function]
    cls.add_method('DscpTypeToString', 
                   'std::string', 
                   [param('ns3::Ipv4Header::DscpType', 'dscp')], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): std::string ns3::Ipv4Header::EcnTypeToString(ns3::Ipv4Header::EcnType ecn) const [member function]
    cls.add_method('EcnTypeToString', 
                   'std::string', 
                   [param('ns3::Ipv4Header::EcnType', 'ecn')], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::EnableChecksum() [member function]
    cls.add_method('EnableChecksum', 
                   'void', 
                   [])
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4Header::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Header::DscpType ns3::Ipv4Header::GetDscp() const [member function]
    cls.add_method('GetDscp', 
                   'ns3::Ipv4Header::DscpType', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Header::EcnType ns3::Ipv4Header::GetEcn() const [member function]
    cls.add_method('GetEcn', 
                   'ns3::Ipv4Header::EcnType', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): uint16_t ns3::Ipv4Header::GetFragmentOffset() const [member function]
    cls.add_method('GetFragmentOffset', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): uint16_t ns3::Ipv4Header::GetIdentification() const [member function]
    cls.add_method('GetIdentification', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): ns3::TypeId ns3::Ipv4Header::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-header.h (module 'internet'): uint16_t ns3::Ipv4Header::GetPayloadSize() const [member function]
    cls.add_method('GetPayloadSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): uint8_t ns3::Ipv4Header::GetProtocol() const [member function]
    cls.add_method('GetProtocol', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): uint32_t ns3::Ipv4Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-header.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4Header::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): uint8_t ns3::Ipv4Header::GetTos() const [member function]
    cls.add_method('GetTos', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): uint8_t ns3::Ipv4Header::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): static ns3::TypeId ns3::Ipv4Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4-header.h (module 'internet'): bool ns3::Ipv4Header::IsChecksumOk() const [member function]
    cls.add_method('IsChecksumOk', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): bool ns3::Ipv4Header::IsDontFragment() const [member function]
    cls.add_method('IsDontFragment', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): bool ns3::Ipv4Header::IsLastFragment() const [member function]
    cls.add_method('IsLastFragment', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetDestination(ns3::Ipv4Address destination) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Ipv4Address', 'destination')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetDontFragment() [member function]
    cls.add_method('SetDontFragment', 
                   'void', 
                   [])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetDscp(ns3::Ipv4Header::DscpType dscp) [member function]
    cls.add_method('SetDscp', 
                   'void', 
                   [param('ns3::Ipv4Header::DscpType', 'dscp')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetEcn(ns3::Ipv4Header::EcnType ecn) [member function]
    cls.add_method('SetEcn', 
                   'void', 
                   [param('ns3::Ipv4Header::EcnType', 'ecn')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetFragmentOffset(uint16_t offsetBytes) [member function]
    cls.add_method('SetFragmentOffset', 
                   'void', 
                   [param('uint16_t', 'offsetBytes')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetIdentification(uint16_t identification) [member function]
    cls.add_method('SetIdentification', 
                   'void', 
                   [param('uint16_t', 'identification')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetLastFragment() [member function]
    cls.add_method('SetLastFragment', 
                   'void', 
                   [])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetMayFragment() [member function]
    cls.add_method('SetMayFragment', 
                   'void', 
                   [])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetMoreFragments() [member function]
    cls.add_method('SetMoreFragments', 
                   'void', 
                   [])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetPayloadSize(uint16_t size) [member function]
    cls.add_method('SetPayloadSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetProtocol(uint8_t num) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint8_t', 'num')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetSource(ns3::Ipv4Address source) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Ipv4Address', 'source')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetTos(uint8_t tos) [member function]
    cls.add_method('SetTos', 
                   'void', 
                   [param('uint8_t', 'tos')])
    ## ipv4-header.h (module 'internet'): void ns3::Ipv4Header::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3Ipv6Header_methods(root_module, cls):
    ## ipv6-header.h (module 'internet'): ns3::Ipv6Header::Ipv6Header(ns3::Ipv6Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Header const &', 'arg0')])
    ## ipv6-header.h (module 'internet'): ns3::Ipv6Header::Ipv6Header() [constructor]
    cls.add_constructor([])
    ## ipv6-header.h (module 'internet'): uint32_t ns3::Ipv6Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ipv6-header.h (module 'internet'): ns3::Ipv6Address ns3::Ipv6Header::GetDestinationAddress() const [member function]
    cls.add_method('GetDestinationAddress', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-header.h (module 'internet'): uint32_t ns3::Ipv6Header::GetFlowLabel() const [member function]
    cls.add_method('GetFlowLabel', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h (module 'internet'): uint8_t ns3::Ipv6Header::GetHopLimit() const [member function]
    cls.add_method('GetHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h (module 'internet'): ns3::TypeId ns3::Ipv6Header::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-header.h (module 'internet'): uint8_t ns3::Ipv6Header::GetNextHeader() const [member function]
    cls.add_method('GetNextHeader', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h (module 'internet'): uint16_t ns3::Ipv6Header::GetPayloadLength() const [member function]
    cls.add_method('GetPayloadLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h (module 'internet'): uint32_t ns3::Ipv6Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-header.h (module 'internet'): ns3::Ipv6Address ns3::Ipv6Header::GetSourceAddress() const [member function]
    cls.add_method('GetSourceAddress', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-header.h (module 'internet'): uint8_t ns3::Ipv6Header::GetTrafficClass() const [member function]
    cls.add_method('GetTrafficClass', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-header.h (module 'internet'): static ns3::TypeId ns3::Ipv6Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::SetDestinationAddress(ns3::Ipv6Address dst) [member function]
    cls.add_method('SetDestinationAddress', 
                   'void', 
                   [param('ns3::Ipv6Address', 'dst')])
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::SetFlowLabel(uint32_t flow) [member function]
    cls.add_method('SetFlowLabel', 
                   'void', 
                   [param('uint32_t', 'flow')])
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::SetHopLimit(uint8_t limit) [member function]
    cls.add_method('SetHopLimit', 
                   'void', 
                   [param('uint8_t', 'limit')])
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::SetNextHeader(uint8_t next) [member function]
    cls.add_method('SetNextHeader', 
                   'void', 
                   [param('uint8_t', 'next')])
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::SetPayloadLength(uint16_t len) [member function]
    cls.add_method('SetPayloadLength', 
                   'void', 
                   [param('uint16_t', 'len')])
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::SetSourceAddress(ns3::Ipv6Address src) [member function]
    cls.add_method('SetSourceAddress', 
                   'void', 
                   [param('ns3::Ipv6Address', 'src')])
    ## ipv6-header.h (module 'internet'): void ns3::Ipv6Header::SetTrafficClass(uint8_t traffic) [member function]
    cls.add_method('SetTrafficClass', 
                   'void', 
                   [param('uint8_t', 'traffic')])
    return

def register_Ns3LtePdcpHeader_methods(root_module, cls):
    ## lte-pdcp-header.h (module 'lte'): ns3::LtePdcpHeader::LtePdcpHeader(ns3::LtePdcpHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePdcpHeader const &', 'arg0')])
    ## lte-pdcp-header.h (module 'lte'): ns3::LtePdcpHeader::LtePdcpHeader() [constructor]
    cls.add_constructor([])
    ## lte-pdcp-header.h (module 'lte'): uint32_t ns3::LtePdcpHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## lte-pdcp-header.h (module 'lte'): uint8_t ns3::LtePdcpHeader::GetDcBit() const [member function]
    cls.add_method('GetDcBit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-pdcp-header.h (module 'lte'): ns3::TypeId ns3::LtePdcpHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-pdcp-header.h (module 'lte'): uint16_t ns3::LtePdcpHeader::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-pdcp-header.h (module 'lte'): uint32_t ns3::LtePdcpHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-pdcp-header.h (module 'lte'): static ns3::TypeId ns3::LtePdcpHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-pdcp-header.h (module 'lte'): void ns3::LtePdcpHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-pdcp-header.h (module 'lte'): void ns3::LtePdcpHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## lte-pdcp-header.h (module 'lte'): void ns3::LtePdcpHeader::SetDcBit(uint8_t dcBit) [member function]
    cls.add_method('SetDcBit', 
                   'void', 
                   [param('uint8_t', 'dcBit')])
    ## lte-pdcp-header.h (module 'lte'): void ns3::LtePdcpHeader::SetSequenceNumber(uint16_t sequenceNumber) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'sequenceNumber')])
    return

def register_Ns3LtePhyTag_methods(root_module, cls):
    ## lte-phy-tag.h (module 'lte'): ns3::LtePhyTag::LtePhyTag(ns3::LtePhyTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePhyTag const &', 'arg0')])
    ## lte-phy-tag.h (module 'lte'): ns3::LtePhyTag::LtePhyTag() [constructor]
    cls.add_constructor([])
    ## lte-phy-tag.h (module 'lte'): ns3::LtePhyTag::LtePhyTag(uint16_t cellId) [constructor]
    cls.add_constructor([param('uint16_t', 'cellId')])
    ## lte-phy-tag.h (module 'lte'): void ns3::LtePhyTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## lte-phy-tag.h (module 'lte'): uint16_t ns3::LtePhyTag::GetCellId() const [member function]
    cls.add_method('GetCellId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-phy-tag.h (module 'lte'): ns3::TypeId ns3::LtePhyTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-phy-tag.h (module 'lte'): uint32_t ns3::LtePhyTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-phy-tag.h (module 'lte'): static ns3::TypeId ns3::LtePhyTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-phy-tag.h (module 'lte'): void ns3::LtePhyTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-phy-tag.h (module 'lte'): void ns3::LtePhyTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3LteRadioBearerTag_methods(root_module, cls):
    ## lte-radio-bearer-tag.h (module 'lte'): ns3::LteRadioBearerTag::LteRadioBearerTag(ns3::LteRadioBearerTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRadioBearerTag const &', 'arg0')])
    ## lte-radio-bearer-tag.h (module 'lte'): ns3::LteRadioBearerTag::LteRadioBearerTag() [constructor]
    cls.add_constructor([])
    ## lte-radio-bearer-tag.h (module 'lte'): ns3::LteRadioBearerTag::LteRadioBearerTag(uint16_t rnti, uint8_t lcId) [constructor]
    cls.add_constructor([param('uint16_t', 'rnti'), param('uint8_t', 'lcId')])
    ## lte-radio-bearer-tag.h (module 'lte'): ns3::LteRadioBearerTag::LteRadioBearerTag(uint16_t rnti, uint8_t lcId, uint8_t layer) [constructor]
    cls.add_constructor([param('uint16_t', 'rnti'), param('uint8_t', 'lcId'), param('uint8_t', 'layer')])
    ## lte-radio-bearer-tag.h (module 'lte'): void ns3::LteRadioBearerTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## lte-radio-bearer-tag.h (module 'lte'): ns3::TypeId ns3::LteRadioBearerTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-radio-bearer-tag.h (module 'lte'): uint8_t ns3::LteRadioBearerTag::GetLayer() const [member function]
    cls.add_method('GetLayer', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-radio-bearer-tag.h (module 'lte'): uint8_t ns3::LteRadioBearerTag::GetLcid() const [member function]
    cls.add_method('GetLcid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-radio-bearer-tag.h (module 'lte'): uint16_t ns3::LteRadioBearerTag::GetRnti() const [member function]
    cls.add_method('GetRnti', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-radio-bearer-tag.h (module 'lte'): uint32_t ns3::LteRadioBearerTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-radio-bearer-tag.h (module 'lte'): static ns3::TypeId ns3::LteRadioBearerTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-radio-bearer-tag.h (module 'lte'): void ns3::LteRadioBearerTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-radio-bearer-tag.h (module 'lte'): void ns3::LteRadioBearerTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## lte-radio-bearer-tag.h (module 'lte'): void ns3::LteRadioBearerTag::SetLayer(uint8_t lcid) [member function]
    cls.add_method('SetLayer', 
                   'void', 
                   [param('uint8_t', 'lcid')])
    ## lte-radio-bearer-tag.h (module 'lte'): void ns3::LteRadioBearerTag::SetLcid(uint8_t lcid) [member function]
    cls.add_method('SetLcid', 
                   'void', 
                   [param('uint8_t', 'lcid')])
    ## lte-radio-bearer-tag.h (module 'lte'): void ns3::LteRadioBearerTag::SetRnti(uint16_t tid) [member function]
    cls.add_method('SetRnti', 
                   'void', 
                   [param('uint16_t', 'tid')])
    return

def register_Ns3LteRlcAmHeader_methods(root_module, cls):
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::LteRlcAmHeader(ns3::LteRlcAmHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcAmHeader const &', 'arg0')])
    ## lte-rlc-am-header.h (module 'lte'): ns3::LteRlcAmHeader::LteRlcAmHeader() [constructor]
    cls.add_constructor([])
    ## lte-rlc-am-header.h (module 'lte'): uint32_t ns3::LteRlcAmHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## lte-rlc-am-header.h (module 'lte'): ns3::SequenceNumber10 ns3::LteRlcAmHeader::GetAckSn() const [member function]
    cls.add_method('GetAckSn', 
                   'ns3::SequenceNumber10', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): uint8_t ns3::LteRlcAmHeader::GetFramingInfo() const [member function]
    cls.add_method('GetFramingInfo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): ns3::TypeId ns3::LteRlcAmHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-am-header.h (module 'lte'): uint16_t ns3::LteRlcAmHeader::GetLastOffset() const [member function]
    cls.add_method('GetLastOffset', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): uint8_t ns3::LteRlcAmHeader::GetLastSegmentFlag() const [member function]
    cls.add_method('GetLastSegmentFlag', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): uint8_t ns3::LteRlcAmHeader::GetPollingBit() const [member function]
    cls.add_method('GetPollingBit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): uint8_t ns3::LteRlcAmHeader::GetResegmentationFlag() const [member function]
    cls.add_method('GetResegmentationFlag', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): uint16_t ns3::LteRlcAmHeader::GetSegmentOffset() const [member function]
    cls.add_method('GetSegmentOffset', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): ns3::SequenceNumber10 ns3::LteRlcAmHeader::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'ns3::SequenceNumber10', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): uint32_t ns3::LteRlcAmHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-am-header.h (module 'lte'): static ns3::TypeId ns3::LteRlcAmHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-rlc-am-header.h (module 'lte'): bool ns3::LteRlcAmHeader::IsControlPdu() const [member function]
    cls.add_method('IsControlPdu', 
                   'bool', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): bool ns3::LteRlcAmHeader::IsDataPdu() const [member function]
    cls.add_method('IsDataPdu', 
                   'bool', 
                   [], 
                   is_const=True)
    ## lte-rlc-am-header.h (module 'lte'): uint8_t ns3::LteRlcAmHeader::PopExtensionBit() [member function]
    cls.add_method('PopExtensionBit', 
                   'uint8_t', 
                   [])
    ## lte-rlc-am-header.h (module 'lte'): uint16_t ns3::LteRlcAmHeader::PopLengthIndicator() [member function]
    cls.add_method('PopLengthIndicator', 
                   'uint16_t', 
                   [])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::PushExtensionBit(uint8_t extensionBit) [member function]
    cls.add_method('PushExtensionBit', 
                   'void', 
                   [param('uint8_t', 'extensionBit')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::PushLengthIndicator(uint16_t lengthIndicator) [member function]
    cls.add_method('PushLengthIndicator', 
                   'void', 
                   [param('uint16_t', 'lengthIndicator')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetAckSn(ns3::SequenceNumber10 ackSn) [member function]
    cls.add_method('SetAckSn', 
                   'void', 
                   [param('ns3::SequenceNumber10', 'ackSn')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetControlPdu(uint8_t controlPduType) [member function]
    cls.add_method('SetControlPdu', 
                   'void', 
                   [param('uint8_t', 'controlPduType')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetDataPdu() [member function]
    cls.add_method('SetDataPdu', 
                   'void', 
                   [])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetFramingInfo(uint8_t framingInfo) [member function]
    cls.add_method('SetFramingInfo', 
                   'void', 
                   [param('uint8_t', 'framingInfo')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetLastSegmentFlag(uint8_t lsf) [member function]
    cls.add_method('SetLastSegmentFlag', 
                   'void', 
                   [param('uint8_t', 'lsf')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetPollingBit(uint8_t pollingBit) [member function]
    cls.add_method('SetPollingBit', 
                   'void', 
                   [param('uint8_t', 'pollingBit')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetResegmentationFlag(uint8_t resegFlag) [member function]
    cls.add_method('SetResegmentationFlag', 
                   'void', 
                   [param('uint8_t', 'resegFlag')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetSegmentOffset(uint16_t segmentOffset) [member function]
    cls.add_method('SetSegmentOffset', 
                   'void', 
                   [param('uint16_t', 'segmentOffset')])
    ## lte-rlc-am-header.h (module 'lte'): void ns3::LteRlcAmHeader::SetSequenceNumber(ns3::SequenceNumber10 sequenceNumber) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('ns3::SequenceNumber10', 'sequenceNumber')])
    return

def register_Ns3LteRlcHeader_methods(root_module, cls):
    ## lte-rlc-header.h (module 'lte'): ns3::LteRlcHeader::LteRlcHeader(ns3::LteRlcHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcHeader const &', 'arg0')])
    ## lte-rlc-header.h (module 'lte'): ns3::LteRlcHeader::LteRlcHeader() [constructor]
    cls.add_constructor([])
    ## lte-rlc-header.h (module 'lte'): uint32_t ns3::LteRlcHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## lte-rlc-header.h (module 'lte'): uint8_t ns3::LteRlcHeader::GetFramingInfo() const [member function]
    cls.add_method('GetFramingInfo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-header.h (module 'lte'): ns3::TypeId ns3::LteRlcHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-header.h (module 'lte'): ns3::SequenceNumber10 ns3::LteRlcHeader::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'ns3::SequenceNumber10', 
                   [], 
                   is_const=True)
    ## lte-rlc-header.h (module 'lte'): uint32_t ns3::LteRlcHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-header.h (module 'lte'): static ns3::TypeId ns3::LteRlcHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-rlc-header.h (module 'lte'): uint8_t ns3::LteRlcHeader::PopExtensionBit() [member function]
    cls.add_method('PopExtensionBit', 
                   'uint8_t', 
                   [])
    ## lte-rlc-header.h (module 'lte'): uint16_t ns3::LteRlcHeader::PopLengthIndicator() [member function]
    cls.add_method('PopLengthIndicator', 
                   'uint16_t', 
                   [])
    ## lte-rlc-header.h (module 'lte'): void ns3::LteRlcHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-header.h (module 'lte'): void ns3::LteRlcHeader::PushExtensionBit(uint8_t extensionBit) [member function]
    cls.add_method('PushExtensionBit', 
                   'void', 
                   [param('uint8_t', 'extensionBit')])
    ## lte-rlc-header.h (module 'lte'): void ns3::LteRlcHeader::PushLengthIndicator(uint16_t lengthIndicator) [member function]
    cls.add_method('PushLengthIndicator', 
                   'void', 
                   [param('uint16_t', 'lengthIndicator')])
    ## lte-rlc-header.h (module 'lte'): void ns3::LteRlcHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-header.h (module 'lte'): void ns3::LteRlcHeader::SetFramingInfo(uint8_t framingInfo) [member function]
    cls.add_method('SetFramingInfo', 
                   'void', 
                   [param('uint8_t', 'framingInfo')])
    ## lte-rlc-header.h (module 'lte'): void ns3::LteRlcHeader::SetSequenceNumber(ns3::SequenceNumber10 sequenceNumber) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('ns3::SequenceNumber10', 'sequenceNumber')])
    return

def register_Ns3LteRlcSduStatusTag_methods(root_module, cls):
    ## lte-rlc-sdu-status-tag.h (module 'lte'): ns3::LteRlcSduStatusTag::LteRlcSduStatusTag(ns3::LteRlcSduStatusTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcSduStatusTag const &', 'arg0')])
    ## lte-rlc-sdu-status-tag.h (module 'lte'): ns3::LteRlcSduStatusTag::LteRlcSduStatusTag() [constructor]
    cls.add_constructor([])
    ## lte-rlc-sdu-status-tag.h (module 'lte'): void ns3::LteRlcSduStatusTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## lte-rlc-sdu-status-tag.h (module 'lte'): ns3::TypeId ns3::LteRlcSduStatusTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-sdu-status-tag.h (module 'lte'): uint32_t ns3::LteRlcSduStatusTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-sdu-status-tag.h (module 'lte'): uint8_t ns3::LteRlcSduStatusTag::GetStatus() const [member function]
    cls.add_method('GetStatus', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-rlc-sdu-status-tag.h (module 'lte'): static ns3::TypeId ns3::LteRlcSduStatusTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-rlc-sdu-status-tag.h (module 'lte'): void ns3::LteRlcSduStatusTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-sdu-status-tag.h (module 'lte'): void ns3::LteRlcSduStatusTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-sdu-status-tag.h (module 'lte'): void ns3::LteRlcSduStatusTag::SetStatus(uint8_t status) [member function]
    cls.add_method('SetStatus', 
                   'void', 
                   [param('uint8_t', 'status')])
    return

def register_Ns3Object_methods(root_module, cls):
    ## object.h (module 'core'): ns3::Object::Object() [constructor]
    cls.add_constructor([])
    ## object.h (module 'core'): void ns3::Object::AggregateObject(ns3::Ptr<ns3::Object> other) [member function]
    cls.add_method('AggregateObject', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'other')])
    ## object.h (module 'core'): void ns3::Object::Dispose() [member function]
    cls.add_method('Dispose', 
                   'void', 
                   [])
    ## object.h (module 'core'): ns3::Object::AggregateIterator ns3::Object::GetAggregateIterator() const [member function]
    cls.add_method('GetAggregateIterator', 
                   'ns3::Object::AggregateIterator', 
                   [], 
                   is_const=True)
    ## object.h (module 'core'): ns3::TypeId ns3::Object::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object.h (module 'core'): static ns3::TypeId ns3::Object::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## object.h (module 'core'): void ns3::Object::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [])
    ## object.h (module 'core'): ns3::Object::Object(ns3::Object const & o) [copy constructor]
    cls.add_constructor([param('ns3::Object const &', 'o')], 
                        visibility='protected')
    ## object.h (module 'core'): void ns3::Object::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## object.h (module 'core'): void ns3::Object::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## object.h (module 'core'): void ns3::Object::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3ObjectAggregateIterator_methods(root_module, cls):
    ## object.h (module 'core'): ns3::Object::AggregateIterator::AggregateIterator(ns3::Object::AggregateIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Object::AggregateIterator const &', 'arg0')])
    ## object.h (module 'core'): ns3::Object::AggregateIterator::AggregateIterator() [constructor]
    cls.add_constructor([])
    ## object.h (module 'core'): bool ns3::Object::AggregateIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## object.h (module 'core'): ns3::Ptr<ns3::Object const> ns3::Object::AggregateIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::Ptr< ns3::Object const >', 
                   [])
    return

def register_Ns3PacketBurst_methods(root_module, cls):
    ## packet-burst.h (module 'network'): ns3::PacketBurst::PacketBurst(ns3::PacketBurst const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketBurst const &', 'arg0')])
    ## packet-burst.h (module 'network'): ns3::PacketBurst::PacketBurst() [constructor]
    cls.add_constructor([])
    ## packet-burst.h (module 'network'): void ns3::PacketBurst::AddPacket(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('AddPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    ## packet-burst.h (module 'network'): std::_List_const_iterator<ns3::Ptr<ns3::Packet> > ns3::PacketBurst::Begin() const [member function]
    cls.add_method('Begin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::Packet > >', 
                   [], 
                   is_const=True)
    ## packet-burst.h (module 'network'): ns3::Ptr<ns3::PacketBurst> ns3::PacketBurst::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [], 
                   is_const=True)
    ## packet-burst.h (module 'network'): std::_List_const_iterator<ns3::Ptr<ns3::Packet> > ns3::PacketBurst::End() const [member function]
    cls.add_method('End', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::Packet > >', 
                   [], 
                   is_const=True)
    ## packet-burst.h (module 'network'): uint32_t ns3::PacketBurst::GetNPackets() const [member function]
    cls.add_method('GetNPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-burst.h (module 'network'): std::list<ns3::Ptr<ns3::Packet>, std::allocator<ns3::Ptr<ns3::Packet> > > ns3::PacketBurst::GetPackets() const [member function]
    cls.add_method('GetPackets', 
                   'std::list< ns3::Ptr< ns3::Packet > >', 
                   [], 
                   is_const=True)
    ## packet-burst.h (module 'network'): uint32_t ns3::PacketBurst::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-burst.h (module 'network'): static ns3::TypeId ns3::PacketBurst::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## packet-burst.h (module 'network'): void ns3::PacketBurst::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PdcpTag_methods(root_module, cls):
    ## lte-pdcp-tag.h (module 'lte'): ns3::PdcpTag::PdcpTag(ns3::PdcpTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PdcpTag const &', 'arg0')])
    ## lte-pdcp-tag.h (module 'lte'): ns3::PdcpTag::PdcpTag() [constructor]
    cls.add_constructor([])
    ## lte-pdcp-tag.h (module 'lte'): ns3::PdcpTag::PdcpTag(ns3::Time senderTimestamp) [constructor]
    cls.add_constructor([param('ns3::Time', 'senderTimestamp')])
    ## lte-pdcp-tag.h (module 'lte'): void ns3::PdcpTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## lte-pdcp-tag.h (module 'lte'): ns3::TypeId ns3::PdcpTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-pdcp-tag.h (module 'lte'): ns3::Time ns3::PdcpTag::GetSenderTimestamp() const [member function]
    cls.add_method('GetSenderTimestamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## lte-pdcp-tag.h (module 'lte'): uint32_t ns3::PdcpTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-pdcp-tag.h (module 'lte'): static ns3::TypeId ns3::PdcpTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-pdcp-tag.h (module 'lte'): void ns3::PdcpTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-pdcp-tag.h (module 'lte'): void ns3::PdcpTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## lte-pdcp-tag.h (module 'lte'): void ns3::PdcpTag::SetSenderTimestamp(ns3::Time senderTimestamp) [member function]
    cls.add_method('SetSenderTimestamp', 
                   'void', 
                   [param('ns3::Time', 'senderTimestamp')])
    return

def register_Ns3RadioEnvironmentMapHelper_methods(root_module, cls):
    ## radio-environment-map-helper.h (module 'lte'): ns3::RadioEnvironmentMapHelper::RadioEnvironmentMapHelper() [constructor]
    cls.add_constructor([])
    ## radio-environment-map-helper.h (module 'lte'): void ns3::RadioEnvironmentMapHelper::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## radio-environment-map-helper.h (module 'lte'): uint8_t ns3::RadioEnvironmentMapHelper::GetBandwidth() const [member function]
    cls.add_method('GetBandwidth', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radio-environment-map-helper.h (module 'lte'): static ns3::TypeId ns3::RadioEnvironmentMapHelper::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radio-environment-map-helper.h (module 'lte'): void ns3::RadioEnvironmentMapHelper::Install() [member function]
    cls.add_method('Install', 
                   'void', 
                   [])
    ## radio-environment-map-helper.h (module 'lte'): void ns3::RadioEnvironmentMapHelper::SetBandwidth(uint8_t bw) [member function]
    cls.add_method('SetBandwidth', 
                   'void', 
                   [param('uint8_t', 'bw')])
    return

def register_Ns3RandomVariableStream_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::RandomVariableStream::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::RandomVariableStream::RandomVariableStream() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): void ns3::RandomVariableStream::SetStream(int64_t stream) [member function]
    cls.add_method('SetStream', 
                   'void', 
                   [param('int64_t', 'stream')])
    ## random-variable-stream.h (module 'core'): int64_t ns3::RandomVariableStream::GetStream() const [member function]
    cls.add_method('GetStream', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): void ns3::RandomVariableStream::SetAntithetic(bool isAntithetic) [member function]
    cls.add_method('SetAntithetic', 
                   'void', 
                   [param('bool', 'isAntithetic')])
    ## random-variable-stream.h (module 'core'): bool ns3::RandomVariableStream::IsAntithetic() const [member function]
    cls.add_method('IsAntithetic', 
                   'bool', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::RandomVariableStream::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::RandomVariableStream::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## random-variable-stream.h (module 'core'): ns3::RngStream * ns3::RandomVariableStream::Peek() const [member function]
    cls.add_method('Peek', 
                   'ns3::RngStream *', 
                   [], 
                   is_const=True, visibility='protected')
    return

def register_Ns3RlcTag_methods(root_module, cls):
    ## lte-rlc-tag.h (module 'lte'): ns3::RlcTag::RlcTag(ns3::RlcTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RlcTag const &', 'arg0')])
    ## lte-rlc-tag.h (module 'lte'): ns3::RlcTag::RlcTag() [constructor]
    cls.add_constructor([])
    ## lte-rlc-tag.h (module 'lte'): ns3::RlcTag::RlcTag(ns3::Time senderTimestamp) [constructor]
    cls.add_constructor([param('ns3::Time', 'senderTimestamp')])
    ## lte-rlc-tag.h (module 'lte'): void ns3::RlcTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## lte-rlc-tag.h (module 'lte'): ns3::TypeId ns3::RlcTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-tag.h (module 'lte'): ns3::Time ns3::RlcTag::GetSenderTimestamp() const [member function]
    cls.add_method('GetSenderTimestamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## lte-rlc-tag.h (module 'lte'): uint32_t ns3::RlcTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-tag.h (module 'lte'): static ns3::TypeId ns3::RlcTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-rlc-tag.h (module 'lte'): void ns3::RlcTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-tag.h (module 'lte'): void ns3::RlcTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## lte-rlc-tag.h (module 'lte'): void ns3::RlcTag::SetSenderTimestamp(ns3::Time senderTimestamp) [member function]
    cls.add_method('SetSenderTimestamp', 
                   'void', 
                   [param('ns3::Time', 'senderTimestamp')])
    return

def register_Ns3SequentialRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::SequentialRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::SequentialRandomVariable::SequentialRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::SequentialRandomVariable::GetMin() const [member function]
    cls.add_method('GetMin', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::SequentialRandomVariable::GetMax() const [member function]
    cls.add_method('GetMax', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): ns3::Ptr<ns3::RandomVariableStream> ns3::SequentialRandomVariable::GetIncrement() const [member function]
    cls.add_method('GetIncrement', 
                   'ns3::Ptr< ns3::RandomVariableStream >', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::SequentialRandomVariable::GetConsecutive() const [member function]
    cls.add_method('GetConsecutive', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::SequentialRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::SequentialRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter< ns3::AttributeAccessor > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter< ns3::AttributeChecker > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter< ns3::AttributeValue > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::SimpleRefCount(ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter< ns3::CallbackImplBase > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3EpcTft_Ns3Empty_Ns3DefaultDeleter__lt__ns3EpcTft__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EpcTft, ns3::empty, ns3::DefaultDeleter<ns3::EpcTft> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EpcTft, ns3::empty, ns3::DefaultDeleter<ns3::EpcTft> >::SimpleRefCount(ns3::SimpleRefCount<ns3::EpcTft, ns3::empty, ns3::DefaultDeleter<ns3::EpcTft> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::EpcTft, ns3::empty, ns3::DefaultDeleter< ns3::EpcTft > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::EpcTft, ns3::empty, ns3::DefaultDeleter<ns3::EpcTft> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3EpcTftClassifier_Ns3Empty_Ns3DefaultDeleter__lt__ns3EpcTftClassifier__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EpcTftClassifier, ns3::empty, ns3::DefaultDeleter<ns3::EpcTftClassifier> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EpcTftClassifier, ns3::empty, ns3::DefaultDeleter<ns3::EpcTftClassifier> >::SimpleRefCount(ns3::SimpleRefCount<ns3::EpcTftClassifier, ns3::empty, ns3::DefaultDeleter<ns3::EpcTftClassifier> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::EpcTftClassifier, ns3::empty, ns3::DefaultDeleter< ns3::EpcTftClassifier > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::EpcTftClassifier, ns3::empty, ns3::DefaultDeleter<ns3::EpcTftClassifier> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::SimpleRefCount(ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter< ns3::EventImpl > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3IdealControlMessage_Ns3Empty_Ns3DefaultDeleter__lt__ns3IdealControlMessage__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >::SimpleRefCount(ns3::SimpleRefCount<ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter< ns3::IdealControlMessage > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Ipv4MulticastRoute_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv4MulticastRoute__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter< ns3::Ipv4MulticastRoute > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Ipv4Route_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv4Route__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter< ns3::Ipv4Route > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3LteSinrChunkProcessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3LteSinrChunkProcessor__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::LteSinrChunkProcessor, ns3::empty, ns3::DefaultDeleter<ns3::LteSinrChunkProcessor> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::LteSinrChunkProcessor, ns3::empty, ns3::DefaultDeleter<ns3::LteSinrChunkProcessor> >::SimpleRefCount(ns3::SimpleRefCount<ns3::LteSinrChunkProcessor, ns3::empty, ns3::DefaultDeleter<ns3::LteSinrChunkProcessor> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::LteSinrChunkProcessor, ns3::empty, ns3::DefaultDeleter< ns3::LteSinrChunkProcessor > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::LteSinrChunkProcessor, ns3::empty, ns3::DefaultDeleter<ns3::LteSinrChunkProcessor> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::SimpleRefCount(ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter< ns3::NixVector > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter< ns3::Packet > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3SpectrumModel_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumModel__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >::SimpleRefCount(ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter< ns3::SpectrumModel > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3SpectrumSignalParameters_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumSignalParameters__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumSignalParameters, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumSignalParameters> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumSignalParameters, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumSignalParameters> >::SimpleRefCount(ns3::SimpleRefCount<ns3::SpectrumSignalParameters, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumSignalParameters> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::SpectrumSignalParameters, ns3::empty, ns3::DefaultDeleter< ns3::SpectrumSignalParameters > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::SpectrumSignalParameters, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumSignalParameters> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3SpectrumValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumValue__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >::SimpleRefCount(ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter< ns3::SpectrumValue > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3TraceSourceAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3TraceSourceAccessor__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::SimpleRefCount(ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter< ns3::TraceSourceAccessor > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3Socket_methods(root_module, cls):
    ## socket.h (module 'network'): ns3::Socket::Socket(ns3::Socket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Socket const &', 'arg0')])
    ## socket.h (module 'network'): ns3::Socket::Socket() [constructor]
    cls.add_constructor([])
    ## socket.h (module 'network'): int ns3::Socket::Bind(ns3::Address const & address) [member function]
    cls.add_method('Bind', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Bind() [member function]
    cls.add_method('Bind', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Bind6() [member function]
    cls.add_method('Bind6', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::Socket::BindToNetDevice(ns3::Ptr<ns3::NetDevice> netdevice) [member function]
    cls.add_method('BindToNetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'netdevice')], 
                   is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Close() [member function]
    cls.add_method('Close', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Connect(ns3::Address const & address) [member function]
    cls.add_method('Connect', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): static ns3::Ptr<ns3::Socket> ns3::Socket::CreateSocket(ns3::Ptr<ns3::Node> node, ns3::TypeId tid) [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::TypeId', 'tid')], 
                   is_static=True)
    ## socket.h (module 'network'): bool ns3::Socket::GetAllowBroadcast() const [member function]
    cls.add_method('GetAllowBroadcast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::NetDevice> ns3::Socket::GetBoundNetDevice() [member function]
    cls.add_method('GetBoundNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## socket.h (module 'network'): ns3::Socket::SocketErrno ns3::Socket::GetErrno() const [member function]
    cls.add_method('GetErrno', 
                   'ns3::Socket::SocketErrno', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::Node> ns3::Socket::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::Socket::GetRxAvailable() const [member function]
    cls.add_method('GetRxAvailable', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::GetSockName(ns3::Address & address) const [member function]
    cls.add_method('GetSockName', 
                   'int', 
                   [param('ns3::Address &', 'address')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Socket::SocketType ns3::Socket::GetSocketType() const [member function]
    cls.add_method('GetSocketType', 
                   'ns3::Socket::SocketType', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::Socket::GetTxAvailable() const [member function]
    cls.add_method('GetTxAvailable', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): static ns3::TypeId ns3::Socket::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h (module 'network'): bool ns3::Socket::IsRecvPktInfo() const [member function]
    cls.add_method('IsRecvPktInfo', 
                   'bool', 
                   [], 
                   is_const=True)
    ## socket.h (module 'network'): int ns3::Socket::Listen() [member function]
    cls.add_method('Listen', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Socket::Recv(uint32_t maxSize, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Socket::Recv() [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## socket.h (module 'network'): int ns3::Socket::Recv(uint8_t * buf, uint32_t size, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'int', 
                   [param('uint8_t *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags')])
    ## socket.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Socket::RecvFrom(uint32_t maxSize, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Socket::RecvFrom(ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Address &', 'fromAddress')])
    ## socket.h (module 'network'): int ns3::Socket::RecvFrom(uint8_t * buf, uint32_t size, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'int', 
                   [param('uint8_t *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')])
    ## socket.h (module 'network'): int ns3::Socket::Send(ns3::Ptr<ns3::Packet> p, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## socket.h (module 'network'): int ns3::Socket::Send(uint8_t const * buf, uint32_t size, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('uint8_t const *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags')])
    ## socket.h (module 'network'): int ns3::Socket::SendTo(ns3::Ptr<ns3::Packet> p, uint32_t flags, ns3::Address const & toAddress) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags'), param('ns3::Address const &', 'toAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::SendTo(uint8_t const * buf, uint32_t size, uint32_t flags, ns3::Address const & address) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('uint8_t const *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags'), param('ns3::Address const &', 'address')])
    ## socket.h (module 'network'): void ns3::Socket::SetAcceptCallback(ns3::Callback<bool, ns3::Ptr<ns3::Socket>, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionRequest, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> newConnectionCreated) [member function]
    cls.add_method('SetAcceptCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::Socket >, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionRequest'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'newConnectionCreated')])
    ## socket.h (module 'network'): bool ns3::Socket::SetAllowBroadcast(bool allowBroadcast) [member function]
    cls.add_method('SetAllowBroadcast', 
                   'bool', 
                   [param('bool', 'allowBroadcast')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::Socket::SetCloseCallbacks(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> normalClose, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> errorClose) [member function]
    cls.add_method('SetCloseCallbacks', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'normalClose'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'errorClose')])
    ## socket.h (module 'network'): void ns3::Socket::SetConnectCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionSucceeded, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionFailed) [member function]
    cls.add_method('SetConnectCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionSucceeded'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionFailed')])
    ## socket.h (module 'network'): void ns3::Socket::SetDataSentCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> dataSent) [member function]
    cls.add_method('SetDataSentCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'dataSent')])
    ## socket.h (module 'network'): void ns3::Socket::SetRecvCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> arg0) [member function]
    cls.add_method('SetRecvCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'arg0')])
    ## socket.h (module 'network'): void ns3::Socket::SetRecvPktInfo(bool flag) [member function]
    cls.add_method('SetRecvPktInfo', 
                   'void', 
                   [param('bool', 'flag')])
    ## socket.h (module 'network'): void ns3::Socket::SetSendCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> sendCb) [member function]
    cls.add_method('SetSendCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'sendCb')])
    ## socket.h (module 'network'): int ns3::Socket::ShutdownRecv() [member function]
    cls.add_method('ShutdownRecv', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::ShutdownSend() [member function]
    cls.add_method('ShutdownSend', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::Socket::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## socket.h (module 'network'): void ns3::Socket::NotifyConnectionFailed() [member function]
    cls.add_method('NotifyConnectionFailed', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): bool ns3::Socket::NotifyConnectionRequest(ns3::Address const & from) [member function]
    cls.add_method('NotifyConnectionRequest', 
                   'bool', 
                   [param('ns3::Address const &', 'from')], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyConnectionSucceeded() [member function]
    cls.add_method('NotifyConnectionSucceeded', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyDataRecv() [member function]
    cls.add_method('NotifyDataRecv', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyDataSent(uint32_t size) [member function]
    cls.add_method('NotifyDataSent', 
                   'void', 
                   [param('uint32_t', 'size')], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyErrorClose() [member function]
    cls.add_method('NotifyErrorClose', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyNewConnectionCreated(ns3::Ptr<ns3::Socket> socket, ns3::Address const & from) [member function]
    cls.add_method('NotifyNewConnectionCreated', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket'), param('ns3::Address const &', 'from')], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyNormalClose() [member function]
    cls.add_method('NotifyNormalClose', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifySend(uint32_t spaceAvailable) [member function]
    cls.add_method('NotifySend', 
                   'void', 
                   [param('uint32_t', 'spaceAvailable')], 
                   visibility='protected')
    return

def register_Ns3SocketAddressTag_methods(root_module, cls):
    ## socket.h (module 'network'): ns3::SocketAddressTag::SocketAddressTag(ns3::SocketAddressTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketAddressTag const &', 'arg0')])
    ## socket.h (module 'network'): ns3::SocketAddressTag::SocketAddressTag() [constructor]
    cls.add_constructor([])
    ## socket.h (module 'network'): void ns3::SocketAddressTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h (module 'network'): ns3::Address ns3::SocketAddressTag::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## socket.h (module 'network'): ns3::TypeId ns3::SocketAddressTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::SocketAddressTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): static ns3::TypeId ns3::SocketAddressTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h (module 'network'): void ns3::SocketAddressTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketAddressTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketAddressTag::SetAddress(ns3::Address addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'addr')])
    return

def register_Ns3SocketIpTtlTag_methods(root_module, cls):
    ## socket.h (module 'network'): ns3::SocketIpTtlTag::SocketIpTtlTag(ns3::SocketIpTtlTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketIpTtlTag const &', 'arg0')])
    ## socket.h (module 'network'): ns3::SocketIpTtlTag::SocketIpTtlTag() [constructor]
    cls.add_constructor([])
    ## socket.h (module 'network'): void ns3::SocketIpTtlTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h (module 'network'): ns3::TypeId ns3::SocketIpTtlTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::SocketIpTtlTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint8_t ns3::SocketIpTtlTag::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## socket.h (module 'network'): static ns3::TypeId ns3::SocketIpTtlTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h (module 'network'): void ns3::SocketIpTtlTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketIpTtlTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketIpTtlTag::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3SocketSetDontFragmentTag_methods(root_module, cls):
    ## socket.h (module 'network'): ns3::SocketSetDontFragmentTag::SocketSetDontFragmentTag(ns3::SocketSetDontFragmentTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketSetDontFragmentTag const &', 'arg0')])
    ## socket.h (module 'network'): ns3::SocketSetDontFragmentTag::SocketSetDontFragmentTag() [constructor]
    cls.add_constructor([])
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Disable() [member function]
    cls.add_method('Disable', 
                   'void', 
                   [])
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [])
    ## socket.h (module 'network'): ns3::TypeId ns3::SocketSetDontFragmentTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::SocketSetDontFragmentTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): static ns3::TypeId ns3::SocketSetDontFragmentTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h (module 'network'): bool ns3::SocketSetDontFragmentTag::IsEnabled() const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3SpectrumInterference_methods(root_module, cls):
    ## spectrum-interference.h (module 'spectrum'): ns3::SpectrumInterference::SpectrumInterference(ns3::SpectrumInterference const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumInterference const &', 'arg0')])
    ## spectrum-interference.h (module 'spectrum'): ns3::SpectrumInterference::SpectrumInterference() [constructor]
    cls.add_constructor([])
    ## spectrum-interference.h (module 'spectrum'): void ns3::SpectrumInterference::AbortRx() [member function]
    cls.add_method('AbortRx', 
                   'void', 
                   [])
    ## spectrum-interference.h (module 'spectrum'): void ns3::SpectrumInterference::AddSignal(ns3::Ptr<ns3::SpectrumValue const> spd, ns3::Time const duration) [member function]
    cls.add_method('AddSignal', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'spd'), param('ns3::Time const', 'duration')])
    ## spectrum-interference.h (module 'spectrum'): bool ns3::SpectrumInterference::EndRx() [member function]
    cls.add_method('EndRx', 
                   'bool', 
                   [])
    ## spectrum-interference.h (module 'spectrum'): void ns3::SpectrumInterference::SetErrorModel(ns3::Ptr<ns3::SpectrumErrorModel> e) [member function]
    cls.add_method('SetErrorModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumErrorModel >', 'e')])
    ## spectrum-interference.h (module 'spectrum'): void ns3::SpectrumInterference::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
    ## spectrum-interference.h (module 'spectrum'): void ns3::SpectrumInterference::StartRx(ns3::Ptr<const ns3::Packet> p, ns3::Ptr<ns3::SpectrumValue const> rxPsd) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd')])
    ## spectrum-interference.h (module 'spectrum'): void ns3::SpectrumInterference::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3SpectrumModel_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## spectrum-model.h (module 'spectrum'): ns3::SpectrumModel::SpectrumModel(ns3::SpectrumModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumModel const &', 'arg0')])
    ## spectrum-model.h (module 'spectrum'): ns3::SpectrumModel::SpectrumModel(std::vector<double, std::allocator<double> > centerFreqs) [constructor]
    cls.add_constructor([param('std::vector< double >', 'centerFreqs')])
    ## spectrum-model.h (module 'spectrum'): ns3::SpectrumModel::SpectrumModel(ns3::Bands bands) [constructor]
    cls.add_constructor([param('ns3::Bands', 'bands')])
    ## spectrum-model.h (module 'spectrum'): __gnu_cxx::__normal_iterator<const ns3::BandInfo*,std::vector<ns3::BandInfo, std::allocator<ns3::BandInfo> > > ns3::SpectrumModel::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::BandInfo const *, std::vector< ns3::BandInfo > >', 
                   [], 
                   is_const=True)
    ## spectrum-model.h (module 'spectrum'): __gnu_cxx::__normal_iterator<const ns3::BandInfo*,std::vector<ns3::BandInfo, std::allocator<ns3::BandInfo> > > ns3::SpectrumModel::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::BandInfo const *, std::vector< ns3::BandInfo > >', 
                   [], 
                   is_const=True)
    ## spectrum-model.h (module 'spectrum'): size_t ns3::SpectrumModel::GetNumBands() const [member function]
    cls.add_method('GetNumBands', 
                   'size_t', 
                   [], 
                   is_const=True)
    ## spectrum-model.h (module 'spectrum'): ns3::SpectrumModelUid_t ns3::SpectrumModel::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'ns3::SpectrumModelUid_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3SpectrumPhy_methods(root_module, cls):
    ## spectrum-phy.h (module 'spectrum'): ns3::SpectrumPhy::SpectrumPhy() [constructor]
    cls.add_constructor([])
    ## spectrum-phy.h (module 'spectrum'): static ns3::TypeId ns3::SpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-phy.h (module 'spectrum'): void ns3::SpectrumPhy::SetDevice(ns3::Ptr<ns3::NetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'd')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): ns3::Ptr<ns3::NetDevice> ns3::SpectrumPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): void ns3::SpectrumPhy::SetMobility(ns3::Ptr<ns3::MobilityModel> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'm')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): ns3::Ptr<ns3::MobilityModel> ns3::SpectrumPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::MobilityModel >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): void ns3::SpectrumPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): ns3::Ptr<ns3::SpectrumModel const> ns3::SpectrumPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): ns3::Ptr<ns3::AntennaModel> ns3::SpectrumPhy::GetRxAntenna() [member function]
    cls.add_method('GetRxAntenna', 
                   'ns3::Ptr< ns3::AntennaModel >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): void ns3::SpectrumPhy::StartRx(ns3::Ptr<ns3::SpectrumSignalParameters> params) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumSignalParameters >', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3SpectrumPropagationLossModel_methods(root_module, cls):
    ## spectrum-propagation-loss-model.h (module 'spectrum'): ns3::SpectrumPropagationLossModel::SpectrumPropagationLossModel(ns3::SpectrumPropagationLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumPropagationLossModel const &', 'arg0')])
    ## spectrum-propagation-loss-model.h (module 'spectrum'): ns3::SpectrumPropagationLossModel::SpectrumPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## spectrum-propagation-loss-model.h (module 'spectrum'): ns3::Ptr<ns3::SpectrumValue> ns3::SpectrumPropagationLossModel::CalcRxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> txPsd, ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('CalcRxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'txPsd'), param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_const=True)
    ## spectrum-propagation-loss-model.h (module 'spectrum'): static ns3::TypeId ns3::SpectrumPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-propagation-loss-model.h (module 'spectrum'): void ns3::SpectrumPropagationLossModel::SetNext(ns3::Ptr<ns3::SpectrumPropagationLossModel> next) [member function]
    cls.add_method('SetNext', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPropagationLossModel >', 'next')])
    ## spectrum-propagation-loss-model.h (module 'spectrum'): void ns3::SpectrumPropagationLossModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## spectrum-propagation-loss-model.h (module 'spectrum'): ns3::Ptr<ns3::SpectrumValue> ns3::SpectrumPropagationLossModel::DoCalcRxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> txPsd, ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'txPsd'), param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3SpectrumSignalParameters_methods(root_module, cls):
    ## spectrum-signal-parameters.h (module 'spectrum'): ns3::SpectrumSignalParameters::SpectrumSignalParameters() [constructor]
    cls.add_constructor([])
    ## spectrum-signal-parameters.h (module 'spectrum'): ns3::SpectrumSignalParameters::SpectrumSignalParameters(ns3::SpectrumSignalParameters const & p) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumSignalParameters const &', 'p')])
    ## spectrum-signal-parameters.h (module 'spectrum'): ns3::Ptr<ns3::SpectrumSignalParameters> ns3::SpectrumSignalParameters::Copy() [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::SpectrumSignalParameters >', 
                   [], 
                   is_virtual=True)
    ## spectrum-signal-parameters.h (module 'spectrum'): ns3::SpectrumSignalParameters::duration [variable]
    cls.add_instance_attribute('duration', 'ns3::Time', is_const=False)
    ## spectrum-signal-parameters.h (module 'spectrum'): ns3::SpectrumSignalParameters::psd [variable]
    cls.add_instance_attribute('psd', 'ns3::Ptr< ns3::SpectrumValue >', is_const=False)
    ## spectrum-signal-parameters.h (module 'spectrum'): ns3::SpectrumSignalParameters::txAntenna [variable]
    cls.add_instance_attribute('txAntenna', 'ns3::Ptr< ns3::AntennaModel >', is_const=False)
    ## spectrum-signal-parameters.h (module 'spectrum'): ns3::SpectrumSignalParameters::txPhy [variable]
    cls.add_instance_attribute('txPhy', 'ns3::Ptr< ns3::SpectrumPhy >', is_const=False)
    return

def register_Ns3SpectrumValue_methods(root_module, cls):
    cls.add_binary_numeric_operator('*', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('double', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('ns3::SpectrumValue const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('double', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('ns3::SpectrumValue const &', 'right'))
    cls.add_unary_numeric_operator('-')
    cls.add_binary_numeric_operator('-', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('double', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('ns3::SpectrumValue const &', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('double', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::SpectrumValue'], root_module['ns3::SpectrumValue'], param('ns3::SpectrumValue const &', 'right'))
    cls.add_output_stream_operator()
    cls.add_inplace_numeric_operator('*=', param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('*=', param('double', 'right'))
    cls.add_inplace_numeric_operator('+=', param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('+=', param('double', 'right'))
    cls.add_inplace_numeric_operator('-=', param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('-=', param('double', 'right'))
    cls.add_inplace_numeric_operator('/=', param('ns3::SpectrumValue const &', 'right'))
    cls.add_inplace_numeric_operator('/=', param('double', 'right'))
    ## spectrum-value.h (module 'spectrum'): ns3::SpectrumValue::SpectrumValue(ns3::SpectrumValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumValue const &', 'arg0')])
    ## spectrum-value.h (module 'spectrum'): ns3::SpectrumValue::SpectrumValue(ns3::Ptr<ns3::SpectrumModel const> sm) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::SpectrumModel const >', 'sm')])
    ## spectrum-value.h (module 'spectrum'): ns3::SpectrumValue::SpectrumValue() [constructor]
    cls.add_constructor([])
    ## spectrum-value.h (module 'spectrum'): __gnu_cxx::__normal_iterator<const ns3::BandInfo*,std::vector<ns3::BandInfo, std::allocator<ns3::BandInfo> > > ns3::SpectrumValue::ConstBandsBegin() const [member function]
    cls.add_method('ConstBandsBegin', 
                   '__gnu_cxx::__normal_iterator< ns3::BandInfo const *, std::vector< ns3::BandInfo > >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h (module 'spectrum'): __gnu_cxx::__normal_iterator<const ns3::BandInfo*,std::vector<ns3::BandInfo, std::allocator<ns3::BandInfo> > > ns3::SpectrumValue::ConstBandsEnd() const [member function]
    cls.add_method('ConstBandsEnd', 
                   '__gnu_cxx::__normal_iterator< ns3::BandInfo const *, std::vector< ns3::BandInfo > >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h (module 'spectrum'): __gnu_cxx::__normal_iterator<const double*,std::vector<double, std::allocator<double> > > ns3::SpectrumValue::ConstValuesBegin() const [member function]
    cls.add_method('ConstValuesBegin', 
                   '__gnu_cxx::__normal_iterator< double const *, std::vector< double > >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h (module 'spectrum'): __gnu_cxx::__normal_iterator<const double*,std::vector<double, std::allocator<double> > > ns3::SpectrumValue::ConstValuesEnd() const [member function]
    cls.add_method('ConstValuesEnd', 
                   '__gnu_cxx::__normal_iterator< double const *, std::vector< double > >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h (module 'spectrum'): ns3::Ptr<ns3::SpectrumValue> ns3::SpectrumValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h (module 'spectrum'): ns3::Ptr<ns3::SpectrumModel const> ns3::SpectrumValue::GetSpectrumModel() const [member function]
    cls.add_method('GetSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True)
    ## spectrum-value.h (module 'spectrum'): ns3::SpectrumModelUid_t ns3::SpectrumValue::GetSpectrumModelUid() const [member function]
    cls.add_method('GetSpectrumModelUid', 
                   'ns3::SpectrumModelUid_t', 
                   [], 
                   is_const=True)
    ## spectrum-value.h (module 'spectrum'): __gnu_cxx::__normal_iterator<double*,std::vector<double, std::allocator<double> > > ns3::SpectrumValue::ValuesBegin() [member function]
    cls.add_method('ValuesBegin', 
                   '__gnu_cxx::__normal_iterator< double *, std::vector< double > >', 
                   [])
    ## spectrum-value.h (module 'spectrum'): __gnu_cxx::__normal_iterator<double*,std::vector<double, std::allocator<double> > > ns3::SpectrumValue::ValuesEnd() [member function]
    cls.add_method('ValuesEnd', 
                   '__gnu_cxx::__normal_iterator< double *, std::vector< double > >', 
                   [])
    return

def register_Ns3Time_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_inplace_numeric_operator('+=', param('ns3::Time const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    cls.add_inplace_numeric_operator('-=', param('ns3::Time const &', 'right'))
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>=')
    ## nstime.h (module 'core'): ns3::Time::Time() [constructor]
    cls.add_constructor([])
    ## nstime.h (module 'core'): ns3::Time::Time(ns3::Time const & o) [copy constructor]
    cls.add_constructor([param('ns3::Time const &', 'o')])
    ## nstime.h (module 'core'): ns3::Time::Time(double v) [constructor]
    cls.add_constructor([param('double', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(int v) [constructor]
    cls.add_constructor([param('int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(long int v) [constructor]
    cls.add_constructor([param('long int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(long long int v) [constructor]
    cls.add_constructor([param('long long int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(unsigned int v) [constructor]
    cls.add_constructor([param('unsigned int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(long unsigned int v) [constructor]
    cls.add_constructor([param('long unsigned int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(long long unsigned int v) [constructor]
    cls.add_constructor([param('long long unsigned int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(std::string const & s) [constructor]
    cls.add_constructor([param('std::string const &', 's')])
    ## nstime.h (module 'core'): ns3::Time::Time(ns3::int64x64_t const & value) [constructor]
    cls.add_constructor([param('ns3::int64x64_t const &', 'value')])
    ## nstime.h (module 'core'): int ns3::Time::Compare(ns3::Time const & o) const [member function]
    cls.add_method('Compare', 
                   'int', 
                   [param('ns3::Time const &', 'o')], 
                   is_const=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::From(ns3::int64x64_t const & from, ns3::Time::Unit timeUnit) [member function]
    cls.add_method('From', 
                   'ns3::Time', 
                   [param('ns3::int64x64_t const &', 'from'), param('ns3::Time::Unit', 'timeUnit')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::From(ns3::int64x64_t const & value) [member function]
    cls.add_method('From', 
                   'ns3::Time', 
                   [param('ns3::int64x64_t const &', 'value')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::FromDouble(double value, ns3::Time::Unit timeUnit) [member function]
    cls.add_method('FromDouble', 
                   'ns3::Time', 
                   [param('double', 'value'), param('ns3::Time::Unit', 'timeUnit')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::FromInteger(uint64_t value, ns3::Time::Unit timeUnit) [member function]
    cls.add_method('FromInteger', 
                   'ns3::Time', 
                   [param('uint64_t', 'value'), param('ns3::Time::Unit', 'timeUnit')], 
                   is_static=True)
    ## nstime.h (module 'core'): double ns3::Time::GetDouble() const [member function]
    cls.add_method('GetDouble', 
                   'double', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetFemtoSeconds() const [member function]
    cls.add_method('GetFemtoSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetInteger() const [member function]
    cls.add_method('GetInteger', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetMicroSeconds() const [member function]
    cls.add_method('GetMicroSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetMilliSeconds() const [member function]
    cls.add_method('GetMilliSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetNanoSeconds() const [member function]
    cls.add_method('GetNanoSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetPicoSeconds() const [member function]
    cls.add_method('GetPicoSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): static ns3::Time::Unit ns3::Time::GetResolution() [member function]
    cls.add_method('GetResolution', 
                   'ns3::Time::Unit', 
                   [], 
                   is_static=True)
    ## nstime.h (module 'core'): double ns3::Time::GetSeconds() const [member function]
    cls.add_method('GetSeconds', 
                   'double', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetTimeStep() const [member function]
    cls.add_method('GetTimeStep', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsNegative() const [member function]
    cls.add_method('IsNegative', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsPositive() const [member function]
    cls.add_method('IsPositive', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsStrictlyNegative() const [member function]
    cls.add_method('IsStrictlyNegative', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsStrictlyPositive() const [member function]
    cls.add_method('IsStrictlyPositive', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsZero() const [member function]
    cls.add_method('IsZero', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): static void ns3::Time::SetResolution(ns3::Time::Unit resolution) [member function]
    cls.add_method('SetResolution', 
                   'void', 
                   [param('ns3::Time::Unit', 'resolution')], 
                   is_static=True)
    ## nstime.h (module 'core'): ns3::int64x64_t ns3::Time::To(ns3::Time::Unit timeUnit) const [member function]
    cls.add_method('To', 
                   'ns3::int64x64_t', 
                   [param('ns3::Time::Unit', 'timeUnit')], 
                   is_const=True)
    ## nstime.h (module 'core'): double ns3::Time::ToDouble(ns3::Time::Unit timeUnit) const [member function]
    cls.add_method('ToDouble', 
                   'double', 
                   [param('ns3::Time::Unit', 'timeUnit')], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::ToInteger(ns3::Time::Unit timeUnit) const [member function]
    cls.add_method('ToInteger', 
                   'int64_t', 
                   [param('ns3::Time::Unit', 'timeUnit')], 
                   is_const=True)
    return

def register_Ns3TraceFadingLossModel_methods(root_module, cls):
    ## trace-fading-loss-model.h (module 'lte'): ns3::TraceFadingLossModel::TraceFadingLossModel(ns3::TraceFadingLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TraceFadingLossModel const &', 'arg0')])
    ## trace-fading-loss-model.h (module 'lte'): ns3::TraceFadingLossModel::TraceFadingLossModel() [constructor]
    cls.add_constructor([])
    ## trace-fading-loss-model.h (module 'lte'): int64_t ns3::TraceFadingLossModel::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')])
    ## trace-fading-loss-model.h (module 'lte'): void ns3::TraceFadingLossModel::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## trace-fading-loss-model.h (module 'lte'): static ns3::TypeId ns3::TraceFadingLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## trace-fading-loss-model.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::TraceFadingLossModel::DoCalcRxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> txPsd, ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'txPsd'), param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3TraceSourceAccessor_methods(root_module, cls):
    ## trace-source-accessor.h (module 'core'): ns3::TraceSourceAccessor::TraceSourceAccessor(ns3::TraceSourceAccessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TraceSourceAccessor const &', 'arg0')])
    ## trace-source-accessor.h (module 'core'): ns3::TraceSourceAccessor::TraceSourceAccessor() [constructor]
    cls.add_constructor([])
    ## trace-source-accessor.h (module 'core'): bool ns3::TraceSourceAccessor::Connect(ns3::ObjectBase * obj, std::string context, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('Connect', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h (module 'core'): bool ns3::TraceSourceAccessor::ConnectWithoutContext(ns3::ObjectBase * obj, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('ConnectWithoutContext', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h (module 'core'): bool ns3::TraceSourceAccessor::Disconnect(ns3::ObjectBase * obj, std::string context, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('Disconnect', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h (module 'core'): bool ns3::TraceSourceAccessor::DisconnectWithoutContext(ns3::ObjectBase * obj, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('DisconnectWithoutContext', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3Trailer_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## trailer.h (module 'network'): ns3::Trailer::Trailer() [constructor]
    cls.add_constructor([])
    ## trailer.h (module 'network'): ns3::Trailer::Trailer(ns3::Trailer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Trailer const &', 'arg0')])
    ## trailer.h (module 'network'): uint32_t ns3::Trailer::Deserialize(ns3::Buffer::Iterator end) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'end')], 
                   is_pure_virtual=True, is_virtual=True)
    ## trailer.h (module 'network'): uint32_t ns3::Trailer::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trailer.h (module 'network'): static ns3::TypeId ns3::Trailer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## trailer.h (module 'network'): void ns3::Trailer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trailer.h (module 'network'): void ns3::Trailer::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3TriangularRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::TriangularRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::TriangularRandomVariable::TriangularRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::TriangularRandomVariable::GetMean() const [member function]
    cls.add_method('GetMean', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::TriangularRandomVariable::GetMin() const [member function]
    cls.add_method('GetMin', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::TriangularRandomVariable::GetMax() const [member function]
    cls.add_method('GetMax', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::TriangularRandomVariable::GetValue(double mean, double min, double max) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'mean'), param('double', 'min'), param('double', 'max')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::TriangularRandomVariable::GetInteger(uint32_t mean, uint32_t min, uint32_t max) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'mean'), param('uint32_t', 'min'), param('uint32_t', 'max')])
    ## random-variable-stream.h (module 'core'): double ns3::TriangularRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::TriangularRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3UeInfo_methods(root_module, cls):
    ## lte-enb-rrc.h (module 'lte'): ns3::UeInfo::UeInfo(ns3::UeInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeInfo const &', 'arg0')])
    ## lte-enb-rrc.h (module 'lte'): ns3::UeInfo::UeInfo() [constructor]
    cls.add_constructor([])
    ## lte-enb-rrc.h (module 'lte'): ns3::UeInfo::UeInfo(uint64_t imsi) [constructor]
    cls.add_constructor([param('uint64_t', 'imsi')])
    ## lte-enb-rrc.h (module 'lte'): uint8_t ns3::UeInfo::AddRadioBearer(ns3::Ptr<ns3::LteRadioBearerInfo> radioBearerInfo) [member function]
    cls.add_method('AddRadioBearer', 
                   'uint8_t', 
                   [param('ns3::Ptr< ns3::LteRadioBearerInfo >', 'radioBearerInfo')])
    ## lte-enb-rrc.h (module 'lte'): uint64_t ns3::UeInfo::GetImsi() [member function]
    cls.add_method('GetImsi', 
                   'uint64_t', 
                   [])
    ## lte-enb-rrc.h (module 'lte'): ns3::Ptr<ns3::LteRadioBearerInfo> ns3::UeInfo::GetRadioBearer(uint8_t lcid) [member function]
    cls.add_method('GetRadioBearer', 
                   'ns3::Ptr< ns3::LteRadioBearerInfo >', 
                   [param('uint8_t', 'lcid')])
    ## lte-enb-rrc.h (module 'lte'): static ns3::TypeId ns3::UeInfo::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-enb-rrc.h (module 'lte'): void ns3::UeInfo::RemoveRadioBearer(uint8_t lcid) [member function]
    cls.add_method('RemoveRadioBearer', 
                   'void', 
                   [param('uint8_t', 'lcid')])
    return

def register_Ns3UniformRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::UniformRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::UniformRandomVariable::UniformRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::UniformRandomVariable::GetMin() const [member function]
    cls.add_method('GetMin', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::UniformRandomVariable::GetMax() const [member function]
    cls.add_method('GetMax', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::UniformRandomVariable::GetValue(double min, double max) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'min'), param('double', 'max')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::UniformRandomVariable::GetInteger(uint32_t min, uint32_t max) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'min'), param('uint32_t', 'max')])
    ## random-variable-stream.h (module 'core'): double ns3::UniformRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::UniformRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3WeibullRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::WeibullRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::WeibullRandomVariable::WeibullRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::WeibullRandomVariable::GetScale() const [member function]
    cls.add_method('GetScale', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::WeibullRandomVariable::GetShape() const [member function]
    cls.add_method('GetShape', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::WeibullRandomVariable::GetBound() const [member function]
    cls.add_method('GetBound', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::WeibullRandomVariable::GetValue(double scale, double shape, double bound) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'scale'), param('double', 'shape'), param('double', 'bound')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::WeibullRandomVariable::GetInteger(uint32_t scale, uint32_t shape, uint32_t bound) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'scale'), param('uint32_t', 'shape'), param('uint32_t', 'bound')])
    ## random-variable-stream.h (module 'core'): double ns3::WeibullRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::WeibullRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3ZetaRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::ZetaRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::ZetaRandomVariable::ZetaRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::ZetaRandomVariable::GetAlpha() const [member function]
    cls.add_method('GetAlpha', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ZetaRandomVariable::GetValue(double alpha) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'alpha')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ZetaRandomVariable::GetInteger(uint32_t alpha) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'alpha')])
    ## random-variable-stream.h (module 'core'): double ns3::ZetaRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ZetaRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3ZipfRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::ZipfRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::ZipfRandomVariable::ZipfRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ZipfRandomVariable::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ZipfRandomVariable::GetAlpha() const [member function]
    cls.add_method('GetAlpha', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ZipfRandomVariable::GetValue(uint32_t n, double alpha) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('uint32_t', 'n'), param('double', 'alpha')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ZipfRandomVariable::GetInteger(uint32_t n, uint32_t alpha) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'n'), param('uint32_t', 'alpha')])
    ## random-variable-stream.h (module 'core'): double ns3::ZipfRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ZipfRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3Application_methods(root_module, cls):
    ## application.h (module 'network'): ns3::Application::Application(ns3::Application const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Application const &', 'arg0')])
    ## application.h (module 'network'): ns3::Application::Application() [constructor]
    cls.add_constructor([])
    ## application.h (module 'network'): ns3::Ptr<ns3::Node> ns3::Application::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## application.h (module 'network'): static ns3::TypeId ns3::Application::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## application.h (module 'network'): void ns3::Application::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## application.h (module 'network'): void ns3::Application::SetStartTime(ns3::Time start) [member function]
    cls.add_method('SetStartTime', 
                   'void', 
                   [param('ns3::Time', 'start')])
    ## application.h (module 'network'): void ns3::Application::SetStopTime(ns3::Time stop) [member function]
    cls.add_method('SetStopTime', 
                   'void', 
                   [param('ns3::Time', 'stop')])
    ## application.h (module 'network'): void ns3::Application::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## application.h (module 'network'): void ns3::Application::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## application.h (module 'network'): void ns3::Application::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## application.h (module 'network'): void ns3::Application::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AttributeAccessor_methods(root_module, cls):
    ## attribute.h (module 'core'): ns3::AttributeAccessor::AttributeAccessor(ns3::AttributeAccessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeAccessor const &', 'arg0')])
    ## attribute.h (module 'core'): ns3::AttributeAccessor::AttributeAccessor() [constructor]
    cls.add_constructor([])
    ## attribute.h (module 'core'): bool ns3::AttributeAccessor::Get(ns3::ObjectBase const * object, ns3::AttributeValue & attribute) const [member function]
    cls.add_method('Get', 
                   'bool', 
                   [param('ns3::ObjectBase const *', 'object'), param('ns3::AttributeValue &', 'attribute')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeAccessor::HasGetter() const [member function]
    cls.add_method('HasGetter', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeAccessor::HasSetter() const [member function]
    cls.add_method('HasSetter', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeAccessor::Set(ns3::ObjectBase * object, ns3::AttributeValue const & value) const [member function]
    cls.add_method('Set', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'object', transfer_ownership=False), param('ns3::AttributeValue const &', 'value')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3AttributeChecker_methods(root_module, cls):
    ## attribute.h (module 'core'): ns3::AttributeChecker::AttributeChecker(ns3::AttributeChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeChecker const &', 'arg0')])
    ## attribute.h (module 'core'): ns3::AttributeChecker::AttributeChecker() [constructor]
    cls.add_constructor([])
    ## attribute.h (module 'core'): bool ns3::AttributeChecker::Check(ns3::AttributeValue const & value) const [member function]
    cls.add_method('Check', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'value')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeChecker::Copy(ns3::AttributeValue const & source, ns3::AttributeValue & destination) const [member function]
    cls.add_method('Copy', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'source'), param('ns3::AttributeValue &', 'destination')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::AttributeChecker::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::AttributeChecker::CreateValidValue(ns3::AttributeValue const & value) const [member function]
    cls.add_method('CreateValidValue', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [param('ns3::AttributeValue const &', 'value')], 
                   is_const=True)
    ## attribute.h (module 'core'): std::string ns3::AttributeChecker::GetUnderlyingTypeInformation() const [member function]
    cls.add_method('GetUnderlyingTypeInformation', 
                   'std::string', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): std::string ns3::AttributeChecker::GetValueTypeName() const [member function]
    cls.add_method('GetValueTypeName', 
                   'std::string', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeChecker::HasUnderlyingTypeInformation() const [member function]
    cls.add_method('HasUnderlyingTypeInformation', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3AttributeValue_methods(root_module, cls):
    ## attribute.h (module 'core'): ns3::AttributeValue::AttributeValue(ns3::AttributeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeValue const &', 'arg0')])
    ## attribute.h (module 'core'): ns3::AttributeValue::AttributeValue() [constructor]
    cls.add_constructor([])
    ## attribute.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::AttributeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_pure_virtual=True, is_virtual=True)
    ## attribute.h (module 'core'): std::string ns3::AttributeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3BooleanChecker_methods(root_module, cls):
    ## boolean.h (module 'core'): ns3::BooleanChecker::BooleanChecker() [constructor]
    cls.add_constructor([])
    ## boolean.h (module 'core'): ns3::BooleanChecker::BooleanChecker(ns3::BooleanChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BooleanChecker const &', 'arg0')])
    return

def register_Ns3BooleanValue_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## boolean.h (module 'core'): ns3::BooleanValue::BooleanValue(ns3::BooleanValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BooleanValue const &', 'arg0')])
    ## boolean.h (module 'core'): ns3::BooleanValue::BooleanValue() [constructor]
    cls.add_constructor([])
    ## boolean.h (module 'core'): ns3::BooleanValue::BooleanValue(bool value) [constructor]
    cls.add_constructor([param('bool', 'value')])
    ## boolean.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::BooleanValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## boolean.h (module 'core'): bool ns3::BooleanValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## boolean.h (module 'core'): bool ns3::BooleanValue::Get() const [member function]
    cls.add_method('Get', 
                   'bool', 
                   [], 
                   is_const=True)
    ## boolean.h (module 'core'): std::string ns3::BooleanValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## boolean.h (module 'core'): void ns3::BooleanValue::Set(bool value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('bool', 'value')])
    return

def register_Ns3CallbackChecker_methods(root_module, cls):
    ## callback.h (module 'core'): ns3::CallbackChecker::CallbackChecker() [constructor]
    cls.add_constructor([])
    ## callback.h (module 'core'): ns3::CallbackChecker::CallbackChecker(ns3::CallbackChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackChecker const &', 'arg0')])
    return

def register_Ns3CallbackImplBase_methods(root_module, cls):
    ## callback.h (module 'core'): ns3::CallbackImplBase::CallbackImplBase() [constructor]
    cls.add_constructor([])
    ## callback.h (module 'core'): ns3::CallbackImplBase::CallbackImplBase(ns3::CallbackImplBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackImplBase const &', 'arg0')])
    ## callback.h (module 'core'): bool ns3::CallbackImplBase::IsEqual(ns3::Ptr<ns3::CallbackImplBase const> other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ptr< ns3::CallbackImplBase const >', 'other')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3CallbackValue_methods(root_module, cls):
    ## callback.h (module 'core'): ns3::CallbackValue::CallbackValue(ns3::CallbackValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackValue const &', 'arg0')])
    ## callback.h (module 'core'): ns3::CallbackValue::CallbackValue() [constructor]
    cls.add_constructor([])
    ## callback.h (module 'core'): ns3::CallbackValue::CallbackValue(ns3::CallbackBase const & base) [constructor]
    cls.add_constructor([param('ns3::CallbackBase const &', 'base')])
    ## callback.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::CallbackValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## callback.h (module 'core'): bool ns3::CallbackValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## callback.h (module 'core'): std::string ns3::CallbackValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## callback.h (module 'core'): void ns3::CallbackValue::Set(ns3::CallbackBase base) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::CallbackBase', 'base')])
    return

def register_Ns3Channel_methods(root_module, cls):
    ## channel.h (module 'network'): ns3::Channel::Channel(ns3::Channel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Channel const &', 'arg0')])
    ## channel.h (module 'network'): ns3::Channel::Channel() [constructor]
    cls.add_constructor([])
    ## channel.h (module 'network'): ns3::Ptr<ns3::NetDevice> ns3::Channel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## channel.h (module 'network'): uint32_t ns3::Channel::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## channel.h (module 'network'): uint32_t ns3::Channel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## channel.h (module 'network'): static ns3::TypeId ns3::Channel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3ConstantRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::ConstantRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::ConstantRandomVariable::ConstantRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::ConstantRandomVariable::GetConstant() const [member function]
    cls.add_method('GetConstant', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ConstantRandomVariable::GetValue(double constant) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'constant')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ConstantRandomVariable::GetInteger(uint32_t constant) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'constant')])
    ## random-variable-stream.h (module 'core'): double ns3::ConstantRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ConstantRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3DataCalculator_methods(root_module, cls):
    ## data-calculator.h (module 'stats'): ns3::DataCalculator::DataCalculator(ns3::DataCalculator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataCalculator const &', 'arg0')])
    ## data-calculator.h (module 'stats'): ns3::DataCalculator::DataCalculator() [constructor]
    cls.add_constructor([])
    ## data-calculator.h (module 'stats'): void ns3::DataCalculator::Disable() [member function]
    cls.add_method('Disable', 
                   'void', 
                   [])
    ## data-calculator.h (module 'stats'): void ns3::DataCalculator::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [])
    ## data-calculator.h (module 'stats'): std::string ns3::DataCalculator::GetContext() const [member function]
    cls.add_method('GetContext', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-calculator.h (module 'stats'): bool ns3::DataCalculator::GetEnabled() const [member function]
    cls.add_method('GetEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## data-calculator.h (module 'stats'): std::string ns3::DataCalculator::GetKey() const [member function]
    cls.add_method('GetKey', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-calculator.h (module 'stats'): void ns3::DataCalculator::Output(ns3::DataOutputCallback & callback) const [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataOutputCallback &', 'callback')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## data-calculator.h (module 'stats'): void ns3::DataCalculator::SetContext(std::string const context) [member function]
    cls.add_method('SetContext', 
                   'void', 
                   [param('std::string const', 'context')])
    ## data-calculator.h (module 'stats'): void ns3::DataCalculator::SetKey(std::string const key) [member function]
    cls.add_method('SetKey', 
                   'void', 
                   [param('std::string const', 'key')])
    ## data-calculator.h (module 'stats'): void ns3::DataCalculator::Start(ns3::Time const & startTime) [member function]
    cls.add_method('Start', 
                   'void', 
                   [param('ns3::Time const &', 'startTime')], 
                   is_virtual=True)
    ## data-calculator.h (module 'stats'): void ns3::DataCalculator::Stop(ns3::Time const & stopTime) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time const &', 'stopTime')], 
                   is_virtual=True)
    ## data-calculator.h (module 'stats'): void ns3::DataCalculator::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3DataOutputInterface_methods(root_module, cls):
    ## data-output-interface.h (module 'stats'): ns3::DataOutputInterface::DataOutputInterface(ns3::DataOutputInterface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataOutputInterface const &', 'arg0')])
    ## data-output-interface.h (module 'stats'): ns3::DataOutputInterface::DataOutputInterface() [constructor]
    cls.add_constructor([])
    ## data-output-interface.h (module 'stats'): std::string ns3::DataOutputInterface::GetFilePrefix() const [member function]
    cls.add_method('GetFilePrefix', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputInterface::Output(ns3::DataCollector & dc) [member function]
    cls.add_method('Output', 
                   'void', 
                   [param('ns3::DataCollector &', 'dc')], 
                   is_pure_virtual=True, is_virtual=True)
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputInterface::SetFilePrefix(std::string const prefix) [member function]
    cls.add_method('SetFilePrefix', 
                   'void', 
                   [param('std::string const', 'prefix')])
    ## data-output-interface.h (module 'stats'): void ns3::DataOutputInterface::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3DataRateChecker_methods(root_module, cls):
    ## data-rate.h (module 'network'): ns3::DataRateChecker::DataRateChecker() [constructor]
    cls.add_constructor([])
    ## data-rate.h (module 'network'): ns3::DataRateChecker::DataRateChecker(ns3::DataRateChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRateChecker const &', 'arg0')])
    return

def register_Ns3DataRateValue_methods(root_module, cls):
    ## data-rate.h (module 'network'): ns3::DataRateValue::DataRateValue() [constructor]
    cls.add_constructor([])
    ## data-rate.h (module 'network'): ns3::DataRateValue::DataRateValue(ns3::DataRateValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRateValue const &', 'arg0')])
    ## data-rate.h (module 'network'): ns3::DataRateValue::DataRateValue(ns3::DataRate const & value) [constructor]
    cls.add_constructor([param('ns3::DataRate const &', 'value')])
    ## data-rate.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::DataRateValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## data-rate.h (module 'network'): bool ns3::DataRateValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## data-rate.h (module 'network'): ns3::DataRate ns3::DataRateValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::DataRate', 
                   [], 
                   is_const=True)
    ## data-rate.h (module 'network'): std::string ns3::DataRateValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## data-rate.h (module 'network'): void ns3::DataRateValue::Set(ns3::DataRate const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::DataRate const &', 'value')])
    return

def register_Ns3DeterministicRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::DeterministicRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::DeterministicRandomVariable::DeterministicRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): void ns3::DeterministicRandomVariable::SetValueArray(double * values, uint64_t length) [member function]
    cls.add_method('SetValueArray', 
                   'void', 
                   [param('double *', 'values'), param('uint64_t', 'length')])
    ## random-variable-stream.h (module 'core'): double ns3::DeterministicRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::DeterministicRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3DoubleValue_methods(root_module, cls):
    ## double.h (module 'core'): ns3::DoubleValue::DoubleValue() [constructor]
    cls.add_constructor([])
    ## double.h (module 'core'): ns3::DoubleValue::DoubleValue(ns3::DoubleValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DoubleValue const &', 'arg0')])
    ## double.h (module 'core'): ns3::DoubleValue::DoubleValue(double const & value) [constructor]
    cls.add_constructor([param('double const &', 'value')])
    ## double.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::DoubleValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## double.h (module 'core'): bool ns3::DoubleValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## double.h (module 'core'): double ns3::DoubleValue::Get() const [member function]
    cls.add_method('Get', 
                   'double', 
                   [], 
                   is_const=True)
    ## double.h (module 'core'): std::string ns3::DoubleValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## double.h (module 'core'): void ns3::DoubleValue::Set(double const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('double const &', 'value')])
    return

def register_Ns3EmpiricalRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): ns3::EmpiricalRandomVariable::EmpiricalRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): void ns3::EmpiricalRandomVariable::CDF(double v, double c) [member function]
    cls.add_method('CDF', 
                   'void', 
                   [param('double', 'v'), param('double', 'c')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::EmpiricalRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::EmpiricalRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): double ns3::EmpiricalRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): double ns3::EmpiricalRandomVariable::Interpolate(double arg0, double arg1, double arg2, double arg3, double arg4) [member function]
    cls.add_method('Interpolate', 
                   'double', 
                   [param('double', 'arg0'), param('double', 'arg1'), param('double', 'arg2'), param('double', 'arg3'), param('double', 'arg4')], 
                   visibility='private', is_virtual=True)
    ## random-variable-stream.h (module 'core'): void ns3::EmpiricalRandomVariable::Validate() [member function]
    cls.add_method('Validate', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3EmptyAttributeValue_methods(root_module, cls):
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue::EmptyAttributeValue(ns3::EmptyAttributeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EmptyAttributeValue const &', 'arg0')])
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue::EmptyAttributeValue() [constructor]
    cls.add_constructor([])
    ## attribute.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::EmptyAttributeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::EmptyAttributeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   visibility='private', is_virtual=True)
    ## attribute.h (module 'core'): std::string ns3::EmptyAttributeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3EnumChecker_methods(root_module, cls):
    ## enum.h (module 'core'): ns3::EnumChecker::EnumChecker(ns3::EnumChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnumChecker const &', 'arg0')])
    ## enum.h (module 'core'): ns3::EnumChecker::EnumChecker() [constructor]
    cls.add_constructor([])
    ## enum.h (module 'core'): void ns3::EnumChecker::Add(int v, std::string name) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('int', 'v'), param('std::string', 'name')])
    ## enum.h (module 'core'): void ns3::EnumChecker::AddDefault(int v, std::string name) [member function]
    cls.add_method('AddDefault', 
                   'void', 
                   [param('int', 'v'), param('std::string', 'name')])
    ## enum.h (module 'core'): bool ns3::EnumChecker::Check(ns3::AttributeValue const & value) const [member function]
    cls.add_method('Check', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'value')], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): bool ns3::EnumChecker::Copy(ns3::AttributeValue const & src, ns3::AttributeValue & dst) const [member function]
    cls.add_method('Copy', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'src'), param('ns3::AttributeValue &', 'dst')], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::EnumChecker::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): std::string ns3::EnumChecker::GetUnderlyingTypeInformation() const [member function]
    cls.add_method('GetUnderlyingTypeInformation', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): std::string ns3::EnumChecker::GetValueTypeName() const [member function]
    cls.add_method('GetValueTypeName', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): bool ns3::EnumChecker::HasUnderlyingTypeInformation() const [member function]
    cls.add_method('HasUnderlyingTypeInformation', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3EnumValue_methods(root_module, cls):
    ## enum.h (module 'core'): ns3::EnumValue::EnumValue(ns3::EnumValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnumValue const &', 'arg0')])
    ## enum.h (module 'core'): ns3::EnumValue::EnumValue() [constructor]
    cls.add_constructor([])
    ## enum.h (module 'core'): ns3::EnumValue::EnumValue(int v) [constructor]
    cls.add_constructor([param('int', 'v')])
    ## enum.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::EnumValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): bool ns3::EnumValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## enum.h (module 'core'): int ns3::EnumValue::Get() const [member function]
    cls.add_method('Get', 
                   'int', 
                   [], 
                   is_const=True)
    ## enum.h (module 'core'): std::string ns3::EnumValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): void ns3::EnumValue::Set(int v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('int', 'v')])
    return

def register_Ns3EpcEnbApplication_methods(root_module, cls):
    ## epc-enb-application.h (module 'lte'): ns3::EpcEnbApplication::EpcEnbApplication(ns3::EpcEnbApplication const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EpcEnbApplication const &', 'arg0')])
    ## epc-enb-application.h (module 'lte'): ns3::EpcEnbApplication::EpcEnbApplication(ns3::Ptr<ns3::Socket> lteSocket, ns3::Ptr<ns3::Socket> s1uSocket, ns3::Ipv4Address sgwAddress) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Socket >', 'lteSocket'), param('ns3::Ptr< ns3::Socket >', 's1uSocket'), param('ns3::Ipv4Address', 'sgwAddress')])
    ## epc-enb-application.h (module 'lte'): void ns3::EpcEnbApplication::ErabSetupRequest(uint32_t teid, uint16_t rnti, uint8_t lcid) [member function]
    cls.add_method('ErabSetupRequest', 
                   'void', 
                   [param('uint32_t', 'teid'), param('uint16_t', 'rnti'), param('uint8_t', 'lcid')])
    ## epc-enb-application.h (module 'lte'): static ns3::TypeId ns3::EpcEnbApplication::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## epc-enb-application.h (module 'lte'): void ns3::EpcEnbApplication::RecvFromLteSocket(ns3::Ptr<ns3::Socket> socket) [member function]
    cls.add_method('RecvFromLteSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket')])
    ## epc-enb-application.h (module 'lte'): void ns3::EpcEnbApplication::RecvFromS1uSocket(ns3::Ptr<ns3::Socket> socket) [member function]
    cls.add_method('RecvFromS1uSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket')])
    ## epc-enb-application.h (module 'lte'): void ns3::EpcEnbApplication::SendToLteSocket(ns3::Ptr<ns3::Packet> packet, uint16_t rnti, uint8_t lcid) [member function]
    cls.add_method('SendToLteSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t', 'rnti'), param('uint8_t', 'lcid')])
    ## epc-enb-application.h (module 'lte'): void ns3::EpcEnbApplication::SendToS1uSocket(ns3::Ptr<ns3::Packet> packet, uint32_t teid) [member function]
    cls.add_method('SendToS1uSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint32_t', 'teid')])
    return

def register_Ns3EpcHelper_methods(root_module, cls):
    ## epc-helper.h (module 'lte'): ns3::EpcHelper::EpcHelper(ns3::EpcHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EpcHelper const &', 'arg0')])
    ## epc-helper.h (module 'lte'): ns3::EpcHelper::EpcHelper() [constructor]
    cls.add_constructor([])
    ## epc-helper.h (module 'lte'): void ns3::EpcHelper::ActivateEpsBearer(ns3::Ptr<ns3::NetDevice> ueLteDevice, ns3::Ptr<ns3::NetDevice> enbLteDevice, ns3::Ptr<ns3::EpcTft> tft, uint16_t rnti, uint8_t lcid) [member function]
    cls.add_method('ActivateEpsBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'ueLteDevice'), param('ns3::Ptr< ns3::NetDevice >', 'enbLteDevice'), param('ns3::Ptr< ns3::EpcTft >', 'tft'), param('uint16_t', 'rnti'), param('uint8_t', 'lcid')])
    ## epc-helper.h (module 'lte'): void ns3::EpcHelper::AddEnb(ns3::Ptr<ns3::Node> enbNode, ns3::Ptr<ns3::NetDevice> lteEnbNetDevice) [member function]
    cls.add_method('AddEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'enbNode'), param('ns3::Ptr< ns3::NetDevice >', 'lteEnbNetDevice')])
    ## epc-helper.h (module 'lte'): ns3::Ipv4InterfaceContainer ns3::EpcHelper::AssignUeIpv4Address(ns3::NetDeviceContainer ueDevices) [member function]
    cls.add_method('AssignUeIpv4Address', 
                   'ns3::Ipv4InterfaceContainer', 
                   [param('ns3::NetDeviceContainer', 'ueDevices')])
    ## epc-helper.h (module 'lte'): void ns3::EpcHelper::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## epc-helper.h (module 'lte'): ns3::Ptr<ns3::Node> ns3::EpcHelper::GetPgwNode() [member function]
    cls.add_method('GetPgwNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [])
    ## epc-helper.h (module 'lte'): static ns3::TypeId ns3::EpcHelper::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## epc-helper.h (module 'lte'): ns3::Ipv4Address ns3::EpcHelper::GetUeDefaultGatewayAddress() [member function]
    cls.add_method('GetUeDefaultGatewayAddress', 
                   'ns3::Ipv4Address', 
                   [])
    return

def register_Ns3EpcSgwPgwApplication_methods(root_module, cls):
    ## epc-sgw-pgw-application.h (module 'lte'): ns3::EpcSgwPgwApplication::EpcSgwPgwApplication(ns3::EpcSgwPgwApplication const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EpcSgwPgwApplication const &', 'arg0')])
    ## epc-sgw-pgw-application.h (module 'lte'): ns3::EpcSgwPgwApplication::EpcSgwPgwApplication(ns3::Ptr<ns3::VirtualNetDevice> const tunDevice, ns3::Ptr<ns3::Socket> const s1uSocket) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::VirtualNetDevice > const', 'tunDevice'), param('ns3::Ptr< ns3::Socket > const', 's1uSocket')])
    ## epc-sgw-pgw-application.h (module 'lte'): uint32_t ns3::EpcSgwPgwApplication::ActivateS1Bearer(ns3::Ipv4Address ueAddr, ns3::Ipv4Address enbAddr, ns3::Ptr<ns3::EpcTft> tft) [member function]
    cls.add_method('ActivateS1Bearer', 
                   'uint32_t', 
                   [param('ns3::Ipv4Address', 'ueAddr'), param('ns3::Ipv4Address', 'enbAddr'), param('ns3::Ptr< ns3::EpcTft >', 'tft')])
    ## epc-sgw-pgw-application.h (module 'lte'): void ns3::EpcSgwPgwApplication::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## epc-sgw-pgw-application.h (module 'lte'): static ns3::TypeId ns3::EpcSgwPgwApplication::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## epc-sgw-pgw-application.h (module 'lte'): void ns3::EpcSgwPgwApplication::RecvFromS1uSocket(ns3::Ptr<ns3::Socket> socket) [member function]
    cls.add_method('RecvFromS1uSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket')])
    ## epc-sgw-pgw-application.h (module 'lte'): bool ns3::EpcSgwPgwApplication::RecvFromTunDevice(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('RecvFromTunDevice', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')])
    ## epc-sgw-pgw-application.h (module 'lte'): void ns3::EpcSgwPgwApplication::SendToS1uSocket(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Address enbS1uAddress, uint32_t teid) [member function]
    cls.add_method('SendToS1uSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Address', 'enbS1uAddress'), param('uint32_t', 'teid')])
    ## epc-sgw-pgw-application.h (module 'lte'): void ns3::EpcSgwPgwApplication::SendToTunDevice(ns3::Ptr<ns3::Packet> packet, uint32_t teid) [member function]
    cls.add_method('SendToTunDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint32_t', 'teid')])
    return

def register_Ns3EpcTft_methods(root_module, cls):
    ## epc-tft.h (module 'lte'): ns3::EpcTft::EpcTft(ns3::EpcTft const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EpcTft const &', 'arg0')])
    ## epc-tft.h (module 'lte'): ns3::EpcTft::EpcTft() [constructor]
    cls.add_constructor([])
    ## epc-tft.h (module 'lte'): uint8_t ns3::EpcTft::Add(ns3::EpcTft::PacketFilter f) [member function]
    cls.add_method('Add', 
                   'uint8_t', 
                   [param('ns3::EpcTft::PacketFilter', 'f')])
    ## epc-tft.h (module 'lte'): static ns3::Ptr<ns3::EpcTft> ns3::EpcTft::Default() [member function]
    cls.add_method('Default', 
                   'ns3::Ptr< ns3::EpcTft >', 
                   [], 
                   is_static=True)
    ## epc-tft.h (module 'lte'): bool ns3::EpcTft::Matches(ns3::EpcTft::Direction direction, ns3::Ipv4Address remoteAddress, ns3::Ipv4Address localAddress, uint16_t remotePort, uint16_t localPort, uint8_t typeOfService) [member function]
    cls.add_method('Matches', 
                   'bool', 
                   [param('ns3::EpcTft::Direction', 'direction'), param('ns3::Ipv4Address', 'remoteAddress'), param('ns3::Ipv4Address', 'localAddress'), param('uint16_t', 'remotePort'), param('uint16_t', 'localPort'), param('uint8_t', 'typeOfService')])
    return

def register_Ns3EpcTftPacketFilter_methods(root_module, cls):
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::PacketFilter(ns3::EpcTft::PacketFilter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EpcTft::PacketFilter const &', 'arg0')])
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::PacketFilter() [constructor]
    cls.add_constructor([])
    ## epc-tft.h (module 'lte'): bool ns3::EpcTft::PacketFilter::Matches(ns3::EpcTft::Direction d, ns3::Ipv4Address ra, ns3::Ipv4Address la, uint16_t rp, uint16_t lp, uint8_t tos) [member function]
    cls.add_method('Matches', 
                   'bool', 
                   [param('ns3::EpcTft::Direction', 'd'), param('ns3::Ipv4Address', 'ra'), param('ns3::Ipv4Address', 'la'), param('uint16_t', 'rp'), param('uint16_t', 'lp'), param('uint8_t', 'tos')])
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::direction [variable]
    cls.add_instance_attribute('direction', 'ns3::EpcTft::Direction', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::localAddress [variable]
    cls.add_instance_attribute('localAddress', 'ns3::Ipv4Address', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::localMask [variable]
    cls.add_instance_attribute('localMask', 'ns3::Ipv4Mask', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::localPortEnd [variable]
    cls.add_instance_attribute('localPortEnd', 'uint16_t', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::localPortStart [variable]
    cls.add_instance_attribute('localPortStart', 'uint16_t', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::precedence [variable]
    cls.add_instance_attribute('precedence', 'uint8_t', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::remoteAddress [variable]
    cls.add_instance_attribute('remoteAddress', 'ns3::Ipv4Address', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::remoteMask [variable]
    cls.add_instance_attribute('remoteMask', 'ns3::Ipv4Mask', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::remotePortEnd [variable]
    cls.add_instance_attribute('remotePortEnd', 'uint16_t', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::remotePortStart [variable]
    cls.add_instance_attribute('remotePortStart', 'uint16_t', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::typeOfService [variable]
    cls.add_instance_attribute('typeOfService', 'uint8_t', is_const=False)
    ## epc-tft.h (module 'lte'): ns3::EpcTft::PacketFilter::typeOfServiceMask [variable]
    cls.add_instance_attribute('typeOfServiceMask', 'uint8_t', is_const=False)
    return

def register_Ns3EpcTftClassifier_methods(root_module, cls):
    ## epc-tft-classifier.h (module 'lte'): ns3::EpcTftClassifier::EpcTftClassifier(ns3::EpcTftClassifier const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EpcTftClassifier const &', 'arg0')])
    ## epc-tft-classifier.h (module 'lte'): ns3::EpcTftClassifier::EpcTftClassifier() [constructor]
    cls.add_constructor([])
    ## epc-tft-classifier.h (module 'lte'): void ns3::EpcTftClassifier::Add(ns3::Ptr<ns3::EpcTft> tft, uint32_t id) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::EpcTft >', 'tft'), param('uint32_t', 'id')])
    ## epc-tft-classifier.h (module 'lte'): uint32_t ns3::EpcTftClassifier::Classify(ns3::Ptr<ns3::Packet> p, ns3::EpcTft::Direction direction) [member function]
    cls.add_method('Classify', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::EpcTft::Direction', 'direction')])
    ## epc-tft-classifier.h (module 'lte'): void ns3::EpcTftClassifier::Delete(uint32_t id) [member function]
    cls.add_method('Delete', 
                   'void', 
                   [param('uint32_t', 'id')])
    return

def register_Ns3ErlangRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::ErlangRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::ErlangRandomVariable::ErlangRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ErlangRandomVariable::GetK() const [member function]
    cls.add_method('GetK', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ErlangRandomVariable::GetLambda() const [member function]
    cls.add_method('GetLambda', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ErlangRandomVariable::GetValue(uint32_t k, double lambda) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('uint32_t', 'k'), param('double', 'lambda')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ErlangRandomVariable::GetInteger(uint32_t k, uint32_t lambda) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'k'), param('uint32_t', 'lambda')])
    ## random-variable-stream.h (module 'core'): double ns3::ErlangRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ErlangRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3EventImpl_methods(root_module, cls):
    ## event-impl.h (module 'core'): ns3::EventImpl::EventImpl(ns3::EventImpl const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EventImpl const &', 'arg0')])
    ## event-impl.h (module 'core'): ns3::EventImpl::EventImpl() [constructor]
    cls.add_constructor([])
    ## event-impl.h (module 'core'): void ns3::EventImpl::Cancel() [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [])
    ## event-impl.h (module 'core'): void ns3::EventImpl::Invoke() [member function]
    cls.add_method('Invoke', 
                   'void', 
                   [])
    ## event-impl.h (module 'core'): bool ns3::EventImpl::IsCancelled() [member function]
    cls.add_method('IsCancelled', 
                   'bool', 
                   [])
    ## event-impl.h (module 'core'): void ns3::EventImpl::Notify() [member function]
    cls.add_method('Notify', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    return

def register_Ns3ExponentialRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::ExponentialRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::ExponentialRandomVariable::ExponentialRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::ExponentialRandomVariable::GetMean() const [member function]
    cls.add_method('GetMean', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ExponentialRandomVariable::GetBound() const [member function]
    cls.add_method('GetBound', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ExponentialRandomVariable::GetValue(double mean, double bound) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'mean'), param('double', 'bound')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ExponentialRandomVariable::GetInteger(uint32_t mean, uint32_t bound) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'mean'), param('uint32_t', 'bound')])
    ## random-variable-stream.h (module 'core'): double ns3::ExponentialRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ExponentialRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3FfMacScheduler_methods(root_module, cls):
    ## ff-mac-scheduler.h (module 'lte'): ns3::FfMacScheduler::FfMacScheduler() [constructor]
    cls.add_constructor([])
    ## ff-mac-scheduler.h (module 'lte'): ns3::FfMacScheduler::FfMacScheduler(ns3::FfMacScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FfMacScheduler const &', 'arg0')])
    ## ff-mac-scheduler.h (module 'lte'): void ns3::FfMacScheduler::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ff-mac-scheduler.h (module 'lte'): ns3::FfMacCschedSapProvider * ns3::FfMacScheduler::GetFfMacCschedSapProvider() [member function]
    cls.add_method('GetFfMacCschedSapProvider', 
                   'ns3::FfMacCschedSapProvider *', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-scheduler.h (module 'lte'): ns3::FfMacSchedSapProvider * ns3::FfMacScheduler::GetFfMacSchedSapProvider() [member function]
    cls.add_method('GetFfMacSchedSapProvider', 
                   'ns3::FfMacSchedSapProvider *', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-scheduler.h (module 'lte'): static ns3::TypeId ns3::FfMacScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ff-mac-scheduler.h (module 'lte'): void ns3::FfMacScheduler::SetFfMacCschedSapUser(ns3::FfMacCschedSapUser * s) [member function]
    cls.add_method('SetFfMacCschedSapUser', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser *', 's')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ff-mac-scheduler.h (module 'lte'): void ns3::FfMacScheduler::SetFfMacSchedSapUser(ns3::FfMacSchedSapUser * s) [member function]
    cls.add_method('SetFfMacSchedSapUser', 
                   'void', 
                   [param('ns3::FfMacSchedSapUser *', 's')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3GammaRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::GammaRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::GammaRandomVariable::GammaRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::GammaRandomVariable::GetAlpha() const [member function]
    cls.add_method('GetAlpha', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::GammaRandomVariable::GetBeta() const [member function]
    cls.add_method('GetBeta', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::GammaRandomVariable::GetValue(double alpha, double beta) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'alpha'), param('double', 'beta')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::GammaRandomVariable::GetInteger(uint32_t alpha, uint32_t beta) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'alpha'), param('uint32_t', 'beta')])
    ## random-variable-stream.h (module 'core'): double ns3::GammaRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::GammaRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3GtpuHeader_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## epc-gtpu-header.h (module 'lte'): ns3::GtpuHeader::GtpuHeader(ns3::GtpuHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GtpuHeader const &', 'arg0')])
    ## epc-gtpu-header.h (module 'lte'): ns3::GtpuHeader::GtpuHeader() [constructor]
    cls.add_constructor([])
    ## epc-gtpu-header.h (module 'lte'): uint32_t ns3::GtpuHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## epc-gtpu-header.h (module 'lte'): bool ns3::GtpuHeader::GetExtensionHeaderFlag() const [member function]
    cls.add_method('GetExtensionHeaderFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): ns3::TypeId ns3::GtpuHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## epc-gtpu-header.h (module 'lte'): uint16_t ns3::GtpuHeader::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): uint8_t ns3::GtpuHeader::GetMessageType() const [member function]
    cls.add_method('GetMessageType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): uint8_t ns3::GtpuHeader::GetNPduNumber() const [member function]
    cls.add_method('GetNPduNumber', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): bool ns3::GtpuHeader::GetNPduNumberFlag() const [member function]
    cls.add_method('GetNPduNumberFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): uint8_t ns3::GtpuHeader::GetNextExtensionType() const [member function]
    cls.add_method('GetNextExtensionType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): bool ns3::GtpuHeader::GetProtocolType() const [member function]
    cls.add_method('GetProtocolType', 
                   'bool', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): uint16_t ns3::GtpuHeader::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): bool ns3::GtpuHeader::GetSequenceNumberFlag() const [member function]
    cls.add_method('GetSequenceNumberFlag', 
                   'bool', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): uint32_t ns3::GtpuHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## epc-gtpu-header.h (module 'lte'): uint32_t ns3::GtpuHeader::GetTeid() const [member function]
    cls.add_method('GetTeid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): static ns3::TypeId ns3::GtpuHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## epc-gtpu-header.h (module 'lte'): uint8_t ns3::GtpuHeader::GetVersion() const [member function]
    cls.add_method('GetVersion', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetExtensionHeaderFlag(bool m_extensionHeaderFlag) [member function]
    cls.add_method('SetExtensionHeaderFlag', 
                   'void', 
                   [param('bool', 'm_extensionHeaderFlag')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetLength(uint16_t m_length) [member function]
    cls.add_method('SetLength', 
                   'void', 
                   [param('uint16_t', 'm_length')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetMessageType(uint8_t m_messageType) [member function]
    cls.add_method('SetMessageType', 
                   'void', 
                   [param('uint8_t', 'm_messageType')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetNPduNumber(uint8_t m_nPduNumber) [member function]
    cls.add_method('SetNPduNumber', 
                   'void', 
                   [param('uint8_t', 'm_nPduNumber')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetNPduNumberFlag(bool m_nPduNumberFlag) [member function]
    cls.add_method('SetNPduNumberFlag', 
                   'void', 
                   [param('bool', 'm_nPduNumberFlag')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetNextExtensionType(uint8_t m_nextExtensionType) [member function]
    cls.add_method('SetNextExtensionType', 
                   'void', 
                   [param('uint8_t', 'm_nextExtensionType')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetProtocolType(bool m_protocolType) [member function]
    cls.add_method('SetProtocolType', 
                   'void', 
                   [param('bool', 'm_protocolType')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetSequenceNumber(uint16_t m_sequenceNumber) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'm_sequenceNumber')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetSequenceNumberFlag(bool m_sequenceNumberFlag) [member function]
    cls.add_method('SetSequenceNumberFlag', 
                   'void', 
                   [param('bool', 'm_sequenceNumberFlag')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetTeid(uint32_t m_teid) [member function]
    cls.add_method('SetTeid', 
                   'void', 
                   [param('uint32_t', 'm_teid')])
    ## epc-gtpu-header.h (module 'lte'): void ns3::GtpuHeader::SetVersion(uint8_t m_version) [member function]
    cls.add_method('SetVersion', 
                   'void', 
                   [param('uint8_t', 'm_version')])
    return

def register_Ns3IdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::IdealControlMessage::IdealControlMessage(ns3::IdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::IdealControlMessage::IdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): ns3::Ptr<ns3::LteNetDevice> ns3::IdealControlMessage::GetDestinationDevice() [member function]
    cls.add_method('GetDestinationDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## ideal-control-messages.h (module 'lte'): ns3::IdealControlMessage::MessageType ns3::IdealControlMessage::GetMessageType() [member function]
    cls.add_method('GetMessageType', 
                   'ns3::IdealControlMessage::MessageType', 
                   [])
    ## ideal-control-messages.h (module 'lte'): ns3::Ptr<ns3::LteNetDevice> ns3::IdealControlMessage::GetSourceDevice() [member function]
    cls.add_method('GetSourceDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## ideal-control-messages.h (module 'lte'): void ns3::IdealControlMessage::SetDestinationDevice(ns3::Ptr<ns3::LteNetDevice> dst) [member function]
    cls.add_method('SetDestinationDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'dst')])
    ## ideal-control-messages.h (module 'lte'): void ns3::IdealControlMessage::SetMessageType(ns3::IdealControlMessage::MessageType type) [member function]
    cls.add_method('SetMessageType', 
                   'void', 
                   [param('ns3::IdealControlMessage::MessageType', 'type')])
    ## ideal-control-messages.h (module 'lte'): void ns3::IdealControlMessage::SetSourceDevice(ns3::Ptr<ns3::LteNetDevice> src) [member function]
    cls.add_method('SetSourceDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'src')])
    return

def register_Ns3IntegerValue_methods(root_module, cls):
    ## integer.h (module 'core'): ns3::IntegerValue::IntegerValue() [constructor]
    cls.add_constructor([])
    ## integer.h (module 'core'): ns3::IntegerValue::IntegerValue(ns3::IntegerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntegerValue const &', 'arg0')])
    ## integer.h (module 'core'): ns3::IntegerValue::IntegerValue(int64_t const & value) [constructor]
    cls.add_constructor([param('int64_t const &', 'value')])
    ## integer.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::IntegerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## integer.h (module 'core'): bool ns3::IntegerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## integer.h (module 'core'): int64_t ns3::IntegerValue::Get() const [member function]
    cls.add_method('Get', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## integer.h (module 'core'): std::string ns3::IntegerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## integer.h (module 'core'): void ns3::IntegerValue::Set(int64_t const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('int64_t const &', 'value')])
    return

def register_Ns3IpL4Protocol_methods(root_module, cls):
    ## ip-l4-protocol.h (module 'internet'): ns3::IpL4Protocol::IpL4Protocol() [constructor]
    cls.add_constructor([])
    ## ip-l4-protocol.h (module 'internet'): ns3::IpL4Protocol::IpL4Protocol(ns3::IpL4Protocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IpL4Protocol const &', 'arg0')])
    ## ip-l4-protocol.h (module 'internet'): ns3::Callback<void,ns3::Ptr<ns3::Packet>,ns3::Ipv4Address,ns3::Ipv4Address,unsigned char,ns3::Ptr<ns3::Ipv4Route>,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ns3::IpL4Protocol::GetDownTarget() const [member function]
    cls.add_method('GetDownTarget', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ip-l4-protocol.h (module 'internet'): ns3::Callback<void,ns3::Ptr<ns3::Packet>,ns3::Ipv6Address,ns3::Ipv6Address,unsigned char,ns3::Ptr<ns3::Ipv6Route>,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ns3::IpL4Protocol::GetDownTarget6() const [member function]
    cls.add_method('GetDownTarget6', 
                   'ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv6Address, ns3::Ipv6Address, unsigned char, ns3::Ptr< ns3::Ipv6Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ip-l4-protocol.h (module 'internet'): int ns3::IpL4Protocol::GetProtocolNumber() const [member function]
    cls.add_method('GetProtocolNumber', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ip-l4-protocol.h (module 'internet'): static ns3::TypeId ns3::IpL4Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ip-l4-protocol.h (module 'internet'): ns3::IpL4Protocol::RxStatus ns3::IpL4Protocol::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ipv4Header const & header, ns3::Ptr<ns3::Ipv4Interface> incomingInterface) [member function]
    cls.add_method('Receive', 
                   'ns3::IpL4Protocol::RxStatus', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv4Header const &', 'header'), param('ns3::Ptr< ns3::Ipv4Interface >', 'incomingInterface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ip-l4-protocol.h (module 'internet'): ns3::IpL4Protocol::RxStatus ns3::IpL4Protocol::Receive(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Address & src, ns3::Ipv6Address & dst, ns3::Ptr<ns3::Ipv6Interface> incomingInterface) [member function]
    cls.add_method('Receive', 
                   'ns3::IpL4Protocol::RxStatus', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Address &', 'src'), param('ns3::Ipv6Address &', 'dst'), param('ns3::Ptr< ns3::Ipv6Interface >', 'incomingInterface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ip-l4-protocol.h (module 'internet'): void ns3::IpL4Protocol::ReceiveIcmp(ns3::Ipv4Address icmpSource, uint8_t icmpTtl, uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo, ns3::Ipv4Address payloadSource, ns3::Ipv4Address payloadDestination, uint8_t const * payload) [member function]
    cls.add_method('ReceiveIcmp', 
                   'void', 
                   [param('ns3::Ipv4Address', 'icmpSource'), param('uint8_t', 'icmpTtl'), param('uint8_t', 'icmpType'), param('uint8_t', 'icmpCode'), param('uint32_t', 'icmpInfo'), param('ns3::Ipv4Address', 'payloadSource'), param('ns3::Ipv4Address', 'payloadDestination'), param('uint8_t const *', 'payload')], 
                   is_virtual=True)
    ## ip-l4-protocol.h (module 'internet'): void ns3::IpL4Protocol::ReceiveIcmp(ns3::Ipv6Address icmpSource, uint8_t icmpTtl, uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo, ns3::Ipv6Address payloadSource, ns3::Ipv6Address payloadDestination, uint8_t const * payload) [member function]
    cls.add_method('ReceiveIcmp', 
                   'void', 
                   [param('ns3::Ipv6Address', 'icmpSource'), param('uint8_t', 'icmpTtl'), param('uint8_t', 'icmpType'), param('uint8_t', 'icmpCode'), param('uint32_t', 'icmpInfo'), param('ns3::Ipv6Address', 'payloadSource'), param('ns3::Ipv6Address', 'payloadDestination'), param('uint8_t const *', 'payload')], 
                   is_virtual=True)
    ## ip-l4-protocol.h (module 'internet'): void ns3::IpL4Protocol::SetDownTarget(ns3::Callback<void,ns3::Ptr<ns3::Packet>,ns3::Ipv4Address,ns3::Ipv4Address,unsigned char,ns3::Ptr<ns3::Ipv4Route>,ns3::empty,ns3::empty,ns3::empty,ns3::empty> cb) [member function]
    cls.add_method('SetDownTarget', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv4Address, ns3::Ipv4Address, unsigned char, ns3::Ptr< ns3::Ipv4Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ip-l4-protocol.h (module 'internet'): void ns3::IpL4Protocol::SetDownTarget6(ns3::Callback<void,ns3::Ptr<ns3::Packet>,ns3::Ipv6Address,ns3::Ipv6Address,unsigned char,ns3::Ptr<ns3::Ipv6Route>,ns3::empty,ns3::empty,ns3::empty,ns3::empty> cb) [member function]
    cls.add_method('SetDownTarget6', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ipv6Address, ns3::Ipv6Address, unsigned char, ns3::Ptr< ns3::Ipv6Route >, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3Ipv4_methods(root_module, cls):
    ## ipv4.h (module 'internet'): ns3::Ipv4::Ipv4(ns3::Ipv4 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4 const &', 'arg0')])
    ## ipv4.h (module 'internet'): ns3::Ipv4::Ipv4() [constructor]
    cls.add_constructor([])
    ## ipv4.h (module 'internet'): bool ns3::Ipv4::AddAddress(uint32_t interface, ns3::Ipv4InterfaceAddress address) [member function]
    cls.add_method('AddAddress', 
                   'bool', 
                   [param('uint32_t', 'interface'), param('ns3::Ipv4InterfaceAddress', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): uint32_t ns3::Ipv4::AddInterface(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddInterface', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): ns3::Ptr<ns3::Socket> ns3::Ipv4::CreateRawSocket() [member function]
    cls.add_method('CreateRawSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::DeleteRawSocket(ns3::Ptr<ns3::Socket> socket) [member function]
    cls.add_method('DeleteRawSocket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): ns3::Ipv4InterfaceAddress ns3::Ipv4::GetAddress(uint32_t interface, uint32_t addressIndex) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv4InterfaceAddress', 
                   [param('uint32_t', 'interface'), param('uint32_t', 'addressIndex')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): int32_t ns3::Ipv4::GetInterfaceForAddress(ns3::Ipv4Address address) const [member function]
    cls.add_method('GetInterfaceForAddress', 
                   'int32_t', 
                   [param('ns3::Ipv4Address', 'address')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): int32_t ns3::Ipv4::GetInterfaceForDevice(ns3::Ptr<const ns3::NetDevice> device) const [member function]
    cls.add_method('GetInterfaceForDevice', 
                   'int32_t', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): int32_t ns3::Ipv4::GetInterfaceForPrefix(ns3::Ipv4Address address, ns3::Ipv4Mask mask) const [member function]
    cls.add_method('GetInterfaceForPrefix', 
                   'int32_t', 
                   [param('ns3::Ipv4Address', 'address'), param('ns3::Ipv4Mask', 'mask')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): uint16_t ns3::Ipv4::GetMetric(uint32_t interface) const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): uint16_t ns3::Ipv4::GetMtu(uint32_t interface) const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): uint32_t ns3::Ipv4::GetNAddresses(uint32_t interface) const [member function]
    cls.add_method('GetNAddresses', 
                   'uint32_t', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): uint32_t ns3::Ipv4::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): ns3::Ptr<ns3::NetDevice> ns3::Ipv4::GetNetDevice(uint32_t interface) [member function]
    cls.add_method('GetNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): ns3::Ptr<ns3::IpL4Protocol> ns3::Ipv4::GetProtocol(int protocolNumber) const [member function]
    cls.add_method('GetProtocol', 
                   'ns3::Ptr< ns3::IpL4Protocol >', 
                   [param('int', 'protocolNumber')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): ns3::Ptr<ns3::Ipv4RoutingProtocol> ns3::Ipv4::GetRoutingProtocol() const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::Ipv4RoutingProtocol >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): static ns3::TypeId ns3::Ipv4::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::Insert(ns3::Ptr<ns3::IpL4Protocol> protocol) [member function]
    cls.add_method('Insert', 
                   'void', 
                   [param('ns3::Ptr< ns3::IpL4Protocol >', 'protocol')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): bool ns3::Ipv4::IsDestinationAddress(ns3::Ipv4Address address, uint32_t iif) const [member function]
    cls.add_method('IsDestinationAddress', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'address'), param('uint32_t', 'iif')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): bool ns3::Ipv4::IsForwarding(uint32_t interface) const [member function]
    cls.add_method('IsForwarding', 
                   'bool', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): bool ns3::Ipv4::IsUp(uint32_t interface) const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ipv4.h (module 'internet'): bool ns3::Ipv4::RemoveAddress(uint32_t interface, uint32_t addressIndex) [member function]
    cls.add_method('RemoveAddress', 
                   'bool', 
                   [param('uint32_t', 'interface'), param('uint32_t', 'addressIndex')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4::SelectSourceAddress(ns3::Ptr<const ns3::NetDevice> device, ns3::Ipv4Address dst, ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e scope) [member function]
    cls.add_method('SelectSourceAddress', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ptr< ns3::NetDevice const >', 'device'), param('ns3::Ipv4Address', 'dst'), param('ns3::Ipv4InterfaceAddress::InterfaceAddressScope_e', 'scope')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::Send(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Address source, ns3::Ipv4Address destination, uint8_t protocol, ns3::Ptr<ns3::Ipv4Route> route) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Address', 'source'), param('ns3::Ipv4Address', 'destination'), param('uint8_t', 'protocol'), param('ns3::Ptr< ns3::Ipv4Route >', 'route')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::SendWithHeader(ns3::Ptr<ns3::Packet> packet, ns3::Ipv4Header ipHeader, ns3::Ptr<ns3::Ipv4Route> route) [member function]
    cls.add_method('SendWithHeader', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Ipv4Header', 'ipHeader'), param('ns3::Ptr< ns3::Ipv4Route >', 'route')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::SetDown(uint32_t interface) [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::SetForwarding(uint32_t interface, bool val) [member function]
    cls.add_method('SetForwarding', 
                   'void', 
                   [param('uint32_t', 'interface'), param('bool', 'val')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::SetMetric(uint32_t interface, uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'interface'), param('uint16_t', 'metric')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::SetRoutingProtocol(ns3::Ptr<ns3::Ipv4RoutingProtocol> routingProtocol) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ipv4RoutingProtocol >', 'routingProtocol')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::SetUp(uint32_t interface) [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ipv4.h (module 'internet'): ns3::Ipv4::IF_ANY [variable]
    cls.add_static_attribute('IF_ANY', 'uint32_t const', is_const=True)
    ## ipv4.h (module 'internet'): bool ns3::Ipv4::GetIpForward() const [member function]
    cls.add_method('GetIpForward', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## ipv4.h (module 'internet'): bool ns3::Ipv4::GetWeakEsModel() const [member function]
    cls.add_method('GetWeakEsModel', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::SetIpForward(bool forward) [member function]
    cls.add_method('SetIpForward', 
                   'void', 
                   [param('bool', 'forward')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## ipv4.h (module 'internet'): void ns3::Ipv4::SetWeakEsModel(bool model) [member function]
    cls.add_method('SetWeakEsModel', 
                   'void', 
                   [param('bool', 'model')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3Ipv4AddressChecker_methods(root_module, cls):
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressChecker::Ipv4AddressChecker() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressChecker::Ipv4AddressChecker(ns3::Ipv4AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressChecker const &', 'arg0')])
    return

def register_Ns3Ipv4AddressValue_methods(root_module, cls):
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue::Ipv4AddressValue() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue::Ipv4AddressValue(ns3::Ipv4AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressValue const &', 'arg0')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue::Ipv4AddressValue(ns3::Ipv4Address const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address const &', 'value')])
    ## ipv4-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Ipv4AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address ns3::Ipv4AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): std::string ns3::Ipv4AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4AddressValue::Set(ns3::Ipv4Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'value')])
    return

def register_Ns3Ipv4MaskChecker_methods(root_module, cls):
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskChecker::Ipv4MaskChecker() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskChecker::Ipv4MaskChecker(ns3::Ipv4MaskChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MaskChecker const &', 'arg0')])
    return

def register_Ns3Ipv4MaskValue_methods(root_module, cls):
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue::Ipv4MaskValue() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue::Ipv4MaskValue(ns3::Ipv4MaskValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MaskValue const &', 'arg0')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue::Ipv4MaskValue(ns3::Ipv4Mask const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv4Mask const &', 'value')])
    ## ipv4-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Ipv4MaskValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4MaskValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask ns3::Ipv4MaskValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): std::string ns3::Ipv4MaskValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4MaskValue::Set(ns3::Ipv4Mask const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv4Mask const &', 'value')])
    return

def register_Ns3Ipv4MulticastRoute_methods(root_module, cls):
    ## ipv4-route.h (module 'internet'): ns3::Ipv4MulticastRoute::Ipv4MulticastRoute(ns3::Ipv4MulticastRoute const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MulticastRoute const &', 'arg0')])
    ## ipv4-route.h (module 'internet'): ns3::Ipv4MulticastRoute::Ipv4MulticastRoute() [constructor]
    cls.add_constructor([])
    ## ipv4-route.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4MulticastRoute::GetGroup() const [member function]
    cls.add_method('GetGroup', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4MulticastRoute::GetOrigin() const [member function]
    cls.add_method('GetOrigin', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h (module 'internet'): uint32_t ns3::Ipv4MulticastRoute::GetOutputTtl(uint32_t oif) [member function]
    cls.add_method('GetOutputTtl', 
                   'uint32_t', 
                   [param('uint32_t', 'oif')], 
                   deprecated=True)
    ## ipv4-route.h (module 'internet'): std::map<unsigned int, unsigned int, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, unsigned int> > > ns3::Ipv4MulticastRoute::GetOutputTtlMap() const [member function]
    cls.add_method('GetOutputTtlMap', 
                   'std::map< unsigned int, unsigned int >', 
                   [], 
                   is_const=True)
    ## ipv4-route.h (module 'internet'): uint32_t ns3::Ipv4MulticastRoute::GetParent() const [member function]
    cls.add_method('GetParent', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-route.h (module 'internet'): void ns3::Ipv4MulticastRoute::SetGroup(ns3::Ipv4Address const group) [member function]
    cls.add_method('SetGroup', 
                   'void', 
                   [param('ns3::Ipv4Address const', 'group')])
    ## ipv4-route.h (module 'internet'): void ns3::Ipv4MulticastRoute::SetOrigin(ns3::Ipv4Address const origin) [member function]
    cls.add_method('SetOrigin', 
                   'void', 
                   [param('ns3::Ipv4Address const', 'origin')])
    ## ipv4-route.h (module 'internet'): void ns3::Ipv4MulticastRoute::SetOutputTtl(uint32_t oif, uint32_t ttl) [member function]
    cls.add_method('SetOutputTtl', 
                   'void', 
                   [param('uint32_t', 'oif'), param('uint32_t', 'ttl')])
    ## ipv4-route.h (module 'internet'): void ns3::Ipv4MulticastRoute::SetParent(uint32_t iif) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('uint32_t', 'iif')])
    ## ipv4-route.h (module 'internet'): ns3::Ipv4MulticastRoute::MAX_INTERFACES [variable]
    cls.add_static_attribute('MAX_INTERFACES', 'uint32_t const', is_const=True)
    ## ipv4-route.h (module 'internet'): ns3::Ipv4MulticastRoute::MAX_TTL [variable]
    cls.add_static_attribute('MAX_TTL', 'uint32_t const', is_const=True)
    return

def register_Ns3Ipv4Route_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ipv4-route.h (module 'internet'): ns3::Ipv4Route::Ipv4Route(ns3::Ipv4Route const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Route const &', 'arg0')])
    ## ipv4-route.h (module 'internet'): ns3::Ipv4Route::Ipv4Route() [constructor]
    cls.add_constructor([])
    ## ipv4-route.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4Route::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4Route::GetGateway() const [member function]
    cls.add_method('GetGateway', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h (module 'internet'): ns3::Ptr<ns3::NetDevice> ns3::Ipv4Route::GetOutputDevice() const [member function]
    cls.add_method('GetOutputDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## ipv4-route.h (module 'internet'): ns3::Ipv4Address ns3::Ipv4Route::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-route.h (module 'internet'): void ns3::Ipv4Route::SetDestination(ns3::Ipv4Address dest) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dest')])
    ## ipv4-route.h (module 'internet'): void ns3::Ipv4Route::SetGateway(ns3::Ipv4Address gw) [member function]
    cls.add_method('SetGateway', 
                   'void', 
                   [param('ns3::Ipv4Address', 'gw')])
    ## ipv4-route.h (module 'internet'): void ns3::Ipv4Route::SetOutputDevice(ns3::Ptr<ns3::NetDevice> outputDevice) [member function]
    cls.add_method('SetOutputDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'outputDevice')])
    ## ipv4-route.h (module 'internet'): void ns3::Ipv4Route::SetSource(ns3::Ipv4Address src) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Ipv4Address', 'src')])
    return

def register_Ns3Ipv6AddressChecker_methods(root_module, cls):
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressChecker::Ipv6AddressChecker() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressChecker::Ipv6AddressChecker(ns3::Ipv6AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6AddressChecker const &', 'arg0')])
    return

def register_Ns3Ipv6AddressValue_methods(root_module, cls):
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue::Ipv6AddressValue() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue::Ipv6AddressValue(ns3::Ipv6AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6AddressValue const &', 'arg0')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue::Ipv6AddressValue(ns3::Ipv6Address const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address const &', 'value')])
    ## ipv6-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Ipv6AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address ns3::Ipv6AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): std::string ns3::Ipv6AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6AddressValue::Set(ns3::Ipv6Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv6Address const &', 'value')])
    return

def register_Ns3Ipv6Interface_methods(root_module, cls):
    ## ipv6-interface.h (module 'internet'): ns3::Ipv6Interface::Ipv6Interface(ns3::Ipv6Interface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Interface const &', 'arg0')])
    ## ipv6-interface.h (module 'internet'): ns3::Ipv6Interface::Ipv6Interface() [constructor]
    cls.add_constructor([])
    ## ipv6-interface.h (module 'internet'): bool ns3::Ipv6Interface::AddAddress(ns3::Ipv6InterfaceAddress iface) [member function]
    cls.add_method('AddAddress', 
                   'bool', 
                   [param('ns3::Ipv6InterfaceAddress', 'iface')])
    ## ipv6-interface.h (module 'internet'): ns3::Ipv6InterfaceAddress ns3::Ipv6Interface::GetAddress(uint32_t index) const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Ipv6InterfaceAddress', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): ns3::Ipv6InterfaceAddress ns3::Ipv6Interface::GetAddressMatchingDestination(ns3::Ipv6Address dst) [member function]
    cls.add_method('GetAddressMatchingDestination', 
                   'ns3::Ipv6InterfaceAddress', 
                   [param('ns3::Ipv6Address', 'dst')])
    ## ipv6-interface.h (module 'internet'): uint16_t ns3::Ipv6Interface::GetBaseReachableTime() const [member function]
    cls.add_method('GetBaseReachableTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): uint8_t ns3::Ipv6Interface::GetCurHopLimit() const [member function]
    cls.add_method('GetCurHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): ns3::Ptr<ns3::NetDevice> ns3::Ipv6Interface::GetDevice() const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-interface.h (module 'internet'): ns3::Ipv6InterfaceAddress ns3::Ipv6Interface::GetLinkLocalAddress() const [member function]
    cls.add_method('GetLinkLocalAddress', 
                   'ns3::Ipv6InterfaceAddress', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): uint16_t ns3::Ipv6Interface::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): uint32_t ns3::Ipv6Interface::GetNAddresses() const [member function]
    cls.add_method('GetNAddresses', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): uint16_t ns3::Ipv6Interface::GetReachableTime() const [member function]
    cls.add_method('GetReachableTime', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): uint16_t ns3::Ipv6Interface::GetRetransTimer() const [member function]
    cls.add_method('GetRetransTimer', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): static ns3::TypeId ns3::Ipv6Interface::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ipv6-interface.h (module 'internet'): bool ns3::Ipv6Interface::IsDown() const [member function]
    cls.add_method('IsDown', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): bool ns3::Ipv6Interface::IsForwarding() const [member function]
    cls.add_method('IsForwarding', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): bool ns3::Ipv6Interface::IsUp() const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-interface.h (module 'internet'): ns3::Ipv6InterfaceAddress ns3::Ipv6Interface::RemoveAddress(uint32_t index) [member function]
    cls.add_method('RemoveAddress', 
                   'ns3::Ipv6InterfaceAddress', 
                   [param('uint32_t', 'index')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::Send(ns3::Ptr<ns3::Packet> p, ns3::Ipv6Address dest) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ipv6Address', 'dest')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetBaseReachableTime(uint16_t baseReachableTime) [member function]
    cls.add_method('SetBaseReachableTime', 
                   'void', 
                   [param('uint16_t', 'baseReachableTime')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetCurHopLimit(uint8_t curHopLimit) [member function]
    cls.add_method('SetCurHopLimit', 
                   'void', 
                   [param('uint8_t', 'curHopLimit')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetDown() [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetForwarding(bool forward) [member function]
    cls.add_method('SetForwarding', 
                   'void', 
                   [param('bool', 'forward')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetMetric(uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint16_t', 'metric')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetNsDadUid(ns3::Ipv6Address address, uint32_t uid) [member function]
    cls.add_method('SetNsDadUid', 
                   'void', 
                   [param('ns3::Ipv6Address', 'address'), param('uint32_t', 'uid')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetReachableTime(uint16_t reachableTime) [member function]
    cls.add_method('SetReachableTime', 
                   'void', 
                   [param('uint16_t', 'reachableTime')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetRetransTimer(uint16_t retransTimer) [member function]
    cls.add_method('SetRetransTimer', 
                   'void', 
                   [param('uint16_t', 'retransTimer')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetState(ns3::Ipv6Address address, ns3::Ipv6InterfaceAddress::State_e state) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::Ipv6Address', 'address'), param('ns3::Ipv6InterfaceAddress::State_e', 'state')])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::SetUp() [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [])
    ## ipv6-interface.h (module 'internet'): void ns3::Ipv6Interface::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3Ipv6PrefixChecker_methods(root_module, cls):
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixChecker::Ipv6PrefixChecker() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixChecker::Ipv6PrefixChecker(ns3::Ipv6PrefixChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PrefixChecker const &', 'arg0')])
    return

def register_Ns3Ipv6PrefixValue_methods(root_module, cls):
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue::Ipv6PrefixValue() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue::Ipv6PrefixValue(ns3::Ipv6PrefixValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PrefixValue const &', 'arg0')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue::Ipv6PrefixValue(ns3::Ipv6Prefix const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const &', 'value')])
    ## ipv6-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Ipv6PrefixValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6PrefixValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix ns3::Ipv6PrefixValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): std::string ns3::Ipv6PrefixValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6PrefixValue::Set(ns3::Ipv6Prefix const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv6Prefix const &', 'value')])
    return

def register_Ns3LogNormalRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::LogNormalRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::LogNormalRandomVariable::LogNormalRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::LogNormalRandomVariable::GetMu() const [member function]
    cls.add_method('GetMu', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::LogNormalRandomVariable::GetSigma() const [member function]
    cls.add_method('GetSigma', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::LogNormalRandomVariable::GetValue(double mu, double sigma) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'mu'), param('double', 'sigma')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::LogNormalRandomVariable::GetInteger(uint32_t mu, uint32_t sigma) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'mu'), param('uint32_t', 'sigma')])
    ## random-variable-stream.h (module 'core'): double ns3::LogNormalRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::LogNormalRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3LteAmc_methods(root_module, cls):
    ## lte-amc.h (module 'lte'): ns3::LteAmc::LteAmc(ns3::LteAmc const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteAmc const &', 'arg0')])
    ## lte-amc.h (module 'lte'): ns3::LteAmc::LteAmc() [constructor]
    cls.add_constructor([])
    ## lte-amc.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::LteAmc::CreateCqiFeedbacks(ns3::SpectrumValue const & sinr, uint8_t rbgSize=0) [member function]
    cls.add_method('CreateCqiFeedbacks', 
                   'std::vector< int >', 
                   [param('ns3::SpectrumValue const &', 'sinr'), param('uint8_t', 'rbgSize', default_value='0')])
    ## lte-amc.h (module 'lte'): int ns3::LteAmc::GetCqiFromSpectralEfficiency(double s) [member function]
    cls.add_method('GetCqiFromSpectralEfficiency', 
                   'int', 
                   [param('double', 's')])
    ## lte-amc.h (module 'lte'): int ns3::LteAmc::GetMcsFromCqi(int cqi) [member function]
    cls.add_method('GetMcsFromCqi', 
                   'int', 
                   [param('int', 'cqi')])
    ## lte-amc.h (module 'lte'): double ns3::LteAmc::GetSpectralEfficiencyFromCqi(int cqi) [member function]
    cls.add_method('GetSpectralEfficiencyFromCqi', 
                   'double', 
                   [param('int', 'cqi')])
    ## lte-amc.h (module 'lte'): int ns3::LteAmc::GetTbSizeFromMcs(int mcs, int nprb) [member function]
    cls.add_method('GetTbSizeFromMcs', 
                   'int', 
                   [param('int', 'mcs'), param('int', 'nprb')])
    ## lte-amc.h (module 'lte'): static ns3::TypeId ns3::LteAmc::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3LteEnbMac_methods(root_module, cls):
    ## lte-enb-mac.h (module 'lte'): ns3::LteEnbMac::LteEnbMac(ns3::LteEnbMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteEnbMac const &', 'arg0')])
    ## lte-enb-mac.h (module 'lte'): ns3::LteEnbMac::LteEnbMac() [constructor]
    cls.add_constructor([])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::DoReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('DoReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::DoReceivePhyPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceivePhyPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::DoUlCqiReport(UlCqi_s ulcqi) [member function]
    cls.add_method('DoUlCqiReport', 
                   'void', 
                   [param('UlCqi_s', 'ulcqi')])
    ## lte-enb-mac.h (module 'lte'): ns3::FfMacCschedSapUser * ns3::LteEnbMac::GetFfMacCschedSapUser() [member function]
    cls.add_method('GetFfMacCschedSapUser', 
                   'ns3::FfMacCschedSapUser *', 
                   [])
    ## lte-enb-mac.h (module 'lte'): ns3::FfMacSchedSapUser * ns3::LteEnbMac::GetFfMacSchedSapUser() [member function]
    cls.add_method('GetFfMacSchedSapUser', 
                   'ns3::FfMacSchedSapUser *', 
                   [])
    ## lte-enb-mac.h (module 'lte'): ns3::LteEnbCmacSapProvider * ns3::LteEnbMac::GetLteEnbCmacSapProvider() [member function]
    cls.add_method('GetLteEnbCmacSapProvider', 
                   'ns3::LteEnbCmacSapProvider *', 
                   [])
    ## lte-enb-mac.h (module 'lte'): ns3::LteEnbPhySapUser * ns3::LteEnbMac::GetLteEnbPhySapUser() [member function]
    cls.add_method('GetLteEnbPhySapUser', 
                   'ns3::LteEnbPhySapUser *', 
                   [])
    ## lte-enb-mac.h (module 'lte'): ns3::LteMacSapProvider * ns3::LteEnbMac::GetLteMacSapProvider() [member function]
    cls.add_method('GetLteMacSapProvider', 
                   'ns3::LteMacSapProvider *', 
                   [])
    ## lte-enb-mac.h (module 'lte'): static ns3::TypeId ns3::LteEnbMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::ReceiveBsrMessage(MacCeListElement_s bsr) [member function]
    cls.add_method('ReceiveBsrMessage', 
                   'void', 
                   [param('MacCeListElement_s', 'bsr')])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::ReceiveDlCqiIdealControlMessage(ns3::Ptr<ns3::DlCqiIdealControlMessage> msg) [member function]
    cls.add_method('ReceiveDlCqiIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::DlCqiIdealControlMessage >', 'msg')])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::SetFfMacCschedSapProvider(ns3::FfMacCschedSapProvider * s) [member function]
    cls.add_method('SetFfMacCschedSapProvider', 
                   'void', 
                   [param('ns3::FfMacCschedSapProvider *', 's')])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::SetFfMacSchedSapProvider(ns3::FfMacSchedSapProvider * s) [member function]
    cls.add_method('SetFfMacSchedSapProvider', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider *', 's')])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::SetLteEnbCmacSapUser(ns3::LteEnbCmacSapUser * s) [member function]
    cls.add_method('SetLteEnbCmacSapUser', 
                   'void', 
                   [param('ns3::LteEnbCmacSapUser *', 's')])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::SetLteEnbPhySapProvider(ns3::LteEnbPhySapProvider * s) [member function]
    cls.add_method('SetLteEnbPhySapProvider', 
                   'void', 
                   [param('ns3::LteEnbPhySapProvider *', 's')])
    ## lte-enb-mac.h (module 'lte'): void ns3::LteEnbMac::SetLteMacSapUser(ns3::LteMacSapUser * s) [member function]
    cls.add_method('SetLteMacSapUser', 
                   'void', 
                   [param('ns3::LteMacSapUser *', 's')])
    return

def register_Ns3LteEnbRrc_methods(root_module, cls):
    ## lte-enb-rrc.h (module 'lte'): ns3::LteEnbRrc::LteEnbRrc(ns3::LteEnbRrc const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteEnbRrc const &', 'arg0')])
    ## lte-enb-rrc.h (module 'lte'): ns3::LteEnbRrc::LteEnbRrc() [constructor]
    cls.add_constructor([])
    ## lte-enb-rrc.h (module 'lte'): uint16_t ns3::LteEnbRrc::AddUe(uint64_t imsi) [member function]
    cls.add_method('AddUe', 
                   'uint16_t', 
                   [param('uint64_t', 'imsi')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::ConfigureCell(uint8_t ulBandwidth, uint8_t dlBandwidth) [member function]
    cls.add_method('ConfigureCell', 
                   'void', 
                   [param('uint8_t', 'ulBandwidth'), param('uint8_t', 'dlBandwidth')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-enb-rrc.h (module 'lte'): uint16_t ns3::LteEnbRrc::GetLastAllocatedRnti() const [member function]
    cls.add_method('GetLastAllocatedRnti', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-enb-rrc.h (module 'lte'): ns3::LteEnbCmacSapUser * ns3::LteEnbRrc::GetLteEnbCmacSapUser() [member function]
    cls.add_method('GetLteEnbCmacSapUser', 
                   'ns3::LteEnbCmacSapUser *', 
                   [])
    ## lte-enb-rrc.h (module 'lte'): static ns3::TypeId ns3::LteEnbRrc::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-enb-rrc.h (module 'lte'): std::map<unsigned short, ns3::Ptr<ns3::UeInfo>, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, ns3::Ptr<ns3::UeInfo> > > > ns3::LteEnbRrc::GetUeMap() const [member function]
    cls.add_method('GetUeMap', 
                   'std::map< unsigned short, ns3::Ptr< ns3::UeInfo > >', 
                   [], 
                   is_const=True)
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::ReleaseRadioBearer(uint16_t rnti, uint8_t lcId) [member function]
    cls.add_method('ReleaseRadioBearer', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint8_t', 'lcId')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::RemoveUe(uint16_t rnti) [member function]
    cls.add_method('RemoveUe', 
                   'void', 
                   [param('uint16_t', 'rnti')])
    ## lte-enb-rrc.h (module 'lte'): bool ns3::LteEnbRrc::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::SetFfMacSchedSapProvider(ns3::FfMacSchedSapProvider * s) [member function]
    cls.add_method('SetFfMacSchedSapProvider', 
                   'void', 
                   [param('ns3::FfMacSchedSapProvider *', 's')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::SetLastAllocatedRnti(uint16_t lastAllocatedRnti) [member function]
    cls.add_method('SetLastAllocatedRnti', 
                   'void', 
                   [param('uint16_t', 'lastAllocatedRnti')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::SetLteEnbCmacSapProvider(ns3::LteEnbCmacSapProvider * s) [member function]
    cls.add_method('SetLteEnbCmacSapProvider', 
                   'void', 
                   [param('ns3::LteEnbCmacSapProvider *', 's')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::SetLteMacSapProvider(ns3::LteMacSapProvider * s) [member function]
    cls.add_method('SetLteMacSapProvider', 
                   'void', 
                   [param('ns3::LteMacSapProvider *', 's')])
    ## lte-enb-rrc.h (module 'lte'): void ns3::LteEnbRrc::SetUeMap(std::map<unsigned short, ns3::Ptr<ns3::UeInfo>, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, ns3::Ptr<ns3::UeInfo> > > > ueMap) [member function]
    cls.add_method('SetUeMap', 
                   'void', 
                   [param('std::map< unsigned short, ns3::Ptr< ns3::UeInfo > >', 'ueMap')])
    ## lte-enb-rrc.h (module 'lte'): uint8_t ns3::LteEnbRrc::SetupRadioBearer(uint16_t rnti, ns3::EpsBearer bearer, ns3::TypeId rlcTypeId) [member function]
    cls.add_method('SetupRadioBearer', 
                   'uint8_t', 
                   [param('uint16_t', 'rnti'), param('ns3::EpsBearer', 'bearer'), param('ns3::TypeId', 'rlcTypeId')])
    return

def register_Ns3LteHelper_methods(root_module, cls):
    ## lte-helper.h (module 'lte'): ns3::LteHelper::LteHelper(ns3::LteHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteHelper const &', 'arg0')])
    ## lte-helper.h (module 'lte'): ns3::LteHelper::LteHelper() [constructor]
    cls.add_constructor([])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::ActivateEpsBearer(ns3::NetDeviceContainer ueDevices, ns3::EpsBearer bearer, ns3::Ptr<ns3::EpcTft> tft) [member function]
    cls.add_method('ActivateEpsBearer', 
                   'void', 
                   [param('ns3::NetDeviceContainer', 'ueDevices'), param('ns3::EpsBearer', 'bearer'), param('ns3::Ptr< ns3::EpcTft >', 'tft')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::ActivateEpsBearer(ns3::Ptr<ns3::NetDevice> ueDevice, ns3::EpsBearer bearer, ns3::Ptr<ns3::EpcTft> tft) [member function]
    cls.add_method('ActivateEpsBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'ueDevice'), param('ns3::EpsBearer', 'bearer'), param('ns3::Ptr< ns3::EpcTft >', 'tft')])
    ## lte-helper.h (module 'lte'): int64_t ns3::LteHelper::AssignStreams(ns3::NetDeviceContainer c, int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('ns3::NetDeviceContainer', 'c'), param('int64_t', 'stream')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::Attach(ns3::NetDeviceContainer ueDevices, ns3::Ptr<ns3::NetDevice> enbDevice) [member function]
    cls.add_method('Attach', 
                   'void', 
                   [param('ns3::NetDeviceContainer', 'ueDevices'), param('ns3::Ptr< ns3::NetDevice >', 'enbDevice')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::Attach(ns3::Ptr<ns3::NetDevice> ueDevice, ns3::Ptr<ns3::NetDevice> enbDevice) [member function]
    cls.add_method('Attach', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'ueDevice'), param('ns3::Ptr< ns3::NetDevice >', 'enbDevice')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::AttachToClosestEnb(ns3::NetDeviceContainer ueDevices, ns3::NetDeviceContainer enbDevices) [member function]
    cls.add_method('AttachToClosestEnb', 
                   'void', 
                   [param('ns3::NetDeviceContainer', 'ueDevices'), param('ns3::NetDeviceContainer', 'enbDevices')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::AttachToClosestEnb(ns3::Ptr<ns3::NetDevice> ueDevice, ns3::NetDeviceContainer enbDevices) [member function]
    cls.add_method('AttachToClosestEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'ueDevice'), param('ns3::NetDeviceContainer', 'enbDevices')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableDlMacTraces() [member function]
    cls.add_method('EnableDlMacTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableDlPdcpTraces() [member function]
    cls.add_method('EnableDlPdcpTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableDlRlcTraces() [member function]
    cls.add_method('EnableDlRlcTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableLogComponents() [member function]
    cls.add_method('EnableLogComponents', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableMacTraces() [member function]
    cls.add_method('EnableMacTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnablePdcpTraces() [member function]
    cls.add_method('EnablePdcpTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableRlcTraces() [member function]
    cls.add_method('EnableRlcTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableTraces() [member function]
    cls.add_method('EnableTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableUlMacTraces() [member function]
    cls.add_method('EnableUlMacTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableUlPdcpTraces() [member function]
    cls.add_method('EnableUlPdcpTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableUlRlcTraces() [member function]
    cls.add_method('EnableUlRlcTraces', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): ns3::Ptr<ns3::RadioBearerStatsCalculator> ns3::LteHelper::GetPdcpStats() [member function]
    cls.add_method('GetPdcpStats', 
                   'ns3::Ptr< ns3::RadioBearerStatsCalculator >', 
                   [])
    ## lte-helper.h (module 'lte'): ns3::Ptr<ns3::RadioBearerStatsCalculator> ns3::LteHelper::GetRlcStats() [member function]
    cls.add_method('GetRlcStats', 
                   'ns3::Ptr< ns3::RadioBearerStatsCalculator >', 
                   [])
    ## lte-helper.h (module 'lte'): ns3::TypeId ns3::LteHelper::GetRlcType(ns3::EpsBearer bearer) [member function]
    cls.add_method('GetRlcType', 
                   'ns3::TypeId', 
                   [param('ns3::EpsBearer', 'bearer')])
    ## lte-helper.h (module 'lte'): static ns3::TypeId ns3::LteHelper::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-helper.h (module 'lte'): ns3::NetDeviceContainer ns3::LteHelper::InstallEnbDevice(ns3::NodeContainer c) [member function]
    cls.add_method('InstallEnbDevice', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## lte-helper.h (module 'lte'): ns3::NetDeviceContainer ns3::LteHelper::InstallUeDevice(ns3::NodeContainer c) [member function]
    cls.add_method('InstallUeDevice', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetEnbAntennaModelAttribute(std::string n, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetEnbAntennaModelAttribute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::AttributeValue const &', 'v')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetEnbAntennaModelType(std::string type) [member function]
    cls.add_method('SetEnbAntennaModelType', 
                   'void', 
                   [param('std::string', 'type')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetEnbDeviceAttribute(std::string n, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetEnbDeviceAttribute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::AttributeValue const &', 'v')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetEpcHelper(ns3::Ptr<ns3::EpcHelper> h) [member function]
    cls.add_method('SetEpcHelper', 
                   'void', 
                   [param('ns3::Ptr< ns3::EpcHelper >', 'h')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetFadingModel(std::string model) [member function]
    cls.add_method('SetFadingModel', 
                   'void', 
                   [param('std::string', 'model')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetFadingModelAttribute(std::string n, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetFadingModelAttribute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::AttributeValue const &', 'v')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetPathlossModelAttribute(std::string n, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetPathlossModelAttribute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::AttributeValue const &', 'v')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetPathlossModelType(std::string type) [member function]
    cls.add_method('SetPathlossModelType', 
                   'void', 
                   [param('std::string', 'type')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetSchedulerAttribute(std::string n, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetSchedulerAttribute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::AttributeValue const &', 'v')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetSchedulerType(std::string type) [member function]
    cls.add_method('SetSchedulerType', 
                   'void', 
                   [param('std::string', 'type')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetSpectrumChannelAttribute(std::string n, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetSpectrumChannelAttribute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::AttributeValue const &', 'v')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetSpectrumChannelType(std::string type) [member function]
    cls.add_method('SetSpectrumChannelType', 
                   'void', 
                   [param('std::string', 'type')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetUeAntennaModelAttribute(std::string n, ns3::AttributeValue const & v) [member function]
    cls.add_method('SetUeAntennaModelAttribute', 
                   'void', 
                   [param('std::string', 'n'), param('ns3::AttributeValue const &', 'v')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::SetUeAntennaModelType(std::string type) [member function]
    cls.add_method('SetUeAntennaModelType', 
                   'void', 
                   [param('std::string', 'type')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3LteHexGridEnbTopologyHelper_methods(root_module, cls):
    ## lte-hex-grid-enb-topology-helper.h (module 'lte'): ns3::LteHexGridEnbTopologyHelper::LteHexGridEnbTopologyHelper(ns3::LteHexGridEnbTopologyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteHexGridEnbTopologyHelper const &', 'arg0')])
    ## lte-hex-grid-enb-topology-helper.h (module 'lte'): ns3::LteHexGridEnbTopologyHelper::LteHexGridEnbTopologyHelper() [constructor]
    cls.add_constructor([])
    ## lte-hex-grid-enb-topology-helper.h (module 'lte'): void ns3::LteHexGridEnbTopologyHelper::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-hex-grid-enb-topology-helper.h (module 'lte'): static ns3::TypeId ns3::LteHexGridEnbTopologyHelper::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-hex-grid-enb-topology-helper.h (module 'lte'): void ns3::LteHexGridEnbTopologyHelper::SetLteHelper(ns3::Ptr<ns3::LteHelper> h) [member function]
    cls.add_method('SetLteHelper', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteHelper >', 'h')])
    ## lte-hex-grid-enb-topology-helper.h (module 'lte'): ns3::NetDeviceContainer ns3::LteHexGridEnbTopologyHelper::SetPositionAndInstallEnbDevice(ns3::NodeContainer c) [member function]
    cls.add_method('SetPositionAndInstallEnbDevice', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')])
    return

def register_Ns3LteInterference_methods(root_module, cls):
    ## lte-interference.h (module 'lte'): ns3::LteInterference::LteInterference(ns3::LteInterference const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteInterference const &', 'arg0')])
    ## lte-interference.h (module 'lte'): ns3::LteInterference::LteInterference() [constructor]
    cls.add_constructor([])
    ## lte-interference.h (module 'lte'): void ns3::LteInterference::AddSignal(ns3::Ptr<ns3::SpectrumValue const> spd, ns3::Time const duration) [member function]
    cls.add_method('AddSignal', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'spd'), param('ns3::Time const', 'duration')])
    ## lte-interference.h (module 'lte'): void ns3::LteInterference::AddSinrChunkProcessor(ns3::Ptr<ns3::LteSinrChunkProcessor> p) [member function]
    cls.add_method('AddSinrChunkProcessor', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteSinrChunkProcessor >', 'p')])
    ## lte-interference.h (module 'lte'): void ns3::LteInterference::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-interference.h (module 'lte'): void ns3::LteInterference::EndRx() [member function]
    cls.add_method('EndRx', 
                   'void', 
                   [])
    ## lte-interference.h (module 'lte'): static ns3::TypeId ns3::LteInterference::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-interference.h (module 'lte'): void ns3::LteInterference::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
    ## lte-interference.h (module 'lte'): void ns3::LteInterference::StartRx(ns3::Ptr<ns3::SpectrumValue const> rxPsd) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd')])
    return

def register_Ns3LtePdcp_methods(root_module, cls):
    ## lte-pdcp.h (module 'lte'): ns3::LtePdcp::LtePdcp(ns3::LtePdcp const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePdcp const &', 'arg0')])
    ## lte-pdcp.h (module 'lte'): ns3::LtePdcp::LtePdcp() [constructor]
    cls.add_constructor([])
    ## lte-pdcp.h (module 'lte'): ns3::LtePdcpSapProvider * ns3::LtePdcp::GetLtePdcpSapProvider() [member function]
    cls.add_method('GetLtePdcpSapProvider', 
                   'ns3::LtePdcpSapProvider *', 
                   [])
    ## lte-pdcp.h (module 'lte'): ns3::LteRlcSapUser * ns3::LtePdcp::GetLteRlcSapUser() [member function]
    cls.add_method('GetLteRlcSapUser', 
                   'ns3::LteRlcSapUser *', 
                   [])
    ## lte-pdcp.h (module 'lte'): static ns3::TypeId ns3::LtePdcp::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-pdcp.h (module 'lte'): void ns3::LtePdcp::SetLcId(uint8_t lcId) [member function]
    cls.add_method('SetLcId', 
                   'void', 
                   [param('uint8_t', 'lcId')])
    ## lte-pdcp.h (module 'lte'): void ns3::LtePdcp::SetLtePdcpSapUser(ns3::LtePdcpSapUser * s) [member function]
    cls.add_method('SetLtePdcpSapUser', 
                   'void', 
                   [param('ns3::LtePdcpSapUser *', 's')])
    ## lte-pdcp.h (module 'lte'): void ns3::LtePdcp::SetLteRlcSapProvider(ns3::LteRlcSapProvider * s) [member function]
    cls.add_method('SetLteRlcSapProvider', 
                   'void', 
                   [param('ns3::LteRlcSapProvider *', 's')])
    ## lte-pdcp.h (module 'lte'): void ns3::LtePdcp::SetRnti(uint16_t rnti) [member function]
    cls.add_method('SetRnti', 
                   'void', 
                   [param('uint16_t', 'rnti')])
    ## lte-pdcp.h (module 'lte'): void ns3::LtePdcp::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [])
    ## lte-pdcp.h (module 'lte'): void ns3::LtePdcp::DoReceivePdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceivePdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='protected', is_virtual=True)
    ## lte-pdcp.h (module 'lte'): void ns3::LtePdcp::DoTransmitRrcPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoTransmitRrcPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3LtePhy_methods(root_module, cls):
    ## lte-phy.h (module 'lte'): ns3::LtePhy::LtePhy(ns3::LtePhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePhy const &', 'arg0')])
    ## lte-phy.h (module 'lte'): ns3::LtePhy::LtePhy() [constructor]
    cls.add_constructor([])
    ## lte-phy.h (module 'lte'): ns3::LtePhy::LtePhy(ns3::Ptr<ns3::LteSpectrumPhy> dlPhy, ns3::Ptr<ns3::LteSpectrumPhy> ulPhy) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteSpectrumPhy >', 'dlPhy'), param('ns3::Ptr< ns3::LteSpectrumPhy >', 'ulPhy')])
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::LtePhy::CreateTxPowerSpectralDensity() [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoSendMacPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoSendMacPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoSetBandwidth(uint8_t ulBandwidth, uint8_t dlBandwidth) [member function]
    cls.add_method('DoSetBandwidth', 
                   'void', 
                   [param('uint8_t', 'ulBandwidth'), param('uint8_t', 'dlBandwidth')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoSetCellId(uint16_t cellId) [member function]
    cls.add_method('DoSetCellId', 
                   'void', 
                   [param('uint16_t', 'cellId')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoSetDownlinkSubChannels() [member function]
    cls.add_method('DoSetDownlinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoSetEarfcn(uint16_t dlEarfcn, uint16_t ulEarfcn) [member function]
    cls.add_method('DoSetEarfcn', 
                   'void', 
                   [param('uint16_t', 'dlEarfcn'), param('uint16_t', 'ulEarfcn')], 
                   is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoSetUplinkSubChannels() [member function]
    cls.add_method('DoSetUplinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::GenerateCqiReport(ns3::SpectrumValue const & sinr) [member function]
    cls.add_method('GenerateCqiReport', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h (module 'lte'): std::list<ns3::Ptr<ns3::IdealControlMessage>,std::allocator<ns3::Ptr<ns3::IdealControlMessage> > > ns3::LtePhy::GetControlMessages() [member function]
    cls.add_method('GetControlMessages', 
                   'std::list< ns3::Ptr< ns3::IdealControlMessage > >', 
                   [])
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::LteNetDevice> ns3::LtePhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::LteSpectrumPhy> ns3::LtePhy::GetDownlinkSpectrumPhy() [member function]
    cls.add_method('GetDownlinkSpectrumPhy', 
                   'ns3::Ptr< ns3::LteSpectrumPhy >', 
                   [])
    ## lte-phy.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::LtePhy::GetDownlinkSubChannels() [member function]
    cls.add_method('GetDownlinkSubChannels', 
                   'std::vector< int >', 
                   [])
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::PacketBurst> ns3::LtePhy::GetPacketBurst() [member function]
    cls.add_method('GetPacketBurst', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [])
    ## lte-phy.h (module 'lte'): uint8_t ns3::LtePhy::GetRbgSize() const [member function]
    cls.add_method('GetRbgSize', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-phy.h (module 'lte'): double ns3::LtePhy::GetTti() const [member function]
    cls.add_method('GetTti', 
                   'double', 
                   [], 
                   is_const=True)
    ## lte-phy.h (module 'lte'): static ns3::TypeId ns3::LtePhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::LteSpectrumPhy> ns3::LtePhy::GetUplinkSpectrumPhy() [member function]
    cls.add_method('GetUplinkSpectrumPhy', 
                   'ns3::Ptr< ns3::LteSpectrumPhy >', 
                   [])
    ## lte-phy.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::LtePhy::GetUplinkSubChannels() [member function]
    cls.add_method('GetUplinkSubChannels', 
                   'std::vector< int >', 
                   [])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetControlMessages(ns3::Ptr<ns3::IdealControlMessage> m) [member function]
    cls.add_method('SetControlMessages', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'm')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetDevice(ns3::Ptr<ns3::LteNetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetDownlinkChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetDownlinkChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetDownlinkSubChannels(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetDownlinkSubChannels', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetMacPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('SetMacPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetTti(double tti) [member function]
    cls.add_method('SetTti', 
                   'void', 
                   [param('double', 'tti')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetUplinkChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetUplinkChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetUplinkSubChannels(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetUplinkSubChannels', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    return

def register_Ns3LteRadioBearerInfo_methods(root_module, cls):
    ## lte-radio-bearer-info.h (module 'lte'): ns3::LteRadioBearerInfo::LteRadioBearerInfo(ns3::LteRadioBearerInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRadioBearerInfo const &', 'arg0')])
    ## lte-radio-bearer-info.h (module 'lte'): ns3::LteRadioBearerInfo::LteRadioBearerInfo() [constructor]
    cls.add_constructor([])
    ## lte-radio-bearer-info.h (module 'lte'): static ns3::TypeId ns3::LteRadioBearerInfo::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-radio-bearer-info.h (module 'lte'): ns3::LteRadioBearerInfo::m_pdcp [variable]
    cls.add_instance_attribute('m_pdcp', 'ns3::Ptr< ns3::LtePdcp >', is_const=False)
    ## lte-radio-bearer-info.h (module 'lte'): ns3::LteRadioBearerInfo::m_rlc [variable]
    cls.add_instance_attribute('m_rlc', 'ns3::Ptr< ns3::LteRlc >', is_const=False)
    return

def register_Ns3LteRlc_methods(root_module, cls):
    ## lte-rlc.h (module 'lte'): ns3::LteRlc::LteRlc(ns3::LteRlc const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlc const &', 'arg0')])
    ## lte-rlc.h (module 'lte'): ns3::LteRlc::LteRlc() [constructor]
    cls.add_constructor([])
    ## lte-rlc.h (module 'lte'): ns3::LteMacSapUser * ns3::LteRlc::GetLteMacSapUser() [member function]
    cls.add_method('GetLteMacSapUser', 
                   'ns3::LteMacSapUser *', 
                   [])
    ## lte-rlc.h (module 'lte'): ns3::LteRlcSapProvider * ns3::LteRlc::GetLteRlcSapProvider() [member function]
    cls.add_method('GetLteRlcSapProvider', 
                   'ns3::LteRlcSapProvider *', 
                   [])
    ## lte-rlc.h (module 'lte'): static ns3::TypeId ns3::LteRlc::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-rlc.h (module 'lte'): void ns3::LteRlc::SetLcId(uint8_t lcId) [member function]
    cls.add_method('SetLcId', 
                   'void', 
                   [param('uint8_t', 'lcId')])
    ## lte-rlc.h (module 'lte'): void ns3::LteRlc::SetLteMacSapProvider(ns3::LteMacSapProvider * s) [member function]
    cls.add_method('SetLteMacSapProvider', 
                   'void', 
                   [param('ns3::LteMacSapProvider *', 's')])
    ## lte-rlc.h (module 'lte'): void ns3::LteRlc::SetLteRlcSapUser(ns3::LteRlcSapUser * s) [member function]
    cls.add_method('SetLteRlcSapUser', 
                   'void', 
                   [param('ns3::LteRlcSapUser *', 's')])
    ## lte-rlc.h (module 'lte'): void ns3::LteRlc::SetRnti(uint16_t rnti) [member function]
    cls.add_method('SetRnti', 
                   'void', 
                   [param('uint16_t', 'rnti')])
    ## lte-rlc.h (module 'lte'): void ns3::LteRlc::DoNotifyHarqDeliveryFailure() [member function]
    cls.add_method('DoNotifyHarqDeliveryFailure', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## lte-rlc.h (module 'lte'): void ns3::LteRlc::DoNotifyTxOpportunity(uint32_t bytes, uint8_t layer) [member function]
    cls.add_method('DoNotifyTxOpportunity', 
                   'void', 
                   [param('uint32_t', 'bytes'), param('uint8_t', 'layer')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## lte-rlc.h (module 'lte'): void ns3::LteRlc::DoReceivePdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceivePdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## lte-rlc.h (module 'lte'): void ns3::LteRlc::DoTransmitPdcpPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoTransmitPdcpPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    return

def register_Ns3LteRlcAm_methods(root_module, cls):
    ## lte-rlc-am.h (module 'lte'): ns3::LteRlcAm::LteRlcAm(ns3::LteRlcAm const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcAm const &', 'arg0')])
    ## lte-rlc-am.h (module 'lte'): ns3::LteRlcAm::LteRlcAm() [constructor]
    cls.add_constructor([])
    ## lte-rlc-am.h (module 'lte'): void ns3::LteRlcAm::DoNotifyHarqDeliveryFailure() [member function]
    cls.add_method('DoNotifyHarqDeliveryFailure', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-rlc-am.h (module 'lte'): void ns3::LteRlcAm::DoNotifyTxOpportunity(uint32_t bytes, uint8_t layer) [member function]
    cls.add_method('DoNotifyTxOpportunity', 
                   'void', 
                   [param('uint32_t', 'bytes'), param('uint8_t', 'layer')], 
                   is_virtual=True)
    ## lte-rlc-am.h (module 'lte'): void ns3::LteRlcAm::DoReceivePdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceivePdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## lte-rlc-am.h (module 'lte'): void ns3::LteRlcAm::DoTransmitPdcpPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoTransmitPdcpPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## lte-rlc-am.h (module 'lte'): static ns3::TypeId ns3::LteRlcAm::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-rlc-am.h (module 'lte'): void ns3::LteRlcAm::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [])
    return

def register_Ns3LteRlcSm_methods(root_module, cls):
    ## lte-rlc.h (module 'lte'): ns3::LteRlcSm::LteRlcSm(ns3::LteRlcSm const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcSm const &', 'arg0')])
    ## lte-rlc.h (module 'lte'): ns3::LteRlcSm::LteRlcSm() [constructor]
    cls.add_constructor([])
    ## lte-rlc.h (module 'lte'): void ns3::LteRlcSm::DoNotifyHarqDeliveryFailure() [member function]
    cls.add_method('DoNotifyHarqDeliveryFailure', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-rlc.h (module 'lte'): void ns3::LteRlcSm::DoNotifyTxOpportunity(uint32_t bytes, uint8_t layer) [member function]
    cls.add_method('DoNotifyTxOpportunity', 
                   'void', 
                   [param('uint32_t', 'bytes'), param('uint8_t', 'layer')], 
                   is_virtual=True)
    ## lte-rlc.h (module 'lte'): void ns3::LteRlcSm::DoReceivePdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceivePdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## lte-rlc.h (module 'lte'): void ns3::LteRlcSm::DoTransmitPdcpPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoTransmitPdcpPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## lte-rlc.h (module 'lte'): static ns3::TypeId ns3::LteRlcSm::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-rlc.h (module 'lte'): void ns3::LteRlcSm::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [])
    return

def register_Ns3LteRlcUm_methods(root_module, cls):
    ## lte-rlc-um.h (module 'lte'): ns3::LteRlcUm::LteRlcUm(ns3::LteRlcUm const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteRlcUm const &', 'arg0')])
    ## lte-rlc-um.h (module 'lte'): ns3::LteRlcUm::LteRlcUm() [constructor]
    cls.add_constructor([])
    ## lte-rlc-um.h (module 'lte'): void ns3::LteRlcUm::DoNotifyHarqDeliveryFailure() [member function]
    cls.add_method('DoNotifyHarqDeliveryFailure', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-rlc-um.h (module 'lte'): void ns3::LteRlcUm::DoNotifyTxOpportunity(uint32_t bytes, uint8_t layer) [member function]
    cls.add_method('DoNotifyTxOpportunity', 
                   'void', 
                   [param('uint32_t', 'bytes'), param('uint8_t', 'layer')], 
                   is_virtual=True)
    ## lte-rlc-um.h (module 'lte'): void ns3::LteRlcUm::DoReceivePdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceivePdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## lte-rlc-um.h (module 'lte'): void ns3::LteRlcUm::DoTransmitPdcpPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoTransmitPdcpPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## lte-rlc-um.h (module 'lte'): static ns3::TypeId ns3::LteRlcUm::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-rlc-um.h (module 'lte'): void ns3::LteRlcUm::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [])
    return

def register_Ns3LteSinrChunkProcessor_methods(root_module, cls):
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LteSinrChunkProcessor::LteSinrChunkProcessor() [constructor]
    cls.add_constructor([])
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LteSinrChunkProcessor::LteSinrChunkProcessor(ns3::LteSinrChunkProcessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteSinrChunkProcessor const &', 'arg0')])
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LteSinrChunkProcessor::End() [member function]
    cls.add_method('End', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LteSinrChunkProcessor::EvaluateSinrChunk(ns3::SpectrumValue const & sinr, ns3::Time duration) [member function]
    cls.add_method('EvaluateSinrChunk', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr'), param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LteSinrChunkProcessor::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3LteSpectrumPhy_methods(root_module, cls):
    ## lte-spectrum-phy.h (module 'lte'): ns3::LteSpectrumPhy::LteSpectrumPhy() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-phy.h (module 'lte'): static ns3::TypeId ns3::LteSpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetMobility(ns3::Ptr<ns3::MobilityModel> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'm')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetDevice(ns3::Ptr<ns3::NetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'd')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::MobilityModel> ns3::LteSpectrumPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::MobilityModel >', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::NetDevice> ns3::LteSpectrumPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumModel const> ns3::LteSpectrumPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::AntennaModel> ns3::LteSpectrumPhy::GetRxAntenna() [member function]
    cls.add_method('GetRxAntenna', 
                   'ns3::Ptr< ns3::AntennaModel >', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::StartRx(ns3::Ptr<ns3::SpectrumSignalParameters> params) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumSignalParameters >', 'params')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetTxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue> txPsd) [member function]
    cls.add_method('SetTxPowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetAntenna(ns3::Ptr<ns3::AntennaModel> a) [member function]
    cls.add_method('SetAntenna', 
                   'void', 
                   [param('ns3::Ptr< ns3::AntennaModel >', 'a')])
    ## lte-spectrum-phy.h (module 'lte'): bool ns3::LteSpectrumPhy::StartTx(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('StartTx', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetGenericPhyTxEndCallback(ns3::GenericPhyTxEndCallback c) [member function]
    cls.add_method('SetGenericPhyTxEndCallback', 
                   'void', 
                   [param('ns3::GenericPhyTxEndCallback', 'c')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetGenericPhyRxEndErrorCallback(ns3::GenericPhyRxEndErrorCallback c) [member function]
    cls.add_method('SetGenericPhyRxEndErrorCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxEndErrorCallback', 'c')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetGenericPhyRxEndOkCallback(ns3::GenericPhyRxEndOkCallback c) [member function]
    cls.add_method('SetGenericPhyRxEndOkCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxEndOkCallback', 'c')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetState(ns3::LteSpectrumPhy::State newState) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::LteSpectrumPhy::State', 'newState')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetCellId(uint16_t cellId) [member function]
    cls.add_method('SetCellId', 
                   'void', 
                   [param('uint16_t', 'cellId')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::AddSinrChunkProcessor(ns3::Ptr<ns3::LteSinrChunkProcessor> p) [member function]
    cls.add_method('AddSinrChunkProcessor', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteSinrChunkProcessor >', 'p')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::AddExpectedTb(uint16_t rnti, uint16_t size, uint8_t mcs, std::vector<int, std::allocator<int> > map, uint8_t layer) [member function]
    cls.add_method('AddExpectedTb', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint16_t', 'size'), param('uint8_t', 'mcs'), param('std::vector< int >', 'map'), param('uint8_t', 'layer')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::UpdateSinrPerceived(ns3::SpectrumValue const & sinr) [member function]
    cls.add_method('UpdateSinrPerceived', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetTransmissionMode(uint8_t txMode) [member function]
    cls.add_method('SetTransmissionMode', 
                   'void', 
                   [param('uint8_t', 'txMode')])
    ## lte-spectrum-phy.h (module 'lte'): int64_t ns3::LteSpectrumPhy::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')])
    return

def register_Ns3LteSpectrumSignalParameters_methods(root_module, cls):
    ## lte-spectrum-signal-parameters.h (module 'lte'): ns3::LteSpectrumSignalParameters::LteSpectrumSignalParameters() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-signal-parameters.h (module 'lte'): ns3::LteSpectrumSignalParameters::LteSpectrumSignalParameters(ns3::LteSpectrumSignalParameters const & p) [copy constructor]
    cls.add_constructor([param('ns3::LteSpectrumSignalParameters const &', 'p')])
    ## lte-spectrum-signal-parameters.h (module 'lte'): ns3::Ptr<ns3::SpectrumSignalParameters> ns3::LteSpectrumSignalParameters::Copy() [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::SpectrumSignalParameters >', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-signal-parameters.h (module 'lte'): ns3::LteSpectrumSignalParameters::packetBurst [variable]
    cls.add_instance_attribute('packetBurst', 'ns3::Ptr< ns3::PacketBurst >', is_const=False)
    return

def register_Ns3LteStatsCalculator_methods(root_module, cls):
    ## lte-stats-calculator.h (module 'lte'): ns3::LteStatsCalculator::LteStatsCalculator(ns3::LteStatsCalculator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteStatsCalculator const &', 'arg0')])
    ## lte-stats-calculator.h (module 'lte'): ns3::LteStatsCalculator::LteStatsCalculator() [constructor]
    cls.add_constructor([])
    ## lte-stats-calculator.h (module 'lte'): bool ns3::LteStatsCalculator::ExistsCellIdPath(std::string path) [member function]
    cls.add_method('ExistsCellIdPath', 
                   'bool', 
                   [param('std::string', 'path')])
    ## lte-stats-calculator.h (module 'lte'): bool ns3::LteStatsCalculator::ExistsImsiPath(std::string path) [member function]
    cls.add_method('ExistsImsiPath', 
                   'bool', 
                   [param('std::string', 'path')])
    ## lte-stats-calculator.h (module 'lte'): uint16_t ns3::LteStatsCalculator::GetCellIdPath(std::string path) [member function]
    cls.add_method('GetCellIdPath', 
                   'uint16_t', 
                   [param('std::string', 'path')])
    ## lte-stats-calculator.h (module 'lte'): std::string ns3::LteStatsCalculator::GetDlOutputFilename() [member function]
    cls.add_method('GetDlOutputFilename', 
                   'std::string', 
                   [])
    ## lte-stats-calculator.h (module 'lte'): uint64_t ns3::LteStatsCalculator::GetImsiPath(std::string path) [member function]
    cls.add_method('GetImsiPath', 
                   'uint64_t', 
                   [param('std::string', 'path')])
    ## lte-stats-calculator.h (module 'lte'): static ns3::TypeId ns3::LteStatsCalculator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-stats-calculator.h (module 'lte'): std::string ns3::LteStatsCalculator::GetUlOutputFilename() [member function]
    cls.add_method('GetUlOutputFilename', 
                   'std::string', 
                   [])
    ## lte-stats-calculator.h (module 'lte'): void ns3::LteStatsCalculator::SetCellIdPath(std::string path, uint16_t cellId) [member function]
    cls.add_method('SetCellIdPath', 
                   'void', 
                   [param('std::string', 'path'), param('uint16_t', 'cellId')])
    ## lte-stats-calculator.h (module 'lte'): void ns3::LteStatsCalculator::SetDlOutputFilename(std::string outputFilename) [member function]
    cls.add_method('SetDlOutputFilename', 
                   'void', 
                   [param('std::string', 'outputFilename')])
    ## lte-stats-calculator.h (module 'lte'): void ns3::LteStatsCalculator::SetImsiPath(std::string path, uint64_t imsi) [member function]
    cls.add_method('SetImsiPath', 
                   'void', 
                   [param('std::string', 'path'), param('uint64_t', 'imsi')])
    ## lte-stats-calculator.h (module 'lte'): void ns3::LteStatsCalculator::SetUlOutputFilename(std::string outputFilename) [member function]
    cls.add_method('SetUlOutputFilename', 
                   'void', 
                   [param('std::string', 'outputFilename')])
    return

def register_Ns3LteUeMac_methods(root_module, cls):
    ## lte-ue-mac.h (module 'lte'): ns3::LteUeMac::LteUeMac(ns3::LteUeMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteUeMac const &', 'arg0')])
    ## lte-ue-mac.h (module 'lte'): ns3::LteUeMac::LteUeMac() [constructor]
    cls.add_constructor([])
    ## lte-ue-mac.h (module 'lte'): void ns3::LteUeMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-ue-mac.h (module 'lte'): void ns3::LteUeMac::DoSubframeIndication(uint32_t frameNo, uint32_t subframeNo) [member function]
    cls.add_method('DoSubframeIndication', 
                   'void', 
                   [param('uint32_t', 'frameNo'), param('uint32_t', 'subframeNo')])
    ## lte-ue-mac.h (module 'lte'): ns3::LteMacSapProvider * ns3::LteUeMac::GetLteMacSapProvider() [member function]
    cls.add_method('GetLteMacSapProvider', 
                   'ns3::LteMacSapProvider *', 
                   [])
    ## lte-ue-mac.h (module 'lte'): ns3::LteUeCmacSapProvider * ns3::LteUeMac::GetLteUeCmacSapProvider() [member function]
    cls.add_method('GetLteUeCmacSapProvider', 
                   'ns3::LteUeCmacSapProvider *', 
                   [])
    ## lte-ue-mac.h (module 'lte'): ns3::LteUePhySapUser * ns3::LteUeMac::GetLteUePhySapUser() [member function]
    cls.add_method('GetLteUePhySapUser', 
                   'ns3::LteUePhySapUser *', 
                   [])
    ## lte-ue-mac.h (module 'lte'): static ns3::TypeId ns3::LteUeMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-ue-mac.h (module 'lte'): void ns3::LteUeMac::SetLteUeCmacSapUser(ns3::LteUeCmacSapUser * s) [member function]
    cls.add_method('SetLteUeCmacSapUser', 
                   'void', 
                   [param('ns3::LteUeCmacSapUser *', 's')])
    ## lte-ue-mac.h (module 'lte'): void ns3::LteUeMac::SetLteUePhySapProvider(ns3::LteUePhySapProvider * s) [member function]
    cls.add_method('SetLteUePhySapProvider', 
                   'void', 
                   [param('ns3::LteUePhySapProvider *', 's')])
    return

def register_Ns3LteUePhy_methods(root_module, cls):
    ## lte-ue-phy.h (module 'lte'): ns3::LteUePhy::LteUePhy(ns3::LteUePhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteUePhy const &', 'arg0')])
    ## lte-ue-phy.h (module 'lte'): ns3::LteUePhy::LteUePhy() [constructor]
    cls.add_constructor([])
    ## lte-ue-phy.h (module 'lte'): ns3::LteUePhy::LteUePhy(ns3::Ptr<ns3::LteSpectrumPhy> dlPhy, ns3::Ptr<ns3::LteSpectrumPhy> ulPhy) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteSpectrumPhy >', 'dlPhy'), param('ns3::Ptr< ns3::LteSpectrumPhy >', 'ulPhy')])
    ## lte-ue-phy.h (module 'lte'): ns3::Ptr<ns3::DlCqiIdealControlMessage> ns3::LteUePhy::CreateDlCqiFeedbackMessage(ns3::SpectrumValue const & sinr) [member function]
    cls.add_method('CreateDlCqiFeedbackMessage', 
                   'ns3::Ptr< ns3::DlCqiIdealControlMessage >', 
                   [param('ns3::SpectrumValue const &', 'sinr')])
    ## lte-ue-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::LteUePhy::CreateTxPowerSpectralDensity() [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::DoSendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('DoSendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::DoSendMacPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoSendMacPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::DoSetTransmissionMode(uint8_t txMode) [member function]
    cls.add_method('DoSetTransmissionMode', 
                   'void', 
                   [param('uint8_t', 'txMode')], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::DoSetUplinkSubChannels() [member function]
    cls.add_method('DoSetUplinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::GenerateCqiReport(ns3::SpectrumValue const & sinr) [member function]
    cls.add_method('GenerateCqiReport', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr')], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): ns3::LteUePhySapProvider * ns3::LteUePhy::GetLteUePhySapProvider() [member function]
    cls.add_method('GetLteUePhySapProvider', 
                   'ns3::LteUePhySapProvider *', 
                   [])
    ## lte-ue-phy.h (module 'lte'): uint8_t ns3::LteUePhy::GetMacChDelay() const [member function]
    cls.add_method('GetMacChDelay', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-ue-phy.h (module 'lte'): double ns3::LteUePhy::GetNoiseFigure() const [member function]
    cls.add_method('GetNoiseFigure', 
                   'double', 
                   [], 
                   is_const=True)
    ## lte-ue-phy.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::LteUePhy::GetSubChannelsForReception() [member function]
    cls.add_method('GetSubChannelsForReception', 
                   'std::vector< int >', 
                   [])
    ## lte-ue-phy.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::LteUePhy::GetSubChannelsForTransmission() [member function]
    cls.add_method('GetSubChannelsForTransmission', 
                   'std::vector< int >', 
                   [])
    ## lte-ue-phy.h (module 'lte'): double ns3::LteUePhy::GetTxPower() const [member function]
    cls.add_method('GetTxPower', 
                   'double', 
                   [], 
                   is_const=True)
    ## lte-ue-phy.h (module 'lte'): static ns3::TypeId ns3::LteUePhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::PhyPduReceived(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('PhyPduReceived', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::SetEnbCellId(uint16_t cellId) [member function]
    cls.add_method('SetEnbCellId', 
                   'void', 
                   [param('uint16_t', 'cellId')])
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::SetLteUePhySapUser(ns3::LteUePhySapUser * s) [member function]
    cls.add_method('SetLteUePhySapUser', 
                   'void', 
                   [param('ns3::LteUePhySapUser *', 's')])
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::SetNoiseFigure(double pow) [member function]
    cls.add_method('SetNoiseFigure', 
                   'void', 
                   [param('double', 'pow')])
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::SetRnti(uint16_t rnti) [member function]
    cls.add_method('SetRnti', 
                   'void', 
                   [param('uint16_t', 'rnti')])
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::SetSubChannelsForReception(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetSubChannelsForReception', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::SetSubChannelsForTransmission(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetSubChannelsForTransmission', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::SetTxPower(double pow) [member function]
    cls.add_method('SetTxPower', 
                   'void', 
                   [param('double', 'pow')])
    ## lte-ue-phy.h (module 'lte'): void ns3::LteUePhy::SubframeIndication(uint32_t frameNo, uint32_t subframeNo) [member function]
    cls.add_method('SubframeIndication', 
                   'void', 
                   [param('uint32_t', 'frameNo'), param('uint32_t', 'subframeNo')])
    return

def register_Ns3LteUeRrc_methods(root_module, cls):
    ## lte-ue-rrc.h (module 'lte'): ns3::LteUeRrc::LteUeRrc(ns3::LteUeRrc const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteUeRrc const &', 'arg0')])
    ## lte-ue-rrc.h (module 'lte'): ns3::LteUeRrc::LteUeRrc() [constructor]
    cls.add_constructor([])
    ## lte-ue-rrc.h (module 'lte'): void ns3::LteUeRrc::ConfigureUe(uint16_t rnti, uint16_t cellId) [member function]
    cls.add_method('ConfigureUe', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint16_t', 'cellId')])
    ## lte-ue-rrc.h (module 'lte'): void ns3::LteUeRrc::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-ue-rrc.h (module 'lte'): void ns3::LteUeRrc::DoRrcConfigurationUpdateInd(ns3::LteUeConfig_t params) [member function]
    cls.add_method('DoRrcConfigurationUpdateInd', 
                   'void', 
                   [param('ns3::LteUeConfig_t', 'params')])
    ## lte-ue-rrc.h (module 'lte'): uint16_t ns3::LteUeRrc::GetCellId() [member function]
    cls.add_method('GetCellId', 
                   'uint16_t', 
                   [])
    ## lte-ue-rrc.h (module 'lte'): std::vector<unsigned char, std::allocator<unsigned char> > ns3::LteUeRrc::GetLcIdVector() [member function]
    cls.add_method('GetLcIdVector', 
                   'std::vector< unsigned char >', 
                   [])
    ## lte-ue-rrc.h (module 'lte'): ns3::LteUeCmacSapUser * ns3::LteUeRrc::GetLteUeCmacSapUser() [member function]
    cls.add_method('GetLteUeCmacSapUser', 
                   'ns3::LteUeCmacSapUser *', 
                   [])
    ## lte-ue-rrc.h (module 'lte'): uint16_t ns3::LteUeRrc::GetRnti() [member function]
    cls.add_method('GetRnti', 
                   'uint16_t', 
                   [])
    ## lte-ue-rrc.h (module 'lte'): static ns3::TypeId ns3::LteUeRrc::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-ue-rrc.h (module 'lte'): void ns3::LteUeRrc::ReleaseRadioBearer(uint16_t rnti, uint8_t lcId) [member function]
    cls.add_method('ReleaseRadioBearer', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint8_t', 'lcId')])
    ## lte-ue-rrc.h (module 'lte'): bool ns3::LteUeRrc::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## lte-ue-rrc.h (module 'lte'): void ns3::LteUeRrc::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## lte-ue-rrc.h (module 'lte'): void ns3::LteUeRrc::SetLteMacSapProvider(ns3::LteMacSapProvider * s) [member function]
    cls.add_method('SetLteMacSapProvider', 
                   'void', 
                   [param('ns3::LteMacSapProvider *', 's')])
    ## lte-ue-rrc.h (module 'lte'): void ns3::LteUeRrc::SetLteUeCmacSapProvider(ns3::LteUeCmacSapProvider * s) [member function]
    cls.add_method('SetLteUeCmacSapProvider', 
                   'void', 
                   [param('ns3::LteUeCmacSapProvider *', 's')])
    ## lte-ue-rrc.h (module 'lte'): void ns3::LteUeRrc::SetupRadioBearer(uint16_t rnti, ns3::EpsBearer bearer, ns3::TypeId rlcTypeId, uint8_t lcid, ns3::Ptr<ns3::EpcTft> tft) [member function]
    cls.add_method('SetupRadioBearer', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('ns3::EpsBearer', 'bearer'), param('ns3::TypeId', 'rlcTypeId'), param('uint8_t', 'lcid'), param('ns3::Ptr< ns3::EpcTft >', 'tft')])
    return

def register_Ns3Mac48AddressChecker_methods(root_module, cls):
    ## mac48-address.h (module 'network'): ns3::Mac48AddressChecker::Mac48AddressChecker() [constructor]
    cls.add_constructor([])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressChecker::Mac48AddressChecker(ns3::Mac48AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48AddressChecker const &', 'arg0')])
    return

def register_Ns3Mac48AddressValue_methods(root_module, cls):
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue::Mac48AddressValue() [constructor]
    cls.add_constructor([])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue::Mac48AddressValue(ns3::Mac48AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48AddressValue const &', 'arg0')])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue::Mac48AddressValue(ns3::Mac48Address const & value) [constructor]
    cls.add_constructor([param('ns3::Mac48Address const &', 'value')])
    ## mac48-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Mac48AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac48-address.h (module 'network'): bool ns3::Mac48AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## mac48-address.h (module 'network'): ns3::Mac48Address ns3::Mac48AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mac48-address.h (module 'network'): std::string ns3::Mac48AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## mac48-address.h (module 'network'): void ns3::Mac48AddressValue::Set(ns3::Mac48Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Mac48Address const &', 'value')])
    return

def register_Ns3MacStatsCalculator_methods(root_module, cls):
    ## mac-stats-calculator.h (module 'lte'): ns3::MacStatsCalculator::MacStatsCalculator(ns3::MacStatsCalculator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacStatsCalculator const &', 'arg0')])
    ## mac-stats-calculator.h (module 'lte'): ns3::MacStatsCalculator::MacStatsCalculator() [constructor]
    cls.add_constructor([])
    ## mac-stats-calculator.h (module 'lte'): void ns3::MacStatsCalculator::DlScheduling(uint16_t cellId, uint64_t imsi, uint32_t frameNo, uint32_t subframeNo, uint16_t rnti, uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2) [member function]
    cls.add_method('DlScheduling', 
                   'void', 
                   [param('uint16_t', 'cellId'), param('uint64_t', 'imsi'), param('uint32_t', 'frameNo'), param('uint32_t', 'subframeNo'), param('uint16_t', 'rnti'), param('uint8_t', 'mcsTb1'), param('uint16_t', 'sizeTb1'), param('uint8_t', 'mcsTb2'), param('uint16_t', 'sizeTb2')])
    ## mac-stats-calculator.h (module 'lte'): std::string ns3::MacStatsCalculator::GetDlOutputFilename() [member function]
    cls.add_method('GetDlOutputFilename', 
                   'std::string', 
                   [])
    ## mac-stats-calculator.h (module 'lte'): static ns3::TypeId ns3::MacStatsCalculator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-stats-calculator.h (module 'lte'): std::string ns3::MacStatsCalculator::GetUlOutputFilename() [member function]
    cls.add_method('GetUlOutputFilename', 
                   'std::string', 
                   [])
    ## mac-stats-calculator.h (module 'lte'): void ns3::MacStatsCalculator::SetDlOutputFilename(std::string outputFilename) [member function]
    cls.add_method('SetDlOutputFilename', 
                   'void', 
                   [param('std::string', 'outputFilename')])
    ## mac-stats-calculator.h (module 'lte'): void ns3::MacStatsCalculator::SetUlOutputFilename(std::string outputFilename) [member function]
    cls.add_method('SetUlOutputFilename', 
                   'void', 
                   [param('std::string', 'outputFilename')])
    ## mac-stats-calculator.h (module 'lte'): void ns3::MacStatsCalculator::UlScheduling(uint16_t cellId, uint64_t imsi, uint32_t frameNo, uint32_t subframeNo, uint16_t rnti, uint8_t mcs, uint16_t sizeTb) [member function]
    cls.add_method('UlScheduling', 
                   'void', 
                   [param('uint16_t', 'cellId'), param('uint64_t', 'imsi'), param('uint32_t', 'frameNo'), param('uint32_t', 'subframeNo'), param('uint16_t', 'rnti'), param('uint8_t', 'mcs'), param('uint16_t', 'sizeTb')])
    return

def register_Ns3MobilityModel_methods(root_module, cls):
    ## mobility-model.h (module 'mobility'): ns3::MobilityModel::MobilityModel(ns3::MobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MobilityModel const &', 'arg0')])
    ## mobility-model.h (module 'mobility'): ns3::MobilityModel::MobilityModel() [constructor]
    cls.add_constructor([])
    ## mobility-model.h (module 'mobility'): int64_t ns3::MobilityModel::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')])
    ## mobility-model.h (module 'mobility'): double ns3::MobilityModel::GetDistanceFrom(ns3::Ptr<const ns3::MobilityModel> position) const [member function]
    cls.add_method('GetDistanceFrom', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'position')], 
                   is_const=True)
    ## mobility-model.h (module 'mobility'): ns3::Vector ns3::MobilityModel::GetPosition() const [member function]
    cls.add_method('GetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## mobility-model.h (module 'mobility'): double ns3::MobilityModel::GetRelativeSpeed(ns3::Ptr<const ns3::MobilityModel> other) const [member function]
    cls.add_method('GetRelativeSpeed', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'other')], 
                   is_const=True)
    ## mobility-model.h (module 'mobility'): static ns3::TypeId ns3::MobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mobility-model.h (module 'mobility'): ns3::Vector ns3::MobilityModel::GetVelocity() const [member function]
    cls.add_method('GetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## mobility-model.h (module 'mobility'): void ns3::MobilityModel::SetPosition(ns3::Vector const & position) [member function]
    cls.add_method('SetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')])
    ## mobility-model.h (module 'mobility'): void ns3::MobilityModel::NotifyCourseChange() const [member function]
    cls.add_method('NotifyCourseChange', 
                   'void', 
                   [], 
                   is_const=True, visibility='protected')
    ## mobility-model.h (module 'mobility'): int64_t ns3::MobilityModel::DoAssignStreams(int64_t start) [member function]
    cls.add_method('DoAssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'start')], 
                   visibility='private', is_virtual=True)
    ## mobility-model.h (module 'mobility'): ns3::Vector ns3::MobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## mobility-model.h (module 'mobility'): ns3::Vector ns3::MobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## mobility-model.h (module 'mobility'): void ns3::MobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3NetDevice_methods(root_module, cls):
    ## net-device.h (module 'network'): ns3::NetDevice::NetDevice() [constructor]
    cls.add_constructor([])
    ## net-device.h (module 'network'): ns3::NetDevice::NetDevice(ns3::NetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDevice const &', 'arg0')])
    ## net-device.h (module 'network'): void ns3::NetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Address ns3::NetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Address ns3::NetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Ptr<ns3::Channel> ns3::NetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): uint32_t ns3::NetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): uint16_t ns3::NetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Address ns3::NetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Address ns3::NetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Ptr<ns3::Node> ns3::NetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): static ns3::TypeId ns3::NetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3NixVector_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## nix-vector.h (module 'network'): ns3::NixVector::NixVector() [constructor]
    cls.add_constructor([])
    ## nix-vector.h (module 'network'): ns3::NixVector::NixVector(ns3::NixVector const & o) [copy constructor]
    cls.add_constructor([param('ns3::NixVector const &', 'o')])
    ## nix-vector.h (module 'network'): void ns3::NixVector::AddNeighborIndex(uint32_t newBits, uint32_t numberOfBits) [member function]
    cls.add_method('AddNeighborIndex', 
                   'void', 
                   [param('uint32_t', 'newBits'), param('uint32_t', 'numberOfBits')])
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::BitCount(uint32_t numberOfNeighbors) const [member function]
    cls.add_method('BitCount', 
                   'uint32_t', 
                   [param('uint32_t', 'numberOfNeighbors')], 
                   is_const=True)
    ## nix-vector.h (module 'network'): ns3::Ptr<ns3::NixVector> ns3::NixVector::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::NixVector >', 
                   [], 
                   is_const=True)
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::Deserialize(uint32_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint32_t const *', 'buffer'), param('uint32_t', 'size')])
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::ExtractNeighborIndex(uint32_t numberOfBits) [member function]
    cls.add_method('ExtractNeighborIndex', 
                   'uint32_t', 
                   [param('uint32_t', 'numberOfBits')])
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::GetRemainingBits() [member function]
    cls.add_method('GetRemainingBits', 
                   'uint32_t', 
                   [])
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::Serialize(uint32_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint32_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3Node_methods(root_module, cls):
    ## node.h (module 'network'): ns3::Node::Node(ns3::Node const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Node const &', 'arg0')])
    ## node.h (module 'network'): ns3::Node::Node() [constructor]
    cls.add_constructor([])
    ## node.h (module 'network'): ns3::Node::Node(uint32_t systemId) [constructor]
    cls.add_constructor([param('uint32_t', 'systemId')])
    ## node.h (module 'network'): uint32_t ns3::Node::AddApplication(ns3::Ptr<ns3::Application> application) [member function]
    cls.add_method('AddApplication', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Application >', 'application')])
    ## node.h (module 'network'): uint32_t ns3::Node::AddDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddDevice', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## node.h (module 'network'): static bool ns3::Node::ChecksumEnabled() [member function]
    cls.add_method('ChecksumEnabled', 
                   'bool', 
                   [], 
                   is_static=True)
    ## node.h (module 'network'): ns3::Ptr<ns3::Application> ns3::Node::GetApplication(uint32_t index) const [member function]
    cls.add_method('GetApplication', 
                   'ns3::Ptr< ns3::Application >', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## node.h (module 'network'): ns3::Ptr<ns3::NetDevice> ns3::Node::GetDevice(uint32_t index) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## node.h (module 'network'): uint32_t ns3::Node::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h (module 'network'): uint32_t ns3::Node::GetNApplications() const [member function]
    cls.add_method('GetNApplications', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h (module 'network'): uint32_t ns3::Node::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h (module 'network'): uint32_t ns3::Node::GetSystemId() const [member function]
    cls.add_method('GetSystemId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h (module 'network'): static ns3::TypeId ns3::Node::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## node.h (module 'network'): void ns3::Node::RegisterDeviceAdditionListener(ns3::Callback<void,ns3::Ptr<ns3::NetDevice>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> listener) [member function]
    cls.add_method('RegisterDeviceAdditionListener', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'listener')])
    ## node.h (module 'network'): void ns3::Node::RegisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler, uint16_t protocolType, ns3::Ptr<ns3::NetDevice> device, bool promiscuous=false) [member function]
    cls.add_method('RegisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler'), param('uint16_t', 'protocolType'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'promiscuous', default_value='false')])
    ## node.h (module 'network'): void ns3::Node::UnregisterDeviceAdditionListener(ns3::Callback<void,ns3::Ptr<ns3::NetDevice>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> listener) [member function]
    cls.add_method('UnregisterDeviceAdditionListener', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'listener')])
    ## node.h (module 'network'): void ns3::Node::UnregisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler) [member function]
    cls.add_method('UnregisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler')])
    ## node.h (module 'network'): void ns3::Node::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## node.h (module 'network'): void ns3::Node::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3NormalRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): ns3::NormalRandomVariable::INFINITE_VALUE [variable]
    cls.add_static_attribute('INFINITE_VALUE', 'double const', is_const=True)
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::NormalRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::NormalRandomVariable::NormalRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::NormalRandomVariable::GetMean() const [member function]
    cls.add_method('GetMean', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::NormalRandomVariable::GetVariance() const [member function]
    cls.add_method('GetVariance', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::NormalRandomVariable::GetBound() const [member function]
    cls.add_method('GetBound', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::NormalRandomVariable::GetValue(double mean, double variance, double bound) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'mean'), param('double', 'variance'), param('double', 'bound')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::NormalRandomVariable::GetInteger(uint32_t mean, uint32_t variance, uint32_t bound) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'mean'), param('uint32_t', 'variance'), param('uint32_t', 'bound')])
    ## random-variable-stream.h (module 'core'): double ns3::NormalRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::NormalRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3ObjectFactoryChecker_methods(root_module, cls):
    ## object-factory.h (module 'core'): ns3::ObjectFactoryChecker::ObjectFactoryChecker() [constructor]
    cls.add_constructor([])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryChecker::ObjectFactoryChecker(ns3::ObjectFactoryChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactoryChecker const &', 'arg0')])
    return

def register_Ns3ObjectFactoryValue_methods(root_module, cls):
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue::ObjectFactoryValue() [constructor]
    cls.add_constructor([])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue::ObjectFactoryValue(ns3::ObjectFactoryValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactoryValue const &', 'arg0')])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue::ObjectFactoryValue(ns3::ObjectFactory const & value) [constructor]
    cls.add_constructor([param('ns3::ObjectFactory const &', 'value')])
    ## object-factory.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::ObjectFactoryValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object-factory.h (module 'core'): bool ns3::ObjectFactoryValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## object-factory.h (module 'core'): ns3::ObjectFactory ns3::ObjectFactoryValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::ObjectFactory', 
                   [], 
                   is_const=True)
    ## object-factory.h (module 'core'): std::string ns3::ObjectFactoryValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## object-factory.h (module 'core'): void ns3::ObjectFactoryValue::Set(ns3::ObjectFactory const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::ObjectFactory const &', 'value')])
    return

def register_Ns3Packet_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## packet.h (module 'network'): ns3::Packet::Packet() [constructor]
    cls.add_constructor([])
    ## packet.h (module 'network'): ns3::Packet::Packet(ns3::Packet const & o) [copy constructor]
    cls.add_constructor([param('ns3::Packet const &', 'o')])
    ## packet.h (module 'network'): ns3::Packet::Packet(uint32_t size) [constructor]
    cls.add_constructor([param('uint32_t', 'size')])
    ## packet.h (module 'network'): ns3::Packet::Packet(uint8_t const * buffer, uint32_t size, bool magic) [constructor]
    cls.add_constructor([param('uint8_t const *', 'buffer'), param('uint32_t', 'size'), param('bool', 'magic')])
    ## packet.h (module 'network'): ns3::Packet::Packet(uint8_t const * buffer, uint32_t size) [constructor]
    cls.add_constructor([param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## packet.h (module 'network'): void ns3::Packet::AddAtEnd(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## packet.h (module 'network'): void ns3::Packet::AddByteTag(ns3::Tag const & tag) const [member function]
    cls.add_method('AddByteTag', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::AddHeader(ns3::Header const & header) [member function]
    cls.add_method('AddHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header')])
    ## packet.h (module 'network'): void ns3::Packet::AddPacketTag(ns3::Tag const & tag) const [member function]
    cls.add_method('AddPacketTag', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::AddPaddingAtEnd(uint32_t size) [member function]
    cls.add_method('AddPaddingAtEnd', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h (module 'network'): void ns3::Packet::AddTrailer(ns3::Trailer const & trailer) [member function]
    cls.add_method('AddTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer')])
    ## packet.h (module 'network'): ns3::PacketMetadata::ItemIterator ns3::Packet::BeginItem() const [member function]
    cls.add_method('BeginItem', 
                   'ns3::PacketMetadata::ItemIterator', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Packet::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::CopyData(uint8_t * buffer, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::CopyData(std::ostream * os, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'void', 
                   [param('std::ostream *', 'os'), param('uint32_t', 'size')], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Packet::CreateFragment(uint32_t start, uint32_t length) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'start'), param('uint32_t', 'length')], 
                   is_const=True)
    ## packet.h (module 'network'): static void ns3::Packet::EnableChecking() [member function]
    cls.add_method('EnableChecking', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet.h (module 'network'): static void ns3::Packet::EnablePrinting() [member function]
    cls.add_method('EnablePrinting', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet.h (module 'network'): bool ns3::Packet::FindFirstMatchingByteTag(ns3::Tag & tag) const [member function]
    cls.add_method('FindFirstMatchingByteTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::ByteTagIterator ns3::Packet::GetByteTagIterator() const [member function]
    cls.add_method('GetByteTagIterator', 
                   'ns3::ByteTagIterator', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::Ptr<ns3::NixVector> ns3::Packet::GetNixVector() const [member function]
    cls.add_method('GetNixVector', 
                   'ns3::Ptr< ns3::NixVector >', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::PacketTagIterator ns3::Packet::GetPacketTagIterator() const [member function]
    cls.add_method('GetPacketTagIterator', 
                   'ns3::PacketTagIterator', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint64_t ns3::Packet::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint8_t const * ns3::Packet::PeekData() const [member function]
    cls.add_method('PeekData', 
                   'uint8_t const *', 
                   [], 
                   deprecated=True, is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::PeekHeader(ns3::Header & header) const [member function]
    cls.add_method('PeekHeader', 
                   'uint32_t', 
                   [param('ns3::Header &', 'header')], 
                   is_const=True)
    ## packet.h (module 'network'): bool ns3::Packet::PeekPacketTag(ns3::Tag & tag) const [member function]
    cls.add_method('PeekPacketTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::PeekTrailer(ns3::Trailer & trailer) [member function]
    cls.add_method('PeekTrailer', 
                   'uint32_t', 
                   [param('ns3::Trailer &', 'trailer')])
    ## packet.h (module 'network'): void ns3::Packet::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::PrintByteTags(std::ostream & os) const [member function]
    cls.add_method('PrintByteTags', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::PrintPacketTags(std::ostream & os) const [member function]
    cls.add_method('PrintPacketTags', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::RemoveAllByteTags() [member function]
    cls.add_method('RemoveAllByteTags', 
                   'void', 
                   [])
    ## packet.h (module 'network'): void ns3::Packet::RemoveAllPacketTags() [member function]
    cls.add_method('RemoveAllPacketTags', 
                   'void', 
                   [])
    ## packet.h (module 'network'): void ns3::Packet::RemoveAtEnd(uint32_t size) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h (module 'network'): void ns3::Packet::RemoveAtStart(uint32_t size) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h (module 'network'): uint32_t ns3::Packet::RemoveHeader(ns3::Header & header) [member function]
    cls.add_method('RemoveHeader', 
                   'uint32_t', 
                   [param('ns3::Header &', 'header')])
    ## packet.h (module 'network'): bool ns3::Packet::RemovePacketTag(ns3::Tag & tag) [member function]
    cls.add_method('RemovePacketTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    ## packet.h (module 'network'): uint32_t ns3::Packet::RemoveTrailer(ns3::Trailer & trailer) [member function]
    cls.add_method('RemoveTrailer', 
                   'uint32_t', 
                   [param('ns3::Trailer &', 'trailer')])
    ## packet.h (module 'network'): uint32_t ns3::Packet::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::SetNixVector(ns3::Ptr<ns3::NixVector> arg0) [member function]
    cls.add_method('SetNixVector', 
                   'void', 
                   [param('ns3::Ptr< ns3::NixVector >', 'arg0')])
    return

def register_Ns3ParetoRandomVariable_methods(root_module, cls):
    ## random-variable-stream.h (module 'core'): static ns3::TypeId ns3::ParetoRandomVariable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-variable-stream.h (module 'core'): ns3::ParetoRandomVariable::ParetoRandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable-stream.h (module 'core'): double ns3::ParetoRandomVariable::GetMean() const [member function]
    cls.add_method('GetMean', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ParetoRandomVariable::GetShape() const [member function]
    cls.add_method('GetShape', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ParetoRandomVariable::GetBound() const [member function]
    cls.add_method('GetBound', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable-stream.h (module 'core'): double ns3::ParetoRandomVariable::GetValue(double mean, double shape, double bound) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'mean'), param('double', 'shape'), param('double', 'bound')])
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ParetoRandomVariable::GetInteger(uint32_t mean, uint32_t shape, uint32_t bound) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 'mean'), param('uint32_t', 'shape'), param('uint32_t', 'bound')])
    ## random-variable-stream.h (module 'core'): double ns3::ParetoRandomVariable::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_virtual=True)
    ## random-variable-stream.h (module 'core'): uint32_t ns3::ParetoRandomVariable::GetInteger() [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3PfFfMacScheduler_methods(root_module, cls):
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::PfFfMacScheduler::PfFfMacScheduler(ns3::PfFfMacScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PfFfMacScheduler const &', 'arg0')])
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::PfFfMacScheduler::PfFfMacScheduler() [constructor]
    cls.add_constructor([])
    ## pf-ff-mac-scheduler.h (module 'lte'): void ns3::PfFfMacScheduler::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::FfMacCschedSapProvider * ns3::PfFfMacScheduler::GetFfMacCschedSapProvider() [member function]
    cls.add_method('GetFfMacCschedSapProvider', 
                   'ns3::FfMacCschedSapProvider *', 
                   [], 
                   is_virtual=True)
    ## pf-ff-mac-scheduler.h (module 'lte'): ns3::FfMacSchedSapProvider * ns3::PfFfMacScheduler::GetFfMacSchedSapProvider() [member function]
    cls.add_method('GetFfMacSchedSapProvider', 
                   'ns3::FfMacSchedSapProvider *', 
                   [], 
                   is_virtual=True)
    ## pf-ff-mac-scheduler.h (module 'lte'): static ns3::TypeId ns3::PfFfMacScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## pf-ff-mac-scheduler.h (module 'lte'): void ns3::PfFfMacScheduler::SetFfMacCschedSapUser(ns3::FfMacCschedSapUser * s) [member function]
    cls.add_method('SetFfMacCschedSapUser', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser *', 's')], 
                   is_virtual=True)
    ## pf-ff-mac-scheduler.h (module 'lte'): void ns3::PfFfMacScheduler::SetFfMacSchedSapUser(ns3::FfMacSchedSapUser * s) [member function]
    cls.add_method('SetFfMacSchedSapUser', 
                   'void', 
                   [param('ns3::FfMacSchedSapUser *', 's')], 
                   is_virtual=True)
    ## pf-ff-mac-scheduler.h (module 'lte'): void ns3::PfFfMacScheduler::TransmissionModeConfigurationUpdate(uint16_t rnti, uint8_t txMode) [member function]
    cls.add_method('TransmissionModeConfigurationUpdate', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint8_t', 'txMode')])
    return

def register_Ns3PointerChecker_methods(root_module, cls):
    ## pointer.h (module 'core'): ns3::PointerChecker::PointerChecker() [constructor]
    cls.add_constructor([])
    ## pointer.h (module 'core'): ns3::PointerChecker::PointerChecker(ns3::PointerChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointerChecker const &', 'arg0')])
    ## pointer.h (module 'core'): ns3::TypeId ns3::PointerChecker::GetPointeeTypeId() const [member function]
    cls.add_method('GetPointeeTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3PointerValue_methods(root_module, cls):
    ## pointer.h (module 'core'): ns3::PointerValue::PointerValue(ns3::PointerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointerValue const &', 'arg0')])
    ## pointer.h (module 'core'): ns3::PointerValue::PointerValue() [constructor]
    cls.add_constructor([])
    ## pointer.h (module 'core'): ns3::PointerValue::PointerValue(ns3::Ptr<ns3::Object> object) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Object >', 'object')])
    ## pointer.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::PointerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## pointer.h (module 'core'): bool ns3::PointerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## pointer.h (module 'core'): ns3::Ptr<ns3::Object> ns3::PointerValue::GetObject() const [member function]
    cls.add_method('GetObject', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## pointer.h (module 'core'): std::string ns3::PointerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## pointer.h (module 'core'): void ns3::PointerValue::SetObject(ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('SetObject', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'object')])
    return

def register_Ns3RadioBearerStatsCalculator_methods(root_module, cls):
    ## radio-bearer-stats-calculator.h (module 'lte'): ns3::RadioBearerStatsCalculator::RadioBearerStatsCalculator(ns3::RadioBearerStatsCalculator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadioBearerStatsCalculator const &', 'arg0')])
    ## radio-bearer-stats-calculator.h (module 'lte'): ns3::RadioBearerStatsCalculator::RadioBearerStatsCalculator() [constructor]
    cls.add_constructor([])
    ## radio-bearer-stats-calculator.h (module 'lte'): ns3::RadioBearerStatsCalculator::RadioBearerStatsCalculator(std::string protocolType) [constructor]
    cls.add_constructor([param('std::string', 'protocolType')])
    ## radio-bearer-stats-calculator.h (module 'lte'): void ns3::RadioBearerStatsCalculator::DlRxPdu(uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay) [member function]
    cls.add_method('DlRxPdu', 
                   'void', 
                   [param('uint64_t', 'imsi'), param('uint16_t', 'rnti'), param('uint8_t', 'lcid'), param('uint32_t', 'packetSize'), param('uint64_t', 'delay')])
    ## radio-bearer-stats-calculator.h (module 'lte'): void ns3::RadioBearerStatsCalculator::DlTxPdu(uint16_t cellId, uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize) [member function]
    cls.add_method('DlTxPdu', 
                   'void', 
                   [param('uint16_t', 'cellId'), param('uint64_t', 'imsi'), param('uint16_t', 'rnti'), param('uint8_t', 'lcid'), param('uint32_t', 'packetSize')])
    ## radio-bearer-stats-calculator.h (module 'lte'): void ns3::RadioBearerStatsCalculator::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## radio-bearer-stats-calculator.h (module 'lte'): uint32_t ns3::RadioBearerStatsCalculator::GetDlCellId(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetDlCellId', 
                   'uint32_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): double ns3::RadioBearerStatsCalculator::GetDlDelay(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetDlDelay', 
                   'double', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): std::vector<double, std::allocator<double> > ns3::RadioBearerStatsCalculator::GetDlDelayStats(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetDlDelayStats', 
                   'std::vector< double >', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): std::string ns3::RadioBearerStatsCalculator::GetDlOutputFilename() [member function]
    cls.add_method('GetDlOutputFilename', 
                   'std::string', 
                   [])
    ## radio-bearer-stats-calculator.h (module 'lte'): std::string ns3::RadioBearerStatsCalculator::GetDlPdcpOutputFilename() [member function]
    cls.add_method('GetDlPdcpOutputFilename', 
                   'std::string', 
                   [])
    ## radio-bearer-stats-calculator.h (module 'lte'): std::vector<double, std::allocator<double> > ns3::RadioBearerStatsCalculator::GetDlPduSizeStats(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetDlPduSizeStats', 
                   'std::vector< double >', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): uint64_t ns3::RadioBearerStatsCalculator::GetDlRxData(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetDlRxData', 
                   'uint64_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): uint32_t ns3::RadioBearerStatsCalculator::GetDlRxPackets(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetDlRxPackets', 
                   'uint32_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): uint64_t ns3::RadioBearerStatsCalculator::GetDlTxData(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetDlTxData', 
                   'uint64_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): uint32_t ns3::RadioBearerStatsCalculator::GetDlTxPackets(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetDlTxPackets', 
                   'uint32_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): static ns3::TypeId ns3::RadioBearerStatsCalculator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radio-bearer-stats-calculator.h (module 'lte'): uint32_t ns3::RadioBearerStatsCalculator::GetUlCellId(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetUlCellId', 
                   'uint32_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): double ns3::RadioBearerStatsCalculator::GetUlDelay(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetUlDelay', 
                   'double', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): std::vector<double, std::allocator<double> > ns3::RadioBearerStatsCalculator::GetUlDelayStats(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetUlDelayStats', 
                   'std::vector< double >', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): std::string ns3::RadioBearerStatsCalculator::GetUlOutputFilename() [member function]
    cls.add_method('GetUlOutputFilename', 
                   'std::string', 
                   [])
    ## radio-bearer-stats-calculator.h (module 'lte'): std::string ns3::RadioBearerStatsCalculator::GetUlPdcpOutputFilename() [member function]
    cls.add_method('GetUlPdcpOutputFilename', 
                   'std::string', 
                   [])
    ## radio-bearer-stats-calculator.h (module 'lte'): std::vector<double, std::allocator<double> > ns3::RadioBearerStatsCalculator::GetUlPduSizeStats(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetUlPduSizeStats', 
                   'std::vector< double >', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): uint64_t ns3::RadioBearerStatsCalculator::GetUlRxData(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetUlRxData', 
                   'uint64_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): uint32_t ns3::RadioBearerStatsCalculator::GetUlRxPackets(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetUlRxPackets', 
                   'uint32_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): uint64_t ns3::RadioBearerStatsCalculator::GetUlTxData(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetUlTxData', 
                   'uint64_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): uint32_t ns3::RadioBearerStatsCalculator::GetUlTxPackets(uint64_t imsi, uint8_t lcid) [member function]
    cls.add_method('GetUlTxPackets', 
                   'uint32_t', 
                   [param('uint64_t', 'imsi'), param('uint8_t', 'lcid')])
    ## radio-bearer-stats-calculator.h (module 'lte'): void ns3::RadioBearerStatsCalculator::SetDlPdcpOutputFilename(std::string outputFilename) [member function]
    cls.add_method('SetDlPdcpOutputFilename', 
                   'void', 
                   [param('std::string', 'outputFilename')])
    ## radio-bearer-stats-calculator.h (module 'lte'): void ns3::RadioBearerStatsCalculator::SetUlPdcpOutputFilename(std::string outputFilename) [member function]
    cls.add_method('SetUlPdcpOutputFilename', 
                   'void', 
                   [param('std::string', 'outputFilename')])
    ## radio-bearer-stats-calculator.h (module 'lte'): void ns3::RadioBearerStatsCalculator::UlRxPdu(uint16_t cellId, uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay) [member function]
    cls.add_method('UlRxPdu', 
                   'void', 
                   [param('uint16_t', 'cellId'), param('uint64_t', 'imsi'), param('uint16_t', 'rnti'), param('uint8_t', 'lcid'), param('uint32_t', 'packetSize'), param('uint64_t', 'delay')])
    ## radio-bearer-stats-calculator.h (module 'lte'): void ns3::RadioBearerStatsCalculator::UlTxPdu(uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize) [member function]
    cls.add_method('UlTxPdu', 
                   'void', 
                   [param('uint64_t', 'imsi'), param('uint16_t', 'rnti'), param('uint8_t', 'lcid'), param('uint32_t', 'packetSize')])
    return

def register_Ns3RemSpectrumPhy_methods(root_module, cls):
    ## rem-spectrum-phy.h (module 'lte'): ns3::RemSpectrumPhy::RemSpectrumPhy() [constructor]
    cls.add_constructor([])
    ## rem-spectrum-phy.h (module 'lte'): void ns3::RemSpectrumPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): static ns3::TypeId ns3::RemSpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rem-spectrum-phy.h (module 'lte'): void ns3::RemSpectrumPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): void ns3::RemSpectrumPhy::SetMobility(ns3::Ptr<ns3::MobilityModel> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'm')], 
                   is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): void ns3::RemSpectrumPhy::SetDevice(ns3::Ptr<ns3::NetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'd')], 
                   is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::MobilityModel> ns3::RemSpectrumPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::MobilityModel >', 
                   [], 
                   is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::NetDevice> ns3::RemSpectrumPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumModel const> ns3::RemSpectrumPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::AntennaModel> ns3::RemSpectrumPhy::GetRxAntenna() [member function]
    cls.add_method('GetRxAntenna', 
                   'ns3::Ptr< ns3::AntennaModel >', 
                   [], 
                   is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): void ns3::RemSpectrumPhy::StartRx(ns3::Ptr<ns3::SpectrumSignalParameters> params) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumSignalParameters >', 'params')], 
                   is_virtual=True)
    ## rem-spectrum-phy.h (module 'lte'): void ns3::RemSpectrumPhy::SetRxSpectrumModel(ns3::Ptr<ns3::SpectrumModel const> m) [member function]
    cls.add_method('SetRxSpectrumModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumModel const >', 'm')])
    ## rem-spectrum-phy.h (module 'lte'): double ns3::RemSpectrumPhy::GetSinr(double noisePower) [member function]
    cls.add_method('GetSinr', 
                   'double', 
                   [param('double', 'noisePower')])
    ## rem-spectrum-phy.h (module 'lte'): void ns3::RemSpectrumPhy::Deactivate() [member function]
    cls.add_method('Deactivate', 
                   'void', 
                   [])
    ## rem-spectrum-phy.h (module 'lte'): bool ns3::RemSpectrumPhy::IsActive() [member function]
    cls.add_method('IsActive', 
                   'bool', 
                   [])
    ## rem-spectrum-phy.h (module 'lte'): void ns3::RemSpectrumPhy::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    return

def register_Ns3RrFfMacScheduler_methods(root_module, cls):
    ## rr-ff-mac-scheduler.h (module 'lte'): ns3::RrFfMacScheduler::RrFfMacScheduler(ns3::RrFfMacScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RrFfMacScheduler const &', 'arg0')])
    ## rr-ff-mac-scheduler.h (module 'lte'): ns3::RrFfMacScheduler::RrFfMacScheduler() [constructor]
    cls.add_constructor([])
    ## rr-ff-mac-scheduler.h (module 'lte'): void ns3::RrFfMacScheduler::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## rr-ff-mac-scheduler.h (module 'lte'): ns3::FfMacCschedSapProvider * ns3::RrFfMacScheduler::GetFfMacCschedSapProvider() [member function]
    cls.add_method('GetFfMacCschedSapProvider', 
                   'ns3::FfMacCschedSapProvider *', 
                   [], 
                   is_virtual=True)
    ## rr-ff-mac-scheduler.h (module 'lte'): ns3::FfMacSchedSapProvider * ns3::RrFfMacScheduler::GetFfMacSchedSapProvider() [member function]
    cls.add_method('GetFfMacSchedSapProvider', 
                   'ns3::FfMacSchedSapProvider *', 
                   [], 
                   is_virtual=True)
    ## rr-ff-mac-scheduler.h (module 'lte'): static ns3::TypeId ns3::RrFfMacScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rr-ff-mac-scheduler.h (module 'lte'): void ns3::RrFfMacScheduler::SetFfMacCschedSapUser(ns3::FfMacCschedSapUser * s) [member function]
    cls.add_method('SetFfMacCschedSapUser', 
                   'void', 
                   [param('ns3::FfMacCschedSapUser *', 's')], 
                   is_virtual=True)
    ## rr-ff-mac-scheduler.h (module 'lte'): void ns3::RrFfMacScheduler::SetFfMacSchedSapUser(ns3::FfMacSchedSapUser * s) [member function]
    cls.add_method('SetFfMacSchedSapUser', 
                   'void', 
                   [param('ns3::FfMacSchedSapUser *', 's')], 
                   is_virtual=True)
    ## rr-ff-mac-scheduler.h (module 'lte'): void ns3::RrFfMacScheduler::TransmissionModeConfigurationUpdate(uint16_t rnti, uint8_t txMode) [member function]
    cls.add_method('TransmissionModeConfigurationUpdate', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint8_t', 'txMode')])
    return

def register_Ns3SpectrumChannel_methods(root_module, cls):
    ## spectrum-channel.h (module 'spectrum'): ns3::SpectrumChannel::SpectrumChannel() [constructor]
    cls.add_constructor([])
    ## spectrum-channel.h (module 'spectrum'): ns3::SpectrumChannel::SpectrumChannel(ns3::SpectrumChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumChannel const &', 'arg0')])
    ## spectrum-channel.h (module 'spectrum'): void ns3::SpectrumChannel::AddPropagationLossModel(ns3::Ptr<ns3::PropagationLossModel> loss) [member function]
    cls.add_method('AddPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationLossModel >', 'loss')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h (module 'spectrum'): void ns3::SpectrumChannel::AddRx(ns3::Ptr<ns3::SpectrumPhy> phy) [member function]
    cls.add_method('AddRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h (module 'spectrum'): void ns3::SpectrumChannel::AddSpectrumPropagationLossModel(ns3::Ptr<ns3::SpectrumPropagationLossModel> loss) [member function]
    cls.add_method('AddSpectrumPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPropagationLossModel >', 'loss')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h (module 'spectrum'): static ns3::TypeId ns3::SpectrumChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-channel.h (module 'spectrum'): void ns3::SpectrumChannel::SetPropagationDelayModel(ns3::Ptr<ns3::PropagationDelayModel> delay) [member function]
    cls.add_method('SetPropagationDelayModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationDelayModel >', 'delay')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-channel.h (module 'spectrum'): void ns3::SpectrumChannel::StartTx(ns3::Ptr<ns3::SpectrumSignalParameters> params) [member function]
    cls.add_method('StartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumSignalParameters >', 'params')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3StringChecker_methods(root_module, cls):
    ## string.h (module 'core'): ns3::StringChecker::StringChecker() [constructor]
    cls.add_constructor([])
    ## string.h (module 'core'): ns3::StringChecker::StringChecker(ns3::StringChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StringChecker const &', 'arg0')])
    return

def register_Ns3StringValue_methods(root_module, cls):
    ## string.h (module 'core'): ns3::StringValue::StringValue() [constructor]
    cls.add_constructor([])
    ## string.h (module 'core'): ns3::StringValue::StringValue(ns3::StringValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StringValue const &', 'arg0')])
    ## string.h (module 'core'): ns3::StringValue::StringValue(std::string const & value) [constructor]
    cls.add_constructor([param('std::string const &', 'value')])
    ## string.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::StringValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## string.h (module 'core'): bool ns3::StringValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## string.h (module 'core'): std::string ns3::StringValue::Get() const [member function]
    cls.add_method('Get', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## string.h (module 'core'): std::string ns3::StringValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## string.h (module 'core'): void ns3::StringValue::Set(std::string const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string const &', 'value')])
    return

def register_Ns3TimeChecker_methods(root_module, cls):
    ## nstime.h (module 'core'): ns3::TimeChecker::TimeChecker() [constructor]
    cls.add_constructor([])
    ## nstime.h (module 'core'): ns3::TimeChecker::TimeChecker(ns3::TimeChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TimeChecker const &', 'arg0')])
    return

def register_Ns3TimeValue_methods(root_module, cls):
    ## nstime.h (module 'core'): ns3::TimeValue::TimeValue() [constructor]
    cls.add_constructor([])
    ## nstime.h (module 'core'): ns3::TimeValue::TimeValue(ns3::TimeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TimeValue const &', 'arg0')])
    ## nstime.h (module 'core'): ns3::TimeValue::TimeValue(ns3::Time const & value) [constructor]
    cls.add_constructor([param('ns3::Time const &', 'value')])
    ## nstime.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::TimeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nstime.h (module 'core'): bool ns3::TimeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## nstime.h (module 'core'): ns3::Time ns3::TimeValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): std::string ns3::TimeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## nstime.h (module 'core'): void ns3::TimeValue::Set(ns3::Time const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Time const &', 'value')])
    return

def register_Ns3TypeIdChecker_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeIdChecker::TypeIdChecker() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeIdChecker::TypeIdChecker(ns3::TypeIdChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeIdChecker const &', 'arg0')])
    return

def register_Ns3TypeIdValue_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeIdValue::TypeIdValue() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeIdValue::TypeIdValue(ns3::TypeIdValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeIdValue const &', 'arg0')])
    ## type-id.h (module 'core'): ns3::TypeIdValue::TypeIdValue(ns3::TypeId const & value) [constructor]
    cls.add_constructor([param('ns3::TypeId const &', 'value')])
    ## type-id.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::TypeIdValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## type-id.h (module 'core'): bool ns3::TypeIdValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeIdValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeIdValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## type-id.h (module 'core'): void ns3::TypeIdValue::Set(ns3::TypeId const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::TypeId const &', 'value')])
    return

def register_Ns3UintegerValue_methods(root_module, cls):
    ## uinteger.h (module 'core'): ns3::UintegerValue::UintegerValue() [constructor]
    cls.add_constructor([])
    ## uinteger.h (module 'core'): ns3::UintegerValue::UintegerValue(ns3::UintegerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UintegerValue const &', 'arg0')])
    ## uinteger.h (module 'core'): ns3::UintegerValue::UintegerValue(uint64_t const & value) [constructor]
    cls.add_constructor([param('uint64_t const &', 'value')])
    ## uinteger.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::UintegerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uinteger.h (module 'core'): bool ns3::UintegerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## uinteger.h (module 'core'): uint64_t ns3::UintegerValue::Get() const [member function]
    cls.add_method('Get', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## uinteger.h (module 'core'): std::string ns3::UintegerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## uinteger.h (module 'core'): void ns3::UintegerValue::Set(uint64_t const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint64_t const &', 'value')])
    return

def register_Ns3UlDciIdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::UlDciIdealControlMessage::UlDciIdealControlMessage(ns3::UlDciIdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UlDciIdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::UlDciIdealControlMessage::UlDciIdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): UlDciListElement_s ns3::UlDciIdealControlMessage::GetDci() [member function]
    cls.add_method('GetDci', 
                   'UlDciListElement_s', 
                   [])
    ## ideal-control-messages.h (module 'lte'): void ns3::UlDciIdealControlMessage::SetDci(UlDciListElement_s dci) [member function]
    cls.add_method('SetDci', 
                   'void', 
                   [param('UlDciListElement_s', 'dci')])
    return

def register_Ns3Vector2DChecker_methods(root_module, cls):
    ## vector.h (module 'core'): ns3::Vector2DChecker::Vector2DChecker() [constructor]
    cls.add_constructor([])
    ## vector.h (module 'core'): ns3::Vector2DChecker::Vector2DChecker(ns3::Vector2DChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector2DChecker const &', 'arg0')])
    return

def register_Ns3Vector2DValue_methods(root_module, cls):
    ## vector.h (module 'core'): ns3::Vector2DValue::Vector2DValue() [constructor]
    cls.add_constructor([])
    ## vector.h (module 'core'): ns3::Vector2DValue::Vector2DValue(ns3::Vector2DValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector2DValue const &', 'arg0')])
    ## vector.h (module 'core'): ns3::Vector2DValue::Vector2DValue(ns3::Vector2D const & value) [constructor]
    cls.add_constructor([param('ns3::Vector2D const &', 'value')])
    ## vector.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::Vector2DValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## vector.h (module 'core'): bool ns3::Vector2DValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## vector.h (module 'core'): ns3::Vector2D ns3::Vector2DValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Vector2D', 
                   [], 
                   is_const=True)
    ## vector.h (module 'core'): std::string ns3::Vector2DValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## vector.h (module 'core'): void ns3::Vector2DValue::Set(ns3::Vector2D const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Vector2D const &', 'value')])
    return

def register_Ns3Vector3DChecker_methods(root_module, cls):
    ## vector.h (module 'core'): ns3::Vector3DChecker::Vector3DChecker() [constructor]
    cls.add_constructor([])
    ## vector.h (module 'core'): ns3::Vector3DChecker::Vector3DChecker(ns3::Vector3DChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector3DChecker const &', 'arg0')])
    return

def register_Ns3Vector3DValue_methods(root_module, cls):
    ## vector.h (module 'core'): ns3::Vector3DValue::Vector3DValue() [constructor]
    cls.add_constructor([])
    ## vector.h (module 'core'): ns3::Vector3DValue::Vector3DValue(ns3::Vector3DValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector3DValue const &', 'arg0')])
    ## vector.h (module 'core'): ns3::Vector3DValue::Vector3DValue(ns3::Vector3D const & value) [constructor]
    cls.add_constructor([param('ns3::Vector3D const &', 'value')])
    ## vector.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::Vector3DValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## vector.h (module 'core'): bool ns3::Vector3DValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## vector.h (module 'core'): ns3::Vector3D ns3::Vector3DValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Vector3D', 
                   [], 
                   is_const=True)
    ## vector.h (module 'core'): std::string ns3::Vector3DValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## vector.h (module 'core'): void ns3::Vector3DValue::Set(ns3::Vector3D const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Vector3D const &', 'value')])
    return

def register_Ns3VirtualNetDevice_methods(root_module, cls):
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::VirtualNetDevice::VirtualNetDevice(ns3::VirtualNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VirtualNetDevice const &', 'arg0')])
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::VirtualNetDevice::VirtualNetDevice() [constructor]
    cls.add_constructor([])
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::Address ns3::VirtualNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::Address ns3::VirtualNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::Ptr<ns3::Channel> ns3::VirtualNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): uint32_t ns3::VirtualNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): uint16_t ns3::VirtualNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::Address ns3::VirtualNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::Address ns3::VirtualNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): ns3::Ptr<ns3::Node> ns3::VirtualNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): static ns3::TypeId ns3::VirtualNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::Receive(ns3::Ptr<ns3::Packet> packet, uint16_t protocol, ns3::Address const & source, ns3::Address const & destination, ns3::NetDevice::PacketType packetType) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t', 'protocol'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'destination'), param('ns3::NetDevice::PacketType', 'packetType')])
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetIsPointToPoint(bool isPointToPoint) [member function]
    cls.add_method('SetIsPointToPoint', 
                   'void', 
                   [param('bool', 'isPointToPoint')])
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetNeedsArp(bool needsArp) [member function]
    cls.add_method('SetNeedsArp', 
                   'void', 
                   [param('bool', 'needsArp')])
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetSendCallback(ns3::Callback<bool, ns3::Ptr<ns3::Packet>, ns3::Address const&, ns3::Address const&, unsigned short, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> transmitCb) [member function]
    cls.add_method('SetSendCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::Address const &, ns3::Address const &, unsigned short, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'transmitCb')])
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::SetSupportsSendFrom(bool supportsSendFrom) [member function]
    cls.add_method('SetSupportsSendFrom', 
                   'void', 
                   [param('bool', 'supportsSendFrom')])
    ## virtual-net-device.h (module 'virtual-net-device'): bool ns3::VirtualNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## virtual-net-device.h (module 'virtual-net-device'): void ns3::VirtualNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3AddressChecker_methods(root_module, cls):
    ## address.h (module 'network'): ns3::AddressChecker::AddressChecker() [constructor]
    cls.add_constructor([])
    ## address.h (module 'network'): ns3::AddressChecker::AddressChecker(ns3::AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AddressChecker const &', 'arg0')])
    return

def register_Ns3AddressValue_methods(root_module, cls):
    ## address.h (module 'network'): ns3::AddressValue::AddressValue() [constructor]
    cls.add_constructor([])
    ## address.h (module 'network'): ns3::AddressValue::AddressValue(ns3::AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AddressValue const &', 'arg0')])
    ## address.h (module 'network'): ns3::AddressValue::AddressValue(ns3::Address const & value) [constructor]
    cls.add_constructor([param('ns3::Address const &', 'value')])
    ## address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## address.h (module 'network'): bool ns3::AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## address.h (module 'network'): ns3::Address ns3::AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## address.h (module 'network'): std::string ns3::AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## address.h (module 'network'): void ns3::AddressValue::Set(ns3::Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Address const &', 'value')])
    return

def register_Ns3BsrIdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::BsrIdealControlMessage::BsrIdealControlMessage(ns3::BsrIdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BsrIdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::BsrIdealControlMessage::BsrIdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): MacCeListElement_s ns3::BsrIdealControlMessage::GetBsr() [member function]
    cls.add_method('GetBsr', 
                   'MacCeListElement_s', 
                   [])
    ## ideal-control-messages.h (module 'lte'): void ns3::BsrIdealControlMessage::SetBsr(MacCeListElement_s ulcqi) [member function]
    cls.add_method('SetBsr', 
                   'void', 
                   [param('MacCeListElement_s', 'ulcqi')])
    return

def register_Ns3DlCqiIdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::DlCqiIdealControlMessage::DlCqiIdealControlMessage(ns3::DlCqiIdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DlCqiIdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::DlCqiIdealControlMessage::DlCqiIdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): CqiListElement_s ns3::DlCqiIdealControlMessage::GetDlCqi() [member function]
    cls.add_method('GetDlCqi', 
                   'CqiListElement_s', 
                   [])
    ## ideal-control-messages.h (module 'lte'): void ns3::DlCqiIdealControlMessage::SetDlCqi(CqiListElement_s dlcqi) [member function]
    cls.add_method('SetDlCqi', 
                   'void', 
                   [param('CqiListElement_s', 'dlcqi')])
    return

def register_Ns3DlDciIdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::DlDciIdealControlMessage::DlDciIdealControlMessage(ns3::DlDciIdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DlDciIdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::DlDciIdealControlMessage::DlDciIdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): DlDciListElement_s ns3::DlDciIdealControlMessage::GetDci() [member function]
    cls.add_method('GetDci', 
                   'DlDciListElement_s', 
                   [])
    ## ideal-control-messages.h (module 'lte'): void ns3::DlDciIdealControlMessage::SetDci(DlDciListElement_s dci) [member function]
    cls.add_method('SetDci', 
                   'void', 
                   [param('DlDciListElement_s', 'dci')])
    return

def register_Ns3LteCqiSinrChunkProcessor_methods(root_module, cls):
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LteCqiSinrChunkProcessor::LteCqiSinrChunkProcessor(ns3::LteCqiSinrChunkProcessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteCqiSinrChunkProcessor const &', 'arg0')])
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LteCqiSinrChunkProcessor::LteCqiSinrChunkProcessor(ns3::Ptr<ns3::LtePhy> p) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LtePhy >', 'p')])
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LteCqiSinrChunkProcessor::End() [member function]
    cls.add_method('End', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LteCqiSinrChunkProcessor::EvaluateSinrChunk(ns3::SpectrumValue const & sinr, ns3::Time duration) [member function]
    cls.add_method('EvaluateSinrChunk', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr'), param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LteCqiSinrChunkProcessor::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3LteEnbPhy_methods(root_module, cls):
    ## lte-enb-phy.h (module 'lte'): ns3::LteEnbPhy::LteEnbPhy(ns3::LteEnbPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteEnbPhy const &', 'arg0')])
    ## lte-enb-phy.h (module 'lte'): ns3::LteEnbPhy::LteEnbPhy() [constructor]
    cls.add_constructor([])
    ## lte-enb-phy.h (module 'lte'): ns3::LteEnbPhy::LteEnbPhy(ns3::Ptr<ns3::LteSpectrumPhy> dlPhy, ns3::Ptr<ns3::LteSpectrumPhy> ulPhy) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteSpectrumPhy >', 'dlPhy'), param('ns3::Ptr< ns3::LteSpectrumPhy >', 'ulPhy')])
    ## lte-enb-phy.h (module 'lte'): bool ns3::LteEnbPhy::AddUePhy(uint16_t rnti, ns3::Ptr<ns3::LteUePhy> phy) [member function]
    cls.add_method('AddUePhy', 
                   'bool', 
                   [param('uint16_t', 'rnti'), param('ns3::Ptr< ns3::LteUePhy >', 'phy')])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::CalcChannelQualityForUe(std::vector<double, std::allocator<double> > sinr, ns3::Ptr<ns3::LteSpectrumPhy> ue) [member function]
    cls.add_method('CalcChannelQualityForUe', 
                   'void', 
                   [param('std::vector< double >', 'sinr'), param('ns3::Ptr< ns3::LteSpectrumPhy >', 'ue')])
    ## lte-enb-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::LteEnbPhy::CreateTxPowerSpectralDensity() [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): UlCqi_s ns3::LteEnbPhy::CreateUlCqiReport(ns3::SpectrumValue const & sinr) [member function]
    cls.add_method('CreateUlCqiReport', 
                   'UlCqi_s', 
                   [param('ns3::SpectrumValue const &', 'sinr')])
    ## lte-enb-phy.h (module 'lte'): bool ns3::LteEnbPhy::DeleteUePhy(uint16_t rnti) [member function]
    cls.add_method('DeleteUePhy', 
                   'bool', 
                   [param('uint16_t', 'rnti')])
    ## lte-enb-phy.h (module 'lte'): std::list<ns3::UlDciIdealControlMessage,std::allocator<ns3::UlDciIdealControlMessage> > ns3::LteEnbPhy::DequeueUlDci() [member function]
    cls.add_method('DequeueUlDci', 
                   'std::list< ns3::UlDciIdealControlMessage >', 
                   [])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): uint8_t ns3::LteEnbPhy::DoGetMacChTtiDelay() [member function]
    cls.add_method('DoGetMacChTtiDelay', 
                   'uint8_t', 
                   [], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::DoSendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('DoSendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::DoSendMacPdu(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoSendMacPdu', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::DoSetDownlinkSubChannels() [member function]
    cls.add_method('DoSetDownlinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::DoSetTransmissionMode(uint16_t rnti, uint8_t txMode) [member function]
    cls.add_method('DoSetTransmissionMode', 
                   'void', 
                   [param('uint16_t', 'rnti'), param('uint8_t', 'txMode')], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::EndFrame() [member function]
    cls.add_method('EndFrame', 
                   'void', 
                   [])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::EndSubFrame() [member function]
    cls.add_method('EndSubFrame', 
                   'void', 
                   [])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::GenerateCqiReport(ns3::SpectrumValue const & sinr) [member function]
    cls.add_method('GenerateCqiReport', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr')], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): ns3::LteEnbPhySapProvider * ns3::LteEnbPhy::GetLteEnbPhySapProvider() [member function]
    cls.add_method('GetLteEnbPhySapProvider', 
                   'ns3::LteEnbPhySapProvider *', 
                   [])
    ## lte-enb-phy.h (module 'lte'): uint8_t ns3::LteEnbPhy::GetMacChDelay() const [member function]
    cls.add_method('GetMacChDelay', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-enb-phy.h (module 'lte'): double ns3::LteEnbPhy::GetNoiseFigure() const [member function]
    cls.add_method('GetNoiseFigure', 
                   'double', 
                   [], 
                   is_const=True)
    ## lte-enb-phy.h (module 'lte'): double ns3::LteEnbPhy::GetTxPower() const [member function]
    cls.add_method('GetTxPower', 
                   'double', 
                   [], 
                   is_const=True)
    ## lte-enb-phy.h (module 'lte'): static ns3::TypeId ns3::LteEnbPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::PhyPduReceived(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('PhyPduReceived', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::QueueUlDci(ns3::UlDciIdealControlMessage m) [member function]
    cls.add_method('QueueUlDci', 
                   'void', 
                   [param('ns3::UlDciIdealControlMessage', 'm')])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::SetLteEnbPhySapUser(ns3::LteEnbPhySapUser * s) [member function]
    cls.add_method('SetLteEnbPhySapUser', 
                   'void', 
                   [param('ns3::LteEnbPhySapUser *', 's')])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::SetMacChDelay(uint8_t delay) [member function]
    cls.add_method('SetMacChDelay', 
                   'void', 
                   [param('uint8_t', 'delay')])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::SetNoiseFigure(double pow) [member function]
    cls.add_method('SetNoiseFigure', 
                   'void', 
                   [param('double', 'pow')])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::SetTxPower(double pow) [member function]
    cls.add_method('SetTxPower', 
                   'void', 
                   [param('double', 'pow')])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::StartFrame() [member function]
    cls.add_method('StartFrame', 
                   'void', 
                   [])
    ## lte-enb-phy.h (module 'lte'): void ns3::LteEnbPhy::StartSubFrame() [member function]
    cls.add_method('StartSubFrame', 
                   'void', 
                   [])
    return

def register_Ns3LteNetDevice_methods(root_module, cls):
    ## lte-net-device.h (module 'lte'): static ns3::TypeId ns3::LteNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-net-device.h (module 'lte'): ns3::LteNetDevice::LteNetDevice() [constructor]
    cls.add_constructor([])
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): uint32_t ns3::LteNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): ns3::Ptr<ns3::Channel> ns3::LteNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): uint16_t ns3::LteNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): ns3::Address ns3::LteNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): ns3::Address ns3::LteNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): ns3::Ptr<ns3::Node> ns3::LteNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): ns3::Address ns3::LteNetDevice::GetMulticast(ns3::Ipv4Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): ns3::Address ns3::LteNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::Receive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    return

def register_Ns3LtePemSinrChunkProcessor_methods(root_module, cls):
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LtePemSinrChunkProcessor::LtePemSinrChunkProcessor(ns3::LtePemSinrChunkProcessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePemSinrChunkProcessor const &', 'arg0')])
    ## lte-sinr-chunk-processor.h (module 'lte'): ns3::LtePemSinrChunkProcessor::LtePemSinrChunkProcessor(ns3::Ptr<ns3::LteSpectrumPhy> p) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteSpectrumPhy >', 'p')])
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LtePemSinrChunkProcessor::End() [member function]
    cls.add_method('End', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LtePemSinrChunkProcessor::EvaluateSinrChunk(ns3::SpectrumValue const & sinr, ns3::Time duration) [member function]
    cls.add_method('EvaluateSinrChunk', 
                   'void', 
                   [param('ns3::SpectrumValue const &', 'sinr'), param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## lte-sinr-chunk-processor.h (module 'lte'): void ns3::LtePemSinrChunkProcessor::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3LteUeNetDevice_methods(root_module, cls):
    ## lte-ue-net-device.h (module 'lte'): static ns3::TypeId ns3::LteUeNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-ue-net-device.h (module 'lte'): ns3::LteUeNetDevice::LteUeNetDevice() [constructor]
    cls.add_constructor([])
    ## lte-ue-net-device.h (module 'lte'): ns3::LteUeNetDevice::LteUeNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::LteUePhy> phy, ns3::Ptr<ns3::LteUeMac> mac, ns3::Ptr<ns3::LteUeRrc> rrc) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::LteUePhy >', 'phy'), param('ns3::Ptr< ns3::LteUeMac >', 'mac'), param('ns3::Ptr< ns3::LteUeRrc >', 'rrc')])
    ## lte-ue-net-device.h (module 'lte'): void ns3::LteUeNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-ue-net-device.h (module 'lte'): bool ns3::LteUeNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## lte-ue-net-device.h (module 'lte'): ns3::Ptr<ns3::LteUeMac> ns3::LteUeNetDevice::GetMac() [member function]
    cls.add_method('GetMac', 
                   'ns3::Ptr< ns3::LteUeMac >', 
                   [])
    ## lte-ue-net-device.h (module 'lte'): ns3::Ptr<ns3::LteUeRrc> ns3::LteUeNetDevice::GetRrc() [member function]
    cls.add_method('GetRrc', 
                   'ns3::Ptr< ns3::LteUeRrc >', 
                   [])
    ## lte-ue-net-device.h (module 'lte'): ns3::Ptr<ns3::LteUePhy> ns3::LteUeNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::LteUePhy >', 
                   [], 
                   is_const=True)
    ## lte-ue-net-device.h (module 'lte'): void ns3::LteUeNetDevice::SetTargetEnb(ns3::Ptr<ns3::LteEnbNetDevice> enb) [member function]
    cls.add_method('SetTargetEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteEnbNetDevice >', 'enb')])
    ## lte-ue-net-device.h (module 'lte'): ns3::Ptr<ns3::LteEnbNetDevice> ns3::LteUeNetDevice::GetTargetEnb() [member function]
    cls.add_method('GetTargetEnb', 
                   'ns3::Ptr< ns3::LteEnbNetDevice >', 
                   [])
    ## lte-ue-net-device.h (module 'lte'): uint64_t ns3::LteUeNetDevice::GetImsi() [member function]
    cls.add_method('GetImsi', 
                   'uint64_t', 
                   [])
    ## lte-ue-net-device.h (module 'lte'): void ns3::LteUeNetDevice::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3LteEnbNetDevice_methods(root_module, cls):
    ## lte-enb-net-device.h (module 'lte'): static ns3::TypeId ns3::LteEnbNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-enb-net-device.h (module 'lte'): ns3::LteEnbNetDevice::LteEnbNetDevice() [constructor]
    cls.add_constructor([])
    ## lte-enb-net-device.h (module 'lte'): void ns3::LteEnbNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-enb-net-device.h (module 'lte'): bool ns3::LteEnbNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## lte-enb-net-device.h (module 'lte'): ns3::Ptr<ns3::LteEnbMac> ns3::LteEnbNetDevice::GetMac() const [member function]
    cls.add_method('GetMac', 
                   'ns3::Ptr< ns3::LteEnbMac >', 
                   [], 
                   is_const=True)
    ## lte-enb-net-device.h (module 'lte'): ns3::Ptr<ns3::LteEnbPhy> ns3::LteEnbNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::LteEnbPhy >', 
                   [], 
                   is_const=True)
    ## lte-enb-net-device.h (module 'lte'): ns3::Ptr<ns3::LteEnbRrc> ns3::LteEnbNetDevice::GetRrc() const [member function]
    cls.add_method('GetRrc', 
                   'ns3::Ptr< ns3::LteEnbRrc >', 
                   [], 
                   is_const=True)
    ## lte-enb-net-device.h (module 'lte'): uint16_t ns3::LteEnbNetDevice::GetCellId() const [member function]
    cls.add_method('GetCellId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-enb-net-device.h (module 'lte'): uint8_t ns3::LteEnbNetDevice::GetUlBandwidth() const [member function]
    cls.add_method('GetUlBandwidth', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-enb-net-device.h (module 'lte'): void ns3::LteEnbNetDevice::SetUlBandwidth(uint8_t bw) [member function]
    cls.add_method('SetUlBandwidth', 
                   'void', 
                   [param('uint8_t', 'bw')])
    ## lte-enb-net-device.h (module 'lte'): uint8_t ns3::LteEnbNetDevice::GetDlBandwidth() const [member function]
    cls.add_method('GetDlBandwidth', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## lte-enb-net-device.h (module 'lte'): void ns3::LteEnbNetDevice::SetDlBandwidth(uint8_t bw) [member function]
    cls.add_method('SetDlBandwidth', 
                   'void', 
                   [param('uint8_t', 'bw')])
    ## lte-enb-net-device.h (module 'lte'): uint16_t ns3::LteEnbNetDevice::GetDlEarfcn() const [member function]
    cls.add_method('GetDlEarfcn', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-enb-net-device.h (module 'lte'): void ns3::LteEnbNetDevice::SetDlEarfcn(uint16_t earfcn) [member function]
    cls.add_method('SetDlEarfcn', 
                   'void', 
                   [param('uint16_t', 'earfcn')])
    ## lte-enb-net-device.h (module 'lte'): uint16_t ns3::LteEnbNetDevice::GetUlEarfcn() const [member function]
    cls.add_method('GetUlEarfcn', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## lte-enb-net-device.h (module 'lte'): void ns3::LteEnbNetDevice::SetUlEarfcn(uint16_t earfcn) [member function]
    cls.add_method('SetUlEarfcn', 
                   'void', 
                   [param('uint16_t', 'earfcn')])
    ## lte-enb-net-device.h (module 'lte'): void ns3::LteEnbNetDevice::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3ConfigMatchContainer_methods(root_module, cls):
    ## config.h (module 'core'): ns3::Config::MatchContainer::MatchContainer(ns3::Config::MatchContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Config::MatchContainer const &', 'arg0')])
    ## config.h (module 'core'): ns3::Config::MatchContainer::MatchContainer() [constructor]
    cls.add_constructor([])
    ## config.h (module 'core'): ns3::Config::MatchContainer::MatchContainer(std::vector<ns3::Ptr<ns3::Object>, std::allocator<ns3::Ptr<ns3::Object> > > const & objects, std::vector<std::string, std::allocator<std::string> > const & contexts, std::string path) [constructor]
    cls.add_constructor([param('std::vector< ns3::Ptr< ns3::Object > > const &', 'objects'), param('std::vector< std::string > const &', 'contexts'), param('std::string', 'path')])
    ## config.h (module 'core'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Object>*,std::vector<ns3::Ptr<ns3::Object>, std::allocator<ns3::Ptr<ns3::Object> > > > ns3::Config::MatchContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Object > const, std::vector< ns3::Ptr< ns3::Object > > >', 
                   [], 
                   is_const=True)
    ## config.h (module 'core'): void ns3::Config::MatchContainer::Connect(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('Connect', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h (module 'core'): void ns3::Config::MatchContainer::ConnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('ConnectWithoutContext', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h (module 'core'): void ns3::Config::MatchContainer::Disconnect(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('Disconnect', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h (module 'core'): void ns3::Config::MatchContainer::DisconnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('DisconnectWithoutContext', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h (module 'core'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Object>*,std::vector<ns3::Ptr<ns3::Object>, std::allocator<ns3::Ptr<ns3::Object> > > > ns3::Config::MatchContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Object > const, std::vector< ns3::Ptr< ns3::Object > > >', 
                   [], 
                   is_const=True)
    ## config.h (module 'core'): ns3::Ptr<ns3::Object> ns3::Config::MatchContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Object >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## config.h (module 'core'): std::string ns3::Config::MatchContainer::GetMatchedPath(uint32_t i) const [member function]
    cls.add_method('GetMatchedPath', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## config.h (module 'core'): uint32_t ns3::Config::MatchContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## config.h (module 'core'): std::string ns3::Config::MatchContainer::GetPath() const [member function]
    cls.add_method('GetPath', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## config.h (module 'core'): void ns3::Config::MatchContainer::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def main():
    out = FileCodeSink(sys.stdout)
    root_module = module_init()
    register_types(root_module)
    register_methods(root_module)
    register_functions(root_module)
    root_module.generate(out)

if __name__ == '__main__':
    main()

