# VANET AODV with Neural Network Link Quality Estimation

## Overview
This project implements a VANET (Vehicular Ad-hoc Network) simulation with AODV routing protocol enhanced by neural network-based link quality estimation. The system classifies vehicle-to-vehicle links into three categories:

- **Connected (2)**: Stable, high-quality link
- **Transient (1)**: Moderate quality, potentially unstable
- **Disconnected (0)**: Poor quality or out of range

## Features
- ✓ Real-time ns3-gym integration with NS-3.40
- ✓ AODV routing protocol with V2V communication
- ✓ Neural network-based link quality classifier (PyTorch)
- ✓ Automatic dataset generation for training
- ✓ Comprehensive metrics: RSSI, SNR, Packet Loss, Distance, Relative Velocity
- ✓ Flow monitoring and performance statistics

## Installation

### Prerequisites
1. **NS-3.40** with **ns3-gym** (opengym module)
2. **Python 3.7+** with virtual environment
3. **PyTorch** and **NumPy**

### File Placement

Place the files in your NS-3 directory structure:

```bash
# Main simulation files
ns-3/contrib/opengym/examples/
├── vanet-aodv-gym.cc          # Main NS-3 simulation
├── vanet-link-env.h           # OpenGym environment header
└── vanet-link-env.cc          # OpenGym environment implementation

# Python agent
ns-3/contrib/opengym/examples/vanet-aodv-nn/
└── link_quality_agent.py      # Python RL agent
```

### Building NS-3 Component

```bash
cd /path/to/ns-3

# Configure and build
./ns3 configure --enable-examples
./ns3 build

# Verify the build
ls build/contrib/opengym/examples/ | grep vanet-aodv-gym
```

## Running the Simulation

### Step 1: Activate Python Virtual Environment

```bash
cd /path/to/ns-3/contrib/opengym
source ns3gym-venv/bin/activate
cd examples/vanet-aodv-nn
```

### Step 2: Start NS-3 Simulation (Terminal 1)

```bash
cd /path/to/ns-3

# Basic run
./build/contrib/opengym/examples/ns3.40-vanet-aodv-gym-default \
    --numNodes=20 \
    --simTime=100 \
    --openGymPort=5555

# With custom parameters
./build/contrib/opengym/examples/ns3.40-vanet-aodv-gym-default \
    --numNodes=30 \
    --simTime=200 \
    --nodeSpeed=25 \
    --openGymPort=5555 \
    --verbose=true
```

### Step 3: Start Python Agent (Terminal 2)

```bash
cd /path/to/ns-3/contrib/opengym
source ns3gym-venv/bin/activate
cd examples/vanet-aodv-nn

# Basic run
python3 link_quality_agent.py --port=5555 --steps=10000

# With custom parameters
python3 link_quality_agent.py \
    --port=5555 \
    --steps=20000 \
    --no-collect  # Disable dataset collection
```

## Command Line Options

### NS-3 Simulation Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `--numNodes` | uint32 | 20 | Number of vehicles in simulation |
| `--simTime` | double | 100.0 | Simulation duration (seconds) |
| `--envStepTime` | double | 0.1 | OpenGym environment step interval |
| `--openGymPort` | uint32 | 5555 | Port for ns3-gym communication |
| `--nodeSpeed` | double | 20.0 | Maximum vehicle speed (m/s, ~72 km/h) |
| `--verbose` | bool | false | Enable detailed logging |

### Python Agent Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `--port` | int | 5555 | Port number for ns3-gym |
| `--steps` | int | 10000 | Maximum training steps |
| `--no-collect` | flag | false | Disable dataset collection |

## Output Files

### 1. Dataset CSV
**Filename**: `vanet_link_dataset_YYYYMMDD_HHMMSS.csv`

Format:
```csv
RSSI,SNR,PacketLoss,Distance,RelSpeed,LinkQuality
0.85,0.75,0.05,120.5,15.2,2
0.45,0.40,0.35,280.3,25.8,1
0.15,0.10,0.80,450.2,18.5,0
```

### 2. Trained Model
**Filename**: `link_quality_model_YYYYMMDD_HHMMSS.pth`

PyTorch model checkpoint containing:
- Model state dictionary
- Optimizer state
- Training statistics
- Episode rewards

## Architecture

### NS-3 C++ Components

**VanetLinkEnv** (`vanet-link-env.h/cc`):
- OpenGym environment implementation
- Real-time link metrics collection
- Observation space: [RSSI, SNR, PacketLoss, Distance, RelSpeed] per link
- Action space: Discrete(3) - link quality classification
- Reward function based on packet delivery and link stability

**Main Simulation** (`vanet-aodv-gym.cc`):
- VANET scenario with AODV routing
- 802.11p-like WiFi configuration (802.11a with proper settings)
- Random waypoint mobility model
- UDP echo server/client traffic
- Flow monitoring and statistics

