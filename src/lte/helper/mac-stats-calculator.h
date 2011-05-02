/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 */

#ifndef MAC_STATS_CALCULATOR_H_
#define MAC_STATS_CALCULATOR_H_

#include "ns3/nstime.h"
#include "ns3/uinteger.h"
#include "ns3/object.h"
#include <string>
#include <fstream>

namespace ns3 {
  
class MacStatsCalculator : public Object
{
public:
  MacStatsCalculator();
  virtual
  ~MacStatsCalculator();
  static TypeId GetTypeId (void);

  void SetUlOutputFilename (std::string outputFilename);
  void SetDlOutputFilename (std::string outputFilename);

  void DlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                     uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2);
  void UlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                     uint8_t mcs, uint16_t sizeTb);


private:

  std::string m_dlOutputFilename;
  bool m_dlFirstWrite;

  std::string m_ulOutputFilename;
  bool m_ulFirstWrite;

};

} // namespace ns3

#endif /* MAC_STATS_CALCULATOR_H_ */
