/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#ifndef UAN_MAC_H
#define UAN_MAC_H

#include "ns3/address.h"
#include "ns3/object.h"
#include "ns3/packet.h"

#include "ns3/address.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

namespace ns3 {

class UanPhy;
class UanChannel;
class UanNetDevice;
class UanTransducer;
class UanAddress;



/**
 * \class UanMac
 *
 * \brief Virtual base class for all UAN MAC protocols
 */
class UanMac : public Object
{
public:
  static TypeId GetTypeId (void);

  /**
   * \returns MAC Address
   */
  virtual Address GetAddress (void) = 0;

  /**
   * \param addr UanAddress for this MAC
   */
  virtual void SetAddress (UanAddress addr) = 0;

  /**
   * Enqueue packet to be transmitted
   * \param pkt Packet to be transmitted
   * \param dest Destination address
   * \param protocolNumber Protocol #.  Usage varies by MAC
   */
  virtual bool Enqueue (Ptr<Packet> pkt, const Address &dest, uint16_t protocolNumber) = 0;
  /**
   * \param cb  Callback to be called when a packet is forwarded up to higher layer
   */
  virtual void SetForwardUpCb (Callback<void, Ptr<Packet>, const UanAddress&> cb) = 0;

  /**
   * Attach PHY layer to this MAC.
   * Some MACs may be designed to work with multiple PHY
   * layers.  Others may only work with one.
   *
   * \param phy Phy layer to attach to this MAC
   *
   */
  virtual void AttachPhy (Ptr<UanPhy> phy) = 0;

  /**
   * \returns Broadcast address
   */
  virtual Address GetBroadcast (void) const = 0;

  /**
   * Clears all pointer references
   */
  virtual void Clear (void) = 0;

};

}

#endif /* UAN_MAC_H */
