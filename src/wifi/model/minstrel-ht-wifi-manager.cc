/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Duy Nguyen
 * Copyright (c) 2015 Ghada Badawy
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
 *         Ghada Badawy <gbadawy@gmail.com>
 *         Matias Richart <mrichart@fing.edu.uy>
 *
 * Some Comments:
 *
 * 1) By default, Minstrel applies the multi-rate retry (the core of Minstrel
 *    algorithm). Otherwise, please use ConstantRateWifiManager instead.
 *
 * 2) Sampling is done differently from legacy Minstrel. Minstrel-HT tries 
 * to sample all rates in all groups at least once and to avoid many 
 * consecutive samplings.
 *
 * 3) Sample rate is tried only once, at first place of the MRR chain.
 *
 * reference: http://lwn.net/Articles/376765/
 */

#include "minstrel-ht-wifi-manager.h"
#include "wifi-phy.h"
#include "ns3/random-variable-stream.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/wifi-mac.h"
#include "ns3/assert.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include <vector>
#include <iomanip>
#include <fstream>

#define Min(a,b) ((a < b) ? a : b)
#define Max(a,b) ((a > b) ? a : b)

NS_LOG_COMPONENT_DEFINE ("MinstrelHtWifiManager");

namespace ns3 {

struct MinstrelHtWifiRemoteStation : MinstrelWifiRemoteStation
{
  void DisposeStation ();

  uint32_t m_sampleGroup;     //!< The group that the sample rate belongs to.

  uint32_t m_sampleWait;      //!< How many transmission attempts to wait until a new sample.
  uint32_t m_sampleTries;     //!< Number of sample tries after waiting sampleWait.
  uint32_t m_sampleCount;     //!< Max number of samples per update interval.
  uint32_t m_numSamplesSlow;  //!< Number of times a slow rate was sampled.

  double m_avgAmpduLen;       //!< Average number of MPDUs in an A-MPDU.
  double m_ampduLen;          //!< Number of MPDUs in an A-MPDU.
  uint32_t m_ampduPacketCount; //!< Number of A-MPDUs transmitted.

  McsGroupData m_groupsTable;  //!< Table of groups with stats.
  bool m_isHt;                 //!< If the station is HT capable.

  std::ofstream m_statsFile;   //!< File where statistics table is written.
};

void
MinstrelHtWifiRemoteStation::DisposeStation ()
{
  if (m_isHt)
    {
      std::vector<std::vector<uint32_t> > ().swap (m_sampleTable);
      for (uint8_t j = 0; j < m_groupsTable.size (); j++)
        {
          std::vector<struct HtRateInfo> ().swap (m_groupsTable[j].m_ratesTable);
        }
      std::vector<struct GroupInfo> ().swap (m_groupsTable);
      m_statsFile.close ();
    }
}

NS_OBJECT_ENSURE_REGISTERED (MinstrelHtWifiManager);

TypeId
MinstrelHtWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MinstrelHtWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<MinstrelHtWifiManager> ()
    .SetGroupName ("Wifi")
    .AddAttribute ("UpdateStatistics",
                   "The interval between updating statistics table ",
                   TimeValue (MilliSeconds (100)),
                   MakeTimeAccessor (&MinstrelHtWifiManager::m_updateStats),
                   MakeTimeChecker ())
    .AddAttribute ("LookAroundRate",
                   "The percentage to try other rates (for legacy Minstrel)",
                   DoubleValue (10),
                   MakeDoubleAccessor (&MinstrelHtWifiManager::m_lookAroundRate),
                   MakeDoubleChecker<double> (0, 100))
    .AddAttribute ("EWMA",
                   "EWMA level",
                   DoubleValue (75),
                   MakeDoubleAccessor (&MinstrelHtWifiManager::m_ewmaLevel),
                   MakeDoubleChecker<double> (0, 100))
    .AddAttribute ("SampleColumn",
                   "The number of columns used for sampling",
                   UintegerValue (10),
                   MakeUintegerAccessor (&MinstrelHtWifiManager::m_nSampleCol),
                   MakeUintegerChecker <uint32_t> ())
    .AddAttribute ("PacketLength",
                   "The packet length used for calculating mode TxTime",
                   UintegerValue (1200),
                   MakeUintegerAccessor (&MinstrelHtWifiManager::m_frameLength),
                   MakeUintegerChecker <uint32_t> ())
    .AddAttribute ("UseVhtOnly",
                   "Use only VHT MCSs (and not HT) when VHT is available",
                   BooleanValue (false),
                   MakeBooleanAccessor (&MinstrelHtWifiManager::m_useVhtOnly),
                   MakeBooleanChecker ())
    .AddAttribute ("PrintStats",
                   "Control the printing of the statistics table",
                   BooleanValue (false),
                   MakeBooleanAccessor (&MinstrelHtWifiManager::m_printStats),
                   MakeBooleanChecker ())
    .AddTraceSource ("RateChange",
                     "The transmission rate has changed",
                     MakeTraceSourceAccessor (&MinstrelHtWifiManager::m_rateChange),
                     "ns3::MinstrelHtWifiManager::RateChangeTracedCallback")
  ;
  return tid;
}

MinstrelHtWifiManager::MinstrelHtWifiManager ()
  : m_numGroups (0),
    m_numRates (0)
{
  NS_LOG_FUNCTION (this);
  m_uniformRandomVariable = CreateObject<UniformRandomVariable> ();
  /**
   *  Create the legacy Minstrel manager in case HT is not supported by the device
   *  or non-HT stations want to associate.
   */
  m_legacyManager = CreateObject<MinstrelWifiManager> ();
}

MinstrelHtWifiManager::~MinstrelHtWifiManager ()
{
  NS_LOG_FUNCTION (this);
  if (HasHtSupported ())
    {
      for (uint32_t i = 0; i < m_numGroups; i++)
        {
          m_minstrelGroups[i].ratesFirstMpduTxTimeTable.clear ();
          m_minstrelGroups[i].ratesTxTimeTable.clear ();
        }
    }
}

int64_t
MinstrelHtWifiManager::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  int64_t numStreamsAssigned = 0;
  m_uniformRandomVariable->SetStream (stream);
  numStreamsAssigned++;
  numStreamsAssigned += m_legacyManager->AssignStreams (stream);
  return numStreamsAssigned;
}

void
MinstrelHtWifiManager::SetupPhy (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);

  /**
   * Here we initialize m_minstrelGroups with all the groups the device
   * would support. Then, after all initializations are finished, we
   * check actual support for each receiving station.
   */

  // Check if the device supports HT or VHT
  if (HasHtSupported () || HasVhtSupported ())
    {
      if (HasVhtSupported ())
        {
          m_numGroups = MAX_SUPPORTED_STREAMS * MAX_VHT_STREAM_GROUPS;
          m_numRates = MAX_HT_GROUP_RATES + MAX_VHT_GROUP_RATES;
          m_maxChWidth = MAX_VHT_WIDTH;
        }
      else
        {
          m_numGroups = MAX_SUPPORTED_STREAMS * MAX_HT_STREAM_GROUPS;
          m_numRates = MAX_HT_GROUP_RATES;
          m_maxChWidth = MAX_HT_WIDTH;
        }

      // Initialize the groups array.
      NS_LOG_DEBUG ("Initialize MCS Groups:");
      m_minstrelGroups = MinstrelMcsGroups (m_numGroups);
      for (uint8_t streams = 1; streams <= MAX_SUPPORTED_STREAMS; streams++)
        {
          for (uint8_t sgi = 0; sgi <= 1; sgi++)
            {
              for (uint32_t chWidth = 20; chWidth <= m_maxChWidth; chWidth *= 2)
                {
                  uint32_t groupId = GetGroupId (streams, sgi, chWidth);

                  m_minstrelGroups[groupId].streams = streams;
                  m_minstrelGroups[groupId].sgi = sgi;
                  m_minstrelGroups[groupId].chWidth = chWidth;

                  for (uint8_t i = 0; i < m_numRates; i++)
                    {
                      // Check for invalid VHT MCSs and do not add time to array.
                      WifiMode mode = phy->GetMcs (i);
                      if (IsValidMcs (phy, streams, chWidth, mode))
                        {
                          // Check if the group corresponds to a VHT MCS.
                          if (streams > 4 || chWidth > MAX_HT_WIDTH || i > (MAX_HT_GROUP_RATES - 1)) //TODO change 4 for a constant
                            {
                              m_minstrelGroups[groupId].isVht = true;
                            }
                          else
                            {
                              m_minstrelGroups[groupId].isVht = false;
                            }
                          AddFirstMpduTxTime (groupId, mode, CalculateFirstMpduTxDuration (phy, streams, sgi, chWidth, mode));
                          AddMpduTxTime (groupId, mode, CalculateMpduTxDuration (phy, streams, sgi, chWidth, mode));
                        }
                    }
                  NS_LOG_DEBUG ("Initialized group " << groupId << ": (" << (uint32_t)streams << "," << (uint32_t)sgi << "," << chWidth << ")");
                }
            }
        }
    }

  // Setup phy for legacy manager.
  m_legacyManager->SetupPhy (phy);
  WifiRemoteStationManager::SetupPhy (phy);
}

