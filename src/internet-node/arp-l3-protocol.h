/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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
#include "ns3/address.h"
#include "ns3/ptr.h"

namespace ns3 {

class ArpCache;
class NetDevice;
class Node;
class Packet;
class TraceResolver;
class TraceContext;
/**
 * \brief An implementation of the ARP protocol
 */
class ArpL3Protocol : public Object
{
public:
  static const InterfaceId iid;
  static const uint16_t PROT_NUMBER;
  /**
   * \brief Constructor
   * \param node The node which this ARP object is associated with
   */
  ArpL3Protocol (Ptr<Node> node);
  virtual ~ArpL3Protocol ();
  /**
   * \brief Recieve a packet
   */
  void Receive(Ptr<NetDevice> device, const Packet& p, uint16_t protocol, const Address &from);
  /**
   * \brief Perform an ARP lookup
   * \param p
   * \param destination
   * \param device
   * \param hardwareDestination
   * \return 
   */
  bool Lookup (Packet &p, Ipv4Address destination, 
	       Ptr<NetDevice> device,
	       Address *hardwareDestination);
protected:
  virtual void DoDispose (void);
private:
  typedef std::list<ArpCache *> CacheList;
  ArpCache *FindCache (Ptr<NetDevice> device);
  void SendArpRequest (ArpCache const *cache, Ipv4Address to);
  void SendArpReply (ArpCache const *cache, Ipv4Address toIp, Address toMac);
  CacheList m_cacheList;
  Ptr<Node> m_node;
};

}//namespace ns3


#endif /* ARP_L3_PROTOCOL_H */
