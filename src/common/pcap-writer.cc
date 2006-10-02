/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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
PcapWriter::open (char const *name)
{
    m_writer = new SystemFile ();
    m_writer->open (name);
}

void 
PcapWriter::writeHeaderEthernet (void)
{
    write_32 (0xa1b2c3d4);
    write_16 (2);
    write_16 (4);
    write_32 (0);
    write_32 (0);
    write_32 (0xffff);
    write_32 (PCAP_ETHERNET);
}

void 
PcapWriter::writePacket (Packet const packet)
{
    if (m_writer != 0) {
        uint64_t current = Simulator::now ().us ();
        uint64_t s = current / 1000000;
        uint64_t us = current % 1000000;
        write_32 (s & 0xffffffff);
        write_32 (us & 0xffffffff);
        write_32 (packet.getSize ());
        write_32 (packet.getSize ());
    	m_writer->write (packet.peekData (), packet.getSize ());
    }
}

void
PcapWriter::writeData (uint8_t *buffer, uint32_t size)
{
    m_writer->write (buffer, size);
}
void
PcapWriter::write_32 (uint32_t data)
{
    m_writer->write ((uint8_t*)&data, 4);
}
void
PcapWriter::write_16 (uint16_t data)
{
    m_writer->write ((uint8_t*)&data, 2);
}

}; // namespace ns3
