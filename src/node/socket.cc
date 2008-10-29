/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation
 *               2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: George F. Riley<riley@ece.gatech.edu>
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/log.h"
#include "ns3/packet.h"
#include "node.h"
#include "socket.h"
#include "socket-factory.h"
#include <limits>

NS_LOG_COMPONENT_DEFINE ("Socket");

namespace ns3 {

Socket::Socket (void)
{
  NS_LOG_FUNCTION_NOARGS ();
}

Socket::~Socket ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ptr<Socket> 
Socket::CreateSocket (Ptr<Node> node, TypeId tid)
{
  Ptr<Socket> s;
  Ptr<SocketFactory> socketFactory = node->GetObject<SocketFactory> (tid);
  s = socketFactory->CreateSocket ();
  NS_ASSERT (s != 0);
  return s;
}

void 
Socket::SetConnectCallback (
  Callback<void, Ptr<Socket> > connectionSucceeded,
  Callback<void, Ptr<Socket> > connectionFailed)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_connectionSucceeded = connectionSucceeded;
  m_connectionFailed = connectionFailed;
}

void 
Socket::SetAcceptCallback (
  Callback<bool, Ptr<Socket>, const Address &> connectionRequest,
  Callback<void, Ptr<Socket>, const Address&> newConnectionCreated)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_connectionRequest = connectionRequest;
  m_newConnectionCreated = newConnectionCreated;
}

void
Socket::SetDataSentCallback (Callback<void, Ptr<Socket>, uint32_t> dataSent)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_dataSent = dataSent;
}

void
Socket::SetSendCallback (Callback<void, Ptr<Socket>, uint32_t> sendCb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_sendCb = sendCb;
}

void 
Socket::SetRecvCallback (Callback<void, Ptr<Socket> > receivedData)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_receivedData = receivedData;
}

int 
Socket::Send (Ptr<Packet> p)
{
  NS_LOG_FUNCTION_NOARGS ();
  return Send (p, 0);
}

int 
Socket::Send (const uint8_t* buf, uint32_t size, uint32_t flags)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Packet> p;
  if (buf)
    {
      p = Create<Packet> (buf, size);
    }
  else
    {
      p = Create<Packet> (size);
    }
  return Send (p, flags);
}

int 
Socket::SendTo (const uint8_t* buf, uint32_t size, uint32_t flags,
                const Address &toAddress)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Packet> p;
  if(buf)
    {
      p = Create<Packet> (buf, size);
    }
  else
    {
      p = Create<Packet> (size);
    }
  return SendTo (p, flags, toAddress);
}

Ptr<Packet>
Socket::Recv (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return Recv (std::numeric_limits<uint32_t>::max(), 0);
}

int 
Socket::Recv (uint8_t* buf, uint32_t size, uint32_t flags)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Packet> p = Recv (size, flags); // read up to "size" bytes
  if (p == 0)
    {
      return 0;
    }
  memcpy (buf, p->PeekData (), p->GetSize());
  return p->GetSize ();
}

Ptr<Packet>
Socket::RecvFrom (Address &fromAddress)
{
  NS_LOG_FUNCTION_NOARGS ();
  return RecvFrom (std::numeric_limits<uint32_t>::max(), 0, fromAddress);
}

int 
Socket::RecvFrom (uint8_t* buf, uint32_t size, uint32_t flags,
                  Address &fromAddress)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Packet> p = RecvFrom (size, flags, fromAddress); 
  if (p == 0)
    {
      return 0;
    }
  memcpy (buf, p->PeekData (), p->GetSize());
  return p->GetSize ();
}


void 
Socket::NotifyConnectionSucceeded (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (!m_connectionSucceeded.IsNull ())
    {
      m_connectionSucceeded (this);
    }
}

void 
Socket::NotifyConnectionFailed (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (!m_connectionFailed.IsNull ())
    {
      m_connectionFailed (this);
    }
}

