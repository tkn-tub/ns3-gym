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
 * Test for the LTE DL Channel Model
 */

#include <iostream>
#include <vector>
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/log.h>
#include <string>
#include <ns3/mobility-module.h>
#include <ns3/spectrum-helper.h>
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



NS_LOG_COMPONENT_DEFINE ("TestChannelModel");

using namespace ns3;


int main (int argc, char** argv)
{

  LogComponentEnable ("LtePropagationLossModel", LOG_LEVEL_ALL);


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


  // CONFIGURE MOBILITY
  Ptr<ConstantPositionMobilityModel> enbMobility = CreateObject<ConstantPositionMobilityModel> ();
  enbMobility->SetPosition (Vector (0.0, 0.0, 0.0));
  phyEnb->GetDownlinkSpectrumPhy ()->SetMobility (enbMobility);
  phyEnb->GetUplinkSpectrumPhy ()->SetMobility (enbMobility);

  Ptr<ConstantVelocityMobilityModel> ueMobility = CreateObject<ConstantVelocityMobilityModel> ();
  ueMobility->SetPosition (Vector (30.0, 0.0, 0.0));
  ueMobility->SetVelocity (Vector (30.0, 0.0, 0.0));
  phyUe->GetDownlinkSpectrumPhy ()->SetMobility (ueMobility);
  phyUe->GetUplinkSpectrumPhy ()->SetMobility (ueMobility);


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



  // CREATE CHANNEL AND ATTACH DEVICE BY ITS, create also PROPAGATION LOSS MODEL
  Ptr<SingleModelSpectrumChannel> downlinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<SingleModelSpectrumChannel> uplinkChannel = CreateObject<SingleModelSpectrumChannel> ();

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



  /*
   * ******************
   * analyze the propagation loss model
   * ******************
   */


  Ptr<SpectrumValue> txPsd = phyEnb->CreateTxPowerSpectralDensity ();

  Ptr<SpectrumValue> rxPsd = mobility->CalcRxPowerSpectralDensity (txPsd,enbMobility, ueMobility);


  Simulator::Destroy ();

  return 0;
}
