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

#ifndef IPV6_INTERFACE_CONTAINER_H
#define IPV6_INTERFACE_CONTAINER_H

#include <stdint.h>

#include <vector>

#include "ns3/ipv6.h"
#include "ns3/ipv6-address.h"

namespace ns3
{

/**
 * \class Ipv6InterfaceContainer
 * \brief Keep track of a set of IPv6 interfaces.
 */
class Ipv6InterfaceContainer
{
public:
  /**
   * \brief Constructor.
   */
  Ipv6InterfaceContainer ();

  /**
   * \brief Get the number of interfaces.
   * \return the number of interfaces stored in this Ipv6InterfaceContainer.
   */
  uint32_t GetN (void) const;

  /**
   * \brief Get the interface index for the specified node index.
   * \param i index of the node
   * \return interface index
   */
  uint32_t GetInterfaceIndex (uint32_t i) const;

  /**
   * \brief Get the address for the specified index.
   * \param i interface index
   * \param j address index, generally index 0 is the link-local address
   * \return IPv6 address
   */
  Ipv6Address GetAddress (uint32_t i, uint32_t j) const;

  /**
   * \brief Add a couple IPv6/interface.
   * \param ipv6 IPv6 address
   * \param interface interface index
   */
  void Add (Ptr<Ipv6> ipv6, uint32_t interface);

  /**
   * \brief Fusion with another Ipv6InterfaceContainer.
   * \param c container
   */
  void Add (Ipv6InterfaceContainer& c);

  /**
   * \brief Add a couple of name/interface.
   * \param ipv6Name name of a node
   * \param interface interface index to add
   */
  void Add (std::string ipv6Name, uint32_t interface);

  /**
   * \brief Set the state of the stack (act as a router or not) for the specified index.
   * \param i index
   * \param router true : is a router, false : is an host
   */
  void SetRouter (uint32_t i, bool router);

  /**
   * \brief Set the default route for the specified index.
   * \param i index
   * \param router the default router
   */
  void SetDefaultRoute (uint32_t i, uint32_t router);

private:
  typedef std::vector<std::pair<Ptr<Ipv6>, uint32_t> > InterfaceVector;

  /**
   * \internal
   * \brief List of IPv6 stack and interfaces index.
   */
  InterfaceVector m_interfaces;
};

} /* namespace ns3 */

#endif /* IPV6_INTERFACE_CONTAINER_H */

