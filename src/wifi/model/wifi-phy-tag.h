/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef WIFI_PHY_TAG_H
#define WIFI_PHY_TAG_H

#include <ns3/tag.h>
#include "wifi-phy.h"

namespace ns3 {

/**
 * \ingroup wifi
 *
 * Tag for WifiTxVector and WifiPreamble information to be embedded in outgoing
 * transmissions as a PacketTag
 */
class WifiPhyTag : public Tag
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  TypeId GetInstanceTypeId (void) const;

  /**
   * Constructor
   */
  WifiPhyTag ();
  /**
   * Constructor
   * \param txVector the WifiTxVector
   * \param mpdutype the mpduType
   */
  WifiPhyTag (WifiTxVector txVector, MpduType mpdutype);
  /**
   * Getter for WifiTxVector parameter
   * \return the WifiTxVector
   */
  WifiTxVector GetWifiTxVector (void) const;
  /**
   * Getter for mpduType parameter
   * \return mpduType the mpduType
   */
  MpduType GetMpduType (void) const;

  // From class Tag
  uint32_t GetSerializedSize (void) const;
  void Serialize (TagBuffer i) const;
  void Deserialize (TagBuffer i);
  void Print (std::ostream &os) const;


private:
  WifiTxVector m_wifiTxVector; ///< wifi transmit vector
  MpduType m_mpduType; ///< MPDU type
};

} // namespace ns3

#endif /* WIFI_PHY_TAG_H */
