/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 */
#ifndef NSC_TCP_SOCKET_IMPL_H
#define NSC_TCP_SOCKET_IMPL_H

#include <stdint.h>
#include <queue>
#include <vector>

#include "ns3/callback.h"
#include "ns3/traced-value.h"
#include "ns3/tcp-socket.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/event-id.h"
#include "pending-data.h"
#include "ns3/sequence-number.h"

struct INetStreamSocket;

namespace ns3 {

class Ipv4EndPoint;
class Node;
class Packet;
class NscTcpL4Protocol;
class TcpHeader;

/**
 * \ingroup socket
 * \ingroup nsctcp
 *
 * \brief Socket logic for the NSC TCP sockets.
 * 
 * Most of the TCP internal
 * logic is handled by the NSC tcp library itself; this class maps ns3::Socket
 * calls to the NSC TCP library.
 */
class NscTcpSocketImpl : public TcpSocket
{
public:
  static TypeId GetTypeId (void);
  /**
   * Create an unbound tcp socket.
   */
  NscTcpSocketImpl ();
  NscTcpSocketImpl (const NscTcpSocketImpl& sock);
  virtual ~NscTcpSocketImpl ();

  void SetNode (Ptr<Node> node);
  void SetTcp (Ptr<NscTcpL4Protocol> tcp);

  virtual enum SocketErrno GetErrno (void) const;
  virtual enum SocketType GetSocketType (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (void);
  virtual int Bind (const Address &address);
  virtual int Close (void);
  virtual int ShutdownSend (void);
  virtual int ShutdownRecv (void);
  virtual int Connect (const Address &address);
  virtual int Listen (void);
  virtual uint32_t GetTxAvailable (void) const;
  virtual int Send (Ptr<Packet> p, uint32_t flags);
  virtual int SendTo (Ptr<Packet> p, uint32_t flags, const Address &toAddress);
  virtual uint32_t GetRxAvailable (void) const;
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags);
  virtual Ptr<Packet> RecvFrom (uint32_t maxSize, uint32_t flags,
                                Address &fromAddress);
  virtual int GetSockName (Address &address) const; 
  virtual bool SetAllowBroadcast (bool allowBroadcast);
  virtual bool GetAllowBroadcast () const;

private:
  void NSCWakeup (void);
  friend class Tcp;
  // invoked by Tcp class
  int FinishBind (void);
  void ForwardUp (Ptr<Packet> p, Ipv4Header header, uint16_t port, 
                  Ptr<Ipv4Interface> incomingInterface);
  void Destroy (void);
  //methods for state
  bool SendPendingData (void);
  bool ReadPendingData (void);
  bool Accept (void);
  void CompleteFork (void);
  void ConnectionSucceeded ();

  // Manage data tx/rx
  // XXX This should be virtual and overridden
  Ptr<NscTcpSocketImpl> Copy ();

  // attribute related
  virtual void SetSndBufSize (uint32_t size);
  virtual uint32_t GetSndBufSize (void) const;
  virtual void SetRcvBufSize (uint32_t size);
  virtual uint32_t GetRcvBufSize (void) const;
  virtual void SetSegSize (uint32_t size);
  virtual uint32_t GetSegSize (void) const;
  virtual void SetAdvWin (uint32_t window);
  virtual uint32_t GetAdvWin (void) const;
  virtual void SetSSThresh (uint32_t threshold);
  virtual uint32_t GetSSThresh (void) const;
  virtual void SetInitialCwnd (uint32_t cwnd);
  virtual uint32_t GetInitialCwnd (void) const;
  virtual void SetConnTimeout (Time timeout);
  virtual Time GetConnTimeout (void) const;
  virtual void SetConnCount (uint32_t count);
  virtual uint32_t GetConnCount (void) const;
  virtual void SetDelAckTimeout (Time timeout);
  virtual Time GetDelAckTimeout (void) const;
  virtual void SetDelAckMaxCount (uint32_t count);
  virtual uint32_t GetDelAckMaxCount (void) const;
  virtual void SetTcpNoDelay (bool noDelay);
  virtual bool GetTcpNoDelay (void) const;
  virtual void SetPersistTimeout (Time timeout);
  virtual Time GetPersistTimeout (void) const;

  enum Socket::SocketErrno GetNativeNs3Errno (int err) const;
  uint32_t m_delAckMaxCount;
  Time m_delAckTimeout;
  bool m_noDelay;

  Ipv4EndPoint *m_endPoint;
  Ptr<Node> m_node;
  Ptr<NscTcpL4Protocol> m_tcp;
  Ipv4Address m_remoteAddress;
  uint16_t m_remotePort;
  //these two are so that the socket/endpoint cloning works
  Ipv4Address m_localAddress;
  uint16_t m_localPort;
  InetSocketAddress m_peerAddress;
  enum SocketErrno m_errno;
  bool m_shutdownSend;
  bool m_shutdownRecv;
  bool m_connected;

  //manage the state information
  TracedValue<TcpStates_t> m_state;
  bool m_closeOnEmpty;

  //needed to queue data when in SYN_SENT state
  std::queue<Ptr<Packet> > m_txBuffer;
  uint32_t m_txBufferSize;

  // Window management
  uint32_t                       m_segmentSize;          //SegmentSize
  uint32_t                       m_rxWindowSize;
  uint32_t                       m_advertisedWindowSize; //Window to advertise
  TracedValue<uint32_t>          m_cWnd;                 //Congestion window
  uint32_t                       m_ssThresh;             //Slow Start Threshold
  uint32_t                       m_initialCWnd;          //Initial cWnd value

  // Round trip time estimation
  Time m_lastMeasuredRtt;

  // Timer-related members
  Time              m_cnTimeout; 
  uint32_t          m_cnCount;
  Time              m_persistTimeout; 

  // Temporary queue for delivering data to application
  std::queue<Ptr<Packet> > m_deliveryQueue;
  uint32_t m_rxAvailable;
  INetStreamSocket* m_nscTcpSocket;

  // Attributes
  uint32_t m_sndBufSize;   // buffer limit for the outgoing queue
  uint32_t m_rcvBufSize;   // maximum receive socket buffer size
};

} // namespace ns3

#endif /* NSC_TCP_SOCKET_IMPL_H */
