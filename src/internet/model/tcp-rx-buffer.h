/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Adrian Sai-wah Tam
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
 * Author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */

#ifndef TCP_RX_BUFFER_H
#define TCP_RX_BUFFER_H

#include <map>
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/sequence-number.h"
#include "ns3/ptr.h"
#include "ns3/tcp-header.h"

namespace ns3 {
class Packet;

/**
 * \ingroup tcp
 *
 * \brief class for the reordering buffer that keeps the data from lower layer, i.e.
 *        TcpL4Protocol, sent to the application
 */
class TcpRxBuffer : public Object
{
public:
  static TypeId GetTypeId (void);
  TcpRxBuffer (uint32_t n = 0);
  virtual ~TcpRxBuffer ();

  // Accessors
  SequenceNumber32 NextRxSequence (void) const;
  SequenceNumber32 MaxRxSequence (void) const;
  void IncNextRxSequence (void);
  void SetNextRxSequence (const SequenceNumber32& s);
  void SetFinSequence (const SequenceNumber32& s);
  uint32_t MaxBufferSize (void) const;
  void SetMaxBufferSize (uint32_t s);
  uint32_t Size (void) const;
  uint32_t Available () const;
  bool Finished (void);

  /**
   * Insert a packet into the buffer and update the availBytes counter to
   * reflect the number of bytes ready to send to the application. This
   * function handles overlap by triming the head of the inputted packet and
   * removing data from the buffer that overlaps the tail of the inputted
   * packet
   *
   * \return True when success, false otherwise.
   */
  bool Add (Ptr<Packet> p, TcpHeader const& tcph);

  /**
   * Extract data from the head of the buffer as indicated by nextRxSeq.
   * The extracted data is going to be forwarded to the application.
   */
  Ptr<Packet> Extract (uint32_t maxSize);
public:
  typedef std::map<SequenceNumber32, Ptr<Packet> >::iterator BufIterator;
  TracedValue<SequenceNumber32> m_nextRxSeq; //< Seqnum of the first missing byte in data (RCV.NXT)
  SequenceNumber32 m_finSeq;                 //< Seqnum of the FIN packet
  bool m_gotFin;                             //< Did I received FIN packet?
  uint32_t m_size;                           //< Number of total data bytes in the buffer, not necessarily contiguous
  uint32_t m_maxBuffer;                      //< Upper bound of the number of data bytes in buffer (RCV.WND)
  uint32_t m_availBytes;                     //< Number of bytes available to read, i.e. contiguous block at head
  std::map<SequenceNumber32, Ptr<Packet> > m_data;
  //< Corresponding data (may be null)
};

} //namepsace ns3

#endif /* TCP_RX_BUFFER_H */
