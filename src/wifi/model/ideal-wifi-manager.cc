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
#include "ns3/assert.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include <cmath>

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

uint32_t
IdealWifiManager::GetChannelWidthForMode (WifiMode mode) const
{
  NS_ASSERT (mode.GetModulationClass () != WIFI_MOD_CLASS_HT);
  NS_ASSERT (mode.GetModulationClass () != WIFI_MOD_CLASS_VHT);
  if (mode.GetModulationClass () == WIFI_MOD_CLASS_DSSS || 
      mode.GetModulationClass () == WIFI_MOD_CLASS_HR_DSSS)
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
  txVector.SetShortGuardInterval (GetPhy ()->GetGuardInterval ());
  for (uint32_t i = 0; i < nModes; i++)
    {
      mode = GetPhy ()->GetMode (i);
      txVector.SetChannelWidth (GetChannelWidthForMode (mode)); 
      txVector.SetNss (nss);
      txVector.SetMode (mode);
      NS_LOG_DEBUG ("Initialize, adding mode = " << mode.GetUniqueName () <<
                    " channel width " << txVector.GetChannelWidth () << 
                    " nss " << (uint16_t) nss << 
                    " short GI " << GetPhy ()->GetGuardInterval ());
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
            }
          else
            {
              nss = GetPhy ()->GetSupportedTxSpatialStreams ();
            }
          NS_LOG_DEBUG ("Initialize, adding mode = " << mode.GetUniqueName () <<
                        " channel width " << GetPhy ()->GetChannelWidth () <<
                        " nss " << (uint16_t) nss << 
                        " short GI " << GetPhy ()->GetGuardInterval ());
          NS_LOG_DEBUG ("In SetupPhy, adding mode = " << mode.GetUniqueName ());
          txVector.SetNss (nss);
          txVector.SetMode (mode);
          AddSnrThreshold (txVector, GetPhy ()->CalculateSnr (txVector, m_ber));
        }
    }
}

double
IdealWifiManager::GetSnrThreshold (WifiTxVector txVector) const
{
  NS_LOG_FUNCTION (this << txVector.GetMode().GetUniqueName ());
  for (Thresholds::const_iterator i = m_thresholds.begin (); i != m_thresholds.end (); i++)
    {
      NS_LOG_DEBUG ("Checking " << i->second.GetMode ().GetUniqueName () <<
                    " nss " << (uint16_t) i->second.GetNss () <<
                    " width " << i->second.GetChannelWidth ());
      NS_LOG_DEBUG ("against TxVector " << txVector.GetMode ().GetUniqueName () <<
                    " nss " << (uint16_t) txVector.GetNss () <<
                    " width " << txVector.GetChannelWidth ());
      if (txVector.GetMode () == i->second.GetMode () &&
          txVector.GetNss () == i->second.GetNss () &&
          txVector.IsShortGuardInterval () == i->second.IsShortGuardInterval () &&
          txVector.GetChannelWidth () == i->second.GetChannelWidth ())
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
IdealWifiManager::DoReportAmpduTxStatus (WifiRemoteStation *st, uint32_t nSuccessfulMpdus, uint32_t nFailedMpdus, double rxSnr, double dataSnr)
{
  NS_LOG_FUNCTION (this << st << nSuccessfulMpdus << nFailedMpdus << rxSnr << dataSnr);
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
  uint8_t nss = 1;
  txVector.SetChannelWidth (GetPhy ()->GetChannelWidth ());
  txVector.SetShortGuardInterval (GetPhy ()->GetGuardInterval ());
  if (station->m_lastSnrObserved == station->m_lastSnrCached)
    {
      // SNR has not changed, so skip the search and use the last
      // mode selected
      maxMode = station->m_lastMode;      
      nss = station->m_nss;
      NS_LOG_DEBUG ("Using cached mode = " << maxMode.GetUniqueName () <<
                    " last snr observed " << station->m_lastSnrObserved <<
                    " cached " << station->m_lastSnrCached);
    }
  else
    {
      if (HasVhtSupported () == true || HasHtSupported () == true)
        {
          for (uint32_t i = 0; i < GetNMcsSupported (station); i++)
            {
              mode = GetMcsSupported (station, i);
              txVector.SetMode (mode);
              if (mode.GetModulationClass () == WIFI_MOD_CLASS_HT)
                {
                  //derive NSS from the Mcs index
                  nss = (mode.GetMcsValue () / 8) + 1;
                }
              else
                {
                  nss = GetPhy ()->GetSupportedTxSpatialStreams ();
                }
              txVector.SetNss (nss);
              if (WifiPhy::IsValidTxVector (txVector) == false)
                 {
                   NS_LOG_DEBUG ("Skipping mode " << mode.GetUniqueName () <<
                                 " nss " << nss << " width " << 
                                 txVector.GetChannelWidth());
                   continue;       
                 }
              double threshold = GetSnrThreshold (txVector);
              // If the node and peer are both VHT capable, only search VHT modes 
              if (mode.GetModulationClass () == WIFI_MOD_CLASS_HT && HasVhtSupported () && GetVhtSupported (st))
                { 
                  continue;
                }
              // If the node and peer are not both VHT capable, only search HT modes 
              if (mode.GetModulationClass () == WIFI_MOD_CLASS_VHT && (!HasVhtSupported () || !GetVhtSupported (st)))
                { 
                  continue;
                }
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
                }
            }
        }
      else 
        {
          // Non-HT selection
          nss = 1;
          for (uint32_t i = 0; i < GetNSupported (station); i++)
            {
              mode = GetSupported (station, i);
              txVector.SetMode (mode);
              txVector.SetNss (nss);
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
      station->m_nss = nss;
    }
  uint32_t channelWidth = GetChannelWidth (station);
  NS_LOG_DEBUG ("Found maxMode: " << maxMode << " channelWidth: " << channelWidth);
  if (m_currentRate != maxMode.GetDataRate (channelWidth, GetPhy ()->GetGuardInterval (), nss))
    {
      NS_LOG_DEBUG ("New datarate: " << maxMode.GetDataRate (channelWidth, GetPhy ()->GetGuardInterval (), nss));
      m_currentRate = maxMode.GetDataRate (channelWidth, GetPhy ()->GetGuardInterval (), nss);
    }
  return WifiTxVector (maxMode, GetDefaultTxPowerLevel (), GetLongRetryCount (station), false, nss, 0, channelWidth, GetAggregation (station), false);
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
  txVector.SetShortGuardInterval (GetPhy ()->GetGuardInterval ());
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
  return WifiTxVector (maxMode, GetDefaultTxPowerLevel (), GetShortRetryCount (station), false, nss, 0, GetChannelWidthForMode (maxMode), GetAggregation (station), false);
}

bool
IdealWifiManager::IsLowLatency (void) const
{
  return true;
}

} //namespace ns3
