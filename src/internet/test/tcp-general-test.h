/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Natale Patriciello <natale.patriciello@gmail.com>
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
 */
#ifndef TCPGENERALTEST_H
#define TCPGENERALTEST_H

#include "ns3/simple-net-device.h"
#include "ns3/error-model.h"
#include "ns3/tcp-socket-base.h"
#include "ns3/test.h"

namespace ns3 {

/**
 * \brief Class for inserting callbacks special points of the flow of TCP sockets
 *
 * This subclass is born to extend TcpSocketBase, inserting callbacks in certain
 * points of the flow, to be used in testing to check certain values or flow
 * directions.
 *
 * There isn't the necessity to fill TcpSocketBase of TracedCallbacks; the rationale
 * is to maintain the base class as clean as possible.
 *
 * To be fair with testing, this class should NOT modify the behavior of TcpSocketBase.
 *
 * \see SetRcvAckCb
 * \see SetProcessedAckCb
 * \see SetRetransmitCb
 */
class TcpSocketMsgBase : public TcpSocketBase
{
public:
  static TypeId GetTypeId (void);

  TcpSocketMsgBase () : TcpSocketBase ()
  {
  }

  TcpSocketMsgBase (const TcpSocketMsgBase &other) : TcpSocketBase (other)
  {
    m_rcvAckCb = other.m_rcvAckCb;
    m_processedAckCb = other.m_processedAckCb;
    m_retrCallback = other.m_retrCallback;
    m_forkCb = other.m_forkCb;
  }

  typedef Callback<void, Ptr<const Packet>, const TcpHeader&,
                   Ptr<const TcpSocketBase> > AckManagementCallback;
  typedef Callback<void, Ptr<const TcpSocketState>,
                   Ptr<const TcpSocketBase> > RetrCallback;

  /**
   * \brief Set the callback invoked when an ACK is received (at the beginning
   * of the processing)
   *
   * \param cb callback
   */
  void SetRcvAckCb (AckManagementCallback cb);

  /**
   * \brief Set the callback invoked when an ACK is received and processed
   * (at the end of the processing)
   *
   * \param cb callback
   */
  void SetProcessedAckCb (AckManagementCallback cb);

  /**
   * \brief Set the callback invoked after the processing of a retransmit timeout
   *
   * \param cb callback
   */
  void SetRetransmitCb (RetrCallback cb);

  /**
   * \brief Set the callback invoked after the forking
   * \param cb callback
   */
  void SetForkCb (Callback<void, Ptr<TcpSocketMsgBase> > cb);

protected:
  virtual void ReceivedAck (Ptr<Packet> packet, const TcpHeader& tcpHeader);
  virtual void Retransmit (void);
  virtual Ptr<TcpSocketBase> Fork (void);
  virtual void CompleteFork (Ptr<Packet> p, const TcpHeader& tcpHeader,
                             const Address& fromAddress, const Address& toAddress);

private:
  AckManagementCallback m_rcvAckCb;
  AckManagementCallback m_processedAckCb;
  RetrCallback m_retrCallback;
  Callback<void, Ptr<TcpSocketMsgBase> > m_forkCb;
};


/**
 * \brief A TCP socket which sends ACKs smaller than the segment received.
 *
 * Usually, a TCP socket which receives the sequence number "x" replies with
 * an ACK to "x+1". What happen if a malicious socket sends smaller ACKs
 * (e.g. two ACKs, one for "x/2", and the other for "x+1") ? A TCP implementation
 * should avoid to artificially increase the congestion window, thinking of
 * having ACKed 2 segments instead of 1.
 *
 * Set the number of bytes that should be acked in each ACK packet with
 * SetBytesToAck.
 *
 * \see TcpSlowStartAttackerTest
 */
class TcpSocketSmallAcks : public TcpSocketMsgBase
{
public:
  static TypeId GetTypeId (void);

  TcpSocketSmallAcks ()
    : TcpSocketMsgBase (),
    m_bytesToAck (125),
    m_bytesLeftToBeAcked (0),
    m_lastAckedSeq (1)
  {
  }

  TcpSocketSmallAcks (const TcpSocketSmallAcks &other) : TcpSocketMsgBase (other),
    m_bytesToAck (other.m_bytesToAck),
    m_bytesLeftToBeAcked (other.m_bytesLeftToBeAcked),
    m_lastAckedSeq (other.m_lastAckedSeq)
  {
  }

  void SetBytesToAck (uint32_t bytes)
  {
    m_bytesToAck = bytes;
  }

protected:
  virtual void SendEmptyPacket (uint8_t flags);
  Ptr<TcpSocketBase> Fork (void);

