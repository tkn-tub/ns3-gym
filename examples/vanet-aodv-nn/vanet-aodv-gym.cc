/*
 * VANET AODV with Neural Network Link Quality Estimation using ns3-gym
 *
 * This simulation implements:
 * - VANET with AODV routing
 * - Real-time link quality monitoring
 * - ns3-gym interface for ML-based route selection
 * - Dataset generation for link quality classification
 */

#include "vanet-link-env.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/aodv-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/opengym-module.h"
#include "ns3/flow-monitor-module.h"
#include <cmath>

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
PhyRxOkTrace(std::string context, Ptr<const Packet> packet, double snr, WifiMode mode, WifiPreamble preamble)
{
    if (!g_gymEnv)
    {
        return;
    }

    // Get the receiving node ID from context
    uint32_t rxNodeId = ExtractNodeId(context);
    if (rxNodeId != UINT32_MAX)
    {
        // Estimate RSSI from SNR (simplified: RSSI ≈ -90 + SNR)
        // For 802.11a: typical noise floor is around -90 to -95 dBm
        double rssi = -90.0 + snr;

        // Notify successful reception
        // Note: We can't easily determine the sender from PHY traces alone
        // but the environment will track this based on recent transmissions
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
    // Simulation parameters
    uint32_t numNodes = 20;
    double simTime = 100.0;
    double envStepTime = 0.1;  // OpenGym step interval
    uint32_t openGymPort = 5555;
    double nodeSpeed = 20.0;   // m/s (~72 km/h)
    uint32_t packetSize = 1024;
    double packetInterval = 0.1;
    bool verbose = false;

    // Command line arguments
    CommandLine cmd;
    cmd.AddValue("numNodes", "Number of vehicles", numNodes);
    cmd.AddValue("simTime", "Simulation time (seconds)", simTime);
    cmd.AddValue("envStepTime", "OpenGym environment step time (seconds)", envStepTime);
    cmd.AddValue("openGymPort", "Port number for OpenGym", openGymPort);
    cmd.AddValue("nodeSpeed", "Maximum node speed (m/s)", nodeSpeed);
    cmd.AddValue("verbose", "Enable verbose logging", verbose);
    cmd.Parse(argc, argv);

    // Enable logging if verbose
    if (verbose)
    {
        LogComponentEnable("VanetAodvGym", LOG_LEVEL_INFO);
        LogComponentEnable("VanetLinkEnv", LOG_LEVEL_INFO);
        LogComponentEnable("OpenGymInterface", LOG_LEVEL_INFO);
    }

    NS_LOG_UNCOND("======================================");
    NS_LOG_UNCOND("VANET AODV Neural Network Simulation");
    NS_LOG_UNCOND("======================================");
    NS_LOG_UNCOND("Nodes: " << numNodes);
    NS_LOG_UNCOND("Simulation Time: " << simTime << "s");
    NS_LOG_UNCOND("OpenGym Port: " << openGymPort);
    NS_LOG_UNCOND("Max Speed: " << nodeSpeed << " m/s");
    NS_LOG_UNCOND("======================================");

    // Create nodes
    NodeContainer nodes;
    nodes.Create(numNodes);
    NS_LOG_UNCOND("Nodes created: " << numNodes);

    // ========================================================================
    // WiFi Configuration for VANET
    // ========================================================================
    // Using 802.11a (5 GHz) - proven stable configuration
    // Provides reliable packet delivery for link quality estimation
    // ========================================================================

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a);

    // Use ConstantRateWifiManager with standard 802.11a rate
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                  "DataMode", StringValue("OfdmRate6Mbps"),
                                  "ControlMode", StringValue("OfdmRate6Mbps"));

    // Physical layer configuration optimized for VANET scenarios
    YansWifiPhyHelper wifiPhy;

    // Transmission power: 23 dBm (200 mW) for extended range in vehicular environment
    // This gives approximately 300-500m range depending on conditions
    wifiPhy.Set("TxPowerStart", DoubleValue(23.0));
    wifiPhy.Set("TxPowerEnd", DoubleValue(23.0));

    // Receiver sensitivity and gain configuration
    wifiPhy.Set("RxGain", DoubleValue(0.0));           // No additional RX gain
    wifiPhy.Set("RxNoiseFigure", DoubleValue(7.0));    // Typical noise figure
    wifiPhy.Set("CcaEdThreshold", DoubleValue(-82.0)); // Carrier sense threshold

    // Channel configuration with VANET-appropriate propagation models
    YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");

    // Use RangePropagationLossModel for predictable behavior + LogDistance for realism
    // This combination ensures packets are delivered within range while modeling attenuation
    wifiChannel.AddPropagationLoss("ns3::RangePropagationLossModel",
                                    "MaxRange", DoubleValue(250.0));  // 250m communication range

    wifiChannel.AddPropagationLoss("ns3::LogDistancePropagationLossModel",
                                    "Exponent", DoubleValue(2.7),           // Urban environment
                                    "ReferenceDistance", DoubleValue(1.0),
                                    "ReferenceLoss", DoubleValue(46.6777)); // Path loss at 1m

    wifiPhy.SetChannel(wifiChannel.Create());

    // MAC layer configuration for ad-hoc networking (VANET)
    // AdhocWifiMac provides infrastructure-less communication suitable for V2V
    WifiMacHelper wifiMac;
    wifiMac.SetType("ns3::AdhocWifiMac");

    // Install WiFi devices
    NS_LOG_UNCOND("Installing WiFi devices...");
    NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, nodes);
    NS_LOG_UNCOND("WiFi devices installed");

    // Mobility model for VANET
    // Use smaller area (500x500m) to ensure nodes stay within communication range
    // 802.11p with 20 dBm has ~300-400m range, so 500x500m ensures connectivity
    MobilityHelper mobility;

    // Position allocator - vehicles start in random positions within 500m x 500m
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();

    // Distribute nodes in a grid-like pattern initially to ensure connectivity
    // Reduced spacing to 100m to ensure all nodes stay within 250m communication range
    double gridSpacing = 100.0;  // 100m spacing keeps max distance under 250m
    uint32_t gridSize = (uint32_t)std::ceil(std::sqrt(numNodes));

    for (uint32_t i = 0; i < numNodes; ++i)
    {
        uint32_t row = i / gridSize;
        uint32_t col = i % gridSize;
        double x = 100.0 + col * gridSpacing;
        double y = 100.0 + row * gridSpacing;
        positionAlloc->Add(Vector(x, y, 0.0));
    }

    mobility.SetPositionAllocator(positionAlloc);

    // Use ConstantPositionMobilityModel for stable connectivity testing
    // Nodes stay in their initial grid positions, ensuring reliable links
    // This is ideal for link quality estimation without mobility effects
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    NS_LOG_UNCOND("Installing mobility model...");
    mobility.Install(nodes);
    NS_LOG_UNCOND("Mobility model installed");

    // Install Internet stack with AODV routing protocol
    // Configure AODV parameters optimized for VANET (high mobility scenario)
    AodvHelper aodv;

    // Enable HELLO messages for neighbor discovery
    aodv.Set("EnableHello", BooleanValue(true));
    aodv.Set("HelloInterval", TimeValue(Seconds(1.0)));  // Send HELLO every 1 second

    // Reduce timeouts for faster route recovery in high mobility
    aodv.Set("ActiveRouteTimeout", TimeValue(Seconds(3.0)));  // Route expires after 3s
    aodv.Set("AllowedHelloLoss", UintegerValue(2));  // Allow 2 HELLO losses before link break

    // Install Internet stack with AODV routing
    NS_LOG_UNCOND("Installing Internet stack with AODV...");
    InternetStackHelper internet;
    internet.SetRoutingHelper(aodv);
    internet.Install(nodes);
    NS_LOG_UNCOND("Internet stack installed");

    // Assign IP addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = ipv4.Assign(devices);

    // Setup traffic: UDP echo servers and clients
    uint16_t port = 9;
    
    // Install echo servers on first 5 nodes (or all nodes if less than 5)
    uint32_t numServers = std::min(5u, numNodes);
    for (uint32_t i = 0; i < numServers; ++i)
    {
        UdpEchoServerHelper server(port);
        ApplicationContainer serverApp = server.Install(nodes.Get(i));
        serverApp.Start(Seconds(1.0));
        serverApp.Stop(Seconds(simTime));
    }

    // Install echo clients on last nodes (if we have more than 5 nodes)
    if (numNodes > 5)
    {
        uint32_t numClients = std::min(5u, numNodes - 5);
        for (uint32_t i = 0; i < numClients; ++i)
        {
            uint32_t clientIdx = numNodes - 1 - i;
            uint32_t serverIdx = i % numServers;
            
            UdpEchoClientHelper client(interfaces.GetAddress(serverIdx), port);
            client.SetAttribute("MaxPackets", UintegerValue(static_cast<uint32_t>(simTime / packetInterval)));
            client.SetAttribute("Interval", TimeValue(Seconds(packetInterval)));
            client.SetAttribute("PacketSize", UintegerValue(packetSize));

            ApplicationContainer clientApp = client.Install(nodes.Get(clientIdx));
            clientApp.Start(Seconds(2.0 + i * 0.1));
            clientApp.Stop(Seconds(simTime));
        }
    }

    NS_LOG_UNCOND("About to create OpenGym interface...");

    // IMPORTANT: Create OpenGym interface FIRST, before environment
    NS_LOG_UNCOND("Creating OpenGym interface...");
    g_openGym = CreateObject<OpenGymInterface>(openGymPort);
    NS_LOG_UNCOND("OpenGym interface created successfully");

    // Then create the environment with nodes
    NS_LOG_UNCOND("Creating OpenGym environment (VanetLinkEnv)...");
    g_gymEnv = CreateObject<VanetLinkEnv>(nodes);
    NS_LOG_UNCOND("VanetLinkEnv created successfully");

    // Link them together
    g_gymEnv->SetOpenGymInterface(g_openGym);
    
    // Notify that we're ready
    NS_LOG_UNCOND("Starting simulation...");
    NS_LOG_UNCOND("Waiting for Python agent to connect on port " << openGymPort << "...");

    // Connect PHY layer traces for link quality monitoring
    // Do this AFTER environment is created
    // Track successful packet transmissions
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin",
                    MakeCallback(&PhyTxBeginTrace));
    // Track successful packet receptions
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/State/RxOk",
                    MakeCallback(&PhyRxOkTrace));
    // Track packet drops at PHY layer
    Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxDrop",
                    MakeCallback(&PhyTxDropTrace));

    // Schedule periodic state updates for OpenGym
    // Start after a small delay to let Python connect
    Simulator::Schedule(Seconds(0.5), &ScheduleNextStateRead, envStepTime, g_openGym);

    // Install flow monitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // Run simulation
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();

    // Print flow statistics
    NS_LOG_UNCOND("\n======================================");
    NS_LOG_UNCOND("        Flow Statistics");
    NS_LOG_UNCOND("======================================");
    
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

    double totalTxPackets = 0;
    double totalRxPackets = 0;
    double totalThroughput = 0;

    for (auto& flow : stats)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(flow.first);
        
        totalTxPackets += flow.second.txPackets;
        totalRxPackets += flow.second.rxPackets;
        
        double throughput = flow.second.rxBytes * 8.0 / simTime / 1024.0;  // kbps
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
    
    NS_LOG_UNCOND("Simulation completed successfully!");
    
    return 0;
}