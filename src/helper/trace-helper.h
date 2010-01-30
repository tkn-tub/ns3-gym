/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Washington
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

#ifndef TRACE_HELPER_H
#define TRACE_HELPER_H

#include "ns3/assert.h"
#include "ns3/net-device-container.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv6-interface-container.h"
#include "ns3/node-container.h"
#include "ns3/simulator.h"
#include "ns3/pcap-file-object.h"
#include "ns3/output-stream-object.h"
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
  bool __attribute__ ((unused)) result = 
    object->TraceConnectWithoutContext (tracename.c_str (), MakeBoundCallback (&DefaultSink, file));
  NS_ASSERT_MSG (result == true, "PcapHelper::HookDefaultSink():  Unable to hook \"" << tracename << "\"");
}

/**
 * \brief Manage ASCII trace files for device models
 *
 * Handling ascii trace files is a common operation for ns-3 devices.  It is 
 * useful to provide a common base class for dealing with these ops.
 */

class AsciiTraceHelper
{
public:
  /**
   * @brief Create an ascii trace helper.
   */
  AsciiTraceHelper ();

  /**
   * @brief Destroy an ascii trace helper.
   */
  ~AsciiTraceHelper ();

  /**
   * @brief Let the ascii trace helper figure out a reasonable filename to use
   * for an ascii trace file associated with a device.
   */
  std::string GetFilenameFromDevice (std::string prefix, Ptr<NetDevice> device, bool useObjectNames = true);

  /**
   * @brief Let the ascii trace helper figure out a reasonable filename to use
   * for an ascii trace file associated with a node.
   */
  std::string GetFilenameFromInterfacePair (std::string prefix, Ptr<Object> object, 
                                            uint32_t interface, bool useObjectNames = true);

  /**
   * @brief Create and initialize an output stream object we'll use to write the 
   * traced bits.
   *
   * One of the common issues users run into when trying to use tracing in ns-3
   * is actually a design decision made in the C++ stream library.  It is not 
   * widely known that copy and assignment of iostreams is forbidden by 
   * std::basic_ios<>.  This is because it is not possible to predict the 
   * semantics of the stream desired by a user.
   *
   * The tempting ns-3 idiom when tracing to a file is to create a bound callback
   * with an ofstream as the bound object.  Unfortunately, this implies a copy 
   * construction in order to get the ofstream object into the callback.  This 
   * operation, as mentioned above, is forbidden by the STL.  You could use a 
   * global ostream and pass a pointer to it, but that is pretty ugly.  You 
   * could create an ostream on the stack and pass a pointer to it, but you may
   * run into object lifetime issues.  Ns-3 has a nice reference counted object
   * that can solve the problem so we use one of those to carry the stream
   * around and deal with the lifetime issues.
   */
  Ptr<OutputStreamObject> CreateFileStream (std::string filename, std::string filemode = "w");

