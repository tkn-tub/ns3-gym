/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Author: Gary Pei <guangyu.pei@boeing.com>
 */
#include "ns3/log.h"
#include <math.h>
#include "ns3/simulator.h"
#include "ns3/lr-wpan-phy.h"

NS_LOG_COMPONENT_DEFINE ("LrWpanPhy");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LrWpanPhy);

// Table 22 in section 6.4.1 of ieee802.15.4
const uint32_t LrWpanPhy::aMaxPhyPacketSize = 127; // max PSDU in octets
const uint32_t LrWpanPhy::aTurnaroundTime = 12;  // RX-to-TX or TX-to-RX in symbol periods
// IEEE802.15.4-2006 Table 2 in section 6.1.2 (kb/s and ksymbol/s)
// The index follows LrWpanPhyOption
const LrWpanPhyDataAndSymbolRates
LrWpanPhy::dataSymbolRates[7] = { { 20.0, 20.0},
                                  { 40.0, 40.0},
                                  { 250.0, 12.5},
                                  { 250.0, 50.0},
                                  { 100.0, 25.0},
                                  { 250.0, 62.5},
                                  { 250.0, 62.5}};
// IEEE802.15.4-2006 Table 19 and Table 20 in section 6.3.
// The PHR is 1 octet and it follows phySymbolsPerOctet in Table 23
// The index follows LrWpanPhyOption
const LrWpanPhyPpduHeaderSymbolNumber
LrWpanPhy::ppduHeaderSymbolNumbers[7] = { { 32.0, 8.0, 8.0},
                                          { 32.0, 8.0, 8.0},
                                          { 2.0, 1.0, 0.4},
                                          { 6.0, 1.0, 1.6},
                                          { 8.0, 2.0, 2.0},
                                          { 8.0, 2.0, 2.0},
                                          { 8.0, 2.0, 2.0}};

TypeId
LrWpanPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LrWpanPhy")
    .SetParent<Object> ()
    .AddConstructor<LrWpanPhy> ()
  ;
  return tid;
}

LrWpanPhy::LrWpanPhy ()
  : m_edRequest (),
    m_setTRXState ()
{
  m_trxState = IEEE_802_15_4_PHY_RX_ON;
  m_trxStateDeferSet = IEEE_802_15_4_PHY_IDLE;
  m_state = IEEE_802_15_4_PHY_IDLE;
  m_sensedChannelState = IEEE_802_15_4_PHY_IDLE;

  // default PHY PIB attributes
  m_phyPIBAttributes.phyCurrentChannel = 11;
  m_phyPIBAttributes.phyTransmitPower = 0;
  m_phyPIBAttributes.phyCurrentPage = 0;
  for (uint32_t i = 0; i < 32; i++)
    {
      m_phyPIBAttributes.phyChannelsSupported[i] = 0x07ffffff;
    }
  m_phyPIBAttributes.phyCCAMode = 1;

  SetMyPhyOption ();

  m_rxEdPeakPower = 0.0;
  m_rxTotalPower = 0.0;
  m_rxTotalNum = 0;
  // default -85 dBm in W for 2.4 GHz
  m_rxSensitivity = pow (10.0, -85.0 / 10.0) / 1000.0;
  LrWpanSpectrumValueHelper psdHelper;
  m_txPsd = psdHelper.CreateTxPowerSpectralDensity (m_phyPIBAttributes.phyTransmitPower,
                                                    m_phyPIBAttributes.phyCurrentChannel);
  m_noise = psdHelper.CreateNoisePowerSpectralDensity (m_phyPIBAttributes.phyCurrentChannel);
  m_rxPsd = 0;
  Ptr <Packet> none = 0;
  m_currentRxPacket = std::make_pair (none, true);
  m_currentTxPacket = std::make_pair (none, true);
}

LrWpanPhy::~LrWpanPhy ()
{
}

