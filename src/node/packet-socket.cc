/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Emmanuelle Laprise, INRIA
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
 * Authors: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "packet-socket.h"
#include "packet-socket-address.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"

#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("PacketSocket");

namespace ns3 {

TypeId
PacketSocket::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PacketSocket")
    .SetParent<Socket> ()
    .AddConstructor<PacketSocket> ()
    .AddTraceSource ("Drop", "Drop packet due to receive buffer overflow",
                     MakeTraceSourceAccessor (&PacketSocket::m_dropTrace))
    .AddAttribute ("RcvBufSize",
                   "PacketSocket maximum receive buffer size (bytes)",
                   UintegerValue (0xffffffffl),
                   MakeUintegerAccessor (&PacketSocket::m_rcvBufSize),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

PacketSocket::PacketSocket () : m_rxAvailable (0)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_state = STATE_OPEN;
  m_shutdownSend = false;
  m_shutdownRecv = false;
  m_errno = ERROR_NOTERROR;
}

void 
PacketSocket::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = node;
}

PacketSocket::~PacketSocket ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
PacketSocket::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_device = 0;
}

enum Socket::SocketErrno
PacketSocket::GetErrno (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_errno;
}

Ptr<Node>
PacketSocket::GetNode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

int
PacketSocket::Bind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  PacketSocketAddress address;
  address.SetProtocol (0);
  address.SetAllDevices ();
  return DoBind (address);
}

int
PacketSocket::Bind (const Address &address)
{ 
  NS_LOG_FUNCTION_NOARGS ();
  if (!PacketSocketAddress::IsMatchingType (address))
    {
      m_errno = ERROR_INVAL;
      return -1;
    }
  PacketSocketAddress ad = PacketSocketAddress::ConvertFrom (address);
  return DoBind (ad);
}

int
PacketSocket::DoBind (const PacketSocketAddress &address)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_state == STATE_BOUND ||
      m_state == STATE_CONNECTED)
    {
      m_errno = ERROR_INVAL;
      return -1;
    }
  if (m_state == STATE_CLOSED)
    {
      m_errno = ERROR_BADF;
      return -1;
    }
  Ptr<NetDevice> dev ;
  if (address.IsSingleDevice ())
    {
      dev = m_node->GetDevice (address.GetSingleDevice ());
    }
  else
    {
      dev = 0;
    }
  m_node->RegisterProtocolHandler (MakeCallback (&PacketSocket::ForwardUp, this),
                                   address.GetProtocol (), dev);
  m_state = STATE_BOUND;
  m_protocol = address.GetProtocol ();
  m_isSingleDevice = address.IsSingleDevice ();
  m_device = address.GetSingleDevice ();
  return 0;
}

int
PacketSocket::ShutdownSend (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_state == STATE_CLOSED)
    {
      m_errno = ERROR_BADF;
      return -1;
    }
  m_shutdownSend = true;
  return 0;
}

int
PacketSocket::ShutdownRecv (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_state == STATE_CLOSED)
    {
      m_errno = ERROR_BADF;
      return -1;
    }
  m_shutdownRecv = false;
  return 0;
}

int
PacketSocket::Close(void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_state == STATE_CLOSED)
    {
      m_errno = ERROR_BADF;
      return -1;
    }
  m_state = STATE_CLOSED;
  return 0;
}

int
PacketSocket::Connect(const Address &ad)
{
  NS_LOG_FUNCTION_NOARGS ();
  PacketSocketAddress address;
  if (m_state == STATE_CLOSED)
    {
      m_errno = ERROR_BADF;
      goto error;
    }
  if (m_state == STATE_OPEN)
    {
      // connect should happen _after_ bind.
      m_errno = ERROR_INVAL; // generic error condition.
      goto error;
    }
  if (m_state == STATE_CONNECTED)
    {
      m_errno = ERROR_ISCONN;
      goto error;
    }
  if (!PacketSocketAddress::IsMatchingType (ad))
    {
      m_errno = ERROR_AFNOSUPPORT;
      goto error;
    }
  m_destAddr = ad;
  m_state = STATE_CONNECTED;
  NotifyConnectionSucceeded ();
  return 0;
 error:
  NotifyConnectionFailed ();
  return -1;
}
int 
PacketSocket::Listen(uint32_t queueLimit)
{
  m_errno = Socket::ERROR_OPNOTSUPP;
  return -1;
}

int
PacketSocket::Send (Ptr<Packet> p, uint32_t flags)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_state == STATE_OPEN ||
      m_state == STATE_BOUND)
    {
      m_errno = ERROR_NOTCONN;
      return -1;
    }
  return SendTo (p, flags, m_destAddr);
}

