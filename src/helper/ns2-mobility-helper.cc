/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include <fstream>
#include <sstream>
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/node.h"
#include "ns3/static-speed-mobility-model.h"
#include "ns2-mobility-helper.h"

NS_LOG_COMPONENT_DEFINE ("Ns2MobilityHelper");

namespace ns3 {


Ns2MobilityHelper::Ns2MobilityHelper (std::string filename)
  : m_filename (filename)
{}



Ptr<StaticSpeedMobilityModel>
Ns2MobilityHelper::GetMobilityModel (std::string idString, const ObjectStore &store) const
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
  Ptr<StaticSpeedMobilityModel> model = object->GetObject<StaticSpeedMobilityModel> ();
  if (model == 0)
    {
      model = CreateObject<StaticSpeedMobilityModel> ();
      object->AggregateObject (model);
    }
  return model;
}

double
Ns2MobilityHelper::ReadDouble (std::string valueString) const
{
  std::istringstream iss;
  iss.str (valueString);
  double value;
  iss >> value;
  return value;
}

void 
Ns2MobilityHelper::LayoutObjectStore (const ObjectStore &store) const
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
              Vector position = model->GetPosition ();
	      if (coordinate == "X")
		{
                  position.x = value;
		  NS_LOG_DEBUG ("X=" << value);
		}
	      else if (coordinate == "Y")
		{
                  position.y = value;
		  NS_LOG_DEBUG ("Y=" << value);
		}
	      else if (coordinate == "Z")
		{
                  position.z = value;
		  NS_LOG_DEBUG ("Z=" << value);
		}
              else
                {
                  continue;
                }
              model->SetPosition (position);
	    }
	  else 
	    {
	      double at = ReadDouble (line.substr (8, startNodeId - 17));
	      std::string::size_type xSpeedEnd = line.find_first_of (" ", endNodeId + 10);
	      std::string::size_type ySpeedEnd = line.find_first_of (" ", xSpeedEnd + 1);
	      double xSpeed = ReadDouble (line.substr (endNodeId + 10, xSpeedEnd - endNodeId - 10));
	      double ySpeed = ReadDouble (line.substr (xSpeedEnd + 1, ySpeedEnd - xSpeedEnd - 1));
	      double zSpeed = ReadDouble (line.substr (ySpeedEnd + 1, std::string::npos));
	      NS_LOG_DEBUG ("at=" << at << "xSpeed=" << xSpeed << ", ySpeed=" << ySpeed << ", zSpeed=" << zSpeed);
	      Simulator::Schedule (Seconds (at), &StaticSpeedMobilityModel::SetSpeed, model,
				   Vector (xSpeed, ySpeed, zSpeed));
	    }
	}
      file.close();
    }
}

void 
Ns2MobilityHelper::Install (void) const
{
  Install (NodeList::Begin (), NodeList::End ());
}

} // namespace ns3