void
LrWpanPhy::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_mobility = 0;
  m_device = 0;
  m_channel = 0;
  m_txPsd = 0;
  m_rxPsd = 0;
  m_noise = 0;
  m_pdDataIndicationCallback = MakeNullCallback< void, uint32_t, Ptr<Packet>, uint32_t > ();
  m_pdDataConfirmCallback = MakeNullCallback< void, LrWpanPhyEnumeration > ();
  m_plmeCcaConfirmCallback = MakeNullCallback< void, LrWpanPhyEnumeration > ();
  m_plmeEdConfirmCallback = MakeNullCallback< void, LrWpanPhyEnumeration,uint8_t > ();
  m_plmeGetAttributeConfirmCallback = MakeNullCallback< void, LrWpanPhyEnumeration, LrWpanPibAttributeIdentifier, LrWpanPhyPIBAttributes* > ();
  m_plmeSetTRXStateConfirmCallback = MakeNullCallback< void, LrWpanPhyEnumeration > ();
  m_plmeSetAttributeConfirmCallback = MakeNullCallback< void, LrWpanPhyEnumeration, LrWpanPibAttributeIdentifier > ();

  SpectrumPhy::DoDispose ();
}

Ptr<Object>
LrWpanPhy::GetDevice ()
{
  NS_LOG_FUNCTION (this);
  return m_device;
}


Ptr<Object>
LrWpanPhy::GetMobility ()
{
  NS_LOG_FUNCTION (this);
  return m_mobility;
}


void
LrWpanPhy::SetDevice (Ptr<Object> d)
{
  NS_LOG_FUNCTION (this << d);
  m_device = d;
}


void
LrWpanPhy::SetMobility (Ptr<Object> m)
{
  NS_LOG_FUNCTION (this << m);
  m_mobility = m;
}


void
LrWpanPhy::SetChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION (this << c);
  m_channel = c;
}


Ptr<SpectrumChannel>
LrWpanPhy::GetChannel (void)
{
  return m_channel;
}


Ptr<const SpectrumModel>
LrWpanPhy::GetRxSpectrumModel () const
{
  if (m_txPsd)
    {
      return m_txPsd->GetSpectrumModel ();
    }
  else
    {
      return 0;
    }
}

void
LrWpanPhy::StartRx (Ptr<PacketBurst> pb, Ptr <const SpectrumValue> rxPsd, SpectrumType st, Time duration)
{
  NS_LOG_FUNCTION (this << pb << rxPsd << st << duration);
  if (st == GetSpectrumType ())
    {
      Ptr<Packet> p = (pb->GetPackets ()).front ();
      m_currentRxPacket = std::make_pair (p, false);
      m_rxPsd = rxPsd;
      Simulator::Schedule (duration, &LrWpanPhy::EndRx, this);
    }
  NS_LOG_LOGIC (this << " state: " << m_state);
}

void LrWpanPhy::EndRx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << "state: " << m_state);
  if (!m_pdDataIndicationCallback.IsNull ())
    {
      Ptr<Packet> p = m_currentRxPacket.first;
      m_pdDataIndicationCallback (p->GetSize (), p, 0);
    }
}

void
LrWpanPhy::PdDataRequest (const uint32_t psduLength, Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << psduLength << p);

  if (psduLength > aMaxPhyPacketSize)
    {
      if (!m_pdDataConfirmCallback.IsNull ())
        {
          m_pdDataConfirmCallback (IEEE_802_15_4_PHY_UNSPECIFIED);
        }
      NS_LOG_DEBUG ("Drop packet because psduLength" << psduLength);
      return;
    }

  if (m_trxState == IEEE_802_15_4_PHY_TX_ON)
    {
      if (m_state == IEEE_802_15_4_PHY_IDLE)
        {
          //send down
          ChangeState (IEEE_802_15_4_PHY_BUSY);   // for carrier sensing
          NS_ASSERT (m_channel);
          Time txTime = CalculateTxTime (p);
          Ptr<PacketBurst> pb = CreateObject<PacketBurst> ();
          pb->AddPacket (p);
          m_channel->StartTx (pb, m_txPsd, GetSpectrumType (), txTime, GetObject<SpectrumPhy> ());
          m_pdDataRequest = Simulator::Schedule (txTime, &LrWpanPhy::EndTx, this);
        }
      else
        {
          NS_ASSERT_MSG (false, "impossible, should not happen");
        }
    }
  else
    {
      if (!m_pdDataConfirmCallback.IsNull ())
        {
          m_pdDataConfirmCallback (m_trxState);
        }
    }

}

