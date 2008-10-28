/*!
\page packet-header-trailer How to create a new type of protocol header or trailer
\anchor howtos-packet-header-trailer

<b>Question:</b> I want to implement a new protocol X which uses a new
type of header Y. How do I implement and use this new header Y in ns-3 ?

<b>Answer:</b> The key is to implement a new subclass of the ns3::Header
base class to represent your protocol header:
\code
class YHeader : public Header
{
public:
  // must be implemented to become a valid new header.
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

  // allow protocol-specific access to the header data.
  void SetData (uint32_t data);
  uint32_t GetData (void) const;
private:
  uint32_t m_data;
};
\endcode

Once this class is implemented, you can easily store your protocol
header into a packet:
\code
Ptr<Packet> p = ...;
YHeader yHeader;
yHeader.SetData (0xdeadbeaf);
// copy the header into the packet
p->AddHeader (yHeader);
\endcode
and get it out of a packet:
\code
Ptr<Packet> p = ...;
YHeader yHeader;
// copy the header from the packet
p->RemoveHeader (yHeader);
uint32_t data = yHeader.GetData ();
\endcode

The implementation of the new header is very simple. First, you need
to give a TypeId to your YHeader class:
\code
TypeId
YHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("YHeader")
    .SetParent<Header> ()
    .AddConstructor<YHeader> ()
  ;
  return tid;
}
TypeId
YHeader::GetInstanceTypeId (void)
{
  return GetTypeId ();
}
\endcode

Then, you need to allow your header to serialize and deserialize itself
to a byte buffer in its network representation. Here, our new protocol
header contains first a 2-byte constant, and, then, the data field so,
the total size of the header is 2+4=6 bytes.
\code
uint32_t 
YHeader::GetSerializedSize (void) const
{
  return 6;
}
void 
YHeader::Serialize (Buffer::Iterator start) const
{
  // The 2 byte-constant
  start.WriteU8 (0xfe);
  start.WriteU8 (0xef);
  // The data.
  start.WriteHtonU32 (m_data);
}
uint32_t 
YHeader::Deserialize (Buffer::Iterator start)
{
  uint8_t tmp;
  tmp = start.ReadU8 ();
  NS_ASSERT (tmp == 0xfe);
  tmp = start.ReadU8 ();
  NS_ASSERT (tmp == 0xef);
  m_data = start.ReadNtohU32 ();
  return 6; // the number of bytes consumed.
}
\endcode

Finally, to make sure that Packet::Print also prints the content
of your header, just as it prints the content of the other
headers of the system, you need to provide a Print method:
\code
void 
YHeader::Print (std::ostream &os) const
{
  os << "data=" << m_data;
}
\endcode

The code will look the same if you want to implement a trailer,
that is, a protocol data structure which will be appended to the
end of the packet, not its start: you need to make sure that
you derive from the ns3::Trailer base class and that you call
Packet::AddTrailer and Packet::RemoveTrailer. Another important
detail is that you must make sure to rewind the iterator in your
Serialize and Deserialize methods writing to or reading from 
the underlying buffer.

*/
