#include "csma-helper.h"
#include "ns3/object-factory.h"
#include "ns3/queue.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-channel.h"
#include <string>

namespace ns3 {

CsmaHelper::CsmaHelper ()
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
      node->AddDevice (device);
      Ptr<Queue> queue = m_queueFactory.Create<Queue> ();
      device->AddQueue (queue);
      device->Attach (channel);
      container.Add (device);
    }
  return container;
}


} // namespace ns3
