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
  : m_txnd (0), m_nRx (0), m_nRxEnd (0), m_fbTx (0), m_lbTx (0), 
    transmitter_loc (Vector (0,0,0))
{
}

AnimPacketInfo::AnimPacketInfo(Ptr<const NetDevice> nd, const Time& fbTx, 
  const Time& lbTx, Vector txLoc)
  : m_txnd (nd), m_nRx (0), m_nRxEnd (0),
    m_fbTx (fbTx.GetSeconds ()), m_lbTx (lbTx.GetSeconds ()), transmitter_loc (txLoc), 
    reception_range (0) 
{
}

void AnimPacketInfo::ProcessRxBegin (Ptr<const NetDevice> nd, const Time& fbRx,
                                 Vector rxLoc)
{
  m_rx[nd->GetNode ()->GetId ()] = AnimRxInfo (fbRx,nd);
  m_nRx++;
  reception_range = std::max (reception_range,CalculateDistance (transmitter_loc,rxLoc));
}

bool AnimPacketInfo::ProcessRxEnd (Ptr<const NetDevice> nd, const Time& lbRx)

{
  uint32_t NodeId = nd->GetNode ()->GetId (); 
  // Find the RxInfo
  if (m_rx.find(NodeId) != m_rx.end ())
    { 
      // Check if the NetDevice matches. A node may have several NetDevices
      if (m_rx[NodeId].m_rxnd != nd) 
        {
          return false; 
        }
      m_rx[NodeId].m_lbRx = lbRx.GetSeconds ();
      firstlastbitDelta = m_rx[NodeId].m_lbRx - m_rx[NodeId].m_fbRx;
      m_nRxEnd++;
      if (m_nRxEnd == m_nRx) 
        {
          return true;
        }
      return false; // Still more RxEnd notifications expected
    }
   NS_ASSERT ("Received RxEnd notification but RxInfo never existed");
  return false; // Still more rxEnd expected
}

void AnimPacketInfo::ProcessRxDrop (Ptr<const NetDevice> nd)
{
  if (m_rx.find (nd->GetNode ()->GetId ()) == m_rx.end()) 
    {
      NS_LOG_DEBUG ("Received RxDrop notification"); 
      return;
    }
  // Remove any entry for which an RxBegin notification was received
  m_rx.erase (m_rx.find (nd->GetNode ()->GetId ()));
  m_nRx--;
}


} // namespace ns3
