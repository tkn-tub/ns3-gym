/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "position-set-notifier.h"

namespace ns3 {

const InterfaceId PositionSetNotifier::iid = MakeInterfaceId ("PositionSetNotifier", Object::iid);
const ClassId PositionSetNotifier::cid = 
  MakeClassId<PositionSetNotifier> ("PositionSetNotifier", 
				    PositionSetNotifier::iid);

PositionSetNotifier::PositionSetNotifier ()
{}

void 
PositionSetNotifier::RegisterListener (Listener listener)
{
  m_listeners.push_back (listener);
}
void 
PositionSetNotifier::UnregisterListener (Listener callback)
{
  for (std::list<Listener>::iterator i = m_listeners.begin ();
       i != m_listeners.end ();)
    {
      Listener listener = *i;
      if (listener.IsEqual (callback))
	{
	  i = m_listeners.erase (i);
	}
      else
	{
	  i++;
	}
    }  
}
void 
PositionSetNotifier::Notify (Ptr<const Position> position) const
{
  for (std::list<Listener>::const_iterator i = m_listeners.begin ();
       i != m_listeners.end (); i++)
    {
      Listener listener = *i;
      listener (position);
    }
}

} // namespace ns3
