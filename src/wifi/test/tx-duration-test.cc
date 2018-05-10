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

#include "ns3/log.h"
#include "ns3/test.h"
#include "ns3/yans-wifi-phy.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("InterferenceHelperTxDurationTest");

static const uint16_t CHANNEL_1_MHZ  = 2412; // a 2.4 GHz center frequency (MHz)
static const uint16_t CHANNEL_36_MHZ = 5180; // a 5 GHz center frequency (MHz)

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Tx Duration Test
 */
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
   * @param payloadMode the WifiMode used for the transmission
   * @param channelWidth the channel width used for the transmission (in MHz)
   * @param guardInterval the guard interval duration used for the transmission (in nanoseconds)
   * @param preamble the WifiPreamble used for the transmission
   * @param knownDuration the known duration value of the transmission
   *
   * @return true if values correspond, false otherwise
   */
  bool CheckPayloadDuration (uint32_t size, WifiMode payloadMode, uint16_t channelWidth, uint16_t guardInterval, WifiPreamble preamble, Time knownDuration);

  /**
   * Check if the overall tx duration returned by InterferenceHelper
   * corresponds to a known value of the pay
   *
   * @param size size of payload in octets (includes everything after the PLCP header)
   * @param payloadMode the WifiMode used for the transmission
   * @param channelWidth the channel width used for the transmission (in MHz)
   * @param guardInterval the guard interval duration used for the transmission (in nanoseconds)
   * @param preamble the WifiPreamble used for the transmission
   * @param knownDuration the known duration value of the transmission
   *
   * @return true if values correspond, false otherwise
   */
  bool CheckTxDuration (uint32_t size, WifiMode payloadMode, uint16_t channelWidth, uint16_t guardInterval, WifiPreamble preamble, Time knownDuration);

};

TxDurationTest::TxDurationTest ()
  : TestCase ("Wifi TX Duration")
{
}

TxDurationTest::~TxDurationTest ()
{
}

bool
TxDurationTest::CheckPayloadDuration (uint32_t size, WifiMode payloadMode, uint16_t channelWidth, uint16_t guardInterval, WifiPreamble preamble, Time knownDuration)
{
  WifiTxVector txVector;
  txVector.SetMode (payloadMode);
  txVector.SetPreambleType (preamble);
  txVector.SetChannelWidth (channelWidth);
  txVector.SetGuardInterval (guardInterval);
  txVector.SetNss (1);
  txVector.SetStbc (0);
  txVector.SetNess (0);
  uint16_t testedFrequency = CHANNEL_1_MHZ;
  Ptr<YansWifiPhy> phy = CreateObject<YansWifiPhy> ();
  if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_OFDM
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HT
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_VHT
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HE)
    {
      testedFrequency = CHANNEL_36_MHZ;
    }
  Time calculatedDuration = phy->GetPayloadDuration (size, txVector, testedFrequency);
  if (calculatedDuration != knownDuration)
    {
      std::cerr << "size=" << size
                << " mode=" << payloadMode
                << " channelWidth=" << channelWidth
                << " guardInterval=" << guardInterval
                << " datarate=" << payloadMode.GetDataRate (channelWidth, guardInterval, 1)
                << " known=" << knownDuration
                << " calculated=" << calculatedDuration
                << std::endl;
      return false;
    }
  if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HT || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HE)
    {
      //Durations vary depending on frequency; test also 2.4 GHz (bug 1971)
      testedFrequency = CHANNEL_1_MHZ;
      calculatedDuration = phy->GetPayloadDuration (size, txVector, testedFrequency);
      knownDuration += MicroSeconds (6);
      if (calculatedDuration != knownDuration)
        {
          std::cerr << "size=" << size
                    << " mode=" << payloadMode
                    << " channelWidth=" << channelWidth
                    << " guardInterval=" << guardInterval
                    << " datarate=" << payloadMode.GetDataRate (channelWidth, guardInterval, 1)
                    << " known=" << knownDuration
                    << " calculated=" << calculatedDuration
                    << std::endl;
          return false;
        }
    }
  return true;
}

