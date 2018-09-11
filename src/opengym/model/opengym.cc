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
#include <boost/algorithm/string.hpp>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OpenGymEnv");

NS_OBJECT_ENSURE_REGISTERED (OpenGymEnv);


CmdMessage::CmdMessage()
{
  NS_LOG_FUNCTION (this);
}

CmdMessage::~CmdMessage ()
{
  NS_LOG_FUNCTION (this);
}


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

template <class Container>
void Deserialize(const std::string& str, Container& cont,
              const std::string& delims = " ")
{
    boost::split(cont, str, boost::is_any_of(delims));
}

void
OpenGymEnv::SetGetActionSpaceCb(Callback<std::string> cb)
{
  NS_LOG_FUNCTION (this);
  m_actionSpaceCb = cb;
}

void
OpenGymEnv::SetGetObservationSpaceCb(Callback<std::string> cb)
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
OpenGymEnv::SetGetStateCb(Callback<std::string> cb)
{
  NS_LOG_FUNCTION (this);
  m_stateCb = cb;
}

void
OpenGymEnv::SetGetRewardCb(Callback<std::string> cb)
{
  NS_LOG_FUNCTION (this);
  m_rewardCb = cb;
}

void
OpenGymEnv::SetExecuteActionsCb(Callback<bool, std::string> cb)
{
  NS_LOG_FUNCTION (this);
  m_actionCb = cb;
}

CmdMessage
OpenGymEnv::ReceiveMsg (void)
{
  zmq::message_t request;
  std::string requestStr;
  std::vector<std::string> seglist;

  //  Wait for next request from client
  m_zmq_socket.recv (&request);
  requestStr = std::string(static_cast<char*>(request.data()), request.size());
  // std::cout << "Received request: " << requestStr <<  std::endl;

  seglist.clear();
  Deserialize(requestStr, seglist, "|");

  int msgType = atoi(seglist[0].c_str());

  CmdMessage requestCmd = CmdMessage();
  requestCmd.type = msgType;
  if (msgType == 5)
  {
    requestCmd.valueStr = seglist[1];
  } else {
    float msgValue = strtof(seglist[1].c_str(), 0);
    requestCmd.value = msgValue;
  }

  return requestCmd;
}


int
OpenGymEnv::SendMsg(std::string msg)
{
  NS_LOG_DEBUG("Send reply: " << msg << " length: " << msg.length());
  zmq::message_t reply (msg.length());
  memcpy (reply.data (), msg.c_str(), msg.length());
  int n = m_zmq_socket.send (reply);
  return n;
}


void 
OpenGymEnv::Init()
{
  NS_LOG_FUNCTION (this);
  zmq_bind (m_zmq_socket, "tcp://*:5555");

  CmdMessage requestCmd;

  bool rxInitReq = false;
  bool rxGetActionSpaceReq = false;

  while (true) {
    requestCmd = ReceiveMsg();
    NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type );

    if (requestCmd.type == 0)
    {
      rxInitReq = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.value);
      m_interval = Seconds(requestCmd.value);
      Simulator::Schedule (m_interval, &OpenGymEnv::WaitForNextStep, this);
      NS_LOG_DEBUG("Received INIT message, step time interval = " << m_interval);
      SendMsg("0|0");
    }
    else if (requestCmd.type == 1)
    {
      rxGetActionSpaceReq = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.value);
      std::string reply;
      reply = GetActionSpace();
      reply = "0|" + reply;
      SendMsg(reply);
      break;
    }
    else {
      NS_LOG_DEBUG("Received unknown request type");
      SendMsg("1|1");
    }

    // check if ready for next step
    if (rxInitReq && rxGetActionSpaceReq) {
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

  CmdMessage requestCmd;
  std::string reply;

  bool getGameOver = false;
  bool getState = false;
  bool getReward = false;
  bool setActions = false;

  // wait for get IsGameOver command
  while (true) {
    requestCmd = ReceiveMsg();
    NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type );

    if (requestCmd.type == 2)
    {
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.value);
      getGameOver = true;
      if (IsGameOver())
      {
        SendMsg("0|1");
        // if simulation is over then return
        // return;
      } 
      else
      {
        SendMsg("0|0");
      }
    }
    else if (requestCmd.type == 3)
    {
      getState = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.value);
      reply = GetState();
      reply = "0|" + reply;
      SendMsg(reply);
    }
    else if (requestCmd.type == 4)
    {
      getReward = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.value);
      reply = GetReward();
      reply = "0|" + reply;
      SendMsg(reply);
    }
    else if (requestCmd.type == 5)
    {
      setActions = true;
      m_stepCount++;
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.valueStr);
      ExecuteActions(requestCmd.valueStr);
      SendMsg("0|0");
    }
    else
    {
      NS_LOG_DEBUG("Received unknown request type");
      SendMsg("1|1");
    }

    // check if ready for next step
    if (getGameOver && getState && getReward && setActions) {
      getGameOver = false;
      getState = false;
      getReward = false;
      setActions = false;
      break;
    }
  }
}

void
OpenGymEnv::WaitForStop()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG("Wait for messages");
  Simulator::Schedule (m_interval, &OpenGymEnv::WaitForNextStep, this);

  CmdMessage requestCmd;
  std::string reply;

  bool getGameOver = false;
  bool getState = false;
  bool getReward = false;

  // wait for get IsGameOver command
  while (true) {
    requestCmd = ReceiveMsg();
    NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type );

    if (requestCmd.type == 2)
    {
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.value);
      getGameOver = true;
      if (IsGameOver())
      {
        SendMsg("0|1");
        // if simulation is over then return
        // return;
      } 
      else
      {
        SendMsg("0|0");
      }
    }
    else if (requestCmd.type == 3)
    {
      getState = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.value);
      reply = GetState();
      reply = "0|" + reply;
      SendMsg(reply);
    }
    else if (requestCmd.type == 4)
    {
      getReward = true;
      NS_LOG_DEBUG("Received request: msgType: " << requestCmd.type << " msgValue: " << requestCmd.value);
      reply = GetReward();
      reply = "0|" + reply;
      SendMsg(reply);
    }
    else
    {
      NS_LOG_DEBUG("Received unknown request type");
      SendMsg("1|1");
    }

    // check if ready for next step
    if (getGameOver && getState && getReward) {
      getGameOver = false;
      getState = false;
      getReward = false;
      break;
    }
  }
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
  return m_gameOver;
}

std::string
OpenGymEnv::GetActionSpace()
{
  NS_LOG_FUNCTION (this);
  std::string reply;
  if (!m_actionSpaceCb.IsNull())
  {
    reply = m_actionSpaceCb();
  }
  return reply;
}

std::string
OpenGymEnv::GetState()
{
  NS_LOG_FUNCTION (this);
  std::string reply;
  if (!m_stateCb.IsNull())
  {
    reply = m_stateCb();
  }
  return reply;
}

std::string
OpenGymEnv::GetReward()
{
  NS_LOG_FUNCTION (this);
  std::string reply;
  if (!m_rewardCb.IsNull())
  {
    reply = m_rewardCb();
  }
  return reply;
}

bool
OpenGymEnv::ExecuteActions(std::string actionString)
{
  NS_LOG_FUNCTION (this);
  bool reply = false;
  if (!m_actionCb.IsNull())
  {
    reply = m_actionCb(actionString);
  }
  return reply;
}

}

