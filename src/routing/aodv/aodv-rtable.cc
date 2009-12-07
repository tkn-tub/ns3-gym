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
 * Authors: Elena Buchatskaia <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#include "aodv-rtable.h"
#include <algorithm>
#include "ns3/simulator.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("AodvRoutingTable");

namespace ns3
{
namespace aodv
{

/*
 The Routing Table
 */

RoutingTableEntry::RoutingTableEntry (Ptr<NetDevice> dev, Ipv4Address dst, bool vSeqNo, uint32_t seqNo,
                                      Ipv4InterfaceAddress iface, uint16_t hops, Ipv4Address nextHop, Time lifetime) :
                                      m_ackTimer (Timer::CANCEL_ON_DESTROY),
                                      m_validSeqNo (vSeqNo), m_seqNo (seqNo), m_hops (hops),
                                      m_lifeTime (lifetime + Simulator::Now ()), m_iface (iface), m_flag (VALID),
                                      m_reqCount (0), m_blackListState (false), m_blackListTimeout (Simulator::Now ())
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
RoutingTableEntry::InsertPrecursor (Ipv4Address id)
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
RoutingTableEntry::LookupPrecursor (Ipv4Address id)
{
  for (std::vector<Ipv4Address>::const_iterator i = m_precursorList.begin (); i
      != m_precursorList.end (); ++i)
    {
      if (*i == id)
        return true;
    }
  return false;
}

bool
RoutingTableEntry::DeletePrecursor (Ipv4Address id)
{
  std::vector<Ipv4Address>::iterator i = std::remove (m_precursorList.begin (),
      m_precursorList.end (), id);
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
}

bool
RoutingTableEntry::IsPrecursorListEmpty () const
{
  return m_precursorList.empty ();
}

void
RoutingTableEntry::GetPrecursors (std::vector<Ipv4Address> & prec) const
{
  if (IsPrecursorListEmpty ())
    return;
  for (std::vector<Ipv4Address>::const_iterator i = m_precursorList.begin (); i
      != m_precursorList.end (); ++i)
    {
      bool result = true;
      for (std::vector<Ipv4Address>::const_iterator j = prec.begin (); j
          != prec.end (); ++j)
        {
          if (*j == *i)
            result = false;
        }
      if (result)
        prec.push_back (*i);
    }
}

void
RoutingTableEntry::Invalidate (Time badLinkLifetime)
{
  if (m_flag == INVALID)
    return;
  m_flag = INVALID;
  m_reqCount = 0;
  m_lifeTime = badLinkLifetime + Simulator::Now ();
}

void
RoutingTableEntry::Print (std::ostream & os) const
{
  os << m_ipv4Route->GetDestination () << "\t" << m_ipv4Route->GetGateway ()
      << "\t" << m_iface.GetLocal () << "\t";
  switch (m_flag)
    {
    case VALID:
      {
        os << "UP";
        break;
      }
    case INVALID:
      {
        os << "DOWN";
        break;
      }
    case IN_SEARCH:
      {
        os << "IN_SEARCH";
        break;
      }
    }
  os << "\t" << (m_lifeTime - Simulator::Now ()).GetSeconds () << "\t"
      << m_hops << "\n";
}

/*
 The Routing Table
 */

RoutingTable::RoutingTable (Time t) : 
  m_badLinkLifetime (t)
{
}

bool
RoutingTable::LookupRoute (Ipv4Address id, RoutingTableEntry & rt)
{
  Purge ();
  if (m_ipv4AddressEntry.empty ())
    return false;
  std::map<Ipv4Address, RoutingTableEntry>::const_iterator i =
      m_ipv4AddressEntry.find (id);
  if (i == m_ipv4AddressEntry.end ())
    return false;
  rt = i->second;
  return true;
}

bool
RoutingTable::DeleteRoute (Ipv4Address dst)
{
  Purge ();
  if (m_ipv4AddressEntry.erase (dst) != 0)
    return true;
  return false;
}

bool
RoutingTable::AddRoute (RoutingTableEntry & rt)
{
  if (rt.GetFlag () != IN_SEARCH)
    rt.SetRreqCnt (0);
  std::pair<std::map<Ipv4Address, RoutingTableEntry>::iterator, bool> result =
      m_ipv4AddressEntry.insert (std::make_pair (rt.GetDestination (), rt));
  return result.second;
}

bool
RoutingTable::Update (RoutingTableEntry & rt)
{
  std::map<Ipv4Address, RoutingTableEntry>::iterator i =
      m_ipv4AddressEntry.find (rt.GetDestination ());
  if (i == m_ipv4AddressEntry.end ())
    return false;
  i->second = rt;
  if (i->second.GetFlag () != IN_SEARCH)
    i->second.SetRreqCnt (0);
  return true;
}

bool
RoutingTable::SetEntryState (Ipv4Address id, RouteFlags state)
{
  std::map<Ipv4Address, RoutingTableEntry>::iterator i =
      m_ipv4AddressEntry.find (id);
  if (i == m_ipv4AddressEntry.end ())
    return false;
  i->second.SetFlag (state);
  i->second.SetRreqCnt (0);
  return true;
}

void
RoutingTable::GetListOfDestinationWithNextHop (Ipv4Address nextHop, std::map<Ipv4Address, uint32_t> & unreachable )
{
  Purge ();
  unreachable.clear ();
  for (std::map<Ipv4Address, RoutingTableEntry>::const_iterator i =
      m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end (); ++i)
    {
      if (i->second.GetNextHop () == nextHop)
        {
          unreachable.insert (std::make_pair (i->first, i->second.GetSeqNo ()));
        }
    }
}

void
RoutingTable::InvalidateRoutesWithDst (const std::map<Ipv4Address, uint32_t> & unreachable)
{
  Purge ();
  for (std::map<Ipv4Address, RoutingTableEntry>::iterator i =
      m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end (); ++i)
    {
      for (std::map<Ipv4Address, uint32_t>::const_iterator j =
          unreachable.begin (); j != unreachable.end (); ++j)
        {
          if ((i->first == j->first) && (i->second.GetFlag () == VALID))
            {
              NS_LOG_LOGIC ("Invalidate route with destination address " << i->first);
              i->second.Invalidate (m_badLinkLifetime);
            }
        }
    }
}

void
RoutingTable::DeleteAllRoutesFromInterface (Ipv4InterfaceAddress iface)
{
  if (m_ipv4AddressEntry.empty ())
    return;
  for (std::map<Ipv4Address, RoutingTableEntry>::iterator i =
      m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end ();)
    {
      if (i->second.GetInterface () == iface)
        {
          std::map<Ipv4Address, RoutingTableEntry>::iterator tmp = i;
          ++i;
          m_ipv4AddressEntry.erase (tmp);
        }
      else
        ++i;
    }
}

void
RoutingTable::Purge ()
{
  if (m_ipv4AddressEntry.empty ())
    return;
  for (std::map<Ipv4Address, RoutingTableEntry>::iterator i =
      m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end ();)
    {
      if (i->second.GetLifeTime () < Seconds (0))
        {
          if (i->second.GetFlag () == INVALID)
            {
              std::map<Ipv4Address, RoutingTableEntry>::iterator tmp = i;
              ++i;
              m_ipv4AddressEntry.erase (tmp);
            }
          else if (i->second.GetFlag () == VALID)
            {
              NS_LOG_LOGIC ("Invalidate route with destination address " << i->first);
              i->second.Invalidate (m_badLinkLifetime);
              ++i;
            }
          else
            ++i;
        }
      else 
        {
          ++i;
        }
    }
}

bool
RoutingTable::MarkLinkAsUnidirectional (Ipv4Address neighbor, Time blacklistTimeout)
{
  std::map<Ipv4Address, RoutingTableEntry>::iterator i =
      m_ipv4AddressEntry.find (neighbor);
  if (i == m_ipv4AddressEntry.end ())
    return false;
  i->second.SetUnidirectional (true);
  i->second.SetBalcklistTimeout (blacklistTimeout);
  i->second.SetRreqCnt (0);
  return true;
}

void
RoutingTable::Print (std::ostream &os)
{
  Purge ();
  os << "\nAODV Routing table\n"
      << "Destination\tGateway\t\tInterface\tFlag\tExpire\tHops\n";
  for (std::map<Ipv4Address, RoutingTableEntry>::const_iterator i =
      m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end (); ++i)
    {
      i->second.Print (os);
    }
  os << "\n";
}

}
}
