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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>,
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Dizhi Zhou <dizhi.zhou@gmail.com>
 */

#ifndef LENA_TEST_PSS_FF_MAC_SCHEDULER_H
#define LENA_TEST_PSS_FF_MAC_SCHEDULER_H

#include "ns3/simulator.h"
#include "ns3/test.h"


namespace ns3 {


/**
* This system test program creates different test cases with a single eNB and 
* several UEs, all having the same Radio Bearer specification. In each test 
* case, the UEs see the same SINR from the eNB; different test cases are 
* implemented obtained by using different SINR values and different numbers of 
* UEs. The test consists on checking that the obtained throughput performance 
* is equal among users is consistent with the definition of token bank fair  
* queue scheduling
*/
class LenaPssFfMacSchedulerTestCase1 : public TestCase
{
public:
  LenaPssFfMacSchedulerTestCase1 (uint16_t nUser, uint16_t nLc, uint16_t dist, double thrRefDl, double thrRefUl, uint16_t packetSize, uint16_t interval, bool  errorModelEnabled);
  virtual ~LenaPssFfMacSchedulerTestCase1 ();

private:
  static std::string BuildNameString (uint16_t nUser, uint16_t dist);
  virtual void DoRun (void);
  uint16_t m_nUser;
  uint16_t m_nLc;
  uint16_t m_dist;
  uint16_t m_packetSize;  // byte
  uint16_t m_interval;    // ms
  double m_thrRefDl;
  double m_thrRefUl;
  bool m_errorModelEnabled;
};


class LenaPssFfMacSchedulerTestCase2 : public TestCase
{
public:
  LenaPssFfMacSchedulerTestCase2 (std::vector<uint16_t> dist, std::vector<uint32_t> estThrPssDl, std::vector<uint16_t> packetSize, uint16_t interval, bool  errorModelEnabled);
  virtual ~LenaPssFfMacSchedulerTestCase2 ();

private:
  static std::string BuildNameString (uint16_t nUser, std::vector<uint16_t> dist);
  virtual void DoRun (void);
  uint16_t m_nUser;
  std::vector<uint16_t> m_dist;
  std::vector<uint16_t> m_packetSize;  // byte
  uint16_t m_interval;    // ms
  std::vector<uint32_t> m_estThrPssDl;
  bool m_errorModelEnabled;
};


class LenaTestPssFfMacSchedulerSuite : public TestSuite
{
public:
  LenaTestPssFfMacSchedulerSuite ();
};




} // namespace ns3


#endif /* LENA_TEST_PSS_FF_MAC_SCHEDULER_H */
