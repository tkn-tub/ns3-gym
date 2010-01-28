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

#ifndef ASCII_TRACE_HELPER_H
#define ASCII_TRACE_HELPER_H

#include "ns3/assert.h"
#include "ns3/net-device-container.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv6.h"
#include "ns3/ipv6-interface-container.h"
#include "ns3/node-container.h"
#include "ns3/simulator.h"
#include "ns3/output-stream-object.h"

namespace ns3 {

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
  void HookDefaultEnqueueSinkWithContext (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

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
  void HookDefaultDropSinkWithContext (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

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
  void HookDefaultDequeueSinkWithContext (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

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
  void HookDefaultReceiveSinkWithContext (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

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
  bool result = object->TraceConnectWithoutContext (tracename, 
                                                    MakeBoundCallback (&DefaultEnqueueSinkWithoutContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultEnqueueSinkWithoutContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultEnqueueSinkWithContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  std::string context ("XXX");
  bool result = object->TraceConnect (tracename, context, MakeBoundCallback (&DefaultEnqueueSinkWithContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultEnqueueSinkWithContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultDropSinkWithoutContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  bool result = object->TraceConnectWithoutContext (tracename, 
                                                    MakeBoundCallback (&DefaultDropSinkWithoutContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultDropSinkWithoutContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultDropSinkWithContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  std::string context ("XXX");
  bool result = object->TraceConnect (tracename, context, MakeBoundCallback (&DefaultDropSinkWithContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultDropSinkWithContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultDequeueSinkWithoutContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  bool result = object->TraceConnectWithoutContext (tracename, 
                                                    MakeBoundCallback (&DefaultDequeueSinkWithoutContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultDequeueSinkWithoutContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultDequeueSinkWithContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  std::string context ("XXX");
  bool result = object->TraceConnect (tracename, context, MakeBoundCallback (&DefaultDequeueSinkWithContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultDequeueSinkWithContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultReceiveSinkWithoutContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  bool result = object->TraceConnectWithoutContext (tracename, 
                                                    MakeBoundCallback (&DefaultReceiveSinkWithoutContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultReceiveSinkWithoutContext():  Unable to hook \"" 
                 << tracename << "\"");
}

template <typename T> void
AsciiTraceHelper::HookDefaultReceiveSinkWithContext (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  std::string context ("XXX");
  bool result = object->TraceConnect (tracename, context, MakeBoundCallback (&DefaultReceiveSinkWithContext, file));
  NS_ASSERT_MSG (result == true, "AsciiTraceHelper::HookDefaultReceiveSinkWithContext():  Unable to hook \"" 
                 << tracename << "\"");
}

/**
 * @brief Base class providing common ascii trace operations for helpers
 * working with devices.
 *
 * There are two basic flavors of ascii tracing.  The first kind will case a
 * trace file to be created for every traced device in the form 
 * <file prefix>-<node number>-<device index>.tr just like the pcap trace 
 * helpers would do.  Additionally, if the object name service is used to 
 * define either the node or device, the name will be substituted in the 
 * file name.  This form of ascii tracing does not include a context string
 * and makes it easier to determine the source of the event.
 *
 * The second kind of tracing is more like ns-2 tracing in that there is
 * one trace file into which all of the specified events of the specified
 * devices are written.  This form of ascii tracing does include a context
 * string and interleaves the trace hits from all of the devices into a 
 * single file.
 */
class AsciiTraceUserHelperForDevice
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
 * @brief Base class providing common ascii trace operations for helpers
 * working with Ipv4 interfaces.
 *
 * There are two basic flavors of ascii tracing.  The first kind will case a
 * trace file to be created for every traced device in the form 
 * <file prefix>-<node number>-<device index>.tr just like the pcap trace 
 * helpers would do.  Additionally, if the object name service is used to 
 * define either the node or device, the name will be substituted in the 
 * file name.  This form of ascii tracing does not include a context string
 * and makes it easier to determine the source of the event.
 *
 * The second kind of tracing is more like ns-2 tracing in that there is
 * one trace file into which all of the specified events of the specified
 * devices are written.  This form of ascii tracing does include a context
 * string and interleaves the trace hits from all of the devices into a 
 * single file.
 */
class AsciiTraceUserHelperForIpv4
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
 * @brief Base class providing common ascii trace operations for helpers
 * working with Ipv6 interfaces.
 *
 * There are two basic flavors of ascii tracing.  The first kind will case a
 * trace file to be created for every traced device in the form 
 * <file prefix>-<node number>-<device index>.tr just like the pcap trace 
 * helpers would do.  Additionally, if the object name service is used to 
 * define either the node or device, the name will be substituted in the 
 * file name.  This form of ascii tracing does not include a context string
 * and makes it easier to determine the source of the event.
 *
 * The second kind of tracing is more like ns-2 tracing in that there is
 * one trace file into which all of the specified events of the specified
 * devices are written.  This form of ascii tracing does include a context
 * string and interleaves the trace hits from all of the devices into a 
 * single file.
 */
class AsciiTraceUserHelperForIpv6
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

#endif /* ASCIITRACE_HELPER_H */
