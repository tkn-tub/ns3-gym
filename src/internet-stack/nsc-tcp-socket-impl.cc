/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * based on tcp-socket-impl.cc, Author: Raj Bhattacharjea <raj.b@gatech.edu>
 * Author: Florian Westphal <fw@strlen.de>
 */

#include "ns3/node.h"
#include "ns3/inet-socket-address.h"
#include "ns3/log.h"
#include "ns3/ipv4.h"
#include "ipv4-end-point.h"
#include "nsc-tcp-l4-protocol.h"
#include "nsc-tcp-socket-impl.h"
#include "ns3/simulation-singleton.h"
#include "tcp-typedefs.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"

#include <algorithm>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "sim_interface.h"
#include "sim_errno.h"

NS_LOG_COMPONENT_DEFINE ("NscTcpSocketImpl");

using namespace std;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NscTcpSocketImpl);

TypeId
NscTcpSocketImpl::GetTypeId ()
{
  static TypeId tid = TypeId("ns3::NscTcpSocketImpl")
    .SetParent<TcpSocket> ()
    .AddTraceSource ("CongestionWindow",
                     "The TCP connection's congestion window",
                     MakeTraceSourceAccessor (&NscTcpSocketImpl::m_cWnd))
    ;
  return tid;
}

  NscTcpSocketImpl::NscTcpSocketImpl ()
  : m_endPoint (0),
    m_node (0),
    m_tcp (0),
    m_peerAddress ("0.0.0.0", 0),
    m_errno (ERROR_NOTERROR),
    m_shutdownSend (false),
    m_shutdownRecv (false),
    m_connected (false),
    m_state (CLOSED),
    m_closeOnEmpty (false),
    m_txBufferSize (0),
    m_rtt (0),
    m_lastMeasuredRtt (Seconds(0.0))
{
  NS_LOG_FUNCTION (this);
}

NscTcpSocketImpl::NscTcpSocketImpl(const NscTcpSocketImpl& sock)
  : TcpSocket(sock), //copy the base class callbacks
    m_delAckMaxCount (sock.m_delAckMaxCount),
    m_delAckTimeout (sock.m_delAckTimeout),
    m_endPoint (0),
    m_node (sock.m_node),
    m_tcp (sock.m_tcp),
    m_remoteAddress (sock.m_remoteAddress),
    m_remotePort (sock.m_remotePort),
    m_localAddress (sock.m_localAddress),
    m_localPort (sock.m_localPort),
    m_peerAddress (sock.m_peerAddress),
    m_errno (sock.m_errno),
    m_shutdownSend (sock.m_shutdownSend),
    m_shutdownRecv (sock.m_shutdownRecv),
    m_connected (sock.m_connected),
    m_state (sock.m_state),
    m_closeOnEmpty (sock.m_closeOnEmpty),
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
    m_nscTcpSocket (0),
    m_sndBufSize (sock.m_sndBufSize)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC("Invoked the copy constructor");
  //copy the pending data if necessary
  if(!sock.m_txBuffer.empty () )
    {
      m_txBuffer = sock.m_txBuffer;
    }
  //copy the rtt if necessary
  if (sock.m_rtt)
    {
      m_rtt = sock.m_rtt->Copy();
    }
  //can't "copy" the endpoint just yes, must do this when we know the peer info
  //too; this is in SYN_ACK_TX
}

NscTcpSocketImpl::~NscTcpSocketImpl ()
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
}

void
NscTcpSocketImpl::SetNode (Ptr<Node> node)
{
  m_node = node;
  // Initialize some variables 
  m_cWnd = m_initialCWnd * m_segmentSize;
  m_rxWindowSize = m_advertisedWindowSize;
}

void 
NscTcpSocketImpl::SetTcp (Ptr<NscTcpL4Protocol> tcp)
{
  m_nscTcpSocket = tcp->m_nscStack->new_tcp_socket();
  m_tcp = tcp;
}
void 
NscTcpSocketImpl::SetRtt (Ptr<RttEstimator> rtt)
{
  m_rtt = rtt;
}


