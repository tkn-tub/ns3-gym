/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "aarfcd-wifi-manager.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/boolean.h"
#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("Aarfcd");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(AarfcdWifiManager);

AarfcdWifiRemoteStation::AarfcdWifiRemoteStation (Ptr<AarfcdWifiManager> manager)
  : m_manager (manager)
{
  m_timerTimeout = m_manager->m_minTimerThreshold;
  m_successThreshold = m_manager->m_minSuccessThreshold;
  m_rate = GetMinRate ();

  m_success = 0;
  m_failed = 0;
  m_recovery = false;
  m_retry = 0;
  m_timer = 0;
  m_rtsOn = false;
  m_rtsWnd = m_manager->m_minRtsWnd;
  m_rtsCounter = 0;
  m_justModifyRate = true;
  m_haveASuccess = false;
}
AarfcdWifiRemoteStation::~AarfcdWifiRemoteStation ()
{}

uint32_t
AarfcdWifiRemoteStation::GetMaxRate (void)
{
  return GetNSupportedModes () - 1;
}
uint32_t
AarfcdWifiRemoteStation::GetMinRate (void)
{
  return 0;
}

void
AarfcdWifiRemoteStation::ReportRecoveryFailure (void)
{
  m_successThreshold = (int)(std::min ((uint32_t)(m_successThreshold * m_manager->m_successK),
                                       m_manager->m_maxSuccessThreshold));
  m_timerTimeout = (int)(std::max (m_manager->m_minTimerThreshold,
                                   (uint32_t)(m_successThreshold * m_manager->m_timerK)));
}

void
AarfcdWifiRemoteStation::ReportFailure (void)
{
  m_timerTimeout = m_manager->m_minTimerThreshold;
  m_successThreshold = m_manager->m_minSuccessThreshold;
}

bool 
AarfcdWifiRemoteStation::NeedRecoveryFallback (void)
{
  if (m_retry >= 1) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}
bool 
AarfcdWifiRemoteStation::NeedNormalFallback (void)
{
  int retryMod = (m_retry - 1) % 2;
  if (retryMod == 1) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}



