#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/simulator.h"
#include "point-to-point-net-device.h"
#include "point-to-point-channel.h"

namespace ns3 {

class PointToPointTest : public Test
{
public:
  PointToPointTest ();

  virtual bool RunTests (void);

private:
  void SendOnePacket (Ptr<PointToPointNetDevice> device);
};

PointToPointTest::PointToPointTest ()
  : Test ("PointToPoint")
{}

void
PointToPointTest::SendOnePacket (Ptr<PointToPointNetDevice> device)
{
  Ptr<Packet> p = Create<Packet> ();
  device->Send (p, device->GetBroadcast (), 0x800);
}


bool
PointToPointTest::RunTests (void)
{
  bool result = true;

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

  return result;
}

static PointToPointTest g_pointToPointTest;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
