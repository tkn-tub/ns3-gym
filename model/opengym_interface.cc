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

#include <sys/types.h>
#include <unistd.h>
#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/simulator.h"
#include "opengym_interface.h"
#include "opengym_env.h"
#include "container.h"
#include "spaces.h"
#include "messages.pb.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymInterface");

NS_OBJECT_ENSURE_REGISTERED (OpenGymInterface);


TypeId
OpenGymInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("OpenGymInterface")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymInterface> ()
    ;
  return tid;
}

Ptr<OpenGymInterface>
OpenGymInterface::Get (uint32_t port)
{
  NS_LOG_FUNCTION_NOARGS ();
  return *DoGet (port);
}

Ptr<OpenGymInterface> *
OpenGymInterface::DoGet (uint32_t port)
{
  NS_LOG_FUNCTION_NOARGS ();
  static Ptr<OpenGymInterface> ptr = nullptr;
  if (ptr == nullptr)
    {
      ptr = CreateObject<OpenGymInterface> (port);
      Config::RegisterRootNamespaceObject (ptr);
      Simulator::ScheduleDestroy (&OpenGymInterface::Delete);
    }
  return &ptr;
}

void
OpenGymInterface::Delete (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Config::UnregisterRootNamespaceObject (Get ());
  (*DoGet ()) = 0;
}

OpenGymInterface::OpenGymInterface(uint32_t port):
  m_port(port), m_zmq_context(1), m_zmq_socket(m_zmq_context, ZMQ_REQ),
  m_simEnd(false), m_stopEnvRequested(false), m_initSimMsgSent(false)
{
  NS_LOG_FUNCTION (this);
}

OpenGymInterface::~OpenGymInterface ()
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymInterface::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymInterface::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
}

void
OpenGymInterface::SetGetActionSpaceCb(Callback< Ptr<OpenGymSpace> > cb)
{
  NS_LOG_FUNCTION (this);
  m_actionSpaceCb = cb;
}

void
OpenGymInterface::SetGetObservationSpaceCb(Callback< Ptr<OpenGymSpace> > cb)
{
  NS_LOG_FUNCTION (this);
  m_observationSpaceCb = cb;
}

void
OpenGymInterface::SetGetGameOverCb(Callback< bool > cb)
{
  NS_LOG_FUNCTION (this);
  m_gameOverCb = cb;
}

void
OpenGymInterface::SetGetObservationCb(Callback< Ptr<OpenGymDataContainer> > cb)
{
  NS_LOG_FUNCTION (this);
  m_obsCb = cb;
}

void
OpenGymInterface::SetGetRewardCb(Callback<float> cb)
{
  NS_LOG_FUNCTION (this);
  m_rewardCb = cb;
}

void
OpenGymInterface::SetGetExtraInfoCb(Callback<std::string> cb)
{
  NS_LOG_FUNCTION (this);
  m_extraInfoCb = cb;
}

void
OpenGymInterface::SetExecuteActionsCb(Callback<bool, Ptr<OpenGymDataContainer> > cb)
{
  NS_LOG_FUNCTION (this);
  m_actionCb = cb;
}

void 
OpenGymInterface::Init()
{
  NS_LOG_FUNCTION (this);
  // do not send init msg twice
  if (m_initSimMsgSent) {
    return;
  }
  m_initSimMsgSent = true;

  std::string connectAddr = "tcp://localhost:" + std::to_string(m_port);
  zmq_connect ((void*)m_zmq_socket, connectAddr.c_str());

  Ptr<OpenGymSpace> obsSpace = GetObservationSpace();
  Ptr<OpenGymSpace> actionSpace = GetActionSpace();

  NS_LOG_UNCOND("Simulation process id: " << ::getpid() << " (parent (waf shell) id: " << ::getppid() << ")");
  NS_LOG_UNCOND("Waiting for Python process to connect on port: "<< connectAddr);
  NS_LOG_UNCOND("Please start proper Python Gym Agent");

  ns3opengym::SimInitMsg simInitMsg;
  simInitMsg.set_simprocessid(::getpid());
  simInitMsg.set_wafshellprocessid(::getppid());

  if (obsSpace) {
    ns3opengym::SpaceDescription spaceDesc;
    spaceDesc = obsSpace->GetSpaceDescription();
    simInitMsg.mutable_obsspace()->CopyFrom(spaceDesc);
  }

  if (actionSpace) {
    ns3opengym::SpaceDescription spaceDesc;
    spaceDesc = actionSpace->GetSpaceDescription();
    simInitMsg.mutable_actspace()->CopyFrom(spaceDesc);
  }

  // send init msg to python
  zmq::message_t request(simInitMsg.ByteSizeLong());;
  simInitMsg.SerializeToArray(request.data(), simInitMsg.ByteSizeLong());
  m_zmq_socket.send (request, zmq::send_flags::none);

  // receive init ack msg form python
  ns3opengym::SimInitAck simInitAck;
  zmq::message_t reply;
  (void) m_zmq_socket.recv (reply, zmq::recv_flags::none);
  simInitAck.ParseFromArray(reply.data(), reply.size());

  bool done = simInitAck.done();
  NS_LOG_DEBUG("Sim Init Ack: " << done);

  bool stopSim = simInitAck.stopsimreq();
  if (stopSim) {
    NS_LOG_DEBUG("---Stop requested: " << stopSim);
    m_stopEnvRequested = true;
    Simulator::Stop();
    Simulator::Destroy ();
    std::exit(0);
  }
}

