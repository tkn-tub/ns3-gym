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

#ifndef TCP_TX_BUFFER_H
#define TCP_TX_BUFFER_H

#include "ns3/object.h"
#include "ns3/traced-value.h"
#include "ns3/sequence-number.h"
#include "ns3/nstime.h"
#include "ns3/tcp-option-sack.h"

namespace ns3 {
class Packet;

/**
 * \ingroup tcp
 *
 * \brief Item that encloses the application packet and some flags for it
 */
class TcpTxItem
{
public:
  /**
   * \brief Constructor
   */
  TcpTxItem ();

  /**
   * \brief Copy-constructor
   * \param other TcpTxTag to copy values from
   */
  TcpTxItem (const TcpTxItem &other);

  /**
   * \brief Print the time
   * \param os ostream
   */
  void Print (std::ostream &os) const;

  Ptr<Packet> m_packet; //!< Application packet
  bool m_lost;          //!< Indicates if the segment has been lost (RTO)
  bool m_retrans;       //!< Indicates if the segment is retransmitted
  Time m_lastSent;      //!< Timestamp of the time at which the segment has
                        //   been sent last time
  bool m_sacked;        //!< Indicates if the segment has been SACKed
};

/**
 * \ingroup tcp
 *
 * \brief Tcp sender buffer
 *
 * The class keeps track of all data that the application wishes to transmit
 * to the other end. When the data is acknowledged, it is removed from the buffer.
 * The buffer has a maximum size, and data is not saved if the amount exceeds
 * the limit. Packets can be added to the class through the method Add().
 * An important thing to remember is that all the data managed is strictly
 * sequential. It can be divided in blocks, but all the data follow a strict
 * ordering. That ordering is managed through SequenceNumber.
 *
 * In other words, this buffer contains numbered bytes (e.g 1,2,3), and the class
 * is allowed to return only ordered (using "<" as operator) subsets (e.g. 1,2
 * or 2,3 or 1,2,3).
 *
 * The data structure underlying this is composed by two distinct packet lists.
 *
 * The first (SentList) is initially empty, and it contains the packets returned
 * by the method CopyFromSequence.
 *
 * The second (AppList) is initially empty, and it contains the packets coming
 * from the applications, but that are not transmitted yet as segments.
 *
 * To discover how the chunk are managed and retrieved from these lists, check
 * CopyFromSequence documentation.
 *
 * The head of the data is represented by m_firstByteSeq, and it is returned by
 * HeadSequence(). The last byte is returned by TailSequence().
 * In this class we store also the size (in bytes) of the packets inside the
 * SentList in the variable m_sentSize.
 *
 * \see Size
 * \see SizeFromSequence
 * \see CopyFromSequence
 */
class TcpTxBuffer : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief Constructor
   * \param n initial Sequence number to be transmitted
   */
  TcpTxBuffer (uint32_t n = 0);
  virtual ~TcpTxBuffer (void);

  // Accessors

  /**
   * \brief Get the sequence number of the buffer head
   * \returns the first byte's sequence number
   */
  SequenceNumber32 HeadSequence (void) const;

  /**
   * \brief Get the sequence number of the buffer tail (plus one)
   * \returns the last byte's sequence number + 1
   */
  SequenceNumber32 TailSequence (void) const;

  /**
   * \brief Returns total number of bytes in this buffer
   * \returns total number of bytes in this Tx buffer
   */
  uint32_t Size (void) const;

  /**
   * \brief Get the maximum buffer size
   * \returns the Tx window size (in bytes)
   */
  uint32_t MaxBufferSize (void) const;

  /**
   * \brief Set the maximum buffer size
   * \param n Tx window size (in bytes)
   */
  void SetMaxBufferSize (uint32_t n);

  /**
   * \brief Returns the available capacity of this buffer
   * \returns available capacity in this Tx window
   */
  uint32_t Available (void) const;

  /**
   * \brief Append a data packet to the end of the buffer
   *
   * \param p The packet to be appended to the Tx buffer
   * \return Boolean to indicate success
   */
  bool Add (Ptr<Packet> p);

  /**
   * \brief Returns the number of bytes from the buffer in the range [seq, tailSequence)
   *
   * \param seq initial sequence number
   * \returns the number of bytes from the buffer in the range
   */
  uint32_t SizeFromSequence (const SequenceNumber32& seq) const;

