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
#include "ns3/composite-trace-resolver.h"

namespace ns3{

InterfaceId 
Node::iid (void)
{
  static InterfaceId iid = MakeInterfaceId ("Node", Object::iid ());
  return iid;
}

NodeNetDeviceIndex::NodeNetDeviceIndex ()
  : m_index (0)
{}
NodeNetDeviceIndex::NodeNetDeviceIndex (uint32_t index)
  : m_index (index)
{}
uint32_t 
NodeNetDeviceIndex::Get (void) const
{
  return m_index;
}
void 
NodeNetDeviceIndex::Print (std::ostream &os) const
{
  os << "device=" << m_index;
}
uint16_t 
NodeNetDeviceIndex::GetUid (void)
{
  static uint16_t uid = AllocateUid<NodeNetDeviceIndex> ("NodeNetDeviceIndex");
  return uid;
}
std::string 
NodeNetDeviceIndex::GetTypeName (void) const
{
  return "ns3::NodeNetDeviceIndex";
}


NodeApplicationIndex::NodeApplicationIndex ()
  : m_index (0)
{}
NodeApplicationIndex::NodeApplicationIndex (uint32_t index)
  : m_index (index)
{}
uint32_t 
NodeApplicationIndex::Get (void) const
{
  return m_index;
}
void 
NodeApplicationIndex::Print (std::ostream &os) const
{
  os << "device=" << m_index;
}
uint16_t 
NodeApplicationIndex::GetUid (void)
{
  static uint16_t uid = AllocateUid<NodeApplicationIndex> ("NodeApplicationIndex");
  return uid;
}
std::string 
NodeApplicationIndex::GetTypeName (void) const
{
  return "ns3::NodeApplicationIndex";
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
  AddInterface (socketFactory);
}
  
Node::~Node ()
{}

Ptr<TraceResolver>
Node::GetTraceResolver (void) const
{
  Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
  resolver->AddArray ("devices", m_devices.begin (), m_devices.end (), NodeNetDeviceIndex ());
  resolver->AddArray ("applications", m_applications.begin (), m_applications.end (), NodeApplicationIndex ());
  resolver->SetParentResolver (Object::GetTraceResolver ());
  return resolver;
}

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
