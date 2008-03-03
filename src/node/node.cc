/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation, INRIA
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
 * Authors: George F. Riley<riley@ece.gatech.edu>
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "node.h"
#include "node-list.h"
#include "net-device.h"
#include "application.h"
#include "packet-socket-factory.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/object-vector.h"
#include "ns3/uinteger.h"

namespace ns3{

NS_OBJECT_ENSURE_REGISTERED (Node);

TypeId 
Node::GetTypeId (void)
{
  static TypeId tid = TypeId ("Node")
    .SetParent<Object> ()
    .AddAttribute ("DeviceList", "The list of devices associated to this Node.",
                   ObjectVector (),
                   MakeObjectVectorAccessor (&Node::m_devices),
                   MakeObjectVectorChecker ())
    .AddAttribute ("ApplicationList", "The list of applications associated to this Node.",
                   ObjectVector (),
                   MakeObjectVectorAccessor (&Node::m_applications),
                   MakeObjectVectorChecker ())
    .AddAttribute ("Id", "The id (unique integer) of this Node.",
                   TypeId::ATTR_GET, // allow only getting it.
                   Uinteger (0),
                   MakeUintegerAccessor (&Node::m_id),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

Node::Node()
  : m_id(0), 
    m_sid(0)
{
  Construct ();
}

Node::Node(uint32_t sid)
  : m_id(0), 
    m_sid(sid)
{ 
  Construct ();
}

void
Node::Construct (void)
{
  m_id = NodeList::Add (this);
  Ptr<PacketSocketFactory> socketFactory = CreateObject<PacketSocketFactory> ();
  AggregateObject (socketFactory);
}
  
Node::~Node ()
{}

uint32_t 
Node::GetId (void) const
{
  return m_id;
}

uint32_t 
Node::GetSystemId (void) const
{
  return m_sid;
}

uint32_t 
Node::AddDevice (Ptr<NetDevice> device)
{
  uint32_t index = m_devices.size ();
  m_devices.push_back (device);
  device->SetIfIndex(index);
  device->SetReceiveCallback (MakeCallback (&Node::ReceiveFromDevice, this));
  NotifyDeviceAdded (device);
  return index;
}
Ptr<NetDevice>
Node::GetDevice (uint32_t index) const
{
  return m_devices[index];
}
uint32_t 
Node::GetNDevices (void) const
{
  return m_devices.size ();
}

uint32_t 
Node::AddApplication (Ptr<Application> application)
{
  uint32_t index = m_applications.size ();
  m_applications.push_back (application);
  return index;
}
Ptr<Application> 
Node::GetApplication (uint32_t index) const
{
  return m_applications[index];
}
uint32_t 
Node::GetNApplications (void) const
{
  return m_applications.size ();
}

void 
Node::DoDispose()
{
  for (std::vector<Ptr<NetDevice> >::iterator i = m_devices.begin ();
       i != m_devices.end (); i++)
    {
      Ptr<NetDevice> device = *i;
      device->Dispose ();
      *i = 0;
    }
  m_devices.clear ();
  for (std::vector<Ptr<Application> >::iterator i = m_applications.begin ();
       i != m_applications.end (); i++)
    {
      Ptr<Application> application = *i;
      application->Dispose ();
      *i = 0;
    }
  m_applications.clear ();
  Object::DoDispose ();
}

void 
Node::NotifyDeviceAdded (Ptr<NetDevice> device)
{}

void
Node::RegisterProtocolHandler (ProtocolHandler handler, 
                               uint16_t protocolType,
                               Ptr<NetDevice> device)
{
  struct Node::ProtocolHandlerEntry entry;
  entry.handler = handler;
  entry.protocol = protocolType;
  entry.device = device;
  m_handlers.push_back (entry);
}

void
Node::UnregisterProtocolHandler (ProtocolHandler handler)
{
  for (ProtocolHandlerList::iterator i = m_handlers.begin ();
       i != m_handlers.end (); i++)
    {
      if (i->handler.IsEqual (handler))
        {
          m_handlers.erase (i);
          break;
        }
    }
}

bool
Node::ReceiveFromDevice (Ptr<NetDevice> device, Ptr<Packet> packet, 
                         uint16_t protocol, const Address &from)
{
  bool found = false;
  for (ProtocolHandlerList::iterator i = m_handlers.begin ();
       i != m_handlers.end (); i++)
    {
      if (i->device == 0 ||
          (i->device != 0 && i->device == device))
        {
          if (i->protocol == 0 || 
              i->protocol == protocol)
            {
              i->handler (device, packet, protocol, from);
              found = true;
            }
        }
    }
  return found;
}

}//namespace ns3
