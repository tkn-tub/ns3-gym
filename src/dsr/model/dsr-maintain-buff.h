/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Yufei Cheng
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
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#ifndef DSR_MAINTAIN_BUFF_H
#define DSR_MAINTAIN_BUFF_H

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-header.h"
#include "dsr-option-header.h"

namespace ns3 {
namespace dsr {
/**
 * The maintenance buffer is responsible for maintaining packet next hop delivery
 * The data packet is saved in maintenance buffer whenever the data packet is sent out of send buffer
 */
struct LinkKey
{
  Ipv4Address m_source;
  Ipv4Address m_destination;
  Ipv4Address m_ourAdd;
  Ipv4Address m_nextHop;

  /**
   * Compare maintain Buffer entries
   * \param o
   * \return true if equal
   */
  bool operator < (LinkKey const & o) const
  {
    return ((m_source < o.m_source) && (m_destination < o.m_destination)
            && (m_ourAdd < o.m_nextHop) && (m_nextHop < o.m_nextHop)
            );
  }
};

struct NetworkKey
{
  uint16_t m_ackId;
  Ipv4Address m_ourAdd;
  Ipv4Address m_nextHop;
  Ipv4Address m_source;
  Ipv4Address m_destination;

  /**
   * Compare maintain Buffer entries
   * \param o
   * \return true if equal
   */
  bool operator < (NetworkKey const & o) const
  {
    return ((m_ackId < o.m_ackId) && (m_ourAdd < o.m_ourAdd) && (m_nextHop < o.m_nextHop) && (m_source < o.m_source)
            && (m_destination < o.m_destination)
            );
  }
};

struct PassiveKey
{
  uint16_t m_ackId;
  Ipv4Address m_source;
  Ipv4Address m_destination;
  uint8_t m_segsLeft;

  /**
   * Compare maintain Buffer entries
   * \param o
   * \return true if equal
   */
  bool operator < (PassiveKey const & o) const
  {
    return ((m_ackId < o.m_ackId) && (m_source < o.m_source)
            && (m_destination < o.m_destination) && (m_segsLeft < o.m_segsLeft)
            );
  }
};

/**
 * \ingroup dsr
 * \brief DSR Maintain Buffer Entry
 */
class MaintainBuffEntry
{
public:
  /**
   * Construct a MaintainBuffEntry with the given parameters
   *
   * \param pa packet
   * \param us our IPv4 address
   * \param n next hop IPv4 address
   * \param s IPv4 address of the source
   * \param dst IPv4 address of the destination
   * \param ackId ACK ID
   * \param segs number of segments left
   * \param exp expiration time
   */
  MaintainBuffEntry (Ptr<const Packet> pa = 0, Ipv4Address us = Ipv4Address (),
                     Ipv4Address n = Ipv4Address (), Ipv4Address s = Ipv4Address (), Ipv4Address dst = Ipv4Address (),
                     uint16_t ackId = 0, uint8_t segs = 0, Time exp = Simulator::Now ())
    : m_packet (pa),
      m_ourAdd (us),
      m_nextHop (n),
      m_src (s),
      m_dst (dst),
      m_ackId (ackId),
      m_segsLeft (segs),
      m_expire (exp + Simulator::Now ())
  {
  }

  ///\name Fields
  // \{
  Ptr<const Packet> GetPacket () const
  {
    return m_packet;
  }
  void SetPacket (Ptr<const Packet> p)
  {
    m_packet = p;
  }
  Ipv4Address GetOurAdd () const
  {
    return m_ourAdd;
  }
  void SetOurAdd (Ipv4Address us)
  {
    m_ourAdd = us;
  }
  Ipv4Address GetNextHop () const
  {
    return m_nextHop;
  }
  void SetNextHop (Ipv4Address n)
  {
    m_nextHop = n;
  }
  Ipv4Address GetDst () const
  {
    return m_dst;
  }
  void SetDst (Ipv4Address n)
  {
    m_dst = n;
  }
  Ipv4Address GetSrc () const
  {
    return m_src;
  }
  void SetSrc (Ipv4Address s)
  {
    m_src = s;
  }
  uint16_t GetAckId () const
  {
    return m_ackId;
  }
  void SetAckId (uint16_t ackId)
  {
    m_ackId = ackId;
  }
  uint8_t GetSegsLeft () const
  {
    return m_segsLeft;
  }
  void SetSegsLeft (uint8_t segs)
  {
    m_segsLeft = segs;
  }
  void SetExpireTime (Time exp)
  {
    m_expire = exp + Simulator::Now ();
  }
  Time GetExpireTime () const
  {
    return m_expire - Simulator::Now ();
  }
  // \}
private:
  /// Data packet
  Ptr<const Packet> m_packet;
  /// Our own ip address
  Ipv4Address m_ourAdd;
  /// Next hop Ip address
  Ipv4Address m_nextHop;
  /// The source address
  Ipv4Address m_src;
  /// The destination address
  Ipv4Address m_dst;
  /// The data ack id
  uint16_t m_ackId;
  /// The segments left field
  uint8_t m_segsLeft;
  /// Expire time for queue entry
  Time m_expire;
};
/**
 * \ingroup dsr
 * \brief DSR maintain buffer
 */
/************************************************************************************************************************/
class MaintainBuffer
{
public:
  /**
   * Default constructor
   */
  MaintainBuffer ()
  {
  }
  /// Push entry in queue, if there is no entry with the same packet and destination address in queue.
  bool Enqueue (MaintainBuffEntry & entry);
  /// Return first found (the earliest) entry for given destination
  bool Dequeue (Ipv4Address dst, MaintainBuffEntry & entry);
  /// Remove all packets with destination IP address dst
  void DropPacketWithNextHop (Ipv4Address nextHop);
  /// Finds whether a packet with destination dst exists in the queue
  bool Find (Ipv4Address nextHop);
  /// Number of entries
  uint32_t GetSize ();
  ///\name Fields
  // \{
  uint32_t GetMaxQueueLen () const
  {
    return m_maxLen;
  }
  void SetMaxQueueLen (uint32_t len)
  {
    m_maxLen = len;
  }
  Time GetMaintainBufferTimeout () const
  {
    return m_maintainBufferTimeout;
  }
  void SetMaintainBufferTimeout (Time t)
  {
    m_maintainBufferTimeout = t;
  }
  /// Verify if all the elements in the maintainence buffer entry is the same
  bool AllEqual (MaintainBuffEntry & entry);
  /// Verify if the maintain buffer entry is the same in every field for link ack
  bool LinkEqual (MaintainBuffEntry & entry);
  /// Verify if the maintain buffer entry is the same in every field for network ack
  bool NetworkEqual (MaintainBuffEntry & entry);
  /// Verify if the maintain buffer entry is the same in every field for promiscuous ack
  bool PromiscEqual (MaintainBuffEntry & entry);
  // \}

private:
  /// The vector of maintain buffer entries
  std::vector<MaintainBuffEntry> m_maintainBuffer;
  std::vector<NetworkKey> m_allNetworkKey;
  /// Remove all expired entries
  void Purge ();
  /// The maximum number of packets that we allow a routing protocol to buffer.
  uint32_t m_maxLen;
  /// The maximum period of time that a routing protocol is allowed to buffer a packet for, seconds.
  Time m_maintainBufferTimeout;
  /// Verify if the maintain buffer is equal or not
  static bool IsEqual (MaintainBuffEntry en, const Ipv4Address nextHop)
  {
    return (en.GetNextHop () == nextHop);
  }
};
/*******************************************************************************************************************************/
} // namespace dsr
} // namespace ns3
#endif /* DSR_MAINTAIN_BUFF_H */
