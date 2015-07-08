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
 *
 * Some Comments:
 *
 * 1) Segment Size is declared for completeness but not used  because it has
 *    to do more with the requirement of the specific hardware.
 *
 * 2) By default, Minstrel applies the multi-rate retry(the core of Minstrel
 *    algorithm). Otherwise, please use ConstantRateWifiManager instead.
 *
 * http://linuxwireless.org/en/developers/Documentation/mac80211/RateControl/minstrel
 */

#include "minstrel-wifi-manager.h"
#include "wifi-phy.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/wifi-mac.h"
#include "ns3/assert.h"
#include <vector>

#define Min(a,b) ((a < b) ? a : b)

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MinstrelWifiManager");

/**
 * \brief hold per-remote-station state for Minstrel Wifi manager.
 *
 * This struct extends from WifiRemoteStation struct to hold additional
 * information required by the Minstrel Wifi manager
 */
struct MinstrelWifiRemoteStation : public WifiRemoteStation
{
  Time m_nextStatsUpdate;  ///< 10 times every second

  /**
   * To keep track of the current position in the our random sample table
   * going row by row from 1st column until the 10th column(Minstrel defines 10)
   * then we wrap back to the row 1 col 1.
   * note: there are many other ways to do this.
   */
  uint32_t m_col, m_index;
  uint32_t m_maxTpRate;          ///< the current throughput rate
  uint32_t m_maxTpRate2;         ///< second highest throughput rate
  uint32_t m_maxProbRate;        ///< rate with highest prob of success
  int m_packetCount;             ///< total number of packets as of now
  int m_sampleCount;             ///< how many packets we have sample so far
  bool m_isSampling;             ///< a flag to indicate we are currently sampling
  uint32_t m_sampleRate;         ///< current sample rate
  bool  m_sampleRateSlower;      ///< a flag to indicate sample rate is slower
  uint32_t m_shortRetry;         ///< short retries such as control packts
  uint32_t m_longRetry;          ///< long retries such as data packets
  uint32_t m_retry;              ///< total retries short + long
  uint32_t m_err;                ///< retry errors
  uint32_t m_txrate;             ///< current transmit rate
  bool m_initialized;            ///< for initializing tables
  MinstrelRate m_minstrelTable;  ///< minstrel table
  SampleRate m_sampleTable;      ///< sample table
};

NS_OBJECT_ENSURE_REGISTERED (MinstrelWifiManager);

TypeId
MinstrelWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MinstrelWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .SetGroupName ("Wifi")
    .AddConstructor<MinstrelWifiManager> ()
    .AddAttribute ("UpdateStatistics",
                   "The interval between updating statistics table ",
                   TimeValue (Seconds (0.1)),
                   MakeTimeAccessor (&MinstrelWifiManager::m_updateStats),
                   MakeTimeChecker ())
    .AddAttribute ("LookAroundRate",
                   "the percentage to try other rates",
                   DoubleValue (10),
                   MakeDoubleAccessor (&MinstrelWifiManager::m_lookAroundRate),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("EWMA",
                   "EWMA level",
                   DoubleValue (75),
                   MakeDoubleAccessor (&MinstrelWifiManager::m_ewmaLevel),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SampleColumn",
                   "The number of columns used for sampling",
                   DoubleValue (10),
                   MakeDoubleAccessor (&MinstrelWifiManager::m_sampleCol),
                   MakeDoubleChecker <double> ())
    .AddAttribute ("PacketLength",
                   "The packet length used for calculating mode TxTime",
                   DoubleValue (1200),
                   MakeDoubleAccessor (&MinstrelWifiManager::m_pktLen),
                   MakeDoubleChecker <double> ())
  ;
  return tid;
}

MinstrelWifiManager::MinstrelWifiManager ()
{
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();

  m_nsupported = 0;
}

MinstrelWifiManager::~MinstrelWifiManager ()
{
}

void
MinstrelWifiManager::SetupPhy (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  uint32_t nModes = phy->GetNModes ();
  for (uint32_t i = 0; i < nModes; i++)
    {
      WifiMode mode = phy->GetMode (i);
      WifiTxVector txVector;
      txVector.SetMode (mode);
      AddCalcTxTime (mode, phy->CalculateTxDuration (m_pktLen, txVector, WIFI_PREAMBLE_LONG, phy->GetFrequency (), 0, 0));
    }
  WifiRemoteStationManager::SetupPhy (phy);
}

