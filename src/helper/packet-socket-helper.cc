#include "packet-socket-helper.h"
#include "ns3/packet-socket-factory.h"
#include "ns3/socket-defaults.h"

namespace ns3 {

void 
PacketSocketHelper::Install (NodeContainer c)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory> ();
      node->AggregateObject (factory);
      Ptr<SocketDefaults> sockDefaults = CreateObject<SocketDefaults> ();
      node->AggregateObject (sockDefaults);
    }
}

} // namespace ns3
