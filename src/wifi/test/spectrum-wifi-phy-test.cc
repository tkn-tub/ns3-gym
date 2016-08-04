/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 University of Washington
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
 */

#include "ns3/test.h"
#include "ns3/packet.h"
#include "ns3/tag.h"
#include "ns3/packet-burst.h"
#include "ns3/spectrum-wifi-helper.h"
#include "ns3/wifi-spectrum-value-helper.h"
#include "ns3/spectrum-wifi-phy.h"
#include "ns3/interference-helper.h"
#include "ns3/nist-error-rate-model.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/wifi-mac-trailer.h"
#include "ns3/wifi-phy-tag.h"
#include "ns3/wifi-phy-standard.h"
#include "ns3/wifi-spectrum-signal-parameters.h"

using namespace ns3;

static const uint16_t CHANNEL_NUMBER = 36;
static const uint32_t FREQUENCY = 5180; // MHz
static const uint32_t CHANNEL_WIDTH = 20; // MHz

class SpectrumWifiPhyBasicTest : public TestCase
{
public:
  SpectrumWifiPhyBasicTest ();
  SpectrumWifiPhyBasicTest (std::string name);
  virtual ~SpectrumWifiPhyBasicTest ();
protected:
  virtual void DoSetup (void);
  Ptr<SpectrumWifiPhy> m_phy;
  Ptr<SpectrumSignalParameters> MakeSignal (double txPowerWatts);
  void SendSignal (double txPowerWatts);
  void SpectrumWifiPhyReceiver (bool rxSucceeded);
  uint32_t m_count;
private:
  virtual void DoRun (void);
};

SpectrumWifiPhyBasicTest::SpectrumWifiPhyBasicTest ()
  : TestCase ("SpectrumWifiPhy test case receives one packet"),
    m_count (0)
{
}

SpectrumWifiPhyBasicTest::SpectrumWifiPhyBasicTest (std::string name)
  : TestCase (name),
    m_count (0)
{
}

// Make a Wi-Fi signal to inject directly to the StartRx() method
Ptr<SpectrumSignalParameters> 
SpectrumWifiPhyBasicTest::MakeSignal (double txPowerWatts)
{
  WifiPreamble preamble;
  preamble = WIFI_PREAMBLE_LONG;
  WifiMode mode = WifiPhy::GetOfdmRate6Mbps ();
  WifiTxVector txVector = WifiTxVector (mode, 0, 0, false, 1, 0, 20000000, false, false);
  enum mpduType mpdutype = NORMAL_MPDU;

  Ptr<Packet> pkt = Create<Packet> (1000);
  WifiMacHeader hdr;
  WifiMacTrailer trailer;

  hdr.SetType (WIFI_MAC_QOSDATA);
  hdr.SetQosTid (0);
  uint32_t size = pkt->GetSize () + hdr.GetSize () + trailer.GetSerializedSize ();
  Time txDuration = m_phy->CalculateTxDuration (size, txVector, preamble, m_phy->GetFrequency(), mpdutype, 0);
  hdr.SetDuration (txDuration);

  pkt->AddHeader (hdr);
  pkt->AddTrailer (trailer);
  WifiPhyTag tag (txVector, preamble, mpdutype);
  pkt->AddPacketTag (tag);
  Ptr<SpectrumValue> txPowerSpectrum = WifiSpectrumValueHelper::CreateOfdmTxPowerSpectralDensity (FREQUENCY, CHANNEL_WIDTH, txPowerWatts);
  Ptr<WifiSpectrumSignalParameters> txParams = Create<WifiSpectrumSignalParameters> ();
  txParams->psd = txPowerSpectrum;
  txParams->txPhy = 0;
  txParams->duration = txDuration;
  txParams->packet = pkt;
  return txParams;
}

// Make a Wi-Fi signal to inject directly to the StartRx() method
void
SpectrumWifiPhyBasicTest::SendSignal (double txPowerWatts)
{
  m_phy->StartRx (MakeSignal (txPowerWatts));
}

void
SpectrumWifiPhyBasicTest::SpectrumWifiPhyReceiver (bool rxSucceeded)
{
  m_count++;
}

SpectrumWifiPhyBasicTest::~SpectrumWifiPhyBasicTest ()
{
}

// Create necessary objects, and inject signals.  Test that the expected
// number of packet receptions occur.
void
SpectrumWifiPhyBasicTest::DoSetup (void)
{
  m_phy = CreateObject<SpectrumWifiPhy> ();
  m_phy->ConfigureStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
  Ptr<ErrorRateModel> error = CreateObject<NistErrorRateModel> ();
  m_phy->SetErrorRateModel (error);
  m_phy->SetChannelNumber (CHANNEL_NUMBER);
  m_phy->SetFrequency (FREQUENCY);
  m_phy->SetPacketReceivedCallback (MakeCallback (&SpectrumWifiPhyBasicTest::SpectrumWifiPhyReceiver, this));
  //Bug 2460: CcaMode1Threshold default should be set to -62 dBm when using Spectrum
  m_phy->SetCcaMode1Threshold (-62.0);
}

