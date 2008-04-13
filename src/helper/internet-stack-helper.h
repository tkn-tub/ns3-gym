/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#ifndef INTERNET_STACK_HELPER_H
#define INTERNET_STACK_HELPER_H

#include "node-container.h"
#include "net-device-container.h"
#include "ns3/pcap-writer.h"
#include "ns3/packet.h"

namespace ns3 {

/**
 * \brief aggregate ip/tcp/udp functionality to existing Nodes.
 */
class InternetStackHelper
{
public:
  /**
   * \param c the set of nodes
   *
   * For each node in the input container, aggregate implementations
   * of the ns3::Ipv4, ns3::Udp, and, ns3::Tcp classes.  The program
   * will assert if this method is called on a container with a node
   * that already has an Ipv4 object aggregated to it.
   * 
   */
  void Install (NodeContainer c);

  /**
   * \param filename filename prefix to use for pcap files.
   *
   * Enable pcap output on each protocol instance which is of the
   * ns3::Ipv4L3Protocol type.  Both Tx and Rx events will be logged.
   */
  static void EnablePcap (std::string filename);

private:
  static void LogRxIp (std::string context, Ptr<const Packet> packet, uint32_t deviceId);
  static void LogTxIp (std::string context, Ptr<const Packet> packet, uint32_t deviceId);
  static Ptr<PcapWriter> GetStream (uint32_t nodeId, uint32_t interfaceId);
  struct Trace {
    uint32_t nodeId;
    uint32_t interfaceId;
    Ptr<PcapWriter> writer;
  };
  static std::string m_pcapBaseFilename;
  static uint32_t GetNodeIndex (std::string context);
  static std::vector<Trace> m_traces;
};

} // namespace ns3

#endif /* INTERNET_STACK_HELPER_H */
