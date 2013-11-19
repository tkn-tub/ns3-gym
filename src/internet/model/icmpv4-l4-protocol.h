#ifndef ICMPV4_L4_PROTOCOL_H
#define ICMPV4_L4_PROTOCOL_H

#include "ip-l4-protocol.h"
#include "icmpv4.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

class Node;
class Ipv4Interface;
class Ipv4Route;

class Icmpv4L4Protocol : public IpL4Protocol
{
public:
  static TypeId GetTypeId (void);
  static const uint8_t PROT_NUMBER;

  Icmpv4L4Protocol ();
  virtual ~Icmpv4L4Protocol ();

  void SetNode (Ptr<Node> node);

  static uint16_t GetStaticProtocolNumber (void);
  virtual int GetProtocolNumber (void) const;

  /**
   * \brief Receive method.
   * \param p the packet
   * \param header the IPv4 header
   * \param interface the interface from which the packet is coming
   */
  virtual enum IpL4Protocol::RxStatus Receive (Ptr<Packet> p,
                                               Ipv4Header const &header,
                                               Ptr<Ipv4Interface> incomingInterface);

  /**
   * \brief Receive method.
   * \param p the packet
   * \param header the IPv6 header
   * \param interface the interface from which the packet is coming
   */
  virtual enum IpL4Protocol::RxStatus Receive (Ptr<Packet> p,
                                               Ipv6Header const &header,
                                               Ptr<Ipv6Interface> incomingInterface);

  void SendDestUnreachFragNeeded (Ipv4Header header, Ptr<const Packet> orgData, uint16_t nextHopMtu);
  void SendTimeExceededTtl (Ipv4Header header, Ptr<const Packet> orgData);
  void SendDestUnreachPort (Ipv4Header header, Ptr<const Packet> orgData);

  // From IpL4Protocol
  virtual void SetDownTarget (IpL4Protocol::DownTargetCallback cb);
  virtual void SetDownTarget6 (IpL4Protocol::DownTargetCallback6 cb);
  // From IpL4Protocol
  virtual IpL4Protocol::DownTargetCallback GetDownTarget (void) const;
  virtual IpL4Protocol::DownTargetCallback6 GetDownTarget6 (void) const;
protected:
  /*
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();
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
  void SendMessage (Ptr<Packet> packet, Ipv4Address source, Ipv4Address dest, uint8_t type, uint8_t code, Ptr<Ipv4Route> route);
  void Forward (Ipv4Address source, Icmpv4Header icmp,
                uint32_t info, Ipv4Header ipHeader,
                const uint8_t payload[8]);

  virtual void DoDispose (void);

  Ptr<Node> m_node;
  IpL4Protocol::DownTargetCallback m_downTarget;
};

} // namespace ns3

#endif /* ICMPV4_L4_PROTOCOL_H */
