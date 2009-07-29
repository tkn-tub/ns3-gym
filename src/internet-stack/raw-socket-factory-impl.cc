#include "raw-socket-factory-impl.h"
#include "raw-socket-impl.h"
#include "ipv4-l3-protocol.h"
#include "ns3/socket.h"
#include "ns3/log.h"

namespace ns3 {

void
RawSocketFactoryImpl::DoDispose (void)
{
  m_ipv4 = 0;
  RawSocketFactory::DoDispose ();
}

Ptr<Socket> 
RawSocketFactoryImpl::CreateSocket (void)
{
  Ptr<Socket> socket = m_ipv4->CreateRawSocket2 ();
  return socket;
}

} // namespace ns3
