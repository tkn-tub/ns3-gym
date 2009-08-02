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
#include "ns3/simulator.h"
#include "ns3/test.h"
#include "ns3/log.h"


namespace ns3 {
namespace aodv {

/*
  The Routing Table
 */

RoutingTableEntry::RoutingTableEntry(Ptr<NetDevice> dev, Ipv4Address dst, bool vSeqNo, u_int32_t seqNo, Ipv4InterfaceAddress iface, u_int16_t  hops,
                              Ipv4Address nextHop, Time lifetime)
                            : m_validSeqNo(vSeqNo), m_seqNo(seqNo), m_hops(hops), m_lifeTime(lifetime + Simulator::Now()), m_iface(iface),
                              m_reqCount(0), m_blackListState(false), m_blackListTimeout(Simulator::Now()),
                              m_ackTimer(Timer::CANCEL_ON_DESTROY), lifeTimeTimer(Timer::CANCEL_ON_DESTROY)
{
  m_ipv4Route = Create<Ipv4Route> ();
  m_ipv4Route->SetDestination (dst);
  m_ipv4Route->SetGateway (nextHop);
  m_ipv4Route->SetSource (m_iface.GetLocal ());
  m_ipv4Route->SetOutputDevice (dev);
  m_flag = RTF_UP;
}

RoutingTableEntry::~RoutingTableEntry()
{
}

bool
RoutingTableEntry::InsertPrecursor(Ipv4Address id)
{
  if (! LookupPrecursor(id))
    {
      m_precursorList.push_back(id);
      return true;
    }
  else return false;
}

bool
RoutingTableEntry::LookupPrecursor(Ipv4Address id)
{
  for(std::vector<Ipv4Address>::const_iterator i = m_precursorList.begin(); i != m_precursorList.end(); ++i)
    if (*i == id) return true;
  return false;
}

bool
RoutingTableEntry::DeletePrecursor(Ipv4Address id)
{
  std::vector<Ipv4Address>::iterator i = std::remove(m_precursorList.begin(), m_precursorList.end(), id);
  if(i == m_precursorList.end())
    return false;
  else
    m_precursorList.erase (i, m_precursorList.end());
  return true;
}

void
RoutingTableEntry::DeleteAllPrecursors()
{
  m_precursorList.clear();
  NS_ASSERT(m_precursorList.empty ());
}

bool
RoutingTableEntry::IsPrecursorListEmpty() const
{
  return m_precursorList.empty();
}

void
RoutingTableEntry::GetPrecursors(std::vector<Ipv4Address> & prec) const
{
  if (IsPrecursorListEmpty()) return;
  for(std::vector<Ipv4Address>::const_iterator i = m_precursorList.begin(); i != m_precursorList.end(); ++i)
  {
    bool result = true;
    for(std::vector<Ipv4Address>::const_iterator j = prec.begin(); j != prec.end(); ++j)
      if (*j == *i) result = false;
    if(result) prec.push_back(*i);
  }
}

void
RoutingTableEntry::Invalidate (Time badLinkLifetime)
{
  if(m_flag == RTF_DOWN) return;
  m_flag = RTF_DOWN;
  m_lifeTime = badLinkLifetime + Simulator::Now();
}

void
RoutingTableEntry::Print(std::ostream & os) const
{
  os << m_ipv4Route->GetDestination() << "\t" << m_ipv4Route->GetGateway() << "\t"
     << m_ipv4Route->GetSource() << "\t";
  switch (m_flag)
  {
    case RTF_UP:
    {
      os << "UP";
      break;
    }
    case RTF_DOWN:
    {
      os << "DOWN";
      break;
    }
    case RTF_IN_REPAIR:
    {
      os << "IN_REPAIR";
      break;
    }
    case RTF_IN_SEARCH:
    {
      os << "IN_SEARCH";
      break;
    }
  }
  os << "\t" << (m_lifeTime - Simulator::Now()).GetSeconds() << "\n";
}

#ifdef RUN_SELF_TESTS
/// Unit test for AODV routing table entry
struct AodvRtableEntryTest : public Test
{
  AodvRtableEntryTest () : Test ("AODV/RtableEntry"), result(true) {}
  virtual bool RunTests();
  bool result;
};

/// Test instance
static AodvRtableEntryTest g_AodvRtableEntryTest;

bool
AodvRtableEntryTest::RunTests ()
{
  Ptr<NetDevice> dev;
  Ipv4InterfaceAddress iface;
  RoutingTableEntry rt (/*output device*/dev, /*dst*/Ipv4Address("1.2.3.4"), /*validSeqNo*/true, /*seqNo*/10,
                           /*interface*/iface, /*hop*/5, /*next hop*/Ipv4Address("3.3.3.3"), /*lifetime*/Seconds(10));
  NS_TEST_ASSERT_EQUAL (rt.GetOutputDevice (), dev);
  NS_TEST_ASSERT_EQUAL (rt.GetDestination (), Ipv4Address("1.2.3.4"));
  NS_TEST_ASSERT_EQUAL (rt.GetValidSeqNo (), true);
  NS_TEST_ASSERT_EQUAL (rt.GetSeqNo (), 10);
  NS_TEST_ASSERT_EQUAL (rt.GetInterface(), iface);
  NS_TEST_ASSERT_EQUAL (rt.GetHop(), 5);
  NS_TEST_ASSERT_EQUAL (rt.GetNextHop (), Ipv4Address ("3.3.3.3"));
  NS_TEST_ASSERT_EQUAL (rt.GetLifeTime (), Seconds (10));
  NS_TEST_ASSERT_EQUAL (rt.GetFlag (), RTF_UP);
  NS_TEST_ASSERT_EQUAL (rt.GetRreqCnt(), 0);
  NS_TEST_ASSERT_EQUAL (rt.IsPrecursorListEmpty(), true);

  rt.SetFlag(RTF_DOWN);
  NS_TEST_ASSERT_EQUAL (rt.GetFlag (), RTF_DOWN);
  rt.SetFlag(RTF_IN_REPAIR);
  NS_TEST_ASSERT_EQUAL (rt.GetFlag (), RTF_IN_REPAIR);
  rt.SetHop(12);
  NS_TEST_ASSERT_EQUAL (rt.GetHop (), 12);
  rt.SetLifeTime(Seconds(1));
  NS_TEST_ASSERT_EQUAL (rt.GetLifeTime (), Seconds(1));
  rt.SetNextHop(Ipv4Address("1.1.1.1"));
  NS_TEST_ASSERT_EQUAL (rt.GetNextHop (), Ipv4Address ("1.1.1.1"));
  rt.SetUnidirectional(true);
  NS_TEST_ASSERT_EQUAL (rt.IsUnidirectional (), true);
  rt.SetBalcklistTimeout (Seconds(7));
  NS_TEST_ASSERT_EQUAL (rt.GetBlacklistTimeout(), Seconds (7));
  rt.SetRreqCnt(2);
  NS_TEST_ASSERT_EQUAL (rt.GetRreqCnt(), 2);
  rt.IncrementRreqCnt();
  NS_TEST_ASSERT_EQUAL (rt.GetRreqCnt(), 3);
  rt.Invalidate(Seconds(13));
  NS_TEST_ASSERT_EQUAL (rt.GetFlag (), RTF_DOWN);
  NS_TEST_ASSERT_EQUAL (rt.GetLifeTime (), Seconds (13));
  rt.SetLifeTime(Seconds(0.1));
  NS_TEST_ASSERT_EQUAL (rt.GetLifeTime (), Seconds (0.1));
  Ptr<Ipv4Route> route = rt.GetRoute();
  NS_TEST_ASSERT_EQUAL (route->GetDestination(), Ipv4Address("1.2.3.4"));

  NS_TEST_ASSERT_EQUAL (rt.InsertPrecursor(Ipv4Address("10.0.0.1")), true);
  NS_TEST_ASSERT_EQUAL (rt.IsPrecursorListEmpty(), false);
  NS_TEST_ASSERT_EQUAL (rt.InsertPrecursor(Ipv4Address("10.0.0.2")), true);
  NS_TEST_ASSERT_EQUAL (rt.InsertPrecursor(Ipv4Address("10.0.0.2")), false);
  NS_TEST_ASSERT_EQUAL (rt.LookupPrecursor(Ipv4Address("10.0.0.3")), false);
  NS_TEST_ASSERT_EQUAL (rt.LookupPrecursor(Ipv4Address("10.0.0.1")), true);
  NS_TEST_ASSERT_EQUAL (rt.DeletePrecursor(Ipv4Address("10.0.0.2")), true);
  NS_TEST_ASSERT_EQUAL (rt.LookupPrecursor(Ipv4Address("10.0.0.2")), false);
  std::vector<Ipv4Address>  prec;
  rt.GetPrecursors(prec);
  NS_TEST_ASSERT_EQUAL (prec.size(), 1);
  NS_TEST_ASSERT_EQUAL (rt.InsertPrecursor(Ipv4Address("10.0.0.4")), true);
  rt.GetPrecursors(prec);
  NS_TEST_ASSERT_EQUAL (prec.size(), 2);
  rt.DeleteAllPrecursors();
  NS_TEST_ASSERT_EQUAL (rt.IsPrecursorListEmpty(), true);
  rt.GetPrecursors(prec);
  NS_TEST_ASSERT_EQUAL (prec.size (), 2);


  return result;
}
#endif

/*
  The Routing Table
 */

bool
RoutingTable::LookupRoute(Ipv4Address id, RoutingTableEntry & rt)
{
  Purge ();
  std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.find(id);
  if (i == m_ipv4AddressEntry.end()) return false;
  rt = (*i).second;
  return true;
}

bool
RoutingTable::DeleteRoute(Ipv4Address dst)
{
  if(m_ipv4AddressEntry.erase (dst) != 0) return true;
  return false;
}

bool
RoutingTable::AddRoute(RoutingTableEntry & rt)
{
  Purge ();
  if(rt.GetFlag() != RTF_IN_SEARCH) rt.SetRreqCnt (0);
  m_ipv4AddressEntry.insert(std::make_pair(rt.GetDestination(), rt));
  return true;
}

bool
RoutingTable::Update(Ipv4Address dst, RoutingTableEntry & rt)
{
  Purge ();
  std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.find(dst);
  if (i == m_ipv4AddressEntry.end()) return false;
  i->second = rt;
  if(i->second.GetFlag() != RTF_IN_SEARCH) i->second.SetRreqCnt (0);
  return true;
}

void
RoutingTable::SetEntryState (Ipv4Address id, uint8_t state)
{
  std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.find(id);
  if (i != m_ipv4AddressEntry.end()) (*i).second.SetFlag(state);
}

void
RoutingTable::GetListOfDestinationWithNextHop(Ipv4Address nextHop, std::map<Ipv4Address, uint32_t> unreachable)
{
  unreachable.clear();
  for(std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end(); ++i)
    if ( (i->second.GetNextHop() == nextHop) && (i->second.GetFlag() == RTF_UP)  && (!i->second.IsPrecursorListEmpty()) )
      unreachable.insert(std::make_pair(i->first, i->second.GetSeqNo()));
}

void
RoutingTable::InvalidateRoutesWithDst(const std::map<Ipv4Address, uint32_t> & unreachable, Time badLinkLifetime)
{
  Purge ();
  for(std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end(); ++i)
  {
    for(std::map<Ipv4Address, uint32_t>::const_iterator j =  unreachable.begin(); j != unreachable.end(); ++j)
      if ( (i->first == j->first) && (i->second.GetFlag() == RTF_UP))
        i->second.Invalidate(badLinkLifetime);
  }

}

void
RoutingTable::Purge()
{
  if(m_ipv4AddressEntry.empty ()) return;
  for(std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end(); ++i)
  {
    if(i->second.GetLifeTime() < Seconds(0))
    {
      if(i->second.GetFlag() == RTF_DOWN)
      {
        std::map<Ipv4Address, RoutingTableEntry>::iterator tmp = i;
        m_ipv4AddressEntry.erase(tmp);
      }
      else if (i->second.GetFlag() == RTF_UP)
      {
        NS_LOG_UNCOND("invalidate routing entry to " << i->second.GetDestination());
        i->second.Invalidate(m_badLinkLifetime);
      }
    }
  }
}

bool
RoutingTable::MarkLinkAsUinidirectional (Ipv4Address neighbor, Time blacklistTimeout)
{
  std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.find(neighbor);
  if (i == m_ipv4AddressEntry.end()) return false;
  i->second.SetUnidirectional (true);
  i->second.SetBalcklistTimeout(blacklistTimeout);
  return true;
}

void
RoutingTable::Print(std::ostream &os) const
{
  os << "\nAODV Routing table\n"
     << "Destination\tGateway\t\tInterface\tFlag\tExpire\n";
  for (std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.begin(); i != m_ipv4AddressEntry.end(); ++i)
    {
      i->second.Print(os);
    }
  os << "\n";

}

#if 0

#ifdef RUN_SELF_TESTS
/// Unit test for AODV routing table
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
  RoutingTableEntry entry1;
  Ipv4Address dst1("3.3.3.3");
  Ipv4Address dst2("1.2.3.4");
  RoutingTableEntry entry2 = RoutingTableEntry(dst2, true, 34, Ipv4Address("2.3.4.5"), 1, Ipv4Address("5.5.5.5"),  Seconds(5));
  NS_TEST_ASSERT( !(entry1 == dst1) );
  NS_TEST_ASSERT(entry2 == dst2);
  
