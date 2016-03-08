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
#include "ns3/object-vector.h"
#include "ns3/packet.h"
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
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&TrafficControlLayer::m_rootQueueDiscs),
                   MakeObjectVectorChecker<QueueDisc> ())
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
  m_rootQueueDiscs.clear ();
  m_handlers.clear ();
  m_netDeviceQueueToQueueDiscMap.clear ();
  Object::DoDispose ();
}

void
TrafficControlLayer::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  for (uint32_t j = 0; j < m_rootQueueDiscs.size (); j++)
    {
      if (m_rootQueueDiscs[j])
        {
          Ptr<NetDevice> device = m_node->GetDevice (j);
          // NetDevices supporting flow control can set the number of device transmission
          // queues through the NetDevice queue interface during initialization. Thus,
          // ensure that the device has completed initialization
          device->Initialize ();

          std::map<Ptr<NetDevice>, NetDeviceInfo>::iterator qdMap = m_netDeviceQueueToQueueDiscMap.find (device);
          NS_ASSERT (qdMap != m_netDeviceQueueToQueueDiscMap.end ());
          Ptr<NetDeviceQueueInterface> devQueueIface = qdMap->second.first;
          NS_ASSERT (devQueueIface);

          devQueueIface->SetQueueDiscInstalled (true);

          // set the wake callbacks on netdevice queues
           if (m_rootQueueDiscs[j]->GetWakeMode () == QueueDisc::WAKE_ROOT)
            {
              for (uint32_t i = 0; i < devQueueIface->GetTxQueuesN (); i++)
                {
                  devQueueIface->GetTxQueue (i)->SetWakeCallback (MakeCallback (&QueueDisc::Run, m_rootQueueDiscs[j]));
                  qdMap->second.second.push_back (m_rootQueueDiscs[j]);
                }
            }
          else if (m_rootQueueDiscs[j]->GetWakeMode () == QueueDisc::WAKE_CHILD)
            {
              NS_ASSERT_MSG (m_rootQueueDiscs[j]->GetNQueueDiscClasses () == devQueueIface->GetTxQueuesN (),
                             "The number of child queue discs does not match the number of netdevice queues");
              for (uint32_t i = 0; i < devQueueIface->GetTxQueuesN (); i++)
                {
                  devQueueIface->GetTxQueue (i)->SetWakeCallback (MakeCallback (&QueueDisc::Run,
                                                                  m_rootQueueDiscs[j]->GetQueueDiscClass (i)->GetQueueDisc ()));
                  qdMap->second.second.push_back (m_rootQueueDiscs[j]->GetQueueDiscClass (i)->GetQueueDisc ());
                }
            }

          // initialize the queue disc
          m_rootQueueDiscs[j]->Initialize ();
        }
    }
  Object::DoInitialize ();
}

void
TrafficControlLayer::SetupDevice (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);

  // ensure this setup is done just once (in case of dual stack nodes)
  if (device->GetObject<NetDeviceQueueInterface> ())
    {
      NS_LOG_DEBUG ("The setup for this device has been already done.");
      return;
    }

  // create a NetDeviceQueueInterface object and aggregate it to the device
  Ptr<NetDeviceQueueInterface> devQueueIface = CreateObject<NetDeviceQueueInterface> ();
  device->AggregateObject (devQueueIface);

  // store a pointer to the created queue interface
  NS_ASSERT_MSG (m_netDeviceQueueToQueueDiscMap.find (device) == m_netDeviceQueueToQueueDiscMap.end (),
                 "This is a bug: SetupDevice should be called only once per device");

  m_netDeviceQueueToQueueDiscMap[device] = NetDeviceInfo (devQueueIface, QueueDiscVector ());
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
  NS_LOG_FUNCTION (this << device);

  uint32_t index = GetDeviceIndex (device);
  NS_ASSERT_MSG (index < m_node->GetNDevices (), "The provided device does not belong to"
                 << " the node which this TrafficControlLayer object is aggregated to"  );
  if (index >= m_rootQueueDiscs.size ())
    {
      m_rootQueueDiscs.resize (index+1);
    }

  NS_ASSERT_MSG (m_rootQueueDiscs[index] == 0, "Cannot install a root queue disc on a "
                 << "device already having one. Delete the existing queue disc first.");

  m_rootQueueDiscs[index] = qDisc;
}

Ptr<QueueDisc>
TrafficControlLayer::GetRootQueueDiscOnDevice (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);

  uint32_t index = GetDeviceIndex (device);
  NS_ASSERT_MSG (index < m_node->GetNDevices (), "The provided device does not belong to"
                 << " the node which this TrafficControlLayer object is aggregated to"  );
  if (index >= m_rootQueueDiscs.size ())
    {
      m_rootQueueDiscs.resize (index+1);
    }
  return m_rootQueueDiscs[index];
}

void
TrafficControlLayer::DeleteRootQueueDiscOnDevice (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);

  uint32_t index = GetDeviceIndex (device);
  NS_ASSERT_MSG (index < m_node->GetNDevices (), "The provided device does not belong to"
                 << " the node which this TrafficControlLayer object is aggregated to"  );

  NS_ASSERT_MSG (index < m_rootQueueDiscs.size () && m_rootQueueDiscs[index] != 0, "No root queue disc"
                 << " installed on device " << device);

  // remove the root queue disc
  m_rootQueueDiscs[index] = 0;
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
TrafficControlLayer::GetDeviceIndex (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  uint32_t i;
  for (i = 0; i < m_node->GetNDevices () && device != m_node->GetDevice (i); i++);
  return i;
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
TrafficControlLayer::Send (Ptr<NetDevice> device, Ptr<Packet> packet,
                           const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << device << packet << dest << protocolNumber);

  NS_LOG_DEBUG ("Send packet to device " << device << " protocol number " <<
                protocolNumber);

  std::map<Ptr<NetDevice>, NetDeviceInfo>::iterator qdMap = m_netDeviceQueueToQueueDiscMap.find (device);
  NS_ASSERT (qdMap != m_netDeviceQueueToQueueDiscMap.end ());
  Ptr<NetDeviceQueueInterface> devQueueIface = qdMap->second.first;
  NS_ASSERT (devQueueIface);

  // determine the transmission queue of the device where the packet will be enqueued
  uint8_t txq = devQueueIface->GetSelectedQueue (Create<QueueDiscItem> (packet, dest, protocolNumber));
  NS_ASSERT (txq < devQueueIface->GetTxQueuesN ());

  if (qdMap->second.second.empty ())
    {
      // The device has no attached queue disc, thus send the packet
      // directly to the device if the selected queue is not stopped
      if (!devQueueIface->GetTxQueue (txq)->IsStopped ())
        {
          device->Send (packet, dest, protocolNumber);
        }
    }
  else
    {
      // Enqueue the packet in the queue disc associated with the netdevice queue
      // selected for the packet and try to dequeue packets from such queue disc
      Ptr<QueueDiscItem> item = Create<QueueDiscItem> (packet, dest, protocolNumber);
      item->SetTxQueueIndex (txq);

      Ptr<QueueDisc> qDisc = qdMap->second.second[txq];
      NS_ASSERT (qDisc);
      qDisc->Enqueue (item);
      qDisc->Run ();
    }
}

} // namespace ns3
