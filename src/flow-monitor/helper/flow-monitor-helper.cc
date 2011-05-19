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

#include "flow-monitor-helper.h"

#include "ns3/flow-monitor.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/ipv4-flow-probe.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/node.h"
#include "ns3/node-list.h"


namespace ns3 {

FlowMonitorHelper::FlowMonitorHelper ()
{
  m_monitorFactory.SetTypeId ("ns3::FlowMonitor");
}

void 
FlowMonitorHelper::SetMonitorAttribute (std::string n1, const AttributeValue &v1)
{
  m_monitorFactory.Set (n1, v1);
}


Ptr<FlowMonitor>
FlowMonitorHelper::GetMonitor ()
{
  if (!m_flowMonitor)
    {
      m_flowMonitor = m_monitorFactory.Create<FlowMonitor> ();
      m_flowClassifier = Create<Ipv4FlowClassifier> ();
      m_flowMonitor->SetFlowClassifier (m_flowClassifier);
    }
  return m_flowMonitor;
}


Ptr<FlowClassifier>
FlowMonitorHelper::GetClassifier ()
{
  if (!m_flowClassifier)
    {
      m_flowClassifier = Create<Ipv4FlowClassifier> ();
    }
  return m_flowClassifier;
}


Ptr<FlowMonitor>
FlowMonitorHelper::Install (Ptr<Node> node)
{
  Ptr<FlowMonitor> monitor = GetMonitor ();
  Ptr<FlowClassifier> classifier = GetClassifier ();
  Ptr<Ipv4FlowProbe> probe = Create<Ipv4FlowProbe> (monitor,
                                                    DynamicCast<Ipv4FlowClassifier> (classifier),
                                                    node);
  return m_flowMonitor;
}


Ptr<FlowMonitor>
FlowMonitorHelper::Install (NodeContainer nodes)
{
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetObject<Ipv4L3Protocol> ())
        {
          Install (node);
        }
    }
  return m_flowMonitor;
}

Ptr<FlowMonitor>
FlowMonitorHelper::InstallAll ()
{
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetObject<Ipv4L3Protocol> ())
        {
          Install (node);
        }
    }
  return m_flowMonitor;
}


} // namespace ns3
