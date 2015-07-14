/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 ResiliNets, ITTC, University of Kansas 
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
 * Authors: Siddharth Gangadhar <siddharth@ittc.ku.edu>, Truc Anh N. Nguyen <annguyen@ittc.ku.edu>,
 * and Greeshma Umapathi
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#ifndef TCP_WESTWOOD_H
#define TCP_WESTWOOD_H

#include "tcp-socket-base.h"
#include "ns3/packet.h"

namespace ns3 {

/**
 * \ingroup socket
 * \ingroup tcp
 *
 * \brief An implementation of a stream socket using TCP.
 *
 * This class contains the implementation of TCP Westwood and Westwood+.
 *
 * Westwood and Westwood+ employ the AIAD (Additive Increase/Adaptive Decrease) 
 * congestion control paradigm. When a congestion episode happens, 
 * instead of halving the cwnd, these protocols try to estimate the network's
 * bandwidth and use the estimated value to adjust the cwnd. 
 * While Westwood performs the bandwidth sampling every ACK reception, 
 * Westwood+ samples the bandwidth every RTT.
 *
 * The two main methods in the implementation are the CountAck (const TCPHeader&)
 * and the EstimateBW (int, const, Time). The CountAck method calculates
 * the number of acknowledged segments on the receipt of an ACK.
 * The EstimateBW estimates the bandwidth based on the value returned by CountAck
 * and the sampling interval (last ACK inter-arrival time for Westwood and last RTT for Westwood+).   
 */
class TcpWestwood : public TcpSocketBase
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  TcpWestwood (void);
  /**
   * \brief Copy constructor
   * \param sock the object to copy
   */
  TcpWestwood (const TcpWestwood& sock);
  virtual ~TcpWestwood (void);

  /**
   * \brief Protocol variant (Westwood or Westwood+)
   */
  enum ProtocolType 
  {
    WESTWOOD,
    WESTWOODPLUS
  };

  /**
   * \brief Filter type (None or Tustin)
   */
  enum FilterType 
  {
    NONE,
    TUSTIN
  };

protected:
  virtual Ptr<TcpSocketBase> Fork (void); // Call CopyObject<TcpTahoe> to clone me
  virtual void NewAck (SequenceNumber32 const& seq); // Inc cwnd and call NewAck() of parent
  virtual void DupAck (const TcpHeader& t, uint32_t count);  // Treat 3 dupack as timeout
  virtual void Retransmit (void); // Retransmit time out

  /**
   * Process the newly received ACK
   *
   * \param packet the received ACK packet
   * \param tcpHeader the header attached to the ACK packet
   */  
  virtual void ReceivedAck (Ptr<Packet> packet, const TcpHeader& tcpHeader);
  
  /**
   * Estimate the RTT, record the minimum value,
   * and run a clock on the RTT to trigger Westwood+ bandwidth sampling
   * \param header the packet header
   */
  virtual void EstimateRtt (const TcpHeader& header);

private:
  /**
   * Calculate the number of acknowledged packets upon the receipt of an ACK packet
   *
   * \param tcpHeader the header of the received ACK packet
   * \return the number of ACKed packets
   */
  int CountAck (const TcpHeader& tcpHeader);

  /**
   * Update the total number of acknowledged packets during the current RTT
   *
   * \param acked the number of packets the currently received ACK acknowledges
   */
  void UpdateAckedSegments (int acked);

  /**
   * Estimate the network's bandwidth
   *
   * \param acked the number of acknowledged packets returned by CountAck
   * \param tcpHeader the header of the packet
   * \param rtt the RTT estimation
   */
  void EstimateBW (int acked, const TcpHeader& tcpHeader, Time rtt);

  /**
   * Tustin filter
   */
  void Filtering (void);

protected:
  bool                   m_inFastRec;              //!< Currently in fast recovery if TRUE

  TracedValue<double>    m_currentBW;              //!< Current value of the estimated BW
  double                 m_lastSampleBW;           //!< Last bandwidth sample
  double                 m_lastBW;                 //!< Last bandwidth sample after being filtered
  Time                   m_minRtt;                 //!< Minimum RTT
  double                 m_lastAck;                //!< The time last ACK was received
  SequenceNumber32       m_prevAckNo;              //!< Previously received ACK number
  int                    m_accountedFor;           //!< The number of received DUPACKs
  enum ProtocolType      m_pType;                  //!< 0 for Westwood, 1 for Westwood+
  enum FilterType        m_fType;                  //!< 0 for none, 1 for Tustin

  int                    m_ackedSegments;          //!< The number of segments ACKed between RTTs
  bool                   m_IsCount;                //!< Start keeping track of m_ackedSegments for Westwood+ if TRUE
  EventId                m_bwEstimateEvent;        //!< The BW estimation event for Westwood+

};

} // namespace ns3

#endif /* TCP_WESTWOOD_H */
