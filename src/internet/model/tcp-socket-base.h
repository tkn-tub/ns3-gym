/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
 * Copyright (c) 2010 Adrian Sai-wah Tam
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
 * Author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */
#ifndef TCP_SOCKET_BASE_H
#define TCP_SOCKET_BASE_H

#include <stdint.h>
#include <queue>
#include "ns3/callback.h"
#include "ns3/traced-value.h"
#include "ns3/tcp-socket.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv6-header.h"
#include "ns3/ipv6-interface.h"
#include "ns3/event-id.h"
#include "tcp-tx-buffer.h"
#include "tcp-rx-buffer.h"
#include "rtt-estimator.h"

namespace ns3 {

class Ipv4EndPoint;
class Ipv6EndPoint;
class Node;
class Packet;
class TcpL4Protocol;
class TcpHeader;

/**
 * \ingroup socket
 * \ingroup tcp
 *
 * \brief A base class for implementation of a stream socket using TCP.
 *
 * This class contains the essential components of TCP, as well as a sockets
 * interface for upper layers to call. This serves as a base for other TCP
 * functions where the sliding window mechanism is handled here. This class
 * provides connection orientation and sliding window flow control. Part of
 * this class is modified from the original NS-3 TCP socket implementation
 * (TcpSocketImpl) by Raj Bhattacharjea <raj.b@gatech.edu> of Georgia Tech.
 */
class TcpSocketBase : public TcpSocket
{
public:
  /**
   * Get the type ID.
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * Create an unbound TCP socket
   */
  TcpSocketBase (void);

  /**
   * Clone a TCP socket, for use upon receiving a connection request in LISTEN state
   *
   * \param sock the original Tcp Socket
   */
  TcpSocketBase (const TcpSocketBase& sock);
  virtual ~TcpSocketBase (void);

  // Set associated Node, TcpL4Protocol, RttEstimator to this socket

  /**
   * \brief Set the associated node.
   * \param node the node
   */
  virtual void SetNode (Ptr<Node> node);

  /**
   * \brief Set the associated TCP L4 protocol.
   * \param tcp the TCP L4 protocol
   */
  virtual void SetTcp (Ptr<TcpL4Protocol> tcp);

  /**
   * \brief Set the associated RTT estimator.
   * \param rtt the RTT estimator
   */
  virtual void SetRtt (Ptr<RttEstimator> rtt);

  // Necessary implementations of null functions from ns3::Socket
  virtual enum SocketErrno GetErrno (void) const;    // returns m_errno
  virtual enum SocketType GetSocketType (void) const; // returns socket type
  virtual Ptr<Node> GetNode (void) const;            // returns m_node
  virtual int Bind (void);    // Bind a socket by setting up endpoint in TcpL4Protocol
  virtual int Bind6 (void);    // Bind a socket by setting up endpoint in TcpL4Protocol
  virtual int Bind (const Address &address);         // ... endpoint of specific addr or port
  virtual int Connect (const Address &address);      // Setup endpoint and call ProcessAction() to connect
  virtual int Listen (void);  // Verify the socket is in a correct state and call ProcessAction() to listen
  virtual int Close (void);   // Close by app: Kill socket upon tx buffer emptied
  virtual int ShutdownSend (void);    // Assert the m_shutdownSend flag to prevent send to network
  virtual int ShutdownRecv (void);    // Assert the m_shutdownRecv flag to prevent forward to app
  virtual int Send (Ptr<Packet> p, uint32_t flags);  // Call by app to send data to network
  virtual int SendTo (Ptr<Packet> p, uint32_t flags, const Address &toAddress); // Same as Send(), toAddress is insignificant
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags); // Return a packet to be forwarded to app
  virtual Ptr<Packet> RecvFrom (uint32_t maxSize, uint32_t flags, Address &fromAddress); // ... and write the remote address at fromAddress
  virtual uint32_t GetTxAvailable (void) const; // Available Tx buffer size
  virtual uint32_t GetRxAvailable (void) const; // Available-to-read data size, i.e. value of m_rxAvailable
  virtual int GetSockName (Address &address) const; // Return local addr:port in address
  virtual void BindToNetDevice (Ptr<NetDevice> netdevice); // NetDevice with my m_endPoint

protected:
  // Implementing ns3::TcpSocket -- Attribute get/set
  // inherited, no need to doc

