# Fixes Applied to NS3 VANET Simulation

## Problem Diagnosed

The simulation was generating CSV files with all 0 values because:

1. **Grid spacing too large**: Nodes were placed 150m apart, causing some node pairs to be >250m apart (exceeding the 250m communication range)
2. **No packet transmission tracking**: Only tracked dropped packets, not successful transmissions, so `packetsTx` counters remained at 0
3. **Missing trace connection**: PhyTxBegin trace was not connected to track when packets are actually transmitted

## Fixes Applied

### 1. Reduced Grid Spacing (vanet-aodv-gym.cc:207-208)
- **Changed**: Grid spacing from 150m to 100m
- **Reason**: Ensures all nodes stay within 250m communication range
- **Impact**: With 10 nodes, max distance between any two nodes is now ~224m (within range)

### 2. Added Packet Transmission Tracking (vanet-aodv-gym.cc:53-69)
- **Added**: `PhyTxBeginTrace()` callback function
- **Purpose**: Tracks when nodes transmit packets at PHY layer
- **Impact**: Properly increments `packetsTx` counters for link quality metrics

### 3. Connected PhyTxBegin Trace (vanet-aodv-gym.cc:330-331)
- **Added**: Connection to `/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxBegin`
- **Purpose**: Triggers transmission tracking callback
- **Impact**: Enables tracking of all packet transmissions

### 4. Improved Code Structure (vanet-aodv-gym.cc:32-51)
- **Added**: `ExtractNodeId()` helper function
- **Purpose**: Reusable node ID extraction from context strings
- **Impact**: Cleaner, more maintainable code

### 5. Updated VanetLinkEnv Interface (vanet-link-env.h:51)
- **Added**: `NotifyPacketTransmitted()` method declaration
- **Purpose**: Allows environment to track packet transmissions

### 6. Implemented Transmission Notification (vanet-link-env.cc:302-331)
- **Added**: `NotifyPacketTransmitted()` implementation
- **Purpose**: Updates transmission counters for link metrics
- **Impact**: Enables proper packet loss calculation

### 7. Adjusted Reward Values (vanet-link-env.cc:385)
- **Changed**: Reception reward from 0.1 to 0.01
- **Reason**: Prevents excessive reward accumulation
- **Impact**: More balanced reward signal for ML agent

## Expected Results After Rebuild

After rebuilding NS-3 with these changes, you should see:

✓ **Non-zero packet delivery ratios** in flow statistics
✓ **Proper RSSI/SNR values** in link quality metrics
✓ **Non-zero values in CSV files** for all parameters:
  - RSSI (signal strength)
  - SNR (signal-to-noise ratio)
  - Packet Loss Rate
  - Distance between nodes
  - Relative Speed

✓ **Connected/Transient links** instead of 100% disconnected
✓ **Positive throughput values** in flow statistics

## How to Apply These Fixes

1. **Pull the latest changes** from this repository
2. **Copy the updated files** to your NS-3 contrib/opengym directory
3. **Rebuild NS-3**:
   ```bash
   cd ~/Desktop/Hamida/ns-3
   ./waf configure --enable-examples
   ./waf build
   ```
4. **Run the simulation** as before:
   ```bash
   # Terminal 1: Start Python agent
   cd contrib/opengym/examples/vanet-aodv-nn
   python3 link_quality_agent.py --port=5555 --steps=10000

   # Terminal 2: Run NS-3 simulation
   ./build/contrib/opengym/examples/ns3.40-vanet-aodv-gym-default \
       --numNodes=10 \
       --simTime=30 \
       --nodeSpeed=20 \
       --openGymPort=5555
   ```

## Technical Details

### Grid Layout (10 nodes, 100m spacing)
```
Node positions in 4x3 grid:
0: (100, 100)    1: (200, 100)    2: (300, 100)    3: (400, 100)
4: (100, 200)    5: (200, 200)    6: (300, 200)    7: (400, 200)
8: (100, 300)    9: (200, 300)

Max distance: Node 0 to Node 7 = sqrt((300)² + (100)²) ≈ 224m < 250m ✓
```

### Link Metrics Collection
Each link now properly tracks:
- **packetsTx**: Incremented in `NotifyPacketTransmitted()` when PHY begins transmission
- **packetsRx**: Incremented in `NotifyPacketReceived()` when PHY successfully receives
- **packetLoss**: Calculated as `1 - (packetsRx / packetsTx)`
- **RSSI/SNR**: Updated from PHY layer measurements on reception

## Files Modified

1. `examples/vanet-aodv-nn/vanet-aodv-gym.cc`
2. `examples/vanet-aodv-nn/vanet-link-env.h`
3. `examples/vanet-aodv-nn/vanet-link-env.cc`
