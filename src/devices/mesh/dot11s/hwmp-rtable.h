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
  void AddPrecursor (Mac48Address destination, uint32_t precursorInterface, Mac48Address precursorAddress);
  void DeleteProactivePath ();
  void DeleteProactivePath (Mac48Address root);
  void DeleteReactivePath (Mac48Address destination);
  struct LookupResult
  {
    Mac48Address retransmitter;
    uint32_t ifIndex;
    uint32_t metric;
    uint32_t seqnum;
  };
  LookupResult LookupReactive (Mac48Address destination);
  LookupResult LookupReactiveExpired (Mac48Address destination);
  LookupResult LookupProactive ();
  LookupResult LookupProactiveExpired ();
  //path error routines:
  ///\brief When peer link with a given MAC-address fails - it returns list of unreachable
  //destination addresses
  std::vector<IePerr::FailedDestination>  GetUnreachableDestinations (Mac48Address peerAddress);
  ///\brief When we talk about multi-interface HWMP, 'precursor' means
  //not only address but also an interface ID
  //So, when we request "precursor list", we mean that this list will
  //consist of interface ID and addresses
  typedef std::vector<std::pair<uint32_t, Mac48Address> > PRECURSOR_LIST;
  PRECURSOR_LIST GetPrecursors (Mac48Address destination);
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
    std::vector<std::pair<uint32_t, Mac48Address> > precursors;
  };
  struct ProactiveRoute
  {
    Mac48Address root;
    Mac48Address retransmitter;
    uint32_t interface;
    uint32_t metric;
    Time whenExpire;
    uint32_t seqnum;
    std::vector<std::pair<uint32_t, Mac48Address> > precursors;
  };
  std::map<Mac48Address, ReactiveRoute>  m_routes;
  ProactiveRoute  m_root;
};
} //namespace dot11s
} //namespace ns3
#endif
