/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Pavel Boyko <boyko@iitp.ru>
 */

#ifndef MESHWIFIBEACON_H_
#define MESHWIFIBEACON_H_

#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/mgt-headers.h"        // from wifi module
#include "ns3/wifi-mac-header.h"
#include "ns3/mesh-information-element-vector.h"

namespace ns3 {

/**
 * \brief Beacon is beacon header + list of arbitrary information elements
 *
 * It is supposed that distinct mesh protocols can use beacons to transport
 * their own information elements.
 */
class MeshWifiBeacon
{
public:
  /**
   * C-tor
   *
   * \param ssid is SSID for beacon header
   * \param rates is a set of supported rates
   * \param us beacon interval in microseconds
   */
  MeshWifiBeacon (Ssid ssid, SupportedRates rates, uint64_t us);
  /// Read standard Wifi beacon header
  MgtBeaconHeader BeaconHeader () const { return m_header; }
  /// Add information element
  void AddInformationElement (Ptr<WifiInformationElement> ie);

  /**
   * Create wifi header for beacon frame.
   *
   * \param address is sender address
   * \param mpAddress is mesh point address
   */
  WifiMacHeader CreateHeader (Mac48Address address, Mac48Address mpAddress);
  ///Returns a beacon interval of wifi beacon
  Time GetBeaconInterval () const;
  /// Create frame = { beacon header + all information elements sorted by ElementId () }
  Ptr<Packet> CreatePacket ();

private:
  /// Beacon header
  MgtBeaconHeader m_header;
  /// List of information elements added
  WifiInformationElementVector m_elements;
};

}


#endif /* MESHWIFIBEACON_H_ */