void
LrWpanPhy::PlmeCcaRequest (void)
{
  NS_LOG_FUNCTION (this);
  if (m_trxState == IEEE_802_15_4_PHY_RX_ON)
    {
      Time ccaTime = Seconds (8.0 / GetDataOrSymbolRate (false));
      Simulator::Schedule (ccaTime, &LrWpanPhy::EndCca, this);
    }
  else
    {
      if (!m_plmeCcaConfirmCallback.IsNull ())
        {
          m_plmeCcaConfirmCallback (m_trxState);
        }
    }
}

void
LrWpanPhy::PlmeEdRequest (void)
{
  NS_LOG_FUNCTION (this);
  if (m_trxState == IEEE_802_15_4_PHY_RX_ON)
    {
      m_rxEdPeakPower = m_rxTotalPower;
      Time edTime = Seconds (8.0 / GetDataOrSymbolRate (false));
      m_edRequest = Simulator::Schedule (edTime, &LrWpanPhy::EndEd, this);
    }
  else
    {
      if (!m_plmeEdConfirmCallback.IsNull ())
        {
          m_plmeEdConfirmCallback (m_trxState,0);
        }
    }
}

void
LrWpanPhy::PlmeGetAttributeRequest (LrWpanPibAttributeIdentifier id)
{
  NS_LOG_FUNCTION (this << id);
  LrWpanPhyEnumeration status;

  switch (id)
    {
    case phyCurrentChannel:
    case phyChannelsSupported:
    case phyTransmitPower:
    case phyCCAMode:
    case phyCurrentPage:
    case phyMaxFrameDuration:
    case phySHRDuration:
    case phySymbolsPerOctet:
      {
        status = IEEE_802_15_4_PHY_SUCCESS;
        break;
      }
    default:
      {
        status = IEEE_802_15_4_PHY_UNSUPPORTED_ATTRIBUTE;
        break;
      }
    }
  if (!m_plmeGetAttributeConfirmCallback.IsNull ())
    {
      LrWpanPhyPIBAttributes retValue;
      memcpy (&retValue, &m_phyPIBAttributes, sizeof(LrWpanPhyPIBAttributes));
      m_plmeGetAttributeConfirmCallback (status,id,&retValue);
    }
}

