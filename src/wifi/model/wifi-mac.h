/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "qos-utils.h"

namespace ns3 {

class Dcf;

/**
 * \brief base class for all MAC-level wifi objects.
 * \ingroup wifi
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
   * \returns the MAC address associated to this MAC layer.
   */
  virtual Mac48Address GetAddress (void) const = 0;
  /**
   * \returns the ssid which this MAC layer is going to try to stay in.
   */
  virtual Ssid GetSsid (void) const = 0;
  /**
   * \param address the current address of this MAC layer.
   */
  virtual void SetAddress (Mac48Address address) = 0;
  /**
   * \param ssid the current ssid of this MAC layer.
   */
  virtual void SetSsid (Ssid ssid) = 0;
  /**
   * \returns the bssid of the network this device belongs to.
   */
  virtual Mac48Address GetBssid (void) const = 0;
  /**
   * \brief Sets the interface in promiscuous mode.
   *
   * Enables promiscuous mode on the interface. Note that any further
   * filtering on the incoming frame path may affect the overall
   * behavior.
   */
  virtual void SetPromisc (void) = 0;

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   * \param from the address from which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the DCF function determines that
   * access it granted to this MAC.  The extra parameter "from" allows
   * this device to operate in a bridged mode, forwarding received
   * frames without altering the source address.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from) = 0;
  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the DCF function determines that
   * access it granted to this MAC.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to) = 0;
  virtual bool SupportsSendFrom (void) const = 0;
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
  virtual void SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback) = 0;
  /**
   * \param linkUp the callback to invoke when the link becomes up.
   */
  virtual void SetLinkUpCallback (Callback<void> linkUp) = 0;
  /**
   * \param linkDown the callback to invoke when the link becomes down.
   */
  virtual void SetLinkDownCallback (Callback<void> linkDown) = 0;
  /* Next functions are not pure virtual so non Qos WifiMacs are not
   * forced to implement them.
   */
  virtual void SetBasicBlockAckTimeout (Time blockAckTimeout);
  virtual Time GetBasicBlockAckTimeout (void) const;
  virtual void SetCompressedBlockAckTimeout (Time blockAckTimeout);
  virtual Time GetCompressedBlockAckTimeout (void) const;

  /**
   * Public method used to fire a MacTx trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyTx (Ptr<const Packet> packet);

  /**
   * Public method used to fire a MacTxDrop trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyTxDrop (Ptr<const Packet> packet);

  /**
   * Public method used to fire a MacRx trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyRx (Ptr<const Packet> packet);

  /**
   * Public method used to fire a MacPromiscRx trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyPromiscRx (Ptr<const Packet> packet);

  /**
   * Public method used to fire a MacRxDrop trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyRxDrop (Ptr<const Packet> packet);
  /**
   * \param standard the wifi standard to be configured
   */
  void ConfigureStandard (enum WifiPhyStandard standard);

protected:
  void ConfigureDcf (Ptr<Dcf> dcf, uint32_t cwmin, uint32_t cwmax, enum AcIndex ac);
  void ConfigureCCHDcf (Ptr<Dcf> dcf, uint32_t cwmin, uint32_t cwmax, enum AcIndex ac);
private:
  static Time GetDefaultMaxPropagationDelay (void);
  static Time GetDefaultSlot (void);
  static Time GetDefaultSifs (void);
  static Time GetDefaultEifsNoDifs (void);
  static Time GetDefaultCtsAckDelay (void);
  static Time GetDefaultCtsAckTimeout (void);
  static Time GetDefaultBasicBlockAckDelay (void);
  static Time GetDefaultBasicBlockAckTimeout (void);
  static Time GetDefaultCompressedBlockAckDelay (void);
  static Time GetDefaultCompressedBlockAckTimeout (void);
  /**
   * \param standard the phy standard to be used
   *
   * This method is called by ns3::WifiMac::ConfigureStandard to complete
   * the configuration process for a requested phy standard. Subclasses should
   * implement this method to configure their dcf queues according to the
   * requested standard.
   */
  virtual void FinishConfigureStandard (enum WifiPhyStandard standard) = 0;

  Time m_maxPropagationDelay;

  void Configure80211a (void);
  void Configure80211b (void);
  void Configure80211g (void);
  void Configure80211_10Mhz (void);
  void Configure80211_5Mhz ();
  void Configure80211p_CCH (void);
  void Configure80211p_SCH (void);

  /**
   * The trace source fired when packets come into the "top" of the device
   * at the L3/L2 transition, before being queued for transmission.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macTxTrace;

  /**
   * The trace source fired when packets coming into the "top" of the device
   * are dropped at the MAC layer during transmission.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macTxDropTrace;

  /**
   * The trace source fired for packets successfully received by the device
   * immediately before being forwarded up to higher layers (at the L2/L3
   * transition).  This is a promiscuous trace.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macPromiscRxTrace;

  /**
   * The trace source fired for packets successfully received by the device
   * immediately before being forwarded up to higher layers (at the L2/L3
   * transition).  This is a non- promiscuous trace.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macRxTrace;

  /**
   * The trace source fired when packets coming into the "top" of the device
   * are dropped at the MAC layer during reception.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macRxDropTrace;

};

} // namespace ns3

#endif /* WIFI_MAC_H */
