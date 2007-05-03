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
#include "ns3/iid-manager.h"

namespace ns3{

const uint32_t ApplicationList::iid = IidManager::Allocate ("ApplicationList");

ApplicationList::ApplicationList(Node* n)
  : NsUnknown (ApplicationList::iid)
{}

void 
ApplicationList::Dispose (void)
{
  for (std::vector<Application*>::const_iterator i = m_apps.begin();
       i != m_apps.end(); ++i)
    {
      Application *app = *i;
      app->Dispose ();
      app->Unref ();
    }
  m_apps.clear ();
}
  
ApplicationList::~ApplicationList()
{ // Destructor, nothing needed as the SmartSet destroys itself
  Dispose ();
}

ApplicationList* ApplicationList::Copy(Node * n) const 
{ // Copy this app list
  ApplicationList* r = new ApplicationList(n);
  return r;
}

void
ApplicationList::Add(Application* a)
{
  a->Ref ();
  m_apps.push_back(a);
}

void ApplicationList::SetNode(Node * n)
{
  // Set the node pointer in each application
  for (std::vector<Application *>::const_iterator i = m_apps.begin();
       i != m_apps.end(); ++i)
    { // Set correct node pointer in each app
      (*i)->SetNode(n);
    }
}
  

uint32_t ApplicationList::Count() const
{
  return m_apps.size();
}

Application* ApplicationList::Get(uint32_t i) const
{ // Get the i'th application. Note, this is linear time in N
  if (m_apps.empty()) return 0;        // List is empty
  return m_apps[i];
}
  
}//namespace ns3
