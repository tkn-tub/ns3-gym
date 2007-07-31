/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
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
 * Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
 */

#ifndef OLSR_H
#define OLSR_H

#include "ns3/node-list.h"

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