void
MinstrelHtWifiManager::SetupMac (Ptr<WifiMac> mac)
{
  NS_LOG_FUNCTION (this << mac);
  m_legacyManager->SetupMac (mac);
  WifiRemoteStationManager::SetupMac (mac);
}

bool
MinstrelHtWifiManager::IsValidMcs (Ptr<WifiPhy> phy, uint8_t streams, uint32_t chWidth, WifiMode mode)
{
  NS_LOG_FUNCTION (this << phy << (int)streams << chWidth << mode);

  WifiTxVector txvector;
  txvector.SetNss (streams);
  txvector.SetChannelWidth (chWidth);
  txvector.SetMode (mode);
  return phy->IsValidTxVector (txvector);
}

Time
MinstrelHtWifiManager::CalculateFirstMpduTxDuration (Ptr<WifiPhy> phy, uint8_t streams, uint8_t sgi, uint32_t chWidth, WifiMode mode)
{
  NS_LOG_FUNCTION (this << phy << (int)streams << (int)sgi << chWidth << mode);

  WifiTxVector txvector;
  txvector.SetNss (streams);
  txvector.SetShortGuardInterval (sgi);
  txvector.SetChannelWidth (chWidth);
  txvector.SetNess (0);
  txvector.SetStbc (phy->GetStbc ());
  txvector.SetMode (mode);
  return phy->CalculateTxDuration (m_frameLength, txvector, WIFI_PREAMBLE_HT_MF, phy->GetFrequency (), MPDU_IN_AGGREGATE, 0);
}

Time
MinstrelHtWifiManager::CalculateMpduTxDuration (Ptr<WifiPhy> phy, uint8_t streams, uint8_t sgi, uint32_t chWidth, WifiMode mode)
{
  NS_LOG_FUNCTION (this << phy << (int)streams << (int)sgi << chWidth << mode);

  WifiTxVector txvector;
  txvector.SetNss (streams);
  txvector.SetShortGuardInterval (sgi);
  txvector.SetChannelWidth (chWidth);
  txvector.SetNess (0);
  txvector.SetStbc (phy->GetStbc ());
  txvector.SetMode (mode);
  return phy->CalculateTxDuration (m_frameLength, txvector, WIFI_PREAMBLE_NONE, phy->GetFrequency (), MPDU_IN_AGGREGATE, 0);
}

Time
MinstrelHtWifiManager::GetFirstMpduTxTime (uint32_t groupId, WifiMode mode) const
{
  NS_LOG_FUNCTION (this << groupId << mode);

  for (TxTime::const_iterator i = m_minstrelGroups[groupId].ratesFirstMpduTxTimeTable.begin (); i != m_minstrelGroups[groupId].ratesFirstMpduTxTimeTable.end (); i++)
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
MinstrelHtWifiManager::AddFirstMpduTxTime (uint32_t groupId, WifiMode mode, Time t)
{
  NS_LOG_FUNCTION (this << groupId << mode << t);

  m_minstrelGroups[groupId].ratesFirstMpduTxTimeTable.push_back (std::make_pair (t, mode));
}

Time
MinstrelHtWifiManager::GetMpduTxTime (uint32_t groupId, WifiMode mode) const
{
  NS_LOG_FUNCTION (this << groupId << mode);

  for (TxTime::const_iterator i = m_minstrelGroups[groupId].ratesTxTimeTable.begin (); i != m_minstrelGroups[groupId].ratesTxTimeTable.end (); i++)
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
MinstrelHtWifiManager::AddMpduTxTime (uint32_t groupId, WifiMode mode, Time t)
{
  NS_LOG_FUNCTION (this << groupId << mode << t);

  m_minstrelGroups[groupId].ratesTxTimeTable.push_back (std::make_pair (t, mode));
}

WifiRemoteStation *
MinstrelHtWifiManager::DoCreateStation (void) const
{
  NS_LOG_FUNCTION (this);

  MinstrelHtWifiRemoteStation *station = new MinstrelHtWifiRemoteStation ();

  // Initialize variables common to both stations.
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
  station->m_txrate = 0;
  station->m_initialized = false;

  // Variables specific to HT station
  station->m_sampleGroup = 0;
  station->m_numSamplesSlow = 0;
  station->m_sampleCount = 16;
  station->m_sampleWait = 0;
  station->m_sampleTries = 4;

  station->m_avgAmpduLen = 1;
  station->m_ampduLen = 0;
  station->m_ampduPacketCount = 0;

  // If the device supports HT
  if (HasHtSupported () || HasVhtSupported ())
    {
      /**
       * Assume the station is HT.
       * When correct information available it will be checked.
       */
      station->m_isHt = true;
    }
  // Use the variable in the station to indicate that the device do not support HT
  else
    {
      station->m_isHt = false;
    }

  return station;
}

void
MinstrelHtWifiManager::CheckInit (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);

  // Note: we appear to be doing late initialization of the table
  // to make sure that the set of supported rates has been initialized
  // before we perform our own initialization.
  if (!station->m_initialized)
    {
      /**
       *  Check if the station supports HT.
       *  Assume that if the device do not supports HT then
       *  the station will not support HT either.
       *  We save from using another check and variable.
       */
      if (!GetHtSupported (station) && !GetVhtSupported (station))
        {
          NS_LOG_DEBUG ("Non-HT station " << station);
          station->m_isHt = false;
          // We will use non-HT minstrel for this station. Initialize the manager.
          m_legacyManager->SetAttribute ("UpdateStatistics", TimeValue (m_updateStats));
          m_legacyManager->SetAttribute ("LookAroundRate", DoubleValue (m_lookAroundRate));
          m_legacyManager->SetAttribute ("EWMA", DoubleValue (m_ewmaLevel));
          m_legacyManager->SetAttribute ("SampleColumn", UintegerValue (m_nSampleCol));
          m_legacyManager->SetAttribute ("PacketLength", UintegerValue (m_frameLength));
          m_legacyManager->SetAttribute ("PrintStats", BooleanValue (m_printStats));
          m_legacyManager->CheckInit (station);
        }
      else
        {
          NS_LOG_DEBUG ("HT station " << station);
          station->m_isHt = true;
          station->m_nModes = GetNMcsSupported (station);
          station->m_sampleTable = SampleRate (station->m_nModes, std::vector<uint32_t> (m_nSampleCol));
          m_legacyManager->InitSampleTable (station);
          RateInit (station);
          std::ostringstream tmp;
          tmp << "minstrel-ht-stats-" << station->m_state->m_address << ".txt";
          station->m_statsFile.open (tmp.str ().c_str (), std::ios::out);
          station->m_initialized = true;
        }
    }
}

void
MinstrelHtWifiManager::DoReportRxOk (WifiRemoteStation *st,
                                     double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << st);

  NS_LOG_DEBUG ("DoReportRxOk m_txrate=" << ((MinstrelHtWifiRemoteStation *)st)->m_txrate);
}

