/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */


#include "hwmp-protocol.h"
#include "hwmp-mac-plugin.h"
#include "hwmp-tag.h"
#include "hwmp-rtable.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/mesh-point-device.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mesh-point-device.h"
#include "ns3/mesh-wifi-interface-mac.h"

NS_LOG_COMPONENT_DEFINE ("HwmpProtocol");

namespace ns3 {
namespace dot11s {

NS_OBJECT_ENSURE_REGISTERED (HwmpProtocol);
TypeId
HwmpProtocol::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::HwmpProtocol")
    .SetParent<MeshL2RoutingProtocol> ()
    .AddConstructor<HwmpProtocol> ()
    .AddAttribute ("dot11MeshHWMPmaxPREQretries",
        "Maximum number of retries before we suppose the destination to be unreachable",
        UintegerValue (3),
        MakeUintegerAccessor (&HwmpProtocol::m_dot11MeshHWMPmaxPREQretries),
        MakeUintegerChecker<uint8_t> (1)
        )
    .AddAttribute ("dot11MeshHWMPnetDiameterTraversalTime",
        "Time we suppose the packet to go from one edge of the network to another",
        TimeValue (MicroSeconds (1024*10)),
        MakeTimeAccessor (&HwmpProtocol::m_dot11MeshHWMPnetDiameterTraversalTime),
        MakeTimeChecker ()
        )
    .AddAttribute ("dot11MeshHWMPpreqMinInterva",
        "Minimal interval between to successive PREQs",
        TimeValue (MicroSeconds (1024*100)),
        MakeTimeAccessor (&HwmpProtocol::m_dot11MeshHWMPpreqMinInterval),
        MakeTimeChecker ()
        )
    .AddAttribute ("dot11MeshHWMPperrMinInterval",
        "Minimal interval between to successive PREQs",
        TimeValue (MicroSeconds (1024*100)),
        MakeTimeAccessor (&HwmpProtocol::m_dot11MeshHWMPperrMinInterval),
        MakeTimeChecker ()
        )
    .AddAttribute ("dot11MeshHWMPactiveRootTimeout",
        "Lifetime of poractive routing information",
        TimeValue (MicroSeconds (1024*5000)),
        MakeTimeAccessor (&HwmpProtocol::m_dot11MeshHWMPactiveRootTimeout),
        MakeTimeChecker ()
        )
    .AddAttribute ("dot11MeshHWMPactivePathTimeout",
        "Lifetime of reactive routing information",
        TimeValue (MicroSeconds (1024*5000)),
        MakeTimeAccessor (&HwmpProtocol::m_dot11MeshHWMPactivePathTimeout),
        MakeTimeChecker ()
        )
    .AddAttribute ("dot11MeshHWMPpathToRootInterval",
        "Interval between two successive proactive PREQs",
        TimeValue (MicroSeconds (1024*5000)),
        MakeTimeAccessor (&HwmpProtocol::m_dot11MeshHWMPpathToRootInterval),
        MakeTimeChecker ()
        )
    .AddAttribute ("dot11MeshHWMPrannInterval",
        "Lifetime of poractive routing information",
        TimeValue (MicroSeconds (1024*5000)),
        MakeTimeAccessor (&HwmpProtocol::m_dot11MeshHWMPrannInterval),
        MakeTimeChecker ()
        )
  .AddAttribute ("maxQueueSize",
        "Maximum number of packets we can store when resolving route",
        UintegerValue (255),
        MakeUintegerAccessor (&HwmpProtocol::m_maxQueueSize),
        MakeUintegerChecker<uint16_t> (1)
        )
  .AddAttribute ("maxTtl",
        "Initial value of Time To Live field",
        UintegerValue (32),
        MakeUintegerAccessor (&HwmpProtocol::m_maxTtl),
        MakeUintegerChecker<uint8_t> (1)
        )
  .AddAttribute ("unicastPerrThreshold",
        "Maximum number of PERR receivers, when we send a PERR as a chain of unicasts",
        UintegerValue (32),
        MakeUintegerAccessor (&HwmpProtocol::m_unicastPerrThreshold),
        MakeUintegerChecker<uint8_t> (1)
        )
  .AddAttribute ("unicastPreqThreshold",
        "Maximum number of PREQ receivers, when we send a PREQ as a chain of unicasts",
        UintegerValue (0),
        MakeUintegerAccessor (&HwmpProtocol::m_unicastPreqThreshold),
        MakeUintegerChecker<uint8_t> (0)
        );
  return tid;
}
HwmpProtocol::HwmpProtocol ():
    m_dataSeqno(1),
    m_hwmpSeqno(1),
    m_rtable (CreateObject<HwmpRtable> ()),
    m_isRoot (false)
{
}

HwmpProtocol::~HwmpProtocol ()
{
}

void
HwmpProtocol::DoDispose ()
{
  for (std::map<Mac48Address, EventId>::iterator i = m_preqTimeouts.begin (); i != m_preqTimeouts.end(); i ++)
    i->second.Cancel ();
  m_preqTimeouts.clear ();
  m_lastDataSeqno.clear ();
  m_lastHwmpSeqno.clear ();
  m_rqueue.clear ();
  m_rtable = 0;
  //TODO: clear plugins
}

bool
HwmpProtocol::RequestRoute (
  uint32_t sourceIface,
  const Mac48Address source,
  const Mac48Address destination,
  Ptr<Packet> packet,
  uint16_t protocolType, //ethrnet 'Protocol' field
  MeshL2RoutingProtocol::RouteReplyCallback routeReply
)
{
  NS_LOG_UNCOND("Packet has come!");
  HwmpTag tag;
  if (sourceIface == GetMeshPoint ()->GetIfIndex())
    // packet from level 3
  {
    NS_LOG_UNCOND("Packet from upper layer. Broadcast frame");
    NS_ASSERT (!packet->FindFirstMatchingTag(tag));
    //Filling TAG:
    tag.SetSeqno (m_dataSeqno++);
    tag.SetAddress (Mac48Address::GetBroadcast());
    tag.SetTtl (m_maxTtl);
    if (m_dataSeqno == 0xffffffff)
      m_dataSeqno = 0;
    packet->AddTag(tag);
  }
  NS_ASSERT (packet->FindFirstMatchingTag(tag));
  if (destination == Mac48Address::GetBroadcast ())
    routeReply (true, packet, source, destination, protocolType, HwmpRtable::INTERFACE_ANY);
  else
    return ForwardUnicast(sourceIface, source, destination, packet, protocolType, routeReply);
  return true;
}
bool
HwmpProtocol::ForwardUnicast(uint32_t  sourceIface, const Mac48Address source, const Mac48Address destination,
    Ptr<Packet>  packet, uint16_t  protocolType, RouteReplyCallback  routeReply)
{
  NS_ASSERT(destination != Mac48Address::GetBroadcast ());
  HwmpRtable::LookupResult result = m_rtable->LookupReactive(destination);
  if(result.retransmitter == Mac48Address::GetBroadcast ())
    result = m_rtable->LookupProactive ();
  if(result.retransmitter != Mac48Address::GetBroadcast ())
  {
    //reply immediately:
    routeReply (true, packet, source, destination, protocolType, result.ifIndex);
    return true;
  }
  if (sourceIface != GetMeshPoint ()->GetIfIndex())
  {
    //Start path error procedure:
    NS_LOG_DEBUG ("Must Send PERR");
    result = m_rtable->LookupReactiveExpired (destination);
    //1.  Lookup expired reactive path. If exists - start path error
    //    procedure towards a next hop of this path
    //2.  If there was no reactive path, we lookup expired proactive
    //    path. If exist - start path error procedure towards path to
    //    root
    //3.  If and only if we are a root station - we queue packet
    if((result.retransmitter == Mac48Address::GetBroadcast ()) && (!m_isRoot))
      result = m_rtable->LookupProactiveExpired ();
    if((result.retransmitter == Mac48Address::GetBroadcast ()) && (!m_isRoot))
      return false;
    MakePathError (result.retransmitter, result.ifIndex);
    if(!m_isRoot)
      return false;
  }
  //Request a destination:
  NS_ASSERT(false);
  QueuedPacket pkt;
  HwmpTag tag;
  tag.SetAddress(Mac48Address::GetBroadcast ());
  packet->RemoveAllTags ();
  packet->AddTag (tag);
  pkt.pkt = packet;
  pkt.dst = destination;
  pkt.src = source;
  pkt.protocol = protocolType;
  pkt.reply = routeReply;
  pkt.inInterface = sourceIface;
  QueuePacket (pkt);
  return true;
}
bool
HwmpProtocol::Install (Ptr<MeshPointDevice> mp)
{
  m_mp = mp;
  std::vector<Ptr<NetDevice> > interfaces = mp->GetInterfaces ();
  for (std::vector<Ptr<NetDevice> >::iterator i = interfaces.begin (); i != interfaces.end(); i++)
    {
      //Checking netdevice:
      const WifiNetDevice * wifiNetDev = dynamic_cast<const WifiNetDevice *> (PeekPointer (*i));
      if (wifiNetDev == NULL)
        return false;
      MeshWifiInterfaceMac * mac = dynamic_cast<MeshWifiInterfaceMac *> (PeekPointer (wifiNetDev->GetMac ()));
      if (mac == NULL)
        return false;
      //Installing plugins:
      Ptr<HwmpMacPlugin> hwmpMac = Create<HwmpMacPlugin> (wifiNetDev->GetIfIndex (), this);
      m_interfaces[wifiNetDev->GetIfIndex ()] = hwmpMac;
      mac->InstallPlugin (hwmpMac);
    }
  mp->SetRoutingProtocol(this);
  return true;
}
bool
HwmpProtocol::DropDataFrame(uint32_t seqno, Mac48Address source)
{
  std::map<Mac48Address, uint32_t,std::less<Mac48Address> >::iterator i = m_lastDataSeqno.find (source);
  if (i == m_lastDataSeqno.end ())
    m_lastDataSeqno[source] = seqno;
  else
  {
    if (i->second >= seqno)
      return true;
    m_lastDataSeqno[source] = seqno;
  }
  return false;
}

#if 0
void
HwmpProtocol::ObtainRoutingInformation (
  HwmpProtocolState::INFO info
)
{
  switch (info.type)
    {
    case HwmpProtocolState::INFO_PREP:
      if (info.me != info.source)
        {
          m_rtable->AddPrecursor (info.source, info.outPort, info.nextHop);
          m_rtable->AddPrecursor (info.destination, info.outPort, info.prevHop);
          NS_LOG_DEBUG ("path to "<<info.source<<" precursor is "<<info.nextHop);
          NS_LOG_DEBUG ("path to "<<info.destination<<" precursor is "<<info.prevHop);
        }
    case HwmpProtocolState::INFO_PREQ:
      m_rtable->AddReactivePath (
        info.destination,
        info.nextHop,
        info.outPort,
        info.metric,
        info.lifetime,
        info.dsn);
      SendAllPossiblePackets (info.destination);
      break;
    case HwmpProtocolState::INFO_PERR:
      //delete first subentry
    case HwmpProtocolState::INFO_PROACTIVE:
      //add information to the root MP.
      m_rtable->AddProactivePath (
        info.metric,
        info.destination,
        info.nextHop,
        info.outPort,
        info.lifetime,
        info.dsn);
      //Set mode as PROACTIVE:
      SetProactive (info.outPort);
      break;
    case HwmpProtocolState::INFO_NEW_PEER:
#if 0
      m_rtable->AddReactivePath (
        info.destination,
        info.nextHop,
        info.outPort,
        info.metric,
        Seconds (0),
        0);
#endif
      break;
    case HwmpProtocolState::INFO_FAILED_PEER:
      /**
       * Conditions for generating PERR
       */
    {
      NS_LOG_DEBUG ("Failed peer"<<info.destination);
      std::vector<HwmpRtable::FailedDestination> failedDestinations =
        m_rtable->GetUnreachableDestinations (info.destination, info.outPort);
      /**
       * Entry about peer does not contain seqnum
       */
      HwmpRtable::FailedDestination peer;
      peer.destination = info.destination;
      peer.seqnum = 0;
      failedDestinations.push_back (peer);
      MakePathError (failedDestinations, info.outPort);
    }
    break;
    default:
      return;
    }
}
#endif
void
HwmpProtocol::MakePathError (Mac48Address retransmitter, uint32_t interface)
{
  NS_LOG_DEBUG ("START PERR");
  //TODO:
  //make a perr IE and send
  std::vector<IePerr::FailedDestination> destinations = m_rtable->GetUnreachableDestinations (retransmitter, interface);
  //HwmpRtable increments a sequence number as written in 11B.9.7.2
  std::vector<Mac48Address> receivers = GetPerrReceivers (destinations, interface);
  NS_ASSERT(false);
}
std::vector<Mac48Address>
HwmpProtocol::GetPerrReceivers (std::vector<IePerr::FailedDestination> failedDest, uint32_t interface)
{
  std::vector<Mac48Address> retransmitters;
    
  for (unsigned int i = 0; i < failedDest.size (); i ++)
  {
    std::vector<Mac48Address> precursors =
      m_rtable->GetPrecursors (failedDest[i].destination, interface);
    for (unsigned int j = 0; j < precursors.size (); j++)
    {
      for (unsigned int k = 0; k < retransmitters.size (); k ++)
        if (retransmitters[k] == precursors[j])
          break;
      retransmitters.push_back (precursors[j]);
    }
  }
  for (unsigned int i = 0; i < failedDest.size (); i ++)
  {
      m_rtable->DeleteReactivePath (failedDest[i].destination, interface);
      m_rtable->DeleteProactivePath(failedDest[i].destination);
  }
  return retransmitters;
}
bool
HwmpProtocol::QueuePacket (QueuedPacket packet)
{
  if (m_rqueue.size () > m_maxQueueSize)
    return false;
  m_rqueue.push_back (packet);
  return true;
}

MeshL2RoutingProtocol::QueuedPacket
HwmpProtocol::DequeueFirstPacketByDst (Mac48Address dst)
{
  QueuedPacket retval;
  retval.pkt = NULL;
  for(std::vector<QueuedPacket>::iterator i = m_rqueue.begin (); i != m_rqueue.end (); i++)
    if((*i).dst == dst)
    {
      retval = (*i);
      m_rqueue.erase (i);
      break;
    }
  return retval;
}
MeshL2RoutingProtocol::QueuedPacket
HwmpProtocol::DequeueFirstPacket ()
{
  QueuedPacket retval;
  retval.pkt = NULL;
  if(m_rqueue.size () != 0)
    retval = m_rqueue[0];
  m_rqueue.erase (m_rqueue.begin ());
  return retval;
}
void
HwmpProtocol::ReactivePathResolved (Mac48Address dst)
{
  HwmpRtable::LookupResult result = m_rtable->LookupReactive (dst);
  NS_ASSERT(result.retransmitter != Mac48Address::GetBroadcast ());
  //Send all packets stored for this destination    
  QueuedPacket packet;
  while (1)
  {
    packet = DequeueFirstPacketByDst (dst);
    if (packet.pkt == NULL)
      return;
    //set RA tag for retransmitter:
    HwmpTag tag;
    NS_ASSERT (packet.pkt->FindFirstMatchingTag(tag));
    tag.SetAddress (result.retransmitter);
    packet.pkt->RemoveAllTags ();
    packet.pkt->AddTag (tag);
    packet.reply (true, packet.pkt, packet.src, packet.dst, packet.protocol, result.ifIndex);
  }
}
void
HwmpProtocol::ProactivePathResolved ()
{
  //send all packets to root
  HwmpRtable::LookupResult result = m_rtable->LookupProactive ();
  NS_ASSERT(result.retransmitter != Mac48Address::GetBroadcast ());
  QueuedPacket packet;
  while (1)
  {
    packet = DequeueFirstPacket ();
    if (packet.pkt == NULL)
      return;
    //set RA tag for retransmitter:
    HwmpTag tag;
    NS_ASSERT (packet.pkt->FindFirstMatchingTag(tag));
    tag.SetAddress (result.retransmitter);
    packet.pkt->RemoveAllTags ();
    packet.pkt->AddTag (tag);
    packet.reply (true, packet.pkt, packet.src, packet.dst, packet.protocol, result.ifIndex);
  }

}

bool
HwmpProtocol::ShouldSendPreq (Mac48Address dst)
{
  std::map<Mac48Address, EventId>::iterator i = m_preqTimeouts.find (dst);
  if (i == m_preqTimeouts.end ())
    {
      m_preqTimeouts[dst] = Simulator::Schedule (
          MilliSeconds (2*(m_dot11MeshHWMPnetDiameterTraversalTime.GetMilliSeconds())),
          &HwmpProtocol::RetryPathDiscovery, this, dst, 0);
      return true;
    }
  return false;
}
void
HwmpProtocol::RetryPathDiscovery (Mac48Address dst, uint8_t numOfRetry)
{
  HwmpRtable::LookupResult result = m_rtable->LookupReactive (dst);
  if(result.retransmitter == Mac48Address::GetBroadcast ())
    result = m_rtable->LookupProactive ();
  if (result.retransmitter != Mac48Address::GetBroadcast ())
    {
      std::map<Mac48Address, EventId>::iterator i = m_preqTimeouts.find (dst);
      NS_ASSERT (i !=  m_preqTimeouts.end());
      m_preqTimeouts.erase (i);
      return;
    }
  numOfRetry++;
  if (numOfRetry > m_dot11MeshHWMPmaxPREQretries)
    {
      QueuedPacket packet;
      //purge queue and delete entry from retryDatabase
      while (1)
        {
          packet = DequeueFirstPacketByDst (dst);
          if (packet.pkt == NULL)
            break;
          packet.reply (false, packet.pkt, packet.src, packet.dst, packet.protocol, HwmpRtable::MAX_METRIC);
        }
      std::map<Mac48Address, EventId>::iterator i = m_preqTimeouts.find (dst);
      NS_ASSERT (i !=  m_preqTimeouts.end());
      m_preqTimeouts.erase (i);
      return;
    }
  //TODO: Request a destination again
  NS_ASSERT(false);
  m_preqTimeouts[dst] = Simulator::Schedule (
      MilliSeconds (2*(m_dot11MeshHWMPnetDiameterTraversalTime.GetMilliSeconds())),
      &HwmpProtocol::RetryPathDiscovery, this, dst, numOfRetry);
}
} //namespace dot11s
} //namespace ns3
