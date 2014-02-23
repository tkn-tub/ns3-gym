/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Author:
 *  Tom Henderson <thomas.r.henderson@boeing.com>
 *  Tommaso Pecorella <tommaso.pecorella@unifi.it>
 *  Margherita Filippetti <morag87@gmail.com>
 */
#ifndef LR_WPAN_NET_DEVICE_H
#define LR_WPAN_NET_DEVICE_H

#include <ns3/net-device.h>
#include <ns3/traced-callback.h>
#include <ns3/lr-wpan-mac.h>

namespace ns3 {

class LrWpanPhy;
class LrWpanCsmaCa;
class SingleModelSpectrumChannel;
class SpectrumChannel;
class Node;

/**
 * \ingroup lr-wpan
 *
 * \brief Network layer to device interface
 *
 * The ns3::NetDevice includes IP-specific API such as GetMulticast(), and
 * the Send() and SendTo() methods do not map well the the 802.15.4 MAC
 * MCPS DataRequest primitive.  So, the basic design is to provide, as
 * much as makes sense, the class ns3::NetDevice API, but rely on the user
 * accessing the LrWpanMac pointer to make 802.15.4-specific API calls.
 * As such, this is really just an encapsulating class.
 */
class LrWpanNetDevice : public NetDevice
{
public:
  static TypeId GetTypeId (void);

  LrWpanNetDevice (void);
  virtual ~LrWpanNetDevice (void);

  void SetMac (Ptr<LrWpanMac> mac);
  void SetPhy (Ptr<LrWpanPhy> phy);
  void SetCsmaCa (Ptr<LrWpanCsmaCa> csmaca);
  void SetChannel (Ptr<SpectrumChannel> channel);
  /**
   * We disallow the conventional Send/SendFrom methods in this NetDevice,
   * preferring instead that the higher layer access the LrWpanMac object
   * directly (with its specific API).
   *
   * \return pointer to the LrWpanMac/Phy/Csmaca object
   */
  Ptr<LrWpanMac> GetMac (void) const;
  Ptr<LrWpanPhy> GetPhy (void) const;
  Ptr<LrWpanCsmaCa> GetCsmaCa (void) const;

  // From class NetDevice
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  /**
   * This method indirects to LrWpanMac::SetShortAddress ()
   */
  virtual void SetAddress (Address address);
  /**
   * This method indirects to LrWpanMac::SetShortAddress ()
   */
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
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

  virtual void SetReceiveCallback (ReceiveCallback cb);
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;

  void McpsDataIndication (McpsDataIndicationParams params, Ptr<Packet> pkt);

private:
  virtual void DoDispose (void);
  virtual void DoStart (void);
  void LinkUp (void);
  void LinkDown (void);
  Ptr<SpectrumChannel> DoGetChannel (void) const;
  void CompleteConfig (void);

  Ptr<LrWpanMac> m_mac;
  Ptr<LrWpanPhy> m_phy;
  Ptr<LrWpanCsmaCa> m_csmaca;
  Ptr<Node> m_node;
  bool m_configComplete;
  bool m_useAcks;

  bool m_linkUp;
  uint32_t m_ifIndex;

  TracedCallback<> m_linkChanges;
  ReceiveCallback m_receiveCallback;
};

} // namespace ns3

#endif /* LR_WPAN_NET_DEVICE_H */
