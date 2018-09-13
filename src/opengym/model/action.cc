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
#include "action.h"
#include "container.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymAction");

NS_OBJECT_ENSURE_REGISTERED (OpenGymAction);


TypeId
OpenGymAction::GetTypeId (void)
{
  static TypeId tid = TypeId ("OpenGymAction")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    ;
  return tid;
}

OpenGymAction::OpenGymAction()
{
  NS_LOG_FUNCTION (this);
}

OpenGymAction::~OpenGymAction ()
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymAction::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymAction::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
}


bool
OpenGymAction::AddActionContainer(Ptr<OpenGymDataContainer> container)
{
  NS_LOG_FUNCTION (this);
  m_containers.push_back(container);
  return true;
}

Ptr<OpenGymDataContainer>
OpenGymAction::GetActionContainer(uint32_t idx)
{
  NS_LOG_FUNCTION (this);
  return m_containers[idx];
}

std::vector<Ptr<OpenGymDataContainer> >
OpenGymAction::GetActionContainers()
{
  NS_LOG_FUNCTION (this);
  return m_containers;
}

}