void
MinstrelHtWifiManager::DoReportRtsFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);

  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *)st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  NS_LOG_DEBUG ("DoReportRtsFailed m_txrate=" << station->m_txrate);
  station->m_shortRetry++;
}

void
MinstrelHtWifiManager::DoReportRtsOk (WifiRemoteStation *st, double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << st);

  NS_LOG_DEBUG ("self=" << st << " rts ok");
}

void
MinstrelHtWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);

  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *)st;
  NS_LOG_DEBUG ("Final RTS failed");

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  UpdateRetry (station);
}

void
MinstrelHtWifiManager::DoReportDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);

  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *)st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  if (!station->m_isHt)
    {
      m_legacyManager->UpdateRate (station);
    }
  else
    {
      NS_LOG_DEBUG ("DoReportDataFailed " << station << "\t rate " << station->m_txrate << "\tlongRetry \t" << station->m_longRetry);

      uint32_t rateId = GetRateId (station->m_txrate);
      uint32_t groupId = GetGroupId (station->m_txrate);
      station->m_groupsTable[groupId].m_ratesTable[rateId].numRateAttempt++; // Increment the attempts counter for the rate used.

      UpdateRate (station);
    }
}

void
MinstrelHtWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                       double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << st << ackSnr << ackMode << dataSnr);
  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *) st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  NS_LOG_DEBUG ("Data OK - Txrate = " << station->m_txrate  );

  if (!station->m_isHt)
    {
      station->m_minstrelTable[station->m_txrate].numRateSuccess++;
      station->m_minstrelTable[station->m_txrate].numRateAttempt++;

      m_legacyManager->UpdatePacketCounters (station);

      UpdateRetry (station);
      m_legacyManager->UpdateStats (station);

      if (station->m_nModes >= 1)
        {
          station->m_txrate = m_legacyManager->FindRate (station);
        }
    }
  else
    {
      uint32_t rateId = GetRateId (station->m_txrate);
      uint32_t groupId = GetGroupId (station->m_txrate);
      station->m_groupsTable[groupId].m_ratesTable[rateId].numRateSuccess++;
      station->m_groupsTable[groupId].m_ratesTable[rateId].numRateAttempt++;

      UpdatePacketCounters (station, 1, 0);

      station->m_isSampling = false;
      station->m_sampleDeferred = false;

      UpdateRetry (station);
      if (Simulator::Now () >=  station->m_nextStatsUpdate)
        {
          UpdateStats (station);
        }

      if (station->m_nModes >= 1)
        {
          station->m_txrate = FindRate (station);
        }
    }

  NS_LOG_DEBUG ("Next rate to use TxRate = " << station->m_txrate  );
}

void
MinstrelHtWifiManager::DoReportFinalDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *) st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  NS_LOG_DEBUG ("DoReportFinalDataFailed - TxRate=" << station->m_txrate);

  if (!station->m_isHt)
    {
      m_legacyManager->UpdatePacketCounters (station);

      UpdateRetry (station);

      m_legacyManager->UpdateStats (station);
      if (station->m_nModes >= 1)
        {
          station->m_txrate = m_legacyManager->FindRate (station);
        }
    }
  else
    {
      UpdatePacketCounters (station, 0, 1);

      station->m_isSampling = false;
      station->m_sampleDeferred = false;

      UpdateRetry (station);
      if (Simulator::Now () >=  station->m_nextStatsUpdate)
        {
          UpdateStats (station);
        }

      if (station->m_nModes >= 1)
        {
          station->m_txrate = FindRate (station);
        }
    }
  NS_LOG_DEBUG ("Next rate to use TxRate = " << station->m_txrate  );
}

void
MinstrelHtWifiManager::DoReportAmpduTxStatus (WifiRemoteStation *st, uint32_t nSuccessfulMpdus, uint32_t nFailedMpdus)
{
  NS_LOG_FUNCTION (this << st << nSuccessfulMpdus << nFailedMpdus);
  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *) st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }

  if (!station->m_isHt)
    {
      NS_ASSERT_MSG (false,"A-MPDU Tx Status called but no HT or VHT supported.");
    }

  NS_LOG_DEBUG ("DoReportAmpduTxStatus. TxRate=" << station->m_txrate << " SuccMpdus= " <<
                nSuccessfulMpdus << " FailedMpdus= " << nFailedMpdus);

  station->m_ampduPacketCount++;
  station->m_ampduLen += nSuccessfulMpdus + nFailedMpdus;

  UpdatePacketCounters (station, nSuccessfulMpdus, nFailedMpdus);

  uint32_t rateId = GetRateId (station->m_txrate);
  uint32_t groupId = GetGroupId (station->m_txrate);
  station->m_groupsTable[groupId].m_ratesTable[rateId].numRateSuccess += nSuccessfulMpdus;
  station->m_groupsTable[groupId].m_ratesTable[rateId].numRateAttempt += nSuccessfulMpdus + nFailedMpdus;

  if (nSuccessfulMpdus == 0 && station->m_longRetry < CountRetries (station))
    {
      // We do not receive a BlockAck. The entire AMPDU fail.
      UpdateRate (station);
    }
  else
    {
      station->m_isSampling = false;
      station->m_sampleDeferred = false;

      UpdateRetry (station);
      if (Simulator::Now () >=  station->m_nextStatsUpdate)
        {
          UpdateStats (station);
        }

      if (station->m_nModes >= 1)
        {
          station->m_txrate = FindRate (station);
        }
      NS_LOG_DEBUG ("Next rate to use TxRate = " << station->m_txrate  );
    }
}

void
MinstrelHtWifiManager::UpdateRate (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);

  /**
   * Retry Chain table is implemented here.
   *
   * FIXME
   * Currently, NS3 does not retransmit an entire A-MPDU when BACK is missing
   * but retransmits each MPDU until MPDUs lifetime expires (or a BACK is received).
   * Then, there is no way to control A-MPDU retries (no call to NeedDataRetransmission).
   * So, it is possible that the A-MPDU keeps retrying after longRetry reaches its limit.
   *
   *
   * Try |     LOOKAROUND RATE     | NORMAL RATE
   * -------------------------------------------------------
   *  1  |  Random rate            | Best throughput
   *  2  |  Next best throughput   | Next best throughput
   *  3  |  Best probability       | Best probability
   *
   * Note: For clarity, multiple blocks of if's and else's are used
   * Following implementation in Linux, in MinstrelHT Lowest baserate is not used.
   * Explanation can be found here: http://marc.info/?l=linux-wireless&m=144602778611966&w=2
   */

  CheckInit (station);
  if (!station->m_initialized)
    {
      return;
    }
  station->m_longRetry++;

  /**
   * Get the ids for all rates.
   */
  uint32_t maxTpRateId = GetRateId (station->m_maxTpRate);
  uint32_t maxTpGroupId = GetGroupId (station->m_maxTpRate);
  uint32_t maxTp2RateId = GetRateId (station->m_maxTpRate2);
  uint32_t maxTp2GroupId = GetGroupId (station->m_maxTpRate2);
  uint32_t maxProbRateId = GetRateId (station->m_maxProbRate);
  uint32_t maxProbGroupId = GetGroupId (station->m_maxProbRate);

  /// For normal rate, we're not currently sampling random rates.
  if (!station->m_isSampling)
    {
      /// Use best throughput rate.
      if (station->m_longRetry <  station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTpRateId].retryCount)
        {
          NS_LOG_DEBUG ("Not Sampling; use the same rate again");
          station->m_txrate = station->m_maxTpRate;  //!<  There are still a few retries.
        }

      /// Use second best throughput rate.
      else if (station->m_longRetry < ( station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTpRateId].retryCount +
                                        station->m_groupsTable[maxTp2GroupId].m_ratesTable[maxTp2RateId].retryCount))
        {
          NS_LOG_DEBUG ("Not Sampling; use the Max TP2");
          station->m_txrate = station->m_maxTpRate2;
        }

      /// Use best probability rate.
      else if (station->m_longRetry <= ( station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTpRateId].retryCount +
                                         station->m_groupsTable[maxTp2GroupId].m_ratesTable[maxTp2RateId].retryCount +
                                         station->m_groupsTable[maxProbGroupId].m_ratesTable[maxProbRateId].retryCount))
        {
          NS_LOG_DEBUG ("Not Sampling; use Max Prob");
          station->m_txrate = station->m_maxProbRate;
        }
      else
        {
          NS_ASSERT_MSG (false,"Max retries reached and m_longRetry not cleared properly. longRetry= " << station->m_longRetry);
        }
    }

  /// We're currently sampling random rates.
  else
    {
      /// Sample rate is used only once
      /// Use the best rate.
      if (station->m_longRetry < 1 + station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTp2RateId].retryCount)
        {
          NS_LOG_DEBUG ("Sampling use the MaxTP rate");
          station->m_txrate = station->m_maxTpRate2;
        }

      /// Use the best probability rate.
      else if (station->m_longRetry <= 1 + station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTp2RateId].retryCount +
               station->m_groupsTable[maxProbGroupId].m_ratesTable[maxProbRateId].retryCount)
        {
          NS_LOG_DEBUG ("Sampling use the MaxProb rate");
          station->m_txrate = station->m_maxProbRate;
        }
      else
        {
          NS_ASSERT_MSG (false,"Max retries reached and m_longRetry not cleared properly. longRetry= " << station->m_longRetry);
        }
    }
  NS_LOG_DEBUG ("Next rate to use TxRate = " << station->m_txrate);
}