void 
AarfcdWifiRemoteStation::DoReportRtsFailed (void)
{
  //printf ("%.9f %p RtsFail %d %d %d\n",Simulator::Now ().GetSeconds (),this,m_rate,m_timer,m_retry);
  NS_LOG_INFO ("" << this << " RtsFail rate=" << m_rate);
  if (m_manager->m_rtsFailsAsDataFails)
    {
      m_rtsCounter--;
      ReportDataFailed ();
    }
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
AarfcdWifiRemoteStation::DoReportDataFailed (void)
{
  NS_LOG_INFO ("" << this << " TxFail rate=" << m_rate);
  m_timer++;
  m_failed++;
  m_retry++;
  m_success = 0;
  //printf ("%.9f %p Fail %d %d %d\n",Simulator::Now ().GetSeconds (),this,m_rate,m_timer,m_retry);
  if (!m_rtsOn) 
    {
      TurnOnRts ();
      if (!m_justModifyRate && !m_haveASuccess) 
        {
          //printf ("%p Increase RTS Windows\n",this);
          IncreaseRtsWnd ();
        }
      else 
        {
          //printf ("%p Reset RTS Window\n",this);
          ResetRtsWnd ();
        }
      m_rtsCounter = m_rtsWnd;
      if (m_retry >= 2) 
        {
          m_timer = 0;
        }
      //printf ("%.9f %p AtcivateRTS %d %d\n",Simulator::Now ().GetSeconds (),this, m_rate, m_rtsCounter);
    }
  else if (m_recovery) 
    {
      NS_ASSERT (m_retry >= 1);
      m_justModifyRate = false;
      m_rtsCounter = m_rtsWnd;
      if (NeedRecoveryFallback ()) 
        {
          if (m_manager->m_turnOffRtsAfterRateDecrease) 
            {
              TurnOffRts ();
            }
          m_justModifyRate = true;
          ReportRecoveryFailure ();
          if (m_rate != GetMinRate ()) 
            {
              m_rate--;
            }
          NS_LOG_INFO ("" << this << " JD rate=" << m_rate << " Sthr=" << m_successTreshold);
          //printf ("%.9f %p DecreaseRateRecovery %d\n", Simulator::Now ().GetSeconds (),this, m_rate);
        }
      m_timer = 0;
    } 
  else 
    {
      NS_ASSERT (m_retry >= 1);
      m_justModifyRate = false;
      m_rtsCounter = m_rtsWnd;
      if (NeedNormalFallback ()) 
        {
          if (m_manager->m_turnOffRtsAfterRateDecrease) 
            {
              TurnOffRts ();
            }
          m_justModifyRate = true;
          ReportFailure ();
          if (m_rate != GetMinRate ()) 
            {
              m_rate--;
            }
          NS_LOG_INFO ("" << this << " JD rate=" << m_rate << " Sthr=" << m_successThreshold);
          //printf ("%.9f %p DecreaseRate %d\n", Simulator::Now ().GetSeconds (),this,m_rate);
        }
      if (m_retry >= 2) 
        {
          m_timer = 0;
        }
    }
  CheckRts ();
}
void 
AarfcdWifiRemoteStation::DoReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
AarfcdWifiRemoteStation::DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_INFO ("" << this << " RtsOk rate=" << m_rate);
  NS_LOG_DEBUG ("self="<<this<<" rts ok");
  m_rtsCounter--;
}
void 
AarfcdWifiRemoteStation::DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_INFO ("" << this << " TxOk rate=" << m_rate);
  m_timer++;
  m_success++;
  m_failed = 0;
  m_recovery = false;
  m_retry = 0;
  m_justModifyRate = false;
  m_haveASuccess = true;
  //printf ("%.9f %p Ok %d %d %d\n",Simulator::Now ().GetSeconds (),this,m_rate,m_timer,m_retry);
  //printf ("%p OK (m_success=%d, th=%d, m_rate=%d, maxRate=%d)\n",this,m_success,GetSuccessThreshold (), m_rate, GetMaxRate ());
  NS_LOG_DEBUG ("self="<<this<<" data ok success="<<m_success<<", timer="<<m_timer);
  if ((m_success == m_successThreshold ||
       m_timer >= m_timerTimeout) &&
      (m_rate < GetMaxRate ())) 
    {
      NS_LOG_DEBUG ("self="<<this<<" inc rate");
      m_rate++;
      NS_LOG_INFO ("" << this << " JI rate=" << m_rate << " Sthr=" << m_successTreshold);
      m_timer = 0;
      m_success = 0;
      m_recovery = true;
      m_justModifyRate = true;
      if (m_manager->m_turnOnRtsAfterRateIncrease) 
        {
          TurnOnRts ();
          ResetRtsWnd ();
          m_rtsCounter = m_rtsWnd;
        }
      //printf ("%.9f %p IncreaseRate %d %d\n", Simulator::Now ().GetSeconds (),this,m_rate,(m_rtsOn?1:0));
    }
  else if (m_success == m_successThreshold ||
           m_timer >= m_timerTimeout) 
    {
      NS_LOG_INFO ("" << this << " JI rate=" << m_rate << " Sthr=" << m_successThreshold);
    }
  CheckRts ();
}
void 
AarfcdWifiRemoteStation::DoReportFinalRtsFailed (void)
{}
void 
AarfcdWifiRemoteStation::DoReportFinalDataFailed (void)
{}

WifiMode
AarfcdWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  return GetSupportedMode (m_rate);
}
WifiMode
AarfcdWifiRemoteStation::DoGetRtsMode (void)
{
  // XXX: we could/should implement the Arf algorithm for
  // RTS only by picking a single rate within the BasicRateSet.
  return GetSupportedMode (0);
}

Ptr<WifiRemoteStationManager>
AarfcdWifiRemoteStation::GetManager (void) const
{
  return m_manager;
}

void
AarfcdWifiRemoteStation::CheckRts (void)
{
  if (m_rtsCounter == 0 && m_rtsOn) 
    {
      //printf ("%p Turn off RTS\n",this);
      TurnOffRts ();
    }
}

