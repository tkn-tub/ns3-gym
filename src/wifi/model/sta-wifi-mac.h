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

#ifndef STA_WIFI_MAC_H
#define STA_WIFI_MAC_H

#include "regular-wifi-mac.h"
#include "supported-rates.h"
#include "capability-information.h"

namespace ns3  {

class MgtAddBaRequestHeader;

/**
 * \ingroup wifi
 *
 * The Wifi MAC high model for a non-AP STA in a BSS.
 */
class StaWifiMac : public RegularWifiMac
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  StaWifiMac ();
  virtual ~StaWifiMac ();

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.
   */
  void Enqueue (Ptr<const Packet> packet, Mac48Address to);

  /**
   * \param missed the number of beacons which must be missed
   * before a new association sequence is started.
   */
  void SetMaxMissedBeacons (uint32_t missed);
  /**
   * \param timeout
   *
   * If no probe response is received within the specified
   * timeout, the station sends a new probe request.
   */
  void SetProbeRequestTimeout (Time timeout);
  /**
   * \param timeout
   *
   * If no association response is received within the specified
   * timeout, the station sends a new association request.
   */
  void SetAssocRequestTimeout (Time timeout);

  /**
   * Start an active association sequence immediately.
   */
  void StartActiveAssociation (void);


private:
  /**
   * The current MAC state of the STA.
   */
  enum MacState
  {
    ASSOCIATED,
    WAIT_PROBE_RESP,
    WAIT_ASSOC_RESP,
    BEACON_MISSED,
    REFUSED
  };

  /**
   * Enable or disable active probing.
   *
   * \param enable enable or disable active probing
   */
  void SetActiveProbing (bool enable);
  /**
   * Return whether active probing is enabled.
   *
   * \return true if active probing is enabled, false otherwise
   */
  bool GetActiveProbing (void) const;

  void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr);

  /**
   * Forward a probe request packet to the DCF. The standard is not clear on the correct
   * queue for management frames if QoS is supported. We always use the DCF.
   */
  void SendProbeRequest (void);
  /**
   * Forward an association request packet to the DCF. The standard is not clear on the correct
   * queue for management frames if QoS is supported. We always use the DCF.
   */
  void SendAssociationRequest (void);
  /**
   * Try to ensure that we are associated with an AP by taking an appropriate action
   * depending on the current association status.
   */
  void TryToEnsureAssociated (void);
  /**
   * This method is called after the association timeout occurred. We switch the state to
   * WAIT_ASSOC_RESP and re-send an association request.
   */
  void AssocRequestTimeout (void);
  /**
   * This method is called after the probe request timeout occurred. We switch the state to
   * WAIT_PROBE_RESP and re-send a probe request.
   */
  void ProbeRequestTimeout (void);
  /**
   * Return whether we are associated with an AP.
   *
   * \return true if we are associated with an AP, false otherwise
   */
  bool IsAssociated (void) const;
  /**
   * Return whether we are waiting for an association response from an AP.
   *
   * \return true if we are waiting for an association response from an AP, false otherwise
   */
  bool IsWaitAssocResp (void) const;
  /**
   * This method is called after we have not received a beacon from the AP
   */
  void MissedBeacons (void);
  /**
   * Restarts the beacon timer.
   *
   * \param delay the delay before the watchdog fires
   */
  void RestartBeaconWatchdog (Time delay);
  /**
   * Return an instance of SupportedRates that contains all rates that we support
   * including HT rates.
   *
   * \return SupportedRates all rates that we support
   */
  SupportedRates GetSupportedRates (void) const;
  /**
   * Set the current MAC state.
   *
   * \param value the new state
   */
  void SetState (MacState value);
  /**
   * Set the EDCA parameters.
   *
   * \param ac the access class
   * \param cwMin the minimum contention window size
   * \param cwMax the maximum contention window size
   * \param aifsn the number of slots that make up an AIFS
   * \param txopLimit the TXOP limit
   */
  void SetEdcaParameters (AcIndex ac, uint8_t cwMin, uint8_t cwMax, uint8_t aifsn, Time txopLimit);
  /**
   * Return the Capability information of the current STA.
   *
   * \return the Capability information that we support
   */
  CapabilityInformation GetCapabilities (void) const;

  MacState m_state;            ///< MAC state
  Time m_probeRequestTimeout;  ///< probe request timeout
  Time m_assocRequestTimeout;  ///< assoc request timeout
  EventId m_probeRequestEvent; ///< probe request event
  EventId m_assocRequestEvent; ///< assoc request event
  EventId m_beaconWatchdog;    ///< beacon watchdog
  Time m_beaconWatchdogEnd;    ///< beacon watchdog end
  uint32_t m_maxMissedBeacons; ///< maximum missed beacons
  bool m_activeProbing;        ///< active probing

  TracedCallback<Mac48Address> m_assocLogger;   ///< assoc logger
  TracedCallback<Mac48Address> m_deAssocLogger; ///< deassoc logger
};

} //namespace ns3

#endif /* STA_WIFI_MAC_H */
