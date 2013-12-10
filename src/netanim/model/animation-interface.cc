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
 * Author: George F. Riley<riley@ece.gatech.edu>
 * Modified by: John Abraham <john.abraham@gatech.edu>
 * Contributions: Eugene Kalishenko <ydginster@gmail.com> (Open Source and Linux Laboratory http://dev.osll.ru/)
 */

// Interface between ns3 and the network animator


// ns3 includes
#include "ns3/animation-interface.h"
#include "ns3/channel.h"
#include "ns3/config.h"
#include "ns3/node.h"
#include "ns3/mobility-model.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/animation-interface-helper.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/wimax-mac-header.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-mac.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/lte-ue-phy.h"
#include "ns3/lte-enb-phy.h"
#include "ns3/uan-net-device.h"
#include "ns3/uan-mac.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/energy-source-container.h"

#include <cstdio>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>

NS_LOG_COMPONENT_DEFINE ("AnimationInterface");

namespace ns3 {

#define PURGE_INTERVAL 5

static bool initialized = false;
std::map <uint32_t, std::string> AnimationInterface::nodeDescriptions;
std::map <uint32_t, Rgb> AnimationInterface::nodeColors;
std::map <P2pLinkNodeIdPair, LinkProperties, LinkPairCompare> AnimationInterface::linkProperties;
Rectangle * AnimationInterface::userBoundary = 0;


AnimationInterface::AnimationInterface (const std::string fn, uint64_t maxPktsPerFile, bool usingXML)
  : m_routingF (0), m_xml (usingXML), m_mobilityPollInterval (Seconds(0.25)), 
    m_outputFileName (fn),
    m_outputFileSet (false), gAnimUid (0), m_randomPosition (true),
    m_writeCallback (0), m_started (false), 
    m_enablePacketMetadata (false), m_startTime (Seconds(0)), m_stopTime (Seconds(3600 * 1000)),
    m_maxPktsPerFile (maxPktsPerFile), m_originalFileName (fn),
    m_routingStopTime (Seconds (0)), m_routingFileName (""),
    m_routingPollInterval (Seconds (5))
{
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
  initialized = true;
  StartAnimation ();
}

AnimationInterface::~AnimationInterface ()
{
  if (userBoundary)
    {
      delete userBoundary;
    }
  StopAnimation ();
}

AnimationInterface & AnimationInterface::EnableIpv4RouteTracking (std::string fileName, Time startTime, Time stopTime, Time pollInterval)
{
  m_routingFileName = fileName;
  SetRoutingOutputFile (m_routingFileName);
  m_routingStopTime = stopTime;
  m_routingPollInterval = pollInterval;
  WriteN (GetXMLOpen_anim (0), m_routingF); 
  Simulator::Schedule (startTime, &AnimationInterface::TrackIpv4Route, this);
  return *this;
}

AnimationInterface & AnimationInterface::EnableIpv4RouteTracking (std::string fileName, Time startTime, Time stopTime, NodeContainer nc, Time pollInterval)
{
  m_routingNc = nc;
  return EnableIpv4RouteTracking (fileName, startTime, stopTime, pollInterval);
}

std::string AnimationInterface::GetIpv4RoutingTable (Ptr <Node> n)
{

  NS_ASSERT (n);
  Ptr <ns3::Ipv4> ipv4 = n->GetObject <ns3::Ipv4> ();
  if (!ipv4)
    {
      NS_LOG_WARN ("Node " << n->GetId () << " Does not have an Ipv4 object");
      return "";
    }
  std::stringstream stream;
  Ptr<OutputStreamWrapper> routingstream = Create<OutputStreamWrapper> (&stream);
  ipv4->GetRoutingProtocol ()->PrintRoutingTable (routingstream);
  return stream.str();

}

void AnimationInterface::RecursiveIpv4RoutePathSearch (std::string from, std::string to, Ipv4RoutePathElements & rpElements)
{
  NS_LOG_INFO ("RecursiveIpv4RoutePathSearch from:" << from.c_str () << " to:" << to.c_str ());
  if ((from == "0.0.0.0") || (from == "127.0.0.1"))
    {
      NS_LOG_INFO ("Got " << from.c_str () << " End recursion");
      return;
    }
  Ptr <Node> fromNode = NodeList::GetNode (m_ipv4ToNodeIdMap[from]);
  Ptr <Node> toNode = NodeList::GetNode (m_ipv4ToNodeIdMap[to]);
  if (fromNode->GetId () == toNode->GetId ())
    {
      Ipv4RoutePathElement elem = { fromNode->GetId (), "L" };
      rpElements.push_back (elem);
      return;
    }
  if (!fromNode)
    {
      NS_FATAL_ERROR ("Node: " << m_ipv4ToNodeIdMap[from] << " Not found");
      return;
    }
  if (!toNode)
    {
      NS_FATAL_ERROR ("Node: " << m_ipv4ToNodeIdMap[to] << " Not found");
      return;
    }
  Ptr <ns3::Ipv4> ipv4 = fromNode->GetObject <ns3::Ipv4> ();
  if (!ipv4)
    {
      NS_LOG_WARN ("ipv4 object not found");
      return;
    }
  Ptr <Ipv4RoutingProtocol> rp = ipv4->GetRoutingProtocol ();
  if (!rp)
    {
      NS_LOG_WARN ("Routing protocol object not found");
      return;
    }
  Ptr<Packet> pkt = Create<Packet> ();
  Ipv4Header header;
  header.SetDestination (Ipv4Address (to.c_str ()));
  Socket::SocketErrno sockerr;
  Ptr <Ipv4Route> rt = rp->RouteOutput (pkt, header, 0, sockerr);
  if (!rt)
    {
      return;
    }
  NS_LOG_DEBUG ("Node: " << fromNode->GetId () << " G:" << rt->GetGateway ());
  std::ostringstream oss;
  oss << rt->GetGateway ();
  if (oss.str () == "0.0.0.0" && (sockerr != Socket::ERROR_NOROUTETOHOST))
    {
      NS_LOG_INFO ("Null gw");
      Ipv4RoutePathElement elem = { fromNode->GetId (), "C" };
      rpElements.push_back (elem);
      if ( m_ipv4ToNodeIdMap.find (to) != m_ipv4ToNodeIdMap.end ())
        {
          Ipv4RoutePathElement elem2 = { m_ipv4ToNodeIdMap[to], "L" };
          rpElements.push_back (elem2);
        }
      return;
    }
  NS_LOG_INFO ("Node:" << fromNode->GetId () << "-->" << rt->GetGateway ()); 
  Ipv4RoutePathElement elem = { fromNode->GetId (), oss.str () };
  rpElements.push_back (elem);
  RecursiveIpv4RoutePathSearch (oss.str (), to, rpElements);

}

void AnimationInterface::TrackIpv4RoutePaths ()
{
  if (m_ipv4RouteTrackElements.empty ())
    {
      return;
    }
  for (std::vector <Ipv4RouteTrackElement>::const_iterator i = m_ipv4RouteTrackElements.begin ();
       i != m_ipv4RouteTrackElements.end ();
       ++i)
    {
      Ipv4RouteTrackElement trackElement = *i;
      Ptr <Node> fromNode = NodeList::GetNode (trackElement.fromNodeId);
      if (!fromNode)
        {
          NS_FATAL_ERROR ("Node: " << trackElement.fromNodeId << " Not found");
          continue;
        }
      Ptr <ns3::Ipv4> ipv4 = fromNode->GetObject <ns3::Ipv4> ();
      if (!ipv4)
        {
          NS_LOG_WARN ("ipv4 object not found");
          continue;
        }
      Ptr <Ipv4RoutingProtocol> rp = ipv4->GetRoutingProtocol ();
      if (!rp)
        {
          NS_LOG_WARN ("Routing protocol object not found");
          continue;
        }
      NS_LOG_INFO ("Begin Track Route for: " << trackElement.destination.c_str () << " From:" << trackElement.fromNodeId);
      Ptr<Packet> pkt = Create<Packet> ();
      Ipv4Header header;
      header.SetDestination (Ipv4Address (trackElement.destination.c_str ()));
      Socket::SocketErrno sockerr;
      Ptr <Ipv4Route> rt = rp->RouteOutput (pkt, header, 0, sockerr);
      Ipv4RoutePathElements rpElements;
      if (!rt)
        {
          NS_LOG_INFO ("No route to :" << trackElement.destination.c_str ());
          Ipv4RoutePathElement elem = { trackElement.fromNodeId, "-1" };
          rpElements.push_back (elem);
          WriteRoutePath (trackElement.fromNodeId, trackElement.destination, rpElements);
          continue;
        }
      std::ostringstream oss;
      oss << rt->GetGateway ();
      NS_LOG_INFO ("Node:" << trackElement.fromNodeId << "-->" << rt->GetGateway ()); 
      if (rt->GetGateway () == "0.0.0.0")
        {
          Ipv4RoutePathElement elem = { trackElement.fromNodeId, "C" };
          rpElements.push_back (elem);
          if ( m_ipv4ToNodeIdMap.find (trackElement.destination) != m_ipv4ToNodeIdMap.end ())
            {
              Ipv4RoutePathElement elem2 = { m_ipv4ToNodeIdMap[trackElement.destination], "L" };
              rpElements.push_back (elem2);
            }
        }
      else if (rt->GetGateway () == "127.0.0.1")
        {
          Ipv4RoutePathElement elem = { trackElement.fromNodeId, "-1" };
          rpElements.push_back (elem);
        }
      else
        {
          Ipv4RoutePathElement elem = { trackElement.fromNodeId, oss.str () };
          rpElements.push_back (elem);
        }
      RecursiveIpv4RoutePathSearch (oss.str (), trackElement.destination, rpElements);
      WriteRoutePath (trackElement.fromNodeId, trackElement.destination, rpElements);
    }

}

void AnimationInterface::TrackIpv4Route ()
{
  if (Simulator::Now () > m_routingStopTime)
  {
    NS_LOG_INFO ("TrackIpv4Route completed");
    return;
  }
  if (m_routingNc.GetN ())
    {
      for (NodeContainer::Iterator i = m_routingNc.Begin (); i != m_routingNc.End (); ++i)
        {
          Ptr <Node> n = *i;
          WriteN (GetXMLOpenClose_routing (n->GetId (), GetIpv4RoutingTable (n)), m_routingF);
        }
    }
  else
    {
      for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
        {
          Ptr <Node> n = *i;
          WriteN (GetXMLOpenClose_routing (n->GetId (), GetIpv4RoutingTable (n)), m_routingF);
        }
    }
  TrackIpv4RoutePaths ();
  Simulator::Schedule (m_routingPollInterval, &AnimationInterface::TrackIpv4Route, this);
}

void AnimationInterface::SetXMLOutput ()
{
  NS_LOG_INFO ("XML output set");
  m_xml = true;
}


void AnimationInterface::StartNewTraceFile ()
{
  static int i = 0;
  std::ostringstream oss;
  oss << i;
  ++m_currentPktCount;
  if (m_currentPktCount <= m_maxPktsPerFile)
    {
      return;
    }
  StopAnimation (true);
  m_outputFileName = m_originalFileName + "-" + oss.str ();
  StartAnimation (true);
  ++i;

}

std::string AnimationInterface::GetNetAnimVersion ()
{
  return "netanim-3.103";
}

void AnimationInterface::SetStartTime (Time t)
{
  m_startTime = t;
}

void AnimationInterface::SetStopTime (Time t)
{
  m_stopTime = t;
}

bool AnimationInterface::SetOutputFile (const std::string& fn)
{
  if (m_outputFileSet)
    {
      return true;
    }
  NS_LOG_INFO ("Creating new trace file:" << fn.c_str ());
  m_f = std::fopen (fn.c_str (), "w");
  if (!m_f)
    {
      NS_FATAL_ERROR ("Unable to open Animation output file");
      return false; // Can't open
    }
  m_outputFileName = fn;
  m_outputFileSet = true;
  return true;
}

bool AnimationInterface::SetRoutingOutputFile (const std::string& fn)
{
  if (m_routingF)
    {
      NS_FATAL_ERROR ("SetRoutingOutputFile already used once");
      return false;
    }
  m_routingF = std::fopen (fn.c_str (), "w");
  if (!m_routingF)
    {
      NS_FATAL_ERROR ("Unable to open Animation Routing output file");
      return false;
    }
  return true;
}

void AnimationInterface::EnablePacketMetadata (bool enable)
{
   m_enablePacketMetadata = enable;
   if (enable)
     Packet::EnablePrinting ();
}

bool AnimationInterface::IsInitialized ()
{
  return initialized;
}

bool AnimationInterface::IsStarted ()
{
  return m_started;

}

void AnimationInterface::SetAnimWriteCallback (AnimWriteCallback cb)
{
  m_writeCallback = cb;
}

void AnimationInterface::ResetAnimWriteCallback ()
{
  m_writeCallback = 0;
}

bool AnimationInterface::IsInTimeWindow ()
{
  if ((Simulator::Now () >= m_startTime) && 
      (Simulator::Now () <= m_stopTime))
    return true;
  else
    return false;
}

bool AnimationInterface::UanPacketIsPending (uint64_t AnimUid)
{
  return (m_pendingUanPackets.find (AnimUid) != m_pendingUanPackets.end ());
}

bool AnimationInterface::WifiPacketIsPending (uint64_t AnimUid)
{
  return (m_pendingWifiPackets.find (AnimUid) != m_pendingWifiPackets.end ());
}

bool AnimationInterface::CsmaPacketIsPending (uint64_t AnimUid)
{
  return (m_pendingCsmaPackets.find (AnimUid) != m_pendingCsmaPackets.end ());
}


bool AnimationInterface::WimaxPacketIsPending (uint64_t AnimUid)
{
  return (m_pendingWimaxPackets.find (AnimUid) != m_pendingWimaxPackets.end ());
}

bool AnimationInterface::LtePacketIsPending (uint64_t AnimUid)
{
  return (m_pendingLtePackets.find (AnimUid) != m_pendingLtePackets.end ());
}

void AnimationInterface::SetMobilityPollInterval (Time t)
{
  m_mobilityPollInterval = t;
}

void AnimationInterface::SetRandomPosition (bool setRandPos)
{
  m_randomPosition = setRandPos;
}

Vector AnimationInterface::UpdatePosition (Ptr <Node> n)
{
  Ptr<MobilityModel> loc = n->GetObject<MobilityModel> ();
  if (loc)
    {
      m_nodeLocation[n->GetId ()] = loc->GetPosition ();
    }
  else
   {
     NS_LOG_UNCOND ( "AnimationInterface WARNING:Node:" << n->GetId () << " Does not have a mobility model. Use SetConstantPosition if it is stationary");
     Vector deterministicVector (100,100,0);
     Vector randomVector (m_uniformRandomVariable->GetValue (0, m_topoMaxX - m_topoMinX), m_uniformRandomVariable->GetValue (0, m_topoMaxY - m_topoMinY), 0);
     if (m_randomPosition)
       {
         m_nodeLocation[n->GetId ()] = randomVector;
       }
     else
       {
         m_nodeLocation[n->GetId ()] = deterministicVector;
       }
   }
  return m_nodeLocation[n->GetId ()];
}

Vector AnimationInterface::UpdatePosition (Ptr <Node> n, Vector v)
{
  m_nodeLocation[n->GetId ()] = v;
  return v;
}

Vector AnimationInterface::GetPosition (Ptr <Node> n)
{
  #ifdef NS_LOG
  if (m_nodeLocation.find (n->GetId()) == m_nodeLocation.end ())
    {
      NS_FATAL_ERROR ("Node:" <<n->GetId() << " not found in Location table");
    }  
  #endif
  return m_nodeLocation[n->GetId ()];
}

void AnimationInterface::PurgePendingWifi ()
{
  if (m_pendingWifiPackets.empty ())
    return;
  std::vector <uint64_t> purgeList;
  for (std::map<uint64_t, AnimPacketInfo>::iterator i = m_pendingWifiPackets.begin ();
       i != m_pendingWifiPackets.end ();
       ++i)
    {
     
      AnimPacketInfo pktInfo = i->second; 
      double delta = (Simulator::Now ().GetSeconds () - pktInfo.m_fbTx);
      if (delta > PURGE_INTERVAL)
        {
          purgeList.push_back (i->first);
        }
    }

  for (std::vector <uint64_t>::iterator i = purgeList.begin ();
       i != purgeList.end ();
       ++i)
    {
      m_pendingWifiPackets.erase (*i);
    }

}

void AnimationInterface::PurgePendingWimax ()
{
  if (m_pendingWimaxPackets.empty ())
    return;
  std::vector <uint64_t> purgeList;
  for (std::map<uint64_t, AnimPacketInfo>::iterator i = m_pendingWimaxPackets.begin ();
       i != m_pendingWimaxPackets.end ();
       ++i)
    {

      AnimPacketInfo pktInfo = i->second;
      double delta = (Simulator::Now ().GetSeconds () - pktInfo.m_fbTx);
      if (delta > PURGE_INTERVAL)
        {
          purgeList.push_back (i->first);
        }
    }

  for (std::vector <uint64_t>::iterator i = purgeList.begin ();
       i != purgeList.end ();
       ++i)
    {
      m_pendingWimaxPackets.erase (*i);
    }

}


void AnimationInterface::PurgePendingLte ()
{
  if (m_pendingLtePackets.empty ())
    return;
  std::vector <uint64_t> purgeList;
  for (std::map<uint64_t, AnimPacketInfo>::iterator i = m_pendingLtePackets.begin ();
       i != m_pendingLtePackets.end ();
       ++i)
    {

      AnimPacketInfo pktInfo = i->second;
      double delta = (Simulator::Now ().GetSeconds () - pktInfo.m_fbTx);
      if (delta > PURGE_INTERVAL)
        {
          purgeList.push_back (i->first);
        }
    }

  for (std::vector <uint64_t>::iterator i = purgeList.begin ();
       i != purgeList.end ();
       ++i)
    {
      m_pendingLtePackets.erase (*i);
    }
}

void AnimationInterface::PurgePendingCsma ()
{
  if (m_pendingCsmaPackets.empty ())
    return;
  std::vector <uint64_t> purgeList;
  for (std::map<uint64_t, AnimPacketInfo>::iterator i = m_pendingCsmaPackets.begin ();
       i != m_pendingCsmaPackets.end ();
       ++i)
    {
    
      AnimPacketInfo pktInfo = i->second;
      double delta = (Simulator::Now ().GetSeconds () - pktInfo.m_fbTx);
      if (delta > PURGE_INTERVAL)
        { 
          purgeList.push_back (i->first);
        }
    }

  for (std::vector <uint64_t>::iterator i = purgeList.begin ();
       i != purgeList.end ();
       ++i)
    {
      m_pendingCsmaPackets.erase (*i);
    }

}

std::string AnimationInterface::GetMacAddress (Ptr <NetDevice> nd)
{
  Address nodeAddr = nd->GetAddress();
  std::ostringstream oss;
  oss << nodeAddr;
  return oss.str ().substr (6); // Skip the first 6 chars to get the Mac
}

std::string AnimationInterface::GetIpv4Address (Ptr <NetDevice> nd)
{
  Ptr<Ipv4> ipv4 = NodeList::GetNode (nd->GetNode ()->GetId ())->GetObject <Ipv4> ();
  if (!ipv4)
    {
      NS_LOG_WARN ("Node: " << nd->GetNode ()->GetId () << " No ipv4 object found");
      return "0.0.0.0";
    }
  int32_t ifIndex = ipv4->GetInterfaceForDevice (nd);
  if (ifIndex == -1)
    {
      NS_LOG_WARN ("Node :" << nd->GetNode ()->GetId () << " Could not find index of NetDevice");
      return "0.0.0.0";
    }
  Ipv4InterfaceAddress addr = ipv4->GetAddress (ifIndex, 0);
  std::ostringstream oss;
  oss << addr.GetLocal ();
  return oss.str ();
}

void AnimationInterface::StartAnimation (bool restart)
{
  m_currentPktCount = 0;
  m_started = true;
  SetOutputFile (m_outputFileName);

  // Find the min/max x/y for the xml topology element
  m_topoMinX = -2;
  m_topoMinY = -2;
  m_topoMaxX = 2;
  m_topoMaxY = 2;
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      NS_LOG_INFO ("Update Position for Node: " << n->GetId ());
      Vector v = UpdatePosition (n); 
      m_topoMinX = std::min (m_topoMinX, v.x);
      m_topoMinY = std::min (m_topoMinY, v.y);
      m_topoMaxX = std::max (m_topoMaxX, v.x);
      m_topoMaxY = std::max (m_topoMaxY, v.y);
      struct Rgb rgb = {255, 0, 0};
      if (nodeColors.find (n->GetId ()) == nodeColors.end ())
      {
        nodeColors[n->GetId ()] = rgb;
      }
    }

  
  std::ostringstream oss;
  oss << GetXMLOpen_anim (0);
  oss << GetPreamble ();
  oss << GetXMLOpen_topology (m_topoMinX, m_topoMinY, m_topoMaxX, m_topoMaxY);
  WriteN (oss.str (), m_f);
  NS_LOG_INFO ("Setting topology for "<<NodeList::GetNNodes ()<<" Nodes");
  // Dump the topology
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      std::ostringstream oss;
      Vector v = GetPosition (n);
      struct Rgb rgb = nodeColors[n->GetId ()];
      oss << GetXMLOpenClose_node (0, n->GetId (), v.x, v.y, rgb);
      WriteN (oss.str (), m_f);
    }
  NS_LOG_INFO ("Setting p2p links");
  // Now dump the p2p links
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      uint32_t n1Id = n->GetId ();
      uint32_t nDev = n->GetNDevices ();  // Number of devices
      for (uint32_t i = 0; i < nDev; ++i)
        {
          Ptr<NetDevice> dev = n->GetDevice (i);
 	  NS_ASSERT (dev);
          Ptr<Channel>   ch = dev->GetChannel ();
          if (!ch) 
            {
	      NS_LOG_DEBUG ("No channel can't be a p2p device");
              // Try to see if it is an LTE NetDevice, which does not return a channel
              if ((dev->GetInstanceTypeId ().GetName () == "ns3::LteUeNetDevice") || 
                  (dev->GetInstanceTypeId ().GetName () == "ns3::LteEnbNetDevice")||
                  (dev->GetInstanceTypeId ().GetName () == "ns3::VirtualNetDevice"))
                {
                  WriteNonP2pLinkProperties (n->GetId (), GetIpv4Address (dev) + "~" + GetMacAddress (dev), dev->GetInstanceTypeId ().GetName ());
                  AddToIpv4AddressNodeIdTable (GetIpv4Address (dev), n->GetId ());
                }
              continue;
            }
          std::string channelType = ch->GetInstanceTypeId ().GetName ();
          NS_LOG_DEBUG ("Got ChannelType" << channelType);
          if (channelType == std::string ("ns3::PointToPointChannel"))
            { // Since these are duplex links, we only need to dump
              // if srcid < dstid
              uint32_t nChDev = ch->GetNDevices ();
              for (uint32_t j = 0; j < nChDev; ++j)
                {
                  Ptr<NetDevice> chDev = ch->GetDevice (j);
                  uint32_t n2Id = chDev->GetNode ()->GetId ();
                  if (n1Id < n2Id)
                    { 
                      // ouptut the p2p link
                      NS_LOG_INFO ("Link:" << GetIpv4Address (dev) << ":" << GetMacAddress (dev) << "----" << GetIpv4Address (chDev) << ":" << GetMacAddress (chDev) );
                      SetLinkDescription (n1Id, n2Id, "", GetIpv4Address (dev) + "~" + GetMacAddress (dev), GetIpv4Address (chDev) + "~" + GetMacAddress (chDev));
                      AddToIpv4AddressNodeIdTable (GetIpv4Address (dev), n1Id);
                      AddToIpv4AddressNodeIdTable (GetIpv4Address (chDev), n2Id);
                      std::ostringstream oss;
                      if (m_xml)
                        {
                          oss << GetXMLOpenClose_link (0, n1Id, 0, n2Id);
                        }
                      else
                        {
                          oss << "0.0 L "  << n1Id << " " << n2Id << std::endl;
                        }
                      WriteN (oss.str (), m_f);
                    }
                }
            }
          else
            {
              NS_LOG_INFO ("Link:" << GetIpv4Address (dev) << " Channel Type:" << channelType << " Mac: " << GetMacAddress (dev));
              WriteNonP2pLinkProperties (n->GetId (), GetIpv4Address (dev) + "~" + GetMacAddress (dev), channelType); 
              AddToIpv4AddressNodeIdTable (GetIpv4Address (dev), n->GetId ());
            }
        }
    }
  linkProperties.clear ();
  if (m_xml && !restart)
    {
      WriteN (GetXMLClose ("topology"), m_f);
      Simulator::Schedule (m_mobilityPollInterval, &AnimationInterface::MobilityAutoCheck, this);
    }
  if (!restart)
    ConnectCallbacks ();
}

