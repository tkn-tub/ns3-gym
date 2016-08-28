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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Ghada Badawy <gbadawy@gmail.com>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 *
 * Ported from yans-wifi-phy.cc by several contributors starting
 * with Nicola Baldo and Dean Armstrong
 */

#include "spectrum-wifi-phy.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-value.h"
#include "ns3/wifi-spectrum-value-helper.h"
#include "wifi-phy-state-helper.h"
#include "ns3/simulator.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ampdu-tag.h"
#include "wifi-spectrum-signal-parameters.h"
#include "wifi-phy-tag.h"
#include "ns3/antenna-model.h"
#include <cmath>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SpectrumWifiPhy");

NS_OBJECT_ENSURE_REGISTERED (SpectrumWifiPhy);

TypeId
SpectrumWifiPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SpectrumWifiPhy")
    .SetParent<WifiPhy> ()
    .SetGroupName ("Wifi")
    .AddConstructor<SpectrumWifiPhy> ()
    .AddAttribute ("DisableWifiReception", "Prevent Wi-Fi frame sync from ever happening",
                   BooleanValue (false),
                   MakeBooleanAccessor (&SpectrumWifiPhy::m_disableWifiReception),
                   MakeBooleanChecker ())
    .AddTraceSource ("SignalArrival",
                     "Signal arrival",
                     MakeTraceSourceAccessor (&SpectrumWifiPhy::m_signalCb),
                     "ns3::SpectrumWifiPhy::SignalArrivalCallback")
  ;
  return tid;
}

SpectrumWifiPhy::SpectrumWifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

SpectrumWifiPhy::~SpectrumWifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

void
SpectrumWifiPhy::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_channel = 0;
  m_wifiSpectrumPhyInterface = 0;
}

void
SpectrumWifiPhy::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  WifiPhy::DoInitialize ();
  // This connection is deferred until frequency and channel width are set
  if (m_channel && m_wifiSpectrumPhyInterface)
    {
      m_channel->AddRx (m_wifiSpectrumPhyInterface);
    }
  else
    {
      NS_FATAL_ERROR ("SpectrumWifiPhy misses channel and WifiSpectrumPhyInterface objects at initialization time");
    }
}

bool
SpectrumWifiPhy::DoChannelSwitch (uint16_t nch)
{
  if (IsInitialized () == false)
    {
      //this is not channel switch, this is initialization
      NS_LOG_DEBUG ("start at channel " << nch);
      return true;
    }

  NS_ASSERT (!IsStateSwitching ());
  switch (m_state->GetState ())
    {
    case SpectrumWifiPhy::RX:
      NS_LOG_DEBUG ("drop packet because of channel switching while reception");
      m_endPlcpRxEvent.Cancel ();
      m_endRxEvent.Cancel ();
      goto switchChannel;
      break;
    case SpectrumWifiPhy::TX:
      NS_LOG_DEBUG ("channel switching postponed until end of current transmission");
      Simulator::Schedule (GetDelayUntilIdle (), &WifiPhy::SetChannelNumber, this, nch);
      break;
    case SpectrumWifiPhy::CCA_BUSY:
    case SpectrumWifiPhy::IDLE:
      goto switchChannel;
      break;
    case SpectrumWifiPhy::SLEEP:
      NS_LOG_DEBUG ("channel switching ignored in sleep mode");
      break;
    default:
      NS_ASSERT (false);
      break;
    }

  return false;

switchChannel:

  NS_LOG_DEBUG ("switching channel " << GetChannelNumber () << " -> " << nch);
  m_rxSpectrumModel = WifiSpectrumValueHelper::GetSpectrumModel (GetFrequency (), GetChannelWidth ());
  m_state->SwitchToChannelSwitching (GetChannelSwitchDelay ());
  m_interference.EraseEvents ();
  /*
   * Needed here to be able to correctly sensed the medium for the first
   * time after the switching. The actual switching is not performed until
   * after m_channelSwitchDelay. Packets received during the switching
   * state are added to the event list and are employed later to figure
   * out the state of the medium after the switching.
   */
  return true;
}

