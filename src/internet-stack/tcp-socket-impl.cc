/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 */


#include "ns3/node.h"
#include "ns3/inet-socket-address.h"
#include "ns3/log.h"
#include "ns3/ipv4.h"
#include "tcp-socket-impl.h"
#include "tcp-l4-protocol.h"
#include "ipv4-end-point.h"
#include "ipv4-l4-demux.h"
#include "ns3/simulation-singleton.h"
#include "tcp-typedefs.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"

#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("TcpSocketImpl");

using namespace std;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpSocketImpl);

TypeId
TcpSocketImpl::GetTypeId ()
{
  static TypeId tid = TypeId("ns3::TcpSocketImpl")
    .SetParent<TcpSocket> ()
    .AddTraceSource ("CongestionWindow",
                     "The TCP connection's congestion window",
                     MakeTraceSourceAccessor (&TcpSocketImpl::m_cWnd))
    ;
  return tid;
}

  TcpSocketImpl::TcpSocketImpl ()
  : m_skipRetxResched (false),
    m_dupAckCount (0),
    m_delAckCount (0),
    m_endPoint (0),
    m_node (0),
    m_tcp (0),
    m_errno (ERROR_NOTERROR),
    m_shutdownSend (false),
    m_shutdownRecv (false),
    m_connected (false),
    m_state (CLOSED),
    m_closeNotified (false),
    m_closeRequestNotified (false),
    m_closeOnEmpty (false),
    m_pendingClose (false),
    m_nextTxSequence (0),
    m_highTxMark (0),
    m_highestRxAck (0),
    m_lastRxAck (0),
    m_nextRxSequence (0),
    m_pendingData (0),
    m_rtt (0),
    m_lastMeasuredRtt (Seconds(0.0)),
    m_rxAvailable (0)
{
  NS_LOG_FUNCTION (this);
}

TcpSocketImpl::TcpSocketImpl(const TcpSocketImpl& sock)
  : TcpSocket(sock), //copy object::m_tid, copy socket::callbacks
    m_skipRetxResched (sock.m_skipRetxResched),
    m_dupAckCount (sock.m_dupAckCount),
    m_delAckCount (0),
    m_delAckMaxCount (sock.m_delAckMaxCount),
    m_delAckTimeout (sock.m_delAckTimeout),
    m_endPoint (0),
    m_node (sock.m_node),
    m_tcp (sock.m_tcp),
    m_remoteAddress (sock.m_remoteAddress),
    m_remotePort (sock.m_remotePort),
    m_localAddress (sock.m_localAddress),
    m_localPort (sock.m_localPort),
    m_errno (sock.m_errno),
    m_shutdownSend (sock.m_shutdownSend),
    m_shutdownRecv (sock.m_shutdownRecv),
    m_connected (sock.m_connected),
    m_state (sock.m_state),
    m_closeNotified (sock.m_closeNotified),
    m_closeRequestNotified (sock.m_closeRequestNotified),
    m_closeOnEmpty (sock.m_closeOnEmpty),
    m_pendingClose (sock.m_pendingClose),
    m_nextTxSequence (sock.m_nextTxSequence),
    m_highTxMark (sock.m_highTxMark),
    m_highestRxAck (sock.m_highestRxAck),
    m_lastRxAck (sock.m_lastRxAck),
    m_nextRxSequence (sock.m_nextRxSequence),
    m_pendingData (0),
    m_segmentSize (sock.m_segmentSize),
    m_rxWindowSize (sock.m_rxWindowSize),
    m_advertisedWindowSize (sock.m_advertisedWindowSize),
    m_cWnd (sock.m_cWnd),
    m_ssThresh (sock.m_ssThresh),
    m_initialCWnd (sock.m_initialCWnd),
    m_rtt (0),
    m_lastMeasuredRtt (Seconds(0.0)),
    m_cnTimeout (sock.m_cnTimeout),
    m_cnCount (sock.m_cnCount),
    m_rxAvailable (0),
    m_sndBufSize (sock.m_sndBufSize),
    m_rcvBufSize(sock.m_rcvBufSize)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC("Invoked the copy constructor");
  //copy the pending data if necessary
  if(sock.m_pendingData)
    {
      m_pendingData = sock.m_pendingData->Copy();
    }
  //copy the rtt if necessary
  if (sock.m_rtt)
    {
      m_rtt = sock.m_rtt->Copy();
    }
  //null out the socket base class recvcallback,
  //make user of the socket register this explicitly
  SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > () );
  //can't "copy" the endpoint just yes, must do this when we know the peer info
  //too; this is in SYN_ACK_TX
}

TcpSocketImpl::~TcpSocketImpl ()
{
  NS_LOG_FUNCTION(this);
  m_node = 0;
  if (m_endPoint != 0)
    {
      NS_ASSERT (m_tcp != 0);
      /**
       * Note that this piece of code is a bit tricky:
       * when DeAllocate is called, it will call into
       * Ipv4EndPointDemux::Deallocate which triggers
       * a delete of the associated endPoint which triggers
       * in turn a call to the method ::Destroy below
       * will will zero the m_endPoint field.
       */
      NS_ASSERT (m_endPoint != 0);
      m_tcp->DeAllocate (m_endPoint);
      NS_ASSERT (m_endPoint == 0);
    }
  m_tcp = 0;
  delete m_pendingData; //prevents leak
  m_pendingData = 0;
}

void
TcpSocketImpl::SetNode (Ptr<Node> node)
{
  m_node = node;
  // Initialize some variables 
  m_cWnd = m_initialCWnd * m_segmentSize;
  m_rxWindowSize = m_advertisedWindowSize;
}

void 
TcpSocketImpl::SetTcp (Ptr<TcpL4Protocol> tcp)
{
  m_tcp = tcp;
}
void 
TcpSocketImpl::SetRtt (Ptr<RttEstimator> rtt)
{
  m_rtt = rtt;
}


enum Socket::SocketErrno
TcpSocketImpl::GetErrno (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_errno;
}

Ptr<Node>
TcpSocketImpl::GetNode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

void 
TcpSocketImpl::Destroy (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_endPoint = 0;
  m_tcp = 0;
  m_retxEvent.Cancel ();
}
int
TcpSocketImpl::FinishBind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetRxCallback (MakeCallback (&TcpSocketImpl::ForwardUp, Ptr<TcpSocketImpl>(this)));
  m_endPoint->SetDestroyCallback (MakeCallback (&TcpSocketImpl::Destroy, Ptr<TcpSocketImpl>(this)));
  m_localAddress = m_endPoint->GetLocalAddress ();
  m_localPort = m_endPoint->GetLocalPort ();
  return 0;
}

