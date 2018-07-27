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

#include "ns3/log.h"
#include "ideal-wifi-manager.h"
#include "wifi-phy.h"

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
  uint8_t m_nss;             //!< Number of spacial streams
  WifiMode m_lastMode;       //!< Mode most recently used to the remote station
};

/// To avoid using the cache before a valid value has been cached
static const double CACHE_INITIAL_VALUE = -100;

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
  NS_LOG_FUNCTION (this);
}

IdealWifiManager::~IdealWifiManager ()
{
  NS_LOG_FUNCTION (this);
}

void
IdealWifiManager::SetupPhy (const Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  WifiRemoteStationManager::SetupPhy (phy);
}

uint16_t
IdealWifiManager::GetChannelWidthForMode (WifiMode mode) const
{
  NS_ASSERT (mode.GetModulationClass () != WIFI_MOD_CLASS_HT
             && mode.GetModulationClass () != WIFI_MOD_CLASS_VHT
             && mode.GetModulationClass () != WIFI_MOD_CLASS_HE);
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
  uint8_t nModes = GetPhy ()->GetNModes ();
  for (uint8_t i = 0; i < nModes; i++)
    {
      mode = GetPhy ()->GetMode (i);
      txVector.SetChannelWidth (GetChannelWidthForMode (mode));
      txVector.SetNss (nss);
      txVector.SetMode (mode);
      NS_LOG_DEBUG ("Initialize, adding mode = " << mode.GetUniqueName ());
      AddSnrThreshold (txVector, GetPhy ()->CalculateSnr (txVector, m_ber));
    }
  // Add all Ht and Vht MCSes
  if (HasVhtSupported () == true || HasHtSupported () == true || HasHeSupported () == true)
    {
      nModes = GetPhy ()->GetNMcs ();
      for (uint8_t i = 0; i < nModes; i++)
        {
          for (uint16_t j = 20; j <= GetPhy ()->GetChannelWidth (); j *= 2)
            {
              txVector.SetChannelWidth (j);
              mode = GetPhy ()->GetMcs (i);
              if (mode.GetModulationClass () == WIFI_MOD_CLASS_HT)
                {
                  uint16_t guardInterval = GetPhy ()->GetShortGuardInterval () ? 400 : 800;
                  txVector.SetGuardInterval (guardInterval);
                  //derive NSS from the MCS index
                  nss = (mode.GetMcsValue () / 8) + 1;
                  NS_LOG_DEBUG ("Initialize, adding mode = " << mode.GetUniqueName () <<
                                " channel width " << j <<
                                " nss " << +nss <<
                                " GI " << guardInterval);
                  NS_LOG_DEBUG ("In SetupPhy, adding mode = " << mode.GetUniqueName ());
                  txVector.SetNss (nss);
                  txVector.SetMode (mode);
                  AddSnrThreshold (txVector, GetPhy ()->CalculateSnr (txVector, m_ber));
                }
              else //VHT or HE
                {
                  uint16_t guardInterval;
                  if (mode.GetModulationClass () == WIFI_MOD_CLASS_VHT)
                    {
                      guardInterval = GetPhy ()->GetShortGuardInterval () ? 400 : 800;
                    }
                  else
                    {
                      guardInterval = static_cast<uint16_t> (GetPhy ()->GetGuardInterval ().GetNanoSeconds ());
                    }
                  txVector.SetGuardInterval (guardInterval);
                  for (uint8_t k = 1; k <= GetPhy ()->GetMaxSupportedTxSpatialStreams (); k++)
                    {
                      NS_LOG_DEBUG ("Initialize, adding mode = " << mode.GetUniqueName () <<
                                    " channel width " << j <<
                                    " nss " << +k <<
                                    " GI " << guardInterval);
                      NS_LOG_DEBUG ("In SetupPhy, adding mode = " << mode.GetUniqueName ());
                      txVector.SetNss (k);
                      txVector.SetMode (mode);
                      AddSnrThreshold (txVector, GetPhy ()->CalculateSnr (txVector, m_ber));
                    }
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
                    " nss " << +i->second.GetNss () <<
                    " GI " << i->second.GetGuardInterval () <<
                    " width " << i->second.GetChannelWidth ());
      NS_LOG_DEBUG ("against TxVector " << txVector.GetMode ().GetUniqueName () <<
                    " nss " << +txVector.GetNss () <<
                    " GI " << txVector.GetGuardInterval () <<
                    " width " << txVector.GetChannelWidth ());
      if (txVector.GetMode () == i->second.GetMode ()
          && txVector.GetNss () == i->second.GetNss ()
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
  station->m_lastSnrCached = CACHE_INITIAL_VALUE;
  station->m_lastMode = GetDefaultMode ();
  station->m_nss = 1;
  return station;
}


void
IdealWifiManager::DoReportRxOk (WifiRemoteStation *station, double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << station << rxSnr << txMode);
}

void
IdealWifiManager::DoReportRtsFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}

void
IdealWifiManager::DoReportDataFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
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
  NS_LOG_FUNCTION (this << st << +nSuccessfulMpdus << +nFailedMpdus << rxSnr << dataSnr);
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
  NS_LOG_FUNCTION (this << station);
  IdealWifiRemoteStation *st = (IdealWifiRemoteStation *)station;
  st->m_lastSnrObserved = 0.0;
}

void
IdealWifiManager::DoReportFinalDataFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
  IdealWifiRemoteStation *st = (IdealWifiRemoteStation *)station;
  st->m_lastSnrObserved = 0.0;
}

