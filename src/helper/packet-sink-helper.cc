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
#include "packet-sink-helper.h"
#include "ns3/string.h"
#include "ns3/inet-socket-address.h"

namespace ns3 {

PacketSinkHelper::PacketSinkHelper (std::string protocol, Address address)
{
  m_factory.SetTypeId ("ns3::PacketSink");
  m_factory.Set ("Protocol", StringValue (protocol));
  m_factory.Set ("Local", AddressValue (address));
}

void 
PacketSinkHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}


#if 0
void 
PacketSinkHelper::SetUdpLocal (Ipv4Address ip, uint16_t port)
{
  m_factory.Set ("Protocol", String ("ns3::UdpSocketFactory"));
  m_factory.Set ("Local", Address (InetSocketAddress (ip, port)));
}
void 
PacketSinkHelper::SetTcpLocal (Ipv4Address ip, uint16_t port)
{
  m_factory.Set ("Protocol", String ("ns3::Tcp"));
  m_factory.Set ("Local", Address (InetSocketAddress (ip, port)));
}
#endif

ApplicationContainer 
PacketSinkHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Application> app = m_factory.Create<Application> ();
      node->AddApplication (app);
      apps.Add (app);
    }
  return apps;
}


} // namespace ns3
