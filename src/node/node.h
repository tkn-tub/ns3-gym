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

// Define the basic Node object for ns3.
// George F. Riley, Georgia Tech, Fall 2006

// The Node class is the building block for all network element definitions
// in ns3.  The design approach is to create a node object by including
// one or mode "node capabilities", selecting the capabilities based on
// the desired features and behavior of a node.  For example, an
// "InternetNode" has capabilities for a list of network devices, a layer 3
// protocol list, a layer 4 protocol list, and a list of processs.
// A "SensorNode" has a list of network devices, a list of "Sensors", and
// an energy model.  
// 
// To create a new node class, perform the following steps.
// 
// 1) Create your node subclass as a direct descendent of the Node base class.
// 2) Add members to your node subclass that are pointers to each of the
//    node capabilities you need.  We use pointers here rather than direct
//    objects, since you might want a SensorNode with a specific energy
//    model that derives from the base EnergyModel capability.
// 3) Override each of the "Get*" virtual member functions of the Node base
//    class to return the appropriate pointer to each capability.
// 4) Implement a copy constructor that calls the "Copy" method on each
//    capability in your class.  Do NOT just copy the pointers, as this will
//    result in "double delete".
// 5) Implement a destructor that deletes each of your capabilities.
// 6) Implement a Copy() method that returns a copy of your node.  This
//    is usually just one line of code, calling "new" and specifying the
//    copy constructor. See the Copy method in InternetNode for an example.
// 
// To implement a new Capability, perform the following steps:
// 
// 1) Create your new capability class as a direct descendent of the
//    NodeCapability base class.
// 2) If needed, implement a copy constructor.  This is typically only
//    needed if your capability does dynamic memory management (ie. new
//    and delete).
// 3) If needed, implement a destructor.  Again, this is typically only
//    needed if you use dynamic memory.
// 4) Implement a Copy() method that returns a copy of your capability.
// 5) Where possible Create a "Null" capability subclass of your capability
//    that does nothing.  For example, the basic energy model provides the
//    NullEnergyModel that is returned by the node base class "GetEnergyModel()"
//    call when the energy model does not exist.  This Null capability of course
//    has the same API as all energy models, but does nothing.  THis allows
//    users of the energy model to use the pointer returned by GetEnergyModel()
//    without having to check for the nil return.
// 6) Implement a static GetNullCapability() method that returns either a
//     pointer to the Null capability (see 5 above) of a nil pointer if no
//     null capability exists.
// 7) Implement a "Get*" virtual method in the node base that returns
//    the null capability.
//
// To implement a variation on an existing capability, perform
// the following steps:
//
// 1) Create your new capability as a subclass of an existing capability.
// 2) Override the capability members as needed to implement the desired
//    behavior.
// 3) Override the Copy() method to create a copy of your capability
//    subclass.
//
// The design team for ns3 expects that the number of different node
// capabilities will remain relatively small over time.  Contributors
// and those modifying ns3 for their own uses are encouraged to subclass
// an existing capability where possible.

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <list>

namespace ns3 {

class L3Demux;
class Ipv4L4Demux;
class NetDeviceList;
class Ipv4;
class Udp;

class Node {
public:
  typedef int32_t Id_t;       // For identifying nodes
  typedef int32_t SystemId_t;     // Iidentifies which processor (distributed sim)

  Node();
  virtual ~Node();
  virtual Node* Copy() const = 0;// Make a copy of this node
  void SetNodeId(Id_t id);
  void SetSystemId(SystemId_t s);

  // Virtual "Getters" for each capability.
  // These exist to allow owners of a generic Node pointer to get
  // a pointer to the underlying capability, a pointer to a "NULL"
  // capability if one exists, or the nil pointer if not.
  // Each of these has a default behavior of returning a null capability
  // of the correct type if one exists, or the nil pointer if no
  // null capability exists.
  virtual L3Demux*         GetL3Demux() const;
  virtual Ipv4L4Demux*     GetIpv4L4Demux() const;
  virtual NetDeviceList*   GetNetDeviceList() const;
  virtual Ipv4 *           GetIpv4 (void) const;
  virtual Udp *            GetUdp (void) const;
  
private:
  Id_t         m_id;         // Node id for this node
  SystemId_t   m_sid;        // System id for this node
};

}; //namespace ns3
#endif