  virtual void     SetSndBufSize (uint32_t size);
  virtual uint32_t GetSndBufSize (void) const;
  virtual void     SetRcvBufSize (uint32_t size);
  virtual uint32_t GetRcvBufSize (void) const;
  virtual void     SetSegSize (uint32_t size);
  virtual uint32_t GetSegSize (void) const;
  virtual void     SetSSThresh (uint32_t threshold) = 0;
  virtual uint32_t GetSSThresh (void) const = 0;
  virtual void     SetInitialCwnd (uint32_t cwnd) = 0;
  virtual uint32_t GetInitialCwnd (void) const = 0;
  virtual void     SetConnTimeout (Time timeout);
  virtual Time     GetConnTimeout (void) const;
  virtual void     SetConnCount (uint32_t count);
  virtual uint32_t GetConnCount (void) const;
  virtual void     SetDelAckTimeout (Time timeout);
  virtual Time     GetDelAckTimeout (void) const;
  virtual void     SetDelAckMaxCount (uint32_t count);
  virtual uint32_t GetDelAckMaxCount (void) const;
  virtual void     SetTcpNoDelay (bool noDelay);
  virtual bool     GetTcpNoDelay (void) const;
  virtual void     SetPersistTimeout (Time timeout);
  virtual Time     GetPersistTimeout (void) const;
  virtual bool     SetAllowBroadcast (bool allowBroadcast);
  virtual bool     GetAllowBroadcast (void) const;



  // Helper functions: Connection set up

  /**
   * \brief Common part of the two Bind(), i.e. set callback and remembering local addr:port
   *
   * \returns 0 on success, -1 on failure
   */
  int SetupCallback (void);

  /**
   * \brief Perform the real connection tasks: Send SYN if allowed, RST if invalid
   *
   * \returns 0 on success
   */
  int DoConnect (void);

  /**
   * \brief Schedule-friendly wrapper for Socket::NotifyConnectionSucceeded()
   */
  void ConnectionSucceeded (void);

  /**
   * \brief Configure the endpoint to a local address. Called by Connect() if Bind() didn't specify one.
   *
   * \returns 0 on success
   */
  int SetupEndpoint (void);

  /**
   * \brief Configure the endpoint v6 to a local address. Called by Connect() if Bind() didn't specify one.
   *
   * \returns 0 on success
   */
  int SetupEndpoint6 (void);

  /**
   * \brief Complete a connection by forking the socket
   *
   * This function is called only if a SYN received in LISTEN state. After
   * TcpSocketBase cloned, allocate a new end point to handle the incoming
   * connection and send a SYN+ACK to complete the handshake.
   *
   * \param p the packet triggering the fork
   * \param tcpHeader the TCP header of the triggering packet
   * \param fromAddress the address of the remote host
   * \param toAddress the address the connection is directed to
   */
  void CompleteFork (Ptr<Packet> p, const TcpHeader& tcpHeader, const Address& fromAddress, const Address& toAddress);



  // Helper functions: Transfer operation

  /**
   * \brief Called by the L3 protocol when it received a packet to pass on to TCP.
   *
   * \param packet the incoming packet
   * \param header the packet's IPv4 header
   * \param port the incoming port
   * \param incomingInterface the incoming interface
   */
  void ForwardUp (Ptr<Packet> packet, Ipv4Header header, uint16_t port, Ptr<Ipv4Interface> incomingInterface);

  /**
   * \brief Called by the L3 protocol when it received a packet to pass on to TCP.
   *
   * \param packet the incoming packet
   * \param header the packet's IPv6 header
   * \param port the incoming port
   * \param incomingInterface the incoming interface
   */
  void ForwardUp6 (Ptr<Packet> packet, Ipv6Header header, uint16_t port, Ptr<Ipv6Interface> incomingInterface);

