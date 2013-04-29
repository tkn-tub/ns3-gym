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

#ifndef FLOW_PROBE_H
#define FLOW_PROBE_H

#include <map>
#include <vector>

#include "ns3/object.h"
#include "ns3/flow-classifier.h"
#include "ns3/nstime.h"

namespace ns3 {

class FlowMonitor;

/// The FlowProbe class is responsible for listening for packet events
/// in a specific point of the simulated space, report those events to
/// the global FlowMonitor, and collect its own flow statistics
/// regarding only the packets that pass through that probe.
class FlowProbe : public Object
{
private:
  FlowProbe (FlowProbe const &);
  FlowProbe& operator= (FlowProbe const &);

protected:

  FlowProbe (Ptr<FlowMonitor> flowMonitor);
  virtual void DoDispose (void);

public:
  virtual ~FlowProbe ();

  struct FlowStats
  {
    FlowStats () : delayFromFirstProbeSum (Seconds (0)), bytes (0), packets (0) {}

    /// packetsDropped[reasonCode] => number of dropped packets
    std::vector<uint32_t> packetsDropped;
    /// bytesDropped[reasonCode] => number of dropped bytes
    std::vector<uint64_t> bytesDropped;
    /// divide by 'packets' to get the average delay from the
    /// first (entry) probe up to this one (partial delay)
    Time delayFromFirstProbeSum;
    /// Number of bytes seen of this flow
    uint64_t bytes;
    /// Number of packets seen of this flow
    uint32_t packets;
  };

  typedef std::map<FlowId, FlowStats> Stats;

  void AddPacketStats (FlowId flowId, uint32_t packetSize, Time delayFromFirstProbe);
  void AddPacketDropStats (FlowId flowId, uint32_t packetSize, uint32_t reasonCode);

  /// Get the partial flow statistics stored in this probe.  With this
  /// information you can, for example, find out what is the delay
  /// from the first probe to this one.
  Stats GetStats () const;

  void SerializeToXmlStream (std::ostream &os, int indent, uint32_t index) const;

protected:
  Ptr<FlowMonitor> m_flowMonitor;
  Stats m_stats;

};


} // namespace ns3

#endif /* FLOW_PROBE_H */
