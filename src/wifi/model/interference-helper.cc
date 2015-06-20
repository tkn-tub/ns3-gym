/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("InterferenceHelper");

/****************************************************************
 *       Phy event class
 ****************************************************************/

InterferenceHelper::Event::Event (uint32_t size, WifiTxVector txVector,
                                  enum WifiPreamble preamble,
                                  Time duration, double rxPower)
  : m_size (size),
    m_txVector (txVector),
    m_preamble (preamble),
    m_startTime (Simulator::Now ()),
    m_endTime (m_startTime + duration),
    m_rxPowerW (rxPower)
{
}

InterferenceHelper::Event::~Event ()
{
}

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

WifiTxVector
InterferenceHelper::Event::GetTxVector (void) const
{
  return m_txVector;
}

WifiMode
InterferenceHelper::Event::GetPayloadMode (void) const
{
  return m_txVector.GetMode ();
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
  : m_time (time),
    m_delta (delta)
{
}

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
  : m_errorRateModel (0),
    m_firstPower (0.0),
    m_rxing (false)
{
}

InterferenceHelper::~InterferenceHelper ()
{
  EraseEvents ();
  m_errorRateModel = 0;
}

Ptr<InterferenceHelper::Event>
InterferenceHelper::Add (uint32_t size, WifiTxVector txVector,
                         enum WifiPreamble preamble,
                         Time duration, double rxPowerW)
{
  Ptr<InterferenceHelper::Event> event;

  event = Create<InterferenceHelper::Event> (size,
                                             txVector,
                                             preamble,
                                             duration,
                                             rxPowerW);
  AppendEvent (event);
  return event;
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
  double noiseInterferenceW = 0.0;
  Time end = now;
  noiseInterferenceW = m_firstPower;
  for (NiChanges::const_iterator i = m_niChanges.begin (); i != m_niChanges.end (); i++)
    {
      noiseInterferenceW += i->GetDelta ();
      end = i->GetTime ();
      if (end < now)
        {
          continue;
        }
      if (noiseInterferenceW < energyW)
        {
          break;
        }
    }
  return end > now ? end - now : MicroSeconds (0);
}

void
InterferenceHelper::AppendEvent (Ptr<InterferenceHelper::Event> event)
{
  Time now = Simulator::Now ();
  if (!m_rxing)
    {
      NiChanges::iterator nowIterator = GetPosition (now);
      for (NiChanges::iterator i = m_niChanges.begin (); i != nowIterator; i++)
        {
          m_firstPower += i->GetDelta ();
        }
      m_niChanges.erase (m_niChanges.begin (), nowIterator);
      m_niChanges.insert (m_niChanges.begin (), NiChange (event->GetStartTime (), event->GetRxPowerW ()));
    }
  else
    {
      AddNiChangeEvent (NiChange (event->GetStartTime (), event->GetRxPowerW ()));
    }
  AddNiChangeEvent (NiChange (event->GetEndTime (), -event->GetRxPowerW ()));

}


double
InterferenceHelper::CalculateSnr (double signal, double noiseInterference, WifiMode mode) const
{
  //thermal noise at 290K in J/s = W
  static const double BOLTZMANN = 1.3803e-23;
  //Nt is the power of thermal noise in W
  double Nt = BOLTZMANN * 290.0 * mode.GetBandwidth ();
  //receiver noise Floor (W) which accounts for thermal noise and non-idealities of the receiver
  double noiseFloor = m_noiseFigure * Nt;
  double noise = noiseFloor + noiseInterference;
  double snr = signal / noise;
  return snr;
}

double
InterferenceHelper::CalculateNoiseInterferenceW (Ptr<InterferenceHelper::Event> event, NiChanges *ni) const
{
  double noiseInterference = m_firstPower;
  NS_ASSERT (m_rxing);
  for (NiChanges::const_iterator i = m_niChanges.begin () + 1; i != m_niChanges.end (); i++)
    {
      if ((event->GetEndTime () == i->GetTime ()) && event->GetRxPowerW () == -i->GetDelta ())
        {
          break;
        }
      ni->push_back (*i);
    }
  ni->insert (ni->begin (), NiChange (event->GetStartTime (), noiseInterference));
  ni->push_back (NiChange (event->GetEndTime (), 0));
  return noiseInterference;
}

double
InterferenceHelper::CalculateChunkSuccessRate (double snir, Time duration, WifiMode mode) const
{
  if (duration == NanoSeconds (0))
    {
      return 1.0;
    }
  uint32_t rate = mode.GetPhyRate ();
  uint64_t nbits = (uint64_t)(rate * duration.GetSeconds ());
  double csr = m_errorRateModel->GetChunkSuccessRate (mode, snir, (uint32_t)nbits);
  return csr;
}

double
InterferenceHelper::CalculatePlcpPayloadPer (Ptr<const InterferenceHelper::Event> event, NiChanges *ni) const
{
  double psr = 1.0; /* Packet Success Rate */
  NiChanges::iterator j = ni->begin ();
  Time previous = (*j).GetTime ();
  WifiMode payloadMode = event->GetPayloadMode ();
  WifiPreamble preamble = event->GetPreambleType ();
  Time plcpHeaderStart = (*j).GetTime () + WifiPhy::GetPlcpPreambleDuration (payloadMode, preamble); //packet start time + preamble
  Time plcpHsigHeaderStart = plcpHeaderStart + WifiPhy::GetPlcpHeaderDuration (payloadMode, preamble); //packet start time + preamble + L-SIG
  Time plcpHtTrainingSymbolsStart = plcpHsigHeaderStart + WifiPhy::GetPlcpHtSigHeaderDuration (preamble); //packet start time + preamble + L-SIG + HT-SIG
  Time plcpPayloadStart = plcpHtTrainingSymbolsStart + WifiPhy::GetPlcpHtTrainingSymbolDuration (preamble,event->GetTxVector ()); //packet start time + preamble + L-SIG + HT-SIG + HT Training
  double noiseInterferenceW = (*j).GetDelta ();
  double powerW = event->GetRxPowerW ();
  j++;
  while (ni->end () != j)
    {
      Time current = (*j).GetTime ();
      NS_ASSERT (current >= previous);
      //Case 1: Both previous and current point to the payload
      if (previous >= plcpPayloadStart)
        {
          psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                          noiseInterferenceW,
                                                          payloadMode),
                                            current - previous,
                                            payloadMode);
        }
      //Case 2: previous is before payload
      else if (previous >= plcpHtTrainingSymbolsStart)
        {
          //Case 2a: current is after payload
          if (current >= plcpPayloadStart)
            {
              //Case 2ai and 2aii: All formats
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              payloadMode),
                                                current - plcpPayloadStart,
                                                payloadMode);
            }
        }
      //Case 3: previous is in HT-SIG: Non HT will not enter here since it didn't enter in the last two and they are all the same for non HT
      else if (previous >= plcpHsigHeaderStart)
        {
          //Case 3a: cuurent after payload start
          if (current >= plcpPayloadStart)
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              payloadMode),
                                                current - plcpPayloadStart,
                                                payloadMode);

            }
        }
      //Case 4: previous in L-SIG: GF will not reach here because it will execute the previous if and exit
      else if (previous >= plcpHeaderStart)
        {
          //Case 4a: current after payload start
          if (current >= plcpPayloadStart)
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              payloadMode),
                                                current - plcpPayloadStart,
                                                payloadMode);

            }
        }
      //Case 5: previous is in the preamble works for all cases
      else
        {
          if (current >= plcpPayloadStart)
            {
              //for all
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              payloadMode),
                                                current - plcpPayloadStart,
                                                payloadMode);
            }
        }

      noiseInterferenceW += (*j).GetDelta ();
      previous = (*j).GetTime ();
      j++;
    }

  double per = 1 - psr;
  return per;
}

