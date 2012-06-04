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
 *         Marco Miozzo <mmiozzo@cttc.es>
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

LtePhy::LtePhy ()
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("This constructor should not be called");
}

LtePhy::LtePhy (Ptr<LteSpectrumPhy> dlPhy, Ptr<LteSpectrumPhy> ulPhy)
  : m_downlinkSpectrumPhy (dlPhy),
    m_uplinkSpectrumPhy (ulPhy),
    m_tti (0.001),
    m_ulBandwidth (0),
    m_dlBandwidth (0),
    m_rbgSize (0),
    m_macChTtiDelay (0)
{
  NS_LOG_FUNCTION (this);
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
  NS_LOG_FUNCTION (this);
}

void
LtePhy::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_packetBurstQueue.clear ();
  m_controlMessagesQueue.clear ();
  m_downlinkSpectrumPhy->Dispose ();
  m_downlinkSpectrumPhy = 0;
  m_uplinkSpectrumPhy->Dispose ();
  m_uplinkSpectrumPhy = 0;
  m_netDevice = 0;
  Object::DoDispose ();
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

Ptr<LteSpectrumPhy> 
LtePhy::GetDownlinkSpectrumPhy ()
{
  return m_downlinkSpectrumPhy;
}

Ptr<LteSpectrumPhy> 
LtePhy::GetUplinkSpectrumPhy ()
{
  return m_uplinkSpectrumPhy;
}


void
LtePhy::SetDownlinkChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION (this << c);
  m_downlinkSpectrumPhy->SetChannel (c);
}

void
LtePhy::SetUplinkChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION (this << c);
  m_uplinkSpectrumPhy->SetChannel (c);
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
LtePhy::DoSetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth)
{
  m_ulBandwidth = ulBandwidth;
  m_dlBandwidth = dlBandwidth;

  int Type0AllocationRbg[4] = {
    10,     // RGB size 1
    26,     // RGB size 2
    63,     // RGB size 3
    110     // RGB size 4
  };  // see table 7.1.6.1-1 of 36.213
  for (int i = 0; i < 4; i++)
    {
      if (dlBandwidth < Type0AllocationRbg[i])
        {
          m_rbgSize = i + 1;
          break;
        }
    }
}

void 
LtePhy::DoSetEarfcn (uint16_t dlEarfcn, uint16_t ulEarfcn)
{
  m_dlEarfcn = dlEarfcn;
  m_ulEarfcn = ulEarfcn;
}

uint8_t
LtePhy::GetRbgSize (void) const
{
  return m_rbgSize;
}

void
LtePhy::SetMacPdu (Ptr<Packet> p)
{
  m_packetBurstQueue.at (m_packetBurstQueue.size () - 1)->AddPacket (p);
}

Ptr<PacketBurst>
LtePhy::GetPacketBurst (void)
{
  if (m_packetBurstQueue.at (0)->GetSize () > 0)
    {
      Ptr<PacketBurst> ret = m_packetBurstQueue.at (0)->Copy ();
      m_packetBurstQueue.erase (m_packetBurstQueue.begin ());
      m_packetBurstQueue.push_back (CreateObject <PacketBurst> ());
      return (ret);
    }
  else
    {
      m_packetBurstQueue.erase (m_packetBurstQueue.begin ());
      m_packetBurstQueue.push_back (CreateObject <PacketBurst> ());
      return (0);
    }
}


void
LtePhy::SetControlMessages (Ptr<IdealControlMessage> m)
{
  // In uplink the queue of control messages and packet are of different sizes
  // for avoiding TTI cancellation due to synchronization of subframe triggers
  m_controlMessagesQueue.at (m_controlMessagesQueue.size () - 1).push_back (m);
}

std::list<Ptr<IdealControlMessage> >
LtePhy::GetControlMessages (void)
{
  if (m_controlMessagesQueue.at (0).size () > 0)
    {
      std::list<Ptr<IdealControlMessage> > ret = m_controlMessagesQueue.at (0);
      m_controlMessagesQueue.erase (m_controlMessagesQueue.begin ());
      std::list<Ptr<IdealControlMessage> > newlist;
      m_controlMessagesQueue.push_back (newlist);
      return (ret);
    }
  else
    {
      m_controlMessagesQueue.erase (m_controlMessagesQueue.begin ());
      std::list<Ptr<IdealControlMessage> > newlist;
      m_controlMessagesQueue.push_back (newlist);
      std::list<Ptr<IdealControlMessage> > emptylist;
      return (emptylist);
    }
}


void
LtePhy::DoSetCellId (uint16_t cellId)
{
  m_cellId = cellId;
  m_downlinkSpectrumPhy->SetCellId (cellId);
  m_uplinkSpectrumPhy->SetCellId (cellId);
}


} // namespace ns3
