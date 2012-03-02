/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include <ns3/lte-module.h>
#include <ns3/config-store.h>
#include <ns3/buildings-module.h>
#include <ns3/radio-environment-map-helper.h>
#include <ns3/log.h>
#include <iomanip>
#include <string>
#include <vector>
#include <ns3/gtk-config-store.h>

// The topology of this simulation program is inspired from 
// 3GPP R4-092042, Section 4.2.1 Dual Stripe Model
// note that the term "apartments" used in that document matches with
// the term "room" used in the BuildingsMobilityModel 

using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("LenaDualStripe");

int
main (int argc, char *argv[])
{
  // scenario parameters
  uint32_t nBlocks = 2;
  uint32_t nApartamentsX = 10;
  uint32_t nApartamentsY = 2;
  uint32_t nFloors = 1;
  uint32_t heightPerFloor = 3;
  double deploymentRatio = 0.4;
  double ueHenbRatio = 10;

  CommandLine cmd;
  cmd.Parse (argc, argv);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  cmd.Parse (argc, argv);

  

  Ptr<GridBuildingAllocator>  gridBuildingAllocator = CreateObject<GridBuildingAllocator> ();
  gridBuildingAllocator->SetAttribute ("GridWidth", UintegerValue (1));
  gridBuildingAllocator->SetAttribute ("MinX", DoubleValue (0));
  gridBuildingAllocator->SetAttribute ("MinY", DoubleValue (0));
  gridBuildingAllocator->SetAttribute ("LengthX", DoubleValue (10*nApartamentsX));
  gridBuildingAllocator->SetAttribute ("LengthY", DoubleValue (10*nApartamentsY));
  gridBuildingAllocator->SetAttribute ("DeltaX", DoubleValue (10));
  gridBuildingAllocator->SetAttribute ("DeltaY", DoubleValue (10));
  gridBuildingAllocator->SetAttribute ("Height", DoubleValue (heightPerFloor*nFloors));
  gridBuildingAllocator->SetBuildingAttribute ("NRoomsX", UintegerValue (nApartamentsX));
  gridBuildingAllocator->SetBuildingAttribute ("NRoomsY", UintegerValue (nApartamentsY));
  gridBuildingAllocator->SetBuildingAttribute ("NFloors", UintegerValue (nFloors));
  gridBuildingAllocator->SetBuildingAttribute ("ExternalWallsType", EnumValue (Building::StoneBlocks));
  gridBuildingAllocator->Create (nBlocks);

  uint32_t nHenbs = round (nApartamentsX * nApartamentsY * nBlocks * nFloors * deploymentRatio);
  NS_LOG_LOGIC ("nHenbs = " << nHenbs);
  uint32_t nUes = round (nHenbs*ueHenbRatio);
  NS_LOG_LOGIC ("nUes = " << nUes);
  
  NodeContainer henbs;
  henbs.Create (nHenbs);
  NodeContainer macroEnbs;
  macroEnbs.Create (3);
  NodeContainer ues;
  ues.Create (nUes);

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");
  mobility.EnableAsciiAll (std::cout);

  // HeNBs placed indoor
  Ptr<PositionAllocator> positionAlloc = CreateObject<RandomRoomPositionAllocator> ();
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (henbs);

  // Macro eNB placed at fixed coordinates
  Ptr<ListPositionAllocator> listPositionAlloc = CreateObject<ListPositionAllocator> ();
  listPositionAlloc->Add (Vector (-50, 0, 30));
  listPositionAlloc->Add (Vector (-50.01, 0.01, 30));
  listPositionAlloc->Add (Vector (-50.01, -0.01, 30));
  mobility.SetPositionAllocator (listPositionAlloc);
  mobility.Install (macroEnbs);

  // UEs randomly located in the whole simulation area
  double xmin = -100;
  double xmax = nApartamentsX*10+10;
  double ymin = -50;
  double ymax = (nApartamentsY*10+10)*nBlocks;
  double zmin = 0;
  double zmax = heightPerFloor*nFloors;
  NS_LOG_LOGIC ("randomly allocating users in "
                << " (" << xmin << "," << xmax << ") "
                << "x (" << ymin << "," << ymax << ") "
                << "x (" << zmin << "," << zmax << ") ");
  positionAlloc = CreateObject<RandomBoxPositionAllocator> ();
  positionAlloc->SetAttribute ("X", RandomVariableValue (UniformVariable (xmin, xmax)));
  positionAlloc->SetAttribute ("Y", RandomVariableValue (UniformVariable (ymin, ymax)));
  positionAlloc->SetAttribute ("Z", RandomVariableValue (UniformVariable (zmin, zmax)));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (ues);

  

  Ptr <LteHelper> lteHelper = CreateObject<LteHelper> ();
  // //lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FriisPropagationLossModel"));
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaExtWalls", DoubleValue (0));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaOutdoor", DoubleValue (1));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaIndoor", DoubleValue (1.5));

  
  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;

  // power setting in dBm for HeNBs
  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (10.0));
  enbDevs.Add (lteHelper->InstallEnbDevice (henbs));


  // power setting in dBm for macro eNB
  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (30.0));
  lteHelper->SetEnbAntennaModelType ("ns3::CosineAntennaModel");
  lteHelper->SetEnbAntennaModelAttribute ("Beamwidth",   DoubleValue (65));
  lteHelper->SetEnbAntennaModelAttribute ("MaxGain",     DoubleValue (0.0));
  lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (0));
  enbDevs.Add (lteHelper->InstallEnbDevice (macroEnbs.Get (0)));
  lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (+120));
  enbDevs.Add (lteHelper->InstallEnbDevice (macroEnbs.Get (1)));
  lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (-120));
  enbDevs.Add (lteHelper->InstallEnbDevice (macroEnbs.Get (2)));

  ueDevs = lteHelper->InstallUeDevice (ues);
  lteHelper->AttachToClosestEnb (ueDevs, enbDevs);
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateEpsBearer (ueDevs, bearer, EpcTft::Default ());


  BuildingsHelper::MakeMobilityModelConsistent ();

  // by default, simulation will anyway stop right after the REM has been generated
  Simulator::Stop (Seconds (0.0069));  

  Ptr<RadioEnvironmentMapHelper> remHelper = CreateObject<RadioEnvironmentMapHelper> ();
  remHelper->SetAttribute ("ChannelPath", StringValue ("/ChannelList/0"));
  remHelper->SetAttribute ("OutputFile", StringValue ("lena-dual-stripe.rem"));
  remHelper->SetAttribute ("XMin", DoubleValue (-70));
  remHelper->SetAttribute ("XMax", DoubleValue (xmax));
  remHelper->SetAttribute ("YMin", DoubleValue (-10));
  remHelper->SetAttribute ("YMax", DoubleValue (ymax));
  remHelper->SetAttribute ("Z", DoubleValue (1.5));
  remHelper->Install ();

  Simulator::Run ();

  //GtkConfigStore config;
  //config.ConfigureAttributes ();

  lteHelper = 0;
  Simulator::Destroy ();
  return 0;
}
