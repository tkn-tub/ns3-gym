/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (C) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */
#ifndef BUILDING_POSITION_ALLOCATOR_H
#define BUILDING_POSITION_ALLOCATOR_H

#include <ns3/ptr.h>
#include <ns3/position-allocator.h>
#include <ns3/node-container.h>
#include "ns3/random-variable-stream.h"

namespace ns3 {

class Building;
class UniformRandomVariable;


/**
 * Allocate each position by randomly chosing a building from the list
 * of all buildings, and then randomly chosing a position inside the building.
 * 
 */
class RandomBuildingPositionAllocator : public PositionAllocator
{
public:
  RandomBuildingPositionAllocator ();

  // inherited from Object
  static TypeId GetTypeId (void);

  // inherited from PositionAllocator
  virtual Vector GetNext (void) const;

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  int64_t AssignStreams (int64_t stream);

private:
  
  bool m_withReplacement;
  mutable std::vector< Ptr<Building> > m_buildingListWithoutReplacement;

  /// Provides uniform random variables.
  Ptr<UniformRandomVariable> m_rand;
};


/**
 * Allocate each position by randomly chosing a room from the list
 * of all buildings, and then randomly chosing a position inside the room.
 * The selection of the room is always done without replacement.
 * 
 */
class RandomRoomPositionAllocator : public PositionAllocator
{
public:
  RandomRoomPositionAllocator ();

  // inherited from Object
  static TypeId GetTypeId (void);

  // inherited from PositionAllocator
  virtual Vector GetNext (void) const;

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  int64_t AssignStreams (int64_t stream);

private:
  
  bool m_withReplacement;
  struct RoomInfo 
  {
    Ptr<Building> b;
    uint32_t roomx;
    uint32_t roomy;
    uint32_t floor;
  };
  mutable std::vector<RoomInfo> m_roomListWithoutReplacement;

  /// Provides uniform random variables.
  Ptr<UniformRandomVariable> m_rand;
};


/**
 * Walks a given NodeContainer sequentially, and for each node allocate a new
 * position randomly in the same room of that node
 * 
 */
class SameRoomPositionAllocator : public PositionAllocator
{
public:
  SameRoomPositionAllocator ();
  SameRoomPositionAllocator (NodeContainer c);

  // inherited from Object
  static TypeId GetTypeId (void);

  // inherited from PositionAllocator
  virtual Vector GetNext (void) const;

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  int64_t AssignStreams (int64_t);

private:

  NodeContainer m_nodes;
  mutable NodeContainer::Iterator m_nodeIt;

  /// Provides uniform random variables.
  Ptr<UniformRandomVariable> m_rand;
};


} // namespace ns3

#endif /* BUILDING_POSITION_ALLOCATOR_H */
