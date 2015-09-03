/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 * Authors: Nicola Baldo <nbaldo@cttc.es>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include <ns3/object.h>
#include <ns3/log.h>
#include <ns3/test.h>
#include <iostream>
#include "ns3/interference-helper.h"
#include "ns3/yans-wifi-phy.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("InterferenceHelperTxDurationTest");

static const double CHANNEL_1_MHZ  = 2412.0; // a 2.4 GHz center frequency (MHz)
static const double CHANNEL_36_MHZ = 5180.0; // a 5 GHz center frequency (MHz)

class TxDurationTest : public TestCase
{
public:
  TxDurationTest ();
  virtual ~TxDurationTest ();
  virtual void DoRun (void);


private:
  /**
   * Check if the payload tx duration returned by InterferenceHelper
   * corresponds to a known value of the pay
   *
   * @param size size of payload in octets (includes everything after the PLCP header)
   * @param payloadMode the WifiMode used
   * @param channelWidth the channel width used (in MHz)
   * @param isShortGuardInterval whether short guard interval is used
   * @param knownDurationMicroSeconds the known duration value of the transmission in microseconds
   *
   * @return true if values correspond, false otherwise
   */
  bool CheckPayloadDuration (uint32_t size, WifiMode payloadMode, uint32_t channelWidth, bool isShortGuardInterval, WifiPreamble preamble, uint32_t knownDurationMicroSeconds);

  /**
   * Check if the overall tx duration returned by InterferenceHelper
   * corresponds to a known value of the pay
   *
   * @param size size of payload in octets (includes everything after the PLCP header)
   * @param payloadMode the WifiMode used
   * @param channelWidth the channel width used (in MHz)
   * @param isShortGuardInterval whether short guard interval is used
   * @param preamble the WifiPreamble used
   * @param knownDurationMicroSeconds the known duration value of the transmission in microseconds
   *
   * @return true if values correspond, false otherwise
   */
  bool CheckTxDuration (uint32_t size, WifiMode payloadMode, uint32_t channelWidth, bool isShortGuardInterval, WifiPreamble preamble, double knownDurationMicroSeconds);

};

TxDurationTest::TxDurationTest ()
  : TestCase ("Wifi TX Duration")
{
}

TxDurationTest::~TxDurationTest ()
{
}

bool
TxDurationTest::CheckPayloadDuration (uint32_t size, WifiMode payloadMode, uint32_t channelWidth, bool isShortGuardInterval, WifiPreamble preamble, uint32_t knownDurationMicroSeconds)
{
  WifiTxVector txVector;
  txVector.SetMode (payloadMode);
  txVector.SetChannelWidth (channelWidth);
  txVector.SetShortGuardInterval (isShortGuardInterval);
  txVector.SetNss (1);
  txVector.SetStbc (0);
  txVector.SetNess (0);
  double testedFrequency = CHANNEL_1_MHZ;
  Ptr<YansWifiPhy> phy = CreateObject<YansWifiPhy> ();
  if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_OFDM
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HT
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_VHT)
    {
      testedFrequency = CHANNEL_36_MHZ;
    }
  double calculatedDurationMicroSeconds = (double)phy->GetPayloadDuration (size, txVector, preamble, testedFrequency, 0, 0).GetMicroSeconds  ();
  if (calculatedDurationMicroSeconds != knownDurationMicroSeconds)
    {
      std::cerr << " size=" << size
                << " mode=" << payloadMode
                << " datarate=" << payloadMode.GetDataRate (channelWidth, isShortGuardInterval, 1)
                << " known=" << knownDurationMicroSeconds
                << " calculated=" << calculatedDurationMicroSeconds
                << std::endl;
      return false;
    }
  if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HT)
    {
      //Durations vary depending on frequency; test also 2.4 GHz (bug 1971)
      testedFrequency = CHANNEL_1_MHZ;
      calculatedDurationMicroSeconds = (double)phy->GetPayloadDuration (size, txVector, preamble, testedFrequency, 0, 0).GetMicroSeconds ();
      if (calculatedDurationMicroSeconds != knownDurationMicroSeconds + 6)
        {
          std::cerr << " size=" << size
                    << " mode=" << payloadMode
                    << " datarate=" << payloadMode.GetDataRate (channelWidth, isShortGuardInterval, 1)
                    << " known=" << knownDurationMicroSeconds
                    << " calculated=" << calculatedDurationMicroSeconds
                    << std::endl;
          return false;
        }
    }
  return true;
}

