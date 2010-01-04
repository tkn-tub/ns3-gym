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

#include "pcap-file-object.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("PcapFileObject");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PcapFileObject);

TypeId 
PcapFileObject::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PcapFileObject")
    .SetParent<Object> ()
    .AddConstructor<PcapFileObject> ()
    ;
  return tid;
}


PcapFileObject::PcapFileObject ()
{
}

PcapFileObject::~PcapFileObject ()
{
  Close ();
}

void
PcapFileObject::Close (void)
{
  file.Close ();
}

bool
PcapFileObject::Open (std::string const &filename, std::string const &mode)
{
  return file.Open (filename, mode);
}

bool
PcapFileObject::Init (uint32_t dataLinkType, uint32_t snapLen, int32_t tzCorrection)
{
  return file.Init (dataLinkType, snapLen, tzCorrection);
}

bool
PcapFileObject::Write (Time t, Ptr<const Packet> p)
{
  uint64_t current = t.GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;

  uint32_t bufferSize = p->GetSize ();
  uint8_t *buffer = new uint8_t[bufferSize];
  p->CopyData (buffer, bufferSize);
  bool rc = file.Write (s, us, buffer, bufferSize);
  delete [] buffer;
  return rc;
}

bool
PcapFileObject::Write (Time t, uint8_t const *buffer, uint32_t length)
{
  uint64_t current = t.GetMicroSeconds ();
  uint64_t s = current / 1000000;
  uint64_t us = current % 1000000;

  return file.Write (s, us, buffer, length);
}

uint32_t
PcapFileObject::GetMagic (void)
{
  return file.GetMagic ();
}

uint16_t
PcapFileObject::GetVersionMajor (void)
{
  return file.GetVersionMajor ();
}

uint16_t
PcapFileObject::GetVersionMinor (void)
{
  return file.GetVersionMinor ();
}

int32_t
PcapFileObject::GetTimeZoneOffset (void)
{
  return file.GetTimeZoneOffset ();
}

uint32_t
PcapFileObject::GetSigFigs (void)
{
  return file.GetSigFigs ();
}

uint32_t
PcapFileObject::GetSnapLen (void)
{
  return file.GetSnapLen ();
}

uint32_t
PcapFileObject::GetDataLinkType (void)
{
  return file.GetDataLinkType ();
}

} //namespace ns3