  entry2.InsertPrecursor(dst1);
  NS_TEST_ASSERT(entry2.LookupPrecursor(dst1));
  NS_TEST_ASSERT(!entry2.DeletePrecursor(dst2));
  NS_TEST_ASSERT(entry2.DeletePrecursor(dst1));
  NS_TEST_ASSERT(entry2.pc_empty());
  entry2.InsertPrecursor(dst2);
  NS_TEST_ASSERT(entry2.InsertPrecursor(dst1));
  NS_TEST_ASSERT(!entry2.InsertPrecursor(dst2));
  entry2.DeleteAllPrecursors();
  NS_TEST_ASSERT(entry2.pc_empty());
  
  RoutingTable rtable;
  rtable.AddRoute(entry2);
  rtable.Print(std::cout);
  entry2.SetLifeTime(Seconds(10));
  NS_TEST_ASSERT(rtable.Update(entry2.GetDst(), entry2));
  NS_TEST_ASSERT(rtable.LookupRoute(dst2, entry1));
  NS_TEST_ASSERT(entry1.GetLifeTime() == entry2.GetLifeTime());
  NS_TEST_ASSERT(entry1 == dst2);
  NS_TEST_ASSERT(!rtable.LookupRoute(dst1, entry1));
  NS_TEST_ASSERT(rtable.DeleteRoute(dst2));
  NS_TEST_ASSERT(!rtable.DeleteRoute(dst2));
  NS_TEST_ASSERT(!rtable.LookupRoute(dst2, entry1));
  
  return result;
}
#endif

#endif

}}
