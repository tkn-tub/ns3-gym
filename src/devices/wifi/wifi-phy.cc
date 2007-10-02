/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
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

#include "wifi-phy.h"
#include "bpsk-mode.h"
#include "qam-mode.h"
#include "propagation-model.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/random-variable.h"

#include <cassert>
#include <math.h>


#define nopePHY80211_DEBUG 1
#define nopePHY80211_STATE_DEBUG 1

/* All the state transitions are marked by these macros. */
#ifdef PHY80211_STATE_DEBUG
#include <iostream>
#  define STATE_FROM(from) \
std::cout << "PHY self=" << this << " old=" << StateToString (from);
#  define STATE_TO(to) \
std::cout << " new=" << StateToString (to);
#  define STATE_AT(at) \
std::cout << " at=" << at << std::endl;
#else
#  define STATE_FROM(from)
#  define STATE_TO(from)
#  define STATE_AT(at)
#endif

#ifdef PHY80211_DEBUG
#include <iostream>
#  define TRACE(x) \
  std::cout << "PHY80211 TRACE " << Simulator::Now ().GetSeconds () << " " << x << std::endl;
#else
#  define TRACE(x)
#endif


namespace ns3 {

/****************************************************************
 *       This destructor is needed.
 ****************************************************************/

WifiPhyListener::~WifiPhyListener ()
{}


/****************************************************************
 *       Phy event class
 ****************************************************************/

class RxEvent {
public:
  RxEvent (uint32_t size, uint8_t payloadMode, 
           Time duration, double rxPower)
    : m_size (size),
      m_payloadMode (payloadMode),
      m_startTime (Simulator::Now ()),
      m_endTime (m_startTime + duration),
      m_rxPowerW (rxPower),
      m_refCount (1)
  {}
  ~RxEvent ()
  {}
  
