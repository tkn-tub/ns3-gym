/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
#include <list>
#include <utility>
#include <iostream>
#include "log.h"
#include "assert.h"
#include "ns3/core-config.h"
#include "fatal-error.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

namespace ns3 {

typedef std::list<std::pair <std::string, LogComponent *> > ComponentList;
typedef std::list<std::pair <std::string, LogComponent *> >::iterator ComponentListI;

static 
ComponentList *GetComponentList (void)
{
  static ComponentList components;
  return &components;
}

void
LogComponentEnableEnvVar (void)
{
  static bool isFirstLog = true;
  if (!isFirstLog)
    {
      return;
    }
#ifdef HAVE_GETENV
  char *envVar = getenv("NS_LOG");
  if (envVar == 0)
    {
      isFirstLog = false;
      return;
    }
  std::string env = envVar;
  if (env == "print-list")
    {
      LogComponentPrintList ();
      isFirstLog = false;
      return;
    }
  bool allFound = true;
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
      std::string::size_type equal = tmp.find ("=");
      std::string component;
      int level;
      if (equal == std::string::npos)
        {
          component = tmp;
          level = LOG_LEVEL_DEBUG;
        }
      else
        {
          component = tmp.substr (0, equal);
          std::string::size_type cur_lev;
          std::string::size_type next_lev = equal;
          do
            {
              cur_lev = next_lev + 1;
              next_lev = tmp.find ("|", cur_lev);
              std::string lev = tmp.substr (cur_lev, next_lev - cur_lev);
              if (lev == "error")
                {
                  level |= LOG_LEVEL_ERROR;
                }
              else if (lev == "warn")
                {
                  level |= LOG_LEVEL_WARN;
                }
              else if (lev == "debug")
                {
                  level |= LOG_LEVEL_DEBUG;
                }
              else if (lev == "info")
                {
                  level |= LOG_LEVEL_INFO;
                }
              else if (lev == "function")
                {
                  level |= LOG_LEVEL_FUNCTION;
                }
              else if (lev == "param")
                {
                  level |= LOG_LEVEL_PARAM;
                }
              else if (lev == "logic")
                {
                  level |= LOG_LEVEL_LOGIC;
                }
              else if (lev == "all")
                {
                  level |= LOG_LEVEL_ALL;
                }
            } while (next_lev != std::string::npos);
        }
      bool found = false;
      ComponentList *components = GetComponentList ();
      for (ComponentListI i = components->begin ();
           i != components->end ();
           i++)
        {
          if (i->first.compare (component) == 0)
            {
              found = true;
              
              i->second->Enable ((enum LogLevel)level);
              break;
            }
        }
      if (!found)
        {
          allFound = false;
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
  if (allFound)
    {
      isFirstLog = false;
    }
  
#endif
}

LogComponent::LogComponent (char const * name)
  : m_levels (0)
{
  ComponentList *components = GetComponentList ();
  for (ComponentListI i = components->begin ();
       i != components->end ();
       i++)
    {
      NS_ASSERT (i->first != name);
    }
  components->push_back (std::make_pair (name, this));
}

bool 
LogComponent::IsEnabled (enum LogLevel level) const
{
  LogComponentEnableEnvVar ();
//  return (level & m_levels) ? 1 : 0;

  return m_levels >= level;
}

bool
LogComponent::IsNoneEnabled (void) const
{
  return m_levels == 0;
}

void 
LogComponent::Enable (enum LogLevel level)
{
  m_levels |= level;
}

void 
LogComponent::Disable (enum LogLevel level)
{
  m_levels &= ~level;
}

void 
LogComponentEnable (char const *name, enum LogLevel level)
{
  ComponentList *components = GetComponentList ();
  for (ComponentListI i = components->begin ();
       i != components->end ();
       i++)
    {
      if (i->first.compare (name) == 0) 
	{
	  i->second->Enable (level);
	  break;
	}
    }  
}

void 
LogComponentDisable (char const *name, enum LogLevel level)
{
  ComponentList *components = GetComponentList ();
  for (ComponentListI i = components->begin ();
       i != components->end ();
       i++)
    {
      if (i->first.compare (name) == 0) 
	{
	  i->second->Disable (level);
	  break;
	}
    }  
}

void 
LogComponentPrintList (void)
{
  ComponentList *components = GetComponentList ();
  for (ComponentListI i = components->begin ();
       i != components->end ();
       i++)
    {
      std::cout << i->first << "=";
      if (i->second->IsNoneEnabled ())
        {
          std::cout << "0" << std::endl;
          continue;
        }
      if (i->second->IsEnabled (LOG_LEVEL_ERROR))
        {
          std::cout << "error";
        }
      if (i->second->IsEnabled (LOG_LEVEL_WARN))
        {
          std::cout << "|warn";
        }
      if (i->second->IsEnabled (LOG_LEVEL_DEBUG))
        {
          std::cout << "|debug";
        }
      if (i->second->IsEnabled (LOG_LEVEL_INFO))
        {
          std::cout << "|info";
        }
      if (i->second->IsEnabled (LOG_LEVEL_FUNCTION))
        {
          std::cout << "|function";
        }
      if (i->second->IsEnabled (LOG_LEVEL_PARAM))
        {
          std::cout << "|param";
        }
      if (i->second->IsEnabled (LOG_LEVEL_LOGIC))
        {
          std::cout << "|logic";
        }
      if (i->second->IsEnabled (LOG_LEVEL_ALL))
        {
          std::cout << "|all";
        }
      std::cout << std::endl;
    }
}

} // namespace ns3


