/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008 INRIA
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
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#include "wimax-helper.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include <string>
#include "ns3/config.h"
#include "ns3/wimax-net-device.h"
#include "ns3/bs-net-device.h"
#include "ns3/ss-net-device.h"
#include "ns3/wimax-channel.h"
#include "ns3/simple-ofdm-wimax-channel.h"
#include "ns3/wimax-phy.h"
#include "ns3/simple-ofdm-wimax-phy.h"
#include "ns3/pointer.h"
#include "ns3/wimax-mac-to-mac-header.h"


NS_LOG_COMPONENT_DEFINE ("WimaxHelper");

namespace ns3 {

WimaxHelper::WimaxHelper (void)
  : m_channel (0)
{
}

WimaxHelper::~WimaxHelper (void)
{
}

void WimaxHelper::EnableAsciiForConnection (Ptr<OutputStreamWrapper> os,
                                            uint32_t nodeid,
                                            uint32_t deviceid,
                                            char *netdevice,
                                            char *connection)
{
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::" << netdevice << "/" << connection
      << "/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultEnqueueSinkWithContext, os));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::" << netdevice << "/" << connection
      << "/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDequeueSinkWithContext, os));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::" << netdevice << "/" << connection
      << "/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDropSinkWithContext, os));
}

Ptr<WimaxPhy> WimaxHelper::CreatePhy (PhyType phyType)
{
  Ptr<WimaxPhy> phy;
  switch (phyType)
    {
    case SIMPLE_PHY_TYPE_OFDM:
      phy = CreateObject<SimpleOfdmWimaxPhy> ();
      if (!m_channel)
        {
          m_channel = CreateObject<SimpleOfdmWimaxChannel> (SimpleOfdmWimaxChannel::COST231_PROPAGATION);
        }
      break;
    default:
      NS_FATAL_ERROR ("Invalid physical type");
      break;
    }

  return phy;
}

void WimaxHelper::SetPropagationLossModel (SimpleOfdmWimaxChannel::PropModel propagationModel)
{
  if (!m_channel)
    {
      m_channel = CreateObject<SimpleOfdmWimaxChannel> ();
    }
  m_channel->GetObject<SimpleOfdmWimaxChannel> ()->SetPropagationModel (propagationModel);
}

Ptr<WimaxPhy> WimaxHelper::CreatePhy (PhyType phyType, char * SNRTraceFilePath, bool activateLoss)
{
  Ptr<WimaxPhy> phy;
  Ptr<SimpleOfdmWimaxPhy> sphy;
  switch (phyType)
    {
    case SIMPLE_PHY_TYPE_OFDM:
      sphy = CreateObject<SimpleOfdmWimaxPhy> ();
      phy = sphy;
      sphy->SetSNRToBlockErrorRateTracesPath (SNRTraceFilePath);
      sphy->ActivateLoss (activateLoss);
      if (!m_channel)
        {
          m_channel = CreateObject<SimpleOfdmWimaxChannel> (SimpleOfdmWimaxChannel::COST231_PROPAGATION);
        }
      break;
    default:
      NS_FATAL_ERROR ("Invalid physical type");
      break;
    }

  return phy;
}

Ptr<WimaxPhy> WimaxHelper::CreatePhyWithoutChannel (PhyType phyType)
{
  Ptr<WimaxPhy> phy;
  switch (phyType)
    {
    case SIMPLE_PHY_TYPE_OFDM:
      phy = CreateObject<SimpleOfdmWimaxPhy> ();
      break;
    default:
      NS_FATAL_ERROR ("Invalid physical type");
      break;
    }

  return phy;
}

Ptr<WimaxPhy> WimaxHelper::CreatePhyWithoutChannel (PhyType phyType, char * SNRTraceFilePath, bool activateLoss)
{
  Ptr<WimaxPhy> phy;
  Ptr<SimpleOfdmWimaxPhy> sphy;
  switch (phyType)
    {
    case SIMPLE_PHY_TYPE_OFDM:
      sphy = CreateObject<SimpleOfdmWimaxPhy> ();
      phy = sphy;
      sphy->SetSNRToBlockErrorRateTracesPath (SNRTraceFilePath);
      sphy->ActivateLoss (activateLoss);
      break;
    default:
      NS_FATAL_ERROR ("Invalid physical type");
      break;
    }

  return phy;
}

Ptr<UplinkScheduler> WimaxHelper::CreateUplinkScheduler (SchedulerType schedulerType)
{
  Ptr<UplinkScheduler> uplinkScheduler;
  switch (schedulerType)
    {
    case SCHED_TYPE_SIMPLE:
      uplinkScheduler = CreateObject<UplinkSchedulerSimple> ();
      break;
    case SCHED_TYPE_RTPS:
      uplinkScheduler = CreateObject<UplinkSchedulerRtps> ();
      break;
    case SCHED_TYPE_MBQOS:
      uplinkScheduler = CreateObject<UplinkSchedulerMBQoS> (Seconds (0.25));
      break;
    default:
      NS_FATAL_ERROR ("Invalid scheduling type");
      break;
    }
  return uplinkScheduler;
}

Ptr<BSScheduler> WimaxHelper::CreateBSScheduler (SchedulerType schedulerType)
{
  Ptr<BSScheduler> bsScheduler;
  switch (schedulerType)
    {
    case SCHED_TYPE_SIMPLE:
      bsScheduler = CreateObject<BSSchedulerSimple> ();
      break;
    case SCHED_TYPE_RTPS:
      bsScheduler = CreateObject<BSSchedulerRtps> ();
      break;
    case SCHED_TYPE_MBQOS:
      bsScheduler = CreateObject<BSSchedulerSimple> ();
      break;
    default:
      NS_FATAL_ERROR ("Invalid scheduling type");
      break;
    }
  return bsScheduler;
}

NetDeviceContainer WimaxHelper::Install (NodeContainer c,
                                         NetDeviceType deviceType,
                                         PhyType phyType,
                                         SchedulerType schedulerType,
                                         double frameDuration)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<WimaxPhy> phy = CreatePhy (phyType);

