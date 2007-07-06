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

namespace {
  class ItemList;
}

namespace ns3 {

class Chunk;

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
  void SetSeparator (std::string separator);
  /**
   * \param printer printer for payload
   */
  void AddPayloadPrinter (PayloadPrinter printer);
  /**
   * \param printer printer for the specified chunk
   * \param fragmentPrinter printer for a fragment of the specified chunk
   *
   * If the user has not specified a callback for a specific header present
   * in a packet, the "default" callback is invoked. If no such callback
   * was specified, nothing happens.
   */
  template <typename T>
  void AddHeaderPrinter (Callback<void,std::ostream &,uint32_t,uint32_t,const T *> printer,
                         ChunkFragmentPrinter fragmentPrinter);
  /**
   * \param printer printer for the specified chunk
   * \param fragmentPrinter printer for a fragment of the specified chunk
   *
   * If the user has not specified a callback for a specific trailer present
   * in a packet, the "default" callback is invoked. If no such callback
   * was specified, nothing happens.
   */
  template <typename T>
  void AddTrailerPrinter (Callback<void,std::ostream &,uint32_t,uint32_t,const T *> printer,
                          ChunkFragmentPrinter fragmentPrinter);
  /**
   * \param printer printer for a chunk for which no callback was specified explicitely
   */
  void AddDefaultPrinter (DefaultPrinter printer);
  
private:
  friend class PacketMetadata;
  typedef void (*DoPrintCallback) (Ptr<CallbackImplBase>, Buffer::Iterator, std::ostream &,
                                   uint32_t, uint32_t);
  typedef std::string (*DoGetNameCallback) (void);
  struct Printer
  {
    uint32_t m_chunkUid;
    Ptr<CallbackImplBase> m_printer;
    Callback<void,std::ostream &,uint32_t,uint32_t,std::string &,struct PacketPrinter::FragmentInformation>
      m_fragmentPrinter;
  };
  struct RegisteredChunk
  {
    DoPrintCallback printCallback;
    DoGetNameCallback getNameCallback;
    bool isHeader;
  };
  typedef std::vector<struct PacketPrinter::Printer> PrinterList;
  typedef std::vector<struct RegisteredChunk> RegisteredChunks;


  static PacketPrinter GetDefault (void);
  static PacketPrinter *PeekDefault (void);
  static PacketPrinter *CreateStaticDefault (void);
  static void DoDefaultPrintPayload (std::ostream & os,
                                     uint32_t packetUid,
                                     uint32_t size,
                                     struct PacketPrinter::FragmentInformation info);
  static void DoDefaultPrintDefault (std::ostream & os,
                                     uint32_t packetUid,
                                     uint32_t size,
                                     std::string &name,
                                     struct PacketPrinter::FragmentInformation info);
  template <typename T>
  static void DoDefaultPrint (std::ostream &os, uint32_t packetUid, uint32_t size, const T *chunk);
  static void DoDefaultPrintFragment (std::ostream & os,
                                      uint32_t packetUid,
                                      uint32_t size,
                                      std::string &name,
                                      struct PacketPrinter::FragmentInformation info);

  template <typename T>
  static void DoPrint (Ptr<CallbackImplBase> callbackPrinter,
                       Buffer::Iterator i, 
                       std::ostream &os, 
                       uint32_t packetUid,
                       uint32_t size);
  template <typename T>
  static std::string DoGetName (void);
  template <typename T>
  static uint32_t GetTrailerUid (void);
  template <typename T>
  static uint32_t GetHeaderUid (void);
  template <typename T>
  static uint32_t AllocateUid (bool isHeader);
  static RegisteredChunks *GetRegisteredChunks (void);
  static bool IsTrailer (uint32_t uid);
  static bool IsHeader (uint32_t uid);


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
  void DoAddPrinter (uint32_t uid,
                     Ptr<CallbackImplBase> printer,
                     Callback<void,
                       std::ostream &, 
                       uint32_t, 
                       uint32_t, 
                       std::string &,
                       struct PacketPrinter::FragmentInformation> fragmentPrinter);

  static PacketPrinter m_defaultPacketPrinter;
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
PacketPrinter::AddHeaderPrinter (Callback<void,std::ostream &, uint32_t, uint32_t, const T *> printer,
			   Callback<void,
		            std::ostream &, 
		            uint32_t, 
                            uint32_t, 
                            std::string &,
		            struct PacketPrinter::FragmentInformation> fragmentPrinter)
{
  static uint32_t uid = PacketPrinter::GetHeaderUid<T> ();
  DoAddPrinter (uid, printer.GetImpl (), fragmentPrinter);
}

template <typename T>
void 
PacketPrinter::AddTrailerPrinter (Callback<void,std::ostream &, uint32_t, uint32_t, const T *> printer,
			   Callback<void,
		            std::ostream &, 
		            uint32_t, 
                            uint32_t, 
                            std::string &,
		            struct PacketPrinter::FragmentInformation> fragmentPrinter)
{
  static uint32_t uid = PacketPrinter::GetTrailerUid<T> ();
  DoAddPrinter (uid, printer.GetImpl (), fragmentPrinter);
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
PacketPrinter::GetHeaderUid (void)
{
  static uint32_t uid = PacketPrinter::AllocateUid<T> (true);
  return uid;
}

template <typename T>
uint32_t 
PacketPrinter::GetTrailerUid (void)
{
  static uint32_t uid = PacketPrinter::AllocateUid<T> (false);
  return uid;
}

template <typename T>
uint32_t
PacketPrinter::AllocateUid (bool isHeader)
{
  RegisteredChunks *chunks = PacketPrinter::GetRegisteredChunks ();
  RegisteredChunk chunk;
  chunk.printCallback = &PacketPrinter::DoPrint<T>;
  chunk.getNameCallback = &PacketPrinter::DoGetName<T>;
  chunk.isHeader = isHeader;
  chunks->push_back (chunk);
  uint32_t uid = chunks->size () * 2;
  PacketPrinter::PeekDefault ()->DoAddPrinter (uid, 
                                               MakeCallback (&PacketPrinter::DoDefaultPrint<T>).GetImpl (),
                                               MakeCallback (&PacketPrinter::DoDefaultPrintFragment));
  return uid;
}

template <typename T>
void 
PacketPrinter::DoDefaultPrint (std::ostream &os, uint32_t packetUid, uint32_t size, const T *chunk)
{
  os << chunk->GetName () << " ";
  chunk->Print (os);
}



} // namespace ns3

#endif /* PACKET_PRINTER_H */
