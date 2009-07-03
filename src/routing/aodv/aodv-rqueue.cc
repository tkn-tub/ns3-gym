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

#include "aodv-rqueue.h"
#include "ns3/simulator.h"
#include "ns3/test.h"
#include <algorithm>

namespace ns3 {
namespace aodv {

aodv_rqueue::aodv_rqueue() : limit_(AODV_RTQ_MAX_LEN), timeout_(Seconds(AODV_RTQ_TIMEOUT))
{
}

uint32_t
aodv_rqueue::size ()
{
  purge();
  return queue.size();
}

void
aodv_rqueue::enque(QueueEntry & entry) 
{
  // Purge any packets that have timed out.
  purge();
  entry.enExpire = Simulator::Now() + timeout_;

  if (queue.size() == limit_) drop(remove_head()); // drop the most aged packet
  queue.push_back(entry);
}

QueueEntry
aodv_rqueue::deque() 
{
  purge();
  return remove_head();
}

bool
aodv_rqueue::deque(Ipv4Address dst, QueueEntry & entry)
{
  purge();
  for(std::vector<QueueEntry>::iterator i = queue.begin(); i != queue.end(); ++i)
    if(i->header.GetDestination() == dst)
      {
        entry = *i;
        queue.erase(i);
        return true;
      }
  return false;
}

bool
aodv_rqueue::find(Ipv4Address dst)
{
  for( std::vector<QueueEntry>::const_iterator i = queue.begin(); i != queue.end(); ++i)
    if(i->header.GetDestination() == dst)
      return true;
  return false;
}

QueueEntry
aodv_rqueue::remove_head() 
{
  QueueEntry entry = queue.front();
  queue.erase(queue.begin());  
  return entry;
}

struct IsExpired
{
  bool operator() (QueueEntry const & e) const
  {
    return (e.enExpire < Simulator::Now());
  }
};

void
aodv_rqueue::purge()
{
  std::vector<QueueEntry>::iterator i = std::remove_if(queue.begin(), queue.end(), IsExpired());
  for (std::vector<QueueEntry>::iterator j = i ; j < queue.end(); ++j)
    drop (*j); 
  queue.erase(i, queue.end());
}

void 
aodv_rqueue::drop(QueueEntry)
{
  // TODO do nothing now.
}

#ifdef RUN_SELF_TESTS
/// Unit test for aodv_rqueue
struct AodvRqueueTest : public Test 
{
  AodvRqueueTest () : Test ("AODV/Rqueue"), result(true) {}
  virtual bool RunTests();
  
  void CheckSizeLimit ();
  void CheckTimeout ();
  
  aodv_rqueue q;
  bool result;
};

/// Test instance
static AodvRqueueTest g_AodvRqueueTest;

bool 
AodvRqueueTest::RunTests ()
{
  Ptr<Packet> packet = Create<Packet>();
  Ipv4Header header;
  QueueEntry e1 (packet, header);
  q.enque (e1);
  QueueEntry e2 = q.deque ();
  NS_TEST_ASSERT (e1 == e2);
  
  Ipv4Address dst("1.2.3.4");
  header.SetDestination (dst);
  e1 = QueueEntry (packet, header);
  q.enque (e1);
  
  bool ok = q.deque (dst, e2);
  NS_TEST_ASSERT (ok);
  NS_TEST_ASSERT (e1 == e2);
  NS_TEST_ASSERT (! q.find(dst));
  q.enque (e1);
  NS_TEST_ASSERT (q.find(dst));
  
  CheckSizeLimit ();
  
  Simulator::Schedule (Seconds(AODV_RTQ_TIMEOUT+1), & AodvRqueueTest::CheckTimeout, this);
  
  Simulator::Run ();
  Simulator::Destroy ();
  
  return result;
}

void 
AodvRqueueTest::CheckSizeLimit ()
{
  Ptr<Packet> packet = Create<Packet>();
  Ipv4Header header;
  QueueEntry e1 (packet, header);
  
  for (uint32_t i = 0; i < AODV_RTQ_MAX_LEN; ++i)
    q.enque (e1);
  NS_TEST_ASSERT (q.size() == AODV_RTQ_MAX_LEN);
  
  for (uint32_t i = 0; i < AODV_RTQ_MAX_LEN; ++i)
      q.enque (e1);
  NS_TEST_ASSERT (q.size() == AODV_RTQ_MAX_LEN);
}

void 
AodvRqueueTest::CheckTimeout ()
{
  NS_TEST_ASSERT (q.size() == 0);
}
#endif
}}
