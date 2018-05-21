/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/llc-snap-header.h"
#include "ns3/channel.h"
#include "ns3/socket.h"
#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/net-device-queue-interface.h"
#include "wifi-net-device.h"
#include "wifi-phy.h"
#include "regular-wifi-mac.h"
#include "wifi-mac-queue.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiNetDevice");

NS_OBJECT_ENSURE_REGISTERED (WifiNetDevice);

TypeId
WifiNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<WifiNetDevice> ()
    .SetGroupName ("Wifi")
    .AddAttribute ("Mtu", "The MAC-level Maximum Transmission Unit",
                   UintegerValue (MAX_MSDU_SIZE - LLC_SNAP_HEADER_LENGTH),
                   MakeUintegerAccessor (&WifiNetDevice::SetMtu,
                                         &WifiNetDevice::GetMtu),
                   MakeUintegerChecker<uint16_t> (1,MAX_MSDU_SIZE - LLC_SNAP_HEADER_LENGTH))
    .AddAttribute ("Channel", "The channel attached to this device",
                   PointerValue (),
                   MakePointerAccessor (&WifiNetDevice::GetChannel),
                   MakePointerChecker<Channel> ())
    .AddAttribute ("Phy", "The PHY layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&WifiNetDevice::GetPhy,
                                        &WifiNetDevice::SetPhy),
                   MakePointerChecker<WifiPhy> ())
    .AddAttribute ("Mac", "The MAC layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&WifiNetDevice::GetMac,
                                        &WifiNetDevice::SetMac),
                   MakePointerChecker<WifiMac> ())
    .AddAttribute ("RemoteStationManager", "The station manager attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&WifiNetDevice::SetRemoteStationManager,
                                        &WifiNetDevice::GetRemoteStationManager),
                   MakePointerChecker<WifiRemoteStationManager> ())
  ;
  return tid;
}

WifiNetDevice::WifiNetDevice ()
  : m_configComplete (false)
{
  NS_LOG_FUNCTION_NOARGS ();
}

WifiNetDevice::~WifiNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
WifiNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_mac->Dispose ();
  m_phy->Dispose ();
  m_stationManager->Dispose ();
  m_mac = 0;
  m_phy = 0;
  m_stationManager = 0;
  m_queueInterface = 0;
  NetDevice::DoDispose ();
}

void
WifiNetDevice::DoInitialize (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_phy->Initialize ();
  m_mac->Initialize ();
  m_stationManager->Initialize ();
  NetDevice::DoInitialize ();
}

void
WifiNetDevice::CompleteConfig (void)
{
  if (m_mac == 0
      || m_phy == 0
      || m_stationManager == 0
      || m_node == 0
      || m_configComplete)
    {
      return;
    }
  m_mac->SetWifiRemoteStationManager (m_stationManager);
  m_mac->SetWifiPhy (m_phy);
  m_mac->SetForwardUpCallback (MakeCallback (&WifiNetDevice::ForwardUp, this));
  m_mac->SetLinkUpCallback (MakeCallback (&WifiNetDevice::LinkUp, this));
  m_mac->SetLinkDownCallback (MakeCallback (&WifiNetDevice::LinkDown, this));
  m_stationManager->SetupPhy (m_phy);
  m_stationManager->SetupMac (m_mac);
  m_configComplete = true;
}

void
WifiNetDevice::NotifyNewAggregate (void)
{
  NS_LOG_FUNCTION (this);
  if (m_queueInterface == 0)
    {
      Ptr<NetDeviceQueueInterface> ndqi = this->GetObject<NetDeviceQueueInterface> ();
      //verify that it's a valid netdevice queue interface and that
      //the netdevice queue interface was not set before
      if (ndqi != 0)
        {
          m_queueInterface = ndqi;
          // register the select queue callback
          m_queueInterface->SetSelectQueueCallback (MakeCallback (&WifiNetDevice::SelectQueue, this));
          m_queueInterface->SetLateTxQueuesCreation (true);
          FlowControlConfig ();
        }
    }
  NetDevice::NotifyNewAggregate ();
}

