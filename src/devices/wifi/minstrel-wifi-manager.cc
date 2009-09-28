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
#include "ns3/random-variable.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/wifi-mac.h"
#include "ns3/assert.h"
#include <vector>

NS_LOG_COMPONENT_DEFINE ("MinstrelWifiManager");


namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MinstrelWifiManager);

TypeId
MinstrelWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MinstrelWifiManager")
    .SetParent<WifiRemoteStationManager> ()
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
    .AddAttribute ("SegmentSize",
                   "The largest allowable segment size packet",
                   DoubleValue (6000),
                   MakeDoubleAccessor (&MinstrelWifiManager::m_segmentSize),
                   MakeDoubleChecker <double> ())
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
{}

MinstrelWifiManager::~MinstrelWifiManager ()
{}

void
MinstrelWifiManager::SetupPhy (Ptr<WifiPhy> phy)
{
  uint32_t nModes = phy->GetNModes ();
  for (uint32_t i = 0; i < nModes; i++)
    {
      WifiMode mode = phy->GetMode (i);
      AddCalcTxTime (mode, phy->CalculateTxDuration (m_pktLen, mode, WIFI_PREAMBLE_LONG));
    }
  WifiRemoteStationManager::SetupPhy (phy);
}

WifiRemoteStation *
MinstrelWifiManager::CreateStation (void)
{
  return new MinstrelWifiRemoteStation (this);
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

MinstrelWifiRemoteStation::MinstrelWifiRemoteStation (Ptr<MinstrelWifiManager> stations)
  :m_stations (stations),
  m_nextStatsUpdate (Simulator::Now () + stations->m_updateStats),
  m_col (0), 
  m_index (0), 
  m_maxTpRate (0), 
  m_maxTpRate2 (0), 
  m_maxProbRate (0),
  m_packetCount (0), 
  m_sampleCount (0), 
  m_isSampling (false), 
  m_sampleRate (0), 
  m_sampleRateSlower (false),
  m_currentRate (0),
  m_shortRetry (0),
  m_longRetry (0),
  m_retry (0),
  m_err (0),
  m_txrate (0),
  m_initialized (false)
{}

MinstrelWifiRemoteStation::~MinstrelWifiRemoteStation ()
{}

void 
MinstrelWifiRemoteStation::CheckInit(void)
{
  if (!m_initialized)
    {
      m_minstrelTable  =  MinstrelRate(GetNSupportedModes ());
      m_sampleTable = SampleRate(GetNSupportedModes (), std::vector<uint32_t> (m_stations->m_sampleCol));
      InitSampleTable ();
      RateInit ();
      m_initialized = true;
    }
}

void
MinstrelWifiRemoteStation::DoReportRxOk (double rxSnr, WifiMode txMode)
{
  NS_LOG_DEBUG("DoReportRxOk m_txrate=" << m_txrate);
}

void
MinstrelWifiRemoteStation::DoReportRtsFailed (void)
{
  NS_LOG_DEBUG("DoReportRtsFailed m_txrate=" << m_txrate);

  m_shortRetry++;
}

void
MinstrelWifiRemoteStation::DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_DEBUG ("self="<<this<<" rts ok");
}

void
MinstrelWifiRemoteStation::DoReportFinalRtsFailed (void)
{
  UpdateRetry ();
  m_err++;
}

