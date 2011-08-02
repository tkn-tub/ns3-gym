/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#include <iostream>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/log.h>
#include <string>
#include <ns3/spectrum-helper.h>
#include <ns3/lte-helper.h>
#include <ns3/enb-phy.h>
#include <ns3/ue-phy.h>
#include <ns3/packet-burst.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/constant-velocity-mobility-model.h>
#include <vector>
#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/test.h"
#include "ns3/uinteger.h"
#include <ns3/simulator.h>


using namespace ns3;

/*
 * Test the LTE physical layer: Test that ENB and UE Phys can transmit a packet
 */
class Ns3LtePhyTestCase : public TestCase
{
public:
  Ns3LtePhyTestCase ();
  virtual ~Ns3LtePhyTestCase ();

private:
  virtual void DoRun (void);

};

Ns3LtePhyTestCase::Ns3LtePhyTestCase ()
  : TestCase ("Test that ENB and UE Phys can transmit a packet.")
{
}

Ns3LtePhyTestCase::~Ns3LtePhyTestCase ()
{
}

void
Ns3LtePhyTestCase::DoRun (void)
{
  LteHelper lte;

  //lte.EnableLogComponents ();

  // CREATE NODE CONTAINER AND CREATE LTE NODES
  NodeContainer ueNodes;
  NodeContainer enbNodes;
  ueNodes.Create (1);
  enbNodes.Create (1);


  // CREATE DEVICE CONTAINER, INSTALL DEVICE TO NODE
  NetDeviceContainer ueDevs, enbDevs;
  ueDevs = lte.Install (ueNodes, LteHelper::DEVICE_TYPE_USER_EQUIPMENT);
  enbDevs = lte.Install (enbNodes, LteHelper::DEVICE_TYPE_ENODEB);

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
  ueMobility->SetPosition (Vector (50.0, 50.0, 0.0));
  ueMobility->SetVelocity (Vector (50.0, 50.0, 0.0));

  lte.AddMobility (ue->GetPhy (), ueMobility);

  lte.AddDownlinkChannelRealization (enbMobility, ueMobility, ue->GetPhy ());




  // ****** simulate a packet transmission in the downlink ******

  Ptr<PacketBurst> pb = Create<PacketBurst> ();
  Ptr<Packet> p1 = Create<Packet> (500);
  Ptr<Packet> p2 = Create<Packet> (500);
  pb->AddPacket (p1);
  pb->AddPacket (p2);


  NS_TEST_ASSERT_MSG_EQ (enb->GetPhy ()->SendPacket (pb), false, "SendPacket() should return false for eNB device.");


  NS_TEST_ASSERT_MSG_EQ (ue->GetPhy ()->SendPacket (pb), false, "SendPacket() should return false for ue device.");

  Simulator::Destroy ();
}
// ==============================================================================

class Ns3LtePhyTestTestSuite : public TestSuite
{
public:
  Ns3LtePhyTestTestSuite ();
};

Ns3LtePhyTestTestSuite::Ns3LtePhyTestTestSuite ()
  : TestSuite ("lte-phy", UNIT)
{
  AddTestCase (new Ns3LtePhyTestCase);
}

static Ns3LtePhyTestTestSuite ns3LtePhyTestTestSuite;
