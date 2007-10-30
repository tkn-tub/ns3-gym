/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2004 Francisco J. Ros 
 * Copyright (c) 2007 INESC Porto
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
 * Authors: Francisco J. Ros  <fjrm@dif.um.es>
 *          Gustavo J. A. M. Carneiro <gjc@inescporto.pt>
 */

///
/// \file	OLSR_rtable.cc
/// \brief	Implementation of our routing table.
///

#include "routing-table.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/log.h"

namespace ns3 { namespace olsr {

NS_LOG_COMPONENT_DEFINE ("OlsrRoutingTable");

///
/// \brief Clears the routing table and frees the memory assigned to each one of its entries.
///
void
RoutingTable::Clear ()
{
  m_table.clear ();
}

///
/// \brief Deletes the entry whose destination address is given.
/// \param dest	address of the destination node.
///
void
RoutingTable::RemoveEntry (Ipv4Address const &dest)
{
  m_table.erase (dest);
}

///
/// \brief Looks up an entry for the specified destination address.
/// \param dest	destination address.
/// \param outEntry output parameter to hold the routing entry result, if fuond
/// \return	true if found, false if not found
///
bool
RoutingTable::Lookup (Ipv4Address const &dest,
                      RoutingTableEntry &outEntry) const
{
  // Get the iterator at "dest" position
  std::map<Ipv4Address, RoutingTableEntry>::const_iterator it =
    m_table.find (dest);
  // If there is no route to "dest", return NULL
  if (it == m_table.end ())
    return false;
  outEntry = it->second;
  return true;
}

///
/// \brief	Finds the appropiate entry which must be used in order to forward
///		a data packet to a next hop (given a destination).
///
/// Imagine a routing table like this: [A,B] [B,C] [C,C]; being each pair of the
/// form [dest addr,next-hop addr]. In this case, if this function is invoked with
/// [A,B] then pair [C,C] is returned because C is the next hop that must be used
/// to forward a data packet destined to A. That is, C is a neighbor of this node,
/// but B isn't. This function finds the appropiate neighbor for forwarding a packet.
///
/// \param entry	the routing table entry which indicates the destination node
///			we are interested in.
/// \return		the appropiate routing table entry which indicates the next
///			hop which must be used for forwarding a data packet, or NULL
///			if there is no such entry.
///
bool
RoutingTable::FindSendEntry (RoutingTableEntry const &entry,
                             RoutingTableEntry &outEntry) const
{
  outEntry = entry;
  while (outEntry.destAddr != outEntry.nextAddr)
    {
      if (not Lookup(outEntry.nextAddr, outEntry))
        return false;
    }
  return true;
}


bool
RoutingTable::RequestRoute (uint32_t ifIndex,
                            const Ipv4Header &ipHeader,
                            Packet packet,
                            RouteReplyCallback routeReply)
{
  RoutingTableEntry entry1, entry2;
  if (Lookup (ipHeader.GetDestination (), entry1))
    {
      bool foundSendEntry = FindSendEntry (entry1, entry2);
      if (!foundSendEntry)
        NS_FATAL_ERROR ("FindSendEntry failure");

      Ipv4Route route = Ipv4Route::CreateHostRouteTo
        (ipHeader.GetDestination (), entry2.nextAddr, entry2.interface);

      NS_LOG_DEBUG ("Olsr node" << m_mainAddress
                    << ": RouteRequest for dest=" << ipHeader.GetDestination ()
                    << " --> destHop=" << entry2.nextAddr
                    << " interface=" << entry2.interface);
      
      routeReply (true, route, packet, ipHeader);
      return true;
    }
  else
    {
      NS_LOG_DEBUG ("Olsr node" << m_mainAddress
                    << ": RouteRequest for dest=" << ipHeader.GetDestination ()
                    << " --> NOT FOUND");
      return false;
    }
}

bool
RoutingTable::RequestIfIndex (Ipv4Address destination,
                              uint32_t& ifIndex)
{
  RoutingTableEntry entry1, entry2;
  if (Lookup (destination, entry1))
    {
      bool foundSendEntry = FindSendEntry (entry1, entry2);
      if (!foundSendEntry)
        NS_FATAL_ERROR ("FindSendEntry failure");
      ifIndex = entry2.interface;
      return true;
    }
  else
    {
      return false;
    }
}


///
/// \brief Adds a new entry into the routing table.
///
/// If an entry for the given destination existed, it is deleted and freed.
///
/// \param dest		address of the destination node.
/// \param next		address of the next hop node.
/// \param iface	address of the local interface.
/// \param dist		distance to the destination node.
///
void
RoutingTable::AddEntry (Ipv4Address const &dest,
                        Ipv4Address const &next,
                        uint32_t interface,
                        uint32_t distance)
{
  // Creates a new rt entry with specified values
  RoutingTableEntry &entry = m_table[dest];

  entry.destAddr = dest;
  entry.nextAddr = next;
  entry.interface = interface;
  entry.distance = distance;
}

void
RoutingTable::AddEntry (Ipv4Address const &dest,
                        Ipv4Address const &next,
                        Ipv4Address const &interfaceAddress,
                        uint32_t distance)
{
  RoutingTableEntry entry;
  NS_ASSERT (m_ipv4);
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
    {
      if (m_ipv4->GetAddress (i) == interfaceAddress)
        {
          AddEntry (dest, next, i, distance);
          return;
        }
    }
  NS_ASSERT (false); // should not be reached
  AddEntry (dest, next, 0, distance);
}


///
/// \brief Returns the number of entries in the routing table.
/// \return the number of entries in the routing table.
///
// u_int32_t
// RoutingTable::size() {
// 	return rt_.size();
// }

}}; // namespace ns3, olsr



