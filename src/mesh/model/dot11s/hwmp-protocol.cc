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
#include "hwmp-protocol-mac.h"
#include "hwmp-tag.h"
#include "hwmp-rtable.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/mesh-point-device.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mesh-point-device.h"
#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/random-variable.h"
#include "airtime-metric.h"
#include "ie-dot11s-preq.h"
#include "ie-dot11s-prep.h"
#include "ns3/trace-source-accessor.h"
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
    .AddAttribute ( "RandomStart",
                    "Random delay at first proactive PREQ",
                    TimeValue (Seconds (0.1)),
                    MakeTimeAccessor (
                      &HwmpProtocol::m_randomStart),
                    MakeTimeChecker ()
                    )
    .AddAttribute ( "MaxQueueSize",
                    "Maximum number of packets we can store when resolving route",
                    UintegerValue (255),
                    MakeUintegerAccessor (
                      &HwmpProtocol::m_maxQueueSize),
                    MakeUintegerChecker<uint16_t> (1)
                    )
    .AddAttribute ( "Dot11MeshHWMPmaxPREQretries",
                    "Maximum number of retries before we suppose the destination to be unreachable",
                    UintegerValue (3),
                    MakeUintegerAccessor (
                      &HwmpProtocol::m_dot11MeshHWMPmaxPREQretries),
                    MakeUintegerChecker<uint8_t> (1)
                    )
    .AddAttribute ( "Dot11MeshHWMPnetDiameterTraversalTime",
                    "Time we suppose the packet to go from one edge of the network to another",
                    TimeValue (MicroSeconds (1024*100)),
                    MakeTimeAccessor (
                      &HwmpProtocol::m_dot11MeshHWMPnetDiameterTraversalTime),
                    MakeTimeChecker ()
                    )
    .AddAttribute ( "Dot11MeshHWMPpreqMinInterval",
                    "Minimal interval between to successive PREQs",
                    TimeValue (MicroSeconds (1024*100)),
                    MakeTimeAccessor (
                      &HwmpProtocol::m_dot11MeshHWMPpreqMinInterval),
                    MakeTimeChecker ()
                    )
    .AddAttribute ( "Dot11MeshHWMPperrMinInterval",
                    "Minimal interval between to successive PREQs",
                    TimeValue (MicroSeconds (1024*100)),
                    MakeTimeAccessor (&HwmpProtocol::m_dot11MeshHWMPperrMinInterval),
                    MakeTimeChecker ()
                    )
    .AddAttribute ( "Dot11MeshHWMPactiveRootTimeout",
                    "Lifetime of poractive routing information",
                    TimeValue (MicroSeconds (1024*5000)),
                    MakeTimeAccessor (
                      &HwmpProtocol::m_dot11MeshHWMPactiveRootTimeout),
                    MakeTimeChecker ()
                    )
    .AddAttribute ( "Dot11MeshHWMPactivePathTimeout",
                    "Lifetime of reactive routing information",
                    TimeValue (MicroSeconds (1024*5000)),
                    MakeTimeAccessor (
                      &HwmpProtocol::m_dot11MeshHWMPactivePathTimeout),
                    MakeTimeChecker ()
                    )
    .AddAttribute ( "Dot11MeshHWMPpathToRootInterval",
                    "Interval between two successive proactive PREQs",
                    TimeValue (MicroSeconds (1024*2000)),
                    MakeTimeAccessor (
                      &HwmpProtocol::m_dot11MeshHWMPpathToRootInterval),
                    MakeTimeChecker ()
                    )
    .AddAttribute ( "Dot11MeshHWMPrannInterval",
                    "Lifetime of poractive routing information",
                    TimeValue (MicroSeconds (1024*5000)),
                    MakeTimeAccessor (
                      &HwmpProtocol::m_dot11MeshHWMPrannInterval),
                    MakeTimeChecker ()
                    )
    .AddAttribute ( "MaxTtl",
                    "Initial value of Time To Live field",
                    UintegerValue (32),
                    MakeUintegerAccessor (
                      &HwmpProtocol::m_maxTtl),
                    MakeUintegerChecker<uint8_t> (2)
                    )
    .AddAttribute ( "UnicastPerrThreshold",
                    "Maximum number of PERR receivers, when we send a PERR as a chain of unicasts",
                    UintegerValue (32),
                    MakeUintegerAccessor (
                      &HwmpProtocol::m_unicastPerrThreshold),
                    MakeUintegerChecker<uint8_t> (1)
                    )
    .AddAttribute ( "UnicastPreqThreshold",
                    "Maximum number of PREQ receivers, when we send a PREQ as a chain of unicasts",
                    UintegerValue (1),
                    MakeUintegerAccessor (
                      &HwmpProtocol::m_unicastPreqThreshold),
                    MakeUintegerChecker<uint8_t> (1)
                    )
    .AddAttribute ( "UnicastDataThreshold",
                    "Maximum number ofbroadcast receivers, when we send a broadcast as a chain of unicasts",
                    UintegerValue (1),
                    MakeUintegerAccessor (
                      &HwmpProtocol::m_unicastDataThreshold),
                    MakeUintegerChecker<uint8_t> (1)
                    )
    .AddAttribute ( "DoFlag",
                    "Destination only HWMP flag",
                    BooleanValue (false),
                    MakeBooleanAccessor (
                      &HwmpProtocol::m_doFlag),
                    MakeBooleanChecker ()
                    )
    .AddAttribute ( "RfFlag",
                    "Reply and forward flag",
                    BooleanValue (true),
                    MakeBooleanAccessor (
                      &HwmpProtocol::m_rfFlag),
                    MakeBooleanChecker ()
                    )
    .AddTraceSource ( "RouteDiscoveryTime",
                      "The time of route discovery procedure",
                      MakeTraceSourceAccessor (
                        &HwmpProtocol::m_routeDiscoveryTimeCallback)
                      )
  ;
  return tid;
}

