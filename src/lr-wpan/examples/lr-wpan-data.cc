/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 */

/*
 * Try to send data end-to-end through a LrWpanMac <-> LrWpanPhy <->
 * SpectrumChannel <-> LrWpanPhy <-> LrWpanMac chain
 */

#include "ns3/core-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/simulator.h"
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/constant-position-mobility-model.h>

#include <iostream>

using namespace ns3;

static void DataIndication (Ptr<Packet> p)
{
  NS_LOG_UNCOND ("Received packet of size " << p->GetSize ());
}

static void DataConfirm (LrWpanMcpsDataConfirmStatus status)
{
  NS_LOG_UNCOND ("LrWpanMcpsDataConfirmStatus = " << status);
}

int main (int argc, char *argv[])
{
  LrWpanHelper lrWpanHelper;
  lrWpanHelper.EnableLogComponents ();

  Ptr<LrWpanMac> mac0 = CreateObject<LrWpanMac> ();
  Ptr<LrWpanMac> mac1 = CreateObject<LrWpanMac> ();
  Ptr<LrWpanPhy> phy0 = CreateObject<LrWpanPhy> ();
  Ptr<LrWpanPhy> phy1 = CreateObject<LrWpanPhy> ();

  phy0->SetPdDataIndicationCallback (MakeCallback (&LrWpanMac::PdDataIndication, mac0));
  phy0->SetPdDataConfirmCallback (MakeCallback (&LrWpanMac::PdDataConfirm, mac0));
  phy0->SetPlmeEdConfirmCallback (MakeCallback (&LrWpanMac::PlmeEdConfirm, mac0));
  phy0->SetPlmeGetAttributeConfirmCallback (MakeCallback (&LrWpanMac::PlmeGetAttributeConfirm, mac0));
  phy0->SetPlmeSetTRXStateConfirmCallback (MakeCallback (&LrWpanMac::PlmeSetTRXStateConfirm, mac0));
  phy0->SetPlmeSetAttributeConfirmCallback (MakeCallback (&LrWpanMac::PlmeSetAttributeConfirm, mac0));

  phy1->SetPdDataIndicationCallback (MakeCallback (&LrWpanMac::PdDataIndication, mac1));
  phy1->SetPdDataConfirmCallback (MakeCallback (&LrWpanMac::PdDataConfirm, mac1));
  phy1->SetPlmeEdConfirmCallback (MakeCallback (&LrWpanMac::PlmeEdConfirm, mac1));
  phy1->SetPlmeGetAttributeConfirmCallback (MakeCallback (&LrWpanMac::PlmeGetAttributeConfirm, mac1));
  phy1->SetPlmeSetTRXStateConfirmCallback (MakeCallback (&LrWpanMac::PlmeSetTRXStateConfirm, mac1));
  phy1->SetPlmeSetAttributeConfirmCallback (MakeCallback (&LrWpanMac::PlmeSetAttributeConfirm, mac1));

  mac0->SetPhy (phy0);
  mac1->SetPhy (phy1);

  Ptr<LrWpanCsmaCa> csmaca0 = CreateObject<LrWpanCsmaCa> ();
  mac0->SetCsmaCa (csmaca0);
  csmaca0->SetMac (mac0);
  Ptr<LrWpanCsmaCa> csmaca1 = CreateObject<LrWpanCsmaCa> ();
  mac1->SetCsmaCa (csmaca1);
  csmaca1->SetMac (mac1);

  csmaca0->SetLrWpanMacStateCallback (MakeCallback (&LrWpanMac::SetLrWpanMacState, mac0));
  csmaca1->SetLrWpanMacStateCallback (MakeCallback (&LrWpanMac::SetLrWpanMacState, mac1));

  phy0->SetPlmeCcaConfirmCallback (MakeCallback (&LrWpanCsmaCa::PlmeCcaConfirm, csmaca0));
  phy1->SetPlmeCcaConfirmCallback (MakeCallback (&LrWpanCsmaCa::PlmeCcaConfirm, csmaca1));

  phy0->SetPdDataIndicationCallback (MakeCallback (&LrWpanMac::PdDataIndication, mac0));
  phy1->SetPdDataIndicationCallback (MakeCallback (&LrWpanMac::PdDataIndication, mac1));
  Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel> ();

  phy0->SetChannel (channel);
  phy1->SetChannel (channel);
  channel->AddRx (phy0);
  channel->AddRx (phy1);

  // CONFIGURE MOBILITY
  Ptr<ConstantPositionMobilityModel> senderMobility = CreateObject<ConstantPositionMobilityModel> ();
  phy0->SetMobility (senderMobility);
  Ptr<ConstantPositionMobilityModel> receiverMobility = CreateObject<ConstantPositionMobilityModel> ();
  phy1->SetMobility (receiverMobility);

  McpsDataConfirmCallback cb0;
  cb0 = MakeCallback (&DataConfirm);
  mac0->SetMcpsDataConfirmCallback (cb0);

  McpsDataIndicationCallback cb1;
  cb1 = MakeCallback (&DataIndication);
  mac1->SetMcpsDataIndicationCallback (cb1);

  Ptr<Packet> p = Create<Packet> (20);  // 20 bytes of dummy data

  // The below should trigger two callbacks when end-to-end data is working
  // 1) DataConfirm callback is called
  // 2) DataIndication callback is called with value of 20
  mac0->McpsDataRequest (p);

  Simulator::Run ();

  // Break reference cycles
  phy0->Dispose ();
  phy1->Dispose ();
  mac0->Dispose ();
  mac1->Dispose ();
  // Clean up allocated memory
  senderMobility = receiverMobility = 0;
  channel = 0;
  csmaca0 = csmaca1 = 0;
  phy0 = phy1 = 0;
  mac0 = mac1 = 0;
  Simulator::Destroy ();
  return 0;

}
