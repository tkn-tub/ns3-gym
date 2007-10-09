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
#include "wifi-mode.h"
#include "wifi-channel.h"
#include "wifi-net-device.h"
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

  // Define all the WifiMode needed for 802.11a
static WifiMode g_6mba = WifiModeFactory::CreateBpsk ("wifia-6mbs",
                                                      true,
                                                      20000000, 6000000 * 1 / 2, 6000000);
static WifiMode g_9mba = WifiModeFactory::CreateBpsk ("wifia-9mbs",
                                                      false,
                                                      20000000, 9000000 * 3 / 4, 9000000);
static WifiMode g_12mba = WifiModeFactory::CreateBpsk ("wifia-12mbs",
                                                       true,
                                                       20000000, 12000000 * 1 / 2, 12000000);
static WifiMode g_18mba = WifiModeFactory::CreateBpsk ("wifia-18mbs",
                                                       false,
                                                       20000000, 18000000 * 3 / 4, 18000000);
static WifiMode g_24mba = WifiModeFactory::CreateBpsk ("wifia-24mbs",
                                                       true,
                                                       20000000, 24000000 * 1 / 2, 24000000);
static WifiMode g_36mba = WifiModeFactory::CreateBpsk ("wifia-36mbs",
                                                       false,
                                                       20000000, 36000000 * 3 / 4, 36000000);
static WifiMode g_48mba = WifiModeFactory::CreateBpsk ("wifia-48mbs",
                                                       false,
                                                       20000000, 48000000 * 2 / 3, 48000000);
static WifiMode g_54mba = WifiModeFactory::CreateBpsk ("wifia-54mbs",
                                                       false,
                                                       20000000, 54000000 * 3 / 4, 54000000);


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
  RxEvent (uint32_t size, WifiMode payloadMode, 
           WifiMode headerMode,
           Time duration, double rxPower)
    : m_size (size),
      m_payloadMode (payloadMode),
      m_headerMode (headerMode),
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
  WifiMode GetPayloadMode (void) const {
    return m_payloadMode;
  }
  WifiMode GetHeaderMode (void) const {
    return m_headerMode;
  }

private:
  uint32_t m_size;
  WifiMode m_payloadMode;
  WifiMode m_headerMode;
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

WifiPhy::WifiPhy (Ptr<WifiNetDevice> device)
  : m_edThresholdW (DbmToW (-140)),
    m_txGainDbm (1.0),
    m_rxGainDbm (1.0),
    m_rxNoiseRatio (DbToRatio (7)),
    m_txPowerBaseDbm (16.0206),
    m_txPowerEndDbm (16.0206),
    m_nTxPower (1),
    m_syncing (false),
    m_endTx (Seconds (0)),
    m_endSync (Seconds (0)),
    m_endCcaBusy (Seconds (0)),
    m_startTx (Seconds (0)),
    m_startSync (Seconds (0)),
    m_startCcaBusy (Seconds (0)),
    m_previousStateChangeTime (Seconds (0)),
    m_device (device),
    m_endSyncEvent (),
    m_random (0.0, 1.0)
{}

WifiPhy::~WifiPhy ()
{
  m_events.clear ();
  m_modes.clear ();
}



