/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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

#include "ipv4-interface.h"
#include "loopback-net-device.h"
#include "ns3/ipv4-address.h"
#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"
#include "arp-cache.h"
#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4Interface");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ipv4Interface);

TypeId 
Ipv4Interface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4Interface")
    .SetParent<Object> ()
    .AddAttribute ("ArpCache",
                   "The arp cache for this ipv4 interface",
                   PointerValue (0),
                   MakePointerAccessor (&Ipv4Interface::SetArpCache, 
                                        &Ipv4Interface::GetArpCache),
                   MakePointerChecker<ArpCache> ())
  ;
  ;
  return tid;
}

/** 
 * By default, Ipv4 interface are created in the "down" state
 *  with no IP addresses.  Before becoming useable, the user must 
 * invoke SetUp on them once an Ipv4 address and mask have been set.
 */
Ipv4Interface::Ipv4Interface () 
  : m_ifup (false),
    m_forwarding (true),
    m_metric (1),
    m_node (0), 
    m_device (0),
    m_cache (0)
{
  NS_LOG_FUNCTION (this);
}

Ipv4Interface::~Ipv4Interface ()
{
  NS_LOG_FUNCTION (this);
}

void
Ipv4Interface::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = 0;
  m_device = 0;
  Object::DoDispose ();
}

void 
Ipv4Interface::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
  DoSetup ();
}

void 
Ipv4Interface::SetDevice (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  m_device = device;
  DoSetup ();
}

void
Ipv4Interface::DoSetup (void)
{
  NS_LOG_FUNCTION (this);
  if (m_node == 0 || m_device == 0)
    {
      return;
    }
  if (!m_device->NeedsArp ())
    {
      return;
    }
  Ptr<ArpL3Protocol> arp = m_node->GetObject<ArpL3Protocol> ();
  m_cache = arp->CreateCache (m_device, this);
}

Ptr<NetDevice>
Ipv4Interface::GetDevice (void) const
{
  NS_LOG_FUNCTION (this);
  return m_device;
}

void
Ipv4Interface::SetMetric (uint16_t metric)
{
  NS_LOG_FUNCTION (this << metric);
  m_metric = metric;
}

uint16_t
Ipv4Interface::GetMetric (void) const
{
  NS_LOG_FUNCTION (this);
  return m_metric;
}

void
Ipv4Interface::SetArpCache (Ptr<ArpCache> a)
{
  NS_LOG_FUNCTION (this << a);
  m_cache = a;
}

Ptr<ArpCache>
Ipv4Interface::GetArpCache () const
{
  NS_LOG_FUNCTION (this);
  return m_cache;
}

/**
 * These are IP interface states and may be distinct from 
 * NetDevice states, such as found in real implementations
 * (where the device may be down but IP interface state is still up).
 */
bool 
Ipv4Interface::IsUp (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifup;
}

bool 
Ipv4Interface::IsDown (void) const
{
  NS_LOG_FUNCTION (this);
  return !m_ifup;
}

void 
Ipv4Interface::SetUp (void)
{
  NS_LOG_FUNCTION (this);
  m_ifup = true;
}

void 
Ipv4Interface::SetDown (void)
{
  NS_LOG_FUNCTION (this);
  m_ifup = false;
}

bool 
Ipv4Interface::IsForwarding (void) const
{
  NS_LOG_FUNCTION (this);
  return m_forwarding;
}

void 
Ipv4Interface::SetForwarding (bool val)
{
  NS_LOG_FUNCTION (this << val);
  m_forwarding = val;
}

