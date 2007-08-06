/* -*-	Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef ASCII_TRACE_H
#define ASCII_TRACE_H

#include <string>
#include <fstream>

namespace ns3 {

class Packet;
class TraceContext;

class AsciiTrace 
{
public:
  AsciiTrace (std::string filename);
  ~AsciiTrace ();
  void TraceAllQueues (void);
  void TraceAllNetDeviceRx (void);
private:
  void LogDevQueue (TraceContext const &context, const Packet &p);
  void LogDevRx (TraceContext const &context, Packet &p);
  std::ofstream m_os;
};

}//namespace ns3

#endif /* ASCII_TRACE_H */
