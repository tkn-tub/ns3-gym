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
* Author: Marco Miozzo <marco.miozzo@cttc.es>
*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>
#include <ns3/hybrid-buildings-propagation-loss-model.h>

#include <iomanip>
#include <string>
#include <vector>

using namespace ns3;
using std::vector;

int
main (int argc, char *argv[])
{
  
  double hEnb = 30.0;
  double hUe = 1.0;
  bool enbIndoor = false;
  bool ueIndoor = false;
  CommandLine cmd;
  
  cmd.AddValue("hEnb", "Height of the eNB", hEnb);
  cmd.AddValue("hUe", "Height of UE", hUe);
  cmd.AddValue("enbIndoor", "Boolean for eNB Indoor/Outdoor selection",               enbIndoor);
  cmd.AddValue("ueIndoor", "Boolean for UE Indoor/Outdoor selection",               ueIndoor);
  cmd.Parse(argc, argv);
  
  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();
  
  
  // parse again so you can override default values from the command line
  cmd.Parse (argc, argv);
  
  
  std::ofstream outFile;
  outFile.open ("buildings-pathloss-profiler.out");
  if (!outFile.is_open ())
  {
    NS_FATAL_ERROR ("Can't open output file");
  }
  
  Ptr<BuildingsMobilityModel> mmEnb = CreateObject<BuildingsMobilityModel> ();
  mmEnb->SetPosition (Vector (0.0, 0.0, hEnb));
  if (enbIndoor)
    {
      Ptr<Building> building1 = Create<Building> (-2, 2, -2, 2, 0.0, 20.0);
      building1->SetBuildingType (Building::Residential);
      building1->SetExtWallsType (Building::ConcreteWithWindows);
    }

  BuildingsHelper::MakeConsistent (mmEnb);
  
  Ptr<HybridBuildingsPropagationLossModel> propagationLossModel = CreateObject<HybridBuildingsPropagationLossModel> ();
  // cancel shadowing effect
  propagationLossModel->SetAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  propagationLossModel->SetAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  propagationLossModel->SetAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));
  
//   propagationLossModel->SetAttribute ("Los2NlosThr", DoubleValue (1550.0));
  
  //for (uint8_t i = 0; i < 23; i++)
  for (uint32_t i = 1; i < 2300; i++)
    {
      Ptr<BuildingsMobilityModel> mmUe = CreateObject<BuildingsMobilityModel> ();
      mmUe->SetPosition (Vector (i, 0.0, hUe));      
      BuildingsHelper::MakeConsistent (mmUe);
      double loss = propagationLossModel->GetLoss (mmEnb, mmUe);
      outFile << i << "\t"
              << loss 
              << std::endl;
    }
    
  Simulator::Destroy ();
  
}
