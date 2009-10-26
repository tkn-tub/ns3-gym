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
#include "id-cache.h"
#include "ns3/test.h"
#include <algorithm>

namespace ns3
{
namespace aodv
{
void
IdCache::InsertId (Ipv4Address addr, uint32_t id, Time saveTime)
{
  if (LookupId (addr, id))
    return;
  struct UniqueId uniqueId =
  { addr, id, saveTime + Simulator::Now () };
  m_idCache.push_back (uniqueId);
}
bool
IdCache::LookupId (Ipv4Address addr, uint32_t id)
{
  Purge ();
  for (std::vector<UniqueId>::const_iterator i = m_idCache.begin ();
      i != m_idCache.end (); ++i)
    if (i->m_context == addr && i->m_id == id)
      return true;
  return false;
}
void
IdCache::Purge ()
{
  m_idCache.erase (remove_if (m_idCache.begin (), m_idCache.end (),
      IsExpired ()), m_idCache.end ());
}

uint32_t
IdCache::GetSize ()
{
  Purge ();
  return m_idCache.size ();
}

//-----------------------------------------------------------------------------
// Tests
//-----------------------------------------------------------------------------
/// Unit test for id cache
struct IdCacheTest : public TestCase
{
  IdCacheTest () : TestCase ("Id Cache")
  {}
  virtual bool DoRun();
  void CheckTimeout1 ();
  void CheckTimeout2 ();
  void CheckTimeout3 ();

  IdCache cache;
};

bool
IdCacheTest::DoRun ()
{
  cache.InsertId (Ipv4Address ("1.2.3.4"), 3, Seconds(5));
  NS_TEST_EXPECT_MSG_EQ (cache.LookupId (Ipv4Address ("1.2.3.4"), 4), false, "Unknown ID");
  NS_TEST_EXPECT_MSG_EQ (cache.LookupId (Ipv4Address ("4.3.2.1"), 3), false, "Unknown address");
  NS_TEST_EXPECT_MSG_EQ (cache.LookupId (Ipv4Address ("1.2.3.4"), 3), true, "Known address & ID");
  cache.InsertId (Ipv4Address ("1.1.1.1"), 4, Seconds(5));
  cache.InsertId (Ipv4Address ("1.1.1.1"), 4, Seconds(5));
  cache.InsertId (Ipv4Address ("2.2.2.2"), 5, Seconds(16));
  cache.InsertId (Ipv4Address ("3.3.3.3"), 6, Seconds(27));
  NS_TEST_EXPECT_MSG_EQ (cache.GetSize (), 4, "trivial");

  Simulator::Schedule (Seconds(1), &IdCacheTest::CheckTimeout1, this);
  Simulator::Schedule (Seconds(6), &IdCacheTest::CheckTimeout2, this);
  Simulator::Schedule (Seconds(30), &IdCacheTest::CheckTimeout3, this);
  Simulator::Run ();
  Simulator::Destroy ();
  
  return GetErrorStatus ();
}

void
IdCacheTest::CheckTimeout1 ()
{
  NS_TEST_EXPECT_MSG_EQ (cache.GetSize (), 4, "Nothing expire");
}

void
IdCacheTest::CheckTimeout2 ()
{
  NS_TEST_EXPECT_MSG_EQ (cache.GetSize (), 2, "2 records left");
}

void
IdCacheTest::CheckTimeout3 ()
{
  NS_TEST_EXPECT_MSG_EQ (cache.GetSize (), 0, "All records expire");
}
//-----------------------------------------------------------------------------
class IdCacheTestSuite : public TestSuite
{
public:
  IdCacheTestSuite () : TestSuite ("routing-id-cache", UNIT)
  {
    AddTestCase (new IdCacheTest);
  }
} g_idCacheTestSuite;

}}