bool
SpectrumWifiPhy::DoFrequencySwitch (uint32_t frequency)
{
  if (IsInitialized () == false)
    {
      //this is not channel switch, this is initialization
      NS_LOG_DEBUG ("start at frequency " << frequency);
      return true;
    }

  NS_ASSERT (!IsStateSwitching ());
  switch (m_state->GetState ())
    {
    case SpectrumWifiPhy::RX:
      NS_LOG_DEBUG ("drop packet because of channel/frequency switching while reception");
      m_endPlcpRxEvent.Cancel ();
      m_endRxEvent.Cancel ();
      goto switchFrequency;
      break;
    case SpectrumWifiPhy::TX:
      NS_LOG_DEBUG ("channel/frequency switching postponed until end of current transmission");
      Simulator::Schedule (GetDelayUntilIdle (), &WifiPhy::SetFrequency, this, frequency);
      break;
    case SpectrumWifiPhy::CCA_BUSY:
    case SpectrumWifiPhy::IDLE:
      goto switchFrequency;
      break;
    case SpectrumWifiPhy::SLEEP:
      NS_LOG_DEBUG ("frequency switching ignored in sleep mode");
      break;
    default:
      NS_ASSERT (false);
      break;
    }

  return false;

switchFrequency:

  NS_LOG_DEBUG ("switching frequency " << GetFrequency () << " -> " << frequency);
  m_rxSpectrumModel = WifiSpectrumValueHelper::GetSpectrumModel (GetFrequency (), GetChannelWidth ());
  m_state->SwitchToChannelSwitching (GetChannelSwitchDelay ());
  m_interference.EraseEvents ();
  /*
   * Needed here to be able to correctly sensed the medium for the first
   * time after the switching. The actual switching is not performed until
   * after m_channelSwitchDelay. Packets received during the switching
   * state are added to the event list and are employed later to figure
   * out the state of the medium after the switching.
   */
  return true;
}

Ptr<const SpectrumModel>
SpectrumWifiPhy::GetRxSpectrumModel () const
{
  NS_LOG_FUNCTION (this);
  if (m_rxSpectrumModel)
    {
      return m_rxSpectrumModel;
    }
  else
    {
      if (GetFrequency () == 0)
        {
          NS_LOG_DEBUG ("Frequency is not set; returning 0");
          return 0;
        }
      else
        {
          NS_LOG_DEBUG ("Creating spectrum model from frequency/width pair of (" << GetFrequency () << ", " << GetChannelWidth () << ")");
          m_rxSpectrumModel = WifiSpectrumValueHelper::GetSpectrumModel (GetFrequency (), GetChannelWidth ());
        }
    }
  return m_rxSpectrumModel;
}

Ptr<WifiChannel>
SpectrumWifiPhy::GetChannel (void) const
{
  return Ptr<WifiChannel> (0);
}

void
SpectrumWifiPhy::SetChannel (Ptr<SpectrumChannel> channel)
{
  m_channel = channel;
}

void
SpectrumWifiPhy::SetPacketReceivedCallback (RxCallback callback)
{
  m_rxCallback = callback;
}

void
SpectrumWifiPhy::AddOperationalChannel (uint16_t channelNumber)
{
  m_operationalChannelList.push_back (channelNumber);
}

std::vector<uint16_t>
SpectrumWifiPhy::GetOperationalChannelList () const
{
  std::vector<uint16_t> channelList;
  channelList.push_back (GetChannelNumber ());  // first channel of list
  for (std::vector<uint16_t>::size_type i = 0; i != m_operationalChannelList.size (); i++)
    {
      if (m_operationalChannelList[i] != GetChannelNumber ())
        {
          channelList.push_back (m_operationalChannelList[i]);
        }
    }
  return channelList;
}

void
SpectrumWifiPhy::ClearOperationalChannelList ()
{
  m_operationalChannelList.clear ();
}

