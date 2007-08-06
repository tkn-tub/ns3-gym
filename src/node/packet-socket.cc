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
#include "ns3/debug.h"
#include "ns3/node.h"

NS_DEBUG_COMPONENT_DEFINE ("PacketSocket");

namespace ns3 {

PacketSocket::PacketSocket (Ptr<Node> node)
  : m_node (node)
{
  Init();
}

void 
PacketSocket::Init()
{
  m_state = STATE_OPEN;
  m_shutdownSend = false;
  m_shutdownRecv = false;
  m_errno = ERROR_NOTERROR;
}

PacketSocket::~PacketSocket ()
{}

void 
PacketSocket::DoDispose (void)
{
  m_device = 0;
}

Ptr<Node>
PacketSocket::GetNode (void) const
{
  return m_node;
}


int
PacketSocket::Bind (void)
{
  PacketSocketAddress address;
  address.SetProtocol (0);
  address.SetAllDevices ();
  return DoBind (address);
}
int
PacketSocket::Bind (const Address &address)
{
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
      dev = 0;
    }
  else
    {
      m_node->GetDevice (address.GetSingleDevice ());
    }
  m_node->RegisterProtocolHandler (MakeCallback (&PacketSocket::ForwardUp, this),
                                   address.GetProtocol (), dev);
  m_state = STATE_BOUND;
  m_protocol = address.GetProtocol ();
  m_isSingleDevice = address.IsSingleDevice ();
  m_device = address.GetSingleDevice ();
  return 0;
}

enum Socket::SocketErrno
PacketSocket::GetErrno (void) const
{
  return m_errno;
}
int
PacketSocket::ShutdownSend (void)
{
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
  if (m_state == STATE_CLOSED)
    {
      m_errno = ERROR_BADF;
      return -1;
    }
  m_shutdownRecv = false;
  return 0;
}
int
PacketSocket::DoClose(ns3::Callback<void, Ptr<Socket> > closeCompleted)
{
  if (m_state == STATE_CLOSED)
    {
      m_errno = ERROR_BADF;
      return -1;
    }
  if (!closeCompleted.IsNull ())
    {
      closeCompleted (this);
    }
  m_state = STATE_CLOSED;
  return 0;
}

int
PacketSocket::DoConnect(const Address &ad,
                        ns3::Callback<void, Ptr<Socket> > connectionSucceeded,
                        ns3::Callback<void, Ptr<Socket> > connectionFailed,
                        ns3::Callback<void, Ptr<Socket> > halfClose)
{
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
  if (!connectionSucceeded.IsNull ())
    {
      connectionSucceeded (this);
    }
  return 0;
 error:
  if (!connectionFailed.IsNull ())
    {
      connectionFailed (this);
    }
  return -1;
}

int
PacketSocket::DoAccept(ns3::Callback<bool, Ptr<Socket>, const Address &> connectionRequest,
                       ns3::Callback<void, Ptr<Socket>, const Address &> newConnectionCreated,
                       ns3::Callback<void, Ptr<Socket> > closeRequested)
{
  // calling accept on a packet socket is a programming error.
  m_errno = ERROR_OPNOTSUPP;
  return -1;
}

int
PacketSocket::DoSend (const uint8_t* buffer,
                      uint32_t size,
                      ns3::Callback<void, Ptr<Socket>, uint32_t> dataSent)
{
  if (m_state == STATE_OPEN ||
      m_state == STATE_BOUND)
    {
      m_errno = ERROR_NOTCONN;
      return -1;
    }
  return DoSendTo (m_destAddr, buffer, size, dataSent);
}

int
PacketSocket::DoSendTo(const Address &address,
                       const uint8_t *buffer,
                       uint32_t size,
                       Callback<void, Ptr<Socket>, uint32_t> dataSent)
{
  PacketSocketAddress ad;
  if (m_state == STATE_CLOSED)
    {
      m_errno = ERROR_BADF;
      return -1;
    }
  if (m_state == STATE_OPEN)
    {
      // XXX should return another error here.
      m_errno = ERROR_INVAL;
      return -1;
    }
  if (m_shutdownSend)
    {
      m_errno = ERROR_SHUTDOWN;
      return -1;
    }
  if (!PacketSocketAddress::IsMatchingType (address))
    {
      m_errno = ERROR_AFNOSUPPORT;
      return -1;
    }
  ad = PacketSocketAddress::ConvertFrom (address);

  Packet p;
  if (buffer == 0)
    {
      p = Packet (size);
    }
  else
    {
      p = Packet (buffer, size);
    }
  
  bool error = false;
  Address dest = ad.GetPhysicalAddress ();
  if (ad.IsSingleDevice ())
    {
      Ptr<NetDevice> device = m_node->GetDevice (ad.GetSingleDevice ());
      if (!device->Send (p, dest, ad.GetProtocol ()))
        {
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
              error = true;
            }
        }
    }
  if (!error && !dataSent.IsNull ())
    {
      dataSent (this, p.GetSize ());
    }

  if (error)
    {
      m_errno = ERROR_INVAL;
      return -1;
    }
  else
    {
      return 0;
    }
}

void 
PacketSocket::DoRecv(ns3::Callback<void, Ptr<Socket>, const uint8_t*, uint32_t,const Address &> callback)
{
  m_rxCallback = callback;
}

void 
PacketSocket::DoRecvDummy(ns3::Callback<void, Ptr<Socket>, uint32_t, const Address &> callback)
{
  m_dummyRxCallback = callback;
}

void 
PacketSocket::ForwardUp (Ptr<NetDevice> device, const Packet &packet, 
                         uint16_t protocol, const Address &from)
{
  if (m_shutdownRecv)
    {
      return;
    }

  Packet p = packet;

  PacketSocketAddress address;
  address.SetPhysicalAddress (from);
  address.SetSingleDevice (device->GetIfIndex ());
  address.SetProtocol (protocol);

  NS_DEBUG ("PacketSocket::ForwardUp: UID is " << packet.GetUid()
            << " PacketSocket " << this);
  if (!m_dummyRxCallback.IsNull ())
    {
      m_dummyRxCallback (this, p.GetSize (), address);
    }
  if (!m_rxCallback.IsNull ())
    {
      m_rxCallback (this, p.PeekData (), p.GetSize (), address);
    }
}

}//namespace ns3
