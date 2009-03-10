#include "wifi-net-device.h"
#include "yans-wifi-channel.h"
#include "yans-wifi-phy.h"
#include "propagation-loss-model.h"
#include "propagation-delay-model.h"
#include "error-rate-model.h"
#include "yans-error-rate-model.h"
#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/vector.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/command-line.h"
#include "ns3/flow-id-tag.h"

using namespace ns3;

class PsrExperiment
{
public:
  struct Input
  {
    Input ();
    double distance;
    std::string txMode;
    uint8_t txPowerLevel;
    uint32_t packetSize;
    uint32_t nPackets;
  };
  struct Output
  {
    uint32_t received;
  };
  PsrExperiment ();

  struct PsrExperiment::Output Run (struct PsrExperiment::Input input);

private:
  void Send (void);
  void Receive (Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble);
  Ptr<WifiPhy> m_tx;
  struct Input m_input;
  struct Output m_output;
};

void 
PsrExperiment::Send (void)
{
  Ptr<Packet> p = Create<Packet> (m_input.packetSize);
  WifiMode mode = WifiMode (m_input.txMode);
  m_tx->SendPacket (p, mode, WIFI_PREAMBLE_SHORT, m_input.txPowerLevel);
}

void 
PsrExperiment::Receive (Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble)
{
  m_output.received++;
}

PsrExperiment::PsrExperiment ()
{}
PsrExperiment::Input::Input ()
  : distance (5.0),
    txMode ("wifia-6mbs"),
    txPowerLevel (0),
    packetSize (2304),
    nPackets (400)
{}

struct PsrExperiment::Output
PsrExperiment::Run (struct PsrExperiment::Input input)
{
  m_output.received = 0;
  m_input = input;

  Ptr<MobilityModel> posTx = CreateObject<ConstantPositionMobilityModel> ();
  posTx->SetPosition (Vector (0.0, 0.0, 0.0));
  Ptr<MobilityModel> posRx = CreateObject<ConstantPositionMobilityModel> ();
  posRx->SetPosition (Vector (m_input.distance, 0.0, 0.0));

  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  channel->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());
  Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();
  channel->SetPropagationLossModel (log);

  Ptr<YansWifiPhy> tx = CreateObject<YansWifiPhy> ();
  Ptr<YansWifiPhy> rx = CreateObject<YansWifiPhy> ();
  Ptr<ErrorRateModel> error = CreateObject<YansErrorRateModel> ();
  tx->SetErrorRateModel (error);
  rx->SetErrorRateModel (error);
  tx->SetChannel (channel);
  rx->SetChannel (channel);
  tx->SetMobility (posTx);
  rx->SetMobility (posRx);

  rx->SetReceiveOkCallback (MakeCallback (&PsrExperiment::Receive, this));

  for (uint32_t i = 0; i < m_input.nPackets; ++i)
    {
      Simulator::Schedule (Seconds (i), &PsrExperiment::Send, this);
    }
  m_tx = tx;
  Simulator::Run ();
  return m_output;
}


class CollisionExperiment
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
    uint8_t txPowerLevelA;
    uint8_t txPowerLevelB;
    uint32_t packetSizeA;
    uint32_t packetSizeB;
    uint32_t nPackets;
  };
  struct Output
  {
    uint32_t receivedA;
    uint32_t receivedB;
  };
  CollisionExperiment ();

  struct CollisionExperiment::Output Run (struct CollisionExperiment::Input input);
private:
  void SendA (void) const;
  void SendB (void) const;
  void Receive (Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble);
  Ptr<WifiPhy> m_txA;
  Ptr<WifiPhy> m_txB;
  uint32_t m_flowIdA;
  uint32_t m_flowIdB;
  struct Input m_input;
  struct Output m_output;
};

void 
CollisionExperiment::SendA (void) const
{
  Ptr<Packet> p = Create<Packet> (m_input.packetSizeA);
  p->AddTag (FlowIdTag (m_flowIdA));
  m_txA->SendPacket (p, WifiMode (m_input.txModeA), 
		     WIFI_PREAMBLE_SHORT, m_input.txPowerLevelA);
}

void 
CollisionExperiment::SendB (void) const
{
  Ptr<Packet> p = Create<Packet> (m_input.packetSizeB);
  p->AddTag (FlowIdTag (m_flowIdB));
  m_txB->SendPacket (p, WifiMode (m_input.txModeB), 
		     WIFI_PREAMBLE_SHORT, m_input.txPowerLevelB);
}