  void Ref (void) const {
    m_refCount++;
  }
  void Unref (void) const {
    m_refCount--;
    if (m_refCount == 0) {
      delete this;
    }
  }
  Time GetDuration (void) const {
    return m_endTime - m_startTime;
  }
  Time GetStartTime (void) const {
    return m_startTime;
  }
  Time GetEndTime (void) const {
    return m_endTime;
  }
  bool Overlaps (Time time) const {
    if (m_startTime <= time &&
        m_endTime >= time) {
      return true;
    } else {
      return false;
    }
  }
  double GetRxPowerW (void) const {
    return m_rxPowerW;
  }
  uint32_t GetSize (void) const {
    return m_size;
  }
  uint8_t GetPayloadMode (void) const {
    return m_payloadMode;
  }
  uint8_t GetHeaderMode (void) const {
    return 0;
  }

private:
  uint32_t m_size;
  uint8_t m_payloadMode;
  Time m_startTime;
  Time m_endTime;
  double m_rxPowerW;
  mutable int m_refCount;
};


/****************************************************************
 *       Class which records SNIR change events for a 
 *       short period of time.
 ****************************************************************/

WifiPhy::NiChange::NiChange (Time time, double delta)
  : m_time (time), m_delta (delta) 
{}
Time
WifiPhy::NiChange::GetTime (void) const
{
  return m_time;
}
double 
WifiPhy::NiChange::GetDelta (void) const
{
  return m_delta;
}
bool 
WifiPhy::NiChange::operator < (WifiPhy::NiChange const &o) const
{
  return (m_time < o.m_time)?true:false;
}



/****************************************************************
 *       The actual WifiPhy class
 ****************************************************************/

WifiPhy::WifiPhy ()
  : m_syncing (false),
    m_endTx (Seconds (0)),
    m_endSync (Seconds (0)),
    m_endCcaBusy (Seconds (0)),
    m_startTx (Seconds (0)),
    m_startSync (Seconds (0)),
    m_startCcaBusy (Seconds (0)),
    m_previousStateChangeTime (Seconds (0)),
    m_endSyncEvent (),
    m_random (0.0, 1.0)
{}

WifiPhy::~WifiPhy ()
{
  m_events.clear ();
  for (ModesCI j = m_modes.begin (); j != m_modes.end (); j++) {
    delete (*j);
  }
  m_modes.erase (m_modes.begin (), m_modes.end ());
}


void 
WifiPhy::SetPropagationModel (PropagationModel *propagation)
{
  m_propagation = propagation;
}

void 
WifiPhy::SetReceiveOkCallback (SyncOkCallback callback)
{
  m_syncOkCallback = callback;
}
void 
WifiPhy::SetReceiveErrorCallback (SyncErrorCallback callback)
{
  m_syncErrorCallback = callback;
}
void 
WifiPhy::ReceivePacket (Packet const packet, 
                         double rxPowerW,
                         uint8_t txMode,
                         uint8_t stuff)
{
  Time rxDuration = CalculateTxDuration (packet.GetSize (), txMode);
  Time endRx = Simulator::Now () + rxDuration;
  m_startRxLogger (rxDuration, rxPowerW);

  Ptr<RxEvent> event = Create<RxEvent> (packet.GetSize (), 
                                        txMode,
                                        rxDuration,
                                        rxPowerW);
  AppendEvent (event);

  switch (GetState ()) {
  case WifiPhy::SYNC:
    TRACE ("drop packet because already in Sync (power="<<
           rxPowerW<<"W)");
    if (endRx > m_endSync) 
      {
        goto maybeCcaBusy;
      }
    break;
  case WifiPhy::TX:
    TRACE ("drop packet because already in Tx (power="<<
           rxPowerW<<"W)");
    if (endRx > m_endTx) 
      {
        goto maybeCcaBusy;
      }
    break;
  case WifiPhy::CCA_BUSY:
  case WifiPhy::IDLE:
    if (rxPowerW > m_edThresholdW) 
      {
        // sync to signal
        NotifySyncStart (rxDuration);
        SwitchToSync (rxDuration);
        m_startSyncLogger (rxDuration, rxPowerW);
        assert (m_endSyncEvent.IsExpired ());
        m_endSyncEvent = Simulator::Schedule (rxDuration, &WifiPhy::EndSync, this, 
                                              packet,
                                              event, 
                                              stuff);
      } 
    else 
      {
        TRACE ("drop packet because signal power too Small ("<<
               rxPowerW<<"<"<<m_edThresholdW<<")");
        goto maybeCcaBusy;
      }
    break;
  }

  event->Unref ();
  return;

 maybeCcaBusy:

  if (rxPowerW > m_edThresholdW) 
    {
      SwitchMaybeToCcaBusy (rxDuration);
      NotifyCcaBusyStart (rxDuration);
    } 
  else 
    {
      double threshold = m_edThresholdW - rxPowerW;
      NiChanges ni;
      CalculateNoiseInterferenceW (event, &ni);
      double noiseInterferenceW = 0.0;
      Time end = Simulator::Now ();
      for (NiChangesI i = ni.begin (); i != ni.end (); i++) 
        {
          noiseInterferenceW += i->GetDelta ();
          if (noiseInterferenceW < threshold) 
            {
              break;
            }
          end = i->GetTime ();
        }
      if (end > Simulator::Now ()) 
        {
          Time delta = end - Simulator::Now ();
          SwitchMaybeToCcaBusy (delta);
          NotifyCcaBusyStart (delta);
        }
    }

  event->Unref ();
}
void 
WifiPhy::SendPacket (Packet const packet, uint8_t txMode, uint8_t txPower, uint8_t stuff)
{
  /* Transmission can happen if:
   *  - we are syncing on a packet. It is the responsability of the
   *    MAC layer to avoid doing this but the PHY does nothing to 
   *    prevent it.
   *  - we are idle
   */
  assert (!IsStateTx ());

  if (IsStateSync ()) {
    m_endSyncEvent.Cancel ();
  }

  Time txDuration = CalculateTxDuration (packet.GetSize (), txMode);
  m_startTxLogger (txDuration, GetModeBitRate (txMode), GetPowerDbm (txPower));
  NotifyTxStart (txDuration);
  SwitchToTx (txDuration);
  m_propagation->Send (packet, GetPowerDbm (txPower), txMode, stuff);
}

void 
WifiPhy::SetEdThresholdDbm (double edThreshold)
{
  m_edThresholdW = DbmToW (edThreshold);
}
void 
WifiPhy::SetRxNoiseDb (double rxNoise)
{
  m_rxNoiseRatio = DbToRatio (rxNoise);
}
void 
WifiPhy::SetTxPowerIncrementsDbm (double txPowerBase, 
                                   double txPowerEnd, 
                                   int nTxPower)
{
  m_txPowerBaseDbm = txPowerBase;
  m_txPowerEndDbm = txPowerEnd;
  m_nTxPower = nTxPower;
}
uint32_t 
WifiPhy::GetNModes (void) const
{
  return m_modes.size ();
}
uint32_t 
WifiPhy::GetModeBitRate (uint8_t mode) const
{
  return GetMode (mode)->GetRate ();
}
uint32_t 
WifiPhy::GetNTxpower (void) const
{
  return m_nTxPower;
}

double 
WifiPhy::CalculateSnr (uint8_t txMode, double ber) const
{
  return GetSnrForBer (GetMode (txMode), ber);;
}

double 
WifiPhy::GetSnrForBer (TransmissionMode *mode, double ber) const
{
  double low, high, precision;
  low = 1e-25;
  high = 1e25;
  precision = 1e-12;
  while (high - low > precision) 
    {
      assert (high >= low);
      double middle = low + (high - low) / 2;
      if ((1 - mode->GetChunkSuccessRate (middle, 1)) > ber) 
        {
          low = middle;
        } 
      else 
        {
          high = middle;
        }
    }
  return low;
}

void
WifiPhy::Configure80211a (void)
{
  m_plcpPreambleDelayUs = 20;
  m_plcpHeaderLength = 4 + 1 + 12 + 1 + 6 + 16 + 6;
  /* 4095 bytes at a 6Mb/s rate with a 1/2 coding rate. */
  m_maxPacketDuration = Seconds (4095.0*8.0/6000000.0*(1.0/2.0));

  AddTxRxMode (new FecBpskMode (20e6, 6000000, 0.5,   10, 11));
  AddTxRxMode (new FecBpskMode (20e6, 9000000, 0.75,  5, 8));
  AddTxRxMode (new FecQamMode (20e6, 12000000, 0.5,   4, 10, 11, 0));
  AddTxRxMode (new FecQamMode (20e6, 18000000, 0.75,  4, 5, 8, 31));
  //AddTxRxMode (new FecQamMode (20e6, 24000000, 0.5,   16, 10, 11, 0));
  AddTxRxMode (new FecQamMode (20e6, 36000000, 0.75,  16, 5, 8, 31));
  //AddTxRxMode (new FecQamMode (20e6, 48000000, 0.666, 64, 6, 1, 16));
  AddTxRxMode (new FecQamMode (20e6, 54000000, 0.75,  64, 5, 8, 31));

#ifdef PHY80211_DEBUG
  for (double db = 0; db < 30; db+= 0.5) {
    std::cout <<db<<" ";
    for (uint8_t i = 0; i < GetNModes (); i++) {
      TransmissionMode *mode = GetMode (i);
      double ber = 1-mode->GetChunkSuccessRate (DbToRatio (db), 1);
      std::cout <<ber<< " ";
    }
    std::cout << std::endl;
  }
#endif
}

void 
WifiPhy::RegisterListener (WifiPhyListener *listener)
{
  m_listeners.push_back (listener);
}

bool 
WifiPhy::IsStateCcaBusy (void)
{
  return GetState () == CCA_BUSY;
}

bool 
WifiPhy::IsStateIdle (void)
{
  return (GetState () == IDLE)?true:false;
}
bool 
WifiPhy::IsStateBusy (void)
{
  return (GetState () != IDLE)?true:false;
}
bool 
WifiPhy::IsStateSync (void)
{
  return (GetState () == SYNC)?true:false;
}
bool 
WifiPhy::IsStateTx (void)
{
  return (GetState () == TX)?true:false;
}

Time
WifiPhy::GetStateDuration (void)
{
  return Simulator::Now () - m_previousStateChangeTime;
}
Time
WifiPhy::GetDelayUntilIdle (void)
{
  Time retval;

  switch (GetState ()) {
  case SYNC:
    retval = m_endSync - Simulator::Now ();
    break;
  case TX:
    retval = m_endTx - Simulator::Now ();
    break;
  case CCA_BUSY:
    retval = m_endCcaBusy - Simulator::Now ();
    break;
  case IDLE:
    retval = Seconds (0);
    break;
  default:
    assert (false);
    // NOTREACHED
    retval = Seconds (0);
    break;
  }
  retval = Max (retval, Seconds (0));
  return retval;
}


Time
WifiPhy::CalculateTxDuration (uint32_t size, uint8_t payloadMode) const
{
  uint64_t delay = m_plcpPreambleDelayUs;
  delay += m_plcpHeaderLength * 1000000 / GetMode (0)->GetDataRate ();
  uint64_t nbits = size * 8;
  delay += nbits * 1000000 / GetMode (payloadMode)->GetDataRate ();
  return MicroSeconds (delay);
}

char const *
WifiPhy::StateToString (enum WifiPhyState state)
{
  switch (state) {
  case TX:
    return "TX";
    break;
  case CCA_BUSY:
    return "CCA_BUSY";
    break;
  case IDLE:
    return "IDLE";
    break;
  case SYNC:
    return "SYNC";
    break;
  default:
    return "XXX";
    break;
  }
}
enum WifiPhy::WifiPhyState 
WifiPhy::GetState (void)
{
  if (m_endTx > Simulator::Now ()) 
    {
      return WifiPhy::TX;
    } 
  else if (m_syncing) 
    {
      return WifiPhy::SYNC;
    } 
  else if (m_endCcaBusy > Simulator::Now ()) 
    {
      return WifiPhy::CCA_BUSY;
    } 
  else 
    {
      return WifiPhy::IDLE;
    }
}

double 
WifiPhy::DbToRatio (double dB) const
{
  double ratio = pow(10.0,dB/10.0);
  return ratio;
}

double 
WifiPhy::DbmToW (double dBm) const
{
  double mW = pow(10.0,dBm/10.0);
  return mW / 1000.0;
}

double
WifiPhy::GetEdThresholdW (void) const
{
  return m_edThresholdW;
}

Time
WifiPhy::GetMaxPacketDuration (void) const
{
  return m_maxPacketDuration;
}

void
WifiPhy::AddTxRxMode (TransmissionMode *mode)
{
  m_modes.push_back (mode);
}

TransmissionMode *
WifiPhy::GetMode (uint8_t mode) const
{
  return m_modes[mode];
}

double 
WifiPhy::GetPowerDbm (uint8_t power) const
{
  assert (m_txPowerBaseDbm <= m_txPowerEndDbm);
  assert (m_nTxPower > 0);
  double dbm = m_txPowerBaseDbm + (m_txPowerEndDbm - m_txPowerBaseDbm) / m_nTxPower;
  return dbm;
}

void 
WifiPhy::NotifyTxStart (Time duration)
{
  for (ListenersCI i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyTxStart (duration);
  }
}
void 
WifiPhy::NotifySyncStart (Time duration)
{
  for (ListenersCI i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxStart (duration);
  }
}
void 
WifiPhy::NotifySyncEndOk (void)
{
  for (ListenersCI i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxEndOk ();
  }
}
void 
WifiPhy::NotifySyncEndError (void)
{
  for (ListenersCI i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxEndError ();
  }
}
void 
WifiPhy::NotifyCcaBusyStart (Time duration)
{
  for (ListenersCI i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyCcaBusyStart (duration);
  }
}

void
WifiPhy::LogPreviousIdleAndCcaBusyStates (void)
{
  Time now = Simulator::Now ();
  Time idleStart = Max (m_endCcaBusy, m_endSync);
  idleStart = Max (idleStart, m_endTx);
  assert (idleStart <= now);
  if (m_endCcaBusy > m_endSync && 
      m_endCcaBusy > m_endTx) {
    Time ccaBusyStart = Max (m_endTx, m_endSync);
    ccaBusyStart = Max (ccaBusyStart, m_startCcaBusy);
    m_stateLogger (ccaBusyStart, idleStart - ccaBusyStart, 2);
  }
  m_stateLogger (idleStart, now - idleStart, 3);
}

void
WifiPhy::SwitchToTx (Time txDuration)
{
  Time now = Simulator::Now ();
  switch (GetState ()) {
  case WifiPhy::SYNC:
    /* The packet which is being received as well
     * as its endSync event are cancelled by the caller.
     */
    m_syncing = false;
    m_stateLogger (m_startSync, now - m_startSync, 1);
    break;
  case WifiPhy::CCA_BUSY: {
    Time ccaStart = Max (m_endSync, m_endTx);
    ccaStart = Max (ccaStart, m_startCcaBusy);
    m_stateLogger (ccaStart, now - ccaStart, 2);
  } break;
  case WifiPhy::IDLE:
    LogPreviousIdleAndCcaBusyStates ();
    break;
  default:
    assert (false);
    break;
  }
  m_stateLogger (now, txDuration, 0);
  m_previousStateChangeTime = now;
  m_endTx = now + txDuration;
  m_startTx = now;
}
void
WifiPhy::SwitchToSync (Time rxDuration)
{
  assert (IsStateIdle () || IsStateCcaBusy ());
  assert (!m_syncing);
  Time now = Simulator::Now ();
  switch (GetState ()) {
  case WifiPhy::IDLE:
    LogPreviousIdleAndCcaBusyStates ();
    break;
  case WifiPhy::CCA_BUSY: {
    Time ccaStart = Max (m_endSync, m_endTx);
    ccaStart = Max (ccaStart, m_startCcaBusy);
    m_stateLogger (ccaStart, now - ccaStart, 2);
  } break;
  case WifiPhy::SYNC:
  case WifiPhy::TX:
    assert (false);
    break;
  }
  m_previousStateChangeTime = now;
  m_syncing = true;
  m_startSync = now;
  m_endSync = now + rxDuration;
  assert (IsStateSync ());
}
void
WifiPhy::SwitchFromSync (void)
{
  assert (IsStateSync ());
  assert (m_syncing);

  Time now = Simulator::Now ();
  m_stateLogger (m_startSync, now - m_startSync, 1);
  m_previousStateChangeTime = now;
  m_syncing = false;

  assert (IsStateIdle () || IsStateCcaBusy ());
}
void
WifiPhy::SwitchMaybeToCcaBusy (Time duration)
{
  Time now = Simulator::Now ();
  switch (GetState ()) {
  case WifiPhy::IDLE:
    LogPreviousIdleAndCcaBusyStates ();
  break;
  case WifiPhy::CCA_BUSY:
    break;
  case WifiPhy::SYNC:
    break;
  case WifiPhy::TX:
    break;
  }
  m_startCcaBusy = now;
  m_endCcaBusy = Max (m_endCcaBusy, now + duration);
}

void 
WifiPhy::AppendEvent (Ptr<RxEvent> event)
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
      m_events.erase (m_events.begin (), i);
    } 
  m_events.push_back (event);
}



