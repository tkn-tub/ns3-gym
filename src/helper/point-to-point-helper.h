#ifndef POINT_TO_POINT_HELPER_H
#define POINT_TO_POINT_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include <string>

namespace ns3 {

class Queue;
class NetDevice;
class Node;
class PcapWriter;

/**
 * \brief build a set of PointToPointNetDevice objects
 */
class PointToPointHelper
{
public:
  // by default, create queues of type DropTailQueue.
  PointToPointHelper ();

  /**
   * \param type the type of queue
   * \param n1 the name of the attribute to set on the queue
   * \param v1 the value of the attribute to set on the queue
   * \param n2 the name of the attribute to set on the queue
   * \param v2 the value of the attribute to set on the queue
   * \param n3 the name of the attribute to set on the queue
   * \param v3 the value of the attribute to set on the queue
   * \param n4 the name of the attribute to set on the queue
   * \param v4 the value of the attribute to set on the queue
   *
   * Set the type of queue to create and associated to each
   * PointToPointNetDevice created through PointToPointHelper::Build.
   */
  void SetQueue (std::string type,
		 std::string n1 = "", Attribute v1 = Attribute (),
		 std::string n2 = "", Attribute v2 = Attribute (),
		 std::string n3 = "", Attribute v3 = Attribute (),
		 std::string n4 = "", Attribute v4 = Attribute ());

  /**
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set
   *
   * Set these parameters on each ns3::PointToPointNetDevice created
   * by PointToPointHelper::Build
   */
  void SetDeviceParameter (std::string name, Attribute value);
  /**
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set
   *
   * Set these parameters on each ns3::PointToPointChannel created
   * by PointToPointHelper::Build
   */
  void SetChannelParameter (std::string name, Attribute value);

  /**
   * \param filename file template to dump pcap traces in.
   *
   * Every ns3::PointToPointNetDevice created through subsequent calls
   * to PointToPointHelper::Build will be configured to dump
   * pcap output in a file named filename-nodeid-deviceid.
   */
  void EnablePcap (std::string filename);
  /**
   * Every ns3::PointToPointNetDevice created through subsequent calls
   * to PointToPointHelper::Build will be configured to not dump any pcap
   * output.
   */
  void DisablePcap (void);

  /**
   * \param os an output stream where ascii trace should be sent.
   *
   * Every ns3::PointToPointNetDevice created through subsequent calls
   * to PointToPointHelper::Build will be configured to dump Rx, EnQueue
   * and Dequeue events as ascii data in the specified output stream.
   */
  void EnableAscii (std::ostream &os);
  /**
   * Every ns3::PointToPointNetDevice created through subsequent calls
   * to PointToPointHelper::Build will be configured to not dump any
   * ascii output.
   */
  void DisableAscii (void);

  /**
   * \param c a set of nodes
   *
   * This method creates a ns3::PointToPointChannel with the
   * attributes configured by PointToPointHelper::SetChannelParameter,
   * then, for each node in the input container, we create a 
   * ns3::PointToPointNetDevice with the requested parameters, 
   * a queue for this ns3::NetDevice, and associate the resulting 
   * ns3::NetDevice with the ns3::Node and ns3::PointToPointChannel.
   */
  NetDeviceContainer Build (NodeContainer c);
  /**
   * \param a first node
   * \param b second node
   *
   * Saves you from having to construct a temporary NodeContainer.
   */
  NetDeviceContainer Build (Ptr<Node> a, Ptr<Node> b);

private:
  void EnablePcap (Ptr<Node> node, Ptr<NetDevice> device, Ptr<Queue> queue);
  void EnableAscii (Ptr<Node> node, Ptr<NetDevice> device);
  static void RxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet);
  static void EnqueueEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet);
  static void AsciiEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  ObjectFactory m_queueFactory;
  ObjectFactory m_channelFactory;
  ObjectFactory m_deviceFactory;
  bool m_pcap;
  std::string m_pcapFilename;
  bool m_ascii;
  std::ostream *m_asciiOs;
};


} // namespace ns3

#endif /* POINT_TO_POINT_HELPER_H */
