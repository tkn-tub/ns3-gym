/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* 
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#include "ns3/peer-manager-plugin.h"
#include "ns3/ie-dot11s-configuration.h"
#include "ns3/ie-dot11s-peer-management.h"
#include "ns3/mesh-wifi-interface-mac.h"

#include "ns3/log.h"

namespace ns3 {
Dot11sPeerManagerMacPlugin::Dot11sPeerManagerMacPlugin ()
{
}

Dot11sPeerManagerMacPlugin::~Dot11sPeerManagerMacPlugin ()
{
}

void
Dot11sPeerManagerMacPlugin::SetParent (Ptr<MeshWifiInterfaceMac> parent)
{
}

bool
Dot11sPeerManagerMacPlugin::Receive (Ptr<Packet> packet, const WifiMacHeader & header)
{
  return false;
}

bool
Dot11sPeerManagerMacPlugin::UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to) const
{
  return false;
}

void
Dot11sPeerManagerMacPlugin::UpdateBeacon (MeshWifiBeacon & beacon) const
{
}

void
Dot11sPeerManagerMacPlugin::SetDeliverPeerLinkFrameCallbback (
    Callback<void, uint32_t, Mac48Address, uint16_t, IeDot11sConfiguration, IeDot11sPeerManagement>
    )
{
}

void
Dot11sPeerManagerMacPlugin::SetDeliverBeaconCallback (Callback<void>)
{
}
}//namespace ns3
