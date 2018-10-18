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
#include <boost/algorithm/string.hpp>

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

OpenGymInterface::OpenGymInterface(uint32_t port):
  m_port(port), m_zmq_context(1), m_zmq_socket(m_zmq_context, ZMQ_REP),
  m_gameOver(false), m_simEnd(false), m_stopEnvRequested(false)
{
  NS_LOG_FUNCTION (this);
  m_rxGetGameOver = false;
  m_rxGetObservation = false;
  m_rxGetReward = false;
  m_rxGetExtraInfo = false;
  m_rxSetActions = false;
  Simulator::Schedule (Seconds(0.0), &OpenGymInterface::Init, this);
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
  m_stateCb = cb;
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

ns3opengym::RequestMsg
OpenGymInterface::ReceiveMsg (void)
{
  zmq::message_t request;
  ns3opengym::RequestMsg requestPbMsg;
  m_zmq_socket.recv (&request);
  requestPbMsg.ParseFromArray(request.data(), request.size());
  return requestPbMsg;
}

int
OpenGymInterface::SendMsg(ns3opengym::ReplyMsg replyPbMsg)
{
  zmq::message_t reply(replyPbMsg.ByteSize());;
  replyPbMsg.SerializeToArray(reply.data(), replyPbMsg.ByteSize());
  int n = m_zmq_socket.send (reply);
  return n;
}

void 
OpenGymInterface::Init()
{
  NS_LOG_FUNCTION (this);
  std::string bindAddr = "tcp://*:" + std::to_string(m_port);
  zmq_bind ((void*)m_zmq_socket, bindAddr.c_str());

  NS_LOG_UNCOND("Simulation process id: " << ::getpid() << " (parent (waf shell) id: " << ::getppid() << ")");
  NS_LOG_UNCOND("Waiting for Python process to connect");

  bool rxInitReq = false;
  bool rxGetActionSpaceReq = false;
  bool rxGetObsSpaceReq = false;

  ns3opengym::RequestMsg requestPbMsg;

  while (true) {
    requestPbMsg = ReceiveMsg();
    NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );

    if (requestPbMsg.type() == ns3opengym::Init) {
      ns3opengym::InitializeRequest initRequestPbMsg;
      NS_LOG_DEBUG("Received Init request");
      if (requestPbMsg.msg().UnpackTo(&initRequestPbMsg)) {
        NS_LOG_DEBUG("Decoded Init request: step interval: " << initRequestPbMsg.timestep() << " seed: " << initRequestPbMsg.simseed());
        rxInitReq = true;

        ns3opengym::InitializeReply initReplyPbMsg;
        ns3opengym::ReplyMsg replyPbMsg;

        initReplyPbMsg.set_done(true);
        initReplyPbMsg.set_simprocessid(::getpid());
        initReplyPbMsg.set_wafshellprocessid(::getppid());
        replyPbMsg.set_type(ns3opengym::Init);
        replyPbMsg.mutable_msg()->PackFrom(initReplyPbMsg);
        
        SendMsg(replyPbMsg);
      }

    } else if (requestPbMsg.type() == ns3opengym::ActionSpace) {
        ns3opengym::GetActionSpaceRequest actionSpaceRequestPbMsg;
        NS_LOG_DEBUG("Received ActionSpace request");
        rxGetActionSpaceReq = true;

        ns3opengym::SpaceDescription spaceDesc;
        ns3opengym::GetSpaceReply innerReplyPbMsg;
        ns3opengym::ReplyMsg replyPbMsg;

        Ptr<OpenGymSpace> actionSpace = GetActionSpace();
        if (actionSpace) {
          spaceDesc = actionSpace->GetSpaceDescription();
          innerReplyPbMsg.mutable_space()->CopyFrom(spaceDesc);

          replyPbMsg.set_type(ns3opengym::ActionSpace);
          replyPbMsg.mutable_msg()->PackFrom(innerReplyPbMsg);
        }
        
        SendMsg(replyPbMsg);

    } else if (requestPbMsg.type() == ns3opengym::ObservationSpace) {
        ns3opengym::GetObservationSpaceRequest obsSpaceRequestPbMsg;
        NS_LOG_DEBUG("Received ObservationSpace request");
        rxGetObsSpaceReq = true;

        ns3opengym::SpaceDescription spaceDesc;
        ns3opengym::GetSpaceReply innerReplyPbMsg;
        ns3opengym::ReplyMsg replyPbMsg;

        Ptr<OpenGymSpace> obsSpace = GetObservationSpace();
        if (obsSpace) {
          spaceDesc = obsSpace->GetSpaceDescription();
          innerReplyPbMsg.mutable_space()->CopyFrom(spaceDesc);

          replyPbMsg.set_type(ns3opengym::ObservationSpace);
          replyPbMsg.mutable_msg()->PackFrom(innerReplyPbMsg);
        }
        
        SendMsg(replyPbMsg);
    }
    else if (requestPbMsg.type() == ns3opengym::StopEnv) {
      Simulator::Stop();
      m_stopEnvRequested = true;

      ns3opengym::StopEnvReply StopEnvReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;
      StopEnvReplyPbMsg.set_done(true);
      replyPbMsg.set_type(ns3opengym::StopEnv);
      replyPbMsg.mutable_msg()->PackFrom(StopEnvReplyPbMsg);
      SendMsg(replyPbMsg);
      return;

    } else {
        NS_LOG_DEBUG("Received unknown request type");
        ns3opengym::ReplyMsg replyPbMsg;
        replyPbMsg.set_type(ns3opengym::Unknown);
        SendMsg(replyPbMsg);
    }

    // check if ready for next step
    if (rxInitReq && rxGetActionSpaceReq && rxGetObsSpaceReq) {
      break;
    }
  }
}

