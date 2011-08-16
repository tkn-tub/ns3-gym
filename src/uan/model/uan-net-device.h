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

#ifndef UAN_NET_DEVICE_H
#define UAN_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/pointer.h"
#include "ns3/traced-callback.h"
#include "uan-address.h"
#include <list>

namespace ns3 {

class UanChannel;
class UanPhy;
class UanMac;
class UanTransducer;

/**
 * \defgroup uan UAN Models
 * This section documents the API of the ns-3 UAN module. For a generic functional description, please refer to the ns-3 manual.
 */

/**
 * \ingroup uan
 * \class UanNetDevice
 *
 * \brief Net device for UAN models
 */
class UanNetDevice : public NetDevice
{
public:
  /**
   * \brief UanPhyList is defined as a standard template lib list of UanPhy objects
   */
  typedef std::list<Ptr<UanPhy> > UanPhyList;
  /**
   * \brief UanTransducerList is a standard template lib list of UanTransducer objects
   */
  typedef std::list<Ptr<UanTransducer> > UanTransducerList;

  static TypeId GetTypeId (void);

  UanNetDevice ();
  virtual ~UanNetDevice ();

  /**
   * \param mac Set MAC layer for this device
   */
  void SetMac (Ptr<UanMac> mac);

  /**
   * \param phy Set PHY layer for this device
   */
  void SetPhy (Ptr<UanPhy> phy);

  /**
   * \param channel The channel this device is attached to
   */
  void SetChannel (Ptr<UanChannel> channel);

  /**
   * \returns MAC this device is using
   */
  Ptr<UanMac> GetMac (void) const;

  /**
   * \returns PHY this device is using
   */
  Ptr<UanPhy> GetPhy (void) const;

  /**
   * \returns Transducer in net device
   */
  Ptr<UanTransducer> GetTransducer (void) const;
  /**
   * \param trans Transducer to use in this net device
   */
  void SetTransducer (Ptr<UanTransducer> trans);

  /**
   * Clears all pointer references
   */
  void Clear (void);

  void SetSleepMode (bool sleep);

  // Purely virtual functions from base class
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual Address GetMulticast (Ipv6Address addr) const;
  virtual bool IsBridge (void) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual void SetAddress (Address address);
private:
  virtual void ForwardUp (Ptr<Packet> pkt, const UanAddress &src);
  Ptr<UanChannel> DoGetChannel (void) const;

  Ptr<UanTransducer> m_trans;
  Ptr<Node> m_node;
  Ptr<UanChannel> m_channel;
  Ptr<UanMac> m_mac;
  Ptr<UanPhy> m_phy;

  std::string m_name;
  uint32_t m_ifIndex;
  uint16_t m_mtu;
  bool m_linkup;
  TracedCallback<> m_linkChanges;
  ReceiveCallback m_forwardUp;

  TracedCallback<Ptr<const Packet>, UanAddress> m_rxLogger;
  TracedCallback<Ptr<const Packet>, UanAddress> m_txLogger;

  bool m_cleared;

protected:
  virtual void DoDispose ();
};

} // namespace ns3

#endif /* UAN_NET_DEVICE_H */
