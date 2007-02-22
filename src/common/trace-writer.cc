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
 *      Thu Feb  8 10:42:52 PST 2007 craigdo:  Created from pcap-writer.c
 */

#include "ns3/debug.h"
#include "trace-writer.h"

NS_DEBUG_COMPONENT_DEFINE ("TraceWriter");

namespace ns3 {

TraceWriter::TraceWriter () :
  m_filestr()
{
  NS_DEBUG ("TraceWriter()::TraceWriter()");

  std::streambuf *sb = m_filestr.rdbuf();

  NS_DEBUG ("TraceWriter()::TraceWriter():  rdbuf ()");
  rdbuf(sb);

  NS_DEBUG ("TraceWriter()::TraceWriter():  done");
}

  TraceWriter::TraceWriter (std::string const &filename) :
  m_filestr()
{
  NS_DEBUG ("TraceWriter()::TraceWriter (\"" << filename << "\")");

  m_filestr.open (filename.c_str(), std::ios::out | std::ios::app);

  std::streambuf *sb = m_filestr.rdbuf();

  NS_DEBUG ("TraceWriter()::TraceWriter():  rdbuf ()");
  rdbuf(sb);

  NS_DEBUG ("TraceWriter()::TraceWriter():  done");
}

TraceWriter::TraceWriter (char const *filename) :
  m_filestr()
{
  NS_DEBUG ("TraceWriter()::TraceWriter (\"" << filename << "\")");

  m_filestr.open (filename, std::ios::out | std::ios::app);

  std::streambuf *sb = m_filestr.rdbuf();

  NS_DEBUG ("TraceWriter()::TraceWriter():  rdbuf ()");
  rdbuf(sb);

  NS_DEBUG ("TraceWriter()::TraceWriter():  done");
}


TraceWriter::~TraceWriter ()
{
  NS_DEBUG ("TraceWriter()::~TraceWriter()");
}

  void
TraceWriter::Open (std::string const &filename)
{
  NS_DEBUG ("TraceWriter()::Open (\"" << filename << "\")");

  m_filestr.open (filename.c_str(), std::ios::out);
}

  void
TraceWriter::Open (char const *filename)
{
  NS_DEBUG ("TraceWriter()::Open (\"" << filename << "\")");

  m_filestr.open (filename, std::ios::out);
}

  void
TraceWriter::Close ()
{
  NS_DEBUG ("TraceWriter()::Close ()");

  m_filestr.close ();
}

  void
TraceWriter::Write (std::string const &str)
{
  NS_DEBUG ("TraceWriter()::Write (\"" << str << "\")");

  m_filestr << str;
}

}; // namespace ns3
