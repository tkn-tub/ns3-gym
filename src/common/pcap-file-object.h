/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#ifndef PCAP_FILE_OBJECT_H
#define PCAP_FILE_OBJECT_H

#include <string.h>
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "pcap-file.h"

namespace ns3 {

/*
 * A class representing a pcap file tailored for use in model code.
 */

class PcapFileObject : public Object
{
public:
  static TypeId GetTypeId (void);

  PcapFileObject ();
  ~PcapFileObject ();

  bool Open (std::string const &filename, std::string const &mode);

  void Close (void);

  bool Init (uint32_t dataLinkType, 
             uint32_t snapLen = PcapFile::SNAPLEN_DEFAULT, 
             int32_t tzCorrection = PcapFile::ZONE_DEFAULT);

  bool Write (Time t, Ptr<const Packet> p);
  bool Write (Time t, uint8_t const *buffer, uint32_t length);

  uint32_t GetMagic (void);
  uint16_t GetVersionMajor (void);
  uint16_t GetVersionMinor (void);
  int32_t GetTimeZoneOffset (void);
  uint32_t GetSigFigs (void);
  uint32_t GetSnapLen (void);
  uint32_t GetDataLinkType (void);
  
private:
  PcapFile file;
};

} //namespace ns3

#endif // PCAP_FILE_OBJECT_H

