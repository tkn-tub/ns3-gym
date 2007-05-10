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

const Iid ApplicationList::iid ("ApplicationList");

ApplicationList::ApplicationList(Ptr<Node> n)
  : NsUnknown (ApplicationList::iid)
{}

void 
ApplicationList::DoDispose (void)
{
  for (std::vector<Ptr<Application> >::iterator i = m_apps.begin();
       i != m_apps.end(); ++i)
    {
      Ptr<Application> app = *i;
      app->Dispose ();
      *i = 0;
    }
  m_apps.clear ();
  NsUnknown::DoDispose ();
}
  
ApplicationList::~ApplicationList()
{}

void
ApplicationList::Add(Ptr<Application> a)
{
  m_apps.push_back(a);
}  

uint32_t ApplicationList::Count() const
{
  return m_apps.size();
}

Ptr<Application> ApplicationList::Get(uint32_t i) const
{
  if (m_apps.empty()) 
    {
      return 0;
    }
  return m_apps[i];
}
  
}//namespace ns3
