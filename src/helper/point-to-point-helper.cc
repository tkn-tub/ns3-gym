#include "point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/queue.h"

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

  Ptr<PointToPointNetDevice> devA = CreateObject<PointToPointNetDevice> ();
  devA->SetAddress (Mac48Address::Allocate ());
  a->AddDevice (devA);
  Ptr<Queue> queueA = m_queueFactory.Create<Queue> ();
  devA->AddQueue (queueA);
  Ptr<PointToPointNetDevice> devB = CreateObject<PointToPointNetDevice> ();
  devB->SetAddress (Mac48Address::Allocate ());
  b->AddDevice (devB);
  Ptr<Queue> queueB = m_queueFactory.Create<Queue> ();
  devB->AddQueue (queueB);
  Ptr<PointToPointChannel> channel = CreateObject<PointToPointChannel> ();
  devA->Attach (channel);
  devB->Attach (channel);
  container.Add (devA);
  container.Add (devB);

  return container;
}


} // namespace ns3
