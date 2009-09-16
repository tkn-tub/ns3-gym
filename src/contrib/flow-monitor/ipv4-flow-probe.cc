/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2009 INESC Porto
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt> <gjcarneiro@gmail.com>
//

#include "ns3/ipv4-flow-probe.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/flow-monitor.h"
#include "ns3/log.h"
#include "ns3/pointer.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ipv4FlowProbe");
  

Ipv4FlowProbe::~Ipv4FlowProbe ()
{
}
  

Ipv4FlowProbe::Ipv4FlowProbe (Ptr<FlowMonitor> monitor,
                              Ptr<Ipv4FlowClassifier> classifier,
                              Ptr<Node> node)
  : FlowProbe (monitor),
    m_classifier (classifier)
{
  NS_LOG_FUNCTION (this << node->GetId ());

  Ptr<Ipv4L3Protocol> ipv4 = node->GetObject<Ipv4L3Protocol> ();

  if (!ipv4->TraceConnectWithoutContext ("SendOutgoing",
                                         MakeCallback (&Ipv4FlowProbe::SendOutgoingLogger, Ptr<Ipv4FlowProbe> (this))))
    {
      NS_FATAL_ERROR ("trace fail");
    }
  if (!ipv4->TraceConnectWithoutContext ("UnicastForward",
                                         MakeCallback (&Ipv4FlowProbe::ForwardLogger, Ptr<Ipv4FlowProbe> (this))))
    {
      NS_FATAL_ERROR ("trace fail");
    }
  if (!ipv4->TraceConnectWithoutContext ("LocalDeliver",
                                         MakeCallback (&Ipv4FlowProbe::ForwardUpLogger, Ptr<Ipv4FlowProbe> (this))))
    {
      NS_FATAL_ERROR ("trace fail");
    }

  if (!ipv4->TraceConnectWithoutContext ("Drop",
                                         MakeCallback (&Ipv4FlowProbe::DropLogger, Ptr<Ipv4FlowProbe> (this))))
    {
      NS_FATAL_ERROR ("trace fail");
    }
}

void
Ipv4FlowProbe::SendOutgoingLogger (const Ipv4Header &ipHeader, Ptr<const Packet> ipPayload, uint32_t interface)
{
  FlowId flowId;
  FlowPacketId packetId;
  
  if (m_classifier->Classify (ipHeader, ipPayload, &flowId, &packetId))
    {
      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("ReportFirstTx ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<"); "
                    << ipHeader << *ipPayload);
      m_flowMonitor->ReportFirstTx (this, flowId, packetId, size);
    }
}

void
Ipv4FlowProbe::ForwardLogger (const Ipv4Header &ipHeader, Ptr<const Packet> ipPayload, uint32_t interface)
{
  FlowId flowId;
  FlowPacketId packetId;
  
  if (m_classifier->Classify (ipHeader, ipPayload, &flowId, &packetId))
    {
      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("ReportForwarding ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<");");
      m_flowMonitor->ReportForwarding (this, flowId, packetId, size);
    }

}

void
Ipv4FlowProbe::ForwardUpLogger (const Ipv4Header &ipHeader, Ptr<const Packet> ipPayload, uint32_t interface)
{  
  FlowId flowId;
  FlowPacketId packetId;
  
  if (m_classifier->Classify (ipHeader, ipPayload, &flowId, &packetId))
    {
      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("ReportLastRx ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<");");
      m_flowMonitor->ReportLastRx (this, flowId, packetId, size);
    }
}

void
Ipv4FlowProbe::DropLogger (const Ipv4Header &ipHeader, Ptr<const Packet> ipPayload,
                           Ipv4L3Protocol::DropReason reason, uint32_t ifIndex)
{
#if 0
  switch (reason)
    {
    case Ipv4L3Protocol::DROP_NO_ROUTE:
      break;
      
    case Ipv4L3Protocol::DROP_TTL_EXPIRED:
    case Ipv4L3Protocol::DROP_BAD_CHECKSUM:
      Ipv4Address addri = m_ipv4->GetAddress (ifIndex);
      Ipv4Mask maski = m_ipv4->GetNetworkMask (ifIndex);
      Ipv4Address bcast = addri.GetSubnetDirectedBroadcast (maski);
      if (ipHeader.GetDestination () == bcast) // we don't want broadcast packets
        {
          return;
        }
    }
#endif

  FlowId flowId;
  FlowPacketId packetId;

  if (m_classifier->Classify (ipHeader, ipPayload, &flowId, &packetId))
    {
      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("Drop ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<", " << reason
                    << ", destIp=" << ipHeader.GetDestination () << "); "
                    << "HDR: " << ipHeader << " PKT: " << *ipPayload);

      DropReason myReason;


      switch (reason)
        {
        case Ipv4L3Protocol::DROP_TTL_EXPIRED:
          myReason = DROP_TTL_EXPIRE;
          NS_LOG_DEBUG ("DROP_TTL_EXPIRE");
          break;
        case Ipv4L3Protocol::DROP_NO_ROUTE:
          myReason = DROP_NO_ROUTE;
          NS_LOG_DEBUG ("DROP_NO_ROUTE");
          break;
        case Ipv4L3Protocol::DROP_BAD_CHECKSUM:
          myReason = DROP_BAD_CHECKSUM;
          NS_LOG_DEBUG ("DROP_BAD_CHECKSUM");
          break;
        default:
          myReason = DROP_INVALID_REASON;
          NS_FATAL_ERROR ("Unexpected drop reason code " << reason);
        }

      m_flowMonitor->ReportDrop (this, flowId, packetId, size, myReason);
    }
}



} // namespace ns3


