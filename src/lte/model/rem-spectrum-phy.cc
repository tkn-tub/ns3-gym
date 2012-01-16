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
#include <ns3/double.h>
#include <ns3/simulator.h>
#include <ns3/trace-source-accessor.h>
#include <ns3/antenna-model.h>

#include "rem-spectrum-phy.h"

NS_LOG_COMPONENT_DEFINE ("RemSpectrumPhy");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RemSpectrumPhy);

RemSpectrumPhy::RemSpectrumPhy ()
  : m_mobility (0),
    m_netDevice (0),
    m_channel (0),
    m_referenceSignalPower (0),
    m_sumPower (0)
{
  NS_LOG_FUNCTION (this);
}



RemSpectrumPhy::~RemSpectrumPhy ()
{
  NS_LOG_FUNCTION (this);
}

void
RemSpectrumPhy::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_mobility = 0;
  m_netDevice = 0;
  m_channel = 0;
  SpectrumPhy::DoDispose ();
}

TypeId
RemSpectrumPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RemSpectrumPhy")
    .SetParent<SpectrumPhy> ()
    .AddConstructor<RemSpectrumPhy> ()
    .AddAttribute ("NoisePower",
                   "the power of the measuring instrument noise, in Watts. Default to a kT of -174 dBm with a noise figure of 9 dB and a bandwidth of 25 LTE Resource Blocks",
                   DoubleValue (1.4230e-10),
                   MakeDoubleAccessor (&RemSpectrumPhy::m_noisePower),
                   MakeDoubleChecker<double> ())
    ;
  return tid;
}



Ptr<NetDevice>
RemSpectrumPhy::GetDevice ()
{
  return m_netDevice;
}


Ptr<MobilityModel>
RemSpectrumPhy::GetMobility ()
{
  return m_mobility;
}


Ptr<const SpectrumModel>
RemSpectrumPhy::GetRxSpectrumModel () const
{
  return m_spectrumModel;
}

void
RemSpectrumPhy::SetDevice (Ptr<NetDevice> d)
{
  NS_LOG_FUNCTION (this << d);
  m_netDevice = d;
}


void
RemSpectrumPhy::SetMobility (Ptr<MobilityModel> m)
{
  NS_LOG_FUNCTION (this << m);
  m_mobility = m;
}


void
RemSpectrumPhy::SetChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION (this << c);
  m_channel = c;
}

void
RemSpectrumPhy::SetRxSpectrumModel (Ptr<SpectrumModel> m)
{
  NS_LOG_FUNCTION (this << m);
  m_spectrumModel = m;
}

Ptr<AntennaModel>
RemSpectrumPhy::GetRxAntenna ()
{
  return 0;
}


void
RemSpectrumPhy::StartRx (Ptr<SpectrumSignalParameters> params)
{
  NS_LOG_FUNCTION ( this << params);
  double power = Integral (*(params->psd));
  NS_ASSERT_MSG (params->duration.GetMilliSeconds () == 1, 
                 "RemSpectrumPhy works only for LTE signals with duration of 1 ms");
  m_sumPower += power;
  if (power > m_referenceSignalPower)
    {
      m_referenceSignalPower = power;
    }
}

double
RemSpectrumPhy::GetSinr ()
{
  return m_referenceSignalPower / (m_sumPower + m_noisePower);
}


} // namespace ns3
