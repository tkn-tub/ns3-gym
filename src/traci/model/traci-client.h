/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/*
 * Copyright (c) 2018 TU Dresden
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
 * Authors: Patrick Schmager <patrick.schmager@tu-dresden.de>
 *          Sebastian Kuehlmorgen <sebastian.kuehlmorgen@tu-dresden.de>
 */

#ifndef TRACI_H
#define TRACI_H

#include <map>
#include <vector>
#include <string>
#include <functional>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"

#include "sumo-TraCIAPI.h"
#include "sumo-TraCIDefs.h"

namespace ns3 {

class TraciClient : public TraCIAPI, public Object
{
public:
  // register this type with the TypeId system.
  static TypeId GetTypeId (void);

  // constructor and destructor
  TraciClient (void);
  ~TraciClient(void);

  // start up sumo; pass function pointers for including and excluding node functions
  void SumoSetup(std::function<Ptr<Node>()> includeNode, std::function<void(Ptr<Node>)> excludeNode);

  void SumoStop();

  // get associated sumo vehicle for ns3 node
  std::string GetVehicleId(Ptr<Node> node);

  uint32_t GetVehicleMapSize(); // size of vehicle map

private:
  // perform sumo simulation for a certain time step
  void SumoSimulationStep(void);

  // get current positions from sumo vehicles and update corresponding ns3 nodes positions
  void UpdatePositions(void);

  // get new (departed) and removed (arrived) vehicles from sumo
  void GetSumoVehicles(std::vector<std::string>& sumoVehicles);

  // synchronise ns3 nodes with sumo vehicles
  void SynchroniseVehicleNodeMap(void);

  // build command line string for sumo start up
  std::string GetSumoCmdString (void);

  // map every sumo vehicle to a ns3 node
  std::map< std::string, Ptr<Node> > m_vehicleNodeMap;

  // a vehicle is untracked if it is simulated in sumo but not linked to a ns3 node because of an penetration rate < 1.0
  std::vector<std::string> m_untrackedVehicles;

  // function pointers to node include/exclude functions 
  std::function<Ptr<Node>()> m_includeNode;
  std::function<void(Ptr<Node>)> m_excludeNode;

  // port handling functionality for multiple parallel simulations
  static bool PortFreeCheck (uint32_t portNum);
  static uint32_t GetFreePort (uint32_t portNum=10000);

  // simulation specific data members
  std::string m_sumoAddCmdOpt;
  std::string m_sumoCommand;
  std::string m_sumoConfigPath;
  std::string m_sumoBinaryPath;
  uint16_t m_sumoPort;
  bool m_sumoGUI;

  double m_penetrationRate;
  ns3::Time m_synchInterval;
  ns3::Time m_startTime;
  
  bool m_sumoLogFile;
  bool m_sumoStepLog;
  double m_altitude;
  int m_sumoSeed;
  ns3::Time m_sumoWaitForSocket;

};

} // end namespace ns3

#endif /* TRACI_H */

