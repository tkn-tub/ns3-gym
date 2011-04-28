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

#include "ns3/simulator.h"

#include "ns3/log.h"

// #include "ns3/packet.h"
// #include "ns3/simulator.h"
// #include "ns3/lte-spectrum-phy.h"
#include "ns3/lte-phy-tag.h"
#include "ns3/lte-ue-phy.h"
#include "ns3/lena-test-sinr-chunk-processor.h"

#include "ns3/lena-test-downlink-sinr.h"

NS_LOG_COMPONENT_DEFINE ("LenaDownlinkSinrTest");

using namespace ns3;


/**
 * Test 1.1 SINR calculation in downlink
 */

/**
 * TestSuite
 */

LenaDownlinkSinrTestSuite::LenaDownlinkSinrTestSuite ()
  : TestSuite ("lena-downlink-sinr", SYSTEM)
{
  NS_LOG_INFO ("Creating LenaDownlinkSinrTestSuite");

  AddTestCase (new LenaDownlinkSinrTestCase);
}

static LenaDownlinkSinrTestSuite lenaDownlinkSinrTestSuite;


/**
 * TestCase
 */

LenaDownlinkSinrTestCase::LenaDownlinkSinrTestCase ()
  : TestCase ("SINR calculation in downlink")
{
  NS_LOG_INFO ("Creating LenaDownlinkSinrTestCase");
}

LenaDownlinkSinrTestCase::~LenaDownlinkSinrTestCase ()
{
}

