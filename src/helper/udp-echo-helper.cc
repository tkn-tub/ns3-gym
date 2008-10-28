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
#include "udp-echo-helper.h"
#include "ns3/udp-echo-server.h"
#include "ns3/udp-echo-client.h"
#include "ns3/uinteger.h"

namespace ns3 {

UdpEchoServerHelper::UdpEchoServerHelper (uint16_t port)
{
  m_factory.SetTypeId (UdpEchoServer::GetTypeId ());
  SetAttribute ("Port", UintegerValue(port));
}

void 
UdpEchoServerHelper::SetAttribute (
  std::string name, 
  const AttributeValue &value)
{
  m_factory.Set (name, value);
}


ApplicationContainer 
UdpEchoServerHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<UdpEchoServer> server = m_factory.Create<UdpEchoServer> ();
      node->AddApplication (server);
      apps.Add (server);
    }
  return apps;
}

UdpEchoClientHelper::UdpEchoClientHelper (Ipv4Address address, uint16_t port)
{
  m_factory.SetTypeId (UdpEchoClient::GetTypeId ());
  SetAttribute ("RemoteAddress", Ipv4AddressValue (address));
  SetAttribute ("RemotePort", UintegerValue (port));
}

void 
UdpEchoClientHelper::SetAttribute (
  std::string name, 
  const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer 
UdpEchoClientHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<UdpEchoClient> client = m_factory.Create<UdpEchoClient> ();
      node->AddApplication (client);
      apps.Add (client);
    }
  return apps;  
}

} // namespace ns3
