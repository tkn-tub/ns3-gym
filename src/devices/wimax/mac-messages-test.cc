/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2009 INRIA, UDcast
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
 *         Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *
 */
#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/test.h"
#include "ns3/uinteger.h"
#include "ns3/inet-socket-address.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/udp-header.h"
#include "ns3/simulator.h"
#include "ns3/wimax-helper.h"
#include "ns3/mobility-helper.h"
#include <iostream>
#include "ns3/global-route-manager.h"
#include "wimax-tlv.h"
#include "ipcs-classifier-record.h"
#include "service-flow.h"

using namespace ns3;
/*
 * Test the DSA request message.
 */
class DsaRequestTestCase : public TestCase
{
public:
  DsaRequestTestCase ();
  virtual ~DsaRequestTestCase ();

private:
  virtual bool DoRun (void);

};

DsaRequestTestCase::DsaRequestTestCase ()
  : TestCase ("Test the DSA request messages")
{
}

DsaRequestTestCase::~DsaRequestTestCase ()
{
}

bool
DsaRequestTestCase::DoRun (void)
{
  IpcsClassifierRecord classifier = IpcsClassifierRecord ();
  CsParameters csParam (CsParameters::ADD, classifier);
  ServiceFlow sf = ServiceFlow (ServiceFlow::SF_DIRECTION_DOWN);

  sf.SetSfid (100);
  sf.SetConvergenceSublayerParam (csParam);
  sf.SetCsSpecification (ServiceFlow::IPV4);
  sf.SetServiceSchedulingType (ServiceFlow::SF_TYPE_UGS);
  sf.SetMaxSustainedTrafficRate (1000000);
  sf.SetMinReservedTrafficRate (1000000);
  sf.SetMinTolerableTrafficRate (1000000);
  sf.SetMaximumLatency (10);
  sf.SetMaxTrafficBurst (1000);
  sf.SetTrafficPriority (1);

  DsaReq dsaReq (sf);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (dsaReq);

  DsaReq dsaReqRecv;
  packet->RemoveHeader (dsaReqRecv);

  ServiceFlow sfRecv = dsaReqRecv.GetServiceFlow ();

  if (sfRecv.GetDirection () != ServiceFlow::SF_DIRECTION_DOWN)
    {

      return true;
    }
  if (sfRecv.GetSfid () != 100)
    {
      return true;
    }
  if (sfRecv.GetCsSpecification () != ServiceFlow::IPV4)
    {
      return true;
    }
  if (sfRecv.GetServiceSchedulingType () != ServiceFlow::SF_TYPE_UGS)
    {
      return true;
    }
  if (sfRecv.GetMaxSustainedTrafficRate () != 1000000)
    {
      return true;
    }
  if (sfRecv.GetMinReservedTrafficRate () != 1000000)
    {
      return true;
    }
  if (sfRecv.GetMinTolerableTrafficRate () != 1000000)
    {
      return true;
    }
  if (sfRecv.GetMaximumLatency () != 10)
    {
      return true;
    }
  if (sfRecv.GetMaxTrafficBurst () != 1000)
    {
      return true;
    }
  if (sfRecv.GetTrafficPriority () != 1)
    {
      return true;
    }

  return false; // Test is passed
}

// ==============================================================================
class Ns3WimaxMacMessagesTestSuite : public TestSuite
{
public:
  Ns3WimaxMacMessagesTestSuite ();
};

Ns3WimaxMacMessagesTestSuite::Ns3WimaxMacMessagesTestSuite ()
  : TestSuite ("wimax-mac-messages", UNIT)
{
  AddTestCase (new DsaRequestTestCase);
}

Ns3WimaxMacMessagesTestSuite ns3WimaxMacMessagesTestSuite;
