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
#include "ns3/packet.h"
#include "layer-connector.h"

#ifndef CHANNEL_H
#define CHANNEL_H

namespace ns3 {

class Channel : public LayerConnectorLower
{
public:
  Channel ();
  virtual ~Channel ();

  // Called by the physical layer to cause bits to propagate along the channel
  // The channel will call Receive on each of the phys.
  bool Propagate (Packet &p);
  bool DoConnectToUpper (LayerConnectorUpper &upper);
  bool LowerDoNotify (LayerConnectorUpper *upper);

protected:
  typedef std::list<LayerConnectorUpper *> ConnectorList;
  ConnectorList m_connectorList;

private:
};

}; // namespace ns3

#endif /* CHANNEL_H */
