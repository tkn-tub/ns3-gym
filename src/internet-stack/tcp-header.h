/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 */

#ifndef TCP_HEADER_H
#define TCP_HEADER_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/buffer.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/sequence-number.h"

namespace ns3 {

/**
 * \brief Header for the Transmission Control Protocol
 *
 * This class has fields corresponding to those in a network TCP header
 * (port numbers, sequence and acknowledgement numbers, flags, etc) as well
 * as methods for serialization to and deserialization from a byte buffer.
 */

class TcpHeader : public Header 
{
public:
  TcpHeader ();
  virtual ~TcpHeader ();

  /**
   * \brief Enable checksum calculation for TCP (XXX currently has no effect)
   */
  void EnableChecksums (void);
//Setters
  /**
   * \param port The source port for this TcpHeader
   */
  void SetSourcePort (uint16_t port);
  /**
   * \param port the destination port for this TcpHeader
   */
  void SetDestinationPort (uint16_t port);
  /**
   * \param sequenceNumber the sequence number for this TcpHeader
   */
  void SetSequenceNumber (SequenceNumber sequenceNumber);
  /**
   * \param ackNumber the ACK number for this TcpHeader
   */
  void SetAckNumber (SequenceNumber ackNumber);
  /**
   * \param length the length of this TcpHeader
   */
  void SetLength (uint8_t length);
  /**
   * \param flags the flags for this TcpHeader
   */
  void SetFlags (uint8_t flags);
  /**
   * \param windowSize the window size for this TcpHeader
   */
  void SetWindowSize (uint16_t windowSize);
  /**
   * \param checksum the checksum for this TcpHeader
   */
  void SetChecksum (uint16_t checksum);
  /**
   * \param urgentPointer the urgent pointer for this TcpHeader
   */
  void SetUrgentPointer (uint16_t urgentPointer);


//Getters
  /**
   * \return The source port for this TcpHeader
   */
  uint16_t GetSourcePort () const;
  /**
   * \return the destination port for this TcpHeader
   */
  uint16_t GetDestinationPort () const;
  /**
   * \return the sequence number for this TcpHeader
   */
  SequenceNumber GetSequenceNumber () const;
  /**
   * \return the ACK number for this TcpHeader
   */
  SequenceNumber GetAckNumber () const;
  /**
   * \return the length of this TcpHeader
   */
  uint8_t  GetLength () const;
  /**
   * \return the flags for this TcpHeader
   */
  uint8_t  GetFlags () const;
  /**
   * \return the window size for this TcpHeader
   */
  uint16_t GetWindowSize () const;
  /**
   * \return the checksum for this TcpHeader
   */
  uint16_t GetChecksum () const;
  /**
   * \return the urgent pointer for this TcpHeader
   */
  uint16_t GetUrgentPointer () const;

  /**
   * \param source the ip source to use in the underlying
   *        ip packet.
   * \param destination the ip destination to use in the
   *        underlying ip packet.
   * \param protocol the protocol number to use in the underlying
   *        ip packet.
   *
   * If you want to use tcp checksums, you should call this
   * method prior to adding the header to a packet.
   */
  void InitializeChecksum (Ipv4Address source, 
                           Ipv4Address destination,
                           uint8_t protocol);

  typedef enum { NONE = 0, FIN = 1, SYN = 2, RST = 4, PSH = 8, ACK = 16, 
    URG = 32} Flags_t;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * \param size The payload size in bytes
   */
  void SetPayloadSize (uint16_t size);

  /**
   * \brief Is the TCP checksum correct ?
   * \returns true if the checksum is correct, false otherwise.
   */
  bool IsChecksumOk (void) const;

private:
  uint16_t m_sourcePort;
  uint16_t m_destinationPort;
  uint32_t m_sequenceNumber;
  uint32_t m_ackNumber;
  uint8_t m_length; // really a uint4_t
  uint8_t m_flags;      // really a uint6_t
  uint16_t m_windowSize;
  uint16_t m_urgentPointer;
  uint16_t m_payloadSize;
  uint16_t m_initialChecksum;
  uint16_t m_checksum;

  bool m_calcChecksum;
  bool m_goodChecksum;

};

}; // namespace ns3

#endif /* TCP_HEADER */
