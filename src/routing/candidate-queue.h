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

/**  
 * \brief a Candidate Queue used in static routing.
 *
 * The CandidateQueue is used in the OSPF shortest path computations.  It
 * is a priority queue used to store candidates for the shortest path to a
 * given network.  
 *
 * The queue holds Shortest Path First Vertex pointers and orders them
 * according to the lowest value of the field m_distanceFromRoot.  Remaining
 * vertices are ordered according to increasing distance.  This implements a
 * priority queue.
 *
 * Although a STL priority_queue almost does what we want, the requirement
 * for a Find () operation, the dynamic nature of the data and the derived
 * requirement for a Reorder () operation led us to implement this simple 
 * enhanced priority queue.
 */
class CandidateQueue
{
public:
  /**
   * Create an empty SPF Candidate Queue.  
   *
   * @see SPFVertex
   */
  CandidateQueue ();
  /**
   * Destroy an SPF Candidate Queue and release any resources held by the 
   * contents.
   *
   * @see SPFVertex
   */
  virtual ~CandidateQueue ();
  /**
   * Empty the Candidate Queue and release all of the resources associated 
   * with the Shortest Path First Vertex pointers in the queue.
   *
   * @see SPFVertex
   */
  void          Clear (void);
  /**
   * Push a Shortest Path First Vertex pointer onto the queue according to the
   * priority scheme.
   * 
   * On completion, the top of the queue will hold the Shortest Path First
   * Vertex pointer that points to a vertex having lowest value of the field
   * m_distanceFromRoot.  Remaining vertices are ordered according to 
   * increasing distance.
   *
   * @see SPFVertex
   * @param vNew The Shortest Path First Vertex to add to the queue.
   */
  void          Push (SPFVertex *vNew);
  /**
   * Pop the Shortest Path First Vertex pointer at the top of the queue.
   * The caller is given the responsiblity for releasing the resources 
   * associated with the vertex.
   *
   * @see SPFVertex
   * @see Top ()
   * @returns The Shortest Path First Vertex pointer at the top of the queue.
   */
  SPFVertex*          Pop (void);
  /**
   * Return the Shortest Path First Vertex pointer at the top of the queue.  
   * This method does not pop the SPFVertex* off of the queue, it simply 
   * returns the pointer.
   *
   * @see SPFVertex
   * @see Pop ()
   * @returns The Shortest Path First Vertex pointer at the top of the queue.
   */
  SPFVertex*    Top (void);
  /**
   * Test the Candidate Queue to determine if it is empty.
   *
   * @returns True if the queue is empty, false otherwise.
   */
  bool          Empty (void);
  /**
   * Return the number of Shortest Path First Vertex pointers presently
   * stored in the Candidate Queue.
   *
   * @see SPFVertex
   * @returns The number of SPFVertex* pointers in the Candidate Queue.
   */
  uint32_t      Size (void);
  /**
   * Searches the Candidate Queue for a Shortest Path First Vertex pointer
   * that points to a vertex having the given IP address.
   *
   * @see SPFVertex
   * @param addr The IP address to search for.
   * @returns The SPFVertex* pointer corresponding to the given IP address.
   */
  SPFVertex*    Find (const Ipv4Address addr);
  /**
   * Reorders the Candidate Queue according to the priority scheme.  On 
   * completion, the top of the queue will hold the Shortest Path First
   * Vertex pointer that points to a vertex having lowest value of the field
   * m_distanceFromRoot.  Remaining vertices are ordered according to 
   * increasing distance.
   *
   * This method is provided in case the values of m_distanceFromRoot change
   * during the routing calculations.
   *
   * @see SPFVertex
   */
  void          Reorder (void);

protected:
  typedef std::list<SPFVertex*> CandidateList_t;
  CandidateList_t m_candidates;

private:
  /**
   * Candidate Queue copy construction is disallowed (not implemented) to 
   * prevent the compiler from slipping in incorrect versions that don't
   * properly deal with deep copies.
   */
  CandidateQueue (CandidateQueue& sr);
  /**
   * Candidate Queue assignment operator is disallowed (not implemented) to
   * prevent the compiler from slipping in incorrect versions that don't
   * properly deal with deep copies.
   */
  CandidateQueue& operator= (CandidateQueue& sr);
};

} // namespace ns3

#endif /* CANDIDATE_QUEUE_H */
