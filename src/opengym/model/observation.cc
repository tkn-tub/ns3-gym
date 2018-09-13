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
#include "observation.h"
#include "container.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymObservation");

NS_OBJECT_ENSURE_REGISTERED (OpenGymObservation);


TypeId
OpenGymObservation::GetTypeId (void)
{
  static TypeId tid = TypeId ("OpenGymObservation")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    ;
  return tid;
}

OpenGymObservation::OpenGymObservation()
{
  NS_LOG_FUNCTION (this);
}

OpenGymObservation::~OpenGymObservation ()
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymObservation::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymObservation::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
}


bool
OpenGymObservation::AddObsContainer(Ptr<OpenGymDataContainer> container)
{
  NS_LOG_FUNCTION (this);
  m_containers.push_back(container);
  return true;
}

std::vector<Ptr<OpenGymDataContainer> >
OpenGymObservation::GetObsContainers()
{
  NS_LOG_FUNCTION (this);
  return m_containers;
}

}