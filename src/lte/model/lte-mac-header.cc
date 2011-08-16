/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */


#include <ns3/log.h>
#include "lte-mac-header.h"


NS_LOG_COMPONENT_DEFINE ("LteMacHeader");

namespace ns3 {

TypeId
LteMacHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("LteMacHeader")
    .SetParent<Header> ()
    .AddConstructor<LteMacHeader> ()
  ;
  return tid;
}

TypeId
LteMacHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}



uint32_t
LteMacHeader::GetSerializedSize (void) const
{
  return 12;
}



void
LteMacHeader::Serialize (Buffer::Iterator start) const
{
  WriteTo (start, m_destination);
  WriteTo (start, m_source);
}

uint32_t
LteMacHeader::Deserialize (Buffer::Iterator start)
{
  ReadFrom (start, m_destination);
  ReadFrom (start, m_source);
  return GetSerializedSize ();
}


void
LteMacHeader::Print (std::ostream &os) const
{
  os << "src="  << m_source
     << "dst="  << m_destination;
}


void
LteMacHeader::SetSource (Mac48Address source)
{
  m_source = source;
}

Mac48Address
LteMacHeader::GetSource () const
{
  return m_source;
}

void
LteMacHeader::SetDestination (Mac48Address dst)
{
  m_destination = dst;
}

Mac48Address
LteMacHeader::GetDestination () const
{
  return m_destination;
}




} // namespace ns3


