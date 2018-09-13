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

Ptr<OpenGymSpace> MyGetActionSpace(void)
{
  // OpenGymDiscreteSpace also works
  //int n = 13;
  //Ptr<OpenGymDiscreteSpace> space = CreateObject<OpenGymDiscreteSpace> (n);

  uint32_t nodeNum = 5;

  float low = 0.0;
  float high = 100.0;
  std::vector<uint32_t> shape = {nodeNum,};
  Dtype dType = Dtype::INT;
  Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace> (low, high, shape, dType);
  NS_LOG_UNCOND ("MyGetActionSpace: " << *space);
  return space;
}

Ptr<OpenGymSpace> MyGetObservationSpace(void)
{
  uint32_t nodeNum = 5;

  float low = 0.0;
  float high = 10.0;
  std::vector<uint32_t> shape = {nodeNum,};
  Dtype dType = Dtype::INT;
  Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace> (low, high, shape, dType);
  NS_LOG_UNCOND ("MyGetObservationSpace: " << *space);
  return space;
}

bool MyGetGameOver(void)
{
  bool isGameOver = false;
  //NS_LOG_UNCOND ("MyGetGameOver: " << isGameOver);
  return isGameOver;
}

Ptr<OpenGymObservation> MyGetObservation(void)
{
  uint32_t nodeNum = 5;

  Ptr<OpenGymObservation> obs = CreateObject<OpenGymObservation> ();
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
  obs->AddObsContainer(box);
  NS_LOG_UNCOND ("MyGetObservation: " << obsString);
  return obs;
}

float MyGetReward(void)
{
  static float reward = 0.0;
  reward += 1;
  //NS_LOG_UNCOND ("MyGetReward: " << reward);
  return reward;
}

bool MyExecuteActions(Ptr<OpenGymAction> action)
{
  Ptr<OpenGymDataContainer> container = action->GetActionContainer(0);
  Ptr<OpenGymBoxContainer<uint32_t> > box = DynamicCast<OpenGymBoxContainer<uint32_t> >(container);
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
  uint32_t trial = 222;
  double simulationTime = 10.1; //seconds

  CommandLine cmd;
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.Parse (argc, argv);

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (trial);

  // OpenGym Env
  Ptr<OpenGymEnv> openGymEnv = CreateObject<OpenGymEnv> ();
  openGymEnv->SetGetActionSpaceCb( MakeCallback (&MyGetActionSpace) );
  openGymEnv->SetGetObservationSpaceCb( MakeCallback (&MyGetObservationSpace) );
  openGymEnv->SetGetGameOverCb( MakeCallback (&MyGetGameOver) );
  openGymEnv->SetGetStateCb( MakeCallback (&MyGetObservation) );
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
