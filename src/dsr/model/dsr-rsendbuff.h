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

#ifndef DSR_SENDBUFF_H
#define DSR_SENDBUFF_H

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace dsr {
/**
 * \ingroup dsr
 * \brief DSR Send Buffer Entry
 */
class SendBuffEntry
{
public:
  // / c-tor
  SendBuffEntry (Ptr<const Packet> pa = 0, Ipv4Address d = Ipv4Address (),
                 Time exp = Simulator::Now (), uint8_t p = 0)
    : m_packet (pa),
      m_dst (d),
      m_expire (exp + Simulator::Now ()),
      m_protocol (p)
  {
  }
  /**
   * Compare send buffer entries
   * \return true if equal
   */
  bool operator== (SendBuffEntry const & o) const
  {
    return ((m_packet == o.m_packet) && (m_dst == o.m_dst) && (m_expire == o.m_expire));
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
  // / Expire time for queue entry
  Time m_expire;
  // / The protocol number
  uint8_t m_protocol;
};

/**
 * \ingroup dsr
 * \brief DSR send buffer
 */
/************************************************************************************************************************/
class SendBuffer
{
public:
  // / Default c-tor
  SendBuffer ()
  {
  }
  // / Push entry in queue, if there is no entry with the same packet and destination address in queue.
  bool Enqueue (SendBuffEntry & entry);
  // / Return first found (the earliest) entry for given destination
  bool Dequeue (Ipv4Address dst, SendBuffEntry & entry);
  // / Remove all packets with destination IP address dst
  void DropPacketWithDst (Ipv4Address dst);
  // / Finds whether a packet with destination dst exists in the queue
  bool Find (Ipv4Address dst);
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
  Time GetSendBufferTimeout () const
  {
    return m_sendBufferTimeout;
  }
  void SetSendBufferTimeout (Time t)
  {
    m_sendBufferTimeout = t;
  }
  // \}

  std::vector<SendBuffEntry> & GetBuffer ()
  {
    return m_sendBuffer;
  }

private:

  std::vector<SendBuffEntry> m_sendBuffer;                      ///< The send buffer to cache unsent packet
  void Purge ();                                                ///< Remove all expired entries
  void Drop (SendBuffEntry en, std::string reason);             ///< Notify that packet is dropped from queue by timeout
  uint32_t m_maxLen;                                            ///< The maximum number of packets that we allow a routing protocol to buffer.
  Time m_sendBufferTimeout;                                     ///< The maximum period of time that a routing protocol is allowed to buffer a packet for, seconds.
  static bool IsEqual (SendBuffEntry en, const Ipv4Address dst) ///< Check if the send buffer entry is the same or not
  {
    return (en.GetDestination () == dst);
  }
};
/*******************************************************************************************************************************/
} // namespace dsr
} // namespace ns3

#endif /* DSR_SENDBUFF_H */
