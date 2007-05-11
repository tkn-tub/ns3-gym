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
#include "ns3/interface.h"
#include "ns3/ptr.h"
#include <vector>

namespace ns3 {

class ApplicationList : public NsUnknown
{
public:
  static const Iid iid;
  ApplicationList(Ptr<Node>);
  // Copy constructor not needed, default one is correct
  virtual ~ApplicationList();
  virtual void Add(Ptr<Application> application);

  uint32_t Count() const;  // Number of applications
  Ptr<Application> Get(uint32_t i) const; // Get app by index
  
protected:
  virtual void DoDispose (void);
private:
  std::vector<Ptr<Application> > m_apps;
};

}//namespace ns3
#endif