void AnimationInterface::AddToIpv4AddressNodeIdTable (std::string ipv4Address, uint32_t nodeId)
{
  m_ipv4ToNodeIdMap[ipv4Address] = nodeId;
}

AnimationInterface & AnimationInterface::AddSourceDestination (uint32_t fromNodeId, std::string ipv4Address)
{
  Ipv4RouteTrackElement element = { ipv4Address, fromNodeId };
  m_ipv4RouteTrackElements.push_back (element);
  return *this;
}


void AnimationInterface::ConnectLteEnb (Ptr <Node> n, Ptr <LteEnbNetDevice> nd, uint32_t devIndex)
{

  Ptr<LteEnbPhy> lteEnbPhy = nd->GetPhy ();
  Ptr<LteSpectrumPhy> dlPhy = lteEnbPhy->GetDownlinkSpectrumPhy ();
  Ptr<LteSpectrumPhy> ulPhy = lteEnbPhy->GetUplinkSpectrumPhy ();
  std::ostringstream oss;
  //NodeList/*/DeviceList/*/
  oss << "NodeList/" << n->GetId () << "/DeviceList/" << devIndex << "/";
  if (dlPhy)
    {
      dlPhy->TraceConnect ("TxStart",oss.str (), MakeCallback (&AnimationInterface::LteSpectrumPhyTxStart, this));
      dlPhy->TraceConnect ("RxStart",oss.str (), MakeCallback (&AnimationInterface::LteSpectrumPhyRxStart, this));
    }
  if (ulPhy)
    {
      ulPhy->TraceConnect ("TxStart",oss.str (), MakeCallback (&AnimationInterface::LteSpectrumPhyTxStart, this));
      ulPhy->TraceConnect ("RxStart",oss.str (), MakeCallback (&AnimationInterface::LteSpectrumPhyRxStart, this));
    }
}