### Python Components

**LinkQualityNN**:
- 4-layer fully connected neural network
- Input: 5 features per link
- Output: 3 classes (Disconnected, Transient, Connected)
- Dropout regularization (0.3)

**LinkQualityEstimator**:
- Real-time link classification
- Online training during simulation
- Dataset collection and export
- Statistics tracking

## Troubleshooting

### Error: `NS_ASSERT failed ... type-id.cc`

**Cause**: TypeId registration issue

**Solutions**:
1. Ensure `ns3::` prefix in TypeId:
   ```cpp
   TypeId("ns3::VanetLinkEnv")  // Correct
   TypeId("VanetLinkEnv")       // Wrong
   ```

2. Check header includes are correct
3. Verify `NS_OBJECT_ENSURE_REGISTERED(VanetLinkEnv)` is present
4. Rebuild NS-3 completely:
   ```bash
   ./ns3 clean
   ./ns3 configure --enable-examples
   ./ns3 build
   ```

### Error: `Cannot connect to ns3-gym`

**Cause**: Port mismatch or NS-3 not running

**Solutions**:
1. Start NS-3 simulation FIRST
2. Ensure port numbers match (default: 5555)
3. Check no other process is using the port:
   ```bash
   netstat -tuln | grep 5555
   ```
4. Try a different port number

### Error: `ImportError: No module named 'ns3gym'`

**Cause**: Virtual environment not activated or ns3-gym not installed

**Solutions**:
1. Activate virtual environment:
   ```bash
   source ns3gym-venv/bin/activate
   ```
2. Reinstall ns3-gym:
   ```bash
   pip install --upgrade ns3-gym
   ```

### Warning: Empty Observations

**Cause**: Simulation hasn't started collecting data yet

**Solution**: Wait a few seconds - this is normal at the start

### Low Packet Delivery Ratio

**Possible Causes**:
- Node density too low
- Nodes moving too fast
- Transmission power too low

**Solutions**:
1. Increase number of nodes: `--numNodes=30`
2. Reduce node speed: `--nodeSpeed=15`
3. Adjust WiFi parameters in the code

## Performance Metrics

The simulation reports the following metrics:

1. **Packet Delivery Ratio (PDR)**: Percentage of successfully delivered packets
2. **Throughput**: Data rate in kbps per flow
3. **Link Classifications**: Distribution of link quality categories
4. **Training Loss**: Neural network training progress

## Example Output

```
======================================
VANET AODV Neural Network Simulation
======================================
Nodes: 20
Simulation Time: 100s
OpenGym Port: 5555
Max Speed: 20 m/s
======================================

Starting simulation...
Waiting for Python agent to connect on port 5555...

[Python agent connects]

Step 100: Training loss = 0.8234
============================================================
Step 100/10000 | Reward: 45.30
Link Classifications:
  Connected:    423 (42.3%)
  Transient:    389 (38.9%)
  Disconnected: 188 (18.8%)
============================================================

...

======================================
        Flow Statistics
======================================
Flow 1 (10.1.1.6 -> 10.1.1.1)
  Tx Packets:   1000
  Rx Packets:   856
  PDR:          85.6 %
  Throughput:   69.8 kbps

...

Overall Statistics:
  Total Tx:     5000 packets
  Total Rx:     4234 packets
  Overall PDR:  84.68 %
  Total Throughput: 345.2 kbps
======================================
```

## Research Applications

This framework can be used for:

1. **Link Quality Prediction**: Train ML models to predict link stability
2. **Adaptive Routing**: Modify AODV routing decisions based on predicted link quality
3. **Network Optimization**: Study the impact of mobility patterns on network performance
4. **Dataset Generation**: Create labeled datasets for supervised learning
5. **Reinforcement Learning**: Implement RL agents for routing optimization

## Citation

If you use this code in your research, please cite:

```bibtex
@misc{vanet_aodv_nn,
  title={VANET AODV with Neural Network Link Quality Estimation},
  author={Your Name},
  year={2024},
  note={NS-3.40 with ns3-gym implementation}
}
```

## License

This project is provided for academic and research purposes.

## Support

For issues and questions:
1. Check the troubleshooting section
2. Verify NS-3 and ns3-gym installation
3. Review NS-3 documentation: https://www.nsnam.org/
4. Check ns3-gym documentation: https://github.com/tkn-tub/ns3-gym

## Contributing

Contributions are welcome! Areas for improvement:
- Additional routing protocols (OLSR, DSR)
- More sophisticated neural network architectures
- Integration with SUMO for realistic mobility
- Real-time visualization
- Multi-agent reinforcement learning

---

**Last Updated**: November 2024  
**NS-3 Version**: 3.40  
**Python Version**: 3.7+