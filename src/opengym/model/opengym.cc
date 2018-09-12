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
#include "opengym.h"
#include "spaces.h"
#include <boost/algorithm/string.hpp>

#include "messages.pb.h"
#include "observation.h"
#include "action.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymEnv");

NS_OBJECT_ENSURE_REGISTERED (OpenGymEnv);


TypeId
OpenGymEnv::GetTypeId (void)
{
  static TypeId tid = TypeId ("OpenGymEnv")
    .SetParent<Object> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<OpenGymEnv> ()
    ;
  return tid;
}

OpenGymEnv::OpenGymEnv() :
  m_zmq_context(1), m_zmq_socket(m_zmq_context, ZMQ_REP), m_gameOver(false)
{
  NS_LOG_FUNCTION (this);
  m_stepCount = 0;
  m_rxGetGameOver = false;
  m_rxGetState = false;
  m_rxGetReward = false;
  m_rxSetActions = false;
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
OpenGymEnv::SetGetActionSpaceCb(Callback< Ptr<OpenGymSpace> > cb)
{
  NS_LOG_FUNCTION (this);
  m_actionSpaceCb = cb;
}

void
OpenGymEnv::SetGetObservationSpaceCb(Callback< Ptr<OpenGymSpace> > cb)
{
  NS_LOG_FUNCTION (this);
  m_observationSpaceCb = cb;
}

void
OpenGymEnv::SetGetGameOverCb(Callback< bool > cb)
{
  NS_LOG_FUNCTION (this);
  m_gameOverCb = cb;
}

void
OpenGymEnv::SetGetStateCb(Callback< Ptr<OpenGymObservation> > cb)
{
  NS_LOG_FUNCTION (this);
  m_stateCb = cb;
}

void
OpenGymEnv::SetGetRewardCb(Callback<float> cb)
{
  NS_LOG_FUNCTION (this);
  m_rewardCb = cb;
}

void
OpenGymEnv::SetExecuteActionsCb(Callback<bool, Ptr<OpenGymAction> > cb)
{
  NS_LOG_FUNCTION (this);
  m_actionCb = cb;
}

ns3opengym::RequestMsg
OpenGymEnv::ReceiveMsg (void)
{
  zmq::message_t request;
  ns3opengym::RequestMsg requestPbMsg;
  m_zmq_socket.recv (&request);
  requestPbMsg.ParseFromArray(request.data(), request.size());
  return requestPbMsg;
}

int
OpenGymEnv::SendMsg(ns3opengym::ReplyMsg replyPbMsg)
{
  zmq::message_t reply(replyPbMsg.ByteSize());;
  replyPbMsg.SerializeToArray(reply.data(), replyPbMsg.ByteSize());
  int n = m_zmq_socket.send (reply);
  return n;
}

void
OpenGymEnv::FillGetSpaceReply(Ptr<OpenGymSpace> space, ns3opengym::GetSpaceReply &spaceReplyPbMsg)
{
  NS_LOG_FUNCTION (this);
  Ptr<OpenGymDiscreteSpace> discrete = DynamicCast<OpenGymDiscreteSpace> (space);
  if (discrete) {
    spaceReplyPbMsg.set_type(ns3opengym::Discrete);
    ns3opengym::DiscreteSpace discreteSpace;
    discreteSpace.set_n(discrete->GetN());
    spaceReplyPbMsg.mutable_space()->PackFrom(discreteSpace);
  }

  Ptr<OpenGymBoxSpace> box = DynamicCast<OpenGymBoxSpace> (space);
  if (box) {
    spaceReplyPbMsg.set_type(ns3opengym::Box);
    ns3opengym::BoxSpace boxSpacePb;
    boxSpacePb.set_low(box->GetLow());
    boxSpacePb.set_high(box->GetHigh());
    std::vector<int> shape = box->GetShape();
    for (auto i = shape.begin(); i != shape.end(); ++i)
    {
      boxSpacePb.add_shape(*i);
    }
    ns3opengym::Dtype dtype = ns3opengym::FLOAT;
    Dtype boxDtype = box->GetDtype();
    if (boxDtype == Dtype::INT)
    {
      dtype = ns3opengym::INT;
    } else if (boxDtype == Dtype::UINT) {
      dtype = ns3opengym::UINT;
    } else if (boxDtype == Dtype::DOUBLE) {
      dtype = ns3opengym::DOUBLE;
    }
    boxSpacePb.set_dtype(dtype);
    spaceReplyPbMsg.mutable_space()->PackFrom(boxSpacePb);
  }
}

void 
OpenGymEnv::Init()
{
  NS_LOG_FUNCTION (this);
  zmq_bind (m_zmq_socket, "tcp://*:5555");

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
        NS_LOG_UNCOND("Decoded Init request: step interval: " << initRequestPbMsg.timestep());
        rxInitReq = true;

        m_interval = Seconds(initRequestPbMsg.timestep());
        Simulator::Schedule (m_interval, &OpenGymEnv::WaitForNextStep, this);

        ns3opengym::InitializeReply initReplyPbMsg;
        ns3opengym::ReplyMsg replyPbMsg;

        initReplyPbMsg.set_done(true);
        replyPbMsg.set_type(ns3opengym::Init);
        replyPbMsg.mutable_msg()->PackFrom(initReplyPbMsg);
        
        SendMsg(replyPbMsg);
      }

    } else if (requestPbMsg.type() == ns3opengym::ActionSpace) {
        ns3opengym::GetActionSpaceRequest actionSpaceRequestPbMsg;
        NS_LOG_DEBUG("Received ActionSpace request");
        rxGetActionSpaceReq = true;

        ns3opengym::GetSpaceReply innerReplyPbMsg;
        ns3opengym::ReplyMsg replyPbMsg;

        Ptr<OpenGymSpace> actionSpace = GetActionSpace();
        FillGetSpaceReply(actionSpace, innerReplyPbMsg);

        replyPbMsg.set_type(ns3opengym::ActionSpace);
        replyPbMsg.mutable_msg()->PackFrom(innerReplyPbMsg);
        
        SendMsg(replyPbMsg);

    } else if (requestPbMsg.type() == ns3opengym::ObservationSpace) {
        ns3opengym::GetObservationSpaceRequest obsSpaceRequestPbMsg;
        NS_LOG_DEBUG("Received ObservationSpace request");
        rxGetObsSpaceReq = true;

        ns3opengym::GetSpaceReply innerReplyPbMsg;
        ns3opengym::ReplyMsg replyPbMsg;

        Ptr<OpenGymSpace> obsSpace = GetObservationSpace();
        FillGetSpaceReply(obsSpace, innerReplyPbMsg);

        replyPbMsg.set_type(ns3opengym::ObservationSpace);
        replyPbMsg.mutable_msg()->PackFrom(innerReplyPbMsg);
        
        SendMsg(replyPbMsg);

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
OpenGymEnv::WaitForNextStep()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG("Wait for messages");
  Simulator::Schedule (m_interval, &OpenGymEnv::WaitForNextStep, this);

  ns3opengym::RequestMsg requestPbMsg;

  while (true) {
    requestPbMsg = ReceiveMsg();
    NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );

    if (requestPbMsg.type() == ns3opengym::IsGameOver)
    {
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );
      m_rxGetGameOver = true;

      ns3opengym::GetIsGameOverReply gameOverReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;

      if (IsGameOver())
      {
        gameOverReplyPbMsg.set_isgameover(true);
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
      m_rxGetState = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );

      ns3opengym::GetObservationReply obsReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;

      Ptr<OpenGymObservation> obs = GetState();
      // TODO: encode obs properly
      obsReplyPbMsg.set_observation(obs->m_obsString);
     
      replyPbMsg.set_type(ns3opengym::Observation);
      replyPbMsg.mutable_msg()->PackFrom(obsReplyPbMsg);
        
      SendMsg(replyPbMsg);
    }
    else if (requestPbMsg.type() == ns3opengym::Reward)
    {
      m_rxGetReward = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type() );

      ns3opengym::GetRewardReply rewardReplyPbMsg;
      ns3opengym::ReplyMsg replyPbMsg;

      float reward = GetReward();
      rewardReplyPbMsg.set_reward(reward);
     
      replyPbMsg.set_type(ns3opengym::Reward);
      replyPbMsg.mutable_msg()->PackFrom(rewardReplyPbMsg);
        
      SendMsg(replyPbMsg);
    }
    else if (requestPbMsg.type() == ns3opengym::Action)
    {
      m_stepCount++;
      m_rxSetActions = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type());

      ns3opengym::SetActionRequest actionRequestPbMsg;
      if (requestPbMsg.msg().UnpackTo(&actionRequestPbMsg)) {

        Ptr<OpenGymAction> action = CreateObject<OpenGymAction> ();
        // TODO: encode action properly
        action->m_actionString = actionRequestPbMsg.action();
        ExecuteActions(action);

        ns3opengym::SetActionReply actionReplyPbMsg;
        ns3opengym::ReplyMsg replyPbMsg;

        actionReplyPbMsg.set_done(true);
       
        replyPbMsg.set_type(ns3opengym::Action);
        replyPbMsg.mutable_msg()->PackFrom(actionReplyPbMsg);
          
        SendMsg(replyPbMsg);
      }
    }
    else
    {
      NS_LOG_DEBUG("Received unknown request type");
      ns3opengym::ReplyMsg replyPbMsg;
      replyPbMsg.set_type(ns3opengym::Unknown);
      SendMsg(replyPbMsg);
    }

    // check if ready for next step
    if (m_rxGetGameOver && m_rxGetState && m_rxGetReward && m_rxSetActions) {
      m_rxGetGameOver = false;
      m_rxGetState = false;
      m_rxGetReward = false;
      m_rxSetActions = false;
      break;
    }
  }
}

