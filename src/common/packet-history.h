/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
#ifndef PACKET_HISTORY_H
#define PACKET_HISTORY_H

#include <stdint.h>
#include <vector>
#include "ns3/callback.h"

namespace ns3 {

class Chunk;
class Buffer;

class PacketPrinter 
{
public:
  void PrintForward (void);
  void PrintBackward (void);
  template <typename T>
  void Add (Callback<void,std::ostream &, T> cb);
  
private:
  std::vector <std::pair<uint32_t,CallbackBase> > m_printers;
  bool m_forward;
};

class PacketHistory {
public:
  static void Enable (void);

  PacketHistory (uint32_t uid, uint32_t size);
  PacketHistory (PacketHistory const &o);
  PacketHistory &operator = (PacketHistory const& o);
  ~PacketHistory ();

  template <typename T>
  void AddHeader (T const &header, uint32_t size);
  template <typename T>
  void RemoveHeader (T const &header, uint32_t size);

  template <typename T>
  void AddTrailer (T const &trailer, uint32_t size);
  template <typename T>
  void RemoveTrailer (T const &trailer, uint32_t size);

  PacketHistory CreateFragment (uint32_t start, uint32_t end) const;
  void AddAtEnd (PacketHistory const&o);
  void AddPaddingAtEnd (uint32_t end);
  void RemoveAtStart (uint32_t start);
  void RemoveAtEnd (uint32_t end);

  void PrintDefault (std::ostream &os, Buffer buffer) const;
  void PrintSimple (std::ostream &os, Buffer buffer) const;
  void PrintComplex (std::ostream &os, Buffer buffer) const;
  void Print (std::ostream &os, Buffer buffer, PacketPrinter printer) const;

private:
  enum CommandType {
    INIT_UID     = 0,
    ADD_HEADER   = 1,
    REM_HEADER   = 2,
    ADD_TRAILER  = 3,
    REM_TRAILER  = 4,
    ADD_AT_END   = 5,
    REM_AT_START = 6,
    REM_AT_END   = 7,
    PADDING_AT_END = 9,
    INIT_SIZE    = 10,
    LAST
  };
  struct CommandData {
    uint32_t m_count;
    uint32_t m_size;
    uint32_t m_dirtyEnd;
    uint8_t m_data[8];
  };
  typedef std::vector<std::pair<uint32_t,uint32_t> > HeadersToPrint;
  typedef std::vector<std::pair<uint32_t,uint32_t> > TrailersToPrint;
  template <typename T>
  class ChunkUid {
  public:
    static const uint32_t GetUid (void);
    static Chunk *CreateStatic (void);
  };
  typedef std::vector<std::pair<uint32_t, Chunk *(*) (void)> > ChunkFactories;
  typedef std::vector<std::pair<uint32_t, Chunk *(*) (void)> >::iterator ChunkFactoriesI;
  typedef std::vector<struct CommandData *> DataFreeList;
  
  PacketHistory ();
  void Reserve (uint32_t n);
  void Construct (uint32_t uid, uint32_t size);
  uint32_t GetUleb128Size (uint32_t value) const;
  uint32_t GetReverseUleb128Size (uint8_t *buffer) const;
  void AppendValue (uint32_t value);
  uint32_t ReadForwardValue (uint8_t **pBuffer) const;
  uint32_t ReadReverseValue (uint8_t **pBuffer) const;
  uint32_t ReadValue (uint8_t *buffer, uint32_t *n) const;
  void AppendOneCommand (uint32_t type, uint32_t data);
  void AppendOneCommand (uint32_t type, uint32_t data0, uint32_t data1);
  void ReserveCopy (uint32_t size);
  void AddHeader (uint32_t uid, Chunk const & header, uint32_t size);
  void RemoveHeader (uint32_t uid, Chunk const & header, uint32_t size);
  void AddTrailer (uint32_t uid, Chunk const & trailer, uint32_t size);
  void RemoveTrailer (uint32_t uid, Chunk const & trailer, uint32_t size);
  static struct PacketHistory::CommandData *Create (uint32_t size);
  static void Recycle (struct CommandData *data);
  static struct PacketHistory::CommandData *Allocate (uint32_t n);
  static void Deallocate (struct CommandData *data);
  static Chunk *CreateStatic (uint32_t uid);
  static uint32_t RegisterChunkFactory (Chunk *(*createStatic) (void));  
  
  static DataFreeList m_freeList;
  static ChunkFactories m_factories;
  static bool m_enable;
  static uint32_t m_maxSize;
  
  struct CommandData *m_data;
  uint32_t m_end;
  uint32_t m_n;
  bool m_aggregated;
};

}; // namespace ns3

namespace ns3 {

template <typename T>
const uint32_t 
PacketHistory::ChunkUid<T>::GetUid (void)
{
  static uint32_t uid = 
    PacketHistory::RegisterChunkFactory (&PacketHistory::ChunkUid<T>::CreateStatic);
  return uid;
}
template <typename T>
Chunk *
PacketHistory::ChunkUid<T>::CreateStatic (void)
{
  static T chunk = T ();
  return &chunk;
}


template <typename T>
void 
PacketHistory::AddHeader (T const &header, uint32_t size)
{
  AddHeader (ChunkUid<T>::GetUid (), header, size);
}

template <typename T>
void 
PacketHistory::RemoveHeader (T const &header, uint32_t size)
{
  RemoveHeader (ChunkUid<T>::GetUid (), header, size);
}
template <typename T>
void 
PacketHistory::AddTrailer (T const &trailer, uint32_t size)
{
  AddTrailer (ChunkUid<T>::GetUid (), trailer, size);
}
template <typename T>
void 
PacketHistory::RemoveTrailer (T const &trailer, uint32_t size)
{
  RemoveTrailer (ChunkUid<T>::GetUid (), trailer, size);
}

}; // namespace ns3


#endif /* PACKET_HISTORY_H */
