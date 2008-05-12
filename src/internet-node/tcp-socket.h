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
#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <stdint.h>
#include <queue>
#include "ns3/callback.h"
#include "ns3/traced-value.h"
#include "ns3/socket.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/event-id.h"
#include "tcp-typedefs.h"
#include "pending-data.h"
#include "sequence-number.h"
#include "rtt-estimator.h"


namespace ns3 {

class Ipv4EndPoint;
class Node;
class Packet;
class TcpL4Protocol;
class TcpHeader;

class TcpSocket : public Socket
{
public:
  static TypeId GetTypeId (void);
  /**
   * Create an unbound tcp socket.
   */
  TcpSocket ();
  TcpSocket (const TcpSocket& sock);
  virtual ~TcpSocket ();

  void SetNode (Ptr<Node> node);
  void SetTcp (Ptr<TcpL4Protocol> tcp);
  void SetRtt (Ptr<RttEstimator> rtt);

  virtual enum SocketErrno GetErrno (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (void);
  virtual int Bind (const Address &address);
  virtual int Close (void);
  virtual int ShutdownSend (void);
  virtual int ShutdownRecv (void);
  virtual int Connect(const Address &address);
  virtual int Send (Ptr<Packet> p);
  virtual int Send (const uint8_t* buf, uint32_t size);
  virtual int SendTo(const Address &address, Ptr<Packet> p);
  virtual uint32_t GetTxAvailable (void) const;
  virtual int Listen(uint32_t queueLimit);

  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags);
  virtual uint32_t GetRxAvailable (void) const;

protected:
  virtual void SetSndBuf (uint32_t size);
  virtual uint32_t GetSndBuf (void) const;
  virtual void SetRcvBuf (uint32_t size);
  virtual uint32_t GetRcvBuf (void) const;

private:
  friend class Tcp;
  // invoked by Tcp class
  int FinishBind (void);
  void ForwardUp (Ptr<Packet> p, Ipv4Address ipv4, uint16_t port);
  void Destroy (void);
  int DoSendTo (Ptr<Packet> p, const Address &daddr);
  int DoSendTo (Ptr<Packet> p, Ipv4Address daddr, uint16_t dport);
  void SendEmptyPacket(uint8_t flags);
  //methods for state
  bool ProcessAction (Actions_t a);
  bool ProcessAction (Actions_t a, const TcpHeader& tcpHeader,
                      Ipv4Address saddr, Ipv4Address daddr);
  bool ProcessPacketAction (Actions_t a, Ptr<Packet> p,
                                       const TcpHeader& tcpHeader,
                                       const Address& fromAddress);
  Actions_t ProcessEvent (Events_t e);
  bool SendPendingData(bool withAck = false);
  void CompleteFork(Ptr<Packet>, const TcpHeader&, const Address& fromAddress);
  void ConnectionSucceeded();
  
  //methods for window management
  virtual uint32_t  UnAckDataCount(); // Return count of number of unacked bytes
  virtual uint32_t  BytesInFlight();  // Return total bytes in flight
  virtual uint32_t  Window();         // Return window size (integer)
  virtual uint32_t  AvailableWindow();// Return unfilled portion of window

  // Manage data tx/rx
  void NewRx (Ptr<Packet>, const TcpHeader&, const Address&);
  // XXX This should be virtual and overridden
  Ptr<TcpSocket> Copy ();
  void NewAck (SequenceNumber seq); 
  // XXX This should be virtual and overridden
  void DupAck (const TcpHeader& t, uint32_t count); 
  void ReTxTimeout ();
  void DelAckTimeout ();
  void LastAckTimeout ();
  void Retransmit ();
  void CommonNewAck (SequenceNumber seq, bool skipTimer = false);

  bool m_skipRetxResched;
  uint32_t m_dupAckCount;
  EventId m_retxEvent;
  EventId m_lastAckEvent;

  EventId m_delAckEvent;
  uint32_t m_delAckCount;
  uint32_t m_delAckMaxCount;
  Time m_delAckTimout;

  Ipv4EndPoint *m_endPoint;
  Ptr<Node> m_node;
  Ptr<TcpL4Protocol> m_tcp;
  Ipv4Address m_remoteAddress;
  uint16_t m_remotePort;
  //these two are so that the socket/endpoint cloning works
  Ipv4Address m_localAddress;
  uint16_t m_localPort;
  enum SocketErrno m_errno;
  bool m_shutdownSend;
  bool m_shutdownRecv;
  bool m_connected;
  
  //manage the state infomation
  States_t m_state;
  bool m_closeNotified;
  bool m_closeRequestNotified;
  bool m_closeOnEmpty;
  bool m_pendingClose;

  
  //sequence info, sender side
  SequenceNumber m_nextTxSequence;
  SequenceNumber m_highTxMark;
  SequenceNumber m_highestRxAck;
  SequenceNumber m_lastRxAck;
  
  //sequence info, reciever side
  SequenceNumber m_nextRxSequence;

  //history data
  //this is the incoming data buffer which sorts out of sequence data
  UnAckData_t m_bufferedData;
  //this is kind of the tx buffer
  PendingData* m_pendingData;
  SequenceNumber m_firstPendingSequence;

  // Window management
  uint32_t                       m_segmentSize;          //SegmentSize
  uint32_t                       m_rxWindowSize;
  uint32_t                       m_advertisedWindowSize; //Window to advertise
  TracedValue<uint32_t>          m_cWnd;                 //Congestion window
  uint32_t                       m_ssThresh;             //Slow Start Threshold
  uint32_t                       m_initialCWnd;          //Initial cWnd value

  // Round trip time estimation
  Ptr<RttEstimator> m_rtt;
  Time m_lastMeasuredRtt;

  // Timer-related members
  Time              m_cnTimeout; 
  uint32_t          m_cnCount;
  
  // Temporary queue for delivering data to application
  std::queue<Ptr<Packet> > m_deliveryQueue;
  uint32_t m_rxAvailable;
  
  uint32_t m_sndBufLimit;   // buffer limit for the outgoing queue
  uint32_t m_rcvBufLimit;   // maximum receive socket buffer size
};

}//namespace ns3

#endif /* TCP_SOCKET_H */
