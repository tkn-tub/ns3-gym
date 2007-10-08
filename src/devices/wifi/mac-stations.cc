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

class NonUnicastMacStation : public MacStation
{
public:
};

} // namespace ns3

namespace ns3 {

MacStations::MacStations ()
  : m_nonUnicast (0)
{}

MacStations::~MacStations ()
{
  for (StationsI i = m_stations.begin (); i != m_stations.end (); i++) 
    {
      delete (*i).second;
    }
  m_stations.clear ();
  delete m_nonUnicast;
}

MacStation *
MacStations::Lookup (Mac48Address address)
{
  if (address.IsBroadcast () ||
      address.IsMulticast ())
    {
      return m_nonUnicast;
    }
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

MacStation *
MacStations::LookupNonUnicast (void)
{
  return m_nonUnicast;
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

