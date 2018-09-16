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
#include "container.h"
#include "entity.h"

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

OpenGymEnv::OpenGymEnv(uint32_t port, Time stepTime):
  m_port(port), m_zmq_context(1), m_zmq_socket(m_zmq_context, ZMQ_REP),
  m_gameOver(false), m_simEnd(false), m_stopEnvRequested(false)
{
  NS_LOG_FUNCTION (this);
  m_stepCount = 0;
  m_interval = stepTime;
  m_rxGetGameOver = false;
  m_rxGetObservation = false;
  m_rxGetReward = false;
  m_rxGetExtraInfo = false;
  m_rxSetActions = false;
  Simulator::Schedule (Seconds(0.0), &OpenGymEnv::Init, this);
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
OpenGymEnv::SetGetObservationCb(Callback< Ptr<OpenGymDataContainer> > cb)
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
OpenGymEnv::SetGetExtraInfoCb(Callback<std::string> cb)
{
  NS_LOG_FUNCTION (this);
  m_extraInfoCb = cb;
}

void
OpenGymEnv::SetExecuteActionsCb(Callback<bool, Ptr<OpenGymDataContainer> > cb)
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
    std::vector<uint32_t> shape = box->GetShape();
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
  std::string bindAddr = "tcp://*:" + std::to_string(m_port);
  zmq_bind (m_zmq_socket, bindAddr.c_str());

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
        NS_LOG_UNCOND("Decoded Init request: step interval: " << initRequestPbMsg.timestep() << " seed: " << initRequestPbMsg.simseed());
        rxInitReq = true;

        double timeStep = initRequestPbMsg.timestep();
        if (timeStep > 0) {
          m_interval = Seconds(timeStep);
        }
        Simulator::Schedule (Seconds(0.0), &OpenGymEnv::WaitForNextStep, this);

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
OpenGymEnv::WaitForNextStep()
{
  NS_LOG_FUNCTION (this);

  if (m_stopEnvRequested) {
    return;
  }

  NS_LOG_DEBUG("Wait for messages");
  Simulator::Schedule (m_interval, &OpenGymEnv::WaitForNextStep, this);

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

      // TODO: add serialzation of Discrete Container

      ns3opengym::BoxDataContainer boxContainerPbMsg;
      std::vector<uint32_t> shape;
      Dtype dtype = obsDataContainer->GetDataType();

      if (dtype == Dtype::INT) {
        Ptr<OpenGymBoxContainer<int32_t> > box = DynamicCast<OpenGymBoxContainer<int32_t> >(obsDataContainer);
        shape = box->GetShape();
        boxContainerPbMsg.set_dtype(ns3opengym::INT);
        std::vector<int32_t> data = box->GetData();
        *boxContainerPbMsg.mutable_intdata() = {data.begin(), data.end()};

      } else if (dtype == Dtype::UINT) {
        Ptr<OpenGymBoxContainer<uint32_t> > box = DynamicCast<OpenGymBoxContainer<uint32_t> >(obsDataContainer);
        shape = box->GetShape();
        boxContainerPbMsg.set_dtype(ns3opengym::UINT);
        std::vector<uint32_t> data = box->GetData();
        *boxContainerPbMsg.mutable_uintdata() = {data.begin(), data.end()};

      } else if (dtype == Dtype::FLOAT) {
        Ptr<OpenGymBoxContainer<float> > box = DynamicCast<OpenGymBoxContainer<float> >(obsDataContainer);
        shape = box->GetShape();
        boxContainerPbMsg.set_dtype(ns3opengym::FLOAT);
        std::vector<float> data = box->GetData();
        *boxContainerPbMsg.mutable_floatdata() = {data.begin(), data.end()};

      } else {
        Ptr<OpenGymBoxContainer<float> > box = DynamicCast<OpenGymBoxContainer<float> >(obsDataContainer);
        shape = box->GetShape();
        boxContainerPbMsg.set_dtype(ns3opengym::FLOAT);
        std::vector<float> data = box->GetData();
        *boxContainerPbMsg.mutable_floatdata() = {data.begin(), data.end()};
      }


      dataContainerPbMsg.set_type(ns3opengym::Box);
      *boxContainerPbMsg.mutable_shape() = {shape.begin(), shape.end()};
      dataContainerPbMsg.mutable_data()->PackFrom(boxContainerPbMsg);

      obsReplyPbMsg.mutable_container()->CopyFrom(dataContainerPbMsg);    
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
      m_stepCount++;
      m_rxSetActions = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestPbMsg.type());

      ns3opengym::SetActionRequest actionRequestPbMsg;
      if (requestPbMsg.msg().UnpackTo(&actionRequestPbMsg)) {

        Ptr<OpenGymDataContainer> actDataContainer = CreateObject<OpenGymDataContainer>();
        ns3opengym::DataContainer containerPbMsg = actionRequestPbMsg.container();

        if (containerPbMsg.type() == ns3opengym::Discrete)
        {
          // TODO implement
        }
        else if (containerPbMsg.type() == ns3opengym::Box)
        {
          ns3opengym::BoxDataContainer boxContainerPbMsg;
          containerPbMsg.data().UnpackTo(&boxContainerPbMsg);

          if (boxContainerPbMsg.dtype() == ns3opengym::INT) {
            Ptr<OpenGymBoxContainer<int32_t> > box = CreateObject<OpenGymBoxContainer<int32_t> >();
            std::vector<int32_t> myData;
            //myData.reserve(boxContainerPbMsg.uintdata().size());
            myData.assign(boxContainerPbMsg.intdata().begin(), boxContainerPbMsg.intdata().end()); 
            box->SetData(myData);
            actDataContainer = box;

          } else if (boxContainerPbMsg.dtype() == ns3opengym::UINT) {
            Ptr<OpenGymBoxContainer<uint32_t> > box = CreateObject<OpenGymBoxContainer<uint32_t> >();
            std::vector<uint32_t> myData;
            //myData.reserve(boxContainerPbMsg.uintdata().size());
            myData.assign(boxContainerPbMsg.uintdata().begin(), boxContainerPbMsg.uintdata().end()); 
            box->SetData(myData);
            actDataContainer = box;

          } else if (boxContainerPbMsg.dtype() == ns3opengym::FLOAT) {
            Ptr<OpenGymBoxContainer<float> > box = CreateObject<OpenGymBoxContainer<float> >();
            std::vector<float> myData;
            std::cout << "DECOFDING FLOAT" << std::endl;
            //myData.reserve(boxContainerPbMsg.uintdata().size());
            myData.assign(boxContainerPbMsg.floatdata().begin(), boxContainerPbMsg.floatdata().end()); 
            box->SetData(myData);
            actDataContainer = box;

          } else if (boxContainerPbMsg.dtype() == ns3opengym::DOUBLE) {
            Ptr<OpenGymBoxContainer<double> > box = CreateObject<OpenGymBoxContainer<double> >();
            std::vector<double> myData;
            //myData.reserve(boxContainerPbMsg.uintdata().size());
            myData.assign(boxContainerPbMsg.doubledata().begin(), boxContainerPbMsg.doubledata().end()); 
            box->SetData(myData);
            actDataContainer = box;

          } else {
            Ptr<OpenGymBoxContainer<float> > box = CreateObject<OpenGymBoxContainer<float> >();
            std::vector<float> myData;
            //myData.reserve(boxContainerPbMsg.uintdata().size());
            myData.assign(boxContainerPbMsg.floatdata().begin(), boxContainerPbMsg.floatdata().end()); 
            box->SetData(myData);
            actDataContainer = box;
          }
        }

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
OpenGymEnv::WaitForStop()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG("Wait for stop message");

  m_rxGetGameOver = false;
  m_rxGetObservation = false;
  m_rxGetReward = false;
  m_rxSetActions = true;
  WaitForNextStep();
}


void
OpenGymEnv::NotifySimulationEnd()
{
  NS_LOG_FUNCTION (this);
  m_gameOver = true;
  m_simEnd = true;
  WaitForStop();
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

Ptr<OpenGymDataContainer>
OpenGymEnv::GetObservation()
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

std::string
OpenGymEnv::GetExtraInfo()
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
OpenGymEnv::ExecuteActions(Ptr<OpenGymDataContainer> action)
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
OpenGymEnv::Notify(Ptr<OpenGymEntity> entity)
{
  NS_LOG_FUNCTION (this);

  //TODO: collect current state to be sent to Python
  // call next step to initiate communication with python
  // execute actions on entitiy
}

}

