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

#include "mygym.h"
#include "ns3/object.h"
#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/node-list.h"
#include "ns3/log.h"
#include <sstream>
#include <iostream>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MyGymEnv");

NS_OBJECT_ENSURE_REGISTERED (MyGymEnv);

MyGymEnv::MyGymEnv ()
{
  NS_LOG_FUNCTION (this);
  m_currentNode = 0;
  m_rxPktNum = 0;
}

MyGymEnv::MyGymEnv (Time stepTime)
{
  NS_LOG_FUNCTION (this);
  m_currentNode = 0;
  m_rxPktNum = 0;
  m_interval = stepTime;

  Simulator::Schedule (Seconds(0.0), &MyGymEnv::ScheduleNextStateRead, this);
}

void
MyGymEnv::ScheduleNextStateRead ()
{
  NS_LOG_FUNCTION (this);
  Simulator::Schedule (m_interval, &MyGymEnv::ScheduleNextStateRead, this);
  Notify();
}

MyGymEnv::~MyGymEnv ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
MyGymEnv::GetTypeId (void)
{
  static TypeId tid = TypeId ("MyGymEnv")
    .SetParent<OpenGymEnv> ()
    .SetGroupName ("OpenGym")
    .AddConstructor<MyGymEnv> ()
  ;
  return tid;
}

void
MyGymEnv::DoDispose ()
{
  NS_LOG_FUNCTION (this);
}

Ptr<OpenGymSpace>
MyGymEnv::GetActionSpace()
{
  NS_LOG_FUNCTION (this);
  uint32_t nodeNum = NodeList::GetNNodes ();
  float low = 0.0;
  float high = 100.0;
  std::vector<uint32_t> shape = {nodeNum,};
  std::string dtype = TypeNameGet<uint32_t> ();
  Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace> (low, high, shape, dtype);
  NS_LOG_UNCOND ("GetActionSpace: " << space);
  return space;
}

Ptr<OpenGymSpace>
MyGymEnv::GetObservationSpace()
{
  NS_LOG_FUNCTION (this);
  uint32_t nodeNum = NodeList::GetNNodes ();
  float low = 0.0;
  float high = 100.0;
  std::vector<uint32_t> shape = {nodeNum,};
  std::string dtype = TypeNameGet<uint32_t> ();
  Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace> (low, high, shape, dtype);
  NS_LOG_UNCOND ("GetObservationSpace: " << space);
  return space;
}

bool
MyGymEnv::GetGameOver()
{
  NS_LOG_FUNCTION (this);
  bool isGameOver = false;
  NS_LOG_UNCOND ("MyGetGameOver: " << isGameOver);
  return isGameOver;
}

Ptr<WifiMacQueue>
MyGymEnv::GetQueue(Ptr<Node> node)
{
  Ptr<NetDevice> dev = node->GetDevice (0);
  Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice> (dev);
  Ptr<WifiMac> wifi_mac = wifi_dev->GetMac ();
  PointerValue ptr;
  wifi_mac->GetAttribute ("Txop", ptr);
  Ptr<Txop> txop = ptr.Get<Txop> ();
  Ptr<WifiMacQueue> queue = txop->GetWifiMacQueue ();
  return queue;
}

Ptr<OpenGymDataContainer>
MyGymEnv::GetObservation()
{
  NS_LOG_FUNCTION (this);
  uint32_t nodeNum = NodeList::GetNNodes ();
  std::vector<uint32_t> shape = {nodeNum,};
  Ptr<OpenGymBoxContainer<uint32_t> > box = CreateObject<OpenGymBoxContainer<uint32_t> >(shape);

  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i) {
    Ptr<Node> node = *i;
    Ptr<WifiMacQueue> queue = GetQueue (node);
    uint32_t value = queue->GetNPackets();
    box->AddValue(value);
  }

  NS_LOG_UNCOND ("MyGetObservation: " << box);
  return box;
}

float
MyGymEnv::GetReward()
{
  NS_LOG_FUNCTION (this);
  static float lastValue = 0.0;
  float reward = m_rxPktNum - lastValue;
  lastValue = m_rxPktNum;
  NS_LOG_UNCOND ("MyGetReward: " << reward);
  return reward;
}

std::string
MyGymEnv::GetExtraInfo()
{
  NS_LOG_FUNCTION (this);
  std::string myInfo = "currentNodeId";
  myInfo += "=";
  if (m_currentNode) {
    myInfo += std::to_string(m_currentNode->GetId());
  }
  NS_LOG_UNCOND("MyGetExtraInfo: " << myInfo);
  return myInfo;
}

bool
MyGymEnv::SetCw(Ptr<Node> node, uint32_t cwMinValue, uint32_t cwMaxValue)
{
  Ptr<NetDevice> dev = node->GetDevice (0);
  Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice> (dev);
  Ptr<WifiMac> wifi_mac = wifi_dev->GetMac ();
  PointerValue ptr;
  wifi_mac->GetAttribute ("Txop", ptr);
  Ptr<Txop> txop = ptr.Get<Txop> ();

  // if both set to the same value then we have uniform backoff?
  if (cwMinValue != 0) {
    NS_LOG_DEBUG ("Set CW min: " << cwMinValue);
    txop->SetMinCw(cwMinValue);
  }

  if (cwMaxValue != 0) {
    NS_LOG_DEBUG ("Set CW max: " << cwMaxValue);
    txop->SetMaxCw(cwMaxValue);
  }
  return true;
}

bool
MyGymEnv::ExecuteActions(Ptr<OpenGymDataContainer> action)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_UNCOND ("MyExecuteActions: " << action);
  Ptr<OpenGymBoxContainer<uint32_t> > box = DynamicCast<OpenGymBoxContainer<uint32_t> >(action);
  std::vector<uint32_t> actionVector = box->GetData();

  uint32_t nodeNum = NodeList::GetNNodes ();
  for (uint32_t i=0; i<nodeNum; i++)
  {
    Ptr<Node> node = NodeList::GetNode(i);
    uint32_t cwSize = actionVector.at(i);
    SetCw(node, cwSize, cwSize);
  }

  return true;
}

void
MyGymEnv::NotifyPktRxEvent(Ptr<MyGymEnv> entity, Ptr<Node> node, Ptr<const Packet> packet)
{
  NS_LOG_DEBUG ("Client received a packet of " << packet->GetSize () << " bytes");
  entity->m_currentNode = node;
  entity->m_rxPktNum++;

  NS_LOG_UNCOND ("Node with ID " << entity->m_currentNode->GetId() << " received " << entity->m_rxPktNum << " packets");

  entity->Notify();
}

void
MyGymEnv::CountRxPkts(Ptr<MyGymEnv> entity, Ptr<Node> node, Ptr<const Packet> packet)
{
  NS_LOG_DEBUG ("Client received a packet of " << packet->GetSize () << " bytes");
  entity->m_currentNode = node;
  entity->m_rxPktNum++;
}


} // ns3 namespace
