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
#include "ns3/type-name.h"
#include "container.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymDataContainer");

NS_OBJECT_ENSURE_REGISTERED (OpenGymDataContainer);


TypeId
OpenGymDataContainer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpenGymDataContainer")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymDataContainer> ()
    ;
  return tid;
}

OpenGymDataContainer::OpenGymDataContainer()
{
  //NS_LOG_FUNCTION (this);
}

OpenGymDataContainer::~OpenGymDataContainer ()
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDataContainer::DoDispose (void)
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDataContainer::DoInitialize (void)
{
  //NS_LOG_FUNCTION (this);
}


TypeId
OpenGymDiscreteContainer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpenGymDiscreteContainer")
    .SetParent<OpenGymDataContainer> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymDiscreteContainer> ()
    ;
  return tid;
}

OpenGymDiscreteContainer::OpenGymDiscreteContainer()
{
  //NS_LOG_FUNCTION (this);
}

OpenGymDiscreteContainer::~OpenGymDiscreteContainer ()
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDiscreteContainer::DoDispose (void)
{
  //NS_LOG_FUNCTION (this);
}

void
OpenGymDiscreteContainer::DoInitialize (void)
{
  //NS_LOG_FUNCTION (this);
}

}