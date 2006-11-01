/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef PCAP_WRITER_H
#define PCAP_WRITER_H

#include "ns3/callback.h"
#include <stdint.h>
#include "packet.h"

namespace ns3 {

class SystemFile;

/**
 * \brief Pcap output for Packet logger
 *
 * Log Packets to a file in pcap format which can be
 * read by pcap readers.
 */
class PcapWriter {
public:
  PcapWriter ();
  ~PcapWriter ();

  /**
   * \param name the name of the file to store packet log into.
   * This method creates the file if it does not exist. If it
   * exists, the file is emptied.
   */
  void Open (char const *name);

  /**
   * Write a pcap header in the output file which specifies
   * that the content of the file will Packets with
   * Ethernet/LLC/SNAP encapsulation. This method should
   * be invoked before ns3::PcapWriter::writePacket and after
   * ns3::PcapWriter::open.
   */
  void WriteHeaderEthernet (void);

  /**
   * \param packet packet to write to output file
   */
  void WritePacket (Packet const packet);

private:
  void WriteData (uint8_t *buffer, uint32_t size);
  void Write32 (uint32_t data);
  void Write16 (uint16_t data);
  SystemFile *m_writer;
  Callback<void,uint8_t *,uint32_t> m_writeCallback;
};

}; // namespace ns3

#endif /* PCAP_WRITER_H */
