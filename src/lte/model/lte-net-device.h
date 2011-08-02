/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#ifndef LTE_NET_DEVICE_H
#define LTE_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/event-id.h"
#include "ns3/mac48-address.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "lte-phy.h"
#include "ideal-control-messages.h"

namespace ns3 {

class Node;
class Packet;
class PacketBurst;
class RadioBearerInstance;
class RrcEntity;

/**
 * \defgroup lte LTE Models
 *
 */

/**
 * \ingroup lte
 *
 * LteNetDevice provides  basic implementation for all LTE network devices
 */
class LteNetDevice : public NetDevice
{
public:
  static TypeId GetTypeId (void);

  LteNetDevice (void);
  virtual ~LteNetDevice (void);

  virtual void DoDispose (void);

  /**
   * \brief set the callback used to instruct the lower layer to start a TX
   * \param c
   */
  void SetGenericPhyTxStartCallback (GenericPhyTxStartCallback c);


  /**
   * \param phy the phy layer to use.
   */
  void SetPhy (Ptr<LtePhy> phy);
  /**
   * \returns a pointer to the physical layer.
   */
  Ptr<LtePhy> GetPhy (void) const;


  /**
   * \brief Set the RRC entity
   * \param rrc the RRC entity
   */
  void SetRrcEntity (Ptr<RrcEntity> rrc);
  /**
   * \brief Get the RRC entity
   * \return the pointer to the RRC entity
   */
  Ptr<RrcEntity> GetRrcEntity (void);


  // inherited from NetDevice
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual void SetAddress (Address address);
  virtual Address GetAddress (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool IsBridge (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual Address GetMulticast (Ipv4Address addr) const;
  virtual Address GetMulticast (Ipv6Address addr) const;
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);

  /**
   * \brief This method is called as soon as the net device is created. 
   * It can be used to start some functionalities of the considered device. For example, 
   * for the eNB device it starts the schedule of LTE Frames.
   */
  virtual void Start (void) = 0;
  /**
   * Ends the run of this device
   */
  virtual void Stop (void) = 0;

  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual bool SupportsSendFrom (void) const;

  /**
   * \brief Receive the packet from the phy layer
   * \param p the received packet
   */
  void Receive (Ptr<Packet> p);
  /**
   * \brief Forward packet to the uppar layer.
   * If is called by DoReceive method
   * \param packet packet sent from Network Device to the upper layer
   * \param source source mac address
   * \param dest mac address of the destination
   */
  void ForwardUp (Ptr<Packet> packet, const Mac48Address &source, const Mac48Address &dest);
  /**
   * \brief Forward packet to the uppar layer.
   * If is called by DoReceive method
   * \param packet packet sent from Network Device to the upper layer
   */
  void ForwardUp (Ptr<Packet> packet);

  /**
   * \brief Set packets to send 
   * \param p the burst of packets to send 
   */ 
  void SetPacketToSend (Ptr<PacketBurst> p);
  /**
   * \brief Get packets to send 
   * \return the pointer to the burst of packets to send 
   */
  Ptr<PacketBurst> GetPacketToSend (void);

  /**
   * \brief Start packet transmission.
   */
  virtual void StartTransmission (void) = 0;
  /**
   * \brief Send packet/packets to the physical layer
   * \param p packet/packets to be sent
   */
  virtual bool SendPacket (Ptr<PacketBurst> p) = 0;


private:
  LteNetDevice (const LteNetDevice &);
  LteNetDevice & operator= (const LteNetDevice &);

  static const uint16_t MAX_MSDU_SIZE = 1500;

  virtual bool DoSend (Ptr<Packet> packet,
                       const Mac48Address& source,
                       const Mac48Address& dest,
                       uint16_t protocolNumber) = 0;
  virtual void DoReceive (Ptr<Packet> p) = 0;


  Ptr<Node> m_node;
  Ptr<LtePhy> m_phy;

  Ptr<RrcEntity> m_rrcEntity;

  TracedCallback<Ptr<const Packet> > m_macTxTrace;
  TracedCallback<Ptr<const Packet> > m_macTxDropTrace;
  TracedCallback<Ptr<const Packet> > m_macPromiscRxTrace;
  TracedCallback<Ptr<const Packet> > m_macRxTrace;

  NetDevice::ReceiveCallback m_rxCallback;
  NetDevice::PromiscReceiveCallback m_promiscRxCallback;
  GenericPhyTxStartCallback m_phyMacTxStartCallback;
  TracedCallback<> m_linkChangeCallbacks;

  uint32_t m_ifIndex;
  bool m_linkUp;
  mutable uint16_t m_mtu;

  Mac48Address m_address;

  Ptr<PacketBurst> m_packetToSend;
};


} // namespace ns3

#endif /* LTE_NET_DEVICE_H */
