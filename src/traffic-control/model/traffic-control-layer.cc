/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Natale Patriciello <natale.patriciello@gmail.com>
 *               2016 Stefano Avallone <stavallo@unina.it>
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

#include "traffic-control-layer.h"
#include "ns3/log.h"
#include "ns3/object-map.h"
#include "ns3/packet.h"
#include "ns3/socket.h"
#include "ns3/queue-disc.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TrafficControlLayer");

NS_OBJECT_ENSURE_REGISTERED (TrafficControlLayer);

TypeId
TrafficControlLayer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TrafficControlLayer")
    .SetParent<Object> ()
    .SetGroupName ("TrafficControl")
    .AddConstructor<TrafficControlLayer> ()
    .AddAttribute ("RootQueueDiscList", "The list of root queue discs associated to this Traffic Control layer.",
                   ObjectMapValue (),
                   MakeObjectMapAccessor (&TrafficControlLayer::GetNDevices,
                                          &TrafficControlLayer::GetRootQueueDiscOnDeviceByIndex),
                   MakeObjectMapChecker<QueueDisc> ())
  ;
  return tid;
}

TypeId
TrafficControlLayer::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

TrafficControlLayer::TrafficControlLayer ()
  : Object ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
TrafficControlLayer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = 0;
  m_handlers.clear ();
  m_netDevices.clear ();
  Object::DoDispose ();
}

void
TrafficControlLayer::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  std::map<Ptr<NetDevice>, NetDeviceInfo>::iterator ndi;
  for (ndi = m_netDevices.begin (); ndi != m_netDevices.end (); ndi++)
    {
      if (ndi->second.rootQueueDisc)
        {
          Ptr<NetDeviceQueueInterface> devQueueIface = ndi->second.ndqi;
          NS_ASSERT (devQueueIface);

          // set the wake callbacks on netdevice queues
           if (ndi->second.rootQueueDisc->GetWakeMode () == QueueDisc::WAKE_ROOT)
            {
              for (uint32_t i = 0; i < devQueueIface->GetTxQueuesN (); i++)
                {
                  devQueueIface->GetTxQueue (i)->SetWakeCallback (MakeCallback (&QueueDisc::Run, ndi->second.rootQueueDisc));
                  ndi->second.queueDiscsToWake.push_back (ndi->second.rootQueueDisc);
                }
            }
          else if (ndi->second.rootQueueDisc->GetWakeMode () == QueueDisc::WAKE_CHILD)
            {
              NS_ASSERT_MSG (ndi->second.rootQueueDisc->GetNQueueDiscClasses () == devQueueIface->GetTxQueuesN (),
                             "The number of child queue discs does not match the number of netdevice queues");
              for (uint32_t i = 0; i < devQueueIface->GetTxQueuesN (); i++)
                {
                  devQueueIface->GetTxQueue (i)->SetWakeCallback (MakeCallback (&QueueDisc::Run,
                                                                  ndi->second.rootQueueDisc->GetQueueDiscClass (i)->GetQueueDisc ()));
                  ndi->second.queueDiscsToWake.push_back (ndi->second.rootQueueDisc->GetQueueDiscClass (i)->GetQueueDisc ());
                }
            }

          // initialize the queue disc
          ndi->second.rootQueueDisc->Initialize ();
        }
    }
  Object::DoInitialize ();
}

void
TrafficControlLayer::SetupDevice (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);

  // ensure this setup is done just once. SetupDevice is called by Ipv4L3Protocol
  // and Ipv6L3Protocol when they add an interface, thus it might be called twice
  // in case of dual stack nodes. Also, SetupDevice might be called twice if the
  // tc helper is invoked (to install a queue disc) before the creation of the
  // Ipv{4,6}Interface, since SetRootQueueDiscOnDevice calls SetupDevice
  if (device->GetObject<NetDeviceQueueInterface> ())
    {
      NS_LOG_DEBUG ("The setup for this device has been already done.");
      return;
    }

  // create a NetDeviceQueueInterface object and aggregate it to the device
  Ptr<NetDeviceQueueInterface> devQueueIface = CreateObject<NetDeviceQueueInterface> ();
  device->AggregateObject (devQueueIface);

  // create an entry in the m_netDevices map for this device
  NS_ASSERT_MSG (m_netDevices.find (device) == m_netDevices.end (), "This is a bug,"
                 << "  SetupDevice only can insert an entry in the m_netDevices map");

  NetDeviceInfo entry = {0, devQueueIface, QueueDiscVector ()};
  m_netDevices[device] = entry;
}

void
TrafficControlLayer::RegisterProtocolHandler (Node::ProtocolHandler handler,
                                              uint16_t protocolType, Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << protocolType << device);

  struct ProtocolHandlerEntry entry;
  entry.handler = handler;
  entry.protocol = protocolType;
  entry.device = device;
  entry.promiscuous = false;

  m_handlers.push_back (entry);

  NS_LOG_DEBUG ("Handler for NetDevice: " << device << " registered for protocol " <<
                protocolType << ".");
}

