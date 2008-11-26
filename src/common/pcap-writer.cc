/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

/*
 * Documentation kindly pointed out by Tom Henderson:
 * http://wiki.ethereal.com/Development/LibpcapFileFormat
 */

#include <fstream>

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/abort.h"
#include "ns3/simulator.h"
#include "pcap-writer.h"
#include "packet.h"

NS_LOG_COMPONENT_DEFINE ("PcapWriter");

namespace ns3 {

enum {
  PCAP_ETHERNET = 1,
  PCAP_PPP      = 9,
  PCAP_RAW_IP   = 101,
  PCAP_80211    = 105,
};

PcapWriter::PcapWriter ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("m_writer = 0");
  m_writer = 0;
}

PcapWriter::~PcapWriter ()
{
  NS_LOG_FUNCTION (this);

  if (m_writer != 0)
    {
      NS_LOG_LOGIC ("m_writer nonzero " << m_writer);
      if (m_writer->is_open ())
        {
          NS_LOG_LOGIC ("m_writer open.  Closing " << m_writer);
          m_writer->close ();
        }

      NS_LOG_LOGIC ("Deleting writer " << m_writer);
      delete m_writer;

      NS_LOG_LOGIC ("m_writer = 0");
      m_writer = 0;
    }
  else
    {
      NS_LOG_LOGIC ("m_writer == 0");
    }
}

void
PcapWriter::Open (std::string const &name)
{
  NS_LOG_FUNCTION (this << name);
  NS_ABORT_MSG_UNLESS (m_writer == 0, "PcapWriter::Open(): m_writer already allocated (std::ofstream leak detected)");

  m_writer = new std::ofstream ();
  NS_ABORT_MSG_UNLESS (m_writer, "PcapWriter::Open(): Cannot allocate m_writer");

  NS_LOG_LOGIC ("Created writer " << m_writer);

  m_writer->open (name.c_str ());
  NS_ABORT_MSG_IF (m_writer->fail (), "PcapWriter::Open(): m_writer->open(" << name.c_str () << ") failed");

  NS_ASSERT_MSG (m_writer->is_open (), "PcapWriter::Open(): m_writer not open");

  NS_LOG_LOGIC ("Writer opened successfully");
}

void 
PcapWriter::WriteEthernetHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_ETHERNET);
}

void 
PcapWriter::WriteIpHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_RAW_IP);
}

void
PcapWriter::WriteWifiHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_80211);
}

void 
PcapWriter::WritePppHeader (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  WriteHeader (PCAP_PPP);
}

void 
PcapWriter::WriteHeader (uint32_t network)
{
  NS_LOG_FUNCTION (this << network);
  Write32 (0xa1b2c3d4);
  Write16 (2);
  Write16 (4);
  Write32 (0);
  Write32 (0);
  Write32 (0xffff);
  Write32 (network);
}

void 
PcapWriter::WritePacket (Ptr<const Packet> packet)
{
  if (m_writer != 0) 
    {
      uint64_t current = Simulator::Now ().GetMicroSeconds ();
      uint64_t s = current / 1000000;
      uint64_t us = current % 1000000;
      Write32 (s & 0xffffffff);
      Write32 (us & 0xffffffff);
      Write32 (packet->GetSize ());
      Write32 (packet->GetSize ());
      WriteData (packet->PeekData (), packet->GetSize ());
    }
}

void
PcapWriter::WriteData (uint8_t const*buffer, uint32_t size)
{
  m_writer->write ((char const *)buffer, size);
}

void
PcapWriter::Write32 (uint32_t data)
{
  uint8_t buffer[4];
  buffer[0] = (data >> 0) & 0xff;
  buffer[1] = (data >> 8) & 0xff;
  buffer[2] = (data >> 16) & 0xff;
  buffer[3] = (data >> 24) & 0xff;
  WriteData (buffer, 4);
}

void
PcapWriter::Write16 (uint16_t data)
{
  uint8_t buffer[2];
  buffer[0] = (data >> 0) & 0xff;
  buffer[1] = (data >> 8) & 0xff;
  WriteData (buffer, 2);
}

} // namespace ns3
