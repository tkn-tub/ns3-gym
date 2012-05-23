/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: John Abraham <john.abraham@gatech.edu>
 */

#include <iostream>
#include "unistd.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"

namespace ns3 {

class AnimationInterfaceTestCase : public TestCase
{
public:
  /**
   * \brief Constructor.
   */
  AnimationInterfaceTestCase ();
  /**
   * \brief Destructor.
   */
  virtual
  ~AnimationInterfaceTestCase ();
  /**
   * \brief Run unit tests for this class.
   * \return false if all tests have passed, false otherwise
   */
  virtual void
  DoRun (void);

};

AnimationInterfaceTestCase::AnimationInterfaceTestCase () :
  TestCase ("Verify AnimationInterface")
{
}

AnimationInterfaceTestCase::~AnimationInterfaceTestCase ()
{
}
void
AnimationInterfaceTestCase::DoRun (void)
{
  NodeContainer nodes;
  nodes.Create (2);
  AnimationInterface::SetConstantPosition (nodes.Get (0), 0 , 10);
  AnimationInterface::SetConstantPosition (nodes.Get (1), 1 , 10);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (100));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  std::string traceFileName = "netanim-test.xml";
  AnimationInterface anim(traceFileName.c_str ());
  Simulator::Run ();
  NS_TEST_ASSERT_MSG_EQ (anim.GetTracePktCount (), 32, "Expected 32 packets traced");
  FILE * fp = fopen (traceFileName.c_str (), "r");
  NS_TEST_ASSERT_MSG_NE (fp, 0, "Trace file was not created");
  fclose (fp);
  unlink (traceFileName.c_str ());
  Simulator::Destroy ();
}

static class AnimationInterfaceTestSuite : public TestSuite
{
public:
  AnimationInterfaceTestSuite () :
    TestSuite ("animation-interface", UNIT)
  {
    AddTestCase (new AnimationInterfaceTestCase ());
  }
} g_animationInterfaceTestSuite;

} // namespace ns3
