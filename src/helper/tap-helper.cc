#include "tap-helper.h"
#include "ns3/tap-net-device.h"
#include "ns3/host-tap-net-device.h"
#include "ns3/tap-channel.h"
#include "ns3/pcap-writer.h"
#include "ns3/config.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TapHelper");

NetDeviceContainer
TapHelper::Install (NodeContainer hosts, NodeContainer taps)
{
  NS_ASSERT (hosts.GetN () == 1 && taps.GetN () == 1);
  NetDeviceContainer devices;

  Ptr<TapChannel> channel = CreateObject<TapChannel> ();

  // the host.
  Ptr<Node> host = hosts.Get (0);
  Ptr<HostTapNetDevice> hostDev = CreateObject<HostTapNetDevice> ();
  hostDev->SetChannel (channel);
  hostDev->SetAddress (Mac48Address::Allocate ());
  host->AddDevice (hostDev);
  devices.Add (hostDev);

  // the tap
  Ptr<Node> tap = taps.Get (0);
  Ptr<TapNetDevice> tapDev = CreateObject<TapNetDevice> ();
  tapDev->SetChannel (channel);
  tapDev->SetAddress (Mac48Address::Allocate ());
  tap->AddDevice (tapDev);
  devices.Add (tapDev);

  return devices;
}

void 
TapHelper::SetupHost (NetDeviceContainer dev, Ipv4Address ad, Ipv4Mask mask, Ipv4Address gateway)
{
  NS_ASSERT (dev.GetN () == 2);
  Ptr<TapNetDevice> tap = dev.Get (1)->GetObject<TapNetDevice> ();
  tap->SetupHost (ad, mask, gateway);
}

static void TxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (writer << packet << from << to);
  writer->WritePacket (packet);
}
static void RxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (writer << packet << from << to);
  writer->WritePacket (packet);
}


void 
TapHelper::EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid)
{
  std::ostringstream oss;
  oss << filename << "-" << nodeid << "-" << deviceid << ".pcap";
  Ptr<PcapWriter> pcap = Create<PcapWriter> ();
  pcap->Open (oss.str ());
  pcap->WriteEthernetHeader ();
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::TapNetDevice/Rx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&RxEvent, pcap));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::TapNetDevice/Tx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&TxEvent, pcap));
}

void 
TapHelper::EnablePcap (std::string filename, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnablePcap (filename, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}
void
TapHelper::EnablePcap (std::string filename, NodeContainer n)
{
  NetDeviceContainer devs;
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      for (uint32_t j = 0; j < node->GetNDevices (); ++j)
	{
	  devs.Add (node->GetDevice (j));
	}
    }
  EnablePcap (filename, devs);
}

void
TapHelper::EnablePcapAll (std::string filename)
{
  EnablePcap (filename, NodeContainer::GetGlobal ());
}


} // namespace ns3
