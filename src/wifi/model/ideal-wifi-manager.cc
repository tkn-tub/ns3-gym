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

#include "ideal-wifi-manager.h"
#include "wifi-phy.h"
#include "ns3/log.h"

namespace ns3 {

/**
 * \brief hold per-remote-station state for Ideal Wifi manager.
 *
 * This struct extends from WifiRemoteStation struct to hold additional
 * information required by the Ideal Wifi manager
 */
struct IdealWifiRemoteStation : public WifiRemoteStation
{
  double m_lastSnrObserved;  //!< SNR of most recently reported packet sent to the remote station
  double m_lastSnrCached;    //!< SNR most recently used to select a rate
  double m_nss;          //!< SNR most recently used to select a rate
  WifiMode m_lastMode;       //!< Mode most recently used to the remote station
};

NS_OBJECT_ENSURE_REGISTERED (IdealWifiManager);

NS_LOG_COMPONENT_DEFINE ("IdealWifiManager");

TypeId
IdealWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::IdealWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .SetGroupName ("Wifi")
    .AddConstructor<IdealWifiManager> ()
    .AddAttribute ("BerThreshold",
                   "The maximum Bit Error Rate acceptable at any transmission mode",
                   DoubleValue (1e-5),
                   MakeDoubleAccessor (&IdealWifiManager::m_ber),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Rate",
                     "Traced value for rate changes (b/s)",
                     MakeTraceSourceAccessor (&IdealWifiManager::m_currentRate),
                     "ns3::TracedValueCallback::Uint64")
  ;
  return tid;
}

IdealWifiManager::IdealWifiManager ()
  : m_currentRate (0)
{
}

IdealWifiManager::~IdealWifiManager ()
{
}

void
IdealWifiManager::SetupPhy (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  WifiRemoteStationManager::SetupPhy (phy);
}

uint8_t
IdealWifiManager::GetChannelWidthForMode (WifiMode mode) const
{
  NS_ASSERT (mode.GetModulationClass () != WIFI_MOD_CLASS_HT);
  NS_ASSERT (mode.GetModulationClass () != WIFI_MOD_CLASS_VHT);
  if (mode.GetModulationClass () == WIFI_MOD_CLASS_DSSS
      || mode.GetModulationClass () == WIFI_MOD_CLASS_HR_DSSS)
    {
      return 22;
    }
  else
    {
      return 20;
    }
}

void
IdealWifiManager::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
  WifiMode mode;
  WifiTxVector txVector;
  uint8_t nss = 1;
  uint32_t nModes = GetPhy ()->GetNModes ();
  txVector.SetGuardInterval (GetPhy ()->GetShortGuardInterval () ? 400 : 800);
  for (uint32_t i = 0; i < nModes; i++)
    {
      mode = GetPhy ()->GetMode (i);
      txVector.SetChannelWidth (GetChannelWidthForMode (mode));
      txVector.SetNss (nss);
      txVector.SetMode (mode);
      NS_LOG_DEBUG ("Initialize, adding mode = " << mode.GetUniqueName () <<
                    " channel width " << (uint16_t) txVector.GetChannelWidth () <<
                    " nss " << (uint16_t) nss <<
                    " short GI " << GetPhy ()->GetShortGuardInterval ());
      AddSnrThreshold (txVector, GetPhy ()->CalculateSnr (txVector, m_ber));
    }
  // Add all Ht and Vht MCSes
  txVector.SetChannelWidth (GetPhy ()->GetChannelWidth ());
  if (HasVhtSupported () == true || HasHtSupported () == true )
    {
      nModes = GetPhy ()->GetNMcs ();
      for (uint32_t i = 0; i < nModes; i++)
        {
          mode = GetPhy ()->GetMcs (i);
          if (mode.GetModulationClass () == WIFI_MOD_CLASS_HT)
            {
              //derive NSS from the Mcs index
              nss = (mode.GetMcsValue () / 8) + 1;
              NS_LOG_DEBUG ("Initialize, adding mode = " << mode.GetUniqueName () <<
                            " channel width " << (uint16_t) GetPhy ()->GetChannelWidth () <<
                            " nss " << (uint16_t) nss <<
                            " short GI " << GetPhy ()->GetShortGuardInterval ());
              NS_LOG_DEBUG ("In SetupPhy, adding mode = " << mode.GetUniqueName ());
              txVector.SetNss (nss);
              txVector.SetMode (mode);
              AddSnrThreshold (txVector, GetPhy ()->CalculateSnr (txVector, m_ber));
            }
          else
            {
              uint8_t maxNss = GetPhy ()->GetMaxSupportedTxSpatialStreams ();
              for (uint8_t i = 1; i <= maxNss; i++)
                {
                  NS_LOG_DEBUG ("Initialize, adding mode = " << mode.GetUniqueName () <<
                                " channel width " << (uint16_t) GetPhy ()->GetChannelWidth () <<
                                " nss " << (uint16_t) i <<
                                " short GI " << GetPhy ()->GetShortGuardInterval ());
                  NS_LOG_DEBUG ("In SetupPhy, adding mode = " << mode.GetUniqueName ());
                  txVector.SetNss (i);
                  txVector.SetMode (mode);
                  AddSnrThreshold (txVector, GetPhy ()->CalculateSnr (txVector, m_ber));
                }
            }
        }
    }
}

