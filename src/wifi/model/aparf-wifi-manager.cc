/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Universidad de la República - Uruguay
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
 * Author: Matias Richart <mrichart@fing.edu.uy>
 */

#include "aparf-wifi-manager.h"
#include "wifi-phy.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"

#define Min(a,b) ((a < b) ? a : b)

NS_LOG_COMPONENT_DEFINE ("ns3::AparfWifiManager");

namespace ns3 {

/**
 * Hold per-remote-station state for APARF Wifi manager.
 *
 * This struct extends from WifiRemoteStation struct to hold additional
 * information required by the APARF Wifi manager
 */
struct
AparfWifiRemoteStation : public WifiRemoteStation
{
  uint32_t m_nSuccess;                  //!< Number of successful transmission attempts.
  uint32_t m_nFailed;                   //!< Number of failed transmission attempts.
  uint32_t m_pCount;                    //!< Number of power changes.
  uint32_t m_successThreshold;          //!< The minimum number of successful transmissions to try a new power or rate.
  uint32_t m_failThreshold;             //!< The minimum number of failed transmissions to try a new power or rate.
  uint32_t m_rate;                      //!< Current rate.
  uint32_t m_rateCrit;                  //!< Critical rate.
  uint8_t m_power;                      //!< Current power.
  uint32_t m_nSupported;                //!< Number of supported rates by the remote station.
  bool m_initialized;                   //!< For initializing variables.
  AparfWifiManager::State m_aparfState; //!< The estimated state of the channel.
};

NS_OBJECT_ENSURE_REGISTERED (AparfWifiManager);

TypeId
AparfWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AparfWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .SetGroupName ("Wifi")
    .AddConstructor<AparfWifiManager> ()
    .AddAttribute ("SuccessThreshold 1",
                   "The minimum number of successful transmissions in \"High\" state to try a new power or rate.",
                   UintegerValue (3),
                   MakeUintegerAccessor (&AparfWifiManager::m_succesMax1),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("SuccessThreshold 2",
                   "The minimum number of successful transmissions in \"Low\" state to try a new power or rate.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&AparfWifiManager::m_succesMax2),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("FailThreshold",
                   "The minimum number of failed transmissions to try a new power or rate.",
                   UintegerValue (1),
                   MakeUintegerAccessor (&AparfWifiManager::m_failMax),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("PowerThreshold",
                   "The maximum number of power changes.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&AparfWifiManager::m_powerMax),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Power decrement step",
                   "Step size for decrement the power.",
                   UintegerValue (1),
                   MakeUintegerAccessor (&AparfWifiManager::m_powerDec),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Power increment step",
                   "Step size for increment the power.",
                   UintegerValue (1),
                   MakeUintegerAccessor (&AparfWifiManager::m_powerInc),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Rate decrement step",
                   "Step size for decrement the rate.",
                   UintegerValue (1),
                   MakeUintegerAccessor (&AparfWifiManager::m_rateDec),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Rate increment step",
                   "Step size for increment the rate.",
                   UintegerValue (1),
                   MakeUintegerAccessor (&AparfWifiManager::m_rateInc),
                   MakeUintegerChecker<uint32_t> ())
    .AddTraceSource ("PowerChange",
                     "The transmission power has change",
                     MakeTraceSourceAccessor (&AparfWifiManager::m_powerChange),
                     "ns3::WifiRemoteStationManager::PowerChangeTracedCallback")
    .AddTraceSource ("RateChange",
                     "The transmission rate has change",
                     MakeTraceSourceAccessor (&AparfWifiManager::m_rateChange),
                     "ns3::WifiRemoteStationManager::RateChangeTracedCallback")
  ;
  return tid;
}

AparfWifiManager::AparfWifiManager ()
{
  NS_LOG_FUNCTION (this);
}

AparfWifiManager::~AparfWifiManager ()
{
  NS_LOG_FUNCTION (this);
}

void
AparfWifiManager::SetupPhy (Ptr<WifiPhy> phy)
{
  m_minPower = phy->GetTxPowerStart ();
  m_maxPower = phy->GetTxPowerEnd ();
  WifiRemoteStationManager::SetupPhy (phy);
}

WifiRemoteStation *
AparfWifiManager::DoCreateStation (void) const
{
  NS_LOG_FUNCTION (this);
  AparfWifiRemoteStation *station = new AparfWifiRemoteStation ();

  station->m_successThreshold = m_succesMax1;
  station->m_failThreshold = m_failMax;
  station->m_nSuccess = 0;
  station->m_nFailed = 0;
  station->m_pCount = 0;
  station->m_aparfState = AparfWifiManager::High;
  station->m_initialized = false;

  NS_LOG_DEBUG ("create station=" << station << ", rate=" << station->m_rate
                                  << ", power=" << (int)station->m_power);

  return station;
}

void
AparfWifiManager::CheckInit (AparfWifiRemoteStation *station)
{
  if (!station->m_initialized)
    {
      station->m_nSupported = GetNSupported (station);
      station->m_rate = station->m_nSupported - 1;
      station->m_power = m_maxPower;
      station->m_rateCrit = 0;
      m_powerChange (station->m_power, station->m_state->m_address);
      m_rateChange (station->m_rate, station->m_state->m_address);
      station->m_initialized = true;
    }
}

void AparfWifiManager::DoReportRtsFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}

void AparfWifiManager::DoReportDataFailed (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  AparfWifiRemoteStation *station = (AparfWifiRemoteStation *) st;
  CheckInit (station);
  station->m_nFailed++;
  station->m_nSuccess = 0;
  NS_LOG_DEBUG ("station=" << station << ", rate=" << station->m_rate
                           << ", power=" << (int)station->m_power);

  if (station->m_aparfState == AparfWifiManager::Low)
    {
      station->m_aparfState = AparfWifiManager::High;
      station->m_successThreshold = m_succesMax1;
    }
  else if (station->m_aparfState == AparfWifiManager::Spread)
    {
      station->m_aparfState = AparfWifiManager::Low;
      station->m_successThreshold = m_succesMax2;
    }

  if (station->m_nFailed == station->m_failThreshold)
    {
      station->m_nFailed = 0;
      station->m_nSuccess = 0;
      station->m_pCount = 0;
      if (station->m_power == m_maxPower)
        {
          station->m_rateCrit = station->m_rate;
          if (station->m_rate != 0)
            {
              NS_LOG_DEBUG ("station=" << station << " dec rate");
              station->m_rate -= m_rateDec;
              m_rateChange (station->m_rate, station->m_state->m_address);
            }
        }
      else
        {
          NS_LOG_DEBUG ("station=" << station << " inc power");
          station->m_power += m_powerInc;
          m_powerChange (station->m_power, station->m_state->m_address);
        }
    }
}

void
AparfWifiManager::DoReportRxOk (WifiRemoteStation *station, double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << station << rxSnr << txMode);
}