/**
 * Stuff specific to the BER model here.
 */

double
WifiPhy::CalculateSnr (double signal, double noiseInterference, TransmissionMode *mode) const
{
  // thermal noise at 290K in J/s = W
  static const double BOLTZMANN = 1.3803e-23;
  double Nt = BOLTZMANN * 290.0 * mode->GetSignalSpread ();
  // receiver noise Floor (W)
  double noiseFloor = m_rxNoiseRatio * Nt;
  double noise = noiseFloor + noiseInterference;
  double snr = signal / noise;
  return snr;
}

double
WifiPhy::CalculateNoiseInterferenceW (Ptr<RxEvent> event, NiChanges *ni) const
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
      if (event->Overlaps ((*i)->GetStartTime ())) 
        {
          ni->push_back (NiChange ((*i)->GetStartTime (), (*i)->GetRxPowerW ()));
        }
      if (event->Overlaps ((*i)->GetEndTime ())) 
        {
          ni->push_back (NiChange ((*i)->GetEndTime (), -(*i)->GetRxPowerW ()));
        }
      if ((*i)->Overlaps (event->GetStartTime ())) 
        {
          noiseInterference += (*i)->GetRxPowerW ();
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
WifiPhy::CalculateChunkSuccessRate (double snir, Time duration, TransmissionMode *mode) const
{
  if (duration == NanoSeconds (0)) {
    return 1.0;
  }
  uint32_t rate = mode->GetRate ();
  uint64_t nbits = (uint64_t)(rate * duration.GetSeconds ());
  double csr = mode->GetChunkSuccessRate (snir, (uint32_t)nbits);
  return csr;
}

double 
WifiPhy::CalculatePer (Ptr<const RxEvent> event, NiChanges *ni) const
{  
  double psr = 1.0; /* Packet Success Rate */
  NiChangesI j = ni->begin ();
  Time previous = (*j).GetTime ();
  Time plcpHeaderStart = (*j).GetTime () + MicroSeconds (m_plcpPreambleDelayUs);
  Time plcpPayloadStart = plcpHeaderStart + 
    Seconds (m_plcpHeaderLength / GetMode (event->GetHeaderMode ())->GetDataRate ());
  double noiseInterferenceW = (*j).GetDelta ();
  double powerW = event->GetRxPowerW ();
  TransmissionMode *payloadMode = GetMode (event->GetPayloadMode ());
  TransmissionMode *headerMode = GetMode (event->GetHeaderMode ());

  j++;
  while (ni->end () != j) 
    {
      Time current = (*j).GetTime ();
      assert (current >= previous);
    
      if (previous >= plcpPayloadStart) 
        {
          psr *= CalculateChunkSuccessRate (CalculateSnr (powerW, 
                                                          noiseInterferenceW, payloadMode), 
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
              assert (current >= plcpHeaderStart);
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


void
WifiPhy::EndSync (Packet const packet, Ptr<RxEvent> event, uint8_t stuff)
{
  assert (IsStateSync ());
  assert (event->GetEndTime () == Simulator::Now ());

  NiChanges ni;
  double noiseInterferenceW = CalculateNoiseInterferenceW (event, &ni);
  double snr = CalculateSnr (event->GetRxPowerW (),
                             noiseInterferenceW,
                             GetMode (event->GetPayloadMode ()));
  
  /* calculate the SNIR at the start of the packet and accumulate
   * all SNIR changes in the snir vector.
   */
  double per = CalculatePer (event, &ni);
  TRACE ("mode="<<((uint32_t)event->GetPayloadMode ())<<
         ", ber="<<(1-GetMode (event->GetPayloadMode ())->GetChunkSuccessRate (snr, 1))<<
         ", snr="<<snr<<", per="<<per<<", size="<<packet.GetSize ());
  
  if (m_random.GetValue () > per) 
    {
      m_endSyncLogger (true);
      NotifySyncEndOk ();
      SwitchFromSync ();
      m_syncOkCallback (packet, snr, event->GetPayloadMode (), stuff);
    } 
  else 
    {
      /* failure. */
      m_endSyncLogger (false);
      NotifySyncEndError ();
      SwitchFromSync ();
      m_syncErrorCallback (packet, snr);
    }
  event->Unref ();
}




}; // namespace ns3
