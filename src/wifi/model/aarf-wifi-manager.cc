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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "aarf-wifi-manager.h"

#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"

#define Min(a,b) ((a < b) ? a : b)
#define Max(a,b) ((a > b) ? a : b)

NS_LOG_COMPONENT_DEFINE ("AarfWifiManager");

namespace ns3 {

struct AarfWifiRemoteStation : public WifiRemoteStation
{
  uint32_t m_timer;
  uint32_t m_success;
  uint32_t m_failed;
  bool m_recovery;
  uint32_t m_retry;

  uint32_t m_timerTimeout;
  uint32_t m_successThreshold;

  uint32_t m_rate;
};


NS_OBJECT_ENSURE_REGISTERED (AarfWifiManager);

TypeId
AarfWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AarfWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<AarfWifiManager> ()
    .AddAttribute ("SuccessK", "Multiplication factor for the success threshold in the AARF algorithm.",
                   DoubleValue (2.0),
                   MakeDoubleAccessor (&AarfWifiManager::m_successK),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TimerK",
                   "Multiplication factor for the timer threshold in the AARF algorithm.",
                   DoubleValue (2.0),
                   MakeDoubleAccessor (&AarfWifiManager::m_timerK),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSuccessThreshold",
                   "Maximum value of the success threshold in the AARF algorithm.",
                   UintegerValue (60),
                   MakeUintegerAccessor (&AarfWifiManager::m_maxSuccessThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinTimerThreshold",
                   "The minimum value for the 'timer' threshold in the AARF algorithm.",
                   UintegerValue (15),
                   MakeUintegerAccessor (&AarfWifiManager::m_minTimerThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinSuccessThreshold",
                   "The minimum value for the success threshold in the AARF algorithm.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&AarfWifiManager::m_minSuccessThreshold),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

AarfWifiManager::AarfWifiManager ()
{
  NS_LOG_FUNCTION (this);
}
AarfWifiManager::~AarfWifiManager ()
{
  NS_LOG_FUNCTION (this);
}

WifiRemoteStation *
AarfWifiManager::DoCreateStation (void) const
{
  NS_LOG_FUNCTION (this);
  AarfWifiRemoteStation *station = new AarfWifiRemoteStation ();

  station->m_successThreshold = m_minSuccessThreshold;
  station->m_timerTimeout = m_minTimerThreshold;
  station->m_rate = 0;
  station->m_success = 0;
  station->m_failed = 0;
  station->m_recovery = false;
  station->m_retry = 0;
  station->m_timer = 0;

  return station;
}

void
AarfWifiManager::DoReportRtsFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}
/**
 * It is important to realize that "recovery" mode starts after failure of
 * the first transmission after a rate increase and ends at the first successful
 * transmission. Specifically, recovery mode transcends retransmissions boundaries.
 * Fundamentally, ARF handles each data transmission independently, whether it
 * is the initial transmission of a packet or the retransmission of a packet.
 * The fundamental reason for this is that there is a backoff between each data
 * transmission, be it an initial transmission or a retransmission.
 */
void
AarfWifiManager::DoReportDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  AarfWifiRemoteStation *station = (AarfWifiRemoteStation *)st;
  station->m_timer++;
  station->m_failed++;
  station->m_retry++;
  station->m_success = 0;

  if (station->m_recovery)
    {
      NS_ASSERT (station->m_retry >= 1);
      if (station->m_retry == 1)
        {
          // need recovery fallback
          station->m_successThreshold = (int)(Min (station->m_successThreshold * m_successK,
                                                   m_maxSuccessThreshold));
          station->m_timerTimeout = (int)(Max (station->m_timerTimeout * m_timerK,
                                               m_minSuccessThreshold));
          if (station->m_rate != 0)
            {
              station->m_rate--;
            }
        }
      station->m_timer = 0;
    }
  else
    {
      NS_ASSERT (station->m_retry >= 1);
      if (((station->m_retry - 1) % 2) == 1)
        {
          // need normal fallback
          station->m_timerTimeout = m_minTimerThreshold;
          station->m_successThreshold = m_minSuccessThreshold;
          if (station->m_rate != 0)
            {
              station->m_rate--;
            }
        }
      if (station->m_retry >= 2)
        {
          station->m_timer = 0;
        }
    }
}
void
AarfWifiManager::DoReportRxOk (WifiRemoteStation *station,
                               double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << station << rxSnr << txMode);
}
void
AarfWifiManager::DoReportRtsOk (WifiRemoteStation *station,
                                double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << station << ctsSnr << ctsMode << rtsSnr);
  NS_LOG_DEBUG ("station=" << station << " rts ok");
}
void
AarfWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                 double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << st << ackSnr << ackMode << dataSnr);
  AarfWifiRemoteStation *station = (AarfWifiRemoteStation *) st;
  station->m_timer++;
  station->m_success++;
  station->m_failed = 0;
  station->m_recovery = false;
  station->m_retry = 0;
  NS_LOG_DEBUG ("station=" << station << " data ok success=" << station->m_success << ", timer=" << station->m_timer);
  if ((station->m_success == station->m_successThreshold
       || station->m_timer == station->m_timerTimeout)
      && (station->m_rate < (GetNSupported (station) - 1)))
    {
      NS_LOG_DEBUG ("station=" << station << " inc rate");
      station->m_rate++;
      station->m_timer = 0;
      station->m_success = 0;
      station->m_recovery = true;
    }
}
void
AarfWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}
void
AarfWifiManager::DoReportFinalDataFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}

WifiMode
AarfWifiManager::DoGetDataMode (WifiRemoteStation *st, uint32_t size)
{
  NS_LOG_FUNCTION (this << st << size);
  AarfWifiRemoteStation *station = (AarfWifiRemoteStation *) st;
  return GetSupported (station, station->m_rate);
}
WifiMode
AarfWifiManager::DoGetRtsMode (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  /// \todo we could/should implement the Aarf algorithm for
  /// RTS only by picking a single rate within the BasicRateSet.
  AarfWifiRemoteStation *station = (AarfWifiRemoteStation *) st;
  return GetSupported (station, 0);
}

bool
AarfWifiManager::IsLowLatency (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

} // namespace ns3
