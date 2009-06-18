/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */
#include "ns3/assert.h"
#include "ns3/simulator.h"
#include "ns3/test.h"
#include "ns3/log.h"

#include "flame-rtable.h"
namespace ns3 {
namespace flame {

NS_LOG_COMPONENT_DEFINE ("FlameRtable");
  
NS_OBJECT_ENSURE_REGISTERED (FlameRtable);

TypeId
FlameRtable::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::flame::FlameRtable")
    .SetParent<Object> ()
    .AddConstructor<FlameRtable> ()
    .AddAttribute ("lifetime", "The lifetime of the routing enrty",
        TimeValue (Seconds (120)),
        MakeTimeAccessor (&FlameRtable::m_lifetime),
        MakeTimeChecker ()
        );
  return tid;
}

FlameRtable::FlameRtable ()
  : m_lifetime (Seconds (120))
{}

FlameRtable::~FlameRtable ()
{
}

void
FlameRtable::DoDispose ()
{
  m_routes.clear ();
}

void
FlameRtable::AddPath (
  const Mac48Address destination,
  const Mac48Address retransmitter,
  const uint32_t interface,
  const uint8_t cost,
  const uint16_t seqnum
)
{
  std::map<Mac48Address, Route>::iterator i = m_routes.find (destination);
  if (i == m_routes.end ())
    {
      Route newroute;
      newroute.cost = cost;
      newroute.retransmitter = retransmitter;
      newroute.interface = interface;
      newroute.whenExpire = Simulator::Now() + m_lifetime;
      newroute.seqnum = seqnum;
      m_routes[destination] = newroute;
      return;
    }
  i->second.seqnum = seqnum;
  NS_ASSERT (i != m_routes.end());
  if (i->second.cost < cost)
    return;
  i->second.retransmitter = retransmitter;
  i->second.interface = interface;
  i->second.cost = cost;
  i->second.whenExpire = Simulator::Now() + m_lifetime;
}
FlameRtable::LookupResult
FlameRtable::Lookup (Mac48Address destination)
{
  std::map<Mac48Address, Route>::iterator i = m_routes.find (destination);
  if (i == m_routes.end ())
    return LookupResult ();
  if ((i->second.whenExpire < Simulator::Now ()))
    {
      NS_LOG_DEBUG ("Route has expired, sorry.");
      m_routes.erase (i);
      return LookupResult();
    }
  return LookupResult (i->second.retransmitter, i->second.interface, i->second.cost, i->second.seqnum);
}
bool FlameRtable::LookupResult::operator==(const FlameRtable::LookupResult & o) const
{
  return (retransmitter == o.retransmitter
      && ifIndex == o.ifIndex 
      && cost  == o.cost
      && seqnum  == o.seqnum
    );
}

bool FlameRtable::LookupResult::IsValid() const
{
  return !( retransmitter == Mac48Address::GetBroadcast ()
        &&  ifIndex == INTERFACE_ANY
        &&  cost == MAX_COST
        &&  seqnum == 0
      );
}


#ifdef RUN_SELF_TESTS
/// Unit test for FlameRtable
class FlameRtableTest : public Test 
{
public:
  FlameRtableTest ();
  virtual bool RunTests(); 
  
private:
  /// Test Add apth and lookup path;
  void TestLookup ();
  /**
   * \name Test add path and try to lookup after entry has expired
   * \{
   */
  void TestAddPath ();
  void TestExpire ();
  ///\}
private:
  bool result;
  
  Mac48Address dst;
  Mac48Address hop;
  uint32_t iface;
  uint8_t cost;
  uint16_t seqnum;
  Ptr<FlameRtable> table;
};

/// Test instance
static FlameRtableTest g_FlameRtableTest;

FlameRtableTest::FlameRtableTest ()  : Test ("Mesh/flame/FlameRtable"), 
  result(true),
  dst ("01:00:00:01:00:01"),
  hop ("01:00:00:01:00:03"),
  iface (8010),
  cost (10),
  seqnum (1)
{
}

void FlameRtableTest::TestLookup ()
{
  FlameRtable::LookupResult correct (hop, iface, cost, seqnum);
  
  table->AddPath (dst, hop, iface, cost, seqnum);
  NS_TEST_ASSERT (table->Lookup (dst) == correct);
}

void FlameRtableTest::TestAddPath ()
{
  table->AddPath (dst, hop, iface, cost, seqnum);
}

void FlameRtableTest::TestExpire ()
{
  // this is assumed to be called when path records are already expired
  FlameRtable::LookupResult correct (hop, iface, cost, seqnum);
  NS_TEST_ASSERT (! table->Lookup (dst).IsValid ());
}
bool FlameRtableTest::RunTests ()
{
  table = CreateObject<FlameRtable> ();
  
  Simulator::Schedule (Seconds (0), & FlameRtableTest::TestLookup, this);
  Simulator::Schedule (Seconds (1), & FlameRtableTest::TestAddPath, this);
  Simulator::Schedule (Seconds (122), & FlameRtableTest::TestExpire, this);
  
  Simulator::Run ();
  Simulator::Destroy ();
  
  return result;
}

#endif // RUN_SELF_TESTS

} //namespace flame
} //namespace ns3
