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
#include "ns3/peer-link.h"
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
                      //peerLinkCleanupTimeout. This constant is not specified in Draft 2.0
                      .AddAttribute ("PeerLinkCleanupPeriod",
                                     "PeerLinkCleanupPeriod",
                                     TimeValue (MilliSeconds (80)),
                                     MakeTimeAccessor (&PeerManagerProtocol::m_peerLinkCleanupPeriod),
                                     MakeTimeChecker ()
                                    )
                      //MaxBeaconLost. This constant is not specified in Draft 2.0
#if 0
                      .AddAttribute ("MaxBeaconLost", "Max Beacon Lost",
                                     UintegerValue (3),
                                     MakeUintegerAccessor (&PeerManagerProtocol::m_maxBeaconLoss),
                                     MakeUintegerChecker<uint8_t> ()
                                    )
#endif
                      //maximum number of peer links.
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
  m_cleanupEvent.Cancel ();
  //TODO: delete a list of descriptors
  for (
    PeerLinksMap::iterator j = m_peerLinks.begin ();
    j != m_peerLinks.end ();
    j++)
    {
      int to_delete = 0;
      for (PeerLinksOnInterface::iterator i = j->second.begin (); i != j->second.end(); i++)
        {
          to_delete ++;
          (*i)->ClearTimingElement ();
          (*i) = 0;
        }
      for (int i = 0; i < to_delete; i ++)
        j->second.pop_back ();
      j->second.clear ();
    }
  m_peerLinks.clear ();
}

bool
PeerManagerProtocol::AttachPorts(std::vector<Ptr<WifiNetDevice> > interfaces)
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

