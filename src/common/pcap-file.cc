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
 * 
 * Author:  Craig Dowell (craigdo@ee.washington.edu)
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "pcap-file.h"
//
// This file is used as part of the ns-3 test framework, so please refrain from 
// adding any ns-3 specific constructs such as Packet to this file.
//
namespace ns3 {

const uint32_t MAGIC = 0xa1b2c3d4;            /**< Magic number identifying standard pcap file format */
const uint32_t SWAPPED_MAGIC = 0xd4c3b2a1;    /**< Looks this way if byte swapping is required */

const uint32_t NS_MAGIC = 0xa1b23cd4;         /**< Magic number identifying nanosec resolution pcap file format */
const uint32_t NS_SWAPPED_MAGIC = 0xd43cb2a1; /**< Looks this way if byte swapping is required */

const uint16_t VERSION_MAJOR = 2;             /**< Major version of supported pcap file format */
const uint16_t VERSION_MINOR = 4;             /**< Minor version of supported pcap file format */
const int32_t  SIGFIGS_DEFAULT = 0;           /**< Significant figures for timestamps (libpcap doesn't even bother) */

PcapFile::PcapFile ()
  : m_filename (""),
    m_filePtr (0),
    m_haveFileHeader (false),
    m_swapMode (false)
{
}

PcapFile::~PcapFile ()
{
  Close ();
}

void
PcapFile::Close (void)
{
  if (m_filePtr)
    {
      fclose (m_filePtr);
    }
  m_filePtr = 0;
  m_filename = "";
  m_haveFileHeader = false;
}

uint32_t
PcapFile::GetMagic (void)
{
  return m_fileHeader.m_magicNumber;
}

uint16_t
PcapFile::GetVersionMajor (void)
{
  return m_fileHeader.m_versionMajor;
}

uint16_t
PcapFile::GetVersionMinor (void)
{
  return m_fileHeader.m_versionMinor;
}

int32_t
PcapFile::GetTimeZoneOffset (void)
{
  return m_fileHeader.m_zone;
}

uint32_t
PcapFile::GetSigFigs (void)
{
  return m_fileHeader.m_sigFigs;
}

uint32_t
PcapFile::GetSnapLen (void)
{
  return m_fileHeader.m_snapLen;
}

uint32_t
PcapFile::GetDataLinkType (void)
{
  return m_fileHeader.m_type;
}

bool
PcapFile::GetSwapMode (void)
{
  return m_swapMode;
}

uint8_t
PcapFile::Swap (uint8_t val)
{
  return val;
}

uint16_t
PcapFile::Swap (uint16_t val)
{
  return ((val >> 8) & 0x00ff) | ((val << 8) & 0xff00);
}

uint32_t 
PcapFile::Swap (uint32_t val)
{
  return ((val >> 24) & 0x000000ff) | ((val >> 8) & 0x0000ff00) | ((val << 8) & 0x00ff0000) | ((val << 24) & 0xff000000);
}

void
PcapFile::Swap (PcapFileHeader *from, PcapFileHeader *to)
{
  to->m_magicNumber = Swap (from->m_magicNumber);
  to->m_versionMajor = Swap (from->m_versionMajor);
  to->m_versionMinor = Swap (from->m_versionMinor);
  to->m_zone = Swap (uint32_t(from->m_zone));
  to->m_sigFigs = Swap (from->m_sigFigs);
  to->m_snapLen = Swap (from->m_snapLen);
  to->m_type = Swap (from->m_type);
}

void
PcapFile::Swap (PcapRecordHeader *from, PcapRecordHeader *to)
{
  to->m_tsSec = Swap (from->m_tsSec);
  to->m_tsUsec = Swap (from->m_tsUsec);
  to->m_inclLen = Swap (from->m_inclLen);
  to->m_origLen = Swap (from->m_origLen);
}

bool
PcapFile::WriteFileHeader (void)
{
  //
  // If we're initializing the file, we need to write the pcap file header
  // at the start of the file.
  //
  int result = fseek (m_filePtr, 0, SEEK_SET);
  if (result)
    {
      return true;
    }
 
  //
  // We have the ability to write out the pcap file header in a foreign endian
  // format, so we need a temp place to swap on the way out.
  //
  PcapFileHeader header;

  //
  // the pointer headerOut selects either the swapped or non-swapped version of
  // the pcap file header.
  //
  PcapFileHeader *headerOut = 0;

  if (m_swapMode == false)
    {
      headerOut = &m_fileHeader;
    }
  else
    {
      Swap (&m_fileHeader, &header);
      headerOut = &header;
    }

  //
  // Watch out for memory alignment differences between machines, so write
  // them all individually.
  //
  result = 0;

  result |= (fwrite (&headerOut->m_magicNumber, sizeof(headerOut->m_magicNumber), 1, m_filePtr) != 1);
  result |= (fwrite (&headerOut->m_versionMajor, sizeof(headerOut->m_versionMajor), 1, m_filePtr) != 1);
  result |= (fwrite (&headerOut->m_versionMinor, sizeof(headerOut->m_versionMinor), 1, m_filePtr) != 1);
  result |= (fwrite (&headerOut->m_zone, sizeof(headerOut->m_zone), 1, m_filePtr) != 1);
  result |= (fwrite (&headerOut->m_sigFigs, sizeof(headerOut->m_sigFigs), 1, m_filePtr) != 1);
  result |= (fwrite (&headerOut->m_snapLen, sizeof(headerOut->m_snapLen), 1, m_filePtr) != 1);
  result |= (fwrite (&headerOut->m_type, sizeof(headerOut->m_type), 1, m_filePtr) != 1);

  //
  // If any of the fwrites above did not succeed in writinging the correct
  // number of objects, result will be nonzero and will indicate an error.
  //
  return result != 0;
}

bool
PcapFile::ReadAndVerifyFileHeader (void)
{
  //
  // Pcap file header is always at the start of the file
  //
  int result = fseek (m_filePtr, 0, SEEK_SET);
  if (result)
    {
      return true;
    }

  //
  // Watch out for memory alignment differences between machines, so read
  // them all individually.
  //
  result = 0;

  result |= (fread (&m_fileHeader.m_magicNumber, sizeof(m_fileHeader.m_magicNumber), 1, m_filePtr) != 1);
  result |= (fread (&m_fileHeader.m_versionMajor, sizeof(m_fileHeader.m_versionMajor), 1, m_filePtr) != 1);
  result |= (fread (&m_fileHeader.m_versionMinor, sizeof(m_fileHeader.m_versionMinor), 1, m_filePtr) != 1);
  result |= (fread (&m_fileHeader.m_zone, sizeof(m_fileHeader.m_zone), 1, m_filePtr) != 1);
  result |= (fread (&m_fileHeader.m_sigFigs, sizeof(m_fileHeader.m_sigFigs), 1, m_filePtr) != 1);
  result |= (fread (&m_fileHeader.m_snapLen, sizeof(m_fileHeader.m_snapLen), 1, m_filePtr) != 1);
  result |= (fread (&m_fileHeader.m_type, sizeof(m_fileHeader.m_type), 1, m_filePtr) != 1);

  //
  // If any of the freads above did not succeed in reading the correct number of 
  // objects, result will be nonzero.
  //
  if (result)
    {
      return true;
    }

  //
  // There are four possible magic numbers that can be there.  Normal and byte
  // swapped versions of the standard magic number, and normal and byte swapped
  // versions of the magic number indicating nanosecond resolution timestamps.
  //
  if (m_fileHeader.m_magicNumber != MAGIC && m_fileHeader.m_magicNumber != SWAPPED_MAGIC && 
      m_fileHeader.m_magicNumber != NS_MAGIC && m_fileHeader.m_magicNumber != NS_SWAPPED_MAGIC)
    {
      return true;
    }

  //
  // If the magic number is swapped, then we can assume that everything else we read
  // is swapped.
  //
  m_swapMode = (m_fileHeader.m_magicNumber == SWAPPED_MAGIC || m_fileHeader.m_magicNumber == NS_SWAPPED_MAGIC) ? true : false;

  if (m_swapMode)
    {
      Swap (&m_fileHeader, &m_fileHeader);
    }

  //
  // We only deal with one version of the pcap file format.
  //
  if (m_fileHeader.m_versionMajor != VERSION_MAJOR || m_fileHeader.m_versionMinor != VERSION_MINOR)
    {
      return true;
    }

  //
  // A quick test of reasonablness for the time zone offset corresponding to 
  // a real place on the planet.
  //
  if (m_fileHeader.m_zone < -12 || m_fileHeader.m_zone > 12)
    {
      return true;
    }

  m_haveFileHeader = true;
  return false;
}

bool
PcapFile::Open (std::string const &filename, std::string const &mode)
{
  //
  // If opening a new file, implicit close of any existing file required.
  //
  Close ();
        
  //
  // All pcap files are binary files, so we just do this automatically.
  //
  std::string realMode = mode + "b";

  //
  // Our modes may be subtly different from the standard fopen semantics since
  // we need to have a pcap file header to succeed in some cases; so we need 
  // to process different modes according to our own definitions of the modes.
  //
  // In the case of read modes, we must read, check and save the pcap file
  // header as well as just opening the file.
  //
  // In the case of write modes, we just pass the call on through to the 
  // library.
  //
  // In the case of append modes, we change the semantics to require the
  // given file to exist.  We can't just create a file since we can't make up
  // a pcap file header on our own.
  //
  if (realMode == "rb" || realMode == "r+b")
    {
      m_filePtr = fopen (filename.c_str (), realMode.c_str ());
      if (m_filePtr == 0)
        {
          return true;
        }
      m_filename = filename;
      return ReadAndVerifyFileHeader ();
    }
  else if (realMode == "wb" || realMode == "w+b")
    {
      m_filePtr = fopen (filename.c_str (), realMode.c_str ());
      if (m_filePtr)
        {
          m_filename = filename;
          return false;
        }
      else
        {
          return true;
        }
    }
  else if (realMode == "ab" || realMode == "a+b")
    {
      //
      // Remember that semantics for append are different here.  We never create
      // a file since we can't make up a pcap file header.  We first have to 
      // open the file in read-only mode and check to see that it exists and
      // read the file header.  If this all works out, then we can go ahead and
      // open the file in append mode and seek to the end (imlicitly).
      //
      m_filePtr = fopen (filename.c_str (), "rb");
      if (m_filePtr == 0)
        {
          return true;
        }

      bool result = ReadAndVerifyFileHeader ();
      if (result == true)
        {
          Close ();
          return true;
        }

      //
      // We have a properly initialized file and have the pcap file header
      // loaded and checked.  This means that the file meets all of the 
      // critera for opening in append mode, but the file is in read-only mode
      // now -- we must close it and open it in the correct mode.
      //
      fclose (m_filePtr);
      m_filePtr = 0;

      m_filePtr = fopen (filename.c_str (), realMode.c_str ());
      if (m_filePtr == 0)
        {
          return true;
        }

      m_filename = filename;
      return false;
    }
  else
    {
      return true;
    }
}

bool
PcapFile::Init (uint32_t dataLinkType, uint32_t snapLen, int32_t timeZoneCorrection, bool swapMode)
{
  //
  // Initialize the in-memory file header.
  //
  m_fileHeader.m_magicNumber = MAGIC;
  m_fileHeader.m_versionMajor = VERSION_MAJOR;
  m_fileHeader.m_versionMinor = VERSION_MINOR;
  m_fileHeader.m_zone = timeZoneCorrection;
  m_fileHeader.m_sigFigs = 0;
  m_fileHeader.m_snapLen = snapLen;
  m_fileHeader.m_type = dataLinkType;

  m_haveFileHeader = true;
  m_swapMode = swapMode;

  return WriteFileHeader ();
}

bool
PcapFile::Write (uint32_t tsSec, uint32_t tsUsec, uint8_t const * const data, uint32_t totalLen)
{
  if (m_haveFileHeader == false)
    {
      return true;
    }

  uint32_t inclLen = totalLen > m_fileHeader.m_snapLen ? m_fileHeader.m_snapLen : totalLen;

  PcapRecordHeader header;
  header.m_tsSec = tsSec;
  header.m_tsUsec = tsUsec;
  header.m_inclLen = inclLen;
  header.m_origLen = totalLen;

  if (m_swapMode)
    {
      Swap (&header, &header);
    }

  //
  // Watch out for memory alignment differences between machines, so write
  // them all individually.
  //
  uint32_t result = 0;

  result |= (fwrite (&header.m_tsSec, sizeof(header.m_tsSec), 1, m_filePtr) != 1);
  result |= (fwrite (&header.m_tsUsec, sizeof(header.m_tsUsec), 1, m_filePtr) != 1);
  result |= (fwrite (&header.m_inclLen, sizeof(header.m_inclLen), 1, m_filePtr) != 1);
  result |= (fwrite (&header.m_origLen, sizeof(header.m_origLen), 1, m_filePtr) != 1);

  result |= fwrite (data, 1, inclLen, m_filePtr) != inclLen;

  return result != 0;
}

bool
PcapFile::Read (
  uint8_t * const data, 
  uint32_t maxBytes,
  uint32_t &tsSec, 
  uint32_t &tsUsec, 
  uint32_t &inclLen, 
  uint32_t &origLen,
  uint32_t &readLen)
{
  if (m_haveFileHeader == false)
    {
      return true;
    }

  PcapRecordHeader header;

  //
  // Watch out for memory alignment differences between machines, so read
  // them all individually.
  //
  uint32_t result = 0;

  result |= (fread (&header.m_tsSec, sizeof(header.m_tsSec), 1, m_filePtr) != 1);
  result |= (fread (&header.m_tsUsec, sizeof(header.m_tsUsec), 1, m_filePtr) != 1);
  result |= (fread (&header.m_inclLen, sizeof(header.m_inclLen), 1, m_filePtr) != 1);
  result |= (fread (&header.m_origLen, sizeof(header.m_origLen), 1, m_filePtr) != 1);

  //
  // If any of the freads above did not succeed in reading the correct number of 
  // objects, result will be nonzero.
  //
  if (result)
    {
      return true;
    }

  if (m_swapMode)
    {
      Swap (&header, &header);
    }

  tsSec = header.m_tsSec;
  tsUsec = header.m_tsUsec;
  inclLen = header.m_inclLen;
  origLen = header.m_origLen;

  //
  // We don't always want to force the client to keep a maximum length buffer 
  // around so we allow her to specify a minimum number of bytes to read.  
  // Usually 64 bytes is enough information to print all of the headers, so
  // it isn't typically necessary to read all thousand bytes of an echo packet,
  // for example, to figure out what is going on.
  //
  readLen = maxBytes < header.m_inclLen ? maxBytes : header.m_inclLen;
  result = fread (data, 1, readLen, m_filePtr) != readLen;
  if (result)
    {
      return result;
    }

  //
  // To keep the file pointer pointed in the right place, however, we always
  // need to account for the entire packet as stored originally.
  //
  if (readLen < header.m_inclLen)
    {
      uint64_t pos = ftell (m_filePtr);
      int result = fseek (m_filePtr, pos + header.m_inclLen - readLen, SEEK_SET);
      if (result)
        {
          return true;
        }
    }

  return false;
}

bool
PcapFile::Diff (std::string const & f1, std::string const & f2, 
                uint32_t & sec, uint32_t & usec, 
                uint32_t snapLen)
{
  PcapFile pcap[2];
  for (int i = 0; i < 2; ++i)
    {
      std::string const & file = (i == 0) ? f1 : f2;
      bool err = pcap[i].Open (file, "r");
      if (err)
        {
          // Can't open file
          return true;
        }
    }
  
  uint8_t data[2][snapLen];
  uint32_t tsSec[2], tsUsec[2], inclLen[2], origLen[2], readLen[2];
  bool err[2];
  bool diff(false);
  
  while (1)
    {
      for (int i = 0; i < 2; ++i)
        err[i] = pcap[i].Read (data[i], snapLen, tsSec[i], tsUsec[i], inclLen[i], origLen[i], readLen[i]);
    
      sec = tsSec[0];
      usec = tsUsec[0];
      
      if (err[0] != err[1])
        {
          diff = true; // Read status doesn't match
          break;
        }
      
      if (err[0]) break; // nothing left
      
      if (tsSec[0] != tsSec[1] || tsUsec[0] != tsUsec[1])
        {
          diff = true; // Next packet timestamps do not match
          break;
        }
      
      if (readLen[0] != readLen[1])
        {
          diff = true; // Packet lengths do not match
          break;
        }
      
      if (std::memcmp(data[0], data[1], readLen[0]) != 0)
        {
          diff = true; // Packet data do not match
          break;
        }
    }  
  pcap[0].Close ();
  pcap[1].Close ();
  return diff;
}

} //namespace ns3
