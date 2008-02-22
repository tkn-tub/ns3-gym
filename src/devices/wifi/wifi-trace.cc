/* -*-	Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */
#include "wifi-trace.h"

#include "ns3/trace-context.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/packet.h"
#include "ns3/queue.h"

namespace ns3 {

WifiTrace::WifiTrace (std::string filename)
{
  m_os.open (filename.c_str ());
}

WifiTrace::WifiTrace ()
{}

WifiTrace::~WifiTrace ()
{
  if (m_os.is_open ())
    m_os.close ();
}

void
WifiTrace::TraceAllNetDeviceRx (void)
{
  Packet::EnableMetadata ();
  NodeList::Connect ("/nodes/*/devices/*/rx",
                     MakeCallback (&WifiTrace::LogDevRx, this));
}

void
WifiTrace::TraceAllNetDeviceTx (void)
{
  Packet::EnableMetadata ();
  NodeList::Connect ("/nodes/*/devices/*/tx",
                     MakeCallback (&WifiTrace::LogDevTx, this));
}

void
WifiTrace::TraceAllPhy (void)
{
  Packet::EnableMetadata ();
  NodeList::Connect ("/nodes/*/devices/*/*/state",
                     MakeCallback (&WifiTrace::LogPhy, this));
}

void
WifiTrace::TraceAllErrors (void)
{
  Packet::EnableMetadata ();
  NodeList::Connect ("/nodes/*/devices/*/*/error",
                     MakeCallback (&WifiTrace::LogErrors, this));
}

void
WifiTrace::TraceAckTimeouts (void)
{
  Packet::EnableMetadata ();
  NodeList::Connect ("/nodes/*/devices/*/*/ackTimeout",
                     MakeCallback (&WifiTrace::LogAckTimeout, this));
}

void
WifiTrace::TraceCtsTimeouts (void)
{
  Packet::EnableMetadata ();
  NodeList::Connect ("/nodes/*/devices/*/*/ctsTimeout",
                     MakeCallback (&WifiTrace::LogCtsTimeout, this));
}

void 
WifiTrace::LogDevRx (TraceContext const &context, Ptr<const Packet> p, Mac48Address addr)
{
  if (!m_os.is_open ())
    return;
  m_os << "r " << Simulator::Now ().GetSeconds () << " ";
  uint8_t buf[6];
  addr.CopyTo (buf);
  for (uint8_t i = 0; i < 6; i++) {
    m_os << (buf[i] + 0);
    if (i < 5)
      m_os << ".";
  }
  m_os << " ";
  context.Print (m_os);
  m_os << " pkt-uid=" << p->GetUid () << " ";
  p->Print (m_os);
  m_os << std::endl;  
}

void 
WifiTrace::LogDevTx (TraceContext const &context, Ptr<const Packet> p, Mac48Address addr)
{
  if (!m_os.is_open ())
    return;
  m_os << "s " << Simulator::Now ().GetSeconds () << " ";
  uint8_t buf[6];
  addr.CopyTo (buf);
  for (uint8_t i = 0; i < 6; i++) {
    m_os << (buf[i] + 0);
    if (i < 5)
      m_os << ".";
  }
  m_os << " ";
  context.Print (m_os);
  m_os << " pkt-uid=" << p->GetUid () << " ";
  p->Print (m_os);
  m_os << std::endl;  
}

void
WifiTrace::LogErrors (TraceContext const &context, Ptr<const Packet> p)
{
  if (!m_os.is_open ())
    return;
  m_os << "d " << Simulator::Now ().GetSeconds () << " ";
  context.Print (m_os);
  m_os << " pkt-uid=" << p->GetUid () << " ";
  p->Print (m_os);
  m_os << std::endl;  
}

void 
WifiTrace::LogPhy (TraceContext const &context, Time s, Time d, WifiPhy::State state)
{
  if (!m_os.is_open ())
    return;
  int prec = m_os.precision ();
  m_os.precision (9);
  m_os << "PHY " << Simulator::Now ().GetSeconds () << " ";
  context.Print (m_os);
  switch(state)
  {
    case WifiPhy::SYNC:
      m_os << " SYNC";
      break;
    case WifiPhy::TX:
      m_os << " TX";
      break;
    case WifiPhy::CCA_BUSY:
      m_os << " CCA_BUSY";
      break;
    case WifiPhy::IDLE:
      m_os << " IDLE";
      break;
  }
  m_os << " t=" << s.GetSeconds () << " d=" << d.GetMicroSeconds ();
  m_os << std::endl; 
  m_os.precision (prec);
}

void 
WifiTrace::LogAckTimeout (TraceContext const &context, uint32_t a)
{
  if (!m_os.is_open ())
    return;
  m_os << "ACK timeout " << Simulator::Now ().GetSeconds () << " ";
  context.Print (m_os);
  m_os << " attemps=" << (a+0);
  m_os << std::endl;  
}

void 
WifiTrace::LogCtsTimeout (TraceContext const &context, uint32_t a)
{
  if (!m_os.is_open ())
    return;
  m_os << "CTS timeout " << Simulator::Now ().GetSeconds () << " ";
  context.Print (m_os);
  m_os << " attemps=" << (a+0);
  m_os << std::endl;  
}

}//namespace ns3
