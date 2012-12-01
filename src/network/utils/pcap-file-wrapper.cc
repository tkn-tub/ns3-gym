/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/buffer.h"
#include "ns3/header.h"
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
  NS_LOG_FUNCTION (this);
}

PcapFileWrapper::~PcapFileWrapper ()
{
  NS_LOG_FUNCTION (this);
  Close ();
}


bool 
PcapFileWrapper::Fail (void) const
{
  NS_LOG_FUNCTION (this);
  return m_file.Fail ();
}
bool 
PcapFileWrapper::Eof (void) const
{
  NS_LOG_FUNCTION (this);
  return m_file.Eof ();
}
void 
PcapFileWrapper::Clear (void)
{
  NS_LOG_FUNCTION (this);
  m_file.Clear ();
}

void
PcapFileWrapper::Close (void)
{
  NS_LOG_FUNCTION (this);
  m_file.Close ();
}

void
PcapFileWrapper::Open (std::string const &filename, std::ios::openmode mode)
{
  NS_LOG_FUNCTION (this << filename << mode);
  m_file.Open (filename, mode);
}

void
PcapFileWrapper::Init (uint32_t dataLinkType, uint32_t snapLen, int32_t tzCorrection)
{
  //
  // If the user doesn't provide a snaplen, the default value will come in.  If
  // this happens, we use the "CaptureSize" Attribute.  If the user does provide
  // a snaplen, we use the one provided.
  //
  NS_LOG_FUNCTION (this << dataLinkType << snapLen << tzCorrection);
  if (snapLen != std::numeric_limits<uint32_t>::max ())
    {
      m_file.Init (dataLinkType, snapLen, tzCorrection);
    } 
  else
    {
      m_file.Init (dataLinkType, m_snapLen, tzCorrection);
    } 
}

void
PcapFileWrapper::Write (Time t, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (this << t << p);
  uint64_t current = t.GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;

  m_file.Write (s, us, p);
}

void
PcapFileWrapper::Write (Time t, Header &header, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (this << t << &header << p);
  uint64_t current = t.GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;

  m_file.Write (s, us, header, p);
}

void
PcapFileWrapper::Write (Time t, uint8_t const *buffer, uint32_t length)
{
  NS_LOG_FUNCTION (this << t << &buffer << length);
  uint64_t current = t.GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;

  m_file.Write (s, us, buffer, length);
}

uint32_t
PcapFileWrapper::GetMagic (void)
{
  NS_LOG_FUNCTION (this);
  return m_file.GetMagic ();
}

uint16_t
PcapFileWrapper::GetVersionMajor (void)
{
  NS_LOG_FUNCTION (this);
  return m_file.GetVersionMajor ();
}

uint16_t
PcapFileWrapper::GetVersionMinor (void)
{
  NS_LOG_FUNCTION (this);
  return m_file.GetVersionMinor ();
}

int32_t
PcapFileWrapper::GetTimeZoneOffset (void)
{
  NS_LOG_FUNCTION (this);
  return m_file.GetTimeZoneOffset ();
}

uint32_t
PcapFileWrapper::GetSigFigs (void)
{
  NS_LOG_FUNCTION (this);
  return m_file.GetSigFigs ();
}

uint32_t
PcapFileWrapper::GetSnapLen (void)
{
  NS_LOG_FUNCTION (this);
  return m_file.GetSnapLen ();
}

uint32_t
PcapFileWrapper::GetDataLinkType (void)
{
  NS_LOG_FUNCTION (this);
  return m_file.GetDataLinkType ();
}

} // namespace ns3
