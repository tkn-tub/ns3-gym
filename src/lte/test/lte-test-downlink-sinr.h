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
 * Modified by Marco Miozzo <mmiozzo@ctt.es>
 *     Extend to Data and Ctrl frames
 */

#ifndef LTE_TEST_DOWNLINK_SINR_H
#define LTE_TEST_DOWNLINK_SINR_H

#include "ns3/spectrum-value.h"

#include "ns3/test.h"


namespace ns3 {


/**
 * Test 1.1 SINR calculation in downlink
 */
class LteDownlinkSinrTestSuite : public TestSuite
{
public:
  LteDownlinkSinrTestSuite ();
};


class LteDownlinkDataSinrTestCase : public TestCase
{
public:
  LteDownlinkDataSinrTestCase (Ptr<SpectrumValue> sv, Ptr<SpectrumValue> sinr, std::string name);
  virtual ~LteDownlinkDataSinrTestCase ();

private:
  virtual void DoRun (void);

  Ptr<SpectrumValue> m_sv;
  Ptr<const SpectrumModel> m_sm;
  Ptr<SpectrumValue> m_sinr;
};


class LteDownlinkCtrlSinrTestCase : public TestCase
{
  public:
    LteDownlinkCtrlSinrTestCase (Ptr<SpectrumValue> sv, Ptr<SpectrumValue> sinr, std::string name);
    virtual ~LteDownlinkCtrlSinrTestCase ();
    
  private:
    virtual void DoRun (void);
    
    Ptr<SpectrumValue> m_sv;
    Ptr<const SpectrumModel> m_sm;
    Ptr<SpectrumValue> m_sinr;
};

} // namespace ns3

#endif /* LTE_TEST_DOWNLINK_SINR_H */
