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

// #include "ns3/simulator.h"
// #include "ns3/test.h"

#include "ns3/log.h"

// #include "ns3/packet.h"
// #include "ns3/simulator.h"
// #include "ns3/lte-spectrum-phy.h"
#include "ns3/lte-phy-tag.h"
#include "ns3/lte-ue-phy.h"
#include "ns3/lena-test-sinr-chunk-processor.h"

#include "ns3/lena-test-downlink-sinr.h"

NS_LOG_COMPONENT_DEFINE ("LenaTest");

using namespace ns3;


/**
 * Test 1.1 SINR calculation in downlink
 */

LenaDownlinkSinrTestCase::LenaDownlinkSinrTestCase ()
  : TestCase ("SINR calculation in downlink")
{
}

LenaDownlinkSinrTestCase::~LenaDownlinkSinrTestCase ()
{
}

void
LenaDownlinkSinrTestCase::DoRun (void)
{
  LogComponentEnable ("LteEnbRrc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeRrc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LteRlc", LOG_LEVEL_ALL);
  LogComponentEnable ("RrPacketScheduler", LOG_LEVEL_ALL);

  LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);

  LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteInterference", LOG_LEVEL_ALL);
  LogComponentEnable ("LteSinrChunkProcessor", LOG_LEVEL_ALL);

  LogComponentEnable ("LtePropagationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("LossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("ShadowingLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PenetrationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("MultipathLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PathLossModel", LOG_LEVEL_ALL);

  LogComponentEnable ("LteNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbNetDevice", LOG_LEVEL_ALL);

  LogComponentEnable ("LenaTestSinrChunkProcessor", LOG_LEVEL_ALL);
  LogComponentEnable ("LenaTest", LOG_LEVEL_ALL);

  /**
   * Instantiate a single receiving LteSpectrumPhy
   */
  Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteUePhy> uePhy = CreateObject<LteUePhy> (dlPhy, ulPhy);

  dlPhy->SetCellId (100);

  Ptr<LenaTestSinrChunkProcessor> p = Create<LenaTestSinrChunkProcessor> (uePhy->GetObject<LtePhy> ());
  dlPhy->AddSinrChunkProcessor (p);

  /**
   * Generate several calls to LteSpectrumPhy::StartRx corresponding to several signals.
   * One will be the signal of interest, i.e., the PhyTag of the first Packet in the Packet burst
   * will have the same CellId of the receiving PHY;
   * the others will have a different CellId and hence will be the interfering signals
   */

  // Number of packet bursts
  int numOfPbs = 3;

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
  double myFreqs[] = {2.10, 2.11, 2.12, 2.13, 2.14, 2.15, 2.16, 2.17, 2.18, 2.19};
  int numOfFreqs = sizeof(myFreqs) / sizeof(double);
  std::vector<double> freqs (myFreqs, myFreqs + sizeof(myFreqs) / sizeof(double) );

  Ptr<SpectrumModel> f = Create<SpectrumModel> (freqs);
  Ptr<SpectrumValue> rxPsd = Create<SpectrumValue> (f);
  for ( int i = 0 ; i < numOfFreqs ; i++ )
    {
      (*rxPsd)[i] = 1.11 + 0.01 * i;
    }

  Ptr<SpectrumValue> noisePsd = Create<SpectrumValue> (f);
  for ( int i = 0 ; i < numOfFreqs ; i++ )
    {
      (*noisePsd)[i] = 0.001;
    }

  SpectrumValue saveRxPsd = *rxPsd;
  SpectrumValue saveNoisePsd = *noisePsd;

  dlPhy->SetNoisePowerSpectralDensity (noisePsd);

  Time delay ("100ms");
  Time duration ("5ms");

  /**
   * Schedule the reception of the <numOfPbs> signals
   */
  for ( int pb = 0 ; pb < numOfPbs ; pb++)
    {
      Simulator::Schedule (delay, &LteSpectrumPhy::StartRx, dlPhy, packetBursts[pb], rxPsd, dlPhy->GetSpectrumType(), duration);
    }

  Simulator::Stop (Seconds (1.0));
  Simulator::Run ();
  Simulator::Destroy ();

  /**
   * Check that the values passed to LteSinrChunkProcessor::EvaluateSinrChunk ()
   * correspond to known values which have been calculated offline for the generated signals
   */
  NS_TEST_ASSERT_MSG_EQ (*rxPsd, saveRxPsd, "Data signal corrupted !");
  NS_TEST_ASSERT_MSG_EQ (*noisePsd, saveNoisePsd, "Noise signal corrupted !");

  SpectrumValue theoreticalSinr = (*rxPsd) / ( ( 2 * (*rxPsd) ) + (*noisePsd) );
  SpectrumValue calculatedSinr = p->GetSinr ();

//   NS_LOG_INFO ("Theoretical SINR: " << theoreticalSinr);
//   NS_LOG_INFO ("Calculated SINR: " << calculatedSinr);

  NS_TEST_ASSERT_MSG_EQ_TOL (calculatedSinr, theoreticalSinr, 0.000001, "Wrong SINR !");
}
