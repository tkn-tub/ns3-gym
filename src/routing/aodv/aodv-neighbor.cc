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

#include "aodv-neighbor.h"
#include "ns3/test.h"

namespace ns3
{
namespace aodv
{

bool
Neighbors::Lookup (Ipv4Address addr)
{
  Purge ();
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr)
      {
        return true;
      }
  return false;
}

bool
Neighbors::IsNeighbor (Ipv4Address addr )
{
  Purge ();
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr)
      return true;
  return false;
}

Time
Neighbors::GetExpireTime (Ipv4Address addr)
{
  Purge ();
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr)
      return (i->m_expireTime - Simulator::Now () );
  return Seconds(0);
}


void
Neighbors::Update (Ipv4Address addr, Time expire )
{
  Purge ();
  for (std::vector<Neighbor>::iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr)
      {
        i->m_expireTime = expire + Simulator::Now ();
        return;
      }
  struct Neighbor neighbor =
  { addr, expire + Simulator::Now () };
  m_nb.push_back (neighbor);
}

void
Neighbors::Purge ()
{
  if (m_nb.empty ()) return;
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_expireTime < Simulator::Now ())
      {
        m_handleLinleFailure (i->m_neighborAddress);
      }
  std::vector<Neighbor>::iterator i = remove_if (m_nb.begin (), m_nb.end (), IsExpired ());
  m_nb.erase (i, m_nb.end ());

}

#ifdef RUN_SELF_TESTS
/// Unit test for neighbors
struct NeighborTest : public Test
{
  NeighborTest () : Test ("AODV/Neighbor"), neighbor(MakeCallback(&NeighborTest::Handler, this)), result(true) {}
  virtual bool RunTests();
  void Handler (Ipv4Address addr);
  void CheckTimeout1 ();
  void CheckTimeout2 ();
  void CheckTimeout3 ();
  Neighbors neighbor;
  bool result;
};

/// Test instance
static NeighborTest g_NeighborTest;

void
NeighborTest::Handler (Ipv4Address addr)
{
}

void
NeighborTest::CheckTimeout1 ()
{
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("1.2.3.4")), true);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("1.1.1.1")), true);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("2.2.2.2")), true);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("3.3.3.3")), true);
}
void
NeighborTest::CheckTimeout2 ()
{
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("1.2.3.4")), false);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("1.1.1.1")), false);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("2.2.2.2")), false);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("3.3.3.3")), true);
}
void
NeighborTest::CheckTimeout3 ()
{
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("1.2.3.4")), false);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("1.1.1.1")), false);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("2.2.2.2")), false);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("3.3.3.3")), false);
}

bool
NeighborTest::RunTests ()
{
  neighbor.Update (Ipv4Address("1.2.3.4"), Seconds(1));
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("1.2.3.4")), true);
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("4.3.2.1")), false);
  neighbor.Update (Ipv4Address("1.2.3.4"), Seconds(10));
  NS_TEST_ASSERT_EQUAL (neighbor.Lookup(Ipv4Address("1.2.3.4")), true);
  NS_TEST_ASSERT_EQUAL (neighbor.GetExpireTime (Ipv4Address("1.2.3.4")), Seconds(10));
  NS_TEST_ASSERT_EQUAL (neighbor.GetExpireTime (Ipv4Address("4.3.2.1")), Seconds(0));
  neighbor.Update (Ipv4Address("1.1.1.1"), Seconds(5));
  neighbor.Update (Ipv4Address("2.2.2.2"), Seconds(10));
  neighbor.Update (Ipv4Address("3.3.3.3"), Seconds(20));

  Simulator::Schedule (Seconds(1), &NeighborTest::CheckTimeout1, this);
  Simulator::Schedule (Seconds(15), &NeighborTest::CheckTimeout2, this);
  Simulator::Schedule (Seconds(30), &NeighborTest::CheckTimeout3, this);
  Simulator::Run ();
  Simulator::Destroy ();
  return result;
}


#endif
}
}

