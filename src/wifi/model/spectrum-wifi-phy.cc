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

#include "ns3/wifi-spectrum-value-helper.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "spectrum-wifi-phy.h"
#include "wifi-spectrum-signal-parameters.h"
#include "wifi-spectrum-phy-interface.h"
#include "wifi-utils.h"

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
  WifiPhy::DoDispose ();
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
          uint16_t channelWidth = GetChannelWidth ();
          NS_LOG_DEBUG ("Creating spectrum model from frequency/width pair of (" << GetFrequency () << ", " << channelWidth << ")");
          m_rxSpectrumModel = WifiSpectrumValueHelper::GetSpectrumModel (GetFrequency (), channelWidth, GetBandBandwidth (), GetGuardBandwidth (channelWidth));
        }
    }
  return m_rxSpectrumModel;
}

Ptr<Channel>
SpectrumWifiPhy::GetChannel (void) const
{
  return m_channel;
}

void
SpectrumWifiPhy::SetChannel (const Ptr<SpectrumChannel> channel)
{
  m_channel = channel;
}

void
SpectrumWifiPhy::ResetSpectrumModel (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (IsInitialized (), "Executing method before run-time");
  uint16_t channelWidth = GetChannelWidth ();
  NS_LOG_DEBUG ("Run-time change of spectrum model from frequency/width pair of (" << GetFrequency () << ", " << channelWidth << ")");
  // Replace existing spectrum model with new one, and must call AddRx ()
  // on the SpectrumChannel to provide this new spectrum model to it
  m_rxSpectrumModel = WifiSpectrumValueHelper::GetSpectrumModel (GetFrequency (), channelWidth, GetBandBandwidth (), GetGuardBandwidth (channelWidth));
  m_channel->AddRx (m_wifiSpectrumPhyInterface);
}

void
SpectrumWifiPhy::SetChannelNumber (uint8_t nch)
{
  NS_LOG_FUNCTION (this << +nch);
  WifiPhy::SetChannelNumber (nch);
  if (IsInitialized ())
    {
      ResetSpectrumModel ();
    }
}

void
SpectrumWifiPhy::SetFrequency (uint16_t freq)
{
  NS_LOG_FUNCTION (this << freq);
  WifiPhy::SetFrequency (freq);
  if (IsInitialized ())
    {
      ResetSpectrumModel ();
    }
}

void
SpectrumWifiPhy::SetChannelWidth (uint16_t channelwidth)
{
  NS_LOG_FUNCTION (this << channelwidth);
  WifiPhy::SetChannelWidth (channelwidth);
  if (IsInitialized ())
    {
      ResetSpectrumModel ();
    }
}

void
SpectrumWifiPhy::ConfigureStandard (WifiPhyStandard standard)
{
  NS_LOG_FUNCTION (this << standard);
  WifiPhy::ConfigureStandard (standard);
  if (IsInitialized ())
    {
      ResetSpectrumModel ();
    }
}

void
SpectrumWifiPhy::AddOperationalChannel (uint8_t channelNumber)
{
  m_operationalChannelList.push_back (channelNumber);
}

std::vector<uint8_t>
SpectrumWifiPhy::GetOperationalChannelList () const
{
  std::vector<uint8_t> channelList;
  channelList.push_back (GetChannelNumber ());  // first channel of list
  for (std::vector<uint8_t>::size_type i = 0; i != m_operationalChannelList.size (); i++)
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
  uint16_t channelWidth = GetChannelWidth ();
  Ptr<SpectrumValue> filter = WifiSpectrumValueHelper::CreateRfFilter (GetFrequency (), channelWidth, GetBandBandwidth (), GetGuardBandwidth (channelWidth));
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
  Ptr<Packet> packet = wifiRxParams->packet->Copy ();
  StartReceivePreambleAndHeader (packet, rxPowerW, rxDuration);
}

Ptr<AntennaModel>
SpectrumWifiPhy::GetRxAntenna (void) const
{
  return m_antenna;
}

void
SpectrumWifiPhy::SetAntenna (const Ptr<AntennaModel> a)
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

Ptr<SpectrumValue>
SpectrumWifiPhy::GetTxPowerSpectralDensity (uint16_t centerFrequency, uint16_t channelWidth, double txPowerW, WifiModulationClass modulationClass) const
{
  NS_LOG_FUNCTION (centerFrequency << channelWidth << txPowerW);
  Ptr<SpectrumValue> v;
  switch (modulationClass)
    {
    case WIFI_MOD_CLASS_OFDM:
    case WIFI_MOD_CLASS_ERP_OFDM:
      v = WifiSpectrumValueHelper::CreateOfdmTxPowerSpectralDensity (centerFrequency, channelWidth, txPowerW, GetGuardBandwidth (channelWidth));
      break;
    case WIFI_MOD_CLASS_DSSS:
    case WIFI_MOD_CLASS_HR_DSSS:
      NS_ABORT_MSG_IF (channelWidth != 22, "Invalid channel width for DSSS");
      v = WifiSpectrumValueHelper::CreateDsssTxPowerSpectralDensity (centerFrequency, txPowerW, GetGuardBandwidth (channelWidth));
      break;
    case WIFI_MOD_CLASS_HT:
    case WIFI_MOD_CLASS_VHT:
      v = WifiSpectrumValueHelper::CreateHtOfdmTxPowerSpectralDensity (centerFrequency, channelWidth, txPowerW, GetGuardBandwidth (channelWidth));
      break;
    case WIFI_MOD_CLASS_HE:
      v = WifiSpectrumValueHelper::CreateHeOfdmTxPowerSpectralDensity (centerFrequency, channelWidth, txPowerW, GetGuardBandwidth (channelWidth));
      break;
    default:
      NS_FATAL_ERROR ("modulation class unknown: " << modulationClass);
      break;
    }
  return v;
}