Ptr<IeDot11sBeaconTiming>
PeerManagerProtocol::SendBeacon(uint32_t interface, Time currentTbtt, Time beaconInterval)
{
  Ptr<IeDot11sBeaconTiming> retval = Create<IeDot11sBeaconTiming> ();
  BeaconInfoMap::iterator i = m_neighbourBeacons.find(interface);
  if(i == m_neighbourBeacons.end())
    return retval;
  for(BeaconsOnInterface::iterator j = i->second.begin(); j != i->second.end(); j++)
  {
    //check beacon loss and make a timing element
    if(
        (j->second.referenceTbtt.GetMicroSeconds() +
        (j->second.beaconInterval.GetMicroSeconds()* m_maxBeaconLoss))
          <
        Simulator::Now().GetMicroSeconds()
        )
      continue;
    retval->AddNeighboursTimingElementUnit(j->second.aid, j->second.referenceTbtt, j->second.beaconInterval);
  }
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
PeerManagerProtocol::ReceiveBeacon(
    uint32_t interface,
    bool meshBeacon,
    IeDot11sBeaconTiming timingElement,
    Mac48Address peerAddress,
    Time receivingTime,
    Time beaconInterval)
{
  FillBeaconInfo(interface, peerAddress, receivingTime, beaconInterval);
   if(!meshBeacon)
     return;
  //PM STATE Machine
  {
   PeerLinksMap::iterator iface = m_peerLinks.find (interface);
   NS_ASSERT (iface != m_peerLinks.end());
   for (PeerLinksOnInterface::iterator i = iface->second.begin (); i != iface->second.end(); i++){
      if ((*i)->GetPeerAddress () == peerAddress)
        {
          (*i)->SetBeaconTimingElement (timingElement);
          (*i)->SetBeaconInformation (receivingTime, beaconInterval);
          return;
        }
    }
   PeerManagerPluginMap::iterator plugin = m_plugins.find (interface);
   NS_ASSERT(plugin != m_plugins.end());
   Ptr<PeerLink> new_link = InitiateLink (interface, peerAddress, receivingTime, beaconInterval);
   new_link->SetBeaconTimingElement (timingElement);
   if (ShouldSendOpen (interface, peerAddress))
     new_link->MLMEActivePeerLinkOpen ();
  }
}

void
PeerManagerProtocol::ReceivePeerLinkFrame (
    uint32_t interface,
    Mac48Address peerAddress,
    uint16_t aid,
    IeDot11sPeerManagement peerManagementElement,
    IeDot11sConfiguration meshConfig
      )
{
  if (peerManagementElement.SubtypeIsOpen ())
  {
    dot11sReasonCode reasonCode;
    bool reject = ! (ShouldAcceptOpen (interface, peerAddress,reasonCode));
    PeerLinksMap::iterator iface = m_peerLinks.find (interface);
    NS_ASSERT (iface != m_peerLinks.end());
    for (PeerLinksOnInterface::iterator i = iface->second.begin (); i != iface->second.end(); i++)
      if ((*i)->GetPeerAddress () == peerAddress)
      {
        if(!reject)
        {
          //Drop from INIT state:
          //(*i)->MLMEPassivePeerLinkOpen ();
          (*i)->OpenAccept (peerManagementElement.GetLocalLinkId(), meshConfig);
        }
        else
          (*i)->OpenReject (peerManagementElement.GetLocalLinkId(), meshConfig, reasonCode);
        return;
      }
    Ptr<PeerLink> new_link = InitiateLink (
        interface,
        peerAddress,
        Simulator::Now (),
        Seconds(1.0)
      );
    if(!reject)
    {
      //Drop from INIT state:
      new_link->MLMEPassivePeerLinkOpen ();
      new_link->OpenAccept (peerManagementElement.GetLocalLinkId(), meshConfig);
    }
    else
      new_link->OpenReject (peerManagementElement.GetLocalLinkId(), meshConfig, reasonCode);
    return;
  }
  PeerLinksMap::iterator iface = m_peerLinks.find (interface);
  NS_ASSERT (iface != m_peerLinks.end());
  for (PeerLinksOnInterface::iterator i = iface->second.begin (); i != iface->second.end(); i++)
    
    if ((*i)->GetPeerAddress () == peerAddress)
    {
      if(peerManagementElement.SubtypeIsConfirm ())
      {
        (*i)->ConfirmAccept (peerManagementElement.GetLocalLinkId(), peerManagementElement.GetPeerLinkId(), aid, meshConfig);
        return;
      }
      if(peerManagementElement.SubtypeIsClose ())
      {
        (*i)->Close (
            peerManagementElement.GetLocalLinkId(),
            peerManagementElement.GetPeerLinkId(),
            peerManagementElement.GetReasonCode());
        return;
      }
    }
  return;
  NS_ASSERT (false);
}

void
PeerManagerProtocol::ConfigurationMismatch (
  uint32_t interface,
  Mac48Address peerAddress
)
{
  PeerLinksMap::iterator iface = m_peerLinks.find (interface);
   
  NS_ASSERT (iface != m_peerLinks.end());
   
  for (PeerLinksOnInterface::iterator i = iface->second.begin (); i != iface->second.end(); i++)
    if ((*i)->GetPeerAddress () == peerAddress)
    {
      (*i)->MLMECancelPeerLink (REASON11S_MESH_CONFIGURATION_POLICY_VIOLATION);
      return;
    }
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
  //find a peer link 
  PeerLinksMap::iterator iface = m_peerLinks.find (interface);
  NS_ASSERT (iface != m_peerLinks.end());
  for (PeerLinksOnInterface::iterator i = iface->second.begin (); i != iface->second.end(); i++)
    if ((*i)->GetPeerAddress () == peerAddress)
    {
      NS_ASSERT(false);
      return (*i);
    }

  PeerManagerPluginMap::iterator plugin = m_plugins.find (interface);
  NS_ASSERT(plugin != m_plugins.end ());

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
  m_cleanupEvent = Simulator::Schedule (m_peerLinkCleanupPeriod, &PeerManagerProtocol::PeerCleanup, this);
}
bool
PeerManagerProtocol::IsActiveLink (uint32_t interface, Mac48Address peerAddress)
{
  PeerLinksMap::iterator iface = m_peerLinks.find (interface);
  NS_ASSERT (iface != m_peerLinks.end());
  for (PeerLinksOnInterface::iterator i = iface->second.begin (); i != iface->second.end(); i++)
    if ((*i)->GetPeerAddress () == peerAddress)
      return ((*i)->LinkIsEstab ());
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
#if 0
Time
PeerManagerProtocol::GetNextBeaconShift (
  Mac48Address interfaceAddress,
  Time myNextTBTT
)
{
  //REMINDER:: in timing element  1) last beacon reception time is measured in units of 256 microseconds
  //                              2) beacon interval is mesured in units of 1024 microseconds
  //                              3) hereafter TU = 1024 microseconds
  //Im my MAC everything is stored in MicroSeconds

  uint32_t myNextTBTTInTimeUnits = 0;
  uint32_t futureBeaconInTimeUnits = 0;
  //Going through all my timing elements and detecting future beacon collisions
  PeerLinksMap::iterator interface = m_peerLinks.find (interfaceAddress);
  NS_ASSERT (interface != m_peerLinks.end());
  BeaconInfoMap::iterator myBeacon = m_myBeaconInfo.find (interfaceAddress);
  NS_ASSERT (myBeacon != m_myBeaconInfo.end());
  for (PeerLinksOnInterface::iterator i = interface->second.begin (); i != interface->second.end(); i++)
    {
      IeDot11sBeaconTiming::NeighboursTimingUnitsList neighbours;
      neighbours = (*i)->GetBeaconTimingElement ().GetNeighboursTimingElementsList();
      //first let's form the list of all kown TBTTs
      for (IeDot11sBeaconTiming::NeighboursTimingUnitsList::const_iterator j = neighbours.begin (); j != neighbours.end(); j++)
        {
          uint16_t beaconIntervalTimeUnits;
          beaconIntervalTimeUnits = (*j)->GetBeaconInterval ();

          //The last beacon time in timing elememt in Time Units
          uint32_t lastBeaconInTimeUnits;
          lastBeaconInTimeUnits = (*j)->GetLastBeacon ()/4;

          //The time of my next beacon sending in Time Units
          myNextTBTTInTimeUnits = myNextTBTT.GetMicroSeconds ()/1024;

          //My beacon interval in Time Units
          uint32_t myBeaconIntervalInTimeUnits;
          myBeaconIntervalInTimeUnits = myBeacon->second.beaconInterval.GetMicroSeconds ()/1024;

          //The time the beacon of other station will be sent
          //we need the time just after my next TBTT (or equal to my TBTT)
          futureBeaconInTimeUnits = lastBeaconInTimeUnits + beaconIntervalTimeUnits;

          //We apply MBAC only if beacon Intervals are equal
          if (beaconIntervalTimeUnits == myBeaconIntervalInTimeUnits)
            {
              //We know when the neighbor STA transmitted it's beacon
              //Now we need to know when it's going to send it's beacon in the future
              //So let's use the valuse of it's beacon interval
              while (myNextTBTTInTimeUnits >= futureBeaconInTimeUnits)
                futureBeaconInTimeUnits = futureBeaconInTimeUnits + beaconIntervalTimeUnits;
              //If we found that my TBTT coincide with another STA's TBTT
              //break all cylce and return time shift for my next TBTT
              if (myNextTBTTInTimeUnits == futureBeaconInTimeUnits)
                break;
            }

        }
      if (myNextTBTTInTimeUnits == futureBeaconInTimeUnits)
        break;
    }

  //TBTTs coincide, so let's calculate the shift
  if (myNextTBTTInTimeUnits == futureBeaconInTimeUnits)
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
  //No collision detecterf, hence no shift is needed
  else
    return MicroSeconds (0);
}

#endif
void
PeerManagerProtocol::PeerLinkStatus (uint32_t interface, Mac48Address peerAddress, bool status)
{
   PeerManagerPluginMap::iterator plugin = m_plugins.find (interface);
   NS_LOG_UNCOND(interface);
   NS_ASSERT(plugin != m_plugins.end());
   NS_LOG_UNCOND("LINK between me:"<<plugin->second->GetAddress() <<" and peer:"<<peerAddress<<", at interface "<<interface);
   if(status)
     NS_LOG_UNCOND("Established");
   else
     NS_LOG_UNCOND("Closed");
}
  
} // namespace dot11s
} //namespace ns3

