#include "socket.h"

namespace ns3 {

Socket::~Socket ()
{}

void 
Socket::Close(Callback<void, Ptr<Socket> > closeCompleted)
{
  DoClose (closeCompleted);
}

void 
Socket::Connect(const Ipv4Address & address,
               uint16_t portNumber,
               Callback<void, Ptr<Socket> > connectionSucceeded,
               Callback<void, Ptr<Socket> > connectionFailed,
               Callback<void, Ptr<Socket> > halfClose)
{
  DoConnect (address, portNumber, connectionSucceeded, connectionFailed, halfClose);
}
int
Socket::Accept(Callback<bool, Ptr<Socket>, const Ipv4Address&, uint16_t> connectionRequest,
	       Callback<void, Ptr<Socket>, const Ipv4Address&, uint16_t> newConnectionCreated,
	       Callback<void, Ptr<Socket> > closeRequested)
{
  return DoAccept (connectionRequest, newConnectionCreated, closeRequested);
}
int 
Socket::Send (const uint8_t* buffer,
	      uint32_t size,
	      Callback<void, Ptr<Socket>, uint32_t> dataSent)
{
  return DoSend (buffer, size, dataSent);
}
int 
Socket::SendTo(const Ipv4Address &address,
	       uint16_t port,
	       const uint8_t *buffer,
	       uint32_t size,
	       Callback<void, Ptr<Socket>, uint32_t> dataSent)
{
  return DoSendTo (address, port, buffer, size, dataSent);
}
void 
Socket::Recv(Callback<void, Ptr<Socket>, const uint8_t*, uint32_t,const Ipv4Address&, uint16_t> callback)
{
  DoRecv (callback);
}
void 
Socket::RecvDummy(Callback<void, Ptr<Socket>, uint32_t,const Ipv4Address&, uint16_t> callback)
{
  DoRecvDummy (callback);
}


bool 
Socket::RefuseAllConnections (Ptr<Socket> socket, const Ipv4Address& address, uint16_t port)
{
  return false;
}
void 
Socket::DummyCallbackVoidSocket (Ptr<Socket> socket)
{}
void
Socket::DummyCallbackVoidSocketUi32 (Ptr<Socket> socket, uint32_t)
{}
void 
Socket::DummyCallbackVoidSocketUi32Ipv4AddressUi16 (Ptr<Socket> socket, uint32_t, const Ipv4Address &, uint16_t)
{}
void 
Socket::DummyCallbackVoidSocketBufferUi32Ipv4AddressUi16 (Ptr<Socket> socket, const uint8_t *, uint32_t,
							  const Ipv4Address &, uint16_t)
{}
void 
Socket::DummyCallbackVoidSocketIpv4AddressUi16 (Ptr<Socket> socket, const Ipv4Address &, uint16_t)
{}


}//namespace ns3
