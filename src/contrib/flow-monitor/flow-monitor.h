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

#ifndef __FLOW_MONITOR_H__
#define __FLOW_MONITOR_H__

#include <vector>
#include <map>

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/flow-probe.h"
#include "ns3/flow-classifier.h"
#include "ns3/histogram.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"

namespace ns3 {


class FlowMonitor : public Object
{
public:

  struct FlowStats
  {
    Time     timeFirstTxPacket;
    Time     timeFirstRxPacket;
    Time     timeLastTxPacket;
    Time     timeLastRxPacket;
    Time     delaySum; // delayCount == rxPackets
    Time     jitterSum; // jitterCount == rxPackets - 1
    Time     lastDelay;
    uint64_t txBytes;
    uint64_t rxBytes;
    uint32_t txPackets;
    uint32_t rxPackets;
    uint32_t lostPackets;
    uint32_t timesForwarded; // number of times a packet was forwarded, summed for all received packets
    Histogram delayHistogram;
    Histogram jitterHistogram;
    Histogram packetSizeHistogram;
    std::vector<uint32_t> packetsDropped; // packetsDropped[reasonCode] => number of dropped packets
    std::vector<uint64_t> bytesDropped; // bytesDropped[reasonCode] => number of dropped bytes
  };

  // --- basic methods ---
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  FlowMonitor ();
  void SetFlowClassifier (Ptr<FlowClassifier> classifier);

  void Start (const Time &time);
  void Stop (const Time &time);
  void StartRightNow ();
  void StopRightNow ();

  // --- methods to be used by the FlowMonitorProbe's only ---
  void AddProbe (Ptr<FlowProbe> probe);
  void ReportFirstTx (Ptr<FlowProbe> probe, FlowId flowId, FlowPacketId packetId, uint32_t packetSize);
  void ReportForwarding (Ptr<FlowProbe> probe, FlowId flowId, FlowPacketId packetId, uint32_t packetSize);
  void ReportLastRx (Ptr<FlowProbe> probe, FlowId flowId, FlowPacketId packetId, uint32_t packetSize);
  void ReportDrop (Ptr<FlowProbe> probe, FlowId flowId, FlowPacketId packetId,
                   uint32_t packetSize, uint32_t reasonCode);
  

  /// Check right now for packets that appear to be lost
  void CheckForLostPackets ();
  void CheckForLostPackets (Time maxDelay);  

  // --- methods to get the results ---
  // remember to call CheckForLostPackets first!
  std::map<FlowId, FlowStats> GetFlowStats () const;
  std::vector< Ptr<FlowProbe> > GetAllProbes () const;

  void SerializeToXmlStream (std::ostream &os, int indent, bool enableHistograms, bool enableProbes);
  std::string SerializeToXmlString (int indent, bool enableHistograms, bool enableProbes);
  void SerializeToXmlFile (std::string fileName, bool enableHistograms, bool enableProbes);


protected:

  virtual void NotifyConstructionCompleted ();

private:

  struct TrackedPacket
  {
    Time firstSeenTime; // absolute time when the packet was first seen by a probe
    Time lastSeenTime; // absolute time when the packet was last seen by a probe
    uint32_t timesForwarded; // number of times the packet was reportedly forwarded
  };

  // FlowId --> FlowStats
  std::map<FlowId, FlowStats> m_flowStats;

  // (FlowId,PacketId) --> TrackedPacket
  typedef std::map< std::pair<FlowId, FlowPacketId>, TrackedPacket> TrackedPacketMap;
  TrackedPacketMap m_trackedPackets;
  Time m_maxPerHopDelay;
  std::vector< Ptr<FlowProbe> > m_flowProbes;

  // note: this is needed only for serialization
  Ptr<FlowClassifier> m_classifier;

  EventId m_startEvent;
  EventId m_stopEvent;
  bool m_enabled;
  double m_delayBinWidth;
  double m_jitterBinWidth;
  double m_packetSizeBinWidth;

  FlowStats& GetStatsForFlow (FlowId flowId);
  void PeriodicCheckForLostPackets ();
};


} // namespace ns3

#endif