enum Socket::SocketErrno
NscTcpSocketImpl::GetErrno (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_errno;
}

Ptr<Node>
NscTcpSocketImpl::GetNode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

void 
NscTcpSocketImpl::Destroy (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_endPoint = 0;
  m_tcp = 0;
}
int
NscTcpSocketImpl::FinishBind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetRxCallback (MakeCallback (&NscTcpSocketImpl::ForwardUp, Ptr<NscTcpSocketImpl>(this)));
  m_endPoint->SetDestroyCallback (MakeCallback (&NscTcpSocketImpl::Destroy, Ptr<NscTcpSocketImpl>(this)));
  m_localAddress = m_endPoint->GetLocalAddress ();
  m_localPort = m_endPoint->GetLocalPort ();
  return 0;
}

int
NscTcpSocketImpl::Bind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_endPoint = m_tcp->Allocate ();
  return FinishBind ();
}
int 
NscTcpSocketImpl::Bind (const Address &address)
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

  m_localPort = port;
  return FinishBind ();
}

int 
NscTcpSocketImpl::ShutdownSend (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_shutdownSend = true;
  return 0;
}
int 
NscTcpSocketImpl::ShutdownRecv (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_shutdownRecv = false;
  return 0;
}

int
NscTcpSocketImpl::Close (void)
{
  NS_LOG_FUNCTION (this << m_state);

  if (m_state == CLOSED)
    {
      return -1;
    }
  if (!m_txBuffer.empty ())
    { // App close with pending data must wait until all data transmitted
      m_closeOnEmpty = true;
      NS_LOG_LOGIC("Socket " << this << 
                   " deferring close, state " << m_state);
      return 0;
    }

  m_nscTcpSocket->disconnect();
  m_state = CLOSED;
  ShutdownSend ();
  return 0;
}

int
NscTcpSocketImpl::Connect (const Address & address)
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

  struct in_addr remoteAddr;
  uint32_t addr32;

  m_remoteAddress.Serialize((uint8_t*)&addr32);
  remoteAddr.s_addr = addr32;

  m_nscTcpSocket->connect(inet_ntoa(remoteAddr), m_remotePort);
  m_state = SYN_SENT;
  return 0;
}

int
NscTcpSocketImpl::Send (const Ptr<Packet> p, uint32_t flags)
{
  NS_LOG_FUNCTION (this << p);

  NS_ASSERT (p->GetSize () > 0);
  if (m_state == ESTABLISHED || m_state == SYN_SENT || m_state == CLOSE_WAIT)
  {
    if (p->GetSize () > GetTxAvailable ())
    {
      m_errno = ERROR_MSGSIZE;
      return -1;
    }

    bool txEmpty = m_txBuffer.empty();
    if (m_state == ESTABLISHED)
      {
        if (txEmpty)
          {
            m_txBuffer.push(p);
            m_txBufferSize += p->GetSize ();
          }
        if (!SendPendingData())
          {
             if (m_errno == ERROR_AGAIN)
               {
                 return txEmpty ? p->GetSize () : -1;
               }
             if (txEmpty)
               {
                  m_txBuffer.pop ();
                  m_txBufferSize = 0;
               }
             return -1;
          }
      }
      else
      {  // SYN_SET -- Queue Data
         m_txBuffer.push(p);
         m_txBufferSize += p->GetSize ();
      }
    return p->GetSize ();
  }
  else
  {
    m_errno = ERROR_NOTCONN;
    return -1;
  }
}

int
NscTcpSocketImpl::SendTo (Ptr<Packet> p, uint32_t flags, const Address &address)
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
NscTcpSocketImpl::GetTxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_txBufferSize != 0)
    {
      NS_ASSERT (m_txBufferSize <= m_sndBufSize);
      return m_sndBufSize - m_txBufferSize;
    }
  else
    {
      return m_sndBufSize;
    }
}

int
NscTcpSocketImpl::Listen (uint32_t q)
{
  NS_LOG_FUNCTION (this << q);
  m_nscTcpSocket->listen(m_localPort);
  m_state = LISTEN;
  return 0;
}


