/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "node-container.h"
#include "ns3/node-list.h"

namespace ns3 {

NodeContainer::NodeContainer ()
{}

NodeContainer::NodeContainer (Ptr<Node> node)
{
  m_nodes.push_back (node);
}
NodeContainer::NodeContainer (const NodeContainer &a, const NodeContainer &b)
{
  Add (a);
  Add (b);
}
NodeContainer::NodeContainer (const NodeContainer &a, const NodeContainer &b, 
                              const NodeContainer &c)
{
  Add (a);
  Add (b);
  Add (c);
}
NodeContainer::NodeContainer (const NodeContainer &a, const NodeContainer &b, 
                              const NodeContainer &c, const NodeContainer &d)
{
  Add (a);
  Add (b);
  Add (c);
  Add (d);
}

NodeContainer::Iterator 
NodeContainer::Begin (void) const
{
  return m_nodes.begin ();
}
NodeContainer::Iterator 
NodeContainer::End (void) const
{
  return m_nodes.end ();
}

uint32_t 
NodeContainer::GetN (void) const
{
  return m_nodes.size ();
}
Ptr<Node> 
NodeContainer::Get (uint32_t i) const
{
  return m_nodes[i];
}
void 
NodeContainer::Create (uint32_t n)
{
  for (uint32_t i = 0; i < n; i++)
    {
      m_nodes.push_back (CreateObject<Node> ());
    }
}
void 
NodeContainer::Add (NodeContainer other)
{
  for (Iterator i = other.Begin (); i != other.End (); i++)
    {
      m_nodes.push_back (*i);
    }
}
void 
NodeContainer::Add (Ptr<Node> node)
{
  m_nodes.push_back (node);
}

NodeContainer 
NodeContainer::GetGlobal (void)
{
  NodeContainer c;
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      c.Add (*i);
    }
  return c;
}

} // namespace ns3
