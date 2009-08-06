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

NS_LOG_COMPONENT_DEFINE ("AodvRoutingTable");

namespace ns3
{
namespace aodv
{

/*
 The Routing Table
 */

RoutingTableEntry::RoutingTableEntry (Ptr<NetDevice> dev, Ipv4Address dst, bool vSeqNo, u_int32_t seqNo, Ipv4InterfaceAddress iface, u_int16_t hops,
    Ipv4Address nextHop, Time lifetime ) : m_validSeqNo (vSeqNo), m_seqNo (seqNo), m_hops (hops), m_lifeTime (lifetime + Simulator::Now ()),
                                           m_iface (iface), m_flag (RTF_UP), m_reqCount (0), m_blackListState (false), m_blackListTimeout (Simulator::Now ()),
                                           m_ackTimer (Timer::CANCEL_ON_DESTROY)
{
  m_ipv4Route = Create<Ipv4Route> ();
  m_ipv4Route->SetDestination (dst);
  m_ipv4Route->SetGateway (nextHop);
  m_ipv4Route->SetSource (m_iface.GetLocal ());
  m_ipv4Route->SetOutputDevice (dev);
}

RoutingTableEntry::~RoutingTableEntry ()
{
}

bool
RoutingTableEntry::InsertPrecursor (Ipv4Address id )
{
  if (!LookupPrecursor (id))
    {
      m_precursorList.push_back (id);
      return true;
    }
  else
    return false;
}

bool
RoutingTableEntry::LookupPrecursor (Ipv4Address id )
{
  for (std::vector<Ipv4Address>::const_iterator i = m_precursorList.begin (); i != m_precursorList.end (); ++i)
    if (*i == id)
      return true;
  return false;
}

bool
RoutingTableEntry::DeletePrecursor (Ipv4Address id )
{
  std::vector<Ipv4Address>::iterator i = std::remove (m_precursorList.begin (), m_precursorList.end (), id);
  if (i == m_precursorList.end ())
    return false;
  else
    m_precursorList.erase (i, m_precursorList.end ());
  return true;
}

void
RoutingTableEntry::DeleteAllPrecursors ()
{
  m_precursorList.clear ();
  NS_ASSERT(m_precursorList.empty ());
}

bool
RoutingTableEntry::IsPrecursorListEmpty () const
{
  return m_precursorList.empty ();
}

void
RoutingTableEntry::GetPrecursors (std::vector<Ipv4Address> & prec ) const
{
  if (IsPrecursorListEmpty ())
    return;
  for (std::vector<Ipv4Address>::const_iterator i = m_precursorList.begin (); i != m_precursorList.end (); ++i)
    {
      bool result = true;
      for (std::vector<Ipv4Address>::const_iterator j = prec.begin (); j != prec.end (); ++j)
        if (*j == *i)
          result = false;
      if (result)
        prec.push_back (*i);
    }
}

void
RoutingTableEntry::Invalidate (Time badLinkLifetime )
{
  if (m_flag == RTF_DOWN)
    return;
  m_flag = RTF_DOWN;
  m_reqCount = 0;
  m_lifeTime = badLinkLifetime + Simulator::Now ();
}

void
RoutingTableEntry::Print (std::ostream & os ) const
{
  os << m_ipv4Route->GetDestination () << "\t" << m_ipv4Route->GetGateway () << "\t" << m_ipv4Route->GetSource () << "\t";
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
  os << "\t" << (m_lifeTime - Simulator::Now ()).GetSeconds () << "\n";
}

#ifdef RUN_SELF_TESTS
/// Unit test for AODV routing table entry
struct AodvRtableEntryTest : public Test
  {
    AodvRtableEntryTest () : Test ("AODV/RtableEntry"), result(true)
      {}
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

