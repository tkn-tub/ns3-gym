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
  RTF_IN_REPAIR = 2,
  RTF_IN_SEARCH = 3
};

/**
 * \ingroup aodv
 * \brief Route Table Entry
 */
class RoutingTableEntry
{
public:
  RoutingTableEntry(Ptr<NetDevice> dev,Ipv4Address dst = Ipv4Address(), bool vSeqNo = false, u_int32_t m_seqNo = 0, Ipv4Address iface = Ipv4Address(), u_int16_t  hops = 0,
      Ipv4Address nextHop = Ipv4Address(), Time lifetime = Seconds(0));

  ~RoutingTableEntry();
  
  ///\name Precursors management
  //\{
  /**
   * Insert precursor in precursor list if it doesn't yet exist in the list
   * \param id precursor address
   * \return true on success
   */
  bool InsertPrecursor(Ipv4Address id);
  /**
   * Lookup precursor by address
   * \param id precursor address
   * \return true on success
   */
  bool LookupPrecursor(Ipv4Address id);
  /**
   * \brief Delete precursor
   * \param id precursor address
   * \return true on success
   */
  bool DeletePrecursor(Ipv4Address id);
  /// Delete all precursors
  void DeleteAllPrecursors();
  /**
   * \return true if precursor list empty
   */
  bool IsPrecursorListEmpty() const;
  //\}
  /// Return last valid hop count
  uint16_t GetLastValidHopCount() { return m_lastHopCount; }
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
  void SetValidSeqNo(bool s) { m_validSeqNo = s; }
  bool GetValidSeqNo() const { return m_validSeqNo; }
  void SetSeqNo(uint32_t sn) { m_seqNo = sn; }
  uint32_t GetSeqNo() const { return m_seqNo; }
  void SetHop(uint16_t hop) { m_hops = hop; }
  uint16_t GetHop() const {return m_hops; }
  void SetLifeTime(Time lt) { m_lifeTime = lt; }
  Time GetLifeTime() const { return m_lifeTime; }
  void SetFlag(uint8_t flag) { m_flag = flag; }
  uint8_t GetFlag() const { return m_flag; }
  void SetRreqCnt(uint8_t n) { m_reqCount = n; }
  uint8_t GetRreqCnt() const { return m_reqCount; }
  void IncrementRreqCnt() { m_reqCount++; }
  void SetRreqTimeout(Time t) {m_reqTimeout = t; }
  Time GetRreqTimeout() const { return m_reqTimeout; }
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
  /// Valid Destination Sequence Number flag
  bool m_validSeqNo;
  /// Destination Sequence Number, if m_validSeqNo = true
  uint32_t m_seqNo;
  /// Hop Count (number of hops needed to reach destination)
  uint16_t m_hops;
  /// Last valid hop count
  uint16_t m_lastHopCount;
  /**
  * \brief Expiration or deletion time of the route
  *	Lifetime field in the routing table plays dual role --
  *	for an active route it is the expiration time, and for an invalid route
  *	it is the deletion time.
  */
  Time m_lifeTime;
  /** Ip route, include
  *   - destination address
  *   - source address
  *   - next hop address (gateway)
  *   - output device
  */
  Ptr<Ipv4Route> m_ipv4Route;
  /// Routing flags: down, up or in repair
  uint8_t m_flag;

  /// List of precursors
  std::vector<Ipv4Address> m_precursorList;
  /// When I can send another request
  Time m_reqTimeout;
  /// Number of route requests
  uint8_t m_reqCount;

  // TODO review and delete
#define MAX_HISTORY     3
  double rt_disc_latency[MAX_HISTORY];
  char hist_indx;
};

/**
 * \ingroup aodv
 * The Routing Table
 */
class RoutingTable
{
public:
  RoutingTable() {}

  /**
   * Add routing table entry if it doesn't yet exist in routing table
   * \param r routing table entry
   * \return true in success
   */
  bool AddRoute(RoutingTableEntry const & r);
  /**
   * Delete routing table entry
   * \param dst destination address
   * \return true on success
   */
  bool DeleteRoute(Ipv4Address dst);
  /**
   * Lookup routing table entry
   * \param dst destination address
   * \param rt entry with destination address dst, if exists
   * \return true on success
   */
  bool LookupRoute(Ipv4Address dst, RoutingTableEntry & rt) const;
  /// Update routing table
  bool Update(Ipv4Address dst, RoutingTableEntry & rt);
  /// Set routing table entry flags
  void SetEntryState (Ipv4Address dst, uint8_t state /*TODO use enum*/);
  /// Print routing table
  void Print(std::ostream &os) const;

private:
  std::map<Ipv4Address, RoutingTableEntry> m_ipv4AddressEntry;
};

}}

#endif /* _aodv__rtable_h__ */
