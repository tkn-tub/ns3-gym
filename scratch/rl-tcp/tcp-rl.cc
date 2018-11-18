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

#include "tcp-rl.h"
#include "tcp-rl-env.h"
#include "ns3/tcp-header.h"
#include "ns3/object.h"
#include "ns3/node-list.h"
#include "ns3/core-module.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/tcp-socket-base.h"
#include "ns3/tcp-l4-protocol.h"


namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (TcpSocketDerived);

TypeId
TcpSocketDerived::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpSocketDerived")
    .SetParent<TcpSocketBase> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpSocketDerived> ()
  ;
  return tid;
}

TypeId
TcpSocketDerived::GetInstanceTypeId () const
{
  return TcpSocketDerived::GetTypeId ();
}

TcpSocketDerived::TcpSocketDerived (void)
{
}

Ptr<TcpCongestionOps>
TcpSocketDerived::GetCongestionControlAlgorithm ()
{
  return m_congestionControl;
}

TcpSocketDerived::~TcpSocketDerived (void)
{
}


NS_LOG_COMPONENT_DEFINE ("ns3::TcpRlBase");
NS_OBJECT_ENSURE_REGISTERED (TcpRlBase);

TypeId
TcpRlBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRlBase")
    .SetParent<TcpCongestionOps> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpRlBase> ()
  ;
  return tid;
}

TcpRlBase::TcpRlBase (void)
  : TcpCongestionOps ()
{
  NS_LOG_FUNCTION (this);
  m_tcpSocket = 0;
  m_tcpGymEnv = 0;
}

TcpRlBase::TcpRlBase (const TcpRlBase& sock)
  : TcpCongestionOps (sock)
{
  NS_LOG_FUNCTION (this);
  m_tcpSocket = 0;
  m_tcpGymEnv = 0;
}

TcpRlBase::~TcpRlBase (void)
{
  m_tcpSocket = 0;
  m_tcpGymEnv = 0;
}

uint64_t
TcpRlBase::GenerateUuid ()
{
  static uint64_t uuid = 0;
  uuid++;
  return uuid;
}

void
TcpRlBase::CreateGymEnv()
{
  NS_LOG_FUNCTION (this);
  // should never be called, only child classes: TcpRl and TcpRlTimeBased
}

void
TcpRlBase::ConnectSocketCallbacks()
{
  NS_LOG_FUNCTION (this);

  bool foundSocket = false;
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i) {
    Ptr<Node> node = *i;
    Ptr<TcpL4Protocol> tcp = node->GetObject<TcpL4Protocol> ();

    ObjectVectorValue socketVec;
    tcp->GetAttribute ("SocketList", socketVec);
    NS_LOG_DEBUG("Node: " << node->GetId() << " TCP socket num: " << socketVec.GetN());

    uint32_t sockNum = socketVec.GetN();
    for (uint32_t j=0; j<sockNum; j++) {
      Ptr<Object> sockObj = socketVec.Get(j);
      Ptr<TcpSocketBase> tcpSocket = DynamicCast<TcpSocketBase> (sockObj);
      NS_LOG_DEBUG("Node: " << node->GetId() << " TCP Socket: " << tcpSocket);
      if(!tcpSocket) { continue; }

      Ptr<TcpSocketDerived> dtcpSocket = StaticCast<TcpSocketDerived>(tcpSocket);
      Ptr<TcpCongestionOps> ca = dtcpSocket->GetCongestionControlAlgorithm();
      NS_LOG_DEBUG("CA name: " << ca->GetName());
      Ptr<TcpRlBase> rlCa = DynamicCast<TcpRlBase>(ca);
      if (rlCa == this) {
        NS_LOG_DEBUG("Found TcpRl CA!");
        foundSocket = true;
        m_tcpSocket = tcpSocket;
        break;
      }
    }

    if (foundSocket) {
      break;
    }
  }

  NS_ASSERT_MSG(m_tcpSocket, "TCP socket was not found.");

  if(m_tcpSocket) {
    NS_LOG_DEBUG("Found TCP Socket: " << m_tcpSocket);
    m_tcpSocket->TraceConnectWithoutContext ("Tx", MakeCallback (&TcpGymEnv::TxPktTrace, m_tcpGymEnv));
    m_tcpSocket->TraceConnectWithoutContext ("Rx", MakeCallback (&TcpGymEnv::RxPktTrace, m_tcpGymEnv));
    NS_LOG_DEBUG("Connect socket callbacks " << m_tcpSocket->GetNode()->GetId());
    m_tcpGymEnv->SetNodeId(m_tcpSocket->GetNode()->GetId());
  }
}

