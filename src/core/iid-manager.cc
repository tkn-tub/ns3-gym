/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "iid-manager.h"
#include "ns3/fatal-error.h"
#include <vector>

namespace ns3 {

class IidManagerPriv
{
public:
  uint32_t Allocate (std::string name);
private:
  typedef std::vector<std::string> NameList;
  NameList m_nameList;
};

uint32_t 
IidManagerPriv::Allocate (std::string name)
{
  for (NameList::iterator i = m_nameList.begin (); i != m_nameList.end (); i++)
    {
      if ((*i) == name)
	{
	  NS_FATAL_ERROR ("Trying to allocate twice the same iid: " << name);
	}
    }
  m_nameList.push_back (name);
  return m_nameList.size ();
}

uint32_t 
IidManager::Allocate (std::string name)
{
  static IidManagerPriv priv;
  return priv.Allocate (name);
}

} // namespace ns3
