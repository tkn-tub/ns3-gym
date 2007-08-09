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
#include "trace-resolver.h"

namespace ns3 {


TraceResolver::~TraceResolver ()
{}

void 
TraceResolver::Connect (std::string path, CallbackBase const &cb, const TraceContext &context)
{
  std::string::size_type cur = 1;
  // check that first char is "/"
  std::string::size_type next = path.find ("/", cur);
  std::string element = std::string (path, cur, next-1);
  TraceResolverList resolverList = DoLookup (element);
  for (TraceResolverList::iterator i = resolverList.begin (); i != resolverList.end (); i++)
    {
      TraceResolver *resolver = i->first;
      TraceContext tmp = context;
      tmp.Add (i->second);
      if (next == std::string::npos) 
	{
	  // we really break the recursion here.
	  resolver->DoConnect (cb, tmp);
	}
      else
	{
	  std::string subpath = std::string (path, next, std::string::npos);
          resolver->Connect (subpath, cb, tmp);
	}
      delete resolver;
    }
  resolverList.erase (resolverList.begin (), resolverList.end ());
}

void 
TraceResolver::Disconnect (std::string path, CallbackBase const &cb)
{
  std::string::size_type cur = 1;
  // check that first char is "/"
  std::string::size_type next = path.find ("/", cur);
  std::string element = std::string (path, cur, next-1);
  TraceResolverList resolverList = DoLookup (element);
  for (TraceResolverList::iterator i = resolverList.begin (); i != resolverList.end (); i++)
    {
      TraceResolver *resolver = i->first;
      if (next == std::string::npos) 
	{
	  // we really break the recursion here.
	  resolver->DoDisconnect (cb);
	}
      else
	{
	  std::string subpath = std::string (path, next, std::string::npos);
          resolver->Disconnect (subpath, cb);
	}
      delete resolver;
    }
  resolverList.erase (resolverList.begin (), resolverList.end ());
}

TraceResolver::TraceResolverList 
TraceResolver::DoLookup (std::string id) const
{
  return TraceResolverList ();
}
void 
TraceResolver::DoConnect (CallbackBase const &cb, const TraceContext &context)
{}

void 
TraceResolver::DoDisconnect (CallbackBase const &cb)
{}


}//namespace ns3
