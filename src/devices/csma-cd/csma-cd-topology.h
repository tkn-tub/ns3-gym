/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2007 Emmanuelle Laprise
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
//
// Topology helper for multipoint channels in ns3.
// 
#ifndef CSMA_CD_TOPOLOGY_H
#define CSMA_CD_TOPOLOGY_H

#include "ns3/ptr.h"
#include "ns3/csma-cd-net-device.h"
#include "ns3/node.h"

// The topology class consists of only static methods thar are used to
// create the topology and data flows for an ns3 simulation

namespace ns3 {

class CsmaCdChannel;
class Node;
class DataRate;
class Queue;

/**
 * \brief A helper class to create CsmaCd Topologies 
 *
 * CsmaCd topologies are created based on the
 * ns3::CsmaCdNetDevice subclasses and ns3::CsmaCdChannel
 * objects.  This class uses the EthernetNetDevice and
 * PacketSocket classes in order to create logical connections between
 * net devices. The PacketSocket class generates the data and the
 * EthernetNetDevice class creates ethernet packets from the
 * data, filling in source and destination addresses. The
 * EthernetNetDevice class filters received data packets
 * according to its destination Mac addresses.
 */
class CsmaCdTopology {
public:
  /** 
   * \param dataRate Maximum transmission link rate 
   * \param delay propagation delay between any two nodes 
   * \return Pointer to the created CsmaCdChannel
   * 
   * Create a CsmaCdChannel. All nodes connected to a multipoint
   * channels will receive all packets written to that channel
   */
  static Ptr<CsmaCdChannel> CreateCsmaCdChannel(
    const DataRate& dataRate, const Time& delay);

#if 0
  /**
   * \param n1 Node to be attached to the multipoint channel
   * \param ch CsmaCdChannel to which node n1 should be attached 
   * \param addr MacAddress that should be assigned to the
   * EthernetNetDevice that will be added to the node.
   *
   * Add a multipoint node to a multipoint channel
   */
  static Ptr<CsmaCdNetDevice> AddCsmaCdEthernetNode(Ptr<Node> n1, 
                                                    Ptr<CsmaCdChannel> ch,
                                                    MacAddress addr);

  /**
   * \param app Application that will be sending data to the agent
   * \param ndSrc Net Device that will be sending the packets onto the
   * network
   * \param ndDest Net Device to which ndSrc will be sending the packets
   * \return A pointer to the PacketSocket
   *
   * Creates an PacketSocket and configure it to send packets between
   * two net devices
   */
static Ptr<PacketSocket> ConnectPacketSocket(Ptr<PacketSocketApp> app,
                                      Ptr<CsmaCdNetDevice> ndSrc,
                                      Ptr<CsmaCdNetDevice> ndDest);

  /**
   * \param app Application that will be sending data to the agent
   * \param ndSrc Net Device that will be sending the packets onto the
   * network 
   * \param macAddr Mac destination address for the packets send by
   * the ndSrc net device \return a Pointer to the created
   * PacketSocket
   *
   * Creates an PacketSocket and configure it to send packets from a
   * net device to a destination MacAddress
   */
static Ptr<PacketSocket> ConnectPacketSocket(Ptr<PacketSocketApp> app,
                                      Ptr<CsmaCdNetDevice> ndSrc,
                                      MacAddress macAddr);

  /**
   * \param n1 Node from which socketfactory should be tested.
   * \param iid_name Interface identifier ("Packet", in this case)
   *
   * This is a test function to make sure that a socket can be created
   * by using the socketfactory interface provided in the
   * netdevicenode.
   */
static  Ptr<Socket> CreatePacketSocket(Ptr<Node> n1, 
                                       std::string iid_name);
#endif

};
} // namespace ns3

#endif

