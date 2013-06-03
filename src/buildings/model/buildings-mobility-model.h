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
#ifndef MOBILITY_BUILDING_INFO_H
#define MOBILITY_BUILDING_INFO_H



#include <ns3/simple-ref-count.h>
#include <ns3/ptr.h>
#include <ns3/object.h>
#include <ns3/box.h>
#include <map>
#include <ns3/building.h>
#include <ns3/constant-velocity-helper.h>



namespace ns3 {


/**
 * \ingroup buildings
 * \brief mobility buildings information (to be used by mobility models)
 *
 * This model implements the managment of scenarios where users might be
 * either indoor (e.g., houses, offices, etc.) and outdoor.
 * 
 */
class MobilityBuildingInfo : public Object
{
public:
  static TypeId GetTypeId (void);
  MobilityBuildingInfo ();

  MobilityBuildingInfo (Ptr<Building> building);

  /** 
   * 
   * \return true if the MobilityBuildingInfo instance is indoor, false otherwise
   */
  bool IsIndoor (void);

  /** 
   * 
   * \return true if the MobilityBuildingInfo instance is outdoor, false otherwise
   */
  bool IsOutdoor (void);

  /** 
   * Mark this MobilityBuildingInfo instance as indoor
   * 
   * \param building the building into which the MobilityBuildingInfo instance is located
   * \param nfloor the floor number 1...nFloors at which the  MobilityBuildingInfo instance is located
   * \param nroomx the X room number 1...nRoomsX at which the  MobilityBuildingInfo instance is located
   * \param nroomy the Y room number 1...nRoomsY at which the  MobilityBuildingInfo instance is located
   */
  void SetIndoor (Ptr<Building> building, uint8_t nfloor, uint8_t nroomx, uint8_t nroomy);


  void SetIndoor (uint8_t nfloor, uint8_t nroomx, uint8_t nroomy);

  /** 
   * Mark this MobilityBuildingInfo instance as outdoor
   * 
   */
  void SetOutdoor ();

  /** 
   * 
   * \return 
   */
  uint8_t GetFloorNumber (void);

  /** 
   * 
   * \return 
   */
  uint8_t GetRoomNumberX (void);

  /** 
   * 
   * \return 
   */
  uint8_t GetRoomNumberY (void);

  /** 
   * 
   * \return 
   */
  Ptr<Building> GetBuilding ();



private:

  Ptr<Building> m_myBuilding;
  bool m_indoor;
  uint8_t m_nFloor;
  uint8_t m_roomX;
  uint8_t m_roomY;

};



} // namespace ns3


#endif // MOBILITY_BUILDING_INFO_H