int
TcpSocketImpl::Bind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_endPoint = m_tcp->Allocate ();
  return FinishBind ();
}
int 
TcpSocketImpl::Bind (const Address &address)
{
  NS_LOG_FUNCTION (this<<address);
  if (!InetSocketAddress::IsMatchingType (address))
    {
      return ERROR_INVAL;
    }
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  Ipv4Address ipv4 = transport.GetIpv4 ();
  uint16_t port = transport.GetPort ();
  if (ipv4 == Ipv4Address::GetAny () && port == 0)
    {
      m_endPoint = m_tcp->Allocate ();
      NS_LOG_LOGIC ("TcpSocketImpl "<<this<<" got an endpoint: "<<m_endPoint);
    }
  else if (ipv4 == Ipv4Address::GetAny () && port != 0)
    {
      m_endPoint = m_tcp->Allocate (port);
      NS_LOG_LOGIC ("TcpSocketImpl "<<this<<" got an endpoint: "<<m_endPoint);
    }
  else if (ipv4 != Ipv4Address::GetAny () && port == 0)
    {
      m_endPoint = m_tcp->Allocate (ipv4);
      NS_LOG_LOGIC ("TcpSocketImpl "<<this<<" got an endpoint: "<<m_endPoint);
    }
  else if (ipv4 != Ipv4Address::GetAny () && port != 0)
    {
      m_endPoint = m_tcp->Allocate (ipv4, port);
      NS_LOG_LOGIC ("TcpSocketImpl "<<this<<" got an endpoint: "<<m_endPoint);
    }

  return FinishBind ();
}

int 
TcpSocketImpl::ShutdownSend (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_shutdownSend = true;
  return 0;
}
int 
TcpSocketImpl::ShutdownRecv (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_shutdownRecv = false;
  return 0;
}

int
TcpSocketImpl::Close (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_state == CLOSED) 
    {
      return -1;
    }
  if (m_pendingData && m_pendingData->Size() != 0)
    { // App close with pending data must wait until all data transmitted
      m_closeOnEmpty = true;
      NS_LOG_LOGIC("Socket " << this << 
                   " deferring close, state " << m_state);
      return 0;
    }

  Actions_t action  = ProcessEvent (APP_CLOSE);
  ProcessAction (action);
  ShutdownSend ();
  return 0;
}

int
TcpSocketImpl::Connect (const Address & address)
{
  NS_LOG_FUNCTION (this << address);
  if (m_endPoint == 0)
    {
      if (Bind () == -1)
        {
          NS_ASSERT (m_endPoint == 0);
          return -1;
        }
      NS_ASSERT (m_endPoint != 0);
    }
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  m_remoteAddress = transport.GetIpv4 ();
  m_remotePort = transport.GetPort ();
  
  uint32_t localIfIndex;
  Ptr<Ipv4> ipv4 = m_node->GetObject<Ipv4> ();

  if (ipv4->GetIfIndexForDestination (m_remoteAddress, localIfIndex))
    {
      m_endPoint->SetLocalAddress (ipv4->GetAddress (localIfIndex));
    }
  else
    {
      m_errno = ERROR_NOROUTETOHOST;
      return -1;
    }

  Actions_t action = ProcessEvent (APP_CONNECT);
  bool success = ProcessAction (action);
  if (success) 
    {
      return 0;
    }
  return -1;
}

//p here is just data, no headers
int 
TcpSocketImpl::Send (Ptr<Packet> p, uint32_t flags) 
{
  NS_LOG_FUNCTION (this << p);
  if (m_state == ESTABLISHED || m_state == SYN_SENT || m_state == CLOSE_WAIT)
  {
    if (p->GetSize() > GetTxAvailable ())
    {
      m_errno = ERROR_MSGSIZE;
      return -1;
    }
    if (!m_pendingData)
    {
      m_pendingData = new PendingData ();   // Create if non-existent
      m_firstPendingSequence = m_nextTxSequence; // Note seq of first
    }
    //PendingData::Add stores a copy of the Ptr p
    m_pendingData->Add (p);
    NS_LOG_DEBUG("TcpSock::Send, pdsize " << m_pendingData->Size() << 
        " state " << m_state);
    Actions_t action = ProcessEvent (APP_SEND);
    NS_LOG_DEBUG(" action " << action);
    if (!ProcessAction (action)) 
    {
      return -1; // Failed, return zero
    }
    return p->GetSize();
  }
  else
  {
    m_errno = ERROR_NOTCONN;
    return -1;
  }
}

int TcpSocketImpl::DoSendTo (Ptr<Packet> p, const Address &address)
{
  NS_LOG_FUNCTION (this << p << address);
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  Ipv4Address ipv4 = transport.GetIpv4 ();
  uint16_t port = transport.GetPort ();
  return DoSendTo (p, ipv4, port);
}

int TcpSocketImpl::DoSendTo (Ptr<Packet> p, Ipv4Address ipv4, uint16_t port)
{
  NS_LOG_FUNCTION (this << p << ipv4 << port);
  if (m_endPoint == 0)
    {
      if (Bind () == -1)
	{
          NS_ASSERT (m_endPoint == 0);
	  return -1;
	}
      NS_ASSERT (m_endPoint != 0);
    }
  if (m_shutdownSend)
    {
      m_errno = ERROR_SHUTDOWN;
      return -1;
    }
  m_tcp->Send (p, m_endPoint->GetLocalAddress (), ipv4,
                  m_endPoint->GetLocalPort (), port);
  NotifyDataSent (p->GetSize ());
  return 0;
}

int 
TcpSocketImpl::SendTo (Ptr<Packet> p, uint32_t flags, const Address &address)
{
  NS_LOG_FUNCTION (this << address << p);
  if (!m_connected)
    {
      m_errno = ERROR_NOTCONN;
      return -1;
    }
  else
    {
      return Send (p, flags); //drop the address according to BSD manpages
    }
}

uint32_t
TcpSocketImpl::GetTxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_pendingData != 0)
    {
      uint32_t unAckedDataSize = 
        m_pendingData->SizeFromSeq (m_firstPendingSequence, m_highestRxAck);
      NS_ASSERT (m_sndBufSize >= unAckedDataSize); //else a logical error
      return m_sndBufSize-unAckedDataSize;
    }
  else
    {
      return m_sndBufSize;
    }
}

int
TcpSocketImpl::Listen (uint32_t q)
{
  NS_LOG_FUNCTION (this << q);
  Actions_t action = ProcessEvent (APP_LISTEN);
  ProcessAction (action);
  return 0;
}

