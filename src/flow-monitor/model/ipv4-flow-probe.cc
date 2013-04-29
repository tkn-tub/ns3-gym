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
#include "ns3/config.h"
#include "ns3/flow-id-tag.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ipv4FlowProbe");

//////////////////////////////////////
// Ipv4FlowProbeTag class implementation //
//////////////////////////////////////

class Ipv4FlowProbeTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer buf) const;
  virtual void Deserialize (TagBuffer buf);
  virtual void Print (std::ostream &os) const;
  Ipv4FlowProbeTag ();
  Ipv4FlowProbeTag (uint32_t flowId, uint32_t packetId, uint32_t packetSize);
  void SetFlowId (uint32_t flowId);
  void SetPacketId (uint32_t packetId);
  void SetPacketSize (uint32_t packetSize);
  uint32_t GetFlowId (void) const;
  uint32_t GetPacketId (void) const;
  uint32_t GetPacketSize (void) const;
private:
  uint32_t m_flowId;
  uint32_t m_packetId;
  uint32_t m_packetSize;

};

TypeId 
Ipv4FlowProbeTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4FlowProbeTag")
    .SetParent<Tag> ()
    .AddConstructor<Ipv4FlowProbeTag> ()
  ;
  return tid;
}
TypeId 
Ipv4FlowProbeTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t 
Ipv4FlowProbeTag::GetSerializedSize (void) const
{
  return 4 + 4 + 4;
}
void 
Ipv4FlowProbeTag::Serialize (TagBuffer buf) const
{
  buf.WriteU32 (m_flowId);
  buf.WriteU32 (m_packetId);
  buf.WriteU32 (m_packetSize);
}
void 
Ipv4FlowProbeTag::Deserialize (TagBuffer buf)
{
  m_flowId = buf.ReadU32 ();
  m_packetId = buf.ReadU32 ();
  m_packetSize = buf.ReadU32 ();
}
void 
Ipv4FlowProbeTag::Print (std::ostream &os) const
{
  os << "FlowId=" << m_flowId;
  os << "PacketId=" << m_packetId;
  os << "PacketSize=" << m_packetSize;
}
Ipv4FlowProbeTag::Ipv4FlowProbeTag ()
  : Tag () 
{
}

Ipv4FlowProbeTag::Ipv4FlowProbeTag (uint32_t flowId, uint32_t packetId, uint32_t packetSize)
  : Tag (), m_flowId (flowId), m_packetId (packetId), m_packetSize (packetSize)
{
}

void
Ipv4FlowProbeTag::SetFlowId (uint32_t id)
{
  m_flowId = id;
}
void
Ipv4FlowProbeTag::SetPacketId (uint32_t id)
{
  m_packetId = id;
}
void
Ipv4FlowProbeTag::SetPacketSize (uint32_t size)
{
  m_packetSize = size;
}
uint32_t
Ipv4FlowProbeTag::GetFlowId (void) const
{
  return m_flowId;
}
uint32_t
Ipv4FlowProbeTag::GetPacketId (void) const
{
  return m_packetId;
} 
uint32_t
Ipv4FlowProbeTag::GetPacketSize (void) const
{
  return m_packetSize;
} 

////////////////////////////////////////
// Ipv4FlowProbe class implementation //
////////////////////////////////////////

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

  // code copied from point-to-point-helper.cc
  std::ostringstream oss;
  oss << "/NodeList/" << node->GetId () << "/DeviceList/*/TxQueue/Drop";
  Config::ConnectWithoutContext (oss.str (), MakeCallback (&Ipv4FlowProbe::QueueDropLogger, Ptr<Ipv4FlowProbe> (this)));
}

Ipv4FlowProbe::~Ipv4FlowProbe ()
{
}

void
Ipv4FlowProbe::DoDispose ()
{
  FlowProbe::DoDispose ();
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

      // tag the packet with the flow id and packet id, so that the packet can be identified even
      // when Ipv4Header is not accessible at some non-IPv4 protocol layer
      Ipv4FlowProbeTag fTag (flowId, packetId, size);
      ipPayload->AddPacketTag (fTag);
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
      // remove the tags that are added by Ipv4FlowProbe::SendOutgoingLogger ()
      Ipv4FlowProbeTag fTag;

      // ConstCast: see http://www.nsnam.org/bugzilla/show_bug.cgi?id=904
      ConstCast<Packet> (ipPayload)->RemovePacketTag (fTag);

      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("ReportLastRx ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<");");
      m_flowMonitor->ReportLastRx (this, flowId, packetId, size);
    }
}

void
Ipv4FlowProbe::DropLogger (const Ipv4Header &ipHeader, Ptr<const Packet> ipPayload,
                           Ipv4L3Protocol::DropReason reason, Ptr<Ipv4> ipv4, uint32_t ifIndex)
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
      // remove the tags that are added by Ipv4FlowProbe::SendOutgoingLogger ()
      Ipv4FlowProbeTag fTag;

      // ConstCast: see http://www.nsnam.org/bugzilla/show_bug.cgi?id=904
      ConstCast<Packet> (ipPayload)->RemovePacketTag (fTag);

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
        case Ipv4L3Protocol::DROP_INTERFACE_DOWN:
          myReason = DROP_INTERFACE_DOWN;
          NS_LOG_DEBUG ("DROP_INTERFACE_DOWN");
          break;
        case Ipv4L3Protocol::DROP_ROUTE_ERROR:
          myReason = DROP_ROUTE_ERROR;
          NS_LOG_DEBUG ("DROP_ROUTE_ERROR");
          break;
        case Ipv4L3Protocol::DROP_FRAGMENT_TIMEOUT:
          myReason = DROP_FRAGMENT_TIMEOUT;
          NS_LOG_DEBUG ("DROP_FRAGMENT_TIMEOUT");
          break;

        default:
          myReason = DROP_INVALID_REASON;
          NS_FATAL_ERROR ("Unexpected drop reason code " << reason);
        }

      m_flowMonitor->ReportDrop (this, flowId, packetId, size, myReason);
    }
}

void 
Ipv4FlowProbe::QueueDropLogger (Ptr<const Packet> ipPayload)
{
  // remove the tags that are added by Ipv4FlowProbe::SendOutgoingLogger ()
  Ipv4FlowProbeTag fTag;

  // ConstCast: see http://www.nsnam.org/bugzilla/show_bug.cgi?id=904
  bool tagFound;
  tagFound = ConstCast<Packet> (ipPayload)->RemovePacketTag (fTag);
  if (!tagFound)
    {
      return;
    }

  FlowId flowId = fTag.GetFlowId ();
  FlowPacketId packetId = fTag.GetPacketId ();
  uint32_t size = fTag.GetPacketSize ();

  NS_LOG_DEBUG ("Drop ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<", " << DROP_QUEUE 
                        << "); ");

  m_flowMonitor->ReportDrop (this, flowId, packetId, size, DROP_QUEUE);
}

} // namespace ns3


