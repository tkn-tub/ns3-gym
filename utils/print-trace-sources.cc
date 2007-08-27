#include "ns3/internet-node.h"
#include "ns3/ptr.h"
#include "ns3/trace-resolver.h"
#include "ns3/node-list.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/csma-cd-net-device.h"
#include "ns3/queue.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  Ptr<Node> node = Create<InternetNode> ();

  Ptr<PointToPointNetDevice> p2p = Create<PointToPointNetDevice> (node);
  p2p->AddQueue (Queue::CreateDefault ());
  Ptr<CsmaCdNetDevice> csma = Create<CsmaCdNetDevice> (node);
  csma->AddQueue (Queue::CreateDefault ());

  NodeList::GetTraceResolver ()->PrintAvailable ("", TraceContext (), std::cout);

  return 0;
}