void
LrWpanPhy::PlmeSetTRXStateRequest (LrWpanPhyEnumeration state)
{
  NS_LOG_FUNCTION (this << state);

  bool delay;
  LrWpanPhyEnumeration status;

  // ignore any pending request
  if (m_trxStateDeferSet != IEEE_802_15_4_PHY_IDLE)
    {
      m_trxStateDeferSet = IEEE_802_15_4_PHY_IDLE;
    }
  else if (m_setTRXState.IsRunning ())
    {
      m_setTRXState.Cancel ();
    }

  status = m_trxState;

  if (state != m_trxState)
    {
      delay = false;

      if (((state == IEEE_802_15_4_PHY_RX_ON) || (state == IEEE_802_15_4_PHY_TRX_OFF))
          && (m_state == IEEE_802_15_4_PHY_BUSY))
        {
          status = IEEE_802_15_4_PHY_BUSY;
          m_trxStateDeferSet = state;
        }
      else if (((state == IEEE_802_15_4_PHY_TX_ON)
                ||(state == IEEE_802_15_4_PHY_TRX_OFF))
               && (m_currentRxPacket.first) &&(!m_currentRxPacket.second))
        {    //if after received a valid SFD
          status = IEEE_802_15_4_PHY_BUSY_RX;
          m_trxStateDeferSet = state;
        }
      else if (state == IEEE_802_15_4_PHY_FORCE_TRX_OFF)
        {
          if (m_trxState == IEEE_802_15_4_PHY_TRX_OFF)
            {
              status = IEEE_802_15_4_PHY_TRX_OFF;
            }
          else
            {
              status = IEEE_802_15_4_PHY_SUCCESS;
            }

          m_trxState = IEEE_802_15_4_PHY_TRX_OFF;

          if (m_currentRxPacket.first)
            {   //terminate reception if needed
                //incomplete reception -- force packet discard
              m_currentRxPacket.second = true;
            }

          if (m_state == IEEE_802_15_4_PHY_BUSY)
            {   //terminate transmission if needed
              m_currentTxPacket.second = true;
              m_currentTxPacket.first = 0;
              m_pdDataRequest.Cancel ();
              m_state = IEEE_802_15_4_PHY_IDLE;
              if (!m_pdDataConfirmCallback.IsNull ())
                {
                  m_pdDataConfirmCallback (IEEE_802_15_4_PHY_TRX_OFF);
                }
              if (m_trxStateDeferSet != IEEE_802_15_4_PHY_IDLE)
                {
                  m_trxStateDeferSet = IEEE_802_15_4_PHY_IDLE;
                }
            }
        }
      else
        {
          status = IEEE_802_15_4_PHY_SUCCESS;
          if (((state == IEEE_802_15_4_PHY_RX_ON)&&(m_trxState == IEEE_802_15_4_PHY_TX_ON))
              ||((state == IEEE_802_15_4_PHY_TX_ON)&&(m_trxState == IEEE_802_15_4_PHY_RX_ON)))
            {
              m_trxStateTurnaround = state;
              delay = true;
            }
          else
            {
              m_trxState = state;
            }
        }

      //we need to delay <aTurnaroundTime> symbols if Tx2Rx or Rx2Tx
      if (delay)
        {    //should be disabled immediately
             //(further transmission/reception will not succeed)
          m_trxState = IEEE_802_15_4_PHY_TRX_OFF;

          Time setTime = Seconds ( (double) aTurnaroundTime / GetDataOrSymbolRate (false));
          m_setTRXState = Simulator::Schedule (setTime, &LrWpanPhy::EndSetTRXState, this);
        }
      else
        {
          if (!m_plmeSetTRXStateConfirmCallback.IsNull ())
            {
              m_plmeSetTRXStateConfirmCallback (status);
            }
        }
    }
  else
    {
      if (!m_plmeSetTRXStateConfirmCallback.IsNull ())
        {
          m_plmeSetTRXStateConfirmCallback (status);
        }
    }

  NS_LOG_LOGIC (this << " SetTRXRequest old: " << m_trxState << " request: " << state
                     << " return: " << status);
}

bool
LrWpanPhy::ChannelSupported (uint8_t channel)
{
  NS_LOG_FUNCTION (this << channel);
  bool retValue = false;

  for (uint32_t i = 0; i < 32; i++)
    {
      if ((m_phyPIBAttributes.phyChannelsSupported[i] & (1 << channel)) != 0)
        {
          retValue = true;
          break;
        }
    }

  return retValue;
}

