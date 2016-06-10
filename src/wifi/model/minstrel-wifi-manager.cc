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
 *         Matías Richart <mrichart@fing.edu.uy>
 *
 * Some Comments:
 *
 * 1) Segment Size is declared for completeness but not used  because it has
 *    to do more with the requirement of the specific hardware.
 *
 * 2) By default, Minstrel applies the multi-rate retry (the core of Minstrel
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
#include "ns3/boolean.h"
#include "ns3/wifi-mac.h"
#include "ns3/assert.h"
#include <vector>
#include <iomanip>

#define Min(a,b) ((a < b) ? a : b)

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MinstrelWifiManager");

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
                   UintegerValue (10),
                   MakeUintegerAccessor (&MinstrelWifiManager::m_sampleCol),
                   MakeUintegerChecker <uint32_t> ())
    .AddAttribute ("PacketLength",
                   "The packet length used for calculating mode TxTime",
                   UintegerValue (1200),
                   MakeUintegerAccessor (&MinstrelWifiManager::m_pktLen),
                   MakeUintegerChecker <uint32_t> ())
    .AddAttribute ("PrintStats",
                   "Print statistics table",
                   BooleanValue (false),
                   MakeBooleanAccessor (&MinstrelWifiManager::m_printStats),
                   MakeBooleanChecker ())
  ;
  return tid;
}

MinstrelWifiManager::MinstrelWifiManager ()
{
  NS_LOG_FUNCTION (this);
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
}

MinstrelWifiManager::~MinstrelWifiManager ()
{
  NS_LOG_FUNCTION (this);
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
      AddCalcTxTime (mode, phy->CalculateTxDuration (m_pktLen, txVector, WIFI_PREAMBLE_LONG, phy->GetFrequency ()));
    }
  WifiRemoteStationManager::SetupPhy (phy);
}

void
MinstrelWifiManager::SetupMac (Ptr<WifiMac> mac)
{
  NS_LOG_FUNCTION (this << mac);
  WifiRemoteStationManager::SetupMac (mac);
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
  NS_LOG_FUNCTION (this << mode);
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
  NS_LOG_FUNCTION (this << mode << t);
  m_calcTxTime.push_back (std::make_pair (t, mode));
}

WifiRemoteStation *
MinstrelWifiManager::DoCreateStation (void) const
{
  NS_LOG_FUNCTION (this);
  MinstrelWifiRemoteStation *station = new MinstrelWifiRemoteStation ();

  station->m_nextStatsUpdate = Simulator::Now () + m_updateStats;
  station->m_col = 0;
  station->m_index = 0;
  station->m_maxTpRate = 0;
  station->m_maxTpRate2 = 0;
  station->m_maxProbRate = 0;
  station->m_nModes = 0;
  station->m_totalPacketsCount = 0;
  station->m_samplePacketsCount = 0;
  station->m_isSampling = false;
  station->m_sampleRate = 0;
  station->m_sampleDeferred = false;
  station->m_shortRetry = 0;
  station->m_longRetry = 0;
  station->m_retry = 0;
  station->m_txrate = 0;
  station->m_initialized = false;

  return station;
}

void
MinstrelWifiManager::CheckInit (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  if (!station->m_initialized && GetNSupported (station) > 1)
    {
      //Note: we appear to be doing late initialization of the table
      //to make sure that the set of supported rates has been initialized
      //before we perform our own initialization.
      station->m_nModes = GetNSupported (station);
      station->m_minstrelTable = MinstrelRate (station->m_nModes);
      station->m_sampleTable = SampleRate (station->m_nModes, std::vector<uint32_t> (m_sampleCol));
      InitSampleTable (station);
      RateInit (station);
      station->m_initialized = true;
      std::ostringstream tmp;
      tmp << "minstrel-stats-" << station->m_state->m_address << ".txt";
      station->m_statsFile.open (tmp.str ().c_str (), std::ios::out);
    }
}

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
 * After failing max retry times, DoReportFinalDataFailed will be called
 */