  /**
   * \brief Called by TcpSocketBase::ForwardUp().
   *
   * \param packet the incoming packet
   * \param header the packet's IPv4 header
   * \param port the incoming port
   * \param incomingInterface the incoming interface
   */
  virtual void DoForwardUp (Ptr<Packet> packet, Ipv4Header header, uint16_t port, Ptr<Ipv4Interface> incomingInterface); //Get a pkt from L3

  /**
   * \brief Called by TcpSocketBase::ForwardUp6().
   *
   * \param packet the incoming packet
   * \param header the packet's IPv6 header
   * \param port the incoming port
   */
  virtual void DoForwardUp (Ptr<Packet> packet, Ipv6Header header, uint16_t port);

  /**
   * \brief Called by the L3 protocol when it received an ICMP packet to pass on to TCP.
   *
   * \param icmpSource the ICMP source address
   * \param icmpTtl the ICMP Time to Live
   * \param icmpType the ICMP Type
   * \param icmpCode the ICMP Code
   * \param icmpInfo the ICMP Info
   */
  void ForwardIcmp (Ipv4Address icmpSource, uint8_t icmpTtl, uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo);

  /**
   * \brief Called by the L3 protocol when it received an ICMPv6 packet to pass on to TCP.
   *
   * \param icmpSource the ICMP source address
   * \param icmpTtl the ICMP Time to Live
   * \param icmpType the ICMP Type
   * \param icmpCode the ICMP Code
   * \param icmpInfo the ICMP Info
   */
  void ForwardIcmp6 (Ipv6Address icmpSource, uint8_t icmpTtl, uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo);

  /**
   * \brief Send as much pending data as possible according to the Tx window.
   *
   * Note that this function did not implement the PSH flag.
   *
   * \param withAck forces an ACK to be sent
   * \returns true if some data have been sent
   */
  bool SendPendingData (bool withAck = false);

  /**
   * \brief Extract at most maxSize bytes from the TxBuffer at sequence seq, add the
   *        TCP header, and send to TcpL4Protocol
   *
   * \param seq the sequence number
   * \param maxSize the maximum data block to be transmitted (in bytes)
   * \param withAck forces an ACK to be sent
   * \returns the number of bytes sent
   */
  uint32_t SendDataPacket (SequenceNumber32 seq, uint32_t maxSize, bool withAck);

  /**
   * \brief Send a empty packet that carries a flag, e.g. ACK
   *
   * \param flags the packet's flags
   */
  void SendEmptyPacket (uint8_t flags);

  /**
   * \brief Send reset and tear down this socket
   */
  void SendRST (void);

  /**
   * \brief Check if a sequence number range is within the rx window
   *
   * \param head start of the Sequence window
   * \param tail end of the Sequence window
   * \returns true if it is in range
   */
  bool OutOfRange (SequenceNumber32 head, SequenceNumber32 tail) const;


  // Helper functions: Connection close

  /**
   * \brief Close a socket by sending RST, FIN, or FIN+ACK, depend on the current state
   *
   * \returns 0 on success
   */
  int DoClose (void);

  /**
   * \brief Peacefully close the socket by notifying the upper layer and deallocate end point
   */
  void CloseAndNotify (void);

  /**
   * \brief Kill this socket by zeroing its attributes (IPv4)
   *
   * This is a callback function configured to m_endpoint in
   * SetupCallback(), invoked when the endpoint is destroyed.
   */
  void Destroy (void);

  /**
   * \brief Kill this socket by zeroing its attributes (IPv6)
   *
   * This is a callback function configured to m_endpoint in
   * SetupCallback(), invoked when the endpoint is destroyed.
   */
  void Destroy6 (void);

  /**
   * \brief Deallocate m_endPoint
   */
  void DeallocateEndPoint (void);

  /**
   * \brief Received a FIN from peer, notify rx buffer
   *
   * \param p the packet
   * \param tcpHeader the packet's TCP header
   */
  void PeerClose (Ptr<Packet> p, const TcpHeader& tcpHeader);

  /**
   * \brief FIN is in sequence, notify app and respond with a FIN
   */
  void DoPeerClose (void);

  /**
   * \brief Cancel all timer when endpoint is deleted
   */
  void CancelAllTimers (void);

