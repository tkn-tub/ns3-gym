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
#ifndef UDP_ECHO_HELPER_H
#define UDP_ECHO_HELPER_H

#include <stdint.h>
#include "application-container.h"
#include "node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

class UdpEchoServerHelper
{
public:
  UdpEchoServerHelper ();
  void SetPort (uint16_t port);
  ApplicationContainer Build (NodeContainer c);
private:
  uint16_t m_port;
};

class UdpEchoClientHelper
{
public:
  UdpEchoClientHelper ();

  void SetRemote (Ipv4Address ip, uint16_t port);
  void SetAppAttribute (std::string name, Attribute value);
  ApplicationContainer Build (NodeContainer c);
 private:
  ObjectFactory m_factory;
  Ipv4Address m_remoteIp;
  uint16_t m_remotePort;
};


} // namespace ns3

#endif /* UDP_ECHO_HELPER_H */
