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

#ifndef LTE_TEST_DOWNLINK_FR_H
#define LTE_TEST_DOWNLINK_FR_H

#include "ns3/test.h"
#include "ns3/spectrum-value.h"
#include <ns3/lte-rrc-sap.h>

#include "ns3/spectrum-test.h"
#include "ns3/lte-spectrum-value-helper.h"

using namespace ns3;

/**
 * Test Downlink FFR algorithms
 */
class LteFrequencyReuseTestSuite : public TestSuite
{
public:
  LteFrequencyReuseTestSuite ();
};

class LteFrTestCase : public TestCase
{
public:
  LteFrTestCase (std::string name,
                 uint32_t userNum,uint8_t dlBandwidth,uint8_t ulBandwidth,
                 std::vector<bool> availableDlRb, std::vector<bool> availableUlRb);
  virtual ~LteFrTestCase ();

  void DlDataRxStart (Ptr<const SpectrumValue> spectrumValue);
  void UlDataRxStart (Ptr<const SpectrumValue> spectrumValue);

protected:
  virtual void DoRun (void);

  uint32_t m_userNum;
  uint8_t m_dlBandwidth;
  uint8_t m_ulBandwidth;

  std::vector<bool> m_availableDlRb;
  bool m_usedMutedDlRbg;

  std::vector<bool> m_availableUlRb;
  bool m_usedMutedUlRbg;
};


class LteHardFrTestCase : public LteFrTestCase
{
public:
  LteHardFrTestCase (std::string name, uint32_t userNum,
                     std::string schedulerType,
                     uint8_t dlBandwidth, uint8_t ulBandwidth,
                     uint8_t dlSubBandOffset, uint8_t dlSubBandwidth,
                     uint8_t ulSubBandOffset, uint8_t ulSubBandwidth,
                     std::vector<bool> availableDlRb, std::vector<bool> availableUlRb);
  virtual ~LteHardFrTestCase ();

private:
  virtual void DoRun (void);

  std::string m_schedulerType;

  uint8_t m_dlSubBandOffset;
  uint8_t m_dlSubBandwidth;

  uint8_t m_ulSubBandOffset;
  uint8_t m_ulSubBandwidth;
};

class LteStrictFrTestCase : public LteFrTestCase
{
public:
  LteStrictFrTestCase (std::string name, uint32_t userNum,
                       std::string schedulerType,
                       uint8_t dlBandwidth, uint8_t ulBandwidth,
                       uint8_t dlCommonSubBandwidth, uint8_t dlEdgeSubBandOffset, uint8_t dlEdgeSubBandwidth,
                       uint8_t ulCommonSubBandwidth, uint8_t ulEdgeSubBandOffset, uint8_t ulEdgeSubBandwidth,
                       std::vector<bool> availableDlRb, std::vector<bool> availableUlRb);
  virtual ~LteStrictFrTestCase ();

private:
  virtual void DoRun (void);

  std::string m_schedulerType;

  uint8_t m_dlCommonSubBandwidth;
  uint8_t m_dlEdgeSubBandOffset;
  uint8_t m_dlEdgeSubBandwidth;

  uint8_t m_ulCommonSubBandwidth;
  uint8_t m_ulEdgeSubBandOffset;
  uint8_t m_ulEdgeSubBandwidth;
};

class LteFrAreaTestCase : public TestCase
{
public:
  LteFrAreaTestCase (std::string name, std::string schedulerType);
  virtual ~LteFrAreaTestCase ();

  void DlDataRxStart (Ptr<const SpectrumValue> spectrumValue);
  void UlDataRxStart (Ptr<const SpectrumValue> spectrumValue);

  void SimpleTeleportUe (uint32_t x, uint32_t y);
  void TeleportUe (uint32_t x, uint32_t y, double expectedPower, std::vector<bool> expectedDlRb);
  void TeleportUe2 (Ptr<Node> ueNode, uint32_t x, uint32_t y, double expectedPower,
                    std::vector<bool> expectedDlRb);

  void SetDlExpectedValues (double expectedPower, std::vector<bool> expectedDlRb);
  void SetUlExpectedValues (double expectedPower, std::vector<bool> expectedDlRb);

protected:
  virtual void DoRun (void);

  std::string m_schedulerType;

  uint8_t m_dlBandwidth;
  uint8_t m_ulBandwidth;

  Time m_teleportTime;
  Ptr<MobilityModel> m_ueMobility;

  double m_expectedDlPower;
  std::vector<bool> m_expectedDlRb;
  bool m_usedWrongDlRbg;
  bool m_usedWrongDlPower;

  double m_expectedUlPower;
  std::vector<bool> m_expectedUlRb;
  bool m_usedWrongUlRbg;
  bool m_usedWrongUlPower;

};

class LteStrictFrAreaTestCase : public LteFrAreaTestCase
{
public:
  LteStrictFrAreaTestCase (std::string name, std::string schedulerType);
  virtual ~LteStrictFrAreaTestCase ();

private:
  virtual void DoRun (void);
};

class LteSoftFrAreaTestCase : public LteFrAreaTestCase
{
public:
  LteSoftFrAreaTestCase (std::string name, std::string schedulerType);
  virtual ~LteSoftFrAreaTestCase ();

private:
  virtual void DoRun (void);

};

class LteSoftFfrAreaTestCase : public LteFrAreaTestCase
{
public:
  LteSoftFfrAreaTestCase (std::string name, std::string schedulerType);
  virtual ~LteSoftFfrAreaTestCase ();

private:
  virtual void DoRun (void);

};

class LteEnhancedFfrAreaTestCase : public LteFrAreaTestCase
{
public:
  LteEnhancedFfrAreaTestCase (std::string name, std::string schedulerType);
  virtual ~LteEnhancedFfrAreaTestCase ();

private:
  virtual void DoRun (void);

};

class LteDistributedFfrAreaTestCase : public LteFrAreaTestCase
{
public:
  LteDistributedFfrAreaTestCase (std::string name, std::string schedulerType);
  virtual ~LteDistributedFfrAreaTestCase ();

private:
  virtual void DoRun (void);

};

#endif /* LTE_TEST_DOWNLINK_FR_H */
