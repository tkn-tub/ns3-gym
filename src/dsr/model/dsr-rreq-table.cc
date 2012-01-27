/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Yufei Cheng
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
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#include "dsr-rreq-table.h"
#include "ns3/log.h"
#include <algorithm>
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("RreqTable");

namespace ns3 {
namespace dsr {

NS_OBJECT_ENSURE_REGISTERED (RreqTable);

TypeId RreqTable::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dsr::RreqTable")
    .SetParent<Object> ()
    .AddConstructor<RreqTable> ()
  ;
  return tid;
}

RreqTable::RreqTable ()
  : m_linkStates (PROBABLE)
{
}

RreqTable::~RreqTable ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
RreqTable::RemoveLeastExpire (std::map<Ipv4Address, RreqTableEntry > & rreqDstMap)
{
  NS_LOG_FUNCTION (this);
  Ipv4Address firstExpire;
  Time max = Seconds (0.0);
  for (std::map<Ipv4Address, RreqTableEntry >::const_iterator i =
         rreqDstMap.begin (); i != rreqDstMap.end (); ++i)
    {
      Ipv4Address dst = i->first;
      RreqTableEntry rreqTableEntry = i->second;
      if (rreqTableEntry.m_expire > max)
        {
          max = rreqTableEntry.m_expire;
          firstExpire = dst;
        }
    }
  rreqDstMap.erase (firstExpire);
}

void
RreqTable::FindAndUpdate (Ipv4Address dst)
{
  NS_LOG_LOGIC ("Find and update the route request entry for " << dst);
  std::map<Ipv4Address, RreqTableEntry >::const_iterator i =
    m_rreqDstMap.find (dst);
  if (i == m_rreqDstMap.end ())
    {
      NS_LOG_DEBUG ("The request table entry not found");
      /*
       * Drop the most aged packet when buffer reaches to max
       */
      if (m_rreqDstMap.size () >= m_requestTableSize)
        {
          RemoveLeastExpire (m_rreqDstMap);
          NS_LOG_DEBUG ("The request table size after erase ");
        }
      RreqTableEntry rreqTableEntry;
      rreqTableEntry.m_reqNo = 0;
      rreqTableEntry.m_expire = Simulator::Now ();
      m_rreqDstMap [dst] = rreqTableEntry;
    }
  else
    {
      NS_LOG_DEBUG ("Find the request table entry, increment the request count");
      Ipv4Address dst = i->first;
      RreqTableEntry rreqTableEntry = i->second;
      NS_LOG_DEBUG ("The request count before incrementing " << rreqTableEntry.m_reqNo);
      rreqTableEntry.m_reqNo = (rreqTableEntry.m_reqNo + 1);
      rreqTableEntry.m_expire = Simulator::Now ();
      m_rreqDstMap [dst] = rreqTableEntry;
    }
}

void
RreqTable::RemoveRreqEntry (Ipv4Address dst)
{
  NS_LOG_DEBUG ("Remove rreq entry with index dst");
  std::map<Ipv4Address, RreqTableEntry >::const_iterator i =
    m_rreqDstMap.find (dst);
  if (i == m_rreqDstMap.end ())
    {
      NS_LOG_DEBUG ("The request table entry not found");
    }
  else
    {
      // erase the request entry
      m_rreqDstMap.erase (dst);
    }
}

uint16_t
RreqTable::GetRreqCnt (Ipv4Address dst)
{
  NS_LOG_DEBUG ("Get the request count for a certain dst");
  std::map<Ipv4Address, RreqTableEntry >::const_iterator i =
    m_rreqDstMap.find (dst);
  if (i == m_rreqDstMap.end ())
    {
      NS_LOG_DEBUG ("The request table entry not found");
      FindAndUpdate (dst);
      return 0;
    }
  else
    {
      RreqTableEntry rreqTableEntry = i->second;
      NS_LOG_DEBUG ("Find the request count for " << dst << " " << rreqTableEntry.m_reqNo);
      return rreqTableEntry.m_reqNo;
    }
}

// ----------------------------------------------------------------------------------------------------------
/**
 * This part takes care of the route request from a specific source
 * need to ignore future route requests from same source for same destination with same identification
 */
bool
RreqTable::FindSrc (Ipv4Address source, Ipv4Address target, uint16_t id)
{
  Purge ();
  std::map<Ipv4Address, std::list<SourceRreqEntry> >::const_iterator i =
    m_rreqMap.find (source);
  if (i == m_rreqMap.end ())
    {
      NS_LOG_LOGIC ("No Request entry for " << source << " found");
      SourceRreqEntry sourceRreqEntry;
      sourceRreqEntry.m_dst = target;
      sourceRreqEntry.m_identification = id;
      sourceRreqEntry.m_expire = m_rreqEntryExpire + Simulator::Now ();
      NS_LOG_DEBUG ("The src rreq expire time " << sourceRreqEntry.m_expire);
      std::list<SourceRreqEntry> rqVector;
      rqVector.push_back (sourceRreqEntry);
      m_rreqMap[source] = rqVector;
      return false;
    }
  else
    {
      NS_LOG_LOGIC ("Request entry for " << source << " found in the cache");
      std::list<SourceRreqEntry> rqVector = i->second;
      for (std::list<SourceRreqEntry>::iterator j = rqVector.begin (); j != rqVector.end (); ++j)
        {
          SourceRreqEntry rreqEntry = *j;
          if ((rreqEntry.m_dst == target) && (rreqEntry.m_identification == id))
            {
              NS_LOG_DEBUG ("Found the request entry for source node with address " << source);
//              j = rqVector.erase (j);
//              rqVector.push_back(*j);
//              m_rreqMap[source] = rqVector;
              return true;
            }
        }

      SourceRreqEntry rreqEntry;
      rreqEntry.m_dst = target;
      rreqEntry.m_identification = id;
      rreqEntry.m_expire = m_rreqEntryExpire + Simulator::Now ();
      if (rqVector.size () >= m_requestIdSize)
        {
          // erase the first element when the size is larger than the request id size (default: 16)
          rqVector.pop_front ();
        }
      // May need to check the size of the entry
      rqVector.push_back (rreqEntry);
      m_rreqMap[source] = rqVector;
      return false;
    }
  return false;
}

void
RreqTable::Purge ()
{
  //Trying to purge the rreq table
  if (m_rreqMap.empty ())
    {
      NS_LOG_DEBUG ("The rreq table is empty");
      return;
    }

  for (std::map<Ipv4Address, std::list<SourceRreqEntry> >::iterator i =
         m_rreqMap.begin (); i != m_rreqMap.end (); )
    {
      // Loop of rreq table entry with the source entries
      std::map<Ipv4Address, std::list<SourceRreqEntry> >::iterator itmp = i;
      /*
       * The rreq table entries
       */
      Ipv4Address dst = i->first;
      std::list<SourceRreqEntry> rqVector = i->second;
      NS_LOG_DEBUG ("The rqVector size for " << dst << " is " << rqVector.size ());
      if (rqVector.size ())
        {
          for (std::list<SourceRreqEntry>::iterator j = rqVector.begin (); j != rqVector.end (); )
            {
              NS_LOG_DEBUG ("The expire time of every entry with expire time " << j->m_expire - Simulator::Now ());
              /*
               * First verify if the rreq table entry has expired or not
               */
              if (j->m_expire - Simulator::Now () <= Seconds (0))
                {
                  /*
                   * When the expire time has passed, erase the certain rreq table entry
                   */
                  NS_LOG_DEBUG ("Erase the expired rreq table entry for " << dst << " with expire time " << j->m_expire - Simulator::Now ());
                  j = rqVector.erase (j);
                }
              else
                {
                  ++j;
                }
            }
          NS_LOG_DEBUG ("The rreq table entry for " << dst << " " << rqVector.size ());
          if (rqVector.size ())
            {
              ++i;
              m_rreqMap.erase (itmp); // erase the entry first
              /*
               * Save the new rreq table entry along with the destination address in map
               */
              std::pair<std::map<Ipv4Address, std::list<SourceRreqEntry> >::iterator, bool> result =
                m_rreqMap.insert (std::make_pair (dst, rqVector));
            }
          else
            {
              ++i;
              m_rreqMap.erase (itmp);
            }
        }
      else
        {
          ++i;
          m_rreqMap.erase (itmp);
        }
    }
  return;
}

// ----------------------------------------------------------------------------------------------------------
/**
 * This part takes care of the route request ID initialized from a specific source to one destination
 * Essentially a counter
 */
uint16_t
RreqTable::CheckUniqueRreqId (Ipv4Address dst)
{
  NS_LOG_DEBUG ("The size of id cache " << m_rreqIdCache.size ());
  std::map<Ipv4Address, uint16_t>::const_iterator i =
    m_rreqIdCache.find (dst);
  if (i == m_rreqIdCache.end ())
    {
      NS_LOG_LOGIC ("No Request id for " << dst << " found");
      m_rreqIdCache[dst] = 0;
      return 0;
    }
  else
    {
      NS_LOG_LOGIC ("Request id for " << dst << " found in the cache");
      uint16_t rreqId = m_rreqIdCache[dst];
      if (rreqId >= m_maxRreqId)
        {
          NS_LOG_DEBUG ("The request id increase past the max value, so reset it to 0");
          rreqId = 0;
          m_rreqIdCache[dst] = rreqId;
        }
      else
        {
          rreqId++;
          m_rreqIdCache[dst] = rreqId;
        }
      NS_LOG_DEBUG ("The Request id for " << dst << " is " << rreqId);
      return rreqId;
    }
}

uint16_t
RreqTable::GetRreqSize ()
{
  return m_rreqIdCache.size ();
}

// ----------------------------------------------------------------------------------------------------------
/**
 * This part takes care of black list which can save unidirectional link information
 */

void
RreqTable::Invalidate ()
{
  if (m_linkStates == QUESTIONABLE)
    {
      return;
    }
  m_linkStates = QUESTIONABLE;
}

BlackList*
RreqTable::FindUnidirectional (Ipv4Address neighbor)
{
  PurgeNeighbor ();  // purge the neighbor cache
  for (std::vector<BlackList>::iterator i = m_blackList.begin ();
       i != m_blackList.end (); ++i)
    {
      if (i->m_neighborAddress == neighbor)
        {
          return &(*i);
        }
    }
  return NULL;
}

bool
RreqTable::MarkLinkAsUnidirectional (Ipv4Address neighbor, Time blacklistTimeout)
{
  NS_LOG_LOGIC ("Add neighbor address in blacklist " << m_blackList.size ());
  for (std::vector<BlackList>::iterator i = m_blackList.begin (); i != m_blackList.end (); ++i)
    {
      if (i->m_neighborAddress == neighbor)
        {
          NS_LOG_DEBUG ("Update the blacklist list timeout if found the blacklist entry");
          i->m_expireTime = std::max (blacklistTimeout + Simulator::Now (), i->m_expireTime);
        }
      BlackList blackList (neighbor, blacklistTimeout + Simulator::Now ());
      m_blackList.push_back (blackList);
      PurgeNeighbor ();
      return true;
    }
  return false;
}

void
RreqTable::PurgeNeighbor ()
{
  /*
   * Purge the expired blacklist entries
   */
  m_blackList.erase (remove_if (m_blackList.begin (), m_blackList.end (),
                                IsExpired ()), m_blackList.end ());
}

} // namespace dsr
} // namespace ns3
