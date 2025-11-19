#!/bin/bash

# VANET AODV Neural Network Simulation Test Script
# This script helps run both NS-3 and Python agent smoothly

echo "========================================"
echo "VANET AODV NN Simulation Test"
echo "========================================"

# Clean up old processes
echo "Cleaning up old processes..."
pkill -f vanet-aodv-gym 2>/dev/null
pkill -f link_quality_agent 2>/dev/null
sleep 2

# Check port
echo "Checking port 5555..."
if lsof -i :5555 > /dev/null 2>&1; then
    echo "WARNING: Port 5555 is in use!"
    echo "Killing process on port 5555..."
    lsof -t -i :5555 | xargs kill -9 2>/dev/null
    sleep 2
fi

echo "✓ Port 5555 is free"
echo ""
echo "========================================"
echo "INSTRUCTIONS:"
echo "========================================"
echo ""
echo "Terminal 1 (NS-3 Simulation):"
echo "  cd /home/uzair/Desktop/Hamida/ns-3"
echo ""
echo "  ./build/contrib/opengym/examples/ns3.40-vanet-aodv-gym-default \\"
echo "      --numNodes=10 \\"
echo "      --simTime=30 \\"
echo "      --nodeSpeed=20 \\"
echo "      --openGymPort=5555"
echo ""
echo "  Wait for: 'Waiting for Python process to connect...'"
echo ""
echo "----------------------------------------"
echo ""
echo "Terminal 2 (Python Agent) - Start 5 seconds AFTER NS-3:"
echo "  cd /home/uzair/Desktop/Hamida/ns-3/contrib/opengym/examples/vanet-aodv-nn"
echo "  source ../../ns3gym-venv/bin/activate"
echo ""
echo "  python3 link_quality_agent.py --port=5555 --steps=1000"
echo ""
echo "========================================"
echo ""
echo "Press Ctrl+C in both terminals to stop"
echo "========================================"
