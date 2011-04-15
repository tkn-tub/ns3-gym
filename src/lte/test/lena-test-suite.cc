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

#include "ns3/test.h"

#include "ns3/lena-test-downlink-sinr.h"


using namespace ns3;


// class LenaTestCase : public TestCase
// {
//   public:
//     LenaTestCase ();
//     virtual ~LenaTestCase ();
// 
//   private:
//     virtual void DoRun (void);
// };
// 
// LenaTestCase::LenaTestCase ()
//   : TestCase ("Test identical (with tolerance) throughput of two UEs the same SINR")
// {
// }
// 
// LenaTestCase::~LenaTestCase ()
// {
// }
// 
// 
// void
// LenaTestCase::DoRun (void)
// {
// //   LenaHelper lena;
// //   lena.EnableLogComponents ();
// // 
// //   // Create Nodes: eNodeB and UE
// //   NodeContainer enbNodes;
// //   NodeContainer ueNodes;
// //   enbNodes.Create (1);
// //   ueNodes.Create (1);
// // 
// //   // Install Mobility Model
// //   MobilityHelper mobility;
// //   mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
// //   mobility.Install (enbNodes);
// // //  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
// //   mobility.Install (ueNodes);
// // 
// //   // Create Devices and install them in the Nodes (eNB and UE)
// //   NetDeviceContainer enbDevs;
// //   NetDeviceContainer ueDevs;
// //   enbDevs = lena.InstallEnbDevice (enbNodes);
// //   ueDevs = lena.InstallUeDevice (ueNodes);
// // 
// //   // Attach a UE to a eNB
// //   lena.Attach (ueDevs, enbDevs.Get (0));
// // 
// //   // Activate an EPS bearer
// //   enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
// //   EpsBearer bearer (q);
// //   lena.ActivateEpsBearer (ueDevs, bearer);
// // 
// //   Simulator::Stop (Seconds (0.01));
// // 
// //   Simulator::Run ();
// //   Simulator::Destroy ();
// 
//   // Calculate throughput = total number of bytes received / simulation time
// 
//   /*
//    * If ( throughput_ref - tolerance < throughput < throughput_ref + tolerance ) then
//    *    Test OK
//    * else
//    *    Test KO
//    */
// 
// // MRE: Now how the status is returned???
// 
// //  // Return false if an error has _not_ occurred
// //  return false;
// }
// 


class LenaTestSuite : public TestSuite
{
public:
  LenaTestSuite ();
};

LenaTestSuite::LenaTestSuite ()
  : TestSuite ("lena", SYSTEM)
{
  AddTestCase (new LenaDownlinkSinrTestCase);
//   AddTestCase (new LenaTestCase);
}

static LenaTestSuite lenaTestSuite;