double
InterferenceHelper::CalculatePlcpHeaderPer (Ptr<const InterferenceHelper::Event> event, NiChanges *ni) const
{
  double psr = 1.0; /* Packet Success Rate */
  NiChanges::iterator j = ni->begin ();
  Time previous = (*j).GetTime ();
  WifiMode payloadMode = event->GetPayloadMode ();
  WifiPreamble preamble = event->GetPreambleType ();
  WifiMode MfHeaderMode;
  if (preamble == WIFI_PREAMBLE_HT_MF)
    {
      MfHeaderMode = WifiPhy::GetMFPlcpHeaderMode (payloadMode, preamble);
    }
  WifiMode headerMode = WifiPhy::GetPlcpHeaderMode (payloadMode, preamble);
  Time plcpHeaderStart = (*j).GetTime () + WifiPhy::GetPlcpPreambleDuration (payloadMode, preamble); //packet start time + preamble
  Time plcpHsigHeaderStart = plcpHeaderStart + WifiPhy::GetPlcpHeaderDuration (payloadMode, preamble); //packet start time + preamble + L-SIG
  Time plcpHtTrainingSymbolsStart = plcpHsigHeaderStart + WifiPhy::GetPlcpHtSigHeaderDuration (preamble); //packet start time + preamble + L-SIG + HT-SIG
  Time plcpPayloadStart = plcpHtTrainingSymbolsStart + WifiPhy::GetPlcpHtTrainingSymbolDuration (preamble, event->GetTxVector ()); //packet start time + preamble + L-SIG + HT-SIG + HT Training
  double noiseInterferenceW = (*j).GetDelta ();
  double powerW = event->GetRxPowerW ();
  j++;
  while (ni->end () != j)
    {
      Time current = (*j).GetTime ();
      NS_ASSERT (current >= previous);
      //Case 1: previous is in HT-SIG: Non HT will not enter here since it didn't enter in the last two and they are all the same for non HT
      if (previous >= plcpHsigHeaderStart)
        {
          //Case 1a: current after payload start
          if (current >= plcpPayloadStart)
            {


              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              headerMode),
                                                plcpHtTrainingSymbolsStart - previous,
                                                headerMode);
            }
          //case 1b: current after HT training symbols start
          else if (current >= plcpHtTrainingSymbolsStart)
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              headerMode),
                                                plcpHtTrainingSymbolsStart - previous,
                                                headerMode);

            }
          //Case 1c: current is with previous in HT sig
          else
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              headerMode),
                                                current - previous,
                                                headerMode);
            }
        }
      //Case 2: previous in L-SIG: GF will not reach here because it will execute the previous if and exit
      else if (previous >= plcpHeaderStart)
        {
          //Case 2a: current after payload start
          if (current >= plcpPayloadStart)
            {
              //Case 2ai: Non HT format (No HT-SIG or Training Symbols)
              if (preamble == WIFI_PREAMBLE_LONG || preamble == WIFI_PREAMBLE_SHORT)
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  headerMode),
                                                    plcpPayloadStart - previous,
                                                    headerMode);
                }
              else
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  headerMode),
                                                    plcpHtTrainingSymbolsStart - plcpHsigHeaderStart,
                                                    headerMode);
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  MfHeaderMode),
                                                    plcpHsigHeaderStart - previous,
                                                    MfHeaderMode);
                }
            }
          //Case 2b: current in HT training symbol. non HT will not come here since it went in previous if or if the previous if is not true this will be not true
          else if (current >= plcpHtTrainingSymbolsStart)
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              headerMode),
                                                plcpHtTrainingSymbolsStart - plcpHsigHeaderStart,
                                                headerMode);
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              MfHeaderMode),
                                                plcpHsigHeaderStart - previous,
                                                MfHeaderMode);
            }
          //Case 2c: current in HT-SIG. non HT will not come here since it went in previous if or if the previous if is not true this will be not true
          else if (current >= plcpHsigHeaderStart)
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              headerMode),
                                                current - plcpHsigHeaderStart,
                                                headerMode);
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              MfHeaderMode),
                                                plcpHsigHeaderStart - previous,
                                                MfHeaderMode);

            }
          //Case 2d: Current with previous in L-SIG
          else
            {
              // Case 4di: Non HT format (No HT-SIG or Training Symbols)
              if (preamble == WIFI_PREAMBLE_LONG || preamble == WIFI_PREAMBLE_SHORT)
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  headerMode),
                                                    current - previous,
                                                    headerMode);
                }
              else
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  MfHeaderMode),
                                                    current - previous,
                                                    MfHeaderMode);
                }
            }
        }
      //Case 3: previous is in the preamble works for all cases
      else
        {
          if (current >= plcpPayloadStart)
            {
              //Non HT format (No HT-SIG or Training Symbols)
              if (preamble == WIFI_PREAMBLE_LONG || preamble == WIFI_PREAMBLE_SHORT)
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  headerMode),
                                                    plcpPayloadStart - plcpHeaderStart,
                                                    headerMode);
                }
              else
                {
                  //Greenfield or Mixed format
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  headerMode),
                                                    plcpHtTrainingSymbolsStart - plcpHsigHeaderStart,
                                                    headerMode);
                }
              if (preamble == WIFI_PREAMBLE_HT_MF)
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  MfHeaderMode),
                                                    plcpHsigHeaderStart - plcpHeaderStart,
                                                    MfHeaderMode);
                }
            }
          else if (current >= plcpHtTrainingSymbolsStart )
            {
              //Non HT format will not come here since it will execute prev if
              //Greenfield or Mixed format
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              headerMode),
                                                plcpHtTrainingSymbolsStart - plcpHsigHeaderStart,
                                                headerMode);
              //Greenfield
              if (preamble == WIFI_PREAMBLE_HT_MF)
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  MfHeaderMode),
                                                    plcpHsigHeaderStart - plcpHeaderStart,
                                                    MfHeaderMode);
                }
            }
          //non HT will not come here
          else if (current >= plcpHsigHeaderStart)
            {
              psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                              noiseInterferenceW,
                                                              headerMode),
                                                current - plcpHsigHeaderStart,
                                                headerMode);
              if  (preamble != WIFI_PREAMBLE_HT_GF)
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  MfHeaderMode),
                                                    plcpHsigHeaderStart - plcpHeaderStart,
                                                    MfHeaderMode);
                }
            }
          //GF will not come here
          else if (current >= plcpHeaderStart)
            {
              if (preamble == WIFI_PREAMBLE_LONG || preamble == WIFI_PREAMBLE_SHORT)
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  headerMode),
                                                    current - plcpHeaderStart,
                                                    headerMode);
                }
              else
                {
                  psr *= CalculateChunkSuccessRate (CalculateSnr (powerW,
                                                                  noiseInterferenceW,
                                                                  MfHeaderMode),
                                                    current - plcpHeaderStart,
                                                    MfHeaderMode);
                }
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
InterferenceHelper::CalculatePlcpPayloadSnrPer (Ptr<InterferenceHelper::Event> event)
{
  NiChanges ni;
  double noiseInterferenceW = CalculateNoiseInterferenceW (event, &ni);
  double snr = CalculateSnr (event->GetRxPowerW (),
                             noiseInterferenceW,
                             event->GetPayloadMode ());

  /* calculate the SNIR at the start of the packet and accumulate
   * all SNIR changes in the snir vector.
   */
  double per = CalculatePlcpPayloadPer (event, &ni);

  struct SnrPer snrPer;
  snrPer.snr = snr;
  snrPer.per = per;
  return snrPer;
}

