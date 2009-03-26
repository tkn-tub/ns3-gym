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
        );
  return tid;
}
HwmpProtocol::HwmpProtocol ():
    m_dataSeqno(0),
    m_hwmpSeqno(0),
    m_maxTtl (32),
    m_rtable (CreateObject<HwmpRtable> ())
{
}

HwmpProtocol::~HwmpProtocol ()
{
}

void
HwmpProtocol::DoDispose ()
{
#if 0
  for (std::map<Mac48Address, EventId>::iterator i = m_preqTimeouts.begin (); i != m_preqTimeouts.end(); i ++)
    i->second.Cancel ();
  m_preqTimeouts.clear ();
  m_seqnoDatabase.clear ();
  m_rtable = 0;

  /**
   * clear routing queue:
   */
  for (
    std::map<Mac48Address, std::queue<QueuedPacket> >::iterator i =  m_rqueue.begin ();
    i != m_rqueue.end ();
    i++
  )
    {
      while (1)
        {
          if (i->second.empty ())
            break;
          i->second.pop ();
        }
    }
  m_rqueue.clear ();
  /**
   * clear HWMP states
   */
  for (unsigned int i = 0; i < m_hwmpStates.size (); i ++)
    m_hwmpStates[i] = 0;
  m_hwmpStates.clear ();
#endif
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
  //HwmpRtable::LookupResult result;
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
    if (destination == Mac48Address::GetBroadcast ())
    {
      //Reply immediately
      routeReply (true, packet, source, destination, protocolType, 0xffffffff);
    }
    else
    {
      
    }
  }
  else
    // packet from own interface
  {
    NS_ASSERT (packet->FindFirstMatchingTag(tag));
    if (destination == Mac48Address::GetBroadcast ())
      //reply immediately
      routeReply (true, packet, source, destination, protocolType, 0xffffffff);
    else
    {
      NS_ASSERT(false);
    }
  }
#if 0
      if (destination == Mac48Address::GetBroadcast ())
        {
          std::map<Mac48Address, uint32_t>::iterator i = m_seqnoDatabase.find (source);
          if (i == m_seqnoDatabase.end ())
            m_seqnoDatabase[source] = tag.GetSeqno ();
          else
            {
              if (i->second >= tag.GetSeqno ())
                return false;
              m_seqnoDatabase[source] = tag.GetSeqno ();
            }
        }
    }
  if (tag.GetTtl () == 0)
    return false;
  tag.DecrementTtl ();
  if (destination == Mac48Address::GetBroadcast ())
    {
      //add RA tag RA = broadcast
      packet->RemoveAllTags ();
      packet->AddTag (tag);
      routeReply (
        true,
        packet,
        source,
        destination,
        protocolType,
        HwmpRtable::PORT_ANY
      );
      return true;
    }
  result = m_rtable->LookupReactive (destination);
  if (result.retransmitter == Mac48Address::GetBroadcast ())
    {
      //no actual route exists, queue packet and start route
      //discover procedure
      if (sourceIface != GetMeshPoint ()->GetIfIndex())
        {
          //Start path error procedure:
          NS_LOG_DEBUG ("Must Send PERR");
          std::vector<HwmpRtable::FailedDestination> destinations;
          HwmpRtable::FailedDestination dst;
          dst.seqnum = m_rtable->RequestSeqnum (destination);
          dst.destination = destination;
          destinations.push_back (dst);
          StartPathErrorProcedure (destinations, result.ifIndex);
        }
      MeshL2RoutingProtocol::QueuedPacket pkt;
      packet->RemoveAllTags ();
      packet->AddTag (tag);
      pkt.pkt = packet;
      pkt.dst = destination;
      pkt.src = source;
      pkt.protocol = protocolType;
      pkt.reply = routeReply;
      pkt.inPort = sourceIface;
      QueuePacket (pkt);
      for (unsigned int i = 0; i< m_requestCallback.size (); i++)
        {
          if ((m_modes[i] == REACTIVE) || (m_modes[i] == ROOT))
            {
              if (ShouldSendPreq (destination))
                m_requestCallback[i] (destination);
            }
          else
            {
              NS_ASSERT (false);
              //PROACTIVE mode
              //lookup a default route
              result = m_rtable->LookupProactive (m_hwmpStates[i]->GetAssociatedIfaceId());
              if (result.retransmitter == Mac48Address::GetBroadcast ())
                {
                  m_rtable->DeleteProactivePath (m_hwmpStates[i]->GetAssociatedIfaceId());
                  m_modes[i] = REACTIVE;
                  if (ShouldSendPreq (destination))
                    m_requestCallback[i] (destination);
                  continue;
                }
              tag.SetAddress (result.retransmitter);
              packet->RemoveAllTags ();
              packet->AddTag (tag);
              routeReply (
                true,
                packet,
                source,
                destination,
                protocolType,
                result.ifIndex
              );
            }
        }
      for (unsigned int i = 0; i< m_requestCallback.size (); i++)
        {
          m_requestCallback[i] (Mac48Address("00:00:00:00:00:19"));
        }
    }
  else
    {
      tag.SetAddress (result.retransmitter);
      packet->RemoveAllTags ();
      packet->AddTag (tag);
      routeReply (
        true,
        packet,
        source,
        destination,
        protocolType,
        result.ifIndex
      );
    }
