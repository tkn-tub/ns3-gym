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

#ifndef CANDIDATE_QUEUE_H
#define CANDIDATE_QUEUE_H

#include <stdint.h>
#include <list>
#include "static-route-manager.h"

namespace ns3 {

class CandidateQueue
{
public:
  CandidateQueue ();
  virtual ~CandidateQueue ();

  void          Clear (void);
  void          Push (SPFVertex *v);
  void          Pop (void);
  SPFVertex*    Top (void);
  bool          Empty (void);
  uint32_t      Size (void);
  SPFVertex*    Find (const Ipv4Address addr);
  void          Reorder (void);

protected:
  typedef std::list<SPFVertex*> CandidateList_t;
  CandidateList_t m_candidates;

private:
};

} // namespace ns3

#endif /* CANDIDATE_QUEUE_H */
