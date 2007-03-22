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

#include "ns3/fatal-error.h"
#include <string>
#include <iostream>
#include <stdint.h>
#include "ns3/nstime.h"

namespace ns3{

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
 */
class DataRate
{
  public:
  /**
   * \brief Integer constructor
   *
   * Construct a data rate from an integer.  This class only supports positive
   * integer data rates in units of bits/s, meaning 1bit/s is the smallest 
   * non-trivial bitrate availiable.
   */
  DataRate (uint64_t bps);
  
  /**
   * \brief String constructor
   *
   * Construct a DataRate from a string.  The supported strings have a 
   * numerical portion, followed by units in the following format:
   * - Prefix: nothing, "k", "M", "G"
   * - Data Unit: "b, "B"
   * - Time Suffix: "ps", "/s"
   * The prefixes are SI powers of 10 (10^0,10^3,10^6,10^9 respectively).\n
   * The units are the bit, and the (8-bit) byte respectively.\n
   * Both time suffixes denote "per second".  Some supported examples include
   * "20B/s", "56kbps", "4.4MB/s", and "100Gb/s".  Any malformed string causes
   * a fatal error.
   */
  DataRate (const std::string s);
  
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
  
  private:
  uint64_t m_bps;
  static uint64_t Parse(const std::string);
};
/**
 * \param lhs
 * \param rhs
 * \return Bits transmitted in rhs seconds at lhs b/s
 */
double operator*(const DataRate& lhs, const TimeUnit<1>& rhs);
double operator*(const TimeUnit<1>& lhs, const DataRate& rhs);

};//namespace ns3