#endif
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
HwmpProtocol::SetRoot (uint32_t port)
{
  int position = 0;
  for (std::vector<Ptr<HwmpProtocolState> >::iterator i = m_hwmpStates.begin (); i != m_hwmpStates.end(); i++)
    {
      if (((*i)->GetAssociatedIfaceId () == port)||(port == HwmpRtable::PORT_ANY))
        {
          if (m_hwmpStates[position]->SetRoot ())
            {
              m_modes[position] = ROOT;
              NS_LOG_DEBUG ("I am proactive");
            }
        }
      position ++;
    }
}
void
HwmpProtocol::SetProactive (uint32_t port)
{
  int position = 0;
  for (std::vector<Ptr<HwmpProtocolState> >::iterator i = m_hwmpStates.begin (); i != m_hwmpStates.end(); i++)
    {
      if ((*i)->GetAssociatedIfaceId () == port)
        {
          m_modes[position] = PROACTIVE;
          return;
        }
      position ++;
    }
}
bool
HwmpProtocol::IsRoot (uint32_t port)
{
  int position = 0;
  for (std::vector<Ptr<HwmpProtocolState> >::iterator i = m_hwmpStates.begin (); i != m_hwmpStates.end(); i++)
    {
      if ((*i)->GetAssociatedIfaceId () == port)
        if (m_modes[position] == ROOT)
          return true;
      position ++;
    }
  return false;
}
void
HwmpProtocol::UnSetRoot (uint32_t port)
{
  int position = 0;
  for (std::vector<Ptr<HwmpProtocolState> >::iterator i = m_hwmpStates.begin (); i != m_hwmpStates.end(); i++)
    {
      if (((*i)->GetAssociatedIfaceId () == port)||(port == HwmpRtable::PORT_ANY))
        {
          m_modes[position] = REACTIVE;
          m_hwmpStates[position]->UnSetRoot ();
        }
      position ++;
    }
}

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
      StartPathErrorProcedure (failedDestinations, info.outPort);
    }
    break;
    default:
      return;
    }
}

HwmpRtable::LookupResult
HwmpProtocol::RequestRouteForAddress (const Mac48Address& dst)
{
  return m_rtable->LookupReactive (dst);
}

HwmpRtable::LookupResult
HwmpProtocol::RequestRootPathForPort (uint32_t port)
{
  return m_rtable->LookupProactive (port);
}