void
LrWpanPhy::PlmeSetAttributeRequest (LrWpanPibAttributeIdentifier id,
                                    LrWpanPhyPIBAttributes* attribute)
{
  NS_ASSERT (attribute);
  NS_LOG_FUNCTION (this << id << attribute);
  LrWpanPhyEnumeration status = IEEE_802_15_4_PHY_SUCCESS;

  switch (id)
    {
    case phyCurrentChannel:
      {
        if (!ChannelSupported (attribute->phyCurrentChannel))
          {
            status = IEEE_802_15_4_PHY_INVALID_PARAMETER;
          }
        if (m_phyPIBAttributes.phyCurrentChannel != attribute->phyCurrentChannel)
          {    //any packet in transmission or reception will be corrupted
            if (m_currentRxPacket.first)
              {
                m_currentRxPacket.second = true;
              }
            if (m_state == IEEE_802_15_4_PHY_BUSY)
              {
                m_currentTxPacket.second = true;
                m_pdDataRequest.Cancel ();
                m_currentTxPacket.first = 0;
                m_state = IEEE_802_15_4_PHY_IDLE;
                if (!m_pdDataConfirmCallback.IsNull ())
                  {
                    m_pdDataConfirmCallback (IEEE_802_15_4_PHY_TRX_OFF);
                  }

                if (m_trxStateDeferSet != IEEE_802_15_4_PHY_IDLE)
                  {
                    m_trxStateDeferSet = IEEE_802_15_4_PHY_IDLE;
                  }
              }
            m_phyPIBAttributes.phyCurrentChannel = attribute->phyCurrentChannel;
          }
        break;
      }
    case phyChannelsSupported:
      {   // only the first element is considered in the array
        if ((attribute->phyChannelsSupported[0] & 0xf8000000) != 0)
          {    //5 MSBs reserved
            status = IEEE_802_15_4_PHY_INVALID_PARAMETER;
          }
        else
          {
            m_phyPIBAttributes.phyChannelsSupported[0] = attribute->phyChannelsSupported[0];
          }
        break;
      }
    case phyTransmitPower:
      {
        if (attribute->phyTransmitPower > 0xbf)
          {
            status = IEEE_802_15_4_PHY_INVALID_PARAMETER;
          }
        else
          {
            m_phyPIBAttributes.phyTransmitPower = attribute->phyTransmitPower;
          }
        break;
      }
    case phyCCAMode:
      {
        if ((attribute->phyCCAMode < 1) || (attribute->phyCCAMode > 3))
          {
            status = IEEE_802_15_4_PHY_INVALID_PARAMETER;
          }
        else
          {
            m_phyPIBAttributes.phyCCAMode = attribute->phyCCAMode;
          }
        break;
      }
    default:
      {
        status = IEEE_802_15_4_PHY_UNSUPPORTED_ATTRIBUTE;
        break;
      }
    }

  if (!m_plmeSetAttributeConfirmCallback.IsNull ())
    {
      m_plmeSetAttributeConfirmCallback (status, id);
    }
}

bool
LrWpanPhy::StartTx (Ptr<Packet> p)
{
  NS_LOG_UNCOND ("StartTx " << this << "pkt " << p);
  NS_LOG_FUNCTION (this << p);
  return true;
}

void
LrWpanPhy::SetPdDataIndicationCallback (PdDataIndicationCallback c)
{
  NS_LOG_FUNCTION (this);
  m_pdDataIndicationCallback = c;
}

void
LrWpanPhy::SetPdDataConfirmCallback (PdDataConfirmCallback c)
{
  NS_LOG_FUNCTION (this);
  m_pdDataConfirmCallback = c;
}

void
LrWpanPhy::SetPlmeCcaConfirmCallback (PlmeCcaConfirmCallback c)
{
  NS_LOG_FUNCTION (this);
  m_plmeCcaConfirmCallback = c;
}

void
LrWpanPhy::SetPlmeEdConfirmCallback (PlmeEdConfirmCallback c)
{
  NS_LOG_FUNCTION (this);
  m_plmeEdConfirmCallback = c;
}

void
LrWpanPhy::SetPlmeGetAttributeConfirmCallback (PlmeGetAttributeConfirmCallback c)
{
  NS_LOG_FUNCTION (this);
  m_plmeGetAttributeConfirmCallback = c;
}

void
LrWpanPhy::SetPlmeSetTRXStateConfirmCallback (PlmeSetTRXStateConfirmCallback c)
{
  NS_LOG_FUNCTION (this);
  m_plmeSetTRXStateConfirmCallback = c;
}

void
LrWpanPhy::SetPlmeSetAttributeConfirmCallback (PlmeSetAttributeConfirmCallback c)
{
  NS_LOG_FUNCTION (this);
  m_plmeSetAttributeConfirmCallback = c;
}

void
LrWpanPhy::ChangeState (LrWpanPhyEnumeration newState)
{
  NS_LOG_LOGIC (this << " state: " << m_state << " -> " << newState);
  m_state = newState;
}

