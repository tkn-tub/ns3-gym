#include "point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/queue.h"
#include "ns3/pcap-writer.h"
#include "ns3/config.h"
#include "ns3/packet.h"


namespace ns3 {


PointToPointHelper::PointToPointHelper ()
{
  m_queueFactory.SetTypeId ("ns3::DropTailQueue");
  m_deviceFactory.SetTypeId ("ns3::PointToPointNetDevice");
  m_channelFactory.SetTypeId ("ns3::PointToPointChannel");
}

void 
PointToPointHelper::SetQueue (std::string type,
			      std::string n1, Attribute v1,
			      std::string n2, Attribute v2,
			      std::string n3, Attribute v3,
			      std::string n4, Attribute v4)
{
  m_queueFactory.SetTypeId (type);
  m_queueFactory.Set (n1, v1);
  m_queueFactory.Set (n2, v2);
  m_queueFactory.Set (n3, v3);
  m_queueFactory.Set (n4, v4);
}

void 
PointToPointHelper::SetDeviceParameter (std::string n1, Attribute v1)
{
  m_deviceFactory.Set (n1, v1);
}

void 
PointToPointHelper::SetChannelParameter (std::string n1, Attribute v1)
{
  m_channelFactory.Set (n1, v1);
}


void 
PointToPointHelper::EnablePcap (std::string filename)
{
  m_pcap = true;
  m_pcapFilename = filename;
}
void 
PointToPointHelper::DisablePcap (void)
{
  m_pcap = false;
}

void 
PointToPointHelper::EnableAscii (std::ostream &os)
{
  m_ascii = true;
  m_asciiOs = &os;
}
void 
PointToPointHelper::DisableAscii (void)
{
  m_ascii = false;
}

void
PointToPointHelper::EnablePcap (Ptr<Node> node, Ptr<NetDevice> device, Ptr<Queue> queue)
{
  std::ostringstream oss;
  oss << m_pcapFilename << "-" << node->GetId () << "-" << device->GetIfIndex ();
  std::string filename = oss.str ();
  Ptr<PcapWriter> pcap = Create<PcapWriter> ();
  pcap->Open (filename);
  pcap->WriteEthernetHeader ();
  device->TraceConnectWithoutContext ("Rx", MakeBoundCallback (&PointToPointHelper::RxEvent, pcap));
  queue->TraceConnectWithoutContext ("Enqueue", MakeBoundCallback (&PointToPointHelper::EnqueueEvent, pcap));
}

void
PointToPointHelper::EnableAscii (Ptr<Node> node, Ptr<NetDevice> device)
{
  Packet::EnableMetadata ();
  std::ostringstream oss;
  oss << "/NodeList/" << node->GetId () << "/DeviceList/" << device->GetIfIndex () << "/Rx";
  Config::Connect (oss.str (), MakeBoundCallback (&PointToPointHelper::AsciiEvent, m_asciiOs));
  oss.str ("");
  oss << "/NodeList/" << node->GetId () << "/DeviceList/" << device->GetIfIndex () << "/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&PointToPointHelper::AsciiEvent, m_asciiOs));
  oss.str ("");
  oss << "/NodeList/" << node->GetId () << "/DeviceList/" << device->GetIfIndex () << "/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&PointToPointHelper::AsciiEvent, m_asciiOs));
}

NetDeviceContainer 
PointToPointHelper::Build (NodeContainer c)
{
  NS_ASSERT (c.GetN () == 2);
  return Build (c.Get (0), c.Get (1));
}
NetDeviceContainer 
PointToPointHelper::Build (Ptr<Node> a, Ptr<Node> b)
{
  NetDeviceContainer container;

  Ptr<PointToPointNetDevice> devA = m_deviceFactory.Create<PointToPointNetDevice> ();
  devA->SetAddress (Mac48Address::Allocate ());
  a->AddDevice (devA);
  Ptr<Queue> queueA = m_queueFactory.Create<Queue> ();
  devA->AddQueue (queueA);
  Ptr<PointToPointNetDevice> devB = m_deviceFactory.Create<PointToPointNetDevice> ();
  devB->SetAddress (Mac48Address::Allocate ());
  b->AddDevice (devB);
  Ptr<Queue> queueB = m_queueFactory.Create<Queue> ();
  devB->AddQueue (queueB);
  Ptr<PointToPointChannel> channel = m_channelFactory.Create<PointToPointChannel> ();
  devA->Attach (channel);
  devB->Attach (channel);
  if (m_pcap)
    {
      EnablePcap (a, devA, queueA);
      EnablePcap (b, devB, queueB);
    }
  if (m_ascii)
    {
      EnableAscii (a, devA);
      EnableAscii (b, devB);
    }

  container.Add (devA);
  container.Add (devB);

  return container;
}

void 
PointToPointHelper::EnqueueEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}
void 
PointToPointHelper::RxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}
void 
PointToPointHelper::AsciiEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << path << " " << *packet << std::endl;
}


} // namespace ns3