  /**
   * \brief Move from CLOSING or FIN_WAIT_2 to TIME_WAIT state
   */
  void TimeWait (void);

  // State transition functions

  /**
   * \brief Received a packet upon ESTABLISHED state.
   *
   * This function is mimicking the role of tcp_rcv_established() in tcp_input.c in Linux kernel.
   *
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   */
  void ProcessEstablished (Ptr<Packet> packet, const TcpHeader& tcpHeader); // Received a packet upon ESTABLISHED state

  /**
   * \brief Received a packet upon LISTEN state.
   *
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   * \param fromAddress the source address
   * \param toAddress the destination address
   */
  void ProcessListen (Ptr<Packet> packet, const TcpHeader& tcpHeader,
                      const Address& fromAddress, const Address& toAddress);

  /**
   * \brief Received a packet upon SYN_SENT
   *
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   */
  void ProcessSynSent (Ptr<Packet> packet, const TcpHeader& tcpHeader);

  /**
   * \brief Received a packet upon SYN_RCVD.
   *
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   * \param fromAddress the source address
   * \param toAddress the destination address
   */
  void ProcessSynRcvd (Ptr<Packet> packet, const TcpHeader& tcpHeader,
                       const Address& fromAddress, const Address& toAddress);

  /**
   * \brief Received a packet upon CLOSE_WAIT, FIN_WAIT_1, FIN_WAIT_2
   *
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   */
  void ProcessWait (Ptr<Packet> packet, const TcpHeader& tcpHeader);

  /**
   * \brief Received a packet upon CLOSING
   *
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   */
  void ProcessClosing (Ptr<Packet> packet, const TcpHeader& tcpHeader);

  /**
   * \brief Received a packet upon LAST_ACK
   *
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   */
  void ProcessLastAck (Ptr<Packet> packet, const TcpHeader& tcpHeader);

  // Window management

  /**
   * \brief Return count of number of unacked bytes
   * \returns count of number of unacked bytes
   */
  virtual uint32_t UnAckDataCount (void);

  /**
   * \brief Return total bytes in flight
   * \returns total bytes in flight
   */
  virtual uint32_t BytesInFlight (void);

  /**
   * \brief Return the max possible number of unacked bytes
   * \returns the max possible number of unacked bytes
   */
  virtual uint32_t Window (void);

  /**
   * \brief Return unfilled portion of window
   * \return unfilled portion of window
   */
  virtual uint32_t AvailableWindow (void);

  /**
   * \brief The amount of Rx window announced to the peer
   * \returns size of Rx window announced to the peer
   */
  virtual uint16_t AdvertisedWindowSize (void);


  // Manage data tx/rx

  /**
   * \brief Call CopyObject<> to clone me
   * \returns a copy of the socket
   */
  virtual Ptr<TcpSocketBase> Fork (void) = 0;

  /**
   * \brief Received an ACK packet
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   */
  virtual void ReceivedAck (Ptr<Packet> packet, const TcpHeader& tcpHeader);

  /**
   * \brief Recv of a data, put into buffer, call L7 to get it if necessary
   * \param packet the packet
   * \param tcpHeader the packet's TCP header
   */
  virtual void ReceivedData (Ptr<Packet> packet, const TcpHeader& tcpHeader);

  /**
   * \brief Take into account the packet for RTT estimation
   * \param tcpHeader the packet's TCP header
   */
  virtual void EstimateRtt (const TcpHeader& tcpHeader);

  /**
   * \brief Update buffers w.r.t. ACK
   * \param seq the sequence number
   */
  virtual void NewAck (SequenceNumber32 const& seq);

  /**
   * \brief Received dupack (duplicate ACK)
   * \param tcpHeader the packet's TCP header
   * \param count counter of duplicate ACKs
   */
  virtual void DupAck (const TcpHeader& tcpHeader, uint32_t count) = 0;

  /**
   * \brief Call Retransmit() upon RTO event
   */
  virtual void ReTxTimeout (void);

  /**
   * \brief Halving cwnd and call DoRetransmit()
   */
  virtual void Retransmit (void);

  /**
   * \brief Action upon delay ACK timeout, i.e. send an ACK
   */
  virtual void DelAckTimeout (void);

