/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/tap-bridge.h"
#include "tap-bridge-helper.h"

NS_LOG_COMPONENT_DEFINE ("TapBridgeHelper");

namespace ns3 {

TapBridgeHelper::TapBridgeHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_deviceFactory.SetTypeId ("ns3::TapBridge");
}

void 
TapBridgeHelper::SetDeviceAttribute (std::string n1, const AttributeValue &v1)
{
  NS_LOG_FUNCTION (n1 << &v1);
  m_deviceFactory.Set (n1, v1);
}

  Ptr<NetDevice>
TapBridgeHelper::Install (Ptr<Node> node, Ptr<NetDevice> nd)
{
  NS_LOG_FUNCTION (node << nd);
  NS_LOG_LOGIC ("Install TapBridge on node " << node->GetId () << " bridging net device " << nd);

  Ptr<TapBridge> bridge = m_deviceFactory.Create<TapBridge> ();
  node->AddDevice (bridge);
  bridge->SetBridgedDevice (nd);

  return bridge;
}

} // namespace ns3
