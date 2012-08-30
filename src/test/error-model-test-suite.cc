/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/test.h"
#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/address.h"
#include "ns3/mac48-address.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/error-model.h"
#include "ns3/pointer.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/rng-seed-manager.h"

using namespace ns3;

static void SendPacket (int num, Ptr<NetDevice> device, Address& addr)
{
  for (int i = 0; i < num; i++)
    {
      Ptr<Packet> pkt = Create<Packet> (1000);  // 1000 dummy bytes of data
      device->Send (pkt, addr, 0);
    }
}

// Two nodes, two devices, one channel
static void BuildSimpleTopology (Ptr<Node> a, Ptr<Node> b, Ptr<SimpleNetDevice> input, Ptr<SimpleNetDevice> output, Ptr<SimpleChannel> channel)
{
  a->AddDevice (input);
  b->AddDevice (output);
  input->SetAddress (Mac48Address::Allocate ());
  input->SetChannel (channel);
  input->SetNode (a);
  output->SetChannel (channel);
  output->SetNode (b);
  output->SetAddress (Mac48Address::Allocate ());
}

class ErrorModelSimple : public TestCase
{
public:
  ErrorModelSimple ();
  virtual ~ErrorModelSimple ();

private:
  virtual void DoRun (void);
  bool Receive (Ptr<NetDevice> nd, Ptr<const Packet> p, uint16_t protocol, const Address& addr);
  void DropEvent (Ptr<const Packet> p);
  uint32_t m_count;
  uint32_t m_drops;
};

// Add some help text to this case to describe what it is intended to test
ErrorModelSimple::ErrorModelSimple ()
  : TestCase ("ErrorModel and PhyRxDrop trace for SimpleNetDevice"), m_count (0), m_drops (0)
{
}

ErrorModelSimple::~ErrorModelSimple ()
{
}

bool 
ErrorModelSimple::Receive (Ptr<NetDevice> nd, Ptr<const Packet> p, uint16_t protocol, const Address& addr)
{
  m_count++;
  return true;
}

void 
ErrorModelSimple::DropEvent (Ptr<const Packet> p)
{
  m_drops++;
}

void
ErrorModelSimple::DoRun (void)
{
  // Set some arbitrary deterministic values
  RngSeedManager::SetSeed (7);
  RngSeedManager::SetRun (2);

  Ptr<Node> a = CreateObject<Node> ();
  Ptr<Node> b = CreateObject<Node> ();

  Ptr<SimpleNetDevice> input = CreateObject<SimpleNetDevice> ();
  Ptr<SimpleNetDevice> output = CreateObject<SimpleNetDevice> ();
  Ptr<SimpleChannel> channel = CreateObject<SimpleChannel> ();
  BuildSimpleTopology (a, b, input, output, channel);

  output->SetReceiveCallback (MakeCallback (&ErrorModelSimple::Receive, this));
  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  // Set this variable to a specific stream 
  uv->SetStream (50);

  Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
  em->SetRandomVariable (uv);
  em->SetAttribute ("ErrorRate", DoubleValue (0.001));
  em->SetAttribute ("ErrorUnit", StringValue ("ERROR_UNIT_PACKET"));

  // The below hooks will cause drops and receptions to be counted
  output->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  output->TraceConnectWithoutContext ("PhyRxDrop", MakeCallback (&ErrorModelSimple::DropEvent, this));

  // Send 10000 packets
  Simulator::Schedule (Seconds (0), &SendPacket, 10000, input, output->GetAddress ());

  Simulator::Run ();
  Simulator::Destroy ();

  // For this combination of values, we expect about 1 packet in 1000 to be
  // dropped.  For this specific RNG stream, we see 9991 receptions and 9 drops
  NS_TEST_ASSERT_MSG_EQ (m_count, 9991, "Wrong number of receptions.");
  NS_TEST_ASSERT_MSG_EQ (m_drops, 9, "Wrong number of drops.");
}

// This is the start of an error model test suite.  For starters, this is
// just testing that the SimpleNetDevice is working but this can be
// extended to many more test cases in the future
class ErrorModelTestSuite : public TestSuite
{
public:
  ErrorModelTestSuite ();
};

ErrorModelTestSuite::ErrorModelTestSuite ()
  : TestSuite ("error-model", UNIT)
{
  AddTestCase (new ErrorModelSimple);
}

// Do not forget to allocate an instance of this TestSuite
static ErrorModelTestSuite errorModelTestSuite;