void AnimationInterface::ConnectLteUe (Ptr <Node> n, Ptr <LteUeNetDevice> nd, uint32_t devIndex)
{

  Ptr<LteUePhy> lteUePhy = nd->GetPhy ();
  Ptr<LteSpectrumPhy> dlPhy = lteUePhy->GetDownlinkSpectrumPhy ();
  Ptr<LteSpectrumPhy> ulPhy = lteUePhy->GetUplinkSpectrumPhy ();
  std::ostringstream oss;
  //NodeList/*/DeviceList/*/
  oss << "NodeList/" << n->GetId () << "/DeviceList/" << devIndex << "/";
  if (dlPhy)
    {
      dlPhy->TraceConnect ("TxStart",oss.str (), MakeCallback (&AnimationInterface::LteSpectrumPhyTxStart, this));
      dlPhy->TraceConnect ("RxStart",oss.str (), MakeCallback (&AnimationInterface::LteSpectrumPhyRxStart, this));
    }
  if (ulPhy)
    {
       ulPhy->TraceConnect ("TxStart",oss.str (), MakeCallback (&AnimationInterface::LteSpectrumPhyTxStart, this));
       ulPhy->TraceConnect ("RxStart",oss.str (), MakeCallback (&AnimationInterface::LteSpectrumPhyRxStart, this));
    }
}

void AnimationInterface::ConnectLte ()
{

  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      NS_ASSERT (n);
      uint32_t nDevices = n->GetNDevices ();
      for (uint32_t devIndex = 0; devIndex < nDevices; ++devIndex)
        {
          Ptr <NetDevice> nd = n->GetDevice(devIndex);
          if (!nd)
            continue;
          Ptr<LteUeNetDevice> lteUeNetDevice = DynamicCast<LteUeNetDevice> (nd);
          if (lteUeNetDevice)
            {
              ConnectLteUe (n, lteUeNetDevice, devIndex);
              continue;
            }
          Ptr<LteEnbNetDevice> lteEnbNetDevice = DynamicCast<LteEnbNetDevice> (nd);
          if (lteEnbNetDevice)
            ConnectLteEnb (n, lteEnbNetDevice, devIndex);
        }

    }
}

