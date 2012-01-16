/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 CTTC
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

#include "buildings-helper.h"

#include <ns3/node-list.h>
#include <ns3/building.h>
#include <ns3/building-list.h>
#include <ns3/mobility-model.h>
#include <ns3/buildings-mobility-model.h>
#include <ns3/abort.h>
#include <ns3/log.h>


NS_LOG_COMPONENT_DEFINE ("BuildingsHelper");

namespace ns3 {

void
BuildingsHelper::MakeAllInstancesConsistent ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for (NodeList::Iterator nit = NodeList::Begin (); nit != NodeList::End (); ++nit)
    {
      Ptr<BuildingsMobilityModel> bmm = (*nit)->GetObject<BuildingsMobilityModel> ();
      NS_ABORT_MSG_UNLESS (0 != bmm, "node " << (*nit)->GetId () << " does not have a BuildingsMobilityModel");
      bool found = false;
      for (BuildingList::Iterator bit = BuildingList::Begin (); bit != BuildingList::End (); ++bit)
	{
	  Vector pos = bmm->GetPosition ();
	  if ((*bit)->IsInside (pos))
	    {
	      NS_LOG_LOGIC ("node " << (*nit)->GetId () << " falls inside building " << (*bit)->GetId ());
	      NS_ABORT_MSG_UNLESS (found == false, "node already inside another building!");		
	      found = true;
	      uint16_t floor = (*bit)->GetFloor (pos);
	      uint16_t roomX = (*bit)->GetRoomX (pos);
	      uint16_t roomY = (*bit)->GetRoomY (pos);	   
	      bmm->SetIndoor (*bit, floor, roomX, roomY);	      
	    }		    	  
	}
      if (!found)
	{
	  NS_LOG_LOGIC ("node " << (*nit)->GetId () << " is outdoor");
	  bmm->SetOutdoor ();
	}
    }
}

} // namespace ns3
