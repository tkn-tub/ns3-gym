/*!
  \page net-device How to create a new OSI layer 1 + 2 implementation ?
  \anchor howtos-net-device

  <b>Question:</b> How do I integrate a new OSI layer 1 + 2 implementation ?

  <b>Answer:</b> The OSI layers 1 and 2 are represented by the ns3::NetDevice
  and ns3::Channel classes. To plug transparently in ns-3, a new layer 1+2 model
  thus simply needs to provide two new subclasses of these two base classes.

  To make that subclassing process easy, two skeleton classes are provided in
  the src/node directory: simple-net-device.h (ns3::SimpleNetDevice) and 
  simple-channel.h (ns3::SimpleChannel) implement a broadcast passthru medium 
  using 48bit MAC addresses without any kind of MAC access algorithm or PHY 
  layer modeling.

  The ns3::SimpleChannel class is really very simple: it provides
  an implementation for the ns3::Channel::GetNDevices and ns3::Channel::GetDevice
  methods defined in the Channel base class and, then defines the channel-specific
  send and add methods:
- The Add method is used by SimpleNetDevice::SetChannel to register a new 
  SimpleNetDevice with its associated channel.
- The Send method is used by SimpleNetDevice::Send to send a packet over the
  broadcast medium and ensure that it gets delivered to all associated devices
  (except the sender).

\code
class SimpleChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  SimpleChannel ();

  void Send (Ptr<Packet> p, uint16_t protocol, Mac48Address to, Mac48Address from,
	     Ptr<SimpleNetDevice> sender);

  void Add (Ptr<SimpleNetDevice> device);

  // inherited from ns3::Channel
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

private:
  std::vector<Ptr<SimpleNetDevice> > m_devices;
};
\endcode

The SimpleNetDevice class is also trivial since it implements no special
MAC-layer processing:
\code
class SimpleNetDevice : public NetDevice
{
public:
  static TypeId GetTypeId (void);
  SimpleNetDevice ();

  void Receive (Ptr<Packet> packet, uint16_t protocol, Mac48Address to, Mac48Address from);
  void SetChannel (Ptr<SimpleChannel> channel);
  void SetAddress (Mac48Address address);

  // inherited from NetDevice base class.
  virtual void SetName(const std::string name);
  ...
};
\endcode

The code below illustrates how the three model-specific methods defined above are
implemented:

\code
void 
SimpleNetDevice::Receive (Ptr<Packet> packet, uint16_t protocol, 
			  Mac48Address to, Mac48Address from)
{
  if (to == m_address || to == Mac48Address::GetBroadcast ())
    {
      m_rxCallback (this, packet, protocol, from);
    }
}
void 
SimpleNetDevice::SetChannel (Ptr<SimpleChannel> channel)
{
  m_channel = channel;
  m_channel->Add (this);
}
void 
SimpleNetDevice::SetAddress (Mac48Address address)
{
  m_address = address;
}
\endcode

*/
