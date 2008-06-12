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

#include "yans-wifi-phy.h"
#include "yans-wifi-channel.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "wifi-phy-state-helper.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/random-variable.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/pointer.h"
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("YansWifiPhy");

namespace ns3 {

/****************************************************************
 *       Phy event class
 ****************************************************************/

class RxEvent
{
public:
  RxEvent (uint32_t size, WifiMode payloadMode, 
           enum WifiPreamble preamble,
           Time duration, double rxPower)
    : m_size (size),
      m_payloadMode (payloadMode),
      m_preamble (preamble),
      m_startTime (Simulator::Now ()),
      m_endTime (m_startTime + duration),
      m_rxPowerW (rxPower),
      m_refCount (1)
  {}
  ~RxEvent ()
  {
    NS_ASSERT (m_refCount == 0);
  }
  
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
  enum WifiPreamble GetPreambleType (void) const {
    return m_preamble;
  }

private:
  uint32_t m_size;
  WifiMode m_payloadMode;
  enum WifiPreamble m_preamble;
  Time m_startTime;
  Time m_endTime;
  double m_rxPowerW;
  mutable int m_refCount;
};


/****************************************************************
 *       Class which records SNIR change events for a 
 *       short period of time.
 ****************************************************************/

YansWifiPhy::NiChange::NiChange (Time time, double delta)
  : m_time (time), m_delta (delta) 
{}
Time
YansWifiPhy::NiChange::GetTime (void) const
{
  return m_time;
}
double 
YansWifiPhy::NiChange::GetDelta (void) const
{
  return m_delta;
}
bool 
YansWifiPhy::NiChange::operator < (YansWifiPhy::NiChange const &o) const
{
  return (m_time < o.m_time)?true:false;
}



/****************************************************************
 *       The actual YansWifiPhy class
 ****************************************************************/

NS_OBJECT_ENSURE_REGISTERED (YansWifiPhy);

TypeId 
YansWifiPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::YansWifiPhy")
    .SetParent<WifiPhy> ()
    .AddConstructor<YansWifiPhy> ()
    .AddAttribute ("EnergyDetectionThreshold",
                   "The energy of a received signal should be higher than "
                   "this threshold (dbm) to allow the PHY layer to detect the signal.",
                   DoubleValue (-140.0),
                   MakeDoubleAccessor (&YansWifiPhy::SetEdThreshold,
                                       &YansWifiPhy::GetEdThreshold),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxGain",
                   "Transmission gain (dB).",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&YansWifiPhy::SetTxGain,
                                       &YansWifiPhy::GetTxGain),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGain",
                   "Reception gain (dB).",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&YansWifiPhy::SetRxGain,
                                       &YansWifiPhy::GetRxGain),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerLevels",
                   "Number of transmission power levels available between "
                   "TxPowerBase and TxPowerEnd included.",
                   UintegerValue (1),
                   MakeUintegerAccessor (&YansWifiPhy::m_nTxPower),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TxPowerEnd",
                   "Maximum available transmission level (dbm).",
                   DoubleValue (16.0206),
                   MakeDoubleAccessor (&YansWifiPhy::SetTxPowerEnd, 
                                       &YansWifiPhy::GetTxPowerEnd),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerStart",
                   "Minimum available transmission level (dbm).",
                   DoubleValue (16.0206),
                   MakeDoubleAccessor (&YansWifiPhy::SetTxPowerStart, 
                                       &YansWifiPhy::GetTxPowerStart),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxNoise",
                   "Ratio of energy lost by receiver (dB).",
                   DoubleValue (7),
                   MakeDoubleAccessor (&YansWifiPhy::SetRxNoise,
                                       &YansWifiPhy::GetRxNoise),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Standard", "The standard chosen configures a set of transmission modes"
                   " and some PHY-specific constants.",
                   EnumValue (WIFI_PHY_STANDARD_80211a),
                   MakeEnumAccessor (&YansWifiPhy::SetStandard),
                   MakeEnumChecker (WIFI_PHY_STANDARD_80211a, "802.11a",
                                    WIFI_PHY_STANDARD_holland, "holland"))
    .AddAttribute ("State", "The state of the PHY layer",
                   PointerValue (),
                   MakePointerAccessor (&YansWifiPhy::m_state),
                   MakePointerChecker<WifiPhyStateHelper> ())
    ;
  return tid;
}

YansWifiPhy::YansWifiPhy ()
 : m_endSyncEvent (),
   m_random (0.0, 1.0)
{
  NS_LOG_FUNCTION (this);
  m_state = CreateObject<WifiPhyStateHelper> ();
}

