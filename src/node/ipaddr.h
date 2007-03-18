// -*- Mode:NS3 -*-
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
// NS3 Base class for IPV4 and IPV6 addresses, and masks.
// George F. Riley, Georgia Tech, Spring 2007

#ifndef __IPADDR_H__
#define __IPADDR_H__

#include <string>

#include "protocol.h"
namespace ns3 {

class IPMask 
{
public:
  virtual ~IPMask();
  virtual uint32_t Size() const = 0;       // Size of mask in bytes
  virtual IPMask*  Copy() const = 0;       // Copy this mask
  virtual Proto_t  L3Proto() const = 0;    // Associated layer 3 protocol number
};
  
class IPAddr {
public:
  virtual ~IPAddr() {}
  virtual uint32_t Size() const = 0;       // Number of bytes in this address
  virtual IPAddr*  Copy() const = 0;       // Copy this address
  virtual Proto_t  L3Proto() const = 0;    // Associated layer 3 protocol number
  virtual std::string ToString() const = 0;// Convert to dotted notation
  virtual operator std::string() const = 0;// For convenience, a string typecast
  virtual IPMask&  GetMask(uint32_t) const;// Get a compatible mask
};

} //namespace ns3

#endif
