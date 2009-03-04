/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "random-stream.h"
#include "ns3/assert.h"

#include <cmath>

namespace ns3 {

RandomStream::~RandomStream ()
{}


RealRandomStream::RealRandomStream ()
  : m_stream (UniformVariable ())
{}
uint32_t 
RealRandomStream::GetNext (uint32_t min, uint32_t max)
{
  return m_stream.GetInteger (min, max);
}


void 
TestRandomStream::AddNext (uint32_t v)
{
  m_nexts.push_back (v);
}

uint32_t 
TestRandomStream::GetNext (uint32_t min, uint32_t max)
{
  NS_ASSERT (!m_nexts.empty ());
  uint32_t next = m_nexts.front ();
  m_nexts.pop_front ();
  return next;
}

} // namespace ns3