      // Set SuperFrame Duration
      phy->SetFrameDuration (Seconds (frameDuration));

      Ptr<WimaxNetDevice> device;
      Ptr<UplinkScheduler> uplinkScheduler = CreateUplinkScheduler (schedulerType);
      Ptr<BSScheduler> bsScheduler = CreateBSScheduler (schedulerType);

      if (deviceType == DEVICE_TYPE_BASE_STATION)
        {
          // attach phy
          Ptr<BaseStationNetDevice> deviceBS;
          deviceBS = CreateObject<BaseStationNetDevice> (node, phy, uplinkScheduler, bsScheduler);
          device = deviceBS;
          uplinkScheduler->SetBs (deviceBS);
          bsScheduler->SetBs (deviceBS);
        }
      else
        {
          device = CreateObject<SubscriberStationNetDevice> (node, phy);
        }
      device->SetAddress (Mac48Address::Allocate ());
      phy->SetDevice (device);
      device->Start ();
      device->Attach (m_channel); // attach channel

      node->AddDevice (device);

      devices.Add (device);
    }
  return devices;
}

NetDeviceContainer WimaxHelper::Install (NodeContainer c,
                                         NetDeviceType deviceType,
                                         PhyType phyType,
                                         SchedulerType schedulerType)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<WimaxPhy> phy = CreatePhy (phyType);
      Ptr<WimaxNetDevice> device;
      Ptr<UplinkScheduler> uplinkScheduler = CreateUplinkScheduler (schedulerType);
      Ptr<BSScheduler> bsScheduler = CreateBSScheduler (schedulerType);

      if (deviceType == DEVICE_TYPE_BASE_STATION)
        {
          // attach phy
          Ptr<BaseStationNetDevice> deviceBS;
          deviceBS = CreateObject<BaseStationNetDevice> (node, phy, uplinkScheduler, bsScheduler);
          device = deviceBS;
          uplinkScheduler->SetBs (deviceBS);
          bsScheduler->SetBs (deviceBS);
        }
      else
        {
          device = CreateObject<SubscriberStationNetDevice> (node, phy);
        }
      device->SetAddress (Mac48Address::Allocate ());
      phy->SetDevice (device);
      device->Start ();
      device->Attach (m_channel); // attach channel

      node->AddDevice (device);

      devices.Add (device);
    }
  return devices;
}