void
MinstrelHtWifiManager::UpdateRetry (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  station->m_shortRetry = 0;
  station->m_longRetry = 0;

}

void
MinstrelHtWifiManager::UpdatePacketCounters (MinstrelHtWifiRemoteStation *station, uint32_t nSuccessfulMpdus, uint32_t nFailedMpdus)
{
  NS_LOG_FUNCTION (this << station << nSuccessfulMpdus << nFailedMpdus);

  station->m_totalPacketsCount += nSuccessfulMpdus + nFailedMpdus;
  if (station->m_isSampling)
    {
      station->m_samplePacketsCount += nSuccessfulMpdus + nFailedMpdus;
    }
  if (station->m_totalPacketsCount == ~0)
    {
      station->m_samplePacketsCount = 0;
      station->m_totalPacketsCount = 0;
    }

  if (!station->m_sampleWait && !station->m_sampleTries && station->m_sampleCount > 0)
    {
      station->m_sampleWait = 16 + 2 * station->m_avgAmpduLen;
      station->m_sampleTries = 1;
      station->m_sampleCount--;
    }

}
void
MinstrelHtWifiManager::DoDisposeStation (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *) st;
  station->DisposeStation ();
}

WifiTxVector
MinstrelHtWifiManager::DoGetDataTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *) st;

  if (!station->m_initialized)
    {
      CheckInit (station);
    }

  if (!station->m_isHt)
    {
      WifiTxVector vector = m_legacyManager->GetDataTxVector (station);

      uint64_t dataRate = vector.GetMode ().GetDataRate (vector.GetChannelWidth (), vector.IsShortGuardInterval (), vector.GetNss ());
      if (!station->m_isSampling)
        {
          m_rateChange (dataRate, station->m_state->m_address);
        }

      return vector;
    }
  else
    {
      NS_LOG_DEBUG ("DoGetDataMode m_txrate= " << station->m_txrate);

      uint32_t rateId = GetRateId (station->m_txrate);
      uint32_t groupId = GetGroupId (station->m_txrate);

      NS_LOG_DEBUG ("DoGetDataMode rateId= " << rateId << " groupId= " << groupId << " mode= " << GetMcsSupported (station, rateId));

      McsGroup group = m_minstrelGroups[groupId];

      // Check consistency of rate selected.
      if ((group.sgi && !GetShortGuardInterval (station)) || group.chWidth > GetChannelWidth (station)  ||  (uint32_t) group.streams > GetNumberOfSupportedRxAntennas (station))
        {
          NS_ASSERT_MSG (false,"Inconsistent group selected. Group: (" << (uint32_t)group.streams << "," << (uint32_t)group.sgi << "," << group.chWidth << ")" <<
                         " Station capabilities: (" << GetNumberOfSupportedRxAntennas (station) << "," << GetShortGuardInterval (station) << "," << GetChannelWidth (station) << ")");
        }


      uint64_t dataRate = GetMcsSupported (station, rateId).GetDataRate (group.chWidth, group.sgi, group.streams);
      if (!station->m_isSampling)
        {
          m_rateChange (dataRate, station->m_state->m_address);
        }

      return WifiTxVector (GetMcsSupported (station, rateId), GetDefaultTxPowerLevel (), GetLongRetryCount (station),
                           group.sgi, group.streams, GetNess (station), group.chWidth, !station->m_isSampling, GetStbc (station));
    }
}

WifiTxVector
MinstrelHtWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *) st;

  if (!station->m_initialized)
    {
      CheckInit (station);
    }

  if (!station->m_isHt)
    {
      return m_legacyManager->GetRtsTxVector (station);
    }
  else
    {
      NS_LOG_DEBUG ("DoGetRtsMode m_txrate=" << station->m_txrate);

      /* RTS is sent in a non-HT frame. RTS with HT is not supported yet in NS3.
       * When supported, decision of using HT has to follow rules in Section 9.7.6 from 802.11-2012.
       * From Sec. 9.7.6.5: "A frame other than a BlockAckReq or BlockAck that is carried in a
       * non-HT PPDU shall be transmitted by the STA using a rate no higher than the highest
       * rate in  the BSSBasicRateSet parameter that is less than or equal to the rate or
       * non-HT reference rate (see 9.7.9) of the previously transmitted frame that was
       * directed to the same receiving STA. If no rate in the BSSBasicRateSet parameter meets
       * these conditions, the control frame shall be transmitted at a rate no higher than the
       * highest mandatory rate of the attached PHY that is less than or equal to the rate
       * or non-HT reference rate (see 9.7.9) of the previously transmitted frame that was
       * directed to the same receiving STA."
       */

      // As we are in Minstrel HT, assume the last rate was an HT rate.
      WifiMode lastRate = GetMcsSupported (station, GetRateId (station->m_txrate));
      uint8_t streams = m_minstrelGroups[GetGroupId (station->m_txrate)].streams;
      uint64_t lastDataRate = lastRate.GetNonHtReferenceRate (streams);
      uint32_t nBasicRates = GetNBasicModes ();

      WifiMode rtsRate;
      bool rateFound = false;

      for (uint32_t i = 0; i < nBasicRates; i++)
        {
          uint64_t rate = GetBasicMode (i).GetDataRate (20,false,1);
          if (rate <= lastDataRate)
            {
              rtsRate = GetBasicMode (i);
              rateFound = true;
            }
        }

      if (!rateFound)
        {
          Ptr<WifiPhy> phy = GetPhy ();
          uint32_t nSupportRates = phy->GetNModes ();
          for (uint32_t i = 0; i < nSupportRates; i++)
            {
              uint64_t rate = phy->GetMode (i).GetDataRate (20,false,1);
              if (rate <= lastDataRate)
                {
                  rtsRate = phy->GetMode (i);
                  rateFound = true;
                }
            }
        }

      NS_ASSERT (rateFound);

      return WifiTxVector (GetSupported (station, 0), GetDefaultTxPowerLevel (), GetShortRetryCount (station),
                           false, 1, 0, GetChannelWidth (station), GetAggregation (station), false);
    }
}

