from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## error-model.h: ns3::ErrorUnit [enumeration]
    module.add_enum('ErrorUnit', ['EU_BIT', 'EU_BYTE', 'EU_PKT'])
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
    ## tag.h: ns3::Tag [class]
    module.add_class('Tag', parent=root_module['ns3::ObjectBase'])
    ## tag-buffer.h: ns3::TagBuffer [class]
    module.add_class('TagBuffer')
