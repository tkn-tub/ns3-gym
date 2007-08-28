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
#include "debug.h"

NS_DEBUG_COMPONENT_DEFINE ("TraceResolver");

namespace ns3 {

TraceResolver::TraceResolver ()
  : m_count (1)
{}

TraceResolver::~TraceResolver ()
{}

void 
TraceResolver::Ref (void)
{
  m_count++;
}
void 
TraceResolver::Unref (void)
{
  m_count--;
  if (m_count == 0)
    {
      NS_DEBUG ("delete "<<this);
      delete this;
    }
}

std::string 
TraceResolver::GetElement (std::string path)
{
  std::string::size_type cur = 1;
  // check that first char is "/"
  std::string::size_type next = path.find ("/", cur);
  std::string id = std::string (path, cur, next-1);
  return id;
}
std::string 
TraceResolver::GetSubpath (std::string path)
{
  std::string::size_type cur = 1;
  // check that first char is "/"
  std::string::size_type next = path.find ("/", cur);
  std::string subpath;
  if (next != std::string::npos)
    {
      subpath = std::string (path, next, std::string::npos);
    }
  else
    {
      subpath = "";
    }
  return subpath;
}

void 
TraceResolver::SourceCollection::AddUnique (std::string path, 
                                            const TraceContext &context,
                                            const TraceDoc &doc)
{
  for (SourceVector::const_iterator i = m_sources.begin (); i != m_sources.end (); i++)
    {
      if (i->path == path &&
          context.IsSimilar (i->context))
        {
          return;
        }
    }
  struct Source source;
  source.path = path;
  source.context = context;
  source.doc = doc;
  m_sources.push_back (source);
}
void 
TraceResolver::SourceCollection::Print (std::ostream &os) const
{
  for (SourceVector::const_iterator i = m_sources.begin (); i != m_sources.end (); i++)
    {
      os << "source=" << i->path << std::endl;
      os << "TraceContext=[";
      i->context.PrintAvailable (os, ",");
      os << "]" << std::endl;
      os << "help=\"" << i->doc.GetHelp () << "\"" << std::endl;
      os << "const TraceContext &: the trace context associated to the connected trace source." << std::endl;
      uint32_t k = 0;
      for (TraceDoc::Iterator j = i->doc.ArgsBegin (); j != i->doc.ArgsEnd (); j++)
        {
          os << "argument " << k << "  --  " << j->first << ": " << j->second << "." << std::endl;
          k++;
        }
      os << std::endl;
    }
}

}//namespace ns3
