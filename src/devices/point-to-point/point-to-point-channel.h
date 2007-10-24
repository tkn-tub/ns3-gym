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
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

namespace ns3 {

class PointToPointNetDevice;

/**
 * \brief Simple Point To Point Channel.
 *
 * This class represents a very simple point to point channel.  Think full
 * duplex RS-232 or RS-422 with null modem and no handshaking.  There is no
 * multi-drop capability on this channel -- there can be a maximum of two 
 * point-to-point net devices connected.  Once we start talking about multi-
 * drop, or CSMA, or some other sharing mechanism, things begin getting 
 * complicated quickly.  Rather than invent some ad-hoc mechanism, we just
 * Keep It Simple everywhere.
 *
 * When the channel is instaniated, the constructor takes parameters for
 * a single speed, in bits per second, and a speed-of-light delay time as a
 * Time object.  Both directions use the same speed and delay time.
 *
 * There are two "wires" in the channel.  The first device connected gets the
 * [0] wire to transmit on.  The second device gets the [1] wire.  There is a
 * state (IDLE, TRANSMITTING) associated with each wire.
 */
class PointToPointChannel : public Channel {
public:
// Each point to point link has exactly two net devices
  static const int N_DEVICES = 2;
  /**
   * \brief Create a PointToPointChannel
   *
   * By default, you get a channel with the name "PointToPoint Channel" that
   * has an "infitely" fast transmission speed and zero delay.
   */
  PointToPointChannel ();
  
  /**
   * \brief Create a PointToPointChannel
   *
   * \param bps The maximum bitrate of the channel
   * \param delay Transmission delay through the channel
   */  
  PointToPointChannel (const DataRate& bps, const Time& delay);
  
  /**
   * \brief Create a PointToPointChannel
   *
   * \param name the name of the channel for identification purposes
   * \param bps The maximum bitrate of the channel
   * \param delay Transmission delay through the channel
   */
  PointToPointChannel (const std::string& name,
                 const DataRate& bps, const Time& delay);

  /**
   * \brief Attach a given netdevice to this channel
   * \param device pointer to the netdevice to attach to the channel
   */
  void Attach (Ptr<PointToPointNetDevice> device);
  bool TransmitStart (Packet& p, Ptr<PointToPointNetDevice> src,
                      const Time& txTime);
  // Below two not needed
  //bool TransmitEnd (Packet &p, Ptr<PointToPointNetDevice> src);
  //void PropagationCompleteEvent(Packet p, Ptr<PointToPointNetDevice> src);


  virtual uint32_t GetNDevices (void) const;
  /*
   * DoGetDevice
   */
  Ptr<PointToPointNetDevice> GetDevice (uint32_t i) const;

  virtual const DataRate& GetDataRate (void);
  virtual const Time&     GetDelay (void);

private:

  virtual Ptr<NetDevice> DoGetDevice (uint32_t i) const;

  DataRate      m_bps;
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
    WireState              m_state;
    Ptr<PointToPointNetDevice> m_src;
    Ptr<PointToPointNetDevice> m_dst;
  };
    
  Link    m_link[N_DEVICES];
};

} // namespace ns3

#endif /* POINT_TO_POINT_CHANNEL_H */
