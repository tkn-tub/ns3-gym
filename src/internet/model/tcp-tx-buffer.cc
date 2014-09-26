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

#include <iostream>
#include <algorithm>
#include <cstring>

#include "ns3/packet.h"
#include "ns3/fatal-error.h"
#include "ns3/log.h"

#include "tcp-tx-buffer.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpTxBuffer");

TypeId
TcpTxBuffer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpTxBuffer")
    .SetParent<Object> ()
    .AddConstructor<TcpTxBuffer> ()
    .AddTraceSource ("UnackSequence",
                     "First unacknowledged sequence number (SND.UNA)",
                     MakeTraceSourceAccessor (&TcpTxBuffer::m_firstByteSeq))
  ;
  return tid;
}

/* A user is supposed to create a TcpSocket through a factory. In TcpSocket,
 * there are attributes SndBufSize and RcvBufSize to control the default Tx and
 * Rx window sizes respectively, with default of 128 KiByte. The attribute
 * SndBufSize is passed to TcpTxBuffer by TcpSocketBase::SetSndBufSize() and in
 * turn, TcpTxBuffer:SetMaxBufferSize(). Therefore, the m_maxBuffer value
 * initialized below is insignificant.
 */
TcpTxBuffer::TcpTxBuffer (uint32_t n)
  : m_firstByteSeq (n), m_size (0), m_maxBuffer (32768), m_data (0)
{
}

TcpTxBuffer::~TcpTxBuffer (void)
{
}

SequenceNumber32
TcpTxBuffer::HeadSequence (void) const
{
  return m_firstByteSeq;
}

SequenceNumber32
TcpTxBuffer::TailSequence (void) const
{
  return m_firstByteSeq + SequenceNumber32 (m_size);
}

uint32_t
TcpTxBuffer::Size (void) const
{
  return m_size;
}

uint32_t
TcpTxBuffer::MaxBufferSize (void) const
{
  return m_maxBuffer;
}

void
TcpTxBuffer::SetMaxBufferSize (uint32_t n)
{
  m_maxBuffer = n;
}

uint32_t
TcpTxBuffer::Available (void) const
{
  return m_maxBuffer - m_size;
}

bool
TcpTxBuffer::Add (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  NS_LOG_LOGIC ("Packet of size " << p->GetSize () << " appending to window starting at "
                                  << m_firstByteSeq << ", availSize="<< Available ());
  if (p->GetSize () <= Available ())
    {
      if (p->GetSize () > 0)
        {
          m_data.push_back (p);
          m_size += p->GetSize ();
          NS_LOG_LOGIC ("Updated size=" << m_size << ", lastSeq=" << m_firstByteSeq + SequenceNumber32 (m_size));
        }
      return true;
    }
  NS_LOG_LOGIC ("Rejected. Not enough room to buffer packet.");
  return false;
}

uint32_t
TcpTxBuffer::SizeFromSequence (const SequenceNumber32& seq) const
{
  NS_LOG_FUNCTION (this << seq);
  // Sequence of last byte in buffer
  SequenceNumber32 lastSeq = m_firstByteSeq + SequenceNumber32 (m_size);
  // Non-negative size
  NS_LOG_LOGIC ("HeadSeq=" << m_firstByteSeq << ", lastSeq=" << lastSeq << ", size=" << m_size <<
                ", returns " << lastSeq - seq);
  return lastSeq - seq;
}

