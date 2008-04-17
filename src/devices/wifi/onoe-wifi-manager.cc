/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

NS_LOG_COMPONENT_DEFINE ("OnoeWifiRemoteStation");

namespace ns3 {

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
    .AddAttribute ("RaiseThreshold", "XXX",
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
{}
WifiRemoteStation *
OnoeWifiManager::CreateStation (void)
{
  return new OnoeWifiRemoteStation (this);
}

OnoeWifiRemoteStation::OnoeWifiRemoteStation (Ptr<OnoeWifiManager> stations)
  : m_stations (stations),
    m_nextModeUpdate (Simulator::Now () + stations->m_updatePeriod),
    m_shortRetry (0),
    m_longRetry (0),
    m_tx_ok (0),
    m_tx_err (0),
    m_tx_retr (0),
    m_tx_upper (0),
    m_txrate (0)
{}
OnoeWifiRemoteStation::~OnoeWifiRemoteStation ()
{}

void 
OnoeWifiRemoteStation::DoReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
OnoeWifiRemoteStation::DoReportRtsFailed (void)
{
  m_shortRetry++;
}
void 
OnoeWifiRemoteStation::DoReportDataFailed (void)
{
  m_longRetry++;
}
void 
OnoeWifiRemoteStation::DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{}
void 
OnoeWifiRemoteStation::DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  UpdateRetry ();
  m_tx_ok++;
}
void 
OnoeWifiRemoteStation::DoReportFinalRtsFailed (void)
{
  UpdateRetry ();
  m_tx_err++;
}
void 
OnoeWifiRemoteStation::DoReportFinalDataFailed (void)
{
  UpdateRetry ();
  m_tx_err++;
}
void
OnoeWifiRemoteStation::UpdateRetry (void)
{
  m_tx_retr += m_shortRetry + m_longRetry;
  m_shortRetry = 0;
  m_longRetry = 0;
}
void
OnoeWifiRemoteStation::UpdateMode (void)
{
  if (Simulator::Now () < m_nextModeUpdate)
    {
      return;
    }
  m_nextModeUpdate = Simulator::Now () + m_stations->m_updatePeriod;
  /**
   * The following 20 lines of code were copied from the Onoe
   * rate control kernel module used in the madwifi driver.
   */

  int dir = 0, enough;
  uint32_t nrate;
  enough = (m_tx_ok + m_tx_err >= 10);

  /* no packet reached -> down */
  if (m_tx_err > 0 && m_tx_ok == 0)
    dir = -1;

  /* all packets needs retry in average -> down */
  if (enough && m_tx_ok < m_tx_retr)
    dir = -1;

  /* no error and less than rate_raise% of packets need retry -> up */
  if (enough && m_tx_err == 0 &&
      m_tx_retr < (m_tx_ok * m_stations->m_addCreditThreshold) / 100)
    dir = 1;

  NS_LOG_DEBUG (this << " ok " << m_tx_ok << " err " << m_tx_err << " retr " << m_tx_retr <<
                " upper " << m_tx_upper << " dir " << dir);

  nrate = m_txrate;
  switch (dir) {
  case 0:
    if (enough && m_tx_upper > 0)
      m_tx_upper--;
    break;
  case -1:
    if (nrate > 0) {
      nrate--;
    }
    m_tx_upper = 0;
    break;
  case 1:
    /* raise rate if we hit rate_raise_threshold */
    if (++m_tx_upper < m_stations->m_raiseThreshold)
      break;
    m_tx_upper = 0;
    if (nrate + 1 < GetNSupportedModes ()) {
      nrate++;
    }
    break;
  }

  if (nrate != m_txrate) {
    NS_ASSERT (nrate < GetNSupportedModes ());
    m_txrate = nrate;
    m_tx_ok = m_tx_err = m_tx_retr = m_tx_upper = 0;
  } else if (enough)
    m_tx_ok = m_tx_err = m_tx_retr = 0;

}

Ptr<WifiRemoteStationManager>
OnoeWifiRemoteStation::GetManager (void) const
{
  return m_stations;
}
WifiMode 
OnoeWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  UpdateMode ();
  NS_ASSERT (m_txrate < GetNSupportedModes ());
  uint32_t rateIndex;
  if (m_longRetry < 4)
    {
      rateIndex = m_txrate;
    }
  else if (m_longRetry < 6)
    {
      if (m_txrate > 0)
        {
          rateIndex = m_txrate - 1;
        }
      else
        {
          rateIndex = m_txrate;
        }
    }
  else if (m_longRetry < 8)
    {
      if (m_txrate > 1)
        {
          rateIndex = m_txrate - 2;
        }
      else
        {
          rateIndex = m_txrate;
        }
    }
  else
    {
      if (m_txrate > 2)
        {
          rateIndex = m_txrate - 3;
        }
      else
        {
          rateIndex = m_txrate;
        }
    }
  return GetSupportedMode (rateIndex);
}
WifiMode 
OnoeWifiRemoteStation::DoGetRtsMode (void)
{
  UpdateMode ();
  // XXX: can we implement something smarter ?
  return GetSupportedMode (0);
}

} // namespace ns3
