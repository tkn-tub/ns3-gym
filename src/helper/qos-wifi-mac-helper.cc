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
#include "qos-wifi-mac-helper.h"
#include "ns3/msdu-aggregator.h"
#include "ns3/wifi-mac.h"
#include "ns3/edca-txop-n.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"

namespace ns3 {

QosWifiMacHelper::QosWifiMacHelper ()
{
  m_aggregators.insert (std::make_pair (AC_VO, ObjectFactory ()));
  m_aggregators.insert (std::make_pair (AC_VI, ObjectFactory ()));
  m_aggregators.insert (std::make_pair (AC_BE, ObjectFactory ()));
  m_aggregators.insert (std::make_pair (AC_BK, ObjectFactory ()));

  m_queues.insert (std::make_pair (AC_VO, ObjectFactory ()));
  m_queues.insert (std::make_pair (AC_VI, ObjectFactory ()));
  m_queues.insert (std::make_pair (AC_BE, ObjectFactory ()));
  m_queues.insert (std::make_pair (AC_BK, ObjectFactory ()));

  m_queues[AC_VO].SetTypeId ("ns3::EdcaTxopN");
  m_queues[AC_VI].SetTypeId ("ns3::EdcaTxopN");
  m_queues[AC_BE].SetTypeId ("ns3::EdcaTxopN");
  m_queues[AC_BK].SetTypeId ("ns3::EdcaTxopN");
}

QosWifiMacHelper::~QosWifiMacHelper ()
{}

QosWifiMacHelper
QosWifiMacHelper::Default (void)
{
  QosWifiMacHelper helper;
  helper.SetType ("ns3::QstaWifiMac");
  /* For more details about this default parameters see IEE802.11 section 7.3.2.29 */
  helper.SetEdcaParametersForAc (AC_VO,"MinCw", UintegerValue (3),
                                       "MaxCw", UintegerValue (7),
                                       "Aifsn", UintegerValue (2));
  helper.SetEdcaParametersForAc (AC_VI,"MinCw", UintegerValue (7),
                                       "MaxCw", UintegerValue (15),
                                       "Aifsn", UintegerValue (2));
  helper.SetEdcaParametersForAc (AC_BE,"MinCw", UintegerValue (15),
                                       "MaxCw", UintegerValue (1023),
                                       "Aifsn", UintegerValue (3));
  helper.SetEdcaParametersForAc (AC_BK,"MinCw", UintegerValue (15),
                                       "MaxCw", UintegerValue (1023),
                                       "Aifsn", UintegerValue (7));
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
QosWifiMacHelper::SetMsduAggregatorForAc (AccessClass accessClass, std::string type,
                                          std::string n0, const AttributeValue &v0,
                                          std::string n1, const AttributeValue &v1,
                                          std::string n2, const AttributeValue &v2,
                                          std::string n3, const AttributeValue &v3)
{
  std::map<AccessClass, ObjectFactory>::iterator it;
  it = m_aggregators.find (accessClass);
  if (it != m_aggregators.end ())
    {
      it->second.SetTypeId (type);
      it->second.Set (n0, v0);
      it->second.Set (n1, v1);
      it->second.Set (n2, v2);
      it->second.Set (n3, v3);
    }
}

void
QosWifiMacHelper::SetEdcaParametersForAc (AccessClass accessClass,
                                          std::string n0, const AttributeValue &v0,
                                          std::string n1, const AttributeValue &v1,
                                          std::string n2, const AttributeValue &v2,
                                          std::string n3, const AttributeValue &v3)
{
  std::map<AccessClass, ObjectFactory>::iterator it;
  it = m_queues.find (accessClass);
  if (it != m_queues.end ())
    {
      it->second.Set (n0, v0);
      it->second.Set (n1, v1);
      it->second.Set (n2, v2);
      it->second.Set (n3, v3);
    }
}

Ptr<WifiMac>
QosWifiMacHelper::Create (void) const
{
  Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();
  
  Ptr<EdcaTxopN> edcaQueue;
  Ptr<MsduAggregator> aggregator;
  std::map<AccessClass, ObjectFactory>::const_iterator itQueue;
  std::map<AccessClass, ObjectFactory>::const_iterator itAggr;

  /* Setting for VO queue */
  itQueue = m_queues.find (AC_VO);
  itAggr = m_aggregators.find (AC_VO);

  edcaQueue = itQueue->second.Create<EdcaTxopN> ();
  if (itAggr->second.GetTypeId ().GetUid () != 0)
    {
      aggregator = itAggr->second.Create<MsduAggregator> ();
      edcaQueue->SetMsduAggregator (aggregator);
    }
  mac->SetAttribute ("VO_EdcaTxopN", PointerValue (edcaQueue));

  /* Setting for VI queue */
  itQueue = m_queues.find (AC_VI);
  itAggr = m_aggregators.find (AC_VI);

  edcaQueue = itQueue->second.Create<EdcaTxopN> ();
  if (itAggr->second.GetTypeId ().GetUid () != 0)
    {
      aggregator = itAggr->second.Create<MsduAggregator> ();
      edcaQueue->SetMsduAggregator (aggregator);
    }
  mac->SetAttribute ("VI_EdcaTxopN", PointerValue (edcaQueue));

  /* Setting for BE queue */
  itQueue = m_queues.find (AC_BE);
  itAggr = m_aggregators.find (AC_BE);

  edcaQueue = itQueue->second.Create<EdcaTxopN> ();
  if (itAggr->second.GetTypeId ().GetUid () != 0)
    {
      aggregator = itAggr->second.Create<MsduAggregator> ();
      edcaQueue->SetMsduAggregator (aggregator);
    }
  mac->SetAttribute ("BE_EdcaTxopN", PointerValue (edcaQueue));
  
  /* Setting for BK queue */
  itQueue = m_queues.find (AC_BK);
  itAggr = m_aggregators.find (AC_BK);

  edcaQueue = itQueue->second.Create<EdcaTxopN> ();
  if (itAggr->second.GetTypeId ().GetUid () != 0)
    {
      aggregator = itAggr->second.Create<MsduAggregator> ();
      edcaQueue->SetMsduAggregator (aggregator);
    }
  mac->SetAttribute ("BK_EdcaTxopN", PointerValue (edcaQueue));

  return mac;
}

} //namespace ns3
