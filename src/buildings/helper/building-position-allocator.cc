/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (C)  2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
#include "building-position-allocator.h"
#include "ns3/random-variable.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/log.h"
#include "ns3/box.h"
#include "ns3/building.h"
#include <cmath>

#include "ns3/building-list.h"

NS_LOG_COMPONENT_DEFINE ("BuildingPositionAllocator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RandomBuildingPositionAllocator);


RandomBuildingPositionAllocator::RandomBuildingPositionAllocator ()
{
}

TypeId
RandomBuildingPositionAllocator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RandomBuildingPositionAllocator")
    .SetParent<PositionAllocator> ()
    .SetGroupName ("Mobility")
    .AddConstructor<RandomBuildingPositionAllocator> ()
    .AddAttribute ("WithReplacement",
                   "If true, the building will be randomly selected with replacement. "
                   "If false, no replacement will occur, until the list of buildings "
                   "to select becomes empty, at which point it will be filled again "
                   "with the list of all buildings.",
                   BooleanValue (false),
                   MakeBooleanAccessor (&RandomBuildingPositionAllocator::m_withReplacement),
                   MakeBooleanChecker ());
  return tid;
}

Vector 
RandomBuildingPositionAllocator::GetNext () const
{
  UniformVariable rand;
  NS_ASSERT_MSG (BuildingList::GetNBuildings () > 0, "no building found");
  Ptr<Building> b;
  if (m_withReplacement)
    {
      uint32_t n = rand.GetInteger (0, BuildingList::GetNBuildings () - 1);
      b = BuildingList::GetBuilding (n);
    }
  else
    {
      if (m_buildingListWithoutReplacement.empty ())
        {
            for (BuildingList::Iterator bit = BuildingList::Begin (); bit != BuildingList::End (); ++bit)
              {
                m_buildingListWithoutReplacement.push_back (*bit);
              }
        }
      uint32_t n = rand.GetInteger (0, m_buildingListWithoutReplacement.size () - 1);
      b = m_buildingListWithoutReplacement.at (n);      
      m_buildingListWithoutReplacement.erase (m_buildingListWithoutReplacement.begin () + n);
    }

  Ptr<RandomBoxPositionAllocator> pa = CreateObject<RandomBoxPositionAllocator> ();
  UniformVariable v;
  BoxValue bv;
  b->GetAttribute ("Boundaries", bv);
  double x = v.GetValue (bv.Get ().xMin, bv.Get ().xMax);
  double y = v.GetValue (bv.Get ().yMin, bv.Get ().yMax);
  double z = v.GetValue (bv.Get ().zMin, bv.Get ().zMax);
  return Vector (x, y, z);
}




NS_OBJECT_ENSURE_REGISTERED (RandomRoomPositionAllocator);


RandomRoomPositionAllocator::RandomRoomPositionAllocator ()
{
}

TypeId
RandomRoomPositionAllocator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RandomRoomPositionAllocator")
    .SetParent<PositionAllocator> ()
    .SetGroupName ("Mobility")
    .AddConstructor<RandomRoomPositionAllocator> ();
  return tid;
}

Vector 
RandomRoomPositionAllocator::GetNext () const
{
  NS_LOG_FUNCTION (this);
  UniformVariable rand;
  NS_ASSERT_MSG (BuildingList::GetNBuildings () > 0, "no building found");
 
  if (m_roomListWithoutReplacement.empty ())
    {
      for (BuildingList::Iterator bit = BuildingList::Begin (); bit != BuildingList::End (); ++bit)
        {
          NS_LOG_LOGIC ("building " << (*bit)->GetId ());
          for (uint32_t rx = 0; rx < (*bit)->GetNRoomsX (); ++rx)
            {
              for (uint32_t ry = 0; ry < (*bit)->GetNRoomsY (); ++ry)
                {
                  for (uint32_t f = 0; f < (*bit)->GetNFloors (); ++f)
                    {
                      RoomInfo i;
                      i.roomx = rx;
                      i.roomy = ry;
                      i.floor = f; 
                      i.b = *bit;
                      NS_LOG_LOGIC ("adding room (" << rx << ", " << ry << ", " << f << ")");
                      m_roomListWithoutReplacement.push_back (i);
                    }
                }
            }
        }
    }
  uint32_t n = rand.GetInteger (0,m_roomListWithoutReplacement.size () - 1);
  RoomInfo r = m_roomListWithoutReplacement.at (n);      
  m_roomListWithoutReplacement.erase (m_roomListWithoutReplacement.begin () + n);  
  NS_LOG_LOGIC ("considering room (" << r.roomx << ", " << r.roomy << ", " << r.floor << ")");

  Ptr<RandomBoxPositionAllocator> pa = CreateObject<RandomBoxPositionAllocator> ();
  UniformVariable v;
  BoxValue bv;
  r.b->GetAttribute ("Boundaries", bv);
  Box box = bv.Get ();
  double rdx =  (box.xMax - box.xMin) / r.b->GetNRoomsX ();
  double rdy =  (box.yMax - box.yMin) / r.b->GetNRoomsY ();
  double rdz =  (box.zMax - box.zMin) / r.b->GetNFloors ();
  double x1 = box.xMin + rdx * r.roomx;
  double x2 = box.xMin + rdx * (r.roomx + 1);
  double y1 = box.yMin + rdy * r.roomy;
  double y2 = box.yMin + rdy * (r.roomy + 1);
  double z1 = box.zMin + rdz * r.floor;
  double z2 = box.zMin + rdz * (r.floor + 1);
  NS_LOG_LOGIC ("randomly allocating position in "
                << " (" << x1 << "," << x2 << ") "
                << "x (" << y1 << "," << y2 << ") "
                << "x (" << z1 << "," << z2 << ") ");

  double x = v.GetValue (x1, x2);
  double y = v.GetValue (y1, y2);
  double z = v.GetValue (z1, z2);
  
  return Vector (x, y, z);
}



} // namespace ns3