bool
MinstrelHtWifiManager::DoNeedDataRetransmission (WifiRemoteStation *st, Ptr<const Packet> packet, bool normally)
{
  NS_LOG_FUNCTION (this << st << packet << normally);

  MinstrelHtWifiRemoteStation *station = (MinstrelHtWifiRemoteStation *)st;

  CheckInit (station);
  if (!station->m_initialized)
    {
      return normally;
    }

  uint32_t maxRetries;

  if (!station->m_isHt)
    {
      maxRetries = m_legacyManager->CountRetries (station);
    }
  else
    {
      maxRetries = CountRetries (station);
    }

  if (station->m_longRetry >= maxRetries)
    {
      NS_LOG_DEBUG ("No re-transmission allowed. Retries: " <<  station->m_longRetry << " Max retries: " << maxRetries);
      return false;
    }
  else
    {
      NS_LOG_DEBUG ("Re-transmit. Retries: " <<  station->m_longRetry << " Max retries: " << maxRetries);
      return true;
    }
}

uint32_t
MinstrelHtWifiManager::CountRetries (MinstrelHtWifiRemoteStation *station)
{
  uint32_t maxProbRateId = GetRateId (station->m_maxProbRate);
  uint32_t maxProbGroupId = GetGroupId (station->m_maxProbRate);
  uint32_t maxTpRateId = GetRateId (station->m_maxTpRate);
  uint32_t maxTpGroupId = GetGroupId (station->m_maxTpRate);
  uint32_t maxTp2RateId = GetRateId (station->m_maxTpRate2);
  uint32_t maxTp2GroupId = GetGroupId (station->m_maxTpRate2);

  if (!station->m_isSampling)
    {
      return station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTpRateId].retryCount +
             station->m_groupsTable[maxTp2GroupId].m_ratesTable[maxTp2RateId].retryCount +
             station->m_groupsTable[maxProbGroupId].m_ratesTable[maxProbRateId].retryCount;
    }
  else
    {
      return 1 + station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTp2RateId].retryCount +
             station->m_groupsTable[maxProbGroupId].m_ratesTable[maxProbRateId].retryCount;
    }
}

bool
MinstrelHtWifiManager::IsLowLatency (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

uint32_t
MinstrelHtWifiManager::GetNextSample (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);

  uint32_t sampleGroup = station->m_sampleGroup;

  uint32_t index = station->m_groupsTable[sampleGroup].m_index;
  uint32_t col = station->m_groupsTable[sampleGroup].m_col;

  uint32_t sampleIndex = station->m_sampleTable[index][col];

  uint32_t rateIndex = GetIndex (sampleGroup, sampleIndex);
  NS_LOG_DEBUG ("Next Sample is " << rateIndex );

  SetNextSample (station); //Calculate the next sample rate.

  return rateIndex;
}

void
MinstrelHtWifiManager::SetNextSample (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  do
    {
      station->m_sampleGroup++;
      station->m_sampleGroup %= m_numGroups;
    }
  while (!station->m_groupsTable[station->m_sampleGroup].m_supported);

  station->m_groupsTable[station->m_sampleGroup].m_index++;

  uint32_t sampleGroup = station->m_sampleGroup;
  uint8_t index = station->m_groupsTable[station->m_sampleGroup].m_index;
  uint8_t col = station->m_groupsTable[sampleGroup].m_col;

  if (index >= station->m_nModes
      || !station->m_groupsTable[sampleGroup].m_ratesTable[station->m_sampleTable[index][col]].supported)
    {
      station->m_groupsTable[station->m_sampleGroup].m_index = 0;
      station->m_groupsTable[station->m_sampleGroup].m_col++;
      if (station->m_groupsTable[station->m_sampleGroup].m_col >= m_nSampleCol)
        {
          station->m_groupsTable[station->m_sampleGroup].m_col = 0;
        }
      index = station->m_groupsTable[station->m_sampleGroup].m_index;
      col = station->m_groupsTable[sampleGroup].m_col;
    }
  NS_LOG_DEBUG ("New sample set: group= " << sampleGroup << " index= " << station->m_sampleTable[index][col]);
}

