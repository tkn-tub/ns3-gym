// -*- Mode:NS3 -*-
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
// Implement the application list capability for NS3 nodes
// George F. Riley, Georgia Tech, Spring 2007

#include "application.h"
#include "application-list.h"

namespace ns3{

ApplicationList::ApplicationList()
    : Capability(0)
{
}
  
ApplicationList::ApplicationList(Node* n)
    : Capability(n)
{
}
  
ApplicationList::~ApplicationList()
{ // Destructor, nothing needed as the SmartSet destroys itself
}

ApplicationList* ApplicationList::Copy(Node * n) const 
{ // Copy this app list
  ApplicationList* r = new ApplicationList();
  r->SetNode(n);
  return r;
}

void
ApplicationList::Add(Application* a)
{
  m_apps.Add(a);
}

void ApplicationList::SetNode(Node * n)
{
  Capability::SetNode(n);
  // Set the node pointer in each application
  for (SmartSet<Application*>::const_iterator i = m_apps.Begin();
       i != m_apps.End(); ++i)
    { // Set correct node pointer in each app
      (*i)->SetNode(n);
    }
}
  
void ApplicationList::Remove(Application* a)
{ // Remove the specified application from the list
  m_apps.Remove(a);
}

SmartSet<Application*>::size_type ApplicationList::Count() const
{
  return m_apps.Size();
}

Application* ApplicationList::Get(SmartSet<Application*>::size_type i) const
{ // Get the i'th application. Note, this is linear time in N
  if (m_apps.Empty()) return 0;        // List is empty
  SmartSet<Application*>::const_iterator k = m_apps.Begin();
  while(i > 0)
    {
      if (k == m_apps.End()) return 0; // Not found
      --i;
      ++k;
    }
  return *k;
}

const SmartSet<Application*>& ApplicationList::GetAll() const
{
  return m_apps;
}
  
}//namespace ns3
