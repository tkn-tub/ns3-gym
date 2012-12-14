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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 */

#ifndef LENA_TEST_MIMO_H
#define LENA_TEST_MIMO_H

#include "ns3/simulator.h"
#include "ns3/test.h"


namespace ns3 {


/**
 * This system test program creates different test cases with a single eNB...
 */
class LenaMimoTestCase : public TestCase
{
public:
  LenaMimoTestCase (uint16_t dist, std::vector<uint32_t> estThrDl, std::string schedulerType, bool useIdealRrc);
  virtual ~LenaMimoTestCase ();

private:
  virtual void DoRun (void);
  
  void GetRlcBufferSample (Ptr<RadioBearerStatsCalculator> rlcStats, uint64_t imsi, uint8_t rnti);
  
  static std::string BuildNameString (uint16_t dist, std::string schedulerType, bool useIdealRrc);
  uint16_t m_nUser;
  uint16_t m_nLc;
  uint16_t m_dist;
  std::vector<uint32_t> m_estThrDl;
  std::string m_schedulerType;
  bool m_useIdealRrc;
  
  std::vector <uint64_t> m_dlDataRxed;

};



class LenaTestMimoSuite : public TestSuite
{
public:
  LenaTestMimoSuite ();
};




} // namespace ns3


#endif /* LENA_TEST_MIMO_H */