void
SpectrumWifiPhy::SetSleepMode (void)
{
  NS_LOG_FUNCTION (this);
  switch (m_state->GetState ())
    {
    case SpectrumWifiPhy::TX:
      NS_LOG_DEBUG ("setting sleep mode postponed until end of current transmission");
      Simulator::Schedule (GetDelayUntilIdle (), &SpectrumWifiPhy::SetSleepMode, this);
      break;
    case SpectrumWifiPhy::RX:
      NS_LOG_DEBUG ("setting sleep mode postponed until end of current reception");
      Simulator::Schedule (GetDelayUntilIdle (), &SpectrumWifiPhy::SetSleepMode, this);
      break;
    case SpectrumWifiPhy::SWITCHING:
      NS_LOG_DEBUG ("setting sleep mode postponed until end of channel switching");
      Simulator::Schedule (GetDelayUntilIdle (), &SpectrumWifiPhy::SetSleepMode, this);
      break;
    case SpectrumWifiPhy::CCA_BUSY:
    case SpectrumWifiPhy::IDLE:
      NS_LOG_DEBUG ("setting sleep mode");
      m_state->SwitchToSleep ();
      break;
    case SpectrumWifiPhy::SLEEP:
      NS_LOG_DEBUG ("already in sleep mode");
      break;
    default:
      NS_ASSERT (false);
      break;
    }
}

void
SpectrumWifiPhy::ResumeFromSleep (void)
{
  NS_LOG_FUNCTION (this);
  switch (m_state->GetState ())
    {
    case SpectrumWifiPhy::TX:
    case SpectrumWifiPhy::RX:
    case SpectrumWifiPhy::IDLE:
    case SpectrumWifiPhy::CCA_BUSY:
    case SpectrumWifiPhy::SWITCHING:
      {
        NS_LOG_DEBUG ("not in sleep mode, there is nothing to resume");
        break;
      }
    case SpectrumWifiPhy::SLEEP:
      {
        NS_LOG_DEBUG ("resuming from sleep mode");
        Time delayUntilCcaEnd = m_interference.GetEnergyDuration (DbmToW (GetCcaMode1Threshold ()));
        m_state->SwitchFromSleep (delayUntilCcaEnd);
        break;
      }
    default:
      {
        NS_ASSERT (false);
        break;
      }
    }
}

void
SpectrumWifiPhy::SetReceiveOkCallback (RxOkCallback callback)
{
  m_state->SetReceiveOkCallback (callback);
}

void
SpectrumWifiPhy::SetReceiveErrorCallback (RxErrorCallback callback)
{
  m_state->SetReceiveErrorCallback (callback);
}

void
SpectrumWifiPhy::SwitchMaybeToCcaBusy (void)
{
  NS_LOG_FUNCTION (this);
  //We are here because we have received the first bit of a packet and we are
  //not going to be able to synchronize on it
  //In this model, CCA becomes busy when the aggregation of all signals as
  //tracked by the InterferenceHelper class is higher than the CcaBusyThreshold

  Time delayUntilCcaEnd = m_interference.GetEnergyDuration (DbmToW (GetCcaMode1Threshold ()));
  if (!delayUntilCcaEnd.IsZero ())
    {
      NS_LOG_DEBUG ("Calling SwitchMaybeToCcaBusy for " << delayUntilCcaEnd.As (Time::S));
      m_state->SwitchMaybeToCcaBusy (delayUntilCcaEnd);
    }
}