void AnimationInterface::ConnectCallbacks ()
{
  // Connect the callbacks
  Config::Connect ("/ChannelList/*/TxRxPointToPoint",
                   MakeCallback (&AnimationInterface::DevTxTrace, this));
  Config::Connect ("NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin",
                   MakeCallback (&AnimationInterface::WifiPhyTxBeginTrace, this));
  Config::Connect ("NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxBegin",
                   MakeCallback (&AnimationInterface::WifiPhyRxBeginTrace, this));
  Config::ConnectWithoutContext ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeCallback (&AnimationInterface::MobilityCourseChangeTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WimaxNetDevice/Tx",
                   MakeCallback (&AnimationInterface::WimaxTxTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WimaxNetDevice/Rx",
                   MakeCallback (&AnimationInterface::WimaxRxTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::LteNetDevice/Tx",
                   MakeCallback (&AnimationInterface::LteTxTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::LteNetDevice/Rx",
                   MakeCallback (&AnimationInterface::LteRxTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/PhyTxBegin",
                   MakeCallback (&AnimationInterface::CsmaPhyTxBeginTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/PhyTxEnd",
                   MakeCallback (&AnimationInterface::CsmaPhyTxEndTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/PhyRxEnd",
                   MakeCallback (&AnimationInterface::CsmaPhyRxEndTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/MacRx",
                   MakeCallback (&AnimationInterface::CsmaMacRxTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::UanNetDevice/Phy/PhyTxBegin",
                   MakeCallback (&AnimationInterface::UanPhyGenTxTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::UanNetDevice/Phy/PhyRxBegin",
                   MakeCallback (&AnimationInterface::UanPhyGenRxTrace, this));
  Config::Connect ("/NodeList/*/$ns3::BasicEnergySource/RemainingEnergy",
                   MakeCallback (&AnimationInterface::RemainingEnergyTrace, this));

  ConnectLte ();

}


void AnimationInterface::StopAnimation (bool onlyAnimation)
{
  m_started = false;
  NS_LOG_INFO ("Stopping Animation");
  ResetAnimWriteCallback ();
  if (m_f) 
    {
      if (m_xml)
        { // Terminate the anim element
          WriteN (GetXMLClose ("anim"), m_f);
        }
      std::fclose (m_f);
    }
    m_outputFileSet = false;
  if (onlyAnimation)
    {
      return;
    }
  if (m_routingF)
    {
      WriteN (GetXMLClose ("anim"), m_routingF);
      std::fclose (m_routingF);
    }
}

int AnimationInterface::WriteN (const std::string& st, FILE * f)
{
  if (m_writeCallback)
    {
      m_writeCallback (st.c_str ());
    }
  return WriteN (st.c_str (), st.length (), f);
}

std::vector <Ptr <Node> >  AnimationInterface::RecalcTopoBounds ()
{
  std::vector < Ptr <Node> > MovedNodes;
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      NS_ASSERT (n);
      Ptr <MobilityModel> mobility = n->GetObject <MobilityModel> ();
      Vector newLocation;
      if (!mobility)
        {
          newLocation = GetPosition (n);
        }
      else
        {
          newLocation = mobility->GetPosition ();
        }
      if (!NodeHasMoved (n, newLocation))
        {
          continue; //Location has not changed
        }
      else
        {
          UpdatePosition (n, newLocation);
          RecalcTopoBounds (newLocation);
          MovedNodes.push_back (n);
        }
    }
  return MovedNodes;
}

void AnimationInterface::RecalcTopoBounds (Vector v)
{
  m_topoMinX = std::min (m_topoMinX, v.x);
  m_topoMinY = std::min (m_topoMinY, v.y);
  m_topoMaxX = std::max (m_topoMaxX, v.x);
  m_topoMaxY = std::max (m_topoMaxY, v.y);
  
}

int AnimationInterface::WriteN (const char* data, uint32_t count, FILE * f)
{ 
  // Write count bytes to h from data
  uint32_t    nLeft   = count;
  const char* p       = data;
  uint32_t    written = 0;
  while (nLeft)
    {
      int n = std::fwrite (p, 1,  nLeft, f);
      if (n <= 0) 
        {
          return written;
        }
      written += n;
      nLeft -= n;
      p += n;
    }
  return written;
}

void AnimationInterface::WriteDummyPacket ()
{
  Time now = Simulator::Now ();
  std::ostringstream oss;
  double fbTx = now.GetSeconds ();
  double lbTx = now.GetSeconds ();
  double fbRx = now.GetSeconds ();
  double lbRx = now.GetSeconds ();
  oss << GetXMLOpenClose_p ("p", 0, fbTx, lbTx, 0, fbRx, lbRx, "", "DummyPktIgnoreThis");
  WriteN (oss.str (), m_f);


}

void AnimationInterface::WriteRoutePath (uint32_t nodeId, std::string destination, Ipv4RoutePathElements rpElements)
{
  NS_LOG_INFO ("Writing Route Path From :" << nodeId << " To: " << destination.c_str ());
  WriteN (GetXMLOpenClose_rp (nodeId, destination, rpElements), m_routingF);
  /*for (Ipv4RoutePathElements::const_iterator i = rpElements.begin ();
       i != rpElements.end ();
       ++i)
    {
      Ipv4RoutePathElement rpElement = *i;
      NS_LOG_INFO ("Node:" << rpElement.nodeId << "-->" << rpElement.nextHop.c_str ());
      WriteN (GetXMLOpenClose_rp (rpElement.node, GetIpv4RoutingTable (n)), m_routingF);

    }
  */
}

void AnimationInterface::WriteNonP2pLinkProperties (uint32_t id, std::string ipv4Address, std::string channelType)
{
  std::ostringstream oss;
  oss << GetXMLOpenClose_NonP2pLinkProperties (id, ipv4Address, channelType);
  WriteN (oss.str (), m_f);
}

void AnimationInterface::DevTxTrace (std::string context, Ptr<const Packet> p,
                                     Ptr<NetDevice> tx, Ptr<NetDevice> rx,
                                     Time txTime, Time rxTime)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  NS_ASSERT (tx);
  NS_ASSERT (rx);
  Time now = Simulator::Now ();
  std::ostringstream oss;
  double fbTx = now.GetSeconds ();
  double lbTx = (now + txTime).GetSeconds ();
  double fbRx = (now + rxTime - txTime).GetSeconds ();
  double lbRx = (now + rxTime).GetSeconds ();
  if (m_xml)
    {
      oss << GetXMLOpenClose_p ("p", tx->GetNode ()->GetId (), fbTx, lbTx, rx->GetNode ()->GetId (), 
                                fbRx, lbRx, m_enablePacketMetadata? GetPacketMetadata (p):"");
      StartNewTraceFile ();
      ++m_currentPktCount;
    }
  else
    {
      oss << std::setprecision (10);
      oss << now.GetSeconds () << " P "
          << tx->GetNode ()->GetId () << " "
          << rx->GetNode ()->GetId () << " "
          << (now + txTime).GetSeconds () << " " // last bit tx time
          << (now + rxTime - txTime).GetSeconds () << " " // first bit rx time
          << (now + rxTime).GetSeconds () << std::endl;         // last bit rx time
    }
  WriteN (oss.str (), m_f);
}


Ptr <Node>
AnimationInterface::GetNodeFromContext (const std::string& context) const
{
  // Use "NodeList/*/ as reference
  // where element [1] is the Node Id

  std::vector <std::string> elements = GetElementsFromContext (context);
  Ptr <Node> n = NodeList::GetNode (atoi (elements.at(1).c_str ()));
  NS_ASSERT (n);

  return n;
}

Ptr <NetDevice>
AnimationInterface::GetNetDeviceFromContext (std::string context)
{
  // Use "NodeList/*/DeviceList/*/ as reference
  // where element [1] is the Node Id
  // element [2] is the NetDevice Id

  std::vector <std::string> elements = GetElementsFromContext (context);
  Ptr <Node> n = GetNodeFromContext(context);

  return n->GetDevice (atoi (elements.at(3).c_str ()));
}

void AnimationInterface::AddPendingUanPacket (uint64_t AnimUid, AnimPacketInfo &pktinfo)
{
  m_pendingUanPackets[AnimUid] = pktinfo;
}

                                  
void AnimationInterface::AddPendingWifiPacket (uint64_t AnimUid, AnimPacketInfo &pktinfo)
{
  m_pendingWifiPackets[AnimUid] = pktinfo;
}

void AnimationInterface::AddPendingWimaxPacket (uint64_t AnimUid, AnimPacketInfo &pktinfo)
{
  NS_ASSERT (pktinfo.m_txnd);
  m_pendingWimaxPackets[AnimUid] = pktinfo;
}

void AnimationInterface::AddPendingLtePacket (uint64_t AnimUid, AnimPacketInfo &pktinfo)
{
  NS_ASSERT (pktinfo.m_txnd);
  m_pendingLtePackets[AnimUid] = pktinfo;
}

void AnimationInterface::AddPendingCsmaPacket (uint64_t AnimUid, AnimPacketInfo &pktinfo)
{
  NS_ASSERT (pktinfo.m_txnd);
  m_pendingCsmaPackets[AnimUid] = pktinfo;
}

uint64_t AnimationInterface::GetAnimUidFromPacket (Ptr <const Packet> p)
{
  AnimByteTag tag;
  TypeId tid = tag.GetInstanceTypeId ();
  ByteTagIterator i = p->GetByteTagIterator ();
  bool found = false;
  while (i.HasNext ())
    {
      ByteTagIterator::Item item = i.Next ();
      if (tid == item.GetTypeId ())
        {
          item.GetTag (tag);
          found = true;
        }
    }
  if (found)
    {
      return tag.Get ();
    }
  else
    {
      return 0;
    }
}

void AnimationInterface::UanPhyGenTxTrace (std::string context, Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  gAnimUid++;
  NS_LOG_INFO ("Uan TxBeginTrace for packet:" << gAnimUid);
  AnimByteTag tag;
  tag.Set (gAnimUid);
  p->AddByteTag (tag);
  AnimPacketInfo pktinfo (ndev, Simulator::Now (), Simulator::Now (), UpdatePosition (n));
  AddPendingUanPacket (gAnimUid, pktinfo);


}

void AnimationInterface::UanPhyGenRxTrace (std::string context, Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("UanPhyGenRxTrace for packet:" << AnimUid);
  if (!UanPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("UanPhyGenRxBeginTrace: unknown Uid");
      return;
    }
  m_pendingUanPackets[AnimUid].ProcessRxBegin (ndev, Simulator::Now ());
  m_pendingUanPackets[AnimUid].ProcessRxEnd (ndev, Simulator::Now (), UpdatePosition (n));
  OutputWirelessPacket (p, m_pendingUanPackets[AnimUid], m_pendingUanPackets[AnimUid].GetRxInfo (ndev));

}

void AnimationInterface::RemainingEnergyTrace (std::string context, double previousEnergy, double currentEnergy)
{
  if (!m_started || !IsInTimeWindow ())
    return;

  const Ptr <const Node> node = GetNodeFromContext(context);
  const uint32_t nodeId = node->GetId();

  NS_LOG_INFO ("Remaining energy on one of sources on node " << nodeId << ": " << currentEnergy);

  const Ptr<EnergySource> energySource = node->GetObject<EnergySource> ();

  NS_ASSERT(energySource);
  // Don't call GetEnergyFraction() because of recursion
  const double energyFraction = currentEnergy / energySource->GetInitialEnergy();

  NS_LOG_INFO ("Total energy fraction on node " << nodeId << ": " << energyFraction);

  m_nodeEnergyFraction[nodeId] = energyFraction;
  WriteNodeUpdate(nodeId);
}

void AnimationInterface::WifiPhyTxBeginTrace (std::string context,
                                          Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context); 
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  // Add a new pending wireless
  gAnimUid++;
  NS_LOG_INFO ("Wifi TxBeginTrace for packet:" << gAnimUid);
  AnimByteTag tag;
  tag.Set (gAnimUid);
  p->AddByteTag (tag);
  AnimPacketInfo pktinfo (ndev, Simulator::Now (), Simulator::Now (), UpdatePosition (n));
  AddPendingWifiPacket (gAnimUid, pktinfo);
  Ptr<WifiNetDevice> netDevice = DynamicCast<WifiNetDevice> (ndev);
  Mac48Address nodeAddr = netDevice->GetMac()->GetAddress();
  std::ostringstream oss; 
  oss << nodeAddr;
  m_macToNodeIdMap[oss.str ()] = n->GetId ();
  NS_LOG_INFO ("Added Mac" << oss.str () << " node:" <<m_macToNodeIdMap[oss.str ()]);
}

void AnimationInterface::WifiPhyTxEndTrace (std::string context,
                                            Ptr<const Packet> p)
{
}

void AnimationInterface::WifiPhyTxDropTrace (std::string context,
                                             Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  // Erase pending wifi
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("TxDropTrace for packet:" << AnimUid);
  NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
  m_pendingWifiPackets.erase (m_pendingWifiPackets.find (AnimUid));
}


void AnimationInterface::WifiPhyRxBeginTrace (std::string context,
                                              Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("Wifi RxBeginTrace for packet:" << AnimUid);
  if (!WifiPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("WifiPhyRxBeginTrace: unknown Uid");
      std::ostringstream oss;
      WifiMacHeader hdr;
      if(!p->PeekHeader (hdr))
      { 
        NS_LOG_WARN ("WifiMacHeader not present");
        return;
      }
      oss << hdr.GetAddr2 ();
      if (m_macToNodeIdMap.find (oss.str ()) == m_macToNodeIdMap.end ()) 
      {
        //NS_LOG_UNCOND (oss.str ());
        return;
      }
      Ptr <Node> txNode = NodeList::GetNode (m_macToNodeIdMap[oss.str ()]);
      AnimPacketInfo pktinfo (0, Simulator::Now (), Simulator::Now (), UpdatePosition (txNode), m_macToNodeIdMap[oss.str ()]);
      AddPendingWifiPacket (AnimUid, pktinfo);
      NS_LOG_WARN ("WifiPhyRxBegin: unknown Uid, but we are adding a wifi packet");
    }
  /// \todo NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
  m_pendingWifiPackets[AnimUid].ProcessRxBegin (ndev, Simulator::Now ());
  m_pendingWifiPackets[AnimUid].ProcessRxEnd (ndev, Simulator::Now (), UpdatePosition (n));
  OutputWirelessPacket (p, m_pendingWifiPackets[AnimUid], m_pendingWifiPackets[AnimUid].GetRxInfo (ndev));
}


void AnimationInterface::WifiPhyRxEndTrace (std::string context,
                                            Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  if (!WifiPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("WifiPhyRxEndTrace: unknown Uid");
      AnimPacketInfo pktinfo (ndev, Simulator::Now (), Simulator::Now (), UpdatePosition (n));
      AddPendingWifiPacket (AnimUid, pktinfo);
    }
  /// \todo NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = m_pendingWifiPackets[AnimUid];
  pktInfo.ProcessRxEnd (ndev, Simulator::Now (), UpdatePosition (n));
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  if (pktrxInfo.IsPhyRxComplete ())
    {
      NS_LOG_INFO ("MacRxTrace for packet:" << AnimUid << " complete");
      OutputWirelessPacket (p, pktInfo, pktrxInfo);
    }
}

void AnimationInterface::WifiMacRxTrace (std::string context,
                                         Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  if (!WifiPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("WifiMacRxTrace: unknown Uid");
      return;
    }
  /// \todo NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = m_pendingWifiPackets[AnimUid];
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  if (pktrxInfo.IsPhyRxComplete ())
    {
      NS_LOG_INFO ("MacRxTrace for packet:" << AnimUid << " complete");
      OutputWirelessPacket (p, pktInfo, pktrxInfo);
    }

}
void AnimationInterface::WifiPhyRxDropTrace (std::string context,
                                             Ptr<const Packet> p)
{
}

void AnimationInterface::WimaxTxTrace (std::string context, Ptr<const Packet> p, const Mac48Address & m)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  gAnimUid++;
  NS_LOG_INFO ("WimaxTxTrace for packet:" << gAnimUid);
  AnimPacketInfo pktinfo (ndev, Simulator::Now (), Simulator::Now () + Seconds (0.001), UpdatePosition (n));
  /// \todo 0.0001 is used until Wimax implements TxBegin and TxEnd traces
  AnimByteTag tag;
  tag.Set (gAnimUid);
  p->AddByteTag (tag);
  AddPendingWimaxPacket (gAnimUid, pktinfo);
}


void AnimationInterface::WimaxRxTrace (std::string context, Ptr<const Packet> p, const Mac48Address & m)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("WimaxRxTrace for packet:" << AnimUid);
  NS_ASSERT (WimaxPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = m_pendingWimaxPackets[AnimUid];
  pktInfo.ProcessRxBegin (ndev, Simulator::Now ());
  pktInfo.ProcessRxEnd (ndev, Simulator::Now () + Seconds (0.001), UpdatePosition (n));
  /// \todo 0.001 is used until Wimax implements RxBegin and RxEnd traces
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  OutputWirelessPacket (p, pktInfo, pktrxInfo);
}

void AnimationInterface::LteTxTrace (std::string context, Ptr<const Packet> p, const Mac48Address & m)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  gAnimUid++;
  NS_LOG_INFO ("LteTxTrace for packet:" << gAnimUid);
  AnimPacketInfo pktinfo (ndev, Simulator::Now (), Simulator::Now () + Seconds (0.001), UpdatePosition (n));
  /// \todo 0.0001 is used until Lte implements TxBegin and TxEnd traces
  AnimByteTag tag;
  tag.Set (gAnimUid);
  p->AddByteTag (tag);
  AddPendingLtePacket (gAnimUid, pktinfo);
}


void AnimationInterface::LteRxTrace (std::string context, Ptr<const Packet> p, const Mac48Address & m)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("LteRxTrace for packet:" << gAnimUid);
  if (!LtePacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("LteRxTrace: unknown Uid");
      return;
    }
  AnimPacketInfo& pktInfo = m_pendingLtePackets[AnimUid];
  pktInfo.ProcessRxBegin (ndev, Simulator::Now ());
  pktInfo.ProcessRxEnd (ndev, Simulator::Now () + Seconds (0.001), UpdatePosition (n));
  /// \todo 0.001 is used until Lte implements RxBegin and RxEnd traces
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  OutputWirelessPacket (p, pktInfo, pktrxInfo);
}

void AnimationInterface::LteSpectrumPhyTxStart (std::string context, Ptr<const PacketBurst> pb)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  if (!pb) 
    {
      NS_LOG_WARN ("pb == 0. Not yet supported");
      return;
    }
  context = "/" + context;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);

  std::list <Ptr <Packet> > pbList = pb->GetPackets ();
  for (std::list <Ptr <Packet> >::iterator i  = pbList.begin ();
       i != pbList.end ();
       ++i)
  {
    Ptr <Packet> p = *i;
    gAnimUid++;
    NS_LOG_INFO ("LteSpectrumPhyTxTrace for packet:" << gAnimUid);
    AnimPacketInfo pktinfo (ndev, Simulator::Now (), Simulator::Now () + Seconds (0.001), UpdatePosition (n));
    /// \todo 0.0001 is used until Lte implements TxBegin and TxEnd traces
    AnimByteTag tag;
    tag.Set (gAnimUid);
    p->AddByteTag (tag);
    AddPendingLtePacket (gAnimUid, pktinfo);
  }
}

void AnimationInterface::LteSpectrumPhyRxStart (std::string context, Ptr<const PacketBurst> pb)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  if (!pb) 
    {
      NS_LOG_WARN ("pb == 0. Not yet supported");
      return;
    }
  context = "/" + context;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);

  std::list <Ptr <Packet> > pbList = pb->GetPackets ();
  for (std::list <Ptr <Packet> >::iterator i  = pbList.begin ();
       i != pbList.end ();
       ++i)
  {
    Ptr <Packet> p = *i;
    uint64_t AnimUid = GetAnimUidFromPacket (p);
    NS_LOG_INFO ("LteSpectrumPhyRxTrace for packet:" << gAnimUid);
    if (!LtePacketIsPending (AnimUid))
      {
        NS_LOG_WARN ("LteSpectrumPhyRxTrace: unknown Uid");
        return;
      }
    AnimPacketInfo& pktInfo = m_pendingLtePackets[AnimUid];
    pktInfo.ProcessRxBegin (ndev, Simulator::Now ());
    pktInfo.ProcessRxEnd (ndev, Simulator::Now () + Seconds (0.001), UpdatePosition (n));
    /// \todo 0.001 is used until Lte implements RxBegin and RxEnd traces
    AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
    OutputWirelessPacket (p, pktInfo, pktrxInfo);
  }
}

