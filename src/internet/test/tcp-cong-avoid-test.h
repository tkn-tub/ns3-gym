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
#ifndef TCPCONGAVOIDTEST_H
#define TCPCONGAVOIDTEST_H

#include "ns3/test.h"
#include "tcp-slow-start-test.h"

namespace ns3 {

/**
 * \brief Test the behavior of RFC congestion avoidance
 *
 * From RFC 5681:\n
 *
 *     cwnd += min (N, SMSS)                      (2)
 *
 *  During congestion avoidance, cwnd is incremented by roughly 1 full-
 *  sized segment per round-trip time (RTT).  Congestion avoidance
 *  continues until congestion is detected.  The basic guidelines for
 *  incrementing cwnd during congestion avoidance are:
 *
 *     * MAY increment cwnd by SMSS bytes
 *
 *     * SHOULD increment cwnd per equation (2) once per RTT
 *
 *     * MUST NOT increment cwnd by more than SMSS bytes
 *
 * To test this behavior and these points, a tracing callback is attached
 * to the cWnd. Each time it increases, the increment is saved. Meanwhile, a
 * timer checks if an amount of time equals to the RTT has passed, and if yes,
 * it checks that the increment has not passed the 1 MSS limit.
 */
class
TcpNewRenoCongAvoidNormalTest : public TcpGeneralTest
{
public:
  TcpNewRenoCongAvoidNormalTest (uint32_t segmentSize, uint32_t packetSize,
                                 uint32_t packets, TypeId& congControl,
                                 const std::string &msg);
protected:
  virtual void CWndTrace (uint32_t oldValue, uint32_t newValue);
  void QueueDrop (SocketWho who);
  void PhyDrop (SocketWho who);
  void NormalClose (SocketWho who);
  void Check ();

  void ConfigureEnvironment ();
  void ConfigureProperties ();

private:
  uint32_t m_segmentSize;
  uint32_t m_packetSize;
  uint32_t m_packets;
  uint32_t m_increment;
  EventId m_event;
  bool   m_initial;
};

} // namespace ns3

#endif // TCPCONGAVOIDTEST_H

