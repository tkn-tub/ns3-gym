/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Telecom Bretagne
 * Copyright (c) 2009 Strasbourg University
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
 *         Mehdi Benamor <benamor.mehdi@ensi.rnu.tn>
 */

#ifndef RADVD_H
#define RADVD_H

#include <map>

#include "ns3/application.h"
#include "ns3/socket.h"
#include "ns3/random-variable-stream.h"

#include "radvd-interface.h"

namespace ns3
{

/**
 * \ingroup applications 
 * \defgroup radvd Radvd
 */

/**
 * \ingroup radvd
 * \class Radvd
 * \brief Router advertisement daemon.
 */
class Radvd : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return type ID
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor.
   */
  Radvd ();

  /**
   * \brief Destructor.
   */
  virtual ~Radvd ();

  /**
   * \brief Default value for maximum delay of RA (ms)
   */
  static const uint32_t MAX_RA_DELAY_TIME = 500;

  /**
   * \brief Add configuration for an interface;
   * \param routerInterface configuration
   */
  void AddConfiguration (Ptr<RadvdInterface> routerInterface);

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  int64_t AssignStreams (int64_t stream);

protected:
  /**
   * \brief Dispose the instance.
   */
  virtual void DoDispose ();

private:
  /// Container: Ptr to RadvdInterface
  typedef std::list<Ptr<RadvdInterface> > RadvdInterfaceList;
  /// Container Iterator: Ptr to RadvdInterface
  typedef std::list<Ptr<RadvdInterface> >::iterator RadvdInterfaceListI;
  /// Container Const Iterator: Ptr to RadvdInterface
  typedef std::list<Ptr<RadvdInterface> >::const_iterator RadvdInterfaceListCI;

  /// Container: interface number, EventId
  typedef std::map<uint32_t, EventId> EventIdMap;
  /// Container Iterator: interface number, EventId
  typedef std::map<uint32_t, EventId>::iterator EventIdMapI;
  /// Container Const Iterator: interface number, EventId
  typedef std::map<uint32_t, EventId>::const_iterator EventIdMapCI;

  /**
   * \brief Start the application.
   */
  virtual void StartApplication ();

  /**
   * \brief Stop the application.
   */
  virtual void StopApplication ();

  /**
   * \brief Schedule sending a packet.
   * \param dt interval between packet
   * \param config interface configuration
   * \param eventId event ID associated
   * \param dst IPv6 destination address
   * \param reschedule if true another send will be reschedule (periodic)
   */
  void ScheduleTransmit (Time dt, Ptr<RadvdInterface> config, EventId& eventId, Ipv6Address dst = Ipv6Address::GetAllNodesMulticast (), bool reschedule = false);

  /**
   * \brief Send a packet.
   * \param config interface configuration
   * \param dst destination address (default ff02::1)
   * \param reschedule if true another send will be reschedule (periodic)
   */
  void Send (Ptr<RadvdInterface> config, Ipv6Address dst = Ipv6Address::GetAllNodesMulticast (), bool reschedule = false);

  /**
   * \brief Handle received packet, especially router solicitation
   * \param socket socket to read data from
   */
  void HandleRead (Ptr<Socket> socket);

  /**
   * \brief Raw socket to send RA.
   */
  Ptr<Socket> m_socket;

  /**
   * \brief List of configuration for interface.
   */
  RadvdInterfaceList m_configurations;

  /**
   * \brief Event ID map.
   */
  EventIdMap m_eventIds;

  /**
   * \brief Variable to provide jitter in advertisement interval
   */
  Ptr<UniformRandomVariable> m_jitter;
};

} /* namespace ns3 */

#endif /* RADVD_H */

