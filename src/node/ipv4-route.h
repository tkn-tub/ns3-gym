/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
#ifndef IPV4_ROUTE_H
#define IPV4_ROUTE_H

#include <list>
#include <vector>
#include <ostream>

#include "ipv4-address.h"

namespace ns3 {
/**
 * \brief A record of an IPv4 route
 */
class Ipv4Route {
public:
  /**
   * \brief This constructor does nothing
   */
  Ipv4Route ();

  /**
   * \brief Copy Constructor
   * \param route The route to copy
   */
  Ipv4Route (Ipv4Route const &route);

  /**
   * \brief Copy Constructor
   * \param route The route to copy
   */
  Ipv4Route (Ipv4Route const *route);

  bool IsHost (void) const;
  /**
   * \return The IPv4 address of the destination of this route
   */
  Ipv4Address GetDest (void) const;

  bool IsNetwork (void) const;
  Ipv4Address GetDestNetwork (void) const;
  Ipv4Mask GetDestNetworkMask (void) const;
  /**
   * \return True if this route is a default route; false otherwise
   */
  bool IsDefault (void) const;

  bool IsGateway (void) const;
  Ipv4Address GetGateway (void) const;

  uint32_t GetInterface (void) const;

  static Ipv4Route CreateHostRouteTo (Ipv4Address dest, 
				      Ipv4Address nextHop, 
				      uint32_t interface);
  static Ipv4Route CreateHostRouteTo (Ipv4Address dest, 
				      uint32_t interface);
  static Ipv4Route CreateNetworkRouteTo (Ipv4Address network, 
					 Ipv4Mask networkMask, 
					 Ipv4Address nextHop, 
					 uint32_t interface);
  static Ipv4Route CreateNetworkRouteTo (Ipv4Address network, 
					 Ipv4Mask networkMask, 
					 uint32_t interface);
  static Ipv4Route CreateDefaultRoute (Ipv4Address nextHop, 
				       uint32_t interface);
  
private:
  Ipv4Route (Ipv4Address network,
	     Ipv4Mask mask,
	     Ipv4Address gateway,
	     uint32_t interface);
  Ipv4Route (Ipv4Address dest,
	     Ipv4Mask mask,
	     uint32_t interface);
  Ipv4Route (Ipv4Address dest,
	     Ipv4Address gateway,
	     uint32_t interface);
  Ipv4Route (Ipv4Address dest,
	     uint32_t interface);

  Ipv4Address m_dest;
  Ipv4Mask m_destNetworkMask;
  Ipv4Address m_gateway;
  uint32_t m_interface;
};

std::ostream& operator<< (std::ostream& os, Ipv4Route const& route);

/**
 * \brief A record of an IPv4 multicast route
 */
class Ipv4MulticastRoute {
public:
  /**
   * \brief This constructor does nothing
   */
  Ipv4MulticastRoute ();

  /**
   * \brief Copy Constructor
   * \param route The route to copy
   */
  Ipv4MulticastRoute (Ipv4MulticastRoute const &route);

  /**
   * \brief Copy Constructor
   * \param route The route to copy
   */
  Ipv4MulticastRoute (Ipv4MulticastRoute const *route);

  /**
   * \return The IPv4 address of the source of this route
   */
  Ipv4Address GetOrigin (void) const;

  /**
   * \return The IPv4 address of the multicast group of this route
   */
  Ipv4Address GetGroup (void) const;

  /**
   * \return The IPv4 address of the input interface of this route
   */
  uint32_t GetInputInterface (void) const;

  /**
   * \return The number of output interfaces of this route
   */
  uint32_t GetNOutputInterfaces (void) const;

  /**
   * \return A specified output interface.
   */
  uint32_t GetOutputInterface (uint32_t n) const;

  /**
   * \return A vector of all of the output interfaces of this route.
   */
  std::vector<uint32_t> GetOutputInterfaces (void) const;

  static Ipv4MulticastRoute CreateMulticastRoute (Ipv4Address origin, 
    Ipv4Address group, uint32_t inputInterface,
    std::vector<uint32_t> outputInterfaces);

private:
  Ipv4MulticastRoute (Ipv4Address origin, Ipv4Address group, 
    uint32_t inputInterface, std::vector<uint32_t> outputInterfaces);

  Ipv4Address m_origin;
  Ipv4Address m_group;
  uint32_t m_inputInterface;
  std::vector<uint32_t> m_outputInterfaces;
};

std::ostream& operator<< (std::ostream& os, Ipv4MulticastRoute const& route);

}//namespace ns3

#endif /* IPV4_ROUTE_H */
