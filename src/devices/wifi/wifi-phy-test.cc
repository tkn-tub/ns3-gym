#include "wifi-phy.h"
#include "wifi-channel.h"
#include "propagation-loss-model.h"
#include "propagation-delay-model.h"
#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/static-mobility-model.h"
#include "ns3/vector.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/command-line.h"

using namespace ns3;

class PsrExperiment
{
public:
  struct Input
  {
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

  static struct PsrExperiment::Input GetDefaultInput (void);

  struct PsrExperiment::Output Run (struct PsrExperiment::Input input);

private:
  void Send (void);
  typedef Callback<void,Ptr<Packet>, double, WifiMode, enum WifiPreamble> SyncOkCallback;
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
struct PsrExperiment::Input 
PsrExperiment::GetDefaultInput (void)
{
  struct PsrExperiment::Input  input;
  input.distance = 5.0;
  input.packetSize = 2304;
  input.nPackets = 400;
  input.txPowerLevel = 0;
  input.txMode = "wifia-6mbs";
  return input;
}

struct PsrExperiment::Output
PsrExperiment::Run (struct PsrExperiment::Input input)
{
  m_output.received = 0;
  m_input = input;

  Ptr<MobilityModel> posTx = CreateObject<StaticMobilityModel> ();
  posTx->SetPosition (Vector (0.0, 0.0, 0.0));
  Ptr<MobilityModel> posRx = CreateObject<StaticMobilityModel> ();
  posRx->SetPosition (Vector (m_input.distance, 0.0, 0.0));

  Ptr<WifiPhy> tx = CreateObject<WifiPhy> ();
  Ptr<WifiPhy> rx = CreateObject<WifiPhy> ();
  rx->SetReceiveOkCallback (MakeCallback (&PsrExperiment::Receive, this));

  Ptr<WifiChannel> channel = CreateObject<WifiChannel> ();
  channel->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());
  Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();
  log->SetReferenceModel (CreateObject<FriisPropagationLossModel> ());
  channel->SetPropagationLossModel (log);

  channel->Add (0, tx, posTx);
  channel->Add (0, rx, posRx);
  tx->SetChannel (channel);
  rx->SetChannel (channel);

  for (uint32_t i = 0; i < m_input.nPackets; ++i)
    {
      Simulator::Schedule (Seconds (i), &PsrExperiment::Send, this);
    }
  m_tx = tx;
  Simulator::Run ();
  return m_output;
}

static void PrintPsr (int argc, char *argv[])
{
  PsrExperiment experiment;
  struct PsrExperiment::Input input = experiment.GetDefaultInput ();

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
  struct PsrExperiment::Input input = PsrExperiment::GetDefaultInput ();
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
  double minPsr = 0.05;
  struct PsrExperiment::Input input = PsrExperiment::GetDefaultInput ();
  CommandLine cmd;
  cmd.AddValue ("TxPowerLevel", "The power level index to use to send each packet", input.txPowerLevel);  
  cmd.AddValue ("TxMode", "The mode to use to send each packet", input.txMode);
  cmd.AddValue ("NPackets", "The number of packets to send", input.nPackets);
  cmd.AddValue ("LowPsr", "The minimum psr needed to assume that we are within range", minPsr);
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
	  if (psr >= minPsr)
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



int main (int argc, char *argv[])
{
  if (argc <= 1)
    {
      std::cout << "Available experiments: "
		<< "Psr "
		<< "SizeVsRange "
		<< "PsrVsDistance "
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

  return 0;
}
