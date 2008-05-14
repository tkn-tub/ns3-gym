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

NS_LOG_COMPONENT_DEFINE ("PacketSocket");

namespace ns3 {

PacketSocket::PacketSocket ()
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
  NotifyCloseCompleted ();
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
PacketSocket::Send (Ptr<Packet> p)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_state == STATE_OPEN ||
      m_state == STATE_BOUND)
    {
      m_errno = ERROR_NOTCONN;
      return -1;
    }
  return SendTo (m_destAddr, p);
}

int
PacketSocket::SendTo(const Address &address, Ptr<Packet> p)
{
  NS_LOG_FUNCTION_NOARGS ();
  PacketSocketAddress ad;
  if (m_state == STATE_CLOSED)
    {
      NS_LOG_LOGIC ("ERROR_BADF");
      m_errno = ERROR_BADF;
      return -1;
    }
  if (m_state == STATE_OPEN)
    {
      // XXX should return another error here.
      NS_LOG_LOGIC ("ERROR_INVAL");
      m_errno = ERROR_INVAL;
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
PacketSocket::ForwardUp (Ptr<NetDevice> device, Ptr<Packet> packet, 
                         uint16_t protocol, const Address &from)
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

  NS_LOG_LOGIC ("UID is " << packet->GetUid() << " PacketSocket " << this);
  NotifyDataReceived (packet, address);
}

}//namespace ns3