void 
WifiPhy::SetChannel (Ptr<WifiChannel> channel)
{
  m_channel = channel;
  m_channel->Add (m_device, MakeCallback (&WifiPhy::ReceivePacket, this));
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
                        WifiMode txMode,
                        WifiMode headerMode)
{
  Time rxDuration = CalculateTxDuration (packet.GetSize (), txMode, headerMode);
  Time endRx = Simulator::Now () + rxDuration;
  m_startRxLogger (rxDuration, rxPowerW);

  Ptr<RxEvent> event = Create<RxEvent> (packet.GetSize (), 
                                        txMode,
                                        headerMode,
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
                                              event);
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
      for (NiChanges::const_iterator i = ni.begin (); i != ni.end (); i++) 
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
WifiPhy::SendPacket (Packet const packet, WifiMode txMode, WifiMode headerMode, uint8_t txPower)
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

  Time txDuration = CalculateTxDuration (packet.GetSize (), txMode, headerMode);
  m_startTxLogger (txDuration, txMode.GetPhyRate (), GetPowerDbm (txPower));
  NotifyTxStart (txDuration);
  SwitchToTx (txDuration);
  m_channel->Send (m_device, packet, GetPowerDbm (txPower) + m_txGainDbm, txMode, headerMode);
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
void 
WifiPhy::SetRxTxGainDbm (double rxGainDbm, double txGainDbm)
{
  m_rxGainDbm = rxGainDbm;
  m_txGainDbm = txGainDbm;
}
uint32_t 
WifiPhy::GetNModes (void) const
{
  return m_modes.size ();
}
WifiMode 
WifiPhy::GetMode (uint32_t mode) const
{
  return m_modes[mode];
}
uint32_t 
WifiPhy::GetNTxpower (void) const
{
  return m_nTxPower;
}

double 
WifiPhy::CalculateSnr (WifiMode txMode, double ber) const
{
  double low, high, precision;
  low = 1e-25;
  high = 1e25;
  precision = 1e-12;
  while (high - low > precision) 
    {
      assert (high >= low);
      double middle = low + (high - low) / 2;
      if ((1 - GetChunkSuccessRate (txMode, middle, 1)) > ber) 
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

  m_modes.push_back (g_6mba);
  m_modes.push_back (g_9mba);
  m_modes.push_back (g_18mba);
  m_modes.push_back (g_24mba);
  m_modes.push_back (g_36mba);
  m_modes.push_back (g_48mba);
  m_modes.push_back (g_54mba);

#ifdef PHY80211_DEBUG
  for (double db = 0; db < 30; db+= 0.5) {
    std::cout <<db<<" ";
    for (uint8_t i = 0; i < GetNModes (); i++) {
      WifiMode mode = GetMode (i);
      double ber = 1-GetChunkSuccessRate (mode, DbToRatio (db), 1);
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
WifiPhy::CalculateTxDuration (uint32_t size, WifiMode payloadMode, WifiMode headerMode) const
{
  uint64_t delay = m_plcpPreambleDelayUs;
  delay += m_plcpHeaderLength * 1000000 / headerMode.GetDataRate ();
  uint64_t nbits = size * 8;
  delay += nbits * 1000000 / payloadMode.GetDataRate ();
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
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyTxStart (duration);
  }
}
void 
WifiPhy::NotifySyncStart (Time duration)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxStart (duration);
  }
}
void 
WifiPhy::NotifySyncEndOk (void)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxEndOk ();
  }
}
void 
WifiPhy::NotifySyncEndError (void)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
    (*i)->NotifyRxEndError ();
  }
}
void 
WifiPhy::NotifyCcaBusyStart (Time duration)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++) {
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
WifiPhy::Log2 (double val) const
{
  return log(val) / log(2.0);
}
double 
WifiPhy::GetBpskBer (double snr, uint32_t signalSpread, uint32_t phyRate) const
{
  double EbNo = snr * signalSpread / phyRate;
  double z = sqrt(EbNo);
  double ber = 0.5 * erfc(z);
  return ber;
}
double 
WifiPhy::GetQamBer (double snr, unsigned int m, uint32_t signalSpread, uint32_t phyRate) const
{
  double EbNo = snr * signalSpread / phyRate;
  double z = sqrt ((1.5 * Log2 (m) * EbNo) / (m - 1.0));
  double z1 = ((1.0 - 1.0 / sqrt (m)) * erfc (z)) ;
  double z2 = 1 - pow ((1-z1), 2.0);
  double ber = z2 / Log2 (m);
  return ber;
}
uint32_t
WifiPhy::Factorial (uint32_t k) const
{
  uint32_t fact = 1;
  while (k > 0) 
    {
      fact *= k;
      k--;
    }
  return fact;
}
double 
WifiPhy::Binomial (uint32_t k, double p, uint32_t n) const
{
  double retval = Factorial (n) / (Factorial (k) * Factorial (n-k)) * pow (p, k) * pow (1-p, n-k);
  return retval;
}
double 
WifiPhy::CalculatePdOdd (double ber, unsigned int d) const
{
  NS_ASSERT ((d % 2) == 1);
  unsigned int dstart = (d + 1) / 2;
  unsigned int dend = d;
  double pd = 0;

  for (unsigned int i = dstart; i < dend; i++) 
    {
      pd += Binomial (i, ber, d);
    }
  return pd;
}
double 
WifiPhy::CalculatePdEven (double ber, unsigned int d) const
{
  NS_ASSERT ((d % 2) == 0);
  unsigned int dstart = d / 2 + 1;
  unsigned int dend = d;
  double pd = 0;

  for (unsigned int i = dstart; i < dend; i++)
    {
      pd +=  Binomial (i, ber, d);
    }
  pd += 0.5 * Binomial (d / 2, ber, d);

  return pd;
}

double 
WifiPhy::CalculatePd (double ber, unsigned int d) const
{
  double pd;
  if ((d % 2) == 0) 
    {
      pd = CalculatePdEven (ber, d);
    } 
  else 
    {
      pd = CalculatePdOdd (ber, d);
    }
  return pd;
}

double
WifiPhy::GetFecBpskBer (double snr, double nbits, 
                         uint32_t signalSpread, uint32_t phyRate,
                         uint32_t dFree, uint32_t adFree) const
{
  double ber = GetBpskBer (snr, signalSpread, phyRate);
  if (ber == 0.0) 
    {
      return 1.0;
    }
  double pd = CalculatePd (ber, dFree);
  double pmu = adFree * pd;
  if (pmu > 1.0) 
    {
      /**
       * If pmu is bigger than 1, then, this calculation is
       * giving us a useless bound. A better bound in this case
       * is 1 - ber which is necessarily bigger than the real 
       * success rate.
       */
      return ber;
    }
  double pms = pow (1 - pmu, nbits);
  return pms;
}

double
WifiPhy::GetFecQamBer (double snr, uint32_t nbits, 
                       uint32_t signalSpread,
                       uint32_t phyRate,
                       uint32_t m, uint32_t dFree,
                       uint32_t adFree, uint32_t adFreePlusOne) const
{
  double ber = GetQamBer (snr, m, signalSpread, phyRate);
  if (ber == 0.0) 
    {
      return 1.0;
    }
  /* first term */
  double pd = CalculatePd (ber, dFree);
  double pmu = adFree * pd;
  /* second term */
  pd = CalculatePd (ber, dFree + 1);
  pmu += adFreePlusOne * pd;
  if (pmu > 1.0) 
    {
      /**
       * If pmu is bigger than 1, then, this calculation is
       * giving us a useless bound. A better bound in this case
       * is 1 - ber which is necessarily bigger than the real 
       * success rate.
       */
      return ber;
    }

  double pms = pow (1 - pmu, nbits);
  return pms;
}

double 
WifiPhy::GetChunkSuccessRate (WifiMode mode, double snr, uint32_t nbits) const
{
  if (mode.GetUid () == g_6mba.GetUid ())
    {
      return GetFecBpskBer (snr, 
                            nbits,
                            20000000, // signal spread
                            6000000, // phy rate
                            10, // dFree
                            11 // adFree
                            );      
    }
  else if (mode.GetUid () == g_9mba.GetUid ())
    {
      return GetFecBpskBer (snr, 
                            nbits,
                            20000000, // signal spread
                            9000000, // phy rate
                            5, // dFree
                            8 // adFree
                            );
    }
  else if (mode.GetUid () == g_12mba.GetUid ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           20000000, // signal spread
                           12000000, // phy rate
                           4,  // m 
                           10, // dFree
                           11, // adFree
                           0   // adFreePlusOne
                           );
    }
  else if (mode.GetUid () == g_18mba.GetUid ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           20000000, // signal spread
                           18000000, // phy rate
                           4, // m
                           5, // dFree
                           8, // adFree
                           31 // adFreePlusOne
                           );
    }
  else if (mode.GetUid () == g_24mba.GetUid ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           20000000, // signal spread
                           24000000, // phy rate
                           16, // m
                           10, // dFree
                           11, // adFree
                           0   // adFreePlusOne
                           );
    }
  else if (mode.GetUid () == g_36mba.GetUid ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           20000000, // signal spread
                           36000000, // phy rate
                           16, // m
                           5,  // dFree
                           8,  // adFree
                           31  // adFreePlusOne
                           );
    }
  else if (mode.GetUid () == g_48mba.GetUid ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           20000000, // signal spread
                           48000000, // phy rate
                           64, // m
                           6,  // dFree
                           1,  // adFree
                           16  // adFreePlusOne
                           );
    }
  else if (mode.GetUid () == g_54mba.GetUid ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           20000000, // signal spread
                           54000000, // phy rate
                           64, // m
                           5,  // dFree
                           8,  // adFree
                           31  // adFreePlusOne
                           );
    }
  return 0;
}

