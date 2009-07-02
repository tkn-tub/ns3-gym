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

#ifndef PCAP_WRITER_H
#define PCAP_WRITER_H

#include <stdint.h>
#include "ns3/ref-count-base.h"

namespace ns3 {

class Packet;

/**
 * \ingroup common
 *
 * \brief Pcap output for Packet logger
 *
 * Log Packets to a file in pcap format which can be
 * read by pcap readers.
 */
class PcapWriter : public RefCountBase
{
public:
  PcapWriter ();
  ~PcapWriter ();

  /**
   * \param name the name of the file to store packet log into.
   * This method creates the file if it does not exist. If it
   * exists, the file is emptied.
   */
  void Open (std::string const &name);

  /**
   * Write a pcap header in the output file which specifies
   * that the content of the file will be Packets with
   * Ethernet/LLC/SNAP encapsulation. This method should
   * be invoked before ns3::PcapWriter::writePacket and after
   * ns3::PcapWriter::open.
   */
  void WriteEthernetHeader (void);

  /**
   * Write a pcap header in the output file which specifies
   * that the content of the file will be IPv4 Packets. This 
   * method should be invoked before ns3::PcapWriter::WritePacket 
   * and after ns3::PcapWriter::Open.
   */
  void WriteIpHeader (void);

  /**
   * Write a pcap header in the output file which specifies
   * that the content of the file will be 802.11 Packets. This 
   * method should be invoked before ns3::PcapWriter::WritePacket 
   * and after ns3::PcapWriter::Open.
   */
  void WriteWifiHeader (void);

  /**
   * Write a pcap header in the output file which specifies
   * that the content of the file will be 802.11 Packets preceded by a
   * radiotap header providing PHY layer info. This method should be
   * invoked before ns3::PcapWriter::WritePacket and after
   * ns3::PcapWriter::Open. 
   */
  void WriteWifiRadiotapHeader (void);

  /**
   * Write a pcap header in the output file which specifies
   * that the content of the file will be 802.11 Packets preceded by a
   * prism header providing PHY layer info. This method should be
   * invoked before ns3::PcapWriter::WritePacket and after
   * ns3::PcapWriter::Open. 
   */
  void WriteWifiPrismHeader (void);

  /**
   * Write a pcap header in the output file which specifies
   * that the content of the file will be ppp Packets. This 
   * method should be invoked before ns3::PcapWriter::WritePacket 
   * and after ns3::PcapWriter::Open.
   */
  void WritePppHeader (void);

  /**
   * \param packet packet to write to output file
   */
  void WritePacket (Ptr<const Packet> packet);

  /** 
   * Write a Packet, possibly adding wifi PHY layer information to it
   *
   * @param packet the packet being received
   * @param channelFreqMhz the frequency in MHz at which the packet is
   * received. Note that in real devices this is normally the
   * frequency to which  the receiver is tuned, and this can be
   * different than the frequency at which the packet was originally
   * transmitted. This is because it is possible to have the receiver
   * tuned on a given channel and still to be able to receive packets
   * on a nearby channel.
   * @param rate the PHY data rate in units of 500kbps (i.e., the same
   * units used both for the radiotap and for the prism header) 
   * @param isShortPreamble true if short preamble is used, false otherwise
   * @param isTx true if packet is being transmitted, false when
   * packet is being received
   * @param signalDbm signal power in dBm
   * @param noiseDbm  noise power in dBm
   */
  void WriteWifiMonitorPacket(Ptr<const Packet> packet, uint16_t channelFreqMhz, 
                              uint32_t rate, bool isShortPreamble, bool isTx, 
                              double signalDbm, double noiseDbm);




private:
  void WriteData (uint8_t const*buffer, uint32_t size);
  void Write64 (uint64_t data);
  void Write32 (uint32_t data);
  void Write16 (uint16_t data);
  void Write8 (uint8_t data);
  void WriteHeader (uint32_t network);
  int8_t RoundToInt8 (double value);
  std::ofstream *m_writer;
  uint32_t m_pcapMode;
};

} // namespace ns3

#endif /* PCAP_WRITER_H */
