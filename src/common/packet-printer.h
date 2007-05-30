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
#ifndef PACKET_PRINTER_H
#define PACKET_PRINTER_H

#include "ns3/callback.h"
#include "ns3/ptr.h"

namespace ns3 {

class PacketPrinter 
{
public:
  /**
   * \brief callback to print payload.
   *
   * Arguments: output stream, packet uid, size, fragment information
   */
  typedef Callback<void,std::ostream &,uint32_t,uint32_t,struct PacketPrinter::FragmentInformation>
    PayloadPrinter;

  /**
   * \brief callback to print whole chunks.
   *
   * Arguments: output stream, packet uid, size, header/trailer type instance
   */
  typedef Callback<void,std::ostream &, uint32_t, uint32_t, const Chunk *> ChunkPrinter;

  /**
   * \brief callback to print fragmented chunks.
   *
   * Arguments: output stream, packet uid, size, header/trailer name, fragment information
   */
  typedef Callback<void,std::ostream &,uint32_t,uint32_t,std::string &,struct PacketPrinter::FragmentInformation>
    ChunkFragmentPrinter;

  /**
   * \brief callback to print chunks for which no specific callback was specified.
   *
   * Arguments: output stream, packet uid, size, header/trailer name, fragment information
   */
  typedef Callback<void,std::ostream&,uint32_t,uint32_t,std::string&,struct PacketPrinter::FragmentInformation> 
    DefaultPrinter;

  struct FragmentInformation
  {
    uint32_t start;
    uint32_t end;
  };

  /**
   * Print the content of the packet forward.
   */
  void PrintForward (void);
  /**
   * Print the content of the packet backward.
   */
  void PrintBackward (void);
  /**
   * \param printer printer for payload
   */
  void AddPayloadPrinter (PayloadPrinter printer);
  /**
   * \param printer printer for the specified chunk
   * \param fragmentPrinter printer for a fragment of the specified chunk
   */
  template <typename T>
  void AddPrinter (Callback<void,std::ostream &,uint32_t,uint32_t,const T *> printer,
		   ChunkFragmentPrinter fragmentPrinter);
  /**
   * \param printer printer for a chunk for which no callback was specified explicitely
   */
  void AddDefaultPrinter (DefaultPrinter printer);
  
private:
  friend class PacketHistory;

  static PacketPrinter GetDefault (void);
  template <typename T>
  static uint32_t GetUid (void) const;
  static std::string GetName (uint32_t uid) const;
  static T &CreateStatic (uint32_t uid) const;

  void PrintChunk (uint32_t uid, 
		   Buffer::iterator i, 
		   std::ostream &os, 
		   uint32_t packetUid,
		   uint32_t size);
  void PrintChunkFragment (uint32_t uid,
			   std::ostream &os,
			   uint32_t packetUid,
			   uint32_t size,
			   uint32_t fragmentStart,
			   uint32_t fragmentEnd);
  void PrintPayload (std::ostream &os, uint32_t packetUid, uint32_t size,
		     uint32_t fragmentStart, uint32_t fragmentEnd);
  struct Printer
  {
    uint32_t m_uid;
    Ptr<CallbackImplBase> m_printer;
    Callback<void,std::ostream &,uint32_t,uint32_t,std::string &,struct PacketPrinter::FragmentInformation>
      m_fragmentPrinter;
  };
  std::vector <struct Printer> m_printers;
  bool m_forward;
};

} // namespace ns3

namespace ns3 {

template <typename T>
void 
PacketHistory::AddPrinter (Callback<void,std::ostream &, uint32_t, uint32_t, const T *> printer,
			   Callback<void,
		            std::ostream &, 
		            uint32_t, 
                            uint32_t, 
                            std::string &,
		            struct PacketPrinter::FragmentInformation> fragmentPrinter)
{
  uint32_t uid = PacketHistory::GetUid<T> ();
  struct Printer printer;
  printer.m_uid = uid;
  printer.m_printer = printer.PeekImpl ();
  printer.m_fragmentPrinter = fragmentPrinter;
  m_printers.push_back (printer);
}

} // namespace ns3

#endif /* PACKET_PRINTER_H */
