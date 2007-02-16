/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 * Author: Craig Dowell <craigdo@ee.washingon.edu>
 *
 *	Thu Feb 15 14:50:46 PST 2007 craigdo: Created.
 */

#include "ns3/debug.h"
#include "channel.h"

namespace ns3 {

namespace {
  int chDebug = 1;
};

Channel::Channel ()
{
  NS3_TRACE(chDebug, "Channel::Channel ()")
}

Channel::~Channel ()
{
  NS3_TRACE(chDebug, "Channel::~Channel ()")
}

  bool
Channel::Connect (PhysicalLayer &phys)
{
  NS3_TRACE(chDebug, "Channel::Connect (" << &phys << ")")
  m_physList.push_back(static_cast<PhysicalLayer *>(&phys));
  return true;
}

  bool
Channel::Propagate (Propagator &propagator)
{
  NS3_TRACE(chDebug, "Channel::Propagate (" << &propagator << ")")

  for (PhysicalLayerList::const_iterator i = m_physList.begin ();
       i != m_physList.end (); 
       i++)
    {
      (*i)->Receive (propagator);
    }

  return true;
}

} // namespace ns3