int64_t
MinstrelWifiManager::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_uniformRandomVariable->SetStream (stream);
  return 1;
}

Time
MinstrelWifiManager::GetCalcTxTime (WifiMode mode) const
{

  for (TxTime::const_iterator i = m_calcTxTime.begin (); i != m_calcTxTime.end (); i++)
    {
      if (mode == i->second)
        {
          return i->first;
        }
    }
  NS_ASSERT (false);
  return Seconds (0);
}

void
MinstrelWifiManager::AddCalcTxTime (WifiMode mode, Time t)
{
  m_calcTxTime.push_back (std::make_pair (t, mode));
}

WifiRemoteStation *
MinstrelWifiManager::DoCreateStation (void) const
{
  MinstrelWifiRemoteStation *station = new MinstrelWifiRemoteStation ();

  station->m_nextStatsUpdate = Simulator::Now () + m_updateStats;
  station->m_col = 0;
  station->m_index = 0;
  station->m_maxTpRate = 0;
  station->m_maxTpRate2 = 0;
  station->m_maxProbRate = 0;
  station->m_packetCount = 0;
  station->m_sampleCount = 0;
  station->m_isSampling = false;
  station->m_sampleRate = 0;
  station->m_sampleRateSlower = false;
  station->m_shortRetry = 0;
  station->m_longRetry = 0;
  station->m_retry = 0;
  station->m_err = 0;
  station->m_txrate = 0;
  station->m_initialized = false;

  return station;
}

void
MinstrelWifiManager::CheckInit (MinstrelWifiRemoteStation *station)
{
  if (!station->m_initialized && GetNSupported (station) > 1)
    {
      //Note: we appear to be doing late initialization of the table
      //to make sure that the set of supported rates has been initialized
      //before we perform our own initialization.
      m_nsupported = GetNSupported (station);
      station->m_minstrelTable = MinstrelRate (m_nsupported);
      station->m_sampleTable = SampleRate (m_nsupported, std::vector<uint32_t> (m_sampleCol));
      InitSampleTable (station);
      RateInit (station);
      station->m_initialized = true;

      PrintTable (station);
      PrintSampleTable (station);
    }
}

void
MinstrelWifiManager::DoReportRxOk (WifiRemoteStation *st,
                                   double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this);
}

void
MinstrelWifiManager::DoReportRtsFailed (WifiRemoteStation *st)
{
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *)st;
  NS_LOG_DEBUG ("DoReportRtsFailed m_txrate=" << station->m_txrate);

  station->m_shortRetry++;
}

void
MinstrelWifiManager::DoReportRtsOk (WifiRemoteStation *st, double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_DEBUG ("self=" << st << " rts ok");
}

void
MinstrelWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *st)
{
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *)st;
  UpdateRetry (station);
  station->m_err++;
}

