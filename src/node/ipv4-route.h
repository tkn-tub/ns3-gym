/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
#ifndef IPV4_ROUTE_H
#define IPV4_ROUTE_H

#include <list>
#include <ostream>

#include "ipv4-address.h"

namespace ns3 {

class Ipv4Route {
public:
  Ipv4Route ();
  Ipv4Route (Ipv4Route const &route);

  bool IsHost (void) const;
  Ipv4Address GetDest (void) const;

  bool IsNetwork (void) const;
  Ipv4Address GetDestNetwork (void) const;
  Ipv4Mask GetDestNetworkMask (void) const;

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

}//namespace ns3

#endif /* IPV4_ROUTE_H */
