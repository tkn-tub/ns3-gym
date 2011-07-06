/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include <iostream>
#include <sstream>

// ns3 includes
#include "ns3/animation-interface.h"
#include "ns3/csma-star-helper.h"
#include "ns3/node-list.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/vector.h"

NS_LOG_COMPONENT_DEFINE ("CsmaStarHelper");

namespace ns3 {

CsmaStarHelper::CsmaStarHelper (uint32_t numSpokes,
                                CsmaHelper csmaHelper)
{
  m_hub.Create (1);
  m_spokes.Create (numSpokes);

  for (uint32_t i = 0; i < m_spokes.GetN (); ++i)
    {
      NodeContainer nodes (m_hub.Get (0), m_spokes.Get (i));
      NetDeviceContainer nd = csmaHelper.Install (nodes);
      m_hubDevices.Add (nd.Get (0));
      m_spokeDevices.Add (nd.Get (1));
    }
}

CsmaStarHelper::~CsmaStarHelper ()
{
}

Ptr<Node>
CsmaStarHelper::GetHub () const
{
  return m_hub.Get (0);
}

Ptr<Node>
CsmaStarHelper::GetSpokeNode (uint32_t i) const
{
  return m_spokes.Get (i);
}

NetDeviceContainer
CsmaStarHelper::GetHubDevices () const
{
  return m_hubDevices;
}

NetDeviceContainer
CsmaStarHelper::GetSpokeDevices () const
{
  return m_spokeDevices;
}

Ipv4Address 
CsmaStarHelper::GetHubIpv4Address (uint32_t i) const
{
  return m_hubInterfaces.GetAddress (i);
}

Ipv4Address 
CsmaStarHelper::GetSpokeIpv4Address (uint32_t i) const
{
  return m_spokeInterfaces.GetAddress (i);
}

uint32_t
CsmaStarHelper::SpokeCount () const
{
  return m_spokes.GetN ();
}

void 
CsmaStarHelper::InstallStack (InternetStackHelper stack)
{
  stack.Install (m_hub);
  stack.Install (m_spokes);
}

void 
CsmaStarHelper::AssignIpv4Addresses (Ipv4AddressHelper address)
{
  for (uint32_t i = 0; i < m_spokes.GetN (); ++i)
    {
      m_hubInterfaces.Add (address.Assign (m_hubDevices.Get (i)));
      m_spokeInterfaces.Add (address.Assign (m_spokeDevices.Get (i)));
      address.NewNetwork ();
    }
}

} // namespace ns3
