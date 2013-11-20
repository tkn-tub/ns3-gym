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

#ifndef TCP_TX_BUFFER_H
#define TCP_TX_BUFFER_H

#include <list>
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object.h"
#include "ns3/sequence-number.h"
#include "ns3/ptr.h"

namespace ns3 {
class Packet;

/**
 * \ingroup tcp
 *
 * \brief class for keeping the data sent by the application to the TCP socket, i.e.
 *        the sending buffer.
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
   * Returns the first byte's sequence number
   * \returns the first byte's sequence number
   */
  SequenceNumber32 HeadSequence (void) const;

  /**
   * Returns the last byte's sequence number + 1
   * \returns the last byte's sequence number + 1
   */
  SequenceNumber32 TailSequence (void) const;

  /**
   * Returns total number of bytes in this Tx buffer
   * \returns total number of bytes in this Tx buffer
   */
  uint32_t Size (void) const;

  /**
   * Returns the Tx window size
   * \returns the Tx window size (in bytes)
   */
  uint32_t MaxBufferSize (void) const;

  /**
   * Set the Tx window size
   * \param n Tx window size (in bytes)
   */
  void SetMaxBufferSize (uint32_t n);

  /**
   * Returns the available capacity in this Tx window
   * \returns available capacity in this Tx window
   */
  uint32_t Available (void) const;

  /**
   * Append a data packet to the end of the buffer
   *
   * \param p The packet to be appended to the Tx buffer
   * \return Boolean to indicate success
   */
  bool Add (Ptr<Packet> p);

  /**
   * Returns the number of bytes from the buffer in the range [seq, tailSequence)
   * \param seq initial sequence number
   * \returns the number of bytes from the buffer in the range
   */
  uint32_t SizeFromSequence (const SequenceNumber32& seq) const;

  /**
   * Copy data of size numBytes into a packet, data from the range [seq, seq+numBytes)
   * \param numBytes number of bytes to copy
   * \param seq start sequence number to extract
   * \returns a packet
   */
  Ptr<Packet> CopyFromSequence (uint32_t numBytes, const SequenceNumber32& seq);

  /**
   * Set the m_firstByteSeq to seq. Supposed to be called only when the
   * connection is just set up and we did not send any data out yet.
   * \param seq The sequence number of the head byte
   */
  void SetHeadSequence (const SequenceNumber32& seq);

  /**
   * Discard data up to but not including this sequence number.
   *
   * \param seq The sequence number of the head byte
   */
  void DiscardUpTo (const SequenceNumber32& seq);

private:
  /// container for data stored in the buffer
  typedef std::list<Ptr<Packet> >::iterator BufIterator;

  TracedValue<SequenceNumber32> m_firstByteSeq; //!< Sequence number of the first byte in data (SND.UNA)
  uint32_t m_size;                              //!< Number of data bytes
  uint32_t m_maxBuffer;                         //!< Max number of data bytes in buffer (SND.WND)
  std::list<Ptr<Packet> > m_data;               //!< Corresponding data (may be null)
};

} // namepsace ns3

#endif /* TCP_TX_BUFFER_H */