bool
TxDurationTest::CheckTxDuration (uint32_t size, WifiMode payloadMode, uint16_t channelWidth, uint16_t guardInterval, WifiPreamble preamble, Time knownDuration)
{
  WifiTxVector txVector;
  txVector.SetMode (payloadMode);
  txVector.SetPreambleType (preamble);
  txVector.SetChannelWidth (channelWidth);
  txVector.SetGuardInterval (guardInterval);
  txVector.SetNss (1);
  txVector.SetStbc (0);
  txVector.SetNess (0);
  uint16_t testedFrequency = CHANNEL_1_MHZ;
  Ptr<YansWifiPhy> phy = CreateObject<YansWifiPhy> ();
  if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_OFDM
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HT
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_VHT
      || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HE)
    {
      testedFrequency = CHANNEL_36_MHZ;
    }
  Time calculatedDuration = phy->CalculateTxDuration (size, txVector, testedFrequency);
  if (calculatedDuration != knownDuration)
    {
      std::cerr << "size=" << size
                << " mode=" << payloadMode
                << " channelWidth=" << +channelWidth
                << " guardInterval=" << guardInterval
                << " datarate=" << payloadMode.GetDataRate (channelWidth, guardInterval, 1)
                << " preamble=" << preamble
                << " known=" << knownDuration
                << " calculated=" << calculatedDuration
                << std::endl;
      return false;
    }
  if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HT || payloadMode.GetModulationClass () == WIFI_MOD_CLASS_HE)
    {
      //Durations vary depending on frequency; test also 2.4 GHz (bug 1971)
      testedFrequency = CHANNEL_1_MHZ;
      calculatedDuration = phy->CalculateTxDuration (size, txVector, testedFrequency);
      knownDuration += MicroSeconds (6);
      if (calculatedDuration != knownDuration)
        {
          std::cerr << "size=" << size
                    << " mode=" << payloadMode
                    << " channelWidth=" << channelWidth
                    << " guardInterval=" << guardInterval
                    << " datarate=" << payloadMode.GetDataRate (channelWidth, guardInterval, 1)
                    << " preamble=" << preamble
                    << " known=" << knownDuration
                    << " calculated=" << calculatedDuration
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
    && CheckPayloadDuration (1023, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (744))
    && CheckPayloadDuration (1024, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (745))
    && CheckPayloadDuration (1025, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (746))
    && CheckPayloadDuration (1026, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (747));

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11b CCK duration failed");

  //Similar, but we add PLCP preamble and header durations
  //and we test different rates.
  //The payload durations for modes other than 11mbb have been
  //calculated by hand according to  IEEE Std 802.11-2007 18.2.3.5
  retval = retval
    && CheckTxDuration (1023, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (744 + 96))
    && CheckTxDuration (1024, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (745 + 96))
    && CheckTxDuration (1025, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (746 + 96))
    && CheckTxDuration (1026, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (747 + 96))
    && CheckTxDuration (1023, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (744 + 192))
    && CheckTxDuration (1024, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (745 + 192))
    && CheckTxDuration (1025, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (746 + 192))
    && CheckTxDuration (1026, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (747 + 192))
    && CheckTxDuration (1023, WifiPhy::GetDsssRate5_5Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (1488 + 96))
    && CheckTxDuration (1024, WifiPhy::GetDsssRate5_5Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (1490 + 96))
    && CheckTxDuration (1025, WifiPhy::GetDsssRate5_5Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (1491 + 96))
    && CheckTxDuration (1026, WifiPhy::GetDsssRate5_5Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (1493 + 96))
    && CheckTxDuration (1023, WifiPhy::GetDsssRate5_5Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (1488 + 192))
    && CheckTxDuration (1024, WifiPhy::GetDsssRate5_5Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (1490 + 192))
    && CheckTxDuration (1025, WifiPhy::GetDsssRate5_5Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (1491 + 192))
    && CheckTxDuration (1026, WifiPhy::GetDsssRate5_5Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (1493 + 192))
    && CheckTxDuration (1023, WifiPhy::GetDsssRate2Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (4092 + 96))
    && CheckTxDuration (1024, WifiPhy::GetDsssRate2Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (4096 + 96))
    && CheckTxDuration (1025, WifiPhy::GetDsssRate2Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (4100 + 96))
    && CheckTxDuration (1026, WifiPhy::GetDsssRate2Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (4104 + 96))
    && CheckTxDuration (1023, WifiPhy::GetDsssRate2Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (4092 + 192))
    && CheckTxDuration (1024, WifiPhy::GetDsssRate2Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (4096 + 192))
    && CheckTxDuration (1025, WifiPhy::GetDsssRate2Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (4100 + 192))
    && CheckTxDuration (1026, WifiPhy::GetDsssRate2Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (4104 + 192))
    && CheckTxDuration (1023, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (8184 + 192))
    && CheckTxDuration (1024, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (8192 + 192))
    && CheckTxDuration (1025, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (8200 + 192))
    && CheckTxDuration (1026, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_SHORT, MicroSeconds (8208 + 192))
    && CheckTxDuration (1023, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (8184 + 192))
    && CheckTxDuration (1024, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (8192 + 192))
    && CheckTxDuration (1025, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (8200 + 192))
    && CheckTxDuration (1026, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (8208 + 192));

  //values from http://mailman.isi.edu/pipermail/ns-developers/2009-July/006226.html
  retval = retval && CheckTxDuration (14, WifiPhy::GetDsssRate1Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (304));

  //values from http://www.oreillynet.com/pub/a/wireless/2003/08/08/wireless_throughput.html
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (1310))
    && CheckTxDuration (76, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (248))
    && CheckTxDuration (14, WifiPhy::GetDsssRate11Mbps (), 22, 800, WIFI_PREAMBLE_LONG, MicroSeconds (203));

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11b duration failed");

  //802.11a durations
  //values from http://www.oreillynet.com/pub/a/wireless/2003/08/08/wireless_throughput.html
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetOfdmRate54Mbps (), 20, 800, WIFI_PREAMBLE_LONG, MicroSeconds (248))
    && CheckTxDuration (76, WifiPhy::GetOfdmRate54Mbps (), 20, 800, WIFI_PREAMBLE_LONG, MicroSeconds (32))
    && CheckTxDuration (14, WifiPhy::GetOfdmRate54Mbps (), 20, 800, WIFI_PREAMBLE_LONG, MicroSeconds (24));

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11a duration failed");

  //802.11g durations are same as 802.11a durations but with 6 us signal extension
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetErpOfdmRate54Mbps (), 20, 800, WIFI_PREAMBLE_LONG, MicroSeconds (254))
    && CheckTxDuration (76, WifiPhy::GetErpOfdmRate54Mbps (), 20, 800, WIFI_PREAMBLE_LONG, MicroSeconds (38))
    && CheckTxDuration (14, WifiPhy::GetErpOfdmRate54Mbps (), 20, 800, WIFI_PREAMBLE_LONG, MicroSeconds (30));

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11g duration failed");

  //802.11n durations
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 20, 800, WIFI_PREAMBLE_HT_MF, MicroSeconds (228))
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 20, 800, WIFI_PREAMBLE_HT_MF, MicroSeconds (48))
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 20, 800, WIFI_PREAMBLE_HT_MF, MicroSeconds (40))
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 20, 800, WIFI_PREAMBLE_HT_GF, MicroSeconds (220))
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 20, 800, WIFI_PREAMBLE_HT_GF, MicroSeconds (40))
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 20, 800, WIFI_PREAMBLE_HT_GF, MicroSeconds (32))
    && CheckTxDuration (1536, WifiPhy::GetHtMcs0 (), 20, 400, WIFI_PREAMBLE_HT_MF, NanoSeconds (1742400))
    && CheckTxDuration (76, WifiPhy::GetHtMcs0 (), 20, 400, WIFI_PREAMBLE_HT_MF, MicroSeconds (126))
    && CheckTxDuration (14, WifiPhy::GetHtMcs0 (), 20, 400, WIFI_PREAMBLE_HT_MF, NanoSeconds (57600))
    && CheckTxDuration (1536,WifiPhy::GetHtMcs0 (), 20, 400, WIFI_PREAMBLE_HT_GF, NanoSeconds (1734400))
    && CheckTxDuration (76, WifiPhy::GetHtMcs0 (), 20, 400, WIFI_PREAMBLE_HT_GF, MicroSeconds (118))
    && CheckTxDuration (14, WifiPhy::GetHtMcs0 (), 20, 400, WIFI_PREAMBLE_HT_GF, NanoSeconds (49600))
    && CheckTxDuration (1536, WifiPhy::GetHtMcs6 (), 20, 400, WIFI_PREAMBLE_HT_MF, NanoSeconds (226800))
    && CheckTxDuration (76, WifiPhy::GetHtMcs6 (), 20, 400, WIFI_PREAMBLE_HT_MF, NanoSeconds (46800))
    && CheckTxDuration (14, WifiPhy::GetHtMcs6 (), 20, 400, WIFI_PREAMBLE_HT_MF, NanoSeconds (39600))
    && CheckTxDuration (1536, WifiPhy::GetHtMcs6 (), 20, 400, WIFI_PREAMBLE_HT_GF, NanoSeconds (218800))
    && CheckTxDuration (76, WifiPhy::GetHtMcs6 (), 20, 400, WIFI_PREAMBLE_HT_GF, NanoSeconds (38800))
    && CheckTxDuration (14, WifiPhy::GetHtMcs6 (), 20, 400, WIFI_PREAMBLE_HT_GF, NanoSeconds (31600))
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 40, 800, WIFI_PREAMBLE_HT_MF, MicroSeconds (128))
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 40, 800, WIFI_PREAMBLE_HT_MF, MicroSeconds (44))
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 40, 800, WIFI_PREAMBLE_HT_MF, MicroSeconds (40))
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 40, 800, WIFI_PREAMBLE_HT_GF, MicroSeconds (120))
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 40, 800, WIFI_PREAMBLE_HT_GF, MicroSeconds (36))
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 40, 800, WIFI_PREAMBLE_HT_GF, MicroSeconds (32))
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 40, 400, WIFI_PREAMBLE_HT_MF, NanoSeconds (118800))
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 40, 400, WIFI_PREAMBLE_HT_MF, NanoSeconds (43200))
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 40, 400, WIFI_PREAMBLE_HT_MF, NanoSeconds (39600))
    && CheckTxDuration (1536, WifiPhy::GetHtMcs7 (), 40, 400, WIFI_PREAMBLE_HT_GF, NanoSeconds (110800))
    && CheckTxDuration (76, WifiPhy::GetHtMcs7 (), 40, 400, WIFI_PREAMBLE_HT_GF, NanoSeconds (35200))
    && CheckTxDuration (14, WifiPhy::GetHtMcs7 (), 40, 400, WIFI_PREAMBLE_HT_GF, NanoSeconds (31600));

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11n duration failed");

  //802.11ac durations
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs8 (), 20, 800, WIFI_PREAMBLE_VHT, MicroSeconds (200))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs8 (), 20, 800, WIFI_PREAMBLE_VHT, MicroSeconds (52))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs8 (), 20, 800, WIFI_PREAMBLE_VHT, MicroSeconds (44))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs8 (), 20, 400, WIFI_PREAMBLE_VHT, MicroSeconds (184))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs8 (), 20, 400, WIFI_PREAMBLE_VHT, NanoSeconds (50800))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs8 (), 20, 400, WIFI_PREAMBLE_VHT, NanoSeconds (43600))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs9 (), 40, 800, WIFI_PREAMBLE_VHT, MicroSeconds (112))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs9 (), 40, 800, WIFI_PREAMBLE_VHT, MicroSeconds (44))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs9 (), 40, 800, WIFI_PREAMBLE_VHT, MicroSeconds (44))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs9 (), 40, 400, WIFI_PREAMBLE_VHT, NanoSeconds (104800))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs9 (), 40, 400, WIFI_PREAMBLE_VHT, NanoSeconds (43600))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs9 (), 40, 400, WIFI_PREAMBLE_VHT, NanoSeconds (43600))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs0 (), 80, 800, WIFI_PREAMBLE_VHT, MicroSeconds (464))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs0 (), 80, 800, WIFI_PREAMBLE_VHT, MicroSeconds (64))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs0 (), 80, 800, WIFI_PREAMBLE_VHT, MicroSeconds (48))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs0 (), 80, 400, WIFI_PREAMBLE_VHT, NanoSeconds (421600))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs0 (), 80, 400, WIFI_PREAMBLE_VHT, NanoSeconds (61600))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs0 (), 80, 400, WIFI_PREAMBLE_VHT, NanoSeconds (47200))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs9 (), 80, 800, WIFI_PREAMBLE_VHT, MicroSeconds (72))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs9 (), 80, 800, WIFI_PREAMBLE_VHT, MicroSeconds (44))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs9 (), 80, 800, WIFI_PREAMBLE_VHT, MicroSeconds (44))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs9 (), 80, 400, WIFI_PREAMBLE_VHT, NanoSeconds (68800))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs9 (), 80, 400, WIFI_PREAMBLE_VHT, NanoSeconds (43600))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs9 (), 80, 400, WIFI_PREAMBLE_VHT, NanoSeconds (43600))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs8 (), 160, 800, WIFI_PREAMBLE_VHT, MicroSeconds (60))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs8 (), 160, 800, WIFI_PREAMBLE_VHT, MicroSeconds (44))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs8 (), 160, 800, WIFI_PREAMBLE_VHT, MicroSeconds (44))
    && CheckTxDuration (1536, WifiPhy::GetVhtMcs8 (), 160, 400, WIFI_PREAMBLE_VHT, MicroSeconds (58))
    && CheckTxDuration (76, WifiPhy::GetVhtMcs8 (), 160, 400, WIFI_PREAMBLE_VHT, NanoSeconds (43600))
    && CheckTxDuration (14, WifiPhy::GetVhtMcs8 (), 160, 400, WIFI_PREAMBLE_VHT, NanoSeconds (43600));

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11ac duration failed");

  //802.11ax durations
  retval = retval
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 20, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (1485600))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 20, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (125600))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 20, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (71200))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 40, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (764800))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 40, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (84800))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 40, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 80, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (397600))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 80, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (71200))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 80, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 160, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (220800))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 160, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 160, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 20, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (1570400))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 20, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (130400))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 20, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (72800))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 40, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (807200))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 40, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (87200))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 40, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 80, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (418400))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 80, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (72800))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 80, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 160, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (231200))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 160, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 160, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 20, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (1740))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 20, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (140))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 20, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (76))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 40, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (892))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 40, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (92))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 40, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 80, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (460))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 80, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (76))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 80, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs0 (), 160, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (252))
    && CheckTxDuration (76, WifiPhy::GetHeMcs0 (), 160, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (14, WifiPhy::GetHeMcs0 (), 160, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 20, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (139200))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 20, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 20, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 40, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (98400))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 40, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 40, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 80, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (71200))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 80, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 80, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 160, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 160, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 160, 800, WIFI_PREAMBLE_HE_SU, NanoSeconds (57600))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 20, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (144800))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 20, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 20, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 40, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (101600))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 40, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 40, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 80, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (72800))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 80, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 80, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 160, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 160, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 160, 1600, WIFI_PREAMBLE_HE_SU, NanoSeconds (58400))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 20, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (156))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 20, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 20, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 40, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (108))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 40, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 40, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 80, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (76))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 80, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 80, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (1536, WifiPhy::GetHeMcs11 (), 160, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (76, WifiPhy::GetHeMcs11 (), 160, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60))
    && CheckTxDuration (14, WifiPhy::GetHeMcs11 (), 160, 3200, WIFI_PREAMBLE_HE_SU, MicroSeconds (60));

  NS_TEST_EXPECT_MSG_EQ (retval, true, "an 802.11ax duration failed");
}

/**
 * \ingroup wifi-test
 * \ingroup tests
 *
 * \brief Tx Duration Test Suite
 */
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

static TxDurationTestSuite g_txDurationTestSuite; ///< the test suite
