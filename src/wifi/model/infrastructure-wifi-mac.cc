/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "infrastructure-wifi-mac.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("InfrastructureWifiMac");

NS_OBJECT_ENSURE_REGISTERED (InfrastructureWifiMac);


TypeId
InfrastructureWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::InfrastructureWifiMac")
    .SetParent<RegularWifiMac> ()
    .SetGroupName ("Wifi")
    .AddAttribute ("PcfSupported",
                   "This Boolean attribute is set to enable PCF support at this STA",
                   BooleanValue (false),
                   MakeBooleanAccessor (&InfrastructureWifiMac::SetPcfSupported,
                                        &InfrastructureWifiMac::GetPcfSupported),
                   MakeBooleanChecker ())
  ;
  return tid;
}

InfrastructureWifiMac::InfrastructureWifiMac ()
  : m_pcfSupported (0)
{
  NS_LOG_FUNCTION (this);
}

InfrastructureWifiMac::~InfrastructureWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
InfrastructureWifiMac::SetQosSupported (bool enable)
{
  NS_ASSERT_MSG (!(GetPcfSupported () && enable), "QoS cannot be enabled when PCF support is activated (not supported)");
  RegularWifiMac::SetQosSupported (enable);
}

void
InfrastructureWifiMac::SetPcfSupported (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  NS_ASSERT_MSG (!(GetQosSupported () && enable), "PCF cannot be enabled when QoS support is activated (not supported)");
  m_pcfSupported = enable;
  if (m_stationManager != 0)
    {
      m_stationManager->SetPcfSupported (enable);
    }
}

bool
InfrastructureWifiMac::GetPcfSupported () const
{
  return m_pcfSupported;
}

} //namespace ns3