uint32_t
MinstrelHtWifiManager::FindRate (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  NS_LOG_DEBUG ("FindRate " << "packet=" << station->m_totalPacketsCount );

  if ((station->m_samplePacketsCount + station->m_totalPacketsCount) == 0)
    {
      return station->m_maxTpRate;
    }

  if (station->m_sampleWait == 0 && station->m_sampleTries != 0)
    {
      //SAMPLING
      NS_LOG_DEBUG ("Obtaining a sampling rate");
      /// Now go through the table and find an index rate.
      uint32_t sampleIdx = GetNextSample (station);
      NS_LOG_DEBUG ("Sampling rate = " << sampleIdx);


      //Evaluate if the sampling rate selected should be used.
      uint32_t sampleGroupId = GetGroupId (sampleIdx);
      uint32_t sampleRateId = GetRateId (sampleIdx);
      McsGroup sampleGroup = m_minstrelGroups[sampleGroupId];
      HtRateInfo sampleRateInfo = station->m_groupsTable[sampleGroupId].m_ratesTable[sampleRateId];

      /**
       * Sampling might add some overhead to the frame.
       * Hence, don't use sampling for the currently used rates.
       *
       * Also do not sample if the probability is already higher than 95%
       * to avoid wasting airtime.
       */
      NS_LOG_DEBUG ("Use sample rate? MaxTpRate= " << station->m_maxTpRate << " CurrentRate= " << station->m_txrate <<
                    " SampleRate= " << sampleIdx << " SampleProb= " << sampleRateInfo.ewmaProb);
      if (sampleIdx != station->m_maxTpRate && sampleIdx != station->m_maxTpRate2
          && sampleIdx != station->m_maxProbRate && sampleRateInfo.ewmaProb <= 95)
        {

          /**
           * Make sure that lower rates get sampled only occasionally,
           * if the link is working perfectly.
           */

          uint32_t maxTpGroupId = GetGroupId (station->m_maxTpRate);
          uint32_t maxTp2GroupId = GetGroupId (station->m_maxTpRate2);
          uint32_t maxTp2RateId = GetRateId (station->m_maxTpRate2);
          uint32_t maxProbGroupId = GetGroupId (station->m_maxProbRate);
          uint32_t maxProbRateId = GetRateId (station->m_maxProbRate);

          uint8_t maxTpStreams = m_minstrelGroups[maxTpGroupId].streams;
          uint8_t sampleStreams = m_minstrelGroups[sampleGroupId].streams;

          Time sampleDuration = sampleRateInfo.perfectTxTime;
          Time maxTp2Duration = station->m_groupsTable[maxTp2GroupId].m_ratesTable[maxTp2RateId].perfectTxTime;
          Time maxProbDuration = station->m_groupsTable[maxProbGroupId].m_ratesTable[maxProbRateId].perfectTxTime;

          NS_LOG_DEBUG ("Use sample rate? SampleDuration= " << sampleDuration << " maxTp2Duration= " << maxTp2Duration <<
                        " maxProbDuration= " << maxProbDuration << " sampleStreams= " << (uint32_t)sampleStreams <<
                        " maxTpStreams= " << (uint32_t)maxTpStreams);
          if (sampleDuration < maxTp2Duration || (sampleStreams <= maxTpStreams - 1 && sampleDuration < maxProbDuration))
            {
              /// Set flag that we are currently sampling.
              station->m_isSampling = true;

              /// set the rate that we're currently sampling
              station->m_sampleRate = sampleIdx;

              NS_LOG_DEBUG ("FindRate " << "sampleRate=" << sampleIdx);
              station->m_sampleTries--;
              return sampleIdx;
            }
          else
            {
              station->m_numSamplesSlow++;
              if (sampleRateInfo.numSamplesSkipped >= 20 && station->m_numSamplesSlow <= 2)
                {
                  /// Set flag that we are currently sampling.
                  station->m_isSampling = true;

                  /// set the rate that we're currently sampling
                  station->m_sampleRate = sampleIdx;

                  NS_LOG_DEBUG ("FindRate " << "sampleRate=" << sampleIdx);
                  station->m_sampleTries--;
                  return sampleIdx;
                }
            }
        }
    }
  if (station->m_sampleWait > 0)
    {
      station->m_sampleWait--;
    }

  ///	Continue using the best rate.

  NS_LOG_DEBUG ("FindRate " << "maxTpRrate=" << station->m_maxTpRate);
  return station->m_maxTpRate;
}
uint32_t
MinstrelHtWifiManager::GetIndex (uint32_t groupid, uint32_t mcsIndex)
{
  NS_LOG_FUNCTION (this << groupid << mcsIndex);
  uint32_t index;
  index = groupid * m_numRates + mcsIndex;
  return index;
}
void
MinstrelHtWifiManager::UpdateStats (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);

  NS_LOG_DEBUG ("Updating stats=" << this);

  station->m_nextStatsUpdate = Simulator::Now () + m_updateStats;

  station->m_numSamplesSlow = 0;
  station->m_sampleCount = 0;

  double tempProb;

  if (station->m_ampduPacketCount > 0)
    {
      double newLen = station->m_ampduLen / station->m_ampduPacketCount;
      station->m_avgAmpduLen = ( newLen * (100 - m_ewmaLevel) + (station->m_avgAmpduLen * m_ewmaLevel) ) / 100;
      station->m_ampduLen = 0;
      station->m_ampduPacketCount = 0;
    }

  uint32_t index;

  /* Initialize global rate indexes */
  station->m_maxTpRate = 0;
  station->m_maxTpRate2 = 0;
  station->m_maxProbRate = 0;

  /// Update throughput and EWMA for each rate inside each group.
  for (uint32_t j = 0; j < m_numGroups; j++)
    {
      if (station->m_groupsTable[j].m_supported)
        {
          station->m_sampleCount++;

          /* (re)Initialize group rate indexes */
          station->m_groupsTable[j].m_maxTpRate = 0;
          station->m_groupsTable[j].m_maxTpRate2 = 0;

          for (uint32_t i = 0; i < station->m_nModes; i++)
            {
              if (station->m_groupsTable[j].m_ratesTable[i].supported)
                {
                  station->m_groupsTable[j].m_ratesTable[i].retryUpdated = false;

                  NS_LOG_DEBUG (i << " " << GetMcsSupported (station, i) <<
                                "\t attempt=" << station->m_groupsTable[j].m_ratesTable[i].numRateAttempt <<
                                "\t success=" << station->m_groupsTable[j].m_ratesTable[i].numRateSuccess);

                  /// If we've attempted something.
                  if (station->m_groupsTable[j].m_ratesTable[i].numRateAttempt > 0)
                    {
                      station->m_groupsTable[j].m_ratesTable[i].numSamplesSkipped = 0;
                      /**
                       * Calculate the probability of success.
                       * Assume probability scales from 0 to 100.
                       */
                      tempProb = (100 * station->m_groupsTable[j].m_ratesTable[i].numRateSuccess) / station->m_groupsTable[j].m_ratesTable[i].numRateAttempt;

                      /// Bookeeping.
                      station->m_groupsTable[j].m_ratesTable[i].prob = tempProb;

                      if (station->m_groupsTable[j].m_ratesTable[i].successHist == 0)
                        {
                          station->m_groupsTable[j].m_ratesTable[i].ewmaProb = tempProb;
                        }
                      else
                        {
                          station->m_groupsTable[j].m_ratesTable[i].ewmsdProb = CalculateEwmsd (station->m_groupsTable[j].m_ratesTable[i].ewmsdProb,
                                                                                                tempProb, station->m_groupsTable[j].m_ratesTable[i].ewmaProb,
                                                                                                m_ewmaLevel);
                          /// EWMA probability
                          tempProb = (tempProb * (100 - m_ewmaLevel) + station->m_groupsTable[j].m_ratesTable[i].ewmaProb * m_ewmaLevel)  / 100;
                          station->m_groupsTable[j].m_ratesTable[i].ewmaProb = tempProb;
                        }

                      station->m_groupsTable[j].m_ratesTable[i].throughput = CalculateThroughput (station, j, i, tempProb);

                      station->m_groupsTable[j].m_ratesTable[i].successHist += station->m_groupsTable[j].m_ratesTable[i].numRateSuccess;
                      station->m_groupsTable[j].m_ratesTable[i].attemptHist += station->m_groupsTable[j].m_ratesTable[i].numRateAttempt;
                    }
                  else
                    {
                      station->m_groupsTable[j].m_ratesTable[i].numSamplesSkipped++;
                    }

                  /// Bookeeping.
                  station->m_groupsTable[j].m_ratesTable[i].prevNumRateSuccess = station->m_groupsTable[j].m_ratesTable[i].numRateSuccess;
                  station->m_groupsTable[j].m_ratesTable[i].prevNumRateAttempt = station->m_groupsTable[j].m_ratesTable[i].numRateAttempt;
                  station->m_groupsTable[j].m_ratesTable[i].numRateSuccess = 0;
                  station->m_groupsTable[j].m_ratesTable[i].numRateAttempt = 0;

                  if (station->m_groupsTable[j].m_ratesTable[i].throughput != 0)
                    {
                      index = GetIndex (j, i);
                      SetBestStationThRates (station, index);
                      SetBestProbabilityRate (station, index);
                    }

                }
            }
        }
    }

  //Try to sample all available rates during each interval.
  station->m_sampleCount *= 8;

  //Recalculate retries for the rates selected.
  CalculateRetransmits (station, station->m_maxTpRate);
  CalculateRetransmits (station, station->m_maxTpRate2);
  CalculateRetransmits (station, station->m_maxProbRate);

  NS_LOG_DEBUG ("max tp=" << station->m_maxTpRate << "\nmax tp2=" <<  station->m_maxTpRate2 << "\nmax prob=" << station->m_maxProbRate);
  if (m_printStats)
    {
      PrintTable (station);
    }
}

double
MinstrelHtWifiManager::CalculateThroughput (MinstrelHtWifiRemoteStation *station, uint32_t groupId, uint32_t rateId, double ewmaProb)
{
  /**
  * Calculating throughput.
  * Do not account throughput if sucess prob is below 10%
  * (as done in minstrel_ht linux implementation).
  */
  if (ewmaProb < 10)
    {
      return 0;
    }
  else
    {
      /**
       * For the throughput calculation, limit the probability value to 90% to
       * account for collision related packet error rate fluctuation.
       */
      Time txTime =  station->m_groupsTable[groupId].m_ratesTable[rateId].perfectTxTime;
      if (ewmaProb > 90)
        {
          return 90 / txTime.GetSeconds ();
        }
      else
        {
          return ewmaProb / txTime.GetSeconds ();
        }
    }
}

void
MinstrelHtWifiManager::SetBestProbabilityRate (MinstrelHtWifiRemoteStation *station, uint32_t index)
{
  GroupInfo *group;
  HtRateInfo rate;
  uint32_t tmpGroupId, tmpRateId;
  double tmpTh, tmpProb;
  uint32_t groupId, rateId;
  double currentTh;
  // maximum group probability (GP) variables
  uint32_t maxGPGroupId, maxGPRateId;
  double maxGPTh;

  groupId = GetGroupId (index);
  rateId = GetRateId (index);
  group = &station->m_groupsTable[groupId];
  rate = group->m_ratesTable[rateId];

  tmpGroupId = GetGroupId (station->m_maxProbRate);
  tmpRateId = GetRateId (station->m_maxProbRate);
  tmpProb = station->m_groupsTable[tmpGroupId].m_ratesTable[tmpRateId].ewmaProb;
  tmpTh =  station->m_groupsTable[tmpGroupId].m_ratesTable[tmpRateId].throughput;

  if (rate.ewmaProb > 75)
    {
      currentTh = station->m_groupsTable[groupId].m_ratesTable[rateId].throughput;
      if (currentTh > tmpTh)
        {
          station->m_maxProbRate = index;
        }

      maxGPGroupId = GetGroupId (group->m_maxProbRate);
      maxGPRateId = GetRateId (group->m_maxProbRate);
      maxGPTh = station->m_groupsTable[maxGPGroupId].m_ratesTable[maxGPRateId].throughput;

      if (currentTh > maxGPTh)
        {
          group->m_maxProbRate = index;
        }
    }
  else
    {
      if (rate.ewmaProb > tmpProb)
        {
          station->m_maxProbRate = index;
        }
      if (rate.ewmaProb > group->m_ratesTable[group->m_maxProbRate].ewmaProb)
        {
          group->m_maxProbRate = index;
        }
    }
}