void
Ipv4Interface::Send (Ptr<Packet> p, Ipv4Address dest)
{
  NS_LOG_FUNCTION (this << *p << dest);
  if (!IsUp ())
    {
      return;
    }
  // Check for a loopback device
  if (DynamicCast<LoopbackNetDevice> (m_device))
    {
      /// \todo additional checks needed here (such as whether multicast
      /// goes to loopback)?
      m_device->Send (p, m_device->GetBroadcast (), 
                      Ipv4L3Protocol::PROT_NUMBER);
      return;
    } 
  // is this packet aimed at a local interface ?
  for (Ipv4InterfaceAddressListCI i = m_ifaddrs.begin (); i != m_ifaddrs.end (); ++i)
    {
      if (dest == (*i).GetLocal ())
        {
          Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();

          ipv4->Receive (m_device, p, Ipv4L3Protocol::PROT_NUMBER, 
                         m_device->GetBroadcast (),
                         m_device->GetBroadcast (),
                         NetDevice::PACKET_HOST // note: linux uses PACKET_LOOPBACK here
                         );
          return;
        }
    }
  if (m_device->NeedsArp ())
    {
      NS_LOG_LOGIC ("Needs ARP" << " " << dest);
      Ptr<ArpL3Protocol> arp = m_node->GetObject<ArpL3Protocol> ();
      Address hardwareDestination;
      bool found = false;
      if (dest.IsBroadcast ())
        {
          NS_LOG_LOGIC ("All-network Broadcast");
          hardwareDestination = m_device->GetBroadcast ();
          found = true;
        }
      else if (dest.IsMulticast ())
        {
          NS_LOG_LOGIC ("IsMulticast");
          NS_ASSERT_MSG (m_device->IsMulticast (),
                         "ArpIpv4Interface::SendTo (): Sending multicast packet over "
                         "non-multicast device");

          hardwareDestination = m_device->GetMulticast (dest);
          found = true;
        }
      else
        {
          for (Ipv4InterfaceAddressListCI i = m_ifaddrs.begin (); i != m_ifaddrs.end (); ++i)
            {
              if (dest.IsSubnetDirectedBroadcast ((*i).GetMask ()))
                {
                  NS_LOG_LOGIC ("Subnetwork Broadcast");
                  hardwareDestination = m_device->GetBroadcast ();
                  found = true;
                  break;
                }
            }
          if (!found)
            {
              NS_LOG_LOGIC ("ARP Lookup");
              found = arp->Lookup (p, dest, m_device, m_cache, &hardwareDestination);
            }
        }

      if (found)
        {
          NS_LOG_LOGIC ("Address Resolved.  Send.");
          m_device->Send (p, hardwareDestination,
                          Ipv4L3Protocol::PROT_NUMBER);
        }
    }
  else
    {
      NS_LOG_LOGIC ("Doesn't need ARP");
      m_device->Send (p, m_device->GetBroadcast (), 
                      Ipv4L3Protocol::PROT_NUMBER);
    }
}

uint32_t
Ipv4Interface::GetNAddresses (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifaddrs.size ();
}

bool
Ipv4Interface::AddAddress (Ipv4InterfaceAddress addr)
{
  NS_LOG_FUNCTION (this << addr);
  m_ifaddrs.push_back (addr);
  return true;
}

Ipv4InterfaceAddress
Ipv4Interface::GetAddress (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  if (index < m_ifaddrs.size ())
    {
      uint32_t tmp = 0;
      for (Ipv4InterfaceAddressListCI i = m_ifaddrs.begin (); i!= m_ifaddrs.end (); i++)
        {
          if (tmp  == index)
            {
              return *i;
            }
          ++tmp;
        }
    }
  NS_ASSERT (false);  // Assert if not found
  Ipv4InterfaceAddress addr;
  return (addr);  // quiet compiler
}

Ipv4InterfaceAddress
Ipv4Interface::RemoveAddress (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  if (index >= m_ifaddrs.size ())
    {
      NS_ASSERT_MSG (false, "Bug in Ipv4Interface::RemoveAddress");
    }
  Ipv4InterfaceAddressListI i = m_ifaddrs.begin ();
  uint32_t tmp = 0;
  while (i != m_ifaddrs.end ())
    {
      if (tmp  == index)
        {
          Ipv4InterfaceAddress addr = *i;
          m_ifaddrs.erase (i);
          return addr;
        }
      ++tmp;
      ++i;
    }
  NS_ASSERT_MSG (false, "Address " << index << " not found");
  Ipv4InterfaceAddress addr;
  return (addr);  // quiet compiler
}

} // namespace ns3