void
LrWpanPhy::EndEd ()
{
  NS_LOG_FUNCTION (this);
  uint8_t energyLevel;

  // Per IEEE802.15.4-2006 sec 6.9.7
  double ratio = m_rxEdPeakPower / m_rxSensitivity;
  ratio = 10.0 * log10 (ratio);
  if (ratio <= 10.0)
    {  // less than 10 dB
      energyLevel = 0;
    }
  else if (ratio >= 40.0)
    { // less than 40 dB
      energyLevel = 255;
    }
  else
    {
      // in-between with linear increase per sec 6.9.7
      energyLevel = (uint8_t)((ratio / 10.0 - 1.0) * (255.0 / 3.0));
    }

  if (!m_plmeEdConfirmCallback.IsNull ())
    {
      m_plmeEdConfirmCallback (IEEE_802_15_4_PHY_SUCCESS, energyLevel);
    }
}

void
LrWpanPhy::EndCca ()
{
  NS_LOG_FUNCTION (this);

  if ((m_state == IEEE_802_15_4_PHY_BUSY)||(m_rxTotalNum > 0))
    {
      m_sensedChannelState = IEEE_802_15_4_PHY_BUSY;
    }
  else if (m_phyPIBAttributes.phyCCAMode == 1)
    { //sec 6.9.9 ED detection
      // -- ED threshold at most 10 dB above receiver sensitivity.
      if (m_rxTotalPower / m_rxSensitivity >= 10.0)
        {
          m_sensedChannelState = IEEE_802_15_4_PHY_BUSY;
        }
      else
        {
          m_sensedChannelState = IEEE_802_15_4_PHY_IDLE;
        }
    }
  else if (m_phyPIBAttributes.phyCCAMode == 2)
    { //sec 6.9.9 carrier sense only
      if (m_rxTotalNum > 0)
        {
          m_sensedChannelState = IEEE_802_15_4_PHY_BUSY;
        }
      else
        {
          m_sensedChannelState = IEEE_802_15_4_PHY_IDLE;
        }
    }
  else if (m_phyPIBAttributes.phyCCAMode == 3)
    { //sect 6.9.9 both
      if ((m_rxTotalPower / m_rxSensitivity >= 10.0)
          && (m_rxTotalNum > 0))
        {
          m_sensedChannelState = IEEE_802_15_4_PHY_BUSY;
        }
      else
        {
          m_sensedChannelState = IEEE_802_15_4_PHY_IDLE;
        }
    }
  else
    {
      NS_ASSERT_MSG (false, "Invalid CCA mode");
    }

  NS_LOG_LOGIC (this << "state: " << m_sensedChannelState);

  if (!m_plmeCcaConfirmCallback.IsNull ())
    {
      m_plmeCcaConfirmCallback (m_sensedChannelState);
    }
}

void
LrWpanPhy::EndSetTRXState ()
{
  NS_LOG_FUNCTION (this);

  m_trxState = m_trxStateTurnaround;

  if (!m_plmeSetTRXStateConfirmCallback.IsNull ())
    {
      m_plmeSetTRXStateConfirmCallback (m_trxState);
    }
}

void
LrWpanPhy::EndTx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << "state: " << m_state);

  NS_ASSERT (m_state == IEEE_802_15_4_PHY_BUSY);

  ChangeState (IEEE_802_15_4_PHY_IDLE);
  if (!m_pdDataConfirmCallback.IsNull ())
    {
      m_pdDataConfirmCallback (IEEE_802_15_4_PHY_SUCCESS);
    }
}

Time
LrWpanPhy::CalculateTxTime (Ptr<const Packet> packet)
{
  Time txTime = Seconds (0);
  bool isData = true;

  txTime = Seconds (GetPpduHeaderTxTime ()
                    + packet->GetSize () * 8.0 / GetDataOrSymbolRate (isData));

  return txTime;
}

SpectrumType
LrWpanPhy::GetSpectrumType ()
{
  NS_LOG_FUNCTION (this);
  static SpectrumType st = SpectrumTypeFactory::Create ("Ieee802.15.4");
  return st;
}