void AnimationInterface::CsmaPhyTxBeginTrace (std::string context, Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  gAnimUid++;
  NS_LOG_INFO ("CsmaPhyTxBeginTrace for packet:" << gAnimUid);
  AnimByteTag tag;
  tag.Set (gAnimUid);
  p->AddByteTag (tag);
  AnimPacketInfo pktinfo (ndev, Simulator::Now (), Simulator::Now (), UpdatePosition (n));
  AddPendingCsmaPacket (gAnimUid, pktinfo);

}

void AnimationInterface::CsmaPhyTxEndTrace (std::string context, Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("CsmaPhyTxEndTrace for packet:" << AnimUid);
  if (!CsmaPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("CsmaPhyTxEndTrace: unknown Uid"); 
      AnimPacketInfo pktinfo (ndev, Simulator::Now (), Simulator::Now (), UpdatePosition (n));
      AddPendingCsmaPacket (AnimUid, pktinfo);
      NS_LOG_WARN ("Unknown Uid, but adding Csma Packet anyway");
    }
  /// \todo NS_ASSERT (CsmaPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = m_pendingCsmaPackets[AnimUid];
  pktInfo.m_lbTx = Simulator::Now ().GetSeconds ();
}

void AnimationInterface::CsmaPhyRxEndTrace (std::string context, Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  if (!CsmaPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("CsmaPhyRxEndTrace: unknown Uid"); 
      return;
    }
  /// \todo NS_ASSERT (CsmaPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = m_pendingCsmaPackets[AnimUid];
  m_pendingCsmaPackets[AnimUid].ProcessRxBegin (ndev, Simulator::Now ());
  pktInfo.ProcessRxEnd (ndev, Simulator::Now (), UpdatePosition (n));
  NS_LOG_INFO ("CsmaPhyRxEndTrace for packet:" << AnimUid);
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  if (pktrxInfo.IsPhyRxComplete ())
    {
      NS_LOG_INFO ("CsmaPhyRxEndTrace for packet:" << AnimUid << " complete");
      OutputCsmaPacket (p, pktInfo, pktrxInfo);
    }
}


