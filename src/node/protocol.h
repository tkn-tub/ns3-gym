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

// NS3 - Layer 3 Protocol base class
// George F. Riley, Georgia Tech, Summer 2006

// Note:  class name tentatively moved to Protocol-- tomh

#ifndef __protocol_h__
#define __protocol_h__

namespace ns3 {

class InternetNode;

class Protocol {
public:
Protocol(InternetNode* n) : m_node(n) {}
Protocol* Copy() const { return new Protocol(*this); }
InternetNode *GetNode (void) const  { return m_node; }

private:
  InternetNode* m_node;
};

};

#endif
