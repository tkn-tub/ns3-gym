/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * VANET Link Quality Environment for ns3-gym
 */

#include "vanet-link-env.h"
#include "ns3/mobility-module.h"
#include "ns3/log.h"
#include <cmath>
#include <algorithm>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("VanetLinkEnv");

NS_OBJECT_ENSURE_REGISTERED(VanetLinkEnv);

VanetLinkEnv::VanetLinkEnv()
{
    NS_LOG_FUNCTION(this);
    m_observationSize = 0;
    m_reward = 0.0;
    m_gameOver = false;
}

VanetLinkEnv::VanetLinkEnv(NodeContainer nodes)
    : m_nodes(nodes)
{
    NS_LOG_FUNCTION(this);
    uint32_t numNodes = nodes.GetN();
    // 5 metrics per link pair (but not self-links)
    m_observationSize = numNodes * (numNodes - 1) * 5;
    m_currentObservation.resize(m_observationSize, 0.0);
    m_reward = 0.0;
    m_gameOver = false;
    
    NS_LOG_INFO("VanetLinkEnv created with " << numNodes << " nodes, observation size: " << m_observationSize);
}

VanetLinkEnv::~VanetLinkEnv()
{
    NS_LOG_FUNCTION(this);
}

TypeId
VanetLinkEnv::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::VanetLinkEnv")
                            .SetParent<OpenGymEnv>()
                            .SetGroupName("OpenGym")
                            .AddConstructor<VanetLinkEnv>();
    return tid;
}

void
VanetLinkEnv::DoDispose()
{
    NS_LOG_FUNCTION(this);
    m_nodes = NodeContainer();
    m_linkMetrics.clear();
    m_currentObservation.clear();
    m_routingDecisions.clear();

    // Call parent class DoDispose
    OpenGymEnv::DoDispose();
}

Ptr<OpenGymSpace>
VanetLinkEnv::GetActionSpace()
{
    NS_LOG_FUNCTION(this);
    // Action: Link quality class (0=disconnected, 1=transient, 2=connected)
    // Discrete action space with 3 possible values
    Ptr<OpenGymDiscreteSpace> space = CreateObject<OpenGymDiscreteSpace>(3);
    NS_LOG_INFO("GetActionSpace: Discrete(3)");
    return space;
}

Ptr<OpenGymSpace>
VanetLinkEnv::GetObservationSpace()
{
    NS_LOG_FUNCTION(this);
    // Observation: [RSSI, SNR, PacketLoss, Distance, RelativeSpeed] for each link
    float low = 0.0;
    float high = 1.0;  // All values normalized to [0, 1]
    std::vector<uint32_t> shape = {m_observationSize};
    std::string dtype = TypeNameGet<float>();
    Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace>(low, high, shape, dtype);
    NS_LOG_INFO("GetObservationSpace: Box(" << m_observationSize << ")");
    return space;
}

bool
VanetLinkEnv::GetGameOver()
{
    return m_gameOver;
}

Ptr<OpenGymDataContainer>
VanetLinkEnv::GetObservation()
{
    NS_LOG_FUNCTION(this);
    
    // Collect current link metrics
    CollectLinkMetrics();

    // Create box container
    std::vector<uint32_t> shape = {static_cast<uint32_t>(m_currentObservation.size())};
    Ptr<OpenGymBoxContainer<float>> box = CreateObject<OpenGymBoxContainer<float>>(shape);

    // Add observation values
    for (size_t i = 0; i < m_currentObservation.size(); ++i)
    {
        box->AddValue(static_cast<float>(m_currentObservation[i]));
    }

    NS_LOG_DEBUG("GetObservation: " << m_currentObservation.size() << " values");
    return box;
}

float
VanetLinkEnv::GetReward()
{
    float reward = m_reward;
    m_reward = 0.0;  // Reset reward after reading
    NS_LOG_DEBUG("GetReward: " << reward);
    return reward;
}

std::string
VanetLinkEnv::GetExtraInfo()
{
    std::string info = "LinkMetrics: " + std::to_string(m_linkMetrics.size()) + " links tracked";
    return info;
}

bool
VanetLinkEnv::ExecuteActions(Ptr<OpenGymDataContainer> action)
{
    NS_LOG_FUNCTION(this);
    
    Ptr<OpenGymDiscreteContainer> discrete = DynamicCast<OpenGymDiscreteContainer>(action);
    
    if (discrete)
    {
        int actionValue = discrete->GetValue();
        NS_LOG_INFO("Received routing decision: " << actionValue);
        
        // Store routing decision for potential AODV modification
        m_routingDecisions.clear();
        m_routingDecisions.push_back(actionValue);
        
        // Give small positive reward for taking action
        m_reward += 0.01;
    }
    else
    {
        NS_LOG_WARN("ExecuteActions: Invalid action container type");
        return false;
    }

    return true;
}

