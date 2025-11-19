// VANET AODV with Neural Network Link Quality Estimation using ns3-gym

#include "vanet-link-env.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/aodv-module.h"
#include "ns3/wifi-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/opengym-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"
#include <cmath>
#include <cstdlib>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("VanetAodvGym");

// Global pointer to environment
Ptr<VanetLinkEnv> g_gymEnv;
Ptr<OpenGymInterface> g_openGym;

// Helper function to extract node ID from context string
uint32_t
ExtractNodeId(const std::string& context)
{
    size_t pos = context.find("/NodeList/");
    if (pos != std::string::npos)
    {
        pos += 10;  // Length of "/NodeList/"
        size_t end = context.find("/", pos);
        if (end != std::string::npos)
        {
            std::string nodeIdStr = context.substr(pos, end - pos);
            try {
                return std::stoul(nodeIdStr);
            } catch (...) {
                return UINT32_MAX;  // Invalid
            }
        }
    }
    return UINT32_MAX;  // Invalid
}

// Callback for packet transmission - tracks when packets are sent
void
PhyTxBeginTrace(std::string context, Ptr<const Packet> packet, double txPowerW)
{
    if (!g_gymEnv)
    {
        return;
    }

    uint32_t txNodeId = ExtractNodeId(context);
    if (txNodeId != UINT32_MAX)
    {
        // Notify that this node transmitted a packet
        // This helps track the total number of transmitted packets per link
        g_gymEnv->NotifyPacketTransmitted(txNodeId);
    }
}

// Callback for packet reception - tracks successful receptions with RSSI/SNR
void
MonitorSnifferRxTrace(std::string context, Ptr<const Packet> packet, uint16_t channelFreqMhz,
                      WifiTxVector txVector, MpduInfo aMpdu, SignalNoiseDbm signalNoise, uint16_t staId)
{
    if (!g_gymEnv)
    {
        return;
    }

    // Get the receiving node ID from context
    uint32_t rxNodeId = ExtractNodeId(context);
    if (rxNodeId != UINT32_MAX)
    {
        // Extract actual RSSI and calculate SNR from signal/noise
        double rssi = signalNoise.signal;  // Actual received signal strength in dBm
        double noise = signalNoise.noise;  // Noise floor in dBm
        double snr = rssi - noise;         // SNR = Signal - Noise (in dB)

        // Notify successful reception with real RSSI and SNR values
        g_gymEnv->NotifyPacketReceived(rxNodeId, rssi, snr);
    }
}

// Callback for packet drop - tracks PHY layer drops
void
PhyTxDropTrace(std::string context, Ptr<const Packet> packet)
{
    if (!g_gymEnv)
    {
        return;
    }

    uint32_t nodeId = ExtractNodeId(context);
    if (nodeId != UINT32_MAX)
    {
        g_gymEnv->NotifyPacketLost(nodeId);
    }
}

// Periodic state update for OpenGym
void
ScheduleNextStateRead(double envStepTime, Ptr<OpenGymInterface> openGym)
{
    Simulator::Schedule(Seconds(envStepTime), &ScheduleNextStateRead, envStepTime, openGym);
    openGym->NotifyCurrentState();
}

