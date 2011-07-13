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

namespace ns3 {

/**
 * \ingroup mobility
 * \brief a 3d building block
 */
class Building : public SimpleRefCount<Building>
{
public:
  enum BuildingType_t
    {
      Residential, Office, Commercial
    };
  enum ExtWallsType_t
    {
      Wood, ConcreteWithWindows, ConcreteWithoutWindows, StoneBlocks
    };
  /**
  * \param _xMin x coordinates of left boundary.
  * \param _xMax x coordinates of right boundary.
  * \param _yMin y coordinates of bottom boundary.
  * \param _yMax y coordinates of top boundary.
  * \param _zMin z coordinates of down boundary.
  * \param _zMax z coordinates of up boundary.
  * \param _nFloors number of floors
  * \param _nRoomX number of rooms in the x axis
  * \param _nRoomY number of rooms in the y axis
  *
  * Create a building.
  */
  Building (double _xMin, double _xMax,
            double _yMin, double _yMax,
            double _zMin, double _zMax/*,
            uint8_t _nFloors, uint8_t _nRoomX, uint8_t _nRoomY*/);
  
  /**
  * Create a zero-sized building located at coordinates (0.0,0.0,0.0)
  * and with no floors and 1 room.
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
  * \param nroomx the number of rooms in the x axis
  *
  * This method allows to set the number of room in x-axis (default is 1)
  * The rooms are disposed as a grid of nº of rooms in X per nº of rooms in Y
  */
  void SetNumberRoomX (uint8_t nroomx);
  
  /**
  * \param nroomy the number of floors in the building
  *
  * This method allows to set the number of floors in the building
  * (default is 1)
  */
  void SetNumberRoomY (uint8_t nroomy);
  
  
  /**
  * \return the type of building
  * Return the type of building (i.e., Residential, Office, Commercial)
  */
  BuildingType_t GetBuildingType ();
  
  /**
  * \return the type of external walls
  * Return the type of external walls (i.e., Wood, ConcreteWithWindows,
  * ConcreteWithoutWindows)
  */
  ExtWallsType_t GetExtWallsType ();
  
  /**
  * \return the number of floors
  * Return the number of floors
  */
  uint8_t GetNumberFloors ();
  
  /**
  * \return the number of room in x-axis
  * Return the number of room in x-axis
  */
  uint8_t GetNumberRoomX ();
  
  /**
  * \return the number of room in y-axis
  * Return the number of room in y-axis
  */
  uint8_t GetNumberRoomY ();
  
  
private:
  Box m_buldingBounds;
  
  uint8_t m_floor;
  uint8_t m_roomX;
  uint8_t m_roomY;
  
  uint8_t m_buildingId;
  BuildingType_t m_buildingType;
  ExtWallsType_t m_externalWalls;
  
};

//std::ostream &operator << (std::ostream &os, const Box &box);
//std::istream &operator >> (std::istream &is, Box &box);


ATTRIBUTE_HELPER_HEADER (Building);

} // namespace ns3

#endif /* BUILDING_H */