void
AarfcdWifiRemoteStation::TurnOffRts (void)
{
  //printf ("%.9f %p DeatcivateRTS %d %d\n",Simulator::Now ().GetSeconds (),this, m_rate, m_rtsCounter);
  m_rtsOn = false;
  m_haveASuccess = false;
}

void
AarfcdWifiRemoteStation::TurnOnRts (void)
{
  m_rtsOn = true;
}

void
AarfcdWifiRemoteStation::IncreaseRtsWnd (void)
{
  if (m_rtsWnd == m_manager->m_maxRtsWnd)
    {
      return;
    }

  m_rtsWnd *= 2;
  if (m_rtsWnd > m_manager->m_maxRtsWnd)
    {
      m_rtsWnd = m_manager->m_maxRtsWnd;
    }
}

void
AarfcdWifiRemoteStation::ResetRtsWnd (void)
{
  m_rtsWnd = m_manager->m_minRtsWnd;
}

bool
AarfcdWifiRemoteStation::DoNeedRts (Ptr<const Packet> packet)
{
  //printf ("%.9f %p NeedRts %d %d\n",Simulator::Now ().GetSeconds (),this,m_rate,(m_rtsOn?1:0));
  NS_LOG_INFO ("" << this << " rate=" << m_rate << " rts=" << (m_rtsOn?"RTS":"BASIC") << " rtsCounter=" << m_rtsCounter);
  return m_rtsOn;
}





TypeId 
AarfcdWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AarfcdWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<AarfcdWifiManager> ()
    .AddAttribute ("SuccessK", "Multiplication factor for the success threshold in the AARF algorithm.",
                   DoubleValue (2.0),
                   MakeDoubleAccessor (&AarfcdWifiManager::m_successK),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TimerK",
                   "Multiplication factor for the timer threshold in the AARF algorithm.",
                   DoubleValue (2.0),
                   MakeDoubleAccessor (&AarfcdWifiManager::m_timerK),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSuccessThreshold",
                   "Maximum value of the success threshold in the AARF algorithm.",
                   UintegerValue (60),
                   MakeUintegerAccessor (&AarfcdWifiManager::m_maxSuccessThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinTimerThreshold",
                   "The minimum value for the 'timer' threshold in the AARF algorithm.",
                   UintegerValue (15),
                   MakeUintegerAccessor (&AarfcdWifiManager::m_minTimerThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinSuccessThreshold",
                   "The minimum value for the success threshold in the AARF algorithm.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&AarfcdWifiManager::m_minSuccessThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinRtsWnd", 
                   "Minimum value for Rts window of Aarf-CD",
                   UintegerValue (1),
                   MakeUintegerAccessor (&AarfcdWifiManager::m_minRtsWnd),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxRtsWnd", 
                   "Maximum value for Rts window of Aarf-CD",
                   UintegerValue (40),
                   MakeUintegerAccessor (&AarfcdWifiManager::m_maxRtsWnd),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RtsFailsAsDataFails",
                   "If true the RTS failures will be treated by Aarf-CD as Data failures",
                   BooleanValue (false),
                   MakeBooleanAccessor (&AarfcdWifiManager::m_rtsFailsAsDataFails),
                   MakeBooleanChecker ())
    .AddAttribute ("TurnOffRtsAfterRateDecrease",
                   "If true the RTS mechanism will be turned off when the rate will be decreased",
                   BooleanValue (true),
                   MakeBooleanAccessor (&AarfcdWifiManager::m_turnOffRtsAfterRateDecrease),
                   MakeBooleanChecker ())
    .AddAttribute ("TurnOnRtsAfterRateIncrease",
                   "If true the RTS mechanism will be turned on when the rate will be increased",
                   BooleanValue (true),
                   MakeBooleanAccessor (&AarfcdWifiManager::m_turnOnRtsAfterRateIncrease),
                   MakeBooleanChecker ())
    ;
  return tid;
}
AarfcdWifiManager::AarfcdWifiManager ()
  : WifiRemoteStationManager ()
{}
AarfcdWifiManager::~AarfcdWifiManager ()
{}
WifiRemoteStation *
AarfcdWifiManager::CreateStation (void)
{
  return new AarfcdWifiRemoteStation (this);
}

} // namespace ns3