Ptr<Packet>
TcpTxBuffer::CopyFromSequence (uint32_t numBytes, const SequenceNumber32& seq)
{
  NS_LOG_FUNCTION (this << numBytes << seq);
  uint32_t s = std::min (numBytes, SizeFromSequence (seq)); // Real size to extract. Insure not beyond end of data
  if (s == 0)
    {
      return Create<Packet> (); // Empty packet returned
    }
  if (m_data.size () == 0)
    { // No actual data, just return dummy-data packet of correct size
      return Create<Packet> (s);
    }

  // Extract data from the buffer and return
  uint32_t offset = seq - m_firstByteSeq.Get ();
  uint32_t count = 0;      // Offset of the first byte of a packet in the buffer
  uint32_t pktSize = 0;
  bool beginFound = false;
  int pktCount = 0;
  Ptr<Packet> outPacket;
  NS_LOG_LOGIC ("There are " << m_data.size () << " number of packets in buffer");
  for (BufIterator i = m_data.begin (); i != m_data.end (); ++i)
    {
      pktCount++;
      pktSize = (*i)->GetSize ();
      if (!beginFound)
        { // Look for first fragment
          if (count + pktSize > offset)
            {
              NS_LOG_LOGIC ("First byte found in packet #" << pktCount << " at buffer offset " << count
                                                           << ", packet len=" << pktSize);
              beginFound = true;
              uint32_t packetOffset = offset - count;
              uint32_t fragmentLength = count + pktSize - offset;
              if (fragmentLength >= s)
                { // Data to be copied falls entirely in this packet
                  return (*i)->CreateFragment (packetOffset, s);
                }
              else
                { // This packet only fulfills part of the request
                  outPacket = (*i)->CreateFragment (packetOffset, fragmentLength);
                }
              NS_LOG_LOGIC ("Output packet is now of size " << outPacket->GetSize ());
            }
        }
      else if (count + pktSize >= offset + s)
        { // Last packet fragment found
          NS_LOG_LOGIC ("Last byte found in packet #" << pktCount << " at buffer offset " << count
                                                      << ", packet len=" << pktSize);
          uint32_t fragmentLength = offset + s - count;
          Ptr<Packet> endFragment = (*i)->CreateFragment (0, fragmentLength);
          outPacket->AddAtEnd (endFragment);
          NS_LOG_LOGIC ("Output packet is now of size " << outPacket->GetSize ());
          break;
        }
      else
        {
          NS_LOG_LOGIC ("Appending to output the packet #" << pktCount << " of offset " << count << " len=" << pktSize);
          outPacket->AddAtEnd (*i);
          NS_LOG_LOGIC ("Output packet is now of size " << outPacket->GetSize ());
        }
      count += pktSize;
    }
  NS_ASSERT (outPacket->GetSize () == s);
  return outPacket;
}

void
TcpTxBuffer::SetHeadSequence (const SequenceNumber32& seq)
{
  NS_LOG_FUNCTION (this << seq);
  m_firstByteSeq = seq;
}

void
TcpTxBuffer::DiscardUpTo (const SequenceNumber32& seq)
{
  NS_LOG_FUNCTION (this << seq);
  NS_LOG_LOGIC ("current data size=" << m_size << ", headSeq=" << m_firstByteSeq << ", maxBuffer=" << m_maxBuffer
                                     << ", numPkts=" << m_data.size ());
  // Cases do not need to scan the buffer
  if (m_firstByteSeq >= seq) return;

  // Scan the buffer and discard packets
  uint32_t offset = seq - m_firstByteSeq.Get ();  // Number of bytes to remove
  uint32_t pktSize;
  NS_LOG_LOGIC ("Offset=" << offset);
  BufIterator i = m_data.begin ();
  while (i != m_data.end ())
    {
      if (offset > (*i)->GetSize ())
        { // This packet is behind the seqnum. Remove this packet from the buffer
          pktSize = (*i)->GetSize ();
          m_size -= pktSize;
          offset -= pktSize;
          m_firstByteSeq += pktSize;
          i = m_data.erase (i);
          NS_LOG_LOGIC ("Removed one packet of size " << pktSize << ", offset=" << offset);
        }
      else if (offset > 0)
        { // Part of the packet is behind the seqnum. Fragment
          pktSize = (*i)->GetSize () - offset;
          *i = (*i)->CreateFragment (offset, pktSize);
          m_size -= offset;
          m_firstByteSeq += offset;
          NS_LOG_LOGIC ("Fragmented one packet by size " << offset << ", new size=" << pktSize);
          break;
        }
    }
  // Catching the case of ACKing a FIN
  if (m_size == 0)
    {
      m_firstByteSeq = seq;
    }
  NS_LOG_LOGIC ("size=" << m_size << " headSeq=" << m_firstByteSeq << " maxBuffer=" << m_maxBuffer
                        <<" numPkts="<< m_data.size ());
  NS_ASSERT (m_firstByteSeq == seq);
}

} // namepsace ns3
