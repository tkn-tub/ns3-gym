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
 * Author: Jaume Nin <jnin@cttc.es>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/config-store.h"
#include "ns3/gtk-config-store.h"
#include <ns3/buildings-propagation-loss-model.h>

#include <iomanip>
#include <string>
#include <vector>

using namespace ns3;
using std::vector;

int
main (int argc, char *argv[])
{
  uint32_t nEnbPerFloor = 1;
  uint32_t nUe = 1;
  uint32_t nFloors = 0;
  double simTime = 1.0;
  std::string traceDirectory = "";
  CommandLine cmd;

  cmd.AddValue("nEnb", "Number of eNodeBs per floor", nEnbPerFloor);
  cmd.AddValue("nUe", "Number of UEs", nUe);
  cmd.AddValue("nFloors", "Number of floors, 0 for Friis propagation model",
      nFloors);
  cmd.AddValue("simTime", "Total duration of the simulation (in seconds)",
      simTime);
  cmd.AddValue("traceDirectory",
      "Destination folder where the traces will be stored", traceDirectory);
  cmd.Parse(argc, argv);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults();
  
  // parse again so you can override default values from the command line
  cmd.Parse(argc, argv);

  // Geometry of the scenario (in meters)
  // Assume squared building
  double nodeHeight = 1.5;
  double roomHeight = 3;
  double roomLength = 8;
  uint32_t nRooms = ceil (sqrt (nEnbPerFloor));
  uint32_t nEnb;

  Ptr < LenaHelper > lena = CreateObject<LenaHelper> ();
  //lena->EnableLogComponents ();
  LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
  if (nFloors == 0)
    {
      lena->SetAttribute("PropagationModel",
          StringValue("ns3::FriisPropagationLossModel"));
      nEnb = nEnbPerFloor;
    }
  else
    {
      lena->SetAttribute("PropagationModel",
          StringValue("ns3::BuildingsPropagationLossModel"));
      nEnb = nFloors * nEnbPerFloor;
    }

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  vector < NodeContainer > ueNodes;

  enbNodes.Create(nEnb);
  for (uint32_t i = 0; i < nEnb; i++)
    {
      NodeContainer ueNode;
      ueNode.Create(nUe);
      ueNodes.push_back(ueNode);
    }

  MobilityHelper mobility;
  vector<Vector> enbPosition;
  Ptr < ListPositionAllocator > positionAlloc = CreateObject<ListPositionAllocator> ();
  Ptr < Building > building;

  if (nFloors == 0)
    {
      mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
      // Position of eNBs
      uint32_t plantedEnb = 0;
      for (uint32_t row = 0; row < nRooms; row++)
        {
          for (uint32_t column = 0; column < nRooms && plantedEnb < nEnbPerFloor; column++, plantedEnb++)
            {
              Vector v(roomLength * (column + 0.5), roomLength * (row + 0.5), nodeHeight);
              positionAlloc->Add(v);
              enbPosition.push_back(v);
              mobility.Install(ueNodes[plantedEnb]);
            }
        }
      mobility.SetPositionAllocator(positionAlloc);
      mobility.Install (enbNodes);

      // Position of UEs attached to eNB
     for (uint32_t i = 0; i < nEnb; i++)
       {
         UniformVariable posX(enbPosition[i].x - roomLength * 0.5,
            enbPosition[i].x + roomLength * 0.5);
         UniformVariable posY(enbPosition[i].y - roomLength * 0.5,
            enbPosition[i].y + roomLength * 0.5);
        positionAlloc = CreateObject<ListPositionAllocator> ();
        for (uint32_t j = 0; j < nUe; j++)
          {
            positionAlloc->Add(Vector(posX.GetValue(), posY.GetValue(), nodeHeight));
            mobility.SetPositionAllocator(positionAlloc);
          }
         mobility.Install(ueNodes[i]);
     }

    }
  else
    {
      building = Create<Building> (0.0, nRooms * roomLength,
                                                    0.0, nRooms * roomLength,
                                                    0.0, nFloors* roomHeight);
      building->SetBuildingType(Building::Residential);
      building->SetExtWallsType(Building::ConcreteWithWindows);
      building->SetFloorsNumber(nFloors);
      building->SetNumberRoomX(nRooms);
      building->SetNumberRoomY(nRooms);
      mobility.SetMobilityModel("ns3::BuildingsMobilityModel");
      mobility.Install (enbNodes);
      uint32_t plantedEnb = 0;
      for (uint32_t floor = 0; floor < nFloors; floor++)
        {
          uint32_t plantedEnbPerFloor = 0;
          for (uint32_t row = 0; row < nRooms; row++)
            {
              for (uint32_t column = 0; column < nRooms && plantedEnbPerFloor < nEnbPerFloor; column++, plantedEnb++, plantedEnbPerFloor++)
                {
                  Vector v (roomLength * (column + 0.5),
                            roomLength * (row + 0.5),
                            nodeHeight + roomHeight * floor);
                  positionAlloc->Add(v);
                  enbPosition.push_back(v);
                  Ptr<BuildingsMobilityModel> mmEnb = enbNodes.Get (plantedEnb)->GetObject<BuildingsMobilityModel> ();
                  mmEnb->SetPosition (v);
                  mmEnb->SetIndoor (building);
                  mmEnb->SetFloorNumber (floor);
                  mmEnb->SetRoomNumberX (row);
                  mmEnb->SetRoomNumberY (column);
                  
                  // Positioning UEs attached to eNB
                  mobility.Install(ueNodes[plantedEnb]);
                  for (uint32_t ue = 0; ue < nUe; ue++)
                    {
                      Ptr<BuildingsMobilityModel> mmUe = ueNodes[plantedEnb].Get (ue)->GetObject<BuildingsMobilityModel> ();
                      Vector vUe (v.x, v.y, v.z);
                      mmUe->SetPosition (vUe);
                      mmUe->SetIndoor (building);
                      mmUe->SetFloorNumber (floor);
                      mmUe->SetRoomNumberX (row);
                      mmUe->SetRoomNumberY (column);
                    }
                }
            }
        }
      

    }




  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  vector < NetDeviceContainer > ueDevs;
  enbDevs = lena->InstallEnbDevice(enbNodes);
  for (uint32_t i = 0; i < nEnb; i++)
    {
      NetDeviceContainer ueDev = lena->InstallUeDevice(ueNodes[i]);
      ueDevs.push_back(ueDev);
      lena->Attach(ueDev, enbDevs.Get(i));
      enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
      EpsBearer bearer(q);
      lena->ActivateEpsBearer(ueDev, bearer);
    }

  Simulator::Stop(Seconds(simTime));
  lena->SetTraceDirectory(traceDirectory);
  lena->EnableRlcTraces();
  lena->EnableMacTraces();

  Simulator::Run();

/*  GtkConfigStore config;
  config.ConfigureAttributes ();*/

  Simulator::Destroy();
  return 0;
}
