/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef IDEAL_WIFI_MANAGER_H
#define IDEAL_WIFI_MANAGER_H

#include <stdint.h>
#include <vector>
#include "wifi-mode.h"
#include "wifi-remote-station-manager.h"

namespace ns3 {

/**
 * \brief Ideal rate control algorithm
 * \ingroup wifi
 *
 * This class implements an 'ideal' rate control algorithm
 * similar to RBAR in spirit (see <i>A rate-adaptive MAC
 * protocol for multihop wireless networks</i> by G. Holland,
 * N. Vaidya, and P. Bahl.): every station keeps track of the
 * snr of every packet received and sends back this snr to the
 * original transmitter by an out-of-band mechanism. Each
 * transmitter keeps track of the last snr sent back by a receiver
 * and uses it to pick a transmission mode based on a set
 * of snr thresholds built from a target ber and transmission
 * mode-specific snr/ber curves.
 */
class IdealWifiManager : public WifiRemoteStationManager
{
public:
  static TypeId GetTypeId (void);
  IdealWifiManager ();
  virtual ~IdealWifiManager ();

  virtual void SetupPhy (Ptr<WifiPhy> phy);

private:
  // overriden from base class
  virtual WifiRemoteStation* DoCreateStation (void) const;
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
  virtual WifiTxVector DoGetDataTxVector (WifiRemoteStation *station, uint32_t size);
  virtual WifiTxVector DoGetRtsTxVector (WifiRemoteStation *station);
  virtual bool IsLowLatency (void) const;

  /**
   * Return the minimum SNR needed to successfully transmit
   * data with this mode at the specified BER.
   *
   * \param mode WifiMode
   * \return the minimum SNR for the given mode
   */
  double GetSnrThreshold (WifiMode mode) const;
  /**
   * Adds a pair of WifiMode and the minimum SNR for that given mode
   * to the list.
   *
   * \param mode WifiMode
   * \param snr the minimum SNR for the given mode
   */
  void AddModeSnrThreshold (WifiMode mode, double snr);

  /**
   * A vector of <snr, mode> pair that holds the minimum SNR for different mode
   */
  typedef std::vector<std::pair<double,WifiMode> > Thresholds;

  double m_ber;  //!< The maximum Bit Error Rate acceptable at any transmission mode
  Thresholds m_thresholds;  //!< List of WifiMode and the minimum SNR pair
};

} // namespace ns3

#endif /* IDEAL_WIFI_MANAGER_H */
