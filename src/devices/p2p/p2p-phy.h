/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 * Author: Craig Dowell <craigdo@ee.washington.edu>
 */

#ifndef POINT_TO_POINT_PHY_H
#define POINT_TO_POINT_PHY_H

namespace ns3 {

class PointToPointNetDevice;
class PointToPointChannel;
class Node;
class Packet;

class PointToPointPhy {
public:
  PointToPointPhy(Node* node, PointToPointNetDevice* netdevice);
  virtual ~PointToPointPhy();

  void Send (Packet &p);
  void Receive (Packet& p);
  void Attach (PointToPointChannel *channel);

  PointToPointNetDevice *GetDevice (void);

private:
  Node* m_node;
  PointToPointChannel *m_channel;
  PointToPointNetDevice* m_netdevice;
};

} // namespace ns3

#endif // POINT_TO_POINT_PHY_H
