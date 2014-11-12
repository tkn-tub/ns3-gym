/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Duy Nguyen
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
 * Author: Duy Nguyen <duy@soe.ucsc.edu>
 */



#ifndef MINSTREL_WIFI_MANAGER_H
#define MINSTREL_WIFI_MANAGER_H

#include "wifi-remote-station-manager.h"
#include "wifi-mode.h"
#include "ns3/nstime.h"
#include "ns3/random-variable-stream.h"

namespace ns3 {

struct MinstrelWifiRemoteStation;

/**
 * A struct to contain all information related to a data rate
 */
struct RateInfo
{
  /**
   * Perfect transmission time calculation, or frame calculation
   * Given a bit rate and a packet length n bytes
   */
  Time perfectTxTime;


  uint32_t retryCount;  ///< retry limit
  uint32_t adjustedRetryCount;  ///< adjust the retry limit for this rate
  uint32_t numRateAttempt;  ///< how many number of attempts so far
  uint32_t numRateSuccess;    ///< number of successful pkts
  uint32_t prob;  ///< (# pkts success )/(# total pkts)

  /**
   * EWMA calculation
   * ewma_prob =[prob *(100 - ewma_level) + (ewma_prob_old * ewma_level)]/100
   */
  uint32_t ewmaProb;

  uint32_t throughput;  ///< throughput of a rate
};

/**
 * Data structure for a Minstrel Rate table
 * A vector of a struct RateInfo
 */
typedef std::vector<struct RateInfo> MinstrelRate;

/**
 * Data structure for a Sample Rate table
 * A vector of a vector uint32_t
 */
typedef std::vector<std::vector<uint32_t> > SampleRate;


/**
 * \author Duy Nguyen
 * \brief Implementation of Minstrel Rate Control Algorithm
 * \ingroup wifi
 *
 * Porting Minstrel from Madwifi and Linux Kernel
 * http://linuxwireless.org/en/developers/Documentation/mac80211/RateControl/minstrel
 */
class MinstrelWifiManager : public WifiRemoteStationManager
{

public:
  static TypeId GetTypeId (void);
  MinstrelWifiManager ();
  virtual ~MinstrelWifiManager ();

  virtual void SetupPhy (Ptr<WifiPhy> phy);

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  int64_t AssignStreams (int64_t stream);

private:
  // overriden from base class
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
  virtual WifiTxVector DoGetDataTxVector (WifiRemoteStation *station, uint32_t size);
  virtual WifiTxVector DoGetRtsTxVector (WifiRemoteStation *station);

  virtual bool DoNeedDataRetransmission (WifiRemoteStation *st, Ptr<const Packet> packet, bool normally);

  virtual bool IsLowLatency (void) const;

  /// for estimating the TxTime of a packet with a given mode
  Time GetCalcTxTime (WifiMode mode) const;
  /**
   * Add transmission time for the given mode to an internal list.
   *
   * \param mode Wi-Fi mode
   * \param t transmission time
   */
  void AddCalcTxTime (WifiMode mode, Time t);

  /// update the number of retries and reset accordingly
  void UpdateRetry (MinstrelWifiRemoteStation *station);

  /// getting the next sample from Sample Table
  uint32_t GetNextSample (MinstrelWifiRemoteStation *station);

  /// find a rate to use from Minstrel Table
  uint32_t FindRate (MinstrelWifiRemoteStation *station);

  /// updating the Minstrel Table every 1/10 seconds
  void UpdateStats (MinstrelWifiRemoteStation *station);

  /// initialize Minstrel Table
  void RateInit (MinstrelWifiRemoteStation *station);

  /**
   * Estimate the time to transmit the given packet with the given number of retries.
   * This function is "roughly" the function "calc_usecs_unicast_packet" in minstrel.c
   * in the madwifi implementation.
   *
   * The basic idea is that, we try to estimate the "average" time used to transmit the
   * packet for the given number of retries while also accounting for the 802.11 congestion
   * window change. The original code in the madwifi seems to estimate the number of backoff
   * slots as the half of the current CW size.
   *
   * There are four main parts:
   *  - wait for DIFS (sense idle channel)
   *  - ACK timeouts
   *  - DATA transmission
   *  - backoffs according to CW
   */
  Time CalculateTimeUnicastPacket (Time dataTransmissionTime, uint32_t shortRetries, uint32_t longRetries);

  /// initialize Sample Table
  void InitSampleTable (MinstrelWifiRemoteStation *station);

  /// printing Sample Table
  void PrintSampleTable (MinstrelWifiRemoteStation *station);

  /// printing Minstrel Table
  void PrintTable (MinstrelWifiRemoteStation *station);

  void CheckInit (MinstrelWifiRemoteStation *station);  ///< check for initializations

  /**
   * typedef for a vector of a pair of Time, WifiMode.
   * (Essentially a list for WifiMode and its corresponding transmission time
   * to transmit a reference packet.
   */
  typedef std::vector<std::pair<Time,WifiMode> > TxTime;

  TxTime m_calcTxTime;  ///< to hold all the calculated TxTime for all modes
  Time m_updateStats;  ///< how frequent do we calculate the stats(1/10 seconds)
  double m_lookAroundRate;  ///< the % to try other rates than our current rate
  double m_ewmaLevel;  ///< exponential weighted moving average
  uint32_t m_sampleCol;  ///< number of sample columns
  uint32_t m_pktLen;  ///< packet length used  for calculate mode TxTime
  uint32_t m_nsupported;  ///< modes supported

  /// Provides uniform random variables.
  Ptr<UniformRandomVariable> m_uniformRandomVariable;
};

} // namespace ns3

#endif /* MINSTREL_WIFI_MANAGER_H */
