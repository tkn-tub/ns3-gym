/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
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
 * Author:  Craig Dowell (craigdo@ee.washington.edu)
 */

#include "ns3/debug.h"
#include "ns3/assert.h"
#include "candidate-queue.h"

NS_DEBUG_COMPONENT_DEFINE ("CandidateQueue");

namespace ns3 {

CandidateQueue::CandidateQueue()
  : m_candidates ()
{
  NS_DEBUG("CandidateQueue::CandidateQueue ()");
}

CandidateQueue::~CandidateQueue()
{
  NS_DEBUG("CandidateQueue::~CandidateQueue ()");
  Clear ();
}

  void
CandidateQueue::Clear (void)
{
  NS_DEBUG("CandidateQueue::Clear ()");

  while (!m_candidates.empty ())
    {
      SPFVertex *p = Pop ();
      delete p;
      p = 0;
    }
}

  void
CandidateQueue::Push (SPFVertex *vNew)
{
  NS_DEBUG("CandidateQueue::Push (" << vNew << ")");

  CandidateList_t::iterator i = m_candidates.begin ();  

  for (; i != m_candidates.end (); i++)
    {
      SPFVertex *v = *i;
      if (vNew->GetDistanceFromRoot () < v->GetDistanceFromRoot ())
        {
          break;
        }
    }
  m_candidates.insert(i, vNew);
}

  SPFVertex *
CandidateQueue::Pop (void)
{
  NS_DEBUG("CandidateQueue::Pop ()");

  if (m_candidates.empty ())
    {
      return 0;
    }

  SPFVertex *v = m_candidates.front ();
  m_candidates.pop_front ();
  return v;
}

  SPFVertex *
CandidateQueue::Top (void) const
{
  NS_DEBUG("CandidateQueue::Top ()");

  if (m_candidates.empty ())
    {
      return 0;
    }

  return m_candidates.front ();
}

  bool
CandidateQueue::Empty (void) const
{
  NS_DEBUG("CandidateQueue::Empty ()");

  return m_candidates.empty ();
}

  uint32_t
CandidateQueue::Size (void) const
{
  NS_DEBUG("CandidateQueue::Size ()");

  return m_candidates.size ();
}

  SPFVertex *
CandidateQueue::Find (const Ipv4Address addr) const
{
  NS_DEBUG("CandidateQueue::Find ()");

  CandidateList_t::const_iterator i = m_candidates.begin ();

  for (; i != m_candidates.end (); i++)
    {
      SPFVertex *v = *i;
      if (v->GetVertexId() == addr)
        {
          return v;
        }
    }

  return 0;
}

  void
CandidateQueue::Reorder (void)
{
  NS_DEBUG("CandidateQueue::Reorder ()");

  std::list<SPFVertex*> temp;

  while (!m_candidates.empty ()) {
    SPFVertex *v = m_candidates.front ();
    m_candidates.pop_front ();
    temp.push_back(v);
  }

  while (!temp.empty ()) {
    Push (temp.front ());
    temp.pop_front ();
  }
}

} // namespace ns3
