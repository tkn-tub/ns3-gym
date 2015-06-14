/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

//
// This script is used to verify the behavior of InterferenceHelper.
//
// The scenario consists of two IEEE 802.11 hidden stations and an access point.
// The two stations have both a packet to transmit to the access point.
//
//
// (xA,0,0)     (0,0,0)      (xB,0,0)
//
//    *   ----->   *   <-----   *
//    |            |            |
//   STA A         AP          STA B
//
//
// The program can be configured at run-time by passing command-line arguments.
// It enables to configure the delay between the transmission from station A
// and the transmission from station B (--delay option). It is also possible to
// select the tx power level (--txPowerA and --txPowerB options), the packet size
// (--packetSizeA and --packetSizeB options) and the modulation (--txModeA and
// --txModeB options) used for the respective transmissions.
//
// By default, IEEE 802.11a with long preamble type is considered, but those
// parameters can be also picked among other IEEE 802.11 flavors and preamble
// types available in the simulator (--standard and --preamble options).
// Note that the program checks the consistency between the selected standard
// the selected preamble type.
//
// The output of the program displays InterfenceHelper and YansWifiPhy trace
// logs associated to the chosen scenario.
//

#include "ns3/core-module.h"
#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/error-rate-model.h"
#include "ns3/yans-error-rate-model.h"
#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/vector.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/command-line.h"
#include "ns3/wifi-tx-vector.h"

using namespace ns3;

class InterferenceExperiment
{
public:
  struct Input
  {
    Input ();
    Time interval;
    double xA;
    double xB;
    std::string txModeA;
    std::string txModeB;
    uint32_t txPowerLevelA;
    uint32_t txPowerLevelB;
    uint32_t packetSizeA;
    uint32_t packetSizeB;
    enum WifiPhyStandard standard;
    enum WifiPreamble preamble;
  };

  InterferenceExperiment ();
  void Run (struct InterferenceExperiment::Input input);

private:
  void SendA (void) const;
  void SendB (void) const;
  Ptr<YansWifiPhy> m_txA;
  Ptr<YansWifiPhy> m_txB;
  struct Input m_input;
};

void
InterferenceExperiment::SendA (void) const
{
  Ptr<Packet> p = Create<Packet> (m_input.packetSizeA);
  WifiTxVector txVector;
  txVector.SetTxPowerLevel (m_input.txPowerLevelA);
  txVector.SetMode (WifiMode (m_input.txModeA));
  m_txA->SendPacket (p, txVector, m_input.preamble, 0);
}

void
InterferenceExperiment::SendB (void) const
{
  Ptr<Packet> p = Create<Packet> (m_input.packetSizeB);
  WifiTxVector txVector;
  txVector.SetTxPowerLevel (m_input.txPowerLevelB);
  txVector.SetMode (WifiMode (m_input.txModeB));
  m_txB->SendPacket (p, txVector, m_input.preamble, 0);
}

InterferenceExperiment::InterferenceExperiment ()
{
}
InterferenceExperiment::Input::Input ()
  : interval (MicroSeconds (0)),
    xA (-5),
    xB (5),
    txModeA ("OfdmRate54Mbps"),
    txModeB ("OfdmRate54Mbps"),
    txPowerLevelA (0),
    txPowerLevelB (0),
    packetSizeA (1500),
    packetSizeB (1500),
    standard (WIFI_PHY_STANDARD_80211a),
    preamble (WIFI_PREAMBLE_LONG)
{
}

void
InterferenceExperiment::Run (struct InterferenceExperiment::Input input)
{
  m_input = input;

  double range = std::max (std::abs (input.xA), input.xB);
  Config::SetDefault ("ns3::RangePropagationLossModel::MaxRange", DoubleValue (range));

  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  channel->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());
  Ptr<RangePropagationLossModel> loss = CreateObject<RangePropagationLossModel> ();
  channel->SetPropagationLossModel (loss);

  Ptr<MobilityModel> posTxA = CreateObject<ConstantPositionMobilityModel> ();
  posTxA->SetPosition (Vector (input.xA, 0.0, 0.0));
  Ptr<MobilityModel> posTxB = CreateObject<ConstantPositionMobilityModel> ();
  posTxB->SetPosition (Vector (input.xB, 0.0, 0.0));
  Ptr<MobilityModel> posRx = CreateObject<ConstantPositionMobilityModel> ();
  posRx->SetPosition (Vector (0.0, 0.0, 0.0));

  m_txA = CreateObject<YansWifiPhy> ();
  m_txB = CreateObject<YansWifiPhy> ();
  Ptr<YansWifiPhy> rx = CreateObject<YansWifiPhy> ();

  Ptr<ErrorRateModel> error = CreateObject<YansErrorRateModel> ();
  m_txA->SetErrorRateModel (error);
  m_txB->SetErrorRateModel (error);
  rx->SetErrorRateModel (error);
  m_txA->SetChannel (channel);
  m_txB->SetChannel (channel);
  rx->SetChannel (channel);
  m_txA->SetMobility (posTxA);
  m_txB->SetMobility (posTxB);
  rx->SetMobility (posRx);

  m_txA->ConfigureStandard (input.standard);
  m_txB->ConfigureStandard (input.standard);
  rx->ConfigureStandard (input.standard);

  Simulator::Schedule (Seconds (0), &InterferenceExperiment::SendA, this);
  Simulator::Schedule (Seconds (0) + input.interval, &InterferenceExperiment::SendB, this);

  Simulator::Run ();
  Simulator::Destroy ();
}