void
OpenGymEnv::WaitForStop()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG("Wait for stop message");

  m_rxGetGameOver = false;
  m_rxGetState = false;
  m_rxGetReward = false;
  m_rxSetActions = true;
  WaitForNextStep();
}


void
OpenGymEnv::SetGameOver()
{
  NS_LOG_FUNCTION (this);
  m_gameOver = true;
}

bool
OpenGymEnv::IsGameOver()
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
OpenGymEnv::GetActionSpace()
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
OpenGymEnv::GetObservationSpace()
{
  NS_LOG_FUNCTION (this);
  Ptr<OpenGymSpace> obsSpace;
  if (!m_observationSpaceCb.IsNull())
  {
    obsSpace = m_observationSpaceCb();
  }
  return obsSpace;
}

Ptr<OpenGymObservation>
OpenGymEnv::GetState()
{
  NS_LOG_FUNCTION (this);
  Ptr<OpenGymObservation>  obs;
  if (!m_stateCb.IsNull())
  {
    obs = m_stateCb();
  }
  return obs;
}

float
OpenGymEnv::GetReward()
{
  NS_LOG_FUNCTION (this);
  float reward = 0.0;
  if (!m_rewardCb.IsNull())
  {
    reward = m_rewardCb();
  }
  return reward;
}

bool
OpenGymEnv::ExecuteActions(Ptr<OpenGymAction> action)
{
  NS_LOG_FUNCTION (this);
  bool reply = false;
  if (!m_actionCb.IsNull())
  {
    reply = m_actionCb(action);
  }
  return reply;
}

}