void
MinstrelWifiManager::DoReportDataFailed (WifiRemoteStation *st)
{
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *)st;
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

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  station->m_longRetry++;
  station->m_minstrelTable[station->m_txrate].numRateAttempt++;

  PrintTable (station);

  NS_LOG_DEBUG ("DoReportDataFailed " << station << " rate " << station->m_txrate << " longRetry " << station->m_longRetry);

  //for normal rate, we're not currently sampling random rates
  if (!station->m_isSampling)
    {
      NS_LOG_DEBUG ("Failed with normal rate: current=" << station->m_txrate << ", sample=" << station->m_sampleRate << ", maxTp=" << station->m_maxTpRate << ", maxTp2=" << station->m_maxTpRate2 << ", maxProb=" << station->m_maxProbRate);
      //use best throughput rate
      if (station->m_longRetry < station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount)
        {
          NS_LOG_DEBUG (" More retries left for the maximum throughput rate.");
          station->m_txrate = station->m_maxTpRate;
        }

      //use second best throughput rate
      else if (station->m_longRetry <= (station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                        station->m_minstrelTable[station->m_maxTpRate2].adjustedRetryCount))
        {
          NS_LOG_DEBUG (" More retries left for the second maximum throughput rate.");
          station->m_txrate = station->m_maxTpRate2;
        }

      //use best probability rate
      else if (station->m_longRetry <= (station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                        station->m_minstrelTable[station->m_maxTpRate2].adjustedRetryCount +
                                        station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount))
        {
          NS_LOG_DEBUG (" More retries left for the maximum probability rate.");
          station->m_txrate = station->m_maxProbRate;
        }

      //use lowest base rate
      else if (station->m_longRetry > (station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                       station->m_minstrelTable[station->m_maxTpRate2].adjustedRetryCount +
                                       station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount))
        {
          NS_LOG_DEBUG (" More retries left for the base rate.");
          station->m_txrate = 0;
        }
    }

  //for look-around rate, we're currently sampling random rates
  else
    {
      NS_LOG_DEBUG ("Failed with look around rate: current=" << station->m_txrate << ", sample=" << station->m_sampleRate << ", maxTp=" << station->m_maxTpRate << ", maxTp2=" << station->m_maxTpRate2 << ", maxProb=" << station->m_maxProbRate);
      //current sampling rate is slower than the current best rate
      if (station->m_sampleRateSlower)
        {
          NS_LOG_DEBUG ("Look around rate is slower than the maximum throughput rate.");
          //use best throughput rate
          if (station->m_longRetry < station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount)
            {
              NS_LOG_DEBUG (" More retries left for the maximum throughput rate.");
              station->m_txrate = station->m_maxTpRate;
            }

          //use random rate
          else if (station->m_longRetry <= (station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                            station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount))
            {
              NS_LOG_DEBUG (" More retries left for the sampling rate.");
              station->m_txrate = station->m_sampleRate;
            }

          //use max probability rate
          else if (station->m_longRetry <= (station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                            station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount +
                                            station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount ))
            {
              NS_LOG_DEBUG (" More retries left for the maximum probability rate.");
              station->m_txrate = station->m_maxProbRate;
            }

          //use lowest base rate
          else if (station->m_longRetry > (station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                           station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount +
                                           station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount))
            {
              NS_LOG_DEBUG (" More retries left for the base rate.");
              station->m_txrate = 0;
            }
        }
      //current sampling rate is better than current best rate
      else
        {
          NS_LOG_DEBUG ("Look around rate is faster than the maximum throughput rate.");
          //use random rate
          if (station->m_longRetry < station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount)
            {
              NS_LOG_DEBUG (" More retries left for the sampling rate.");
              station->m_txrate = station->m_sampleRate;
            }

          //use the best throughput rate
          else if (station->m_longRetry <= (station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount +
                                            station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount))
            {
              NS_LOG_DEBUG (" More retries left for the maximum throughput rate.");
              station->m_txrate = station->m_maxTpRate;
            }

          //use the best probability rate
          else if (station->m_longRetry <= (station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount +
                                            station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                            station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount))
            {
              NS_LOG_DEBUG (" More retries left for the maximum probability rate.");
              station->m_txrate = station->m_maxProbRate;
            }

          //use the lowest base rate
          else if (station->m_longRetry > (station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount +
                                           station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                           station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount))
            {
              NS_LOG_DEBUG (" More retries left for the base rate.");
              station->m_txrate = 0;
            }
        }
    }
}

void
MinstrelWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                     double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (st << ackSnr << ackMode << dataSnr);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *) st;

  station->m_isSampling = false;
  station->m_sampleRateSlower = false;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }
  NS_LOG_DEBUG ("DoReportDataOk m_txrate = " << station->m_txrate << ", attempt = " << station->m_minstrelTable[station->m_txrate].numRateAttempt << ", success = " << station->m_minstrelTable[station->m_txrate].numRateSuccess << " (before update).");

  station->m_minstrelTable[station->m_txrate].numRateSuccess++;
  station->m_minstrelTable[station->m_txrate].numRateAttempt++;

  NS_LOG_DEBUG ("DoReportDataOk m_txrate = " << station->m_txrate << ", attempt = " << station->m_minstrelTable[station->m_txrate].numRateAttempt << ", success = " << station->m_minstrelTable[station->m_txrate].numRateSuccess << " (after update).");

  UpdateRetry (station);

  station->m_packetCount++;

  if (m_nsupported >= 1)
    {
      station->m_txrate = FindRate (station);
    }
}

