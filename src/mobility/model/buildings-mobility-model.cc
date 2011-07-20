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

#include <ns3/simulator.h>
#include <ns3/position-allocator.h>
#include <ns3/buildings-mobility-model.h>
#include "ns3/pointer.h"


namespace ns3 {
  
  NS_OBJECT_ENSURE_REGISTERED (BuildingsMobilityModel);
  
TypeId
BuildingsMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BuildingsMobilityModel")
  .SetParent<MobilityModel> ()
  .SetGroupName ("Mobility")
  .AddConstructor<BuildingsMobilityModel> ()
  .AddAttribute ("Bounds",
                  "Bounds of the area to cruise.",
                  BoxValue (Box (-100.0, 100.0, -100.0, 100.0, 0.0, 100.0)),
                  MakeBoxAccessor (&BuildingsMobilityModel::m_bounds),
                  MakeBoxChecker ());
                                                                                                                                          
                                                                                                                                           return tid;
}
  

BuildingsMobilityModel::BuildingsMobilityModel ()
{
  m_indoor = false;
  m_nFloor = 1;
  m_roomX = 1;
  m_roomY = 1;
}

void
BuildingsMobilityModel::DoDispose (void)
{
  // chain up
  m_surroudingBuildings.clear ();
  MobilityModel::DoDispose ();
}

Vector
BuildingsMobilityModel::DoGetPosition (void) const
{
  m_helper.Update ();
  return m_helper.GetCurrentPosition ();
}
void 
BuildingsMobilityModel::DoSetPosition (const Vector &position)
{
  m_helper.SetPosition (position);
}
Vector
BuildingsMobilityModel::DoGetVelocity (void) const
{
  return m_helper.GetVelocity ();
}

bool
BuildingsMobilityModel::IsIndoor (void)
{
  return (m_indoor);
}

bool
BuildingsMobilityModel::IsOutdoor (void)
{
  return (!m_indoor);
}

void
BuildingsMobilityModel::SetIndoor (Ptr<Building> building)
{
  m_indoor = true;
  m_myBuilding = building;
}

void
BuildingsMobilityModel::SetOutdoor (void)
{
  m_indoor = false;
}

void
BuildingsMobilityModel::SetFloorNumber (uint8_t nfloor)
{
  m_nFloor = nfloor;
}

void
BuildingsMobilityModel::SetRoomNumberX (uint8_t nroomx)
{
  m_roomX = nroomx;
}

void
BuildingsMobilityModel::SetRoomNumberY (uint8_t nroomy)
{
  m_roomY = nroomy;
}


void
BuildingsMobilityModel::SetSurroudingBuilding (Ptr<Building> building)
{
  m_surroudingBuildings.push_back (building);
}

uint8_t
BuildingsMobilityModel::GetFloorNumber (void)
{
  return (m_nFloor);
}

uint8_t
BuildingsMobilityModel::GetRoomNumberX (void)
{
  return (m_roomX);
}

uint8_t
BuildingsMobilityModel::GetRoomNumberY (void)
{
  return (m_roomY);
}


Ptr<Building>
BuildingsMobilityModel::GetBuilding ()
{
  return (m_myBuilding);
}

  
} // namespace