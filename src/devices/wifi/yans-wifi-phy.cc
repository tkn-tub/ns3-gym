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
#include "error-rate-model.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/random-variable.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/pointer.h"
#include "ns3/net-device.h"
#include "ns3/trace-source-accessor.h"
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("YansWifiPhy");

namespace ns3 {

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
    .AddAttribute ("CcaMode1Threshold",
                   "The energy of a received signal should be higher than "
                   "this threshold (dbm) to allow the PHY layer to declare CCA BUSY state",
                   DoubleValue (-140.0),
                   MakeDoubleAccessor (&YansWifiPhy::SetCcaMode1Threshold,
                                       &YansWifiPhy::GetCcaMode1Threshold),
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
  m_modes.clear ();
  m_device = 0;
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
  m_interference.SetNoiseFloorW (DbToRatio (db));
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
void 
YansWifiPhy::SetCcaMode1Threshold (double threshold)
{
  NS_LOG_FUNCTION (this << threshold);
  m_ccaMode1ThresholdW = DbmToW (threshold);
}
void 
YansWifiPhy::SetErrorRateModel (Ptr<ErrorRateModel> rate)
{
  m_interference.SetErrorRateModel (rate);
}
void 
YansWifiPhy::SetDevice (Ptr<Object> device)
{
  m_device = device;
}
void 
YansWifiPhy::SetMobility (Ptr<Object> mobility)
{
  m_mobility = mobility;
}

double 
YansWifiPhy::GetRxNoise (void) const
{
  return RatioToDb (m_interference.GetNoiseFloorW ());
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

double 
YansWifiPhy::GetCcaMode1Threshold (void) const
{
  return WToDbm (m_ccaMode1ThresholdW);
}

Ptr<ErrorRateModel> 
YansWifiPhy::GetErrorRateModel (void) const
{
  return m_interference.GetErrorRateModel ();
}
Ptr<Object> 
YansWifiPhy::GetDevice (void) const
{
  return m_device;
}
Ptr<Object> 
YansWifiPhy::GetMobility (void)
{
  return m_mobility;
}

double 
YansWifiPhy::CalculateSnr (WifiMode txMode, double ber) const
{
  return m_interference.GetErrorRateModel ()->CalculateSnr (txMode, ber);
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
  m_channel->Add (this);
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

  Ptr<InterferenceHelper::Event> event;
  event = m_interference.Add (packet->GetSize (), 
                              txMode,
                              preamble,
                              rxDuration,
                              rxPowerW);

  switch (m_state->GetState ()) {
  case YansWifiPhy::SYNC:
    NS_LOG_DEBUG ("drop packet because already in Sync (power="<<
                  rxPowerW<<"W)");
    NotifyRxDrop (packet);
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
    NotifyRxDrop (packet);
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
        NotifyRxBegin (packet);
        m_endSyncEvent = Simulator::Schedule (rxDuration, &YansWifiPhy::EndSync, this, 
                                              packet,
                                              event);
      }
    else 
      {
        NS_LOG_DEBUG ("drop packet because signal power too Small ("<<
                      rxPowerW<<"<"<<m_edThresholdW<<")");
        NotifyRxDrop (packet);
        goto maybeCcaBusy;
      }
    break;
  }

  return;

 maybeCcaBusy:
  // We are here because we have received the first bit of a packet and we are
  // not going to be able to synchronize on it
  // In this model, CCA becomes busy when the aggregation of all signals as
  // tracked by the InterferenceHelper class is higher than the CcaBusyThreshold

  Time delayUntilCcaEnd = m_interference.GetEnergyDuration (m_ccaMode1ThresholdW);
  if (!delayUntilCcaEnd.IsZero ())
    {
      m_state->SwitchMaybeToCcaBusy (delayUntilCcaEnd);
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
  NotifyTxBegin (packet);
  NotifyPromiscSniff (packet);
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

void
YansWifiPhy::Configure80211a (void)
{
  NS_LOG_FUNCTION (this);
  m_interference.Configure80211aParameters ();
  m_modes.push_back (WifiPhy::Get6mba ());
  m_modes.push_back (WifiPhy::Get9mba ());
  m_modes.push_back (WifiPhy::Get12mba ());
  m_modes.push_back (WifiPhy::Get18mba ());
  m_modes.push_back (WifiPhy::Get24mba ());
  m_modes.push_back (WifiPhy::Get36mba ());
  m_modes.push_back (WifiPhy::Get48mba ());
  m_modes.push_back (WifiPhy::Get54mba ());
}

void
YansWifiPhy::ConfigureHolland (void)
{
  NS_LOG_FUNCTION (this);
  m_interference.Configure80211aParameters ();
  m_modes.push_back (WifiPhy::Get6mba ());
  m_modes.push_back (WifiPhy::Get12mba ());
  m_modes.push_back (WifiPhy::Get18mba ());
  m_modes.push_back (WifiPhy::Get36mba ());
  m_modes.push_back (WifiPhy::Get54mba ());
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
YansWifiPhy::CalculateTxDuration (uint32_t size, WifiMode payloadMode, enum WifiPreamble preamble) const
{
  return m_interference.CalculateTxDuration (size, payloadMode, preamble);
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

double 
YansWifiPhy::GetPowerDbm (uint8_t power) const
{
  NS_ASSERT (m_txPowerBaseDbm <= m_txPowerEndDbm);
  NS_ASSERT (m_nTxPower > 0);
  double dbm = m_txPowerBaseDbm + power * (m_txPowerEndDbm - m_txPowerBaseDbm) / m_nTxPower;
  return dbm;
}

void
YansWifiPhy::EndSync (Ptr<Packet> packet, Ptr<InterferenceHelper::Event> event)
{
  NS_LOG_FUNCTION (this << packet << event);
  NS_ASSERT (IsStateSync ());
  NS_ASSERT (event->GetEndTime () == Simulator::Now ());

  struct InterferenceHelper::SnrPer snrPer;
  snrPer = m_interference.CalculateSnrPer (event);

  NS_LOG_DEBUG ("mode="<<(event->GetPayloadMode ().GetDataRate ())<<
                ", snr="<<snrPer.snr<<", per="<<snrPer.per<<", size="<<packet->GetSize ());
  
  if (m_random.GetValue () > snrPer.per) 
    {
      NotifyRxEnd (packet);
      NotifyPromiscSniff (packet);
      m_state->SwitchFromSyncEndOk (packet, snrPer.snr, event->GetPayloadMode (), event->GetPreambleType ());
    } 
  else 
    {
      /* failure. */
      NotifyRxDrop (packet);
      m_state->SwitchFromSyncEndError (packet, snrPer.snr);
    }
}

} // namespace ns3
