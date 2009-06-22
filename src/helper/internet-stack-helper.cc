/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Faker Moatamri <faker.moatamri@sophia.inria.fr>
 */

/**
 * \ingroup internetStack
 * \defgroup internetStackModel Internet Stack Model
 *
 * \section internetStackTracingModel Tracing in the Internet Stack
 *
 * The internet stack provides a number of trace sources in its various
 * protocol implementations.  These trace sources can be hooked using your own 
 * custom trace code, or you can use our helper functions in some cases to 
 * arrange for tracing to be enabled.
 *
 * \subsection internetStackArpTracingModel Tracing in ARP
 *
 * ARP provides two trace hooks, one in the cache, and one in the layer three
 * protocol.  The trace accessor in the cache is given the name "Drop."  When
 * a packet is transmitted over an interface that requires ARP, it is first
 * queued for transmission in the ARP cache until the required MAC address is
 * resolved.  There are a number of retries that may be done trying to get the 
 * address, and if the maximum retry count is exceeded the packet in question 
 * is dropped by ARP.  The single trace hook in the ARP cache is called,
 *
 * - If an outbound packet is placed in the ARP cache pending address resolution
 *   and no resolution can be made within the maximum retry count, the outbound 
 *   packet is dropped and this trace is fired;
 *
 * A second trace hook lives in the ARP L3 protocol (also named "Drop") and may 
 * be called for a  number of reasons.
 *
 * - If an ARP reply is received for an entry that is not waiting for a reply,
 *   the ARP reply packet is dropped and this trace is fired;
 * - If an ARP reply is received for a non-existant entry, the ARP reply packet 
 *   is dropped and this trace is fired;
 * - If an ARP cache entry is in the DEAD state (has timed out) and an ARP reply
 *   packet is received, the reply packet is dropped and this trace is fired.
 * - Each ARP cache entry has a queue of pending packets.  If the size of the
 *   queue is exceeded, the outbound packet is dropped and this trace is fired.
 *
 * \subsection internetStackIpv4TracingModel Tracing in IPv4
 *
 * The IPv4 layer three protocol provides three trace hooks.  These are the 
 * "Tx" (ns3::Ipv4L3Protocol::m_txTrace), "Rx" (ns3::Ipv4L3Protocol::m_rxTrace) 
 * and "Drop" (ns3::Ipv4L3Protocol::m_dropTrace) trace sources.
 *
 * The "Tx" trace is fired in a number of situations, all of which indicate that
 * a given packet is about to be sent down to a given ns3::Ipv4Interface.
 *
 * - In the case of a packet destined for the broadcast address, the 
 *   Ipv4InterfaceList is iterated and for every interface that is up and can
 *   fragment the packet or has a large enough MTU to transmit the packet,
 *   the trace is hit.  See ns3::Ipv4L3Protocol::Send.
 *
 * - In the case of a packet that needs routing, the "Tx" trace may be fired
 *   just before a packet is sent to the interface appropriate to the default 
 *   gateway.  See ns3::Ipv4L3Protocol::SendRealOut.
 *
 * - Also in the case of a packet that needs routing, the "Tx" trace may be 
 *   fired just before a packet is sent to the outgoing interface appropriate
 *   to the discovered route.  See ns3::Ipv4L3Protocol::SendRealOut.
 *
 * The "Rx" trace is fired when a packet is passed from the device up to the
 * ns3::Ipv4L3Protocol::Receive function.
 *
 * - In the receive function, the Ipv4InterfaceList is iterated, and if the
 *   Ipv4Interface corresponding to the receiving device is fount to be in the
 *   UP state, the trace is fired.
 *
 * The "Drop" trace is fired in any case where the packet is dropped (in both
 * the transmit and receive paths).
 *
 * - In the ns3::Ipv4Interface::Receive function, the packet is dropped and the
 *   drop trace is hit if the interface corresponding to the receiving device
 *   is in the DOWN state.
 *
 * - Also in the ns3::Ipv4Interface::Receive function, the packet is dropped and
 *   the drop trace is hit if the checksum is found to be bad.
 *
 * - In ns3::Ipv4L3Protocol::Send, an outgoing packet bound for the broadcast
 *   address is dropped and the "Drop" trace is fired if the "don't fragement"
 *   bit is set and fragmentation is available and required.
 *
 * - Also in ns3::Ipv4L3Protocol::Send, an outgoing packet destined for the 
 *   broadcast address is dropped and the "Drop" trace is hit if fragmentation
 *   is not available and is required (MTU < packet size).
 *
 * - In the case of a broadcast address, an outgoing packet is cloned for each
 *   outgoing interface.  If any of the interfaces is in the DOWN state, the 
 *   "Drop" trace event fires with a reference to the copied packet.
 *
 * - In the case of a packet requiring a route, an outgoing packet is dropped
 *   and the "Drop" trace event fires if no route to the remote host is found.
 *
 * - In ns3::Ipv4L3Protocol::SendRealOut, an outgoing packet being routed
 *   is dropped and the "Drop" trace is fired if the "don't fragement" bit is 
 *   set and fragmentation is available and required.
 *
 * - Also in ns3::Ipv4L3Protocol::SendRealOut, an outgoing packet being routed
 *   is dropped and the "Drop" trace is hit if fragmentation is not available 
 *   and is required (MTU < packet size).
 *
 * - An outgoing packet being routed is dropped and the "Drop" trace event fires
 *   if the required Ipv4Interface is in the DOWN state.
 *
 * - If a packet is being forwarded, and the TTL is exceeded (see
 *   ns3::Ipv4L3Protocol::DoForward), the packet is dropped and the "Drop" trace 
 *   event is fired.
 *
 * \subsection internetStackNs3TCPTracingModel Tracing in ns-3 TCP
 *
 * There is currently one trace source in the ns-3 TCP implementation named
 * "CongestionWindow" (see ns3::TcpSocketImpl::m_cWnd).  This is set in a number
 * of places (see file tcp-socket-impl.cc) whenever the value of the congestion
 * window is changed.
 *
 * \subsection internetStackNscTCPTracingModel Tracing in NSC TCP
 *
 * There is currently one trace source in the Network Simulation Cradle TCP 
 * implementation named "CongestionWindow" (see ns3::NscTcpSocketImpl::m_cWnd).
 * This is set in a number of places (see file nsc-tcp-socket-impl.cc) when 
 * the value of the cogestion window is initially set.  Note that this is not
 * instrumented from the underlying TCP implementaion.
 *
 * \subsection internetStackNs3UdpTracingModel Tracing in ns-3 UDP
 *
 * There is currently one trace source in the ns-3 UDP implementation named
 * "Drop" (see ns3::UdpSocketImpl::m_dropTrace).  This is set when a packet
 * is received in ns3::UdpSocketImpl::ForwardUp and the receive buffer cannot
 * accomodate the encapsulated data.
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/names.h"
#include "ns3/ipv4.h"
#include "internet-stack-helper.h"
#include "ns3/packet-socket-factory.h"
#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/core-config.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/ipv4-static-routing.h"
#include <limits>

namespace ns3 {

std::vector<InternetStackHelper::Trace> InternetStackHelper::m_traces;
std::string InternetStackHelper::m_pcapBaseFilename;

InternetStackHelper::InternetStackHelper ()
{
  SetTcp ("ns3::TcpL4Protocol");
}

void
InternetStackHelper::Cleanup (void)
{
  uint32_t illegal = std::numeric_limits<uint32_t>::max();

  for (std::vector<Trace>::iterator i = m_traces.begin ();
       i != m_traces.end (); i++)
  {
    i->nodeId = illegal;
    i->interfaceId = illegal;
    i->writer = 0;
  }
  m_traces.clear ();
}

void
InternetStackHelper::SetTcp (const std::string tid)
{
  m_tcpFactory.SetTypeId (tid);
}

void 
InternetStackHelper::SetTcp (std::string tid, std::string n0, const AttributeValue &v0)
{
  m_tcpFactory.SetTypeId (tid);
  m_tcpFactory.Set (n0,v0);
}

void 
InternetStackHelper::Install (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i);
    }
}

static void
CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId)
{
  ObjectFactory factory;
  factory.SetTypeId(typeId);
  Ptr<Object> protocol = factory.Create <Object> ();
  node->AggregateObject (protocol);
}

void
InternetStackHelper::Install (Ptr<Node> node) const
{
  if (node->GetObject<Ipv4> () != 0)
    {
      NS_FATAL_ERROR ("InternetStackHelper::Install(): Aggregating " 
                      "an InternetStack to a node with an existing Ipv4 object");
      return;
    }

  CreateAndAggregateObjectFromTypeId (node, "ns3::ArpL3Protocol");
  CreateAndAggregateObjectFromTypeId (node, "ns3::Ipv4L3Protocol");
  CreateAndAggregateObjectFromTypeId (node, "ns3::Icmpv4L4Protocol");
  CreateAndAggregateObjectFromTypeId (node, "ns3::UdpL4Protocol");
  node->AggregateObject (m_tcpFactory.Create<Object> ());
  Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory> ();
  node->AggregateObject (factory);
  // Set routing
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  // XXX cut this over to use of TypeIds and factories
  Ptr<Ipv4ListRouting> ipv4Routing = CreateObject<Ipv4ListRouting> ();
  Ptr<Ipv4StaticRouting> ipv4staticRouting = CreateObject<Ipv4StaticRouting> ();
  ipv4Routing->AddRoutingProtocol (ipv4staticRouting, 0);
  ipv4->SetRoutingProtocol (ipv4Routing);
}

void
InternetStackHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  Install (node);
}
void
InternetStackHelper::EnableAscii (std::ostream &os, NodeContainer n)
{
  Packet::EnablePrinting ();
  std::ostringstream oss;
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      oss << "/NodeList/" << node->GetId () << "/$ns3::Ipv4L3Protocol/Drop";
      Config::Connect (oss.str (), MakeBoundCallback (&InternetStackHelper::AsciiDropEvent, &os));
      oss.str ("");
      oss << "/NodeList/" << node->GetId () << "/$ns3::ArpL3Protocol/Drop";
      Config::Connect (oss.str (), MakeBoundCallback (&InternetStackHelper::AsciiDropEvent, &os));
      oss.str ("");
    }
}

void
InternetStackHelper::EnableAsciiAll (std::ostream &os)
{
  EnableAscii (os, NodeContainer::GetGlobal ());
}

void
InternetStackHelper::EnablePcapAll (std::string filename)
{
  Simulator::ScheduleDestroy (&InternetStackHelper::Cleanup);

  InternetStackHelper::m_pcapBaseFilename = filename;
  Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/Tx",
                   MakeCallback (&InternetStackHelper::LogTxIp));
  Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/Rx",
                   MakeCallback (&InternetStackHelper::LogRxIp));
}

uint32_t
InternetStackHelper::GetNodeIndex (std::string context)
{
  std::string::size_type pos;
  pos = context.find ("/NodeList/");
  NS_ASSERT (pos == 0);
  std::string::size_type afterNodeIndex = context.find ("/", 11);
  NS_ASSERT (afterNodeIndex != std::string::npos);
  std::string index = context.substr (10, afterNodeIndex - 10);
  std::istringstream iss;
  iss.str (index);
  uint32_t nodeIndex;
  iss >> nodeIndex;
  return nodeIndex;
}

void
InternetStackHelper::LogTxIp (std::string context, Ptr<const Packet> packet, uint32_t interfaceIndex)
{
  Ptr<PcapWriter> writer = InternetStackHelper::GetStream (GetNodeIndex (context), interfaceIndex);
  writer->WritePacket (packet);
}

void
InternetStackHelper::LogRxIp (std::string context, Ptr<const Packet> packet, uint32_t interfaceIndex)
{
  Ptr<PcapWriter> writer = InternetStackHelper::GetStream (GetNodeIndex (context), interfaceIndex);
  writer->WritePacket (packet);
}

Ptr<PcapWriter>
InternetStackHelper::GetStream (uint32_t nodeId, uint32_t interfaceId)
{
  for (std::vector<Trace>::iterator i = m_traces.begin ();
       i != m_traces.end (); i++)
  {
    if (i->nodeId == nodeId &&
        i->interfaceId == interfaceId)
    {
      return i->writer;
    }
  }
  InternetStackHelper::Trace trace;
  trace.nodeId = nodeId;
  trace.interfaceId = interfaceId;
  trace.writer = Create<PcapWriter> ();
  std::ostringstream oss;
  oss << m_pcapBaseFilename << "-" << nodeId << "-" << interfaceId << ".pcap";
  trace.writer->Open (oss.str ());
  trace.writer->WriteIpHeader ();
  m_traces.push_back (trace);
  return trace.writer;
}

void
InternetStackHelper::AsciiDropEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "d " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}

} // namespace ns3
