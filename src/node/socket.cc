#include "socket.h"

namespace ns3 {

Socket::~Socket ()
{}

void 
Socket::Close(Callback<void, Socket*> closeCompleted)
{
  DoClose (closeCompleted);
}

void 
Socket::Connect(const Ipv4Address & address,
               uint16_t portNumber,
               Callback<void, Socket*> connectionSucceeded,
               Callback<void, Socket*> connectionFailed,
               Callback<void, Socket*> halfClose)
{
  DoConnect (address, portNumber, connectionSucceeded, connectionFailed, halfClose);
}
int
Socket::Accept(Callback<bool, Socket*, const Ipv4Address&, uint16_t> connectionRequest,
	       Callback<void, Socket*, const Ipv4Address&, uint16_t> newConnectionCreated,
	       Callback<void, Socket*> closeRequested)
{
  return DoAccept (connectionRequest, newConnectionCreated, closeRequested);
}
int 
Socket::Send (const uint8_t* buffer,
	      uint32_t size,
	      Callback<void, Socket*, uint32_t> dataSent)
{
  return DoSend (buffer, size, dataSent);
}
int 
Socket::SendTo(const Ipv4Address &address,
	       uint16_t port,
	       const uint8_t *buffer,
	       uint32_t size,
	       Callback<void, Socket*, uint32_t> dataSent)
{
  return DoSendTo (address, port, buffer, size, dataSent);
}
void 
Socket::Recv(Callback<void, Socket*, const uint8_t*, uint32_t,const Ipv4Address&, uint16_t> callback)
{
  DoRecv (callback);
}
void 
Socket::RecvDummy(Callback<void, Socket*, uint32_t,const Ipv4Address&, uint16_t> callback)
{
  DoRecvDummy (callback);
}


bool 
Socket::RefuseAllConnections (Socket* socket, const Ipv4Address& address, uint16_t port)
{
  return false;
}
void 
Socket::DummyCallbackVoidSocket (Socket *socket)
{}
void
Socket::DummyCallbackVoidSocketUi32 (Socket *socket, uint32_t)
{}
void 
Socket::DummyCallbackVoidSocketUi32Ipv4AddressUi16 (Socket *socket, uint32_t, const Ipv4Address &, uint16_t)
{}
void 
Socket::DummyCallbackVoidSocketBufferUi32Ipv4AddressUi16 (Socket *socket, const uint8_t *, uint32_t,
							  const Ipv4Address &, uint16_t)
{}
void 
Socket::DummyCallbackVoidSocketIpv4AddressUi16 (Socket *socket, const Ipv4Address &, uint16_t)
{}


}//namespace ns3
