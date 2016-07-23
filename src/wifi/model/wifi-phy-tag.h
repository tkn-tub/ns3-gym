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
#include <ns3/wifi-tx-vector.h>
#include <ns3/wifi-preamble.h>
#include <ns3/wifi-phy.h>

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
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Constructor
   */
  WifiPhyTag ();
  /**
   * Constructor
   * \param txVector the WifiTxVector
   * \param preamble the WifiPreamble
   * \param mpduType the mpduType
   */
  WifiPhyTag (WifiTxVector txVector, WifiPreamble preamble, enum mpduType mpdutype);
  /**
   * Getter for WifiTxVector parameter
   * \return the WifiTxVector
   */
  WifiTxVector GetWifiTxVector (void) const;
  /**
   * Getter for WifiPreamble parameter
   * \return preamble the WifiPreamble
   */
  WifiPreamble GetWifiPreamble (void) const;
  /**
   * Getter for mpduType parameter
   * \return mpduType the mpduType
   */
  enum mpduType GetMpduType (void) const;

  // From class Tag
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

private:
  WifiTxVector m_wifiTxVector;
  int32_t m_wifiPreamble;
  enum mpduType m_mpduType;
};

} // namespace ns3

#endif /* WIFI_PHY_TAG_H */
