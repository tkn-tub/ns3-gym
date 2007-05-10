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
#include "ns3/composite-trace-resolver.h"
#include "ns3/node.h"
#include "ns3/net-device.h"

#include "arp-ipv4-interface.h"
#include "i-arp-private.h"
#include "ipv4.h"

namespace ns3 {

ArpIpv4Interface::ArpIpv4Interface (Ptr<Node> node, NetDevice *device)
  : Ipv4Interface (device),
    m_node (node)
{}
ArpIpv4Interface::~ArpIpv4Interface ()
{}

TraceResolver *
ArpIpv4Interface::DoCreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  if (PeekDevice () != 0)
    {
      resolver->Add ("netdevice",
                     MakeCallback (&NetDevice::CreateTraceResolver, PeekDevice ()),
                     ArpIpv4Interface::NETDEVICE);
    }
  
  return resolver;
}

void 
ArpIpv4Interface::SendTo (Packet p, Ipv4Address dest)
{
  NS_ASSERT (PeekDevice () != 0);
  if (PeekDevice ()->NeedsArp ())
    {
      IArpPrivate * arp = m_node->QueryInterface<IArpPrivate> (IArpPrivate::iid);
      MacAddress hardwareDestination;
      bool found = arp->Lookup (p, dest, PeekDevice (), &hardwareDestination);
      if (found)
        {
          PeekDevice ()->Send (p, hardwareDestination, Ipv4::PROT_NUMBER);
        }
      arp->Unref ();
    }
  else
    {
      PeekDevice ()->Send (p, PeekDevice ()->GetBroadcast (), Ipv4::PROT_NUMBER);
    }
}

}//namespace ns3
