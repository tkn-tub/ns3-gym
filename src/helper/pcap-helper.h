/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#ifndef PCAP_HELPER_H
#define PCAP_HELPER_H

#include "ns3/assert.h"
#include "ns3/net-device-container.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv6-interface-container.h"
#include "ns3/node-container.h"
#include "ns3/simulator.h"
#include "ns3/pcap-file-object.h"
#include "ns3/ipv4.h"
#include "ns3/ipv6.h"

namespace ns3 {

/**
 * \brief Manage pcap files for device models
 *
 * Handling pcap files is a common operation for ns-3 devices.  It is useful to
 * provide a common base class for dealing with these ops.
 */

class PcapHelper
{
public:
  //
  // These are the data link types that will be written to the pcap file.  We
  // don't include pcap-bpf.h to avoid an explicit dependency on the real pcap
  // and we don't make an enumeration of all of the values to make it easy to
  // pass new values in.
  //
  enum {DLT_NULL = 0};
  enum {DLT_EN10MB = 1};
  enum {DLT_PPP = 9};
  enum {DLT_RAW = 101};
  enum {DLT_IEEE802_11 = 105};
  enum {DLT_PRISM_HEADER = 119};
  enum {DLT_IEEE802_11_RADIO = 127};

  /**
   * @brief Create a pcap helper.
   */
  PcapHelper ();

  /**
   * @brief Destroy a pcap helper.
   */
  ~PcapHelper ();

  /**
   * @brief Let the pcap helper figure out a reasonable filename to use for a
   * pcap file associated with a device.
   */
  std::string GetFilenameFromDevice (std::string prefix, Ptr<NetDevice> device, bool useObjectNames = true);

  /**
   * @brief Let the pcap helper figure out a reasonable filename to use for the
   * pcap file associated with a node.
   */
  std::string GetFilenameFromInterfacePair (std::string prefix, Ptr<Object> object, 
                                            uint32_t interface, bool useObjectNames = true);

  /**
   * @brief Create and initialize a pcap file.
   */
  Ptr<PcapFileObject> CreateFile (std::string filename, std::string filemode,
                                  uint32_t dataLinkType,  uint32_t snapLen = 65535, int32_t tzCorrection = 0);
  /**
   * @brief Hook a trace source to the default trace sink
   */
  template <typename T> void HookDefaultSink (Ptr<T> object, std::string traceName, Ptr<PcapFileObject> file);

private:
  static void DefaultSink (Ptr<PcapFileObject> file, Ptr<const Packet> p);
};

template <typename T> void
PcapHelper::HookDefaultSink (Ptr<T> object, std::string tracename, Ptr<PcapFileObject> file)
{
  bool result = object->TraceConnectWithoutContext (tracename.c_str (), MakeBoundCallback (&DefaultSink, file));
  NS_ASSERT_MSG (result == true, "PcapHelper::HookDefaultSink():  Unable to hook \"" << tracename << "\"");
}

/**
 * \brief Base class providing common user-level pcap operations for helpers
 * representing net devices.
 */
class PcapUserHelperForDevice
{
public:
  /**
   * @brief Enable pcap output the indicated net device.
   * @internal
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param nd Net device for which you want to enable tracing.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  virtual void EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous) = 0;

  /**
   * @brief Enable pcap output the indicated net device.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param nd Net device for which you want to enable tracing.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, Ptr<NetDevice> nd, bool promiscuous = false);

  /**
   * @brief Enable pcap output the indicated net device using a device previously
   * named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for pcap files.
   * @param ndName The name of the net device in which you want to enable tracing.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, std::string ndName, bool promiscuous = false);

  /**
   * @brief Enable pcap output on each device in the container which is of the 
   * appropriate type.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param d container of devices of type ns3::CsmaNetDevice
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, NetDeviceContainer d, bool promiscuous = false);

  /**
   * @brief Enable pcap output on each device (which is of the appropriate type)
   * in the nodes provided in the container.
   *
   * \param prefix Filename prefix to use for pcap files.
   * \param n container of nodes.
   * \param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, NodeContainer n, bool promiscuous = false);

  /**
   * @brief Enable pcap output on the device specified by a global node-id (of
   * a previously created node) and associated device-id.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcap (std::string prefix, uint32_t nodeid, uint32_t deviceid, bool promiscuous = false);

  /**
   * @brief Enable pcap output on each device (which is of the appropriate type)
   * in the set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param promiscuous If true capture all possible packets available at the device.
   */
  void EnablePcapAll (std::string prefix, bool promiscuous = false);
};

/**
 * \brief Base class providing common user-level pcap operations for helpers
 * representing IPv4 protocols .
 */
class PcapUserHelperForIpv4
{
public:
  /**
   * @brief Enable pcap output the indicated Ipv4 and interface pair.
   * @internal
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface Interface on ipv4 on which you want to enable tracing.
   */
  virtual void EnablePcapIpv4Internal (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface) = 0;

