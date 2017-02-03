/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2015 Adrian Sai-wah Tam
 * Copyright (c) 2016 Natale Patriciello <natale.patriciello@gmail.com>
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
 * Original author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */

#include <algorithm>
#include <iostream>

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/tcp-option-ts.h"

#include "tcp-tx-buffer.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpTxBuffer");

TcpTxItem::TcpTxItem ()
  : m_packet (0),
    m_lost (false),
    m_retrans (false),
    m_lastSent (Time::Min ()),
    m_sacked (false)
{
}

TcpTxItem::TcpTxItem (const TcpTxItem &other)
  : m_packet (other.m_packet),
    m_lost (other.m_lost),
    m_retrans (other.m_retrans),
    m_lastSent (other.m_lastSent),
    m_sacked (other.m_sacked)
{
}

void
TcpTxItem::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this);
  bool comma = false;
  os << "pkt pointer: " << m_packet;

  if (m_lost)
    {
      os << "[lost]";
      comma = true;
    }
  if (m_retrans)
    {
      if (comma)
        {
          os << ",";
        }

      os << "[retrans]";
      comma = true;
    }
  if (m_sacked)
    {
      if (comma)
        {
          os << ",";
        }
      os << "[sacked]";
      comma = true;
    }
  if (comma)
    {
      os << ",";
    }
  os << "last sent: " << m_lastSent;
}

NS_OBJECT_ENSURE_REGISTERED (TcpTxBuffer);

TypeId
TcpTxBuffer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpTxBuffer")
    .SetParent<Object> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpTxBuffer> ()
    .AddTraceSource ("UnackSequence",
                     "First unacknowledged sequence number (SND.UNA)",
                     MakeTraceSourceAccessor (&TcpTxBuffer::m_firstByteSeq),
                     "ns3::SequenceNumber32TracedValueCallback")
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
  : m_maxBuffer (32768), m_size (0), m_sentSize (0), m_firstByteSeq (n)
{
}