void
LenaDownlinkSinrTestCase::DoRun (void)
{
  LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);

  LogComponentEnable ("LteEnbRrc", logLevel);
  LogComponentEnable ("LteUeRrc", logLevel);
  LogComponentEnable ("LteEnbMac", logLevel);
  LogComponentEnable ("LteUeMac", logLevel);
  LogComponentEnable ("LteRlc", logLevel);
  LogComponentEnable ("RrPacketScheduler", logLevel);

  LogComponentEnable ("LtePhy", logLevel);
  LogComponentEnable ("LteEnbPhy", logLevel);
  LogComponentEnable ("LteUePhy", logLevel);

  LogComponentEnable ("LteSpectrumPhy", logLevel);
  LogComponentEnable ("LteInterference", logLevel);
  LogComponentEnable ("LteSinrChunkProcessor", logLevel);

  LogComponentEnable ("LtePropagationLossModel", logLevel);
  LogComponentEnable ("LossModel", logLevel);
  LogComponentEnable ("ShadowingLossModel", logLevel);
  LogComponentEnable ("PenetrationLossModel", logLevel);
  LogComponentEnable ("MultipathLossModel", logLevel);
  LogComponentEnable ("PathLossModel", logLevel);

  LogComponentEnable ("LteNetDevice", logLevel);
  LogComponentEnable ("LteUeNetDevice", logLevel);
  LogComponentEnable ("LteEnbNetDevice", logLevel);

  LogComponentEnable ("LenaTestSinrChunkProcessor", logLevel);
  LogComponentEnable ("LenaDownlinkSinrTest", logLevel);
  LogComponentEnable ("LenaUplinkSinrTest", logLevel);

  /**
   * Instantiate a single receiving LteSpectrumPhy
   */
  Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteUePhy> uePhy = CreateObject<LteUePhy> (dlPhy, ulPhy);

  dlPhy->SetCellId (100);

  Ptr<LenaTestSinrChunkProcessor> chunkProcessor = Create<LenaTestSinrChunkProcessor> (uePhy->GetObject<LtePhy> ());
  dlPhy->AddSinrChunkProcessor (chunkProcessor);

  /**
   * Generate several calls to LteSpectrumPhy::StartRx corresponding to several signals.
   * One will be the signal of interest, i.e., the PhyTag of the first Packet in the Packet burst
   * will have the same CellId of the receiving PHY;
   * the others will have a different CellId and hence will be the interfering signals
   */

  // Number of packet bursts (1 data + 4 interferences)
  int numOfPbs = 5;

  // Number of packets in the packet bursts
  int numOfPkts = 10;

  // Packet bursts
  Ptr<PacketBurst> packetBursts[numOfPbs];

  // Packets
  Ptr<Packet> pkt[numOfPbs][numOfPkts];

  // Phy tags
  LtePhyTag pktTag[numOfPbs];

  /**
   * Build packet burst
   */
  for ( int pb = 0 ; pb < numOfPbs ; pb++ )
    {
      // Create packet burst
      packetBursts[pb] = CreateObject<PacketBurst> ();

      // Create packets and add them to the burst
      for ( int i = 0 ; i < numOfPkts ; i++ )
        {
          pkt[pb][i] = Create<Packet> (1000);

          if ( i == 0 )
            {
              // Create phy tag (different for each packet burst)
              // and add to the first packet
              pktTag[pb] = LtePhyTag (100 * (pb+1));
              pkt[pb][i]->AddPacketTag ( pktTag[pb] );
            }

          packetBursts[pb]->AddPacket ( pkt[pb][i] );
        }
    }

  /**
   * Build Spectrum Model values for signal
   */
  Ptr<SpectrumModel> sm;

  Bands bands;
  BandInfo bi;

  bi.fl = 2.400e9;
  bi.fc = 2.410e9;
  bi.fh = 2.420e9;
  bands.push_back (bi);

  bi.fl = 2.420e9;
  bi.fc = 2.431e9;
  bi.fh = 2.442e9;
  bands.push_back (bi);

  sm = Create<SpectrumModel> (bands);

  // Power Spectral Density of the signal of interest = [-46 -48] dBm and BW = [20 22] MHz
  Ptr<SpectrumValue> rxPsd = Create<SpectrumValue> (sm);
  (*rxPsd)[0] = 1.255943215755e-15;
  (*rxPsd)[1] = 7.204059965732e-16;

  Ptr<SpectrumValue> noisePsd = Create<SpectrumValue> (sm);
  Ptr<SpectrumValue> i1 = Create<SpectrumValue> (sm);
  Ptr<SpectrumValue> i2 = Create<SpectrumValue> (sm);
  Ptr<SpectrumValue> i3 = Create<SpectrumValue> (sm);
  Ptr<SpectrumValue> i4 = Create<SpectrumValue> (sm);

  (*noisePsd)[0] = 5.000000000000e-19;
  (*noisePsd)[1] = 4.545454545455e-19;

  (*i1)[0] = 5.000000000000e-18;
  (*i2)[0] = 5.000000000000e-16;
  (*i3)[0] = 1.581138830084e-16;
  (*i4)[0] = 7.924465962306e-17;
  (*i1)[1] = 1.437398936440e-18;
  (*i2)[1] = 5.722388235428e-16;
  (*i3)[1] = 7.204059965732e-17;
  (*i4)[1] = 5.722388235428e-17;

  Time ts  = Seconds (1);
  Time ds  = Seconds (1);
  Time ti1 = Seconds (0);
  Time di1 = Seconds (3);
  Time ti2 = Seconds (0.7);
  Time di2 = Seconds (1);
  Time ti3 = Seconds (1.2);
  Time di3 = Seconds (1);
  Time ti4 = Seconds (1.5);
  Time di4 = Seconds (0.1);

  dlPhy->SetNoisePowerSpectralDensity (noisePsd);

  /**
   * Schedule the reception of the data signal plus the interference signals
   */

  Simulator::Schedule (ts, &LteSpectrumPhy::StartRx, dlPhy, packetBursts[0], rxPsd, dlPhy->GetSpectrumType(), ds);

  Simulator::Schedule (ti1, &LteSpectrumPhy::StartRx, dlPhy, packetBursts[1], i1, dlPhy->GetSpectrumType(), di1);
  Simulator::Schedule (ti2, &LteSpectrumPhy::StartRx, dlPhy, packetBursts[2], i2, dlPhy->GetSpectrumType(), di2);
  Simulator::Schedule (ti3, &LteSpectrumPhy::StartRx, dlPhy, packetBursts[3], i3, dlPhy->GetSpectrumType(), di3);
  Simulator::Schedule (ti4, &LteSpectrumPhy::StartRx, dlPhy, packetBursts[4], i4, dlPhy->GetSpectrumType(), di4);

  Simulator::Stop (Seconds (5.0));
  Simulator::Run ();
  Simulator::Destroy ();

  /**
   * Check that the values passed to LteSinrChunkProcessor::EvaluateSinrChunk () correspond
   * to known values which have been calculated offline (with octave) for the generated signals
   */
  SpectrumValue theoreticalSinr(sm);
  theoreticalSinr[0] = 3.72589167251055;
  theoreticalSinr[1] = 3.72255684126076;

  SpectrumValue calculatedSinr = chunkProcessor->GetSinr ();

  NS_LOG_INFO ("Theoretical SINR: " << theoreticalSinr);
  NS_LOG_INFO ("Calculated SINR: " << calculatedSinr);

  NS_TEST_ASSERT_MSG_EQ_TOL (calculatedSinr, theoreticalSinr, 0.0000001, "Wrong SINR !");
}
