/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 University of Washington
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
 * Author: Tom Henderson (tomhend@u.washington.edu)
 */

#include <cmath>
#include "ns3/test.h"
#include "ns3/nist-error-rate-model.h"

using namespace ns3;

double
FromRss (double rssDbw)
{
  // SINR is based on receiver noise figure of 7 dB and thermal noise
  // of -100.5522786 dBm in this 22 MHz bandwidth at 290K
  double noisePowerDbw = -100.5522786 + 7;

  double sinrDb = rssDbw - noisePowerDbw;
  // return SINR expressed as ratio
  return pow (10.0, sinrDb / 10.0);
}

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Wifi Error Rate Models Test Case Dsss
 */
class WifiErrorRateModelsTestCaseDsss : public TestCase
{
public:
  WifiErrorRateModelsTestCaseDsss ();
  virtual ~WifiErrorRateModelsTestCaseDsss ();

private:
  virtual void DoRun (void);
};

WifiErrorRateModelsTestCaseDsss::WifiErrorRateModelsTestCaseDsss ()
  : TestCase ("WifiErrorRateModel test case DSSS")
{
}

WifiErrorRateModelsTestCaseDsss::~WifiErrorRateModelsTestCaseDsss ()
{
}

void
WifiErrorRateModelsTestCaseDsss::DoRun (void)
{

  // 1024 bytes plus headers
  uint32_t size = (1024 + 40 + 14) * 8;
  // Spot test some values returned from DsssErrorRateModel
  // Values taken from sample 80211b.c program used in validation paper
  double value;
  // DBPSK
  value = DsssErrorRateModel::GetDsssDbpskSuccessRate (FromRss (-105.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0, 1e-13, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDbpskSuccessRate (FromRss (-100.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 1.5e-13, 1e-13, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDbpskSuccessRate (FromRss (-99.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.0003, 0.0001, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDbpskSuccessRate (FromRss (-98.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.202, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDbpskSuccessRate (FromRss (-97.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.813, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDbpskSuccessRate (FromRss (-96.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.984, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDbpskSuccessRate (FromRss (-95.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.999, 0.001, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDbpskSuccessRate (FromRss (-90.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 1, 0.001, "Not equal within tolerance");

  // DQPSK
  //
  value = DsssErrorRateModel::GetDsssDqpskSuccessRate (FromRss (-96.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0, 1e-13, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskSuccessRate (FromRss (-95.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 4.5e-6, 1e-6, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskSuccessRate (FromRss (-94.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.036, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskSuccessRate (FromRss (-93.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.519, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskSuccessRate (FromRss (-92.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.915, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskSuccessRate (FromRss (-91.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.993, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskSuccessRate (FromRss (-90.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.999, 0.001, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskSuccessRate (FromRss (-89.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 1, 0.001, "Not equal within tolerance");

#ifdef HAVE_GSL
  // DQPSK_CCK5.5
  value = DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (FromRss (-94.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0, 1e-13, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (FromRss (-93.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 6.6e-14, 5e-14, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (FromRss (-92.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.0001, 0.00005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (FromRss (-91.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.132, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (FromRss (-90.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.744, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (FromRss (-89.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.974, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (FromRss (-88.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.999, 0.001, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (FromRss (-87.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 1, 0.001, "Not equal within tolerance");

  // DQPSK_CCK11
  value = DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (FromRss (-91.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0, 1e-14, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (FromRss (-90.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 4.7e-14, 1e-14, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (FromRss (-89.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 8.85e-5, 1e-5, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (FromRss (-88.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.128, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (FromRss (-87.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.739, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (FromRss (-86.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.973, 0.005, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (FromRss (-85.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 0.999, 0.001, "Not equal within tolerance");
  value = DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (FromRss (-84.0), size);
  NS_TEST_ASSERT_MSG_EQ_TOL (value, 1, 0.001, "Not equal within tolerance");
#endif
}

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Wifi Error Rate Models Test Case Nist
 */
class WifiErrorRateModelsTestCaseNist : public TestCase
{
public:
  WifiErrorRateModelsTestCaseNist ();
  virtual ~WifiErrorRateModelsTestCaseNist ();

private:
  virtual void DoRun (void);
};

WifiErrorRateModelsTestCaseNist::WifiErrorRateModelsTestCaseNist ()
  : TestCase ("WifiErrorRateModel test case NIST")
{
}

WifiErrorRateModelsTestCaseNist::~WifiErrorRateModelsTestCaseNist ()
{
}

void
WifiErrorRateModelsTestCaseNist::DoRun (void)
{

  uint32_t FrameSize = 2000;
  WifiTxVector txVector;
  Ptr<NistErrorRateModel> nist = CreateObject<NistErrorRateModel> ();

  double ps; // probability of success
  double snr; // dB

  // Spot test some values returned from NistErrorRateModel
  // values can be generated by the example program ofdm-validation.cc
  snr = 2.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate6Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 2.04e-10, 1e-10, "Not equal within tolerance");
  snr = 3.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate6Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.020, 0.001, "Not equal within tolerance");
  snr = 4.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate6Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.885, 0.001, "Not equal within tolerance");
  snr = 5.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate6Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.997, 0.001, "Not equal within tolerance");

  snr = 6.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate9Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.097, 0.001, "Not equal within tolerance");
  snr = 7.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate9Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.918, 0.001, "Not equal within tolerance");
  snr = 8.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate9Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.998, 0.001, "Not equal within tolerance");
  snr = 9.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate9Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.999, 0.001, "Not equal within tolerance");

  snr = 6.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate12Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.0174, 0.001, "Not equal within tolerance");
  snr = 7.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate12Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.881, 0.001, "Not equal within tolerance");
  snr = 8.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate12Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.997, 0.001, "Not equal within tolerance");
  snr = 9.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate12Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.999, 0.001, "Not equal within tolerance");

  snr = 8.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate18Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 2.85e-6, 1e-6, "Not equal within tolerance");
  snr = 9.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate18Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.623, 0.001, "Not equal within tolerance");
  snr = 10.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate18Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.985, 0.001, "Not equal within tolerance");
  snr = 11.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate18Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.999, 0.001, "Not equal within tolerance");

  snr = 12.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate24Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 2.22e-7, 1e-7, "Not equal within tolerance");
  snr = 13.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate24Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.495, 0.001, "Not equal within tolerance");
  snr = 14.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate24Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.974, 0.001, "Not equal within tolerance");
  snr = 15.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate24Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.999, 0.001, "Not equal within tolerance");

  snr = 15.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate36Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.012, 0.001, "Not equal within tolerance");
  snr = 16.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate36Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.818, 0.001, "Not equal within tolerance");
  snr = 17.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate36Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.993, 0.001, "Not equal within tolerance");
  snr = 18.5;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate36Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.999, 0.001, "Not equal within tolerance");

  snr = 20.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate48Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 1.3e-4, 1e-4, "Not equal within tolerance");
  snr = 21.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate48Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.649, 0.001, "Not equal within tolerance");
  snr = 22.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate48Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.983, 0.001, "Not equal within tolerance");
  snr = 23.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate48Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.999, 0.001, "Not equal within tolerance");

  snr = 21.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate54Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 5.44e-8, 1e-8, "Not equal within tolerance");
  snr = 22.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate54Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.410, 0.001, "Not equal within tolerance");
  snr = 23.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate54Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.958, 0.001, "Not equal within tolerance");
  snr = 24.0;
  ps = nist->GetChunkSuccessRate (WifiMode ("OfdmRate54Mbps"), txVector, std::pow (10.0, snr / 10.0), FrameSize * 8);
  NS_TEST_ASSERT_MSG_EQ_TOL (ps, 0.999, 0.001, "Not equal within tolerance");
}

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Wifi Error Rate Models Test Suite
 */
class WifiErrorRateModelsTestSuite : public TestSuite
{
public:
  WifiErrorRateModelsTestSuite ();
};

WifiErrorRateModelsTestSuite::WifiErrorRateModelsTestSuite ()
  : TestSuite ("wifi-error-rate-models", UNIT)
{
  AddTestCase (new WifiErrorRateModelsTestCaseDsss, TestCase::QUICK);
  AddTestCase (new WifiErrorRateModelsTestCaseNist, TestCase::QUICK);
}

static WifiErrorRateModelsTestSuite wifiErrorRateModelsTestSuite; ///< the test suite

