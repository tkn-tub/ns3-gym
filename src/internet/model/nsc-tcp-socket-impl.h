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
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * Create an unbound tcp socket.
   */
  NscTcpSocketImpl ();

  /**
   * Clone a TCP socket, for use upon receiving a connection request in LISTEN state
   *
   * \param sock the original Tcp Socket
   */
  NscTcpSocketImpl (const NscTcpSocketImpl& sock);
  virtual ~NscTcpSocketImpl ();

  /**
   * \brief Set the associated node.
   * \param node the node
   */
  void SetNode (Ptr<Node> node);

  /**
   * \brief Set the associated TCP L4 protocol.
   * \param tcp the TCP L4 protocol
   */
  void SetTcp (Ptr<NscTcpL4Protocol> tcp);

  virtual enum SocketErrno GetErrno (void) const;
  virtual enum SocketType GetSocketType (void) const;
  virtual int GetPeerName (Address &address) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (void);
  virtual int Bind6 (void);
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
  virtual void BindToNetDevice (Ptr<NetDevice> netdevice); // NetDevice with my m_endPoint

private:
  /**
   * \brief Called by NscTcpSocketImpl::ForwardUp()
   *
   * Actually performs the ForwardUp operations
   */
  void NSCWakeup (void);
  /**
   * \brief Tcp friend class.
   * \relates Tcp
   */
  friend class Tcp;
  // invoked by Tcp class
  /**
   * Finish the binding process
   * \returns 0 on success, -1 on failure
   */
  int FinishBind (void);
  /**
   * \brief Called by the L3 protocol when it received a packet to pass on to TCP.
   *
   * \param p the incoming packet
   * \param header the packet's IPv4 header
   * \param port the incoming port
   * \param incomingInterface the incoming interface
   */
  void ForwardUp (Ptr<Packet> p, Ipv4Header header, uint16_t port, 
                  Ptr<Ipv4Interface> incomingInterface);
  /**
   * \brief Kill this socket by zeroing its attributes (IPv4)
   *
   * This is a callback function configured to m_endpoint in
   * SetupCallback(), invoked when the endpoint is destroyed.
   */
  void Destroy (void);
  //methods for state
  /**
   * \brief Send all the pending data
   * \returns true on success
   */
  bool SendPendingData (void);
  /**
   * \brief Read all the pending data
   * \returns true on success
   */
  bool ReadPendingData (void);
  /**
   * \brief Accept an incoming connection
   * \returns true on success
   */
  bool Accept (void);
  /**
   * \brief Complete the Fork operations (after a connection has been accepted)
   */
  void CompleteFork (void);

  /**
   * \brief Called when a connection is in Established state
   */
  void ConnectionSucceeded ();

  // Manage data tx/rx
  // \todo This should be virtual and overridden
  /**
   * \brief Copy self
   * \returns a copy of self
   */
  Ptr<NscTcpSocketImpl> Copy ();

  // attribute related
  virtual void SetSndBufSize (uint32_t size);
  virtual uint32_t GetSndBufSize (void) const;
  virtual void SetRcvBufSize (uint32_t size);
  virtual uint32_t GetRcvBufSize (void) const;
  virtual void SetSegSize (uint32_t size);
  virtual uint32_t GetSegSize (void) const;
  /**
   * \brief Set the Advertised Window size
   * \param window the window size
   */
  virtual void SetAdvWin (uint32_t window);
  /**
   * \brief Get the Advertised Window size
   * \returns the window size
   */
  virtual uint32_t GetAdvWin (void) const;
  virtual void SetInitialSSThresh (uint32_t threshold);
  virtual uint32_t GetInitialSSThresh (void) const;
  virtual void SetInitialCwnd (uint32_t cwnd);
  virtual uint32_t GetInitialCwnd (void) const;
  virtual void SetConnTimeout (Time timeout);
  virtual Time GetConnTimeout (void) const;
  virtual uint32_t GetSynRetries (void) const;
  virtual void SetSynRetries (uint32_t count);
  virtual void SetDataRetries (uint32_t retries);
  virtual uint32_t GetDataRetries (void) const;
  virtual void SetDelAckTimeout (Time timeout);
  virtual Time GetDelAckTimeout (void) const;
  virtual void SetDelAckMaxCount (uint32_t count);
  virtual uint32_t GetDelAckMaxCount (void) const;
  virtual void SetTcpNoDelay (bool noDelay);
  virtual bool GetTcpNoDelay (void) const;
  virtual void SetPersistTimeout (Time timeout);
  virtual Time GetPersistTimeout (void) const;

  /**
   * \brief Translate between a NSC error and a ns-3 error code
   * \param err NSC error
   * \returns ns-3 error code
   */
  enum Socket::SocketErrno GetNativeNs3Errno (int err) const;
  uint32_t m_delAckMaxCount;  //!< Number of packet to fire an ACK before delay timeout
  Time m_delAckTimeout;       //!< Time to delay an ACK
  bool m_noDelay;             //!< Disable ACk delay

  Ipv4EndPoint *m_endPoint;     //!< the IPv4 endpoint
  Ptr<Node> m_node;             //!< the associated node
  Ptr<NscTcpL4Protocol> m_tcp;  //!< the associated TCP L4 protocol
  Ipv4Address m_remoteAddress;  //!< peer IP address
  uint16_t m_remotePort;        //!< peer port
  //these two are so that the socket/endpoint cloning works
  Ipv4Address m_localAddress;   //!< local address
  uint16_t m_localPort;         //!< local port
  InetSocketAddress m_peerAddress; //!< peer IP and port
  mutable enum SocketErrno m_errno; //!< last error number
  bool m_shutdownSend;          //!< Send no longer allowed
  bool m_shutdownRecv;          //!< Receive no longer allowed
  bool m_connected;             //!< Connection established

  //manage the state information
  TracedValue<TcpStates_t> m_state; //!< state information
  bool m_closeOnEmpty;              //!< true if socket will close when buffer is empty

  //needed to queue data when in SYN_SENT state
  std::queue<Ptr<Packet> > m_txBuffer; //!< transmission buffer
  uint32_t m_txBufferSize;             //!< transmission buffer size

  // Window management
  uint32_t                       m_segmentSize;          //!< SegmentSize
  uint32_t                       m_rxWindowSize;         //!< Receive window size
  uint32_t                       m_advertisedWindowSize; //!< Window to advertise
  TracedValue<uint32_t>          m_cWnd;                 //!< Congestion window
  TracedValue<uint32_t>          m_ssThresh;             //!< Slow Start Threshold
  uint32_t                       m_initialCWnd;          //!< Initial cWnd value
  uint32_t                       m_initialSsThresh;      //!< Initial Slow Start Threshold

  // Round trip time estimation
  Time m_lastMeasuredRtt; //!< Last measured RTT

  // Timer-related members
  Time              m_cnTimeout;       //!< Timeout for connection retry
  uint32_t          m_synRetries;      //!< Count of remaining connection retries
  uint32_t          m_dataRetries;     //!< Count of remaining data retransmission attempts
  Time              m_persistTimeout;  //!< Time between sending 1-byte probes

  // Temporary queue for delivering data to application
  std::queue<Ptr<Packet> > m_deliveryQueue; //!< receive buffer
  uint32_t m_rxAvailable;                   //!< receive buffer available size
  INetStreamSocket* m_nscTcpSocket;         //!< the real NSC TCP socket

  // Attributes
  uint32_t m_sndBufSize;   //!< buffer limit for the outgoing queue
  uint32_t m_rcvBufSize;   //!< maximum receive socket buffer size
};

} // namespace ns3

#endif /* NSC_TCP_SOCKET_IMPL_H */
