/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#include "wifi-helper.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-mac.h"
#include "ns3/regular-wifi-mac.h"
#include "ns3/dca-txop.h"
#include "ns3/edca-txop-n.h"
#include "ns3/minstrel-wifi-manager.h"
#include "ns3/ap-wifi-mac.h"
#include "ns3/wifi-phy.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/wifi-channel.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/mobility-model.h"
#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/names.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiHelper");

WifiPhyHelper::~WifiPhyHelper ()
{
}

WifiMacHelper::~WifiMacHelper ()
{
}

WifiHelper::~WifiHelper ()
{
}

WifiHelper::WifiHelper ()
  : m_standard (WIFI_PHY_STANDARD_80211a)
{
}

WifiHelper
WifiHelper::Default (void)
{
  WifiHelper helper;
  helper.SetRemoteStationManager ("ns3::ArfWifiManager");
  return helper;
}

void
WifiHelper::SetRemoteStationManager (std::string type,
                                     std::string n0, const AttributeValue &v0,
                                     std::string n1, const AttributeValue &v1,
                                     std::string n2, const AttributeValue &v2,
                                     std::string n3, const AttributeValue &v3,
                                     std::string n4, const AttributeValue &v4,
                                     std::string n5, const AttributeValue &v5,
                                     std::string n6, const AttributeValue &v6,
                                     std::string n7, const AttributeValue &v7)
{
  m_stationManager = ObjectFactory ();
  m_stationManager.SetTypeId (type);
  m_stationManager.Set (n0, v0);
  m_stationManager.Set (n1, v1);
  m_stationManager.Set (n2, v2);
  m_stationManager.Set (n3, v3);
  m_stationManager.Set (n4, v4);
  m_stationManager.Set (n5, v5);
  m_stationManager.Set (n6, v6);
  m_stationManager.Set (n7, v7);
}

void
WifiHelper::SetStandard (enum WifiPhyStandard standard)
{
  m_standard = standard;
}

NetDeviceContainer
WifiHelper::Install (const WifiPhyHelper &phyHelper,
                     const WifiMacHelper &macHelper, NodeContainer c) const
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
      Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
      Ptr<WifiMac> mac = macHelper.Create ();
      Ptr<WifiPhy> phy = phyHelper.Create (node, device);
      mac->SetAddress (Mac48Address::Allocate ());
      mac->ConfigureStandard (m_standard);
      phy->ConfigureStandard (m_standard);
      device->SetMac (mac);
      device->SetPhy (phy);
      device->SetRemoteStationManager (manager);
      node->AddDevice (device);
      devices.Add (device);
      NS_LOG_DEBUG ("node=" << node << ", mob=" << node->GetObject<MobilityModel> ());
    }
  return devices;
}

NetDeviceContainer
WifiHelper::Install (const WifiPhyHelper &phy,
                     const WifiMacHelper &mac, Ptr<Node> node) const
{
  return Install (phy, mac, NodeContainer (node));
}

NetDeviceContainer
WifiHelper::Install (const WifiPhyHelper &phy,
                     const WifiMacHelper &mac, std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return Install (phy, mac, NodeContainer (node));
}

void
WifiHelper::EnableLogComponents (void)
{
  LogComponentEnable ("Aarfcd", LOG_LEVEL_ALL);
  LogComponentEnable ("AdhocWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("AmrrWifiRemoteStation", LOG_LEVEL_ALL);
  LogComponentEnable ("ApWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("ArfWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("Cara", LOG_LEVEL_ALL);
  LogComponentEnable ("DcaTxop", LOG_LEVEL_ALL);
  LogComponentEnable ("DcfManager", LOG_LEVEL_ALL);
  LogComponentEnable ("DsssErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("EdcaTxopN", LOG_LEVEL_ALL);
  LogComponentEnable ("InterferenceHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("Jakes", LOG_LEVEL_ALL);
  LogComponentEnable ("MacLow", LOG_LEVEL_ALL);
  LogComponentEnable ("MacRxMiddle", LOG_LEVEL_ALL);
  LogComponentEnable ("MsduAggregator", LOG_LEVEL_ALL);
  LogComponentEnable ("MsduStandardAggregator", LOG_LEVEL_ALL);
  LogComponentEnable ("NistErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("OnoeWifiRemoteStation", LOG_LEVEL_ALL);
  LogComponentEnable ("PropagationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("RegularWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("RraaWifiManager", LOG_LEVEL_ALL);
  LogComponentEnable ("StaWifiMac", LOG_LEVEL_ALL);
  LogComponentEnable ("SupportedRates", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiPhyStateHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("WifiRemoteStationManager", LOG_LEVEL_ALL);
  LogComponentEnable ("YansErrorRateModel", LOG_LEVEL_ALL);
  LogComponentEnable ("YansWifiChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("YansWifiPhy", LOG_LEVEL_ALL);
}

int64_t
WifiHelper::AssignStreams (NetDeviceContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<NetDevice> netDevice;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      netDevice = (*i);
      Ptr<WifiNetDevice> wifi = DynamicCast<WifiNetDevice> (netDevice);
      if (wifi)
        {
          // Handle any random numbers in the PHY objects.
          currentStream += wifi->GetPhy ()->AssignStreams (currentStream);

          // Handle any random numbers in the station managers.
          Ptr<WifiRemoteStationManager> manager = wifi->GetRemoteStationManager ();
          Ptr<MinstrelWifiManager> minstrel = DynamicCast<MinstrelWifiManager> (manager);
          if (minstrel)
            {
              currentStream += minstrel->AssignStreams (currentStream);
            }

          // Handle any random numbers in the MAC objects.
          Ptr<WifiMac> mac = wifi->GetMac ();
          Ptr<RegularWifiMac> rmac = DynamicCast<RegularWifiMac> (mac);
          if (rmac)
            {
              PointerValue ptr;
              rmac->GetAttribute ("DcaTxop", ptr);
              Ptr<DcaTxop> dcaTxop = ptr.Get<DcaTxop> ();
              currentStream += dcaTxop->AssignStreams (currentStream);

              rmac->GetAttribute ("VO_EdcaTxopN", ptr);
              Ptr<EdcaTxopN> vo_edcaTxopN = ptr.Get<EdcaTxopN> ();
              currentStream += vo_edcaTxopN->AssignStreams (currentStream);

              rmac->GetAttribute ("VI_EdcaTxopN", ptr);
              Ptr<EdcaTxopN> vi_edcaTxopN = ptr.Get<EdcaTxopN> ();
              currentStream += vi_edcaTxopN->AssignStreams (currentStream);

              rmac->GetAttribute ("BE_EdcaTxopN", ptr);
              Ptr<EdcaTxopN> be_edcaTxopN = ptr.Get<EdcaTxopN> ();
              currentStream += be_edcaTxopN->AssignStreams (currentStream);

              rmac->GetAttribute ("BK_EdcaTxopN", ptr);
              Ptr<EdcaTxopN> bk_edcaTxopN = ptr.Get<EdcaTxopN> ();
              currentStream += bk_edcaTxopN->AssignStreams (currentStream);

              // if an AP, handle any beacon jitter
              Ptr<ApWifiMac> apmac = DynamicCast<ApWifiMac> (rmac);
              if (apmac)
                {
                  currentStream += apmac->AssignStreams (currentStream);
                }
            }
        }
    }
  return (currentStream - stream);
}

} // namespace ns3
