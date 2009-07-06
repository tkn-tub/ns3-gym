/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 1997, 1998 Carnegie Mellon University.
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
 * Authors: The AODV code developed by the CMU/MONARCH group was optimized and
 * tuned by Samir Das and Mahesh Marina, University of Cincinnati.
 * The work was partially done in Sun Microsystems.
 *
 * Ported to ns-3 by Elena Borovkova <borovkovaes@iitp.ru>
 */
#ifndef AODVPACKET_H_
#define AODVPACKET_H_

#include <stdint.h>
#include <iostream>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"

namespace ns3 {
namespace aodv {

/// AODV message types
enum MessageType 
{
  AODVTYPE_HELLO = 0x01,
  AODVTYPE_RREQ  = 0x02,
  AODVTYPE_RREP  = 0x04,
  AODVTYPE_RERR  = 0x08,
  AODVTYPE_RREP_ACK = 0x10
};


/**
 * \ingroup aodv
 * \brief   Route Request (RREQ) Message Format

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
 */
class RreqHeader : public Header 
{
public:
  RreqHeader ();
  
  ///\name Header serialization/deserialization
  //\{
  uint32_t GetSerializedSize ();
  void Serialize (Buffer::Iterator start);
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
  void SetSrc (Ipv4Address a) { rq_src = a; }
  Ipv4Address GetSrc () const { return rq_src; }  
  void SetSrcSeqno (uint32_t s) { rq_src_seqno = s; }
  uint32_t GetSrcSeqno () const { return rq_src_seqno; }
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
   static MessageType type() { return AODVTYPE_RREQ; }
   
   uint8_t         rq_flags;       ///< |J|R|G|D|U| bit flags, see RFC
   uint8_t        reserved;       ///< Not used
   uint8_t        rq_hop_count;   ///< Hop Count
   uint32_t       rq_bcast_id;    ///< RREQ ID
   Ipv4Address     rq_dst;         ///< Destination IP Address
   uint32_t       rq_dst_seqno;   ///< Destination Sequence Number
   Ipv4Address     rq_src;         ///< Source IP Address
   uint32_t       rq_src_seqno;   ///< Source Sequence Number
};

std::ostream & operator<<(std::ostream & os, RreqHeader const &);

}
}
#endif /* AODVPACKET_H_ */