void
MinstrelWifiManager::UpdateRate (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  station->m_longRetry++;
  station->m_minstrelTable[station->m_txrate].numRateAttempt++;

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
      if (station->m_sampleDeferred)
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

WifiTxVector
MinstrelWifiManager::GetDataTxVector (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  uint32_t channelWidth = GetChannelWidth (station);
  if (channelWidth > 20 && channelWidth != 22)
    {
      //avoid to use legacy rate adaptation algorithms for IEEE 802.11n/ac
      channelWidth = 20;
    }
  if (!station->m_initialized)
    {
      CheckInit (station);

      //start the rate at half way
      station->m_txrate = station->m_nModes / 2;
    }
  return WifiTxVector (GetSupported (station, station->m_txrate), GetDefaultTxPowerLevel (), GetLongRetryCount (station), false, 1, 0, channelWidth, GetAggregation (station), false);
}

WifiTxVector
MinstrelWifiManager::GetRtsTxVector (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  NS_LOG_DEBUG ("DoGetRtsMode m_txrate=" << station->m_txrate);
  uint32_t channelWidth = GetChannelWidth (station);
  if (channelWidth > 20 && channelWidth != 22)
    {
      //avoid to use legacy rate adaptation algorithms for IEEE 802.11n/ac
      channelWidth = 20;
    }
  WifiTxVector rtsTxVector;
  if (GetUseNonErpProtection () == false)
    {
      rtsTxVector = WifiTxVector (GetSupported (station, 0), GetDefaultTxPowerLevel (), GetShortRetryCount (station), false, 1, 0, channelWidth, GetAggregation (station), false);
    }
  else
    {
      rtsTxVector = WifiTxVector (GetNonErpSupported (station, 0), GetDefaultTxPowerLevel (), GetShortRetryCount (station), false, 1, 0, channelWidth, GetAggregation (station), false);
    }
  return rtsTxVector;
}

uint32_t
MinstrelWifiManager::CountRetries (MinstrelWifiRemoteStation *station)
{
  if (!station->m_isSampling)
    {
      return station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
             station->m_minstrelTable[station->m_maxTpRate2].adjustedRetryCount +
             station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount +
             station->m_minstrelTable[0].adjustedRetryCount;
    }
  else
    {
      return station->m_minstrelTable[station->m_sampleRate].adjustedRetryCount +
             station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount +
             station->m_minstrelTable[station->m_maxProbRate].adjustedRetryCount +
             station->m_minstrelTable[0].adjustedRetryCount;
    }
}

uint32_t
MinstrelWifiManager::FindRate (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);

  if (station->m_totalPacketsCount == 0)
    {
      return 0;
    }


  uint32_t idx;

  int delta = (station->m_totalPacketsCount * m_lookAroundRate / 100) - (station->m_samplePacketsCount + station->m_numSamplesDeferred / 2);

  NS_LOG_DEBUG ("Decide sampling. Delta: " << delta << " lookAroundRatio: " <<  m_lookAroundRate);

  /* delta < 0: no sampling required */
  if (delta >= 0)
    {
      NS_LOG_DEBUG ("Search next sampling rate");
      int ratesSupported = station->m_nModes;
      if (delta > ratesSupported * 2)
        {
          /* From Linux implementation:
           * With multi-rate retry, not every planned sample
           * attempt actually gets used, due to the way the retry
           * chain is set up - [max_tp,sample,prob,lowest] for
           * sample_rate < max_tp.
           *
           * If there's too much sampling backlog and the link
           * starts getting worse, minstrel would start bursting
           * out lots of sampling frames, which would result
           * in a large throughput loss. */
          station->m_samplePacketsCount += (delta - ratesSupported * 2);
        }

      //now go through the table and find an index rate
      idx = GetNextSample (station);

      //error check
      if (idx >= station->m_nModes)
        {
          NS_LOG_DEBUG ("ALERT!!! ERROR");
        }

      //set the rate that we're currently sampling
      station->m_sampleRate = idx;

      /* From Linux implementation:
       * Decide if direct ( 1st mrr stage) or indirect (2nd mrr stage)
       * rate sampling method should be used.
       * Respect such rates that are not sampled for 20 iterations.
       */
      if ((station->m_minstrelTable[idx].perfectTxTime > station->m_minstrelTable[station->m_maxTpRate].perfectTxTime)
          && (station->m_minstrelTable[idx].numSamplesSkipped < 20))
        {
          // If the rate is slower and we have sample it enough, defer to second stage
          station->m_sampleDeferred = true;
          station->m_numSamplesDeferred++;

          //set flag that we are currently sampling
          station->m_isSampling = true;
        }
      else
        {
          // if samplieLimit is zero, then don't sample this rate
          if (!station->m_minstrelTable[idx].sampleLimit)
            {
              idx =  station->m_maxTpRate;
              station->m_isSampling = false;
            }
          else
            {
              //set flag that we are currently sampling
              station->m_isSampling = true;
              if (station->m_minstrelTable[idx].sampleLimit > 0)
                {
                  station->m_minstrelTable[idx].sampleLimit--;
                }
            }
        }

      //using the best rate instead
      if (station->m_sampleDeferred)
        {
          NS_LOG_DEBUG ("The next look around rate is slower than the maximum throughput rate, continue with the maximum throughput rate: " << station->m_maxTpRate << "(" << GetSupported (station, station->m_maxTpRate) << ")");
          idx =  station->m_maxTpRate;
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
  NS_LOG_FUNCTION (this << station);
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
  for (uint32_t i = 0; i < station->m_nModes; i++)
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
          station->m_minstrelTable[i].numSamplesSkipped = 0;
          /**
           * calculate the probability of success
           * assume probability scales from 0 to 18000
           */
          tempProb = (station->m_minstrelTable[i].numRateSuccess * 18000) / station->m_minstrelTable[i].numRateAttempt;

          //bookeeping
          station->m_minstrelTable[i].prob = tempProb;

          if (station->m_minstrelTable[i].successHist == 0)
            {
              station->m_minstrelTable[i].ewmaProb = tempProb;
            }
          else
            {
              //ewma probability (cast for gcc 3.4 compatibility)
              tempProb = static_cast<uint32_t> (((tempProb * (100 - m_ewmaLevel)) + (station->m_minstrelTable[i].ewmaProb * m_ewmaLevel) ) / 100);

              station->m_minstrelTable[i].ewmaProb = tempProb;
            }

          //calculating throughput
          station->m_minstrelTable[i].throughput = tempProb * (1000000 / txTime.GetMicroSeconds ());

        }
      else
        {
          station->m_minstrelTable[i].numSamplesSkipped++;
        }

      //bookeeping
      station->m_minstrelTable[i].successHist += station->m_minstrelTable[i].numRateSuccess;
      station->m_minstrelTable[i].attemptHist += station->m_minstrelTable[i].numRateAttempt;
      station->m_minstrelTable[i].prevNumRateSuccess = station->m_minstrelTable[i].numRateSuccess;
      station->m_minstrelTable[i].prevNumRateAttempt = station->m_minstrelTable[i].numRateAttempt;
      station->m_minstrelTable[i].numRateSuccess = 0;
      station->m_minstrelTable[i].numRateAttempt = 0;

      //Sample less often below 10% and  above 95% of success
      if ((station->m_minstrelTable[i].ewmaProb > 17100) || (station->m_minstrelTable[i].ewmaProb < 1800))
        {
          /**
           * See: http://wireless.kernel.org/en/developers/Documentation/mac80211/RateControl/minstrel/
           *
           * Analysis of information showed that the system was sampling too hard at some rates.
           * For those rates that never work (54mb, 500m range) there is no point in retrying 10 sample packets (< 6 ms time).
           * Consequently, for the very low probability rates, we try at most twice when fails and not sample more than 4 times.
           */
          if (station->m_minstrelTable[i].retryCount > 2)
            {
              station->m_minstrelTable[i].adjustedRetryCount = 2;
            }
          station->m_minstrelTable[i].sampleLimit = 4;
        }
      else
        {
          // no sampling limit.
          station->m_minstrelTable[i].sampleLimit = -1;
          station->m_minstrelTable[i].adjustedRetryCount = station->m_minstrelTable[i].retryCount;
        }

      //if it's 0 allow two retries.
      if (station->m_minstrelTable[i].adjustedRetryCount == 0)
        {
          station->m_minstrelTable[i].adjustedRetryCount = 2;
        }
    }

  NS_LOG_DEBUG ("Attempt/success resetted to 0");

  uint32_t max_tp = 0, index_max_tp = 0, index_max_tp2 = 0;

  //go find max throughput, second maximum throughput, high probability succ
  NS_LOG_DEBUG ("Finding the maximum throughput, second maximum throughput, and highest probability");
  NS_LOG_DEBUG ("Index-Rate\t\tT-put\tEWMA");
  for (uint32_t i = 0; i < station->m_nModes; i++)
    {
      NS_LOG_DEBUG (i << " " << GetSupported (station, i) <<
                    "\t" << station->m_minstrelTable[i].throughput <<
                    "\t" << station->m_minstrelTable[i].ewmaProb);

      if (max_tp < station->m_minstrelTable[i].throughput)
        {
          index_max_tp = i;
          max_tp = station->m_minstrelTable[i].throughput;
        }
    }

  max_tp = 0;
  //find the second highest max
  for (uint32_t i = 0; i < station->m_nModes; i++)
    {
      if ((i != index_max_tp) && (max_tp < station->m_minstrelTable[i].throughput))
        {
          index_max_tp2 = i;
          max_tp = station->m_minstrelTable[i].throughput;
        }
    }

  uint32_t max_prob = 0, index_max_prob = 0;
  for (uint32_t i = 0; i < station->m_nModes; i++)
    {
      if ((station->m_minstrelTable[i].ewmaProb >= 95 * 180) && (station->m_minstrelTable[i].throughput >= station->m_minstrelTable[index_max_prob].throughput))
        {
          index_max_prob = i;
          max_prob = station->m_minstrelTable[i].ewmaProb;
        }
      else if (station->m_minstrelTable[i].ewmaProb >= max_prob)
        {
          index_max_prob = i;
          max_prob = station->m_minstrelTable[i].ewmaProb;
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
  if (m_printStats)
    {
      PrintTable (station);
    }
}

void
MinstrelWifiManager::DoReportRxOk (WifiRemoteStation *st,
                                   double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << st << rxSnr << txMode);
}

void
MinstrelWifiManager::DoReportRtsFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *)st;
  NS_LOG_DEBUG ("DoReportRtsFailed m_txrate=" << station->m_txrate);

  station->m_shortRetry++;
}

