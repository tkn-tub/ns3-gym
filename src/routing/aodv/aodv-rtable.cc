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
#include "aodv-rtable.h"
#include <algorithm>
#include "ns3/test.h"

namespace ns3 {
namespace aodv {

/*
  The Routing Table
 */


aodv_rt_entry::aodv_rt_entry(Ptr<NetDevice> dev, Ipv4Address dst, bool vSeqNo, u_int32_t seqNo, Ipv4Address iface, u_int16_t  hops,
                              Ipv4Address nextHop, Time lifetime)
                            : validSeqNo(vSeqNo), rt_seqno(seqNo), rt_hops(hops), rt_last_hop_count(hops), rt_lifetime(lifetime), rt_req_cnt(0)
{
  m_ipv4Route = Create<Ipv4Route> ();
  m_ipv4Route->SetDestination(dst);
  m_ipv4Route->SetGateway(nextHop);
  m_ipv4Route->SetSource(iface);
  m_ipv4Route->SetOutputDevice(dev);
  rt_flags = RTF_UP;
  for(uint8_t i = 0; i < MAX_HISTORY; ++i)
    {
     rt_disc_latency[i] = 0;
    }
  hist_indx = 0;
}

aodv_rt_entry::~aodv_rt_entry()
{
}

bool
aodv_rt_entry::pc_insert(Ipv4Address id)
{
  if (! pc_lookup(id))
    {
      rt_pclist.push_back(id);
      return true;
    }
  else return false;
}

bool
aodv_rt_entry::pc_lookup(Ipv4Address id)
{
  for(std::vector<Ipv4Address>::const_iterator i = rt_pclist.begin(); i != rt_pclist.end(); ++i)
    if (*i == id) return true;
  return false;
}

bool
aodv_rt_entry::pc_delete(Ipv4Address id)
{
  std::vector<Ipv4Address>::iterator i = std::remove(rt_pclist.begin(), rt_pclist.end(), id);
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

void
aodv_rt_entry::Down ()
{
  if(rt_flags == RTF_DOWN) return;

  rt_last_hop_count = rt_hops;
  rt_hops = INFINITY2;
  rt_flags = RTF_DOWN;
  rt_lifetime = Seconds (DELETE_PERIOD);
}

void
aodv_rt_entry::Print(std::ostream & os) const
{
  os << m_ipv4Route->GetDestination() << "\t" << m_ipv4Route->GetGateway() << "\t"
     << m_ipv4Route->GetSource() << "\t" << rt_lifetime << "\n";
}

/*
  The Routing Table
 */

bool
aodv_rtable::rt_lookup(Ipv4Address id, aodv_rt_entry & rt) const
{
  std::map<Ipv4Address, aodv_rt_entry>::const_iterator i = rthead.find(id);
  if (i == rthead.end()) return false;
  rt = (*i).second;
  return true;
}

bool
aodv_rtable::rt_delete(Ipv4Address dst)
{
  if(rthead.erase (dst) != 0) return true;
  return false;
}

bool
aodv_rtable::rt_add(aodv_rt_entry const & rt)
{
  Ptr<NetDevice> dev;
  aodv_rt_entry dummy(dev);
  if(rt_lookup(rt.GetDestination(), dummy))
  	return false;
  rthead.insert(std::make_pair(rt.GetDestination(), rt));
  return true;
}

bool
aodv_rtable::Update(Ipv4Address dst, aodv_rt_entry & rt)
{
  std::map<Ipv4Address, aodv_rt_entry>::iterator i = rthead.find(dst);
  if (i == rthead.end()) return false;
  i->second = rt;
  return true;
}

void
aodv_rtable::SetEntryState (Ipv4Address id, uint8_t state)
{
  std::map<Ipv4Address, aodv_rt_entry>::iterator i = rthead.find(id);
  if (i != rthead.end()) (*i).second.rt_flags = state;
}

void
aodv_rtable::Print(std::ostream &os) const
{
  os << "\nAODV Routing table\n"
     << "Destination\tGateway\t\tInterface\tExpire\n";
  for (std::map<Ipv4Address, aodv_rt_entry>::const_iterator i = rthead.begin(); i != rthead.end(); ++i)
    {
      i->second.Print(os);
    }
  os << "\n";

}

#if 0

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
  aodv_rt_entry entry1;
  Ipv4Address dst1("3.3.3.3");
  Ipv4Address dst2("1.2.3.4");
  aodv_rt_entry entry2 = aodv_rt_entry(dst2, true, 34, Ipv4Address("2.3.4.5"), 1, Ipv4Address("5.5.5.5"),  Seconds(5));
  NS_TEST_ASSERT( !(entry1 == dst1) );
  NS_TEST_ASSERT(entry2 == dst2);
  
  entry2.pc_insert(dst1);
  NS_TEST_ASSERT(entry2.pc_lookup(dst1));
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
  rtable.Print(std::cout);
  entry2.SetLifeTime(Seconds(10));
  NS_TEST_ASSERT(rtable.Update(entry2.GetDst(), entry2));
  NS_TEST_ASSERT(rtable.rt_lookup(dst2, entry1));
  NS_TEST_ASSERT(entry1.GetLifeTime() == entry2.GetLifeTime());
  NS_TEST_ASSERT(entry1 == dst2);
  NS_TEST_ASSERT(!rtable.rt_lookup(dst1, entry1));
  NS_TEST_ASSERT(rtable.rt_delete(dst2));
  NS_TEST_ASSERT(!rtable.rt_delete(dst2));
  NS_TEST_ASSERT(!rtable.rt_lookup(dst2, entry1));
  
  return result;
}
#endif

#endif

}}