uint32_t
PacketSocket::GetMinMtu (PacketSocketAddress ad) const
{
  if (ad.IsSingleDevice ())
    {
      Ptr<NetDevice> device = m_node->GetDevice (ad.GetSingleDevice ());
      return device->GetMtu ();
    }
  else
    {
      uint32_t minMtu = 0xffff;
      for (uint32_t i = 0; i < m_node->GetNDevices (); i++)
        {
          Ptr<NetDevice> device = m_node->GetDevice (i);
          minMtu = std::min (minMtu, (uint32_t)device->GetMtu ());
        }
      return minMtu;
    }
}

uint32_t 
PacketSocket::GetTxAvailable (void) const
{
  if (m_state == STATE_CONNECTED)
    {
      PacketSocketAddress ad = PacketSocketAddress::ConvertFrom (m_destAddr);      
      return GetMinMtu (ad);
    }
  // If we are not connected, we return a 'safe' value by default.
  return 0xffff;
}

int
PacketSocket::SendTo (Ptr<Packet> p, uint32_t flags, const Address &address)
{
  NS_LOG_FUNCTION_NOARGS ();
  PacketSocketAddress ad;
  if (m_state == STATE_CLOSED)
    {
      NS_LOG_LOGIC ("ERROR_BADF");
      m_errno = ERROR_BADF;
      return -1;
    }
  if (m_shutdownSend)
    {
      NS_LOG_LOGIC ("ERROR_SHUTDOWN");
      m_errno = ERROR_SHUTDOWN;
      return -1;
    }
  if (!PacketSocketAddress::IsMatchingType (address))
    {
      NS_LOG_LOGIC ("ERROR_AFNOSUPPORT");
      m_errno = ERROR_AFNOSUPPORT;
      return -1;
    }
  ad = PacketSocketAddress::ConvertFrom (address);
  if (p->GetSize () > GetMinMtu (ad))
    {
      m_errno = ERROR_MSGSIZE;
      return -1;
    }
  
  bool error = false;
  Address dest = ad.GetPhysicalAddress ();
  if (ad.IsSingleDevice ())
    {
      Ptr<NetDevice> device = m_node->GetDevice (ad.GetSingleDevice ());
      if (!device->Send (p, dest, ad.GetProtocol ()))
        {
          NS_LOG_LOGIC ("error: NetDevice::Send error");
          error = true;
        }
    }
  else
    {
      for (uint32_t i = 0; i < m_node->GetNDevices (); i++)
        {
          Ptr<NetDevice> device = m_node->GetDevice (i);
          if (!device->Send (p, dest, ad.GetProtocol ()))
            {
              NS_LOG_LOGIC ("error: NetDevice::Send error");
              error = true;
            }
        }
    }
  if (!error)
    {
      NotifyDataSent (p->GetSize ());
    }

  if (error)
    {
      NS_LOG_LOGIC ("ERROR_INVAL 2");
      m_errno = ERROR_INVAL;
      return -1;
    }
  else
    {
      return 0;
    }
}

void 
PacketSocket::ForwardUp (Ptr<NetDevice> device, Ptr<const Packet> packet, 
                         uint16_t protocol, const Address &from,
                         const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_shutdownRecv)
    {
      return;
    }


  PacketSocketAddress address;
  address.SetPhysicalAddress (from);
  address.SetSingleDevice (device->GetIfIndex ());
  address.SetProtocol (protocol);

  if ((m_rxAvailable + packet->GetSize ()) <= m_rcvBufSize)
    {
      SocketAddressTag tag;
      tag.SetAddress (address);
      packet->AddTag (tag);
      m_deliveryQueue.push (packet->Copy ());
      m_rxAvailable += packet->GetSize ();
      NS_LOG_LOGIC ("UID is " << packet->GetUid() << " PacketSocket " << this);
      NotifyDataRecv ();
    }
  else
    {
      // In general, this case should not occur unless the
      // receiving application reads data from this socket slowly
      // in comparison to the arrival rate
      //
      // drop and trace packet
      NS_LOG_WARN ("No receive buffer space available.  Drop.");
      m_dropTrace (packet);
    }
}

uint32_t
PacketSocket::GetRxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  // We separately maintain this state to avoid walking the queue 
  // every time this might be called
  return m_rxAvailable;
}

Ptr<Packet> 
PacketSocket::Recv (uint32_t maxSize, uint32_t flags)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_deliveryQueue.empty() )
    {
      return 0;
    }
  Ptr<Packet> p = m_deliveryQueue.front ();
  if (p->GetSize () <= maxSize)
    {
      m_deliveryQueue.pop ();
      m_rxAvailable -= p->GetSize ();
    }
  else
    {
      p = 0;
    }
  return p;
}

Ptr<Packet>
PacketSocket::RecvFrom (uint32_t maxSize, uint32_t flags, Address &fromAddress)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Packet> packet = Recv (maxSize, flags);
  if (packet != 0)
    {
      SocketAddressTag tag;
      bool found;
      found = packet->FindFirstMatchingTag (tag);
      NS_ASSERT (found);
      fromAddress = tag.GetAddress ();
    }
  return packet;
}

}//namespace ns3
