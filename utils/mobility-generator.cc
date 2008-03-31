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

#include "ns3/ns2-mobility-file-topology.h"
#include "ns3/object.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model-notifier.h"
#include <iostream>

using namespace ns3;

static void 
CourseChange (Ptr<const MobilityModel> position)
{
  Position pos = position->Get ();
  std::cout << Simulator::Now () << ", pos=" << position << ", x=" << pos.x << ", y=" << pos.y
            << ", z=" << pos.z << std::endl;
}

int main (int argc, char *argv[])
{
  std::vector<Ptr<Object> > objects;
  while (argc > 0)
    {
      if (strncmp (*argv, "--n=", strlen ("--n=")) == 0)
	{
	  uint32_t n = atoi (*argv + strlen ("--n="));
	  for (uint32_t i = 0; i < n; i++)
	    {
	      Ptr<MobilityModelNotifier> notifier = CreateObject<MobilityModelNotifier> ();
	      notifier->RegisterListener (MakeCallback (&CourseChange));
	      objects.push_back (notifier);
	    }
	} 
      else if (strncmp (*argv, "--ns2-topology=", 
			strlen ("--ns2-topology=")) == 0)
	{
	  const char *filename = *argv + strlen ("--ns2-topology=");
	  Ns2MobilityFileTopology topology (filename);
	  topology.Layout (objects.begin (), objects.end ());
	}
      argc--;
      argv++;
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
