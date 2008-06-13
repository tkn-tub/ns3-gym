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

#ifndef OLSR_H
#define OLSR_H

#include "ns3/node-list.h"

/**
 * \ingroup routing
 * \defgroup olsr OLSR
 *
 * \section model Model
 *
 * This model implements the base specification of the Optimized
 * Link State Routing (OLSR) protocol, which is a dynamic mobile ad hoc
 * unicast routing protocol.  It has been developed at the 
 * University of Murcia (Spain) by Francisco J. Ros for ns-2, and was
 * ported to ns-3 by Gustavo Carneiro.
 *
 * Here is a summary of software's main features:
 *  - Compliant with core-OLSR (but the use of multiple interfaces which is currently not supported by NS2) as it is documented in RFC 3626 
 * (http://www.ietf.org/rfc/rfc3626.txt).
 *  -  Support for MAC layer feedback as described in RFC 3626.
 *
 * \section api API and Usage
 * 
 * A helper class for OLSR has been written.  After an IPv4 topology
 * has been created and assigned IP addresses, the simulation script 
 * writer can call one of three overloaded functions with different scope 
 * to enable OLSR:  ns3::OlsrHelper::Install (NodeContainer container);
 * ns3::OlsrHelper::Install (Ptr<Node> node);
 * or ns3::OlsrHelper::InstallAll (void);
 *
 * In addition, the behavior of OLSR can be modified by changing certain
 * attributes.  The method ns3::OlsrHelper::SetAgent () can be used
 * to set OLSR attributes.  These include HelloInterval, TcInterval,
 * MidInterval, Willingness.  Other parameters are defined as macros
 * in olsr-agent-impl.cc.
 */

namespace ns3
{
  /**
   * \namespace ns3::olsr
   * \brief Includes a set of utility functions to enable OLSR on
   * certain nodes with default parameters.  For finer grained control
   * of OLSR parameters, see OlsrAgent.
   */
  namespace olsr
  {
    /// \brief Start the OLSR routing agent on all nodes
    void EnableAllNodes (void);

    /// \brief Start the OLSR routing agent on a given list of nodes
    template <typename InputIterator>
    void EnableNodes (InputIterator begin, InputIterator end);

    /// \brief Start the OLSR routing agent on the given node
    void EnableNode (Ptr<Node> node);
  }
}


// implementation
namespace ns3
{
  namespace olsr
  {
    template <typename InputIterator>
    void EnableNodes (InputIterator begin, InputIterator end)
    {
      for (InputIterator i = begin; i != end; i++)
        {
          EnableNode (*i);
        }
    }
  }
}


#endif /* OLSR_H */
