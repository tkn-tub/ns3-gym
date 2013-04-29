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

/// \brief Helper to enable IPv4 flow monitoring on a set of Nodes
class FlowMonitorHelper
{
public:
  /// \brief Construct a FlowMonitorHelper class which makes it easier to 
  /// configure and use the FlowMonitor
  FlowMonitorHelper ();
  /// \brief Dispose of objects allocated by the helper
  ~FlowMonitorHelper ();

  /// \brief Set an attribute for the to-be-created FlowMonitor object
  void SetMonitorAttribute (std::string n1, const AttributeValue &v1);

  /// \brief Enable flow monitoring on a set of nodes
  /// \param nodes A NodeContainer holding the set of nodes to work with.
  Ptr<FlowMonitor> Install (NodeContainer nodes);
  /// \brief Enable flow monitoring on a single node
  /// \param node A Ptr<Node> to the node on which to enable flow monitoring.
  Ptr<FlowMonitor> Install (Ptr<Node> node);
  /// \brief Enable flow monitoring on all nodes
  Ptr<FlowMonitor> InstallAll ();

  /// \brief Retrieve the FlowMonitor object created by the Install* methods
  Ptr<FlowMonitor> GetMonitor ();

  /// \brief Retrieve the FlowClassifier object created by the Install* methods
  Ptr<FlowClassifier> GetClassifier ();

private:
  ObjectFactory m_monitorFactory;
  Ptr<FlowMonitor> m_flowMonitor;
  Ptr<FlowClassifier> m_flowClassifier;
};

} // namespace ns3


#endif /* FLOW_MONITOR_HELPER_H */