void
OpenGymInterface::NotifyCurrentState()
{
  NS_LOG_FUNCTION (this);

  if (!m_initSimMsgSent) {
    Init();
  }

  if (m_stopEnvRequested) {
    return;
  }

  // collect current env state
  Ptr<OpenGymDataContainer> obsDataContainer = GetObservation();
  float reward = GetReward();
  bool isGameOver = IsGameOver();
  std::string extraInfo = GetExtraInfo();

  ns3opengym::EnvStateMsg envStateMsg;
  // observation
  ns3opengym::DataContainer obsDataContainerPbMsg;
  if (obsDataContainer) {
    obsDataContainerPbMsg = obsDataContainer->GetDataContainerPbMsg();
    envStateMsg.mutable_obsdata()->CopyFrom(obsDataContainerPbMsg);
  }
  // reward
  envStateMsg.set_reward(reward);
  // game over
  envStateMsg.set_isgameover(false);
  if (isGameOver)
  {
    envStateMsg.set_isgameover(true);
    if (m_simEnd) {
      envStateMsg.set_reason(ns3opengym::EnvStateMsg::SimulationEnd);
    } else {
      envStateMsg.set_reason(ns3opengym::EnvStateMsg::GameOver);
    }
  }

  // extra info
  envStateMsg.set_info(extraInfo);

  // send env state msg to python
  zmq::message_t request(envStateMsg.ByteSizeLong());;
  envStateMsg.SerializeToArray(request.data(), envStateMsg.ByteSizeLong());
  m_zmq_socket.send (request, zmq::send_flags::none);

  // receive act msg form python
  ns3opengym::EnvActMsg envActMsg;
  zmq::message_t reply;
  (void) m_zmq_socket.recv (reply, zmq::recv_flags::none);
  envActMsg.ParseFromArray(reply.data(), reply.size());

  if (m_simEnd) {
    // if sim end only rx ms and quit
    return;
  }

  bool stopSim = envActMsg.stopsimreq();
  if (stopSim) {
    NS_LOG_DEBUG("---Stop requested: " << stopSim);
    m_stopEnvRequested = true;
    Simulator::Stop();
    Simulator::Destroy ();
    std::exit(0);
  }

  // first step after reset is called without actions, just to get current state
  ns3opengym::DataContainer actDataContainerPbMsg = envActMsg.actdata();
  Ptr<OpenGymDataContainer> actDataContainer = OpenGymDataContainer::CreateFromDataContainerPbMsg(actDataContainerPbMsg);
  ExecuteActions(actDataContainer);

}

void
OpenGymInterface::WaitForStop()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_UNCOND("Wait for stop message");
  NotifyCurrentState();
}

void
OpenGymInterface::NotifySimulationEnd()
{
  NS_LOG_FUNCTION (this);
  m_simEnd = true;
  if (m_initSimMsgSent) {
    WaitForStop();
  }
}

bool
OpenGymInterface::IsGameOver()
{
  NS_LOG_FUNCTION (this);
  bool gameOver = false;
  if (!m_gameOverCb.IsNull())
  {
    gameOver = m_gameOverCb();
  }
  return (gameOver || m_simEnd);
}

Ptr<OpenGymSpace>
OpenGymInterface::GetActionSpace()
{
  NS_LOG_FUNCTION (this);
  Ptr<OpenGymSpace> actionSpace;
  if (!m_actionSpaceCb.IsNull())
  {
    actionSpace = m_actionSpaceCb();
  }
  return actionSpace;
}

Ptr<OpenGymSpace>
OpenGymInterface::GetObservationSpace()
{
  NS_LOG_FUNCTION (this);
  Ptr<OpenGymSpace> obsSpace;
  if (!m_observationSpaceCb.IsNull())
  {
    obsSpace = m_observationSpaceCb();
  }
  return obsSpace;
}

Ptr<OpenGymDataContainer>
OpenGymInterface::GetObservation()
{
  NS_LOG_FUNCTION (this);
  Ptr<OpenGymDataContainer>  obs;
  if (!m_obsCb.IsNull())
  {
    obs = m_obsCb();
  }
  return obs;
}

float
OpenGymInterface::GetReward()
{
  NS_LOG_FUNCTION (this);
  float reward = 0.0;
  if (!m_rewardCb.IsNull())
  {
    reward = m_rewardCb();
  }
  return reward;
}

std::string
OpenGymInterface::GetExtraInfo()
{
  NS_LOG_FUNCTION (this);
  std::string info;
  if (!m_extraInfoCb.IsNull())
  {
    info = m_extraInfoCb();
  }
  return info;
}

bool
OpenGymInterface::ExecuteActions(Ptr<OpenGymDataContainer> action)
{
  NS_LOG_FUNCTION (this);
  bool reply = false;
  if (!m_actionCb.IsNull())
  {
    reply = m_actionCb(action);
  }
  return reply;
}

void
OpenGymInterface::Notify(Ptr<OpenGymEnv> entity)
{
  NS_LOG_FUNCTION (this);

  SetGetGameOverCb( MakeCallback (&OpenGymEnv::GetGameOver, entity) );
  SetGetObservationCb( MakeCallback (&OpenGymEnv::GetObservation, entity) );
  SetGetRewardCb( MakeCallback (&OpenGymEnv::GetReward, entity) );
  SetGetExtraInfoCb( MakeCallback (&OpenGymEnv::GetExtraInfo, entity) );
  SetExecuteActionsCb( MakeCallback (&OpenGymEnv::ExecuteActions, entity) );

  NotifyCurrentState();
}

}