/*
 * Find & sort topmost throughput rates
 *
 * If multiple rates provide equal throughput the sorting is based on their
 * current success probability. Higher success probability is preferred among
 * MCS groups.
 */
void
MinstrelHtWifiManager::SetBestStationThRates (MinstrelHtWifiRemoteStation *station, uint32_t index)
{
  uint32_t groupId, rateId;
  double th, prob;
  uint32_t maxTpGroupId, maxTpRateId;
  uint32_t maxTp2GroupId, maxTp2RateId;
  double maxTpTh, maxTpProb;
  double maxTp2Th, maxTp2Prob;

  groupId = GetGroupId (index);
  rateId = GetRateId (index);
  prob = station->m_groupsTable[groupId].m_ratesTable[rateId].ewmaProb;
  th = station->m_groupsTable[groupId].m_ratesTable[rateId].throughput;

  maxTpGroupId = GetGroupId (station->m_maxTpRate);
  maxTpRateId = GetRateId (station->m_maxTpRate);
  maxTpProb = station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTpRateId].ewmaProb;
  maxTpTh = station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTpRateId].throughput;

  maxTp2GroupId = GetGroupId (station->m_maxTpRate2);
  maxTp2RateId = GetRateId (station->m_maxTpRate2);
  maxTp2Prob = station->m_groupsTable[maxTp2GroupId].m_ratesTable[maxTp2RateId].ewmaProb;
  maxTp2Th = station->m_groupsTable[maxTp2GroupId].m_ratesTable[maxTp2RateId].throughput;

  if (th > maxTpTh || (th == maxTpTh && prob > maxTpProb))
    {
      station->m_maxTpRate2 = station->m_maxTpRate;
      station->m_maxTpRate = index;
    }
  else if (th > maxTp2Th || (th == maxTp2Th && prob > maxTp2Prob))
    {
      station->m_maxTpRate2 = index;
    }

  //Find best rates per group

  GroupInfo *group = &station->m_groupsTable[groupId];
  maxTpGroupId = GetGroupId (group->m_maxTpRate);
  maxTpRateId = GetRateId (group->m_maxTpRate);
  maxTpProb = group->m_ratesTable[maxTpRateId].ewmaProb;
  maxTpTh = station->m_groupsTable[maxTpGroupId].m_ratesTable[maxTpRateId].throughput;

  maxTp2GroupId = GetGroupId (group->m_maxTpRate2);
  maxTp2RateId = GetRateId (group->m_maxTpRate2);
  maxTp2Prob = group->m_ratesTable[maxTp2RateId].ewmaProb;
  maxTp2Th = station->m_groupsTable[maxTp2GroupId].m_ratesTable[maxTp2RateId].throughput;

  if (th > maxTpTh || (th == maxTpTh && prob > maxTpProb))
    {
      group->m_maxTpRate2 = group->m_maxTpRate;
      group->m_maxTpRate = index;
    }
  else if (th > maxTp2Th || (th == maxTp2Th && prob > maxTp2Prob))
    {
      group->m_maxTpRate2 = index;
    }
}

void
MinstrelHtWifiManager::RateInit (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  NS_LOG_DEBUG ("RateInit=" << station);

  station->m_groupsTable = McsGroupData (m_numGroups);

  /**
  * Initialize groups supported by both transmitter and receiver.
  * Remember that we check again the transmitter because of late
  * initialization of some parameters.
  */
  NS_LOG_DEBUG ("Supported groups by station:");
  for (uint32_t j = 0; j < m_numGroups; j++)
    {
      station->m_groupsTable[j].m_supported = false;
      if (m_minstrelGroups[j].isVht || (!m_minstrelGroups[j].isVht && !m_useVhtOnly))       ///If it is an HT MCS, check if VHT only is disabled
        {
          if (!(!GetPhy ()->GetGuardInterval () && m_minstrelGroups[j].sgi)                   ///Is SGI supported by the transmitter?
              && (GetPhy ()->GetChannelWidth () >= m_minstrelGroups[j].chWidth)               ///Is channel width supported by the transmitter?
              && (GetPhy ()->GetNumberOfTransmitAntennas () >= m_minstrelGroups[j].streams)) ///Are streams supported by the transmitter?
            {
              if (!(!GetVhtSupported (station) && m_minstrelGroups[j].isVht)                 ///Is VHT supported by the receiver?
                  && !(!GetShortGuardInterval (station) && m_minstrelGroups[j].sgi)         ///Is SGI supported by the receiver?
                  && (GetChannelWidth (station) >= m_minstrelGroups[j].chWidth)             ///Is channel width supported by the receiver?
                  && (GetNumberOfSupportedRxAntennas (station) >= m_minstrelGroups[j].streams)) ///Are streams supported by the receiver?
                {
                  NS_LOG_DEBUG ("Group " << j << ": (" << (uint32_t)m_minstrelGroups[j].streams <<
                                "," << (uint32_t)m_minstrelGroups[j].sgi << "," << m_minstrelGroups[j].chWidth << ")");

                  station->m_groupsTable[j].m_supported = true;                                ///Group supported.

                  station->m_groupsTable[j].m_ratesTable = HtMinstrelRate (station->m_nModes);
                  station->m_groupsTable[j].m_col = 0;
                  station->m_groupsTable[j].m_index = 0;

                  // Initialize all modes supported by the group.
                  for (uint32_t i = 0; i < station->m_nModes; i++)
                    {
                      NS_LOG_DEBUG ("Mode " << i << ": " << GetMcsSupported (station,i));
                      ///Check for invalid VHT MCS
                      if  (IsValidMcs (GetPhy (), m_minstrelGroups[j].streams,
                                       m_minstrelGroups[j].chWidth, GetMcsSupported (station,i)))
                        {
                          station->m_groupsTable[j].m_ratesTable[i].supported = true;
                          station->m_groupsTable[j].m_ratesTable[i].numRateAttempt = 0;
                          station->m_groupsTable[j].m_ratesTable[i].numRateSuccess = 0;
                          station->m_groupsTable[j].m_ratesTable[i].prob = 0;
                          station->m_groupsTable[j].m_ratesTable[i].ewmaProb = 0;
                          station->m_groupsTable[j].m_ratesTable[i].prevNumRateAttempt = 0;
                          station->m_groupsTable[j].m_ratesTable[i].prevNumRateSuccess = 0;
                          station->m_groupsTable[j].m_ratesTable[i].numSamplesSkipped = 0;
                          station->m_groupsTable[j].m_ratesTable[i].successHist = 0;
                          station->m_groupsTable[j].m_ratesTable[i].attemptHist = 0;
                          station->m_groupsTable[j].m_ratesTable[i].throughput = 0;
                          station->m_groupsTable[j].m_ratesTable[i].perfectTxTime = GetFirstMpduTxTime (j, GetMcsSupported (station,i));
                          station->m_groupsTable[j].m_ratesTable[i].retryCount = 0;
                          station->m_groupsTable[j].m_ratesTable[i].adjustedRetryCount = 0;
                          CalculateRetransmits (station, j, i);
                        }
                      else
                        {
                          station->m_groupsTable[j].m_ratesTable[i].supported = false;
                        }
                    }
                }
            }
        }
    }
  SetNextSample (station);                  /// Select the initial sample index.
  UpdateStats (station);                    /// Calculate the initial high throughput rates.
  station->m_txrate = FindRate (station);   /// Select the rate to use.
}

void
MinstrelHtWifiManager::CalculateRetransmits (MinstrelHtWifiRemoteStation *station, uint32_t index)
{
  NS_LOG_FUNCTION (this << station << index);
  uint32_t groupId = GetGroupId (index);
  uint32_t rateId = GetRateId (index);
  if (!station->m_groupsTable[groupId].m_ratesTable[rateId].retryUpdated)
    {
      CalculateRetransmits (station, groupId, rateId);
    }
}

