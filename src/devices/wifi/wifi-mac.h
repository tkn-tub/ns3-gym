/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef WIFI_MAC_H
#define WIFI_MAC_H

#include "ns3/packet.h"
#include "ns3/mac48-address.h"

#include "wifi-phy.h"
#include "wifi-remote-station-manager.h"
#include "ssid.h"

namespace ns3 {

/**
 * \brief base class for all MAC-level wifi objects.
 *
 * This class encapsulates all the low-level MAC functionality
 * DCA, EDCA, etc) and all the high-level MAC functionality
 * (association/disassociation state machines).
 *
 */
class WifiMac : public Object
{
public:
  static TypeId GetTypeId (void);
  
  /**
   * \param slotTime the slot duration
   */
  virtual void SetSlot (Time slotTime) = 0;
  /**
   * \param sifs the sifs duration
   */
  virtual void SetSifs (Time sifs) = 0;
  /**
   * \param eifsNoDifs the duration of an EIFS minus DIFS.
   *
   * This value is used to calculate the EIFS depending
   * on AIFSN.
   */
  virtual void SetEifsNoDifs (Time eifsNoDifs) = 0;
  /**
   * \param pifs the pifs duration.
   */
  virtual void SetPifs (Time pifs) = 0;
  /**
   * \param ctsTimeout the duration of a CTS timeout.
   */
  virtual void SetCtsTimeout (Time ctsTimeout) = 0;
  /**
   * \param ackTimeout the duration of an ACK timeout.
   */
  virtual void SetAckTimeout (Time ackTimeout) = 0;
  /**
   * \param delay the max propagation delay.
   *
   * Unused for now.
   */
  void SetMaxPropagationDelay (Time delay);

  /**
   * \returns the current PIFS duration.
   */
  virtual Time GetPifs (void) const = 0;
  /**
   * \returns the current SIFS duration.
   */
  virtual Time GetSifs (void) const = 0;
  /**
   * \returns the current slot duration.
   */
  virtual Time GetSlot (void) const = 0;
  /**
   * \returns the current EIFS minus DIFS duration
   */
  virtual Time GetEifsNoDifs (void) const = 0;
  /**
   * \returns the current CTS timeout duration.
   */
  virtual Time GetCtsTimeout (void) const = 0;
  /**
   * \returns the current ACK timeout duration.
   */
  virtual Time GetAckTimeout (void) const = 0;
  /**
   * Unused for now.
   */
  Time GetMsduLifetime (void) const;
  /**
   * Unused for now.
   */
  Time GetMaxPropagationDelay (void) const;
  /**
   * \returns the maximum size of a MAC-level data payload.
   */
  uint32_t GetMaxMsduSize (void) const;

  /**
   * \returns the MAC address associated to this MAC layer.
   */
  virtual Mac48Address GetAddress (void) const = 0;
  /**
   * \returns the ssid which this MAC layer is going to try to stay in.
   */
  virtual Ssid GetSsid (void) const = 0;
  /**
   * \returns the BSSID associated to the current SSID. 
   *
   * If we are an AP, this is the address of the AP itself.
   * If are a STA, this is the address of the AP with which
   * the STA is associated.
   */
  virtual Mac48Address GetBssid (void) const = 0;
  /**
   * \param address the current address of this MAC layer.
   */
  virtual void SetAddress (Mac48Address address) = 0;
  /**
   * \param ssid the current ssid of this MAC layer.
   */
  virtual void SetSsid (Ssid ssid) = 0;

private:
  friend class WifiNetDevice;
  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the DCF function determines that
   * access it granted to this MAC.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from) = 0;
  /**
   * \param phy the physical layer attached to this MAC.
   */
  virtual void SetWifiPhy (Ptr<WifiPhy> phy) = 0;
  /**
   * \param stationManager the station manager attached to this MAC.
   */
  virtual void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager) = 0;
  /**
   * \param upCallback the callback to invoke when a packet must be forwarded up the stack.
   */
  virtual void SetForwardUpCallback (Callback<void,Ptr<Packet>, const Mac48Address &> upCallback) = 0;
  /**
   * \param linkUp the callback to invoke when the link becomes up.
   */
  virtual void SetLinkUpCallback (Callback<void> linkUp) = 0;
  /**
   * \param linkDown the callback to invoke when the link becomes down.
   */
  virtual void SetLinkDownCallback (Callback<void> linkDown) = 0;


  static Time GetDefaultMaxPropagationDelay (void);
  static Time GetDefaultSlot (void);
  static Time GetDefaultSifs (void);
  static Time GetDefaultEifsNoDifs (void);
  static Time GetDefaultCtsAckDelay (void);
  static Time GetDefaultCtsAckTimeout (void);

  Time m_pifs;
  Time m_ctsTimeout;
  Time m_maxPropagationDelay;
  uint32_t m_maxMsduSize;
};

} // namespace ns3

#endif /* WIFI_MAC_H */
