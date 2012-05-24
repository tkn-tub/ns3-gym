/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Marco Miozzo  <marco.miozzo@cttc.es>
 * 
 */
#ifndef BUILDING_H
#define BUILDING_H

#include <ns3/attribute.h>
#include <ns3/attribute-helper.h>
#include <ns3/vector.h>
#include <ns3/box.h>
#include <ns3/simple-ref-count.h>
#include <ns3/object.h>

namespace ns3 {

/**
 * \ingroup mobility
 * \brief a 3d building block
 */
class Building : public Object
{
public:

  // inherited from Object
  static TypeId GetTypeId (void);
  virtual void DoDispose ();


  enum BuildingType_t
    {
      Residential, Office, Commercial
    };
  enum ExtWallsType_t
    {
      Wood, ConcreteWithWindows, ConcreteWithoutWindows, StoneBlocks
    };
  
  /**
   * Construct a simple building with 1 room and 1 floor
   * 
   * \param xMin x coordinates of left boundary.
   * \param xMax x coordinates of right boundary.
   * \param yMin y coordinates of bottom boundary.
   * \param yMax y coordinates of top boundary.
   * \param zMin z coordinates of down boundary.
   * \param zMax z coordinates of up boundary.
   *
   */
  Building (double xMin, 
            double xMax,
            double yMin, 
            double yMax,
            double zMin, 
            double zMax);

  /**
   * Create a zero-sized building located at coordinates (0.0,0.0,0.0)
   * and with 1 floors and 1 room.
   */
  Building ();

  /** 
   * Destructor
   * 
   */
  virtual ~Building ();

  /**
   * \return the unique id of this Building. This unique id happens to
   * be also the index of the Building into the BuildingList. 
   */
  uint32_t GetId (void) const;

  /** 
   * Set the boundaries of the building
   * 
   * \param box the Box defining the boundaries of the building
   */
  void SetBoundaries (Box box);

  /**
   * \param t the type of building (i.e., Residential, Office, Commercial)
   *
   * This method allows to set building type (default is Residential)
   */
  void SetBuildingType (Building::BuildingType_t t);

  /**
   * \param t the type of external walls (i.e., Wood, ConcreteWithWindows,
   * ConcreteWithoutWindows and StoneBlocks), used for evaluating the loss
   * due to the penetration of external walls in outdoor <-> indoor comm.
   *
   * This method allows to set external walls type (default is Residential)
   */
  void SetExtWallsType (Building::ExtWallsType_t t);

  /**
   * \param nfloors the number of floors in the building
   *
   * This method allows to set the number of floors in the building
   * (default is 1)
   */
  void SetNFloors (uint16_t nfloors);

  /**
   * \param nroomx the number of rooms along the x axis
   *
   * This method allows to set the number of rooms along the x-axis
   */
  void SetNRoomsX (uint16_t nroomx);

  /**
   * \param nroomy the number of floors in the building
   *
   * This method allows to set the number of rooms along the y-axis
   */
  void SetNRoomsY (uint16_t nroomy);

  /** 
   * 
   * \return the boundaries of the building
   */
  Box GetBoundaries () const;

  /**
   * \return the type of building
   */
  BuildingType_t GetBuildingType () const;

  /**
   * \return the type of external walls of the building
   */
  ExtWallsType_t GetExtWallsType () const;

  /**
   * \return the number of floors of the building
   */
  uint16_t GetNFloors () const;

  /**
   * \return the number of rooms along the x-axis of the building
   */
  uint16_t GetNRoomsX () const;

  /**
   * \return the number of rooms along the y-axis
   */
  uint16_t GetNRoomsY () const;
  
  /** 
   * 
   * 
   * \param position some position
   * 
   * \return true if the position fall inside the building, false otherwise
   */
  bool IsInside (Vector position) const;
 
  /** 
   * 
   * 
   * \param position a position inside the building
   * 
   * \return the number of the room along the X axis where the
   * position falls
   */
  uint16_t GetRoomX (Vector position) const;

  /** 
   * 
   * 
   * \param position a position inside the building
   * 
   * \return  the number of the room along the Y axis where the
   * position falls
   */
  uint16_t GetRoomY (Vector position) const;

  /** 
   * 
   * \param position a position inside the building 
   * 
   * \return  the floor where the position falls
   */
  uint16_t GetFloor (Vector position) const;




private:

  Box m_buildingBounds;

  /**
   * number of floors, must be greater than 0, and 1 means only one floor
   * (i.e., groundfloor)
   */
  uint16_t m_floors;
  uint16_t m_roomsX;
  uint16_t m_roomsY;

  uint32_t m_buildingId;
  BuildingType_t m_buildingType;
  ExtWallsType_t m_externalWalls;

};

} // namespace ns3

#endif /* BUILDING_H */
