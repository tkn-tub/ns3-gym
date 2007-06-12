// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//

// Implement the basic Node object for ns3.
// George F. Riley, Georgia Tech, Fall 2006

#include "node.h"
#include "node-list.h"
#include "net-device.h"
#include "application.h"
#include "ns3/simulator.h"

namespace ns3{

const InterfaceId Node::iid = MakeInterfaceId ("Node", Object::iid);

Node::Node()
  : m_id(0), 
    m_sid(0)
{
  SetInterfaceId (Node::iid);
  m_id = NodeList::Add (this);
}

Node::Node(uint32_t sid)
  : m_id(0), 
    m_sid(sid)
{ 
  SetInterfaceId (Node::iid);
  m_id = NodeList::Add (this);
}
  
Node::~Node ()
{}

TraceResolver *
Node::CreateTraceResolver (TraceContext const &context)
{
  return DoCreateTraceResolver (context);
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
  DoAddDevice (device);
  device->SetIfIndex(index);
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


void Node::DoDispose()
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

}//namespace ns3