bool 
Socket::NotifyConnectionRequest (const Address &from)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (!m_connectionRequest.IsNull ())
    {
      return m_connectionRequest (this, from);
    }
  else
    {
      // accept all incoming connections by default.
      // this way people writing code don't have to do anything
      // special like register a callback that returns true
      // just to get incoming connections
      return true;
    }
}

void 
Socket::NotifyNewConnectionCreated (Ptr<Socket> socket, const Address &from)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (!m_newConnectionCreated.IsNull ())
    {
      m_newConnectionCreated (socket, from);
    }
}

void 
Socket::NotifyDataSent (uint32_t size)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (!m_dataSent.IsNull ())
    {
      m_dataSent (this, size);
    }
}

void 
Socket::NotifySend (uint32_t spaceAvailable)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (!m_sendCb.IsNull ())
    {
      m_sendCb (this, spaceAvailable);
    }
}

void 
Socket::NotifyDataRecv (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (!m_receivedData.IsNull ())
    {
      m_receivedData (this);
    }
}

/***************************************************************
 *           Socket Tags
 ***************************************************************/

SocketAddressTag::SocketAddressTag ()  
{
}

void 
SocketAddressTag::SetAddress (Address addr)
{
  m_address = addr;
}

Address 
SocketAddressTag::GetAddress (void) const
{
  return m_address;
}


TypeId
SocketAddressTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SocketAddressTag")
    .SetParent<Tag> ()
    .AddConstructor<SocketAddressTag> ()
    ;
  return tid;
}
TypeId
SocketAddressTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
SocketAddressTag::GetSerializedSize (void) const
{
  return m_address.GetSerializedSize ();
}
void
SocketAddressTag::Serialize (TagBuffer i) const
{
  m_address.Serialize (i);
}
void
SocketAddressTag::Deserialize (TagBuffer i)
{
  m_address.Deserialize (i);
}
void
SocketAddressTag::Print (std::ostream &os) const
{
  os << "address=" << m_address;
}

SocketIpTtlTag::SocketIpTtlTag ()  
{
}

void 
SocketIpTtlTag::SetTtl (uint8_t ttl)
{
  m_ttl = ttl;
}

uint8_t 
SocketIpTtlTag::GetTtl (void) const
{
  return m_ttl;
}


TypeId
SocketIpTtlTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SocketIpTtlTag")
    .SetParent<Tag> ()
    .AddConstructor<SocketIpTtlTag> ()
    ;
  return tid;
}
TypeId
SocketIpTtlTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
SocketIpTtlTag::GetSerializedSize (void) const
{ 
  return 1;
}
void 
SocketIpTtlTag::Serialize (TagBuffer i) const
{ 
  i.WriteU8 (m_ttl);
}
void 
SocketIpTtlTag::Deserialize (TagBuffer i)
{ 
  m_ttl = i.ReadU8 ();
}
void
SocketIpTtlTag::Print (std::ostream &os) const
{
  os << "Ttl=" << (uint32_t) m_ttl;
}


SocketSetDontFragmentTag::SocketSetDontFragmentTag ()
{}
void 
SocketSetDontFragmentTag::Enable (void)
{
  m_dontFragment = true;
}
void 
SocketSetDontFragmentTag::Disable (void)
{
  m_dontFragment = false;
}
bool 
SocketSetDontFragmentTag::IsEnabled (void) const
{
  return m_dontFragment;
}

TypeId 
SocketSetDontFragmentTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SocketSetDontFragmentTag")
    .SetParent<Tag> ()
    .AddConstructor<SocketSetDontFragmentTag> ()
    ;
  return tid;
}
TypeId 
SocketSetDontFragmentTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t 
SocketSetDontFragmentTag::GetSerializedSize (void) const
{
  return 1;
}
void 
SocketSetDontFragmentTag::Serialize (TagBuffer i) const
{
  i.WriteU8 (m_dontFragment?1:0);
}
void 
SocketSetDontFragmentTag::Deserialize (TagBuffer i)
{
  m_dontFragment = (i.ReadU8 () == 1)?true:false;
}
void 
SocketSetDontFragmentTag::Print (std::ostream &os) const
{
  os << (m_dontFragment?"true":"false");
}

}//namespace ns3
