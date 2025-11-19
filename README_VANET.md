# VANET AODV Neural Network - NS-3 OpenGym Integration

[![NS-3 Version](https://img.shields.io/badge/NS--3-3.40-blue.svg)](https://www.nsnam.org/)
[![Python](https://img.shields.io/badge/Python-3.8+-green.svg)](https://www.python.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A complete **Vehicular Ad-hoc Network (VANET)** simulation using NS-3.40 with OpenGym integration for Neural Network-based link quality estimation. This project implements AODV routing with real-time RSSI/SNR measurements and machine learning for intelligent routing decisions.

## 🚀 Features

### Core Functionality
- ✅ **VANET Simulation** with Vehicle-to-Vehicle (V2V) and Vehicle-to-Infrastructure (V2I) communication
- ✅ **AODV Routing Protocol** optimized for mobile scenarios
- ✅ **RandomWaypoint Mobility Model** for realistic vehicle movement
- ✅ **Grid-based RSU Placement** for optimal infrastructure coverage
- ✅ **Real RSSI/SNR Measurements** using MonitorSnifferRx trace callbacks
- ✅ **Neural Network Integration** via ns3-gym for link quality prediction
- ✅ **NetAnim Visualization** for network animation

### Enhanced Monitoring
- 📊 **Multi-layer Statistics**: PHY, MAC, and Queue monitoring
- 📊 **Smart Packet Filtering**: Separate data and control traffic
- 📊 **Congestion Detection**: Per-node queue drop tracking
- 📊 **Collision Tracking**: MAC layer collision counters
- 📊 **Validated Metrics**: RSSI (-100 to -30 dBm), SNR (0-40 dB) range validation
- 📊 **Comprehensive Reports**: PDR, PLR, throughput, and per-flow statistics

### Dataset Generation
- 📁 **CSV Export**: RSSI, SNR, PacketLoss, Distance, RelativeSpeed
- 📁 **Real-time Collection**: Link metrics collected every 100ms
- 📁 **ML-Ready Format**: Compatible with PyTorch/TensorFlow training

## 📋 Table of Contents

- [Architecture](#architecture)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Project Structure](#project-structure)
- [Configuration](#configuration)
- [Output Files](#output-files)
- [Troubleshooting](#troubleshooting)
- [Performance Optimization](#performance-optimization)
- [Contributing](#contributing)
- [Citation](#citation)
- [License](#license)

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    VANET Simulation Layer                    │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐            │
│  │  Vehicles  │  │    RSUs    │  │   Server   │            │
│  │  (Mobile)  │  │  (Static)  │  │   (CSMA)   │            │
│  └─────┬──────┘  └─────┬──────┘  └─────┬──────┘            │
│        │                │                │                    │
│        └────────────────┴────────────────┘                    │
│                         │                                     │
│              ┌──────────▼──────────┐                         │
│              │   AODV Routing      │                         │
│              │   WiFi 802.11p      │                         │
│              │   Friis Propagation │                         │
│              └──────────┬──────────┘                         │
└─────────────────────────┼─────────────────────────────────────┘
                          │
┌─────────────────────────▼─────────────────────────────────────┐
│              NS-3 OpenGym Interface (ZMQ)                      │
└─────────────────────────┬─────────────────────────────────────┘
                          │
┌─────────────────────────▼─────────────────────────────────────┐
│                  Python ML Agent                               │
│  ┌──────────────────────────────────────────────────┐         │
│  │          Neural Network (PyTorch)                │         │
│  │  Input: [RSSI, SNR, Loss, Distance, RelSpeed]   │         │
│  │  Output: Link Quality [0=Bad, 1=Fair, 2=Good]   │         │
│  └──────────────────────────────────────────────────┘         │
└───────────────────────────────────────────────────────────────┘
```

## 💻 Installation

### Prerequisites

- **OS**: Ubuntu 20.04+ or Debian-based Linux
- **GCC**: 9.0+
- **CMake**: 3.10+
- **Python**: 3.8+
- **NS-3**: Version 3.40

### Option 1: Automated Installation (Recommended)

```bash
# Download installation script
wget https://raw.githubusercontent.com/Muhammaduazir69/ns3-gym/app-ns-3.36+/install_vanet_ns3.sh

# Make executable and run
chmod +x install_vanet_ns3.sh
./install_vanet_ns3.sh
```

### Option 2: Manual Installation

#### 1. Install System Dependencies

```bash
sudo apt update
sudo apt install -y build-essential gcc g++ cmake git python3 python3-dev \
    python3-pip python3-venv libboost-all-dev libxml2-dev libzmq3-dev \
    libprotobuf-dev protobuf-compiler qtbase5-dev
```

#### 2. Download and Build NS-3.40

```bash
cd ~/Desktop
mkdir -p Hamida && cd Hamida
wget https://www.nsnam.org/releases/ns-allinone-3.40.tar.bz2
tar -xjf ns-allinone-3.40.tar.bz2
cd ns-allinone-3.40/ns-3.40
./ns3 configure --enable-examples --enable-tests
./ns3 build
```

#### 3. Clone ns3-gym with VANET Example

```bash
cd contrib/
git clone https://github.com/Muhammaduazir69/ns3-gym.git opengym
cd opengym
protoc -I=./model/messages --cpp_out=./model/messages ./model/messages/*.proto
```

#### 4. Rebuild NS-3 with OpenGym

```bash
cd ../../
./ns3 clean
./ns3 configure --enable-examples --enable-tests
./ns3 build
```

#### 5. Setup Python Environment

```bash
cd contrib/opengym/
python3 -m venv ns3gym-venv
source ns3gym-venv/bin/activate
pip install --upgrade pip

# Install compatible packages
pip install numpy==1.26.4 protobuf==3.20.3 gym==0.26.2
pip install matplotlib==3.8.3 pandas==2.3.2
pip install torch==2.1.0 torchvision==0.16.0 --index-url https://download.pytorch.org/whl/cpu

# Install ns3gym bindings
cd ns3gym_bindings
pip install -e .
```

## 🚀 Quick Start

### 1. Activate Python Environment

```bash
cd ~/Desktop/Hamida/ns-allinone-3.40/ns-3.40/contrib/opengym
source ns3gym-venv/bin/activate
```

### 2. Navigate to Project

```bash
cd examples/vanet-aodv-nn/
```

### 3. Run Simulation

```bash
# Basic simulation (50 seconds, 10 nodes)
python3 link_quality_agent.py --simTime=50 --numNodes=10

# Quick test (5 seconds, 6 nodes)
python3 link_quality_agent.py --simTime=5 --numNodes=6

# Large-scale test (100 seconds, 16 nodes)
python3 link_quality_agent.py --simTime=100 --numNodes=16
```

### 4. View Results

```bash
# Check generated CSV dataset
ls -lh vanet_link_dataset_*.csv

# View NetAnim visualization
cd ~/Desktop/Hamida/ns-allinone-3.40/
./netanim/NetAnim  # Open vanet-aodv-gym.xml
```

## 📁 Project Structure

```
contrib/opengym/examples/vanet-aodv-nn/
├── vanet-aodv-gym.cc           # Main NS-3 simulation (C++)
├── vanet-link-env.cc           # OpenGym environment implementation
├── vanet-link-env.h            # Environment header
├── link_quality_agent.py       # Python ML agent
├── .gitignore                  # Git ignore patterns
└── README.md                   # This file

Generated Files:
├── vanet_link_dataset_*.csv    # Link quality measurements
├── link_quality_model_*.pth    # Trained PyTorch models
└── vanet-aodv-gym.xml          # NetAnim visualization
```

## ⚙️ Configuration

### Simulation Parameters

| Parameter | Default | Range | Description |
|-----------|---------|-------|-------------|
| `--numNodes` | 10 | 6-20 | Number of vehicle nodes |
| `--simTime` | 50 | 5-200 | Simulation duration (seconds) |
| `--openGymPort` | 5555 | 1024-65535 | OpenGym communication port |
| `--enableNetAnim` | 1 | 0-1 | Enable/disable animation |

### Network Configuration (in C++ code)

```cpp
// Area size (meters)
double areaWidth = 300.0;
double areaHeight = 300.0;

// Vehicle mobility
double minSpeed = 10.0;  // m/s
double maxSpeed = 20.0;  // m/s

// WiFi parameters
double txPower = 23.0;   // dBm
uint32_t channelWidth = 10; // MHz

// RSU placement
uint32_t numRSUs = 4;
double rsuSpacing = 75.0; // meters
```

### Neural Network Configuration

```python
# Network architecture
input_size = 5  # RSSI, SNR, PacketLoss, Distance, RelSpeed
hidden_size = 64
output_classes = 3  # Bad, Fair, Good

# Training parameters
learning_rate = 0.001
batch_size = 32
epochs = 50
```

## 📊 Output Files

### CSV Dataset Format

```csv
RSSI,SNR,PacketLoss,Distance,RelSpeed,LinkQuality
-65.3,25.8,0.15,145.2,15.3,2
-78.1,18.2,0.35,280.5,22.7,1
-92.5,8.5,0.85,450.8,5.2,0
```

**Columns:**
- `RSSI`: Received Signal Strength Indicator (dBm)
- `SNR`: Signal-to-Noise Ratio (dB)
- `PacketLoss`: Packet loss rate [0.0-1.0]
- `Distance`: Distance between nodes (meters)
- `RelSpeed`: Relative speed (m/s)
- `LinkQuality`: Classification (0=Disconnected, 1=Transient, 2=Connected)

### Console Output Example

```
======================================
Flow Statistics (First 4 Flows Only):
======================================
Flow 1 (V2V): 192.168.1.1 -> 192.168.1.2
  Tx Packets: 245
  Rx Packets: 208
  PDR: 84.89%
  Throughput: 42.35 kbps

========== ENHANCED NETWORK STATISTICS ==========
PHY Layer Metrics:
  Total Packets Transmitted: 2845
  Total Packets Received: 2312
  Packet Delivery Ratio (PDR): 81.26%
  Packet Loss Ratio (PLR): 18.74%

MAC Layer Metrics:
  Total MAC Collisions: 127

Queue Congestion (per node):
  Node 2: 15 queue drops
  Node 5: 8 queue drops
==================================================
```

## 🔧 Troubleshooting

### Common Issues

#### 1. "No module named ns3gym" Error
```bash
source ~/Desktop/Hamida/ns-allinone-3.40/ns-3.40/contrib/opengym/ns3gym-venv/bin/activate
pip install -e ~/Desktop/Hamida/ns-allinone-3.40/ns-3.40/contrib/opengym/ns3gym_bindings/
```

#### 2. Build Errors
```bash
cd ~/Desktop/Hamida/ns-allinone-3.40/ns-3.40/
./ns3 clean
./ns3 configure --enable-examples
./ns3 build
```

#### 3. Port Already in Use
```bash
killall -9 ns3.40-vanet-aodv-gym-default
# Or use different port
python3 link_quality_agent.py --openGymPort=5556
```

#### 4. CSV Shows All Zeros
✅ **FIXED** in this version! The updated code uses `MonitorSnifferRx` trace for real RSSI/SNR values.

#### 5. NumPy Compatibility Issues
```bash
pip uninstall numpy
pip install numpy==1.26.4
```

## 🚀 Performance Optimization

### For Faster Simulations
```bash
# Disable NetAnim
--enableNetAnim=0

# Reduce nodes and time
--simTime=10 --numNodes=6
```

### For Better Dataset Quality
```bash
# Longer simulation with more nodes
--simTime=200 --numNodes=16

# Multiple runs for diverse data
for i in {1..10}; do
    python3 link_quality_agent.py --simTime=100 --numNodes=12
done
```

### For Resource-Constrained Systems
- Reduce `envStepTime` from 0.1 to 0.5 seconds (in C++ code)
- Lower `numVehicles` to 6
- Disable comprehensive statistics printing

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines
- Follow NS-3 coding style
- Add comments for complex logic
- Test with multiple node configurations
- Update documentation for new features

## 📚 Citation

If you use this code in your research, please cite:

```bibtex
@software{vanet_ns3_gym_2024,
  title={VANET AODV Neural Network with NS-3 OpenGym Integration},
  author={Uzair, Muhammad},
  year={2024},
  url={https://github.com/Muhammaduazir69/ns3-gym},
  note={NS-3.40 implementation with enhanced monitoring}
}
```

## 📖 References

- [NS-3 Documentation](https://www.nsnam.org/documentation/)
- [ns3-gym Original Paper](https://arxiv.org/abs/1810.03943)
- [AODV RFC 3561](https://tools.ietf.org/html/rfc3561)
- [IEEE 802.11p Standard](https://ieeexplore.ieee.org/document/5514475)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- NS-3 Development Team
- ns3-gym authors (TU Berlin)
- OpenAI Gym framework
- VANET research community

## 📧 Contact

**Muhammad Uzair**
- GitHub: [@Muhammaduazir69](https://github.com/Muhammaduazir69)
- Repository: [ns3-gym](https://github.com/Muhammaduazir69/ns3-gym)

## 🔄 Version History

### v2.0.0 (2024-11-17) - Current
- ✅ Fixed critical node ID extraction bug
- ✅ Real RSSI/SNR measurements using MonitorSnifferRx
- ✅ Multi-layer network monitoring (PHY + MAC + Queue)
- ✅ Smart packet filtering
- ✅ Enhanced statistics reporting
- ✅ Validated metric ranges
- ✅ Added comprehensive documentation

### v1.0.0 (Initial Release)
- Basic VANET simulation with AODV
- OpenGym integration
- Neural network link quality estimation

---

**⭐ Star this repository if you find it helpful!**

**🐛 Report issues:** [GitHub Issues](https://github.com/Muhammaduazir69/ns3-gym/issues)
