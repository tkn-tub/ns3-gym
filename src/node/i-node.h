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

#ifndef __NODE_H__
#define __NODE_H__

#include <vector>

#include "ns3/interface.h"

namespace ns3 {

class TraceContext;
class TraceResolver;
class NetDevice;
class Application;

/**
 * \brief A network Node.
 *
 * This class holds together:
 *   - a list of NetDevice objects which represent the network interfaces
 *     of this node which are connected to other Node instances through
 *     Channel instances.
 *   - a list of Application objects which represent the userspace
 *     traffic generation applications which interact with the Node
 *     through the Socket API.
 *   - a node Id: a unique per-node identifier.
 *   - a system Id: a unique Id used for parallel simulations.
 *   - a trace resolver which can be used to connect user trace sinks
 *     to the node's trace sources.
 *
 * Every Node created is added to the NodeList automatically.
 */
class Node : public Interface
{
public:
  static const InterfaceId iid;

  virtual ~Node();

  /**
   * \param context the trace context for the TraceResolver to create
   * \returns a newly-created TraceResolver. The caller takes
   *          ownership of the returned pointer.
   *
   * Request the Node to create a trace resolver. This method
   * could be used directly by a user who needs access to very low-level
   * trace configuration.
   */
  TraceResolver *CreateTraceResolver (TraceContext const &context);

  /**
   * \returns the unique id of this node.
   * 
   * This unique id happens to be also the index of the Node into
   * the NodeList. 
   */
  uint32_t GetId (void) const;

  /**
   * \returns the system id for parallel simulations associated
   *          to this node.
   */
  uint32_t GetSystemId (void) const;

  /**
   * \param device NetDevice to associate to this node.
   * \returns the index of the NetDevice into the Node's list of
   *          NetDevice.
   *
   * Associate this device to this node.
   * This method is called automatically from NetDevice::NetDevice
   * so the user has little reason to call this method himself.
   */
  uint32_t AddDevice (Ptr<NetDevice> device);
  /**
   * \param index the index of the requested NetDevice
   * \returns the requested NetDevice associated to this Node.
   */
  Ptr<NetDevice> GetDevice (uint32_t index) const;
  /**
   * \returns the number of NetDevice instances associated
   *          to this Node.
   */
  uint32_t GetNDevices (void) const;

  /**
   * \param application Application to associate to this node.
   * \returns the index of the Application within the Node's list
   *          of Application.
   *
   * Associated this Application to this Node. This method is called
   * automatically from Application::Application so the user
   * has little reasons to call this method directly.
   */
  uint32_t AddApplication (Ptr<Application> application);
  /**
   * \param index
   * \returns the application associated to this requested index
   *          within this Node.
   */
  Ptr<Application> GetApplication (uint32_t index) const;
  /**
   * \returns the number of applications associated to this Node.
   */
  uint32_t GetNApplications (void) const;

protected:
  /**
   * Must be invoked by subclasses only.
   */
  Node();
  /**
   * \param systemId a unique integer used for parallel simulations.
   *
   * Must be invoked by subclasses only.
   */
  Node(uint32_t systemId);
  /**
   * The dispose method. Subclasses must override this method
   * and must chain up to it by calling Node::DoDispose at the
   * end of their own DoDispose method.
   */
  virtual void DoDispose (void);
private:
  /**
   * \param context the trace context
   * \returns a trace resolver to the user. The user must delete it.
   *
   * Subclasses must implement this method.
   */
  virtual TraceResolver *DoCreateTraceResolver (TraceContext const &context) = 0;
  /**
   * \param device the device added to this Node.
   *
   * This method is invoked whenever a user calls Node::AddDevice.
   * Subclasses are expected to call NetDevice::SetReceiveCallback
   * at this point to setup the node's receive function for
   * the NetDevice packets.
   */
  virtual void DoAddDevice (Ptr<NetDevice> device) const = 0;

  uint32_t    m_id;         // Node id for this node
  uint32_t    m_sid;        // System id for this node
  std::vector<Ptr<NetDevice> > m_devices;
  std::vector<Ptr<Application> > m_applications;
};

} //namespace ns3
#endif
