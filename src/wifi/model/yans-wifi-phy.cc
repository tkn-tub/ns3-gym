/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Ghada Badawy <gbadawy@gmail.com>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "yans-wifi-phy.h"
#include "yans-wifi-channel.h"
#include "ns3/log.h"
#include "wifi-utils.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("YansWifiPhy");

NS_OBJECT_ENSURE_REGISTERED (YansWifiPhy);

TypeId
YansWifiPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::YansWifiPhy")
    .SetParent<WifiPhy> ()
    .SetGroupName ("Wifi")
    .AddConstructor<YansWifiPhy> ()
  ;
  return tid;
}

YansWifiPhy::YansWifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

YansWifiPhy::~YansWifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

void
YansWifiPhy::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_channel = 0;
}

Ptr<Channel>
YansWifiPhy::GetChannel (void) const
{
  return m_channel;
}

void
YansWifiPhy::SetChannel (const Ptr<YansWifiChannel> channel)
{
  m_channel = channel;
  m_channel->Add (this);
}

void
YansWifiPhy::StartTx (Ptr<Packet> packet, WifiTxVector txVector, Time txDuration)
{
  NS_LOG_DEBUG ("Start transmission: signal power before antenna gain=" << GetPowerDbm (txVector.GetTxPowerLevel ()) << "dBm");
  m_channel->Send (this, packet, GetPowerDbm (txVector.GetTxPowerLevel ()) + GetTxGain (), txDuration);
}

} //namespace ns3