#ifdef RUN_SELF_TESTS


#include "ns3/test.h"


namespace ns3 { namespace olsr {

class OlsrRoutingTableTest : public ns3::Test {
private:
public:
  OlsrRoutingTableTest ();
  virtual bool RunTests (void);


};

OlsrRoutingTableTest::OlsrRoutingTableTest ()
  : ns3::Test ("OlsrRoutingTable")
{}


bool 
OlsrRoutingTableTest::RunTests (void)
{
  bool result = true;

  RoutingTable table;

  table.AddEntry (Ipv4Address ("1.2.3.5"),
                  Ipv4Address ("1.2.3.4"),
                  0,
                  1);

  table.AddEntry (Ipv4Address ("1.2.3.4"),
                  Ipv4Address ("1.2.3.4"),
                  0,
                  1);
  
  RoutingTableEntry entry1;
  NS_TEST_ASSERT (table.Lookup (Ipv4Address ("1.2.3.5"), entry1));
  NS_TEST_ASSERT_EQUAL (entry1.destAddr, Ipv4Address ("1.2.3.5"));
  NS_TEST_ASSERT_EQUAL (entry1.nextAddr, Ipv4Address ("1.2.3.4"));
  NS_TEST_ASSERT_EQUAL (entry1.interface, 0);
  NS_TEST_ASSERT_EQUAL (entry1.distance, 1);

  RoutingTableEntry entry2;
  NS_TEST_ASSERT (table.Lookup (Ipv4Address ("1.2.3.4"), entry2));
  NS_TEST_ASSERT_EQUAL (entry2.destAddr, Ipv4Address ("1.2.3.4"));
  NS_TEST_ASSERT_EQUAL (entry2.nextAddr, Ipv4Address ("1.2.3.4"));
  NS_TEST_ASSERT_EQUAL (entry2.interface, 0);
  NS_TEST_ASSERT_EQUAL (entry2.distance, 1);

  RoutingTableEntry sendEntry;
  NS_TEST_ASSERT (table.FindSendEntry (entry1, sendEntry));
  NS_TEST_ASSERT_EQUAL (sendEntry.destAddr, Ipv4Address ("1.2.3.4"));
  NS_TEST_ASSERT_EQUAL (sendEntry.nextAddr, Ipv4Address ("1.2.3.4"));
  NS_TEST_ASSERT_EQUAL (sendEntry.interface, 0);
  NS_TEST_ASSERT_EQUAL (sendEntry.distance, 1);

  table.RemoveEntry (Ipv4Address ("1.2.3.5"));
  RoutingTableEntry removedEntry;
  NS_TEST_ASSERT (not table.Lookup (Ipv4Address ("1.2.3.5"), removedEntry));

  return result;
}

static OlsrRoutingTableTest gOlsrRoutingTableTest;

}}; // namespace


#endif /* RUN_SELF_TESTS */
