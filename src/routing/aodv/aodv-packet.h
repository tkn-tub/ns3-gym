/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Based on 
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 * 
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Borovkova <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#ifndef AODVPACKET_H_
#define AODVPACKET_H_

#include <stdint.h>
#include <iostream>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include <map>
#include "ns3/nstime.h"

namespace ns3 {
namespace aodv {

enum MessageType
{
  AODVTYPE_RREQ  = 1,  //!< AODVTYPE_RREQ
  AODVTYPE_RREP  = 2,  //!< AODVTYPE_RREP
  AODVTYPE_RERR  = 3,  //!< AODVTYPE_RERR
  AODVTYPE_RREP_ACK = 4//!< AODVTYPE_RREP_ACK
};

/**
* \ingroup aodv
* \brief AODV types
*/
class TypeHeader : public Header
{
public:
  TypeHeader(uint8_t t);

  ///\name Header serialization/deserialization
  //\{
  TypeId GetInstanceTypeId() const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;
  //\}

  uint8_t Get() const { return type; }
  bool IsValid() const { return valid; }
  bool operator==(TypeHeader const & o) const;
private:
  uint8_t type;
  bool valid;
};

std::ostream & operator<<(std::ostream & os, TypeHeader const & h);

/**
* \ingroup aodv
* \brief   Route Request (RREQ) Message Format
  \verbatim
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |J|R|G|D|U|   Reserved          |   Hop Count   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                            RREQ ID                            |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                    Destination IP Address                     |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                  Destination Sequence Number                  |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                    Originator IP Address                      |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                  Originator Sequence Number                   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class RreqHeader : public Header 
{
public:
  RreqHeader ();

  ///\name Header serialization/deserialization
  //\{
  TypeId GetInstanceTypeId() const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;
  //\}

  ///\name Fields
  //\{
  void SetHopCount (uint8_t count) { rq_hop_count = count; }
  uint8_t GetHopCount () const { return rq_hop_count; }
  void SetId (uint32_t id) { rq_bcast_id = id; }
  uint8_t GetId () const { return rq_bcast_id; }  
  void SetDst (Ipv4Address a) { rq_dst = a; }
  Ipv4Address GetDst () const { return rq_dst; }
  void SetDstSeqno (uint32_t s) { rq_dst_seqno = s; }
  uint32_t GetDstSeqno () const { return rq_dst_seqno; }
  void SetOrigin (Ipv4Address a) { rq_src = a; }
  Ipv4Address GetOrigin () const { return rq_src; }
  void SetOriginSeqno (uint32_t s) { rq_src_seqno = s; }
  uint32_t GetOriginSeqno () const { return rq_src_seqno; }
  //\}

  ///\name Flags
  //\{
  void SetGratiousRrep (bool f);
  bool GetGratiousRrep () const;
  void SetDestinationOnly (bool f);
  bool GetDestinationOnly () const;
  void SetUnknownSeqno (bool f);
  bool GetUnknownSeqno () const;
  //\}

  bool operator==(RreqHeader const & o) const;
private:
  uint8_t        rq_flags;       ///< |J|R|G|D|U| bit flags, see RFC
  uint8_t        reserved;       ///< Not used
  uint8_t        rq_hop_count;   ///< Hop Count
  uint32_t       rq_bcast_id;    ///< RREQ ID
  Ipv4Address    rq_dst;         ///< Destination IP Address
  uint32_t       rq_dst_seqno;   ///< Destination Sequence Number
  Ipv4Address    rq_src;         ///< Source IP Address
  uint32_t       rq_src_seqno;   ///< Source Sequence Number
};

std::ostream & operator<<(std::ostream & os, RreqHeader const &);

/**
* \ingroup aodv
* \brief Route Reply (RREP) Message Format
  \verbatim
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |R|A|    Reserved     |Prefix Sz|   Hop Count   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                     Destination IP address                    |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                  Destination Sequence Number                  |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                    Originator IP address                      |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                           Lifetime                            |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class RrepHeader : public Header
{
public:
  RrepHeader();
  ///\name Header serialization/deserialization
  //\{
  TypeId GetInstanceTypeId() const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;
  //\}

  ///\name Fields
  //\{
  void SetHopCount (uint8_t count) { rp_hop_count = count; }
  uint8_t GetHopCount () const { return rp_hop_count; }
  void SetDst (Ipv4Address a) { rp_dst = a; }
  Ipv4Address GetDst () const { return rp_dst; }
  void SetDstSeqno (uint32_t s) { rp_dst_seqno = s; }
  uint32_t GetDstSeqno () const { return rp_dst_seqno; }
  void SetOrigin (Ipv4Address a) { rp_src = a; }
  Ipv4Address GetOrigin () const { return rp_src; }
  void SetLifeTime (Time t);
  Time GetLifeTime () const;
  //\}

  ///\name Flags
  //\{
  void SetAckRequired (bool f);
  bool GetAckRequired () const;
  void SetPrefixSize(uint8_t sz);
  uint8_t GetPrefixSize() const;
  //\}

  /// Configure RREP to be a Hello message
  void SetHello(Ipv4Address src, uint32_t srcSeqNo, Time lifetime);

  bool operator==(RrepHeader const & o) const;
private:
  uint8_t       rp_flags;	    ///< A - acknowledgment required flag
  uint8_t       prefixSize;	    ///< Prefix Size
  uint8_t	rp_hop_count;       ///< Hop Count
  Ipv4Address   rp_dst;             ///< Destination IP Address
  uint32_t      rp_dst_seqno;       ///< Destination Sequence Number
  Ipv4Address	rp_src;             ///< Source IP Address
  uint32_t      rp_lifetime;        ///< Lifetime
};

std::ostream & operator<<(std::ostream & os, RrepHeader const &);

/**
* \ingroup aodv
* \brief Route Reply Acknowledgment (RREP-ACK) Message Format
  \verbatim
  0                   1
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |   Reserved    |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class RrepAckHeader : public Header
{
public:
  RrepAckHeader ();

  ///\name Header serialization/deserialization
  //\{
  TypeId GetInstanceTypeId() const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;
  //\}

  bool operator==(RrepAckHeader const & o) const;
private:
  uint8_t       reserved;
};
std::ostream & operator<<(std::ostream & os, RrepAckHeader const &);


/**
* \ingroup aodv
* \brief Route Error (RERR) Message Format
  \verbatim
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |N|          Reserved           |   DestCount   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |            Unreachable Destination IP Address (1)             |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |         Unreachable Destination Sequence Number (1)           |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-|
  |  Additional Unreachable Destination IP Addresses (if needed)  |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |Additional Unreachable Destination Sequence Numbers (if needed)|
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class RerrHeader : public Header
{
public:
  RerrHeader ();

  ///\name Header serialization/deserialization
  //\{
  TypeId GetInstanceTypeId() const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator i) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;
  //\}

  ///\name No delete flag
  //\{
  void SetNoDelete(bool f);
  bool GetNoDelete() const;
  //\}

  bool AddUnDestination(Ipv4Address dst, uint32_t seqNo);
  uint8_t GetDestCount() const { return (uint8_t)unreachable_dst.size(); }
  bool operator==(RerrHeader const & o) const;
private:
  uint8_t er_flag;        ///< No delete flag
  uint8_t reserved;       ///< Not used

  /// List of Unreachable destination IP addresses and sequence numbers
  std::map<Ipv4Address, uint32_t> unreachable_dst;
};

std::ostream & operator<<(std::ostream & os, RerrHeader const &);
}
}
#endif /* AODVPACKET_H_ */
