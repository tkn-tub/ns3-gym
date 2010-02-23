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

#ifndef PCAP_FILE_WRAPPER_H
#define PCAP_FILE_WRAPPER_H

#include <string.h>
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "pcap-file.h"

namespace ns3 {

/*
 * A class representing a pcap file tailored for use in model code.
 */

class PcapFileWrapper : public Object
{
public:
  static TypeId GetTypeId (void);

  PcapFileWrapper ();
  ~PcapFileWrapper ();

  /**
   * Create a new pcap file wrapper representing a new or existing pcap file.
   * Semantics are similar to the C standard library function \c fopen
   *
   * Possible modes are:
   *
   * \verbatim
   * "r":   Open a file for reading.  The file must exist.  The pcap header
   *        is assumed to exist in the file and will be read and checked.
   *        The file seek position indicator is set to point to the first 
   *        packet on exit.
   *
   * "w":   Create an empty file for writing. If a file with the same name 
   *        already exists its content is erased and the file is treated as a 
   *        new empty pcap file.  The file is assumed not to have a pcap 
   *        header and the caller is responsible for calling Init before saving
   *        any packet data.  The file seek position indicator is set to point 
   *        to the beginning of the file on exit since there will be no pcap
   *        header.
   *
   * "a":   Append to an existing file. This mode allows for adding packet data
   *        to the end of an existing pcap file.  The file must exist and have a
   *        valid pcap header written (N.B. this is different from standard fopen
   *        semantics).  The file seek position indicator is set to point 
   *        to the end of the file on exit.
   *
   * "r+":  Open a file for update -- both reading and writing. The file must 
   *        exist.  The pcap header is assumed to have been written to the 
   *        file and will be read and checked.  The file seek position indicator
   *        is set to point to the first packet on exit.
   *
   * "w+":  Create an empty file for both reading and writing.  If a file with
   *        the same name already exists, its content is erased and the file is 
   *        treated as a new empty pcap file.  Since this new file will not have
   *        a pcap header, the caller is responsible for calling Init before 
   *        saving any packet data.  On exit, the file seek position indicator is
   *        set to point to the beginning of the file.
   *
   * "a+"   Open a file for reading and appending.  The file must exist and have a
   *        valid pcap header written (N.B. this is different from standard fopen
   *        semantics).  The file seek position indicator is set to point 
   *        to the end of the file on exit.  Existing content is preserved.
   * \endverbatim
   *
   * Since a pcap file is always a binary file, the file type is automatically 
   * selected as a binary file.  For example, providing a mode string "a+" 
   * results in the underlying OS file being opened in "a+b" mode.
   *
   * \param filename String containing the name of the file.
   *
   * \param mode String containing the access mode for the file.
   *
   * \returns Error indication that should be interpreted as, "did an error 
   * happen"?  That is, the method returns false if the open succeeds, true 
   * otherwise.  The errno variable will be set by the OS to to provide a 
   * more descriptive failure indication.
   */
  bool Open (std::string const &filename, std::string const &mode);

  /**
   * Close the underlying pcap file.
   */
  void Close (void);

  /**
   * Initialize the pcap file associated with this wrapper.  This file must have
   * been previously opened with write permissions.
   *
   * \param dataLinkType A data link type as defined in the pcap library.  If
   * you want to make resulting pcap files visible in existing tools, the 
   * data link type must match existing definitions, such as PCAP_ETHERNET,
   * PCAP_PPP, PCAP_80211, etc.  If you are storing different kinds of packet
   * data, such as naked TCP headers, you are at liberty to locally define your
   * own data link types.  According to the pcap-linktype man page, "well-known"
   * pcap linktypes range from 0 to 177.  If you use a large random number for
   * your type, chances are small for a collision.
   *
   * \param snapLen An optional maximum size for packets written to the file.
   * Defaults to 65535.  If packets exceed this length they are truncated.
   *
   * \param tzCorrection An integer describing the offset of your local
   * time zone from UTC/GMT.  For example, Pacific Standard Time in the US is
   * GMT-8, so one would enter -8 for that correction.  Defaults to 0 (UTC).
   *
   * \return false if the open succeeds, true otherwise.
   *
   * \warning Calling this method on an existing file will result in the loss
   * any existing data.
   */
  bool Init (uint32_t dataLinkType, 
             uint32_t snapLen = std::numeric_limits<uint32_t>::max (), 
             int32_t tzCorrection = PcapFile::ZONE_DEFAULT);

  /**
   * \brief Write the next packet to file
   * 
   * \param t Packet timestamp as ns3::Time.
   * \param p Packet to write to the pcap file.
   * 
   * \return true on error, false otherwise
   */
  bool Write (Time t, Ptr<const Packet> p);

  /**
   * \brief Write the provided header along with the packet to the pcap file.
   *
   * It is the case that adding a header to a packet prior to writing it to a
   * file must trigger a deep copy in the Packet.  By providing the header
   * separately, we can avoid that copy.
   * 
   * \param t Packet timestamp as ns3::Time.
   * \param header The Header to prepend to the packet.
   * \param p Packet to write to the pcap file.
   * 
   * \return true on error, false otherwise
   */
  bool Write (Time t, Header &header, Ptr<const Packet> p);

  /**
   * \brief Write the provided data buffer to the pcap file.
   *
   * \param t Packet timestamp as ns3::Time.
   * \param buffer The buffer to write.
   * \param length The size of the buffer.
   * 
   * \return true on error, false otherwise
   */
  bool Write (Time t, uint8_t const *buffer, uint32_t length);

  /*
   * \brief Returns the magic number of the pcap file as defined by the magic_number
   * field in the pcap global header.
   *
   * See http://wiki.wireshark.org/Development/LibpcapFileFormat
   */ 
  uint32_t GetMagic (void);

  /*
   * \brief Returns the major version of the pcap file as defined by the version_major
   * field in the pcap global header.
   *
   * See http://wiki.wireshark.org/Development/LibpcapFileFormat
   */ 
  uint16_t GetVersionMajor (void);

  /*
   * \brief Returns the minor version of the pcap file as defined by the version_minor
   * field in the pcap global header.
   *
   * See http://wiki.wireshark.org/Development/LibpcapFileFormat
   */ 
  uint16_t GetVersionMinor (void);

  /*
   * \brief Returns the time zone offset of the pcap file as defined by the thiszone
   * field in the pcap global header.
   *
   * See http://wiki.wireshark.org/Development/LibpcapFileFormat
   */ 
  int32_t GetTimeZoneOffset (void);

  /*
   * \brief Returns the accuracy of timestamps field of the pcap file as defined
   * by the sigfigs field in the pcap global header.
   *
   * See http://wiki.wireshark.org/Development/LibpcapFileFormat
   */ 
  uint32_t GetSigFigs (void);

  /*
   * \brief Returns the max length of saved packets field of the pcap file as 
   * defined by the snaplen field in the pcap global header.
   *
   * See http://wiki.wireshark.org/Development/LibpcapFileFormat
   */ 
  uint32_t GetSnapLen (void);

  /*
   * \brief Returns the data link type field of the pcap file as defined by the 
   * network field in the pcap global header.
   *
   * See http://wiki.wireshark.org/Development/LibpcapFileFormat
   */ 
  uint32_t GetDataLinkType (void);
  
private:
  PcapFile m_file;
  uint32_t m_snapLen;
};

} //namespace ns3

#endif // PCAP_FILE_WRAPPER_H
