/* -*-	Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef INTERNET_NODE_H
#define INTERNET_NODE_H

#include "ns3/i-node.h"
#include "ns3/ptr.h"

namespace ns3 {

/**
 * \returns a newly-created Node which supports the Ipv4 interfaces
 */
Ptr<INode> MakeInternetNode (void);

/**
 * \param systemId a systemId for parallel simulations.
 * \returns a newly-created Node which supports the Ipv4 interfaces
 */
Ptr<INode> MakeInternetNode (uint32_t systemId);

} // namespace ns3

#endif /* INTERNET_NODE_H */
