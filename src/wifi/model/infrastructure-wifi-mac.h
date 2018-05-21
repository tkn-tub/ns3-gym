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

#ifndef INFRASTRUCTURE_WIFI_MAC_H
#define INFRASTRUCTURE_WIFI_MAC_H

#include "regular-wifi-mac.h"

namespace ns3 {

/**
 * \ingroup wifi
 *
 * The Wifi MAC high model for a STA or AP in a BSS.
 */
class InfrastructureWifiMac : public RegularWifiMac
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  InfrastructureWifiMac ();
  virtual ~InfrastructureWifiMac ();

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to) = 0;
  /**
   * Enable or disable QoS support for the device.
   *
   * \param enable whether QoS is supported
   */
  void SetQosSupported (bool enable);

  /**
   * Enable or disable PCF support for the device.
   *
   * \param enable whether PCF is supported
   */
  void SetPcfSupported (bool enable);
  /**
   * Return whether the device supports PCF.
   *
   * \return true if PCF is supported, false otherwise
   */
  bool GetPcfSupported () const;


private:
  /**
   * This Boolean is set \c true iff this WifiMac support PCF
   */
  bool m_pcfSupported;
};

} //namespace ns3

#endif /* INFRASTRUCTURE_WIFI_MAC_H */
