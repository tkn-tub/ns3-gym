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
#ifndef BUILDINGS_MOBILITY_MODEL_H
#define BUILDINGS_MOBILITY_MODEL_H



#include <ns3/mobility-model.h>
#include <ns3/ptr.h>
#include <ns3/object.h>
#include <ns3/box.h>
#include <map>
#include <ns3/constant-velocity-helper.h>
#include <ns3/building.h>
//#include "ns3/random-variable.h"
//#include "ns3/nstime.h"
//#include "ns3/event-id.h"



namespace ns3 {


/**
 * \ingroup mobility
 * \brief Buildings mobility model
 *
 * This model implements the managment of scenarios where users might be
 * either indoor (e.g., houses, offices, etc.) and outdoor.
 * 
 */


class BuildingsMobilityModel : public MobilityModel
{
  public:
    static TypeId GetTypeId (void);
    BuildingsMobilityModel ();
    
    bool IsIndoor (void);
    bool IsOutdoor (void);
    
    void SetIndoor (Ptr<Building> building);
    void SetOutdoor (void);
    
    void SetSurroudingBuilding (Ptr<Building> building);
    
    void SetFloorNumber (uint8_t nfloor);
    void SetRoomNumberX (uint8_t nroomx);
    void SetRoomNumberY (uint8_t nroomy);
    
    void SetStreetCrossingDistence (double d);
    
    uint8_t GetFloorNumber (void);
    uint8_t GetRoomNumberX (void);
    uint8_t GetRoomNumberY (void);
    
    double GetStreetCrossingDistence (void);
    
    Ptr<Building> GetBuilding ();
    
    
    
  private:
    virtual void DoDispose (void);
    virtual Vector DoGetPosition (void) const;
    virtual void DoSetPosition (const Vector &position);
    virtual Vector DoGetVelocity (void) const;
    ConstantVelocityHelper m_helper;
    Box m_bounds;   // bounds of the simulation field (if needed)
    std::list < Ptr<Building> > m_surroudingBuildings;  // buildings blocks
    Ptr<Building> m_myBuilding;
    bool m_indoor;
    uint8_t m_nFloor;
    uint8_t m_roomX;
    uint8_t m_roomY;
    
    double m_streetCrossingDistance; // in meters (used by ITU-R P.1411 for NLOS propagation within street canyons)
};



} // namespace ns3


#endif // BUILDINGS_MOBILITY_MODEL_H