    Ptr<NetDevice> dev2;
    Ipv4InterfaceAddress iface2;
    rt.SetOutputDevice(dev2);
    NS_TEST_ASSERT_EQUAL (rt.GetOutputDevice (), dev2);
    rt.SetInterface(iface2);
    NS_TEST_ASSERT_EQUAL (rt.GetInterface(), iface2);
    rt.SetValidSeqNo(false);
    NS_TEST_ASSERT_EQUAL (rt.GetValidSeqNo (), false);
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
    std::vector<Ipv4Address> prec;
    rt.GetPrecursors(prec);
    NS_TEST_ASSERT_EQUAL (prec.size(), 1);
    NS_TEST_ASSERT_EQUAL (rt.InsertPrecursor(Ipv4Address("10.0.0.4")), true);
    NS_TEST_ASSERT_EQUAL (rt.DeletePrecursor(Ipv4Address("10.0.0.5")), false);
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

RoutingTable::RoutingTable (Time t) : m_badLinkLifetime (t)
{
}


bool
RoutingTable::LookupRoute (Ipv4Address id, RoutingTableEntry & rt )
{
  Purge ();
  if (m_ipv4AddressEntry.empty ())
    return false;
  std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.find (id);
  if (i == m_ipv4AddressEntry.end ())
    return false;
  rt = i->second;
  return true;
}

bool
RoutingTable::DeleteRoute (Ipv4Address dst )
{
  Purge ();
  if (m_ipv4AddressEntry.erase (dst) != 0)
    return true;
  return false;
}

bool
RoutingTable::AddRoute (RoutingTableEntry & rt )
{
  Purge ();
  if (rt.GetFlag () != RTF_IN_SEARCH)
    rt.SetRreqCnt (0);
  std::pair<std::map<Ipv4Address, RoutingTableEntry>::iterator, bool> result = m_ipv4AddressEntry.insert (std::make_pair (rt.GetDestination (), rt));
  return result.second;
}

bool
RoutingTable::Update (RoutingTableEntry & rt )
{
  Purge ();
  std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.find (rt.GetDestination ());
  if (i == m_ipv4AddressEntry.end ())
    return false;
  i->second = rt;
  if (i->second.GetFlag () != RTF_IN_SEARCH)
    i->second.SetRreqCnt (0);
  return true;
}

bool
RoutingTable::SetEntryState (Ipv4Address id, RouteFlags state )
{
  Purge ();
  std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.find (id);
  if (i == m_ipv4AddressEntry.end ())
    return false;
  i->second.SetFlag (state);
  i->second.SetRreqCnt(0);
  return true;
}

void
RoutingTable::GetListOfDestinationWithNextHop (Ipv4Address nextHop, std::map<Ipv4Address, uint32_t> unreachable )
{
  Purge ();
  unreachable.clear ();
  for (std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end (); ++i)
    if ((i->second.GetNextHop () == nextHop) && (i->second.GetFlag () == RTF_UP) && (!i->second.IsPrecursorListEmpty ()))
      unreachable.insert (std::make_pair (i->first, i->second.GetSeqNo ()));
}

void
RoutingTable::InvalidateRoutesWithDst (const std::map<Ipv4Address, uint32_t> & unreachable )
{
  Purge ();
  for (std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end (); ++i)
    {
      for (std::map<Ipv4Address, uint32_t>::const_iterator j = unreachable.begin (); j != unreachable.end (); ++j)
        if ((i->first == j->first) && (i->second.GetFlag () == RTF_UP))
          {
            NS_LOG_LOGIC ("invalidate route with dst " << i->first);
            i->second.Invalidate (m_badLinkLifetime);
          }
    }

}

void
RoutingTable::Purge ()
{
  if (m_ipv4AddressEntry.empty ())
    return;
  for (std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end ();)
    {
      if (i->second.GetLifeTime () < Seconds (0))
        {
          if (i->second.GetFlag () == RTF_DOWN)
            {
              std::map<Ipv4Address, RoutingTableEntry>::iterator tmp = i;
              ++i;
              m_ipv4AddressEntry.erase (tmp);
            }
          else if (i->second.GetFlag () == RTF_UP)
            {
              NS_LOG_LOGIC ("invalidate route with dst " << i->first );
              i->second.Invalidate (m_badLinkLifetime);
              NS_LOG_LOGIC (Simulator::Now().GetSeconds());
              Print(std::cout);
              ++i;
            }
          else
            ++i;
        }
      ++i;
    }
}

bool
RoutingTable::MarkLinkAsUinidirectional (Ipv4Address neighbor, Time blacklistTimeout )
{
  Purge ();
  std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.find (neighbor);
  if (i == m_ipv4AddressEntry.end ())
    return false;
  i->second.SetUnidirectional (true);
  i->second.SetBalcklistTimeout (blacklistTimeout);
  i->second.SetRreqCnt(0);
  return true;
}

void
RoutingTable::Print (std::ostream &os )
{
  Purge ();
  os << "\nAODV Routing table\n" << "Destination\tGateway\t\tInterface\tFlag\tExpire\n";
  for (std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end (); ++i)
    {
      i->second.Print (os);
    }
  os << "\n";

}

#ifdef RUN_SELF_TESTS
/// Unit test for AODV routing table
struct AodvRtableTest : public Test
  {
    AodvRtableTest () : Test ("AODV/Rtable"), result(true)
      {}
    virtual bool RunTests();
    bool result;
  };

/// Test instance
static AodvRtableTest g_AodvRtableTest;

bool
AodvRtableTest::RunTests ()
  {
    RoutingTable rtable (Seconds(2));
    NS_TEST_ASSERT_EQUAL (rtable.GetBadLinkLifetime(), Seconds(2));
    rtable.SetBadLinkLifetime(Seconds(1));
    NS_TEST_ASSERT_EQUAL (rtable.GetBadLinkLifetime(), Seconds(1));
    Ptr<NetDevice> dev;
    Ipv4InterfaceAddress iface;
    RoutingTableEntry rt (/*output device*/dev, /*dst*/Ipv4Address("1.2.3.4"), /*validSeqNo*/true, /*seqNo*/10,
        /*interface*/iface, /*hop*/5, /*next hop*/Ipv4Address("1.1.1.1"), /*lifetime*/Seconds(10));
    NS_TEST_ASSERT_EQUAL (rtable.AddRoute (rt), true);
    NS_TEST_ASSERT_EQUAL (rtable.AddRoute (rt), false);
    RoutingTableEntry rt2 (/*output device*/dev, /*dst*/Ipv4Address("4.3.2.1"), /*validSeqNo*/false, /*seqNo*/0,
        /*interface*/iface, /*hop*/15, /*next hop*/Ipv4Address("1.1.1.1"), /*lifetime*/Seconds(1));
    NS_TEST_ASSERT_EQUAL (rtable.AddRoute (rt2), true);
    NS_TEST_ASSERT_EQUAL (rtable.LookupRoute(rt2.GetDestination(), rt), true);
    NS_TEST_ASSERT_EQUAL (rt2.GetDestination (), rt.GetDestination ());
    rt.SetHop(20);
    rt.InsertPrecursor(Ipv4Address("10.0.0.3"));
    NS_TEST_ASSERT_EQUAL (rtable.Update (rt), true);
    RoutingTableEntry rt3;
    NS_TEST_ASSERT_EQUAL (rtable.LookupRoute(Ipv4Address("10.0.0.1"), rt), false);
    NS_TEST_ASSERT_EQUAL (rtable.Update (rt3), false);
    NS_TEST_ASSERT_EQUAL (rtable.SetEntryState(Ipv4Address("10.0.0.1"), RTF_DOWN), false);
    NS_TEST_ASSERT_EQUAL (rtable.SetEntryState(Ipv4Address("1.2.3.4"), RTF_IN_SEARCH), true);
    NS_TEST_ASSERT_EQUAL (rtable.DeleteRoute(Ipv4Address("5.5.5.5")), false);
    RoutingTableEntry rt4 (/*output device*/dev, /*dst*/Ipv4Address("5.5.5.5"), /*validSeqNo*/false, /*seqNo*/0,
        /*interface*/iface, /*hop*/15, /*next hop*/Ipv4Address("1.1.1.1"), /*lifetime*/Seconds(-10));
    NS_TEST_ASSERT_EQUAL (rtable.AddRoute (rt4), true);
    NS_TEST_ASSERT_EQUAL (rtable.SetEntryState(Ipv4Address("5.5.5.5"), RTF_DOWN), true);
    NS_TEST_ASSERT_EQUAL (rtable.LookupRoute(Ipv4Address("5.5.5.5"), rt), false);

    NS_TEST_ASSERT_EQUAL (rtable.MarkLinkAsUinidirectional(Ipv4Address("1.2.3.4"), Seconds(2)), true);
    NS_TEST_ASSERT_EQUAL (rtable.LookupRoute(Ipv4Address("1.2.3.4"), rt), true);
    NS_TEST_ASSERT_EQUAL (rt.IsUnidirectional(), true);
    rt.SetLifeTime(Seconds(-5));
    NS_TEST_ASSERT_EQUAL (rtable.Update (rt), true);
    std::map<Ipv4Address, uint32_t> unreachable;
    unreachable.insert (std::make_pair(Ipv4Address("4.3.2.1"), 3));
    rtable.InvalidateRoutesWithDst(unreachable);
    NS_TEST_ASSERT_EQUAL (rtable.LookupRoute(Ipv4Address("4.3.2.1"), rt), true);
    NS_TEST_ASSERT_EQUAL (rt.GetFlag(), RTF_DOWN);
    rtable.GetListOfDestinationWithNextHop(Ipv4Address("1.1.1.1"), unreachable);
    NS_TEST_ASSERT_EQUAL (unreachable.size (), 1);
    NS_TEST_ASSERT_EQUAL (rtable.DeleteRoute(Ipv4Address("1.2.3.4")), true);
    NS_TEST_ASSERT_EQUAL (rtable.DeleteRoute(Ipv4Address("1.2.3.4")), false);

    return result;
  }
#endif

}
}
