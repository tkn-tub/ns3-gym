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
#include <vector>

namespace ns3 {

class Chunk;

class PacketPrinter 
{
public:
  struct FragmentInformation
  {
    uint32_t start;
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
  struct Printer
  {
    uint32_t m_chunkUid;
    Ptr<CallbackImplBase> m_printer;
    Callback<void,std::ostream &,uint32_t,uint32_t,std::string &,struct PacketPrinter::FragmentInformation>
      m_fragmentPrinter;
  };
  typedef void (*DoPrintCallback) (Ptr<CallbackImplBase>, Buffer::Iterator, std::ostream &,
                                   uint32_t, uint32_t);
  typedef std::string (*DoGetNameCallback) (void);
  typedef std::vector<struct PacketPrinter::Printer> PrinterList;
  typedef std::vector<std::pair<DoPrintCallback,DoGetNameCallback> > RegisteredChunks;


  static PacketPrinter GetDefault (void);

  template <typename T>
  static void DoPrint (Ptr<CallbackImplBase> callbackPrinter,
                       Buffer::Iterator i, 
                       std::ostream &os, 
                       uint32_t packetUid,
                       uint32_t size);
  template <typename T>
  static std::string DoGetName (void);
  template <typename T>
  static uint32_t GetUid (void);
  template <typename T>
  static uint32_t AllocateUid (void);
  static RegisteredChunks *GetRegisteredChunks (void);


  void PrintChunk (uint32_t uid, 
		   Buffer::Iterator i, 
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

  static PacketPrinter m_defaultPacketPrinter;
  PrinterList m_printerList;
  PayloadPrinter m_payloadPrinter;
  DefaultPrinter m_defaultPrinter;
  bool m_forward;
};

} // namespace ns3

namespace ns3 {

template <typename T>
void 
PacketPrinter::AddPrinter (Callback<void,std::ostream &, uint32_t, uint32_t, const T *> printer,
			   Callback<void,
		            std::ostream &, 
		            uint32_t, 
                            uint32_t, 
                            std::string &,
		            struct PacketPrinter::FragmentInformation> fragmentPrinter)
{
  static uint32_t uid = PacketPrinter::GetUid<T> ();
  struct PacketPrinter::Printer p;
  p.m_chunkUid = uid;
  p.m_printer = printer.PeekImpl ();
  p.m_fragmentPrinter = fragmentPrinter;
  m_printerList.push_back (p);
}

template <typename T>
void
PacketPrinter::DoPrint (Ptr<CallbackImplBase> printerCallback,
                        Buffer::Iterator i,
                        std::ostream &os, 
                        uint32_t packetUid,
                        uint32_t size)
{
  T chunk = T ();
  chunk.Deserialize (i);
  Callback<void,std::ostream&,uint32_t,uint32_t,const T*> callback;
  callback.Assign (printerCallback);
  callback (os, packetUid, size, &chunk);
}

template <typename T>
std::string
PacketPrinter::DoGetName (void)
{
  T chunk = T ();
  return chunk.GetName ();
}

template <typename T>
uint32_t 
PacketPrinter::GetUid (void)
{
  static uint32_t uid = PacketPrinter::AllocateUid<T> ();
  return uid;
}

template <typename T>
uint32_t
PacketPrinter::AllocateUid (void)
{
  RegisteredChunks *chunks = PacketPrinter::GetRegisteredChunks ();
  chunks->push_back (std::make_pair(&PacketPrinter::DoPrint<T>, 
                                    &PacketPrinter::DoGetName<T>));
  return chunks->size ();
}


} // namespace ns3

#endif /* PACKET_PRINTER_H */
