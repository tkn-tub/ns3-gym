/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * All rights reserved.
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
 */

#include "ns3/debug.h"
#include "ns3/default-value.h"

#include "routing-environment.h"

NS_DEBUG_COMPONENT_DEFINE ("RoutingEnvironment");

namespace ns3 {
namespace RoutingEnvironment {

BooleanDefaultValue g_doStaticRoutingDefaultValue ("DoStaticRouting", 
  "Enable global static routing", false);

    bool
StaticRoutingEnabled(void)
{
  return g_doStaticRoutingDefaultValue.GetValue();
}

} // namespace RoutingEnvironment
} // namespace ns3
