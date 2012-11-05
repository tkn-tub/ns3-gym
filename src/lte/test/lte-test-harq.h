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

#ifndef LENA_TEST_HARQ_H
#define LENA_TEST_HARQ_H

#include "ns3/simulator.h"
#include "ns3/test.h"


namespace ns3 {


/**
 * This system test program creates different test cases with a single eNB and
 * several UEs, all having the same Radio Bearer specification. In each test
 * case, the UEs see the same SINR from the eNB; different test cases are
 * implemented obtained by using different SINR values and different numbers of
 * UEs. The test consists on ...
 */
class LenaHarqTestCase : public TestCase
{
public:
  LenaHarqTestCase (uint16_t nUser, uint16_t dist, uint16_t tbSize, double amcBer, double thrRef);
  virtual ~LenaHarqTestCase ();

private:
  virtual void DoRun (void);
  static std::string BuildNameString (uint16_t nUser, uint16_t dist);
  uint16_t m_nUser;
  uint16_t m_dist;
  uint16_t m_tbSize;
  double m_amcBer;
  double m_throughputRef;

};





class LenaTestHarqSuite : public TestSuite
{
public:
  LenaTestHarqSuite ();
};




} // namespace ns3


#endif /* LENA_TEST_HARQ_H */
