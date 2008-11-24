#ifndef ICMPV4_L4_PROTOCOL_H
#define ICMPV4_L4_PROTOCOL_H

#include "ipv4-l4-protocol.h"
#include "icmpv4.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

class Node;
class Ipv4Interface;

class Icmpv4L4Protocol : public Ipv4L4Protocol
{
public:
  static TypeId GetTypeId (void);
  Icmpv4L4Protocol ();
  virtual ~Icmpv4L4Protocol ();

  void SetNode (Ptr<Node> node);

  static uint16_t GetStaticProtocolNumber (void);
  virtual int GetProtocolNumber (void) const;
  virtual enum Ipv4L4Protocol::RxStatus Receive(Ptr<Packet> p, 
						const Ipv4Address &source,
						const Ipv4Address &destination,
						Ptr<Ipv4Interface> incomingInterface);

  void SendDestUnreachFragNeeded (Ipv4Header header, Ptr<const Packet> orgData, uint16_t nextHopMtu);
  void SendTimeExceededTtl (Ipv4Header header, Ptr<const Packet> orgData);
  void SendDestUnreachPort (Ipv4Header header, Ptr<const Packet> orgData);

private:
  void HandleEcho (Ptr<Packet> p,
		   Icmpv4Header header, 
		   Ipv4Address source,
		   Ipv4Address destination);
  void HandleDestUnreach (Ptr<Packet> p,
			  Icmpv4Header header, 
			  Ipv4Address source,
			  Ipv4Address destination);
  void HandleTimeExceeded (Ptr<Packet> p,
			   Icmpv4Header icmp, 
			   Ipv4Address source,
			   Ipv4Address destination);
  void SendDestUnreach (Ipv4Header header, Ptr<const Packet> orgData, 
			uint8_t code, uint16_t nextHopMtu);
  void SendMessage (Ptr<Packet> packet, Ipv4Address dest, uint8_t type, uint8_t code);
  void SendMessage (Ptr<Packet> packet, Ipv4Address source, Ipv4Address dest, uint8_t type, uint8_t code);
  void Forward (Ipv4Address source, Icmpv4Header icmp,
		uint32_t info, Ipv4Header ipHeader,
		const uint8_t payload[8]);

  virtual void DoDispose (void);

  Ptr<Node> m_node;
  bool m_calcChecksum;
};

} // namespace ns3

#endif /* ICMPV4_L4_PROTOCOL_H */
