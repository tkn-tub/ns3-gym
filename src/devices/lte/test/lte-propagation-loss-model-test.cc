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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#include <iostream>
#include <ns3/log.h>
#include <string>
#include <vector>
#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/test.h"
#include "ns3/uinteger.h"
#include <ns3/simulator.h>
#include <ns3/lte-helper.h>
#include <ns3/enb-phy.h>
#include <ns3/ue-phy.h>
#include <ns3/packet-burst.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/constant-velocity-mobility-model.h>
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/lte-spectrum-phy.h"
#include "ns3/enb-lte-spectrum-phy.h"
#include "ns3/ue-lte-spectrum-phy.h"
#include "ns3/ue-net-device.h"
#include "ns3/enb-net-device.h"
#include "ns3/ue-manager.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/lte-propagation-loss-model.h"

using namespace ns3;

/*
 * Test the LTE physical layer.
 */
class Ns3LtePropagationLossModelTestCase : public TestCase
{
public:
  Ns3LtePropagationLossModelTestCase ();
  virtual ~Ns3LtePropagationLossModelTestCase ();

private:
  virtual void DoRun (void);

};

Ns3LtePropagationLossModelTestCase::Ns3LtePropagationLossModelTestCase ()
  : TestCase ("Test the LTE propagation loss model")
{
}

Ns3LtePropagationLossModelTestCase::~Ns3LtePropagationLossModelTestCase ()
{
}

void
Ns3LtePropagationLossModelTestCase::DoRun (void)
{
  // CREATE PHY LAYER FOR BOTH UE AND ENB
  Ptr<EnbLtePhy> phyEnb = CreateObject<EnbLtePhy> ();
  Ptr<EnbLteSpectrumPhy> dlEnb = CreateObject<EnbLteSpectrumPhy> ();
  Ptr<EnbLteSpectrumPhy> ulEnb = CreateObject<EnbLteSpectrumPhy> ();
  phyEnb->SetDownlinkSpectrumPhy (dlEnb);
  phyEnb->SetUplinkSpectrumPhy (ulEnb);
  phyEnb->SetTxPower (43);

  Ptr<UeLtePhy> phyUe = CreateObject<UeLtePhy> ();
  Ptr<UeLteSpectrumPhy> dlUe = CreateObject<UeLteSpectrumPhy> ();
  Ptr<UeLteSpectrumPhy> ulUe = CreateObject<UeLteSpectrumPhy> ();
  phyUe->SetDownlinkSpectrumPhy (dlUe);
  phyUe->SetUplinkSpectrumPhy (ulUe);


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

  phyEnb->SetDownlinkSubChannels (dlSubChannels);
  phyEnb->SetUplinkSubChannels (ulSubChannels);

  phyUe->SetDownlinkSubChannels (dlSubChannels);
  phyUe->SetUplinkSubChannels (ulSubChannels);


  // CONFIGURE MOBILITY
  Ptr<ConstantPositionMobilityModel> enbMobility = CreateObject<ConstantPositionMobilityModel> ();
  enbMobility->SetPosition (Vector (0.0, 0.0, 0.0));
  phyEnb->GetDownlinkSpectrumPhy ()->SetMobility (enbMobility);
  phyEnb->GetUplinkSpectrumPhy ()->SetMobility (enbMobility);

  Ptr<ConstantVelocityMobilityModel> ueMobility = CreateObject<ConstantVelocityMobilityModel> ();
  ueMobility->SetPosition (Vector (50.0, 50.0, 0.0));
  ueMobility->SetVelocity (Vector (50.0, 50.0, 0.0));
  phyUe->GetDownlinkSpectrumPhy ()->SetMobility (ueMobility);
  phyUe->GetUplinkSpectrumPhy ()->SetMobility (ueMobility);




  // CREATE CHANNEL AND ATTACH DEVICE BY ITS, create also PROPAGATION LOSS MODEL
  Ptr<SingleModelSpectrumChannel> downlinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<SingleModelSpectrumChannel> uplinkChannel =  CreateObject<SingleModelSpectrumChannel> ();

  dlUe->SetChannel (downlinkChannel);
  ulUe->SetChannel (uplinkChannel);
  downlinkChannel->AddRx (dlUe);

  dlEnb->SetChannel (downlinkChannel);
  ulEnb->SetChannel (uplinkChannel);
  downlinkChannel->AddRx (dlEnb);
  uplinkChannel->AddRx (ulEnb);

  Ptr<LtePropagationLossModel> mobility = CreateObject<LtePropagationLossModel> ();
  downlinkChannel->AddSpectrumPropagationLossModel (mobility->GetObject<SpectrumPropagationLossModel> ());

  mobility->CreateChannelRealization (enbMobility, ueMobility);
  // initialize multipath model
  Ptr<JakesFadingLossModel> m = mobility->GetChannelRealization (enbMobility, ueMobility)->GetJakesFadingLossModel ();
  m->SetPhy (phyUe);




  //analyze the propagation loss model
  Ptr<SpectrumValue> txPsd = phyEnb->CreateTxPowerSpectralDensity ();
  Ptr<SpectrumValue> rxPsd = mobility->CalcRxPowerSpectralDensity (txPsd,enbMobility, ueMobility);
  std::vector<double> tx, rx;
  for (Values::const_iterator it = txPsd->ConstValuesBegin ();
       it != txPsd->ConstValuesEnd (); it++ )
    {
      double tx_ = (*it);
      tx.push_back (tx_);
    }

  for (Values::const_iterator it = rxPsd->ConstValuesBegin ();
       it != rxPsd->ConstValuesEnd (); it++ )
    {
      double rx_ = (*it);
      rx.push_back (rx_);
    }


  NS_TEST_ASSERT_MSG_EQ (tx.size (), rx.size (), "Sizes of tx and rx don't agree.");

  int nbOfValues = tx.size ();
  for (int i = 0; i < nbOfValues; i++)
    {
      NS_TEST_ASSERT_MSG_EQ (tx.at (i) == 0 && rx.at (i) != 0, false, "Problem with elements of tx and rx.");
      NS_TEST_ASSERT_MSG_EQ (tx.at (i) != 0 && (tx.at (i) <= rx.at (i)), false, "Problem with elements of tx and rx.");
    }


  Simulator::Destroy ();
}
// ==============================================================================

class Ns3LtePropagationLossModelTestTestSuite : public TestSuite
{
public:
  Ns3LtePropagationLossModelTestTestSuite ();
};

Ns3LtePropagationLossModelTestTestSuite::Ns3LtePropagationLossModelTestTestSuite ()
  : TestSuite ("lte-propagation-loss-model", UNIT)
{
  AddTestCase (new Ns3LtePropagationLossModelTestCase);
}

static Ns3LtePropagationLossModelTestTestSuite ns3LtePropagationLossModelTestTestSuite;