bool
TxDurationTest::CheckTxDuration (uint32_t size, WifiMode payloadMode, uint32_t channelWidth, bool isShortGuardInterval, WifiPreamble preamble, double knownDurationMicroSeconds)
{
  WifiTxVector txVector;
  txVector.SetMode (payloadMode);
  txVector.SetChannelWidth (channelWidth);
  txVector.SetShortGuardInterval (isShortGuardInterval);
  txVector.SetNss (1);
  txVector.SetStbc (0);
  txVector.SetNess (0);
  double testedFrequency = CHANNEL_1_MHZ;
  Ptr<YansWifiPhy> phy = CreateObject<YansWifiPhy> ();
  if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_OFDM
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HT
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_VHT)
    {
      testedFrequency = CHANNEL_36_MHZ;
    }
  double calculatedDurationMicroSeconds = ((double)phy->CalculateTxDuration (size, txVector, preamble, testedFrequency, 0, 0).GetNanoSeconds ()) / 1000;
  if (calculatedDurationMicroSeconds != knownDurationMicroSeconds)
    {
      std::cerr << " size=" << size
                << " mode=" << payloadMode
                << " datarate=" << payloadMode.GetDataRate (channelWidth, isShortGuardInterval, 1)
                << " preamble=" << preamble
                << " known=" << knownDurationMicroSeconds
                << " calculated=" << calculatedDurationMicroSeconds
                << std::endl;
      return false;
    }
  if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HT)
    {
      //Durations vary depending on frequency; test also 2.4 GHz (bug 1971)
      testedFrequency = CHANNEL_1_MHZ;
      calculatedDurationMicroSeconds = ((double)phy->CalculateTxDuration (size, txVector, preamble, testedFrequency, 0, 0).GetNanoSeconds ()) / 1000;
      if (calculatedDurationMicroSeconds != knownDurationMicroSeconds + 6)
        {
          std::cerr << " size=" << size
                    << " mode=" << payloadMode
                    << " datarate=" << payloadMode.GetDataRate (channelWidth, isShortGuardInterval, 1)
                    << " preamble=" << preamble
                    << " known=" << knownDurationMicroSeconds
                    << " calculated=" << calculatedDurationMicroSeconds
                    << std::endl;
          return false;
        }
    }
  return true;
}

