/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef ARP_L3_PROTOCOL_H
#define ARP_L3_PROTOCOL_H

#include <list>
#include "ns3/ipv4-address.h"
#include "ns3/net-device.h"
#include "ns3/address.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"

namespace ns3 {

class ArpCache;
class NetDevice;
class Node;
class Packet;
class Ipv4Interface;

/**
 * \ingroup internet
 * \defgroup arp Arp
 *
 * This is an overview of Arp capabilities (write me).
 */
/**
 * \ingroup arp
 * \brief An implementation of the ARP protocol
 */
class ArpL3Protocol : public Object
{
public:
  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  ArpL3Protocol ();
  virtual ~ArpL3Protocol ();

  void SetNode (Ptr<Node> node);

  Ptr<ArpCache> CreateCache (Ptr<NetDevice> device, Ptr<Ipv4Interface> interface);

  /**
   * \brief Receive a packet
   */
  void Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from, const Address &to,
                NetDevice::PacketType packetType);
  /**
   * \brief Perform an ARP lookup
   * \param p
   * \param destination
   * \param device
   * \param cache
   * \param hardwareDestination
   * \return 
   */
  bool Lookup (Ptr<Packet> p, Ipv4Address destination, 
               Ptr<NetDevice> device,
               Ptr<ArpCache> cache,
               Address *hardwareDestination);
protected:
  virtual void DoDispose (void);
  /*
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();
private:
  typedef std::list<Ptr<ArpCache> > CacheList;
  ArpL3Protocol (const ArpL3Protocol &o);
  ArpL3Protocol &operator = (const ArpL3Protocol &o);
  Ptr<ArpCache> FindCache (Ptr<NetDevice> device);
  void SendArpRequest (Ptr<const ArpCache>cache, Ipv4Address to);
  void SendArpReply (Ptr<const ArpCache> cache, Ipv4Address myIp, Ipv4Address toIp, Address toMac);
  CacheList m_cacheList;
  Ptr<Node> m_node;
  TracedCallback<Ptr<const Packet> > m_dropTrace;
};

} //namespace ns3


#endif /* ARP_L3_PROTOCOL_H */
