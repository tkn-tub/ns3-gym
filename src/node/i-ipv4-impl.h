/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef I_IPV4_IMPL_H
#define I_IPV4_IMPL_H

#include "i-ipv4.h"

namespace ns3 {

class Ipv4;

class IIpv4Impl : public IIpv4
{
public:
  IIpv4Impl (Ipv4 *ipv4);

  virtual ~IIpv4Impl ();

  virtual void AddHostRouteTo (Ipv4Address dest, 
			       Ipv4Address nextHop, 
			       uint32_t interface);
  virtual void AddHostRouteTo (Ipv4Address dest, 
			       uint32_t interface);
  virtual void AddNetworkRouteTo (Ipv4Address network, 
				  Ipv4Mask networkMask, 
				  Ipv4Address nextHop, 
				  uint32_t interface);
  virtual void AddNetworkRouteTo (Ipv4Address network, 
				  Ipv4Mask networkMask, 
				  uint32_t interface);
  virtual void SetDefaultRoute (Ipv4Address nextHop, 
				uint32_t interface);
  virtual uint32_t GetNRoutes (void);
  virtual Ipv4Route *GetRoute (uint32_t i);
  virtual void RemoveRoute (uint32_t i);
  virtual uint32_t AddInterface (NetDevice *device);
  virtual uint32_t GetNInterfaces (void);  

  virtual void SetAddress (uint32_t i, Ipv4Address address);
  virtual void SetNetworkMask (uint32_t i, Ipv4Mask mask);
  virtual Ipv4Mask GetNetworkMask (uint32_t t) const;
  virtual Ipv4Address GetAddress (uint32_t i) const;
  virtual uint16_t GetMtu (uint32_t i) const;
  virtual bool IsUp (uint32_t i) const;
  virtual void SetUp (uint32_t i);
  virtual void SetDown (uint32_t i);
protected:
  virtual void DoDispose (void);
private:
  Ipv4 *m_ipv4;
};

} // namespace ns3

#endif /* I_IPV4_IMPL_H */
