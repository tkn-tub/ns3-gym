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
 */

// Interface between ns3 and the network animator


// ns3 includes
#include "ns3/animation-interface.h"
#include "ns3/channel.h"
#include "ns3/config.h"
#include "ns3/node.h"
#include "ns3/random-variable.h"
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

#include <stdio.h>
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

AnimationInterface::AnimationInterface (const std::string fn, uint64_t maxPktsPerFile, bool usingXML)
  : m_xml (usingXML), m_mobilityPollInterval (Seconds(0.25)), 
    m_outputFileName (fn),
    m_outputFileSet (false), gAnimUid (0), m_randomPosition (true),
    m_writeCallback (0), m_started (false), 
    m_enablePacketMetadata (false), m_startTime (Seconds(0)), m_stopTime (Seconds(3600 * 1000)),
    m_maxPktsPerFile (maxPktsPerFile), m_originalFileName (fn)
{
  initialized = true;
  StartAnimation ();
}

AnimationInterface::~AnimationInterface ()
{
  StopAnimation ();
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
  StopAnimation ();
  m_outputFileName = m_originalFileName + "-" + oss.str ();
  StartAnimation (true);
  ++i;

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
  m_f = fopen (fn.c_str (), "w");
  if (!m_f)
    {
      NS_FATAL_ERROR ("Unable to open Animation output file");
      return false; // Can't open
    }
  m_outputFileName = fn;
  m_outputFileSet = true;
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
     Vector randomVector (UniformVariable (0, m_topoMaxX - m_topoMinX).GetValue (), UniformVariable (0, m_topoMaxY - m_topoMinY).GetValue (), 0);
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
    }

  AddMargin ();
  if (m_xml)
    { // output the xml headers
      std::ostringstream oss;
      oss << GetXMLOpen_anim (0);
      oss << GetPreamble ();
      oss << GetXMLOpen_topology (m_topoMinX, m_topoMinY, m_topoMaxX, m_topoMaxY);
      WriteN (oss.str ());
    }
  NS_LOG_INFO ("Setting topology for "<<NodeList::GetNNodes ()<<" Nodes");
  // Dump the topology
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      std::ostringstream oss;
      if (m_xml)
        {
          Vector v = GetPosition (n);
          oss << GetXMLOpenClose_node (0, n->GetId (), v.x, v.y);
	  WriteN (oss.str ());
        }
      else
        {
          // Location exists, dump it
          Vector v = GetPosition (n);
          oss << "0.0 N " << n->GetId () 
              << " " << v.x << " " << v.y << std::endl;
      	  WriteN (oss.str ().c_str (), oss.str ().length ());
        }
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
              continue;
            }
          std::string channelType = ch->GetInstanceTypeId ().GetName ();
          if (channelType == std::string ("ns3::PointToPointChannel"))
            { // Since these are duplex links, we only need to dump
              // if srcid < dstid
              uint32_t nChDev = ch->GetNDevices ();
              for (uint32_t j = 0; j < nChDev; ++j)
                {
                  Ptr<NetDevice> chDev = ch->GetDevice (j);
                  uint32_t n2Id = chDev->GetNode ()->GetId ();
                  if (n1Id < n2Id)
                    { // ouptut the p2p link
                      std::ostringstream oss;
                      if (m_xml)
                        {
                          oss << GetXMLOpenClose_link (0,n1Id,0,n2Id);
                        }
                      else
                        {
                          oss << "0.0 L "  << n1Id << " " << n2Id << std::endl;
                        }
                      WriteN (oss.str ());
                    }
                }
            }
          else
            {
              //NS_FATAL_ERROR ("Net animation currently only supports point-to-point links.");
            }
        }
    }
  if (m_xml && !restart)
    {
      WriteN (GetXMLClose ("topology"));
      Simulator::Schedule (m_mobilityPollInterval, &AnimationInterface::MobilityAutoCheck, this);
    }
  if (!restart)
    ConnectCallbacks ();
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

  ConnectLte ();

}


void AnimationInterface::StopAnimation ()
{
  m_started = false;
  NS_LOG_INFO ("Stopping Animation");
  ResetAnimWriteCallback ();
  if (m_f) 
    {
      if (m_xml)
        { // Terminate the anim element
          WriteN (GetXMLClose ("anim"));
        }
          fclose (m_f);
    }
    m_outputFileSet = false;
}

int AnimationInterface::WriteN (const std::string& st)
{
  if (m_writeCallback)
    {
      m_writeCallback (st.c_str ());
    }
  return WriteN (st.c_str (), st.length ());
}

