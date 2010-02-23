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

#include "ns3/log.h"
#include "ns3/uinteger.h"

#include "buffer.h"
#include "header.h"
#include "pcap-file-wrapper.h"

NS_LOG_COMPONENT_DEFINE ("PcapFileWrapper");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PcapFileWrapper);

TypeId 
PcapFileWrapper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PcapFileWrapper")
    .SetParent<Object> ()
    .AddConstructor<PcapFileWrapper> ()
    .AddAttribute ("CaptureSize",
                   "Maximum length of captured packets (cf. pcap snaplen)",
                   UintegerValue (PcapFile::SNAPLEN_DEFAULT),
                   MakeUintegerAccessor (&PcapFileWrapper::m_snapLen),
                   MakeUintegerChecker<uint32_t> (0, PcapFile::SNAPLEN_DEFAULT))
    ;
  return tid;
}


PcapFileWrapper::PcapFileWrapper ()
{
}

PcapFileWrapper::~PcapFileWrapper ()
{
  Close ();
}

void
PcapFileWrapper::Close (void)
{
  m_file.Close ();
}

bool
PcapFileWrapper::Open (std::string const &filename, std::string const &mode)
{
  return m_file.Open (filename, mode);
}

bool
PcapFileWrapper::Init (uint32_t dataLinkType, uint32_t snapLen, int32_t tzCorrection)
{
  //
  // If the user doesn't provide a snaplen, the default value will come in.  If
  // this happens, we use the "CaptureSize" Attribute.  If the user does provide
  // a snaplen, we use the one provided.
  //
  if (snapLen != std::numeric_limits<uint32_t>::max ())
    {
      return m_file.Init (dataLinkType, snapLen, tzCorrection);
    } 
  else
    {
      return m_file.Init (dataLinkType, m_snapLen, tzCorrection);
    } 

  //
  // Quiet the compiler
  //
  return true;
}

bool
PcapFileWrapper::Write (Time t, Ptr<const Packet> p)
{
  uint8_t buffer[PcapFile::SNAPLEN_DEFAULT];

  uint64_t current = t.GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;

  uint32_t bufferSize = p->GetSize ();
  p->CopyData (buffer, bufferSize);
  bool rc = m_file.Write (s, us, buffer, bufferSize);
  return rc;
}

bool
PcapFileWrapper::Write (Time t, Header &header, Ptr<const Packet> p)
{
  uint8_t buffer[PcapFile::SNAPLEN_DEFAULT];

  uint64_t current = t.GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;

  Buffer headerBuffer;
  uint32_t headerSize = header.GetSerializedSize ();
  uint32_t packetSize = p->GetSize ();
  uint32_t bufferSize = headerSize + packetSize;

  headerBuffer.AddAtStart (headerSize);
  header.Serialize (headerBuffer.Begin ());

  headerBuffer.Begin ().Read (buffer, headerSize);
  p->CopyData (&buffer[headerSize], packetSize);
  bool rc = m_file.Write (s, us, buffer, bufferSize);

  return rc;
}

bool
PcapFileWrapper::Write (Time t, uint8_t const *buffer, uint32_t length)
{
  uint64_t current = t.GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;

  return m_file.Write (s, us, buffer, length);
}

uint32_t
PcapFileWrapper::GetMagic (void)
{
  return m_file.GetMagic ();
}

uint16_t
PcapFileWrapper::GetVersionMajor (void)
{
  return m_file.GetVersionMajor ();
}

uint16_t
PcapFileWrapper::GetVersionMinor (void)
{
  return m_file.GetVersionMinor ();
}

int32_t
PcapFileWrapper::GetTimeZoneOffset (void)
{
  return m_file.GetTimeZoneOffset ();
}

uint32_t
PcapFileWrapper::GetSigFigs (void)
{
  return m_file.GetSigFigs ();
}

uint32_t
PcapFileWrapper::GetSnapLen (void)
{
  return m_file.GetSnapLen ();
}

uint32_t
PcapFileWrapper::GetDataLinkType (void)
{
  return m_file.GetDataLinkType ();
}

} //namespace ns3