void 
CollisionExperiment::Receive (Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble)
{
  FlowIdTag tag;
  p->FindFirstMatchingTag (tag);
  if (tag.GetFlowId () == m_flowIdA)
    {
      m_output.receivedA++;
    }
  else if (tag.GetFlowId () == m_flowIdB)
    {
      m_output.receivedB++;
    }
}

CollisionExperiment::CollisionExperiment ()
{}
CollisionExperiment::Input::Input ()
  : interval (MicroSeconds (0)),
    xA (-5),
    xB (5),
    txModeA ("wifia-6mbs"),
    txModeB ("wifia-6mbs"),
    txPowerLevelA (0),
    txPowerLevelB (0),
    packetSizeA (2304),
    packetSizeB (2304),
    nPackets (400)
{}

struct CollisionExperiment::Output
CollisionExperiment::Run (struct CollisionExperiment::Input input)
{
  m_output.receivedA = 0;
  m_output.receivedB = 0;
  m_input = input;

  m_flowIdA = FlowIdTag::AllocateFlowId ();
  m_flowIdB = FlowIdTag::AllocateFlowId ();

  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  channel->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());
  Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();
  channel->SetPropagationLossModel (log);

  Ptr<MobilityModel> posTxA = CreateObject<ConstantPositionMobilityModel> ();
  posTxA->SetPosition (Vector (input.xA, 0.0, 0.0));
  Ptr<MobilityModel> posTxB = CreateObject<ConstantPositionMobilityModel> ();
  posTxB->SetPosition (Vector (input.xB, 0.0, 0.0));
  Ptr<MobilityModel> posRx = CreateObject<ConstantPositionMobilityModel> ();
  posRx->SetPosition (Vector (0, 0.0, 0.0));

  Ptr<YansWifiPhy> txA = CreateObject<YansWifiPhy> ();
  Ptr<YansWifiPhy> txB = CreateObject<YansWifiPhy> ();
  Ptr<YansWifiPhy> rx = CreateObject<YansWifiPhy> ();

  Ptr<ErrorRateModel> error = CreateObject<YansErrorRateModel> ();
  txA->SetErrorRateModel (error);
  txB->SetErrorRateModel (error);
  rx->SetErrorRateModel (error);
  txA->SetChannel (channel);
  txB->SetChannel (channel);
  rx->SetChannel (channel);
  txA->SetMobility (posTxA);
  txB->SetMobility (posTxB);
  rx->SetMobility (posRx);


  rx->SetReceiveOkCallback (MakeCallback (&CollisionExperiment::Receive, this));

  for (uint32_t i = 0; i < m_input.nPackets; ++i)
    {
      Simulator::Schedule (Seconds (i), &CollisionExperiment::SendA, this);
    }
  for (uint32_t i = 0; i < m_input.nPackets; ++i)
    {
      Simulator::Schedule (Seconds (i) + m_input.interval, &CollisionExperiment::SendB, this);
    }
  m_txA = txA;
  m_txB = txB;
  Simulator::Run ();
  return m_output;
}


static void PrintPsr (int argc, char *argv[])
{
  PsrExperiment experiment;
  struct PsrExperiment::Input input;

  CommandLine cmd;
  cmd.AddValue ("Distance", "The distance between two phys", input.distance);
  cmd.AddValue ("PacketSize", "The size of each packet sent", input.packetSize);
  cmd.AddValue ("TxMode", "The mode to use to send each packet", input.txMode);
  cmd.AddValue ("NPackets", "The number of packets to send", input.nPackets);
  cmd.AddValue ("TxPowerLevel", "The power level index to use to send each packet", input.txPowerLevel);
  cmd.Parse (argc, argv);

  struct PsrExperiment::Output output;
  output = experiment.Run (input);

  double psr = output.received;
  psr /= input.nPackets ;
  
  std::cout << psr << std::endl;
}

double CalcPsr (struct PsrExperiment::Output output, struct PsrExperiment::Input input)
{
  double psr = output.received;
  psr /= input.nPackets ;
  return psr;
}