void
SpectrumWifiPhy::StartRx (Ptr<SpectrumSignalParameters> rxParams)
{
  NS_LOG_FUNCTION (this << rxParams);
  Time rxDuration = rxParams->duration;
  Ptr<SpectrumValue> receivedSignalPsd = rxParams->psd;
  NS_LOG_DEBUG ("Received signal with PSD " << *receivedSignalPsd << " and duration " << rxDuration.As (Time::NS));
  uint32_t senderNodeId = 0;
  if (rxParams->txPhy)
    {
      senderNodeId = rxParams->txPhy->GetDevice ()->GetNode ()->GetId ();
    }
  NS_LOG_DEBUG ("Received signal from " << senderNodeId << " with unfiltered power " << WToDbm (Integral (*receivedSignalPsd)) << " dBm");
  // Integrate over our receive bandwidth (i.e., all that the receive
  // spectral mask representing our filtering allows) to find the
  // total energy apparent to the "demodulator".
  Ptr<SpectrumValue> filter = WifiSpectrumValueHelper::CreateRfFilter (GetFrequency (), GetChannelWidth ());
  SpectrumValue filteredSignal = (*filter) * (*receivedSignalPsd);
  // Add receiver antenna gain
  NS_LOG_DEBUG ("Signal power received (watts) before antenna gain: " << Integral (filteredSignal));
  double rxPowerW = Integral (filteredSignal) * DbToRatio (GetRxGain ());
  NS_LOG_DEBUG ("Signal power received after antenna gain: " << rxPowerW << " W (" << WToDbm (rxPowerW) << " dBm)");

  Ptr<WifiSpectrumSignalParameters> wifiRxParams = DynamicCast<WifiSpectrumSignalParameters> (rxParams);

  // Log the signal arrival to the trace source
  m_signalCb (wifiRxParams ? true : false, senderNodeId, WToDbm (rxPowerW), rxDuration);
  if (wifiRxParams == 0)
    {
      NS_LOG_INFO ("Received non Wi-Fi signal");
      m_interference.AddForeignSignal (rxDuration, rxPowerW);
      SwitchMaybeToCcaBusy ();
      return;
    }
  if (wifiRxParams && m_disableWifiReception)
    {
      NS_LOG_INFO ("Received Wi-Fi signal but blocked from syncing");
      m_interference.AddForeignSignal (rxDuration, rxPowerW);
      SwitchMaybeToCcaBusy ();
      return;
    }

  NS_LOG_INFO ("Received Wi-Fi signal");
  Ptr<Packet> packet = wifiRxParams->packet;
  WifiPhyTag tag;
  bool found = packet->PeekPacketTag (tag);
  if (!found)
    {
      NS_FATAL_ERROR ("Received Wi-Fi Spectrum Signal with no WifiPhyTag");
      return;
    }

  WifiTxVector txVector = tag.GetWifiTxVector ();
  if (txVector.GetNss () > GetNumberOfReceiveAntennas ())
    {
      /* failure. */
      NotifyRxDrop (packet);
      NS_LOG_INFO ("Reception ends in failure because less RX antennas than number of spatial streams");
      SwitchMaybeToCcaBusy ();
      return;
    }

  enum WifiPreamble preamble = tag.GetWifiPreamble ();
  enum mpduType mpdutype = tag.GetMpduType ();

  // At this point forward, processing parallels that of
  // YansWifiPhy::StartReceivePreambleAndHeader ()

  AmpduTag ampduTag;
  Time endRx = Simulator::Now () + rxDuration;
  Time preambleAndHeaderDuration = CalculatePlcpPreambleAndHeaderDuration (txVector, preamble);

  Ptr<InterferenceHelper::Event> event;
  event = m_interference.Add (packet->GetSize (),
                              txVector,
                              preamble,
                              rxDuration,
                              rxPowerW);

  switch (m_state->GetState ())
    {
    case SpectrumWifiPhy::SWITCHING:
      NS_LOG_DEBUG ("drop packet because of channel switching");
      NotifyRxDrop (packet);
      m_plcpSuccess = false;
      /*
       * Packets received on the upcoming channel are added to the event list
       * during the switching state. This way the medium can be correctly sensed
       * when the device listens to the channel for the first time after the
       * switching e.g. after channel switching, the channel may be sensed as
       * busy due to other devices' tramissions started before the end of
       * the switching.
       */
      if (endRx > Simulator::Now () + m_state->GetDelayUntilIdle ())
        {
          //that packet will be noise _after_ the completion of the
          //channel switching.
          SwitchMaybeToCcaBusy ();
          return;
        }
      break;
    case SpectrumWifiPhy::RX:
      NS_LOG_DEBUG ("drop packet because already in Rx (power=" <<
                    rxPowerW << "W)");
      NotifyRxDrop (packet);
      if (endRx > Simulator::Now () + m_state->GetDelayUntilIdle ())
        {
          //that packet will be noise _after_ the reception of the
          //currently-received packet.
          SwitchMaybeToCcaBusy ();
          return;
        }
      break;
    case SpectrumWifiPhy::TX:
      NS_LOG_DEBUG ("drop packet because already in Tx (power=" <<
                    rxPowerW << "W)");
      NotifyRxDrop (packet);
      if (endRx > Simulator::Now () + m_state->GetDelayUntilIdle ())
        {
          //that packet will be noise _after_ the transmission of the
          //currently-transmitted packet.
          SwitchMaybeToCcaBusy ();
          return;
        }
      break;
    case SpectrumWifiPhy::CCA_BUSY:
    case SpectrumWifiPhy::IDLE:
      if (rxPowerW > GetEdThresholdW ()) //checked here, no need to check in the payload reception (current implementation assumes constant rx power over the packet duration)
        {
          if (preamble == WIFI_PREAMBLE_NONE && m_mpdusNum == 0)
            {
              NS_LOG_DEBUG ("drop packet because no preamble has been received");
              NotifyRxDrop (packet);
              SwitchMaybeToCcaBusy ();
              return;
            }
          else if (preamble == WIFI_PREAMBLE_NONE && m_plcpSuccess == false) //A-MPDU reception fails
            {
              NS_LOG_DEBUG ("Drop MPDU because no plcp has been received");
              NotifyRxDrop (packet);
              SwitchMaybeToCcaBusy ();
              return;
            }
          else if (preamble != WIFI_PREAMBLE_NONE && packet->PeekPacketTag (ampduTag) && m_mpdusNum == 0)
            {
              //received the first MPDU in an MPDU
              m_mpdusNum = ampduTag.GetRemainingNbOfMpdus () - 1;
              m_rxMpduReferenceNumber++;
            }
          else if (preamble == WIFI_PREAMBLE_NONE && packet->PeekPacketTag (ampduTag) && m_mpdusNum > 0)
            {
              //received the other MPDUs that are part of the A-MPDU
              if (ampduTag.GetRemainingNbOfMpdus () < m_mpdusNum)
                {
                  NS_LOG_DEBUG ("Missing MPDU from the A-MPDU " << m_mpdusNum - ampduTag.GetRemainingNbOfMpdus ());
                  m_mpdusNum = ampduTag.GetRemainingNbOfMpdus ();
                }
              else
                {
                  m_mpdusNum--;
                }
            }
          else if (preamble != WIFI_PREAMBLE_NONE && m_mpdusNum > 0 )
            {
              NS_LOG_DEBUG ("Didn't receive the last MPDUs from an A-MPDU " << m_mpdusNum);
              m_mpdusNum = 0;
            }

          NS_LOG_DEBUG ("sync to signal (power=" << rxPowerW << "W)");
          //sync to signal
          m_state->SwitchToRx (rxDuration);
          NS_ASSERT (m_endPlcpRxEvent.IsExpired ());
          NotifyRxBegin (packet);
          m_interference.NotifyRxStart ();

          if (preamble != WIFI_PREAMBLE_NONE)
            {
              NS_ASSERT (m_endPlcpRxEvent.IsExpired ());
              m_endPlcpRxEvent = Simulator::Schedule (preambleAndHeaderDuration, &SpectrumWifiPhy::StartReceivePacket, this,
                                                      packet, txVector, preamble, mpdutype, event);
            }

          NS_ASSERT (m_endRxEvent.IsExpired ());
          m_endRxEvent = Simulator::Schedule (rxDuration, &SpectrumWifiPhy::EndReceive, this,
                                              packet, preamble, mpdutype, event);
        }
      else
        {
          NS_LOG_DEBUG ("drop packet because signal power too Small (" <<
                        rxPowerW << "<" << GetEdThresholdW () << ")");
          NotifyRxDrop (packet);
          m_plcpSuccess = false;
          SwitchMaybeToCcaBusy ();
          return;
        }
      break;
    case SpectrumWifiPhy::SLEEP:
      NS_LOG_DEBUG ("drop packet because in sleep mode");
      NotifyRxDrop (packet);
      m_plcpSuccess = false;
      break;
    }

  return;
}

