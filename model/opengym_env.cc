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

#include "ns3/log.h"
#include "ns3/object.h"
#include "opengym_env.h"
#include "container.h"
#include "spaces.h"
#include "opengym_interface.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (OpenGymEnv);

NS_LOG_COMPONENT_DEFINE ("OpenGymEnv");

TypeId
OpenGymEnv::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpenGymEnv")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    ;
  return tid;
}

OpenGymEnv::OpenGymEnv()
{
  NS_LOG_FUNCTION (this);
}

OpenGymEnv::~OpenGymEnv ()
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymEnv::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymEnv::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymEnv::SetOpenGymInterface(Ptr<OpenGymInterface> openGymInterface)
{
  NS_LOG_FUNCTION (this);
  m_openGymInterface = openGymInterface;
  openGymInterface->SetGetActionSpaceCb( MakeCallback (&OpenGymEnv::GetActionSpace, this) );
  openGymInterface->SetGetObservationSpaceCb( MakeCallback (&OpenGymEnv::GetObservationSpace, this) );
  openGymInterface->SetGetGameOverCb( MakeCallback (&OpenGymEnv::GetGameOver, this) );
  openGymInterface->SetGetObservationCb( MakeCallback (&OpenGymEnv::GetObservation, this) );
  openGymInterface->SetGetRewardCb( MakeCallback (&OpenGymEnv::GetReward, this) );
  openGymInterface->SetGetExtraInfoCb( MakeCallback (&OpenGymEnv::GetExtraInfo, this) );
  openGymInterface->SetExecuteActionsCb( MakeCallback (&OpenGymEnv::ExecuteActions, this) );
}

void
OpenGymEnv::Notify()
{
  NS_LOG_FUNCTION (this);
  if (m_openGymInterface)
  {
    m_openGymInterface->Notify(this);
  }
}

void
OpenGymEnv::NotifySimulationEnd()
{
  NS_LOG_FUNCTION (this);
  if (m_openGymInterface)
  {
    m_openGymInterface->NotifySimulationEnd();
  }
}

}