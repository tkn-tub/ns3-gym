/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 SEBASTIEN DERONNE
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

#ifndef VHT_WIFI_MAC_HELPER_H
#define VHT_WIFI_MAC_HELPER_H

#include "ns3/string.h"
#include "wifi-helper.h"
#include "ns3/qos-utils.h"
#include "qos-wifi-mac-helper.h"
#include <map>

/**
 * (Deprecated) ns3::VhtWifiMacHelper declaration.
 */

namespace ns3 {

/**
 * \brief create VHT-enabled MAC layers for a ns3::WifiNetDevice.
 *
 * This class can create MACs of type ns3::ApWifiMac, ns3::StaWifiMac,
 * and, ns3::AdhocWifiMac, with QosSupported, HTSupported and VHTSupported attributes set to True.
 */
class VhtWifiMacHelper : public QosWifiMacHelper
{
public:
  /**
   * Create a VhtWifiMacHelper that is used to make life easier when working
   * with Wifi devices using a QOS MAC layer.
   */
  VhtWifiMacHelper ();

  /**
   * \internal
   * Destroy a VhtWifiMacHelper
   */
  virtual ~VhtWifiMacHelper ();

  /**
   * Create a mac helper in a default working state.
   */
  static VhtWifiMacHelper Default (void);

  /**
   * Converts a VHT MCS value into a DataRate value
   */
  static StringValue DataRateForMcs (int mcs);
};

} //namespace ns3

#endif /* VHT_WIFI_MAC_HELPER_H */
