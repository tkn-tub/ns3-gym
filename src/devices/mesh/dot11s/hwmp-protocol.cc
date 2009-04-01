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
#include "ie-dot11s-preq.h"
#include "ie-dot11s-prep.h"
#include "ie-dot11s-perr.h"

NS_LOG_COMPONENT_DEFINE ("HwmpProtocol");

namespace ns3 {
namespace dot11s {

NS_OBJECT_ENSURE_REGISTERED (HwmpProtocol);
TypeId
HwmpProtocol::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dot11s::HwmpProtocol")
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
    .AddAttribute ("dot11MeshHWMPpreqMinInterval",
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
        MakeUintegerChecker<uint8_t> (2)
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
    m_dataSeqno (1),
    m_hwmpSeqno (1),
    m_preqId (0),
    m_rtable (CreateObject<HwmpRtable> ()),
    m_isRoot (false),
    m_doFlag (false),
    m_rfFlag (false)
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
  HwmpTag tag;
  if (sourceIface == GetMeshPoint ()->GetIfIndex())
    // packet from level 3
  {
    NS_ASSERT (!packet->PeekPacketTag(tag));
    //Filling TAG:
    if(destination == Mac48Address::GetBroadcast ())
    {
      tag.SetSeqno (m_dataSeqno++);
      tag.SetAddress (Mac48Address::GetBroadcast());
      tag.SetTtl (m_maxTtl+1);
      if (m_dataSeqno == 0xffffffff)
        m_dataSeqno = 0;
      NS_LOG_UNCOND("add a tag"<<packet->GetUid());
      packet->AddPacketTag(tag);
    }
  }
  else
  {
    NS_ASSERT (packet->PeekPacketTag(tag));
    if (tag.GetTtl () == 0)
      return false;
    tag.DecrementTtl ();
  }
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
    //packet->RemoveAllTags ();
    //Add a proper HWMP-tag:
    HwmpTag tag;
    NS_ASSERT(!packet->RemovePacketTag(tag));
    tag.SetAddress (result.retransmitter);
    //seqno and metric is not used;
    packet->AddPacketTag(tag);
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
    MakePathError (result.retransmitter);
    if(!m_isRoot)
      return false;
  }
  //Request a destination:
  if(ShouldSendPreq(destination))
    for(HwmpPluginMap::iterator i = m_interfaces.begin (); i != m_interfaces.end (); i ++)
      i->second->RequestDestination(destination);
  QueuedPacket pkt;
  HwmpTag tag;
  packet->RemovePacketTag (tag);
  tag.SetAddress(Mac48Address::GetBroadcast ());
  packet->AddPacketTag (tag);
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
HwmpProtocol::HandleIncomingFrame (Ptr<Packet> packet, Mac48Address src, Mac48Address dst, uint16_t protocol)
{
  //Handle only incoming frames:
  NS_ASSERT(dst == m_address);
  HwmpTag tag;
  NS_ASSERT(packet->RemovePacketTag(tag));
  return true;
}
void
HwmpProtocol::ReceivePreq (IePreq preq, Mac48Address from, uint32_t interface)
{
  preq.IncrementMetric (1);
  //acceptance cretirea:
  std::map<Mac48Address, uint32_t>::iterator i = m_lastHwmpSeqno.find (preq.GetOriginatorAddress());
  if (i == m_lastHwmpSeqno.end ())
    {
      m_lastHwmpSeqno[preq.GetOriginatorAddress ()] = preq.GetOriginatorSeqNumber();
      m_lastHwmpMetric[preq.GetOriginatorAddress ()] = preq.GetMetric();
    }
  else
    {
      if (i->second > preq.GetOriginatorSeqNumber ())
        return;
      if (i->second == preq.GetOriginatorSeqNumber ())
        {
          //find metric
          std::map<Mac48Address, uint32_t>::iterator j =
            m_lastHwmpMetric.find (preq.GetOriginatorAddress());
          NS_ASSERT (j != m_lastHwmpSeqno.end());
          if (j->second <= preq.GetMetric ())
            return;
        }
      m_lastHwmpSeqno[preq.GetOriginatorAddress ()] = preq.GetOriginatorSeqNumber();
      m_lastHwmpMetric[preq.GetOriginatorAddress ()] = preq.GetMetric();
    }
  //check if can answer:
  std::vector<Ptr<DestinationAddressUnit> > destinations = preq.GetDestinationList ();
  for (std::vector<Ptr<DestinationAddressUnit> >::iterator i = destinations.begin (); i != destinations.end(); i++)
    {
      if ((*i)->GetDestinationAddress () == Mac48Address::GetBroadcast())
        {
          //only proactive PREQ contains destination
          //address as broadcast! Proactive preq MUST
          //have destination count equal to 1 and
          //per destination flags DO and RF
          NS_ASSERT (preq.GetDestCount() == 1);
          NS_ASSERT (((*i)->IsDo()) && ((*i)->IsRf()));
          m_rtable->AddProactivePath (
              preq.GetMetric (),
              preq.GetOriginatorAddress (),
              from,
              interface,
              MicroSeconds (preq.GetLifetime () * 1024),
              preq.GetOriginatorSeqNumber ()
              );
          ProactivePathResolved ();
          if (!preq.IsNeedNotPrep ())
              SendPrep (
                  m_address,
                  preq.GetOriginatorAddress (),
                  from,
                  preq.GetMetric (),
                  preq.GetOriginatorSeqNumber (),
                  GetNextHwmpSeqno (),
                  preq.GetLifetime (),
                  interface
              );
          break;
        }
      if ((*i)->GetDestinationAddress () == m_address)
        {
          NS_LOG_UNCOND("PREQ has reached destination:"<<m_address);
          preq.DelDestinationAddressElement ((*i)->GetDestinationAddress());
          SendPrep (
              m_address,
              preq.GetOriginatorAddress (),
              from,
              (uint32_t)0,
              preq.GetOriginatorSeqNumber (),
              GetNextHwmpSeqno (),
              preq.GetLifetime (),
              interface
          );
          continue;
        }
      //check if can answer:
      HwmpRtable::LookupResult result = m_rtable->LookupReactive ((*i)->GetDestinationAddress());
      if ((! ((*i)->IsDo())) && (result.retransmitter != Mac48Address::GetBroadcast()))
        {
          //have a valid information and acn answer
          if ((*i)->IsRf ())
            (*i)->SetFlags (true, false); //DO = 1, RF = 0 (as it was)
          else
            {
              //send a PREP and delete destination
              preq.DelDestinationAddressElement ((*i)->GetDestinationAddress());
              SendPrep (
                  (*i)->GetDestinationAddress (),
                  preq.GetOriginatorAddress (),
                  from,
                  result.metric,
                  preq.GetOriginatorSeqNumber (),
                  result.seqnum +1,
                  preq.GetLifetime (),
                  interface
              );
              continue;
            }
        }
    }
  m_rtable->AddReactivePath (
      preq.GetOriginatorAddress (),
      from,
      interface,
      preq.GetMetric (),
      MicroSeconds (preq.GetLifetime () *1024),
      preq.GetOriginatorSeqNumber ()
      );
  ReactivePathResolved (preq.GetOriginatorAddress ());
  //m_routingInfoCallback (newInfo);
  //chack if must retransmit:
  if (preq.GetDestCount () == 0)
    return;
  //Forward PREQ to all interfaces:
  NS_LOG_UNCOND("I am "<<m_address<<"retransmitting PREQ:"<<preq);
  for(HwmpPluginMap::iterator i = m_interfaces.begin (); i != m_interfaces.end (); i ++)
    i->second->SendPreq (preq);
}
void
HwmpProtocol::ReceivePrep (IePrep prep, Mac48Address from, uint32_t interface)
{
  prep.IncrementMetric (1);
  //acceptance cretirea:
  NS_LOG_UNCOND("I am "<<m_address<<", received prep");
  std::map<Mac48Address, uint32_t>::iterator i = m_lastHwmpSeqno.find (prep.GetOriginatorAddress());
  if (i == m_lastHwmpSeqno.end ())
    {
      m_lastHwmpSeqno[prep.GetOriginatorAddress ()] = prep.GetOriginatorSeqNumber();
    }
  else
    if (i->second > prep.GetOriginatorSeqNumber ())
      return;
  //update routing info
  //Now add a path to destination and add precursor to source
  m_rtable->AddPrecursor (prep.GetDestinationAddress (), interface, from);
  m_rtable->AddReactivePath (
      prep.GetOriginatorAddress (),
      from,
      interface,
      prep.GetMetric (),
      MicroSeconds(prep.GetLifetime () * 1024),
      prep.GetOriginatorSeqNumber ());
  if(prep.GetDestinationAddress() == m_address)
    NS_LOG_UNCOND("Destination resolved:"<<prep.GetOriginatorAddress ());
  HwmpRtable::LookupResult result = m_rtable->LookupReactive(prep.GetDestinationAddress());
  if (result.retransmitter == Mac48Address::GetBroadcast ())
    //try to look for default route
    result = m_rtable->LookupProactive ();
  if (result.retransmitter == Mac48Address::GetBroadcast ())
  {
    NS_LOG_UNCOND("I am "<<m_address<<", can not forward prep");
    return;
  }
  m_rtable->AddPrecursor (prep.GetOriginatorAddress (), interface, result.retransmitter);
  //Forward PREP
  HwmpPluginMap::iterator prep_sender = m_interfaces.find (result.ifIndex);
  NS_ASSERT(prep_sender != m_interfaces.end ());
  prep_sender->second->SendPrep(prep, result.retransmitter);

}
void
HwmpProtocol::ReceivePerr (IePerr perr, Mac48Address from, uint32_t interface)
{
}
void
HwmpProtocol::SendPrep (
    Mac48Address src,
    Mac48Address dst,
    Mac48Address retransmitter,
    uint32_t initMetric,
    uint32_t originatorDsn,
    uint32_t destinationSN,
    uint32_t lifetime,
    uint32_t interface)
{
  NS_LOG_UNCOND("sending prep to "<<dst<<" through "<<retransmitter);
  IePrep prep;
  prep.SetHopcount (0);
  prep.SetTtl (m_maxTtl);
  prep.SetDestinationAddress (dst);
  prep.SetDestinationSeqNumber (destinationSN);
  prep.SetLifetime (lifetime);
  prep.SetMetric (0);
  prep.SetOriginatorAddress (src);
  prep.SetOriginatorSeqNumber (originatorDsn);
  HwmpPluginMap::iterator prep_sender = m_interfaces.find (interface);
  NS_ASSERT(prep_sender != m_interfaces.end ());
  prep_sender->second->SendPrep(prep, retransmitter);
  //m_prepCallback (prep, retransmitter);

}
bool
HwmpProtocol::Install (Ptr<MeshPointDevice> mp)
{
  m_mp = mp;
  std::vector<Ptr<NetDevice> > interfaces = mp->GetInterfaces ();
  for (std::vector<Ptr<NetDevice> >::iterator i = interfaces.begin (); i != interfaces.end(); i++)
    {
      // Checking for compatible net device
      const WifiNetDevice * wifiNetDev = dynamic_cast<const WifiNetDevice *> (PeekPointer (*i));
      if (wifiNetDev == NULL)
        return false;
      MeshWifiInterfaceMac * mac = dynamic_cast<MeshWifiInterfaceMac *> (PeekPointer (wifiNetDev->GetMac ()));
      if (mac == NULL)
        return false;
      // Installing plugins:
      Ptr<HwmpMacPlugin> hwmpMac = Create<HwmpMacPlugin> (wifiNetDev->GetIfIndex (), this);
      m_interfaces[wifiNetDev->GetIfIndex ()] = hwmpMac;
      mac->InstallPlugin (hwmpMac);
    }
  mp->SetRoutingProtocol (this);
  // Mesh point aggregates all installed protocols
  mp->AggregateObject (this);
  //Address tmp_addr = mp->GetAddress ();
  //Mac48Address * address = dynamic_cast<Mac48Address *> (&tmp_addr);
  //if (address == NULL)
  //  return false;
  m_address = Mac48Address::ConvertFrom (mp->GetAddress ());//* address;
  return true;
}
void
HwmpProtocol::PeerLinkStatus(Mac48Address peerAddress, uint32_t interface, bool status)
{
  if(status)
  {
   // m_rtable->AddReactivePath(peerAddress, peerAddress, interface, 1, Seconds (0), 0);
  }
  else
    MakePathError (peerAddress);
}
void
HwmpProtocol::SetNeighboursCallback(Callback<std::vector<Mac48Address>, uint32_t> cb)
{
  m_neighboursCallback = cb;
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
HwmpProtocol::MakePathError (Mac48Address retransmitter)
{
  NS_LOG_UNCOND ("START PERR, I am "<<m_address);
  //TODO:
  //make a perr IE and send
  std::vector<IePerr::FailedDestination> destinations = m_rtable->GetUnreachableDestinations (retransmitter);
  //HwmpRtable increments a sequence number as written in 11B.9.7.2
  NS_LOG_UNCOND("Number of unreachable destinations:"<<destinations.size ());
  for(std::vector<IePerr::FailedDestination>::iterator i =  destinations.begin (); i != destinations.end (); i ++)
  {
    HwmpRtable::LookupResult result = m_rtable->LookupReactiveExpired (i->destination);
    NS_LOG_UNCOND("Address::"<<i->destination<<", next hop is "<<result.retransmitter);
  }
  std::vector<std::pair<uint32_t, Mac48Address> > receivers = GetPerrReceivers (destinations);
  NS_LOG_UNCOND("Number of perr receivers:"<<receivers.size ());
  if(receivers.size () == 0)
    return;
  for(std::vector<std::pair<uint32_t, Mac48Address> >::iterator i = receivers.begin (); i != receivers.end (); i ++)
    NS_LOG_UNCOND("Address:"<<i->second<<", interface:"<<i->first);
  //form a path error and send it to proper ports
  IePerr perr;
  NS_ASSERT(false);
}
std::vector<std::pair<uint32_t, Mac48Address> >
HwmpProtocol::GetPerrReceivers (std::vector<IePerr::FailedDestination> failedDest)
{
  HwmpRtable::PrecursorList retval;
  for (unsigned int i = 0; i < failedDest.size (); i ++)
  {
    HwmpRtable::PrecursorList precursors = m_rtable->GetPrecursors(failedDest[i].destination);
    m_rtable->DeleteReactivePath (failedDest[i].destination);
    m_rtable->DeleteProactivePath(failedDest[i].destination);
    for (unsigned int j = 0; j < precursors.size (); j ++)
      retval.push_back(precursors[j]);
  }
  //Check if we have dublicates in retval and precursors:
  unsigned int size = retval.size();
  for (unsigned int i = 0; i < size; i ++)
    for (unsigned int j = i; j < size; j ++)
      if(retval[i].second == retval[j].second)
      {
        //erase and check size
        NS_LOG_UNCOND("deleting dublicate");
        retval.erase(retval.begin() + j);
        size --;
      }
  return retval;
}
std::vector<Mac48Address>
HwmpProtocol::GetPreqReceivers (uint32_t interface)
{
  std::vector<Mac48Address> retval;
  if(!m_neighboursCallback.IsNull ()) retval = m_neighboursCallback (interface);
  if (retval.size() >= m_unicastPreqThreshold)
  {
    retval.clear ();
    retval.push_back (Mac48Address::GetBroadcast ());
  }
  return retval;
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
    NS_ASSERT (packet.pkt->PeekPacketTag(tag));
    tag.SetAddress (result.retransmitter);
    packet.pkt->AddPacketTag (tag);
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
    NS_ASSERT (packet.pkt->PeekPacketTag(tag));
    tag.SetAddress (result.retransmitter);
    packet.pkt->AddPacketTag (tag);
    packet.reply (true, packet.pkt, packet.src, packet.dst, packet.protocol, result.ifIndex);
  }

}

bool
HwmpProtocol::ShouldSendPreq (Mac48Address dst)
{
  std::map<Mac48Address, EventId>::iterator i = m_preqTimeouts.find (dst);
  if (i == m_preqTimeouts.end ())
    {
      NS_LOG_UNCOND("Timeout is:" <<2*(m_dot11MeshHWMPnetDiameterTraversalTime.GetMilliSeconds()));
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
  for(HwmpPluginMap::iterator i = m_interfaces.begin (); i != m_interfaces.end (); i ++)
  {
    i->second->RequestDestination(dst);
    i->second->RequestDestination(Mac48Address("00:00:00:00:00:10"));
  }
  m_preqTimeouts[dst] = Simulator::Schedule (
      MilliSeconds (2*(m_dot11MeshHWMPnetDiameterTraversalTime.GetMilliSeconds())),
      &HwmpProtocol::RetryPathDiscovery, this, dst, numOfRetry);
}
//Proactive PREQ routines:
void
HwmpProtocol::SetRoot ()
{
  SendProactivePreq ();
  m_isRoot = true;
}
void
HwmpProtocol::UnsetRoot ()
{
  m_proactivePreqTimer.Cancel ();
}
void
HwmpProtocol::SendProactivePreq ()
{
  NS_LOG_DEBUG ("Sending proactive PREQ");
  IePreq preq;
  //By default: must answer
  preq.SetHopcount (0);
  preq.SetTTL (m_maxTtl);
  if (m_preqId == 0xffffffff)
    m_preqId = 0;
  preq.SetLifetime (m_dot11MeshHWMPpathToRootInterval.GetMicroSeconds () /1024);
  //\attention: do not forget to set originator address, sequence
  //number and preq ID in HWMP-MAC plugin
  preq.AddDestinationAddressElement (true, true, Mac48Address::GetBroadcast (), 0);
  for(HwmpPluginMap::iterator i = m_interfaces.begin (); i != m_interfaces.end (); i ++)
    i->second->SendPreq(preq);
  m_proactivePreqTimer = Simulator::Schedule (m_dot11MeshHWMPactiveRootTimeout, &HwmpProtocol::SendProactivePreq, this);
}
bool
HwmpProtocol::GetDoFlag ()
{
  return m_doFlag;
}
bool
HwmpProtocol::GetRfFlag ()
{
  return m_rfFlag;
}
Time
HwmpProtocol::GetPreqMinInterval ()
{
  return m_dot11MeshHWMPpreqMinInterval;
}
Time
HwmpProtocol::GetPerrMinInterval ()
{
  return m_dot11MeshHWMPperrMinInterval;
}
uint8_t
HwmpProtocol::GetMaxTtl ()
{
  return m_maxTtl;
}
uint32_t
HwmpProtocol::GetNextPreqId ()
{
  m_preqId ++;
  if(m_preqId == 0xffffffff)
    m_preqId = 0;
  return m_preqId;
}
uint32_t
HwmpProtocol::GetNextHwmpSeqno ()
{
  m_hwmpSeqno ++;
  if(m_hwmpSeqno == 0xffffffff)
    m_hwmpSeqno = 0;
  return m_hwmpSeqno;
}
uint32_t
HwmpProtocol::GetActivePathLifetime ()
{
  return m_dot11MeshHWMPactivePathTimeout.GetMicroSeconds () / 1024;
}
Mac48Address
HwmpProtocol::GetAddress ()
{
  return m_address;
}
} //namespace dot11s
} //namespace ns3
