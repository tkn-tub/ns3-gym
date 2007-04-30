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
// 5) Implement a "Get*" virtual method in the node base that returns
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

#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <list>

#include "ns3/smartvector.h"
#include "ns3/smartset.h"

namespace ns3 {

class NodeList;

class NetDeviceList;
class ApplicationList;

// The below five may be encapsulated/abstracted in a Kernel or Stack class
class L3Demux;
class Ipv4L4Demux;
class Ipv4;
class Udp;
class Arp;

class TraceContext;
class TraceResolver;
class NetDevice;

class Node {
friend class NodeList;
friend class SmartVector<Node*>;
friend class SmartSet<Node*>;

public:
  typedef SmartVector<Node*> SmartNodeVec_t;
  Node();
  Node(uint32_t); // Specify which system for a distributed simulation
  virtual ~Node();
  virtual Node* Copy() const = 0;// Make a copy of this node

  virtual TraceResolver *CreateTraceResolver (TraceContext const &context) = 0;

  uint32_t GetId (void) const;
  uint32_t GetSystemId (void) const;
  void SetSystemId(uint32_t s);

  uint32_t AddDevice (NetDevice *device);
  NetDevice *GetDevice (uint32_t index) const;
  uint32_t GetNDevices (void) const;

private:
  virtual void DoAddDevice (NetDevice *device) const = 0;

#ifdef REMOVE_FOR_NOW
  // Define a protected delete operator. This will prevent users
  // from attempting to delete Node objects.  The deletion of
  // Nodes is completely the responsibility of the Node class,
  // and in no case should be deleted by users.
protected:
  void operator delete(void* a)
  { // Just call the normal delete
    ::delete (Node*)a;
  }
#endif
public:
  // Static methods for creating nodes and managing the node stack

  // Create a new node.  The node will be a copy of the top of the
  // node prototype list
  static Node* Create();
  // Create with a uint32_t is used by distributed simulations to
  // indicate system ownership of the new node.
  static Node* Create(uint32_t);
  static Node* GetNodePrototype(); // Get the current node prototype
  // Specifies the type of node to be returned by Create()
  // This version specifies a pre-configured node to use as the prototype
  // Of course the passed object can be any subclass of Node.
  static Node* PushNodePrototype(const Node&); 
  // THis version replicates the top of the prototype stack
  static Node* PushNodePrototype();
  // Remove the top of the prototype stack
  static void PopNodePrototype();
  // Node access
  static const SmartNodeVec_t& Nodes(); // Get a vector of all nodes
  static void  ClearAll();          // Delete all nodes for memory leak checking
  static void  ClearAllPrototypes();// Delete the prototype stack

  // Global static variables
private: 
  static uint32_t       g_nextId;     // Next available ID
  static SmartNodeVec_t g_nodes;      // Vector of all nodes created
  static SmartNodeVec_t g_prototypes; // Node prototype stack

protected:
  void SetId(uint32_t);            // NodeList::Add() calls this

public:
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
  virtual ApplicationList* GetApplicationList() const;
  virtual Ipv4 *           GetIpv4 (void) const;
  virtual Udp *            GetUdp (void) const;
  virtual Arp *            GetArp (void) const;
  
private:
  uint32_t    m_id;         // Node id for this node
  uint32_t    m_sid;        // System id for this node
  std::vector<NetDevice *> m_devices;
};

} //namespace ns3
#endif
