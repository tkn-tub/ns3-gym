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
#include <ns3/pointer.h>
#include <ns3/log.h>
#include <ns3/assert.h>

NS_LOG_COMPONENT_DEFINE ("BuildingsMobilityModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (BuildingsMobilityModel);

TypeId
BuildingsMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BuildingsMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<BuildingsMobilityModel> ();

  return tid;
}


BuildingsMobilityModel::BuildingsMobilityModel ()
{
  NS_LOG_FUNCTION (this);
  m_indoor = false;
  m_nFloor = 1;
  m_roomX = 1;
  m_roomY = 1;
}

void
BuildingsMobilityModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  MobilityModel::DoDispose ();
}

Vector
BuildingsMobilityModel::DoGetPosition (void) const
{
  NS_LOG_FUNCTION (this);
  m_helper.Update ();
  return m_helper.GetCurrentPosition ();
}
void 
BuildingsMobilityModel::DoSetPosition (const Vector &position)
{
  NS_LOG_FUNCTION (this);
  m_helper.SetPosition (position);
}
Vector
BuildingsMobilityModel::DoGetVelocity (void) const
{
  NS_LOG_FUNCTION (this);
  return m_helper.GetVelocity ();
}

bool
BuildingsMobilityModel::IsIndoor (void)
{
  NS_LOG_FUNCTION (this);
  return (m_indoor);
}

bool
BuildingsMobilityModel::IsOutdoor (void)
{
  NS_LOG_FUNCTION (this);
  return (!m_indoor);
}

void
BuildingsMobilityModel::SetIndoor (Ptr<Building> building, uint8_t nfloor, uint8_t nroomx, uint8_t nroomy)
{
  NS_LOG_FUNCTION (this);
  m_indoor = true;
  m_myBuilding = building;
  m_nFloor = nfloor;
  m_roomX = nroomx;
  m_roomY = nroomy;
  
  NS_ASSERT_MSG (building->IsInside (m_helper.GetCurrentPosition ()), "Position of the node is outside of building bounds");
  NS_ASSERT (m_roomX > 0);
  NS_ASSERT (m_roomX <= building->GetNRoomsX ());
  NS_ASSERT (m_roomY > 0);
  NS_ASSERT (m_roomY <= building->GetNRoomsY ());
  NS_ASSERT (m_nFloor > 0);
  NS_ASSERT (m_nFloor <= building->GetNFloors ());

}


void
BuildingsMobilityModel::SetOutdoor (void)
{
  NS_LOG_FUNCTION (this);
  m_indoor = false;
}

uint8_t
BuildingsMobilityModel::GetFloorNumber (void)
{
  NS_LOG_FUNCTION (this);
  return (m_nFloor);
}

uint8_t
BuildingsMobilityModel::GetRoomNumberX (void)
{
  NS_LOG_FUNCTION (this);
  return (m_roomX);
}

uint8_t
BuildingsMobilityModel::GetRoomNumberY (void)
{
  NS_LOG_FUNCTION (this);
  return (m_roomY);
}


Ptr<Building>
BuildingsMobilityModel::GetBuilding ()
{
  NS_LOG_FUNCTION (this);
  return (m_myBuilding);
}

  
} // namespace