void
NscTcpSocketImpl::NSCWakeup ()
{
  switch (m_state) {
  case SYN_SENT:
      if (!m_nscTcpSocket->is_connected())
          break;
      m_state = ESTABLISHED;
      Simulator::ScheduleNow(&NscTcpSocketImpl::ConnectionSucceeded, this);
      // fall through to schedule read/write events
  case ESTABLISHED:
      if (!m_txBuffer.empty ())
          Simulator::ScheduleNow(&NscTcpSocketImpl::SendPendingData, this);
      Simulator::ScheduleNow(&NscTcpSocketImpl::ReadPendingData, this);
      break;
  case LISTEN:
      Simulator::ScheduleNow(&NscTcpSocketImpl::Accept, this);
      break;
  case CLOSED: break;
  default:
      NS_LOG_DEBUG (this << " invalid state: " << m_state);
  }
}

Ptr<Packet>
NscTcpSocketImpl::Recv (uint32_t maxSize, uint32_t flags)
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
NscTcpSocketImpl::RecvFrom (uint32_t maxSize, uint32_t flags,
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

uint32_t
NscTcpSocketImpl::GetRxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  // We separately maintain this state to avoid walking the queue 
  // every time this might be called
  return m_rxAvailable;
}

void
NscTcpSocketImpl::ForwardUp (Ptr<Packet> packet, Ipv4Address ipv4, uint16_t port)
{
  NSCWakeup();
}

void NscTcpSocketImpl::CompleteFork(void)
{
  // The address pairs (m_localAddress, m_localPort, m_remoteAddress, m_remotePort)
  // are bogus, but this isn't important at the moment, because
  // address <-> Socket handling is done by NSC internally.
  // We only need to add the new ns-3 socket to the list of sockets, so
  // we use plain Allocate() instead of Allocate(m_localAddress, ... )
  uint8_t buf[4];
  int port;
  size_t buflen = sizeof(buf);

  if (0 == m_nscTcpSocket->getpeername((void *) buf, &buflen, &port)) {
    m_remotePort = ntohs(port);
    m_remoteAddress = m_remoteAddress.Deserialize(buf);
    m_peerAddress = InetSocketAddress(m_remoteAddress, m_remotePort);
  }

  m_endPoint = m_tcp->Allocate ();

  //the cloned socket with be in listen state, so manually change state
  NS_ASSERT(m_state == LISTEN);
  m_state = ESTABLISHED;

  buflen = sizeof(buf);
  if (0 == m_nscTcpSocket->getsockname((void *) &buf, &buflen, &port))
    m_localAddress = m_localAddress.Deserialize(buf);

  NS_LOG_LOGIC ("NscTcpSocketImpl " << this << " accepted connection from " 
                 << m_remoteAddress << ":" << m_remotePort
                 << " to " << m_localAddress << ":" << m_localPort);
  //equivalent to FinishBind
  m_endPoint->SetRxCallback (MakeCallback (&NscTcpSocketImpl::ForwardUp, Ptr<NscTcpSocketImpl>(this)));
  m_endPoint->SetDestroyCallback (MakeCallback (&NscTcpSocketImpl::Destroy, Ptr<NscTcpSocketImpl>(this)));

  NotifyNewConnectionCreated (this, m_peerAddress);
}

void NscTcpSocketImpl::ConnectionSucceeded()
{ // We would preferred to have scheduled an event directly to
  // NotifyConnectionSucceeded, but (sigh) these are protected
  // and we can get the address of it :(

  uint8_t buf[4];
  int port;
  size_t buflen = sizeof(buf);
  if (0 == m_nscTcpSocket->getsockname((void *) &buf, &buflen, &port)) {
    m_localAddress = m_localAddress.Deserialize(buf);
    m_localPort = ntohs(port);
  }

  NS_LOG_LOGIC ("NscTcpSocketImpl " << this << " connected to "
                 << m_remoteAddress << ":" << m_remotePort
                 << " from " << m_localAddress << ":" << m_localPort);
  NotifyConnectionSucceeded();
}