void
SpectrumWifiPhy::StartReceivePacket (Ptr<Packet> packet,
                                     WifiTxVector txVector,
                                     enum WifiPreamble preamble,
                                     enum mpduType mpdutype,
                                     Ptr<InterferenceHelper::Event> event)
{
  NS_LOG_FUNCTION (this << packet << txVector.GetMode () << preamble << (uint32_t)mpdutype);
  NS_ASSERT (IsStateRx ());
  NS_ASSERT (m_endPlcpRxEvent.IsExpired ());
  AmpduTag ampduTag;
  WifiMode txMode = txVector.GetMode ();

  struct InterferenceHelper::SnrPer snrPer;
  snrPer = m_interference.CalculatePlcpHeaderSnrPer (event);

  NS_LOG_DEBUG ("snr(dB)=" << RatioToDb (snrPer.snr) << ", per=" << snrPer.per);

  if (m_random->GetValue () > snrPer.per) //plcp reception succeeded
    {
      if (IsModeSupported (txMode) || IsMcsSupported (txMode))
        {
          NS_LOG_DEBUG ("receiving plcp payload"); //endReceive is already scheduled
          m_plcpSuccess = true;
        }
      else //mode is not allowed
        {
          NS_LOG_DEBUG ("drop packet because it was sent using an unsupported mode (" << txMode << ")");
          NotifyRxDrop (packet);
          m_plcpSuccess = false;
        }
    }
  else //plcp reception failed
    {
      NS_LOG_DEBUG ("drop packet because plcp preamble/header reception failed");
      NotifyRxDrop (packet);
      m_plcpSuccess = false;
    }
}