Ptr<Packet>
TcpSocketImpl::Recv (uint32_t maxSize, uint32_t flags)
{
  NS_LOG_FUNCTION_NOARGS ();
  if(m_bufferedData.empty())
    {
      return 0;
    }
  UnAckData_t out; //serves as buffer to return up to the user
  UnAckData_t::iterator i;
  while (!m_bufferedData.empty ())
    { // Check the buffered data for delivery
      NS_LOG_LOGIC("TCP " << this << " bufferedData.size() " 
        << m_bufferedData.size () 
        << " time " << Simulator::Now ());
      i = m_bufferedData.begin ();
      SequenceNumber s1 = 0;
      if (i->first > m_nextRxSequence) 
        {
          break;  // we're done, no more in-sequence data exits
        }
      else // (i->first <= m_nextRxSequence)
        { // Two cases here.
          // 1) seq + length > nextRxSeq, can deliver partial
          // 2) seq + length <= nextRxSeq, deliver whole
          s1 = i->second->GetSize ();
          if (i->first + s1 > m_nextRxSequence)
            { // Remove partial data to prepare for delivery
              uint32_t avail = s1 + i->first - m_nextRxSequence;
              i->second = i->second->CreateFragment (0, avail);
            }
          // else this packet is okay to deliver whole
          // so don't do anything else and output it
          out[i->first]  = i->second;
        }
      m_rxAvailable -= i->second->GetSize ();
      m_bufferedData.erase (i);     // Remove from list
    }
  if (out.size() == 0)
    {
      return 0;
    }
  Ptr<Packet> outPacket = Create<Packet>();
  for(i = out.begin(); i!=out.end(); ++i)
  {
    if (outPacket->GetSize() + i->second->GetSize() <= maxSize )
    {
      outPacket->AddAtEnd(i->second);
    }
    else
    {
      //only append as much as will fit
      uint32_t avail = maxSize - outPacket->GetSize();
      outPacket->AddAtEnd(i->second->CreateFragment(0,avail));
      //put the rest back into the buffer
      m_bufferedData[i->first+SequenceNumber(avail)] 
          = i->second->CreateFragment(avail,i->second->GetSize()-avail);
      m_rxAvailable += i->second->GetSize()-avail;
    }
  }
  return outPacket;
}

uint32_t
TcpSocketImpl::GetRxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  // We separately maintain this state to avoid walking the queue 
  // every time this might be called
  return m_rxAvailable;
}

Ptr<Packet>
TcpSocketImpl::RecvFrom (uint32_t maxSize, uint32_t flags,
  Address &fromAddress)
{
  NS_LOG_FUNCTION (this << maxSize << flags);
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

void
TcpSocketImpl::ForwardUp (Ptr<Packet> packet, Ipv4Address ipv4, uint16_t port)
{
  NS_LOG_DEBUG("Socket " << this << " got forward up" <<
               " dport " << m_endPoint->GetLocalPort() <<
               " daddr " << m_endPoint->GetLocalAddress() <<
               " sport " << m_endPoint->GetPeerPort() <<
               " saddr " << m_endPoint->GetPeerAddress());

  NS_LOG_FUNCTION (this << packet << ipv4 << port);
  if (m_shutdownRecv)
    {
      return;
    }
  TcpHeader tcpHeader;
  packet->RemoveHeader (tcpHeader);

  if (tcpHeader.GetFlags () & TcpHeader::ACK)
    {
      Time m = m_rtt->AckSeq (tcpHeader.GetAckNumber () );
      if (m != Seconds (0.0))
        {
          m_lastMeasuredRtt = m;
        }
    }

  Events_t event = SimulationSingleton<TcpStateMachine>::Get ()->FlagsEvent (tcpHeader.GetFlags () );
  Actions_t action = ProcessEvent (event); //updates the state
  Address address = InetSocketAddress (ipv4, port);
  NS_LOG_DEBUG("Socket " << this << 
               " processing pkt action, " << action <<
               " current state " << m_state);
  ProcessPacketAction (action, packet, tcpHeader, address);
}

Actions_t TcpSocketImpl::ProcessEvent (Events_t e)
{
  NS_LOG_FUNCTION (this << e);
  States_t saveState = m_state;
  NS_LOG_LOGIC ("TcpSocketImpl " << this << " processing event " << e);
  // simulation singleton is a way to get a single global static instance of a
  // class intended to be a singleton; see simulation-singleton.h
  SA stateAction = SimulationSingleton<TcpStateMachine>::Get ()->Lookup (m_state,e);
  // debug
  if (stateAction.action == RST_TX)
    {
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " sending RST from state "
              << saveState << " event " << e);
    }
  bool needCloseNotify = (stateAction.state == CLOSED && m_state != CLOSED 
    && e != TIMEOUT);
  m_state = stateAction.state;
  NS_LOG_LOGIC ("TcpSocketImpl " << this << " moved from state " << saveState 
    << " to state " <<m_state);
  NS_LOG_LOGIC ("TcpSocketImpl " << this << " pendingData " << m_pendingData);

  //extra event logic is here for RX events
  //e = SYN_ACK_RX
  if (saveState == SYN_SENT && m_state == ESTABLISHED)
    // this means the application side has completed its portion of 
    // the handshaking
    {
      Simulator::ScheduleNow(&TcpSocketImpl::ConnectionSucceeded, this);
      //NotifyConnectionSucceeded ();
      m_connected = true;
      m_endPoint->SetPeer (m_remoteAddress, m_remotePort);
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " Connected!");
    }

  if (needCloseNotify && !m_closeNotified)
    {
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " transition to CLOSED from " 
               << m_state << " event " << e << " closeNot " << m_closeNotified
               << " action " << stateAction.action);
      m_closeNotified = true;
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " calling Closed from PE"
              << " origState " << saveState
              << " event " << e);
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " transition to CLOSED from "
          << m_state << " event " << e
          << " set CloseNotif ");
    }
  return stateAction.action;
}

void TcpSocketImpl::SendEmptyPacket (uint8_t flags)
{
  NS_LOG_FUNCTION (this << flags);
  Ptr<Packet> p = Create<Packet> ();
  TcpHeader header;

  header.SetFlags (flags);
  header.SetSequenceNumber (m_nextTxSequence);
  header.SetAckNumber (m_nextRxSequence);
  header.SetSourcePort (m_endPoint->GetLocalPort ());
  header.SetDestinationPort (m_remotePort);
  header.SetWindowSize (m_advertisedWindowSize);
  m_tcp->SendPacket (p, header, m_endPoint->GetLocalAddress (), 
    m_remoteAddress);
  Time rto = m_rtt->RetransmitTimeout ();
  if (flags & TcpHeader::SYN)
    {
      rto = m_cnTimeout;
      m_cnTimeout = m_cnTimeout + m_cnTimeout;
      m_cnCount--;
    }
  if (m_retxEvent.IsExpired () ) //no outstanding timer
  {
    NS_LOG_LOGIC ("Schedule retransmission timeout at time " 
          << Simulator::Now ().GetSeconds () << " to expire at time " 
          << (Simulator::Now () + rto).GetSeconds ());
    m_retxEvent = Simulator::Schedule (rto, &TcpSocketImpl::ReTxTimeout, this);
  }
}

