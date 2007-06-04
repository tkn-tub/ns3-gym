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
#include "packet-printer.h"

namespace {
class ItemList;
}

namespace ns3 {

class Chunk;
class Buffer;

class PacketHistory {
public:
  static void Enable (void);

  inline PacketHistory (uint32_t uid, uint32_t size);
  inline PacketHistory (PacketHistory const &o);
  inline PacketHistory &operator = (PacketHistory const& o);
  inline ~PacketHistory ();

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
  void Print (std::ostream &os, Buffer buffer, PacketPrinter const &printer) const;

  static void PrintStats (void);

private:
  enum CommandType {
    INIT         = 0,
    ADD_HEADER   = 1,
    REM_HEADER   = 2,
    ADD_TRAILER  = 3,
    REM_TRAILER  = 4,
    ADD_AT_END   = 5,
    REM_AT_START = 6,
    REM_AT_END   = 7,
    PADDING_AT_END = 9,
    LAST
  };
  struct CommandData {
    uint32_t m_count;
    uint32_t m_size;
    uint32_t m_dirtyEnd;
    uint8_t m_data[8];
  };
  typedef std::vector<struct CommandData *> DataFreeList;
  
  PacketHistory ();
  void Reserve (uint32_t n);
  inline void Construct (uint32_t uid, uint32_t size);
  uint32_t GetUleb128Size (uint32_t value) const;
  void AppendValue (uint32_t value);
  uint32_t ReadForwardValue (uint8_t **pBuffer) const;
  uint32_t ReadValue (uint8_t *buffer, uint32_t *n) const;
  void AppendOneCommand (uint32_t type, uint32_t data);
  void AppendOneCommand (uint32_t type, uint32_t data0, uint32_t data1);
  void ReserveCopy (uint32_t size);
  void AddHeader (uint32_t uid, Chunk const & header, uint32_t size);
  void RemoveHeader (uint32_t uid, Chunk const & header, uint32_t size);
  void AddTrailer (uint32_t uid, Chunk const & trailer, uint32_t size);
  void RemoveTrailer (uint32_t uid, Chunk const & trailer, uint32_t size);
  void PrintComplex (std::ostream &os, Buffer buffer, const PacketPrinter &printer) const;
  void BuildItemList (ItemList *list, uint8_t **buffer, uint32_t size, uint32_t n) const;
  inline bool TryToAppendValue (uint32_t value, uint8_t **buffer);
  inline bool TryToAppendSmallValue (uint32_t value, uint8_t **buffer);

  static struct PacketHistory::CommandData *Create (uint32_t size);
  static void Recycle (struct CommandData *data);
  static struct PacketHistory::CommandData *Allocate (uint32_t n);
  static void Deallocate (struct CommandData *data);
  
  static DataFreeList m_freeList;
  static bool m_enable;
  static uint32_t m_maxSize;
  
  struct CommandData *m_data;
  uint32_t m_end;
  uint32_t m_n;
};

}; // namespace ns3

namespace ns3 {

template <typename T>
void 
PacketHistory::AddHeader (T const &header, uint32_t size)
{
  AddHeader (PacketPrinter::GetUid<T> (), header, size);
}

template <typename T>
void 
PacketHistory::RemoveHeader (T const &header, uint32_t size)
{
  RemoveHeader (PacketPrinter::GetUid<T> (), header, size);
}
template <typename T>
void 
PacketHistory::AddTrailer (T const &trailer, uint32_t size)
{
  AddTrailer (PacketPrinter::GetUid<T> (), trailer, size);
}
template <typename T>
void 
PacketHistory::RemoveTrailer (T const &trailer, uint32_t size)
{
  RemoveTrailer (PacketPrinter::GetUid<T> (), trailer, size);
}


PacketHistory::PacketHistory (uint32_t uid, uint32_t size)
  : m_data (0),
    m_end (0),
    m_n (0)
{
  Construct (uid, size);
}
void
PacketHistory::Construct (uint32_t uid, uint32_t size)
{
  if (m_enable) 
    {
      m_data = PacketHistory::Create (0);
      AppendOneCommand (PacketHistory::INIT,
                        size, uid);
    }
}
PacketHistory::PacketHistory (PacketHistory const &o)
  : m_data (o.m_data),
    m_end (o.m_end),
    m_n (o.m_n)
{
  if (m_data != 0) 
    {
      m_data->m_count++;
    }
}
PacketHistory &
PacketHistory::operator = (PacketHistory const& o)
{
  if (m_data == o.m_data) 
    {
      // self assignment
      return *this;
    }
  if (m_data != 0) 
    {
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          PacketHistory::Recycle (m_data);
        }
    }
  m_data = o.m_data;
  m_end = o.m_end;
  m_n = o.m_n;
  if (m_data != 0) 
    {
      m_data->m_count++;
    }
  return *this;
}
PacketHistory::~PacketHistory ()
{
  if (m_data != 0) 
    {
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          PacketHistory::Recycle (m_data);
        }
    }
}


}; // namespace ns3


#endif /* PACKET_HISTORY_H */
