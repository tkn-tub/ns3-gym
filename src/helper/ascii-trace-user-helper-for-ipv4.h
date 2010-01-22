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

#ifndef ASCII_TRACE_USER_HELPER_FOR_IPV4_H
#define ASCII_TRACE_USER_HELPER_FOR_IPV4_H

#include <string>
#include "ns3/ipv4-interface-container.h"
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
  virtual void EnableAsciiInternal (Ptr<OutputStreamObject> stream, std::string prefix, 
                                    Ptr<Ipv4> ipv4, uint32_t interface) = 0;

  /**
   * @brief Enable ascii trace output on the indicated Ipv4 and interface pair.
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAscii (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface);

  /**
   * @brief Enable ascii trace output on the indicated Ipv4 and interface pair.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param ipv4 Ptr<Ipv4> on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, Ptr<Ipv4> ipv4, uint32_t interface);

  /**
   * @brief Enable ascii trace output the indicated Ipv4 and interface pair
   * using an Ipv4 previously named using the ns-3 object name service.
   *
   * @param filename filename prefix to use for ascii files.
   * @param ipv4Name The name of the Ipv4 on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAscii (std::string prefix, std::string ipv4Name, uint32_t interface);

  /**
   * @brief Enable ascii trace output the indicated net device using a device 
   * previously named using the ns-3 object name service.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param ipv4Name The name of the Ipv4 on which you want to enable tracing.
   * @param interface The interface on which you want to enable tracing.
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, std::string ipv4Name, uint32_t interface);

  /**
   * @brief Enable ascii trace output on each Ipv4 and interface pair in the 
   * container
   *
   * @param prefix Filename prefix to use for ascii files.
   * @param c Ipv4InterfaceContainer of Ipv4 and interface pairs on which to 
   *          enable tracing.
   */
  void EnableAscii (std::string prefix, Ipv4InterfaceContainer c);

  /**
   * @brief Enable ascii trace output on each device in the container which is
   * of the appropriate type.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * @param c Ipv4InterfaceContainer of Ipv4 and interface pairs on which to 
   *          enable tracing.
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, Ipv4InterfaceContainer c);

  /**
   * @brief Enable ascii trace output on all Ipv4 and interface pairs existing
   * in the nodes provided in the container.
   *
   * \param prefix Filename prefix to use for ascii files.
   * \param n container of nodes.
   */
  void EnableAscii (std::string prefix, NodeContainer n);

  /**
   * @brief Enable ascii trace output on all Ipv4 and interface pairs existing
   * in the nodes provided in the container.
   *
   * @param stream An OutputStreamObject representing an existing file to use
   *               when writing trace data.
   * \param n container of nodes.
   */
  void EnableAscii (Ptr<OutputStreamObject> stream, NodeContainer n);

  /**
   * @brief Enable ascii trace output on all Ipv4 and interface pairs existing
   * in the set of all nodes created in the simulation.
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
  void EnableAscii (std::string prefix, uint32_t nodeid, uint32_t deviceid);

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
  void EnableAscii (Ptr<OutputStreamObject> stream, uint32_t nodeid, uint32_t interface);

private:
  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, uint32_t nodeid, uint32_t interface);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, NodeContainer n);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, Ipv4InterfaceContainer c);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, std::string ipv4Name, uint32_t interface);

  /**
   * @internal Avoid code duplication.
   */
  void EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface);

};

} // namespace ns3

#endif // ASCII_USER_HELPER_FOR_IPV4_H
