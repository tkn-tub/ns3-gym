#include "ipv6-raw-socket-factory-impl.h"
#include "ipv6-l3-protocol.h"
#include "ns3/socket.h"

namespace ns3
{

Ptr<Socket> Ipv6RawSocketFactoryImpl::CreateSocket ()
{
  Ptr<Ipv6L3Protocol> ipv6 = GetObject<Ipv6L3Protocol> ();
  Ptr<Socket> socket = ipv6->CreateRawSocket ();
  return socket;
}

} /* namespace ns3 */

