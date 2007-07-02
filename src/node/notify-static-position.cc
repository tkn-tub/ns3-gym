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
#include "notify-static-position.h"

namespace ns3 {

const InterfaceId NotifyStaticPosition::iid = MakeInterfaceId ("NotifyStaticPosition", StaticPosition::iid);
const ClassId NotifyStaticPosition::cid = 
  MakeClassId<NotifyStaticPosition,double, double> ("NotifyStaticPosition", 
                                                    NotifyStaticPosition::iid);

NotifyStaticPosition::NotifyStaticPosition ()
  : StaticPosition ()
{}
NotifyStaticPosition::NotifyStaticPosition (double x, double y)
  : StaticPosition (x, y, 0.0)
{}
NotifyStaticPosition::NotifyStaticPosition (double x, double y, double z)
  : StaticPosition (x, y, z)
{}

void 
NotifyStaticPosition::RegisterListener (Listener listener)
{
  m_listeners.push_back (listener);
}
void 
NotifyStaticPosition::UnregisterListener (Listener callback)
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
NotifyStaticPosition::NotifyPositionChange (void) const
{
  for (std::list<Listener>::const_iterator i = m_listeners.begin ();
       i != m_listeners.end (); i++)
    {
      Listener listener = *i;
      listener (*this);
    }
}

} // namespace ns3