void
MinstrelWifiManager::DoReportRtsOk (WifiRemoteStation *st, double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << st);
  NS_LOG_DEBUG ("self=" << st << " rts ok");
}

void
MinstrelWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *)st;
  UpdateRetry (station);
}

void
MinstrelWifiManager::DoReportDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *)st;
  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  UpdateRate (station);
}

void
MinstrelWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                     double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (st << ackSnr << ackMode << dataSnr);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *) st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  NS_LOG_DEBUG ("DoReportDataOk m_txrate = " << station->m_txrate << ", attempt = " << station->m_minstrelTable[station->m_txrate].numRateAttempt << ", success = " << station->m_minstrelTable[station->m_txrate].numRateSuccess << " (before update).");

  station->m_minstrelTable[station->m_txrate].numRateSuccess++;
  station->m_minstrelTable[station->m_txrate].numRateAttempt++;

  UpdatePacketCounters (station);

  NS_LOG_DEBUG ("DoReportDataOk m_txrate = " << station->m_txrate << ", attempt = " << station->m_minstrelTable[station->m_txrate].numRateAttempt << ", success = " << station->m_minstrelTable[station->m_txrate].numRateSuccess << " (after update).");

  UpdateRetry (station);
  UpdateStats (station);

  if (station->m_nModes >= 1)
    {
      station->m_txrate = FindRate (station);
    }
}

