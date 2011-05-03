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

#ifndef UANRCEXAMPLE_H
#define UANRCEXAMPLE_H

#include "ns3/network-module.h"
#include "ns3/tools-module.h"
#include "ns3/uan-module.h"

using namespace ns3;

class Experiment
{
public:
  uint32_t m_simMin;
  uint32_t m_simMax;
  uint32_t m_simStep;
  uint32_t m_numRates;
  uint32_t m_totalRate;
  uint32_t m_maxRange;
  uint32_t m_numNodes;
  uint32_t m_pktSize;
  bool m_doNode;
  Time m_sifs;
  Time m_simTime;

  std::string m_gnuplotfile;

  uint32_t m_bytesTotal;

  UanModesList m_dataModes;
  UanModesList m_controlModes;

  void ReceivePacket (Ptr<Socket> socket);
  UanTxMode CreateMode (uint32_t kass,
                        uint32_t fc,
                        bool upperblock,
                        std::string name);
  void CreateDualModes (uint32_t fc);
  uint32_t Run (uint32_t param);

  Experiment();

};

#endif //UANRCEXAMPLE_H