Ptr<WifiSpectrumPhyInterface>
SpectrumWifiPhy::GetSpectrumPhy (void) const
{
  return m_wifiSpectrumPhyInterface;
}

Ptr<AntennaModel>
SpectrumWifiPhy::GetRxAntenna (void) const
{
  return m_antenna;
}

void
SpectrumWifiPhy::SetAntenna (Ptr<AntennaModel> a)
{
  NS_LOG_FUNCTION (this << a);
  m_antenna = a;
}

void
SpectrumWifiPhy::CreateWifiSpectrumPhyInterface (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  m_wifiSpectrumPhyInterface = CreateObject<WifiSpectrumPhyInterface> ();
  m_wifiSpectrumPhyInterface->SetSpectrumWifiPhy (this);
  m_wifiSpectrumPhyInterface->SetDevice (device);
}

void
SpectrumWifiPhy::SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, WifiPreamble preamble)
{
  SendPacket (packet, txVector, preamble, NORMAL_MPDU);
}

Ptr<SpectrumValue>
SpectrumWifiPhy::GetTxPowerSpectralDensity (uint32_t centerFrequency, uint32_t channelWidth, double txPowerW) const
{
  NS_LOG_FUNCTION (centerFrequency << channelWidth << txPowerW);
  Ptr<SpectrumValue> v;
  switch (GetStandard ())
    {
    case WIFI_PHY_STANDARD_80211a:
    case WIFI_PHY_STANDARD_80211g:
    case WIFI_PHY_STANDARD_holland:
    case WIFI_PHY_STANDARD_80211_10MHZ:
    case WIFI_PHY_STANDARD_80211_5MHZ:
      v = WifiSpectrumValueHelper::CreateOfdmTxPowerSpectralDensity (centerFrequency, channelWidth, txPowerW);
      break;
    case WIFI_PHY_STANDARD_80211b:
      v = WifiSpectrumValueHelper::CreateDsssTxPowerSpectralDensity (centerFrequency, txPowerW);
      break;
    case WIFI_PHY_STANDARD_80211n_2_4GHZ:
    case WIFI_PHY_STANDARD_80211n_5GHZ:
    case WIFI_PHY_STANDARD_80211ac:
      v = WifiSpectrumValueHelper::CreateHtOfdmTxPowerSpectralDensity (centerFrequency, channelWidth, txPowerW);
      break;
    default:
      NS_FATAL_ERROR ("Standard unknown: " << GetStandard ());
      break;
    }
  return v;
}

