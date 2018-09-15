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

#include "ns3/core-module.h"
#include "ns3/opengym-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("OpenGym");

Ptr<OpenGymSpace> MyGetObservationSpace(void)
{
  uint32_t nodeNum = 5;

  float low = 0.0;
  float high = 10.0;
  std::vector<uint32_t> shape = {nodeNum,};
  std::string dtype = TypeNameGet<uint32_t> ();
  Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace> (low, high, shape, dtype);
  NS_LOG_UNCOND ("MyGetObservationSpace: " << *space);
  return space;
}

Ptr<OpenGymSpace> MyGetActionSpace(void)
{
  // OpenGymDiscreteSpace also works
  //int n = 13;
  //Ptr<OpenGymDiscreteSpace> space = CreateObject<OpenGymDiscreteSpace> (n);

  uint32_t nodeNum = 5;

  float low = 0.0;
  float high = 100.0;
  std::vector<uint32_t> shape = {nodeNum,};
  std::string dtype = TypeNameGet<uint32_t> ();
  Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace> (low, high, shape, dtype);
  NS_LOG_UNCOND ("MyGetActionSpace: " << *space);
  return space;
}

bool MyGetGameOver(void)
{
  bool isGameOver = false;
  bool test = false;
  static float stepCounter = 0.0;
  stepCounter += 1;
  if (stepCounter == 10 && test) {
      isGameOver = true;
  }
  NS_LOG_UNCOND ("MyGetGameOver: " << isGameOver);
  return isGameOver;
}

Ptr<OpenGymDataContainer> MyGetObservation(void)
{
  uint32_t nodeNum = 5;
  std::vector<uint32_t> shape = {nodeNum,};
  Ptr<OpenGymBoxContainer<uint32_t> > box = CreateObject<OpenGymBoxContainer<uint32_t> >(shape);

  // generate random data
  std::string obsString = "[";
  uint32_t low = 0.0;
  uint32_t high = 10.0;
  for (uint32_t i = 0; i<nodeNum; i++){
    uint32_t value = rand() % high + low;
    box->AddValue(value);
    obsString += std::to_string(value) +",";
  }
  obsString += "]";

  NS_LOG_UNCOND ("MyGetObservation: " << obsString);
  return box;
}

float MyGetReward(void)
{
  static float reward = 0.0;
  reward += 1;
  //NS_LOG_UNCOND ("MyGetReward: " << reward);
  //if (reward == 10) {
  //    Simulator::Stop();
  //}
  return reward;
}

bool MyExecuteActions(Ptr<OpenGymDataContainer> action)
{
  Ptr<OpenGymBoxContainer<uint32_t> > box = DynamicCast<OpenGymBoxContainer<uint32_t> >(action);
  std::vector<uint32_t> actionVector = box->GetData();

  std::string actionString  = "[";
  for (auto i = actionVector.begin(); i != actionVector.end(); ++i)
  {
    actionString += std::to_string(*i) +",";
  }
  actionString += "]";
  NS_LOG_UNCOND ("MyExecuteActions: " << actionString);

  return true;
}

int
main (int argc, char *argv[])
{
  // Parameters of the scenario
  uint32_t simSeed = 1;
  double simulationTime = 10; //seconds
  double envStepTime = 0.1; //seconds, ns3gym env step time interval
  uint32_t openGymPort = 5555;
  uint32_t testArg = 0;

  CommandLine cmd;
  cmd.AddValue ("simTime", "Simulation time in seconds. Default: 10s", simulationTime);
  cmd.AddValue ("openGymPort", "Port number for OpenGym env. Default: 5555", openGymPort);
  cmd.AddValue ("simSeed", "Seed for random generator. Default: 1", simSeed);
  cmd.AddValue ("testArg", "Extra simulation argument. Default: 0", testArg);
  cmd.Parse (argc, argv);

  NS_LOG_UNCOND("Ns3Env parameters:");
  NS_LOG_UNCOND("--simulationTime: " << simulationTime);
  NS_LOG_UNCOND("--openGymPort: " << openGymPort);
  NS_LOG_UNCOND("--envStepTime: " << envStepTime);
  NS_LOG_UNCOND("--seed: " << simSeed);
  NS_LOG_UNCOND("--testArg: " << testArg);

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (simSeed);

  // OpenGym Env
  Ptr<OpenGymEnv> openGymEnv = CreateObject<OpenGymEnv> (openGymPort, Seconds(envStepTime));
  openGymEnv->SetGetActionSpaceCb( MakeCallback (&MyGetActionSpace) );
  openGymEnv->SetGetObservationSpaceCb( MakeCallback (&MyGetObservationSpace) );
  openGymEnv->SetGetGameOverCb( MakeCallback (&MyGetGameOver) );
  openGymEnv->SetGetObservationCb( MakeCallback (&MyGetObservation) );
  openGymEnv->SetGetRewardCb( MakeCallback (&MyGetReward) );
  openGymEnv->SetExecuteActionsCb( MakeCallback (&MyExecuteActions) );

  openGymEnv->Init();
  NS_LOG_UNCOND ("Simulation start");
  Simulator::Stop (Seconds (simulationTime));
  Simulator::Run ();
  NS_LOG_UNCOND ("Simulation stop");

  openGymEnv->SetGameOver();
  openGymEnv->WaitForStop();
  Simulator::Destroy ();

}
