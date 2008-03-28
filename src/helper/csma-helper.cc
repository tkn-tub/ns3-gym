#include "csma-helper.h"
#include "ns3/object-factory.h"
#include "ns3/queue.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-channel.h"
#include "ns3/pcap-writer.h"
#include "ns3/config.h"
#include "ns3/packet.h"
#include <string>

namespace ns3 {

CsmaHelper::CsmaHelper ()
  : m_pcap (false), 
    m_ascii (false)
{
  m_queueFactory.SetTypeId ("ns3::DropTailQueue");
  m_deviceFactory.SetTypeId ("ns3::CsmaNetDevice");
  m_channelFactory.SetTypeId ("ns3::CsmaChannel");
}

void 
CsmaHelper::SetQueue (std::string type,
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
CsmaHelper::SetDeviceParameter (std::string n1, Attribute v1)
{
  m_deviceFactory.Set (n1, v1);
}

void 
CsmaHelper::SetChannelParameter (std::string n1, Attribute v1)
{
  m_channelFactory.Set (n1, v1);
}

void 
CsmaHelper::EnablePcap (std::string filename)
{
  m_pcap = true;
  m_pcapFilename = filename;
}
void 
CsmaHelper::DisablePcap (void)
{
  m_pcap = false;
}

void 
CsmaHelper::EnableAscii (std::ostream &os)
{
  m_ascii = true;
  m_asciiOs = &os;
}
void 
CsmaHelper::DisableAscii (void)
{
  m_ascii = false;
}


NetDeviceContainer 
CsmaHelper::Build (const NodeContainer &c)
{
  Ptr<CsmaChannel> channel = m_channelFactory.Create ()->GetObject<CsmaChannel> ();
  return Build (c, channel);
}

NetDeviceContainer 
CsmaHelper::Build (const NodeContainer &c, Ptr<CsmaChannel> channel)
{
  NetDeviceContainer container;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<CsmaNetDevice> device = m_deviceFactory.Create<CsmaNetDevice> ();
      device->SetAddress (Mac48Address::Allocate ());
      node->AddDevice (device);
      Ptr<Queue> queue = m_queueFactory.Create<Queue> ();
      device->AddQueue (queue);
      device->Attach (channel);
      if (m_pcap)
	{
	  std::ostringstream oss;
	  oss << m_pcapFilename << "-" << node->GetId () << "-" << device->GetIfIndex ();
	  std::string filename = oss.str ();
	  Ptr<PcapWriter> pcap = Create<PcapWriter> ();
	  pcap->Open (filename);
	  pcap->WriteEthernetHeader ();
	  device->TraceConnectWithoutContext ("Rx", MakeBoundCallback (&CsmaHelper::RxEvent, pcap));
	  queue->TraceConnectWithoutContext ("Enqueue", MakeBoundCallback (&CsmaHelper::EnqueueEvent, pcap));
	}
      if (m_ascii)
	{
	  Packet::EnableMetadata ();
	  std::ostringstream oss;
	  oss << "/NodeList/" << node->GetId () << "/DeviceList/" << device->GetIfIndex () << "/Rx";
	  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiEvent, m_asciiOs));
	  oss.str ("");
	  oss << "/NodeList/" << node->GetId () << "/DeviceList/" << device->GetIfIndex () << "/TxQueue/Enqueue";
	  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiEvent, m_asciiOs));
	  oss.str ("");
	  oss << "/NodeList/" << node->GetId () << "/DeviceList/" << device->GetIfIndex () << "/TxQueue/Dequeue";
	  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiEvent, m_asciiOs));
	  
	}
      container.Add (device);
    }
  return container;
}

void 
CsmaHelper::EnqueueEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}
void 
CsmaHelper::RxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}
void 
CsmaHelper::AsciiEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << path << " " << *packet << std::endl;
}


} // namespace ns3
