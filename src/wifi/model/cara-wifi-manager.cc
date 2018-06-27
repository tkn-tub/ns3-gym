/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2004,2005,2006 INRIA
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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */

#include "ns3/log.h"
#include "cara-wifi-manager.h"
#include "wifi-tx-vector.h"

#define Min(a,b) ((a < b) ? a : b)

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CaraWifiManager");

/**
 * \brief hold per-remote-station state for CARA Wifi manager.
 *
 * This struct extends from WifiRemoteStation struct to hold additional
 * information required by the CARA Wifi manager
 */
struct CaraWifiRemoteStation : public WifiRemoteStation
{
  uint32_t m_timer; ///< timer count
  uint32_t m_success; ///< success count
  uint32_t m_failed; ///< failed count
  uint8_t m_rate; ///< rate
};

NS_OBJECT_ENSURE_REGISTERED (CaraWifiManager);

TypeId
CaraWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CaraWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .SetGroupName ("Wifi")
    .AddConstructor<CaraWifiManager> ()
    .AddAttribute ("ProbeThreshold",
                   "The number of consecutive transmissions failure to activate the RTS probe.",
                   UintegerValue (1),
                   MakeUintegerAccessor (&CaraWifiManager::m_probeThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("FailureThreshold",
                   "The number of consecutive transmissions failure to decrease the rate.",
                   UintegerValue (2),
                   MakeUintegerAccessor (&CaraWifiManager::m_failureThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("SuccessThreshold",
                   "The minimum number of successful transmissions to try a new rate.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&CaraWifiManager::m_successThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Timeout",
                   "The 'timer' in the CARA algorithm",
                   UintegerValue (15),
                   MakeUintegerAccessor (&CaraWifiManager::m_timerTimeout),
                   MakeUintegerChecker<uint32_t> ())
    .AddTraceSource ("Rate",
                     "Traced value for rate changes (b/s)",
                     MakeTraceSourceAccessor (&CaraWifiManager::m_currentRate),
                     "ns3::TracedValueCallback::Uint64")
  ;
  return tid;
}

CaraWifiManager::CaraWifiManager ()
  : WifiRemoteStationManager (),
    m_currentRate (0)
{
  NS_LOG_FUNCTION (this);
}

CaraWifiManager::~CaraWifiManager ()
{
  NS_LOG_FUNCTION (this);
}

WifiRemoteStation *
CaraWifiManager::DoCreateStation (void) const
{
  NS_LOG_FUNCTION (this);
  CaraWifiRemoteStation *station = new CaraWifiRemoteStation ();
  station->m_rate = 0;
  station->m_success = 0;
  station->m_failed = 0;
  station->m_timer = 0;
  return station;
}

void
CaraWifiManager::DoReportRtsFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
}

void
CaraWifiManager::DoReportDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  CaraWifiRemoteStation *station = (CaraWifiRemoteStation *) st;
  station->m_timer++;
  station->m_failed++;
  station->m_success = 0;
  if (station->m_failed >= m_failureThreshold)
    {
      NS_LOG_DEBUG ("self=" << station << " dec rate");
      if (station->m_rate != 0)
        {
          station->m_rate--;
        }
      station->m_failed = 0;
      station->m_timer = 0;
    }
}

void
CaraWifiManager::DoReportRxOk (WifiRemoteStation *st,
                               double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << st << rxSnr << txMode);
}

void
CaraWifiManager::DoReportRtsOk (WifiRemoteStation *st,
                                double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << st << ctsSnr << ctsMode << rtsSnr);
}

void
CaraWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                 double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << st << ackSnr << ackMode << dataSnr);
  CaraWifiRemoteStation *station = (CaraWifiRemoteStation *) st;
  station->m_timer++;
  station->m_success++;
  station->m_failed = 0;
  NS_LOG_DEBUG ("self=" << station << " data ok success=" << station->m_success << ", timer=" << station->m_timer);
  if ((station->m_success == m_successThreshold
       || station->m_timer >= m_timerTimeout))
    {
      if (station->m_rate < GetNSupported (station) - 1)
        {
          station->m_rate++;
        }
      NS_LOG_DEBUG ("self=" << station << " inc rate=" << +station->m_rate);
      station->m_timer = 0;
      station->m_success = 0;
    }
}

void
CaraWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
}

void
CaraWifiManager::DoReportFinalDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
}

WifiTxVector
CaraWifiManager::DoGetDataTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  CaraWifiRemoteStation *station = (CaraWifiRemoteStation *) st;
  uint16_t channelWidth = GetChannelWidth (station);
  if (channelWidth > 20 && channelWidth != 22)
    {
      //avoid to use legacy rate adaptation algorithms for IEEE 802.11n/ac
      channelWidth = 20;
    }
  WifiMode mode = GetSupported (station, station->m_rate);
  if (m_currentRate != mode.GetDataRate (channelWidth))
    {
      NS_LOG_DEBUG ("New datarate: " << mode.GetDataRate (channelWidth));
      m_currentRate = mode.GetDataRate (channelWidth);
    }
  return WifiTxVector (mode, GetDefaultTxPowerLevel (), GetPreambleForTransmission (mode, GetAddress (station)), 800, 1, 1, 0, channelWidth, GetAggregation (station), false);
}

WifiTxVector
CaraWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  CaraWifiRemoteStation *station = (CaraWifiRemoteStation *) st;
  /// \todo we could/should implement the Arf algorithm for
  /// RTS only by picking a single rate within the BasicRateSet.
  uint16_t channelWidth = GetChannelWidth (station);
  if (channelWidth > 20 && channelWidth != 22)
    {
      //avoid to use legacy rate adaptation algorithms for IEEE 802.11n/ac
      channelWidth = 20;
    }
  WifiTxVector rtsTxVector;
  WifiMode mode;
  if (GetUseNonErpProtection () == false)
    {
      mode = GetSupported (station, 0);
    }
  else
    {
      mode = GetNonErpSupported (station, 0);
    }
  rtsTxVector = WifiTxVector (mode, GetDefaultTxPowerLevel (), GetPreambleForTransmission (mode, GetAddress (station)), 800, 1, 1, 0, channelWidth, GetAggregation (station), false);
  return rtsTxVector;
}

bool
CaraWifiManager::DoNeedRts (WifiRemoteStation *st,
                            Ptr<const Packet> packet, bool normally)
{
  NS_LOG_FUNCTION (this << st << normally);
  CaraWifiRemoteStation *station = (CaraWifiRemoteStation *) st;
  return normally || station->m_failed >= m_probeThreshold;
}

bool
CaraWifiManager::IsLowLatency (void) const
{
  return true;
}

void
CaraWifiManager::SetHtSupported (bool enable)
{
  //HT is not supported by this algorithm.
  if (enable)
    {
      NS_FATAL_ERROR ("WifiRemoteStationManager selected does not support HT rates");
    }
}

void
CaraWifiManager::SetVhtSupported (bool enable)
{
  //VHT is not supported by this algorithm.
  if (enable)
    {
      NS_FATAL_ERROR ("WifiRemoteStationManager selected does not support VHT rates");
    }
}

void
CaraWifiManager::SetHeSupported (bool enable)
{
  //HE is not supported by this algorithm.
  if (enable)
    {
      NS_FATAL_ERROR ("WifiRemoteStationManager selected does not support HE rates");
    }
}

} //namespace ns3
