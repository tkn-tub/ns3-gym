/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#include "spectrum-type.h"
#include <ns3/assert.h>


namespace ns3 {



std::vector<std::string> SpectrumTypeFactory::m_names;


SpectrumType
SpectrumTypeFactory::Create (std::string name)
{
  std::vector<std::string>::iterator it;
  for (it = m_names.begin (); it != m_names.end (); ++it)
    {
      NS_ASSERT_MSG (name != *it, "name \"" << name << "\" already registered!");
    }
  m_names.push_back (name);
  return SpectrumType (m_names.size () - 1);
}



std::string
SpectrumTypeFactory::GetNameByUid (uint32_t uid)
{
  return m_names.at (uid);
}



SpectrumType::SpectrumType (uint32_t uid)
  : m_uid (uid)
{
}

uint32_t
SpectrumType::GetUid () const
{
  return m_uid;
}


std::string
SpectrumType::GetName () const
{
  return SpectrumTypeFactory::GetNameByUid (m_uid);
}


bool
operator== (const SpectrumType& lhs, const SpectrumType& rhs)
{
  return (lhs.GetUid () == rhs.GetUid ());
}


bool
operator!= (const SpectrumType& lhs, const SpectrumType& rhs)
{
  return (lhs.GetUid () != rhs.GetUid ());
}

std::ostream&
operator<<  (std::ostream& os, const SpectrumType& rhs)
{
  os << rhs.GetName ();
  return os;
}


} // namespace ns3

