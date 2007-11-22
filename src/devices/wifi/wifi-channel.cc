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
#include "wifi-channel.h"
#include "propagation-loss-model.h"
#include "propagation-delay-model.h"

NS_LOG_COMPONENT_DEFINE ("WifiChannel");

namespace ns3 {

WifiChannel::WifiChannel ()
  : m_loss (PropagationLossModel::CreateDefault ()),
    m_delay (PropagationDelayModel::CreateDefault ())
{}
WifiChannel::~WifiChannel ()
{
  m_deviceList.clear ();
}

void 
WifiChannel::SetPropationLossModel (Ptr<PropagationLossModel> loss)
{
  m_loss = loss;
}
void 
WifiChannel::SetPropagationDelayModel (Ptr<PropagationDelayModel> delay)
{
  m_delay = delay;
}

void 
WifiChannel::Add (Ptr<NetDevice> device,  ReceiveCallback callback)
{
  m_deviceList.push_back (std::make_pair (device, callback));
}
void 
WifiChannel::Send (Ptr<NetDevice> sender, Ptr<const Packet> packet, double txPowerDbm,
                   WifiMode wifiMode, WifiPreamble preamble) const
{
  Ptr<MobilityModel> senderMobility = sender->GetNode ()->QueryInterface<MobilityModel> (MobilityModel::iid);
  uint32_t j = 0;
  for (DeviceList::const_iterator i = m_deviceList.begin (); i != m_deviceList.end (); i++)
    {
      if (sender != i->first)
        {
          Ptr<MobilityModel> receiverMobility = i->first->GetNode ()->QueryInterface<MobilityModel> (MobilityModel::iid);
          Time delay = m_delay->GetDelay (senderMobility, receiverMobility);
          double rxPowerDbm = m_loss->GetRxPower (txPowerDbm, senderMobility, receiverMobility);
          NS_LOG_DEBUG ("propagation: txPower="<<txPowerDbm<<"dbm, rxPower="<<rxPowerDbm<<"dbm, "<<
                        "distance="<<senderMobility->GetDistanceFrom (receiverMobility)<<"m, delay="<<delay);
          Ptr<Packet> copy = packet->Copy ();
          Simulator::Schedule (delay, &WifiChannel::Receive, this, 
                               j, copy, rxPowerDbm, wifiMode, preamble);
        }
      j++;
    }
}

void
WifiChannel::Receive (uint32_t i, Ptr<Packet> packet, double rxPowerDbm,
                      WifiMode txMode, WifiPreamble preamble) const
{
  m_deviceList[i].second (packet, rxPowerDbm, txMode, preamble);
}

uint32_t 
WifiChannel::GetNDevices (void) const
{
  return m_deviceList.size ();
}
Ptr<NetDevice> 
WifiChannel::DoGetDevice (uint32_t i) const
{
  return m_deviceList[i].first;
}

} // namespace ns3