bool TcpSocketImpl::ProcessAction (Actions_t a)
{ // These actions do not require a packet or any TCP Headers
  NS_LOG_FUNCTION (this << a);
  switch (a)
  {
    case NO_ACT:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action: NO_ACT");
      break;
    case ACK_TX:
      SendEmptyPacket (TcpHeader::ACK);
      break;
    case ACK_TX_1:
      NS_ASSERT (false); // This should be processed in ProcessPacketAction
      break;
    case RST_TX:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action RST_TX");
      SendEmptyPacket (TcpHeader::RST);
      break;
    case SYN_TX:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action SYN_TX");
      // TCP SYN Flag consumes one byte
      // is the above correct? we're SENDING a syn, not acking back -- Raj
      // commented out for now
      // m_nextTxSequence+= 1;
      SendEmptyPacket (TcpHeader::SYN);
      break;
    case SYN_ACK_TX:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action SYN_ACK_TX");
      // TCP SYN Flag consumes one byte
      ++m_nextRxSequence;
      SendEmptyPacket (TcpHeader::SYN | TcpHeader::ACK);
      break;
    case FIN_TX:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action FIN_TX");
      SendEmptyPacket (TcpHeader::FIN);
      break;
    case FIN_ACK_TX:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action FIN_ACK_TX");
      SendEmptyPacket (TcpHeader::FIN | TcpHeader::ACK);
      break;
    case NEW_ACK:
      NS_ASSERT (false); // This should be processed in ProcessPacketAction
      break;
    case NEW_SEQ_RX:
      NS_ASSERT (false); // This should be processed in ProcessPacketAction
      break;
    case RETX:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action RETX");
      break;
    case TX_DATA:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action TX_DATA");
      SendPendingData ();
      break;
    case PEER_CLOSE:
      NS_ASSERT (false); // This should be processed in ProcessPacketAction
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action PEER_CLOSE");
      break;
    case APP_CLOSED:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action APP_CLOSED");
      break;
    case CANCEL_TM:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action CANCEL_TM");
      break;
    case APP_NOTIFY:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action APP_NOTIFY");
      break;
    case SERV_NOTIFY:
      NS_ASSERT (false); // This should be processed in ProcessPacketAction
      break;
    case LAST_ACTION:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action LAST_ACTION");
      break;
  }
  return true;
}

bool TcpSocketImpl::ProcessPacketAction (Actions_t a, Ptr<Packet> p,
                                     const TcpHeader& tcpHeader,
                                     const Address& fromAddress)
{
  NS_LOG_FUNCTION (this << a << p  << fromAddress);
  uint32_t localIfIndex;
  Ptr<Ipv4> ipv4 = m_node->GetObject<Ipv4> ();
  switch (a)
  {
    case SYN_ACK_TX:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action SYN_ACK_TX");
//      m_remotePort = InetSocketAddress::ConvertFrom (fromAddress).GetPort ();
//      m_remoteAddress = InetSocketAddress::ConvertFrom (fromAddress).GetIpv4 ();
//       if (ipv4->GetIfIndexForDestination (m_remoteAddress, localIfIndex))
//         {
//           m_localAddress = ipv4->GetAddress (localIfIndex);
//         }
      if (m_state == LISTEN) //this means we should fork a new TcpSocketImpl
        {
          NS_LOG_DEBUG("In SYN_ACK_TX, m_state is LISTEN, this " << this);
          //notify the server that we got a SYN
          // If server refuses connection do nothing
          if (!NotifyConnectionRequest(fromAddress)) return true;
          // Clone the socket
          Ptr<TcpSocketImpl> newSock = Copy ();
          NS_LOG_LOGIC ("Cloned a TcpSocketImpl " << newSock);
          //this listening socket should do nothing more
          Simulator::ScheduleNow (&TcpSocketImpl::CompleteFork, newSock,
                                  p, tcpHeader,fromAddress);
          return true;
        }
        // This is the cloned endpoint
        m_endPoint->SetPeer (m_remoteAddress, m_remotePort);
        if (ipv4->GetIfIndexForDestination (m_remoteAddress, localIfIndex))
          {
            m_localAddress = ipv4->GetAddress (localIfIndex);
            m_endPoint->SetLocalAddress (m_localAddress);
            // Leave local addr in the portmap to any, as the path from
            // remote can change and packets can arrive on different interfaces
            //m_endPoint->SetLocalAddress (Ipv4Address::GetAny());
          }
        // TCP SYN consumes one byte
        m_nextRxSequence = tcpHeader.GetSequenceNumber() + SequenceNumber(1);
        SendEmptyPacket (TcpHeader::SYN | TcpHeader::ACK);
      break;
    case ACK_TX_1:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action ACK_TX_1");
      // TCP SYN consumes one byte
      m_nextRxSequence = tcpHeader.GetSequenceNumber() + SequenceNumber(1);
      m_nextTxSequence = tcpHeader.GetAckNumber ();
      m_firstPendingSequence = m_nextTxSequence;  //bug 166
      NS_LOG_DEBUG ("TcpSocketImpl " << this << " ACK_TX_1" <<
                    " nextRxSeq " << m_nextRxSequence);
      SendEmptyPacket (TcpHeader::ACK);
      m_rxWindowSize = tcpHeader.GetWindowSize ();
      if (tcpHeader.GetAckNumber () > m_highestRxAck)
      {
        m_highestRxAck = tcpHeader.GetAckNumber ();
        // Data freed from the send buffer; notify any blocked sender
        if (GetTxAvailable () > 0)
          {
            NotifySend (GetTxAvailable ());
          }
      }
      SendPendingData ();
      break;
    case NEW_ACK:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action NEW_ACK_TX");
      if (tcpHeader.GetAckNumber () < m_highestRxAck) //old ack, do nothing
      {
        break;
      }
      if (tcpHeader.GetAckNumber () == m_highestRxAck && 
         tcpHeader.GetAckNumber ()  < m_nextTxSequence)
      {
        DupAck (tcpHeader, ++m_dupAckCount);
        break;
      }
      if (tcpHeader.GetAckNumber () > m_highestRxAck)  
        {
          m_dupAckCount = 0;
        }
      NewAck (tcpHeader.GetAckNumber ());
      break;
    case NEW_SEQ_RX:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action NEW_SEQ_RX");
      NewRx (p, tcpHeader, fromAddress); // Process new data received
      break;
    case PEER_CLOSE:
    {
      // First we have to be sure the FIN packet was not received
      // out of sequence.  If so, note pending close and process
      // new sequence rx
      if (tcpHeader.GetSequenceNumber () != m_nextRxSequence)
        { // process close later
          m_pendingClose = true;
          NS_LOG_LOGIC ("TcpSocketImpl " << this << " setting pendingClose" 
            << " rxseq " << tcpHeader.GetSequenceNumber () 
            << " nextRxSeq " << m_nextRxSequence);
          NewRx (p, tcpHeader, fromAddress);
          return true;
        }
      // Now we need to see if any data came with the FIN
      // if so, call NewRx
      if (p->GetSize () != 0)
        {
          NewRx (p, tcpHeader, fromAddress);
        }
      States_t saveState = m_state; // Used to see if app responds
      NS_LOG_LOGIC ("TcpSocketImpl " << this 
          << " peer close, state " << m_state);
      if (!m_closeRequestNotified)
        {
          NS_LOG_LOGIC ("TCP " << this 
              << " calling AppCloseRequest");
          m_closeRequestNotified = true;
        }
      NS_LOG_LOGIC ("TcpSocketImpl " << this 
          << " peer close, state after " << m_state);
      if (m_state == saveState)
        { // Need to ack, the application will close later
          SendEmptyPacket (TcpHeader::ACK);
//               // Also need to re-tx the ack if we
        }
      if (m_state == LAST_ACK)
        {
          NS_LOG_LOGIC ("TcpSocketImpl " << this << " scheduling LATO1");
          m_lastAckEvent = Simulator::Schedule (m_rtt->RetransmitTimeout (),
                                                &TcpSocketImpl::LastAckTimeout,this);
        }
      break;
    }
    case SERV_NOTIFY:
      NS_LOG_LOGIC ("TcpSocketImpl " << this <<" Action SERV_NOTIFY");
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " Connected!");
      NotifyNewConnectionCreated (this, fromAddress);
      m_connected = true; // ! This is bogus; fix when we clone the tcp
      m_endPoint->SetPeer (m_remoteAddress, m_remotePort);
      //treat the connection orientation final ack as a newack
      CommonNewAck (tcpHeader.GetAckNumber (), true);
      break;
    default:
      break;
  }
  return true;
}