YansWifiPhy::~YansWifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

void
YansWifiPhy::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_channel = 0;
  m_events.clear ();
  m_modes.clear ();
}

void
YansWifiPhy::SetStandard (enum WifiPhyStandard standard)
{
  NS_LOG_FUNCTION (this << standard);
  m_standard = standard;
  switch (standard) {
  case WIFI_PHY_STANDARD_80211a:
    Configure80211a ();
    break;
  case WIFI_PHY_STANDARD_holland:
    ConfigureHolland ();
    break;
  default:
    NS_ASSERT (false);
    break;
  }
}


void 
YansWifiPhy::SetRxNoise (double db)
{
  NS_LOG_FUNCTION (this << db);
  m_rxNoiseRatio = DbToRatio (db);
}
void 
YansWifiPhy::SetTxPowerStart (double start)
{
  NS_LOG_FUNCTION (this << start);
  m_txPowerBaseDbm = start;
}
void 
YansWifiPhy::SetTxPowerEnd (double end)
{
  NS_LOG_FUNCTION (this << end);
  m_txPowerEndDbm = end;
}
void 
YansWifiPhy::SetNTxPower (uint32_t n)
{
  NS_LOG_FUNCTION (this << n);
  m_nTxPower = n;
}
void 
YansWifiPhy::SetTxGain (double gain)
{
  NS_LOG_FUNCTION (this << gain);
  m_txGainDb = gain;
}
void 
YansWifiPhy::SetRxGain (double gain)
{
  NS_LOG_FUNCTION (this << gain);
  m_rxGainDb = gain;
}
void 
YansWifiPhy::SetEdThreshold (double threshold)
{
  NS_LOG_FUNCTION (this << threshold);
  m_edThresholdW = DbmToW (threshold);
}
double 
YansWifiPhy::GetRxNoise (void) const
{
  return RatioToDb (m_rxNoiseRatio);
}
double 
YansWifiPhy::GetTxPowerStart (void) const
{
  return m_txPowerBaseDbm;
}
double 
YansWifiPhy::GetTxPowerEnd (void) const
{
  return m_txPowerEndDbm;
}
double 
YansWifiPhy::GetTxGain (void) const
{
  return m_txGainDb;
}
double 
YansWifiPhy::GetRxGain (void) const
{
  return m_rxGainDb;
}

double 
YansWifiPhy::GetEdThreshold (void) const
{
  return WToDbm (m_edThresholdW);
}

Ptr<WifiChannel> 
YansWifiPhy::GetChannel (void) const
{
  return m_channel;
}

void 
YansWifiPhy::SetChannel (Ptr<YansWifiChannel> channel)
{
  m_channel = channel;
}

void 
YansWifiPhy::SetReceiveOkCallback (SyncOkCallback callback)
{
  m_state->SetReceiveOkCallback (callback);
}
void 
YansWifiPhy::SetReceiveErrorCallback (SyncErrorCallback callback)
{
  m_state->SetReceiveErrorCallback (callback);
}
void 
YansWifiPhy::StartReceivePacket (Ptr<Packet> packet, 
                             double rxPowerDbm,
                             WifiMode txMode,
                             enum WifiPreamble preamble)
{
  NS_LOG_FUNCTION (this << packet << rxPowerDbm << txMode << preamble);
  rxPowerDbm += m_rxGainDb;
  double rxPowerW = DbmToW (rxPowerDbm);
  Time rxDuration = CalculateTxDuration (packet->GetSize (), txMode, preamble);
  Time endRx = Simulator::Now () + rxDuration;

  Ptr<RxEvent> event = Create<RxEvent> (packet->GetSize (), 
                                        txMode,
                                        preamble,
                                        rxDuration,
                                        rxPowerW);
  AppendEvent (event);

  switch (m_state->GetState ()) {
  case YansWifiPhy::SYNC:
    NS_LOG_DEBUG ("drop packet because already in Sync (power="<<
                  rxPowerW<<"W)");
    if (endRx > Simulator::Now () + m_state->GetDelayUntilIdle ()) 
      {
        // that packet will be noise _after_ the reception of the
        // currently-received packet.
        goto maybeCcaBusy;
      }
    break;
  case YansWifiPhy::TX:
    NS_LOG_DEBUG ("drop packet because already in Tx (power="<<
                  rxPowerW<<"W)");
    if (endRx > Simulator::Now () + m_state->GetDelayUntilIdle ()) 
      {
        // that packet will be noise _after_ the transmission of the
        // currently-transmitted packet.
        goto maybeCcaBusy;
      }
    break;
  case YansWifiPhy::CCA_BUSY:
  case YansWifiPhy::IDLE:
    if (rxPowerW > m_edThresholdW) 
      {
        NS_LOG_DEBUG ("sync (power="<<rxPowerW<<"W)");
        // sync to signal
        m_state->SwitchToSync (rxDuration);
        NS_ASSERT (m_endSyncEvent.IsExpired ());
        m_endSyncEvent = Simulator::Schedule (rxDuration, &YansWifiPhy::EndSync, this, 
                                              packet,
                                              event);
      }
    else 
      {
        NS_LOG_DEBUG ("drop packet because signal power too Small ("<<
                      rxPowerW<<"<"<<m_edThresholdW<<")");
        goto maybeCcaBusy;
      }
    break;
  }