// Private methods
void AnimationInterface::AddMargin ()
{
  // Compute width/height, and add a small margin
  double w = m_topoMaxX - m_topoMinX;
  double h = m_topoMaxY - m_topoMinY;
  m_topoMinX -= w * 0.05;
  m_topoMinY -= h * 0.05;
  m_topoMaxX = m_topoMinX + w * 1.5;
  m_topoMaxY = m_topoMinY + h * 1.5;
  NS_LOG_INFO ("Added Canvas Margin:" << m_topoMinX << "," <<
               m_topoMinY << "," << m_topoMaxX << "," << m_topoMaxY);                 
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
  double oldminX = m_topoMinX;
  double oldminY = m_topoMinY;
  double oldmaxX = m_topoMaxX;
  double oldmaxY = m_topoMaxY;
  m_topoMinX = std::min (m_topoMinX, v.x);
  m_topoMinY = std::min (m_topoMinY, v.y);
  m_topoMaxX = std::max (m_topoMaxX, v.x);
  m_topoMaxY = std::max (m_topoMaxY, v.y);
  
  if ((m_topoMinX != oldminX) || (m_topoMinY != oldminY) ||
      (m_topoMaxX != oldmaxX) || (m_topoMaxY != oldmaxY))
    {
      AddMargin ();
    } 
}

