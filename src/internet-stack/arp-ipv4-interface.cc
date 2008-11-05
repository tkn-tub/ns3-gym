/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/address.h"
#include "ns3/pointer.h"

#include "arp-ipv4-interface.h"
#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"
#include "arp-cache.h"

NS_LOG_COMPONENT_DEFINE ("ArpIpv4Interface");

namespace ns3 {

TypeId 
ArpIpv4Interface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ArpIpv4Interface")
    .SetParent<Ipv4Interface> ()
    .AddAttribute ("ArpCache",
                   "The arp cache for this ipv4 interface",
                   PointerValue (0),
                   MakePointerAccessor (&ArpIpv4Interface::m_cache),
                   MakePointerChecker<ArpIpv4Interface> ())
    ;
  return tid;
}

ArpIpv4Interface::ArpIpv4Interface ()
  : m_node (0),
    m_device (0)
{
  NS_LOG_FUNCTION (this);
}

ArpIpv4Interface::~ArpIpv4Interface ()
{
  NS_LOG_FUNCTION (this);
}

void 
ArpIpv4Interface::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_device = 0;
  m_cache = 0;
  Ipv4Interface::DoDispose ();
}

void 
ArpIpv4Interface::SetNode (Ptr<Node> node)
{
  m_node = node;
  DoSetup ();
}
void 
ArpIpv4Interface::SetDevice (Ptr<NetDevice> device)
{
  m_device = device;
  DoSetup ();
}

Ptr<NetDevice> 
ArpIpv4Interface::GetDevice (void) const
{
  return m_device;
}

void
ArpIpv4Interface::DoSetup (void)
{
  if (m_node == 0 || m_device == 0)
    {
      return;
    }
  Ptr<ArpL3Protocol> arp = m_node->GetObject<ArpL3Protocol> ();
  m_cache = arp->CreateCache (m_device, this);
}

void 
ArpIpv4Interface::SendTo (Ptr<Packet> p, Ipv4Address dest)
{
  NS_LOG_FUNCTION (this << p << dest);

  NS_ASSERT (GetDevice () != 0);
  if (dest == GetAddress ())
    {
      Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
        
      ipv4->Receive (0, p, Ipv4L3Protocol::PROT_NUMBER, 
                     GetDevice ()->GetBroadcast (),
                     GetDevice ()->GetBroadcast (),
                     NetDevice::PACKET_HOST // note: linux uses PACKET_LOOPBACK here
                     );
      return;
    }
  if (m_device->NeedsArp ())
    {
      NS_LOG_LOGIC ("Needs ARP");
      Ptr<ArpL3Protocol> arp = 
        m_node->GetObject<ArpL3Protocol> ();
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

          hardwareDestination = GetDevice ()->GetMulticast(dest);
          found = true;
        }
      else
        {
          NS_LOG_LOGIC ("ARP Lookup");
          found = arp->Lookup (p, dest, GetDevice (), m_cache, &hardwareDestination);
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