bool NscTcpSocketImpl::Accept (void)
{
  if (m_state == CLOSED)
    {  // Happens if application closes listening socket after Accept() was scheduled.
      return false;
    }
  NS_ASSERT (m_state == LISTEN);

  if (!m_nscTcpSocket->is_listening())
    {
      return false;
    }
  INetStreamSocket *newsock;
  int res = m_nscTcpSocket->accept(&newsock);
  if (res != 0)
    {
      return false;
    }
// We could obtain a fromAddress using getpeername, but we've already
// finished the tcp handshake here, i.e. this is a new connection
// and not a connection request.
// if (!NotifyConnectionRequest(fromAddress))
//   return true;

  // Clone the socket
  Ptr<NscTcpSocketImpl> newSock = Copy ();
  newSock->m_nscTcpSocket = newsock;
  NS_LOG_LOGIC ("Cloned a NscTcpSocketImpl " << newSock);

  Simulator::ScheduleNow (&NscTcpSocketImpl::CompleteFork, newSock);
  return true;
}

bool NscTcpSocketImpl::ReadPendingData (void)
{
  if (m_state != ESTABLISHED)
    {
      return false;
    }
  int len, err;
  uint8_t buffer[8192];
  len = sizeof(buffer);
  m_errno = ERROR_NOTERROR;
  err = m_nscTcpSocket->read_data(buffer, &len);
  if (err == 0 && len == 0)
    {
      NS_LOG_LOGIC ("ReadPendingData got EOF from socket");
      m_state = CLOSED;
      return false;
    }
  m_errno = GetNativeNs3Errno(err);
  switch (m_errno)
    {
      case ERROR_NOTERROR: break; // some data was sent
      case ERROR_AGAIN: return false;
      default:
        NS_LOG_WARN ("Error (" << err << ") " <<
                     "during read_data, ns-3 errno set to" << m_errno);
        m_state = CLOSED;
        return false;
    }

  Ptr<Packet> p =  Create<Packet> (buffer, len);

  SocketAddressTag tag;

  tag.SetAddress (m_peerAddress);
  p->AddTag (tag);
  m_deliveryQueue.push (p);
  m_rxAvailable += p->GetSize ();

  NotifyDataRecv ();
  return true;
}

bool NscTcpSocketImpl::SendPendingData (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("ENTERING SendPendingData");

  if (m_txBuffer.empty ())
    {
      return false;
    }

  int ret;
  size_t size, written = 0;

  do {
    Ptr<Packet> &p = m_txBuffer.front ();
    size = p->GetSize ();
    NS_ASSERT (size > 0);

    m_errno = ERROR_NOTERROR;
    ret = m_nscTcpSocket->send_data((const char *)p->PeekData (), size);
    if (ret <= 0)
      {
        m_errno = GetNativeNs3Errno(ret);
        if (m_errno != ERROR_AGAIN)
          {
            NS_LOG_WARN ("Error (" << ret << ") " <<
                         "during send_data, ns-3 errno set to" << m_errno);
          }
        break;
      }
    written += ret;

    NS_ASSERT (m_txBufferSize >= (size_t)ret);
    m_txBufferSize -= ret;

    if ((size_t)ret < size)
      {
        p->RemoveAtStart(ret);
        break;
      }

    m_txBuffer.pop ();

    if (m_txBuffer.empty ())
      {
        if (m_closeOnEmpty)
          {
            m_nscTcpSocket->disconnect();
            m_state = CLOSED;
          }
        break;
      }
  } while ((size_t) ret == size);

  if (written > 0)
    {
      Simulator::ScheduleNow(&NscTcpSocketImpl::NotifyDataSent, this, ret);
      return true;
    }
  return false;
}

Ptr<NscTcpSocketImpl> NscTcpSocketImpl::Copy ()
{
  return CopyObject<NscTcpSocketImpl> (this);
}

void
NscTcpSocketImpl::SetSndBufSize (uint32_t size)
{
  m_sndBufSize = size;
}

