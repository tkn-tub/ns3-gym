/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "interference-helper.h"
#include "wifi-phy.h"
#include "error-rate-model.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("InterferenceHelper");

namespace ns3 {

/****************************************************************
 *       Phy event class
 ****************************************************************/

InterferenceHelper::Event::Event (uint32_t size, WifiMode payloadMode, 
					       enum WifiPreamble preamble,
					       Time duration, double rxPower)
  : m_size (size),
    m_payloadMode (payloadMode),
    m_preamble (preamble),
    m_startTime (Simulator::Now ()),
    m_endTime (m_startTime + duration),
    m_rxPowerW (rxPower)
{}
InterferenceHelper::Event::~Event ()
{}
  
Time 
InterferenceHelper::Event::GetDuration (void) const 
{
  return m_endTime - m_startTime;
}
Time 
InterferenceHelper::Event::GetStartTime (void) const
{
  return m_startTime;
}
Time 
InterferenceHelper::Event::GetEndTime (void) const
{
  return m_endTime;
}
bool 
InterferenceHelper::Event::Overlaps (Time time) const
{
  if (m_startTime <= time &&
      m_endTime >= time) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}
double 
InterferenceHelper::Event::GetRxPowerW (void) const
{
  return m_rxPowerW;
}
uint32_t 
InterferenceHelper::Event::GetSize (void) const
{
  return m_size;
}
WifiMode 
InterferenceHelper::Event::GetPayloadMode (void) const
{
  return m_payloadMode;
}
enum WifiPreamble 
InterferenceHelper::Event::GetPreambleType (void) const
{
  return m_preamble;
}

/****************************************************************
 *       Class which records SNIR change events for a 
 *       short period of time.
 ****************************************************************/

InterferenceHelper::NiChange::NiChange (Time time, double delta)
  : m_time (time), m_delta (delta) 
{}
Time
InterferenceHelper::NiChange::GetTime (void) const
{
  return m_time;
}
double 
InterferenceHelper::NiChange::GetDelta (void) const
{
  return m_delta;
}
bool 
InterferenceHelper::NiChange::operator < (const InterferenceHelper::NiChange& o) const
{
  return (m_time < o.m_time);
}

/****************************************************************
 *       The actual InterferenceHelper
 ****************************************************************/

InterferenceHelper::InterferenceHelper ()
  : m_maxPacketDuration (Seconds(0)),
    m_errorRateModel (0)  
{}
InterferenceHelper::~InterferenceHelper ()
{
  EraseEvents ();
  m_errorRateModel = 0;
}

Ptr<InterferenceHelper::Event> 
InterferenceHelper::Add (uint32_t size, WifiMode payloadMode, 
			 enum WifiPreamble preamble,
			 Time duration, double rxPowerW)
{
  Ptr<InterferenceHelper::Event> event;

  event = Create<InterferenceHelper::Event> 
    (size,
     payloadMode,
     preamble,
     duration,
     rxPowerW);

  m_maxPacketDuration = std::max(duration, m_maxPacketDuration);
  AppendEvent (event);
  return event;
}

Time 
InterferenceHelper::GetMaxPacketDuration (void) const
{
  return m_maxPacketDuration;
}

void 
InterferenceHelper::SetNoiseFigure (double value)
{
  m_noiseFigure = value;
}

double 
InterferenceHelper::GetNoiseFigure (void) const
{
  return m_noiseFigure;
}

void 
InterferenceHelper::SetErrorRateModel (Ptr<ErrorRateModel> rate)
{
  m_errorRateModel = rate;
}

Ptr<ErrorRateModel> 
InterferenceHelper::GetErrorRateModel (void) const
{
  return m_errorRateModel;
}

Time 
InterferenceHelper::GetEnergyDuration (double energyW)
{
  Time now = Simulator::Now ();

  // first, we iterate over all events and, each event
  // which contributes energy to the channel now is 
  // appended to the noise interference array.
  Events::const_iterator i = m_events.begin ();
  double noiseInterferenceW = 0.0;
  NiChanges ni;
  while (i != m_events.end ()) 
    {
      Ptr<Event> ev = *i;
      NS_ASSERT (ev->GetStartTime () <= now);
      if (ev->GetEndTime () > now)
	{
          ni.push_back (NiChange (ev->GetEndTime (), -ev->GetRxPowerW ()));
          noiseInterferenceW += ev->GetRxPowerW ();
	}
      i++;
    }
  if (noiseInterferenceW < energyW)
    {
      return MicroSeconds (0);
    }

  /* quicksort vector of NI changes by time. 
   */
  std::sort (ni.begin (), ni.end (), std::less<NiChange> ());

  // Now, we iterate the piecewise linear noise function
  Time end = now;
  for (NiChanges::const_iterator i = ni.begin (); i != ni.end (); i++) 
    {
      noiseInterferenceW += i->GetDelta ();
      end = i->GetTime ();
      if (noiseInterferenceW < energyW) 
	{
	  break;
	}
    }
  return end - now;
}

WifiMode 
InterferenceHelper::GetPlcpHeaderMode (WifiMode payloadMode, WifiPreamble preamble)
{
  switch (payloadMode.GetStandard ())
    {
    case WIFI_PHY_STANDARD_holland:
    case WIFI_PHY_STANDARD_80211a:
      // IEEE Std 802.11-2007, 17.3.2
      // actually this is only the first part of the PlcpHeader,
      // because the last 16 bits of the PlcpHeader are using the
      // same mode of the payload
      return WifiPhy::Get6mba ();      
      
    case WIFI_PHY_STANDARD_80211b:
      if (preamble == WIFI_PREAMBLE_LONG)
        {
          // IEEE Std 802.11-2007, sections 15.2.3 and 18.2.2.1 
          return WifiPhy::Get1mbb ();
        }
      else //  WIFI_PREAMBLE_SHORT
        {
          // IEEE Std 802.11-2007, section 18.2.2.2
          return WifiPhy::Get2mbb ();
        }
      
    case WIFI_PHY_STANDARD_80211_10Mhz:
      return WifiPhy::Get3mb10Mhz ();
      
    case WIFI_PHY_STANDARD_80211_5Mhz:
      return WifiPhy::Get1_5mb5Mhz ();
    
    default:
      NS_FATAL_ERROR("unknown standard");
      return WifiMode ();
    }
}

uint32_t
InterferenceHelper::GetPlcpHeaderDurationMicroSeconds (WifiMode payloadMode, WifiPreamble preamble)
{
    switch (payloadMode.GetStandard ())
    {
    case WIFI_PHY_STANDARD_holland:
    case WIFI_PHY_STANDARD_80211a:
      // IEEE Std 802.11-2007, section 17.3.3 and figure 17-4
      // also section 17.3.2.3, table 17-4
      // We return the duration of the SIGNAL field only, since the
      // SERVICE field (which strictly speaking belongs to the PLCP
      // header, see section 17.3.2 and figure 17-1) is sent using the
      // payload mode.  
      return 4; 
      
    case WIFI_PHY_STANDARD_80211_10Mhz:
      // IEEE Std 802.11-2007, section 17.3.2.3, table 17-4
      return 8;
      
    case WIFI_PHY_STANDARD_80211_5Mhz:
      // IEEE Std 802.11-2007, section 17.3.2.3, table 17-4
      return 16;

    case WIFI_PHY_STANDARD_80211b:
        if (preamble == WIFI_PREAMBLE_SHORT)
        {
          // IEEE Std 802.11-2007, section 18.2.2.2 and figure 18-2
          return 24;
        }
      else // WIFI_PREAMBLE_LONG
        {
          // IEEE Std 802.11-2007, sections 18.2.2.1 and figure 18-1
          return 48;
        }      
      
    default:
      NS_FATAL_ERROR("unknown standard");
      return 0;
    }
  
}

uint32_t 
InterferenceHelper::GetPlcpPreambleDurationMicroSeconds (WifiMode payloadMode, WifiPreamble preamble)
{
  switch (payloadMode.GetStandard ())
    {
    case WIFI_PHY_STANDARD_holland:
    case WIFI_PHY_STANDARD_80211a:
      // IEEE Std 802.11-2007, section 17.3.3,  figure 17-4
      // also section 17.3.2.3, table 17-4
      return 16; 
      
    case WIFI_PHY_STANDARD_80211_10Mhz:
      // IEEE Std 802.11-2007, section 17.3.3, table 17-4 
      // also section 17.3.2.3, table 17-4
      return 32;
      
    case WIFI_PHY_STANDARD_80211_5Mhz:
      // IEEE Std 802.11-2007, section 17.3.3
      // also section 17.3.2.3, table 17-4
      return 64;

    case WIFI_PHY_STANDARD_80211b:
      if (preamble == WIFI_PREAMBLE_SHORT)
        {
          // IEEE Std 802.11-2007, section 18.2.2.2 and figure 18-2
          return 72;
        }
      else // WIFI_PREAMBLE_LONG
        {
          // IEEE Std 802.11-2007, sections 18.2.2.1 and figure 18-1
          return 144;
        }
      
    default:
      NS_FATAL_ERROR("unknown standard");
      return 0;
    }
}

uint32_t 
InterferenceHelper::GetPayloadDurationMicroSeconds (uint32_t size, WifiMode payloadMode)
{
  NS_LOG_FUNCTION(size << payloadMode);
  switch (payloadMode.GetStandard ())
    {
    case WIFI_PHY_STANDARD_80211a:
    case WIFI_PHY_STANDARD_holland:
    case WIFI_PHY_STANDARD_80211_10Mhz: 
    case WIFI_PHY_STANDARD_80211_5Mhz: 
      {
        // IEEE Std 802.11-2007, section 17.3.2.3, table 17-4
        // corresponds to T_{SYM} in the table
        uint32_t symbolDurationUs; 
        switch (payloadMode.GetStandard ())
          {       
          case WIFI_PHY_STANDARD_holland:   
          case WIFI_PHY_STANDARD_80211a:
            symbolDurationUs = 4;
            break;
          case WIFI_PHY_STANDARD_80211_10Mhz: 
            symbolDurationUs = 8;
            break;
          case WIFI_PHY_STANDARD_80211_5Mhz: 
            symbolDurationUs = 16;
            break;
          case WIFI_PHY_STANDARD_80211b:
            NS_FATAL_ERROR("can't happen here");
            symbolDurationUs = 0; // quiet compiler
            break;
          default:
            NS_FATAL_ERROR("unknown standard");
            symbolDurationUs = 0; // quiet compiler
            break;
          }
      
        // IEEE Std 802.11-2007, section 17.3.2.2, table 17-3
        // corresponds to N_{DBPS} in the table
        double numDataBitsPerSymbol = payloadMode.GetDataRate ()  * symbolDurationUs / 1e6;

        // IEEE Std 802.11-2007, section 17.3.5.3, equation (17-11)
        uint32_t numSymbols = lrint (ceil ((16 + size * 8.0 + 6.0)/numDataBitsPerSymbol));
      
        return numSymbols*symbolDurationUs;
      }
    case WIFI_PHY_STANDARD_80211b:
      // IEEE Std 802.11-2007, section 18.2.3.5
      NS_LOG_LOGIC(" size=" << size
                   << " mode=" << payloadMode 
                   << " rate=" << payloadMode.GetDataRate () );
      return lrint(ceil ((size * 8.0) / (payloadMode.GetDataRate () / 1.0e6)));

    default:
      NS_FATAL_ERROR("unknown standard");
      return 0;
    }
}

Time
InterferenceHelper::CalculateTxDuration (uint32_t size, WifiMode payloadMode, WifiPreamble preamble) 
{
  uint32_t duration = GetPlcpPreambleDurationMicroSeconds (payloadMode, preamble)     
                      + GetPlcpHeaderDurationMicroSeconds (payloadMode, preamble)  
                      + GetPayloadDurationMicroSeconds (size, payloadMode);
  return MicroSeconds (duration);
}

void 
InterferenceHelper::AppendEvent (Ptr<InterferenceHelper::Event> event)
{
  /* attempt to remove the events which are 
   * not useful anymore. 
   * i.e.: all events which end _before_
   *       now - m_maxPacketDuration
   */
  
  if (Simulator::Now () > GetMaxPacketDuration ())
    {
      Time end = Simulator::Now () - GetMaxPacketDuration ();
      Events::iterator i = m_events.begin ();
      while (i != m_events.end () &&
             (*i)->GetEndTime () <= end) 
        {
          i++;
        }
      EraseEvents (m_events.begin (), i);
    } 
  m_events.push_back (event);
}


double
InterferenceHelper::CalculateSnr (double signal, double noiseInterference, WifiMode mode) const
{
  // thermal noise at 290K in J/s = W
  static const double BOLTZMANN = 1.3803e-23;
  // Nt is the power of thermal noise in W
  double Nt = BOLTZMANN * 290.0 * mode.GetBandwidth ();
  // receiver noise Floor (W) which accounts for thermal noise and non-idealities of the receiver
  double noiseFloor = m_noiseFigure * Nt;
  double noise = noiseFloor + noiseInterference;
  double snr = signal / noise;
  return snr;
}

double
InterferenceHelper::CalculateNoiseInterferenceW (Ptr<InterferenceHelper::Event> event, NiChanges *ni) const
{
  Events::const_iterator i = m_events.begin ();
  double noiseInterference = 0.0;
  while (i != m_events.end ()) 
    {
      if (event == (*i)) 
        {
          i++;
          continue;
        }
      if ((*i)->Overlaps (event->GetStartTime ())) 
        {
          noiseInterference += (*i)->GetRxPowerW ();
        }
      else if (event->Overlaps ((*i)->GetStartTime ())) 
        {
          ni->push_back (NiChange ((*i)->GetStartTime (), (*i)->GetRxPowerW ()));
        }
      if (event->Overlaps ((*i)->GetEndTime ())) 
        {
          ni->push_back (NiChange ((*i)->GetEndTime (), -(*i)->GetRxPowerW ()));
        }
      i++;
    }
  ni->push_back (NiChange (event->GetStartTime (), noiseInterference));
  ni->push_back (NiChange (event->GetEndTime (), 0));

  /* quicksort vector of NI changes by time. */
  std::sort (ni->begin (), ni->end (), std::less<NiChange> ());

  return noiseInterference;
}

double
InterferenceHelper::CalculateChunkSuccessRate (double snir, Time duration, WifiMode mode) const
{
  if (duration == NanoSeconds (0)) {
    return 1.0;
  }
  uint32_t rate = mode.GetPhyRate ();
  uint64_t nbits = (uint64_t)(rate * duration.GetSeconds ());
  double csr = m_errorRateModel->GetChunkSuccessRate (mode, snir, (uint32_t)nbits);
  return csr;
}

double 
InterferenceHelper::CalculatePer (Ptr<const InterferenceHelper::Event> event, NiChanges *ni) const
{  
  double psr = 1.0; /* Packet Success Rate */
  NiChanges::iterator j = ni->begin ();
  Time previous = (*j).GetTime (); 
  WifiMode payloadMode = event->GetPayloadMode ();
  WifiPreamble preamble = event->GetPreambleType ();
  WifiMode headerMode = GetPlcpHeaderMode (payloadMode, preamble);
  Time plcpHeaderStart = (*j).GetTime () + MicroSeconds (GetPlcpPreambleDurationMicroSeconds (payloadMode, preamble));
  Time plcpPayloadStart = plcpHeaderStart + MicroSeconds (GetPlcpHeaderDurationMicroSeconds (payloadMode, preamble));
  double noiseInterferenceW = (*j).GetDelta ();
  double powerW = event->GetRxPowerW ();

  j++;
  while (ni->end () != j) 
    {
      Time current = (*j).GetTime ();
      NS_ASSERT (current >= previous);
    
      if (previous >= plcpPayloadStart) 
        {
          psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                          noiseInterferenceW, 
                                                          payloadMode), 
                                            current - previous,
                                            payloadMode);
        } 
      else if (previous >= plcpHeaderStart) 
        {
          if (current >= plcpPayloadStart) 
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              headerMode), 
                                                plcpPayloadStart - previous,
                                                headerMode);
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              payloadMode),
                                                current - plcpPayloadStart,
                                                payloadMode);
            } 
          else 
            {
              NS_ASSERT (current >= plcpHeaderStart);
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              headerMode), 
                                                current - previous,
                                                headerMode);
            }
        } 
      else 
        {
          if (current >= plcpPayloadStart) 
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              headerMode), 
                                                plcpPayloadStart - plcpHeaderStart,
                                                headerMode);
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              payloadMode), 
                                                current - plcpPayloadStart,
                                                payloadMode);
            } 
          else if (current >= plcpHeaderStart) 
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                              noiseInterferenceW, 
                                                              headerMode), 
                                                current - plcpHeaderStart,
                                                headerMode);
            }
        }

      noiseInterferenceW += (*j).GetDelta ();
      previous = (*j).GetTime ();
      j++;
    }

  double per = 1 - psr;
  return per;
}


struct InterferenceHelper::SnrPer 
InterferenceHelper::CalculateSnrPer (Ptr<InterferenceHelper::Event> event)
{
  NiChanges ni;
  double noiseInterferenceW = CalculateNoiseInterferenceW (event, &ni);
  double snr = CalculateSnr (event->GetRxPowerW (),
                             noiseInterferenceW,
                             event->GetPayloadMode ());
  
  /* calculate the SNIR at the start of the packet and accumulate
   * all SNIR changes in the snir vector.
   */
  double per = CalculatePer (event, &ni);

  struct SnrPer snrPer;
  snrPer.snr = snr;
  snrPer.per = per;
  return snrPer;
}

void
InterferenceHelper::EraseEvents (void) 
{  
  for (Events::iterator i = m_events.begin (); i != m_events.end (); ++i)
    {
      *i = 0;
    }
  m_events.clear ();
}

void
InterferenceHelper::EraseEvents (Events::iterator start, Events::iterator end) 
{  
  for (Events::iterator i = start; i != end; ++i)
    {
      *i = 0;
    }
  m_events.erase (start, end);
}

} // namespace ns3
