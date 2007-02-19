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

NS_DEBUG_COMPONENT_DEFINE ("Channel");

namespace ns3 {

Channel::Channel ()
{
  NS_DEBUG("Channel::Channel ()")
}

Channel::~Channel ()
{
  NS_DEBUG("Channel::~Channel ()")
}

  bool
Channel::DoConnectToUpper (LayerConnectorUpper &upper)
{
  NS_DEBUG("Channel::DoConnectToUpper (" << &upper << ")")
  m_connectorList.push_back(&upper);

  return true;
}

  bool
Channel::LowerDoNotify (LayerConnectorUpper *upper)
{
  NS_DEBUG("Channel::LowerDoNotify ()")

  Packet p;

  NS_DEBUG("Channel::LowerDoNotify (): Starting pull")

  upper->UpperPull(p);

  NS_DEBUG("Channel::LowerDoNotify (): Got bits,  Propagate()")

  return Propagate(p);
}

  bool
Channel::Propagate (Packet &p)
{
  NS_DEBUG("Channel::Propagate (" << &p << ")")

  for (ConnectorList::const_iterator i = m_connectorList.begin ();
       i != m_connectorList.end (); 
       i++)
    {
      (*i)->UpperSendUp (p);
    }

  return true;
}

} // namespace ns3
