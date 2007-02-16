/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 * Author: Craig Dowell <craigdo@ee.washingon.edu>
 *
 *      Wed Feb 14 16:05:46 PST 2007 craigdo:  Created
 */

#include <list>
#include "physical-layer.h"

#ifndef CHANNEL_H
#define CHANNEL_H

namespace ns3 {

class Propagator;

class Channel
{
public:
  Channel ();
  virtual ~Channel ();

  // called by the builderto introduce the channel and the physical layer
  // of the node.
  bool Connect (PhysicalLayer &phys);

  // Called by the physical layer to cause bits to propagate along the channel
  // The channel will call Receive on each of the phys.
  bool Propagate (Propagator &p);

protected:
  typedef std::list<PhysicalLayer *> PhysicalLayerList;
  PhysicalLayerList m_physList;

private:
};

}; // namespace ns3

#endif /* SERIAL_PHY_H */
