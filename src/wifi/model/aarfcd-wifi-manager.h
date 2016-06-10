/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */

#ifndef AARFCD_WIFI_MANAGER_H
#define AARFCD_WIFI_MANAGER_H

#include "wifi-remote-station-manager.h"

namespace ns3 {

struct AarfcdWifiRemoteStation;

/**
 * \brief an implementation of the AARF-CD algorithm
 * \ingroup wifi
 *
 * This algorithm was first described in "Efficient Collision Detection for Auto Rate Fallback Algorithm".
 * The implementation available here was done by Federico Maguolo for a very early development
 * version of ns-3. Federico died before merging this work in ns-3 itself so his code was ported
 * to ns-3 later without his supervision.
 *
 * This RAA does not support HT or VHT modes and will error exit
 * if the user tries to configure this RAA with a Wi-Fi MAC that
 * has VhtSupported or HtSupported set.
 */
class AarfcdWifiManager : public WifiRemoteStationManager
{
public:
  static TypeId GetTypeId (void);
  AarfcdWifiManager ();
  virtual ~AarfcdWifiManager ();

  // Inherited from WifiRemoteStationManager
  virtual void SetHtSupported (bool enable);
  virtual void SetVhtSupported (bool enable);

private:
  // overriden from base class
  virtual WifiRemoteStation * DoCreateStation (void) const;
  virtual void DoReportRxOk (WifiRemoteStation *station,
                             double rxSnr, WifiMode txMode);

  virtual void DoReportRtsFailed (WifiRemoteStation *station);
  /**
   * It is important to realize that "recovery" mode starts after failure of
   * the first transmission after a rate increase and ends at the first successful
   * transmission. Specifically, recovery mode transcends retransmissions boundaries.
   * Fundamentally, ARF handles each data transmission independently, whether it
   * is the initial transmission of a packet or the retransmission of a packet.
   * The fundamental reason for this is that there is a backoff between each data
   * transmission, be it an initial transmission or a retransmission.
   *
   * \param station the station that we failed to send DATA
   */
  virtual void DoReportDataFailed (WifiRemoteStation *station);
  virtual void DoReportRtsOk (WifiRemoteStation *station,
                              double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void DoReportDataOk (WifiRemoteStation *station,
                               double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void DoReportFinalRtsFailed (WifiRemoteStation *station);
  virtual void DoReportFinalDataFailed (WifiRemoteStation *station);
  virtual WifiTxVector DoGetDataTxVector (WifiRemoteStation *station);
  virtual WifiTxVector DoGetRtsTxVector (WifiRemoteStation *station);
  virtual bool DoNeedRts (WifiRemoteStation *station,
                          Ptr<const Packet> packet, bool normally);
  virtual bool IsLowLatency (void) const;

  /**
   * Check if the use of RTS for the given station can be turned off.
   *
   * \param station the station that we are checking
   */
  void CheckRts (AarfcdWifiRemoteStation *station);
  /**
   * Increase the RTS window size of the given station.
   *
   * \param station the station to increase RTS window
   */
  void IncreaseRtsWnd (AarfcdWifiRemoteStation *station);
  /**
   * Reset the RTS window of the given station.
   *
   * \param station the station to reset RTS window
   */
  void ResetRtsWnd (AarfcdWifiRemoteStation *station);
  /**
   * Turn off RTS for the given station.
   *
   * \param station the station to turn RTS off
   */
  void TurnOffRts (AarfcdWifiRemoteStation *station);
  /**
   * Turn on RTS for the given station.
   *
   * \param station the station to turn RTS on
   */
  void TurnOnRts (AarfcdWifiRemoteStation *station);

  //aarf fields below
  uint32_t m_minTimerThreshold;
  uint32_t m_minSuccessThreshold;
  double m_successK;
  uint32_t m_maxSuccessThreshold;
  double m_timerK;

  //aarf-cd fields below
  uint32_t m_minRtsWnd;
  uint32_t m_maxRtsWnd;
  bool m_turnOffRtsAfterRateDecrease;
  bool m_turnOnRtsAfterRateIncrease;
};

} //namespace ns3

#endif /* AARFCD_WIFI_MANAGER_H */
