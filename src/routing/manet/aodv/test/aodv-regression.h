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

#ifndef AODVREGRESSION_H_
#define AODVREGRESSION_H_

#include "ns3/test.h"
#include "ns3/nstime.h"
#include "ns3/node-container.h"

namespace ns3 {
namespace aodv {

/**
 * \ingroup aodv
 * 
 * \brief AODV chain regression test
 *
 * TODO: describe test topology and traffic 
 * 
 * \verbatim
 * TODO: draw expected packets time diagram
 * \endverbatim
 */
class ChainRegressionTest : public TestCase
{
public:
  ChainRegressionTest ();
  ~ChainRegressionTest ();  
private:
  /// Unique PCAP files prefix for this test
  static const char * const PREFIX;
  /// XXX It is important to have pointers here
  NodeContainer * m_nodes;
  
  /// Total simulation time
  const Time m_time;
  /// Chain size
  const uint32_t m_size;
  /// Chain step, meters
  const double m_step;
  
  /// Create test topology
  void CreateNodes ();
  /// Create devices, install TCP/IP stack and applications
  void CreateDevices ();
  /// Compare traces with reference ones
  void CheckResults ();
  /// Go
  bool DoRun ();
};

}
}
#endif /* AODVREGRESSION_H_ */
