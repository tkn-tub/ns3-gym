#include "ns3/internet-node.h"
#include "ns3/simulator.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  InternetNode *a = new InternetNode ();

  Simulator::Run ();

  Simulator::Destroy ();

  delete a;

  return 0;
}
