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

#ifndef SERIAL_CHANNEL_H
#define SERIAL_CHANNEL_H

#include <list>
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"

namespace ns3 {

// temporary until Raj's code makes it into the dev tree
typedef uint64_t DataRate;
class SerialPhy;
class NetDevice;

/**
 * \brief Simple Point To Point Channel.
 *
 * This class represents a very simple serial channel.  Think full duplex
 * RS-232 or RS-422 with null modem and no handshaking.  There is no multi-
 * drop capability on this channel -- there can be a maximum of two serial 
 * net devices connected.  Once we start talking about multi-drop, or CSMA, 
 * or some other sharing mechanism, things begin getting complicated quickly.
 * Rather than invent some ad-hoc mechanism, we just Keep It Simple everywhere.
 *
 * When the channel is instaniated, the constructor takes parameters for
 * a single channel speed, in bits per second, and a delay time as a Time
 * object.  Both directions use the same speed and delay time.
 *
 * There are two "wires" in the channel.  The first device connected gets the
 * [0] wire to transmit on.  The second device gets the [1] wire.  There is a
 * state (IDLE, TRANSMITTING) associated with each wire.
 */
class SerialChannel : public Channel {
public:
//
// This is really kidding myself, since just setting N_DEVICES to 3 isn't
// going to come close to magically creating a multi-drop link, but I can't
// bring myself to just type 2 in the code (even though I type 0 and 1 :-).
//
  static const int N_DEVICES = 2;

  SerialChannel ();
  SerialChannel (const DataRate& bps, const Time& delay);
  SerialChannel (const std::string& name,
                 const DataRate& bps, const Time& delay);

  void Attach (SerialPhy* phy);
  bool Propagate (Packet& p, SerialPhy *src);

  virtual uint32_t GetNDevices (void) const;
  virtual NetDevice *GetDevice (uint32_t i) const;


private:
  void TransmitCompleteEvent (Packet p, SerialPhy *src);

  DataRate      m_bps;
  Time          m_delay;

  int32_t       m_nDevices;

  enum WireState
    {
      INITIALIZING,
      IDLE,
      TRANSMITTING
    };

  class Link
  {
  public:
    Link() : m_state (INITIALIZING), m_src (0), m_dst (0) {}
    WireState        m_state;
    SerialPhy *m_src;
    SerialPhy *m_dst;
  };
    
  Link    m_link[N_DEVICES];
};

} // namespace ns3

#endif /* SERIAL_CHANNEL_H */
