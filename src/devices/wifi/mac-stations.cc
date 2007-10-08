/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#include "mac-stations.h"

namespace ns3 {

MacStations::MacStations ()
{}

MacStations::~MacStations ()
{
  for (StationsI i = m_stations.begin (); i != m_stations.end (); i++) 
    {
      delete (*i).second;
    }
  m_stations.erase (m_stations.begin (), m_stations.end ());
}

MacStation *
MacStations::Lookup (Mac48Address address)
{
  for (StationsI i = m_stations.begin (); i != m_stations.end (); i++) 
    {
      if ((*i).first == address)
        {
          return (*i).second;
        }
    }
  MacStation *station = CreateStation ();
  m_stations.push_back (std::make_pair (address, station));
  return station;
}

} // namespace ns3

/***************************************************************
 *           MacStation below.
 ***************************************************************/ 

namespace ns3 {

MacStation::MacStation ()
  : m_state (DISASSOC)
{}


MacStation::~MacStation ()
{}

bool 
MacStation::IsAssociated (void) const
{
  return m_state == GOT_ASSOC_TX_OK;
}
bool 
MacStation::IsWaitAssocTxOk (void) const
{
  return m_state == WAIT_ASSOC_TX_OK;
}
void 
MacStation::RecordWaitAssocTxOk (void)
{
  m_state = WAIT_ASSOC_TX_OK;
}
void 
MacStation::RecordGotAssocTxOk (void)
{
  m_state = GOT_ASSOC_TX_OK;
}
void 
MacStation::RecordGotAssocTxFailed (void)
{
  m_state = DISASSOC;
}
void 
MacStation::RecordDisassociated (void)
{
  m_state = DISASSOC;
}

} // namespace ns3

