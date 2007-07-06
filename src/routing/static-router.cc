/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/debug.h"
#include "ns3/internet-node.h"
#include "static-router.h"

NS_DEBUG_COMPONENT_DEFINE ("StaticRouter");

namespace ns3 {

const InterfaceId StaticRouter::iid = 
  MakeInterfaceId ("StaticRouter", Object::iid);

StaticRouter::StaticRouter (Ptr<Node> node)
  : m_node(node)
{
  SetInterfaceId (StaticRouter::iid);
  NS_DEBUG("StaticRouter::StaticRouter ()");
}

StaticRouter::~StaticRouter ()
{
  NS_DEBUG("StaticRouter::~StaticRouter ()");
}

} // namespace ns3
