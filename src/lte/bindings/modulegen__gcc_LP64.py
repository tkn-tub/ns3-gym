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
    
    ## log.h (module 'core'): ns3::LogLevel [enumeration]
    module.add_enum('LogLevel', ['LOG_NONE', 'LOG_ERROR', 'LOG_LEVEL_ERROR', 'LOG_WARN', 'LOG_LEVEL_WARN', 'LOG_DEBUG', 'LOG_LEVEL_DEBUG', 'LOG_INFO', 'LOG_LEVEL_INFO', 'LOG_FUNCTION', 'LOG_LEVEL_FUNCTION', 'LOG_LOGIC', 'LOG_LEVEL_LOGIC', 'LOG_ALL', 'LOG_LEVEL_ALL', 'LOG_PREFIX_FUNC', 'LOG_PREFIX_TIME', 'LOG_PREFIX_NODE'], import_from_module='ns.core')
    ## address.h (module 'network'): ns3::Address [class]
    module.add_class('Address', import_from_module='ns.network')
    ## address.h (module 'network'): ns3::Address::MaxSize_e [enumeration]
    module.add_enum('MaxSize_e', ['MAX_SIZE'], outer_class=root_module['ns3::Address'], import_from_module='ns.network')
    ## attribute-list.h (module 'core'): ns3::AttributeList [class]
    module.add_class('AttributeList', import_from_module='ns.core')
    ## spectrum-model.h (module 'spectrum'): ns3::BandInfo [struct]
    module.add_class('BandInfo', import_from_module='ns.spectrum')
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
    ## data-rate.h (module 'network'): ns3::DataRate [class]
    module.add_class('DataRate', import_from_module='ns.network')
    ## event-id.h (module 'core'): ns3::EventId [class]
    module.add_class('EventId', import_from_module='ns.core')
    ## ipcs-classifier-record.h (module 'wimax'): ns3::IpcsClassifierRecord [class]
    module.add_class('IpcsClassifierRecord', import_from_module='ns.wimax')
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
    ## log.h (module 'core'): ns3::LogComponent [class]
    module.add_class('LogComponent', import_from_module='ns.core')
    ## lte-helper.h (module 'lte'): ns3::LteHelper [class]
    module.add_class('LteHelper')
    ## lte-helper.h (module 'lte'): ns3::LteHelper::NetDeviceType [enumeration]
    module.add_enum('NetDeviceType', ['DEVICE_TYPE_USER_EQUIPMENT', 'DEVICE_TYPE_ENODEB'], outer_class=root_module['ns3::LteHelper'])
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::LteSpectrumValueHelper [class]
    module.add_class('LteSpectrumValueHelper')
    ## mac48-address.h (module 'network'): ns3::Mac48Address [class]
    module.add_class('Mac48Address', import_from_module='ns.network')
    ## mac48-address.h (module 'network'): ns3::Mac48Address [class]
    root_module['ns3::Mac48Address'].implicitly_converts_to(root_module['ns3::Address'])
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
    ## random-variable.h (module 'core'): ns3::RandomVariable [class]
    module.add_class('RandomVariable', import_from_module='ns.core')
    ## random-variable.h (module 'core'): ns3::SeedManager [class]
    module.add_class('SeedManager', import_from_module='ns.core')
    ## random-variable.h (module 'core'): ns3::SequentialVariable [class]
    module.add_class('SequentialVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Object', 'ns3::ObjectBase', 'ns3::ObjectDeleter'], parent=root_module['ns3::ObjectBase'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## spectrum-type.h (module 'spectrum'): ns3::SpectrumType [class]
    module.add_class('SpectrumType', import_from_module='ns.spectrum')
    ## spectrum-type.h (module 'spectrum'): ns3::SpectrumTypeFactory [class]
    module.add_class('SpectrumTypeFactory', import_from_module='ns.spectrum')
    ## tag.h (module 'network'): ns3::Tag [class]
    module.add_class('Tag', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## tag-buffer.h (module 'network'): ns3::TagBuffer [class]
    module.add_class('TagBuffer', import_from_module='ns.network')
    ## wimax-tlv.h (module 'wimax'): ns3::TlvValue [class]
    module.add_class('TlvValue', allow_subclassing=True, import_from_module='ns.wimax')
    ## wimax-tlv.h (module 'wimax'): ns3::TosTlvValue [class]
    module.add_class('TosTlvValue', import_from_module='ns.wimax', parent=root_module['ns3::TlvValue'])
    ## random-variable.h (module 'core'): ns3::TriangularVariable [class]
    module.add_class('TriangularVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## type-id.h (module 'core'): ns3::TypeId [class]
    module.add_class('TypeId', import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId::AttributeFlag [enumeration]
    module.add_enum('AttributeFlag', ['ATTR_GET', 'ATTR_SET', 'ATTR_CONSTRUCT', 'ATTR_SGC'], outer_class=root_module['ns3::TypeId'], import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInfo [struct]
    module.add_class('AttributeInfo', import_from_module='ns.core', outer_class=root_module['ns3::TypeId'])
    ## wimax-tlv.h (module 'wimax'): ns3::U16TlvValue [class]
    module.add_class('U16TlvValue', import_from_module='ns.wimax', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h (module 'wimax'): ns3::U32TlvValue [class]
    module.add_class('U32TlvValue', import_from_module='ns.wimax', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h (module 'wimax'): ns3::U8TlvValue [class]
    module.add_class('U8TlvValue', import_from_module='ns.wimax', parent=root_module['ns3::TlvValue'])
    ## random-variable.h (module 'core'): ns3::UniformVariable [class]
    module.add_class('UniformVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## attribute-list.h (module 'core'): ns3::UnsafeAttributeList [class]
    module.add_class('UnsafeAttributeList', import_from_module='ns.core')
    ## vector.h (module 'core'): ns3::Vector2D [class]
    module.add_class('Vector2D', import_from_module='ns.core')
    ## vector.h (module 'core'): ns3::Vector3D [class]
    module.add_class('Vector3D', import_from_module='ns.core')
    ## wimax-tlv.h (module 'wimax'): ns3::VectorTlvValue [class]
    module.add_class('VectorTlvValue', import_from_module='ns.wimax', parent=root_module['ns3::TlvValue'])
    ## random-variable.h (module 'core'): ns3::WeibullVariable [class]
    module.add_class('WeibullVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::ZetaVariable [class]
    module.add_class('ZetaVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::ZipfVariable [class]
    module.add_class('ZipfVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## empty.h (module 'core'): ns3::empty [class]
    module.add_class('empty', import_from_module='ns.core')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t [class]
    module.add_class('int64x64_t', import_from_module='ns.core')
    ## chunk.h (module 'network'): ns3::Chunk [class]
    module.add_class('Chunk', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## wimax-tlv.h (module 'wimax'): ns3::ClassificationRuleVectorTlvValue [class]
    module.add_class('ClassificationRuleVectorTlvValue', import_from_module='ns.wimax', parent=root_module['ns3::VectorTlvValue'])
    ## wimax-tlv.h (module 'wimax'): ns3::ClassificationRuleVectorTlvValue::ClassificationRuleTlvType [enumeration]
    module.add_enum('ClassificationRuleTlvType', ['Priority', 'ToS', 'Protocol', 'IP_src', 'IP_dst', 'Port_src', 'Port_dst', 'Index'], outer_class=root_module['ns3::ClassificationRuleVectorTlvValue'], import_from_module='ns.wimax')
    ## random-variable.h (module 'core'): ns3::ConstantVariable [class]
    module.add_class('ConstantVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## wimax-tlv.h (module 'wimax'): ns3::CsParamVectorTlvValue [class]
    module.add_class('CsParamVectorTlvValue', import_from_module='ns.wimax', parent=root_module['ns3::VectorTlvValue'])
    ## wimax-tlv.h (module 'wimax'): ns3::CsParamVectorTlvValue::Type [enumeration]
    module.add_enum('Type', ['Classifier_DSC_Action', 'Packet_Classification_Rule'], outer_class=root_module['ns3::CsParamVectorTlvValue'], import_from_module='ns.wimax')
    ## random-variable.h (module 'core'): ns3::DeterministicVariable [class]
    module.add_class('DeterministicVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::EmpiricalVariable [class]
    module.add_class('EmpiricalVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::ErlangVariable [class]
    module.add_class('ErlangVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::ExponentialVariable [class]
    module.add_class('ExponentialVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::GammaVariable [class]
    module.add_class('GammaVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## header.h (module 'network'): ns3::Header [class]
    module.add_class('Header', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## random-variable.h (module 'core'): ns3::IntEmpiricalVariable [class]
    module.add_class('IntEmpiricalVariable', import_from_module='ns.core', parent=root_module['ns3::EmpiricalVariable'])
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue [class]
    module.add_class('Ipv4AddressTlvValue', import_from_module='ns.wimax', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue::ipv4Addr [struct]
    module.add_class('ipv4Addr', import_from_module='ns.wimax', outer_class=root_module['ns3::Ipv4AddressTlvValue'])
    ## random-variable.h (module 'core'): ns3::LogNormalVariable [class]
    module.add_class('LogNormalVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## lte-mac-header.h (module 'lte'): ns3::LteMacHeader [class]
    module.add_class('LteMacHeader', parent=root_module['ns3::Header'])
    ## random-variable.h (module 'core'): ns3::NormalVariable [class]
    module.add_class('NormalVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## object.h (module 'core'): ns3::Object [class]
    module.add_class('Object', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    ## object.h (module 'core'): ns3::Object::AggregateIterator [class]
    module.add_class('AggregateIterator', import_from_module='ns.core', outer_class=root_module['ns3::Object'])
    ## packet-burst.h (module 'network'): ns3::PacketBurst [class]
    module.add_class('PacketBurst', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## packet-scheduler.h (module 'lte'): ns3::PacketScheduler [class]
    module.add_class('PacketScheduler', parent=root_module['ns3::Object'])
    ## random-variable.h (module 'core'): ns3::ParetoVariable [class]
    module.add_class('ParetoVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue [class]
    module.add_class('PortRangeTlvValue', import_from_module='ns.wimax', parent=root_module['ns3::TlvValue'])
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue::PortRange [struct]
    module.add_class('PortRange', import_from_module='ns.wimax', outer_class=root_module['ns3::PortRangeTlvValue'])
    ## propagation-delay-model.h (module 'propagation'): ns3::PropagationDelayModel [class]
    module.add_class('PropagationDelayModel', import_from_module='ns.propagation', parent=root_module['ns3::Object'])
    ## wimax-tlv.h (module 'wimax'): ns3::ProtocolTlvValue [class]
    module.add_class('ProtocolTlvValue', import_from_module='ns.wimax', parent=root_module['ns3::TlvValue'])
    ## radio-bearer-instance.h (module 'lte'): ns3::RadioBearerInstance [class]
    module.add_class('RadioBearerInstance', parent=root_module['ns3::Object'])
    ## radio-bearer-instance.h (module 'lte'): ns3::RadioBearerInstance::BearerDirection [enumeration]
    module.add_enum('BearerDirection', ['DIRECTION_TYPE_UL', 'DIRECTION_TYPE_DL'], outer_class=root_module['ns3::RadioBearerInstance'])
    ## radio-bearer-instance.h (module 'lte'): ns3::RadioBearerInstance::BearerType [enumeration]
    module.add_enum('BearerType', ['BEARER_TYPE_SRB1', 'BEARER_TYPE_SRB2', 'BEARER_TYPE_DRB'], outer_class=root_module['ns3::RadioBearerInstance'])
    ## propagation-delay-model.h (module 'propagation'): ns3::RandomPropagationDelayModel [class]
    module.add_class('RandomPropagationDelayModel', import_from_module='ns.propagation', parent=root_module['ns3::PropagationDelayModel'])
    ## rlc-entity.h (module 'lte'): ns3::RlcEntity [class]
    module.add_class('RlcEntity', parent=root_module['ns3::Object'])
    ## rrc-entity.h (module 'lte'): ns3::RrcEntity [class]
    module.add_class('RrcEntity', parent=root_module['ns3::Object'])
    ## wimax-tlv.h (module 'wimax'): ns3::SfVectorTlvValue [class]
    module.add_class('SfVectorTlvValue', import_from_module='ns.wimax', parent=root_module['ns3::VectorTlvValue'])
    ## wimax-tlv.h (module 'wimax'): ns3::SfVectorTlvValue::Type [enumeration]
    module.add_enum('Type', ['SFID', 'CID', 'Service_Class_Name', 'reserved1', 'QoS_Parameter_Set_Type', 'Traffic_Priority', 'Maximum_Sustained_Traffic_Rate', 'Maximum_Traffic_Burst', 'Minimum_Reserved_Traffic_Rate', 'Minimum_Tolerable_Traffic_Rate', 'Service_Flow_Scheduling_Type', 'Request_Transmission_Policy', 'Tolerated_Jitter', 'Maximum_Latency', 'Fixed_length_versus_Variable_length_SDU_Indicator', 'SDU_Size', 'Target_SAID', 'ARQ_Enable', 'ARQ_WINDOW_SIZE', 'ARQ_RETRY_TIMEOUT_Transmitter_Delay', 'ARQ_RETRY_TIMEOUT_Receiver_Delay', 'ARQ_BLOCK_LIFETIME', 'ARQ_SYNC_LOSS', 'ARQ_DELIVER_IN_ORDER', 'ARQ_PURGE_TIMEOUT', 'ARQ_BLOCK_SIZE', 'reserved2', 'CS_Specification', 'IPV4_CS_Parameters'], outer_class=root_module['ns3::SfVectorTlvValue'], import_from_module='ns.wimax')
    ## simple-packet-scheduler.h (module 'lte'): ns3::SimplePacketScheduler [class]
    module.add_class('SimplePacketScheduler', parent=root_module['ns3::PacketScheduler'])
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
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::IdealControlMessage', 'ns3::empty', 'ns3::DefaultDeleter<ns3::IdealControlMessage>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::NixVector', 'ns3::empty', 'ns3::DefaultDeleter<ns3::NixVector>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Packet', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Packet>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::SpectrumModel', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumModel>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::SpectrumValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## spectrum-interference.h (module 'spectrum'): ns3::SpectrumInterference [class]
    module.add_class('SpectrumInterference', import_from_module='ns.spectrum', parent=root_module['ns3::Object'])
    ## spectrum-model.h (module 'spectrum'): ns3::SpectrumModel [class]
    module.add_class('SpectrumModel', import_from_module='ns.spectrum', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >'])
    ## spectrum-phy.h (module 'spectrum'): ns3::SpectrumPhy [class]
    module.add_class('SpectrumPhy', import_from_module='ns.spectrum', parent=root_module['ns3::Object'])
    ## spectrum-propagation-loss-model.h (module 'spectrum'): ns3::SpectrumPropagationLossModel [class]
    module.add_class('SpectrumPropagationLossModel', import_from_module='ns.spectrum', parent=root_module['ns3::Object'])
    ## spectrum-value.h (module 'spectrum'): ns3::SpectrumValue [class]
    module.add_class('SpectrumValue', import_from_module='ns.spectrum', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >'])
    ## nstime.h (module 'core'): ns3::Time [class]
    module.add_class('Time', import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time::Unit [enumeration]
    module.add_enum('Unit', ['S', 'MS', 'US', 'NS', 'PS', 'FS', 'LAST'], outer_class=root_module['ns3::Time'], import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time [class]
    root_module['ns3::Time'].implicitly_converts_to(root_module['ns3::int64x64_t'])
    ## wimax-tlv.h (module 'wimax'): ns3::Tlv [class]
    module.add_class('Tlv', import_from_module='ns.wimax', parent=root_module['ns3::Header'])
    ## wimax-tlv.h (module 'wimax'): ns3::Tlv::CommonTypes [enumeration]
    module.add_enum('CommonTypes', ['HMAC_TUPLE', 'MAC_VERSION_ENCODING', 'CURRENT_TRANSMIT_POWER', 'DOWNLINK_SERVICE_FLOW', 'UPLINK_SERVICE_FLOW', 'VENDOR_ID_EMCODING', 'VENDOR_SPECIFIC_INFORMATION'], outer_class=root_module['ns3::Tlv'], import_from_module='ns.wimax')
    ## trailer.h (module 'network'): ns3::Trailer [class]
    module.add_class('Trailer', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## ue-manager.h (module 'lte'): ns3::UeManager [class]
    module.add_class('UeManager', parent=root_module['ns3::Object'])
    ## ue-record.h (module 'lte'): ns3::UeRecord [class]
    module.add_class('UeRecord', parent=root_module['ns3::Object'])
    ## ue-record.h (module 'lte'): ns3::UeRecord::CqiFeedback [struct]
    module.add_class('CqiFeedback', outer_class=root_module['ns3::UeRecord'])
    ## amc-module.h (module 'lte'): ns3::AmcModule [class]
    module.add_class('AmcModule', parent=root_module['ns3::Object'])
    ## attribute.h (module 'core'): ns3::AttributeAccessor [class]
    module.add_class('AttributeAccessor', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    ## attribute.h (module 'core'): ns3::AttributeChecker [class]
    module.add_class('AttributeChecker', allow_subclassing=False, automatic_type_narrowing=True, import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    ## attribute.h (module 'core'): ns3::AttributeValue [class]
    module.add_class('AttributeValue', allow_subclassing=False, automatic_type_narrowing=True, import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    ## bearer-qos-parameters.h (module 'lte'): ns3::BearerQosParameters [class]
    module.add_class('BearerQosParameters', parent=root_module['ns3::Object'])
    ## bearer-qos-parameters.h (module 'lte'): ns3::BearerQosParameters::BearerQosType [enumeration]
    module.add_enum('BearerQosType', ['BEARER_TYPE_GBR', 'BEARER_TYPE_NGBR'], outer_class=root_module['ns3::BearerQosParameters'])
    ## callback.h (module 'core'): ns3::CallbackChecker [class]
    module.add_class('CallbackChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## callback.h (module 'core'): ns3::CallbackImplBase [class]
    module.add_class('CallbackImplBase', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    ## callback.h (module 'core'): ns3::CallbackValue [class]
    module.add_class('CallbackValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## channel.h (module 'network'): ns3::Channel [class]
    module.add_class('Channel', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## channel-realization.h (module 'lte'): ns3::ChannelRealization [class]
    module.add_class('ChannelRealization', parent=root_module['ns3::Object'])
    ## propagation-delay-model.h (module 'propagation'): ns3::ConstantSpeedPropagationDelayModel [class]
    module.add_class('ConstantSpeedPropagationDelayModel', import_from_module='ns.propagation', parent=root_module['ns3::PropagationDelayModel'])
    ## data-rate.h (module 'network'): ns3::DataRateChecker [class]
    module.add_class('DataRateChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## data-rate.h (module 'network'): ns3::DataRateValue [class]
    module.add_class('DataRateValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## discrete-time-loss-model.h (module 'lte'): ns3::DiscreteTimeLossModel [class]
    module.add_class('DiscreteTimeLossModel', parent=root_module['ns3::Object'])
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue [class]
    module.add_class('EmptyAttributeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## event-impl.h (module 'core'): ns3::EventImpl [class]
    module.add_class('EventImpl', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    ## ideal-control-messages.h (module 'lte'): ns3::IdealControlMessage [class]
    module.add_class('IdealControlMessage', parent=root_module['ns3::SimpleRefCount< ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >'])
    ## ideal-control-messages.h (module 'lte'): ns3::IdealControlMessage::MessageType [enumeration]
    module.add_enum('MessageType', ['CQI_FEEDBACKS', 'ALLOCATION_MAP'], outer_class=root_module['ns3::IdealControlMessage'])
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
    ## jakes-fading-loss-model.h (module 'lte'): ns3::JakesFadingLossModel [class]
    module.add_class('JakesFadingLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## lte-mac-queue.h (module 'lte'): ns3::LteMacQueue [class]
    module.add_class('LteMacQueue', parent=root_module['ns3::Object'])
    ## lte-phy.h (module 'lte'): ns3::LtePhy [class]
    module.add_class('LtePhy', parent=root_module['ns3::Object'])
    ## lte-propagation-loss-model.h (module 'lte'): ns3::LtePropagationLossModel [class]
    module.add_class('LtePropagationLossModel', parent=root_module['ns3::SpectrumPropagationLossModel'])
    ## lte-spectrum-phy.h (module 'lte'): ns3::LteSpectrumPhy [class]
    module.add_class('LteSpectrumPhy', parent=root_module['ns3::SpectrumPhy'])
    ## lte-spectrum-phy.h (module 'lte'): ns3::LteSpectrumPhy::State [enumeration]
    module.add_enum('State', ['IDLE', 'TX', 'RX'], outer_class=root_module['ns3::LteSpectrumPhy'])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressChecker [class]
    module.add_class('Mac48AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue [class]
    module.add_class('Mac48AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## mac-entity.h (module 'lte'): ns3::MacEntity [class]
    module.add_class('MacEntity', parent=root_module['ns3::Object'])
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
    ## object-factory.h (module 'core'): ns3::ObjectFactoryChecker [class]
    module.add_class('ObjectFactoryChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue [class]
    module.add_class('ObjectFactoryValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## packet.h (module 'network'): ns3::Packet [class]
    module.add_class('Packet', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    ## path-loss-model.h (module 'lte'): ns3::PathLossModel [class]
    module.add_class('PathLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage [class]
    module.add_class('PdcchMapIdealControlMessage', parent=root_module['ns3::IdealControlMessage'])
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::Direction [enumeration]
    module.add_enum('Direction', ['DOWNLINK', 'UPLINK'], outer_class=root_module['ns3::PdcchMapIdealControlMessage'])
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::IdealPdcchRecord [struct]
    module.add_class('IdealPdcchRecord', outer_class=root_module['ns3::PdcchMapIdealControlMessage'])
    ## penetration-loss-model.h (module 'lte'): ns3::PenetrationLossModel [class]
    module.add_class('PenetrationLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## random-variable.h (module 'core'): ns3::RandomVariableChecker [class]
    module.add_class('RandomVariableChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## random-variable.h (module 'core'): ns3::RandomVariableValue [class]
    module.add_class('RandomVariableValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## shadowing-loss-model.h (module 'lte'): ns3::ShadowingLossModel [class]
    module.add_class('ShadowingLossModel', parent=root_module['ns3::DiscreteTimeLossModel'])
    ## spectrum-channel.h (module 'spectrum'): ns3::SpectrumChannel [class]
    module.add_class('SpectrumChannel', import_from_module='ns.spectrum', parent=root_module['ns3::Channel'])
    ## nstime.h (module 'core'): ns3::TimeChecker [class]
    module.add_class('TimeChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## nstime.h (module 'core'): ns3::TimeValue [class]
    module.add_class('TimeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## type-id.h (module 'core'): ns3::TypeIdChecker [class]
    module.add_class('TypeIdChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## type-id.h (module 'core'): ns3::TypeIdValue [class]
    module.add_class('TypeIdValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## ue-phy.h (module 'lte'): ns3::UeLtePhy [class]
    module.add_class('UeLtePhy', parent=root_module['ns3::LtePhy'])
    ## ue-lte-spectrum-phy.h (module 'lte'): ns3::UeLteSpectrumPhy [class]
    module.add_class('UeLteSpectrumPhy', parent=root_module['ns3::LteSpectrumPhy'])
    ## ue-mac-entity.h (module 'lte'): ns3::UeMacEntity [class]
    module.add_class('UeMacEntity', parent=root_module['ns3::MacEntity'])
    ## uinteger.h (module 'core'): ns3::UintegerValue [class]
    module.add_class('UintegerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## vector.h (module 'core'): ns3::Vector2DChecker [class]
    module.add_class('Vector2DChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## vector.h (module 'core'): ns3::Vector2DValue [class]
    module.add_class('Vector2DValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## vector.h (module 'core'): ns3::Vector3DChecker [class]
    module.add_class('Vector3DChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## vector.h (module 'core'): ns3::Vector3DValue [class]
    module.add_class('Vector3DValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## address.h (module 'network'): ns3::AddressChecker [class]
    module.add_class('AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## address.h (module 'network'): ns3::AddressValue [class]
    module.add_class('AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ideal-control-messages.h (module 'lte'): ns3::CqiIdealControlMessage [class]
    module.add_class('CqiIdealControlMessage', parent=root_module['ns3::IdealControlMessage'])
    ## ideal-control-messages.h (module 'lte'): ns3::CqiIdealControlMessage::CqiFeedback [struct]
    module.add_class('CqiFeedback', outer_class=root_module['ns3::CqiIdealControlMessage'])
    ## enb-phy.h (module 'lte'): ns3::EnbLtePhy [class]
    module.add_class('EnbLtePhy', parent=root_module['ns3::LtePhy'])
    ## enb-lte-spectrum-phy.h (module 'lte'): ns3::EnbLteSpectrumPhy [class]
    module.add_class('EnbLteSpectrumPhy', parent=root_module['ns3::LteSpectrumPhy'])
    ## enb-mac-entity.h (module 'lte'): ns3::EnbMacEntity [class]
    module.add_class('EnbMacEntity', parent=root_module['ns3::MacEntity'])
    ## lte-net-device.h (module 'lte'): ns3::LteNetDevice [class]
    module.add_class('LteNetDevice', parent=root_module['ns3::NetDevice'])
    ## single-model-spectrum-channel.h (module 'spectrum'): ns3::SingleModelSpectrumChannel [class]
    module.add_class('SingleModelSpectrumChannel', import_from_module='ns.spectrum', parent=root_module['ns3::SpectrumChannel'])
    ## ue-net-device.h (module 'lte'): ns3::UeNetDevice [class]
    module.add_class('UeNetDevice', parent=root_module['ns3::LteNetDevice'])
    ## enb-net-device.h (module 'lte'): ns3::EnbNetDevice [class]
    module.add_class('EnbNetDevice', parent=root_module['ns3::LteNetDevice'])
    module.add_container('std::vector< int >', 'int', container_type='vector')
    module.add_container('std::list< ns3::Ptr< ns3::Packet > >', 'ns3::Ptr< ns3::Packet >', container_type='list')
    module.add_container('std::vector< ns3::Ptr< ns3::RadioBearerInstance > >', 'ns3::Ptr< ns3::RadioBearerInstance >', container_type='vector')
    module.add_container('std::vector< double >', 'double', container_type='vector')
    module.add_container('ns3::Bands', 'ns3::BandInfo', container_type='vector')
    module.add_container('std::vector< ns3::Ptr< ns3::UeRecord > >', 'ns3::Ptr< ns3::UeRecord >', container_type='vector')
    module.add_container('std::vector< ns3::UeRecord::CqiFeedback >', 'ns3::UeRecord::CqiFeedback', container_type='vector')
    module.add_container('std::vector< std::vector< double > >', 'std::vector< double >', container_type='vector')
    module.add_container('std::deque< ns3::LteMacQueue::QueueElement >', 'ns3::LteMacQueue::QueueElement', container_type='dequeue')
    module.add_container('std::list< ns3::PdcchMapIdealControlMessage::IdealPdcchRecord >', 'ns3::PdcchMapIdealControlMessage::IdealPdcchRecord', container_type='list')
    module.add_container('std::list< ns3::CqiIdealControlMessage::CqiFeedback >', 'ns3::CqiIdealControlMessage::CqiFeedback', container_type='list')
    module.add_container('std::vector< ns3::Ptr< ns3::SpectrumPhy > >', 'ns3::Ptr< ns3::SpectrumPhy >', container_type='vector')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxEndOkCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxEndOkCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxEndOkCallback&')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >', 'ns3::Bands')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >*', 'ns3::Bands*')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >&', 'ns3::Bands&')
    typehandlers.add_type_alias('ns3::Vector3DValue', 'ns3::VectorValue')
    typehandlers.add_type_alias('ns3::Vector3DValue*', 'ns3::VectorValue*')
    typehandlers.add_type_alias('ns3::Vector3DValue&', 'ns3::VectorValue&')
    module.add_typedef(root_module['ns3::Vector3DValue'], 'VectorValue')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) *', 'ns3::LogNodePrinter')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) **', 'ns3::LogNodePrinter*')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) *&', 'ns3::LogNodePrinter&')
    typehandlers.add_type_alias('ns3::Vector3D', 'ns3::Vector')
    typehandlers.add_type_alias('ns3::Vector3D*', 'ns3::Vector*')
    typehandlers.add_type_alias('ns3::Vector3D&', 'ns3::Vector&')
    module.add_typedef(root_module['ns3::Vector3D'], 'Vector')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxStartCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxStartCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxStartCallback&')
    typehandlers.add_type_alias('uint32_t', 'ns3::SpectrumModelUid_t')
    typehandlers.add_type_alias('uint32_t*', 'ns3::SpectrumModelUid_t*')
    typehandlers.add_type_alias('uint32_t&', 'ns3::SpectrumModelUid_t&')
    typehandlers.add_type_alias('ns3::Vector3DChecker', 'ns3::VectorChecker')
    typehandlers.add_type_alias('ns3::Vector3DChecker*', 'ns3::VectorChecker*')
    typehandlers.add_type_alias('ns3::Vector3DChecker&', 'ns3::VectorChecker&')
    module.add_typedef(root_module['ns3::Vector3DChecker'], 'VectorChecker')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) *', 'ns3::LogTimePrinter')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) **', 'ns3::LogTimePrinter*')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) *&', 'ns3::LogTimePrinter&')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyTxStartCallback')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyTxStartCallback*')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyTxStartCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxEndErrorCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxEndErrorCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxEndErrorCallback&')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >', 'ns3::Values')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >*', 'ns3::Values*')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >&', 'ns3::Values&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyTxEndCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyTxEndCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyTxEndCallback&')
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace addressUtils
    
    nested_module = module.add_cpp_namespace('addressUtils')
    register_types_ns3_addressUtils(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Address_methods(root_module, root_module['ns3::Address'])
    register_Ns3AttributeList_methods(root_module, root_module['ns3::AttributeList'])
    register_Ns3BandInfo_methods(root_module, root_module['ns3::BandInfo'])
    register_Ns3Buffer_methods(root_module, root_module['ns3::Buffer'])
    register_Ns3BufferIterator_methods(root_module, root_module['ns3::Buffer::Iterator'])
    register_Ns3ByteTagIterator_methods(root_module, root_module['ns3::ByteTagIterator'])
    register_Ns3ByteTagIteratorItem_methods(root_module, root_module['ns3::ByteTagIterator::Item'])
    register_Ns3ByteTagList_methods(root_module, root_module['ns3::ByteTagList'])
    register_Ns3ByteTagListIterator_methods(root_module, root_module['ns3::ByteTagList::Iterator'])
    register_Ns3ByteTagListIteratorItem_methods(root_module, root_module['ns3::ByteTagList::Iterator::Item'])
    register_Ns3CallbackBase_methods(root_module, root_module['ns3::CallbackBase'])
    register_Ns3DataRate_methods(root_module, root_module['ns3::DataRate'])
    register_Ns3EventId_methods(root_module, root_module['ns3::EventId'])
    register_Ns3IpcsClassifierRecord_methods(root_module, root_module['ns3::IpcsClassifierRecord'])
    register_Ns3Ipv4Address_methods(root_module, root_module['ns3::Ipv4Address'])
    register_Ns3Ipv4Mask_methods(root_module, root_module['ns3::Ipv4Mask'])
    register_Ns3Ipv6Address_methods(root_module, root_module['ns3::Ipv6Address'])
    register_Ns3Ipv6Prefix_methods(root_module, root_module['ns3::Ipv6Prefix'])
    register_Ns3LogComponent_methods(root_module, root_module['ns3::LogComponent'])
    register_Ns3LteHelper_methods(root_module, root_module['ns3::LteHelper'])
    register_Ns3LteSpectrumValueHelper_methods(root_module, root_module['ns3::LteSpectrumValueHelper'])
    register_Ns3Mac48Address_methods(root_module, root_module['ns3::Mac48Address'])
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
    register_Ns3RandomVariable_methods(root_module, root_module['ns3::RandomVariable'])
    register_Ns3SeedManager_methods(root_module, root_module['ns3::SeedManager'])
    register_Ns3SequentialVariable_methods(root_module, root_module['ns3::SequentialVariable'])
    register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    register_Ns3SpectrumType_methods(root_module, root_module['ns3::SpectrumType'])
    register_Ns3SpectrumTypeFactory_methods(root_module, root_module['ns3::SpectrumTypeFactory'])
    register_Ns3Tag_methods(root_module, root_module['ns3::Tag'])
    register_Ns3TagBuffer_methods(root_module, root_module['ns3::TagBuffer'])
    register_Ns3TlvValue_methods(root_module, root_module['ns3::TlvValue'])
    register_Ns3TosTlvValue_methods(root_module, root_module['ns3::TosTlvValue'])
    register_Ns3TriangularVariable_methods(root_module, root_module['ns3::TriangularVariable'])
    register_Ns3TypeId_methods(root_module, root_module['ns3::TypeId'])
    register_Ns3TypeIdAttributeInfo_methods(root_module, root_module['ns3::TypeId::AttributeInfo'])
    register_Ns3U16TlvValue_methods(root_module, root_module['ns3::U16TlvValue'])
    register_Ns3U32TlvValue_methods(root_module, root_module['ns3::U32TlvValue'])
    register_Ns3U8TlvValue_methods(root_module, root_module['ns3::U8TlvValue'])
    register_Ns3UniformVariable_methods(root_module, root_module['ns3::UniformVariable'])
    register_Ns3UnsafeAttributeList_methods(root_module, root_module['ns3::UnsafeAttributeList'])
    register_Ns3Vector2D_methods(root_module, root_module['ns3::Vector2D'])
    register_Ns3Vector3D_methods(root_module, root_module['ns3::Vector3D'])
    register_Ns3VectorTlvValue_methods(root_module, root_module['ns3::VectorTlvValue'])
    register_Ns3WeibullVariable_methods(root_module, root_module['ns3::WeibullVariable'])
    register_Ns3ZetaVariable_methods(root_module, root_module['ns3::ZetaVariable'])
    register_Ns3ZipfVariable_methods(root_module, root_module['ns3::ZipfVariable'])
    register_Ns3Empty_methods(root_module, root_module['ns3::empty'])
    register_Ns3Int64x64_t_methods(root_module, root_module['ns3::int64x64_t'])
    register_Ns3Chunk_methods(root_module, root_module['ns3::Chunk'])
    register_Ns3ClassificationRuleVectorTlvValue_methods(root_module, root_module['ns3::ClassificationRuleVectorTlvValue'])
    register_Ns3ConstantVariable_methods(root_module, root_module['ns3::ConstantVariable'])
    register_Ns3CsParamVectorTlvValue_methods(root_module, root_module['ns3::CsParamVectorTlvValue'])
    register_Ns3DeterministicVariable_methods(root_module, root_module['ns3::DeterministicVariable'])
    register_Ns3EmpiricalVariable_methods(root_module, root_module['ns3::EmpiricalVariable'])
    register_Ns3ErlangVariable_methods(root_module, root_module['ns3::ErlangVariable'])
    register_Ns3ExponentialVariable_methods(root_module, root_module['ns3::ExponentialVariable'])
    register_Ns3GammaVariable_methods(root_module, root_module['ns3::GammaVariable'])
    register_Ns3Header_methods(root_module, root_module['ns3::Header'])
    register_Ns3IntEmpiricalVariable_methods(root_module, root_module['ns3::IntEmpiricalVariable'])
    register_Ns3Ipv4AddressTlvValue_methods(root_module, root_module['ns3::Ipv4AddressTlvValue'])
    register_Ns3Ipv4AddressTlvValueIpv4Addr_methods(root_module, root_module['ns3::Ipv4AddressTlvValue::ipv4Addr'])
    register_Ns3LogNormalVariable_methods(root_module, root_module['ns3::LogNormalVariable'])
    register_Ns3LteMacHeader_methods(root_module, root_module['ns3::LteMacHeader'])
    register_Ns3NormalVariable_methods(root_module, root_module['ns3::NormalVariable'])
    register_Ns3Object_methods(root_module, root_module['ns3::Object'])
    register_Ns3ObjectAggregateIterator_methods(root_module, root_module['ns3::Object::AggregateIterator'])
    register_Ns3PacketBurst_methods(root_module, root_module['ns3::PacketBurst'])
    register_Ns3PacketScheduler_methods(root_module, root_module['ns3::PacketScheduler'])
    register_Ns3ParetoVariable_methods(root_module, root_module['ns3::ParetoVariable'])
    register_Ns3PortRangeTlvValue_methods(root_module, root_module['ns3::PortRangeTlvValue'])
    register_Ns3PortRangeTlvValuePortRange_methods(root_module, root_module['ns3::PortRangeTlvValue::PortRange'])
    register_Ns3PropagationDelayModel_methods(root_module, root_module['ns3::PropagationDelayModel'])
    register_Ns3ProtocolTlvValue_methods(root_module, root_module['ns3::ProtocolTlvValue'])
    register_Ns3RadioBearerInstance_methods(root_module, root_module['ns3::RadioBearerInstance'])
    register_Ns3RandomPropagationDelayModel_methods(root_module, root_module['ns3::RandomPropagationDelayModel'])
    register_Ns3RlcEntity_methods(root_module, root_module['ns3::RlcEntity'])
    register_Ns3RrcEntity_methods(root_module, root_module['ns3::RrcEntity'])
    register_Ns3SfVectorTlvValue_methods(root_module, root_module['ns3::SfVectorTlvValue'])
    register_Ns3SimplePacketScheduler_methods(root_module, root_module['ns3::SimplePacketScheduler'])
    register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    register_Ns3SimpleRefCount__Ns3IdealControlMessage_Ns3Empty_Ns3DefaultDeleter__lt__ns3IdealControlMessage__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::IdealControlMessage, ns3::empty, ns3::DefaultDeleter<ns3::IdealControlMessage> >'])
    register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    register_Ns3SimpleRefCount__Ns3SpectrumModel_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumModel__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >'])
    register_Ns3SimpleRefCount__Ns3SpectrumValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >'])
    register_Ns3SpectrumInterference_methods(root_module, root_module['ns3::SpectrumInterference'])
    register_Ns3SpectrumModel_methods(root_module, root_module['ns3::SpectrumModel'])
    register_Ns3SpectrumPhy_methods(root_module, root_module['ns3::SpectrumPhy'])
    register_Ns3SpectrumPropagationLossModel_methods(root_module, root_module['ns3::SpectrumPropagationLossModel'])
    register_Ns3SpectrumValue_methods(root_module, root_module['ns3::SpectrumValue'])
    register_Ns3Time_methods(root_module, root_module['ns3::Time'])
    register_Ns3Tlv_methods(root_module, root_module['ns3::Tlv'])
    register_Ns3Trailer_methods(root_module, root_module['ns3::Trailer'])
    register_Ns3UeManager_methods(root_module, root_module['ns3::UeManager'])
    register_Ns3UeRecord_methods(root_module, root_module['ns3::UeRecord'])
    register_Ns3UeRecordCqiFeedback_methods(root_module, root_module['ns3::UeRecord::CqiFeedback'])
    register_Ns3AmcModule_methods(root_module, root_module['ns3::AmcModule'])
    register_Ns3AttributeAccessor_methods(root_module, root_module['ns3::AttributeAccessor'])
    register_Ns3AttributeChecker_methods(root_module, root_module['ns3::AttributeChecker'])
    register_Ns3AttributeValue_methods(root_module, root_module['ns3::AttributeValue'])
    register_Ns3BearerQosParameters_methods(root_module, root_module['ns3::BearerQosParameters'])
    register_Ns3CallbackChecker_methods(root_module, root_module['ns3::CallbackChecker'])
    register_Ns3CallbackImplBase_methods(root_module, root_module['ns3::CallbackImplBase'])
    register_Ns3CallbackValue_methods(root_module, root_module['ns3::CallbackValue'])
    register_Ns3Channel_methods(root_module, root_module['ns3::Channel'])
    register_Ns3ChannelRealization_methods(root_module, root_module['ns3::ChannelRealization'])
    register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, root_module['ns3::ConstantSpeedPropagationDelayModel'])
    register_Ns3DataRateChecker_methods(root_module, root_module['ns3::DataRateChecker'])
    register_Ns3DataRateValue_methods(root_module, root_module['ns3::DataRateValue'])
    register_Ns3DiscreteTimeLossModel_methods(root_module, root_module['ns3::DiscreteTimeLossModel'])
    register_Ns3EmptyAttributeValue_methods(root_module, root_module['ns3::EmptyAttributeValue'])
    register_Ns3EventImpl_methods(root_module, root_module['ns3::EventImpl'])
    register_Ns3IdealControlMessage_methods(root_module, root_module['ns3::IdealControlMessage'])
    register_Ns3Ipv4AddressChecker_methods(root_module, root_module['ns3::Ipv4AddressChecker'])
    register_Ns3Ipv4AddressValue_methods(root_module, root_module['ns3::Ipv4AddressValue'])
    register_Ns3Ipv4MaskChecker_methods(root_module, root_module['ns3::Ipv4MaskChecker'])
    register_Ns3Ipv4MaskValue_methods(root_module, root_module['ns3::Ipv4MaskValue'])
    register_Ns3Ipv6AddressChecker_methods(root_module, root_module['ns3::Ipv6AddressChecker'])
    register_Ns3Ipv6AddressValue_methods(root_module, root_module['ns3::Ipv6AddressValue'])
    register_Ns3Ipv6PrefixChecker_methods(root_module, root_module['ns3::Ipv6PrefixChecker'])
    register_Ns3Ipv6PrefixValue_methods(root_module, root_module['ns3::Ipv6PrefixValue'])
    register_Ns3JakesFadingLossModel_methods(root_module, root_module['ns3::JakesFadingLossModel'])
    register_Ns3LteMacQueue_methods(root_module, root_module['ns3::LteMacQueue'])
    register_Ns3LtePhy_methods(root_module, root_module['ns3::LtePhy'])
    register_Ns3LtePropagationLossModel_methods(root_module, root_module['ns3::LtePropagationLossModel'])
    register_Ns3LteSpectrumPhy_methods(root_module, root_module['ns3::LteSpectrumPhy'])
    register_Ns3Mac48AddressChecker_methods(root_module, root_module['ns3::Mac48AddressChecker'])
    register_Ns3Mac48AddressValue_methods(root_module, root_module['ns3::Mac48AddressValue'])
    register_Ns3MacEntity_methods(root_module, root_module['ns3::MacEntity'])
    register_Ns3MobilityModel_methods(root_module, root_module['ns3::MobilityModel'])
    register_Ns3NetDevice_methods(root_module, root_module['ns3::NetDevice'])
    register_Ns3NixVector_methods(root_module, root_module['ns3::NixVector'])
    register_Ns3Node_methods(root_module, root_module['ns3::Node'])
    register_Ns3ObjectFactoryChecker_methods(root_module, root_module['ns3::ObjectFactoryChecker'])
    register_Ns3ObjectFactoryValue_methods(root_module, root_module['ns3::ObjectFactoryValue'])
    register_Ns3Packet_methods(root_module, root_module['ns3::Packet'])
    register_Ns3PathLossModel_methods(root_module, root_module['ns3::PathLossModel'])
    register_Ns3PdcchMapIdealControlMessage_methods(root_module, root_module['ns3::PdcchMapIdealControlMessage'])
    register_Ns3PdcchMapIdealControlMessageIdealPdcchRecord_methods(root_module, root_module['ns3::PdcchMapIdealControlMessage::IdealPdcchRecord'])
    register_Ns3PenetrationLossModel_methods(root_module, root_module['ns3::PenetrationLossModel'])
    register_Ns3RandomVariableChecker_methods(root_module, root_module['ns3::RandomVariableChecker'])
    register_Ns3RandomVariableValue_methods(root_module, root_module['ns3::RandomVariableValue'])
    register_Ns3ShadowingLossModel_methods(root_module, root_module['ns3::ShadowingLossModel'])
    register_Ns3SpectrumChannel_methods(root_module, root_module['ns3::SpectrumChannel'])
    register_Ns3TimeChecker_methods(root_module, root_module['ns3::TimeChecker'])
    register_Ns3TimeValue_methods(root_module, root_module['ns3::TimeValue'])
    register_Ns3TypeIdChecker_methods(root_module, root_module['ns3::TypeIdChecker'])
    register_Ns3TypeIdValue_methods(root_module, root_module['ns3::TypeIdValue'])
    register_Ns3UeLtePhy_methods(root_module, root_module['ns3::UeLtePhy'])
    register_Ns3UeLteSpectrumPhy_methods(root_module, root_module['ns3::UeLteSpectrumPhy'])
    register_Ns3UeMacEntity_methods(root_module, root_module['ns3::UeMacEntity'])
    register_Ns3UintegerValue_methods(root_module, root_module['ns3::UintegerValue'])
    register_Ns3Vector2DChecker_methods(root_module, root_module['ns3::Vector2DChecker'])
    register_Ns3Vector2DValue_methods(root_module, root_module['ns3::Vector2DValue'])
    register_Ns3Vector3DChecker_methods(root_module, root_module['ns3::Vector3DChecker'])
    register_Ns3Vector3DValue_methods(root_module, root_module['ns3::Vector3DValue'])
    register_Ns3AddressChecker_methods(root_module, root_module['ns3::AddressChecker'])
    register_Ns3AddressValue_methods(root_module, root_module['ns3::AddressValue'])
    register_Ns3CqiIdealControlMessage_methods(root_module, root_module['ns3::CqiIdealControlMessage'])
    register_Ns3CqiIdealControlMessageCqiFeedback_methods(root_module, root_module['ns3::CqiIdealControlMessage::CqiFeedback'])
    register_Ns3EnbLtePhy_methods(root_module, root_module['ns3::EnbLtePhy'])
    register_Ns3EnbLteSpectrumPhy_methods(root_module, root_module['ns3::EnbLteSpectrumPhy'])
    register_Ns3EnbMacEntity_methods(root_module, root_module['ns3::EnbMacEntity'])
    register_Ns3LteNetDevice_methods(root_module, root_module['ns3::LteNetDevice'])
    register_Ns3SingleModelSpectrumChannel_methods(root_module, root_module['ns3::SingleModelSpectrumChannel'])
    register_Ns3UeNetDevice_methods(root_module, root_module['ns3::UeNetDevice'])
    register_Ns3EnbNetDevice_methods(root_module, root_module['ns3::EnbNetDevice'])
    return

def register_Ns3Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('!=')
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

def register_Ns3AttributeList_methods(root_module, cls):
    ## attribute-list.h (module 'core'): ns3::AttributeList::AttributeList() [constructor]
    cls.add_constructor([])
    ## attribute-list.h (module 'core'): ns3::AttributeList::AttributeList(ns3::AttributeList const & o) [copy constructor]
    cls.add_constructor([param('ns3::AttributeList const &', 'o')])
    ## attribute-list.h (module 'core'): bool ns3::AttributeList::DeserializeFromString(std::string value) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value')])
    ## attribute-list.h (module 'core'): static ns3::AttributeList * ns3::AttributeList::GetGlobal() [member function]
    cls.add_method('GetGlobal', 
                   'ns3::AttributeList *', 
                   [], 
                   is_static=True)
    ## attribute-list.h (module 'core'): void ns3::AttributeList::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## attribute-list.h (module 'core'): std::string ns3::AttributeList::SerializeToString() const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## attribute-list.h (module 'core'): void ns3::AttributeList::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## attribute-list.h (module 'core'): bool ns3::AttributeList::SetFailSafe(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## attribute-list.h (module 'core'): void ns3::AttributeList::SetWithTid(ns3::TypeId tid, std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetWithTid', 
                   'void', 
                   [param('ns3::TypeId', 'tid'), param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
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

def register_Ns3IpcsClassifierRecord_methods(root_module, cls):
    ## ipcs-classifier-record.h (module 'wimax'): ns3::IpcsClassifierRecord::IpcsClassifierRecord(ns3::IpcsClassifierRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IpcsClassifierRecord const &', 'arg0')])
    ## ipcs-classifier-record.h (module 'wimax'): ns3::IpcsClassifierRecord::IpcsClassifierRecord() [constructor]
    cls.add_constructor([])
    ## ipcs-classifier-record.h (module 'wimax'): ns3::IpcsClassifierRecord::IpcsClassifierRecord(ns3::Ipv4Address srcAddress, ns3::Ipv4Mask srcMask, ns3::Ipv4Address dstAddress, ns3::Ipv4Mask dstMask, uint16_t srcPortLow, uint16_t srcPortHigh, uint16_t dstPortLow, uint16_t dstPortHigh, uint8_t protocol, uint8_t priority) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'srcAddress'), param('ns3::Ipv4Mask', 'srcMask'), param('ns3::Ipv4Address', 'dstAddress'), param('ns3::Ipv4Mask', 'dstMask'), param('uint16_t', 'srcPortLow'), param('uint16_t', 'srcPortHigh'), param('uint16_t', 'dstPortLow'), param('uint16_t', 'dstPortHigh'), param('uint8_t', 'protocol'), param('uint8_t', 'priority')])
    ## ipcs-classifier-record.h (module 'wimax'): ns3::IpcsClassifierRecord::IpcsClassifierRecord(ns3::Tlv tlv) [constructor]
    cls.add_constructor([param('ns3::Tlv', 'tlv')])
    ## ipcs-classifier-record.h (module 'wimax'): void ns3::IpcsClassifierRecord::AddDstAddr(ns3::Ipv4Address dstAddress, ns3::Ipv4Mask dstMask) [member function]
    cls.add_method('AddDstAddr', 
                   'void', 
                   [param('ns3::Ipv4Address', 'dstAddress'), param('ns3::Ipv4Mask', 'dstMask')])
    ## ipcs-classifier-record.h (module 'wimax'): void ns3::IpcsClassifierRecord::AddDstPortRange(uint16_t dstPortLow, uint16_t dstPortHigh) [member function]
    cls.add_method('AddDstPortRange', 
                   'void', 
                   [param('uint16_t', 'dstPortLow'), param('uint16_t', 'dstPortHigh')])
    ## ipcs-classifier-record.h (module 'wimax'): void ns3::IpcsClassifierRecord::AddProtocol(uint8_t proto) [member function]
    cls.add_method('AddProtocol', 
                   'void', 
                   [param('uint8_t', 'proto')])
    ## ipcs-classifier-record.h (module 'wimax'): void ns3::IpcsClassifierRecord::AddSrcAddr(ns3::Ipv4Address srcAddress, ns3::Ipv4Mask srcMask) [member function]
    cls.add_method('AddSrcAddr', 
                   'void', 
                   [param('ns3::Ipv4Address', 'srcAddress'), param('ns3::Ipv4Mask', 'srcMask')])
    ## ipcs-classifier-record.h (module 'wimax'): void ns3::IpcsClassifierRecord::AddSrcPortRange(uint16_t srcPortLow, uint16_t srcPortHigh) [member function]
    cls.add_method('AddSrcPortRange', 
                   'void', 
                   [param('uint16_t', 'srcPortLow'), param('uint16_t', 'srcPortHigh')])
    ## ipcs-classifier-record.h (module 'wimax'): bool ns3::IpcsClassifierRecord::CheckMatch(ns3::Ipv4Address srcAddress, ns3::Ipv4Address dstAddress, uint16_t srcPort, uint16_t dstPort, uint8_t proto) const [member function]
    cls.add_method('CheckMatch', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'srcAddress'), param('ns3::Ipv4Address', 'dstAddress'), param('uint16_t', 'srcPort'), param('uint16_t', 'dstPort'), param('uint8_t', 'proto')], 
                   is_const=True)
    ## ipcs-classifier-record.h (module 'wimax'): uint16_t ns3::IpcsClassifierRecord::GetCid() const [member function]
    cls.add_method('GetCid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipcs-classifier-record.h (module 'wimax'): uint16_t ns3::IpcsClassifierRecord::GetIndex() const [member function]
    cls.add_method('GetIndex', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipcs-classifier-record.h (module 'wimax'): uint8_t ns3::IpcsClassifierRecord::GetPriority() const [member function]
    cls.add_method('GetPriority', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipcs-classifier-record.h (module 'wimax'): void ns3::IpcsClassifierRecord::SetCid(uint16_t cid) [member function]
    cls.add_method('SetCid', 
                   'void', 
                   [param('uint16_t', 'cid')])
    ## ipcs-classifier-record.h (module 'wimax'): void ns3::IpcsClassifierRecord::SetIndex(uint16_t index) [member function]
    cls.add_method('SetIndex', 
                   'void', 
                   [param('uint16_t', 'index')])
    ## ipcs-classifier-record.h (module 'wimax'): void ns3::IpcsClassifierRecord::SetPriority(uint8_t prio) [member function]
    cls.add_method('SetPriority', 
                   'void', 
                   [param('uint8_t', 'prio')])
    ## ipcs-classifier-record.h (module 'wimax'): ns3::Tlv ns3::IpcsClassifierRecord::ToTlv() const [member function]
    cls.add_method('ToTlv', 
                   'ns3::Tlv', 
                   [], 
                   is_const=True)
    return

def register_Ns3Ipv4Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('!=')
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
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('!=')
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
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsLinkLocal() const [member function]
    cls.add_method('IsLinkLocal', 
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

def register_Ns3LogComponent_methods(root_module, cls):
    ## log.h (module 'core'): ns3::LogComponent::LogComponent(ns3::LogComponent const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LogComponent const &', 'arg0')])
    ## log.h (module 'core'): ns3::LogComponent::LogComponent(char const * name) [constructor]
    cls.add_constructor([param('char const *', 'name')])
    ## log.h (module 'core'): void ns3::LogComponent::Disable(ns3::LogLevel level) [member function]
    cls.add_method('Disable', 
                   'void', 
                   [param('ns3::LogLevel', 'level')])
    ## log.h (module 'core'): void ns3::LogComponent::Enable(ns3::LogLevel level) [member function]
    cls.add_method('Enable', 
                   'void', 
                   [param('ns3::LogLevel', 'level')])
    ## log.h (module 'core'): void ns3::LogComponent::EnvVarCheck(char const * name) [member function]
    cls.add_method('EnvVarCheck', 
                   'void', 
                   [param('char const *', 'name')])
    ## log.h (module 'core'): bool ns3::LogComponent::IsEnabled(ns3::LogLevel level) const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [param('ns3::LogLevel', 'level')], 
                   is_const=True)
    ## log.h (module 'core'): bool ns3::LogComponent::IsNoneEnabled() const [member function]
    cls.add_method('IsNoneEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## log.h (module 'core'): char const * ns3::LogComponent::Name() const [member function]
    cls.add_method('Name', 
                   'char const *', 
                   [], 
                   is_const=True)
    return

def register_Ns3LteHelper_methods(root_module, cls):
    ## lte-helper.h (module 'lte'): ns3::LteHelper::LteHelper(ns3::LteHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteHelper const &', 'arg0')])
    ## lte-helper.h (module 'lte'): ns3::LteHelper::LteHelper() [constructor]
    cls.add_constructor([])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::AddDownlinkChannelRealization(ns3::Ptr<ns3::MobilityModel> enbMobility, ns3::Ptr<ns3::MobilityModel> ueMobility, ns3::Ptr<ns3::LtePhy> phy) [member function]
    cls.add_method('AddDownlinkChannelRealization', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'enbMobility'), param('ns3::Ptr< ns3::MobilityModel >', 'ueMobility'), param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::AddMobility(ns3::Ptr<ns3::LtePhy> phy, ns3::Ptr<ns3::MobilityModel> m) [member function]
    cls.add_method('AddMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::LtePhy >', 'phy'), param('ns3::Ptr< ns3::MobilityModel >', 'm')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::EnableLogComponents() [member function]
    cls.add_method('EnableLogComponents', 
                   'void', 
                   [])
    ## lte-helper.h (module 'lte'): ns3::NetDeviceContainer ns3::LteHelper::Install(ns3::NodeContainer c, ns3::LteHelper::NetDeviceType type) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c'), param('ns3::LteHelper::NetDeviceType', 'type')])
    ## lte-helper.h (module 'lte'): void ns3::LteHelper::RegisterUeToTheEnb(ns3::Ptr<ns3::UeNetDevice> ue, ns3::Ptr<ns3::EnbNetDevice> enb) [member function]
    cls.add_method('RegisterUeToTheEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue'), param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    return

def register_Ns3LteSpectrumValueHelper_methods(root_module, cls):
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::LteSpectrumValueHelper::LteSpectrumValueHelper() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::LteSpectrumValueHelper::LteSpectrumValueHelper(ns3::LteSpectrumValueHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteSpectrumValueHelper const &', 'arg0')])
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateDownlinkNoisePowerSpectralDensity() [member function]
    cls.add_method('CreateDownlinkNoisePowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [])
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateDownlinkTxPowerSpectralDensity(double powerTx, std::vector<int, std::allocator<int> > channels) [member function]
    cls.add_method('CreateDownlinkTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'powerTx'), param('std::vector< int >', 'channels')])
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateUplinkNoisePowerSpectralDensity() [member function]
    cls.add_method('CreateUplinkNoisePowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [])
    ## lte-spectrum-value-helper.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::LteSpectrumValueHelper::CreateUplinkTxPowerSpectralDensity(double powerTx, std::vector<int, std::allocator<int> > channels) [member function]
    cls.add_method('CreateUplinkTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('double', 'powerTx'), param('std::vector< int >', 'channels')])
    return

def register_Ns3Mac48Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('!=')
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
    ## object-base.h (module 'core'): void ns3::ObjectBase::ConstructSelf(ns3::AttributeList const & attributes) [member function]
    cls.add_method('ConstructSelf', 
                   'void', 
                   [param('ns3::AttributeList const &', 'attributes')], 
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
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::Set(ns3::AttributeList const & list) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::AttributeList const &', 'list')])
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

def register_Ns3RandomVariable_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## random-variable.h (module 'core'): ns3::RandomVariable::RandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::RandomVariable::RandomVariable(ns3::RandomVariable const & o) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariable const &', 'o')])
    ## random-variable.h (module 'core'): uint32_t ns3::RandomVariable::GetInteger() const [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): double ns3::RandomVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    return

def register_Ns3SeedManager_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::SeedManager::SeedManager() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::SeedManager::SeedManager(ns3::SeedManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SeedManager const &', 'arg0')])
    ## random-variable.h (module 'core'): static bool ns3::SeedManager::CheckSeed(uint32_t seed) [member function]
    cls.add_method('CheckSeed', 
                   'bool', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    ## random-variable.h (module 'core'): static uint32_t ns3::SeedManager::GetRun() [member function]
    cls.add_method('GetRun', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## random-variable.h (module 'core'): static uint32_t ns3::SeedManager::GetSeed() [member function]
    cls.add_method('GetSeed', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## random-variable.h (module 'core'): static void ns3::SeedManager::SetRun(uint32_t run) [member function]
    cls.add_method('SetRun', 
                   'void', 
                   [param('uint32_t', 'run')], 
                   is_static=True)
    ## random-variable.h (module 'core'): static void ns3::SeedManager::SetSeed(uint32_t seed) [member function]
    cls.add_method('SetSeed', 
                   'void', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    return

def register_Ns3SequentialVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(ns3::SequentialVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SequentialVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(double f, double l, double i=1, uint32_t c=1) [constructor]
    cls.add_constructor([param('double', 'f'), param('double', 'l'), param('double', 'i', default_value='1'), param('uint32_t', 'c', default_value='1')])
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(double f, double l, ns3::RandomVariable const & i, uint32_t c=1) [constructor]
    cls.add_constructor([param('double', 'f'), param('double', 'l'), param('ns3::RandomVariable const &', 'i'), param('uint32_t', 'c', default_value='1')])
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

def register_Ns3SpectrumType_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## spectrum-type.h (module 'spectrum'): ns3::SpectrumType::SpectrumType(ns3::SpectrumType const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumType const &', 'arg0')])
    ## spectrum-type.h (module 'spectrum'): std::string ns3::SpectrumType::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## spectrum-type.h (module 'spectrum'): uint32_t ns3::SpectrumType::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3SpectrumTypeFactory_methods(root_module, cls):
    ## spectrum-type.h (module 'spectrum'): ns3::SpectrumTypeFactory::SpectrumTypeFactory() [constructor]
    cls.add_constructor([])
    ## spectrum-type.h (module 'spectrum'): ns3::SpectrumTypeFactory::SpectrumTypeFactory(ns3::SpectrumTypeFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumTypeFactory const &', 'arg0')])
    ## spectrum-type.h (module 'spectrum'): static ns3::SpectrumType ns3::SpectrumTypeFactory::Create(std::string name) [member function]
    cls.add_method('Create', 
                   'ns3::SpectrumType', 
                   [param('std::string', 'name')], 
                   is_static=True)
    ## spectrum-type.h (module 'spectrum'): static std::string ns3::SpectrumTypeFactory::GetNameByUid(uint32_t uid) [member function]
    cls.add_method('GetNameByUid', 
                   'std::string', 
                   [param('uint32_t', 'uid')], 
                   is_static=True)
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

def register_Ns3TlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::TlvValue::TlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::TlvValue::TlvValue(ns3::TlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::TlvValue * ns3::TlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::TlvValue *', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::TlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLen) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLen')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::TlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::TlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3TosTlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::TosTlvValue::TosTlvValue(ns3::TosTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TosTlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::TosTlvValue::TosTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::TosTlvValue::TosTlvValue(uint8_t arg0, uint8_t arg1, uint8_t arg2) [constructor]
    cls.add_constructor([param('uint8_t', 'arg0'), param('uint8_t', 'arg1'), param('uint8_t', 'arg2')])
    ## wimax-tlv.h (module 'wimax'): ns3::TosTlvValue * ns3::TosTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::TosTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::TosTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint8_t ns3::TosTlvValue::GetHigh() const [member function]
    cls.add_method('GetHigh', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint8_t ns3::TosTlvValue::GetLow() const [member function]
    cls.add_method('GetLow', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint8_t ns3::TosTlvValue::GetMask() const [member function]
    cls.add_method('GetMask', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::TosTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::TosTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3TriangularVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::TriangularVariable::TriangularVariable(ns3::TriangularVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TriangularVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::TriangularVariable::TriangularVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::TriangularVariable::TriangularVariable(double s, double l, double mean) [constructor]
    cls.add_constructor([param('double', 's'), param('double', 'l'), param('double', 'mean')])
    return

def register_Ns3TypeId_methods(root_module, cls):
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('!=')
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
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::AddTraceSource(std::string name, std::string help, ns3::Ptr<const ns3::TraceSourceAccessor> accessor) [member function]
    cls.add_method('AddTraceSource', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::Ptr< ns3::TraceSourceAccessor const >', 'accessor')])
    ## type-id.h (module 'core'): ns3::Ptr<ns3::AttributeAccessor const> ns3::TypeId::GetAttributeAccessor(uint32_t i) const [member function]
    cls.add_method('GetAttributeAccessor', 
                   'ns3::Ptr< ns3::AttributeAccessor const >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): ns3::Ptr<ns3::AttributeChecker const> ns3::TypeId::GetAttributeChecker(uint32_t i) const [member function]
    cls.add_method('GetAttributeChecker', 
                   'ns3::Ptr< ns3::AttributeChecker const >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetAttributeFlags(uint32_t i) const [member function]
    cls.add_method('GetAttributeFlags', 
                   'uint32_t', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetAttributeFullName(uint32_t i) const [member function]
    cls.add_method('GetAttributeFullName', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetAttributeHelp(uint32_t i) const [member function]
    cls.add_method('GetAttributeHelp', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): ns3::Ptr<ns3::AttributeValue const> ns3::TypeId::GetAttributeInitialValue(uint32_t i) const [member function]
    cls.add_method('GetAttributeInitialValue', 
                   'ns3::Ptr< ns3::AttributeValue const >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetAttributeN() const [member function]
    cls.add_method('GetAttributeN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetAttributeName(uint32_t i) const [member function]
    cls.add_method('GetAttributeName', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
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
    ## type-id.h (module 'core'): ns3::Ptr<const ns3::TraceSourceAccessor> ns3::TypeId::GetTraceSourceAccessor(uint32_t i) const [member function]
    cls.add_method('GetTraceSourceAccessor', 
                   'ns3::Ptr< ns3::TraceSourceAccessor const >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetTraceSourceHelp(uint32_t i) const [member function]
    cls.add_method('GetTraceSourceHelp', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetTraceSourceN() const [member function]
    cls.add_method('GetTraceSourceN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetTraceSourceName(uint32_t i) const [member function]
    cls.add_method('GetTraceSourceName', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
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
    ## type-id.h (module 'core'): static bool ns3::TypeId::LookupAttributeByFullName(std::string fullName, ns3::TypeId::AttributeInfo * info) [member function]
    cls.add_method('LookupAttributeByFullName', 
                   'bool', 
                   [param('std::string', 'fullName'), param('ns3::TypeId::AttributeInfo *', 'info')], 
                   is_static=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::LookupAttributeByName(std::string name, ns3::TypeId::AttributeInfo * info) const [member function]
    cls.add_method('LookupAttributeByName', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::TypeId::AttributeInfo *', 'info', transfer_ownership=False)], 
                   is_const=True)
    ## type-id.h (module 'core'): static ns3::TypeId ns3::TypeId::LookupByName(std::string name) [member function]
    cls.add_method('LookupByName', 
                   'ns3::TypeId', 
                   [param('std::string', 'name')], 
                   is_static=True)
    ## type-id.h (module 'core'): ns3::Ptr<const ns3::TraceSourceAccessor> ns3::TypeId::LookupTraceSourceByName(std::string name) const [member function]
    cls.add_method('LookupTraceSourceByName', 
                   'ns3::Ptr< ns3::TraceSourceAccessor const >', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::MustHideFromDocumentation() const [member function]
    cls.add_method('MustHideFromDocumentation', 
                   'bool', 
                   [], 
                   is_const=True)
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

def register_Ns3TypeIdAttributeInfo_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInfo::AttributeInfo() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInfo::AttributeInfo(ns3::TypeId::AttributeInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeId::AttributeInfo const &', 'arg0')])
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInfo::accessor [variable]
    cls.add_instance_attribute('accessor', 'ns3::Ptr< ns3::AttributeAccessor const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInfo::checker [variable]
    cls.add_instance_attribute('checker', 'ns3::Ptr< ns3::AttributeChecker const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInfo::flags [variable]
    cls.add_instance_attribute('flags', 'uint32_t', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInfo::initialValue [variable]
    cls.add_instance_attribute('initialValue', 'ns3::Ptr< ns3::AttributeValue const >', is_const=False)
    return

def register_Ns3U16TlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::U16TlvValue::U16TlvValue(ns3::U16TlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::U16TlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::U16TlvValue::U16TlvValue(uint16_t value) [constructor]
    cls.add_constructor([param('uint16_t', 'value')])
    ## wimax-tlv.h (module 'wimax'): ns3::U16TlvValue::U16TlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::U16TlvValue * ns3::U16TlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::U16TlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U16TlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLen) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLen')], 
                   is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U16TlvValue::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U16TlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint16_t ns3::U16TlvValue::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::U16TlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3U32TlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::U32TlvValue::U32TlvValue(ns3::U32TlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::U32TlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::U32TlvValue::U32TlvValue(uint32_t value) [constructor]
    cls.add_constructor([param('uint32_t', 'value')])
    ## wimax-tlv.h (module 'wimax'): ns3::U32TlvValue::U32TlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::U32TlvValue * ns3::U32TlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::U32TlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U32TlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLen) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLen')], 
                   is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U32TlvValue::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U32TlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U32TlvValue::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::U32TlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3U8TlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::U8TlvValue::U8TlvValue(ns3::U8TlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::U8TlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::U8TlvValue::U8TlvValue(uint8_t value) [constructor]
    cls.add_constructor([param('uint8_t', 'value')])
    ## wimax-tlv.h (module 'wimax'): ns3::U8TlvValue::U8TlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::U8TlvValue * ns3::U8TlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::U8TlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U8TlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLen) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLen')], 
                   is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U8TlvValue::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')])
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::U8TlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint8_t ns3::U8TlvValue::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::U8TlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3UniformVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::UniformVariable::UniformVariable(ns3::UniformVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UniformVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::UniformVariable::UniformVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::UniformVariable::UniformVariable(double s, double l) [constructor]
    cls.add_constructor([param('double', 's'), param('double', 'l')])
    ## random-variable.h (module 'core'): uint32_t ns3::UniformVariable::GetInteger(uint32_t s, uint32_t l) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 's'), param('uint32_t', 'l')])
    ## random-variable.h (module 'core'): double ns3::UniformVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): double ns3::UniformVariable::GetValue(double s, double l) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 's'), param('double', 'l')])
    return

def register_Ns3UnsafeAttributeList_methods(root_module, cls):
    ## attribute-list.h (module 'core'): ns3::UnsafeAttributeList::UnsafeAttributeList() [constructor]
    cls.add_constructor([])
    ## attribute-list.h (module 'core'): ns3::UnsafeAttributeList::UnsafeAttributeList(ns3::UnsafeAttributeList const & o) [copy constructor]
    cls.add_constructor([param('ns3::UnsafeAttributeList const &', 'o')])
    ## attribute-list.h (module 'core'): ns3::AttributeList ns3::UnsafeAttributeList::GetSafe(std::string name) const [member function]
    cls.add_method('GetSafe', 
                   'ns3::AttributeList', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## attribute-list.h (module 'core'): void ns3::UnsafeAttributeList::Set(std::string name, ns3::AttributeValue const & param) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'param')])
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

def register_Ns3VectorTlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::VectorTlvValue::VectorTlvValue(ns3::VectorTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VectorTlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::VectorTlvValue::VectorTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): void ns3::VectorTlvValue::Add(ns3::Tlv const & val) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Tlv const &', 'val')])
    ## wimax-tlv.h (module 'wimax'): __gnu_cxx::__normal_iterator<ns3::Tlv* const*,std::vector<ns3::Tlv*, std::allocator<ns3::Tlv*> > > ns3::VectorTlvValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Tlv * const *, std::vector< ns3::Tlv * > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): ns3::VectorTlvValue * ns3::VectorTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::VectorTlvValue *', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::VectorTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_pure_virtual=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): __gnu_cxx::__normal_iterator<ns3::Tlv* const*,std::vector<ns3::Tlv*, std::allocator<ns3::Tlv*> > > ns3::VectorTlvValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Tlv * const *, std::vector< ns3::Tlv * > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::VectorTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::VectorTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3WeibullVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable(ns3::WeibullVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WeibullVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable(double m, double s) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's')])
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable(double m, double s, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's'), param('double', 'b')])
    return

def register_Ns3ZetaVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ZetaVariable::ZetaVariable(ns3::ZetaVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ZetaVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ZetaVariable::ZetaVariable(double alpha) [constructor]
    cls.add_constructor([param('double', 'alpha')])
    ## random-variable.h (module 'core'): ns3::ZetaVariable::ZetaVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3ZipfVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ZipfVariable::ZipfVariable(ns3::ZipfVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ZipfVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ZipfVariable::ZipfVariable(long int N, double alpha) [constructor]
    cls.add_constructor([param('long int', 'N'), param('double', 'alpha')])
    ## random-variable.h (module 'core'): ns3::ZipfVariable::ZipfVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3Empty_methods(root_module, cls):
    ## empty.h (module 'core'): ns3::empty::empty() [constructor]
    cls.add_constructor([])
    ## empty.h (module 'core'): ns3::empty::empty(ns3::empty const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::empty const &', 'arg0')])
    return

def register_Ns3Int64x64_t_methods(root_module, cls):
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
    cls.add_binary_comparison_operator('!=')
    cls.add_inplace_numeric_operator('*=', param('ns3::int64x64_t const &', 'right'))
    cls.add_inplace_numeric_operator('+=', param('ns3::int64x64_t const &', 'right'))
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

def register_Ns3ClassificationRuleVectorTlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::ClassificationRuleVectorTlvValue::ClassificationRuleVectorTlvValue(ns3::ClassificationRuleVectorTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ClassificationRuleVectorTlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::ClassificationRuleVectorTlvValue::ClassificationRuleVectorTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::ClassificationRuleVectorTlvValue * ns3::ClassificationRuleVectorTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::ClassificationRuleVectorTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::ClassificationRuleVectorTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    return

def register_Ns3ConstantVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ConstantVariable::ConstantVariable(ns3::ConstantVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ConstantVariable::ConstantVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::ConstantVariable::ConstantVariable(double c) [constructor]
    cls.add_constructor([param('double', 'c')])
    ## random-variable.h (module 'core'): void ns3::ConstantVariable::SetConstant(double c) [member function]
    cls.add_method('SetConstant', 
                   'void', 
                   [param('double', 'c')])
    return

def register_Ns3CsParamVectorTlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::CsParamVectorTlvValue::CsParamVectorTlvValue(ns3::CsParamVectorTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CsParamVectorTlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::CsParamVectorTlvValue::CsParamVectorTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::CsParamVectorTlvValue * ns3::CsParamVectorTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::CsParamVectorTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::CsParamVectorTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    return

def register_Ns3DeterministicVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::DeterministicVariable::DeterministicVariable(ns3::DeterministicVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DeterministicVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::DeterministicVariable::DeterministicVariable(double * d, uint32_t c) [constructor]
    cls.add_constructor([param('double *', 'd'), param('uint32_t', 'c')])
    return

def register_Ns3EmpiricalVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::EmpiricalVariable::EmpiricalVariable(ns3::EmpiricalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EmpiricalVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::EmpiricalVariable::EmpiricalVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): void ns3::EmpiricalVariable::CDF(double v, double c) [member function]
    cls.add_method('CDF', 
                   'void', 
                   [param('double', 'v'), param('double', 'c')])
    return

def register_Ns3ErlangVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ErlangVariable::ErlangVariable(ns3::ErlangVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErlangVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ErlangVariable::ErlangVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::ErlangVariable::ErlangVariable(unsigned int k, double lambda) [constructor]
    cls.add_constructor([param('unsigned int', 'k'), param('double', 'lambda')])
    ## random-variable.h (module 'core'): double ns3::ErlangVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): double ns3::ErlangVariable::GetValue(unsigned int k, double lambda) const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('unsigned int', 'k'), param('double', 'lambda')], 
                   is_const=True)
    return

def register_Ns3ExponentialVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ExponentialVariable::ExponentialVariable(ns3::ExponentialVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ExponentialVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ExponentialVariable::ExponentialVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::ExponentialVariable::ExponentialVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h (module 'core'): ns3::ExponentialVariable::ExponentialVariable(double m, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'b')])
    return

def register_Ns3GammaVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::GammaVariable::GammaVariable(ns3::GammaVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GammaVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::GammaVariable::GammaVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::GammaVariable::GammaVariable(double alpha, double beta) [constructor]
    cls.add_constructor([param('double', 'alpha'), param('double', 'beta')])
    ## random-variable.h (module 'core'): double ns3::GammaVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): double ns3::GammaVariable::GetValue(double alpha, double beta) const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'alpha'), param('double', 'beta')], 
                   is_const=True)
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

def register_Ns3IntEmpiricalVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::IntEmpiricalVariable::IntEmpiricalVariable(ns3::IntEmpiricalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntEmpiricalVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::IntEmpiricalVariable::IntEmpiricalVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3Ipv4AddressTlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue::Ipv4AddressTlvValue(ns3::Ipv4AddressTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressTlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue::Ipv4AddressTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): void ns3::Ipv4AddressTlvValue::Add(ns3::Ipv4Address address, ns3::Ipv4Mask Mask) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ipv4Address', 'address'), param('ns3::Ipv4Mask', 'Mask')])
    ## wimax-tlv.h (module 'wimax'): __gnu_cxx::__normal_iterator<const ns3::Ipv4AddressTlvValue::ipv4Addr*,std::vector<ns3::Ipv4AddressTlvValue::ipv4Addr, std::allocator<ns3::Ipv4AddressTlvValue::ipv4Addr> > > ns3::Ipv4AddressTlvValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ipv4AddressTlvValue::ipv4Addr const *, std::vector< ns3::Ipv4AddressTlvValue::ipv4Addr > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue * ns3::Ipv4AddressTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ipv4AddressTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::Ipv4AddressTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): __gnu_cxx::__normal_iterator<const ns3::Ipv4AddressTlvValue::ipv4Addr*,std::vector<ns3::Ipv4AddressTlvValue::ipv4Addr, std::allocator<ns3::Ipv4AddressTlvValue::ipv4Addr> > > ns3::Ipv4AddressTlvValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ipv4AddressTlvValue::ipv4Addr const *, std::vector< ns3::Ipv4AddressTlvValue::ipv4Addr > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::Ipv4AddressTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::Ipv4AddressTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Ipv4AddressTlvValueIpv4Addr_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue::ipv4Addr::ipv4Addr() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue::ipv4Addr::ipv4Addr(ns3::Ipv4AddressTlvValue::ipv4Addr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressTlvValue::ipv4Addr const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue::ipv4Addr::Address [variable]
    cls.add_instance_attribute('Address', 'ns3::Ipv4Address', is_const=False)
    ## wimax-tlv.h (module 'wimax'): ns3::Ipv4AddressTlvValue::ipv4Addr::Mask [variable]
    cls.add_instance_attribute('Mask', 'ns3::Ipv4Mask', is_const=False)
    return

def register_Ns3LogNormalVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::LogNormalVariable::LogNormalVariable(ns3::LogNormalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LogNormalVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::LogNormalVariable::LogNormalVariable(double mu, double sigma) [constructor]
    cls.add_constructor([param('double', 'mu'), param('double', 'sigma')])
    return

def register_Ns3LteMacHeader_methods(root_module, cls):
    ## lte-mac-header.h (module 'lte'): ns3::LteMacHeader::LteMacHeader() [constructor]
    cls.add_constructor([])
    ## lte-mac-header.h (module 'lte'): ns3::LteMacHeader::LteMacHeader(ns3::LteMacHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMacHeader const &', 'arg0')])
    ## lte-mac-header.h (module 'lte'): uint32_t ns3::LteMacHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## lte-mac-header.h (module 'lte'): ns3::Mac48Address ns3::LteMacHeader::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## lte-mac-header.h (module 'lte'): ns3::TypeId ns3::LteMacHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-mac-header.h (module 'lte'): uint32_t ns3::LteMacHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-mac-header.h (module 'lte'): ns3::Mac48Address ns3::LteMacHeader::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## lte-mac-header.h (module 'lte'): static ns3::TypeId ns3::LteMacHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-mac-header.h (module 'lte'): void ns3::LteMacHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## lte-mac-header.h (module 'lte'): void ns3::LteMacHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## lte-mac-header.h (module 'lte'): void ns3::LteMacHeader::SetDestination(ns3::Mac48Address destination) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination')])
    ## lte-mac-header.h (module 'lte'): void ns3::LteMacHeader::SetSource(ns3::Mac48Address source) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Mac48Address', 'source')])
    return

def register_Ns3NormalVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::NormalVariable::NormalVariable(ns3::NormalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NormalVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::NormalVariable::NormalVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::NormalVariable::NormalVariable(double m, double v) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'v')])
    ## random-variable.h (module 'core'): ns3::NormalVariable::NormalVariable(double m, double v, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'v'), param('double', 'b')])
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

def register_Ns3PacketScheduler_methods(root_module, cls):
    ## packet-scheduler.h (module 'lte'): ns3::PacketScheduler::PacketScheduler(ns3::PacketScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketScheduler const &', 'arg0')])
    ## packet-scheduler.h (module 'lte'): ns3::PacketScheduler::PacketScheduler() [constructor]
    cls.add_constructor([])
    ## packet-scheduler.h (module 'lte'): ns3::PacketScheduler::PacketScheduler(ns3::Ptr<ns3::EnbNetDevice> enb) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## packet-scheduler.h (module 'lte'): void ns3::PacketScheduler::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## packet-scheduler.h (module 'lte'): void ns3::PacketScheduler::DoRunPacketScheduler() [member function]
    cls.add_method('DoRunPacketScheduler', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## packet-scheduler.h (module 'lte'): ns3::Ptr<ns3::EnbNetDevice> ns3::PacketScheduler::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::EnbNetDevice >', 
                   [])
    ## packet-scheduler.h (module 'lte'): ns3::Ptr<ns3::MacEntity> ns3::PacketScheduler::GetMacEntity() [member function]
    cls.add_method('GetMacEntity', 
                   'ns3::Ptr< ns3::MacEntity >', 
                   [])
    ## packet-scheduler.h (module 'lte'): static ns3::TypeId ns3::PacketScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## packet-scheduler.h (module 'lte'): void ns3::PacketScheduler::RunPacketScheduler() [member function]
    cls.add_method('RunPacketScheduler', 
                   'void', 
                   [])
    ## packet-scheduler.h (module 'lte'): void ns3::PacketScheduler::SetDevice(ns3::Ptr<ns3::EnbNetDevice> enb) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## packet-scheduler.h (module 'lte'): void ns3::PacketScheduler::SetMacEntity(ns3::Ptr<ns3::MacEntity> mac) [member function]
    cls.add_method('SetMacEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::MacEntity >', 'mac')])
    return

def register_Ns3ParetoVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(ns3::ParetoVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ParetoVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(double m, double s) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(double m, double s, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's'), param('double', 'b')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(std::pair<double,double> params) [constructor]
    cls.add_constructor([param('std::pair< double, double >', 'params')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(std::pair<double,double> params, double b) [constructor]
    cls.add_constructor([param('std::pair< double, double >', 'params'), param('double', 'b')])
    return

def register_Ns3PortRangeTlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue::PortRangeTlvValue(ns3::PortRangeTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PortRangeTlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue::PortRangeTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): void ns3::PortRangeTlvValue::Add(uint16_t portLow, uint16_t portHigh) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('uint16_t', 'portLow'), param('uint16_t', 'portHigh')])
    ## wimax-tlv.h (module 'wimax'): __gnu_cxx::__normal_iterator<const ns3::PortRangeTlvValue::PortRange*,std::vector<ns3::PortRangeTlvValue::PortRange, std::allocator<ns3::PortRangeTlvValue::PortRange> > > ns3::PortRangeTlvValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::PortRangeTlvValue::PortRange const *, std::vector< ns3::PortRangeTlvValue::PortRange > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue * ns3::PortRangeTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::PortRangeTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::PortRangeTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): __gnu_cxx::__normal_iterator<const ns3::PortRangeTlvValue::PortRange*,std::vector<ns3::PortRangeTlvValue::PortRange, std::allocator<ns3::PortRangeTlvValue::PortRange> > > ns3::PortRangeTlvValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::PortRangeTlvValue::PortRange const *, std::vector< ns3::PortRangeTlvValue::PortRange > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::PortRangeTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::PortRangeTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3PortRangeTlvValuePortRange_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue::PortRange::PortRange() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue::PortRange::PortRange(ns3::PortRangeTlvValue::PortRange const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PortRangeTlvValue::PortRange const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue::PortRange::PortHigh [variable]
    cls.add_instance_attribute('PortHigh', 'uint16_t', is_const=False)
    ## wimax-tlv.h (module 'wimax'): ns3::PortRangeTlvValue::PortRange::PortLow [variable]
    cls.add_instance_attribute('PortLow', 'uint16_t', is_const=False)
    return

def register_Ns3PropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h (module 'propagation'): ns3::PropagationDelayModel::PropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h (module 'propagation'): ns3::PropagationDelayModel::PropagationDelayModel(ns3::PropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h (module 'propagation'): ns3::Time ns3::PropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## propagation-delay-model.h (module 'propagation'): static ns3::TypeId ns3::PropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3ProtocolTlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::ProtocolTlvValue::ProtocolTlvValue(ns3::ProtocolTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ProtocolTlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::ProtocolTlvValue::ProtocolTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): void ns3::ProtocolTlvValue::Add(uint8_t protiocol) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('uint8_t', 'protiocol')])
    ## wimax-tlv.h (module 'wimax'): __gnu_cxx::__normal_iterator<const unsigned char*,std::vector<unsigned char, std::allocator<unsigned char> > > ns3::ProtocolTlvValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< unsigned char const *, std::vector< unsigned char > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): ns3::ProtocolTlvValue * ns3::ProtocolTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::ProtocolTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::ProtocolTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): __gnu_cxx::__normal_iterator<const unsigned char*,std::vector<unsigned char, std::allocator<unsigned char> > > ns3::ProtocolTlvValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< unsigned char const *, std::vector< unsigned char > >', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::ProtocolTlvValue::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::ProtocolTlvValue::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3RadioBearerInstance_methods(root_module, cls):
    ## radio-bearer-instance.h (module 'lte'): ns3::RadioBearerInstance::RadioBearerInstance(ns3::RadioBearerInstance const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadioBearerInstance const &', 'arg0')])
    ## radio-bearer-instance.h (module 'lte'): ns3::RadioBearerInstance::RadioBearerInstance() [constructor]
    cls.add_constructor([])
    ## radio-bearer-instance.h (module 'lte'): ns3::Ptr<ns3::Packet> ns3::RadioBearerInstance::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## radio-bearer-instance.h (module 'lte'): ns3::Ptr<ns3::Packet> ns3::RadioBearerInstance::Dequeue(uint32_t availableByte) [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'availableByte')])
    ## radio-bearer-instance.h (module 'lte'): bool ns3::RadioBearerInstance::Enqueue(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    ## radio-bearer-instance.h (module 'lte'): ns3::RadioBearerInstance::BearerDirection ns3::RadioBearerInstance::GetBearerDirection() const [member function]
    cls.add_method('GetBearerDirection', 
                   'ns3::RadioBearerInstance::BearerDirection', 
                   [], 
                   is_const=True)
    ## radio-bearer-instance.h (module 'lte'): ns3::RadioBearerInstance::BearerType ns3::RadioBearerInstance::GetBearerType() const [member function]
    cls.add_method('GetBearerType', 
                   'ns3::RadioBearerInstance::BearerType', 
                   [], 
                   is_const=True)
    ## radio-bearer-instance.h (module 'lte'): ns3::IpcsClassifierRecord * ns3::RadioBearerInstance::GetIpcsClassifierRecord() [member function]
    cls.add_method('GetIpcsClassifierRecord', 
                   'ns3::IpcsClassifierRecord *', 
                   [])
    ## radio-bearer-instance.h (module 'lte'): ns3::Ptr<ns3::BearerQosParameters> ns3::RadioBearerInstance::GetQosParameters() [member function]
    cls.add_method('GetQosParameters', 
                   'ns3::Ptr< ns3::BearerQosParameters >', 
                   [])
    ## radio-bearer-instance.h (module 'lte'): ns3::Ptr<ns3::LteMacQueue> ns3::RadioBearerInstance::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::LteMacQueue >', 
                   [], 
                   is_const=True)
    ## radio-bearer-instance.h (module 'lte'): ns3::Ptr<ns3::RlcEntity> ns3::RadioBearerInstance::GetRlcEntity() [member function]
    cls.add_method('GetRlcEntity', 
                   'ns3::Ptr< ns3::RlcEntity >', 
                   [])
    ## radio-bearer-instance.h (module 'lte'): static ns3::TypeId ns3::RadioBearerInstance::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radio-bearer-instance.h (module 'lte'): bool ns3::RadioBearerInstance::HasPackets() const [member function]
    cls.add_method('HasPackets', 
                   'bool', 
                   [], 
                   is_const=True)
    ## radio-bearer-instance.h (module 'lte'): void ns3::RadioBearerInstance::SetBearerDirection(ns3::RadioBearerInstance::BearerDirection direction) [member function]
    cls.add_method('SetBearerDirection', 
                   'void', 
                   [param('ns3::RadioBearerInstance::BearerDirection', 'direction')])
    ## radio-bearer-instance.h (module 'lte'): void ns3::RadioBearerInstance::SetBearerType(ns3::RadioBearerInstance::BearerType type) [member function]
    cls.add_method('SetBearerType', 
                   'void', 
                   [param('ns3::RadioBearerInstance::BearerType', 'type')])
    ## radio-bearer-instance.h (module 'lte'): void ns3::RadioBearerInstance::SetIpcsClassifierRecord(ns3::IpcsClassifierRecord * c) [member function]
    cls.add_method('SetIpcsClassifierRecord', 
                   'void', 
                   [param('ns3::IpcsClassifierRecord *', 'c')])
    ## radio-bearer-instance.h (module 'lte'): void ns3::RadioBearerInstance::SetQosParameters(ns3::Ptr<ns3::BearerQosParameters> qosParameters) [member function]
    cls.add_method('SetQosParameters', 
                   'void', 
                   [param('ns3::Ptr< ns3::BearerQosParameters >', 'qosParameters')])
    ## radio-bearer-instance.h (module 'lte'): void ns3::RadioBearerInstance::SetRlcEntity(ns3::Ptr<ns3::RlcEntity> rlc) [member function]
    cls.add_method('SetRlcEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::RlcEntity >', 'rlc')])
    ## radio-bearer-instance.h (module 'lte'): void ns3::RadioBearerInstance::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3RandomPropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h (module 'propagation'): ns3::RandomPropagationDelayModel::RandomPropagationDelayModel(ns3::RandomPropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomPropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h (module 'propagation'): ns3::RandomPropagationDelayModel::RandomPropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h (module 'propagation'): ns3::Time ns3::RandomPropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## propagation-delay-model.h (module 'propagation'): static ns3::TypeId ns3::RandomPropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3RlcEntity_methods(root_module, cls):
    ## rlc-entity.h (module 'lte'): ns3::RlcEntity::RlcEntity(ns3::RlcEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RlcEntity const &', 'arg0')])
    ## rlc-entity.h (module 'lte'): ns3::RlcEntity::RlcEntity() [constructor]
    cls.add_constructor([])
    ## rlc-entity.h (module 'lte'): ns3::RlcEntity::RlcEntity(ns3::Ptr<ns3::LteNetDevice> d) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## rlc-entity.h (module 'lte'): ns3::Ptr<ns3::Packet> ns3::RlcEntity::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## rlc-entity.h (module 'lte'): void ns3::RlcEntity::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## rlc-entity.h (module 'lte'): ns3::Ptr<ns3::LteNetDevice> ns3::RlcEntity::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## rlc-entity.h (module 'lte'): ns3::Ptr<ns3::RadioBearerInstance> ns3::RlcEntity::GetRadioBearer() [member function]
    cls.add_method('GetRadioBearer', 
                   'ns3::Ptr< ns3::RadioBearerInstance >', 
                   [])
    ## rlc-entity.h (module 'lte'): static ns3::TypeId ns3::RlcEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rlc-entity.h (module 'lte'): void ns3::RlcEntity::SetDevice(ns3::Ptr<ns3::LteNetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## rlc-entity.h (module 'lte'): void ns3::RlcEntity::SetRadioBearer(ns3::Ptr<ns3::RadioBearerInstance> b) [member function]
    cls.add_method('SetRadioBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'b')])
    return

def register_Ns3RrcEntity_methods(root_module, cls):
    ## rrc-entity.h (module 'lte'): ns3::RrcEntity::RrcEntity(ns3::RrcEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RrcEntity const &', 'arg0')])
    ## rrc-entity.h (module 'lte'): ns3::RrcEntity::RrcEntity() [constructor]
    cls.add_constructor([])
    ## rrc-entity.h (module 'lte'): void ns3::RrcEntity::AddDownlinkGbrBearer(ns3::Ptr<ns3::RadioBearerInstance> bearer) [member function]
    cls.add_method('AddDownlinkGbrBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'bearer')])
    ## rrc-entity.h (module 'lte'): void ns3::RrcEntity::AddDownlinkNgbrBearer(ns3::Ptr<ns3::RadioBearerInstance> bearer) [member function]
    cls.add_method('AddDownlinkNgbrBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'bearer')])
    ## rrc-entity.h (module 'lte'): void ns3::RrcEntity::AddUplinkGbrBearer(ns3::Ptr<ns3::RadioBearerInstance> bearer) [member function]
    cls.add_method('AddUplinkGbrBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'bearer')])
    ## rrc-entity.h (module 'lte'): void ns3::RrcEntity::AddUplinkNgbrBearer(ns3::Ptr<ns3::RadioBearerInstance> bearer) [member function]
    cls.add_method('AddUplinkNgbrBearer', 
                   'void', 
                   [param('ns3::Ptr< ns3::RadioBearerInstance >', 'bearer')])
    ## rrc-entity.h (module 'lte'): ns3::Ptr<ns3::RadioBearerInstance> ns3::RrcEntity::Classify(ns3::Ptr<ns3::Packet> p) const [member function]
    cls.add_method('Classify', 
                   'ns3::Ptr< ns3::RadioBearerInstance >', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_const=True)
    ## rrc-entity.h (module 'lte'): void ns3::RrcEntity::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## rrc-entity.h (module 'lte'): ns3::Ptr<ns3::RadioBearerInstance> ns3::RrcEntity::GetDefaultBearer() [member function]
    cls.add_method('GetDefaultBearer', 
                   'ns3::Ptr< ns3::RadioBearerInstance >', 
                   [])
    ## rrc-entity.h (module 'lte'): std::vector<ns3::Ptr<ns3::RadioBearerInstance>,std::allocator<ns3::Ptr<ns3::RadioBearerInstance> > > * ns3::RrcEntity::GetDownlinkGbrBearers() const [member function]
    cls.add_method('GetDownlinkGbrBearers', 
                   'std::vector< ns3::Ptr< ns3::RadioBearerInstance > > *', 
                   [], 
                   is_const=True)
    ## rrc-entity.h (module 'lte'): std::vector<ns3::Ptr<ns3::RadioBearerInstance>,std::allocator<ns3::Ptr<ns3::RadioBearerInstance> > > * ns3::RrcEntity::GetDownlinkNgbrBearers() const [member function]
    cls.add_method('GetDownlinkNgbrBearers', 
                   'std::vector< ns3::Ptr< ns3::RadioBearerInstance > > *', 
                   [], 
                   is_const=True)
    ## rrc-entity.h (module 'lte'): static ns3::TypeId ns3::RrcEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## rrc-entity.h (module 'lte'): std::vector<ns3::Ptr<ns3::RadioBearerInstance>,std::allocator<ns3::Ptr<ns3::RadioBearerInstance> > > * ns3::RrcEntity::GetUplinkGbrBearers() const [member function]
    cls.add_method('GetUplinkGbrBearers', 
                   'std::vector< ns3::Ptr< ns3::RadioBearerInstance > > *', 
                   [], 
                   is_const=True)
    ## rrc-entity.h (module 'lte'): std::vector<ns3::Ptr<ns3::RadioBearerInstance>,std::allocator<ns3::Ptr<ns3::RadioBearerInstance> > > * ns3::RrcEntity::GetUplinkNgbrBearers() const [member function]
    cls.add_method('GetUplinkNgbrBearers', 
                   'std::vector< ns3::Ptr< ns3::RadioBearerInstance > > *', 
                   [], 
                   is_const=True)
    return

def register_Ns3SfVectorTlvValue_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::SfVectorTlvValue::SfVectorTlvValue(ns3::SfVectorTlvValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SfVectorTlvValue const &', 'arg0')])
    ## wimax-tlv.h (module 'wimax'): ns3::SfVectorTlvValue::SfVectorTlvValue() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::SfVectorTlvValue * ns3::SfVectorTlvValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::SfVectorTlvValue *', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::SfVectorTlvValue::Deserialize(ns3::Buffer::Iterator start, uint64_t valueLength) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint64_t', 'valueLength')], 
                   is_virtual=True)
    return

def register_Ns3SimplePacketScheduler_methods(root_module, cls):
    ## simple-packet-scheduler.h (module 'lte'): ns3::SimplePacketScheduler::SimplePacketScheduler(ns3::SimplePacketScheduler const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimplePacketScheduler const &', 'arg0')])
    ## simple-packet-scheduler.h (module 'lte'): ns3::SimplePacketScheduler::SimplePacketScheduler() [constructor]
    cls.add_constructor([])
    ## simple-packet-scheduler.h (module 'lte'): ns3::SimplePacketScheduler::SimplePacketScheduler(ns3::Ptr<ns3::EnbNetDevice> enb) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## simple-packet-scheduler.h (module 'lte'): void ns3::SimplePacketScheduler::DoRunPacketScheduler() [member function]
    cls.add_method('DoRunPacketScheduler', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## simple-packet-scheduler.h (module 'lte'): static ns3::TypeId ns3::SimplePacketScheduler::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
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

def register_Ns3SpectrumInterference_methods(root_module, cls):
    ## spectrum-interference.h (module 'spectrum'): ns3::SpectrumInterference::SpectrumInterference(ns3::SpectrumInterference const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumInterference const &', 'arg0')])
    ## spectrum-interference.h (module 'spectrum'): ns3::SpectrumInterference::SpectrumInterference() [constructor]
    cls.add_constructor([])
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
    ## spectrum-phy.h (module 'spectrum'): ns3::SpectrumPhy::SpectrumPhy(ns3::SpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumPhy const &', 'arg0')])
    ## spectrum-phy.h (module 'spectrum'): ns3::Ptr<ns3::Object> ns3::SpectrumPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): ns3::Ptr<ns3::Object> ns3::SpectrumPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): ns3::Ptr<ns3::SpectrumModel const> ns3::SpectrumPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): static ns3::TypeId ns3::SpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## spectrum-phy.h (module 'spectrum'): void ns3::SpectrumPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): void ns3::SpectrumPhy::SetDevice(ns3::Ptr<ns3::Object> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'd')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): void ns3::SpectrumPhy::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_pure_virtual=True, is_virtual=True)
    ## spectrum-phy.h (module 'spectrum'): void ns3::SpectrumPhy::StartRx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::SpectrumType st, ns3::Time duration) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration')], 
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
    cls.add_binary_numeric_operator('+', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('!=')
    cls.add_inplace_numeric_operator('+=', param('ns3::Time const &', 'right'))
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

def register_Ns3Tlv_methods(root_module, cls):
    ## wimax-tlv.h (module 'wimax'): ns3::Tlv::Tlv(uint8_t type, uint64_t length, ns3::TlvValue const & value) [constructor]
    cls.add_constructor([param('uint8_t', 'type'), param('uint64_t', 'length'), param('ns3::TlvValue const &', 'value')])
    ## wimax-tlv.h (module 'wimax'): ns3::Tlv::Tlv() [constructor]
    cls.add_constructor([])
    ## wimax-tlv.h (module 'wimax'): ns3::Tlv::Tlv(ns3::Tlv const & tlv) [copy constructor]
    cls.add_constructor([param('ns3::Tlv const &', 'tlv')])
    ## wimax-tlv.h (module 'wimax'): ns3::Tlv * ns3::Tlv::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Tlv *', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): ns3::TlvValue * ns3::Tlv::CopyValue() const [member function]
    cls.add_method('CopyValue', 
                   'ns3::TlvValue *', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::Tlv::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): ns3::TypeId ns3::Tlv::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint64_t ns3::Tlv::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint32_t ns3::Tlv::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): uint8_t ns3::Tlv::GetSizeOfLen() const [member function]
    cls.add_method('GetSizeOfLen', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): uint8_t ns3::Tlv::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## wimax-tlv.h (module 'wimax'): ns3::TlvValue * ns3::Tlv::PeekValue() [member function]
    cls.add_method('PeekValue', 
                   'ns3::TlvValue *', 
                   [])
    ## wimax-tlv.h (module 'wimax'): void ns3::Tlv::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## wimax-tlv.h (module 'wimax'): void ns3::Tlv::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
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

def register_Ns3UeManager_methods(root_module, cls):
    ## ue-manager.h (module 'lte'): ns3::UeManager::UeManager(ns3::UeManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeManager const &', 'arg0')])
    ## ue-manager.h (module 'lte'): ns3::UeManager::UeManager() [constructor]
    cls.add_constructor([])
    ## ue-manager.h (module 'lte'): void ns3::UeManager::CreateUeRecord(ns3::Ptr<ns3::UeNetDevice> ue, ns3::Ptr<ns3::EnbNetDevice> enb) [member function]
    cls.add_method('CreateUeRecord', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue'), param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## ue-manager.h (module 'lte'): void ns3::UeManager::DeleteUeRecord(ns3::Ptr<ns3::UeNetDevice> ue) [member function]
    cls.add_method('DeleteUeRecord', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue')])
    ## ue-manager.h (module 'lte'): void ns3::UeManager::DeleteUeRecord(ns3::Mac48Address const & macAddress) [member function]
    cls.add_method('DeleteUeRecord', 
                   'void', 
                   [param('ns3::Mac48Address const &', 'macAddress')])
    ## ue-manager.h (module 'lte'): uint32_t ns3::UeManager::GetNRegisteredUes() const [member function]
    cls.add_method('GetNRegisteredUes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ue-manager.h (module 'lte'): ns3::Ptr<ns3::UeRecord> ns3::UeManager::GetUeRecord(ns3::Ptr<ns3::UeNetDevice> ue) [member function]
    cls.add_method('GetUeRecord', 
                   'ns3::Ptr< ns3::UeRecord >', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue')])
    ## ue-manager.h (module 'lte'): ns3::Ptr<ns3::UeRecord> ns3::UeManager::GetUeRecord(ns3::Mac48Address const macAddress) [member function]
    cls.add_method('GetUeRecord', 
                   'ns3::Ptr< ns3::UeRecord >', 
                   [param('ns3::Mac48Address const', 'macAddress')])
    ## ue-manager.h (module 'lte'): std::vector<ns3::Ptr<ns3::UeRecord>,std::allocator<ns3::Ptr<ns3::UeRecord> > > * ns3::UeManager::GetUeRecords() [member function]
    cls.add_method('GetUeRecords', 
                   'std::vector< ns3::Ptr< ns3::UeRecord > > *', 
                   [])
    ## ue-manager.h (module 'lte'): bool ns3::UeManager::IsRegistered(ns3::Ptr<ns3::UeNetDevice> ue) const [member function]
    cls.add_method('IsRegistered', 
                   'bool', 
                   [param('ns3::Ptr< ns3::UeNetDevice >', 'ue')], 
                   is_const=True)
    ## ue-manager.h (module 'lte'): bool ns3::UeManager::IsRegistered(ns3::Mac48Address const & macAddress) const [member function]
    cls.add_method('IsRegistered', 
                   'bool', 
                   [param('ns3::Mac48Address const &', 'macAddress')], 
                   is_const=True)
    return

def register_Ns3UeRecord_methods(root_module, cls):
    ## ue-record.h (module 'lte'): ns3::UeRecord::UeRecord(ns3::UeRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeRecord const &', 'arg0')])
    ## ue-record.h (module 'lte'): ns3::UeRecord::UeRecord() [constructor]
    cls.add_constructor([])
    ## ue-record.h (module 'lte'): ns3::UeRecord::UeRecord(ns3::Ptr<ns3::NetDevice> ue, ns3::Ptr<ns3::NetDevice> enb) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'ue'), param('ns3::Ptr< ns3::NetDevice >', 'enb')])
    ## ue-record.h (module 'lte'): std::vector<ns3::UeRecord::CqiFeedback, std::allocator<ns3::UeRecord::CqiFeedback> > ns3::UeRecord::GetCqiFeedbacks() [member function]
    cls.add_method('GetCqiFeedbacks', 
                   'std::vector< ns3::UeRecord::CqiFeedback >', 
                   [])
    ## ue-record.h (module 'lte'): ns3::Ptr<ns3::NetDevice> ns3::UeRecord::GetEnb() [member function]
    cls.add_method('GetEnb', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## ue-record.h (module 'lte'): ns3::Ptr<ns3::NetDevice> ns3::UeRecord::GetUe() [member function]
    cls.add_method('GetUe', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## ue-record.h (module 'lte'): void ns3::UeRecord::SetCqiFeedbacks(std::vector<ns3::UeRecord::CqiFeedback, std::allocator<ns3::UeRecord::CqiFeedback> > cqiFeedbacks) [member function]
    cls.add_method('SetCqiFeedbacks', 
                   'void', 
                   [param('std::vector< ns3::UeRecord::CqiFeedback >', 'cqiFeedbacks')])
    ## ue-record.h (module 'lte'): void ns3::UeRecord::SetEnb(ns3::Ptr<ns3::NetDevice> enb) [member function]
    cls.add_method('SetEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'enb')])
    ## ue-record.h (module 'lte'): void ns3::UeRecord::SetUe(ns3::Ptr<ns3::NetDevice> ue) [member function]
    cls.add_method('SetUe', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'ue')])
    return

def register_Ns3UeRecordCqiFeedback_methods(root_module, cls):
    ## ue-record.h (module 'lte'): ns3::UeRecord::CqiFeedback::CqiFeedback() [constructor]
    cls.add_constructor([])
    ## ue-record.h (module 'lte'): ns3::UeRecord::CqiFeedback::CqiFeedback(ns3::UeRecord::CqiFeedback const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeRecord::CqiFeedback const &', 'arg0')])
    ## ue-record.h (module 'lte'): ns3::UeRecord::CqiFeedback::m_cqi [variable]
    cls.add_instance_attribute('m_cqi', 'int', is_const=False)
    ## ue-record.h (module 'lte'): ns3::UeRecord::CqiFeedback::m_subChannelId [variable]
    cls.add_instance_attribute('m_subChannelId', 'int', is_const=False)
    return

def register_Ns3AmcModule_methods(root_module, cls):
    ## amc-module.h (module 'lte'): ns3::AmcModule::AmcModule(ns3::AmcModule const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AmcModule const &', 'arg0')])
    ## amc-module.h (module 'lte'): ns3::AmcModule::AmcModule() [constructor]
    cls.add_constructor([])
    ## amc-module.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::AmcModule::CreateCqiFeedbacks(std::vector<double, std::allocator<double> > sinr) [member function]
    cls.add_method('CreateCqiFeedbacks', 
                   'std::vector< int >', 
                   [param('std::vector< double >', 'sinr')])
    ## amc-module.h (module 'lte'): int ns3::AmcModule::GetMcsFromCqi(int cqi) [member function]
    cls.add_method('GetMcsFromCqi', 
                   'int', 
                   [param('int', 'cqi')])
    ## amc-module.h (module 'lte'): double ns3::AmcModule::GetSpectralEfficiencyFromCqi(int cqi) [member function]
    cls.add_method('GetSpectralEfficiencyFromCqi', 
                   'double', 
                   [param('int', 'cqi')])
    ## amc-module.h (module 'lte'): int ns3::AmcModule::GetTbSizeFromMcs(int mcs) [member function]
    cls.add_method('GetTbSizeFromMcs', 
                   'int', 
                   [param('int', 'mcs')])
    ## amc-module.h (module 'lte'): static ns3::TypeId ns3::AmcModule::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## amc-module.h (module 'lte'): void ns3::AmcModule::Initialize() [member function]
    cls.add_method('Initialize', 
                   'void', 
                   [])
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

def register_Ns3BearerQosParameters_methods(root_module, cls):
    ## bearer-qos-parameters.h (module 'lte'): ns3::BearerQosParameters::BearerQosParameters(ns3::BearerQosParameters const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BearerQosParameters const &', 'arg0')])
    ## bearer-qos-parameters.h (module 'lte'): ns3::BearerQosParameters::BearerQosParameters() [constructor]
    cls.add_constructor([])
    ## bearer-qos-parameters.h (module 'lte'): ns3::BearerQosParameters::BearerQosParameters(int qci, double gbr, double mbr) [constructor]
    cls.add_constructor([param('int', 'qci'), param('double', 'gbr'), param('double', 'mbr')])
    ## bearer-qos-parameters.h (module 'lte'): ns3::BearerQosParameters::BearerQosParameters(int qci, bool apec, bool apev, double gbr, double mbr) [constructor]
    cls.add_constructor([param('int', 'qci'), param('bool', 'apec'), param('bool', 'apev'), param('double', 'gbr'), param('double', 'mbr')])
    ## bearer-qos-parameters.h (module 'lte'): bool ns3::BearerQosParameters::GetArpPreEmptionCapability() const [member function]
    cls.add_method('GetArpPreEmptionCapability', 
                   'bool', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h (module 'lte'): bool ns3::BearerQosParameters::GetArpPreEmptionVulnerability() const [member function]
    cls.add_method('GetArpPreEmptionVulnerability', 
                   'bool', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h (module 'lte'): ns3::BearerQosParameters::BearerQosType ns3::BearerQosParameters::GetBearerQosType() const [member function]
    cls.add_method('GetBearerQosType', 
                   'ns3::BearerQosParameters::BearerQosType', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h (module 'lte'): double ns3::BearerQosParameters::GetGbr() const [member function]
    cls.add_method('GetGbr', 
                   'double', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h (module 'lte'): double ns3::BearerQosParameters::GetMaxDelay() const [member function]
    cls.add_method('GetMaxDelay', 
                   'double', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h (module 'lte'): double ns3::BearerQosParameters::GetMbr() const [member function]
    cls.add_method('GetMbr', 
                   'double', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h (module 'lte'): int ns3::BearerQosParameters::GetQci() const [member function]
    cls.add_method('GetQci', 
                   'int', 
                   [], 
                   is_const=True)
    ## bearer-qos-parameters.h (module 'lte'): void ns3::BearerQosParameters::SetArpPreEmptionCapability(bool apec) [member function]
    cls.add_method('SetArpPreEmptionCapability', 
                   'void', 
                   [param('bool', 'apec')])
    ## bearer-qos-parameters.h (module 'lte'): void ns3::BearerQosParameters::SetArpPreEmptionVulnerability(bool apev) [member function]
    cls.add_method('SetArpPreEmptionVulnerability', 
                   'void', 
                   [param('bool', 'apev')])
    ## bearer-qos-parameters.h (module 'lte'): void ns3::BearerQosParameters::SetBearerQosType(ns3::BearerQosParameters::BearerQosType QosType) [member function]
    cls.add_method('SetBearerQosType', 
                   'void', 
                   [param('ns3::BearerQosParameters::BearerQosType', 'QosType')])
    ## bearer-qos-parameters.h (module 'lte'): void ns3::BearerQosParameters::SetGbr(double gbr) [member function]
    cls.add_method('SetGbr', 
                   'void', 
                   [param('double', 'gbr')])
    ## bearer-qos-parameters.h (module 'lte'): void ns3::BearerQosParameters::SetMaxDelay(double targetDelay) [member function]
    cls.add_method('SetMaxDelay', 
                   'void', 
                   [param('double', 'targetDelay')])
    ## bearer-qos-parameters.h (module 'lte'): void ns3::BearerQosParameters::SetMbr(double mbr) [member function]
    cls.add_method('SetMbr', 
                   'void', 
                   [param('double', 'mbr')])
    ## bearer-qos-parameters.h (module 'lte'): void ns3::BearerQosParameters::SetQci(int qci) [member function]
    cls.add_method('SetQci', 
                   'void', 
                   [param('int', 'qci')])
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

def register_Ns3ChannelRealization_methods(root_module, cls):
    ## channel-realization.h (module 'lte'): ns3::ChannelRealization::ChannelRealization(ns3::ChannelRealization const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ChannelRealization const &', 'arg0')])
    ## channel-realization.h (module 'lte'): ns3::ChannelRealization::ChannelRealization() [constructor]
    cls.add_constructor([])
    ## channel-realization.h (module 'lte'): ns3::Ptr<ns3::JakesFadingLossModel> ns3::ChannelRealization::GetJakesFadingLossModel() [member function]
    cls.add_method('GetJakesFadingLossModel', 
                   'ns3::Ptr< ns3::JakesFadingLossModel >', 
                   [])
    ## channel-realization.h (module 'lte'): ns3::Ptr<ns3::PathLossModel> ns3::ChannelRealization::GetPathLossModel() [member function]
    cls.add_method('GetPathLossModel', 
                   'ns3::Ptr< ns3::PathLossModel >', 
                   [])
    ## channel-realization.h (module 'lte'): ns3::Ptr<ns3::PenetrationLossModel> ns3::ChannelRealization::GetPenetrationLossModel() [member function]
    cls.add_method('GetPenetrationLossModel', 
                   'ns3::Ptr< ns3::PenetrationLossModel >', 
                   [])
    ## channel-realization.h (module 'lte'): ns3::Ptr<ns3::ShadowingLossModel> ns3::ChannelRealization::GetShadowingLossModel() [member function]
    cls.add_method('GetShadowingLossModel', 
                   'ns3::Ptr< ns3::ShadowingLossModel >', 
                   [])
    ## channel-realization.h (module 'lte'): static ns3::TypeId ns3::ChannelRealization::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## channel-realization.h (module 'lte'): void ns3::ChannelRealization::SetJakesFadingLossModel(ns3::Ptr<ns3::JakesFadingLossModel> l) [member function]
    cls.add_method('SetJakesFadingLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::JakesFadingLossModel >', 'l')])
    ## channel-realization.h (module 'lte'): void ns3::ChannelRealization::SetPathLossModel(ns3::Ptr<ns3::PathLossModel> l) [member function]
    cls.add_method('SetPathLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PathLossModel >', 'l')])
    ## channel-realization.h (module 'lte'): void ns3::ChannelRealization::SetPenetrationLossModel(ns3::Ptr<ns3::PenetrationLossModel> l) [member function]
    cls.add_method('SetPenetrationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PenetrationLossModel >', 'l')])
    ## channel-realization.h (module 'lte'): void ns3::ChannelRealization::SetShadowingLossModel(ns3::Ptr<ns3::ShadowingLossModel> l) [member function]
    cls.add_method('SetShadowingLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ShadowingLossModel >', 'l')])
    return

def register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h (module 'propagation'): ns3::ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel(ns3::ConstantSpeedPropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantSpeedPropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h (module 'propagation'): ns3::ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h (module 'propagation'): ns3::Time ns3::ConstantSpeedPropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## propagation-delay-model.h (module 'propagation'): double ns3::ConstantSpeedPropagationDelayModel::GetSpeed() const [member function]
    cls.add_method('GetSpeed', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-delay-model.h (module 'propagation'): static ns3::TypeId ns3::ConstantSpeedPropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-delay-model.h (module 'propagation'): void ns3::ConstantSpeedPropagationDelayModel::SetSpeed(double speed) [member function]
    cls.add_method('SetSpeed', 
                   'void', 
                   [param('double', 'speed')])
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

def register_Ns3DiscreteTimeLossModel_methods(root_module, cls):
    ## discrete-time-loss-model.h (module 'lte'): ns3::DiscreteTimeLossModel::DiscreteTimeLossModel(ns3::DiscreteTimeLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DiscreteTimeLossModel const &', 'arg0')])
    ## discrete-time-loss-model.h (module 'lte'): ns3::DiscreteTimeLossModel::DiscreteTimeLossModel() [constructor]
    cls.add_constructor([])
    ## discrete-time-loss-model.h (module 'lte'): ns3::Time ns3::DiscreteTimeLossModel::GetLastUpdate() [member function]
    cls.add_method('GetLastUpdate', 
                   'ns3::Time', 
                   [])
    ## discrete-time-loss-model.h (module 'lte'): double ns3::DiscreteTimeLossModel::GetSamplingPeriod() [member function]
    cls.add_method('GetSamplingPeriod', 
                   'double', 
                   [])
    ## discrete-time-loss-model.h (module 'lte'): static ns3::TypeId ns3::DiscreteTimeLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## discrete-time-loss-model.h (module 'lte'): bool ns3::DiscreteTimeLossModel::NeedForUpdate() [member function]
    cls.add_method('NeedForUpdate', 
                   'bool', 
                   [])
    ## discrete-time-loss-model.h (module 'lte'): void ns3::DiscreteTimeLossModel::SetLastUpdate() [member function]
    cls.add_method('SetLastUpdate', 
                   'void', 
                   [])
    ## discrete-time-loss-model.h (module 'lte'): void ns3::DiscreteTimeLossModel::SetSamplingPeriod(double sp) [member function]
    cls.add_method('SetSamplingPeriod', 
                   'void', 
                   [param('double', 'sp')])
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

def register_Ns3JakesFadingLossModel_methods(root_module, cls):
    ## jakes-fading-loss-model.h (module 'lte'): ns3::JakesFadingLossModel::JakesFadingLossModel(ns3::JakesFadingLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::JakesFadingLossModel const &', 'arg0')])
    ## jakes-fading-loss-model.h (module 'lte'): ns3::JakesFadingLossModel::JakesFadingLossModel() [constructor]
    cls.add_constructor([])
    ## jakes-fading-loss-model.h (module 'lte'): ns3::Ptr<ns3::LtePhy> ns3::JakesFadingLossModel::GetPhy() [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::LtePhy >', 
                   [])
    ## jakes-fading-loss-model.h (module 'lte'): static ns3::TypeId ns3::JakesFadingLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## jakes-fading-loss-model.h (module 'lte'): double ns3::JakesFadingLossModel::GetValue(int subChannel) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('int', 'subChannel')])
    ## jakes-fading-loss-model.h (module 'lte'): void ns3::JakesFadingLossModel::SetPhy(ns3::Ptr<ns3::LtePhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## jakes-fading-loss-model.h (module 'lte'): void ns3::JakesFadingLossModel::SetValue() [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [])
    return

def register_Ns3LteMacQueue_methods(root_module, cls):
    ## lte-mac-queue.h (module 'lte'): ns3::LteMacQueue::LteMacQueue(ns3::LteMacQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteMacQueue const &', 'arg0')])
    ## lte-mac-queue.h (module 'lte'): ns3::LteMacQueue::LteMacQueue() [constructor]
    cls.add_constructor([])
    ## lte-mac-queue.h (module 'lte'): ns3::LteMacQueue::LteMacQueue(uint32_t maxSize) [constructor]
    cls.add_constructor([param('uint32_t', 'maxSize')])
    ## lte-mac-queue.h (module 'lte'): ns3::Ptr<ns3::Packet> ns3::LteMacQueue::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## lte-mac-queue.h (module 'lte'): ns3::Ptr<ns3::Packet> ns3::LteMacQueue::Dequeue(uint32_t availableByte) [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'availableByte')])
    ## lte-mac-queue.h (module 'lte'): bool ns3::LteMacQueue::Enqueue(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    ## lte-mac-queue.h (module 'lte'): uint32_t ns3::LteMacQueue::GetMaxSize() const [member function]
    cls.add_method('GetMaxSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h (module 'lte'): uint32_t ns3::LteMacQueue::GetNBytes() const [member function]
    cls.add_method('GetNBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h (module 'lte'): std::deque<ns3::LteMacQueue::QueueElement, std::allocator<ns3::LteMacQueue::QueueElement> > const & ns3::LteMacQueue::GetPacketQueue() const [member function]
    cls.add_method('GetPacketQueue', 
                   'std::deque< ns3::LteMacQueue::QueueElement > const &', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h (module 'lte'): uint32_t ns3::LteMacQueue::GetQueueLengthWithMACOverhead() [member function]
    cls.add_method('GetQueueLengthWithMACOverhead', 
                   'uint32_t', 
                   [])
    ## lte-mac-queue.h (module 'lte'): uint32_t ns3::LteMacQueue::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h (module 'lte'): static ns3::TypeId ns3::LteMacQueue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-mac-queue.h (module 'lte'): bool ns3::LteMacQueue::IsEmpty() const [member function]
    cls.add_method('IsEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h (module 'lte'): ns3::Ptr<ns3::Packet> ns3::LteMacQueue::Peek() const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## lte-mac-queue.h (module 'lte'): void ns3::LteMacQueue::SetMaxSize(uint32_t maxSize) [member function]
    cls.add_method('SetMaxSize', 
                   'void', 
                   [param('uint32_t', 'maxSize')])
    return

def register_Ns3LtePhy_methods(root_module, cls):
    ## lte-phy.h (module 'lte'): ns3::LtePhy::LtePhy(ns3::LtePhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePhy const &', 'arg0')])
    ## lte-phy.h (module 'lte'): ns3::LtePhy::LtePhy() [constructor]
    cls.add_constructor([])
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
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoSetDownlinkSubChannels() [member function]
    cls.add_method('DoSetDownlinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::DoSetUplinkSubChannels() [member function]
    cls.add_method('DoSetUplinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::LteNetDevice> ns3::LtePhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumChannel> ns3::LtePhy::GetDownlinkChannel() [member function]
    cls.add_method('GetDownlinkChannel', 
                   'ns3::Ptr< ns3::SpectrumChannel >', 
                   [])
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::LteSpectrumPhy> ns3::LtePhy::GetDownlinkSpectrumPhy() [member function]
    cls.add_method('GetDownlinkSpectrumPhy', 
                   'ns3::Ptr< ns3::LteSpectrumPhy >', 
                   [])
    ## lte-phy.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::LtePhy::GetDownlinkSubChannels() [member function]
    cls.add_method('GetDownlinkSubChannels', 
                   'std::vector< int >', 
                   [])
    ## lte-phy.h (module 'lte'): uint32_t ns3::LtePhy::GetNrFrames() const [member function]
    cls.add_method('GetNrFrames', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-phy.h (module 'lte'): uint32_t ns3::LtePhy::GetNrSubFrames() const [member function]
    cls.add_method('GetNrSubFrames', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## lte-phy.h (module 'lte'): double ns3::LtePhy::GetTti() const [member function]
    cls.add_method('GetTti', 
                   'double', 
                   [], 
                   is_const=True)
    ## lte-phy.h (module 'lte'): double ns3::LtePhy::GetTxPower() [member function]
    cls.add_method('GetTxPower', 
                   'double', 
                   [])
    ## lte-phy.h (module 'lte'): static ns3::TypeId ns3::LtePhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumChannel> ns3::LtePhy::GetUplinkChannel() [member function]
    cls.add_method('GetUplinkChannel', 
                   'ns3::Ptr< ns3::SpectrumChannel >', 
                   [])
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
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('SendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h (module 'lte'): bool ns3::LtePhy::SendPacket(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetDevice(ns3::Ptr<ns3::LteNetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetDownlinkChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetDownlinkChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetDownlinkSpectrumPhy(ns3::Ptr<ns3::LteSpectrumPhy> s) [member function]
    cls.add_method('SetDownlinkSpectrumPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteSpectrumPhy >', 's')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetDownlinkSubChannels(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetDownlinkSubChannels', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetNrFrames(uint32_t nrFrames) [member function]
    cls.add_method('SetNrFrames', 
                   'void', 
                   [param('uint32_t', 'nrFrames')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetNrSubFrames(uint32_t nrSubFrames) [member function]
    cls.add_method('SetNrSubFrames', 
                   'void', 
                   [param('uint32_t', 'nrSubFrames')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetTti(double tti) [member function]
    cls.add_method('SetTti', 
                   'void', 
                   [param('double', 'tti')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetTxPower(double pw) [member function]
    cls.add_method('SetTxPower', 
                   'void', 
                   [param('double', 'pw')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetUplinkChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetUplinkChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetUplinkSpectrumPhy(ns3::Ptr<ns3::LteSpectrumPhy> s) [member function]
    cls.add_method('SetUplinkSpectrumPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteSpectrumPhy >', 's')])
    ## lte-phy.h (module 'lte'): void ns3::LtePhy::SetUplinkSubChannels(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetUplinkSubChannels', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    return

def register_Ns3LtePropagationLossModel_methods(root_module, cls):
    ## lte-propagation-loss-model.h (module 'lte'): ns3::LtePropagationLossModel::LtePropagationLossModel(ns3::LtePropagationLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LtePropagationLossModel const &', 'arg0')])
    ## lte-propagation-loss-model.h (module 'lte'): ns3::LtePropagationLossModel::LtePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## lte-propagation-loss-model.h (module 'lte'): void ns3::LtePropagationLossModel::CreateChannelRealization(ns3::Ptr<const ns3::MobilityModel> enbMobility, ns3::Ptr<const ns3::MobilityModel> ueMobility) [member function]
    cls.add_method('CreateChannelRealization', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'enbMobility'), param('ns3::Ptr< ns3::MobilityModel const >', 'ueMobility')])
    ## lte-propagation-loss-model.h (module 'lte'): ns3::Ptr<ns3::ChannelRealization> ns3::LtePropagationLossModel::GetChannelRealization(ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('GetChannelRealization', 
                   'ns3::Ptr< ns3::ChannelRealization >', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_const=True)
    ## lte-propagation-loss-model.h (module 'lte'): static ns3::TypeId ns3::LtePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-propagation-loss-model.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::LtePropagationLossModel::DoCalcRxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> txPsd, ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'txPsd'), param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3LteSpectrumPhy_methods(root_module, cls):
    ## lte-spectrum-phy.h (module 'lte'): ns3::LteSpectrumPhy::LteSpectrumPhy(ns3::LteSpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LteSpectrumPhy const &', 'arg0')])
    ## lte-spectrum-phy.h (module 'lte'): ns3::LteSpectrumPhy::LteSpectrumPhy() [constructor]
    cls.add_constructor([])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::CalcSinrValues(ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::Ptr<ns3::SpectrumValue const> noise) [member function]
    cls.add_method('CalcSinrValues', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::Ptr< ns3::SpectrumValue const >', 'noise')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumChannel> ns3::LteSpectrumPhy::GetChannel() [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::SpectrumChannel >', 
                   [])
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::Object> ns3::LteSpectrumPhy::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::Object> ns3::LteSpectrumPhy::GetMobility() [member function]
    cls.add_method('GetMobility', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue const> ns3::LteSpectrumPhy::GetNoisePowerSpectralDensity() [member function]
    cls.add_method('GetNoisePowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue const >', 
                   [])
    ## lte-spectrum-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumModel const> ns3::LteSpectrumPhy::GetRxSpectrumModel() const [member function]
    cls.add_method('GetRxSpectrumModel', 
                   'ns3::Ptr< ns3::SpectrumModel const >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): ns3::SpectrumType ns3::LteSpectrumPhy::GetSpectrumType() [member function]
    cls.add_method('GetSpectrumType', 
                   'ns3::SpectrumType', 
                   [])
    ## lte-spectrum-phy.h (module 'lte'): static ns3::TypeId ns3::LteSpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetChannel(ns3::Ptr<ns3::SpectrumChannel> c) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumChannel >', 'c')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetDevice(ns3::Ptr<ns3::Object> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'd')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetGenericPhyRxEndErrorCallback(ns3::GenericPhyRxEndErrorCallback c) [member function]
    cls.add_method('SetGenericPhyRxEndErrorCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxEndErrorCallback', 'c')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetGenericPhyRxEndOkCallback(ns3::GenericPhyRxEndOkCallback c) [member function]
    cls.add_method('SetGenericPhyRxEndOkCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxEndOkCallback', 'c')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetGenericPhyRxStartCallback(ns3::GenericPhyRxStartCallback c) [member function]
    cls.add_method('SetGenericPhyRxStartCallback', 
                   'void', 
                   [param('ns3::GenericPhyRxStartCallback', 'c')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetGenericPhyTxEndCallback(ns3::GenericPhyTxEndCallback c) [member function]
    cls.add_method('SetGenericPhyTxEndCallback', 
                   'void', 
                   [param('ns3::GenericPhyTxEndCallback', 'c')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetMobility(ns3::Ptr<ns3::Object> m) [member function]
    cls.add_method('SetMobility', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'm')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetNoisePowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue const> noisePsd) [member function]
    cls.add_method('SetNoisePowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'noisePsd')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetState(ns3::LteSpectrumPhy::State newState) [member function]
    cls.add_method('SetState', 
                   'void', 
                   [param('ns3::LteSpectrumPhy::State', 'newState')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::SetTxPowerSpectralDensity(ns3::Ptr<ns3::SpectrumValue> txPsd) [member function]
    cls.add_method('SetTxPowerSpectralDensity', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::StartRx(ns3::Ptr<ns3::PacketBurst> pb, ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::SpectrumType st, ns3::Time duration) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb'), param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration')], 
                   is_virtual=True)
    ## lte-spectrum-phy.h (module 'lte'): bool ns3::LteSpectrumPhy::StartTx(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('StartTx', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')])
    ## lte-spectrum-phy.h (module 'lte'): void ns3::LteSpectrumPhy::EndRx() [member function]
    cls.add_method('EndRx', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
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

def register_Ns3MacEntity_methods(root_module, cls):
    ## mac-entity.h (module 'lte'): ns3::MacEntity::MacEntity(ns3::MacEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MacEntity const &', 'arg0')])
    ## mac-entity.h (module 'lte'): ns3::MacEntity::MacEntity() [constructor]
    cls.add_constructor([])
    ## mac-entity.h (module 'lte'): void ns3::MacEntity::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## mac-entity.h (module 'lte'): ns3::Ptr<ns3::AmcModule> ns3::MacEntity::GetAmcModule() const [member function]
    cls.add_method('GetAmcModule', 
                   'ns3::Ptr< ns3::AmcModule >', 
                   [], 
                   is_const=True)
    ## mac-entity.h (module 'lte'): ns3::Ptr<ns3::LteNetDevice> ns3::MacEntity::GetDevice() [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::LteNetDevice >', 
                   [])
    ## mac-entity.h (module 'lte'): static ns3::TypeId ns3::MacEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mac-entity.h (module 'lte'): void ns3::MacEntity::SetAmcModule(ns3::Ptr<ns3::AmcModule> amcModule) [member function]
    cls.add_method('SetAmcModule', 
                   'void', 
                   [param('ns3::Ptr< ns3::AmcModule >', 'amcModule')])
    ## mac-entity.h (module 'lte'): void ns3::MacEntity::SetDevice(ns3::Ptr<ns3::LteNetDevice> d) [member function]
    cls.add_method('SetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    return

def register_Ns3MobilityModel_methods(root_module, cls):
    ## mobility-model.h (module 'mobility'): ns3::MobilityModel::MobilityModel(ns3::MobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MobilityModel const &', 'arg0')])
    ## mobility-model.h (module 'mobility'): ns3::MobilityModel::MobilityModel() [constructor]
    cls.add_constructor([])
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
    ## node.h (module 'network'): void ns3::Node::RegisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler, uint16_t protocolType, ns3::Ptr<ns3::NetDevice> device, bool promiscuous=false) [member function]
    cls.add_method('RegisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler'), param('uint16_t', 'protocolType'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'promiscuous', default_value='false')])
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
    ## node.h (module 'network'): void ns3::Node::NotifyDeviceAdded(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('NotifyDeviceAdded', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   visibility='private', is_virtual=True)
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

def register_Ns3PathLossModel_methods(root_module, cls):
    ## path-loss-model.h (module 'lte'): ns3::PathLossModel::PathLossModel(ns3::PathLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PathLossModel const &', 'arg0')])
    ## path-loss-model.h (module 'lte'): ns3::PathLossModel::PathLossModel() [constructor]
    cls.add_constructor([])
    ## path-loss-model.h (module 'lte'): static ns3::TypeId ns3::PathLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## path-loss-model.h (module 'lte'): double ns3::PathLossModel::GetValue(ns3::Ptr<const ns3::MobilityModel> a, ns3::Ptr<const ns3::MobilityModel> b) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'a'), param('ns3::Ptr< ns3::MobilityModel const >', 'b')])
    ## path-loss-model.h (module 'lte'): void ns3::PathLossModel::SetValue(double pl) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('double', 'pl')])
    return

def register_Ns3PdcchMapIdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::PdcchMapIdealControlMessage(ns3::PdcchMapIdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PdcchMapIdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::PdcchMapIdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): void ns3::PdcchMapIdealControlMessage::AddNewRecord(ns3::PdcchMapIdealControlMessage::Direction direction, int subChannel, ns3::Ptr<ns3::LteNetDevice> ue, double mcs) [member function]
    cls.add_method('AddNewRecord', 
                   'void', 
                   [param('ns3::PdcchMapIdealControlMessage::Direction', 'direction'), param('int', 'subChannel'), param('ns3::Ptr< ns3::LteNetDevice >', 'ue'), param('double', 'mcs')])
    ## ideal-control-messages.h (module 'lte'): std::list<ns3::PdcchMapIdealControlMessage::IdealPdcchRecord,std::allocator<ns3::PdcchMapIdealControlMessage::IdealPdcchRecord> > * ns3::PdcchMapIdealControlMessage::GetMessage() [member function]
    cls.add_method('GetMessage', 
                   'std::list< ns3::PdcchMapIdealControlMessage::IdealPdcchRecord > *', 
                   [])
    return

def register_Ns3PdcchMapIdealControlMessageIdealPdcchRecord_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::IdealPdcchRecord() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::IdealPdcchRecord(ns3::PdcchMapIdealControlMessage::IdealPdcchRecord const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PdcchMapIdealControlMessage::IdealPdcchRecord const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::m_direction [variable]
    cls.add_instance_attribute('m_direction', 'ns3::PdcchMapIdealControlMessage::Direction', is_const=False)
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::m_idSubChannel [variable]
    cls.add_instance_attribute('m_idSubChannel', 'int', is_const=False)
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::m_mcsIndex [variable]
    cls.add_instance_attribute('m_mcsIndex', 'double', is_const=False)
    ## ideal-control-messages.h (module 'lte'): ns3::PdcchMapIdealControlMessage::IdealPdcchRecord::m_ue [variable]
    cls.add_instance_attribute('m_ue', 'ns3::Ptr< ns3::LteNetDevice >', is_const=False)
    return

def register_Ns3PenetrationLossModel_methods(root_module, cls):
    ## penetration-loss-model.h (module 'lte'): ns3::PenetrationLossModel::PenetrationLossModel(ns3::PenetrationLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PenetrationLossModel const &', 'arg0')])
    ## penetration-loss-model.h (module 'lte'): ns3::PenetrationLossModel::PenetrationLossModel() [constructor]
    cls.add_constructor([])
    ## penetration-loss-model.h (module 'lte'): static ns3::TypeId ns3::PenetrationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## penetration-loss-model.h (module 'lte'): double ns3::PenetrationLossModel::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [])
    ## penetration-loss-model.h (module 'lte'): void ns3::PenetrationLossModel::SetValue(double pnl) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('double', 'pnl')])
    return

def register_Ns3RandomVariableChecker_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::RandomVariableChecker::RandomVariableChecker() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::RandomVariableChecker::RandomVariableChecker(ns3::RandomVariableChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariableChecker const &', 'arg0')])
    return

def register_Ns3RandomVariableValue_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::RandomVariableValue::RandomVariableValue() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::RandomVariableValue::RandomVariableValue(ns3::RandomVariableValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariableValue const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::RandomVariableValue::RandomVariableValue(ns3::RandomVariable const & value) [constructor]
    cls.add_constructor([param('ns3::RandomVariable const &', 'value')])
    ## random-variable.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::RandomVariableValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## random-variable.h (module 'core'): bool ns3::RandomVariableValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## random-variable.h (module 'core'): ns3::RandomVariable ns3::RandomVariableValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::RandomVariable', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): std::string ns3::RandomVariableValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## random-variable.h (module 'core'): void ns3::RandomVariableValue::Set(ns3::RandomVariable const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::RandomVariable const &', 'value')])
    return

def register_Ns3ShadowingLossModel_methods(root_module, cls):
    ## shadowing-loss-model.h (module 'lte'): ns3::ShadowingLossModel::ShadowingLossModel(ns3::ShadowingLossModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ShadowingLossModel const &', 'arg0')])
    ## shadowing-loss-model.h (module 'lte'): ns3::ShadowingLossModel::ShadowingLossModel() [constructor]
    cls.add_constructor([])
    ## shadowing-loss-model.h (module 'lte'): ns3::ShadowingLossModel::ShadowingLossModel(double mu, double sigma, double samplingPeriod) [constructor]
    cls.add_constructor([param('double', 'mu'), param('double', 'sigma'), param('double', 'samplingPeriod')])
    ## shadowing-loss-model.h (module 'lte'): static ns3::TypeId ns3::ShadowingLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## shadowing-loss-model.h (module 'lte'): double ns3::ShadowingLossModel::GetValue() [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [])
    ## shadowing-loss-model.h (module 'lte'): void ns3::ShadowingLossModel::SetValue(double sh) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('double', 'sh')])
    return

def register_Ns3SpectrumChannel_methods(root_module, cls):
    ## spectrum-channel.h (module 'spectrum'): ns3::SpectrumChannel::SpectrumChannel() [constructor]
    cls.add_constructor([])
    ## spectrum-channel.h (module 'spectrum'): ns3::SpectrumChannel::SpectrumChannel(ns3::SpectrumChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SpectrumChannel const &', 'arg0')])
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
    ## spectrum-channel.h (module 'spectrum'): void ns3::SpectrumChannel::StartTx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> txPsd, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> sender) [member function]
    cls.add_method('StartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'sender')], 
                   is_pure_virtual=True, is_virtual=True)
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

def register_Ns3UeLtePhy_methods(root_module, cls):
    ## ue-phy.h (module 'lte'): ns3::UeLtePhy::UeLtePhy(ns3::UeLtePhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeLtePhy const &', 'arg0')])
    ## ue-phy.h (module 'lte'): ns3::UeLtePhy::UeLtePhy() [constructor]
    cls.add_constructor([])
    ## ue-phy.h (module 'lte'): ns3::UeLtePhy::UeLtePhy(ns3::Ptr<ns3::LteNetDevice> d) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## ue-phy.h (module 'lte'): void ns3::UeLtePhy::CreateCqiFeedbacks(std::vector<double, std::allocator<double> > sinr) [member function]
    cls.add_method('CreateCqiFeedbacks', 
                   'void', 
                   [param('std::vector< double >', 'sinr')])
    ## ue-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::UeLtePhy::CreateTxPowerSpectralDensity() [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_virtual=True)
    ## ue-phy.h (module 'lte'): void ns3::UeLtePhy::DoSetUplinkSubChannels() [member function]
    cls.add_method('DoSetUplinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-phy.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::UeLtePhy::GetSubChannelsForReception() [member function]
    cls.add_method('GetSubChannelsForReception', 
                   'std::vector< int >', 
                   [])
    ## ue-phy.h (module 'lte'): std::vector<int, std::allocator<int> > ns3::UeLtePhy::GetSubChannelsForTransmission() [member function]
    cls.add_method('GetSubChannelsForTransmission', 
                   'std::vector< int >', 
                   [])
    ## ue-phy.h (module 'lte'): static ns3::TypeId ns3::UeLtePhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ue-phy.h (module 'lte'): void ns3::UeLtePhy::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## ue-phy.h (module 'lte'): void ns3::UeLtePhy::SendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('SendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## ue-phy.h (module 'lte'): bool ns3::UeLtePhy::SendPacket(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')], 
                   is_virtual=True)
    ## ue-phy.h (module 'lte'): void ns3::UeLtePhy::SetSubChannelsForReception(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetSubChannelsForReception', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    ## ue-phy.h (module 'lte'): void ns3::UeLtePhy::SetSubChannelsForTransmission(std::vector<int, std::allocator<int> > mask) [member function]
    cls.add_method('SetSubChannelsForTransmission', 
                   'void', 
                   [param('std::vector< int >', 'mask')])
    return

def register_Ns3UeLteSpectrumPhy_methods(root_module, cls):
    ## ue-lte-spectrum-phy.h (module 'lte'): ns3::UeLteSpectrumPhy::UeLteSpectrumPhy(ns3::UeLteSpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeLteSpectrumPhy const &', 'arg0')])
    ## ue-lte-spectrum-phy.h (module 'lte'): ns3::UeLteSpectrumPhy::UeLteSpectrumPhy() [constructor]
    cls.add_constructor([])
    ## ue-lte-spectrum-phy.h (module 'lte'): void ns3::UeLteSpectrumPhy::CalcSinrValues(ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::Ptr<ns3::SpectrumValue const> noise) [member function]
    cls.add_method('CalcSinrValues', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::Ptr< ns3::SpectrumValue const >', 'noise')], 
                   is_virtual=True)
    ## ue-lte-spectrum-phy.h (module 'lte'): static ns3::TypeId ns3::UeLteSpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3UeMacEntity_methods(root_module, cls):
    ## ue-mac-entity.h (module 'lte'): ns3::UeMacEntity::UeMacEntity(ns3::UeMacEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UeMacEntity const &', 'arg0')])
    ## ue-mac-entity.h (module 'lte'): ns3::UeMacEntity::UeMacEntity() [constructor]
    cls.add_constructor([])
    ## ue-mac-entity.h (module 'lte'): ns3::Ptr<ns3::CqiIdealControlMessage> ns3::UeMacEntity::CreateCqiFeedbacks(std::vector<double, std::allocator<double> > sinr) [member function]
    cls.add_method('CreateCqiFeedbacks', 
                   'ns3::Ptr< ns3::CqiIdealControlMessage >', 
                   [param('std::vector< double >', 'sinr')])
    ## ue-mac-entity.h (module 'lte'): static ns3::TypeId ns3::UeMacEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
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

def register_Ns3CqiIdealControlMessage_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::CqiIdealControlMessage::CqiIdealControlMessage(ns3::CqiIdealControlMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CqiIdealControlMessage const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::CqiIdealControlMessage::CqiIdealControlMessage() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): void ns3::CqiIdealControlMessage::AddNewRecord(int subChannel, double cqi) [member function]
    cls.add_method('AddNewRecord', 
                   'void', 
                   [param('int', 'subChannel'), param('double', 'cqi')])
    ## ideal-control-messages.h (module 'lte'): std::list<ns3::CqiIdealControlMessage::CqiFeedback,std::allocator<ns3::CqiIdealControlMessage::CqiFeedback> > * ns3::CqiIdealControlMessage::GetMessage() [member function]
    cls.add_method('GetMessage', 
                   'std::list< ns3::CqiIdealControlMessage::CqiFeedback > *', 
                   [])
    return

def register_Ns3CqiIdealControlMessageCqiFeedback_methods(root_module, cls):
    ## ideal-control-messages.h (module 'lte'): ns3::CqiIdealControlMessage::CqiFeedback::CqiFeedback() [constructor]
    cls.add_constructor([])
    ## ideal-control-messages.h (module 'lte'): ns3::CqiIdealControlMessage::CqiFeedback::CqiFeedback(ns3::CqiIdealControlMessage::CqiFeedback const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CqiIdealControlMessage::CqiFeedback const &', 'arg0')])
    ## ideal-control-messages.h (module 'lte'): ns3::CqiIdealControlMessage::CqiFeedback::m_cqi [variable]
    cls.add_instance_attribute('m_cqi', 'double', is_const=False)
    ## ideal-control-messages.h (module 'lte'): ns3::CqiIdealControlMessage::CqiFeedback::m_idSubChannel [variable]
    cls.add_instance_attribute('m_idSubChannel', 'int', is_const=False)
    return

def register_Ns3EnbLtePhy_methods(root_module, cls):
    ## enb-phy.h (module 'lte'): ns3::EnbLtePhy::EnbLtePhy(ns3::EnbLtePhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnbLtePhy const &', 'arg0')])
    ## enb-phy.h (module 'lte'): ns3::EnbLtePhy::EnbLtePhy() [constructor]
    cls.add_constructor([])
    ## enb-phy.h (module 'lte'): ns3::EnbLtePhy::EnbLtePhy(ns3::Ptr<ns3::LteNetDevice> d) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::LteNetDevice >', 'd')])
    ## enb-phy.h (module 'lte'): void ns3::EnbLtePhy::CalcChannelQualityForUe(std::vector<double, std::allocator<double> > sinr, ns3::Ptr<ns3::LteSpectrumPhy> ue) [member function]
    cls.add_method('CalcChannelQualityForUe', 
                   'void', 
                   [param('std::vector< double >', 'sinr'), param('ns3::Ptr< ns3::LteSpectrumPhy >', 'ue')])
    ## enb-phy.h (module 'lte'): ns3::Ptr<ns3::SpectrumValue> ns3::EnbLtePhy::CreateTxPowerSpectralDensity() [member function]
    cls.add_method('CreateTxPowerSpectralDensity', 
                   'ns3::Ptr< ns3::SpectrumValue >', 
                   [], 
                   is_virtual=True)
    ## enb-phy.h (module 'lte'): void ns3::EnbLtePhy::DoSetDownlinkSubChannels() [member function]
    cls.add_method('DoSetDownlinkSubChannels', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-phy.h (module 'lte'): void ns3::EnbLtePhy::EndFrame() [member function]
    cls.add_method('EndFrame', 
                   'void', 
                   [])
    ## enb-phy.h (module 'lte'): void ns3::EnbLtePhy::EndSubFrame() [member function]
    cls.add_method('EndSubFrame', 
                   'void', 
                   [])
    ## enb-phy.h (module 'lte'): static ns3::TypeId ns3::EnbLtePhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## enb-phy.h (module 'lte'): void ns3::EnbLtePhy::ReceiveIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('ReceiveIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## enb-phy.h (module 'lte'): void ns3::EnbLtePhy::SendIdealControlMessage(ns3::Ptr<ns3::IdealControlMessage> msg) [member function]
    cls.add_method('SendIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::IdealControlMessage >', 'msg')], 
                   is_virtual=True)
    ## enb-phy.h (module 'lte'): bool ns3::EnbLtePhy::SendPacket(ns3::Ptr<ns3::PacketBurst> pb) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'pb')], 
                   is_virtual=True)
    ## enb-phy.h (module 'lte'): void ns3::EnbLtePhy::StartFrame() [member function]
    cls.add_method('StartFrame', 
                   'void', 
                   [])
    ## enb-phy.h (module 'lte'): void ns3::EnbLtePhy::StartSubFrame() [member function]
    cls.add_method('StartSubFrame', 
                   'void', 
                   [])
    return

def register_Ns3EnbLteSpectrumPhy_methods(root_module, cls):
    ## enb-lte-spectrum-phy.h (module 'lte'): ns3::EnbLteSpectrumPhy::EnbLteSpectrumPhy(ns3::EnbLteSpectrumPhy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnbLteSpectrumPhy const &', 'arg0')])
    ## enb-lte-spectrum-phy.h (module 'lte'): ns3::EnbLteSpectrumPhy::EnbLteSpectrumPhy() [constructor]
    cls.add_constructor([])
    ## enb-lte-spectrum-phy.h (module 'lte'): void ns3::EnbLteSpectrumPhy::CalcSinrValues(ns3::Ptr<ns3::SpectrumValue const> rxPsd, ns3::Ptr<ns3::SpectrumValue const> noise) [member function]
    cls.add_method('CalcSinrValues', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumValue const >', 'rxPsd'), param('ns3::Ptr< ns3::SpectrumValue const >', 'noise')], 
                   is_virtual=True)
    ## enb-lte-spectrum-phy.h (module 'lte'): static ns3::TypeId ns3::EnbLteSpectrumPhy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3EnbMacEntity_methods(root_module, cls):
    ## enb-mac-entity.h (module 'lte'): ns3::EnbMacEntity::EnbMacEntity(ns3::EnbMacEntity const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnbMacEntity const &', 'arg0')])
    ## enb-mac-entity.h (module 'lte'): ns3::EnbMacEntity::EnbMacEntity() [constructor]
    cls.add_constructor([])
    ## enb-mac-entity.h (module 'lte'): void ns3::EnbMacEntity::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-mac-entity.h (module 'lte'): ns3::Ptr<ns3::PacketScheduler> ns3::EnbMacEntity::GetDownlinkPacketScheduler() [member function]
    cls.add_method('GetDownlinkPacketScheduler', 
                   'ns3::Ptr< ns3::PacketScheduler >', 
                   [])
    ## enb-mac-entity.h (module 'lte'): static ns3::TypeId ns3::EnbMacEntity::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## enb-mac-entity.h (module 'lte'): ns3::Ptr<ns3::PacketScheduler> ns3::EnbMacEntity::GetUplinkPacketScheduler() [member function]
    cls.add_method('GetUplinkPacketScheduler', 
                   'ns3::Ptr< ns3::PacketScheduler >', 
                   [])
    ## enb-mac-entity.h (module 'lte'): void ns3::EnbMacEntity::ReceiveCqiIdealControlMessage(ns3::Ptr<ns3::CqiIdealControlMessage> msg) [member function]
    cls.add_method('ReceiveCqiIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::CqiIdealControlMessage >', 'msg')])
    ## enb-mac-entity.h (module 'lte'): void ns3::EnbMacEntity::SendPdcchMapIdealControlMessage(ns3::Ptr<ns3::PdcchMapIdealControlMessage> msg) [member function]
    cls.add_method('SendPdcchMapIdealControlMessage', 
                   'void', 
                   [param('ns3::Ptr< ns3::PdcchMapIdealControlMessage >', 'msg')])
    ## enb-mac-entity.h (module 'lte'): void ns3::EnbMacEntity::SetDownlinkPacketScheduler(ns3::Ptr<ns3::PacketScheduler> s) [member function]
    cls.add_method('SetDownlinkPacketScheduler', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketScheduler >', 's')])
    ## enb-mac-entity.h (module 'lte'): void ns3::EnbMacEntity::SetUplinkPacketScheduler(ns3::Ptr<ns3::PacketScheduler> s) [member function]
    cls.add_method('SetUplinkPacketScheduler', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketScheduler >', 's')])
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
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetGenericPhyTxStartCallback(ns3::GenericPhyTxStartCallback c) [member function]
    cls.add_method('SetGenericPhyTxStartCallback', 
                   'void', 
                   [param('ns3::GenericPhyTxStartCallback', 'c')])
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetPhy(ns3::Ptr<ns3::LtePhy> phy) [member function]
    cls.add_method('SetPhy', 
                   'void', 
                   [param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## lte-net-device.h (module 'lte'): ns3::Ptr<ns3::LtePhy> ns3::LteNetDevice::GetPhy() const [member function]
    cls.add_method('GetPhy', 
                   'ns3::Ptr< ns3::LtePhy >', 
                   [], 
                   is_const=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetRrcEntity(ns3::Ptr<ns3::RrcEntity> rrc) [member function]
    cls.add_method('SetRrcEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::RrcEntity >', 'rrc')])
    ## lte-net-device.h (module 'lte'): ns3::Ptr<ns3::RrcEntity> ns3::LteNetDevice::GetRrcEntity() [member function]
    cls.add_method('GetRrcEntity', 
                   'ns3::Ptr< ns3::RrcEntity >', 
                   [])
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
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
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
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::ForwardUp(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest')])
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::ForwardUp(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('ForwardUp', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::SetPacketToSend(ns3::Ptr<ns3::PacketBurst> p) [member function]
    cls.add_method('SetPacketToSend', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p')])
    ## lte-net-device.h (module 'lte'): ns3::Ptr<ns3::PacketBurst> ns3::LteNetDevice::GetPacketToSend() [member function]
    cls.add_method('GetPacketToSend', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [])
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::StartTransmission() [member function]
    cls.add_method('StartTransmission', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::SendPacket(ns3::Ptr<ns3::PacketBurst> p) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## lte-net-device.h (module 'lte'): bool ns3::LteNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## lte-net-device.h (module 'lte'): void ns3::LteNetDevice::DoReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3SingleModelSpectrumChannel_methods(root_module, cls):
    ## single-model-spectrum-channel.h (module 'spectrum'): ns3::SingleModelSpectrumChannel::SingleModelSpectrumChannel(ns3::SingleModelSpectrumChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SingleModelSpectrumChannel const &', 'arg0')])
    ## single-model-spectrum-channel.h (module 'spectrum'): ns3::SingleModelSpectrumChannel::SingleModelSpectrumChannel() [constructor]
    cls.add_constructor([])
    ## single-model-spectrum-channel.h (module 'spectrum'): void ns3::SingleModelSpectrumChannel::AddRx(ns3::Ptr<ns3::SpectrumPhy> phy) [member function]
    cls.add_method('AddRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPhy >', 'phy')], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): void ns3::SingleModelSpectrumChannel::AddSpectrumPropagationLossModel(ns3::Ptr<ns3::SpectrumPropagationLossModel> loss) [member function]
    cls.add_method('AddSpectrumPropagationLossModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SpectrumPropagationLossModel >', 'loss')], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): ns3::Ptr<ns3::NetDevice> ns3::SingleModelSpectrumChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): uint32_t ns3::SingleModelSpectrumChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): ns3::Ptr<ns3::SpectrumPropagationLossModel> ns3::SingleModelSpectrumChannel::GetSpectrumPropagationLossModel() [member function]
    cls.add_method('GetSpectrumPropagationLossModel', 
                   'ns3::Ptr< ns3::SpectrumPropagationLossModel >', 
                   [], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): static ns3::TypeId ns3::SingleModelSpectrumChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): void ns3::SingleModelSpectrumChannel::SetPropagationDelayModel(ns3::Ptr<ns3::PropagationDelayModel> delay) [member function]
    cls.add_method('SetPropagationDelayModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::PropagationDelayModel >', 'delay')], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): void ns3::SingleModelSpectrumChannel::StartTx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> txPsd, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> sender) [member function]
    cls.add_method('StartTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'txPsd'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'sender')], 
                   is_virtual=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): void ns3::SingleModelSpectrumChannel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## single-model-spectrum-channel.h (module 'spectrum'): void ns3::SingleModelSpectrumChannel::StartRx(ns3::Ptr<ns3::PacketBurst> p, ns3::Ptr<ns3::SpectrumValue> rxPowerSpectrum, ns3::SpectrumType st, ns3::Time duration, ns3::Ptr<ns3::SpectrumPhy> receiver) [member function]
    cls.add_method('StartRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p'), param('ns3::Ptr< ns3::SpectrumValue >', 'rxPowerSpectrum'), param('ns3::SpectrumType', 'st'), param('ns3::Time', 'duration'), param('ns3::Ptr< ns3::SpectrumPhy >', 'receiver')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3UeNetDevice_methods(root_module, cls):
    ## ue-net-device.h (module 'lte'): static ns3::TypeId ns3::UeNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ue-net-device.h (module 'lte'): ns3::UeNetDevice::UeNetDevice() [constructor]
    cls.add_constructor([])
    ## ue-net-device.h (module 'lte'): ns3::UeNetDevice::UeNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::LtePhy> phy) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## ue-net-device.h (module 'lte'): ns3::UeNetDevice::UeNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::LtePhy> phy, ns3::Ptr<ns3::EnbNetDevice> targetEnb) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::LtePhy >', 'phy'), param('ns3::Ptr< ns3::EnbNetDevice >', 'targetEnb')])
    ## ue-net-device.h (module 'lte'): void ns3::UeNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-net-device.h (module 'lte'): void ns3::UeNetDevice::SetMacEntity(ns3::Ptr<ns3::UeMacEntity> m) [member function]
    cls.add_method('SetMacEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeMacEntity >', 'm')])
    ## ue-net-device.h (module 'lte'): ns3::Ptr<ns3::UeMacEntity> ns3::UeNetDevice::GetMacEntity() [member function]
    cls.add_method('GetMacEntity', 
                   'ns3::Ptr< ns3::UeMacEntity >', 
                   [])
    ## ue-net-device.h (module 'lte'): void ns3::UeNetDevice::InitUeNetDevice() [member function]
    cls.add_method('InitUeNetDevice', 
                   'void', 
                   [])
    ## ue-net-device.h (module 'lte'): void ns3::UeNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-net-device.h (module 'lte'): void ns3::UeNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-net-device.h (module 'lte'): void ns3::UeNetDevice::SetTargetEnb(ns3::Ptr<ns3::EnbNetDevice> enb) [member function]
    cls.add_method('SetTargetEnb', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnbNetDevice >', 'enb')])
    ## ue-net-device.h (module 'lte'): ns3::Ptr<ns3::EnbNetDevice> ns3::UeNetDevice::GetTargetEnb() [member function]
    cls.add_method('GetTargetEnb', 
                   'ns3::Ptr< ns3::EnbNetDevice >', 
                   [])
    ## ue-net-device.h (module 'lte'): void ns3::UeNetDevice::StartTransmission() [member function]
    cls.add_method('StartTransmission', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ue-net-device.h (module 'lte'): bool ns3::UeNetDevice::SendPacket(ns3::Ptr<ns3::PacketBurst> p) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p')], 
                   is_virtual=True)
    ## ue-net-device.h (module 'lte'): bool ns3::UeNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   visibility='private', is_virtual=True)
    ## ue-net-device.h (module 'lte'): void ns3::UeNetDevice::DoReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3EnbNetDevice_methods(root_module, cls):
    ## enb-net-device.h (module 'lte'): static ns3::TypeId ns3::EnbNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## enb-net-device.h (module 'lte'): ns3::EnbNetDevice::EnbNetDevice() [constructor]
    cls.add_constructor([])
    ## enb-net-device.h (module 'lte'): ns3::EnbNetDevice::EnbNetDevice(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::LtePhy> phy) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::LtePhy >', 'phy')])
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::InitEnbNetDevice() [member function]
    cls.add_method('InitEnbNetDevice', 
                   'void', 
                   [])
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::SetUeManager(ns3::Ptr<ns3::UeManager> m) [member function]
    cls.add_method('SetUeManager', 
                   'void', 
                   [param('ns3::Ptr< ns3::UeManager >', 'm')])
    ## enb-net-device.h (module 'lte'): ns3::Ptr<ns3::UeManager> ns3::EnbNetDevice::GetUeManager() [member function]
    cls.add_method('GetUeManager', 
                   'ns3::Ptr< ns3::UeManager >', 
                   [])
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::SetMacEntity(ns3::Ptr<ns3::EnbMacEntity> m) [member function]
    cls.add_method('SetMacEntity', 
                   'void', 
                   [param('ns3::Ptr< ns3::EnbMacEntity >', 'm')])
    ## enb-net-device.h (module 'lte'): ns3::Ptr<ns3::EnbMacEntity> ns3::EnbNetDevice::GetMacEntity() [member function]
    cls.add_method('GetMacEntity', 
                   'ns3::Ptr< ns3::EnbMacEntity >', 
                   [])
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::StartTransmission() [member function]
    cls.add_method('StartTransmission', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## enb-net-device.h (module 'lte'): bool ns3::EnbNetDevice::SendPacket(ns3::Ptr<ns3::PacketBurst> p) [member function]
    cls.add_method('SendPacket', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PacketBurst >', 'p')], 
                   is_virtual=True)
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::SendIdealPdcchMessage() [member function]
    cls.add_method('SendIdealPdcchMessage', 
                   'void', 
                   [])
    ## enb-net-device.h (module 'lte'): bool ns3::EnbNetDevice::DoSend(ns3::Ptr<ns3::Packet> packet, ns3::Mac48Address const & source, ns3::Mac48Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('DoSend', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Mac48Address const &', 'source'), param('ns3::Mac48Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   visibility='private', is_virtual=True)
    ## enb-net-device.h (module 'lte'): void ns3::EnbNetDevice::DoReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
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

