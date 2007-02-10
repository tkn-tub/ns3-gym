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
#include "ipv4-l4-demux.h"
#include "internet-node.h"
#include "udp.h"
#include "ipv4.h"

namespace ns3 {

InternetNode::InternetNode()
{
  // Instantiate the capabilities
  m_netDevices = new NetDeviceList();
  m_l3Demux = new L3Demux();
  m_ipv4L4Demux = new Ipv4L4Demux();
  // add an ipv4 protocol handler.
  Ipv4 ipv4;
  m_l3Demux->Insert (ipv4);
  // add a udp protocol handler.
  Udp udp = Udp (this);
  m_ipv4L4Demux->Insert (udp);
}

InternetNode::InternetNode(const InternetNode& rhs)
{ // Copy constructor
  // Note we do not copy the contents of the process list or
  // the interfaces list, as these are added later.
  m_netDevices = new NetDeviceList();
  // Make a copy of each capability
  m_l3Demux  = rhs.GetL3Demux()->Copy();
  m_ipv4L4Demux  = rhs.GetIpv4L4Demux()->Copy();
}

// Copy this node
InternetNode* 
InternetNode::Copy() const
{
  return new InternetNode(*this);
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
