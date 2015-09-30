/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2009 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 */

#ifndef AP_WIFI_MAC_H
#define AP_WIFI_MAC_H

#include "regular-wifi-mac.h"
#include "capability-information.h"
#include "ht-capabilities.h"
#include "vht-capabilities.h"
#include "amsdu-subframe-header.h"
#include "supported-rates.h"
#include "ns3/random-variable-stream.h"

namespace ns3 {

/**
 * \brief Wi-Fi AP state machine
 * \ingroup wifi
 *
 * Handle association, dis-association and authentication,
 * of STAs within an infrastructure BSS.
 */
class ApWifiMac : public RegularWifiMac
{
public:
  static TypeId GetTypeId (void);

  ApWifiMac ();
  virtual ~ApWifiMac ();

  /**
   * \param stationManager the station manager attached to this MAC.
   */
  virtual void SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager);

  /**
   * \param linkUp the callback to invoke when the link becomes up.
   */
  virtual void SetLinkUpCallback (Callback<void> linkUp);

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to);

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   * \param from the address from which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.  The extra parameter "from" allows
   * this device to operate in a bridged mode, forwarding received
   * frames without altering the source address.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from);

  virtual bool SupportsSendFrom (void) const;

  /**
   * \param address the current address of this MAC layer.
   */
  virtual void SetAddress (Mac48Address address);
  /**
   * \param interval the interval between two beacon transmissions.
   */
  void SetBeaconInterval (Time interval);
  /**
   * \return the interval between two beacon transmissions.
   */
  Time GetBeaconInterval (void) const;
  /**
   * Start beacon transmission immediately.
   */
  void StartBeaconing (void);

  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model.  Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use
   *
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);


private:
  virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr);
  /**
   * The packet we sent was successfully received by the receiver
   * (i.e. we received an ACK from the receiver).  If the packet
   * was an association response to the receiver, we record that
   * the receiver is now associated with us.
   *
   * \param hdr the header of the packet that we successfully sent
   */
  virtual void TxOk (const WifiMacHeader &hdr);
  /**
   * The packet we sent was successfully received by the receiver
   * (i.e. we did not receive an ACK from the receiver).  If the packet
   * was an association response to the receiver, we record that
   * the receiver is not associated with us yet.
   *
   * \param hdr the header of the packet that we failed to sent
   */
  virtual void TxFailed (const WifiMacHeader &hdr);

  /**
   * This method is called to de-aggregate an A-MSDU and forward the
   * constituent packets up the stack. We override the WifiMac version
   * here because, as an AP, we also need to think about redistributing
   * to other associated STAs.
   *
   * \param aggregatedPacket the Packet containing the A-MSDU.
   * \param hdr a pointer to the MAC header for \c aggregatedPacket.
   */
  virtual void DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket,
                                           const WifiMacHeader *hdr);
  /**
   * Forward the packet down to DCF/EDCAF (enqueue the packet). This method
   * is a wrapper for ForwardDown with traffic id.
   *
   * \param packet the packet we are forwarding to DCF/EDCAF
   * \param from the address to be used for Address 3 field in the header
   * \param to the address to be used for Address 1 field in the header
   */
  void ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to);
  /**
   * Forward the packet down to DCF/EDCAF (enqueue the packet).
   *
   * \param packet the packet we are forwarding to DCF/EDCAF
   * \param from the address to be used for Address 3 field in the header
   * \param to the address to be used for Address 1 field in the header
   * \param tid the traffic id for the packet
   */
  void ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to, uint8_t tid);
  /**
   * Forward a probe response packet to the DCF. The standard is not clear on the correct
   * queue for management frames if QoS is supported. We always use the DCF.
   *
   * \param to the address of the STA we are sending a probe response to
   */
  void SendProbeResp (Mac48Address to);
  /**
   * Forward an association response packet to the DCF. The standard is not clear on the correct
   * queue for management frames if QoS is supported. We always use the DCF.
   *
   * \param to the address of the STA we are sending an association response to
   * \param success indicates whether the association was successful or not
   */
  void SendAssocResp (Mac48Address to, bool success);
  /**
   * Forward a beacon packet to the beacon special DCF.
   */
  void SendOneBeacon (void);
  /**
   * Return the Capability information of the current AP.
   *
   * \return the Capability information that we support
   */
  CapabilityInformation GetCapabilities (void) const;
  /**
   * Return the HT capability of the current AP.
   *
   * \return the HT capability that we support
   */
  HtCapabilities GetHtCapabilities (void) const;
  /**
   * Return the VHT capability of the current AP.
   *
   * \return the VHT capability that we support
   */
  VhtCapabilities GetVhtCapabilities (void) const;
  /**
   * Return an instance of SupportedRates that contains all rates that we support
   * including HT rates.
   *
   * \return SupportedRates all rates that we support
   */
  SupportedRates GetSupportedRates (void) const;
  /**
   * Enable or disable beacon generation of the AP.
   *
   * \param enable enable or disable beacon generation
   */
  void SetBeaconGeneration (bool enable);
  /**
   * Return whether the AP is generating beacons.
   *
   * \return true if beacons are periodically generated, false otherwise
   */
  bool GetBeaconGeneration (void) const;

  virtual void DoDispose (void);
  virtual void DoInitialize (void);

  Ptr<DcaTxop> m_beaconDca;                  //!< Dedicated DcaTxop for beacons
  Time m_beaconInterval;                     //!< Interval between beacons
  bool m_enableBeaconGeneration;             //!< Flag if beacons are being generated
  EventId m_beaconEvent;                     //!< Event to generate one beacon
  Ptr<UniformRandomVariable> m_beaconJitter; //!< UniformRandomVariable used to randomize the time of the first beacon
  bool m_enableBeaconJitter;                 //!< Flag if the first beacon should be generated at random time
};

} //namespace ns3

#endif /* AP_WIFI_MAC_H */