struct InterferenceHelper::SnrPer
InterferenceHelper::CalculatePlcpHeaderSnrPer (Ptr<InterferenceHelper::Event> event)
{
  NiChanges ni;
  double noiseInterferenceW = CalculateNoiseInterferenceW (event, &ni);
  double snr = CalculateSnr (event->GetRxPowerW (),
                             noiseInterferenceW,
                             WifiPhy::GetPlcpHeaderMode (event->GetPayloadMode (), event->GetPreambleType ()));

  /* calculate the SNIR at the start of the plcp header and accumulate
   * all SNIR changes in the snir vector.
   */
  double per = CalculatePlcpHeaderPer (event, &ni);

  struct SnrPer snrPer;
  snrPer.snr = snr;
  snrPer.per = per;
  return snrPer;
}

void
InterferenceHelper::EraseEvents (void)
{
  m_niChanges.clear ();
  m_rxing = false;
  m_firstPower = 0.0;
}

InterferenceHelper::NiChanges::iterator
InterferenceHelper::GetPosition (Time moment)
{
  return std::upper_bound (m_niChanges.begin (), m_niChanges.end (), NiChange (moment, 0));
}

void
InterferenceHelper::AddNiChangeEvent (NiChange change)
{
  m_niChanges.insert (GetPosition (change.GetTime ()), change);
}

void
InterferenceHelper::NotifyRxStart ()
{
  m_rxing = true;
}

void
InterferenceHelper::NotifyRxEnd ()
{
  m_rxing = false;
}

} //namespace ns3
