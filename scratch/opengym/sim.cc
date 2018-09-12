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
  float low = -10.0;
  float high = 10.0;
  std::vector<int> shape = {7,};
  Dtype dType = Dtype::INT;
  Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace> (low, high, shape, dType);
  NS_LOG_UNCOND ("MyGetActionSpace: " << *space);
  return space;
}

Ptr<OpenGymSpace> MyGetObservationSpace(void)
{
  int n = 13;
  Ptr<OpenGymDiscreteSpace> space = CreateObject<OpenGymDiscreteSpace> (n);
  NS_LOG_UNCOND ("MyGetObservationSpace: " << *space);
  return space;
}

bool MyGetGameOver(void)
{
  bool isGameOver = false;
  //NS_LOG_UNCOND ("MyGetGameOver: " << isGameOver);
  return isGameOver;
}

Ptr<OpenGymObservation> MyGetState(void)
{
  Ptr<OpenGymObservation> obs = CreateObject<OpenGymObservation> ();
  std::string state = "1,12,15,1";
  obs->m_obsString = state;
  //NS_LOG_UNCOND ("MyGetState: " << state);
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
  NS_LOG_UNCOND ("MyExecuteActions: " << action->m_actionString);
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
  openGymEnv->SetGetStateCb( MakeCallback (&MyGetState) );
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
