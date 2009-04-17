/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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

#ifndef POINT_TO_POINT_CHANNEL_H
#define POINT_TO_POINT_CHANNEL_H

#include <list>
#include "ns3/channel.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

namespace ns3 {

class PointToPointNetDevice;
class Packet;

/**
 * \brief Simple Point To Point Channel.
 *
 * This class represents a very simple point to point channel.  Think full
 * duplex RS-232 or RS-422 with null modem and no handshaking.  There is no
 * multi-drop capability on this channel -- there can be a maximum of two 
 * point-to-point net devices connected.
 *
 * There are two "wires" in the channel.  The first device connected gets the
 * [0] wire to transmit on.  The second device gets the [1] wire.  There is a
 * state (IDLE, TRANSMITTING) associated with each wire.
 */
class PointToPointChannel : public Channel 
{
public:
  static TypeId GetTypeId (void);

  /**
   * \brief Create a PointToPointChannel
   *
   * By default, you get a channel that
   * has zero transmission delay.
   */
  PointToPointChannel ();

  /**
   * \brief Attach a given netdevice to this channel
   * \param device pointer to the netdevice to attach to the channel
   */
  void Attach (Ptr<PointToPointNetDevice> device);

  /**
   * \brief Transmit a packet over this channel
   * \param p Packet to transmit
   * \param src Source PointToPointNetDevice
   * \param txTime Transmit time to apply
   * \returns true if successful (currently always true)
   */
  bool TransmitStart (Ptr<Packet> p, Ptr<PointToPointNetDevice> src, Time txTime);

  /**
   * \brief Get number of devices on this channel
   * \returns number of devices on this channel
   */
  virtual uint32_t GetNDevices (void) const;

  /*
   * \brief Get PointToPointNetDevice corresponding to index i on this channel
   * \param i Index number of the device requested
   * \returns Ptr to PointToPointNetDevice requested
   */
  Ptr<PointToPointNetDevice> GetPointToPointDevice (uint32_t i) const;

  /*
   * \brief Get NetDevice corresponding to index i on this channel
   * \param i Index number of the device requested
   * \returns Ptr to NetDevice requested
   */
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

private:
  // Each point to point link has exactly two net devices
  static const int N_DEVICES = 2;

  Time          m_delay;
  int32_t       m_nDevices;

  enum WireState
    {
      INITIALIZING,
      IDLE,
      TRANSMITTING,
      PROPAGATING
    };

  class Link
  {
  public:
    Link() : m_state (INITIALIZING), m_src (0), m_dst (0) {}
    WireState                  m_state;
    Ptr<PointToPointNetDevice> m_src;
    Ptr<PointToPointNetDevice> m_dst;
  };
    
  Link    m_link[N_DEVICES];
};

} // namespace ns3

#endif /* POINT_TO_POINT_CHANNEL_H */
