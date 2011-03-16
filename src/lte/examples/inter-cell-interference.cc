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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 */


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  double enbDist = 5.0;
  
  CommandLine cmd;
  cmd.AddValue ("enbDist", "distance between the two eNBs", enbDist);
  

  LenaHelper lena;

  //lena.EnableLogComponents ();

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes1, ueNodes2;
  enbNodes.Create (2);
  ueNodes1.Create (1);
  ueNodes2.Create (1);

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (enbDist, 0.0, 0.0));
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (enbDist, 0.0, 0.0));
  


  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (enbNodes);
  mobility.Install (ueNodes1);
  mobility.Install (ueNodes2);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs1;
  NetDeviceContainer ueDevs2;
  enbDevs = lena.InstallEnbDevice (enbNodes);
  ueDevs1 = lena.InstallUeDevice (ueNodes1);
  ueDevs2 = lena.InstallUeDevice (ueNodes2);

  // Attach UEs to a eNB
  lena.Attach (ueDevs1, enbDevs.Get (0));
  lena.Attach (ueDevs2, enbDevs.Get (1));

  // Activate an EPS bearer on all UEs
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena.ActivateEpsBearer (ueDevs1, bearer);
  lena.ActivateEpsBearer (ueDevs2, bearer);

  Simulator::Stop (Seconds (0.004));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