HwmpProtocol::HwmpProtocol () :
  m_dataSeqno (1),
  m_hwmpSeqno (1),
  m_preqId (0),
  m_rtable (CreateObject<HwmpRtable> ()),
  m_randomStart (Seconds (0.1)),
  m_maxQueueSize (255),
  m_dot11MeshHWMPmaxPREQretries (3),
  m_dot11MeshHWMPnetDiameterTraversalTime (MicroSeconds (1024*100)),
  m_dot11MeshHWMPpreqMinInterval (MicroSeconds (1024*100)),
  m_dot11MeshHWMPperrMinInterval (MicroSeconds (1024*100)),
  m_dot11MeshHWMPactiveRootTimeout (MicroSeconds (1024*5000)),
  m_dot11MeshHWMPactivePathTimeout (MicroSeconds (1024*5000)),
  m_dot11MeshHWMPpathToRootInterval (MicroSeconds (1024*2000)),
  m_dot11MeshHWMPrannInterval (MicroSeconds (1024*5000)),
  m_isRoot (false),
  m_maxTtl (32),
  m_unicastPerrThreshold (32),
  m_unicastPreqThreshold (1),
  m_unicastDataThreshold (1),
  m_doFlag (false),
  m_rfFlag (false)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_isRoot)
    {
      SetRoot ();
    }
}

HwmpProtocol::~HwmpProtocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
HwmpProtocol::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for (std::map<Mac48Address, PreqEvent>::iterator i = m_preqTimeouts.begin (); i != m_preqTimeouts.end (); i++)
    {
      i->second.preqTimeout.Cancel ();
    }
  m_proactivePreqTimer.Cancel ();
  m_preqTimeouts.clear ();
  m_lastDataSeqno.clear ();
  m_hwmpSeqnoMetricDatabase.clear ();
  m_interfaces.clear ();
  m_rqueue.clear ();
  m_rtable = 0;
  m_mp = 0;
}

