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

#ifndef OPENGYM_H
#define OPENGYM_H

#include "ns3/object.h"
#include "ns3/simulator.h"
#include <map>

#include <zmq.hpp>
#include <string>
#include <iostream>

namespace ns3 {

class CmdMessage
{
  public:
  CmdMessage ();
  virtual ~CmdMessage ();

  int type;
  float value;
  std::string valueStr;
};

class OpenGymEnv : public Object
{
public:
  OpenGymEnv ();
  virtual ~OpenGymEnv ();

  static TypeId GetTypeId ();

  void Init();
  CmdMessage ReceiveMsg();
  int SendMsg(std::string msg);
  void WaitForNextStep();
  void WaitForStop();

  void SetGameOver();
  bool IsGameOver();

  std::string GetActionSpace();
  std::string GetState();
  std::string GetReward();
  bool ExecuteActions(std::string actionString);

  void SetGetActionSpaceCb(Callback<std::string> cb);
  void SetGetStateCb(Callback<std::string> cb);
  void SetGetRewardCb(Callback<std::string> cb);
  void SetExecuteActionsCb(Callback<bool, std::string> cb);

protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

private:
  zmq::context_t m_zmq_context;
  zmq::socket_t m_zmq_socket;
  bool m_gameOver;
  uint64_t m_stepCount;

  Time m_interval;

  Callback<std::string> m_actionSpaceCb;
  Callback<std::string> m_stateCb;
  Callback<std::string> m_rewardCb;
  Callback<bool, std::string> m_actionCb;

};

} // end of namespace ns3

#endif /* OPENGYM_H */