void TcpSocketImpl::CompleteFork(Ptr<Packet> p, const TcpHeader& h, const Address& fromAddress)
{
  // Get port and address from peer (connecting host)
  m_remotePort = InetSocketAddress::ConvertFrom (fromAddress).GetPort ();
  m_remoteAddress = InetSocketAddress::ConvertFrom (fromAddress).GetIpv4 ();
  m_endPoint = m_tcp->Allocate (m_localAddress,
                                m_localPort,
                                m_remoteAddress,
                                m_remotePort);
  //the cloned socket with be in listen state, so manually change state
  m_state = SYN_RCVD;
  //equivalent to FinishBind
  m_endPoint->SetRxCallback (MakeCallback (&TcpSocketImpl::ForwardUp, Ptr<TcpSocketImpl>(this)));
  m_endPoint->SetDestroyCallback (MakeCallback (&TcpSocketImpl::Destroy, Ptr<TcpSocketImpl>(this)));
  ProcessPacketAction(SYN_ACK_TX, p, h, fromAddress);
 }

void TcpSocketImpl::ConnectionSucceeded()
{ // We would preferred to have scheduled an event directly to
  // NotifyConnectionSucceeded, but (sigh) these are protected
  // and we can get the address of it :(
  NotifyConnectionSucceeded();
}

bool TcpSocketImpl::SendPendingData (bool withAck)
{
  NS_LOG_FUNCTION (this << withAck);
  NS_LOG_LOGIC ("ENTERING SendPendingData");
  if (!m_pendingData)
    {
      return false; // No data exists
    }
  uint32_t nPacketsSent = 0;
  while (m_pendingData->SizeFromSeq (m_firstPendingSequence, m_nextTxSequence))
    {
      uint32_t w = AvailableWindow ();// Get available window size
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " SendPendingData"
           << " w " << w 
           << " rxwin " << m_rxWindowSize
           << " cWnd " << m_cWnd
           << " segsize " << m_segmentSize
           << " nextTxSeq " << m_nextTxSequence
           << " highestRxAck " << m_highestRxAck 
           << " pd->Size " << m_pendingData->Size ()
           << " pd->SFS " << m_pendingData->SizeFromSeq (m_firstPendingSequence, m_nextTxSequence));

      if (w < m_segmentSize && m_pendingData->Size () > w)
        {
          break; // No more
        }
      uint32_t s = std::min (w, m_segmentSize);  // Send no more than window
      Ptr<Packet> p = m_pendingData->CopyFromSeq (s, m_firstPendingSequence, 
        m_nextTxSequence);
      NS_LOG_LOGIC("TcpSocketImpl " << this << " SendPendingData"
                   << " txseq " << m_nextTxSequence
                   << " s " << s 
                   << " datasize " << p->GetSize() );
      uint8_t flags = 0;
      if (withAck)
        {
          flags |= TcpHeader::ACK;
        }
      uint32_t sz = p->GetSize (); // Size of packet
      uint32_t remainingData = m_pendingData->SizeFromSeq(
          m_firstPendingSequence,
          m_nextTxSequence + SequenceNumber (sz));
      if (m_closeOnEmpty && (remainingData == 0))
        {
          flags = TcpHeader::FIN;
          m_state = FIN_WAIT_1;
        }

      TcpHeader header;
      header.SetFlags (flags);
      header.SetSequenceNumber (m_nextTxSequence);
      header.SetAckNumber (m_nextRxSequence);
      header.SetSourcePort (m_endPoint->GetLocalPort());
      header.SetDestinationPort (m_remotePort);
      if (m_shutdownSend)
        {
          m_errno = ERROR_SHUTDOWN;
          return -1;
        }

      
      if (m_retxEvent.IsExpired () ) //go ahead and schedule the retransmit
        {
            Time rto = m_rtt->RetransmitTimeout (); 
            NS_LOG_LOGIC ("SendPendingData Schedule retransmission timeout at time " << 
              Simulator::Now ().GetSeconds () << " to expire at time " <<
              (Simulator::Now () + rto).GetSeconds () );
          m_retxEvent = Simulator::Schedule (rto,&TcpSocketImpl::ReTxTimeout,this);
        }
      NS_LOG_LOGIC ("About to send a packet with flags: " << flags);
      m_tcp->SendPacket (p, header,
                         m_endPoint->GetLocalAddress (),
                         m_remoteAddress);
      m_rtt->SentSeq(m_nextTxSequence, sz);       // notify the RTT
      // Notify the application
      Simulator::ScheduleNow(&TcpSocketImpl::NotifyDataSent, this, p->GetSize ());
      nPacketsSent++;                             // Count sent this loop
      m_nextTxSequence += sz;                     // Advance next tx sequence
      // Note the high water mark
      m_highTxMark = std::max (m_nextTxSequence, m_highTxMark);
    }
    NS_LOG_LOGIC ("SendPendingData Sent "<<nPacketsSent<<" packets");
  NS_LOG_LOGIC("RETURN SendPendingData");
  return (nPacketsSent>0);
}