void
MinstrelWifiManager::DoReportFinalDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (st);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *) st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  NS_LOG_DEBUG ("DoReportFinalDataFailed m_txrate = " << station->m_txrate << ", attempt = " << station->m_minstrelTable[station->m_txrate].numRateAttempt << ", success = " << station->m_minstrelTable[station->m_txrate].numRateSuccess << " (before update).");

  station->m_isSampling = false;
  station->m_sampleRateSlower = false;

  UpdateRetry (station);

  station->m_err++;

  NS_LOG_DEBUG ("DoReportFinalDataFailed m_txrate = " << station->m_txrate << ", attempt = " << station->m_minstrelTable[station->m_txrate].numRateAttempt << ", success = " << station->m_minstrelTable[station->m_txrate].numRateSuccess << " (after update).");

  if (m_nsupported >= 1)
    {
      station->m_txrate = FindRate (station);
    }
}

void
MinstrelWifiManager::UpdateRetry (MinstrelWifiRemoteStation *station)
{
  station->m_retry = station->m_shortRetry + station->m_longRetry;
  station->m_shortRetry = 0;
  station->m_longRetry = 0;
}

WifiTxVector
MinstrelWifiManager::DoGetDataTxVector (WifiRemoteStation *st,
                                        uint32_t size)
{
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *) st;
  if (!station->m_initialized)
    {
      CheckInit (station);

      //start the rate at half way
      station->m_txrate = m_nsupported / 2;
    }
  UpdateStats (station);
  return WifiTxVector (GetSupported (station, station->m_txrate), GetDefaultTxPowerLevel (), GetLongRetryCount (station), GetShortGuardInterval (station), Min (GetNumberOfReceiveAntennas (station),GetNumberOfTransmitAntennas ()), GetNess (station), GetAggregation (station), GetStbc (station));
}

WifiTxVector
MinstrelWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *) st;
  NS_LOG_DEBUG ("DoGetRtsMode m_txrate=" << station->m_txrate);

  return WifiTxVector (GetSupported (station, 0), GetDefaultTxPowerLevel (), GetShortRetryCount (station), GetShortGuardInterval (station), Min (GetNumberOfReceiveAntennas (station),GetNumberOfTransmitAntennas ()), GetNess (station), GetAggregation (station), GetStbc (station));
}

bool
MinstrelWifiManager::DoNeedDataRetransmission (WifiRemoteStation *st, Ptr<const Packet> packet, bool normally)
{
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *)st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return normally;
    }

  if (!station->m_isSampling)
    {
      if (station->m_longRetry > (station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                  station->m_minstrelTable[station->m_maxTpRate2].adjustedRetryCount +
                                  station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount +
                                  station->m_minstrelTable[0].adjustedRetryCount))
        {
          return false;
        }
      else
        {
          return true;
        }
    }
  else
    {
      if (station->m_longRetry > (station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount +
                                  station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
                                  station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount +
                                  station->m_minstrelTable[0].adjustedRetryCount))
        {
          return false;
        }
      else
        {
          return true;
        }
    }
}

bool
MinstrelWifiManager::IsLowLatency (void) const
{
  return true;
}

uint32_t
MinstrelWifiManager::GetNextSample (MinstrelWifiRemoteStation *station)
{
  uint32_t bitrate;
  bitrate = station->m_sampleTable[station->m_index][station->m_col];
  station->m_index++;

  //bookeeping for m_index and m_col variables
  if (station->m_index > (m_nsupported - 2))
    {
      station->m_index = 0;
      station->m_col++;
      if (station->m_col >= m_sampleCol)
        {
          station->m_col = 0;
        }
    }
  return bitrate;
}

