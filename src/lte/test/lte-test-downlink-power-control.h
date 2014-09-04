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
 *
 */

#ifndef LTE_TEST_DOWNLINK_POWER_CONTROL_H
#define LTE_TEST_DOWNLINK_POWER_CONTROL_H

#include "ns3/spectrum-value.h"
#include <ns3/lte-rrc-sap.h>

#include "ns3/spectrum-test.h"
#include "ns3/lte-spectrum-value-helper.h"

#include "ns3/test.h"

using namespace ns3;

/**
 * Test 1.1 SINR calculation in downlink
 */
class LteDownlinkPowerControlTestSuite : public TestSuite
{
public:
  LteDownlinkPowerControlTestSuite ();

  double CalculateRbTxPower (double txPower, uint8_t pa);
};

class LteDownlinkPowerControlSpectrumValueTestCase : public TestCase
{
public:
  LteDownlinkPowerControlSpectrumValueTestCase (std::string name,
                                                uint16_t earfcn, uint8_t bw, double powerTx,
                                                std::map<int, double> powerTxMap, std::vector <int> activeRbs,
                                                SpectrumValue& expected);
  virtual ~LteDownlinkPowerControlSpectrumValueTestCase ();

private:
  virtual void DoRun (void);
  Ptr<SpectrumValue> m_actual;
  Ptr<SpectrumValue> m_expected;

};

class LteDownlinkPowerControlTestCase : public TestCase
{
public:
  LteDownlinkPowerControlTestCase (bool changePower, uint8_t pa, std::string name);
  virtual ~LteDownlinkPowerControlTestCase ();

private:
  virtual void DoRun (void);

  bool m_changePdschConfigDedicated;
  LteRrcSap::PdschConfigDedicated m_pdschConfigDedicated;
  double m_expectedPowerDiff;
};


class LteDownlinkPowerControlRrcConnectionReconfigurationTestCase : public TestCase
{
public:
  LteDownlinkPowerControlRrcConnectionReconfigurationTestCase (bool useIdealRrc, std::string name);
  virtual ~LteDownlinkPowerControlRrcConnectionReconfigurationTestCase ();

  void ConnectionReconfigurationEnb (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);

  void ConnectionReconfigurationUe (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);

  void ChangePdschConfigDedicated (uint16_t rnti, uint8_t pa);
private:
  virtual void DoRun (void);
  bool m_useIdealRrc;

  bool m_changePdschConfigDedicatedTriggered;
  bool m_connectionReconfigurationUeReceived;
  bool m_connectionReconfigurationEnbCompleted;
};

#endif /* LTE_TEST_DOWNLINK_POWER_CONTROL_H */