  uint32_t m_bytesToAck;
  uint32_t m_bytesLeftToBeAcked;
  SequenceNumber32 m_lastAckedSeq;
};

/**
* \brief General infrastructure for TCP testing
*
* The class provides a simple setup for a connection testing. Implement
* or modify the virtual methods in order to install a specified
* channel, a specified socket and a specified error model on this simulation.
* Default values are a null error model, and as a channel a SimpleChannel with
* the propagation delay set through the constructor.
*
* Check DoRun documentation for more information on the environment setup.
*
* Apart from setting up the environment for testing, subclassing permits also
* to track and check what is happening inside the two connected sockets. Thanks
* to TcpSocketMsgBase, there are many information provided to children:
*
* - Tracing of states inside the state machines (TCP and ACK ones, through
*   functions CongStateTrace and TcpStateTrace)
* - cWnd tracing (through CWndTrace)
* - Socket closing: error state, or normal state (NormalClose and ErrorClose)
* - Packet drop, inside queue or over the link (QueueDrop, PhyDrop)
* - Ack received (RcvAck) and Ack processed (ProcessedAck). The first is used to
*   signal that an ACK has been received; after the processing of it, the Second
*   is called
* - A packet is transmitted to IP layer or received from IP layer (Tx and Rx)
* - The RTO expires (RTOExpired)
*
* The default version of such methods is empty; implement their behavior differently,
* based on what you want to test. Default is empty to avoid the need to implement
* useless pure virtual function.
*
* If you need values from TcpSocketBase, thanks to the friend relationship between
* this class and TcpSocketBase itself you can get it. Remember that friendship
* isn't passed by inheritance, so the way to go is to write a Getters (like
* GetSegSize) and call it in the subclass.
*
* \see DoRun
* \see TcpSocketMsgBase
*/
class TcpGeneralTest : public TestCase
{
public:
  /**
   * \brief TcpGeneralTest constructor
   *
   * \param pktSize application packet size
   * \param pktCount count of application packet to generate
   * \param pktInterval (application) interval between each generated packet
   * \param propagationDelay propagation delay of the channel
   * \param startTime time of the first application-generated packet
   * \param mtu MTU of the environment
   * \param desc description of the test
   */
  TcpGeneralTest (const std::string &desc,
                  uint32_t pktSize = 500,
                  uint32_t pktCount = 10,
                  const Time& pktInterval = Seconds (0.01),
                  const Time& propagationDelay = Seconds (0.5),
                  const Time& startTime = Seconds (10),
                  uint32_t initialSlowStartThresh = 0xffff,
                  uint32_t initialCwnd = 1,
                  uint32_t segmentSize = 500,
                  TypeId   congestionControl = TcpNewReno::GetTypeId (),
                  uint32_t mtu = 1500);
  ~TcpGeneralTest ();

  /**
   * \brief Used as parameter of methods, specifies on what node
   * the caller is interested (e.g. GetSegSize).
   */
  enum SocketWho
  {
    SENDER,  //!< Sender node
    RECEIVER //!< Receiver node
  };

protected:
  /**
   * \brief Create and return the channel installed between the two socket
   *
   * \return A SimpleChannel subclass
   */
  virtual Ptr<SimpleChannel> CreateChannel ();

  /**
   * \brief Create and return the error model to install in the sender node
   *
   * \return sender error model
   */
  virtual Ptr<ErrorModel> CreateSenderErrorModel ();

  /**
   * \brief Create and return the error model to install in the receiver node
   *
   * \return receiver error model
   */
  virtual Ptr<ErrorModel> CreateReceiverErrorModel ();

  /**
   * \brief Create and install the socket to install on the receiver
   * \param node receiver node pointer
   * \return the socket to be installed in the receiver
   */
  virtual Ptr<TcpSocketMsgBase> CreateReceiverSocket (Ptr<Node> node);

  /**
   * \brief Create and install the socket to install on the sender
   * \param node sender node pointer
   * \return the socket to be installed in the sender
   */
  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);

  /**
   * \brief Create a socket
   *
   * \param node associated node
   * \param congControl congestion control
   * \return a pointer to the newer created socket
   */
  virtual Ptr<TcpSocketMsgBase> CreateSocket (Ptr<Node> node, TypeId socketType,
                                              TypeId congControl);

  /**
   * \brief Get the pointer to a previously created sender socket
   * \return ptr to sender socket or 0
   */
  Ptr<TcpSocketMsgBase> GetSenderSocket ()
  {
    return m_senderSocket;
  }

  /**
   * \brief Get the pointer to a previously created receiver socket
   * \return ptr to receiver socket or 0
   */
  Ptr<TcpSocketMsgBase> GetReceiverSocket ()
  {
    return m_receiverSocket;
  }

  /**
   * \brief Execute the tcp test
   *
   * As environment, two socket are connected through a SimpleChannel. Each device
   * has an MTU of 1500 bytes, and the application starts to send packet at
   * 10s of simulated time, through SendPacket.
   */
  virtual void DoRun (void);

