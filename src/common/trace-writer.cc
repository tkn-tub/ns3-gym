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
 *      Thu Feb  8 10:42:52 PST 2007 craigdo:  Created
 */

#include "ns3/debug.h"
#include "trace-writer.h"

namespace ns3 {

namespace {
  int twDebug = 0;
}

  void
TraceWriter::Init (const char *filename)
{
  NS3_TRACE(twDebug, "TraceWriter()::Init(" << filename << ")")

  std::streambuf *sb = m_filestr.rdbuf();
  rdbuf(sb);

  if (filename) 
    {
      m_filestr.open (filename, std::ios::out | std::ios::app);
    }
}

TraceWriter::TraceWriter () :
  m_filestr()
{
  NS3_TRACE(twDebug, "TraceWriter()::TraceWriter()")

  Init (0);
}

TraceWriter::TraceWriter (std::string const &filename) :
  m_filestr()
{
  NS3_TRACE(twDebug, "TraceWriter()::TraceWriter (\"" << filename << "\")")

  Init (filename.c_str());
}

TraceWriter::TraceWriter (char const *filename) :
  m_filestr()
{
  NS3_TRACE(twDebug, "TraceWriter()::TraceWriter (\"" << filename << "\")")

  Init (filename);
}


TraceWriter::~TraceWriter ()
{
  NS3_TRACE(twDebug, "TraceWriter()::~TraceWriter()")
}

  void
TraceWriter::Open (std::string const &filename)
{
  NS3_TRACE(twDebug, "TraceWriter()::Open (\"" << filename << "\")")

  Init(filename.c_str());
}

  void
TraceWriter::Open (char const *filename)
{
  NS3_TRACE(twDebug, "TraceWriter()::Open (\"" << filename << "\")")

  Init(filename);
}

  void
TraceWriter::Close ()
{
  NS3_TRACE(twDebug, "TraceWriter()::Close ()")

  m_filestr.close ();
}

  void
TraceWriter::Write (std::string const &str)
{
  NS3_TRACE(twDebug, "TraceWriter()::Write (\"" << str << "\")")

  m_filestr << str;
}

}; // namespace ns3