/**
 * \brief Collect link quality metrics for all vehicle-to-vehicle links
 *
 * This function is the core of the link quality estimation system. It collects
 * five critical metrics for each link between vehicles:
 * 1. RSSI (Received Signal Strength Indicator) - indicates signal power
 * 2. SNR (Signal-to-Noise Ratio) - indicates signal quality
 * 3. Packet Loss Rate - indicates link reliability
 * 4. Distance - physical separation between vehicles
 * 5. Relative Speed - how fast vehicles are moving relative to each other
 *
 * These metrics are normalized to [0, 1] range for neural network input.
 * The function is called periodically by the OpenGym interface to provide
 * observations to the Python ML agent.
 *
 * \return void - Results are stored in m_currentObservation vector
 */
void
VanetLinkEnv::CollectLinkMetrics()
{
    NS_LOG_FUNCTION(this);

    // Clear previous observations
    m_currentObservation.clear();

    uint32_t numNodes = m_nodes.GetN();

    if (numNodes == 0)
    {
        NS_LOG_WARN("No nodes in container");
        return;
    }

    // Iterate through all node pairs (excluding self-links)
    // For N nodes, we have N*(N-1) directed links
    for (uint32_t i = 0; i < numNodes; ++i)
    {
        Ptr<Node> node = m_nodes.Get(i);
        Ptr<MobilityModel> mob = node->GetObject<MobilityModel>();

        if (!mob)
        {
            NS_LOG_WARN("Node " << i << " has no mobility model");
            continue;
        }

        // Get current position and velocity of source node
        Vector pos = mob->GetPosition();
        Vector vel = mob->GetVelocity();

        // Check link quality to all other nodes
        for (uint32_t j = 0; j < numNodes; ++j)
        {
            if (i == j) continue;  // Skip self-links

            Ptr<Node> neighbor = m_nodes.Get(j);
            Ptr<MobilityModel> neighborMob = neighbor->GetObject<MobilityModel>();

            if (!neighborMob)
            {
                NS_LOG_WARN("Node " << j << " has no mobility model");
                continue;
            }

            // Get neighbor's position and velocity
            Vector neighborPos = neighborMob->GetPosition();
            Vector neighborVel = neighborMob->GetVelocity();

            // Calculate Euclidean distance between nodes
            double distance = mob->GetDistanceFrom(neighborMob);

            // Calculate relative velocity vector
            Vector relVel = Vector(vel.x - neighborVel.x, vel.y - neighborVel.y, vel.z - neighborVel.z);

            // Calculate relative speed (magnitude of relative velocity)
            double relSpeed = std::sqrt(relVel.x * relVel.x + relVel.y * relVel.y + relVel.z * relVel.z);

            // Retrieve link metrics from map (creates default entry if not exists)
            auto key = std::make_pair(i, j);
            LinkMetrics& metrics = m_linkMetrics[key];

            // ===== NORMALIZATION TO [0, 1] RANGE =====

            // RSSI normalization: [-100 dBm, -40 dBm] -> [0, 1]
            // -100 dBm = very weak signal (normalized to 0)
            // -40 dBm = very strong signal (normalized to 1)
            double rssiNorm = (metrics.rssi + 100.0) / 60.0;
            rssiNorm = std::max(0.0, std::min(1.0, rssiNorm));

            // SNR normalization: [0 dB, 30 dB] -> [0, 1]
            // 0 dB = poor quality (normalized to 0)
            // 30 dB = excellent quality (normalized to 1)
            double snrNorm = metrics.snr / 30.0;
            snrNorm = std::max(0.0, std::min(1.0, snrNorm));

            // Packet Loss: already in [0, 1] range
            // 0 = no loss (best), 1 = 100% loss (worst)
            double plNorm = std::max(0.0, std::min(1.0, metrics.packetLoss));

            // Distance normalization: [0m, 500m] -> [0, 1]
            // 0m = same location (normalized to 0)
            // 500m = max VANET range (normalized to 1)
            double distNorm = distance / 500.0;
            distNorm = std::max(0.0, std::min(1.0, distNorm));

            // Relative Speed normalization: [0 m/s, 50 m/s] -> [0, 1]
            // 0 m/s = same speed (normalized to 0)
            // 50 m/s = ~180 km/h relative (normalized to 1)
            double speedNorm = relSpeed / 50.0;
            speedNorm = std::max(0.0, std::min(1.0, speedNorm));

            // Add normalized metrics to observation vector in fixed order:
            // [RSSI, SNR, PacketLoss, Distance, RelativeSpeed]
            m_currentObservation.push_back(rssiNorm);
            m_currentObservation.push_back(snrNorm);
            m_currentObservation.push_back(plNorm);
            m_currentObservation.push_back(distNorm);
            m_currentObservation.push_back(speedNorm);
        }
    }

    NS_LOG_DEBUG("Collected " << m_currentObservation.size() << " observation values");
}