WifiTxVector
IdealWifiManager::DoGetDataTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  //We search within the Supported rate set the mode with the
  //highest data rate for which the snr threshold is smaller than m_lastSnr
  //to ensure correct packet delivery.
  WifiMode maxMode = GetDefaultMode ();
  WifiTxVector txVector;
  WifiMode mode;
  uint64_t bestRate = 0;
  uint8_t selectedNss = 1;
  uint16_t guardInterval;
  uint16_t channelWidth = std::min (GetChannelWidth (station), GetPhy ()->GetChannelWidth ());
  txVector.SetChannelWidth (channelWidth);
  if (station->m_lastSnrCached != CACHE_INITIAL_VALUE && station->m_lastSnrObserved == station->m_lastSnrCached)
    {
      // SNR has not changed, so skip the search and use the last
      // mode selected
      maxMode = station->m_lastMode;
      selectedNss = station->m_nss;
      NS_LOG_DEBUG ("Using cached mode = " << maxMode.GetUniqueName () <<
                    " last snr observed " << station->m_lastSnrObserved <<
                    " cached " << station->m_lastSnrCached <<
                    " nss " << +selectedNss);
    }
  else
    {
      if ((HasVhtSupported () == true || HasHtSupported () == true || HasHeSupported () == true)
          && (GetHtSupported (st) == true || GetVhtSupported (st) == true || GetHeSupported (st) == true))
        {
          for (uint8_t i = 0; i < GetNMcsSupported (station); i++)
            {
              mode = GetMcsSupported (station, i);
              txVector.SetMode (mode);
              if (mode.GetModulationClass () == WIFI_MOD_CLASS_HT)
                {
                  guardInterval = static_cast<uint16_t> (std::max (GetShortGuardInterval (station) ? 400 : 800, GetPhy ()->GetShortGuardInterval () ? 400 : 800));
                  txVector.SetGuardInterval (guardInterval);
                  // If the node and peer are both VHT capable, only search VHT modes
                  if (HasVhtSupported () && GetVhtSupported (station))
                    {
                      continue;
                    }
                  // If the node and peer are both HE capable, only search HE modes
                  if (HasHeSupported () && GetHeSupported (station))
                    {
                      continue;
                    }
                  // Derive NSS from the MCS index. There is a different mode for each possible NSS value.
                  uint8_t nss = (mode.GetMcsValue () / 8) + 1;
                  txVector.SetNss (nss);
                  if (!txVector.IsValid ()
                      || nss > std::min (GetMaxNumberOfTransmitStreams (), GetNumberOfSupportedStreams (st)))
                    {
                      NS_LOG_DEBUG ("Skipping mode " << mode.GetUniqueName () <<
                                    " nss " << +nss <<
                                    " width " << txVector.GetChannelWidth ());
                      continue;
                    }
                  double threshold = GetSnrThreshold (txVector);
                  uint64_t dataRate = mode.GetDataRate (txVector.GetChannelWidth (), txVector.GetGuardInterval (), nss);
                  NS_LOG_DEBUG ("Testing mode " << mode.GetUniqueName () <<
                                " data rate " << dataRate <<
                                " threshold " << threshold  << " last snr observed " <<
                                station->m_lastSnrObserved << " cached " <<
                                station->m_lastSnrCached);
                  if (dataRate > bestRate && threshold < station->m_lastSnrObserved)
                    {
                      NS_LOG_DEBUG ("Candidate mode = " << mode.GetUniqueName () <<
                                    " data rate " << dataRate <<
                                    " threshold " << threshold  <<
                                    " last snr observed " <<
                                    station->m_lastSnrObserved);
                      bestRate = dataRate;
                      maxMode = mode;
                      selectedNss = nss;
                    }
                }
              else if (mode.GetModulationClass () == WIFI_MOD_CLASS_VHT)
                {
                  guardInterval = static_cast<uint16_t> (std::max (GetShortGuardInterval (station) ? 400 : 800, GetPhy ()->GetShortGuardInterval () ? 400 : 800));
                  txVector.SetGuardInterval (guardInterval);
                  // If the node and peer are both HE capable, only search HE modes
                  if (HasHeSupported () && GetHeSupported (station))
                    {
                      continue;
                    }
                  // If the node and peer are not both VHT capable, only search HT modes
                  if (!HasVhtSupported () || !GetVhtSupported (station))
                    {
                      continue;
                    }
                  for (uint8_t nss = 1; nss <= std::min (GetMaxNumberOfTransmitStreams (), GetNumberOfSupportedStreams (station)); nss++)
                    {
                      txVector.SetNss (nss);
                      if (!txVector.IsValid ())
                        {
                          NS_LOG_DEBUG ("Skipping mode " << mode.GetUniqueName () <<
                                        " nss " << +nss <<
                                        " width " << txVector.GetChannelWidth ());
                          continue;
                        }
                      double threshold = GetSnrThreshold (txVector);
                      uint64_t dataRate = mode.GetDataRate (txVector.GetChannelWidth (), txVector.GetGuardInterval (), nss);
                      NS_LOG_DEBUG ("Testing mode = " << mode.GetUniqueName () <<
                                    " data rate " << dataRate <<
                                    " threshold " << threshold << " last snr observed " <<
                                    station->m_lastSnrObserved << " cached " <<
                                    station->m_lastSnrCached);
                      if (dataRate > bestRate && threshold < station->m_lastSnrObserved)
                        {
                          NS_LOG_DEBUG ("Candidate mode = " << mode.GetUniqueName () <<
                                        " data rate " << dataRate <<
                                        " threshold " << threshold  <<
                                        " last snr observed " <<
                                        station->m_lastSnrObserved);
                          bestRate = dataRate;
                          maxMode = mode;
                          selectedNss = nss;
                        }
                    }
                }
              else //HE
                {
                  guardInterval = std::max (GetGuardInterval (station), static_cast<uint16_t> (GetPhy ()->GetGuardInterval ().GetNanoSeconds ()));
                  txVector.SetGuardInterval (guardInterval);
                  // If the node and peer are not both HE capable, only search (V)HT modes
                  if (!HasHeSupported () || !GetHeSupported (station))
                    {
                      continue;
                    }
                  for (uint8_t nss = 1; nss <= GetNumberOfSupportedStreams (station); nss++)
                    {
                      txVector.SetNss (nss);
                      if (!txVector.IsValid ())
                        {
                          NS_LOG_DEBUG ("Skipping mode " << mode.GetUniqueName () <<
                                        " nss " << +nss <<
                                        " width " << +txVector.GetChannelWidth ());
                          continue;
                        }
                      double threshold = GetSnrThreshold (txVector);
                      uint64_t dataRate = mode.GetDataRate (txVector.GetChannelWidth (), txVector.GetGuardInterval (), nss);
                      NS_LOG_DEBUG ("Testing mode = " << mode.GetUniqueName () <<
                                    " data rate " << dataRate <<
                                    " threshold " << threshold  << " last snr observed " <<
                                    station->m_lastSnrObserved << " cached " <<
                                    station->m_lastSnrCached);
                      if (dataRate > bestRate && threshold < station->m_lastSnrObserved)
                        {
                          NS_LOG_DEBUG ("Candidate mode = " << mode.GetUniqueName () <<
                                        " data rate " << dataRate <<
                                        " threshold " << threshold  <<
                                        " last snr observed " <<
                                        station->m_lastSnrObserved);
                          bestRate = dataRate;
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
          for (uint8_t i = 0; i < GetNSupported (station); i++)
            {
              mode = GetSupported (station, i);
              txVector.SetMode (mode);
              txVector.SetNss (selectedNss);
              txVector.SetChannelWidth (GetChannelWidthForMode (mode));
              double threshold = GetSnrThreshold (txVector);
              uint64_t dataRate = mode.GetDataRate (txVector.GetChannelWidth (), txVector.GetGuardInterval (), txVector.GetNss ());
              NS_LOG_DEBUG ("mode = " << mode.GetUniqueName () <<
                            " threshold " << threshold  <<
                            " last snr observed " <<
                            station->m_lastSnrObserved);
              if (dataRate > bestRate && threshold < station->m_lastSnrObserved)
                {
                  NS_LOG_DEBUG ("Candidate mode = " << mode.GetUniqueName () <<
                                " data rate " << dataRate <<
                                " threshold " << threshold  <<
                                " last snr observed " <<
                                station->m_lastSnrObserved);
                  bestRate = dataRate;
                  maxMode = mode;
                }
            }
        }
      NS_LOG_DEBUG ("Updating cached values for station to " <<  maxMode.GetUniqueName () << " snr " << station->m_lastSnrObserved);
      station->m_lastSnrCached = station->m_lastSnrObserved;
      station->m_lastMode = maxMode;
      station->m_nss = selectedNss;
    }
  NS_LOG_DEBUG ("Found maxMode: " << maxMode << " channelWidth: " << channelWidth);
  if (maxMode.GetModulationClass () == WIFI_MOD_CLASS_HE)
    {
      guardInterval = std::max (GetGuardInterval (station), static_cast<uint16_t> (GetPhy ()->GetGuardInterval ().GetNanoSeconds ()));
    }
  else
    {
      guardInterval = static_cast<uint16_t> (std::max (GetShortGuardInterval (station) ? 400 : 800, GetPhy ()->GetShortGuardInterval () ? 400 : 800));
    }
  if (m_currentRate != maxMode.GetDataRate (channelWidth, guardInterval, selectedNss))
    {
      NS_LOG_DEBUG ("New datarate: " << maxMode.GetDataRate (channelWidth, guardInterval, selectedNss));
      m_currentRate = maxMode.GetDataRate (channelWidth, guardInterval, selectedNss);
    }
  return WifiTxVector (maxMode, GetDefaultTxPowerLevel (), GetPreambleForTransmission (maxMode, GetAddress (station)), guardInterval, GetNumberOfAntennas (), selectedNss, 0, GetChannelWidthForTransmission (maxMode, channelWidth), GetAggregation (station), false);
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
  //avoid to use legacy rate adaptation algorithms for IEEE 802.11n/ac/ax
  //RTS is sent in a legacy frame; RTS with HT/VHT/HE is not yet supported
  for (uint8_t i = 0; i < GetNBasicModes (); i++)
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
  return WifiTxVector (maxMode, GetDefaultTxPowerLevel (), GetPreambleForTransmission (maxMode, GetAddress (station)), 800, GetNumberOfAntennas (), nss, 0, GetChannelWidthForMode (maxMode), GetAggregation (station), false);
}

bool
IdealWifiManager::IsLowLatency (void) const
{
  return true;
}

} //namespace ns3
