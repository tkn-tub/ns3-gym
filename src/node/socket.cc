#include "socket.h"

namespace ns3 {

Socket::~Socket ()
{}

int
Socket::Close(Callback<void, Ptr<Socket> > closeCompleted)
{
  return DoClose (closeCompleted);
}

int
Socket::Connect(const Address & address,
               Callback<void, Ptr<Socket> > connectionSucceeded,
               Callback<void, Ptr<Socket> > connectionFailed,
               Callback<void, Ptr<Socket> > halfClose)
{
  return DoConnect (address, connectionSucceeded, connectionFailed, halfClose);
}
int
Socket::Accept(Callback<bool, Ptr<Socket>, const Address&> connectionRequest,
	       Callback<void, Ptr<Socket>, const Address&> newConnectionCreated,
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
Socket::SendTo(const Address &address,
	       const uint8_t *buffer,
	       uint32_t size,
	       Callback<void, Ptr<Socket>, uint32_t> dataSent)
{
  return DoSendTo (address, buffer, size, dataSent);
}
void 
Socket::Recv(Callback<void, Ptr<Socket>, const uint8_t*, uint32_t,const Address&> callback)
{
  DoRecv (callback);
}
void 
Socket::RecvDummy(Callback<void, Ptr<Socket>, uint32_t,const Address&> callback)
{
  DoRecvDummy (callback);
}


bool 
Socket::RefuseAllConnections (Ptr<Socket> socket, const Address& address)
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
Socket::DummyCallbackVoidSocketUi32Address (Ptr<Socket> socket, uint32_t, const Address &)
{}
void 
Socket::DummyCallbackVoidSocketBufferUi32Address (Ptr<Socket> socket, const uint8_t *, uint32_t,
						  const Address &)
{}
void 
Socket::DummyCallbackVoidSocketAddress (Ptr<Socket> socket, const Address &)
{}


}//namespace ns3