double
IdealWifiManager::GetSnrThreshold (WifiTxVector txVector) const
{
  NS_LOG_FUNCTION (this << txVector.GetMode ().GetUniqueName ());
  for (Thresholds::const_iterator i = m_thresholds.begin (); i != m_thresholds.end (); i++)
    {
      NS_LOG_DEBUG ("Checking " << i->second.GetMode ().GetUniqueName () <<
                    " nss " << (uint16_t) i->second.GetNss () <<
                    " width " << (uint16_t) i->second.GetChannelWidth ());
      NS_LOG_DEBUG ("against TxVector " << txVector.GetMode ().GetUniqueName () <<
                    " nss " << (uint16_t) txVector.GetNss () <<
                    " width " << (uint16_t) txVector.GetChannelWidth ());
      if (txVector.GetMode () == i->second.GetMode ()
          && txVector.GetNss () == i->second.GetNss ()
          && txVector.GetGuardInterval () == i->second.GetGuardInterval ()
          && txVector.GetChannelWidth () == i->second.GetChannelWidth ())
        {
          return i->first;
        }
    }
  NS_ASSERT (false);
  return 0.0;
}

void
IdealWifiManager::AddSnrThreshold (WifiTxVector txVector, double snr)
{
  NS_LOG_FUNCTION (this << txVector.GetMode ().GetUniqueName () << snr);
  m_thresholds.push_back (std::make_pair (snr, txVector));
}

WifiRemoteStation *
IdealWifiManager::DoCreateStation (void) const
{
  NS_LOG_FUNCTION (this);
  IdealWifiRemoteStation *station = new IdealWifiRemoteStation ();
  station->m_lastSnrObserved = 0.0;
  station->m_lastSnrCached = 0.0;
  station->m_lastMode = GetDefaultMode ();
  station->m_nss = 1;
  return station;
}


void
IdealWifiManager::DoReportRxOk (WifiRemoteStation *station,
                                double rxSnr, WifiMode txMode)
{
}

void
IdealWifiManager::DoReportRtsFailed (WifiRemoteStation *station)
{
}

void
IdealWifiManager::DoReportDataFailed (WifiRemoteStation *station)
{
}

void
IdealWifiManager::DoReportRtsOk (WifiRemoteStation *st,
                                 double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << st << ctsSnr << ctsMode.GetUniqueName () << rtsSnr);
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  station->m_lastSnrObserved = rtsSnr;
}

void
IdealWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                  double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << st << ackSnr << ackMode.GetUniqueName () << dataSnr);
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  if (dataSnr == 0)
    {
      NS_LOG_WARN ("DataSnr reported to be zero; not saving this report.");
      return;
    }
  station->m_lastSnrObserved = dataSnr;
}

void
IdealWifiManager::DoReportAmpduTxStatus (WifiRemoteStation *st, uint8_t nSuccessfulMpdus, uint8_t nFailedMpdus, double rxSnr, double dataSnr)
{
  NS_LOG_FUNCTION (this << st << (uint16_t)nSuccessfulMpdus << (uint16_t)nFailedMpdus << rxSnr << dataSnr);
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  if (dataSnr == 0)
    {
      NS_LOG_WARN ("DataSnr reported to be zero; not saving this report.");
      return;
    }
  station->m_lastSnrObserved = dataSnr;
}


void
IdealWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *station)
{
}

void
IdealWifiManager::DoReportFinalDataFailed (WifiRemoteStation *station)
{
}

