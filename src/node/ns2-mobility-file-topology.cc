/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ns3/debug.h"
#include "ns3/simulator.h"
#include "ns2-mobility-file-topology.h"
#include "node-list.h"
#include "node.h"
#include "static-speed-mobility-model.h"
#include <fstream>
#include <sstream>

NS_DEBUG_COMPONENT_DEFINE ("Ns2MobileFileTopology");

namespace ns3 {


Ns2MobileFileTopology::Ns2MobileFileTopology (std::string filename)
  : m_filename (filename)
{}


Ptr<StaticSpeedMobilityModel>
Ns2MobileFileTopology::GetMobilityModel (std::string idString, const ObjectStore &store) const
{
  std::istringstream iss;
  iss.str (idString);
  uint32_t id;
  iss >> id;
  Ptr<Object> object = store.Get (id);
  if (object == 0)
    {
      return 0;
    }
  Ptr<StaticSpeedMobilityModel> model = 
    object->QueryInterface<StaticSpeedMobilityModel> (StaticSpeedMobilityModel::iid);
  if (model == 0)
    {
      model = Create<StaticSpeedMobilityModel> ();
      object->AddInterface (model);
    }
  return model;
}

double
Ns2MobileFileTopology::ReadDouble (std::string valueString) const
{
  std::istringstream iss;
  iss.str (valueString);
  double value;
  iss >> value;
  return value;
}

void 
Ns2MobileFileTopology::LayoutObjectStore (const ObjectStore &store) const
{
  std::ifstream file (m_filename.c_str (), std::ios::in);
  if (file.is_open())
    {
      while (!file.eof() )
	{
	  std::string line;
	  getline (file, line);
	  std::string::size_type startNodeId = line.find_first_of ("(");
	  std::string::size_type endNodeId = line.find_first_of (")");
	  if (startNodeId == std::string::npos ||
	      endNodeId == std::string::npos)
	    {
	      continue;
	    }
	  Ptr<StaticSpeedMobilityModel> model = GetMobilityModel (line.substr (startNodeId + 1, 
									       endNodeId - startNodeId), 
								  store);
	  if (model == 0)
	    {
	      continue;
	    }
	  if (startNodeId == 6)
	    {
	      double value = ReadDouble (line.substr (endNodeId + 9, std::string::npos));
	      std::string coordinate = line.substr (endNodeId + 6, 1);
              Position position = model->Get ();
	      if (coordinate == "X")
		{
                  position.x = value;
		  NS_DEBUG ("X=" << value);
		}
	      else if (coordinate == "Y")
		{
                  position.y = value;
		  NS_DEBUG ("Y=" << value);
		}
	      else if (coordinate == "Z")
		{
                  position.z = value;
		  NS_DEBUG ("Z=" << value);
		}
              else
                {
                  continue;
                }
              model->Set (position);
	    }
	  else 
	    {
	      double at = ReadDouble (line.substr (8, startNodeId - 17));
	      std::string::size_type xSpeedEnd = line.find_first_of (" ", endNodeId + 10);
	      std::string::size_type ySpeedEnd = line.find_first_of (" ", xSpeedEnd + 1);
	      double xSpeed = ReadDouble (line.substr (endNodeId + 10, xSpeedEnd - endNodeId - 10));
	      double ySpeed = ReadDouble (line.substr (xSpeedEnd + 1, ySpeedEnd - xSpeedEnd - 1));
	      double zSpeed = ReadDouble (line.substr (ySpeedEnd + 1, std::string::npos));
	      NS_DEBUG ("at=" << at << "xSpeed=" << xSpeed << ", ySpeed=" << ySpeed << ", zSpeed=" << zSpeed);
	      Simulator::Schedule (Seconds (at), &StaticSpeedMobilityModel::SetSpeed, model,
				   Speed (xSpeed, ySpeed, zSpeed));
	    }
	}
      file.close();
    }
}

void 
Ns2MobileFileTopology::Layout (void) const
{
  Layout (NodeList::Begin (), NodeList::End ());
}

} // namespace ns3