void AnimationInterface::CsmaMacRxTrace (std::string context,
                                         Ptr<const Packet> p)
{
  if (!m_started || !IsInTimeWindow ())
    return;
  NS_LOG_FUNCTION (this);
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  if (!CsmaPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("CsmaMacRxTrace: unknown Uid"); 
      return;
    }
  /// \todo NS_ASSERT (CsmaPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = m_pendingCsmaPackets[AnimUid];
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  if (pktrxInfo.IsPhyRxComplete ())
    {
      NS_LOG_INFO ("MacRxTrace for packet:" << AnimUid << " complete");
      OutputCsmaPacket (p, pktInfo, pktrxInfo);
    }
}


void AnimationInterface::MobilityCourseChangeTrace (Ptr <const MobilityModel> mobility)

{
  if (!m_started || !IsInTimeWindow ())
    return;
  Ptr <Node> n = mobility->GetObject <Node> ();
  NS_ASSERT (n);
  Vector v ;
  if (!mobility)
    {
      v = GetPosition (n);
    } 
  else
    {
      v = mobility->GetPosition ();
    }
  UpdatePosition (n,v);
  RecalcTopoBounds (v);
  std::ostringstream oss; 
  oss << GetXMLOpen_topology (m_topoMinX, m_topoMinY, m_topoMaxX, m_topoMaxY);
  oss << GetXMLOpenClose_node (0, n->GetId (), v.x, v.y, nodeColors[n->GetId ()]);
  oss << GetXMLClose ("topology");
  WriteN (oss.str (), m_f);
  WriteDummyPacket ();
}

bool AnimationInterface::NodeHasMoved (Ptr <Node> n, Vector newLocation)
{
  Vector oldLocation = GetPosition (n);
  if ((ceil (oldLocation.x) == ceil (newLocation.x)) &&
    (ceil (oldLocation.y) == ceil (newLocation.y)))
    {
     
      return false;
    }
  else
    {
      return true;
    }
}

void AnimationInterface::MobilityAutoCheck ()
{
  if (!m_started || !IsInTimeWindow ())
    return;
  std::vector <Ptr <Node> > MovedNodes = RecalcTopoBounds ();
  std::ostringstream oss;
  oss << GetXMLOpen_topology (m_topoMinX, m_topoMinY, m_topoMaxX, m_topoMaxY);
  for (uint32_t i = 0; i < MovedNodes.size (); i++)
    {
      Ptr <Node> n = MovedNodes [i];
      NS_ASSERT (n);
      Vector v = GetPosition (n);
      oss << GetXMLOpenClose_node (0, n->GetId (), v.x, v.y);
    }
  oss << GetXMLClose ("topology");
  WriteN (oss.str (), m_f);
  WriteDummyPacket ();
  if (!Simulator::IsFinished ())
    {
      PurgePendingWifi ();
      PurgePendingWimax ();
      PurgePendingLte ();
      PurgePendingCsma ();
      Simulator::Schedule (m_mobilityPollInterval, &AnimationInterface::MobilityAutoCheck, this);
    }
}

std::string AnimationInterface::GetPacketMetadata (Ptr<const Packet> p)
{
  std::ostringstream oss;
  p->Print (oss);
  return oss.str ();
}

uint64_t AnimationInterface::GetTracePktCount ()
{
  return m_currentPktCount;
}

double AnimationInterface::GetNodeEnergyFraction (Ptr <const Node> node) const
{
  const EnergyFractionMap::const_iterator fractionIter = m_nodeEnergyFraction.find(node->GetId());

  NS_ASSERT (fractionIter != m_nodeEnergyFraction.end());
  return fractionIter->second;
}

int64_t
AnimationInterface::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_uniformRandomVariable->SetStream (stream);
  return 1;
}

// Helper to output a wireless packet.
// For now, only the XML interface is supported


std::string AnimationInterface::GetPreamble ()
{
  std::string s = 
    "<information><!-- \n\
    Description of attributes:\n\
    =========================\n\
    anim\n\
    * ver = Current version\n\
    topology\n\
    * minX = minimum X coordinate of the canvas\n\
    * minY = minimum Y coordinate of the canvas\n\
    * maxX = maximum X coordinate of the canvas\n\
    * maxY = maximum Y coordinate of the canvas\n\
    node\n\
    * id = Node Id\n\
    * locX = X coordinate\n\
    * locY = Y coordinate\n\
    * r = Red component\n\
    * g = Green component\n\
    * b = Blue component\n\
    link\n\
    * fromId = From Node Id\n\
    * toId   = To Node Id\n\
    * fd = From Node description (for IP Address)\n\
    * td = To Node description (for IP Address)\n\
    * ld = Link description (for Bandwidth, delay etc)\n\
    linkupdate\n\
    * t = Simulation time\n\
    * ld = Link description (for Bandwidth, delay etc)\n\
    nodeupdate\n\
    * t = Simulation time\n\
    * descr = Node description\n\
    * r = Red component\n\
    * g = Green component\n\
    * b = Blue component\n\
    * visible = Node visibility\n\
    * rc = Residual energy (between 0 and 1)\n\
    p\n\
    * fId = From Node Id\n\
    * fbTx = First bit transmit time\n\
    * lbTx = Last bit transmit time\n\
    * tId = To Node Id\n\
    * fbRx = First bit Rx Time\n\
    * lbRx = Last bit Rx\n\
    * meta-info = Packet meta data\n\
    wp\n\
    * fId = From Node Id\n\
    * fbTx = First bit transmit time\n\
    * lbTx = Last bit transmit time\n\
    * range = Reception range\n\
    * tId = To Node Id\n\
    * fbRx = First bit Rx time\n\
    * meta-info = Packet meta data\n\
    * lbRx = Last bit Rx time-->\n\
    </information>\n";
return s;
}