  /**
   * \brief Teardown the TCP test
   */
  virtual void DoTeardown (void);

  /**
   * \brief Scheduled at 0.0, SENDER starts the connection to RECEIVER
   */
  void DoConnect ();

  /**
   * \brief Packet received
   *
   * The method processes the packet (application-layer)
   * \param socket socket which has received the packet
   */
  virtual void ReceivePacket (Ptr<Socket> socket);

  /**
   * \brief Send packets to other endpoint
   *
   * \param socket Socket
   * \param pktSize size of the packet
   * \param pktCount number of packets to send
   * \param pktInterval interval between packet (application-level)
   */
  void SendPacket (Ptr<Socket> socket, uint32_t pktSize,
                   uint32_t pktCount, Time pktInterval);

  /**
   * \brief Get the segment size of the node specified
   *
   * \param who node to get the parameter from
   *
   * \return segment size of the specified node
   */
  uint32_t GetSegSize (SocketWho who);

  /**
   * \brief Get the retransmission threshold
   * \param who node to get the parameter from
   * \return retransmission threshold
   */
  uint32_t GetReTxThreshold (SocketWho who);

  /**
   * \brief Get the number of dupack received
   * \param who node to get the parameter from
   * \return number of dupack
   */
  uint32_t GetDupAckCount (SocketWho who);

  /**
   * \brief Get the number of delayed ack (if present)
   * \param who node to get the parameter from
   * \return number of ack we will wait before sending an ACK
   */
  uint32_t GetDelAckCount (SocketWho who);

  /**
   * \brief Get the timeout of delayed ack (if present)
   * \param who node to get the parameter from
   * \return time we will wait before sending an ACK
   */
  Time GetDelAckTimeout (SocketWho who);

  /**
   * \brief Get the retransmission time
   *
   * \param who node to get the parameter from
   * \return calculated RTO time
   */
  Time GetRto (SocketWho who);

  /**
   * \brief Get the minimun RTO attribute
   *
   * \param who node to get the parameter from
   * \return minimum RTO time
   */
  Time GetMinRto (SocketWho who);

  /**
   * \brief Get the retransmission time for the SYN segments
   *
   * \param who node to get the parameter from
   * \return SYN segments RTO time
   */
  Time GetConnTimeout (SocketWho who);

  /**
   * \brief Get the Rtt estimator of the socket
   *
   * \param who node to get the parameter from
   * \return Rtt estimator
   */
  Ptr<RttEstimator> GetRttEstimator (SocketWho who);

  /**
   * \brief Get the clock granularity attribute
   *
   * \param who node to get the parameter from
   * \return clock granularity
   */
  Time GetClockGranularity (SocketWho who);

  /**
   * \brief Forcefully set a defined size for rx buffer
   *
   * \param who socket to force
   * \param size size of the rx buffer
   */
  void SetRcvBufSize (SocketWho who, uint32_t size);

  /**
   * \brief State on Ack state machine changes
   * \param oldValue old value
   * \param newValue new value
   */
  virtual void CongStateTrace (const TcpSocketState::TcpCongState_t oldValue,
                              const TcpSocketState::TcpCongState_t newValue)
  {
  }

  /**
   * \brief Congestion window changes
   *
   * \param oldValue old value
   * \param newValue new value
   */
  virtual void CWndTrace (uint32_t oldValue, uint32_t newValue)
  {
  }

  /**
   * \brief Socket closed normally
   * \param who the socket closed (SENDER or RECEIVER)
   */
  virtual void NormalClose (SocketWho who)
  {
  }

  /**
   * \brief Socket closed with an error
   *
   * \param who the socket closed (SENDER or RECEIVER)
   */
  virtual void ErrorClose  (SocketWho who)
  {
    /** \todo indicate the error */
  }

  /**
   * \brief Drop on the queue
   * \param who where the drop occurred (SENDER or RECEIVER)
   */
  virtual void QueueDrop   (SocketWho who)
  {
  }

  /**
   * \brief Link drop
   * \param who where the drop occurred (SENDER or RECEIVER)
   */
  virtual void PhyDrop     (SocketWho who)
  {
  }

  /**
   * \brief Received ack
   *
   * Invoked when an ACK is received (no processing is done yet)
   *
   * \param tcb Transmission Control Block
   * \param h the header of segment
   * \param who the socket which has received the ACK (SENDER or RECEIVER)
   */
  virtual void RcvAck      (const Ptr<const TcpSocketState> tcb,
                            const TcpHeader& h, SocketWho who)
  {
  }

  /**
   * \brief Processed ack
   *
   * Invoked after the processing of the ACK
   *
   * \param tcb Transmission Control Block
   * \param h the header of segment
   * \param who the socket which has processed the ACK (SENDER or RECEIVER)
   */
  virtual void ProcessedAck (const Ptr<const TcpSocketState> tcb,
                             const TcpHeader& h, SocketWho who)
  {
  }

