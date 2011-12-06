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
#include "ns3/constant-position-mobility-model.h"

#include <stdio.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>

// Socket related includes
#if defined(HAVE_SYS_SOCKET_H) && defined(HAVE_NETINET_IN_H)
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <fcntl.h>
#endif

NS_LOG_COMPONENT_DEFINE ("AnimationInterface");

#define PURGE_INTERVAL 5
static bool initialized = false;

namespace ns3 {

AnimationInterface::AnimationInterface ()
  : m_fHandle (STDOUT_FILENO), m_xml (false), mobilitypollinterval (Seconds(0.25)),
    usingSockets (false), mport (0), outputfilename (""),
    OutputFileSet (false), ServerPortSet (false), gAnimUid (0),randomPosition (true),
    m_writeCallback (0), m_started (false)
{
  initialized = true;
  StartAnimation ();
}

AnimationInterface::AnimationInterface (const std::string fn, bool usingXML)
  : m_fHandle (STDOUT_FILENO), m_xml (usingXML), mobilitypollinterval (Seconds(0.25)), 
    usingSockets (false), mport (0), outputfilename (fn),
    OutputFileSet (false), ServerPortSet (false), gAnimUid (0), randomPosition (true),
    m_writeCallback (0), m_started (false)
{
  initialized = true;
  StartAnimation ();
}

AnimationInterface::AnimationInterface (const uint16_t port, bool usingXML)
  : m_fHandle (STDOUT_FILENO), m_xml (usingXML), mobilitypollinterval (Seconds(0.25)), 
    usingSockets (true), mport (port), outputfilename (""),
    OutputFileSet (false), ServerPortSet (false), gAnimUid (0), randomPosition (true),
    m_writeCallback (0), m_started (false)
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

bool AnimationInterface::SetOutputFile (const std::string& fn)
{
  if (OutputFileSet)
    {
      return true;
    }
  if (fn == "")
    {
      m_fHandle = STDOUT_FILENO;
      OutputFileSet = true;
      return true;
    }
  FILE* f = fopen (fn.c_str (), "w");
  if (!f)
    {
      NS_FATAL_ERROR ("Unable to open Animation output file");
      return false; // Can't open
    }
  m_fHandle = fileno (f); // Set the file handle
  usingSockets = false;
  outputfilename = fn;
  OutputFileSet = true;
  return true;
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

bool AnimationInterface::SetServerPort (uint16_t port)
{
#if defined(HAVE_SYS_SOCKET_H) && defined(HAVE_NETINET_IN_H)
  if (ServerPortSet)
    {
      return true;
    }
  int s = socket (AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons (port);
  addr.sin_addr.s_addr = htonl (INADDR_ANY);
  if (bind (s, (struct sockaddr*)&addr, sizeof (addr)) < 0)
    {
      NS_LOG_WARN ("Can't bind to port " << port << ", exiting.");
      return false;
    }
  listen (s, 1);
  NS_LOG_INFO ("Waiting for animator connection");
  // Now wait for the animator to connect in
  m_fHandle = accept (s, 0, 0);
  NS_LOG_INFO ("Got animator connection from remote");
  // set the linger socket option
  int t = 1;
  setsockopt (s, SOL_SOCKET, SO_LINGER, &t, sizeof(t));
  usingSockets = true;
  ServerPortSet = true;
  return true;
#endif
  return false; // never reached unless the above is disabled
                // which is done to support a platform like MinGW
}

bool AnimationInterface::WifiPacketIsPending (uint64_t AnimUid)
{
  return (pendingWifiPackets.find (AnimUid) != pendingWifiPackets.end ());
}

bool AnimationInterface::CsmaPacketIsPending (uint64_t AnimUid)
{
  return (pendingCsmaPackets.find (AnimUid) != pendingCsmaPackets.end ());
}


bool AnimationInterface::WimaxPacketIsPending (uint64_t AnimUid)
{
  return (pendingWimaxPackets.find (AnimUid) != pendingWimaxPackets.end ());
}

void AnimationInterface::SetMobilityPollInterval (Time t)
{
  mobilitypollinterval = t;
}

void AnimationInterface::SetRandomPosition (bool setRandPos)
{
  randomPosition = setRandPos;
}

Vector AnimationInterface::UpdatePosition (Ptr <Node> n)
{
  Ptr<MobilityModel> loc = n->GetObject<MobilityModel> ();
  if (loc)
    {
      nodeLocation[n->GetId ()] = loc->GetPosition ();
    }
  else
   {
     NS_LOG_WARN ( "Node:" << n->GetId () << " Does not have a mobility model");
     Vector deterministicVector (100,100,0);
     Vector randomVector (UniformVariable (0, topo_maxX-topo_minX).GetValue (), UniformVariable (0, topo_maxY-topo_minY).GetValue (), 0);
     if (randomPosition)
       {
         nodeLocation[n->GetId ()] = randomVector;
       }
     else
       {
         nodeLocation[n->GetId ()] = deterministicVector;
       }
   }
  return nodeLocation[n->GetId ()];
}

Vector AnimationInterface::UpdatePosition (Ptr <Node> n, Vector v)
{
  nodeLocation[n->GetId ()] = v;
  return v;
}

Vector AnimationInterface::GetPosition (Ptr <Node> n)
{
  #ifdef NS_LOG
  if (nodeLocation.find (n->GetId()) == nodeLocation.end ())
    {
      NS_FATAL_ERROR ("Node:" <<n->GetId() << " not found in Location table");
    }  
  #endif
  return nodeLocation[n->GetId ()];
}

void AnimationInterface::PurgePendingWifi ()
{
  if (pendingWifiPackets.empty ())
    return;
  std::vector <uint64_t> purgeList;
  for (std::map<uint64_t, AnimPacketInfo>::iterator i = pendingWifiPackets.begin ();
       i != pendingWifiPackets.end ();
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
      pendingWifiPackets.erase (*i);
    }

}

void AnimationInterface::PurgePendingWimax ()
{
  if (pendingWimaxPackets.empty ())
    return;
  std::vector <uint64_t> purgeList;
  for (std::map<uint64_t, AnimPacketInfo>::iterator i = pendingWimaxPackets.begin ();
       i != pendingWimaxPackets.end ();
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
      pendingWimaxPackets.erase (*i);
    }

}

void AnimationInterface::PurgePendingCsma ()
{
  if (pendingCsmaPackets.empty ())
    return;
  std::vector <uint64_t> purgeList;
  for (std::map<uint64_t, AnimPacketInfo>::iterator i = pendingCsmaPackets.begin ();
       i != pendingCsmaPackets.end ();
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
      pendingCsmaPackets.erase (*i);
    }

}

void AnimationInterface::StartAnimation ()
{
  m_started = true;
  if (usingSockets)
    {
      SetServerPort (mport);
    }
  else
    {
      SetOutputFile (outputfilename);
    }      

  // Find the min/max x/y for the xml topology element
  topo_minX = -2;
  topo_minY = -2;
  topo_maxX = 2;
  topo_maxY = 2;
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      NS_LOG_INFO ("Update Position for Node: " << n->GetId ());
      Vector v = UpdatePosition (n); 
      topo_minX = std::min (topo_minX, v.x);
      topo_minY = std::min (topo_minY, v.y);
      topo_maxX = std::max (topo_maxX, v.x);
      topo_maxY = std::max (topo_maxY, v.y);
    }

  AddMargin ();
  if (m_xml)
    { // output the xml headers
      std::ostringstream oss;
      oss << GetXMLOpen_anim (0);
      oss << GetPreamble ();
      oss << GetXMLOpen_topology (topo_minX, topo_minY, topo_maxX, topo_maxY);
      WriteN (m_fHandle, oss.str ());
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
	  WriteN (m_fHandle, oss.str ());
        }
      else
        {
          // Location exists, dump it
          Vector v = GetPosition (n);
          oss << "0.0 N " << n->GetId () 
              << " " << v.x << " " << v.y << std::endl;
      	  WriteN (m_fHandle, oss.str ().c_str (), oss.str ().length ());
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
                      WriteN (m_fHandle, oss.str ());
                    }
                }
            }
          else
            {
              //NS_FATAL_ERROR ("Net animation currently only supports point-to-point links.");
            }
        }
    }
  if (m_xml)
    {
      WriteN (m_fHandle, GetXMLClose ("topology"));
      Simulator::Schedule (mobilitypollinterval, &AnimationInterface::MobilityAutoCheck, this);
    }

  ConnectCallbacks ();
}