uint32_t
MinstrelWifiManager::FindRate (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);

  if ((station->m_sampleCount + station->m_packetCount) == 0)
    {
      return 0;
    }


  uint32_t idx;

  //for determining when to try a sample rate
  int coinFlip = m_uniformRandomVariable->GetInteger (0, 100) % 2;

  /**
   * if we are below the target of look around rate percentage, look around
   * note: do it randomly by flipping a coin instead sampling
   * all at once until it reaches the look around rate
   */
  if ( (((100 * station->m_sampleCount) / (station->m_sampleCount + station->m_packetCount )) < m_lookAroundRate)
       && (coinFlip == 1) )
    {
      NS_LOG_DEBUG ("Using look around rate");
      //now go through the table and find an index rate
      idx = GetNextSample (station);

      /**
       * This if condition is used to make sure that we don't need to use
       * the sample rate it is the same as our current rate
       */
      if (idx != station->m_maxTpRate && idx != station->m_txrate)
        {

          //start sample count
          station->m_sampleCount++;

          //set flag that we are currently sampling
          station->m_isSampling = true;

          //bookeeping for resetting stuff
          if (station->m_packetCount >= 10000)
            {
              station->m_sampleCount = 0;
              station->m_packetCount = 0;
            }

          //error check
          if (idx >= m_nsupported)
            {
              NS_LOG_DEBUG ("ALERT!!! ERROR");
            }

          //set the rate that we're currently sampling
          station->m_sampleRate = idx;

          if (station->m_sampleRate == station->m_maxTpRate)
            {
              station->m_sampleRate = station->m_maxTpRate2;
            }

          //is this rate slower than the current best rate
          station->m_sampleRateSlower =
            (station->m_minstrelTable[idx].perfectTxTime > station->m_minstrelTable[station->m_maxTpRate].perfectTxTime);

          //using the best rate instead
          if (station->m_sampleRateSlower)
            {
              NS_LOG_DEBUG ("The next look around rate is slower than the maximum throughput rate, continue with the maximum throughput rate: " << station->m_maxTpRate << "(" << GetSupported (station, station->m_maxTpRate) << ")");
              idx =  station->m_maxTpRate;
            }
        }
    }
  //continue using the best rate
  else
    {
      NS_LOG_DEBUG ("Continue using the maximum throughput rate: " << station->m_maxTpRate << "(" << GetSupported (station, station->m_maxTpRate) << ")");
      idx = station->m_maxTpRate;
    }

  NS_LOG_DEBUG ("Rate = " << idx << "(" << GetSupported (station, idx) << ")");

  return idx;
}

