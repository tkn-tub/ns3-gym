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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */
#include "ns3/test.h"
#include "ns3/node-container.h"
#include "ns3/nstime.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/pcap-file.h"

using namespace ns3;

/**
 * \ingroup dot11s
 * \brief test for multihop path establishing and path error
 * procedures
 * Initiate scenario with 6 stations. Procedure of opening peer link
 * is the following: (PMP routines are not shown)
 * \verbatim
 *      1    2    3    4    5    6
 *      |    |    |    |    |<---|--->  ARP request (first attempt has failed!)
 *      |....|....|....|....|....|      ARP request
 *      |<---|--->|    |    |    |      ARP request
 *  <---|--->|    |    |    |    |      PREQ       } This order is broken
 *  <---|--->|    |    |    |    |      ARP request} due to BroadcastDca 
 *      |<---|--->|    |    |    |      PREQ
 *      |....|....|....|....|....|      ARP request
 *      |    |    |    |<---|--->|      PREQ
 *      |    |    |    |    |<---|      PREP
 *      |....|....|....|....|....|      PREP 
 *      |<---|    |    |    |    |      PREP
 *      |--->|    |    |    |    |      ARP response
 *      |....|....|....|....|....|      ARP response
 *      |    |    |    |    |--->|      ARP response
 *      |    |    |    |    |<---|      Data
 *      |....|....|....|....|....|      Data
 *      |<---|    |    |    |    |      Data
 *  <---|--->|    |    |    |    |      ARP request
 *      |....|....|....|....|....|      ARP request
 *      |    |    |    |    |<---|--->  ARP request
 *      |    |    |    |    |<---|      ARP response
 *      |....|....|....|....|....|      ARP response
 *      |<---|    |    |    |    |      ARP response
 *      |--->|    |    |    |    |      Data
 * At 5 station number 4 disappears, and PERR is forwarded from 3 to 1
 * and from 5 to 6, and station 6 starts path discovery procedure
 * again:
 *      |    |<---|         |--->|      PERR (one due to beacon loss and one due to TX error)
 *      |<---|    |         |    |      PERR
 *      |    |    |         |<---|--->  PREQ
 *      |    |    |     <---|--->|      PREQ
 *      |....|....|.........|....|      Repeated attempts of PREQ
 * \endverbatim
 */

class HwmpReactiveRegressionTest : public TestCase
{
public:
  HwmpReactiveRegressionTest ();
  virtual ~HwmpReactiveRegressionTest();

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