void
SpectrumWifiPhy::SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, WifiPreamble preamble, enum mpduType mpdutype)
{
  NS_LOG_FUNCTION (this << packet << txVector.GetMode ()
                        << txVector.GetMode ().GetDataRate (txVector)
                        << preamble << (uint32_t)txVector.GetTxPowerLevel () << (uint32_t)mpdutype);
  /* Transmission can happen if:
   *  - we are syncing on a packet. It is the responsability of the
   *    MAC layer to avoid doing this but the PHY does nothing to
   *    prevent it.
   *  - we are idle
   */
  NS_ASSERT (!m_state->IsStateTx () && !m_state->IsStateSwitching ());

  if (m_state->IsStateSleep ())
    {
      NS_LOG_DEBUG ("Dropping packet because in sleep mode");
      NotifyTxDrop (packet);
      return;
    }

  Time txDuration = CalculateTxDuration (packet->GetSize (), txVector, preamble, GetFrequency (), mpdutype, 1);
  NS_ASSERT (txDuration > NanoSeconds (0));

  if (m_state->IsStateRx ())
    {
      m_endPlcpRxEvent.Cancel ();
      m_endRxEvent.Cancel ();
      m_interference.NotifyRxEnd ();
    }
  NotifyTxBegin (packet);
  uint32_t dataRate500KbpsUnits;
  if (txVector.GetMode ().GetModulationClass () == WIFI_MOD_CLASS_HT || txVector.GetMode ().GetModulationClass () == WIFI_MOD_CLASS_VHT)
    {
      dataRate500KbpsUnits = 128 + txVector.GetMode ().GetMcsValue ();
    }
  else
    {
      dataRate500KbpsUnits = txVector.GetMode ().GetDataRate (txVector.GetChannelWidth (), txVector.IsShortGuardInterval (), 1) * txVector.GetNss () / 500000;
    }
  if (mpdutype == MPDU_IN_AGGREGATE && preamble != WIFI_PREAMBLE_NONE)
    {
      //send the first MPDU in an MPDU
      m_txMpduReferenceNumber++;
    }
  struct mpduInfo aMpdu;
  aMpdu.type = mpdutype;
  aMpdu.mpduRefNumber = m_txMpduReferenceNumber;
  NotifyMonitorSniffTx (packet, (uint16_t) GetFrequency (), GetChannelNumber (), dataRate500KbpsUnits, preamble, txVector, aMpdu);
  m_state->SwitchToTx (txDuration, packet, GetPowerDbm (txVector.GetTxPowerLevel ()), txVector, preamble);
  //
  // Spectrum elements added here
  //
  Ptr<Packet> newPacket = packet->Copy (); // obtain non-const Packet
  WifiPhyTag oldtag;
  newPacket->RemovePacketTag (oldtag);
  WifiPhyTag tag (txVector, preamble, mpdutype);
  newPacket->AddPacketTag (tag);

  NS_LOG_DEBUG ("Transmission signal power before antenna gain: " << GetPowerDbm (txVector.GetTxPowerLevel ()) << " dBm");
  double txPowerWatts = DbmToW (GetPowerDbm (txVector.GetTxPowerLevel ()) + GetTxGain ());

  Ptr<SpectrumValue> txPowerSpectrum = GetTxPowerSpectralDensity (GetFrequency (), GetChannelWidth (), txPowerWatts);
  Ptr<WifiSpectrumSignalParameters> txParams = Create<WifiSpectrumSignalParameters> ();
  txParams->duration = txDuration;
  txParams->psd = txPowerSpectrum;
  NS_ASSERT_MSG (m_wifiSpectrumPhyInterface, "SpectrumPhy() is not set; maybe forgot to call CreateWifiSpectrumPhyInterface?");
  txParams->txPhy = m_wifiSpectrumPhyInterface->GetObject<SpectrumPhy> ();
  txParams->txAntenna = m_antenna;
  txParams->packet = newPacket;
  NS_LOG_DEBUG ("Starting transmission with power " << WToDbm (txPowerWatts) << " dBm on channel " << GetChannelNumber ());
  NS_LOG_DEBUG ("Starting transmission with integrated spectrum power " << WToDbm (Integral (*txPowerSpectrum)) << " dBm; spectrum model Uid: " << txPowerSpectrum->GetSpectrumModel ()->GetUid ());
  m_channel->StartTx (txParams);
}

