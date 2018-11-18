/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 Technische Universität Berlin
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
 * Author: Piotr Gawlowicz <gawlowicz@tkn.tu-berlin.de>
 */

#ifndef TCP_RL_ENV_H
#define TCP_RL_ENV_H

#include "ns3/opengym-module.h"
#include "ns3/tcp-socket-base.h"
#include <vector>

namespace ns3 {

class Packet;
class TcpHeader;
class TcpSocketBase;
class Time;


class TcpGymEnv : public OpenGymEnv
{
public:
  TcpGymEnv ();
  virtual ~TcpGymEnv ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  void SetNodeId(uint32_t id);
  void SetSocketUuid(uint32_t id);

  std::string GetTcpCongStateName(const TcpSocketState::TcpCongState_t state);
  std::string GetTcpCAEventName(const TcpSocketState::TcpCAEvent_t event);

  // OpenGym interface
  virtual Ptr<OpenGymSpace> GetActionSpace();
  virtual bool GetGameOver();
  virtual float GetReward();
  virtual std::string GetExtraInfo();
  virtual bool ExecuteActions(Ptr<OpenGymDataContainer> action);

  virtual Ptr<OpenGymSpace> GetObservationSpace() = 0;
  virtual Ptr<OpenGymDataContainer> GetObservation() = 0;

  // trace packets, e.g. for calculating inter tx/rx time
  virtual void TxPktTrace(Ptr<const Packet>, const TcpHeader&, Ptr<const TcpSocketBase>) = 0;
  virtual void RxPktTrace(Ptr<const Packet>, const TcpHeader&, Ptr<const TcpSocketBase>) = 0;

  // TCP congestion control interface
  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight) = 0;
  virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked) = 0;
  // optional functions used to collect obs
  virtual void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt) = 0;
  virtual void CongestionStateSet (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState) = 0;
  virtual void CwndEvent (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCAEvent_t event) = 0;

  typedef enum
  {
    GET_SS_THRESH = 0,
    INCREASE_WINDOW,
    PKTS_ACKED,
    CONGESTION_STATE_SET,
    CWND_EVENT,
  } CalledFunc_t;

protected:
  uint32_t m_nodeId;
  uint32_t m_socketUuid;

  // state
  // obs has to be implemented in child class

  // game over
  bool m_isGameOver;

  // reward
  float m_envReward;

  // extra info
  std::string m_info;

  // actions
  uint32_t m_new_ssThresh;
  uint32_t m_new_cWnd;
};


class TcpEventGymEnv : public TcpGymEnv
{
public:
  TcpEventGymEnv ();
  virtual ~TcpEventGymEnv ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  void SetReward(float value);
  void SetPenalty(float value);

  // OpenGym interface
  virtual Ptr<OpenGymSpace> GetObservationSpace();
  Ptr<OpenGymDataContainer> GetObservation();

  // trace packets, e.g. for calculating inter tx/rx time
  virtual void TxPktTrace(Ptr<const Packet>, const TcpHeader&, Ptr<const TcpSocketBase>);
  virtual void RxPktTrace(Ptr<const Packet>, const TcpHeader&, Ptr<const TcpSocketBase>);

  // TCP congestion control interface
  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight);
  virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);
  // optional functions used to collect obs
  virtual void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt);
  virtual void CongestionStateSet (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState);
  virtual void CwndEvent (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCAEvent_t event);

private:
  // state
  CalledFunc_t m_calledFunc;
  Ptr<const TcpSocketState> m_tcb;
  uint32_t m_bytesInFlight;
  uint32_t m_segmentsAcked;
  Time m_rtt;
  TcpSocketState::TcpCongState_t m_newState;
  TcpSocketState::TcpCAEvent_t m_event;

  // reward
  float m_reward;
  float m_penalty;
};


class TcpTimeStepGymEnv : public TcpGymEnv
{
public:
  TcpTimeStepGymEnv ();
  TcpTimeStepGymEnv (Time timeStep);
  virtual ~TcpTimeStepGymEnv ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  // OpenGym interface
  virtual Ptr<OpenGymSpace> GetObservationSpace();
  Ptr<OpenGymDataContainer> GetObservation();

  // trace packets, e.g. for calculating inter tx/rx time
  virtual void TxPktTrace(Ptr<const Packet>, const TcpHeader&, Ptr<const TcpSocketBase>);
  virtual void RxPktTrace(Ptr<const Packet>, const TcpHeader&, Ptr<const TcpSocketBase>);

  // TCP congestion control interface
  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight);
  virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);
  // optional functions used to collect obs
  virtual void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt);
  virtual void CongestionStateSet (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState);
  virtual void CwndEvent (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCAEvent_t event);

private:
  void ScheduleNextStateRead();
  bool m_started {false};
  Time m_timeStep;
  // state
  Ptr<const TcpSocketState> m_tcb;
  std::vector<uint32_t> m_bytesInFlight;
  std::vector<uint32_t> m_segmentsAcked;

  uint64_t m_rttSampleNum {0};
  Time m_rttSum {MicroSeconds (0.0)};

  Time m_lastPktTxTime {MicroSeconds(0.0)};
  Time m_lastPktRxTime {MicroSeconds(0.0)};
  uint64_t m_interTxTimeNum {0};
  Time m_interTxTimeSum {MicroSeconds (0.0)};
  uint64_t m_interRxTimeNum {0};
  Time m_interRxTimeSum {MicroSeconds (0.0)};

  // reward
};



} // namespace ns3

#endif /* TCP_RL_ENV_H */