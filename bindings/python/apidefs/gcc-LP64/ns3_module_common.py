from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## error-model.h: ns3::ErrorUnit [enumeration]
    module.add_enum('ErrorUnit', ['EU_BIT', 'EU_BYTE', 'EU_PKT'])
    ## spectrum-model.h: ns3::BandInfo [struct]
    module.add_class('BandInfo')
    ## buffer.h: ns3::Buffer [class]
    module.add_class('Buffer')
    ## buffer.h: ns3::Buffer::Iterator [class]
    module.add_class('Iterator', outer_class=root_module['ns3::Buffer'])
    ## packet.h: ns3::ByteTagIterator [class]
    module.add_class('ByteTagIterator')
    ## packet.h: ns3::ByteTagIterator::Item [class]
    module.add_class('Item', outer_class=root_module['ns3::ByteTagIterator'])
    ## byte-tag-list.h: ns3::ByteTagList [class]
    module.add_class('ByteTagList')
    ## byte-tag-list.h: ns3::ByteTagList::Iterator [class]
    module.add_class('Iterator', outer_class=root_module['ns3::ByteTagList'])
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item [struct]
    module.add_class('Item', outer_class=root_module['ns3::ByteTagList::Iterator'])
    ## data-rate.h: ns3::DataRate [class]
    module.add_class('DataRate')
    ## packet-metadata.h: ns3::PacketMetadata [class]
    module.add_class('PacketMetadata')
    ## packet-metadata.h: ns3::PacketMetadata::Item [struct]
    module.add_class('Item', outer_class=root_module['ns3::PacketMetadata'])
    ## packet-metadata.h: ns3::PacketMetadata::Item [enumeration]
    module.add_enum('', ['PAYLOAD', 'HEADER', 'TRAILER'], outer_class=root_module['ns3::PacketMetadata::Item'])
    ## packet-metadata.h: ns3::PacketMetadata::ItemIterator [class]
    module.add_class('ItemIterator', outer_class=root_module['ns3::PacketMetadata'])
    ## packet.h: ns3::PacketTagIterator [class]
    module.add_class('PacketTagIterator')
    ## packet.h: ns3::PacketTagIterator::Item [class]
    module.add_class('Item', outer_class=root_module['ns3::PacketTagIterator'])
    ## packet-tag-list.h: ns3::PacketTagList [class]
    module.add_class('PacketTagList')
    ## packet-tag-list.h: ns3::PacketTagList::TagData [struct]
    module.add_class('TagData', outer_class=root_module['ns3::PacketTagList'])
    ## pcap-file.h: ns3::PcapFile [class]
    module.add_class('PcapFile')
    ## sequence-number.h: ns3::SequenceNumber<unsigned int, int> [class]
    module.add_class('SequenceNumber32')
    ## spectrum-type.h: ns3::SpectrumType [class]
    module.add_class('SpectrumType')
    ## spectrum-type.h: ns3::SpectrumTypeFactory [class]
    module.add_class('SpectrumTypeFactory')
    ## tag.h: ns3::Tag [class]
    module.add_class('Tag', parent=root_module['ns3::ObjectBase'])
    ## tag-buffer.h: ns3::TagBuffer [class]
    module.add_class('TagBuffer')
    ## chunk.h: ns3::Chunk [class]
    module.add_class('Chunk', parent=root_module['ns3::ObjectBase'])
    ## header.h: ns3::Header [class]
    module.add_class('Header', parent=root_module['ns3::Chunk'])
    ## packet-burst.h: ns3::PacketBurst [class]
    module.add_class('PacketBurst', parent=root_module['ns3::Object'])
    ## pcap-file-wrapper.h: ns3::PcapFileWrapper [class]
    module.add_class('PcapFileWrapper', parent=root_module['ns3::Object'])
    ## propagation-delay-model.h: ns3::PropagationDelayModel [class]
    module.add_class('PropagationDelayModel', parent=root_module['ns3::Object'])
    ## propagation-loss-model.h: ns3::PropagationLossModel [class]
    module.add_class('PropagationLossModel', parent=root_module['ns3::Object'])
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel [class]
    module.add_class('RandomPropagationDelayModel', parent=root_module['ns3::PropagationDelayModel'])
    ## propagation-loss-model.h: ns3::RandomPropagationLossModel [class]
    module.add_class('RandomPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::RangePropagationLossModel [class]
    module.add_class('RangePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::NixVector', 'ns3::empty', 'ns3::DefaultDeleter<ns3::NixVector>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::OutputStreamWrapper', 'ns3::empty', 'ns3::DefaultDeleter<ns3::OutputStreamWrapper>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Packet', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Packet>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::SpectrumConverter', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumConverter>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::SpectrumModel', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumModel>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::SpectrumValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SpectrumValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## spectrum-converter.h: ns3::SpectrumConverter [class]
    module.add_class('SpectrumConverter', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> >'])
    ## spectrum-model.h: ns3::SpectrumModel [class]
    module.add_class('SpectrumModel', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >'])
    ## spectrum-propagation-loss-model.h: ns3::SpectrumPropagationLossModel [class]
    module.add_class('SpectrumPropagationLossModel', parent=root_module['ns3::Object'])
    ## spectrum-value.h: ns3::SpectrumValue [class]
    module.add_class('SpectrumValue', parent=root_module['ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >'])
    ## propagation-loss-model.h: ns3::ThreeLogDistancePropagationLossModel [class]
    module.add_class('ThreeLogDistancePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## trailer.h: ns3::Trailer [class]
    module.add_class('Trailer', parent=root_module['ns3::Chunk'])
    ## propagation-loss-model.h: ns3::TwoRayGroundPropagationLossModel [class]
    module.add_class('TwoRayGroundPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel [class]
    module.add_class('ConstantSpeedPropagationDelayModel', parent=root_module['ns3::PropagationDelayModel'])
    ## cost231-propagation-loss-model.h: ns3::Cost231PropagationLossModel [class]
    module.add_class('Cost231PropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## cost231-propagation-loss-model.h: ns3::Cost231PropagationLossModel::Environment [enumeration]
    module.add_enum('Environment', ['SubUrban', 'MediumCity', 'Metropolitan'], outer_class=root_module['ns3::Cost231PropagationLossModel'])
    ## data-rate.h: ns3::DataRateChecker [class]
    module.add_class('DataRateChecker', parent=root_module['ns3::AttributeChecker'])
    ## data-rate.h: ns3::DataRateValue [class]
    module.add_class('DataRateValue', parent=root_module['ns3::AttributeValue'])
    ## error-model.h: ns3::ErrorModel [class]
    module.add_class('ErrorModel', parent=root_module['ns3::Object'])
    ## propagation-loss-model.h: ns3::FixedRssLossModel [class]
    module.add_class('FixedRssLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::FriisPropagationLossModel [class]
    module.add_class('FriisPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## friis-spectrum-propagation-loss.h: ns3::FriisSpectrumPropagationLossModel [class]
    module.add_class('FriisSpectrumPropagationLossModel', parent=root_module['ns3::SpectrumPropagationLossModel'])
    ## jakes-propagation-loss-model.h: ns3::JakesPropagationLossModel [class]
    module.add_class('JakesPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## error-model.h: ns3::ListErrorModel [class]
    module.add_class('ListErrorModel', parent=root_module['ns3::ErrorModel'])
    ## propagation-loss-model.h: ns3::LogDistancePropagationLossModel [class]
    module.add_class('LogDistancePropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::MatrixPropagationLossModel [class]
    module.add_class('MatrixPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## propagation-loss-model.h: ns3::NakagamiPropagationLossModel [class]
    module.add_class('NakagamiPropagationLossModel', parent=root_module['ns3::PropagationLossModel'])
    ## nix-vector.h: ns3::NixVector [class]
    module.add_class('NixVector', parent=root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    ## output-stream-wrapper.h: ns3::OutputStreamWrapper [class]
    module.add_class('OutputStreamWrapper', parent=root_module['ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >'])
    ## packet.h: ns3::Packet [class]
    module.add_class('Packet', parent=root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    ## error-model.h: ns3::RateErrorModel [class]
    module.add_class('RateErrorModel', parent=root_module['ns3::ErrorModel'])
    ## error-model.h: ns3::ReceiveListErrorModel [class]
    module.add_class('ReceiveListErrorModel', parent=root_module['ns3::ErrorModel'])
    module.add_container('ns3::SpectrumConverterMap_t', ('unsigned int', 'ns3::SpectrumConverter'), container_type='map')
    module.add_container('ns3::Bands', 'ns3::BandInfo', container_type='vector')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >', 'ns3::SequenceNumber16')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >*', 'ns3::SequenceNumber16*')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >&', 'ns3::SequenceNumber16&')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >', 'ns3::SequenceNumber32')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >*', 'ns3::SequenceNumber32*')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >&', 'ns3::SequenceNumber32&')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >', 'ns3::Values')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >*', 'ns3::Values*')
    typehandlers.add_type_alias('std::vector< double, std::allocator< double > >&', 'ns3::Values&')
    typehandlers.add_type_alias('uint32_t', 'ns3::SpectrumModelUid_t')
    typehandlers.add_type_alias('uint32_t*', 'ns3::SpectrumModelUid_t*')
    typehandlers.add_type_alias('uint32_t&', 'ns3::SpectrumModelUid_t&')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >', 'ns3::Bands')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >*', 'ns3::Bands*')
    typehandlers.add_type_alias('std::vector< ns3::BandInfo, std::allocator< ns3::BandInfo > >&', 'ns3::Bands&')
    
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
    register_Ns3BandInfo_methods(root_module, root_module['ns3::BandInfo'])
    register_Ns3Buffer_methods(root_module, root_module['ns3::Buffer'])
    register_Ns3BufferIterator_methods(root_module, root_module['ns3::Buffer::Iterator'])
    register_Ns3ByteTagIterator_methods(root_module, root_module['ns3::ByteTagIterator'])
    register_Ns3ByteTagIteratorItem_methods(root_module, root_module['ns3::ByteTagIterator::Item'])
    register_Ns3ByteTagList_methods(root_module, root_module['ns3::ByteTagList'])
    register_Ns3ByteTagListIterator_methods(root_module, root_module['ns3::ByteTagList::Iterator'])
    register_Ns3ByteTagListIteratorItem_methods(root_module, root_module['ns3::ByteTagList::Iterator::Item'])
    register_Ns3DataRate_methods(root_module, root_module['ns3::DataRate'])
    register_Ns3PacketMetadata_methods(root_module, root_module['ns3::PacketMetadata'])
    register_Ns3PacketMetadataItem_methods(root_module, root_module['ns3::PacketMetadata::Item'])
    register_Ns3PacketMetadataItemIterator_methods(root_module, root_module['ns3::PacketMetadata::ItemIterator'])
    register_Ns3PacketTagIterator_methods(root_module, root_module['ns3::PacketTagIterator'])
    register_Ns3PacketTagIteratorItem_methods(root_module, root_module['ns3::PacketTagIterator::Item'])
    register_Ns3PacketTagList_methods(root_module, root_module['ns3::PacketTagList'])
    register_Ns3PacketTagListTagData_methods(root_module, root_module['ns3::PacketTagList::TagData'])
    register_Ns3PcapFile_methods(root_module, root_module['ns3::PcapFile'])
    register_Ns3SequenceNumber32_methods(root_module, root_module['ns3::SequenceNumber32'])
    register_Ns3SpectrumType_methods(root_module, root_module['ns3::SpectrumType'])
    register_Ns3SpectrumTypeFactory_methods(root_module, root_module['ns3::SpectrumTypeFactory'])
    register_Ns3Tag_methods(root_module, root_module['ns3::Tag'])
    register_Ns3TagBuffer_methods(root_module, root_module['ns3::TagBuffer'])
    register_Ns3Chunk_methods(root_module, root_module['ns3::Chunk'])
    register_Ns3Header_methods(root_module, root_module['ns3::Header'])
    register_Ns3PacketBurst_methods(root_module, root_module['ns3::PacketBurst'])
    register_Ns3PcapFileWrapper_methods(root_module, root_module['ns3::PcapFileWrapper'])
    register_Ns3PropagationDelayModel_methods(root_module, root_module['ns3::PropagationDelayModel'])
    register_Ns3PropagationLossModel_methods(root_module, root_module['ns3::PropagationLossModel'])
    register_Ns3RandomPropagationDelayModel_methods(root_module, root_module['ns3::RandomPropagationDelayModel'])
    register_Ns3RandomPropagationLossModel_methods(root_module, root_module['ns3::RandomPropagationLossModel'])
    register_Ns3RangePropagationLossModel_methods(root_module, root_module['ns3::RangePropagationLossModel'])
    register_Ns3SpectrumConverter_methods(root_module, root_module['ns3::SpectrumConverter'])
    register_Ns3SpectrumModel_methods(root_module, root_module['ns3::SpectrumModel'])
    register_Ns3SpectrumPropagationLossModel_methods(root_module, root_module['ns3::SpectrumPropagationLossModel'])
    register_Ns3SpectrumValue_methods(root_module, root_module['ns3::SpectrumValue'])
    register_Ns3ThreeLogDistancePropagationLossModel_methods(root_module, root_module['ns3::ThreeLogDistancePropagationLossModel'])
    register_Ns3Trailer_methods(root_module, root_module['ns3::Trailer'])
    register_Ns3TwoRayGroundPropagationLossModel_methods(root_module, root_module['ns3::TwoRayGroundPropagationLossModel'])
    register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, root_module['ns3::ConstantSpeedPropagationDelayModel'])
    register_Ns3Cost231PropagationLossModel_methods(root_module, root_module['ns3::Cost231PropagationLossModel'])
    register_Ns3DataRateChecker_methods(root_module, root_module['ns3::DataRateChecker'])
    register_Ns3DataRateValue_methods(root_module, root_module['ns3::DataRateValue'])
    register_Ns3ErrorModel_methods(root_module, root_module['ns3::ErrorModel'])
    register_Ns3FixedRssLossModel_methods(root_module, root_module['ns3::FixedRssLossModel'])
    register_Ns3FriisPropagationLossModel_methods(root_module, root_module['ns3::FriisPropagationLossModel'])
    register_Ns3FriisSpectrumPropagationLossModel_methods(root_module, root_module['ns3::FriisSpectrumPropagationLossModel'])
    register_Ns3JakesPropagationLossModel_methods(root_module, root_module['ns3::JakesPropagationLossModel'])
    register_Ns3ListErrorModel_methods(root_module, root_module['ns3::ListErrorModel'])
    register_Ns3LogDistancePropagationLossModel_methods(root_module, root_module['ns3::LogDistancePropagationLossModel'])
    register_Ns3MatrixPropagationLossModel_methods(root_module, root_module['ns3::MatrixPropagationLossModel'])
    register_Ns3NakagamiPropagationLossModel_methods(root_module, root_module['ns3::NakagamiPropagationLossModel'])
    register_Ns3NixVector_methods(root_module, root_module['ns3::NixVector'])
    register_Ns3OutputStreamWrapper_methods(root_module, root_module['ns3::OutputStreamWrapper'])
    register_Ns3Packet_methods(root_module, root_module['ns3::Packet'])
    register_Ns3RateErrorModel_methods(root_module, root_module['ns3::RateErrorModel'])
    register_Ns3ReceiveListErrorModel_methods(root_module, root_module['ns3::ReceiveListErrorModel'])
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

def register_Ns3Buffer_methods(root_module, cls):
    ## buffer.h: ns3::Buffer::Buffer() [constructor]
    cls.add_constructor([])
    ## buffer.h: ns3::Buffer::Buffer(uint32_t dataSize) [constructor]
    cls.add_constructor([param('uint32_t', 'dataSize')])
    ## buffer.h: ns3::Buffer::Buffer(uint32_t dataSize, bool initialize) [constructor]
    cls.add_constructor([param('uint32_t', 'dataSize'), param('bool', 'initialize')])
    ## buffer.h: ns3::Buffer::Buffer(ns3::Buffer const & o) [copy constructor]
    cls.add_constructor([param('ns3::Buffer const &', 'o')])
    ## buffer.h: bool ns3::Buffer::AddAtEnd(uint32_t end) [member function]
    cls.add_method('AddAtEnd', 
                   'bool', 
                   [param('uint32_t', 'end')])
    ## buffer.h: void ns3::Buffer::AddAtEnd(ns3::Buffer const & o) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Buffer const &', 'o')])
    ## buffer.h: bool ns3::Buffer::AddAtStart(uint32_t start) [member function]
    cls.add_method('AddAtStart', 
                   'bool', 
                   [param('uint32_t', 'start')])
    ## buffer.h: ns3::Buffer::Iterator ns3::Buffer::Begin() const [member function]
    cls.add_method('Begin', 
                   'ns3::Buffer::Iterator', 
                   [], 
                   is_const=True)
    ## buffer.h: void ns3::Buffer::CopyData(std::ostream * os, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'void', 
                   [param('std::ostream *', 'os'), param('uint32_t', 'size')], 
                   is_const=True)
    ## buffer.h: uint32_t ns3::Buffer::CopyData(uint8_t * buffer, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')], 
                   is_const=True)
    ## buffer.h: ns3::Buffer ns3::Buffer::CreateFragment(uint32_t start, uint32_t length) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::Buffer', 
                   [param('uint32_t', 'start'), param('uint32_t', 'length')], 
                   is_const=True)
    ## buffer.h: ns3::Buffer ns3::Buffer::CreateFullCopy() const [member function]
    cls.add_method('CreateFullCopy', 
                   'ns3::Buffer', 
                   [], 
                   is_const=True)
    ## buffer.h: uint32_t ns3::Buffer::Deserialize(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h: ns3::Buffer::Iterator ns3::Buffer::End() const [member function]
    cls.add_method('End', 
                   'ns3::Buffer::Iterator', 
                   [], 
                   is_const=True)
    ## buffer.h: int32_t ns3::Buffer::GetCurrentEndOffset() const [member function]
    cls.add_method('GetCurrentEndOffset', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## buffer.h: int32_t ns3::Buffer::GetCurrentStartOffset() const [member function]
    cls.add_method('GetCurrentStartOffset', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## buffer.h: uint32_t ns3::Buffer::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h: uint32_t ns3::Buffer::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h: uint8_t const * ns3::Buffer::PeekData() const [member function]
    cls.add_method('PeekData', 
                   'uint8_t const *', 
                   [], 
                   is_const=True)
    ## buffer.h: void ns3::Buffer::RemoveAtEnd(uint32_t end) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## buffer.h: void ns3::Buffer::RemoveAtStart(uint32_t start) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'start')])
    ## buffer.h: uint32_t ns3::Buffer::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3BufferIterator_methods(root_module, cls):
    ## buffer.h: ns3::Buffer::Iterator::Iterator(ns3::Buffer::Iterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator const &', 'arg0')])
    ## buffer.h: ns3::Buffer::Iterator::Iterator() [constructor]
    cls.add_constructor([])
    ## buffer.h: uint16_t ns3::Buffer::Iterator::CalculateIpChecksum(uint16_t size) [member function]
    cls.add_method('CalculateIpChecksum', 
                   'uint16_t', 
                   [param('uint16_t', 'size')])
    ## buffer.h: uint16_t ns3::Buffer::Iterator::CalculateIpChecksum(uint16_t size, uint32_t initialChecksum) [member function]
    cls.add_method('CalculateIpChecksum', 
                   'uint16_t', 
                   [param('uint16_t', 'size'), param('uint32_t', 'initialChecksum')])
    ## buffer.h: uint32_t ns3::Buffer::Iterator::GetDistanceFrom(ns3::Buffer::Iterator const & o) const [member function]
    cls.add_method('GetDistanceFrom', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator const &', 'o')], 
                   is_const=True)
    ## buffer.h: uint32_t ns3::Buffer::Iterator::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h: bool ns3::Buffer::Iterator::IsEnd() const [member function]
    cls.add_method('IsEnd', 
                   'bool', 
                   [], 
                   is_const=True)
    ## buffer.h: bool ns3::Buffer::Iterator::IsStart() const [member function]
    cls.add_method('IsStart', 
                   'bool', 
                   [], 
                   is_const=True)
    ## buffer.h: void ns3::Buffer::Iterator::Next() [member function]
    cls.add_method('Next', 
                   'void', 
                   [])
    ## buffer.h: void ns3::Buffer::Iterator::Next(uint32_t delta) [member function]
    cls.add_method('Next', 
                   'void', 
                   [param('uint32_t', 'delta')])
    ## buffer.h: void ns3::Buffer::Iterator::Prev() [member function]
    cls.add_method('Prev', 
                   'void', 
                   [])
    ## buffer.h: void ns3::Buffer::Iterator::Prev(uint32_t delta) [member function]
    cls.add_method('Prev', 
                   'void', 
                   [param('uint32_t', 'delta')])
    ## buffer.h: void ns3::Buffer::Iterator::Read(uint8_t * buffer, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h: uint16_t ns3::Buffer::Iterator::ReadLsbtohU16() [member function]
    cls.add_method('ReadLsbtohU16', 
                   'uint16_t', 
                   [])
    ## buffer.h: uint32_t ns3::Buffer::Iterator::ReadLsbtohU32() [member function]
    cls.add_method('ReadLsbtohU32', 
                   'uint32_t', 
                   [])
    ## buffer.h: uint64_t ns3::Buffer::Iterator::ReadLsbtohU64() [member function]
    cls.add_method('ReadLsbtohU64', 
                   'uint64_t', 
                   [])
    ## buffer.h: uint16_t ns3::Buffer::Iterator::ReadNtohU16() [member function]
    cls.add_method('ReadNtohU16', 
                   'uint16_t', 
                   [])
    ## buffer.h: uint32_t ns3::Buffer::Iterator::ReadNtohU32() [member function]
    cls.add_method('ReadNtohU32', 
                   'uint32_t', 
                   [])
    ## buffer.h: uint64_t ns3::Buffer::Iterator::ReadNtohU64() [member function]
    cls.add_method('ReadNtohU64', 
                   'uint64_t', 
                   [])
    ## buffer.h: uint16_t ns3::Buffer::Iterator::ReadU16() [member function]
    cls.add_method('ReadU16', 
                   'uint16_t', 
                   [])
    ## buffer.h: uint32_t ns3::Buffer::Iterator::ReadU32() [member function]
    cls.add_method('ReadU32', 
                   'uint32_t', 
                   [])
    ## buffer.h: uint64_t ns3::Buffer::Iterator::ReadU64() [member function]
    cls.add_method('ReadU64', 
                   'uint64_t', 
                   [])
    ## buffer.h: uint8_t ns3::Buffer::Iterator::ReadU8() [member function]
    cls.add_method('ReadU8', 
                   'uint8_t', 
                   [])
    ## buffer.h: void ns3::Buffer::Iterator::Write(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h: void ns3::Buffer::Iterator::Write(ns3::Buffer::Iterator start, ns3::Buffer::Iterator end) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start'), param('ns3::Buffer::Iterator', 'end')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteHtolsbU16(uint16_t data) [member function]
    cls.add_method('WriteHtolsbU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteHtolsbU32(uint32_t data) [member function]
    cls.add_method('WriteHtolsbU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteHtolsbU64(uint64_t data) [member function]
    cls.add_method('WriteHtolsbU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteHtonU16(uint16_t data) [member function]
    cls.add_method('WriteHtonU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteHtonU32(uint32_t data) [member function]
    cls.add_method('WriteHtonU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteHtonU64(uint64_t data) [member function]
    cls.add_method('WriteHtonU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteU16(uint16_t data) [member function]
    cls.add_method('WriteU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteU32(uint32_t data) [member function]
    cls.add_method('WriteU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteU64(uint64_t data) [member function]
    cls.add_method('WriteU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteU8(uint8_t data) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteU8(uint8_t data, uint32_t len) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'data'), param('uint32_t', 'len')])
    return

def register_Ns3ByteTagIterator_methods(root_module, cls):
    ## packet.h: ns3::ByteTagIterator::ByteTagIterator(ns3::ByteTagIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagIterator const &', 'arg0')])
    ## packet.h: bool ns3::ByteTagIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet.h: ns3::ByteTagIterator::Item ns3::ByteTagIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::ByteTagIterator::Item', 
                   [])
    return

def register_Ns3ByteTagIteratorItem_methods(root_module, cls):
    ## packet.h: ns3::ByteTagIterator::Item::Item(ns3::ByteTagIterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagIterator::Item const &', 'arg0')])
    ## packet.h: uint32_t ns3::ByteTagIterator::Item::GetEnd() const [member function]
    cls.add_method('GetEnd', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h: uint32_t ns3::ByteTagIterator::Item::GetStart() const [member function]
    cls.add_method('GetStart', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h: void ns3::ByteTagIterator::Item::GetTag(ns3::Tag & tag) const [member function]
    cls.add_method('GetTag', 
                   'void', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h: ns3::TypeId ns3::ByteTagIterator::Item::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    return

def register_Ns3ByteTagList_methods(root_module, cls):
    ## byte-tag-list.h: ns3::ByteTagList::ByteTagList() [constructor]
    cls.add_constructor([])
    ## byte-tag-list.h: ns3::ByteTagList::ByteTagList(ns3::ByteTagList const & o) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList const &', 'o')])
    ## byte-tag-list.h: ns3::TagBuffer ns3::ByteTagList::Add(ns3::TypeId tid, uint32_t bufferSize, int32_t start, int32_t end) [member function]
    cls.add_method('Add', 
                   'ns3::TagBuffer', 
                   [param('ns3::TypeId', 'tid'), param('uint32_t', 'bufferSize'), param('int32_t', 'start'), param('int32_t', 'end')])
    ## byte-tag-list.h: void ns3::ByteTagList::Add(ns3::ByteTagList const & o) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::ByteTagList const &', 'o')])
    ## byte-tag-list.h: void ns3::ByteTagList::AddAtEnd(int32_t adjustment, int32_t appendOffset) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('int32_t', 'adjustment'), param('int32_t', 'appendOffset')])
    ## byte-tag-list.h: void ns3::ByteTagList::AddAtStart(int32_t adjustment, int32_t prependOffset) [member function]
    cls.add_method('AddAtStart', 
                   'void', 
                   [param('int32_t', 'adjustment'), param('int32_t', 'prependOffset')])
    ## byte-tag-list.h: ns3::ByteTagList::Iterator ns3::ByteTagList::Begin(int32_t offsetStart, int32_t offsetEnd) const [member function]
    cls.add_method('Begin', 
                   'ns3::ByteTagList::Iterator', 
                   [param('int32_t', 'offsetStart'), param('int32_t', 'offsetEnd')], 
                   is_const=True)
    ## byte-tag-list.h: void ns3::ByteTagList::RemoveAll() [member function]
    cls.add_method('RemoveAll', 
                   'void', 
                   [])
    return

def register_Ns3ByteTagListIterator_methods(root_module, cls):
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Iterator(ns3::ByteTagList::Iterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList::Iterator const &', 'arg0')])
    ## byte-tag-list.h: uint32_t ns3::ByteTagList::Iterator::GetOffsetStart() const [member function]
    cls.add_method('GetOffsetStart', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## byte-tag-list.h: bool ns3::ByteTagList::Iterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item ns3::ByteTagList::Iterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::ByteTagList::Iterator::Item', 
                   [])
    return

def register_Ns3ByteTagListIteratorItem_methods(root_module, cls):
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item::Item(ns3::ByteTagList::Iterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList::Iterator::Item const &', 'arg0')])
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item::Item(ns3::TagBuffer buf) [constructor]
    cls.add_constructor([param('ns3::TagBuffer', 'buf')])
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item::buf [variable]
    cls.add_instance_attribute('buf', 'ns3::TagBuffer', is_const=False)
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item::end [variable]
    cls.add_instance_attribute('end', 'int32_t', is_const=False)
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item::size [variable]
    cls.add_instance_attribute('size', 'uint32_t', is_const=False)
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item::start [variable]
    cls.add_instance_attribute('start', 'int32_t', is_const=False)
    ## byte-tag-list.h: ns3::ByteTagList::Iterator::Item::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3DataRate_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## data-rate.h: ns3::DataRate::DataRate(ns3::DataRate const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRate const &', 'arg0')])
    ## data-rate.h: ns3::DataRate::DataRate() [constructor]
    cls.add_constructor([])
    ## data-rate.h: ns3::DataRate::DataRate(uint64_t bps) [constructor]
    cls.add_constructor([param('uint64_t', 'bps')])
    ## data-rate.h: ns3::DataRate::DataRate(std::string rate) [constructor]
    cls.add_constructor([param('std::string', 'rate')])
    ## data-rate.h: double ns3::DataRate::CalculateTxTime(uint32_t bytes) const [member function]
    cls.add_method('CalculateTxTime', 
                   'double', 
                   [param('uint32_t', 'bytes')], 
                   is_const=True)
    ## data-rate.h: uint64_t ns3::DataRate::GetBitRate() const [member function]
    cls.add_method('GetBitRate', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3PacketMetadata_methods(root_module, cls):
    ## packet-metadata.h: ns3::PacketMetadata::PacketMetadata(uint64_t uid, uint32_t size) [constructor]
    cls.add_constructor([param('uint64_t', 'uid'), param('uint32_t', 'size')])
    ## packet-metadata.h: ns3::PacketMetadata::PacketMetadata(ns3::PacketMetadata const & o) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata const &', 'o')])
    ## packet-metadata.h: void ns3::PacketMetadata::AddAtEnd(ns3::PacketMetadata const & o) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::PacketMetadata const &', 'o')])
    ## packet-metadata.h: void ns3::PacketMetadata::AddHeader(ns3::Header const & header, uint32_t size) [member function]
    cls.add_method('AddHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header'), param('uint32_t', 'size')])
    ## packet-metadata.h: void ns3::PacketMetadata::AddPaddingAtEnd(uint32_t end) [member function]
    cls.add_method('AddPaddingAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## packet-metadata.h: void ns3::PacketMetadata::AddTrailer(ns3::Trailer const & trailer, uint32_t size) [member function]
    cls.add_method('AddTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer'), param('uint32_t', 'size')])
    ## packet-metadata.h: ns3::PacketMetadata::ItemIterator ns3::PacketMetadata::BeginItem(ns3::Buffer buffer) const [member function]
    cls.add_method('BeginItem', 
                   'ns3::PacketMetadata::ItemIterator', 
                   [param('ns3::Buffer', 'buffer')], 
                   is_const=True)
    ## packet-metadata.h: ns3::PacketMetadata ns3::PacketMetadata::CreateFragment(uint32_t start, uint32_t end) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::PacketMetadata', 
                   [param('uint32_t', 'start'), param('uint32_t', 'end')], 
                   is_const=True)
    ## packet-metadata.h: uint32_t ns3::PacketMetadata::Deserialize(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## packet-metadata.h: static void ns3::PacketMetadata::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet-metadata.h: static void ns3::PacketMetadata::EnableChecking() [member function]
    cls.add_method('EnableChecking', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet-metadata.h: uint32_t ns3::PacketMetadata::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-metadata.h: uint64_t ns3::PacketMetadata::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## packet-metadata.h: void ns3::PacketMetadata::RemoveAtEnd(uint32_t end) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## packet-metadata.h: void ns3::PacketMetadata::RemoveAtStart(uint32_t start) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'start')])
    ## packet-metadata.h: void ns3::PacketMetadata::RemoveHeader(ns3::Header const & header, uint32_t size) [member function]
    cls.add_method('RemoveHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header'), param('uint32_t', 'size')])
    ## packet-metadata.h: void ns3::PacketMetadata::RemoveTrailer(ns3::Trailer const & trailer, uint32_t size) [member function]
    cls.add_method('RemoveTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer'), param('uint32_t', 'size')])
    ## packet-metadata.h: uint32_t ns3::PacketMetadata::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3PacketMetadataItem_methods(root_module, cls):
    ## packet-metadata.h: ns3::PacketMetadata::Item::Item() [constructor]
    cls.add_constructor([])
    ## packet-metadata.h: ns3::PacketMetadata::Item::Item(ns3::PacketMetadata::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata::Item const &', 'arg0')])
    ## packet-metadata.h: ns3::PacketMetadata::Item::current [variable]
    cls.add_instance_attribute('current', 'ns3::Buffer::Iterator', is_const=False)
    ## packet-metadata.h: ns3::PacketMetadata::Item::currentSize [variable]
    cls.add_instance_attribute('currentSize', 'uint32_t', is_const=False)
    ## packet-metadata.h: ns3::PacketMetadata::Item::currentTrimedFromEnd [variable]
    cls.add_instance_attribute('currentTrimedFromEnd', 'uint32_t', is_const=False)
    ## packet-metadata.h: ns3::PacketMetadata::Item::currentTrimedFromStart [variable]
    cls.add_instance_attribute('currentTrimedFromStart', 'uint32_t', is_const=False)
    ## packet-metadata.h: ns3::PacketMetadata::Item::isFragment [variable]
    cls.add_instance_attribute('isFragment', 'bool', is_const=False)
    ## packet-metadata.h: ns3::PacketMetadata::Item::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3PacketMetadataItemIterator_methods(root_module, cls):
    ## packet-metadata.h: ns3::PacketMetadata::ItemIterator::ItemIterator(ns3::PacketMetadata::ItemIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata::ItemIterator const &', 'arg0')])
    ## packet-metadata.h: ns3::PacketMetadata::ItemIterator::ItemIterator(ns3::PacketMetadata const * metadata, ns3::Buffer buffer) [constructor]
    cls.add_constructor([param('ns3::PacketMetadata const *', 'metadata'), param('ns3::Buffer', 'buffer')])
    ## packet-metadata.h: bool ns3::PacketMetadata::ItemIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet-metadata.h: ns3::PacketMetadata::Item ns3::PacketMetadata::ItemIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::PacketMetadata::Item', 
                   [])
    return

def register_Ns3PacketTagIterator_methods(root_module, cls):
    ## packet.h: ns3::PacketTagIterator::PacketTagIterator(ns3::PacketTagIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagIterator const &', 'arg0')])
    ## packet.h: bool ns3::PacketTagIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet.h: ns3::PacketTagIterator::Item ns3::PacketTagIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::PacketTagIterator::Item', 
                   [])
    return

def register_Ns3PacketTagIteratorItem_methods(root_module, cls):
    ## packet.h: ns3::PacketTagIterator::Item::Item(ns3::PacketTagIterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagIterator::Item const &', 'arg0')])
    ## packet.h: void ns3::PacketTagIterator::Item::GetTag(ns3::Tag & tag) const [member function]
    cls.add_method('GetTag', 
                   'void', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h: ns3::TypeId ns3::PacketTagIterator::Item::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    return

def register_Ns3PacketTagList_methods(root_module, cls):
    ## packet-tag-list.h: ns3::PacketTagList::PacketTagList() [constructor]
    cls.add_constructor([])
    ## packet-tag-list.h: ns3::PacketTagList::PacketTagList(ns3::PacketTagList const & o) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagList const &', 'o')])
    ## packet-tag-list.h: void ns3::PacketTagList::Add(ns3::Tag const & tag) const [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet-tag-list.h: ns3::PacketTagList::TagData const * ns3::PacketTagList::Head() const [member function]
    cls.add_method('Head', 
                   'ns3::PacketTagList::TagData const *', 
                   [], 
                   is_const=True)
    ## packet-tag-list.h: bool ns3::PacketTagList::Peek(ns3::Tag & tag) const [member function]
    cls.add_method('Peek', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet-tag-list.h: bool ns3::PacketTagList::Remove(ns3::Tag & tag) [member function]
    cls.add_method('Remove', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    ## packet-tag-list.h: void ns3::PacketTagList::RemoveAll() [member function]
    cls.add_method('RemoveAll', 
                   'void', 
                   [])
    return

def register_Ns3PacketTagListTagData_methods(root_module, cls):
    ## packet-tag-list.h: ns3::PacketTagList::TagData::TagData() [constructor]
    cls.add_constructor([])
    ## packet-tag-list.h: ns3::PacketTagList::TagData::TagData(ns3::PacketTagList::TagData const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagList::TagData const &', 'arg0')])
    ## packet-tag-list.h: ns3::PacketTagList::TagData::count [variable]
    cls.add_instance_attribute('count', 'uint32_t', is_const=False)
    ## packet-tag-list.h: ns3::PacketTagList::TagData::data [variable]
    cls.add_instance_attribute('data', 'uint8_t [ 20 ]', is_const=False)
    ## packet-tag-list.h: ns3::PacketTagList::TagData::next [variable]
    cls.add_instance_attribute('next', 'ns3::PacketTagList::TagData *', is_const=False)
    ## packet-tag-list.h: ns3::PacketTagList::TagData::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3PcapFile_methods(root_module, cls):
    ## pcap-file.h: ns3::PcapFile::PcapFile() [constructor]
    cls.add_constructor([])
    ## pcap-file.h: void ns3::PcapFile::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## pcap-file.h: void ns3::PcapFile::Close() [member function]
    cls.add_method('Close', 
                   'void', 
                   [])
    ## pcap-file.h: static bool ns3::PcapFile::Diff(std::string const & f1, std::string const & f2, uint32_t & sec, uint32_t & usec, uint32_t snapLen=ns3::PcapFile::SNAPLEN_DEFAULT) [member function]
    cls.add_method('Diff', 
                   'bool', 
                   [param('std::string const &', 'f1'), param('std::string const &', 'f2'), param('uint32_t &', 'sec'), param('uint32_t &', 'usec'), param('uint32_t', 'snapLen', default_value='ns3::PcapFile::SNAPLEN_DEFAULT')], 
                   is_static=True)
    ## pcap-file.h: bool ns3::PcapFile::Eof() const [member function]
    cls.add_method('Eof', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file.h: bool ns3::PcapFile::Fail() const [member function]
    cls.add_method('Fail', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file.h: uint32_t ns3::PcapFile::GetDataLinkType() [member function]
    cls.add_method('GetDataLinkType', 
                   'uint32_t', 
                   [])
    ## pcap-file.h: uint32_t ns3::PcapFile::GetMagic() [member function]
    cls.add_method('GetMagic', 
                   'uint32_t', 
                   [])
    ## pcap-file.h: uint32_t ns3::PcapFile::GetSigFigs() [member function]
    cls.add_method('GetSigFigs', 
                   'uint32_t', 
                   [])
    ## pcap-file.h: uint32_t ns3::PcapFile::GetSnapLen() [member function]
    cls.add_method('GetSnapLen', 
                   'uint32_t', 
                   [])
    ## pcap-file.h: bool ns3::PcapFile::GetSwapMode() [member function]
    cls.add_method('GetSwapMode', 
                   'bool', 
                   [])
    ## pcap-file.h: int32_t ns3::PcapFile::GetTimeZoneOffset() [member function]
    cls.add_method('GetTimeZoneOffset', 
                   'int32_t', 
                   [])
    ## pcap-file.h: uint16_t ns3::PcapFile::GetVersionMajor() [member function]
    cls.add_method('GetVersionMajor', 
                   'uint16_t', 
                   [])
    ## pcap-file.h: uint16_t ns3::PcapFile::GetVersionMinor() [member function]
    cls.add_method('GetVersionMinor', 
                   'uint16_t', 
                   [])
    ## pcap-file.h: void ns3::PcapFile::Init(uint32_t dataLinkType, uint32_t snapLen=ns3::PcapFile::SNAPLEN_DEFAULT, int32_t timeZoneCorrection=ns3::PcapFile::ZONE_DEFAULT, bool swapMode=false) [member function]
    cls.add_method('Init', 
                   'void', 
                   [param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='ns3::PcapFile::SNAPLEN_DEFAULT'), param('int32_t', 'timeZoneCorrection', default_value='ns3::PcapFile::ZONE_DEFAULT'), param('bool', 'swapMode', default_value='false')])
    ## pcap-file.h: void ns3::PcapFile::Open(std::string const & filename, std::_Ios_Openmode mode) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string const &', 'filename'), param('std::_Ios_Openmode', 'mode')])
    ## pcap-file.h: void ns3::PcapFile::Read(uint8_t * const data, uint32_t maxBytes, uint32_t & tsSec, uint32_t & tsUsec, uint32_t & inclLen, uint32_t & origLen, uint32_t & readLen) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t * const', 'data'), param('uint32_t', 'maxBytes'), param('uint32_t &', 'tsSec'), param('uint32_t &', 'tsUsec'), param('uint32_t &', 'inclLen'), param('uint32_t &', 'origLen'), param('uint32_t &', 'readLen')])
    ## pcap-file.h: void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, uint8_t const * const data, uint32_t totalLen) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('uint8_t const * const', 'data'), param('uint32_t', 'totalLen')])
    ## pcap-file.h: void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file.h: void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, ns3::Header & header, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('ns3::Header &', 'header'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file.h: ns3::PcapFile::SNAPLEN_DEFAULT [variable]
    cls.add_static_attribute('SNAPLEN_DEFAULT', 'uint32_t const', is_const=True)
    ## pcap-file.h: ns3::PcapFile::ZONE_DEFAULT [variable]
    cls.add_static_attribute('ZONE_DEFAULT', 'int32_t const', is_const=True)
    return

def register_Ns3SequenceNumber32_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_numeric_operator('+', root_module['ns3::SequenceNumber32'], root_module['ns3::SequenceNumber32'], param('ns3::SequenceNumber< unsigned int, int > const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::SequenceNumber32'], root_module['ns3::SequenceNumber32'], param('int', 'right'))
    cls.add_inplace_numeric_operator('+=', param('int', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::SequenceNumber32'], root_module['ns3::SequenceNumber32'], param('int', 'right'))
    cls.add_inplace_numeric_operator('-=', param('int', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## sequence-number.h: ns3::SequenceNumber<unsigned int, int>::SequenceNumber() [constructor]
    cls.add_constructor([])
    ## sequence-number.h: ns3::SequenceNumber<unsigned int, int>::SequenceNumber(unsigned int value) [constructor]
    cls.add_constructor([param('unsigned int', 'value')])
    ## sequence-number.h: ns3::SequenceNumber<unsigned int, int>::SequenceNumber(ns3::SequenceNumber<unsigned int, int> const & value) [copy constructor]
    cls.add_constructor([param('ns3::SequenceNumber< unsigned int, int > const &', 'value')])
    ## sequence-number.h: unsigned int ns3::SequenceNumber<unsigned int, int>::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'unsigned int', 
                   [], 
                   is_const=True)
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

def register_Ns3Tag_methods(root_module, cls):
    ## tag.h: ns3::Tag::Tag() [constructor]
    cls.add_constructor([])
    ## tag.h: ns3::Tag::Tag(ns3::Tag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Tag const &', 'arg0')])
    ## tag.h: void ns3::Tag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_pure_virtual=True, is_virtual=True)
    ## tag.h: uint32_t ns3::Tag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## tag.h: static ns3::TypeId ns3::Tag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tag.h: void ns3::Tag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## tag.h: void ns3::Tag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3TagBuffer_methods(root_module, cls):
    ## tag-buffer.h: ns3::TagBuffer::TagBuffer(ns3::TagBuffer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TagBuffer const &', 'arg0')])
    ## tag-buffer.h: ns3::TagBuffer::TagBuffer(uint8_t * start, uint8_t * end) [constructor]
    cls.add_constructor([param('uint8_t *', 'start'), param('uint8_t *', 'end')])
    ## tag-buffer.h: void ns3::TagBuffer::CopyFrom(ns3::TagBuffer o) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('ns3::TagBuffer', 'o')])
    ## tag-buffer.h: void ns3::TagBuffer::Read(uint8_t * buffer, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')])
    ## tag-buffer.h: double ns3::TagBuffer::ReadDouble() [member function]
    cls.add_method('ReadDouble', 
                   'double', 
                   [])
    ## tag-buffer.h: uint16_t ns3::TagBuffer::ReadU16() [member function]
    cls.add_method('ReadU16', 
                   'uint16_t', 
                   [])
    ## tag-buffer.h: uint32_t ns3::TagBuffer::ReadU32() [member function]
    cls.add_method('ReadU32', 
                   'uint32_t', 
                   [])
    ## tag-buffer.h: uint64_t ns3::TagBuffer::ReadU64() [member function]
    cls.add_method('ReadU64', 
                   'uint64_t', 
                   [])
    ## tag-buffer.h: uint8_t ns3::TagBuffer::ReadU8() [member function]
    cls.add_method('ReadU8', 
                   'uint8_t', 
                   [])
    ## tag-buffer.h: void ns3::TagBuffer::TrimAtEnd(uint32_t trim) [member function]
    cls.add_method('TrimAtEnd', 
                   'void', 
                   [param('uint32_t', 'trim')])
    ## tag-buffer.h: void ns3::TagBuffer::Write(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## tag-buffer.h: void ns3::TagBuffer::WriteDouble(double v) [member function]
    cls.add_method('WriteDouble', 
                   'void', 
                   [param('double', 'v')])
    ## tag-buffer.h: void ns3::TagBuffer::WriteU16(uint16_t data) [member function]
    cls.add_method('WriteU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## tag-buffer.h: void ns3::TagBuffer::WriteU32(uint32_t data) [member function]
    cls.add_method('WriteU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## tag-buffer.h: void ns3::TagBuffer::WriteU64(uint64_t v) [member function]
    cls.add_method('WriteU64', 
                   'void', 
                   [param('uint64_t', 'v')])
    ## tag-buffer.h: void ns3::TagBuffer::WriteU8(uint8_t v) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'v')])
    return

def register_Ns3Chunk_methods(root_module, cls):
    ## chunk.h: ns3::Chunk::Chunk() [constructor]
    cls.add_constructor([])
    ## chunk.h: ns3::Chunk::Chunk(ns3::Chunk const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Chunk const &', 'arg0')])
    ## chunk.h: uint32_t ns3::Chunk::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_virtual=True)
    ## chunk.h: static ns3::TypeId ns3::Chunk::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## chunk.h: void ns3::Chunk::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3Header_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## header.h: ns3::Header::Header() [constructor]
    cls.add_constructor([])
    ## header.h: ns3::Header::Header(ns3::Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Header const &', 'arg0')])
    ## header.h: uint32_t ns3::Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_virtual=True)
    ## header.h: uint32_t ns3::Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## header.h: static ns3::TypeId ns3::Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## header.h: void ns3::Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## header.h: void ns3::Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3PacketBurst_methods(root_module, cls):
    ## packet-burst.h: ns3::PacketBurst::PacketBurst(ns3::PacketBurst const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketBurst const &', 'arg0')])
    ## packet-burst.h: ns3::PacketBurst::PacketBurst() [constructor]
    cls.add_constructor([])
    ## packet-burst.h: void ns3::PacketBurst::AddPacket(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('AddPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')])
    ## packet-burst.h: std::_List_const_iterator<ns3::Ptr<ns3::Packet> > ns3::PacketBurst::Begin() const [member function]
    cls.add_method('Begin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::Packet > >', 
                   [], 
                   is_const=True)
    ## packet-burst.h: ns3::Ptr<ns3::PacketBurst> ns3::PacketBurst::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::PacketBurst >', 
                   [], 
                   is_const=True)
    ## packet-burst.h: std::_List_const_iterator<ns3::Ptr<ns3::Packet> > ns3::PacketBurst::End() const [member function]
    cls.add_method('End', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::Packet > >', 
                   [], 
                   is_const=True)
    ## packet-burst.h: uint32_t ns3::PacketBurst::GetNPackets() const [member function]
    cls.add_method('GetNPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-burst.h: std::list<ns3::Ptr<ns3::Packet>, std::allocator<ns3::Ptr<ns3::Packet> > > ns3::PacketBurst::GetPackets() const [member function]
    cls.add_method('GetPackets', 
                   'std::list< ns3::Ptr< ns3::Packet > >', 
                   [], 
                   is_const=True)
    ## packet-burst.h: uint32_t ns3::PacketBurst::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-burst.h: static ns3::TypeId ns3::PacketBurst::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## packet-burst.h: void ns3::PacketBurst::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PcapFileWrapper_methods(root_module, cls):
    ## pcap-file-wrapper.h: static ns3::TypeId ns3::PcapFileWrapper::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## pcap-file-wrapper.h: ns3::PcapFileWrapper::PcapFileWrapper() [constructor]
    cls.add_constructor([])
    ## pcap-file-wrapper.h: bool ns3::PcapFileWrapper::Fail() const [member function]
    cls.add_method('Fail', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file-wrapper.h: bool ns3::PcapFileWrapper::Eof() const [member function]
    cls.add_method('Eof', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file-wrapper.h: void ns3::PcapFileWrapper::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## pcap-file-wrapper.h: void ns3::PcapFileWrapper::Open(std::string const & filename, std::_Ios_Openmode mode) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string const &', 'filename'), param('std::_Ios_Openmode', 'mode')])
    ## pcap-file-wrapper.h: void ns3::PcapFileWrapper::Close() [member function]
    cls.add_method('Close', 
                   'void', 
                   [])
    ## pcap-file-wrapper.h: void ns3::PcapFileWrapper::Init(uint32_t dataLinkType, uint32_t snapLen=std::numeric_limits<unsigned int>::max(), int32_t tzCorrection=ns3::PcapFile::ZONE_DEFAULT) [member function]
    cls.add_method('Init', 
                   'void', 
                   [param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='std::numeric_limits<unsigned int>::max()'), param('int32_t', 'tzCorrection', default_value='ns3::PcapFile::ZONE_DEFAULT')])
    ## pcap-file-wrapper.h: void ns3::PcapFileWrapper::Write(ns3::Time t, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file-wrapper.h: void ns3::PcapFileWrapper::Write(ns3::Time t, ns3::Header & header, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('ns3::Header &', 'header'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file-wrapper.h: void ns3::PcapFileWrapper::Write(ns3::Time t, uint8_t const * buffer, uint32_t length) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('uint8_t const *', 'buffer'), param('uint32_t', 'length')])
    ## pcap-file-wrapper.h: uint32_t ns3::PcapFileWrapper::GetMagic() [member function]
    cls.add_method('GetMagic', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h: uint16_t ns3::PcapFileWrapper::GetVersionMajor() [member function]
    cls.add_method('GetVersionMajor', 
                   'uint16_t', 
                   [])
    ## pcap-file-wrapper.h: uint16_t ns3::PcapFileWrapper::GetVersionMinor() [member function]
    cls.add_method('GetVersionMinor', 
                   'uint16_t', 
                   [])
    ## pcap-file-wrapper.h: int32_t ns3::PcapFileWrapper::GetTimeZoneOffset() [member function]
    cls.add_method('GetTimeZoneOffset', 
                   'int32_t', 
                   [])
    ## pcap-file-wrapper.h: uint32_t ns3::PcapFileWrapper::GetSigFigs() [member function]
    cls.add_method('GetSigFigs', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h: uint32_t ns3::PcapFileWrapper::GetSnapLen() [member function]
    cls.add_method('GetSnapLen', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h: uint32_t ns3::PcapFileWrapper::GetDataLinkType() [member function]
    cls.add_method('GetDataLinkType', 
                   'uint32_t', 
                   [])
    return

def register_Ns3PropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::PropagationDelayModel::PropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: ns3::PropagationDelayModel::PropagationDelayModel(ns3::PropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: ns3::Time ns3::PropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## propagation-delay-model.h: static ns3::TypeId ns3::PropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
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
    ## propagation-loss-model.h: double ns3::PropagationLossModel::CalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('CalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::PropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RandomPropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel::RandomPropagationDelayModel(ns3::RandomPropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomPropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: ns3::RandomPropagationDelayModel::RandomPropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: ns3::Time ns3::RandomPropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## propagation-delay-model.h: static ns3::TypeId ns3::RandomPropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3RandomPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::RandomPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::RandomPropagationLossModel::RandomPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::RandomPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RangePropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::RangePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::RangePropagationLossModel::RangePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::RangePropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
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

def register_Ns3ThreeLogDistancePropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::ThreeLogDistancePropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::ThreeLogDistancePropagationLossModel::ThreeLogDistancePropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::ThreeLogDistancePropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3Trailer_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## trailer.h: ns3::Trailer::Trailer() [constructor]
    cls.add_constructor([])
    ## trailer.h: ns3::Trailer::Trailer(ns3::Trailer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Trailer const &', 'arg0')])
    ## trailer.h: uint32_t ns3::Trailer::Deserialize(ns3::Buffer::Iterator end) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'end')], 
                   is_pure_virtual=True, is_virtual=True)
    ## trailer.h: uint32_t ns3::Trailer::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trailer.h: static ns3::TypeId ns3::Trailer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## trailer.h: void ns3::Trailer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trailer.h: void ns3::Trailer::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3TwoRayGroundPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::TwoRayGroundPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::TwoRayGroundPropagationLossModel::TwoRayGroundPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetLambda(double frequency, double speed) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'frequency'), param('double', 'speed')])
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetLambda(double lambda) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'lambda')])
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetSystemLoss(double systemLoss) [member function]
    cls.add_method('SetSystemLoss', 
                   'void', 
                   [param('double', 'systemLoss')])
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetMinDistance(double minDistance) [member function]
    cls.add_method('SetMinDistance', 
                   'void', 
                   [param('double', 'minDistance')])
    ## propagation-loss-model.h: double ns3::TwoRayGroundPropagationLossModel::GetMinDistance() const [member function]
    cls.add_method('GetMinDistance', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::TwoRayGroundPropagationLossModel::GetLambda() const [member function]
    cls.add_method('GetLambda', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: double ns3::TwoRayGroundPropagationLossModel::GetSystemLoss() const [member function]
    cls.add_method('GetSystemLoss', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-loss-model.h: void ns3::TwoRayGroundPropagationLossModel::SetHeightAboveZ(double heightAboveZ) [member function]
    cls.add_method('SetHeightAboveZ', 
                   'void', 
                   [param('double', 'heightAboveZ')])
    ## propagation-loss-model.h: double ns3::TwoRayGroundPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ConstantSpeedPropagationDelayModel_methods(root_module, cls):
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel(ns3::ConstantSpeedPropagationDelayModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantSpeedPropagationDelayModel const &', 'arg0')])
    ## propagation-delay-model.h: ns3::ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel() [constructor]
    cls.add_constructor([])
    ## propagation-delay-model.h: ns3::Time ns3::ConstantSpeedPropagationDelayModel::GetDelay(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetDelay', 
                   'ns3::Time', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, is_virtual=True)
    ## propagation-delay-model.h: double ns3::ConstantSpeedPropagationDelayModel::GetSpeed() const [member function]
    cls.add_method('GetSpeed', 
                   'double', 
                   [], 
                   is_const=True)
    ## propagation-delay-model.h: static ns3::TypeId ns3::ConstantSpeedPropagationDelayModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-delay-model.h: void ns3::ConstantSpeedPropagationDelayModel::SetSpeed(double speed) [member function]
    cls.add_method('SetSpeed', 
                   'void', 
                   [param('double', 'speed')])
    return

def register_Ns3Cost231PropagationLossModel_methods(root_module, cls):
    ## cost231-propagation-loss-model.h: static ns3::TypeId ns3::Cost231PropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## cost231-propagation-loss-model.h: ns3::Cost231PropagationLossModel::Cost231PropagationLossModel() [constructor]
    cls.add_constructor([])
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetLoss(ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('GetLoss', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetBSAntennaHeight(double height) [member function]
    cls.add_method('SetBSAntennaHeight', 
                   'void', 
                   [param('double', 'height')])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetSSAntennaHeight(double height) [member function]
    cls.add_method('SetSSAntennaHeight', 
                   'void', 
                   [param('double', 'height')])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetEnvironment(ns3::Cost231PropagationLossModel::Environment env) [member function]
    cls.add_method('SetEnvironment', 
                   'void', 
                   [param('ns3::Cost231PropagationLossModel::Environment', 'env')])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetLambda(double lambda) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'lambda')])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetMinDistance(double minDistance) [member function]
    cls.add_method('SetMinDistance', 
                   'void', 
                   [param('double', 'minDistance')])
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetBSAntennaHeight() const [member function]
    cls.add_method('GetBSAntennaHeight', 
                   'double', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetSSAntennaHeight() const [member function]
    cls.add_method('GetSSAntennaHeight', 
                   'double', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: ns3::Cost231PropagationLossModel::Environment ns3::Cost231PropagationLossModel::GetEnvironment() const [member function]
    cls.add_method('GetEnvironment', 
                   'ns3::Cost231PropagationLossModel::Environment', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetMinDistance() const [member function]
    cls.add_method('GetMinDistance', 
                   'double', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetLambda() const [member function]
    cls.add_method('GetLambda', 
                   'double', 
                   [], 
                   is_const=True)
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetLambda(double frequency, double speed) [member function]
    cls.add_method('SetLambda', 
                   'void', 
                   [param('double', 'frequency'), param('double', 'speed')])
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::GetShadowing() [member function]
    cls.add_method('GetShadowing', 
                   'double', 
                   [])
    ## cost231-propagation-loss-model.h: void ns3::Cost231PropagationLossModel::SetShadowing(double shadowing) [member function]
    cls.add_method('SetShadowing', 
                   'void', 
                   [param('double', 'shadowing')])
    ## cost231-propagation-loss-model.h: double ns3::Cost231PropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3DataRateChecker_methods(root_module, cls):
    ## data-rate.h: ns3::DataRateChecker::DataRateChecker() [constructor]
    cls.add_constructor([])
    ## data-rate.h: ns3::DataRateChecker::DataRateChecker(ns3::DataRateChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRateChecker const &', 'arg0')])
    return

def register_Ns3DataRateValue_methods(root_module, cls):
    ## data-rate.h: ns3::DataRateValue::DataRateValue() [constructor]
    cls.add_constructor([])
    ## data-rate.h: ns3::DataRateValue::DataRateValue(ns3::DataRateValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRateValue const &', 'arg0')])
    ## data-rate.h: ns3::DataRateValue::DataRateValue(ns3::DataRate const & value) [constructor]
    cls.add_constructor([param('ns3::DataRate const &', 'value')])
    ## data-rate.h: ns3::Ptr<ns3::AttributeValue> ns3::DataRateValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## data-rate.h: bool ns3::DataRateValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## data-rate.h: ns3::DataRate ns3::DataRateValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::DataRate', 
                   [], 
                   is_const=True)
    ## data-rate.h: std::string ns3::DataRateValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## data-rate.h: void ns3::DataRateValue::Set(ns3::DataRate const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::DataRate const &', 'value')])
    return

def register_Ns3ErrorModel_methods(root_module, cls):
    ## error-model.h: ns3::ErrorModel::ErrorModel(ns3::ErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErrorModel const &', 'arg0')])
    ## error-model.h: ns3::ErrorModel::ErrorModel() [constructor]
    cls.add_constructor([])
    ## error-model.h: void ns3::ErrorModel::Disable() [member function]
    cls.add_method('Disable', 
                   'void', 
                   [])
    ## error-model.h: void ns3::ErrorModel::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [])
    ## error-model.h: static ns3::TypeId ns3::ErrorModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## error-model.h: bool ns3::ErrorModel::IsCorrupt(ns3::Ptr<ns3::Packet> pkt) [member function]
    cls.add_method('IsCorrupt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt')])
    ## error-model.h: bool ns3::ErrorModel::IsEnabled() const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## error-model.h: void ns3::ErrorModel::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## error-model.h: bool ns3::ErrorModel::DoCorrupt(ns3::Ptr<ns3::Packet> arg0) [member function]
    cls.add_method('DoCorrupt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'arg0')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## error-model.h: void ns3::ErrorModel::DoReset() [member function]
    cls.add_method('DoReset', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    return

def register_Ns3FixedRssLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::FixedRssLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::FixedRssLossModel::FixedRssLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::FixedRssLossModel::SetRss(double rss) [member function]
    cls.add_method('SetRss', 
                   'void', 
                   [param('double', 'rss')])
    ## propagation-loss-model.h: double ns3::FixedRssLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
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
    ## propagation-loss-model.h: double ns3::FriisPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
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

def register_Ns3JakesPropagationLossModel_methods(root_module, cls):
    ## jakes-propagation-loss-model.h: ns3::JakesPropagationLossModel::JakesPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## jakes-propagation-loss-model.h: uint8_t ns3::JakesPropagationLossModel::GetNOscillators() const [member function]
    cls.add_method('GetNOscillators', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## jakes-propagation-loss-model.h: uint8_t ns3::JakesPropagationLossModel::GetNRays() const [member function]
    cls.add_method('GetNRays', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## jakes-propagation-loss-model.h: static ns3::TypeId ns3::JakesPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## jakes-propagation-loss-model.h: void ns3::JakesPropagationLossModel::SetNOscillators(uint8_t nOscillators) [member function]
    cls.add_method('SetNOscillators', 
                   'void', 
                   [param('uint8_t', 'nOscillators')])
    ## jakes-propagation-loss-model.h: void ns3::JakesPropagationLossModel::SetNRays(uint8_t nRays) [member function]
    cls.add_method('SetNRays', 
                   'void', 
                   [param('uint8_t', 'nRays')])
    ## jakes-propagation-loss-model.h: double ns3::JakesPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ListErrorModel_methods(root_module, cls):
    ## error-model.h: ns3::ListErrorModel::ListErrorModel(ns3::ListErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ListErrorModel const &', 'arg0')])
    ## error-model.h: ns3::ListErrorModel::ListErrorModel() [constructor]
    cls.add_constructor([])
    ## error-model.h: std::list<unsigned int, std::allocator<unsigned int> > ns3::ListErrorModel::GetList() const [member function]
    cls.add_method('GetList', 
                   'std::list< unsigned int >', 
                   [], 
                   is_const=True)
    ## error-model.h: static ns3::TypeId ns3::ListErrorModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## error-model.h: void ns3::ListErrorModel::SetList(std::list<unsigned int, std::allocator<unsigned int> > const & packetlist) [member function]
    cls.add_method('SetList', 
                   'void', 
                   [param('std::list< unsigned int > const &', 'packetlist')])
    ## error-model.h: bool ns3::ListErrorModel::DoCorrupt(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorrupt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: void ns3::ListErrorModel::DoReset() [member function]
    cls.add_method('DoReset', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
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
    ## propagation-loss-model.h: double ns3::LogDistancePropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3MatrixPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::MatrixPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::MatrixPropagationLossModel::MatrixPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: void ns3::MatrixPropagationLossModel::SetLoss(ns3::Ptr<ns3::Node> a, ns3::Ptr<ns3::Node> b, double loss, bool symmetric=true) [member function]
    cls.add_method('SetLoss', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'a'), param('ns3::Ptr< ns3::Node >', 'b'), param('double', 'loss'), param('bool', 'symmetric', default_value='true')])
    ## propagation-loss-model.h: void ns3::MatrixPropagationLossModel::SetDefaultLoss(double arg0) [member function]
    cls.add_method('SetDefaultLoss', 
                   'void', 
                   [param('double', 'arg0')])
    ## propagation-loss-model.h: double ns3::MatrixPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3NakagamiPropagationLossModel_methods(root_module, cls):
    ## propagation-loss-model.h: static ns3::TypeId ns3::NakagamiPropagationLossModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## propagation-loss-model.h: ns3::NakagamiPropagationLossModel::NakagamiPropagationLossModel() [constructor]
    cls.add_constructor([])
    ## propagation-loss-model.h: double ns3::NakagamiPropagationLossModel::DoCalcRxPower(double txPowerDbm, ns3::Ptr<ns3::MobilityModel> a, ns3::Ptr<ns3::MobilityModel> b) const [member function]
    cls.add_method('DoCalcRxPower', 
                   'double', 
                   [param('double', 'txPowerDbm'), param('ns3::Ptr< ns3::MobilityModel >', 'a'), param('ns3::Ptr< ns3::MobilityModel >', 'b')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3NixVector_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## nix-vector.h: ns3::NixVector::NixVector() [constructor]
    cls.add_constructor([])
    ## nix-vector.h: ns3::NixVector::NixVector(ns3::NixVector const & o) [copy constructor]
    cls.add_constructor([param('ns3::NixVector const &', 'o')])
    ## nix-vector.h: void ns3::NixVector::AddNeighborIndex(uint32_t newBits, uint32_t numberOfBits) [member function]
    cls.add_method('AddNeighborIndex', 
                   'void', 
                   [param('uint32_t', 'newBits'), param('uint32_t', 'numberOfBits')])
    ## nix-vector.h: uint32_t ns3::NixVector::BitCount(uint32_t numberOfNeighbors) const [member function]
    cls.add_method('BitCount', 
                   'uint32_t', 
                   [param('uint32_t', 'numberOfNeighbors')], 
                   is_const=True)
    ## nix-vector.h: ns3::Ptr<ns3::NixVector> ns3::NixVector::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::NixVector >', 
                   [], 
                   is_const=True)
    ## nix-vector.h: uint32_t ns3::NixVector::Deserialize(uint32_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint32_t const *', 'buffer'), param('uint32_t', 'size')])
    ## nix-vector.h: uint32_t ns3::NixVector::ExtractNeighborIndex(uint32_t numberOfBits) [member function]
    cls.add_method('ExtractNeighborIndex', 
                   'uint32_t', 
                   [param('uint32_t', 'numberOfBits')])
    ## nix-vector.h: uint32_t ns3::NixVector::GetRemainingBits() [member function]
    cls.add_method('GetRemainingBits', 
                   'uint32_t', 
                   [])
    ## nix-vector.h: uint32_t ns3::NixVector::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## nix-vector.h: uint32_t ns3::NixVector::Serialize(uint32_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint32_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3OutputStreamWrapper_methods(root_module, cls):
    ## output-stream-wrapper.h: ns3::OutputStreamWrapper::OutputStreamWrapper(ns3::OutputStreamWrapper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OutputStreamWrapper const &', 'arg0')])
    ## output-stream-wrapper.h: ns3::OutputStreamWrapper::OutputStreamWrapper(std::string filename, std::_Ios_Openmode filemode) [constructor]
    cls.add_constructor([param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode')])
    ## output-stream-wrapper.h: std::ostream * ns3::OutputStreamWrapper::GetStream() [member function]
    cls.add_method('GetStream', 
                   'std::ostream *', 
                   [])
    return

def register_Ns3Packet_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## packet.h: ns3::Packet::Packet() [constructor]
    cls.add_constructor([])
    ## packet.h: ns3::Packet::Packet(ns3::Packet const & o) [copy constructor]
    cls.add_constructor([param('ns3::Packet const &', 'o')])
    ## packet.h: ns3::Packet::Packet(uint32_t size) [constructor]
    cls.add_constructor([param('uint32_t', 'size')])
    ## packet.h: ns3::Packet::Packet(uint8_t const * buffer, uint32_t size, bool magic) [constructor]
    cls.add_constructor([param('uint8_t const *', 'buffer'), param('uint32_t', 'size'), param('bool', 'magic')])
    ## packet.h: ns3::Packet::Packet(uint8_t const * buffer, uint32_t size) [constructor]
    cls.add_constructor([param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## packet.h: void ns3::Packet::AddAtEnd(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## packet.h: void ns3::Packet::AddByteTag(ns3::Tag const & tag) const [member function]
    cls.add_method('AddByteTag', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::AddHeader(ns3::Header const & header) [member function]
    cls.add_method('AddHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header')])
    ## packet.h: void ns3::Packet::AddPacketTag(ns3::Tag const & tag) const [member function]
    cls.add_method('AddPacketTag', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::AddPaddingAtEnd(uint32_t size) [member function]
    cls.add_method('AddPaddingAtEnd', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h: void ns3::Packet::AddTrailer(ns3::Trailer const & trailer) [member function]
    cls.add_method('AddTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer')])
    ## packet.h: ns3::PacketMetadata::ItemIterator ns3::Packet::BeginItem() const [member function]
    cls.add_method('BeginItem', 
                   'ns3::PacketMetadata::ItemIterator', 
                   [], 
                   is_const=True)
    ## packet.h: ns3::Ptr<ns3::Packet> ns3::Packet::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## packet.h: uint32_t ns3::Packet::CopyData(uint8_t * buffer, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::CopyData(std::ostream * os, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'void', 
                   [param('std::ostream *', 'os'), param('uint32_t', 'size')], 
                   is_const=True)
    ## packet.h: ns3::Ptr<ns3::Packet> ns3::Packet::CreateFragment(uint32_t start, uint32_t length) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'start'), param('uint32_t', 'length')], 
                   is_const=True)
    ## packet.h: static void ns3::Packet::EnableChecking() [member function]
    cls.add_method('EnableChecking', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet.h: static void ns3::Packet::EnablePrinting() [member function]
    cls.add_method('EnablePrinting', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet.h: bool ns3::Packet::FindFirstMatchingByteTag(ns3::Tag & tag) const [member function]
    cls.add_method('FindFirstMatchingByteTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h: ns3::ByteTagIterator ns3::Packet::GetByteTagIterator() const [member function]
    cls.add_method('GetByteTagIterator', 
                   'ns3::ByteTagIterator', 
                   [], 
                   is_const=True)
    ## packet.h: ns3::Ptr<ns3::NixVector> ns3::Packet::GetNixVector() const [member function]
    cls.add_method('GetNixVector', 
                   'ns3::Ptr< ns3::NixVector >', 
                   [], 
                   is_const=True)
    ## packet.h: ns3::PacketTagIterator ns3::Packet::GetPacketTagIterator() const [member function]
    cls.add_method('GetPacketTagIterator', 
                   'ns3::PacketTagIterator', 
                   [], 
                   is_const=True)
    ## packet.h: uint32_t ns3::Packet::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h: uint32_t ns3::Packet::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h: uint64_t ns3::Packet::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## packet.h: uint8_t const * ns3::Packet::PeekData() const [member function]
    cls.add_method('PeekData', 
                   'uint8_t const *', 
                   [], 
                   deprecated=True, is_const=True)
    ## packet.h: uint32_t ns3::Packet::PeekHeader(ns3::Header & header) const [member function]
    cls.add_method('PeekHeader', 
                   'uint32_t', 
                   [param('ns3::Header &', 'header')], 
                   is_const=True)
    ## packet.h: bool ns3::Packet::PeekPacketTag(ns3::Tag & tag) const [member function]
    cls.add_method('PeekPacketTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h: uint32_t ns3::Packet::PeekTrailer(ns3::Trailer & trailer) [member function]
    cls.add_method('PeekTrailer', 
                   'uint32_t', 
                   [param('ns3::Trailer &', 'trailer')])
    ## packet.h: void ns3::Packet::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::PrintByteTags(std::ostream & os) const [member function]
    cls.add_method('PrintByteTags', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::PrintPacketTags(std::ostream & os) const [member function]
    cls.add_method('PrintPacketTags', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::RemoveAllByteTags() [member function]
    cls.add_method('RemoveAllByteTags', 
                   'void', 
                   [])
    ## packet.h: void ns3::Packet::RemoveAllPacketTags() [member function]
    cls.add_method('RemoveAllPacketTags', 
                   'void', 
                   [])
    ## packet.h: void ns3::Packet::RemoveAtEnd(uint32_t size) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h: void ns3::Packet::RemoveAtStart(uint32_t size) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h: uint32_t ns3::Packet::RemoveHeader(ns3::Header & header) [member function]
    cls.add_method('RemoveHeader', 
                   'uint32_t', 
                   [param('ns3::Header &', 'header')])
    ## packet.h: bool ns3::Packet::RemovePacketTag(ns3::Tag & tag) [member function]
    cls.add_method('RemovePacketTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    ## packet.h: uint32_t ns3::Packet::RemoveTrailer(ns3::Trailer & trailer) [member function]
    cls.add_method('RemoveTrailer', 
                   'uint32_t', 
                   [param('ns3::Trailer &', 'trailer')])
    ## packet.h: uint32_t ns3::Packet::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::SetNixVector(ns3::Ptr<ns3::NixVector> arg0) [member function]
    cls.add_method('SetNixVector', 
                   'void', 
                   [param('ns3::Ptr< ns3::NixVector >', 'arg0')])
    return

def register_Ns3RateErrorModel_methods(root_module, cls):
    ## error-model.h: ns3::RateErrorModel::RateErrorModel(ns3::RateErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RateErrorModel const &', 'arg0')])
    ## error-model.h: ns3::RateErrorModel::RateErrorModel() [constructor]
    cls.add_constructor([])
    ## error-model.h: double ns3::RateErrorModel::GetRate() const [member function]
    cls.add_method('GetRate', 
                   'double', 
                   [], 
                   is_const=True)
    ## error-model.h: static ns3::TypeId ns3::RateErrorModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## error-model.h: ns3::ErrorUnit ns3::RateErrorModel::GetUnit() const [member function]
    cls.add_method('GetUnit', 
                   'ns3::ErrorUnit', 
                   [], 
                   is_const=True)
    ## error-model.h: void ns3::RateErrorModel::SetRandomVariable(ns3::RandomVariable const & ranvar) [member function]
    cls.add_method('SetRandomVariable', 
                   'void', 
                   [param('ns3::RandomVariable const &', 'ranvar')])
    ## error-model.h: void ns3::RateErrorModel::SetRate(double rate) [member function]
    cls.add_method('SetRate', 
                   'void', 
                   [param('double', 'rate')])
    ## error-model.h: void ns3::RateErrorModel::SetUnit(ns3::ErrorUnit error_unit) [member function]
    cls.add_method('SetUnit', 
                   'void', 
                   [param('ns3::ErrorUnit', 'error_unit')])
    ## error-model.h: bool ns3::RateErrorModel::DoCorrupt(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorrupt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: bool ns3::RateErrorModel::DoCorruptBit(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorruptBit', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: bool ns3::RateErrorModel::DoCorruptByte(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorruptByte', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: bool ns3::RateErrorModel::DoCorruptPkt(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorruptPkt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: void ns3::RateErrorModel::DoReset() [member function]
    cls.add_method('DoReset', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3ReceiveListErrorModel_methods(root_module, cls):
    ## error-model.h: ns3::ReceiveListErrorModel::ReceiveListErrorModel(ns3::ReceiveListErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ReceiveListErrorModel const &', 'arg0')])
    ## error-model.h: ns3::ReceiveListErrorModel::ReceiveListErrorModel() [constructor]
    cls.add_constructor([])
    ## error-model.h: std::list<unsigned int, std::allocator<unsigned int> > ns3::ReceiveListErrorModel::GetList() const [member function]
    cls.add_method('GetList', 
                   'std::list< unsigned int >', 
                   [], 
                   is_const=True)
    ## error-model.h: static ns3::TypeId ns3::ReceiveListErrorModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## error-model.h: void ns3::ReceiveListErrorModel::SetList(std::list<unsigned int, std::allocator<unsigned int> > const & packetlist) [member function]
    cls.add_method('SetList', 
                   'void', 
                   [param('std::list< unsigned int > const &', 'packetlist')])
    ## error-model.h: bool ns3::ReceiveListErrorModel::DoCorrupt(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorrupt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: void ns3::ReceiveListErrorModel::DoReset() [member function]
    cls.add_method('DoReset', 
                   'void', 
                   [], 
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
    ## data-rate.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeDataRateChecker() [free function]
    module.add_function('MakeDataRateChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
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