  /**
   * \brief Timeout at LAST_ACK, close the connection
   */
  virtual void LastAckTimeout (void);

  /**
   * \brief Send 1 byte probe to get an updated window size
   */
  virtual void PersistTimeout (void);

  /**
   * \brief Retransmit the oldest packet
   */
  virtual void DoRetransmit (void);

  /**
   * \brief Read option from incoming packets
   * \param tcpHeader the packet's TCP header
   */
  virtual void ReadOptions (const TcpHeader& tcpHeader);

  /**
   * \brief Add option to outgoing packets
   * \param tcpHeader the packet's TCP header
   */
  virtual void AddOptions (TcpHeader& tcpHeader);


protected:
  // Counters and events
  EventId           m_retxEvent;       //!< Retransmission event
  EventId           m_lastAckEvent;    //!< Last ACK timeout event
  EventId           m_delAckEvent;     //!< Delayed ACK timeout event
  EventId           m_persistEvent;    //!< Persist event: Send 1 byte to probe for a non-zero Rx window
  EventId           m_timewaitEvent;   //!< TIME_WAIT expiration event: Move this socket to CLOSED state
  uint32_t          m_dupAckCount;     //!< Dupack counter
  uint32_t          m_delAckCount;     //!< Delayed ACK counter
  uint32_t          m_delAckMaxCount;  //!< Number of packet to fire an ACK before delay timeout
  bool              m_noDelay;         //!< Set to true to disable Nagle's algorithm
  uint32_t          m_cnCount;         //!< Count of remaining connection retries
  uint32_t          m_cnRetries;       //!< Number of connection retries before giving up
  TracedValue<Time> m_rto;             //!< Retransmit timeout
  TracedValue<Time> m_lastRtt;         //!< Last RTT sample collected
  Time              m_delAckTimeout;   //!< Time to delay an ACK
  Time              m_persistTimeout;  //!< Time between sending 1-byte probes
  Time              m_cnTimeout;       //!< Timeout for connection retry

  // Connections to other layers of TCP/IP
  Ipv4EndPoint*       m_endPoint;   //!< the IPv4 endpoint
  Ipv6EndPoint*       m_endPoint6;  //!< the IPv6 endpoint
  Ptr<Node>           m_node;       //!< the associated node
  Ptr<TcpL4Protocol>  m_tcp;        //!< the associated TCP L4 protocol
  Callback<void, Ipv4Address,uint8_t,uint8_t,uint8_t,uint32_t> m_icmpCallback;  //!< ICMP callback
  Callback<void, Ipv6Address,uint8_t,uint8_t,uint8_t,uint32_t> m_icmpCallback6; //!< ICMPv6 callback

  Ptr<RttEstimator> m_rtt; //!< Round trip time estimator

  // Rx and Tx buffer management
  TracedValue<SequenceNumber32> m_nextTxSequence; //!< Next seqnum to be sent (SND.NXT), ReTx pushes it back
  TracedValue<SequenceNumber32> m_highTxMark;     //!< Highest seqno ever sent, regardless of ReTx
  TcpRxBuffer                   m_rxBuffer;       //!< Rx buffer (reordering buffer)
  TcpTxBuffer                   m_txBuffer;       //!< Tx buffer

  // State-related attributes
  TracedValue<TcpStates_t> m_state;         //!< TCP state
  enum SocketErrno         m_errno;         //!< Socket error code
  bool                     m_closeNotified; //!< Told app to close socket
  bool                     m_closeOnEmpty;  //!< Close socket upon tx buffer emptied
  bool                     m_shutdownSend;  //!< Send no longer allowed
  bool                     m_shutdownRecv;  //!< Receive no longer allowed
  bool                     m_connected;     //!< Connection established
  double                   m_msl;           //!< Max segment lifetime

  // Window management
  uint32_t              m_segmentSize; //!< Segment size
  uint16_t              m_maxWinSize;  //!< Maximum window size to advertise
  TracedValue<uint32_t> m_rWnd;        //!< Flow control window at remote side
};

} // namespace ns3

#endif /* TCP_SOCKET_BASE_H */
