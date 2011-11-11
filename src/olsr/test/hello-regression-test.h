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
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

#ifndef HELLO_REGRESSION_TEST_H
#define HELLO_REGRESSION_TEST_H

#include "ns3/test.h"
#include "ns3/nstime.h"
#include "ns3/node-container.h"

namespace ns3
{
namespace olsr
{
/**
 * \ingroup olsr
 * \brief Trivial (still useful) test of OLSR operation
 * 
 * This test creates 2 stations with point-to-point link and runs OLSR without any extra traffic. 
 * It is expected that only HELLO messages will be sent.
 * 
 * Expected trace (5 seconds):
   \verbatim
    1       2
    |------>|   HELLO (empty) src = 10.1.1.1
    |<------|   HELLO (empty) src = 10.1.1.2
    |------>|   HELLO (Link type: Asymmetric link, Neighbor address: 10.1.1.2) src = 10.1.1.1
    |<------|   HELLO (Link type: Asymmetric link, Neighbor address: 10.1.1.1) src = 10.1.1.2
    |------>|   HELLO (Link type: Symmetric link, Neighbor address: 10.1.1.2) src = 10.1.1.1
    |<------|   HELLO (Link type: Symmetric link, Neighbor address: 10.1.1.1) src = 10.1.1.2
   \endverbatim 
 */
class HelloRegressionTest : public TestCase
{
public:
  HelloRegressionTest (); 
  ~HelloRegressionTest ();
private:
  /// Unique PCAP files prefix for this test
  static const char * const PREFIX;
  /// Total simulation time
  const Time m_time;
  /// Create & configure test network
  void CreateNodes ();
  /// Compare traces with reference ones
  void CheckResults ();
  /// Go
  void DoRun ();
};

}
}

#endif /* HELLO_REGRESSION_TEST_H */
