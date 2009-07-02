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

namespace ns3 {
namespace aodv {

#define INFINITY2        0xff

/*
   AODV Neighbor Cache Entry
 */
class AODV_Neighbor 
{
  friend class AODV;
  friend class aodv_rt_entry;

public:
  AODV_Neighbor(Ipv4Address const & a) : nb_addr(a), nb_expire(Seconds(0)) {}

protected:
  Ipv4Address nb_addr;
  Time nb_expire;
};

/*
   AODV Precursor list data structure
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
  Ipv4Address pc_addr;	// precursor address
};

/*
  Route Table Entry
 */

class aodv_rt_entry 
{
  friend class aodv_rtable;
  friend class AODV;
  friend class LocalRepairTimer;
public:
  /// TODO make functional c-tor
  aodv_rt_entry(); 
  ~aodv_rt_entry();

  void            nb_insert(Ipv4Address id);
  /// Lookup neighbor by address, return true on success
  bool nb_lookup(Ipv4Address id, AODV_Neighbor & n);
  void            pc_insert(Ipv4Address id);
  /// Lookup precursor by address, return true on success
  bool pc_lookup(Ipv4Address id, AODV_Precursor & p);
  void 		pc_delete(Ipv4Address id);
  void 		pc_delete();
  bool 		pc_empty() const;
  
  /// Compare destination address
  bool operator==(Ipv4Address id) const
  {
    return rt_dst == id;
  }

  double          rt_req_timeout;         // when I can send another req
  u_int8_t        rt_req_cnt;             // number of route requests

protected:
  Ipv4Address     rt_dst;
  u_int32_t       rt_seqno;
  /* u_int8_t 	rt_interface; */
  u_int16_t       rt_hops;       		// hop count
  int 		  rt_last_hop_count;	// last valid hop count
  Ipv4Address     rt_nexthop;    		// next hop IP address
  /* list of precursors */ 
  std::vector<AODV_Precursor> rt_pclist;
  double          rt_expire;     		// when entry expires
  u_int8_t        rt_flags;

#define RTF_DOWN 0
#define RTF_UP 1
#define RTF_IN_REPAIR 2

  /*
   *  Must receive 4 errors within 3 seconds in order to mark
   *  the route down.
        u_int8_t        rt_errors;      // error count
        double          rt_error_time;
#define MAX_RT_ERROR            4       // errors
#define MAX_RT_ERROR_TIME       3       // seconds
   */

#define MAX_HISTORY	3
  double 		rt_disc_latency[MAX_HISTORY];
  char 		hist_indx;
  int 		rt_req_last_ttl;        // last ttl value used
  // last few route discovery latencies
  // double 		rt_length [MAX_HISTORY];
  // last few route lengths

  /*
   * a list of neighbors that are using this route.
   */
  std::vector<AODV_Neighbor> rt_nblist;
};


/*
  The Routing Table
 */

class aodv_rtable 
{
public:
  aodv_rtable() {}

  // aodv_rt_entry * head() {TODO}
  
  void rt_add(aodv_rt_entry const & r);
  void rt_delete(Ipv4Address id);
  /// Lookup routing table entry by destination. Return true on success.
  bool rt_lookup(Ipv4Address id, aodv_rt_entry & rt) const;

private:
  std::vector<aodv_rt_entry> rthead;
};

}}

#endif /* _aodv__rtable_h__ */