void
MinstrelWifiManager::UpdateStats (MinstrelWifiRemoteStation *station)
{
  if (Simulator::Now () <  station->m_nextStatsUpdate)
    {
      return;
    }

  if (!station->m_initialized)
    {
      return;
    }
  NS_LOG_FUNCTION (this);
  station->m_nextStatsUpdate = Simulator::Now () + m_updateStats;
  NS_LOG_DEBUG ("Next update at " << station->m_nextStatsUpdate);
  NS_LOG_DEBUG ("Currently using rate: " << station->m_txrate << " (" << GetSupported (station, station->m_txrate) << ")");

  Time txTime;
  uint32_t tempProb;

  NS_LOG_DEBUG ("Index-Rate\t\tAttempt\tSuccess");
  for (uint32_t i = 0; i < m_nsupported; i++)
    {

      //calculate the perfect tx time for this rate
      txTime = station->m_minstrelTable[i].perfectTxTime;

      //just for initialization
      if (txTime.GetMicroSeconds () == 0)
        {
          txTime = Seconds (1);
        }

      NS_LOG_DEBUG (i << " " << GetSupported (station, i) <<
                    "\t" << station->m_minstrelTable[i].numRateAttempt <<
                    "\t" << station->m_minstrelTable[i].numRateSuccess);

      //if we've attempted something
      if (station->m_minstrelTable[i].numRateAttempt)
        {
          /**
           * calculate the probability of success
           * assume probability scales from 0 to 18000
           */
          tempProb = (station->m_minstrelTable[i].numRateSuccess * 18000) / station->m_minstrelTable[i].numRateAttempt;

          //bookeeping
          station->m_minstrelTable[i].prob = tempProb;

          //ewma probability (cast for gcc 3.4 compatibility)
          tempProb = static_cast<uint32_t> (((tempProb * (100 - m_ewmaLevel)) + (station->m_minstrelTable[i].ewmaProb * m_ewmaLevel) ) / 100);

          station->m_minstrelTable[i].ewmaProb = tempProb;

          //calculating throughput
          station->m_minstrelTable[i].throughput = tempProb * (1000000 / txTime.GetMicroSeconds ());

        }

      //bookeeping
      station->m_minstrelTable[i].numRateSuccess = 0;
      station->m_minstrelTable[i].numRateAttempt = 0;

      //Sample less often below 10% and  above 95% of success
      if ((station->m_minstrelTable[i].ewmaProb > 17100) || (station->m_minstrelTable[i].ewmaProb < 1800))
        {
          /**
           * See: http://wireless.kernel.org/en/developers/Documentation/mac80211/RateControl/minstrel/
           *
           * Analysis of information showed that the system was sampling too hard at some rates.
           * For those rates that never work (54mb, 500m range) there is no point in sending 10 sample packets (< 6 ms time).
           * Consequently, for the very very low probability rates, we sample at most twice.
           */
          if (station->m_minstrelTable[i].retryCount > 2)
            {
              station->m_minstrelTable[i].adjustedRetryCount = 2;
            }
          else
            {
              station->m_minstrelTable[i].adjustedRetryCount = station->m_minstrelTable[i].retryCount;
            }
        }
      else
        {
          station->m_minstrelTable[i].adjustedRetryCount = station->m_minstrelTable[i].retryCount;
        }

      //if it's 0 allow one retry limit
      if (station->m_minstrelTable[i].adjustedRetryCount == 0)
        {
          station->m_minstrelTable[i].adjustedRetryCount = 1;
        }
    }

  NS_LOG_DEBUG ("Attempt/success resetted to 0");

  uint32_t max_prob = 0, index_max_prob = 0, max_tp = 0, index_max_tp = 0, index_max_tp2 = 0;

  //go find max throughput, second maximum throughput, high probability succ
  NS_LOG_DEBUG ("Finding the maximum throughput, second maximum throughput, and highest probability");
  NS_LOG_DEBUG ("Index-Rate\t\tT-put\tEWMA");
  for (uint32_t i = 0; i < m_nsupported; i++)
    {
      NS_LOG_DEBUG (i << " " << GetSupported (station, i) <<
                    "\t" << station->m_minstrelTable[i].throughput <<
                    "\t" << station->m_minstrelTable[i].ewmaProb);

      if (max_tp < station->m_minstrelTable[i].throughput)
        {
          index_max_tp = i;
          max_tp = station->m_minstrelTable[i].throughput;
        }

      if (max_prob < station->m_minstrelTable[i].ewmaProb)
        {
          index_max_prob = i;
          max_prob = station->m_minstrelTable[i].ewmaProb;
        }
    }


  max_tp = 0;
  //find the second highest max
  for (uint32_t i = 0; i < m_nsupported; i++)
    {
      if ((i != index_max_tp) && (max_tp < station->m_minstrelTable[i].throughput))
        {
          index_max_tp2 = i;
          max_tp = station->m_minstrelTable[i].throughput;
        }
    }

  station->m_maxTpRate = index_max_tp;
  station->m_maxTpRate2 = index_max_tp2;
  station->m_maxProbRate = index_max_prob;

  if (index_max_tp > station->m_txrate)
    {
      station->m_txrate = index_max_tp;
    }

  NS_LOG_DEBUG ("max throughput=" << index_max_tp << "(" << GetSupported (station, index_max_tp) <<
                ")\tsecond max throughput=" << index_max_tp2 << "(" << GetSupported (station, index_max_tp2) <<
                ")\tmax prob=" << index_max_prob << "(" << GetSupported (station, index_max_prob) << ")");
}

