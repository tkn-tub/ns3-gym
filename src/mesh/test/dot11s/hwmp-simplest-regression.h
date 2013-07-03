/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Authors: Kirill Andreev  <andreev@iitp.ru>
 */

#include "ns3/test.h"
#include "ns3/node-container.h"
#include "ns3/nstime.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/pcap-file.h"

using namespace ns3;

/**
 * \ingroup dot11s
 * \brief Peering Management & HWM Protocol regression test
 * Initiate scenario with 2 stations. Procedure of opening peer link
 * is the following:
 * \verbatim
 * <-----------|----------->   Broadcast frame
 *             |----------->|  Unicast frame
 *
 *                                        !!! PMP routines:
 * <-----------|----------->|             Beacon
 *             |<-----------|             Peer Link Open frame 
 *             |----------->|             Peer Link Open frame
 *             |----------->|             Peer Link Confirm frame
 *             |<-----------|             Peer Link Confirm frame
 *             |............|             !!! Data started:
 *             |<-----------|-----------> Arp Request
 * <-----------|----------->|             Arp Request (fwd)
 * <-----------|----------->|             PREQ
 *             |<-----------|             PREP
 *             |----------->|             ARP reply
 *             |<-----------|             Data
 *             |----------->|             Data
 *             |............|             Some other beacons
 *             |<-----------|             Data
 *             |----------->|             Data
 *             |............|             !!! Route expiration routines:
 *             |<-----------|-----------> PREQ (route expired)
 *             |----------->|             PREP
 *             |<-----------|             Data
 *             |----------->|             Data
 *             |............|
 * \endverbatim
 * At 10 seconds stations become unreachable, so UDP client tries to
 * close peer link due to TX-fail, and UDP-srver tries to close link
 * due to beacon loss


 */
class HwmpSimplestRegressionTest : public TestCase
{
public:
  HwmpSimplestRegressionTest ();
  virtual ~HwmpSimplestRegressionTest();

  virtual void DoRun ();
  void CheckResults ();

private:
  /// \internal It is important to have pointers here
  NodeContainer * m_nodes;
  /// Simulation time
  Time m_time;
  Ipv4InterfaceContainer m_interfaces;

  void CreateNodes ();
  void CreateDevices ();
  void InstallApplications ();
  void ResetPosition ();
};

