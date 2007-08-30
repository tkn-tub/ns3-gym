/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
 * Authors: 
 *  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>,
 */
#ifndef NODE_LIST_H
#define NODE_LIST_H

#include <vector>
#include "ns3/trace-context-element.h"
#include "ns3/ptr.h"

namespace ns3 {

class Node;
class CallbackBase;
class TraceResolver;

/**
 * \brief hold in a TraceContext the index of a node within a NodeList.
 */
class NodeListIndex : public TraceContextElement
{
public:
  NodeListIndex ();
  NodeListIndex (uint32_t index);
  void Print (std::ostream &os);
  static uint16_t GetUid (void);
  /**
   * \returns the index of the node within the NodeList
   */
  uint32_t Get (void) const;
  std::string GetTypeName (void) const;
private:
  uint32_t m_index;
};


/**
 * \brief the list of simulation nodes.
 *
 * Every Node created is automatically added to this list.
 */
class NodeList
{
public:
  typedef std::vector< Ptr<Node> >::const_iterator Iterator;

  /**
   * \param node node to add
   * \returns index of node in list.
   *
   * This method is called automatically from Node::Node so
   * the user has little reason to call it himself.
   */
  static uint32_t Add (Ptr<Node> node);
  /**
   * \returns a C++ iterator located at the beginning of this
   *          list.
   */
  static Iterator Begin (void);
  /**
   * \returns a C++ iterator located at the end of this
   *          list.
   */
  static Iterator End (void);
  /**
   * \param n index of requested node.
   * \returns the Node associated to index n.
   */
  static Ptr<Node> GetNode (uint32_t n);
  /**
   * \param name namespace regexp to match
   * \param cb callback to connect
   *
   * Connect input callback to all trace sources which match
   * the input namespace regexp.
   */
  static void Connect (std::string name, const CallbackBase &cb);
  /**
   * \param name namespace regexp to match
   * \param cb callback to connect
   *
   * Disconnect input callback from all trace sources which match
   * the input namespace regexp.
   */
  static void Disconnect (std::string name, const CallbackBase &cb);
  static Ptr<TraceResolver> GetTraceResolver (void);
private:
};

}//namespace ns3


#endif /* NODE_LIST_H */
