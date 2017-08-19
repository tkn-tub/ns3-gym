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
#include "ns3/yans-wifi-channel.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/nist-error-rate-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/simple-frame-capture-model.h"
#include "ns3/log.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("test-interference-helper");

bool checkResults = false;
bool expectRxASuccessfull = false;
bool expectRxBSuccessfull = false;

/// InterferenceExperiment
class InterferenceExperiment
{
public:
  /// Input atructure
  struct Input
  {
    Input ();
    Time interval; ///< interval
    double xA; ///< x A
    double xB; ///< x B
    std::string txModeA; ///< transmit mode A
    std::string txModeB; ///< transmit mode B
    double txPowerLevelA; ///< transmit power level A
    double txPowerLevelB; ///< transmit power level B
    uint32_t packetSizeA; ///< packet size A
    uint32_t packetSizeB; ///< packet size B
    WifiPhyStandard standard; ///< standard
    WifiPreamble preamble; ///< preamble
    bool captureEnabled; ///< whether physical layer capture is enabled
    double captureMargin; ///< margin used for physical layer capture
  };

  InterferenceExperiment ();
  /**
   * Run function
   * \param input the interference experiment data
   */
  void Run (struct InterferenceExperiment::Input input);

private:
  /**
   * Function triggered when a packet is dropped
   * \param packet the packet that was dropped
   */
  void PacketDropped(Ptr<const Packet> packet);
  /// Send A function
  void SendA (void) const;
  /// Send B function
  void SendB (void) const;
  Ptr<YansWifiPhy> m_txA; ///< transmit A function
  Ptr<YansWifiPhy> m_txB; ///< transmit B function
  struct Input m_input; ///< input
  bool m_droppedA; ///< flag to indicate whether packet A has been dropped
  bool m_droppedB; ///< flag to indicate whether packet B has been dropped
};

void
InterferenceExperiment::SendA (void) const
{
  Ptr<Packet> p = Create<Packet> (m_input.packetSizeA);
  WifiTxVector txVector;
  txVector.SetTxPowerLevel (m_input.txPowerLevelA);
  txVector.SetMode (WifiMode (m_input.txModeA));
  txVector.SetPreambleType (m_input.preamble);
  m_txA->SendPacket (p, txVector);
}

void
InterferenceExperiment::SendB (void) const
{
  Ptr<Packet> p = Create<Packet> (m_input.packetSizeB);
  WifiTxVector txVector;
  txVector.SetTxPowerLevel (m_input.txPowerLevelB);
  txVector.SetMode (WifiMode (m_input.txModeB));
  txVector.SetPreambleType (m_input.preamble);
  m_txB->SendPacket (p, txVector);
}

void
InterferenceExperiment::PacketDropped(Ptr<const Packet> packet)
{
    if (packet->GetUid () == 0)
    {
      m_droppedA = true;
    }
    else if (packet->GetUid () == 1)
    {
      m_droppedB = true;
    }
    else
    {
      NS_LOG_ERROR ("Unknown packet!");
      exit (1);
    }
}

InterferenceExperiment::InterferenceExperiment ()
  : m_droppedA (false),
    m_droppedB (false)
{
}

InterferenceExperiment::Input::Input ()
  : interval (MicroSeconds (0)),
    xA (-5),
    xB (5),
    txModeA ("OfdmRate54Mbps"),
    txModeB ("OfdmRate54Mbps"),
    txPowerLevelA (16.0206),
    txPowerLevelB (16.0206),
    packetSizeA (1500),
    packetSizeB (1500),
    standard (WIFI_PHY_STANDARD_80211a),
    preamble (WIFI_PREAMBLE_LONG),
    captureEnabled (false),
    captureMargin (0)
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
  m_txA->SetTxPowerStart (input.txPowerLevelA);
  m_txA->SetTxPowerEnd (input.txPowerLevelA);
  m_txB = CreateObject<YansWifiPhy> ();
  m_txB->SetTxPowerStart (input.txPowerLevelB);
  m_txB->SetTxPowerEnd (input.txPowerLevelB);
  Ptr<YansWifiPhy> rx = CreateObject<YansWifiPhy> ();

  Ptr<ErrorRateModel> error = CreateObject<NistErrorRateModel> ();
  m_txA->SetErrorRateModel (error);
  m_txB->SetErrorRateModel (error);
  rx->SetErrorRateModel (error);
  m_txA->SetChannel (channel);
  m_txB->SetChannel (channel);
  rx->SetChannel (channel);
  m_txA->SetMobility (posTxA);
  m_txB->SetMobility (posTxB);
  rx->SetMobility (posRx);
  if (input.captureEnabled)
    {
      Ptr<SimpleFrameCaptureModel> frameCaptureModel = CreateObject<SimpleFrameCaptureModel> ();
      frameCaptureModel->SetMargin (input.captureMargin);
      rx->SetFrameCaptureModel (frameCaptureModel);
    }

  m_txA->ConfigureStandard (input.standard);
  m_txB->ConfigureStandard (input.standard);
  rx->ConfigureStandard (input.standard);

  rx->TraceConnectWithoutContext("PhyRxDrop", MakeCallback(&InterferenceExperiment::PacketDropped, this));

  Simulator::Schedule (Seconds (0), &InterferenceExperiment::SendA, this);
  Simulator::Schedule (Seconds (0) + input.interval, &InterferenceExperiment::SendB, this);

  Simulator::Run ();
  Simulator::Destroy ();
  m_txB->Dispose ();
  m_txA->Dispose ();
  rx->Dispose ();

  if(checkResults && (m_droppedA == expectRxASuccessfull || m_droppedB == expectRxBSuccessfull))
  {
    NS_LOG_ERROR ("Results are not expected!");
    exit (1);
  }
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
  cmd.AddValue ("enableCapture", "Enable/disable physical layer capture", input.captureEnabled);
  cmd.AddValue ("captureMargin", "Margin used for physical layer capture", input.captureMargin);
  cmd.AddValue ("checkResults", "Used to check results at the end of the test", checkResults);
  cmd.AddValue ("expectRxASuccessfull", "Indicate whether packet A is expected to be successfully received", expectRxASuccessfull);
  cmd.AddValue ("expectRxBSuccessfull", "Indicate whether packet B is expected to be successfully received", expectRxBSuccessfull);
  cmd.Parse (argc, argv);

  LogComponentEnable ("WifiPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("YansWifiPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("InterferenceHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("SimpleFrameCaptureModel", LOG_LEVEL_ALL);

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
  else if (str_standard == "WIFI_PHY_STANDARD_80211ac")
    {
      input.standard = WIFI_PHY_STANDARD_80211ac;
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
  else if (str_preamble == "WIFI_PREAMBLE_VHT" && input.standard == WIFI_PHY_STANDARD_80211ac)
    {
      input.preamble = WIFI_PREAMBLE_VHT;
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
