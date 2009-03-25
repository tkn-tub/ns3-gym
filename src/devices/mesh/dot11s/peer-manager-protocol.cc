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
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#include "peer-manager-protocol.h"

#include "ns3/dot11s-parameters.h"
#include "ns3/simulator.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/mesh-wifi-interface-mac-plugin.h"
#include "peer-link.h"
#include "peer-manager-plugin.h"


NS_LOG_COMPONENT_DEFINE ("PeerManagerProtocol");
namespace ns3 {
namespace dot11s {
/***************************************************
 * PeerManager
 ***************************************************/
NS_OBJECT_ENSURE_REGISTERED (PeerManagerProtocol);

TypeId
PeerManagerProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PeerManagerProtocol")
    .SetParent<Object> ()
    .AddConstructor<PeerManagerProtocol> ()
    /// peerLinkCleanupTimeout. We go through the map of peer links and 
    /// remove all links which state is IDLE.
    .AddAttribute ("PeerLinkCleanupPeriod",
        "PeerLinkCleanupPeriod",
        TimeValue (MilliSeconds (80)),
        MakeTimeAccessor (&PeerManagerProtocol::m_peerLinkCleanupPeriod),
        MakeTimeChecker ()
        )
    /// maximum number of peer links. Now we calculate the total
    /// number of peer links on all interfaces
    .AddAttribute ("MaxNumberOfPeerLinks",
        "Maximum number of peer links ",
        UintegerValue (32),
        MakeUintegerAccessor (&PeerManagerProtocol::m_maxNumberOfPeerLinks),
        MakeUintegerChecker<uint8_t> ()
        );
  return tid;
}
PeerManagerProtocol::PeerManagerProtocol ():
  m_lastAssocId (0),
  m_lastLocalLinkId (1),
  m_numberOfActivePeers (0)
{
  m_cleanupEvent = Simulator::Schedule (m_peerLinkCleanupPeriod, &PeerManagerProtocol::PeerCleanup, this);
}
PeerManagerProtocol::~PeerManagerProtocol ()
{
  //cancel cleanup event and go through the map of peer links,
  //deleting each
  m_cleanupEvent.Cancel ();
  for (PeerLinksMap::iterator j = m_peerLinks.begin (); j != m_peerLinks.end (); j++)
    {
      for (PeerLinksOnInterface::iterator i = j->second.begin (); i != j->second.end(); i++)
        {
          (*i)->ClearTimingElement ();
          (*i) = 0;
        }
      j->second.clear ();
    }
  m_peerLinks.clear ();
  //cleaning beacon structures:
  for(BeaconInfoMap::iterator i =  m_neighbourBeacons.begin(); i != m_neighbourBeacons.end(); i ++)
  {
    i->second.clear();
  }
  m_neighbourBeacons.clear();
}

bool
PeerManagerProtocol::AttachInterfaces(std::vector<Ptr<WifiNetDevice> > interfaces)
{
  for(std::vector<Ptr<WifiNetDevice> >::iterator i = interfaces.begin(); i != interfaces.end(); i ++)
  {
    MeshWifiInterfaceMac * mac = dynamic_cast<MeshWifiInterfaceMac *> (PeekPointer ((*i)->GetMac ()));
    if (mac == NULL)
      return false;
    Ptr<PeerManagerMacPlugin> peerPlugin = Create<PeerManagerMacPlugin> ((*i)->GetIfIndex(), this);
    mac->InstallPlugin(peerPlugin);
    m_plugins[(*i)->GetIfIndex()] = peerPlugin;
    PeerLinksOnInterface newmap;
    m_peerLinks[(*i)->GetIfIndex()] = newmap;
  }
  return true;
}

Ptr<IeBeaconTiming>
PeerManagerProtocol::GetBeaconTimingElement(uint32_t interface)
{
  Ptr<IeBeaconTiming> retval = Create<IeBeaconTiming> ();
  BeaconInfoMap::iterator i = m_neighbourBeacons.find(interface);
  if(i == m_neighbourBeacons.end())
    return retval;
  bool cleaned = false;
  while(!cleaned)
  {
    for(BeaconsOnInterface::iterator j = i->second.begin(); j != i->second.end(); j++)
    {
      //check beacon loss and make a timing element
      //if last beacon was 3 beacons ago - we do not put it to the
      //timing element
      if(
          (j->second.referenceTbtt.GetMicroSeconds() +
           (j->second.beaconInterval.GetMicroSeconds()* 3))
          <
          Simulator::Now().GetMicroSeconds()
          )
      {
        i->second.erase(j);
        break;
      }
    }
    cleaned = true;
  }
  for(BeaconsOnInterface::iterator j = i->second.begin(); j != i->second.end(); j++)
    retval->AddNeighboursTimingElementUnit(j->second.aid, j->second.referenceTbtt, j->second.beaconInterval);
  return retval;
}
void
PeerManagerProtocol::FillBeaconInfo(uint32_t interface, Mac48Address peerAddress, Time receivingTime, Time beaconInterval)
{
  BeaconInfoMap::iterator i = m_neighbourBeacons.find(interface);
  if(i == m_neighbourBeacons.end())
  {
     BeaconsOnInterface newMap;
     m_neighbourBeacons[interface] = newMap;
  }
  i = m_neighbourBeacons.find(interface);
  BeaconsOnInterface::iterator j = i->second.find(peerAddress);
  if(j == i->second.end())
  {
    BeaconInfo newInfo;
    newInfo.referenceTbtt = receivingTime;
    newInfo.beaconInterval = beaconInterval;
    newInfo.aid = m_lastAssocId++;
    if(m_lastAssocId == 0xff)
      m_lastAssocId = 0;
    i->second[peerAddress] = newInfo;
  }
  else
  {
    j->second.referenceTbtt = receivingTime;
    j->second.beaconInterval = beaconInterval;
  }
}

void
PeerManagerProtocol::UpdatePeerBeaconTiming(
    uint32_t interface,
    bool meshBeacon,
    IeBeaconTiming timingElement,
    Mac48Address peerAddress,
    Time receivingTime,
    Time beaconInterval)
{
  FillBeaconInfo(interface, peerAddress, receivingTime, beaconInterval);
   if(!meshBeacon)
     return;
   //BCA:
   PeerManagerPluginMap::iterator plugin = m_plugins.find (interface);
   NS_ASSERT(plugin != m_plugins.end ());
   plugin->second->SetBeaconShift(GetNextBeaconShift(interface));
   //PM STATE Machine
   Ptr<PeerLink> peerLink = FindPeerLink(interface, peerAddress);
   if(peerLink !=0)  
   {
     peerLink->SetBeaconTimingElement (timingElement);
     peerLink->SetBeaconInformation (receivingTime, beaconInterval);
   }
   else
   {
     peerLink = InitiateLink (interface, peerAddress, receivingTime, beaconInterval);
     peerLink->SetBeaconTimingElement (timingElement);
     if (ShouldSendOpen (interface, peerAddress))
       peerLink->MLMEActivePeerLinkOpen ();
   }
}

void
PeerManagerProtocol::ReceivePeerLinkFrame (
    uint32_t interface,
    Mac48Address peerAddress,
    uint16_t aid,
    IePeerManagement peerManagementElement,
    IeConfiguration meshConfig
      )
{
  Ptr<PeerLink> peerLink = FindPeerLink(interface, peerAddress);
  if (peerManagementElement.SubtypeIsOpen ())
  {
    dot11sReasonCode reasonCode;
    bool reject = ! (ShouldAcceptOpen (interface, peerAddress,reasonCode));
    if (peerLink == 0)
      peerLink = InitiateLink (interface, peerAddress, Simulator::Now (), Seconds(1.0));
    if(!reject)
    {
      peerLink->MLMEPassivePeerLinkOpen ();
      peerLink->OpenAccept (peerManagementElement.GetLocalLinkId(), meshConfig);
    }
    else
      peerLink->OpenReject (peerManagementElement.GetLocalLinkId(), meshConfig, reasonCode);
  }
  if (peerLink == 0)
    return;
  if (peerManagementElement.SubtypeIsConfirm ())
    peerLink->ConfirmAccept (peerManagementElement.GetLocalLinkId(), peerManagementElement.GetPeerLinkId(), aid, meshConfig);
  if (peerManagementElement.SubtypeIsClose ())
    peerLink->Close (
        peerManagementElement.GetLocalLinkId(),
        peerManagementElement.GetPeerLinkId(),
        peerManagementElement.GetReasonCode()
        );
}

void
PeerManagerProtocol::ConfigurationMismatch (uint32_t interface, Mac48Address peerAddress)
{
  Ptr<PeerLink> peerLink = FindPeerLink(interface, peerAddress);
  if(peerLink != 0)
      peerLink->MLMECancelPeerLink (REASON11S_MESH_CONFIGURATION_POLICY_VIOLATION);
}

Ptr<PeerLink>
PeerManagerProtocol::InitiateLink (
  uint32_t interface,
  Mac48Address peerAddress,
  Time lastBeacon,
  Time beaconInterval)
{
  Ptr<PeerLink> new_link = CreateObject<PeerLink> ();
  if (m_lastLocalLinkId == 0xff)
    m_lastLocalLinkId = 0;
  //find a beacon entry
  BeaconInfoMap::iterator beaconsOnInterface = m_neighbourBeacons.find (interface);
  if(beaconsOnInterface == m_neighbourBeacons.end())
    FillBeaconInfo(interface, peerAddress, lastBeacon, beaconInterval);
  BeaconsOnInterface::iterator beacon = beaconsOnInterface->second.find (peerAddress);
  if(beacon == beaconsOnInterface->second.end ())
    FillBeaconInfo(interface, peerAddress, lastBeacon, beaconInterval);
  //find a peer link  - it must not exist
  NS_ASSERT(FindPeerLink(interface, peerAddress) == 0);
  /// Plugin must exust
  PeerManagerPluginMap::iterator plugin = m_plugins.find (interface);
  NS_ASSERT(plugin != m_plugins.end ());
  PeerLinksMap::iterator iface = m_peerLinks.find (interface);
  NS_ASSERT (iface != m_peerLinks.end());
  new_link->SetLocalAid (beacon->second.aid);
  new_link->SetInterface (interface);
  new_link->SetLocalLinkId (m_lastLocalLinkId++);
  new_link->SetPeerAddress (peerAddress);
  new_link->SetBeaconInformation (lastBeacon, beaconInterval);
  new_link->SetMacPlugin (plugin->second);
  new_link->MLMESetSignalStatusCallback (MakeCallback(&PeerManagerProtocol::PeerLinkStatus, this));
  iface->second.push_back (new_link);  
  return new_link;
}
Ptr<PeerLink>
PeerManagerProtocol::FindPeerLink(uint32_t interface, Mac48Address peerAddress)
{
  PeerLinksMap::iterator iface = m_peerLinks.find (interface);
  NS_ASSERT (iface != m_peerLinks.end());
  for (PeerLinksOnInterface::iterator i = iface->second.begin (); i != iface->second.end(); i++)  
    if ((*i)->GetPeerAddress () == peerAddress)
      return (*i);
  return 0;
}
void
PeerManagerProtocol::PeerCleanup ()
{
  //Cleanup a peer link descriptors:
  for (
    PeerLinksMap::iterator j = m_peerLinks.begin ();
    j != m_peerLinks.end ();
    j++)
    {
      std::vector<unsigned int> to_erase;
      for (unsigned int i = 0; i< j->second.size (); i++)
        if (j->second[i]->LinkIsIdle ())
          {
            j->second[i]->ClearTimingElement ();
            j->second[i] = 0;
            to_erase.push_back (i);
          }
      if (to_erase.size () == 0)
        return;
      for (unsigned int i = to_erase.size ()-1 ; i >= 0; i--)
        j->second.erase (j->second.begin() + to_erase[i]);
      to_erase.clear ();
    }
  // cleanup neighbour beacons:
  //NS_ASSERT(false);
  m_cleanupEvent = Simulator::Schedule (m_peerLinkCleanupPeriod, &PeerManagerProtocol::PeerCleanup, this);
}
bool
PeerManagerProtocol::IsActiveLink (uint32_t interface, Mac48Address peerAddress)
{
  Ptr<PeerLink> peerLink = FindPeerLink(interface, peerAddress);
    if(peerLink != 0)
      return (peerLink->LinkIsEstab ());
  return false;
}
bool
PeerManagerProtocol::ShouldSendOpen (uint32_t interface, Mac48Address peerAddress)
{
  if (m_numberOfActivePeers > m_maxNumberOfPeerLinks)
    return false;
  return true;
}
bool
PeerManagerProtocol::ShouldAcceptOpen (uint32_t interface, Mac48Address peerAddress, dot11sReasonCode & reasonCode)
{
  if (m_numberOfActivePeers > m_maxNumberOfPeerLinks)
    {
      reasonCode = REASON11S_MESH_MAX_PEERS;
      return false;
    }
  return true;
}
Time
PeerManagerProtocol::GetNextBeaconShift (uint32_t interface)
{
  //REMINDER:: in timing element  1) last beacon reception time is measured in units of 256 microseconds
  //                              2) beacon interval is mesured in units of 1024 microseconds
  //                              3) hereafter TU = 1024 microseconds
  //Im my MAC everything is stored in MicroSeconds

  uint32_t myNextTbttInTimeUnits = 0;
  uint32_t futureBeaconInTimeUnits = 0;
  //Going through all my timing elements and detecting future beacon collisions
  PeerLinksMap::iterator iface = m_peerLinks.find (interface);
  NS_ASSERT (iface != m_peerLinks.end());
  PeerManagerPluginMap::iterator plugin = m_plugins.find (interface);
  NS_ASSERT(plugin != m_plugins.end());
  std::pair<Time, Time> myBeacon = plugin->second->GetBeaconInfo ();
  for (PeerLinksOnInterface::iterator i = iface->second.begin (); i != iface->second.end (); i++)
    {
      IeBeaconTiming::NeighboursTimingUnitsList neighbours;
      neighbours = (*i)->GetBeaconTimingElement ().GetNeighboursTimingElementsList();
      //first let's form the list of all kown Tbtts
      for (IeBeaconTiming::NeighboursTimingUnitsList::const_iterator j = neighbours.begin (); j != neighbours.end(); j++)
        {
          uint16_t beaconIntervalTimeUnits;
          beaconIntervalTimeUnits = (*j)->GetBeaconInterval ();
          //The last beacon time in timing elememt in Time Units
          uint32_t lastBeaconInTimeUnits;
          lastBeaconInTimeUnits = (*j)->GetLastBeacon ()/4;
          //The time of my next beacon sending in Time Units
          myNextTbttInTimeUnits = myBeacon.first.GetMicroSeconds ()/1024;
          //My beacon interval in Time Units
          uint32_t myBeaconIntervalInTimeUnits;
          myBeaconIntervalInTimeUnits = myBeacon.second.GetMicroSeconds ()/1024;
          //The time the beacon of other station will be sent
          //we need the time just after my next Tbtt (or equal to my Tbtt)
          futureBeaconInTimeUnits = lastBeaconInTimeUnits + beaconIntervalTimeUnits;
          //We apply MBAC only if beacon Intervals are equal
          if (beaconIntervalTimeUnits == myBeaconIntervalInTimeUnits)
            {
              //We know when the neighbor STA transmitted it's beacon
              //Now we need to know when it's going to send it's beacon in the future
              //So let's use the valuse of it's beacon interval
              while (myNextTbttInTimeUnits >= futureBeaconInTimeUnits)
                futureBeaconInTimeUnits = futureBeaconInTimeUnits + beaconIntervalTimeUnits;
              //If we found that my Tbtt coincide with another STA's Tbtt
              //break all cylce and return time shift for my next Tbtt
              if (myNextTbttInTimeUnits == futureBeaconInTimeUnits)
                break;
            }
        }
      if (myNextTbttInTimeUnits == futureBeaconInTimeUnits)
        break;
    }
  //Tbtts coincide, so let's calculate the shift
  if (myNextTbttInTimeUnits == futureBeaconInTimeUnits)
    {
      NS_LOG_DEBUG ("MBCA: Future beacon collision is detected, applying avoidance mechanism");
      UniformVariable randomSign (-1, 1);
      int coefficientSign = -1;
      if (randomSign.GetValue () >= 0)
        coefficientSign = 1;
      UniformVariable randomShift (1, 15);
      //So, the shift is a random integer variable uniformly distributed in [-15;-1] U [1;15]
      int beaconShift = randomShift.GetInteger (1,15) * coefficientSign;
      NS_LOG_DEBUG ("Shift value = " << beaconShift << " beacon TUs");
      //We need the result not in Time Units, but in microseconds
      return MicroSeconds (beaconShift * 1024);
    }
  //No collision detected, hence no shift is needed
  else
    return MicroSeconds (0);
}
void
PeerManagerProtocol::PeerLinkStatus (uint32_t interface, Mac48Address peerAddress, bool status)
{
   PeerManagerPluginMap::iterator plugin = m_plugins.find (interface);
   NS_ASSERT(plugin != m_plugins.end());
   NS_LOG_UNCOND("LINK between me:"<<plugin->second->GetAddress() <<" and peer:"<<peerAddress<<", at interface "<<interface);
   if(status)
   {
     NS_LOG_UNCOND("Established");
     m_numberOfActivePeers ++;
   }
   else
   {
     NS_LOG_UNCOND("Closed");
     m_numberOfActivePeers --;
   }
}
  
} // namespace dot11s
} //namespace ns3