int main (int argc, char *argv[])
{
  InterferenceExperiment::Input input;
  std::string str_standard = "WIFI_PHY_STANDARD_80211a";
  std::string str_preamble = "WIFI_PREAMBLE_LONG";
  double delay = 0; //microseconds

  CommandLine cmd;
  cmd.AddValue ("delay", "Delay in microseconds between frame transmission from sender A and frame transmission from sender B", delay);
  cmd.AddValue ("xA", "The position of transmitter A (< 0)", input.xA);
  cmd.AddValue ("xB", "The position of transmitter B (> 0)", input.xB);
  cmd.AddValue ("packetSizeA", "Packet size in bytes of transmitter A", input.packetSizeA);
  cmd.AddValue ("packetSizeB", "Packet size in bytes of transmitter B", input.packetSizeB);
  cmd.AddValue ("txPowerA", "TX power level of transmitter A", input.txPowerLevelA);
  cmd.AddValue ("txPowerB", "TX power level of transmitter B", input.txPowerLevelB);
  cmd.AddValue ("txModeA", "Wifi mode used for payload transmission of sender A", input.txModeA);
  cmd.AddValue ("txModeB", "Wifi mode used for payload transmission of sender B", input.txModeB);
  cmd.AddValue ("standard", "IEEE 802.11 flavor", str_standard);
  cmd.AddValue ("preamble", "Type of preamble", str_preamble);
  cmd.Parse (argc, argv);

  LogComponentEnable ("YansWifiPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("InterferenceHelper", LOG_LEVEL_ALL);

  input.interval = MicroSeconds (delay);

  if (input.xA >= 0 || input.xB <= 0)
    {
      std::cout << "Value of xA must be smaller than 0 and value of xB must be bigger than 0!" << std::endl;
      return 0;
    }

  if (str_standard == "WIFI_PHY_STANDARD_80211a")
    {
      input.standard = WIFI_PHY_STANDARD_80211a;
    }
  else if (str_standard == "WIFI_PHY_STANDARD_80211b")
    {
      input.standard = WIFI_PHY_STANDARD_80211b;
    }
  else if (str_standard == "WIFI_PHY_STANDARD_80211g")
    {
      input.standard = WIFI_PHY_STANDARD_80211g;
    }
  else if (str_standard == "WIFI_PHY_STANDARD_80211n_2_4GHZ")
    {
      input.standard = WIFI_PHY_STANDARD_80211n_2_4GHZ;
    }
  else if (str_standard == "WIFI_PHY_STANDARD_80211n_5GHZ")
    {
      input.standard = WIFI_PHY_STANDARD_80211n_5GHZ;
    }

  if (str_preamble == "WIFI_PREAMBLE_LONG" && (input.standard == WIFI_PHY_STANDARD_80211a || input.standard == WIFI_PHY_STANDARD_80211b || input.standard == WIFI_PHY_STANDARD_80211g))
    {
      input.preamble = WIFI_PREAMBLE_LONG;
    }
  else if (str_preamble == "WIFI_PREAMBLE_SHORT" && (input.standard == WIFI_PHY_STANDARD_80211b || input.standard == WIFI_PHY_STANDARD_80211g))
    {
      input.preamble = WIFI_PREAMBLE_SHORT;
    }
  else if (str_preamble == "WIFI_PREAMBLE_HT_MF" && (input.standard == WIFI_PHY_STANDARD_80211n_2_4GHZ || input.standard == WIFI_PHY_STANDARD_80211n_5GHZ))
    {
      input.preamble = WIFI_PREAMBLE_HT_MF;
    }
  else if (str_preamble == "WIFI_PREAMBLE_HT_GF" && (input.standard == WIFI_PHY_STANDARD_80211n_2_4GHZ || input.standard == WIFI_PHY_STANDARD_80211n_5GHZ))
    {
      input.preamble = WIFI_PREAMBLE_HT_GF;
    }
  else
    {
      std::cout << "Preamble does not exist or is not compatible with the selected standard!" << std::endl;
      return 0;
    }

  InterferenceExperiment experiment;
  experiment.Run (input);

  return 0;
}
