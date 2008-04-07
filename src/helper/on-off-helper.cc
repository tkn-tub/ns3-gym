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
#include "on-off-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-socket-address.h"
#include "ns3/string.h"

namespace ns3 {

OnOffHelper::OnOffHelper ()
{
  m_factory.SetTypeId ("ns3::OnOffApplication");
}

void 
OnOffHelper::SetUdpRemote (Ipv4Address ip, uint16_t port)
{
  m_factory.Set ("Protocol", String ("ns3::Udp"));
  m_factory.Set ("Remote", Address (InetSocketAddress (ip, port)));
}
void 
OnOffHelper::SetTcpRemote (Ipv4Address ip, uint16_t port)
{
  m_factory.Set ("Protocol", String ("ns3::Tcp"));
  m_factory.Set ("Remote", Address (InetSocketAddress (ip, port)));
}
void 
OnOffHelper::SetPacketRemote (Ptr<NetDevice> source, Address destination, uint16_t protocol)
{
  PacketSocketAddress packet;
  packet.SetSingleDevice (source->GetIfIndex ());
  packet.SetPhysicalAddress (destination);
  packet.SetProtocol (protocol);
  m_factory.Set ("Protocol", String ("ns3::PacketSocketFactory"));
  m_factory.Set ("Remote", Address (packet));
}
  
void 
OnOffHelper::SetAppAttribute (std::string name, Attribute value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
OnOffHelper::Install (NodeContainer c)
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