int AnimationInterface::WriteN (const char* data, uint32_t count)
{ 
  // Write count bytes to h from data
  uint32_t    nLeft   = count;
  const char* p       = data;
  uint32_t    written = 0;
  while (nLeft)
    {
      int n = fwrite (p, 1,  nLeft, m_f);
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
  if (m_xml)
    {
      oss << GetXMLOpen_packet (0, 0, fbTx, lbTx, "DummyPktIgnoreThis");
      oss << GetXMLOpenClose_rx (0, 0, fbRx, lbRx);
      oss << GetXMLClose ("packet");
    }
  WriteN (oss.str ());


}
void AnimationInterface::DevTxTrace (std::string context, Ptr<const Packet> p,
                                     Ptr<NetDevice> tx, Ptr<NetDevice> rx,
                                     Time txTime, Time rxTime)
{
  if (!m_started)
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
      oss << GetXMLOpen_packet (0, tx->GetNode ()->GetId (), fbTx, lbTx);
      oss << GetXMLOpenClose_rx (0, rx->GetNode ()->GetId (), fbRx, lbRx); 
      if (m_enablePacketMetadata)
        oss << GetXMLOpenClose_meta (GetPacketMetadata (p));
      oss << GetXMLClose ("packet");
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
  WriteN (oss.str ());
}


Ptr <NetDevice>
AnimationInterface::GetNetDeviceFromContext (std::string context)
{
  // Use "NodeList/*/DeviceList/*/ as reference
  // where element [1] is the Node Id
  // element [2] is the NetDevice Id

  std::vector <std::string> elements = GetElementsFromContext (context);
  Ptr <Node> n = NodeList::GetNode (atoi (elements[1].c_str ()));
  NS_ASSERT (n);
  return n->GetDevice (atoi (elements[3].c_str ()));
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
  NS_LOG_INFO ("TxBeginTrace for packet:" << gAnimUid);
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
  NS_LOG_INFO ("RxBeginTrace for packet:" << AnimUid);
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
  // TODO: NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
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
  // TODO: NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
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
  // TODO: NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
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
  //TODO 0.0001 is used until Wimax implements TxBegin and TxEnd traces
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
  //TODO 0.001 is used until Wimax implements RxBegin and RxEnd traces
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
  //TODO 0.0001 is used until Lte implements TxBegin and TxEnd traces
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
  //TODO 0.001 is used until Lte implements RxBegin and RxEnd traces
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  OutputWirelessPacket (p, pktInfo, pktrxInfo);
}

void AnimationInterface::LteSpectrumPhyTxStart (std::string context, Ptr<const PacketBurst> pb)
{
  if (!m_started || !IsInTimeWindow ())
    return;
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
    //TODO 0.0001 is used until Lte implements TxBegin and TxEnd traces
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
    //TODO 0.001 is used until Lte implements RxBegin and RxEnd traces
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
  // TODO: NS_ASSERT (CsmaPacketIsPending (AnimUid) == true);
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
  // TODO: NS_ASSERT (CsmaPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = m_pendingCsmaPackets[AnimUid];
  m_pendingCsmaPackets[AnimUid].ProcessRxBegin (ndev, Simulator::Now ());
  pktInfo.ProcessRxEnd (ndev, Simulator::Now (), UpdatePosition (n));
  NS_LOG_INFO ("CsmaPhyRxEndTrace for packet:" << AnimUid);
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
  // TODO: NS_ASSERT (CsmaPacketIsPending (AnimUid) == true);
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
  oss << GetXMLOpenClose_node (0,n->GetId (),v.x,v.y);
  oss << GetXMLClose ("topology");
  WriteN (oss.str ());
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
  WriteN (oss.str ());
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


// Helper to output a wireless packet.
// For now, only the XML interface is supported


std::string AnimationInterface::GetPreamble ()
{
  std::string s = 
    "<information><!-- \n\
    Description of attributes:\n\
    =========================\n\
    anim\n\
    topology\n\
    * minX = minimum X coordinate of the canvas\n\
    * minY = minimum Y coordinate of the canvas\n\
    * maxX = maximum X coordinate of the canvas\n\
    * maxY = maximum Y coordinate of the canvas\n\
    node\n\
    * id = Node Id\n\
    * locX = X coordinate\n\
    * locY = Y coordinate\n\
    link\n\
    * fromId = From Node Id\n\
    * toId   = To Node Id\n\
    packet\n\
    * fbTx = First bit transmit time\n\
    * lbTx = Last bit transmit time\n\
    rx\n\
    * toId = To Node Id\n\
    * fbRx = First bit Rx Time\n\
    * lbRx = Last bit Rx\n\
    wpacket\n\
    * fromId = From Node Id\n\
    * fbTx = First bit transmit time\n\
    * lbTx = Last bit transmit time\n\
    * range = Reception range\n\
    rx\n\
    * toId = To Node Id\n\
    * fbRx = First bit Rx time\n\
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
  oss << GetXMLOpen_wpacket (0, nodeId, pktInfo.m_fbTx, lbTx, pktrxInfo.rxRange);

  uint32_t rxId = pktrxInfo.m_rxnd->GetNode ()->GetId ();
  oss << GetXMLOpenClose_rx (0, rxId, pktrxInfo.m_fbRx, pktrxInfo.m_lbRx);
  if (m_enablePacketMetadata)
    oss << GetXMLOpenClose_meta (GetPacketMetadata (p));

  oss << GetXMLClose ("wpacket");
  WriteN (oss.str ());
}

void AnimationInterface::OutputCsmaPacket (Ptr<const Packet> p, AnimPacketInfo &pktInfo, AnimRxInfo pktrxInfo)
{
  StartNewTraceFile ();
  NS_ASSERT (m_xml);
  std::ostringstream oss;
  NS_ASSERT (pktInfo.m_txnd);
  uint32_t nodeId = pktInfo.m_txnd->GetNode ()->GetId ();

  oss << GetXMLOpen_packet (0, nodeId, pktInfo.m_fbTx, pktInfo.m_lbTx);
  uint32_t rxId = pktrxInfo.m_rxnd->GetNode ()->GetId ();
  oss << GetXMLOpenClose_rx (0, rxId, pktrxInfo.m_fbRx, pktrxInfo.m_lbRx);
  if (m_enablePacketMetadata)
    oss << GetXMLOpenClose_meta (GetPacketMetadata (p));
  oss << GetXMLClose ("packet");
  WriteN (oss.str ());
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

void AnimationInterface::SetNodeDescription (Ptr <Node> n, std::string descr) 
{
  NS_ASSERT (n);
  nodeDescriptions[n->GetId ()] = descr;
}

void AnimationInterface::SetNodeDescription (NodeContainer nc, std::string descr)
{
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
  oss <<"<anim lp = \"" << lp << "\" >\n";
  return oss.str ();
}
std::string AnimationInterface::GetXMLOpen_topology (double minX,double minY,double maxX,double maxY)
{
  std::ostringstream oss;
  oss << "<topology minX = \"" << minX << "\" minY = \"" << minY
      << "\" maxX = \"" << maxX << "\" maxY = \"" << maxY
      << "\">" << std::endl;
  return oss.str ();

}

std::string AnimationInterface::GetXMLOpenClose_node (uint32_t lp,uint32_t id,double locX,double locY)
{
  std::ostringstream oss;
  oss <<"<node id = \"" << id << "\""; 
  if (nodeDescriptions.find (id) != nodeDescriptions.end ())
    {
      oss << " descr=\""<< nodeDescriptions[id] << "\"";
    }
  else
    {
      oss << " descr=\"\"";
    }
  oss << " locX = \"" << locX << "\" " << "locY = \"" << locY << "\" />\n";
  return oss.str ();
}
std::string AnimationInterface::GetXMLOpenClose_link (uint32_t fromLp,uint32_t fromId, uint32_t toLp, uint32_t toId)
{
  std::ostringstream oss;
  oss << "<link fromId=\"" << fromId
      << "\" toLp=\"0\" toId=\"" << toId
      << "\"/>" << std::endl;
  return oss.str ();
}


std::string AnimationInterface::GetXMLOpen_packet (uint32_t fromLp,uint32_t fromId, double fbTx, double lbTx, std::string auxInfo)
{
  std::ostringstream oss;
  oss << std::setprecision (10);
  oss << "<packet fromId=\"" << fromId
      << "\" fbTx=\"" << fbTx
      << "\" lbTx=\"" << lbTx
      << (auxInfo.empty()?"":"\" aux=\"") << auxInfo.c_str ()
      << "\">";
  return oss.str ();
}

std::string AnimationInterface::GetXMLOpen_wpacket (uint32_t fromLp,uint32_t fromId, double fbTx, double lbTx, double range)
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

std::vector<std::string> AnimationInterface::GetElementsFromContext (std::string context)
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
