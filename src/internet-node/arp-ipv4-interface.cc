/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Authors: 
 *  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>,
 */

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/composite-trace-resolver.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/address.h"

#include "arp-ipv4-interface.h"
#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"

NS_LOG_COMPONENT_DEFINE ("ArpIpv4Interface");

namespace ns3 {

ArpIpv4Interface::ArpIpv4Interface (Ptr<Node> node, Ptr<NetDevice> device)
  : Ipv4Interface (device),
    m_node (node)
{
  NS_LOG_FUNCTION;
}

ArpIpv4Interface::~ArpIpv4Interface ()
{
  NS_LOG_FUNCTION;
}

Ptr<TraceResolver>
ArpIpv4Interface::GetTraceResolver (void) const
{
  NS_LOG_FUNCTION;
  Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
  if (GetDevice () != 0)
    {
      resolver->AddComposite ("netdevice", GetDevice ());
    }
  resolver->SetParentResolver (Ipv4Interface::GetTraceResolver ());
  return resolver;
}

void 
ArpIpv4Interface::SendTo (Ptr<Packet> p, Ipv4Address dest)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << p << ", " << dest << ")");

  NS_ASSERT (GetDevice () != 0);
  if (GetDevice ()->NeedsArp ())
    {
      NS_LOG_LOGIC ("Needs ARP");
      Ptr<ArpL3Protocol> arp = 
        m_node->QueryInterface<ArpL3Protocol> (ArpL3Protocol::iid);
      Address hardwareDestination;
      bool found;
      
      if (dest.IsBroadcast () || 
          dest.IsSubnetDirectedBroadcast (GetNetworkMask ()) )
        {
          NS_LOG_LOGIC ("IsBroadcast");
          hardwareDestination = GetDevice ()->GetBroadcast ();
          found = true;
        }
      else if (dest.IsMulticast ())
        {
          NS_LOG_LOGIC ("IsMulticast");
          NS_ASSERT_MSG(GetDevice ()->IsMulticast (),
            "ArpIpv4Interface::SendTo (): Sending multicast packet over "
            "non-multicast device");

          hardwareDestination = GetDevice ()->MakeMulticastAddress(dest);
          found = true;
        }
      else
        {
          NS_LOG_LOGIC ("ARP Lookup");
          found = arp->Lookup (p, dest, GetDevice (), &hardwareDestination);
        }

      if (found)
        {
          NS_LOG_LOGIC ("Address Resolved.  Send.");
          GetDevice ()->Send (p, hardwareDestination, 
            Ipv4L3Protocol::PROT_NUMBER);
        }
    }
  else
    {
      NS_LOG_LOGIC ("Doesn't need ARP");
      GetDevice ()->Send (p, GetDevice ()->GetBroadcast (), 
        Ipv4L3Protocol::PROT_NUMBER);
    }
}

}//namespace ns3
