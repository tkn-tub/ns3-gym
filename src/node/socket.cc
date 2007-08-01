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
#include "socket.h"
#include "ns3/packet.h"

namespace ns3 {

Socket::~Socket ()
{}

void 
Socket::SetCloseCallback (Callback<void,Ptr<Socket> > closeCompleted)
{
  m_closeCompleted = closeCompleted;
}
void 
Socket::SetConnectCallback (Callback<void, Ptr<Socket> > connectionSucceeded,
			    Callback<void, Ptr<Socket> > connectionFailed,
			    Callback<void, Ptr<Socket> > halfClose)
{
  m_connectionSucceeded = connectionSucceeded;
  m_connectionFailed = connectionFailed;
  m_halfClose = halfClose;
}
void 
Socket::SetAcceptCallback (Callback<bool, Ptr<Socket>, const Address &> connectionRequest,
			   Callback<void, Ptr<Socket>, const Address&> newConnectionCreated,
			   Callback<void, Ptr<Socket> > closeRequested)
{
  m_connectionRequest = connectionRequest;
  m_newConnectionCreated = newConnectionCreated;
  m_closeRequested = closeRequested;
}
void 
Socket::SetSendCallback (Callback<void, Ptr<Socket>, uint32_t> dataSent)
{
  m_dataSent = dataSent;
}
void 
Socket::SetRecvCallback (Callback<void, Ptr<Socket>, const uint8_t*, uint32_t,const Address&> receivedData,
			 Callback<void, Ptr<Socket>, uint32_t,const Address&> receivedDummyData)
{
  m_receivedData = receivedData;
  m_receivedDummyData = receivedDummyData;
}

void 
Socket::NotifyCloseCompleted (void)
{
  if (!m_closeCompleted.IsNull ())
    {
      m_closeCompleted (this);
    }
}
void 
Socket::NotifyConnectionSucceeded (void)
{
  if (!m_connectionSucceeded.IsNull ())
    {
      m_connectionSucceeded (this);
    }
}
void 
Socket::NotifyConnectionFailed (void)
{
  if (!m_connectionFailed.IsNull ())
    {
      m_connectionFailed (this);
    }
}
void 
Socket::NotifyHalfClose (void)
{
  if (!m_halfClose.IsNull ())
    {
      m_halfClose (this);
    }
}
bool 
Socket::NotifyConnectionRequest (const Address &from)
{
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
  if (!m_newConnectionCreated.IsNull ())
    {
      m_newConnectionCreated (socket, from);
    }
}
void 
Socket::NotifyCloseRequested (void)
{
  if (!m_closeRequested.IsNull ())
    {
      m_closeRequested (this);
    }
}
void 
Socket::NotifyDataSent (uint32_t size)
{
  if (!m_dataSent.IsNull ())
    {
      m_dataSent (this, size);
    }
}
void 
Socket::NotifyDataReceived (const Packet &p, const Address &from)
{
  if (!m_receivedData.IsNull ())
    {
      m_receivedData (this, p.PeekData (), p.GetSize (), from);
    }
  if (!m_receivedDummyData.IsNull ())
    {
      m_receivedDummyData (this, p.GetSize (), from);
    }
}



}//namespace ns3