void
AparfWifiManager::DoReportRtsOk (WifiRemoteStation *station, double ctsSnr,
                                 WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << station << ctsSnr << ctsMode << rtsSnr);
  NS_LOG_DEBUG ("station=" << station << " rts ok");
}

void
AparfWifiManager::DoReportDataOk (WifiRemoteStation *st, double ackSnr,
                                  WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << st << ackSnr << ackMode << dataSnr);
  AparfWifiRemoteStation *station = (AparfWifiRemoteStation *) st;
  CheckInit (station);
  station->m_nSuccess++;
  station->m_nFailed = 0;
  NS_LOG_DEBUG ("station=" << station << " data ok success=" << station->m_nSuccess << ", rate=" << station->m_rate << ", power=" << (int)station->m_power);

  if ((station->m_aparfState == AparfWifiManager::High) && (station->m_nSuccess >= station->m_successThreshold))
    {
      station->m_aparfState = AparfWifiManager::Spread;
    }
  else if ((station->m_aparfState == AparfWifiManager::Low) && (station->m_nSuccess >= station->m_successThreshold))
    {
      station->m_aparfState = AparfWifiManager::Spread;
    }
  else if (station->m_aparfState == AparfWifiManager::Spread)
    {
      station->m_aparfState = AparfWifiManager::High;
      station->m_successThreshold = m_succesMax1;
    }

  if (station->m_nSuccess == station->m_successThreshold)
    {
      station->m_nSuccess = 0;
      station->m_nFailed = 0;
      if (station->m_rate == (station->m_state->m_operationalRateSet.size () - 1))
        {
          if (station->m_power != m_minPower)
            {
              NS_LOG_DEBUG ("station=" << station << " dec power");
              station->m_power -= m_powerDec;
              m_powerChange (station->m_power, station->m_state->m_address);
            }
        }
      else
        {
          if (station->m_rateCrit == 0)
            {
              if (station->m_rate != (station->m_state->m_operationalRateSet.size () - 1))
                {
                  NS_LOG_DEBUG ("station=" << station << " inc rate");
                  station->m_rate += m_rateInc;
                  m_rateChange (station->m_rate, station->m_state->m_address);
                }
            }
          else
            {
              if (station->m_pCount == m_powerMax)
                {
                  station->m_power = m_maxPower;
                  m_powerChange (station->m_power, station->m_state->m_address);
                  station->m_rate = station->m_rateCrit;
                  m_rateChange (station->m_rate, station->m_state->m_address);
                  station->m_pCount = 0;
                  station->m_rateCrit = 0;
                }
              else
                {
                  if (station->m_power != m_minPower)
                    {
                      station->m_power -= m_powerDec;
                      m_powerChange (station->m_power, station->m_state->m_address);
                      station->m_pCount++;
                    }
                }
            }
        }
    }
}

void
AparfWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}

void
AparfWifiManager::DoReportFinalDataFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}

WifiTxVector
AparfWifiManager::DoGetDataTxVector (WifiRemoteStation *st, uint32_t size)
{
  NS_LOG_FUNCTION (this << st << size);
  AparfWifiRemoteStation *station = (AparfWifiRemoteStation *) st;
  uint32_t channelWidth = GetChannelWidth (station);
  if (channelWidth > 20 && channelWidth != 22)
    {
      //avoid to use legacy rate adaptation algorithms for IEEE 802.11n/ac
      channelWidth = 20;
    }
  CheckInit (station);
  return WifiTxVector (GetSupported (station, station->m_rate), station->m_power, GetLongRetryCount (station), false, 1, 0, channelWidth, GetAggregation (station), false);
}

WifiTxVector
AparfWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  /// \todo we could/should implement the Arf algorithm for
  /// RTS only by picking a single rate within the BasicRateSet.
  AparfWifiRemoteStation *station = (AparfWifiRemoteStation *) st;
  uint32_t channelWidth = GetChannelWidth (station);
  if (channelWidth > 20 && channelWidth != 22)
    {
      //avoid to use legacy rate adaptation algorithms for IEEE 802.11n/ac
      channelWidth = 20;
    }
  return WifiTxVector (GetSupported (station, 0), GetDefaultTxPowerLevel (), GetShortRetryCount (station), false, 1, 0, channelWidth, GetAggregation (station), false);
}

bool
AparfWifiManager::IsLowLatency (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

} //namespace ns3