  /**
   * \brief Copy data from the range [seq, seq+numBytes) into a packet
   *
   * In the following, we refer to the block [seq, seq+numBytes) simply as "block".
   * We check the boundary of the block, and divide the possibilities in three
   * cases:
   *
   * - the block have already been transmitted (managed in GetTransmittedSegment)
   * - the block have not been transmitted yet (managed in GetNewSegment)
   *
   * The last case is when the block is partially transmitted and partially
   * not transmitted. We trick this case by requesting the portion not transmitted
   * from GetNewSegment, and then calling GetTransmittedSegment with the full
   * block range.
   *
   * \param numBytes number of bytes to copy
   * \param seq start sequence number to extract
   * \returns a packet
   */
  Ptr<Packet> CopyFromSequence (uint32_t numBytes, const SequenceNumber32& seq);

  /**
   * \brief Set the head sequence of the buffer
   *
   * Set the head (m_firstByteSeq) to seq. Supposed to be called only when the
   * connection is just set up and we did not send any data out yet.
   * \param seq The sequence number of the head byte
   */
  void SetHeadSequence (const SequenceNumber32& seq);

  /**
   * \brief Discard data up to but not including this sequence number.
   *
   * \param seq The first sequence number to maintain after discarding all the
   * previous sequences.
   */
  void DiscardUpTo (const SequenceNumber32& seq);

  /**
   * \brief Update the scoreboard
   * \param list list of SACKed blocks
   */
  bool Update (const TcpOptionSack::SackList &list);

  /**
   * \brief Check if a segment is lost per RFC 6675
   * \param seq sequence to check
   * \param dupThresh dupAck threshold
   * \param segmentSize segment size
   * \return true if the sequence is supposed to be lost, false otherwise
   */
  bool IsLost (const SequenceNumber32 &seq, uint32_t dupThresh, uint32_t segmentSize) const;

  /**
   * \brief Get the next sequence number to transmit, according to RFC 6675
   *
   * \param seq Next sequence number to transmit, based on the scoreboard information
   * \param dupThresh dupAck threshold
   * \param segmentSize segment size
   * \param isRecovery true if the socket congestion state is in recovery mode
   * \return true is seq is updated, false otherwise
   */
  bool NextSeg (SequenceNumber32 *seq, uint32_t dupThresh, uint32_t segmentSize,
                bool isRecovery) const;

  /**
   * \brief Return total bytes in flight
   *
   * The routine follows the "SetPipe" function in RFC 6675
   *
   * \returns total bytes in flight
   */
  uint32_t BytesInFlight (uint32_t dupThresh, uint32_t segmentSize) const;

  /**
   * \brief Reset the Scoreboard from all SACK informations
   */
  void ResetScoreboard ();

  /**
   * \brief Check if the head is retransmitted
   *
   * \return true if the head is retransmitted, false in all other cases
   * (including no segment sent)
   */
  bool IsHeadRetransmitted () const;

  /**
   * \brief Reset the sent list
   *
   * Move all the packets (except the HEAD) from the sent list to the appList.
   * The head is then marked as un-sacked, un-retransmitted, and lost.
   */
  void ResetSentList ();

  /**
   * \brief Take the last segment sent and put it back into the un-sent list
   * (at the beginning)
   */
  void ResetLastSegmentSent ();

private:
  friend std::ostream & operator<< (std::ostream & os, TcpTxBuffer const & tcpTxBuf);

  typedef std::list<TcpTxItem*> PacketList; //!< container for data stored in the buffer

  /**
   * \brief Check if a segment is lost per RFC 6675
   * \param seq sequence to check
   * \param segment Iterator pointing at seq
   * \param dupThresh dupAck threshold
   * \param segmentSize segment size
   * \return true if the sequence is supposed to be lost, false otherwise
   */
  bool IsLost (const SequenceNumber32 &seq, const PacketList::const_iterator &segment, uint32_t dupThresh,
               uint32_t segmentSize) const;

  /**
   * \brief Get a block of data not transmitted yet and move it into SentList
   *
   * If the block is not yet transmitted, hopefully, seq is exactly the sequence
   * number of the first byte of the first packet inside AppList. We extract
   * the block from AppList and move it into the SentList, before returning the
   * block itself. We manage possible fragmentation (or merges) inside AppList
   * through GetPacketFromList.
   *
   * \see GetPacketFromList
   * \param numBytes number of bytes to copy
   *
   * \return the item that contains the right packet
   */
  TcpTxItem* GetNewSegment (uint32_t numBytes);