WifiTxVector
IdealWifiManager::DoGetDataTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  //We search within the Supported rate set the mode with the
  //highest snr threshold possible which is smaller than m_lastSnr
  //to ensure correct packet delivery.
  double maxThreshold = 0.0;
  WifiMode maxMode = GetDefaultMode ();
  std::vector<WifiTxVector> candidateTxVectors;
  WifiTxVector txVector;
  WifiMode mode;
  uint8_t selectedNss = 1;
  txVector.SetChannelWidth (GetPhy ()->GetChannelWidth ());
  txVector.SetGuardInterval (GetPhy ()->GetShortGuardInterval () ? 400 : 800);
  if (station->m_lastSnrObserved == station->m_lastSnrCached)
    {
      // SNR has not changed, so skip the search and use the last
      // mode selected
      maxMode = station->m_lastMode;
      selectedNss = station->m_nss;
      NS_LOG_DEBUG ("Using cached mode = " << maxMode.GetUniqueName () <<
                    " last snr observed " << station->m_lastSnrObserved <<
                    " cached " << station->m_lastSnrCached);
    }
  else
    {
      if ((HasVhtSupported () == true || HasHtSupported () == true)
          && (GetHtSupported (st) == true || GetVhtSupported (st) == true))
        {
          for (uint32_t i = 0; i < GetNMcsSupported (station); i++)
            {
              mode = GetMcsSupported (station, i);
              txVector.SetMode (mode);
              if (mode.GetModulationClass () == WIFI_MOD_CLASS_HT)
                {
                  // If the node and peer are both VHT capable, only search VHT modes
                  if (HasVhtSupported () && GetVhtSupported (station))
                    {
                      continue;
                    }
                  // Derive NSS from the MCS index. There is a different mode for each possible NSS value.
                  uint8_t nss = (mode.GetMcsValue () / 8) + 1;
                  txVector.SetNss (nss);
                  if (WifiPhy::IsValidTxVector (txVector) == false)
                    {
                      NS_LOG_DEBUG ("Skipping mode " << mode.GetUniqueName () <<
                                    " nss " << (uint16_t) nss << " width " <<
                                    (uint16_t) txVector.GetChannelWidth ());
                      continue;
                    }
                  double threshold = GetSnrThreshold (txVector);
                  NS_LOG_DEBUG ("Testing mode = " << mode.GetUniqueName () <<
                                " threshold " << threshold  << " maxThreshold " <<
                                maxThreshold << " last snr observed " <<
                                station->m_lastSnrObserved << " cached " <<
                                station->m_lastSnrCached);
                  if (threshold > maxThreshold && threshold < station->m_lastSnrObserved)
                    {
                      NS_LOG_DEBUG ("Candidate mode = " << mode.GetUniqueName () <<
                                    " threshold " << threshold  <<
                                    " last snr observed " <<
                                    station->m_lastSnrObserved);
                      maxThreshold = threshold;
                      maxMode = mode;
                      selectedNss = nss;
                    }
                 }
              else
                {
                  // If the node and peer are not both VHT capable, only search HT modes
                  if (!HasVhtSupported () || !GetVhtSupported (station))
                    {
                      continue;
                    }
                  uint8_t maxNss = GetPhy ()->GetMaxSupportedTxSpatialStreams ();
                  for (uint8_t nss = 1; nss <= maxNss; nss++)
                    {
                      // If the peer does not support more streams, stop searching.
                      if (GetNumberOfSupportedStreams (station) < nss)
                        {
                          break;
                        }
                      txVector.SetNss (nss);
                      if (WifiPhy::IsValidTxVector (txVector) == false)
                        {
                          NS_LOG_DEBUG ("Skipping mode " << mode.GetUniqueName () <<
                                        " nss " << (uint16_t) nss << " width " <<
                                        (uint16_t) txVector.GetChannelWidth ());
                          continue;
                        }
                      double threshold = GetSnrThreshold (txVector);
                      NS_LOG_DEBUG ("Testing mode = " << mode.GetUniqueName () <<
                                    " threshold " << threshold  << " maxThreshold " <<
                                    maxThreshold << " last snr observed " <<
                                    station->m_lastSnrObserved << " cached " <<
                                    station->m_lastSnrCached);
                      if (threshold > maxThreshold && threshold < station->m_lastSnrObserved)
                        {
                          NS_LOG_DEBUG ("Candidate mode = " << mode.GetUniqueName () <<
                                        " threshold " << threshold  <<
                                        " last snr observed " <<
                                        station->m_lastSnrObserved);
                          maxThreshold = threshold;
                          maxMode = mode;
                          selectedNss = nss;
                        }
                    }
                }
            }
        }
      else
        {
          // Non-HT selection
          selectedNss = 1;
          for (uint32_t i = 0; i < GetNSupported (station); i++)
            {
              mode = GetSupported (station, i);
              txVector.SetMode (mode);
              txVector.SetNss (selectedNss);
              txVector.SetChannelWidth (GetChannelWidthForMode (mode));
              double threshold = GetSnrThreshold (txVector);
              NS_LOG_DEBUG ("mode = " << mode.GetUniqueName () <<
                            " threshold " << threshold  <<
                            " last snr observed " <<
                            station->m_lastSnrObserved);
              if (threshold > maxThreshold && threshold < station->m_lastSnrObserved)
                {
                  NS_LOG_DEBUG ("Candidate mode = " << mode.GetUniqueName () <<
                                " threshold " << threshold  <<
                                " last snr observed " <<
                                station->m_lastSnrObserved);
                  maxThreshold = threshold;
                  maxMode = mode;
                }
            }
        }
      NS_LOG_DEBUG ("Updating cached values for station to " <<  maxMode.GetUniqueName () << " snr " << station->m_lastSnrObserved);
      station->m_lastSnrCached = station->m_lastSnrObserved;
      station->m_lastMode = maxMode;
      station->m_nss = selectedNss;
    }
  uint8_t channelWidth = GetChannelWidth (station);
  NS_LOG_DEBUG ("Found maxMode: " << maxMode << " channelWidth: " << (uint16_t) channelWidth);
  if (m_currentRate != maxMode.GetDataRate (channelWidth, GetPhy ()->GetShortGuardInterval () ? 400 : 800, selectedNss))
    {
      NS_LOG_DEBUG ("New datarate: " << maxMode.GetDataRate (channelWidth, GetPhy ()->GetShortGuardInterval () ? 400 : 800, selectedNss));
      m_currentRate = maxMode.GetDataRate (channelWidth, GetPhy ()->GetShortGuardInterval () ? 400 : 800, selectedNss);
    }
  return WifiTxVector (maxMode, GetDefaultTxPowerLevel (), GetLongRetryCount (station), GetPreambleForTransmission (maxMode, GetAddress (station)), 800, GetNumberOfAntennas (), selectedNss, 0, channelWidth, GetAggregation (station), false);
}

