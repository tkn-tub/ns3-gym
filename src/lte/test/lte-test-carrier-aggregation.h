/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Biljana Bojovic <biljana.bojovic@cttc.es>
 *
 */

#ifndef TEST_CARRIER_AGGREGATION_H
#define TEST_CARRIER_AGGREGATION_H

#include "ns3/simulator.h"
#include "ns3/test.h"
#include "fcntl.h"


using namespace ns3;


/**
 * This system test program creates different test cases with a single eNB and 
 * several UEs, all having the same Radio Bearer specification. In each test 
 * case, the UEs see the same SINR from the eNB; different test cases are 
 * implemented obtained by using different SINR values and different numbers of 
 * UEs. eNb and UEs are configured to use the secondary carrier and the component
 * carrier manager is configured to split the data equally between primary and
 * secondary carrier. The test consists of checking that the throughput
 * obtained over different carriers are equal within a given tolerance.
 */
class CarrierAggregationTestCase : public TestCase
{
public:

  static bool s_writeResults;

  CarrierAggregationTestCase (uint16_t nUser, uint16_t dist, uint32_t dlbandwidth, uint32_t ulBandwidth, uint numberOfComponentCarriers);
  virtual ~CarrierAggregationTestCase ();
  void DlScheduling (DlSchedulingCallbackInfo dlInfo);
  void UlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti, uint8_t mcs, uint16_t sizeTb, uint8_t componentCarrierId);
  void WriteResultToFile ();


private:

  virtual void DoRun (void);
  static std::string BuildNameString  (uint16_t nUser, uint16_t dist, uint32_t dlBandwidth, uint32_t ulBandwidth, uint32_t numberOfComponentCarriers);

  uint16_t m_nUser;
  uint16_t m_dist;
  uint32_t m_dlBandwidth;
  uint32_t m_ulBandwidth;
  uint32_t m_numberOfComponentCarriers;

  std::map <uint8_t, uint32_t> m_ccDownlinkTraffic;
  std::map <uint8_t, uint32_t> m_ccUplinkTraffic;
  uint64_t m_dlThroughput;
  uint64_t m_ulThroughput;
  double m_statsDuration;
};



class TestCarrierAggregationSuite : public TestSuite
{
public:
  TestCarrierAggregationSuite ();
};

#endif /* TEST_CARRIER_AGGREGATION_H */
