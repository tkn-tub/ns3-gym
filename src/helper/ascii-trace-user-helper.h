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

#ifndef ASCII_USER_HELPER_H
#define ASCII_USER_HELPER_H

#include <string>
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"

namespace ns3 {

/**
 * \brief Base class providing common ascii trace operations.
 */
class AsciiTraceUserHelper
{
public:
  /**
   * @brief Enable ascii trace output on the indicated net device.
   * @internal
   *
   * @param prefix Filename prefix to use for ascii trace files.
   * @param nd Net device for which you want to enable tracing.
   */
  virtual void EnableAsciiInternal (std::string prefix, Ptr<NetDevice> nd) = 0;

  /**
   * @brief Enable ascii trace output on the indicated net device.
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param nd Net device for which you want to enable tracing.
   */
  void EnableAscii (std::string prefix, Ptr<NetDevice> nd);

  /**
   * @brief Enable ascii trace output the indicated net device using a device 
   * previously named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for ascii files.
   * @param ndName The name of the net device in which you want to enable tracing.
   */
  void EnableAscii (std::string prefix, std::string ndName);

  /**
   * @brief Enable ascii trace output on each device in the container which is
   * of the appropriate type.
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param d container of devices of type ns3::CsmaNetDevice
   */
  void EnableAscii (std::string prefix, NetDeviceContainer d);

  /**
   * @brief Enable ascii trace output on each device (which is of the 
   * appropriate type) in the nodes provided in the container.
   *
   * \param prefix Filename prefix to use for ascii files.
   * \param n container of nodes.
   */
  void EnableAscii (std::string prefix, NodeContainer n);

  /**
   * @brief Enable ascii trace output on each device (which is of the
   * appropriate type) in the set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for ascii files.
   */
  void EnableAsciiAllXXX (std::string prefix);

  /**
   * @brief Enable ascii trace output on the device specified by a global 
   * node-id (of a previously created node) and associated device-id.
   *
   * @param prefix Filename prefix to use for ascii files.
   */
  void EnableAscii (std::string prefix, uint32_t nodeid, uint32_t deviceid);
};

} // namespace ns3

#endif // ASCII_USER_HELPER_H
