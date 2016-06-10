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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef ARF_WIFI_MANAGER_H
#define ARF_WIFI_MANAGER_H

#include "wifi-remote-station-manager.h"

namespace ns3 {

/**
 * \ingroup wifi
 * \brief ARF Rate control algorithm
 *
 * This class implements the so-called ARF algorithm which was
 * initially described in <i>WaveLAN-II: A High-performance wireless
 * LAN for the unlicensed band</i>, by A. Kamerman and L. Monteban. in
 * Bell Lab Technical Journal, pages 118-133, Summer 1997.
 *
 * This implementation differs from the initial description in that it
 * uses a packet-based timer rather than a time-based timer as described
 * in XXX (I cannot find back the original paper which described how
 * the time-based timer could be easily replaced with a packet-based
 * timer.)
 *
 * This RAA does not support HT or VHT modes and will error exit
 * if the user tries to configure this RAA with a Wi-Fi MAC that
 * has VhtSupported or HtSupported set.
 */
class ArfWifiManager : public WifiRemoteStationManager
{
public:
  static TypeId GetTypeId (void);
  ArfWifiManager ();
  virtual ~ArfWifiManager ();

  // Inherited from WifiRemoteStationManager
  virtual void SetHtSupported (bool enable);
  virtual void SetVhtSupported (bool enable);

private:
  //overriden from base class
  virtual WifiRemoteStation * DoCreateStation (void) const;
  virtual void DoReportRxOk (WifiRemoteStation *station,
                             double rxSnr, WifiMode txMode);
  virtual void DoReportRtsFailed (WifiRemoteStation *station);
  virtual void DoReportDataFailed (WifiRemoteStation *station);
  virtual void DoReportRtsOk (WifiRemoteStation *station,
                              double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void DoReportDataOk (WifiRemoteStation *station,
                               double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void DoReportFinalRtsFailed (WifiRemoteStation *station);
  virtual void DoReportFinalDataFailed (WifiRemoteStation *station);
  virtual WifiTxVector DoGetDataTxVector (WifiRemoteStation *station);
  virtual WifiTxVector DoGetRtsTxVector (WifiRemoteStation *station);
  virtual bool IsLowLatency (void) const;

  uint32_t m_timerThreshold;
  uint32_t m_successThreshold;
};

} //namespace ns3

#endif /* ARF_WIFI_MANAGER_H */
