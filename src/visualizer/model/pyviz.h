/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INESC Porto
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
 * C++ helper functions for use by the python visualizer (for things
 * Python is too slow at).
 *
 * Author: Gustavo Carneiro  <gjc@inescporto.pt>
 */
#ifndef NS3_PYVIZ_H
#define NS3_PYVIZ_H

#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-l3-protocol.h"

#include <map>
#include <set>

namespace ns3 {

/**
 * \ingroup visualizer
 *
 * \brief helper class to be used by the visualizer
 * \internal
 *
 * This class is not meant to be used by simulations.  It is only
 * meant to be used by the visualizer tool (PyViz).  The only reason
 * it is public is because Python bindings for it are needed,
 * otherwise it should be considered private.
 **/
class PyViz
{
public:
  PyViz ();
  ~PyViz ();

  void RegisterDropTracePath (std::string const &tracePath);

  void RegisterCsmaLikeDevice (std::string const &deviceTypeName);
  void RegisterWifiLikeDevice (std::string const &deviceTypeName);
  void RegisterPointToPointLikeDevice (std::string const &deviceTypeName);

  // Run simulation until a given (simulated, absolute) time is reached
  void SimulatorRunUntil (Time time);

  static void Pause (std::string const &message);
  std::vector<std::string> GetPauseMessages () const;

  struct TransmissionSample
  {
    Ptr<Node> transmitter;
    Ptr<Node> receiver; // NULL if broadcast
    Ptr<Channel> channel;
    uint32_t bytes;
  };
  typedef std::vector<TransmissionSample> TransmissionSampleList;
  TransmissionSampleList GetTransmissionSamples () const;

  struct PacketDropSample
  {
    Ptr<Node> transmitter;
    uint32_t bytes;
  };
  typedef std::vector<PacketDropSample> PacketDropSampleList;
  PacketDropSampleList GetPacketDropSamples () const;


  struct PacketSample
  {
    Time time;
    Ptr<Packet> packet;
    Ptr<NetDevice> device;
  };
  struct TxPacketSample : PacketSample
  {
    Mac48Address to;
  };
  struct RxPacketSample : PacketSample
  {
    Mac48Address from;
  };

  struct LastPacketsSample
  {
    std::vector<RxPacketSample> lastReceivedPackets;
    std::vector<TxPacketSample> lastTransmittedPackets;
    std::vector<PacketSample> lastDroppedPackets;
  };
  LastPacketsSample GetLastPackets (uint32_t nodeId) const;


  void SetNodesOfInterest (std::set<uint32_t> nodes);

  struct NetDeviceStatistics
  {
    NetDeviceStatistics () : transmittedBytes (0), receivedBytes (0),
                             transmittedPackets (0), receivedPackets (0) {}
    uint64_t transmittedBytes;
    uint64_t receivedBytes;
    uint32_t transmittedPackets;
    uint32_t receivedPackets;
  };

  struct NodeStatistics
  {
    uint32_t nodeId;
    std::vector<NetDeviceStatistics> statistics;
  };

  std::vector<NodeStatistics> GetNodesStatistics () const;

  enum PacketCaptureMode {
    PACKET_CAPTURE_DISABLED=1, // packet capture is disabled
    PACKET_CAPTURE_FILTER_HEADERS_OR, // packet capture if any of the indicated headers is present
    PACKET_CAPTURE_FILTER_HEADERS_AND, // packet capture if all of the indicated headers are present
  };

  struct PacketCaptureOptions
  {
    std::set<TypeId> headers;
    uint32_t numLastPackets;
    PacketCaptureMode mode;
  };

  void SetPacketCaptureOptions (uint32_t nodeId, PacketCaptureOptions options);


  // Yes, I know, this is just a utility function, not really related to the class in any way.

  // -#- @lineX1(direction=inout); @lineY1(direction=inout); @lineX2(direction=inout); @lineY2(direction=inout) -#-
  static void LineClipping (double boundsX1, double boundsY1, double boundsX2, double boundsY2, double &lineX1, double &lineY1, double &lineX2, double &lineY2); // don't break this line or pybindgen will not be able to pick up the above annotation :(


private:

  bool GetPacketCaptureOptions (uint32_t nodeId, const PacketCaptureOptions **outOptions) const;
  static bool FilterPacket (Ptr<const Packet> packet, const PacketCaptureOptions &options);


  typedef std::pair<Ptr<Channel>, uint32_t> TxRecordKey;

  struct TxRecordValue
  {
    Time time;
    Ptr<Node> srcNode;
    bool isBroadcast;
  };

  struct TransmissionSampleKey
  {
    bool operator < (TransmissionSampleKey const &other) const;
    bool operator == (TransmissionSampleKey const &other) const;
    Ptr<Node> transmitter;
    Ptr<Node> receiver; // NULL if broadcast
    Ptr<Channel> channel;
  };

  struct TransmissionSampleValue
  {
    uint32_t bytes;
  };

  // data
  std::map<uint32_t, PacketCaptureOptions> m_packetCaptureOptions;
  std::vector<std::string> m_pauseMessages;
  std::map<TxRecordKey, TxRecordValue> m_txRecords;
  std::map<TransmissionSampleKey, TransmissionSampleValue> m_transmissionSamples;
  std::map<Ptr<Node>, uint32_t> m_packetDrops;
  std::set<uint32_t> m_nodesOfInterest; // list of node IDs whose transmissions will be monitored
  std::map<uint32_t, Time> m_packetsOfInterest; // list of packet UIDs that will be monitored
  std::map<uint32_t, LastPacketsSample> m_lastPackets;
  std::map<uint32_t, std::vector<NetDeviceStatistics> > m_nodesStatistics;

  // Trace callbacks
  void TraceNetDevTxCommon (std::string const &context, Ptr<const Packet> packet, Mac48Address const &destination);
  void TraceNetDevRxCommon (std::string const &context, Ptr<const Packet> packet, Mac48Address const &source);

  void TraceNetDevTxWifi (std::string context, Ptr<const Packet> packet);
  void TraceNetDevRxWifi (std::string context, Ptr<const Packet> packet);

  void TraceDevQueueDrop (std::string context, Ptr<const Packet> packet);
  void TraceIpv4Drop (std::string context, ns3::Ipv4Header const &hdr, Ptr<const Packet> packet,
                      ns3::Ipv4L3Protocol::DropReason reason, Ptr<Ipv4> dummy_ipv4, uint32_t interface);

  void TraceNetDevTxCsma (std::string context, Ptr<const Packet> packet);
  void TraceNetDevRxCsma (std::string context, Ptr<const Packet> packet);
  void TraceNetDevPromiscRxCsma (std::string context, Ptr<const Packet> packet);

  void TraceNetDevTxPointToPoint (std::string context, Ptr<const Packet> packet);
  void TraceNetDevRxPointToPoint (std::string context, Ptr<const Packet> packet);

  void TraceNetDevTxWimax (std::string context, Ptr<const Packet> packet, Mac48Address const &destination);
  void TraceNetDevRxWimax (std::string context, Ptr<const Packet> packet, Mac48Address const &source);

  inline NetDeviceStatistics & FindNetDeviceStatistics (int node, int interface);

  void DoPause (std::string const &message);

  bool m_stop;
  EventId m_stopCallbackEvent;
  void CallbackStopSimulation ();
};


}

#endif /* NS3_PYVIZ_H */
