/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "time-default-value.h"

namespace ns3 {

TimeDefaultValue::TimeDefaultValue (const std::string name,
				    const std::string help,
				    Time defaultValue)
  : DefaultValueBase (name, help),
    m_defaultValue (defaultValue),
    m_value (defaultValue)
{
  DefaultValueList::Add (this);
}
Time 
TimeDefaultValue::GetValue (void) const
{
  return m_value;
}
bool 
TimeDefaultValue::DoParseValue (const std::string &value)
{
  std::string::size_type n = value.find_first_not_of("0123456789.");
  if (n == std::string::npos)
    {
      return false;
    }
  std::string trailer = value.substr(n, std::string::npos);
  std::istringstream iss;
  iss.str (value.substr(0, n));

  if (trailer == std::string("s"))
    {
      double v;
      iss >> v;
      m_value = Seconds (v);
      return !iss.bad () && !iss.fail ();
    }
  uint64_t integer;
  iss >> integer;
  if (iss.bad () || iss.fail ())
    {
      return false;
    }
  if (trailer == std::string("ms"))
    {
      m_value = MilliSeconds (integer);
      return true;
    }
  if (trailer == std::string("us"))
    {
      m_value = MicroSeconds (integer);
      return true;
    }
  if (trailer == std::string("ns"))
    {
      m_value = NanoSeconds (integer);
      return true;
    }
  if (trailer == std::string("ps"))
    {
      m_value = PicoSeconds (integer);
      return true;
    }
  if (trailer == std::string("fs"))
    {
      m_value = FemtoSeconds (integer);
      return true;
    }
  return false;
}
std::string 
TimeDefaultValue::DoGetType (void) const
{
  return "(s|ms|us|ns|ps|fs)";
}
std::string 
TimeDefaultValue::DoGetDefaultValue (void) const
{
  std::ostringstream oss;
  oss << m_value.GetSeconds () << "s";
  return oss.str ();
}


} // namespace ns3
