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

#include "data-rate.h"
#include "ns3/nstime.h"

namespace ns3{

DataRate::DataRate(uint64_t bps)
  :m_bps(bps)
{
}

DataRate::DataRate (const std::string s)
  : m_bps(DataRate::Parse(s))
{
}

uint64_t DataRate::Parse(const std::string s)
{
  std::string::size_type n = s.find_first_not_of("0123456789.");
  if (n != std::string::npos)
  { // Found non-numeric
    double r = atof(s.substr(0, n).c_str());
    std::string trailer = s.substr(n, std::string::npos);
    if (trailer == std::string("bps"))
      return (uint64_t)r;                // Bit/s
    if (trailer == std::string("b/s"))
      return (uint64_t)r;                // Bit/s
    if (trailer == std::string("Bps")) 
      return  (uint64_t)(r * 8);          // Byte/s
    if (trailer == std::string("B/s")) 
      return  (uint64_t)(r * 8);          // Byte/s
    if (trailer == std::string("kbps"))
      return  (uint64_t)(r * 1000);       // KiloBit/s
    if (trailer == std::string("kb/s"))
      return  (uint64_t)(r * 1000);       // KiloBit/s
    if (trailer == std::string("kBps"))
      return  (uint64_t)(r * 1000);       // KiloBit/s
    if (trailer == std::string("kB/s"))
      return  (uint64_t)(r * 1000);       // KiloBit/s
    if (trailer == std::string("Mbps"))
      return  (uint64_t)(r * 1000000);    // MegaBit/s
    if (trailer == std::string("Mb/s"))
      return  (uint64_t)(r * 1000000);    // MegaBit/s
    if (trailer == std::string("MBps"))
      return  (uint64_t)(r * 8000000);    // MegaByte/s
    if (trailer == std::string("MB/s"))
      return  (uint64_t)(r * 8000000);    // MegaByte/s
    if (trailer == std::string("Gbps"))
      return  (uint64_t)(r * 1000000000); // GigaBit/s
    if (trailer == std::string("Gb/s"))
      return  (uint64_t)(r * 1000000000); // GigaBit/s
    if (trailer == std::string("GBps"))
      return  (uint64_t)(r * 8*1000000000); // GigaByte/s
    if (trailer == std::string("GB/s"))
      return  (uint64_t)(r * 8*1000000000); // GigaByte/s
    NS_FATAL_ERROR("Can't Parse data rate "<<s);
  }
  return atoll(s.c_str());
}

double DataRate::CalculateTxTime(uint32_t bytes) const
{
  return static_cast<double>(bytes)*8/m_bps;
}

uint64_t DataRate::GetBitRate() const
{
  return m_bps;
}

double operator*(const DataRate& lhs, const Time& rhs)
{
  return rhs.GetSeconds()*lhs.GetBitRate();
}

double operator*(const Time& lhs, const DataRate& rhs)
{
  return lhs.GetSeconds()*rhs.GetBitRate();
}

};//namespace ns3
