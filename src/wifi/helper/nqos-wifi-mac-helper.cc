/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */

#include "nqos-wifi-mac-helper.h"
#include "ns3/wifi-mac.h"
#include "ns3/pointer.h"
#include "ns3/boolean.h"
#include "ns3/dca-txop.h"

namespace ns3 {

NqosWifiMacHelper::NqosWifiMacHelper ()
{
}

NqosWifiMacHelper::~NqosWifiMacHelper ()
{
}

NqosWifiMacHelper
NqosWifiMacHelper::Default (void)
{
  NqosWifiMacHelper helper;
  //We're making non QoS-enabled Wi-Fi MACs here, so we set the
  //necessary attribute. I've carefully positioned this here so that
  //someone who knows what they're doing can override with explicit attributes.
  helper.SetType ("ns3::AdhocWifiMac",
                  "QosSupported", BooleanValue (false));
  return helper;
}

void
NqosWifiMacHelper::SetType (std::string type,
                            std::string n0, const AttributeValue &v0,
                            std::string n1, const AttributeValue &v1,
                            std::string n2, const AttributeValue &v2,
                            std::string n3, const AttributeValue &v3,
                            std::string n4, const AttributeValue &v4,
                            std::string n5, const AttributeValue &v5,
                            std::string n6, const AttributeValue &v6,
                            std::string n7, const AttributeValue &v7,
                            std::string n8, const AttributeValue &v8,
                            std::string n9, const AttributeValue &v9,
                            std::string n10, const AttributeValue &v10)
{
  m_mac.SetTypeId (type);
  m_mac.Set (n0, v0);
  m_mac.Set (n1, v1);
  m_mac.Set (n2, v2);
  m_mac.Set (n3, v3);
  m_mac.Set (n4, v4);
  m_mac.Set (n5, v5);
  m_mac.Set (n6, v6);
  m_mac.Set (n7, v7);
  m_mac.Set (n8, v8);
  m_mac.Set (n9, v9);
  m_mac.Set (n10, v10);
}

Ptr<WifiMac>
NqosWifiMacHelper::Create (void) const
{
  Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();
  return mac;
}

} //namespace ns3
