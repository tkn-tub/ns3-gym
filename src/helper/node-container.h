/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef NODE_CONTAINER_H
#define NODE_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "ns3/node.h"

namespace ns3 {

/**
 * \brief keep track of a set of node pointers.
 *
 */
class NodeContainer
{
public:
  typedef std::vector<Ptr<Node> >::const_iterator Iterator;

  /**
   * Create an empty NodeContainer.
   */
  NodeContainer ();
  /**
   * \param node a node to add to the container
   *
   * Create a NodeContainer with exactly one node.
   */
  NodeContainer (Ptr<Node> node);
  /**
   * \param a a node container
   * \param b another node container
   *
   * Create a node container which is a concatenation of the two input
   * NodeContainers.
   *
   * \note A frequently seen idiom that uses these constructors involves the
   * implicit conversion by constructor of Ptr<Node>.  When used, two 
   * Ptr<Node> will be passed to this constructor instead of NodeContainer&.
   * C++ will notice the implicit conversion path that goes through the 
   * NodeContainer (Ptr<Node> node) constructor above.  Using this conversion
   * one may provide optionally provide arguments of Ptr<Node> to these 
   * constructors.
   */
  NodeContainer (const NodeContainer &a, const NodeContainer &b);

  NodeContainer (const NodeContainer &a, const NodeContainer &b, const NodeContainer &c);
  NodeContainer (const NodeContainer &a, const NodeContainer &b, const NodeContainer &c, const NodeContainer &d);
  NodeContainer (const NodeContainer &a, const NodeContainer &b, const NodeContainer &c, const NodeContainer &d,
                 const NodeContainer &e);

  /**
   * \returns an iterator to the start of the vector of node pointers.
   */
  Iterator Begin (void) const;
  /**
   * \returns an iterator to the end of the vector of node pointers.
   */
  Iterator End (void) const;

  /**
   * \returns the number of node pointers stored in this NodeContainer.
   */
  uint32_t GetN (void) const;
  /**
   * \param i the index of the requested node pointer.
   * \returns the requested node pointer.
   */
  Ptr<Node> Get (uint32_t i) const;

  /**
   * \param n the number of nodes to create
   *
   * Create n nodes and append pointers to them to the end of this NodeContainer.
   */
  void Create (uint32_t n);
  /**
   * \param other another NodeContainer
   *
   * Append the node pointers from the input NodeContainer at the end
   * of this NodeContainer.
   */
  void Add (NodeContainer other);
  /**
   * \param node a node pointer
   *
   * Append the input node pointer at the end of this NodeContainer.
   */
  void Add (Ptr<Node> node);

  /**
   * \returns a container which contains a list of _all_ nodes
   *          created through NodeContainer::Create and stored
   *          in ns3::NodeList.
   */
  static NodeContainer GetGlobal (void);

 private:
  std::vector<Ptr<Node> > m_nodes;
};

} // namespace ns3

#endif /* NODE_CONTAINER_H */