  /**
   * @brief Hook a trace source to the default enqueue operation trace sink that
   * does not accept nor log a trace context.
   */
  template <typename T> 
  void HookDefaultEnqueueSinkWithoutContext (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default enqueue operation trace sink that
   * does accept and log a trace context.
   */
  template <typename T> 
  void HookDefaultEnqueueSinkWithContext (Ptr<T> object, 
                                          std::string context, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default drop operation trace sink that 
   * does not accept nor log a trace context.
   */
  template <typename T> 
  void HookDefaultDropSinkWithoutContext (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default drop operation trace sink that 
   * does accept and log a trace context.
   */
  template <typename T> 
  void HookDefaultDropSinkWithContext (Ptr<T> object, 
                                       std::string context, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default dequeue operation trace sink
   * that does not accept nor log a trace context.
   */
  template <typename T> 
  void HookDefaultDequeueSinkWithoutContext (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default dequeue operation trace sink
   * that does accept and log a trace context.
   */
  template <typename T> 
  void HookDefaultDequeueSinkWithContext (Ptr<T> object, 
                                          std::string context, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default receive operation trace sink
   * that does not accept nor log a trace context.
   */
  template <typename T> 
  void HookDefaultReceiveSinkWithoutContext (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default receive operation trace sink
   * that does accept and log a trace context.
   */
  template <typename T> 
  void HookDefaultReceiveSinkWithContext (Ptr<T> object, 
                                          std::string context, std::string traceName, Ptr<OutputStreamObject> stream);

  static void DefaultEnqueueSinkWithoutContext (Ptr<OutputStreamObject> file, Ptr<const Packet> p);
  static void DefaultEnqueueSinkWithContext (Ptr<OutputStreamObject> file, std::string context, Ptr<const Packet> p);

  static void DefaultDropSinkWithoutContext (Ptr<OutputStreamObject> file, Ptr<const Packet> p);
  static void DefaultDropSinkWithContext (Ptr<OutputStreamObject> file, std::string context, Ptr<const Packet> p);

  static void DefaultDequeueSinkWithoutContext (Ptr<OutputStreamObject> file, Ptr<const Packet> p);
  static void DefaultDequeueSinkWithContext (Ptr<OutputStreamObject> file, std::string context, Ptr<const Packet> p);

  static void DefaultReceiveSinkWithoutContext (Ptr<OutputStreamObject> file, Ptr<const Packet> p);
  static void DefaultReceiveSinkWithContext (Ptr<OutputStreamObject> file, std::string context, Ptr<const Packet> p);
};

template <typename T> void
AsciiTraceHelper::HookDefaultEnqueueSinkWithoutContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  bool __attribute__ ((unused)) result = 
    object->TraceConnectWithoutContext (tracename, MakeBoundCallback (&DefaultEnqueueSinkWithoutContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultEnqueueSinkWithoutContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultEnqueueSinkWithContext (
  Ptr<T> object, 
  std::string context, 
  std::string tracename, 
  Ptr<OutputStreamObject> stream)
{
  bool __attribute__ ((unused)) result = 
    object->TraceConnect (tracename, context, MakeBoundCallback (&DefaultEnqueueSinkWithContext, stream));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultEnqueueSinkWithContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultDropSinkWithoutContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  bool __attribute__ ((unused)) result = 
    object->TraceConnectWithoutContext (tracename, MakeBoundCallback (&DefaultDropSinkWithoutContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultDropSinkWithoutContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultDropSinkWithContext (
  Ptr<T> object, 
  std::string context,
  std::string tracename, 
  Ptr<OutputStreamObject> stream)
{
  bool __attribute__ ((unused)) result = 
    object->TraceConnect (tracename, context, MakeBoundCallback (&DefaultDropSinkWithContext, stream));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultDropSinkWithContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultDequeueSinkWithoutContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  bool __attribute__ ((unused)) result = 
    object->TraceConnectWithoutContext (tracename, MakeBoundCallback (&DefaultDequeueSinkWithoutContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultDequeueSinkWithoutContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultDequeueSinkWithContext (
  Ptr<T> object, 
  std::string context,
  std::string tracename, 
  Ptr<OutputStreamObject> stream)
{
  bool __attribute__ ((unused)) result = 
    object->TraceConnect (tracename, context, MakeBoundCallback (&DefaultDequeueSinkWithContext, stream));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultDequeueSinkWithContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultReceiveSinkWithoutContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  bool __attribute__ ((unused)) result = 
    object->TraceConnectWithoutContext (tracename, MakeBoundCallback (&DefaultReceiveSinkWithoutContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultReceiveSinkWithoutContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultReceiveSinkWithContext (
  Ptr<T> object, 
  std::string context,
  std::string tracename, 
  Ptr<OutputStreamObject> stream)
{
  bool __attribute__ ((unused)) result = 
    object->TraceConnect (tracename, context, MakeBoundCallback (&DefaultReceiveSinkWithContext, stream));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultReceiveSinkWithContext():  Unable to hook \"" 
                 << tracename << "\"");
}

/**
 * \brief Base class providing common user-level pcap operations for helpers
 * representing net devices.
 */
class PcapHelperForDevice
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
 * @brief Class providing common pcap and ascii trace operations for helpers
 * working with devices.
 *
 * It would be nice to make this class completely independent of the pcap
 * trace helper for devices, but pybindgen doesn't support multiple inheritance
 * no matter how well behaved, so even mixins are out of the question. Because
 * of this, we have a hierarchy of tracing functionality that devices can
 * add.  If your device helper inherits from PcapHelperForDevice, you get pcap
 * tracing.  If your device helper inherits from TraceHelperForDevice, you
 * get both ascii and pcap tracing.
 */
class TraceHelperForDevice : public PcapHelperForDevice
{
public:
  /**
   * @brief Enable ascii trace output on the indicated net device.
   * @internal
   *
   * The implementation is expected to use a provided Ptr<OutputStreamObject>
   * if it is non-null.  If the OutputStreamObject is null, the implementation
   * is expected to use a provided prefix to construct a new file name for
   * each net device using the rules described in the class overview.
   *
   * If the prefix is provided, there will be one file per net device created.
   * In this case, adding a trace context to the file would be pointless, so
   * the device implementation is expected to TraceConnectWithoutContext.
   *
   * If the output stream object is provided, there may be many different 
   * devices writing to a single file.  In this case, the device adding a 
   * trace context could be important, so the device implementation is 
   * expected to TraceConnect.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param prefix Filename prefix to use for ascii trace files.
   * @param nd Net device for which you want to enable tracing.
   */
  virtual void EnableAsciiInternal (Ptr<OutputStreamObject> stream, std::string prefix, Ptr<NetDevice> nd) = 0;

  /**
   * @brief Enable ascii trace output on the indicated net device.
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param nd Net device for which you want to enable tracing.
   */
  void EnableAscii (std::string prefix, Ptr<NetDevice> nd);

  /**
   * @brief Enable ascii trace output on the indicated net device.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param nd Net device for which you want to enable tracing.
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, Ptr<NetDevice> nd);

  /**
   * @brief Enable ascii trace output the indicated net device using a device 
   * previously named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for ascii files.
   * @param ndName The name of the net device in which you want to enable tracing.
   */
  void EnableAscii (std::string prefix, std::string ndName);

  /**
   * @brief Enable ascii trace output the indicated net device using a device 
   * previously named using the ns-3 object name service.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param ndName The name of the net device in which you want to enable tracing.
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, std::string ndName);

  /**
   * @brief Enable ascii trace output on each device in the container which is
   * of the appropriate type.
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param d container of devices of type ns3::CsmaNetDevice
   */
  void EnableAscii (std::string prefix, NetDeviceContainer d);

  /**
   * @brief Enable ascii trace output on each device in the container which is
   * of the appropriate type.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param d container of devices of type ns3::CsmaNetDevice
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, NetDeviceContainer d);

  /**
   * @brief Enable ascii trace output on each device (which is of the 
   * appropriate type) in the nodes provided in the container.
   *
   * \param prefix Filename prefix to use for ascii files.
   * \param n container of nodes.
   */
  void EnableAscii (std::string prefix, NodeContainer n);

  /**
   * @brief Enable ascii trace output on each device (which is of the 
   * appropriate type) in the nodes provided in the container.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * \param n container of nodes.
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, NodeContainer n);

  /**
   * @brief Enable ascii trace output on each device (which is of the
   * appropriate type) in the set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for ascii files.
   */
  void EnableAsciiAll (std::string prefix);

  /**
   * @brief Enable ascii trace output on each device (which is of the
   * appropriate type) in the set of all nodes created in the simulation.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   */
  void EnableAsciiAll (Ptr<OutputStreamObject> stream);

  /**
   * @brief Enable ascii trace output on the device specified by a global 
   * node-id (of a previously created node) and associated device-id.
   *
   * @param prefix Filename prefix to use when creating ascii trace files
   * @param nodeid The node identifier/number of the node on which to enable
   *               ascii tracing
   * @param deviceid The device identifier/index of the device on which to enable
   *               ascii tracing
   */
  void EnableAscii (std::string prefix, uint32_t nodeid, uint32_t deviceid);

  /**
   * @brief Enable ascii trace output on the device specified by a global 
   * node-id (of a previously created node) and associated device-id.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param nodeid The node identifier/number of the node on which to enable
   *               ascii tracing
   * @param deviceid The device identifier/index of the device on which to enable
   *               ascii tracing
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, uint32_t nodeid, uint32_t deviceid);

private:
  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, uint32_t nodeid, uint32_t deviceid);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, NodeContainer n);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, NetDeviceContainer d);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, std::string ndName);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, Ptr<NetDevice> nd);

};

/**
 * \brief Base class providing common user-level pcap operations for helpers
 * representing IPv4 protocols .
 */
class PcapHelperForIpv4
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
 * @brief Base class providing common pcap and ascii trace operations for 
 * helpers working with Ipv4 interfaces.
 *
 * It would be nice to make this class completely independent of the pcap
 * trace helper for Ipv4, but pybindgen doesn't support multiple inheritance
 * no matter how well behaved, so even mixins are out of the question. Because
 * of this, we have a hierarchy of tracing functionality that protocol helpers
 * can add.  If your helper inherits from PcapHelperForIpv4, you get pcap
 * tracing.  If your helper inherits from PcapAndAsciiHelperForIpv4, you
 * get both ascii and pcap tracing.
 */
class PcapAndAsciiHelperForIpv4 : public PcapHelperForIpv4
{
public:
  /**
   * @brief Enable ascii trace output on the indicated Ipv4 and interface pair.
   * @internal
   *
   * The implementation is expected to use a provided Ptr<OutputStreamObject>
   * if it is non-null.  If the OutputStreamObject is null, the implementation
   * is expected to use a provided prefix to construct a new file name for
   * each net device using the rules described in the class overview.
   *
   * If the prefix is provided, there will be one file per Ipv4 and interface pair
   * created.  In this case, adding a trace context to the file would be pointless,
   * so the helper implementation is expected to TraceConnectWithoutContext.
   *
   * If the output stream object is provided, there may be many different Ipv4 
   * and interface pairs writing to a single file.  In this case, the trace 
   * context could be important, so the helper implementation is expected to 
   * TraceConnect.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param prefix Filename prefix to use for ascii trace files.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  virtual void EnableAsciiIpv4Internal (Ptr<OutputStreamObject> stream, std::string prefix, 
                                    Ptr<Ipv4> ipv4, uint32_t interface) = 0;

  /**
   * @brief Enable ascii trace output on the indicated Ipv4 and interface pair.
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface);

  /**
   * @brief Enable ascii trace output on the indicated Ipv4 and interface pair.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, Ptr<Ipv4> ipv4, uint32_t interface);

  /**
   * @brief Enable ascii trace output the indicated Ipv4 and interface pair
   * using an Ipv4 previously named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for ascii files.
   * @param ipv4Name The name of the Ipv4 on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv4 (std::string prefix, std::string ipv4Name, uint32_t interface);

  /**
   * @brief Enable ascii trace output the indicated net device using a device 
   * previously named using the ns-3 object name service.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param ipv4Name The name of the Ipv4 on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, std::string ipv4Name, uint32_t interface);

  /**
   * @brief Enable ascii trace output on each Ipv4 and interface pair in the 
   * container
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param c Ipv4InterfaceContainer of Ipv4 and interface pairs on which to 
   *          enable tracing.
   */
  void EnableAsciiIpv4 (std::string prefix, Ipv4InterfaceContainer c);

  /**
   * @brief Enable ascii trace output on each device in the container which is
   * of the appropriate type.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param c Ipv4InterfaceContainer of Ipv4 and interface pairs on which to 
   *          enable tracing.
   */
  void EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, Ipv4InterfaceContainer c);

  /**
   * @brief Enable ascii trace output on all Ipv4 and interface pairs existing
   * in the nodes provided in the container.
   *
   * \param prefix Filename prefix to use for ascii files.
   * \param n container of nodes.
   */
  void EnableAsciiIpv4 (std::string prefix, NodeContainer n);

  /**
   * @brief Enable ascii trace output on all Ipv4 and interface pairs existing
   * in the nodes provided in the container.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * \param n container of nodes.
   */
  void EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, NodeContainer n);

  /**
   * @brief Enable ascii trace output on all Ipv4 and interface pairs existing
   * in the set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for ascii files.
   */
  void EnableAsciiIpv4All (std::string prefix);

  /**
   * @brief Enable ascii trace output on each device (which is of the
   * appropriate type) in the set of all nodes created in the simulation.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   */
  void EnableAsciiIpv4All (Ptr<OutputStreamObject> stream);

  /**
   * @brief Enable pcap output on the Ipv4 and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ipv4 aggregated to a node, the node-id unambiguously 
   * determines the Ipv4.
   *
   * @param prefix Filename prefix to use when creating ascii trace files
   * @param nodeid The node identifier/number of the node on which to enable
   *               ascii tracing
   * @param interface The device identifier/index of the device on which to enable
   *               ascii tracing
   */
  void EnableAsciiIpv4 (std::string prefix, uint32_t nodeid, uint32_t deviceid);

  /**
   * @brief Enable pcap output on the Ipv4 and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ipv4 aggregated to a node, the node-id unambiguously 
   * determines the Ipv4.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param nodeid The node identifier/number of the node on which to enable
   *               ascii tracing
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, uint32_t nodeid, uint32_t interface);

private:
  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv4Impl (Ptr<OutputStreamObject> stream, std::string prefix, uint32_t nodeid, uint32_t interface);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv4Impl (Ptr<OutputStreamObject> stream, std::string prefix, NodeContainer n);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv4Impl (Ptr<OutputStreamObject> stream, std::string prefix, Ipv4InterfaceContainer c);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv4Impl (Ptr<OutputStreamObject> stream, std::string prefix, std::string ipv4Name, uint32_t interface);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv4Impl (Ptr<OutputStreamObject> stream, std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface);

};

/**
 * \brief Base class providing common user-level ascii and pcap tracing
 * for Ipv4 protocols plus pcap tracing for IPv6 protocols .
 *
 * It would be nice to make this class completely independent of the trace 
 * helpers for Ipv4, but pybindgen doesn't support multiple inheritance
 * no matter how well behaved, so even mixins are out of the question. Because
 * of this, we have a hierarchy of tracing functionality that protocols can
 * add.  If your protocol helper inherits from PcapHelperForIpv4, you get pcap
 * tracing for Ipv4 protocols.  If your protocol helper inherits from 
 * PcapAndAsciiHelperForIpv4, you get both ascii and pcap tracing for Ipv4
 * protocols.  If you inherit from PcapAndAsciiHelperForIpv4AndPcapHelperForIpv6
 * you get both ascii and pcap tracing for Ipv4 and pcap tracing for Ipv6.
 */
class PcapAndAsciiHelperForIpv4AndPcapHelperForIpv6 : public PcapAndAsciiHelperForIpv4
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

/**
 * @brief Base class providing common ascii trace operations for helpers
 * working with Ipv4 and Ipv6 interfaces.
 *
 * It would be nice to make this class completely independent of the trace 
 * helpers for Ipv4, and the pcap helper for Ipv6, but pybindgen doesn't support
 * multiple inheritance no matter how well behaved, so even mixins are out of 
 * the question. Because of this, we have a hierarchy of tracing functionality
 * that protocoll helpers can add.  If your protocol helper inherits from 
 * PcapHelperForIpv4, you get pcap tracing for Ipv4 protocols.  If your protocol
 * helper inherits from PcapAndAsciiHelperForIpv4, you get both ascii and pcap 
 * tracing for Ipv4 protocols.  If your helper inherits from 
 * PcapAndAsciiHelperForIpv4AndPcapHelperForIpv6 you get both ascii and pcap 
 * tracing for Ipv4 and pcap tracing for Ipv6.  If your device helper inherits
 * from TraceHelperForProtocol, you get the full meal deal.
 */
class TraceHelperForProtocol : public PcapAndAsciiHelperForIpv4AndPcapHelperForIpv6
{
public:
  /**
   * @brief Enable ascii trace output on the indicated Ipv6 and interface pair.
   * @internal
   *
   * The implementation is expected to use a provided Ptr<OutputStreamObject>
   * if it is non-null.  If the OutputStreamObject is null, the implementation
   * is expected to use a provided prefix to construct a new file name for
   * each net device using the rules described in the class overview.
   *
   * If the prefix is provided, there will be one file per Ipv6 and interface pair
   * created.  In this case, adding a trace context to the file would be pointless,
   * so the helper implementation is expected to TraceConnectWithoutContext.
   *
   * If the output stream object is provided, there may be many different Ipv6 
   * and interface pairs writing to a single file.  In this case, the trace 
   * context could be important, so the helper implementation is expected to 
   * TraceConnect.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param prefix Filename prefix to use for ascii trace files.
   * @param ipv6 Ptr<Ipv6> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  virtual void EnableAsciiIpv6Internal (Ptr<OutputStreamObject> stream, std::string prefix, 
                                        Ptr<Ipv6> ipv6, uint32_t interface) = 0;

  /**
   * @brief Enable ascii trace output on the indicated Ipv6 and interface pair.
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param ipv6 Ptr<Ipv6> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv6 (std::string prefix, 
                        Ptr<Ipv6> ipv6, uint32_t interface);

  /**
   * @brief Enable ascii trace output on the indicated Ipv6 and interface pair.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param ipv6 Ptr<Ipv6> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, 
                        Ptr<Ipv6> ipv6, uint32_t interface);

  /**
   * @brief Enable ascii trace output the indicated Ipv6 and interface pair
   * using an Ipv6 previously named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for ascii files.
   * @param ipv6Name The name of the Ipv6 on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv6 (std::string prefix, 
                        std::string ipv6Name, uint32_t interface);

  /**
   * @brief Enable ascii trace output the indicated net device using a device 
   * previously named using the ns-3 object name service.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param ipv6Name The name of the Ipv6 on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, 
                        std::string ipv6Name, uint32_t interface);

  /**
   * @brief Enable ascii trace output on each Ipv6 and interface pair in the 
   * container
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param c Ipv6InterfaceContainer of Ipv6 and interface pairs on which to 
   *          enable tracing.
   */
  void EnableAsciiIpv6 (std::string prefix, Ipv6InterfaceContainer c);

  /**
   * @brief Enable ascii trace output on each device in the container which is
   * of the appropriate type.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param c Ipv6InterfaceContainer of Ipv6 and interface pairs on which to 
   *          enable tracing.
   */
  void EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, Ipv6InterfaceContainer c);

  /**
   * @brief Enable ascii trace output on all Ipv6 and interface pairs existing
   * in the nodes provided in the container.
   *
   * \param prefix Filename prefix to use for ascii files.
   * \param n container of nodes.
   */
  void EnableAsciiIpv6 (std::string prefix, NodeContainer n);

  /**
   * @brief Enable ascii trace output on all Ipv6 and interface pairs existing
   * in the nodes provided in the container.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * \param n container of nodes.
   */
  void EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, NodeContainer n);

  /**
   * @brief Enable pcap output on the Ipv6 and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ipv6 aggregated to a node, the node-id unambiguously 
   * determines the Ipv6.
   *
   * @param prefix Filename prefix to use when creating ascii trace files
   * @param nodeid The node identifier/number of the node on which to enable
   *               ascii tracing
   * @param interface The device identifier/index of the device on which to enable
   *               ascii tracing
   */
  void EnableAsciiIpv6 (std::string prefix, uint32_t nodeid, uint32_t deviceid);

  /**
   * @brief Enable pcap output on the Ipv6 and interface pair specified by a 
   * global node-id (of a previously created node) and interface.  Since there
   * can be only one Ipv6 aggregated to a node, the node-id unambiguously 
   * determines the Ipv6.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param nodeid The node identifier/number of the node on which to enable
   *               ascii tracing
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, uint32_t nodeid, uint32_t interface);

  /**
   * @brief Enable ascii trace output on all Ipv6 and interface pairs existing
   * in the set of all nodes created in the simulation.
   *
   * @param prefix Filename prefix to use for ascii files.
   */
  void EnableAsciiIpv6All (std::string prefix);

  /**
   * @brief Enable ascii trace output on each device (which is of the
   * appropriate type) in the set of all nodes created in the simulation.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   */
  void EnableAsciiIpv6All (Ptr<OutputStreamObject> stream);

private:
  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv6Impl (Ptr<OutputStreamObject> stream, std::string prefix, uint32_t nodeid, uint32_t interface);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv6Impl (Ptr<OutputStreamObject> stream, std::string prefix, NodeContainer n);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv6Impl (Ptr<OutputStreamObject> stream, std::string prefix, Ipv6InterfaceContainer c);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv6Impl (Ptr<OutputStreamObject> stream, std::string prefix, std::string ipv6Name, uint32_t interface);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiIpv6Impl (Ptr<OutputStreamObject> stream, std::string prefix, Ptr<Ipv6> ipv6, uint32_t interface);

};

} // namespace ns3

#endif /* TRACE_HELPER_H */
