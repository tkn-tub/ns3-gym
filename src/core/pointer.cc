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
#include "pointer.h"

namespace ns3 {

Pointer::Pointer ()
  : m_value ()
{}

Pointer::Pointer (Ptr<Object> object)
  : m_value (object)
{}

void 
Pointer::SetObject (Ptr<Object> object)
{
  m_value = object;
}

Ptr<Object> 
Pointer::GetObject (void) const
{
  return m_value;
}

ATTRIBUTE_VALUE_IMPLEMENT (Pointer);
ATTRIBUTE_CONVERTER_IMPLEMENT (Pointer);

std::ostream & operator << (std::ostream &os, const Pointer &pointer)
{
  os << pointer.GetObject ();
  return os;
}
std::istream & operator >> (std::istream &is, Pointer &pointer)
{
  // XXX: This cannot work;
  return is;
}


} // namespace ns3
