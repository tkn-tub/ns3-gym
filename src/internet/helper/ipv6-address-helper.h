/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008-2009 Strasbourg University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#ifndef IPV6_ADDRESS_STATIC_HELPER_H
#define IPV6_ADDRESS_STATIC_HELPER_H

#include <vector>

#include "ns3/ipv6-address.h"

#include "ns3/net-device-container.h"
#include "ipv6-interface-container.h"

namespace ns3
{

/**
 * \class Ipv6AddressHelper
 * \brief Helper class to assign IPv6 address statically.
 */
class Ipv6AddressHelper
{
public:
  /**
   * \brief Constructor.
   */
  Ipv6AddressHelper ();

  /**
   * \brief Allocate a new network.
   * \param network The IPv6 network
   * \param prefix The prefix
   */
  void NewNetwork (Ipv6Address network, Ipv6Prefix prefix);

  /**
   * \brief Allocate a new address.
   * \param addr L2 address (currently only ethernet address is supported)
   * \return newly created Ipv6Address
   */
  Ipv6Address NewAddress (Address addr);

  /**
   * \brief Allocate an Ipv6InterfaceContainer.
   * \param c netdevice container
   * \return newly created Ipv6InterfaceContainer
   */
  Ipv6InterfaceContainer Assign (const NetDeviceContainer &c);

  /**
   * \brief Allocate an Ipv6InterfaceContainer.
   * \param c netdevice container
   * \param withConfiguration true : interface statically configured, false : no static configuration
   * \return newly created Ipv6InterfaceContainer
   */
  Ipv6InterfaceContainer Assign (const NetDeviceContainer &c, std::vector<bool> withConfiguration);

  /**
   * \brief Allocate an Ipv6InterfaceContainer without static IPv6 configuration.
   * \param c netdevice container
   * \return newly created Ipv6InterfaceContainer
   */
  Ipv6InterfaceContainer AssignWithoutAddress (const NetDeviceContainer &c);

private:
  /**
   * \internal
   * \brief The IPv6 network.
   */
  Ipv6Address m_network;

  /**
   * \internal
   * \brief IPv6 The prefix (mask).
   */
  Ipv6Prefix m_prefix;
};

} /* namespace ns3 */

#endif /* IPV6_ADDRESS_STATIC_HELPER_H */

