/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#include <iostream>
#include "ns3/assert.h"

#include "net-device.h"
#include "l3-demux.h"
#include "l3-protocol.h"
#include "llc-snap-header.h"
#include "node.h"

namespace ns3 {

NetDevice::NetDevice(Node *node, const MacAddress& addr) : 
  m_node (node), 
  m_name(""), 
  m_ifIndex (0), 
  m_address (addr),
  m_mtu (0), 
  m_isUp (false), 
  m_isBroadcast (false), 
  m_isMulticast (false), 
  m_isPointToPoint (false)
{}

MacAddress 
NetDevice::GetAddress (void) const
{
  return m_address;
}

bool
NetDevice::SetMtu (const uint16_t mtu) 
{
  m_mtu = mtu;
  return true;
}

uint16_t 
NetDevice::GetMtu (void) const
{
  return m_mtu;
}

void
NetDevice::SetName(const std::string name) 
{ 
  m_name = name; 
}

std::string 
NetDevice::GetName(void) const 
{ 
  return m_name; 
}

bool 
NetDevice::IsLinkUp (void) const
{
  return m_isUp;
}

void 
NetDevice::SetLinkChangeCallback (Callback<void> callback)
{
  m_linkChangeCallback = callback;
}

bool
NetDevice::IsBroadcast (void) const
{
  return m_isBroadcast;
}
MacAddress const &
NetDevice::GetBroadcast (void) const
{
  NS_ASSERT (m_isBroadcast);
  return m_broadcast;
}

void
NetDevice::EnableBroadcast (MacAddress broadcast)
{
  m_isBroadcast = true;
  m_broadcast = broadcast;
}

void
NetDevice::DisableBroadcast (void)
{
  m_isBroadcast = false;
}

bool
NetDevice::IsMulticast (void) const
{
  return m_isMulticast;
}

void
NetDevice::EnableMulticast (void)
{
  m_isMulticast = true;
}

void
NetDevice::DisableMulticast (void)
{
  m_isMulticast = false;
}

bool
NetDevice::IsPointToPoint (void) const
{
  return m_isPointToPoint;
}

void
NetDevice::EnablePointToPoint (void)
{
  m_isPointToPoint = true;
}

void
NetDevice::DisablePointToPoint (void)
{
  m_isPointToPoint = false;
}

// Receive packet from above
bool 
NetDevice::Send(Packet& p, const MacAddress& dest, uint16_t protocolNumber)
{
  if (m_isUp)
    {
      LlcSnapHeader llc;
      llc.SetType (protocolNumber);
      p.Add (llc);
      return SendTo(p, dest);
    }
  else
    {
      return false;
    }
}

TraceResolver *
NetDevice::CreateTraceResolver (TraceContext const &context)
{
  return DoCreateTraceResolver (context);
}

// Receive packet from below
bool
NetDevice::ForwardUp (Packet& packet)
{
  LlcSnapHeader llc;
  packet.Peek (llc);
  packet.Remove (llc);
  if (GetNode()->GetL3Demux() != 0)
    {
      L3Protocol *target = GetNode()->GetL3Demux()->Lookup(llc.GetType ());
      if (target != 0) 
        {
          target->Receive(packet, *this);
          return true;
        }
    }
  return false;
}

void 
NetDevice::NotifyLinkUp (void)
{
  m_isUp = true;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

void 
NetDevice::NotifyLinkDown (void)
{
  m_isUp = false;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

Node *
NetDevice::GetNode (void) const
{
  return m_node;
}

}; // namespace ns3
