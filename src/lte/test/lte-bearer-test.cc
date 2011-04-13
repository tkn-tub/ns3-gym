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
#include "ns3/bearer-qos-parameters.h"
#include "ns3/radio-bearer-instance.h"


using namespace ns3;

/*
 * Test for LTE Bearers: Test that LTE Bearer can enqueue and dequeue one packet
 */
class Ns3LteBearerTestCase : public TestCase
{
public:
  Ns3LteBearerTestCase ();
  virtual ~Ns3LteBearerTestCase ();

private:
  virtual void DoRun (void);

};

Ns3LteBearerTestCase::Ns3LteBearerTestCase ()
  : TestCase ("Test that LTE Bearer can enqueue and dequeue one packet")
{
}

Ns3LteBearerTestCase::~Ns3LteBearerTestCase ()
{
}

void
Ns3LteBearerTestCase::DoRun (void)
{
  // create downlink data radio bearer and its qos parameters
  Ptr<RadioBearerInstance> bearer = CreateObject<RadioBearerInstance> ();
  bearer->SetBearerDirection (RadioBearerInstance::DIRECTION_TYPE_DL);
  bearer->SetBearerType (RadioBearerInstance::BEARER_TYPE_DRB);


  Ptr<BearerQosParameters> qos = CreateObject<BearerQosParameters> (1, false, false, 28000., 28000.);
  qos->SetBearerQosType (BearerQosParameters::BEARER_TYPE_GBR);

  bearer->SetQosParameters (qos);


  Ptr<Packet> p = Create<Packet> ();

  bearer->Enqueue (p);

  // the queue should have 1 packet
  NS_TEST_ASSERT_MSG_EQ (bearer->HasPackets (), true,  "The queue did not have any packets.");

  bearer->Dequeue ();

  // the queue should be empty
  NS_TEST_ASSERT_MSG_EQ (bearer->HasPackets (), false,  "The queue had packets.");

  // Free memory; handle reference cycle that bearer has with RlcEntity 
  bearer->Dispose ();
  bearer = 0;
  Simulator::Destroy ();
}
// ==============================================================================

class Ns3LteBearerTestTestSuite : public TestSuite
{
public:
  Ns3LteBearerTestTestSuite ();
};

Ns3LteBearerTestTestSuite::Ns3LteBearerTestTestSuite ()
  : TestSuite ("lte-bearer", UNIT)
{
  AddTestCase (new Ns3LteBearerTestCase);
}

static Ns3LteBearerTestTestSuite ns3LteBearerTestTestSuite;
