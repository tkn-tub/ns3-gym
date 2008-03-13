#include "ns3/internet-node.h"
#include "ns3/ptr.h"
#include "ns3/node-list.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/csma-net-device.h"
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/mobility-model-notifier.h"
#include "ns3/string.h"

using namespace ns3;



int main (int argc, char *argv[])
{
  Ptr<Node> node = CreateObject<InternetNode> ();
  node->AggregateObject (CreateObject<MobilityModelNotifier> ());

  Ptr<PointToPointNetDevice> p2p = CreateObject<PointToPointNetDevice> ("Address", Mac48Address::Allocate ());
  node->AddDevice (p2p);
  p2p->AddQueue (CreateObject<DropTailQueue> ());
  Ptr<CsmaNetDevice> csma = CreateObject<CsmaNetDevice> ("Address", Mac48Address::Allocate (),
							 "EncapsulationMode", String ("Llc"));
  node->AddDevice (csma);
  csma->AddQueue (CreateObject<DropTailQueue> ());


  return 0;
}
