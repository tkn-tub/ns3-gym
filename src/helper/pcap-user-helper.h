/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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

#ifndef PCAP_USER_HELPER_H
#define PCAP_USER_HELPER_H

#include <string>
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"

namespace ns3 {

/**
 * \brief Base class providing common pcap operations.
 */
class PcapUserHelper
{
public:
  /**
   * @brief Enable pcap output the indicated net device.
   * @internal
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param nd Net device for which you want to enable tracing.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  virtual void EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous) = 0;

  /**
   * @brief Enable pcap output the indicated net device.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param nd Net device for which you want to enable tracing.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, Ptr<NetDevice> nd, bool promiscuous = false);

  /**
   * @brief Enable pcap output the indicated net device using a device previously
   * named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for pcap files.
   * @param ndName The name of the net device in which you want to enable tracing.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, std::string ndName, bool promiscuous = false);

  /**
   * @brief Enable pcap output on each device in the container which is of the 
   * appropriate type.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param d container of devices of type ns3::CsmaNetDevice
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, NetDeviceContainer d, bool promiscuous = false);

  /**
   * @brief Enable pcap output on each device (which is of the appropriate type)
   * in the nodes provided in the container.
   *
   * \param prefix Filename prefix to use for pcap files.
   * \param n container of nodes.
   * \param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, NodeContainer n, bool promiscuous = false);

  /**
   * @brief Enable pcap output on each device (which is of the appropriate type)
   * in the set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcapAll (std::string prefix, bool promiscuous = false);

  /**
   * @brief Enable pcap output on the device specified by a global node-id (of
   * a previously created node) and associated device-id.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, uint32_t nodeid, uint32_t deviceid, bool promiscuous = false);
};

} // namespace ns3

#endif // PCAP_USER_HELPER_H