void
MinstrelWifiManager::DoReportFinalDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *) st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  NS_LOG_DEBUG ("DoReportFinalDataFailed m_txrate = " << station->m_txrate << ", attempt = " << station->m_minstrelTable[station->m_txrate].numRateAttempt << ", success = " << station->m_minstrelTable[station->m_txrate].numRateSuccess << " (before update).");

  UpdatePacketCounters (station);

  UpdateRetry (station);
  UpdateStats (station);

  NS_LOG_DEBUG ("DoReportFinalDataFailed m_txrate = " << station->m_txrate << ", attempt = " << station->m_minstrelTable[station->m_txrate].numRateAttempt << ", success = " << station->m_minstrelTable[station->m_txrate].numRateSuccess << " (after update).");

  if (station->m_nModes >= 1)
    {
      station->m_txrate = FindRate (station);
    }
}

void
MinstrelWifiManager::UpdatePacketCounters (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);

  station->m_totalPacketsCount++;
  // If it is a sampling frame and the sampleRate was used, increase counter
  if (station->m_isSampling && (!station->m_sampleDeferred || station->m_longRetry >= station->m_minstrelTable[station->m_maxTpRate].adjustedRetryCount))
    {
      station->m_samplePacketsCount++;
    }

  if (station->m_numSamplesDeferred > 0)
    {
      station->m_numSamplesDeferred--;
    }

  if (station->m_totalPacketsCount == ~0)
    {
      station->m_numSamplesDeferred = 0;
      station->m_samplePacketsCount = 0;
      station->m_totalPacketsCount = 0;
    }
  station->m_isSampling = false;
  station->m_sampleDeferred = false;
}

