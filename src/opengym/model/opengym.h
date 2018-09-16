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
#include "messages.pb.h"

namespace ns3 {

class OpenGymSpace;
class OpenGymDataContainer;
class OpenGymEntity;

class OpenGymEnv : public Object
{
public:
  OpenGymEnv (uint32_t port=5555, Time stepTime=Seconds(0.1));
  virtual ~OpenGymEnv ();

  static TypeId GetTypeId ();

  void Init();
  ns3opengym::RequestMsg ReceiveMsg();
  int SendMsg(ns3opengym::ReplyMsg replyPbMsg);

  void WaitForNextStep();
  void WaitForStop();

  void NotifySimulationEnd();
  bool IsGameOver();

  Ptr<OpenGymSpace> GetActionSpace();
  Ptr<OpenGymSpace> GetObservationSpace();
  void FillGetSpaceReply(Ptr<OpenGymSpace> space, ns3opengym::GetSpaceReply &spaceReplyPbMsg);
  Ptr<OpenGymDataContainer> GetObservation();
  float GetReward();
  bool ExecuteActions(Ptr<OpenGymDataContainer> action);

  void SetGetActionSpaceCb(Callback< Ptr<OpenGymSpace> > cb);
  void SetGetObservationSpaceCb(Callback< Ptr<OpenGymSpace> > cb);
  void SetGetGameOverCb(Callback< bool > cb);
  void SetGetObservationCb(Callback< Ptr<OpenGymDataContainer> > cb);
  void SetGetRewardCb(Callback<float> cb);
  void SetExecuteActionsCb(Callback<bool, Ptr<OpenGymDataContainer> > cb);

  void Notify(Ptr<OpenGymEntity> entity);

protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

private:
  uint32_t m_port;
  zmq::context_t m_zmq_context;
  zmq::socket_t m_zmq_socket;
  bool m_gameOver;
  bool m_simEnd;
  bool m_stopEnvRequested;
  uint64_t m_stepCount;

  Time m_interval;

  Callback< Ptr<OpenGymSpace> > m_actionSpaceCb;
  Callback< Ptr<OpenGymSpace> > m_observationSpaceCb;
  Callback< bool > m_gameOverCb;
  Callback< Ptr<OpenGymDataContainer> > m_stateCb;
  Callback<float> m_rewardCb;
  Callback<bool, Ptr<OpenGymDataContainer> > m_actionCb;

  bool m_rxGetGameOver;
  bool m_rxGetObservation;
  bool m_rxGetReward;
  bool m_rxSetActions;
};

} // end of namespace ns3

#endif /* OPENGYM_H */

