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
// Manages the list of applications associated with a node.
// George F. Riley, Georgia Tech, Spring 2007

#ifndef __APPLICATION_LIST_H__
#define __APPLICATION_LIST_H__

#include "application.h"
#include "ns3/smartset.h"
#include "capability.h"

namespace ns3 {

class ApplicationList : public Capability {
public:
  ApplicationList(Node*);
  // Copy constructor not needed, default one is correct
  ~ApplicationList();
  // Inherited from Capabilty
  virtual ApplicationList* Copy(Node*) const;
  virtual void SetNode(Node *);              // Sets the node for all apps
  virtual void Add(Application*);      // Add an already new'ed app
  // Manage the list
  template <typename T> T* AddCopy(const T& t)  // Add a new application
  {
    T* a = t.Copy();
    m_apps.Add(a);
    return a;
  }
  void Remove(Application*);                // Application has finished
  SmartSet<Application*>::size_type Count() const;  // Number of applications
  Application* Get(SmartSet<Application*>::size_type) const; // Get app by index
  const SmartSet<Application*>& GetAll() const;     // Get the entire app list
  
private:
  SmartSet<Application*> m_apps;
};

}//namespace ns3
#endif

