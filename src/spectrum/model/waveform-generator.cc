/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */


#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <ns3/simulator.h>
#include <ns3/double.h>
#include <ns3/packet-burst.h>
#include <ns3/antenna-model.h>

#include "waveform-generator.h"

NS_LOG_COMPONENT_DEFINE ("WaveformGenerator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WaveformGenerator);

WaveformGenerator::WaveformGenerator ()
  : m_mobility (0),
    m_netDevice (0),
    m_channel (0),
    m_txPowerSpectralDensity (0),
    m_startTime (Seconds (0)),
    m_active (false)
{

}


WaveformGenerator::~WaveformGenerator ()
{

}


void
WaveformGenerator::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_channel = 0;
  m_netDevice = 0;
  m_mobility = 0;
}

TypeId
WaveformGenerator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WaveformGenerator")
    .SetParent<SpectrumPhy> ()
    .AddConstructor<WaveformGenerator> ()
    .AddAttribute ("Period",
                   "the period (=1/frequency)",
                   TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&WaveformGenerator::SetPeriod,
                                     &WaveformGenerator::GetPeriod),
                   MakeTimeChecker ())
    .AddAttribute ("DutyCycle",
                   "the duty cycle of the generator, i.e., the fraction of the period that is occupied by a signal",
                   DoubleValue (0.5),
                   MakeDoubleAccessor (&WaveformGenerator::SetDutyCycle,
                                       &WaveformGenerator::GetDutyCycle),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("TxStart",
                     "Trace fired when a new transmission is started",
                     MakeTraceSourceAccessor (&WaveformGenerator::m_phyTxStartTrace))
    .AddTraceSource ("TxEnd",
                     "Trace fired when a previosuly started transmission is finished",
                     MakeTraceSourceAccessor (&WaveformGenerator::m_phyTxEndTrace))
  ;
  return tid;
}



Ptr<NetDevice>
WaveformGenerator::GetDevice ()
{
  return m_netDevice;
}


Ptr<MobilityModel>
WaveformGenerator::GetMobility ()
{
  return m_mobility;
}


Ptr<const SpectrumModel>
WaveformGenerator::GetRxSpectrumModel () const
{
  // this device is not interested in RX
  return 0;
}

void
WaveformGenerator::SetDevice (Ptr<NetDevice> d)
{
  m_netDevice = d;
}


void
WaveformGenerator::SetMobility (Ptr<MobilityModel> m)
{
  m_mobility = m;
}


void
WaveformGenerator::SetChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_channel = c;
}



void
WaveformGenerator::StartRx (Ptr<SpectrumSignalParameters> params)
{
  NS_LOG_FUNCTION (this << params);
}

void
WaveformGenerator::SetTxPowerSpectralDensity (Ptr<SpectrumValue> txPsd)
{
  NS_LOG_FUNCTION (this << *txPsd);
  m_txPowerSpectralDensity = txPsd;
}

Ptr<AntennaModel>
WaveformGenerator::GetRxAntenna ()
{
  return m_antenna;
}

void
WaveformGenerator::SetAntenna (Ptr<AntennaModel> a)
{
  NS_LOG_FUNCTION (this << a);
  m_antenna = a;
}

void
WaveformGenerator::SetPeriod (Time period)
{
  m_period = period;
}

Time
WaveformGenerator::GetPeriod () const
{
  return m_period;
}



void
WaveformGenerator::SetDutyCycle (double dutyCycle)
{
  m_dutyCycle = dutyCycle;
}

double WaveformGenerator::GetDutyCycle () const
{
  return m_dutyCycle;
}



void
WaveformGenerator::GenerateWaveform ()
{
  NS_LOG_FUNCTION (this);

  Ptr<SpectrumSignalParameters> txParams = Create<SpectrumSignalParameters> ();
  txParams->duration = Time (m_period * m_dutyCycle);
  txParams->psd = m_txPowerSpectralDensity;
  txParams->txPhy = GetObject<SpectrumPhy> ();
  txParams->txAntenna = m_antenna;

  NS_LOG_LOGIC ("generating waveform : " << *m_txPowerSpectralDensity);
  m_phyTxStartTrace (0);
  m_channel->StartTx (txParams);

  if (m_active)
    {
      NS_LOG_LOGIC ("scheduling next waveform");
      Simulator::Schedule (m_period, &WaveformGenerator::GenerateWaveform, this);
    }
}


void
WaveformGenerator::Start ()
{
  NS_LOG_FUNCTION (this);
  if (!m_active)
    {
      NS_LOG_LOGIC ("generator was not active, now starting");
      m_active = true;
      m_startTime = Now ();
      Simulator::ScheduleNow (&WaveformGenerator::GenerateWaveform, this);
    }
}


void
WaveformGenerator::Stop ()
{
  NS_LOG_FUNCTION (this);
  m_active = false;
}


} // namespace ns3
