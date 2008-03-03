/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
//

#ifndef DATA_RATE_H
#define DATA_RATE_H

#include <string>
#include <iostream>
#include <stdint.h>
#include "ns3/nstime.h"
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"

namespace ns3 {

/**
 * \brief Class for representing data rates
 *
 * Allows for natural and familiar use of data rates.  Allows construction
 * from strings, natural multiplication e.g.:
 * \code
 * DataRate x("56kbps");
 * double nBits = x*ns3::Seconds(19.2);
 * uint32_t nBytes = 20;
 * double txtime = x.CalclulateTxTime(nBytes);
 * \endcode
 * This class also supports the regular comparison operators <, >, <=, >=, ==,
 * and !=
 */
class DataRate
{
public:
  DataRate ();
  /**
   * \brief Integer constructor
   *
   * Construct a data rate from an integer.  This class only supports positive
   * integer data rates in units of bits/s, meaning 1bit/s is the smallest 
   * non-trivial bitrate availiable.
   */
  DataRate (uint64_t bps);
  DataRate (std::string rate);
    
  bool operator <  (const DataRate& rhs) const;
  bool operator <= (const DataRate& rhs) const;
  bool operator >  (const DataRate& rhs) const;
  bool operator >= (const DataRate& rhs) const;
  bool operator == (const DataRate& rhs) const;
  bool operator != (const DataRate& rhs) const;
  
  /**
   * \brief Calculate transmission time
   *
   * Calculates the transmission time at this data rate
   * \param bytes The number of bytes (not bits) for which to calculate
   * \return The tranmission time in seconds for the number of bytes specified
   */
  double CalculateTxTime(uint32_t bytes) const;
  
  /**
   * Get the underlying bitrate
   * \return The underlying bitrate in bits per second
   */
  uint64_t GetBitRate() const;

  VALUE_HELPER_HEADER_1 (DataRate);
private:
  uint64_t m_bps;
  static uint64_t Parse(const std::string);
};

std::ostream &operator << (std::ostream &os, const DataRate &rate);
std::istream &operator >> (std::istream &is, DataRate &rate);

VALUE_HELPER_HEADER_2 (DataRate);

/**
 * \param lhs
 * \param rhs
 * \return Bits transmitted in rhs seconds at lhs b/s
 */
double operator*(const DataRate& lhs, const TimeUnit<1>& rhs);
double operator*(const TimeUnit<1>& lhs, const DataRate& rhs);

} //namespace ns3

#endif /* DATA_RATE_H */