  /**
   * \brief Get a block of data previously transmitted
   *
   * This is clearly a retransmission, and if everything is going well,
   * the block requested is matching perfectly with another one requested
   * in the past. If not, fragmentation or merge are required. We manage
   * both inside GetPacketFromList.
   *
   * \see GetPacketFromList
   *
   * \param numBytes number of bytes to copy
   * \param seq sequence requested
   * \return the item that contains the right packet
   */
  TcpTxItem* GetTransmittedSegment (uint32_t numBytes, const SequenceNumber32 &seq);

  /**
   * \brief Get a block (which is returned as Packet) from a list
   *
   * This function extract a block [requestedSeq,numBytes) from the list, which
   * starts at startingSeq.
   *
   * The cases we need to manage are two, and they are depicted in the following
   * image:
   *
   *\verbatim
                       |------|     |----|     |----|
                list = |      | --> |    | --> |    |
                       |------|     |----|     |----|

                       ^      ^
                       | ^ ^  |         (1)
                     seq | |  seq + numBytes
                         | |
                         | |
                      seq   seq + numBytes     (2)
   \endverbatim
   *
   * The case 1 is easy to manage: the requested block is exactly a packet
   * already stored. If one value (seq or seq + numBytes) does not align
   * to a packet boundary, or when both values does not align (case 2), it is
   * a bit more complex.
   *
   * Basically, we have two possible operations:
   *
   *  - fragment : split an existing packet in two
   *  - merge    : merge two existing packets in one
   *
   * and we reduce case (2) to case (1) through sequentially applying fragment
   * or merge. For instance:
   *
   *\verbatim
      |------|
      |      |
      |------|

      ^ ^  ^ ^
      | |  | |
  start |  | |
        |  | end
       seq |
           seq + numBytes
   \endverbatim
   *
   * To reduce to case (1), we need to perform two fragment operations:
   *
   * - fragment (start, seq)
   * - fragment (seq + numBytes, end)
   *
   * After these operations, the requested block is exactly the resulting packet.
   * Merge operation is required when the requested block span over two (or more)
   * existing packets.
   *
   * While this could be extremely slow in the worst possible scenario (one big
   * packet which is split in small packets for transmission, and merged for
   * re-transmission) that scenario is unlikely during a TCP transmission (since
   * MSS can change, but it is stable, and retransmissions do not happen for
   * each segment).
   *
   * \param list List to extract block from
   * \param startingSeq Starting sequence of the list
   * \param numBytes Bytes to extract, starting from requestedSeq
   * \param requestedSeq Requested sequence
   * \return the item that contains the right packet
   */
  TcpTxItem* GetPacketFromList (PacketList &list, const SequenceNumber32 &startingSeq,
                                uint32_t numBytes, const SequenceNumber32 &requestedSeq) const;

  /**
   * \brief Merge two TcpTxItem
   *
   * Merge t2 in t1. It consists in copying the lastSent field if t2 is more
   * recent than t1. Retransmitted field is copied only if it set in t2 but not
   * in t1. Sacked is copied only if it is true in both items.
   *
   * \param t1 first item
   * \param t2 second item
   */
  void MergeItems (TcpTxItem &t1, TcpTxItem &t2) const;

  /**
   * \brief Split one TcpTxItem
   *
   * Move "size" bytes from t2 into t1, copying all the fields.
   *
   * \param t1 first item
   * \param t2 second item
   * \param size Size to split
   */
  void SplitItems (TcpTxItem &t1, TcpTxItem &t2, uint32_t size) const;

  PacketList m_appList;  //!< Buffer for application data
  PacketList m_sentList; //!< Buffer for sent (but not acked) data
  uint32_t m_maxBuffer;  //!< Max number of data bytes in buffer (SND.WND)
  uint32_t m_size;       //!< Size of all data in this buffer
  uint32_t m_sentSize;   //!< Size of sent (and not discarded) segments

  TracedValue<SequenceNumber32> m_firstByteSeq; //!< Sequence number of the first byte in data (SND.UNA)

};

std::ostream & operator<< (std::ostream & os, TcpTxBuffer const & tcpTxBuf);

} // namepsace ns3

#endif /* TCP_TX_BUFFER_H */
