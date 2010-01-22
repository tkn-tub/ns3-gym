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
#include "ns3/node-container.h"
#include "ns3/simulator.h"
#include "ns3/pcap-file-object.h"
#include "ns3/ipv4.h"

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
  std::string GetFilenameFromInterfacePair (std::string prefix, Ptr<Ipv4> ipv4, 
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

} // namespace ns3

#endif /* PCAP_HELPER_H */