void
TxDurationTest::DoRun (void)
{
  bool retval = true;

  //IEEE Std 802.11-2007 Table 18-2 "Example of LENGTH calculations for CCK"
  retval = retval
    && CheckPayloadDuration (1023, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 744)
    && CheckPayloadDuration (1024, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 745)
    && CheckPayloadDuration (1025, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 746)
    && CheckPayloadDuration (1026, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 747);

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11b CCK duration failed");

  //Similar, but we add PLCP preamble and header durations
  //and we test different rates.
  //The payload durations for modes other than 11mbb have been
  //calculated by hand according to  IEEE Std 802.11-2007 18.2.3.5
  retval = retval
    && CheckTxDuration (1023, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 744 + 96)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 745 + 96)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 746 + 96)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 747 + 96)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 744 + 192)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 745 + 192)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 746 + 192)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 747 + 192)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate5_5Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 1488 + 96)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate5_5Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 1490 + 96)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate5_5Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 1491 + 96)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate5_5Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 1493 + 96)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate5_5Mbps (), 22, false, WIFI_PREAMBLE_LONG, 1488 + 192)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate5_5Mbps (), 22, false, WIFI_PREAMBLE_LONG, 1490 + 192)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate5_5Mbps (), 22, false, WIFI_PREAMBLE_LONG, 1491 + 192)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate5_5Mbps (), 22, false, WIFI_PREAMBLE_LONG, 1493 + 192)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate2Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 4092 + 96)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate2Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 4096 + 96)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate2Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 4100 + 96)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate2Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 4104 + 96)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate2Mbps (), 22, false, WIFI_PREAMBLE_LONG, 4092 + 192)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate2Mbps (), 22, false, WIFI_PREAMBLE_LONG, 4096 + 192)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate2Mbps (), 22, false, WIFI_PREAMBLE_LONG, 4100 + 192)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate2Mbps (), 22, false, WIFI_PREAMBLE_LONG, 4104 + 192)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 8184 + 96)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 8192 + 96)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 8200 + 96)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_SHORT, 8208 + 96)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_LONG, 8184 + 192)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_LONG, 8192 + 192)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_LONG, 8200 + 192)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_LONG, 8208 + 192);


  //values from http://mailman.isi.edu/pipermail/ns-developers/2009-July/006226.html
  retval = retval && CheckTxDuration (14, WifiPhy::GetDsssRate1Mbps (), 22, false, WIFI_PREAMBLE_LONG, 304);

  //values from http://www.oreillynet.com/pub/a/wireless/2003/08/08/wireless_throughput.html
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 1310)
    && CheckTxDuration (76, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 248)
    && CheckTxDuration (14, WifiPhy::GetDsssRate11Mbps (), 22, false, WIFI_PREAMBLE_LONG, 203);

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11b duration failed");

  //802.11a durations
  //values from http://www.oreillynet.com/pub/a/wireless/2003/08/08/wireless_throughput.html
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetOfdmRate54Mbps (), 20, false, WIFI_PREAMBLE_LONG, 248)
    && CheckTxDuration (76, WifiPhy::GetOfdmRate54Mbps (), 20, false, WIFI_PREAMBLE_LONG, 32)
    && CheckTxDuration (14, WifiPhy::GetOfdmRate54Mbps (), 20, false, WIFI_PREAMBLE_LONG, 24);

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11a duration failed");

  //802.11g durations are same as 802.11a durations but with 6 us signal extension
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetErpOfdmRate54Mbps (), 20, false, WIFI_PREAMBLE_LONG, 254)
    && CheckTxDuration (76, WifiPhy::GetErpOfdmRate54Mbps (), 20, false, WIFI_PREAMBLE_LONG, 38)
    && CheckTxDuration (14, WifiPhy::GetErpOfdmRate54Mbps (), 20, false, WIFI_PREAMBLE_LONG, 30);

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11g duration failed");

  //802.11n durations
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 20, false, WIFI_PREAMBLE_HT_MF, 228)
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 20, false, WIFI_PREAMBLE_HT_MF, 48)
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 20, false, WIFI_PREAMBLE_HT_MF, 40)
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 20, false, WIFI_PREAMBLE_HT_GF, 220)
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 20, false, WIFI_PREAMBLE_HT_GF, 40)
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 20, false, WIFI_PREAMBLE_HT_GF, 32)
    && CheckTxDuration (1536, WifiPhy::GetHtMcs0 (), 20, true, WIFI_PREAMBLE_HT_MF, 1742.4)
    && CheckTxDuration (76, WifiPhy::GetHtMcs0 (), 20, true, WIFI_PREAMBLE_HT_MF, 126)
    && CheckTxDuration (14, WifiPhy::GetHtMcs0 (), 20, true, WIFI_PREAMBLE_HT_MF, 57.6)
    && CheckTxDuration (1536,WifiPhy::GetHtMcs0 (), 20, true, WIFI_PREAMBLE_HT_GF, 1734.4)
    && CheckTxDuration (76, WifiPhy::GetHtMcs0 (), 20, true, WIFI_PREAMBLE_HT_GF, 118)
    && CheckTxDuration (14, WifiPhy::GetHtMcs0 (), 20, true, WIFI_PREAMBLE_HT_GF, 49.6)
    && CheckTxDuration (1536, WifiPhy::GetHtMcs6 (), 20, true, WIFI_PREAMBLE_HT_MF, 226.8)
    && CheckTxDuration (76, WifiPhy::GetHtMcs6 (), 20, true, WIFI_PREAMBLE_HT_MF, 46.8)
    && CheckTxDuration (14, WifiPhy::GetHtMcs6 (), 20, true, WIFI_PREAMBLE_HT_MF, 39.6)
    && CheckTxDuration (1536, WifiPhy::GetHtMcs6 (), 20, true, WIFI_PREAMBLE_HT_GF, 218.8)
    && CheckTxDuration (76, WifiPhy::GetHtMcs6 (), 20, true, WIFI_PREAMBLE_HT_GF, 38.8)
    && CheckTxDuration (14, WifiPhy::GetHtMcs6 (), 20, true, WIFI_PREAMBLE_HT_GF, 31.6)
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 40, false, WIFI_PREAMBLE_HT_MF, 128)
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 40, false, WIFI_PREAMBLE_HT_MF, 44)
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 40, false, WIFI_PREAMBLE_HT_MF, 40)
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 40, false, WIFI_PREAMBLE_HT_GF, 120)
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 40, false, WIFI_PREAMBLE_HT_GF, 36)
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 40, false, WIFI_PREAMBLE_HT_GF, 32)
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 40, true, WIFI_PREAMBLE_HT_MF, 118.8)
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 40, true, WIFI_PREAMBLE_HT_MF, 43.2)
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 40, true, WIFI_PREAMBLE_HT_MF, 39.6)
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 40, true, WIFI_PREAMBLE_HT_GF, 110.8)
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 40, true, WIFI_PREAMBLE_HT_GF, 35.2)
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 40, true, WIFI_PREAMBLE_HT_GF, 31.6);

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11n duration failed");

  //802.11ac durations
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs8 (), 20, false, WIFI_PREAMBLE_VHT, 200)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs8 (), 20, false, WIFI_PREAMBLE_VHT, 52)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs8 (), 20, false, WIFI_PREAMBLE_VHT, 44)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs8 (), 20, true, WIFI_PREAMBLE_VHT, 184)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs8 (), 20, true, WIFI_PREAMBLE_VHT, 50.8)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs8 (), 20, true, WIFI_PREAMBLE_VHT, 43.6)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs9 (), 40, false, WIFI_PREAMBLE_VHT, 112)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs9 (), 40, false, WIFI_PREAMBLE_VHT, 44)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs9 (), 40, false, WIFI_PREAMBLE_VHT, 44)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs9 (), 40, true, WIFI_PREAMBLE_VHT, 104.8)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs9 (), 40, true, WIFI_PREAMBLE_VHT, 43.6)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs9 (), 40, true, WIFI_PREAMBLE_VHT, 43.6)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs0 (), 80, false, WIFI_PREAMBLE_VHT, 464)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs0 (), 80, false, WIFI_PREAMBLE_VHT, 64)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs0 (), 80, false, WIFI_PREAMBLE_VHT, 48)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs0 (), 80, true, WIFI_PREAMBLE_VHT, 421.6)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs0 (), 80, true, WIFI_PREAMBLE_VHT, 61.6)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs0 (), 80, true, WIFI_PREAMBLE_VHT, 47.2)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs9 (), 80, false, WIFI_PREAMBLE_VHT, 72)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs9 (), 80, false, WIFI_PREAMBLE_VHT, 44)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs9 (), 80, false, WIFI_PREAMBLE_VHT, 44)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs9 (), 80, true, WIFI_PREAMBLE_VHT, 68.8)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs9 (), 80, true, WIFI_PREAMBLE_VHT, 43.6)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs9 (), 80, true, WIFI_PREAMBLE_VHT, 43.6)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs8 (), 160, false, WIFI_PREAMBLE_VHT, 60)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs8 (), 160, false, WIFI_PREAMBLE_VHT, 44)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs8 (), 160, false, WIFI_PREAMBLE_VHT, 44)
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs8 (), 160, true, WIFI_PREAMBLE_VHT, 58)
    && CheckTxDuration (76, WifiPhy::GetVhtMcs8 (), 160, true, WIFI_PREAMBLE_VHT, 43.6)
    && CheckTxDuration (14, WifiPhy::GetVhtMcs8 (), 160, true, WIFI_PREAMBLE_VHT, 43.6);

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11ac duration failed");
}


class TxDurationTestSuite : public TestSuite
{
public:
  TxDurationTestSuite ();
};

TxDurationTestSuite::TxDurationTestSuite ()
  : TestSuite ("devices-wifi-tx-duration", UNIT)
{
  AddTestCase (new TxDurationTest, TestCase::QUICK);
}

static TxDurationTestSuite g_txDurationTestSuite;