NetDeviceContainer WimaxHelper::Install (NodeContainer c,
                                         NetDeviceType deviceType,
                                         PhyType phyType,
                                         Ptr<WimaxChannel> channel,
                                         SchedulerType schedulerType)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;

      Ptr<WimaxPhy> phy = CreatePhyWithoutChannel (phyType, (char*) "dummy", 0);
      Ptr<WimaxNetDevice> device;
      Ptr<UplinkScheduler> uplinkScheduler = CreateUplinkScheduler (schedulerType);
      Ptr<BSScheduler> bsScheduler = CreateBSScheduler (schedulerType);

      if (deviceType == DEVICE_TYPE_BASE_STATION)
        {
          Ptr<BaseStationNetDevice> deviceBS;
          deviceBS = CreateObject<BaseStationNetDevice> (node, phy, uplinkScheduler, bsScheduler);
          device = deviceBS;
          uplinkScheduler->SetBs (deviceBS);
          bsScheduler->SetBs (deviceBS);
        }
      else
        {
          device = CreateObject<SubscriberStationNetDevice> (node, phy);
        }
      device->SetAddress (Mac48Address::Allocate ());
      phy->SetDevice (device);
      device->Start ();
      device->Attach (channel);

      node->AddDevice (device);
      devices.Add (device);
    }
  return devices;
}

Ptr<WimaxNetDevice> WimaxHelper::Install (Ptr<Node> node,
                                          NetDeviceType deviceType,
                                          PhyType phyType,
                                          Ptr<WimaxChannel> channel,
                                          SchedulerType schedulerType)
{

  // Ptr<WimaxPhy> phy = CreatePhyWithoutChannel (phyType);
  Ptr<WimaxPhy> phy = CreatePhyWithoutChannel (phyType, (char*) "dummy", 0);
  Ptr<WimaxNetDevice> device;
  Ptr<UplinkScheduler> uplinkScheduler = CreateUplinkScheduler (schedulerType);
  Ptr<BSScheduler> bsScheduler = CreateBSScheduler (schedulerType);

  if (deviceType == DEVICE_TYPE_BASE_STATION)
    {
      Ptr<BaseStationNetDevice> deviceBS;
      deviceBS = CreateObject<BaseStationNetDevice> (node, phy, uplinkScheduler, bsScheduler);
      device = deviceBS;
      uplinkScheduler->SetBs (deviceBS);
      bsScheduler->SetBs (deviceBS);
    }
  else
    {
      device = CreateObject<SubscriberStationNetDevice> (node, phy);
    }
  device->SetAddress (Mac48Address::Allocate ());
  phy->SetDevice (device);
  device->Start ();
  device->Attach (channel);

  node->AddDevice (device);

  return device;
}

