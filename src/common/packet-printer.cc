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

namespace ns3 {

PacketPrinter::PacketPrinter ()
  : m_forward (true),
    m_separator ("")
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
PacketPrinter::AddPayloadPrinter (PayloadPrinter printer)
{
  m_payloadPrinter = printer;
}
void 
PacketPrinter::AddDefaultPrinter (DefaultPrinter printer)
{
  m_defaultPrinter = printer;
}

PacketPrinter::RegisteredChunks *
PacketPrinter::GetRegisteredChunks (void)
{
  static RegisteredChunks registeredChunks;
  return &registeredChunks;
}

PacketPrinter 
PacketPrinter::GetDefault (void)
{
  return *(PacketPrinter::PeekDefault ());
}
PacketPrinter *
PacketPrinter::PeekDefault (void)
{
  static PacketPrinter *tmp = PacketPrinter::CreateStaticDefault ();
  return tmp;
}
PacketPrinter *
PacketPrinter::CreateStaticDefault (void)
{
  static PacketPrinter tmp;
  tmp.PrintForward ();
  tmp.AddPayloadPrinter (MakeCallback (&PacketPrinter::DoDefaultPrintPayload));
  tmp.SetSeparator (" ");
  return &tmp;
}


void 
PacketPrinter::PrintChunk (uint32_t chunkUid, 
                           Buffer::Iterator start, 
                           std::ostream &os, 
                           uint32_t packetUid,
                           uint32_t size) const
{
  RegisteredChunks *registeredChunks = PacketPrinter::GetRegisteredChunks ();
  NS_ASSERT (chunkUid >= 1 && chunkUid/2 <= registeredChunks->size ());
  for (PrinterList::const_iterator i = m_printerList.begin (); i != m_printerList.end (); i++)
    {
      if (i->m_chunkUid == chunkUid)
        {
          DoPrintCallback cb = (*registeredChunks)[chunkUid/2-1].printCallback;
          cb (i->m_printer, start, os, packetUid, size);
          return;
        }
    }
  DoGetNameCallback cb = (*registeredChunks)[chunkUid/2-1].getNameCallback;
  std::string name = cb ();
  struct PacketPrinter::FragmentInformation info;
  info.start = 0;
  info.end = 0;
  if (!m_defaultPrinter.IsNull ())
    {
      m_defaultPrinter (os, packetUid, size, name, info);
    }
}
void 
PacketPrinter::PrintChunkFragment (uint32_t chunkUid,
                                   std::ostream &os,
                                   uint32_t packetUid,
                                   uint32_t size,
                                   uint32_t fragmentStart,
                                   uint32_t fragmentEnd) const
{
  RegisteredChunks *registeredChunks = PacketPrinter::GetRegisteredChunks ();
  NS_ASSERT (chunkUid >= 1 && chunkUid/2 <= registeredChunks->size ());
  DoGetNameCallback cb = (*registeredChunks)[chunkUid/2-1].getNameCallback;
  std::string name = cb ();
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
  if (!m_defaultPrinter.IsNull ())
    {
      m_defaultPrinter (os, packetUid, size, name, info);
    }
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
    }
}

void 
PacketPrinter::DoDefaultPrintPayload (std::ostream & os,
                                      uint32_t packetUid,
                                      uint32_t size,
                                      struct PacketPrinter::FragmentInformation info)
{
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

void 
PacketPrinter::DoDefaultPrintDefault (std::ostream & os,
                                      uint32_t packetUid,
                                      uint32_t size,
                                      std::string &name,
                                      struct PacketPrinter::FragmentInformation info)
{
  NS_ASSERT_MSG (false, "This should never happen because we provide a printer for _all_ chunk types.");
}

void 
PacketPrinter::DoDefaultPrintFragment (std::ostream & os,
                                       uint32_t packetUid,
                                       uint32_t size,
                                       std::string &name,
                                       struct PacketPrinter::FragmentInformation info)
{
  NS_ASSERT (info.start != 0 || info.end != 0);
  os << name << " "
     << "("
     << "length " << size - (info.end + info.start) << " "
     << "trim_start " << info.start << " "
     << "trim_end " << info.end
     << ")"
    ;
}

void
PacketPrinter::DoAddPrinter (uint32_t uid,
                             Ptr<CallbackImplBase> printer,
                             Callback<void,
                             std::ostream &, 
                             uint32_t, 
                             uint32_t, 
                             std::string &,
                             struct PacketPrinter::FragmentInformation> fragmentPrinter)
{
  struct PacketPrinter::Printer p;
  p.m_chunkUid = uid;
  p.m_printer = printer;
  p.m_fragmentPrinter = fragmentPrinter;
  m_printerList.push_back (p);
}

bool 
PacketPrinter::IsTrailer (uint32_t uid)
{
  RegisteredChunks *registeredChunks = PacketPrinter::GetRegisteredChunks ();
  NS_ASSERT (uid >= 1 && uid/2 <= registeredChunks->size ());
  bool isHeader = (*registeredChunks)[uid/2-1].isHeader;
  return !isHeader;
}
bool 
PacketPrinter::IsHeader (uint32_t uid)
{
  RegisteredChunks *registeredChunks = PacketPrinter::GetRegisteredChunks ();
  NS_ASSERT (uid >= 1 && uid/2 <= registeredChunks->size ());
  bool isHeader = (*registeredChunks)[uid/2-1].isHeader;
  return isHeader;
}



} // namespace ns3
