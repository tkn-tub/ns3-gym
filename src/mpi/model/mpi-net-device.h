/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: George Riley <riley@ece.gatech.edu>
 */

// Provides a mixin interface to allow MPI-compatible NetDevices to inherit

#ifndef NS3_MPI_NET_DEVICE_H
#define NS3_MPI_NET_DEVICE_H

#include "ns3/packet.h"

namespace ns3 {

/**
 * Class to mixin to a NetDevice if it supports MPI capability
 * 
 * Subclass must implement DoMpiReceive to direct it to the device's
 * normal Receive() method.
 */
class MpiNetDevice
{
public:
  virtual ~MpiNetDevice();
  /**
   * 
   * Receive a packet 
   *
   * \param p Ptr to the received packet.
   */
  void MpiReceive (Ptr<Packet> p);
protected:
  virtual void DoMpiReceive (Ptr<Packet> p) = 0;
};

} // namespace ns3

#endif /* NS3_MPI_NET_DEVICE_H */
