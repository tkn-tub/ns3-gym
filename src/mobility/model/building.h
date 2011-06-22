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
            double _zMin, double _zMax,
            uint8_t _nFloors, uint8_t _nRoomX, uint8_t _nRoomY);
  
  /**
  * Create a zero-sized building located at coordinates (0.0,0.0,0.0)
  * and with no floors and 1 room.
  */
  Building ();
  
private:
  Box m_buldingBounds;
  
  uint8_t m_floor;
  uint8_t m_roomX;
  uint8_t m_roomY;
  
  uint8_t m_buildingId;
  
};

//std::ostream &operator << (std::ostream &os, const Box &box);
//std::istream &operator >> (std::istream &is, Box &box);


ATTRIBUTE_HELPER_HEADER (Building);

} // namespace ns3

#endif /* BUILDING_H */