uint32_t  TcpSocketImpl::UnAckDataCount ()
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_nextTxSequence - m_highestRxAck;
}

uint32_t  TcpSocketImpl::BytesInFlight ()
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_highTxMark - m_highestRxAck;
}

uint32_t  TcpSocketImpl::Window ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC ("TcpSocketImpl::Window() "<<this);
  return std::min (m_rxWindowSize, m_cWnd.Get());
}

uint32_t  TcpSocketImpl::AvailableWindow ()
{
  NS_LOG_FUNCTION_NOARGS ();
  uint32_t unack = UnAckDataCount (); // Number of outstanding bytes
  uint32_t win = Window ();
  if (win < unack) 
    {
      return 0;  // No space available
    }
  return (win - unack);       // Amount of window space available
}

void TcpSocketImpl::NewRx (Ptr<Packet> p,
                        const TcpHeader& tcpHeader, 
                        const Address& fromAddress)
{
  NS_LOG_FUNCTION (this << p << "tcpHeader " << fromAddress);
  NS_LOG_LOGIC ("TcpSocketImpl " << this << " NewRx,"
                << " seq " << tcpHeader.GetSequenceNumber()
                << " ack " << tcpHeader.GetAckNumber()
                << " p.size is " << p->GetSize () );
  NS_LOG_DEBUG ("TcpSocketImpl " << this <<
                " NewRx," <<
                " seq " << tcpHeader.GetSequenceNumber() <<
                " ack " << tcpHeader.GetAckNumber() <<
                " p.size is " << p->GetSize());
  States_t origState = m_state;
  uint32_t s = p->GetSize ();  // Size of associated data
  if (s == 0)
    {// Nothing to do if no associated data
      return;
    }
  // Log sequence received if enabled
  // NoteTimeSeq(LOG_SEQ_RX, h->sequenceNumber);
  // Three possibilities
  // 1) Received seq is expected, buffer this, update rxAvailable, and ack
  // 2) Received seq is < expected, just re-ack previous
  // 3) Received seq is > expected, just re-ack previous and buffer data
  if (tcpHeader.GetSequenceNumber () == m_nextRxSequence)
    { // If seq is expected seq
      // Trim the end if necessary
      // 1) Update nextRxSeq
      // 2) Buffer this packet so Recv can read it
      // 3) Send the ack
      UnAckData_t::iterator next = m_bufferedData.upper_bound (m_nextRxSequence);
      if (next != m_bufferedData.end ())
      {
        SequenceNumber nextBufferedSeq = next->first;
        if (m_nextRxSequence + SequenceNumber(s) > nextBufferedSeq)
        {//tail end isn't all new, trim enough off the end
          s = nextBufferedSeq - m_nextRxSequence;
        }
      }
      p = p->CreateFragment (0,s);
      m_nextRxSequence += s;           // Advance next expected sequence
      NS_LOG_LOGIC("Case 1, advanced nrxs to " << m_nextRxSequence );
      SocketAddressTag tag;
      tag.SetAddress (fromAddress);
      p->AddTag (tag);
      //buffer this, it'll be read by call to Recv
      UnAckData_t::iterator i = 
          m_bufferedData.find (tcpHeader.GetSequenceNumber () );
      if (i != m_bufferedData.end () ) //we found it already in the buffer
      {
        i->second = 0; // relase reference to already buffered
      }
      // Save for later delivery
      m_bufferedData[tcpHeader.GetSequenceNumber () ] = p;  
      m_rxAvailable += p->GetSize ();
      RxBufFinishInsert (tcpHeader.GetSequenceNumber ());
      NotifyDataRecv ();
      if (m_closeNotified)
        {
          NS_LOG_LOGIC ("Tcp " << this << " HuH?  Got data after closeNotif");
        }
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " adv rxseq by " << s);
      if (m_pendingClose || (origState > ESTABLISHED))
        { // See if we can close now
          if (m_bufferedData.empty())
            {
              ProcessPacketAction (PEER_CLOSE, p, tcpHeader, fromAddress);
            }
        }
    }
  else if (tcpHeader.GetSequenceNumber () > m_nextRxSequence)
    { // Need to buffer this one, but trim off the front and back if necessary
      NS_LOG_LOGIC ("Case 2, buffering " << tcpHeader.GetSequenceNumber () );
      UnAckData_t::iterator previous =
          m_bufferedData.lower_bound (tcpHeader.GetSequenceNumber ());
      SequenceNumber startSeq = tcpHeader.GetSequenceNumber();
      if (previous != m_bufferedData.begin ())
        {
          --previous;
          startSeq = previous->first + SequenceNumber(previous->second->GetSize());
          if (startSeq > tcpHeader.GetSequenceNumber ())
            {
              s = tcpHeader.GetSequenceNumber () + SequenceNumber(s) - startSeq;
            }
          else
            {
              startSeq = tcpHeader.GetSequenceNumber();
            }
        }
      //possibly trim off the end
      UnAckData_t::iterator next = m_bufferedData.upper_bound (tcpHeader.GetSequenceNumber());
      if (next != m_bufferedData.end ())
      {
        SequenceNumber nextBufferedSeq = next->first;
        if (startSeq + SequenceNumber(s) > nextBufferedSeq)
        {//tail end isn't all new either, trim enough off the end
          s = nextBufferedSeq - startSeq;
        }
      }
      p = p->CreateFragment (startSeq - tcpHeader.GetSequenceNumber (),s);
      UnAckData_t::iterator i = 
          m_bufferedData.find (startSeq);
      if (i != m_bufferedData.end () )
        {
          if(p->GetSize() > i->second->GetSize())
          {
            i->second = 0; // relase reference to already buffered
          }
          else
          {
            p = i->second;
          }
        }
      // Save for later delivery
      SocketAddressTag tag;
      tag.SetAddress (fromAddress);
      p->AddTag (tag);
      m_bufferedData[startSeq] = p;  
      i = m_bufferedData.find (startSeq);
      next = i;
      ++next;
      if(next != m_bufferedData.end())
        {
          NS_ASSERT(next->first >= i->first + SequenceNumber(i->second->GetSize ()));
        }
    }
  else if (tcpHeader.GetSequenceNumber () + SequenceNumber(s) > m_nextRxSequence)
    {//parial new data case, only part of the packet is new data
      //trim the beginning
      s = tcpHeader.GetSequenceNumber () + SequenceNumber(s) - m_nextRxSequence; //how much new
      //possibly trim off the end
      UnAckData_t::iterator next = m_bufferedData.upper_bound (m_nextRxSequence);
      if (next != m_bufferedData.end ())
      {
        SequenceNumber nextBufferedSeq = next->first;
        if (m_nextRxSequence + SequenceNumber(s) > nextBufferedSeq)
        {//tail end isn't all new either, trim enough off the end
          s = nextBufferedSeq - m_nextRxSequence;
        }
      }
      p = p->CreateFragment (m_nextRxSequence - tcpHeader.GetSequenceNumber (),s);
      SequenceNumber start = m_nextRxSequence;
      m_nextRxSequence += s;           // Advance next expected sequence
      SocketAddressTag tag;
      tag.SetAddress (fromAddress);
      p->AddTag (tag);
      //buffer the new fragment, it'll be read by call to Recv
      UnAckData_t::iterator i = m_bufferedData.find (start);
      if (i != m_bufferedData.end () ) //we found it already in the buffer
        {
          i->second = 0; // relase reference to already buffered
        }
      // Save for later delivery
      m_bufferedData[start] = p;
      m_rxAvailable += p->GetSize ();
      RxBufFinishInsert(start);
      NotifyDataRecv ();
    }
  else
    { // debug
      NS_LOG_LOGIC("TCP " << this 
               << " got seq " << tcpHeader.GetSequenceNumber ()
               << " expected " << m_nextRxSequence
               << "       flags " << tcpHeader.GetFlags ());
    }
  // Now send a new ack packet acknowledging all received and delivered data
  if(++m_delAckCount >= m_delAckMaxCount)
  {
    m_delAckEvent.Cancel();
    m_delAckCount = 0;
    SendEmptyPacket (TcpHeader::ACK);
  }
  else
  {
    m_delAckEvent = Simulator::Schedule (m_delAckTimeout, &TcpSocketImpl::DelAckTimeout, this);
  }
}

