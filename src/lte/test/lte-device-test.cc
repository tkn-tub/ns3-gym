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
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ue-manager.h"
#include "ns3/ue-record.h"
#include "ns3/lte-mac-header.h"


using namespace ns3;

/*
 * Test the LTE Device: Test that the LTE device can send a packet
 */
class Ns3LteDeviceTestCase : public TestCase
{
public:
  Ns3LteDeviceTestCase ();
  virtual ~Ns3LteDeviceTestCase ();

private:
  virtual void DoRun (void);

};

Ns3LteDeviceTestCase::Ns3LteDeviceTestCase ()
  : TestCase ("Test that the LTE device can send a packet.")
{
}

Ns3LteDeviceTestCase::~Ns3LteDeviceTestCase ()
{
}

void
Ns3LteDeviceTestCase::DoRun (void)
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
  ueMobility->SetPosition (Vector (50.0, 50.0, 0.0));
  ueMobility->SetVelocity (Vector (50.0, 50.0, 0.0));

  lte.AddMobility (ue->GetPhy (), ueMobility);

  lte.AddDownlinkChannelRealization (enbMobility, ueMobility, ue->GetPhy ());






  // ****** test if the ue is registered with the eNB ******

  NS_TEST_ASSERT_MSG_EQ (enb->GetUeManager ()->IsRegistered (ue), true, "The ue was not registered with the eNB.");

  // ****** test packet transmission******

  Ptr<PacketBurst> pb = Create<PacketBurst> ();
  Ptr<Packet> p = Create<Packet> (1);

  Mac48Address from = Mac48Address::ConvertFrom (enb->GetAddress ());

  Mac48Address to = Mac48Address::ConvertFrom (enb->GetAddress ());

  LteMacHeader header;
  header.SetSource (from);
  header.SetDestination (to);
  p->AddHeader (header);

  pb->AddPacket (p);
  enb->SetPacketToSend (pb);

  enb->GetPhy ()->SendPacket (enb->GetPacketToSend ());

  Simulator::Stop (Seconds (.01));
  Simulator::Run ();

  Simulator::Destroy ();
}
// ==============================================================================

class Ns3LteDeviceTestTestSuite : public TestSuite
{
public:
  Ns3LteDeviceTestTestSuite ();
};

Ns3LteDeviceTestTestSuite::Ns3LteDeviceTestTestSuite ()
  : TestSuite ("lte-device", UNIT)
{
  AddTestCase (new Ns3LteDeviceTestCase);
}

static Ns3LteDeviceTestTestSuite ns3LteDeviceTestTestSuite;
