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

#include "aodv-rtable.h"
#include <algorithm>
#include "ns3/test.h"

namespace ns3 {
namespace aodv {

/*
  The Routing Table
 */

aodv_rt_entry::aodv_rt_entry()
{
  rt_req_timeout = MilliSeconds(0.0);
  rt_req_cnt = 0;
  rt_dst = Ipv4Address("127.0.0.1");
  validSeqNo = false;
  rt_seqno = 0;
  rt_hops = rt_last_hop_count = INFINITY2;
  rt_nexthop = Ipv4Address("127.0.0.1");
  rt_lifetime = MilliSeconds(0.0);
  rt_flags = RTF_DOWN;

  for (int i=0; i < MAX_HISTORY; i++)
    {
      rt_disc_latency[i] = 0.0;
    }
  hist_indx = 0;
  rt_req_last_ttl = 0;
}

aodv_rt_entry::aodv_rt_entry(Ipv4Address dst, bool vSeqNo, u_int32_t seqNo,
		u_int16_t  hops,Ipv4Address nextHop, Time lifetime)
{
  rt_dst = dst;
  validSeqNo = vSeqNo;
  if (validSeqNo) rt_seqno = seqNo;
  rt_hops = hops;
  rt_nexthop = nextHop;
  rt_lifetime = lifetime;
}

aodv_rt_entry::~aodv_rt_entry()
{
}

bool
aodv_rt_entry::pc_insert(Ipv4Address id)
{
  AODV_Precursor p (id);

  if (! pc_lookup(id, p))
    {
      rt_pclist.push_back(p);
      return true;
    }
  else return false;
}

bool
aodv_rt_entry::pc_lookup(Ipv4Address id, AODV_Precursor & p)
{
  for(std::vector<AODV_Precursor>::const_iterator i = rt_pclist.begin(); i != rt_pclist.end(); ++i)
    if (i->pc_addr == id)
      {
        p = *i;
        return true;
      }
  return false;
}

bool
aodv_rt_entry::pc_delete(Ipv4Address id)
{
  AODV_Precursor p(id);
  std::vector<AODV_Precursor>::iterator i = std::remove(rt_pclist.begin(), rt_pclist.end(), p);
  if(i == rt_pclist.end())
    return false;
  else
    rt_pclist.erase (i, rt_pclist.end());
  return true;
}

void
aodv_rt_entry::pc_delete()
{
  rt_pclist.clear();
}

bool
aodv_rt_entry::pc_empty() const
{
  return rt_pclist.empty();
}

/*
  The Routing Table
 */

bool
aodv_rtable::rt_lookup(Ipv4Address id, aodv_rt_entry & rt) const
{
  std::vector<aodv_rt_entry>::const_iterator i = std::find (rthead.begin(), rthead.end(), id);
  if (i == rthead.end()) return false;
  rt = *i;
  return true;
}

bool
aodv_rtable::rt_delete(Ipv4Address dst)
{
  std::vector<aodv_rt_entry>::iterator i = std::remove(rthead.begin(), rthead.end(), dst);
  if (i == rthead.end()) return false;
  rthead.erase (i, rthead.end());
  return true;
}

bool
aodv_rtable::rt_add(aodv_rt_entry const & rt)
{
  aodv_rt_entry dummy;
  if(rt_lookup(rt.rt_dst, dummy))
  	return false;
  rthead.push_back(rt);
  return true;
}

#ifdef RUN_SELF_TESTS
/// Unit test for aodv_rqueue
struct AodvRtableTest : public Test
{
  AodvRtableTest () : Test ("AODV/Rtable"), result(true) {}
  virtual bool RunTests();
  bool result;
};

/// Test instance
static AodvRtableTest g_AodvRtableTest;

bool
AodvRtableTest::RunTests ()
{
  AODV_Neighbor nb1(Ipv4Address("1.2.3.4"));
  AODV_Neighbor nb2(Ipv4Address("4.3.2.1"));
  NS_TEST_ASSERT(!(nb1==nb2));
  
  AODV_Neighbor nb3(Ipv4Address("3.3.3.3"));
  AODV_Neighbor nb4(Ipv4Address("3.3.3.3"), Seconds(1));
  NS_TEST_ASSERT(!(nb1==nb2));
  
  AODV_Precursor pc1(Ipv4Address("1.1.1.1"));
  AODV_Precursor pc2(Ipv4Address("2.2.2.2"));
  NS_TEST_ASSERT(!(pc1==pc2));
  
  aodv_rt_entry entry1;
  Ipv4Address dst1("3.3.3.3");
  Ipv4Address dst2("1.2.3.4");
  aodv_rt_entry entry2 = aodv_rt_entry(dst2, true, 34, 1, Ipv4Address("5.5.5.5"),  Seconds(5));
  NS_TEST_ASSERT( !(entry1 == dst1) );
  NS_TEST_ASSERT(entry2 == dst2);
  
  entry2.pc_insert(dst1);
  AODV_Precursor pc3(dst1);
  NS_TEST_ASSERT(entry2.pc_lookup(dst1, pc2));
  NS_TEST_ASSERT(pc3==pc2);
  NS_TEST_ASSERT(!entry2.pc_delete(dst2));
  NS_TEST_ASSERT(entry2.pc_delete(dst1));
  NS_TEST_ASSERT(entry2.pc_empty());
  entry2.pc_insert(dst2);
  NS_TEST_ASSERT(entry2.pc_insert(dst1));
  NS_TEST_ASSERT(!entry2.pc_insert(dst2));
  entry2.pc_delete();
  NS_TEST_ASSERT(entry2.pc_empty());
  
  aodv_rtable rtable;
  rtable.rt_add(entry2);
  NS_TEST_ASSERT(rtable.rt_lookup(dst2, entry1));
  NS_TEST_ASSERT(entry1 == dst2);
  NS_TEST_ASSERT(!rtable.rt_lookup(dst1, entry1));
  NS_TEST_ASSERT(rtable.rt_delete(dst2));
  NS_TEST_ASSERT(!rtable.rt_lookup(dst2, entry1));
  
  return result;
}
#endif

}}