void
WimaxHelper::EnableLogComponents (void)
{
  LogComponentEnable ("BandwidthManager", LOG_LEVEL_ALL);
  LogComponentEnable ("BSLinkManager", LOG_LEVEL_ALL);
  LogComponentEnable ("BaseStationNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("BSSchedulerRtps", LOG_LEVEL_ALL);
  LogComponentEnable ("BSSchedulerSimple", LOG_LEVEL_ALL);
  LogComponentEnable ("BSScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("BsServiceFlowManager", LOG_LEVEL_ALL);
  LogComponentEnable ("UplinkSchedulerMBQoS", LOG_LEVEL_ALL);
  LogComponentEnable ("UplinkSchedulerRtps", LOG_LEVEL_ALL);
  LogComponentEnable ("UplinkSchedulerSimple", LOG_LEVEL_ALL);
  LogComponentEnable ("UplinkScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("BurstProfileManager", LOG_LEVEL_ALL);
  LogComponentEnable ("ConnectionManager", LOG_LEVEL_ALL);
  LogComponentEnable ("IpcsClassifierRecord", LOG_LEVEL_ALL);
  LogComponentEnable ("IpcsClassifier", LOG_LEVEL_ALL);
  LogComponentEnable ("MACMESSAGES", LOG_LEVEL_ALL);
  LogComponentEnable ("PacketBurst", LOG_LEVEL_ALL);
  LogComponentEnable ("ServiceFlowManager", LOG_LEVEL_ALL);
  LogComponentEnable ("simpleOfdmWimaxChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("SimpleOfdmWimaxPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("SNRToBlockErrorRateManager", LOG_LEVEL_ALL);
  LogComponentEnable ("SSLinkManager", LOG_LEVEL_ALL);
  LogComponentEnable ("SSManager", LOG_LEVEL_ALL);
  LogComponentEnable ("SubscriberStationNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("SSScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("SsServiceFlowManager", LOG_LEVEL_ALL);
  LogComponentEnable ("WimaxChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("WimaxMacQueue", LOG_LEVEL_ALL);
  LogComponentEnable ("WimaxNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WimaxPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("Tlv", LOG_LEVEL_ALL);
  LogComponentEnable ("BandwidthManager", LOG_LEVEL_ALL);
  LogComponentEnable ("BaseStationNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("BSSchedulerRtps", LOG_LEVEL_ALL);
  LogComponentEnable ("BSSchedulerSimple", LOG_LEVEL_ALL);
  LogComponentEnable ("BSScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("SubscriberStationNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("SSScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("WimaxMacQueue", LOG_LEVEL_ALL);
}


void WimaxHelper::AsciiRxEvent (Ptr<OutputStreamWrapper> stream,
                                std::string path,
                                Ptr<const Packet> packet,
                                const Mac48Address &source)
{
  *stream->GetStream () << "r " << Simulator::Now ().GetSeconds () << " from: " << source << " ";
  *stream->GetStream () << path << std::endl;
}

void WimaxHelper::AsciiTxEvent (Ptr<OutputStreamWrapper> stream, std::string path, Ptr<const Packet> packet, const Mac48Address &dest)
{
  *stream->GetStream () << "t " << Simulator::Now ().GetSeconds () << " to: " << dest << " ";
  *stream->GetStream () << path << std::endl;
}

ServiceFlow WimaxHelper::CreateServiceFlow (ServiceFlow::Direction direction,
                                            ServiceFlow::SchedulingType schedulinType,
                                            IpcsClassifierRecord classifier)
{
  CsParameters csParam (CsParameters::ADD, classifier);
  ServiceFlow serviceFlow = ServiceFlow (direction);
  serviceFlow.SetConvergenceSublayerParam (csParam);
  serviceFlow.SetCsSpecification (ServiceFlow::IPV4);
  serviceFlow.SetServiceSchedulingType (schedulinType);
  serviceFlow.SetMaxSustainedTrafficRate (100);
  serviceFlow.SetMinReservedTrafficRate (1000000);
  serviceFlow.SetMinTolerableTrafficRate (1000000);
  serviceFlow.SetMaximumLatency (100);
  serviceFlow.SetMaxTrafficBurst (2000);
  serviceFlow.SetTrafficPriority (1);
  serviceFlow.SetUnsolicitedGrantInterval (1);
  serviceFlow.SetMaxSustainedTrafficRate (70);
  serviceFlow.SetToleratedJitter (10);
  serviceFlow.SetSduSize (49);
  serviceFlow.SetRequestTransmissionPolicy (0);
  return serviceFlow;
}

void
WimaxHelper::EnableAsciiInternal (Ptr<OutputStreamWrapper> stream,
                                  std::string prefix,
                                  Ptr<NetDevice> nd,
                                  bool explicitFilename)
{
  //
  // All of the ascii enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type CsmaNetDevice.
  //
  Ptr<WimaxNetDevice> device = nd->GetObject<WimaxNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("WimaxHelper::EnableAsciiInternal(): Device " << device << " not of type ns3::WimaxNetDevice");
      return;
    }

  //
  // Our default trace sinks are going to use packet printing, so we have to
  // make sure that is turned on.
  //
  Packet::EnablePrinting ();

  //
  // If we are not provided an OutputStreamWrapper, we are expected to create
  // one using the usual trace filename conventions and do a Hook*WithoutContext
  // since there will be one file per context and therefore the context would
  // be redundant.
  //
  if (stream == 0)
    {
      //
      // Set up an output stream object to deal with private ofstream copy
      // constructor and lifetime issues.  Let the helper decide the actual
      // name of the file given the prefix.
      //
      AsciiTraceHelper asciiTraceHelper;
      std::string filename;
      if (explicitFilename)
        {
          filename = prefix;
        }
      else
        {
          filename = asciiTraceHelper.GetFilenameFromDevice (prefix, device);
        }
      Ptr<OutputStreamWrapper> theStream = asciiTraceHelper.CreateFileStream (filename);

      uint32_t nodeid = nd->GetNode ()->GetId ();
      uint32_t deviceid = nd->GetIfIndex ();
      std::ostringstream oss;
      //
      // The MacRx trace source provides our "r" event.
      //

      oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WimaxNetDevice/Rx";
      Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiRxEvent, theStream));
      oss.str ("");
      oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WimaxNetDevice/Tx";
      Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiTxEvent, theStream));
      //
      // The "+", '-', and 'd' events are driven by trace sources actually in the
      // transmit queue.
      //

      EnableAsciiForConnection (theStream, nodeid, deviceid, (char*) "WimaxNetDevice", (char*) "InitialRangingConnection");
      EnableAsciiForConnection (theStream, nodeid, deviceid, (char*) "WimaxNetDevice", (char*) "BroadcastConnection");
      EnableAsciiForConnection (theStream, nodeid, deviceid, (char*) "SubscriberStationNetDevice", (char*) "BasicConnection");
      EnableAsciiForConnection (theStream, nodeid, deviceid, (char*) "SubscriberStationNetDevice", (char*) "PrimaryConnection");
      return;
    }

  //
  // If we are provided an OutputStreamWrapper, we are expected to use it, and
  // to providd a context.  We are free to come up with our own context if we
  // want, and use the AsciiTraceHelper Hook*WithContext functions, but for
  // compatibility and simplicity, we just use Config::Connect and let it deal
  // with the context.
  //
  // Note that we are going to use the default trace sinks provided by the
  // ascii trace helper.  There is actually no AsciiTraceHelper in sight here,
  // but the default trace sinks are actually publicly available static
  // functions that are always there waiting for just such a case.
  //
  uint32_t nodeid = nd->GetNode ()->GetId ();
  uint32_t deviceid = nd->GetIfIndex ();
  std::ostringstream oss;

  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WimaxNetDevice/Rx";
  Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiRxEvent, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WimaxNetDevice/Tx";
  Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiTxEvent, stream));

  EnableAsciiForConnection (stream, nodeid, deviceid, (char*) "WimaxNetDevice", (char*) "InitialRangingConnection");
  EnableAsciiForConnection (stream, nodeid, deviceid, (char*) "WimaxNetDevice", (char*) "BroadcastConnection");
  EnableAsciiForConnection (stream, nodeid, deviceid, (char*) "SubscriberStationNetDevice", (char*) "BasicConnection");
  EnableAsciiForConnection (stream, nodeid, deviceid, (char*) "SubscriberStationNetDevice", (char*) "PrimaryConnection");

}

static void PcapSniffTxRxEvent (Ptr<PcapFileWrapper> file,
                                Ptr<const PacketBurst> burst)
{
  std::list<Ptr<Packet> > packets = burst->GetPackets ();
  for (std::list<Ptr<Packet> >::iterator iter = packets.begin (); iter != packets.end (); ++iter)
    {
      Ptr<Packet> p = (*iter)->Copy ();
      WimaxMacToMacHeader m2m (p->GetSize ());
      p->AddHeader (m2m);
      file->Write (Simulator::Now (), p);
    }
}

void
WimaxHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool explicitFilename, bool promiscuous)
{
  //
  // All of the Pcap enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type WimaxNetDevice.
  //
  Ptr<WimaxNetDevice> device = nd->GetObject<WimaxNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("WimaxHelper::EnablePcapInternal(): Device " << &device << " not of type ns3::WimaxNetDevice");
      return;
    }

  Ptr<WimaxPhy> phy = device->GetPhy ();
  PcapHelper pcapHelper;
  std::string filename;
  if (explicitFilename)
    {
      filename = prefix;
    }
  else
    {
      filename = pcapHelper.GetFilenameFromDevice (prefix, device);
    }

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out, PcapHelper::DLT_EN10MB);

  phy->TraceConnectWithoutContext ("Tx", MakeBoundCallback (&PcapSniffTxRxEvent, file));
  phy->TraceConnectWithoutContext ("Rx", MakeBoundCallback (&PcapSniffTxRxEvent, file));
}

} // namespace ns3
