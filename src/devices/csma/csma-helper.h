#ifndef CSMA_HELPER_H
#define CSMA_HELPER_H

namespace ns3 {

class CsmaHelper
{
public:
  CsmaHelper ();

  void SetQueue (std::string type,
		 std::string n1 = "", PValue v1 = PValue (),
		 std::string n2 = "", PValue v2 = PValue (),
		 std::string n3 = "", PValue v3 = PValue (),
		 std::string n4 = "", PValue v4 = PValue ());

  /**
   * Set these parameters on each PointToPointNetDevice created
   * by this helper.
   */
  void SetDeviceParameter (std::string n1, PValue v1);

  void SetChannelParameter (std::string n1, PValue v1);

  NetDeviceContainer Build (const NodeContainer &c);

  NetDeviceContainer Build (const NodeContainer &c, Ptr<CsmaChannel> channel);

private:
  ObjectFactory m_queueFactory;
  ObjectFactory m_deviceFactory;
  ObjectFactory m_channelFactory;
};


} // namespace ns3

#endif /* CSMA_HELPER_H */
