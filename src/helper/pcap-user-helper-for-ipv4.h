/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Washington
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
 */

#ifndef PCAP_USER_HELPER_FOR_IPV4_H
#define PCAP_USER_HELPER_FOR_IPV4_H

#include <string>
#include "ns3/ipv4-interface-container.h"
#include "ns3/node-container.h"

namespace ns3 {

/**
 * \brief Base class providing common user-level pcap operations for helpers
 * representing IPv4 protocols .
 */
class PcapUserHelperForIpv4
{
public:
  /**
   * @brief Enable pcap output the indicated Ipv4 and interface pair.
   * @internal
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface Interface on ipv4 on which you want to enable tracing.
   */
  virtual void EnablePcapInternal (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface) = 0;

  /**
   * @brief Enable pcap output the indicated Ipv4 and interface pair.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface Interface on ipv4 on which you want to enable tracing.
   */
  void EnablePcap (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface);

  /**
   * @brief Enable pcap output the indicated Ipv4 and interface pair using a
   * Ptr<Ipv4> previously named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for pcap files.
   * @param ipv4Name Name of the Ptr<Ipv4> on which you want to enable tracing.
   * @param interface Interface on ipv4 on which you want to enable tracing.
   */
  void EnablePcap (std::string prefix, std::string ipv4Name, uint32_t interface);

  /**
   * @brief Enable pcap output on each Ipv4 and interface pair in the container.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param c Ipv4InterfaceContainer of Ipv4 and interface pairs
   */
  void EnablePcap (std::string prefix, Ipv4InterfaceContainer c);

  /**
   * @brief Enable pcap output on all Ipv4 and interface pairs existing in the
   * nodes provided in the container.
   *
   * \param prefix Filename prefix to use for pcap files.
   * \param n container of nodes.
   */
  void EnablePcap (std::string prefix, NodeContainer n);

  /**
   * @brief Enable pcap output on all Ipv4 and interface pairs existing in the 
   * set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for pcap files.
   */
  void EnablePcapAll (std::string prefix);

  /**
   * @brief Enable pcap output on the Ipv4 and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ipv4 aggregated to a node, the node-id unambiguously 
   * determines the Ipv4.
   *
   * @param prefix Filename prefix to use for pcap files.
   */
  void EnablePcap (std::string prefix, uint32_t nodeid, uint32_t interface);
};

} // namespace ns3

#endif // PCAP_USER_HELPER_FOR_IPV4_H
