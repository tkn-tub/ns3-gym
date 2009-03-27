/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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


#include "ns3/object.h"
#include "ns3/assert.h"
#include "ns3/simulator.h"
#include "hwmp-rtable.h"

namespace ns3 {
namespace dot11s {

NS_OBJECT_ENSURE_REGISTERED (HwmpRtable);

TypeId
HwmpRtable::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::HwmpRtable")
    .SetParent<Object> ()
    .AddConstructor<HwmpRtable> ();
  return tid;
}
HwmpRtable::HwmpRtable ()
{
  DeleteProactivePath ();
}
HwmpRtable::~HwmpRtable ()
{
  DoDispose ();
}
void
HwmpRtable::DoDispose ()
{
  m_routes.clear ();
}
void
HwmpRtable::AddReactivePath (
  Mac48Address destination,
  Mac48Address retransmitter,
  uint32_t interface,
  uint32_t metric,
  Time  lifetime,
  uint32_t seqnum
)
{
  std::map<Mac48Address, ReactiveRoute>::iterator i = m_routes.find (destination);
  if (i == m_routes.end ())
    {
      ReactiveRoute newroute;
      m_routes[destination] = newroute;
    }
  else
    {
      /**
       * if outinterface differs from stored, routing info is
       * actual and metric is worse - we ignore this
       * information
       */
      if (
        (i->second.interface != interface) &&
        (i->second.metric < metric) &&
        /**
         * The routing info is actual or it
         * was received from peer
         */
        ((i->second.whenExpire > Simulator::Now ())||(i->second.whenExpire == Seconds(0)))
      )
        return;
    }
  i = m_routes.find (destination);
  NS_ASSERT (i != m_routes.end());
  i->second.retransmitter = retransmitter;
  i->second.interface = interface;
  i->second.metric = metric;
  if (lifetime != Seconds (0))
    i->second.whenExpire = MilliSeconds (Simulator::Now().GetMilliSeconds() + lifetime.GetMilliSeconds());
  else
    /**
     * Information about peer does not have lifetime
     */
    i->second.whenExpire = Seconds (0);
  i->second.seqnum = seqnum;
}
void
HwmpRtable::AddProactivePath (
  uint32_t metric,
  Mac48Address root,
  Mac48Address retransmitter,
  uint32_t interface,
  Time  lifetime,
  uint32_t seqnum
)
{
  m_root.root = root;
  m_root.retransmitter = retransmitter;
  m_root.metric = metric;
  m_root.whenExpire = MilliSeconds (Simulator::Now().GetMilliSeconds() + lifetime.GetMilliSeconds());
  m_root.seqnum = seqnum;
  m_root.interface = interface;
}
void
HwmpRtable::AddPrecursor (Mac48Address destination, uint32_t interface, Mac48Address precursor)
{
  std::map<Mac48Address, ReactiveRoute>::iterator i = m_routes.find (destination);
  if ((i != m_routes.end ()) && (i->second.interface == interface))
    {
      bool should_add = true;
      for (unsigned int j = 0 ; j < i->second.precursors.size (); j ++)
        if (i->second.precursors[j] == precursor)
          {
            should_add = false;
            break;
          }
      if (should_add)
        i->second.precursors.push_back (precursor);
    }
  if((m_root.root == destination) && (m_root.interface == interface))
    for (unsigned int j = 0 ; j < m_root.precursors.size (); j ++)
      if (m_root.precursors[j] == precursor)
        return;
  m_root.precursors.push_back(precursor);
}
void
HwmpRtable::DeleteProactivePath ()
{
  m_root.precursors.clear ();
  m_root.interface = INTERFACE_ANY;
  m_root.metric = MAX_METRIC;
  m_root.retransmitter = Mac48Address::GetBroadcast ();
  m_root.seqnum = 0;
  m_root.whenExpire = Simulator::Now ();
}
void
HwmpRtable::DeleteReactivePath (Mac48Address destination, uint32_t interface)
{
  std::map<Mac48Address, ReactiveRoute>::iterator i = m_routes.find (destination);
  if (i != m_routes.end ())
    if (i->second.interface ==  interface)
      m_routes.erase (i);
}
HwmpRtable::LookupResult
HwmpRtable::LookupReactive (Mac48Address destination)
{
  LookupResult result;
  result.retransmitter = Mac48Address::GetBroadcast ();
  result.metric = MAX_METRIC;
  result.ifIndex = INTERFACE_ANY;

  std::map<Mac48Address, ReactiveRoute>::iterator i = m_routes.find (destination);
  if (i == m_routes.end ())
    return result;
  result.ifIndex = i->second.interface;
  //Seconds (0) means that this is routing
  if (i->second.whenExpire < Simulator::Now ())
    if (i->second.retransmitter != destination)
      return result;
  result.retransmitter = i->second.retransmitter;
  result.metric = i->second.metric;
  result.seqnum = i->second.seqnum;
  return result;
}
HwmpRtable::LookupResult
HwmpRtable::LookupProactive ()
{
  if (m_root.whenExpire < Simulator::Now ())
    DeleteProactivePath ();
  LookupResult retval;
  retval.retransmitter = m_root.retransmitter;
  retval.ifIndex = m_root.interface;
  retval.metric = m_root.metric;
  retval.seqnum = m_root.seqnum;
  return retval;
}
std::vector<IePerr::FailedDestination>
HwmpRtable::GetUnreachableDestinations (Mac48Address peerAddress, uint32_t interface)
{
  std::vector<IePerr::FailedDestination> retval;
  for (std::map<Mac48Address, ReactiveRoute>::iterator i = m_routes.begin (); i != m_routes.end(); i++)
    if ((i->second.retransmitter == peerAddress)&& (i->second.interface == interface))
      {
        IePerr::FailedDestination dst;
        dst.destination = i->first;
        i->second.seqnum ++;
        dst.seqnum = i->second.seqnum;
        retval.push_back (dst);
      }
  /**
   * Lookup a path to root
   */
  if (m_root.retransmitter == peerAddress)
    {
      IePerr::FailedDestination dst;
      dst.destination = m_root.root;
      dst.seqnum = m_root.seqnum;
      retval.push_back (dst);
    }
  return retval;
}
uint32_t
HwmpRtable::RequestSeqnum (Mac48Address destination)
{
  std::map<Mac48Address, ReactiveRoute>::iterator i = m_routes.find (destination);
  if (i == m_routes.end ())
    return 0;
  return i->second.seqnum;
}
std::vector<Mac48Address>
HwmpRtable::GetPrecursors (Mac48Address destination, uint32_t interface)
{
  std::vector<Mac48Address> retval;
  if (m_root.root == destination)
      for (unsigned int i = 0; i < m_root.precursors.size (); i ++)
        retval.push_back (m_root.precursors[i]);
  std::map<Mac48Address, ReactiveRoute>::iterator route = m_routes.find (destination);
  if ( (route != m_routes.end ()) && (route->second.interface == interface) )
      for (unsigned int i = 0; i < route->second.precursors.size (); i ++)
        retval.push_back (route->second.precursors[i]);
  return retval;
}
} //namespace dot11s
} //namespace ns3
