/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrea Sacco
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
 * Author: Andrea Sacco <andrea.sacco85@gmail.com>
 */

#ifndef UAN_CW_EXAMPLE_H
#define UAN_CW_EXAMPLE_H

#include "ns3/network-module.h"
#include "ns3/tools-module.h"
#include "ns3/uan-module.h"

using namespace ns3;

/**
 * \class NetAnimExperiment
 * \brief Helper class for UAN CW MAC example
 *
 */
class NetAnimExperiment
{
public:
  void Run (UanHelper &uan);
  void ReceivePacket (Ptr<Socket> socket);
  void UpdatePositions (NodeContainer &nodes);
  void ResetData ();
  void IncrementCw (uint32_t cw);
  uint32_t m_numNodes;
  uint32_t m_dataRate;
  double m_depth;
  double m_boundary;
  uint32_t m_packetSize;
  uint32_t m_bytesTotal;
  uint32_t m_cwMin;
  uint32_t m_cwMax;
  uint32_t m_cwStep;
  uint32_t m_avgs;

  Time m_slotTime;
  Time m_simTime;

  std::vector<double> m_throughputs;

  NetAnimExperiment ();
};

#endif /* UAN_CW_EXAMPLE_H */
