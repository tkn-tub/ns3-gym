/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 INRIA
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
 * Author: Guillaume Seguin <guillaume@segu.in>
 */
#include "ascii-writer.h"

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/simulation-singleton.h"
#include "ns3/packet.h"

#include <fstream>
#include <map>


NS_LOG_COMPONENT_DEFINE ("AsciiWriter");

namespace ns3 {

typedef std::map<std::ostream*, Ptr<AsciiWriter> > AsciiWritersMap;

Ptr<AsciiWriter>
AsciiWriter::Get (std::ostream &os)
{
  AsciiWritersMap *map = SimulationSingleton<AsciiWritersMap>::Get ();
  Ptr<AsciiWriter> writer = (*map)[&os];
  if (writer == 0)
    {
      // don't call Create<> because constructor is private
      writer = Ptr<AsciiWriter> (new AsciiWriter (&os), false);
      (*map)[&os] = writer;
    }
  return writer;
}

AsciiWriter::AsciiWriter (std::ostream *os)
  : m_writer (os)
{
  NS_LOG_FUNCTION (this);
}

AsciiWriter::~AsciiWriter (void)
{
  NS_LOG_FUNCTION (this);
}

void
AsciiWriter::WritePacket (enum Type type, std::string message, Ptr<const Packet> packet)
{
  std::string typeString;
  switch (type)
    {
    case ENQUEUE:
      typeString = "+";
      break;
    case DEQUEUE:
      typeString = "-";
      break;
    case RX:
      typeString = "r";
      break;
    case TX:
      typeString = "t";
      break;
    case DROP:
      typeString = "d";
      break;
    }
  NS_LOG_FUNCTION (this << typeString << message);
  *m_writer << typeString << " " << Simulator::Now ().GetSeconds () << " "
            << message << " " << *packet << std::endl;
}

} // namespace ns3
