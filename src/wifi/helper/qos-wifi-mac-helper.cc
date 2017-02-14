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

#include "qos-wifi-mac-helper.h"
#include "ns3/msdu-aggregator.h"
#include "ns3/mpdu-aggregator.h"
#include "ns3/wifi-mac.h"
#include "ns3/edca-txop-n.h"
#include "ns3/pointer.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/mac-low.h"

namespace ns3 {

QosWifiMacHelper::QosWifiMacHelper ()
{
}

QosWifiMacHelper::~QosWifiMacHelper ()
{
}

QosWifiMacHelper
QosWifiMacHelper::Default (void)
{
  QosWifiMacHelper helper;

  //We're making QoS-enabled Wi-Fi MACs here, so we set the necessary
  //attribute. I've carefully positioned this here so that someone
  //who knows what they're doing can override with explicit attributes.
  helper.SetType ("ns3::StaWifiMac",
                  "QosSupported", BooleanValue (true));

  return helper;
}

void
QosWifiMacHelper::SetType (std::string type,
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

void
QosWifiMacHelper::SetMsduAggregatorForAc (AcIndex ac, std::string type,
                                          std::string n0, const AttributeValue &v0,
                                          std::string n1, const AttributeValue &v1,
                                          std::string n2, const AttributeValue &v2,
                                          std::string n3, const AttributeValue &v3)
{
  std::map<AcIndex, ObjectFactory>::iterator it = m_msduAggregators.find (ac);
  if (it != m_msduAggregators.end ())
    {
      it->second.SetTypeId (type);
      it->second.Set (n0, v0);
      it->second.Set (n1, v1);
      it->second.Set (n2, v2);
      it->second.Set (n3, v3);
    }
  else
    {
      ObjectFactory factory;
      factory.SetTypeId (type);
      factory.Set (n0, v0);
      factory.Set (n1, v1);
      factory.Set (n2, v2);
      factory.Set (n3, v3);
      m_msduAggregators.insert (std::make_pair (ac, factory));
    }
}

void
QosWifiMacHelper::SetMpduAggregatorForAc (enum AcIndex ac, std::string type,
                                          std::string n0, const AttributeValue &v0,
                                          std::string n1, const AttributeValue &v1,
                                          std::string n2, const AttributeValue &v2,
                                          std::string n3, const AttributeValue &v3)
{
  std::map<AcIndex, ObjectFactory>::iterator it = m_mpduAggregators.find (ac);
  if (it != m_mpduAggregators.end ())
    {
      it->second.SetTypeId (type);
      it->second.Set (n0, v0);
      it->second.Set (n1, v1);
      it->second.Set (n2, v2);
      it->second.Set (n3, v3);
    }
  else
    {
      ObjectFactory factory;
      factory.SetTypeId (type);
      factory.Set (n0, v0);
      factory.Set (n1, v1);
      factory.Set (n2, v2);
      factory.Set (n3, v3);
      m_mpduAggregators.insert (std::make_pair (ac, factory));
    }
}

void
QosWifiMacHelper::SetBlockAckThresholdForAc (enum AcIndex ac, uint8_t threshold)
{
  m_bAckThresholds[ac] = threshold;
}

void
QosWifiMacHelper::SetBlockAckInactivityTimeoutForAc (enum AcIndex ac, uint16_t timeout)
{
  m_bAckInactivityTimeouts[ac] = timeout;
}

void
QosWifiMacHelper::Setup (Ptr<WifiMac> mac, enum AcIndex ac, std::string edcaAttrName) const
{
  PointerValue ptr;
  mac->GetAttribute (edcaAttrName, ptr);
  Ptr<EdcaTxopN> edca = ptr.Get<EdcaTxopN> ();

  std::map<AcIndex, ObjectFactory>::const_iterator it_msdu = m_msduAggregators.find (ac);
  if (it_msdu != m_msduAggregators.end ())
    {
      ObjectFactory factory = it_msdu->second;
      Ptr<MsduAggregator> msduAggregator = factory.Create<MsduAggregator> ();
      edca->SetMsduAggregator (msduAggregator);
    }

  std::map<AcIndex, ObjectFactory>::const_iterator it_mpdu = m_mpduAggregators.find (ac);
  if (it_mpdu != m_mpduAggregators.end ())
    {
      ObjectFactory factory = it_mpdu->second;
      Ptr<MpduAggregator> mpduAggregator = factory.Create<MpduAggregator> ();
      edca->SetMpduAggregator (mpduAggregator);
    }
    
  if (m_bAckThresholds.find (ac) != m_bAckThresholds.end ())
    {
      edca->SetBlockAckThreshold (m_bAckThresholds.find (ac)->second);
    }
    
  if (m_bAckInactivityTimeouts.find (ac) != m_bAckInactivityTimeouts.end ())
    {
      edca->SetBlockAckInactivityTimeout (m_bAckInactivityTimeouts.find (ac)->second);
    }
}

Ptr<WifiMac>
QosWifiMacHelper::Create (void) const
{
  Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();

  Setup (mac, AC_VO, "VO_EdcaTxopN");
  Setup (mac, AC_VI, "VI_EdcaTxopN");
  Setup (mac, AC_BE, "BE_EdcaTxopN");
  Setup (mac, AC_BK, "BK_EdcaTxopN");

  return mac;
}

} //namespace ns3