WifiTxVector
IdealWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  //We search within the Basic rate set the mode with the highest
  //snr threshold possible which is smaller than m_lastSnr to
  //ensure correct packet delivery.
  double maxThreshold = 0.0;
  WifiTxVector txVector;
  WifiMode mode;
  uint8_t nss = 1;
  WifiMode maxMode = GetDefaultMode ();
  //avoid to use legacy rate adaptation algorithms for IEEE 802.11n/ac
  // RTS is sent in a non-HT frame; RTS with (V)HT is not yet supported
  txVector.SetGuardInterval (GetPhy ()->GetShortGuardInterval () ? 400 : 800);
  for (uint32_t i = 0; i < GetNBasicModes (); i++)
    {
      mode = GetBasicMode (i);
      txVector.SetMode (mode);
      txVector.SetNss (nss);
      txVector.SetChannelWidth (GetChannelWidthForMode (mode));
      double threshold = GetSnrThreshold (txVector);
      if (threshold > maxThreshold && threshold < station->m_lastSnrObserved)
        {
          maxThreshold = threshold;
          maxMode = mode;
        }
    }
  return WifiTxVector (maxMode, GetDefaultTxPowerLevel (), GetShortRetryCount (station), GetPreambleForTransmission (maxMode, GetAddress (station)), 800, GetNumberOfAntennas (), nss, 0, GetChannelWidthForMode (maxMode), GetAggregation (station), false);
}

bool
IdealWifiManager::IsLowLatency (void) const
{
  return true;
}

void
IdealWifiManager::SetHeSupported (bool enable)
{
  //HE is not supported yet by this algorithm.
  if (enable)
    {
      NS_FATAL_ERROR ("WifiRemoteStationManager selected does not support HE rates");
    }
}

} //namespace ns3
