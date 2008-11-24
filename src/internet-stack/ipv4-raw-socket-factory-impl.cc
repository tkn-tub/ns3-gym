#include "ipv4-raw-socket-factory-impl.h"
#include "ipv4-l3-protocol.h"
#include "ns3/socket.h"

namespace ns3 {


Ptr<Socket> 
Ipv4RawSocketFactoryImpl::CreateSocket (void)
{
  Ptr<Ipv4L3Protocol> ipv4 = GetObject<Ipv4L3Protocol> ();
  Ptr<Socket> socket = ipv4->CreateRawSocket ();
  return socket;
}

} // namespace ns3