void
OpenGymInterface::NotifyCurrentState()
{
  NS_LOG_FUNCTION (this);

  if (m_stopEnvRequested) {
    return;
  }

  NS_LOG_DEBUG("Wait for messages");
  ns3opengym::RequestMsg requestPbMsg;

  // collect current env state, TODO: provide possibility to set single callback getState(obs, reward, done)
  Ptr<OpenGymDataContainer> obsDataContainer = GetObservation();
  float reward = GetReward();
  bool isGameOver = IsGameOver();
  std::string extraInfo = GetExtraInfo();

  while (true) {
    requestPbMsg = ReceiveMsg();
    NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );

    if (requestPbMsg.type() == ns3opengym::IsGameOver)
    {
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );
      m_rxGetGameOver = true;

      ns3opengym::GetIsGameOverReply gameOverReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;

      if (isGameOver)
      {
        gameOverReplyPbMsg.set_isgameover(true);
        if (m_simEnd) {
          gameOverReplyPbMsg.set_reason(ns3opengym::GetIsGameOverReply::SimulationEnd);
        } else {
          gameOverReplyPbMsg.set_reason(ns3opengym::GetIsGameOverReply::GameOver);
        }

      } 
      else
      {
        gameOverReplyPbMsg.set_isgameover(false);
      }
      
      replyPbMsg.set_type(ns3opengym::IsGameOver);
      replyPbMsg.mutable_msg()->PackFrom(gameOverReplyPbMsg);
        
      SendMsg(replyPbMsg);
    }
    else if (requestPbMsg.type() == ns3opengym::Observation)
    {
      m_rxGetObservation = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );

      ns3opengym::DataContainer dataContainerPbMsg;
      ns3opengym::GetObservationReply obsReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;

      if (obsDataContainer) {
        obsDataContainer->FillDataContainerPbMsg(dataContainerPbMsg);
        obsReplyPbMsg.mutable_container()->CopyFrom(dataContainerPbMsg);    
        replyPbMsg.set_type(ns3opengym::Observation);
        replyPbMsg.mutable_msg()->PackFrom(obsReplyPbMsg);
      }
        
      SendMsg(replyPbMsg);
    }
    else if (requestPbMsg.type() == ns3opengym::Reward)
    {
      m_rxGetReward = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );

      ns3opengym::GetRewardReply rewardReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;
      rewardReplyPbMsg.set_reward(reward);
     
      replyPbMsg.set_type(ns3opengym::Reward);
      replyPbMsg.mutable_msg()->PackFrom(rewardReplyPbMsg);
        
      SendMsg(replyPbMsg);
    }
    else if (requestPbMsg.type() == ns3opengym::ExtraInfo)
    {
      m_rxGetReward = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );

      ns3opengym::GetExtraInfoReply rewardReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;
      rewardReplyPbMsg.set_info(extraInfo);

      replyPbMsg.set_type(ns3opengym::ExtraInfo);
      replyPbMsg.mutable_msg()->PackFrom(rewardReplyPbMsg);

      SendMsg(replyPbMsg);
    }
    else if (requestPbMsg.type() == ns3opengym::Action)
    {
      m_rxSetActions = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type());

      ns3opengym::SetActionRequest actionRequestPbMsg;
      if (requestPbMsg.msg().UnpackTo(&actionRequestPbMsg)) {

        ns3opengym::DataContainer dataContainerPbMsg = actionRequestPbMsg.container();
        Ptr<OpenGymDataContainer> actDataContainer = OpenGymDataContainer::CreateFromDataContainerPbMsg(dataContainerPbMsg);

        bool done = ExecuteActions(actDataContainer);

        ns3opengym::SetActionReply actionReplyPbMsg;
        ns3opengym::ReplyMsg replyPbMsg;

        actionReplyPbMsg.set_done(done);
       
        replyPbMsg.set_type(ns3opengym::Action);
        replyPbMsg.mutable_msg()->PackFrom(actionReplyPbMsg);
          
        SendMsg(replyPbMsg);
      }
    }
    else if (requestPbMsg.type() == ns3opengym::StopEnv)
    {
      Simulator::Stop();
      m_stopEnvRequested = true;

      ns3opengym::StopEnvReply StopEnvReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;
      StopEnvReplyPbMsg.set_done(true);
      replyPbMsg.set_type(ns3opengym::StopEnv);
      replyPbMsg.mutable_msg()->PackFrom(StopEnvReplyPbMsg);
      SendMsg(replyPbMsg);
      return;
    }
    else
    {
      NS_LOG_DEBUG("Received unknown request type");
      ns3opengym::ReplyMsg replyPbMsg;
      replyPbMsg.set_type(ns3opengym::Unknown);
      SendMsg(replyPbMsg);
    }

    // check if ready for next step
    if (m_rxGetGameOver && m_rxGetObservation && m_rxGetReward && m_rxSetActions) {
      m_rxGetGameOver = false;
      m_rxGetObservation = false;
      m_rxGetReward = false;
      m_rxSetActions = false;
      break;
    }
  }
}

void
OpenGymInterface::WaitForStop()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG("Wait for stop message");

  m_rxGetGameOver = false;
  m_rxGetObservation = false;
  m_rxGetReward = false;
  m_rxSetActions = true;
  NotifyCurrentState();
}


void
OpenGymInterface::NotifySimulationEnd()
{
  NS_LOG_FUNCTION (this);
  m_gameOver = true;
  m_simEnd = true;
  WaitForStop();
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
  return (gameOver || m_gameOver);
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
  if (!m_stateCb.IsNull())
  {
    obs = m_stateCb();
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