int
main(int argc, char* argv[])
{
    // Simulation Parameters
    uint32_t numVehicles = 8;
    uint32_t numRSUs = 4;
    uint32_t numCSMAServers = 1;
    double simTime = 100.0;
    double envStepTime = 0.1;
    uint32_t openGymPort = 5555;

    // Mobility parameters
    double vehicleSpeed = 20.0;
    double minVehicleSpeed = 10.0;
    double nodePause = 0.0;
    double areaWidth = 300.0;
    double areaHeight = 300.0;

    // Traffic parameters
    uint32_t packetSize = 256;
    double packetInterval = 5.0;

    bool verbose = false;
    bool enableNetAnim = true;
    bool enableMobility = true;
    std::string sumoTraceFile = "";

    // Command line arguments
    CommandLine cmd;
    cmd.AddValue("numVehicles", "Number of vehicles", numVehicles);
    cmd.AddValue("numRSUs", "Number of Road Side Units", numRSUs);
    cmd.AddValue("simTime", "Simulation time (seconds)", simTime);
    cmd.AddValue("envStepTime", "OpenGym environment step time", envStepTime);
    cmd.AddValue("openGymPort", "Port number for OpenGym", openGymPort);
    cmd.AddValue("vehicleSpeed", "Maximum vehicle speed (m/s)", vehicleSpeed);
    cmd.AddValue("verbose", "Enable verbose logging", verbose);
    cmd.AddValue("enableNetAnim", "Enable NetAnim visualization", enableNetAnim);
    cmd.AddValue("enableMobility", "Enable vehicle mobility", enableMobility);
    cmd.AddValue("sumoTraceFile", "SUMO trace file path", sumoTraceFile);
    cmd.Parse(argc, argv);
    srand(42);

    if (verbose)
    {
        LogComponentEnable("VanetAodvGym", LOG_LEVEL_INFO);
        LogComponentEnable("VanetLinkEnv", LOG_LEVEL_INFO);
        LogComponentEnable("OpenGymInterface", LOG_LEVEL_INFO);
    }

    NS_LOG_UNCOND("==============================================");
    NS_LOG_UNCOND(" COMPREHENSIVE VANET AODV NN SIMULATION");
    NS_LOG_UNCOND("==============================================");
    NS_LOG_UNCOND("Vehicles:        " << numVehicles);
    NS_LOG_UNCOND("RSUs:            " << numRSUs);
    NS_LOG_UNCOND("Total Nodes:     " << (numVehicles + numRSUs));
    NS_LOG_UNCOND("Simulation Time: " << simTime << "s");
    NS_LOG_UNCOND("OpenGym Port:    " << openGymPort);
    NS_LOG_UNCOND("Max Speed:       " << vehicleSpeed << " m/s");
    NS_LOG_UNCOND("NetAnim:         " << (enableNetAnim ? "Enabled" : "Disabled"));
    NS_LOG_UNCOND("Mobility:        " << (enableMobility ? "Enabled" : "Static"));
    if (!sumoTraceFile.empty())
    {
        NS_LOG_UNCOND("SUMO Trace:      " << sumoTraceFile);
    }
    NS_LOG_UNCOND("==============================================");

    // Create nodes
    NodeContainer vehicleNodes;
    vehicleNodes.Create(numVehicles);
    NodeContainer rsuNodes;
    rsuNodes.Create(numRSUs);
    NodeContainer csmaServerNodes;
    csmaServerNodes.Create(numCSMAServers);

    NodeContainer wirelessNodes = NodeContainer(vehicleNodes, rsuNodes);
    NodeContainer allNodes = NodeContainer(wirelessNodes, csmaServerNodes);

    NS_LOG_UNCOND("Created " << numVehicles << " vehicles, " << numRSUs << " RSUs, "
                  << numCSMAServers << " CSMA servers");

    // WiFi Configuration

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a);
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                  "DataMode", StringValue("OfdmRate6Mbps"),
                                  "ControlMode", StringValue("OfdmRate6Mbps"));

    YansWifiPhyHelper wifiPhy;
    wifiPhy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
    wifiPhy.Set("TxPowerStart", DoubleValue(23.0));
    wifiPhy.Set("TxPowerEnd", DoubleValue(23.0));
    wifiPhy.Set("RxGain", DoubleValue(0.0));
    wifiPhy.Set("RxNoiseFigure", DoubleValue(7.0));
    wifiPhy.Set("CcaEdThreshold", DoubleValue(-82.0));

    Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue("2200"));
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("2200"));

    YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel",
                                    "Frequency", DoubleValue(5.9e9),
                                    "SystemLoss", DoubleValue(1.0));
    wifiPhy.SetChannel(wifiChannel.Create());

    WifiMacHelper wifiMac;
    wifiMac.SetType("ns3::AdhocWifiMac");

    Config::SetDefault("ns3::WifiMacQueue::MaxSize", StringValue("50p"));
    Config::SetDefault("ns3::WifiPhy::ChannelSwitchDelay", TimeValue(MicroSeconds(250)));
    Config::SetDefault("ns3::ArpCache::AliveTimeout", TimeValue(Seconds(120)));

    // Install WiFi
    NS_LOG_UNCOND("Installing WiFi...");
    NetDeviceContainer vehicleDevices = wifi.Install(wifiPhy, wifiMac, vehicleNodes);
    NetDeviceContainer rsuDevices = wifi.Install(wifiPhy, wifiMac, rsuNodes);
    NetDeviceContainer wifiDevices = NetDeviceContainer(vehicleDevices, rsuDevices);

    // Install CSMA
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));
    NetDeviceContainer csmaDevices = csma.Install(csmaServerNodes);

    // Mobility Models
    MobilityHelper mobilityVehicles, mobilityRSUs, mobilityCSMA;

    // Vehicle mobility
    if (enableMobility)
    {
        ObjectFactory pos;
        pos.SetTypeId("ns3::RandomRectanglePositionAllocator");
        pos.Set("X", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=" + std::to_string(areaWidth) + "]"));
        pos.Set("Y", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=" + std::to_string(areaHeight) + "]"));
        Ptr<PositionAllocator> vehiclePositionAlloc = pos.Create()->GetObject<PositionAllocator>();

        std::stringstream ssSpeed, ssPause;
        ssSpeed << "ns3::UniformRandomVariable[Min=" << minVehicleSpeed << "|Max=" << vehicleSpeed << "]";
        ssPause << "ns3::ConstantRandomVariable[Constant=" << nodePause << "]";

        mobilityVehicles.SetMobilityModel("ns3::RandomWaypointMobilityModel",
                                          "Speed", StringValue(ssSpeed.str()),
                                          "Pause", StringValue(ssPause.str()),
                                          "PositionAllocator", PointerValue(vehiclePositionAlloc));
        mobilityVehicles.SetPositionAllocator(vehiclePositionAlloc);
        mobilityVehicles.Install(vehicleNodes);
    }
    else
    {
        Ptr<ListPositionAllocator> vehiclePositionAlloc = CreateObject<ListPositionAllocator>();
        for (uint32_t i = 0; i < numVehicles; ++i)
            vehiclePositionAlloc->Add(Vector(100.0 + i * 150.0, 100.0 + (i % 2) * 100.0, 1.5));
        mobilityVehicles.SetPositionAllocator(vehiclePositionAlloc);
        mobilityVehicles.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobilityVehicles.Install(vehicleNodes);
    }

    // RSU mobility
    Ptr<ListPositionAllocator> rsuPositions = CreateObject<ListPositionAllocator>();
    double centerX = areaWidth / 2.0, centerY = areaHeight / 2.0, spacing = 75.0;
    rsuPositions->Add(Vector(centerX - spacing, centerY - spacing, 10.0));
    rsuPositions->Add(Vector(centerX + spacing, centerY - spacing, 10.0));
    rsuPositions->Add(Vector(centerX - spacing, centerY + spacing, 10.0));
    rsuPositions->Add(Vector(centerX + spacing, centerY + spacing, 10.0));
    mobilityRSUs.SetPositionAllocator(rsuPositions);
    mobilityRSUs.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobilityRSUs.Install(rsuNodes);

    // CSMA mobility
    Ptr<ListPositionAllocator> csmaPositions = CreateObject<ListPositionAllocator>();
    csmaPositions->Add(Vector(areaWidth / 2.0, areaHeight / 2.0, 0.0));
    mobilityCSMA.SetPositionAllocator(csmaPositions);
    mobilityCSMA.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobilityCSMA.Install(csmaServerNodes);

    // AODV routing
    AodvHelper aodv;
    aodv.Set("EnableHello", BooleanValue(false));
    aodv.Set("ActiveRouteTimeout", TimeValue(Seconds(100.0)));
    aodv.Set("AllowedHelloLoss", UintegerValue(10));
    aodv.Set("RreqRetries", UintegerValue(2));
    aodv.Set("NetDiameter", UintegerValue(10));

    // Internet stack
    InternetStackHelper internetWireless;
    internetWireless.SetRoutingHelper(aodv);
    internetWireless.Install(wirelessNodes);
    InternetStackHelper internetCSMA;
    internetCSMA.Install(csmaServerNodes);

    // IP addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer vehicleInterfaces = ipv4.Assign(vehicleDevices);
    ipv4.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer rsuInterfaces = ipv4.Assign(rsuDevices);
    ipv4.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces = ipv4.Assign(csmaDevices);

    // Applications
    uint16_t port = 9;

    // RSU servers
    for (uint32_t i = 0; i < numRSUs; ++i)
    {
        UdpEchoServerHelper server(port + i);
        ApplicationContainer serverApp = server.Install(rsuNodes.Get(i));
        serverApp.Start(Seconds(1.0));
        serverApp.Stop(Seconds(simTime));
    }

    // V2V flows
    uint32_t numV2VPairs = std::min(3u, numVehicles / 2);
    for (uint32_t i = 0; i < numV2VPairs; ++i)
    {
        UdpEchoClientHelper client(vehicleInterfaces.GetAddress(i), port + i);
        client.SetAttribute("MaxPackets", UintegerValue(UINT32_MAX));
        client.SetAttribute("Interval", TimeValue(Seconds(packetInterval)));
        client.SetAttribute("PacketSize", UintegerValue(packetSize));
        ApplicationContainer clientApp = client.Install(vehicleNodes.Get(numVehicles - 1 - i));
        clientApp.Start(Seconds(10.0 + i * 2.0));
        clientApp.Stop(Seconds(simTime));
    }

    // V2I flows
    uint32_t numV2IFlows = std::min(numVehicles, numRSUs * 2);
    for (uint32_t i = 0; i < numV2IFlows; ++i)
    {
        uint32_t rsuIdx = i % numRSUs;
        UdpEchoClientHelper client(rsuInterfaces.GetAddress(rsuIdx), port + 10 + rsuIdx);
        client.SetAttribute("MaxPackets", UintegerValue(UINT32_MAX));
        client.SetAttribute("Interval", TimeValue(Seconds(packetInterval * 1.5)));
        client.SetAttribute("PacketSize", UintegerValue(packetSize));
        ApplicationContainer clientApp = client.Install(vehicleNodes.Get(i));
        clientApp.Start(Seconds(20.0 + i * 2.0));
        clientApp.Stop(Seconds(simTime));
    }

    // OpenGym interface
    NS_LOG_UNCOND("Creating OpenGym interface...");
    g_openGym = CreateObject<OpenGymInterface>(openGymPort);
    NS_LOG_UNCOND("OpenGym interface created successfully");

    // Then create the environment with vehicle nodes only (monitoring vehicle links)
    NS_LOG_UNCOND("Creating OpenGym environment (VanetLinkEnv for vehicles)...");
    g_gymEnv = CreateObject<VanetLinkEnv>(vehicleNodes);
    NS_LOG_UNCOND("VanetLinkEnv created successfully (monitoring " << numVehicles << " vehicles)");

    // Link them together
    g_gymEnv->SetOpenGymInterface(g_openGym);
    
    // Notify that we're ready
    NS_LOG_UNCOND("Starting simulation...");
    NS_LOG_UNCOND("Waiting for Python agent to connect on port " << openGymPort << "...");

    // Connect PHY layer traces for link quality monitoring
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin",
                    MakeCallback(&PhyTxBeginTrace));
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/MonitorSnifferRx",
                    MakeCallback(&MonitorSnifferRxTrace));
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxDrop",
                    MakeCallback(&PhyTxDropTrace));

    // Schedule periodic state updates for OpenGym
    // Start after a small delay to let Python connect
    Simulator::Schedule(Seconds(0.5), &ScheduleNextStateRead, envStepTime, g_openGym);

    // ========================================================================
    // Flow Monitor for Statistics
    // ========================================================================
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // NetAnim
    AnimationInterface* anim = nullptr;
    if (enableNetAnim)
    {
        anim = new AnimationInterface("vanet-aodv-gym.xml");
        anim->EnablePacketMetadata(false);

        for (uint32_t i = 0; i < numVehicles; ++i)
        {
            anim->UpdateNodeDescription(vehicleNodes.Get(i), "V-" + std::to_string(i));
            anim->UpdateNodeColor(vehicleNodes.Get(i), 0, 255, 0);
            anim->UpdateNodeSize(vehicleNodes.Get(i)->GetId(), 5.0, 5.0);
        }
        for (uint32_t i = 0; i < numRSUs; ++i)
        {
            anim->UpdateNodeDescription(rsuNodes.Get(i), "RSU-" + std::to_string(i));
            anim->UpdateNodeColor(rsuNodes.Get(i), 255, 0, 0);
            anim->UpdateNodeSize(rsuNodes.Get(i)->GetId(), 8.0, 8.0);
        }
        for (uint32_t i = 0; i < numCSMAServers; ++i)
        {
            anim->UpdateNodeDescription(csmaServerNodes.Get(i), "S-" + std::to_string(i));
            anim->UpdateNodeColor(csmaServerNodes.Get(i), 0, 0, 255);
            anim->UpdateNodeSize(csmaServerNodes.Get(i)->GetId(), 10.0, 10.0);
        }
    }

    // Run simulation
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();

    // Flow statistics
    NS_LOG_UNCOND("\n======================================");
    NS_LOG_UNCOND("        Flow Statistics (First 4)");
    NS_LOG_UNCOND("======================================");
    
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

    double totalTxPackets = 0;
    double totalRxPackets = 0;
    double totalThroughput = 0;

    uint32_t flowCount = 0;
    for (auto& flow : stats)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(flow.first);

        // Only process first 4 flows for statistics
        if (flowCount < 4)
        {
            totalTxPackets += flow.second.txPackets;
            totalRxPackets += flow.second.rxPackets;

            double throughput = flow.second.rxBytes * 8.0 / simTime / 1024.0;
            totalThroughput += throughput;

            NS_LOG_UNCOND("Flow " << flow.first
                         << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")");
            NS_LOG_UNCOND("  Tx Packets:   " << flow.second.txPackets);
            NS_LOG_UNCOND("  Rx Packets:   " << flow.second.rxPackets);

            if (flow.second.txPackets > 0)
            {
                double pdr = (flow.second.rxPackets * 100.0) / flow.second.txPackets;
                NS_LOG_UNCOND("  PDR:          " << pdr << " %");
            }

            NS_LOG_UNCOND("  Throughput:   " << throughput << " kbps");
            NS_LOG_UNCOND("");
        }
        flowCount++;
    }

    if (stats.size() > 4)
    {
        NS_LOG_UNCOND("... (" << (stats.size() - 4) << " more flows not shown)");
        NS_LOG_UNCOND("");
    }

    NS_LOG_UNCOND("======================================");
    NS_LOG_UNCOND("Overall Statistics:");
    NS_LOG_UNCOND("  Total Tx:     " << totalTxPackets << " packets");
    NS_LOG_UNCOND("  Total Rx:     " << totalRxPackets << " packets");
    
    if (totalTxPackets > 0)
    {
        double overallPDR = (totalRxPackets * 100.0) / totalTxPackets;
        NS_LOG_UNCOND("  Overall PDR:  " << overallPDR << " %");
    }
    
    NS_LOG_UNCOND("  Total Throughput: " << totalThroughput << " kbps");
    NS_LOG_UNCOND("======================================");

    // Notify simulation end
    g_openGym->NotifySimulationEnd();

    // Cleanup
    Simulator::Destroy();

    if (anim)
    {
        delete anim;
        NS_LOG_UNCOND("NetAnim file written successfully");
    }

    NS_LOG_UNCOND("\n==============================================");
    NS_LOG_UNCOND("Simulation completed successfully!");
    NS_LOG_UNCOND("==============================================");

    return 0;
}