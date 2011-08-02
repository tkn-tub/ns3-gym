/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#include <ns3/waveform-generator.h>
#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <math.h>
#include <ns3/simulator.h>
#include "ns3/spectrum-error-model.h"
#include "lte-phy.h"
#include "lte-net-device.h"

NS_LOG_COMPONENT_DEFINE ("LtePhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (LtePhy);

uint32_t LtePhy::m_nrFrames = 0;
uint32_t LtePhy::m_nrSubFrames = 0;

LtePhy::LtePhy ()
  : m_netDevice (0),
    m_downlinkSpectrumPhy (0),
    m_uplinkSpectrumPhy (0)
{
  SetTti (0.001);
}


TypeId
LtePhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LtePhy")
    .SetParent<Object> ()
  ;
  return tid;
}


LtePhy::~LtePhy ()
{
}

void
LtePhy::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_downlinkSpectrumPhy = 0;
  m_uplinkSpectrumPhy = 0;
  m_netDevice = 0;
}

void
LtePhy::SetDevice (Ptr<LteNetDevice> d)
{
  NS_LOG_FUNCTION (this << d);
  m_netDevice = d;
}


Ptr<LteNetDevice>
LtePhy::GetDevice ()
{
  NS_LOG_FUNCTION (this);
  return m_netDevice;
}


void
LtePhy::SetDownlinkSpectrumPhy (Ptr<LteSpectrumPhy> s)
{
  NS_LOG_FUNCTION (this << s);
  m_downlinkSpectrumPhy = s;
}


void
LtePhy::SetUplinkSpectrumPhy (Ptr<LteSpectrumPhy> s)
{
  NS_LOG_FUNCTION (this << s);
  m_uplinkSpectrumPhy = s;
}


Ptr<LteSpectrumPhy>
LtePhy::GetDownlinkSpectrumPhy ()
{
  NS_LOG_FUNCTION (this);
  return m_downlinkSpectrumPhy;
}


Ptr<LteSpectrumPhy>
LtePhy::GetUplinkSpectrumPhy ()
{
  NS_LOG_FUNCTION (this);
  return m_uplinkSpectrumPhy;
}


void
LtePhy::SetDownlinkChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION (this << c);
  m_downlinkSpectrumPhy->SetChannel (c);
}


Ptr<SpectrumChannel>
LtePhy::GetDownlinkChannel ()
{
  NS_LOG_FUNCTION (this);
  return m_downlinkSpectrumPhy->GetChannel ();
}


void
LtePhy::SetUplinkChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION (this << c);
  m_uplinkSpectrumPhy->SetChannel (c);
}


Ptr<SpectrumChannel>
LtePhy::GetUplinkChannel ()
{
  NS_LOG_FUNCTION (this);
  return m_uplinkSpectrumPhy->GetChannel ();
}


void
LtePhy::SetDownlinkSubChannels (std::vector<int> mask )
{
  NS_LOG_FUNCTION (this);
  m_listOfDownlinkSubchannel = mask;
  DoSetDownlinkSubChannels ();
}


void
LtePhy::DoSetDownlinkSubChannels ()
{
  NS_LOG_FUNCTION (this);
}


void
LtePhy::SetUplinkSubChannels (std::vector<int> mask )
{
  NS_LOG_FUNCTION (this);
  m_listOfUplinkSubchannel = mask;
  DoSetUplinkSubChannels ();
}


void
LtePhy::DoSetUplinkSubChannels ()
{
  NS_LOG_FUNCTION (this);
}


std::vector<int>
LtePhy::GetDownlinkSubChannels (void)
{
  NS_LOG_FUNCTION (this);
  return m_listOfDownlinkSubchannel;
}


std::vector<int>
LtePhy::GetUplinkSubChannels (void)
{
  NS_LOG_FUNCTION (this);
  return m_listOfUplinkSubchannel;
}


void
LtePhy::SetTxPower (double pw)
{
  NS_LOG_FUNCTION (this << pw);
  m_txPower = pw;
}


double
LtePhy::GetTxPower (void)
{
  NS_LOG_FUNCTION (this);
  return m_txPower;
}

void
LtePhy::SetTti (double tti)
{
  NS_LOG_FUNCTION (this << tti);
  m_tti = tti;
}


double
LtePhy::GetTti (void) const
{
  NS_LOG_FUNCTION (this << m_tti);
  return m_tti;
}

void
LtePhy::SetNrFrames (uint32_t nrFrames)
{
  NS_LOG_FUNCTION (this << nrFrames);
  m_nrFrames = nrFrames;
}


uint32_t
LtePhy::GetNrFrames (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nrFrames;
}


void
LtePhy::SetNrSubFrames (uint32_t nrSubFrames)
{
  NS_LOG_FUNCTION (this << nrSubFrames);
  m_nrSubFrames = nrSubFrames;
}


uint32_t
LtePhy::GetNrSubFrames (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nrSubFrames;
}

} // namespace ns3
