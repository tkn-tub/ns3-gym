from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## packetbb.h: ns3::PbbAddressLength [enumeration]
    module.add_enum('PbbAddressLength', ['IPV4', 'IPV6'])
    ## error-model.h: ns3::ErrorUnit [enumeration]
    module.add_enum('ErrorUnit', ['EU_BIT', 'EU_BYTE', 'EU_PKT'])
    ## ethernet-header.h: ns3::ethernet_header_t [enumeration]
    module.add_enum('ethernet_header_t', ['LENGTH', 'VLAN', 'QINQ'])
    ## address.h: ns3::Address [class]
    module.add_class('Address')
    ## address.h: ns3::Address::MaxSize_e [enumeration]
    module.add_enum('MaxSize_e', ['MAX_SIZE'], outer_class=root_module['ns3::Address'])
    ## application-container.h: ns3::ApplicationContainer [class]
    module.add_class('ApplicationContainer')
    ## trace-helper.h: ns3::AsciiTraceHelper [class]
    module.add_class('AsciiTraceHelper')
    ## trace-helper.h: ns3::AsciiTraceHelperForDevice [class]
    module.add_class('AsciiTraceHelperForDevice', allow_subclassing=True)
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
    ## channel-list.h: ns3::ChannelList [class]
    module.add_class('ChannelList')
    ## data-rate.h: ns3::DataRate [class]
    module.add_class('DataRate')
    ## inet6-socket-address.h: ns3::Inet6SocketAddress [class]
    module.add_class('Inet6SocketAddress')
    ## inet6-socket-address.h: ns3::Inet6SocketAddress [class]
    root_module['ns3::Inet6SocketAddress'].implicitly_converts_to(root_module['ns3::Address'])
    ## inet-socket-address.h: ns3::InetSocketAddress [class]
    module.add_class('InetSocketAddress')
    ## inet-socket-address.h: ns3::InetSocketAddress [class]
    root_module['ns3::InetSocketAddress'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv4-address.h: ns3::Ipv4Address [class]
    module.add_class('Ipv4Address')
    ## ipv4-address.h: ns3::Ipv4Address [class]
    root_module['ns3::Ipv4Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv4-address.h: ns3::Ipv4Mask [class]
    module.add_class('Ipv4Mask')
    ## ipv6-address.h: ns3::Ipv6Address [class]
    module.add_class('Ipv6Address')
    ## ipv6-address.h: ns3::Ipv6Address [class]
    root_module['ns3::Ipv6Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv6-address.h: ns3::Ipv6Prefix [class]
    module.add_class('Ipv6Prefix')
    ## mac48-address.h: ns3::Mac48Address [class]
    module.add_class('Mac48Address')
    ## mac48-address.h: ns3::Mac48Address [class]
    root_module['ns3::Mac48Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## mac64-address.h: ns3::Mac64Address [class]
    module.add_class('Mac64Address')
    ## mac64-address.h: ns3::Mac64Address [class]
    root_module['ns3::Mac64Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## net-device-container.h: ns3::NetDeviceContainer [class]
    module.add_class('NetDeviceContainer')
    ## node-container.h: ns3::NodeContainer [class]
    module.add_class('NodeContainer')
    ## node-list.h: ns3::NodeList [class]
    module.add_class('NodeList')
    ## packet-metadata.h: ns3::PacketMetadata [class]
    module.add_class('PacketMetadata')
    ## packet-metadata.h: ns3::PacketMetadata::Item [struct]
    module.add_class('Item', outer_class=root_module['ns3::PacketMetadata'])
    ## packet-metadata.h: ns3::PacketMetadata::Item [enumeration]
    module.add_enum('', ['PAYLOAD', 'HEADER', 'TRAILER'], outer_class=root_module['ns3::PacketMetadata::Item'])
    ## packet-metadata.h: ns3::PacketMetadata::ItemIterator [class]
    module.add_class('ItemIterator', outer_class=root_module['ns3::PacketMetadata'])
    ## packet-socket-address.h: ns3::PacketSocketAddress [class]
    module.add_class('PacketSocketAddress')
    ## packet-socket-address.h: ns3::PacketSocketAddress [class]
    root_module['ns3::PacketSocketAddress'].implicitly_converts_to(root_module['ns3::Address'])
    ## packet-socket-helper.h: ns3::PacketSocketHelper [class]
    module.add_class('PacketSocketHelper')
    ## packet.h: ns3::PacketTagIterator [class]
    module.add_class('PacketTagIterator')
    ## packet.h: ns3::PacketTagIterator::Item [class]
    module.add_class('Item', outer_class=root_module['ns3::PacketTagIterator'])
    ## packet-tag-list.h: ns3::PacketTagList [class]
    module.add_class('PacketTagList')
    ## packet-tag-list.h: ns3::PacketTagList::TagData [struct]
    module.add_class('TagData', outer_class=root_module['ns3::PacketTagList'])
    ## packetbb.h: ns3::PbbAddressTlvBlock [class]
    module.add_class('PbbAddressTlvBlock')
    ## packetbb.h: ns3::PbbTlvBlock [class]
    module.add_class('PbbTlvBlock')
    ## pcap-file.h: ns3::PcapFile [class]
    module.add_class('PcapFile')
    ## trace-helper.h: ns3::PcapHelper [class]
    module.add_class('PcapHelper')
    ## trace-helper.h: ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_NULL', 'DLT_EN10MB', 'DLT_PPP', 'DLT_RAW', 'DLT_IEEE802_11', 'DLT_PRISM_HEADER', 'DLT_IEEE802_11_RADIO'], outer_class=root_module['ns3::PcapHelper'])
    ## trace-helper.h: ns3::PcapHelperForDevice [class]
    module.add_class('PcapHelperForDevice', allow_subclassing=True)
    ## sequence-number.h: ns3::SequenceNumber<unsigned int, int> [class]
    module.add_class('SequenceNumber32')
    ## tag.h: ns3::Tag [class]
    module.add_class('Tag', parent=root_module['ns3::ObjectBase'])
    ## tag-buffer.h: ns3::TagBuffer [class]
    module.add_class('TagBuffer')
    ## chunk.h: ns3::Chunk [class]
    module.add_class('Chunk', parent=root_module['ns3::ObjectBase'])
    ## flow-id-tag.h: ns3::FlowIdTag [class]
    module.add_class('FlowIdTag', parent=root_module['ns3::Tag'])
    ## header.h: ns3::Header [class]
    module.add_class('Header', parent=root_module['ns3::Chunk'])
    ## llc-snap-header.h: ns3::LlcSnapHeader [class]
    module.add_class('LlcSnapHeader', parent=root_module['ns3::Header'])
    ## packet-burst.h: ns3::PacketBurst [class]
    module.add_class('PacketBurst', parent=root_module['ns3::Object'])
    ## pcap-file-wrapper.h: ns3::PcapFileWrapper [class]
    module.add_class('PcapFileWrapper', parent=root_module['ns3::Object'])
    ## queue.h: ns3::Queue [class]
    module.add_class('Queue', parent=root_module['ns3::Object'])
    ## radiotap-header.h: ns3::RadiotapHeader [class]
    module.add_class('RadiotapHeader', parent=root_module['ns3::Header'])
    ## radiotap-header.h: ns3::RadiotapHeader [enumeration]
    module.add_enum('', ['FRAME_FLAG_NONE', 'FRAME_FLAG_CFP', 'FRAME_FLAG_SHORT_PREAMBLE', 'FRAME_FLAG_WEP', 'FRAME_FLAG_FRAGMENTED', 'FRAME_FLAG_FCS_INCLUDED', 'FRAME_FLAG_DATA_PADDING', 'FRAME_FLAG_BAD_FCS', 'FRAME_FLAG_SHORT_GUARD'], outer_class=root_module['ns3::RadiotapHeader'])
    ## radiotap-header.h: ns3::RadiotapHeader [enumeration]
    module.add_enum('', ['CHANNEL_FLAG_NONE', 'CHANNEL_FLAG_TURBO', 'CHANNEL_FLAG_CCK', 'CHANNEL_FLAG_OFDM', 'CHANNEL_FLAG_SPECTRUM_2GHZ', 'CHANNEL_FLAG_SPECTRUM_5GHZ', 'CHANNEL_FLAG_PASSIVE', 'CHANNEL_FLAG_DYNAMIC', 'CHANNEL_FLAG_GFSK'], outer_class=root_module['ns3::RadiotapHeader'])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::NixVector', 'ns3::empty', 'ns3::DefaultDeleter<ns3::NixVector>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::OutputStreamWrapper', 'ns3::empty', 'ns3::DefaultDeleter<ns3::OutputStreamWrapper>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Packet', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Packet>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::PbbAddressBlock', 'ns3::empty', 'ns3::DefaultDeleter<ns3::PbbAddressBlock>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::PbbMessage', 'ns3::empty', 'ns3::DefaultDeleter<ns3::PbbMessage>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::PbbPacket', 'ns3::Header', 'ns3::DefaultDeleter<ns3::PbbPacket>'], parent=root_module['ns3::Header'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::PbbTlv', 'ns3::empty', 'ns3::DefaultDeleter<ns3::PbbTlv>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## socket.h: ns3::Socket [class]
    module.add_class('Socket', parent=root_module['ns3::Object'])
    ## socket.h: ns3::Socket::SocketErrno [enumeration]
    module.add_enum('SocketErrno', ['ERROR_NOTERROR', 'ERROR_ISCONN', 'ERROR_NOTCONN', 'ERROR_MSGSIZE', 'ERROR_AGAIN', 'ERROR_SHUTDOWN', 'ERROR_OPNOTSUPP', 'ERROR_AFNOSUPPORT', 'ERROR_INVAL', 'ERROR_BADF', 'ERROR_NOROUTETOHOST', 'ERROR_NODEV', 'ERROR_ADDRNOTAVAIL', 'SOCKET_ERRNO_LAST'], outer_class=root_module['ns3::Socket'])
    ## socket.h: ns3::Socket::SocketType [enumeration]
    module.add_enum('SocketType', ['NS3_SOCK_STREAM', 'NS3_SOCK_SEQPACKET', 'NS3_SOCK_DGRAM', 'NS3_SOCK_RAW'], outer_class=root_module['ns3::Socket'])
    ## socket.h: ns3::SocketAddressTag [class]
    module.add_class('SocketAddressTag', parent=root_module['ns3::Tag'])
    ## socket-factory.h: ns3::SocketFactory [class]
    module.add_class('SocketFactory', parent=root_module['ns3::Object'])
    ## socket.h: ns3::SocketIpTtlTag [class]
    module.add_class('SocketIpTtlTag', parent=root_module['ns3::Tag'])
    ## socket.h: ns3::SocketSetDontFragmentTag [class]
    module.add_class('SocketSetDontFragmentTag', parent=root_module['ns3::Tag'])
    ## trailer.h: ns3::Trailer [class]
    module.add_class('Trailer', parent=root_module['ns3::Chunk'])
    ## application.h: ns3::Application [class]
    module.add_class('Application', parent=root_module['ns3::Object'])
    ## channel.h: ns3::Channel [class]
    module.add_class('Channel', parent=root_module['ns3::Object'])
    ## data-rate.h: ns3::DataRateChecker [class]
    module.add_class('DataRateChecker', parent=root_module['ns3::AttributeChecker'])
    ## data-rate.h: ns3::DataRateValue [class]
    module.add_class('DataRateValue', parent=root_module['ns3::AttributeValue'])
    ## drop-tail-queue.h: ns3::DropTailQueue [class]
    module.add_class('DropTailQueue', parent=root_module['ns3::Queue'])
    ## drop-tail-queue.h: ns3::DropTailQueue::Mode [enumeration]
    module.add_enum('Mode', ['ILLEGAL', 'PACKETS', 'BYTES'], outer_class=root_module['ns3::DropTailQueue'])
    ## error-model.h: ns3::ErrorModel [class]
    module.add_class('ErrorModel', parent=root_module['ns3::Object'])
    ## ethernet-header.h: ns3::EthernetHeader [class]
    module.add_class('EthernetHeader', parent=root_module['ns3::Header'])
    ## ethernet-trailer.h: ns3::EthernetTrailer [class]
    module.add_class('EthernetTrailer', parent=root_module['ns3::Trailer'])
    ## ipv4-address.h: ns3::Ipv4AddressChecker [class]
    module.add_class('Ipv4AddressChecker', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h: ns3::Ipv4AddressValue [class]
    module.add_class('Ipv4AddressValue', parent=root_module['ns3::AttributeValue'])
    ## ipv4-address.h: ns3::Ipv4MaskChecker [class]
    module.add_class('Ipv4MaskChecker', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h: ns3::Ipv4MaskValue [class]
    module.add_class('Ipv4MaskValue', parent=root_module['ns3::AttributeValue'])
    ## ipv6-address.h: ns3::Ipv6AddressChecker [class]
    module.add_class('Ipv6AddressChecker', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h: ns3::Ipv6AddressValue [class]
    module.add_class('Ipv6AddressValue', parent=root_module['ns3::AttributeValue'])
    ## ipv6-address.h: ns3::Ipv6PrefixChecker [class]
    module.add_class('Ipv6PrefixChecker', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h: ns3::Ipv6PrefixValue [class]
    module.add_class('Ipv6PrefixValue', parent=root_module['ns3::AttributeValue'])
    ## error-model.h: ns3::ListErrorModel [class]
    module.add_class('ListErrorModel', parent=root_module['ns3::ErrorModel'])
    ## mac48-address.h: ns3::Mac48AddressChecker [class]
    module.add_class('Mac48AddressChecker', parent=root_module['ns3::AttributeChecker'])
    ## mac48-address.h: ns3::Mac48AddressValue [class]
    module.add_class('Mac48AddressValue', parent=root_module['ns3::AttributeValue'])
    ## net-device.h: ns3::NetDevice [class]
    module.add_class('NetDevice', parent=root_module['ns3::Object'])
    ## net-device.h: ns3::NetDevice::PacketType [enumeration]
    module.add_enum('PacketType', ['PACKET_HOST', 'NS3_PACKET_HOST', 'PACKET_BROADCAST', 'NS3_PACKET_BROADCAST', 'PACKET_MULTICAST', 'NS3_PACKET_MULTICAST', 'PACKET_OTHERHOST', 'NS3_PACKET_OTHERHOST'], outer_class=root_module['ns3::NetDevice'])
    ## nix-vector.h: ns3::NixVector [class]
    module.add_class('NixVector', parent=root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    ## node.h: ns3::Node [class]
    module.add_class('Node', parent=root_module['ns3::Object'])
    ## output-stream-wrapper.h: ns3::OutputStreamWrapper [class]
    module.add_class('OutputStreamWrapper', parent=root_module['ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >'])
    ## packet.h: ns3::Packet [class]
    module.add_class('Packet', parent=root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    ## packet-socket.h: ns3::PacketSocket [class]
    module.add_class('PacketSocket', parent=root_module['ns3::Socket'])
    ## packet-socket-factory.h: ns3::PacketSocketFactory [class]
    module.add_class('PacketSocketFactory', parent=root_module['ns3::SocketFactory'])
    ## packetbb.h: ns3::PbbAddressBlock [class]
    module.add_class('PbbAddressBlock', parent=root_module['ns3::SimpleRefCount< ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> >'])
    ## packetbb.h: ns3::PbbAddressBlockIpv4 [class]
    module.add_class('PbbAddressBlockIpv4', parent=root_module['ns3::PbbAddressBlock'])
    ## packetbb.h: ns3::PbbAddressBlockIpv6 [class]
    module.add_class('PbbAddressBlockIpv6', parent=root_module['ns3::PbbAddressBlock'])
    ## packetbb.h: ns3::PbbMessage [class]
    module.add_class('PbbMessage', parent=root_module['ns3::SimpleRefCount< ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> >'])
    ## packetbb.h: ns3::PbbMessageIpv4 [class]
    module.add_class('PbbMessageIpv4', parent=root_module['ns3::PbbMessage'])
    ## packetbb.h: ns3::PbbMessageIpv6 [class]
    module.add_class('PbbMessageIpv6', parent=root_module['ns3::PbbMessage'])
    ## packetbb.h: ns3::PbbPacket [class]
    module.add_class('PbbPacket', parent=root_module['ns3::SimpleRefCount< ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> >'])
    ## packetbb.h: ns3::PbbTlv [class]
    module.add_class('PbbTlv', parent=root_module['ns3::SimpleRefCount< ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> >'])
    ## error-model.h: ns3::RateErrorModel [class]
    module.add_class('RateErrorModel', parent=root_module['ns3::ErrorModel'])
    ## error-model.h: ns3::ReceiveListErrorModel [class]
    module.add_class('ReceiveListErrorModel', parent=root_module['ns3::ErrorModel'])
    ## simple-channel.h: ns3::SimpleChannel [class]
    module.add_class('SimpleChannel', parent=root_module['ns3::Channel'])
    ## simple-net-device.h: ns3::SimpleNetDevice [class]
    module.add_class('SimpleNetDevice', parent=root_module['ns3::NetDevice'])
    ## address.h: ns3::AddressChecker [class]
    module.add_class('AddressChecker', parent=root_module['ns3::AttributeChecker'])
    ## address.h: ns3::AddressValue [class]
    module.add_class('AddressValue', parent=root_module['ns3::AttributeValue'])
    ## packetbb.h: ns3::PbbAddressTlv [class]
    module.add_class('PbbAddressTlv', parent=root_module['ns3::PbbTlv'])
    module.add_container('ns3::olsr::MprSet', 'ns3::Ipv4Address', container_type='set')
    module.add_container('std::vector< ns3::Ipv4Address >', 'ns3::Ipv4Address', container_type='vector')
    module.add_container('std::vector< ns3::Ipv6Address >', 'ns3::Ipv6Address', container_type='vector')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >', 'ns3::SequenceNumber16')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >*', 'ns3::SequenceNumber16*')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >&', 'ns3::SequenceNumber16&')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >', 'ns3::SequenceNumber32')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >*', 'ns3::SequenceNumber32*')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >&', 'ns3::SequenceNumber32&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyTxEndCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyTxEndCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyTxEndCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxStartCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxStartCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxStartCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxEndErrorCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxEndErrorCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxEndErrorCallback&')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyTxStartCallback')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyTxStartCallback*')
    typehandlers.add_type_alias('ns3::Callback< bool, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyTxStartCallback&')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ns3::GenericPhyRxEndOkCallback')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >*', 'ns3::GenericPhyRxEndOkCallback*')
    typehandlers.add_type_alias('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >&', 'ns3::GenericPhyRxEndOkCallback&')
    
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
    
    module.add_container('std::vector< ns3::Mac48Address >', 'ns3::Mac48Address', container_type='vector')

def register_types_ns3_dsdv(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Address_methods(root_module, root_module['ns3::Address'])
    register_Ns3ApplicationContainer_methods(root_module, root_module['ns3::ApplicationContainer'])
    register_Ns3AsciiTraceHelper_methods(root_module, root_module['ns3::AsciiTraceHelper'])
    register_Ns3AsciiTraceHelperForDevice_methods(root_module, root_module['ns3::AsciiTraceHelperForDevice'])
    register_Ns3Buffer_methods(root_module, root_module['ns3::Buffer'])
    register_Ns3BufferIterator_methods(root_module, root_module['ns3::Buffer::Iterator'])
    register_Ns3ByteTagIterator_methods(root_module, root_module['ns3::ByteTagIterator'])
    register_Ns3ByteTagIteratorItem_methods(root_module, root_module['ns3::ByteTagIterator::Item'])
    register_Ns3ByteTagList_methods(root_module, root_module['ns3::ByteTagList'])
    register_Ns3ByteTagListIterator_methods(root_module, root_module['ns3::ByteTagList::Iterator'])
    register_Ns3ByteTagListIteratorItem_methods(root_module, root_module['ns3::ByteTagList::Iterator::Item'])
    register_Ns3ChannelList_methods(root_module, root_module['ns3::ChannelList'])
    register_Ns3DataRate_methods(root_module, root_module['ns3::DataRate'])
    register_Ns3Inet6SocketAddress_methods(root_module, root_module['ns3::Inet6SocketAddress'])
    register_Ns3InetSocketAddress_methods(root_module, root_module['ns3::InetSocketAddress'])
    register_Ns3Ipv4Address_methods(root_module, root_module['ns3::Ipv4Address'])
    register_Ns3Ipv4Mask_methods(root_module, root_module['ns3::Ipv4Mask'])
    register_Ns3Ipv6Address_methods(root_module, root_module['ns3::Ipv6Address'])
    register_Ns3Ipv6Prefix_methods(root_module, root_module['ns3::Ipv6Prefix'])
    register_Ns3Mac48Address_methods(root_module, root_module['ns3::Mac48Address'])
    register_Ns3Mac64Address_methods(root_module, root_module['ns3::Mac64Address'])
    register_Ns3NetDeviceContainer_methods(root_module, root_module['ns3::NetDeviceContainer'])
    register_Ns3NodeContainer_methods(root_module, root_module['ns3::NodeContainer'])
    register_Ns3NodeList_methods(root_module, root_module['ns3::NodeList'])
    register_Ns3PacketMetadata_methods(root_module, root_module['ns3::PacketMetadata'])
    register_Ns3PacketMetadataItem_methods(root_module, root_module['ns3::PacketMetadata::Item'])
    register_Ns3PacketMetadataItemIterator_methods(root_module, root_module['ns3::PacketMetadata::ItemIterator'])
    register_Ns3PacketSocketAddress_methods(root_module, root_module['ns3::PacketSocketAddress'])
    register_Ns3PacketSocketHelper_methods(root_module, root_module['ns3::PacketSocketHelper'])
    register_Ns3PacketTagIterator_methods(root_module, root_module['ns3::PacketTagIterator'])
    register_Ns3PacketTagIteratorItem_methods(root_module, root_module['ns3::PacketTagIterator::Item'])
    register_Ns3PacketTagList_methods(root_module, root_module['ns3::PacketTagList'])
    register_Ns3PacketTagListTagData_methods(root_module, root_module['ns3::PacketTagList::TagData'])
    register_Ns3PbbAddressTlvBlock_methods(root_module, root_module['ns3::PbbAddressTlvBlock'])
    register_Ns3PbbTlvBlock_methods(root_module, root_module['ns3::PbbTlvBlock'])
    register_Ns3PcapFile_methods(root_module, root_module['ns3::PcapFile'])
    register_Ns3PcapHelper_methods(root_module, root_module['ns3::PcapHelper'])
    register_Ns3PcapHelperForDevice_methods(root_module, root_module['ns3::PcapHelperForDevice'])
    register_Ns3SequenceNumber32_methods(root_module, root_module['ns3::SequenceNumber32'])
    register_Ns3Tag_methods(root_module, root_module['ns3::Tag'])
    register_Ns3TagBuffer_methods(root_module, root_module['ns3::TagBuffer'])
    register_Ns3Chunk_methods(root_module, root_module['ns3::Chunk'])
    register_Ns3FlowIdTag_methods(root_module, root_module['ns3::FlowIdTag'])
    register_Ns3Header_methods(root_module, root_module['ns3::Header'])
    register_Ns3LlcSnapHeader_methods(root_module, root_module['ns3::LlcSnapHeader'])
    register_Ns3PacketBurst_methods(root_module, root_module['ns3::PacketBurst'])
    register_Ns3PcapFileWrapper_methods(root_module, root_module['ns3::PcapFileWrapper'])
    register_Ns3Queue_methods(root_module, root_module['ns3::Queue'])
    register_Ns3RadiotapHeader_methods(root_module, root_module['ns3::RadiotapHeader'])
    register_Ns3Socket_methods(root_module, root_module['ns3::Socket'])
    register_Ns3SocketAddressTag_methods(root_module, root_module['ns3::SocketAddressTag'])
    register_Ns3SocketFactory_methods(root_module, root_module['ns3::SocketFactory'])
    register_Ns3SocketIpTtlTag_methods(root_module, root_module['ns3::SocketIpTtlTag'])
    register_Ns3SocketSetDontFragmentTag_methods(root_module, root_module['ns3::SocketSetDontFragmentTag'])
    register_Ns3Trailer_methods(root_module, root_module['ns3::Trailer'])
    register_Ns3Application_methods(root_module, root_module['ns3::Application'])
    register_Ns3Channel_methods(root_module, root_module['ns3::Channel'])
    register_Ns3DataRateChecker_methods(root_module, root_module['ns3::DataRateChecker'])
    register_Ns3DataRateValue_methods(root_module, root_module['ns3::DataRateValue'])
    register_Ns3DropTailQueue_methods(root_module, root_module['ns3::DropTailQueue'])
    register_Ns3ErrorModel_methods(root_module, root_module['ns3::ErrorModel'])
    register_Ns3EthernetHeader_methods(root_module, root_module['ns3::EthernetHeader'])
    register_Ns3EthernetTrailer_methods(root_module, root_module['ns3::EthernetTrailer'])
    register_Ns3Ipv4AddressChecker_methods(root_module, root_module['ns3::Ipv4AddressChecker'])
    register_Ns3Ipv4AddressValue_methods(root_module, root_module['ns3::Ipv4AddressValue'])
    register_Ns3Ipv4MaskChecker_methods(root_module, root_module['ns3::Ipv4MaskChecker'])
    register_Ns3Ipv4MaskValue_methods(root_module, root_module['ns3::Ipv4MaskValue'])
    register_Ns3Ipv6AddressChecker_methods(root_module, root_module['ns3::Ipv6AddressChecker'])
    register_Ns3Ipv6AddressValue_methods(root_module, root_module['ns3::Ipv6AddressValue'])
    register_Ns3Ipv6PrefixChecker_methods(root_module, root_module['ns3::Ipv6PrefixChecker'])
    register_Ns3Ipv6PrefixValue_methods(root_module, root_module['ns3::Ipv6PrefixValue'])
    register_Ns3ListErrorModel_methods(root_module, root_module['ns3::ListErrorModel'])
    register_Ns3Mac48AddressChecker_methods(root_module, root_module['ns3::Mac48AddressChecker'])
    register_Ns3Mac48AddressValue_methods(root_module, root_module['ns3::Mac48AddressValue'])
    register_Ns3NetDevice_methods(root_module, root_module['ns3::NetDevice'])
    register_Ns3NixVector_methods(root_module, root_module['ns3::NixVector'])
    register_Ns3Node_methods(root_module, root_module['ns3::Node'])
    register_Ns3OutputStreamWrapper_methods(root_module, root_module['ns3::OutputStreamWrapper'])
    register_Ns3Packet_methods(root_module, root_module['ns3::Packet'])
    register_Ns3PacketSocket_methods(root_module, root_module['ns3::PacketSocket'])
    register_Ns3PacketSocketFactory_methods(root_module, root_module['ns3::PacketSocketFactory'])
    register_Ns3PbbAddressBlock_methods(root_module, root_module['ns3::PbbAddressBlock'])
    register_Ns3PbbAddressBlockIpv4_methods(root_module, root_module['ns3::PbbAddressBlockIpv4'])
    register_Ns3PbbAddressBlockIpv6_methods(root_module, root_module['ns3::PbbAddressBlockIpv6'])
    register_Ns3PbbMessage_methods(root_module, root_module['ns3::PbbMessage'])
    register_Ns3PbbMessageIpv4_methods(root_module, root_module['ns3::PbbMessageIpv4'])
    register_Ns3PbbMessageIpv6_methods(root_module, root_module['ns3::PbbMessageIpv6'])
    register_Ns3PbbPacket_methods(root_module, root_module['ns3::PbbPacket'])
    register_Ns3PbbTlv_methods(root_module, root_module['ns3::PbbTlv'])
    register_Ns3RateErrorModel_methods(root_module, root_module['ns3::RateErrorModel'])
    register_Ns3ReceiveListErrorModel_methods(root_module, root_module['ns3::ReceiveListErrorModel'])
    register_Ns3SimpleChannel_methods(root_module, root_module['ns3::SimpleChannel'])
    register_Ns3SimpleNetDevice_methods(root_module, root_module['ns3::SimpleNetDevice'])
    register_Ns3AddressChecker_methods(root_module, root_module['ns3::AddressChecker'])
    register_Ns3AddressValue_methods(root_module, root_module['ns3::AddressValue'])
    register_Ns3PbbAddressTlv_methods(root_module, root_module['ns3::PbbAddressTlv'])
    return

def register_Ns3Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## address.h: ns3::Address::Address() [constructor]
    cls.add_constructor([])
    ## address.h: ns3::Address::Address(uint8_t type, uint8_t const * buffer, uint8_t len) [constructor]
    cls.add_constructor([param('uint8_t', 'type'), param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h: ns3::Address::Address(ns3::Address const & address) [copy constructor]
    cls.add_constructor([param('ns3::Address const &', 'address')])
    ## address.h: bool ns3::Address::CheckCompatible(uint8_t type, uint8_t len) const [member function]
    cls.add_method('CheckCompatible', 
                   'bool', 
                   [param('uint8_t', 'type'), param('uint8_t', 'len')], 
                   is_const=True)
    ## address.h: uint32_t ns3::Address::CopyAllFrom(uint8_t const * buffer, uint8_t len) [member function]
    cls.add_method('CopyAllFrom', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h: uint32_t ns3::Address::CopyAllTo(uint8_t * buffer, uint8_t len) const [member function]
    cls.add_method('CopyAllTo', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint8_t', 'len')], 
                   is_const=True)
    ## address.h: uint32_t ns3::Address::CopyFrom(uint8_t const * buffer, uint8_t len) [member function]
    cls.add_method('CopyFrom', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h: uint32_t ns3::Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## address.h: void ns3::Address::Deserialize(ns3::TagBuffer buffer) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buffer')])
    ## address.h: uint8_t ns3::Address::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## address.h: uint32_t ns3::Address::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## address.h: bool ns3::Address::IsInvalid() const [member function]
    cls.add_method('IsInvalid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## address.h: bool ns3::Address::IsMatchingType(uint8_t type) const [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('uint8_t', 'type')], 
                   is_const=True)
    ## address.h: static uint8_t ns3::Address::Register() [member function]
    cls.add_method('Register', 
                   'uint8_t', 
                   [], 
                   is_static=True)
    ## address.h: void ns3::Address::Serialize(ns3::TagBuffer buffer) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buffer')], 
                   is_const=True)
    return

def register_Ns3ApplicationContainer_methods(root_module, cls):
    ## application-container.h: ns3::ApplicationContainer::ApplicationContainer(ns3::ApplicationContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ApplicationContainer const &', 'arg0')])
    ## application-container.h: ns3::ApplicationContainer::ApplicationContainer() [constructor]
    cls.add_constructor([])
    ## application-container.h: ns3::ApplicationContainer::ApplicationContainer(ns3::Ptr<ns3::Application> application) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Application >', 'application')])
    ## application-container.h: ns3::ApplicationContainer::ApplicationContainer(std::string name) [constructor]
    cls.add_constructor([param('std::string', 'name')])
    ## application-container.h: void ns3::ApplicationContainer::Add(ns3::ApplicationContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::ApplicationContainer', 'other')])
    ## application-container.h: void ns3::ApplicationContainer::Add(ns3::Ptr<ns3::Application> application) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'application')])
    ## application-container.h: void ns3::ApplicationContainer::Add(std::string name) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'name')])
    ## application-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Application>*,std::vector<ns3::Ptr<ns3::Application>, std::allocator<ns3::Ptr<ns3::Application> > > > ns3::ApplicationContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Application > const, std::vector< ns3::Ptr< ns3::Application > > >', 
                   [], 
                   is_const=True)
    ## application-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Application>*,std::vector<ns3::Ptr<ns3::Application>, std::allocator<ns3::Ptr<ns3::Application> > > > ns3::ApplicationContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Application > const, std::vector< ns3::Ptr< ns3::Application > > >', 
                   [], 
                   is_const=True)
    ## application-container.h: ns3::Ptr<ns3::Application> ns3::ApplicationContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Application >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## application-container.h: uint32_t ns3::ApplicationContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## application-container.h: void ns3::ApplicationContainer::Start(ns3::Time start) [member function]
    cls.add_method('Start', 
                   'void', 
                   [param('ns3::Time', 'start')])
    ## application-container.h: void ns3::ApplicationContainer::Stop(ns3::Time stop) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time', 'stop')])
    return

def register_Ns3AsciiTraceHelper_methods(root_module, cls):
    ## trace-helper.h: ns3::AsciiTraceHelper::AsciiTraceHelper(ns3::AsciiTraceHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AsciiTraceHelper const &', 'arg0')])
    ## trace-helper.h: ns3::AsciiTraceHelper::AsciiTraceHelper() [constructor]
    cls.add_constructor([])
    ## trace-helper.h: ns3::Ptr<ns3::OutputStreamWrapper> ns3::AsciiTraceHelper::CreateFileStream(std::string filename, std::_Ios_Openmode filemode=std::ios_base::out) [member function]
    cls.add_method('CreateFileStream', 
                   'ns3::Ptr< ns3::OutputStreamWrapper >', 
                   [param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode', default_value='std::ios_base::out')])
    ## trace-helper.h: static void ns3::AsciiTraceHelper::DefaultDequeueSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDequeueSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h: static void ns3::AsciiTraceHelper::DefaultDequeueSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDequeueSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h: static void ns3::AsciiTraceHelper::DefaultDropSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDropSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h: static void ns3::AsciiTraceHelper::DefaultDropSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDropSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h: static void ns3::AsciiTraceHelper::DefaultEnqueueSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultEnqueueSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h: static void ns3::AsciiTraceHelper::DefaultEnqueueSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultEnqueueSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h: static void ns3::AsciiTraceHelper::DefaultReceiveSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultReceiveSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h: static void ns3::AsciiTraceHelper::DefaultReceiveSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultReceiveSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h: std::string ns3::AsciiTraceHelper::GetFilenameFromDevice(std::string prefix, ns3::Ptr<ns3::NetDevice> device, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromDevice', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'useObjectNames', default_value='true')])
    ## trace-helper.h: std::string ns3::AsciiTraceHelper::GetFilenameFromInterfacePair(std::string prefix, ns3::Ptr<ns3::Object> object, uint32_t interface, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromInterfacePair', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Object >', 'object'), param('uint32_t', 'interface'), param('bool', 'useObjectNames', default_value='true')])
    return

def register_Ns3AsciiTraceHelperForDevice_methods(root_module, cls):
    ## trace-helper.h: ns3::AsciiTraceHelperForDevice::AsciiTraceHelperForDevice(ns3::AsciiTraceHelperForDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AsciiTraceHelperForDevice const &', 'arg0')])
    ## trace-helper.h: ns3::AsciiTraceHelperForDevice::AsciiTraceHelperForDevice() [constructor]
    cls.add_constructor([])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename=false) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, std::string ndName, bool explicitFilename=false) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('std::string', 'ndName'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string ndName) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'ndName')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NetDeviceContainer', 'd')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::NetDeviceContainer', 'd')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NodeContainer', 'n')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::NodeContainer', 'n')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, uint32_t nodeid, uint32_t deviceid, bool explicitFilename) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('bool', 'explicitFilename')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAsciiAll(std::string prefix) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::string', 'prefix')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAsciiAll(ns3::Ptr<ns3::OutputStreamWrapper> stream) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')])
    ## trace-helper.h: void ns3::AsciiTraceHelperForDevice::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   is_pure_virtual=True, is_virtual=True)
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

def register_Ns3ChannelList_methods(root_module, cls):
    ## channel-list.h: ns3::ChannelList::ChannelList() [constructor]
    cls.add_constructor([])
    ## channel-list.h: ns3::ChannelList::ChannelList(ns3::ChannelList const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ChannelList const &', 'arg0')])
    ## channel-list.h: static uint32_t ns3::ChannelList::Add(ns3::Ptr<ns3::Channel> channel) [member function]
    cls.add_method('Add', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Channel >', 'channel')], 
                   is_static=True)
    ## channel-list.h: static __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Channel>*,std::vector<ns3::Ptr<ns3::Channel>, std::allocator<ns3::Ptr<ns3::Channel> > > > ns3::ChannelList::Begin() [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Channel > const, std::vector< ns3::Ptr< ns3::Channel > > >', 
                   [], 
                   is_static=True)
    ## channel-list.h: static __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Channel>*,std::vector<ns3::Ptr<ns3::Channel>, std::allocator<ns3::Ptr<ns3::Channel> > > > ns3::ChannelList::End() [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Channel > const, std::vector< ns3::Ptr< ns3::Channel > > >', 
                   [], 
                   is_static=True)
    ## channel-list.h: static ns3::Ptr<ns3::Channel> ns3::ChannelList::GetChannel(uint32_t n) [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [param('uint32_t', 'n')], 
                   is_static=True)
    ## channel-list.h: static uint32_t ns3::ChannelList::GetNChannels() [member function]
    cls.add_method('GetNChannels', 
                   'uint32_t', 
                   [], 
                   is_static=True)
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

