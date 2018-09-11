/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 Piotr Gawlowicz
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
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 */

#include <algorithm>
#include "ns3/log.h"
#include "ns3/node.h"
#include "spaces.h"
#include <boost/algorithm/string.hpp>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymSpace");

NS_OBJECT_ENSURE_REGISTERED (OpenGymSpace);


TypeId
OpenGymSpace::GetTypeId (void)
{
  static TypeId tid = TypeId ("OpenGymSpace")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymSpace> ()
    ;
  return tid;
}

OpenGymSpace::OpenGymSpace()
{
  NS_LOG_FUNCTION (this);
}

OpenGymSpace::~OpenGymSpace ()
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymSpace::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymSpace::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
}

}