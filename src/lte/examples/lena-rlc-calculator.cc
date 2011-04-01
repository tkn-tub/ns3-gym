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
 */


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/rlc-stats-calculator.h"


//#include "ns3/gtk-config-store.h"

using namespace ns3;

void TxPduCallback(Ptr<RlcStatsCalculator> rlcStats, std::string path,
                   uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  rlcStats->TxPdu(rnti, lcid, packetSize);
}

void RxPduCallback(Ptr<RlcStatsCalculator> rlcStats, std::string path,
                   uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  rlcStats->RxPdu(rnti, lcid, packetSize, delay);
}

int main (int argc, char *argv[])
{
  LenaHelper lena;

  // Command line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Enable LTE log components
  lena.EnableLogComponents ();

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (3);

  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  enbDevs = lena.InstallEnbDevice (enbNodes);
  ueDevs = lena.InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  lena.Attach (ueDevs, enbDevs.Get (0));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena.ActivateEpsBearer (ueDevs, bearer);

  Simulator::Stop (Seconds (0.1));

  // Insert RLC Performance Calculator
  Ptr<RlcStatsCalculator> rlcStats = CreateObject<RlcStatsCalculator> ();
  Config::Connect("/NodeList/0/DeviceList/0/LteEnbRrc/UeMap/*/RadioBearerMap/*/LteRlc/TxPDU",
                   MakeBoundCallback(&TxPduCallback, rlcStats));
  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbRrc/UeMap/*/RadioBearerMap/*/LteRlc/RxPDU",
                   MakeBoundCallback(&RxPduCallback, rlcStats));

  Simulator::Run ();

  // Uncomment to show available paths
  //GtkConfigStore config;
  //config.ConfigureAttributes ();

  Simulator::Destroy ();

  // Generate RLC output

  return 0;
}