  return;

 maybeCcaBusy:

  if (rxPowerW > m_edThresholdW) 
    {
      m_state->SwitchMaybeToCcaBusy (rxDuration);
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
          m_state->SwitchMaybeToCcaBusy (delta);
        }
    }

}
void 
YansWifiPhy::SendPacket (Ptr<const Packet> packet, WifiMode txMode, WifiPreamble preamble, uint8_t txPower)
{
  NS_LOG_FUNCTION (this << packet << txMode << preamble << (uint32_t)txPower);
  /* Transmission can happen if:
   *  - we are syncing on a packet. It is the responsability of the
   *    MAC layer to avoid doing this but the PHY does nothing to 
   *    prevent it.
   *  - we are idle
   */
  NS_ASSERT (!m_state->IsStateTx ());

  Time txDuration = CalculateTxDuration (packet->GetSize (), txMode, preamble);
  if (m_state->IsStateSync ())
    {
      m_endSyncEvent.Cancel ();
    }
  m_state->SwitchToTx (txDuration, packet, txMode, preamble, txPower);
  m_channel->Send (this, packet, GetPowerDbm (txPower) + m_txGainDb, txMode, preamble);
}

uint32_t 
YansWifiPhy::GetNModes (void) const
{
  return m_modes.size ();
}
WifiMode 
YansWifiPhy::GetMode (uint32_t mode) const
{
  return m_modes[mode];
}
uint32_t 
YansWifiPhy::GetNTxPower (void) const
{
  return m_nTxPower;
}