  /**
   * \brief Packet transmitted down to IP layer
   *
   * \param p packet
   * \param h header
   * \param who the socket which has received the packet (SENDER or RECEIVER)
   */
  virtual void Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);

  /**
   * \brief Packet received from IP layer
   *
   * \param p packet
   * \param h header
   * \param who  the socket which has received the packet (SENDER or RECEIVER)
   */
  virtual void Rx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);

  /**
   * \brief Rto has expired
   *
   * \param tcb Transmission control block
   * \param who where the RTO has expired (SENDER or RECEIVER)
   */
  virtual void RTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who)
  {
  }

  /**
   * \brief Performs the (eventual) final checks through test asserts
   *
   */
  virtual void FinalChecks ()
  {
  }

  /**
   * \brief Get the channel Propagation Delay
   * \return propagation delay of the channel
   */
  Time GetPropagationDelay () const
  {
    return m_propagationDelay;
  }

  /**
   * \brief Get the data start time
   * \return start time of data packets
   */
  Time GetStartTime () const
  {
    return m_startTime;
  }

  /**
   * \brief Get the MTU of the environment
   * \return MTU of the environment
   */
  uint32_t GetMtu () const
  {
    return m_mtu;
  }

  /**
   * \brief Get the application packet size
   * \return application packet size
   */
  uint32_t GetPktSize () const
  {
    return m_pktSize;
  }

  /**
   * \brief Get the number of application packets
   * \return count of application packets to be generated
   */
  uint32_t GetPktCount () const
  {
    return m_pktCount;
  }

  /**
   * \brief Get the interval to wait for each packet sent down from application
   * to TCP
   * \return interval between packet
   */
  Time GetPktInterval () const
  {
    return m_interPacketInterval;
  }

  /**
   * \brief Get the initial slow start threshold
   * \return initial slow start threshold
   */
  uint32_t GetInitialSsThresh () const
  {
    return m_initialSlowStartThresh;
  }

  /**
   * \brief Get the initial congestion window
   * \return initial cwnd
   */
  uint32_t GetInitialCwnd () const
  {
    return m_initialCwnd;
  }


  TypeId   m_congControlTypeId;      //!< Congestion control

private:
  // Member variables, accessible through getters
  // giving write access to subclass can be dangerous
  Time     m_propagationDelay;     //!< Propagation delay of the channel

  Time     m_startTime;            //!< Data transmission time
  uint32_t m_mtu;                  //!< MTU of the environment

  uint32_t m_pktSize;              //!< Size of the application packet
  uint32_t m_pktCount;             //!< Count of the application packet
  Time     m_interPacketInterval;  //!< Time between sending application packet
                                   //   down to tcp socket
  uint32_t m_initialSlowStartThresh; //!< Initial slow start threshold
  uint32_t m_initialCwnd;            //!< Initial congestion window
  uint32_t m_segmentSize;            //!< Segment size

  Ptr<TcpSocketMsgBase> m_senderSocket;   //!< Pointer to sender socket
  Ptr<TcpSocketMsgBase> m_receiverSocket; //!< Pointer to receiver socket

private:
  // De-multiplexing callbacks.
  void NormalCloseCb  (Ptr<Socket> socket);
  void ErrorCloseCb   (Ptr<Socket> socket);
  void QueueDropCb    (std::string context, Ptr<const Packet> p);
  void PhyDropCb      (std::string context, Ptr<const Packet> p);
  void RcvAckCb       (Ptr<const Packet> p, const TcpHeader& h,
                       Ptr<const TcpSocketBase> tcp);
  void ProcessedAckCb (Ptr<const Packet> p, const TcpHeader& h,
                       Ptr<const TcpSocketBase> tcp);
  void TxPacketCb     (const Ptr<const Packet> p, const TcpHeader& h,
                       const Ptr<const TcpSocketBase> tcp);
  void RxPacketCb     (const Ptr<const Packet> p, const TcpHeader& h,
                       const Ptr<const TcpSocketBase> tcp);
  void RtoExpiredCb   (const Ptr<const TcpSocketState> tcb,
                       const Ptr<const TcpSocketBase> tcp);
  void ForkCb         (Ptr<TcpSocketMsgBase> tcp);
  void HandleAccept (Ptr<Socket> socket, const Address& from);

  InetSocketAddress m_remoteAddr;
};

/**
 * \brief Convenience function to retrieve the ACK state from a TCB
 *
 * \param tcb Transmission control block
 * \return the state of the ACK state machine
 */
static inline TcpSocketState::TcpCongState_t
GetCongStateFrom (Ptr<const TcpSocketState> tcb)
{
  return tcb->m_congState.Get ();
}

} // namespace ns3

#endif // TCPGENERALTEST_H

