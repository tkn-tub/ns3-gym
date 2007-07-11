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
#include "ns3/candidate-queue.h"

using namespace ns3;

  int 
main (int argc, char *argv[])
{
  NS_DEBUG_UNCOND("Candidate Queue Test");

#if 0
  DebugComponentEnable("CandidateQueue");
#endif

  CandidateQueue candidate;
  NS_ASSERT(candidate.Size () == 0);
 
  for (int i = 0; i < 100; ++i)
    {
      SPFVertex *v = new SPFVertex;
      v->m_distanceFromRoot = rand () % 100;
      candidate.Push (v);
    }

  uint32_t lastDistance = 0;
  bool ok = true;

  for (int i = 0; i < 100; ++i)
    {
      SPFVertex *v = candidate.Top ();
      candidate.Pop ();
      if (v->m_distanceFromRoot < lastDistance)
        {
          ok = false;
          NS_ASSERT(0);
        }
      lastDistance = v->m_distanceFromRoot;
    }

  for (int i = 0; i < 100; ++i)
    {
      SPFVertex *v = new SPFVertex;
      v->m_distanceFromRoot = rand () % 100;
      candidate.Push (v);
    }

#if 0
  for (int i = 0; i < 100; ++i)
    {
      SPFVertex *v = candidate.Find (i);
      if (v) {
        v->m_distanceFromRoot = rand () % 100;
      }
    }
#endif

  candidate.Reorder ();

  lastDistance = 0;

  for (int i = 0; i < 100; ++i)
    {
      SPFVertex *v = candidate.Top ();
      candidate.Pop ();
      if (v->m_distanceFromRoot < lastDistance)
        {
          ok = false;
          NS_ASSERT(0);
        }
      lastDistance = v->m_distanceFromRoot;
    }

  NS_ASSERT(candidate.Size () == 0);
 
  return 0;
}
