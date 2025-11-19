/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * VANET Link Quality Environment for ns3-gym
 */

#ifndef VANET_LINK_ENV_H
#define VANET_LINK_ENV_H

#include "ns3/opengym-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include <vector>
#include <map>

namespace ns3 {

struct LinkMetrics {
    double rssi = -100.0;           // Received Signal Strength
    double snr = 0.0;               // Signal to Noise Ratio
    double packetLoss = 0.0;        // Packet loss rate
    double distance = 0.0;          // Distance between nodes
    double relativeSpeed = 0.0;     // Relative velocity
    uint32_t packetsRx = 0;
    uint32_t packetsTx = 0;
    Time lastUpdate = Seconds(0);
    
    LinkMetrics() = default;
};

class VanetLinkEnv : public OpenGymEnv
{
public:
    VanetLinkEnv();
    VanetLinkEnv(NodeContainer nodes);
    virtual ~VanetLinkEnv();
    static TypeId GetTypeId();
    virtual void DoDispose();

    // OpenGym interface
    virtual Ptr<OpenGymSpace> GetActionSpace();
    virtual Ptr<OpenGymSpace> GetObservationSpace();
    virtual bool GetGameOver();
    virtual Ptr<OpenGymDataContainer> GetObservation();
    virtual float GetReward();
    virtual std::string GetExtraInfo();
    virtual bool ExecuteActions(Ptr<OpenGymDataContainer> action);

    // VANET specific
    void CollectLinkMetrics();
    void SetNodes(NodeContainer nodes) { m_nodes = nodes; }
    void NotifyPacketTransmitted(uint32_t nodeId);
    void NotifyPacketReceived(uint32_t nodeId, double rssi, double snr);
    void NotifyPacketLost(uint32_t nodeId);

private:
    NodeContainer m_nodes;
    uint32_t m_observationSize;
    std::map<std::pair<uint32_t, uint32_t>, LinkMetrics> m_linkMetrics;
    std::vector<double> m_currentObservation;
    std::vector<int> m_routingDecisions;  // Routing decisions from NN
    double m_reward;
    bool m_gameOver;
};

} // namespace ns3

#endif /* VANET_LINK_ENV_H */