/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2003,2007 INRIA
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

#include "onoe-wifi-manager.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"

#define Min(a,b) ((a < b) ? a : b)

NS_LOG_COMPONENT_DEFINE ("OnoeWifiRemoteStation");

namespace ns3 {

/**
 * \brief hold per-remote-station state for ONOE Wifi manager.
 *
 * This struct extends from WifiRemoteStation struct to hold additional
 * information required by the ONOE Wifi manager
 */
struct OnoeWifiRemoteStation : public WifiRemoteStation
{
  Time m_nextModeUpdate;
  uint32_t m_shortRetry;
  uint32_t m_longRetry;
  uint32_t m_tx_ok;
  uint32_t m_tx_err;
  uint32_t m_tx_retr;
  uint32_t m_tx_upper;
  uint32_t m_txrate;
};


NS_OBJECT_ENSURE_REGISTERED (OnoeWifiManager);

TypeId
OnoeWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OnoeWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<OnoeWifiManager> ()
    .AddAttribute ("UpdatePeriod",
                   "The interval between decisions about rate control changes",
                   TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&OnoeWifiManager::m_updatePeriod),
                   MakeTimeChecker ())
    .AddAttribute ("RaiseThreshold", "Attempt to raise the rate if we hit that threshold",
                   UintegerValue (10),
                   MakeUintegerAccessor (&OnoeWifiManager::m_raiseThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("AddCreditThreshold", "Add credit threshold",
                   UintegerValue (10),
                   MakeUintegerAccessor (&OnoeWifiManager::m_addCreditThreshold),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

OnoeWifiManager::OnoeWifiManager ()
{
}
WifiRemoteStation *
OnoeWifiManager::DoCreateStation (void) const
{
  OnoeWifiRemoteStation *station = new OnoeWifiRemoteStation ();
  station->m_nextModeUpdate = Simulator::Now () + m_updatePeriod;
  station->m_shortRetry = 0;
  station->m_longRetry = 0;
  station->m_tx_ok = 0;
  station->m_tx_err = 0;
  station->m_tx_retr = 0;
  station->m_tx_upper = 0;
  station->m_txrate = 0;
  return station;
}
void
OnoeWifiManager::DoReportRxOk (WifiRemoteStation *station,
                               double rxSnr, WifiMode txMode)
{
}
void
OnoeWifiManager::DoReportRtsFailed (WifiRemoteStation *st)
{
  OnoeWifiRemoteStation *station = (OnoeWifiRemoteStation *)st;
  station->m_shortRetry++;
}
void
OnoeWifiManager::DoReportDataFailed (WifiRemoteStation *st)
{
  OnoeWifiRemoteStation *station = (OnoeWifiRemoteStation *)st;
  station->m_longRetry++;
}
void
OnoeWifiManager::DoReportRtsOk (WifiRemoteStation *station,
                                double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
}
void
OnoeWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                 double ackSnr, WifiMode ackMode, double dataSnr)
{
  OnoeWifiRemoteStation *station = (OnoeWifiRemoteStation *)st;
  UpdateRetry (station);
  station->m_tx_ok++;
}
void
OnoeWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *st)
{
  OnoeWifiRemoteStation *station = (OnoeWifiRemoteStation *)st;
  UpdateRetry (station);
  station->m_tx_err++;
}
void
OnoeWifiManager::DoReportFinalDataFailed (WifiRemoteStation *st)
{
  OnoeWifiRemoteStation *station = (OnoeWifiRemoteStation *)st;
  UpdateRetry (station);
  station->m_tx_err++;
}
void
OnoeWifiManager::UpdateRetry (OnoeWifiRemoteStation *station)
{
  station->m_tx_retr += station->m_shortRetry + station->m_longRetry;
  station->m_shortRetry = 0;
  station->m_longRetry = 0;
}
void
OnoeWifiManager::UpdateMode (OnoeWifiRemoteStation *station)
{
  if (Simulator::Now () < station->m_nextModeUpdate)
    {
      return;
    }
  station->m_nextModeUpdate = Simulator::Now () + m_updatePeriod;
  /**
   * The following 20 lines of code were copied from the Onoe
   * rate control kernel module used in the madwifi driver.
   */

  int dir = 0, enough;
  uint32_t nrate;
  enough = (station->m_tx_ok + station->m_tx_err >= 10);

  /* no packet reached -> down */
  if (station->m_tx_err > 0 && station->m_tx_ok == 0)
    {
      dir = -1;
    }

  /* all packets needs retry in average -> down */
  if (enough && station->m_tx_ok < station->m_tx_retr)
    {
      dir = -1;
    }

  /* no error and less than rate_raise% of packets need retry -> up */
  if (enough && station->m_tx_err == 0
      && station->m_tx_retr < (station->m_tx_ok * m_addCreditThreshold) / 100)
    {
      dir = 1;
    }

  NS_LOG_DEBUG (this << " ok " << station->m_tx_ok << " err " << station->m_tx_err << " retr " << station->m_tx_retr <<
                " upper " << station->m_tx_upper << " dir " << dir);

  nrate = station->m_txrate;
  switch (dir)
    {
    case 0:
      if (enough && station->m_tx_upper > 0)
        {
          station->m_tx_upper--;
        }
      break;
    case -1:
      if (nrate > 0)
        {
          nrate--;
        }
      station->m_tx_upper = 0;
      break;
    case 1:
      /* raise rate if we hit rate_raise_threshold */
      if (++station->m_tx_upper < m_raiseThreshold)
        {
          break;
        }
      station->m_tx_upper = 0;
      if (nrate + 1 < GetNSupported (station))
        {
          nrate++;
        }
      break;
    }

  if (nrate != station->m_txrate)
    {
      NS_ASSERT (nrate < GetNSupported (station));
      station->m_txrate = nrate;
      station->m_tx_ok = station->m_tx_err = station->m_tx_retr = station->m_tx_upper = 0;
    }
  else if (enough)
    {
      station->m_tx_ok = station->m_tx_err = station->m_tx_retr = 0;
    }

}

WifiTxVector
OnoeWifiManager::DoGetDataTxVector (WifiRemoteStation *st,
                                uint32_t size)
{
  OnoeWifiRemoteStation *station = (OnoeWifiRemoteStation *)st;
  UpdateMode (station);
  NS_ASSERT (station->m_txrate < GetNSupported (station));
  uint32_t rateIndex;
  if (station->m_longRetry < 4)
    {
      rateIndex = station->m_txrate;
    }
  else if (station->m_longRetry < 6)
    {
      if (station->m_txrate > 0)
        {
          rateIndex = station->m_txrate - 1;
        }
      else
        {
          rateIndex = station->m_txrate;
        }
    }
  else if (station->m_longRetry < 8)
    {
      if (station->m_txrate > 1)
        {
          rateIndex = station->m_txrate - 2;
        }
      else
        {
          rateIndex = station->m_txrate;
        }
    }
  else
    {
      if (station->m_txrate > 2)
        {
          rateIndex = station->m_txrate - 3;
        }
      else
        {
          rateIndex = station->m_txrate;
        }
    }
  return WifiTxVector (GetSupported (station, rateIndex), GetDefaultTxPowerLevel (), GetLongRetryCount (station), GetShortGuardInterval (station), Min (GetNumberOfReceiveAntennas (station),GetNumberOfTransmitAntennas()), GetNumberOfTransmitAntennas (station), GetStbc (station));
}
WifiTxVector
OnoeWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  OnoeWifiRemoteStation *station = (OnoeWifiRemoteStation *)st;
  UpdateMode (station);
  /// \todo can we implement something smarter ?
  return WifiTxVector (GetSupported (station, 0), GetDefaultTxPowerLevel (), GetShortRetryCount (station), GetShortGuardInterval (station), Min (GetNumberOfReceiveAntennas (station),GetNumberOfTransmitAntennas()), GetNumberOfTransmitAntennas (station), GetStbc (station));
}

bool
OnoeWifiManager::IsLowLatency (void) const
{
  return false;
}

} // namespace ns3
