/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro <g.piro@poliba.it>
 */

/*
 * Test for LTE PHY layer in the downlink
 *
 *      /\                                                    +
 *     /--\                                                 __|
 *    /....\                                               |  |
 *   /------\                                              |__|
 *     eNB                                                  UE
 *
 *   SendPacket(Pb)
 *     |
 *     V
 *  |+++++++++++++++++++++++++++++|                |+++++++++++++++++++++++++++++|
 *  |           EnbLtePhy         |                |           EnbLtePhy         |
 *  |+++++++++++++++++++++++++++++|                |+++++++++++++++++++++++++++++|
 *  | SpectrumPhy  | SpectrumPhy  |                | SpectrumPhy  | SpectrumPhy  |
 *  |     dl       |     ul       |                |     dl       |     ul       |
 *  |+++++++++++++++++++++++++++++|                |+++++++++++++++++++++++++++++|
 *     |                                                        |
 *     |                                                        |
 *     V                                                        |
 * StartTx (pb)                                          StartRx(Pb)
 *     |                                                        |
 *     V                                                        |
 *  |+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|
 *  |                         Downlink Spectrum Channel                           |
 *  |+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|
 */

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/log.h>
#include <ns3/mobility-module.h>
#include <ns3/spectrum-helper.h>
#include <ns3/internet-module.h>
#include <ns3/lte-helper.h>
#include <ns3/enb-phy.h>
#include <ns3/ue-phy.h>
#include <ns3/packet-burst.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/constant-velocity-mobility-model.h>

#include <iostream>
#include <string>
#include <vector>


NS_LOG_COMPONENT_DEFINE ("TestSimpleLtePhy");

using namespace ns3;


int main (int argc, char** argv)
{
  LteHelper lte;

  lte.EnableLogComponents ();

  // CREATE NODE CONTAINER AND CREATE LTE NODES
  NodeContainer ueNodes;
  NodeContainer enbNodes;
  ueNodes.Create (1);
  enbNodes.Create (1);


  // CREATE DEVICE CONTAINER, INSTALL DEVICE TO NODE
  NetDeviceContainer ueDevs, enbDevs;
  ueDevs = lte.Install (ueNodes, LteHelper::DEVICE_TYPE_USER_EQUIPMENT);
  enbDevs = lte.Install (enbNodes, LteHelper::DEVICE_TYPE_ENODEB);



  // INSTALL INTERNET STACKS
  InternetStackHelper stack;
  stack.Install (ueNodes);
  stack.Install (enbNodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer UEinterfaces = address.Assign (ueDevs);
  Ipv4InterfaceContainer ENBinterface = address.Assign (enbDevs);



  // MANAGE LTE NET DEVICES
  Ptr<EnbNetDevice> enb;
  enb = enbDevs.Get (0)->GetObject<EnbNetDevice> ();

  Ptr<UeNetDevice> ue = ueDevs.Get (0)->GetObject<UeNetDevice> ();
  lte.RegisterUeToTheEnb (ue, enb);




  // CONFIGURE DL and UL SUB CHANNELS
  // Define a list of sub channels for the downlink
  std::vector<int> dlSubChannels;
  for (int i = 0; i < 25; i++)
    {
      dlSubChannels.push_back (i);
    }
  // Define a list of sub channels for the uplink
  std::vector<int> ulSubChannels;
  for (int i = 50; i < 100; i++)
    {
      ulSubChannels.push_back (i);
    }

  enb->GetPhy ()->SetDownlinkSubChannels (dlSubChannels);
  enb->GetPhy ()->SetUplinkSubChannels (ulSubChannels);

  ue->GetPhy ()->SetDownlinkSubChannels (dlSubChannels);
  ue->GetPhy ()->SetUplinkSubChannels (ulSubChannels);



  // CONFIGURE MOBILITY
  Ptr<ConstantPositionMobilityModel> enbMobility = CreateObject<ConstantPositionMobilityModel> ();
  enbMobility->SetPosition (Vector (0.0, 0.0, 0.0));
  lte.AddMobility (enb->GetPhy (), enbMobility);

  Ptr<ConstantVelocityMobilityModel> ueMobility = CreateObject<ConstantVelocityMobilityModel> ();
  ueMobility->SetPosition (Vector (30.0, 0.0, 0.0));
  ueMobility->SetVelocity (Vector (30.0, 0.0, 0.0));

  lte.AddMobility (ue->GetPhy (), ueMobility);

  lte.AddDownlinkChannelRealization (enbMobility, ueMobility, ue->GetPhy ());





  // ****** simulate a packet transmission in the downlink ******

  Ptr<PacketBurst> pb = CreateObject<PacketBurst> ();
  Ptr<Packet> p1 = Create<Packet> (500);
  Ptr<Packet> p2 = Create<Packet> (500);
  pb->AddPacket (p1);
  pb->AddPacket (p2);



  enb->GetPhy ()->SendPacket (pb);


  Simulator::Stop (Seconds (.1));


  Simulator::Run ();

  Simulator::Destroy ();

  return 0;
}
