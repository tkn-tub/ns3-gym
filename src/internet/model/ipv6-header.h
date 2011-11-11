/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007-2008 Louis Pasteur University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#ifndef IPV6_HEADER_H
#define IPV6_HEADER_H

#include "ns3/header.h"
#include "ns3/ipv6-address.h"

namespace ns3 {

/**
 * \class Ipv6Header
 * \brief Packet header for IPv6
 */
class Ipv6Header : public Header
{
public:
  /**
   * \enum NextHeader_e
   * \brief IPv6 next-header value
   */
  enum NextHeader_e
  {
    IPV6_EXT_HOP_BY_HOP = 0,
    IPV6_IPV4 = 4,
    IPV6_TCP = 6,
    IPV6_UDP = 17,
    IPV6_IPV6 = 41,
    IPV6_EXT_ROUTING = 43,
    IPV6_EXT_FRAGMENTATION = 44,
    IPV6_EXT_CONFIDENTIALITY = 50,
    IPV6_EXT_AUTHENTIFICATION = 51,
    IPV6_ICMPV6 = 58,
    IPV6_EXT_END = 59,
    IPV6_EXT_DESTINATION = 60,
    IPV6_SCTP = 135,
    IPV6_EXT_MOBILITY = 135,
    IPV6_UDP_LITE = 136,
  };

  /**
   * \brief Get the type identifier.
   * \return type identifier
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * \brief Constructor.
   */
  Ipv6Header (void);

  /**
   * \brief Set the "Traffic class" field.
   * \param traffic the 8-bit value
   */
  void SetTrafficClass (uint8_t traffic);

  /**
   * \brief Get the "Traffic class" field.
   * \return the traffic value
   */
  uint8_t GetTrafficClass (void) const;

  /**
   * \brief Set the "Flow label" field.
   * \param flow the 20-bit value
   */
  void SetFlowLabel (uint32_t flow);

  /**
   * \brief Get the "Flow label" field.
   * \return the flow label value
   */
  uint32_t GetFlowLabel (void) const;

  /**
   * \brief Set the "Payload length" field.
   * \param len the length of the payload in bytes
   */
  void SetPayloadLength (uint16_t len);

  /**
   * \brief Get the "Payload length" field.
   * \return the payload length
   */
  uint16_t GetPayloadLength (void) const;

  /**
   * \brief Set the "Next header" field.
   * \param next the next header number
   */
  void SetNextHeader (uint8_t next);

  /**
   * \brief Get the next header.
   * \return the next header number
   */
  uint8_t GetNextHeader (void) const;

  /**
   * \brief Set the "Hop limit" field (TTL).
   * \param limit the 8-bit value
   */
  void SetHopLimit (uint8_t limit);

  /**
   * \brief Get the "Hop limit" field (TTL).
   * \return the hop limit value
   */
  uint8_t GetHopLimit (void) const;

  /**
   * \brief Set the "Source address" field.
   * \param src the source address
   */
  void SetSourceAddress (Ipv6Address src);

  /**
   * \brief Get the "Source address" field.
   * \return the source address
   */
  Ipv6Address GetSourceAddress (void) const;

  /**
   * \brief Set the "Destination address" field.
   * \param dst the destination address
   */
  void SetDestinationAddress (Ipv6Address dst);

  /**
   * \brief Get the "Destination address" field.
   * \return the destination address
   */
  Ipv6Address GetDestinationAddress (void) const;

  /**
   * \brief Print some informations about the packet.
   * \param os output stream
   * \return info about this packet
   */
  virtual void Print (std::ostream& os) const;

  /**
   * \brief Get the serialized size of the packet.
   * \return size
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize the packet.
   * \param start Buffer iterator
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * \brief Deserialize the packet.
   * \param start Buffer iterator
   * \return size of the packet
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  /**
   * \brief The version (always equal to 6).
   */
  uint32_t m_version : 4;
  /**
   * \brief The traffic class.
   */
  uint32_t m_trafficClass : 8;

  /**
   * \brief The flow label.
   * \note This is 20-bit value.
   */
  uint32_t m_flowLabel : 20;

  /**
   * \brief The payload length.
   */
  uint16_t m_payloadLength;

  /**
   * \brief The Next header number.
   */
  uint8_t m_nextHeader;

  /**
   * \brief The Hop limit value.
   */
  uint8_t m_hopLimit;

  /**
   * \brief The source address.
   */
  Ipv6Address m_sourceAddress;

  /**
   * \brief The destination address.
   */
  Ipv6Address m_destinationAddress;
};

} /* namespace ns3 */

#endif /* IPV6_HEADER_H */