void
TrafficControlLayer::SetRootQueueDiscOnDevice (Ptr<NetDevice> device, Ptr<QueueDisc> qDisc)
{
  NS_LOG_FUNCTION (this << device << qDisc);

  std::map<Ptr<NetDevice>, NetDeviceInfo>::iterator ndi = m_netDevices.find (device);

  if (ndi == m_netDevices.end ())
    {
      // SetupDevice has not been called yet. This may happen when the tc helper is
      // invoked (to install a queue disc) before the creation of the Ipv{4,6}Interface.
      // Since queue discs require that a netdevice queue interface is aggregated
      // to the device, call SetupDevice
      SetupDevice (device);
      ndi = m_netDevices.find (device);
      NS_ASSERT (ndi != m_netDevices.end ());
    }

  NS_ASSERT_MSG (ndi->second.rootQueueDisc == 0, "Cannot install a root queue disc on a "
                  << "device already having one. Delete the existing queue disc first.");
  ndi->second.rootQueueDisc = qDisc;
}

Ptr<QueueDisc>
TrafficControlLayer::GetRootQueueDiscOnDevice (Ptr<NetDevice> device) const
{
  NS_LOG_FUNCTION (this << device);

  std::map<Ptr<NetDevice>, NetDeviceInfo>::const_iterator ndi = m_netDevices.find (device);

  if (ndi == m_netDevices.end ())
    {
      return 0;
    }
  return ndi->second.rootQueueDisc;
}

Ptr<QueueDisc>
TrafficControlLayer::GetRootQueueDiscOnDeviceByIndex (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  return GetRootQueueDiscOnDevice (m_node->GetDevice (index));
}

void
TrafficControlLayer::DeleteRootQueueDiscOnDevice (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);

  std::map<Ptr<NetDevice>, NetDeviceInfo>::iterator ndi = m_netDevices.find (device);

  NS_ASSERT_MSG (ndi != m_netDevices.end () && ndi->second.rootQueueDisc != 0, "No root queue disc"
                 << " installed on device " << device);

  // remove the root queue disc
  ndi->second.rootQueueDisc = 0;
  ndi->second.queueDiscsToWake.clear ();
}

void
TrafficControlLayer::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
}

void
TrafficControlLayer::NotifyNewAggregate ()
{
  NS_LOG_FUNCTION (this);
  if (m_node == 0)
    {
      Ptr<Node> node = this->GetObject<Node> ();
      //verify that it's a valid node and that
      //the node was not set before
      if (node != 0)
        {
          this->SetNode (node);
        }
    }
  Object::NotifyNewAggregate ();
}

uint32_t
TrafficControlLayer::GetNDevices (void) const
{
  return m_node->GetNDevices ();
}


void
TrafficControlLayer::Receive (Ptr<NetDevice> device, Ptr<const Packet> p,
                              uint16_t protocol, const Address &from, const Address &to,
                              NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << device << p << protocol << from << to << packetType);

  bool found = false;

  for (ProtocolHandlerList::iterator i = m_handlers.begin ();
       i != m_handlers.end (); i++)
    {
      if (i->device == 0
          || (i->device != 0 && i->device == device))
        {
          if (i->protocol == 0
              || i->protocol == protocol)
            {
              NS_LOG_DEBUG ("Found handler for packet " << p << ", protocol " <<
                            protocol << " and NetDevice " << device <<
                            ". Send packet up");
              i->handler (device, p, protocol, from, to, packetType);
              found = true;
            }
        }
    }

  if (! found)
    {
      NS_FATAL_ERROR ("Handler for protocol " << p << " and device " << device <<
                      " not found. It isn't forwarded up; it dies here.");
    }
}

void
TrafficControlLayer::Send (Ptr<NetDevice> device, Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << device << item);

  NS_LOG_DEBUG ("Send packet to device " << device << " protocol number " <<
                item->GetProtocol ());

  std::map<Ptr<NetDevice>, NetDeviceInfo>::iterator ndi = m_netDevices.find (device);
  NS_ASSERT (ndi != m_netDevices.end ());
  Ptr<NetDeviceQueueInterface> devQueueIface = ndi->second.ndqi;
  NS_ASSERT (devQueueIface);

  // determine the transmission queue of the device where the packet will be enqueued
  uint8_t txq = devQueueIface->GetSelectedQueue (item);
  NS_ASSERT (txq < devQueueIface->GetTxQueuesN ());

  if (ndi->second.rootQueueDisc == 0)
    {
      // The device has no attached queue disc, thus add the header to the packet and
      // send it directly to the device if the selected queue is not stopped
      if (!devQueueIface->GetTxQueue (txq)->IsStopped ())
        {
          item->AddHeader ();
          // a single queue device makes no use of the priority tag
          if (devQueueIface->GetTxQueuesN () == 1)
            {
              SocketPriorityTag priorityTag;
              item->GetPacket ()->RemovePacketTag (priorityTag);
            }
          device->Send (item->GetPacket (), item->GetAddress (), item->GetProtocol ());
        }
    }
  else
    {
      // Enqueue the packet in the queue disc associated with the netdevice queue
      // selected for the packet and try to dequeue packets from such queue disc
      item->SetTxQueueIndex (txq);

      Ptr<QueueDisc> qDisc = ndi->second.queueDiscsToWake[txq];
      NS_ASSERT (qDisc);
      qDisc->Enqueue (item);
      qDisc->Run ();
    }
}

} // namespace ns3