TcpTxBuffer::~TcpTxBuffer (void)
{
  PacketList::iterator it;

  for (it = m_sentList.begin (); it != m_sentList.end (); ++it)
    {
      TcpTxItem *item = *it;
      m_sentSize -= item->m_packet->GetSize ();
      delete item;
    }

  for (it = m_appList.begin (); it != m_appList.end (); ++it)
    {
      TcpTxItem *item = *it;
      m_size -= item->m_packet->GetSize ();
      delete item;
    }
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

void
TcpTxBuffer::SetHeadSequence (const SequenceNumber32& seq)
{
  NS_LOG_FUNCTION (this << seq);
  m_firstByteSeq = seq;
}

bool
TcpTxBuffer::Add (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  NS_LOG_INFO ("Try to append " << p->GetSize () << " bytes to window starting at "
                                << m_firstByteSeq << ", availSize=" << Available ());
  if (p->GetSize () <= Available ())
    {
      if (p->GetSize () > 0)
        {
          TcpTxItem *item = new TcpTxItem ();
          item->m_packet = p;
          m_appList.insert (m_appList.end (), item);
          m_size += p->GetSize ();

          NS_LOG_INFO ("Updated size=" << m_size << ", lastSeq=" <<
                       m_firstByteSeq + SequenceNumber32 (m_size));
        }
      return true;
    }
  NS_LOG_WARN ("Rejected. Not enough room to buffer packet.");
  return false;
}

uint32_t
TcpTxBuffer::SizeFromSequence (const SequenceNumber32& seq) const
{
  NS_LOG_FUNCTION (this << seq);
  // Sequence of last byte in buffer
  SequenceNumber32 lastSeq = TailSequence ();

  if (lastSeq >= seq)
    {
      return lastSeq - seq;
    }

  NS_LOG_ERROR ("Requested a sequence beyond our space (" << seq << " > " << lastSeq <<
                "). Returning 0 for convenience.");
  return 0;
}

Ptr<Packet>
TcpTxBuffer::CopyFromSequence (uint32_t numBytes, const SequenceNumber32& seq)
{
  NS_LOG_FUNCTION (*this << numBytes << seq);

  if (m_firstByteSeq > seq)
    {
      NS_LOG_ERROR ("Requested a sequence number which is not in the buffer anymore");
      return Create<Packet> ();
    }

  // Real size to extract. Insure not beyond end of data
  uint32_t s = std::min (numBytes, SizeFromSequence (seq));

  if (s == 0)
    {
      return Create<Packet> ();
    }

  TcpTxItem *outItem = 0;

  if (m_firstByteSeq + m_sentSize >= seq + s)
    {
      // already sent this block completely
      outItem = GetTransmittedSegment (s, seq);
      NS_ASSERT (outItem != 0);
      outItem->m_retrans = true;

      NS_LOG_DEBUG ("Retransmitting [" << seq << ";" << seq + s << "|" << s <<
                    "] from " << *this);
    }
  else if (m_firstByteSeq + m_sentSize <= seq)
    {
      NS_ABORT_MSG_UNLESS (m_firstByteSeq + m_sentSize == seq,
                           "Requesting a piece of new data with an hole");

      // this is the first time we transmit this block
      outItem = GetNewSegment (s);
      NS_ASSERT (outItem != 0);
      NS_ASSERT (outItem->m_retrans == false);

      NS_LOG_DEBUG ("New segment [" << seq << ";" << seq + s << "|" << s <<
                    "] from " << *this);
    }
  else if (m_firstByteSeq + m_sentSize > seq && m_firstByteSeq + m_sentSize < seq + s)
    {
      // Partial: a part is retransmission, the remaining data is new

      // Take the new data and move it into sent list
      uint32_t amount = seq + s - m_firstByteSeq.Get () - m_sentSize;
      NS_LOG_DEBUG ("Moving segment [" << m_firstByteSeq + m_sentSize << ";" <<
                    m_firstByteSeq + m_sentSize + amount <<"|" << amount <<
                    "] from " << *this);

      outItem = GetNewSegment (amount);
      NS_ASSERT (outItem != 0);

      // Now get outItem from the sent list (there will be a merge)
      return CopyFromSequence (numBytes, seq);
    }

  outItem->m_lost = false;
  outItem->m_lastSent = Simulator::Now ();
  Ptr<Packet> toRet = outItem->m_packet->Copy ();

  NS_ASSERT (toRet->GetSize () == s);

  return toRet;
}

TcpTxItem*
TcpTxBuffer::GetNewSegment (uint32_t numBytes)
{
  NS_LOG_FUNCTION (this << numBytes);

  SequenceNumber32 startOfAppList = m_firstByteSeq + m_sentSize;

  TcpTxItem *item = GetPacketFromList (m_appList, startOfAppList,
                                       numBytes, startOfAppList);

  // Move item from AppList to SentList (should be the first, not too complex)
  PacketList::iterator it = std::find (m_appList.begin (), m_appList.end (), item);
  NS_ASSERT (it != m_appList.end ());

  m_appList.erase (it);
  m_sentList.insert (m_sentList.end (), item);
  m_sentSize += item->m_packet->GetSize ();

  return item;
}

TcpTxItem*
TcpTxBuffer::GetTransmittedSegment (uint32_t numBytes, const SequenceNumber32 &seq)
{
  NS_LOG_FUNCTION (this << numBytes << seq);
  NS_ASSERT (seq >= m_firstByteSeq);
  NS_ASSERT (numBytes <= m_sentSize);

  return GetPacketFromList (m_sentList, m_firstByteSeq, numBytes, seq);
}

void
TcpTxBuffer::SplitItems (TcpTxItem &t1, TcpTxItem &t2, uint32_t size) const
{
  NS_LOG_FUNCTION (this << size);

  t1.m_packet = t2.m_packet->CreateFragment (0, size);
  t2.m_packet->RemoveAtStart (size);

  t1.m_sacked = t2.m_sacked;
  t1.m_lastSent = t2.m_lastSent;
  t1.m_retrans = t2.m_retrans;
  t1.m_lost = t2.m_lost;
}

TcpTxItem*
TcpTxBuffer::GetPacketFromList (PacketList &list, const SequenceNumber32 &listStartFrom,
                                uint32_t numBytes, const SequenceNumber32 &seq) const
{
  NS_LOG_FUNCTION (this << numBytes << seq);

  /*
   * Our possibilites are sketched out in the following:
   *
   *                    |------|     |----|     |----|
   * GetList (m_data) = |      | --> |    | --> |    |
   *                    |------|     |----|     |----|
   *
   *                    ^ ^ ^  ^
   *                    | | |  |         (1)
   *                  seq | |  numBytes
   *                      | |
   *                      | |
   *                    seq numBytes     (2)
   *
   * (1) seq and numBytes are the boundary of some packet
   * (2) seq and numBytes are not the boundary of some packet
   *
   * We can have mixed case (e.g. seq over the boundary while numBytes not).
   *
   * If we discover that we are in (2) or in a mixed case, we split
   * packets accordingly to the requested bounds and re-run the function.
   *
   * In (1), things are pretty easy, it's just a matter of walking the list and
   * defragment packets, if needed (e.g. seq is the beginning of the first packet
   * while maxBytes is the end of some packet next in the list).
   */

  Ptr<Packet> currentPacket = 0;
  TcpTxItem *currentItem = 0;
  TcpTxItem *outItem = 0;
  PacketList::iterator it = list.begin ();
  SequenceNumber32 beginOfCurrentPacket = listStartFrom;

  while (it != list.end ())
    {
      currentItem = *it;
      currentPacket = currentItem->m_packet;

      // The objective of this snippet is to find (or to create) the packet
      // that begin with the sequence seq

      if (seq < beginOfCurrentPacket + currentPacket->GetSize ())
        {
          // seq is inside the current packet
          if (seq == beginOfCurrentPacket)
            {
              // seq is the beginning of the current packet. Hurray!
              outItem = currentItem;
              NS_LOG_INFO ("Current packet starts at seq " << seq <<
                           " ends at " << seq + currentPacket->GetSize ());
            }
          else if (seq > beginOfCurrentPacket)
            {
              // seq is inside the current packet but seq is not the beginning,
              // it's somewhere in the middle. Just fragment the beginning and
              // start again.
              NS_LOG_INFO ("we are at " << beginOfCurrentPacket <<
                           " searching for " << seq <<
                           " and now we recurse because packet ends at "
                                        << beginOfCurrentPacket + currentPacket->GetSize ());
              TcpTxItem *firstPart = new TcpTxItem ();
              SplitItems (*firstPart, *currentItem, seq - beginOfCurrentPacket);

              // insert firstPart before currentItem
              list.insert (it, firstPart);

              return GetPacketFromList (list, listStartFrom, numBytes, seq);
            }
          else
            {
              NS_FATAL_ERROR ("seq < beginOfCurrentPacket: our data is before");
            }
        }
      else
        {
          // Walk the list, the current packet does not contain seq
          beginOfCurrentPacket += currentPacket->GetSize ();
          it++;
          continue;
        }

      NS_ASSERT (outItem != 0);

      // The objective of this snippet is to find (or to create) the packet
      // that ends after numBytes bytes. We are sure that outPacket starts
      // at seq.

      if (seq + numBytes <= beginOfCurrentPacket + currentPacket->GetSize ())
        {
          // the end boundary is inside the current packet
          if (numBytes == currentPacket->GetSize ())
            {
              // the end boundary is exactly the end of the current packet. Hurray!
              if (currentItem->m_packet == outItem->m_packet)
                {
                  // A perfect match!
                  return outItem;
                }
              else
                {
                  // the end is exactly the end of current packet, but
                  // current > outPacket in the list. Merge current with the
                  // previous, and recurse.
                  NS_ASSERT (it != list.begin ());
                  TcpTxItem *previous = *(--it);

                  list.erase (it);

                  MergeItems (*previous, *currentItem);
                  delete currentItem;

                  return GetPacketFromList (list, listStartFrom, numBytes, seq);
                }
            }
          else if (numBytes < currentPacket->GetSize ())
            {
              // the end is inside the current packet, but it isn't exactly
              // the packet end. Just fragment, fix the list, and return.
              TcpTxItem *firstPart = new TcpTxItem ();
              SplitItems (*firstPart, *currentItem, numBytes);

              // insert firstPart before currentItem
              list.insert (it, firstPart);

              return firstPart;
            }
        }
      else
        {
          // The end isn't inside current packet, but there is an exception for
          // the merge and recurse strategy...
          if (++it == list.end ())
            {
              // ...current is the last packet we sent. We have not more data;
              // Go for this one.
              NS_LOG_WARN ("Cannot reach the end, but this case is covered "
                           "with conditional statements inside CopyFromSequence."
                           "Something has gone wrong, report a bug");
              return outItem;
            }

          // The current packet does not contain the requested end. Merge current
          // with the packet that follows, and recurse
          TcpTxItem *next = (*it); // Please remember we have incremented it
                                   // in the previous if

          MergeItems (*currentItem, *next);
          list.erase (it);

          delete next;

          return GetPacketFromList (list, listStartFrom, numBytes, seq);
        }
    }

  NS_FATAL_ERROR ("This point is not reachable");
}

void
TcpTxBuffer::MergeItems (TcpTxItem &t1, TcpTxItem &t2) const
{
  NS_LOG_FUNCTION (this);
  if (t1.m_sacked == true && t2.m_sacked == true)
    {
      t1.m_sacked = true;
    }
  else
    {
      t1.m_sacked = false;
    }

  if (t2.m_retrans == true && t1.m_retrans == false)
    {
      t1.m_retrans = true;
    }
  if (t1.m_lastSent < t2.m_lastSent)
    {
      t1.m_lastSent = t2.m_lastSent;
    }
  if (t2.m_lost)
    {
      t1.m_lost = true;
    }

  t1.m_packet->AddAtEnd (t2.m_packet);
}

void
TcpTxBuffer::DiscardUpTo (const SequenceNumber32& seq)
{
  NS_LOG_FUNCTION (this << seq);

  // Cases do not need to scan the buffer
  if (m_firstByteSeq >= seq)
    {
      NS_LOG_DEBUG ("Seq " << seq << " already discarded.");
      return;
    }

  // Scan the buffer and discard packets
  uint32_t offset = seq - m_firstByteSeq.Get ();  // Number of bytes to remove
  uint32_t pktSize;
  PacketList::iterator i = m_sentList.begin ();
  while (m_size > 0 && offset > 0)
    {
      if (i == m_sentList.end ())
        {
          Ptr<Packet> p = CopyFromSequence (offset, m_firstByteSeq);
          NS_ASSERT (p != 0);
          i = m_sentList.begin ();
          NS_ASSERT (i != m_sentList.end ());
        }
      TcpTxItem *item = *i;
      Ptr<Packet> p = item->m_packet;
      pktSize = p->GetSize ();

      if (offset >= pktSize)
        { // This packet is behind the seqnum. Remove this packet from the buffer
          m_size -= pktSize;
          m_sentSize -= pktSize;
          offset -= pktSize;
          m_firstByteSeq += pktSize;
          i = m_sentList.erase (i);
          delete item;
          NS_LOG_INFO ("While removing up to " << seq <<
                       ".Removed one packet of size " << pktSize <<
                       " starting from " << m_firstByteSeq - pktSize <<
                       ". Remaining data " << m_size);
        }
      else if (offset > 0)
        { // Part of the packet is behind the seqnum. Fragment
          pktSize -= offset;
          // PacketTags are preserved when fragmenting
          item->m_packet = item->m_packet->CreateFragment (offset, pktSize);
          m_size -= offset;
          m_sentSize -= offset;
          m_firstByteSeq += offset;
          NS_LOG_INFO ("Fragmented one packet by size " << offset <<
                       ", new size=" << pktSize);
          break;
        }
    }
  // Catching the case of ACKing a FIN
  if (m_size == 0)
    {
      m_firstByteSeq = seq;
    }

  if (!m_sentList.empty ())
    {
      TcpTxItem *head = m_sentList.front ();
      if (head->m_sacked)
        {
          // It is not possible to have the UNA sacked; otherwise, it would
          // have been ACKed. This is, most likely, our wrong guessing
          // when crafting the SACK option for a non-SACK receiver.
          head->m_sacked = false;
        }
    }

  NS_LOG_DEBUG ("Discarded up to " << seq);
  NS_LOG_LOGIC ("Buffer status after discarding data " << *this);
  NS_ASSERT (m_firstByteSeq >= seq);
}

bool
TcpTxBuffer::Update (const TcpOptionSack::SackList &list)
{
  NS_LOG_FUNCTION (this);

  bool modified = false;
  TcpOptionSack::SackList::const_iterator option_it;
  NS_LOG_INFO ("Updating scoreboard, got " << list.size () << " blocks to analyze");
  for (option_it = list.begin (); option_it != list.end (); ++option_it)
    {
      Ptr<Packet> current;
      TcpTxItem *item;
      const TcpOptionSack::SackBlock b = (*option_it);

      PacketList::iterator item_it = m_sentList.begin ();
      SequenceNumber32 beginOfCurrentPacket = m_firstByteSeq;

      while (item_it != m_sentList.end ())
        {
          item = *item_it;
          current = item->m_packet;

          // Check the boundary of this packet ... only mark as sacked if
          // it is precisely mapped over the option
          if (beginOfCurrentPacket >= b.first
              && beginOfCurrentPacket + current->GetSize () <= b.second)
            {
              if (item->m_sacked)
                {
                  NS_LOG_INFO ("Received block [" << b.first << ";" << b.second <<
                               ", checking sentList for block " << beginOfCurrentPacket <<
                               ";" << beginOfCurrentPacket + current->GetSize () <<
                               "], found in the sackboard already sacked");
                }
              else
                {
                  item->m_sacked = true;
                  NS_LOG_INFO ("Received block [" << b.first << ";" << b.second <<
                               ", checking sentList for block " << beginOfCurrentPacket <<
                               ";" << beginOfCurrentPacket + current->GetSize () <<
                               "], found in the sackboard, sacking");
                }
              modified = true;
            }
          else if (beginOfCurrentPacket + current->GetSize () > b.second)
            {
              // we missed the block. It's useless to iterate again; Say "ciao"
              // to the loop for optimization purposes
              NS_LOG_INFO ("Received block [" << b.first << ";" << b.second <<
                           ", checking sentList for block " << beginOfCurrentPacket <<
                           ";" << beginOfCurrentPacket + current->GetSize () <<
                           "], not found, breaking loop");
              break;
            }

          beginOfCurrentPacket += current->GetSize ();
          ++item_it;
        }
    }

  return modified;
}

bool
TcpTxBuffer::IsLost (const SequenceNumber32 &seq, const PacketList::const_iterator &segment,
                     uint32_t dupThresh, uint32_t segmentSize) const
{
  NS_LOG_FUNCTION (this << seq << dupThresh << segmentSize);
  uint32_t count = 0;
  uint32_t bytes = 0;
  PacketList::const_iterator it;
  TcpTxItem *item;
  Ptr<const Packet> current;
  SequenceNumber32 beginOfCurrentPacket = seq;

  // From RFC 6675:
  // > The routine returns true when either dupThresh discontiguous SACKed
  // > sequences have arrived above 'seq' or more than (dupThresh - 1) * SMSS bytes
  // > with sequence numbers greater than 'SeqNum' have been SACKed.  Otherwise, the
  // > routine returns false.
  for (it = segment; it != m_sentList.end (); ++it)
    {
      item = *it;
      current = item->m_packet;

      if (item->m_lost)
        {
          return true;
        }

      if (item->m_sacked)
        {
          NS_LOG_DEBUG ("Segment found to be SACKed, increasing counter and bytes by " <<
                        current->GetSize ());
          ++count;
          bytes += current->GetSize ();
          if ((count >= dupThresh) || (bytes > (dupThresh-1) * segmentSize))
            {
              return true;
            }
        }

      beginOfCurrentPacket += current->GetSize ();
    }

  return false;
}

bool
TcpTxBuffer::IsLost (const SequenceNumber32 &seq, uint32_t dupThresh,
                     uint32_t segmentSize) const
{
  NS_LOG_FUNCTION (this << seq << dupThresh);

  SequenceNumber32 beginOfCurrentPacket = m_firstByteSeq;
  PacketList::const_iterator it;

  // This O(n) method is called only once, and outside this class.
  // It should not harm the performance
  for (it = m_sentList.begin (); it != m_sentList.end (); ++it)
    {
      // Search for the right iterator before calling IsLost()
      if (beginOfCurrentPacket > seq)
        {
          return IsLost (beginOfCurrentPacket, it, dupThresh, segmentSize);
        }

      beginOfCurrentPacket += (*it)->m_packet->GetSize ();
    }

  return false;
}

std::ostream &
operator<< (std::ostream & os, TcpTxBuffer const & tcpTxBuf)
{
  TcpTxBuffer::PacketList::const_iterator it;
  std::stringstream ss;
  SequenceNumber32 beginOfCurrentPacket = tcpTxBuf.m_firstByteSeq;
  uint32_t sentSize = 0, appSize = 0;

  Ptr<Packet> p;
  for (it = tcpTxBuf.m_sentList.begin (); it != tcpTxBuf.m_sentList.end (); ++it)
    {
      p = (*it)->m_packet;
      ss << "[" << beginOfCurrentPacket << ";"
         << beginOfCurrentPacket + p->GetSize () << "|" << p->GetSize () << "|";
      (*it)->Print (ss);
      ss << "]";
      sentSize += p->GetSize ();
      beginOfCurrentPacket += p->GetSize ();
    }

  for (it = tcpTxBuf.m_appList.begin (); it != tcpTxBuf.m_appList.end (); ++it)
    {
      appSize += (*it)->m_packet->GetSize ();
    }

  os << "Sent list: " << ss.str () << ", size = " << tcpTxBuf.m_sentList.size () <<
    " Total size: " << tcpTxBuf.m_size <<
    " m_firstByteSeq = " << tcpTxBuf.m_firstByteSeq <<
    " m_sentSize = " << tcpTxBuf.m_sentSize;

  NS_ASSERT (sentSize == tcpTxBuf.m_sentSize);
  NS_ASSERT (tcpTxBuf.m_size - tcpTxBuf.m_sentSize == appSize);
  return os;
}

} // namepsace ns3
