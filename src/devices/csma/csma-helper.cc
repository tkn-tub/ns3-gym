#include "csma-helper.h"

namespace ns3 {

CsmaHelper::CsmaHelper ()
{
  m_queueFactory.SetTypeId ("DropTailQueue");
  m_deviceFactory.SetTypeId ("CsmaNetDevice");
  m_deviceChannel.SetTypeId ("CsmaChannel");
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

  /**
   * Set these parameters on each PointToPointNetDevice created
   * by this helper.
   */
void 
CsmaHelper::SetDeviceParameter (std::string n1, Attribute v1)
{
  m_deviceFactory.Set (n1, v1);
}

void 
CsmaHelper::SetChannelParameter (std::string n1, Attribute v1)
{
  m_csmaFactory.Set (n1, v1);
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
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      
    }
}


} // namespace ns3
