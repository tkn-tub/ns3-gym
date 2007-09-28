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
#include "ns3/fatal-error.h"


static bool
DoParse (const std::string s, uint64_t *v)
{
  std::string::size_type n = s.find_first_not_of("0123456789.");
  if (n != std::string::npos)
  { // Found non-numeric
    double r = ::atof(s.substr(0, n).c_str());
    std::string trailer = s.substr(n, std::string::npos);
    if (trailer == "bps")
      {
        // Bit/s
        *v = (uint64_t)r;
      }
    else if (trailer == "b/s")
      {
        // Bit/s
        *v = (uint64_t)r;
      }
    else if (trailer == "Bps")
      {
        // Byte/s
        *v = (uint64_t)(r * 8);
      }
    else if (trailer == "B/s") 
      {
        // Byte/s
        *v = (uint64_t)(r * 8);
      }
    else if (trailer == "kbps")
      {
        // KiloBit/s
        *v = (uint64_t)(r * 1000);
      }
    else if (trailer == "kb/s")
      {
        // KiloBit/s
        *v = (uint64_t)(r * 1000);
      }
    else if (trailer == "kBps")
      {
        // KiloBit/s
        *v = (uint64_t)(r * 1000);
      }
    else if (trailer == "kB/s")
      {
        // KiloBit/s
        *v = (uint64_t)(r * 1000);
      }
    else if (trailer == "Mbps")
      {
        // MegaBit/s
        *v = (uint64_t)(r * 1000000);
      }
    else if (trailer == "Mb/s")
      {
        // MegaBit/s
        *v = (uint64_t)(r * 1000000);
      }
    else if (trailer == "MBps")
      {
        // MegaByte/s
        *v = (uint64_t)(r * 8000000);
      }
    else if (trailer == "MB/s")
      {
        // MegaByte/s
        *v = (uint64_t)(r * 8000000);
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
    else
      {
        return false;
      }
    return true;
  }
  *v = ::atoll(s.c_str());
  return true;
}


namespace ns3 {

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
  uint64_t v;
  if (!DoParse (s, &v))
    {
      NS_FATAL_ERROR("Can't Parse data rate "<<s);
    }
  return v;
}

bool DataRate::operator < (const DataRate& rhs)
{
	return m_bps<rhs.m_bps;
}

bool DataRate::operator <= (const DataRate& rhs)
{
	return m_bps<=rhs.m_bps;
}

bool DataRate::operator >  (const DataRate& rhs)
{
	return m_bps>rhs.m_bps;
}

bool DataRate::operator >= (const DataRate& rhs)
{
	return m_bps>=rhs.m_bps;
}

bool DataRate::operator == (const DataRate& rhs)
{
	return m_bps==rhs.m_bps;
}

bool DataRate::operator != (const DataRate& rhs)
{
	return m_bps!=rhs.m_bps;
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


DataRateDefaultValue::DataRateDefaultValue (std::string name,
                                            std::string help,
                                            DataRate defaultValue)
  : DefaultValueBase (name, help),
    m_defaultValue (defaultValue),
    m_value (defaultValue)
{
  DefaultValueList::Add (this);
}
void 
DataRateDefaultValue::SetValue (DataRate rate)
{
  m_value = rate;
}
DataRate 
DataRateDefaultValue::GetValue ()
{
  return m_value;
}
bool 
DataRateDefaultValue::DoParseValue (const std::string &value)
{
  uint64_t v;
  if (DoParse (value, &v))
    {
      m_value = DataRate (v);
      return true;
    }
  return false;
}
std::string 
DataRateDefaultValue::DoGetType (void) const
{
  return "(b/s|kb/s|Mb/s)";
}
std::string 
DataRateDefaultValue::DoGetDefaultValue (void) const
{
  uint64_t defaultValue = m_defaultValue.GetBitRate ();
  std::ostringstream oss;
  if (defaultValue < 1000)
    {
      oss << defaultValue << "b/s";
    }
  else if (defaultValue < 1000000)
    {
      oss << (defaultValue/1000) << "kb/s";
    }
  else 
    {
      oss << (defaultValue/1000) << "Mb/s";
    }
  return oss.str ();
}


};//namespace ns3
