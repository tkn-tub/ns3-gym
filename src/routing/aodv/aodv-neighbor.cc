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
#include "ns3/log.h"
#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("AodvNeighbors");

namespace ns3
{
namespace aodv
{
Neighbors::Neighbors (Time delay) : m_ntimer (Timer::CANCEL_ON_DESTROY)
{
  m_ntimer.SetDelay(delay);
  m_ntimer.SetFunction(&Neighbors::Purge, this);
  m_txErrorCallback = MakeCallback (& Neighbors::ProcessTxError, this);
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
  for (std::vector<Neighbor>::iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr)
      {
        i->m_expireTime =  std::max(expire + Simulator::Now (), i->m_expireTime);
        return;
      }

  // Lookup mac address
  Mac48Address hwaddr;
  for (std::vector<Ptr<ArpCache> >::const_iterator i = m_arp.begin(); i != m_arp.end(); ++i)
    {
      ArpCache::Entry * entry = (*i)->Lookup (addr);
      if (entry != 0 && entry->IsAlive () && ! entry->IsExpired ())
        {
          hwaddr = Mac48Address::ConvertFrom(entry->GetMacAddress ());
          break;
        }
    }
  
  Neighbor neighbor (addr, hwaddr, expire + Simulator::Now ());
  m_nb.push_back (neighbor);
  Purge ();
}

void
Neighbors::Purge ()
{
  if (m_nb.empty ()) return;
  std::vector<Neighbor>::iterator i = remove_if (m_nb.begin (), m_nb.end (), IsExpired ());
  if (!m_handleLinleFailure.IsNull ())
    {
      for (std::vector<Neighbor>::const_iterator j = i; j != m_nb.end (); ++j)
        {
          m_handleLinleFailure (i->m_neighborAddress);
        }
    }
  m_nb.erase (i, m_nb.end ());
  m_ntimer.Cancel();
  m_ntimer.Schedule();
}

void
Neighbors::ScheduleTimer ()
{
  m_ntimer.Cancel();
  m_ntimer.Schedule();
}

void
Neighbors::AddArpCache (Ptr<ArpCache> a)
{
  m_arp.push_back(a);
}

void
Neighbors::DelArpCache (Ptr<ArpCache> a)
{
  m_arp.erase(std::remove(m_arp.begin(), m_arp.end(), a), m_arp.end());
}


void
Neighbors::ProcessTxError (WifiMacHeader const & hdr)
{
  Mac48Address addr = hdr.GetAddr1();
  
  for (std::vector<Neighbor>::iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_hardwareAddress == addr)
      {
        NS_LOG_LOGIC ("Close link to " << i->m_neighborAddress << " because of layer 2 TX error notification");
        i->m_expireTime = Simulator::Now();
      }  
  Purge();
}

#ifdef RUN_SELF_TESTS
/// Unit test for neighbors
struct NeighborTest : public Test
{
  NeighborTest () : Test ("AODV/Neighbor"), neighbor(0),  result(true) { }
  virtual bool RunTests();
  void Handler (Ipv4Address addr);
  void CheckTimeout1 ();
  void CheckTimeout2 ();
  void CheckTimeout3 ();
  Neighbors * neighbor;
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
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("1.2.3.4")), true);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("1.1.1.1")), true);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("2.2.2.2")), true);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("3.3.3.3")), true);
}
void
NeighborTest::CheckTimeout2 ()
{
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("1.2.3.4")), false);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("1.1.1.1")), false);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("2.2.2.2")), false);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("3.3.3.3")), true);
}
void
NeighborTest::CheckTimeout3 ()
{
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("1.2.3.4")), false);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("1.1.1.1")), false);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("2.2.2.2")), false);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("3.3.3.3")), false);
}

bool
NeighborTest::RunTests ()
{
  Neighbors nb (Seconds (1));
  neighbor = &nb;
  neighbor->SetCallback(MakeCallback(&NeighborTest::Handler, this));
  neighbor->Update (Ipv4Address("1.2.3.4"), Seconds(1));
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("1.2.3.4")), true);
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("4.3.2.1")), false);
  neighbor->Update (Ipv4Address("1.2.3.4"), Seconds(10));
  NS_TEST_ASSERT_EQUAL (neighbor->IsNeighbor(Ipv4Address("1.2.3.4")), true);
  NS_TEST_ASSERT_EQUAL (neighbor->GetExpireTime (Ipv4Address("1.2.3.4")), Seconds(10));
  NS_TEST_ASSERT_EQUAL (neighbor->GetExpireTime (Ipv4Address("4.3.2.1")), Seconds(0));
  neighbor->Update (Ipv4Address("1.1.1.1"), Seconds(5));
  neighbor->Update (Ipv4Address("2.2.2.2"), Seconds(10));
  neighbor->Update (Ipv4Address("3.3.3.3"), Seconds(20));

  Simulator::Schedule (Seconds(2), &NeighborTest::CheckTimeout1, this);
  Simulator::Schedule (Seconds(15), &NeighborTest::CheckTimeout2, this);
  Simulator::Schedule (Seconds(30), &NeighborTest::CheckTimeout3, this);
  Simulator::Run ();
  Simulator::Destroy ();
  return result;
}


#endif
}
}