void
SpectrumWifiPhy::RegisterListener (WifiPhyListener *listener)
{
  m_state->RegisterListener (listener);
}

void
SpectrumWifiPhy::UnregisterListener (WifiPhyListener *listener)
{
  m_state->UnregisterListener (listener);
}

void
SpectrumWifiPhy::EndReceive (Ptr<Packet> packet, enum WifiPreamble preamble, enum mpduType mpdutype, Ptr<InterferenceHelper::Event> event)
{
  NS_LOG_FUNCTION (this << packet << event);
  NS_ASSERT (IsStateRx ());
  NS_ASSERT (event->GetEndTime () == Simulator::Now ());

  struct InterferenceHelper::SnrPer snrPer;
  snrPer = m_interference.CalculatePlcpPayloadSnrPer (event);
  m_interference.NotifyRxEnd ();
  bool rxSucceeded;

  if (m_plcpSuccess == true)
    {
      NS_LOG_DEBUG ("mode=" << (event->GetPayloadMode ().GetDataRate (event->GetTxVector ())) <<
                    ", snr(dB)=" << RatioToDb (snrPer.snr) << ", per=" << snrPer.per << ", size=" << packet->GetSize ());

      if (m_random->GetValue () > snrPer.per)
        {
          NotifyRxEnd (packet);
          uint32_t dataRate500KbpsUnits;
          if ((event->GetPayloadMode ().GetModulationClass () == WIFI_MOD_CLASS_HT) || (event->GetPayloadMode ().GetModulationClass () == WIFI_MOD_CLASS_VHT))
            {
              dataRate500KbpsUnits = 128 + event->GetPayloadMode ().GetMcsValue ();
            }
          else
            {
              dataRate500KbpsUnits = event->GetPayloadMode ().GetDataRate (event->GetTxVector ().GetChannelWidth (), event->GetTxVector ().IsShortGuardInterval (), 1) * event->GetTxVector ().GetNss () / 500000;
            }
          struct signalNoiseDbm signalNoise;
          signalNoise.signal = RatioToDb (event->GetRxPowerW ()) + 30;
          signalNoise.noise = RatioToDb (event->GetRxPowerW () / snrPer.snr) - GetRxNoiseFigure () + 30;
          struct mpduInfo aMpdu;
          aMpdu.type = mpdutype;
          aMpdu.mpduRefNumber = m_rxMpduReferenceNumber;
          NotifyMonitorSniffRx (packet, (uint16_t) GetFrequency (), GetChannelNumber (), dataRate500KbpsUnits, event->GetPreambleType (), event->GetTxVector (), aMpdu, signalNoise);
          m_state->SwitchFromRxEndOk (packet, snrPer.snr, event->GetTxVector (), event->GetPreambleType ());
          rxSucceeded = true;
        }
      else
        {
          /* failure. */
          NotifyRxDrop (packet);
          m_state->SwitchFromRxEndError (packet, snrPer.snr);
          rxSucceeded = false;
        }
      if (!m_rxCallback.IsNull ())
        {
          m_rxCallback (rxSucceeded);
        }
    }
  else
    {
      m_state->SwitchFromRxEndError (packet, snrPer.snr);
      if (!m_rxCallback.IsNull ())
        {
          m_rxCallback (false);
        }
    }

  if (preamble == WIFI_PREAMBLE_NONE && mpdutype == LAST_MPDU_IN_AGGREGATE)
    {
      m_plcpSuccess = false;
    }

}

} //namespace ns3