void
MinstrelHtWifiManager::CalculateRetransmits (MinstrelHtWifiRemoteStation *station, uint32_t groupId, uint32_t rateId)
{
  NS_LOG_FUNCTION (this << station << groupId << groupId);
  NS_LOG_DEBUG (" Calculating the number of retries");

  uint32_t cw = 15;                     // Is an approximation.
  uint32_t cwMax = 1023;
  Time cwTime, txTime, dataTxTime;
  Time slotTime = GetMac ()->GetSlot ();
  Time ackTime = GetMac ()->GetBasicBlockAckTimeout ();

  if (station->m_groupsTable[groupId].m_ratesTable[rateId].ewmaProb < 1)
    {
      station->m_groupsTable[groupId].m_ratesTable[rateId].retryCount = 1;
    }
  else
    {
      station->m_groupsTable[groupId].m_ratesTable[rateId].retryCount = 2;
      station->m_groupsTable[groupId].m_ratesTable[rateId].retryUpdated = true;

      dataTxTime = GetFirstMpduTxTime (groupId, GetMcsSupported (station,rateId)) +
        GetMpduTxTime (groupId, GetMcsSupported (station, rateId)) * (station->m_avgAmpduLen - 1);

      /* Contention time for first 2 tries */
      cwTime = (cw / 2) * slotTime;
      cw = Min ((cw + 1) * 2, cwMax);
      cwTime += (cw / 2) * slotTime;
      cw = Min ((cw + 1) * 2, cwMax);

      /* Total TX time for data and Contention after first 2 tries */
      txTime = cwTime + 2 * (dataTxTime + ackTime);

      /* See how many more tries we can fit inside segment size */
      do
        {
          /* Contention time for this try */
          cwTime = (cw / 2) * slotTime;
          cw = Min ((cw + 1) * 2, cwMax);

          /* Total TX time after this try */
          txTime += cwTime + ackTime + dataTxTime;
        }
      while ((txTime < MilliSeconds (6))
             && (++station->m_groupsTable[groupId].m_ratesTable[rateId].retryCount < 7));
    }
}

double
MinstrelHtWifiManager::CalculateEwmsd (double oldEwmsd, double currentProb, double ewmaProb, uint32_t weight)
{
  double diff, incr, tmp;

  /* calculate exponential weighted moving variance */
  diff = currentProb - ewmaProb;
  incr = (100 - weight) * diff / 100;
  tmp = oldEwmsd * oldEwmsd;
  tmp = weight * (tmp + diff * incr) / 100;

  /* return standard deviation */
  return sqrt (tmp);
}

void
MinstrelHtWifiManager::PrintTable (MinstrelHtWifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  NS_LOG_DEBUG ("PrintTable=" << station);

  station->m_statsFile << "               best   ____________rate__________    ________statistics________    ________last_______    ______sum-of________\n" <<
    " mode guard #  rate  [name   idx airtime  max_tp]  [avg(tp) avg(prob) sd(prob)]  [prob.|retry|suc|att]  [#success | #attempts]\n";
  for (uint32_t i = 0; i < m_numGroups; i++)
    {
      StatsDump (station, i, station->m_statsFile);
    }

  station->m_statsFile << "\nTotal packet count::    ideal " << Max (0, station->m_totalPacketsCount - station->m_samplePacketsCount) <<
    "              lookaround " << station->m_samplePacketsCount << "\n";
  station->m_statsFile << "Average # of aggregated frames per A-MPDU: " << station->m_avgAmpduLen << "\n\n";

  station->m_statsFile.flush ();
}

void
MinstrelHtWifiManager::StatsDump (MinstrelHtWifiRemoteStation *station, uint32_t groupId, std::ofstream &of)
{
  uint32_t numRates = station->m_nModes;
  McsGroup group = m_minstrelGroups[groupId];
  Time txTime;
  char giMode;
  if (group.sgi)
    {
      giMode = 'S';
    }
  else
    {
      giMode = 'L';
    }
  for (uint32_t i = 0; i < numRates; i++)
    {
      if (station->m_groupsTable[groupId].m_supported && station->m_groupsTable[groupId].m_ratesTable[i].supported)
        {
          if (!group.isVht)
            {
              of << "HT" << group.chWidth << "   " << giMode << "GI  " << (int)group.streams << "   ";
            }
          else
            {
              of << "VHT" << group.chWidth << "   " << giMode << "GI  " << (int)group.streams << "   ";
            }

          uint32_t maxTpRate = station->m_maxTpRate;
          uint32_t maxTpRate2 = station->m_maxTpRate2;
          uint32_t maxProbRate = station->m_maxProbRate;

          uint32_t idx = GetIndex (groupId, i);
          if (idx == maxTpRate)
            {
              of << 'A';
            }
          else
            {
              of << ' ';
            }
          if (idx == maxTpRate2)
            {
              of << 'B';
            }
          else
            {
              of << ' ';
            }
          if (idx == maxProbRate)
            {
              of << 'P';
            }
          else
            {
              of << ' ';
            }

          if (!group.isVht)
            {
              of << std::setw (4) << "   MCS" << (group.streams - 1) * 8 + i;
            }
          else
            {
              of << std::setw (7) << "   MCS" << i << "/" << (int) group.streams;
            }

          of << "  " << std::setw (3) << idx << "  ";

          /* tx_time[rate(i)] in usec */
          txTime = GetFirstMpduTxTime (groupId, GetMcsSupported (station, i));
          of << std::setw (6) << txTime.GetMicroSeconds () << "  ";

          of << std::setw (7) << CalculateThroughput (station, groupId, i, 100) / 100 << "   " <<
            std::setw (7) << station->m_groupsTable[groupId].m_ratesTable[i].throughput / 100 << "   " <<
            std::setw (7) << station->m_groupsTable[groupId].m_ratesTable[i].ewmaProb << "  " <<
            std::setw (7) << station->m_groupsTable[groupId].m_ratesTable[i].ewmsdProb << "  " <<
            std::setw (7) << station->m_groupsTable[groupId].m_ratesTable[i].prob << "  " <<
            std::setw (2) << station->m_groupsTable[groupId].m_ratesTable[i].retryCount << "   " <<
            std::setw (3) << station->m_groupsTable[groupId].m_ratesTable[i].prevNumRateSuccess << "  " <<
            std::setw (3) << station->m_groupsTable[groupId].m_ratesTable[i].prevNumRateAttempt << "   " <<
            std::setw (9) << station->m_groupsTable[groupId].m_ratesTable[i].successHist << "   " <<
            std::setw (9) << station->m_groupsTable[groupId].m_ratesTable[i].attemptHist << "\n";
        }
    }
}


uint32_t
MinstrelHtWifiManager::GetRateId (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);

  uint32_t id;
  id = index % m_numRates;
  return id;
}

uint32_t
MinstrelHtWifiManager::GetGroupId (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);

  return index / m_numRates;
}

uint32_t
MinstrelHtWifiManager::GetGroupId (uint8_t txstreams, uint8_t sgi, uint32_t chWidth)
{
  NS_LOG_FUNCTION (this << (int)txstreams << (int)sgi << chWidth);

  return MAX_SUPPORTED_STREAMS * 2 * (chWidth == 160 ? 3 : chWidth == 80 ? 2 : chWidth == 40 ? 1 : 0) + MAX_SUPPORTED_STREAMS * sgi + txstreams - 1;
}

uint32_t
MinstrelHtWifiManager::GetVhtGroupId (uint8_t txstreams, uint8_t sgi, uint32_t chWidth)
{
  NS_LOG_FUNCTION (this << (int)txstreams << (int)sgi << chWidth);

  return MAX_SUPPORTED_STREAMS * 2 * (chWidth == 160 ? 3 : chWidth == 80 ? 2 : chWidth == 40 ? 1 : 0) + MAX_SUPPORTED_STREAMS * sgi + txstreams - 1;
}

} // namespace ns3





