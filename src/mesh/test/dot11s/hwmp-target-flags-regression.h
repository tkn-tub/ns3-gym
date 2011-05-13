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
 * \brief This is a test for intermediate reply and saving routing
 * information about neighbour. 4 stations and 3 UDP ping streams are initiated.
 * \verbatim
 *  <-----------|----------->   Broadcast frame
 *              |----------->|  Unicast frame
 *            10          11            12           13
 *             |            |<-----------|----------->|             ARP request (12 asks who has 10)
 *             |            |            |<-----------|-----------> ARP request
 *             |<-----------|----------->|            |             ARP request
 * <-----------|----------->|            |            |             PREQ
 *             |<-----------|----------->|            |             PREQ
 *             |            |<-----------|            |             PREP
 *             |<-----------|            |            |             PREP
 *             |----------->|            |            |             ARP reply
 *             |            |----------->|            |             ARP REPLY
 *             |            |<-----------|            |             Data
 *             |............|............|............|
 *             |<-----------|----------->|            |             ARP request (11 asks who has 10)
 *             |............|............|............|
 *             |----------->|            |            |             ARP reply
 *                  ^ Note, that this arp reply goes without route
 *                  discovery procedure, because route is known from
 *                  previous PREQ/PREP exchange
 *             |<-----------|            |            |             DATA
 *             |............|............|............|
 * <-----------|----------->|            |            |             ARP request (10 asks who has 13)
 *             |............|............|............|
 *             |            |            |<-----------|-----------> PREQ (13 asks about 10) DO=0 RF=1
 *             |            |            |----------->|             PREP (intermediate reply - 12 knows about 10)
 *             |            |<-----------|----------->|             PREQ DO=1 RF=0
 *             |............|............|............|
 *             |----------->|            |            |             PREP
 *             |            |----------->|            |             PREP
 *             |            |            |----------->|             PREP
 * \endverbatim
 */
class HwmpDoRfRegressionTest : public TestCase
{
public:
  HwmpDoRfRegressionTest ();
  virtual ~HwmpDoRfRegressionTest();

  virtual void DoRun ();
  void CheckResults ();

private:
  /// XXX It is important to have pointers here
  NodeContainer * m_nodes;
  /// Simulation time
  Time m_time;
  Ipv4InterfaceContainer m_interfaces;

  void CreateNodes ();
  void CreateDevices ();
  void InstallApplications ();
  void ResetPosition ();
};

