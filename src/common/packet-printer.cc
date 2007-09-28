/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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

#include "packet-printer.h"
#include "chunk-registry.h"

namespace ns3 {

PacketPrinter::PacketPrinter ()
  : m_forward (true),
    m_separator (" ")
{}

void 
PacketPrinter::PrintForward (void)
{
  m_forward = true;
}
void 
PacketPrinter::PrintBackward (void)
{
  m_forward = false;
}
void
PacketPrinter::SetSeparator (std::string separator)
{
  m_separator = separator;
}
void 
PacketPrinter::SetPayloadPrinter (PayloadPrinter printer)
{
  m_payloadPrinter = printer;
}

void 
PacketPrinter::PrintChunk (uint32_t chunkUid, 
                           Buffer::Iterator start, 
                           std::ostream &os, 
                           uint32_t packetUid,
                           uint32_t size) const
{
  uint8_t *instance = ChunkRegistry::GetStaticInstance (chunkUid);
  ChunkRegistry::Deserialize (chunkUid, instance, start);

  for (PrinterList::const_iterator i = m_printerList.begin (); i != m_printerList.end (); i++)
    {
      if (i->m_chunkUid == chunkUid)
        {
          ChunkRegistry::InvokePrintCallback (chunkUid, instance, os, packetUid, size, i->m_printer);
          return;
        }
    }
  // if the over did not override this type of chunk,
  // we print something by default.
  std::string name = ChunkRegistry::GetName (chunkUid, instance);
  os << name;
  ChunkRegistry::Print (chunkUid, instance, os);
}
void 
PacketPrinter::PrintChunkFragment (uint32_t chunkUid,
                                   std::ostream &os,
                                   uint32_t packetUid,
                                   uint32_t size,
                                   uint32_t fragmentStart,
                                   uint32_t fragmentEnd) const
{
  uint8_t *instance = ChunkRegistry::GetStaticInstance (chunkUid);
  std::string name = ChunkRegistry::GetName (chunkUid, instance);
  struct PacketPrinter::FragmentInformation info;
  info.start = fragmentStart;
  info.end = fragmentEnd;
  for (PrinterList::const_iterator i = m_printerList.begin (); i != m_printerList.end (); i++)
    {
      if (i->m_chunkUid == chunkUid)
        {

          i->m_fragmentPrinter (os, packetUid, size, name, info);
          return;
        }
    }
  // if the user did not override this type of chunk,
  // we print something by default.
  NS_ASSERT (info.start != 0 || info.end != 0);
  os << name << " "
     << "("
     << "length " << size - (info.end + info.start) << " "
     << "trim_start " << info.start << " "
     << "trim_end " << info.end
     << ")";
}
void 
PacketPrinter::PrintPayload (std::ostream &os, uint32_t packetUid, uint32_t size,
                             uint32_t fragmentStart, uint32_t fragmentEnd) const
{
  struct PacketPrinter::FragmentInformation info;
  info.start = fragmentStart;
  info.end = fragmentEnd;
  if (!m_payloadPrinter.IsNull ())
    {
      m_payloadPrinter (os, packetUid, size, info);
      return;
    }
  // if the user did not override the payload printer,
  // we print something anyway.
  os << "DATA ("
     << "length " << size - (info.end + info.start);
  if (info.start != 0 || info.end != 0)
    {
      os << " "
         << "trim_start " << info.start << " "
         << "trim_end " << info.end;
    }
  os << ")";

}

} // namespace ns3
