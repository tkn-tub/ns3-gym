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

#ifndef ASCII_TRACE_USER_HELPER_FOR_DEVICE_H
#define ASCII_TRACE_USER_HELPER_FOR_DEVICE_H

#include <string>
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/output-stream-object.h"

namespace ns3 {

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

} // namespace ns3

#endif // ASCII_USER_HELPER_FOR_DEVICE_H
