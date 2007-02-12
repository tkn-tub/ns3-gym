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

#ifndef NET_DEVICE_LIST_H
#define NET_DEVICE_LIST_H

#include <vector>

namespace ns3{

class NetDevice;

class NetDeviceList {
public:
  typedef std::vector<NetDevice *>::const_iterator Iterator;

  NetDeviceList();
  ~NetDeviceList();
  // Manage the list
  void Add(NetDevice *device);
  NetDeviceList::Iterator Begin () const;
  NetDeviceList::Iterator End () const;
public:
  typedef std::vector<NetDevice *> NetDevices_t;
  NetDevices_t m_netdevices;
};

} //namespace ns3
#endif