void
MinstrelWifiRemoteStation::DoReportDataFailed (void)
{
  CheckInit();

  m_longRetry++;

  NS_LOG_DEBUG ("DoReportDataFailed " << this << "\t rate " << m_txrate << "\tlongRetry \t" << m_longRetry);

  /// for normal rate, we're not currently sampling random rates
  if (!m_isSampling)
    {
      /// use best throughput rate
      if( m_longRetry < m_minstrelTable[m_txrate].adjustedRetryCount)
        {
          ;  ///<  there's still a few retries left
        }

      /// use second best throughput rate
      else if (m_longRetry <= (m_minstrelTable[m_txrate].adjustedRetryCount +  
               m_minstrelTable[m_maxTpRate].adjustedRetryCount))
        {
          m_txrate = m_maxTpRate2;
        }

      /// use best probability rate
      else if (m_longRetry <= (m_minstrelTable[m_txrate].adjustedRetryCount +  
               m_minstrelTable[m_maxTpRate2].adjustedRetryCount + 
               m_minstrelTable[m_maxTpRate].adjustedRetryCount))
        {
          m_txrate = m_maxProbRate;
        }

      /// use lowest base rate	
      else if (m_longRetry > (m_minstrelTable[m_txrate].adjustedRetryCount +  
               m_minstrelTable[m_maxTpRate2].adjustedRetryCount + 
               m_minstrelTable[m_maxTpRate].adjustedRetryCount))
        {
          m_txrate = 0;
        }
    }

  /// for look-around rate, we're currently sampling random rates
  else
    {
      /// current sampling rate is slower than the current best rate
      if (m_sampleRateSlower)
        {
          /// use best throughput rate
          if (m_longRetry < m_minstrelTable[m_txrate].adjustedRetryCount)
            {
              ;	///<  there are a few retries left
            }

          ///	use random rate
          else if (m_longRetry <= (m_minstrelTable[m_txrate].adjustedRetryCount + 
                   m_minstrelTable[m_maxTpRate].adjustedRetryCount))
            {
              m_txrate = m_sampleRate;
            }

          /// use max probability rate
          else if (m_longRetry <= (m_minstrelTable[m_txrate].adjustedRetryCount +  
                   m_minstrelTable[m_sampleRate].adjustedRetryCount + 
                   m_minstrelTable[m_maxTpRate].adjustedRetryCount ))
            {
              m_txrate = m_maxProbRate;
            }

          /// use lowest base rate
          else if (m_longRetry > (m_minstrelTable[m_txrate].adjustedRetryCount +  
                   m_minstrelTable[m_sampleRate].adjustedRetryCount + 
                   m_minstrelTable[m_maxTpRate].adjustedRetryCount)) 
            {
              m_txrate = 0;
            }
        }

        /// current sampling rate is better than current best rate 
        else
          {
            /// use random rate
            if (m_longRetry < m_minstrelTable[m_txrate].adjustedRetryCount)
              {
                ;  ///< keep using it
              }

            /// use the best rate
            else if (m_longRetry <= m_minstrelTable[m_txrate].adjustedRetryCount + 
                     m_minstrelTable[m_sampleRate].adjustedRetryCount)
              {
                m_txrate = m_maxTpRate;
              }

            /// use the best probability rate
            else if (m_longRetry <= m_minstrelTable[m_txrate].adjustedRetryCount + 
                     m_minstrelTable[m_maxTpRate].adjustedRetryCount +  
                     m_minstrelTable[m_sampleRate].adjustedRetryCount)
              {
                m_txrate = m_maxProbRate;
              }

            /// use the lowest base rate
            else if (m_longRetry > m_minstrelTable[m_txrate].adjustedRetryCount + 
                     m_minstrelTable[m_maxTpRate].adjustedRetryCount +  
                     m_minstrelTable[m_sampleRate].adjustedRetryCount) 
              {
                m_txrate = 0;
              }
          }
    }
}

void
MinstrelWifiRemoteStation::DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  m_isSampling = false;
  m_sampleRateSlower=false;

  CheckInit ();

  m_minstrelTable[m_txrate].numRateSuccess++;
  m_minstrelTable[m_txrate].numRateAttempt++;
	
  UpdateRetry ();

  m_minstrelTable[m_txrate].numRateAttempt += m_retry;
  m_packetCount++;

  if (GetNSupportedModes () >= 1)
    {
      m_txrate = FindRate ();
    }
}

void
MinstrelWifiRemoteStation::DoReportFinalDataFailed (void)
{
  NS_LOG_DEBUG ("DoReportFinalDataFailed m_txrate=" << m_txrate);

  m_isSampling = false;
  m_sampleRateSlower=false;

  UpdateRetry ();

  m_minstrelTable[m_txrate].numRateAttempt += m_retry;
  m_err++;

  if (GetNSupportedModes () >= 1)
    {
      m_txrate = FindRate ();
    }
}

void
MinstrelWifiRemoteStation::UpdateRetry (void)
{
  m_retry = m_shortRetry + m_longRetry;
  m_shortRetry = 0;
  m_longRetry = 0;
}

Ptr<WifiRemoteStationManager>
MinstrelWifiRemoteStation::GetManager (void) const
{
  return m_stations;
}

WifiMode
MinstrelWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  if (!m_initialized)
    {
      CheckInit ();

      /// start the rate at half way
      m_txrate = GetNSupportedModes () / 2;
    }
  UpdateStats ();
  return GetSupportedMode (m_txrate);
}

WifiMode
MinstrelWifiRemoteStation::DoGetRtsMode (void)
{
  NS_LOG_DEBUG ("DoGetRtsMode m_txrate=" << m_txrate);

  return GetSupportedMode (0);
}

uint32_t 
MinstrelWifiRemoteStation::GetNextSample ()
{
  uint32_t bitrate;
  bitrate = m_sampleTable[m_index][m_col];
  m_index++;

  /// bookeeping for m_index and m_col variables
  if (m_index > (GetNSupportedModes () -2)) 
    {
      m_index =0;
      m_col++;
      if (m_col >= m_stations->m_sampleCol)
        {
          m_col = 0;
        }
    }
  return bitrate;
}

