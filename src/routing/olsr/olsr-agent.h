/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
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
 * Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
 */

#ifndef OLSR_AGENT_H
#define OLSR_AGENT_H

#include "ns3/node.h"

namespace ns3 {
namespace olsr {

/**
 * \brief Class implementing the OLSR state machine
 *
 * This class represents an instance of the OLSR protocol.  It
 * attaches itself to a Node, and its lifecycle is bound to that node.
 * Normally the functions in the ns3::olsr namespace are more simple
 * to use to start OLSR on nodes, but access to the underlying OLSR
 * agent can be useful in order to customize the OLSR parameters.
 * Example:
 *
 * \code
 * Ptr<olsr::Agent> olsr = Agend::CreateDefault ();
 * agent->SetMainInterface (2);
 * agent->Start ();
 * \endcode
 */
class Agent : public Object
{
public:
  static TypeId GetTypeId (void);

  static Ptr<Agent> CreateDefault (Ptr<Node> node);

  /**
   * \brief Sets the main interface to be used by OLSR
   *
   * Normally OLSR supports multiple interfaces, but the protocol
   * requires the definition of a "main interface".  This interface's
   * IPv4 address provides the identity of the node, and all outgoing
   * OLSR routing messages must have the main interface address,
   * regardless of the actual interface used to transmit the packet.
   * This method allows one to explicitly select an interface as the
   * main interface.  It must be called before the agent starts, but
   * calling it is optional; if not called, the agent tries to guess
   * and uses a suitable interface.
   */
  virtual void SetMainInterface (uint32_t interface) = 0;

  /**
   * \brief Starts the OLSR protocol operation
   *
   * Calling this method essentially bootstraps the OLSR protocol, and
   * causes the agent to start broadcasting OLSR messages to
   * neighbors, as well start listening to messages from neighbors.
   */
  virtual void Start () = 0;
};


}} // namespace olsr, ns3

#endif /* OLSR_AGENT_H */