def register_Ns3Inet6SocketAddress_methods(root_module, cls):
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(ns3::Inet6SocketAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Inet6SocketAddress const &', 'arg0')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(ns3::Ipv6Address ipv6, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'ipv6'), param('uint16_t', 'port')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(ns3::Ipv6Address ipv6) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address', 'ipv6')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(char const * ipv6, uint16_t port) [constructor]
    cls.add_constructor([param('char const *', 'ipv6'), param('uint16_t', 'port')])
    ## inet6-socket-address.h: ns3::Inet6SocketAddress::Inet6SocketAddress(char const * ipv6) [constructor]
    cls.add_constructor([param('char const *', 'ipv6')])
    ## inet6-socket-address.h: static ns3::Inet6SocketAddress ns3::Inet6SocketAddress::ConvertFrom(ns3::Address const & addr) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Inet6SocketAddress', 
                   [param('ns3::Address const &', 'addr')], 
                   is_static=True)
    ## inet6-socket-address.h: ns3::Ipv6Address ns3::Inet6SocketAddress::GetIpv6() const [member function]
    cls.add_method('GetIpv6', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## inet6-socket-address.h: uint16_t ns3::Inet6SocketAddress::GetPort() const [member function]
    cls.add_method('GetPort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## inet6-socket-address.h: static bool ns3::Inet6SocketAddress::IsMatchingType(ns3::Address const & addr) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'addr')], 
                   is_static=True)
    ## inet6-socket-address.h: void ns3::Inet6SocketAddress::SetIpv6(ns3::Ipv6Address ipv6) [member function]
    cls.add_method('SetIpv6', 
                   'void', 
                   [param('ns3::Ipv6Address', 'ipv6')])
    ## inet6-socket-address.h: void ns3::Inet6SocketAddress::SetPort(uint16_t port) [member function]
    cls.add_method('SetPort', 
                   'void', 
                   [param('uint16_t', 'port')])
    return

def register_Ns3InetSocketAddress_methods(root_module, cls):
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(ns3::InetSocketAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::InetSocketAddress const &', 'arg0')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(ns3::Ipv4Address ipv4, uint16_t port) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ipv4'), param('uint16_t', 'port')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(ns3::Ipv4Address ipv4) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address', 'ipv4')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(uint16_t port) [constructor]
    cls.add_constructor([param('uint16_t', 'port')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(char const * ipv4, uint16_t port) [constructor]
    cls.add_constructor([param('char const *', 'ipv4'), param('uint16_t', 'port')])
    ## inet-socket-address.h: ns3::InetSocketAddress::InetSocketAddress(char const * ipv4) [constructor]
    cls.add_constructor([param('char const *', 'ipv4')])
    ## inet-socket-address.h: static ns3::InetSocketAddress ns3::InetSocketAddress::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::InetSocketAddress', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## inet-socket-address.h: ns3::Ipv4Address ns3::InetSocketAddress::GetIpv4() const [member function]
    cls.add_method('GetIpv4', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## inet-socket-address.h: uint16_t ns3::InetSocketAddress::GetPort() const [member function]
    cls.add_method('GetPort', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## inet-socket-address.h: static bool ns3::InetSocketAddress::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## inet-socket-address.h: void ns3::InetSocketAddress::SetIpv4(ns3::Ipv4Address address) [member function]
    cls.add_method('SetIpv4', 
                   'void', 
                   [param('ns3::Ipv4Address', 'address')])
    ## inet-socket-address.h: void ns3::InetSocketAddress::SetPort(uint16_t port) [member function]
    cls.add_method('SetPort', 
                   'void', 
                   [param('uint16_t', 'port')])
    return

def register_Ns3Ipv4Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ipv4-address.h: ns3::Ipv4Address::Ipv4Address(ns3::Ipv4Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Address const &', 'arg0')])
    ## ipv4-address.h: ns3::Ipv4Address::Ipv4Address() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4Address::Ipv4Address(uint32_t address) [constructor]
    cls.add_constructor([param('uint32_t', 'address')])
    ## ipv4-address.h: ns3::Ipv4Address::Ipv4Address(char const * address) [constructor]
    cls.add_constructor([param('char const *', 'address')])
    ## ipv4-address.h: ns3::Ipv4Address ns3::Ipv4Address::CombineMask(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('CombineMask', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::Deserialize(uint8_t const * buf) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Ipv4Address', 
                   [param('uint8_t const *', 'buf')], 
                   is_static=True)
    ## ipv4-address.h: uint32_t ns3::Ipv4Address::Get() const [member function]
    cls.add_method('Get', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::GetAny() [member function]
    cls.add_method('GetAny', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: ns3::Ipv4Address ns3::Ipv4Address::GetSubnetDirectedBroadcast(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('GetSubnetDirectedBroadcast', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Address ns3::Ipv4Address::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsEqual(ns3::Ipv4Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv4Address const &', 'other')], 
                   is_const=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsLocalMulticast() const [member function]
    cls.add_method('IsLocalMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: static bool ns3::Ipv4Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: bool ns3::Ipv4Address::IsSubnetDirectedBroadcast(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('IsSubnetDirectedBroadcast', 
                   'bool', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Address::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Address::Serialize(uint8_t * buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Address::Set(uint32_t address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint32_t', 'address')])
    ## ipv4-address.h: void ns3::Ipv4Address::Set(char const * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('char const *', 'address')])
    return

def register_Ns3Ipv4Mask_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv4-address.h: ns3::Ipv4Mask::Ipv4Mask(ns3::Ipv4Mask const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Mask const &', 'arg0')])
    ## ipv4-address.h: ns3::Ipv4Mask::Ipv4Mask() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4Mask::Ipv4Mask(uint32_t mask) [constructor]
    cls.add_constructor([param('uint32_t', 'mask')])
    ## ipv4-address.h: ns3::Ipv4Mask::Ipv4Mask(char const * mask) [constructor]
    cls.add_constructor([param('char const *', 'mask')])
    ## ipv4-address.h: uint32_t ns3::Ipv4Mask::Get() const [member function]
    cls.add_method('Get', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: uint32_t ns3::Ipv4Mask::GetInverse() const [member function]
    cls.add_method('GetInverse', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Mask ns3::Ipv4Mask::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: static ns3::Ipv4Mask ns3::Ipv4Mask::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: uint16_t ns3::Ipv4Mask::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: static ns3::Ipv4Mask ns3::Ipv4Mask::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h: bool ns3::Ipv4Mask::IsEqual(ns3::Ipv4Mask other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv4Mask', 'other')], 
                   is_const=True)
    ## ipv4-address.h: bool ns3::Ipv4Mask::IsMatch(ns3::Ipv4Address a, ns3::Ipv4Address b) const [member function]
    cls.add_method('IsMatch', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'a'), param('ns3::Ipv4Address', 'b')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Mask::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv4-address.h: void ns3::Ipv4Mask::Set(uint32_t mask) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint32_t', 'mask')])
    return

def register_Ns3Ipv6Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address(char const * address) [constructor]
    cls.add_constructor([param('char const *', 'address')])
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address(uint8_t * address) [constructor]
    cls.add_constructor([param('uint8_t *', 'address')])
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address(ns3::Ipv6Address const & addr) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Address const &', 'addr')])
    ## ipv6-address.h: ns3::Ipv6Address::Ipv6Address(ns3::Ipv6Address const * addr) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address const *', 'addr')])
    ## ipv6-address.h: ns3::Ipv6Address ns3::Ipv6Address::CombinePrefix(ns3::Ipv6Prefix const & prefix) [member function]
    cls.add_method('CombinePrefix', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv6Prefix const &', 'prefix')])
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::Deserialize(uint8_t const * buf) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Ipv6Address', 
                   [param('uint8_t const *', 'buf')], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetAllHostsMulticast() [member function]
    cls.add_method('GetAllHostsMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetAllNodesMulticast() [member function]
    cls.add_method('GetAllNodesMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetAllRoutersMulticast() [member function]
    cls.add_method('GetAllRoutersMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetAny() [member function]
    cls.add_method('GetAny', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: void ns3::Ipv6Address::GetBytes(uint8_t * buf) const [member function]
    cls.add_method('GetBytes', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsAllHostsMulticast() const [member function]
    cls.add_method('IsAllHostsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsAllNodesMulticast() const [member function]
    cls.add_method('IsAllNodesMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsAllRoutersMulticast() const [member function]
    cls.add_method('IsAllRoutersMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsAny() const [member function]
    cls.add_method('IsAny', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsEqual(ns3::Ipv6Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Address const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsLinkLocal() const [member function]
    cls.add_method('IsLinkLocal', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsLocalhost() const [member function]
    cls.add_method('IsLocalhost', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: static bool ns3::Ipv6Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Address::IsSolicitedMulticast() const [member function]
    cls.add_method('IsSolicitedMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac48Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac48Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'mac')], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Address ns3::Ipv6Address::MakeSolicitedAddress(ns3::Ipv6Address addr) [member function]
    cls.add_method('MakeSolicitedAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_static=True)
    ## ipv6-address.h: void ns3::Ipv6Address::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv6-address.h: void ns3::Ipv6Address::Serialize(uint8_t * buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h: void ns3::Ipv6Address::Set(char const * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('char const *', 'address')])
    ## ipv6-address.h: void ns3::Ipv6Address::Set(uint8_t * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint8_t *', 'address')])
    return

def register_Ns3Ipv6Prefix_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(uint8_t * prefix) [constructor]
    cls.add_constructor([param('uint8_t *', 'prefix')])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(char const * prefix) [constructor]
    cls.add_constructor([param('char const *', 'prefix')])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(uint8_t prefix) [constructor]
    cls.add_constructor([param('uint8_t', 'prefix')])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(ns3::Ipv6Prefix const & prefix) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const &', 'prefix')])
    ## ipv6-address.h: ns3::Ipv6Prefix::Ipv6Prefix(ns3::Ipv6Prefix const * prefix) [constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const *', 'prefix')])
    ## ipv6-address.h: void ns3::Ipv6Prefix::GetBytes(uint8_t * buf) const [member function]
    cls.add_method('GetBytes', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h: static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: uint8_t ns3::Ipv6Prefix::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h: bool ns3::Ipv6Prefix::IsEqual(ns3::Ipv6Prefix const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Prefix const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h: bool ns3::Ipv6Prefix::IsMatch(ns3::Ipv6Address a, ns3::Ipv6Address b) const [member function]
    cls.add_method('IsMatch', 
                   'bool', 
                   [param('ns3::Ipv6Address', 'a'), param('ns3::Ipv6Address', 'b')], 
                   is_const=True)
    ## ipv6-address.h: void ns3::Ipv6Prefix::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3Mac48Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## mac48-address.h: ns3::Mac48Address::Mac48Address(ns3::Mac48Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48Address const &', 'arg0')])
    ## mac48-address.h: ns3::Mac48Address::Mac48Address() [constructor]
    cls.add_constructor([])
    ## mac48-address.h: ns3::Mac48Address::Mac48Address(char const * str) [constructor]
    cls.add_constructor([param('char const *', 'str')])
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Mac48Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## mac48-address.h: void ns3::Mac48Address::CopyFrom(uint8_t const * buffer) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('uint8_t const *', 'buffer')])
    ## mac48-address.h: void ns3::Mac48Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'void', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetMulticast(ns3::Ipv4Address address) [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Mac48Address', 
                   [param('ns3::Ipv4Address', 'address')], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetMulticast(ns3::Ipv6Address address) [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Mac48Address', 
                   [param('ns3::Ipv6Address', 'address')], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetMulticast6Prefix() [member function]
    cls.add_method('GetMulticast6Prefix', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h: static ns3::Mac48Address ns3::Mac48Address::GetMulticastPrefix() [member function]
    cls.add_method('GetMulticastPrefix', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h: bool ns3::Mac48Address::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac48-address.h: bool ns3::Mac48Address::IsGroup() const [member function]
    cls.add_method('IsGroup', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac48-address.h: static bool ns3::Mac48Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    return

def register_Ns3Mac64Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## mac64-address.h: ns3::Mac64Address::Mac64Address(ns3::Mac64Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac64Address const &', 'arg0')])
    ## mac64-address.h: ns3::Mac64Address::Mac64Address() [constructor]
    cls.add_constructor([])
    ## mac64-address.h: ns3::Mac64Address::Mac64Address(char const * str) [constructor]
    cls.add_constructor([param('char const *', 'str')])
    ## mac64-address.h: static ns3::Mac64Address ns3::Mac64Address::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::Mac64Address', 
                   [], 
                   is_static=True)
    ## mac64-address.h: static ns3::Mac64Address ns3::Mac64Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Mac64Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## mac64-address.h: void ns3::Mac64Address::CopyFrom(uint8_t const * buffer) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('uint8_t const *', 'buffer')])
    ## mac64-address.h: void ns3::Mac64Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'void', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## mac64-address.h: static bool ns3::Mac64Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    return

def register_Ns3NetDeviceContainer_methods(root_module, cls):
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer(ns3::NetDeviceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDeviceContainer const &', 'arg0')])
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer() [constructor]
    cls.add_constructor([])
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer(ns3::Ptr<ns3::NetDevice> dev) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'dev')])
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer(std::string devName) [constructor]
    cls.add_constructor([param('std::string', 'devName')])
    ## net-device-container.h: ns3::NetDeviceContainer::NetDeviceContainer(ns3::NetDeviceContainer const & a, ns3::NetDeviceContainer const & b) [constructor]
    cls.add_constructor([param('ns3::NetDeviceContainer const &', 'a'), param('ns3::NetDeviceContainer const &', 'b')])
    ## net-device-container.h: void ns3::NetDeviceContainer::Add(ns3::NetDeviceContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::NetDeviceContainer', 'other')])
    ## net-device-container.h: void ns3::NetDeviceContainer::Add(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## net-device-container.h: void ns3::NetDeviceContainer::Add(std::string deviceName) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'deviceName')])
    ## net-device-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::NetDevice>*,std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > > ns3::NetDeviceContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::NetDevice > const, std::vector< ns3::Ptr< ns3::NetDevice > > >', 
                   [], 
                   is_const=True)
    ## net-device-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::NetDevice>*,std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > > ns3::NetDeviceContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::NetDevice > const, std::vector< ns3::Ptr< ns3::NetDevice > > >', 
                   [], 
                   is_const=True)
    ## net-device-container.h: ns3::Ptr<ns3::NetDevice> ns3::NetDeviceContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## net-device-container.h: uint32_t ns3::NetDeviceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3NodeContainer_methods(root_module, cls):
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'arg0')])
    ## node-container.h: ns3::NodeContainer::NodeContainer() [constructor]
    cls.add_constructor([])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node')])
    ## node-container.h: ns3::NodeContainer::NodeContainer(std::string nodeName) [constructor]
    cls.add_constructor([param('std::string', 'nodeName')])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b')])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c')])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c, ns3::NodeContainer const & d) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c'), param('ns3::NodeContainer const &', 'd')])
    ## node-container.h: ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c, ns3::NodeContainer const & d, ns3::NodeContainer const & e) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c'), param('ns3::NodeContainer const &', 'd'), param('ns3::NodeContainer const &', 'e')])
    ## node-container.h: void ns3::NodeContainer::Add(ns3::NodeContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::NodeContainer', 'other')])
    ## node-container.h: void ns3::NodeContainer::Add(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## node-container.h: void ns3::NodeContainer::Add(std::string nodeName) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'nodeName')])
    ## node-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h: void ns3::NodeContainer::Create(uint32_t n) [member function]
    cls.add_method('Create', 
                   'void', 
                   [param('uint32_t', 'n')])
    ## node-container.h: void ns3::NodeContainer::Create(uint32_t n, uint32_t systemId) [member function]
    cls.add_method('Create', 
                   'void', 
                   [param('uint32_t', 'n'), param('uint32_t', 'systemId')])
    ## node-container.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h: ns3::Ptr<ns3::Node> ns3::NodeContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## node-container.h: static ns3::NodeContainer ns3::NodeContainer::GetGlobal() [member function]
    cls.add_method('GetGlobal', 
                   'ns3::NodeContainer', 
                   [], 
                   is_static=True)
    ## node-container.h: uint32_t ns3::NodeContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3NodeList_methods(root_module, cls):
    ## node-list.h: ns3::NodeList::NodeList() [constructor]
    cls.add_constructor([])
    ## node-list.h: ns3::NodeList::NodeList(ns3::NodeList const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NodeList const &', 'arg0')])
    ## node-list.h: static uint32_t ns3::NodeList::Add(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Add', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_static=True)
    ## node-list.h: static __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeList::Begin() [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_static=True)
    ## node-list.h: static __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeList::End() [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_static=True)
    ## node-list.h: static uint32_t ns3::NodeList::GetNNodes() [member function]
    cls.add_method('GetNNodes', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## node-list.h: static ns3::Ptr<ns3::Node> ns3::NodeList::GetNode(uint32_t n) [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'n')], 
                   is_static=True)
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

def register_Ns3PacketSocketAddress_methods(root_module, cls):
    ## packet-socket-address.h: ns3::PacketSocketAddress::PacketSocketAddress(ns3::PacketSocketAddress const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSocketAddress const &', 'arg0')])
    ## packet-socket-address.h: ns3::PacketSocketAddress::PacketSocketAddress() [constructor]
    cls.add_constructor([])
    ## packet-socket-address.h: static ns3::PacketSocketAddress ns3::PacketSocketAddress::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::PacketSocketAddress', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## packet-socket-address.h: ns3::Address ns3::PacketSocketAddress::GetPhysicalAddress() const [member function]
    cls.add_method('GetPhysicalAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## packet-socket-address.h: uint16_t ns3::PacketSocketAddress::GetProtocol() const [member function]
    cls.add_method('GetProtocol', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## packet-socket-address.h: uint32_t ns3::PacketSocketAddress::GetSingleDevice() const [member function]
    cls.add_method('GetSingleDevice', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-socket-address.h: static bool ns3::PacketSocketAddress::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## packet-socket-address.h: bool ns3::PacketSocketAddress::IsSingleDevice() const [member function]
    cls.add_method('IsSingleDevice', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet-socket-address.h: void ns3::PacketSocketAddress::SetAllDevices() [member function]
    cls.add_method('SetAllDevices', 
                   'void', 
                   [])
    ## packet-socket-address.h: void ns3::PacketSocketAddress::SetPhysicalAddress(ns3::Address const address) [member function]
    cls.add_method('SetPhysicalAddress', 
                   'void', 
                   [param('ns3::Address const', 'address')])
    ## packet-socket-address.h: void ns3::PacketSocketAddress::SetProtocol(uint16_t protocol) [member function]
    cls.add_method('SetProtocol', 
                   'void', 
                   [param('uint16_t', 'protocol')])
    ## packet-socket-address.h: void ns3::PacketSocketAddress::SetSingleDevice(uint32_t device) [member function]
    cls.add_method('SetSingleDevice', 
                   'void', 
                   [param('uint32_t', 'device')])
    return

def register_Ns3PacketSocketHelper_methods(root_module, cls):
    ## packet-socket-helper.h: ns3::PacketSocketHelper::PacketSocketHelper() [constructor]
    cls.add_constructor([])
    ## packet-socket-helper.h: ns3::PacketSocketHelper::PacketSocketHelper(ns3::PacketSocketHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSocketHelper const &', 'arg0')])
    ## packet-socket-helper.h: void ns3::PacketSocketHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## packet-socket-helper.h: void ns3::PacketSocketHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## packet-socket-helper.h: void ns3::PacketSocketHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
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

def register_Ns3PbbAddressTlvBlock_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbAddressTlvBlock::PbbAddressTlvBlock(ns3::PbbAddressTlvBlock const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressTlvBlock const &', 'arg0')])
    ## packetbb.h: ns3::PbbAddressTlvBlock::PbbAddressTlvBlock() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> ns3::PbbAddressTlvBlock::Back() const [member function]
    cls.add_method('Back', 
                   'ns3::Ptr< ns3::PbbAddressTlv >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Begin() [member function]
    cls.add_method('Begin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Begin() const [member function]
    cls.add_method('Begin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: bool ns3::PbbAddressTlvBlock::Empty() const [member function]
    cls.add_method('Empty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::End() [member function]
    cls.add_method('End', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::End() const [member function]
    cls.add_method('End', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > position) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > last) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> ns3::PbbAddressTlvBlock::Front() const [member function]
    cls.add_method('Front', 
                   'ns3::Ptr< ns3::PbbAddressTlv >', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint32_t ns3::PbbAddressTlvBlock::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressTlvBlock::Insert(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > position, ns3::Ptr<ns3::PbbAddressTlv> const tlv) [member function]
    cls.add_method('Insert', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'position'), param('ns3::Ptr< ns3::PbbAddressTlv > const', 'tlv')])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::PopBack() [member function]
    cls.add_method('PopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::PopFront() [member function]
    cls.add_method('PopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressTlvBlock::PushBack(ns3::Ptr<ns3::PbbAddressTlv> tlv) [member function]
    cls.add_method('PushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::PushFront(ns3::Ptr<ns3::PbbAddressTlv> tlv) [member function]
    cls.add_method('PushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbAddressTlvBlock::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: int ns3::PbbAddressTlvBlock::Size() const [member function]
    cls.add_method('Size', 
                   'int', 
                   [], 
                   is_const=True)
    return

def register_Ns3PbbTlvBlock_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbTlvBlock::PbbTlvBlock(ns3::PbbTlvBlock const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbTlvBlock const &', 'arg0')])
    ## packetbb.h: ns3::PbbTlvBlock::PbbTlvBlock() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbTlvBlock::Back() const [member function]
    cls.add_method('Back', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Begin() [member function]
    cls.add_method('Begin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Begin() const [member function]
    cls.add_method('Begin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlvBlock::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbTlvBlock::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: bool ns3::PbbTlvBlock::Empty() const [member function]
    cls.add_method('Empty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::End() [member function]
    cls.add_method('End', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::End() const [member function]
    cls.add_method('End', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > position) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > last) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbTlvBlock::Front() const [member function]
    cls.add_method('Front', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint32_t ns3::PbbTlvBlock::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbTlvBlock::Insert(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > position, ns3::Ptr<ns3::PbbTlv> const tlv) [member function]
    cls.add_method('Insert', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'position'), param('ns3::Ptr< ns3::PbbTlv > const', 'tlv')])
    ## packetbb.h: void ns3::PbbTlvBlock::PopBack() [member function]
    cls.add_method('PopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbTlvBlock::PopFront() [member function]
    cls.add_method('PopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbTlvBlock::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlvBlock::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlvBlock::PushBack(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('PushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbTlvBlock::PushFront(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('PushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbTlvBlock::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: int ns3::PbbTlvBlock::Size() const [member function]
    cls.add_method('Size', 
                   'int', 
                   [], 
                   is_const=True)
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

def register_Ns3PcapHelper_methods(root_module, cls):
    ## trace-helper.h: ns3::PcapHelper::PcapHelper(ns3::PcapHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PcapHelper const &', 'arg0')])
    ## trace-helper.h: ns3::PcapHelper::PcapHelper() [constructor]
    cls.add_constructor([])
    ## trace-helper.h: ns3::Ptr<ns3::PcapFileWrapper> ns3::PcapHelper::CreateFile(std::string filename, std::_Ios_Openmode filemode, uint32_t dataLinkType, uint32_t snapLen=65535, int32_t tzCorrection=0) [member function]
    cls.add_method('CreateFile', 
                   'ns3::Ptr< ns3::PcapFileWrapper >', 
                   [param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode'), param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='65535'), param('int32_t', 'tzCorrection', default_value='0')])
    ## trace-helper.h: std::string ns3::PcapHelper::GetFilenameFromDevice(std::string prefix, ns3::Ptr<ns3::NetDevice> device, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromDevice', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'useObjectNames', default_value='true')])
    ## trace-helper.h: std::string ns3::PcapHelper::GetFilenameFromInterfacePair(std::string prefix, ns3::Ptr<ns3::Object> object, uint32_t interface, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromInterfacePair', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Object >', 'object'), param('uint32_t', 'interface'), param('bool', 'useObjectNames', default_value='true')])
    return

def register_Ns3PcapHelperForDevice_methods(root_module, cls):
    ## trace-helper.h: ns3::PcapHelperForDevice::PcapHelperForDevice(ns3::PcapHelperForDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PcapHelperForDevice const &', 'arg0')])
    ## trace-helper.h: ns3::PcapHelperForDevice::PcapHelperForDevice() [constructor]
    cls.add_constructor([])
    ## trace-helper.h: void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous=false, bool explicitFilename=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous', default_value='false'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h: void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, std::string ndName, bool promiscuous=false, bool explicitFilename=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('std::string', 'ndName'), param('bool', 'promiscuous', default_value='false'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h: void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::NetDeviceContainer d, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NetDeviceContainer', 'd'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h: void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::NodeContainer n, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NodeContainer', 'n'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h: void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, uint32_t nodeid, uint32_t deviceid, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h: void ns3::PcapHelperForDevice::EnablePcapAll(std::string prefix, bool promiscuous=false) [member function]
    cls.add_method('EnablePcapAll', 
                   'void', 
                   [param('std::string', 'prefix'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h: void ns3::PcapHelperForDevice::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   is_pure_virtual=True, is_virtual=True)
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

def register_Ns3FlowIdTag_methods(root_module, cls):
    ## flow-id-tag.h: ns3::FlowIdTag::FlowIdTag(ns3::FlowIdTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowIdTag const &', 'arg0')])
    ## flow-id-tag.h: ns3::FlowIdTag::FlowIdTag() [constructor]
    cls.add_constructor([])
    ## flow-id-tag.h: ns3::FlowIdTag::FlowIdTag(uint32_t flowId) [constructor]
    cls.add_constructor([param('uint32_t', 'flowId')])
    ## flow-id-tag.h: static uint32_t ns3::FlowIdTag::AllocateFlowId() [member function]
    cls.add_method('AllocateFlowId', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## flow-id-tag.h: void ns3::FlowIdTag::Deserialize(ns3::TagBuffer buf) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buf')], 
                   is_virtual=True)
    ## flow-id-tag.h: uint32_t ns3::FlowIdTag::GetFlowId() const [member function]
    cls.add_method('GetFlowId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## flow-id-tag.h: ns3::TypeId ns3::FlowIdTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flow-id-tag.h: uint32_t ns3::FlowIdTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flow-id-tag.h: static ns3::TypeId ns3::FlowIdTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flow-id-tag.h: void ns3::FlowIdTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## flow-id-tag.h: void ns3::FlowIdTag::Serialize(ns3::TagBuffer buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buf')], 
                   is_const=True, is_virtual=True)
    ## flow-id-tag.h: void ns3::FlowIdTag::SetFlowId(uint32_t flowId) [member function]
    cls.add_method('SetFlowId', 
                   'void', 
                   [param('uint32_t', 'flowId')])
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

def register_Ns3LlcSnapHeader_methods(root_module, cls):
    ## llc-snap-header.h: ns3::LlcSnapHeader::LlcSnapHeader(ns3::LlcSnapHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LlcSnapHeader const &', 'arg0')])
    ## llc-snap-header.h: ns3::LlcSnapHeader::LlcSnapHeader() [constructor]
    cls.add_constructor([])
    ## llc-snap-header.h: uint32_t ns3::LlcSnapHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## llc-snap-header.h: ns3::TypeId ns3::LlcSnapHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## llc-snap-header.h: uint32_t ns3::LlcSnapHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## llc-snap-header.h: uint16_t ns3::LlcSnapHeader::GetType() [member function]
    cls.add_method('GetType', 
                   'uint16_t', 
                   [])
    ## llc-snap-header.h: static ns3::TypeId ns3::LlcSnapHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## llc-snap-header.h: void ns3::LlcSnapHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## llc-snap-header.h: void ns3::LlcSnapHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## llc-snap-header.h: void ns3::LlcSnapHeader::SetType(uint16_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint16_t', 'type')])
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

def register_Ns3Queue_methods(root_module, cls):
    ## queue.h: ns3::Queue::Queue(ns3::Queue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Queue const &', 'arg0')])
    ## queue.h: ns3::Queue::Queue() [constructor]
    cls.add_constructor([])
    ## queue.h: ns3::Ptr<ns3::Packet> ns3::Queue::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## queue.h: void ns3::Queue::DequeueAll() [member function]
    cls.add_method('DequeueAll', 
                   'void', 
                   [])
    ## queue.h: bool ns3::Queue::Enqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## queue.h: uint32_t ns3::Queue::GetNBytes() const [member function]
    cls.add_method('GetNBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetNPackets() const [member function]
    cls.add_method('GetNPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetTotalDroppedBytes() const [member function]
    cls.add_method('GetTotalDroppedBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetTotalDroppedPackets() const [member function]
    cls.add_method('GetTotalDroppedPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetTotalReceivedBytes() const [member function]
    cls.add_method('GetTotalReceivedBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: uint32_t ns3::Queue::GetTotalReceivedPackets() const [member function]
    cls.add_method('GetTotalReceivedPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h: static ns3::TypeId ns3::Queue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## queue.h: bool ns3::Queue::IsEmpty() const [member function]
    cls.add_method('IsEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## queue.h: ns3::Ptr<ns3::Packet const> ns3::Queue::Peek() const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## queue.h: void ns3::Queue::ResetStatistics() [member function]
    cls.add_method('ResetStatistics', 
                   'void', 
                   [])
    ## queue.h: void ns3::Queue::Drop(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('Drop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')], 
                   visibility='protected')
    ## queue.h: ns3::Ptr<ns3::Packet> ns3::Queue::DoDequeue() [member function]
    cls.add_method('DoDequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## queue.h: bool ns3::Queue::DoEnqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoEnqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## queue.h: ns3::Ptr<ns3::Packet const> ns3::Queue::DoPeek() const [member function]
    cls.add_method('DoPeek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RadiotapHeader_methods(root_module, cls):
    ## radiotap-header.h: ns3::RadiotapHeader::RadiotapHeader(ns3::RadiotapHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RadiotapHeader const &', 'arg0')])
    ## radiotap-header.h: ns3::RadiotapHeader::RadiotapHeader() [constructor]
    cls.add_constructor([])
    ## radiotap-header.h: uint32_t ns3::RadiotapHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## radiotap-header.h: uint8_t ns3::RadiotapHeader::GetAntennaNoisePower() const [member function]
    cls.add_method('GetAntennaNoisePower', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint8_t ns3::RadiotapHeader::GetAntennaSignalPower() const [member function]
    cls.add_method('GetAntennaSignalPower', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint16_t ns3::RadiotapHeader::GetChannelFlags() const [member function]
    cls.add_method('GetChannelFlags', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint16_t ns3::RadiotapHeader::GetChannelFrequency() const [member function]
    cls.add_method('GetChannelFrequency', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint8_t ns3::RadiotapHeader::GetFrameFlags() const [member function]
    cls.add_method('GetFrameFlags', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: ns3::TypeId ns3::RadiotapHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## radiotap-header.h: uint8_t ns3::RadiotapHeader::GetRate() const [member function]
    cls.add_method('GetRate', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: uint32_t ns3::RadiotapHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## radiotap-header.h: uint64_t ns3::RadiotapHeader::GetTsft() const [member function]
    cls.add_method('GetTsft', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## radiotap-header.h: static ns3::TypeId ns3::RadiotapHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## radiotap-header.h: void ns3::RadiotapHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## radiotap-header.h: void ns3::RadiotapHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## radiotap-header.h: void ns3::RadiotapHeader::SetAntennaNoisePower(double noise) [member function]
    cls.add_method('SetAntennaNoisePower', 
                   'void', 
                   [param('double', 'noise')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetAntennaSignalPower(double signal) [member function]
    cls.add_method('SetAntennaSignalPower', 
                   'void', 
                   [param('double', 'signal')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetChannelFrequencyAndFlags(uint16_t frequency, uint16_t flags) [member function]
    cls.add_method('SetChannelFrequencyAndFlags', 
                   'void', 
                   [param('uint16_t', 'frequency'), param('uint16_t', 'flags')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetFrameFlags(uint8_t flags) [member function]
    cls.add_method('SetFrameFlags', 
                   'void', 
                   [param('uint8_t', 'flags')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetRate(uint8_t rate) [member function]
    cls.add_method('SetRate', 
                   'void', 
                   [param('uint8_t', 'rate')])
    ## radiotap-header.h: void ns3::RadiotapHeader::SetTsft(uint64_t tsft) [member function]
    cls.add_method('SetTsft', 
                   'void', 
                   [param('uint64_t', 'tsft')])
    return

def register_Ns3Socket_methods(root_module, cls):
    ## socket.h: ns3::Socket::Socket(ns3::Socket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Socket const &', 'arg0')])
    ## socket.h: ns3::Socket::Socket() [constructor]
    cls.add_constructor([])
    ## socket.h: int ns3::Socket::Bind(ns3::Address const & address) [member function]
    cls.add_method('Bind', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::Bind() [member function]
    cls.add_method('Bind', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: void ns3::Socket::BindToNetDevice(ns3::Ptr<ns3::NetDevice> netdevice) [member function]
    cls.add_method('BindToNetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'netdevice')], 
                   is_virtual=True)
    ## socket.h: int ns3::Socket::Close() [member function]
    cls.add_method('Close', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::Connect(ns3::Address const & address) [member function]
    cls.add_method('Connect', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: static ns3::Ptr<ns3::Socket> ns3::Socket::CreateSocket(ns3::Ptr<ns3::Node> node, ns3::TypeId tid) [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::TypeId', 'tid')], 
                   is_static=True)
    ## socket.h: bool ns3::Socket::GetAllowBroadcast() const [member function]
    cls.add_method('GetAllowBroadcast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::NetDevice> ns3::Socket::GetBoundNetDevice() [member function]
    cls.add_method('GetBoundNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## socket.h: ns3::Socket::SocketErrno ns3::Socket::GetErrno() const [member function]
    cls.add_method('GetErrno', 
                   'ns3::Socket::SocketErrno', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::Node> ns3::Socket::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::Socket::GetRxAvailable() const [member function]
    cls.add_method('GetRxAvailable', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: int ns3::Socket::GetSockName(ns3::Address & address) const [member function]
    cls.add_method('GetSockName', 
                   'int', 
                   [param('ns3::Address &', 'address')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: ns3::Socket::SocketType ns3::Socket::GetSocketType() const [member function]
    cls.add_method('GetSocketType', 
                   'ns3::Socket::SocketType', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::Socket::GetTxAvailable() const [member function]
    cls.add_method('GetTxAvailable', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h: int ns3::Socket::Listen() [member function]
    cls.add_method('Listen', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::Packet> ns3::Socket::Recv(uint32_t maxSize, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::Packet> ns3::Socket::Recv() [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## socket.h: int ns3::Socket::Recv(uint8_t * buf, uint32_t size, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'int', 
                   [param('uint8_t *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags')])
    ## socket.h: ns3::Ptr<ns3::Packet> ns3::Socket::RecvFrom(uint32_t maxSize, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: ns3::Ptr<ns3::Packet> ns3::Socket::RecvFrom(ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Address &', 'fromAddress')])
    ## socket.h: int ns3::Socket::RecvFrom(uint8_t * buf, uint32_t size, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'int', 
                   [param('uint8_t *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')])
    ## socket.h: int ns3::Socket::Send(ns3::Ptr<ns3::Packet> p, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## socket.h: int ns3::Socket::Send(uint8_t const * buf, uint32_t size, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('uint8_t const *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags')])
    ## socket.h: int ns3::Socket::SendTo(ns3::Ptr<ns3::Packet> p, uint32_t flags, ns3::Address const & toAddress) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags'), param('ns3::Address const &', 'toAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::SendTo(uint8_t const * buf, uint32_t size, uint32_t flags, ns3::Address const & address) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('uint8_t const *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags'), param('ns3::Address const &', 'address')])
    ## socket.h: void ns3::Socket::SetAcceptCallback(ns3::Callback<bool, ns3::Ptr<ns3::Socket>, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionRequest, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> newConnectionCreated) [member function]
    cls.add_method('SetAcceptCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::Socket >, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionRequest'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'newConnectionCreated')])
    ## socket.h: bool ns3::Socket::SetAllowBroadcast(bool allowBroadcast) [member function]
    cls.add_method('SetAllowBroadcast', 
                   'bool', 
                   [param('bool', 'allowBroadcast')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: void ns3::Socket::SetCloseCallbacks(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> normalClose, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> errorClose) [member function]
    cls.add_method('SetCloseCallbacks', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'normalClose'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'errorClose')])
    ## socket.h: void ns3::Socket::SetConnectCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionSucceeded, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionFailed) [member function]
    cls.add_method('SetConnectCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionSucceeded'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionFailed')])
    ## socket.h: void ns3::Socket::SetDataSentCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> dataSent) [member function]
    cls.add_method('SetDataSentCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'dataSent')])
    ## socket.h: void ns3::Socket::SetRecvCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> arg0) [member function]
    cls.add_method('SetRecvCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'arg0')])
    ## socket.h: void ns3::Socket::SetRecvPktInfo(bool flag) [member function]
    cls.add_method('SetRecvPktInfo', 
                   'void', 
                   [param('bool', 'flag')])
    ## socket.h: void ns3::Socket::SetSendCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> sendCb) [member function]
    cls.add_method('SetSendCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'sendCb')])
    ## socket.h: int ns3::Socket::ShutdownRecv() [member function]
    cls.add_method('ShutdownRecv', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: int ns3::Socket::ShutdownSend() [member function]
    cls.add_method('ShutdownSend', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h: void ns3::Socket::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## socket.h: void ns3::Socket::NotifyConnectionFailed() [member function]
    cls.add_method('NotifyConnectionFailed', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: bool ns3::Socket::NotifyConnectionRequest(ns3::Address const & from) [member function]
    cls.add_method('NotifyConnectionRequest', 
                   'bool', 
                   [param('ns3::Address const &', 'from')], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyConnectionSucceeded() [member function]
    cls.add_method('NotifyConnectionSucceeded', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyDataRecv() [member function]
    cls.add_method('NotifyDataRecv', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyDataSent(uint32_t size) [member function]
    cls.add_method('NotifyDataSent', 
                   'void', 
                   [param('uint32_t', 'size')], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyErrorClose() [member function]
    cls.add_method('NotifyErrorClose', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyNewConnectionCreated(ns3::Ptr<ns3::Socket> socket, ns3::Address const & from) [member function]
    cls.add_method('NotifyNewConnectionCreated', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket'), param('ns3::Address const &', 'from')], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifyNormalClose() [member function]
    cls.add_method('NotifyNormalClose', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h: void ns3::Socket::NotifySend(uint32_t spaceAvailable) [member function]
    cls.add_method('NotifySend', 
                   'void', 
                   [param('uint32_t', 'spaceAvailable')], 
                   visibility='protected')
    return

def register_Ns3SocketAddressTag_methods(root_module, cls):
    ## socket.h: ns3::SocketAddressTag::SocketAddressTag(ns3::SocketAddressTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketAddressTag const &', 'arg0')])
    ## socket.h: ns3::SocketAddressTag::SocketAddressTag() [constructor]
    cls.add_constructor([])
    ## socket.h: void ns3::SocketAddressTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h: ns3::Address ns3::SocketAddressTag::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## socket.h: ns3::TypeId ns3::SocketAddressTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::SocketAddressTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: static ns3::TypeId ns3::SocketAddressTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h: void ns3::SocketAddressTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketAddressTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketAddressTag::SetAddress(ns3::Address addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'addr')])
    return

def register_Ns3SocketFactory_methods(root_module, cls):
    ## socket-factory.h: ns3::SocketFactory::SocketFactory(ns3::SocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketFactory const &', 'arg0')])
    ## socket-factory.h: ns3::SocketFactory::SocketFactory() [constructor]
    cls.add_constructor([])
    ## socket-factory.h: ns3::Ptr<ns3::Socket> ns3::SocketFactory::CreateSocket() [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket-factory.h: static ns3::TypeId ns3::SocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3SocketIpTtlTag_methods(root_module, cls):
    ## socket.h: ns3::SocketIpTtlTag::SocketIpTtlTag(ns3::SocketIpTtlTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketIpTtlTag const &', 'arg0')])
    ## socket.h: ns3::SocketIpTtlTag::SocketIpTtlTag() [constructor]
    cls.add_constructor([])
    ## socket.h: void ns3::SocketIpTtlTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h: ns3::TypeId ns3::SocketIpTtlTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::SocketIpTtlTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: uint8_t ns3::SocketIpTtlTag::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## socket.h: static ns3::TypeId ns3::SocketIpTtlTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h: void ns3::SocketIpTtlTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketIpTtlTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketIpTtlTag::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3SocketSetDontFragmentTag_methods(root_module, cls):
    ## socket.h: ns3::SocketSetDontFragmentTag::SocketSetDontFragmentTag(ns3::SocketSetDontFragmentTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketSetDontFragmentTag const &', 'arg0')])
    ## socket.h: ns3::SocketSetDontFragmentTag::SocketSetDontFragmentTag() [constructor]
    cls.add_constructor([])
    ## socket.h: void ns3::SocketSetDontFragmentTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h: void ns3::SocketSetDontFragmentTag::Disable() [member function]
    cls.add_method('Disable', 
                   'void', 
                   [])
    ## socket.h: void ns3::SocketSetDontFragmentTag::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [])
    ## socket.h: ns3::TypeId ns3::SocketSetDontFragmentTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: uint32_t ns3::SocketSetDontFragmentTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h: static ns3::TypeId ns3::SocketSetDontFragmentTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h: bool ns3::SocketSetDontFragmentTag::IsEnabled() const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## socket.h: void ns3::SocketSetDontFragmentTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h: void ns3::SocketSetDontFragmentTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
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

def register_Ns3Application_methods(root_module, cls):
    ## application.h: ns3::Application::Application(ns3::Application const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Application const &', 'arg0')])
    ## application.h: ns3::Application::Application() [constructor]
    cls.add_constructor([])
    ## application.h: ns3::Ptr<ns3::Node> ns3::Application::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## application.h: static ns3::TypeId ns3::Application::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## application.h: void ns3::Application::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## application.h: void ns3::Application::SetStartTime(ns3::Time start) [member function]
    cls.add_method('SetStartTime', 
                   'void', 
                   [param('ns3::Time', 'start')])
    ## application.h: void ns3::Application::SetStopTime(ns3::Time stop) [member function]
    cls.add_method('SetStopTime', 
                   'void', 
                   [param('ns3::Time', 'stop')])
    ## application.h: void ns3::Application::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## application.h: void ns3::Application::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## application.h: void ns3::Application::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## application.h: void ns3::Application::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3Channel_methods(root_module, cls):
    ## channel.h: ns3::Channel::Channel(ns3::Channel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Channel const &', 'arg0')])
    ## channel.h: ns3::Channel::Channel() [constructor]
    cls.add_constructor([])
    ## channel.h: ns3::Ptr<ns3::NetDevice> ns3::Channel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## channel.h: uint32_t ns3::Channel::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## channel.h: uint32_t ns3::Channel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## channel.h: static ns3::TypeId ns3::Channel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
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

def register_Ns3DropTailQueue_methods(root_module, cls):
    ## drop-tail-queue.h: ns3::DropTailQueue::DropTailQueue(ns3::DropTailQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DropTailQueue const &', 'arg0')])
    ## drop-tail-queue.h: ns3::DropTailQueue::DropTailQueue() [constructor]
    cls.add_constructor([])
    ## drop-tail-queue.h: ns3::DropTailQueue::Mode ns3::DropTailQueue::GetMode() [member function]
    cls.add_method('GetMode', 
                   'ns3::DropTailQueue::Mode', 
                   [])
    ## drop-tail-queue.h: static ns3::TypeId ns3::DropTailQueue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## drop-tail-queue.h: void ns3::DropTailQueue::SetMode(ns3::DropTailQueue::Mode mode) [member function]
    cls.add_method('SetMode', 
                   'void', 
                   [param('ns3::DropTailQueue::Mode', 'mode')])
    ## drop-tail-queue.h: ns3::Ptr<ns3::Packet> ns3::DropTailQueue::DoDequeue() [member function]
    cls.add_method('DoDequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   visibility='private', is_virtual=True)
    ## drop-tail-queue.h: bool ns3::DropTailQueue::DoEnqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoEnqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## drop-tail-queue.h: ns3::Ptr<ns3::Packet const> ns3::DropTailQueue::DoPeek() const [member function]
    cls.add_method('DoPeek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
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

def register_Ns3EthernetHeader_methods(root_module, cls):
    ## ethernet-header.h: ns3::EthernetHeader::EthernetHeader(ns3::EthernetHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EthernetHeader const &', 'arg0')])
    ## ethernet-header.h: ns3::EthernetHeader::EthernetHeader(bool hasPreamble) [constructor]
    cls.add_constructor([param('bool', 'hasPreamble')])
    ## ethernet-header.h: ns3::EthernetHeader::EthernetHeader() [constructor]
    cls.add_constructor([])
    ## ethernet-header.h: uint32_t ns3::EthernetHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ethernet-header.h: ns3::Mac48Address ns3::EthernetHeader::GetDestination() const [member function]
    cls.add_method('GetDestination', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: uint32_t ns3::EthernetHeader::GetHeaderSize() const [member function]
    cls.add_method('GetHeaderSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: ns3::TypeId ns3::EthernetHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ethernet-header.h: uint16_t ns3::EthernetHeader::GetLengthType() const [member function]
    cls.add_method('GetLengthType', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: ns3::ethernet_header_t ns3::EthernetHeader::GetPacketType() const [member function]
    cls.add_method('GetPacketType', 
                   'ns3::ethernet_header_t', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: uint64_t ns3::EthernetHeader::GetPreambleSfd() const [member function]
    cls.add_method('GetPreambleSfd', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: uint32_t ns3::EthernetHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ethernet-header.h: ns3::Mac48Address ns3::EthernetHeader::GetSource() const [member function]
    cls.add_method('GetSource', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## ethernet-header.h: static ns3::TypeId ns3::EthernetHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ethernet-header.h: void ns3::EthernetHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ethernet-header.h: void ns3::EthernetHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ethernet-header.h: void ns3::EthernetHeader::SetDestination(ns3::Mac48Address destination) [member function]
    cls.add_method('SetDestination', 
                   'void', 
                   [param('ns3::Mac48Address', 'destination')])
    ## ethernet-header.h: void ns3::EthernetHeader::SetLengthType(uint16_t size) [member function]
    cls.add_method('SetLengthType', 
                   'void', 
                   [param('uint16_t', 'size')])
    ## ethernet-header.h: void ns3::EthernetHeader::SetPreambleSfd(uint64_t preambleSfd) [member function]
    cls.add_method('SetPreambleSfd', 
                   'void', 
                   [param('uint64_t', 'preambleSfd')])
    ## ethernet-header.h: void ns3::EthernetHeader::SetSource(ns3::Mac48Address source) [member function]
    cls.add_method('SetSource', 
                   'void', 
                   [param('ns3::Mac48Address', 'source')])
    return

def register_Ns3EthernetTrailer_methods(root_module, cls):
    ## ethernet-trailer.h: ns3::EthernetTrailer::EthernetTrailer(ns3::EthernetTrailer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EthernetTrailer const &', 'arg0')])
    ## ethernet-trailer.h: ns3::EthernetTrailer::EthernetTrailer() [constructor]
    cls.add_constructor([])
    ## ethernet-trailer.h: void ns3::EthernetTrailer::CalcFcs(ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('CalcFcs', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## ethernet-trailer.h: bool ns3::EthernetTrailer::CheckFcs(ns3::Ptr<ns3::Packet const> p) const [member function]
    cls.add_method('CheckFcs', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_const=True)
    ## ethernet-trailer.h: uint32_t ns3::EthernetTrailer::Deserialize(ns3::Buffer::Iterator end) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'end')], 
                   is_virtual=True)
    ## ethernet-trailer.h: void ns3::EthernetTrailer::EnableFcs(bool enable) [member function]
    cls.add_method('EnableFcs', 
                   'void', 
                   [param('bool', 'enable')])
    ## ethernet-trailer.h: uint32_t ns3::EthernetTrailer::GetFcs() [member function]
    cls.add_method('GetFcs', 
                   'uint32_t', 
                   [])
    ## ethernet-trailer.h: ns3::TypeId ns3::EthernetTrailer::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ethernet-trailer.h: uint32_t ns3::EthernetTrailer::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ethernet-trailer.h: uint32_t ns3::EthernetTrailer::GetTrailerSize() const [member function]
    cls.add_method('GetTrailerSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ethernet-trailer.h: static ns3::TypeId ns3::EthernetTrailer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ethernet-trailer.h: void ns3::EthernetTrailer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ethernet-trailer.h: void ns3::EthernetTrailer::Serialize(ns3::Buffer::Iterator end) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'end')], 
                   is_const=True, is_virtual=True)
    ## ethernet-trailer.h: void ns3::EthernetTrailer::SetFcs(uint32_t fcs) [member function]
    cls.add_method('SetFcs', 
                   'void', 
                   [param('uint32_t', 'fcs')])
    return

def register_Ns3Ipv4AddressChecker_methods(root_module, cls):
    ## ipv4-address.h: ns3::Ipv4AddressChecker::Ipv4AddressChecker() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4AddressChecker::Ipv4AddressChecker(ns3::Ipv4AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressChecker const &', 'arg0')])
    return

def register_Ns3Ipv4AddressValue_methods(root_module, cls):
    ## ipv4-address.h: ns3::Ipv4AddressValue::Ipv4AddressValue() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4AddressValue::Ipv4AddressValue(ns3::Ipv4AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressValue const &', 'arg0')])
    ## ipv4-address.h: ns3::Ipv4AddressValue::Ipv4AddressValue(ns3::Ipv4Address const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address const &', 'value')])
    ## ipv4-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Ipv4AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h: bool ns3::Ipv4AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv4-address.h: ns3::Ipv4Address ns3::Ipv4AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: std::string ns3::Ipv4AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h: void ns3::Ipv4AddressValue::Set(ns3::Ipv4Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'value')])
    return

def register_Ns3Ipv4MaskChecker_methods(root_module, cls):
    ## ipv4-address.h: ns3::Ipv4MaskChecker::Ipv4MaskChecker() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4MaskChecker::Ipv4MaskChecker(ns3::Ipv4MaskChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MaskChecker const &', 'arg0')])
    return

def register_Ns3Ipv4MaskValue_methods(root_module, cls):
    ## ipv4-address.h: ns3::Ipv4MaskValue::Ipv4MaskValue() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h: ns3::Ipv4MaskValue::Ipv4MaskValue(ns3::Ipv4MaskValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MaskValue const &', 'arg0')])
    ## ipv4-address.h: ns3::Ipv4MaskValue::Ipv4MaskValue(ns3::Ipv4Mask const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv4Mask const &', 'value')])
    ## ipv4-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Ipv4MaskValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h: bool ns3::Ipv4MaskValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv4-address.h: ns3::Ipv4Mask ns3::Ipv4MaskValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-address.h: std::string ns3::Ipv4MaskValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h: void ns3::Ipv4MaskValue::Set(ns3::Ipv4Mask const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv4Mask const &', 'value')])
    return

def register_Ns3Ipv6AddressChecker_methods(root_module, cls):
    ## ipv6-address.h: ns3::Ipv6AddressChecker::Ipv6AddressChecker() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6AddressChecker::Ipv6AddressChecker(ns3::Ipv6AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6AddressChecker const &', 'arg0')])
    return

def register_Ns3Ipv6AddressValue_methods(root_module, cls):
    ## ipv6-address.h: ns3::Ipv6AddressValue::Ipv6AddressValue() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6AddressValue::Ipv6AddressValue(ns3::Ipv6AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6AddressValue const &', 'arg0')])
    ## ipv6-address.h: ns3::Ipv6AddressValue::Ipv6AddressValue(ns3::Ipv6Address const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address const &', 'value')])
    ## ipv6-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Ipv6AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h: bool ns3::Ipv6AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv6-address.h: ns3::Ipv6Address ns3::Ipv6AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: std::string ns3::Ipv6AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h: void ns3::Ipv6AddressValue::Set(ns3::Ipv6Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv6Address const &', 'value')])
    return

def register_Ns3Ipv6PrefixChecker_methods(root_module, cls):
    ## ipv6-address.h: ns3::Ipv6PrefixChecker::Ipv6PrefixChecker() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6PrefixChecker::Ipv6PrefixChecker(ns3::Ipv6PrefixChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PrefixChecker const &', 'arg0')])
    return

def register_Ns3Ipv6PrefixValue_methods(root_module, cls):
    ## ipv6-address.h: ns3::Ipv6PrefixValue::Ipv6PrefixValue() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h: ns3::Ipv6PrefixValue::Ipv6PrefixValue(ns3::Ipv6PrefixValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PrefixValue const &', 'arg0')])
    ## ipv6-address.h: ns3::Ipv6PrefixValue::Ipv6PrefixValue(ns3::Ipv6Prefix const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const &', 'value')])
    ## ipv6-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Ipv6PrefixValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h: bool ns3::Ipv6PrefixValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv6-address.h: ns3::Ipv6Prefix ns3::Ipv6PrefixValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_const=True)
    ## ipv6-address.h: std::string ns3::Ipv6PrefixValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h: void ns3::Ipv6PrefixValue::Set(ns3::Ipv6Prefix const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv6Prefix const &', 'value')])
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

def register_Ns3Mac48AddressChecker_methods(root_module, cls):
    ## mac48-address.h: ns3::Mac48AddressChecker::Mac48AddressChecker() [constructor]
    cls.add_constructor([])
    ## mac48-address.h: ns3::Mac48AddressChecker::Mac48AddressChecker(ns3::Mac48AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48AddressChecker const &', 'arg0')])
    return

def register_Ns3Mac48AddressValue_methods(root_module, cls):
    ## mac48-address.h: ns3::Mac48AddressValue::Mac48AddressValue() [constructor]
    cls.add_constructor([])
    ## mac48-address.h: ns3::Mac48AddressValue::Mac48AddressValue(ns3::Mac48AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48AddressValue const &', 'arg0')])
    ## mac48-address.h: ns3::Mac48AddressValue::Mac48AddressValue(ns3::Mac48Address const & value) [constructor]
    cls.add_constructor([param('ns3::Mac48Address const &', 'value')])
    ## mac48-address.h: ns3::Ptr<ns3::AttributeValue> ns3::Mac48AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac48-address.h: bool ns3::Mac48AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## mac48-address.h: ns3::Mac48Address ns3::Mac48AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mac48-address.h: std::string ns3::Mac48AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## mac48-address.h: void ns3::Mac48AddressValue::Set(ns3::Mac48Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Mac48Address const &', 'value')])
    return

def register_Ns3NetDevice_methods(root_module, cls):
    ## net-device.h: ns3::NetDevice::NetDevice() [constructor]
    cls.add_constructor([])
    ## net-device.h: ns3::NetDevice::NetDevice(ns3::NetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDevice const &', 'arg0')])
    ## net-device.h: void ns3::NetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: ns3::Address ns3::NetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Address ns3::NetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Ptr<ns3::Channel> ns3::NetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: uint32_t ns3::NetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: uint16_t ns3::NetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Address ns3::NetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Address ns3::NetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: ns3::Ptr<ns3::Node> ns3::NetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: static ns3::TypeId ns3::NetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## net-device.h: bool ns3::NetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: void ns3::NetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h: bool ns3::NetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
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

def register_Ns3Node_methods(root_module, cls):
    ## node.h: ns3::Node::Node(ns3::Node const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Node const &', 'arg0')])
    ## node.h: ns3::Node::Node() [constructor]
    cls.add_constructor([])
    ## node.h: ns3::Node::Node(uint32_t systemId) [constructor]
    cls.add_constructor([param('uint32_t', 'systemId')])
    ## node.h: uint32_t ns3::Node::AddApplication(ns3::Ptr<ns3::Application> application) [member function]
    cls.add_method('AddApplication', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Application >', 'application')])
    ## node.h: uint32_t ns3::Node::AddDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddDevice', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## node.h: static bool ns3::Node::ChecksumEnabled() [member function]
    cls.add_method('ChecksumEnabled', 
                   'bool', 
                   [], 
                   is_static=True)
    ## node.h: ns3::Ptr<ns3::Application> ns3::Node::GetApplication(uint32_t index) const [member function]
    cls.add_method('GetApplication', 
                   'ns3::Ptr< ns3::Application >', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## node.h: ns3::Ptr<ns3::NetDevice> ns3::Node::GetDevice(uint32_t index) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## node.h: uint32_t ns3::Node::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h: uint32_t ns3::Node::GetNApplications() const [member function]
    cls.add_method('GetNApplications', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h: uint32_t ns3::Node::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h: uint32_t ns3::Node::GetSystemId() const [member function]
    cls.add_method('GetSystemId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h: static ns3::TypeId ns3::Node::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## node.h: void ns3::Node::RegisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler, uint16_t protocolType, ns3::Ptr<ns3::NetDevice> device, bool promiscuous=false) [member function]
    cls.add_method('RegisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler'), param('uint16_t', 'protocolType'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'promiscuous', default_value='false')])
    ## node.h: void ns3::Node::UnregisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler) [member function]
    cls.add_method('UnregisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler')])
    ## node.h: void ns3::Node::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## node.h: void ns3::Node::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## node.h: void ns3::Node::NotifyDeviceAdded(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('NotifyDeviceAdded', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3OutputStreamWrapper_methods(root_module, cls):
    ## output-stream-wrapper.h: ns3::OutputStreamWrapper::OutputStreamWrapper(ns3::OutputStreamWrapper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OutputStreamWrapper const &', 'arg0')])
    ## output-stream-wrapper.h: ns3::OutputStreamWrapper::OutputStreamWrapper(std::string filename, std::_Ios_Openmode filemode) [constructor]
    cls.add_constructor([param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode')])
    ## output-stream-wrapper.h: ns3::OutputStreamWrapper::OutputStreamWrapper(std::ostream * os) [constructor]
    cls.add_constructor([param('std::ostream *', 'os')])
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

def register_Ns3PacketSocket_methods(root_module, cls):
    ## packet-socket.h: ns3::PacketSocket::PacketSocket(ns3::PacketSocket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSocket const &', 'arg0')])
    ## packet-socket.h: ns3::PacketSocket::PacketSocket() [constructor]
    cls.add_constructor([])
    ## packet-socket.h: int ns3::PacketSocket::Bind() [member function]
    cls.add_method('Bind', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## packet-socket.h: int ns3::PacketSocket::Bind(ns3::Address const & address) [member function]
    cls.add_method('Bind', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_virtual=True)
    ## packet-socket.h: int ns3::PacketSocket::Close() [member function]
    cls.add_method('Close', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## packet-socket.h: int ns3::PacketSocket::Connect(ns3::Address const & address) [member function]
    cls.add_method('Connect', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_virtual=True)
    ## packet-socket.h: bool ns3::PacketSocket::GetAllowBroadcast() const [member function]
    cls.add_method('GetAllowBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packet-socket.h: ns3::Socket::SocketErrno ns3::PacketSocket::GetErrno() const [member function]
    cls.add_method('GetErrno', 
                   'ns3::Socket::SocketErrno', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packet-socket.h: ns3::Ptr<ns3::Node> ns3::PacketSocket::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packet-socket.h: uint32_t ns3::PacketSocket::GetRxAvailable() const [member function]
    cls.add_method('GetRxAvailable', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packet-socket.h: int ns3::PacketSocket::GetSockName(ns3::Address & address) const [member function]
    cls.add_method('GetSockName', 
                   'int', 
                   [param('ns3::Address &', 'address')], 
                   is_const=True, is_virtual=True)
    ## packet-socket.h: ns3::Socket::SocketType ns3::PacketSocket::GetSocketType() const [member function]
    cls.add_method('GetSocketType', 
                   'ns3::Socket::SocketType', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packet-socket.h: uint32_t ns3::PacketSocket::GetTxAvailable() const [member function]
    cls.add_method('GetTxAvailable', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packet-socket.h: static ns3::TypeId ns3::PacketSocket::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## packet-socket.h: int ns3::PacketSocket::Listen() [member function]
    cls.add_method('Listen', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## packet-socket.h: ns3::Ptr<ns3::Packet> ns3::PacketSocket::Recv(uint32_t maxSize, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags')], 
                   is_virtual=True)
    ## packet-socket.h: ns3::Ptr<ns3::Packet> ns3::PacketSocket::RecvFrom(uint32_t maxSize, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')], 
                   is_virtual=True)
    ## packet-socket.h: int ns3::PacketSocket::Send(ns3::Ptr<ns3::Packet> p, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags')], 
                   is_virtual=True)
    ## packet-socket.h: int ns3::PacketSocket::SendTo(ns3::Ptr<ns3::Packet> p, uint32_t flags, ns3::Address const & toAddress) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags'), param('ns3::Address const &', 'toAddress')], 
                   is_virtual=True)
    ## packet-socket.h: bool ns3::PacketSocket::SetAllowBroadcast(bool allowBroadcast) [member function]
    cls.add_method('SetAllowBroadcast', 
                   'bool', 
                   [param('bool', 'allowBroadcast')], 
                   is_virtual=True)
    ## packet-socket.h: void ns3::PacketSocket::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## packet-socket.h: int ns3::PacketSocket::ShutdownRecv() [member function]
    cls.add_method('ShutdownRecv', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## packet-socket.h: int ns3::PacketSocket::ShutdownSend() [member function]
    cls.add_method('ShutdownSend', 
                   'int', 
                   [], 
                   is_virtual=True)
    ## packet-socket.h: void ns3::PacketSocket::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PacketSocketFactory_methods(root_module, cls):
    ## packet-socket-factory.h: ns3::PacketSocketFactory::PacketSocketFactory(ns3::PacketSocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketSocketFactory const &', 'arg0')])
    ## packet-socket-factory.h: ns3::PacketSocketFactory::PacketSocketFactory() [constructor]
    cls.add_constructor([])
    ## packet-socket-factory.h: ns3::Ptr<ns3::Socket> ns3::PacketSocketFactory::CreateSocket() [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_virtual=True)
    ## packet-socket-factory.h: static ns3::TypeId ns3::PacketSocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3PbbAddressBlock_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbAddressBlock::PbbAddressBlock(ns3::PbbAddressBlock const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressBlock const &', 'arg0')])
    ## packetbb.h: ns3::PbbAddressBlock::PbbAddressBlock() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Address ns3::PbbAddressBlock::AddressBack() const [member function]
    cls.add_method('AddressBack', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressBegin() [member function]
    cls.add_method('AddressBegin', 
                   'std::_List_iterator< ns3::Address >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Address> ns3::PbbAddressBlock::AddressBegin() const [member function]
    cls.add_method('AddressBegin', 
                   'std::_List_const_iterator< ns3::Address >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::AddressClear() [member function]
    cls.add_method('AddressClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbAddressBlock::AddressEmpty() const [member function]
    cls.add_method('AddressEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressEnd() [member function]
    cls.add_method('AddressEnd', 
                   'std::_List_iterator< ns3::Address >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Address> ns3::PbbAddressBlock::AddressEnd() const [member function]
    cls.add_method('AddressEnd', 
                   'std::_List_const_iterator< ns3::Address >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressErase(std::_List_iterator<ns3::Address> position) [member function]
    cls.add_method('AddressErase', 
                   'std::_List_iterator< ns3::Address >', 
                   [param('std::_List_iterator< ns3::Address >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressErase(std::_List_iterator<ns3::Address> first, std::_List_iterator<ns3::Address> last) [member function]
    cls.add_method('AddressErase', 
                   'std::_List_iterator< ns3::Address >', 
                   [param('std::_List_iterator< ns3::Address >', 'first'), param('std::_List_iterator< ns3::Address >', 'last')])
    ## packetbb.h: ns3::Address ns3::PbbAddressBlock::AddressFront() const [member function]
    cls.add_method('AddressFront', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Address> ns3::PbbAddressBlock::AddressInsert(std::_List_iterator<ns3::Address> position, ns3::Address const value) [member function]
    cls.add_method('AddressInsert', 
                   'std::_List_iterator< ns3::Address >', 
                   [param('std::_List_iterator< ns3::Address >', 'position'), param('ns3::Address const', 'value')])
    ## packetbb.h: void ns3::PbbAddressBlock::AddressPopBack() [member function]
    cls.add_method('AddressPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::AddressPopFront() [member function]
    cls.add_method('AddressPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::AddressPushBack(ns3::Address address) [member function]
    cls.add_method('AddressPushBack', 
                   'void', 
                   [param('ns3::Address', 'address')])
    ## packetbb.h: void ns3::PbbAddressBlock::AddressPushFront(ns3::Address address) [member function]
    cls.add_method('AddressPushFront', 
                   'void', 
                   [param('ns3::Address', 'address')])
    ## packetbb.h: int ns3::PbbAddressBlock::AddressSize() const [member function]
    cls.add_method('AddressSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: uint32_t ns3::PbbAddressBlock::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbAddressBlock::PrefixBack() const [member function]
    cls.add_method('PrefixBack', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixBegin() [member function]
    cls.add_method('PrefixBegin', 
                   'std::_List_iterator< unsigned char >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<unsigned char> ns3::PbbAddressBlock::PrefixBegin() const [member function]
    cls.add_method('PrefixBegin', 
                   'std::_List_const_iterator< unsigned char >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixClear() [member function]
    cls.add_method('PrefixClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbAddressBlock::PrefixEmpty() const [member function]
    cls.add_method('PrefixEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixEnd() [member function]
    cls.add_method('PrefixEnd', 
                   'std::_List_iterator< unsigned char >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<unsigned char> ns3::PbbAddressBlock::PrefixEnd() const [member function]
    cls.add_method('PrefixEnd', 
                   'std::_List_const_iterator< unsigned char >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixErase(std::_List_iterator<unsigned char> position) [member function]
    cls.add_method('PrefixErase', 
                   'std::_List_iterator< unsigned char >', 
                   [param('std::_List_iterator< unsigned char >', 'position')])
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixErase(std::_List_iterator<unsigned char> first, std::_List_iterator<unsigned char> last) [member function]
    cls.add_method('PrefixErase', 
                   'std::_List_iterator< unsigned char >', 
                   [param('std::_List_iterator< unsigned char >', 'first'), param('std::_List_iterator< unsigned char >', 'last')])
    ## packetbb.h: uint8_t ns3::PbbAddressBlock::PrefixFront() const [member function]
    cls.add_method('PrefixFront', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<unsigned char> ns3::PbbAddressBlock::PrefixInsert(std::_List_iterator<unsigned char> position, uint8_t const value) [member function]
    cls.add_method('PrefixInsert', 
                   'std::_List_iterator< unsigned char >', 
                   [param('std::_List_iterator< unsigned char >', 'position'), param('uint8_t const', 'value')])
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixPopBack() [member function]
    cls.add_method('PrefixPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixPopFront() [member function]
    cls.add_method('PrefixPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixPushBack(uint8_t prefix) [member function]
    cls.add_method('PrefixPushBack', 
                   'void', 
                   [param('uint8_t', 'prefix')])
    ## packetbb.h: void ns3::PbbAddressBlock::PrefixPushFront(uint8_t prefix) [member function]
    cls.add_method('PrefixPushFront', 
                   'void', 
                   [param('uint8_t', 'prefix')])
    ## packetbb.h: int ns3::PbbAddressBlock::PrefixSize() const [member function]
    cls.add_method('PrefixSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> ns3::PbbAddressBlock::TlvBack() [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbAddressTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> const ns3::PbbAddressBlock::TlvBack() const [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbAddressTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvBegin() [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvBegin() const [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressBlock::TlvClear() [member function]
    cls.add_method('TlvClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbAddressBlock::TlvEmpty() const [member function]
    cls.add_method('TlvEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvEnd() [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvEnd() const [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvErase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > position) [member function]
    cls.add_method('TlvErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvErase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > last) [member function]
    cls.add_method('TlvErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> ns3::PbbAddressBlock::TlvFront() [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbAddressTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressTlv> const ns3::PbbAddressBlock::TlvFront() const [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbAddressTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > ns3::PbbAddressBlock::TlvInsert(std::_List_iterator<ns3::Ptr<ns3::PbbAddressTlv> > position, ns3::Ptr<ns3::PbbTlv> const value) [member function]
    cls.add_method('TlvInsert', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressTlv > >', 'position'), param('ns3::Ptr< ns3::PbbTlv > const', 'value')])
    ## packetbb.h: void ns3::PbbAddressBlock::TlvPopBack() [member function]
    cls.add_method('TlvPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::TlvPopFront() [member function]
    cls.add_method('TlvPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbAddressBlock::TlvPushBack(ns3::Ptr<ns3::PbbAddressTlv> address) [member function]
    cls.add_method('TlvPushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressTlv >', 'address')])
    ## packetbb.h: void ns3::PbbAddressBlock::TlvPushFront(ns3::Ptr<ns3::PbbAddressTlv> address) [member function]
    cls.add_method('TlvPushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressTlv >', 'address')])
    ## packetbb.h: int ns3::PbbAddressBlock::TlvSize() const [member function]
    cls.add_method('TlvSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Address ns3::PbbAddressBlock::DeserializeAddress(uint8_t * buffer) const [member function]
    cls.add_method('DeserializeAddress', 
                   'ns3::Address', 
                   [param('uint8_t *', 'buffer')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: uint8_t ns3::PbbAddressBlock::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'uint8_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlock::PrintAddress(std::ostream & os, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('PrintAddress', 
                   'void', 
                   [param('std::ostream &', 'os'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlock::SerializeAddress(uint8_t * buffer, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('SerializeAddress', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbAddressBlockIpv4_methods(root_module, cls):
    ## packetbb.h: ns3::PbbAddressBlockIpv4::PbbAddressBlockIpv4(ns3::PbbAddressBlockIpv4 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressBlockIpv4 const &', 'arg0')])
    ## packetbb.h: ns3::PbbAddressBlockIpv4::PbbAddressBlockIpv4() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Address ns3::PbbAddressBlockIpv4::DeserializeAddress(uint8_t * buffer) const [member function]
    cls.add_method('DeserializeAddress', 
                   'ns3::Address', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: uint8_t ns3::PbbAddressBlockIpv4::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlockIpv4::PrintAddress(std::ostream & os, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('PrintAddress', 
                   'void', 
                   [param('std::ostream &', 'os'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlockIpv4::SerializeAddress(uint8_t * buffer, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('SerializeAddress', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbAddressBlockIpv6_methods(root_module, cls):
    ## packetbb.h: ns3::PbbAddressBlockIpv6::PbbAddressBlockIpv6(ns3::PbbAddressBlockIpv6 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressBlockIpv6 const &', 'arg0')])
    ## packetbb.h: ns3::PbbAddressBlockIpv6::PbbAddressBlockIpv6() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Address ns3::PbbAddressBlockIpv6::DeserializeAddress(uint8_t * buffer) const [member function]
    cls.add_method('DeserializeAddress', 
                   'ns3::Address', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: uint8_t ns3::PbbAddressBlockIpv6::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlockIpv6::PrintAddress(std::ostream & os, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('PrintAddress', 
                   'void', 
                   [param('std::ostream &', 'os'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbAddressBlockIpv6::SerializeAddress(uint8_t * buffer, std::_List_const_iterator<ns3::Address> iter) const [member function]
    cls.add_method('SerializeAddress', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('std::_List_const_iterator< ns3::Address >', 'iter')], 
                   is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbMessage_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbMessage::PbbMessage(ns3::PbbMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbMessage const &', 'arg0')])
    ## packetbb.h: ns3::PbbMessage::PbbMessage() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessage::AddressBlockBack() [member function]
    cls.add_method('AddressBlockBack', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> const ns3::PbbMessage::AddressBlockBack() const [member function]
    cls.add_method('AddressBlockBack', 
                   'ns3::Ptr< ns3::PbbAddressBlock > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockBegin() [member function]
    cls.add_method('AddressBlockBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockBegin() const [member function]
    cls.add_method('AddressBlockBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::AddressBlockClear() [member function]
    cls.add_method('AddressBlockClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbMessage::AddressBlockEmpty() const [member function]
    cls.add_method('AddressBlockEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockEnd() [member function]
    cls.add_method('AddressBlockEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockEnd() const [member function]
    cls.add_method('AddressBlockEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockErase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > position) [member function]
    cls.add_method('AddressBlockErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > ns3::PbbMessage::AddressBlockErase(std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > first, std::_List_iterator<ns3::Ptr<ns3::PbbAddressBlock> > last) [member function]
    cls.add_method('AddressBlockErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbAddressBlock > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessage::AddressBlockFront() [member function]
    cls.add_method('AddressBlockFront', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> const ns3::PbbMessage::AddressBlockFront() const [member function]
    cls.add_method('AddressBlockFront', 
                   'ns3::Ptr< ns3::PbbAddressBlock > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::AddressBlockPopBack() [member function]
    cls.add_method('AddressBlockPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbMessage::AddressBlockPopFront() [member function]
    cls.add_method('AddressBlockPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbMessage::AddressBlockPushBack(ns3::Ptr<ns3::PbbAddressBlock> block) [member function]
    cls.add_method('AddressBlockPushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressBlock >', 'block')])
    ## packetbb.h: void ns3::PbbMessage::AddressBlockPushFront(ns3::Ptr<ns3::PbbAddressBlock> block) [member function]
    cls.add_method('AddressBlockPushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbAddressBlock >', 'block')])
    ## packetbb.h: int ns3::PbbMessage::AddressBlockSize() const [member function]
    cls.add_method('AddressBlockSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: static ns3::Ptr<ns3::PbbMessage> ns3::PbbMessage::DeserializeMessage(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('DeserializeMessage', 
                   'ns3::Ptr< ns3::PbbMessage >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_static=True)
    ## packetbb.h: uint8_t ns3::PbbMessage::GetHopCount() const [member function]
    cls.add_method('GetHopCount', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbMessage::GetHopLimit() const [member function]
    cls.add_method('GetHopLimit', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Address ns3::PbbMessage::GetOriginatorAddress() const [member function]
    cls.add_method('GetOriginatorAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint16_t ns3::PbbMessage::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint32_t ns3::PbbMessage::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbMessage::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbMessage::HasHopCount() const [member function]
    cls.add_method('HasHopCount', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbMessage::HasHopLimit() const [member function]
    cls.add_method('HasHopLimit', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbMessage::HasOriginatorAddress() const [member function]
    cls.add_method('HasOriginatorAddress', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbMessage::HasSequenceNumber() const [member function]
    cls.add_method('HasSequenceNumber', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::SetHopCount(uint8_t hopcount) [member function]
    cls.add_method('SetHopCount', 
                   'void', 
                   [param('uint8_t', 'hopcount')])
    ## packetbb.h: void ns3::PbbMessage::SetHopLimit(uint8_t hoplimit) [member function]
    cls.add_method('SetHopLimit', 
                   'void', 
                   [param('uint8_t', 'hoplimit')])
    ## packetbb.h: void ns3::PbbMessage::SetOriginatorAddress(ns3::Address address) [member function]
    cls.add_method('SetOriginatorAddress', 
                   'void', 
                   [param('ns3::Address', 'address')])
    ## packetbb.h: void ns3::PbbMessage::SetSequenceNumber(uint16_t seqnum) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'seqnum')])
    ## packetbb.h: void ns3::PbbMessage::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbMessage::TlvBack() [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> const ns3::PbbMessage::TlvBack() const [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvBegin() [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvBegin() const [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::TlvClear() [member function]
    cls.add_method('TlvClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbMessage::TlvEmpty() const [member function]
    cls.add_method('TlvEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvEnd() [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvEnd() const [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvErase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > position) [member function]
    cls.add_method('TlvErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbMessage::TlvErase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > last) [member function]
    cls.add_method('TlvErase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'last')])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbMessage::TlvFront() [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> const ns3::PbbMessage::TlvFront() const [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbMessage::TlvPopBack() [member function]
    cls.add_method('TlvPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbMessage::TlvPopFront() [member function]
    cls.add_method('TlvPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbMessage::TlvPushBack(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('TlvPushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbMessage::TlvPushFront(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('TlvPushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: int ns3::PbbMessage::TlvSize() const [member function]
    cls.add_method('TlvSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessage::AddressBlockDeserialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('AddressBlockDeserialize', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::Address ns3::PbbMessage::DeserializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('DeserializeOriginatorAddress', 
                   'ns3::Address', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::PbbAddressLength ns3::PbbMessage::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'ns3::PbbAddressLength', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessage::PrintOriginatorAddress(std::ostream & os) const [member function]
    cls.add_method('PrintOriginatorAddress', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessage::SerializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('SerializeOriginatorAddress', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_pure_virtual=True, is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbMessageIpv4_methods(root_module, cls):
    ## packetbb.h: ns3::PbbMessageIpv4::PbbMessageIpv4(ns3::PbbMessageIpv4 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbMessageIpv4 const &', 'arg0')])
    ## packetbb.h: ns3::PbbMessageIpv4::PbbMessageIpv4() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessageIpv4::AddressBlockDeserialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('AddressBlockDeserialize', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::Address ns3::PbbMessageIpv4::DeserializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('DeserializeOriginatorAddress', 
                   'ns3::Address', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::PbbAddressLength ns3::PbbMessageIpv4::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'ns3::PbbAddressLength', 
                   [], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessageIpv4::PrintOriginatorAddress(std::ostream & os) const [member function]
    cls.add_method('PrintOriginatorAddress', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessageIpv4::SerializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('SerializeOriginatorAddress', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbMessageIpv6_methods(root_module, cls):
    ## packetbb.h: ns3::PbbMessageIpv6::PbbMessageIpv6(ns3::PbbMessageIpv6 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbMessageIpv6 const &', 'arg0')])
    ## packetbb.h: ns3::PbbMessageIpv6::PbbMessageIpv6() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::Ptr<ns3::PbbAddressBlock> ns3::PbbMessageIpv6::AddressBlockDeserialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('AddressBlockDeserialize', 
                   'ns3::Ptr< ns3::PbbAddressBlock >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::Address ns3::PbbMessageIpv6::DeserializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('DeserializeOriginatorAddress', 
                   'ns3::Address', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: ns3::PbbAddressLength ns3::PbbMessageIpv6::GetAddressLength() const [member function]
    cls.add_method('GetAddressLength', 
                   'ns3::PbbAddressLength', 
                   [], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessageIpv6::PrintOriginatorAddress(std::ostream & os) const [member function]
    cls.add_method('PrintOriginatorAddress', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## packetbb.h: void ns3::PbbMessageIpv6::SerializeOriginatorAddress(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('SerializeOriginatorAddress', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True, visibility='protected', is_virtual=True)
    return

def register_Ns3PbbPacket_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbPacket::PbbPacket(ns3::PbbPacket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbPacket const &', 'arg0')])
    ## packetbb.h: ns3::PbbPacket::PbbPacket() [constructor]
    cls.add_constructor([])
    ## packetbb.h: uint32_t ns3::PbbPacket::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > position) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > first, std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > last) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 'last')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > position) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 'position')])
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::Erase(std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > first, std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > last) [member function]
    cls.add_method('Erase', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [param('std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 'first'), param('std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 'last')])
    ## packetbb.h: ns3::TypeId ns3::PbbPacket::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packetbb.h: uint16_t ns3::PbbPacket::GetSequenceNumber() const [member function]
    cls.add_method('GetSequenceNumber', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint32_t ns3::PbbPacket::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## packetbb.h: static ns3::TypeId ns3::PbbPacket::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## packetbb.h: uint8_t ns3::PbbPacket::GetVersion() const [member function]
    cls.add_method('GetVersion', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbPacket::HasSequenceNumber() const [member function]
    cls.add_method('HasSequenceNumber', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbMessage> ns3::PbbPacket::MessageBack() [member function]
    cls.add_method('MessageBack', 
                   'ns3::Ptr< ns3::PbbMessage >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbMessage> const ns3::PbbPacket::MessageBack() const [member function]
    cls.add_method('MessageBack', 
                   'ns3::Ptr< ns3::PbbMessage > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::MessageBegin() [member function]
    cls.add_method('MessageBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::MessageBegin() const [member function]
    cls.add_method('MessageBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::MessageClear() [member function]
    cls.add_method('MessageClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbPacket::MessageEmpty() const [member function]
    cls.add_method('MessageEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::MessageEnd() [member function]
    cls.add_method('MessageEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbMessage> > ns3::PbbPacket::MessageEnd() const [member function]
    cls.add_method('MessageEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbMessage > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbMessage> ns3::PbbPacket::MessageFront() [member function]
    cls.add_method('MessageFront', 
                   'ns3::Ptr< ns3::PbbMessage >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbMessage> const ns3::PbbPacket::MessageFront() const [member function]
    cls.add_method('MessageFront', 
                   'ns3::Ptr< ns3::PbbMessage > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::MessagePopBack() [member function]
    cls.add_method('MessagePopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbPacket::MessagePopFront() [member function]
    cls.add_method('MessagePopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbPacket::MessagePushBack(ns3::Ptr<ns3::PbbMessage> message) [member function]
    cls.add_method('MessagePushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbMessage >', 'message')])
    ## packetbb.h: void ns3::PbbPacket::MessagePushFront(ns3::Ptr<ns3::PbbMessage> message) [member function]
    cls.add_method('MessagePushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbMessage >', 'message')])
    ## packetbb.h: int ns3::PbbPacket::MessageSize() const [member function]
    cls.add_method('MessageSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## packetbb.h: void ns3::PbbPacket::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## packetbb.h: void ns3::PbbPacket::SetSequenceNumber(uint16_t number) [member function]
    cls.add_method('SetSequenceNumber', 
                   'void', 
                   [param('uint16_t', 'number')])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbPacket::TlvBack() [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> const ns3::PbbPacket::TlvBack() const [member function]
    cls.add_method('TlvBack', 
                   'ns3::Ptr< ns3::PbbTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::TlvBegin() [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::TlvBegin() const [member function]
    cls.add_method('TlvBegin', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::TlvClear() [member function]
    cls.add_method('TlvClear', 
                   'void', 
                   [])
    ## packetbb.h: bool ns3::PbbPacket::TlvEmpty() const [member function]
    cls.add_method('TlvEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: std::_List_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::TlvEnd() [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [])
    ## packetbb.h: std::_List_const_iterator<ns3::Ptr<ns3::PbbTlv> > ns3::PbbPacket::TlvEnd() const [member function]
    cls.add_method('TlvEnd', 
                   'std::_List_const_iterator< ns3::Ptr< ns3::PbbTlv > >', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> ns3::PbbPacket::TlvFront() [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbTlv >', 
                   [])
    ## packetbb.h: ns3::Ptr<ns3::PbbTlv> const ns3::PbbPacket::TlvFront() const [member function]
    cls.add_method('TlvFront', 
                   'ns3::Ptr< ns3::PbbTlv > const', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbPacket::TlvPopBack() [member function]
    cls.add_method('TlvPopBack', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbPacket::TlvPopFront() [member function]
    cls.add_method('TlvPopFront', 
                   'void', 
                   [])
    ## packetbb.h: void ns3::PbbPacket::TlvPushBack(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('TlvPushBack', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: void ns3::PbbPacket::TlvPushFront(ns3::Ptr<ns3::PbbTlv> tlv) [member function]
    cls.add_method('TlvPushFront', 
                   'void', 
                   [param('ns3::Ptr< ns3::PbbTlv >', 'tlv')])
    ## packetbb.h: int ns3::PbbPacket::TlvSize() const [member function]
    cls.add_method('TlvSize', 
                   'int', 
                   [], 
                   is_const=True)
    return

def register_Ns3PbbTlv_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('!=')
    ## packetbb.h: ns3::PbbTlv::PbbTlv(ns3::PbbTlv const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbTlv const &', 'arg0')])
    ## packetbb.h: ns3::PbbTlv::PbbTlv() [constructor]
    cls.add_constructor([])
    ## packetbb.h: void ns3::PbbTlv::Deserialize(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')])
    ## packetbb.h: uint32_t ns3::PbbTlv::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbTlv::GetType() const [member function]
    cls.add_method('GetType', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbTlv::GetTypeExt() const [member function]
    cls.add_method('GetTypeExt', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: ns3::Buffer ns3::PbbTlv::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'ns3::Buffer', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbTlv::HasTypeExt() const [member function]
    cls.add_method('HasTypeExt', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbTlv::HasValue() const [member function]
    cls.add_method('HasValue', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlv::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlv::Print(std::ostream & os, int level) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os'), param('int', 'level')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlv::Serialize(ns3::Buffer::Iterator & start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbTlv::SetType(uint8_t type) [member function]
    cls.add_method('SetType', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## packetbb.h: void ns3::PbbTlv::SetTypeExt(uint8_t type) [member function]
    cls.add_method('SetTypeExt', 
                   'void', 
                   [param('uint8_t', 'type')])
    ## packetbb.h: void ns3::PbbTlv::SetValue(ns3::Buffer start) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('ns3::Buffer', 'start')])
    ## packetbb.h: void ns3::PbbTlv::SetValue(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('SetValue', 
                   'void', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## packetbb.h: uint8_t ns3::PbbTlv::GetIndexStart() const [member function]
    cls.add_method('GetIndexStart', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: uint8_t ns3::PbbTlv::GetIndexStop() const [member function]
    cls.add_method('GetIndexStop', 
                   'uint8_t', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: bool ns3::PbbTlv::HasIndexStart() const [member function]
    cls.add_method('HasIndexStart', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: bool ns3::PbbTlv::HasIndexStop() const [member function]
    cls.add_method('HasIndexStop', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: bool ns3::PbbTlv::IsMultivalue() const [member function]
    cls.add_method('IsMultivalue', 
                   'bool', 
                   [], 
                   is_const=True, visibility='protected')
    ## packetbb.h: void ns3::PbbTlv::SetIndexStart(uint8_t index) [member function]
    cls.add_method('SetIndexStart', 
                   'void', 
                   [param('uint8_t', 'index')], 
                   visibility='protected')
    ## packetbb.h: void ns3::PbbTlv::SetIndexStop(uint8_t index) [member function]
    cls.add_method('SetIndexStop', 
                   'void', 
                   [param('uint8_t', 'index')], 
                   visibility='protected')
    ## packetbb.h: void ns3::PbbTlv::SetMultivalue(bool isMultivalue) [member function]
    cls.add_method('SetMultivalue', 
                   'void', 
                   [param('bool', 'isMultivalue')], 
                   visibility='protected')
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

def register_Ns3SimpleChannel_methods(root_module, cls):
    ## simple-channel.h: ns3::SimpleChannel::SimpleChannel(ns3::SimpleChannel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimpleChannel const &', 'arg0')])
    ## simple-channel.h: ns3::SimpleChannel::SimpleChannel() [constructor]
    cls.add_constructor([])
    ## simple-channel.h: void ns3::SimpleChannel::Add(ns3::Ptr<ns3::SimpleNetDevice> device) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::SimpleNetDevice >', 'device')])
    ## simple-channel.h: ns3::Ptr<ns3::NetDevice> ns3::SimpleChannel::GetDevice(uint32_t i) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True, is_virtual=True)
    ## simple-channel.h: uint32_t ns3::SimpleChannel::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-channel.h: static ns3::TypeId ns3::SimpleChannel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## simple-channel.h: void ns3::SimpleChannel::Send(ns3::Ptr<ns3::Packet> p, uint16_t protocol, ns3::Mac48Address to, ns3::Mac48Address from, ns3::Ptr<ns3::SimpleNetDevice> sender) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint16_t', 'protocol'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from'), param('ns3::Ptr< ns3::SimpleNetDevice >', 'sender')])
    return

def register_Ns3SimpleNetDevice_methods(root_module, cls):
    ## simple-net-device.h: ns3::SimpleNetDevice::SimpleNetDevice(ns3::SimpleNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SimpleNetDevice const &', 'arg0')])
    ## simple-net-device.h: ns3::SimpleNetDevice::SimpleNetDevice() [constructor]
    cls.add_constructor([])
    ## simple-net-device.h: void ns3::SimpleNetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## simple-net-device.h: ns3::Address ns3::SimpleNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Address ns3::SimpleNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Ptr<ns3::Channel> ns3::SimpleNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: uint32_t ns3::SimpleNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: uint16_t ns3::SimpleNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Address ns3::SimpleNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Address ns3::SimpleNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: ns3::Ptr<ns3::Node> ns3::SimpleNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: static ns3::TypeId ns3::SimpleNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::Receive(ns3::Ptr<ns3::Packet> packet, uint16_t protocol, ns3::Mac48Address to, ns3::Mac48Address from) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('uint16_t', 'protocol'), param('ns3::Mac48Address', 'to'), param('ns3::Mac48Address', 'from')])
    ## simple-net-device.h: bool ns3::SimpleNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetChannel(ns3::Ptr<ns3::SimpleChannel> channel) [member function]
    cls.add_method('SetChannel', 
                   'void', 
                   [param('ns3::Ptr< ns3::SimpleChannel >', 'channel')])
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## simple-net-device.h: bool ns3::SimpleNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::SetReceiveErrorModel(ns3::Ptr<ns3::ErrorModel> em) [member function]
    cls.add_method('SetReceiveErrorModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorModel >', 'em')])
    ## simple-net-device.h: bool ns3::SimpleNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## simple-net-device.h: void ns3::SimpleNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3AddressChecker_methods(root_module, cls):
    ## address.h: ns3::AddressChecker::AddressChecker() [constructor]
    cls.add_constructor([])
    ## address.h: ns3::AddressChecker::AddressChecker(ns3::AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AddressChecker const &', 'arg0')])
    return

def register_Ns3AddressValue_methods(root_module, cls):
    ## address.h: ns3::AddressValue::AddressValue() [constructor]
    cls.add_constructor([])
    ## address.h: ns3::AddressValue::AddressValue(ns3::AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AddressValue const &', 'arg0')])
    ## address.h: ns3::AddressValue::AddressValue(ns3::Address const & value) [constructor]
    cls.add_constructor([param('ns3::Address const &', 'value')])
    ## address.h: ns3::Ptr<ns3::AttributeValue> ns3::AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## address.h: bool ns3::AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## address.h: ns3::Address ns3::AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## address.h: std::string ns3::AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## address.h: void ns3::AddressValue::Set(ns3::Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Address const &', 'value')])
    return

def register_Ns3PbbAddressTlv_methods(root_module, cls):
    ## packetbb.h: ns3::PbbAddressTlv::PbbAddressTlv() [constructor]
    cls.add_constructor([])
    ## packetbb.h: ns3::PbbAddressTlv::PbbAddressTlv(ns3::PbbAddressTlv const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PbbAddressTlv const &', 'arg0')])
    ## packetbb.h: uint8_t ns3::PbbAddressTlv::GetIndexStart() const [member function]
    cls.add_method('GetIndexStart', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: uint8_t ns3::PbbAddressTlv::GetIndexStop() const [member function]
    cls.add_method('GetIndexStop', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbAddressTlv::HasIndexStart() const [member function]
    cls.add_method('HasIndexStart', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbAddressTlv::HasIndexStop() const [member function]
    cls.add_method('HasIndexStop', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: bool ns3::PbbAddressTlv::IsMultivalue() const [member function]
    cls.add_method('IsMultivalue', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packetbb.h: void ns3::PbbAddressTlv::SetIndexStart(uint8_t index) [member function]
    cls.add_method('SetIndexStart', 
                   'void', 
                   [param('uint8_t', 'index')])
    ## packetbb.h: void ns3::PbbAddressTlv::SetIndexStop(uint8_t index) [member function]
    cls.add_method('SetIndexStop', 
                   'void', 
                   [param('uint8_t', 'index')])
    ## packetbb.h: void ns3::PbbAddressTlv::SetMultivalue(bool isMultivalue) [member function]
    cls.add_method('SetMultivalue', 
                   'void', 
                   [param('bool', 'isMultivalue')])
    return

def register_functions(root_module):
    module = root_module
    ## address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeAddressChecker() [free function]
    module.add_function('MakeAddressChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## data-rate.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeDataRateChecker() [free function]
    module.add_function('MakeDataRateChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ipv4-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeIpv4AddressChecker() [free function]
    module.add_function('MakeIpv4AddressChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ipv4-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeIpv4MaskChecker() [free function]
    module.add_function('MakeIpv4MaskChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ipv6-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeIpv6AddressChecker() [free function]
    module.add_function('MakeIpv6AddressChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ipv6-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeIpv6PrefixChecker() [free function]
    module.add_function('MakeIpv6PrefixChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## mac48-address.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeMac48AddressChecker() [free function]
    module.add_function('MakeMac48AddressChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## address-utils.h: extern void ns3::ReadFrom(ns3::Buffer::Iterator & i, ns3::Address & ad, uint32_t len) [free function]
    module.add_function('ReadFrom', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Address &', 'ad'), param('uint32_t', 'len')])
    ## address-utils.h: extern void ns3::ReadFrom(ns3::Buffer::Iterator & i, ns3::Ipv4Address & ad) [free function]
    module.add_function('ReadFrom', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Ipv4Address &', 'ad')])
    ## address-utils.h: extern void ns3::ReadFrom(ns3::Buffer::Iterator & i, ns3::Ipv6Address & ad) [free function]
    module.add_function('ReadFrom', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Ipv6Address &', 'ad')])
    ## address-utils.h: extern void ns3::ReadFrom(ns3::Buffer::Iterator & i, ns3::Mac48Address & ad) [free function]
    module.add_function('ReadFrom', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Mac48Address &', 'ad')])
    ## address-utils.h: extern void ns3::WriteTo(ns3::Buffer::Iterator & i, ns3::Address const & ad) [free function]
    module.add_function('WriteTo', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Address const &', 'ad')])
    ## address-utils.h: extern void ns3::WriteTo(ns3::Buffer::Iterator & i, ns3::Ipv4Address ad) [free function]
    module.add_function('WriteTo', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Ipv4Address', 'ad')])
    ## address-utils.h: extern void ns3::WriteTo(ns3::Buffer::Iterator & i, ns3::Ipv6Address ad) [free function]
    module.add_function('WriteTo', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Ipv6Address', 'ad')])
    ## address-utils.h: extern void ns3::WriteTo(ns3::Buffer::Iterator & i, ns3::Mac48Address ad) [free function]
    module.add_function('WriteTo', 
                        'void', 
                        [param('ns3::Buffer::Iterator &', 'i'), param('ns3::Mac48Address', 'ad')])
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
    ## address-utils.h: extern bool ns3::addressUtils::IsMulticast(ns3::Address const & ad) [free function]
    module.add_function('IsMulticast', 
                        'bool', 
                        [param('ns3::Address const &', 'ad')])
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