  /**
   * @brief Enable pcap output the indicated Ipv4 and interface pair.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface Interface on ipv4 on which you want to enable tracing.
   */
  void EnablePcapIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface);

  /**
   * @brief Enable pcap output the indicated Ipv4 and interface pair using a
   * Ptr<Ipv4> previously named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for pcap files.
   * @param ipv4Name Name of the Ptr<Ipv4> on which you want to enable tracing.
   * @param interface Interface on ipv4 on which you want to enable tracing.
   */
  void EnablePcapIpv4 (std::string prefix, std::string ipv4Name, uint32_t interface);

  /**
   * @brief Enable pcap output on each Ipv4 and interface pair in the container.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param c Ipv4InterfaceContainer of Ipv4 and interface pairs
   */
  void EnablePcapIpv4 (std::string prefix, Ipv4InterfaceContainer c);

  /**
   * @brief Enable pcap output on all Ipv4 and interface pairs existing in the
   * nodes provided in the container.
   *
   * \param prefix Filename prefix to use for pcap files.
   * \param n container of nodes.
   */
  void EnablePcapIpv4 (std::string prefix, NodeContainer n);

  /**
   * @brief Enable pcap output on the Ipv4 and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ipv4 aggregated to a node, the node-id unambiguously 
   * determines the Ipv4.
   *
   * @param prefix Filename prefix to use for pcap files.
   */
  void EnablePcapIpv4 (std::string prefix, uint32_t nodeid, uint32_t interface);

  /**
   * @brief Enable pcap output on all Ipv4 and interface pairs existing in the 
   * set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for pcap files.
   */
  void EnablePcapIpv4All (std::string prefix);

};

/**
 * \brief Base class providing common user-level pcap operations for helpers
 * representing IPv6 protocols .
 */
class PcapUserHelperForIpv6
{
public:
  /**
   * @brief Enable pcap output the indicated Ipv6 and interface pair.
   * @internal
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param ipv6 Ptr<Ipv6> on which you want to enable tracing.
   * @param interface Interface on ipv6 on which you want to enable tracing.
   */
  virtual void EnablePcapIpv6Internal (std::string prefix, Ptr<Ipv6> ipv6, uint32_t interface) = 0;

  /**
   * @brief Enable pcap output the indicated Ipv6 and interface pair.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param ipv6 Ptr<Ipv6> on which you want to enable tracing.
   * @param interface Interface on ipv6 on which you want to enable tracing.
   */
  void EnablePcapIpv6 (std::string prefix, Ptr<Ipv6> ipv6, uint32_t interface);

  /**
   * @brief Enable pcap output the indicated Ipv6 and interface pair using a
   * Ptr<Ipv6> previously named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for pcap files.
   * @param ipv6Name Name of the Ptr<Ipv6> on which you want to enable tracing.
   * @param interface Interface on ipv6 on which you want to enable tracing.
   */
  void EnablePcapIpv6 (std::string prefix, std::string ipv6Name, uint32_t interface);

  /**
   * @brief Enable pcap output on each Ipv6 and interface pair in the container.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param c Ipv6InterfaceContainer of Ipv6 and interface pairs
   */
  void EnablePcapIpv6 (std::string prefix, Ipv6InterfaceContainer c);

  /**
   * @brief Enable pcap output on all Ipv6 and interface pairs existing in the
   * nodes provided in the container.
   *
   * \param prefix Filename prefix to use for pcap files.
   * \param n container of nodes.
   */
  void EnablePcapIpv6 (std::string prefix, NodeContainer n);

  /**
   * @brief Enable pcap output on the Ipv6 and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ipv6 aggregated to a node, the node-id unambiguously 
   * determines the Ipv6.
   *
   * @param prefix Filename prefix to use for pcap files.
   */
  void EnablePcapIpv6 (std::string prefix, uint32_t nodeid, uint32_t interface);

  /**
   * @brief Enable pcap output on all Ipv6 and interface pairs existing in the 
   * set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for pcap files.
   */
  void EnablePcapIpv6All (std::string prefix);
};

} // namespace ns3

#endif /* PCAP_HELPER_H */
