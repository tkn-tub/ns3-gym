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
 * \brief aggregate IP/TCP/UDP functionality to existing Nodes.
 */
class InternetStackHelper
{
public:
  InternetStackHelper(void);

  /**
   * Aggregate implementations of the ns3::Ipv4, ns3::Udp, and ns3::Tcp classes
   * onto the provided node.  This method will assert if called on a node that 
   * already has an Ipv4 object aggregated to it.
   * 
   * \param node The node on which to install the stack.
   */
  void Install (Ptr<Node> node) const;

  /**
   * For each node in the input container, aggregate implementations of the 
   * ns3::Ipv4, ns3::Udp, and, ns3::Tcp classes.  The program will assert 
   * if this method is called on a container with a node that already has
   * an Ipv4 object aggregated to it.
   * 
   * \param c NodeContainer that holds the set of nodes on which to install the
   * new stacks.
   */
  void Install (NodeContainer c) const;

  /**
   * \brief Enable or disable use of the Network Simulation Cradle stack.  
   *
   * Give the NSC stack a shared library file name to use when creating the 
   * statck implementation.  By providing a non-empty string as a parameter, you
   * select the NSC version of the stack.  By providing an empty string, you 
   * select the ns-3 default version.
   *
   * \param soname name of the shared library with the nsc tcp stack
   * to use, e.g. 'liblinux2.6.26.so'.
   */
  void SetNscStack(std::string soname);

  /**
   * \param os output stream
   * \param n node container
   *
   * Enable ascii output on these drop traces, for each node in the NodeContainer..
   * /NodeList/[i]/$ns3ArpL3Protocol/Drop 
   * /NodeList/[i]/$ns3Ipv4L3Protocol/Drop 
   */
  static void EnableAscii (std::ostream &os, NodeContainer n);

  /**
   * \param os output stream
   *
   * Enable ascii output on these drop traces, for all nodes.
   * /NodeList/[i]/$ns3ArpL3Protocol/Drop 
   * /NodeList/[i]/$ns3Ipv4L3Protocol/Drop 
   */
  static void EnableAsciiAll (std::ostream &os);

  /**
   * Enable pcap output on each protocol instance which is of the
   * ns3::Ipv4L3Protocol type.  Both Tx and Rx events will be logged.
   *
   * \param filename filename prefix to use for pcap files.
   *
   * \warning If you perform multiple simulations in a single script,
   * each iteration of the simulation will result in the trace files
   * being overwritten.  We don't attempt to anticipate what a user
   * might actually want to do, so we leave it up to them.  If you want
   * to save any particular data, do so manually at inter-simulation 
   * time.
   */
  static void EnablePcapAll (std::string filename);

private:
  std::string m_nscLibrary;
  static void Cleanup (void);
  static void LogRxIp (std::string context, Ptr<const Packet> packet, uint32_t deviceId);
  static void LogTxIp (std::string context, Ptr<const Packet> packet, uint32_t deviceId);
  static Ptr<PcapWriter> GetStream (uint32_t nodeId, uint32_t interfaceId);
  struct Trace {
    uint32_t nodeId;
    uint32_t interfaceId;
    Ptr<PcapWriter> writer;
  };
  static void AsciiDropEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static std::string m_pcapBaseFilename;
  static uint32_t GetNodeIndex (std::string context);
  static std::vector<Trace> m_traces;
};

} // namespace ns3

#endif /* INTERNET_STACK_HELPER_H */
