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
    root_module = Module('ns.mesh', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    root_module = module.get_root()
    
    ## ht-operations.h (module 'wifi'): ns3::HtProtectionType [enumeration]
    module.add_enum('HtProtectionType', ['NO_PROTECTION', 'NON_MEMBER_PROTECTION', 'TWENTY_MHZ_PROTECTION', 'MIXED_MODE_PROTECTION'], import_from_module='ns.wifi')
    ## wifi-mac-header.h (module 'wifi'): ns3::WifiMacType [enumeration]
    module.add_enum('WifiMacType', ['WIFI_MAC_CTL_CTLWRAPPER', 'WIFI_MAC_CTL_RTS', 'WIFI_MAC_CTL_CTS', 'WIFI_MAC_CTL_ACK', 'WIFI_MAC_CTL_BACKREQ', 'WIFI_MAC_CTL_BACKRESP', 'WIFI_MAC_MGT_BEACON', 'WIFI_MAC_MGT_ASSOCIATION_REQUEST', 'WIFI_MAC_MGT_ASSOCIATION_RESPONSE', 'WIFI_MAC_MGT_DISASSOCIATION', 'WIFI_MAC_MGT_REASSOCIATION_REQUEST', 'WIFI_MAC_MGT_REASSOCIATION_RESPONSE', 'WIFI_MAC_MGT_PROBE_REQUEST', 'WIFI_MAC_MGT_PROBE_RESPONSE', 'WIFI_MAC_MGT_AUTHENTICATION', 'WIFI_MAC_MGT_DEAUTHENTICATION', 'WIFI_MAC_MGT_ACTION', 'WIFI_MAC_MGT_ACTION_NO_ACK', 'WIFI_MAC_MGT_MULTIHOP_ACTION', 'WIFI_MAC_DATA', 'WIFI_MAC_DATA_CFACK', 'WIFI_MAC_DATA_CFPOLL', 'WIFI_MAC_DATA_CFACK_CFPOLL', 'WIFI_MAC_DATA_NULL', 'WIFI_MAC_DATA_NULL_CFACK', 'WIFI_MAC_DATA_NULL_CFPOLL', 'WIFI_MAC_DATA_NULL_CFACK_CFPOLL', 'WIFI_MAC_QOSDATA', 'WIFI_MAC_QOSDATA_CFACK', 'WIFI_MAC_QOSDATA_CFPOLL', 'WIFI_MAC_QOSDATA_CFACK_CFPOLL', 'WIFI_MAC_QOSDATA_NULL', 'WIFI_MAC_QOSDATA_NULL_CFPOLL', 'WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL'], import_from_module='ns.wifi')
    ## wifi-preamble.h (module 'wifi'): ns3::WifiPreamble [enumeration]
    module.add_enum('WifiPreamble', ['WIFI_PREAMBLE_LONG', 'WIFI_PREAMBLE_SHORT', 'WIFI_PREAMBLE_HT_MF', 'WIFI_PREAMBLE_HT_GF', 'WIFI_PREAMBLE_VHT', 'WIFI_PREAMBLE_NONE'], import_from_module='ns.wifi')
    ## wifi-phy.h (module 'wifi'): ns3::mpduType [enumeration]
    module.add_enum('mpduType', ['NORMAL_MPDU', 'MPDU_IN_AGGREGATE', 'LAST_MPDU_IN_AGGREGATE'], import_from_module='ns.wifi')
    ## wifi-mode.h (module 'wifi'): ns3::WifiModulationClass [enumeration]
    module.add_enum('WifiModulationClass', ['WIFI_MOD_CLASS_UNKNOWN', 'WIFI_MOD_CLASS_IR', 'WIFI_MOD_CLASS_FHSS', 'WIFI_MOD_CLASS_DSSS', 'WIFI_MOD_CLASS_HR_DSSS', 'WIFI_MOD_CLASS_ERP_PBCC', 'WIFI_MOD_CLASS_DSSS_OFDM', 'WIFI_MOD_CLASS_ERP_OFDM', 'WIFI_MOD_CLASS_OFDM', 'WIFI_MOD_CLASS_HT', 'WIFI_MOD_CLASS_VHT'], import_from_module='ns.wifi')
    ## wifi-phy-standard.h (module 'wifi'): ns3::WifiPhyStandard [enumeration]
    module.add_enum('WifiPhyStandard', ['WIFI_PHY_STANDARD_80211a', 'WIFI_PHY_STANDARD_80211b', 'WIFI_PHY_STANDARD_80211g', 'WIFI_PHY_STANDARD_80211_10MHZ', 'WIFI_PHY_STANDARD_80211_5MHZ', 'WIFI_PHY_STANDARD_holland', 'WIFI_PHY_STANDARD_80211n_2_4GHZ', 'WIFI_PHY_STANDARD_80211n_5GHZ', 'WIFI_PHY_STANDARD_80211ac'], import_from_module='ns.wifi')
    ## wifi-mode.h (module 'wifi'): ns3::WifiCodeRate [enumeration]
    module.add_enum('WifiCodeRate', ['WIFI_CODE_RATE_UNDEFINED', 'WIFI_CODE_RATE_3_4', 'WIFI_CODE_RATE_2_3', 'WIFI_CODE_RATE_1_2', 'WIFI_CODE_RATE_5_6'], import_from_module='ns.wifi')
    ## qos-utils.h (module 'wifi'): ns3::AcIndex [enumeration]
    module.add_enum('AcIndex', ['AC_BE', 'AC_BK', 'AC_VI', 'AC_VO', 'AC_BE_NQOS', 'AC_UNDEF'], import_from_module='ns.wifi')
    ## edca-txop-n.h (module 'wifi'): ns3::TypeOfStation [enumeration]
    module.add_enum('TypeOfStation', ['STA', 'AP', 'ADHOC_STA', 'MESH', 'HT_STA', 'HT_AP', 'HT_ADHOC_STA', 'OCB'], import_from_module='ns.wifi')
    ## ctrl-headers.h (module 'wifi'): ns3::BlockAckType [enumeration]
    module.add_enum('BlockAckType', ['BASIC_BLOCK_ACK', 'COMPRESSED_BLOCK_ACK', 'MULTI_TID_BLOCK_ACK'], import_from_module='ns.wifi')
    ## address.h (module 'network'): ns3::Address [class]
    module.add_class('Address', import_from_module='ns.network')
    ## address.h (module 'network'): ns3::Address::MaxSize_e [enumeration]
    module.add_enum('MaxSize_e', ['MAX_SIZE'], outer_class=root_module['ns3::Address'], import_from_module='ns.network')
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelper [class]
    module.add_class('AsciiTraceHelper', import_from_module='ns.network')
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelperForDevice [class]
    module.add_class('AsciiTraceHelperForDevice', allow_subclassing=True, import_from_module='ns.network')
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList [class]
    module.add_class('AttributeConstructionList', import_from_module='ns.core')
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item [struct]
    module.add_class('Item', import_from_module='ns.core', outer_class=root_module['ns3::AttributeConstructionList'])
    ## block-ack-manager.h (module 'wifi'): ns3::Bar [struct]
    module.add_class('Bar', import_from_module='ns.wifi')
    ## block-ack-agreement.h (module 'wifi'): ns3::BlockAckAgreement [class]
    module.add_class('BlockAckAgreement', import_from_module='ns.wifi')
    ## block-ack-manager.h (module 'wifi'): ns3::BlockAckManager [class]
    module.add_class('BlockAckManager', import_from_module='ns.wifi')
    ## buffer.h (module 'network'): ns3::Buffer [class]
    module.add_class('Buffer', import_from_module='ns.network')
    ## buffer.h (module 'network'): ns3::Buffer::Iterator [class]
    module.add_class('Iterator', import_from_module='ns.network', outer_class=root_module['ns3::Buffer'])
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
    ## capability-information.h (module 'wifi'): ns3::CapabilityInformation [class]
    module.add_class('CapabilityInformation', import_from_module='ns.wifi')
    ## event-id.h (module 'core'): ns3::EventId [class]
    module.add_class('EventId', import_from_module='ns.core')
    ## hash.h (module 'core'): ns3::Hasher [class]
    module.add_class('Hasher', import_from_module='ns.core')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address [class]
    module.add_class('Ipv4Address', import_from_module='ns.network')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address [class]
    root_module['ns3::Ipv4Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask [class]
    module.add_class('Ipv4Mask', import_from_module='ns.network')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address [class]
    module.add_class('Ipv6Address', import_from_module='ns.network')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address [class]
    root_module['ns3::Ipv6Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix [class]
    module.add_class('Ipv6Prefix', import_from_module='ns.network')
    ## mac48-address.h (module 'network'): ns3::Mac48Address [class]
    module.add_class('Mac48Address', import_from_module='ns.network')
    ## mac48-address.h (module 'network'): ns3::Mac48Address [class]
    root_module['ns3::Mac48Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## mesh-helper.h (module 'mesh'): ns3::MeshHelper [class]
    module.add_class('MeshHelper')
    ## mesh-helper.h (module 'mesh'): ns3::MeshHelper::ChannelPolicy [enumeration]
    module.add_enum('ChannelPolicy', ['SPREAD_CHANNELS', 'ZERO_CHANNEL'], outer_class=root_module['ns3::MeshHelper'])
    ## mesh-wifi-beacon.h (module 'mesh'): ns3::MeshWifiBeacon [class]
    module.add_class('MeshWifiBeacon')
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
    ## originator-block-ack-agreement.h (module 'wifi'): ns3::OriginatorBlockAckAgreement [class]
    module.add_class('OriginatorBlockAckAgreement', import_from_module='ns.wifi', parent=root_module['ns3::BlockAckAgreement'])
    ## originator-block-ack-agreement.h (module 'wifi'): ns3::OriginatorBlockAckAgreement::State [enumeration]
    module.add_enum('State', ['PENDING', 'ESTABLISHED', 'INACTIVE', 'UNSUCCESSFUL'], outer_class=root_module['ns3::OriginatorBlockAckAgreement'], import_from_module='ns.wifi')
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
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::TagData_e [enumeration]
    module.add_enum('TagData_e', ['MAX_SIZE'], outer_class=root_module['ns3::PacketTagList::TagData'], import_from_module='ns.network')
    ## pcap-file.h (module 'network'): ns3::PcapFile [class]
    module.add_class('PcapFile', import_from_module='ns.network')
    ## trace-helper.h (module 'network'): ns3::PcapHelper [class]
    module.add_class('PcapHelper', import_from_module='ns.network')
    ## trace-helper.h (module 'network'): ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_NULL', 'DLT_EN10MB', 'DLT_PPP', 'DLT_RAW', 'DLT_IEEE802_11', 'DLT_LINUX_SLL', 'DLT_PRISM_HEADER', 'DLT_IEEE802_11_RADIO', 'DLT_IEEE802_15_4', 'DLT_NETLINK'], outer_class=root_module['ns3::PcapHelper'], import_from_module='ns.network')
    ## trace-helper.h (module 'network'): ns3::PcapHelperForDevice [class]
    module.add_class('PcapHelperForDevice', allow_subclassing=True, import_from_module='ns.network')
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Object', 'ns3::ObjectBase', 'ns3::ObjectDeleter'], parent=root_module['ns3::ObjectBase'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simulator.h (module 'core'): ns3::Simulator [class]
    module.add_class('Simulator', destructor_visibility='private', import_from_module='ns.core')
    ## status-code.h (module 'wifi'): ns3::StatusCode [class]
    module.add_class('StatusCode', import_from_module='ns.wifi')
    ## tag.h (module 'network'): ns3::Tag [class]
    module.add_class('Tag', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## tag-buffer.h (module 'network'): ns3::TagBuffer [class]
    module.add_class('TagBuffer', import_from_module='ns.network')
    ## nstime.h (module 'core'): ns3::TimeWithUnit [class]
    module.add_class('TimeWithUnit', import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId [class]
    module.add_class('TypeId', import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId::AttributeFlag [enumeration]
    module.add_enum('AttributeFlag', ['ATTR_GET', 'ATTR_SET', 'ATTR_CONSTRUCT', 'ATTR_SGC'], outer_class=root_module['ns3::TypeId'], import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation [struct]
    module.add_class('AttributeInformation', import_from_module='ns.core', outer_class=root_module['ns3::TypeId'])
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation [struct]
    module.add_class('TraceSourceInformation', import_from_module='ns.core', outer_class=root_module['ns3::TypeId'])
    ## wifi-helper.h (module 'wifi'): ns3::WifiHelper [class]
    module.add_class('WifiHelper', allow_subclassing=True, import_from_module='ns.wifi')
    ## wifi-mac-helper.h (module 'wifi'): ns3::WifiMacHelper [class]
    module.add_class('WifiMacHelper', allow_subclassing=True, import_from_module='ns.wifi')
    ## wifi-mode.h (module 'wifi'): ns3::WifiMode [class]
    module.add_class('WifiMode', import_from_module='ns.wifi')
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeFactory [class]
    module.add_class('WifiModeFactory', import_from_module='ns.wifi')
    ## wifi-helper.h (module 'wifi'): ns3::WifiPhyHelper [class]
    module.add_class('WifiPhyHelper', allow_subclassing=True, import_from_module='ns.wifi')
    ## wifi-phy.h (module 'wifi'): ns3::WifiPhyListener [class]
    module.add_class('WifiPhyListener', allow_subclassing=True, import_from_module='ns.wifi')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStation [struct]
    module.add_class('WifiRemoteStation', import_from_module='ns.wifi')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationInfo [class]
    module.add_class('WifiRemoteStationInfo', import_from_module='ns.wifi')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState [struct]
    module.add_class('WifiRemoteStationState', import_from_module='ns.wifi')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState [enumeration]
    module.add_enum('', ['BRAND_NEW', 'DISASSOC', 'WAIT_ASSOC_TX_OK', 'GOT_ASSOC_TX_OK'], outer_class=root_module['ns3::WifiRemoteStationState'], import_from_module='ns.wifi')
    ## wifi-tx-vector.h (module 'wifi'): ns3::WifiTxVector [class]
    module.add_class('WifiTxVector', import_from_module='ns.wifi')
    ## empty.h (module 'core'): ns3::empty [class]
    module.add_class('empty', import_from_module='ns.core')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t [class]
    module.add_class('int64x64_t', import_from_module='ns.core')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::impl_type [enumeration]
    module.add_enum('impl_type', ['int128_impl', 'cairo_impl', 'ld_impl'], outer_class=root_module['ns3::int64x64_t'], import_from_module='ns.core')
    ## wifi-phy.h (module 'wifi'): ns3::mpduInfo [struct]
    module.add_class('mpduInfo', import_from_module='ns.wifi')
    ## wifi-phy.h (module 'wifi'): ns3::signalNoiseDbm [struct]
    module.add_class('signalNoiseDbm', import_from_module='ns.wifi')
    ## chunk.h (module 'network'): ns3::Chunk [class]
    module.add_class('Chunk', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## header.h (module 'network'): ns3::Header [class]
    module.add_class('Header', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## mgt-headers.h (module 'wifi'): ns3::MgtAddBaRequestHeader [class]
    module.add_class('MgtAddBaRequestHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## mgt-headers.h (module 'wifi'): ns3::MgtAddBaResponseHeader [class]
    module.add_class('MgtAddBaResponseHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## mgt-headers.h (module 'wifi'): ns3::MgtAssocRequestHeader [class]
    module.add_class('MgtAssocRequestHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## mgt-headers.h (module 'wifi'): ns3::MgtAssocResponseHeader [class]
    module.add_class('MgtAssocResponseHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## mgt-headers.h (module 'wifi'): ns3::MgtDelBaHeader [class]
    module.add_class('MgtDelBaHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## mgt-headers.h (module 'wifi'): ns3::MgtProbeRequestHeader [class]
    module.add_class('MgtProbeRequestHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## mgt-headers.h (module 'wifi'): ns3::MgtProbeResponseHeader [class]
    module.add_class('MgtProbeResponseHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## object.h (module 'core'): ns3::Object [class]
    module.add_class('Object', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    ## object.h (module 'core'): ns3::Object::AggregateIterator [class]
    module.add_class('AggregateIterator', import_from_module='ns.core', outer_class=root_module['ns3::Object'])
    ## pcap-file-wrapper.h (module 'network'): ns3::PcapFileWrapper [class]
    module.add_class('PcapFileWrapper', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeChecker', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeChecker>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::CallbackImplBase', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CallbackImplBase>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::EventImpl', 'ns3::empty', 'ns3::DefaultDeleter<ns3::EventImpl>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Hash::Implementation', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Hash::Implementation>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::MeshWifiInterfaceMacPlugin', 'ns3::empty', 'ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NetDeviceQueue, ns3::empty, ns3::DefaultDeleter<ns3::NetDeviceQueue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::NetDeviceQueue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::NetDeviceQueue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::NixVector', 'ns3::empty', 'ns3::DefaultDeleter<ns3::NixVector>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::OutputStreamWrapper', 'ns3::empty', 'ns3::DefaultDeleter<ns3::OutputStreamWrapper>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Packet', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Packet>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::QueueItem, ns3::empty, ns3::DefaultDeleter<ns3::QueueItem> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::QueueItem', 'ns3::empty', 'ns3::DefaultDeleter<ns3::QueueItem>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::TraceSourceAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::TraceSourceAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::WifiInformationElement', 'ns3::empty', 'ns3::DefaultDeleter<ns3::WifiInformationElement>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::dot11s::DestinationAddressUnit', 'ns3::empty', 'ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::dot11s::IeBeaconTimingUnit', 'ns3::empty', 'ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## nstime.h (module 'core'): ns3::Time [class]
    module.add_class('Time', import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time::Unit [enumeration]
    module.add_enum('Unit', ['Y', 'D', 'H', 'MIN', 'S', 'MS', 'US', 'NS', 'PS', 'FS', 'LAST'], outer_class=root_module['ns3::Time'], import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time [class]
    root_module['ns3::Time'].implicitly_converts_to(root_module['ns3::int64x64_t'])
    ## trace-source-accessor.h (module 'core'): ns3::TraceSourceAccessor [class]
    module.add_class('TraceSourceAccessor', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    ## trailer.h (module 'network'): ns3::Trailer [class]
    module.add_class('Trailer', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader [class]
    module.add_class('WifiActionHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::CategoryValue [enumeration]
    module.add_enum('CategoryValue', ['BLOCK_ACK', 'MESH', 'MULTIHOP', 'SELF_PROTECTED', 'VENDOR_SPECIFIC_ACTION'], outer_class=root_module['ns3::WifiActionHeader'], import_from_module='ns.wifi')
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::SelfProtectedActionValue [enumeration]
    module.add_enum('SelfProtectedActionValue', ['PEER_LINK_OPEN', 'PEER_LINK_CONFIRM', 'PEER_LINK_CLOSE', 'GROUP_KEY_INFORM', 'GROUP_KEY_ACK'], outer_class=root_module['ns3::WifiActionHeader'], import_from_module='ns.wifi')
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::MultihopActionValue [enumeration]
    module.add_enum('MultihopActionValue', ['PROXY_UPDATE', 'PROXY_UPDATE_CONFIRMATION'], outer_class=root_module['ns3::WifiActionHeader'], import_from_module='ns.wifi')
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::MeshActionValue [enumeration]
    module.add_enum('MeshActionValue', ['LINK_METRIC_REPORT', 'PATH_SELECTION', 'PORTAL_ANNOUNCEMENT', 'CONGESTION_CONTROL_NOTIFICATION', 'MDA_SETUP_REQUEST', 'MDA_SETUP_REPLY', 'MDAOP_ADVERTISMENT_REQUEST', 'MDAOP_ADVERTISMENTS', 'MDAOP_SET_TEARDOWN', 'TBTT_ADJUSTMENT_REQUEST', 'TBTT_ADJUSTMENT_RESPONSE'], outer_class=root_module['ns3::WifiActionHeader'], import_from_module='ns.wifi')
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::BlockAckActionValue [enumeration]
    module.add_enum('BlockAckActionValue', ['BLOCK_ACK_ADDBA_REQUEST', 'BLOCK_ACK_ADDBA_RESPONSE', 'BLOCK_ACK_DELBA'], outer_class=root_module['ns3::WifiActionHeader'], import_from_module='ns.wifi')
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::ActionValue [union]
    module.add_class('ActionValue', import_from_module='ns.wifi', outer_class=root_module['ns3::WifiActionHeader'])
    ## wifi-information-element.h (module 'wifi'): ns3::WifiInformationElement [class]
    module.add_class('WifiInformationElement', import_from_module='ns.wifi', parent=root_module['ns3::SimpleRefCount< ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >'])
    ## wifi-information-element-vector.h (module 'wifi'): ns3::WifiInformationElementVector [class]
    module.add_class('WifiInformationElementVector', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## wifi-mac.h (module 'wifi'): ns3::WifiMac [class]
    module.add_class('WifiMac', import_from_module='ns.wifi', parent=root_module['ns3::Object'])
    ## wifi-mac-header.h (module 'wifi'): ns3::WifiMacHeader [class]
    module.add_class('WifiMacHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## wifi-mac-header.h (module 'wifi'): ns3::WifiMacHeader::QosAckPolicy [enumeration]
    module.add_enum('QosAckPolicy', ['NORMAL_ACK', 'NO_ACK', 'NO_EXPLICIT_ACK', 'BLOCK_ACK'], outer_class=root_module['ns3::WifiMacHeader'], import_from_module='ns.wifi')
    ## wifi-mac-header.h (module 'wifi'): ns3::WifiMacHeader::AddressType [enumeration]
    module.add_enum('AddressType', ['ADDR1', 'ADDR2', 'ADDR3', 'ADDR4'], outer_class=root_module['ns3::WifiMacHeader'], import_from_module='ns.wifi')
    ## wifi-phy.h (module 'wifi'): ns3::WifiPhy [class]
    module.add_class('WifiPhy', import_from_module='ns.wifi', parent=root_module['ns3::Object'])
    ## wifi-phy.h (module 'wifi'): ns3::WifiPhy::State [enumeration]
    module.add_enum('State', ['IDLE', 'CCA_BUSY', 'TX', 'RX', 'SWITCHING', 'SLEEP'], outer_class=root_module['ns3::WifiPhy'], import_from_module='ns.wifi')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationManager [class]
    module.add_class('WifiRemoteStationManager', import_from_module='ns.wifi', parent=root_module['ns3::Object'])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationManager::ProtectionMode [enumeration]
    module.add_enum('ProtectionMode', ['RTS_CTS', 'CTS_TO_SELF'], outer_class=root_module['ns3::WifiRemoteStationManager'], import_from_module='ns.wifi')
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
    ## ctrl-headers.h (module 'wifi'): ns3::CtrlBAckRequestHeader [class]
    module.add_class('CtrlBAckRequestHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## ctrl-headers.h (module 'wifi'): ns3::CtrlBAckResponseHeader [class]
    module.add_class('CtrlBAckResponseHeader', import_from_module='ns.wifi', parent=root_module['ns3::Header'])
    ## dcf.h (module 'wifi'): ns3::Dcf [class]
    module.add_class('Dcf', import_from_module='ns.wifi', parent=root_module['ns3::Object'])
    ## double.h (module 'core'): ns3::DoubleValue [class]
    module.add_class('DoubleValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## edca-txop-n.h (module 'wifi'): ns3::EdcaTxopN [class]
    module.add_class('EdcaTxopN', import_from_module='ns.wifi', parent=root_module['ns3::Dcf'])
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue [class]
    module.add_class('EmptyAttributeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## enum.h (module 'core'): ns3::EnumChecker [class]
    module.add_class('EnumChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## enum.h (module 'core'): ns3::EnumValue [class]
    module.add_class('EnumValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## erp-information.h (module 'wifi'): ns3::ErpInformation [class]
    module.add_class('ErpInformation', import_from_module='ns.wifi', parent=root_module['ns3::WifiInformationElement'])
    ## erp-information.h (module 'wifi'): ns3::ErpInformationChecker [class]
    module.add_class('ErpInformationChecker', import_from_module='ns.wifi', parent=root_module['ns3::AttributeChecker'])
    ## erp-information.h (module 'wifi'): ns3::ErpInformationValue [class]
    module.add_class('ErpInformationValue', import_from_module='ns.wifi', parent=root_module['ns3::AttributeValue'])
    ## event-impl.h (module 'core'): ns3::EventImpl [class]
    module.add_class('EventImpl', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    ## supported-rates.h (module 'wifi'): ns3::ExtendedSupportedRatesIE [class]
    module.add_class('ExtendedSupportedRatesIE', import_from_module='ns.wifi', parent=root_module['ns3::WifiInformationElement'])
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilities [class]
    module.add_class('HtCapabilities', import_from_module='ns.wifi', parent=root_module['ns3::WifiInformationElement'])
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilitiesChecker [class]
    module.add_class('HtCapabilitiesChecker', import_from_module='ns.wifi', parent=root_module['ns3::AttributeChecker'])
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilitiesValue [class]
    module.add_class('HtCapabilitiesValue', import_from_module='ns.wifi', parent=root_module['ns3::AttributeValue'])
    ## ht-operations.h (module 'wifi'): ns3::HtOperations [class]
    module.add_class('HtOperations', import_from_module='ns.wifi', parent=root_module['ns3::WifiInformationElement'])
    ## ht-operations.h (module 'wifi'): ns3::HtOperationsChecker [class]
    module.add_class('HtOperationsChecker', import_from_module='ns.wifi', parent=root_module['ns3::AttributeChecker'])
    ## ht-operations.h (module 'wifi'): ns3::HtOperationsValue [class]
    module.add_class('HtOperationsValue', import_from_module='ns.wifi', parent=root_module['ns3::AttributeValue'])
    ## integer.h (module 'core'): ns3::IntegerValue [class]
    module.add_class('IntegerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressChecker [class]
    module.add_class('Ipv4AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue [class]
    module.add_class('Ipv4AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskChecker [class]
    module.add_class('Ipv4MaskChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue [class]
    module.add_class('Ipv4MaskValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressChecker [class]
    module.add_class('Ipv6AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue [class]
    module.add_class('Ipv6AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixChecker [class]
    module.add_class('Ipv6PrefixChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue [class]
    module.add_class('Ipv6PrefixValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressChecker [class]
    module.add_class('Mac48AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue [class]
    module.add_class('Mac48AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## mesh-information-element-vector.h (module 'mesh'): ns3::MeshInformationElementVector [class]
    module.add_class('MeshInformationElementVector', parent=root_module['ns3::WifiInformationElementVector'])
    ## mesh-l2-routing-protocol.h (module 'mesh'): ns3::MeshL2RoutingProtocol [class]
    module.add_class('MeshL2RoutingProtocol', parent=root_module['ns3::Object'])
    ## mesh-stack-installer.h (module 'mesh'): ns3::MeshStack [class]
    module.add_class('MeshStack', parent=root_module['ns3::Object'])
    ## mesh-wifi-interface-mac-plugin.h (module 'mesh'): ns3::MeshWifiInterfaceMacPlugin [class]
    module.add_class('MeshWifiInterfaceMacPlugin', parent=root_module['ns3::SimpleRefCount< ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >'])
    ## mgt-headers.h (module 'wifi'): ns3::MgtBeaconHeader [class]
    module.add_class('MgtBeaconHeader', import_from_module='ns.wifi', parent=root_module['ns3::MgtProbeResponseHeader'])
    ## net-device.h (module 'network'): ns3::NetDevice [class]
    module.add_class('NetDevice', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## net-device.h (module 'network'): ns3::NetDevice::PacketType [enumeration]
    module.add_enum('PacketType', ['PACKET_HOST', 'NS3_PACKET_HOST', 'PACKET_BROADCAST', 'NS3_PACKET_BROADCAST', 'PACKET_MULTICAST', 'NS3_PACKET_MULTICAST', 'PACKET_OTHERHOST', 'NS3_PACKET_OTHERHOST'], outer_class=root_module['ns3::NetDevice'], import_from_module='ns.network')
    ## net-device.h (module 'network'): ns3::NetDeviceQueue [class]
    module.add_class('NetDeviceQueue', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::NetDeviceQueue, ns3::empty, ns3::DefaultDeleter<ns3::NetDeviceQueue> >'])
    ## net-device.h (module 'network'): ns3::NetDeviceQueueInterface [class]
    module.add_class('NetDeviceQueueInterface', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## nix-vector.h (module 'network'): ns3::NixVector [class]
    module.add_class('NixVector', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    ## node.h (module 'network'): ns3::Node [class]
    module.add_class('Node', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryChecker [class]
    module.add_class('ObjectFactoryChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue [class]
    module.add_class('ObjectFactoryValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## output-stream-wrapper.h (module 'network'): ns3::OutputStreamWrapper [class]
    module.add_class('OutputStreamWrapper', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >'])
    ## packet.h (module 'network'): ns3::Packet [class]
    module.add_class('Packet', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    ## net-device.h (module 'network'): ns3::QueueItem [class]
    module.add_class('QueueItem', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::QueueItem, ns3::empty, ns3::DefaultDeleter<ns3::QueueItem> >'])
    ## regular-wifi-mac.h (module 'wifi'): ns3::RegularWifiMac [class]
    module.add_class('RegularWifiMac', import_from_module='ns.wifi', parent=root_module['ns3::WifiMac'])
    ## ssid.h (module 'wifi'): ns3::Ssid [class]
    module.add_class('Ssid', import_from_module='ns.wifi', parent=root_module['ns3::WifiInformationElement'])
    ## ssid.h (module 'wifi'): ns3::SsidChecker [class]
    module.add_class('SsidChecker', import_from_module='ns.wifi', parent=root_module['ns3::AttributeChecker'])
    ## ssid.h (module 'wifi'): ns3::SsidValue [class]
    module.add_class('SsidValue', import_from_module='ns.wifi', parent=root_module['ns3::AttributeValue'])
    ## supported-rates.h (module 'wifi'): ns3::SupportedRates [class]
    module.add_class('SupportedRates', import_from_module='ns.wifi', parent=root_module['ns3::WifiInformationElement'])
    ## nstime.h (module 'core'): ns3::TimeValue [class]
    module.add_class('TimeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## type-id.h (module 'core'): ns3::TypeIdChecker [class]
    module.add_class('TypeIdChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## type-id.h (module 'core'): ns3::TypeIdValue [class]
    module.add_class('TypeIdValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## uinteger.h (module 'core'): ns3::UintegerValue [class]
    module.add_class('UintegerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilities [class]
    module.add_class('VhtCapabilities', import_from_module='ns.wifi', parent=root_module['ns3::WifiInformationElement'])
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilitiesChecker [class]
    module.add_class('VhtCapabilitiesChecker', import_from_module='ns.wifi', parent=root_module['ns3::AttributeChecker'])
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilitiesValue [class]
    module.add_class('VhtCapabilitiesValue', import_from_module='ns.wifi', parent=root_module['ns3::AttributeValue'])
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeChecker [class]
    module.add_class('WifiModeChecker', import_from_module='ns.wifi', parent=root_module['ns3::AttributeChecker'])
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeValue [class]
    module.add_class('WifiModeValue', import_from_module='ns.wifi', parent=root_module['ns3::AttributeValue'])
    ## address.h (module 'network'): ns3::AddressChecker [class]
    module.add_class('AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## address.h (module 'network'): ns3::AddressValue [class]
    module.add_class('AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## bridge-channel.h (module 'bridge'): ns3::BridgeChannel [class]
    module.add_class('BridgeChannel', import_from_module='ns.bridge', parent=root_module['ns3::Channel'])
    ## dca-txop.h (module 'wifi'): ns3::DcaTxop [class]
    module.add_class('DcaTxop', import_from_module='ns.wifi', parent=root_module['ns3::Dcf'])
    ## dot11s-installer.h (module 'mesh'): ns3::Dot11sStack [class]
    module.add_class('Dot11sStack', parent=root_module['ns3::MeshStack'])
    ## flame-installer.h (module 'mesh'): ns3::FlameStack [class]
    module.add_class('FlameStack', parent=root_module['ns3::MeshStack'])
    ## mesh-point-device.h (module 'mesh'): ns3::MeshPointDevice [class]
    module.add_class('MeshPointDevice', parent=root_module['ns3::NetDevice'])
    ## mesh-wifi-interface-mac.h (module 'mesh'): ns3::MeshWifiInterfaceMac [class]
    module.add_class('MeshWifiInterfaceMac', parent=root_module['ns3::RegularWifiMac'])
    module.add_container('ns3::WifiModeList', 'ns3::WifiMode', container_type=u'vector')
    module.add_container('std::vector< unsigned int >', 'unsigned int', container_type=u'vector')
    module.add_container('std::map< ns3::Mac48Address, bool >', ('ns3::Mac48Address', 'bool'), container_type=u'map')
    module.add_container('std::vector< ns3::Ptr< ns3::NetDevice > >', 'ns3::Ptr< ns3::NetDevice >', container_type=u'vector')
    typehandlers.add_type_alias(u'uint8_t', u'ns3::WifiInformationElementId')
    typehandlers.add_type_alias(u'uint8_t*', u'ns3::WifiInformationElementId*')
    typehandlers.add_type_alias(u'uint8_t&', u'ns3::WifiInformationElementId&')
    typehandlers.add_type_alias(u'std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > >', u'ns3::WifiModeList')
    typehandlers.add_type_alias(u'std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > >*', u'ns3::WifiModeList*')
    typehandlers.add_type_alias(u'std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > >&', u'ns3::WifiModeList&')
    typehandlers.add_type_alias(u'__gnu_cxx::__normal_iterator< ns3::WifiMode const *, std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > > >', u'ns3::WifiModeListIterator')
    typehandlers.add_type_alias(u'__gnu_cxx::__normal_iterator< ns3::WifiMode const *, std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > > >*', u'ns3::WifiModeListIterator*')
    typehandlers.add_type_alias(u'__gnu_cxx::__normal_iterator< ns3::WifiMode const *, std::vector< ns3::WifiMode, std::allocator< ns3::WifiMode > > >&', u'ns3::WifiModeListIterator&')
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace Hash
    
    nested_module = module.add_cpp_namespace('Hash')
    register_types_ns3_Hash(nested_module)
    
    
    ## Register a nested module for the namespace TracedValueCallback
    
    nested_module = module.add_cpp_namespace('TracedValueCallback')
    register_types_ns3_TracedValueCallback(nested_module)
    
    
    ## Register a nested module for the namespace dot11s
    
    nested_module = module.add_cpp_namespace('dot11s')
    register_types_ns3_dot11s(nested_module)
    
    
    ## Register a nested module for the namespace flame
    
    nested_module = module.add_cpp_namespace('flame')
    register_types_ns3_flame(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_Hash(module):
    root_module = module.get_root()
    
    ## hash-function.h (module 'core'): ns3::Hash::Implementation [class]
    module.add_class('Implementation', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >'])
    typehandlers.add_type_alias(u'uint32_t ( * ) ( char const *, size_t ) *', u'ns3::Hash::Hash32Function_ptr')
    typehandlers.add_type_alias(u'uint32_t ( * ) ( char const *, size_t ) **', u'ns3::Hash::Hash32Function_ptr*')
    typehandlers.add_type_alias(u'uint32_t ( * ) ( char const *, size_t ) *&', u'ns3::Hash::Hash32Function_ptr&')
    typehandlers.add_type_alias(u'uint64_t ( * ) ( char const *, size_t ) *', u'ns3::Hash::Hash64Function_ptr')
    typehandlers.add_type_alias(u'uint64_t ( * ) ( char const *, size_t ) **', u'ns3::Hash::Hash64Function_ptr*')
    typehandlers.add_type_alias(u'uint64_t ( * ) ( char const *, size_t ) *&', u'ns3::Hash::Hash64Function_ptr&')
    
    ## Register a nested module for the namespace Function
    
    nested_module = module.add_cpp_namespace('Function')
    register_types_ns3_Hash_Function(nested_module)
    

def register_types_ns3_Hash_Function(module):
    root_module = module.get_root()
    
    ## hash-fnv.h (module 'core'): ns3::Hash::Function::Fnv1a [class]
    module.add_class('Fnv1a', import_from_module='ns.core', parent=root_module['ns3::Hash::Implementation'])
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash32 [class]
    module.add_class('Hash32', import_from_module='ns.core', parent=root_module['ns3::Hash::Implementation'])
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash64 [class]
    module.add_class('Hash64', import_from_module='ns.core', parent=root_module['ns3::Hash::Implementation'])
    ## hash-murmur3.h (module 'core'): ns3::Hash::Function::Murmur3 [class]
    module.add_class('Murmur3', import_from_module='ns.core', parent=root_module['ns3::Hash::Implementation'])

def register_types_ns3_TracedValueCallback(module):
    root_module = module.get_root()
    
    typehandlers.add_type_alias(u'void ( * ) ( double, double ) *', u'ns3::TracedValueCallback::Double')
    typehandlers.add_type_alias(u'void ( * ) ( double, double ) **', u'ns3::TracedValueCallback::Double*')
    typehandlers.add_type_alias(u'void ( * ) ( double, double ) *&', u'ns3::TracedValueCallback::Double&')
    typehandlers.add_type_alias(u'void ( * ) ( uint8_t, uint8_t ) *', u'ns3::TracedValueCallback::Uint8')
    typehandlers.add_type_alias(u'void ( * ) ( uint8_t, uint8_t ) **', u'ns3::TracedValueCallback::Uint8*')
    typehandlers.add_type_alias(u'void ( * ) ( uint8_t, uint8_t ) *&', u'ns3::TracedValueCallback::Uint8&')
    typehandlers.add_type_alias(u'void ( * ) ( int32_t, int32_t ) *', u'ns3::TracedValueCallback::Int32')
    typehandlers.add_type_alias(u'void ( * ) ( int32_t, int32_t ) **', u'ns3::TracedValueCallback::Int32*')
    typehandlers.add_type_alias(u'void ( * ) ( int32_t, int32_t ) *&', u'ns3::TracedValueCallback::Int32&')
    typehandlers.add_type_alias(u'void ( * ) ( bool, bool ) *', u'ns3::TracedValueCallback::Bool')
    typehandlers.add_type_alias(u'void ( * ) ( bool, bool ) **', u'ns3::TracedValueCallback::Bool*')
    typehandlers.add_type_alias(u'void ( * ) ( bool, bool ) *&', u'ns3::TracedValueCallback::Bool&')
    typehandlers.add_type_alias(u'void ( * ) ( int8_t, int8_t ) *', u'ns3::TracedValueCallback::Int8')
    typehandlers.add_type_alias(u'void ( * ) ( int8_t, int8_t ) **', u'ns3::TracedValueCallback::Int8*')
    typehandlers.add_type_alias(u'void ( * ) ( int8_t, int8_t ) *&', u'ns3::TracedValueCallback::Int8&')
    typehandlers.add_type_alias(u'void ( * ) ( uint16_t, uint16_t ) *', u'ns3::TracedValueCallback::Uint16')
    typehandlers.add_type_alias(u'void ( * ) ( uint16_t, uint16_t ) **', u'ns3::TracedValueCallback::Uint16*')
    typehandlers.add_type_alias(u'void ( * ) ( uint16_t, uint16_t ) *&', u'ns3::TracedValueCallback::Uint16&')
    typehandlers.add_type_alias(u'void ( * ) ( uint32_t, uint32_t ) *', u'ns3::TracedValueCallback::Uint32')
    typehandlers.add_type_alias(u'void ( * ) ( uint32_t, uint32_t ) **', u'ns3::TracedValueCallback::Uint32*')
    typehandlers.add_type_alias(u'void ( * ) ( uint32_t, uint32_t ) *&', u'ns3::TracedValueCallback::Uint32&')
    typehandlers.add_type_alias(u'void ( * ) ( int16_t, int16_t ) *', u'ns3::TracedValueCallback::Int16')
    typehandlers.add_type_alias(u'void ( * ) ( int16_t, int16_t ) **', u'ns3::TracedValueCallback::Int16*')
    typehandlers.add_type_alias(u'void ( * ) ( int16_t, int16_t ) *&', u'ns3::TracedValueCallback::Int16&')
    typehandlers.add_type_alias(u'void ( * ) ( ns3::Time, ns3::Time ) *', u'ns3::TracedValueCallback::Time')
    typehandlers.add_type_alias(u'void ( * ) ( ns3::Time, ns3::Time ) **', u'ns3::TracedValueCallback::Time*')
    typehandlers.add_type_alias(u'void ( * ) ( ns3::Time, ns3::Time ) *&', u'ns3::TracedValueCallback::Time&')

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::dot11sSynchronizationProtocolIdentifier [enumeration]
    module.add_enum('dot11sSynchronizationProtocolIdentifier', ['SYNC_NEIGHBOUR_OFFSET', 'SYNC_NULL'])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::dot11sCongestionControlMode [enumeration]
    module.add_enum('dot11sCongestionControlMode', ['CONGESTION_SIGNALING', 'CONGESTION_NULL'])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::dot11sAuthenticationProtocol [enumeration]
    module.add_enum('dot11sAuthenticationProtocol', ['AUTH_NULL', 'AUTH_SAE', 'AUTH_IEEE'])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::dot11sPathSelectionMetric [enumeration]
    module.add_enum('dot11sPathSelectionMetric', ['METRIC_AIRTIME'])
    ## ie-dot11s-peer-management.h (module 'mesh'): ns3::dot11s::PmpReasonCode [enumeration]
    module.add_enum('PmpReasonCode', ['REASON11S_PEERING_CANCELLED', 'REASON11S_MESH_MAX_PEERS', 'REASON11S_MESH_CAPABILITY_POLICY_VIOLATION', 'REASON11S_MESH_CLOSE_RCVD', 'REASON11S_MESH_MAX_RETRIES', 'REASON11S_MESH_CONFIRM_TIMEOUT', 'REASON11S_MESH_INVALID_GTK', 'REASON11S_MESH_INCONSISTENT_PARAMETERS', 'REASON11S_MESH_INVALID_SECURITY_CAPABILITY', 'REASON11S_RESERVED'])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::dot11sPathSelectionProtocol [enumeration]
    module.add_enum('dot11sPathSelectionProtocol', ['PROTOCOL_HWMP'])
    ## ie-dot11s-preq.h (module 'mesh'): ns3::dot11s::DestinationAddressUnit [class]
    module.add_class('DestinationAddressUnit', parent=root_module['ns3::SimpleRefCount< ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> >'])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability [class]
    module.add_class('Dot11sMeshCapability')
    ## hwmp-protocol.h (module 'mesh'): ns3::dot11s::HwmpProtocol [class]
    module.add_class('HwmpProtocol', parent=root_module['ns3::MeshL2RoutingProtocol'])
    ## hwmp-protocol.h (module 'mesh'): ns3::dot11s::HwmpProtocol::FailedDestination [struct]
    module.add_class('FailedDestination', outer_class=root_module['ns3::dot11s::HwmpProtocol'])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable [class]
    module.add_class('HwmpRtable', parent=root_module['ns3::Object'])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult [struct]
    module.add_class('LookupResult', outer_class=root_module['ns3::dot11s::HwmpRtable'])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): ns3::dot11s::IeBeaconTiming [class]
    module.add_class('IeBeaconTiming', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): ns3::dot11s::IeBeaconTimingUnit [class]
    module.add_class('IeBeaconTimingUnit', parent=root_module['ns3::SimpleRefCount< ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >'])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::IeConfiguration [class]
    module.add_class('IeConfiguration', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-metric-report.h (module 'mesh'): ns3::dot11s::IeLinkMetricReport [class]
    module.add_class('IeLinkMetricReport', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshId [class]
    module.add_class('IeMeshId', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshIdChecker [class]
    module.add_class('IeMeshIdChecker', parent=root_module['ns3::AttributeChecker'])
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshIdValue [class]
    module.add_class('IeMeshIdValue', parent=root_module['ns3::AttributeValue'])
    ## ie-dot11s-peer-management.h (module 'mesh'): ns3::dot11s::IePeerManagement [class]
    module.add_class('IePeerManagement', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-peer-management.h (module 'mesh'): ns3::dot11s::IePeerManagement::Subtype [enumeration]
    module.add_enum('Subtype', ['PEER_OPEN', 'PEER_CONFIRM', 'PEER_CLOSE'], outer_class=root_module['ns3::dot11s::IePeerManagement'])
    ## ie-dot11s-peering-protocol.h (module 'mesh'): ns3::dot11s::IePeeringProtocol [class]
    module.add_class('IePeeringProtocol', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-perr.h (module 'mesh'): ns3::dot11s::IePerr [class]
    module.add_class('IePerr', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-prep.h (module 'mesh'): ns3::dot11s::IePrep [class]
    module.add_class('IePrep', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-preq.h (module 'mesh'): ns3::dot11s::IePreq [class]
    module.add_class('IePreq', parent=root_module['ns3::WifiInformationElement'])
    ## ie-dot11s-rann.h (module 'mesh'): ns3::dot11s::IeRann [class]
    module.add_class('IeRann', parent=root_module['ns3::WifiInformationElement'])
    ## dot11s-mac-header.h (module 'mesh'): ns3::dot11s::MeshHeader [class]
    module.add_class('MeshHeader', parent=root_module['ns3::Header'])
    ## peer-link.h (module 'mesh'): ns3::dot11s::PeerLink [class]
    module.add_class('PeerLink', parent=root_module['ns3::Object'])
    ## peer-link.h (module 'mesh'): ns3::dot11s::PeerLink::PeerState [enumeration]
    module.add_enum('PeerState', ['IDLE', 'OPN_SNT', 'CNF_RCVD', 'OPN_RCVD', 'ESTAB', 'HOLDING'], outer_class=root_module['ns3::dot11s::PeerLink'])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkCloseStart [class]
    module.add_class('PeerLinkCloseStart', parent=root_module['ns3::Header'])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields [struct]
    module.add_class('PlinkCloseStartFields', outer_class=root_module['ns3::dot11s::PeerLinkCloseStart'])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart [class]
    module.add_class('PeerLinkConfirmStart', parent=root_module['ns3::Header'])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields [struct]
    module.add_class('PlinkConfirmStartFields', outer_class=root_module['ns3::dot11s::PeerLinkConfirmStart'])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart [class]
    module.add_class('PeerLinkOpenStart', parent=root_module['ns3::Header'])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields [struct]
    module.add_class('PlinkOpenStartFields', outer_class=root_module['ns3::dot11s::PeerLinkOpenStart'])
    ## peer-management-protocol.h (module 'mesh'): ns3::dot11s::PeerManagementProtocol [class]
    module.add_class('PeerManagementProtocol', parent=root_module['ns3::Object'])
    module.add_container('std::vector< std::pair< unsigned int, ns3::Mac48Address > >', 'std::pair< unsigned int, ns3::Mac48Address >', container_type=u'vector')
    module.add_container('std::vector< ns3::dot11s::HwmpProtocol::FailedDestination >', 'ns3::dot11s::HwmpProtocol::FailedDestination', container_type=u'vector')
    module.add_container('std::vector< ns3::Ptr< ns3::dot11s::IeBeaconTimingUnit > >', 'ns3::Ptr< ns3::dot11s::IeBeaconTimingUnit >', container_type=u'vector')
    module.add_container('std::vector< ns3::Ptr< ns3::dot11s::DestinationAddressUnit > >', 'ns3::Ptr< ns3::dot11s::DestinationAddressUnit >', container_type=u'vector')
    module.add_container('std::vector< ns3::Ptr< ns3::dot11s::PeerLink > >', 'ns3::Ptr< ns3::dot11s::PeerLink >', container_type=u'vector')
    module.add_container('std::vector< ns3::Mac48Address >', 'ns3::Mac48Address', container_type=u'vector')

def register_types_ns3_flame(module):
    root_module = module.get_root()
    
    ## flame-header.h (module 'mesh'): ns3::flame::FlameHeader [class]
    module.add_class('FlameHeader', parent=root_module['ns3::Header'])
    ## flame-protocol.h (module 'mesh'): ns3::flame::FlameProtocol [class]
    module.add_class('FlameProtocol', parent=root_module['ns3::MeshL2RoutingProtocol'])
    ## flame-protocol-mac.h (module 'mesh'): ns3::flame::FlameProtocolMac [class]
    module.add_class('FlameProtocolMac', parent=root_module['ns3::MeshWifiInterfaceMacPlugin'])
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable [class]
    module.add_class('FlameRtable', parent=root_module['ns3::Object'])
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::LookupResult [struct]
    module.add_class('LookupResult', outer_class=root_module['ns3::flame::FlameRtable'])
    ## flame-protocol.h (module 'mesh'): ns3::flame::FlameTag [class]
    module.add_class('FlameTag', parent=root_module['ns3::Tag'])

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Address_methods(root_module, root_module['ns3::Address'])
    register_Ns3AsciiTraceHelper_methods(root_module, root_module['ns3::AsciiTraceHelper'])
    register_Ns3AsciiTraceHelperForDevice_methods(root_module, root_module['ns3::AsciiTraceHelperForDevice'])
    register_Ns3AttributeConstructionList_methods(root_module, root_module['ns3::AttributeConstructionList'])
    register_Ns3AttributeConstructionListItem_methods(root_module, root_module['ns3::AttributeConstructionList::Item'])
    register_Ns3Bar_methods(root_module, root_module['ns3::Bar'])
    register_Ns3BlockAckAgreement_methods(root_module, root_module['ns3::BlockAckAgreement'])
    register_Ns3BlockAckManager_methods(root_module, root_module['ns3::BlockAckManager'])
    register_Ns3Buffer_methods(root_module, root_module['ns3::Buffer'])
    register_Ns3BufferIterator_methods(root_module, root_module['ns3::Buffer::Iterator'])
    register_Ns3ByteTagIterator_methods(root_module, root_module['ns3::ByteTagIterator'])
    register_Ns3ByteTagIteratorItem_methods(root_module, root_module['ns3::ByteTagIterator::Item'])
    register_Ns3ByteTagList_methods(root_module, root_module['ns3::ByteTagList'])
    register_Ns3ByteTagListIterator_methods(root_module, root_module['ns3::ByteTagList::Iterator'])
    register_Ns3ByteTagListIteratorItem_methods(root_module, root_module['ns3::ByteTagList::Iterator::Item'])
    register_Ns3CallbackBase_methods(root_module, root_module['ns3::CallbackBase'])
    register_Ns3CapabilityInformation_methods(root_module, root_module['ns3::CapabilityInformation'])
    register_Ns3EventId_methods(root_module, root_module['ns3::EventId'])
    register_Ns3Hasher_methods(root_module, root_module['ns3::Hasher'])
    register_Ns3Ipv4Address_methods(root_module, root_module['ns3::Ipv4Address'])
    register_Ns3Ipv4Mask_methods(root_module, root_module['ns3::Ipv4Mask'])
    register_Ns3Ipv6Address_methods(root_module, root_module['ns3::Ipv6Address'])
    register_Ns3Ipv6Prefix_methods(root_module, root_module['ns3::Ipv6Prefix'])
    register_Ns3Mac48Address_methods(root_module, root_module['ns3::Mac48Address'])
    register_Ns3MeshHelper_methods(root_module, root_module['ns3::MeshHelper'])
    register_Ns3MeshWifiBeacon_methods(root_module, root_module['ns3::MeshWifiBeacon'])
    register_Ns3NetDeviceContainer_methods(root_module, root_module['ns3::NetDeviceContainer'])
    register_Ns3NodeContainer_methods(root_module, root_module['ns3::NodeContainer'])
    register_Ns3ObjectBase_methods(root_module, root_module['ns3::ObjectBase'])
    register_Ns3ObjectDeleter_methods(root_module, root_module['ns3::ObjectDeleter'])
    register_Ns3ObjectFactory_methods(root_module, root_module['ns3::ObjectFactory'])
    register_Ns3OriginatorBlockAckAgreement_methods(root_module, root_module['ns3::OriginatorBlockAckAgreement'])
    register_Ns3PacketMetadata_methods(root_module, root_module['ns3::PacketMetadata'])
    register_Ns3PacketMetadataItem_methods(root_module, root_module['ns3::PacketMetadata::Item'])
    register_Ns3PacketMetadataItemIterator_methods(root_module, root_module['ns3::PacketMetadata::ItemIterator'])
    register_Ns3PacketTagIterator_methods(root_module, root_module['ns3::PacketTagIterator'])
    register_Ns3PacketTagIteratorItem_methods(root_module, root_module['ns3::PacketTagIterator::Item'])
    register_Ns3PacketTagList_methods(root_module, root_module['ns3::PacketTagList'])
    register_Ns3PacketTagListTagData_methods(root_module, root_module['ns3::PacketTagList::TagData'])
    register_Ns3PcapFile_methods(root_module, root_module['ns3::PcapFile'])
    register_Ns3PcapHelper_methods(root_module, root_module['ns3::PcapHelper'])
    register_Ns3PcapHelperForDevice_methods(root_module, root_module['ns3::PcapHelperForDevice'])
    register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    register_Ns3Simulator_methods(root_module, root_module['ns3::Simulator'])
    register_Ns3StatusCode_methods(root_module, root_module['ns3::StatusCode'])
    register_Ns3Tag_methods(root_module, root_module['ns3::Tag'])
    register_Ns3TagBuffer_methods(root_module, root_module['ns3::TagBuffer'])
    register_Ns3TimeWithUnit_methods(root_module, root_module['ns3::TimeWithUnit'])
    register_Ns3TypeId_methods(root_module, root_module['ns3::TypeId'])
    register_Ns3TypeIdAttributeInformation_methods(root_module, root_module['ns3::TypeId::AttributeInformation'])
    register_Ns3TypeIdTraceSourceInformation_methods(root_module, root_module['ns3::TypeId::TraceSourceInformation'])
    register_Ns3WifiHelper_methods(root_module, root_module['ns3::WifiHelper'])
    register_Ns3WifiMacHelper_methods(root_module, root_module['ns3::WifiMacHelper'])
    register_Ns3WifiMode_methods(root_module, root_module['ns3::WifiMode'])
    register_Ns3WifiModeFactory_methods(root_module, root_module['ns3::WifiModeFactory'])
    register_Ns3WifiPhyHelper_methods(root_module, root_module['ns3::WifiPhyHelper'])
    register_Ns3WifiPhyListener_methods(root_module, root_module['ns3::WifiPhyListener'])
    register_Ns3WifiRemoteStation_methods(root_module, root_module['ns3::WifiRemoteStation'])
    register_Ns3WifiRemoteStationInfo_methods(root_module, root_module['ns3::WifiRemoteStationInfo'])
    register_Ns3WifiRemoteStationState_methods(root_module, root_module['ns3::WifiRemoteStationState'])
    register_Ns3WifiTxVector_methods(root_module, root_module['ns3::WifiTxVector'])
    register_Ns3Empty_methods(root_module, root_module['ns3::empty'])
    register_Ns3Int64x64_t_methods(root_module, root_module['ns3::int64x64_t'])
    register_Ns3MpduInfo_methods(root_module, root_module['ns3::mpduInfo'])
    register_Ns3SignalNoiseDbm_methods(root_module, root_module['ns3::signalNoiseDbm'])
    register_Ns3Chunk_methods(root_module, root_module['ns3::Chunk'])
    register_Ns3Header_methods(root_module, root_module['ns3::Header'])
    register_Ns3MgtAddBaRequestHeader_methods(root_module, root_module['ns3::MgtAddBaRequestHeader'])
    register_Ns3MgtAddBaResponseHeader_methods(root_module, root_module['ns3::MgtAddBaResponseHeader'])
    register_Ns3MgtAssocRequestHeader_methods(root_module, root_module['ns3::MgtAssocRequestHeader'])
    register_Ns3MgtAssocResponseHeader_methods(root_module, root_module['ns3::MgtAssocResponseHeader'])
    register_Ns3MgtDelBaHeader_methods(root_module, root_module['ns3::MgtDelBaHeader'])
    register_Ns3MgtProbeRequestHeader_methods(root_module, root_module['ns3::MgtProbeRequestHeader'])
    register_Ns3MgtProbeResponseHeader_methods(root_module, root_module['ns3::MgtProbeResponseHeader'])
    register_Ns3Object_methods(root_module, root_module['ns3::Object'])
    register_Ns3ObjectAggregateIterator_methods(root_module, root_module['ns3::Object::AggregateIterator'])
    register_Ns3PcapFileWrapper_methods(root_module, root_module['ns3::PcapFileWrapper'])
    register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    register_Ns3SimpleRefCount__Ns3HashImplementation_Ns3Empty_Ns3DefaultDeleter__lt__ns3HashImplementation__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >'])
    register_Ns3SimpleRefCount__Ns3MeshWifiInterfaceMacPlugin_Ns3Empty_Ns3DefaultDeleter__lt__ns3MeshWifiInterfaceMacPlugin__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >'])
    register_Ns3SimpleRefCount__Ns3NetDeviceQueue_Ns3Empty_Ns3DefaultDeleter__lt__ns3NetDeviceQueue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::NetDeviceQueue, ns3::empty, ns3::DefaultDeleter<ns3::NetDeviceQueue> >'])
    register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    register_Ns3SimpleRefCount__Ns3OutputStreamWrapper_Ns3Empty_Ns3DefaultDeleter__lt__ns3OutputStreamWrapper__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >'])
    register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    register_Ns3SimpleRefCount__Ns3QueueItem_Ns3Empty_Ns3DefaultDeleter__lt__ns3QueueItem__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::QueueItem, ns3::empty, ns3::DefaultDeleter<ns3::QueueItem> >'])
    register_Ns3SimpleRefCount__Ns3TraceSourceAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3TraceSourceAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    register_Ns3SimpleRefCount__Ns3WifiInformationElement_Ns3Empty_Ns3DefaultDeleter__lt__ns3WifiInformationElement__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >'])
    register_Ns3SimpleRefCount__Ns3Dot11sDestinationAddressUnit_Ns3Empty_Ns3DefaultDeleter__lt__ns3Dot11sDestinationAddressUnit__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> >'])
    register_Ns3SimpleRefCount__Ns3Dot11sIeBeaconTimingUnit_Ns3Empty_Ns3DefaultDeleter__lt__ns3Dot11sIeBeaconTimingUnit__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >'])
    register_Ns3Time_methods(root_module, root_module['ns3::Time'])
    register_Ns3TraceSourceAccessor_methods(root_module, root_module['ns3::TraceSourceAccessor'])
    register_Ns3Trailer_methods(root_module, root_module['ns3::Trailer'])
    register_Ns3WifiActionHeader_methods(root_module, root_module['ns3::WifiActionHeader'])
    register_Ns3WifiActionHeaderActionValue_methods(root_module, root_module['ns3::WifiActionHeader::ActionValue'])
    register_Ns3WifiInformationElement_methods(root_module, root_module['ns3::WifiInformationElement'])
    register_Ns3WifiInformationElementVector_methods(root_module, root_module['ns3::WifiInformationElementVector'])
    register_Ns3WifiMac_methods(root_module, root_module['ns3::WifiMac'])
    register_Ns3WifiMacHeader_methods(root_module, root_module['ns3::WifiMacHeader'])
    register_Ns3WifiPhy_methods(root_module, root_module['ns3::WifiPhy'])
    register_Ns3WifiRemoteStationManager_methods(root_module, root_module['ns3::WifiRemoteStationManager'])
    register_Ns3AttributeAccessor_methods(root_module, root_module['ns3::AttributeAccessor'])
    register_Ns3AttributeChecker_methods(root_module, root_module['ns3::AttributeChecker'])
    register_Ns3AttributeValue_methods(root_module, root_module['ns3::AttributeValue'])
    register_Ns3BooleanChecker_methods(root_module, root_module['ns3::BooleanChecker'])
    register_Ns3BooleanValue_methods(root_module, root_module['ns3::BooleanValue'])
    register_Ns3CallbackChecker_methods(root_module, root_module['ns3::CallbackChecker'])
    register_Ns3CallbackImplBase_methods(root_module, root_module['ns3::CallbackImplBase'])
    register_Ns3CallbackValue_methods(root_module, root_module['ns3::CallbackValue'])
    register_Ns3Channel_methods(root_module, root_module['ns3::Channel'])
    register_Ns3CtrlBAckRequestHeader_methods(root_module, root_module['ns3::CtrlBAckRequestHeader'])
    register_Ns3CtrlBAckResponseHeader_methods(root_module, root_module['ns3::CtrlBAckResponseHeader'])
    register_Ns3Dcf_methods(root_module, root_module['ns3::Dcf'])
    register_Ns3DoubleValue_methods(root_module, root_module['ns3::DoubleValue'])
    register_Ns3EdcaTxopN_methods(root_module, root_module['ns3::EdcaTxopN'])
    register_Ns3EmptyAttributeValue_methods(root_module, root_module['ns3::EmptyAttributeValue'])
    register_Ns3EnumChecker_methods(root_module, root_module['ns3::EnumChecker'])
    register_Ns3EnumValue_methods(root_module, root_module['ns3::EnumValue'])
    register_Ns3ErpInformation_methods(root_module, root_module['ns3::ErpInformation'])
    register_Ns3ErpInformationChecker_methods(root_module, root_module['ns3::ErpInformationChecker'])
    register_Ns3ErpInformationValue_methods(root_module, root_module['ns3::ErpInformationValue'])
    register_Ns3EventImpl_methods(root_module, root_module['ns3::EventImpl'])
    register_Ns3ExtendedSupportedRatesIE_methods(root_module, root_module['ns3::ExtendedSupportedRatesIE'])
    register_Ns3HtCapabilities_methods(root_module, root_module['ns3::HtCapabilities'])
    register_Ns3HtCapabilitiesChecker_methods(root_module, root_module['ns3::HtCapabilitiesChecker'])
    register_Ns3HtCapabilitiesValue_methods(root_module, root_module['ns3::HtCapabilitiesValue'])
    register_Ns3HtOperations_methods(root_module, root_module['ns3::HtOperations'])
    register_Ns3HtOperationsChecker_methods(root_module, root_module['ns3::HtOperationsChecker'])
    register_Ns3HtOperationsValue_methods(root_module, root_module['ns3::HtOperationsValue'])
    register_Ns3IntegerValue_methods(root_module, root_module['ns3::IntegerValue'])
    register_Ns3Ipv4AddressChecker_methods(root_module, root_module['ns3::Ipv4AddressChecker'])
    register_Ns3Ipv4AddressValue_methods(root_module, root_module['ns3::Ipv4AddressValue'])
    register_Ns3Ipv4MaskChecker_methods(root_module, root_module['ns3::Ipv4MaskChecker'])
    register_Ns3Ipv4MaskValue_methods(root_module, root_module['ns3::Ipv4MaskValue'])
    register_Ns3Ipv6AddressChecker_methods(root_module, root_module['ns3::Ipv6AddressChecker'])
    register_Ns3Ipv6AddressValue_methods(root_module, root_module['ns3::Ipv6AddressValue'])
    register_Ns3Ipv6PrefixChecker_methods(root_module, root_module['ns3::Ipv6PrefixChecker'])
    register_Ns3Ipv6PrefixValue_methods(root_module, root_module['ns3::Ipv6PrefixValue'])
    register_Ns3Mac48AddressChecker_methods(root_module, root_module['ns3::Mac48AddressChecker'])
    register_Ns3Mac48AddressValue_methods(root_module, root_module['ns3::Mac48AddressValue'])
    register_Ns3MeshInformationElementVector_methods(root_module, root_module['ns3::MeshInformationElementVector'])
    register_Ns3MeshL2RoutingProtocol_methods(root_module, root_module['ns3::MeshL2RoutingProtocol'])
    register_Ns3MeshStack_methods(root_module, root_module['ns3::MeshStack'])
    register_Ns3MeshWifiInterfaceMacPlugin_methods(root_module, root_module['ns3::MeshWifiInterfaceMacPlugin'])
    register_Ns3MgtBeaconHeader_methods(root_module, root_module['ns3::MgtBeaconHeader'])
    register_Ns3NetDevice_methods(root_module, root_module['ns3::NetDevice'])
    register_Ns3NetDeviceQueue_methods(root_module, root_module['ns3::NetDeviceQueue'])
    register_Ns3NetDeviceQueueInterface_methods(root_module, root_module['ns3::NetDeviceQueueInterface'])
    register_Ns3NixVector_methods(root_module, root_module['ns3::NixVector'])
    register_Ns3Node_methods(root_module, root_module['ns3::Node'])
    register_Ns3ObjectFactoryChecker_methods(root_module, root_module['ns3::ObjectFactoryChecker'])
    register_Ns3ObjectFactoryValue_methods(root_module, root_module['ns3::ObjectFactoryValue'])
    register_Ns3OutputStreamWrapper_methods(root_module, root_module['ns3::OutputStreamWrapper'])
    register_Ns3Packet_methods(root_module, root_module['ns3::Packet'])
    register_Ns3QueueItem_methods(root_module, root_module['ns3::QueueItem'])
    register_Ns3RegularWifiMac_methods(root_module, root_module['ns3::RegularWifiMac'])
    register_Ns3Ssid_methods(root_module, root_module['ns3::Ssid'])
    register_Ns3SsidChecker_methods(root_module, root_module['ns3::SsidChecker'])
    register_Ns3SsidValue_methods(root_module, root_module['ns3::SsidValue'])
    register_Ns3SupportedRates_methods(root_module, root_module['ns3::SupportedRates'])
    register_Ns3TimeValue_methods(root_module, root_module['ns3::TimeValue'])
    register_Ns3TypeIdChecker_methods(root_module, root_module['ns3::TypeIdChecker'])
    register_Ns3TypeIdValue_methods(root_module, root_module['ns3::TypeIdValue'])
    register_Ns3UintegerValue_methods(root_module, root_module['ns3::UintegerValue'])
    register_Ns3VhtCapabilities_methods(root_module, root_module['ns3::VhtCapabilities'])
    register_Ns3VhtCapabilitiesChecker_methods(root_module, root_module['ns3::VhtCapabilitiesChecker'])
    register_Ns3VhtCapabilitiesValue_methods(root_module, root_module['ns3::VhtCapabilitiesValue'])
    register_Ns3WifiModeChecker_methods(root_module, root_module['ns3::WifiModeChecker'])
    register_Ns3WifiModeValue_methods(root_module, root_module['ns3::WifiModeValue'])
    register_Ns3AddressChecker_methods(root_module, root_module['ns3::AddressChecker'])
    register_Ns3AddressValue_methods(root_module, root_module['ns3::AddressValue'])
    register_Ns3BridgeChannel_methods(root_module, root_module['ns3::BridgeChannel'])
    register_Ns3DcaTxop_methods(root_module, root_module['ns3::DcaTxop'])
    register_Ns3Dot11sStack_methods(root_module, root_module['ns3::Dot11sStack'])
    register_Ns3FlameStack_methods(root_module, root_module['ns3::FlameStack'])
    register_Ns3MeshPointDevice_methods(root_module, root_module['ns3::MeshPointDevice'])
    register_Ns3MeshWifiInterfaceMac_methods(root_module, root_module['ns3::MeshWifiInterfaceMac'])
    register_Ns3HashImplementation_methods(root_module, root_module['ns3::Hash::Implementation'])
    register_Ns3HashFunctionFnv1a_methods(root_module, root_module['ns3::Hash::Function::Fnv1a'])
    register_Ns3HashFunctionHash32_methods(root_module, root_module['ns3::Hash::Function::Hash32'])
    register_Ns3HashFunctionHash64_methods(root_module, root_module['ns3::Hash::Function::Hash64'])
    register_Ns3HashFunctionMurmur3_methods(root_module, root_module['ns3::Hash::Function::Murmur3'])
    register_Ns3Dot11sDestinationAddressUnit_methods(root_module, root_module['ns3::dot11s::DestinationAddressUnit'])
    register_Ns3Dot11sDot11sMeshCapability_methods(root_module, root_module['ns3::dot11s::Dot11sMeshCapability'])
    register_Ns3Dot11sHwmpProtocol_methods(root_module, root_module['ns3::dot11s::HwmpProtocol'])
    register_Ns3Dot11sHwmpProtocolFailedDestination_methods(root_module, root_module['ns3::dot11s::HwmpProtocol::FailedDestination'])
    register_Ns3Dot11sHwmpRtable_methods(root_module, root_module['ns3::dot11s::HwmpRtable'])
    register_Ns3Dot11sHwmpRtableLookupResult_methods(root_module, root_module['ns3::dot11s::HwmpRtable::LookupResult'])
    register_Ns3Dot11sIeBeaconTiming_methods(root_module, root_module['ns3::dot11s::IeBeaconTiming'])
    register_Ns3Dot11sIeBeaconTimingUnit_methods(root_module, root_module['ns3::dot11s::IeBeaconTimingUnit'])
    register_Ns3Dot11sIeConfiguration_methods(root_module, root_module['ns3::dot11s::IeConfiguration'])
    register_Ns3Dot11sIeLinkMetricReport_methods(root_module, root_module['ns3::dot11s::IeLinkMetricReport'])
    register_Ns3Dot11sIeMeshId_methods(root_module, root_module['ns3::dot11s::IeMeshId'])
    register_Ns3Dot11sIeMeshIdChecker_methods(root_module, root_module['ns3::dot11s::IeMeshIdChecker'])
    register_Ns3Dot11sIeMeshIdValue_methods(root_module, root_module['ns3::dot11s::IeMeshIdValue'])
    register_Ns3Dot11sIePeerManagement_methods(root_module, root_module['ns3::dot11s::IePeerManagement'])
    register_Ns3Dot11sIePeeringProtocol_methods(root_module, root_module['ns3::dot11s::IePeeringProtocol'])
    register_Ns3Dot11sIePerr_methods(root_module, root_module['ns3::dot11s::IePerr'])
    register_Ns3Dot11sIePrep_methods(root_module, root_module['ns3::dot11s::IePrep'])
    register_Ns3Dot11sIePreq_methods(root_module, root_module['ns3::dot11s::IePreq'])
    register_Ns3Dot11sIeRann_methods(root_module, root_module['ns3::dot11s::IeRann'])
    register_Ns3Dot11sMeshHeader_methods(root_module, root_module['ns3::dot11s::MeshHeader'])
    register_Ns3Dot11sPeerLink_methods(root_module, root_module['ns3::dot11s::PeerLink'])
    register_Ns3Dot11sPeerLinkCloseStart_methods(root_module, root_module['ns3::dot11s::PeerLinkCloseStart'])
    register_Ns3Dot11sPeerLinkCloseStartPlinkCloseStartFields_methods(root_module, root_module['ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields'])
    register_Ns3Dot11sPeerLinkConfirmStart_methods(root_module, root_module['ns3::dot11s::PeerLinkConfirmStart'])
    register_Ns3Dot11sPeerLinkConfirmStartPlinkConfirmStartFields_methods(root_module, root_module['ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields'])
    register_Ns3Dot11sPeerLinkOpenStart_methods(root_module, root_module['ns3::dot11s::PeerLinkOpenStart'])
    register_Ns3Dot11sPeerLinkOpenStartPlinkOpenStartFields_methods(root_module, root_module['ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields'])
    register_Ns3Dot11sPeerManagementProtocol_methods(root_module, root_module['ns3::dot11s::PeerManagementProtocol'])
    register_Ns3FlameFlameHeader_methods(root_module, root_module['ns3::flame::FlameHeader'])
    register_Ns3FlameFlameProtocol_methods(root_module, root_module['ns3::flame::FlameProtocol'])
    register_Ns3FlameFlameProtocolMac_methods(root_module, root_module['ns3::flame::FlameProtocolMac'])
    register_Ns3FlameFlameRtable_methods(root_module, root_module['ns3::flame::FlameRtable'])
    register_Ns3FlameFlameRtableLookupResult_methods(root_module, root_module['ns3::flame::FlameRtable::LookupResult'])
    register_Ns3FlameFlameTag_methods(root_module, root_module['ns3::flame::FlameTag'])
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

def register_Ns3AsciiTraceHelper_methods(root_module, cls):
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelper::AsciiTraceHelper(ns3::AsciiTraceHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AsciiTraceHelper const &', 'arg0')])
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelper::AsciiTraceHelper() [constructor]
    cls.add_constructor([])
    ## trace-helper.h (module 'network'): ns3::Ptr<ns3::OutputStreamWrapper> ns3::AsciiTraceHelper::CreateFileStream(std::string filename, std::_Ios_Openmode filemode=std::ios_base::out) [member function]
    cls.add_method('CreateFileStream', 
                   'ns3::Ptr< ns3::OutputStreamWrapper >', 
                   [param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode', default_value='std::ios_base::out')])
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultDequeueSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDequeueSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultDequeueSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDequeueSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultDropSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDropSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultDropSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDropSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultEnqueueSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultEnqueueSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultEnqueueSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultEnqueueSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultReceiveSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultReceiveSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultReceiveSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultReceiveSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): std::string ns3::AsciiTraceHelper::GetFilenameFromDevice(std::string prefix, ns3::Ptr<ns3::NetDevice> device, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromDevice', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'useObjectNames', default_value='true')])
    ## trace-helper.h (module 'network'): std::string ns3::AsciiTraceHelper::GetFilenameFromInterfacePair(std::string prefix, ns3::Ptr<ns3::Object> object, uint32_t interface, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromInterfacePair', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Object >', 'object'), param('uint32_t', 'interface'), param('bool', 'useObjectNames', default_value='true')])
    return

def register_Ns3AsciiTraceHelperForDevice_methods(root_module, cls):
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelperForDevice::AsciiTraceHelperForDevice(ns3::AsciiTraceHelperForDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AsciiTraceHelperForDevice const &', 'arg0')])
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelperForDevice::AsciiTraceHelperForDevice() [constructor]
    cls.add_constructor([])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename=false) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, std::string ndName, bool explicitFilename=false) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('std::string', 'ndName'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string ndName) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'ndName')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NetDeviceContainer', 'd')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::NetDeviceContainer', 'd')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NodeContainer', 'n')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::NodeContainer', 'n')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, uint32_t nodeid, uint32_t deviceid, bool explicitFilename) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('bool', 'explicitFilename')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAsciiAll(std::string prefix) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::string', 'prefix')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAsciiAll(ns3::Ptr<ns3::OutputStreamWrapper> stream) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   is_pure_virtual=True, is_virtual=True)
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

def register_Ns3Bar_methods(root_module, cls):
    ## block-ack-manager.h (module 'wifi'): ns3::Bar::Bar(ns3::Bar const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Bar const &', 'arg0')])
    ## block-ack-manager.h (module 'wifi'): ns3::Bar::Bar() [constructor]
    cls.add_constructor([])
    ## block-ack-manager.h (module 'wifi'): ns3::Bar::Bar(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address recipient, uint8_t tid, bool immediate) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('bool', 'immediate')])
    ## block-ack-manager.h (module 'wifi'): ns3::Bar::bar [variable]
    cls.add_instance_attribute('bar', 'ns3::Ptr< ns3::Packet const >', is_const=False)
    ## block-ack-manager.h (module 'wifi'): ns3::Bar::immediate [variable]
    cls.add_instance_attribute('immediate', 'bool', is_const=False)
    ## block-ack-manager.h (module 'wifi'): ns3::Bar::recipient [variable]
    cls.add_instance_attribute('recipient', 'ns3::Mac48Address', is_const=False)
    ## block-ack-manager.h (module 'wifi'): ns3::Bar::tid [variable]
    cls.add_instance_attribute('tid', 'uint8_t', is_const=False)
    return

def register_Ns3BlockAckAgreement_methods(root_module, cls):
    ## block-ack-agreement.h (module 'wifi'): ns3::BlockAckAgreement::BlockAckAgreement(ns3::BlockAckAgreement const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BlockAckAgreement const &', 'arg0')])
    ## block-ack-agreement.h (module 'wifi'): ns3::BlockAckAgreement::BlockAckAgreement() [constructor]
    cls.add_constructor([])
    ## block-ack-agreement.h (module 'wifi'): ns3::BlockAckAgreement::BlockAckAgreement(ns3::Mac48Address peer, uint8_t tid) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'peer'), param('uint8_t', 'tid')])
    ## block-ack-agreement.h (module 'wifi'): uint16_t ns3::BlockAckAgreement::GetBufferSize() const [member function]
    cls.add_method('GetBufferSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): ns3::Mac48Address ns3::BlockAckAgreement::GetPeer() const [member function]
    cls.add_method('GetPeer', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): uint16_t ns3::BlockAckAgreement::GetStartingSequence() const [member function]
    cls.add_method('GetStartingSequence', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): uint16_t ns3::BlockAckAgreement::GetStartingSequenceControl() const [member function]
    cls.add_method('GetStartingSequenceControl', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): uint8_t ns3::BlockAckAgreement::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): uint16_t ns3::BlockAckAgreement::GetTimeout() const [member function]
    cls.add_method('GetTimeout', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): uint16_t ns3::BlockAckAgreement::GetWinEnd() const [member function]
    cls.add_method('GetWinEnd', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): bool ns3::BlockAckAgreement::IsAmsduSupported() const [member function]
    cls.add_method('IsAmsduSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): bool ns3::BlockAckAgreement::IsHtSupported() const [member function]
    cls.add_method('IsHtSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): bool ns3::BlockAckAgreement::IsImmediateBlockAck() const [member function]
    cls.add_method('IsImmediateBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetAmsduSupport(bool supported) [member function]
    cls.add_method('SetAmsduSupport', 
                   'void', 
                   [param('bool', 'supported')])
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetBufferSize(uint16_t bufferSize) [member function]
    cls.add_method('SetBufferSize', 
                   'void', 
                   [param('uint16_t', 'bufferSize')])
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetDelayedBlockAck() [member function]
    cls.add_method('SetDelayedBlockAck', 
                   'void', 
                   [])
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetHtSupported(bool htSupported) [member function]
    cls.add_method('SetHtSupported', 
                   'void', 
                   [param('bool', 'htSupported')])
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetImmediateBlockAck() [member function]
    cls.add_method('SetImmediateBlockAck', 
                   'void', 
                   [])
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetStartingSequence(uint16_t seq) [member function]
    cls.add_method('SetStartingSequence', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetStartingSequenceControl(uint16_t seq) [member function]
    cls.add_method('SetStartingSequenceControl', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetTimeout(uint16_t timeout) [member function]
    cls.add_method('SetTimeout', 
                   'void', 
                   [param('uint16_t', 'timeout')])
    ## block-ack-agreement.h (module 'wifi'): void ns3::BlockAckAgreement::SetWinEnd(uint16_t seq) [member function]
    cls.add_method('SetWinEnd', 
                   'void', 
                   [param('uint16_t', 'seq')])
    return

def register_Ns3BlockAckManager_methods(root_module, cls):
    ## block-ack-manager.h (module 'wifi'): ns3::BlockAckManager::BlockAckManager() [constructor]
    cls.add_constructor([])
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::AlreadyExists(uint16_t currentSeq, ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('AlreadyExists', 
                   'bool', 
                   [param('uint16_t', 'currentSeq'), param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::CompleteAmpduExchange(ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('CompleteAmpduExchange', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::CreateAgreement(ns3::MgtAddBaRequestHeader const * reqHdr, ns3::Mac48Address recipient) [member function]
    cls.add_method('CreateAgreement', 
                   'void', 
                   [param('ns3::MgtAddBaRequestHeader const *', 'reqHdr'), param('ns3::Mac48Address', 'recipient')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::DestroyAgreement(ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('DestroyAgreement', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::ExistsAgreement(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('ExistsAgreement', 
                   'bool', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::ExistsAgreementInState(ns3::Mac48Address recipient, uint8_t tid, ns3::OriginatorBlockAckAgreement::State state) const [member function]
    cls.add_method('ExistsAgreementInState', 
                   'bool', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('ns3::OriginatorBlockAckAgreement::State', 'state')], 
                   is_const=True)
    ## block-ack-manager.h (module 'wifi'): uint32_t ns3::BlockAckManager::GetNBufferedPackets(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('GetNBufferedPackets', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## block-ack-manager.h (module 'wifi'): uint32_t ns3::BlockAckManager::GetNRetryNeededPackets(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('GetNRetryNeededPackets', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## block-ack-manager.h (module 'wifi'): ns3::Ptr<ns3::Packet const> ns3::BlockAckManager::GetNextPacket(ns3::WifiMacHeader & hdr) [member function]
    cls.add_method('GetNextPacket', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [param('ns3::WifiMacHeader &', 'hdr')])
    ## block-ack-manager.h (module 'wifi'): uint32_t ns3::BlockAckManager::GetNextPacketSize() const [member function]
    cls.add_method('GetNextPacketSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## block-ack-manager.h (module 'wifi'): uint16_t ns3::BlockAckManager::GetSeqNumOfNextRetryPacket(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('GetSeqNumOfNextRetryPacket', 
                   'uint16_t', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::HasBar(ns3::Bar & bar) [member function]
    cls.add_method('HasBar', 
                   'bool', 
                   [param('ns3::Bar &', 'bar')])
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::HasOtherFragments(uint16_t sequenceNumber) const [member function]
    cls.add_method('HasOtherFragments', 
                   'bool', 
                   [param('uint16_t', 'sequenceNumber')], 
                   is_const=True)
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::HasPackets() const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [], 
                   is_const=True)
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::NeedBarRetransmission(uint8_t tid, uint16_t seqNumber, ns3::Mac48Address recipient) [member function]
    cls.add_method('NeedBarRetransmission', 
                   'bool', 
                   [param('uint8_t', 'tid'), param('uint16_t', 'seqNumber'), param('ns3::Mac48Address', 'recipient')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::NotifyAgreementEstablished(ns3::Mac48Address recipient, uint8_t tid, uint16_t startingSeq) [member function]
    cls.add_method('NotifyAgreementEstablished', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('uint16_t', 'startingSeq')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::NotifyAgreementUnsuccessful(ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('NotifyAgreementUnsuccessful', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::NotifyGotBlockAck(ns3::CtrlBAckResponseHeader const * blockAck, ns3::Mac48Address recipient, ns3::WifiMode txMode) [member function]
    cls.add_method('NotifyGotBlockAck', 
                   'void', 
                   [param('ns3::CtrlBAckResponseHeader const *', 'blockAck'), param('ns3::Mac48Address', 'recipient'), param('ns3::WifiMode', 'txMode')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::NotifyMpduTransmission(ns3::Mac48Address recipient, uint8_t tid, uint16_t nextSeqNumber, ns3::WifiMacHeader::QosAckPolicy policy) [member function]
    cls.add_method('NotifyMpduTransmission', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('uint16_t', 'nextSeqNumber'), param('ns3::WifiMacHeader::QosAckPolicy', 'policy')])
    ## block-ack-manager.h (module 'wifi'): ns3::Ptr<ns3::Packet const> ns3::BlockAckManager::PeekNextPacket(ns3::WifiMacHeader & hdr, ns3::Mac48Address recipient, uint8_t tid, ns3::Time * timestamp) [member function]
    cls.add_method('PeekNextPacket', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [param('ns3::WifiMacHeader &', 'hdr'), param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('ns3::Time *', 'timestamp')])
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::RemovePacket(uint8_t tid, ns3::Mac48Address recipient, uint16_t seqnumber) [member function]
    cls.add_method('RemovePacket', 
                   'bool', 
                   [param('uint8_t', 'tid'), param('ns3::Mac48Address', 'recipient'), param('uint16_t', 'seqnumber')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetBlockAckInactivityCallback(ns3::Callback<void, ns3::Mac48Address, unsigned char, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetBlockAckInactivityCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Mac48Address, unsigned char, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetBlockAckThreshold(uint8_t nPackets) [member function]
    cls.add_method('SetBlockAckThreshold', 
                   'void', 
                   [param('uint8_t', 'nPackets')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetBlockAckType(ns3::BlockAckType bAckType) [member function]
    cls.add_method('SetBlockAckType', 
                   'void', 
                   [param('ns3::BlockAckType', 'bAckType')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetBlockDestinationCallback(ns3::Callback<void, ns3::Mac48Address, unsigned char, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetBlockDestinationCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Mac48Address, unsigned char, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetMaxPacketDelay(ns3::Time maxDelay) [member function]
    cls.add_method('SetMaxPacketDelay', 
                   'void', 
                   [param('ns3::Time', 'maxDelay')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetQueue(ns3::Ptr<ns3::WifiMacQueue> queue) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiMacQueue >', 'queue')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetTxFailedCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxFailedCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetTxMiddle(ns3::MacTxMiddle * txMiddle) [member function]
    cls.add_method('SetTxMiddle', 
                   'void', 
                   [param('ns3::MacTxMiddle *', 'txMiddle')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetTxOkCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetUnblockDestinationCallback(ns3::Callback<void, ns3::Mac48Address, unsigned char, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetUnblockDestinationCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Mac48Address, unsigned char, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> manager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'manager')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::StorePacket(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const & hdr, ns3::Time tStamp) [member function]
    cls.add_method('StorePacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const &', 'hdr'), param('ns3::Time', 'tStamp')])
    ## block-ack-manager.h (module 'wifi'): bool ns3::BlockAckManager::SwitchToBlockAckIfNeeded(ns3::Mac48Address recipient, uint8_t tid, uint16_t startingSeq) [member function]
    cls.add_method('SwitchToBlockAckIfNeeded', 
                   'bool', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('uint16_t', 'startingSeq')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::TearDownBlockAck(ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('TearDownBlockAck', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## block-ack-manager.h (module 'wifi'): void ns3::BlockAckManager::UpdateAgreement(ns3::MgtAddBaResponseHeader const * respHdr, ns3::Mac48Address recipient) [member function]
    cls.add_method('UpdateAgreement', 
                   'void', 
                   [param('ns3::MgtAddBaResponseHeader const *', 'respHdr'), param('ns3::Mac48Address', 'recipient')])
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
    ## buffer.h (module 'network'): void ns3::Buffer::AddAtEnd(uint32_t end) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## buffer.h (module 'network'): void ns3::Buffer::AddAtEnd(ns3::Buffer const & o) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Buffer const &', 'o')])
    ## buffer.h (module 'network'): void ns3::Buffer::AddAtStart(uint32_t start) [member function]
    cls.add_method('AddAtStart', 
                   'void', 
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
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Deserialize(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h (module 'network'): ns3::Buffer::Iterator ns3::Buffer::End() const [member function]
    cls.add_method('End', 
                   'ns3::Buffer::Iterator', 
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
    ## buffer.h (module 'network'): uint8_t ns3::Buffer::Iterator::PeekU8() [member function]
    cls.add_method('PeekU8', 
                   'uint8_t', 
                   [])
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
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Read(ns3::Buffer::Iterator start, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'size')])
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
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::AddAtEnd(int32_t appendOffset) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('int32_t', 'appendOffset')])
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::AddAtStart(int32_t prependOffset) [member function]
    cls.add_method('AddAtStart', 
                   'void', 
                   [param('int32_t', 'prependOffset')])
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::Adjust(int32_t adjustment) [member function]
    cls.add_method('Adjust', 
                   'void', 
                   [param('int32_t', 'adjustment')])
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
    return

def register_Ns3CapabilityInformation_methods(root_module, cls):
    ## capability-information.h (module 'wifi'): ns3::CapabilityInformation::CapabilityInformation(ns3::CapabilityInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CapabilityInformation const &', 'arg0')])
    ## capability-information.h (module 'wifi'): ns3::CapabilityInformation::CapabilityInformation() [constructor]
    cls.add_constructor([])
    ## capability-information.h (module 'wifi'): ns3::Buffer::Iterator ns3::CapabilityInformation::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## capability-information.h (module 'wifi'): uint32_t ns3::CapabilityInformation::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## capability-information.h (module 'wifi'): bool ns3::CapabilityInformation::IsEss() const [member function]
    cls.add_method('IsEss', 
                   'bool', 
                   [], 
                   is_const=True)
    ## capability-information.h (module 'wifi'): bool ns3::CapabilityInformation::IsIbss() const [member function]
    cls.add_method('IsIbss', 
                   'bool', 
                   [], 
                   is_const=True)
    ## capability-information.h (module 'wifi'): bool ns3::CapabilityInformation::IsShortPreamble() const [member function]
    cls.add_method('IsShortPreamble', 
                   'bool', 
                   [], 
                   is_const=True)
    ## capability-information.h (module 'wifi'): bool ns3::CapabilityInformation::IsShortSlotTime() const [member function]
    cls.add_method('IsShortSlotTime', 
                   'bool', 
                   [], 
                   is_const=True)
    ## capability-information.h (module 'wifi'): ns3::Buffer::Iterator ns3::CapabilityInformation::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## capability-information.h (module 'wifi'): void ns3::CapabilityInformation::SetEss() [member function]
    cls.add_method('SetEss', 
                   'void', 
                   [])
    ## capability-information.h (module 'wifi'): void ns3::CapabilityInformation::SetIbss() [member function]
    cls.add_method('SetIbss', 
                   'void', 
                   [])
    ## capability-information.h (module 'wifi'): void ns3::CapabilityInformation::SetShortPreamble(bool shortPreamble) [member function]
    cls.add_method('SetShortPreamble', 
                   'void', 
                   [param('bool', 'shortPreamble')])
    ## capability-information.h (module 'wifi'): void ns3::CapabilityInformation::SetShortSlotTime(bool shortSlotTime) [member function]
    cls.add_method('SetShortSlotTime', 
                   'void', 
                   [param('bool', 'shortSlotTime')])
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

def register_Ns3Hasher_methods(root_module, cls):
    ## hash.h (module 'core'): ns3::Hasher::Hasher(ns3::Hasher const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hasher const &', 'arg0')])
    ## hash.h (module 'core'): ns3::Hasher::Hasher() [constructor]
    cls.add_constructor([])
    ## hash.h (module 'core'): ns3::Hasher::Hasher(ns3::Ptr<ns3::Hash::Implementation> hp) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Hash::Implementation >', 'hp')])
    ## hash.h (module 'core'): uint32_t ns3::Hasher::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')])
    ## hash.h (module 'core'): uint32_t ns3::Hasher::GetHash32(std::string const s) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('std::string const', 's')])
    ## hash.h (module 'core'): uint64_t ns3::Hasher::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')])
    ## hash.h (module 'core'): uint64_t ns3::Hasher::GetHash64(std::string const s) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('std::string const', 's')])
    ## hash.h (module 'core'): ns3::Hasher & ns3::Hasher::clear() [member function]
    cls.add_method('clear', 
                   'ns3::Hasher &', 
                   [])
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
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsAny() const [member function]
    cls.add_method('IsAny', 
                   'bool', 
                   [], 
                   is_const=True)
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
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsLocalhost() const [member function]
    cls.add_method('IsLocalhost', 
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
                   deprecated=True, is_const=True)
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
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsDocumentation() const [member function]
    cls.add_method('IsDocumentation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsEqual(ns3::Ipv6Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Address const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsIpv4MappedAddress() const [member function]
    cls.add_method('IsIpv4MappedAddress', 
                   'bool', 
                   [], 
                   is_const=True)
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
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac16Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac16Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac48Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac64Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac64Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac16Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac16Address', 'mac')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac48Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'mac')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac64Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac64Address', 'mac')], 
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

def register_Ns3MeshHelper_methods(root_module, cls):
    ## mesh-helper.h (module 'mesh'): ns3::MeshHelper::MeshHelper(ns3::MeshHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshHelper const &', 'arg0')])
    ## mesh-helper.h (module 'mesh'): ns3::MeshHelper::MeshHelper() [constructor]
    cls.add_constructor([])
    ## mesh-helper.h (module 'mesh'): int64_t ns3::MeshHelper::AssignStreams(ns3::NetDeviceContainer c, int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('ns3::NetDeviceContainer', 'c'), param('int64_t', 'stream')])
    ## mesh-helper.h (module 'mesh'): static ns3::MeshHelper ns3::MeshHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::MeshHelper', 
                   [], 
                   is_static=True)
    ## mesh-helper.h (module 'mesh'): ns3::NetDeviceContainer ns3::MeshHelper::Install(ns3::WifiPhyHelper const & phyHelper, ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phyHelper'), param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## mesh-helper.h (module 'mesh'): void ns3::MeshHelper::Report(ns3::Ptr<ns3::NetDevice> const & arg0, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice > const &', 'arg0'), param('std::ostream &', 'arg1')])
    ## mesh-helper.h (module 'mesh'): void ns3::MeshHelper::ResetStats(ns3::Ptr<ns3::NetDevice> const & arg0) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice > const &', 'arg0')])
    ## mesh-helper.h (module 'mesh'): void ns3::MeshHelper::SetMacType(std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetMacType', 
                   'void', 
                   [param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## mesh-helper.h (module 'mesh'): void ns3::MeshHelper::SetNumberOfInterfaces(uint32_t nInterfaces) [member function]
    cls.add_method('SetNumberOfInterfaces', 
                   'void', 
                   [param('uint32_t', 'nInterfaces')])
    ## mesh-helper.h (module 'mesh'): void ns3::MeshHelper::SetRemoteStationManager(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetRemoteStationManager', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## mesh-helper.h (module 'mesh'): void ns3::MeshHelper::SetSpreadInterfaceChannels(ns3::MeshHelper::ChannelPolicy arg0) [member function]
    cls.add_method('SetSpreadInterfaceChannels', 
                   'void', 
                   [param('ns3::MeshHelper::ChannelPolicy', 'arg0')])
    ## mesh-helper.h (module 'mesh'): void ns3::MeshHelper::SetStackInstaller(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetStackInstaller', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## mesh-helper.h (module 'mesh'): void ns3::MeshHelper::SetStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('SetStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')])
    return

def register_Ns3MeshWifiBeacon_methods(root_module, cls):
    ## mesh-wifi-beacon.h (module 'mesh'): ns3::MeshWifiBeacon::MeshWifiBeacon(ns3::MeshWifiBeacon const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshWifiBeacon const &', 'arg0')])
    ## mesh-wifi-beacon.h (module 'mesh'): ns3::MeshWifiBeacon::MeshWifiBeacon(ns3::Ssid ssid, ns3::SupportedRates rates, uint64_t us) [constructor]
    cls.add_constructor([param('ns3::Ssid', 'ssid'), param('ns3::SupportedRates', 'rates'), param('uint64_t', 'us')])
    ## mesh-wifi-beacon.h (module 'mesh'): void ns3::MeshWifiBeacon::AddInformationElement(ns3::Ptr<ns3::WifiInformationElement> ie) [member function]
    cls.add_method('AddInformationElement', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiInformationElement >', 'ie')])
    ## mesh-wifi-beacon.h (module 'mesh'): ns3::MgtBeaconHeader ns3::MeshWifiBeacon::BeaconHeader() const [member function]
    cls.add_method('BeaconHeader', 
                   'ns3::MgtBeaconHeader', 
                   [], 
                   is_const=True)
    ## mesh-wifi-beacon.h (module 'mesh'): ns3::WifiMacHeader ns3::MeshWifiBeacon::CreateHeader(ns3::Mac48Address address, ns3::Mac48Address mpAddress) [member function]
    cls.add_method('CreateHeader', 
                   'ns3::WifiMacHeader', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::Mac48Address', 'mpAddress')])
    ## mesh-wifi-beacon.h (module 'mesh'): ns3::Ptr<ns3::Packet> ns3::MeshWifiBeacon::CreatePacket() [member function]
    cls.add_method('CreatePacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## mesh-wifi-beacon.h (module 'mesh'): ns3::Time ns3::MeshWifiBeacon::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
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
    ## object-base.h (module 'core'): bool ns3::ObjectBase::GetAttributeFailSafe(std::string name, ns3::AttributeValue & value) const [member function]
    cls.add_method('GetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'value')], 
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

def register_Ns3OriginatorBlockAckAgreement_methods(root_module, cls):
    ## originator-block-ack-agreement.h (module 'wifi'): ns3::OriginatorBlockAckAgreement::OriginatorBlockAckAgreement(ns3::OriginatorBlockAckAgreement const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OriginatorBlockAckAgreement const &', 'arg0')])
    ## originator-block-ack-agreement.h (module 'wifi'): ns3::OriginatorBlockAckAgreement::OriginatorBlockAckAgreement() [constructor]
    cls.add_constructor([])
    ## originator-block-ack-agreement.h (module 'wifi'): ns3::OriginatorBlockAckAgreement::OriginatorBlockAckAgreement(ns3::Mac48Address recipient, uint8_t tid) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## originator-block-ack-agreement.h (module 'wifi'): void ns3::OriginatorBlockAckAgreement::CompleteExchange() [member function]
    cls.add_method('CompleteExchange', 
                   'void', 
                   [])
    ## originator-block-ack-agreement.h (module 'wifi'): bool ns3::OriginatorBlockAckAgreement::IsBlockAckRequestNeeded() const [member function]
    cls.add_method('IsBlockAckRequestNeeded', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h (module 'wifi'): bool ns3::OriginatorBlockAckAgreement::IsEstablished() const [member function]
    cls.add_method('IsEstablished', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h (module 'wifi'): bool ns3::OriginatorBlockAckAgreement::IsInactive() const [member function]
    cls.add_method('IsInactive', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h (module 'wifi'): bool ns3::OriginatorBlockAckAgreement::IsPending() const [member function]
    cls.add_method('IsPending', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h (module 'wifi'): bool ns3::OriginatorBlockAckAgreement::IsUnsuccessful() const [member function]
    cls.add_method('IsUnsuccessful', 
                   'bool', 
                   [], 
                   is_const=True)
    ## originator-block-ack-agreement.h (module 'wifi'): void ns3::OriginatorBlockAckAgreement::NotifyMpduTransmission(uint16_t nextSeqNumber) [member function]
    cls.add_method('NotifyMpduTransmission', 
                   'void', 
                   [param('uint16_t', 'nextSeqNumber')])
    ## originator-block-ack-agreement.h (module 'wifi'): void ns3::OriginatorBlockAckAgreement::SetState(ns3::OriginatorBlockAckAgreement::State state) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::OriginatorBlockAckAgreement::State', 'state')])
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
    ## packet-tag-list.h (module 'network'): bool ns3::PacketTagList::Replace(ns3::Tag & tag) [member function]
    cls.add_method('Replace', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    return

def register_Ns3PacketTagListTagData_methods(root_module, cls):
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::TagData() [constructor]
    cls.add_constructor([])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::TagData(ns3::PacketTagList::TagData const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagList::TagData const &', 'arg0')])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::count [variable]
    cls.add_instance_attribute('count', 'uint32_t', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::data [variable]
    cls.add_instance_attribute('data', 'uint8_t [ 21 ]', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::next [variable]
    cls.add_instance_attribute('next', 'ns3::PacketTagList::TagData *', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3PcapFile_methods(root_module, cls):
    ## pcap-file.h (module 'network'): ns3::PcapFile::PcapFile() [constructor]
    cls.add_constructor([])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Close() [member function]
    cls.add_method('Close', 
                   'void', 
                   [])
    ## pcap-file.h (module 'network'): static bool ns3::PcapFile::Diff(std::string const & f1, std::string const & f2, uint32_t & sec, uint32_t & usec, uint32_t & packets, uint32_t snapLen=ns3::PcapFile::SNAPLEN_DEFAULT) [member function]
    cls.add_method('Diff', 
                   'bool', 
                   [param('std::string const &', 'f1'), param('std::string const &', 'f2'), param('uint32_t &', 'sec'), param('uint32_t &', 'usec'), param('uint32_t &', 'packets'), param('uint32_t', 'snapLen', default_value='ns3::PcapFile::SNAPLEN_DEFAULT')], 
                   is_static=True)
    ## pcap-file.h (module 'network'): bool ns3::PcapFile::Eof() const [member function]
    cls.add_method('Eof', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file.h (module 'network'): bool ns3::PcapFile::Fail() const [member function]
    cls.add_method('Fail', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file.h (module 'network'): uint32_t ns3::PcapFile::GetDataLinkType() [member function]
    cls.add_method('GetDataLinkType', 
                   'uint32_t', 
                   [])
    ## pcap-file.h (module 'network'): uint32_t ns3::PcapFile::GetMagic() [member function]
    cls.add_method('GetMagic', 
                   'uint32_t', 
                   [])
    ## pcap-file.h (module 'network'): uint32_t ns3::PcapFile::GetSigFigs() [member function]
    cls.add_method('GetSigFigs', 
                   'uint32_t', 
                   [])
    ## pcap-file.h (module 'network'): uint32_t ns3::PcapFile::GetSnapLen() [member function]
    cls.add_method('GetSnapLen', 
                   'uint32_t', 
                   [])
    ## pcap-file.h (module 'network'): bool ns3::PcapFile::GetSwapMode() [member function]
    cls.add_method('GetSwapMode', 
                   'bool', 
                   [])
    ## pcap-file.h (module 'network'): int32_t ns3::PcapFile::GetTimeZoneOffset() [member function]
    cls.add_method('GetTimeZoneOffset', 
                   'int32_t', 
                   [])
    ## pcap-file.h (module 'network'): uint16_t ns3::PcapFile::GetVersionMajor() [member function]
    cls.add_method('GetVersionMajor', 
                   'uint16_t', 
                   [])
    ## pcap-file.h (module 'network'): uint16_t ns3::PcapFile::GetVersionMinor() [member function]
    cls.add_method('GetVersionMinor', 
                   'uint16_t', 
                   [])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Init(uint32_t dataLinkType, uint32_t snapLen=ns3::PcapFile::SNAPLEN_DEFAULT, int32_t timeZoneCorrection=ns3::PcapFile::ZONE_DEFAULT, bool swapMode=false, bool nanosecMode=false) [member function]
    cls.add_method('Init', 
                   'void', 
                   [param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='ns3::PcapFile::SNAPLEN_DEFAULT'), param('int32_t', 'timeZoneCorrection', default_value='ns3::PcapFile::ZONE_DEFAULT'), param('bool', 'swapMode', default_value='false'), param('bool', 'nanosecMode', default_value='false')])
    ## pcap-file.h (module 'network'): bool ns3::PcapFile::IsNanoSecMode() [member function]
    cls.add_method('IsNanoSecMode', 
                   'bool', 
                   [])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Open(std::string const & filename, std::_Ios_Openmode mode) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string const &', 'filename'), param('std::_Ios_Openmode', 'mode')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Read(uint8_t * const data, uint32_t maxBytes, uint32_t & tsSec, uint32_t & tsUsec, uint32_t & inclLen, uint32_t & origLen, uint32_t & readLen) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t * const', 'data'), param('uint32_t', 'maxBytes'), param('uint32_t &', 'tsSec'), param('uint32_t &', 'tsUsec'), param('uint32_t &', 'inclLen'), param('uint32_t &', 'origLen'), param('uint32_t &', 'readLen')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, uint8_t const * const data, uint32_t totalLen) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('uint8_t const * const', 'data'), param('uint32_t', 'totalLen')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, ns3::Header const & header, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('ns3::Header const &', 'header'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file.h (module 'network'): ns3::PcapFile::SNAPLEN_DEFAULT [variable]
    cls.add_static_attribute('SNAPLEN_DEFAULT', 'uint32_t const', is_const=True)
    ## pcap-file.h (module 'network'): ns3::PcapFile::ZONE_DEFAULT [variable]
    cls.add_static_attribute('ZONE_DEFAULT', 'int32_t const', is_const=True)
    return

def register_Ns3PcapHelper_methods(root_module, cls):
    ## trace-helper.h (module 'network'): ns3::PcapHelper::PcapHelper(ns3::PcapHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PcapHelper const &', 'arg0')])
    ## trace-helper.h (module 'network'): ns3::PcapHelper::PcapHelper() [constructor]
    cls.add_constructor([])
    ## trace-helper.h (module 'network'): ns3::Ptr<ns3::PcapFileWrapper> ns3::PcapHelper::CreateFile(std::string filename, std::_Ios_Openmode filemode, uint32_t dataLinkType, uint32_t snapLen=std::numeric_limits<unsigned int>::max(), int32_t tzCorrection=0) [member function]
    cls.add_method('CreateFile', 
                   'ns3::Ptr< ns3::PcapFileWrapper >', 
                   [param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode'), param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='std::numeric_limits<unsigned int>::max()'), param('int32_t', 'tzCorrection', default_value='0')])
    ## trace-helper.h (module 'network'): std::string ns3::PcapHelper::GetFilenameFromDevice(std::string prefix, ns3::Ptr<ns3::NetDevice> device, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromDevice', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'useObjectNames', default_value='true')])
    ## trace-helper.h (module 'network'): std::string ns3::PcapHelper::GetFilenameFromInterfacePair(std::string prefix, ns3::Ptr<ns3::Object> object, uint32_t interface, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromInterfacePair', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Object >', 'object'), param('uint32_t', 'interface'), param('bool', 'useObjectNames', default_value='true')])
    return

def register_Ns3PcapHelperForDevice_methods(root_module, cls):
    ## trace-helper.h (module 'network'): ns3::PcapHelperForDevice::PcapHelperForDevice(ns3::PcapHelperForDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PcapHelperForDevice const &', 'arg0')])
    ## trace-helper.h (module 'network'): ns3::PcapHelperForDevice::PcapHelperForDevice() [constructor]
    cls.add_constructor([])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous=false, bool explicitFilename=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous', default_value='false'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, std::string ndName, bool promiscuous=false, bool explicitFilename=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('std::string', 'ndName'), param('bool', 'promiscuous', default_value='false'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::NetDeviceContainer d, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NetDeviceContainer', 'd'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::NodeContainer n, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NodeContainer', 'n'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, uint32_t nodeid, uint32_t deviceid, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcapAll(std::string prefix, bool promiscuous=false) [member function]
    cls.add_method('EnablePcapAll', 
                   'void', 
                   [param('std::string', 'prefix'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   is_pure_virtual=True, is_virtual=True)
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
    ## simulator.h (module 'core'): static void ns3::Simulator::Stop(ns3::Time const & delay) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time const &', 'delay')], 
                   is_static=True)
    return

def register_Ns3StatusCode_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## status-code.h (module 'wifi'): ns3::StatusCode::StatusCode(ns3::StatusCode const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StatusCode const &', 'arg0')])
    ## status-code.h (module 'wifi'): ns3::StatusCode::StatusCode() [constructor]
    cls.add_constructor([])
    ## status-code.h (module 'wifi'): ns3::Buffer::Iterator ns3::StatusCode::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## status-code.h (module 'wifi'): uint32_t ns3::StatusCode::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## status-code.h (module 'wifi'): bool ns3::StatusCode::IsSuccess() const [member function]
    cls.add_method('IsSuccess', 
                   'bool', 
                   [], 
                   is_const=True)
    ## status-code.h (module 'wifi'): ns3::Buffer::Iterator ns3::StatusCode::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## status-code.h (module 'wifi'): void ns3::StatusCode::SetFailure() [member function]
    cls.add_method('SetFailure', 
                   'void', 
                   [])
    ## status-code.h (module 'wifi'): void ns3::StatusCode::SetSuccess() [member function]
    cls.add_method('SetSuccess', 
                   'void', 
                   [])
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

def register_Ns3TimeWithUnit_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## nstime.h (module 'core'): ns3::TimeWithUnit::TimeWithUnit(ns3::TimeWithUnit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TimeWithUnit const &', 'arg0')])
    ## nstime.h (module 'core'): ns3::TimeWithUnit::TimeWithUnit(ns3::Time const time, ns3::Time::Unit const unit) [constructor]
    cls.add_constructor([param('ns3::Time const', 'time'), param('ns3::Time::Unit const', 'unit')])
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
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::Ptr< ns3::TraceSourceAccessor const >', 'accessor')], 
                   deprecated=True)
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::AddTraceSource(std::string name, std::string help, ns3::Ptr<ns3::TraceSourceAccessor const> accessor, std::string callback) [member function]
    cls.add_method('AddTraceSource', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::Ptr< ns3::TraceSourceAccessor const >', 'accessor'), param('std::string', 'callback')])
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
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetHash() const [member function]
    cls.add_method('GetHash', 
                   'uint32_t', 
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
    ## type-id.h (module 'core'): std::size_t ns3::TypeId::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'std::size_t', 
                   [], 
                   is_const=True)
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
    ## type-id.h (module 'core'): static ns3::TypeId ns3::TypeId::LookupByHash(uint32_t hash) [member function]
    cls.add_method('LookupByHash', 
                   'ns3::TypeId', 
                   [param('uint32_t', 'hash')], 
                   is_static=True)
    ## type-id.h (module 'core'): static bool ns3::TypeId::LookupByHashFailSafe(uint32_t hash, ns3::TypeId * tid) [member function]
    cls.add_method('LookupByHashFailSafe', 
                   'bool', 
                   [param('uint32_t', 'hash'), param('ns3::TypeId *', 'tid')], 
                   is_static=True)
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
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::SetSize(std::size_t size) [member function]
    cls.add_method('SetSize', 
                   'ns3::TypeId', 
                   [param('std::size_t', 'size')])
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
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::callback [variable]
    cls.add_instance_attribute('callback', 'std::string', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::help [variable]
    cls.add_instance_attribute('help', 'std::string', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::name [variable]
    cls.add_instance_attribute('name', 'std::string', is_const=False)
    return

def register_Ns3WifiHelper_methods(root_module, cls):
    ## wifi-helper.h (module 'wifi'): ns3::WifiHelper::WifiHelper(ns3::WifiHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiHelper const &', 'arg0')])
    ## wifi-helper.h (module 'wifi'): ns3::WifiHelper::WifiHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h (module 'wifi'): int64_t ns3::WifiHelper::AssignStreams(ns3::NetDeviceContainer c, int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('ns3::NetDeviceContainer', 'c'), param('int64_t', 'stream')])
    ## wifi-helper.h (module 'wifi'): static ns3::WifiHelper ns3::WifiHelper::Default() [member function]
    cls.add_method('Default', 
                   'ns3::WifiHelper', 
                   [], 
                   is_static=True, deprecated=True)
    ## wifi-helper.h (module 'wifi'): static void ns3::WifiHelper::EnableLogComponents() [member function]
    cls.add_method('EnableLogComponents', 
                   'void', 
                   [], 
                   is_static=True)
    ## wifi-helper.h (module 'wifi'): ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('ns3::NodeContainer', 'c')], 
                   is_const=True, is_virtual=True)
    ## wifi-helper.h (module 'wifi'): ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True, is_virtual=True)
    ## wifi-helper.h (module 'wifi'): ns3::NetDeviceContainer ns3::WifiHelper::Install(ns3::WifiPhyHelper const & phy, ns3::WifiMacHelper const & mac, std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::WifiPhyHelper const &', 'phy'), param('ns3::WifiMacHelper const &', 'mac'), param('std::string', 'nodeName')], 
                   is_const=True, is_virtual=True)
    ## wifi-helper.h (module 'wifi'): void ns3::WifiHelper::SetRemoteStationManager(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetRemoteStationManager', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()')])
    ## wifi-helper.h (module 'wifi'): void ns3::WifiHelper::SetStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('SetStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_virtual=True)
    return

def register_Ns3WifiMacHelper_methods(root_module, cls):
    ## wifi-mac-helper.h (module 'wifi'): ns3::WifiMacHelper::WifiMacHelper(ns3::WifiMacHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMacHelper const &', 'arg0')])
    ## wifi-mac-helper.h (module 'wifi'): ns3::WifiMacHelper::WifiMacHelper() [constructor]
    cls.add_constructor([])
    ## wifi-mac-helper.h (module 'wifi'): ns3::Ptr<ns3::WifiMac> ns3::WifiMacHelper::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-helper.h (module 'wifi'): void ns3::WifiMacHelper::SetType(std::string type, std::string n0="", ns3::AttributeValue const & v0=ns3::EmptyAttributeValue(), std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue(), std::string n5="", ns3::AttributeValue const & v5=ns3::EmptyAttributeValue(), std::string n6="", ns3::AttributeValue const & v6=ns3::EmptyAttributeValue(), std::string n7="", ns3::AttributeValue const & v7=ns3::EmptyAttributeValue(), std::string n8="", ns3::AttributeValue const & v8=ns3::EmptyAttributeValue(), std::string n9="", ns3::AttributeValue const & v9=ns3::EmptyAttributeValue(), std::string n10="", ns3::AttributeValue const & v10=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n0', default_value='""'), param('ns3::AttributeValue const &', 'v0', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n5', default_value='""'), param('ns3::AttributeValue const &', 'v5', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n6', default_value='""'), param('ns3::AttributeValue const &', 'v6', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n7', default_value='""'), param('ns3::AttributeValue const &', 'v7', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n8', default_value='""'), param('ns3::AttributeValue const &', 'v8', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n9', default_value='""'), param('ns3::AttributeValue const &', 'v9', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n10', default_value='""'), param('ns3::AttributeValue const &', 'v10', default_value='ns3::EmptyAttributeValue()')], 
                   is_virtual=True)
    return

def register_Ns3WifiMode_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## wifi-mode.h (module 'wifi'): ns3::WifiMode::WifiMode(ns3::WifiMode const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMode const &', 'arg0')])
    ## wifi-mode.h (module 'wifi'): ns3::WifiMode::WifiMode() [constructor]
    cls.add_constructor([])
    ## wifi-mode.h (module 'wifi'): ns3::WifiMode::WifiMode(std::string name) [constructor]
    cls.add_constructor([param('std::string', 'name')])
    ## wifi-mode.h (module 'wifi'): ns3::WifiCodeRate ns3::WifiMode::GetCodeRate(uint8_t nss) const [member function]
    cls.add_method('GetCodeRate', 
                   'ns3::WifiCodeRate', 
                   [param('uint8_t', 'nss')], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): uint16_t ns3::WifiMode::GetConstellationSize(uint8_t nss) const [member function]
    cls.add_method('GetConstellationSize', 
                   'uint16_t', 
                   [param('uint8_t', 'nss')], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): uint64_t ns3::WifiMode::GetDataRate(uint32_t channelWidth, bool isShortGuardInterval, uint8_t nss) const [member function]
    cls.add_method('GetDataRate', 
                   'uint64_t', 
                   [param('uint32_t', 'channelWidth'), param('bool', 'isShortGuardInterval'), param('uint8_t', 'nss')], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): uint64_t ns3::WifiMode::GetDataRate(ns3::WifiTxVector txVector) const [member function]
    cls.add_method('GetDataRate', 
                   'uint64_t', 
                   [param('ns3::WifiTxVector', 'txVector')], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): uint8_t ns3::WifiMode::GetMcsValue() const [member function]
    cls.add_method('GetMcsValue', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): ns3::WifiModulationClass ns3::WifiMode::GetModulationClass() const [member function]
    cls.add_method('GetModulationClass', 
                   'ns3::WifiModulationClass', 
                   [], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): uint64_t ns3::WifiMode::GetPhyRate(uint32_t channelWidth, bool isShortGuardInterval, uint8_t nss) const [member function]
    cls.add_method('GetPhyRate', 
                   'uint64_t', 
                   [param('uint32_t', 'channelWidth'), param('bool', 'isShortGuardInterval'), param('uint8_t', 'nss')], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): uint64_t ns3::WifiMode::GetPhyRate(ns3::WifiTxVector txVector) const [member function]
    cls.add_method('GetPhyRate', 
                   'uint64_t', 
                   [param('ns3::WifiTxVector', 'txVector')], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): uint32_t ns3::WifiMode::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): std::string ns3::WifiMode::GetUniqueName() const [member function]
    cls.add_method('GetUniqueName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): bool ns3::WifiMode::IsMandatory() const [member function]
    cls.add_method('IsMandatory', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3WifiModeFactory_methods(root_module, cls):
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeFactory::WifiModeFactory(ns3::WifiModeFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeFactory const &', 'arg0')])
    ## wifi-mode.h (module 'wifi'): static ns3::WifiMode ns3::WifiModeFactory::CreateWifiMcs(std::string uniqueName, uint8_t mcsValue, ns3::WifiModulationClass modClass) [member function]
    cls.add_method('CreateWifiMcs', 
                   'ns3::WifiMode', 
                   [param('std::string', 'uniqueName'), param('uint8_t', 'mcsValue'), param('ns3::WifiModulationClass', 'modClass')], 
                   is_static=True)
    ## wifi-mode.h (module 'wifi'): static ns3::WifiMode ns3::WifiModeFactory::CreateWifiMode(std::string uniqueName, ns3::WifiModulationClass modClass, bool isMandatory, ns3::WifiCodeRate codingRate, uint16_t constellationSize) [member function]
    cls.add_method('CreateWifiMode', 
                   'ns3::WifiMode', 
                   [param('std::string', 'uniqueName'), param('ns3::WifiModulationClass', 'modClass'), param('bool', 'isMandatory'), param('ns3::WifiCodeRate', 'codingRate'), param('uint16_t', 'constellationSize')], 
                   is_static=True)
    return

def register_Ns3WifiPhyHelper_methods(root_module, cls):
    ## wifi-helper.h (module 'wifi'): ns3::WifiPhyHelper::WifiPhyHelper() [constructor]
    cls.add_constructor([])
    ## wifi-helper.h (module 'wifi'): ns3::WifiPhyHelper::WifiPhyHelper(ns3::WifiPhyHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhyHelper const &', 'arg0')])
    ## wifi-helper.h (module 'wifi'): ns3::Ptr<ns3::WifiPhy> ns3::WifiPhyHelper::Create(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::NetDevice> device) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3WifiPhyListener_methods(root_module, cls):
    ## wifi-phy.h (module 'wifi'): ns3::WifiPhyListener::WifiPhyListener() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h (module 'wifi'): ns3::WifiPhyListener::WifiPhyListener(ns3::WifiPhyListener const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhyListener const &', 'arg0')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhyListener::NotifyMaybeCcaBusyStart(ns3::Time duration) [member function]
    cls.add_method('NotifyMaybeCcaBusyStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhyListener::NotifyRxEndError() [member function]
    cls.add_method('NotifyRxEndError', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhyListener::NotifyRxEndOk() [member function]
    cls.add_method('NotifyRxEndOk', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhyListener::NotifyRxStart(ns3::Time duration) [member function]
    cls.add_method('NotifyRxStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhyListener::NotifySleep() [member function]
    cls.add_method('NotifySleep', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhyListener::NotifySwitchingStart(ns3::Time duration) [member function]
    cls.add_method('NotifySwitchingStart', 
                   'void', 
                   [param('ns3::Time', 'duration')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhyListener::NotifyTxStart(ns3::Time duration, double txPowerDbm) [member function]
    cls.add_method('NotifyTxStart', 
                   'void', 
                   [param('ns3::Time', 'duration'), param('double', 'txPowerDbm')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhyListener::NotifyWakeup() [member function]
    cls.add_method('NotifyWakeup', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3WifiRemoteStation_methods(root_module, cls):
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStation::WifiRemoteStation() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStation::WifiRemoteStation(ns3::WifiRemoteStation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStation const &', 'arg0')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStation::m_slrc [variable]
    cls.add_instance_attribute('m_slrc', 'uint32_t', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStation::m_ssrc [variable]
    cls.add_instance_attribute('m_ssrc', 'uint32_t', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStation::m_state [variable]
    cls.add_instance_attribute('m_state', 'ns3::WifiRemoteStationState *', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStation::m_tid [variable]
    cls.add_instance_attribute('m_tid', 'uint8_t', is_const=False)
    return

def register_Ns3WifiRemoteStationInfo_methods(root_module, cls):
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationInfo::WifiRemoteStationInfo(ns3::WifiRemoteStationInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStationInfo const &', 'arg0')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationInfo::WifiRemoteStationInfo() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h (module 'wifi'): double ns3::WifiRemoteStationInfo::GetFrameErrorRate() const [member function]
    cls.add_method('GetFrameErrorRate', 
                   'double', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationInfo::NotifyTxFailed() [member function]
    cls.add_method('NotifyTxFailed', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationInfo::NotifyTxSuccess(uint32_t retryCounter) [member function]
    cls.add_method('NotifyTxSuccess', 
                   'void', 
                   [param('uint32_t', 'retryCounter')])
    return

def register_Ns3WifiRemoteStationState_methods(root_module, cls):
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::WifiRemoteStationState() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::WifiRemoteStationState(ns3::WifiRemoteStationState const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStationState const &', 'arg0')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_address [variable]
    cls.add_instance_attribute('m_address', 'ns3::Mac48Address', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_aggregation [variable]
    cls.add_instance_attribute('m_aggregation', 'bool', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_channelWidth [variable]
    cls.add_instance_attribute('m_channelWidth', 'uint32_t', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_greenfield [variable]
    cls.add_instance_attribute('m_greenfield', 'bool', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_htSupported [variable]
    cls.add_instance_attribute('m_htSupported', 'bool', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_info [variable]
    cls.add_instance_attribute('m_info', 'ns3::WifiRemoteStationInfo', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_ness [variable]
    cls.add_instance_attribute('m_ness', 'uint32_t', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_operationalMcsSet [variable]
    cls.add_instance_attribute('m_operationalMcsSet', 'ns3::WifiModeList', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_operationalRateSet [variable]
    cls.add_instance_attribute('m_operationalRateSet', 'ns3::WifiModeList', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_rx [variable]
    cls.add_instance_attribute('m_rx', 'uint8_t', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_shortGuardInterval [variable]
    cls.add_instance_attribute('m_shortGuardInterval', 'bool', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_shortPreamble [variable]
    cls.add_instance_attribute('m_shortPreamble', 'bool', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_shortSlotTime [variable]
    cls.add_instance_attribute('m_shortSlotTime', 'bool', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_stbc [variable]
    cls.add_instance_attribute('m_stbc', 'bool', is_const=False)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationState::m_vhtSupported [variable]
    cls.add_instance_attribute('m_vhtSupported', 'bool', is_const=False)
    return

def register_Ns3WifiTxVector_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## wifi-tx-vector.h (module 'wifi'): ns3::WifiTxVector::WifiTxVector(ns3::WifiTxVector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiTxVector const &', 'arg0')])
    ## wifi-tx-vector.h (module 'wifi'): ns3::WifiTxVector::WifiTxVector() [constructor]
    cls.add_constructor([])
    ## wifi-tx-vector.h (module 'wifi'): ns3::WifiTxVector::WifiTxVector(ns3::WifiMode mode, uint8_t powerLevel, uint8_t retries, bool shortGuardInterval, uint8_t nss, uint8_t ness, uint32_t channelWidth, bool aggregation, bool stbc) [constructor]
    cls.add_constructor([param('ns3::WifiMode', 'mode'), param('uint8_t', 'powerLevel'), param('uint8_t', 'retries'), param('bool', 'shortGuardInterval'), param('uint8_t', 'nss'), param('uint8_t', 'ness'), param('uint32_t', 'channelWidth'), param('bool', 'aggregation'), param('bool', 'stbc')])
    ## wifi-tx-vector.h (module 'wifi'): uint32_t ns3::WifiTxVector::GetChannelWidth() const [member function]
    cls.add_method('GetChannelWidth', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): ns3::WifiMode ns3::WifiTxVector::GetMode() const [member function]
    cls.add_method('GetMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): uint8_t ns3::WifiTxVector::GetNess() const [member function]
    cls.add_method('GetNess', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): uint8_t ns3::WifiTxVector::GetNss() const [member function]
    cls.add_method('GetNss', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): uint8_t ns3::WifiTxVector::GetRetries() const [member function]
    cls.add_method('GetRetries', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): uint8_t ns3::WifiTxVector::GetTxPowerLevel() const [member function]
    cls.add_method('GetTxPowerLevel', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): bool ns3::WifiTxVector::IsAggregation() const [member function]
    cls.add_method('IsAggregation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): bool ns3::WifiTxVector::IsShortGuardInterval() const [member function]
    cls.add_method('IsShortGuardInterval', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): bool ns3::WifiTxVector::IsStbc() const [member function]
    cls.add_method('IsStbc', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetAggregation(bool aggregation) [member function]
    cls.add_method('SetAggregation', 
                   'void', 
                   [param('bool', 'aggregation')])
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetChannelWidth(uint32_t channelWidth) [member function]
    cls.add_method('SetChannelWidth', 
                   'void', 
                   [param('uint32_t', 'channelWidth')])
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetMode(ns3::WifiMode mode) [member function]
    cls.add_method('SetMode', 
                   'void', 
                   [param('ns3::WifiMode', 'mode')])
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetNess(uint8_t ness) [member function]
    cls.add_method('SetNess', 
                   'void', 
                   [param('uint8_t', 'ness')])
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetNss(uint8_t nss) [member function]
    cls.add_method('SetNss', 
                   'void', 
                   [param('uint8_t', 'nss')])
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetRetries(uint8_t retries) [member function]
    cls.add_method('SetRetries', 
                   'void', 
                   [param('uint8_t', 'retries')])
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetShortGuardInterval(bool guardinterval) [member function]
    cls.add_method('SetShortGuardInterval', 
                   'void', 
                   [param('bool', 'guardinterval')])
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetStbc(bool stbc) [member function]
    cls.add_method('SetStbc', 
                   'void', 
                   [param('bool', 'stbc')])
    ## wifi-tx-vector.h (module 'wifi'): void ns3::WifiTxVector::SetTxPowerLevel(uint8_t powerlevel) [member function]
    cls.add_method('SetTxPowerLevel', 
                   'void', 
                   [param('uint8_t', 'powerlevel')])
    return

def register_Ns3Empty_methods(root_module, cls):
    ## empty.h (module 'core'): ns3::empty::empty() [constructor]
    cls.add_constructor([])
    ## empty.h (module 'core'): ns3::empty::empty(ns3::empty const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::empty const &', 'arg0')])
    return

def register_Ns3Int64x64_t_methods(root_module, cls):
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', u'right'))
    cls.add_inplace_numeric_operator('+=', param('ns3::int64x64_t const &', u'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', u'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', u'right'))
    cls.add_unary_numeric_operator('-')
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', u'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    cls.add_inplace_numeric_operator('*=', param('ns3::int64x64_t const &', u'right'))
    cls.add_inplace_numeric_operator('-=', param('ns3::int64x64_t const &', u'right'))
    cls.add_inplace_numeric_operator('/=', param('ns3::int64x64_t const &', u'right'))
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>=')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t() [constructor]
    cls.add_constructor([])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(double v) [constructor]
    cls.add_constructor([param('double', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long double v) [constructor]
    cls.add_constructor([param('long double', 'v')])
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
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::implementation [variable]
    cls.add_static_attribute('implementation', 'ns3::int64x64_t::impl_type const', is_const=True)
    return

def register_Ns3MpduInfo_methods(root_module, cls):
    ## wifi-phy.h (module 'wifi'): ns3::mpduInfo::mpduInfo() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h (module 'wifi'): ns3::mpduInfo::mpduInfo(ns3::mpduInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::mpduInfo const &', 'arg0')])
    ## wifi-phy.h (module 'wifi'): ns3::mpduInfo::mpduRefNumber [variable]
    cls.add_instance_attribute('mpduRefNumber', 'uint32_t', is_const=False)
    ## wifi-phy.h (module 'wifi'): ns3::mpduInfo::type [variable]
    cls.add_instance_attribute('type', 'ns3::mpduType', is_const=False)
    return

def register_Ns3SignalNoiseDbm_methods(root_module, cls):
    ## wifi-phy.h (module 'wifi'): ns3::signalNoiseDbm::signalNoiseDbm() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h (module 'wifi'): ns3::signalNoiseDbm::signalNoiseDbm(ns3::signalNoiseDbm const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::signalNoiseDbm const &', 'arg0')])
    ## wifi-phy.h (module 'wifi'): ns3::signalNoiseDbm::noise [variable]
    cls.add_instance_attribute('noise', 'double', is_const=False)
    ## wifi-phy.h (module 'wifi'): ns3::signalNoiseDbm::signal [variable]
    cls.add_instance_attribute('signal', 'double', is_const=False)
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

def register_Ns3MgtAddBaRequestHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::MgtAddBaRequestHeader::MgtAddBaRequestHeader(ns3::MgtAddBaRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtAddBaRequestHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): ns3::MgtAddBaRequestHeader::MgtAddBaRequestHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtAddBaRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint16_t ns3::MgtAddBaRequestHeader::GetBufferSize() const [member function]
    cls.add_method('GetBufferSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::TypeId ns3::MgtAddBaRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtAddBaRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint16_t ns3::MgtAddBaRequestHeader::GetStartingSequence() const [member function]
    cls.add_method('GetStartingSequence', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): uint8_t ns3::MgtAddBaRequestHeader::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): uint16_t ns3::MgtAddBaRequestHeader::GetTimeout() const [member function]
    cls.add_method('GetTimeout', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::MgtAddBaRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h (module 'wifi'): bool ns3::MgtAddBaRequestHeader::IsAmsduSupported() const [member function]
    cls.add_method('IsAmsduSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): bool ns3::MgtAddBaRequestHeader::IsImmediateBlockAck() const [member function]
    cls.add_method('IsImmediateBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::SetAmsduSupport(bool supported) [member function]
    cls.add_method('SetAmsduSupport', 
                   'void', 
                   [param('bool', 'supported')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::SetBufferSize(uint16_t size) [member function]
    cls.add_method('SetBufferSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::SetDelayedBlockAck() [member function]
    cls.add_method('SetDelayedBlockAck', 
                   'void', 
                   [])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::SetImmediateBlockAck() [member function]
    cls.add_method('SetImmediateBlockAck', 
                   'void', 
                   [])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::SetStartingSequence(uint16_t seq) [member function]
    cls.add_method('SetStartingSequence', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::SetTid(uint8_t tid) [member function]
    cls.add_method('SetTid', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaRequestHeader::SetTimeout(uint16_t timeout) [member function]
    cls.add_method('SetTimeout', 
                   'void', 
                   [param('uint16_t', 'timeout')])
    return

def register_Ns3MgtAddBaResponseHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::MgtAddBaResponseHeader::MgtAddBaResponseHeader(ns3::MgtAddBaResponseHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtAddBaResponseHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): ns3::MgtAddBaResponseHeader::MgtAddBaResponseHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtAddBaResponseHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint16_t ns3::MgtAddBaResponseHeader::GetBufferSize() const [member function]
    cls.add_method('GetBufferSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::TypeId ns3::MgtAddBaResponseHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtAddBaResponseHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::StatusCode ns3::MgtAddBaResponseHeader::GetStatusCode() const [member function]
    cls.add_method('GetStatusCode', 
                   'ns3::StatusCode', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): uint8_t ns3::MgtAddBaResponseHeader::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): uint16_t ns3::MgtAddBaResponseHeader::GetTimeout() const [member function]
    cls.add_method('GetTimeout', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::MgtAddBaResponseHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h (module 'wifi'): bool ns3::MgtAddBaResponseHeader::IsAmsduSupported() const [member function]
    cls.add_method('IsAmsduSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): bool ns3::MgtAddBaResponseHeader::IsImmediateBlockAck() const [member function]
    cls.add_method('IsImmediateBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::SetAmsduSupport(bool supported) [member function]
    cls.add_method('SetAmsduSupport', 
                   'void', 
                   [param('bool', 'supported')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::SetBufferSize(uint16_t size) [member function]
    cls.add_method('SetBufferSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::SetDelayedBlockAck() [member function]
    cls.add_method('SetDelayedBlockAck', 
                   'void', 
                   [])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::SetImmediateBlockAck() [member function]
    cls.add_method('SetImmediateBlockAck', 
                   'void', 
                   [])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::SetStatusCode(ns3::StatusCode code) [member function]
    cls.add_method('SetStatusCode', 
                   'void', 
                   [param('ns3::StatusCode', 'code')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::SetTid(uint8_t tid) [member function]
    cls.add_method('SetTid', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAddBaResponseHeader::SetTimeout(uint16_t timeout) [member function]
    cls.add_method('SetTimeout', 
                   'void', 
                   [param('uint16_t', 'timeout')])
    return

def register_Ns3MgtAssocRequestHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::MgtAssocRequestHeader::MgtAssocRequestHeader(ns3::MgtAssocRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtAssocRequestHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): ns3::MgtAssocRequestHeader::MgtAssocRequestHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtAssocRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::CapabilityInformation ns3::MgtAssocRequestHeader::GetCapabilities() const [member function]
    cls.add_method('GetCapabilities', 
                   'ns3::CapabilityInformation', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::HtCapabilities ns3::MgtAssocRequestHeader::GetHtCapabilities() const [member function]
    cls.add_method('GetHtCapabilities', 
                   'ns3::HtCapabilities', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::TypeId ns3::MgtAssocRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint16_t ns3::MgtAssocRequestHeader::GetListenInterval() const [member function]
    cls.add_method('GetListenInterval', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtAssocRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::Ssid ns3::MgtAssocRequestHeader::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::SupportedRates ns3::MgtAssocRequestHeader::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::MgtAssocRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h (module 'wifi'): ns3::VhtCapabilities ns3::MgtAssocRequestHeader::GetVhtCapabilities() const [member function]
    cls.add_method('GetVhtCapabilities', 
                   'ns3::VhtCapabilities', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocRequestHeader::SetCapabilities(ns3::CapabilityInformation capabilities) [member function]
    cls.add_method('SetCapabilities', 
                   'void', 
                   [param('ns3::CapabilityInformation', 'capabilities')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocRequestHeader::SetHtCapabilities(ns3::HtCapabilities htcapabilities) [member function]
    cls.add_method('SetHtCapabilities', 
                   'void', 
                   [param('ns3::HtCapabilities', 'htcapabilities')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocRequestHeader::SetListenInterval(uint16_t interval) [member function]
    cls.add_method('SetListenInterval', 
                   'void', 
                   [param('uint16_t', 'interval')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocRequestHeader::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocRequestHeader::SetSupportedRates(ns3::SupportedRates rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates', 'rates')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocRequestHeader::SetVhtCapabilities(ns3::VhtCapabilities vhtcapabilities) [member function]
    cls.add_method('SetVhtCapabilities', 
                   'void', 
                   [param('ns3::VhtCapabilities', 'vhtcapabilities')])
    return

def register_Ns3MgtAssocResponseHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::MgtAssocResponseHeader::MgtAssocResponseHeader(ns3::MgtAssocResponseHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtAssocResponseHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): ns3::MgtAssocResponseHeader::MgtAssocResponseHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtAssocResponseHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::CapabilityInformation ns3::MgtAssocResponseHeader::GetCapabilities() const [member function]
    cls.add_method('GetCapabilities', 
                   'ns3::CapabilityInformation', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::ErpInformation ns3::MgtAssocResponseHeader::GetErpInformation() const [member function]
    cls.add_method('GetErpInformation', 
                   'ns3::ErpInformation', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::HtCapabilities ns3::MgtAssocResponseHeader::GetHtCapabilities() const [member function]
    cls.add_method('GetHtCapabilities', 
                   'ns3::HtCapabilities', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::HtOperations ns3::MgtAssocResponseHeader::GetHtOperations() const [member function]
    cls.add_method('GetHtOperations', 
                   'ns3::HtOperations', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::TypeId ns3::MgtAssocResponseHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtAssocResponseHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::StatusCode ns3::MgtAssocResponseHeader::GetStatusCode() [member function]
    cls.add_method('GetStatusCode', 
                   'ns3::StatusCode', 
                   [])
    ## mgt-headers.h (module 'wifi'): ns3::SupportedRates ns3::MgtAssocResponseHeader::GetSupportedRates() [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [])
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::MgtAssocResponseHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h (module 'wifi'): ns3::VhtCapabilities ns3::MgtAssocResponseHeader::GetVhtCapabilities() const [member function]
    cls.add_method('GetVhtCapabilities', 
                   'ns3::VhtCapabilities', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::SetCapabilities(ns3::CapabilityInformation capabilities) [member function]
    cls.add_method('SetCapabilities', 
                   'void', 
                   [param('ns3::CapabilityInformation', 'capabilities')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::SetErpInformation(ns3::ErpInformation erpInformation) [member function]
    cls.add_method('SetErpInformation', 
                   'void', 
                   [param('ns3::ErpInformation', 'erpInformation')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::SetHtCapabilities(ns3::HtCapabilities htcapabilities) [member function]
    cls.add_method('SetHtCapabilities', 
                   'void', 
                   [param('ns3::HtCapabilities', 'htcapabilities')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::SetHtOperations(ns3::HtOperations htoperations) [member function]
    cls.add_method('SetHtOperations', 
                   'void', 
                   [param('ns3::HtOperations', 'htoperations')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::SetStatusCode(ns3::StatusCode code) [member function]
    cls.add_method('SetStatusCode', 
                   'void', 
                   [param('ns3::StatusCode', 'code')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::SetSupportedRates(ns3::SupportedRates rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates', 'rates')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtAssocResponseHeader::SetVhtCapabilities(ns3::VhtCapabilities vhtcapabilities) [member function]
    cls.add_method('SetVhtCapabilities', 
                   'void', 
                   [param('ns3::VhtCapabilities', 'vhtcapabilities')])
    return

def register_Ns3MgtDelBaHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::MgtDelBaHeader::MgtDelBaHeader(ns3::MgtDelBaHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtDelBaHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): ns3::MgtDelBaHeader::MgtDelBaHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtDelBaHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::TypeId ns3::MgtDelBaHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtDelBaHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint8_t ns3::MgtDelBaHeader::GetTid() const [member function]
    cls.add_method('GetTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::MgtDelBaHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h (module 'wifi'): bool ns3::MgtDelBaHeader::IsByOriginator() const [member function]
    cls.add_method('IsByOriginator', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtDelBaHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtDelBaHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtDelBaHeader::SetByOriginator() [member function]
    cls.add_method('SetByOriginator', 
                   'void', 
                   [])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtDelBaHeader::SetByRecipient() [member function]
    cls.add_method('SetByRecipient', 
                   'void', 
                   [])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtDelBaHeader::SetTid(uint8_t arg0) [member function]
    cls.add_method('SetTid', 
                   'void', 
                   [param('uint8_t', 'arg0')])
    return

def register_Ns3MgtProbeRequestHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::MgtProbeRequestHeader::MgtProbeRequestHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): ns3::MgtProbeRequestHeader::MgtProbeRequestHeader(ns3::MgtProbeRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtProbeRequestHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtProbeRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::HtCapabilities ns3::MgtProbeRequestHeader::GetHtCapabilities() const [member function]
    cls.add_method('GetHtCapabilities', 
                   'ns3::HtCapabilities', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::TypeId ns3::MgtProbeRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtProbeRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::Ssid ns3::MgtProbeRequestHeader::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::SupportedRates ns3::MgtProbeRequestHeader::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::MgtProbeRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h (module 'wifi'): ns3::VhtCapabilities ns3::MgtProbeRequestHeader::GetVhtCapabilities() const [member function]
    cls.add_method('GetVhtCapabilities', 
                   'ns3::VhtCapabilities', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeRequestHeader::SetHtCapabilities(ns3::HtCapabilities htcapabilities) [member function]
    cls.add_method('SetHtCapabilities', 
                   'void', 
                   [param('ns3::HtCapabilities', 'htcapabilities')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeRequestHeader::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeRequestHeader::SetSupportedRates(ns3::SupportedRates rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates', 'rates')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeRequestHeader::SetVhtCapabilities(ns3::VhtCapabilities vhtcapabilities) [member function]
    cls.add_method('SetVhtCapabilities', 
                   'void', 
                   [param('ns3::VhtCapabilities', 'vhtcapabilities')])
    return

def register_Ns3MgtProbeResponseHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::MgtProbeResponseHeader::MgtProbeResponseHeader(ns3::MgtProbeResponseHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtProbeResponseHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): ns3::MgtProbeResponseHeader::MgtProbeResponseHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtProbeResponseHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint64_t ns3::MgtProbeResponseHeader::GetBeaconIntervalUs() const [member function]
    cls.add_method('GetBeaconIntervalUs', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::CapabilityInformation ns3::MgtProbeResponseHeader::GetCapabilities() const [member function]
    cls.add_method('GetCapabilities', 
                   'ns3::CapabilityInformation', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::ErpInformation ns3::MgtProbeResponseHeader::GetErpInformation() const [member function]
    cls.add_method('GetErpInformation', 
                   'ns3::ErpInformation', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::HtCapabilities ns3::MgtProbeResponseHeader::GetHtCapabilities() const [member function]
    cls.add_method('GetHtCapabilities', 
                   'ns3::HtCapabilities', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::HtOperations ns3::MgtProbeResponseHeader::GetHtOperations() const [member function]
    cls.add_method('GetHtOperations', 
                   'ns3::HtOperations', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::TypeId ns3::MgtProbeResponseHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::MgtProbeResponseHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::Ssid ns3::MgtProbeResponseHeader::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): ns3::SupportedRates ns3::MgtProbeResponseHeader::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): uint64_t ns3::MgtProbeResponseHeader::GetTimestamp() [member function]
    cls.add_method('GetTimestamp', 
                   'uint64_t', 
                   [])
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::MgtProbeResponseHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h (module 'wifi'): ns3::VhtCapabilities ns3::MgtProbeResponseHeader::GetVhtCapabilities() const [member function]
    cls.add_method('GetVhtCapabilities', 
                   'ns3::VhtCapabilities', 
                   [], 
                   is_const=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::SetBeaconIntervalUs(uint64_t us) [member function]
    cls.add_method('SetBeaconIntervalUs', 
                   'void', 
                   [param('uint64_t', 'us')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::SetCapabilities(ns3::CapabilityInformation capabilities) [member function]
    cls.add_method('SetCapabilities', 
                   'void', 
                   [param('ns3::CapabilityInformation', 'capabilities')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::SetErpInformation(ns3::ErpInformation erpInformation) [member function]
    cls.add_method('SetErpInformation', 
                   'void', 
                   [param('ns3::ErpInformation', 'erpInformation')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::SetHtCapabilities(ns3::HtCapabilities htcapabilities) [member function]
    cls.add_method('SetHtCapabilities', 
                   'void', 
                   [param('ns3::HtCapabilities', 'htcapabilities')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::SetHtOperations(ns3::HtOperations htoperations) [member function]
    cls.add_method('SetHtOperations', 
                   'void', 
                   [param('ns3::HtOperations', 'htoperations')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::SetSupportedRates(ns3::SupportedRates rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates', 'rates')])
    ## mgt-headers.h (module 'wifi'): void ns3::MgtProbeResponseHeader::SetVhtCapabilities(ns3::VhtCapabilities vhtcapabilities) [member function]
    cls.add_method('SetVhtCapabilities', 
                   'void', 
                   [param('ns3::VhtCapabilities', 'vhtcapabilities')])
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
    ## object.h (module 'core'): void ns3::Object::Initialize() [member function]
    cls.add_method('Initialize', 
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
    ## object.h (module 'core'): void ns3::Object::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
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

def register_Ns3PcapFileWrapper_methods(root_module, cls):
    ## pcap-file-wrapper.h (module 'network'): static ns3::TypeId ns3::PcapFileWrapper::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## pcap-file-wrapper.h (module 'network'): ns3::PcapFileWrapper::PcapFileWrapper() [constructor]
    cls.add_constructor([])
    ## pcap-file-wrapper.h (module 'network'): bool ns3::PcapFileWrapper::Fail() const [member function]
    cls.add_method('Fail', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file-wrapper.h (module 'network'): bool ns3::PcapFileWrapper::Eof() const [member function]
    cls.add_method('Eof', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Open(std::string const & filename, std::_Ios_Openmode mode) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string const &', 'filename'), param('std::_Ios_Openmode', 'mode')])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Close() [member function]
    cls.add_method('Close', 
                   'void', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Init(uint32_t dataLinkType, uint32_t snapLen=std::numeric_limits<unsigned int>::max(), int32_t tzCorrection=ns3::PcapFile::ZONE_DEFAULT) [member function]
    cls.add_method('Init', 
                   'void', 
                   [param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='std::numeric_limits<unsigned int>::max()'), param('int32_t', 'tzCorrection', default_value='ns3::PcapFile::ZONE_DEFAULT')])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Write(ns3::Time t, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Write(ns3::Time t, ns3::Header const & header, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('ns3::Header const &', 'header'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Write(ns3::Time t, uint8_t const * buffer, uint32_t length) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('uint8_t const *', 'buffer'), param('uint32_t', 'length')])
    ## pcap-file-wrapper.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::PcapFileWrapper::Read(ns3::Time & t) [member function]
    cls.add_method('Read', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Time &', 't')])
    ## pcap-file-wrapper.h (module 'network'): uint32_t ns3::PcapFileWrapper::GetMagic() [member function]
    cls.add_method('GetMagic', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint16_t ns3::PcapFileWrapper::GetVersionMajor() [member function]
    cls.add_method('GetVersionMajor', 
                   'uint16_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint16_t ns3::PcapFileWrapper::GetVersionMinor() [member function]
    cls.add_method('GetVersionMinor', 
                   'uint16_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): int32_t ns3::PcapFileWrapper::GetTimeZoneOffset() [member function]
    cls.add_method('GetTimeZoneOffset', 
                   'int32_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint32_t ns3::PcapFileWrapper::GetSigFigs() [member function]
    cls.add_method('GetSigFigs', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint32_t ns3::PcapFileWrapper::GetSnapLen() [member function]
    cls.add_method('GetSnapLen', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint32_t ns3::PcapFileWrapper::GetDataLinkType() [member function]
    cls.add_method('GetDataLinkType', 
                   'uint32_t', 
                   [])
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

def register_Ns3SimpleRefCount__Ns3HashImplementation_Ns3Empty_Ns3DefaultDeleter__lt__ns3HashImplementation__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter< ns3::Hash::Implementation > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3MeshWifiInterfaceMacPlugin_Ns3Empty_Ns3DefaultDeleter__lt__ns3MeshWifiInterfaceMacPlugin__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >::SimpleRefCount(ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter< ns3::MeshWifiInterfaceMacPlugin > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NetDeviceQueue_Ns3Empty_Ns3DefaultDeleter__lt__ns3NetDeviceQueue__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NetDeviceQueue, ns3::empty, ns3::DefaultDeleter<ns3::NetDeviceQueue> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NetDeviceQueue, ns3::empty, ns3::DefaultDeleter<ns3::NetDeviceQueue> >::SimpleRefCount(ns3::SimpleRefCount<ns3::NetDeviceQueue, ns3::empty, ns3::DefaultDeleter<ns3::NetDeviceQueue> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::NetDeviceQueue, ns3::empty, ns3::DefaultDeleter< ns3::NetDeviceQueue > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::NetDeviceQueue, ns3::empty, ns3::DefaultDeleter<ns3::NetDeviceQueue> >::Cleanup() [member function]
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

def register_Ns3SimpleRefCount__Ns3OutputStreamWrapper_Ns3Empty_Ns3DefaultDeleter__lt__ns3OutputStreamWrapper__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::SimpleRefCount(ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter< ns3::OutputStreamWrapper > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::Cleanup() [member function]
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

def register_Ns3SimpleRefCount__Ns3QueueItem_Ns3Empty_Ns3DefaultDeleter__lt__ns3QueueItem__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::QueueItem, ns3::empty, ns3::DefaultDeleter<ns3::QueueItem> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::QueueItem, ns3::empty, ns3::DefaultDeleter<ns3::QueueItem> >::SimpleRefCount(ns3::SimpleRefCount<ns3::QueueItem, ns3::empty, ns3::DefaultDeleter<ns3::QueueItem> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::QueueItem, ns3::empty, ns3::DefaultDeleter< ns3::QueueItem > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::QueueItem, ns3::empty, ns3::DefaultDeleter<ns3::QueueItem> >::Cleanup() [member function]
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

def register_Ns3SimpleRefCount__Ns3WifiInformationElement_Ns3Empty_Ns3DefaultDeleter__lt__ns3WifiInformationElement__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >::SimpleRefCount(ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter< ns3::WifiInformationElement > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Dot11sDestinationAddressUnit_Ns3Empty_Ns3DefaultDeleter__lt__ns3Dot11sDestinationAddressUnit__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> >::SimpleRefCount(ns3::SimpleRefCount<ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter< ns3::dot11s::DestinationAddressUnit > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::dot11s::DestinationAddressUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::DestinationAddressUnit> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Dot11sIeBeaconTimingUnit_Ns3Empty_Ns3DefaultDeleter__lt__ns3Dot11sIeBeaconTimingUnit__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >::SimpleRefCount(ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter< ns3::dot11s::IeBeaconTimingUnit > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3Time_methods(root_module, cls):
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_numeric_operator('*', root_module['ns3::Time'], root_module['ns3::Time'], param('int64_t const &', u'right'))
    cls.add_inplace_numeric_operator('+=', param('ns3::Time const &', u'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', u'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', u'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::Time'], root_module['ns3::Time'], param('int64_t const &', u'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    cls.add_inplace_numeric_operator('-=', param('ns3::Time const &', u'right'))
    cls.add_output_stream_operator()
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
    ## nstime.h (module 'core'): ns3::Time::Time(ns3::int64x64_t const & v) [constructor]
    cls.add_constructor([param('ns3::int64x64_t const &', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(std::string const & s) [constructor]
    cls.add_constructor([param('std::string const &', 's')])
    ## nstime.h (module 'core'): ns3::TimeWithUnit ns3::Time::As(ns3::Time::Unit const unit) const [member function]
    cls.add_method('As', 
                   'ns3::TimeWithUnit', 
                   [param('ns3::Time::Unit const', 'unit')], 
                   is_const=True)
    ## nstime.h (module 'core'): int ns3::Time::Compare(ns3::Time const & o) const [member function]
    cls.add_method('Compare', 
                   'int', 
                   [param('ns3::Time const &', 'o')], 
                   is_const=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::From(ns3::int64x64_t const & value) [member function]
    cls.add_method('From', 
                   'ns3::Time', 
                   [param('ns3::int64x64_t const &', 'value')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::From(ns3::int64x64_t const & value, ns3::Time::Unit unit) [member function]
    cls.add_method('From', 
                   'ns3::Time', 
                   [param('ns3::int64x64_t const &', 'value'), param('ns3::Time::Unit', 'unit')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::FromDouble(double value, ns3::Time::Unit unit) [member function]
    cls.add_method('FromDouble', 
                   'ns3::Time', 
                   [param('double', 'value'), param('ns3::Time::Unit', 'unit')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::FromInteger(uint64_t value, ns3::Time::Unit unit) [member function]
    cls.add_method('FromInteger', 
                   'ns3::Time', 
                   [param('uint64_t', 'value'), param('ns3::Time::Unit', 'unit')], 
                   is_static=True)
    ## nstime.h (module 'core'): double ns3::Time::GetDays() const [member function]
    cls.add_method('GetDays', 
                   'double', 
                   [], 
                   is_const=True)
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
    ## nstime.h (module 'core'): double ns3::Time::GetHours() const [member function]
    cls.add_method('GetHours', 
                   'double', 
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
    ## nstime.h (module 'core'): double ns3::Time::GetMinutes() const [member function]
    cls.add_method('GetMinutes', 
                   'double', 
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
    ## nstime.h (module 'core'): double ns3::Time::GetYears() const [member function]
    cls.add_method('GetYears', 
                   'double', 
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
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::Max() [member function]
    cls.add_method('Max', 
                   'ns3::Time', 
                   [], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::Min() [member function]
    cls.add_method('Min', 
                   'ns3::Time', 
                   [], 
                   is_static=True)
    ## nstime.h (module 'core'): static void ns3::Time::SetResolution(ns3::Time::Unit resolution) [member function]
    cls.add_method('SetResolution', 
                   'void', 
                   [param('ns3::Time::Unit', 'resolution')], 
                   is_static=True)
    ## nstime.h (module 'core'): static bool ns3::Time::StaticInit() [member function]
    cls.add_method('StaticInit', 
                   'bool', 
                   [], 
                   is_static=True)
    ## nstime.h (module 'core'): ns3::int64x64_t ns3::Time::To(ns3::Time::Unit unit) const [member function]
    cls.add_method('To', 
                   'ns3::int64x64_t', 
                   [param('ns3::Time::Unit', 'unit')], 
                   is_const=True)
    ## nstime.h (module 'core'): double ns3::Time::ToDouble(ns3::Time::Unit unit) const [member function]
    cls.add_method('ToDouble', 
                   'double', 
                   [param('ns3::Time::Unit', 'unit')], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::ToInteger(ns3::Time::Unit unit) const [member function]
    cls.add_method('ToInteger', 
                   'int64_t', 
                   [param('ns3::Time::Unit', 'unit')], 
                   is_const=True)
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

def register_Ns3WifiActionHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::WifiActionHeader(ns3::WifiActionHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiActionHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::WifiActionHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::WifiActionHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::ActionValue ns3::WifiActionHeader::GetAction() [member function]
    cls.add_method('GetAction', 
                   'ns3::WifiActionHeader::ActionValue', 
                   [])
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::CategoryValue ns3::WifiActionHeader::GetCategory() [member function]
    cls.add_method('GetCategory', 
                   'ns3::WifiActionHeader::CategoryValue', 
                   [])
    ## mgt-headers.h (module 'wifi'): ns3::TypeId ns3::WifiActionHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): uint32_t ns3::WifiActionHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::WifiActionHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mgt-headers.h (module 'wifi'): void ns3::WifiActionHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::WifiActionHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## mgt-headers.h (module 'wifi'): void ns3::WifiActionHeader::SetAction(ns3::WifiActionHeader::CategoryValue type, ns3::WifiActionHeader::ActionValue action) [member function]
    cls.add_method('SetAction', 
                   'void', 
                   [param('ns3::WifiActionHeader::CategoryValue', 'type'), param('ns3::WifiActionHeader::ActionValue', 'action')])
    return

def register_Ns3WifiActionHeaderActionValue_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::ActionValue::ActionValue() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::ActionValue::ActionValue(ns3::WifiActionHeader::ActionValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiActionHeader::ActionValue const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::ActionValue::blockAck [variable]
    cls.add_instance_attribute('blockAck', 'ns3::WifiActionHeader::BlockAckActionValue', is_const=False)
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::ActionValue::meshAction [variable]
    cls.add_instance_attribute('meshAction', 'ns3::WifiActionHeader::MeshActionValue', is_const=False)
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::ActionValue::multihopAction [variable]
    cls.add_instance_attribute('multihopAction', 'ns3::WifiActionHeader::MultihopActionValue', is_const=False)
    ## mgt-headers.h (module 'wifi'): ns3::WifiActionHeader::ActionValue::selfProtectedAction [variable]
    cls.add_instance_attribute('selfProtectedAction', 'ns3::WifiActionHeader::SelfProtectedActionValue', is_const=False)
    return

def register_Ns3WifiInformationElement_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## wifi-information-element.h (module 'wifi'): ns3::WifiInformationElement::WifiInformationElement() [constructor]
    cls.add_constructor([])
    ## wifi-information-element.h (module 'wifi'): ns3::WifiInformationElement::WifiInformationElement(ns3::WifiInformationElement const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiInformationElement const &', 'arg0')])
    ## wifi-information-element.h (module 'wifi'): ns3::Buffer::Iterator ns3::WifiInformationElement::Deserialize(ns3::Buffer::Iterator i) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')])
    ## wifi-information-element.h (module 'wifi'): ns3::Buffer::Iterator ns3::WifiInformationElement::DeserializeIfPresent(ns3::Buffer::Iterator i) [member function]
    cls.add_method('DeserializeIfPresent', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')])
    ## wifi-information-element.h (module 'wifi'): uint8_t ns3::WifiInformationElement::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-information-element.h (module 'wifi'): ns3::WifiInformationElementId ns3::WifiInformationElement::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-information-element.h (module 'wifi'): uint8_t ns3::WifiInformationElement::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-information-element.h (module 'wifi'): uint16_t ns3::WifiInformationElement::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-information-element.h (module 'wifi'): void ns3::WifiInformationElement::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element.h (module 'wifi'): ns3::Buffer::Iterator ns3::WifiInformationElement::Serialize(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True)
    ## wifi-information-element.h (module 'wifi'): void ns3::WifiInformationElement::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3WifiInformationElementVector_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## wifi-information-element-vector.h (module 'wifi'): ns3::WifiInformationElementVector::WifiInformationElementVector(ns3::WifiInformationElementVector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiInformationElementVector const &', 'arg0')])
    ## wifi-information-element-vector.h (module 'wifi'): ns3::WifiInformationElementVector::WifiInformationElementVector() [constructor]
    cls.add_constructor([])
    ## wifi-information-element-vector.h (module 'wifi'): bool ns3::WifiInformationElementVector::AddInformationElement(ns3::Ptr<ns3::WifiInformationElement> element) [member function]
    cls.add_method('AddInformationElement', 
                   'bool', 
                   [param('ns3::Ptr< ns3::WifiInformationElement >', 'element')])
    ## wifi-information-element-vector.h (module 'wifi'): __gnu_cxx::__normal_iterator<ns3::Ptr<ns3::WifiInformationElement>*,std::vector<ns3::Ptr<ns3::WifiInformationElement>, std::allocator<ns3::Ptr<ns3::WifiInformationElement> > > > ns3::WifiInformationElementVector::Begin() [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::WifiInformationElement >, std::vector< ns3::Ptr< ns3::WifiInformationElement > > >', 
                   [])
    ## wifi-information-element-vector.h (module 'wifi'): uint32_t ns3::WifiInformationElementVector::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wifi-information-element-vector.h (module 'wifi'): uint32_t ns3::WifiInformationElementVector::DeserializeSingleIe(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DeserializeSingleIe', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wifi-information-element-vector.h (module 'wifi'): __gnu_cxx::__normal_iterator<ns3::Ptr<ns3::WifiInformationElement>*,std::vector<ns3::Ptr<ns3::WifiInformationElement>, std::allocator<ns3::Ptr<ns3::WifiInformationElement> > > > ns3::WifiInformationElementVector::End() [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::WifiInformationElement >, std::vector< ns3::Ptr< ns3::WifiInformationElement > > >', 
                   [])
    ## wifi-information-element-vector.h (module 'wifi'): ns3::Ptr<ns3::WifiInformationElement> ns3::WifiInformationElementVector::FindFirst(ns3::WifiInformationElementId id) const [member function]
    cls.add_method('FindFirst', 
                   'ns3::Ptr< ns3::WifiInformationElement >', 
                   [param('ns3::WifiInformationElementId', 'id')], 
                   is_const=True)
    ## wifi-information-element-vector.h (module 'wifi'): ns3::TypeId ns3::WifiInformationElementVector::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element-vector.h (module 'wifi'): uint32_t ns3::WifiInformationElementVector::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element-vector.h (module 'wifi'): static ns3::TypeId ns3::WifiInformationElementVector::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-information-element-vector.h (module 'wifi'): void ns3::WifiInformationElementVector::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element-vector.h (module 'wifi'): void ns3::WifiInformationElementVector::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wifi-information-element-vector.h (module 'wifi'): void ns3::WifiInformationElementVector::SetMaxSize(uint16_t size) [member function]
    cls.add_method('SetMaxSize', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## wifi-information-element-vector.h (module 'wifi'): uint32_t ns3::WifiInformationElementVector::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, visibility='protected')
    return

def register_Ns3WifiMac_methods(root_module, cls):
    ## wifi-mac.h (module 'wifi'): ns3::WifiMac::WifiMac() [constructor]
    cls.add_constructor([])
    ## wifi-mac.h (module 'wifi'): ns3::WifiMac::WifiMac(ns3::WifiMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMac const &', 'arg0')])
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::ConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('ConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')])
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Mac48Address ns3::WifiMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetBasicBlockAckTimeout() const [member function]
    cls.add_method('GetBasicBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Mac48Address ns3::WifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetCompressedBlockAckTimeout() const [member function]
    cls.add_method('GetCompressedBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetMaxPropagationDelay() const [member function]
    cls.add_method('GetMaxPropagationDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetMsduLifetime() const [member function]
    cls.add_method('GetMsduLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetRifs() const [member function]
    cls.add_method('GetRifs', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): bool ns3::WifiMac::GetShortSlotTimeSupported() const [member function]
    cls.add_method('GetShortSlotTimeSupported', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Time ns3::WifiMac::GetSlot() const [member function]
    cls.add_method('GetSlot', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Ssid ns3::WifiMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): static ns3::TypeId ns3::WifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::WifiPhy> ns3::WifiMac::GetWifiPhy() const [member function]
    cls.add_method('GetWifiPhy', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::WifiRemoteStationManager> ns3::WifiMac::GetWifiRemoteStationManager() const [member function]
    cls.add_method('GetWifiRemoteStationManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::NotifyPromiscRx(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyPromiscRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::NotifyRx(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::NotifyRxDrop(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::NotifyTx(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::NotifyTxDrop(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::ResetWifiPhy() [member function]
    cls.add_method('ResetWifiPhy', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetBasicBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetBasicBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')], 
                   is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetCompressedBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetCompressedBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')], 
                   is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetMaxPropagationDelay(ns3::Time delay) [member function]
    cls.add_method('SetMaxPropagationDelay', 
                   'void', 
                   [param('ns3::Time', 'delay')])
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetPromisc() [member function]
    cls.add_method('SetPromisc', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetRifs(ns3::Time rifs) [member function]
    cls.add_method('SetRifs', 
                   'void', 
                   [param('ns3::Time', 'rifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetShortSlotTimeSupported(bool enable) [member function]
    cls.add_method('SetShortSlotTimeSupported', 
                   'void', 
                   [param('bool', 'enable')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): bool ns3::WifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::ConfigureDcf(ns3::Ptr<ns3::Dcf> dcf, uint32_t cwmin, uint32_t cwmax, ns3::AcIndex ac) [member function]
    cls.add_method('ConfigureDcf', 
                   'void', 
                   [param('ns3::Ptr< ns3::Dcf >', 'dcf'), param('uint32_t', 'cwmin'), param('uint32_t', 'cwmax'), param('ns3::AcIndex', 'ac')], 
                   visibility='protected')
    ## wifi-mac.h (module 'wifi'): void ns3::WifiMac::FinishConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('FinishConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3WifiMacHeader_methods(root_module, cls):
    ## wifi-mac-header.h (module 'wifi'): ns3::WifiMacHeader::WifiMacHeader(ns3::WifiMacHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiMacHeader const &', 'arg0')])
    ## wifi-mac-header.h (module 'wifi'): ns3::WifiMacHeader::WifiMacHeader() [constructor]
    cls.add_constructor([])
    ## wifi-mac-header.h (module 'wifi'): uint32_t ns3::WifiMacHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wifi-mac-header.h (module 'wifi'): ns3::Mac48Address ns3::WifiMacHeader::GetAddr1() const [member function]
    cls.add_method('GetAddr1', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): ns3::Mac48Address ns3::WifiMacHeader::GetAddr2() const [member function]
    cls.add_method('GetAddr2', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): ns3::Mac48Address ns3::WifiMacHeader::GetAddr3() const [member function]
    cls.add_method('GetAddr3', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): ns3::Mac48Address ns3::WifiMacHeader::GetAddr4() const [member function]
    cls.add_method('GetAddr4', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): ns3::Time ns3::WifiMacHeader::GetDuration() const [member function]
    cls.add_method('GetDuration', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): uint16_t ns3::WifiMacHeader::GetFragmentNumber() const [member function]
    cls.add_method('GetFragmentNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): ns3::TypeId ns3::WifiMacHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-header.h (module 'wifi'): ns3::WifiMacHeader::QosAckPolicy ns3::WifiMacHeader::GetQosAckPolicy() const [member function]
    cls.add_method('GetQosAckPolicy', 
                   'ns3::WifiMacHeader::QosAckPolicy', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): uint8_t ns3::WifiMacHeader::GetQosTid() const [member function]
    cls.add_method('GetQosTid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): uint8_t ns3::WifiMacHeader::GetQosTxopLimit() const [member function]
    cls.add_method('GetQosTxopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): uint16_t ns3::WifiMacHeader::GetRawDuration() const [member function]
    cls.add_method('GetRawDuration', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): uint16_t ns3::WifiMacHeader::GetSequenceControl() const [member function]
    cls.add_method('GetSequenceControl', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): uint16_t ns3::WifiMacHeader::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): uint32_t ns3::WifiMacHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-header.h (module 'wifi'): uint32_t ns3::WifiMacHeader::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): ns3::WifiMacType ns3::WifiMacHeader::GetType() const [member function]
    cls.add_method('GetType', 
                   'ns3::WifiMacType', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): static ns3::TypeId ns3::WifiMacHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-mac-header.h (module 'wifi'): char const * ns3::WifiMacHeader::GetTypeString() const [member function]
    cls.add_method('GetTypeString', 
                   'char const *', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsAck() const [member function]
    cls.add_method('IsAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsAction() const [member function]
    cls.add_method('IsAction', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsAssocReq() const [member function]
    cls.add_method('IsAssocReq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsAssocResp() const [member function]
    cls.add_method('IsAssocResp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsAuthentication() const [member function]
    cls.add_method('IsAuthentication', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsBeacon() const [member function]
    cls.add_method('IsBeacon', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsBlockAck() const [member function]
    cls.add_method('IsBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsBlockAckReq() const [member function]
    cls.add_method('IsBlockAckReq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsCfpoll() const [member function]
    cls.add_method('IsCfpoll', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsCtl() const [member function]
    cls.add_method('IsCtl', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsCts() const [member function]
    cls.add_method('IsCts', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsData() const [member function]
    cls.add_method('IsData', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsDeauthentication() const [member function]
    cls.add_method('IsDeauthentication', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsDisassociation() const [member function]
    cls.add_method('IsDisassociation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsFromDs() const [member function]
    cls.add_method('IsFromDs', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsMgt() const [member function]
    cls.add_method('IsMgt', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsMoreFragments() const [member function]
    cls.add_method('IsMoreFragments', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsMultihopAction() const [member function]
    cls.add_method('IsMultihopAction', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsProbeReq() const [member function]
    cls.add_method('IsProbeReq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsProbeResp() const [member function]
    cls.add_method('IsProbeResp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsQosAck() const [member function]
    cls.add_method('IsQosAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsQosAmsdu() const [member function]
    cls.add_method('IsQosAmsdu', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsQosBlockAck() const [member function]
    cls.add_method('IsQosBlockAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsQosData() const [member function]
    cls.add_method('IsQosData', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsQosEosp() const [member function]
    cls.add_method('IsQosEosp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsQosNoAck() const [member function]
    cls.add_method('IsQosNoAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsReassocReq() const [member function]
    cls.add_method('IsReassocReq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsReassocResp() const [member function]
    cls.add_method('IsReassocResp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsRetry() const [member function]
    cls.add_method('IsRetry', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsRts() const [member function]
    cls.add_method('IsRts', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): bool ns3::WifiMacHeader::IsToDs() const [member function]
    cls.add_method('IsToDs', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetAction() [member function]
    cls.add_method('SetAction', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetAddr1(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr1', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetAddr2(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr2', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetAddr3(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr3', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetAddr4(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr4', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetAssocReq() [member function]
    cls.add_method('SetAssocReq', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetAssocResp() [member function]
    cls.add_method('SetAssocResp', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetBeacon() [member function]
    cls.add_method('SetBeacon', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetBlockAck() [member function]
    cls.add_method('SetBlockAck', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetBlockAckReq() [member function]
    cls.add_method('SetBlockAckReq', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetDsFrom() [member function]
    cls.add_method('SetDsFrom', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetDsNotFrom() [member function]
    cls.add_method('SetDsNotFrom', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetDsNotTo() [member function]
    cls.add_method('SetDsNotTo', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetDsTo() [member function]
    cls.add_method('SetDsTo', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetDuration(ns3::Time duration) [member function]
    cls.add_method('SetDuration', 
                   'void', 
                   [param('ns3::Time', 'duration')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetFragmentNumber(uint8_t frag) [member function]
    cls.add_method('SetFragmentNumber', 
                   'void', 
                   [param('uint8_t', 'frag')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetId(uint16_t id) [member function]
    cls.add_method('SetId', 
                   'void', 
                   [param('uint16_t', 'id')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetMoreFragments() [member function]
    cls.add_method('SetMoreFragments', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetMultihopAction() [member function]
    cls.add_method('SetMultihopAction', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetNoMoreFragments() [member function]
    cls.add_method('SetNoMoreFragments', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetNoOrder() [member function]
    cls.add_method('SetNoOrder', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetNoRetry() [member function]
    cls.add_method('SetNoRetry', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetOrder() [member function]
    cls.add_method('SetOrder', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetProbeReq() [member function]
    cls.add_method('SetProbeReq', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetProbeResp() [member function]
    cls.add_method('SetProbeResp', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosAckPolicy(ns3::WifiMacHeader::QosAckPolicy policy) [member function]
    cls.add_method('SetQosAckPolicy', 
                   'void', 
                   [param('ns3::WifiMacHeader::QosAckPolicy', 'policy')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosAmsdu() [member function]
    cls.add_method('SetQosAmsdu', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosBlockAck() [member function]
    cls.add_method('SetQosBlockAck', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosEosp() [member function]
    cls.add_method('SetQosEosp', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosMeshControlPresent() [member function]
    cls.add_method('SetQosMeshControlPresent', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosNoAck() [member function]
    cls.add_method('SetQosNoAck', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosNoAmsdu() [member function]
    cls.add_method('SetQosNoAmsdu', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosNoEosp() [member function]
    cls.add_method('SetQosNoEosp', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosNoMeshControlPresent() [member function]
    cls.add_method('SetQosNoMeshControlPresent', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosNormalAck() [member function]
    cls.add_method('SetQosNormalAck', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosTid(uint8_t tid) [member function]
    cls.add_method('SetQosTid', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetQosTxopLimit(uint8_t txop) [member function]
    cls.add_method('SetQosTxopLimit', 
                   'void', 
                   [param('uint8_t', 'txop')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetRawDuration(uint16_t duration) [member function]
    cls.add_method('SetRawDuration', 
                   'void', 
                   [param('uint16_t', 'duration')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetRetry() [member function]
    cls.add_method('SetRetry', 
                   'void', 
                   [])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetSequenceNumber(uint16_t seq) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetType(ns3::WifiMacType type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('ns3::WifiMacType', 'type')])
    ## wifi-mac-header.h (module 'wifi'): void ns3::WifiMacHeader::SetTypeData() [member function]
    cls.add_method('SetTypeData', 
                   'void', 
                   [])
    return

def register_Ns3WifiPhy_methods(root_module, cls):
    ## wifi-phy.h (module 'wifi'): ns3::WifiPhy::WifiPhy(ns3::WifiPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiPhy const &', 'arg0')])
    ## wifi-phy.h (module 'wifi'): ns3::WifiPhy::WifiPhy() [constructor]
    cls.add_constructor([])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::AddSupportedChannelWidth(uint32_t width) [member function]
    cls.add_method('AddSupportedChannelWidth', 
                   'void', 
                   [param('uint32_t', 'width')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): int64_t ns3::WifiPhy::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::CalculatePlcpPreambleAndHeaderDuration(ns3::WifiTxVector txVector, ns3::WifiPreamble preamble) [member function]
    cls.add_method('CalculatePlcpPreambleAndHeaderDuration', 
                   'ns3::Time', 
                   [param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble')])
    ## wifi-phy.h (module 'wifi'): double ns3::WifiPhy::CalculateSnr(ns3::WifiTxVector txVector, double ber) const [member function]
    cls.add_method('CalculateSnr', 
                   'double', 
                   [param('ns3::WifiTxVector', 'txVector'), param('double', 'ber')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::CalculateTxDuration(uint32_t size, ns3::WifiTxVector txVector, ns3::WifiPreamble preamble, double frequency) [member function]
    cls.add_method('CalculateTxDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble'), param('double', 'frequency')])
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::CalculateTxDuration(uint32_t size, ns3::WifiTxVector txVector, ns3::WifiPreamble preamble, double frequency, ns3::mpduType mpdutype, uint8_t incFlag) [member function]
    cls.add_method('CalculateTxDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble'), param('double', 'frequency'), param('ns3::mpduType', 'mpdutype'), param('uint8_t', 'incFlag')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::ConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('ConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint32_t ns3::WifiPhy::GetBssMembershipSelector(uint32_t selector) const [member function]
    cls.add_method('GetBssMembershipSelector', 
                   'uint32_t', 
                   [param('uint32_t', 'selector')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::Ptr<ns3::WifiChannel> ns3::WifiPhy::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::WifiChannel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint16_t ns3::WifiPhy::GetChannelNumber() const [member function]
    cls.add_method('GetChannelNumber', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::GetChannelSwitchDelay() const [member function]
    cls.add_method('GetChannelSwitchDelay', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint32_t ns3::WifiPhy::GetChannelWidth() const [member function]
    cls.add_method('GetChannelWidth', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::GetDelayUntilIdle() [member function]
    cls.add_method('GetDelayUntilIdle', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetDsssRate11Mbps() [member function]
    cls.add_method('GetDsssRate11Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetDsssRate1Mbps() [member function]
    cls.add_method('GetDsssRate1Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetDsssRate2Mbps() [member function]
    cls.add_method('GetDsssRate2Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetDsssRate5_5Mbps() [member function]
    cls.add_method('GetDsssRate5_5Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate12Mbps() [member function]
    cls.add_method('GetErpOfdmRate12Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate18Mbps() [member function]
    cls.add_method('GetErpOfdmRate18Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate24Mbps() [member function]
    cls.add_method('GetErpOfdmRate24Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate36Mbps() [member function]
    cls.add_method('GetErpOfdmRate36Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate48Mbps() [member function]
    cls.add_method('GetErpOfdmRate48Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate54Mbps() [member function]
    cls.add_method('GetErpOfdmRate54Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate6Mbps() [member function]
    cls.add_method('GetErpOfdmRate6Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetErpOfdmRate9Mbps() [member function]
    cls.add_method('GetErpOfdmRate9Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): uint32_t ns3::WifiPhy::GetFrequency() const [member function]
    cls.add_method('GetFrequency', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::GetGreenfield() const [member function]
    cls.add_method('GetGreenfield', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::GetGuardInterval() const [member function]
    cls.add_method('GetGuardInterval', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs0() [member function]
    cls.add_method('GetHtMcs0', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs1() [member function]
    cls.add_method('GetHtMcs1', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs10() [member function]
    cls.add_method('GetHtMcs10', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs11() [member function]
    cls.add_method('GetHtMcs11', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs12() [member function]
    cls.add_method('GetHtMcs12', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs13() [member function]
    cls.add_method('GetHtMcs13', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs14() [member function]
    cls.add_method('GetHtMcs14', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs15() [member function]
    cls.add_method('GetHtMcs15', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs16() [member function]
    cls.add_method('GetHtMcs16', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs17() [member function]
    cls.add_method('GetHtMcs17', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs18() [member function]
    cls.add_method('GetHtMcs18', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs19() [member function]
    cls.add_method('GetHtMcs19', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs2() [member function]
    cls.add_method('GetHtMcs2', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs20() [member function]
    cls.add_method('GetHtMcs20', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs21() [member function]
    cls.add_method('GetHtMcs21', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs22() [member function]
    cls.add_method('GetHtMcs22', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs23() [member function]
    cls.add_method('GetHtMcs23', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs24() [member function]
    cls.add_method('GetHtMcs24', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs25() [member function]
    cls.add_method('GetHtMcs25', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs26() [member function]
    cls.add_method('GetHtMcs26', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs27() [member function]
    cls.add_method('GetHtMcs27', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs28() [member function]
    cls.add_method('GetHtMcs28', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs29() [member function]
    cls.add_method('GetHtMcs29', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs3() [member function]
    cls.add_method('GetHtMcs3', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs30() [member function]
    cls.add_method('GetHtMcs30', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs31() [member function]
    cls.add_method('GetHtMcs31', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs4() [member function]
    cls.add_method('GetHtMcs4', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs5() [member function]
    cls.add_method('GetHtMcs5', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs6() [member function]
    cls.add_method('GetHtMcs6', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs7() [member function]
    cls.add_method('GetHtMcs7', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs8() [member function]
    cls.add_method('GetHtMcs8', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtMcs9() [member function]
    cls.add_method('GetHtMcs9', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetHtPlcpHeaderMode(ns3::WifiMode payloadMode) [member function]
    cls.add_method('GetHtPlcpHeaderMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiMode', 'payloadMode')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::GetLastRxStartTime() const [member function]
    cls.add_method('GetLastRxStartTime', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::GetLdpc() const [member function]
    cls.add_method('GetLdpc', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::WifiMode ns3::WifiPhy::GetMcs(uint8_t mcs) const [member function]
    cls.add_method('GetMcs', 
                   'ns3::WifiMode', 
                   [param('uint8_t', 'mcs')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::WifiModeList ns3::WifiPhy::GetMembershipSelectorModes(uint32_t selector) [member function]
    cls.add_method('GetMembershipSelectorModes', 
                   'ns3::WifiModeList', 
                   [param('uint32_t', 'selector')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::WifiMode ns3::WifiPhy::GetMode(uint32_t mode) const [member function]
    cls.add_method('GetMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'mode')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint32_t ns3::WifiPhy::GetNBssMembershipSelectors() const [member function]
    cls.add_method('GetNBssMembershipSelectors', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint8_t ns3::WifiPhy::GetNMcs() const [member function]
    cls.add_method('GetNMcs', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint32_t ns3::WifiPhy::GetNModes() const [member function]
    cls.add_method('GetNModes', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint32_t ns3::WifiPhy::GetNTxPower() const [member function]
    cls.add_method('GetNTxPower', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint32_t ns3::WifiPhy::GetNumberOfReceiveAntennas() const [member function]
    cls.add_method('GetNumberOfReceiveAntennas', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint32_t ns3::WifiPhy::GetNumberOfTransmitAntennas() const [member function]
    cls.add_method('GetNumberOfTransmitAntennas', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate12Mbps() [member function]
    cls.add_method('GetOfdmRate12Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate12MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate12MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate12MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate12MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate13_5MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate13_5MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate18Mbps() [member function]
    cls.add_method('GetOfdmRate18Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate18MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate18MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate1_5MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate1_5MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate24Mbps() [member function]
    cls.add_method('GetOfdmRate24Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate24MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate24MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate27MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate27MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate2_25MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate2_25MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate36Mbps() [member function]
    cls.add_method('GetOfdmRate36Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate3MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate3MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate3MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate3MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate48Mbps() [member function]
    cls.add_method('GetOfdmRate48Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate4_5MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate4_5MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate4_5MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate4_5MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate54Mbps() [member function]
    cls.add_method('GetOfdmRate54Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate6Mbps() [member function]
    cls.add_method('GetOfdmRate6Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate6MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate6MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate6MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate6MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate9Mbps() [member function]
    cls.add_method('GetOfdmRate9Mbps', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate9MbpsBW10MHz() [member function]
    cls.add_method('GetOfdmRate9MbpsBW10MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetOfdmRate9MbpsBW5MHz() [member function]
    cls.add_method('GetOfdmRate9MbpsBW5MHz', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::GetPayloadDuration(uint32_t size, ns3::WifiTxVector txVector, ns3::WifiPreamble preamble, double frequency) [member function]
    cls.add_method('GetPayloadDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble'), param('double', 'frequency')])
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::GetPayloadDuration(uint32_t size, ns3::WifiTxVector txVector, ns3::WifiPreamble preamble, double frequency, ns3::mpduType mpdutype, uint8_t incFlag) [member function]
    cls.add_method('GetPayloadDuration', 
                   'ns3::Time', 
                   [param('uint32_t', 'size'), param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble'), param('double', 'frequency'), param('ns3::mpduType', 'mpdutype'), param('uint8_t', 'incFlag')])
    ## wifi-phy.h (module 'wifi'): static ns3::Time ns3::WifiPhy::GetPlcpHeaderDuration(ns3::WifiTxVector txVector, ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpHeaderDuration', 
                   'ns3::Time', 
                   [param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetPlcpHeaderMode(ns3::WifiMode payloadMode, ns3::WifiPreamble preamble, ns3::WifiTxVector txVector) [member function]
    cls.add_method('GetPlcpHeaderMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiMode', 'payloadMode'), param('ns3::WifiPreamble', 'preamble'), param('ns3::WifiTxVector', 'txVector')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::Time ns3::WifiPhy::GetPlcpHtSigHeaderDuration(ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpHtSigHeaderDuration', 
                   'ns3::Time', 
                   [param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::Time ns3::WifiPhy::GetPlcpHtTrainingSymbolDuration(ns3::WifiPreamble preamble, ns3::WifiTxVector txVector) [member function]
    cls.add_method('GetPlcpHtTrainingSymbolDuration', 
                   'ns3::Time', 
                   [param('ns3::WifiPreamble', 'preamble'), param('ns3::WifiTxVector', 'txVector')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::Time ns3::WifiPhy::GetPlcpPreambleDuration(ns3::WifiTxVector txVector, ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpPreambleDuration', 
                   'ns3::Time', 
                   [param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::Time ns3::WifiPhy::GetPlcpVhtSigA1Duration(ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpVhtSigA1Duration', 
                   'ns3::Time', 
                   [param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::Time ns3::WifiPhy::GetPlcpVhtSigA2Duration(ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpVhtSigA2Duration', 
                   'ns3::Time', 
                   [param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::Time ns3::WifiPhy::GetPlcpVhtSigBDuration(ns3::WifiPreamble preamble) [member function]
    cls.add_method('GetPlcpVhtSigBDuration', 
                   'ns3::Time', 
                   [param('ns3::WifiPreamble', 'preamble')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::GetShortPlcpPreambleSupported() const [member function]
    cls.add_method('GetShortPlcpPreambleSupported', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): ns3::Time ns3::WifiPhy::GetStateDuration() [member function]
    cls.add_method('GetStateDuration', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::GetStbc() const [member function]
    cls.add_method('GetStbc', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): std::vector<unsigned int, std::allocator<unsigned int> > ns3::WifiPhy::GetSupportedChannelWidthSet() const [member function]
    cls.add_method('GetSupportedChannelWidthSet', 
                   'std::vector< unsigned int >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint8_t ns3::WifiPhy::GetSupportedRxSpatialStreams() const [member function]
    cls.add_method('GetSupportedRxSpatialStreams', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): uint8_t ns3::WifiPhy::GetSupportedTxSpatialStreams() const [member function]
    cls.add_method('GetSupportedTxSpatialStreams', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): double ns3::WifiPhy::GetTxPowerEnd() const [member function]
    cls.add_method('GetTxPowerEnd', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): double ns3::WifiPhy::GetTxPowerStart() const [member function]
    cls.add_method('GetTxPowerStart', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): static ns3::TypeId ns3::WifiPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs0() [member function]
    cls.add_method('GetVhtMcs0', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs1() [member function]
    cls.add_method('GetVhtMcs1', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs2() [member function]
    cls.add_method('GetVhtMcs2', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs3() [member function]
    cls.add_method('GetVhtMcs3', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs4() [member function]
    cls.add_method('GetVhtMcs4', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs5() [member function]
    cls.add_method('GetVhtMcs5', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs6() [member function]
    cls.add_method('GetVhtMcs6', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs7() [member function]
    cls.add_method('GetVhtMcs7', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs8() [member function]
    cls.add_method('GetVhtMcs8', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtMcs9() [member function]
    cls.add_method('GetVhtMcs9', 
                   'ns3::WifiMode', 
                   [], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): static ns3::WifiMode ns3::WifiPhy::GetVhtPlcpHeaderMode(ns3::WifiMode payloadMode) [member function]
    cls.add_method('GetVhtPlcpHeaderMode', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiMode', 'payloadMode')], 
                   is_static=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::IsModeSupported(ns3::WifiMode mode) const [member function]
    cls.add_method('IsModeSupported', 
                   'bool', 
                   [param('ns3::WifiMode', 'mode')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::IsStateBusy() [member function]
    cls.add_method('IsStateBusy', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::IsStateCcaBusy() [member function]
    cls.add_method('IsStateCcaBusy', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::IsStateIdle() [member function]
    cls.add_method('IsStateIdle', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::IsStateRx() [member function]
    cls.add_method('IsStateRx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::IsStateSleep() [member function]
    cls.add_method('IsStateSleep', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::IsStateSwitching() [member function]
    cls.add_method('IsStateSwitching', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): bool ns3::WifiPhy::IsStateTx() [member function]
    cls.add_method('IsStateTx', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::NotifyMonitorSniffRx(ns3::Ptr<ns3::Packet const> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, ns3::WifiPreamble preamble, ns3::WifiTxVector txVector, ns3::mpduInfo aMpdu, ns3::signalNoiseDbm signalNoise) [member function]
    cls.add_method('NotifyMonitorSniffRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'channelFreqMhz'), param('uint16_t', 'channelNumber'), param('uint32_t', 'rate'), param('ns3::WifiPreamble', 'preamble'), param('ns3::WifiTxVector', 'txVector'), param('ns3::mpduInfo', 'aMpdu'), param('ns3::signalNoiseDbm', 'signalNoise')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::NotifyMonitorSniffTx(ns3::Ptr<ns3::Packet const> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, ns3::WifiPreamble preamble, ns3::WifiTxVector txVector, ns3::mpduInfo aMpdu) [member function]
    cls.add_method('NotifyMonitorSniffTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'channelFreqMhz'), param('uint16_t', 'channelNumber'), param('uint32_t', 'rate'), param('ns3::WifiPreamble', 'preamble'), param('ns3::WifiTxVector', 'txVector'), param('ns3::mpduInfo', 'aMpdu')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::NotifyRxBegin(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRxBegin', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::NotifyRxDrop(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::NotifyRxEnd(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyRxEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::NotifyTxBegin(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTxBegin', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::NotifyTxDrop(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTxDrop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::NotifyTxEnd(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NotifyTxEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::RegisterListener(ns3::WifiPhyListener * listener) [member function]
    cls.add_method('RegisterListener', 
                   'void', 
                   [param('ns3::WifiPhyListener *', 'listener')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::ResumeFromSleep() [member function]
    cls.add_method('ResumeFromSleep', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SendPacket(ns3::Ptr<ns3::Packet const> packet, ns3::WifiTxVector txVector, ns3::WifiPreamble preamble) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SendPacket(ns3::Ptr<ns3::Packet const> packet, ns3::WifiTxVector txVector, ns3::WifiPreamble preamble, ns3::mpduType mpdutype) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiTxVector', 'txVector'), param('ns3::WifiPreamble', 'preamble'), param('ns3::mpduType', 'mpdutype')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetChannelNumber(uint16_t id) [member function]
    cls.add_method('SetChannelNumber', 
                   'void', 
                   [param('uint16_t', 'id')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetChannelWidth(uint32_t channelwidth) [member function]
    cls.add_method('SetChannelWidth', 
                   'void', 
                   [param('uint32_t', 'channelwidth')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetFrequency(uint32_t freq) [member function]
    cls.add_method('SetFrequency', 
                   'void', 
                   [param('uint32_t', 'freq')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetGreenfield(bool greenfield) [member function]
    cls.add_method('SetGreenfield', 
                   'void', 
                   [param('bool', 'greenfield')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetGuardInterval(bool guardInterval) [member function]
    cls.add_method('SetGuardInterval', 
                   'void', 
                   [param('bool', 'guardInterval')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetLdpc(bool ldpc) [member function]
    cls.add_method('SetLdpc', 
                   'void', 
                   [param('bool', 'ldpc')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetNumberOfReceiveAntennas(uint32_t rx) [member function]
    cls.add_method('SetNumberOfReceiveAntennas', 
                   'void', 
                   [param('uint32_t', 'rx')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetNumberOfTransmitAntennas(uint32_t tx) [member function]
    cls.add_method('SetNumberOfTransmitAntennas', 
                   'void', 
                   [param('uint32_t', 'tx')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetReceiveErrorCallback(ns3::Callback<void,ns3::Ptr<ns3::Packet>,double,bool,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveErrorCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetReceiveOkCallback(ns3::Callback<void,ns3::Ptr<ns3::Packet>,double,ns3::WifiTxVector,ns3::WifiPreamble,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('SetReceiveOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, double, ns3::WifiTxVector, ns3::WifiPreamble, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetShortPlcpPreambleSupported(bool preamble) [member function]
    cls.add_method('SetShortPlcpPreambleSupported', 
                   'void', 
                   [param('bool', 'preamble')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetSleepMode() [member function]
    cls.add_method('SetSleepMode', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::SetStbc(bool stbc) [member function]
    cls.add_method('SetStbc', 
                   'void', 
                   [param('bool', 'stbc')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wifi-phy.h (module 'wifi'): void ns3::WifiPhy::UnregisterListener(ns3::WifiPhyListener * listener) [member function]
    cls.add_method('UnregisterListener', 
                   'void', 
                   [param('ns3::WifiPhyListener *', 'listener')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3WifiRemoteStationManager_methods(root_module, cls):
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationManager::WifiRemoteStationManager(ns3::WifiRemoteStationManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiRemoteStationManager const &', 'arg0')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationManager::WifiRemoteStationManager() [constructor]
    cls.add_constructor([])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddAllSupportedMcs(ns3::Mac48Address address) [member function]
    cls.add_method('AddAllSupportedMcs', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddAllSupportedModes(ns3::Mac48Address address) [member function]
    cls.add_method('AddAllSupportedModes', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddBasicMcs(ns3::WifiMode mcs) [member function]
    cls.add_method('AddBasicMcs', 
                   'void', 
                   [param('ns3::WifiMode', 'mcs')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddBasicMode(ns3::WifiMode mode) [member function]
    cls.add_method('AddBasicMode', 
                   'void', 
                   [param('ns3::WifiMode', 'mode')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddStationHtCapabilities(ns3::Mac48Address from, ns3::HtCapabilities htcapabilities) [member function]
    cls.add_method('AddStationHtCapabilities', 
                   'void', 
                   [param('ns3::Mac48Address', 'from'), param('ns3::HtCapabilities', 'htcapabilities')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddStationVhtCapabilities(ns3::Mac48Address from, ns3::VhtCapabilities vhtcapabilities) [member function]
    cls.add_method('AddStationVhtCapabilities', 
                   'void', 
                   [param('ns3::Mac48Address', 'from'), param('ns3::VhtCapabilities', 'vhtcapabilities')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddSupportedErpSlotTime(ns3::Mac48Address address, bool isShortSlotTimeSupported) [member function]
    cls.add_method('AddSupportedErpSlotTime', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('bool', 'isShortSlotTimeSupported')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddSupportedMcs(ns3::Mac48Address address, ns3::WifiMode mcs) [member function]
    cls.add_method('AddSupportedMcs', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'mcs')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddSupportedMode(ns3::Mac48Address address, ns3::WifiMode mode) [member function]
    cls.add_method('AddSupportedMode', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'mode')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::AddSupportedPlcpPreamble(ns3::Mac48Address address, bool isShortPreambleSupported) [member function]
    cls.add_method('AddSupportedPlcpPreamble', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('bool', 'isShortPreambleSupported')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::DoGetCtsToSelfTxVector() [member function]
    cls.add_method('DoGetCtsToSelfTxVector', 
                   'ns3::WifiTxVector', 
                   [])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::GetAckTxVector(ns3::Mac48Address address, ns3::WifiMode dataMode) [member function]
    cls.add_method('GetAckTxVector', 
                   'ns3::WifiTxVector', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'dataMode')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetBasicMcs(uint32_t i) const [member function]
    cls.add_method('GetBasicMcs', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetBasicMode(uint32_t i) const [member function]
    cls.add_method('GetBasicMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::GetBlockAckTxVector(ns3::Mac48Address address, ns3::WifiMode dataMode) [member function]
    cls.add_method('GetBlockAckTxVector', 
                   'ns3::WifiTxVector', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'dataMode')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::GetCtsToSelfTxVector(ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('GetCtsToSelfTxVector', 
                   'ns3::WifiTxVector', 
                   [param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::GetCtsTxVector(ns3::Mac48Address address, ns3::WifiMode rtsMode) [member function]
    cls.add_method('GetCtsTxVector', 
                   'ns3::WifiTxVector', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'rtsMode')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::GetDataTxVector(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('GetDataTxVector', 
                   'ns3::WifiTxVector', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetDefaultMcs() const [member function]
    cls.add_method('GetDefaultMcs', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetDefaultMode() const [member function]
    cls.add_method('GetDefaultMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::GetDefaultTxPowerLevel() const [member function]
    cls.add_method('GetDefaultTxPowerLevel', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetFragmentOffset(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('GetFragmentOffset', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')])
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetFragmentSize(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('GetFragmentSize', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')])
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetFragmentationThreshold() const [member function]
    cls.add_method('GetFragmentationThreshold', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetGreenfieldSupported(ns3::Mac48Address address) const [member function]
    cls.add_method('GetGreenfieldSupported', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationInfo ns3::WifiRemoteStationManager::GetInfo(ns3::Mac48Address address) [member function]
    cls.add_method('GetInfo', 
                   'ns3::WifiRemoteStationInfo', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetMaxSlrc() const [member function]
    cls.add_method('GetMaxSlrc', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetMaxSsrc() const [member function]
    cls.add_method('GetMaxSsrc', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetNBasicMcs() const [member function]
    cls.add_method('GetNBasicMcs', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetNBasicModes() const [member function]
    cls.add_method('GetNBasicModes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetNNonErpBasicModes() const [member function]
    cls.add_method('GetNNonErpBasicModes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetNonErpBasicMode(uint32_t i) const [member function]
    cls.add_method('GetNonErpBasicMode', 
                   'ns3::WifiMode', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetNonUnicastMode() const [member function]
    cls.add_method('GetNonUnicastMode', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetNumberOfTransmitAntennas() [member function]
    cls.add_method('GetNumberOfTransmitAntennas', 
                   'uint32_t', 
                   [])
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStationManager::ProtectionMode ns3::WifiRemoteStationManager::GetProtectionMode() const [member function]
    cls.add_method('GetProtectionMode', 
                   'ns3::WifiRemoteStationManager::ProtectionMode', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetRtsCtsThreshold() const [member function]
    cls.add_method('GetRtsCtsThreshold', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::GetRtsTxVector(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('GetRtsTxVector', 
                   'ns3::WifiTxVector', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetShortPreambleEnabled() const [member function]
    cls.add_method('GetShortPreambleEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetShortPreambleSupported(ns3::Mac48Address address) const [member function]
    cls.add_method('GetShortPreambleSupported', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetShortSlotTimeEnabled() const [member function]
    cls.add_method('GetShortSlotTimeEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetShortSlotTimeSupported(ns3::Mac48Address address) const [member function]
    cls.add_method('GetShortSlotTimeSupported', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): static ns3::TypeId ns3::WifiRemoteStationManager::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetUseNonErpProtection() const [member function]
    cls.add_method('GetUseNonErpProtection', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::HasHtSupported() const [member function]
    cls.add_method('HasHtSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::HasVhtSupported() const [member function]
    cls.add_method('HasVhtSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::IsAssociated(ns3::Mac48Address address) const [member function]
    cls.add_method('IsAssociated', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::IsBrandNew(ns3::Mac48Address address) const [member function]
    cls.add_method('IsBrandNew', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::IsLastFragment(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, uint32_t fragmentNumber) [member function]
    cls.add_method('IsLastFragment', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint32_t', 'fragmentNumber')])
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::IsWaitAssocTxOk(ns3::Mac48Address address) const [member function]
    cls.add_method('IsWaitAssocTxOk', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_const=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::NeedCtsToSelf(ns3::WifiTxVector txVector) [member function]
    cls.add_method('NeedCtsToSelf', 
                   'bool', 
                   [param('ns3::WifiTxVector', 'txVector')])
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::NeedDataRetransmission(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NeedDataRetransmission', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::NeedFragmentation(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NeedFragmentation', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::NeedRts(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet, ns3::WifiTxVector txVector) [member function]
    cls.add_method('NeedRts', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiTxVector', 'txVector')])
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::NeedRtsRetransmission(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('NeedRtsRetransmission', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::PrepareForQueue(ns3::Mac48Address address, ns3::WifiMacHeader const * header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('PrepareForQueue', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::RecordDisassociated(ns3::Mac48Address address) [member function]
    cls.add_method('RecordDisassociated', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::RecordGotAssocTxFailed(ns3::Mac48Address address) [member function]
    cls.add_method('RecordGotAssocTxFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::RecordGotAssocTxOk(ns3::Mac48Address address) [member function]
    cls.add_method('RecordGotAssocTxOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::RecordWaitAssocTxOk(ns3::Mac48Address address) [member function]
    cls.add_method('RecordWaitAssocTxOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::ReportDataFailed(ns3::Mac48Address address, ns3::WifiMacHeader const * header) [member function]
    cls.add_method('ReportDataFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::ReportDataOk(ns3::Mac48Address address, ns3::WifiMacHeader const * header, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('ReportDataOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::ReportFinalDataFailed(ns3::Mac48Address address, ns3::WifiMacHeader const * header) [member function]
    cls.add_method('ReportFinalDataFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::ReportFinalRtsFailed(ns3::Mac48Address address, ns3::WifiMacHeader const * header) [member function]
    cls.add_method('ReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::ReportRtsFailed(ns3::Mac48Address address, ns3::WifiMacHeader const * header) [member function]
    cls.add_method('ReportRtsFailed', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::ReportRtsOk(ns3::Mac48Address address, ns3::WifiMacHeader const * header, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('ReportRtsOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::ReportRxOk(ns3::Mac48Address address, ns3::WifiMacHeader const * header, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('ReportRxOk', 
                   'void', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMacHeader const *', 'header'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::Reset(ns3::Mac48Address address) [member function]
    cls.add_method('Reset', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetDefaultTxPowerLevel(uint8_t txPower) [member function]
    cls.add_method('SetDefaultTxPowerLevel', 
                   'void', 
                   [param('uint8_t', 'txPower')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetFragmentationThreshold(uint32_t threshold) [member function]
    cls.add_method('SetFragmentationThreshold', 
                   'void', 
                   [param('uint32_t', 'threshold')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetHtSupported(bool enable) [member function]
    cls.add_method('SetHtSupported', 
                   'void', 
                   [param('bool', 'enable')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetMaxSlrc(uint32_t maxSlrc) [member function]
    cls.add_method('SetMaxSlrc', 
                   'void', 
                   [param('uint32_t', 'maxSlrc')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetMaxSsrc(uint32_t maxSsrc) [member function]
    cls.add_method('SetMaxSsrc', 
                   'void', 
                   [param('uint32_t', 'maxSsrc')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetProtectionMode(ns3::WifiRemoteStationManager::ProtectionMode mode) [member function]
    cls.add_method('SetProtectionMode', 
                   'void', 
                   [param('ns3::WifiRemoteStationManager::ProtectionMode', 'mode')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetRtsCtsThreshold(uint32_t threshold) [member function]
    cls.add_method('SetRtsCtsThreshold', 
                   'void', 
                   [param('uint32_t', 'threshold')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetShortPreambleEnabled(bool enable) [member function]
    cls.add_method('SetShortPreambleEnabled', 
                   'void', 
                   [param('bool', 'enable')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetShortSlotTimeEnabled(bool enable) [member function]
    cls.add_method('SetShortSlotTimeEnabled', 
                   'void', 
                   [param('bool', 'enable')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetUseNonErpProtection(bool enable) [member function]
    cls.add_method('SetUseNonErpProtection', 
                   'void', 
                   [param('bool', 'enable')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetVhtSupported(bool enable) [member function]
    cls.add_method('SetVhtSupported', 
                   'void', 
                   [param('bool', 'enable')])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetupMac(ns3::Ptr<ns3::WifiMac> mac) [member function]
    cls.add_method('SetupMac', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiMac >', 'mac')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::SetupPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetupPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::UpdateFragmentationThreshold() [member function]
    cls.add_method('UpdateFragmentationThreshold', 
                   'void', 
                   [])
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetAggregation(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetAggregation', 
                   'bool', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetChannelWidth(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetChannelWidth', 
                   'uint32_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetGreenfield(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetGreenfield', 
                   'bool', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetHtSupported(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetHtSupported', 
                   'bool', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetLongRetryCount(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetLongRetryCount', 
                   'uint32_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::Ptr<ns3::WifiMac> ns3::WifiRemoteStationManager::GetMac() const [member function]
    cls.add_method('GetMac', 
                   'ns3::Ptr< ns3::WifiMac >', 
                   [], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetMcsSupported(ns3::WifiRemoteStation const * station, uint32_t i) const [member function]
    cls.add_method('GetMcsSupported', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation const *', 'station'), param('uint32_t', 'i')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetNMcsSupported(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetNMcsSupported', 
                   'uint32_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetNNonErpSupported(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetNNonErpSupported', 
                   'uint32_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetNSupported(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetNSupported', 
                   'uint32_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetNess(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetNess', 
                   'uint32_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetNonErpSupported(ns3::WifiRemoteStation const * station, uint32_t i) const [member function]
    cls.add_method('GetNonErpSupported', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation const *', 'station'), param('uint32_t', 'i')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::GetNumberOfSupportedRxAntennas(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetNumberOfSupportedRxAntennas', 
                   'uint8_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::Ptr<ns3::WifiPhy> ns3::WifiRemoteStationManager::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetShortGuardInterval(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetShortGuardInterval', 
                   'bool', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::GetShortRetryCount(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetShortRetryCount', 
                   'uint32_t', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetStbc(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetStbc', 
                   'bool', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiMode ns3::WifiRemoteStationManager::GetSupported(ns3::WifiRemoteStation const * station, uint32_t i) const [member function]
    cls.add_method('GetSupported', 
                   'ns3::WifiMode', 
                   [param('ns3::WifiRemoteStation const *', 'station'), param('uint32_t', 'i')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::GetVhtSupported(ns3::WifiRemoteStation const * station) const [member function]
    cls.add_method('GetVhtSupported', 
                   'bool', 
                   [param('ns3::WifiRemoteStation const *', 'station')], 
                   is_const=True, visibility='protected')
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiRemoteStation * ns3::WifiRemoteStationManager::DoCreateStation() const [member function]
    cls.add_method('DoCreateStation', 
                   'ns3::WifiRemoteStation *', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::DoGetAckTxChannelWidth(ns3::Mac48Address address, ns3::WifiMode ctsMode) [member function]
    cls.add_method('DoGetAckTxChannelWidth', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ctsMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoGetAckTxGuardInterval(ns3::Mac48Address address, ns3::WifiMode ackMode) [member function]
    cls.add_method('DoGetAckTxGuardInterval', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ackMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetAckTxNess(ns3::Mac48Address address, ns3::WifiMode ackMode) [member function]
    cls.add_method('DoGetAckTxNess', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ackMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetAckTxNss(ns3::Mac48Address address, ns3::WifiMode ackMode) [member function]
    cls.add_method('DoGetAckTxNss', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ackMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetAckTxPowerLevel(ns3::Mac48Address address, ns3::WifiMode ackMode) [member function]
    cls.add_method('DoGetAckTxPowerLevel', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ackMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoGetAckTxStbc(ns3::Mac48Address address, ns3::WifiMode ackMode) [member function]
    cls.add_method('DoGetAckTxStbc', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ackMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::DoGetBlockAckTxChannelWidth(ns3::Mac48Address address, ns3::WifiMode ctsMode) [member function]
    cls.add_method('DoGetBlockAckTxChannelWidth', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ctsMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoGetBlockAckTxGuardInterval(ns3::Mac48Address address, ns3::WifiMode blockAckMode) [member function]
    cls.add_method('DoGetBlockAckTxGuardInterval', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'blockAckMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetBlockAckTxNess(ns3::Mac48Address address, ns3::WifiMode blockAckMode) [member function]
    cls.add_method('DoGetBlockAckTxNess', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'blockAckMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetBlockAckTxNss(ns3::Mac48Address address, ns3::WifiMode blockAckMode) [member function]
    cls.add_method('DoGetBlockAckTxNss', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'blockAckMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetBlockAckTxPowerLevel(ns3::Mac48Address address, ns3::WifiMode blockAckMode) [member function]
    cls.add_method('DoGetBlockAckTxPowerLevel', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'blockAckMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoGetBlockAckTxStbc(ns3::Mac48Address address, ns3::WifiMode blockAckMode) [member function]
    cls.add_method('DoGetBlockAckTxStbc', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'blockAckMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint32_t ns3::WifiRemoteStationManager::DoGetCtsTxChannelWidth(ns3::Mac48Address address, ns3::WifiMode ctsMode) [member function]
    cls.add_method('DoGetCtsTxChannelWidth', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ctsMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoGetCtsTxGuardInterval(ns3::Mac48Address address, ns3::WifiMode ctsMode) [member function]
    cls.add_method('DoGetCtsTxGuardInterval', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ctsMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetCtsTxNess(ns3::Mac48Address address, ns3::WifiMode ctsMode) [member function]
    cls.add_method('DoGetCtsTxNess', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ctsMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetCtsTxNss(ns3::Mac48Address address, ns3::WifiMode ctsMode) [member function]
    cls.add_method('DoGetCtsTxNss', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ctsMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): uint8_t ns3::WifiRemoteStationManager::DoGetCtsTxPowerLevel(ns3::Mac48Address address, ns3::WifiMode ctsMode) [member function]
    cls.add_method('DoGetCtsTxPowerLevel', 
                   'uint8_t', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ctsMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoGetCtsTxStbc(ns3::Mac48Address address, ns3::WifiMode ctsMode) [member function]
    cls.add_method('DoGetCtsTxStbc', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('ns3::WifiMode', 'ctsMode')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::DoGetDataTxVector(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetDataTxVector', 
                   'ns3::WifiTxVector', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): ns3::WifiTxVector ns3::WifiRemoteStationManager::DoGetRtsTxVector(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoGetRtsTxVector', 
                   'ns3::WifiTxVector', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoNeedDataRetransmission(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedDataRetransmission', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoNeedFragmentation(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedFragmentation', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoNeedRts(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedRts', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::DoNeedRtsRetransmission(ns3::WifiRemoteStation * station, ns3::Ptr<ns3::Packet const> packet, bool normally) [member function]
    cls.add_method('DoNeedRtsRetransmission', 
                   'bool', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('bool', 'normally')], 
                   visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::DoReportDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::DoReportDataOk(ns3::WifiRemoteStation * station, double ackSnr, ns3::WifiMode ackMode, double dataSnr) [member function]
    cls.add_method('DoReportDataOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ackSnr'), param('ns3::WifiMode', 'ackMode'), param('double', 'dataSnr')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::DoReportFinalDataFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalDataFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::DoReportFinalRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportFinalRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::DoReportRtsFailed(ns3::WifiRemoteStation * station) [member function]
    cls.add_method('DoReportRtsFailed', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::DoReportRtsOk(ns3::WifiRemoteStation * station, double ctsSnr, ns3::WifiMode ctsMode, double rtsSnr) [member function]
    cls.add_method('DoReportRtsOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'ctsSnr'), param('ns3::WifiMode', 'ctsMode'), param('double', 'rtsSnr')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): void ns3::WifiRemoteStationManager::DoReportRxOk(ns3::WifiRemoteStation * station, double rxSnr, ns3::WifiMode txMode) [member function]
    cls.add_method('DoReportRxOk', 
                   'void', 
                   [param('ns3::WifiRemoteStation *', 'station'), param('double', 'rxSnr'), param('ns3::WifiMode', 'txMode')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## wifi-remote-station-manager.h (module 'wifi'): bool ns3::WifiRemoteStationManager::IsLowLatency() const [member function]
    cls.add_method('IsLowLatency', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
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
    ## callback.h (module 'core'): std::string ns3::CallbackImplBase::GetTypeid() const [member function]
    cls.add_method('GetTypeid', 
                   'std::string', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## callback.h (module 'core'): bool ns3::CallbackImplBase::IsEqual(ns3::Ptr<ns3::CallbackImplBase const> other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ptr< ns3::CallbackImplBase const >', 'other')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## callback.h (module 'core'): static std::string ns3::CallbackImplBase::Demangle(std::string const & mangled) [member function]
    cls.add_method('Demangle', 
                   'std::string', 
                   [param('std::string const &', 'mangled')], 
                   is_static=True, visibility='protected')
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

def register_Ns3CtrlBAckRequestHeader_methods(root_module, cls):
    ## ctrl-headers.h (module 'wifi'): ns3::CtrlBAckRequestHeader::CtrlBAckRequestHeader(ns3::CtrlBAckRequestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CtrlBAckRequestHeader const &', 'arg0')])
    ## ctrl-headers.h (module 'wifi'): ns3::CtrlBAckRequestHeader::CtrlBAckRequestHeader() [constructor]
    cls.add_constructor([])
    ## ctrl-headers.h (module 'wifi'): uint32_t ns3::CtrlBAckRequestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): ns3::TypeId ns3::CtrlBAckRequestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): uint32_t ns3::CtrlBAckRequestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): uint16_t ns3::CtrlBAckRequestHeader::GetStartingSequence() const [member function]
    cls.add_method('GetStartingSequence', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): uint16_t ns3::CtrlBAckRequestHeader::GetStartingSequenceControl() const [member function]
    cls.add_method('GetStartingSequenceControl', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): uint8_t ns3::CtrlBAckRequestHeader::GetTidInfo() const [member function]
    cls.add_method('GetTidInfo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): static ns3::TypeId ns3::CtrlBAckRequestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckRequestHeader::IsBasic() const [member function]
    cls.add_method('IsBasic', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckRequestHeader::IsCompressed() const [member function]
    cls.add_method('IsCompressed', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckRequestHeader::IsMultiTid() const [member function]
    cls.add_method('IsMultiTid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckRequestHeader::MustSendHtImmediateAck() const [member function]
    cls.add_method('MustSendHtImmediateAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckRequestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckRequestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckRequestHeader::SetHtImmediateAck(bool immediateAck) [member function]
    cls.add_method('SetHtImmediateAck', 
                   'void', 
                   [param('bool', 'immediateAck')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckRequestHeader::SetStartingSequence(uint16_t seq) [member function]
    cls.add_method('SetStartingSequence', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckRequestHeader::SetTidInfo(uint8_t tid) [member function]
    cls.add_method('SetTidInfo', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckRequestHeader::SetType(ns3::BlockAckType type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('ns3::BlockAckType', 'type')])
    return

def register_Ns3CtrlBAckResponseHeader_methods(root_module, cls):
    ## ctrl-headers.h (module 'wifi'): ns3::CtrlBAckResponseHeader::CtrlBAckResponseHeader(ns3::CtrlBAckResponseHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CtrlBAckResponseHeader const &', 'arg0')])
    ## ctrl-headers.h (module 'wifi'): ns3::CtrlBAckResponseHeader::CtrlBAckResponseHeader() [constructor]
    cls.add_constructor([])
    ## ctrl-headers.h (module 'wifi'): uint32_t ns3::CtrlBAckResponseHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): uint16_t const * ns3::CtrlBAckResponseHeader::GetBitmap() const [member function]
    cls.add_method('GetBitmap', 
                   'uint16_t const *', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): uint64_t ns3::CtrlBAckResponseHeader::GetCompressedBitmap() const [member function]
    cls.add_method('GetCompressedBitmap', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): ns3::TypeId ns3::CtrlBAckResponseHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): uint32_t ns3::CtrlBAckResponseHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): uint16_t ns3::CtrlBAckResponseHeader::GetStartingSequence() const [member function]
    cls.add_method('GetStartingSequence', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): uint16_t ns3::CtrlBAckResponseHeader::GetStartingSequenceControl() const [member function]
    cls.add_method('GetStartingSequenceControl', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): uint8_t ns3::CtrlBAckResponseHeader::GetTidInfo() const [member function]
    cls.add_method('GetTidInfo', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): static ns3::TypeId ns3::CtrlBAckResponseHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckResponseHeader::IsBasic() const [member function]
    cls.add_method('IsBasic', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckResponseHeader::IsCompressed() const [member function]
    cls.add_method('IsCompressed', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckResponseHeader::IsFragmentReceived(uint16_t seq, uint8_t frag) const [member function]
    cls.add_method('IsFragmentReceived', 
                   'bool', 
                   [param('uint16_t', 'seq'), param('uint8_t', 'frag')], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckResponseHeader::IsMultiTid() const [member function]
    cls.add_method('IsMultiTid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckResponseHeader::IsPacketReceived(uint16_t seq) const [member function]
    cls.add_method('IsPacketReceived', 
                   'bool', 
                   [param('uint16_t', 'seq')], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): bool ns3::CtrlBAckResponseHeader::MustSendHtImmediateAck() const [member function]
    cls.add_method('MustSendHtImmediateAck', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::ResetBitmap() [member function]
    cls.add_method('ResetBitmap', 
                   'void', 
                   [])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::SetHtImmediateAck(bool immediateAck) [member function]
    cls.add_method('SetHtImmediateAck', 
                   'void', 
                   [param('bool', 'immediateAck')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::SetReceivedFragment(uint16_t seq, uint8_t frag) [member function]
    cls.add_method('SetReceivedFragment', 
                   'void', 
                   [param('uint16_t', 'seq'), param('uint8_t', 'frag')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::SetReceivedPacket(uint16_t seq) [member function]
    cls.add_method('SetReceivedPacket', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::SetStartingSequence(uint16_t seq) [member function]
    cls.add_method('SetStartingSequence', 
                   'void', 
                   [param('uint16_t', 'seq')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::SetStartingSequenceControl(uint16_t seqControl) [member function]
    cls.add_method('SetStartingSequenceControl', 
                   'void', 
                   [param('uint16_t', 'seqControl')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::SetTidInfo(uint8_t tid) [member function]
    cls.add_method('SetTidInfo', 
                   'void', 
                   [param('uint8_t', 'tid')])
    ## ctrl-headers.h (module 'wifi'): void ns3::CtrlBAckResponseHeader::SetType(ns3::BlockAckType type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('ns3::BlockAckType', 'type')])
    return

def register_Ns3Dcf_methods(root_module, cls):
    ## dcf.h (module 'wifi'): ns3::Dcf::Dcf() [constructor]
    cls.add_constructor([])
    ## dcf.h (module 'wifi'): ns3::Dcf::Dcf(ns3::Dcf const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Dcf const &', 'arg0')])
    ## dcf.h (module 'wifi'): uint32_t ns3::Dcf::GetAifsn() const [member function]
    cls.add_method('GetAifsn', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## dcf.h (module 'wifi'): uint32_t ns3::Dcf::GetMaxCw() const [member function]
    cls.add_method('GetMaxCw', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## dcf.h (module 'wifi'): uint32_t ns3::Dcf::GetMinCw() const [member function]
    cls.add_method('GetMinCw', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## dcf.h (module 'wifi'): static ns3::TypeId ns3::Dcf::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dcf.h (module 'wifi'): void ns3::Dcf::SetAifsn(uint32_t aifsn) [member function]
    cls.add_method('SetAifsn', 
                   'void', 
                   [param('uint32_t', 'aifsn')], 
                   is_pure_virtual=True, is_virtual=True)
    ## dcf.h (module 'wifi'): void ns3::Dcf::SetMaxCw(uint32_t maxCw) [member function]
    cls.add_method('SetMaxCw', 
                   'void', 
                   [param('uint32_t', 'maxCw')], 
                   is_pure_virtual=True, is_virtual=True)
    ## dcf.h (module 'wifi'): void ns3::Dcf::SetMinCw(uint32_t minCw) [member function]
    cls.add_method('SetMinCw', 
                   'void', 
                   [param('uint32_t', 'minCw')], 
                   is_pure_virtual=True, is_virtual=True)
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

def register_Ns3EdcaTxopN_methods(root_module, cls):
    ## edca-txop-n.h (module 'wifi'): ns3::EdcaTxopN::m_aMpduEnabled [variable]
    cls.add_instance_attribute('m_aMpduEnabled', 'std::map< ns3::Mac48Address, bool >', is_const=False)
    ## edca-txop-n.h (module 'wifi'): static ns3::TypeId ns3::EdcaTxopN::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## edca-txop-n.h (module 'wifi'): ns3::EdcaTxopN::EdcaTxopN() [constructor]
    cls.add_constructor([])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetLow(ns3::Ptr<ns3::MacLow> low) [member function]
    cls.add_method('SetLow', 
                   'void', 
                   [param('ns3::Ptr< ns3::MacLow >', 'low')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetTxMiddle(ns3::MacTxMiddle * txMiddle) [member function]
    cls.add_method('SetTxMiddle', 
                   'void', 
                   [param('ns3::MacTxMiddle *', 'txMiddle')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetManager(ns3::DcfManager * manager) [member function]
    cls.add_method('SetManager', 
                   'void', 
                   [param('ns3::DcfManager *', 'manager')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetTxOkCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetTxFailedCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxFailedCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> remoteManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'remoteManager')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetTypeOfStation(ns3::TypeOfStation type) [member function]
    cls.add_method('SetTypeOfStation', 
                   'void', 
                   [param('ns3::TypeOfStation', 'type')])
    ## edca-txop-n.h (module 'wifi'): ns3::TypeOfStation ns3::EdcaTxopN::GetTypeOfStation() const [member function]
    cls.add_method('GetTypeOfStation', 
                   'ns3::TypeOfStation', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): ns3::Ptr<ns3::WifiMacQueue> ns3::EdcaTxopN::GetEdcaQueue() const [member function]
    cls.add_method('GetEdcaQueue', 
                   'ns3::Ptr< ns3::WifiMacQueue >', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetMinCw(uint32_t minCw) [member function]
    cls.add_method('SetMinCw', 
                   'void', 
                   [param('uint32_t', 'minCw')], 
                   is_virtual=True)
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetMaxCw(uint32_t maxCw) [member function]
    cls.add_method('SetMaxCw', 
                   'void', 
                   [param('uint32_t', 'maxCw')], 
                   is_virtual=True)
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetAifsn(uint32_t aifsn) [member function]
    cls.add_method('SetAifsn', 
                   'void', 
                   [param('uint32_t', 'aifsn')], 
                   is_virtual=True)
    ## edca-txop-n.h (module 'wifi'): uint32_t ns3::EdcaTxopN::GetMinCw() const [member function]
    cls.add_method('GetMinCw', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## edca-txop-n.h (module 'wifi'): uint32_t ns3::EdcaTxopN::GetMaxCw() const [member function]
    cls.add_method('GetMaxCw', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## edca-txop-n.h (module 'wifi'): uint32_t ns3::EdcaTxopN::GetAifsn() const [member function]
    cls.add_method('GetAifsn', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## edca-txop-n.h (module 'wifi'): ns3::Ptr<ns3::MacLow> ns3::EdcaTxopN::Low() [member function]
    cls.add_method('Low', 
                   'ns3::Ptr< ns3::MacLow >', 
                   [])
    ## edca-txop-n.h (module 'wifi'): ns3::Ptr<ns3::MsduAggregator> ns3::EdcaTxopN::GetMsduAggregator() const [member function]
    cls.add_method('GetMsduAggregator', 
                   'ns3::Ptr< ns3::MsduAggregator >', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): ns3::Ptr<ns3::MpduAggregator> ns3::EdcaTxopN::GetMpduAggregator() const [member function]
    cls.add_method('GetMpduAggregator', 
                   'ns3::Ptr< ns3::MpduAggregator >', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): bool ns3::EdcaTxopN::GetBaAgreementExists(ns3::Mac48Address address, uint8_t tid) [member function]
    cls.add_method('GetBaAgreementExists', 
                   'bool', 
                   [param('ns3::Mac48Address', 'address'), param('uint8_t', 'tid')])
    ## edca-txop-n.h (module 'wifi'): uint32_t ns3::EdcaTxopN::GetNOutstandingPacketsInBa(ns3::Mac48Address address, uint8_t tid) [member function]
    cls.add_method('GetNOutstandingPacketsInBa', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'address'), param('uint8_t', 'tid')])
    ## edca-txop-n.h (module 'wifi'): uint32_t ns3::EdcaTxopN::GetNRetryNeededPackets(ns3::Mac48Address recipient, uint8_t tid) const [member function]
    cls.add_method('GetNRetryNeededPackets', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::CompleteAmpduTransfer(ns3::Mac48Address recipient, uint8_t tid) [member function]
    cls.add_method('CompleteAmpduTransfer', 
                   'void', 
                   [param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid')])
    ## edca-txop-n.h (module 'wifi'): bool ns3::EdcaTxopN::NeedsAccess() const [member function]
    cls.add_method('NeedsAccess', 
                   'bool', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::NotifyAccessGranted() [member function]
    cls.add_method('NotifyAccessGranted', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::NotifyInternalCollision() [member function]
    cls.add_method('NotifyInternalCollision', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::NotifyCollision() [member function]
    cls.add_method('NotifyCollision', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::NotifyChannelSwitching() [member function]
    cls.add_method('NotifyChannelSwitching', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::NotifySleep() [member function]
    cls.add_method('NotifySleep', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::NotifyWakeUp() [member function]
    cls.add_method('NotifyWakeUp', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::GotCts(double snr, ns3::WifiMode txMode) [member function]
    cls.add_method('GotCts', 
                   'void', 
                   [param('double', 'snr'), param('ns3::WifiMode', 'txMode')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::MissedCts() [member function]
    cls.add_method('MissedCts', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::GotAck(double snr, ns3::WifiMode txMode) [member function]
    cls.add_method('GotAck', 
                   'void', 
                   [param('double', 'snr'), param('ns3::WifiMode', 'txMode')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::GotBlockAck(ns3::CtrlBAckResponseHeader const * blockAck, ns3::Mac48Address recipient, ns3::WifiMode txMode) [member function]
    cls.add_method('GotBlockAck', 
                   'void', 
                   [param('ns3::CtrlBAckResponseHeader const *', 'blockAck'), param('ns3::Mac48Address', 'recipient'), param('ns3::WifiMode', 'txMode')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::MissedBlockAck() [member function]
    cls.add_method('MissedBlockAck', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::GotAddBaResponse(ns3::MgtAddBaResponseHeader const * respHdr, ns3::Mac48Address recipient) [member function]
    cls.add_method('GotAddBaResponse', 
                   'void', 
                   [param('ns3::MgtAddBaResponseHeader const *', 'respHdr'), param('ns3::Mac48Address', 'recipient')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::GotDelBaFrame(ns3::MgtDelBaHeader const * delBaHdr, ns3::Mac48Address recipient) [member function]
    cls.add_method('GotDelBaFrame', 
                   'void', 
                   [param('ns3::MgtDelBaHeader const *', 'delBaHdr'), param('ns3::Mac48Address', 'recipient')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::MissedAck() [member function]
    cls.add_method('MissedAck', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::StartNext() [member function]
    cls.add_method('StartNext', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::Cancel() [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::EndTxNoAck() [member function]
    cls.add_method('EndTxNoAck', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::RestartAccessIfNeeded() [member function]
    cls.add_method('RestartAccessIfNeeded', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::StartAccessIfNeeded() [member function]
    cls.add_method('StartAccessIfNeeded', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): bool ns3::EdcaTxopN::NeedRtsRetransmission() [member function]
    cls.add_method('NeedRtsRetransmission', 
                   'bool', 
                   [])
    ## edca-txop-n.h (module 'wifi'): bool ns3::EdcaTxopN::NeedDataRetransmission() [member function]
    cls.add_method('NeedDataRetransmission', 
                   'bool', 
                   [])
    ## edca-txop-n.h (module 'wifi'): bool ns3::EdcaTxopN::NeedBarRetransmission() [member function]
    cls.add_method('NeedBarRetransmission', 
                   'bool', 
                   [])
    ## edca-txop-n.h (module 'wifi'): bool ns3::EdcaTxopN::NeedFragmentation() const [member function]
    cls.add_method('NeedFragmentation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): uint32_t ns3::EdcaTxopN::GetNextFragmentSize() [member function]
    cls.add_method('GetNextFragmentSize', 
                   'uint32_t', 
                   [])
    ## edca-txop-n.h (module 'wifi'): uint32_t ns3::EdcaTxopN::GetFragmentSize() [member function]
    cls.add_method('GetFragmentSize', 
                   'uint32_t', 
                   [])
    ## edca-txop-n.h (module 'wifi'): uint32_t ns3::EdcaTxopN::GetFragmentOffset() [member function]
    cls.add_method('GetFragmentOffset', 
                   'uint32_t', 
                   [])
    ## edca-txop-n.h (module 'wifi'): bool ns3::EdcaTxopN::IsLastFragment() const [member function]
    cls.add_method('IsLastFragment', 
                   'bool', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::NextFragment() [member function]
    cls.add_method('NextFragment', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): ns3::Ptr<ns3::Packet> ns3::EdcaTxopN::GetFragmentPacket(ns3::WifiMacHeader * hdr) [member function]
    cls.add_method('GetFragmentPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::WifiMacHeader *', 'hdr')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetAccessCategory(ns3::AcIndex ac) [member function]
    cls.add_method('SetAccessCategory', 
                   'void', 
                   [param('ns3::AcIndex', 'ac')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::Queue(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('Queue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetMsduAggregator(ns3::Ptr<ns3::MsduAggregator> aggr) [member function]
    cls.add_method('SetMsduAggregator', 
                   'void', 
                   [param('ns3::Ptr< ns3::MsduAggregator >', 'aggr')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetMpduAggregator(ns3::Ptr<ns3::MpduAggregator> aggr) [member function]
    cls.add_method('SetMpduAggregator', 
                   'void', 
                   [param('ns3::Ptr< ns3::MpduAggregator >', 'aggr')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::PushFront(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('PushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::CompleteConfig() [member function]
    cls.add_method('CompleteConfig', 
                   'void', 
                   [])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetBlockAckThreshold(uint8_t threshold) [member function]
    cls.add_method('SetBlockAckThreshold', 
                   'void', 
                   [param('uint8_t', 'threshold')])
    ## edca-txop-n.h (module 'wifi'): uint8_t ns3::EdcaTxopN::GetBlockAckThreshold() const [member function]
    cls.add_method('GetBlockAckThreshold', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetBlockAckInactivityTimeout(uint16_t timeout) [member function]
    cls.add_method('SetBlockAckInactivityTimeout', 
                   'void', 
                   [param('uint16_t', 'timeout')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SendDelbaFrame(ns3::Mac48Address addr, uint8_t tid, bool byOriginator) [member function]
    cls.add_method('SendDelbaFrame', 
                   'void', 
                   [param('ns3::Mac48Address', 'addr'), param('uint8_t', 'tid'), param('bool', 'byOriginator')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::CompleteMpduTx(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader hdr, ns3::Time tstamp) [member function]
    cls.add_method('CompleteMpduTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader', 'hdr'), param('ns3::Time', 'tstamp')])
    ## edca-txop-n.h (module 'wifi'): bool ns3::EdcaTxopN::GetAmpduExist(ns3::Mac48Address dest) [member function]
    cls.add_method('GetAmpduExist', 
                   'bool', 
                   [param('ns3::Mac48Address', 'dest')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::SetAmpduExist(ns3::Mac48Address dest, bool enableAmpdu) [member function]
    cls.add_method('SetAmpduExist', 
                   'void', 
                   [param('ns3::Mac48Address', 'dest'), param('bool', 'enableAmpdu')])
    ## edca-txop-n.h (module 'wifi'): uint16_t ns3::EdcaTxopN::GetNextSequenceNumberfor(ns3::WifiMacHeader * hdr) [member function]
    cls.add_method('GetNextSequenceNumberfor', 
                   'uint16_t', 
                   [param('ns3::WifiMacHeader *', 'hdr')])
    ## edca-txop-n.h (module 'wifi'): uint16_t ns3::EdcaTxopN::PeekNextSequenceNumberfor(ns3::WifiMacHeader * hdr) [member function]
    cls.add_method('PeekNextSequenceNumberfor', 
                   'uint16_t', 
                   [param('ns3::WifiMacHeader *', 'hdr')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::RemoveRetransmitPacket(uint8_t tid, ns3::Mac48Address recipient, uint16_t seqnumber) [member function]
    cls.add_method('RemoveRetransmitPacket', 
                   'void', 
                   [param('uint8_t', 'tid'), param('ns3::Mac48Address', 'recipient'), param('uint16_t', 'seqnumber')])
    ## edca-txop-n.h (module 'wifi'): ns3::Ptr<ns3::Packet const> ns3::EdcaTxopN::PeekNextRetransmitPacket(ns3::WifiMacHeader & header, ns3::Mac48Address recipient, uint8_t tid, ns3::Time * timestamp) [member function]
    cls.add_method('PeekNextRetransmitPacket', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [param('ns3::WifiMacHeader &', 'header'), param('ns3::Mac48Address', 'recipient'), param('uint8_t', 'tid'), param('ns3::Time *', 'timestamp')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::BaTxOk(ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('BaTxOk', 
                   'void', 
                   [param('ns3::WifiMacHeader const &', 'hdr')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::BaTxFailed(ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('BaTxFailed', 
                   'void', 
                   [param('ns3::WifiMacHeader const &', 'hdr')])
    ## edca-txop-n.h (module 'wifi'): int64_t ns3::EdcaTxopN::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')])
    ## edca-txop-n.h (module 'wifi'): void ns3::EdcaTxopN::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
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
    ## enum.h (module 'core'): void ns3::EnumChecker::Add(int value, std::string name) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('int', 'value'), param('std::string', 'name')])
    ## enum.h (module 'core'): void ns3::EnumChecker::AddDefault(int value, std::string name) [member function]
    cls.add_method('AddDefault', 
                   'void', 
                   [param('int', 'value'), param('std::string', 'name')])
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
    ## enum.h (module 'core'): ns3::EnumValue::EnumValue(int value) [constructor]
    cls.add_constructor([param('int', 'value')])
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
    ## enum.h (module 'core'): void ns3::EnumValue::Set(int value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('int', 'value')])
    return

def register_Ns3ErpInformation_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## erp-information.h (module 'wifi'): ns3::ErpInformation::ErpInformation(ns3::ErpInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErpInformation const &', 'arg0')])
    ## erp-information.h (module 'wifi'): ns3::ErpInformation::ErpInformation() [constructor]
    cls.add_constructor([])
    ## erp-information.h (module 'wifi'): uint8_t ns3::ErpInformation::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## erp-information.h (module 'wifi'): ns3::WifiInformationElementId ns3::ErpInformation::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## erp-information.h (module 'wifi'): uint8_t ns3::ErpInformation::GetBarkerPreambleMode() const [member function]
    cls.add_method('GetBarkerPreambleMode', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## erp-information.h (module 'wifi'): uint8_t ns3::ErpInformation::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## erp-information.h (module 'wifi'): uint8_t ns3::ErpInformation::GetNonErpPresent() const [member function]
    cls.add_method('GetNonErpPresent', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## erp-information.h (module 'wifi'): uint16_t ns3::ErpInformation::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## erp-information.h (module 'wifi'): uint8_t ns3::ErpInformation::GetUseProtection() const [member function]
    cls.add_method('GetUseProtection', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## erp-information.h (module 'wifi'): ns3::Buffer::Iterator ns3::ErpInformation::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## erp-information.h (module 'wifi'): void ns3::ErpInformation::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## erp-information.h (module 'wifi'): void ns3::ErpInformation::SetBarkerPreambleMode(uint8_t barkerPreambleMode) [member function]
    cls.add_method('SetBarkerPreambleMode', 
                   'void', 
                   [param('uint8_t', 'barkerPreambleMode')])
    ## erp-information.h (module 'wifi'): void ns3::ErpInformation::SetErpSupported(uint8_t erpSupported) [member function]
    cls.add_method('SetErpSupported', 
                   'void', 
                   [param('uint8_t', 'erpSupported')])
    ## erp-information.h (module 'wifi'): void ns3::ErpInformation::SetNonErpPresent(uint8_t nonErpPresent) [member function]
    cls.add_method('SetNonErpPresent', 
                   'void', 
                   [param('uint8_t', 'nonErpPresent')])
    ## erp-information.h (module 'wifi'): void ns3::ErpInformation::SetUseProtection(uint8_t useProtection) [member function]
    cls.add_method('SetUseProtection', 
                   'void', 
                   [param('uint8_t', 'useProtection')])
    return

def register_Ns3ErpInformationChecker_methods(root_module, cls):
    ## erp-information.h (module 'wifi'): ns3::ErpInformationChecker::ErpInformationChecker() [constructor]
    cls.add_constructor([])
    ## erp-information.h (module 'wifi'): ns3::ErpInformationChecker::ErpInformationChecker(ns3::ErpInformationChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErpInformationChecker const &', 'arg0')])
    return

def register_Ns3ErpInformationValue_methods(root_module, cls):
    ## erp-information.h (module 'wifi'): ns3::ErpInformationValue::ErpInformationValue() [constructor]
    cls.add_constructor([])
    ## erp-information.h (module 'wifi'): ns3::ErpInformationValue::ErpInformationValue(ns3::ErpInformationValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErpInformationValue const &', 'arg0')])
    ## erp-information.h (module 'wifi'): ns3::ErpInformationValue::ErpInformationValue(ns3::ErpInformation const & value) [constructor]
    cls.add_constructor([param('ns3::ErpInformation const &', 'value')])
    ## erp-information.h (module 'wifi'): ns3::Ptr<ns3::AttributeValue> ns3::ErpInformationValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## erp-information.h (module 'wifi'): bool ns3::ErpInformationValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## erp-information.h (module 'wifi'): ns3::ErpInformation ns3::ErpInformationValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::ErpInformation', 
                   [], 
                   is_const=True)
    ## erp-information.h (module 'wifi'): std::string ns3::ErpInformationValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## erp-information.h (module 'wifi'): void ns3::ErpInformationValue::Set(ns3::ErpInformation const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::ErpInformation const &', 'value')])
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

def register_Ns3ExtendedSupportedRatesIE_methods(root_module, cls):
    ## supported-rates.h (module 'wifi'): ns3::ExtendedSupportedRatesIE::ExtendedSupportedRatesIE(ns3::ExtendedSupportedRatesIE const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ExtendedSupportedRatesIE const &', 'arg0')])
    ## supported-rates.h (module 'wifi'): ns3::ExtendedSupportedRatesIE::ExtendedSupportedRatesIE() [constructor]
    cls.add_constructor([])
    ## supported-rates.h (module 'wifi'): ns3::ExtendedSupportedRatesIE::ExtendedSupportedRatesIE(ns3::SupportedRates * rates) [constructor]
    cls.add_constructor([param('ns3::SupportedRates *', 'rates')])
    ## supported-rates.h (module 'wifi'): uint8_t ns3::ExtendedSupportedRatesIE::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## supported-rates.h (module 'wifi'): ns3::WifiInformationElementId ns3::ExtendedSupportedRatesIE::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h (module 'wifi'): uint8_t ns3::ExtendedSupportedRatesIE::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h (module 'wifi'): uint16_t ns3::ExtendedSupportedRatesIE::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## supported-rates.h (module 'wifi'): ns3::Buffer::Iterator ns3::ExtendedSupportedRatesIE::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## supported-rates.h (module 'wifi'): void ns3::ExtendedSupportedRatesIE::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h (module 'wifi'): void ns3::ExtendedSupportedRatesIE::SetSupportedRates(ns3::SupportedRates * rates) [member function]
    cls.add_method('SetSupportedRates', 
                   'void', 
                   [param('ns3::SupportedRates *', 'rates')])
    return

def register_Ns3HtCapabilities_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilities::HtCapabilities(ns3::HtCapabilities const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::HtCapabilities const &', 'arg0')])
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilities::HtCapabilities() [constructor]
    cls.add_constructor([])
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ht-capabilities.h (module 'wifi'): ns3::WifiInformationElementId ns3::HtCapabilities::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetAmpduParameters() const [member function]
    cls.add_method('GetAmpduParameters', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetAntennaSelectionCapabilities() const [member function]
    cls.add_method('GetAntennaSelectionCapabilities', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint16_t ns3::HtCapabilities::GetExtendedHtCapabilities() const [member function]
    cls.add_method('GetExtendedHtCapabilities', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetGreenfield() const [member function]
    cls.add_method('GetGreenfield', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint16_t ns3::HtCapabilities::GetHtCapabilitiesInfo() const [member function]
    cls.add_method('GetHtCapabilitiesInfo', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetLSigProtectionSupport() const [member function]
    cls.add_method('GetLSigProtectionSupport', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetLdpc() const [member function]
    cls.add_method('GetLdpc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetMaxAmpduLength() const [member function]
    cls.add_method('GetMaxAmpduLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetMaxAmsduLength() const [member function]
    cls.add_method('GetMaxAmsduLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetRxHighestSupportedAntennas() const [member function]
    cls.add_method('GetRxHighestSupportedAntennas', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint16_t ns3::HtCapabilities::GetRxHighestSupportedDataRate() const [member function]
    cls.add_method('GetRxHighestSupportedDataRate', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t * ns3::HtCapabilities::GetRxMcsBitmask() [member function]
    cls.add_method('GetRxMcsBitmask', 
                   'uint8_t *', 
                   [])
    ## ht-capabilities.h (module 'wifi'): uint16_t ns3::HtCapabilities::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetShortGuardInterval20() const [member function]
    cls.add_method('GetShortGuardInterval20', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetShortGuardInterval40() const [member function]
    cls.add_method('GetShortGuardInterval40', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetSupportedChannelWidth() const [member function]
    cls.add_method('GetSupportedChannelWidth', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint64_t ns3::HtCapabilities::GetSupportedMcsSet1() const [member function]
    cls.add_method('GetSupportedMcsSet1', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint64_t ns3::HtCapabilities::GetSupportedMcsSet2() const [member function]
    cls.add_method('GetSupportedMcsSet2', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint32_t ns3::HtCapabilities::GetTxBfCapabilities() const [member function]
    cls.add_method('GetTxBfCapabilities', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetTxMaxNSpatialStreams() const [member function]
    cls.add_method('GetTxMaxNSpatialStreams', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetTxMcsSetDefined() const [member function]
    cls.add_method('GetTxMcsSetDefined', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetTxRxMcsSetUnequal() const [member function]
    cls.add_method('GetTxRxMcsSetUnequal', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): uint8_t ns3::HtCapabilities::GetTxUnequalModulation() const [member function]
    cls.add_method('GetTxUnequalModulation', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): bool ns3::HtCapabilities::IsSupportedMcs(uint8_t mcs) const [member function]
    cls.add_method('IsSupportedMcs', 
                   'bool', 
                   [param('uint8_t', 'mcs')], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): ns3::Buffer::Iterator ns3::HtCapabilities::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetAmpduParameters(uint8_t ctrl) [member function]
    cls.add_method('SetAmpduParameters', 
                   'void', 
                   [param('uint8_t', 'ctrl')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetAntennaSelectionCapabilities(uint8_t ctrl) [member function]
    cls.add_method('SetAntennaSelectionCapabilities', 
                   'void', 
                   [param('uint8_t', 'ctrl')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetExtendedHtCapabilities(uint16_t ctrl) [member function]
    cls.add_method('SetExtendedHtCapabilities', 
                   'void', 
                   [param('uint16_t', 'ctrl')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetGreenfield(uint8_t greenfield) [member function]
    cls.add_method('SetGreenfield', 
                   'void', 
                   [param('uint8_t', 'greenfield')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetHtCapabilitiesInfo(uint16_t ctrl) [member function]
    cls.add_method('SetHtCapabilitiesInfo', 
                   'void', 
                   [param('uint16_t', 'ctrl')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetHtSupported(uint8_t htsupported) [member function]
    cls.add_method('SetHtSupported', 
                   'void', 
                   [param('uint8_t', 'htsupported')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetLSigProtectionSupport(uint8_t lsigprotection) [member function]
    cls.add_method('SetLSigProtectionSupport', 
                   'void', 
                   [param('uint8_t', 'lsigprotection')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetLdpc(uint8_t ldpc) [member function]
    cls.add_method('SetLdpc', 
                   'void', 
                   [param('uint8_t', 'ldpc')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetMaxAmpduLength(uint8_t maxampdulength) [member function]
    cls.add_method('SetMaxAmpduLength', 
                   'void', 
                   [param('uint8_t', 'maxampdulength')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetMaxAmsduLength(uint8_t maxamsdulength) [member function]
    cls.add_method('SetMaxAmsduLength', 
                   'void', 
                   [param('uint8_t', 'maxamsdulength')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetRxHighestSupportedDataRate(uint16_t maxsupportedrate) [member function]
    cls.add_method('SetRxHighestSupportedDataRate', 
                   'void', 
                   [param('uint16_t', 'maxsupportedrate')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetRxMcsBitmask(uint8_t index) [member function]
    cls.add_method('SetRxMcsBitmask', 
                   'void', 
                   [param('uint8_t', 'index')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetShortGuardInterval20(uint8_t shortguardinterval) [member function]
    cls.add_method('SetShortGuardInterval20', 
                   'void', 
                   [param('uint8_t', 'shortguardinterval')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetShortGuardInterval40(uint8_t shortguardinterval) [member function]
    cls.add_method('SetShortGuardInterval40', 
                   'void', 
                   [param('uint8_t', 'shortguardinterval')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetSupportedChannelWidth(uint8_t supportedchannelwidth) [member function]
    cls.add_method('SetSupportedChannelWidth', 
                   'void', 
                   [param('uint8_t', 'supportedchannelwidth')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetSupportedMcsSet(uint64_t ctrl1, uint64_t ctrl2) [member function]
    cls.add_method('SetSupportedMcsSet', 
                   'void', 
                   [param('uint64_t', 'ctrl1'), param('uint64_t', 'ctrl2')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetTxBfCapabilities(uint32_t ctrl) [member function]
    cls.add_method('SetTxBfCapabilities', 
                   'void', 
                   [param('uint32_t', 'ctrl')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetTxMaxNSpatialStreams(uint8_t maxtxspatialstreams) [member function]
    cls.add_method('SetTxMaxNSpatialStreams', 
                   'void', 
                   [param('uint8_t', 'maxtxspatialstreams')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetTxMcsSetDefined(uint8_t txmcssetdefined) [member function]
    cls.add_method('SetTxMcsSetDefined', 
                   'void', 
                   [param('uint8_t', 'txmcssetdefined')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetTxRxMcsSetUnequal(uint8_t txrxmcssetunequal) [member function]
    cls.add_method('SetTxRxMcsSetUnequal', 
                   'void', 
                   [param('uint8_t', 'txrxmcssetunequal')])
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilities::SetTxUnequalModulation(uint8_t txunequalmodulation) [member function]
    cls.add_method('SetTxUnequalModulation', 
                   'void', 
                   [param('uint8_t', 'txunequalmodulation')])
    return

def register_Ns3HtCapabilitiesChecker_methods(root_module, cls):
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilitiesChecker::HtCapabilitiesChecker() [constructor]
    cls.add_constructor([])
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilitiesChecker::HtCapabilitiesChecker(ns3::HtCapabilitiesChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::HtCapabilitiesChecker const &', 'arg0')])
    return

def register_Ns3HtCapabilitiesValue_methods(root_module, cls):
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilitiesValue::HtCapabilitiesValue() [constructor]
    cls.add_constructor([])
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilitiesValue::HtCapabilitiesValue(ns3::HtCapabilitiesValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::HtCapabilitiesValue const &', 'arg0')])
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilitiesValue::HtCapabilitiesValue(ns3::HtCapabilities const & value) [constructor]
    cls.add_constructor([param('ns3::HtCapabilities const &', 'value')])
    ## ht-capabilities.h (module 'wifi'): ns3::Ptr<ns3::AttributeValue> ns3::HtCapabilitiesValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ht-capabilities.h (module 'wifi'): bool ns3::HtCapabilitiesValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ht-capabilities.h (module 'wifi'): ns3::HtCapabilities ns3::HtCapabilitiesValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::HtCapabilities', 
                   [], 
                   is_const=True)
    ## ht-capabilities.h (module 'wifi'): std::string ns3::HtCapabilitiesValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ht-capabilities.h (module 'wifi'): void ns3::HtCapabilitiesValue::Set(ns3::HtCapabilities const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::HtCapabilities const &', 'value')])
    return

def register_Ns3HtOperations_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ht-operations.h (module 'wifi'): ns3::HtOperations::HtOperations(ns3::HtOperations const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::HtOperations const &', 'arg0')])
    ## ht-operations.h (module 'wifi'): ns3::HtOperations::HtOperations() [constructor]
    cls.add_constructor([])
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ht-operations.h (module 'wifi'): ns3::WifiInformationElementId ns3::HtOperations::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ht-operations.h (module 'wifi'): uint64_t ns3::HtOperations::GetBasicMcsSet1() const [member function]
    cls.add_method('GetBasicMcsSet1', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint64_t ns3::HtOperations::GetBasicMcsSet2() const [member function]
    cls.add_method('GetBasicMcsSet2', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetDualBeacon() const [member function]
    cls.add_method('GetDualBeacon', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetDualCtsProtection() const [member function]
    cls.add_method('GetDualCtsProtection', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetHtProtection() const [member function]
    cls.add_method('GetHtProtection', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetInformationSubset1() const [member function]
    cls.add_method('GetInformationSubset1', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint16_t ns3::HtOperations::GetInformationSubset2() const [member function]
    cls.add_method('GetInformationSubset2', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint16_t ns3::HtOperations::GetInformationSubset3() const [member function]
    cls.add_method('GetInformationSubset3', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetLSigTxopProtectionFullSupport() const [member function]
    cls.add_method('GetLSigTxopProtectionFullSupport', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetNonGfHtStasPresent() const [member function]
    cls.add_method('GetNonGfHtStasPresent', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetObssNonHtStasPresent() const [member function]
    cls.add_method('GetObssNonHtStasPresent', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetPcoActive() const [member function]
    cls.add_method('GetPcoActive', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetPhase() const [member function]
    cls.add_method('GetPhase', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetPrimaryChannel() const [member function]
    cls.add_method('GetPrimaryChannel', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetRifsMode() const [member function]
    cls.add_method('GetRifsMode', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint16_t ns3::HtOperations::GetRxHighestSupportedDataRate() const [member function]
    cls.add_method('GetRxHighestSupportedDataRate', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetSecondaryChannelOffset() const [member function]
    cls.add_method('GetSecondaryChannelOffset', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint16_t ns3::HtOperations::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetStaChannelWidth() const [member function]
    cls.add_method('GetStaChannelWidth', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetStbcBeacon() const [member function]
    cls.add_method('GetStbcBeacon', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetTxMaxNSpatialStreams() const [member function]
    cls.add_method('GetTxMaxNSpatialStreams', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetTxMcsSetDefined() const [member function]
    cls.add_method('GetTxMcsSetDefined', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetTxRxMcsSetUnequal() const [member function]
    cls.add_method('GetTxRxMcsSetUnequal', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): uint8_t ns3::HtOperations::GetTxUnequalModulation() const [member function]
    cls.add_method('GetTxUnequalModulation', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): bool ns3::HtOperations::IsSupportedMcs(uint8_t mcs) [member function]
    cls.add_method('IsSupportedMcs', 
                   'bool', 
                   [param('uint8_t', 'mcs')])
    ## ht-operations.h (module 'wifi'): ns3::Buffer::Iterator ns3::HtOperations::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetBasicMcsSet(uint64_t ctrl1, uint64_t ctrl2) [member function]
    cls.add_method('SetBasicMcsSet', 
                   'void', 
                   [param('uint64_t', 'ctrl1'), param('uint64_t', 'ctrl2')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetDualBeacon(uint8_t dualbeacon) [member function]
    cls.add_method('SetDualBeacon', 
                   'void', 
                   [param('uint8_t', 'dualbeacon')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetDualCtsProtection(uint8_t dualctsprotection) [member function]
    cls.add_method('SetDualCtsProtection', 
                   'void', 
                   [param('uint8_t', 'dualctsprotection')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetHtProtection(uint8_t htprotection) [member function]
    cls.add_method('SetHtProtection', 
                   'void', 
                   [param('uint8_t', 'htprotection')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetHtSupported(uint8_t htsupported) [member function]
    cls.add_method('SetHtSupported', 
                   'void', 
                   [param('uint8_t', 'htsupported')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetInformationSubset1(uint8_t ctrl) [member function]
    cls.add_method('SetInformationSubset1', 
                   'void', 
                   [param('uint8_t', 'ctrl')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetInformationSubset2(uint16_t ctrl) [member function]
    cls.add_method('SetInformationSubset2', 
                   'void', 
                   [param('uint16_t', 'ctrl')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetInformationSubset3(uint16_t ctrl) [member function]
    cls.add_method('SetInformationSubset3', 
                   'void', 
                   [param('uint16_t', 'ctrl')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetLSigTxopProtectionFullSupport(uint8_t lsigtxopprotectionfullsupport) [member function]
    cls.add_method('SetLSigTxopProtectionFullSupport', 
                   'void', 
                   [param('uint8_t', 'lsigtxopprotectionfullsupport')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetNonGfHtStasPresent(uint8_t nongfhtstaspresent) [member function]
    cls.add_method('SetNonGfHtStasPresent', 
                   'void', 
                   [param('uint8_t', 'nongfhtstaspresent')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetObssNonHtStasPresent(uint8_t obssnonhtstaspresent) [member function]
    cls.add_method('SetObssNonHtStasPresent', 
                   'void', 
                   [param('uint8_t', 'obssnonhtstaspresent')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetPcoActive(uint8_t pcoactive) [member function]
    cls.add_method('SetPcoActive', 
                   'void', 
                   [param('uint8_t', 'pcoactive')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetPhase(uint8_t pcophase) [member function]
    cls.add_method('SetPhase', 
                   'void', 
                   [param('uint8_t', 'pcophase')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetPrimaryChannel(uint8_t ctrl) [member function]
    cls.add_method('SetPrimaryChannel', 
                   'void', 
                   [param('uint8_t', 'ctrl')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetRifsMode(uint8_t rifsmode) [member function]
    cls.add_method('SetRifsMode', 
                   'void', 
                   [param('uint8_t', 'rifsmode')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetRxHighestSupportedDataRate(uint16_t maxsupportedrate) [member function]
    cls.add_method('SetRxHighestSupportedDataRate', 
                   'void', 
                   [param('uint16_t', 'maxsupportedrate')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetRxMcsBitmask(uint8_t index) [member function]
    cls.add_method('SetRxMcsBitmask', 
                   'void', 
                   [param('uint8_t', 'index')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetSecondaryChannelOffset(uint8_t secondarychanneloffset) [member function]
    cls.add_method('SetSecondaryChannelOffset', 
                   'void', 
                   [param('uint8_t', 'secondarychanneloffset')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetStaChannelWidth(uint8_t stachannelwidth) [member function]
    cls.add_method('SetStaChannelWidth', 
                   'void', 
                   [param('uint8_t', 'stachannelwidth')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetStbcBeacon(uint8_t stbcbeacon) [member function]
    cls.add_method('SetStbcBeacon', 
                   'void', 
                   [param('uint8_t', 'stbcbeacon')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetTxMaxNSpatialStreams(uint8_t maxtxspatialstreams) [member function]
    cls.add_method('SetTxMaxNSpatialStreams', 
                   'void', 
                   [param('uint8_t', 'maxtxspatialstreams')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetTxMcsSetDefined(uint8_t txmcssetdefined) [member function]
    cls.add_method('SetTxMcsSetDefined', 
                   'void', 
                   [param('uint8_t', 'txmcssetdefined')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetTxRxMcsSetUnequal(uint8_t txrxmcssetunequal) [member function]
    cls.add_method('SetTxRxMcsSetUnequal', 
                   'void', 
                   [param('uint8_t', 'txrxmcssetunequal')])
    ## ht-operations.h (module 'wifi'): void ns3::HtOperations::SetTxUnequalModulation(uint8_t txunequalmodulation) [member function]
    cls.add_method('SetTxUnequalModulation', 
                   'void', 
                   [param('uint8_t', 'txunequalmodulation')])
    return

def register_Ns3HtOperationsChecker_methods(root_module, cls):
    ## ht-operations.h (module 'wifi'): ns3::HtOperationsChecker::HtOperationsChecker() [constructor]
    cls.add_constructor([])
    ## ht-operations.h (module 'wifi'): ns3::HtOperationsChecker::HtOperationsChecker(ns3::HtOperationsChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::HtOperationsChecker const &', 'arg0')])
    return

def register_Ns3HtOperationsValue_methods(root_module, cls):
    ## ht-operations.h (module 'wifi'): ns3::HtOperationsValue::HtOperationsValue() [constructor]
    cls.add_constructor([])
    ## ht-operations.h (module 'wifi'): ns3::HtOperationsValue::HtOperationsValue(ns3::HtOperationsValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::HtOperationsValue const &', 'arg0')])
    ## ht-operations.h (module 'wifi'): ns3::HtOperationsValue::HtOperationsValue(ns3::HtOperations const & value) [constructor]
    cls.add_constructor([param('ns3::HtOperations const &', 'value')])
    ## ht-operations.h (module 'wifi'): ns3::Ptr<ns3::AttributeValue> ns3::HtOperationsValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ht-operations.h (module 'wifi'): bool ns3::HtOperationsValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ht-operations.h (module 'wifi'): ns3::HtOperations ns3::HtOperationsValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::HtOperations', 
                   [], 
                   is_const=True)
    ## ht-operations.h (module 'wifi'): std::string ns3::HtOperationsValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ht-operations.h (module 'wifi'): void ns3::HtOperationsValue::Set(ns3::HtOperations const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::HtOperations const &', 'value')])
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

def register_Ns3MeshInformationElementVector_methods(root_module, cls):
    ## mesh-information-element-vector.h (module 'mesh'): ns3::MeshInformationElementVector::MeshInformationElementVector(ns3::MeshInformationElementVector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshInformationElementVector const &', 'arg0')])
    ## mesh-information-element-vector.h (module 'mesh'): ns3::MeshInformationElementVector::MeshInformationElementVector() [constructor]
    cls.add_constructor([])
    ## mesh-information-element-vector.h (module 'mesh'): uint32_t ns3::MeshInformationElementVector::DeserializeSingleIe(ns3::Buffer::Iterator start) [member function]
    cls.add_method('DeserializeSingleIe', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## mesh-information-element-vector.h (module 'mesh'): ns3::TypeId ns3::MeshInformationElementVector::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-information-element-vector.h (module 'mesh'): static ns3::TypeId ns3::MeshInformationElementVector::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3MeshL2RoutingProtocol_methods(root_module, cls):
    ## mesh-l2-routing-protocol.h (module 'mesh'): ns3::MeshL2RoutingProtocol::MeshL2RoutingProtocol() [constructor]
    cls.add_constructor([])
    ## mesh-l2-routing-protocol.h (module 'mesh'): ns3::MeshL2RoutingProtocol::MeshL2RoutingProtocol(ns3::MeshL2RoutingProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshL2RoutingProtocol const &', 'arg0')])
    ## mesh-l2-routing-protocol.h (module 'mesh'): ns3::Ptr<ns3::MeshPointDevice> ns3::MeshL2RoutingProtocol::GetMeshPoint() const [member function]
    cls.add_method('GetMeshPoint', 
                   'ns3::Ptr< ns3::MeshPointDevice >', 
                   [], 
                   is_const=True)
    ## mesh-l2-routing-protocol.h (module 'mesh'): static ns3::TypeId ns3::MeshL2RoutingProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-l2-routing-protocol.h (module 'mesh'): bool ns3::MeshL2RoutingProtocol::RemoveRoutingStuff(uint32_t fromIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet> packet, uint16_t & protocolType) [member function]
    cls.add_method('RemoveRoutingStuff', 
                   'bool', 
                   [param('uint32_t', 'fromIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t &', 'protocolType')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-l2-routing-protocol.h (module 'mesh'): bool ns3::MeshL2RoutingProtocol::RequestRoute(uint32_t sourceIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet const> packet, uint16_t protocolType, ns3::Callback<void, bool, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'sourceIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocolType'), param('ns3::Callback< void, bool, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-l2-routing-protocol.h (module 'mesh'): void ns3::MeshL2RoutingProtocol::SetMeshPoint(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('SetMeshPoint', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')])
    return

def register_Ns3MeshStack_methods(root_module, cls):
    ## mesh-stack-installer.h (module 'mesh'): ns3::MeshStack::MeshStack() [constructor]
    cls.add_constructor([])
    ## mesh-stack-installer.h (module 'mesh'): ns3::MeshStack::MeshStack(ns3::MeshStack const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshStack const &', 'arg0')])
    ## mesh-stack-installer.h (module 'mesh'): static ns3::TypeId ns3::MeshStack::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-stack-installer.h (module 'mesh'): bool ns3::MeshStack::InstallStack(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('InstallStack', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-stack-installer.h (module 'mesh'): void ns3::MeshStack::Report(ns3::Ptr<ns3::MeshPointDevice> const mp, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp'), param('std::ostream &', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-stack-installer.h (module 'mesh'): void ns3::MeshStack::ResetStats(ns3::Ptr<ns3::MeshPointDevice> const mp) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3MeshWifiInterfaceMacPlugin_methods(root_module, cls):
    ## mesh-wifi-interface-mac-plugin.h (module 'mesh'): ns3::MeshWifiInterfaceMacPlugin::MeshWifiInterfaceMacPlugin() [constructor]
    cls.add_constructor([])
    ## mesh-wifi-interface-mac-plugin.h (module 'mesh'): ns3::MeshWifiInterfaceMacPlugin::MeshWifiInterfaceMacPlugin(ns3::MeshWifiInterfaceMacPlugin const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshWifiInterfaceMacPlugin const &', 'arg0')])
    ## mesh-wifi-interface-mac-plugin.h (module 'mesh'): int64_t ns3::MeshWifiInterfaceMacPlugin::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h (module 'mesh'): bool ns3::MeshWifiInterfaceMacPlugin::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const & header) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const &', 'header')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h (module 'mesh'): void ns3::MeshWifiInterfaceMacPlugin::SetParent(ns3::Ptr<ns3::MeshWifiInterfaceMac> parent) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMac >', 'parent')], 
                   is_pure_virtual=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h (module 'mesh'): void ns3::MeshWifiInterfaceMacPlugin::UpdateBeacon(ns3::MeshWifiBeacon & beacon) const [member function]
    cls.add_method('UpdateBeacon', 
                   'void', 
                   [param('ns3::MeshWifiBeacon &', 'beacon')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac-plugin.h (module 'mesh'): bool ns3::MeshWifiInterfaceMacPlugin::UpdateOutcomingFrame(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader & header, ns3::Mac48Address from, ns3::Mac48Address to) [member function]
    cls.add_method('UpdateOutcomingFrame', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader &', 'header'), param('ns3::Mac48Address', 'from'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3MgtBeaconHeader_methods(root_module, cls):
    ## mgt-headers.h (module 'wifi'): ns3::MgtBeaconHeader::MgtBeaconHeader() [constructor]
    cls.add_constructor([])
    ## mgt-headers.h (module 'wifi'): ns3::MgtBeaconHeader::MgtBeaconHeader(ns3::MgtBeaconHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MgtBeaconHeader const &', 'arg0')])
    ## mgt-headers.h (module 'wifi'): static ns3::TypeId ns3::MgtBeaconHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
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

def register_Ns3NetDeviceQueue_methods(root_module, cls):
    ## net-device.h (module 'network'): ns3::NetDeviceQueue::NetDeviceQueue(ns3::NetDeviceQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDeviceQueue const &', 'arg0')])
    ## net-device.h (module 'network'): ns3::NetDeviceQueue::NetDeviceQueue() [constructor]
    cls.add_constructor([])
    ## net-device.h (module 'network'): bool ns3::NetDeviceQueue::HasWakeCallbackSet() const [member function]
    cls.add_method('HasWakeCallbackSet', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDeviceQueue::IsStopped() const [member function]
    cls.add_method('IsStopped', 
                   'bool', 
                   [], 
                   is_const=True)
    ## net-device.h (module 'network'): void ns3::NetDeviceQueue::SetWakeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetWakeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDeviceQueue::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDeviceQueue::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDeviceQueue::Wake() [member function]
    cls.add_method('Wake', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3NetDeviceQueueInterface_methods(root_module, cls):
    ## net-device.h (module 'network'): ns3::NetDeviceQueueInterface::NetDeviceQueueInterface(ns3::NetDeviceQueueInterface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDeviceQueueInterface const &', 'arg0')])
    ## net-device.h (module 'network'): ns3::NetDeviceQueueInterface::NetDeviceQueueInterface() [constructor]
    cls.add_constructor([])
    ## net-device.h (module 'network'): uint8_t ns3::NetDeviceQueueInterface::GetSelectedQueue(ns3::Ptr<ns3::QueueItem> item) const [member function]
    cls.add_method('GetSelectedQueue', 
                   'uint8_t', 
                   [param('ns3::Ptr< ns3::QueueItem >', 'item')], 
                   is_const=True)
    ## net-device.h (module 'network'): ns3::Ptr<ns3::NetDeviceQueue> ns3::NetDeviceQueueInterface::GetTxQueue(uint8_t i) const [member function]
    cls.add_method('GetTxQueue', 
                   'ns3::Ptr< ns3::NetDeviceQueue >', 
                   [param('uint8_t', 'i')], 
                   is_const=True)
    ## net-device.h (module 'network'): uint8_t ns3::NetDeviceQueueInterface::GetTxQueuesN() const [member function]
    cls.add_method('GetTxQueuesN', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## net-device.h (module 'network'): static ns3::TypeId ns3::NetDeviceQueueInterface::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## net-device.h (module 'network'): bool ns3::NetDeviceQueueInterface::IsQueueDiscInstalled() const [member function]
    cls.add_method('IsQueueDiscInstalled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## net-device.h (module 'network'): void ns3::NetDeviceQueueInterface::SetQueueDiscInstalled(bool installed) [member function]
    cls.add_method('SetQueueDiscInstalled', 
                   'void', 
                   [param('bool', 'installed')])
    ## net-device.h (module 'network'): void ns3::NetDeviceQueueInterface::SetSelectQueueCallback(ns3::Callback<unsigned char, ns3::Ptr<ns3::QueueItem>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetSelectQueueCallback', 
                   'void', 
                   [param('ns3::Callback< unsigned char, ns3::Ptr< ns3::QueueItem >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## net-device.h (module 'network'): void ns3::NetDeviceQueueInterface::SetTxQueuesN(uint8_t numTxQueues) [member function]
    cls.add_method('SetTxQueuesN', 
                   'void', 
                   [param('uint8_t', 'numTxQueues')])
    ## net-device.h (module 'network'): void ns3::NetDeviceQueueInterface::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
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
    ## node.h (module 'network'): ns3::Time ns3::Node::GetLocalTime() const [member function]
    cls.add_method('GetLocalTime', 
                   'ns3::Time', 
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
    ## node.h (module 'network'): void ns3::Node::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
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

def register_Ns3OutputStreamWrapper_methods(root_module, cls):
    ## output-stream-wrapper.h (module 'network'): ns3::OutputStreamWrapper::OutputStreamWrapper(ns3::OutputStreamWrapper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OutputStreamWrapper const &', 'arg0')])
    ## output-stream-wrapper.h (module 'network'): ns3::OutputStreamWrapper::OutputStreamWrapper(std::string filename, std::_Ios_Openmode filemode) [constructor]
    cls.add_constructor([param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode')])
    ## output-stream-wrapper.h (module 'network'): ns3::OutputStreamWrapper::OutputStreamWrapper(std::ostream * os) [constructor]
    cls.add_constructor([param('std::ostream *', 'os')])
    ## output-stream-wrapper.h (module 'network'): std::ostream * ns3::OutputStreamWrapper::GetStream() [member function]
    cls.add_method('GetStream', 
                   'std::ostream *', 
                   [])
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
    ## packet.h (module 'network'): void ns3::Packet::AddAtEnd(ns3::Ptr<ns3::Packet const> packet) [member function]
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
    ## packet.h (module 'network'): bool ns3::Packet::ReplacePacketTag(ns3::Tag & tag) [member function]
    cls.add_method('ReplacePacketTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    ## packet.h (module 'network'): uint32_t ns3::Packet::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::SetNixVector(ns3::Ptr<ns3::NixVector> nixVector) [member function]
    cls.add_method('SetNixVector', 
                   'void', 
                   [param('ns3::Ptr< ns3::NixVector >', 'nixVector')])
    ## packet.h (module 'network'): std::string ns3::Packet::ToString() const [member function]
    cls.add_method('ToString', 
                   'std::string', 
                   [], 
                   is_const=True)
    return

def register_Ns3QueueItem_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## net-device.h (module 'network'): ns3::QueueItem::QueueItem(ns3::Ptr<ns3::Packet> p) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet >', 'p')])
    ## net-device.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::QueueItem::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## net-device.h (module 'network'): uint32_t ns3::QueueItem::GetPacketSize() const [member function]
    cls.add_method('GetPacketSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::QueueItem::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3RegularWifiMac_methods(root_module, cls):
    ## regular-wifi-mac.h (module 'wifi'): static ns3::TypeId ns3::RegularWifiMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::RegularWifiMac::RegularWifiMac() [constructor]
    cls.add_constructor([])
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetSlot(ns3::Time slotTime) [member function]
    cls.add_method('SetSlot', 
                   'void', 
                   [param('ns3::Time', 'slotTime')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetSifs(ns3::Time sifs) [member function]
    cls.add_method('SetSifs', 
                   'void', 
                   [param('ns3::Time', 'sifs')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetEifsNoDifs(ns3::Time eifsNoDifs) [member function]
    cls.add_method('SetEifsNoDifs', 
                   'void', 
                   [param('ns3::Time', 'eifsNoDifs')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetPifs(ns3::Time pifs) [member function]
    cls.add_method('SetPifs', 
                   'void', 
                   [param('ns3::Time', 'pifs')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetRifs(ns3::Time rifs) [member function]
    cls.add_method('SetRifs', 
                   'void', 
                   [param('ns3::Time', 'rifs')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetCtsTimeout(ns3::Time ctsTimeout) [member function]
    cls.add_method('SetCtsTimeout', 
                   'void', 
                   [param('ns3::Time', 'ctsTimeout')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetAckTimeout(ns3::Time ackTimeout) [member function]
    cls.add_method('SetAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'ackTimeout')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetRifs() const [member function]
    cls.add_method('GetRifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetPifs() const [member function]
    cls.add_method('GetPifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetSifs() const [member function]
    cls.add_method('GetSifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetSlot() const [member function]
    cls.add_method('GetSlot', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetEifsNoDifs() const [member function]
    cls.add_method('GetEifsNoDifs', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetCtsTimeout() const [member function]
    cls.add_method('GetCtsTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetAckTimeout() const [member function]
    cls.add_method('GetAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetCtsToSelfSupported(bool enable) [member function]
    cls.add_method('SetCtsToSelfSupported', 
                   'void', 
                   [param('bool', 'enable')])
    ## regular-wifi-mac.h (module 'wifi'): bool ns3::RegularWifiMac::GetCtsToSelfSupported() const [member function]
    cls.add_method('GetCtsToSelfSupported', 
                   'bool', 
                   [], 
                   is_const=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetShortSlotTimeSupported(bool enable) [member function]
    cls.add_method('SetShortSlotTimeSupported', 
                   'void', 
                   [param('bool', 'enable')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): bool ns3::RegularWifiMac::GetShortSlotTimeSupported() const [member function]
    cls.add_method('GetShortSlotTimeSupported', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Mac48Address ns3::RegularWifiMac::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Ssid ns3::RegularWifiMac::GetSsid() const [member function]
    cls.add_method('GetSsid', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetAddress(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetSsid(ns3::Ssid ssid) [member function]
    cls.add_method('SetSsid', 
                   'void', 
                   [param('ns3::Ssid', 'ssid')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetBssid(ns3::Mac48Address bssid) [member function]
    cls.add_method('SetBssid', 
                   'void', 
                   [param('ns3::Mac48Address', 'bssid')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Mac48Address ns3::RegularWifiMac::GetBssid() const [member function]
    cls.add_method('GetBssid', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetPromisc() [member function]
    cls.add_method('SetPromisc', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): bool ns3::RegularWifiMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_pure_virtual=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetWifiPhy(ns3::Ptr<ns3::WifiPhy> phy) [member function]
    cls.add_method('SetWifiPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiPhy >', 'phy')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::WifiPhy> ns3::RegularWifiMac::GetWifiPhy() const [member function]
    cls.add_method('GetWifiPhy', 
                   'ns3::Ptr< ns3::WifiPhy >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::ResetWifiPhy() [member function]
    cls.add_method('ResetWifiPhy', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> stationManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'stationManager')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::WifiRemoteStationManager> ns3::RegularWifiMac::GetWifiRemoteStationManager() const [member function]
    cls.add_method('GetWifiRemoteStationManager', 
                   'ns3::Ptr< ns3::WifiRemoteStationManager >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::HtCapabilities ns3::RegularWifiMac::GetHtCapabilities() const [member function]
    cls.add_method('GetHtCapabilities', 
                   'ns3::HtCapabilities', 
                   [], 
                   is_const=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::VhtCapabilities ns3::RegularWifiMac::GetVhtCapabilities() const [member function]
    cls.add_method('GetVhtCapabilities', 
                   'ns3::VhtCapabilities', 
                   [], 
                   is_const=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetForwardUpCallback(ns3::Callback<void, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> upCallback) [member function]
    cls.add_method('SetForwardUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'upCallback')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetLinkDownCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkDown) [member function]
    cls.add_method('SetLinkDownCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkDown')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetBasicBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetBasicBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetBasicBlockAckTimeout() const [member function]
    cls.add_method('GetBasicBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetCompressedBlockAckTimeout(ns3::Time blockAckTimeout) [member function]
    cls.add_method('SetCompressedBlockAckTimeout', 
                   'void', 
                   [param('ns3::Time', 'blockAckTimeout')], 
                   is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Time ns3::RegularWifiMac::GetCompressedBlockAckTimeout() const [member function]
    cls.add_method('GetCompressedBlockAckTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::DcaTxop> ns3::RegularWifiMac::GetDcaTxop() const [member function]
    cls.add_method('GetDcaTxop', 
                   'ns3::Ptr< ns3::DcaTxop >', 
                   [], 
                   is_const=True, visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::EdcaTxopN> ns3::RegularWifiMac::GetVOQueue() const [member function]
    cls.add_method('GetVOQueue', 
                   'ns3::Ptr< ns3::EdcaTxopN >', 
                   [], 
                   is_const=True, visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::EdcaTxopN> ns3::RegularWifiMac::GetVIQueue() const [member function]
    cls.add_method('GetVIQueue', 
                   'ns3::Ptr< ns3::EdcaTxopN >', 
                   [], 
                   is_const=True, visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::EdcaTxopN> ns3::RegularWifiMac::GetBEQueue() const [member function]
    cls.add_method('GetBEQueue', 
                   'ns3::Ptr< ns3::EdcaTxopN >', 
                   [], 
                   is_const=True, visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): ns3::Ptr<ns3::EdcaTxopN> ns3::RegularWifiMac::GetBKQueue() const [member function]
    cls.add_method('GetBKQueue', 
                   'ns3::Ptr< ns3::EdcaTxopN >', 
                   [], 
                   is_const=True, visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::FinishConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('FinishConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::ConfigureContentionWindow(uint32_t cwMin, uint32_t cwMax) [member function]
    cls.add_method('ConfigureContentionWindow', 
                   'void', 
                   [param('uint32_t', 'cwMin'), param('uint32_t', 'cwMax')], 
                   visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetTypeOfStation(ns3::TypeOfStation type) [member function]
    cls.add_method('SetTypeOfStation', 
                   'void', 
                   [param('ns3::TypeOfStation', 'type')], 
                   visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::TxOk(ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('TxOk', 
                   'void', 
                   [param('ns3::WifiMacHeader const &', 'hdr')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::TxFailed(ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('TxFailed', 
                   'void', 
                   [param('ns3::WifiMacHeader const &', 'hdr')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::ForwardUp(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address from, ns3::Mac48Address to) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address', 'from'), param('ns3::Mac48Address', 'to')], 
                   visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::DeaggregateAmsduAndForward(ns3::Ptr<ns3::Packet> aggregatedPacket, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('DeaggregateAmsduAndForward', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'aggregatedPacket'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SendAddBaResponse(ns3::MgtAddBaRequestHeader const * reqHdr, ns3::Mac48Address originator) [member function]
    cls.add_method('SendAddBaResponse', 
                   'void', 
                   [param('ns3::MgtAddBaRequestHeader const *', 'reqHdr'), param('ns3::Mac48Address', 'originator')], 
                   visibility='protected', is_virtual=True)
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetQosSupported(bool enable) [member function]
    cls.add_method('SetQosSupported', 
                   'void', 
                   [param('bool', 'enable')], 
                   visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): bool ns3::RegularWifiMac::GetQosSupported() const [member function]
    cls.add_method('GetQosSupported', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetHtSupported(bool enable) [member function]
    cls.add_method('SetHtSupported', 
                   'void', 
                   [param('bool', 'enable')], 
                   visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): bool ns3::RegularWifiMac::GetHtSupported() const [member function]
    cls.add_method('GetHtSupported', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetVhtSupported(bool enable) [member function]
    cls.add_method('SetVhtSupported', 
                   'void', 
                   [param('bool', 'enable')], 
                   visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): bool ns3::RegularWifiMac::GetVhtSupported() const [member function]
    cls.add_method('GetVhtSupported', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): void ns3::RegularWifiMac::SetErpSupported(bool enable) [member function]
    cls.add_method('SetErpSupported', 
                   'void', 
                   [param('bool', 'enable')], 
                   visibility='protected')
    ## regular-wifi-mac.h (module 'wifi'): bool ns3::RegularWifiMac::GetErpSupported() const [member function]
    cls.add_method('GetErpSupported', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    return

def register_Ns3Ssid_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ssid.h (module 'wifi'): ns3::Ssid::Ssid(ns3::Ssid const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ssid const &', 'arg0')])
    ## ssid.h (module 'wifi'): ns3::Ssid::Ssid() [constructor]
    cls.add_constructor([])
    ## ssid.h (module 'wifi'): ns3::Ssid::Ssid(std::string s) [constructor]
    cls.add_constructor([param('std::string', 's')])
    ## ssid.h (module 'wifi'): ns3::Ssid::Ssid(char const * ssid, uint8_t length) [constructor]
    cls.add_constructor([param('char const *', 'ssid'), param('uint8_t', 'length')])
    ## ssid.h (module 'wifi'): uint8_t ns3::Ssid::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ssid.h (module 'wifi'): ns3::WifiInformationElementId ns3::Ssid::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ssid.h (module 'wifi'): uint8_t ns3::Ssid::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ssid.h (module 'wifi'): bool ns3::Ssid::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ssid.h (module 'wifi'): bool ns3::Ssid::IsEqual(ns3::Ssid const & o) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ssid const &', 'o')], 
                   is_const=True)
    ## ssid.h (module 'wifi'): char * ns3::Ssid::PeekString() const [member function]
    cls.add_method('PeekString', 
                   'char *', 
                   [], 
                   is_const=True)
    ## ssid.h (module 'wifi'): void ns3::Ssid::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3SsidChecker_methods(root_module, cls):
    ## ssid.h (module 'wifi'): ns3::SsidChecker::SsidChecker() [constructor]
    cls.add_constructor([])
    ## ssid.h (module 'wifi'): ns3::SsidChecker::SsidChecker(ns3::SsidChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SsidChecker const &', 'arg0')])
    return

def register_Ns3SsidValue_methods(root_module, cls):
    ## ssid.h (module 'wifi'): ns3::SsidValue::SsidValue() [constructor]
    cls.add_constructor([])
    ## ssid.h (module 'wifi'): ns3::SsidValue::SsidValue(ns3::SsidValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SsidValue const &', 'arg0')])
    ## ssid.h (module 'wifi'): ns3::SsidValue::SsidValue(ns3::Ssid const & value) [constructor]
    cls.add_constructor([param('ns3::Ssid const &', 'value')])
    ## ssid.h (module 'wifi'): ns3::Ptr<ns3::AttributeValue> ns3::SsidValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ssid.h (module 'wifi'): bool ns3::SsidValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ssid.h (module 'wifi'): ns3::Ssid ns3::SsidValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ssid', 
                   [], 
                   is_const=True)
    ## ssid.h (module 'wifi'): std::string ns3::SsidValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ssid.h (module 'wifi'): void ns3::SsidValue::Set(ns3::Ssid const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ssid const &', 'value')])
    return

def register_Ns3SupportedRates_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## supported-rates.h (module 'wifi'): ns3::SupportedRates::SupportedRates() [constructor]
    cls.add_constructor([])
    ## supported-rates.h (module 'wifi'): ns3::SupportedRates::SupportedRates(ns3::SupportedRates const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SupportedRates const &', 'arg0')])
    ## supported-rates.h (module 'wifi'): void ns3::SupportedRates::AddSupportedRate(uint32_t bs) [member function]
    cls.add_method('AddSupportedRate', 
                   'void', 
                   [param('uint32_t', 'bs')])
    ## supported-rates.h (module 'wifi'): uint8_t ns3::SupportedRates::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## supported-rates.h (module 'wifi'): ns3::WifiInformationElementId ns3::SupportedRates::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h (module 'wifi'): uint8_t ns3::SupportedRates::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h (module 'wifi'): uint8_t ns3::SupportedRates::GetNRates() const [member function]
    cls.add_method('GetNRates', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## supported-rates.h (module 'wifi'): uint32_t ns3::SupportedRates::GetRate(uint8_t i) const [member function]
    cls.add_method('GetRate', 
                   'uint32_t', 
                   [param('uint8_t', 'i')], 
                   is_const=True)
    ## supported-rates.h (module 'wifi'): bool ns3::SupportedRates::IsBasicRate(uint32_t bs) const [member function]
    cls.add_method('IsBasicRate', 
                   'bool', 
                   [param('uint32_t', 'bs')], 
                   is_const=True)
    ## supported-rates.h (module 'wifi'): bool ns3::SupportedRates::IsSupportedRate(uint32_t bs) const [member function]
    cls.add_method('IsSupportedRate', 
                   'bool', 
                   [param('uint32_t', 'bs')], 
                   is_const=True)
    ## supported-rates.h (module 'wifi'): void ns3::SupportedRates::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## supported-rates.h (module 'wifi'): void ns3::SupportedRates::SetBasicRate(uint32_t bs) [member function]
    cls.add_method('SetBasicRate', 
                   'void', 
                   [param('uint32_t', 'bs')])
    ## supported-rates.h (module 'wifi'): ns3::SupportedRates::MAX_SUPPORTED_RATES [variable]
    cls.add_static_attribute('MAX_SUPPORTED_RATES', 'uint8_t const', is_const=True)
    ## supported-rates.h (module 'wifi'): ns3::SupportedRates::extended [variable]
    cls.add_instance_attribute('extended', 'ns3::ExtendedSupportedRatesIE', is_const=False)
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

def register_Ns3VhtCapabilities_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilities::VhtCapabilities(ns3::VhtCapabilities const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VhtCapabilities const &', 'arg0')])
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilities::VhtCapabilities() [constructor]
    cls.add_constructor([])
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## vht-capabilities.h (module 'wifi'): ns3::WifiInformationElementId ns3::VhtCapabilities::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetMaxAmpduLengthExponent() const [member function]
    cls.add_method('GetMaxAmpduLengthExponent', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetMaxMpduLength() const [member function]
    cls.add_method('GetMaxMpduLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint16_t ns3::VhtCapabilities::GetRxHighestSupportedLgiDataRate() const [member function]
    cls.add_method('GetRxHighestSupportedLgiDataRate', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetRxLdpc() const [member function]
    cls.add_method('GetRxLdpc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint16_t ns3::VhtCapabilities::GetRxMcsMap() const [member function]
    cls.add_method('GetRxMcsMap', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetRxStbc() const [member function]
    cls.add_method('GetRxStbc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint16_t ns3::VhtCapabilities::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetShortGuardIntervalFor160Mhz() const [member function]
    cls.add_method('GetShortGuardIntervalFor160Mhz', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetShortGuardIntervalFor80Mhz() const [member function]
    cls.add_method('GetShortGuardIntervalFor80Mhz', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetSupportedChannelWidthSet() const [member function]
    cls.add_method('GetSupportedChannelWidthSet', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint64_t ns3::VhtCapabilities::GetSupportedMcsAndNssSet() const [member function]
    cls.add_method('GetSupportedMcsAndNssSet', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint16_t ns3::VhtCapabilities::GetTxHighestSupportedLgiDataRate() const [member function]
    cls.add_method('GetTxHighestSupportedLgiDataRate', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint16_t ns3::VhtCapabilities::GetTxMcsMap() const [member function]
    cls.add_method('GetTxMcsMap', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint8_t ns3::VhtCapabilities::GetTxStbc() const [member function]
    cls.add_method('GetTxStbc', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): uint32_t ns3::VhtCapabilities::GetVhtCapabilitiesInfo() const [member function]
    cls.add_method('GetVhtCapabilitiesInfo', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): bool ns3::VhtCapabilities::IsSupportedMcs(uint8_t mcs, uint8_t Nss) const [member function]
    cls.add_method('IsSupportedMcs', 
                   'bool', 
                   [param('uint8_t', 'mcs'), param('uint8_t', 'Nss')], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): bool ns3::VhtCapabilities::IsSupportedRxMcs(uint8_t mcs) const [member function]
    cls.add_method('IsSupportedRxMcs', 
                   'bool', 
                   [param('uint8_t', 'mcs')], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): bool ns3::VhtCapabilities::IsSupportedTxMcs(uint8_t mcs) const [member function]
    cls.add_method('IsSupportedTxMcs', 
                   'bool', 
                   [param('uint8_t', 'mcs')], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): ns3::Buffer::Iterator ns3::VhtCapabilities::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SerializeInformationField(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetMaxAmpduLengthExponent(uint8_t exponent) [member function]
    cls.add_method('SetMaxAmpduLengthExponent', 
                   'void', 
                   [param('uint8_t', 'exponent')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetMaxMpduLength(uint8_t length) [member function]
    cls.add_method('SetMaxMpduLength', 
                   'void', 
                   [param('uint8_t', 'length')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetRxHighestSupportedLgiDataRate(uint16_t supporteddatarate) [member function]
    cls.add_method('SetRxHighestSupportedLgiDataRate', 
                   'void', 
                   [param('uint16_t', 'supporteddatarate')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetRxLdpc(uint8_t rxldpc) [member function]
    cls.add_method('SetRxLdpc', 
                   'void', 
                   [param('uint8_t', 'rxldpc')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetRxMcsMap(uint16_t map) [member function]
    cls.add_method('SetRxMcsMap', 
                   'void', 
                   [param('uint16_t', 'map')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetRxMcsMap(uint8_t mcs, uint8_t nss) [member function]
    cls.add_method('SetRxMcsMap', 
                   'void', 
                   [param('uint8_t', 'mcs'), param('uint8_t', 'nss')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetRxStbc(uint8_t rxstbc) [member function]
    cls.add_method('SetRxStbc', 
                   'void', 
                   [param('uint8_t', 'rxstbc')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetShortGuardIntervalFor160Mhz(uint8_t shortguardinterval) [member function]
    cls.add_method('SetShortGuardIntervalFor160Mhz', 
                   'void', 
                   [param('uint8_t', 'shortguardinterval')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetShortGuardIntervalFor80Mhz(uint8_t shortguardinterval) [member function]
    cls.add_method('SetShortGuardIntervalFor80Mhz', 
                   'void', 
                   [param('uint8_t', 'shortguardinterval')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetSupportedChannelWidthSet(uint8_t channelwidthset) [member function]
    cls.add_method('SetSupportedChannelWidthSet', 
                   'void', 
                   [param('uint8_t', 'channelwidthset')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetSupportedMcsAndNssSet(uint64_t ctrl) [member function]
    cls.add_method('SetSupportedMcsAndNssSet', 
                   'void', 
                   [param('uint64_t', 'ctrl')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetTxHighestSupportedLgiDataRate(uint16_t supporteddatarate) [member function]
    cls.add_method('SetTxHighestSupportedLgiDataRate', 
                   'void', 
                   [param('uint16_t', 'supporteddatarate')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetTxMcsMap(uint16_t map) [member function]
    cls.add_method('SetTxMcsMap', 
                   'void', 
                   [param('uint16_t', 'map')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetTxMcsMap(uint8_t mcs, uint8_t nss) [member function]
    cls.add_method('SetTxMcsMap', 
                   'void', 
                   [param('uint8_t', 'mcs'), param('uint8_t', 'nss')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetTxStbc(uint8_t txstbc) [member function]
    cls.add_method('SetTxStbc', 
                   'void', 
                   [param('uint8_t', 'txstbc')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetVhtCapabilitiesInfo(uint32_t ctrl) [member function]
    cls.add_method('SetVhtCapabilitiesInfo', 
                   'void', 
                   [param('uint32_t', 'ctrl')])
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilities::SetVhtSupported(uint8_t vhtsupported) [member function]
    cls.add_method('SetVhtSupported', 
                   'void', 
                   [param('uint8_t', 'vhtsupported')])
    return

def register_Ns3VhtCapabilitiesChecker_methods(root_module, cls):
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilitiesChecker::VhtCapabilitiesChecker() [constructor]
    cls.add_constructor([])
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilitiesChecker::VhtCapabilitiesChecker(ns3::VhtCapabilitiesChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VhtCapabilitiesChecker const &', 'arg0')])
    return

def register_Ns3VhtCapabilitiesValue_methods(root_module, cls):
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilitiesValue::VhtCapabilitiesValue() [constructor]
    cls.add_constructor([])
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilitiesValue::VhtCapabilitiesValue(ns3::VhtCapabilitiesValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VhtCapabilitiesValue const &', 'arg0')])
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilitiesValue::VhtCapabilitiesValue(ns3::VhtCapabilities const & value) [constructor]
    cls.add_constructor([param('ns3::VhtCapabilities const &', 'value')])
    ## vht-capabilities.h (module 'wifi'): ns3::Ptr<ns3::AttributeValue> ns3::VhtCapabilitiesValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## vht-capabilities.h (module 'wifi'): bool ns3::VhtCapabilitiesValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## vht-capabilities.h (module 'wifi'): ns3::VhtCapabilities ns3::VhtCapabilitiesValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::VhtCapabilities', 
                   [], 
                   is_const=True)
    ## vht-capabilities.h (module 'wifi'): std::string ns3::VhtCapabilitiesValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## vht-capabilities.h (module 'wifi'): void ns3::VhtCapabilitiesValue::Set(ns3::VhtCapabilities const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::VhtCapabilities const &', 'value')])
    return

def register_Ns3WifiModeChecker_methods(root_module, cls):
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeChecker::WifiModeChecker() [constructor]
    cls.add_constructor([])
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeChecker::WifiModeChecker(ns3::WifiModeChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeChecker const &', 'arg0')])
    return

def register_Ns3WifiModeValue_methods(root_module, cls):
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeValue::WifiModeValue() [constructor]
    cls.add_constructor([])
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeValue::WifiModeValue(ns3::WifiModeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WifiModeValue const &', 'arg0')])
    ## wifi-mode.h (module 'wifi'): ns3::WifiModeValue::WifiModeValue(ns3::WifiMode const & value) [constructor]
    cls.add_constructor([param('ns3::WifiMode const &', 'value')])
    ## wifi-mode.h (module 'wifi'): ns3::Ptr<ns3::AttributeValue> ns3::WifiModeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wifi-mode.h (module 'wifi'): bool ns3::WifiModeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## wifi-mode.h (module 'wifi'): ns3::WifiMode ns3::WifiModeValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::WifiMode', 
                   [], 
                   is_const=True)
    ## wifi-mode.h (module 'wifi'): std::string ns3::WifiModeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## wifi-mode.h (module 'wifi'): void ns3::WifiModeValue::Set(ns3::WifiMode const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::WifiMode const &', 'value')])
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

def register_Ns3BridgeChannel_methods(root_module, cls):
    ## bridge-channel.h (module 'bridge'): static ns3::TypeId ns3::BridgeChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## bridge-channel.h (module 'bridge'): ns3::BridgeChannel::BridgeChannel() [constructor]
    cls.add_constructor([])
    ## bridge-channel.h (module 'bridge'): void ns3::BridgeChannel::AddChannel(ns3::Ptr<ns3::Channel> bridgedChannel) [member function]
    cls.add_method('AddChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::Channel >', 'bridgedChannel')])
    ## bridge-channel.h (module 'bridge'): uint32_t ns3::BridgeChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## bridge-channel.h (module 'bridge'): ns3::Ptr<ns3::NetDevice> ns3::BridgeChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3DcaTxop_methods(root_module, cls):
    ## dca-txop.h (module 'wifi'): static ns3::TypeId ns3::DcaTxop::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dca-txop.h (module 'wifi'): ns3::DcaTxop::DcaTxop() [constructor]
    cls.add_constructor([])
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetLow(ns3::Ptr<ns3::MacLow> low) [member function]
    cls.add_method('SetLow', 
                   'void', 
                   [param('ns3::Ptr< ns3::MacLow >', 'low')])
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetManager(ns3::DcfManager * manager) [member function]
    cls.add_method('SetManager', 
                   'void', 
                   [param('ns3::DcfManager *', 'manager')])
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetWifiRemoteStationManager(ns3::Ptr<ns3::WifiRemoteStationManager> remoteManager) [member function]
    cls.add_method('SetWifiRemoteStationManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::WifiRemoteStationManager >', 'remoteManager')])
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetTxMiddle(ns3::MacTxMiddle * txMiddle) [member function]
    cls.add_method('SetTxMiddle', 
                   'void', 
                   [param('ns3::MacTxMiddle *', 'txMiddle')])
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetTxOkCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxOkCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetTxFailedCallback(ns3::Callback<void, ns3::WifiMacHeader const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('SetTxFailedCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::WifiMacHeader const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## dca-txop.h (module 'wifi'): ns3::Ptr<ns3::WifiMacQueue> ns3::DcaTxop::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::WifiMacQueue >', 
                   [], 
                   is_const=True)
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetMinCw(uint32_t minCw) [member function]
    cls.add_method('SetMinCw', 
                   'void', 
                   [param('uint32_t', 'minCw')], 
                   is_virtual=True)
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetMaxCw(uint32_t maxCw) [member function]
    cls.add_method('SetMaxCw', 
                   'void', 
                   [param('uint32_t', 'maxCw')], 
                   is_virtual=True)
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::SetAifsn(uint32_t aifsn) [member function]
    cls.add_method('SetAifsn', 
                   'void', 
                   [param('uint32_t', 'aifsn')], 
                   is_virtual=True)
    ## dca-txop.h (module 'wifi'): uint32_t ns3::DcaTxop::GetMinCw() const [member function]
    cls.add_method('GetMinCw', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dca-txop.h (module 'wifi'): uint32_t ns3::DcaTxop::GetMaxCw() const [member function]
    cls.add_method('GetMaxCw', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dca-txop.h (module 'wifi'): uint32_t ns3::DcaTxop::GetAifsn() const [member function]
    cls.add_method('GetAifsn', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::Queue(ns3::Ptr<ns3::Packet const> packet, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('Queue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## dca-txop.h (module 'wifi'): int64_t ns3::DcaTxop::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')])
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## dca-txop.h (module 'wifi'): void ns3::DcaTxop::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Dot11sStack_methods(root_module, cls):
    ## dot11s-installer.h (module 'mesh'): ns3::Dot11sStack::Dot11sStack(ns3::Dot11sStack const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Dot11sStack const &', 'arg0')])
    ## dot11s-installer.h (module 'mesh'): ns3::Dot11sStack::Dot11sStack() [constructor]
    cls.add_constructor([])
    ## dot11s-installer.h (module 'mesh'): void ns3::Dot11sStack::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## dot11s-installer.h (module 'mesh'): static ns3::TypeId ns3::Dot11sStack::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dot11s-installer.h (module 'mesh'): bool ns3::Dot11sStack::InstallStack(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('InstallStack', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')], 
                   is_virtual=True)
    ## dot11s-installer.h (module 'mesh'): void ns3::Dot11sStack::Report(ns3::Ptr<ns3::MeshPointDevice> const mp, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp'), param('std::ostream &', 'arg1')], 
                   is_virtual=True)
    ## dot11s-installer.h (module 'mesh'): void ns3::Dot11sStack::ResetStats(ns3::Ptr<ns3::MeshPointDevice> const mp) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp')], 
                   is_virtual=True)
    return

def register_Ns3FlameStack_methods(root_module, cls):
    ## flame-installer.h (module 'mesh'): ns3::FlameStack::FlameStack(ns3::FlameStack const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlameStack const &', 'arg0')])
    ## flame-installer.h (module 'mesh'): ns3::FlameStack::FlameStack() [constructor]
    cls.add_constructor([])
    ## flame-installer.h (module 'mesh'): void ns3::FlameStack::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-installer.h (module 'mesh'): static ns3::TypeId ns3::FlameStack::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-installer.h (module 'mesh'): bool ns3::FlameStack::InstallStack(ns3::Ptr<ns3::MeshPointDevice> mp) [member function]
    cls.add_method('InstallStack', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'mp')], 
                   is_virtual=True)
    ## flame-installer.h (module 'mesh'): void ns3::FlameStack::Report(ns3::Ptr<ns3::MeshPointDevice> const mp, std::ostream & arg1) [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp'), param('std::ostream &', 'arg1')], 
                   is_virtual=True)
    ## flame-installer.h (module 'mesh'): void ns3::FlameStack::ResetStats(ns3::Ptr<ns3::MeshPointDevice> const mp) [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshPointDevice > const', 'mp')], 
                   is_virtual=True)
    return

def register_Ns3MeshPointDevice_methods(root_module, cls):
    ## mesh-point-device.h (module 'mesh'): ns3::MeshPointDevice::MeshPointDevice(ns3::MeshPointDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MeshPointDevice const &', 'arg0')])
    ## mesh-point-device.h (module 'mesh'): ns3::MeshPointDevice::MeshPointDevice() [constructor]
    cls.add_constructor([])
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::AddInterface(ns3::Ptr<ns3::NetDevice> port) [member function]
    cls.add_method('AddInterface', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'port')])
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): ns3::Address ns3::MeshPointDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): ns3::Address ns3::MeshPointDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): ns3::Ptr<ns3::Channel> ns3::MeshPointDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): uint32_t ns3::MeshPointDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): ns3::Ptr<ns3::NetDevice> ns3::MeshPointDevice::GetInterface(uint32_t id) const [member function]
    cls.add_method('GetInterface', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'id')], 
                   is_const=True)
    ## mesh-point-device.h (module 'mesh'): std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > ns3::MeshPointDevice::GetInterfaces() const [member function]
    cls.add_method('GetInterfaces', 
                   'std::vector< ns3::Ptr< ns3::NetDevice > >', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h (module 'mesh'): uint16_t ns3::MeshPointDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): ns3::Address ns3::MeshPointDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): ns3::Address ns3::MeshPointDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): uint32_t ns3::MeshPointDevice::GetNInterfaces() const [member function]
    cls.add_method('GetNInterfaces', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h (module 'mesh'): ns3::Ptr<ns3::Node> ns3::MeshPointDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): ns3::Ptr<ns3::MeshL2RoutingProtocol> ns3::MeshPointDevice::GetRoutingProtocol() const [member function]
    cls.add_method('GetRoutingProtocol', 
                   'ns3::Ptr< ns3::MeshL2RoutingProtocol >', 
                   [], 
                   is_const=True)
    ## mesh-point-device.h (module 'mesh'): static ns3::TypeId ns3::MeshPointDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::Report(std::ostream & os) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::SetAddress(ns3::Address a) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'a')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## mesh-point-device.h (module 'mesh'): void ns3::MeshPointDevice::SetRoutingProtocol(ns3::Ptr<ns3::MeshL2RoutingProtocol> protocol) [member function]
    cls.add_method('SetRoutingProtocol', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshL2RoutingProtocol >', 'protocol')])
    ## mesh-point-device.h (module 'mesh'): bool ns3::MeshPointDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3MeshWifiInterfaceMac_methods(root_module, cls):
    ## mesh-wifi-interface-mac.h (module 'mesh'): ns3::MeshWifiInterfaceMac::MeshWifiInterfaceMac() [constructor]
    cls.add_constructor([])
    ## mesh-wifi-interface-mac.h (module 'mesh'): int64_t ns3::MeshWifiInterfaceMac::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): bool ns3::MeshWifiInterfaceMac::CheckSupportedRates(ns3::SupportedRates rates) const [member function]
    cls.add_method('CheckSupportedRates', 
                   'bool', 
                   [param('ns3::SupportedRates', 'rates')], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::Enqueue(ns3::Ptr<ns3::Packet const> packet, ns3::Mac48Address to) [member function]
    cls.add_method('Enqueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::FinishConfigureStandard(ns3::WifiPhyStandard standard) [member function]
    cls.add_method('FinishConfigureStandard', 
                   'void', 
                   [param('ns3::WifiPhyStandard', 'standard')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): ns3::Time ns3::MeshWifiInterfaceMac::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): uint16_t ns3::MeshWifiInterfaceMac::GetFrequencyChannel() const [member function]
    cls.add_method('GetFrequencyChannel', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): uint32_t ns3::MeshWifiInterfaceMac::GetLinkMetric(ns3::Mac48Address peerAddress) [member function]
    cls.add_method('GetLinkMetric', 
                   'uint32_t', 
                   [param('ns3::Mac48Address', 'peerAddress')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): ns3::Mac48Address ns3::MeshWifiInterfaceMac::GetMeshPointAddress() const [member function]
    cls.add_method('GetMeshPointAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): ns3::WifiPhyStandard ns3::MeshWifiInterfaceMac::GetPhyStandard() const [member function]
    cls.add_method('GetPhyStandard', 
                   'ns3::WifiPhyStandard', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): ns3::SupportedRates ns3::MeshWifiInterfaceMac::GetSupportedRates() const [member function]
    cls.add_method('GetSupportedRates', 
                   'ns3::SupportedRates', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): ns3::Time ns3::MeshWifiInterfaceMac::GetTbtt() const [member function]
    cls.add_method('GetTbtt', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): static ns3::TypeId ns3::MeshWifiInterfaceMac::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::InstallPlugin(ns3::Ptr<ns3::MeshWifiInterfaceMacPlugin> plugin) [member function]
    cls.add_method('InstallPlugin', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMacPlugin >', 'plugin')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::SendManagementFrame(ns3::Ptr<ns3::Packet> frame, ns3::WifiMacHeader const & hdr) [member function]
    cls.add_method('SendManagementFrame', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'frame'), param('ns3::WifiMacHeader const &', 'hdr')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::SetBeaconGeneration(bool enable) [member function]
    cls.add_method('SetBeaconGeneration', 
                   'void', 
                   [param('bool', 'enable')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::SetBeaconInterval(ns3::Time interval) [member function]
    cls.add_method('SetBeaconInterval', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::SetLinkMetricCallback(ns3::Callback<unsigned int, ns3::Mac48Address, ns3::Ptr<ns3::MeshWifiInterfaceMac>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetLinkMetricCallback', 
                   'void', 
                   [param('ns3::Callback< unsigned int, ns3::Mac48Address, ns3::Ptr< ns3::MeshWifiInterfaceMac >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::SetLinkUpCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> linkUp) [member function]
    cls.add_method('SetLinkUpCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'linkUp')], 
                   is_virtual=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::SetMeshPointAddress(ns3::Mac48Address arg0) [member function]
    cls.add_method('SetMeshPointAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'arg0')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::SetRandomStartDelay(ns3::Time interval) [member function]
    cls.add_method('SetRandomStartDelay', 
                   'void', 
                   [param('ns3::Time', 'interval')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::ShiftTbtt(ns3::Time shift) [member function]
    cls.add_method('ShiftTbtt', 
                   'void', 
                   [param('ns3::Time', 'shift')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): bool ns3::MeshWifiInterfaceMac::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::SwitchFrequencyChannel(uint16_t new_id) [member function]
    cls.add_method('SwitchFrequencyChannel', 
                   'void', 
                   [param('uint16_t', 'new_id')])
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## mesh-wifi-interface-mac.h (module 'mesh'): void ns3::MeshWifiInterfaceMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const * hdr) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const *', 'hdr')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3HashImplementation_methods(root_module, cls):
    ## hash-function.h (module 'core'): ns3::Hash::Implementation::Implementation(ns3::Hash::Implementation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Implementation const &', 'arg0')])
    ## hash-function.h (module 'core'): ns3::Hash::Implementation::Implementation() [constructor]
    cls.add_constructor([])
    ## hash-function.h (module 'core'): uint32_t ns3::Hash::Implementation::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_pure_virtual=True, is_virtual=True)
    ## hash-function.h (module 'core'): uint64_t ns3::Hash::Implementation::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-function.h (module 'core'): void ns3::Hash::Implementation::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3HashFunctionFnv1a_methods(root_module, cls):
    ## hash-fnv.h (module 'core'): ns3::Hash::Function::Fnv1a::Fnv1a(ns3::Hash::Function::Fnv1a const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Function::Fnv1a const &', 'arg0')])
    ## hash-fnv.h (module 'core'): ns3::Hash::Function::Fnv1a::Fnv1a() [constructor]
    cls.add_constructor([])
    ## hash-fnv.h (module 'core'): uint32_t ns3::Hash::Function::Fnv1a::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-fnv.h (module 'core'): uint64_t ns3::Hash::Function::Fnv1a::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-fnv.h (module 'core'): void ns3::Hash::Function::Fnv1a::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3HashFunctionHash32_methods(root_module, cls):
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash32::Hash32(ns3::Hash::Function::Hash32 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Function::Hash32 const &', 'arg0')])
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash32::Hash32(ns3::Hash::Hash32Function_ptr hp) [constructor]
    cls.add_constructor([param('ns3::Hash::Hash32Function_ptr', 'hp')])
    ## hash-function.h (module 'core'): uint32_t ns3::Hash::Function::Hash32::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-function.h (module 'core'): void ns3::Hash::Function::Hash32::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3HashFunctionHash64_methods(root_module, cls):
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash64::Hash64(ns3::Hash::Function::Hash64 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Function::Hash64 const &', 'arg0')])
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash64::Hash64(ns3::Hash::Hash64Function_ptr hp) [constructor]
    cls.add_constructor([param('ns3::Hash::Hash64Function_ptr', 'hp')])
    ## hash-function.h (module 'core'): uint32_t ns3::Hash::Function::Hash64::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-function.h (module 'core'): uint64_t ns3::Hash::Function::Hash64::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-function.h (module 'core'): void ns3::Hash::Function::Hash64::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3HashFunctionMurmur3_methods(root_module, cls):
    ## hash-murmur3.h (module 'core'): ns3::Hash::Function::Murmur3::Murmur3(ns3::Hash::Function::Murmur3 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Function::Murmur3 const &', 'arg0')])
    ## hash-murmur3.h (module 'core'): ns3::Hash::Function::Murmur3::Murmur3() [constructor]
    cls.add_constructor([])
    ## hash-murmur3.h (module 'core'): uint32_t ns3::Hash::Function::Murmur3::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-murmur3.h (module 'core'): uint64_t ns3::Hash::Function::Murmur3::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-murmur3.h (module 'core'): void ns3::Hash::Function::Murmur3::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3Dot11sDestinationAddressUnit_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-preq.h (module 'mesh'): ns3::dot11s::DestinationAddressUnit::DestinationAddressUnit(ns3::dot11s::DestinationAddressUnit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::DestinationAddressUnit const &', 'arg0')])
    ## ie-dot11s-preq.h (module 'mesh'): ns3::dot11s::DestinationAddressUnit::DestinationAddressUnit() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-preq.h (module 'mesh'): uint32_t ns3::dot11s::DestinationAddressUnit::GetDestSeqNumber() const [member function]
    cls.add_method('GetDestSeqNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::DestinationAddressUnit::GetDestinationAddress() const [member function]
    cls.add_method('GetDestinationAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): bool ns3::dot11s::DestinationAddressUnit::IsDo() [member function]
    cls.add_method('IsDo', 
                   'bool', 
                   [])
    ## ie-dot11s-preq.h (module 'mesh'): bool ns3::dot11s::DestinationAddressUnit::IsRf() [member function]
    cls.add_method('IsRf', 
                   'bool', 
                   [])
    ## ie-dot11s-preq.h (module 'mesh'): bool ns3::dot11s::DestinationAddressUnit::IsUsn() [member function]
    cls.add_method('IsUsn', 
                   'bool', 
                   [])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::DestinationAddressUnit::SetDestSeqNumber(uint32_t dest_seq_number) [member function]
    cls.add_method('SetDestSeqNumber', 
                   'void', 
                   [param('uint32_t', 'dest_seq_number')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::DestinationAddressUnit::SetDestinationAddress(ns3::Mac48Address dest_address) [member function]
    cls.add_method('SetDestinationAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'dest_address')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::DestinationAddressUnit::SetFlags(bool doFlag, bool rfFlag, bool usnFlag) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('bool', 'doFlag'), param('bool', 'rfFlag'), param('bool', 'usnFlag')])
    return

def register_Ns3Dot11sDot11sMeshCapability_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::Dot11sMeshCapability(ns3::dot11s::Dot11sMeshCapability const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::Dot11sMeshCapability const &', 'arg0')])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::Dot11sMeshCapability() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::Buffer::Iterator ns3::dot11s::Dot11sMeshCapability::Deserialize(ns3::Buffer::Iterator i) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')])
    ## ie-dot11s-configuration.h (module 'mesh'): uint8_t ns3::dot11s::Dot11sMeshCapability::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-configuration.h (module 'mesh'): uint8_t ns3::dot11s::Dot11sMeshCapability::GetUint8() const [member function]
    cls.add_method('GetUint8', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-configuration.h (module 'mesh'): bool ns3::dot11s::Dot11sMeshCapability::Is(uint8_t cap, uint8_t n) const [member function]
    cls.add_method('Is', 
                   'bool', 
                   [param('uint8_t', 'cap'), param('uint8_t', 'n')], 
                   is_const=True)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::Buffer::Iterator ns3::dot11s::Dot11sMeshCapability::Serialize(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer::Iterator', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::MCCAEnabled [variable]
    cls.add_instance_attribute('MCCAEnabled', 'bool', is_const=False)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::MCCASupported [variable]
    cls.add_instance_attribute('MCCASupported', 'bool', is_const=False)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::TBTTAdjustment [variable]
    cls.add_instance_attribute('TBTTAdjustment', 'bool', is_const=False)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::acceptPeerLinks [variable]
    cls.add_instance_attribute('acceptPeerLinks', 'bool', is_const=False)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::beaconTimingReport [variable]
    cls.add_instance_attribute('beaconTimingReport', 'bool', is_const=False)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::forwarding [variable]
    cls.add_instance_attribute('forwarding', 'bool', is_const=False)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability::powerSaveLevel [variable]
    cls.add_instance_attribute('powerSaveLevel', 'bool', is_const=False)
    return

def register_Ns3Dot11sHwmpProtocol_methods(root_module, cls):
    ## hwmp-protocol.h (module 'mesh'): ns3::dot11s::HwmpProtocol::HwmpProtocol() [constructor]
    cls.add_constructor([])
    ## hwmp-protocol.h (module 'mesh'): int64_t ns3::dot11s::HwmpProtocol::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')])
    ## hwmp-protocol.h (module 'mesh'): void ns3::dot11s::HwmpProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## hwmp-protocol.h (module 'mesh'): static ns3::TypeId ns3::dot11s::HwmpProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## hwmp-protocol.h (module 'mesh'): bool ns3::dot11s::HwmpProtocol::Install(ns3::Ptr<ns3::MeshPointDevice> arg0) [member function]
    cls.add_method('Install', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'arg0')])
    ## hwmp-protocol.h (module 'mesh'): void ns3::dot11s::HwmpProtocol::PeerLinkStatus(ns3::Mac48Address meshPontAddress, ns3::Mac48Address peerAddress, uint32_t interface, bool status) [member function]
    cls.add_method('PeerLinkStatus', 
                   'void', 
                   [param('ns3::Mac48Address', 'meshPontAddress'), param('ns3::Mac48Address', 'peerAddress'), param('uint32_t', 'interface'), param('bool', 'status')])
    ## hwmp-protocol.h (module 'mesh'): bool ns3::dot11s::HwmpProtocol::RemoveRoutingStuff(uint32_t fromIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet> packet, uint16_t & protocolType) [member function]
    cls.add_method('RemoveRoutingStuff', 
                   'bool', 
                   [param('uint32_t', 'fromIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t &', 'protocolType')], 
                   is_virtual=True)
    ## hwmp-protocol.h (module 'mesh'): void ns3::dot11s::HwmpProtocol::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## hwmp-protocol.h (module 'mesh'): bool ns3::dot11s::HwmpProtocol::RequestRoute(uint32_t sourceIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet const> packet, uint16_t protocolType, ns3::Callback<void, bool, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'sourceIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocolType'), param('ns3::Callback< void, bool, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_virtual=True)
    ## hwmp-protocol.h (module 'mesh'): void ns3::dot11s::HwmpProtocol::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## hwmp-protocol.h (module 'mesh'): void ns3::dot11s::HwmpProtocol::SetNeighboursCallback(ns3::Callback<std::vector<ns3::Mac48Address, std::allocator<ns3::Mac48Address> >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetNeighboursCallback', 
                   'void', 
                   [param('ns3::Callback< std::vector< ns3::Mac48Address >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## hwmp-protocol.h (module 'mesh'): void ns3::dot11s::HwmpProtocol::SetRoot() [member function]
    cls.add_method('SetRoot', 
                   'void', 
                   [])
    ## hwmp-protocol.h (module 'mesh'): void ns3::dot11s::HwmpProtocol::UnsetRoot() [member function]
    cls.add_method('UnsetRoot', 
                   'void', 
                   [])
    ## hwmp-protocol.h (module 'mesh'): void ns3::dot11s::HwmpProtocol::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Dot11sHwmpProtocolFailedDestination_methods(root_module, cls):
    ## hwmp-protocol.h (module 'mesh'): ns3::dot11s::HwmpProtocol::FailedDestination::FailedDestination() [constructor]
    cls.add_constructor([])
    ## hwmp-protocol.h (module 'mesh'): ns3::dot11s::HwmpProtocol::FailedDestination::FailedDestination(ns3::dot11s::HwmpProtocol::FailedDestination const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::HwmpProtocol::FailedDestination const &', 'arg0')])
    ## hwmp-protocol.h (module 'mesh'): ns3::dot11s::HwmpProtocol::FailedDestination::destination [variable]
    cls.add_instance_attribute('destination', 'ns3::Mac48Address', is_const=False)
    ## hwmp-protocol.h (module 'mesh'): ns3::dot11s::HwmpProtocol::FailedDestination::seqnum [variable]
    cls.add_instance_attribute('seqnum', 'uint32_t', is_const=False)
    return

def register_Ns3Dot11sHwmpRtable_methods(root_module, cls):
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::HwmpRtable(ns3::dot11s::HwmpRtable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::HwmpRtable const &', 'arg0')])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::HwmpRtable() [constructor]
    cls.add_constructor([])
    ## hwmp-rtable.h (module 'mesh'): void ns3::dot11s::HwmpRtable::AddPrecursor(ns3::Mac48Address destination, uint32_t precursorInterface, ns3::Mac48Address precursorAddress, ns3::Time lifetime) [member function]
    cls.add_method('AddPrecursor', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination'), param('uint32_t', 'precursorInterface'), param('ns3::Mac48Address', 'precursorAddress'), param('ns3::Time', 'lifetime')])
    ## hwmp-rtable.h (module 'mesh'): void ns3::dot11s::HwmpRtable::AddProactivePath(uint32_t metric, ns3::Mac48Address root, ns3::Mac48Address retransmitter, uint32_t interface, ns3::Time lifetime, uint32_t seqnum) [member function]
    cls.add_method('AddProactivePath', 
                   'void', 
                   [param('uint32_t', 'metric'), param('ns3::Mac48Address', 'root'), param('ns3::Mac48Address', 'retransmitter'), param('uint32_t', 'interface'), param('ns3::Time', 'lifetime'), param('uint32_t', 'seqnum')])
    ## hwmp-rtable.h (module 'mesh'): void ns3::dot11s::HwmpRtable::AddReactivePath(ns3::Mac48Address destination, ns3::Mac48Address retransmitter, uint32_t interface, uint32_t metric, ns3::Time lifetime, uint32_t seqnum) [member function]
    cls.add_method('AddReactivePath', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination'), param('ns3::Mac48Address', 'retransmitter'), param('uint32_t', 'interface'), param('uint32_t', 'metric'), param('ns3::Time', 'lifetime'), param('uint32_t', 'seqnum')])
    ## hwmp-rtable.h (module 'mesh'): void ns3::dot11s::HwmpRtable::DeleteProactivePath() [member function]
    cls.add_method('DeleteProactivePath', 
                   'void', 
                   [])
    ## hwmp-rtable.h (module 'mesh'): void ns3::dot11s::HwmpRtable::DeleteProactivePath(ns3::Mac48Address root) [member function]
    cls.add_method('DeleteProactivePath', 
                   'void', 
                   [param('ns3::Mac48Address', 'root')])
    ## hwmp-rtable.h (module 'mesh'): void ns3::dot11s::HwmpRtable::DeleteReactivePath(ns3::Mac48Address destination) [member function]
    cls.add_method('DeleteReactivePath', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination')])
    ## hwmp-rtable.h (module 'mesh'): void ns3::dot11s::HwmpRtable::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## hwmp-rtable.h (module 'mesh'): std::vector<std::pair<unsigned int, ns3::Mac48Address>, std::allocator<std::pair<unsigned int, ns3::Mac48Address> > > ns3::dot11s::HwmpRtable::GetPrecursors(ns3::Mac48Address destination) [member function]
    cls.add_method('GetPrecursors', 
                   'std::vector< std::pair< unsigned int, ns3::Mac48Address > >', 
                   [param('ns3::Mac48Address', 'destination')])
    ## hwmp-rtable.h (module 'mesh'): static ns3::TypeId ns3::dot11s::HwmpRtable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## hwmp-rtable.h (module 'mesh'): std::vector<ns3::dot11s::HwmpProtocol::FailedDestination, std::allocator<ns3::dot11s::HwmpProtocol::FailedDestination> > ns3::dot11s::HwmpRtable::GetUnreachableDestinations(ns3::Mac48Address peerAddress) [member function]
    cls.add_method('GetUnreachableDestinations', 
                   'std::vector< ns3::dot11s::HwmpProtocol::FailedDestination >', 
                   [param('ns3::Mac48Address', 'peerAddress')])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult ns3::dot11s::HwmpRtable::LookupProactive() [member function]
    cls.add_method('LookupProactive', 
                   'ns3::dot11s::HwmpRtable::LookupResult', 
                   [])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult ns3::dot11s::HwmpRtable::LookupProactiveExpired() [member function]
    cls.add_method('LookupProactiveExpired', 
                   'ns3::dot11s::HwmpRtable::LookupResult', 
                   [])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult ns3::dot11s::HwmpRtable::LookupReactive(ns3::Mac48Address destination) [member function]
    cls.add_method('LookupReactive', 
                   'ns3::dot11s::HwmpRtable::LookupResult', 
                   [param('ns3::Mac48Address', 'destination')])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult ns3::dot11s::HwmpRtable::LookupReactiveExpired(ns3::Mac48Address destination) [member function]
    cls.add_method('LookupReactiveExpired', 
                   'ns3::dot11s::HwmpRtable::LookupResult', 
                   [param('ns3::Mac48Address', 'destination')])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::INTERFACE_ANY [variable]
    cls.add_static_attribute('INTERFACE_ANY', 'uint32_t const', is_const=True)
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::MAX_METRIC [variable]
    cls.add_static_attribute('MAX_METRIC', 'uint32_t const', is_const=True)
    return

def register_Ns3Dot11sHwmpRtableLookupResult_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult::LookupResult(ns3::dot11s::HwmpRtable::LookupResult const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::HwmpRtable::LookupResult const &', 'arg0')])
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult::LookupResult(ns3::Mac48Address r=ns3::Mac48Address::GetBroadcast(), uint32_t i=ns3::dot11s::HwmpRtable::INTERFACE_ANY, uint32_t m=ns3::dot11s::HwmpRtable::MAX_METRIC, uint32_t s=0, ns3::Time l=ns3::Seconds( )) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'r', default_value='ns3::Mac48Address::GetBroadcast()'), param('uint32_t', 'i', default_value='ns3::dot11s::HwmpRtable::INTERFACE_ANY'), param('uint32_t', 'm', default_value='ns3::dot11s::HwmpRtable::MAX_METRIC'), param('uint32_t', 's', default_value='0'), param('ns3::Time', 'l', default_value='ns3::Seconds(0)')])
    ## hwmp-rtable.h (module 'mesh'): bool ns3::dot11s::HwmpRtable::LookupResult::IsValid() const [member function]
    cls.add_method('IsValid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult::ifIndex [variable]
    cls.add_instance_attribute('ifIndex', 'uint32_t', is_const=False)
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult::lifetime [variable]
    cls.add_instance_attribute('lifetime', 'ns3::Time', is_const=False)
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult::metric [variable]
    cls.add_instance_attribute('metric', 'uint32_t', is_const=False)
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult::retransmitter [variable]
    cls.add_instance_attribute('retransmitter', 'ns3::Mac48Address', is_const=False)
    ## hwmp-rtable.h (module 'mesh'): ns3::dot11s::HwmpRtable::LookupResult::seqnum [variable]
    cls.add_instance_attribute('seqnum', 'uint32_t', is_const=False)
    return

def register_Ns3Dot11sIeBeaconTiming_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ie-dot11s-beacon-timing.h (module 'mesh'): ns3::dot11s::IeBeaconTiming::IeBeaconTiming(ns3::dot11s::IeBeaconTiming const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeBeaconTiming const &', 'arg0')])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): ns3::dot11s::IeBeaconTiming::IeBeaconTiming() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): void ns3::dot11s::IeBeaconTiming::AddNeighboursTimingElementUnit(uint16_t aid, ns3::Time last_beacon, ns3::Time beacon_interval) [member function]
    cls.add_method('AddNeighboursTimingElementUnit', 
                   'void', 
                   [param('uint16_t', 'aid'), param('ns3::Time', 'last_beacon'), param('ns3::Time', 'beacon_interval')])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): void ns3::dot11s::IeBeaconTiming::ClearTimingElement() [member function]
    cls.add_method('ClearTimingElement', 
                   'void', 
                   [])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): void ns3::dot11s::IeBeaconTiming::DelNeighboursTimingElementUnit(uint16_t aid, ns3::Time last_beacon, ns3::Time beacon_interval) [member function]
    cls.add_method('DelNeighboursTimingElementUnit', 
                   'void', 
                   [param('uint16_t', 'aid'), param('ns3::Time', 'last_beacon'), param('ns3::Time', 'beacon_interval')])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): uint8_t ns3::dot11s::IeBeaconTiming::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-beacon-timing.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IeBeaconTiming::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-beacon-timing.h (module 'mesh'): uint8_t ns3::dot11s::IeBeaconTiming::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-beacon-timing.h (module 'mesh'): std::vector<ns3::Ptr<ns3::dot11s::IeBeaconTimingUnit>, std::allocator<ns3::Ptr<ns3::dot11s::IeBeaconTimingUnit> > > ns3::dot11s::IeBeaconTiming::GetNeighboursTimingElementsList() [member function]
    cls.add_method('GetNeighboursTimingElementsList', 
                   'std::vector< ns3::Ptr< ns3::dot11s::IeBeaconTimingUnit > >', 
                   [])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): void ns3::dot11s::IeBeaconTiming::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-beacon-timing.h (module 'mesh'): void ns3::dot11s::IeBeaconTiming::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Dot11sIeBeaconTimingUnit_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-beacon-timing.h (module 'mesh'): ns3::dot11s::IeBeaconTimingUnit::IeBeaconTimingUnit(ns3::dot11s::IeBeaconTimingUnit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeBeaconTimingUnit const &', 'arg0')])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): ns3::dot11s::IeBeaconTimingUnit::IeBeaconTimingUnit() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): uint8_t ns3::dot11s::IeBeaconTimingUnit::GetAid() const [member function]
    cls.add_method('GetAid', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-beacon-timing.h (module 'mesh'): uint16_t ns3::dot11s::IeBeaconTimingUnit::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-beacon-timing.h (module 'mesh'): uint16_t ns3::dot11s::IeBeaconTimingUnit::GetLastBeacon() const [member function]
    cls.add_method('GetLastBeacon', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-beacon-timing.h (module 'mesh'): void ns3::dot11s::IeBeaconTimingUnit::SetAid(uint8_t aid) [member function]
    cls.add_method('SetAid', 
                   'void', 
                   [param('uint8_t', 'aid')])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): void ns3::dot11s::IeBeaconTimingUnit::SetBeaconInterval(uint16_t beaconInterval) [member function]
    cls.add_method('SetBeaconInterval', 
                   'void', 
                   [param('uint16_t', 'beaconInterval')])
    ## ie-dot11s-beacon-timing.h (module 'mesh'): void ns3::dot11s::IeBeaconTimingUnit::SetLastBeacon(uint16_t lastBeacon) [member function]
    cls.add_method('SetLastBeacon', 
                   'void', 
                   [param('uint16_t', 'lastBeacon')])
    return

def register_Ns3Dot11sIeConfiguration_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::IeConfiguration::IeConfiguration(ns3::dot11s::IeConfiguration const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeConfiguration const &', 'arg0')])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::IeConfiguration::IeConfiguration() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-configuration.h (module 'mesh'): uint8_t ns3::dot11s::IeConfiguration::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IeConfiguration::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-configuration.h (module 'mesh'): uint8_t ns3::dot11s::IeConfiguration::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-configuration.h (module 'mesh'): uint8_t ns3::dot11s::IeConfiguration::GetNeighborCount() [member function]
    cls.add_method('GetNeighborCount', 
                   'uint8_t', 
                   [])
    ## ie-dot11s-configuration.h (module 'mesh'): bool ns3::dot11s::IeConfiguration::IsAirtime() [member function]
    cls.add_method('IsAirtime', 
                   'bool', 
                   [])
    ## ie-dot11s-configuration.h (module 'mesh'): bool ns3::dot11s::IeConfiguration::IsHWMP() [member function]
    cls.add_method('IsHWMP', 
                   'bool', 
                   [])
    ## ie-dot11s-configuration.h (module 'mesh'): ns3::dot11s::Dot11sMeshCapability const & ns3::dot11s::IeConfiguration::MeshCapability() [member function]
    cls.add_method('MeshCapability', 
                   'ns3::dot11s::Dot11sMeshCapability const &', 
                   [])
    ## ie-dot11s-configuration.h (module 'mesh'): void ns3::dot11s::IeConfiguration::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-configuration.h (module 'mesh'): void ns3::dot11s::IeConfiguration::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-configuration.h (module 'mesh'): void ns3::dot11s::IeConfiguration::SetMetric(ns3::dot11s::dot11sPathSelectionMetric metricId) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('ns3::dot11s::dot11sPathSelectionMetric', 'metricId')])
    ## ie-dot11s-configuration.h (module 'mesh'): void ns3::dot11s::IeConfiguration::SetNeighborCount(uint8_t neighbors) [member function]
    cls.add_method('SetNeighborCount', 
                   'void', 
                   [param('uint8_t', 'neighbors')])
    ## ie-dot11s-configuration.h (module 'mesh'): void ns3::dot11s::IeConfiguration::SetRouting(ns3::dot11s::dot11sPathSelectionProtocol routingId) [member function]
    cls.add_method('SetRouting', 
                   'void', 
                   [param('ns3::dot11s::dot11sPathSelectionProtocol', 'routingId')])
    return

def register_Ns3Dot11sIeLinkMetricReport_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-metric-report.h (module 'mesh'): ns3::dot11s::IeLinkMetricReport::IeLinkMetricReport(ns3::dot11s::IeLinkMetricReport const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeLinkMetricReport const &', 'arg0')])
    ## ie-dot11s-metric-report.h (module 'mesh'): ns3::dot11s::IeLinkMetricReport::IeLinkMetricReport() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-metric-report.h (module 'mesh'): ns3::dot11s::IeLinkMetricReport::IeLinkMetricReport(uint32_t metric) [constructor]
    cls.add_constructor([param('uint32_t', 'metric')])
    ## ie-dot11s-metric-report.h (module 'mesh'): uint8_t ns3::dot11s::IeLinkMetricReport::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-metric-report.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IeLinkMetricReport::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-metric-report.h (module 'mesh'): uint8_t ns3::dot11s::IeLinkMetricReport::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-metric-report.h (module 'mesh'): uint32_t ns3::dot11s::IeLinkMetricReport::GetMetric() [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [])
    ## ie-dot11s-metric-report.h (module 'mesh'): void ns3::dot11s::IeLinkMetricReport::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-metric-report.h (module 'mesh'): void ns3::dot11s::IeLinkMetricReport::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-metric-report.h (module 'mesh'): void ns3::dot11s::IeLinkMetricReport::SetMetric(uint32_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    return

def register_Ns3Dot11sIeMeshId_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshId::IeMeshId(ns3::dot11s::IeMeshId const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeMeshId const &', 'arg0')])
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshId::IeMeshId() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshId::IeMeshId(std::string s) [constructor]
    cls.add_constructor([param('std::string', 's')])
    ## ie-dot11s-id.h (module 'mesh'): uint8_t ns3::dot11s::IeMeshId::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-id.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IeMeshId::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h (module 'mesh'): uint8_t ns3::dot11s::IeMeshId::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h (module 'mesh'): bool ns3::dot11s::IeMeshId::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-id.h (module 'mesh'): bool ns3::dot11s::IeMeshId::IsEqual(ns3::dot11s::IeMeshId const & o) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::dot11s::IeMeshId const &', 'o')], 
                   is_const=True)
    ## ie-dot11s-id.h (module 'mesh'): char * ns3::dot11s::IeMeshId::PeekString() const [member function]
    cls.add_method('PeekString', 
                   'char *', 
                   [], 
                   is_const=True)
    ## ie-dot11s-id.h (module 'mesh'): void ns3::dot11s::IeMeshId::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h (module 'mesh'): void ns3::dot11s::IeMeshId::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Dot11sIeMeshIdChecker_methods(root_module, cls):
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshIdChecker::IeMeshIdChecker() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshIdChecker::IeMeshIdChecker(ns3::dot11s::IeMeshIdChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeMeshIdChecker const &', 'arg0')])
    return

def register_Ns3Dot11sIeMeshIdValue_methods(root_module, cls):
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshIdValue::IeMeshIdValue() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshIdValue::IeMeshIdValue(ns3::dot11s::IeMeshIdValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeMeshIdValue const &', 'arg0')])
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshIdValue::IeMeshIdValue(ns3::dot11s::IeMeshId const & value) [constructor]
    cls.add_constructor([param('ns3::dot11s::IeMeshId const &', 'value')])
    ## ie-dot11s-id.h (module 'mesh'): ns3::Ptr<ns3::AttributeValue> ns3::dot11s::IeMeshIdValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h (module 'mesh'): bool ns3::dot11s::IeMeshIdValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ie-dot11s-id.h (module 'mesh'): ns3::dot11s::IeMeshId ns3::dot11s::IeMeshIdValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::dot11s::IeMeshId', 
                   [], 
                   is_const=True)
    ## ie-dot11s-id.h (module 'mesh'): std::string ns3::dot11s::IeMeshIdValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-id.h (module 'mesh'): void ns3::dot11s::IeMeshIdValue::Set(ns3::dot11s::IeMeshId const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::dot11s::IeMeshId const &', 'value')])
    return

def register_Ns3Dot11sIePeerManagement_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-peer-management.h (module 'mesh'): ns3::dot11s::IePeerManagement::IePeerManagement(ns3::dot11s::IePeerManagement const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePeerManagement const &', 'arg0')])
    ## ie-dot11s-peer-management.h (module 'mesh'): ns3::dot11s::IePeerManagement::IePeerManagement() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-peer-management.h (module 'mesh'): uint8_t ns3::dot11s::IePeerManagement::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IePeerManagement::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): uint8_t ns3::dot11s::IePeerManagement::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): uint16_t ns3::dot11s::IePeerManagement::GetLocalLinkId() const [member function]
    cls.add_method('GetLocalLinkId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): uint16_t ns3::dot11s::IePeerManagement::GetPeerLinkId() const [member function]
    cls.add_method('GetPeerLinkId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): ns3::dot11s::PmpReasonCode ns3::dot11s::IePeerManagement::GetReasonCode() const [member function]
    cls.add_method('GetReasonCode', 
                   'ns3::dot11s::PmpReasonCode', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): uint8_t ns3::dot11s::IePeerManagement::GetSubtype() const [member function]
    cls.add_method('GetSubtype', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): void ns3::dot11s::IePeerManagement::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): void ns3::dot11s::IePeerManagement::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): void ns3::dot11s::IePeerManagement::SetPeerClose(uint16_t localLinkID, uint16_t peerLinkId, ns3::dot11s::PmpReasonCode reasonCode) [member function]
    cls.add_method('SetPeerClose', 
                   'void', 
                   [param('uint16_t', 'localLinkID'), param('uint16_t', 'peerLinkId'), param('ns3::dot11s::PmpReasonCode', 'reasonCode')])
    ## ie-dot11s-peer-management.h (module 'mesh'): void ns3::dot11s::IePeerManagement::SetPeerConfirm(uint16_t localLinkID, uint16_t peerLinkId) [member function]
    cls.add_method('SetPeerConfirm', 
                   'void', 
                   [param('uint16_t', 'localLinkID'), param('uint16_t', 'peerLinkId')])
    ## ie-dot11s-peer-management.h (module 'mesh'): void ns3::dot11s::IePeerManagement::SetPeerOpen(uint16_t localLinkId) [member function]
    cls.add_method('SetPeerOpen', 
                   'void', 
                   [param('uint16_t', 'localLinkId')])
    ## ie-dot11s-peer-management.h (module 'mesh'): bool ns3::dot11s::IePeerManagement::SubtypeIsClose() const [member function]
    cls.add_method('SubtypeIsClose', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): bool ns3::dot11s::IePeerManagement::SubtypeIsConfirm() const [member function]
    cls.add_method('SubtypeIsConfirm', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-peer-management.h (module 'mesh'): bool ns3::dot11s::IePeerManagement::SubtypeIsOpen() const [member function]
    cls.add_method('SubtypeIsOpen', 
                   'bool', 
                   [], 
                   is_const=True)
    return

def register_Ns3Dot11sIePeeringProtocol_methods(root_module, cls):
    ## ie-dot11s-peering-protocol.h (module 'mesh'): ns3::dot11s::IePeeringProtocol::IePeeringProtocol(ns3::dot11s::IePeeringProtocol const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePeeringProtocol const &', 'arg0')])
    ## ie-dot11s-peering-protocol.h (module 'mesh'): ns3::dot11s::IePeeringProtocol::IePeeringProtocol() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-peering-protocol.h (module 'mesh'): uint8_t ns3::dot11s::IePeeringProtocol::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-peering-protocol.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IePeeringProtocol::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peering-protocol.h (module 'mesh'): uint8_t ns3::dot11s::IePeeringProtocol::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peering-protocol.h (module 'mesh'): void ns3::dot11s::IePeeringProtocol::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-peering-protocol.h (module 'mesh'): void ns3::dot11s::IePeeringProtocol::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Dot11sIePerr_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-perr.h (module 'mesh'): ns3::dot11s::IePerr::IePerr(ns3::dot11s::IePerr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePerr const &', 'arg0')])
    ## ie-dot11s-perr.h (module 'mesh'): ns3::dot11s::IePerr::IePerr() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-perr.h (module 'mesh'): void ns3::dot11s::IePerr::AddAddressUnit(ns3::dot11s::HwmpProtocol::FailedDestination unit) [member function]
    cls.add_method('AddAddressUnit', 
                   'void', 
                   [param('ns3::dot11s::HwmpProtocol::FailedDestination', 'unit')])
    ## ie-dot11s-perr.h (module 'mesh'): void ns3::dot11s::IePerr::DeleteAddressUnit(ns3::Mac48Address address) [member function]
    cls.add_method('DeleteAddressUnit', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## ie-dot11s-perr.h (module 'mesh'): uint8_t ns3::dot11s::IePerr::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-perr.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IePerr::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-perr.h (module 'mesh'): std::vector<ns3::dot11s::HwmpProtocol::FailedDestination, std::allocator<ns3::dot11s::HwmpProtocol::FailedDestination> > ns3::dot11s::IePerr::GetAddressUnitVector() const [member function]
    cls.add_method('GetAddressUnitVector', 
                   'std::vector< ns3::dot11s::HwmpProtocol::FailedDestination >', 
                   [], 
                   is_const=True)
    ## ie-dot11s-perr.h (module 'mesh'): uint8_t ns3::dot11s::IePerr::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-perr.h (module 'mesh'): uint8_t ns3::dot11s::IePerr::GetNumOfDest() const [member function]
    cls.add_method('GetNumOfDest', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-perr.h (module 'mesh'): bool ns3::dot11s::IePerr::IsFull() const [member function]
    cls.add_method('IsFull', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-perr.h (module 'mesh'): void ns3::dot11s::IePerr::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-perr.h (module 'mesh'): void ns3::dot11s::IePerr::ResetPerr() [member function]
    cls.add_method('ResetPerr', 
                   'void', 
                   [])
    ## ie-dot11s-perr.h (module 'mesh'): void ns3::dot11s::IePerr::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Dot11sIePrep_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-prep.h (module 'mesh'): ns3::dot11s::IePrep::IePrep(ns3::dot11s::IePrep const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePrep const &', 'arg0')])
    ## ie-dot11s-prep.h (module 'mesh'): ns3::dot11s::IePrep::IePrep() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::DecrementTtl() [member function]
    cls.add_method('DecrementTtl', 
                   'void', 
                   [])
    ## ie-dot11s-prep.h (module 'mesh'): uint8_t ns3::dot11s::IePrep::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-prep.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IePrep::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-prep.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::IePrep::GetDestinationAddress() const [member function]
    cls.add_method('GetDestinationAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): uint32_t ns3::dot11s::IePrep::GetDestinationSeqNumber() const [member function]
    cls.add_method('GetDestinationSeqNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): uint8_t ns3::dot11s::IePrep::GetFlags() const [member function]
    cls.add_method('GetFlags', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): uint8_t ns3::dot11s::IePrep::GetHopcount() const [member function]
    cls.add_method('GetHopcount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): uint8_t ns3::dot11s::IePrep::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-prep.h (module 'mesh'): uint32_t ns3::dot11s::IePrep::GetLifetime() const [member function]
    cls.add_method('GetLifetime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): uint32_t ns3::dot11s::IePrep::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::IePrep::GetOriginatorAddress() const [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): uint32_t ns3::dot11s::IePrep::GetOriginatorSeqNumber() const [member function]
    cls.add_method('GetOriginatorSeqNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): uint32_t ns3::dot11s::IePrep::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::IncrementMetric(uint32_t metric) [member function]
    cls.add_method('IncrementMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetDestinationAddress(ns3::Mac48Address dest_address) [member function]
    cls.add_method('SetDestinationAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'dest_address')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetDestinationSeqNumber(uint32_t dest_seq_number) [member function]
    cls.add_method('SetDestinationSeqNumber', 
                   'void', 
                   [param('uint32_t', 'dest_seq_number')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetFlags(uint8_t flags) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetHopcount(uint8_t hopcount) [member function]
    cls.add_method('SetHopcount', 
                   'void', 
                   [param('uint8_t', 'hopcount')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetLifetime(uint32_t lifetime) [member function]
    cls.add_method('SetLifetime', 
                   'void', 
                   [param('uint32_t', 'lifetime')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetMetric(uint32_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetOriginatorAddress(ns3::Mac48Address originator_address) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'originator_address')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetOriginatorSeqNumber(uint32_t originator_seq_number) [member function]
    cls.add_method('SetOriginatorSeqNumber', 
                   'void', 
                   [param('uint32_t', 'originator_seq_number')])
    ## ie-dot11s-prep.h (module 'mesh'): void ns3::dot11s::IePrep::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3Dot11sIePreq_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-preq.h (module 'mesh'): ns3::dot11s::IePreq::IePreq(ns3::dot11s::IePreq const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IePreq const &', 'arg0')])
    ## ie-dot11s-preq.h (module 'mesh'): ns3::dot11s::IePreq::IePreq() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::AddDestinationAddressElement(bool doFlag, bool rfFlag, ns3::Mac48Address dest_address, uint32_t dest_seq_number) [member function]
    cls.add_method('AddDestinationAddressElement', 
                   'void', 
                   [param('bool', 'doFlag'), param('bool', 'rfFlag'), param('ns3::Mac48Address', 'dest_address'), param('uint32_t', 'dest_seq_number')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::ClearDestinationAddressElements() [member function]
    cls.add_method('ClearDestinationAddressElements', 
                   'void', 
                   [])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::DecrementTtl() [member function]
    cls.add_method('DecrementTtl', 
                   'void', 
                   [])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::DelDestinationAddressElement(ns3::Mac48Address dest_address) [member function]
    cls.add_method('DelDestinationAddressElement', 
                   'void', 
                   [param('ns3::Mac48Address', 'dest_address')])
    ## ie-dot11s-preq.h (module 'mesh'): uint8_t ns3::dot11s::IePreq::DeserializeInformationField(ns3::Buffer::Iterator i, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'i'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-preq.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IePreq::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-preq.h (module 'mesh'): uint8_t ns3::dot11s::IePreq::GetDestCount() const [member function]
    cls.add_method('GetDestCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): std::vector<ns3::Ptr<ns3::dot11s::DestinationAddressUnit>, std::allocator<ns3::Ptr<ns3::dot11s::DestinationAddressUnit> > > ns3::dot11s::IePreq::GetDestinationList() [member function]
    cls.add_method('GetDestinationList', 
                   'std::vector< ns3::Ptr< ns3::dot11s::DestinationAddressUnit > >', 
                   [])
    ## ie-dot11s-preq.h (module 'mesh'): uint8_t ns3::dot11s::IePreq::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): uint8_t ns3::dot11s::IePreq::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-preq.h (module 'mesh'): uint32_t ns3::dot11s::IePreq::GetLifetime() const [member function]
    cls.add_method('GetLifetime', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): uint32_t ns3::dot11s::IePreq::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::IePreq::GetOriginatorAddress() const [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): uint32_t ns3::dot11s::IePreq::GetOriginatorSeqNumber() const [member function]
    cls.add_method('GetOriginatorSeqNumber', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): uint32_t ns3::dot11s::IePreq::GetPreqID() const [member function]
    cls.add_method('GetPreqID', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): uint8_t ns3::dot11s::IePreq::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::IncrementMetric(uint32_t metric) [member function]
    cls.add_method('IncrementMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-preq.h (module 'mesh'): bool ns3::dot11s::IePreq::IsFull() const [member function]
    cls.add_method('IsFull', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): bool ns3::dot11s::IePreq::IsNeedNotPrep() const [member function]
    cls.add_method('IsNeedNotPrep', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): bool ns3::dot11s::IePreq::IsUnicastPreq() const [member function]
    cls.add_method('IsUnicastPreq', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ie-dot11s-preq.h (module 'mesh'): bool ns3::dot11s::IePreq::MayAddAddress(ns3::Mac48Address originator) [member function]
    cls.add_method('MayAddAddress', 
                   'bool', 
                   [param('ns3::Mac48Address', 'originator')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetDestCount(uint8_t dest_count) [member function]
    cls.add_method('SetDestCount', 
                   'void', 
                   [param('uint8_t', 'dest_count')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetHopcount(uint8_t hopcount) [member function]
    cls.add_method('SetHopcount', 
                   'void', 
                   [param('uint8_t', 'hopcount')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetLifetime(uint32_t lifetime) [member function]
    cls.add_method('SetLifetime', 
                   'void', 
                   [param('uint32_t', 'lifetime')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetMetric(uint32_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetNeedNotPrep() [member function]
    cls.add_method('SetNeedNotPrep', 
                   'void', 
                   [])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetOriginatorAddress(ns3::Mac48Address originator_address) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'originator_address')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetOriginatorSeqNumber(uint32_t originator_seq_number) [member function]
    cls.add_method('SetOriginatorSeqNumber', 
                   'void', 
                   [param('uint32_t', 'originator_seq_number')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetPreqID(uint32_t id) [member function]
    cls.add_method('SetPreqID', 
                   'void', 
                   [param('uint32_t', 'id')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetTTL(uint8_t ttl) [member function]
    cls.add_method('SetTTL', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    ## ie-dot11s-preq.h (module 'mesh'): void ns3::dot11s::IePreq::SetUnicastPreq() [member function]
    cls.add_method('SetUnicastPreq', 
                   'void', 
                   [])
    return

def register_Ns3Dot11sIeRann_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ie-dot11s-rann.h (module 'mesh'): ns3::dot11s::IeRann::IeRann(ns3::dot11s::IeRann const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::IeRann const &', 'arg0')])
    ## ie-dot11s-rann.h (module 'mesh'): ns3::dot11s::IeRann::IeRann() [constructor]
    cls.add_constructor([])
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::DecrementTtl() [member function]
    cls.add_method('DecrementTtl', 
                   'void', 
                   [])
    ## ie-dot11s-rann.h (module 'mesh'): uint8_t ns3::dot11s::IeRann::DeserializeInformationField(ns3::Buffer::Iterator start, uint8_t length) [member function]
    cls.add_method('DeserializeInformationField', 
                   'uint8_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint8_t', 'length')], 
                   is_virtual=True)
    ## ie-dot11s-rann.h (module 'mesh'): ns3::WifiInformationElementId ns3::dot11s::IeRann::ElementId() const [member function]
    cls.add_method('ElementId', 
                   'ns3::WifiInformationElementId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-rann.h (module 'mesh'): uint32_t ns3::dot11s::IeRann::GetDestSeqNumber() [member function]
    cls.add_method('GetDestSeqNumber', 
                   'uint32_t', 
                   [])
    ## ie-dot11s-rann.h (module 'mesh'): uint8_t ns3::dot11s::IeRann::GetFlags() [member function]
    cls.add_method('GetFlags', 
                   'uint8_t', 
                   [])
    ## ie-dot11s-rann.h (module 'mesh'): uint8_t ns3::dot11s::IeRann::GetHopcount() [member function]
    cls.add_method('GetHopcount', 
                   'uint8_t', 
                   [])
    ## ie-dot11s-rann.h (module 'mesh'): uint8_t ns3::dot11s::IeRann::GetInformationFieldSize() const [member function]
    cls.add_method('GetInformationFieldSize', 
                   'uint8_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-rann.h (module 'mesh'): uint32_t ns3::dot11s::IeRann::GetMetric() [member function]
    cls.add_method('GetMetric', 
                   'uint32_t', 
                   [])
    ## ie-dot11s-rann.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::IeRann::GetOriginatorAddress() [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Mac48Address', 
                   [])
    ## ie-dot11s-rann.h (module 'mesh'): uint8_t ns3::dot11s::IeRann::GetTtl() [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [])
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::IncrementMetric(uint32_t metric) [member function]
    cls.add_method('IncrementMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::SerializeInformationField(ns3::Buffer::Iterator i) const [member function]
    cls.add_method('SerializeInformationField', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'i')], 
                   is_const=True, is_virtual=True)
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::SetDestSeqNumber(uint32_t dest_seq_number) [member function]
    cls.add_method('SetDestSeqNumber', 
                   'void', 
                   [param('uint32_t', 'dest_seq_number')])
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::SetFlags(uint8_t flags) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::SetHopcount(uint8_t hopcount) [member function]
    cls.add_method('SetHopcount', 
                   'void', 
                   [param('uint8_t', 'hopcount')])
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::SetMetric(uint32_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint32_t', 'metric')])
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::SetOriginatorAddress(ns3::Mac48Address originator_address) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'originator_address')])
    ## ie-dot11s-rann.h (module 'mesh'): void ns3::dot11s::IeRann::SetTTL(uint8_t ttl) [member function]
    cls.add_method('SetTTL', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3Dot11sMeshHeader_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## dot11s-mac-header.h (module 'mesh'): ns3::dot11s::MeshHeader::MeshHeader(ns3::dot11s::MeshHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::MeshHeader const &', 'arg0')])
    ## dot11s-mac-header.h (module 'mesh'): ns3::dot11s::MeshHeader::MeshHeader() [constructor]
    cls.add_constructor([])
    ## dot11s-mac-header.h (module 'mesh'): uint32_t ns3::dot11s::MeshHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## dot11s-mac-header.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::MeshHeader::GetAddr4() const [member function]
    cls.add_method('GetAddr4', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::MeshHeader::GetAddr5() const [member function]
    cls.add_method('GetAddr5', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::MeshHeader::GetAddr6() const [member function]
    cls.add_method('GetAddr6', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h (module 'mesh'): uint8_t ns3::dot11s::MeshHeader::GetAddressExt() const [member function]
    cls.add_method('GetAddressExt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h (module 'mesh'): ns3::TypeId ns3::dot11s::MeshHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dot11s-mac-header.h (module 'mesh'): uint32_t ns3::dot11s::MeshHeader::GetMeshSeqno() const [member function]
    cls.add_method('GetMeshSeqno', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h (module 'mesh'): uint8_t ns3::dot11s::MeshHeader::GetMeshTtl() const [member function]
    cls.add_method('GetMeshTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## dot11s-mac-header.h (module 'mesh'): uint32_t ns3::dot11s::MeshHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## dot11s-mac-header.h (module 'mesh'): static ns3::TypeId ns3::dot11s::MeshHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## dot11s-mac-header.h (module 'mesh'): void ns3::dot11s::MeshHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## dot11s-mac-header.h (module 'mesh'): void ns3::dot11s::MeshHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## dot11s-mac-header.h (module 'mesh'): void ns3::dot11s::MeshHeader::SetAddr4(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr4', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## dot11s-mac-header.h (module 'mesh'): void ns3::dot11s::MeshHeader::SetAddr5(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr5', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## dot11s-mac-header.h (module 'mesh'): void ns3::dot11s::MeshHeader::SetAddr6(ns3::Mac48Address address) [member function]
    cls.add_method('SetAddr6', 
                   'void', 
                   [param('ns3::Mac48Address', 'address')])
    ## dot11s-mac-header.h (module 'mesh'): void ns3::dot11s::MeshHeader::SetAddressExt(uint8_t num_of_addresses) [member function]
    cls.add_method('SetAddressExt', 
                   'void', 
                   [param('uint8_t', 'num_of_addresses')])
    ## dot11s-mac-header.h (module 'mesh'): void ns3::dot11s::MeshHeader::SetMeshSeqno(uint32_t seqno) [member function]
    cls.add_method('SetMeshSeqno', 
                   'void', 
                   [param('uint32_t', 'seqno')])
    ## dot11s-mac-header.h (module 'mesh'): void ns3::dot11s::MeshHeader::SetMeshTtl(uint8_t TTL) [member function]
    cls.add_method('SetMeshTtl', 
                   'void', 
                   [param('uint8_t', 'TTL')])
    return

def register_Ns3Dot11sPeerLink_methods(root_module, cls):
    ## peer-link.h (module 'mesh'): static ns3::TypeId ns3::dot11s::PeerLink::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## peer-link.h (module 'mesh'): ns3::dot11s::PeerLink::PeerLink() [constructor]
    cls.add_constructor([])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::SetBeaconInformation(ns3::Time lastBeacon, ns3::Time BeaconInterval) [member function]
    cls.add_method('SetBeaconInformation', 
                   'void', 
                   [param('ns3::Time', 'lastBeacon'), param('ns3::Time', 'BeaconInterval')])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::SetLinkStatusCallback(ns3::Callback<void,unsigned int,ns3::Mac48Address,bool,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> cb) [member function]
    cls.add_method('SetLinkStatusCallback', 
                   'void', 
                   [param('ns3::Callback< void, unsigned int, ns3::Mac48Address, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::SetPeerAddress(ns3::Mac48Address macaddr) [member function]
    cls.add_method('SetPeerAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'macaddr')])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::SetPeerMeshPointAddress(ns3::Mac48Address macaddr) [member function]
    cls.add_method('SetPeerMeshPointAddress', 
                   'void', 
                   [param('ns3::Mac48Address', 'macaddr')])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::SetInterface(uint32_t interface) [member function]
    cls.add_method('SetInterface', 
                   'void', 
                   [param('uint32_t', 'interface')])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::SetLocalLinkId(uint16_t id) [member function]
    cls.add_method('SetLocalLinkId', 
                   'void', 
                   [param('uint16_t', 'id')])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::SetLocalAid(uint16_t aid) [member function]
    cls.add_method('SetLocalAid', 
                   'void', 
                   [param('uint16_t', 'aid')])
    ## peer-link.h (module 'mesh'): uint16_t ns3::dot11s::PeerLink::GetPeerAid() const [member function]
    cls.add_method('GetPeerAid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::SetBeaconTimingElement(ns3::dot11s::IeBeaconTiming beaconTiming) [member function]
    cls.add_method('SetBeaconTimingElement', 
                   'void', 
                   [param('ns3::dot11s::IeBeaconTiming', 'beaconTiming')])
    ## peer-link.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::PeerLink::GetPeerAddress() const [member function]
    cls.add_method('GetPeerAddress', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## peer-link.h (module 'mesh'): uint16_t ns3::dot11s::PeerLink::GetLocalAid() const [member function]
    cls.add_method('GetLocalAid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## peer-link.h (module 'mesh'): ns3::Time ns3::dot11s::PeerLink::GetLastBeacon() const [member function]
    cls.add_method('GetLastBeacon', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## peer-link.h (module 'mesh'): ns3::Time ns3::dot11s::PeerLink::GetBeaconInterval() const [member function]
    cls.add_method('GetBeaconInterval', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## peer-link.h (module 'mesh'): ns3::dot11s::IeBeaconTiming ns3::dot11s::PeerLink::GetBeaconTimingElement() const [member function]
    cls.add_method('GetBeaconTimingElement', 
                   'ns3::dot11s::IeBeaconTiming', 
                   [], 
                   is_const=True)
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::MLMECancelPeerLink(ns3::dot11s::PmpReasonCode reason) [member function]
    cls.add_method('MLMECancelPeerLink', 
                   'void', 
                   [param('ns3::dot11s::PmpReasonCode', 'reason')])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::MLMEActivePeerLinkOpen() [member function]
    cls.add_method('MLMEActivePeerLinkOpen', 
                   'void', 
                   [])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::MLMEPeeringRequestReject() [member function]
    cls.add_method('MLMEPeeringRequestReject', 
                   'void', 
                   [])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::MLMESetSignalStatusCallback(ns3::Callback<void, unsigned int, ns3::Mac48Address, ns3::Mac48Address, ns3::dot11s::PeerLink::PeerState, ns3::dot11s::PeerLink::PeerState, ns3::empty, ns3::empty, ns3::empty, ns3::empty> arg0) [member function]
    cls.add_method('MLMESetSignalStatusCallback', 
                   'void', 
                   [param('ns3::Callback< void, unsigned int, ns3::Mac48Address, ns3::Mac48Address, ns3::dot11s::PeerLink::PeerState, ns3::dot11s::PeerLink::PeerState, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'arg0')])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::TransmissionSuccess() [member function]
    cls.add_method('TransmissionSuccess', 
                   'void', 
                   [])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::TransmissionFailure() [member function]
    cls.add_method('TransmissionFailure', 
                   'void', 
                   [])
    ## peer-link.h (module 'mesh'): void ns3::dot11s::PeerLink::Report(std::ostream & os) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3Dot11sPeerLinkCloseStart_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkCloseStart::PeerLinkCloseStart() [constructor]
    cls.add_constructor([])
    ## peer-link-frame.h (module 'mesh'): uint32_t ns3::dot11s::PeerLinkCloseStart::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields ns3::dot11s::PeerLinkCloseStart::GetFields() const [member function]
    cls.add_method('GetFields', 
                   'ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields', 
                   [], 
                   is_const=True)
    ## peer-link-frame.h (module 'mesh'): ns3::TypeId ns3::dot11s::PeerLinkCloseStart::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): uint32_t ns3::dot11s::PeerLinkCloseStart::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): static ns3::TypeId ns3::dot11s::PeerLinkCloseStart::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkCloseStart::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkCloseStart::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkCloseStart::SetPlinkCloseStart(ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields arg0) [member function]
    cls.add_method('SetPlinkCloseStart', 
                   'void', 
                   [param('ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields', 'arg0')])
    return

def register_Ns3Dot11sPeerLinkCloseStartPlinkCloseStartFields_methods(root_module, cls):
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields::PlinkCloseStartFields() [constructor]
    cls.add_constructor([])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields::PlinkCloseStartFields(ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields const &', 'arg0')])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields::meshId [variable]
    cls.add_instance_attribute('meshId', 'ns3::dot11s::IeMeshId', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkCloseStart::PlinkCloseStartFields::protocol [variable]
    cls.add_instance_attribute('protocol', 'ns3::dot11s::IePeeringProtocol', is_const=False)
    return

def register_Ns3Dot11sPeerLinkConfirmStart_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PeerLinkConfirmStart() [constructor]
    cls.add_constructor([])
    ## peer-link-frame.h (module 'mesh'): uint32_t ns3::dot11s::PeerLinkConfirmStart::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields ns3::dot11s::PeerLinkConfirmStart::GetFields() const [member function]
    cls.add_method('GetFields', 
                   'ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields', 
                   [], 
                   is_const=True)
    ## peer-link-frame.h (module 'mesh'): ns3::TypeId ns3::dot11s::PeerLinkConfirmStart::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): uint32_t ns3::dot11s::PeerLinkConfirmStart::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): static ns3::TypeId ns3::dot11s::PeerLinkConfirmStart::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkConfirmStart::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkConfirmStart::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkConfirmStart::SetPlinkConfirmStart(ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields arg0) [member function]
    cls.add_method('SetPlinkConfirmStart', 
                   'void', 
                   [param('ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields', 'arg0')])
    return

def register_Ns3Dot11sPeerLinkConfirmStartPlinkConfirmStartFields_methods(root_module, cls):
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields::PlinkConfirmStartFields() [constructor]
    cls.add_constructor([])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields::PlinkConfirmStartFields(ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields const &', 'arg0')])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields::aid [variable]
    cls.add_instance_attribute('aid', 'uint16_t', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields::capability [variable]
    cls.add_instance_attribute('capability', 'uint16_t', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields::config [variable]
    cls.add_instance_attribute('config', 'ns3::dot11s::IeConfiguration', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields::protocol [variable]
    cls.add_instance_attribute('protocol', 'ns3::dot11s::IePeeringProtocol', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkConfirmStart::PlinkConfirmStartFields::rates [variable]
    cls.add_instance_attribute('rates', 'ns3::SupportedRates', is_const=False)
    return

def register_Ns3Dot11sPeerLinkOpenStart_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PeerLinkOpenStart() [constructor]
    cls.add_constructor([])
    ## peer-link-frame.h (module 'mesh'): uint32_t ns3::dot11s::PeerLinkOpenStart::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields ns3::dot11s::PeerLinkOpenStart::GetFields() const [member function]
    cls.add_method('GetFields', 
                   'ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields', 
                   [], 
                   is_const=True)
    ## peer-link-frame.h (module 'mesh'): ns3::TypeId ns3::dot11s::PeerLinkOpenStart::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): uint32_t ns3::dot11s::PeerLinkOpenStart::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): static ns3::TypeId ns3::dot11s::PeerLinkOpenStart::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkOpenStart::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkOpenStart::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## peer-link-frame.h (module 'mesh'): void ns3::dot11s::PeerLinkOpenStart::SetPlinkOpenStart(ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields arg0) [member function]
    cls.add_method('SetPlinkOpenStart', 
                   'void', 
                   [param('ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields', 'arg0')])
    return

def register_Ns3Dot11sPeerLinkOpenStartPlinkOpenStartFields_methods(root_module, cls):
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields::PlinkOpenStartFields() [constructor]
    cls.add_constructor([])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields::PlinkOpenStartFields(ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields const &', 'arg0')])
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields::capability [variable]
    cls.add_instance_attribute('capability', 'uint16_t', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields::config [variable]
    cls.add_instance_attribute('config', 'ns3::dot11s::IeConfiguration', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields::meshId [variable]
    cls.add_instance_attribute('meshId', 'ns3::dot11s::IeMeshId', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields::protocol [variable]
    cls.add_instance_attribute('protocol', 'ns3::dot11s::IePeeringProtocol', is_const=False)
    ## peer-link-frame.h (module 'mesh'): ns3::dot11s::PeerLinkOpenStart::PlinkOpenStartFields::rates [variable]
    cls.add_instance_attribute('rates', 'ns3::SupportedRates', is_const=False)
    return

def register_Ns3Dot11sPeerManagementProtocol_methods(root_module, cls):
    ## peer-management-protocol.h (module 'mesh'): ns3::dot11s::PeerManagementProtocol::PeerManagementProtocol() [constructor]
    cls.add_constructor([])
    ## peer-management-protocol.h (module 'mesh'): int64_t ns3::dot11s::PeerManagementProtocol::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::ConfigurationMismatch(uint32_t interface, ns3::Mac48Address peerAddress) [member function]
    cls.add_method('ConfigurationMismatch', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## peer-management-protocol.h (module 'mesh'): ns3::Ptr<ns3::dot11s::PeerLink> ns3::dot11s::PeerManagementProtocol::FindPeerLink(uint32_t interface, ns3::Mac48Address peerAddress) [member function]
    cls.add_method('FindPeerLink', 
                   'ns3::Ptr< ns3::dot11s::PeerLink >', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress')])
    ## peer-management-protocol.h (module 'mesh'): ns3::Mac48Address ns3::dot11s::PeerManagementProtocol::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [])
    ## peer-management-protocol.h (module 'mesh'): bool ns3::dot11s::PeerManagementProtocol::GetBeaconCollisionAvoidance() const [member function]
    cls.add_method('GetBeaconCollisionAvoidance', 
                   'bool', 
                   [], 
                   is_const=True)
    ## peer-management-protocol.h (module 'mesh'): ns3::Ptr<ns3::dot11s::IeBeaconTiming> ns3::dot11s::PeerManagementProtocol::GetBeaconTimingElement(uint32_t interface) [member function]
    cls.add_method('GetBeaconTimingElement', 
                   'ns3::Ptr< ns3::dot11s::IeBeaconTiming >', 
                   [param('uint32_t', 'interface')])
    ## peer-management-protocol.h (module 'mesh'): ns3::Ptr<ns3::dot11s::IeMeshId> ns3::dot11s::PeerManagementProtocol::GetMeshId() const [member function]
    cls.add_method('GetMeshId', 
                   'ns3::Ptr< ns3::dot11s::IeMeshId >', 
                   [], 
                   is_const=True)
    ## peer-management-protocol.h (module 'mesh'): uint8_t ns3::dot11s::PeerManagementProtocol::GetNumberOfLinks() [member function]
    cls.add_method('GetNumberOfLinks', 
                   'uint8_t', 
                   [])
    ## peer-management-protocol.h (module 'mesh'): std::vector<ns3::Ptr<ns3::dot11s::PeerLink>,std::allocator<ns3::Ptr<ns3::dot11s::PeerLink> > > ns3::dot11s::PeerManagementProtocol::GetPeerLinks() const [member function]
    cls.add_method('GetPeerLinks', 
                   'std::vector< ns3::Ptr< ns3::dot11s::PeerLink > >', 
                   [], 
                   is_const=True)
    ## peer-management-protocol.h (module 'mesh'): std::vector<ns3::Mac48Address,std::allocator<ns3::Mac48Address> > ns3::dot11s::PeerManagementProtocol::GetPeers(uint32_t interface) const [member function]
    cls.add_method('GetPeers', 
                   'std::vector< ns3::Mac48Address >', 
                   [param('uint32_t', 'interface')], 
                   is_const=True)
    ## peer-management-protocol.h (module 'mesh'): static ns3::TypeId ns3::dot11s::PeerManagementProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## peer-management-protocol.h (module 'mesh'): bool ns3::dot11s::PeerManagementProtocol::Install(ns3::Ptr<ns3::MeshPointDevice> arg0) [member function]
    cls.add_method('Install', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'arg0')])
    ## peer-management-protocol.h (module 'mesh'): bool ns3::dot11s::PeerManagementProtocol::IsActiveLink(uint32_t interface, ns3::Mac48Address peerAddress) [member function]
    cls.add_method('IsActiveLink', 
                   'bool', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::NotifyBeaconSent(uint32_t interface, ns3::Time beaconInterval) [member function]
    cls.add_method('NotifyBeaconSent', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Time', 'beaconInterval')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::ReceiveBeacon(uint32_t interface, ns3::Mac48Address peerAddress, ns3::Time beaconInterval, ns3::Ptr<ns3::dot11s::IeBeaconTiming> beaconTiming) [member function]
    cls.add_method('ReceiveBeacon', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress'), param('ns3::Time', 'beaconInterval'), param('ns3::Ptr< ns3::dot11s::IeBeaconTiming >', 'beaconTiming')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::ReceivePeerLinkFrame(uint32_t interface, ns3::Mac48Address peerAddress, ns3::Mac48Address peerMeshPointAddress, uint16_t aid, ns3::dot11s::IePeerManagement peerManagementElement, ns3::dot11s::IeConfiguration meshConfig) [member function]
    cls.add_method('ReceivePeerLinkFrame', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address', 'peerAddress'), param('ns3::Mac48Address', 'peerMeshPointAddress'), param('uint16_t', 'aid'), param('ns3::dot11s::IePeerManagement', 'peerManagementElement'), param('ns3::dot11s::IeConfiguration', 'meshConfig')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::SetBeaconCollisionAvoidance(bool enable) [member function]
    cls.add_method('SetBeaconCollisionAvoidance', 
                   'void', 
                   [param('bool', 'enable')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::SetMeshId(std::string s) [member function]
    cls.add_method('SetMeshId', 
                   'void', 
                   [param('std::string', 's')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::SetPeerLinkStatusCallback(ns3::Callback<void, ns3::Mac48Address, ns3::Mac48Address, unsigned int, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPeerLinkStatusCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Mac48Address, ns3::Mac48Address, unsigned int, bool, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::TransmissionFailure(uint32_t interface, ns3::Mac48Address const peerAddress) [member function]
    cls.add_method('TransmissionFailure', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address const', 'peerAddress')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::TransmissionSuccess(uint32_t interface, ns3::Mac48Address const peerAddress) [member function]
    cls.add_method('TransmissionSuccess', 
                   'void', 
                   [param('uint32_t', 'interface'), param('ns3::Mac48Address const', 'peerAddress')])
    ## peer-management-protocol.h (module 'mesh'): void ns3::dot11s::PeerManagementProtocol::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3FlameFlameHeader_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## flame-header.h (module 'mesh'): ns3::flame::FlameHeader::FlameHeader(ns3::flame::FlameHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameHeader const &', 'arg0')])
    ## flame-header.h (module 'mesh'): ns3::flame::FlameHeader::FlameHeader() [constructor]
    cls.add_constructor([])
    ## flame-header.h (module 'mesh'): void ns3::flame::FlameHeader::AddCost(uint8_t cost) [member function]
    cls.add_method('AddCost', 
                   'void', 
                   [param('uint8_t', 'cost')])
    ## flame-header.h (module 'mesh'): uint32_t ns3::flame::FlameHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## flame-header.h (module 'mesh'): uint8_t ns3::flame::FlameHeader::GetCost() const [member function]
    cls.add_method('GetCost', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## flame-header.h (module 'mesh'): ns3::TypeId ns3::flame::FlameHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-header.h (module 'mesh'): ns3::Mac48Address ns3::flame::FlameHeader::GetOrigDst() const [member function]
    cls.add_method('GetOrigDst', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## flame-header.h (module 'mesh'): ns3::Mac48Address ns3::flame::FlameHeader::GetOrigSrc() const [member function]
    cls.add_method('GetOrigSrc', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## flame-header.h (module 'mesh'): uint16_t ns3::flame::FlameHeader::GetProtocol() const [member function]
    cls.add_method('GetProtocol', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-header.h (module 'mesh'): uint16_t ns3::flame::FlameHeader::GetSeqno() const [member function]
    cls.add_method('GetSeqno', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-header.h (module 'mesh'): uint32_t ns3::flame::FlameHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-header.h (module 'mesh'): static ns3::TypeId ns3::flame::FlameHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-header.h (module 'mesh'): void ns3::flame::FlameHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## flame-header.h (module 'mesh'): void ns3::flame::FlameHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## flame-header.h (module 'mesh'): void ns3::flame::FlameHeader::SetOrigDst(ns3::Mac48Address dst) [member function]
    cls.add_method('SetOrigDst', 
                   'void', 
                   [param('ns3::Mac48Address', 'dst')])
    ## flame-header.h (module 'mesh'): void ns3::flame::FlameHeader::SetOrigSrc(ns3::Mac48Address OrigSrc) [member function]
    cls.add_method('SetOrigSrc', 
                   'void', 
                   [param('ns3::Mac48Address', 'OrigSrc')])
    ## flame-header.h (module 'mesh'): void ns3::flame::FlameHeader::SetProtocol(uint16_t protocol) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint16_t', 'protocol')])
    ## flame-header.h (module 'mesh'): void ns3::flame::FlameHeader::SetSeqno(uint16_t seqno) [member function]
    cls.add_method('SetSeqno', 
                   'void', 
                   [param('uint16_t', 'seqno')])
    return

def register_Ns3FlameFlameProtocol_methods(root_module, cls):
    ## flame-protocol.h (module 'mesh'): ns3::flame::FlameProtocol::FlameProtocol() [constructor]
    cls.add_constructor([])
    ## flame-protocol.h (module 'mesh'): void ns3::flame::FlameProtocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-protocol.h (module 'mesh'): ns3::Mac48Address ns3::flame::FlameProtocol::GetAddress() [member function]
    cls.add_method('GetAddress', 
                   'ns3::Mac48Address', 
                   [])
    ## flame-protocol.h (module 'mesh'): static ns3::TypeId ns3::flame::FlameProtocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-protocol.h (module 'mesh'): bool ns3::flame::FlameProtocol::Install(ns3::Ptr<ns3::MeshPointDevice> arg0) [member function]
    cls.add_method('Install', 
                   'bool', 
                   [param('ns3::Ptr< ns3::MeshPointDevice >', 'arg0')])
    ## flame-protocol.h (module 'mesh'): bool ns3::flame::FlameProtocol::RemoveRoutingStuff(uint32_t fromIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet> packet, uint16_t & protocolType) [member function]
    cls.add_method('RemoveRoutingStuff', 
                   'bool', 
                   [param('uint32_t', 'fromIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t &', 'protocolType')], 
                   is_virtual=True)
    ## flame-protocol.h (module 'mesh'): void ns3::flame::FlameProtocol::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## flame-protocol.h (module 'mesh'): bool ns3::flame::FlameProtocol::RequestRoute(uint32_t sourceIface, ns3::Mac48Address const source, ns3::Mac48Address const destination, ns3::Ptr<ns3::Packet const> packet, uint16_t protocolType, ns3::Callback<void, bool, ns3::Ptr<ns3::Packet>, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty> routeReply) [member function]
    cls.add_method('RequestRoute', 
                   'bool', 
                   [param('uint32_t', 'sourceIface'), param('ns3::Mac48Address const', 'source'), param('ns3::Mac48Address const', 'destination'), param('ns3::Ptr< ns3::Packet const >', 'packet'), param('uint16_t', 'protocolType'), param('ns3::Callback< void, bool, ns3::Ptr< ns3::Packet >, ns3::Mac48Address, ns3::Mac48Address, unsigned short, unsigned int, ns3::empty, ns3::empty, ns3::empty >', 'routeReply')], 
                   is_virtual=True)
    ## flame-protocol.h (module 'mesh'): void ns3::flame::FlameProtocol::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    return

def register_Ns3FlameFlameProtocolMac_methods(root_module, cls):
    ## flame-protocol-mac.h (module 'mesh'): ns3::flame::FlameProtocolMac::FlameProtocolMac(ns3::flame::FlameProtocolMac const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameProtocolMac const &', 'arg0')])
    ## flame-protocol-mac.h (module 'mesh'): ns3::flame::FlameProtocolMac::FlameProtocolMac(ns3::Ptr<ns3::flame::FlameProtocol> arg0) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::flame::FlameProtocol >', 'arg0')])
    ## flame-protocol-mac.h (module 'mesh'): int64_t ns3::flame::FlameProtocolMac::AssignStreams(int64_t stream) [member function]
    cls.add_method('AssignStreams', 
                   'int64_t', 
                   [param('int64_t', 'stream')], 
                   is_virtual=True)
    ## flame-protocol-mac.h (module 'mesh'): uint16_t ns3::flame::FlameProtocolMac::GetChannelId() const [member function]
    cls.add_method('GetChannelId', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## flame-protocol-mac.h (module 'mesh'): bool ns3::flame::FlameProtocolMac::Receive(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader const & header) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader const &', 'header')], 
                   is_virtual=True)
    ## flame-protocol-mac.h (module 'mesh'): void ns3::flame::FlameProtocolMac::Report(std::ostream & arg0) const [member function]
    cls.add_method('Report', 
                   'void', 
                   [param('std::ostream &', 'arg0')], 
                   is_const=True)
    ## flame-protocol-mac.h (module 'mesh'): void ns3::flame::FlameProtocolMac::ResetStats() [member function]
    cls.add_method('ResetStats', 
                   'void', 
                   [])
    ## flame-protocol-mac.h (module 'mesh'): void ns3::flame::FlameProtocolMac::SetParent(ns3::Ptr<ns3::MeshWifiInterfaceMac> parent) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('ns3::Ptr< ns3::MeshWifiInterfaceMac >', 'parent')], 
                   is_virtual=True)
    ## flame-protocol-mac.h (module 'mesh'): void ns3::flame::FlameProtocolMac::UpdateBeacon(ns3::MeshWifiBeacon & beacon) const [member function]
    cls.add_method('UpdateBeacon', 
                   'void', 
                   [param('ns3::MeshWifiBeacon &', 'beacon')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol-mac.h (module 'mesh'): bool ns3::flame::FlameProtocolMac::UpdateOutcomingFrame(ns3::Ptr<ns3::Packet> packet, ns3::WifiMacHeader & header, ns3::Mac48Address from, ns3::Mac48Address to) [member function]
    cls.add_method('UpdateOutcomingFrame', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::WifiMacHeader &', 'header'), param('ns3::Mac48Address', 'from'), param('ns3::Mac48Address', 'to')], 
                   is_virtual=True)
    return

def register_Ns3FlameFlameRtable_methods(root_module, cls):
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::FlameRtable() [constructor]
    cls.add_constructor([])
    ## flame-rtable.h (module 'mesh'): void ns3::flame::FlameRtable::AddPath(ns3::Mac48Address const destination, ns3::Mac48Address const retransmitter, uint32_t const interface, uint8_t const cost, uint16_t const seqnum) [member function]
    cls.add_method('AddPath', 
                   'void', 
                   [param('ns3::Mac48Address const', 'destination'), param('ns3::Mac48Address const', 'retransmitter'), param('uint32_t const', 'interface'), param('uint8_t const', 'cost'), param('uint16_t const', 'seqnum')])
    ## flame-rtable.h (module 'mesh'): void ns3::flame::FlameRtable::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## flame-rtable.h (module 'mesh'): static ns3::TypeId ns3::flame::FlameRtable::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::LookupResult ns3::flame::FlameRtable::Lookup(ns3::Mac48Address destination) [member function]
    cls.add_method('Lookup', 
                   'ns3::flame::FlameRtable::LookupResult', 
                   [param('ns3::Mac48Address', 'destination')])
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::INTERFACE_ANY [variable]
    cls.add_static_attribute('INTERFACE_ANY', 'uint32_t const', is_const=True)
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::MAX_COST [variable]
    cls.add_static_attribute('MAX_COST', 'uint32_t const', is_const=True)
    return

def register_Ns3FlameFlameRtableLookupResult_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::LookupResult::LookupResult(ns3::flame::FlameRtable::LookupResult const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameRtable::LookupResult const &', 'arg0')])
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::LookupResult::LookupResult(ns3::Mac48Address r=ns3::Mac48Address::GetBroadcast(), uint32_t i=ns3::flame::FlameRtable::INTERFACE_ANY, uint8_t c=ns3::flame::FlameRtable::MAX_COST, uint16_t s=0) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'r', default_value='ns3::Mac48Address::GetBroadcast()'), param('uint32_t', 'i', default_value='ns3::flame::FlameRtable::INTERFACE_ANY'), param('uint8_t', 'c', default_value='ns3::flame::FlameRtable::MAX_COST'), param('uint16_t', 's', default_value='0')])
    ## flame-rtable.h (module 'mesh'): bool ns3::flame::FlameRtable::LookupResult::IsValid() const [member function]
    cls.add_method('IsValid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::LookupResult::cost [variable]
    cls.add_instance_attribute('cost', 'uint8_t', is_const=False)
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::LookupResult::ifIndex [variable]
    cls.add_instance_attribute('ifIndex', 'uint32_t', is_const=False)
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::LookupResult::retransmitter [variable]
    cls.add_instance_attribute('retransmitter', 'ns3::Mac48Address', is_const=False)
    ## flame-rtable.h (module 'mesh'): ns3::flame::FlameRtable::LookupResult::seqnum [variable]
    cls.add_instance_attribute('seqnum', 'uint16_t', is_const=False)
    return

def register_Ns3FlameFlameTag_methods(root_module, cls):
    ## flame-protocol.h (module 'mesh'): ns3::flame::FlameTag::FlameTag(ns3::flame::FlameTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::flame::FlameTag const &', 'arg0')])
    ## flame-protocol.h (module 'mesh'): ns3::flame::FlameTag::FlameTag(ns3::Mac48Address a=ns3::Mac48Address()) [constructor]
    cls.add_constructor([param('ns3::Mac48Address', 'a', default_value='ns3::Mac48Address()')])
    ## flame-protocol.h (module 'mesh'): void ns3::flame::FlameTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## flame-protocol.h (module 'mesh'): ns3::TypeId ns3::flame::FlameTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h (module 'mesh'): uint32_t ns3::flame::FlameTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h (module 'mesh'): static ns3::TypeId ns3::flame::FlameTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flame-protocol.h (module 'mesh'): void ns3::flame::FlameTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h (module 'mesh'): void ns3::flame::FlameTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## flame-protocol.h (module 'mesh'): ns3::flame::FlameTag::receiver [variable]
    cls.add_instance_attribute('receiver', 'ns3::Mac48Address', is_const=False)
    ## flame-protocol.h (module 'mesh'): ns3::flame::FlameTag::transmitter [variable]
    cls.add_instance_attribute('transmitter', 'ns3::Mac48Address', is_const=False)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_Hash(module.get_submodule('Hash'), root_module)
    register_functions_ns3_TracedValueCallback(module.get_submodule('TracedValueCallback'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_Hash(module, root_module):
    register_functions_ns3_Hash_Function(module.get_submodule('Function'), root_module)
    return

def register_functions_ns3_Hash_Function(module, root_module):
    return

def register_functions_ns3_TracedValueCallback(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    ## ie-dot11s-id.h (module 'mesh'): extern ns3::Ptr<ns3::AttributeChecker const> ns3::dot11s::MakeIeMeshIdChecker() [free function]
    module.add_function('MakeIeMeshIdChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    return

def register_functions_ns3_flame(module, root_module):
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