void
HwmpProtocol::StartPathErrorProcedure (std::vector<HwmpRtable::FailedDestination> destinations, uint32_t port)
{
  NS_LOG_DEBUG ("START PERR");
  for (unsigned int i  = 0; i < m_hwmpStates.size (); i++)
    if (m_hwmpStates[i]->GetAssociatedIfaceId () == port)
      m_pathErrorCallback[i] (destinations);
}
std::vector<Mac48Address>
HwmpProtocol::GetRetransmittersForFailedDestinations (std::vector<HwmpRtable::FailedDestination> failedDest, uint32_t port)
{
  std::vector<Mac48Address> retransmitters;
  if (m_broadcastPerr)
    retransmitters.push_back (Mac48Address::GetBroadcast());
  else
    for (unsigned int i = 0; i < failedDest.size (); i ++)
      {
        std::vector<Mac48Address> precursors =
          m_rtable->GetPrecursors (failedDest[i].destination, port);
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
      m_rtable->DeleteReactivePath (failedDest[i].destination, port);
      m_rtable->DeleteProactivePath (failedDest[i].destination, port);
    }
  return retransmitters;
}
#endif
void
HwmpProtocol::SetMaxQueueSize (int maxPacketsPerDestination)
{
}
bool
HwmpProtocol::QueuePacket (MeshL2RoutingProtocol::QueuedPacket packet)
{
#if 0
  if ((int)m_rqueue[packet.dst].size () > m_maxQueueSize)
    return false;
  m_rqueue[packet.dst].push (packet);
#endif
  return true;
}

MeshL2RoutingProtocol::QueuedPacket
HwmpProtocol::DequeuePacket (Mac48Address dst)
{
  MeshL2RoutingProtocol::QueuedPacket retval;
#if 0
  retval.pkt = NULL;
  //Ptr<Packet> in this structure is NULL when queue is empty
  std::map<Mac48Address, std::queue<QueuedPacket> >:: iterator i = m_rqueue.find (dst);
  if (i == m_rqueue.end ())
    return retval;
  if ((int)m_rqueue[dst].size () == 0)
    return retval;
  if ((int)i->second.size () == 0)
    {
      m_rqueue.erase (i);
      return retval;
    }
  retval = m_rqueue[dst].front ();
  m_rqueue[dst].pop ();
#endif
  return retval;
}
void
HwmpProtocol::SendAllPossiblePackets (Mac48Address dst)
{
#if 0
  HwmpRtable::LookupResult result = m_rtable->LookupReactive (dst);
  MeshL2RoutingProtocol::QueuedPacket packet;
  while (1)

    {
      packet = DequeuePacket (dst);
      if (packet.pkt == NULL)
        return;
      //set RA tag for retransmitter:
      HwmpProtocolTag tag;
      NS_ASSERT (packet.pkt->FindFirstMatchingTag(tag));
      tag.SetAddress (result.retransmitter);
      NS_ASSERT (result.retransmitter != Mac48Address::GetBroadcast());
      packet.pkt->RemoveAllTags ();
      packet.pkt->AddTag (tag);
      packet.reply (true, packet.pkt, packet.src, packet.dst, packet.protocol, result.ifIndex);
    }
#endif
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
#if 0
  HwmpRtable::LookupResult result = m_rtable->LookupReactive (dst);
  if (result.retransmitter != Mac48Address::GetBroadcast ())
    {
      std::map<Mac48Address, EventId>::iterator i = m_preqTimeouts.find (dst);
      NS_ASSERT (i !=  m_preqTimeouts.end());
      m_preqTimeouts.erase (i);
      return;
    }
  numOfRetry++;
  if (numOfRetry > dot11sParameters::dot11MeshHWMPmaxPREQretries)
    {
      MeshL2RoutingProtocol::QueuedPacket packet;
      //purge queue and delete entry from retryDatabase
      while (1)
        {
          packet = DequeuePacket (dst);
          if (packet.pkt == NULL)
            break;
          packet.reply (false, packet.pkt, packet.src, packet.dst, packet.protocol, HwmpRtable::MAX_METRIC);
        }
      std::map<Mac48Address, EventId>::iterator i = m_preqTimeouts.find (dst);
      NS_ASSERT (i !=  m_preqTimeouts.end());
      m_preqTimeouts.erase (i);
      return;
    }
#if 0
  for (unsigned int i = 0; i < m_requestCallback.size (); i++)
    if ((m_modes[i] == REACTIVE) || (m_modes[i] == ROOT))
      m_requestCallback[i] (dst);
#endif
  m_preqTimeouts[dst] = Simulator::Schedule (
                             MilliSeconds (2*(dot11sParameters::dot11MeshHWMPnetDiameterTraversalTime.GetMilliSeconds())),
                             &HwmpProtocol::RetryPathDiscovery, this, dst, numOfRetry);
#endif
}
} //namespace dot11s
} //namespace ns3