void AnimationInterface::OutputWirelessPacket (Ptr<const Packet> p, AnimPacketInfo &pktInfo, AnimRxInfo pktrxInfo)
{
  StartNewTraceFile ();
  NS_ASSERT (m_xml);
  std::ostringstream oss;
  uint32_t nodeId =  0;
  if (pktInfo.m_txnd)
    nodeId = pktInfo.m_txnd->GetNode ()->GetId ();
  else
    nodeId = pktInfo.m_txNodeId;

  double lbTx = pktInfo.firstlastbitDelta + pktInfo.m_fbTx;
  uint32_t rxId = pktrxInfo.m_rxnd->GetNode ()->GetId ();

  oss << GetXMLOpenClose_p ("wp", nodeId, pktInfo.m_fbTx, lbTx, rxId,
                            pktrxInfo.m_fbRx, pktrxInfo.m_lbRx, m_enablePacketMetadata? GetPacketMetadata (p):"");
  WriteN (oss.str (), m_f);
}

void AnimationInterface::OutputCsmaPacket (Ptr<const Packet> p, AnimPacketInfo &pktInfo, AnimRxInfo pktrxInfo)
{
  StartNewTraceFile ();
  NS_ASSERT (m_xml);
  std::ostringstream oss;
  NS_ASSERT (pktInfo.m_txnd);
  uint32_t nodeId = pktInfo.m_txnd->GetNode ()->GetId ();
  uint32_t rxId = pktrxInfo.m_rxnd->GetNode ()->GetId ();

  oss << GetXMLOpenClose_p ("p", nodeId, pktInfo.m_fbTx, pktInfo.m_lbTx, rxId,
                            pktrxInfo.m_fbRx, pktrxInfo.m_lbRx, m_enablePacketMetadata? GetPacketMetadata (p):"");
  WriteN (oss.str (), m_f);
}

void AnimationInterface::SetConstantPosition (Ptr <Node> n, double x, double y, double z)
{
  NS_ASSERT (n);
  Ptr<ConstantPositionMobilityModel> hubLoc =  n->GetObject<ConstantPositionMobilityModel> ();
  if (hubLoc == 0)
    {
      hubLoc = CreateObject<ConstantPositionMobilityModel> ();
      n->AggregateObject (hubLoc);
    }
  Vector hubVec (x, y, z);
  hubLoc->SetPosition (hubVec);
  NS_LOG_INFO ("Node:" << n->GetId () << " Position set to:(" << x << "," << y << "," << z << ")");

}

void AnimationInterface::SetBoundary (double minX, double minY, double maxX, double maxY)
{
  if (initialized)
    NS_FATAL_ERROR ("SetBoundary must be used prior to creating the AnimationInterface object");
  NS_ASSERT (minX < maxX);
  NS_ASSERT (minY < maxY);
  if (!userBoundary)
    {
      userBoundary = new Rectangle;
    }
  userBoundary->xMax = maxX;
  userBoundary->yMax = maxY;
  userBoundary->xMin = minX;
  userBoundary->yMin = minY;
}

void AnimationInterface::SetNodeColor (Ptr <Node> n, uint8_t r, uint8_t g, uint8_t b)
{
  if (initialized)
    NS_FATAL_ERROR ("SetNodeColor must be used prior to creating the AnimationInterface object");
  NS_ASSERT (n);
  NS_LOG_INFO ("Setting node color for Node Id:" << n->GetId ());
  struct Rgb rgb = {r, g, b};
  nodeColors[n->GetId ()] = rgb;
}

void AnimationInterface::ShowNode (uint32_t nodeId, bool show)
{
  NS_ASSERT (NodeList::GetNode (nodeId));
  NS_LOG_INFO ("Setting node visibility for Node Id:" << nodeId); 
  WriteNodeUpdate(nodeId);
}

void AnimationInterface::ShowNode (Ptr <Node> n, bool show)
{
  ShowNode (n, show);
}

void AnimationInterface::WriteNodeUpdate (uint32_t nodeId)
{
  std::ostringstream oss;
  oss << GetXMLOpenClose_nodeupdate (nodeId);
  WriteN (oss.str (), m_f);
}

void AnimationInterface::UpdateNodeColor (Ptr <Node> n, uint8_t r, uint8_t g, uint8_t b)
{
  UpdateNodeColor (n->GetId (), r, g, b);
}

void AnimationInterface::UpdateNodeColor (uint32_t nodeId, uint8_t r, uint8_t g, uint8_t b)
{
  NS_ASSERT (NodeList::GetNode (nodeId));
  NS_LOG_INFO ("Setting node color for Node Id:" << nodeId); 
  struct Rgb rgb = {r, g, b};
  nodeColors[nodeId] = rgb;
  WriteNodeUpdate(nodeId);
}



void AnimationInterface::SetNodeColor (NodeContainer nc, uint8_t r, uint8_t g, uint8_t b)
{
  for (uint32_t i = 0; i < nc.GetN (); ++i)
    {
      Ptr <Node> n = nc.Get (i);
      NS_ASSERT (n);
      SetNodeColor (n, r, g, b);
    }
}


void AnimationInterface::UpdateLinkDescription (uint32_t fromNode, uint32_t toNode,
                                                std::string linkDescription)
{
  std::ostringstream oss;
  oss << GetXMLOpenClose_linkupdate (fromNode, toNode, linkDescription);
  WriteN (oss.str (), m_f);
}

void AnimationInterface::UpdateLinkDescription (Ptr <Node> fromNode, Ptr <Node> toNode,
                                                std::string linkDescription)
{
  NS_ASSERT (fromNode);
  NS_ASSERT (toNode);
  std::ostringstream oss;
  oss << GetXMLOpenClose_linkupdate (fromNode->GetId (), toNode->GetId (), linkDescription);
  WriteN (oss.str (), m_f);
}

void AnimationInterface::SetLinkDescription (uint32_t fromNode, uint32_t toNode, 
                                             std::string linkDescription,
                                             std::string fromNodeDescription,
                                             std::string toNodeDescription)
{

  P2pLinkNodeIdPair p2pPair;
  p2pPair.fromNode = fromNode;
  p2pPair.toNode = toNode;
  LinkProperties lp = { fromNodeDescription, toNodeDescription, linkDescription };
  linkProperties[p2pPair] = lp;
  /* DEBUG */
  /*
  for (std::map <P2pLinkNodeIdPair, LinkProperties>::const_iterator i = linkProperties.begin ();
	i != linkProperties.end(); ++i)
   {
    P2pLinkNodeIdPair ppair = i->first;
    LinkProperties l = i->second;
    NS_LOG_UNCOND ("A:" << ppair.fromNode << " B:" << ppair.toNode << " ad:" << l.fromNodeDescription << " bd:" << l.toNodeDescription << " ld:" << l.linkDescription);
     
   }
   */
}

void AnimationInterface::SetLinkDescription (Ptr <Node> fromNode, Ptr <Node> toNode,
                                             std::string linkDescription,
                                             std::string fromNodeDescription,
                                             std::string toNodeDescription)
{
  NS_ASSERT (fromNode);
  NS_ASSERT (toNode);
  SetLinkDescription (fromNode->GetId (), toNode->GetId (), linkDescription, fromNodeDescription, toNodeDescription);
}


void AnimationInterface::SetNodeDescription (Ptr <Node> n, std::string descr) 
{
  if (initialized)
    NS_FATAL_ERROR ("SetNodeDescription must be used prior to creating the AnimationInterface object");
  NS_ASSERT (n);
  nodeDescriptions[n->GetId ()] = descr;
}

void AnimationInterface::UpdateNodeDescription (Ptr <Node> n, std::string descr)
{
  UpdateNodeDescription (n->GetId (), descr);
}

void AnimationInterface::UpdateNodeDescription (uint32_t nodeId, std::string descr)
{
  NS_ASSERT (NodeList::GetNode (nodeId));
  nodeDescriptions[nodeId] = descr;
  std::ostringstream oss;
  oss << GetXMLOpenClose_nodeupdate (nodeId);
  WriteN (oss.str (), m_f);
}



void AnimationInterface::SetNodeDescription (NodeContainer nc, std::string descr)
{
  if (initialized)
    NS_FATAL_ERROR ("SetNodeDescription must be used prior to creating the AnimationInterface object");
  for (uint32_t i = 0; i < nc.GetN (); ++i)
    {
      Ptr <Node> n = nc.Get (i);
      NS_ASSERT (n);
      nodeDescriptions[n->GetId ()] = descr;
    }
}


// XML Private Helpers

std::string AnimationInterface::GetXMLOpen_anim (uint32_t lp)
{
  std::ostringstream oss;
  oss <<"<anim ver=\"" << GetNetAnimVersion () << "\">\n";
  return oss.str ();
}
std::string AnimationInterface::GetXMLOpen_topology (double minX, double minY, double maxX, double maxY)
{
  if (userBoundary)
    {
      minX = userBoundary->xMin;
      minY = userBoundary->yMin;
      maxX = userBoundary->xMax;
      maxY = userBoundary->yMax;
    }
  std::ostringstream oss;
  oss << "<topology minX = \"" << minX << "\" minY = \"" << minY
      << "\" maxX = \"" << maxX << "\" maxY = \"" << maxY
      << "\">" << std::endl;
  return oss.str ();

}

