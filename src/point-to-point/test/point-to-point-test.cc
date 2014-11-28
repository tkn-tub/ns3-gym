#include "ns3/test.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/simulator.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"

using namespace ns3;

/**
 * \brief Test class for PointToPoint model
 *
 * It tries to send one packet from one NetDevice to another, over a
 * PointToPointChannel.
 */
class PointToPointTest : public TestCase
{
public:
  /**
   * \brief Create the test
   */
  PointToPointTest ();

  /**
   * \brief Run the test
   */
  virtual void DoRun (void);

private:
  /**
   * \brief Send one packet to the device specified
   *
   * \param device NetDevice to send to
   */
  void SendOnePacket (Ptr<PointToPointNetDevice> device);
};

PointToPointTest::PointToPointTest ()
  : TestCase ("PointToPoint")
{
}

void
PointToPointTest::SendOnePacket (Ptr<PointToPointNetDevice> device)
{
  Ptr<Packet> p = Create<Packet> ();
  device->Send (p, device->GetBroadcast (), 0x800);
}


void
PointToPointTest::DoRun (void)
{
  Ptr<Node> a = CreateObject<Node> ();
  Ptr<Node> b = CreateObject<Node> ();
  Ptr<PointToPointNetDevice> devA = CreateObject<PointToPointNetDevice> ();
  Ptr<PointToPointNetDevice> devB = CreateObject<PointToPointNetDevice> ();
  Ptr<PointToPointChannel> channel = CreateObject<PointToPointChannel> ();

  devA->Attach (channel);
  devA->SetAddress (Mac48Address::Allocate ());
  devA->SetQueue (CreateObject<DropTailQueue> ());
  devB->Attach (channel);
  devB->SetAddress (Mac48Address::Allocate ());
  devB->SetQueue (CreateObject<DropTailQueue> ());

  a->AddDevice (devA);
  b->AddDevice (devB);

  Simulator::Schedule (Seconds (1.0), &PointToPointTest::SendOnePacket, this, devA);

  Simulator::Run ();

  Simulator::Destroy ();
}

/**
 * \brief TestSuite for PointToPoint module
 */
class PointToPointTestSuite : public TestSuite
{
public:
  /**
   * \brief Constructor
   */
  PointToPointTestSuite ();
};

PointToPointTestSuite::PointToPointTestSuite ()
  : TestSuite ("devices-point-to-point", UNIT)
{
  AddTestCase (new PointToPointTest, TestCase::QUICK);
}

static PointToPointTestSuite g_pointToPointTestSuite; //!< The testsuite
