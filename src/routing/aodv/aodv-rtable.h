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
#ifndef __aodv_rtable_h__
#define __aodv_rtable_h__

#include <cassert>
#include <map>
#include <sys/types.h>
#include "ns3/ipv4.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/net-device.h"


namespace ns3 {
namespace aodv {

#define INFINITY2       0xff
#define DELETE_PERIOD   10 // seconds. TODO: remove defines

enum RouteFlags
{
  RTF_DOWN = 0,
  RTF_UP = 1,
  RTF_IN_REPAIR = 2
};

/**
 * \ingroup aodv
 * \brief Route Table Entry
 */
class aodv_rt_entry
{
public:
  aodv_rt_entry(Ptr<NetDevice> dev,Ipv4Address dst = Ipv4Address(), bool vSeqNo = false, u_int32_t seqNo = 0, Ipv4Address iface = Ipv4Address(), u_int16_t  hops = 0,
      Ipv4Address nextHop = Ipv4Address(), Time lifetime = Seconds(0));

  ~aodv_rt_entry();
  
  ///\name Precursors management
  //\{
  /**
   * Insert precursor in precursor list if it doesn't yet exist in the list
   * \param id precursor address
   * \return true on success
   */
  bool pc_insert(Ipv4Address id);
  /**
   * Lookup precursor by address
   * \param id precursor address
   * \return true on success
   */
  bool pc_lookup(Ipv4Address id);
  /**
   * \brief Delete precursor
   * \param id precursor address
   * \return true on success
   */
  bool pc_delete(Ipv4Address id);
  /// Delete all precursors
  void pc_delete();
  /**
   * \return true if precursor list empty
   */
  bool pc_empty() const;
  //\}
  /// Return last valid hop count
  uint16_t GetLastValidHopCount() { return rt_last_hop_count; }
  /// Mark entry as "down" (i.e. disable it)
  void Down ();
  ///\name Fields
  //\{
  Ipv4Address GetDestination() const { return m_ipv4Route->GetDestination(); }
  Ptr<Ipv4Route> GetRoute() const { return  m_ipv4Route; }
  void SetRoute(Ptr<Ipv4Route> r) { m_ipv4Route = r; }
  void SetNextHop (Ipv4Address nextHop) { m_ipv4Route->SetGateway(nextHop); }
  Ipv4Address GetNextHop () const { return m_ipv4Route->GetGateway(); }
  void SetOutputDevice(Ptr<NetDevice> dev) { m_ipv4Route->SetOutputDevice(dev); }
  Ptr<NetDevice> GetOutputDevice() const { return m_ipv4Route->GetOutputDevice(); }
  void SetValidSeqNo(bool s) { validSeqNo = s; }
  bool GetValidSeqNo() const { return validSeqNo; }
  void SetSeqNo(uint32_t sn) { rt_seqno = sn; }
  uint32_t GetSeqNo() const { return rt_seqno; }
  void SetHop(uint16_t hop) { rt_hops = hop; }
  uint16_t GetHop() const {return rt_hops; }
  void SetLifeTime(Time lt) { rt_lifetime = lt; }
  Time GetLifeTime() const { return rt_lifetime; }
  void SetFlag(uint8_t flag) { rt_flags = flag; }
  uint8_t GetFlag() const { return rt_flags; }
  void SetRreqCnt(uint8_t n) { rt_req_cnt = n; }
  uint8_t GetRreqCnt() const { return rt_req_cnt; }
  void SetRreqTimeout(Time t) {rt_req_timeout = t; }
  Time GetRreqTimeout() const { return rt_req_timeout; }
  //\}

  /**
   * \brief Compare destination address
   * \return true if equal
   */
  bool operator==(Ipv4Address const  dst) const
  {
    return (m_ipv4Route->GetDestination() == dst);
  }
  void Print(std::ostream & os) const;
private:
  friend class aodv_rtable;
  /// Valid Destination Sequence Number flag
  bool validSeqNo;
  /// Destination Sequence Number, if validSeqNo = true
  uint32_t rt_seqno;
  /// Hop Count (number of hops needed to reach destination)
  uint16_t rt_hops;
  /// Last valid hop count
  uint16_t rt_last_hop_count;

  /**
  * \brief Expiration or deletion time of the route
  *	Lifetime field in the routing table plays dual role --
  *	for an active route it is the expiration time, and for an invalid route
  *	it is the deletion time.
  */
  Time rt_lifetime;

  /** Ip route, include
  *   - destination address
  *   - source address
  *   - next hop address (gateway)
  *   - output device
  */
  Ptr<Ipv4Route> m_ipv4Route;

  /// Routing flags: down, up or in repair
  uint8_t rt_flags; /*TODO use enum*/

#define MAX_HISTORY     3

  /// List of precursors
  std::vector<Ipv4Address> rt_pclist;
  /// When I can send another request
  Time rt_req_timeout;
  /// Number of route requests
  uint8_t rt_req_cnt;

  // TODO review and delete
  double rt_disc_latency[MAX_HISTORY];
  char hist_indx;
};

/**
 * \ingroup aodv
 * The Routing Table
 */
class aodv_rtable
{
public:
  aodv_rtable() {}

  // aodv_rt_entry * head() {TODO}
  /**
   * Add routing table entry if it doesn't yet exist in routing table
   * \param r routing table entry
   * \return true in success
   */
  bool rt_add(aodv_rt_entry const & r);
  /**
   * Delete routing table entry
   * \param dst destination address
   * \return true on success
   */
  bool rt_delete(Ipv4Address dst);
  /**
   * Lookup routing table entry
   * \param dst destination address
   * \param rt entry with destination address dst, if exists
   * \return true on success
   */
  bool rt_lookup(Ipv4Address dst, aodv_rt_entry & rt) const;
  /// Update routing table
  bool Update(Ipv4Address dst, aodv_rt_entry & rt);
  /// Set routing table entry flags
  void SetEntryState (Ipv4Address dst, uint8_t state /*TODO use enum*/);
  /// Print routing table
  void Print(std::ostream &os) const;

private:
  std::map<Ipv4Address, aodv_rt_entry> rthead;
};

}}

#endif /* _aodv__rtable_h__ */
