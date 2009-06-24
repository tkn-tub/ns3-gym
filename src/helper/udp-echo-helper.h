/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef UDP_ECHO_HELPER_H
#define UDP_ECHO_HELPER_H

#include <stdint.h>
#include "application-container.h"
#include "node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

class UdpEchoServerHelper
{
public:
  UdpEchoServerHelper (uint16_t port);

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (Ptr<Node> node) const;
  ApplicationContainer Install (std::string nodeName) const;
  ApplicationContainer Install (NodeContainer c) const;

private:
  Ptr<Application> InstallPriv (Ptr<Node> node) const;

  ObjectFactory m_factory;
};

class UdpEchoClientHelper
{
public:
  UdpEchoClientHelper (Ipv4Address ip, uint16_t port);

  void SetAttribute (std::string name, const AttributeValue &value);

  /**
   * Given a pointer to a UdpEchoClient application, set the data fill of the 
   * packet (what is sent as data to the server) to the contents of the fill
   * string (including the trailing zero terminator).
   *
   * \warn The size of resulting echo packets will be automatically adjusted
   * to reflect the size of the fill string -- this means that the PacketSize
   * attribute may be changed as a result of this call.
   *
   * \param app Smart pointer to the application (real type must be UdpEchoClient).
   * \param fill The string to use as the actual echo data bytes.
   */
  void SetFill (Ptr<Application> app, std::string fill);

  /**
   * Given a pointer to a UdpEchoClient application, set the data fill of the 
   * packet (what is sent as data to the server) to the contents of the fill
   * byte.
   *
   * The fill byte will be used to initialize the contents of the data packet.
   * 
   * \warn The size of resulting echo packets will be automatically adjusted
   * to reflect the dataLength parameter -- this means that the PacketSize
   * attribute may be changed as a result of this call.
   *
   * \param app Smart pointer to the application (real type must be UdpEchoClient).
   * \param fill The byte to be repeated in constructing the packet data..
   * \param dataLength The desired length of the resulting echo packet data.
   */
  void SetFill (Ptr<Application> app, uint8_t fill, uint32_t dataLength);

  /**
   * Given a pointer to a UdpEchoClient application, set the data fill of the 
   * packet (what is sent as data to the server) to the contents of the fill
   * buffer, repeated as many times as is required.
   *
   * Initializing the fill to the contents of a single buffer is accomplished
   * by providing a complete buffer with fillLength set to your desired 
   * dataLength
   *
   * \warn The size of resulting echo packets will be automatically adjusted
   * to reflect the dataLength parameter -- this means that the PacketSize
   * attribute of the Application may be changed as a result of this call.
   *
   * \param app Smart pointer to the application (real type must be UdpEchoClient).
   * \param fill The fill pattern to use when constructing packets.
   * \param fillLength The number of bytes in the provided fill pattern.
   * \param dataLength The desired length of the final echo data.
   */
  void SetFill (Ptr<Application> app, uint8_t *fill, uint32_t fillLength, uint32_t dataLength);

  ApplicationContainer Install (Ptr<Node> node) const;
  ApplicationContainer Install (std::string nodeName) const;
  ApplicationContainer Install (NodeContainer c) const;

private:
  Ptr<Application> InstallPriv (Ptr<Node> node) const;
  ObjectFactory m_factory;
};

} // namespace ns3

#endif /* UDP_ECHO_HELPER_H */
