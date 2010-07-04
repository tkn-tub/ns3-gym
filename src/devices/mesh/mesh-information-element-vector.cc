/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Pavel Boyko <boyko@iitp.ru>
 */

#include "mesh-information-element-vector.h"
#include "ns3/packet.h"
#include <algorithm>
#include "ns3/test.h"
#include "ns3/hwmp-protocol.h"
// All information elements:
#include "dot11s/ie-dot11s-beacon-timing.h"
#include "dot11s/ie-dot11s-configuration.h"
#include "dot11s/ie-dot11s-id.h"
#include "dot11s/ie-dot11s-metric-report.h"
#include "dot11s/ie-dot11s-peer-management.h"
#include "dot11s/ie-dot11s-peering-protocol.h"
#include "dot11s/ie-dot11s-perr.h"
#include "dot11s/ie-dot11s-prep.h"
#include "dot11s/ie-dot11s-preq.h"
#include "dot11s/ie-dot11s-rann.h"

namespace ns3 {

uint32_t
MeshInformationElementVector::DeserializeSingleIe(Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t id = i.ReadU8 ();
  uint8_t length = i.ReadU8 ();
  Ptr<WifiInformationElement> newElement;
  switch (id)
    {
  case IE11S_MESH_CONFIGURATION:
    newElement = Create<dot11s::IeConfiguration> ();
    break;
  case IE11S_MESH_ID:
    newElement = Create<dot11s::IeMeshId> ();
    break;
  case IE11S_LINK_METRIC_REPORT:
    newElement = Create<dot11s::IeLinkMetricReport> ();
    break;
  case IE11S_PEERING_MANAGEMENT:
    newElement = Create<dot11s::IePeerManagement> ();
    break;
  case IE11S_BEACON_TIMING:
    newElement = Create<dot11s::IeBeaconTiming> ();
    break;
  case IE11S_RANN:
    newElement = Create<dot11s::IeRann> ();
    break;
  case IE11S_PREQ:
    newElement = Create<dot11s::IePreq> ();
    break;
  case IE11S_PREP:
    newElement = Create<dot11s::IePrep> ();
    break;
  case IE11S_PERR:
    newElement = Create<dot11s::IePerr> ();
    break;
  case IE11S_MESH_PEERING_PROTOCOL_VERSION:
    newElement = Create<dot11s::IePeeringProtocol> ();
    break;
  default:
    // We peeked at the ID and length, so we need to back up the
    // pointer before deferring to our parent.
    i.Prev (2);
    return WifiInformationElementVector::DeserializeSingleIe (i);
    }
  if (GetSize () + length > m_maxSize)
    {
      NS_FATAL_ERROR ("Check max size for information element!");
    }
  newElement->DeserializeInformationField (i, length);
  i.Next (length);
  m_elements.push_back (newElement);
  return i.GetDistanceFrom(start);
}


// Unit tests
//-----------------------------------------------------------------------------
/// Built-in self test for MeshInformationElementVector and all IE
struct MeshInformationElementVectorBist : public TestCase
{
  MeshInformationElementVectorBist () :
    TestCase ("Serializarion test for all mesh information elements")
  {
  };
  bool DoRun ();
};

bool
MeshInformationElementVectorBist::DoRun ()
{
  MeshInformationElementVector vector;
  {
    //Mesh ID test
    Ptr<dot11s::IeMeshId> meshId = Create<dot11s::IeMeshId> ("qwerty");
    vector.AddInformationElement (meshId);
  }
  {
    Ptr<dot11s::IeConfiguration> config = Create<dot11s::IeConfiguration> ();
    vector.AddInformationElement (config);
  }
  {
    Ptr<dot11s::IeLinkMetricReport> report = Create<dot11s::IeLinkMetricReport> (123456);
    vector.AddInformationElement (report);
  }
  {
    Ptr<dot11s::IePeerManagement> peerMan1 = Create<dot11s::IePeerManagement> ();
    peerMan1->SetPeerOpen (1);
    Ptr<dot11s::IePeerManagement> peerMan2 = Create<dot11s::IePeerManagement> ();
    peerMan2->SetPeerConfirm (1, 2);
    Ptr<dot11s::IePeerManagement> peerMan3 = Create<dot11s::IePeerManagement> ();
    peerMan3->SetPeerClose (1, 2, dot11s::REASON11S_MESH_CAPABILITY_POLICY_VIOLATION);
    vector.AddInformationElement (peerMan1);
    vector.AddInformationElement (peerMan2);
    vector.AddInformationElement (peerMan3);
  }
  {
    Ptr<dot11s::IeBeaconTiming>  beaconTiming = Create<dot11s::IeBeaconTiming> ();
    beaconTiming->AddNeighboursTimingElementUnit (1, Seconds (1.0), Seconds (4.0));
    beaconTiming->AddNeighboursTimingElementUnit (2, Seconds (2.0), Seconds (3.0));
    beaconTiming->AddNeighboursTimingElementUnit (3, Seconds (3.0), Seconds (2.0));
    beaconTiming->AddNeighboursTimingElementUnit (4, Seconds (4.0), Seconds (1.0));
    vector.AddInformationElement (beaconTiming);
  }
  {
    Ptr<dot11s::IeRann> rann = Create<dot11s::IeRann> ();
    rann->SetFlags (1);
    rann->SetHopcount (2);
    rann->SetTTL (4);
    rann->DecrementTtl ();
    NS_TEST_ASSERT_MSG_EQ (rann->GetTtl (), 3, "SetTtl works");
    rann->SetOriginatorAddress (Mac48Address ("11:22:33:44:55:66"));
    rann->SetDestSeqNumber (5);
    rann->SetMetric (6);
    rann->IncrementMetric (2);
    NS_TEST_ASSERT_MSG_EQ (rann->GetMetric (), 8, "SetMetric works");
    vector.AddInformationElement (rann);
  }
  {
    Ptr<dot11s::IePreq> preq = Create<dot11s::IePreq> ();
    preq->SetHopcount (0);
    preq->SetTTL (1);
    preq->SetPreqID (2);
    preq->SetOriginatorAddress (Mac48Address ("11:22:33:44:55:66"));
    preq->SetOriginatorSeqNumber (3);
    preq->SetLifetime (4);
    preq->AddDestinationAddressElement (false, false, Mac48Address ("11:11:11:11:11:11"), 5);
    preq->AddDestinationAddressElement (false, false, Mac48Address ("22:22:22:22:22:22"), 6);
    vector.AddInformationElement (preq);
  }
  {
    Ptr<dot11s::IePrep> prep = Create<dot11s::IePrep> ();
    prep->SetFlags (12);
    prep->SetHopcount (11);
    prep->SetTtl (10);
    prep->SetDestinationAddress (Mac48Address ("11:22:33:44:55:66"));
    prep->SetDestinationSeqNumber (123);
    prep->SetLifetime (5000);
    prep->SetMetric (4321);
    prep->SetOriginatorAddress (Mac48Address ("33:00:22:00:11:00"));
    prep->SetOriginatorSeqNumber (666);
    vector.AddInformationElement (prep);
  }
  {
    Ptr<dot11s::IePerr> perr = Create<dot11s::IePerr> ();
    dot11s::HwmpProtocol::FailedDestination dest;
    dest.destination = Mac48Address ("11:22:33:44:55:66");
    dest.seqnum = 1;
    perr->AddAddressUnit (dest);
    dest.destination = Mac48Address ("10:20:30:40:50:60");
    dest.seqnum = 2;
    perr->AddAddressUnit (dest);
    dest.destination = Mac48Address ("01:02:03:04:05:06");
    dest.seqnum = 3;
    perr->AddAddressUnit (dest);
    vector.AddInformationElement (perr);
  }
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (vector);
  MeshInformationElementVector resultVector;
  packet->RemoveHeader (resultVector);
  NS_TEST_ASSERT_MSG_EQ (vector, resultVector, "Roundtrip serialization of all known information elements works");
  
  return false;
}

class MeshTestSuite : public TestSuite
{
public:
  MeshTestSuite ();
};

MeshTestSuite::MeshTestSuite ()
  : TestSuite ("devices-mesh", UNIT)
{
  AddTestCase (new MeshInformationElementVectorBist);
}

MeshTestSuite g_meshTestSuite;

} //namespace ns3
