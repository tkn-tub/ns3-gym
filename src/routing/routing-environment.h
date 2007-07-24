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
#ifndef ROUTING_ENVIRONMENT_H
#define ROUTING_ENVIRONMENT_H

#include <stdint.h>
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

namespace ns3 {
namespace RoutingEnvironment {
 
/**
 * @brief This function tests the value of the global default value
 * "DoStaticRouting".  This approach puts everything in one compilation
 * unit, as opposed to explicitly testing the value of the underlying
 * static variable.
 */
bool StaticRoutingEnabled(void);
/**
 * @brief Allocate a 32-bit router ID from monotonically increasing counter.
 */
uint32_t AllocateRouterId(void);

} // namespace RoutingEnvironment
} // namespace ns3

#endif /* ROUTING_ENVIRONMENT_H */