uint32_t
NscTcpSocketImpl::GetSndBufSize (void) const
{
  return m_sndBufSize;
}

void
NscTcpSocketImpl::SetRcvBufSize (uint32_t size)
{
  m_rcvBufSize = size;
}

uint32_t
NscTcpSocketImpl::GetRcvBufSize (void) const
{
  return m_rcvBufSize;
}

void
NscTcpSocketImpl::SetSegSize (uint32_t size)
{
  m_segmentSize = size;
}

uint32_t
NscTcpSocketImpl::GetSegSize (void) const
{
  return m_segmentSize;
}

void
NscTcpSocketImpl::SetAdvWin (uint32_t window)
{
  m_advertisedWindowSize = window;
}

uint32_t
NscTcpSocketImpl::GetAdvWin (void) const
{
  return m_advertisedWindowSize;
}

void
NscTcpSocketImpl::SetSSThresh (uint32_t threshold)
{
  m_ssThresh = threshold;
}

uint32_t
NscTcpSocketImpl::GetSSThresh (void) const
{
  return m_ssThresh;
}

void
NscTcpSocketImpl::SetInitialCwnd (uint32_t cwnd)
{
  m_initialCWnd = cwnd;
}

uint32_t
NscTcpSocketImpl::GetInitialCwnd (void) const
{
  return m_initialCWnd;
}

void 
NscTcpSocketImpl::SetConnTimeout (Time timeout)
{
  m_cnTimeout = timeout;
}

Time
NscTcpSocketImpl::GetConnTimeout (void) const
{
  return m_cnTimeout;
}

void 
NscTcpSocketImpl::SetConnCount (uint32_t count)
{
  m_cnCount = count;
}

uint32_t 
NscTcpSocketImpl::GetConnCount (void) const
{
  return m_cnCount;
}

void 
NscTcpSocketImpl::SetDelAckTimeout (Time timeout)
{
  m_delAckTimeout = timeout;
}

Time
NscTcpSocketImpl::GetDelAckTimeout (void) const
{
  return m_delAckTimeout;
}

void
NscTcpSocketImpl::SetDelAckMaxCount (uint32_t count)
{
  m_delAckMaxCount = count;
}

uint32_t
NscTcpSocketImpl::GetDelAckMaxCount (void) const
{
  return m_delAckMaxCount;
}

enum Socket::SocketErrno
NscTcpSocketImpl::GetNativeNs3Errno(int error) const
{
  enum nsc_errno err;

  if (error >= 0)
    {
       return ERROR_NOTERROR;
    }
  err = (enum nsc_errno) error;
  switch (err)
    {
      case NSC_EADDRINUSE: // fallthrough
      case NSC_EADDRNOTAVAIL: return ERROR_AFNOSUPPORT;
      case NSC_EINPROGRESS: // Altough nsc sockets are nonblocking, we pretend they're not.
      case NSC_EAGAIN: return ERROR_AGAIN;
      case NSC_EISCONN: // fallthrough
      case NSC_EALREADY: return ERROR_ISCONN;
      case NSC_ECONNREFUSED: return ERROR_NOROUTETOHOST; // XXX, better mapping?
      case NSC_ECONNRESET: // for no, all of these fall through
      case NSC_EHOSTDOWN:
      case NSC_ENETUNREACH:
      case NSC_EHOSTUNREACH: return ERROR_NOROUTETOHOST;
      case NSC_EMSGSIZE: return ERROR_MSGSIZE;
      case NSC_ENOTCONN: return ERROR_NOTCONN;
      case NSC_ESHUTDOWN: return ERROR_SHUTDOWN;
      case NSC_ETIMEDOUT: return ERROR_NOTCONN; // XXX - this mapping isn't correct
      case NSC_ENOTDIR: // used by eg. sysctl(2). Shouldn't happen normally,
                        // but is triggered by e.g. show_config().
      case NSC_EUNKNOWN: return ERROR_INVAL; // Catches stacks that 'return -1' without real mapping
    }
  NS_ASSERT_MSG(0, "Unknown NSC error");
  return ERROR_INVAL;
}

}//namespace ns3
