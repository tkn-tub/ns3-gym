/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/dca-txop.h"

namespace ns3 {

NqosWifiMacHelper::NqosWifiMacHelper ()
{
  m_queue.SetTypeId ("ns3::DcaTxop");
}

NqosWifiMacHelper::~NqosWifiMacHelper ()
{}

NqosWifiMacHelper
NqosWifiMacHelper::Default (void)
{
  NqosWifiMacHelper helper;
  helper.SetType ("ns3::AdhocWifiMac");
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
                            std::string n7, const AttributeValue &v7)
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
}

void
NqosWifiMacHelper::SetDcaParameters (std::string n0, const AttributeValue &v0,
                                     std::string n1, const AttributeValue &v1,
                                     std::string n2, const AttributeValue &v2,
                                     std::string n3, const AttributeValue &v3)
{
  m_queue.Set (n0, v0);
  m_queue.Set (n1, v1);
  m_queue.Set (n2, v2);
  m_queue.Set (n3, v3);
}

Ptr<WifiMac>
NqosWifiMacHelper::Create (void) const
{
  Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();
  Ptr<DcaTxop> queue = m_queue.Create<DcaTxop> ();
  mac->SetAttribute ("DcaTxop", PointerValue (queue));
  return mac;
}

} //namespace ns3
