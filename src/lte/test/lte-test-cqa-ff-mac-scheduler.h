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
 * Author:  Biljana Bojovic<bbojovic@cttc.es>
 *          Dizhi Zhou <dizhi.zhou@gmail.com>
 * 			Marco Miozzo <marco.miozzo@cttc.es>,
 *          Nicola Baldo <nbaldo@cttc.es>
 *
 */

#ifndef LENA_TEST_CQA_FF_MAC_SCHEDULER_H
#define LENA_TEST_CQA_FF_MAC_SCHEDULER_H

#include "ns3/simulator.h"
#include "ns3/test.h"


namespace ns3 {

/**
* This is a system test program. The test is based on a scenario with single eNB and several UEs.
* The goal of the test is validating if the obtained throughput performance is consistent with the definition of CQA scheduler.
*
*/

class LenaCqaFfMacSchedulerTestCase1 : public TestCase
{
public:
  LenaCqaFfMacSchedulerTestCase1 (uint16_t nUser, uint16_t dist, double thrRefDl, double thrRefUl, uint16_t packetSize, uint16_t interval, bool  errorModelEnabled);
  virtual ~LenaCqaFfMacSchedulerTestCase1 ();

private:
  static std::string BuildNameString (uint16_t nUser, uint16_t dist);
  virtual void DoRun (void);
  uint16_t m_nUser;
  uint16_t m_dist;
  uint16_t m_packetSize;  // byte
  uint16_t m_interval;    // ms
  double m_thrRefDl;
  double m_thrRefUl;
  bool m_errorModelEnabled;
};


class LenaCqaFfMacSchedulerTestCase2 : public TestCase
{
public:
  LenaCqaFfMacSchedulerTestCase2 (std::vector<uint16_t> dist, std::vector<uint32_t> estThrCqaDl, std::vector<uint16_t> packetSize, uint16_t interval, bool  errorModelEnabled);
  virtual ~LenaCqaFfMacSchedulerTestCase2 ();

private:
  static std::string BuildNameString (uint16_t nUser, std::vector<uint16_t> dist);
  virtual void DoRun (void);
  uint16_t m_nUser;
  std::vector<uint16_t> m_dist;
  std::vector<uint16_t> m_packetSize;  // byte
  uint16_t m_interval;    // ms
  std::vector<uint32_t> m_estThrCqaDl;
  bool m_errorModelEnabled;
};


class LenaTestCqaFfMacSchedulerSuite : public TestSuite
{
public:
  LenaTestCqaFfMacSchedulerSuite ();
};




} // namespace ns3


#endif /* LENA_TEST_CQA_FF_MAC_SCHEDULER_H */
