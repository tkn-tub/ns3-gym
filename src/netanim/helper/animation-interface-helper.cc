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
 * Author: John Abraham<john.abraham@gatech.edu>
 */

// Animation Interface helpers

#include "ns3/animation-interface-helper.h"
#include "ns3/log.h"

#include <iostream>

NS_LOG_COMPONENT_DEFINE ("AnimationInterfaceHelper");

namespace ns3 {
AnimPacketInfo::AnimPacketInfo()
  : m_txnd (0), m_fbTx (0), m_lbTx (0), 
    m_txLoc (Vector (0,0,0))
{
}

AnimPacketInfo::AnimPacketInfo(Ptr <const NetDevice> txnd, const Time& fbTx, 
  const Time& lbTx, Vector txLoc, uint32_t txNodeId)
  : m_txnd (txnd), m_fbTx (fbTx.GetSeconds ()), m_lbTx (lbTx.GetSeconds ()), 
    m_txLoc (txLoc)
{
  if (!m_txnd)
    m_txNodeId = txNodeId;
}

void AnimPacketInfo::ProcessRxBegin (Ptr<const NetDevice> nd, const Time& fbRx)
{
  m_rx[nd->GetNode ()->GetId ()] = AnimRxInfo (fbRx, nd, 0);
}

bool AnimPacketInfo::ProcessRxEnd (Ptr<const NetDevice> nd, const Time& lbRx, Vector rxLoc)
{
  uint32_t NodeId = nd->GetNode ()->GetId (); 
  // Find the RxInfo
  if (m_rx.find (NodeId) == m_rx.end ())
    {
      return false;
    } 
  AnimRxInfo& rxInfo = m_rx[NodeId];
  // Check if the NetDevice matches. A node may have several NetDevices
  if (rxInfo.m_rxnd != nd) 
    {
      return false; 
    }
  rxInfo.rxRange = CalculateDistance (m_txLoc, rxLoc);
  rxInfo.m_lbRx = lbRx.GetSeconds ();
  rxInfo.SetPhyRxComplete ();
  firstlastbitDelta = rxInfo.m_lbRx - rxInfo.m_fbRx;
  return true;
}

AnimRxInfo AnimPacketInfo::GetRxInfo (Ptr<const NetDevice> nd)
{
  uint32_t NodeId = nd->GetNode ()->GetId (); 
  NS_ASSERT (m_rx.find (NodeId) != m_rx.end ()); 
  return m_rx[NodeId];
}

void AnimPacketInfo::RemoveRxInfo (Ptr<const NetDevice> nd)
{
  uint32_t NodeId = nd->GetNode ()->GetId (); 
  m_rx.erase (m_rx.find (NodeId));
}

void AnimPacketInfo::ProcessRxDrop (Ptr<const NetDevice> nd)
{
}

bool AnimRxInfo::IsPhyRxComplete ()
{
  return m_PhyRxComplete;
}

void AnimRxInfo::SetPhyRxComplete ()
{
  m_PhyRxComplete = true; 
}

} // namespace ns3
