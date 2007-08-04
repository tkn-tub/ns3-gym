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
#include "buffer.h"
#include "chunk-registry.h"
#include <vector>

namespace ns3 {

/**
 * \brief hold a list of print callbacks for packet headers and trailers
 *
 * Users can register in instances of this class print callbacks
 * which are used by Packet::Print to print the content of a packet.
 */
class PacketPrinter 
{
public:
  /**
   * \brief indicates how many bytes were trimmed from a header
   * or a trailer.
   */
  struct FragmentInformation
  {
    /**
     * The number of bytes trimmed from the start of the header or the trailer.
     */
    uint32_t start;
    /**
     * The number of bytes trimmed from the end of the header or the trailer.
     */
    uint32_t end;
  };
  /**
   * \brief callback to print payload.
   *
   * Arguments: output stream, packet uid, size, fragment information
   */
  typedef Callback<void,std::ostream &,uint32_t,uint32_t,struct PacketPrinter::FragmentInformation>
    PayloadPrinter;

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

  PacketPrinter ();

  /**
   * Print the content of the packet forward.
   */
  void PrintForward (void);
  /**
   * Print the content of the packet backward.
   */
  void PrintBackward (void);
  /**
   * \param separator the new separator
   *
   * The default separator is a single space character.
   */
  void SetSeparator (std::string separator);
  /**
   * \param printer printer for payload
   */
  void SetPayloadPrinter (PayloadPrinter printer);
  /**
   * \param printer printer for the specified chunk
   * \param fragmentPrinter printer for a fragment of the specified chunk
   *
   * If the user does not force a user-specific printing function through
   * a call to SetHeaderPrinter, the default print output is generated.
   */
  template <typename T>
  void SetHeaderPrinter (Callback<void,std::ostream &,uint32_t,uint32_t,const T *> printer,
                         ChunkFragmentPrinter fragmentPrinter);
  /**
   * \param printer printer for the specified chunk
   * \param fragmentPrinter printer for a fragment of the specified chunk
   *
   * If the user does not force a user-specific printing function through
   * a call to SetTrailerPrinter, the default print output is generated.
   */
  template <typename T>
  void SetTrailerPrinter (Callback<void,std::ostream &,uint32_t,uint32_t,const T *> printer,
                          ChunkFragmentPrinter fragmentPrinter);  
private:
  friend class PacketMetadata;
  void PrintChunk (uint32_t uid, 
		   Buffer::Iterator i, 
		   std::ostream &os, 
		   uint32_t packetUid,
		   uint32_t size) const;
  void PrintChunkFragment (uint32_t uid,
			   std::ostream &os,
			   uint32_t packetUid,
			   uint32_t size,
			   uint32_t fragmentStart,
			   uint32_t fragmentEnd) const;
  void PrintPayload (std::ostream &os, uint32_t packetUid, uint32_t size,
		     uint32_t fragmentStart, uint32_t fragmentEnd) const;
  struct Printer
  {
    uint32_t m_chunkUid;
    Ptr<CallbackImplBase> m_printer;
    Callback<void,std::ostream &,uint32_t,uint32_t,std::string &,
             struct PacketPrinter::FragmentInformation> m_fragmentPrinter;
  };
  typedef std::vector<struct PacketPrinter::Printer> PrinterList;
  
  PrinterList m_printerList;
  PayloadPrinter m_payloadPrinter;
  DefaultPrinter m_defaultPrinter;
  bool m_forward;
  std::string m_separator;
};

} // namespace ns3

namespace ns3 {

template <typename T>
void 
PacketPrinter::SetHeaderPrinter (Callback<void,std::ostream &,uint32_t,uint32_t,const T *> printer,
                                 ChunkFragmentPrinter fragmentPrinter)
{
  Printer p;
  p.m_chunkUid = ChunkRegistry::GetHeaderUid<T> ();
  p.m_printer = printer.GetImpl ();
  p.m_fragmentPrinter = fragmentPrinter;
  m_printerList.push_back (p);
}

template <typename T>
void 
PacketPrinter::SetTrailerPrinter (Callback<void,std::ostream &,uint32_t,uint32_t,const T *> printer,
                                 ChunkFragmentPrinter fragmentPrinter)
{
  Printer p;
  p.m_chunkUid = ChunkRegistry::GetTrailerUid<T> ();
  p.m_printer = printer.GetImpl ();
  p.m_fragmentPrinter = fragmentPrinter;
  m_printerList.push_back (p);
}


} // namespace ns3

#endif /* PACKET_PRINTER_H */
