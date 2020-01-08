#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traci-applications-module.h"
#include "ns3/network-module.h"
#include "ns3/traci-module.h"
#include "ns3/wave-module.h"
#include "ns3/ocb-wifi-mac.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/netanim-module.h"
#include <functional>
#include <stdlib.h>

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("ns3-sumo-coupling-simple");

int
main (int argc, char *argv[])
{
  /*** 0. Logging Options ***/
  bool verbose = true;

  CommandLine cmd;
  cmd.Parse (argc, argv);
  if (verbose)
    {
      LogComponentEnable ("TraciClient", LOG_LEVEL_INFO);
      LogComponentEnable ("TrafficControlApplication", LOG_LEVEL_INFO);
    }

  /*** 1. Create node pool and counter; large enough to cover all sumo vehicles ***/
  ns3::Time simulationTime (ns3::Seconds(500));
  NodeContainer nodePool;
  nodePool.Create (20);
  uint32_t nodeCounter (0);

  /*** 2. Create and setup channel ***/
  std::string phyMode ("OfdmRate6MbpsBW10MHz");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.Set ("TxPowerStart", DoubleValue (20));
  wifiPhy.Set ("TxPowerEnd", DoubleValue (20));
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  Ptr<YansWifiChannel> channel = wifiChannel.Create ();
  wifiPhy.SetChannel (channel);

  /*** 3. Create and setup MAC ***/
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
  NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default ();
  Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default ();
  wifi80211p.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue (phyMode), "ControlMode", StringValue (phyMode));
  NetDeviceContainer netDevices = wifi80211p.Install (wifiPhy, wifi80211pMac, nodePool);

  /*** 4. Add Internet layers stack and routing ***/
  InternetStackHelper stack;
  stack.Install (nodePool);

  /*** 5. Assign IP address to each device ***/
  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer ipv4Interfaces;
  ipv4Interfaces = address.Assign (netDevices);

  /*** 6. Setup Mobility and position node pool ***/
  MobilityHelper mobility;
  Ptr<UniformDiscPositionAllocator> positionAlloc = CreateObject<UniformDiscPositionAllocator> ();
  positionAlloc->SetX (320.0);
  positionAlloc->SetY (320.0);
  positionAlloc->SetRho (25.0);
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodePool);

  /*** 7. Setup Traci and start SUMO ***/
  Ptr<TraciClient> sumoClient = CreateObject<TraciClient> ();
  sumoClient->SetAttribute ("SumoConfigPath", StringValue ("src/traci/examples/circle-simple/circle.sumo.cfg"));
  sumoClient->SetAttribute ("SumoBinaryPath", StringValue (""));    // use system installation of sumo
  sumoClient->SetAttribute ("SynchInterval", TimeValue (Seconds (0.1)));
  sumoClient->SetAttribute ("StartTime", TimeValue (Seconds (0.0)));
  sumoClient->SetAttribute ("SumoGUI", BooleanValue (false));
  sumoClient->SetAttribute ("SumoPort", UintegerValue (3400));
  sumoClient->SetAttribute ("PenetrationRate", DoubleValue (1.0));  // portion of vehicles equipped with wifi
  sumoClient->SetAttribute ("SumoLogFile", BooleanValue (true));
  sumoClient->SetAttribute ("SumoStepLog", BooleanValue (false));
  sumoClient->SetAttribute ("SumoSeed", IntegerValue (10));
  sumoClient->SetAttribute ("SumoAdditionalCmdOptions", StringValue ("--verbose true"));
  sumoClient->SetAttribute ("SumoWaitForSocket", TimeValue (Seconds (1.0)));

  /*** 8. Create and Setup Applications for the RSU node and set position ***/
  RsuSpeedControlHelper rsuSpeedControlHelper (9); // Port #9
  rsuSpeedControlHelper.SetAttribute ("Velocity", UintegerValue (30));           // initial velocity value which is sent to vehicles
  rsuSpeedControlHelper.SetAttribute ("Interval", TimeValue (Seconds (7.0)));    // packet interval
  rsuSpeedControlHelper.SetAttribute ("Client", (PointerValue) (sumoClient));    // pass TraciClient object for accessing sumo in application

  ApplicationContainer rsuSpeedControlApps = rsuSpeedControlHelper.Install (nodePool.Get (0));
  rsuSpeedControlApps.Start (Seconds (1.0));
  rsuSpeedControlApps.Stop (simulationTime);

  Ptr<MobilityModel> mobilityRsuNode = nodePool.Get (0)->GetObject<MobilityModel> ();
  mobilityRsuNode->SetPosition (Vector (70.7, 70.7, 3.0)); // set RSU to fixed position
  nodeCounter++;    // one node (RSU) consumed from "node pool"

  /*** 9. Setup interface and application for dynamic nodes ***/
  VehicleSpeedControlHelper vehicleSpeedControlHelper (9);
  vehicleSpeedControlHelper.SetAttribute ("Client", (PointerValue) sumoClient); // pass TraciClient object for accessing sumo in application

  // callback function for node creation
  std::function<Ptr<Node> ()> setupNewWifiNode = [&] () -> Ptr<Node>
    {
      if (nodeCounter >= nodePool.GetN())
        NS_FATAL_ERROR("Node Pool empty!: " << nodeCounter << " nodes created.");

      // don't create and install the protocol stack of the node at simulation time -> take from "node pool"
      Ptr<Node> includedNode = nodePool.Get(nodeCounter);
      ++nodeCounter;// increment counter for next node

      // Install Application
      ApplicationContainer vehicleSpeedControlApps = vehicleSpeedControlHelper.Install (includedNode);
      vehicleSpeedControlApps.Start (Seconds (0.0));
      vehicleSpeedControlApps.Stop (simulationTime);

      return includedNode;
    };

  // callback function for node shutdown
  std::function<void (Ptr<Node>)> shutdownWifiNode = [] (Ptr<Node> exNode)
    {
      // stop all applications
      Ptr<VehicleSpeedControl> vehicleSpeedControl = exNode->GetApplication(0)->GetObject<VehicleSpeedControl>();
      if(vehicleSpeedControl)
        vehicleSpeedControl->StopApplicationNow();

      // set position outside communication range
      Ptr<ConstantPositionMobilityModel> mob = exNode->GetObject<ConstantPositionMobilityModel>();
      mob->SetPosition(Vector(-100.0+(rand()%25),320.0+(rand()%25),250.0));// rand() for visualization purposes

      // NOTE: further actions could be required for a save shut down!
    };

  // start traci client with given function pointers
  sumoClient->SumoSetup (setupNewWifiNode, shutdownWifiNode);

  /*** 10. Setup and Start Simulation + Animation ***/
  AnimationInterface anim ("src/traci-applications/examples/ns3-sumo-coupling.xml"); // Mandatory
  Simulator::Stop (simulationTime);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
