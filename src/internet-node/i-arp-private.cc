/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "i-arp-private.h"
#include "ns3/iid-manager.h"
#include "arp.h"
#include "ns3/assert.h"

namespace ns3 {

const uint32_t IArpPrivate::iid = IidManager::Allocate ("IArpPrivate");

IArpPrivate::IArpPrivate (Arp *arp)
  : NsUnknown (IArpPrivate::iid),
    m_arp (arp)
{
  m_arp->Ref ();
}
IArpPrivate::~IArpPrivate ()
{
  NS_ASSERT (m_arp == 0);
}

bool 
IArpPrivate::Lookup (Packet &p, Ipv4Address destination, 
		     NetDevice *device,
		     MacAddress *hardwareDestination)
{
  return m_arp->Lookup (p, destination, device, hardwareDestination);
}

void
IArpPrivate::DoDispose (void)
{
  m_arp->Unref ();
  m_arp = 0;
  NsUnknown::DoDispose ();
}


} // namespace ns3