std::string
TcpRlBase::GetName () const
{
  return "TcpRlBase";
}

uint32_t
TcpRlBase::GetSsThresh (Ptr<const TcpSocketState> state,
                         uint32_t bytesInFlight)
{
  NS_LOG_FUNCTION (this << state << bytesInFlight);

  if (!m_tcpGymEnv) {
    CreateGymEnv();
  }

  uint32_t newSsThresh = 0;
  if (m_tcpGymEnv) {
      newSsThresh = m_tcpGymEnv->GetSsThresh(state, bytesInFlight);
  }

  return newSsThresh;
}

void
TcpRlBase::IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << tcb << segmentsAcked);

  if (!m_tcpGymEnv) {
    CreateGymEnv();
  }

  if (m_tcpGymEnv) {
     m_tcpGymEnv->IncreaseWindow(tcb, segmentsAcked);
  }
}

void
TcpRlBase::PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt)
{
  NS_LOG_FUNCTION (this);

  if (!m_tcpGymEnv) {
    CreateGymEnv();
  }

  if (m_tcpGymEnv) {
     m_tcpGymEnv->PktsAcked(tcb, segmentsAcked, rtt);
  }
}

void
TcpRlBase::CongestionStateSet (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState)
{
  NS_LOG_FUNCTION (this);

  if (!m_tcpGymEnv) {
    CreateGymEnv();
  }

  if (m_tcpGymEnv) {
     m_tcpGymEnv->CongestionStateSet(tcb, newState);
  }
}

void
TcpRlBase::CwndEvent (Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCAEvent_t event)
{
  NS_LOG_FUNCTION (this);

  if (!m_tcpGymEnv) {
    CreateGymEnv();
  }

  if (m_tcpGymEnv) {
     m_tcpGymEnv->CwndEvent(tcb, event);
  }
}

Ptr<TcpCongestionOps>
TcpRlBase::Fork ()
{
  return CopyObject<TcpRlBase> (this);
}


NS_OBJECT_ENSURE_REGISTERED (TcpRl);

TypeId
TcpRl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRl")
    .SetParent<TcpRlBase> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpRl> ()
    .AddAttribute ("Reward", "Reward when increasing congestion window.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&TcpRl::m_reward),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Penalty", "Reward when increasing congestion window.",
                   DoubleValue (-10.0),
                   MakeDoubleAccessor (&TcpRl::m_penalty),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

TcpRl::TcpRl (void)
  : TcpRlBase ()
{
  NS_LOG_FUNCTION (this);
}

TcpRl::TcpRl (const TcpRl& sock)
  : TcpRlBase (sock)
{
  NS_LOG_FUNCTION (this);
}

TcpRl::~TcpRl (void)
{
}

std::string
TcpRl::GetName () const
{
  return "TcpRl";
}

void
TcpRl::CreateGymEnv()
{
  NS_LOG_FUNCTION (this);
  Ptr<TcpEventGymEnv> env = CreateObject<TcpEventGymEnv>();
  env->SetSocketUuid(TcpRlBase::GenerateUuid());
  env->SetReward(m_reward);
  env->SetPenalty(m_penalty);
  m_tcpGymEnv = env;

  ConnectSocketCallbacks();
}


NS_OBJECT_ENSURE_REGISTERED (TcpRlTimeBased);

TypeId
TcpRlTimeBased::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRlTimeBased")
    .SetParent<TcpRlBase> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpRlTimeBased> ()
    .AddAttribute ("StepTime",
                   "Step interval used in TCP env. Default: 100ms",
                   TimeValue (MilliSeconds (100)),
                   MakeTimeAccessor (&TcpRlTimeBased::m_timeStep),
                   MakeTimeChecker ())
  ;
  return tid;
}

TcpRlTimeBased::TcpRlTimeBased (void) : TcpRlBase ()
{
  NS_LOG_FUNCTION (this);
}

TcpRlTimeBased::TcpRlTimeBased (const TcpRlTimeBased& sock)
  : TcpRlBase (sock)
{
  NS_LOG_FUNCTION (this);
}

TcpRlTimeBased::~TcpRlTimeBased (void)
{
}

std::string
TcpRlTimeBased::GetName () const
{
  return "TcpRlTimeBased";
}

void
TcpRlTimeBased::CreateGymEnv()
{
  NS_LOG_FUNCTION (this);
  Ptr<TcpTimeStepGymEnv> env = CreateObject<TcpTimeStepGymEnv> (m_timeStep);
  env->SetSocketUuid(TcpRlBase::GenerateUuid());
  m_tcpGymEnv = env;

  ConnectSocketCallbacks();
}

} // namespace ns3
