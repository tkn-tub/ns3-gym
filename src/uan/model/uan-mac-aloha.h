/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef UANMACALOHA_H
#define UANMACALOHA_H

#include "uan-mac.h"
#include "uan-address.h"

namespace ns3
{


class UanPhy;
class UanTxMode;

/**
 * \class UanMacAloha
 * \brief ALOHA MAC Protocol
 *
 * The simplest MAC protocol for wireless networks.  Packets enqueued
 * are immediately transmitted.  This MAC attaches a UanHeaderCommon
 * to outgoing packets for address information.  (The type field is not used)
 */
class UanMacAloha : public UanMac
{
public:
  UanMacAloha ();
  virtual ~UanMacAloha ();
  static TypeId GetTypeId (void);


  //Inheritted functions
  Address GetAddress (void);
  virtual void SetAddress (UanAddress addr);
  virtual bool Enqueue (Ptr<Packet> pkt, const Address &dest, uint16_t protocolNumber);
  virtual void SetForwardUpCb (Callback<void, Ptr<Packet>, const UanAddress& > cb);
  virtual void AttachPhy (Ptr<UanPhy> phy);
  virtual Address GetBroadcast (void) const;
  virtual void Clear (void);

private:
  UanAddress m_address;
  Ptr<UanPhy> m_phy;
  Callback<void, Ptr<Packet>, const UanAddress& > m_forUpCb;
  bool m_cleared;

  /**
   * \brief Receive packet from lower layer (passed to PHY as callback)
   * \param pkt Packet being received
   * \param sinr SINR of received packet
   * \param txMode Mode of received packet
   */
  void RxPacketGood (Ptr<Packet> pkt, double sinr, UanTxMode txMode);

  /**
   * \brief Packet received at lower layer in error
   * \param pkt Packet received in error
   * \param sinr SINR of received packet
   */
  void RxPacketError (Ptr<Packet> pkt, double sinr);
protected:
  virtual void DoDispose ();
};

}

#endif // UANMACALOHA_H