// Test that the expected number of packet receptions occur.
void
SpectrumWifiPhyBasicTest::DoRun (void)
{
  double txPowerWatts = 0.010;
  // Send packets spaced 1 second apart; all should be received
  Simulator::Schedule (Seconds (1), &SpectrumWifiPhyBasicTest::SendSignal, this, txPowerWatts); 
  Simulator::Schedule (Seconds (2), &SpectrumWifiPhyBasicTest::SendSignal, this, txPowerWatts); 
  Simulator::Schedule (Seconds (3), &SpectrumWifiPhyBasicTest::SendSignal, this, txPowerWatts); 
  // Send packets spaced 1 microsecond second apart; only one should be received
  Simulator::Schedule (MicroSeconds (4000000), &SpectrumWifiPhyBasicTest::SendSignal, this, txPowerWatts); 
  Simulator::Schedule (MicroSeconds (4000001), &SpectrumWifiPhyBasicTest::SendSignal, this, txPowerWatts); 
  Simulator::Run ();
  Simulator::Destroy ();

  NS_TEST_ASSERT_MSG_EQ (m_count, 4, "Didn't receive right number of packets");
}

class TestPhyListener : public ns3::WifiPhyListener
{
public:
  /**
   * Create a test PhyListener
   *
   */
  TestPhyListener (void) :
    m_notifyRxStart (0),
    m_notifyRxEndOk (0),
    m_notifyRxEndError (0),
    m_notifyMaybeCcaBusyStart (0)
  {
  }
  virtual ~TestPhyListener ()
  {
  }
  virtual void NotifyRxStart (Time duration)
  {
    ++m_notifyRxStart;
  }
  virtual void NotifyRxEndOk (void)
  {
    ++m_notifyRxEndOk;
  }
  virtual void NotifyRxEndError (void)
  {
    ++m_notifyRxEndError;
  }
  virtual void NotifyTxStart (Time duration, double txPowerDbm)
  {
  }
  virtual void NotifyMaybeCcaBusyStart (Time duration)
  {
    ++m_notifyMaybeCcaBusyStart;
  }
  virtual void NotifySwitchingStart (Time duration)
  {
  }
  virtual void NotifySleep (void)
  {
  }
  virtual void NotifyWakeup (void)
  {
  }
  uint32_t m_notifyRxStart;
  uint32_t m_notifyRxEndOk;
  uint32_t m_notifyRxEndError;
  uint32_t m_notifyMaybeCcaBusyStart;
private:
};


class SpectrumWifiPhyListenerTest : public SpectrumWifiPhyBasicTest
{
public:
  SpectrumWifiPhyListenerTest ();
  virtual ~SpectrumWifiPhyListenerTest ();
private:
  virtual void DoSetup (void);
  virtual void DoRun (void);
  TestPhyListener* m_listener;
};

SpectrumWifiPhyListenerTest::SpectrumWifiPhyListenerTest ()
  : SpectrumWifiPhyBasicTest ("SpectrumWifiPhy test operation of WifiPhyListener")
{
}

SpectrumWifiPhyListenerTest::~SpectrumWifiPhyListenerTest ()
{
}

void
SpectrumWifiPhyListenerTest::DoSetup (void)
{
  SpectrumWifiPhyBasicTest::DoSetup ();
  m_listener = new TestPhyListener;
  m_phy->RegisterListener (m_listener);
}

void
SpectrumWifiPhyListenerTest::DoRun (void)
{
  double txPowerWatts = 0.010;
  Simulator::Schedule (Seconds (1), &SpectrumWifiPhyListenerTest::SendSignal, this, txPowerWatts); 
  Simulator::Run ();

  NS_TEST_ASSERT_MSG_EQ (m_count, 1, "Didn't receive right number of packets");
  NS_TEST_ASSERT_MSG_EQ (m_listener->m_notifyRxStart, 1, "Didn't receive NotifyRxStart");
  NS_TEST_ASSERT_MSG_EQ (m_listener->m_notifyRxEndOk, 1, "Didn't receive NotifyRxEnd");
  NS_TEST_ASSERT_MSG_EQ (m_listener->m_notifyMaybeCcaBusyStart, 0, "Received NotifyMaybeCcaBusyStart unexpectedly");

  Simulator::Destroy ();
  delete m_listener;
}

class SpectrumWifiPhyTestSuite : public TestSuite
{
public:
  SpectrumWifiPhyTestSuite ();
};

SpectrumWifiPhyTestSuite::SpectrumWifiPhyTestSuite ()
  : TestSuite ("spectrum-wifi-phy", UNIT)
{
  AddTestCase (new SpectrumWifiPhyBasicTest, TestCase::QUICK);
  AddTestCase (new SpectrumWifiPhyListenerTest, TestCase::QUICK);
}

static SpectrumWifiPhyTestSuite spectrumWifiPhyTestSuite;
