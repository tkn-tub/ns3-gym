/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
      Pop ();
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
      if (vNew->m_distanceFromRoot < v->m_distanceFromRoot)
        {
          break;
        }
    }
  m_candidates.insert(i, vNew);
}

  void
CandidateQueue::Pop (void)
{
  NS_DEBUG("CandidateQueue::Pop ()");

  if (m_candidates.empty ())
    {
      return;
    }

  SPFVertex *v = m_candidates.front ();
  m_candidates.pop_front ();
  delete v;
  v = 0;
}

  SPFVertex *
CandidateQueue::Top (void)
{
  NS_DEBUG("CandidateQueue::Top ()");

  if (m_candidates.empty ())
    {
      return 0;
    }

  return m_candidates.front ();
}

  bool
CandidateQueue::Empty (void)
{
  NS_DEBUG("CandidateQueue::Empty ()");

  return m_candidates.empty ();
}

  uint32_t
CandidateQueue::Size (void)
{
  NS_DEBUG("CandidateQueue::Size ()");

  return m_candidates.size ();
}

  SPFVertex *
CandidateQueue::Find (const Ipv4Address addr)
{
  NS_DEBUG("CandidateQueue::Find ()");

  CandidateList_t::iterator i = m_candidates.begin ();

  for (; i != m_candidates.end (); i++)
    {
      SPFVertex *v = *i;
      if (v->m_vertexId == addr)
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
