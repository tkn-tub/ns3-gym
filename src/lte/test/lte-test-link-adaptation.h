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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef LTE_TEST_LINK_ADAPTATION_H
#define LTE_TEST_LINK_ADAPTATION_H

#include "ns3/test.h"


using namespace ns3;




/**
 * Test 1.3 Link adaptation
 */
class LteLinkAdaptationTestSuite : public TestSuite
{
public:
  LteLinkAdaptationTestSuite ();
};


class LteLinkAdaptationTestCase : public TestCase
{
  public:
//     LteLinkAdaptationTestCase (Ptr<SpectrumValue> sv, Ptr<SpectrumValue> sinr, std::string name);
    LteLinkAdaptationTestCase (double loss);
    LteLinkAdaptationTestCase ();
    virtual ~LteLinkAdaptationTestCase ();

  private:
    virtual void DoRun (void);

    Ptr<MacStatsCalculator> macStats;

    double m_loss;
//     Ptr<SpectrumValue> m_sv;
//     Ptr<const SpectrumModel> m_sm;
//     Ptr<SpectrumValue> m_sinr;

    static uint32_t m_runId;
};


#endif /* LTE_TEST_LINK_ADAPTATION_H */