void TcpSocketImpl::RxBufFinishInsert (SequenceNumber seq)
{
  //putting data into the buffer might have filled in a sequence gap so we have
  //to iterate through the list to find the largest contiguous sequenced chunk,
  //and update m_rxAvailable and m_nextRxSequence appropriately
  UnAckData_t::iterator i = m_bufferedData.find (seq);
  UnAckData_t::iterator next = i;
  ++next;
  //make sure the buffer is logically sequenced
  if(next != m_bufferedData.end())
  {
    NS_ASSERT(next->first >= i->first + SequenceNumber(i->second->GetSize ()));
  }
  while(next != m_bufferedData.end())
  {
    if(i->first + SequenceNumber(i->second->GetSize ()) == next->first)
    {
      //next packet is in sequence, count it
      m_rxAvailable += next->second->GetSize();
      m_nextRxSequence += next->second->GetSize();
    }
    else
    {
      break; //no more in this contiguous chunk
    }
    ++i;
    ++next;
  }
}

void TcpSocketImpl::DelAckTimeout ()
{
  m_delAckCount = 0;
  SendEmptyPacket (TcpHeader::ACK);
}

void TcpSocketImpl::CommonNewAck (SequenceNumber ack, bool skipTimer)
{ // CommonNewAck is called only for "New" (non-duplicate) acks
  // and MUST be called by any subclass, from the NewAck function
  // Always cancel any pending re-tx timer on new acknowledgement
  NS_LOG_FUNCTION (this << ack << skipTimer); 
  //DEBUG(1,(cout << "TCP " << this << "Cancelling retx timer " << endl));
  if (!skipTimer)
    {
      m_retxEvent.Cancel ();  
      //On recieving a "New" ack we restart retransmission timer .. RFC 2988
      Time rto = m_rtt->RetransmitTimeout ();
      NS_LOG_LOGIC ("Schedule retransmission timeout at time " 
          << Simulator::Now ().GetSeconds () << " to expire at time " 
          << (Simulator::Now () + rto).GetSeconds ());
    m_retxEvent = Simulator::Schedule (rto, &TcpSocketImpl::ReTxTimeout, this);
    }
  NS_LOG_LOGIC ("TCP " << this << " NewAck " << ack 
           << " numberAck " << (ack - m_highestRxAck)); // Number bytes ack'ed
  m_highestRxAck = ack;         // Note the highest recieved Ack
  if (GetTxAvailable () > 0)
    {
      NotifySend (GetTxAvailable ());
    }
  if (ack > m_nextTxSequence) 
    {
      m_nextTxSequence = ack; // If advanced
    }
  // See if all pending ack'ed; if so we can delete the data
  if (m_pendingData)
    { // Data exists, see if can be deleted
      if (m_pendingData->SizeFromSeq (m_firstPendingSequence, m_highestRxAck) == 0)
        { // All pending acked, can be deleted
          m_pendingData->Clear ();
          delete m_pendingData;
          m_pendingData = 0;
          // Insure no re-tx timer
          m_retxEvent.Cancel ();
        }
    }
  // Try to send more data
  SendPendingData();
}

Ptr<TcpSocketImpl> TcpSocketImpl::Copy ()
{
  return CopyObject<TcpSocketImpl> (this);
}

void TcpSocketImpl::NewAck (SequenceNumber seq)
{ // New acknowledgement up to sequence number "seq"
  // Adjust congestion window in response to new ack's received
  NS_LOG_FUNCTION (this << seq);
  NS_LOG_LOGIC ("TcpSocketImpl " << this << " NewAck "
           << " seq " << seq
           << " cWnd " << m_cWnd
           << " ssThresh " << m_ssThresh);
  if (m_cWnd < m_ssThresh)
    { // Slow start mode, add one segSize to cWnd
      m_cWnd += m_segmentSize;
      NS_LOG_LOGIC ("TcpSocketImpl " << this << " NewCWnd SlowStart, cWnd " << m_cWnd 
          << " sst " << m_ssThresh);
    }
  else
    { // Congestion avoidance mode, adjust by (ackBytes*segSize) / cWnd
      double adder =  ((double) m_segmentSize * m_segmentSize) / m_cWnd.Get();
      if (adder < 1.0) 
        {
          adder = 1.0;
        }
      m_cWnd += (uint32_t) adder;
      NS_LOG_LOGIC ("NewCWnd CongAvoid, cWnd " << m_cWnd 
           << " sst " << m_ssThresh);
    }
  CommonNewAck (seq, false);           // Complete newAck processing
}