std::string AnimationInterface::GetXMLOpenClose_nodeupdate (uint32_t id, bool visible)
{
  std::ostringstream oss;
  oss << "<nodeupdate id=\"" << id << "\"";
  oss << " t=\"" << Simulator::Now ().GetSeconds () << "\"";
  if (visible)
    oss << " visible=\"" << 1 << "\"";
  else
    oss << " visible=\"" << 0 << "\"";
  AppendXMLNodeDescription(oss, id);
  AppendXMLNodeColor(oss, nodeColors[id]);
  AppendXMLRemainingEnergy(oss, id);

  oss  <<"/>\n";

  return oss.str ();

}

std::string AnimationInterface::GetXMLOpenClose_node (uint32_t lp, uint32_t id, double locX, double locY)
{
  std::ostringstream oss;
  oss <<"<node id=\"" << id << "\""; 
  AppendXMLNodeDescription(oss, id);
  oss << " locX = \"" << locX << "\" " << "locY = \"" << locY << "\"";
  AppendXMLRemainingEnergy(oss, id);

  oss  <<"/>\n";

  return oss.str ();
}

std::string AnimationInterface::GetXMLOpenClose_node (uint32_t lp, uint32_t id, double locX, double locY, struct Rgb rgb)
{
  std::ostringstream oss;
  oss <<"<node id = \"" << id << "\"";
  AppendXMLNodeDescription(oss, id);
  oss << " locX=\"" << locX << "\" " << "locY=\"" << locY << "\"";
  AppendXMLNodeColor(oss, rgb);
  AppendXMLRemainingEnergy(oss, id);

  oss  <<"/>\n";

  return oss.str ();
}

std::string AnimationInterface::GetXMLOpenClose_linkupdate (uint32_t fromId, uint32_t toId, std::string linkDescription)
{
  std::ostringstream oss;
  oss << "<linkupdate t=\"" << Simulator::Now ().GetSeconds () << "\""
      << " fromId=\"" << fromId
      << "\" toId=\"" << toId
      << "\" ";

  oss << " ld=\"" << linkDescription << "\""
      << " />\n";
  return oss.str ();

}

std::string AnimationInterface::GetXMLOpenClose_link (uint32_t fromLp, uint32_t fromId, uint32_t toLp, uint32_t toId)
{
  std::ostringstream oss;
  oss << "<link fromId=\"" << fromId
      << "\" toId=\"" << toId
      << "\" ";

  LinkProperties lprop ;
  lprop.fromNodeDescription = "";
  lprop.toNodeDescription = "";
  lprop.linkDescription = "";

  P2pLinkNodeIdPair p1 = { fromId, toId };
  P2pLinkNodeIdPair p2 = { toId, fromId };
  if (linkProperties.find (p1) != linkProperties.end())
    {
      lprop = linkProperties[p1];
    }
  else if (linkProperties.find (p2) != linkProperties.end())
    {
      lprop = linkProperties[p2];
    }
   
  oss << " fd=\"" << lprop.fromNodeDescription << "\""
      << " td=\"" << lprop.toNodeDescription << "\""
      << " ld=\"" << lprop.linkDescription << "\""
      << " />\n";
  return oss.str ();
}


std::string AnimationInterface::GetXMLOpen_packet (uint32_t fromLp, uint32_t fromId, double fbTx, double lbTx, std::string auxInfo)
{
  std::ostringstream oss;
  oss << std::setprecision (10);
  oss << "<p fId=\"" << fromId
      << "\" fbTx=\"" << fbTx
      << "\" lbTx=\"" << lbTx
      << (auxInfo.empty()?"":"\" aux=\"") << auxInfo.c_str () << "\">";
  return oss.str ();
}

std::string AnimationInterface::GetXMLOpenClose_routing (uint32_t nodeId, std::string routingInfo)
{
  std::ostringstream oss;
  oss << "<" << "rt" << " t=\"" << Simulator::Now ().GetSeconds () << "\"" 
      << " id=\"" << nodeId << "\""
      << " info=\"" << routingInfo.c_str () << "\""
      << "/>" << std::endl;
  return oss.str ();
}

std::string AnimationInterface::GetXMLOpenClose_rp (uint32_t nodeId, std::string destination, Ipv4RoutePathElements rpElements)
{
  std::ostringstream oss;
  oss << "<" << "rp" << " t =\"" << Simulator::Now ().GetSeconds () << "\""
      << " id=\"" << nodeId << "\"" << " d=\"" << destination.c_str () << "\""
      << " c=\""  << rpElements.size () << "\"" << ">" << std::endl;
  for (Ipv4RoutePathElements::const_iterator i = rpElements.begin ();
       i != rpElements.end ();
       ++i)
    {
      Ipv4RoutePathElement rpElement = *i;
      oss << "<rpe" << " n=\"" << rpElement.nodeId << "\"" << " nH=\"" << rpElement.nextHop.c_str () << "\"" << "/>" << std::endl;
    }
  oss << "</rp>" << std::endl;
  return oss.str ();
}


std::string AnimationInterface::GetXMLOpenClose_p (std::string pktType, uint32_t fId, double fbTx, double lbTx, 
                                                   uint32_t tId, double fbRx, double lbRx, std::string metaInfo, 
                                                   std::string auxInfo)
{
  std::ostringstream oss;
  oss << std::setprecision (10);
  oss << "<" << pktType << " fId=\"" << fId
      << "\" fbTx=\"" << fbTx
      << "\" lbTx=\"" << lbTx << "\"";
  if (!auxInfo.empty ())
    {
      oss << " aux=\"" << auxInfo.c_str () << "\"";
    }
  if (!metaInfo.empty ())
    {
      oss << " meta-info=\"" << metaInfo.c_str () << "\"";
    }
  oss << " tId=\"" << tId << "\" fbRx=\"" << fbRx << "\" lbRx=\"" << lbRx << "\">" << std::endl;
  return oss.str ();
}



std::string AnimationInterface::GetXMLOpen_wpacket (uint32_t fromLp, uint32_t fromId, double fbTx, double lbTx, double range)
{
  std::ostringstream oss;
  oss << std::setprecision (10);
  oss << "<wpacket fromId=\"" << fromId
      << "\" fbTx=\"" << fbTx
      << "\" lbTx=\"" << lbTx
      << "\" range=\"" << range << "\">" << std::endl;
  return oss.str ();

}

std::string AnimationInterface::GetXMLOpenClose_rx (uint32_t toLp, uint32_t toId, double fbRx, double lbRx)
{
  std::ostringstream oss;
  oss << std::setprecision (10);
  oss << "<rx toId=\"" << toId
      << "\" fbRx=\"" << fbRx
      << "\" lbRx=\"" << lbRx
      << "\"/>" << std::endl;
  return oss.str ();
}

std::string AnimationInterface::GetXMLOpenClose_meta (std::string metaInfo)
{
  std::ostringstream oss;
  oss << "<meta info=\""
      << metaInfo << "\" />" << std::endl;
  return oss.str ();      
}


std::string AnimationInterface::GetXMLOpenClose_NonP2pLinkProperties (uint32_t id, std::string ipv4Address, std::string channelType)
{
  std::ostringstream oss;
  oss << "<nonp2plinkproperties id=\""
      << id << "\""
      << " ipv4Address=\"" << ipv4Address << "\""
      << " channelType=\"" << channelType << "\""
      << "/>" << std::endl;
  return oss.str ();
}


const std::vector<std::string> AnimationInterface::GetElementsFromContext (const std::string& context) const
{
  std::vector <std::string> elements;
  size_t pos1=0, pos2;
  while (pos1 != context.npos)
  {
    pos1 = context.find ("/",pos1);
    pos2 = context.find ("/",pos1+1);
    elements.push_back (context.substr (pos1+1,pos2-(pos1+1)));
    pos1 = pos2; 
    pos2 = context.npos;
  }
  return elements;
}

void AnimationInterface::AppendXMLNodeDescription(std::ostream& ostream, uint32_t id) const
{
  if (nodeDescriptions.find (id) != nodeDescriptions.end ())
    {
      ostream << " descr=\""<< nodeDescriptions[id] << "\"";
    }
  else
    {
      ostream << " descr=\"\"";
    }
}

void AnimationInterface::AppendXMLNodeColor(std::ostream& ostream, const Rgb& color) const
{
  const uint8_t r = color.r;
  const uint8_t g = color.g;
  const uint8_t b = color.b;

  ostream << " r=\"" << (uint32_t)r << "\" "
          << " g=\"" << (uint32_t)g << "\" "
          << " b=\"" << (uint32_t)b <<"\" ";
}

void AnimationInterface::AppendXMLRemainingEnergy(std::ostream& ostream, uint32_t id) const
{
  const EnergyFractionMap::const_iterator fractionIter = m_nodeEnergyFraction.find(id);

  if(fractionIter != m_nodeEnergyFraction.end())
    ostream << "rc = \"" << fractionIter->second <<"\" ";
  else if(NodeList::GetNode (id)->GetObject<EnergySource>())
    ostream << "rc = \"1\" ";
}

TypeId
AnimByteTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AnimByteTag")
    .SetParent<Tag> ()
    .AddConstructor<AnimByteTag> ()
  ;
  return tid;
}
TypeId
AnimByteTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
AnimByteTag::GetSerializedSize (void) const
{
  return sizeof (uint64_t);
}
void
AnimByteTag::Serialize (TagBuffer i) const
{
  i.WriteU64 (m_AnimUid);
}
void
AnimByteTag::Deserialize (TagBuffer i)
{
  m_AnimUid = i.ReadU64 ();
}
void
AnimByteTag::Print (std::ostream &os) const
{
  os << "AnimUid=" << m_AnimUid;
}
void
AnimByteTag::Set (uint64_t AnimUid)
{
  m_AnimUid = AnimUid;
}

uint64_t
AnimByteTag::Get (void) const
{
  return m_AnimUid;
}


} // namespace ns3
