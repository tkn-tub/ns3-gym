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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "application-container.h"

namespace ns3 {

ApplicationContainer::Iterator 
ApplicationContainer::Begin (void) const
{
  return m_applications.begin ();
}
ApplicationContainer::Iterator 
ApplicationContainer::End (void) const
{
  return m_applications.end ();
}

uint32_t 
ApplicationContainer::GetN (void) const
{
  return m_applications.size ();
}
Ptr<Application> 
ApplicationContainer::Get (uint32_t i) const
{
  return m_applications[i];
}
void 
ApplicationContainer::Add (ApplicationContainer other)
{
  for (Iterator i = other.Begin (); i != other.End (); i++)
    {
      m_applications.push_back (*i);
    }
}
void 
ApplicationContainer::Add (Ptr<Application> application)
{
  m_applications.push_back (application);
}

void 
ApplicationContainer::Start (Time start)
{
  for (Iterator i = Begin (); i != End (); ++i)
    {
      Ptr<Application> app = *i;
      app->Start (start);
    }
}
void 
ApplicationContainer::Stop (Time stop)
{
  for (Iterator i = Begin (); i != End (); ++i)
    {
      Ptr<Application> app = *i;
      app->Stop (stop);
    }
}


} // namespace ns3
