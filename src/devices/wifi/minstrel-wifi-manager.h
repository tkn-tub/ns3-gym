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
#include <vector>



/**
 * \author Duy Nguyen
 * \brief Implementation of Minstrel Rate Control Algorithm 
 *
 * Porting Minstrel from Madwifi and Linux Kernel 
 * http://linuxwireless.org/en/developers/Documentation/mac80211/RateControl/minstrel
 */


namespace ns3 {


/**
 * A struct to contain all information related to a data rate 
 */
struct RateInfo{

  /**
   * Perfect transmission time calculation, or frame calculation
   * Given a bit rate and a packet length n bytes 
   */
  Time perfectTxTime;		


  uint32_t retryCount;  ///< retry limit
  uint32_t adjustedRetryCount;  ///< adjust the retry limit for this rate
  uint32_t numRateAttempt;  ///< how many number of attempts so far
  uint32_t numRateSuccess;  ///< number of successful pkts 
  uint32_t prob;  ///< (# pkts success )/(# total pkts)

  /**
   * EWMA calculation
   * ewma_prob =[prob *(100 - ewma_level) + (ewma_prob_old * ewma_level)]/100 
   */
  uint32_t ewmaProb;

  uint32_t prevNumRateAttempt;  ///< from last rate
  uint32_t prevNumRateSuccess;  ///< from last rate
  uint64_t successHist;  ///< aggregate of all successes
  uint64_t attemptHist;  ///< aggregate of all attempts
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


class MinstrelWifiManager : public WifiRemoteStationManager
{

public:
  static TypeId GetTypeId (void);
  MinstrelWifiManager ();
  virtual ~MinstrelWifiManager();

  virtual void SetupPhy (Ptr<WifiPhy> phy);

  /// for estimating the TxTime of a packet with a given mode  
  Time GetCalcTxTime (WifiMode mode) const;
  void AddCalcTxTime (WifiMode mode, Time t);

private:
  friend class MinstrelWifiRemoteStation;
  virtual class WifiRemoteStation *CreateStation (void);

  typedef std::vector<std::pair<Time,WifiMode> > TxTime;

  TxTime m_calcTxTime;  ///< to hold all the calculated TxTime for all modes
  Time m_updateStats;  ///< how frequent do we calculate the stats(1/10 seconds)
  double m_lookAroundRate;  ///< the % to try other rates than our current rate 
  double m_ewmaLevel;  ///< exponential weighted moving average
  uint32_t m_segmentSize;  ///< largest allowable segment size
  uint32_t m_sampleCol;  ///< number of sample columns
  uint32_t m_pktLen;  ///< packet length used  for calculate mode TxTime
  
};

class MinstrelWifiRemoteStation : public WifiRemoteStation
{
public:
  MinstrelWifiRemoteStation (Ptr<MinstrelWifiManager> stations);
 
  virtual ~MinstrelWifiRemoteStation ();

protected:

  /** 
   * when packet is successfully received
   * see wifi-remote-station-manager.h for more documentation
   */
  virtual void DoReportRxOk (double rxSnr, WifiMode txMode);

  /// when RTS timeout expires
  virtual void DoReportRtsFailed (void);


  /**
   *
   * Retry Chain table is implemented here
   *
   * Try |         LOOKAROUND RATE              | NORMAL RATE
   *     | random < best    | random > best     |
   * --------------------------------------------------------------
   *  1  | Best throughput  | Random rate       | Best throughput
   *  2  | Random rate      | Best throughput   | Next best throughput
   *  3  | Best probability | Best probability  | Best probability
   *  4  | Lowest Baserate  | Lowest baserate   | Lowest baserate
   *
   * Note: For clarity, multiple blocks of if's and else's are used
   * After a failing 7 times, DoReportFinalDataFailed will be called
   */
  virtual void DoReportDataFailed (void);

  /// when receive a CTS, associated with an RTS
  virtual void DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);

  /// when an ACK, associated with a data pkt, is received
  virtual void DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);

  /// after calling ReportRtsFailed if NeedRtsRetransmission returns false
  virtual void DoReportFinalRtsFailed (void);

  /// after calling ReportDataFailed if NeedDataRetransmission returns false
  virtual void DoReportFinalDataFailed (void);


private:
  virtual Ptr<WifiRemoteStationManager> GetManager (void) const;

  /** 
   * returns the transmission mode for sending this packet
   * this function gets called when node is getting ready to send DATA
   * see wifi-remote-station-manager.h for more documentation
   */
  virtual WifiMode DoGetDataMode (uint32_t size);

  /// returns the transmission mode for sending RTS packet
  virtual WifiMode DoGetRtsMode (void);

  /// update the number of retries and reset accordingly
  void UpdateRetry (void);

  /// getting the next sample from Sample Table
  uint32_t GetNextSample (void);

  /// find a rate to use from Minstrel Table
  uint32_t FindRate (void);

  /// updating the Minstrel Table every 1/10 seconds
  void UpdateStats (void);

  /// initialize Minstrel Table
  void RateInit (void);

  /// initialize Sample Table
  void InitSampleTable (void);

  /// printing Sample Table
  void PrintSampleTable (void);

  /// printing Minstrel Table
  void PrintTable (void);

  /**
   * \param packet lenghth 
   * \param current WifiMode
   * \returns calcuated transmit duration 
   */
  Time CalcRatePacket (uint32_t, WifiMode);

  void CheckInit(void);  ///< check for initializations

  Ptr<MinstrelWifiManager> m_stations;

  Time m_nextStatsUpdate;  ///< 10 times every second

  MinstrelRate m_minstrelTable;  ///< minstrel table	

  SampleRate m_sampleTable;  ///< sample table

  /**
   * To keep track of the current position in the our random sample table
   * going row by row from 1st column until the 10th column(Minstrel defines 10)
   * then we wrap back to the row 1 col 1.
   * note: there are many other ways to do this.
   */
  uint32_t m_col, m_index;							

  uint32_t m_maxTpRate;  ///< the current throughput rate 
  uint32_t m_maxTpRate2;  ///< second highest throughput rate
  uint32_t m_maxProbRate;  ///< rate with highest prob of success

  int m_packetCount;  ///< total number of packets as of now
  int m_sampleCount;  ///< how many packets we have sample so far

  bool m_isSampling;  ///< a flag to indicate we are currently sampling
  uint32_t m_sampleRate;  ///< current sample rate
  bool 	m_sampleRateSlower;  ///< a flag to indicate sample rate is slower
  uint32_t m_currentRate;  ///< current rate we are using

  uint32_t m_shortRetry;  ///< short retries such as control packts
  uint32_t m_longRetry;  ///< long retries such as data packets
  uint32_t m_retry;  ///< total retries short + long
  uint32_t m_err;  ///< retry errors
  uint32_t m_txrate;  ///< current transmit rate

  bool m_initialized;  ///< for initializing tables


}; 

}// namespace ns3

#endif /* MINSTREL_WIFI_MANAGER_H */