void TcpSocketImpl::DupAck (const TcpHeader& t, uint32_t count)
{
  NS_LOG_FUNCTION (this << "t " << count);
  NS_LOG_LOGIC ("TcpSocketImpl " << this << " DupAck " <<  t.GetAckNumber ()
      << ", count " << count
      << ", time " << Simulator::Now ());
  if (count == 3)
  { // Count of three indicates triple duplicate ack
    m_ssThresh = Window () / 2; // Per RFC2581
    m_ssThresh = std::max (m_ssThresh, 2 * m_segmentSize);
    NS_LOG_LOGIC("TcpSocketImpl " << this << "Tahoe TDA, time " << Simulator::Now ()
        << " seq " << t.GetAckNumber ()
        << " in flight " << BytesInFlight ()
        << " new ssthresh " << m_ssThresh);

    m_cWnd = m_segmentSize; // Collapse cwnd (re-enter slowstart)
    // For Tahoe, we also reset nextTxSeq
    m_nextTxSequence = m_highestRxAck;
    SendPendingData ();
  }
}

void TcpSocketImpl::ReTxTimeout ()
{ // Retransmit timeout
  NS_LOG_FUNCTION (this);
  m_ssThresh = Window () / 2; // Per RFC2581
  m_ssThresh = std::max (m_ssThresh, 2 * m_segmentSize);
  // Set cWnd to segSize on timeout,  per rfc2581
  // Collapse congestion window (re-enter slowstart)
  m_cWnd = m_segmentSize;           
  m_nextTxSequence = m_highestRxAck; // Start from highest Ack
  m_rtt->IncreaseMultiplier (); // DoubleValue timeout value for next retx timer
  Retransmit ();             // Retransmit the packet
}

void TcpSocketImpl::LastAckTimeout ()
{
  m_lastAckEvent.Cancel ();
  if (m_state == LAST_ACK)
    {
      Actions_t action = ProcessEvent (TIMEOUT);
      ProcessAction (action);
    }
  if (!m_closeNotified)
    {
      m_closeNotified = true;
    }
}

void TcpSocketImpl::Retransmit ()
{
  NS_LOG_FUNCTION (this);
  uint8_t flags = TcpHeader::NONE;
  if (m_state == SYN_SENT) 
    {
      if (m_cnCount > 0) 
        {
          SendEmptyPacket (TcpHeader::SYN);
          return;
        }
      else
        {
          NotifyConnectionFailed ();
          return;
        }
    } 
  if (!m_pendingData)
    {
      if (m_state == FIN_WAIT_1)
        { // Must have lost FIN, re-send
          SendEmptyPacket (TcpHeader::FIN);
        }
      return;
    }
  Ptr<Packet> p = m_pendingData->CopyFromSeq (m_segmentSize,
                                            m_firstPendingSequence,
                                            m_highestRxAck);
  // Calculate remaining data for COE check
  uint32_t remainingData = m_pendingData->SizeFromSeq (
      m_firstPendingSequence,
      m_nextTxSequence + SequenceNumber(p->GetSize ()));
  if (m_closeOnEmpty && remainingData == 0)
    { // Add the FIN flag
      flags = flags | TcpHeader::FIN;
    }

  NS_LOG_LOGIC ("TcpSocketImpl " << this << " retxing seq " << m_highestRxAck);
  if (m_retxEvent.IsExpired () )
    {
      Time rto = m_rtt->RetransmitTimeout ();
      NS_LOG_LOGIC ("Schedule retransmission timeout at time "
          << Simulator::Now ().GetSeconds () << " to expire at time "
          << (Simulator::Now () + rto).GetSeconds ());
      m_retxEvent = Simulator::Schedule (rto,&TcpSocketImpl::ReTxTimeout,this);
    }
  m_rtt->SentSeq (m_highestRxAck,p->GetSize ());
  // And send the packet
  TcpHeader tcpHeader;
  tcpHeader.SetSequenceNumber (m_nextTxSequence);
  tcpHeader.SetAckNumber (m_nextRxSequence);
  tcpHeader.SetSourcePort (m_endPoint->GetLocalPort());
  tcpHeader.SetDestinationPort (m_remotePort);
  tcpHeader.SetFlags (flags);
  tcpHeader.SetWindowSize (m_advertisedWindowSize);

  m_tcp->SendPacket (p, tcpHeader, m_endPoint->GetLocalAddress (),
    m_remoteAddress);
}

void
TcpSocketImpl::SetSndBufSize (uint32_t size)
{
  m_sndBufSize = size;
}

uint32_t
TcpSocketImpl::GetSndBufSize (void) const
{
  return m_sndBufSize;
}

void
TcpSocketImpl::SetRcvBufSize (uint32_t size)
{
  m_rcvBufSize = size;
}

uint32_t
TcpSocketImpl::GetRcvBufSize (void) const
{
  return m_rcvBufSize;
}

void
TcpSocketImpl::SetSegSize (uint32_t size)
{
  m_segmentSize = size;
}

uint32_t
TcpSocketImpl::GetSegSize (void) const
{
  return m_segmentSize;
}

void
TcpSocketImpl::SetAdvWin (uint32_t window)
{
  m_advertisedWindowSize = window;
}

uint32_t
TcpSocketImpl::GetAdvWin (void) const
{
  return m_advertisedWindowSize;
}

void
TcpSocketImpl::SetSSThresh (uint32_t threshold)
{
  m_ssThresh = threshold;
}

uint32_t
TcpSocketImpl::GetSSThresh (void) const
{
  return m_ssThresh;
}

void
TcpSocketImpl::SetInitialCwnd (uint32_t cwnd)
{
  m_initialCWnd = cwnd;
}

uint32_t
TcpSocketImpl::GetInitialCwnd (void) const
{
  return m_initialCWnd;
}

void 
TcpSocketImpl::SetConnTimeout (Time timeout)
{
  m_cnTimeout = timeout;
}

Time
TcpSocketImpl::GetConnTimeout (void) const
{
  return m_cnTimeout;
}

void 
TcpSocketImpl::SetConnCount (uint32_t count)
{
  m_cnCount = count;
}

uint32_t 
TcpSocketImpl::GetConnCount (void) const
{
  return m_cnCount;
}

void 
TcpSocketImpl::SetDelAckTimeout (Time timeout)
{
  m_delAckTimeout = timeout;
}

Time
TcpSocketImpl::GetDelAckTimeout (void) const
{
  return m_delAckTimeout;
}

void
TcpSocketImpl::SetDelAckMaxCount (uint32_t count)
{
  m_delAckMaxCount = count;
}

uint32_t
TcpSocketImpl::GetDelAckMaxCount (void) const
{
  return m_delAckMaxCount;
}

}//namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "internet-stack.h"
#include <string>

namespace ns3 {
	
class TcpSocketImplTest: public Test
{
  public:
  TcpSocketImplTest ();
  virtual bool RunTests (void);
};

TcpSocketImplTest::TcpSocketImplTest ()
  : Test ("TcpSocketImpl")
{
}

bool
TcpSocketImplTest::RunTests (void)
{
	bool result = true;
	return result;
}

static TcpSocketImplTest gTcpSocketImplTest;

}//namespace ns3

#endif /* RUN_SELF_TESTS */