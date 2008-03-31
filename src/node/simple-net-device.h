#ifndef SIMPLE_NET_DEVICE_H
#define SIMPLE_NET_DEVICE_H

#include "net-device.h"
#include "mac48-address.h"
#include <stdint.h>
#include <string>

namespace ns3 {

class SimpleChannel;
class Node;

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
  virtual std::string GetName(void) const;
  virtual void SetIfIndex(const uint32_t index);
  virtual uint32_t GetIfIndex(void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void SetLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (void) const;
  virtual Address MakeMulticastAddress (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

protected:
  virtual void DoDispose (void);
private:
  Ptr<SimpleChannel> m_channel;
  NetDevice::ReceiveCallback m_rxCallback;
  Ptr<Node> m_node;
  uint16_t m_mtu;
  std::string m_name;
  uint32_t m_ifIndex;
  Mac48Address m_address;
};

} // namespace ns3

#endif /* SIMPLE_NET_DEVICE_H */