uint32_t
MinstrelWifiRemoteStation::FindRate ()
{
  NS_LOG_DEBUG ("FindRate " << "packet=" << m_packetCount );

  if ((m_sampleCount + m_packetCount) == 0)
    {
      return 0;
    }


  uint32_t idx;

  /// for determining when to try a sample rate 
  UniformVariable coinFlip (0, 100);

  /**
   * if we are below the target of look around rate percentage, look around
   * note: do it randomly by flipping a coin instead sampling 
   * all at once until it reaches the look around rate
   */
  if ( (((100* m_sampleCount) / (m_sampleCount + m_packetCount )) < m_stations->m_lookAroundRate) &&
     ((int)coinFlip.GetValue ()) % 2 == 1 )
    {

      /// now go through the table and find an index rate
      idx = GetNextSample();
			
			
      /**
       * This if condition is used to make sure that we don't need to use
       * the sample rate it is the same as our current rate
       */
      if (idx != m_maxTpRate && idx != m_txrate) 
        {
		
          /// start sample count
          m_sampleCount++;

          /// set flag that we are currently sampling
          m_isSampling = true;

          /// bookeeping for resetting stuff
          if (m_packetCount >= 10000) 
            {
              m_sampleCount = 0;
              m_packetCount = 0;
            }

          /// error check
          if (idx >= GetNSupportedModes	() || idx < 0 )
            {
              NS_LOG_DEBUG ("ALERT!!! ERROR");
            }

          /// set the rate that we're currently sampling
          m_sampleRate = idx;

          if (m_sampleRate == m_maxTpRate)
            {
              m_sampleRate = m_maxTpRate2;
            }

          /// is this rate slower than the current best rate
          m_sampleRateSlower = (m_minstrelTable[idx].perfectTxTime > m_minstrelTable[m_maxTpRate].perfectTxTime);

          /// using the best rate instead
          if (m_sampleRateSlower)
            {
              idx =  m_maxTpRate;
            }
        }
			
    } 

  ///	continue using the best rate
  else
    {
      idx = m_maxTpRate; 
    }


  NS_LOG_DEBUG ("FindRate " << "sample rate=" << idx);

  return idx;
}

void
MinstrelWifiRemoteStation::UpdateStats ()
{
  if (Simulator::Now () <  m_nextStatsUpdate)
    {
      return;
    }

  NS_LOG_DEBUG ("Updating stats="<<this);

  m_nextStatsUpdate = Simulator::Now () + m_stations->m_updateStats;

  Time txTime;
  uint32_t tempProb;

  for (uint32_t i =0; i < GetNSupportedModes (); i++)
    {        

      /// calculate the perfect tx time for this rate
      txTime = m_minstrelTable[i].perfectTxTime;       

      /// just for initialization
      if (txTime.GetMicroSeconds () == 0)
        {
          txTime = Seconds (1);
        }

      NS_LOG_DEBUG ("m_txrate=" << m_txrate << "\t attempt=" << m_minstrelTable[i].numRateAttempt << "\t success=" << m_minstrelTable[i].numRateSuccess);

      /// if we've attempted something
      if (m_minstrelTable[i].numRateAttempt)
        {
          /**
           * calculate the probability of success
           * assume probability scales from 0 to 18000
           */
          tempProb = (m_minstrelTable[i].numRateSuccess * 18000) / m_minstrelTable[i].numRateAttempt;

          /// bookeeping
          m_minstrelTable[i].successHist += m_minstrelTable[i].numRateSuccess;
          m_minstrelTable[i].attemptHist += m_minstrelTable[i].numRateAttempt;
          m_minstrelTable[i].prob = tempProb;

          /// ewma probability (cast for gcc 3.4 compatibility)
          tempProb = static_cast<uint32_t>(((tempProb * (100 - m_stations->m_ewmaLevel)) + (m_minstrelTable[i].ewmaProb * m_stations->m_ewmaLevel) )/100);

          m_minstrelTable[i].ewmaProb = tempProb;

          /// calculating throughput
          m_minstrelTable[i].throughput = tempProb * (1000000 / txTime.GetMicroSeconds());

        }

      /// bookeeping
      m_minstrelTable[i].prevNumRateAttempt= m_minstrelTable[i].numRateAttempt;
      m_minstrelTable[i].prevNumRateSuccess = m_minstrelTable[i].numRateSuccess;
      m_minstrelTable[i].numRateSuccess = 0;
      m_minstrelTable[i].numRateAttempt = 0;

      /// Sample less often below 10% and  above 95% of success
      if ((m_minstrelTable[i].ewmaProb > 17100) || (m_minstrelTable[i].ewmaProb < 1800)) 
        {
          /**
           * retry count denotes the number of retries permitted for each rate
           * # retry_count/2
           */
          m_minstrelTable[i].adjustedRetryCount = m_minstrelTable[i].retryCount >> 1;
          if (m_minstrelTable[i].adjustedRetryCount > 2)
            {
              m_minstrelTable[i].adjustedRetryCount = 2 ;
            }
        }
      else
        {
          m_minstrelTable[i].adjustedRetryCount = m_minstrelTable[i].retryCount;
        }

      /// if it's 0 allow one retry limit
      if (m_minstrelTable[i].adjustedRetryCount == 0)
        {
          m_minstrelTable[i].adjustedRetryCount = 1;
        }
    }


  uint32_t max_prob = 0, index_max_prob =0, max_tp =0, index_max_tp=0, index_max_tp2=0;

  /// go find max throughput, second maximum throughput, high probability succ
  for (uint32_t i =0; i < GetNSupportedModes (); i++) 
    {
      NS_LOG_DEBUG ("throughput" << m_minstrelTable[i].throughput << "\n ewma" << m_minstrelTable[i].ewmaProb);

      if (max_tp < m_minstrelTable[i].throughput) 
        {
          index_max_tp = i;
          max_tp = m_minstrelTable[i].throughput;
        }

      if (max_prob < m_minstrelTable[i].ewmaProb) 
        {
          index_max_prob = i;
          max_prob = m_minstrelTable[i].ewmaProb;
        }
    }


  max_tp = 0;
  /// find the second highest max
  for (uint32_t i =0; i < GetNSupportedModes (); i++) 
    {
      if ((i != index_max_tp) && (max_tp < m_minstrelTable[i].throughput))
        {
          index_max_tp2 = i;
          max_tp = m_minstrelTable[i].throughput;
        }
    }

  m_maxTpRate = index_max_tp;
  m_maxTpRate2 = index_max_tp2;
  m_maxProbRate = index_max_prob;
  m_currentRate = index_max_tp;

  if (index_max_tp > m_txrate)
    {
      m_txrate= index_max_tp;
    }

  NS_LOG_DEBUG ("max tp="<< index_max_tp << "\nmax tp2="<< index_max_tp2<< "\nmax prob="<< index_max_prob);

  /// reset it
  RateInit ();
}

