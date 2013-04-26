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

#ifndef DSR_PASSIVEBUFF_H
#define DSR_PASSIVEBUFF_H

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace dsr {
/**
 * \ingroup dsr
 * \brief DSR Passive Buffer Entry
 */
class PassiveBuffEntry
{
public:
  // / c-tor
  PassiveBuffEntry (Ptr<const Packet> pa = 0, Ipv4Address d = Ipv4Address (), Ipv4Address s = Ipv4Address (),
                  Ipv4Address n = Ipv4Address (), uint16_t i = 0, uint16_t f = 0, uint8_t seg = 0, Time exp = Simulator::Now (),
                  uint8_t p = 0)
    : m_packet (pa),
      m_dst (d),
      m_source (s),
      m_nextHop (n),
      m_identification (i),
      m_fragmentOffset (f),
      m_segsLeft (seg),
      m_expire (exp + Simulator::Now ()),
      m_protocol (p)
  {
  }
  /**
   * Compare send buffer entries
   * \return true if equal
   */
  bool operator== (PassiveBuffEntry const & o) const
  {
    return ((m_packet == o.m_packet) && (m_source == o.m_source) && (m_nextHop == o.m_nextHop) && (m_dst == o.m_dst) && (m_expire == o.m_expire));
  }
  // /\name Fields
  // \{
  Ptr<const Packet> GetPacket () const
  {
    return m_packet;
  }
  void SetPacket (Ptr<const Packet> p)
  {
    m_packet = p;
  }
  Ipv4Address GetDestination () const
  {
    return m_dst;
  }
  void SetDestination (Ipv4Address d)
  {
    m_dst = d;
  }
  Ipv4Address GetSource () const
  {
    return m_source;
  }
  void SetSource (Ipv4Address s)
  {
    m_source = s;
  }
  Ipv4Address GetNextHop () const
  {
    return m_nextHop;
  }
  void SetNextHop (Ipv4Address n)
  {
    m_nextHop = n;
  }
  uint16_t GetIdentification () const
  {
    return m_identification;
  }
  void SetIdentification (uint16_t i)
  {
    m_identification = i;
  }
  uint16_t GetFragmentOffset () const
  {
    return m_fragmentOffset;
  }
  void SetFragmentOffset (uint16_t f)
  {
    m_fragmentOffset = f;
  }
  uint8_t GetSegsLeft () const
  {
    return m_segsLeft;
  }
  void SetSegsLeft (uint8_t seg)
  {
    m_segsLeft = seg;
  }
  void SetExpireTime (Time exp)
  {
    m_expire = exp + Simulator::Now ();
  }
  Time GetExpireTime () const
  {
    return m_expire - Simulator::Now ();
  }
  void SetProtocol (uint8_t p)
  {
    m_protocol = p;
  }
  uint8_t GetProtocol () const
  {
    return m_protocol;
  }
  // \}
private:
  // / Data packet
  Ptr<const Packet> m_packet;
  // / Destination address
  Ipv4Address m_dst;
  // / Source address
  Ipv4Address m_source;
  // / Nexthop address
  Ipv4Address m_nextHop;
  // /
  uint16_t m_identification;
  uint16_t m_fragmentOffset;
  uint8_t m_segsLeft;
  // / Expire time for queue entry
  Time m_expire;
  // / The protocol number
  uint8_t m_protocol;
};

/**
 * \ingroup dsr
 * \brief DSR passive buffer
 */
/************************************************************************************************************************/
class PassiveBuffer  : public Object
{
public:
  // / c-tor
  /**
   * \brief Get the type identificator.
   * \return type identificator
   */
  static TypeId GetTypeId ();
  /**
   * \brief Constructor.
   */
  PassiveBuffer ();
  /**
   * \brief Destructor.
   */
  virtual ~PassiveBuffer ();
  // / Push entry in queue, if there is no entry with the same packet and destination address in queue.
  bool Enqueue (PassiveBuffEntry & entry);
  // / Return first found (the earliest) entry for given destination
  bool Dequeue (Ipv4Address dst, PassiveBuffEntry & entry);
  // / Finds whether a packet with destination dst exists in the queue
  bool Find (Ipv4Address dst);
  // / Check if all the entries in passive buffer entry is all equal or not
  bool AllEqual (PassiveBuffEntry & entry);
  // / Number of entries
  uint32_t GetSize ();
  // /\name Fields
  // \{
  uint32_t GetMaxQueueLen () const
  {
    return m_maxLen;
  }
  void SetMaxQueueLen (uint32_t len)
  {
    m_maxLen = len;
  }
  Time GetPassiveBufferTimeout () const
  {
    return m_passiveBufferTimeout;
  }
  void SetPassiveBufferTimeout (Time t)
  {
    m_passiveBufferTimeout = t;
  }
  // \}

private:
  // / The send buffer to cache unsent packet
  std::vector<PassiveBuffEntry> m_passiveBuffer;
  // / Remove all expired entries
  void Purge ();
  // / Notify that packet is dropped from queue by timeout
  void Drop (PassiveBuffEntry en, std::string reason);
  // / Notify that packet is dropped from queue by timeout
  void DropLink (PassiveBuffEntry en, std::string reason);
  // / The maximum number of packets that we allow a routing protocol to buffer.
  uint32_t m_maxLen;
  // / The maximum period of time that a routing protocol is allowed to buffer a packet for, seconds.
  Time m_passiveBufferTimeout;
  // / Check if the send buffer entry is the same or not
  static bool LinkEqual (PassiveBuffEntry en, const std::vector<Ipv4Address> link)
  {
    return ((en.GetSource () == link[0]) && (en.GetNextHop () == link[1]));
  }
};
/*******************************************************************************************************************************/
} // namespace dsr
} // namespace ns3

#endif /* DSR_PASSIVEBUFF_H */