double
WifiPhy::CalculateSnr (double signal, double noiseInterference, WifiMode mode) const
{
  // thermal noise at 290K in J/s = W
  static const double BOLTZMANN = 1.3803e-23;
  double Nt = BOLTZMANN * 290.0 * mode.GetBandwidth ();
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
WifiPhy::CalculateChunkSuccessRate (double snir, Time duration, WifiMode mode) const
{
  if (duration == NanoSeconds (0)) {
    return 1.0;
  }
  uint32_t rate = mode.GetPhyRate ();
  uint64_t nbits = (uint64_t)(rate * duration.GetSeconds ());
  double csr = GetChunkSuccessRate (mode, snir, (uint32_t)nbits);
  return csr;
}

double 
WifiPhy::CalculatePer (Ptr<const RxEvent> event, NiChanges *ni) const
{  
  double psr = 1.0; /* Packet Success Rate */
  NiChanges::iterator j = ni->begin ();
  Time previous = (*j).GetTime ();
  Time plcpHeaderStart = (*j).GetTime () + MicroSeconds (m_plcpPreambleDelayUs);
  Time plcpPayloadStart = plcpHeaderStart + 
    Seconds (m_plcpHeaderLength / event->GetHeaderMode ().GetDataRate ());
  double noiseInterferenceW = (*j).GetDelta ();
  double powerW = event->GetRxPowerW ();
  WifiMode payloadMode = event->GetPayloadMode ();
  WifiMode headerMode = event->GetHeaderMode ();

  j++;
  while (ni->end () != j) 
    {
      Time current = (*j).GetTime ();
      assert (current >= previous);
    
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
WifiPhy::EndSync (Packet const packet, Ptr<RxEvent> event)
{
  assert (IsStateSync ());
  assert (event->GetEndTime () == Simulator::Now ());

  NiChanges ni;
  double noiseInterferenceW = CalculateNoiseInterferenceW (event, &ni);
  double snr = CalculateSnr (event->GetRxPowerW (),
                             noiseInterferenceW,
                             event->GetPayloadMode ());
  
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
      m_syncOkCallback (packet, snr, event->GetPayloadMode (), event->GetHeaderMode ());
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
