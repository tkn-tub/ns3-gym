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

// Define an object to create a dumbbell topology.

#ifndef CSMA_STAR_HELPER_H
#define CSMA_STAR_HELPER_H

#include <string>

#include "csma-helper.h"
#include "ipv4-address-helper.h"
#include "internet-stack-helper.h"
#include "ipv4-interface-container.h"

namespace ns3 {
  
class CsmaStarHelper
{
public:
  CsmaStarHelper (uint32_t numSpokes, 
                  CsmaHelper csmaHelper);
  ~CsmaStarHelper ();
public:
  Ptr<Node> GetHub () const;
  Ptr<Node> GetSpoke (uint32_t) const;
  NetDeviceContainer GetHubDevices () const;
  NetDeviceContainer GetSpokeDevices () const;
  Ipv4Address GetHubIpv4Address (uint32_t) const;
  Ipv4Address GetSpokeIpv4Address (uint32_t) const;
  uint32_t SpokeCount () const;
  void InstallStack (InternetStackHelper stack);
  void AssignIpv4Addresses (Ipv4AddressHelper address);

private:
  NodeContainer m_hub;
  NetDeviceContainer m_hubDevices;
  NodeContainer m_spokes;
  NetDeviceContainer m_spokeDevices;
  Ipv4InterfaceContainer m_hubInterfaces;
  Ipv4InterfaceContainer m_spokeInterfaces;
};
}
#endif


