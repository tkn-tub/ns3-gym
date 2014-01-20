// -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2009 INESC Porto
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
// Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt> <gjcarneiro@gmail.com>
//
#ifndef FLOW_MONITOR_HELPER_H
#define FLOW_MONITOR_HELPER_H

#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-classifier.h"
#include <string>

namespace ns3 {

class AttributeValue;
class Ipv4FlowClassifier;

/**
 * \ingroup flow-monitor
 * \brief Helper to enable IPv4 flow monitoring on a set of Nodes
 */
class FlowMonitorHelper
{
public:

  FlowMonitorHelper ();
  ~FlowMonitorHelper ();

  /**
   * \brief Set an attribute for the to-be-created FlowMonitor object
   * \param n1 attribute name
   * \param v1 attibute value
   */
  void SetMonitorAttribute (std::string n1, const AttributeValue &v1);

  /**
   * \brief Enable flow monitoring on a set of nodes
   * \param nodes A NodeContainer holding the set of nodes to work with.
   * \returns a pointer to the FlowMonitor object
   */
  Ptr<FlowMonitor> Install (NodeContainer nodes);
  /**
   * \brief Enable flow monitoring on a single node
   * \param node A Ptr<Node> to the node on which to enable flow monitoring.
   * \returns a pointer to the FlowMonitor object
   */
  Ptr<FlowMonitor> Install (Ptr<Node> node);
  /**
   * \brief Enable flow monitoring on all nodes
   * \returns a pointer to the FlowMonitor object
   */
  Ptr<FlowMonitor> InstallAll ();

  /**
   * \brief Retrieve the FlowMonitor object created by the Install* methods
   * \returns a pointer to the FlowMonitor object
   */
  Ptr<FlowMonitor> GetMonitor ();

  /**
   * \brief Retrieve the FlowClassifier object created by the Install* methods
   * \returns a pointer to the FlowClassifier object
   */
  Ptr<FlowClassifier> GetClassifier ();

private:
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  FlowMonitorHelper (const FlowMonitorHelper&);
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   * \returns
   */
  FlowMonitorHelper& operator= (const FlowMonitorHelper&);

  ObjectFactory m_monitorFactory;       //!< Object factory
  Ptr<FlowMonitor> m_flowMonitor;       //!< the FlowMonitor object
  Ptr<FlowClassifier> m_flowClassifier; //!< the FlowClassifier object
};

} // namespace ns3


#endif /* FLOW_MONITOR_HELPER_H */
