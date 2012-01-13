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
  static TypeId GetTypeId (void);

  enum BuildingType_t
    {
      Residential, Office, Commercial
    };
  enum ExtWallsType_t
    {
      Wood, ConcreteWithWindows, ConcreteWithoutWindows, StoneBlocks
    };
  
  /**
   * \param xMin x coordinates of left boundary.
   * \param xMax x coordinates of right boundary.
   * \param yMin y coordinates of bottom boundary.
   * \param yMax y coordinates of top boundary.
   * \param zMin z coordinates of down boundary.
   * \param zMax z coordinates of up boundary.
   *
   * Create a building.
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
  void SetFloorsNumber (uint8_t nfloors);

  /**
   * \param nroomx the number of rooms along the x axis
   *
   * This method allows to set the number of rooms along the x-axis
   */
  void SetNumberRoomX (uint8_t nroomx);

  /**
   * \param nroomy the number of floors in the building
   *
   * This method allows to set the number of rooms along the y-axis
   */
  void SetNumberRoomY (uint8_t nroomy);


  /**
   * \return the type of building
   * Return the type of building (i.e., Residential, Office, Commercial)
   */
  BuildingType_t GetBuildingType ();

  /**
   * \return the type of external walls of the building
   */
  ExtWallsType_t GetExtWallsType ();

  /**
   * \return the number of floors of the building
   */
  uint8_t GetNumberFloors ();

  /**
   * \return the number of rooms along the x-axis of the building
   */
  uint8_t GetNumberRoomX ();

  /**
   * \return the number of rooms along the y-axis
   */
  uint8_t GetNumberRoomY ();
  
  /**
   * \return the bounds of the building as Box class
   */
  Box GetBuildingBounds ();

 

private:

  void Construct ();

  Box m_buldingBounds;
  /**
   * number of floors, must be greater than 0, and 1 means only one floor
   * (i.e., groundfloor)
   */
  uint8_t m_floor;
  uint8_t m_roomX;
  uint8_t m_roomY;

  uint32_t m_buildingId;
  BuildingType_t m_buildingType;
  ExtWallsType_t m_externalWalls;

};

//std::ostream &operator << (std::ostream &os, const Box &box);
//std::istream &operator >> (std::istream &is, Box &box);


ATTRIBUTE_HELPER_HEADER (Building);

} // namespace ns3

#endif /* BUILDING_H */
