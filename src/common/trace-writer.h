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
 *      Thu Feb  8 10:41:40 PST 2007 craigdo: Created
 */

#ifndef TRACE_WRITER_H
#define TRACE_WRITER_H

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include "ns3/callback.h"

namespace ns3 {

class TraceWriter : public std::ostream {
public:
  TraceWriter ();
  TraceWriter (std::string const &filename);
  TraceWriter (char const *filename);

  ~TraceWriter ();

  void Open (std::string const &filename);
  void Open (char const *filename);
  void Close ();

  /**
   * \param String to write to output file
   */
  void Write (std::string const &str);

protected:
  std::ofstream m_filestr;
  void Init (const char *filename);
};

}; // namespace ns3

#endif /* TRACE_WRITER_H */
