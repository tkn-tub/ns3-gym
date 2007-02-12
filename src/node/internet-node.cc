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

#include "net-device-list.h"
#include "l3-demux.h"
#include "ipv4-l3-protocol.h"
#include "ipv4-l4-demux.h"
#include "internet-node.h"
#include "udp.h"
#include "ipv4.h"
#include "arp.h"
#include "udp-ipv4-l4-protocol.h"
#include "arp-l3-protocol.h"

namespace ns3 {

InternetNode::InternetNode()
{
  // Instantiate the capabilities
  m_netDevices = new NetDeviceList();
  m_l3Demux = new L3Demux(this);
  m_ipv4L4Demux = new Ipv4L4Demux(this);
  m_udp = new Udp (this);
  m_ipv4 = new Ipv4 (this);
  m_arp = new Arp (this);
  m_l3Demux->Insert (Ipv4L3Protocol (this));
  m_l3Demux->Insert (ArpL3Protocol (this));
  m_ipv4L4Demux->Insert (UdpIpv4L4Protocol (this));
}

InternetNode::InternetNode (InternetNode const &o)
{
  m_netDevices = new NetDeviceList ();
  m_l3Demux = o.m_l3Demux->Copy (this);
  m_ipv4L4Demux = o.m_ipv4L4Demux->Copy (this);
  m_udp = o.m_udp->Copy (this);
  m_ipv4 = o.m_ipv4->Copy (this);
  m_arp = o.m_arp->Copy (this);
}

// Copy this node
InternetNode* 
InternetNode::Copy() const
{
  InternetNode *copy = new InternetNode (*this);
   return copy;
}


NetDeviceList*   
InternetNode::GetNetDevices() const
{
  return m_netDevices;
}

L3Demux*     
InternetNode::GetL3Demux() const
{
  return m_l3Demux;
}

Ipv4L4Demux*     
InternetNode::GetIpv4L4Demux() const
{
  return m_ipv4L4Demux;
}

}//namespace ns3