double
LrWpanPhy::GetDataOrSymbolRate (bool isData)
{
  double rate = 0.0;

  NS_ASSERT (m_phyOption < IEEE_802_15_4_INVALID_PHY_OPTION);

  if (isData)
    {
      rate = dataSymbolRates [m_phyOption].bitRate;
    }
  else
    {
      rate = dataSymbolRates [m_phyOption].symbolRate;
    }

  return (rate * 1000.0);
}

double
LrWpanPhy::GetPpduHeaderTxTime (void)
{
  bool isData = false;
  double totalPpduHdrSymbols;

  NS_ASSERT (m_phyOption < IEEE_802_15_4_INVALID_PHY_OPTION);

  totalPpduHdrSymbols = ppduHeaderSymbolNumbers[m_phyOption].shrPreamble
    + ppduHeaderSymbolNumbers[m_phyOption].shrSfd
    + ppduHeaderSymbolNumbers[m_phyOption].phr;

  return totalPpduHdrSymbols / GetDataOrSymbolRate (isData);
}

// IEEE802.15.4-2006 Table 2 in section 6.1.2
void
LrWpanPhy::SetMyPhyOption (void)
{
  m_phyOption = IEEE_802_15_4_INVALID_PHY_OPTION;

  if (m_phyPIBAttributes.phyCurrentPage == 0)
    {
      if (m_phyPIBAttributes.phyCurrentChannel == 0)
        {  // 868 MHz BPSK
          m_phyOption = IEEE_802_15_4_868MHZ_BPSK;
        }
      else if (m_phyPIBAttributes.phyCurrentChannel <= 10)
        {  // 915 MHz BPSK
          m_phyOption = IEEE_802_15_4_915MHZ_BPSK;
        }
      else if (m_phyPIBAttributes.phyCurrentChannel <= 26)
        {  // 2.4 GHz MHz O-QPSK
          m_phyOption = IEEE_802_15_4_2_4GHZ_OQPSK;
        }
    }
  else if (m_phyPIBAttributes.phyCurrentPage == 1)
    {
      if (m_phyPIBAttributes.phyCurrentChannel == 0)
        {  // 868 MHz ASK
          m_phyOption = IEEE_802_15_4_868MHZ_ASK;
        }
      else if (m_phyPIBAttributes.phyCurrentChannel <= 10)
        {  // 915 MHz ASK
          m_phyOption = IEEE_802_15_4_915MHZ_ASK;
        }
    }
  else if (m_phyPIBAttributes.phyCurrentPage == 2)
    {
      if (m_phyPIBAttributes.phyCurrentChannel == 0)
        {  // 868 MHz O-QPSK
          m_phyOption = IEEE_802_15_4_868MHZ_OQPSK;
        }
      else if (m_phyPIBAttributes.phyCurrentChannel <= 10)
        {  // 915 MHz O-QPSK
          m_phyOption = IEEE_802_15_4_915MHZ_OQPSK;
        }
    }
}

LrWpanPhyOption
LrWpanPhy::GetMyPhyOption (void)
{
  return m_phyOption;
}

void
LrWpanPhy::SetTxPowerSpectralDensity (Ptr<SpectrumValue> txPsd)
{
  NS_LOG_FUNCTION (this << txPsd);
  NS_ASSERT (txPsd);
  m_txPsd = txPsd;
  NS_LOG_INFO ("\t computed tx_psd: " << *txPsd << "\t stored tx_psd: " << *m_txPsd);
}

void
LrWpanPhy::SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd)
{
  NS_LOG_FUNCTION (this << noisePsd);
  NS_LOG_INFO ("\t computed noise_psd: " << *noisePsd );
  NS_ASSERT (noisePsd);
  m_noise = noisePsd;
}

Ptr<const SpectrumValue>
LrWpanPhy::GetNoisePowerSpectralDensity (void)
{
  NS_LOG_FUNCTION (this);
  return m_noise;
}

} // namespace ns3
