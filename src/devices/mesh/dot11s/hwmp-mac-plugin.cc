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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/mesh-wifi-mac-header.h"

#include "hwmp-mac-plugin.h"
#include "hwmp-protocol.h"
#include "hwmp-tag.h"
#include "ie-dot11s-preq.h"
#include "ie-dot11s-prep.h"
#include "ie-dot11s-perr.h"

namespace ns3 {
namespace dot11s {

NS_LOG_COMPONENT_DEFINE ("HwmpMacPlugin");
HwmpMacPlugin::HwmpMacPlugin (uint32_t ifIndex, Ptr<HwmpProtocol> protocol)
{
  m_ifIndex = ifIndex;
  m_protocol = protocol;
}
HwmpMacPlugin::~HwmpMacPlugin ()
{
}
void
HwmpMacPlugin::SetParent (Ptr<MeshWifiInterfaceMac> parent)
{
  m_parent = parent;
}
bool
HwmpMacPlugin::Receive (Ptr<Packet> packet, const WifiMacHeader & header)
{
  //TODO: here we fix only mesh header
  if(header.IsData())
  {
    WifiMeshHeader meshHdr;
    HwmpTag tag;
    NS_ASSERT(! packet->PeekPacketTag (tag));
    packet->RemoveHeader(meshHdr);
    //TODO: address extension
    Mac48Address destination;
    switch (meshHdr.GetAddressExt ())
    {
      case 0:
        destination = header.GetAddr3 ();
        break;
      default:
        NS_ASSERT(false);
    };
    tag.SetSeqno (meshHdr.GetMeshSeqno ());
    if(meshHdr.GetMeshTtl () == 0)
      return false;
    tag.SetTtl (meshHdr.GetMeshTtl () - 1);
    tag.SetAddress (header.GetAddr2 ());
    packet->AddPacketTag(tag);
    if (destination == Mac48Address::GetBroadcast ())
      if(m_protocol->DropDataFrame (meshHdr.GetMeshSeqno (), header.GetAddr4 ()) )
        return false;
  }
  if(header.IsMultihopAction())
  {
    WifiMeshHeader meshHdr;
    packet->RemoveHeader (meshHdr);
    //parse multihop action header:
    WifiMeshMultihopActionHeader multihopHdr;
    packet->RemoveHeader (multihopHdr);
    WifiMeshMultihopActionHeader::ACTION_VALUE actionValue = multihopHdr.GetAction ();
    if(multihopHdr.GetCategory () != WifiMeshMultihopActionHeader::MESH_PATH_SELECTION)
      return true;
    if(meshHdr.GetMeshTtl () == 0)
        return false;
    switch (actionValue.pathSelection)
    {
      case WifiMeshMultihopActionHeader::PATH_REQUEST:
        {
          IePreq preq;
          packet->RemoveHeader (preq);
          if (preq.GetOriginatorAddress () == m_parent->GetAddress ())
            return false;
          if (preq.GetTtl () == 0)
            return false;
          preq.DecrementTtl ();
          m_protocol->ReceivePreq (preq, header.GetAddr2 (), m_ifIndex);
          return false;
        }
      case WifiMeshMultihopActionHeader::PATH_REPLY:
        {
          IePrep prep;
          packet->RemoveHeader (prep);
          if(prep.GetTtl () == 0)
            return false;
          prep.DecrementTtl ();
          m_protocol->ReceivePrep (prep, header.GetAddr2 (), m_ifIndex);
          return false;
        }
      case WifiMeshMultihopActionHeader::PATH_ERROR:
        {
          IePerr perr;
          packet->RemoveHeader (perr);
          m_protocol->ReceivePerr (perr, header.GetAddr2 (), m_ifIndex);
          return false;
        }
      case WifiMeshMultihopActionHeader::ROOT_ANNOUNCEMENT:
        return false;
    }
  }
  return true;
}
bool
HwmpMacPlugin::UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to) const
{
  //TODO: add a mesh header and remove a TAG
  NS_ASSERT(header.IsData ());
  HwmpTag tag;
  NS_ASSERT(packet->RemovePacketTag(tag));
  WifiMeshHeader meshHdr;
  meshHdr.SetMeshSeqno(tag.GetSeqno());
  meshHdr.SetMeshTtl(tag.GetTtl());
  packet->AddHeader(meshHdr);
  header.SetAddr1(tag.GetAddress());
  return true;
}
#if 0
HwmpMacPlugin::HwmpMacPlugin ():
    m_myPreq (m_preqQueue.end()),
{
}

HwmpMacPlugin::~HwmpMacPlugin ()
{
  m_preqQueue.clear ();
}
//Interaction with HWMP:
#endif
void
HwmpMacPlugin::SendPreq(IePreq preq)
{
  //Create packet
  Ptr<Packet> packet  = Create<Packet> ();
  packet->AddHeader(preq);
  //Multihop action header:
  WifiMeshMultihopActionHeader multihopHdr;
  WifiMeshMultihopActionHeader::ACTION_VALUE action;
  action.pathSelection = WifiMeshMultihopActionHeader::PATH_REQUEST;
  multihopHdr.SetAction (WifiMeshMultihopActionHeader::MESH_PATH_SELECTION, action);
  packet->AddHeader (multihopHdr);
  //Mesh header
  WifiMeshHeader meshHdr;
  meshHdr.SetMeshTtl (m_protocol->GetMaxTtl ());
  //TODO: should seqno be here?
  meshHdr.SetMeshSeqno (0);
  meshHdr.SetAddressExt(1);
  meshHdr.SetAddr4(preq.GetOriginatorAddress ());
  packet->AddHeader (meshHdr);
  //create 802.11 header:
  WifiMacHeader hdr;
  hdr.SetMultihopAction ();
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  hdr.SetAddr2 (m_parent->GetAddress ());
  hdr.SetAddr3 (Mac48Address::GetBroadcast ());
  //Send Management frame
  std::vector <Mac48Address> receivers = m_protocol->GetPreqReceivers (m_ifIndex);
  for(std::vector<Mac48Address>::iterator i = receivers.begin (); i != receivers.end (); i ++)
  {
    hdr.SetAddr1 (*i);
    m_parent->SendManagementFrame(packet, hdr);
  }
}
void
HwmpMacPlugin::RequestDestination (Mac48Address dst)
{
  if (m_preqQueue.end () == m_myPreq)
  {
    IePreq preq;
    //fill PREQ:
    preq.SetHopcount (0);
    preq.SetTTL (m_protocol->GetMaxTtl ());
    preq.SetPreqID (m_protocol->GetNextPreqId ());
    preq.SetOriginatorAddress (m_protocol->GetAddress ());
    preq.SetOriginatorSeqNumber (m_protocol->GetNextHwmpSeqno());
    preq.SetLifetime (m_protocol->GetActivePathLifetime ());
    preq.AddDestinationAddressElement (false, false, dst, 0); //DO = 0, RF = 0
    m_preqQueue.push_back (preq);
    //set iterator position to my preq:
    m_myPreq = m_preqQueue.end () -1;
    NS_LOG_UNCOND("no preq");
    SendOnePreq ();
  }
  else
  {
    NS_ASSERT (m_myPreq->GetOriginatorAddress() == m_parent->GetAddress());
    NS_LOG_UNCOND ("add a destination "<<dst);
    m_myPreq->AddDestinationAddressElement (false, false, dst, 0); //DO = 0, RF = 0
  }
}
#if 0
void
HwmpMacPlugin::SendPathError (std::vector<HwmpRtable::FailedDestination> destinations)
{
  std::vector<Mac48Address> receivers =  m_retransmittersOfPerrCallback (destinations, m_ifIndex);
  NS_LOG_DEBUG ("SendPathError started");
  if (receivers.size () == 0)
    return;
  NS_LOG_DEBUG (m_address<<" Should Send PERR to");
  for (unsigned int i = 0; i < receivers.size (); i ++)
    {
      AddPerrReceiver (receivers[i]);
      NS_LOG_DEBUG (receivers[i]);
    }
  NS_LOG_DEBUG ("To tel about failure with");
  for (unsigned int i = 0; i < destinations.size (); i ++)
    {
      m_myPerr.AddAddressUnit (destinations[i]);
      NS_LOG_DEBUG (destinations[i].destination);
    }
  if (!m_perrTimer.IsRunning ())
    {
      m_perrCallback (m_myPerr,m_myPerrReceivers);
      m_myPerr.ResetPerr ();
      m_perrTimer = Simulator::Schedule (dot11sParameters::dot11MeshHWMPperrMinInterval,&HwmpMacPlugin::SendOnePerr,this);
    }
}
//needed to fill routing information structure

void
HwmpMacPlugin::ReceivePrep (IeDot11sPrep& prep, const Mac48Address& from, const uint32_t& metric)
{
  if (m_disabled)
    return;
  prep.DecrementTtl ();
  prep.IncrementMetric (metric);
  //acceptance cretirea:
  std::map<Mac48Address, uint32_t>::iterator i = m_dsnDatabase.find (prep.GetDestinationAddress());
  if (i == m_dsnDatabase.end ())
    {
      m_dsnDatabase[prep.GetDestinationAddress ()] = prep.GetDestinationSeqNumber();
    }
  else
    if (i->second > prep.GetDestinationSeqNumber ())
      return;
  //update routing info
  HwmpRtable::LookupResult result = m_requestRouteCallback (prep.GetDestinationAddress());
  if (result.retransmitter == Mac48Address::GetBroadcast ())
    //try to look for default route
    result = m_requestRootPathCallback (m_ifIndex);
  if ((result.retransmitter == Mac48Address::GetBroadcast ())&&(m_address != prep.GetDestinationAddress()))
    return;
  INFO newInfo;
  newInfo.me = m_address;
  newInfo.destination = prep.GetOriginatorAddress ();
  newInfo.source = prep.GetDestinationAddress ();
  newInfo.nextHop = from;
  newInfo.metric = prep.GetMetric ();
  newInfo.lifetime = TU_TO_TIME (prep.GetLifetime());
  newInfo.outPort = m_ifIndex;
  newInfo.dsn = prep.GetOriginatorSeqNumber ();
  newInfo.prevHop = result.retransmitter;
  newInfo.type = INFO_PREP;
  NS_LOG_DEBUG ("Path to "<<newInfo.source<<", i am "<<m_address<<", precursor is "<<from);
  NS_LOG_DEBUG ("Path to "<<newInfo.destination<<", i am "<<m_address<<", precursor is "<<result.retransmitter);
  m_routingInfoCallback (newInfo);
  if (prep.GetDestinationAddress () == m_address)
    {
      NS_LOG_DEBUG ("Destination resolved"<<newInfo.destination);
      return;
    }
  m_prepCallback (prep, result.retransmitter);
}

void
HwmpMacPlugin::ReceivePerr (IeDot11sPerr& perr, const Mac48Address& from)
{
  if (m_disabled)
    return;
  NS_LOG_DEBUG (m_address<<" RECEIVED PERR from "<<from);
  /**
   * Check forwarding conditions:
   */
  std::vector<HwmpRtable::FailedDestination> destinations = perr.GetAddressUnitVector ();
  for (unsigned int i = 0; i < destinations.size (); i ++)
    {
      /**
       * Lookup for a valid routing information
       */
      HwmpRtable::LookupResult result = m_requestRouteCallback (destinations[i].destination);
      if (
        (result.retransmitter != from)
        || (result.seqnum >= destinations[i].seqnum)
      )

        perr.DeleteAddressUnit (destinations[i].destination);
    }
  NS_LOG_DEBUG ("Retransmit "<<(int)perr.GetNumOfDest());
  if (perr.GetNumOfDest () == 0)
    return;
  destinations = perr.GetAddressUnitVector ();
  SendPathError (destinations);
}

void
HwmpMacPlugin::PeerStatus (const Mac48Address peerAddress, const bool status, const uint32_t metric)
{
  INFO newInfo;
  newInfo.me = m_address;
  newInfo.destination = peerAddress;
  newInfo.nextHop = peerAddress;
  newInfo.metric = metric;
  newInfo.outPort = m_ifIndex;
  newInfo.dsn = 0;
  if (status)
    newInfo.type = INFO_NEW_PEER;
  else
    newInfo.type = INFO_FAILED_PEER;
  m_routingInfoCallback (newInfo);
}
void
HwmpMacPlugin::AddPerrReceiver (Mac48Address receiver)
{
  /**
   * add new vector of addresses to m_perrReceiversand check
   * duplicates
   */
  for (unsigned int j = 0; j < m_myPerrReceivers.size (); j++)
    if (m_myPerrReceivers[j] == receiver)
      return;
  m_myPerrReceivers.push_back (receiver);
}
#endif
void
HwmpMacPlugin::SendOnePreq ()
{
  if(m_preqTimer.IsRunning ())
    return;
  if (m_preqQueue.size () == 0)
    return;
  if (m_myPreq == m_preqQueue.begin ())
    m_myPreq == m_preqQueue.end ();
  NS_LOG_UNCOND ("I am "<<m_parent->GetAddress ()<<"sending PREQ:"<<m_preqQueue[0]);
  SendPreq(m_preqQueue[0]);
  //erase first!
  m_preqQueue.erase (m_preqQueue.begin());
  //reschedule sending PREQ
  NS_ASSERT (!m_preqTimer.IsRunning());
  m_preqTimer = Simulator::Schedule (m_protocol->GetPreqMinInterval (), &HwmpMacPlugin::SendOnePreq, this);
}
void
HwmpMacPlugin::SendPrep (IePrep prep, Mac48Address receiver)
{
  //Create packet
  Ptr<Packet> packet  = Create<Packet> ();
  packet->AddHeader(prep);
  //Multihop action header:
  WifiMeshMultihopActionHeader multihopHdr;
  WifiMeshMultihopActionHeader::ACTION_VALUE action;
  action.pathSelection = WifiMeshMultihopActionHeader::PATH_REPLY;
  multihopHdr.SetAction (WifiMeshMultihopActionHeader::MESH_PATH_SELECTION, action);
  packet->AddHeader (multihopHdr);
  //Mesh header
  WifiMeshHeader meshHdr;
  meshHdr.SetMeshTtl (m_protocol->GetMaxTtl ());
  //TODO: should seqno be here?
  meshHdr.SetMeshSeqno (0);
  meshHdr.SetAddressExt(1);
  meshHdr.SetAddr4(prep.GetOriginatorAddress ());
  packet->AddHeader (meshHdr);
  //create 802.11 header:
  WifiMacHeader hdr;
  hdr.SetMultihopAction ();
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  hdr.SetAddr1 (receiver);
  hdr.SetAddr2 (m_parent->GetAddress ());
  hdr.SetAddr3 (Mac48Address::GetBroadcast ());
  //Send Management frame
  NS_LOG_UNCOND("Sending PREP");
  m_parent->SendManagementFrame(packet, hdr);
}
#if 0
void
HwmpMacPlugin::SendPrep (Mac48Address dst,
                    Mac48Address src,
                    Mac48Address retransmitter,
                    uint32_t initMetric,
                    uint32_t dsn,
                    uint32_t originatorDsn,
                    uint32_t lifetime)
{
  IeDot11sPrep prep;
  prep.SetHopcount (0);
  prep.SetTTL (m_maxTtl);
  prep.SetDestinationAddress (dst);
  prep.SetDestinationSeqNumber (dsn);
  prep.SetLifetime (lifetime);
  prep.SetMetric (0);
  prep.SetOriginatorAddress (src);
  prep.SetOriginatorSeqNumber (originatorDsn);
  //m_prepCallback (prep, retransmitter);
}

void
HwmpMacPlugin::SendOnePerr ()
{
  if (m_myPerr.GetNumOfDest () == 0)
    return;
  m_perrCallback (m_myPerr, m_myPerrReceivers);
  m_myPerr.ResetPerr ();
}
#endif
} //namespace dot11s
}//namespace ns3
