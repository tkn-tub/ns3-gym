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

std::string MyGetActionSpace(void)
{
  std::string actionSpace = "0,1,0,1,0,1,0,1";
  //NS_LOG_UNCOND ("MyGetActionSpace: " << actionSpace);
  return actionSpace;
}

std::string MyGetState(void)
{
  std::string state = "1,12,15,1";
  //NS_LOG_UNCOND ("MyGetState: " << state);
  return state;
}

std::string MyGetReward(void)
{
  std::string reward = "1,1,1,1";
  //NS_LOG_UNCOND ("MyGetReward: " << reward);
  return reward;
}

bool MyExecuteActions(std::string actions)
{
  NS_LOG_UNCOND ("MyExecuteActions: " << actions);
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
