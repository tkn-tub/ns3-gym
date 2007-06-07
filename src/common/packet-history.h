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
#include "ns3/assert.h"
#include "packet-printer.h"

namespace ns3 {

class Chunk;
class Buffer;

class PacketHistory {
public:
  static void Enable (void);
  static void SetOptOne (bool optOne);

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
  /**
     head -(next)-> tail
       ^             |
        \---(prev)---|
   */
  struct Data {
    uint16_t m_count;
    uint16_t m_size;
    uint16_t m_dirtyEnd;
    uint8_t m_data[10];
  };
  struct SmallItem {
    uint16_t next;
    uint16_t prev;
    uint32_t typeUid;
    uint32_t size;
    uint16_t chunkUid;
  };
  struct ExtraItem {
    uint32_t fragmentStart;
    uint32_t fragmentEnd;
    uint32_t packetUid;
  };

  typedef std::vector<struct Data *> DataFreeList;
  
  PacketHistory ();
  void DoAddHeader (uint32_t uid, uint32_t size);
  void DoRemoveHeader (uint32_t uid, uint32_t size);
  void DoAddTrailer (uint32_t uid, uint32_t size);
  void DoRemoveTrailer (uint32_t uid, uint32_t size);

  inline uint16_t AddSmall (const PacketHistory::SmallItem *item);
  uint16_t AddBig (uint32_t head, uint32_t tail,
                   const PacketHistory::SmallItem *item, 
                   const PacketHistory::ExtraItem *extraItem);
  void ReplaceTail (PacketHistory::SmallItem *item, 
                    PacketHistory::ExtraItem *extraItem,
                    uint32_t available);
  inline void UpdateHead (uint16_t written);
  inline void UpdateTail (uint16_t written);
  uint32_t GetUleb128Size (uint32_t value) const;
  uint32_t ReadUleb128 (const uint8_t **pBuffer) const;
  inline void Append16 (uint16_t value, uint8_t *buffer);
  inline bool TryToAppend (uint32_t value, uint8_t **pBuffer, uint8_t *end);
  inline bool TryToAppendFast (uint32_t value, uint8_t **pBuffer, uint8_t *end);
  inline bool TryToAppend32 (uint32_t value, uint8_t **pBuffer, uint8_t *end);
  inline bool TryToAppend16 (uint16_t value, uint8_t **pBuffer, uint8_t *end);
  void AppendValue (uint32_t value, uint8_t *buffer);
  void AppendValueExtra (uint32_t value, uint8_t *buffer);
  inline void Reserve (uint32_t n);
  void ReserveCopy (uint32_t n);
  uint32_t DoPrint (struct PacketHistory::SmallItem *item, uint32_t current,
                    Buffer data, uint32_t offset, const PacketPrinter &printer,
                    std::ostream &os) const;
  uint32_t GetTotalSize (void) const;
  uint32_t ReadItems (uint16_t current, 
                      struct PacketHistory::SmallItem *item,
                      struct PacketHistory::ExtraItem *extraItem) const;


  static struct PacketHistory::Data *Create (uint32_t size);
  static void Recycle (struct PacketHistory::Data *data);
  static struct PacketHistory::Data *Allocate (uint32_t n);
  static void Deallocate (struct PacketHistory::Data *data);
  
  static DataFreeList m_freeList;
  static bool m_enable;
  static uint32_t m_maxSize;
  static uint16_t m_chunkUid;
  
  struct Data *m_data;
  uint16_t m_head;
  uint16_t m_tail;
  uint16_t m_used;
  uint32_t m_packetUid;
};

}; // namespace ns3

namespace ns3 {

template <typename T>
void 
PacketHistory::AddHeader (T const &header, uint32_t size)
{
  DoAddHeader (PacketPrinter::GetHeaderUid<T> (), size);
}

template <typename T>
void 
PacketHistory::RemoveHeader (T const &header, uint32_t size)
{
  DoRemoveHeader (PacketPrinter::GetHeaderUid<T> (), size);
}
template <typename T>
void 
PacketHistory::AddTrailer (T const &trailer, uint32_t size)
{
  DoAddTrailer (PacketPrinter::GetTrailerUid<T> (), size);
}
template <typename T>
void 
PacketHistory::RemoveTrailer (T const &trailer, uint32_t size)
{
  DoRemoveTrailer (PacketPrinter::GetTrailerUid<T> (), size);
}


PacketHistory::PacketHistory (uint32_t uid, uint32_t size)
  : m_data (m_data = PacketHistory::Create (10)),
    m_head (0xffff),
    m_tail (0xffff),
    m_used (0),
    m_packetUid (uid)
{
  memset (m_data->m_data, 0xff, 4);
  if (size > 0)
    {
      DoAddHeader (0, size);
    }
}
PacketHistory::PacketHistory (PacketHistory const &o)
  : m_data (o.m_data),
    m_head (o.m_head),
    m_tail (o.m_tail),
    m_used (o.m_used),
    m_packetUid (o.m_packetUid)
{
  NS_ASSERT (m_data != 0);
  m_data->m_count++;
}
PacketHistory &
PacketHistory::operator = (PacketHistory const& o)
{
  if (m_data == o.m_data) 
    {
      // self assignment
      return *this;
    }
  NS_ASSERT (m_data != 0);
  m_data->m_count--;
  if (m_data->m_count == 0) 
    {
      PacketHistory::Recycle (m_data);
    }
  m_data = o.m_data;
  m_head = o.m_head;
  m_tail = o.m_tail;
  m_used = o.m_used;
  m_packetUid = o.m_packetUid;
  NS_ASSERT (m_data != 0);
  m_data->m_count++;
  return *this;
}
PacketHistory::~PacketHistory ()
{
  NS_ASSERT (m_data != 0);
  m_data->m_count--;
  if (m_data->m_count == 0) 
    {
      PacketHistory::Recycle (m_data);
    }
}

}; // namespace ns3


#endif /* PACKET_HISTORY_H */