void
MinstrelWifiManager::UpdateRetry (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  station->m_retry = station->m_shortRetry + station->m_longRetry;
  station->m_shortRetry = 0;
  station->m_longRetry = 0;
}

WifiTxVector
MinstrelWifiManager::DoGetDataTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *) st;
  return GetDataTxVector (station);
}

WifiTxVector
MinstrelWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *) st;
  return GetRtsTxVector (station);
}

bool
MinstrelWifiManager::DoNeedDataRetransmission (WifiRemoteStation *st, Ptr<const Packet> packet, bool normally)
{
  NS_LOG_FUNCTION (this << st << packet << normally);
  MinstrelWifiRemoteStation *station = (MinstrelWifiRemoteStation *)st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return normally;
    }
  if (station->m_longRetry > CountRetries (station))
    {
      return false;
    }
  else
    {
      return true;
    }
}

bool
MinstrelWifiManager::IsLowLatency (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

uint32_t
MinstrelWifiManager::GetNextSample (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  uint32_t bitrate;
  bitrate = station->m_sampleTable[station->m_index][station->m_col];
  station->m_index++;

  //bookeeping for m_index and m_col variables
  if (station->m_index > (station->m_nModes - 2))
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

void
MinstrelWifiManager::RateInit (MinstrelWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (station);

  for (uint32_t i = 0; i < station->m_nModes; i++)
    {
      NS_LOG_DEBUG ("Initializing rate index " << i << " " << GetSupported (station, i));
      station->m_minstrelTable[i].numRateAttempt = 0;
      station->m_minstrelTable[i].numRateSuccess = 0;
      station->m_minstrelTable[i].prevNumRateSuccess = 0;
      station->m_minstrelTable[i].prevNumRateAttempt = 0;
      station->m_minstrelTable[i].successHist = 0;
      station->m_minstrelTable[i].attemptHist = 0;
      station->m_minstrelTable[i].numSamplesSkipped = 0;
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
          station->m_minstrelTable[i].sampleLimit = -1;
          station->m_minstrelTable[i].retryCount = retries;
          station->m_minstrelTable[i].adjustedRetryCount = retries;
        }
    }
  UpdateStats (station);
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

  //for off-setting to make rates fall between 0 and nModes
  uint32_t numSampleRates = station->m_nModes;

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

          //this loop is used for filling in other uninitialized places
          while (station->m_sampleTable[newIndex][col] != 0)
            {
              newIndex = (newIndex + 1) % station->m_nModes;
            }
          station->m_sampleTable[newIndex][col] = i;
        }
    }
}

