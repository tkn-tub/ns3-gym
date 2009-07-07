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
#include <vector>
#include <sys/types.h>
#include "ns3/ipv4.h"
#include "ns3/nstime.h"
#include "ns3/nstime.h"

namespace ns3 {
namespace aodv {

#define INFINITY2       0xff
#define DELETE_PERIOD   10 // seconds. TODO: remove defines

/**
 * \ingroup aodv
 *
 * \brief AODV Precursor list data structure
 */
class AODV_Precursor
{
  friend class AODV;
  friend class aodv_rt_entry;
public:
  AODV_Precursor(Ipv4Address const & a) : pc_addr(a) {}
  bool operator==(AODV_Precursor const & o) const
  {
    return (pc_addr == o.pc_addr);
  }
protected:
  /// Precursor address
  Ipv4Address pc_addr;
};

/**
 * \ingroup aodv
 * \brief Route Table Entry
 */
class aodv_rt_entry
{
public:
  aodv_rt_entry();
  aodv_rt_entry(Ipv4Address dst, bool vSeqNo, u_int32_t seqNo,
  		u_int16_t  hops,Ipv4Address nextHop, Time lifetime);
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
   * \param p precursor with address id if exists
   * \return true on success
   */
  bool pc_lookup(Ipv4Address id, AODV_Precursor & p);
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
  
  /// Mark entry as "down" (i.e. disable it)
  void Down ();
  
  /**
   * \brief Compare destination address
   * \return true if equal
   */
  bool operator==(Ipv4Address const  dst) const
  {
    return rt_dst == dst;
  }

  /// When I can send another request
  Time rt_req_timeout;
  /// Number of route requests
  uint8_t rt_req_cnt;

private:
  friend class aodv_rtable;
  friend class RoutingProtocol;

  /// Destination address
  Ipv4Address rt_dst;
  /// Valid Destination Sequence Number flag
  bool validSeqNo;
  /// Destination Sequence Number, if validSeqNo = true
  uint32_t rt_seqno;
  /// Interface index
  uint32_t rt_interface;
  /// Hop Count (number of hops needed to reach destination)
  uint16_t rt_hops;
  /// Last valid hop count
  uint16_t rt_last_hop_count;
  /// Next hop IP address
  Ipv4Address rt_nexthop;
  /// List of precursors
  std::vector<AODV_Precursor> rt_pclist;
  /**
  * \brief Expiration or deletion time of the route
  *	Lifetime field in the routing table plays dual role --
  *	for an active route it is the expiration time, and for an invalid route
  *	it is the deletion time.
  */
  Time rt_lifetime;
  /// Routing flags: down, up or in repair
  uint8_t rt_flags; /*TODO use enum*/

#define RTF_DOWN        0
#define RTF_UP          1
#define RTF_IN_REPAIR   2
#define MAX_HISTORY     3

  // TODO review and delete
  double rt_disc_latency[MAX_HISTORY];
  char hist_indx;
  /// Last ttl value used
  uint16_t rt_req_last_ttl;
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
  /// Set routing table entry flags
  void SetEntryState (Ipv4Address dst, uint8_t state /*TODO use enum*/);

private:
  std::vector<aodv_rt_entry> rthead;
};

}}

#endif /* _aodv__rtable_h__ */