static void PrintPsrVsDistance (int argc, char *argv[])
{
  struct PsrExperiment::Input input;
  CommandLine cmd;
  cmd.AddValue ("TxPowerLevel", "The power level index to use to send each packet", input.txPowerLevel);  
  cmd.AddValue ("TxMode", "The mode to use to send each packet", input.txMode);
  cmd.AddValue ("NPackets", "The number of packets to send", input.nPackets);
  cmd.AddValue ("PacketSize", "The size of each packet sent", input.packetSize);
  cmd.Parse (argc, argv);
  for (input.distance = 1.0; input.distance < 165; input.distance += 2.0)
    {
      std::cout << input.distance;
      PsrExperiment experiment;
      struct PsrExperiment::Output output;

      input.txMode = "wifia-6mbs";
      output = experiment.Run (input);
      std::cout << " " << CalcPsr (output, input);
    
      input.txMode = "wifia-9mbs";
      output = experiment.Run (input);
      std::cout << " " << CalcPsr (output, input);

      input.txMode = "wifia-12mbs";
      output = experiment.Run (input);
      std::cout << " " << CalcPsr (output, input);

      input.txMode = "wifia-18mbs";
      output = experiment.Run (input);
      std::cout << " " << CalcPsr (output, input);

      input.txMode = "wifia-24mbs";
      output = experiment.Run (input);
      std::cout << " " << CalcPsr (output, input);

      input.txMode = "wifia-36mbs";
      output = experiment.Run (input);
      std::cout << " " << CalcPsr (output, input);

      input.txMode = "wifia-48mbs";
      output = experiment.Run (input);
      std::cout << " " << CalcPsr (output, input);

      input.txMode = "wifia-54mbs";
      output = experiment.Run (input);
      std::cout << " " << CalcPsr (output, input);

      std::cout << std::endl;
    }
}

static void PrintSizeVsRange (int argc, char *argv[])
{
  double targetPsr = 0.05;
  struct PsrExperiment::Input input;
  CommandLine cmd;
  cmd.AddValue ("TxPowerLevel", "The power level index to use to send each packet", input.txPowerLevel);  
  cmd.AddValue ("TxMode", "The mode to use to send each packet", input.txMode);
  cmd.AddValue ("NPackets", "The number of packets to send", input.nPackets);
  cmd.AddValue ("TargetPsr", "The psr needed to assume that we are within range", targetPsr);
  cmd.Parse (argc, argv);
  for (input.packetSize = 10; input.packetSize < 3000; input.packetSize += 40)
    {
      double precision = 0.1;
      double low = 1.0;
      double high = 200.0;
      while (high - low > precision)
	{
	  double middle = low + (high - low) / 2;
	  struct PsrExperiment::Output output;
	  PsrExperiment experiment;
	  input.distance = middle;
	  output = experiment.Run (input);
	  double psr = CalcPsr (output, input);
	  if (psr >= targetPsr)
	    {
	      low = middle;
	    }
	  else
	    {
	      high = middle;
	    }
	}
      std::cout << input.packetSize << " " << input.distance << std::endl;
    }
}

static void PrintPsrVsCollisionInterval (int argc, char *argv[])
{
  CollisionExperiment::Input input;
  input.nPackets = 100;
  CommandLine cmd;
  cmd.AddValue ("NPackets", "The number of packets to send for each transmitter", input.nPackets);
  cmd.AddValue ("xA", "the position of transmitter A", input.xA);
  cmd.AddValue ("xB", "the position of transmitter B", input.xB);
  for (uint32_t i = 0; i < 100; i += 1)
    {
      CollisionExperiment experiment;
      CollisionExperiment::Output output;
      input.interval = MicroSeconds (i);
      output = experiment.Run (input);
      double perA = (output.receivedA+0.0) / (input.nPackets+0.0);
      double perB = (output.receivedB+0.0) / (input.nPackets+0.0);
      std::cout << i << " " << perA << " " << perB << std::endl;
    }
  for (uint32_t i = 100; i < 4000; i += 50)
    {
      CollisionExperiment experiment;
      CollisionExperiment::Output output;
      input.interval = MicroSeconds (i);
      output = experiment.Run (input);
      double perA = (output.receivedA+0.0) / (input.nPackets+0.0);
      double perB = (output.receivedB+0.0) / (input.nPackets+0.0);
      std::cout << i << " " << perA << " " << perB << std::endl;
    }
}



int main (int argc, char *argv[])
{
  if (argc <= 1)
    {
      std::cout << "Available experiments: "
		<< "Psr "
		<< "SizeVsRange "
		<< "PsrVsDistance "
		<< "PsrVsCollisionInterval "
		<< std::endl;
      return -1;
    }
  std::string type = argv[1];
  argc--;
  argv[1] = argv[0];
  argv++;
  if (type == "Psr")
    {
      PrintPsr (argc, argv);
    }
  else if (type == "SizeVsRange")
    {
      PrintSizeVsRange (argc, argv);
    }
  else if (type == "PsrVsDistance")
    {
      PrintPsrVsDistance (argc, argv);
    }
  else if (type == "PsrVsCollisionInterval")
    {
      PrintPsrVsCollisionInterval (argc, argv);
    }

  return 0;
}
