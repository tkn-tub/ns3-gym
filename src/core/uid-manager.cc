/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "uid-manager.h"
#include "fatal-error.h"
#include "assert.h"


namespace ns3 {


uint32_t 
UidManager::Allocate (std::string name)
{
  for (NameList::iterator i = m_nameList.begin (); i != m_nameList.end (); i++)
    {
      if ((*i) == name)
	{
	  NS_FATAL_ERROR ("Trying to allocate twice the same uid: " << name);
	}
    }
  m_nameList.push_back (name);
  return m_nameList.size ();
}

uint32_t 
UidManager::LookupByName (std::string name)
{
  uint32_t j = 0;
  for (NameList::iterator i = m_nameList.begin (); i != m_nameList.end (); i++)
    {
      j++;
      if ((*i) == name)
        {
          return j;
        }
    }
  return 0;
}

std::string 
UidManager::LookupByUid (uint32_t uid)
{
  NS_ASSERT (uid > 0);
  NS_ASSERT (m_nameList.size () >= uid);
  return m_nameList[uid-1];
}

} // namespace ns3
