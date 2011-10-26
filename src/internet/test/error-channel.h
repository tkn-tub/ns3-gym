/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Universita' di Firenze, Italy
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
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */
#ifndef ERROR_CHANNEL_H
#define ERROR_CHANNEL_H

#include "ns3/channel.h"
#include "ns3/error-model.h"
#include "ns3/mac48-address.h"
#include "ns3/nstime.h"
#include <vector>

namespace ns3 {

class ErrorNetDevice;
class Packet;

/**
 * \ingroup channel
 * \brief A Error channel, introducing deterministic delays on even/odd packets. Used for testing
 */
class ErrorChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  ErrorChannel ();

  void Send (Ptr<Packet> p, uint16_t protocol, Mac48Address to, Mac48Address from,
	     Ptr<ErrorNetDevice> sender);

  void Add (Ptr<ErrorNetDevice> device);

  // inherited from ns3::Channel
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  /**
   * \brief Set the delay for the odd packets (even ones are not delayed)
   * \param delay Delay for the odd packets.
   */
  void SetJumpingTime(Time delay);

  /**
   * \brief Set if the odd packets are delayed (even ones are not delayed ever)
   * \param mode true if the odd packets should be delayed.
   */
void SetJumpingMode(bool mode);

private:
  std::vector<Ptr<ErrorNetDevice> > m_devices;
  Time jumpingTime;
  uint8_t jumpingState;
  bool jumping;

};

class BinaryErrorModel : public ErrorModel
{
public:
  static TypeId GetTypeId (void);

  BinaryErrorModel ();
  virtual ~BinaryErrorModel ();
  void Reset (void);

private:
  virtual bool DoCorrupt (Ptr<Packet> p);
  virtual void DoReset (void);

  uint8_t counter;

};

} // namespace ns3

#endif /* ERROR_CHANNEL_H */
