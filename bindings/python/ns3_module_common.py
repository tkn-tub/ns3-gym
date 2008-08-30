from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## error-model.h: ns3::ErrorUnit [enumeration]
    module.add_enum('ErrorUnit', ['EU_BIT', 'EU_BYTE', 'EU_PKT'])
    ## packet.h: ns3::Packet [class]
    module.add_class('Packet', memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## packet.h: ns3::TagIterator [class]
    module.add_class('TagIterator')
    ## packet.h: ns3::TagIterator::Item [class]
    module.add_class('Item', outer_class=root_module['ns3::TagIterator'])
    ## tag-list.h: ns3::TagList [class]
    module.add_class('TagList')
    ## tag-list.h: ns3::TagList::Iterator [class]
    module.add_class('Iterator', outer_class=root_module['ns3::TagList'])
    ## tag-list.h: ns3::TagList::Iterator::Item [struct]
    module.add_class('Item', outer_class=root_module['ns3::TagList::Iterator'])
    ## tag-buffer.h: ns3::TagBuffer [class]
    module.add_class('TagBuffer')
    ## buffer.h: ns3::Buffer [class]
    module.add_class('Buffer')
    ## buffer.h: ns3::Buffer::Iterator [class]
    module.add_class('Iterator', outer_class=root_module['ns3::Buffer'])
    ## chunk.h: ns3::Chunk [class]
    module.add_class('Chunk', parent=root_module['ns3::ObjectBase'])
    ## data-rate.h: ns3::DataRate [class]
    module.add_class('DataRate')
    ## tag.h: ns3::Tag [class]
    module.add_class('Tag', parent=root_module['ns3::ObjectBase'])
    ## pcap-writer.h: ns3::PcapWriter [class]
    module.add_class('PcapWriter', parent=root_module['ns3::RefCountBase'])
    ## data-rate.h: ns3::DataRateChecker [class]
    module.add_class('DataRateChecker', parent=root_module['ns3::AttributeChecker'])
    ## error-model.h: ns3::ErrorModel [class]
    module.add_class('ErrorModel', parent=root_module['ns3::Object'])
    ## header.h: ns3::Header [class]
    module.add_class('Header', parent=root_module['ns3::Chunk'])
    ## trailer.h: ns3::Trailer [class]
    module.add_class('Trailer', parent=root_module['ns3::Chunk'])
    ## error-model.h: ns3::ListErrorModel [class]
    module.add_class('ListErrorModel', parent=root_module['ns3::ErrorModel'])
    ## data-rate.h: ns3::DataRateValue [class]
    module.add_class('DataRateValue', parent=root_module['ns3::AttributeValue'])
    ## error-model.h: ns3::RateErrorModel [class]
    module.add_class('RateErrorModel', parent=root_module['ns3::ErrorModel'])
    
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
    register_Ns3Packet_methods(root_module, root_module['ns3::Packet'])
    register_Ns3TagIterator_methods(root_module, root_module['ns3::TagIterator'])
    register_Ns3TagIteratorItem_methods(root_module, root_module['ns3::TagIterator::Item'])
    register_Ns3TagList_methods(root_module, root_module['ns3::TagList'])
    register_Ns3TagListIterator_methods(root_module, root_module['ns3::TagList::Iterator'])
    register_Ns3TagListIteratorItem_methods(root_module, root_module['ns3::TagList::Iterator::Item'])
    register_Ns3TagBuffer_methods(root_module, root_module['ns3::TagBuffer'])
    register_Ns3Buffer_methods(root_module, root_module['ns3::Buffer'])
    register_Ns3BufferIterator_methods(root_module, root_module['ns3::Buffer::Iterator'])
    register_Ns3Chunk_methods(root_module, root_module['ns3::Chunk'])
    register_Ns3DataRate_methods(root_module, root_module['ns3::DataRate'])
    register_Ns3Tag_methods(root_module, root_module['ns3::Tag'])
    register_Ns3PcapWriter_methods(root_module, root_module['ns3::PcapWriter'])
    register_Ns3DataRateChecker_methods(root_module, root_module['ns3::DataRateChecker'])
    register_Ns3ErrorModel_methods(root_module, root_module['ns3::ErrorModel'])
    register_Ns3Header_methods(root_module, root_module['ns3::Header'])
    register_Ns3Trailer_methods(root_module, root_module['ns3::Trailer'])
    register_Ns3ListErrorModel_methods(root_module, root_module['ns3::ListErrorModel'])
    register_Ns3DataRateValue_methods(root_module, root_module['ns3::DataRateValue'])
    register_Ns3RateErrorModel_methods(root_module, root_module['ns3::RateErrorModel'])
    return

def register_Ns3Packet_methods(root_module, cls):
    ## packet.h: ns3::Packet::Packet() [constructor]
    cls.add_constructor([])
    ## packet.h: ns3::Packet::Packet(ns3::Packet const & o) [copy constructor]
    cls.add_constructor([param('ns3::Packet&', 'o', is_const=True)])
    ## packet.h: ns3::Packet::Packet(uint32_t size) [constructor]
    cls.add_constructor([param('uint32_t', 'size')])
    ## packet.h: ns3::Packet::Packet(uint8_t const * buffer, uint32_t size) [constructor]
    cls.add_constructor([param('uint8_t *', 'buffer', transfer_ownership=False, is_const=True), param('uint32_t', 'size')])
    ## packet.h: void ns3::Packet::AddAtEnd(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')])
    ## packet.h: void ns3::Packet::AddHeader(ns3::Header const & header) [member function]
    cls.add_method('AddHeader', 
                   'void', 
                   [param('ns3::Header&', 'header', is_const=True)])
    ## packet.h: void ns3::Packet::AddPaddingAtEnd(uint32_t size) [member function]
    cls.add_method('AddPaddingAtEnd', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h: void ns3::Packet::AddTag(ns3::Tag const & tag) const [member function]
    cls.add_method('AddTag', 
                   'void', 
                   [param('ns3::Tag&', 'tag', is_const=True)], 
                   is_const=True)
    ## packet.h: void ns3::Packet::AddTrailer(ns3::Trailer const & trailer) [member function]
    cls.add_method('AddTrailer', 
                   'void', 
                   [param('ns3::Trailer&', 'trailer', is_const=True)])
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
    ## packet.h: ns3::Ptr<ns3::Packet> ns3::Packet::CreateFragment(uint32_t start, uint32_t length) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'start'), param('uint32_t', 'length')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::Deserialize(ns3::Buffer buffer) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer', 'buffer')])
    ## packet.h: static void ns3::Packet::EnableChecking() [member function]
    cls.add_method('EnableChecking', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet.h: static void ns3::Packet::EnableMetadata() [member function]
    cls.add_method('EnableMetadata', 
                   'void', 
                   [], 
                   is_static=True, deprecated=True)
    ## packet.h: static void ns3::Packet::EnablePrinting() [member function]
    cls.add_method('EnablePrinting', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet.h: bool ns3::Packet::FindFirstMatchingTag(ns3::Tag & tag) const [member function]
    cls.add_method('FindFirstMatchingTag', 
                   'bool', 
                   [param('ns3::Tag&', 'tag')], 
                   is_const=True)
    ## packet.h: uint32_t ns3::Packet::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h: ns3::TagIterator ns3::Packet::GetTagIterator() const [member function]
    cls.add_method('GetTagIterator', 
                   'ns3::TagIterator', 
                   [], 
                   is_const=True)
    ## packet.h: uint32_t ns3::Packet::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h: uint8_t const * ns3::Packet::PeekData() const [member function]
    cls.add_method('PeekData', 
                   retval('uint8_t *', is_const=True, caller_owns_return=False), 
                   [], 
                   is_const=True)
    ## packet.h: uint32_t ns3::Packet::PeekHeader(ns3::Header & header) [member function]
    cls.add_method('PeekHeader', 
                   'uint32_t', 
                   [param('ns3::Header&', 'header')])
    ## packet.h: uint32_t ns3::Packet::PeekTrailer(ns3::Trailer & trailer) [member function]
    cls.add_method('PeekTrailer', 
                   'uint32_t', 
                   [param('ns3::Trailer&', 'trailer')])
    ## packet.h: void ns3::Packet::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::PrintTags(std::ostream & os) const [member function]
    cls.add_method('PrintTags', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_const=True)
    ## packet.h: void ns3::Packet::RemoveAllTags() [member function]
    cls.add_method('RemoveAllTags', 
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
                   [param('ns3::Header&', 'header')])
    ## packet.h: uint32_t ns3::Packet::RemoveTrailer(ns3::Trailer & trailer) [member function]
    cls.add_method('RemoveTrailer', 
                   'uint32_t', 
                   [param('ns3::Trailer&', 'trailer')])
    ## packet.h: ns3::Buffer ns3::Packet::Serialize() const [member function]
    cls.add_method('Serialize', 
                   'ns3::Buffer', 
                   [], 
                   is_const=True)
    cls.add_output_stream_operator()
    return

def register_Ns3TagIterator_methods(root_module, cls):
    ## packet.h: ns3::TagIterator::TagIterator(ns3::TagIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TagIterator&', 'arg0', is_const=True)])
    ## packet.h: bool ns3::TagIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet.h: ns3::TagIterator::Item ns3::TagIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::TagIterator::Item', 
                   [])
    return

def register_Ns3TagIteratorItem_methods(root_module, cls):
    ## packet.h: ns3::TagIterator::Item::Item(ns3::TagIterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TagIterator::Item&', 'arg0', is_const=True)])
    ## packet.h: ns3::TypeId ns3::TagIterator::Item::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## packet.h: uint32_t ns3::TagIterator::Item::GetStart() const [member function]
    cls.add_method('GetStart', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h: uint32_t ns3::TagIterator::Item::GetEnd() const [member function]
    cls.add_method('GetEnd', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h: void ns3::TagIterator::Item::GetTag(ns3::Tag & tag) const [member function]
    cls.add_method('GetTag', 
                   'void', 
                   [param('ns3::Tag&', 'tag')], 
                   is_const=True)
    return

def register_Ns3TagList_methods(root_module, cls):
    ## tag-list.h: ns3::TagList::TagList() [constructor]
    cls.add_constructor([])
    ## tag-list.h: ns3::TagList::TagList(ns3::TagList const & o) [copy constructor]
    cls.add_constructor([param('ns3::TagList&', 'o', is_const=True)])
    ## tag-list.h: ns3::TagBuffer ns3::TagList::Add(ns3::TypeId tid, uint32_t bufferSize, int32_t start, int32_t end) [member function]
    cls.add_method('Add', 
                   'ns3::TagBuffer', 
                   [param('ns3::TypeId', 'tid'), param('uint32_t', 'bufferSize'), param('int32_t', 'start'), param('int32_t', 'end')])
    ## tag-list.h: void ns3::TagList::Add(ns3::TagList const & o) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::TagList&', 'o', is_const=True)])
    ## tag-list.h: void ns3::TagList::RemoveAll() [member function]
    cls.add_method('RemoveAll', 
                   'void', 
                   [])
    ## tag-list.h: ns3::TagList::Iterator ns3::TagList::Begin(int32_t offsetStart, int32_t offsetEnd) const [member function]
    cls.add_method('Begin', 
                   'ns3::TagList::Iterator', 
                   [param('int32_t', 'offsetStart'), param('int32_t', 'offsetEnd')], 
                   is_const=True)
    ## tag-list.h: void ns3::TagList::AddAtEnd(int32_t adjustment, int32_t appendOffset) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('int32_t', 'adjustment'), param('int32_t', 'appendOffset')])
    ## tag-list.h: void ns3::TagList::AddAtStart(int32_t adjustment, int32_t prependOffset) [member function]
    cls.add_method('AddAtStart', 
                   'void', 
                   [param('int32_t', 'adjustment'), param('int32_t', 'prependOffset')])
    return

def register_Ns3TagListIterator_methods(root_module, cls):
    ## tag-list.h: ns3::TagList::Iterator::Iterator(ns3::TagList::Iterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TagList::Iterator&', 'arg0', is_const=True)])
    ## tag-list.h: bool ns3::TagList::Iterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## tag-list.h: ns3::TagList::Iterator::Item ns3::TagList::Iterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::TagList::Iterator::Item', 
                   [])
    ## tag-list.h: uint32_t ns3::TagList::Iterator::GetOffsetStart() const [member function]
    cls.add_method('GetOffsetStart', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3TagListIteratorItem_methods(root_module, cls):
    ## tag-list.h: ns3::TagList::Iterator::Item::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    ## tag-list.h: ns3::TagList::Iterator::Item::size [variable]
    cls.add_instance_attribute('size', 'uint32_t', is_const=False)
    ## tag-list.h: ns3::TagList::Iterator::Item::start [variable]
    cls.add_instance_attribute('start', 'int32_t', is_const=False)
    ## tag-list.h: ns3::TagList::Iterator::Item::end [variable]
    cls.add_instance_attribute('end', 'int32_t', is_const=False)
    ## tag-list.h: ns3::TagList::Iterator::Item::buf [variable]
    cls.add_instance_attribute('buf', 'ns3::TagBuffer', is_const=False)
    ## tag-list.h: ns3::TagList::Iterator::Item::Item(ns3::TagList::Iterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TagList::Iterator::Item&', 'arg0', is_const=True)])
    ## tag-list.h: ns3::TagList::Iterator::Item::Item(ns3::TagBuffer buf) [constructor]
    cls.add_constructor([param('ns3::TagBuffer', 'buf')])
    return

def register_Ns3TagBuffer_methods(root_module, cls):
    ## tag-buffer.h: ns3::TagBuffer::TagBuffer(ns3::TagBuffer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TagBuffer&', 'arg0', is_const=True)])
    ## tag-buffer.h: ns3::TagBuffer::TagBuffer(uint8_t * start, uint8_t * end) [constructor]
    cls.add_constructor([param('uint8_t *', 'start'), param('uint8_t *', 'end')])
    ## tag-buffer.h: void ns3::TagBuffer::TrimAtEnd(uint32_t trim) [member function]
    cls.add_method('TrimAtEnd', 
                   'void', 
                   [param('uint32_t', 'trim')])
    ## tag-buffer.h: void ns3::TagBuffer::CopyFrom(ns3::TagBuffer o) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('ns3::TagBuffer', 'o')])
    ## tag-buffer.h: void ns3::TagBuffer::WriteU8(uint8_t v) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'v')])
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
    ## tag-buffer.h: void ns3::TagBuffer::WriteDouble(double v) [member function]
    cls.add_method('WriteDouble', 
                   'void', 
                   [param('double', 'v')])
    ## tag-buffer.h: void ns3::TagBuffer::Write(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint8_t *', 'buffer', transfer_ownership=False, is_const=True), param('uint32_t', 'size')])
    ## tag-buffer.h: uint8_t ns3::TagBuffer::ReadU8() [member function]
    cls.add_method('ReadU8', 
                   'uint8_t', 
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
    ## tag-buffer.h: double ns3::TagBuffer::ReadDouble() [member function]
    cls.add_method('ReadDouble', 
                   'double', 
                   [])
    ## tag-buffer.h: void ns3::TagBuffer::Read(uint8_t * buffer, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')])
    return

def register_Ns3Buffer_methods(root_module, cls):
    ## buffer.h: uint32_t ns3::Buffer::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h: uint8_t const * ns3::Buffer::PeekData() const [member function]
    cls.add_method('PeekData', 
                   retval('uint8_t *', is_const=True, caller_owns_return=False), 
                   [], 
                   is_const=True)
    ## buffer.h: bool ns3::Buffer::AddAtStart(uint32_t start) [member function]
    cls.add_method('AddAtStart', 
                   'bool', 
                   [param('uint32_t', 'start')])
    ## buffer.h: bool ns3::Buffer::AddAtEnd(uint32_t end) [member function]
    cls.add_method('AddAtEnd', 
                   'bool', 
                   [param('uint32_t', 'end')])
    ## buffer.h: void ns3::Buffer::AddAtEnd(ns3::Buffer const & o) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Buffer&', 'o', is_const=True)])
    ## buffer.h: void ns3::Buffer::RemoveAtStart(uint32_t start) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'start')])
    ## buffer.h: void ns3::Buffer::RemoveAtEnd(uint32_t end) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## buffer.h: ns3::Buffer ns3::Buffer::CreateFragment(uint32_t start, uint32_t length) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::Buffer', 
                   [param('uint32_t', 'start'), param('uint32_t', 'length')], 
                   is_const=True)
    ## buffer.h: ns3::Buffer::Iterator ns3::Buffer::Begin() const [member function]
    cls.add_method('Begin', 
                   'ns3::Buffer::Iterator', 
                   [], 
                   is_const=True)
    ## buffer.h: ns3::Buffer::Iterator ns3::Buffer::End() const [member function]
    cls.add_method('End', 
                   'ns3::Buffer::Iterator', 
                   [], 
                   is_const=True)
    ## buffer.h: ns3::Buffer ns3::Buffer::CreateFullCopy() const [member function]
    cls.add_method('CreateFullCopy', 
                   'ns3::Buffer', 
                   [], 
                   is_const=True)
    ## buffer.h: int32_t ns3::Buffer::GetCurrentStartOffset() const [member function]
    cls.add_method('GetCurrentStartOffset', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## buffer.h: int32_t ns3::Buffer::GetCurrentEndOffset() const [member function]
    cls.add_method('GetCurrentEndOffset', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## buffer.h: ns3::Buffer::Buffer(ns3::Buffer const & o) [copy constructor]
    cls.add_constructor([param('ns3::Buffer&', 'o', is_const=True)])
    ## buffer.h: ns3::Buffer::Buffer() [constructor]
    cls.add_constructor([])
    ## buffer.h: ns3::Buffer::Buffer(uint32_t dataSize) [constructor]
    cls.add_constructor([param('uint32_t', 'dataSize')])
    return

def register_Ns3BufferIterator_methods(root_module, cls):
    ## buffer.h: ns3::Buffer::Iterator::Iterator(ns3::Buffer::Iterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator&', 'arg0', is_const=True)])
    ## buffer.h: ns3::Buffer::Iterator::Iterator() [constructor]
    cls.add_constructor([])
    ## buffer.h: void ns3::Buffer::Iterator::Next() [member function]
    cls.add_method('Next', 
                   'void', 
                   [])
    ## buffer.h: void ns3::Buffer::Iterator::Prev() [member function]
    cls.add_method('Prev', 
                   'void', 
                   [])
    ## buffer.h: void ns3::Buffer::Iterator::Next(uint32_t delta) [member function]
    cls.add_method('Next', 
                   'void', 
                   [param('uint32_t', 'delta')])
    ## buffer.h: void ns3::Buffer::Iterator::Prev(uint32_t delta) [member function]
    cls.add_method('Prev', 
                   'void', 
                   [param('uint32_t', 'delta')])
    ## buffer.h: uint32_t ns3::Buffer::Iterator::GetDistanceFrom(ns3::Buffer::Iterator const & o) const [member function]
    cls.add_method('GetDistanceFrom', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator&', 'o', is_const=True)], 
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
    ## buffer.h: void ns3::Buffer::Iterator::WriteU8(uint8_t data) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'data')])
    ## buffer.h: void ns3::Buffer::Iterator::WriteU8(uint8_t data, uint32_t len) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'data'), param('uint32_t', 'len')])
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
    ## buffer.h: void ns3::Buffer::Iterator::Write(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint8_t *', 'buffer', transfer_ownership=False, is_const=True), param('uint32_t', 'size')])
    ## buffer.h: void ns3::Buffer::Iterator::Write(ns3::Buffer::Iterator start, ns3::Buffer::Iterator end) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start'), param('ns3::Buffer::Iterator', 'end')])
    ## buffer.h: uint8_t ns3::Buffer::Iterator::ReadU8() [member function]
    cls.add_method('ReadU8', 
                   'uint8_t', 
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
    ## buffer.h: void ns3::Buffer::Iterator::Read(uint8_t * buffer, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h: uint16_t ns3::Buffer::Iterator::CalculateIpChecksum(uint16_t size) [member function]
    cls.add_method('CalculateIpChecksum', 
                   'uint16_t', 
                   [param('uint16_t', 'size')])
    ## buffer.h: uint16_t ns3::Buffer::Iterator::CalculateIpChecksum(uint16_t size, uint32_t initialChecksum) [member function]
    cls.add_method('CalculateIpChecksum', 
                   'uint16_t', 
                   [param('uint16_t', 'size'), param('uint32_t', 'initialChecksum')])
    ## buffer.h: uint32_t ns3::Buffer::Iterator::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3Chunk_methods(root_module, cls):
    ## chunk.h: ns3::Chunk::Chunk(ns3::Chunk const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Chunk&', 'arg0', is_const=True)])
    ## chunk.h: ns3::Chunk::Chunk() [constructor]
    cls.add_constructor([])
    ## chunk.h: static ns3::TypeId ns3::Chunk::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## chunk.h: uint32_t ns3::Chunk::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_virtual=True)
    ## chunk.h: void ns3::Chunk::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3DataRate_methods(root_module, cls):
    ## data-rate.h: ns3::DataRate::DataRate(ns3::DataRate const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRate&', 'arg0', is_const=True)])
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
    cls.add_output_stream_operator()
    return

def register_Ns3Tag_methods(root_module, cls):
    ## tag.h: ns3::Tag::Tag(ns3::Tag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Tag&', 'arg0', is_const=True)])
    ## tag.h: ns3::Tag::Tag() [constructor]
    cls.add_constructor([])
    ## tag.h: static ns3::TypeId ns3::Tag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tag.h: uint32_t ns3::Tag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## tag.h: void ns3::Tag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## tag.h: void ns3::Tag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_pure_virtual=True, is_virtual=True)
    ## tag.h: void ns3::Tag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream&', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3PcapWriter_methods(root_module, cls):
    ## pcap-writer.h: ns3::PcapWriter::PcapWriter(ns3::PcapWriter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PcapWriter&', 'arg0', is_const=True)])
    ## pcap-writer.h: ns3::PcapWriter::PcapWriter() [constructor]
    cls.add_constructor([])
    ## pcap-writer.h: void ns3::PcapWriter::Open(std::string const & name) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string&', 'name', is_const=True)])
    ## pcap-writer.h: void ns3::PcapWriter::WriteEthernetHeader() [member function]
    cls.add_method('WriteEthernetHeader', 
                   'void', 
                   [])
    ## pcap-writer.h: void ns3::PcapWriter::WriteIpHeader() [member function]
    cls.add_method('WriteIpHeader', 
                   'void', 
                   [])
    ## pcap-writer.h: void ns3::PcapWriter::WriteWifiHeader() [member function]
    cls.add_method('WriteWifiHeader', 
                   'void', 
                   [])
    ## pcap-writer.h: void ns3::PcapWriter::WritePppHeader() [member function]
    cls.add_method('WritePppHeader', 
                   'void', 
                   [])
    ## pcap-writer.h: void ns3::PcapWriter::WritePacket(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('WritePacket', 
                   'void', 
                   [param('ns3::Ptr< const ns3::Packet >', 'packet')])
    return

def register_Ns3DataRateChecker_methods(root_module, cls):
    ## data-rate.h: ns3::DataRateChecker::DataRateChecker(ns3::DataRateChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRateChecker&', 'arg0', is_const=True)])
    ## data-rate.h: ns3::DataRateChecker::DataRateChecker() [constructor]
    cls.add_constructor([])
    return

def register_Ns3ErrorModel_methods(root_module, cls):
    ## error-model.h: ns3::ErrorModel::ErrorModel(ns3::ErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErrorModel&', 'arg0', is_const=True)])
    ## error-model.h: static ns3::TypeId ns3::ErrorModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## error-model.h: ns3::ErrorModel::ErrorModel() [constructor]
    cls.add_constructor([])
    ## error-model.h: bool ns3::ErrorModel::IsCorrupt(ns3::Ptr<ns3::Packet> pkt) [member function]
    cls.add_method('IsCorrupt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'pkt')])
    ## error-model.h: void ns3::ErrorModel::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## error-model.h: void ns3::ErrorModel::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [])
    ## error-model.h: void ns3::ErrorModel::Disable() [member function]
    cls.add_method('Disable', 
                   'void', 
                   [])
    ## error-model.h: bool ns3::ErrorModel::IsEnabled() const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
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

def register_Ns3Header_methods(root_module, cls):
    ## header.h: ns3::Header::Header() [constructor]
    cls.add_constructor([])
    ## header.h: ns3::Header::Header(ns3::Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Header&', 'arg0', is_const=True)])
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
                   [param('std::ostream&', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## header.h: void ns3::Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    cls.add_output_stream_operator()
    return

def register_Ns3Trailer_methods(root_module, cls):
    ## trailer.h: ns3::Trailer::Trailer() [constructor]
    cls.add_constructor([])
    ## trailer.h: ns3::Trailer::Trailer(ns3::Trailer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Trailer&', 'arg0', is_const=True)])
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
                   [param('std::ostream&', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trailer.h: void ns3::Trailer::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    cls.add_output_stream_operator()
    return

def register_Ns3ListErrorModel_methods(root_module, cls):
    ## error-model.h: ns3::ListErrorModel::ListErrorModel(ns3::ListErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ListErrorModel&', 'arg0', is_const=True)])
    ## error-model.h: static ns3::TypeId ns3::ListErrorModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## error-model.h: ns3::ListErrorModel::ListErrorModel() [constructor]
    cls.add_constructor([])
    ## error-model.h: std::list<unsigned int, std::allocator<unsigned int> > ns3::ListErrorModel::GetList() const [member function]
    cls.add_method('GetList', 
                   'std::list< unsigned int >', 
                   [], 
                   is_const=True)
    ## error-model.h: void ns3::ListErrorModel::SetList(std::list<unsigned int, std::allocator<unsigned int> > const & packetlist) [member function]
    cls.add_method('SetList', 
                   'void', 
                   [param('std::list< unsigned int >&', 'packetlist', is_const=True)])
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

def register_Ns3DataRateValue_methods(root_module, cls):
    ## data-rate.h: ns3::DataRateValue::DataRateValue(ns3::DataRateValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRateValue&', 'arg0', is_const=True)])
    ## data-rate.h: ns3::DataRateValue::DataRateValue() [constructor]
    cls.add_constructor([])
    ## data-rate.h: ns3::DataRateValue::DataRateValue(ns3::DataRate const & value) [constructor]
    cls.add_constructor([param('ns3::DataRate&', 'value', is_const=True)])
    ## data-rate.h: void ns3::DataRateValue::Set(ns3::DataRate const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::DataRate&', 'value', is_const=True)])
    ## data-rate.h: ns3::DataRate ns3::DataRateValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::DataRate', 
                   [], 
                   is_const=True)
    ## data-rate.h: ns3::Ptr<ns3::AttributeValue> ns3::DataRateValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## data-rate.h: std::string ns3::DataRateValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## data-rate.h: bool ns3::DataRateValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    return

def register_Ns3RateErrorModel_methods(root_module, cls):
    ## error-model.h: ns3::RateErrorModel::RateErrorModel(ns3::RateErrorModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RateErrorModel&', 'arg0', is_const=True)])
    ## error-model.h: static ns3::TypeId ns3::RateErrorModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## error-model.h: ns3::RateErrorModel::RateErrorModel() [constructor]
    cls.add_constructor([])
    ## error-model.h: ns3::ErrorUnit ns3::RateErrorModel::GetUnit() const [member function]
    cls.add_method('GetUnit', 
                   'ns3::ErrorUnit', 
                   [], 
                   is_const=True)
    ## error-model.h: void ns3::RateErrorModel::SetUnit(ns3::ErrorUnit error_unit) [member function]
    cls.add_method('SetUnit', 
                   'void', 
                   [param('ns3::ErrorUnit', 'error_unit')])
    ## error-model.h: double ns3::RateErrorModel::GetRate() const [member function]
    cls.add_method('GetRate', 
                   'double', 
                   [], 
                   is_const=True)
    ## error-model.h: void ns3::RateErrorModel::SetRate(double rate) [member function]
    cls.add_method('SetRate', 
                   'void', 
                   [param('double', 'rate')])
    ## error-model.h: void ns3::RateErrorModel::SetRandomVariable(ns3::RandomVariable const & ranvar) [member function]
    cls.add_method('SetRandomVariable', 
                   'void', 
                   [param('ns3::RandomVariable&', 'ranvar', is_const=True)])
    ## error-model.h: bool ns3::RateErrorModel::DoCorrupt(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorrupt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: bool ns3::RateErrorModel::DoCorruptPkt(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorruptPkt', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: bool ns3::RateErrorModel::DoCorruptByte(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorruptByte', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: bool ns3::RateErrorModel::DoCorruptBit(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoCorruptBit', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## error-model.h: void ns3::RateErrorModel::DoReset() [member function]
    cls.add_method('DoReset', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    ## data-rate.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeDataRateChecker() [free function]
    module.add_function('MakeDataRateChecker', 
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

