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
 *
 * Author: George F. Riley<riley@ece.gatech.edu>
 */

// Define an object to create a dumbbell topology.

#ifndef POINT_TO_POINT_DUMBBELL_HELPER_H
#define POINT_TO_POINT_DUMBBELL_HELPER_H

#include <string>

#include "point-to-point-helper.h"
#include "ipv4-address-helper.h"
#include "internet-stack-helper.h"
#include "ipv4-interface-container.h"

namespace ns3 {
  
class PointToPointDumbbellHelper
{
public:
  PointToPointDumbbellHelper (uint32_t nLeftLeaf,  // Number of left size leaf nodes
           PointToPointHelper leftHelper,
           uint32_t nRightLeaf, // Number of right side leaf nodes
           PointToPointHelper rightHelper,
           PointToPointHelper bottleneckHelper);
  ~PointToPointDumbbellHelper ();
public:
  Ptr<Node> GetLeft ()           const; // Get the left side bottleneck router
  Ptr<Node> GetLeft (uint32_t)   const; // Get the i'th left side leaf
  Ptr<Node> GetRight ()          const; // Get the right side bottleneck router
  Ptr<Node> GetRight (uint32_t)  const; // Get the i'th right side leaf
  Ipv4Address GetLeftIpv4Address (uint32_t)  const; // Get left leaf address
  Ipv4Address GetRightIpv4Address (uint32_t) const; // Get right leaf address
  uint32_t  LeftCount ()         const; // Number of left side nodes
  uint32_t  RightCount ()        const; // Number of right side nodes
  void      InstallStack (InternetStackHelper stack);
  void      AssignIpv4Addresses (Ipv4AddressHelper leftIp,
                            Ipv4AddressHelper rightIp,
                            Ipv4AddressHelper routerIp);
  // Add locations in the specified bounding box
  // Arguments are uppler left x, upper left y, lower right x, lower right y
  void      BoundingBox (double, double, double, double);
  
private:
  NodeContainer          m_leftLeaf;
  NetDeviceContainer     m_leftLeafDevices;
  NodeContainer          m_rightLeaf;
  NetDeviceContainer     m_rightLeafDevices;
  NodeContainer          m_routers;
  NetDeviceContainer     m_routerDevices; // just two connecting the routers
  // Device containers for the router devices connecting to the leaf devices
  NetDeviceContainer     m_leftRouterDevices;
  NetDeviceContainer     m_rightRouterDevices;
  Ipv4InterfaceContainer m_leftLeafInterfaces;
  Ipv4InterfaceContainer m_leftRouterInterfaces;
  Ipv4InterfaceContainer m_rightLeafInterfaces;
  Ipv4InterfaceContainer m_rightRouterInterfaces;
  Ipv4InterfaceContainer m_routerInterfaces;
};
}
#endif


