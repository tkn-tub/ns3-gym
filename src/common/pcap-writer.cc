/* -*- Mode:NS3; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

/*
 * Documentation kindly pointed out by Tom Henderson:
 * http://wiki.ethereal.com/Development/LibpcapFileFormat
 */

#include "ns3/simulator.h"
#include "ns3/system-file.h"
#include "pcap-writer.h"
#include "packet.h"


namespace ns3 {

enum {
    PCAP_ETHERNET = 1
};

PcapWriter::PcapWriter ()
{
    m_writer = 0;
}
PcapWriter::~PcapWriter ()
{
    delete m_writer;
}

void
PcapWriter::Open (char const *name)
{
    m_writer = new SystemFile ();
    m_writer->Open (name);
}

void 
PcapWriter::WriteHeaderEthernet (void)
{
    Write32 (0xa1b2c3d4);
    Write16 (2);
    Write16 (4);
    Write32 (0);
    Write32 (0);
    Write32 (0xffff);
    Write32 (PCAP_ETHERNET);
}

void 
PcapWriter::WritePacket (Packet const packet)
{
    if (m_writer != 0) {
        uint64_t current = Simulator::Now ().Us ();
        uint64_t s = current / 1000000;
        uint64_t us = current % 1000000;
        Write32 (s & 0xffffffff);
        Write32 (us & 0xffffffff);
        Write32 (packet.GetSize ());
        Write32 (packet.GetSize ());
    	m_writer->Write (packet.PeekData (), packet.GetSize ());
    }
}

void
PcapWriter::WriteData (uint8_t *buffer, uint32_t size)
{
    m_writer->Write (buffer, size);
}
void
PcapWriter::Write32 (uint32_t data)
{
    m_writer->Write ((uint8_t*)&data, 4);
}
void
PcapWriter::Write16 (uint16_t data)
{
    m_writer->Write ((uint8_t*)&data, 2);
}

}; // namespace ns3