void
MinstrelWifiManager::RateInit (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (station);

  for (uint32_t i = 0; i < m_nsupported; i++)
    {
      NS_LOG_DEBUG ("Initializing rate index " << i << " " << GetSupported (station, i));
      station->m_minstrelTable[i].numRateAttempt = 0;
      station->m_minstrelTable[i].numRateSuccess = 0;
      station->m_minstrelTable[i].prob = 0;
      station->m_minstrelTable[i].ewmaProb = 0;
      station->m_minstrelTable[i].throughput = 0;
      station->m_minstrelTable[i].perfectTxTime = GetCalcTxTime (GetSupported (station, i));
      NS_LOG_DEBUG (" perfectTxTime = " << station->m_minstrelTable[i].perfectTxTime);
      station->m_minstrelTable[i].retryCount = 1;
      station->m_minstrelTable[i].adjustedRetryCount = 1;
      //Emulating minstrel.c::ath_rate_ctl_reset
      //We only check from 2 to 10 retries. This guarantee that
      //at least one retry is permitter.
      Time totalTxTimeWithGivenRetries = Seconds (0.0); //tx_time in minstrel.c
      NS_LOG_DEBUG (" Calculating the number of retries");
      for (uint32_t retries = 2; retries < 11; retries++)
        {
          NS_LOG_DEBUG ("  Checking " << retries << " retries");
          totalTxTimeWithGivenRetries = CalculateTimeUnicastPacket (station->m_minstrelTable[i].perfectTxTime, 0, retries);
          NS_LOG_DEBUG ("   totalTxTimeWithGivenRetries = " << totalTxTimeWithGivenRetries);
          if (totalTxTimeWithGivenRetries > MilliSeconds (6))
            {
              break;
            }
          station->m_minstrelTable[i].retryCount = retries;
          station->m_minstrelTable[i].adjustedRetryCount = retries;
        }
    }
}

Time
MinstrelWifiManager::CalculateTimeUnicastPacket (Time dataTransmissionTime, uint32_t shortRetries, uint32_t longRetries)
{
  NS_LOG_FUNCTION (this << dataTransmissionTime << shortRetries << longRetries);
  //See rc80211_minstrel.c

  //First transmission (DATA + ACK timeout)
  Time tt = dataTransmissionTime + GetMac ()->GetAckTimeout ();

  uint32_t cwMax = 1023;
  uint32_t cw = 31;
  for (uint32_t retry = 0; retry < longRetries; retry++)
    {
      //Add one re-transmission (DATA + ACK timeout)
      tt += dataTransmissionTime + GetMac ()->GetAckTimeout ();

      //Add average back off (half the current contention window)
      tt += NanoSeconds ((cw / 2) * GetMac ()->GetSlot ());

      //Update contention window
      cw = std::min (cwMax, (cw + 1) * 2);
    }

  return tt;
}

void
MinstrelWifiManager::InitSampleTable (MinstrelWifiRemoteStation *station)
{
  NS_LOG_DEBUG ("InitSampleTable=" << this);

  station->m_col = station->m_index = 0;

  //for off-seting to make rates fall between 0 and numrates
  uint32_t numSampleRates = m_nsupported;

  uint32_t newIndex;
  for (uint32_t col = 0; col < m_sampleCol; col++)
    {
      for (uint32_t i = 0; i < numSampleRates; i++ )
        {
          /**
           * The next two lines basically tries to generate a random number
           * between 0 and the number of available rates
           */
          int uv = m_uniformRandomVariable->GetInteger (0, numSampleRates);
          newIndex = (i + uv) % numSampleRates;

          //this loop is used for filling in other uninitilized places
          while (station->m_sampleTable[newIndex][col] != 0)
            {
              newIndex = (newIndex + 1) % m_nsupported;
            }
          station->m_sampleTable[newIndex][col] = i;
        }
    }
}

void
MinstrelWifiManager::PrintSampleTable (MinstrelWifiRemoteStation *station)
{
  NS_LOG_DEBUG ("PrintSampleTable=" << station);

  uint32_t numSampleRates = m_nsupported;
  std::stringstream table;
  for (uint32_t i = 0; i < numSampleRates; i++)
    {
      for (uint32_t j = 0; j < m_sampleCol; j++)
        {
          table << station->m_sampleTable[i][j] << "\t";
        }
      table << std::endl;
    }
  NS_LOG_DEBUG (table.str ());
}

void
MinstrelWifiManager::PrintTable (MinstrelWifiRemoteStation *station)
{
  NS_LOG_DEBUG ("PrintTable=" << station);

  for (uint32_t i = 0; i < m_nsupported; i++)
    {
      NS_LOG_DEBUG (i << " (" << GetSupported (station, i) << "): "  << station->m_minstrelTable[i].perfectTxTime << ", retryCount = " << station->m_minstrelTable[i].retryCount << ", adjustedRetryCount = " << station->m_minstrelTable[i].adjustedRetryCount);
    }
}

} //namespace ns3
