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

#ifndef DSR_NETWORK_QUEUE_H
#define DSR_NETWORK_QUEUE_H

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-header.h"
#include "dsr-option-header.h"

namespace ns3 {
namespace dsr {

enum DsrMessageType
{
  DSR_CONTROL_PACKET = 1,
  DSR_DATA_PACKET = 2
};
/**
 * \ingroup dsr
 * \brief DSR Network Queue Entry
 */
class DsrNetworkQueueEntry
{
public:
  /// c-tor
  DsrNetworkQueueEntry (Ptr<const Packet> pa = 0, Ipv4Address s = Ipv4Address (), Ipv4Address n = Ipv4Address (),
                        Time exp = Simulator::Now (), Ptr<Ipv4Route> r = 0)
    : m_packet (pa),
      m_srcAddr (s),
      m_nextHopAddr (n),
      tstamp (exp),
      m_ipv4Route (r)
  {
  }
  /**
   * Compare send buffer entries
   * \return true if equal
   */
  bool operator== (DsrNetworkQueueEntry const & o) const
  {
    return ((m_packet == o.m_packet) && (m_srcAddr == o.m_srcAddr) && (m_nextHopAddr == o.m_nextHopAddr)
            && (tstamp == o.tstamp) && (m_ipv4Route == o.m_ipv4Route));
  }

  ///\name Fields
  //\{
  Ptr<const Packet> GetPacket () const
  {
    return m_packet;
  }
  void SetPacket (Ptr<const Packet> p)
  {
    m_packet = p;
  }
  Ptr<Ipv4Route> GetIpv4Route () const
  {
    return m_ipv4Route;
  }
  void SetIpv4Route (Ptr<Ipv4Route> route)
  {
    m_ipv4Route = route;
  }
  Ipv4Address GetSourceAddress () const
  {
    return m_srcAddr;
  }
  void SetSourceAddress (Ipv4Address addr)
  {
    m_srcAddr = addr;
  }
  Ipv4Address GetNextHopAddress () const
  {
    return m_nextHopAddr;
  }
  void SetNextHopAddress (Ipv4Address addr)
  {
    m_nextHopAddr = addr;
  }
  Time GetInsertedTimeStamp (void) const
  {
    return tstamp;
  }
  void SetInsertedTimeStamp (Time time)
  {
    tstamp = time;
  }
  //\}
private:
  /// Data packet
  Ptr<const Packet> m_packet;
  Ipv4Address m_srcAddr;
  Ipv4Address m_nextHopAddr;
  Time tstamp;
  /// Ipv4Route
  Ptr<Ipv4Route> m_ipv4Route;
};

class DsrNetworkQueue : public Object
{
public:
  static TypeId GetTypeID (void);
  /// Default c-tor
  DsrNetworkQueue ();
  DsrNetworkQueue (uint32_t maxLen, Time maxDelay);
  ~DsrNetworkQueue ();
  /// Push entry in queue, if there is no entry with the same packet and destination address in queue.
  bool Enqueue (DsrNetworkQueueEntry & entry);
  /// Return first found (the earliest) entry for given destination
  bool Dequeue (DsrNetworkQueueEntry & entry);
  /// Number of entries
  uint32_t GetSize ();

  void SetMaxNetworkSize (uint32_t maxSize);
  void SetMaxNetworkDelay (Time delay);
  uint32_t GetMaxNetworkSize (void) const;
  Time GetMaxNetworkDelay (void) const;
  void Flush (void);

  std::vector<DsrNetworkQueueEntry> & GetQueue ()
  {
    return m_dsrNetworkQueue;
  }

private:
  void Cleanup (void);
  std::vector<DsrNetworkQueueEntry> m_dsrNetworkQueue;
  uint32_t m_size;
  uint32_t m_maxSize;
  Time m_maxDelay;
};

} // namespace dsr
} // namespace ns3

#endif /* DSR_NETWORK_QUEUE_H */