double 
YansWifiPhy::CalculateSnr (WifiMode txMode, double ber) const
{
  double low, high, precision;
  low = 1e-25;
  high = 1e25;
  precision = 1e-12;
  while (high - low > precision) 
    {
      NS_ASSERT (high >= low);
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
YansWifiPhy::Configure80211aParameters (void)
{
  NS_LOG_FUNCTION (this);
  m_plcpLongPreambleDelayUs = 16;
  m_plcpShortPreambleDelayUs = 16;
  m_longPlcpHeaderMode = g_6mba;
  m_shortPlcpHeaderMode = g_6mba;
  m_plcpHeaderLength = 4 + 1 + 12 + 1 + 6;
  /* 4095 bytes at a 6Mb/s rate with a 1/2 coding rate. */
  m_maxPacketDuration = CalculateTxDuration (4095, g_6mba, WIFI_PREAMBLE_LONG);
}

void
YansWifiPhy::PrintModes (void) const
{
#if 0
  for (double db = -10; db < 30; db+= 0.5) {
    double snr = DbToRatio (db);
    std::cout <<snr<<" ";
    for (uint8_t i = 0; i < GetNModes (); i++) {
      WifiMode mode = GetMode (i);
      double ber = 1-GetChunkSuccessRate (mode,snr, 2000*8);
      std::cout <<ber<< " ";
    }
    std::cout << std::endl;
  }
#endif
}

void
YansWifiPhy::Configure80211a (void)
{
  NS_LOG_FUNCTION (this);
  Configure80211aParameters ();
  m_modes.push_back (g_6mba);
  m_modes.push_back (g_9mba);
  m_modes.push_back (g_12mba);
  m_modes.push_back (g_18mba);
  m_modes.push_back (g_24mba);
  m_modes.push_back (g_36mba);
  m_modes.push_back (g_48mba);
  m_modes.push_back (g_54mba);

  PrintModes ();
}

void
YansWifiPhy::ConfigureHolland (void)
{
  NS_LOG_FUNCTION (this);
  Configure80211aParameters ();
  m_modes.push_back (g_6mba);
  m_modes.push_back (g_12mba);
  m_modes.push_back (g_18mba);
  m_modes.push_back (g_36mba);
  m_modes.push_back (g_54mba);

  PrintModes ();
}

void 
YansWifiPhy::RegisterListener (WifiPhyListener *listener)
{
  m_state->RegisterListener (listener);
}

bool 
YansWifiPhy::IsStateCcaBusy (void)
{
  return m_state->IsStateCcaBusy ();
}

bool 
YansWifiPhy::IsStateIdle (void)
{
  return m_state->IsStateIdle ();
}
bool 
YansWifiPhy::IsStateBusy (void)
{
  return m_state->IsStateBusy ();
}
bool 
YansWifiPhy::IsStateSync (void)
{
  return m_state->IsStateSync ();
}
bool 
YansWifiPhy::IsStateTx (void)
{
  return m_state->IsStateTx ();
}

Time
YansWifiPhy::GetStateDuration (void)
{
  return m_state->GetStateDuration ();
}
Time
YansWifiPhy::GetDelayUntilIdle (void)
{
  return m_state->GetDelayUntilIdle ();
}

Time 
YansWifiPhy::GetLastRxStartTime (void) const
{
  return m_state->GetLastRxStartTime ();
}


Time
YansWifiPhy::CalculateTxDuration (uint32_t size, WifiMode payloadMode, WifiPreamble preamble) const
{
  uint64_t delay = 0;
  switch (m_standard) {
  case WIFI_PHY_STANDARD_80211a:
  case WIFI_PHY_STANDARD_holland: {
    delay += m_plcpLongPreambleDelayUs;
    // symbol duration is 4us
    delay += 4;
    delay += lrint (ceil ((size * 8.0 + 16.0 + 6.0) / payloadMode.GetDataRate () / 4e-6) * 4);
  } break;
  default:
    // quiet compiler.
    NS_ASSERT (false);
    break;
  }
  return MicroSeconds (delay);
}


double 
YansWifiPhy::DbToRatio (double dB) const
{
  double ratio = pow(10.0,dB/10.0);
  return ratio;
}

double 
YansWifiPhy::DbmToW (double dBm) const
{
  double mW = pow(10.0,dBm/10.0);
  return mW / 1000.0;
}

double
YansWifiPhy::WToDbm (double w) const
{
  return 10.0 * log10(w * 1000.0);
}

double
YansWifiPhy::RatioToDb (double ratio) const
{
  return 10.0 * log10(ratio);
}

double
YansWifiPhy::GetEdThresholdW (void) const
{
  return m_edThresholdW;
}

Time
YansWifiPhy::GetMaxPacketDuration (void) const
{
  return m_maxPacketDuration;
}

double 
YansWifiPhy::GetPowerDbm (uint8_t power) const
{
  NS_ASSERT (m_txPowerBaseDbm <= m_txPowerEndDbm);
  NS_ASSERT (m_nTxPower > 0);
  double dbm = m_txPowerBaseDbm + power * (m_txPowerEndDbm - m_txPowerBaseDbm) / m_nTxPower;
  return dbm;
}


void 
YansWifiPhy::AppendEvent (Ptr<RxEvent> event)
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
YansWifiPhy::Log2 (double val) const
{
  return log(val) / log(2.0);
}
double 
YansWifiPhy::GetBpskBer (double snr, uint32_t signalSpread, uint32_t phyRate) const
{
  double EbNo = snr * signalSpread / phyRate;
  double z = sqrt(EbNo);
  double ber = 0.5 * erfc(z);
  NS_LOG_INFO ("bpsk snr="<<snr<<" ber="<<ber);
  return ber;
}
double 
YansWifiPhy::GetQamBer (double snr, unsigned int m, uint32_t signalSpread, uint32_t phyRate) const
{
  double EbNo = snr * signalSpread / phyRate;
  double z = sqrt ((1.5 * Log2 (m) * EbNo) / (m - 1.0));
  double z1 = ((1.0 - 1.0 / sqrt (m)) * erfc (z)) ;
  double z2 = 1 - pow ((1-z1), 2.0);
  double ber = z2 / Log2 (m);
  NS_LOG_INFO ("Qam m="<<m<<" rate=" << phyRate << " snr="<<snr<<" ber="<<ber);
  return ber;
}
uint32_t
YansWifiPhy::Factorial (uint32_t k) const
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
YansWifiPhy::Binomial (uint32_t k, double p, uint32_t n) const
{
  double retval = Factorial (n) / (Factorial (k) * Factorial (n-k)) * pow (p, k) * pow (1-p, n-k);
  return retval;
}
double 
YansWifiPhy::CalculatePdOdd (double ber, unsigned int d) const
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
YansWifiPhy::CalculatePdEven (double ber, unsigned int d) const
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
YansWifiPhy::CalculatePd (double ber, unsigned int d) const
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
YansWifiPhy::GetFecBpskBer (double snr, double nbits, 
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
  pmu = std::min (pmu, 1.0);
  double pms = pow (1 - pmu, nbits);
  return pms;
}

double
YansWifiPhy::GetFecQamBer (double snr, uint32_t nbits, 
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
  pmu = std::min (pmu, 1.0);
  double pms = pow (1 - pmu, nbits);
  return pms;
}

double 
YansWifiPhy::GetChunkSuccessRate (WifiMode mode, double snr, uint32_t nbits) const
{
  if (mode.GetUid () == g_6mba.GetUid ())
    {
      return GetFecBpskBer (snr, 
                            nbits,
                            mode.GetBandwidth (), // signal spread
                            mode.GetPhyRate (), // phy rate
                            10, // dFree
                            11 // adFree
                            );      
    }
  else if (mode.GetUid () == g_9mba.GetUid ())
    {
      return GetFecBpskBer (snr, 
                            nbits,
                            mode.GetBandwidth (), // signal spread
                            mode.GetPhyRate (), // phy rate
                            5, // dFree
                            8 // adFree
                            );
    }
  else if (mode.GetUid () == g_12mba.GetUid ())
    {
      return GetFecQamBer (snr, 
                           nbits,
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
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
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
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
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
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
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
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
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
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
                           mode.GetBandwidth (), // signal spread
                           mode.GetPhyRate (), // phy rate
                           64, // m
                           5,  // dFree
                           8,  // adFree
                           31  // adFreePlusOne
                           );
    }
  return 0;
}

double
YansWifiPhy::CalculateSnr (double signal, double noiseInterference, WifiMode mode) const
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
YansWifiPhy::CalculateNoiseInterferenceW (Ptr<RxEvent> event, NiChanges *ni) const
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
YansWifiPhy::CalculateChunkSuccessRate (double snir, Time duration, WifiMode mode) const
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
YansWifiPhy::CalculatePer (Ptr<const RxEvent> event, NiChanges *ni) const
{  
  double psr = 1.0; /* Packet Success Rate */
  NiChanges::iterator j = ni->begin ();
  Time previous = (*j).GetTime ();
  uint64_t plcpPreambleDelayUs;
  WifiMode payloadMode = event->GetPayloadMode ();
  WifiMode headerMode;
  switch (event->GetPreambleType ()) {
  case WIFI_PREAMBLE_LONG:
    plcpPreambleDelayUs = m_plcpLongPreambleDelayUs;
    headerMode = m_longPlcpHeaderMode;
    break;
  case WIFI_PREAMBLE_SHORT:
    plcpPreambleDelayUs = m_plcpShortPreambleDelayUs;
    headerMode = m_shortPlcpHeaderMode;
    break;
  default:
    NS_ASSERT (false);
    // only to quiet compiler. Really stupid.
    plcpPreambleDelayUs = 0;
    headerMode = m_shortPlcpHeaderMode;
    break;
  }
  Time plcpHeaderStart = (*j).GetTime () + MicroSeconds (plcpPreambleDelayUs);
  Time plcpPayloadStart = plcpHeaderStart + 
    Seconds ((m_plcpHeaderLength + 0.0) / headerMode.GetDataRate ());
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


void
YansWifiPhy::EndSync (Ptr<Packet> packet, Ptr<RxEvent> event)
{
  NS_LOG_FUNCTION (this << packet << event);
  NS_ASSERT (IsStateSync ());
  NS_ASSERT (event->GetEndTime () == Simulator::Now ());

  NiChanges ni;
  double noiseInterferenceW = CalculateNoiseInterferenceW (event, &ni);
  double snr = CalculateSnr (event->GetRxPowerW (),
                             noiseInterferenceW,
                             event->GetPayloadMode ());
  
  /* calculate the SNIR at the start of the packet and accumulate
   * all SNIR changes in the snir vector.
   */
  double per = CalculatePer (event, &ni);
  NS_LOG_DEBUG ("mode="<<(event->GetPayloadMode ().GetDataRate ())<<
                ", ber="<<(1-GetChunkSuccessRate (event->GetPayloadMode (), snr, 1))<<
                ", snr="<<snr<<", per="<<per<<", size="<<packet->GetSize ());
  
  if (m_random.GetValue () > per) 
    {
      m_state->SwitchFromSyncEndOk (packet, snr, event->GetPayloadMode (), event->GetPreambleType ());
    } 
  else 
    {
      /* failure. */
      m_state->SwitchFromSyncEndError (packet, snr);
    }
}




} // namespace ns3