void
MinstrelWifiManager::PrintSampleTable (MinstrelWifiRemoteStation *station)
{
  NS_LOG_DEBUG ("PrintSampleTable=" << station);

  uint32_t numSampleRates = station->m_nModes;
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
  NS_LOG_FUNCTION (this << station);
  NS_LOG_DEBUG ("PrintTable=" << station);

  station->m_statsFile << "best   _______________rate________________    ________statistics________    ________last_______    ______sum-of________\n" <<
    "rate  [      name       idx airtime max_tp]  [avg(tp) avg(prob) sd(prob)]  [prob.|retry|suc|att]  [#success | #attempts]\n";

  uint32_t maxTpRate = station->m_maxTpRate;
  uint32_t maxTpRate2 = station->m_maxTpRate2;
  uint32_t maxProbRate = station->m_maxProbRate;

  for (uint32_t i = 0; i < station->m_nModes; i++)
    {
      RateInfo rate = station->m_minstrelTable[i];

      if (i == maxTpRate)
        {
          station->m_statsFile << 'A';
        }
      else
        {
          station->m_statsFile << ' ';
        }
      if (i == maxTpRate2)
        {
          station->m_statsFile << 'B';
        }
      else
        {
          station->m_statsFile << ' ';
        }
      if (i == maxProbRate)
        {
          station->m_statsFile << 'P';
        }
      else
        {
          station->m_statsFile << ' ';
        }

      float tmpTh = rate.throughput / 100000.0;
      station->m_statsFile << "   " <<
        std::setw (17) << GetSupported (station, i) << "  " <<
        std::setw (2) << i << "  " <<
        std::setw (4) << rate.perfectTxTime.GetMicroSeconds () <<
        std::setw (8) << "    -----    " <<
        std::setw (8) << tmpTh << "    " <<
        std::setw (3) << rate.ewmaProb / 180 <<
        std::setw (3) << "       ---      " <<
        std::setw (3) << rate.prob / 180 << "     " <<
        std::setw (1) << rate.adjustedRetryCount << "   " <<
        std::setw (3) << rate.prevNumRateSuccess << " " <<
        std::setw (3) << rate.prevNumRateAttempt << "   " <<
        std::setw (9) << rate.successHist << "   " <<
        std::setw (9) << rate.attemptHist << "\n";
    }
  station->m_statsFile << "\nTotal packet count:    ideal " << station->m_totalPacketsCount - station->m_samplePacketsCount
                       << "      lookaround " << station->m_samplePacketsCount << "\n\n";

  station->m_statsFile.flush ();
}


void
MinstrelWifiManager::SetHtSupported (bool enable)
{
  //HT is not supported by this algorithm.
  if (enable)
    {
      NS_FATAL_ERROR ("WifiRemoteStationManager selected does not support HT rates");
    }
}

void
MinstrelWifiManager::SetVhtSupported (bool enable)
{
  //VHT is not supported by this algorithm.
  if (enable)
    {
      NS_FATAL_ERROR ("WifiRemoteStationManager selected does not support VHT rates");
    }
}

} //namespace ns3
