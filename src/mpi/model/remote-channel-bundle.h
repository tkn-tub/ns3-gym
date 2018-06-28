/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright 2013. Lawrence Livermore National Security, LLC.
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
 * Author: Steven Smith <smith84@llnl.gov>
 *
 */

#ifndef NS3_REMOTE_CHANNEL_BUNDLE
#define NS3_REMOTE_CHANNEL_BUNDLE

#include "null-message-simulator-impl.h"

#include <ns3/channel.h>
#include <ns3/ptr.h>
#include <ns3/pointer.h>

#include <map>

namespace ns3 {

/**
 * \ingroup mpi
 * 
 * \brief Collection of NS3 channels between local and remote nodes.
 * 
 * An instance exists for each remote system that the local system is
 * in communication with.  These are created and managed by the
 * RemoteChannelBundleManager class.  Stores time information for each
 * bundle.
 */
class RemoteChannelBundle : public Object
{
public:
  static TypeId GetTypeId (void);

  RemoteChannelBundle ();

  RemoteChannelBundle (const uint32_t remoteSystemId);

  ~RemoteChannelBundle ()
  {
  }

  /**
   * \param channel to add to the bundle
   * \param delay time for the channel (usually the latency)
   */
  void AddChannel (Ptr<Channel> channel, Time delay);

  /**
   * \return SystemID for remote side of this bundle
   */
  uint32_t GetSystemId () const;

  /**
   * \return guarantee time
   */
  Time GetGuaranteeTime (void) const;

  /**
   * \param guarantee time
   *
   * Set the guarantee time for the bundle.  This should be called
   * after a packet or Null Message received.
   */
  void SetGuaranteeTime (Time time);

  /**
   * \return the minimum delay along any channel in this bundle
   */
  Time GetDelay (void) const;

  /**
   * Set the event ID of the Null Message send event current scheduled
   * for this channel.
   */
  void SetEventId (EventId id);

  /**
   * \return the event ID of the Null Message send event for this bundle
   */
  EventId GetEventId (void) const;

  /**
   * \return number of NS3 channels in this bundle
   */
  std::size_t GetSize (void) const;

  /**
   * \param time 
   *
   * Send Null Message to the remote task associated with this bundle.
   * Message will be delivered at current simulation time + the time
   * passed in.
   */
  void Send(Time time);

  /**
   * Output for debugging purposes.
   */
  friend std::ostream& operator<< (std::ostream& out, ns3::RemoteChannelBundle& bundle );

private:
  /*
   * Remote rank.
   */
  uint32_t m_remoteSystemId;

  /*
   * NS3 Channels that are connected from nodes in this MPI task to remote_rank.
   *
   * Would be more efficient to use unordered_map when C++11 is adopted by NS3.
   */
  std::map < uint32_t, Ptr < Channel > > m_channels;

  /*
   * Guarantee time for the incoming Channels from MPI task remote_rank.
   * No PacketMessage will ever arrive on any incoming channel in this bundle with a
   * ReceiveTime less than this.  Initialized to StartTime.
   */
  Time m_guaranteeTime;

  /*
   * Delay for this Channel bundle.   min link delay over all incoming channels;
   */
  Time m_delay;

  /*
   * Event scheduled to send Null Message for this bundle.
   */
  EventId m_nullEventId;

};

}

#endif
