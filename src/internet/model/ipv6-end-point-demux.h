/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007-2009 Strasbourg University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#ifndef IPV6_END_POINT_DEMUX_H
#define IPV6_END_POINT_DEMUX_H

#include <stdint.h>
#include <list>
#include "ns3/ipv6-address.h"
#include "ipv6-interface.h"

namespace ns3
{

class Ipv6EndPoint;

/**
 * \class Ipv6EndPointDemux
 * \brief Demultiplexor for end points.
 */
class Ipv6EndPointDemux
{
public:
  typedef std::list<Ipv6EndPoint *>EndPoints;
  typedef std::list<Ipv6EndPoint *>::iterator EndPointsI;

  /**
   * \brief Constructor.
   */
  Ipv6EndPointDemux ();

  /**
   * \brief Destructor.
   */
  ~Ipv6EndPointDemux ();

  /**
   * \brief Lookup for port local.
   * \param port port to test
   * \return true if a port local is in EndPoints, false otherwise
   */
  bool LookupPortLocal (uint16_t port);

  /**
   * \brief Lookup for address and port.
   * \param addr address to test
   * \param port port to test
   * \return true if there is a match in EndPoints, false otherwise
   */
  bool LookupLocal (Ipv6Address addr, uint16_t port);

  /**
   * \brief lookup for a match with all the parameters.
   * \param dst destination address to test
   * \param dport destination port to test
   * \param src source address to test
   * \param sport source port to test
   * \param incomingInterface the incoming interface
   * \return list en IPv6EndPoints (could be 0 element)
   */
  EndPoints Lookup (Ipv6Address dst, uint16_t dport, Ipv6Address src, uint16_t sport, Ptr<Ipv6Interface> incomingInterface);

  /**
   * \brief Simple lookup for a four-tuple match.
   * \param dst destination address to test
   * \param dport destination port to test
   * \param src source address to test
   * \param sport source port to test
   * \return match or 0 if not found
   */
  Ipv6EndPoint* SimpleLookup (Ipv6Address dst, uint16_t dport, Ipv6Address src, uint16_t sport);

  /**
   * \brief Allocate a Ipv6EndPoint.
   * \return an empty Ipv6EndPoint instance
   */
  Ipv6EndPoint *Allocate (void);

  /**
   * \brief Allocate a Ipv6EndPoint.
   * \param address IPv6 address
   * \return an Ipv6EndPoint instance
   */
  Ipv6EndPoint *Allocate (Ipv6Address address);

  /**
   * \brief Allocate a Ipv6EndPoint.
   * \param port local port
   * \return an Ipv6EndPoint instance
   */
  Ipv6EndPoint *Allocate (uint16_t port);

  /**
   * \brief Allocate a Ipv6EndPoint.
   * \param address local address
   * \param port local port
   * \return an Ipv6EndPoint instance
   */
  Ipv6EndPoint *Allocate (Ipv6Address address, uint16_t port);

  /**
   * \brief Allocate a Ipv6EndPoint.
   * \param localAddress local address
   * \param localPort local port
   * \param peerAddress peer address
   * \param peerPort peer port
   * \return an Ipv6EndPoint instance
   */
  Ipv6EndPoint *Allocate (Ipv6Address localAddress, uint16_t localPort, Ipv6Address peerAddress, uint16_t peerPort);

  /**
   * \brief Remove a end point.
   * \param endPoint the end point to remove
   */
  void DeAllocate (Ipv6EndPoint *endPoint);

  /**
   * \brief Get the entire list of end points registered.
   * \return list of Ipv6EndPoint
   */
  EndPoints GetEndPoints () const;

private:
  /**
   * \brief Allocate a ephemeral port.
   * \return a port
   */
  uint16_t AllocateEphemeralPort ();

  /**
   * \brief The ephemeral port.
   */
  uint16_t m_ephemeral;

  /**
   * \brief A list of IPv6 end points.
   */
  EndPoints m_endPoints;
};

} /* namespace ns3 */

#endif /* IPV6_END_POINT_DEMUX_H */

