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
// Manages the list of network device interfaces associated with a node.
// George F. Riley, Georgia Tech, Spring 2007

#include "net-device-list.h"

namespace ns3 {

NetDeviceList::NetDeviceList()
{}
NetDeviceList::~NetDeviceList()
{
  // XXX shall I delete each NetDevice ?
}
void
NetDeviceList::Add(NetDevice *device)
{
  m_netdevices.push_back (device);
}
NetDeviceList::Iterator 
NetDeviceList::Begin () const
{
  return m_netdevices.begin ();
}
NetDeviceList::Iterator 
NetDeviceList::End () const
{
  return m_netdevices.end ();
}

}//namespace ns3
