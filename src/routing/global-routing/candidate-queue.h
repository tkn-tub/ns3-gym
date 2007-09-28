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

#ifndef CANDIDATE_QUEUE_H
#define CANDIDATE_QUEUE_H

#include <stdint.h>
#include <list>
#include "global-route-manager-impl.h"

namespace ns3 {

/**  
 * \brief A Candidate Queue used in static routing.
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
 * @brief Create an empty SPF Candidate Queue.  
 * @internal
 *
 * @see SPFVertex
 */
  CandidateQueue ();

/**
 * @internal Destroy an SPF Candidate Queue and release any resources held 
 * by the contents.
 * @internal
 *
 * @see SPFVertex
 */
  virtual ~CandidateQueue ();

/**
 * @brief Empty the Candidate Queue and release all of the resources 
 * associated with the Shortest Path First Vertex pointers in the queue.
 * @internal
 *
 * @see SPFVertex
 */
  void Clear (void);

/**
 * @brief Push a Shortest Path First Vertex pointer onto the queue according
 * to the priority scheme.
 * @internal
 * 
 * On completion, the top of the queue will hold the Shortest Path First
 * Vertex pointer that points to a vertex having lowest value of the field
 * m_distanceFromRoot.  Remaining vertices are ordered according to 
 * increasing distance.
 *
 * @see SPFVertex
 * @param vNew The Shortest Path First Vertex to add to the queue.
 */
  void Push (SPFVertex *vNew);

/**
 * @brief Pop the Shortest Path First Vertex pointer at the top of the queue.
 * @internal
 *
 * The caller is given the responsiblity for releasing the resources 
 * associated with the vertex.
 *
 * @see SPFVertex
 * @see Top ()
 * @returns The Shortest Path First Vertex pointer at the top of the queue.
 */
  SPFVertex* Pop (void);

/**
 * @brief Return the Shortest Path First Vertex pointer at the top of the 
 * queue.  
 * @internal
 *
 * This method does not pop the SPFVertex* off of the queue, it simply 
 * returns the pointer.
 *
 * @see SPFVertex
 * @see Pop ()
 * @returns The Shortest Path First Vertex pointer at the top of the queue.
 */
  SPFVertex* Top (void) const;

/**
 * @brief Test the Candidate Queue to determine if it is empty.
 * @internal
 *
 * @returns True if the queue is empty, false otherwise.
 */
  bool Empty (void) const;

/**
 * @brief Return the number of Shortest Path First Vertex pointers presently
 * stored in the Candidate Queue.
 * @internal
 *
 * @see SPFVertex
 * @returns The number of SPFVertex* pointers in the Candidate Queue.
 */
  uint32_t Size (void) const;

/**
 * @brief Searches the Candidate Queue for a Shortest Path First Vertex 
 * pointer that points to a vertex having the given IP address.
 * @internal
 *
 * @see SPFVertex
 * @param addr The IP address to search for.
 * @returns The SPFVertex* pointer corresponding to the given IP address.
 */
  SPFVertex* Find (const Ipv4Address addr) const;

/**
 * @brief Reorders the Candidate Queue according to the priority scheme.  
 * @internal
 * 
 * On completion, the top of the queue will hold the Shortest Path First
 * Vertex pointer that points to a vertex having lowest value of the field
 * m_distanceFromRoot.  Remaining vertices are ordered according to 
 * increasing distance.
 *
 * This method is provided in case the values of m_distanceFromRoot change
 * during the routing calculations.
 *
 * @see SPFVertex
 */
  void Reorder (void);

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
