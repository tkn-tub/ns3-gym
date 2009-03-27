/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#ifndef HWMP_RTABLE_H
#define HWMP_RTABLE_H

#include <map>
#include "ns3/nstime.h"
#include "ns3/mac48-address.h"
#include "ie-dot11s-perr.h"
namespace ns3 {
namespace dot11s {
/**
 * \ingroup mesh
 */
class HwmpRtable : public Object
{
public:
  static TypeId GetTypeId ();
  HwmpRtable ();
  ~HwmpRtable ();
  void DoDispose ();
  void AddReactivePath (
    Mac48Address destination,
    Mac48Address retransmitter,
    uint32_t interface,
    uint32_t metric,
    Time  lifetime,
    uint32_t seqnum
  );
  void AddProactivePath (
    uint32_t metric,
    Mac48Address root,
    Mac48Address retransmitter,
    uint32_t interface,
    Time  lifetime,
    uint32_t seqnum
  );
  void AddPrecursor (Mac48Address destination, uint32_t interface, Mac48Address precursor);
  void DeleteProactivePath ();
  void DeleteReactivePath (Mac48Address destination, uint32_t interface);
  struct LookupResult
  {
    Mac48Address retransmitter;
    uint32_t ifIndex;
    uint32_t metric;
    uint32_t seqnum;
  };
  LookupResult  LookupReactive (Mac48Address destination);
  LookupResult  LookupProactive ();
  //path error routines:
  std::vector<IePerr::FailedDestination>  GetUnreachableDestinations (Mac48Address peerAddress, uint32_t interface);
  uint32_t  RequestSeqnum (Mac48Address dst);
  std::vector<Mac48Address>  GetPrecursors (Mac48Address destination, uint32_t interface);
  const static uint32_t INTERFACE_ANY = 0xffffffff;
  const static uint32_t MAX_METRIC = 0xffffffff;
private:
  struct ReactiveRoute
  {
    Mac48Address retransmitter;
    uint32_t interface;
    uint32_t metric;
    Time whenExpire;
    uint32_t seqnum;
    std::vector<Mac48Address> precursors;
  };
  struct ProactiveRoute
  {
    Mac48Address root;
    Mac48Address retransmitter;
    uint32_t interface;
    uint32_t metric;
    Time whenExpire;
    uint32_t seqnum;
    std::vector<Mac48Address> precursors;
  };
  std::map<Mac48Address, ReactiveRoute>  m_routes;
  ProactiveRoute  m_root;
};
} //namespace dot11s
} //namespace ns3
#endif
