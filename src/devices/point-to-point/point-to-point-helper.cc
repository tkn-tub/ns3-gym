#include "point-to-point-helper.h"
#include "point-to-point-net-device.h"
#include "point-to-point-channel.h"
#include "ns3/queue.h"

namespace ns3 {


PointToPointHelper::PointToPointHelper ()
{}

void 
PointToPointHelper::SetQueue (std::string type,
			      std::string n1, PValue v1,
			      std::string n2, PValue v2,
			      std::string n3, PValue v3,
			      std::string n4, PValue v4)
{
  m_queueFactory.SetTypeId (type);
  m_queueFactory.Set (n1, v1);
  m_queueFactory.Set (n2, v2);
  m_queueFactory.Set (n3, v3);
  m_queueFactory.Set (n4, v4);
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

  Ptr<PointToPointNetDevice> devA = CreateObject<PointToPointNetDevice> (a);
  Ptr<Queue> queueA = m_queueFactory.Create ()->GetObject<Queue> ();
  devA->AddQueue (queueA);
  Ptr<PointToPointNetDevice> devB = CreateObject<PointToPointNetDevice> (b);
  Ptr<Queue> queueB = m_queueFactory.Create ()->GetObject<Queue> ();
  devB->AddQueue (queueB);
  Ptr<PointToPointChannel> channel = CreateObject<PointToPointChannel> ();
  devA->Attach (channel);
  devB->Attach (channel);
  container.Add (devA);
  container.Add (devB);

  return container;
}


} // namespace ns3
