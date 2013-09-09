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

#ifndef LENA_TEST_PHY_ERROR_MODEL_H
#define LENA_TEST_PHY_ERROR_MODEL_H

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
class LenaDataPhyErrorModelTestCase : public TestCase
{
public:
  LenaDataPhyErrorModelTestCase (uint16_t nUser, uint16_t dist, uint16_t tbSize, double refBler, uint16_t toleranceRxPackets, uint32_t rngRun);
  virtual ~LenaDataPhyErrorModelTestCase ();

private:
  virtual void DoRun (void);
  static std::string BuildNameString (uint16_t nUser, uint16_t dist, uint32_t rngRun);
  uint16_t m_nUser;
  uint16_t m_dist;
  uint16_t m_tbSize;
  double m_blerRef;
  uint16_t m_toleranceRxPackets;
  uint32_t m_rngRun;

};



class LenaDlCtrlPhyErrorModelTestCase : public TestCase
{
public:
  LenaDlCtrlPhyErrorModelTestCase (uint16_t nEnbr, uint16_t dist, uint16_t tbSize, double refBler, uint16_t toleranceRxPackets, uint32_t rngRun);
  virtual ~LenaDlCtrlPhyErrorModelTestCase ();
  
private:
  virtual void DoRun (void);
  static std::string BuildNameString (uint16_t nUser, uint16_t dist, uint32_t rngRun);
  uint16_t m_nEnb;
  uint16_t m_dist;
  uint16_t m_tbSize;
  double m_blerRef;
  uint16_t m_toleranceRxPackets;
  uint32_t m_rngRun;

};



class LenaTestPhyErrorModelrSuite : public TestSuite
{
public:
  LenaTestPhyErrorModelrSuite ();
};




} // namespace ns3


#endif /* LENA_TEST_PHY_ERROR_MODEL_H */
