/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Piotr Gawlowicz
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
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 * Based on lte-test-interference.{h,cc} by Manuel Requena <manuel.requena@cttc.es>
 *                                          Nicola Baldo <nbaldo@cttc.es>
 *
 */

#ifndef LTE_TEST_INTERFERENCE_FR_H
#define LTE_TEST_INTERFERENCE_FR_H

#include "ns3/test.h"

using namespace ns3;

/**
 * Test Interference level with FR algorithms
 */
class LteInterferenceFrTestSuite : public TestSuite
{
public:
  LteInterferenceFrTestSuite ();
};


class LteInterferenceHardFrTestCase : public TestCase
{
public:
  LteInterferenceHardFrTestCase (std::string name, double d1, double d2, double dlSinr, double ulSinr);
  virtual ~LteInterferenceHardFrTestCase ();

private:
  virtual void DoRun (void);

  double m_d1;
  double m_d2;
  double m_expectedDlSinrDb;
};

class LteInterferenceStrictFrTestCase : public TestCase
{
public:
  LteInterferenceStrictFrTestCase (std::string name, double d1, double d2,
                                   double commonDlSinr, double commonUlSinr, double edgeDlSinr, double edgeUlSinr,
                                   uint32_t rspqThreshold);
  virtual ~LteInterferenceStrictFrTestCase ();

private:
  virtual void DoRun (void);

  double m_d1;
  double m_d2;
  double m_commonDlSinrDb;
  double m_edgeDlSinrDb;

  uint32_t m_rspqThreshold;
};

#endif /* LTE_TEST_INTERFERENCE_FR_H */
