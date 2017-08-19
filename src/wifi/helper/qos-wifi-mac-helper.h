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

#ifndef QOS_WIFI_MAC_HELPER_H
#define QOS_WIFI_MAC_HELPER_H

#include "wifi-helper.h"
#include "ns3/qos-utils.h"

#include <map>


namespace ns3 {

/**
 * \brief create QoS-enabled MAC layers for a ns3::WifiNetDevice.
 *
 * This class can create MACs of type ns3::ApWifiMac, ns3::StaWifiMac,
 * and, ns3::AdhocWifiMac, with QosSupported attribute set to True.
 *
 * \deprecated This class deprecated and replaced by ns3::WifiMacHelper
 */
class QosWifiMacHelper : public WifiMacHelper
{
public:
  /**
   * Create a QosWifiMacHelper that is used to make life easier when working
   * with Wifi devices using a QOS MAC layer.
   */
  QosWifiMacHelper ();

  /**
   * Destroy a QosWifiMacHelper
   */
  virtual ~QosWifiMacHelper ();

  /**
   * Create a mac helper in a default working state.
   * \returns a default QosWifiMacHelper
   */
  static QosWifiMacHelper Default (void);

  /**
   * Set the underlying type of the MAC and its attributes.
   *
   * \param type the type of ns3::WifiMac to create.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   * \param n8 the name of the attribute to set
   * \param v8 the value of the attribute to set
   * \param n9 the name of the attribute to set
   * \param v9 the value of the attribute to set
   * \param n10 the name of the attribute to set
   * \param v10 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested mac.
   */
  virtual void SetType (std::string type,
                        std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                        std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                        std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                        std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                        std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                        std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                        std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                        std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue (),
                        std::string n8 = "", const AttributeValue &v8 = EmptyAttributeValue (),
                        std::string n9 = "", const AttributeValue &v9 = EmptyAttributeValue (),
                        std::string n10 = "", const AttributeValue &v10 = EmptyAttributeValue ());
  /**
   * Set the class, type and attributes for the Msdu aggregator
   *
   * \param ac access category for which we are setting aggregator.
   *        Possibilities are: AC_BK, AC_BE, AC_VI, AC_VO.
   * \param type the type of ns3::MsduAggregator to create.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested aggregator.
   */
  void SetMsduAggregatorForAc (AcIndex ac, std::string type,
                               std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                               std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                               std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                               std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue ());
  /**
   * Set the class, type and attributes for the Mpdu aggregator
   *
   * \param ac access category for which we are setting aggregator.
   *        Possibilities are: AC_BK, AC_BE, AC_VI, AC_VO.
   * \param type the type of ns3::MsduAggregator to create.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested aggregator.
   */
  void SetMpduAggregatorForAc (enum AcIndex ac, std::string type,
                               std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                               std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                               std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                               std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue ());
  /**
   * This method sets value of block ack threshold for a specific access class.
   * If number of packets in the respective queue reaches this value block ack mechanism
   * is used.
   *
   * \param ac access category for which we are setting block ack threshold.
   *        Possibilities are: AC_BK, AC_BE, AC_VI, AC_VO.
   * \param threshold the threshold (number of packets)
   */
  void SetBlockAckThresholdForAc (enum AcIndex ac, uint8_t threshold);
  /**
   * Sets value of block ack inactivity timeout for a specific access class. <i>timeout</i>
   * represents number of blocks of 1024 microseconds. When this timer expires the relative
   * block ack agreement is teared down. Timer is reset in an recipient station every time
   * that a block ack request or a MPDU with ack policy BLOCK ACK is received.
   * Timer is reset in a originator station every time that a block ack frame is received.
   *
   * \param ac access category for which we are setting block ack threshold.
   *        Possibilities are: AC_BK, AC_BE, AC_VI, AC_VO.
   * \param timeout number of block of 1024 microseconds.
   */
  void SetBlockAckInactivityTimeoutForAc (enum AcIndex ac, uint16_t timeout);


protected:
  ObjectFactory m_mac; ///< MAC object


private:
  /**
   * \returns a newly-created MAC object.
   *
   * This method implements the pure virtual method defined in \ref ns3::WifiMacHelper.
   */
  virtual Ptr<WifiMac> Create (void) const;
  /**
   * Setup function
   * \param mac the wifi MAC
   * \param ac the access category
   * \param edcaAttrName the EDCA attribute name (VO_EdcaTxopN, VI_EdcaTxopN, BE_EdcaTxopN, BK_EdcaTxopN)
   */
  void Setup (Ptr<WifiMac> mac, enum AcIndex ac, std::string edcaAttrName) const;

  std::map<AcIndex, ObjectFactory> m_msduAggregators; ///< A-MSDU aggregators
  std::map<AcIndex, ObjectFactory> m_mpduAggregators; ///< A-MPDU aggregators
  /*
   * Next maps contain, for every access category, the values for
   * block ack threshold and block ack inactivity timeout.
   */
  std::map<AcIndex, uint8_t> m_bAckThresholds; ///< Block ACK thresholds
  std::map<AcIndex, uint16_t> m_bAckInactivityTimeouts; ///< Block ACK inactivity timeouts
};

} //namespace ns3

#endif /* QOS_WIFI_MAC_HELPER_H */
