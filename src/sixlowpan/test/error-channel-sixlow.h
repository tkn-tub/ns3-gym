/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Universita' di Firenze, Italy
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
#ifndef ERROR_CHANNEL_SIXLOW_H
#define ERROR_CHANNEL_SIXLOW_H

#include "ns3/channel.h"
#include "ns3/simple-channel.h"
#include "ns3/error-model.h"
#include "ns3/mac48-address.h"
#include "ns3/nstime.h"
#include <vector>

namespace ns3 {

class SimpleNetDevice;
class Packet;

/**
 * \ingroup channel
 * \brief A Error channel, introducing deterministic delays on even/odd packets. Used for testing
 */
class ErrorChannelSixlow : public SimpleChannel
{
public:
  static TypeId GetTypeId (void);
  ErrorChannelSixlow ();

  virtual void Send (Ptr<Packet> p, uint16_t protocol, Mac48Address to, Mac48Address from,
                     Ptr<SimpleNetDevice> sender);

  virtual void Add (Ptr<SimpleNetDevice> device);

  // inherited from ns3::Channel
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  /**
   * \brief Set the delay for the odd packets (even ones are not delayed)
   * \param delay Delay for the odd packets.
   */
  void SetJumpingTime (Time delay);

  /**
   * \brief Set if the odd packets are delayed (even ones are not delayed ever)
   * \param mode true if the odd packets should be delayed.
   */
  void SetJumpingMode (bool mode);

  /**
   * \brief Set the delay for the odd duplicate packets (even ones are not duplicated)
   * \param delay Delay for the odd packets.
   */
  void SetDuplicateTime (Time delay);

  /**
   * \brief Set if the odd packets are duplicated (even ones are not duplicated ever)
   * \param mode true if the odd packets should be duplicated.
   */
  void SetDuplicateMode (bool mode);

private:
  std::vector<Ptr<SimpleNetDevice> > m_devices;
  Time m_jumpingTime;
  uint8_t m_jumpingState;
  bool m_jumping;
  Time m_duplicateTime;
  bool m_duplicate;
  uint8_t m_duplicateState;
};

class BinaryErrorSixlowModel : public ErrorModel
{
public:
  static TypeId GetTypeId (void);

  BinaryErrorSixlowModel ();
  virtual ~BinaryErrorSixlowModel ();
  void Reset (void);

private:
  virtual bool DoCorrupt (Ptr<Packet> p);
  virtual void DoReset (void);

  uint8_t m_counter;

};

} // namespace ns3

#endif /* ERROR_CHANNEL_SIXLOW_H */
