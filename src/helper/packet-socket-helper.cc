#include "packet-socket-helper.h"
#include "ns3/packet-socket-factory.h"

namespace ns3 {

void 
PacketSocketHelper::Build (NodeContainer c)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory> ();
      node->AggregateObject (factory);
    }
}

} // namespace ns3
