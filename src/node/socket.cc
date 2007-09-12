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

#include "ns3/debug.h"
#include "ns3/packet.h"
#include "socket.h"

NS_DEBUG_COMPONENT_DEFINE ("Socket");

namespace ns3 {

Socket::~Socket ()
{
  NS_DEBUG("Socket::~Socket ()");
}

void 
Socket::SetCloseCallback (Callback<void,Ptr<Socket> > closeCompleted)
{
  NS_DEBUG("Socket::SetCloseCallback ()");
  m_closeCompleted = closeCompleted;
}

void 
Socket::SetConnectCallback (
  Callback<void, Ptr<Socket> > connectionSucceeded,
  Callback<void, Ptr<Socket> > connectionFailed,
  Callback<void, Ptr<Socket> > halfClose)
{
  NS_DEBUG("Socket::SetConnectCallback ()");
  m_connectionSucceeded = connectionSucceeded;
  m_connectionFailed = connectionFailed;
  m_halfClose = halfClose;
}

void 
Socket::SetAcceptCallback (
  Callback<bool, Ptr<Socket>, const Address &> connectionRequest,
  Callback<void, Ptr<Socket>, const Address&> newConnectionCreated,
  Callback<void, Ptr<Socket> > closeRequested)
{
  NS_DEBUG("Socket::SetAcceptCallback ()");
  m_connectionRequest = connectionRequest;
  m_newConnectionCreated = newConnectionCreated;
  m_closeRequested = closeRequested;
}

void 
Socket::SetSendCallback (Callback<void, Ptr<Socket>, uint32_t> dataSent)
{
  NS_DEBUG("Socket::SetSendCallback ()");
  m_dataSent = dataSent;
}

void 
Socket::SetRecvCallback (Callback<void, Ptr<Socket>, const Packet &,const Address&> receivedData)
{
  NS_DEBUG("Socket::SetRecvCallback ()");
  m_receivedData = receivedData;
}

void 
Socket::NotifyCloseCompleted (void)
{
  NS_DEBUG("Socket::NotifyCloseCompleted ()");

  if (!m_closeCompleted.IsNull ())
    {
      m_closeCompleted (this);
    }
}

void 
Socket::NotifyConnectionSucceeded (void)
{
  NS_DEBUG("Socket::NotifyConnectionSucceeded ()");

  if (!m_connectionSucceeded.IsNull ())
    {
      m_connectionSucceeded (this);
    }
}

void 
Socket::NotifyConnectionFailed (void)
{
  NS_DEBUG("Socket::NotifyConnectionFailed ()");

  if (!m_connectionFailed.IsNull ())
    {
      m_connectionFailed (this);
    }
}

void 
Socket::NotifyHalfClose (void)
{
  NS_DEBUG("Socket::NotifyHalfClose ()");

  if (!m_halfClose.IsNull ())
    {
      m_halfClose (this);
    }
}

bool 
Socket::NotifyConnectionRequest (const Address &from)
{
  NS_DEBUG("Socket::NotifyConnectionRequest ()");

  if (!m_connectionRequest.IsNull ())
    {
      return m_connectionRequest (this, from);
    }
  else
    {
      // refuse all incomming connections by default.
      return false;
    }
}

void 
Socket::NotifyNewConnectionCreated (Ptr<Socket> socket, const Address &from)
{
  NS_DEBUG("Socket::NotifyNewConnectionCreated ()");

  if (!m_newConnectionCreated.IsNull ())
    {
      m_newConnectionCreated (socket, from);
    }
}

void 
Socket::NotifyCloseRequested (void)
{
  NS_DEBUG("Socket::NotifyCloseRequested ()");

  if (!m_closeRequested.IsNull ())
    {
      m_closeRequested (this);
    }
}

void 
Socket::NotifyDataSent (uint32_t size)
{
  NS_DEBUG("Socket::NotifyDataSent ()");

  if (!m_dataSent.IsNull ())
    {
      m_dataSent (this, size);
    }
}

void 
Socket::NotifyDataReceived (const Packet &p, const Address &from)
{
  NS_DEBUG("Socket::NotifyDataReceived ()");

  if (!m_receivedData.IsNull ())
    {
      m_receivedData (this, p, from);
    }
}

}//namespace ns3
