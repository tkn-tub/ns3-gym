// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//
// Implementation of the InternetNode class for ns3.
// George F. Riley, Georgia Tech, Fall 2006

#include "ns3/composite-trace-resolver.h"

#include "application-list.h"
#include "l3-demux.h"
#include "ipv4-l4-demux.h"
#include "internet-node.h"
#include "udp.h"
#include "ipv4.h"
#include "arp.h"
#include "net-device.h"

namespace ns3 {


InternetNode::InternetNode()
{
  // Instantiate the capabilities
  m_applicationList = new ApplicationList(this);
  m_l3Demux = new L3Demux(this);
  m_ipv4L4Demux = new Ipv4L4Demux(this);
  m_l3Demux->Insert (Ipv4 (this));
  m_l3Demux->Insert (Arp (this));
  m_ipv4L4Demux->Insert (Udp (this));
}

InternetNode::~InternetNode ()
{
  Dispose ();
}

void
InternetNode::SetName (std::string name)
{
  m_name = name;
}

TraceResolver *
InternetNode::CreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  Ipv4 *ipv4 = GetIpv4 ();
  resolver->Add ("ipv4",
                 MakeCallback (&Ipv4::CreateTraceResolver, ipv4),
                 InternetNode::IPV4);
  ipv4->Unref ();

  Arp *arp = GetArp ();
  resolver->Add ("arp",
                 MakeCallback (&Arp::CreateTraceResolver, arp),
                 InternetNode::ARP);
  arp->Unref ();

  Udp *udp = GetUdp ();
  resolver->Add ("udp",
                 MakeCallback (&Udp::CreateTraceResolver, udp),
                 InternetNode::UDP);
  udp->Unref ();

  return resolver;
}

void InternetNode::Dispose()
{
  if (m_l3Demux != 0)
    {
      m_l3Demux->Dispose ();
      m_l3Demux->Unref ();
      m_l3Demux = 0;
    }
  if (m_ipv4L4Demux != 0)
    {
      m_ipv4L4Demux->Dispose ();
      m_ipv4L4Demux->Unref ();
      m_ipv4L4Demux = 0;
    }

  if (m_applicationList != 0)
    {
      m_applicationList->Dispose ();
      m_applicationList->Unref ();
      m_applicationList = 0;
    }

  // chain up.
  Node::Dispose ();
}

ApplicationList* 
InternetNode::GetApplicationList() const
{ 
  m_applicationList->Ref ();
  return m_applicationList;
} 

L3Demux*     
InternetNode::GetL3Demux() const
{
  m_l3Demux->Ref ();
  return m_l3Demux;
}

Ipv4L4Demux*     
InternetNode::GetIpv4L4Demux() const
{
  m_ipv4L4Demux->Ref ();
  return m_ipv4L4Demux;
}

Ipv4 *
InternetNode::GetIpv4 (void) const
{
  Ipv4 *ipv4 = static_cast<Ipv4*> (m_l3Demux->PeekProtocol (Ipv4::PROT_NUMBER));
  ipv4->Ref ();
  return ipv4;
}
Udp *
InternetNode::GetUdp (void) const
{
  Udp *udp = static_cast<Udp*> (m_ipv4L4Demux->PeekProtocol (Udp::PROT_NUMBER));
  udp->Ref ();
  return udp;
}

Arp *
InternetNode::GetArp (void) const
{
  Arp *arp = static_cast<Arp*> (m_l3Demux->PeekProtocol (Arp::PROT_NUMBER));
  arp->Ref ();
  return arp;
}

void 
InternetNode::DoAddDevice (NetDevice *device) const
{
  device->SetReceiveCallback (MakeCallback (&InternetNode::ReceiveFromDevice, this));
}

bool
InternetNode::ReceiveFromDevice (NetDevice *device, const Packet &p, uint16_t protocolNumber) const
{
  L3Protocol *target = GetL3Demux()->PeekProtocol (protocolNumber);
  if (target != 0) 
    {
      Packet packet = p;
      target->Receive(packet, device);
      return true;
    }
  return false;
}


}//namespace ns3