void AnimationInterface::ConnectCallbacks ()
{
  // Connect the callbacks
  Config::Connect ("/ChannelList/*/TxRxPointToPoint",
                   MakeCallback (&AnimationInterface::DevTxTrace, this));
  Config::Connect ("NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin",
                   MakeCallback (&AnimationInterface::WifiPhyTxBeginTrace, this));
  Config::Connect ("NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxEnd",
                   MakeCallback (&AnimationInterface::WifiPhyTxEndTrace, this));
  Config::Connect ("NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxBegin",
                   MakeCallback (&AnimationInterface::WifiPhyRxBeginTrace, this));
  Config::Connect ("NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxEnd",
                   MakeCallback (&AnimationInterface::WifiPhyRxEndTrace, this));
  Config::Connect ("NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/MacRx",
                   MakeCallback (&AnimationInterface::WifiMacRxTrace, this));
  Config::ConnectWithoutContext ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeCallback (&AnimationInterface::MobilityCourseChangeTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WimaxNetDevice/Tx",
                   MakeCallback (&AnimationInterface::WimaxTxTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WimaxNetDevice/Rx",
                   MakeCallback (&AnimationInterface::WimaxRxTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/PhyTxBegin",
                   MakeCallback (&AnimationInterface::CsmaPhyTxBeginTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/PhyTxEnd",
                   MakeCallback (&AnimationInterface::CsmaPhyTxEndTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/PhyRxEnd",
                   MakeCallback (&AnimationInterface::CsmaPhyRxEndTrace, this));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/MacRx",
                   MakeCallback (&AnimationInterface::CsmaMacRxTrace, this));


}


void AnimationInterface::StopAnimation ()
{
  m_started = false;
  NS_LOG_INFO ("Stopping Animation");
  ResetAnimWriteCallback ();
  if (m_fHandle > 0) 
    {
      if (m_xml)
        { // Terminate the anim element
          WriteN (m_fHandle, GetXMLClose ("anim"));
        }
      if (m_fHandle != STDOUT_FILENO)
        {
          close (m_fHandle);
        }
      OutputFileSet = false;
      m_fHandle = -1;
    }
}

int AnimationInterface::WriteN (int h, const std::string& st)
{
  if (h < 0)
    { 
      return 0;
    }
  if (m_writeCallback)
    {
      m_writeCallback (st.c_str ());
    }
  return WriteN (h, st.c_str (), st.length ());
}


// Private methods
void AnimationInterface::AddMargin ()
{
  // Compute width/height, and add a small margin
  double w = topo_maxX - topo_minX;
  double h = topo_maxY - topo_minY;
  topo_minX -= w * 0.05;
  topo_minY -= h * 0.05;
  topo_maxX = topo_minX + w * 1.5;
  topo_maxY = topo_minY + h * 1.5;
  NS_LOG_INFO ("Added Canvas Margin:" << topo_minX << "," <<
               topo_minY << "," << topo_maxX << "," << topo_maxY);                 
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
  double oldminX = topo_minX;
  double oldminY = topo_minY;
  double oldmaxX = topo_maxX;
  double oldmaxY = topo_maxY;
  topo_minX = std::min (topo_minX, v.x);
  topo_minY = std::min (topo_minY, v.y);
  topo_maxX = std::max (topo_maxX, v.x);
  topo_maxY = std::max (topo_maxY, v.y);
  
  if ((topo_minX != oldminX) || (topo_minY != oldminY) ||
      (topo_maxX != oldmaxX) || (topo_maxY != oldmaxY))
    {
      AddMargin ();
    } 
}

int AnimationInterface::WriteN (HANDLETYPE h, const char* data, uint32_t count)
{ 
  if (h < 0)
    {
      return 0;
    }
  // Write count bytes to h from data
  uint32_t    nLeft   = count;
  const char* p       = data;
  uint32_t    written = 0;
  while (nLeft)
    {
      int n = write (h, p, nLeft);
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
      oss << GetXMLOpen_packet (0,0,fbTx,lbTx,"DummyPktIgnoreThis");
      oss << GetXMLOpenClose_rx (0,0,fbRx,lbRx);
      oss << GetXMLClose ("packet");
    }
  WriteN (m_fHandle, oss.str ());


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
      oss << GetXMLOpen_packet (0,tx->GetNode ()->GetId (),fbTx,lbTx);
      oss << GetXMLOpenClose_rx (0,rx->GetNode ()->GetId (),fbRx,lbRx); 
      oss << GetXMLClose ("packet");
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
  WriteN (m_fHandle, oss.str ());
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
  NS_ASSERT (pktinfo.m_txnd);
  pendingWifiPackets[AnimUid] = pktinfo;
}

void AnimationInterface::AddPendingWimaxPacket (uint64_t AnimUid, AnimPacketInfo &pktinfo)
{
  NS_ASSERT (pktinfo.m_txnd);
  pendingWimaxPackets[AnimUid] = pktinfo;
}


void AnimationInterface::AddPendingCsmaPacket (uint64_t AnimUid, AnimPacketInfo &pktinfo)
{
  NS_ASSERT (pktinfo.m_txnd);
  pendingCsmaPackets[AnimUid] = pktinfo;
}

uint64_t AnimationInterface::GetAnimUidFromPacket (Ptr <const Packet> p)
{
  AnimByteTag tag;
  if (p->FindFirstMatchingByteTag (tag))
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
  if (!m_started)
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
}

void AnimationInterface::WifiPhyTxEndTrace (std::string context,
                                            Ptr<const Packet> p)
{
}

void AnimationInterface::WifiPhyTxDropTrace (std::string context,
                                             Ptr<const Packet> p)
{
  if (!m_started)
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  // Erase pending wifi
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("TxDropTrace for packet:" << AnimUid);
  NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
  pendingWifiPackets.erase (pendingWifiPackets.find (AnimUid));
}


void AnimationInterface::WifiPhyRxBeginTrace (std::string context,
                                              Ptr<const Packet> p)
{
  if (!m_started)
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("RxBeginTrace for packet:" << AnimUid);
  if (!WifiPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("WifiPhyRxBeginTrace: unknown Uid");
      return;
    }
  // TODO: NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
  pendingWifiPackets[AnimUid].ProcessRxBegin (ndev, Simulator::Now ());
}


void AnimationInterface::WifiPhyRxEndTrace (std::string context,
                                            Ptr<const Packet> p)
{
  if (!m_started)
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  if (!WifiPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("WifiPhyRxEndTrace: unknown Uid");
      return;
    }
  // TODO: NS_ASSERT (WifiPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = pendingWifiPackets[AnimUid];
  pktInfo.ProcessRxEnd (ndev, Simulator::Now (), UpdatePosition (n));
}

void AnimationInterface::WifiMacRxTrace (std::string context,
                                         Ptr<const Packet> p)
{
  if (!m_started)
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
  AnimPacketInfo& pktInfo = pendingWifiPackets[AnimUid];
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  if (pktrxInfo.IsPhyRxComplete ())
    {
      NS_LOG_INFO ("MacRxTrace for packet:" << AnimUid << " complete");
      OutputWirelessPacket (pktInfo, pktrxInfo);
    }

}
void AnimationInterface::WifiPhyRxDropTrace (std::string context,
                                             Ptr<const Packet> p)
{
}

void AnimationInterface::WimaxTxTrace (std::string context, Ptr<const Packet> p, const Mac48Address & m)
{
  if (!m_started)
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
  if (!m_started)
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  Ptr <Node> n = ndev->GetNode ();
  NS_ASSERT (n);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_ASSERT (WimaxPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = pendingWimaxPackets[AnimUid];
  pktInfo.ProcessRxBegin (ndev, Simulator::Now ());
  pktInfo.ProcessRxEnd (ndev, Simulator::Now () + Seconds (0.001), UpdatePosition (n));
  //TODO 0.001 is used until Wimax implements RxBegin and RxEnd traces
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  OutputWirelessPacket (pktInfo, pktrxInfo);
}

void AnimationInterface::CsmaPhyTxBeginTrace (std::string context, Ptr<const Packet> p)
{
  if (!m_started)
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
  if (!m_started)
    return;
  Ptr <NetDevice> ndev = GetNetDeviceFromContext (context);
  NS_ASSERT (ndev);
  uint64_t AnimUid = GetAnimUidFromPacket (p);
  NS_LOG_INFO ("CsmaPhyTxEndTrace for packet:" << AnimUid);
  if (!CsmaPacketIsPending (AnimUid))
    {
      NS_LOG_WARN ("CsmaPhyTxEndTrace: unknown Uid"); 
      return;
    }
  // TODO: NS_ASSERT (CsmaPacketIsPending (AnimUid) == true);
  AnimPacketInfo& pktInfo = pendingCsmaPackets[AnimUid];
  pktInfo.m_lbTx = Simulator::Now ().GetSeconds ();
}

void AnimationInterface::CsmaPhyRxEndTrace (std::string context, Ptr<const Packet> p)
{
  if (!m_started)
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
  AnimPacketInfo& pktInfo = pendingCsmaPackets[AnimUid];
  pendingCsmaPackets[AnimUid].ProcessRxBegin (ndev, Simulator::Now ());
  pktInfo.ProcessRxEnd (ndev, Simulator::Now (), UpdatePosition (n));
  NS_LOG_INFO ("CsmaPhyRxEndTrace for packet:" << AnimUid);
}


void AnimationInterface::CsmaMacRxTrace (std::string context,
                                         Ptr<const Packet> p)
{
  if (!m_started)
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
  AnimPacketInfo& pktInfo = pendingCsmaPackets[AnimUid];
  AnimRxInfo pktrxInfo = pktInfo.GetRxInfo (ndev);
  if (pktrxInfo.IsPhyRxComplete ())
    {
      NS_LOG_INFO ("MacRxTrace for packet:" << AnimUid << " complete");
      OutputCsmaPacket (pktInfo, pktrxInfo);
    }
}


void AnimationInterface::MobilityCourseChangeTrace (Ptr <const MobilityModel> mobility)

{
  if (!m_started)
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
  oss << GetXMLOpen_topology (topo_minX,topo_minY,topo_maxX,topo_maxY);
  oss << GetXMLOpenClose_node (0,n->GetId (),v.x,v.y);
  oss << GetXMLClose ("topology");
  WriteN (m_fHandle, oss.str ());
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
  std::vector <Ptr <Node> > MovedNodes = RecalcTopoBounds ();
  std::ostringstream oss;
  oss << GetXMLOpen_topology (topo_minX, topo_minY, topo_maxX, topo_maxY);
  for (uint32_t i = 0; i < MovedNodes.size (); i++)
    {
      Ptr <Node> n = MovedNodes [i];
      NS_ASSERT (n);
      Vector v = GetPosition (n);
      oss << GetXMLOpenClose_node (0, n->GetId (), v.x, v.y);
    }
  oss << GetXMLClose ("topology");
  WriteN (m_fHandle, oss.str ());
  WriteDummyPacket ();
  if (!Simulator::IsFinished ())
    {
      PurgePendingWifi ();
      PurgePendingWimax ();
      PurgePendingCsma ();
      Simulator::Schedule (mobilitypollinterval, &AnimationInterface::MobilityAutoCheck, this);
    }
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
    * lp = Logical Processor Id\n\
    topology\n\
    * minX = minimum X coordinate of the canvas\n\
    * minY = minimum Y coordinate of the canvas\n\
    * maxX = maximum X coordinate of the canvas\n\
    * maxY = maximum Y coordinate of the canvas\n\
    node\n\
    * lp = Logical Processor Id\n\
    * id = Node Id\n\
    * locX = X coordinate\n\
    * locY = Y coordinate\n\
    link\n\
    * fromLp = From logical processor Id\n\
    * fromId = From Node Id\n\
    * toLp   = To logical processor Id\n\
    * toId   = To Node Id\n\
    packet\n\
    * fromLp = From logical processor Id\n\
    * fbTx = First bit transmit time\n\
    * lbTx = Last bit transmit time\n\
    rx\n\
    * toLp = To logical processor Id\n\
    * toId = To Node Id\n\
    * fbRx = First bit Rx Time\n\
    * lbRx = Last bit Rx\n\
    wpacket\n\
    * fromLp = From logical processor Id\n\
    * fromId = From Node Id\n\
    * fbTx = First bit transmit time\n\
    * lbTx = Last bit transmit time\n\
    * range = Reception range\n\
    rx\n\
    * toLp = To logical processor Id\n\
    * toId = To Node Id\n\
    * fbRx = First bit Rx time\n\
    * lbRx = Last bit Rx time-->\n\
    </information>\n";
return s;
}

void AnimationInterface::OutputWirelessPacket (AnimPacketInfo &pktInfo, AnimRxInfo pktrxInfo)
{
  NS_ASSERT (m_xml);
  std::ostringstream oss;
  NS_ASSERT (pktInfo.m_txnd);
  uint32_t nodeId = pktInfo.m_txnd->GetNode ()->GetId ();

  double lbTx = pktInfo.firstlastbitDelta + pktInfo.m_fbTx;
  oss << GetXMLOpen_wpacket (0, nodeId, pktInfo.m_fbTx, lbTx, pktrxInfo.rxRange);

  uint32_t rxId = pktrxInfo.m_rxnd->GetNode ()->GetId ();
  oss << GetXMLOpenClose_rx (0, rxId, pktrxInfo.m_fbRx, pktrxInfo.m_lbRx);

  oss << GetXMLClose ("wpacket");
  WriteN (m_fHandle, oss.str ());
}

void AnimationInterface::OutputCsmaPacket (AnimPacketInfo &pktInfo, AnimRxInfo pktrxInfo)
{
  NS_ASSERT (m_xml);
  std::ostringstream oss;
  NS_ASSERT (pktInfo.m_txnd);
  uint32_t nodeId = pktInfo.m_txnd->GetNode ()->GetId ();

  oss << GetXMLOpen_packet (0, nodeId, pktInfo.m_fbTx, pktInfo.m_lbTx);
  uint32_t rxId = pktrxInfo.m_rxnd->GetNode ()->GetId ();
  oss << GetXMLOpenClose_rx (0, rxId, pktrxInfo.m_fbRx, pktrxInfo.m_lbRx);
  oss << GetXMLClose ("packet");
  WriteN (m_fHandle, oss.str ());
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
  oss <<"<node lp = \"" << lp << "\" id = \"" << id << "\"" << " locX = \"" 
      << locX << "\" " << "locY = \"" << locY << "\" />\n";
  return oss.str ();
}
std::string AnimationInterface::GetXMLOpenClose_link (uint32_t fromLp,uint32_t fromId, uint32_t toLp, uint32_t toId)
{
  std::ostringstream oss;
  oss << "<link fromLp=\"0\" fromId=\"" << fromId
      << "\" toLp=\"0\" toId=\"" << toId
      << "\"/>" << std::endl;
  return oss.str ();
}


std::string AnimationInterface::GetXMLOpen_packet (uint32_t fromLp,uint32_t fromId, double fbTx, double lbTx, std::string auxInfo)
{
  std::ostringstream oss;
  oss << std::setprecision (10);
  oss << "<packet fromLp=\"" << fromLp << "\" fromId=\"" << fromId
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
  oss << "<wpacket fromLp = \"" << fromLp << "\" fromId = \"" << fromId
      << "\" fbTx = \"" << fbTx
      << "\" lbTx = \"" << lbTx
      << "\" range = \"" << range << "\">" << std::endl;
  return oss.str ();

}

std::string AnimationInterface::GetXMLOpenClose_rx (uint32_t toLp, uint32_t toId, double fbRx, double lbRx)
{
  std::ostringstream oss;
  oss << std::setprecision (10);
  oss << "<rx toLp=\"" << toLp <<"\" toId=\"" << toId
      << "\" fbRx=\"" << fbRx
      << "\" lbRx=\"" << lbRx
      << "\"/>" << std::endl;
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
