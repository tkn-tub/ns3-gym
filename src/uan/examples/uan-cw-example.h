/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#ifndef UANCWEXAMPLE_H
#define UANCWEXAMPLE_H

#include "ns3/network-module.h"
#include "ns3/tools-module.h"
#include "ns3/uan-module.h"

using namespace ns3;

/**
 * \class Experiment
 * \brief Helper class for UAN CW MAC example
 *
 */
class Experiment
{
public:
  Gnuplot2dDataset Run (UanHelper &uan);
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

  std::string m_gnudatfile;
  std::string m_asciitracefile;
  std::string m_bhCfgFile;

  Gnuplot2dDataset m_data;
  std::vector<double> m_throughputs;

  Experiment ();
};

#endif //UANCWEXAMPLE_H
