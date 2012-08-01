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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#include <ns3/object.h>
#include <ns3/log.h>
#include <ns3/test.h>
#include <iostream>
#include "ns3/interference-helper.h"
#include "ns3/wifi-phy.h"

NS_LOG_COMPONENT_DEFINE ("InterferenceHelperTxDurationTest");

namespace ns3 {

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
   * @param knownDurationMicroSeconds the known payload size
   *
   * @return true if values correspond, false otherwise
   */
  bool CheckPayloadDuration (uint32_t size, WifiMode payloadMode,  uint32_t knownDurationMicroSeconds);

  /**
   * Check if the overall tx duration returned by InterferenceHelper
   * corresponds to a known value of the pay
   *
   * @param size size of payload in octets (includes everything after the PLCP header)
   * @param payloadMode the WifiMode used
   * @param preamble the WifiPreamble used
   * @param knownDurationMicroSeconds the known value of the transmission
   *
   * @return true if values correspond, false otherwise
   */
  bool CheckTxDuration (uint32_t size, WifiMode payloadMode,  WifiPreamble preamble, uint32_t knownDurationMicroSeconds);

};


TxDurationTest::TxDurationTest ()
  : TestCase ("Wifi TX Duration")
{
}


TxDurationTest::~TxDurationTest ()
{
}

bool
TxDurationTest::CheckPayloadDuration (uint32_t size, WifiMode payloadMode, uint32_t knownDurationMicroSeconds)
{
  uint32_t calculatedDurationMicroSeconds = WifiPhy::GetPayloadDurationMicroSeconds (size, payloadMode);
  if (calculatedDurationMicroSeconds != knownDurationMicroSeconds)
    {
      std::cerr << " size=" << size
                << " mode=" << payloadMode
                << " known=" << knownDurationMicroSeconds
                << " calculated=" << calculatedDurationMicroSeconds
                << std::endl;
      return false;
    }
  return true;
}

bool
TxDurationTest::CheckTxDuration (uint32_t size, WifiMode payloadMode, WifiPreamble preamble, uint32_t knownDurationMicroSeconds)
{
  uint32_t calculatedDurationMicroSeconds = WifiPhy::CalculateTxDuration (size, payloadMode, preamble).GetMicroSeconds ();
  if (calculatedDurationMicroSeconds != knownDurationMicroSeconds)
    {
      std::cerr << " size=" << size
                << " mode=" << payloadMode
                << " preamble=" << preamble
                << " known=" << knownDurationMicroSeconds
                << " calculated=" << calculatedDurationMicroSeconds
                << std::endl;
      return false;
    }
  return true;
}

void
TxDurationTest::DoRun (void)
{
  bool retval = true;

  // IEEE Std 802.11-2007 Table 18-2 "Example of LENGTH calculations for CCK"
  retval = retval
    && CheckPayloadDuration (1023, WifiPhy::GetDsssRate11Mbps (), 744)
    && CheckPayloadDuration (1024, WifiPhy::GetDsssRate11Mbps (), 745)
    && CheckPayloadDuration (1025, WifiPhy::GetDsssRate11Mbps (), 746)
    && CheckPayloadDuration (1026, WifiPhy::GetDsssRate11Mbps (), 747);


  // Similar, but we add PLCP preamble and header durations
  // and we test different rates.
  // The payload durations for modes other than 11mbb have been
  // calculated by hand according to  IEEE Std 802.11-2007 18.2.3.5
  retval = retval
    && CheckTxDuration (1023, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_SHORT, 744 + 96)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_SHORT, 745 + 96)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_SHORT, 746 + 96)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_SHORT, 747 + 96)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_LONG, 744 + 192)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_LONG, 745 + 192)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_LONG, 746 + 192)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_LONG, 747 + 192)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate5_5Mbps (), WIFI_PREAMBLE_SHORT, 1488 + 96)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate5_5Mbps (), WIFI_PREAMBLE_SHORT, 1490 + 96)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate5_5Mbps (), WIFI_PREAMBLE_SHORT, 1491 + 96)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate5_5Mbps (), WIFI_PREAMBLE_SHORT, 1493 + 96)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate5_5Mbps (), WIFI_PREAMBLE_LONG, 1488 + 192)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate5_5Mbps (), WIFI_PREAMBLE_LONG, 1490 + 192)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate5_5Mbps (), WIFI_PREAMBLE_LONG, 1491 + 192)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate5_5Mbps (), WIFI_PREAMBLE_LONG, 1493 + 192)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate2Mbps (), WIFI_PREAMBLE_SHORT, 4092 + 96)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate2Mbps (), WIFI_PREAMBLE_SHORT, 4096 + 96)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate2Mbps (), WIFI_PREAMBLE_SHORT, 4100 + 96)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate2Mbps (), WIFI_PREAMBLE_SHORT, 4104 + 96)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate2Mbps (), WIFI_PREAMBLE_LONG, 4092 + 192)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate2Mbps (), WIFI_PREAMBLE_LONG, 4096 + 192)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate2Mbps (), WIFI_PREAMBLE_LONG, 4100 + 192)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate2Mbps (), WIFI_PREAMBLE_LONG, 4104 + 192)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_SHORT, 8184 + 96)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_SHORT, 8192 + 96)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_SHORT, 8200 + 96)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_SHORT, 8208 + 96)
    && CheckTxDuration (1023, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_LONG, 8184 + 192)
    && CheckTxDuration (1024, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_LONG, 8192 + 192)
    && CheckTxDuration (1025, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_LONG, 8200 + 192)
    && CheckTxDuration (1026, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_LONG, 8208 + 192);

  // values from http://mailman.isi.edu/pipermail/ns-developers/2009-July/006226.html
  retval = retval && CheckTxDuration (14, WifiPhy::GetDsssRate1Mbps (), WIFI_PREAMBLE_LONG, 304);

  // values from
  // http://www.oreillynet.com/pub/a/wireless/2003/08/08/wireless_throughput.html
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_LONG, 1310)
    && CheckTxDuration (76, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_LONG, 248)
    && CheckTxDuration (14, WifiPhy::GetDsssRate11Mbps (), WIFI_PREAMBLE_LONG, 203)
    && CheckTxDuration (1536, WifiPhy::GetOfdmRate54Mbps (), WIFI_PREAMBLE_LONG, 248)
    && CheckTxDuration (76, WifiPhy::GetOfdmRate54Mbps (), WIFI_PREAMBLE_LONG, 32)
    && CheckTxDuration (14, WifiPhy::GetOfdmRate54Mbps (), WIFI_PREAMBLE_LONG, 24);

  // 802.11g durations are same as 802.11a durations but with 6 us signal extension
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetErpOfdmRate54Mbps (), WIFI_PREAMBLE_LONG, 254)
    && CheckTxDuration (76, WifiPhy::GetErpOfdmRate54Mbps (), WIFI_PREAMBLE_LONG, 38)
    && CheckTxDuration (14, WifiPhy::GetErpOfdmRate54Mbps (), WIFI_PREAMBLE_LONG, 30);
}

class TxDurationTestSuite : public TestSuite
{
public:
  TxDurationTestSuite ();
};

TxDurationTestSuite::TxDurationTestSuite ()
  : TestSuite ("devices-wifi-tx-duration", UNIT)
{
  AddTestCase (new TxDurationTest);
}

static TxDurationTestSuite g_txDurationTestSuite;
} // namespace ns3

