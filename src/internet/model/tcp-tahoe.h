/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

#ifndef TCP_TAHOE_H
#define TCP_TAHOE_H

#include "tcp-socket-base.h"

namespace ns3 {

/**
 * \ingroup socket
 * \ingroup tcp
 *
 * \brief An implementation of a stream socket using TCP.
 *
 * This class contains the Tahoe implementation of TCP. Tahoe is not officially
 * published in RFC. The reference for implementing this is based on
 * Kevin Fall and Sally Floyd, "Simulation-based Comparisons of Tahoe, Reno, and SACK TCP", CCR, 1996
 * http://inst.eecs.berkeley.edu/~ee122/fa05/projects/Project2/proj2_spec_files/sacks.pdf
 * In summary, we have slow start, congestion avoidance, and fast retransmit.
 * The implementation of these algorithms are based on W. R. Stevens's book and
 * also \RFC{2001}.
 */
class TcpTahoe : public TcpSocketBase
{
public:
  static TypeId GetTypeId (void);
  /**
   * Create an unbound tcp socket.
   */
  TcpTahoe (void);
  TcpTahoe (const TcpTahoe& sock);
  virtual ~TcpTahoe (void);

  // From TcpSocketBase
  virtual int Connect (const Address &address);
  virtual int Listen (void);

protected:
  virtual uint32_t Window (void); // Return the max possible number of unacked bytes
  virtual Ptr<TcpSocketBase> Fork (void); // Call CopyObject<TcpTahoe> to clone me
  virtual void NewAck (SequenceNumber32 const& seq); // Inc cwnd and call NewAck() of parent
  virtual void DupAck (const TcpHeader& t, uint32_t count);  // Treat 3 dupack as timeout
  virtual void Retransmit (void); // Retransmit time out

  // Implementing ns3::TcpSocket -- Attribute get/set
  virtual void     SetSegSize (uint32_t size);
  virtual void     SetSSThresh (uint32_t threshold);
  virtual uint32_t GetSSThresh (void) const;
  virtual void     SetInitialCwnd (uint32_t cwnd);
  virtual uint32_t GetInitialCwnd (void) const;
private:
  void InitializeCwnd (void);            // set m_cWnd when connection starts

protected:
  TracedValue<uint32_t>  m_cWnd;         //< Congestion window
  uint32_t               m_ssThresh;     //< Slow Start Threshold
  uint32_t               m_initialCWnd;  //< Initial cWnd value
  uint32_t               m_retxThresh;   //< Fast Retransmit threshold
};

} // namespace ns3

#endif /* TCP_TAHOE_H */
