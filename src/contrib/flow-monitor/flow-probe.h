/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2009 INESC Porto
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt> <gjcarneiro@gmail.com>
//

#ifndef __FLOW_PROBE_H__
#define __FLOW_PROBE_H__

#include <map>
#include <vector>

#include "ns3/ref-count-base.h"
#include "ns3/flow-classifier.h"
#include "ns3/nstime.h"

namespace ns3 {

class FlowMonitor;
  

class FlowProbe : public RefCountBase
{
protected:
  
  FlowProbe (Ptr<FlowMonitor> flowMonitor);
  
public:
  ~FlowProbe ();

  struct FlowStats
  {
    FlowStats () : delayFromFirstProbeSum (Seconds (0)), bytes (0), packets (0) {}

    std::vector<uint32_t> packetsDropped; // packetsDropped[reasonCode] => number of dropped packets
    std::vector<uint64_t> bytesDropped; // bytesDropped[reasonCode] => number of dropped bytes
    Time delayFromFirstProbeSum; // divide by 'Scalar (packets)' to get the average delay
    uint64_t bytes;
    uint32_t packets;
  };
  
  typedef std::map<FlowId, FlowStats> Stats;
  
  void AddPacketStats (FlowId flowId, uint32_t packetSize, Time delayFromFirstProbe);
  void AddPacketDropStats (FlowId flowId, uint32_t packetSize, uint32_t reasonCode);
  Stats GetStats () const;

  void SerializeToXmlStream (std::ostream &os, int indent, uint32_t index) const;

protected:
  Ptr<FlowMonitor> m_flowMonitor;
  Stats m_stats;

};


} // namespace ns3

#endif