/**
 * \brief Callback function triggered when a packet is transmitted
 *
 * This function is called by the WiFi PHY layer trace (PhyTxBeginTrace) whenever
 * a node begins transmitting a packet. It updates the transmission counters for
 * link quality estimation.
 *
 * \param nodeId The ID of the node that transmitted the packet
 *
 * Purpose:
 * - Track packet transmissions for PDR (Packet Delivery Ratio) calculation
 * - Maintain accurate Tx counters for each link
 */
void
VanetLinkEnv::NotifyPacketTransmitted(uint32_t nodeId)
{
    NS_LOG_FUNCTION(this << nodeId);

    uint32_t numNodes = m_nodes.GetN();

    // Update transmission counters for all potential links from this node
    // Since we don't know the specific destination in broadcast/multicast scenarios,
    // we increment counters for all possible destinations
    for (uint32_t i = 0; i < numNodes; ++i)
    {
        if (i != nodeId)
        {
            // Link key: (source_node, destination_node)
            auto key = std::make_pair(nodeId, i);
            LinkMetrics& metrics = m_linkMetrics[key];

            // Increment transmission counter
            metrics.packetsTx++;

            // Update packet loss rate if we have reception data
            if (metrics.packetsTx > 0)
            {
                metrics.packetLoss = 1.0 - (static_cast<double>(metrics.packetsRx) /
                                           static_cast<double>(metrics.packetsTx));
            }
        }
    }
}

/**
 * \brief Callback function triggered when a packet is successfully received
 *
 * This function is called by the WiFi PHY layer trace (PhyRxOkTrace) whenever
 * a packet is successfully decoded and received. It updates link quality metrics
 * based on the physical layer measurements.
 *
 * \param nodeId The ID of the node that received the packet
 * \param rssi Received Signal Strength Indicator in dBm (typically -100 to -40)
 * \param snr Signal-to-Noise Ratio in dB (typically 0 to 30)
 *
 * Purpose:
 * - Track successful packet receptions for PDR (Packet Delivery Ratio) calculation
 * - Update RSSI and SNR values for link quality estimation
 * - Provide positive reward signal to the ML agent for good link performance
 */
void
VanetLinkEnv::NotifyPacketReceived(uint32_t nodeId, double rssi, double snr)
{
    NS_LOG_FUNCTION(this << nodeId << rssi << snr);

    uint32_t numNodes = m_nodes.GetN();

    // Update metrics for all potential links to this receiving node
    // Since we track at PHY layer, we update all possible incoming links
    // The actual sender will have the most accurate metrics
    for (uint32_t i = 0; i < numNodes; ++i)
    {
        if (i != nodeId)
        {
            // Link key: (source_node, destination_node)
            auto key = std::make_pair(i, nodeId);
            LinkMetrics& metrics = m_linkMetrics[key];

            // Update physical layer metrics
            metrics.rssi = rssi;  // Signal strength
            metrics.snr = snr;    // Signal quality
            metrics.packetsRx++;  // Increment received packet counter

            // Calculate Packet Loss Rate: PLR = 1 - (RxPackets / TxPackets)
            // Lower is better (0 = no loss, 1 = all packets lost)
            if (metrics.packetsTx > 0)
            {
                metrics.packetLoss = 1.0 - (static_cast<double>(metrics.packetsRx) /
                                           static_cast<double>(metrics.packetsTx));
            }

            // Record timestamp of last update for staleness tracking
            metrics.lastUpdate = Simulator::Now();

            // Provide positive reward for successful packet reception
            // This encourages the ML agent to prefer reliable links
            m_reward += 0.01;  // Reduced reward to avoid excessive accumulation
        }
    }
}

void
VanetLinkEnv::NotifyPacketLost(uint32_t nodeId)
{
    NS_LOG_FUNCTION(this << nodeId);
    
    uint32_t numNodes = m_nodes.GetN();
    
    // Update packet loss for all potential links from this node
    for (uint32_t i = 0; i < numNodes; ++i)
    {
        if (i != nodeId)
        {
            auto key = std::make_pair(nodeId, i);
            LinkMetrics& metrics = m_linkMetrics[key];
            
            metrics.packetsTx++;
            
            // Update packet loss rate
            if (metrics.packetsTx > 0)
            {
                metrics.packetLoss = 1.0 - (static_cast<double>(metrics.packetsRx) / 
                                           static_cast<double>(metrics.packetsTx));
            }

            // Negative reward for packet loss
            m_reward -= 0.05;
        }
    }
}

} // namespace ns3