void
WifiNetDevice::FlowControlConfig (void)
{
  if (m_mac == 0 || m_queueInterface == 0)
    {
      return;
    }

  Ptr<RegularWifiMac> mac = DynamicCast<RegularWifiMac> (m_mac);
  if (mac == 0)
    {
      NS_LOG_WARN ("Flow control is only supported by RegularWifiMac");
      return;
    }

  BooleanValue qosSupported;
  mac->GetAttributeFailSafe ("QosSupported", qosSupported);
  PointerValue ptr;
  Ptr<WifiMacQueue> wmq;
  if (qosSupported.Get ())
    {
      m_queueInterface->SetTxQueuesN (4);
      m_queueInterface->CreateTxQueues ();

      mac->GetAttributeFailSafe ("BE_Txop", ptr);
      wmq = ptr.Get<QosTxop> ()->GetWifiMacQueue ();
      m_queueInterface->ConnectQueueTraces<WifiMacQueueItem> (wmq, 0);

      mac->GetAttributeFailSafe ("BK_Txop", ptr);
      wmq = ptr.Get<QosTxop> ()->GetWifiMacQueue ();
      m_queueInterface->ConnectQueueTraces<WifiMacQueueItem> (wmq, 1);

      mac->GetAttributeFailSafe ("VI_Txop", ptr);
      wmq = ptr.Get<QosTxop> ()->GetWifiMacQueue ();
      m_queueInterface->ConnectQueueTraces<WifiMacQueueItem> (wmq, 2);

      mac->GetAttributeFailSafe ("VO_Txop", ptr);
      wmq = ptr.Get<QosTxop> ()->GetWifiMacQueue ();
      m_queueInterface->ConnectQueueTraces<WifiMacQueueItem> (wmq, 3);
    }
  else
    {
      m_queueInterface->CreateTxQueues ();

      mac->GetAttributeFailSafe ("Txop", ptr);
      wmq = ptr.Get<Txop> ()->GetWifiMacQueue ();
      m_queueInterface->ConnectQueueTraces<WifiMacQueueItem> (wmq, 0);
    }
}

void
WifiNetDevice::SetMac (const Ptr<WifiMac> mac)
{
  m_mac = mac;
  CompleteConfig ();
  FlowControlConfig ();
}

void
WifiNetDevice::SetPhy (const Ptr<WifiPhy> phy)
{
  m_phy = phy;
  CompleteConfig ();
}

void
WifiNetDevice::SetRemoteStationManager (const Ptr<WifiRemoteStationManager> manager)
{
  m_stationManager = manager;
  CompleteConfig ();
}

Ptr<WifiMac>
WifiNetDevice::GetMac (void) const
{
  return m_mac;
}

Ptr<WifiPhy>
WifiNetDevice::GetPhy (void) const
{
  return m_phy;
}

Ptr<WifiRemoteStationManager>
WifiNetDevice::GetRemoteStationManager (void) const
{
  return m_stationManager;
}

void
WifiNetDevice::SetIfIndex (const uint32_t index)
{
  m_ifIndex = index;
}

uint32_t
WifiNetDevice::GetIfIndex (void) const
{
  return m_ifIndex;
}

Ptr<Channel>
WifiNetDevice::GetChannel (void) const
{
  return m_phy->GetChannel ();
}

void
WifiNetDevice::SetAddress (Address address)
{
  m_mac->SetAddress (Mac48Address::ConvertFrom (address));
}

Address
WifiNetDevice::GetAddress (void) const
{
  return m_mac->GetAddress ();
}

bool
WifiNetDevice::SetMtu (const uint16_t mtu)
{
  if (mtu > MAX_MSDU_SIZE - LLC_SNAP_HEADER_LENGTH)
    {
      return false;
    }
  m_mtu = mtu;
  return true;
}

uint16_t
WifiNetDevice::GetMtu (void) const
{
  return m_mtu;
}

bool
WifiNetDevice::IsLinkUp (void) const
{
  return m_phy != 0 && m_linkUp;
}

void
WifiNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  m_linkChanges.ConnectWithoutContext (callback);
}

