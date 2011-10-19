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
//#include "ns3/gtk-config-store.h"

#include <iomanip>
#include <string>
#include <vector>

using namespace ns3;
using std::vector;

int main (int argc, char *argv[])
{
  double radius = 0.0;
  uint32_t nEnb = 1;
  uint32_t nUe  = 1;
  double enbDist = 100;
  CommandLine cmd;

  cmd.AddValue ("nEnb", "Number of eNodeBs", nEnb);
  cmd.AddValue ("nUe", "Number of UEs", nUe);
  cmd.AddValue ("radius", "the radius of the disc where UEs are placed around an eNB", radius);
  cmd.Parse (argc, argv);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  // parse again so you can override default values from the command line
  cmd.Parse (argc, argv);

  // determine the string tag that identifies this simulation run
  // this tag is then appended to all filenames

  IntegerValue runValue;
  GlobalValue::GetValueByName ("RngRun", runValue);

//  std::ostringstream tag;
/*  tag  << "_enbDist" << std::setw(3) << std::setfill ('0') << std::fixed << std::setprecision (0) << enbDist
       << "_radius"  << std::setw(3) << std::setfill ('0') << std::fixed << std::setprecision (0) << radius
       << "_nUe"  << std::setw(3) << std::setfill('0')  << nUe
       << "_rngRun"  << std::setw(3) << std::setfill('0')  << runValue.Get () ;
*/
  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  lena->EnableLogComponents ();

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  vector<NodeContainer> ueNodes;

  enbNodes.Create (nEnb);
  for (uint32_t i = 0; i < nEnb; i++)
    {
      NodeContainer ueNode;
      ueNode.Create (nUe);
      ueNodes.push_back (ueNode);
    }

  // Position of eNBs
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint32_t i = 0; i < nEnb; i++)
    {
      positionAlloc->Add (Vector (enbDist * i, enbDist * i, 0.0));
    }

  MobilityHelper enbMobility;
  enbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator (positionAlloc);
  enbMobility.Install (enbNodes);

  // Position of UEs attached to eNB
  vector<MobilityHelper> ueMobility;
  for (uint32_t i = 0; i < nEnb; i++)
    {
      MobilityHelper ueMob;
      ueMob.SetPositionAllocator ("ns3::UniformDiscPositionAllocator",
                                  "X", DoubleValue (enbDist * i),
                                  "Y", DoubleValue (enbDist * i),
                                  "rho", DoubleValue (radius));
      ueMob.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      ueMobility.push_back (ueMob);
      ueMobility[i].Install (ueNodes[i]);
    }

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  vector<NetDeviceContainer> ueDevs;
  //NetDeviceContainer ueDevs2;
  enbDevs = lena->InstallEnbDevice (enbNodes);
  for (uint32_t i = 0; i < nEnb; i++)
    {
      NetDeviceContainer ueDev = lena->InstallUeDevice (ueNodes[i]);
      ueDevs.push_back (ueDev);
      lena->Attach (ueDev, enbDevs.Get (i));
      enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
      EpsBearer bearer (q);
      lena->ActivateEpsBearer (ueDev, bearer, LteTft::Default ());


    }

  Simulator::Stop (Seconds (60));
  //lena->EnableRlcTraces ();

  Simulator::Run ();

  /*GtkConfigStore config;
  config.ConfigureAttributes ();*/

  Simulator::Destroy ();
  return 0;
}
