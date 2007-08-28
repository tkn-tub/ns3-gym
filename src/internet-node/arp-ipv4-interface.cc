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
#include "ns3/debug.h"
#include "ns3/composite-trace-resolver.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/address.h"

#include "arp-ipv4-interface.h"
#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"

NS_DEBUG_COMPONENT_DEFINE ("ArpIpv4Interface");

namespace ns3 {

ArpIpv4Interface::ArpIpv4Interface (Ptr<Node> node, Ptr<NetDevice> device)
  : Ipv4Interface (device),
    m_node (node)
{
  NS_DEBUG ("ArpIpv4Interface::ArpIpv4Interface ()");
}

ArpIpv4Interface::~ArpIpv4Interface ()
{
  NS_DEBUG ("ArpIpv4Interface::~ArpIpv4Interface ()");
}

TraceResolver *
ArpIpv4Interface::DoCreateTraceResolver (TraceContext const &context)
{
  NS_DEBUG ("ArpIpv4Interface::DoCreateTraceResolver ()");
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  if (GetDevice () != 0)
    {
      resolver->Add ("netdevice",
                     MakeCallback (&NetDevice::CreateTraceResolver, PeekPointer (GetDevice ())));
    }
  
  return resolver;
}

//
// RFC 1112 says that an IP host group address is mapped to an Ethernet 
// multicast address by placing the low-order 23-bits of the IP address into 
// the low-order 23 bits of the Ethernet multicast address 01-00-5E-00-00-00 
// (hex).  These are completely different animals and they're encapsulated
// very nicely.  Translation: This isn't going to be very pretty.
//
Address
ArpIpv4Interface::MakeMulticastAddress(Ipv4Address multicastGroup)
{
  NS_DEBUG ("ArpIpv4Interface::MakeMulticastAddress (" << multicastGroup <<
    ")");
//
// First, get the generic multicast address from the device.  Since it is 
// connected to this object, and this object is an IPV4 stack, we hope that
// it is really an Eui48Address.  If it's not, then we don't know what to do.
//
  Address hardwareDestination = GetDevice ()->GetMulticast ();

  NS_DEBUG ("ArpIpv4Interface::MakeMulticastAddress (): "
    "Device multicast address: " << hardwareDestination);

  Eui48Address etherAddr = Eui48Address::ConvertFrom (hardwareDestination);
//
// We now have the multicast address in an abstract 48-bit container.  We 
// need to pull it out so we can play with it.  When we're done, we have the 
// high order bits in etherBuffer[0], etc.
//
  uint8_t etherBuffer[6];
  etherAddr.CopyTo (etherBuffer);
//
// If the device is playing the game correctly, the low order 23 bits of the
// multicast base address will be zero.
//
  NS_ASSERT_MSG((etherBuffer[4] & 0x7f) == 0,
    "ArpIpv4Interface::SendTo ():  Expected low order bits zeroed");
  NS_ASSERT_MSG(etherBuffer[5] == 0, 
    "ArpIpv4Interface::SendTo ():  Expected low order bits zeroed");
  NS_ASSERT_MSG(etherBuffer[6] == 0, 
    "ArpIpv4Interface::SendTo ():  Expected low order bits zeroed");
//
// Now we need to pull the raw bits out of the Ipv4 destination address.
//
  uint8_t ipBuffer[4];
  multicastGroup.Serialize (ipBuffer);
//
// We need to place the low order 23 bits of the IP address into the low order
// 23 bits of the ethernet address.
//
  etherBuffer[3] |= ipBuffer[1] & 0x7f;
  etherBuffer[4] = ipBuffer[2];
  etherBuffer[5] = ipBuffer[3];
//
// Now, etherBuffer has the desired ethernet multicast address.  We have to
// suck these bits back into the Eui48Address; and then suck those bits back
// into the abstract hardwareAddress.
//
  etherAddr.CopyFrom (etherBuffer);
//
// Implicit conversion (operator Address ()) is defined for Eui48Address, so
// use it.
//
  NS_DEBUG ("ArpIpv4Interface::MakeMulticastAddress (): "
    "multicast address is " << etherAddr);

  return etherAddr;
}

void 
ArpIpv4Interface::SendTo (Packet p, Ipv4Address dest)
{
  NS_DEBUG ("ArpIpv4Interface::SendTo (" << &p << ", " << dest << ")");

  NS_ASSERT (GetDevice () != 0);
  if (GetDevice ()->NeedsArp ())
    {
      NS_DEBUG ("ArpIpv4Interface::SendTo (): Needs ARP");
      Ptr<ArpL3Protocol> arp = 
        m_node->QueryInterface<ArpL3Protocol> (ArpL3Protocol::iid);
      Address hardwareDestination;
      bool found;

      if (dest.IsBroadcast ())
        {
          NS_DEBUG ("ArpIpv4Interface::SendTo (): IsBroadcast");
          hardwareDestination = GetDevice ()->GetBroadcast ();
          found = true;
        }
      else if (dest.IsMulticast ())
        {
          NS_DEBUG ("ArpIpv4Interface::SendTo (): IsMulticast");
          NS_ASSERT_MSG(GetDevice ()->IsMulticast (),
            "ArpIpv4Interface::SendTo (): Sending multicast packet over "
            "non-multicast device");

          hardwareDestination = MakeMulticastAddress(dest);
          found = true;
        }
      else
        {
          NS_DEBUG ("ArpIpv4Interface::SendTo (): ARP Lookup");
          found = arp->Lookup (p, dest, GetDevice (), &hardwareDestination);
        }

      if (found)
        {
          NS_DEBUG ("ArpIpv4Interface::SendTo (): Address Resolved.  Send.");
          GetDevice ()->Send (p, hardwareDestination, 
            Ipv4L3Protocol::PROT_NUMBER);
        }
    }
  else
    {
      NS_DEBUG ("ArpIpv4Interface::SendTo (): Doesn't need ARP");
      GetDevice ()->Send (p, GetDevice ()->GetBroadcast (), 
        Ipv4L3Protocol::PROT_NUMBER);
    }
}

}//namespace ns3