bool
WifiNetDevice::IsBroadcast (void) const
{
  return true;
}

Address
WifiNetDevice::GetBroadcast (void) const
{
  return Mac48Address::GetBroadcast ();
}

bool
WifiNetDevice::IsMulticast (void) const
{
  return true;
}

Address
WifiNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  return Mac48Address::GetMulticast (multicastGroup);
}

Address WifiNetDevice::GetMulticast (Ipv6Address addr) const
{
  return Mac48Address::GetMulticast (addr);
}

bool
WifiNetDevice::IsPointToPoint (void) const
{
  return false;
}

bool
WifiNetDevice::IsBridge (void) const
{
  return false;
}

bool
WifiNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << dest << protocolNumber);
  NS_ASSERT (Mac48Address::IsMatchingType (dest));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_mac->NotifyTx (packet);
  m_mac->Enqueue (packet, realTo);
  return true;
}

Ptr<Node>
WifiNetDevice::GetNode (void) const
{
  return m_node;
}

void
WifiNetDevice::SetNode (const Ptr<Node> node)
{
  m_node = node;
  CompleteConfig ();
}

bool
WifiNetDevice::NeedsArp (void) const
{
  return true;
}

void
WifiNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_forwardUp = cb;
}

void
WifiNetDevice::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << from << to);
  LlcSnapHeader llc;
  NetDevice::PacketType type;
  if (to.IsBroadcast ())
    {
      type = NetDevice::PACKET_BROADCAST;
    }
  else if (to.IsGroup ())
    {
      type = NetDevice::PACKET_MULTICAST;
    }
  else if (to == m_mac->GetAddress ())
    {
      type = NetDevice::PACKET_HOST;
    }
  else
    {
      type = NetDevice::PACKET_OTHERHOST;
    }

  if (type != NetDevice::PACKET_OTHERHOST)
    {
      m_mac->NotifyRx (packet);
      packet->RemoveHeader (llc);
      m_forwardUp (this, packet, llc.GetType (), from);
    }
  else
    {
      packet->RemoveHeader (llc);
    }

  if (!m_promiscRx.IsNull ())
    {
      m_mac->NotifyPromiscRx (packet);
      m_promiscRx (this, packet, llc.GetType (), from, to, type);
    }
}

void
WifiNetDevice::LinkUp (void)
{
  m_linkUp = true;
  m_linkChanges ();
}

void
WifiNetDevice::LinkDown (void)
{
  m_linkUp = false;
  m_linkChanges ();
}

bool
WifiNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << source << dest << protocolNumber);
  NS_ASSERT (Mac48Address::IsMatchingType (dest));
  NS_ASSERT (Mac48Address::IsMatchingType (source));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);
  Mac48Address realFrom = Mac48Address::ConvertFrom (source);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_mac->NotifyTx (packet);
  m_mac->Enqueue (packet, realTo, realFrom);

  return true;
}

void
WifiNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  m_promiscRx = cb;
  m_mac->SetPromisc ();
}

bool
WifiNetDevice::SupportsSendFrom (void) const
{
  return m_mac->SupportsSendFrom ();
}

uint8_t
WifiNetDevice::SelectQueue (Ptr<QueueItem> item) const
{
  NS_LOG_FUNCTION (this << item);

  NS_ASSERT (m_queueInterface != 0);

  if (m_queueInterface->GetNTxQueues () == 1)
    {
      return 0;
    }

  uint8_t dscp, priority = 0;
  if (item->GetUint8Value (QueueItem::IP_DSFIELD, dscp))
    {
      // if the QoS map element is implemented, it should be used here
      // to set the priority.
      // User priority is set to the three most significant bits of the DS field
      priority = dscp >> 5;
    }

  // replace the priority tag
  SocketPriorityTag priorityTag;
  priorityTag.SetPriority (priority);
  item->GetPacket ()->ReplacePacketTag (priorityTag);

  // if the admission control were implemented, here we should check whether
  // the access category assigned to the packet should be downgraded

  return static_cast<uint8_t> (QosUtilsMapTidToAc (priority));
}

} //namespace ns3
