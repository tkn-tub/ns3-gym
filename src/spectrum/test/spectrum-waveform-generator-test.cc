/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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
 * Author: Luis Pacheco <luisbelem@gmail.com>
 */
#include <ns3/core-module.h>
#include <ns3/test.h>
#include <ns3/spectrum-module.h>


NS_LOG_COMPONENT_DEFINE ("WaveformGeneratorTest");

using namespace ns3;



class WaveformGeneratorTestCase : public TestCase
{
public:
  WaveformGeneratorTestCase (double period, double dutyCycle, double stop);
  virtual ~WaveformGeneratorTestCase ();

private:
  virtual void DoRun (void);

  void    TraceWave (Ptr<const Packet> newPkt);
  double  m_period;
  double  m_dutyCycle;
  double  m_stop;
  int     m_fails;
};

void
WaveformGeneratorTestCase::TraceWave (Ptr<const Packet> newPkt)
{
  if (Now ().GetSeconds () > m_stop)
    {
      m_fails++;
    }
}

WaveformGeneratorTestCase::WaveformGeneratorTestCase (double period, double dutyCycle, double stop)
  : TestCase ("Check stop method"),
  m_period (period),
  m_dutyCycle (dutyCycle),
  m_stop (stop),
  m_fails (0)
{
}

WaveformGeneratorTestCase::~WaveformGeneratorTestCase ()
{
}


void
WaveformGeneratorTestCase::DoRun (void)
{
  Ptr<SpectrumValue> txPsd = MicrowaveOvenSpectrumValueHelper::CreatePowerSpectralDensityMwo1 ();

  SpectrumChannelHelper channelHelper = SpectrumChannelHelper::Default ();
  channelHelper.SetChannel ("ns3::SingleModelSpectrumChannel");
  Ptr<SpectrumChannel> channel = channelHelper.Create ();

  Ptr<Node> n = CreateObject<Node> ();

  WaveformGeneratorHelper waveformGeneratorHelper;
  waveformGeneratorHelper.SetTxPowerSpectralDensity (txPsd);
  waveformGeneratorHelper.SetChannel (channel);
  waveformGeneratorHelper.SetPhyAttribute ("Period", TimeValue (Seconds (m_period)));
  waveformGeneratorHelper.SetPhyAttribute ("DutyCycle", DoubleValue (m_dutyCycle));
  NetDeviceContainer waveformGeneratorDevices = waveformGeneratorHelper.Install (n);

  Ptr<WaveformGenerator> wave = waveformGeneratorDevices.Get (0)->GetObject<NonCommunicatingNetDevice> ()->GetPhy ()->GetObject<WaveformGenerator> ();

  wave->TraceConnectWithoutContext ("TxStart", MakeCallback (&WaveformGeneratorTestCase::TraceWave,this));

  Simulator::Schedule (Seconds (1.0), &WaveformGenerator::Start, wave);
  Simulator::Schedule (Seconds (m_stop), &WaveformGenerator::Stop, wave);

  Simulator::Stop (Seconds (3.0));
  Simulator::Run ();

  NS_TEST_ASSERT_MSG_EQ (m_fails, 0, "Wave started after the stop method was called");

  Simulator::Destroy ();
}


class WaveformGeneratorTestSuite : public TestSuite
{
public:
  WaveformGeneratorTestSuite ();
};

WaveformGeneratorTestSuite::WaveformGeneratorTestSuite ()
  : TestSuite ("waveform-generator", SYSTEM)
{
  NS_LOG_INFO ("creating WaveformGeneratorTestSuite");

  // Stop while wave is active
  AddTestCase (new WaveformGeneratorTestCase (1.0, 0.5, 1.2), TestCase::QUICK);
  // Stop after wave
  AddTestCase (new WaveformGeneratorTestCase (1.0, 0.5, 1.7), TestCase::QUICK);
}

static WaveformGeneratorTestSuite g_waveformGeneratorTestSuite;
