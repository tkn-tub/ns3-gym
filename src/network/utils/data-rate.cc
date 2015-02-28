/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
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
#include "ns3/fatal-error.h"
#include "ns3/log.h"

namespace ns3 {
  
NS_LOG_COMPONENT_DEFINE ("DataRate");

ATTRIBUTE_HELPER_CPP (DataRate);

/* static */
bool
DataRate::DoParse (const std::string s, uint64_t *v)
{
  NS_LOG_FUNCTION (s << v);
  std::string::size_type n = s.find_first_not_of ("0123456789.");
  if (n != std::string::npos)
    { // Found non-numeric
      std::istringstream iss;
      iss.str (s.substr (0, n));
      double r;
      iss >> r;
      std::string trailer = s.substr (n, std::string::npos);
      if (trailer == "bps")
        {
          // bit/s
          *v = (uint64_t)r;
        }
      else if (trailer == "b/s")
        {
          // bit/s
          *v = (uint64_t)r;
        }
      else if (trailer == "Bps")
        {
          // byte/s
          *v = (uint64_t)(r * 8);
        }
      else if (trailer == "B/s")
        {
          // byte/s
          *v = (uint64_t)(r * 8);
        }
      else if (trailer == "kbps")
        {
          // kilobits/s
          *v = (uint64_t)(r * 1000);
        }
      else if (trailer == "kb/s")
        {
          // kilobits/s
          *v = (uint64_t)(r * 1000);
        }
      else if (trailer == "Kbps")
        {
          // kilobits/s
          *v = (uint64_t)(r * 1000);
        }
      else if (trailer == "Kb/s")
        {
          // kilobits/s
          *v = (uint64_t)(r * 1000);
        }
      else if (trailer == "kBps")
        {
          // kiloByte/s
          *v = (uint64_t)(r * 8000);
        }
      else if (trailer == "kB/s")
        {
          // KiloByte/s
          *v = (uint64_t)(r * 8000);
        }
      else if (trailer == "KBps")
        {
          // kiloByte/s
          *v = (uint64_t)(r * 8000);
        }
      else if (trailer == "KB/s")
        {
          // KiloByte/s
          *v = (uint64_t)(r * 8000);
        }
      else if (trailer == "Kib/s")
        {
          // kibibit/s
          *v = (uint64_t)(r * 1024);
        }
      else if (trailer == "KiB/s")
        {
          // kibibyte/s
          *v = (uint64_t)(r * 8192);
        }
      else if (trailer == "Mbps")
        {
          // MegaBits/s
          *v = (uint64_t)(r * 1000000);
        }
      else if (trailer == "Mb/s")
        {
          // MegaBits/s
          *v = (uint64_t)(r * 1000000);
        }
      else if (trailer == "MBps")
        {
          // MegaBytes/s
          *v = (uint64_t)(r * 8000000);
        }
      else if (trailer == "MB/s")
        {
          // MegaBytes/s
          *v = (uint64_t)(r * 8000000);
        }
      else if (trailer == "Mib/s")
        {
          // MebiBits/s
          *v = (uint64_t)(r * 1048576);
        }
      else if (trailer == "MiB/s")
        {
          // MebiByte/s
          *v = (uint64_t)(r * 1048576 * 8);
        }
      else if (trailer == "Gbps")
        {
          // GigaBit/s
          *v = (uint64_t)(r * 1000000000);
        }
      else if (trailer == "Gb/s")
        {
          // GigaBit/s
          *v = (uint64_t)(r * 1000000000);
        }
      else if (trailer == "GBps")
        {
          // GigaByte/s
          *v = (uint64_t)(r * 8*1000000000);
        }
      else if (trailer == "GB/s")
        {
          // GigaByte/s
          *v = (uint64_t)(r * 8*1000000000);
        }
      else if (trailer == "Gib/s")
        {
          // GibiBits/s
          *v = (uint64_t)(r * 1048576 * 1024);
        }
      else if (trailer == "GiB/s")
        {
          // GibiByte/s
          *v = (uint64_t)(r * 1048576 * 1024 * 8);
        }
      else
        {
          return false;
        }
      return true;
    }
  std::istringstream iss;
  iss.str (s);
  iss >> *v;
  return true;
}

DataRate::DataRate ()
  : m_bps (0)
{
  NS_LOG_FUNCTION (this);
}

DataRate::DataRate(uint64_t bps)
  : m_bps (bps)
{
  NS_LOG_FUNCTION (this << bps);
}

bool DataRate::operator < (const DataRate& rhs) const
{
  return m_bps<rhs.m_bps;
}

bool DataRate::operator <= (const DataRate& rhs) const
{
  return m_bps<=rhs.m_bps;
}

bool DataRate::operator >  (const DataRate& rhs) const
{
  return m_bps>rhs.m_bps;
}

bool DataRate::operator >= (const DataRate& rhs) const
{
  return m_bps>=rhs.m_bps;
}

bool DataRate::operator == (const DataRate& rhs) const
{
  return m_bps==rhs.m_bps;
}

bool DataRate::operator != (const DataRate& rhs) const
{
  return m_bps!=rhs.m_bps;
}

double DataRate::CalculateTxTime (uint32_t bytes) const
{
  NS_LOG_FUNCTION (this << bytes);
  return static_cast<double>(bytes)*8/m_bps;
}

Time DataRate::CalculateBytesTxTime (uint32_t bytes) const
{
  NS_LOG_FUNCTION (this << bytes);
  // \todo avoid to use double (if possible).
  return Seconds (static_cast<double>(bytes)*8/m_bps);
}

Time DataRate::CalculateBitsTxTime (uint32_t bits) const
{
  NS_LOG_FUNCTION (this << bits);
  // \todo avoid to use double (if possible).
  return Seconds (static_cast<double>(bits)/m_bps);
}

uint64_t DataRate::GetBitRate () const
{
  NS_LOG_FUNCTION (this);
  return m_bps;
}

DataRate::DataRate (std::string rate)
{
  NS_LOG_FUNCTION (this << rate);
  bool ok = DoParse (rate, &m_bps);
  if (!ok)
    {
      NS_FATAL_ERROR ("Could not parse rate: "<<rate);
    }
}

/* For printing of data rate */
std::ostream &operator << (std::ostream &os, const DataRate &rate)
{
  os << rate.GetBitRate () << "bps";
  return os;
}
/* Initialize a data rate from an input stream */
std::istream &operator >> (std::istream &is, DataRate &rate)
{
  std::string value;
  is >> value;
  uint64_t v;
  bool ok = DataRate::DoParse (value, &v);
  if (!ok)
    {
      is.setstate (std::ios_base::failbit);
    }
  rate = DataRate (v);
  return is;
}

double operator* (const DataRate& lhs, const Time& rhs)
{
  return rhs.GetSeconds ()*lhs.GetBitRate ();
}

double operator* (const Time& lhs, const DataRate& rhs)
{
  return lhs.GetSeconds ()*rhs.GetBitRate ();
}

} // namespace ns3
