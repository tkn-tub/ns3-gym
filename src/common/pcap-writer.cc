/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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

#include "yans/simulator.h"
#include "yans/system-file.h"
#include "pcap-writer.h"
#include "packet.h"


namespace yans {

enum {
	PCAP_ETHERNET = 1
};

PcapWriter::PcapWriter ()
{
	m_writer = 0;
	m_write_callback = make_callback (&PcapWriter::write_data, this);
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
PcapWriter::write_header_ethernet (void)
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
PcapWriter::write_packet (Packet const packet)
{
	if (m_writer != 0) {
		uint64_t current = Simulator::now_us ();
		uint64_t s = current / 1000000;
		uint64_t us = current % 1000000;
		write_32 (s & 0xffffffff);
		write_32 (us & 0xffffffff);
		write_32 (packet.get_size ());
		write_32 (packet.get_size ());
		packet.write (m_write_callback);
	}
}

void
PcapWriter::write_data (uint8_t *buffer, uint32_t size)
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

}; // namespace yans
