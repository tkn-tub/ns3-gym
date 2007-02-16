/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include <cassert>
#include <list>
#include <utility>
#include <iostream>
#include "debug.h"
#include "ns3/core-config.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

namespace ns3 {

typedef std::list<std::pair <std::string, DebugComponent *> > ComponentList;
typedef std::list<std::pair <std::string, DebugComponent *> >::iterator ComponentListI;

static ComponentList g_components;
static bool g_firstDebug = true;

void
DebugComponentEnableEnvVar (void)
{
#ifdef HAVE_GETENV
  char *envVar = getenv("NS3_DEBUG");
  if (envVar == 0)
    {
      return;
    }
  std::string env = envVar;
  std::string::size_type cur = 0;
  std::string::size_type next = 0;
  while (true)
    {
      next = env.find_first_of (";", cur);
      std::string tmp = std::string (env, cur, next);
      {
        /* The following code is a workaround for a bug in the g++
         * c++ string library. Its goal is to remove any trailing ';'
         * from the string even though there should not be any in
         * it. This code should be safe even if the bug is not there.
         */
        std::string::size_type trailing = tmp.find_first_of (";");
        tmp = tmp.substr (0, trailing);
      }
      if (tmp.size () == 0)
        {
          break;
        }
      bool found = false;
      for (ComponentListI i = g_components.begin ();
           i != g_components.end ();
           i++)
        {
          if (i->first.compare (tmp) == 0)
            {
              found = true;
              i->second->Enable ();
              break;
            }
        }
      if (!found)
        {
          std::cout << "No debug component named=\"" << tmp << "\"" << std::endl;
        }
      if (next == std::string::npos)
        {
          break;
        }
      cur = next + 1;
      if (cur >= env.size ()) 
        {
          break;
        }
    }
#endif
}


DebugComponent::DebugComponent (char const * name)
  : m_isEnabled (false)
{
  for (ComponentListI i = g_components.begin ();
       i != g_components.end ();
       i++)
    {
      assert (i->first.compare (name) != 0);
    }
  g_components.push_back (std::make_pair (name, this));
}
bool 
DebugComponent::IsEnabled (void)
{
  if (g_firstDebug) {
    DebugComponentEnableEnvVar ();
    g_firstDebug = false;
  }
  return m_isEnabled;
}
void 
DebugComponent::Enable (void)
{
  m_isEnabled = true;
}
void 
DebugComponent::Disable (void)
{
  m_isEnabled = false;
}

void 
DebugComponentEnable (char const *name)
{
  for (ComponentListI i = g_components.begin ();
       i != g_components.end ();
       i++)
    {
      if (i->first.compare (name) == 0) 
	{
	  i->second->Enable ();
	  break;
	}
    }  
}
void 
DebugComponentDisable (char const *name)
{
  for (ComponentListI i = g_components.begin ();
       i != g_components.end ();
       i++)
    {
      if (i->first.compare (name) == 0) 
	{
	  i->second->Disable ();
	  break;
	}
    }  
}


void 
DebugComponentPrintList (void)
{
  for (ComponentListI i = g_components.begin ();
       i != g_components.end ();
       i++)
    {
      std::cout << i->first << "=" << (i->second->IsEnabled ()?"enabled":"disabled") << std::endl;
    }
}

}; // namespace ns3


