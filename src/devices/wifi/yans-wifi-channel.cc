/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
 * Author: Mathieu Lacage, <mathieu.lacage@sophia.inria.fr>
 */
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/object-factory.h"
#include "yans-wifi-channel.h"
#include "yans-wifi-phy.h"
#include "wifi-net-device.h"
#include "propagation-loss-model.h"
#include "propagation-delay-model.h"

NS_LOG_COMPONENT_DEFINE ("YansWifiChannel");

namespace ns3 {

TypeId 
YansWifiChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::YansWifiChannel")
    .SetParent<WifiChannel> ()
    .AddConstructor<YansWifiChannel> ()
    .AddAttribute ("PropagationLossModel", "A pointer to the propagation loss model attached to this channel.",
                   PointerValue (),
                   MakePointerAccessor (&YansWifiChannel::m_loss),
                   MakePointerChecker<PropagationLossModel> ())
    .AddAttribute ("PropagationDelayModel", "A pointer to the propagation delay model attached to this channel.",
                   PointerValue (),
                   MakePointerAccessor (&YansWifiChannel::m_delay),
                   MakePointerChecker<PropagationDelayModel> ())
    ;
  return tid;
}

YansWifiChannel::YansWifiChannel ()
{}
YansWifiChannel::~YansWifiChannel ()
{
  m_deviceList.clear ();
}

void 
YansWifiChannel::SetPropagationLossModel (Ptr<PropagationLossModel> loss)
{
  m_loss = loss;
}
void 
YansWifiChannel::SetPropagationDelayModel (Ptr<PropagationDelayModel> delay)
{
  m_delay = delay;
}

void 
YansWifiChannel::Send (Ptr<YansWifiPhy> sender, Ptr<const Packet> packet, double txPowerDbm,
                       WifiMode wifiMode, WifiPreamble preamble) const
{
  Ptr<MobilityModel> senderMobility = 0;
  for (DeviceList::const_iterator i = m_deviceList.begin (); i != m_deviceList.end (); i++)
    {
      if (sender == i->phy)
        {
          senderMobility = i->mobility->GetObject<MobilityModel> ();
          break;
        }
    }
  NS_ASSERT (senderMobility != 0);
  uint32_t j = 0;
  for (DeviceList::const_iterator i = m_deviceList.begin (); i != m_deviceList.end (); i++)
    {
      if (sender != i->phy)
        {
          Ptr<MobilityModel> receiverMobility = i->mobility->GetObject<MobilityModel> ();
          Time delay = m_delay->GetDelay (senderMobility, receiverMobility);
          double rxPowerDbm = txPowerDbm + m_loss->GetLoss (senderMobility, receiverMobility);
          NS_LOG_DEBUG ("propagation: txPower="<<txPowerDbm<<"dbm, rxPower="<<rxPowerDbm<<"dbm, "<<
                        "distance="<<senderMobility->GetDistanceFrom (receiverMobility)<<"m, delay="<<delay);
          Ptr<Packet> copy = packet->Copy ();
          Simulator::Schedule (delay, &YansWifiChannel::Receive, this, 
                               j, copy, rxPowerDbm, wifiMode, preamble);
        }
      j++;
    }
}

void
YansWifiChannel::Receive (uint32_t i, Ptr<Packet> packet, double rxPowerDbm,
                      WifiMode txMode, WifiPreamble preamble) const
{
  m_deviceList[i].phy->StartReceivePacket (packet, rxPowerDbm, txMode, preamble);
}

uint32_t 
YansWifiChannel::GetNDevices (void) const
{
  return m_deviceList.size ();
}
Ptr<NetDevice> 
YansWifiChannel::GetDevice (uint32_t i) const
{
  return m_deviceList[i].device;
}

Ptr<WifiPhy> 
YansWifiChannel::CreatePhy (Ptr<WifiNetDevice> device,
                        Ptr<Object> mobility,
                        UnsafeAttributeList list)
{
  ObjectFactory factory;
  factory.SetTypeId ("ns3::YansWifiPhy");
  factory.Set (list.GetSafe ("ns3::YansWifiPhy"));
  Ptr<YansWifiPhy> phy = factory.Create<YansWifiPhy> ();
  phy->SetChannel (this);
  struct Item item;
  item.device = device;
  item.phy = phy;
  item.mobility = mobility;
  m_deviceList.push_back (item);
  return phy;
}

} // namespace ns3