uint16_t
SpectrumWifiPhy::GetCenterFrequencyForChannelWidth (WifiTxVector txVector) const
{
  NS_LOG_FUNCTION (this << txVector);
  uint16_t centerFrequencyForSupportedWidth = GetFrequency ();
  uint16_t supportedWidth = GetChannelWidth ();
  uint16_t currentWidth = txVector.GetChannelWidth ();
  if (currentWidth != supportedWidth)
    {
      uint16_t startingFrequency = centerFrequencyForSupportedWidth - (supportedWidth / 2);
      return startingFrequency + (currentWidth / 2); // primary channel is in the lower part (for the time being)
    }
  return centerFrequencyForSupportedWidth;
}

void
SpectrumWifiPhy::StartTx (Ptr<Packet> packet, WifiTxVector txVector, Time txDuration)
{
  NS_LOG_DEBUG ("Start transmission: signal power before antenna gain=" << GetPowerDbm (txVector.GetTxPowerLevel ()) << "dBm");
  double txPowerWatts = DbmToW (GetPowerDbm (txVector.GetTxPowerLevel ()) + GetTxGain ());
  Ptr<SpectrumValue> txPowerSpectrum = GetTxPowerSpectralDensity (GetCenterFrequencyForChannelWidth (txVector), txVector.GetChannelWidth (), txPowerWatts, txVector.GetMode ().GetModulationClass ());
  Ptr<WifiSpectrumSignalParameters> txParams = Create<WifiSpectrumSignalParameters> ();
  txParams->duration = txDuration;
  txParams->psd = txPowerSpectrum;
  NS_ASSERT_MSG (m_wifiSpectrumPhyInterface, "SpectrumPhy() is not set; maybe forgot to call CreateWifiSpectrumPhyInterface?");
  txParams->txPhy = m_wifiSpectrumPhyInterface->GetObject<SpectrumPhy> ();
  txParams->txAntenna = m_antenna;
  txParams->packet = packet;
  NS_LOG_DEBUG ("Starting transmission with power " << WToDbm (txPowerWatts) << " dBm on channel " << +GetChannelNumber ());
  NS_LOG_DEBUG ("Starting transmission with integrated spectrum power " << WToDbm (Integral (*txPowerSpectrum)) << " dBm; spectrum model Uid: " << txPowerSpectrum->GetSpectrumModel ()->GetUid ());
  m_channel->StartTx (txParams);
}

double
SpectrumWifiPhy::GetBandBandwidth (void) const
{
  double bandBandwidth = 0;
  switch (GetStandard ())
    {
    case WIFI_PHY_STANDARD_80211a:
    case WIFI_PHY_STANDARD_80211g:
    case WIFI_PHY_STANDARD_holland:
    case WIFI_PHY_STANDARD_80211b:
    case WIFI_PHY_STANDARD_80211n_2_4GHZ:
    case WIFI_PHY_STANDARD_80211n_5GHZ:
    case WIFI_PHY_STANDARD_80211ac:
      // Use OFDM subcarrier width of 312.5 KHz as band granularity
      bandBandwidth = 312500;
      break;
    case WIFI_PHY_STANDARD_80211_10MHZ:
      // Use OFDM subcarrier width of 156.25 KHz as band granularity
      bandBandwidth = 156250;
      break;
    case WIFI_PHY_STANDARD_80211_5MHZ:
      // Use OFDM subcarrier width of 78.125 KHz as band granularity
      bandBandwidth = 78125;
      break;
    case WIFI_PHY_STANDARD_80211ax_2_4GHZ:
    case WIFI_PHY_STANDARD_80211ax_5GHZ:
      // Use OFDM subcarrier width of 78.125 KHz as band granularity
      bandBandwidth = 78125;
      break;
    default:
      NS_FATAL_ERROR ("Standard unknown: " << GetStandard ());
      break;
    }
  return bandBandwidth;
}

uint16_t
SpectrumWifiPhy::GetGuardBandwidth (uint16_t currentChannelWidth) const
{
  uint16_t guardBandwidth = 0;
  if (currentChannelWidth == 22)
    {
      //handle case of use of legacy DSSS transmission
      guardBandwidth = 10;
    }
  else
    {
      //In order to properly model out of band transmissions for OFDM, the guard
      //band has been configured so as to expand the modeled spectrum up to the
      //outermost referenced point in "Transmit spectrum mask" sections' PSDs of
      //each PHY specification of 802.11-2016 standard. It thus ultimately corresponds
      //to the currently considered channel bandwidth (which can be different from
      //supported channel width).
      guardBandwidth = currentChannelWidth;
    }
  return guardBandwidth;
}

} //namespace ns3