bool
HwmpProtocol::RequestRoute (
  uint32_t sourceIface,
  const Mac48Address source,
  const Mac48Address destination,
  Ptr<const Packet> constPacket,
  uint16_t protocolType, //ethrnet 'Protocol' field
  MeshL2RoutingProtocol::RouteReplyCallback routeReply
  )
{
  Ptr <Packet> packet = constPacket->Copy ();
  HwmpTag tag;
  if (sourceIface == GetMeshPoint ()->GetIfIndex ())
    {
      // packet from level 3
      if (packet->PeekPacketTag (tag))
        {
          NS_FATAL_ERROR ("HWMP tag has come with a packet from upper layer. This must not occur...");
        }
      //Filling TAG:
      if (destination == Mac48Address::GetBroadcast ())
        {
          tag.SetSeqno (m_dataSeqno++);
        }
      tag.SetTtl (m_maxTtl);
    }
  else
    {
      if (!packet->RemovePacketTag (tag))
        {
          NS_FATAL_ERROR ("HWMP tag is supposed to be here at this point.");
        }
      tag.DecrementTtl ();
      if (tag.GetTtl () == 0)
        {
          m_stats.droppedTtl++;
          return false;
        }
    }
  if (destination == Mac48Address::GetBroadcast ())
    {
      m_stats.txBroadcast++;
      m_stats.txBytes += packet->GetSize ();
      //channel IDs where we have already sent broadcast:
      std::vector<uint16_t> channels;
      for (HwmpProtocolMacMap::const_iterator plugin = m_interfaces.begin (); plugin != m_interfaces.end (); plugin++)
        {
          bool shouldSend = true;
          for (std::vector<uint16_t>::const_iterator chan = channels.begin (); chan != channels.end (); chan++)
            {
              if ((*chan) == plugin->second->GetChannelId ())
                {
                  shouldSend = false;
                }
            }
          if (!shouldSend)
            {
              continue;
            }
          channels.push_back (plugin->second->GetChannelId ());
          std::vector<Mac48Address> receivers = GetBroadcastReceivers (plugin->first);
          for (std::vector<Mac48Address>::const_iterator i = receivers.begin (); i != receivers.end (); i++)
            {
              Ptr<Packet> packetCopy = packet->Copy ();
              //
              // 64-bit Intel valgrind complains about tag.SetAddress (*i).  It
              // likes this just fine.
              //
              Mac48Address address = *i;
              tag.SetAddress (address);
              packetCopy->AddPacketTag (tag);
              routeReply (true, packetCopy, source, destination, protocolType, plugin->first);
            }
        }
    }
  else
    {
      return ForwardUnicast (sourceIface, source, destination, packet, protocolType, routeReply, tag.GetTtl ());
    }
  return true;
}
bool
HwmpProtocol::RemoveRoutingStuff (uint32_t fromIface, const Mac48Address source,
                                  const Mac48Address destination, Ptr<Packet>  packet, uint16_t&  protocolType)
{
  HwmpTag tag;
  if (!packet->RemovePacketTag (tag))
    {
      NS_FATAL_ERROR ("HWMP tag must exist when packet received from the network");
    }
  return true;
}
bool
HwmpProtocol::ForwardUnicast (uint32_t  sourceIface, const Mac48Address source, const Mac48Address destination,
                              Ptr<Packet>  packet, uint16_t  protocolType, RouteReplyCallback  routeReply, uint32_t ttl)
{
  NS_ASSERT (destination != Mac48Address::GetBroadcast ());
  HwmpRtable::LookupResult result = m_rtable->LookupReactive (destination);
  NS_LOG_DEBUG ("Requested src = "<<source<<", dst = "<<destination<<", I am "<<GetAddress ()<<", RA = "<<result.retransmitter);
  if (result.retransmitter == Mac48Address::GetBroadcast ())
    {
      result = m_rtable->LookupProactive ();
    }
  HwmpTag tag;
  tag.SetAddress (result.retransmitter);
  tag.SetTtl (ttl);
  //seqno and metric is not used;
  packet->AddPacketTag (tag);
  if (result.retransmitter != Mac48Address::GetBroadcast ())
    {
      //reply immediately:
      routeReply (true, packet, source, destination, protocolType, result.ifIndex);
      m_stats.txUnicast++;
      m_stats.txBytes += packet->GetSize ();
      return true;
    }
  if (sourceIface != GetMeshPoint ()->GetIfIndex ())
    {
      //Start path error procedure:
      NS_LOG_DEBUG ("Must Send PERR");
      result = m_rtable->LookupReactiveExpired (destination);
      //1.  Lookup expired reactive path. If exists - start path error
      //    procedure towards a next hop of this path
      //2.  If there was no reactive path, we lookup expired proactive
      //    path. If exist - start path error procedure towards path to
      //    root
      if (result.retransmitter == Mac48Address::GetBroadcast ())
        {
          result = m_rtable->LookupProactiveExpired ();
        }
      if (result.retransmitter != Mac48Address::GetBroadcast ())
        {
          std::vector<FailedDestination> destinations = m_rtable->GetUnreachableDestinations (result.retransmitter);
          InitiatePathError (MakePathError (destinations));
        }
      m_stats.totalDropped++;
      return false;
    }
  //Request a destination:
  result = m_rtable->LookupReactiveExpired (destination);
  if (ShouldSendPreq (destination))
    {
      uint32_t originator_seqno = GetNextHwmpSeqno ();
      uint32_t dst_seqno = 0;
      if (result.retransmitter != Mac48Address::GetBroadcast ())
        {
          dst_seqno = result.seqnum;
        }
      m_stats.initiatedPreq++;
      for (HwmpProtocolMacMap::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
        {
          i->second->RequestDestination (destination, originator_seqno, dst_seqno);
        }
    }
  QueuedPacket pkt;
  pkt.pkt = packet;
  pkt.dst = destination;
  pkt.src = source;
  pkt.protocol = protocolType;
  pkt.reply = routeReply;
  pkt.inInterface = sourceIface;
  if (QueuePacket (pkt))
    {
      m_stats.totalQueued++;
      return true;
    }
  else
    {
      m_stats.totalDropped++;
      return false;
    }
}
void
HwmpProtocol::ReceivePreq (IePreq preq, Mac48Address from, uint32_t interface, Mac48Address fromMp, uint32_t metric)
{
  preq.IncrementMetric (metric);
  //acceptance cretirea:
  std::map<Mac48Address, std::pair<uint32_t, uint32_t> >::const_iterator i = m_hwmpSeqnoMetricDatabase.find (
      preq.GetOriginatorAddress ());
  bool freshInfo (true);
  if (i != m_hwmpSeqnoMetricDatabase.end ())
    {
      if ((int32_t)(i->second.first - preq.GetOriginatorSeqNumber ())  > 0)
        {
          return;
        }
      if (i->second.first == preq.GetOriginatorSeqNumber ())
        {
          freshInfo = false;
          if (i->second.second <= preq.GetMetric ())
            {
              return;
            }
        }
    }
  m_hwmpSeqnoMetricDatabase[preq.GetOriginatorAddress ()] =
    std::make_pair (preq.GetOriginatorSeqNumber (), preq.GetMetric ());
  NS_LOG_DEBUG ("I am " << GetAddress () << "Accepted preq from address" << from << ", preq:" << preq);
  std::vector<Ptr<DestinationAddressUnit> > destinations = preq.GetDestinationList ();
  //Add reactive path to originator:
  if (
    (freshInfo) ||
    (
      (m_rtable->LookupReactive (preq.GetOriginatorAddress ()).retransmitter == Mac48Address::GetBroadcast ()) ||
      (m_rtable->LookupReactive (preq.GetOriginatorAddress ()).metric > preq.GetMetric ())
    )
    )
    {
      m_rtable->AddReactivePath (
        preq.GetOriginatorAddress (),
        from,
        interface,
        preq.GetMetric (),
        MicroSeconds (preq.GetLifetime () * 1024),
        preq.GetOriginatorSeqNumber ()
        );
      ReactivePathResolved (preq.GetOriginatorAddress ());
    }
  if (
    (m_rtable->LookupReactive (fromMp).retransmitter == Mac48Address::GetBroadcast ()) ||
    (m_rtable->LookupReactive (fromMp).metric > metric)
    )
    {
      m_rtable->AddReactivePath (
        fromMp,
        from,
        interface,
        metric,
        MicroSeconds (preq.GetLifetime () * 1024),
        preq.GetOriginatorSeqNumber ()
        );
      ReactivePathResolved (fromMp);
    }
  for (std::vector<Ptr<DestinationAddressUnit> >::const_iterator i = destinations.begin (); i != destinations.end (); i++)
    {
      if ((*i)->GetDestinationAddress () == Mac48Address::GetBroadcast ())
        {
          //only proactive PREQ contains destination
          //address as broadcast! Proactive preq MUST
          //have destination count equal to 1 and
          //per destination flags DO and RF
          NS_ASSERT (preq.GetDestCount () == 1);
          NS_ASSERT (((*i)->IsDo ()) && ((*i)->IsRf ()));
          //Add proactive path only if it is the better then existed
          //before
          if (
            ((m_rtable->LookupProactive ()).retransmitter == Mac48Address::GetBroadcast ()) ||
            ((m_rtable->LookupProactive ()).metric > preq.GetMetric ())
            )
            {
              m_rtable->AddProactivePath (
                preq.GetMetric (),
                preq.GetOriginatorAddress (),
                from,
                interface,
                MicroSeconds (preq.GetLifetime () * 1024),
                preq.GetOriginatorSeqNumber ()
                );
              ProactivePathResolved ();
            }
          if (!preq.IsNeedNotPrep ())
            {
              SendPrep (
                GetAddress (),
                preq.GetOriginatorAddress (),
                from,
                (uint32_t)0,
                preq.GetOriginatorSeqNumber (),
                GetNextHwmpSeqno (),
                preq.GetLifetime (),
                interface
                );
            }
          break;
        }
      if ((*i)->GetDestinationAddress () == GetAddress ())
        {
          SendPrep (
            GetAddress (),
            preq.GetOriginatorAddress (),
            from,
            (uint32_t)0,
            preq.GetOriginatorSeqNumber (),
            GetNextHwmpSeqno (),
            preq.GetLifetime (),
            interface
            );
          NS_ASSERT (m_rtable->LookupReactive (preq.GetOriginatorAddress ()).retransmitter != Mac48Address::GetBroadcast ());
          preq.DelDestinationAddressElement ((*i)->GetDestinationAddress ());
          continue;
        }
      //check if can answer:
      HwmpRtable::LookupResult result = m_rtable->LookupReactive ((*i)->GetDestinationAddress ());
      if ((!((*i)->IsDo ())) && (result.retransmitter != Mac48Address::GetBroadcast ()))
        {
          //have a valid information and can answer
          uint32_t lifetime = result.lifetime.GetMicroSeconds () / 1024;
          if ((lifetime > 0) && ((int32_t)(result.seqnum - (*i)->GetDestSeqNumber ()) >= 0))
            {
              SendPrep (
                (*i)->GetDestinationAddress (),
                preq.GetOriginatorAddress (),
                from,
                result.metric,
                preq.GetOriginatorSeqNumber (),
                result.seqnum,
                lifetime,
                interface
                );
              m_rtable->AddPrecursor ((*i)->GetDestinationAddress (), interface, from,
                                      MicroSeconds (preq.GetLifetime () * 1024));
              if ((*i)->IsRf ())
                {
                  (*i)->SetFlags (true, false, (*i)->IsUsn ()); //DO = 1, RF = 0
                }
              else
                {
                  preq.DelDestinationAddressElement ((*i)->GetDestinationAddress ());
                  continue;
                }
            }
        }
    }
  //check if must retransmit:
  if (preq.GetDestCount () == 0)
    {
      return;
    }
  //Forward PREQ to all interfaces:
  NS_LOG_DEBUG ("I am " << GetAddress () << "retransmitting PREQ:" << preq);
  for (HwmpProtocolMacMap::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      i->second->SendPreq (preq);
    }
}
void
HwmpProtocol::ReceivePrep (IePrep prep, Mac48Address from, uint32_t interface, Mac48Address fromMp, uint32_t metric)
{
  prep.IncrementMetric (metric);
  //acceptance cretirea:
  std::map<Mac48Address, std::pair<uint32_t, uint32_t> >::const_iterator i = m_hwmpSeqnoMetricDatabase.find (
      prep.GetOriginatorAddress ());
  bool freshInfo (true);
  if (i != m_hwmpSeqnoMetricDatabase.end ())
    {
      if ((int32_t)(i->second.first - prep.GetOriginatorSeqNumber ()) > 0)
        {
          return;
        }
      if (i->second.first == prep.GetOriginatorSeqNumber ())
        {
          freshInfo = false;
        }
    }
  m_hwmpSeqnoMetricDatabase[prep.GetOriginatorAddress ()] = std::make_pair (prep.GetOriginatorSeqNumber (), prep.GetMetric ());
  //update routing info
  //Now add a path to destination and add precursor to source
  NS_LOG_DEBUG ("I am " << GetAddress () << ", received prep from " << prep.GetOriginatorAddress () << ", receiver was:" << from);
  HwmpRtable::LookupResult result = m_rtable->LookupReactive (prep.GetDestinationAddress ());
  //Add a reactive path only if seqno is fresher or it improves the
  //metric
  if (
    (freshInfo) ||
    (
      ((m_rtable->LookupReactive (prep.GetOriginatorAddress ())).retransmitter == Mac48Address::GetBroadcast ()) ||
      ((m_rtable->LookupReactive (prep.GetOriginatorAddress ())).metric > prep.GetMetric ())
    )
    )
    {
      m_rtable->AddReactivePath (
        prep.GetOriginatorAddress (),
        from,
        interface,
        prep.GetMetric (),
        MicroSeconds (prep.GetLifetime () * 1024),
        prep.GetOriginatorSeqNumber ());
      m_rtable->AddPrecursor (prep.GetDestinationAddress (), interface, from,
                              MicroSeconds (prep.GetLifetime () * 1024));
      if (result.retransmitter != Mac48Address::GetBroadcast ())
        {
          m_rtable->AddPrecursor (prep.GetOriginatorAddress (), interface, result.retransmitter,
                                  result.lifetime);
        }
      ReactivePathResolved (prep.GetOriginatorAddress ());
    }
  if (
    ((m_rtable->LookupReactive (fromMp)).retransmitter == Mac48Address::GetBroadcast ()) ||
    ((m_rtable->LookupReactive (fromMp)).metric > metric)
    )
    {
      m_rtable->AddReactivePath (
        fromMp,
        from,
        interface,
        metric,
        MicroSeconds (prep.GetLifetime () * 1024),
        prep.GetOriginatorSeqNumber ());
      ReactivePathResolved (fromMp);
    }
  if (prep.GetDestinationAddress () == GetAddress ())
    {
      NS_LOG_DEBUG ("I am "<<GetAddress ()<<", resolved "<<prep.GetOriginatorAddress ());
      return;
    }
  if (result.retransmitter == Mac48Address::GetBroadcast ())
    {
      return;
    }
  //Forward PREP
  HwmpProtocolMacMap::const_iterator prep_sender = m_interfaces.find (result.ifIndex);
  NS_ASSERT (prep_sender != m_interfaces.end ());
  prep_sender->second->SendPrep (prep, result.retransmitter);
}
void
HwmpProtocol::ReceivePerr (std::vector<FailedDestination> destinations, Mac48Address from, uint32_t interface, Mac48Address fromMp)
{
  //Acceptance cretirea:
  NS_LOG_DEBUG ("I am "<<GetAddress ()<<", received PERR from "<<from);
  std::vector<FailedDestination> retval;
  HwmpRtable::LookupResult result;
  for (unsigned int i = 0; i < destinations.size (); i++)
    {
      result = m_rtable->LookupReactiveExpired (destinations[i].destination);
      if (!(
            (result.retransmitter != from) ||
            (result.ifIndex != interface) ||
            ((int32_t)(result.seqnum - destinations[i].seqnum) > 0)
            ))
        {
          retval.push_back (destinations[i]);
        }
    }
  if (retval.size () == 0)
    {
      return;
    }
  ForwardPathError (MakePathError (retval));
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
  IePrep prep;
  prep.SetHopcount (0);
  prep.SetTtl (m_maxTtl);
  prep.SetDestinationAddress (dst);
  prep.SetDestinationSeqNumber (destinationSN);
  prep.SetLifetime (lifetime);
  prep.SetMetric (initMetric);
  prep.SetOriginatorAddress (src);
  prep.SetOriginatorSeqNumber (originatorDsn);
  HwmpProtocolMacMap::const_iterator prep_sender = m_interfaces.find (interface);
  NS_ASSERT (prep_sender != m_interfaces.end ());
  prep_sender->second->SendPrep (prep, retransmitter);
  m_stats.initiatedPrep++;
}
bool
HwmpProtocol::Install (Ptr<MeshPointDevice> mp)
{
  m_mp = mp;
  std::vector<Ptr<NetDevice> > interfaces = mp->GetInterfaces ();
  for (std::vector<Ptr<NetDevice> >::const_iterator i = interfaces.begin (); i != interfaces.end (); i++)
    {
      // Checking for compatible net device
      Ptr<WifiNetDevice> wifiNetDev = (*i)->GetObject<WifiNetDevice> ();
      if (wifiNetDev == 0)
        {
          return false;
        }
      Ptr<MeshWifiInterfaceMac>  mac = wifiNetDev->GetMac ()->GetObject<MeshWifiInterfaceMac> ();
      if (mac == 0)
        {
          return false;
        }
      // Installing plugins:
      Ptr<HwmpProtocolMac> hwmpMac = Create<HwmpProtocolMac> (wifiNetDev->GetIfIndex (), this);
      m_interfaces[wifiNetDev->GetIfIndex ()] = hwmpMac;
      mac->InstallPlugin (hwmpMac);
      //Installing airtime link metric:
      Ptr<AirtimeLinkMetricCalculator> metric = CreateObject <AirtimeLinkMetricCalculator> ();
      mac->SetLinkMetricCallback (MakeCallback (&AirtimeLinkMetricCalculator::CalculateMetric, metric));
    }
  mp->SetRoutingProtocol (this);
  // Mesh point aggregates all installed protocols
  mp->AggregateObject (this);
  m_address = Mac48Address::ConvertFrom (mp->GetAddress ()); // address;
  return true;
}
void
HwmpProtocol::PeerLinkStatus (Mac48Address meshPointAddress, Mac48Address peerAddress, uint32_t interface, bool status)
{
  if (status)
    {
      return;
    }
  std::vector<FailedDestination> destinations = m_rtable->GetUnreachableDestinations (peerAddress);
  InitiatePathError (MakePathError (destinations));
}
void
HwmpProtocol::SetNeighboursCallback (Callback<std::vector<Mac48Address>, uint32_t> cb)
{
  m_neighboursCallback = cb;
}
bool
HwmpProtocol::DropDataFrame (uint32_t seqno, Mac48Address source)
{
  if (source == GetAddress ())
    {
      return true;
    }
  std::map<Mac48Address, uint32_t,std::less<Mac48Address> >::const_iterator i = m_lastDataSeqno.find (source);
  if (i == m_lastDataSeqno.end ())
    {
      m_lastDataSeqno[source] = seqno;
    }
  else
    {
      if ((int32_t)(i->second - seqno)  >= 0)
        {
          return true;
        }
      m_lastDataSeqno[source] = seqno;
    }
  return false;
}
HwmpProtocol::PathError
HwmpProtocol::MakePathError (std::vector<FailedDestination> destinations)
{
  PathError retval;
  //HwmpRtable increments a sequence number as written in 11B.9.7.2
  retval.receivers = GetPerrReceivers (destinations);
  if (retval.receivers.size () == 0)
    {
      return retval;
    }
  m_stats.initiatedPerr++;
  for (unsigned int i = 0; i < destinations.size (); i++)
    {
      retval.destinations.push_back (destinations[i]);
      m_rtable->DeleteReactivePath (destinations[i].destination);
    }
  return retval;
}
void
HwmpProtocol::InitiatePathError (PathError perr)
{
  for (HwmpProtocolMacMap::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      std::vector<Mac48Address> receivers_for_interface;
      for (unsigned int j = 0; j < perr.receivers.size (); j++)
        {
          if (i->first == perr.receivers[j].first)
            {
              receivers_for_interface.push_back (perr.receivers[j].second);
            }
        }
      i->second->InitiatePerr (perr.destinations, receivers_for_interface);
    }
}
void
HwmpProtocol::ForwardPathError (PathError perr)
{
  for (HwmpProtocolMacMap::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      std::vector<Mac48Address> receivers_for_interface;
      for (unsigned int j = 0; j < perr.receivers.size (); j++)
        {
          if (i->first == perr.receivers[j].first)
            {
              receivers_for_interface.push_back (perr.receivers[j].second);
            }
        }
      i->second->ForwardPerr (perr.destinations, receivers_for_interface);
    }
}

std::vector<std::pair<uint32_t, Mac48Address> >
HwmpProtocol::GetPerrReceivers (std::vector<FailedDestination> failedDest)
{
  HwmpRtable::PrecursorList retval;
  for (unsigned int i = 0; i < failedDest.size (); i++)
    {
      HwmpRtable::PrecursorList precursors = m_rtable->GetPrecursors (failedDest[i].destination);
      m_rtable->DeleteReactivePath (failedDest[i].destination);
      m_rtable->DeleteProactivePath (failedDest[i].destination);
      for (unsigned int j = 0; j < precursors.size (); j++)
        {
          retval.push_back (precursors[j]);
        }
    }
  //Check if we have dublicates in retval and precursors:
  for (unsigned int i = 0; i < retval.size (); i++)
    {
      for (unsigned int j = i+1; j < retval.size (); j++)
        {
          if (retval[i].second == retval[j].second)
            {
              retval.erase (retval.begin () + j);
            }
        }
    }
  return retval;
}
std::vector<Mac48Address>
HwmpProtocol::GetPreqReceivers (uint32_t interface)
{
  std::vector<Mac48Address> retval;
  if (!m_neighboursCallback.IsNull ())
    {
      retval = m_neighboursCallback (interface);
    }
  if ((retval.size () >= m_unicastPreqThreshold) || (retval.size () == 0))
    {
      retval.clear ();
      retval.push_back (Mac48Address::GetBroadcast ());
    }
  return retval;
}
std::vector<Mac48Address>
HwmpProtocol::GetBroadcastReceivers (uint32_t interface)
{
  std::vector<Mac48Address> retval;
  if (!m_neighboursCallback.IsNull ())
    {
      retval = m_neighboursCallback (interface);
    }
  if ((retval.size () >= m_unicastDataThreshold) || (retval.size () == 0))
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
    {
      return false;
    }
  m_rqueue.push_back (packet);
  return true;
}

HwmpProtocol::QueuedPacket
HwmpProtocol::DequeueFirstPacketByDst (Mac48Address dst)
{
  QueuedPacket retval;
  retval.pkt = 0;
  for (std::vector<QueuedPacket>::iterator i = m_rqueue.begin (); i != m_rqueue.end (); i++)
    {
      if ((*i).dst == dst)
        {
          retval = (*i);
          m_rqueue.erase (i);
          break;
        }
    }
  return retval;
}

HwmpProtocol::QueuedPacket
HwmpProtocol::DequeueFirstPacket ()
{
  QueuedPacket retval;
  retval.pkt = 0;
  if (m_rqueue.size () != 0)
    {
      retval = m_rqueue[0];
      m_rqueue.erase (m_rqueue.begin ());
    }
  return retval;
}

void
HwmpProtocol::ReactivePathResolved (Mac48Address dst)
{
  std::map<Mac48Address, PreqEvent>::iterator i = m_preqTimeouts.find (dst);
  if (i != m_preqTimeouts.end ())
    {
      m_routeDiscoveryTimeCallback (Simulator::Now () - i->second.whenScheduled);
    }

  HwmpRtable::LookupResult result = m_rtable->LookupReactive (dst);
  NS_ASSERT (result.retransmitter != Mac48Address::GetBroadcast ());
  //Send all packets stored for this destination
  QueuedPacket packet = DequeueFirstPacketByDst (dst);
  while (packet.pkt != 0)
    {
      //set RA tag for retransmitter:
      HwmpTag tag;
      packet.pkt->RemovePacketTag (tag);
      tag.SetAddress (result.retransmitter);
      packet.pkt->AddPacketTag (tag);
      m_stats.txUnicast++;
      m_stats.txBytes += packet.pkt->GetSize ();
      packet.reply (true, packet.pkt, packet.src, packet.dst, packet.protocol, result.ifIndex);

      packet = DequeueFirstPacketByDst (dst);
    }
}
void
HwmpProtocol::ProactivePathResolved ()
{
  //send all packets to root
  HwmpRtable::LookupResult result = m_rtable->LookupProactive ();
  NS_ASSERT (result.retransmitter != Mac48Address::GetBroadcast ());
  QueuedPacket packet = DequeueFirstPacket ();
  while (packet.pkt != 0)
    {
      //set RA tag for retransmitter:
      HwmpTag tag;
      if (!packet.pkt->RemovePacketTag (tag))
        {
          NS_FATAL_ERROR ("HWMP tag must be present at this point");
        }
      tag.SetAddress (result.retransmitter);
      packet.pkt->AddPacketTag (tag);
      m_stats.txUnicast++;
      m_stats.txBytes += packet.pkt->GetSize ();
      packet.reply (true, packet.pkt, packet.src, packet.dst, packet.protocol, result.ifIndex);

      packet = DequeueFirstPacket ();
    }
}

bool
HwmpProtocol::ShouldSendPreq (Mac48Address dst)
{
  std::map<Mac48Address, PreqEvent>::const_iterator i = m_preqTimeouts.find (dst);
  if (i == m_preqTimeouts.end ())
    {
      m_preqTimeouts[dst].preqTimeout = Simulator::Schedule (
          Time (m_dot11MeshHWMPnetDiameterTraversalTime * 2),
          &HwmpProtocol::RetryPathDiscovery, this, dst, 1);
      m_preqTimeouts[dst].whenScheduled = Simulator::Now ();
      return true;
    }
  return false;
}
void
HwmpProtocol::RetryPathDiscovery (Mac48Address dst, uint8_t numOfRetry)
{
  HwmpRtable::LookupResult result = m_rtable->LookupReactive (dst);
  if (result.retransmitter == Mac48Address::GetBroadcast ())
    {
      result = m_rtable->LookupProactive ();
    }
  if (result.retransmitter != Mac48Address::GetBroadcast ())
    {
      std::map<Mac48Address, PreqEvent>::iterator i = m_preqTimeouts.find (dst);
      NS_ASSERT (i != m_preqTimeouts.end ());
      m_preqTimeouts.erase (i);
      return;
    }
  if (numOfRetry > m_dot11MeshHWMPmaxPREQretries)
    {
      QueuedPacket packet = DequeueFirstPacketByDst (dst);
      //purge queue and delete entry from retryDatabase
      while (packet.pkt != 0)
        {
          m_stats.totalDropped++;
          packet.reply (false, packet.pkt, packet.src, packet.dst, packet.protocol, HwmpRtable::MAX_METRIC);
          packet = DequeueFirstPacketByDst (dst);
        }
      std::map<Mac48Address, PreqEvent>::iterator i = m_preqTimeouts.find (dst);
      NS_ASSERT (i != m_preqTimeouts.end ());
      m_routeDiscoveryTimeCallback (Simulator::Now () - i->second.whenScheduled);
      m_preqTimeouts.erase (i);
      return;
    }
  numOfRetry++;
  uint32_t originator_seqno = GetNextHwmpSeqno ();
  uint32_t dst_seqno = m_rtable->LookupReactiveExpired (dst).seqnum;
  for (HwmpProtocolMacMap::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      i->second->RequestDestination (dst, originator_seqno, dst_seqno);
    }
  m_preqTimeouts[dst].preqTimeout = Simulator::Schedule (
      Time ((2 * (numOfRetry + 1)) *  m_dot11MeshHWMPnetDiameterTraversalTime),
      &HwmpProtocol::RetryPathDiscovery, this, dst, numOfRetry);
}
//Proactive PREQ routines:
void
HwmpProtocol::SetRoot ()
{
  UniformVariable coefficient (0.0, m_randomStart.GetSeconds ());
  Time randomStart = Seconds (coefficient.GetValue ());
  m_proactivePreqTimer = Simulator::Schedule (randomStart, &HwmpProtocol::SendProactivePreq, this);
  NS_LOG_DEBUG ("ROOT IS: " << m_address);
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
  IePreq preq;
  //By default: must answer
  preq.SetHopcount (0);
  preq.SetTTL (m_maxTtl);
  preq.SetLifetime (m_dot11MeshHWMPactiveRootTimeout.GetMicroSeconds () /1024);
  //\attention: do not forget to set originator address, sequence
  //number and preq ID in HWMP-MAC plugin
  preq.AddDestinationAddressElement (true, true, Mac48Address::GetBroadcast (), 0);
  preq.SetOriginatorAddress (GetAddress ());
  preq.SetPreqID (GetNextPreqId ());
  preq.SetOriginatorSeqNumber (GetNextHwmpSeqno ());
  for (HwmpProtocolMacMap::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      i->second->SendPreq (preq);
    }
  m_proactivePreqTimer = Simulator::Schedule (m_dot11MeshHWMPpathToRootInterval, &HwmpProtocol::SendProactivePreq, this);
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
  m_preqId++;
  return m_preqId;
}
uint32_t
HwmpProtocol::GetNextHwmpSeqno ()
{
  m_hwmpSeqno++;
  return m_hwmpSeqno;
}
uint32_t
HwmpProtocol::GetActivePathLifetime ()
{
  return m_dot11MeshHWMPactivePathTimeout.GetMicroSeconds () / 1024;
}
uint8_t
HwmpProtocol::GetUnicastPerrThreshold ()
{
  return m_unicastPerrThreshold;
}
Mac48Address
HwmpProtocol::GetAddress ()
{
  return m_address;
}
//Statistics:
HwmpProtocol::Statistics::Statistics () :
  txUnicast (0),
  txBroadcast (0),
  txBytes (0),
  droppedTtl (0),
  totalQueued (0),
  totalDropped (0),
  initiatedPreq (0),
  initiatedPrep (0),
  initiatedPerr (0)
{
}
void HwmpProtocol::Statistics::Print (std::ostream & os) const
{
  os << "<Statistics "
  "txUnicast=\"" << txUnicast << "\" "
  "txBroadcast=\"" << txBroadcast << "\" "
  "txBytes=\"" << txBytes << "\" "
  "droppedTtl=\"" << droppedTtl << "\" "
  "totalQueued=\"" << totalQueued << "\" "
  "totalDropped=\"" << totalDropped << "\" "
  "initiatedPreq=\"" << initiatedPreq << "\" "
  "initiatedPrep=\"" << initiatedPrep << "\" "
  "initiatedPerr=\"" << initiatedPerr << "\"/>" << std::endl;
}
void
HwmpProtocol::Report (std::ostream & os) const
{
  os << "<Hwmp "
  "address=\"" << m_address << "\"" << std::endl <<
  "maxQueueSize=\"" << m_maxQueueSize << "\"" << std::endl <<
  "Dot11MeshHWMPmaxPREQretries=\"" << (uint16_t)m_dot11MeshHWMPmaxPREQretries << "\"" << std::endl <<
  "Dot11MeshHWMPnetDiameterTraversalTime=\"" << m_dot11MeshHWMPnetDiameterTraversalTime.GetSeconds () << "\"" << std::endl <<
  "Dot11MeshHWMPpreqMinInterval=\"" << m_dot11MeshHWMPpreqMinInterval.GetSeconds () << "\"" << std::endl <<
  "Dot11MeshHWMPperrMinInterval=\"" << m_dot11MeshHWMPperrMinInterval.GetSeconds () << "\"" << std::endl <<
  "Dot11MeshHWMPactiveRootTimeout=\"" << m_dot11MeshHWMPactiveRootTimeout.GetSeconds () << "\"" << std::endl <<
  "Dot11MeshHWMPactivePathTimeout=\"" << m_dot11MeshHWMPactivePathTimeout.GetSeconds () << "\"" << std::endl <<
  "Dot11MeshHWMPpathToRootInterval=\"" << m_dot11MeshHWMPpathToRootInterval.GetSeconds () << "\"" << std::endl <<
  "Dot11MeshHWMPrannInterval=\"" << m_dot11MeshHWMPrannInterval.GetSeconds () << "\"" << std::endl <<
  "isRoot=\"" << m_isRoot << "\"" << std::endl <<
  "maxTtl=\"" << (uint16_t)m_maxTtl << "\"" << std::endl <<
  "unicastPerrThreshold=\"" << (uint16_t)m_unicastPerrThreshold << "\"" << std::endl <<
  "unicastPreqThreshold=\"" << (uint16_t)m_unicastPreqThreshold << "\"" << std::endl <<
  "unicastDataThreshold=\"" << (uint16_t)m_unicastDataThreshold << "\"" << std::endl <<
  "doFlag=\"" << m_doFlag << "\"" << std::endl <<
  "rfFlag=\"" << m_rfFlag << "\">" << std::endl;
  m_stats.Print (os);
  for (HwmpProtocolMacMap::const_iterator plugin = m_interfaces.begin (); plugin != m_interfaces.end (); plugin++)
    {
      plugin->second->Report (os);
    }
  os << "</Hwmp>" << std::endl;
}
void
HwmpProtocol::ResetStats ()
{
  m_stats = Statistics ();
  for (HwmpProtocolMacMap::const_iterator plugin = m_interfaces.begin (); plugin != m_interfaces.end (); plugin++)
    {
      plugin->second->ResetStats ();
    }
}
HwmpProtocol::QueuedPacket::QueuedPacket () :
  pkt (0),
  protocol (0),
  inInterface (0)
{
}
} //namespace dot11s
} //namespace ns3