void
MinstrelWifiRemoteStation::RateInit ()
{
  NS_LOG_DEBUG ("RateInit="<<this);

  for (uint32_t i = 0; i < GetNSupportedModes (); i++)
    {
      m_minstrelTable[i].numRateAttempt = 0;
      m_minstrelTable[i].numRateSuccess = 0;
      m_minstrelTable[i].prob = 0;
      m_minstrelTable[i].ewmaProb = 0;
      m_minstrelTable[i].prevNumRateAttempt = 0;
      m_minstrelTable[i].prevNumRateSuccess = 0;
      m_minstrelTable[i].successHist = 0;
      m_minstrelTable[i].attemptHist = 0;
      m_minstrelTable[i].throughput = 0;
      m_minstrelTable[i].perfectTxTime = m_stations->GetCalcTxTime (GetSupportedMode (i));
      m_minstrelTable[i].retryCount =1;
      m_minstrelTable[i].adjustedRetryCount =1;
    }
}

void
MinstrelWifiRemoteStation::InitSampleTable ()
{
  NS_LOG_DEBUG ("InitSampleTable="<<this);
	
  m_col = m_index = 0;

  /// for off-seting to make rates fall between 0 and numrates 
  uint32_t numSampleRates= GetNSupportedModes () - 1;

  uint32_t newIndex;
  for (uint32_t col = 0; col < m_stations->m_sampleCol; col++)
    {
      for (uint32_t i = 0; i < numSampleRates; i++ )
        {

          /**
           * The next two lines basically tries to generate a random number
           * between 0 and the number of available rates 
           */
          UniformVariable uv (0, numSampleRates);
          newIndex = (i + (uint32_t)uv.GetValue	()) % numSampleRates;	

          /// this loop is used for filling in other uninitilized places
          while	(m_sampleTable[newIndex][col] != 0)
            {
              newIndex = (newIndex + 1)%GetNSupportedModes ();
            }
          m_sampleTable[newIndex][col] = i+1;

        }
    }
}

void
MinstrelWifiRemoteStation::PrintSampleTable ()
{
  NS_LOG_DEBUG ("PrintSampleTable="<<this );

  uint32_t numSampleRates= GetNSupportedModes ();
  for (uint32_t i=0; i < numSampleRates; i++)
    {
      for (uint32_t j=0; j < m_stations->m_sampleCol; j++)
        {
          std::cout << m_sampleTable[i][j] << "\t";
        }
      std::cout << std::endl;
    }
}
	
void
MinstrelWifiRemoteStation::PrintTable ()
{
  NS_LOG_DEBUG ("PrintTable="<<this);

  for (uint32_t i=0; i < GetNSupportedModes (); i++)
    {
      std::cout << "index(" << i << ") = " << m_minstrelTable[i].perfectTxTime<< "\n";
    }
}

} //namespace ns3





