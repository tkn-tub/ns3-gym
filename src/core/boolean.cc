/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "boolean.h"
#include "fatal-error.h"

namespace ns3 {

Boolean::Boolean ()
  : m_value (false)
{}
Boolean::Boolean (bool value)
  : m_value (value)
{}
void 
Boolean::Set (bool value)
{
  m_value = value;
}
bool 
Boolean::Get (void) const
{
  return m_value;
}
Boolean::operator bool () const
{
  return m_value;
}

std::ostream & operator << (std::ostream &os, const Boolean &value)
{
  if (value.Get ())
    {
      os << "true";
    }
  else
    {
      os << "false";
    }
  return os;
}
std::istream & operator >> (std::istream &is, Boolean &value)
{
  std::string v;
  is >> v;
  if (v == "true" ||
      v == "1" ||
      v == "t")
    {
      value.Set (true);
    }
  else if (v == "false" ||
	   v == "0" ||
	   v == "f")
    {
      value.Set (false);
    }
  else
    {
      is.setstate (std::ios_base::badbit);
    }  
  return is;
}

ATTRIBUTE_CONVERTER_IMPLEMENT (Boolean);
ATTRIBUTE_VALUE_IMPLEMENT (Boolean);
ATTRIBUTE_CHECKER_IMPLEMENT (Boolean);

} // namespace ns3
